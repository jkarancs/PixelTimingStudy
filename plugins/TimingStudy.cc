//
//
//
//

//
//
// ------------------------------------------------------------------------------------------------
//
#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/MuonTime.h"
#include "DataFormats/MuonDetId/interface/DTChamberId.h"
#include "DataFormats/MuonDetId/interface/MuonSubdetId.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"
#include "DataFormats/Math/interface/deltaPhi.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"
#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/BeamSpot/interface/BeamSpot.h"
#include "DQM/SiStripCommon/interface/SiStripFolderOrganizer.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/PixelClusterParameterEstimator.h"
#include "TimingStudy.h"
//#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include "Geometry/TrackerGeometryBuilder/interface/RectangularPixelTopology.h"
#include <Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h>
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"
#include "FWCore/Common/interface/TriggerNames.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "CLHEP/Random/RandPoissonQ.h"
// SimDataFormats
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

// For ROOT
#include <TROOT.h>
#include <TTree.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TStopwatch.h>

// STD
#include <memory>
#include <string>
#include <iostream>
#include <vector>

// For Resolution
#include "Geometry/CommonTopologies/interface/Topology.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/Records/interface/TransientRecHitRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"

//set to 1 in order to switch on logging of debug info - may create large log file 
//(set to 0 for Grid runs)
#define JKDEBUG 0
#define SPLIT 1

using namespace std;
using namespace edm;
using namespace reco;


TimingStudy::TimingStudy(edm::ParameterSet const& iConfig) : 
  iConfig_(iConfig)
{
  eventTree_=NULL;
  lumiTree_=NULL;
  runTree_=NULL;
  trackTree_=NULL;
  clustTree_=NULL;
  trajTree_=NULL;
  digiTree_=NULL;

  outfile_=NULL;

  extrapolateFrom_=2;
  extrapolateTo_=1;
  maxlxmatch_=0.2;
  maxlymatch_=0.2;
  keepOriginalMissingHit_=true;

  usePixelCPE_=false;
  nthClusterToSave_=100;
  minNStripHits_=0;
  minTrkPt_=0.6;
  useClosestVtx_=false;

  mcLumiScale_=1.0;

  calcWeights_=false;
  mcPileupFile_="";
  mcPileupHistoName_="";
  dataPileupFile_="";
  dataPileupHistoName_="";

  LumiWeights_ = edm::LumiReWeighting();

  isNewLS_ = false;
  lumi_.init(); // ctor of lumi_ should take care of this, but just to be sure

  // Tokens needed after 76X
#if CMSSW_VER >= 76
  condInRunBlockToken_=mayConsume<edm::ConditionsInRunBlock, InRun >(edm::InputTag("conditionsInEdm"));
  condInLumiBlockToken_=mayConsume<edm::ConditionsInLumiBlock, InLumi >(edm::InputTag("conditionsInEdm"));
  lumiSummaryToken_=mayConsume<LumiSummary, InLumi >(edm::InputTag("lumiProducer"));
  if (iConfig_.exists("triggerTag")) {
    triggerTag_=iConfig_.getParameter<edm::InputTag>("triggerTag");
    std::cout<<"NON-DEFAULT PARAMETER: triggerTag= "<<triggerTag_<<std::endl;
  } else triggerTag_=edm::InputTag("TriggerResults","", "HLT");
  triggerResultsToken_=consumes<edm::TriggerResults>(triggerTag_);
  muonCollectionToken_=consumes<reco::MuonCollection>(edm::InputTag("muonsWitht0Correction"));
  pileupSummaryToken_=consumes<std::vector<PileupSummaryInfo> >(edm::InputTag("addPileupInfo"));
  l1tscollectionToken_=consumes<Level1TriggerScalersCollection>(edm::InputTag("scalersRawToDigi"));
  std::string trajTrackCollectionInput = iConfig.getParameter<std::string>("trajectoryInput");
  trajTrackCollToken_=consumes<TrajTrackAssociationCollection>(edm::InputTag(trajTrackCollectionInput));
  pixelDigiCollectionToken_=consumes<edm::DetSetVector<PixelDigi> >(edm::InputTag("siPixelDigis"));
  trackingErrorToken_=consumes<edm::EDCollection<DetId> >(edm::InputTag("siPixelDigis"));
  userErrorToken_=consumes<edm::EDCollection<DetId> >(edm::InputTag("siPixelDigis", "UserErrorModules"));
  rawDataErrorToken_=consumes<edm::DetSetVector<SiPixelRawDataError> >(edm::InputTag("siPixelDigis"));
  primaryVerticesToken_=consumes<reco::VertexCollection>(edm::InputTag("offlinePrimaryVertices"));
  clustersToken_=consumes<edmNew::DetSetVector<SiPixelCluster> >(edm::InputTag("siPixelClusters"));
  measTrackerEvtToken_=consumes<MeasurementTrackerEvent>(edm::InputTag("MeasurementTrackerEvent"));
#endif
}


TimingStudy::~TimingStudy() { }  


void TimingStudy::endJob() 
{
  outfile_->Write();
  outfile_->Close();
  delete outfile_;
}


void TimingStudy::beginJob()
{

  std::string fileName="test.root";
  if (iConfig_.exists("fileName")) {
    fileName=iConfig_.getParameter<std::string>("fileName");
    std::cout<<"NON-DEFAULT PARAMETER: fileName= "<<fileName<<std::endl;
  }
  if (iConfig_.exists("extrapolateFrom")) {
    extrapolateFrom_=iConfig_.getParameter<int>("extrapolateFrom");
    std::cout<<"NON-DEFAULT PARAMETER: extrapolateFrom= "<<extrapolateFrom_<<std::endl;
  }
  if (iConfig_.exists("extrapolateTo")) {
    extrapolateTo_=iConfig_.getParameter<int>("extrapolateTo");
    std::cout<<"NON-DEFAULT PARAMETER: extrapolateTo= "<<extrapolateTo_<<std::endl;
  }
  if (iConfig_.exists("maxLxMatch")) {
    maxlxmatch_=iConfig_.getParameter<int>("maxLxMatch");
    std::cout<<"NON-DEFAULT PARAMETER: maxLxMatch= "<<maxlxmatch_<<std::endl;
  }
  if (iConfig_.exists("maxLyMatch")) {
    maxlymatch_=iConfig_.getParameter<int>("maxLyMatch");
    std::cout<<"NON-DEFAULT PARAMETER: maxLyMatch= "<<maxlymatch_<<std::endl;
  }
  if (iConfig_.exists("keepOriginalMissingHit")) {
    keepOriginalMissingHit_=iConfig_.getParameter<bool>("keepOriginalMissingHit");
    std::cout<<"NON-DEFAULT PARAMETER: keepOriginalMissingHit= "<<keepOriginalMissingHit_
	     <<std::endl;
  }
  if (iConfig_.exists("usePixelCPE")) {
    usePixelCPE_=iConfig_.getParameter<bool>("usePixelCPE");
    std::cout<<"NON-DEFAULT PARAMETER: usePixelCPE= "<<usePixelCPE_<<std::endl;
  }
  if (iConfig_.exists("nthClusterToSave")) {
    nthClusterToSave_=iConfig_.getParameter<int>("nthClusterToSave");
    std::cout<<"NON-DEFAULT PARAMETER: nthClusterToSave= "<<nthClusterToSave_<<std::endl;
  }
  if (iConfig_.exists("minNStripHits")) {
    minNStripHits_=iConfig_.getParameter<int>("minNStripHits");
    std::cout<<"NON-DEFAULT PARAMETER: minNStripHits= "<<minNStripHits_<<std::endl;
  }
  if (iConfig_.exists("minTrkPt")) {
    minTrkPt_=iConfig_.getParameter<double>("minTrkPt");
    std::cout<<"NON-DEFAULT PARAMETER: minTrkPt= "<<minTrkPt_<<std::endl;
  }
  if (iConfig_.exists("useClosestVtx")) {
    useClosestVtx_=iConfig_.getParameter<bool>("useClosestVtx");
    std::cout<<"NON-DEFAULT PARAMETER: useClosestVtx= "<<useClosestVtx_<<std::endl;
  }
#if CMSSW_VER < 76
  if (iConfig_.exists("triggerTag")) {
    triggerTag_=iConfig_.getParameter<edm::InputTag>("triggerTag");
    std::cout<<"NON-DEFAULT PARAMETER: triggerTag= "<<triggerTag_<<std::endl;
  } else {
    triggerTag_=edm::InputTag("TriggerResults","", "HLT");
  }
#endif
  if (iConfig_.exists("triggerNames")) {
    triggerNames_=iConfig_.getParameter<std::vector<std::string> >("triggerNames");
    std::cout<<"NON-DEFAULT PARAMETER: triggerNames= ";
    for (size_t i=0; i<triggerNames_.size(); i++) std::cout<<triggerNames_[i]<<" ";
    std::cout<<std::endl;
  }
  if (iConfig_.exists("mcLumiScale")) {
    mcLumiScale_=iConfig_.getParameter<double>("mcLumiScale");
    std::cout<<"NON-DEFAULT PARAMETER: mcLumiScale= "<<mcLumiScale_<<std::endl;
  }
  
  // Read instantaneous luminosity and average pileup for data externally from a txt file
  std::string instlumi_txt = iConfig_.getUntrackedParameter<std::string>("instlumiTextFile", "run_ls_instlumi_pileup_2015.txt");

  // Pileup reweighting
  if (iConfig_.exists("mcPileupFile")&&iConfig_.exists("mcPileupHistoName")&&
      iConfig_.exists("dataPileupFile")&&iConfig_.exists("dataPileupHistoName")) {
    mcPileupFile_=iConfig_.getParameter<std::string>("mcPileupFile");
    mcPileupHistoName_=iConfig_.getParameter<std::string>("mcPileupHistoName");
    dataPileupFile_=iConfig_.getParameter<std::string>("dataPileupFile");
    dataPileupHistoName_=iConfig_.getParameter<std::string>("dataPileupHistoName");

    std::cout<<"NON-DEFAULT PARAMETERS: mcPileupFile= "<<mcPileupFile_<<std::endl;
    std::cout<<"mcPileupHistoName= "<<mcPileupHistoName_<<std::endl;
    std::cout<<"dataPileupFile= "<<dataPileupFile_<<std::endl;
    std::cout<<"dataPileupHistoName= "<<dataPileupHistoName_<<std::endl;

    calcWeights_ = true;
  }
  clu_stat_counter_=0;

//   es.get<TrackerDigiGeometryRecord>().get(tkGeom_);
//   es.get<IdealMagneticFieldRecord>().get(magneticField_);

  //edm::Service<TFileService> fs;
  outfile_ = new TFile(fileName.c_str(), "RECREATE");
  std::cout<<"Output file created: "<<outfile_->GetName()<<std::endl;

  // The event
  eventTree_ = new TTree("eventTree", "The event");
  eventTree_->Branch("event", &evt_, evt_.list.data());

  // The lumi
  lumiTree_ = new TTree("lumiTree", "The lumi");
  lumiTree_->Branch("lumi", &lumi_, lumi_.list.data());

  // The run
  runTree_ = new TTree("runTree", "The run");
  runTree_->Branch("run", &run_, run_.list.data());

  #ifndef SPLIT
  #ifdef COMPLETE
  TrackData track_;
  trackTree_ = new TTree("trackTree", "The track in the event");
  //trackTree_->SetDirectory(outfile_);
  //trackTree_->AutoSave();
  trackTree_->Branch("event", &evt_, evt_.list.data());
  trackTree_->Branch("track", &track_, track_.list.data());


  Cluster clust;
  clustTree_ = new TTree("clustTree", "Pixel clusters");
  //clustTree_->SetDirectory(outfile_);
  //clustTree_->AutoSave();
  clustTree_->Branch("event", &evt_, evt_.list.data());
  clustTree_->Branch("clust", &clust, clust.list.data());
  clustTree_->Branch("clust_pix", &clust.pix, "pix[size][2]/F");
  clustTree_->Branch("module", &clust.mod, clust.mod.list.data());
  clustTree_->Branch("module_on", &clust.mod_on, clust.mod_on.list.data());
  #endif

  TrajMeasurement trajmeas;
  trajTree_ = new TTree("trajTree", "Trajectory measurements in the Pixel");
  //trajTree_->SetDirectory(outfile_);
  //trajTree_->AutoSave();
  trajTree_->Branch("event", &evt_, evt_.list.data());
  trajTree_->Branch("traj", &trajmeas, trajmeas.list.data());
  #ifdef COMPLETE
  trajTree_->Branch("module", &trajmeas.mod, trajmeas.mod.list.data());
  #endif
  trajTree_->Branch("module_on", &trajmeas.mod_on, trajmeas.mod_on.list.data());
  trajTree_->Branch("clust", &trajmeas.clu, trajmeas.clu.list.data());
  trajTree_->Branch("clust_pix", &trajmeas.clu.pix, "pix[size][2]/F");
  trajTree_->Branch("track", &trajmeas.trk, trajmeas.trk.list.data());


  #ifdef COMPLETE
  Digi digi;
  digiTree_ = new TTree("digiTree", "Pixel digis");
  //digiTree_->SetDirectory(outfile_);
  //digiTree_->AutoSave();
  digiTree_->Branch("event", &evt_, evt_.list.data());
  digiTree_->Branch("digi", &digi, digi.list.data());
  digiTree_->Branch("module", &digi.mod, digi.mod.list.data());
  digiTree_->Branch("module_on", &digi.mod_on, digi.mod_on.list.data());
  #endif

  // Split mode
  #else
  TrajMeasurement trajmeas;
  trajTree_ = new TTree("trajTree", "Trajectory measurements in the Pixel");
  //trajTree_->SetDirectory(outfile_);
  //trajTree_->AutoSave();
  
  // Non-splitted branches
  trajTree_->Branch("event", &evt_, evt_.list.data());
  trajTree_->Branch("module_on", &trajmeas.mod_on, trajmeas.mod_on.list.data());
  
  // traj
  // Non-splitted branch
  trajTree_->Branch("traj", &trajmeas, "validhit/I:missing:lx/F:ly:res_dx:res_dz:lev:clust_near/I:hit_near:pass_effcuts");
  #if SPLIT > 0
  // Paired branches
  trajTree_->Branch("traj_occup",            &trajmeas.nclu_mod,        "nclu_mod/I:nclu_roc:npix_mod:npix_roc");
  trajTree_->Branch("traj_alphabeta",        &trajmeas.alpha,           "alpha/F:beta");
  trajTree_->Branch("traj_dxy_cl",           &trajmeas.dx_cl,           "dx_cl[2]/F:dy_cl[2]");
  trajTree_->Branch("traj_dxy_hit",          &trajmeas.dx_hit,          "dx_hit/F:dy_hit");
  #endif
  // Split-mode branches
  trajTree_->Branch("traj_norm_charge",      &trajmeas.norm_charge,     "norm_charge/F");
  #ifdef COMPLETE
  trajTree_->Branch("traj_lz",               &trajmeas.lz,              "lz/F");
  trajTree_->Branch("traj_glx",              &trajmeas.glx,             "glx/F");
  trajTree_->Branch("traj_gly",              &trajmeas.gly,             "gly/F");
  trajTree_->Branch("traj_glz",              &trajmeas.glz,             "glz/F");
  trajTree_->Branch("traj_lxmatch",          &trajmeas.lxmatch,         "lxmatch/F");
  trajTree_->Branch("traj_lymatch",          &trajmeas.lymatch,         "lymatch/F");
  trajTree_->Branch("traj_i",                &trajmeas.i,               "i/I");
  trajTree_->Branch("traj_onedge",           &trajmeas.onedge,          "onedge/I");
  trajTree_->Branch("traj_inactive",         &trajmeas.inactive,        "inactive/I");
  trajTree_->Branch("traj_badhit",           &trajmeas.badhit,          "badhit/I");
  //   trajTree_->Branch("traj_telescope",        &trajmeas.telescope,       "telescope/I"); // Not used variables
  //   trajTree_->Branch("traj_telescope_valid",  &trajmeas.telescope_valid, "telescope_valid/I");
  //   trajTree_->Branch("traj_dmodule",          &trajmeas.dmodule,         "dmodule/I");
  //   trajTree_->Branch("traj_dladder",          &trajmeas.dladder,         "dladder/I");
  //   trajTree_->Branch("traj_glmatch",          &trajmeas.glmatch,         "glmatch/F");
  //   trajTree_->Branch("traj_lx_err",           &trajmeas.lx_err,          "lx_err/F");
  //   trajTree_->Branch("traj_ly_err",           &trajmeas.ly_err,          "ly_err/F");
  //   trajTree_->Branch("traj_lz_err",           &trajmeas.lz_err,          "lz_err/F");
  //   trajTree_->Branch("traj_lxymatch",         &trajmeas.lxymatch,        "lxymatch/F");
  //   trajTree_->Branch("traj_res_hit",          &trajmeas.res_hit,         "res_hit/F");
  //   trajTree_->Branch("traj_sig_hit",          &trajmeas.sig_hit,         "sig_hit/F");
  //   trajTree_->Branch("traj_d_cl",             &trajmeas.d_cl,            "d_cl[2]/F");
  #endif
  
  // clust
  // Paired branches
  #if SPLIT > 1
  trajTree_->Branch("clust_xy",              &trajmeas.clu.x,           "x/F:y");
  #endif
  // Split-mode branches
  #ifdef COMPLETE
  //   trajTree_->Branch("clust_i",               &trajmeas.clu.i,           "i/I"); // NOVAL_I (trajTree)
  trajTree_->Branch("clust_edge",            &trajmeas.clu.edge,        "edge/I");
  trajTree_->Branch("clust_badpix",          &trajmeas.clu.badpix,      "badpix/I");
  trajTree_->Branch("clust_tworoc",          &trajmeas.clu.tworoc,      "tworoc/I");
  #endif
  trajTree_->Branch("clust_size",            &trajmeas.clu.size,        "size/I");
  trajTree_->Branch("clust_sizeXY",          &trajmeas.clu.sizeX,       "sizeX/I:sizeY");
  trajTree_->Branch("clust_adc",             &trajmeas.clu.pix,         "adc[size]/F");
  #if SPLIT > 1
  trajTree_->Branch("clust_charge",          &trajmeas.clu.charge,      "charge/F");
  trajTree_->Branch("clust_pix",             &trajmeas.clu.pix,         "pix[size][2]/F");
  #endif
  
  // track
  // Non-splitted branch
  trajTree_->Branch("track", &trajmeas.trk, "validfpix[2]/I:validbpix[3]:strip:nstripmissing:nstriplost:nstriplayer:quality:d0/F:dz:pt");
  // Paired branches
  trajTree_->Branch("track_ndofchi2",        &trajmeas.trk.ndof,        "ndof/F:chi2");
  // Split-mode branches
  trajTree_->Branch("track_eta",             &trajmeas.trk.eta,         "eta/F");
  trajTree_->Branch("track_phi",             &trajmeas.trk.phi,         "phi/F");
  #ifdef COMPLETE
  trajTree_->Branch("track_theta",           &trajmeas.trk.theta,       "theta/F");
  trajTree_->Branch("track_p",               &trajmeas.trk.p,           "p/F");
  trajTree_->Branch("track_algo",            &trajmeas.trk.algo,        "algo/I");
  trajTree_->Branch("track_i",               &trajmeas.trk.i,           "i/I");
  trajTree_->Branch("track_pix",             &trajmeas.trk.pix,         "pix/I");
  trajTree_->Branch("track_pixhit",          &trajmeas.trk.pixhit,      "pixhit[2]/I");
  trajTree_->Branch("track_validpixhit",     &trajmeas.trk.validpixhit, "validpixhit[2]/I");
  trajTree_->Branch("track_fpix",            &trajmeas.trk.fpix,        "fpix[2]/I");
  trajTree_->Branch("track_bpix",            &trajmeas.trk.bpix,        "bpix[3]/I");
  trajTree_->Branch("track_highPurity",      &trajmeas.trk.highPurity,  "highPurity/I");
  //   trajTree_->Branch("track_fromVtx",         &trajmeas.trk.fromVtx,     "fromVtx/I"); // old
  #endif
  
  Cluster clust;
  clustTree_ = new TTree("clustTree", "Pixel clusters");
  //clustTree_->SetDirectory(outfile_);
  //clustTree_->AutoSave();

  // Non-splitted branches
  clustTree_->Branch("event", &evt_, evt_.list.data());
  clustTree_->Branch("module_on", &clust.mod_on, clust.mod_on.list.data());

  // clust
  // Paired branches
  clustTree_->Branch("clust_xy",              &clust.x,               "x/F:y");
  // Split-mode branches
  clustTree_->Branch("clust_i",               &clust.i,               "i/I");
  clustTree_->Branch("clust_edge",            &clust.edge,            "edge/I");
  clustTree_->Branch("clust_badpix",          &clust.badpix,          "badpix/I");
  clustTree_->Branch("clust_tworoc",          &clust.tworoc,          "tworoc/I");
  clustTree_->Branch("clust_size",            &clust.size,            "size/I");
  clustTree_->Branch("clust_sizeXY",          &clust.sizeX,           "sizeX/I:sizeY");
  clustTree_->Branch("clust_charge",          &clust.charge,          "charge/F");
  clustTree_->Branch("clust_adc",             &clust.adc,             "adc[size]/F");
  clustTree_->Branch("clust_pix",             &clust.pix,             "pix[size][2]/F");
  
  #ifdef COMPLETE
  TrackData track_;
  trackTree_ = new TTree("trackTree", "The track in the event");
  //trackTree_->SetDirectory(outfile_);
  //trackTree_->AutoSave();
  trackTree_->Branch("event", &evt_, evt_.list.data());
  trackTree_->Branch("track", &track_, track_.list.data());
  #endif
  #endif

  // Some external information until I figure out how to do this inside CMSSW

  // Defining read-out groups
  std::cout << "\nReading portcardmap.dat\n";
  std::ifstream portcardmap_file;
  portcardmap_file.open ("portcardmap.dat", ifstream::in);
  std::string port, mod, ch;
  int aoh;
  for (int i=0; i<5; i++) if (portcardmap_file.good()) portcardmap_file>>port;

  while (portcardmap_file.good()) {
    ch="";
    portcardmap_file >> port >> mod;
    if (port.find("FPix")!=std::string::npos) portcardmap_file >> aoh;
    else portcardmap_file >> ch >> aoh;
    if (!portcardmap_file.eof()) {
      if (ch!="") { mod+="_"; mod+=ch; }
      portcardmap.insert(std::pair<std::string,std::string>(mod,port));
      if (JKDEBUG) std::cout << port << " " << mod << " " << aoh << std::endl;
    }
  }
  portcardmap_file.close();


  // Get the configured delays for each run in 2009 until Oct23
  std::cout << "\nReading Runs_and_delays.txt\n";
  std::ifstream runsndelays_file;
  runsndelays_file.open ("Runs_and_delays.txt", ifstream::in);
  int delay=NOVAL_I;

  while (runsndelays_file.good()) {
    std::string var=""; 
    runsndelays_file >> var;
    //std::cout << "Read " << var;

    if (var.find(".log")!=std::string::npos) { // Reset delay when reading a new log file
      delay=NOVAL_I;
      //std::cout << " - Reset delay" << endl;
    } else if (var.find("elay")!=std::string::npos) { // Read delay
      runsndelays_file >> std::hex >> delay;
      //std::cout << " - Delay set to " << delay << endl;
    } else if (var.find("Run")!=std::string::npos) { // Read delay
      size_t run=0;
      runsndelays_file >> std::dec >> run;
      //std::cout << " - Run " << run;

      std::map<size_t,int>::iterator it=globaldelay.find(run);
      if (it==globaldelay.end()) {
	globaldelay.insert(std::pair<size_t,int>(run, delay));
	//std::cout << " new run, delay " << delay << endl;
      } else {
	//if (it->second!=delay) std::cout << " old run with new delay!!!";
	//std::cout << std::endl;
      }
    }
  }
  runsndelays_file.close();


  // Get the configured WBC for each run in 2009 until Oct23
  std::cout << "\nReading Runs_and_dacs.txt\n";
  std::ifstream runsndacs_file;
  runsndacs_file.open ("Runs_and_dacs.txt", ifstream::in);
  int dac=NOVAL_I;

  while (runsndacs_file.good()) {
    std::string var=""; 
    runsndacs_file >> var;
    //std::cout << "Read " << var;

    if (var.find(".log")!=std::string::npos) { // Reset wbc when reading a new log file
      dac=NOVAL_I;
      //std::cout << " - Reset wbc" << endl;
    } else if (var.find("WBC")!=std::string::npos) { // Read WBC
      runsndacs_file >> dac;
      //std::cout << " - WBC set to " << dac << endl;
    } else if (var.find("dac")!=std::string::npos) { // Read dac
      runsndacs_file >> dac;
      dac*=-1;
      //std::cout << " - WBC set to " << dac << endl;
    } else if (var.find("Run")!=std::string::npos) { // Read delay
      size_t run=0;
      runsndacs_file >> run;
      //std::cout << " - Run " << run;

      std::map<size_t,int>::iterator it=wbc.find(run);
      if (it==wbc.end()) {
	wbc.insert(std::pair<size_t,int>(run, dac));
	//std::cout << " new run, WBC " << dac << endl;
      } else {
	//if (it->second!=dac) std::cout << " old run with new WBC!!!";
	//std::cout << std::endl;
      }
    }
  }
  runsndacs_file.close();

  if (JKDEBUG) {
    ModuleData modtemp;
    for (std::map<int, std::string>::iterator it = modtemp.federrortypes.begin();
	 it!=modtemp.federrortypes.end(); ++it)
      std::cout << "FED Error type (" << (*it).first << ") : " << (*it).second << std::endl;
  }

  // Read InstLumi (delivered) and Pileup from text file generated with lumiCalc2.py
  int run = 0;
  int ls = 0;
  unsigned long int runls = 0;
  double instlumi = 0.0;
  double pileup = 0.0;
  FILE * input = fopen (instlumi_txt.c_str(),"r");
  if (input) std::cout<<"Reading "<<instlumi_txt<<std::endl;
  else {
    std::cout<<instlumi_txt<<" not found,"<<std::endl;
    std::cout<<"instlumi_ext and pileup variables not filled."<<std::endl;
  }
  int a = (input!=0);

  while (a==1) {
    a = fscanf (input, "%d", &run);
    a = fscanf (input, "%d", &ls);
    a = fscanf (input, "%lf", &instlumi);
    a = fscanf (input, "%lf", &pileup);
    if (a==1) {
      runls = run * 100000 + ls;
      runls_instlumi_[runls] = instlumi;
      runls_pileup_[runls] = pileup;
    }
  }
  if (input) fclose (input);

  // Initialize LumiReWeighting
  if (calcWeights_) LumiWeights_ = edm::LumiReWeighting(mcPileupFile_, dataPileupFile_,
							mcPileupHistoName_, dataPileupHistoName_);

  edm::LogInfo("TimingStudy") << "Begin Job Finished" << std::endl;

}

void TimingStudy::beginRun(edm::Run const& iRun,
			   edm::EventSetup const& iSetup){
  run_.init();
  badroc_list_.clear();

  run_.run = iRun.run();
  
  // get ConditionsInRunBlock for fill number
  edm::Handle<edm::ConditionsInRunBlock> condInRunBlock;
#if CMSSW_VER >= 76
  iRun.getByToken(condInRunBlockToken_, condInRunBlock);
#else
  iRun.getByLabel("conditionsInEdm", condInRunBlock);
#endif
  if (condInRunBlock.isValid()) {
    run_.fill = condInRunBlock->lhcFillNumber;
  } else if (run_.run==1) {
    run_.fill = 0;
  } else {
    std::cout<<"** ERROR (beginRun): no condInRunBlock info is available\n";
    std::cout<<"** WARNING: Bad ROC List cannot be loaded\n";
    return;
  }
  std::cout<<"Begin Run: "<<run_.run<<" in Fill: "<<run_.fill<<std::endl;
  
  // Load BadRocList from text file for the specific fill
  if (run_.run!=1) {
    int fill = -1, badroc = -1;
    FILE *f = fopen("fills_and_badrocs.txt","r");
    if (f==NULL) {
      std::cout<<"** WARNING: Bad ROC List (fills_and_badrocs.txt) was not found\n";
    } else {
      while (fscanf(f, "%d %d", &fill, &badroc)!=-1)
	if (fill==run_.fill) badroc_list_.insert(badroc);
      if (badroc_list_.empty())
	std::cout<<"** WARNING: Bad ROC List is empty for fill "<<run_.fill<<std::endl;
      fclose(f);
    }
  }
}

void TimingStudy::endRun(edm::Run const& iRun,
			   edm::EventSetup const& iSetup){
  // get ConditionsInRunBlock
  edm::Handle<edm::ConditionsInRunBlock> condInRunBlock;
#if CMSSW_VER >= 76
  iRun.getByToken(condInRunBlockToken_, condInRunBlock);
#else
  iRun.getByLabel("conditionsInEdm", condInRunBlock);
#endif
  if (!condInRunBlock.isValid()) {
    std::cout<<"** ERROR (endRun): no condInRunBlock info is available\n";
  } else {
    run_.fill = condInRunBlock->lhcFillNumber;
    run_.run = iRun.run();
    std::cout<<"End Run: "<<run_.run<<" in Fill: "<<run_.fill<<std::endl;
  }
  runTree_->Fill();
}

void TimingStudy::beginLuminosityBlock(edm::LuminosityBlock const& iLumi, 
				       edm::EventSetup const& iSetup){

  // isNewLS_ should be set false either in the ctor or by the endLumiBlock(..
  //   assert(isNewLS_==false); 
  isNewLS_=true;
  
  assert(lumi_.run==NOVAL_I);
  assert(lumi_.ls==NOVAL_I);

}


void TimingStudy::endLuminosityBlock(edm::LuminosityBlock const& iLumi, 
				     edm::EventSetup const& iSetup){

  // isNewLS_ should be set false by the first event, if it is not false,
  // there was no event in this lumisection!!!!
  //   assert(isNewLS_==false);

  edm::Handle<LumiSummary> lumi;
#if CMSSW_VER >= 76
  iLumi.getByToken(lumiSummaryToken_, lumi);
#else
  iLumi.getByLabel("lumiProducer", lumi);
#endif
  if (!lumi.isValid()) {
    std::cout<<"** WARNING: no LumiSummary info is available, are you running on RAW or ReReco?\n";
    lumi_.init();
  } else {
    lumi_.init(); // temporal values deleted, now we fill it for real
    lumi_.intlumi=lumi->intgRecLumi();
    lumi_.instlumi=lumi->avgInsDelLumi();
    std::cout<<"L1 trigger prescales\n";
    lumi_.l1_size = lumi->nTriggerLine();
    for (size_t iL1=0; iL1<lumi->nTriggerLine(); iL1++) {
      lumi_.l1_prescale[iL1] = lumi->l1info(iL1).prescale;
      if (DEBUG) std::cout<<"\tPS: "<<lumi->l1info(iL1).prescale<<std::endl;
      /* Note: input/ratecounts are no longer stored
	 The trigger name info can only be accessed from vectors
	 in a new object: LumiSummaryRunHeader
	 RECO data: Cannot test until new RECO data ready
	 RAW  data: This info is created in endRun only, match recursively? */
    }
  }

  // get ConditionsInLumiBlock
  edm::Handle<edm::ConditionsInLumiBlock> cond;
#if CMSSW_VER >= 76
  iLumi.getByToken(condInLumiBlockToken_, cond);
#else
  iLumi.getByLabel("conditionsInEdm", cond);
#endif
  if (!cond.isValid()) {
    std::cout<<"** ERROR: no ConditionsInLumiBlock info is available\n";
    return;
  }

  // check that this lumiblock info is consistent with the last event
  //   assert(lumi_.run == int(iLumi.run()));
  //   assert(lumi_.ls == int(iLumi.luminosityBlock()));

  lumi_.fill=run_.fill;
  lumi_.run=iLumi.run();
  lumi_.ls=iLumi.luminosityBlock();
  lumi_.time=iLumi.beginTime().unixTime();
  
  // Loading externally provided variables 
  // InstLumi in units of ub-1s-1
  // Implying run == 1 for MC
  unsigned long int runls = lumi_.run*100000 + lumi_.ls;
  lumi_.instlumi_ext = (lumi_.run==1) ? NOVAL_F :
    (runls_instlumi_.count(runls) ? runls_instlumi_[runls] * 1000 / 23.3104 : NOVAL_F);
  lumi_.pileup = (lumi_.run==1) ? NOVAL_F : (runls_pileup_.count(runls) ? runls_pileup_[runls] : NOVAL_F);
  
  lumi_.beamint[0]=cond->totalIntensityBeam1;
  lumi_.beamint[1]=cond->totalIntensityBeam2;
  std::cout<< "New lumi block: Run "<<lumi_.run<<" LS = "<<lumi_.ls;
  std::cout << " inst lumi "<<lumi_.instlumi<<" int lumi "<<lumi_.intlumi<<std::endl;
  std::cout << " beam 1 int "<<lumi_.beamint[0]<<" beam 2 int "<<lumi_.beamint[1]<<std::endl;

  lumiTree_->Fill();

  // make sure that the beginLuminosityBlock can check that we were called:
  lumi_.init();

}


void TimingStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  if (isNewLS_==true) { // beginLuminosityBlock() was just called
    lumi_.run=iEvent.id().run();
    lumi_.ls=iEvent.luminosityBlock();
    isNewLS_=false;
  } else {
    // if following fails, it can mean two things:
    // 1. either beginLuminosityBlock() was not called, e.g. LuminosityBlock
    //    is not in sync with the data, but we changed to a new LS block
    // 2. or an event from another lumi block got mixed here
    assert(lumi_.run == int(iEvent.id().run()));
    assert(lumi_.ls == int(iEvent.luminosityBlock()));
  }

  TStopwatch w;
  if (JKDEBUG) w.Start();
  init_all(); // clear all containers
  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Initializing event variables\n";
    w.Print();
  }
  //iSetup.get<TrackerDigiGeometryRecord>().get(tkGeom_);
  iSetup.get<IdealMagneticFieldRecord>().get(magneticField_);


  //
  // Read event info
  //
  if (JKDEBUG) w.Start();
  evt_.fill=run_.fill;
  evt_.run=iEvent.id().run();
  evt_.evt=iEvent.id().event();
  evt_.ls=iEvent.luminosityBlock();

  edm::LuminosityBlock const& iLumi = iEvent.getLuminosityBlock();
  edm::Handle<LumiSummary> lumi;
#if CMSSW_VER >= 76
  iLumi.getByToken(lumiSummaryToken_, lumi);
#else
  iLumi.getByLabel("lumiProducer", lumi);
#endif
  // This will only work when running on RECO until they fix the bug in the FW.
  // When running on RAW and reconstructing the LumiSummary, it will not appear
  // in the event before reaching endLuminosityBlock(). Therefore, it is not
  // possible to fill every event with this info. However, if the bug is fixed
  // running or RAW should produce the same result as RECO
  if (lumi.isValid()) {
    evt_.intlumi=lumi->intgRecLumi();
    evt_.instlumi=lumi->avgInsDelLumi();
  } else {
    // std::cout << "** ERROR: LumiSummary missing\n";
    evt_.intlumi=NOVAL_F;
    evt_.instlumi=NOVAL_F;
  }

  edm::Handle<edm::ConditionsInLumiBlock> cond;
#if CMSSW_VER >= 76
  iLumi.getByToken(condInLumiBlockToken_, cond);
#else
  iLumi.getByLabel("conditionsInEdm", cond);
#endif
  if (cond.isValid()) {
    evt_.beamint[0]=cond->totalIntensityBeam1;
    evt_.beamint[1]=cond->totalIntensityBeam2;
  } else {
    // std::cout << "** ERROR: conditionsInEdm block missing\n";
    evt_.beamint[0]=evt_.beamint[1]=abs(NOVAL_I);
  }

  edm::Handle<Level1TriggerScalersCollection> l1trig;
#if CMSSW_VER >= 76
  iEvent.getByToken(l1tscollectionToken_, l1trig);
#else
  iEvent.getByLabel("scalersRawToDigi", l1trig);
#endif
  if (l1trig.isValid() && l1trig->size()!=0) {
    evt_.l1_rate=l1trig->begin()->gtTriggersRate();
  } else {
    // std::cout<<"** ERROR: L1 Trigger Information missing"<<std::endl;
    evt_.l1_rate=NOVAL_F;
  }
  

  // For Monte Carlo, estimate the instantaneous luminosity based on number of true interactions
  //
  if (evt_.run==1) {
    edm::Handle<std::vector<PileupSummaryInfo> > puInfo;
#if CMSSW_VER >= 76
    iEvent.getByToken(pileupSummaryToken_, puInfo);
#else
    iEvent.getByLabel("addPileupInfo", puInfo);
#endif
    
    if (puInfo.isValid()) {
      // look for the intime PileupSummaryInfo
      std::vector<PileupSummaryInfo>::const_iterator pu;
      std::vector<PileupSummaryInfo>::const_iterator pu0=puInfo->end();

      for(pu=puInfo->begin(); pu!=puInfo->end(); ++pu) {
	if (pu->getBunchCrossing()==0) pu0=pu;
	if (JKDEBUG) {
	  std::cout<<"BX="<<pu->getBunchCrossing()<<" - N_int="<<pu->getPU_NumInteractions()<<" / "
		   <<"N_true="<<pu->getTrueNumInteractions()<<"\n";
	}
      }

      if(pu0!=puInfo->end()) {
	float pileup = pu0->getTrueNumInteractions();
	evt_.pileup = pileup;
	evt_.instlumi = pileup * mcLumiScale_;
	if (calcWeights_) evt_.weight = LumiWeights_.weight(pileup);
      } else {
	std::cout<<"** ERROR: Cannot find the in-time pileup info\n";
      }
    } else std::cout<<"** WARNING: PileupInfo invalid\n";

  }

  // Loading externally provided variables 
  // InstLumi in units of ub-1s-1
  // Implying run == 1 for MC
  unsigned long int runls = evt_.run*100000 + evt_.ls;
  evt_.instlumi_ext = (evt_.run==1) ? NOVAL_F :
    (runls_instlumi_.count(runls) ? runls_instlumi_[runls] * 1000 / 23.3104 : NOVAL_F);
  evt_.pileup = (evt_.run==1) ? evt_.pileup : (runls_pileup_.count(runls) ? runls_pileup_[runls] : NOVAL_F);
  if (evt_.run!=1) evt_.weight = 1.0;

  evt_.good=NOVAL_I;

  if (evt_.run==124022) {
    if (evt_.ls>=66 && evt_.ls<=160) evt_.good=1;
  } else if (evt_.run==124023) {
    if (evt_.ls>=41 && evt_.ls<=96) evt_.good=1;
  } else if (evt_.run==124024) {
    if (evt_.ls<=83 && evt_.ls>=2) evt_.good=1;
  } else if (evt_.run==124025) {
    if (evt_.ls>=3 && evt_.ls<=13) evt_.good=1;
  } else if (evt_.run==124027) {
    if (evt_.ls>=23 && evt_.ls<=39) evt_.good=1;
  } else if (evt_.run==124030) {
    if (evt_.ls<=32) evt_.good=1;
  } else if (evt_.run==124230) {
    if (evt_.ls>=25 && evt_.ls<=68) evt_.good=1;
  }

  evt_.wbc=wbc[iEvent.id().run()];
  evt_.delay=globaldelay[iEvent.id().run()];
  evt_.bx=iEvent.bunchCrossing();
  evt_.orb=iEvent.orbitNumber();
  
  evt_.trig=NOVAL_I;
  edm::Handle<edm::TriggerResults> triggerResults;
#if CMSSW_VER >= 76
  iEvent.getByToken(triggerResultsToken_, triggerResults);
#else
  iEvent.getByLabel(triggerTag_, triggerResults);
#endif
  if (triggerResults.isValid()) {
    evt_.trig=0;
    const edm::TriggerNames& triggerNames = iEvent.triggerNames(*triggerResults);
    for (size_t itrig=0; itrig<triggerNames.size(); itrig++) {
      std::string sname=triggerNames.triggerNames()[itrig];
      for (size_t k=0; k<triggerNames_.size(); k++) {
	//if (sname.find(triggerNames_[k])==std::string::npos) continue;
	// if (sname!=triggerNames_[k]) continue;
	if (sname.find(triggerNames_[k])) continue;
	if (triggerResults->accept(itrig)==0) continue;
	evt_.trig|=(1<<k);
      }
    }
  }

  if (JKDEBUG) {
    std::cout<<"\n\nProcess Event: Run "<<evt_.run<<" Event "<<evt_.evt<< " LS "<<evt_.ls
	     <<std::endl;
    w.Stop();
    std::cout<<"DONE: Reading event info\n";
    w.Print();
  }


  //
  // Read track info
  //
  if (JKDEBUG) w.Start();
  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(trajTrackCollToken_, trajTrackCollectionHandle);
#else
  std::string trajTrackCollectionInput= iConfig_.getParameter<std::string>("trajectoryInput");
  iEvent.getByLabel(trajTrackCollectionInput, trajTrackCollectionHandle);
#endif

  if (trajTrackCollectionHandle.isValid()||1) {
    if (JKDEBUG) std::cout << "\n\nRun " << evt_.run << " Event " << evt_.evt;
    if (JKDEBUG) std::cout << " Number of tracks =" <<trajTrackCollectionHandle->size()<<std::endl;

    evt_.ntracks=trajTrackCollectionHandle->size();

    //if (trajTrackCollectionInput=="ctfRefitter") {
      //
      // Consider events with exactly one track
      //
      //if (trajTrackCollectionHandle->size()!=1) return;
    //}
  }

  evt_.ntrackFPix[0]=evt_.ntrackFPix[1]=0;
  evt_.ntrackBPix[0]=evt_.ntrackBPix[1]=evt_.ntrackBPix[2]=0;
  evt_.ntrackFPixvalid[0]=evt_.ntrackFPixvalid[1]=0;
  evt_.ntrackBPixvalid[0]=evt_.ntrackBPixvalid[1]=evt_.ntrackBPixvalid[2]=0;

  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Reading track info\n";
    w.Print();
  }

  // Read FED error info

  std::map<uint32_t, int> federrors;
  int federr[16];
  for (int i=0; i<16; i++) federr[i]=0;
  assert(evt_.federrs_size==0);

  if (JKDEBUG) w.Start();
  edm::Handle<edm::DetSetVector<SiPixelRawDataError> >  siPixelRawDataErrorCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(rawDataErrorToken_, siPixelRawDataErrorCollectionHandle);
#else
  iEvent.getByLabel("siPixelDigis", siPixelRawDataErrorCollectionHandle);
#endif
  
  if (siPixelRawDataErrorCollectionHandle.isValid()) {
    const edm::DetSetVector<SiPixelRawDataError>& siPixelRawDataErrorCollection = *siPixelRawDataErrorCollectionHandle;
    edm::DetSetVector<SiPixelRawDataError>::const_iterator itPixelErrorSet = siPixelRawDataErrorCollection.begin();
    //std::map<int, int> evt_federrs;

    for (; itPixelErrorSet!=siPixelRawDataErrorCollection.end(); itPixelErrorSet++) {
      edm::DetSet<SiPixelRawDataError>::const_iterator itPixelError=itPixelErrorSet->begin();
      
      for(; itPixelError!=itPixelErrorSet->end(); ++itPixelError) {
	if (JKDEBUG) { 
	  std::cout << "FED ID: " << itPixelError->getFedId() << std::endl;
	  std::cout << "Word32: " << itPixelError->getWord32() << std::endl;
	  std::cout << "Word64: " << itPixelError->getWord64() << std::endl;
	  std::cout << "Type: " << itPixelError->getType() << std::endl;
	  std::cout << "Error message: " << itPixelError->getMessage() << std::endl;
	}
	if (itPixelErrorSet->detId()!=0xffffffff) {
	  DetId detId(itPixelErrorSet->detId());
	  int type = itPixelError->getType();
	  federrors.insert(std::pair<uint32_t,int>(detId.rawId(), type));
	  if (type>24&&type<=40) federr[type-25]++;
	  else std::cout<<"ERROR: Found new FED error with not recognised Error type: "<<type<<std::endl;
	}
      }
    }
    for (int i=0; i<16; i++) {
      if (federr[i]!=0) {
	evt_.federrs[evt_.federrs_size][0]=federr[i];
	evt_.federrs[evt_.federrs_size][1]=i+25;
	evt_.federrs_size++;
      }
    }
  }

  // Run this if siPixelRawDataErrorCollection is not available (eg in RECO)
  if (!siPixelRawDataErrorCollectionHandle.isValid()) {
    
    // Tracking Error list
    edm::Handle<edm::EDCollection<DetId> > TrackingErrorDetIdCollectionHandle;
#if CMSSW_VER >= 76
    iEvent.getByToken(trackingErrorToken_, TrackingErrorDetIdCollectionHandle);
#else
    iEvent.getByLabel("siPixelDigis", TrackingErrorDetIdCollectionHandle);
#endif
    
    if (TrackingErrorDetIdCollectionHandle.isValid()) {
      const edm::EDCollection<DetId>& TrackingErrorDetIdCollection = *TrackingErrorDetIdCollectionHandle;
      for (size_t i=0; i<TrackingErrorDetIdCollection.size(); i++) {
	federrors.insert(std::pair<uint32_t,int>(TrackingErrorDetIdCollection[i].rawId(), 29));
	federr[4]++;
      }
    }
    
    // User Error List (Overflow only by default)
    edm::Handle<edm::EDCollection<DetId> > UserErrorDetIdCollectionHandle;
#if CMSSW_VER >= 76
    iEvent.getByToken(userErrorToken_, UserErrorDetIdCollectionHandle);
#else
    iEvent.getByLabel("siPixelDigis", "UserErrorModules", UserErrorDetIdCollectionHandle);
#endif
    
    if (UserErrorDetIdCollectionHandle.isValid()) {
      const edm::EDCollection<DetId>& UserErrorDetIdCollection = *UserErrorDetIdCollectionHandle;
      for (size_t i=0; i<UserErrorDetIdCollection.size(); i++) {
	federrors.insert(std::pair<uint32_t,int>(UserErrorDetIdCollection[i].rawId(), 40));
	federr[15]++;
      }
    }
    for (int i=0; i<16; i++) {
      if (federr[i]!=0) {
	evt_.federrs[evt_.federrs_size][0]=federr[i];
	evt_.federrs[evt_.federrs_size][1]=i+25;
	evt_.federrs_size++;
      }
    }
  }

  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Reading FEDError info\n";
    w.Print();
  }

  //=========================================================================================
  //                      Resolution code from PixelTriplets (13 June 2014)
  //=========================================================================================
#if CMSSW_VER >= 71
  
  // Fitter
  edm::ESHandle<TrajectoryFitter> aFitter;
  iSetup.get<TrajectoryFitter::Record>().get("KFFittingSmootherWithOutliersRejectionAndRK",aFitter);
  std::unique_ptr<TrajectoryFitter> theFitter = aFitter->clone();
  
  //----------------------------------------------------------------------------
  // Transient Rechit Builders
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB );

  // Transient rec hits:
  edm::ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  iSetup.get<TransientRecHitRecord>().get( "WithTrackAngle", hitBuilder );

  // Cloner, New from 71Xpre7
  const TkTransientTrackingRecHitBuilder * builder =
    static_cast<TkTransientTrackingRecHitBuilder const *>(hitBuilder.product());
  auto hitCloner = builder->cloner();
  theFitter->setHitCloner(&hitCloner);

#else
  // old
  edm::ESHandle<TrajectoryFitter> TF;
  iSetup.get<TrajectoryFitter::Record>().get( "KFFittingSmootherWithOutliersRejectionAndRK", TF );
  const TrajectoryFitter* theFitter = TF.product();
  
  edm::ESHandle<TransientTrackBuilder> theB;
  iSetup.get<TransientTrackRecord>().get( "TransientTrackBuilder", theB );
  
  // transient rec hits:
  edm::ESHandle<TransientTrackingRecHitBuilder> hitBuilder;
  iSetup.get<TransientRecHitRecord>().get( "WithTrackAngle", hitBuilder );
  
#endif
  
  // TrackPropagator:
  edm::ESHandle<Propagator> prop;
  iSetup.get<TrackingComponentsRecord>().get( "PropagatorWithMaterial", prop );
  const Propagator* thePropagator = prop.product();
  
  Surface::GlobalPoint origin = Surface::GlobalPoint(0,0,0);
  
  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - >


  //
  // Fill event with muon time
  //
  if (JKDEBUG) w.Start();

  edm::Handle<reco::MuonCollection> muonCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(muonCollectionToken_, muonCollectionHandle);
#else
  iEvent.getByLabel("muonsWitht0Correction", muonCollectionHandle);
#endif

  if (muonCollectionHandle.isValid() && muonCollectionHandle->size()>=1) {
    const MuonCollection& muons=*muonCollectionHandle;
    for (size_t i=0; i<muons.size(); i++) {
      if (!muons[i].isGlobalMuon()) continue;
      
      MuonTime time=muons[i].time();
      if (muons[i].isTimeValid() && fabs(evt_.tmuon_err)>time.timeAtIpInOutErr) {
	evt_.tmuon=time.timeAtIpInOut;
	evt_.tmuon_err=time.timeAtIpInOutErr;
      }
      
      if (!muons[i].isEnergyValid()) continue;
      if (muons[i].calEnergy().ecal_id.subdetId()==EcalBarrel) {
	float tecal_err=33./(muons[i].calEnergy().emMax/0.3);
	if (fabs(evt_.tecal_err)>tecal_err) {
	  evt_.tecal_err=tecal_err;
	  EBDetId hitId(muons[i].calEnergy().ecal_id);
	  float theta=
	    atan(exp((hitId.ieta()-0.5*(hitId.ieta()/fabs(hitId.ieta())))*-1.479/85.))*2;
	  evt_.tecal_raw=muons[i].calEnergy().ecal_time;
	  evt_.tecal=evt_.tecal_raw*-4.77/sin(theta);
	}
      } // ecal time only in the barrel
    } // loop along muons
  } // if there is a muon
  
  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Reading muon info\n";
    w.Print();
  }



  //
  // Magnetic field
  //

  if (JKDEBUG) w.Start();
  edm::ESHandle<MagneticField> magFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get(magFieldHandle);
  const MagneticField& magField = *magFieldHandle;

  float mag_x = magField.inTesla(GlobalPoint(0,0,0)).x();
  float mag_y = magField.inTesla(GlobalPoint(0,0,0)).y();
  float mag_z = magField.inTesla(GlobalPoint(0,0,0)).z();

  evt_.field=sqrt(mag_x*mag_x+mag_y*mag_y+mag_z*mag_z);


  //
  // Vertex
  //
  edm::Handle<reco::VertexCollection> vertexCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(primaryVerticesToken_, vertexCollectionHandle);
#else
  iEvent.getByLabel("offlinePrimaryVertices", vertexCollectionHandle);
#endif

  evt_.nvtx=0;
  const reco::VertexCollection & vertices = *vertexCollectionHandle.product();
  reco::VertexCollection::const_iterator bestVtx=vertices.end();
  for(reco::VertexCollection::const_iterator it=vertices.begin() ; it!=vertices.end() ; ++it) {
    if (!it->isValid()) continue;
    if (evt_.vtxntrk==NOVAL_I || 
	evt_.vtxntrk<int(it->tracksSize()) || 
	(evt_.vtxntrk==int(it->tracksSize()) && fabs(evt_.vtxZ)>fabs(it->z()))) {
      evt_.vtxntrk=it->tracksSize();
      evt_.vtxD0=it->position().rho();
      evt_.vtxX=it->x();
      evt_.vtxY=it->y();
      evt_.vtxZ=it->z();
      evt_.vtxndof=it->ndof();
      evt_.vtxchi2=it->chi2();
      bestVtx=it;
    }
    if (fabs(it->z())<=25. && fabs(it->position().rho())<=2. && it->ndof()>4) evt_.nvtx++;
  }
  
  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Reading magnetic field and vertex info\n";
    w.Print();
  }

  //
  // Read digi information
  //

  #ifdef COMPLETE

  if (JKDEBUG) w.Start();

  edm::Handle<edm::DetSetVector<PixelDigi> >  digiCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(pixelDigiCollectionToken_, digiCollectionHandle);
#else
  iEvent.getByLabel("siPixelDigis", digiCollectionHandle);
#endif

  if (digiCollectionHandle.isValid()) {
    const edm::DetSetVector<PixelDigi>& digiCollection = *digiCollectionHandle;
    edm::DetSetVector<PixelDigi>::const_iterator itDigiSet = digiCollection.begin();

    for (; itDigiSet!=digiCollection.end(); itDigiSet++) {
      
      DetId detId(itDigiSet->detId());
      unsigned int subDetId=detId.subdetId();
      
      // Take only pixel digis
      if (subDetId!=PixelSubdetector::PixelBarrel &&
	  subDetId!=PixelSubdetector::PixelEndcap) {
	if (JKDEBUG) std::cout << "ERROR: not a pixel digi!!!" << std::endl; // should not happen
	continue;
      }
      
      ModuleData module=getModuleData(detId.rawId(), federrors);
      
      if (JKDEBUG) {
	std::cout << "Run " << evt_.run << " Event " << evt_.evt << " number of digis on ";
	if (module.det==0) {
	  std::cout << " PixelBarrel layer " << module.layer << " ladder " << module.ladder;
	  std::cout << " module " << module.module << " ";
	} else {
	  std::cout << " PixelForward disk " << module.disk << " blade " << module.blade;
	  std::cout << " panel " << module.panel << " module " << module.module << " ";
	}
      }
      ModuleData module_on=getModuleData(detId.rawId(), federrors, "online");
      if (JKDEBUG) std::cout << ": " << itDigiSet->size() << std::endl;
      
      edm::DetSet<PixelDigi>::const_iterator itDigi=itDigiSet->begin();
      
      for(; itDigi!=itDigiSet->end(); ++itDigi) {
	Digi digi;
	digi.i=itDigi-itDigiSet->begin();
	digi.row=itDigi->row();
	digi.col=itDigi->column();
	digi.adc=itDigi->adc();
	digi.mod=module;
	digi.mod_on=module_on;
	digis_.push_back(digi);
	if (JKDEBUG) {
	  std::cout<<"\t#"<<digi.i<<" adc "<<digi.adc<<" at ("<<digi.col<<", "<<digi.row<<")";
	  std::cout<<std::endl;
	}
      }
    } // loop on digis
  }

  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Reading the Digis\n";
    w.Print();
  }

  #endif


  //
  // Read cluster information
  //

  if (JKDEBUG) w.Start();

  // For counting number of cluster/pixels
  for (size_t i=0; i<4; i++) evt_.nclu[i]=evt_.npix[i]=0;
  std::map<unsigned int, int> nclu_mod;
  std::map<unsigned int, int> npix_mod;
  std::map<unsigned long int, int> nclu_roc;
  std::map<unsigned long int, int> npix_roc;

  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > clusterCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(clustersToken_, clusterCollectionHandle);
#else
  iEvent.getByLabel("siPixelClusters", clusterCollectionHandle);
#endif

  if (clusterCollectionHandle.isValid()) {
    const edmNew::DetSetVector<SiPixelCluster>& clusterCollection=*clusterCollectionHandle;
    edmNew::DetSetVector<SiPixelCluster>::const_iterator itClusterSet= clusterCollection.begin();

    for (; clusterCollectionHandle.isValid() && itClusterSet!=clusterCollection.end(); 
	 itClusterSet++) {
      
      DetId detId(itClusterSet->id());
      unsigned int subDetId=detId.subdetId();
      
      // Take only pixel clusters
      if (subDetId!=PixelSubdetector::PixelBarrel &&
	  subDetId!=PixelSubdetector::PixelEndcap) {
	std::cout << "ERROR: not a pixel cluster!!!" << std::endl; // should not happen
	continue;
      }
      
      ModuleData module=getModuleData(detId.rawId(), federrors);
      
      if (JKDEBUG) {
	std::cout << "Run " << evt_.run << " Event " << evt_.evt << " number of clusters on ";
	if (module.det==0) {
	  std::cout << " PixelBarrel layer " << module.layer << " ladder " << module.ladder;
	  std::cout << " module " << module.module << " ";
	} else {
	  std::cout << " PixelForward disk " << module.disk << " blade " << module.blade;
	  std::cout << " panel " << module.panel << " module " << module.module << " ";
	}
      }
      ModuleData module_on=getModuleData(detId.rawId(), federrors, "online");
      if (JKDEBUG) std::cout << ": " << itClusterSet->size() << std::endl;

      edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=itClusterSet->begin();
      
      for(; itCluster!=itClusterSet->end(); ++itCluster) {
	Cluster clust;
	clust.i=itCluster-itClusterSet->begin();
	clust.charge=itCluster->charge()/1000.0;
	clust.size=itCluster->size();
	clust.x=itCluster->x();
	clust.y=itCluster->y();
	clust.sizeX=itCluster->sizeX();
	clust.sizeY=itCluster->sizeY();
	for (int i=0; i<itCluster->size() && i<1000; i++) {
	  clust.adc[i]=float(itCluster->pixelADC()[i])/1000.0;
	  clust.pix[i][0]=((itCluster->pixels())[i]).x;
	  clust.pix[i][1]=((itCluster->pixels())[i]).y;
	}
	// Layer Occupancy
	if (module_on.det!=NOVAL_I) {
	  evt_.nclu[(1-module_on.det)*module_on.layer]++;
	  evt_.npix[(1-module_on.det)*module_on.layer]+=itCluster->size();
	}
	// Module Occupancy
	if (!nclu_mod.count(module_on.rawid)) nclu_mod[module_on.rawid] = 0;
	if (!npix_mod.count(module_on.rawid)) npix_mod[module_on.rawid] = 0;
	nclu_mod[module_on.rawid]++;
	npix_mod[module_on.rawid]+=itCluster->size();
	// Roc Occupancy
	int RocID = get_RocID_from_cluster_coords(clust.x, clust.y, module_on);
	unsigned long int modroc = module_on.rawid * 100 + RocID;
	if (!nclu_roc.count(modroc)) nclu_roc[modroc] = 0;
	if (!npix_roc.count(modroc)) npix_roc[modroc] = 0;
	nclu_roc[modroc]++;
	npix_roc[modroc]+=itCluster->size();
	
	clust.mod=module;
	clust.mod_on=module_on;
	clusts_.push_back(clust);
	if (JKDEBUG) {
	  std::cout<<"\t#"<<clust.i<<" charge "<<clust.charge<<" size "<<clust.size<<std::endl;
	}
      }
    } // loop on cluster sets
  }
  if (JKDEBUG) {
    std::cout<<"Nclusters on L1: "<<evt_.nclu[1]<<" L2: "<<evt_.nclu[2]
	     <<" L3: "<<evt_.nclu[3]<<" FPix: "<<evt_.nclu[0]
	     <<"\tNpixels on L1: "<<evt_.npix[1]<<" L2: "<<evt_.npix[2]
	     <<" L3: "<<evt_.npix[3]<<" FPix: "<<evt_.npix[0]<<std::endl;
  }
  
  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Reading the Clusters\n";
    w.Print();
  }
  
  //
  // Process tracks
  //
  if (JKDEBUG) std::cout<<"Processing tracks...\n";
  
  if (trajTrackCollectionHandle.isValid()||1) { // check track collection

    TrajTrackAssociationCollection::const_iterator itTrajTrack=trajTrackCollectionHandle->begin();

//     edm::Handle<reco::BeamSpot> beamSpotHandle;
//     iEvent.getByLabel("offlineBeamSpot", beamSpotHandle);
//     assert(beamSpotHandle.isValid());

    int itrack=0;
    for (;itTrajTrack!=trajTrackCollectionHandle->end(); itTrajTrack++) {  // loop on tracks

      if (JKDEBUG) std::cout<<"\nProcessing track"<<itrack<<"...\n";      
      if (JKDEBUG) w.Start();
      TStopwatch w2;
      if (DEBUG) w2.Start();

      const Trajectory& traj  = *itTrajTrack->key;
      const Track&      track = *itTrajTrack->val;
      

      //=========================================================================================
      //                      Resolution code from PixelTriplets (13 June 2014)
      //=========================================================================================
      
      // transient track (for resolution)
      TransientTrack tTrack = theB->build(track);
      TrajectoryStateOnSurface initialTSOS = tTrack.innermostMeasurementState();
      TrajectoryStateClosestToPoint tscp = tTrack.trajectoryStateClosestToPoint( origin );
      double kap = tTrack.initialFreeState().transverseCurvature();
      if( tscp.isValid() ) {
	kap = tscp.perigeeParameters().transverseCurvature();
      }
      double rho = 1/kap;
      double rinv = -kap;
      
      // precise hit in CMS global coordinates
      double xPXB1 = 0;
      double yPXB1 = 0;
      double zPXB1 = 0;
      double xPXB2 = 0;
      double yPXB2 = 0;
      double zPXB2 = 0;
      double xPXB3 = 0;
      double yPXB3 = 0;
      double zPXB3 = 0;
      int n1 = 0;
      int n2 = 0;
      int n3 = 0;    

      uint32_t innerDetId = 0;
      edm::OwnVector<TrackingRecHit> recHitVector; // for seed
      Trajectory::RecHitContainer coTTRHvec; // for fit, constant

      // Loop on recHits in order to fill above hit coordinates
      for( trackingRecHit_iterator irecHit = track.recHitsBegin();
	   irecHit != track.recHitsEnd(); ++irecHit ) {
	
	DetId detId = (*irecHit)->geographicalId();
	
	// enum Detector { Tracker=1, Muon=2, Ecal=3, Hcal=4, Calo=5 };
	if( detId.det() != 1 ) continue;
	
	recHitVector.push_back( (*irecHit)->clone() );
	
	// build transient hit:
#if CMSSW_VER >= 71
	// for pre7
	auto tmprh = (*irecHit)->cloneForFit(*builder->geometry()->idToDet((**irecHit).geographicalId()));
	auto transRecHit = hitCloner.makeShared(tmprh, initialTSOS);
#else
	TransientTrackingRecHit::RecHitPointer transRecHit = hitBuilder->build( &*(*irecHit), initialTSOS);
#endif
	coTTRHvec.push_back( transRecHit );
	
	if( ! (*irecHit)->isValid() ) continue;
	
	double gX = transRecHit->globalPosition().x();
	double gY = transRecHit->globalPosition().y();
	double gZ = transRecHit->globalPosition().z();
	
	if( transRecHit->canImproveWithTrack() ) {//use z from track to apply alignment
	    
	  TrajectoryStateOnSurface propTSOS = thePropagator->propagate( initialTSOS, transRecHit->det()->surface() );
	  if( propTSOS.isValid() ){
#if CMSSW_VER >= 71
	    auto preciseHit = hitCloner.makeShared(tmprh,propTSOS); //pre7
#else
	    TransientTrackingRecHit::RecHitPointer preciseHit = transRecHit->clone(propTSOS);
#endif
	    gX = preciseHit->globalPosition().x();
	    gY = preciseHit->globalPosition().y();
	    gZ = preciseHit->globalPosition().z();
	  }
	}
	
	if( sqrt( gX*gX + gY*gY ) < 99.9 ) innerDetId = detId.rawId();

	int ilay = PXBDetId(detId).layer();
	if ( ilay == 1 ) {
	  n1++;
	  xPXB1 = gX;
	  yPXB1 = gY;
	  zPXB1 = gZ;
	} else if ( ilay == 2 ) {
	  n2++;
	  xPXB2 = gX;
	  yPXB2 = gY;
	  zPXB2 = gZ;
	} else if ( ilay == 3 ){
	  n3++;
	  xPXB3 = gX;
	  yPXB3 = gY;
	  zPXB3 = gZ;
	}
      }
      
      // trajectory residuals:
      PTrajectoryStateOnDet PTraj = trajectoryStateTransform::persistentState( initialTSOS, innerDetId );
      const TrajectorySeed seed( PTraj, recHitVector, alongMomentum );
      std::vector<Trajectory> refitTrajectoryCollection = theFitter->fit( seed, coTTRHvec, initialTSOS );
      if( refitTrajectoryCollection.size() > 0 ) { // should be either 0 or 1
	
	const Trajectory& refitTrajectory = refitTrajectoryCollection.front();
	
	// Trajectory.measurements:
	Trajectory::DataContainer refitTMs = refitTrajectory.measurements();
	
	for( Trajectory::DataContainer::iterator iTM = refitTMs.begin();
	     iTM != refitTMs.end(); iTM++ ) {
	  
	  if( ! iTM->recHit()->isValid() ) continue;
	  
	  TrajectoryStateOnSurface combinedPredictedState =
	    TrajectoryStateCombiner().combine( iTM->forwardPredictedState(), iTM->backwardPredictedState() );
	  
	  if( ! combinedPredictedState.isValid() ) continue;//skip hit
	  
	  // use Topology. no effect in PXB, essential in TID, TEC
	  const Topology* theTopology = &(iTM->recHit()->detUnit()->topology() );
	  
	  // TODO: Use the measurementPosition(point, trackdir) version of this function in order to take bows into account!
	  MeasurementPoint hitMeasurement = theTopology->measurementPosition( iTM->recHit()->localPosition() );
	  
	  // TID and TEC have trapezoidal detectors:
	  // translation from channel number into local x depends on local y
	  // track prediction has local x,y => can convert into proper channel number MeasurementPoint:
	  // TODO: Use the measurementPosition(point, trackdir) version of this function in order to take bows into account!
	  MeasurementPoint combinedPredictedMeasurement = theTopology->measurementPosition( combinedPredictedState.localPosition() );
	  
	  // Overwrite global coordinates (why?)
	  DetId detId = iTM->recHit()->geographicalId();
	  uint32_t subDet = detId.subdetId();
	  MeasurementPoint mp( hitMeasurement.x(), (subDet<3) ? hitMeasurement.y() : combinedPredictedMeasurement.y() );      
	  Surface::LocalPoint lp = theTopology->localPosition( mp );
	  const GeomDet * myGeomDet = iTM->recHit()->det(); // makes no difference in TEC
	  Surface::GlobalPoint gp = myGeomDet->toGlobal( lp );
	  
	  double gX = gp.x();
	  double gY = gp.y();
	  double gZ = gp.z();
	  
	  //2012: overwrite PXB global coordinates once more, using topology:
	  if( subDet == PixelSubdetector::PixelBarrel ) {
	    int ilay = PXBDetId(detId).layer();
	    if( ilay == 1 ) {
	      xPXB1 = gX;
	      yPXB1 = gY;
	      zPXB1 = gZ;
	    } else if( ilay == 2 ) {
	      xPXB2 = gX;
	      yPXB2 = gY;
	      zPXB2 = gZ;
	    } else if( ilay == 3 ) {
	      xPXB3 = gX;
	      yPXB3 = gY;
	      zPXB3 = gZ;
	    }
	  }//PXB
	}
      }
      
      float lev_l1 = NOVAL_F;
      float lev_l2 = NOVAL_F;
      float lev_l3 = NOVAL_F;
      float res_dx_l1 = NOVAL_F;
      float res_dz_l1 = NOVAL_F;
      float res_dx_l2 = NOVAL_F;
      float res_dz_l2 = NOVAL_F;
      float res_dx_l3 = NOVAL_F;
      float res_dz_l3 = NOVAL_F;
      
      if( n1*n2*n3 > 0 ) { // Only look at triplets
	// Lever arms
	// Layer 1
	double ax = xPXB3 - xPXB2;
	double ay = yPXB3 - yPXB2;
	double aa = sqrt( ax*ax + ay*ay ); // from 2 to 3
	
	double xmid = 0.5 * ( xPXB2 + xPXB3 );
	double ymid = 0.5 * ( yPXB2 + yPXB3 );
	double bx = xPXB1 - xmid;
	double by = yPXB1 - ymid;
	double bb = sqrt( bx*bx + by*by ); // from mid point to point 1
	
	lev_l1 = bb/aa;
	
	// Layer 2
	ax = xPXB3 - xPXB1;
	ay = yPXB3 - yPXB1;
	aa = sqrt( ax*ax + ay*ay ); // from 1 to 3

	xmid = 0.5 * ( xPXB1 + xPXB3 );
	ymid = 0.5 * ( yPXB1 + yPXB3 );
	bx = xPXB2 - xmid;
	by = yPXB2 - ymid;
	bb = sqrt( bx*bx + by*by ); // from mid point to point 2
	
	lev_l2 = bb/aa;
	
	// Layer 3
	ax = xPXB2 - xPXB1;
	ay = yPXB2 - yPXB1;
	aa = sqrt( ax*ax + ay*ay ); // from 1 to 2
	
	xmid = 0.5 * ( xPXB1 + xPXB2 );
	ymid = 0.5 * ( yPXB1 + yPXB2 );
	bx = xPXB3 - xmid;
	by = yPXB3 - ymid;
	bb = sqrt( bx*bx + by*by ); // from mid point to point 3
	
	lev_l3 = bb/aa;
	
	// Calculate the centre of the helix in xy-projection that
	// transverses the two spacepoints. The points with the same
	// distance from the two points are lying on a line.
	// LAMBDA is the distance between the point in the middle of
	// the two spacepoints and the centre of the helix.
	
	// Layer 1
	double lam = sqrt( -0.25 + rho*rho / ( ( xPXB2 - xPXB3 )*( xPXB2 - xPXB3 ) + ( yPXB2 - yPXB3 )*( yPXB2 - yPXB3 ) ) );
	
	// There are two solutions, the sign of kap gives the information
	// which of them is correct:
	
	if( kap > 0 ) lam = -lam;
	
	// ( X0, Y0 ) is the centre of the circle
	// that describes the helix in xy-projection:
	
	double x0 =  0.5*( xPXB2 + xPXB3 ) + lam * ( -yPXB2 + yPXB3 );
	double y0 =  0.5*( yPXB2 + yPXB3 ) + lam * (  xPXB2 - xPXB3 );
	
	// Calculate theta:
	
	double num = ( yPXB3 - y0 ) * ( xPXB2 - x0 ) - ( xPXB3 - x0 ) * ( yPXB2 - y0 );
	double den = ( xPXB2 - x0 ) * ( xPXB3 - x0 ) + ( yPXB2 - y0 ) * ( yPXB3 - y0 );
	double tandip = kap * ( zPXB3 - zPXB2 ) / atan( num / den );
	
	// To get phi0 in the right interval one must distinguish
	// two cases with positve and negative kap:
	
	double uphi;
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );
	
	// The distance of the closest approach DCA depends on the sign
	// of kappa:
	
	double udca;
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );
	
	// angle from first hit to dca point:
	
	double dphi = atan( ( ( xPXB2 - x0 ) * y0 - ( yPXB2 - y0 ) * x0 )
			    / ( ( xPXB2 - x0 ) * x0 + ( yPXB2 - y0 ) * y0 ) );
	
	double uz0 = zPXB2 + tandip * dphi * rho;
	
	// extrapolate to inner hit:
	// cirmov
	// we already have rinv = -kap
	
	double cosphi = cos(uphi);
	double sinphi = sin(uphi);
	double dp = -xPXB1*sinphi + yPXB1*cosphi + udca;
	double dl = -xPXB1*cosphi - yPXB1*sinphi;
	double sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca1 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 1
	double ud = 1 + rinv*udca;
	double phi1 = atan2( -rinv*xPXB1 + ud*sinphi, rinv*yPXB1 + ud*cosphi );//direction
	
	// arc length:
	
	double xx = xPXB1 + dca1 * sin(phi1); // point on track
	double yy = yPXB1 - dca1 * cos(phi1);
	
	double f0 = uphi;//
	double kx = kap*xx;
	double ky = kap*yy;
	double kd = kap*udca;
	
	// Solve track equation for s:
	
	double dx = kx - (kd-1)*sin(f0);
	double dy = ky + (kd-1)*cos(f0);
	double ks = atan2( dx, -dy ) - f0;// turn angle
	
	const double pi = 4*atan(1); // Geom::pi();
        const double twopi = 2*pi;
	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;
	
	double s = ks*rho;// signed
	double uz1 = uz0 + s*tandip; //track z at R1
	double dz1 = zPXB1 - uz1;
	
	// Layer 2
	lam = sqrt( -0.25 + rho*rho / ( ( xPXB1 - xPXB3 )*( xPXB1 - xPXB3 ) + ( yPXB1 - yPXB3 )*( yPXB1 - yPXB3 ) ) );
	if( kap > 0 ) lam = -lam;
	
	x0 = 0.5*( xPXB1 + xPXB3 ) + lam * ( -yPXB1 + yPXB3 );
	y0 = 0.5*( yPXB1 + yPXB3 ) + lam * ( xPXB1 - xPXB3 );
	
	num = ( yPXB3 - y0 ) * ( xPXB1 - x0 ) - ( xPXB3 - x0 ) * ( yPXB1 - y0 );
        den = ( xPXB1 - x0 ) * ( xPXB3 - x0 ) + ( yPXB1 - y0 ) * ( yPXB3 - y0 );
        tandip = kap * ( zPXB3 - zPXB1 ) / atan( num / den );
        
        uphi = ( kap > 0 ) ? atan2( -x0, y0 ) : atan2( x0, -y0 );
        udca = ( kap > 0 ) ? rho - sqrt( x0*x0 + y0*y0 ) : rho + sqrt( x0*x0 + y0*y0 );
	dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
		     / ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );
        
        uz0 = zPXB1 + tandip * dphi * rho;
        
        cosphi = cos(uphi);
        sinphi = sin(uphi);
        dp = -xPXB2*sinphi + yPXB2*cosphi + udca;
        dl = -xPXB2*cosphi - yPXB2*sinphi;
        sa = 2*dp + rinv * ( dp*dp + dl*dl );
        double dca2 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 2
        ud = 1 + rinv*udca;
        double phi2 = atan2( -rinv*xPXB2 + ud*sinphi, rinv*yPXB2 + ud*cosphi );//direction
        
        xx = xPXB2 + dca2 * sin(phi2); // point on track
        yy = yPXB2 - dca2 * cos(phi2);
        
        f0 = uphi;//
        kx = kap*xx;
        ky = kap*yy;
        kd = kap*udca;
        
        dx = kx - (kd-1)*sin(f0);
        dy = ky + (kd-1)*cos(f0);
        ks = atan2( dx, -dy ) - f0;// turn angle
        if( ks > pi ) ks = ks - twopi;
        else if( ks < -pi ) ks = ks + twopi;
        
        s = ks*rho; // signed
        double uz2 = uz0 + s*tandip; // track z at R2
        double dz2 = zPXB2 - uz2;
        
	// Layer 3
	lam = sqrt( -0.25 + rho*rho / ( ( xPXB1 - xPXB2 )*( xPXB1 - xPXB2 ) + ( yPXB1 - yPXB2 )*( yPXB1 - yPXB2 ) ) );
	if( kap > 0 ) lam = -lam;
	
	x0 =  0.5*( xPXB1 + xPXB2 ) + lam * ( -yPXB1 + yPXB2 );
	y0 =  0.5*( yPXB1 + yPXB2 ) + lam * (  xPXB1 - xPXB2 );
	
	num = ( yPXB2 - y0 ) * ( xPXB1 - x0 ) - ( xPXB2 - x0 ) * ( yPXB1 - y0 );
	den = ( xPXB1 - x0 ) * ( xPXB2 - x0 ) + ( yPXB1 - y0 ) * ( yPXB2 - y0 );
	tandip = kap * ( zPXB2 - zPXB1 ) / atan( num / den );
	
	if( kap > 0 ) uphi = atan2( -x0,  y0 );
	else          uphi = atan2(  x0, -y0 );
	
	if( kap > 0 ) udca = rho - sqrt( x0*x0 + y0*y0 );
	else          udca = rho + sqrt( x0*x0 + y0*y0 );
	
	dphi = atan( ( ( xPXB1 - x0 ) * y0 - ( yPXB1 - y0 ) * x0 )
			  / ( ( xPXB1 - x0 ) * x0 + ( yPXB1 - y0 ) * y0 ) );
	uz0 = zPXB1 + tandip * dphi * rho;
	
	cosphi = cos(uphi);
	sinphi = sin(uphi);
	dp = -xPXB3*sinphi + yPXB3*cosphi + udca;
	dl = -xPXB3*cosphi - yPXB3*sinphi;
	sa = 2*dp + rinv * ( dp*dp + dl*dl );
	double dca3 = sa / ( 1 + sqrt(1 + rinv*sa) );// distance to hit 3
	ud = 1 + rinv*udca;
	double phi3 = atan2( -rinv*xPXB3 + ud*sinphi, rinv*yPXB3 + ud*cosphi );//track direction
	
	xx = xPXB3 + dca3 * sin(phi3); // point on track
	yy = yPXB3 - dca3 * cos(phi3);
	
	f0 = uphi;
	kx = kap*xx;
	ky = kap*yy;
	kd = kap*udca;
	
	dx = kx - (kd-1)*sin(f0);
	dy = ky + (kd-1)*cos(f0);
	ks = atan2( dx, -dy ) - f0;// turn angle
	if(      ks >  pi ) ks = ks - twopi;
	else if( ks < -pi ) ks = ks + twopi;
	
	s = ks*rho;// signed
	double uz3 = uz0 + s*tandip; //track z at R3
	double dz3 = zPXB3 - uz3;
	
        res_dx_l1 = dca1*1E4;
        res_dz_l1 = dz1*1E4;
        res_dx_l2 = dca2*1E4;
        res_dz_l2 = dz2*1E4;
        res_dx_l3 = dca3*1E4;
        res_dz_l3 = dz3*1E4;
	
	//std::cout<<"Residual (layer 1) dx: "<<res_dx_l1<<" um  dz: "<<res_dz_l1<<" um"<<std::endl;
        //std::cout<<"         (layer 2) dx: "<<res_dx_l2<<" um  dz: "<<res_dz_l2<<" um"<<std::endl;
        //std::cout<<"         (layer 3) dx: "<<res_dx_l3<<" um  dz: "<<res_dz_l3<<" um"<<std::endl;
      }
      // End of Resolution stuff
      //------------------------------------------------------------------------------------

      // New on 02.25.2016 (JK)
      // Find closest vertex to the track
      reco::VertexCollection::const_iterator closestVtx=vertices.end();
      double min_distance = 9999;
      for(reco::VertexCollection::const_iterator it=vertices.begin() ; it!=vertices.end() ; ++it) {
	if (!it->isValid()) continue;
	double trk_vtx_d0 = track.dxy(it->position())*-1.0;
	double trk_vtx_dz = track.dz(it->position());
	double trk_vtx_dB = sqrt(trk_vtx_d0*trk_vtx_d0 + trk_vtx_dz*trk_vtx_dz);
	if (trk_vtx_dB<min_distance) {
	  min_distance = trk_vtx_dB;
	  closestVtx=it;
	}
      }
      // End of JK

      TrackData track_;
      std::vector<TrajMeasurement> trajmeas;
      //
      // Read track info - USED BY TrajMeasurement!!!
      //
      track_.i=itrack++;
      track_.pix=0;
      track_.strip=0;
      track_.pixhit[0]=0;
      track_.pixhit[1]=0;
      track_.validpixhit[0]=0;
      track_.validpixhit[1]=0;
      track_.ndof=track.ndof();
      track_.chi2=track.chi2();
      if (useClosestVtx_) {
	track_.d0=track.dxy(closestVtx->position())*-1.0;
	track_.dz=track.dz(closestVtx->position());
      } else {
	track_.d0=track.dxy(bestVtx->position())*-1.0;
	track_.dz=track.dz(bestVtx->position());
      }
      track_.pt=track.pt();
      track_.p=track.p();
      track_.eta=track.eta();
      track_.theta=track.theta();
      track_.phi=track.phi();
      track_.fpix[0]=track_.fpix[1]=0;
      track_.bpix[0]=track_.bpix[1]=track_.bpix[2]=0;
      track_.validfpix[0]=track_.validfpix[1]=0;
      track_.validbpix[0]=track_.validbpix[1]=track_.validbpix[2]=0;
      track_.fromVtx= ( (fabs(track_.dz) <0.25) && (fabs(track_.d0) <0.2) ) ? 1 : 0;
      track_.highPurity= (track.quality(reco::TrackBase::highPurity)) ? 1 : 0;
      track_.quality=track.qualityMask();
      track_.algo=track.algo();
#if CMSSW_VER > 71
      track_.nstripmissing=track.hitPattern().stripLayersWithoutMeasurement(HitPattern::TRACK_HITS);
      track_.nstriplost=track.hitPattern().numberOfLostStripHits(HitPattern::TRACK_HITS);
#else
      track_.nstripmissing=track.hitPattern().stripLayersWithoutMeasurement();
      track_.nstriplost=track.hitPattern().numberOfLostStripHits();
#endif
      track_.nstriplayer=track.hitPattern().stripLayersWithMeasurement();

      //
      // New on 02.18.2010
      // 

      ESHandle<MeasurementTracker> measurementTrackerHandle;
      iSetup.get<CkfComponentsRecord>().get(measurementTrackerHandle);
      
#if CMSSW_VER >= 76
      Handle<MeasurementTrackerEvent> measurementTrackerEventHandle;
      iEvent.getByToken(measTrackerEvtToken_, measurementTrackerEventHandle);
#elif CMSSW_VER != 53
      Handle<MeasurementTrackerEvent> measurementTrackerEventHandle;
      iEvent.getByLabel("MeasurementTrackerEvent", measurementTrackerEventHandle);
#endif
      
      edm::ESHandle<Chi2MeasurementEstimatorBase> est;
      iSetup.get<TrackingComponentsRecord>().get("Chi2",est);
      
      edm::ESHandle<Propagator> prop;
      iSetup.get<TrackingComponentsRecord>().get("PropagatorWithMaterial",prop);
      Propagator* thePropagator = prop.product()->clone();
      if (extrapolateFrom_>=extrapolateTo_) {
	thePropagator->setPropagationDirection(oppositeToMomentum);
      }

      // CUT OUT HERE

      //
      // Loop along trajectory measurements
      //
    
      std::vector<TrajectoryMeasurement> trajMeasurements = traj.measurements();
      TrajectoryStateCombiner trajStateComb;
    

      if (DEBUG) w2.Stop();
      if (DEBUG) std::cout<<"PARTIALLY: Processing track info\n";
      if (DEBUG) w2.Print();

      if (JKDEBUG) std::cout << "Run " << evt_.run << " Event " << evt_.evt;
      if (JKDEBUG) {
	std::cout << " number of measurements on track "<< trajMeasurements.size() << std::endl;
      }
    
      //int nTopHits=0; int nBottomHits=0; int nTopValidHits=0; int nBottomValidHits=0;
      //int nValidStripHits=0;
      std::vector<TrajectoryMeasurement>::const_iterator itTraj;    
      std::vector<TrajectoryMeasurement> expTrajMeasurements; // for Layer1 only this gets recorded
      int imeas=0;
      int realHitFound=0;

      for(itTraj=trajMeasurements.begin(); itTraj!=trajMeasurements.end(); ++itTraj) {

	TrajMeasurement meas;

	TransientTrackingRecHit::ConstRecHitPointer chkRecHit = itTraj->recHit();
	ModuleData chkmod=getModuleData(chkRecHit->geographicalId().rawId(), federrors, "offline");

	TrajectoryStateOnSurface chkPredTrajState=trajStateComb(itTraj->forwardPredictedState(),
								itTraj->backwardPredictedState());
	float chkx=chkPredTrajState.globalPosition().x();
	float chky=chkPredTrajState.globalPosition().y();
	float chkz=chkPredTrajState.globalPosition().z();
	LocalPoint chklp=chkPredTrajState.localPosition();
	
	// Resolution
	if (chkmod.det==0) {
	  meas.res_dx = (chkmod.layer==1) ? res_dx_l1 : ( (chkmod.layer==2) ? res_dx_l2 : res_dx_l3 );
	  meas.res_dz = (chkmod.layer==1) ? res_dz_l1 : ( (chkmod.layer==2) ? res_dz_l2 : res_dz_l3 );
	  meas.lev = (chkmod.layer==1) ? lev_l1 : ( (chkmod.layer==2) ? lev_l2 : lev_l3 );
	} else {
	  meas.res_dx = NOVAL_F;
	  meas.res_dz = NOVAL_F;
	  meas.lev = NOVAL_F;
	}
	
	if (chkmod.det==0&&chkmod.layer==extrapolateTo_) {
	  if (DEBUG) w2.Start();
	  if (JKDEBUG) std::cout<<"Layer "<<extrapolateTo_<<" hit found";
	  // Here we will drop the extrapolated hits if there is a hit and use that hit
	  size_t imatch=0;
	  float glmatch=9999.;
	  for (size_t iexp=0; iexp<expTrajMeasurements.size(); iexp++) {
	    ModuleData mod=
	      getModuleData(expTrajMeasurements[iexp].recHit()->geographicalId().rawId(),
			    federrors, "offline");
	    if (mod.det!=0 || mod.layer!=extrapolateTo_) continue;
// 	    if (sqrt(meas.dladder*meas.dladder+meas.dmodule*meas.dmodule)<match) {
// 	      match=sqrt(meas.dladder*meas.dladder+meas.dmodule*meas.dmodule);
// 	      imatch=iexp;
// 	    }
	    int dladder=abs(mod.ladder-chkmod.ladder);
	    if (dladder>10) dladder=20-dladder;
	    int dmodule=abs(mod.module-chkmod.module);
	    if (sqrt(dladder*dladder+dmodule*dmodule) <
		sqrt(meas.dladder*meas.dladder+meas.dmodule*meas.dmodule) ) {
	      meas.dladder=dladder;
	      meas.dmodule=dmodule;
	    }
	    if (dladder!=0||dmodule!=0) {
	      if (JKDEBUG) std::cout<<" (module mismatch - skipping it) ";
	      continue;
	    }
	    TrajectoryStateOnSurface predTrajState=expTrajMeasurements[iexp].updatedState();
	    float x=predTrajState.globalPosition().x();
	    float y=predTrajState.globalPosition().y();
	    float z=predTrajState.globalPosition().z();
	    float dxyz=sqrt((chkx-x)*(chkx-x)+(chky-y)*(chky-y)+(chkz-z)*(chkz-z));
	    if (dxyz<glmatch) {
	      glmatch=dxyz;
	      imatch=iexp;
	    }
	  } // found the propagated traj best matching the hit in data
	  
	  if (glmatch<9999.) { // if there is any propagated trajectory for this hit
	    meas.glmatch=glmatch;

	    ModuleData mod=
	      getModuleData(expTrajMeasurements[imatch].recHit()->geographicalId().rawId(),
			    federrors, "offline");
	    meas.dladder=abs(mod.ladder-chkmod.ladder);
	    if (meas.dladder>10) meas.dladder=20-meas.dladder;
	    meas.dmodule=abs(mod.module-chkmod.module);

	    LocalPoint lp=expTrajMeasurements[imatch].updatedState().localPosition();
	    meas.lxmatch=fabs(lp.x()-chklp.x());
	    meas.lymatch=fabs(lp.y()-chklp.y());
	    meas.lxymatch=sqrt(meas.lxmatch*meas.lxmatch+meas.lymatch*meas.lymatch);
	  }
	  
	  if (meas.lxmatch<maxlxmatch_ && meas.lxmatch!=NOVAL_F &&
	      meas.lymatch<maxlymatch_ && meas.lymatch!=NOVAL_F) {

	    if (chkRecHit->getType()!=TrackingRecHit::missing || keepOriginalMissingHit_) {
	      expTrajMeasurements.erase(expTrajMeasurements.begin()+imatch);
	    }
	    if (JKDEBUG) {
	      std::cout<<" and matched to extrapolated hit "<<imatch<<" with accuracy "<<glmatch;
	    }
	  } else {
	    if (JKDEBUG) {
	      std::cout<<" and was not matched to any extrapolated hit, match="<<glmatch;
	      if (fabs(chklp.x())>=0.55 || fabs(chklp.y())>=3.0 ) 
		std::cout<<" but NON-FIDUCIAL";
	      if (chkmod.half==1) 
		std::cout<<" half module";
	      std::cout<<std::endl;
	    }
	    //continue; // Ignore hit
	  }
	  if (JKDEBUG) std::cout<<std::endl;

	  if (DEBUG) w2.Stop();
	  if (DEBUG) std::cout<<"PARTIALLY: Checking for layer "<<extrapolateTo_<<std::endl;
	  if (DEBUG) w2.Print();
	} // END of processing extrapolated hit


	//
	// Propagate valid layer2 or disk1 hits
	//

	bool lastValidL2=false;
	if ((chkmod.det==0&&chkmod.layer==extrapolateFrom_) || (chkmod.det==1&&chkmod.disk==1)) {
	  if (DEBUG) w2.Start();
	  if (chkRecHit->isValid()) {
	    if (itTraj==trajMeasurements.end()-1) {
	      lastValidL2=true;
	    } else {
	      itTraj++;
	      TransientTrackingRecHit::ConstRecHitPointer nextRecHit = itTraj->recHit();
	      ModuleData nextmod=getModuleData(nextRecHit->geographicalId().rawId(), 
					       federrors, "offline");
	      if (nextmod.det==0 && nextmod.layer==extrapolateTo_ ) {
		lastValidL2=true; //&& !nextRecHit->isValid()) lastValidL2=true;
	      }
	      itTraj--;
	    }
	  }

	  if (DEBUG) w2.Stop();
	  if (DEBUG) std::cout<<"PARTIALLY: Checking if it is Layer2/Disk1 hit\n";
	  if (DEBUG) w2.Print();
	}

	if (lastValidL2) {
	  if (DEBUG) w2.Start();
	  if (JKDEBUG) {
	    std::cout<<"Valid L"<<extrapolateFrom_<<" hit found. Extrapolating to L"<<
	      extrapolateTo_<<"\n";
	  }
#if CMSSW_VER == 53
	  const LayerMeasurements* theLayerMeasurements =
	    new LayerMeasurements(&*measurementTrackerHandle);
	  const std::vector<BarrelDetLayer*> pxbLayers = 
	    measurementTrackerHandle->geometricSearchTracker()->pixelBarrelLayers();
#else
	  const LayerMeasurements* theLayerMeasurements =
	    new LayerMeasurements(*measurementTrackerHandle, *measurementTrackerEventHandle);
	  std::vector<const BarrelDetLayer*> pxbLayers = 
	    measurementTrackerHandle->geometricSearchTracker()->pixelBarrelLayers();
#endif
	  const DetLayer* pxb1 = pxbLayers[extrapolateTo_-1];
	  const MeasurementEstimator* estimator = est.product();
	  const TrajectoryStateOnSurface tsosPXB2 = itTraj->updatedState();
	  expTrajMeasurements = 
	    theLayerMeasurements->measurements(*pxb1, tsosPXB2, *thePropagator, *estimator);
	  delete theLayerMeasurements;

	  if ( !expTrajMeasurements.empty()) {
	    if (JKDEBUG) {
	      std::cout <<"size of TM from propagation = "<<expTrajMeasurements.size()<< std::endl;
	    }

	    TrajectoryMeasurement pxb1TM(expTrajMeasurements.front());
	    //ConstReferenceCountingPointer<TransientTrackingRecHit> pxb1Hit;
	    //pxb1Hit = pxb1TM.recHit();
	    TransientTrackingRecHit::ConstRecHitPointer pxb1Hit = pxb1TM.recHit();
	    
	    if (pxb1Hit->geographicalId().rawId()!=0) {
	      ModuleData pxbMod=getModuleData(pxb1Hit->geographicalId().rawId(), federrors, "offline");
	      if (JKDEBUG) {
		std::cout << "Extrapolated hit found det=" << pxbMod.det<< " layer=";
		std::cout << pxbMod.layer<< " type=" <<pxb1Hit->getType()<< std::endl;
	      }
	    }
	  }
	  //
	  if (DEBUG) w2.Stop();
	  if (DEBUG) std::cout<<"PARTIALLY: Propagating Layer2/Disk1 hit\n";
	  if (DEBUG) w2.Print();
	}


	// end of new
	//
	if (DEBUG) w2.Start();

	meas.i=imeas++;
      
	if (JKDEBUG) std::cout << "Run " << evt_.run << " Event " << evt_.evt;
	if (JKDEBUG) std::cout << " TrajMeas #" << meas.i;

	TransientTrackingRecHit::ConstRecHitPointer recHit = itTraj->recHit();
	if (recHit->geographicalId().det()!=DetId::Tracker) continue;
      
	meas.mod=getModuleData(recHit->geographicalId().rawId(), federrors, "offline");
      
	uint subDetId = recHit->geographicalId().subdetId();
      
	if (subDetId == PixelSubdetector::PixelBarrel) {
	  if (JKDEBUG) std::cout << " PixelBarrel layer " << meas.mod.layer;
	  if (JKDEBUG) std::cout << " ladder " << meas.mod.ladder << " module " << meas.mod.module;
	  if (recHit->isValid()) track_.pix++;
	} else if (subDetId == PixelSubdetector::PixelEndcap) {
	  if (JKDEBUG) std::cout<<" PixelForward disk "<<meas.mod.disk<<" blade "<< meas.mod.blade;
	  if (JKDEBUG) std::cout << " panel " << meas.mod.panel << " module " << meas.mod.module;
	  if (recHit->isValid()) track_.pix++;
	} else if (subDetId == StripSubdetector::TIB) { 
	  if (JKDEBUG) std::cout << " TIB layer" << TIBDetId(meas.mod.rawid).layer();
	  if (recHit->isValid()) track_.strip++;
	} else if (subDetId == StripSubdetector::TOB) {
	  if (JKDEBUG) std::cout << " TOB layer" << TOBDetId(meas.mod.rawid).layer();
	  if (recHit->isValid()) track_.strip++;
	} else if (subDetId == StripSubdetector::TID) { 
	  if (JKDEBUG) std::cout << " TID wheel" << TIDDetId(meas.mod.rawid).wheel();
	  if (recHit->isValid()) track_.strip++;
	} else if (subDetId == StripSubdetector::TEC) {
	  if (JKDEBUG) std::cout << " TEC wheel" << TECDetId(meas.mod.rawid).wheel();
	  if (recHit->isValid()) track_.strip++;
	}
	
	meas.mod_on=getModuleData(recHit->geographicalId().rawId(), federrors, "online");
      
	if (!itTraj->updatedState().isValid()) 
	  if (JKDEBUG) std::cout<<", updatedState is invalid";
	if (JKDEBUG) std::cout<<", ";

// 	meas.validhit=1;
// 	if (!recHit->isValid()) {
// 	  meas.validhit=0;
// 	  std::cout << "RecHit is _invalid_";
// 	} else {
// 	std::cout << "RecHit is _valid_";
//      }
	
// 	meas.missing= (recHit->getType()==TrackingRecHit::missing) ? 1 : 0;
// 	meas.inactive= (recHit->getType()==TrackingRecHit::inactive) ? 1 : 0;
// 	meas.badhit= (recHit->getType()==TrackingRecHit::bad) ? 1 : 0;
	correctHitTypeAssignment(meas, recHit); // Needs to have meas.mod_on set correctly
	if (JKDEBUG) std::cout<<std::endl;
      
	//
	// Dealing only with pixel measurements from here on
	//
	if (subDetId!=PixelSubdetector::PixelBarrel && 
	    subDetId!=PixelSubdetector::PixelEndcap) continue;

	TrajectoryStateOnSurface predTrajState=trajStateComb(itTraj->forwardPredictedState(),
							     itTraj->backwardPredictedState());
	
	meas.glx=predTrajState.globalPosition().x();
	meas.gly=predTrajState.globalPosition().y();
	meas.glz=predTrajState.globalPosition().z();    
	meas.lx=predTrajState.localPosition().x();
	meas.ly=predTrajState.localPosition().y();
	meas.lz=predTrajState.localPosition().z();
	meas.lx_err=predTrajState.localError().positionError().xx();
	meas.ly_err=predTrajState.localError().positionError().yy();
	meas.telescope=0;
	meas.telescope_valid=0;
      	meas.onedge=1;
	if (fabs(meas.lx)<0.55 && fabs(meas.ly)<3.0) meas.onedge=0;

	LocalTrajectoryParameters predTrajParam= predTrajState.localParameters();
	LocalVector dir = predTrajParam.momentum()/predTrajParam.momentum().mag();
	meas.alpha = atan2(dir.z(), dir.x());
	meas.beta = atan2(dir.z(), dir.y());	

	//
	// New: Nov 7, 2010 - find closest cluster
	//
	findClosestClusters(iEvent, iSetup, recHit->geographicalId().rawId(),
			    meas.lx, meas.ly, meas.dx_cl, meas.dy_cl);
	for (size_t i=0; i<2; i++) {
	  if (meas.dx_cl[i]!=NOVAL_F) {
	    meas.d_cl[i]=sqrt(meas.dx_cl[i]*meas.dx_cl[i]+meas.dy_cl[i]*meas.dy_cl[i]);
	  } else {
	    meas.d_cl[i]=NOVAL_F;
	  }
	}

	//
	// Count hits for track (also used by telescope cut)
	//
	if (meas.gly<0) {
	  track_.pixhit[1]++;
	  if (recHit->isValid()) track_.validpixhit[1]++;
	} else {
	  track_.pixhit[0]++;
	  if (recHit->isValid()) track_.validpixhit[0]++;
	}

	if (subDetId==PixelSubdetector::PixelBarrel) {
	  track_.bpix[meas.mod.layer-1]++;
	  if (recHit->isValid()) track_.validbpix[meas.mod.layer-1]++;
	} else if (subDetId==PixelSubdetector::PixelEndcap) {
	  track_.fpix[meas.mod.disk-1]++;
	  if (recHit->isValid()) track_.validfpix[meas.mod.disk-1]++;
	}
	
	// Read associated cluster parameters
	if (recHit->isValid() && recHit->hit()!=0) {
	  const SiPixelRecHit *hit=(const SiPixelRecHit*)recHit->hit();
	  SiPixelRecHit::ClusterRef const& clust=hit->cluster();
	  if (clust.isNonnull()) {
	    meas.clu.charge=(*clust).charge()/1000.0;
	    meas.clu.size=(*clust).size();
	    meas.clu.edge=hit->isOnEdge() ? 1 : 0;
	    meas.clu.badpix=hit->hasBadPixels() ? 1 : 0;
	    meas.clu.tworoc=hit->spansTwoROCs() ? 1 : 0;
	    meas.clu.sizeX=(*clust).sizeX();
	    meas.clu.sizeY=(*clust).sizeY();
	    meas.clu.x=(*clust).x();
	    meas.clu.y=(*clust).y();
	    for (int i=0; i<(*clust).size() && i<1000; i++) {
	      meas.clu.adc[i]=float((*clust).pixelADC()[i])/1000.0;
	      meas.clu.pix[i][0]=(((*clust).pixels())[i]).x;
	      meas.clu.pix[i][1]=(((*clust).pixels())[i]).y;
	    }
	    meas.norm_charge = meas.clu.charge*
	      sqrt(1.0/(1.0/pow(tan(meas.alpha),2)+1.0/pow(tan(meas.beta),2)+1.0));
	  }
	}
	
	//meas.trk=track_;
	if (meas.mod.det==0&&meas.mod.layer==extrapolateTo_) {
	  if (keepOriginalMissingHit_ || meas.missing!=1) {
	    trajmeas.push_back(meas);
	    realHitFound++;
	  } else {
	    if (JKDEBUG) std::cout<<"Hit is dopped\n";
	  }
	} else {
	  trajmeas.push_back(meas);
	}
	
	if (DEBUG) w2.Stop();
	if (DEBUG) std::cout<<"PARTIALLY: Processing a trajectory measurement\n";
	if (DEBUG) w2.Print();

      } // loop on trajectory measurements
      

      //
      // Now add the non-valid extrapolated hits to trajmeas
      //
      if (JKDEBUG) std::cout<<"Adding missing trajectory measurements\n";

      for(itTraj=expTrajMeasurements.begin(); itTraj!=expTrajMeasurements.end() &&
	    !realHitFound; ++itTraj) {
	if (itTraj!=expTrajMeasurements.begin()) continue;
	if (DEBUG) w2.Start();

	TrajMeasurement meas;
	meas.i=imeas++;
	
	if (JKDEBUG) std::cout << "Run " << evt_.run << " Event " << evt_.evt;
	if (JKDEBUG) std::cout << " TrajMeas #" << meas.i;
	
	TransientTrackingRecHit::ConstRecHitPointer recHit = itTraj->recHit();
	if (recHit->geographicalId().det()!=DetId::Tracker) {
	  if (JKDEBUG) std::cout<<" hit not consistent with tracker, dropping it\n";
	  continue;
	}
	
	meas.mod=getModuleData(recHit->geographicalId().rawId(), federrors, "offline");
	
	uint subDetId = recHit->geographicalId().subdetId();
	
	if (subDetId == PixelSubdetector::PixelBarrel) {
	  if (JKDEBUG) std::cout << " PixelBarrel layer " << meas.mod.layer;
	  if (JKDEBUG) std::cout << " ladder " << meas.mod.ladder << " module " << meas.mod.module;
	  if (recHit->isValid()) track_.pix++;
	} else if (subDetId == PixelSubdetector::PixelEndcap) {
	  if (JKDEBUG) std::cout<<" PixelForward disk "<<meas.mod.disk<<" blade "<< meas.mod.blade;
	  if (JKDEBUG) std::cout << " panel " << meas.mod.panel << " module " << meas.mod.module;
	  if (recHit->isValid()) track_.pix++;
	} else if (subDetId == StripSubdetector::TIB) { 
	  if (JKDEBUG) std::cout << " TIB layer" << TIBDetId(meas.mod.rawid).layer();
	  if (recHit->isValid()) track_.strip++;
	} else if (subDetId == StripSubdetector::TOB) {
	  if (JKDEBUG) std::cout << " TOB layer" << TOBDetId(meas.mod.rawid).layer();
	  if (recHit->isValid()) track_.strip++;
	} else if (subDetId == StripSubdetector::TID) { 
	  if (JKDEBUG) std::cout << " TID wheel" << TIDDetId(meas.mod.rawid).wheel();
	  if (recHit->isValid()) track_.strip++;
	} else if (subDetId == StripSubdetector::TEC) {
	  if (JKDEBUG) std::cout << " TEC wheel" << TECDetId(meas.mod.rawid).wheel();
	  if (recHit->isValid()) track_.strip++;
	} else {
	  if (JKDEBUG) {
	    std::cout<<"Detector module not recognized, raw id="<<recHit->geographicalId().rawId();
	    std::cout << std::endl;
	  }
	  continue;
	}
	
	meas.mod_on=getModuleData(recHit->geographicalId().rawId(), federrors, "online");
	
	if (!itTraj->updatedState().isValid()) 
	  if (JKDEBUG) std::cout<<", updatedState is invalid";
	if (JKDEBUG) std::cout<<", ";
	
// 	meas.validhit=1;
// 	if (!recHit->isValid()) {
// 	  meas.validhit=0;
// 	  std::cout << "RecHit is _invalid_";
// 	} else {
// 	  std::cout << "RecHit is _valid_";
// 	}
	
// 	meas.missing= (recHit->getType()==TrackingRecHit::missing) ? 1 : 0;
// 	meas.inactive= (recHit->getType()==TrackingRecHit::inactive) ? 1 : 0;
// 	meas.badhit= (recHit->getType()==TrackingRecHit::bad) ? 1 : 0;
	correctHitTypeAssignment(meas, recHit); // Needs to have meas.mod_on set correctly
	if (JKDEBUG) std::cout<<std::endl;
	
	
// 	//
// 	// Dealing only with pixel measurements from here on
// 	//
// 	if (subDetId!=PixelSubdetector::PixelBarrel && 
// 	    subDetId!=PixelSubdetector::PixelEndcap) continue;

	TrajectoryStateOnSurface predTrajState=itTraj->updatedState();
	
	meas.glx=predTrajState.globalPosition().x();
	meas.gly=predTrajState.globalPosition().y();
	meas.glz=predTrajState.globalPosition().z();    
	meas.lx=predTrajState.localPosition().x();
	meas.ly=predTrajState.localPosition().y();
	meas.lz=predTrajState.localPosition().z();
	meas.lx_err=predTrajState.localError().positionError().xx();
	meas.ly_err=predTrajState.localError().positionError().yy();
	meas.telescope=0;
	meas.telescope_valid=0;
	meas.onedge=1;
	if (fabs(meas.lx)<0.55 && fabs(meas.ly)<3.0) meas.onedge=0;
 
// 	if (recHit->hit()!=NULL) {
// 	  const SiPixelRecHit *hit=(const SiPixelRecHit*)recHit->hit();
// 	  if (hit->isOnEdge()) {
// 	    meas.onedge=1;
// 	    std::cout<<"Hit is on edge\n";
// 	  }
// 	}

	LocalTrajectoryParameters predTrajParam= predTrajState.localParameters();
	LocalVector dir = predTrajParam.momentum()/predTrajParam.momentum().mag();
	meas.alpha = atan2(dir.z(), dir.x());
	meas.beta = atan2(dir.z(), dir.y());	

	//
	// New: Nov 7, 2010 - find closest cluster
	//
	findClosestClusters(iEvent, iSetup, recHit->geographicalId().rawId(),
			    meas.lx, meas.ly, meas.dx_cl, meas.dy_cl);
	for (size_t i=0; i<2; i++) {
	  if (meas.dx_cl[i]!=NOVAL_F) {
	    meas.d_cl[i]=sqrt(meas.dx_cl[i]*meas.dx_cl[i]+meas.dy_cl[i]*meas.dy_cl[i]);
	  } else {
	    meas.d_cl[i]=NOVAL_F;
	  }
	}

	//
	// Count hits for track (also used by telescope cut)
	//
	if (meas.gly<0) {
	  track_.pixhit[1]++;
	  if (recHit->isValid()) track_.validpixhit[1]++;
	} else {
	  track_.pixhit[0]++;
	  if (recHit->isValid()) track_.validpixhit[0]++;
	}

	if (subDetId==PixelSubdetector::PixelBarrel) {
	  track_.bpix[meas.mod.layer-1]++;
	  if (recHit->isValid()) track_.validbpix[meas.mod.layer-1]++;
	} else if (subDetId==PixelSubdetector::PixelEndcap) {
	  track_.fpix[meas.mod.disk-1]++;
	  if (recHit->isValid()) track_.validfpix[meas.mod.disk-1]++;
	}

	if (!keepOriginalMissingHit_ || meas.missing!=1) {
	  trajmeas.push_back(meas);
	} else {
	  if (JKDEBUG) std::cout<<"Hit is dopped\n";
	}
	
	if (DEBUG) w2.Stop();
	if (DEBUG) std::cout<<"PARTIALLY: Processing a missing trajectory measurement\n";
	if (DEBUG) w2.Print();

      } // loop on extrapolated trajectory measurements   
      

      //
      // Make telescope cut for Trajectory Measurements
      //
      
      if (track_.pixhit[0]>0 && track_.pixhit[1]>0) {
	for (size_t i=0; i<trajmeas.size(); i++) {
	  if (trajmeas[i].gly<0 && track_.pixhit[1]>1) {
	    trajmeas[i].telescope=1;
	    if (JKDEBUG) std::cout << "Layer " << trajmeas[i].mod.layer << " module " \
		      << trajmeas[i].mod.module << "(y=" << trajmeas[i].gly \
		      << ") passed telescope" << std::endl;
	  }
	  if (trajmeas[i].gly>0 && track_.pixhit[0]>1) {
	    trajmeas[i].telescope=1;
	    
	    if (JKDEBUG) {
	      std::cout << "Layer " << trajmeas[i].mod.layer << " module " \
			<< trajmeas[i].mod.module << "(y=" << trajmeas[i].gly \
			<< ") passed telescope" << std::endl;
	    }
	  }
	}
      }
      
      if (track_.validpixhit[0]>0 && track_.validpixhit[1]>0) {
	for (size_t i=0; i<trajmeas.size(); i++) {
	  if (trajmeas[i].gly<0 && (track_.validpixhit[1]>1 || !trajmeas[i].validhit)) {
	    trajmeas[i].telescope_valid=1;
	    if (JKDEBUG) {
	      std::cout << "Layer " << trajmeas[i].mod.layer << " module " \
			<< trajmeas[i].mod.module << "(y=" << trajmeas[i].gly \
			<< ") passed telescope_valid" << std::endl;
	    }
	  }
	  if (trajmeas[i].gly>0 && (track_.validpixhit[0]>1 || !trajmeas[i].validhit)) {
	    trajmeas[i].telescope_valid=1;
	    if (JKDEBUG) {
	      std::cout << "Layer " << trajmeas[i].mod.layer << " module " \
			<< trajmeas[i].mod.module << "(y=" << trajmeas[i].gly \
			<< ") passed telescope_valid" << std::endl;
	    }
	  }
	}
      }

      //
//       // New on 02.18.2010.
//       //
//       std::cout<<"Number of RecHits found: "<<trajmeas.size()<<std::endl;
//       std::cout<<"Number of RecHits not found: "<<exp_trajmeas.size()<<std::endl;
//       if (trajmeas.size()>=2) {
// 	trajmeas.insert(trajmeas.end(), exp_trajmeas.begin(), exp_trajmeas.end());
// 	std::cout<<"Number of total hits: "<<trajmeas.size()<<std::endl;
//       }
//       // 
//       // End of new ------------------
//       //

      // Fill track field of the traj measurements that are from this track

      if (DEBUG) w2.Start();
      for (size_t i=0; i<trajmeas.size(); i++) {
	trajmeas[i].trk=track_;
      }
      
      evt_.ntrackFPix[0]+=track_.fpix[0];
      evt_.ntrackFPix[1]+=track_.fpix[1];
      evt_.ntrackFPixvalid[0]+=track_.validfpix[0];
      evt_.ntrackFPixvalid[1]+=track_.validfpix[1];
      evt_.ntrackBPix[0]+=track_.bpix[0];
      evt_.ntrackBPix[1]+=track_.bpix[1];
      evt_.ntrackBPix[2]+=track_.bpix[2];
      evt_.ntrackBPixvalid[0]+=track_.validbpix[0];
      evt_.ntrackBPixvalid[1]+=track_.validbpix[1];
      evt_.ntrackBPixvalid[2]+=track_.validbpix[2];
 
      tracks_.push_back(track_);

//       for (size_t i=0; i<trajmeas_.size(); i++) {
// 	trajTree_->SetBranchAddress("event", &evt_);
// 	trajTree_->SetBranchAddress("traj", &trajmeas_[i]);
// 	trajTree_->SetBranchAddress("module", &trajmeas_[i].mod);
// 	trajTree_->SetBranchAddress("module_on", &trajmeas_[i].mod_on);
// 	trajTree_->SetBranchAddress("clust", &trajmeas_[i].clu);
// 	trajTree_->SetBranchAddress("track", &track_);
// 	trajTree_->Fill();
//       }
      
      trajmeas_.push_back(trajmeas);

      if (DEBUG) w2.Stop();
      if (DEBUG) std::cout<<"PARTIALLY: Push-back of this trajectory measurement\n";
      if (DEBUG) w2.Print();
      
      if (JKDEBUG) {
	w.Stop();
	std::cout<<"DONE: Processing a track\n";
	w.Print();
      }

      delete thePropagator;
    } // end of tracks loop

  }


  //
  // Event conditions:
  //

  // 1. has two valid trajectory hits too close to each other?
  // if so, set event good to "BAD"==0
  if (JKDEBUG) w.Start();

  float minDxy=9999.;
  int minTrk=9999;
  int minTraj=9999;
  int nTracks=trajmeas_.size();
  for (int itrk=0; itrk<nTracks-1; itrk++) {
    for (size_t i=0; i<trajmeas_[itrk].size(); i++) {
      if (trajmeas_[itrk][i].validhit!=1) continue;
      if (trajmeas_[itrk][i].mod.det!=0) continue;
      for (int jtrk=itrk+1; jtrk<nTracks; jtrk++) {
	for (size_t j=0; j<trajmeas_[jtrk].size(); j++) {
	  if (trajmeas_[jtrk][j].mod.det!=0) continue;
	  if (trajmeas_[itrk][i].mod.ladder!=trajmeas_[jtrk][j].mod.ladder) continue;
	  if (trajmeas_[itrk][i].mod.module!=trajmeas_[jtrk][j].mod.module) continue;	  
	  if (trajmeas_[itrk][i].mod.layer!=trajmeas_[jtrk][j].mod.layer) continue;
	  if (trajmeas_[jtrk][j].validhit!=1) continue;
	  float dx=trajmeas_[itrk][i].lx-trajmeas_[jtrk][j].lx;
	  float dy=trajmeas_[itrk][i].ly-trajmeas_[jtrk][j].ly;
	  float dxy=sqrt(dx*dx-dy*dy);
	  if (dxy>minDxy) continue;
	  minDxy=dxy;
	  minTrk=itrk;
	  minTraj=i;
	}
      }
    }
  }

  if (minDxy<9999) evt_.trackSep=minDxy;

  if (minDxy<0.2) {
    if (JKDEBUG) { 
      std::cout<<" *** Found two valid hits too close to each other on";
      std::cout<<" layer "<<trajmeas_[minTrk][minTraj].mod.layer
	       <<" ladder "<<trajmeas_[minTrk][minTraj].mod.ladder
	       <<" module "<<trajmeas_[minTrk][minTraj].mod.module
	       <<" : "<<minDxy<<std::endl;
    }
    if (evt_.good==1) evt_.good=0;
  }

  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Event filtering\n";
    w.Print();
  }

  //
  // Fill the trees
  //
  if (JKDEBUG) w.Start();

  eventTree_->SetBranchAddress("event", &evt_);
  eventTree_->Fill();

  #ifdef COMPLETE
  TrackData trk;
  trackTree_->SetBranchAddress("event", &evt_);
  trackTree_->SetBranchAddress("track", &trk);
  for (size_t i=0; i<tracks_.size(); i++) {
    trk = tracks_[i];
    trackTree_->Fill();
  }
  #endif

  Cluster clu;
  #ifndef SPLIT
  clustTree_->SetBranchAddress("event", &evt_);
  clustTree_->SetBranchAddress("clust", &clu);
  clustTree_->SetBranchAddress("clust_pix", &clu.pix);
  clustTree_->SetBranchAddress("module", &clu.mod);
  clustTree_->SetBranchAddress("module_on", &clu.mod_on);
  #else
  // Split mode
  // Non-splitted branches
  clustTree_->SetBranchAddress("event", &evt_);
  clustTree_->SetBranchAddress("module_on", &clu.mod_on);
  
  // clust
  // Paired branches
  clustTree_->SetBranchAddress("clust_xy",              &clu.x);
  // Split-mode branches
  clustTree_->SetBranchAddress("clust_i",               &clu.i);
  clustTree_->SetBranchAddress("clust_edge",            &clu.edge);
  clustTree_->SetBranchAddress("clust_badpix",          &clu.badpix);
  clustTree_->SetBranchAddress("clust_tworoc",          &clu.tworoc);
  clustTree_->SetBranchAddress("clust_size",            &clu.size);
  clustTree_->SetBranchAddress("clust_sizeXY",          &clu.sizeX);
  clustTree_->SetBranchAddress("clust_charge",          &clu.charge);
  clustTree_->SetBranchAddress("clust_adc",             &clu.adc);
  clustTree_->SetBranchAddress("clust_pix",             &clu.pix);
  #endif
  for (size_t i=0; i<clusts_.size(); i++) {
    clu = clusts_[i];
    #ifdef COMPLETE
    clustTree_->Fill();
    #else
    // Save only nth fraction of clusters to save space
    if (clu_stat_counter_%nthClusterToSave_==0) clustTree_->Fill();
    ++clu_stat_counter_;
    #endif
  }
  
  #ifndef SPLIT
  Digi dig;
  digiTree_->SetBranchAddress("event", &evt_);
  digiTree_->SetBranchAddress("digi", &dig);
  digiTree_->SetBranchAddress("module", &dig.mod);
  digiTree_->SetBranchAddress("module_on", &dig.mod_on);
  for (size_t i=0; i<digis_.size(); i++) {
    dig = digis_[i];
    digiTree_->Fill();
  }
  #endif

  TrajMeasurement traj;
  #ifndef SPLIT
  trajTree_->SetBranchAddress("event", &evt_);
  trajTree_->SetBranchAddress("traj", &traj);
  #ifdef COMPLETE
  trajTree_->SetBranchAddress("module", &traj.mod);
  #endif
  trajTree_->SetBranchAddress("module_on", &traj.mod_on);
  trajTree_->SetBranchAddress("clust", &traj.clu);
  trajTree_->SetBranchAddress("clust_pix", &traj.clu.pix);
  trajTree_->SetBranchAddress("track", &traj.trk);
  #else
  // Split mode
  // Non-splitted branches
  trajTree_->SetBranchAddress("event",                 &evt_);
  trajTree_->SetBranchAddress("module_on",             &traj.mod_on);
        
  // traj
  // Non-splitted branch
  trajTree_->SetBranchAddress("traj",                  &traj);
  #if SPLIT > 0
  // Paired branches
  trajTree_->SetBranchAddress("traj_occup",            &traj.nclu_mod);
  trajTree_->SetBranchAddress("traj_alphabeta",        &traj.alpha);
  trajTree_->SetBranchAddress("traj_dxy_cl",           &traj.dx_cl);
  trajTree_->SetBranchAddress("traj_dxy_hit",          &traj.dx_hit);
  #endif
  // Split-mode branches
  trajTree_->SetBranchAddress("traj_norm_charge",      &traj.norm_charge);
  #ifdef COMPLETE
  trajTree_->SetBranchAddress("traj_lz",               &traj.lz);
  trajTree_->SetBranchAddress("traj_glx",              &traj.glx);
  trajTree_->SetBranchAddress("traj_gly",              &traj.gly);
  trajTree_->SetBranchAddress("traj_glz",              &traj.glz);
  trajTree_->SetBranchAddress("traj_lxmatch",          &traj.lxmatch);
  trajTree_->SetBranchAddress("traj_lymatch",          &traj.lymatch);
  trajTree_->SetBranchAddress("traj_i",                &traj.i);
  trajTree_->SetBranchAddress("traj_onedge",           &traj.onedge);
  trajTree_->SetBranchAddress("traj_inactive",         &traj.inactive);
  trajTree_->SetBranchAddress("traj_badhit",           &traj.badhit);
  //   trajTree_->SetBranchAddress("traj_telescope",        &traj.telescope);
  //   trajTree_->SetBranchAddress("traj_telescope_valid",  &traj.telescope_valid);
  //   trajTree_->SetBranchAddress("traj_dmodule",          &traj.dmodule);
  //   trajTree_->SetBranchAddress("traj_dladder",          &traj.dladder);
  //   trajTree_->SetBranchAddress("traj_glmatch",          &traj.glmatch);
  //   trajTree_->SetBranchAddress("traj_lx_err",           &traj.lx_err);
  //   trajTree_->SetBranchAddress("traj_ly_err",           &traj.ly_err);
  //   trajTree_->SetBranchAddress("traj_lz_err",           &traj.lz_err);
  //   trajTree_->SetBranchAddress("traj_lxymatch",         &traj.lxymatch);
  //   trajTree_->SetBranchAddress("traj_res_hit",          &traj.res_hit);
  //   trajTree_->SetBranchAddress("traj_sig_hit",          &traj.sig_hit);
  //   trajTree_->SetBranchAddress("traj_d_cl",             &traj.d_cl);
  #endif
  
  // clust
  // Paired branches
  #if SPLIT > 1
  trajTree_->SetBranchAddress("clust_xy",              &traj.clu.x);
  #endif
  // Split-mode branches
  #ifdef COMPLETE
  //   trajTree_->SetBranchAddress("clust_i",               &traj.clu.i);
  trajTree_->SetBranchAddress("clust_edge",            &traj.clu.edge);
  trajTree_->SetBranchAddress("clust_badpix",          &traj.clu.badpix);
  trajTree_->SetBranchAddress("clust_tworoc",          &traj.clu.tworoc);
  #endif
  trajTree_->SetBranchAddress("clust_size",            &traj.clu.size);
  trajTree_->SetBranchAddress("clust_sizeXY",          &traj.clu.sizeX);
  trajTree_->SetBranchAddress("clust_adc",             &traj.clu.adc);
  #if SPLIT > 1
  trajTree_->SetBranchAddress("clust_charge",          &traj.clu.charge);
  trajTree_->SetBranchAddress("clust_pix",             &traj.clu.pix);
  #endif
  
  
  // track
  // Non-splitted branch
  trajTree_->SetBranchAddress("track",                 &traj.trk);
  // Paired branches
  trajTree_->SetBranchAddress("track_ndofchi2",        &traj.trk.ndof);
  // Split-mode branches
  trajTree_->SetBranchAddress("track_eta",             &traj.trk.eta);
  trajTree_->SetBranchAddress("track_phi",             &traj.trk.phi);
  #ifdef COMPLETE
  trajTree_->SetBranchAddress("track_theta",           &traj.trk.theta);
  trajTree_->SetBranchAddress("track_p",               &traj.trk.p);
  trajTree_->SetBranchAddress("track_algo",            &traj.trk.algo);
  trajTree_->SetBranchAddress("track_i",               &traj.trk.i);
  trajTree_->SetBranchAddress("track_pix",             &traj.trk.pix);
  trajTree_->SetBranchAddress("track_pixhit",          &traj.trk.pixhit);
  trajTree_->SetBranchAddress("track_validpixhit",     &traj.trk.validpixhit);
  trajTree_->SetBranchAddress("track_fpix",            &traj.trk.fpix);
  trajTree_->SetBranchAddress("track_bpix",            &traj.trk.bpix);
  trajTree_->SetBranchAddress("track_highPurity",      &traj.trk.highPurity);
  //   trajTree_->SetBranchAddress("track_fromVtx",         &traj.trk.fromVtx);
  #endif
  #endif

  for (size_t itrk=0; itrk<trajmeas_.size(); itrk++) {
    for (size_t i=0; i<trajmeas_[itrk].size(); i++) {
      #ifndef COMPLETE
      if (trajmeas_[itrk][i].trk.strip<minNStripHits_) continue;
      if (trajmeas_[itrk][i].trk.pt<minTrkPt_) continue;
      #endif
      
      // Search for closest other trajmeas before saving it
      float minD=10000.;
      for (size_t jtrk=0; jtrk<trajmeas_.size(); jtrk++) {
	for (size_t j=0; j<trajmeas_[jtrk].size(); j++) {
	  if (jtrk==itrk && j==i) continue;
	  if (trajmeas_[jtrk][j].mod_on.rawid!=trajmeas_[itrk][i].mod_on.rawid) continue;
	  float dx_hit=fabs(trajmeas_[itrk][i].lx-trajmeas_[jtrk][j].lx);
	  float dy_hit=fabs(trajmeas_[itrk][i].ly-trajmeas_[jtrk][j].ly);
	  float D=sqrt(dx_hit*dx_hit+dy_hit*dy_hit);
	  if (D<minD) {
	    minD=D;
	    trajmeas_[itrk][i].dx_hit=dx_hit;
	    trajmeas_[itrk][i].dy_hit=dy_hit;
	  }
	}
      }
      trajmeas_[itrk][i].clust_near = (trajmeas_[itrk][i].d_cl[0]!=NOVAL_F&&trajmeas_[itrk][i].d_cl[0]<0.05);
      trajmeas_[itrk][i].hit_near = (minD<0.5);

      // Module/ROC Occupancies
      trajmeas_[itrk][i].nclu_mod = (nclu_mod.count(trajmeas_[itrk][i].mod_on.rawid)) ?
	nclu_mod[trajmeas_[itrk][i].mod_on.rawid] : 0;
      trajmeas_[itrk][i].npix_mod = (npix_mod.count(trajmeas_[itrk][i].mod_on.rawid)) ?
	npix_mod[trajmeas_[itrk][i].mod_on.rawid] : 0;
      
      int RocID = get_RocID_from_local_coords(trajmeas_[itrk][i].lx, trajmeas_[itrk][i].ly, trajmeas_[itrk][i].mod_on);
      if (RocID!=NOVAL_I) {
	unsigned long int modroc = trajmeas_[itrk][i].mod_on.rawid * 100 + RocID;
	trajmeas_[itrk][i].nclu_roc = (nclu_roc.count(modroc)) ? nclu_roc[modroc] : 0;
	trajmeas_[itrk][i].npix_roc = (npix_roc.count(modroc)) ? npix_roc[modroc] : 0;
      } else { // Hit is outside module boundaries
	trajmeas_[itrk][i].nclu_roc = NOVAL_I;
	trajmeas_[itrk][i].npix_roc = NOVAL_I;
      }
      
      //=========================================================================================
      //                                    Efficiency Cuts
      //=========================================================================================
      //__________________________________________________________________________________________
      //                                    Event Selections
      
      // Number of Primary Verices >= 1 (where VtxZ<20 cm, VtxD0 < 2 cm, VtxNdof > 4)
      bool nvtx = evt_.nvtx>=1;
      
      // No FED Error in the Event
      // Previous cut: trajmeas_[itrk][i].mod_on.federr==0
      bool federr = evt_.federrs_size==0;
      
      //__________________________________________________________________________________________
      //                                    Track Selections
      
      // Track quality bit: HighPurity track
      bool hp = (trajmeas_[itrk][i].trk.quality&4)!=0;
      
      // Hit originates from track with Pt > 1.0 GeV
      // Previous pt cuts:
      // BPix - 1.0 GeV, FPix 1.2 GeV
      // 0.6 GeV
      bool pt = trajmeas_[itrk][i].trk.pt>1.0;
      
      // Number of hits in the Silicon Strip Tracker > 10
      bool nstrip = trajmeas_[itrk][i].trk.strip>10;
      
      // Track Impact parameters (track to vertex D0/Z distance)
      bool d0 = trajmeas_[itrk][i].mod_on.det==0 ?
        (trajmeas_[itrk][i].mod_on.layer==1 ? fabs(trajmeas_[itrk][i].trk.d0)<0.01 // L1
         : fabs(trajmeas_[itrk][i].trk.d0)<0.02 ) // L2,3
        : trajmeas_[itrk][i].mod_on.det==1 && fabs(trajmeas_[itrk][i].trk.d0)<0.05; // FPix
      
      bool dz = (trajmeas_[itrk][i].mod_on.det==0 && fabs(trajmeas_[itrk][i].trk.dz)<0.1) 
        ||(trajmeas_[itrk][i].mod_on.det==1 && fabs(trajmeas_[itrk][i].trk.dz)<0.5);
      
      // Require 2 Valid hit on 2 other layer(s) and/or disk(s)
      bool pixhit = trajmeas_[itrk][i].mod_on.det==0 ?
        ( // BPix
         trajmeas_[itrk][i].mod_on.layer==1 ?
         (trajmeas_[itrk][i].trk.validbpix[1]>0 && trajmeas_[itrk][i].trk.validbpix[2]>0) // L2 + L3
         || (trajmeas_[itrk][i].trk.validbpix[1]>0 && trajmeas_[itrk][i].trk.validfpix[0]>0) // L2 + D1
         || (trajmeas_[itrk][i].trk.validfpix[0]>0 && trajmeas_[itrk][i].trk.validfpix[1]>0) // D1 + D2
         : trajmeas_[itrk][i].mod_on.layer==2 ?
         (trajmeas_[itrk][i].trk.validbpix[0]>0 && trajmeas_[itrk][i].trk.validbpix[2]>0) // L1 + L3
         || (trajmeas_[itrk][i].trk.validbpix[0]>0 && trajmeas_[itrk][i].trk.validfpix[0]>0) // L1 + D1
         : trajmeas_[itrk][i].mod_on.layer==3 &&
         trajmeas_[itrk][i].trk.validbpix[0]>0 && trajmeas_[itrk][i].trk.validbpix[1]>0 ) // L1 + L2
        : trajmeas_[itrk][i].mod_on.det==1 &&
        ( // FPix
         abs(trajmeas_[itrk][i].mod_on.disk)==1 ?
         (trajmeas_[itrk][i].trk.validbpix[0]>0 && trajmeas_[itrk][i].trk.validfpix[1]>0) // L1 + D2
         || (trajmeas_[itrk][i].trk.validbpix[1]>0 && trajmeas_[itrk][i].trk.validfpix[1]>0) // or L2 + D2
         : abs(trajmeas_[itrk][i].mod_on.disk)==2 &&
         trajmeas_[itrk][i].trk.validbpix[0]>0 && trajmeas_[itrk][i].trk.validfpix[0]>0 ); // L1 + D1
      
      //__________________________________________________________________________________________
      //                          Module/ROC Fiducial Region Selections:
      //                       Cut on the local coordinates of a hit LX/LY
      //             Exclude Module/ROC edges and regions where nearby modules overlap
      
      // Module Edge Cut - LX
      bool edge_lx = trajmeas_[itrk][i].mod_on.det==0 ? 
        ( // BPix
         trajmeas_[itrk][i].mod_on.half==0 ? fabs(trajmeas_[itrk][i].lx)<0.65
         : trajmeas_[itrk][i].mod_on.half==1 && trajmeas_[itrk][i].lx>-0.3 && trajmeas_[itrk][i].lx<0.25 )
        : trajmeas_[itrk][i].mod_on.det==1 &&
        ( // FPix
         trajmeas_[itrk][i].mod_on.panel==1 ?
         ( // Panel 1
          trajmeas_[itrk][i].mod_on.module== 1 ? trajmeas_[itrk][i].lx>-0.15 && trajmeas_[itrk][i].lx<0.3
          : trajmeas_[itrk][i].mod_on.module==2 ? (abs(trajmeas_[itrk][i].mod_on.disk)==1  ? trajmeas_[itrk][i].lx>-0.55 && trajmeas_[itrk][i].lx<0.6 :
          			 abs(trajmeas_[itrk][i].mod_on.disk)==2 && trajmeas_[itrk][i].lx>-0.6 && trajmeas_[itrk][i].lx<0.3)
          : trajmeas_[itrk][i].mod_on.module==3 ? (abs(trajmeas_[itrk][i].mod_on.disk)==1  ? trajmeas_[itrk][i].lx> 0.3 && trajmeas_[itrk][i].lx<0.6 :
          			 abs(trajmeas_[itrk][i].mod_on.disk)==2 && trajmeas_[itrk][i].lx>-0.6 && trajmeas_[itrk][i].lx<0.5)
          : trajmeas_[itrk][i].mod_on.module==4 && trajmeas_[itrk][i].lx>-0.3 && trajmeas_[itrk][i].lx<0.15 )
         : trajmeas_[itrk][i].mod_on.panel==2 &&
         ( // Panel 2
          trajmeas_[itrk][i].mod_on.module==1 ? trajmeas_[itrk][i].lx>-0.55 && trajmeas_[itrk][i].lx<0.6
          : trajmeas_[itrk][i].mod_on.module==2 ? (abs(trajmeas_[itrk][i].mod_on.disk)==1  ? fabs(trajmeas_[itrk][i].lx)<0.55 :
          			 abs(trajmeas_[itrk][i].mod_on.disk)==2 && trajmeas_[itrk][i].lx>-0.6 && trajmeas_[itrk][i].lx<0.55)
          : trajmeas_[itrk][i].mod_on.module==3 && fabs(trajmeas_[itrk][i].lx)<0.55) );
      
      // ROC Edge cut - LX
      bool rocedge_lx = !((trajmeas_[itrk][i].mod_on.half==0||(trajmeas_[itrk][i].mod_on.det==1&&!(trajmeas_[itrk][i].mod_on.panel==1&&(trajmeas_[itrk][i].mod_on.module==1||trajmeas_[itrk][i].mod_on.module==4))))
          		&& fabs(trajmeas_[itrk][i].lx)<0.06);
      
      bool lx_fid = edge_lx && rocedge_lx;
      
      
      // Module Edge Cut - LY
      bool edge_ly = trajmeas_[itrk][i].mod_on.det==0 ? fabs(trajmeas_[itrk][i].ly)<3.1 // BPix
        : trajmeas_[itrk][i].mod_on.det==1 &&
        ( // FPix
         trajmeas_[itrk][i].mod_on.panel==1 ?
         ( // Panel 1
          trajmeas_[itrk][i].mod_on.module==1 ? fabs(trajmeas_[itrk][i].ly)<0.7
          : trajmeas_[itrk][i].mod_on.module==2 ? fabs(trajmeas_[itrk][i].ly)<1.1 
          && !(trajmeas_[itrk][i].mod_on.disk==-1 && trajmeas_[itrk][i].lx>0.25 && trajmeas_[itrk][i].ly> 0.75)
          && !(trajmeas_[itrk][i].mod_on.disk== 1 && trajmeas_[itrk][i].lx>0.25 && trajmeas_[itrk][i].ly<-0.75)
          : trajmeas_[itrk][i].mod_on.module==3 ? fabs(trajmeas_[itrk][i].ly)<1.5
          && !(trajmeas_[itrk][i].mod_on.disk==-1 && trajmeas_[itrk][i].ly > 1.1)
          && !(trajmeas_[itrk][i].mod_on.disk== 1 && trajmeas_[itrk][i].ly <-1.0)
          && !(trajmeas_[itrk][i].mod_on.disk==-2 && trajmeas_[itrk][i].lx>0.15 && trajmeas_[itrk][i].ly> 1.1)
          && !(trajmeas_[itrk][i].mod_on.disk== 2 && trajmeas_[itrk][i].lx>0.15 && trajmeas_[itrk][i].ly<-1.1)
          : trajmeas_[itrk][i].mod_on.module==4 && fabs(trajmeas_[itrk][i].ly)<1.9
          && !(trajmeas_[itrk][i].mod_on.disk==-2 && trajmeas_[itrk][i].ly> 1.5)
          && !(trajmeas_[itrk][i].mod_on.disk== 2 && trajmeas_[itrk][i].ly<-1.5) )
         : trajmeas_[itrk][i].mod_on.panel==2 &&
         ( // Panel 2
          trajmeas_[itrk][i].mod_on.module==1 ? fabs(trajmeas_[itrk][i].ly)<0.7
          : trajmeas_[itrk][i].mod_on.module==2 ? fabs(trajmeas_[itrk][i].ly)<1.1
          : trajmeas_[itrk][i].mod_on.module==3 && fabs(trajmeas_[itrk][i].ly)<1.6
          && !(trajmeas_[itrk][i].mod_on.disk>0 && trajmeas_[itrk][i].ly>1.5)
          && !(trajmeas_[itrk][i].mod_on.disk<0 && trajmeas_[itrk][i].ly<-1.5) ) );
      
      // ROC Edge cut - LY
      float ly_mod = fabs(trajmeas_[itrk][i].ly) + (trajmeas_[itrk][i].mod_on.det==1&&(trajmeas_[itrk][i].mod_on.panel+trajmeas_[itrk][i].mod_on.module)%2==1)*0.405;
      float d_rocedge = fabs(fabs(fmod(ly_mod,float(0.81))-0.405)-0.405);
      bool rocedge_ly = d_rocedge > 0.06;
      
      bool ly_fid = edge_ly && rocedge_ly;
      
      //__________________________________________________________________________________________
      //                            Efficiency Related Selections
      
      // Valid or Missing hit (not Inactive, Invalid or Bad)
      bool valmis = trajmeas_[itrk][i].validhit==1||trajmeas_[itrk][i].missing==1;
      
      // Nearest other hit is far away (D_hit > 0.5 cm)
      bool hitsep = trajmeas_[itrk][i].hit_near==0;
      
      // Check if the Roc is not on the Badoc List
      bool goodroc = true;
      
      if (RocID == NOVAL_I) { // Hit is outside module boundaries
	goodroc = false;
      } else {
	int shl = (trajmeas_[itrk][i].mod_on.det==0) ? 
	  2 * (trajmeas_[itrk][i].mod_on.module<0) + (trajmeas_[itrk][i].mod_on.ladder<0)
	  : 2 * (trajmeas_[itrk][i].mod_on.disk<0) + (trajmeas_[itrk][i].mod_on.blade<0);
	int detshl = trajmeas_[itrk][i].mod_on.det*4 + shl;
	
	// BPix
	int seclyrldr = (trajmeas_[itrk][i].mod_on.det==1) ? NOVAL_I :
	  abs(trajmeas_[itrk][i].mod_on.ladder)-1 + (trajmeas_[itrk][i].mod_on.layer>1)*10 + (trajmeas_[itrk][i].mod_on.layer>2)*16;
	int modroc = (trajmeas_[itrk][i].mod_on.det==1) ? NOVAL_I :
	  (RocID==NOVAL_I ? NOVAL_I : (abs(trajmeas_[itrk][i].mod_on.module)-1)*16 + RocID);
	
	// FPix
	int dskbld = (trajmeas_[itrk][i].mod_on.det==0) ? NOVAL_I :
	  (abs(trajmeas_[itrk][i].mod_on.disk)-1)*12 + abs(trajmeas_[itrk][i].mod_on.blade)-1;
	int pnlplqroc = (trajmeas_[itrk][i].mod_on.det==0) ? NOVAL_I :
	  ( (trajmeas_[itrk][i].mod_on.panel==1) ? 
	    ( (trajmeas_[itrk][i].mod_on.module>1)*2 + (trajmeas_[itrk][i].mod_on.module>2)*6 + (trajmeas_[itrk][i].mod_on.module>3)*8 )
	    :  ( 21 + (trajmeas_[itrk][i].mod_on.module>1)*6 + (trajmeas_[itrk][i].mod_on.module>2)*8 )
	    ) + RocID;
	
	int rocnum = detshl * 10000 + ((trajmeas_[itrk][i].mod_on.det) ? dskbld : seclyrldr) * 100 + ((trajmeas_[itrk][i].mod_on.det) ? pnlplqroc : modroc);
	
	goodroc = !badroc_list_.count(rocnum);
      }
      
      trajmeas_[itrk][i].pass_effcuts = nvtx && federr && hp && pt && nstrip && d0 && dz && pixhit && goodroc && lx_fid && ly_fid && valmis && hitsep;
      
      traj = trajmeas_[itrk][i];
      trajTree_->Fill();
    }
  }
  
  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Filling event tree\n";
    w.Print();
  }
}


void TimingStudy::correctHitTypeAssignment(TrajMeasurement& meas, 
					   TransientTrackingRecHit::ConstRecHitPointer& recHit) {
  
  meas.validhit= (recHit->getType()==TrackingRecHit::valid) ? 1 : 0;
  meas.missing= (recHit->getType()==TrackingRecHit::missing) ? 1 : 0;
  meas.inactive= (recHit->getType()==TrackingRecHit::inactive) ? 1 : 0;
  meas.badhit= (recHit->getType()==TrackingRecHit::bad) ? 1 : 0;
  
  if (!recHit->isValid()) {
    //meas.validhit=0;
    if (JKDEBUG) std::cout << "RecHit is _non-valid_";
  } else {
    if (JKDEBUG) std::cout<<"RecHits is _valid_";
  }  
  if (JKDEBUG) std::cout<<" ("<<meas.validhit<<","<<meas.missing<<","<<meas.inactive<<","
			<<meas.badhit<<")";  
}


void TimingStudy::findClosestClusters(const edm::Event& iEvent, const edm::EventSetup& iSetup, 
				      uint32_t rawId, 
				      float lx, float ly, float* dx_cl, float* dy_cl) {
  
  for (size_t i=0; i<2; i++) dx_cl[i]=dy_cl[i]=NOVAL_F;

  edm::ESHandle<PixelClusterParameterEstimator> cpEstimator;
  iSetup.get<TkPixelCPERecord>().get("PixelCPEGeneric", cpEstimator);
  if (!cpEstimator.isValid()) return;
  const PixelClusterParameterEstimator &cpe(*cpEstimator);

  edm::ESHandle<TrackerGeometry> tracker;
  iSetup.get<TrackerDigiGeometryRecord>().get(tracker);
  if (!tracker.isValid()) return;
  const TrackerGeometry *tkgeom = &(*tracker);

  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > clusterCollectionHandle;
#if CMSSW_VER >= 76
  iEvent.getByToken(clustersToken_, clusterCollectionHandle);
#else
  iEvent.getByLabel("siPixelClusters", clusterCollectionHandle);
#endif
  if (!clusterCollectionHandle.isValid()) return;

  const edmNew::DetSetVector<SiPixelCluster>& clusterCollection=*clusterCollectionHandle;
  edmNew::DetSetVector<SiPixelCluster>::const_iterator itClusterSet= clusterCollection.begin();

  float minD[2];
  minD[0]=minD[1]=10000.;

  for ( ; itClusterSet!=clusterCollection.end(); itClusterSet++) {
    
    DetId detId(itClusterSet->id());
    
    if (detId.rawId()!=rawId) continue;

    unsigned int subDetId=detId.subdetId();
    if (subDetId!=PixelSubdetector::PixelBarrel &&
	subDetId!=PixelSubdetector::PixelEndcap) {
      std::cout << "ERROR: not a pixel cluster!!!" << std::endl; // should not happen
      continue;
    }

    const PixelGeomDetUnit *pixdet = (const PixelGeomDetUnit*) tkgeom->idToDetUnit(detId);

    edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=itClusterSet->begin();  
    for(; itCluster!=itClusterSet->end(); ++itCluster) {
      //const Surface* surface;
      //surface = &(tracker->idToDet(detId)->surface());
      //GlobalPoint gp;
      LocalPoint lp(itCluster->x(), itCluster->y(), 0.);
      if (usePixelCPE_) {
#if CMSSW_VER >= 71
	PixelClusterParameterEstimator::ReturnType params=cpe.getParameters(*itCluster,*pixdet);
	lp = std::get<0>(params);
#else
	PixelClusterParameterEstimator::LocalValues params=cpe.localParameters(*itCluster,*pixdet);
	lp = params.first;
	//gp = surface->toGlobal(params.first);
#endif
      } 
      //else {
      //  LocalPoint lp(itCluster->x(), itCluster->y(), 0.);
      //  gp = surface->toGlobal(lp);
      //}
      //floatD=sqrt((gp.x()-glx)*(gp.x()-glx)+(gp.y()-gly)*(gp.y()-gly)+(gp.z()-glz)*(gp.z()-glz));
      float D = sqrt((lp.x()-lx)*(lp.x()-lx)+(lp.y()-ly)*(lp.y()-ly));
      if (D<minD[0]) {
	minD[1]=minD[0];
	dx_cl[1]=dx_cl[0];
	dy_cl[1]=dy_cl[0];
	minD[0]=D;
	dx_cl[0]=lp.x();
	dy_cl[0]=lp.y();
      } else if (D<minD[1]) {
	minD[1]=D;
	dx_cl[1]=lp.x();
	dy_cl[1]=lp.y();
      }
    } // loop on cluster sets
  }

  for (size_t i=0; i<2; i++) {
    if (minD[i]<9999.) {
      dx_cl[i]=fabs(dx_cl[i]-lx);
      dy_cl[i]=fabs(dy_cl[i]-ly);
    }
  }

}




TimingStudy::ModuleData 
  TimingStudy::getModuleData(uint32_t rawId, const std::map<uint32_t, int>& federrors, std::string scheme) {

  ModuleData offline;
  ModuleData online;

  offline.rawid = online.rawid = rawId;
  int subDetId = DetId(offline.rawid).subdetId();

  std::map<uint32_t, int>::const_iterator federrors_it=federrors.find(offline.rawid);
  offline.federr=online.federr= (federrors_it!=federrors.end()) ? federrors_it->second :0;

  if (subDetId == PixelSubdetector::PixelBarrel) {
    PXBDetId pxbid=PXBDetId(offline.rawid);
    offline.det=online.det=0;
    offline.layer=online.layer=pxbid.layer();
    offline.ladder=pxbid.ladder();
    offline.module=pxbid.module();
    offline.half=0;
    if (offline.layer==1) {
      if (offline.ladder==5||offline.ladder==6||offline.ladder==15||offline.ladder==16) {
	offline.half=1;
      }
      offline.outer=offline.ladder%2;
    } else if (offline.layer==2) {
      if (offline.ladder==8||offline.ladder==9||offline.ladder==24||offline.ladder==25) {
	offline.half=1;
      }
      offline.outer=1-offline.ladder%2;
    } else if (offline.layer==3) {
      if (offline.ladder==11||offline.ladder==12||offline.ladder==33||offline.ladder==34) {
	offline.half=1;
      }
      offline.outer=offline.ladder%2;
    }
    
    if (scheme.find("on")==std::string::npos) return offline;

    if (offline.layer==1) {
      if (offline.ladder<=5 && offline.ladder>=1)        online.ladder = 6-offline.ladder;
      else if (offline.ladder<=15 && offline.ladder>=6)  online.ladder = 5-offline.ladder;
      else if (offline.ladder<=20 && offline.ladder>=16) online.ladder = 26-offline.ladder;
    } else if (offline.layer==2) {
      if (offline.ladder<=8 && offline.ladder>=1)        online.ladder = 9-offline.ladder;
      else if (offline.ladder<=24 && offline.ladder>=9)  online.ladder = 8-offline.ladder;
      else if (offline.ladder<=32 && offline.ladder>=25) online.ladder = 41-offline.ladder;
    } if (offline.layer==3) {
      if (offline.ladder<=11 && offline.ladder>=1)       online.ladder = 12-offline.ladder;
      else if (offline.ladder<=33 && offline.ladder>=12) online.ladder = 11-offline.ladder;
      else if (offline.ladder<=44 && offline.ladder>=34) online.ladder = 56-offline.ladder;
    }

    if (offline.module>=1 && offline.module<=4)          online.module = offline.module-5;
    else if (offline.module>=4 && offline.module<=8)     online.module = offline.module-4;

    online.shl=online.shell_num();
    online.half=offline.half;
    online.outer=offline.outer;

    std::map<std::string, std::string>::const_iterator it;
    std::ostringstream sector;
    sector << "BPix_" << online.shell() << "_SEC";

    for (size_t i=1; i<=8; i++) {
      std::ostringstream ladder;
      ladder << sector.str() <<i << "_LYR" << online.layer << "_LDR" << abs(online.ladder);
      for (int j=0; j<2; j++) {
	std::string type= (j) ? "H_MOD" : "F_MOD";
	std::ostringstream mod_a;
	mod_a << ladder.str() << type << abs(online.module) << "_A";
	it=portcardmap.find(mod_a.str());
	if (it==portcardmap.end()) {
	  std::ostringstream mod_b;
	  mod_b << ladder.str() << type << abs(online.module) << "_B";
	  it=portcardmap.find(mod_b.str());
	}
	if (it!=portcardmap.end()) {
	  online.sec=i;
	  sector << i << "_PRT%d";
	  sscanf(it->second.data(), sector.str().data(), &online.prt);
	  if (JKDEBUG) std::cout<<" (" << it->second << ": " << online.sec << ", " << online.prt 
				<< ")";
	  break;
	}
      }
      if (online.prt!=NOVAL_I) break;
    }

    return online;
  }


  if (subDetId == PixelSubdetector::PixelEndcap) {

    PXFDetId pxfid=PXFDetId(offline.rawid);
    offline.det=online.det=1;
    offline.side=pxfid.side();
    offline.disk=pxfid.disk();
    offline.blade=pxfid.blade();
    offline.panel=online.panel=pxfid.panel();
    offline.module=online.module=pxfid.module(); // not checked

    if (scheme.find("on")==std::string::npos) return offline;

    online.disk=offline.disk*(offline.side*2-3);

    if (offline.blade<=6 && offline.blade>=1)            online.blade = 7-offline.blade;
    else if (offline.blade<=18 && offline.blade>=7)      online.blade = 6-offline.blade;
    else if (offline.blade<=24 && offline.blade>=19)     online.blade = 31-offline.blade;

    online.shl=online.shell_num();

    std::map<std::string, std::string>::const_iterator it;
    std::ostringstream disk;
    disk << "FPix_" << online.shell() << "_D" << abs(online.disk);
    std::ostringstream panel;
    panel << disk.str() << "_BLD" << abs(online.blade) << "_PNL" << online.panel;
    it=portcardmap.find(panel.str());
    if (it!=portcardmap.end()) {
      disk << "_PRT%d";
      sscanf(it->second.data(), disk.str().data(), &online.prt);
      if (JKDEBUG) std::cout<<" (" << it->second << ")";
    }

    return online;
  }

  return offline;
}


int TimingStudy::get_RocID_from_cluster_coords(const float& x, const float& y, const TimingStudy::ModuleData& mod_on) {
  if (x!=NOVAL_F&&y!=NOVAL_F) {
    if (mod_on.det==0) {
      int ny = (int)(y / 52.0);
      int roc = (mod_on.half) ? ((mod_on.module<0)*8+ny) : ((mod_on.module>0) ? ((x>80.0) ? ny : 15-ny) : ((x>80.0) ? 8+ny : 7-ny));
      return roc;
    } else if (mod_on.det==1) {
      int nrocly = mod_on.module + mod_on.panel;
      int rocy = (int)(y/52.0);
      int roc = ((mod_on.panel==2 && x<80.0)
		 || (mod_on.panel==1 && (mod_on.module==1||mod_on.module==4))
		 || (mod_on.panel==1 && (mod_on.module==2||mod_on.module==3) && x>80.0 )) ? rocy
	: 2*nrocly -1 - rocy;
      return roc;
    } else return NOVAL_I;
  } else return NOVAL_I;
}

int TimingStudy::get_RocID_from_local_coords(const float& lx, const float& ly, const TimingStudy::ModuleData& mod_on) {
  if (lx!=NOVAL_F&&ly!=NOVAL_F) {
    if (mod_on.det==0) {
      if (fabs(ly)<3.24&&((mod_on.half==0&&fabs(lx)<0.8164)||(mod_on.half==1&&fabs(lx)<0.4082))) {
	int ny = (int)(ly / 0.81 + 4);
	int roc = (mod_on.half) ? ((mod_on.module<0)*8+ny) : ((mod_on.module>0) ? ((lx>0.0) ? ny : 15-ny) : ((lx>0.0) ? 8+ny : 7-ny));
	return roc;
      } else return NOVAL_I;
    } else if (mod_on.det==1) {
      int nrocly = mod_on.module + mod_on.panel;
      if (fabs(ly)<(nrocly*0.405) && ( ( !(mod_on.panel==1&&(mod_on.module==1||mod_on.module==4)) && fabs(lx)<0.8164 ) || (fabs(lx)<0.4082) )) {
	int rocy = (int)((ly+(nrocly%2)*0.405)/0.81+nrocly/2);
	int roc = ((mod_on.panel==2 && lx<0.0)
		   || (mod_on.panel==1 && (mod_on.module==1||mod_on.module==4))
		   || (mod_on.panel==1 && (mod_on.module==2||mod_on.module==3) && lx>0.0 )) ? rocy
	  : 2*nrocly -1 - rocy;
	return roc;
      } else return NOVAL_I;
    } else return NOVAL_I;
  } else return NOVAL_I;
}

// int TimingStudy::get_RocID_from_local_coords(const float &lx, const float &ly, const TimingStudy::ModuleData& mod_on) {
//   if (mod_on.det==0) {
//     if (!(fabs(ly)<3.24&&((mod_on.half==0&&fabs(lx)<0.8164)||(mod_on.half==1&&fabs(lx)<0.4082)))) return NOVAL_I;
//   } else {
//     int nrocly = mod_on.module + mod_on.panel;
//     if (!(fabs(ly)<(nrocly*0.405) && ( ( !(mod_on.panel==1&&(mod_on.module==1||mod_on.module==4)) && fabs(lx)<0.8164 ) || (fabs(lx)<0.4082) ))) return NOVAL_I;
//   }	
//   float y = (ly/0.81 + (mod_on.det ? (mod_on.module+mod_on.panel)/2.0 : 4.0)) * 52;
//   //float y = 208.052 + 65.9161*(ly-int(ly/0.8066)*0.0266)); // BPix only - ROC gap/long pixel correction
//   
//   float x = mod_on.det ? (panel==1&&(module==1||module==4) ? 40.77+94.5*lx : 79.55+((lx>0)*1.5)+96.78*lx) :
//     (mod_on.half ? 41.06+99.21*lx : 79.8+((lx>0)*1.52)+99.4*lx);
//   
//   return get_RocID_from_cluster_coords(x. y. mod_on);
// }



// alternative way to get if a cluster is at the edge:
// 	const TrackerGeometry& tkgeom(*tkGeom_);
// 	const PixelGeomDetUnit* detUnit =
// 	  (const PixelGeomDetUnit*)tkgeom.idToDet(recHit->geographicalId());
// 	const RectangularPixelTopology& detUnitTop = 
// 	  (const RectangularPixelTopology&)(detUnit->specificTopology());
// 	if (detUnitTop.isItEdgePixelInX((*clust).minPixelRow()) ||
// 	    detUnitTop.isItEdgePixelInX((*clust).maxPixelRow()) ||
// 	    detUnitTop.isItEdgePixelInY((*clust).minPixelCol()) ||
// 	    detUnitTop.isItEdgePixelInY((*clust).minPixelCol()) ) meas.clu.edge=1;


// define this as a plug-in
//


 //      const reco::HitPattern& hitP = track.trackerExpectedHitsInner();
//       int nExpPixHits=0;
//       std::vector<TrajMeasurement> exp_trajmeas;

//       std::cout<<"Analyze Expected Inner HitPattern...\n";

//       //const reco::HitPattern& hitP = track.hitPattern();
//       for (int ihitP=hitP.numberOfHits()-1; ihitP>=0; ihitP--) {
// 	uint32_t hit = hitP.getHitPattern(ihitP);
// 	std::cout << "Run " << evt_.run << " Event " <<evt_.evt<<" track "<<itrack<<"("<<hit<<")";
// 	if (hitP.pixelEndcapHitFilter(hit)) { // if PXF
// 	  TrajMeasurement meas;
// 	  nExpPixHits--;
// 	  meas.i=nExpPixHits;
// 	  meas.mod.det=1;
// 	  meas.mod.disk=hitP.getLayer(hit);
// 	  int type=hitP.getHitType(hit);
// 	  std::cout << " disk "<< meas.mod.disk <<" expected hit type "<< type <<std::endl;
// 	  meas.validhit= (type==0) ? 1 : 0;
// 	  meas.missing= (type==1) ? 1 : 0;
// 	  meas.inactive= (type==2) ? 1 : 0;
// 	  meas.badhit=  (type==3) ? 1 : 0;
// 	  meas.mod_on.det=meas.mod.det;
// 	  meas.mod_on.disk=meas.mod.disk;
// 	  if (meas.mod.disk==1) 
// 	    trajmeas.push_back(meas);
// 	} else if (hitP.pixelBarrelHitFilter(hit)) { // or PXB
// 	  TrajMeasurement meas;
// 	  nExpPixHits--;
// 	  meas.i=nExpPixHits;
// 	  meas.mod.det=0;
// 	  meas.mod.layer=hitP.getLayer(hit);
// 	  int type=hitP.getHitType(hit);
// 	  std::cout << " layer "<< meas.mod.layer <<" expected hit type "<< type <<std::endl;
// 	  meas.validhit= (type==0) ? 1 : 0;
// 	  meas.missing= (type==1) ? 1 : 0;
// 	  meas.inactive= (type==2) ? 1 : 0;
// 	  meas.badhit=  (type==3) ? 1 : 0;
// 	  meas.mod_on.det=meas.mod.det;
// 	  meas.mod_on.layer=meas.mod.layer;
// 	  if (meas.mod.layer==1) 
// 	    trajmeas.push_back(meas);
// 	} else {
// 	  std::cout << " not a pixel hit ";
// 	  printf("%x\n", hit);
// 	}
//       }

//       // restore order (from in to out)
//       for (int iTM=trajmeas.size()-1; iTM>=0; iTM--) exp_trajmeas.push_back(trajmeas[iTM]);
//       trajmeas.clear();

//       std::cout<<"Analyze HitPattern...\n";

//       const reco::HitPattern& valHitP = track.hitPattern();
//       for (int ihitP=valHitP.numberOfHits()-1; ihitP>=0; ihitP--) {
// 	uint32_t hit = valHitP.getHitPattern(ihitP);
// 	std::cout << "Run " << evt_.run << " Event " <<evt_.evt<<" track "<<itrack<<"("<<hit<<")";
// 	if (valHitP.pixelEndcapHitFilter(hit)) { // if PXF
// 	  std::cout << " disk "<<valHitP.getLayer(hit) <<" has hit type "
// 		    << valHitP.getHitType(hit) <<std::endl;
// 	} else if (valHitP.pixelBarrelHitFilter(hit)) { // or PXB
// 	  std::cout << " layer "<<valHitP.getLayer(hit) <<" has hit type "
// 		    << valHitP.getHitType(hit) <<std::endl;
// 	} else {
// 	  std::cout << " not a pixel hit ";
// 	  printf("%x\n", hit);
// 	}
//       }      
      
//       //
//       // End of new ----
//       //



DEFINE_FWK_MODULE(TimingStudy);
