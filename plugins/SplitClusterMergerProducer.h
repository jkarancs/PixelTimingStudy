#ifndef SPLIT_CLUSTER_MERGER_PRODUCER_H
#define SPLIT_CLUSTER_MERGER_PRODUCER_H

//////////////////////////
// EDM plugin libraries //
//////////////////////////

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
// #include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

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
// For the tracker topology handle
#include "FWCore/Framework/interface/ESHandle.h"
// Tracker topology
#include "Geometry/Records/interface/TrackerTopologyRcd.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"
// Module data
#include "../interface/ModuleDataProducer.h"
// For the merging
#include "../interface/ClusterPairMergingValidator.h"

///////////
// Other //
///////////

// Errors token
#include "DataFormats/SiPixelRawData/interface/SiPixelRawDataError.h"
// Clusters token
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"
// Cluster geometry functions
#include "../interface/ClusterGeometry.h"

////////////////////
// Root libraries //
////////////////////

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TRandom3.h>

////////////////
// C++ system //
////////////////

#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <functional>


class SplitClusterMergerProducer: public edm::EDProducer 
{
	private:
		// Tokens
		edm::EDGetTokenT<edm::DetSetVector<SiPixelRawDataError>> rawDataErrorToken;
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster>>   clustersToken;
	public:
		SplitClusterMergerProducer(edm::ParameterSet const& iConfig);
		virtual void beginJob();
		virtual void produce(edm::Event& iEvent, const edm::EventSetup& iSetup);
		virtual void endJob();
	private:
		void mergeClusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& clusterCollection, const TrackerTopology* const trackerTopology, const std::map<uint32_t, int>& fedErrors, std::unique_ptr<edm::DetSetVector<SiPixelCluster>>& producedData);
		// Utility
		static bool checkClusterPairOrder(const SiPixelCluster& lhs, const SiPixelCluster& rhs);
		static SiPixelCluster mergeClusterPair(const SiPixelCluster& lhs, const SiPixelCluster& rhs);
};

#endif