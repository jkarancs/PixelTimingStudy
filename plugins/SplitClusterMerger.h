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

///////////
// Other //
///////////

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

		/////////////////
		// Output file //
		/////////////////

		// Default: "Ntuple_scm.root"
		std::string ntuple_output_filename = "Ntuple_scm.root";
		TFile*      ntuple_output_file;

		///////////////////////////////////
		// Tokens for accessing the data //
		///////////////////////////////////

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