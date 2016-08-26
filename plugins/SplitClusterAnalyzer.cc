#include "SplitClusterAnalyzer.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG

#pragma message("Recompiling SplitClusterAnalyzer.cc...")

SplitClusterAnalyzer::SplitClusterAnalyzer(edm::ParameterSet const& iConfig)
{
	// Tokens
	rawDataErrorToken        = consumes<edm::DetSetVector<SiPixelRawDataError>>(edm::InputTag("siPixelDigis"));
	digiFlagsToken           = consumes<edm::DetSetVector<PixelDigi>>          (edm::InputTag("dcolLostNeighbourDigiFlags"));
	clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>  (edm::InputTag("siPixelClusters"));
	trajTrackCollectionToken = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));
}

SplitClusterAnalyzer::~SplitClusterAnalyzer() {}

void SplitClusterAnalyzer::beginJob()
{
	// Override output filename
	if(iConfig.exists("fileName"))
	{
		ntupleOutputFilename = iConfig.getParameter<std::string>("filename");
	}
	// Create output file
	ntupleOutputFile = new TFile(ntupleOutputFilename.c_str(), "RECREATE");
	if(!(ntupleOutputFile -> IsOpen()))
	{
		handleDefaultError("file_operations", "file_operations", {"Failed to open output file: ", ntupleOutputFilename});
	}
	LogDebug("file_operations") << "Output file: \"" << ntupleOutputFilename << "\" created." << std::endl;
	// Tree definitions
	eventTree = new TTree("eventTree", "Event informations");
	EventDataTree::defineEventTreeBranches(eventTree, eventField);
	clusterTree = new TTree("clustTree", "Pixel clusters");
	ClusterDataTree::defineClusterTreeBranches(clusterTree, clusterField);
	mergeTree   = new TTree("mergeTree", "Cluster merging info");
	MergingStatisticsTree::defineMergingStatTreeBranches(mergeTree, mergeStatField);
	mergeSnapshotTree = new TTree("mergeSnapshotTree", "Cluster merging snapshots");
	MergingStatisticsTree::defineMergingSnapshotTreeBranches(mergeSnapshotTree, mergeSnapshotField);
	pixelTree = new TTree("pixelTree", "Pixel data");
	PixelDataTree::definePixelTreeBranches(pixelTree, pixelField, eventField);
}

void SplitClusterAnalyzer::endJob()
{
	LogDebug("file_operations") << "Writing plots to file: \"" << ntupleOutputFilename << "\"." << std::endl;
	ntupleOutputFile -> Write();
	ntupleOutputFile -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << c_blue << ntupleOutputFilename << c_def << "\"." << std::endl;
}

void SplitClusterAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	// Accessing the tracker topology for module informations
	edm::ESHandle<TrackerTopology> trackerTopologyHandle;
	iSetup.get<TrackerTopologyRcd>().get(trackerTopologyHandle);
	const TrackerTopology* const trackerTopology = trackerTopologyHandle.product();
	// FED errors
	std::map<uint32_t, int> fedErrors = FedErrorFetcher::getFedErrors(iEvent, rawDataErrorToken);
	// Fetching the markers for digis that have a dcol lost digi next to them
	edm::Handle<edm::DetSetVector<PixelDigi>> digiFlagsCollection;
	iEvent.getByToken(digiFlagsToken,         digiFlagsCollection);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollection;
	iEvent.getByToken(clustersToken,                  clusterCollection);
	// Fetching the tracks by token
	edm::Handle<TrajTrackAssociationCollection> trajTrackCollection;
	iEvent.getByToken(trajTrackCollectionToken, trajTrackCollection);
	// Trying to access the clusters
	// if(!dcolLostDigiCollection.isValid()) handleDefaultError("data access", "data_access", "Failed to fetch dcol lost digis.");
	if(!clusterCollection.isValid())      handleDefaultError("data access", "data_access", "Failed to fetch clusters.");
	if(!trajTrackCollection.isValid())    handleDefaultError("data access", "data_access", "Failed to fetch trajectory measurements.");
	// Setting up data fields
	EventDataTree::setEventTreeDataFields(eventTree, eventField);
	eventField.init();
	ClusterDataTree::setClusterTreeDataFields(clusterTree, clusterField);
	clusterField.init();
	MergingStatisticsTree::setMergingStatTreeDataFields(mergeTree, mergeStatField);
	mergeStatField.init();
	MergingStatisticsTree::setMergingSnapshotTreeDataFields(mergeSnapshotTree, mergeSnapshotField);
	for(int& i: mergeSnapshotField)
	{
		i = 0;
	}
	PixelDataTree::setPixelTreeDataFields(pixelTree, pixelField, eventField);
	pixelField.init();
	// Processing data
	handleEvent(iEvent);
	// Preparing a trajectory to closest cluster map
	TrajClusterMap trajClosestClustMap;
	trajClosestClustMap = getTrajClosestClusterMap(trajTrackCollection, clusterCollection, trackerTopology);
	handleClusters(clusterCollection, digiFlagsCollection, trackerTopology, fedErrors);
	clearAllContainers();
}

SplitClusterAnalyzer::TrajClusterMap SplitClusterAnalyzer::getTrajClosestClusterMap(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollection, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology)
{
	auto mapComparator = [] (const TrajectoryMeasurement& lhs, const TrajectoryMeasurement& rhs)
	{
		return lhs.estimate() < rhs.estimate();
	};
	TrajClusterMap trajClosestClustMap(mapComparator);
	for(const auto& currentTrackKeypair: *trajTrackCollection)
	{
		// Trajectory segments + corresponding track informations
		const edm::Ref<std::vector<Trajectory>> traj = currentTrackKeypair.key;
		const reco::TrackRef track                   = currentTrackKeypair.val; // TrackRef is actually a pointer type
		// Discarding tracks without pixel measurements
		if(TrajAnalyzer::trajectoryHasPixelHit(traj)) continue;
		// Looping again to check hit efficiency of pixel hits
		for(auto& measurement: traj -> measurements())
		{
			// Check measurement validity
			if(!measurement.updatedState().isValid()) continue;
			auto hit = measurement.recHit();
			// Det id
			DetId detId = hit -> geographicalId();
			uint32_t subdetid = (detId.subdetId());
			// Looking for pixel hits
			bool is_pixel_hit = false;
			is_pixel_hit |= subdetid == PixelSubdetector::PixelBarrel;
			is_pixel_hit |= subdetid == PixelSubdetector::PixelEndcap;
			if(!is_pixel_hit) continue;
			// Fetch the hit
			const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(hit -> hit());
			// Check hit qualty
			if(!pixhit) continue;
			// Position measurements
			TrajectoryStateCombiner  trajStateComb;
			TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
			LocalPoint localPosition = trajStateOnSurface.localPosition();
			float lx = localPosition.x();
			float ly = localPosition.y();
			SiPixelCluster closestCluster = findClosestCluster(clusterCollection, detId.rawId(), lx, ly);
			// Do nothing if no cluster is found
			if(closestCluster.minPixelRow() == SiPixelCluster::MAXPOS && closestCluster.minPixelCol() == SiPixelCluster::MAXPOS) continue;
			// float lz = localPosition.z()
			trajClosestClustMap.insert(std::pair<TrajectoryMeasurement, SiPixelCluster>(measurement, closestCluster));
		}
	}
	return trajClosestClustMap;
}

SiPixelCluster SplitClusterAnalyzer::findClosestCluster(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const uint32_t& rawId, const float& lx, const float& ly)
{
	auto getDistanceSquared = [] (float x1, float y1, float x2, float y2) {return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);};
	const SiPixelCluster* closestCluster = nullptr;
	float minDistanceSquared;
	// Looping on all the clusters
	for(const auto& clusterSetOnModule: *clusterCollection)
	{
		DetId detId(clusterSetOnModule.id());
		// Discarding clusters on a different module
		if(detId.rawId() != rawId) continue;
		unsigned int subdetId = detId.subdetId();
		// Discarding non-pixel clusters
		if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap) continue;
		// Guess that the first cluster is the closest
		closestCluster = clusterSetOnModule.begin();
		// First set the minimum distance to the distance of the first cluster
		minDistanceSquared = getDistanceSquared(closestCluster -> x(), closestCluster -> y(), lx, ly);
		// Looping on clusters on the same detector_part
		for(const auto& currentCluster: clusterSetOnModule)
		{
			float distanceSquared = getDistanceSquared(currentCluster.x(), currentCluster.y(), lx, ly);
			if(distanceSquared < minDistanceSquared)
			{
				closestCluster = &currentCluster;
				minDistanceSquared = distanceSquared;
			}
		}
	}
	if(closestCluster == nullptr) 
	{
		return SiPixelCluster();
	}
	return *closestCluster;
}

void SplitClusterAnalyzer::handleEvent(const edm::Event& iEvent)
{
	// Create new event plot
	if(numSavedEventPlots < maxEventPlotsToSave)
	{
		createEventPlot();
	}
	// Save event data
	eventField.fill         = static_cast<int>(0); // FIXME
	eventField.run          = static_cast<int>(iEvent.id().run());
	eventField.ls           = static_cast<int>(iEvent.luminosityBlock());
	eventField.orb          = static_cast<int>(iEvent.orbitNumber());
	eventField.bx           = static_cast<int>(iEvent.bunchCrossing());
	eventField.evt          = static_cast<int>(iEvent.id().event());	
	eventTree -> Fill();
}

void SplitClusterAnalyzer::handleClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const edm::Handle<edm::DetSetVector<PixelDigi>>& digiFlagsCollection, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
{
	int numClusters = 0;
	auto currentDigiFlagsIterator = digiFlagsCollection -> begin();
	// Looping on all the clusters
	for(const auto& clusterSetOnModule: *clusterCollection)
	{
		// std::cerr << "Num clusters on module: " << clusterSetOnModule.size() << std::endl;
		DetId detId(clusterSetOnModule.id());
		unsigned int subdetId = detId.subdetId();
		ModuleData mod    = ModuleDataProducer::getPhaseZeroOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
		ModuleData mod_on = ModuleDataProducer::convertPhaseZeroOfflineOnline(clusterField.mod);
		std::map<const SiPixelCluster*, const SiPixelCluster*> clusterPairsToMerge;
		// Take only pixel clusters
		if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap) continue;
		// Looping on clusters on the same detector_part
		for(const auto& currentCluster: clusterSetOnModule)
		{
			++numClusters;
			bool isCurrentClusterSplit = checkIfNextToDcolLostDigi(currentCluster, *currentDigiFlagsIterator);
			// Saving the pixel vectors to save computing time
			std::vector<SiPixelCluster::Pixel> currentClusterPixels = currentCluster.pixels();
			// Save cluster data
			saveClusterData(currentCluster, mod, mod_on);
			// Save digis data
			for(const auto& pixel: currentClusterPixels)
			{
				savePixelData(pixel, mod, mod_on, *currentDigiFlagsIterator);
				if(numSavedEventPlots < maxEventPlotsToSave)
				{
					fillEventPlot(pixel, mod_on, *currentDigiFlagsIterator);
				}
			}
			// Find pixels that are close to the current cluster
			std::vector<const SiPixelCluster*> closeClusters;
			for(const auto& otherCluster: clusterSetOnModule)
			{
				// Discard the cluster itself
				if(ClusterGeometry::isSameCluster(currentCluster, otherCluster)) continue;
				// Filter pixels closer than a given distance
				if(ClusterGeometry::isCloserThan(14.0, currentCluster, otherCluster))
				{
					closeClusters.push_back(&otherCluster);
				}
			}
			const SiPixelCluster* clusterToMergePtr = nullptr;
			// Filtering
			for(const SiPixelCluster* closeClusterPtr: closeClusters)
			{
				ClusterPairMergingValidator validator(currentCluster, *closeClusterPtr);
				if(!(validator.isMissingPartDoubleColumn())) continue;
				if(!(validator.isPairNotTooLong()))          continue;
				// if(!(validator.isDirSimilar()))              continue;
				if(clusterToMergePtr != nullptr)
				{
					std::cerr << c_red << "Error: " << c_def << "One of the clusters has more than one mergeable cluster candidates. Merging only the last one..." << std::endl;
				}
				clusterToMergePtr = closeClusterPtr;
			}
			// Stop if no mergeable clusters found
			if(clusterToMergePtr == nullptr) continue;
			const SiPixelCluster& clusterToMerge = *clusterToMergePtr;
			// Save every cluster pair only once: check the cluster order
			if(checkClusterPairOrder(currentCluster, clusterToMerge))
			{
				clusterPairsToMerge.insert(std::make_pair(&currentCluster, &clusterToMerge));
				// Saving the pixel vector to save computing time
				std::vector<SiPixelCluster::Pixel> clusterToMergePixels = clusterToMerge.pixels();
				// Saving off informations about the mergeable pixels
				saveMergingData(currentCluster, clusterToMerge, currentClusterPixels, clusterToMergePixels, isCurrentClusterSplit, mod, mod_on);
				// Save x (10) snapshots
				if(numSavedSnapshots < maxSnapshotsToSave)
				{
					saveSnapshotData(currentCluster, clusterToMerge);
					++numSavedSnapshots;
				}
			}
		}
		++currentDigiFlagsIterator; // TODO: check if this is safe
	}
	if(numSavedEventPlots < maxEventPlotsToSave)
	{
		saveEventPlot();
		++numSavedEventPlots;
	}
	std::cerr << "Num clusters: " << numClusters << std::endl;
}

bool SplitClusterAnalyzer::checkIfNextToDcolLostDigi(const SiPixelCluster& clusterToCheck, const edm::DetSet<PixelDigi>& digiFlags)
{
	std::cerr << "digiFlags.size(): " << digiFlags.size() << std::endl;
	for(const auto& digi: clusterToCheck.pixels())
	{
		int channel = PixelDigi::pixelToChannel(digi.x, digi.y);
		for(const auto& digiInSet: digiFlags)
		{
			int channelToCompareWith = digiInSet.channel();
			if(channel == channelToCompareWith)
			{
				return true;
			}
		}
	}
	return false;
}

bool SplitClusterAnalyzer::checkIfNextToDcolLostDigi(const SiPixelCluster::Pixel& pixelToCheck, const edm::DetSet<PixelDigi>& digiFlags)
{
	int channel = PixelDigi::pixelToChannel(pixelToCheck.x, pixelToCheck.y);
	for(const auto& digiInSet: digiFlags)
	{
		int channelToCompareWith = digiInSet.channel();
		if(channel == channelToCompareWith)
		{
			return true;
		}
	}
	return false;
}

bool SplitClusterAnalyzer::checkIfDigiIsInDetSet(const PixelDigi& digi, const edm::DetSet<PixelDigi>& digiDetSet)
{
	int channel = digi.channel();
	for(const auto& digiInSet: digiDetSet)
	{
		int channelToCompareWith = digiInSet.channel();
		if(channel == channelToCompareWith)
		{
			return true;
		}
	}
	return false;
}

// std::vector<SiPixelCluster::Pixel> SplitClusterAnalyzer::getDigisOnModule(const edmNew::DetSet<SiPixelCluster>& clusterSetOnModule)
// {
// 	std::vector<SiPixelCluster::Pixel> pixelsOnModule;
// 	for(const auto& currentCluster: clusterSetOnModule)
// 	{
// 		std::vector<SiPixelCluster::Pixel> pixelsInCluster = currentCluster.pixels();
// 		pixelsOnModule.reserve(pixelsOnModule.size() + pixelsInCluster.size()); 
// 		pixelsOnModule.insert(pixelsOnModule.end(), pixelsInCluster.begin(), pixelsInCluster.end());
// 	}
// 	return pixelsOnModule;
// }

// const SiPixelCluster* SplitClusterAnalyzer::findBestMergeableClusterCandidate(const SiPixelCluster& cluster, const edmNew::DetSet<SiPixelCluster>& clusterSet)
// {
// 	const SiPixelCluster* searchResult = nullptr;
// 	auto distanceScore = [] (float x1, float y1, float x2, float y2) { return std::abs(x1 - x2) + 3 * std::abs(y1 - y2); };
// 	if(clusterSet.size() < 2) return searchResult;
// 	float currentX = cluster.x();
// 	float currentY = cluster.y();
// 	// Lower score is better
// 	float minDistanceScore = distanceScore(currentX, currentY,  clusterSet.begin() -> x(), clusterSet.begin() -> y());
// 	searchResult = &(*(clusterSet.begin()));
// 	if(minDistanceScore == 0)
// 	{
// 		minDistanceScore = distanceScore(currentX, currentY, (clusterSet.begin() + 1) -> x(), (clusterSet.begin() + 1) -> y());
// 		searchResult = &(*(clusterSet.begin() + 1));
// 	}
// 	for(const auto& otherCluster: clusterSet)
// 	{
// 		// Y distance is more important than X distance
// 		float otherScore = distanceScore(currentX, currentY, otherCluster.x(), otherCluster.y());
// 		if(otherScore <= minDistanceScore && otherScore != 0)
// 		{
// 			minDistanceScore = otherScore;
// 			searchResult = &otherCluster;
// 		}
// 	}
// 	return searchResult;
// }

void SplitClusterAnalyzer::saveClusterData(const SiPixelCluster& cluster, const ModuleData& mod, const ModuleData& mod_on)
{
	clusterField.mod    = mod;
	clusterField.mod_on = mod_on;
	// Position and size
	clusterField.x     = cluster.x();
	clusterField.y     = cluster.y();
	clusterField.sizeX = cluster.sizeX();
	clusterField.sizeY = cluster.sizeY();
	clusterField.size  = cluster.size();
	// Charge
	clusterField.charge = cluster.charge();
	// Misc.
	for(int i = 0; i < clusterField.size && i < 1000; ++i)
	{
		const auto& currentPixels = cluster.pixels();
		clusterField.adc[i]    = cluster.pixelADC()[i] / 1000.0;
		clusterField.pix[i][0] = currentPixels[i].x;
		clusterField.pix[i][1] = currentPixels[i].y;
	}
	clusterTree -> Fill();
}

void SplitClusterAnalyzer::saveMergingData(const SiPixelCluster& currentCluster, const SiPixelCluster& clusterToMerge, const std::vector<SiPixelCluster::Pixel>& currentClusterPixels, const std::vector<SiPixelCluster::Pixel>& clusterToMergePixels, bool isCurrentClusterSplit, const ModuleData& mod, const ModuleData& mod_on)
{
	// Checking the charge of the cluster and the neighbour
	std::vector<uint16_t> currentClPixelADCs;
	std::vector<uint16_t> clusterToMergeADCs;
	double currentClusterTotalPixelADC  = 0;
	double clusterToMergeTotalPixelADC = 0;
	for(const auto pixel: currentClusterPixels)
	{
		currentClPixelADCs.push_back(pixel.adc);
		currentClusterTotalPixelADC += pixel.adc;
	}
	std::sort(currentClPixelADCs.begin(), currentClPixelADCs.end());
	for(const auto pixel: clusterToMerge.pixels())
	{
		clusterToMergeADCs.push_back(pixel.adc);
		clusterToMergeTotalPixelADC += pixel.adc;
	}
	std::sort(clusterToMergeADCs.begin(), clusterToMergeADCs.end());
	// Saving informations about the mergeable clusters
	mergeStatField.clusterSizes[0]           = currentCluster.size();
	mergeStatField.clusterSizes[1]           = clusterToMerge.size();
	mergeStatField.clusterCharges[0]         = currentCluster.charge();
	mergeStatField.clusterCharges[1]         = clusterToMerge.charge();
	mergeStatField.totalPixelADCs[0]         = currentClusterTotalPixelADC;
	mergeStatField.totalPixelADCs[1]         = clusterToMergeTotalPixelADC;
	mergeStatField.medianPixelADCs[0]        = currentClPixelADCs[currentClPixelADCs.size() / 2];
	mergeStatField.medianPixelADCs[1]        = clusterToMergeADCs[clusterToMergeADCs.size() / 2];
	mergeStatField.averagePixelADCs[0]       = currentClusterTotalPixelADC / currentClPixelADCs.size();
	mergeStatField.averagePixelADCs[1]       = clusterToMergeTotalPixelADC / clusterToMergeADCs.size();
	mergeStatField.sizeDifference            = currentCluster.size()   - clusterToMerge.size();
	mergeStatField.chargeDifference          = currentCluster.charge() - clusterToMerge.charge();
	// mergeStatField.closestPixelADCDifference = closePixelPairs[0].first.adc - closePixelPairs[0].second.adc;
	mergeStatField.isSplitCluster            = isCurrentClusterSplit;
	mergeStatField.averagePixelADCDifference = mergeStatField.averagePixelADCs[0] - mergeStatField.averagePixelADCs[1];
	mergeStatField.medianPixelADCDifference  = mergeStatField.medianPixelADCs[0]  - mergeStatField.medianPixelADCs[1];
	mergeStatField.mod                       = mod;
	mergeStatField.mod_on                    = mod_on;
	mergeTree -> Fill();
}

void SplitClusterAnalyzer::saveSnapshotData(const SiPixelCluster& currentCluster, const SiPixelCluster& clusterToMerge)
{
	int centerXFirst  = currentCluster.x();
	int centerYFirst  = currentCluster.y();
	// First cluster
	for(const auto& pixel: currentCluster.pixels())
	{
		int x = pixel.x - centerXFirst + 16;
		int y = pixel.y - centerYFirst + 16;
		int index = 32 * y + x;
		if(index < 0 || 1023 < index)
		{
			std::cerr << "Error creating merging snapshot: index out of borders: " << index << "." << std::endl;
			continue;
		}
		mergeSnapshotField[index] = 1;
	}
	// Second cluster
	for(const auto& pixel: clusterToMerge.pixels())
	{
		int x = pixel.x - centerXFirst + 16;
		int y = pixel.y - centerYFirst + 16;
		int index = 32 * y + x;
		if(index < 0 || 1023 < index)
		{
			std::cerr << "Error creating merging snapshot: index out of borders: " << index << "." << std::endl;
			continue;
		}
		mergeSnapshotField[index] = 2;
	}
	TH2I* snapshotHisto = new TH2I(("snapshot_" + std::to_string(numSavedSnapshots)).c_str(), ("snapshot_" + std::to_string(numSavedSnapshots)).c_str(), 32, 0, 32, 32, 0, 32);
	for(int i = 0; i < 1024; ++i)
	{
		if(mergeSnapshotField[i] == 0) continue;
		int x = i % 32, y = i / 32;
		snapshotHisto -> Fill(x, y);
		if(mergeSnapshotField[i] == 1) continue;
		snapshotHisto -> Fill(x, y);
	}
	snapshotHisto -> Write();
	mergeSnapshotTree -> Fill();
}

void SplitClusterAnalyzer::savePixelData(const SiPixelCluster::Pixel& pixelToSave, const ModuleData& mod, const ModuleData& mod_on, const edm::DetSet<PixelDigi>& digiFlagsCollection)
{
	pixelField.mod      = mod;
	pixelField.mod_on   = mod_on;
	pixelField.col      = static_cast<int>(pixelToSave.x);
	pixelField.row      = static_cast<int>(pixelToSave.y);
	pixelField.adc      = static_cast<int>(pixelToSave.adc);
	pixelField.isMarked = checkIfNextToDcolLostDigi(pixelToSave, digiFlagsCollection);
	pixelTree -> Fill();
}

void SplitClusterAnalyzer::createEventPlot()
{
	currentEventPlotLayer1 = new TH2D(("event_plot_layer_1_" + std::to_string(numSavedEventPlots)).c_str(), ("event_plot_layer_1_" + std::to_string(numSavedEventPlots)).c_str(), 468, 234, 234, 3600, 1800, 1800);
	currentEventPlotLayer2 = new TH2D(("event_plot_layer_2_" + std::to_string(numSavedEventPlots)).c_str(), ("event_plot_layer_2_" + std::to_string(numSavedEventPlots)).c_str(), 468, 234, 234, 3600, 1800, 1800);
	currentEventPlotLayer3 = new TH2D(("event_plot_layer_3_" + std::to_string(numSavedEventPlots)).c_str(), ("event_plot_layer_3_" + std::to_string(numSavedEventPlots)).c_str(), 468, 234, 234, 3600, 1800, 1800);
}

void SplitClusterAnalyzer::fillEventPlot(const SiPixelCluster::Pixel& pixelToSave, const ModuleData& mod_on, const edm::DetSet<PixelDigi>& digiFlagsCollection)
{
    // Layers range: 1 - 3
	// Module range: -4.5 - 4.5
	// Ladders range: -22.5 - 22.5
	int moduleCoordinate = mod_on.module * 52;
	int ladderCoordinate = mod_on.ladder * 80;
	switch(mod_on.layer)
	{
		case 1:
			currentEventPlotLayer1 -> Fill(moduleCoordinate, ladderCoordinate);
			break;
		case 2:
			currentEventPlotLayer2 -> Fill(moduleCoordinate, ladderCoordinate);
			break;
		case 3:
			currentEventPlotLayer3 -> Fill(moduleCoordinate, ladderCoordinate);
			break;
		default:
			handleDefaultError("data_analysis", "data_analysis", {"Failed to deduce the layer coordinate of a pixel: layer: ", std::to_string(mod_on.layer)});
	}
}

void SplitClusterAnalyzer::saveEventPlot()
{
	currentEventPlotLayer1 -> Write();
	currentEventPlotLayer2 -> Write();
	currentEventPlotLayer3 -> Write();
}

bool SplitClusterAnalyzer::checkClusterPairOrder(const SiPixelCluster& lhs, const SiPixelCluster& rhs)
{
	// Perform checks once per cluster pairs
	if(lhs.size() < rhs.size()) return true;
	if(lhs.size() > rhs.size()) return false;
	if(lhs.charge() < rhs.charge()) return true;
	if(lhs.charge() > rhs.charge()) return false;
	if(lhs.charge() < rhs.charge()) return true;
	if(lhs.charge() > rhs.charge()) return false;
	if(lhs.x() < rhs.x()) return true;
	if(lhs.x() > rhs.x()) return false;
	if(lhs.y() < rhs.y()) return true;
	if(lhs.y() > rhs.y()) return false;
	std::cerr << c_red << "Error: " << c_def << "SplitClusterMergerProducer::checkClusterPairOrder() called with the same cluster twice as argument. Check code!" << std::endl;
	return false;
}

void SplitClusterAnalyzer::beginRun(edm::Run const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterAnalyzer::beginRun()..." << std::endl;
}

void SplitClusterAnalyzer::endRun(edm::Run const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterAnalyzer::endRun()..." << std::endl;
}

void SplitClusterAnalyzer::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterAnalyzer::beginLuminosityBlock()..." << std::endl;
}

void SplitClusterAnalyzer::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterAnalyzer::endLuminosityBlock()..." << std::endl;
}

/////////////
// Utility //
/////////////

void SplitClusterAnalyzer::clearAllContainers()
{
	// clusterMergeableDigis.clear();
}

////////////////////
// Error handling //
////////////////////

void SplitClusterAnalyzer::handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg)
{
	edm::LogError(streamType.c_str()) << c_red << msg << c_def << std::endl;
	printEvtInfo(streamType);
	throw cms::Exception(exceptionType.c_str());
}

void SplitClusterAnalyzer::handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg)
{
	edm::LogError(streamType.c_str()) << c_red;
	for(const auto& msg_part: msg)
	{
		edm::LogError(streamType.c_str()) << msg_part;
	}
	edm::LogError(streamType.c_str()) << c_def << std::endl;
	printEvtInfo(streamType);
	throw cms::Exception(exceptionType.c_str());
}

void SplitClusterAnalyzer::printEvtInfo(const std::string& streamType)
{
	edm::LogError(streamType.c_str()) << c_blue <<
		"Run: "    << currentEvent -> id().run()        << 
		" Ls: "    << currentEvent -> luminosityBlock() << 
		" Event: " << currentEvent -> id().event()      << c_def << std::endl;
}

DEFINE_FWK_MODULE(SplitClusterAnalyzer);

// // Cluster Parameter Estimator
// edm::ESHandle<PixelClusterParameterEstimator> clusterParameterEstimatorHandle;
// iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", clusterParameterEstimatorHandle);
// if(!clusterParameterEstimatorHandle.isValid()) return;
// const PixelClusterParameterEstimator& clusterParameterEstimator(*clusterParameterEstimatorHandle);


// // Find the digis on this module
// edm::DetSetVector<PixelDigi>::const_iterator digiSetOnModule;
// digiSetOnModule = std::find_if(digiCollection -> begin(), digiCollection -> end(), [&] (edm::DetSetVector<PixelDigi>::const_reference detSet)
// {
// 	bool result  = detSet.id() == detId;
// 	// result      &= detSet.id().subdetid() == subdetId;
// 	return result;
// } );
// // Check if the digis are found
// if(digiSetOnModule == digiCollection -> end())
// {
// 	handleDefaultError("data_access", "data_access", "No digis found on module with pixels.");
// }

// // Rough filtering
// for(const auto& pixel: pixelsOnModule)
// {
// 	// Skip pixels that are already part of the cluster
// 	auto isSamePixel = [&pixel] (SiPixelCluster::Pixel pixelToCheck) { return pixelToCheck.x == pixel.x && pixelToCheck.y == pixelToCheck.y; };
// 	if(std::find_if(currentClusterPixels.begin(), currentClusterPixels.end(), isSamePixel) != currentClusterPixels.end())
// 	{
// 		continue;
// 	}
// 	// The pixels close enough become mergeable candidates
// 	if(pixelXDistance(pixel, clusterPositionApproximator) < 6 && pixelYDistance(pixel, clusterPositionApproximator) < 6)
// 	{
// 		mergeablePixelCandidates.push_back(pixel);
// 	}
// }
// // Secondary filtering
// for(const auto& pixelInCluster: currentClusterPixels)
// {
// 	// The good pixels are one double-column distance away (y)
// 	// in the same row (x) or one pixel further in the x direction
// 	auto pixelSelector = [&pixelInCluster] (SiPixelCluster::Pixel pixel)
// 	{
// 		int xDist = SplitClusterAnalyzer::pixelXDistance(pixelInCluster, pixel);
// 		int yDist = SplitClusterAnalyzer::pixelYDistance(pixelInCluster, pixel);
// 		if(0 <= xDist && xDist < 2 && (yDist == 2 || yDist == 3))
// 		{
// 			return 0;
// 		}
// 		return 1;
// 	};
// 	std::remove_if(mergeablePixelCandidates.begin(), mergeablePixelCandidates.end(), pixelSelector);
// }