#ifndef SPLIT_CLUSTER_ANALYZER_H
#define SPLIT_CLUSTER_ANALYZER_H

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
#include "../interface/PixelDataTree.h"
#include "../interface/EventDataTree.h"
// Module data
#include "../interface/ModuleDataProducer.h"
// Cluster/Pixel geometry
#include "../interface/ClusterGeometry.h"
// Tracker topology
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
// Tracks
#include "DataFormats/TrackReco/interface/Track.h"
#include "../interface/TrajAnalyzer.h"
// Hits
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"

// Position
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
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
// For the merging
#include "../interface/ClusterPairMergingValidator.h"

////////////////////
// Root libraries //
////////////////////

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
// #include <TH1F.h>
#include <TH2I.h>
#include <TRandom3.h>

////////////////
// C++ system //
////////////////

#include <iostream>
#include <string>
#include <vector>
#include <limits>

class SplitClusterAnalyzer : public edm::EDAnalyzer
{
	private:
		// FIXME: This type is just plain ugly
		typedef std::map<TrajectoryMeasurement, SiPixelCluster, std::function<bool(const TrajectoryMeasurement&, const TrajectoryMeasurement&)>> TrajClusterMap;
		edm::ParameterSet iConfig;
		// For debug
		const edm::Event* currentEvent;
		// Tokens
		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken;
		// edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           pixelDigisToken;
		edm::EDGetTokenT<edm::DetSetVector<PixelDigi>>           digiFlagsToken;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken;
		edm::EDGetTokenT<TrajTrackAssociationCollection>         trajTrackCollectionToken;
		// Output file path
		// Default: "Ntuple_scm.root"
		std::string ntupleOutputFilename = "Ntuple_scm.root";
		TFile*      ntupleOutputFile;
		// Tree definition
		TTree*                eventTree;
		EventData             eventField;
		TTree*                clusterTree;
		Cluster               clusterField;
		TTree*                mergeTree;
		MergingStatisticsData mergeStatField;
		TTree*                mergeSnapshotTree;
		TTree*                pixelTree;
		Pixel                 pixelField;
		int                   mergeSnapshotField[1024];
		unsigned int          numSavedSnapshots   = 0;
		unsigned int          maxSnapshotsToSave  = 10;
		unsigned int          numSavedEventPlots  = 0;
		TH2D*                 currentEventPlotLayer1;
		TH2D*                 currentEventPlotLayer2;
		TH2D*                 currentEventPlotLayer3;
		unsigned int          maxEventPlotsToSave = 0;
		// Data containers
		// std::multimap<SiPixelCluster&, int> clusterMergeableDigis;

		/////////////////////
		// Data processing //
		/////////////////////

		TrajClusterMap                     getTrajClosestClusterMap(const edm::Handle<TrajTrackAssociationCollection>& trajTrackCollection, const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology);
		SiPixelCluster                     findClosestCluster(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const uint32_t& rawId, const float& lx, const float& ly);
		void                               handleEvent(const edm::Event& iEvent);
		void                               handleClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollectionHandle, const edm::Handle<edm::DetSetVector<PixelDigi>>& digiFlagsCollection, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& federrors);
		// std::vector<SiPixelCluster::Pixel> getDigisOnModule(const edmNew::DetSet<SiPixelCluster>& clusterSetOnModule);
		// static const SiPixelCluster*       findBestMergeableClusterCandidate(const SiPixelCluster& cluster, const edmNew::DetSet<SiPixelCluster>& clusterSet);
		void                               saveClusterData(const SiPixelCluster& cluster, const ModuleData& mod, const ModuleData& mod_on);
		void                               saveMergingData(const SiPixelCluster& currentCluster, const SiPixelCluster& clusterToMerge, const std::vector<SiPixelCluster::Pixel>& currentClusterPixels, const std::vector<SiPixelCluster::Pixel>& clusterToMergePixels, bool isCurrentClusterSplit, const ModuleData& mod, const ModuleData& mod_on);
		void                               saveSnapshotData(const SiPixelCluster& currentCluster, const SiPixelCluster& clusterToMerge);
		void                               savePixelData(const SiPixelCluster::Pixel& pixelToSave, const ModuleData& mod, const ModuleData& mod_on, const edm::DetSet<PixelDigi>& digiFlagsCollection);
		void                               createEventPlot();
		void                               fillEventPlot(const SiPixelCluster::Pixel& pixelToSave, const ModuleData& mod_on, const edm::DetSet<PixelDigi>& digiFlagsCollection);
		void                               saveEventPlot();

		static bool                        checkClusterPairOrder(const SiPixelCluster& lhs, const SiPixelCluster& rhs);
		static bool                        checkIfDigiIsInDetSet(const PixelDigi& digi, const edm::DetSet<PixelDigi>& digiFlags);
		static bool                        checkIfNextToDcolLostDigi(const SiPixelCluster& clusterToCheck, const edm::DetSet<PixelDigi>& digiFlags);
		static bool                        checkIfNextToDcolLostDigi(const SiPixelCluster::Pixel& pixelToCheck, const edm::DetSet<PixelDigi>& digiFlags);
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
		SplitClusterAnalyzer(edm::ParameterSet const& iConfig);
		virtual ~SplitClusterAnalyzer();
		virtual void beginJob();
		virtual void endJob();
		virtual void analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup);
		virtual void beginRun(edm::Run const&, edm::EventSetup const&);
		virtual void endRun(edm::Run const&, edm::EventSetup const&);
		virtual void beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
		virtual void endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&);
};


#endif