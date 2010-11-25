
//
//
//
//

//
//
// ------------------------------------------------------------------------------------------------

#include "DataFormats/MuonReco/interface/MuonFwd.h"
#include "DataFormats/MuonReco/interface/Muon.h"
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
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/PixelClusterParameterEstimator.h"
#include "TimingStudy.h"
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"
#include <Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h>
#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/FWLite/interface/TFileService.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "RecoTracker/MeasurementDet/interface/MeasurementTracker.h"
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "TrackingTools/KalmanUpdators/interface/Chi2MeasurementEstimator.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "RecoTracker/TkDetLayers/interface/GeometricSearchTracker.h"
#include "TrackingTools/MeasurementDet/interface/LayerMeasurements.h"

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

//set to 1 in order to switch on logging of debug info - may create large log file 
//(set to 0 for Grid runs)
#define JKDEBUG 0

using namespace std;
using namespace edm;
using namespace reco;


TimingStudy::TimingStudy(edm::ParameterSet const& iConfig) : 
  iConfig_(iConfig)
{
  eventTree_=NULL;
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
  minNStripHits_=11;
}


TimingStudy::~TimingStudy() { }  


void TimingStudy::endJob() 
{
  outfile_->Write();
  outfile_->Close();
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
  if (iConfig_.exists("minNStripHits")) {
    minNStripHits_=iConfig_.getParameter<int>("minNStripHits");
    std::cout<<"NON-DEFAULT PARAMETER: minNStripHits= "<<minNStripHits_<<std::endl;
  }

//   es.get<TrackerDigiGeometryRecord>().get(tkGeom_);
//   es.get<IdealMagneticFieldRecord>().get(magneticField_);

  //edm::Service<TFileService> fs;
  outfile_ = new TFile(fileName.c_str(), "RECREATE");
  std::cout<<"Output file created: "<<outfile_->GetName()<<std::endl;

  // The event
  eventTree_ = new TTree("eventTree", "The event");
  eventTree_->Branch("event", &evt_, evt_.list.data());

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


  edm::LogInfo("TimingStudy") << "Begin Job Finished" << std::endl;

}



void TimingStudy::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{

  TStopwatch w;
  if (JKDEBUG) w.Start();
  init_all(); // clear all containers
  if (JKDEBUG) {
    w.Stop();
    std::cout<<"DONE: Initializing event variables\n";
    w.Print();
  }
  iSetup.get<TrackerDigiGeometryRecord>().get(tkGeom_);
  iSetup.get<IdealMagneticFieldRecord>().get(magneticField_);


  //
  // Read event info
  //
  if (JKDEBUG) w.Start();
  evt_.run=iEvent.id().run();
  evt_.evt=iEvent.id().event();
  evt_.ls=iEvent.luminosityBlock();
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
  std::string trajTrackCollectionInput=
    iConfig_.getParameter<std::string>("trajectoryInput");
  iEvent.getByLabel(trajTrackCollectionInput, trajTrackCollectionHandle);


  if (trajTrackCollectionHandle.isValid()||1) {
    if (JKDEBUG) std::cout << "\n\nRun " << evt_.run << " Event " << evt_.evt;
    if (JKDEBUG) std::cout << " Number of tracks =" <<trajTrackCollectionHandle->size()<<std::endl;

    evt_.ntracks=trajTrackCollectionHandle->size();

    if (trajTrackCollectionInput=="ctfRefitter") {
      //
      // Consider events with exactly one track
      //
      //if (trajTrackCollectionHandle->size()!=1) return;
    }
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

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - >


  //
  // Fill event with muon time
  //
  if (JKDEBUG) w.Start();

  edm::Handle<MuonCollection> muonCollectionHandle;
  iEvent.getByLabel("muonsWitht0Correction", muonCollectionHandle);

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
  iEvent.getByLabel("offlinePrimaryVertices", vertexCollectionHandle);

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
    if (fabs(it->z())<=15. && fabs(it->position().rho())<=2.) evt_.nvtx++;
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
  iEvent.getByLabel("siPixelDigis", digiCollectionHandle);

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
      
      ModuleData module=getModuleData(detId.rawId());
      
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
      ModuleData module_on=getModuleData(detId.rawId(), "online");
      if (JKDEBUG) std::cout << ": " << itDigiSet->size() << std::endl;
      
      edm::DetSet<PixelDigi>::const_iterator itDigi=itDigiSet->begin();
      
      for(; itDigi!=itDigiSet->end(); ++itDigi) {
	Digi digi;
	digi.i=itDigi-itDigiSet->begin();
	digi.row=itDigi->row();
	digi.col=itDigi->column();
	digi.adc=itDigi->adc();
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

  #ifdef COMPLETE

  if (JKDEBUG) w.Start();

  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > clusterCollectionHandle;
  iEvent.getByLabel("siPixelClusters", clusterCollectionHandle);

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
      
      ModuleData module=getModuleData(detId.rawId());
      
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
      ModuleData module_on=getModuleData(detId.rawId(), "online");
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
	}
	
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
    w.Stop();
    std::cout<<"DONE: Reading the Clusters\n";
    w.Print();
  }

  #endif

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
      
      TrackData track_;
      //trajmeas_.clear();
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
      track_.d0=track.dxy(bestVtx->position())*-1.0;
      track_.dz=track.dz(bestVtx->position());
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

      //
      // New on 02.18.2010
      // 

      ESHandle<MeasurementTracker> measurementTrackerHandle;
      iSetup.get<CkfComponentsRecord>().get(measurementTrackerHandle);
      
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
	ModuleData chkmod=getModuleData(chkRecHit->geographicalId().rawId(), "offline");

	TrajectoryStateOnSurface chkPredTrajState=trajStateComb(itTraj->forwardPredictedState(),
								itTraj->backwardPredictedState());
	float chkx=chkPredTrajState.globalPosition().x();
	float chky=chkPredTrajState.globalPosition().y();
	float chkz=chkPredTrajState.globalPosition().z();
	LocalPoint chklp=chkPredTrajState.localPosition();

	if (chkmod.det==0&&chkmod.layer==extrapolateTo_) {
	  if (DEBUG) w2.Start();
	  if (JKDEBUG) std::cout<<"Layer "<<extrapolateTo_<<" hit found";
	  // Here we will drop the extrapolated hits if there is a hit and use that hit
	  size_t imatch=0;
	  float glmatch=9999.;
	  for (size_t iexp=0; iexp<expTrajMeasurements.size(); iexp++) {
	    ModuleData mod=
	      getModuleData(expTrajMeasurements[iexp].recHit()->geographicalId().rawId(),
			    "offline");
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
			    "offline");
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
					       "offline");
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
	  std::vector< BarrelDetLayer*> pxbLayers = 
	    measurementTrackerHandle->geometricSearchTracker()->pixelBarrelLayers();
	  const DetLayer* pxb1 = pxbLayers[extrapolateTo_-1];
	  const MeasurementEstimator* estimator = est.product();
	  const LayerMeasurements* theLayerMeasurements = 
	    new LayerMeasurements(&*measurementTrackerHandle);
	  const TrajectoryStateOnSurface tsosPXB2 = itTraj->updatedState();
	  expTrajMeasurements = 
	    theLayerMeasurements->measurements(*pxb1, tsosPXB2, *thePropagator, *estimator);
	  delete theLayerMeasurements;

	  if ( !expTrajMeasurements.empty()) {
	    if (JKDEBUG) {
	      std::cout <<"size of TM from propagation = "<<expTrajMeasurements.size()<< std::endl;
	    }

	    TrajectoryMeasurement pxb1TM(expTrajMeasurements.front());
	    ConstReferenceCountingPointer<TransientTrackingRecHit> pxb1Hit;
	    pxb1Hit = pxb1TM.recHit();
	    
	    if (pxb1Hit->geographicalId().rawId()!=0) {
	      ModuleData pxbMod=getModuleData(pxb1Hit->geographicalId().rawId(), "offline");
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
      
	meas.mod=getModuleData(recHit->geographicalId().rawId(), "offline");
      
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
	
	meas.mod_on=getModuleData(recHit->geographicalId().rawId(), "online");
      
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
	
	meas.mod=getModuleData(recHit->geographicalId().rawId(), "offline");
	
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
	
	meas.mod_on=getModuleData(recHit->geographicalId().rawId(), "online");
	
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
	  if (trajmeas_[itrk][i].mod.ladder!=trajmeas_[jtrk][j].mod.layer) continue;
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
  for (size_t i=0; i<tracks_.size(); i++) {
    trackTree_->SetBranchAddress("event", &evt_);
    trackTree_->SetBranchAddress("track", &tracks_[i]);
    trackTree_->Fill();
  }
  #endif

  #ifdef COMPLETE
  for (size_t i=0; i<clusts_.size(); i++) {
    clustTree_->SetBranchAddress("event", &evt_);
    clustTree_->SetBranchAddress("clust", &clusts_[i]);
    clustTree_->SetBranchAddress("module", &clusts_[i].mod);
    clustTree_->SetBranchAddress("module_on", &clusts_[i].mod_on);
    clustTree_->Fill();
  }

  for (size_t i=0; i<digis_.size(); i++) {
    digiTree_->SetBranchAddress("event", &evt_);
    digiTree_->SetBranchAddress("digi", &digis_[i]);
    digiTree_->SetBranchAddress("module", &digis_[i].mod);
    digiTree_->SetBranchAddress("module_on", &digis_[i].mod_on);
    digiTree_->Fill();
  }
  #endif

  for (size_t itrk=0; itrk<trajmeas_.size(); itrk++) {
    for (size_t i=0; i<trajmeas_[itrk].size(); i++) {
      #ifndef COMPLETE
      if (trajmeas_[itrk][i].trk.strip<minNStripHits_) continue;
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
      //

      trajTree_->SetBranchAddress("event", &evt_);
      trajTree_->SetBranchAddress("traj", &trajmeas_[itrk][i]);
      #ifdef COMPLETE
      trajTree_->SetBranchAddress("module", &trajmeas_[itrk][i].mod);
      #endif
      trajTree_->SetBranchAddress("module_on", &trajmeas_[itrk][i].mod_on);
      trajTree_->SetBranchAddress("clust", &trajmeas_[itrk][i].clu);
      trajTree_->SetBranchAddress("track", &trajmeas_[itrk][i].trk);
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

  // Exceptions:
  // One full module on layer 1 is out. Need to fix classification here, because
  // the trajectory propagation does not include this
  if (meas.mod_on.det==0&&meas.mod_on.layer==1&&meas.mod_on.ladder==9&&meas.mod_on.module==2) {
    if (JKDEBUG) std::cout<<" (reclassified as inactive by hand)";
    meas.validhit = 0;
    meas.missing = 0;
    meas.inactive = 1;
    meas.badhit = 0;
  }

  // One full module on layer 2, because it is not in the database yet
  if (meas.mod_on.det==0&&meas.mod_on.layer==2&&meas.mod_on.ladder==-8&&meas.mod_on.module==-4) {
    if (JKDEBUG) std::cout<<" (reclassified as inactive by hand)";
    meas.validhit = 0;
    meas.missing = 0;
    meas.inactive = 1;
    meas.badhit = 0;
  }
  
  
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
  iEvent.getByLabel("siPixelClusters", clusterCollectionHandle);
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
	PixelClusterParameterEstimator::LocalValues params=cpe.localParameters(*itCluster,*pixdet);
	//gp = surface->toGlobal(params.first);
	lp = params.first;
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
TimingStudy::getModuleData(uint32_t rawId, std::string scheme) {

  ModuleData offline;
  ModuleData online;

  offline.rawid = online.rawid = rawId;
  int subDetId = DetId(offline.rawid).subdetId();
  
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
