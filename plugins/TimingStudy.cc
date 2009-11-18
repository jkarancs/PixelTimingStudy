
//
//
//
//

//
//
// ----------------------------------------------------------------------------------------

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
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "MagneticField/Engine/interface/MagneticField.h"

// SimDataFormats
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"

// For ROOT
#include <TROOT.h>
#include <TTree.h>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

// STD
#include <memory>
#include <string>
#include <iostream>
#include <vector>

using namespace std;
using namespace edm;
using namespace reco;


TimingStudy::TimingStudy(edm::ParameterSet const& iConfig) : 
  iConfig_(iConfig)
{
  trackTree_=NULL;
  clustTree_=NULL;
  trajTree_=NULL;
  digiTree_=NULL;
}


TimingStudy::~TimingStudy() { }  


void TimingStudy::endJob() 
{

}


void TimingStudy::beginJob(const edm::EventSetup& es)
{
  es.get<TrackerDigiGeometryRecord>().get(tkGeom_);
  es.get<IdealMagneticFieldRecord>().get(magneticField_);

  edm::Service<TFileService> fs;


  trackTree_ = fs->make<TTree>("trackTree", "The track in the event");
  trackTree_->Branch("event", &evt_, evt_.list.data());
  trackTree_->Branch("track", &track_, track_.list.data());


  Cluster clust;
  clustTree_ = fs->make<TTree>("clustTree", "Pixel clusters");
  clustTree_->Branch("event", &evt_, evt_.list.data());
  clustTree_->Branch("clust", &clust, clust.list.data());
  clustTree_->Branch("module", &clust.mod, clust.mod.list.data());
  clustTree_->Branch("module_on", &clust.mod_on, clust.mod_on.list.data());


  TrajMeasurement trajmeas;
  trajTree_ = fs->make<TTree>("trajTree", "Trajectory measurements in the Pixel");
  trajTree_->Branch("event", &evt_, evt_.list.data());
  trajTree_->Branch("traj", &trajmeas, trajmeas.list.data());
  trajTree_->Branch("module", &trajmeas.mod, trajmeas.mod.list.data());
  trajTree_->Branch("module_on", &trajmeas.mod_on, trajmeas.mod_on.list.data());
  trajTree_->Branch("clust", &trajmeas.clu, trajmeas.clu.list.data());
  trajTree_->Branch("track", &trajmeas.trk, trajmeas.trk.list.data());


  Digi digi;
  digiTree_ = fs->make<TTree>("digiTree", "Pixel digis");
  digiTree_->Branch("event", &evt_, evt_.list.data());
  digiTree_->Branch("digi", &digi, digi.list.data());
  digiTree_->Branch("module", &digi.mod, digi.mod.list.data());
  digiTree_->Branch("module_on", &digi.mod_on, digi.mod_on.list.data());


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
      std::cout << port << " " << mod << " " << aoh << std::endl;
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

  init_all(); // clear all containers



  //
  // Read track info
  //
  edm::Handle<TrajTrackAssociationCollection> trajTrackCollectionHandle;
  iEvent.getByLabel(iConfig_.getParameter<std::string>("trajectoryInput"),
		    trajTrackCollectionHandle);


  //
  // Consider events with exactly one track
  //
  std::cout << "\n\nRun " << evt_.run << " Event " << evt_.evt;
  std::cout << " Number of tracks =" << trajTrackCollectionHandle->size() << std::endl;
  if (trajTrackCollectionHandle->size()!=1) return;

  // -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  -  - >



  //
  // Read event info
  //
  evt_.run=iEvent.id().run();
  evt_.evt=iEvent.id().event();
  evt_.wbc=wbc[iEvent.id().run()];
  evt_.delay=globaldelay[iEvent.id().run()];
  evt_.bx=iEvent.bunchCrossing();
  evt_.orb=iEvent.orbitNumber();

  //
  // Fill event with muon time
  //
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

  edm::ESHandle<MagneticField> magFieldHandle;
  iSetup.get<IdealMagneticFieldRecord>().get(magFieldHandle);
  const MagneticField& magField = *magFieldHandle;

  float mag_x = magField.inTesla(GlobalPoint(0,0,0)).x();
  float mag_y = magField.inTesla(GlobalPoint(0,0,0)).y();
  float mag_z = magField.inTesla(GlobalPoint(0,0,0)).z();
 
  evt_.field=sqrt(mag_x*mag_x+mag_y*mag_y+mag_z*mag_z);



  //
  // Read digi information
  //

  edm::Handle<edm::DetSetVector<PixelDigi> >  digiCollectionHandle;
  iEvent.getByLabel("siPixelDigis", digiCollectionHandle);

  const edm::DetSetVector<PixelDigi>& digiCollection = *digiCollectionHandle;
  edm::DetSetVector<PixelDigi>::const_iterator itDigiSet = digiCollection.begin();

  for (; itDigiSet!=digiCollection.end(); itDigiSet++) {

    DetId detId(itDigiSet->detId());
    unsigned int subDetId=detId.subdetId();

    // Take only pixel digis
    if (subDetId!=PixelSubdetector::PixelBarrel &&
	subDetId!=PixelSubdetector::PixelEndcap) {
      std::cout << "ERROR: not a pixel digi!!!" << std::endl; // should not happen
      continue;
    }

    ModuleData module=getModuleData(detId.rawId());

    std::cout << "Run " << evt_.run << " Event " << evt_.evt << " number of digis on ";
    if (module.det==0) {
      std::cout << " PixelBarrel layer " << module.layer << " ladder " << module.ladder;
      std::cout << " module " << module.module << " ";
    } else {
      std::cout << " PixelForward disk " << module.disk << " blade " << module.blade;
      std::cout << " panel " << module.panel << " module " << module.module << " ";
    }
    ModuleData module_on=getModuleData(detId.rawId(), "online");
    std::cout << ": " << itDigiSet->size() << std::endl;

    edm::DetSet<PixelDigi>::const_iterator itDigi=itDigiSet->begin();

    for(; itDigi!=itDigiSet->end(); ++itDigi) {
      Digi digi;
      digi.i=itDigi-itDigiSet->begin();
      digi.row=itDigi->row();
      digi.col=itDigi->column();
      digi.adc=itDigi->adc();
      std::cout<<"\t#"<<digi.i<<" adc "<<digi.adc<<" at ("<<digi.col<<", "<<digi.row<<")";
      std::cout<<std::endl;
    }
  } // loop on cluster sets



  //
  // Read cluster information
  //
  edm::Handle<edmNew::DetSetVector<SiPixelCluster> > clusterCollectionHandle;
  iEvent.getByLabel("siPixelClusters", clusterCollectionHandle);

  const edmNew::DetSetVector<SiPixelCluster>& clusterCollection = *clusterCollectionHandle;
  edmNew::DetSetVector<SiPixelCluster>::const_iterator itClusterSet = 
    clusterCollection.begin();

  for (; itClusterSet!=clusterCollection.end(); itClusterSet++) {

    DetId detId(itClusterSet->id());
    unsigned int subDetId=detId.subdetId();

    // Take only pixel clusters
    if (subDetId!=PixelSubdetector::PixelBarrel &&
	subDetId!=PixelSubdetector::PixelEndcap) {
      std::cout << "ERROR: not a pixel cluster!!!" << std::endl; // should not happen
      continue;
    }

    ModuleData module=getModuleData(detId.rawId());

    std::cout << "Run " << evt_.run << " Event " << evt_.evt << " number of clusters on ";
    if (module.det==0) {
      std::cout << " PixelBarrel layer " << module.layer << " ladder " << module.ladder;
      std::cout << " module " << module.module << " ";
    } else {
      std::cout << " PixelForward disk " << module.disk << " blade " << module.blade;
      std::cout << " panel " << module.panel << " module " << module.module << " ";
    }
    ModuleData module_on=getModuleData(detId.rawId(), "online");
    std::cout << ": " << itClusterSet->size() << std::endl;

    edmNew::DetSet<SiPixelCluster>::const_iterator itCluster=itClusterSet->begin();

    for(; itCluster!=itClusterSet->end(); ++itCluster) {
      Cluster clust;
      clust.i=itCluster-itClusterSet->begin();
      clust.charge=itCluster->charge()/1000.0;
      clust.size=itCluster->size();
      for (int i=0; i<itCluster->size() && i<1000; i++) {
	clust.adc[i]=float(itCluster->pixelADC()[i])/1000.0;
      }

      clust.mod=module;
      clust.mod_on=module_on;
      clusts_.push_back(clust);
      std::cout<<"\t#"<<clust.i<<" charge "<<clust.charge<<" size "<<clust.size<<std::endl;
    }
  } // loop on cluster sets



  //
  // Process tracks
  //

  TrajTrackAssociationCollection::const_iterator itTrajTrack;
  itTrajTrack = trajTrackCollectionHandle->begin();

  const Trajectory& traj  = *itTrajTrack->key;
  const Track&      track = *itTrajTrack->val;


  //
  // Read track info - USED BY TrajMeasurement!!!
  //
  track_.pix=0;
  track_.strip=0;
  track_.pixhit[0]=0;
  track_.pixhit[1]=0;
  track_.validpixhit[0]=0;
  track_.validpixhit[1]=0;
  track_.ndof=track.ndof();
  track_.chi2=track.chi2();
  track_.d0=track.d0();
  track_.dz=track.dz();
  track_.pt=track.pt();



  //
  // Loop along trajectory measurements
  //

  std::vector<TrajectoryMeasurement> trajMeasurements = traj.measurements();
  TrajectoryStateCombiner trajStateComb;

  std::cout << "Run " << evt_.run << " Event " << evt_.evt;
  std::cout << " number of measurements on track "<< trajMeasurements.size() << std::endl;

  //int nTopHits=0; int nBottomHits=0; int nTopValidHits=0; int nBottomValidHits=0;
  //int nValidStripHits=0;
  std::vector<TrajectoryMeasurement>::const_iterator itTraj;
  
  for(itTraj=trajMeasurements.begin(); itTraj!=trajMeasurements.end(); ++itTraj) {

    TrajMeasurement meas;
    meas.i=itTraj-trajMeasurements.begin();

    std::cout << "Run " << evt_.run << " Event " << evt_.evt;
    std::cout << " TrajMeas #" << meas.i;

    TransientTrackingRecHit::ConstRecHitPointer recHit = itTraj->recHit();
    if (recHit->geographicalId().det()!=DetId::Tracker) continue;

    meas.mod=getModuleData(recHit->geographicalId().rawId(), "offline");

    uint subDetId = recHit->geographicalId().subdetId();

    if (subDetId == PixelSubdetector::PixelBarrel) {
      std::cout << " PixelBarrel layer " << meas.mod.layer;
      std::cout << " ladder " << meas.mod.ladder << " module " << meas.mod.module;
      if (recHit->isValid()) track_.pix++;
    } else if (subDetId == PixelSubdetector::PixelEndcap) {
      std::cout<<" PixelForward disk "<<meas.mod.disk<<" blade "<< meas.mod.blade;
      std::cout << " panel " << meas.mod.panel << " module " << meas.mod.module;
      if (recHit->isValid()) track_.pix++;
    } else if (subDetId == StripSubdetector::TIB) { 
      std::cout << " TIB layer" << TIBDetId(meas.mod.rawid).layer();
      if (recHit->isValid()) track_.strip++;
    } else if (subDetId == StripSubdetector::TOB) {
      std::cout << " TOB layer" << TOBDetId(meas.mod.rawid).layer();
      if (recHit->isValid()) track_.strip++;
    } else if (subDetId == StripSubdetector::TID) { 
      std::cout << " TID wheel" << TIDDetId(meas.mod.rawid).wheel();
      if (recHit->isValid()) track_.strip++;
    } else if (subDetId == StripSubdetector::TEC) {
      std::cout << " TEC wheel" << TECDetId(meas.mod.rawid).wheel();
      if (recHit->isValid()) track_.strip++;
    }

    meas.mod_on=getModuleData(recHit->geographicalId().rawId(), "online");

    if (!itTraj->updatedState().isValid()) std::cout<<", updatedState is invalid";
    meas.validhit=1;
    if (!recHit->isValid()) {
      meas.validhit=0;
      std::cout << ", RecHit is invalid";
    }
    std::cout << std::endl;

    meas.missing= (recHit->getType()==TrackingRecHit::missing) ? 1 : 0;
    meas.inactive= (recHit->getType()==TrackingRecHit::inactive) ? 1 : 0;
    meas.badhit= (recHit->getType()==TrackingRecHit::bad) ? 1 : 0;

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
    meas.telescope=0;
    meas.telescope_valid=0;

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

	LocalTrajectoryParameters predTrajParam= predTrajState.localParameters();
	LocalVector dir = predTrajParam.momentum()/predTrajParam.momentum().mag();

	meas.alpha = atan2(dir.z(), dir.x());
	meas.beta = atan2(dir.z(), dir.y());	
	meas.norm_charge = meas.clu.charge*
	  sqrt(1.0/(1.0/pow(tan(meas.alpha),2)+1.0/pow(tan(meas.beta),2)+1.0));
      }
    }

    meas.trk=track_;
    trajmeas_.push_back(meas);
  } // loop on trajectory measurements

  
  //
  // Make telescope cut for Trajectory Measurements
  //
  
  if (track_.pixhit[0]>0 && track_.pixhit[1]>0) {
    for (size_t i=0; i<trajmeas_.size(); i++) {
      if (trajmeas_[i].gly<0 && track_.pixhit[1]>1) {
	trajmeas_[i].telescope=1;
	std::cout << "Layer " << trajmeas_[i].mod.layer << " module " \
		  << trajmeas_[i].mod.module << "(y=" << trajmeas_[i].gly \
		  << ") passed telescope" << std::endl;
      }
      if (trajmeas_[i].gly>0 && track_.pixhit[0]>1) {
	trajmeas_[i].telescope=1;

	std::cout << "Layer " << trajmeas_[i].mod.layer << " module " \
		  << trajmeas_[i].mod.module << "(y=" << trajmeas_[i].gly \
		  << ") passed telescope" << std::endl;
      }
    }
  }

  if (track_.validpixhit[0]>0 && track_.validpixhit[1]>0) {
    for (size_t i=0; i<trajmeas_.size(); i++) {
      if (trajmeas_[i].gly<0 && (track_.validpixhit[1]>1 || !trajmeas_[i].validhit)) {
	trajmeas_[i].telescope_valid=1;
	std::cout << "Layer " << trajmeas_[i].mod.layer << " module " \
		  << trajmeas_[i].mod.module << "(y=" << trajmeas_[i].gly \
		  << ") passed telescope_valid" << std::endl;
      }
      if (trajmeas_[i].gly>0 && (track_.validpixhit[0]>1 || !trajmeas_[i].validhit)) {
	trajmeas_[i].telescope_valid=1;
	std::cout << "Layer " << trajmeas_[i].mod.layer << " module " \
		  << trajmeas_[i].mod.module << "(y=" << trajmeas_[i].gly \
		  << ") passed telescope_valid" << std::endl;
      }
    }
  }


  
  //
  // Fill the trees
  //

  trackTree_->Fill();

  for (size_t i=0; i<clusts_.size(); i++) {
    clustTree_->SetBranchAddress("event", &evt_);
    clustTree_->SetBranchAddress("clust", &clusts_[i]);
    clustTree_->SetBranchAddress("module", &clusts_[i].mod);
    clustTree_->SetBranchAddress("module_on", &clusts_[i].mod_on);
    clustTree_->Fill();
  }

  for (size_t i=0; i<digis_.size(); i++) {
    digiTree_->SetBranchAddress("event", &evt_);
    digiTree_->SetBranchAddress("clust", &digis_[i]);
    digiTree_->SetBranchAddress("module", &digis_[i].mod);
    digiTree_->SetBranchAddress("module_on", &digis_[i].mod_on);
    digiTree_->Fill();
  }

  for (size_t i=0; i<trajmeas_.size(); i++) {
    trajTree_->SetBranchAddress("event", &evt_);
    trajTree_->SetBranchAddress("traj", &trajmeas_[i]);
    trajTree_->SetBranchAddress("module", &trajmeas_[i].mod);
    trajTree_->SetBranchAddress("module_on", &trajmeas_[i].mod_on);
    trajTree_->SetBranchAddress("clust", &trajmeas_[i].clu);
    trajTree_->SetBranchAddress("track", &track_);
    trajTree_->Fill();
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
	  std::cout<<" (" << it->second << ": " << online.sec << ", " << online.prt << ")";
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
      std::cout<<" (" << it->second << ")";
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
DEFINE_FWK_MODULE(TimingStudy);
