#include "../plugins/SplitClusterMergerProducer.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG

SplitClusterMergerProducer::SplitClusterMergerProducer(edm::ParameterSet const& iConfig)
{
	// Tokens
	rawDataErrorToken        = consumes<edm::DetSetVector<SiPixelRawDataError>>(edm::InputTag("siPixelDigis"));
	clustersToken            = consumes<edmNew::DetSetVector<SiPixelCluster>>  (edm::InputTag("siPixelClusters"));
	// Produces
	produces<edmNew::DetSetVector<SiPixelCluster>>();
}

void SplitClusterMergerProducer::beginJob()
{

}

void SplitClusterMergerProducer::produce(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	auto producedData = std::make_unique<edm::DetSetVector<SiPixelCluster>>();
	// Accessing the tracker topology for module informations
	edm::ESHandle<TrackerTopology> trackerTopologyHandle;
	iSetup.get<TrackerTopologyRcd>().get(trackerTopologyHandle);
	const TrackerTopology* const trackerTopology = trackerTopologyHandle.product();
	// FED errors
	std::map<uint32_t, int> fedErrors = FedErrorFetcher::getFedErrors(iEvent, rawDataErrorToken);
	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster>> clusterCollection;
	iEvent.getByToken(clustersToken,                  clusterCollection);
	// Do the merging
	mergeClusters(clusterCollection, trackerTopology, fedErrors, producedData);
	iEvent.put(std::move(producedData), "siPixelMergedClusters");
}

void SplitClusterMergerProducer::mergeClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors, std::unique_ptr<edm::DetSetVector<SiPixelCluster>>& producedData)
{
	// Looping on all the clusters
	for(const auto& clusterSetOnModule: *clusterCollection)
	{
		// std::cerr << "Num clusters on module: " << clusterSetOnModule.size() << std::endl;
		DetId detId(clusterSetOnModule.id());
		edm::DetSet<SiPixelCluster> mergedClusterDetset(detId.rawId());
		std::map<const SiPixelCluster*, const SiPixelCluster*> clusterPairsToMerge;
		unsigned int subdetId = detId.subdetId();
		// Take only pixel clusters
		if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap) continue;
		// Looping on clusters on the same detector_part
		for(const auto& currentCluster: clusterSetOnModule)
		{
			// Save current cluster data
			std::vector<SiPixelCluster::Pixel> currentClusterPixels = currentCluster.pixels();
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
				if(!(validator.isDirSimilar()))              continue;
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
			if(checkClusterPairOrder(currentCluster, clusterToMerge)) continue;
			clusterPairsToMerge.insert(std::make_pair(&currentCluster, &clusterToMerge));
			// Saving off informations
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

			// Saving informations about the mergeable pixel 
			// mergeStatField.clusterSizes[0]           = currentCluster.size();
			// mergeStatField.clusterSizes[1]           = clusterToMerge.size();
			// mergeStatField.clusterCharges[0]         = currentCluster.charge();
			// mergeStatField.clusterCharges[1]         = clusterToMerge.charge();
			// mergeStatField.totalPixelADCs[0]         = currentClusterTotalPixelADC;
			// mergeStatField.totalPixelADCs[1]         = clusterToMergeTotalPixelADC;
			// mergeStatField.medianPixelADCs[0]        = currentClPixelADCs[currentClPixelADCs.size() / 2];
			// mergeStatField.medianPixelADCs[1]        = clusterToMergeADCs[clusterToMergeADCs.size() / 2];
			// mergeStatField.averagePixelADCs[0]       = currentClusterTotalPixelADC / currentClPixelADCs.size();
			// mergeStatField.averagePixelADCs[1]       = clusterToMergeTotalPixelADC / clusterToMergeADCs.size();
			// mergeStatField.sizeDifference            = currentCluster.size()   - clusterToMerge.size();
			// mergeStatField.chargeDifference          = currentCluster.charge() - clusterToMerge.charge();
			// mergeStatField.closestPixelADCDifference = closePixelPairs[0].first.adc - closePixelPairs[0].second.adc;
			// mergeStatField.averagePixelADCDifference = mergeStatField.averagePixelADCs[0] - mergeStatField.averagePixelADCs[1];
			// mergeStatField.medianPixelADCDifference  = mergeStatField.medianPixelADCs[0]  - mergeStatField.medianPixelADCs[1];
			// mergeStatField.mod                       = ModuleDataProducer::getPhaseZeroOfflineModuleData(detId.rawId(), trackerTopology, fedErrors);
			// mergeStatField.mod_on                    = ModuleDataProducer::convertPhaseZeroOfflineOnline(mergeStatField.mod);
			// mergeTree -> Fill();

		}
		// Copy with merging
		for(const auto& currentCluster: clusterSetOnModule)
		{
			typedef std::pair<const SiPixelCluster*, const SiPixelCluster*> pair_t;
			auto find_as_key   = [&currentCluster] (pair_t clusterPair) { return ClusterGeometry::isSameCluster(currentCluster, *(clusterPair.first )); };
			auto find_as_value = [&currentCluster] (pair_t clusterPair) { return ClusterGeometry::isSameCluster(currentCluster, *(clusterPair.second)); };
			auto found_as_key   = std::find_if(clusterPairsToMerge.begin(), clusterPairsToMerge.end(), find_as_key);
			auto found_as_value = std::find_if(clusterPairsToMerge.begin(), clusterPairsToMerge.end(), find_as_value);
			if(found_as_key != clusterPairsToMerge.end())
			{
				// Merged clusters
				mergedClusterDetset.push_back(mergeClusterPair(*(found_as_key -> first), *(found_as_key -> second)));
				continue;
			}
			if(found_as_value != clusterPairsToMerge.end()) continue;
			// Clusters without pair to merge with
			mergedClusterDetset.push_back(currentCluster);
		}	
		producedData -> insert(mergedClusterDetset);
	}
}

/////////////
// Utility //
/////////////

bool SplitClusterMergerProducer::checkClusterPairOrder(const SiPixelCluster& lhs, const SiPixelCluster& rhs)
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

SiPixelCluster SplitClusterMergerProducer::mergeClusterPair(const SiPixelCluster& lhs, const SiPixelCluster& rhs)
{
	SiPixelCluster result;
	return result;
}


void SplitClusterMergerProducer::endJob()
{

}


DEFINE_FWK_MODULE(SplitClusterMergerProducer);