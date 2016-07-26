#include "SplitClusterMerger.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG

SplitClusterMerger::SplitClusterMerger(edm::ParameterSet const& iConfig)
{
	LogDebug("step") << "SplitClusterMerger() constructor called." << std::endl;
}

SplitClusterMerger::~SplitClusterMerger()
{
	LogDebug("step") << "~SplitClusterMerger()" << std::endl;
}

void SplitClusterMerger::beginJob()
{
	LogDebug("step") << "Executing SplitClusterMerger::beginJob()..." << std::endl;
	this -> iConfig = iConfig;

	if(iConfig.exists("fileName"))
	{
		ntuple_output_filename = iConfig.getParameter<std::string>("filename");
	}
	// Create output file
	ntuple_output_file = new TFile(ntuple_output_filename.c_str(), "RECREATE");
	if(!(ntuple_output_file -> IsOpen()))
	{
		handle_default_error("file_operations", "file_operations", { "Failed to open output file: ", ntuple_output_filename });
	}
	LogDebug("file_operations") << "Output file: \"" << ntuple_output_filename << "\" created." << std::endl;
	// Branch definitions
	// traj_tree = new TTree("trajTree", "Trajectory measurements in the Pixel");
	// PhaseIDataTrees::define_traj_tree_branches(traj_tree, event_field, traj_field);
}

void SplitClusterMerger::endJob()
{
	LogDebug("step") << "Executing SplitClusterMerger::endJob()..." << std::endl;
	LogDebug("file_operations") << "Writing plots to file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Write();
	LogDebug("file_operations") << "Closing file: \"" << ntuple_output_filename << "\"." << std::endl;
	ntuple_output_file -> Close();
	LogDebug("file_operations") << "File succesfully closed: \"" << ntuple_output_filename << "\"." << std::endl;
}

void SplitClusterMerger::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
	LogDebug("step") << "Executing SplitClusterMerger::analyze()..." << std::endl;
	currentEvent = &iEvent;
	// clear_all_containers();
}

void SplitClusterMerger::beginRun(edm::Run const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing SplitClusterMerger::beginRun()..." << std::endl;
}

void SplitClusterMerger::endRun(edm::Run const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing SplitClusterMerger::endRun()..." << std::endl;
}

void SplitClusterMerger::beginLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing SplitClusterMerger::beginLuminosityBlock()..." << std::endl;
}

void SplitClusterMerger::endLuminosityBlock(edm::LuminosityBlock const&, edm::EventSetup const&)
{
	LogDebug("step") << "Executing SplitClusterMerger::endLuminosityBlock()..." << std::endl;
}

/////////////
// Utility //
/////////////

// void SplitClusterMerger::clear_all_containers()
// {
// }

////////////////////
// Error handling //
////////////////////

void SplitClusterMerger::handle_default_error(const std::string& exception_type, const std::string& stream_type, std::string msg)
{
	edm::LogError(stream_type.c_str()) << c_red << msg << c_def << std::endl;
	print_evt_info(stream_type);
	throw cms::Exception(exception_type.c_str());
}

void SplitClusterMerger::handle_default_error(const std::string& exception_type, const std::string& stream_type, std::vector<std::string> msg)
{
	// event_field.run          = static_cast<int>(iEvent.id().run());
	// event_field.ls           = static_cast<int>(iEvent.luminosityBlock());
	// event_field.orb          = static_cast<int>(iEvent.orbitNumber());
	edm::LogError(stream_type.c_str()) << c_red;
	for(const auto& msg_part: msg)
	{
		edm::LogError(stream_type.c_str()) << msg_part;
	}
	edm::LogError(stream_type.c_str()) << c_def << std::endl;
	print_evt_info(stream_type);
	throw cms::Exception(exception_type.c_str());
}

void SplitClusterMerger::print_evt_info(const std::string& stream_type)
{
	edm::LogError(stream_type.c_str()) << c_blue <<
		"Run: "    << currentEvent -> id().run()        << 
		" Ls: "    << currentEvent -> luminosityBlock() << 
		" Orbit: " << currentEvent -> orbitNumber()     << c_def << std::endl;
}

DEFINE_FWK_MODULE(SplitClusterMerger);