#ifndef SPLIT_CLUSTER_MERGER_H
#define SPLIT_CLUSTER_MERGER_H

//////////////////////////
// EDM plugin libraries //
//////////////////////////

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
// #include "FWCore/Framework/interface/ESHandle.h"

////////////////////////////
// Message logger service //
////////////////////////////

#include "FWCore/MessageLogger/interface/MessageLogger.h"
// Adding some colors :)
#include "../interface/Console_colors.h"

///////////
// Tools //
///////////

// Trajectory measurements
// #include "DataFormats/TrackReco/interface/Track.h"
// #include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
// #include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
// #include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
// #include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHit.h"
// #include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
// #include "DataFormats/SiStripDetId/interface/StripSubdetector.h"
// Position
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include "Geometry/TrackerNumberingBuilder/interface/GeometricDet.h"
#include "DataFormats/TrackerCommon/interface/TrackerTopology.h"


///////////
// Other //
///////////

// Clusters token
#include "DataFormats/SiPixelCluster/interface/SiPixelCluster.h"

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

class SplitClusterMerger : public edm::EDAnalyzer
{
	private:
		edm::ParameterSet iConfig;
		// For debug
		const edm::Event* currentEvent;
		// Tokens
		edm::EDGetTokenT<edmNew::DetSetVector<SiPixelCluster> > clusters_token;
		// Output file
		// Default: "Ntuple_scm.root"
		std::string ntuple_output_filename = "Ntuple_scm.root";
		TFile*      ntuple_output_file;

		/////////////////////
		// Data processing //
		/////////////////////

		void handle_clusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& cluster_collection_handle);

		////////////////////
		// Error handling //
		////////////////////

		void handle_default_error(const std::string& exception_type, const std::string& stream_type, std::string msg);
		void handle_default_error(const std::string& exception_type, const std::string& stream_type, std::vector<std::string> msg);
		void print_evt_info(const std::string& stream_type);

		/////////////
		// Utility //
		/////////////

		// void clear_all_containers();

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