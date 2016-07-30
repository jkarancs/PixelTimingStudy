#ifndef SPLIT_CLUSTER_MERGER_H
#define SPLIT_CLUSTER_MERGER_H

//////////////////////////
// EDM plugin libraries //
//////////////////////////

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// For the tracker topology handle
#include "FWCore/Framework/interface/ESHandle.h"

////////////////////////////
// Message logger service //
////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"
// Adding some colors :)
#include "../interface/Console_colors.h"

///////////
// Tools //
///////////

// Fed errors
#include "../interface/FedErrorFetcher.h"
// Tree branching
#include "../interface/ClusterDataTree.h"
#include "../interface/MergingStatisticsTree.h"
// Module data
#include "../interface/ModuleDataProducer.h"
// Tracker topology
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
// Tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "../interface/TrajAnalyzer.h"
// Position
// #include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
// #include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"

///////////
// Other //
///////////

// Errors token
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
// Digi token
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
// Clusters token
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
// Tracjectories token
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"

////////////////////
// Root libraries //
////////////////////

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1F.h>
// #include <TH2F.h>
#include <TRandom3.h>

////////////////
// C++ system //
////////////////

#include <iostream>
#include <string>
#include <vector>
#include <limits>

class SplitClusterMerger : public edm::EDAnalyzer
{
	private:
		edm::ParameterSet iConfig;
		// For debug
		const edm::Event* currentEvent;
		// Tokens
		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken;
		edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           pixelDigisToken;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken;
		edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken;
		// Output file path
		// Default: "Ntuple_scm.root"
		std::string ntupleOutputFilename = "Ntuple_scm.root";
		TFile*      ntupleOutputFile;
		// Tree definition
		TTree*                clusterTree;
		Cluster               clusterField;
		TTree*                mergeTree;
		MergingStatisticsData mergeStatField;
		// Data containers
		// std::multimap<SiPixelCluster&, int> clusterMergeableDigis;

		/////////////////////
		// Data processing //
		/////////////////////

		// void handleClusters(const edm::Handle<edm::DetSetVector<PixelDigi>>& digiCollectionHandle, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		std::map<Trajectory, SiPixelCluster> getTrajClosestClusterMap(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollection, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology);
		void                                 handleClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		std::vector<SiPixelCluster::Pixel>   getDigisOnModule(const edmNew::DetSet<SiPixelCluster>& clusterSetOnModule);
		static const SiPixelCluster*         findBestMergeableClusterCandidate(const SiPixelCluster& cluster, const edmNew::DetSet<SiPixelCluster>& clusterSet);
		static int                           pixelXDistance(SiPixelCluster::Pixel lhs, SiPixelCluster::Pixel rhs);
		static int                           pixelYDistance(SiPixelCluster::Pixel lhs, SiPixelCluster::Pixel rhs);
		static int                           pixelSquareDistance(SiPixelCluster::Pixel lhs, SiPixelCluster::Pixel rhs);
		void                                 saveClusterData(const SiPixelCluster& cluster, DetId detId, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);

		////////////////////
		// Error handling //
		////////////////////

		void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::string msg);
		void handleDefaultError(const std::string& exceptionType, const std::string& streamType, std::vector<std::string> msg);
		void printEvtInfo(const std::string& streamType);

		/////////////
		// Utility //
		/////////////

		void clearAllContainers();

	public:
		SplitClusterMerger(edm::ParameterSet const& iConfig);
		virtual ~SplitClusterMerger();
		virtual void beginJob();
		virtual void endJob();
		virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
};


#endif