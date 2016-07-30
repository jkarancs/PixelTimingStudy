#include "SplitClusterMerger.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG

SplitClusterMerger::SplitClusterMerger(edm::ParameterSet const& iConfig)
{
	// LogDebug("step") << "SplitClusterMerger() constructor called." << std::endl;
	// Tokens
	rawDataErrorToken        = consumes<edm::DetSetVector<SiPixelRawDataError>>(edm::InputTag("siPixelDigis"));
	// pixelDigisToken   = consumes<edm::DetSetVector<PixelDigi>>          (edm::InputTag("siPixelDigis"));
	clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>  (edm::InputTag("siPixelClusters"));
	trajTrackCollectionToken = consumes<TrajTrackAssociationCollection>(iConfig.getParameter<edm::InputTag>("trajectoryInput"));

}

SplitClusterMerger::~SplitClusterMerger() {}

void SplitClusterMerger::beginJob()
{
	// LogDebug("step") << "Executing SplitClusterMerger::beginJob()..." << std::endl;
	this -> iConfig = iConfig;
	// Override output filename
	if(iConfig.exists("fileName"))
	{
		ntupleOutputFilename = iConfig.getParameter<std::string>("filename");
	}
	// Create output file
	ntupleOutputFile = new TFile(ntupleOutputFilename.c_str(), "RECREATE");
	if(!(ntupleOutputFile -> IsOpen()))
	{
		handleDefaultError("file_operations", "file_operations", { "Failed to open output file: ", ntupleOutputFilename });
	}
	LogDebug("file_operations") << "Output file: \"" << ntupleOutputFilename << "\" created." << std::endl;
	// Tree definitions
	clusterTree = new TTree("clustTree", "Pixel clusters");
	ClusterDataTree::defineClusterTreeBranches(clusterTree, clusterField);
	mergeTree   = new TTree("mergeTree", "Cluster merging info");
	MergingStatisticsTree::defineMergingStatTreeBranches(mergeTree, mergeStatField);
}

void SplitClusterMerger::endJob()
{
	// LogDebug("step") << "Executing SplitClusterMerger::endJob()..." << std::endl;
	LogDebug("file_operations") << "Writing plots to file: \"" << ntupleOutputFilename << "\"." << std::endl;
	ntupleOutputFile -> Write();
	ntupleOutputFile -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << c_blue << ntupleOutputFilename << c_def << "\"." << std::endl;
}

// Steps:
// 1. Loop on all modules and panels
// 2. For all the clusters on the given detector part
// 3. Get all the possible cluster-part positions
// 4. Check if there is charge-deposit on them
// 5. Perform checks for merging
// 6. If the checks are passed, interpolate for charge deposition
// 7. Merge the pixels by setting inner charges

void SplitClusterMerger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	// LogDebug("step") << "Executing SplitClusterMerger::analyze()..." << std::endl;
	currentEvent = &iEvent;
	// Accessing the tracker topology for module informations
	edm::ESHandle<TrackerTopology> trackerTopologyHandle;
	iSetup.get<TrackerTopologyRcd>().get(trackerTopologyHandle);
	const TrackerTopology* const trackerTopology = trackerTopologyHandle.product();
	// FED errors
	std::map<uint32_t, int> fedErrors = FedErrorFetcher::getFedErrors(iEvent, rawDataErrorToken);
	// Fetching the digis by token
	// edm::Handle<edm::DetSetVector<PixelDigi>> digiCollection;
	// iEvent.getByToken(pixelDigisToken,        digiCollection);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollection;
	iEvent.getByToken(clustersToken,                  clusterCollection);
	// Fetching the tracks by token
	edm::Handle<TrajTrackAssociationCollection> trajTrackCollection;
	iEvent.getByToken(trajTrackCollectionToken, trajTrackCollection);
	// Trying to access the clusters
	// if(!digiCollection.isValid())    handleDefaultError("data access", "data_access", "Failed to fetch digis.");
	if(!clusterCollection.isValid())   handleDefaultError("data access", "data_access", "Failed to fetch clusters.");
	if(!trajTrackCollection.isValid()) handleDefaultError("data access", "data_access", "Failed to fetch trajectory measurements.");
	// Preparing a trajectory to closest cluster map
	std::map<Trajectory, SiPixelCluster> trajClosestClustMap;
	// getTrajClosestClusterMap(trajTrackCollection, clusterCollection, trackerTopology);
	// Processing data
	ClusterDataTree::setClusterTreeDataFields(clusterTree, clusterField);
	clusterField.init();
	MergingStatisticsTree::setMergingStatTreeDataFields(mergeTree, mergeStatField);
	mergeStatField.init();
	// handleClusters(digiCollection, clusterCollection, trackerTopology, fedErrors);
	handleClusters(clusterCollection, trackerTopology, fedErrors);
	clearAllContainers();
}

// std::map<Trajectory, SiPixelCluster> SplitClusterMerger::getTrajClosestClusterMap(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollection, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology)
// {
// 	std::map<Trajectory, SiPixelCluster> trajClosestClustMap;
// 	for(const auto& currentTrackKeypair: *trajTrackCollection)
// 	{
// 		// Trajectory segments + corresponding track informations
// 		const edm::Ref<std::vector<Trajectory>> traj = currentTrackKeypair.key;
// 		const reco::TrackRef track                   = currentTrackKeypair.val; // TrackRef is actually a pointer type
// 		// Discarding tracks without pixel measurements
// 		if(TrajAnalyzer::trajectoryHasPixelHit(traj)) continue;
// 		// Looping again to check hit efficiency of pixel hits
// 		for(auto& measurement: traj -> measurements())
// 		{
// 			// Check measurement validity
// 			if(!measurement.updatedState().isValid()) continue;
// 			auto hit = measurement.recHit();
// 			// Det id
// 			DetId detID = hit -> geographicalId();
// 			uint32_t subdetid = (detID.subdetId());
// 			// Looking for pixel hits
// 			bool is_pixel_hit = false;
// 			is_pixel_hit |= subdetid == PixelSubdetector::PixelBarrel;
// 			is_pixel_hit |= subdetid == PixelSubdetector::PixelEndcap;
// 			if(!is_pixel_hit) continue;
// 			// Fetch the hit
// 			const SiPixelRecHit* pixhit = dynamic_cast<const SiPixelRecHit*>(hit -> hit());
// 			// Check hit qualty
// 			if(!pixhit) continue;
// 			// Position measurements
// 			TrajectoryStateCombiner  trajStateComb;
// 			TrajectoryStateOnSurface trajStateOnSurface = trajStateComb(measurement.forwardPredictedState(), measurement.backwardPredictedState());
// 			auto localPosition = trajStateOnSurface.localPosition();
// 			double lx = localPosition.x();
// 			double ly = localPosition.y();
// 			// double lz = localPosition.z();
// 		}
// 	}
// 	return trajClosestClustMap;
// }

// void SplitClusterMerger::handleClusters(const edm::Handle<edm::DetSetVector<PixelDigi>>& digiCollection, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
void SplitClusterMerger::handleClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
{
	// Looping on all the clusters
	for(const auto& clusterSetOnModule: *clusterCollection)
	{
		// std::cerr << "Num clusters on module: " << clusterSetOnModule.size() << std::endl;
		DetId detId(clusterSetOnModule.id());
		unsigned int subdetId = detId.subdetId();
		// Take only pixel clusters
		if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap)
		{
			continue;
		}
		// Get digis on module
		std::vector<SiPixelCluster::Pixel> pixelsOnModule = getDigisOnModule(clusterSetOnModule);
		// Looping on clusters on the same detector_part
		for(const auto& currentCluster: clusterSetOnModule)
		{
			// Save cluster data
			saveClusterData(currentCluster, detId, trackerTopology, fedErrors);
			clusterTree -> Fill();
			// Info:
			// X coordinate range: 0-159 (row)
			// Y coordinate range: 0-415 (column)
			// Save current cluster data
			std::vector<SiPixelCluster::Pixel> currentClusterPixels = currentCluster.pixels();
			// Find pixels that are close to the current cluster
			const SiPixelCluster* mergeableClusterCandidate = findBestMergeableClusterCandidate(currentCluster, clusterSetOnModule);
			// Stop if no candidate found
			if(mergeableClusterCandidate == nullptr) continue;
			// Perform checks once per cluster pairs
			if(currentCluster.size() <  mergeableClusterCandidate -> size()) continue;
			if(currentCluster.size() == mergeableClusterCandidate -> size())
			{
				if(currentCluster.charge() <  mergeableClusterCandidate -> charge()) continue;
				if(currentCluster.charge() == mergeableClusterCandidate -> charge())
				{
					if(currentCluster.charge() <  mergeableClusterCandidate -> charge()) continue;
					if(currentCluster.charge() == mergeableClusterCandidate -> charge())
					{
						if(currentCluster.x() <  mergeableClusterCandidate -> x()) continue;
						if(currentCluster.x() == mergeableClusterCandidate -> x());
						{
							if(currentCluster.y() <  mergeableClusterCandidate -> y()) continue;
							if(currentCluster.y() == mergeableClusterCandidate -> y()) continue;
							{
								std::cerr << c_red << "Error: " << c_def << "Found the same cluster as closest. Check code!" << std::endl;
							}
						}
					}
				}
			}
			std::vector<std::pair<SiPixelCluster::Pixel, SiPixelCluster::Pixel>> closePixelPairs;
			// Filtering
			for(const auto& candidateClusterPixel: mergeableClusterCandidate -> pixels())
			{
				// The good pixels are one double-column distance away (y)
				// in the same row (x) or one pixel further in the x direction
				for(const auto& currentClusterPixel: currentClusterPixels)
				{
					int xDist = SplitClusterMerger::pixelXDistance(currentClusterPixel, candidateClusterPixel);
					int yDist = SplitClusterMerger::pixelYDistance(currentClusterPixel, candidateClusterPixel);
					if(0 <= xDist && xDist < 2 && (yDist == 2 || yDist == 3))
					{
						closePixelPairs.push_back(std::make_pair(currentClusterPixel, candidateClusterPixel));
					}
				};
			}
			std::sort(closePixelPairs.begin(), closePixelPairs.end(), [] (auto lhs, auto rhs) { return SplitClusterMerger::pixelSquareDistance(lhs.first, lhs.second) < SplitClusterMerger::pixelSquareDistance(rhs.first, rhs.second); } );
			// Stop if no mergeable clusters found
			if(closePixelPairs.empty()) continue;
			// Checking the charge of the cluster and the neighbour
			std::vector<uint16_t> currentPixelADCs;
			std::vector<uint16_t> mergeablePixelADCs;
			double currentClusterTotalPixelADC   = 0;
			double mergeableClusterTotalPixelADC = 0;
			for(const auto pixel: currentClusterPixels)
			{
				currentPixelADCs.push_back(pixel.adc);
				currentClusterTotalPixelADC += pixel.adc;
			}
			std::sort(currentPixelADCs.begin(), currentPixelADCs.end());
			for(const auto pixel: mergeableClusterCandidate -> pixels())
			{
				mergeablePixelADCs.push_back(pixel.adc);
				mergeableClusterTotalPixelADC += pixel.adc;
			}
			std::sort(mergeablePixelADCs.begin(), mergeablePixelADCs.end());
			// Saving informations about the mergeable pixel 
			std::cerr << "--- Begin cluster merging info ---" << std::endl
				<< "Current cluster position:   x: " << currentCluster.x()               << " y: " << currentCluster.y()               << std::endl
				<< "Mergeable cluster position: x: " << mergeableClusterCandidate -> x() << " y: " << mergeableClusterCandidate -> y() << std::endl
				<< "Current   cluster           charge: " << currentCluster.charge()               << std::endl
				<< "Mergeable cluster           charge: " << mergeableClusterCandidate -> charge() << std::endl
				<< "Current   cluster summed pixel adc: " << currentClusterTotalPixelADC   << std::endl
				<< "Mergeable cluster summed pixel adc: " << mergeableClusterTotalPixelADC << std::endl
				<< "Current   cluster avg.   pixel adc: " << currentClusterTotalPixelADC   / currentPixelADCs.size()   << std::endl
				<< "Mergeable cluster avg.   pixel adc: " << mergeableClusterTotalPixelADC / mergeablePixelADCs.size() << std::endl
				<< "Current   cluster median pixel adc: " << currentPixelADCs[currentPixelADCs.size()     / 2] << std::endl
				<< "Mergeable cluster median pixel adc: " << mergeablePixelADCs[mergeablePixelADCs.size() / 2] << std::endl;

			mergeStatField.clusterSizes[0]           = currentCluster.size();
			mergeStatField.clusterSizes[1]           = mergeableClusterCandidate -> size();
			mergeStatField.clusterCharges[0]         = currentCluster.charge();
			mergeStatField.clusterCharges[1]         = mergeableClusterCandidate ->charge();
			mergeStatField.totalPixelADCs[0]         = currentClusterTotalPixelADC;
			mergeStatField.totalPixelADCs[1]         = mergeableClusterTotalPixelADC;
			mergeStatField.medianPixelADCs[0]        = currentPixelADCs[currentPixelADCs.size()     / 2];
			mergeStatField.medianPixelADCs[1]        = mergeablePixelADCs[mergeablePixelADCs.size() / 2];
			mergeStatField.averagePixelADCs[0]       = currentClusterTotalPixelADC   / currentPixelADCs.size();
			mergeStatField.averagePixelADCs[1]       = mergeableClusterTotalPixelADC / mergeablePixelADCs.size();
			mergeStatField.sizeDifference            = currentCluster.size() - mergeableClusterCandidate -> size();
			mergeStatField.chargeDifference          = currentCluster.charge() - mergeableClusterCandidate -> charge();
			mergeStatField.closestPixelADCDifference = closePixelPairs[0].first.adc - closePixelPairs[0].second.adc;
			mergeStatField.averagePixelADCDifference = mergeStatField.averagePixelADCs[0] - mergeStatField.averagePixelADCs[1];
			mergeStatField.medianPixelADCDifference  = mergeStatField.medianPixelADCs[0] - mergeStatField.medianPixelADCs[1];
			mergeStatField.mod                       = ModuleDataProducer::getPhaseZeroOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
			mergeStatField.mod_on                    = ModuleDataProducer::convertPhaseZeroOfflineOnline(mergeStatField.mod);
			mergeTree -> Fill();
			// TODO: Merging
		}
	}
}

std::vector<SiPixelCluster::Pixel> SplitClusterMerger::getDigisOnModule(const edmNew::DetSet<SiPixelCluster>& clusterSetOnModule)
{
	std::vector<SiPixelCluster::Pixel> pixelsOnModule;
	for(const auto& currentCluster: clusterSetOnModule)
	{
		std::vector<SiPixelCluster::Pixel> pixelsInCluster = currentCluster.pixels();
		pixelsOnModule.reserve(pixelsOnModule.size() + pixelsInCluster.size()); 
		pixelsOnModule.insert(pixelsOnModule.end(), pixelsInCluster.begin(), pixelsInCluster.end());
	}
	return pixelsOnModule;
}

const SiPixelCluster* SplitClusterMerger::findBestMergeableClusterCandidate(const SiPixelCluster& cluster, const edmNew::DetSet<SiPixelCluster>& clusterSet)
{
	const SiPixelCluster* searchResult = nullptr;
	auto distanceScore = [] (float x1, float y1, float x2, float y2)
	{
		return std::abs(x1 - x2) + 3 * std::abs(y1 - y2);
	};
	if(clusterSet.size() < 2) return searchResult;
	float currentX = cluster.x();
	float currentY = cluster.y();
	// Lower score is better
	float minDistanceScore = distanceScore(currentX, currentY,  clusterSet.begin() -> x(), clusterSet.begin() -> y());
	searchResult = &(*(clusterSet.begin()));
	if(minDistanceScore == 0)
	{
		minDistanceScore = distanceScore(currentX, currentY, (clusterSet.begin() + 1) -> x(), (clusterSet.begin() + 1) -> y());
		searchResult = &(*(clusterSet.begin() + 1));
	}
	for(const auto& otherCluster: clusterSet)
	{
		// Y distance is more important than X distance
		float otherScore = distanceScore(currentX, currentY, otherCluster.x(), otherCluster.y());
		if(otherScore <= minDistanceScore && otherScore != 0)
		{
			minDistanceScore = otherScore;
			searchResult = &otherCluster;
		}
	}
	return searchResult;
}

int SplitClusterMerger::pixelXDistance(SiPixelCluster::Pixel lhs, SiPixelCluster::Pixel rhs)
{
	// 0-159 corresponds to 2 * 80
	return std::abs(lhs.x - rhs.x);
}

int SplitClusterMerger::pixelYDistance(SiPixelCluster::Pixel lhs, SiPixelCluster::Pixel rhs)
{
	// 0-415 corresponds to 52 * 8;
	return std::abs(lhs.y - rhs.y);
}

int SplitClusterMerger::pixelSquareDistance(SiPixelCluster::Pixel lhs, SiPixelCluster::Pixel rhs)
{
	int xDist = SplitClusterMerger::pixelXDistance(lhs, rhs);
	int yDist = SplitClusterMerger::pixelYDistance(lhs, rhs);
	return xDist * xDist + yDist * yDist;
}

void SplitClusterMerger::saveClusterData(const SiPixelCluster& cluster, DetId detId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors)
{
	clusterField.mod    = ModuleDataProducer::getPhaseZeroOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
	clusterField.mod_on = ModuleDataProducer::convertPhaseZeroOfflineOnline(clusterField.mod);
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
}

void SplitClusterMerger::beginRun(edm::Run const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterMerger::beginRun()..." << std::endl;
}

void SplitClusterMerger::endRun(edm::Run const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterMerger::endRun()..." << std::endl;
}

void SplitClusterMerger::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterMerger::beginLuminosityBlock()..." << std::endl;
}

void SplitClusterMerger::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	// LogDebug("step") << "Executing SplitClusterMerger::endLuminosityBlock()..." << std::endl;
}

/////////////
// Utility //
/////////////

void SplitClusterMerger::clearAllContainers()
{
	// clusterMergeableDigis.clear();
}

////////////////////
// Error handling //
////////////////////

void SplitClusterMerger::handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg)
{
	edm::LogError(streamType.c_str()) << c_red << msg << c_def << std::endl;
	printEvtInfo(streamType);
	throw cms::Exception(exceptionType.c_str());
}

void SplitClusterMerger::handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg)
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

void SplitClusterMerger::printEvtInfo(const std::string& streamType)
{
	edm::LogError(streamType.c_str()) << c_blue <<
		"Run: "    << currentEvent -> id().run()        << 
		" Ls: "    << currentEvent -> luminosityBlock() << 
		" Event: " << currentEvent -> id().event()      << c_def << std::endl;
}

DEFINE_FWK_MODULE(SplitClusterMerger);

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
// 		int xDist = SplitClusterMerger::pixelXDistance(pixelInCluster, pixel);
// 		int yDist = SplitClusterMerger::pixelYDistance(pixelInCluster, pixel);
// 		if(0 <= xDist && xDist < 2 && (yDist == 2 || yDist == 3))
// 		{
// 			return 0;
// 		}
// 		return 1;
// 	};
// 	std::remove_if(mergeablePixelCandidates.begin(), mergeablePixelCandidates.end(), pixelSelector);
// }