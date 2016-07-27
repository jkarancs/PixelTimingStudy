#include "SplitClusterMerger.h"

#define EDM_ML_LOGDEBUG
#define ML_DEBUG

SplitClusterMerger::SplitClusterMerger(edm::ParameterSet const& iConfig)
{
	LogDebug("step") << "SplitClusterMerger() constructor called." << std::endl;
	// Tokens
	clusters_token = consumes<edmNew::DetSetVector<SiPixelCluster>>(edm::InputTag("siPixelClusters"));
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
	LogDebug("step") << "Executing SplitClusterMerger::analyze()..." << std::endl;
	currentEvent = &iEvent;

	// Fetching the clusters by token
	edm::Handle<edmNew::DetSetVector<SiPixelCluster>> cluster_collection_handle;
	iEvent.getByToken(clusters_token, cluster_collection_handle);
	// Trying to access the clusters
	if(cluster_collection_handle.isValid())
	{
	}

	// clear_all_containers();
}

void SplitClusterMerger::handle_clusters(const edm::Handle<edmNew::DetSetVector<SiPixelCluster>>& cluster_collection_handle)
{
	// const edmNew::DetSetVector<SiPixelCluster>& current_cluster_collection = *cluster_collection_handle;
	// Looping on clusters on the same detector_part
	// typedef edmNew::DetSetVector<SiPixelCluster>::const_iterator clust_coll_it_t;
	for(const auto& cluster_set_on_module: *cluster_collection_handle)
	// for(clust_coll_it_t cluster_set_on_module_it = current_cluster_collection.begin(); cluster_set_on_module_it != current_cluster_collection.end(); ++cluster_set_on_module_it)
	{
		// const auto& cluster_set_on_module = *cluster_set_on_module_it;
		DetId det_id(cluster_set_on_module.id());
		unsigned int subdetId = det_id.subdetId();
		// Take only pixel clusters
		if(subdetId != PixelSubdetector::PixelBarrel && subdetId != PixelSubdetector::PixelEndcap)
		{
			continue;
		}
		// typedef edmNew::DetSet<SiPixelCluster>::const_iterator clust_set_it_t;
		for(const auto& current_cluster: cluster_set_on_module)
		// for(clust_set_it_t current_cluster_it = current_cluster_set.begin(); current_cluster_it != current_cluster_set.end(); ++current_cluster_it)
		{
			// const auto& current_cluster = *current_cluster_it;
			// Serial num of cluster in the given module
			// cluster_field.mod    = get_offline_module_data(det_id.rawId(), tracker_topology, federrors);
			// cluster_field.mod_on = convert_offline_to_online_module_data(cluster_field.mod);
			// Position and size
			// cluster_field.x     = current_cluster.x();
			// cluster_field.y     = current_cluster.y();
			// cluster_field.sizeX = current_cluster.sizeX();
			// cluster_field.sizeY = current_cluster.sizeY();
			// cluster_field.size  = current_cluster.size();
			// Charge
			// cluster_field.charge = current_cluster.charge();
			// Misc.
			// for(int i = 0; i < cluster_field.size && i < 1000; ++i)
			// {
			// 	const auto& current_pixels = current_cluster.pixels();
			// 	cluster_field.adc[i]    = current_cluster.pixelADC()[i] / 1000.0;
			// 	cluster_field.pix[i][0] = current_pixels[i].x;
			// 	cluster_field.pix[i][1] = current_pixels[i].y;
			// }
			// complete_cluster_collection.push_back(cluster_field);
			// The number of saved clusters can be downscaled to save space
		}
	}
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
		" Event: " << currentEvent -> id().event()      << c_def << std::endl;
}

DEFINE_FWK_MODULE(SplitClusterMerger);