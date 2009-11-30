
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <map>
#include <cmath>

#ifndef __CINT__
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TROOT.h"
#include "TF1.h"

#include "DataStructures.h"
#include "debDataMaker/interface/Histogram.hh"
#include "debDataMaker/interface/Plot.hh"


///////////////////////////////////////////////////////////////////////////////////////////////////
//
// Delays
//

std::pair<int,int> getStepDelayCosmics(int globaldelay25, int run) {
  int step=NOVAL_I;
  int delay=NOVAL_I;
  if (run<111140) { delay=0; step=2; }
  if (run>112080) {
    delay= (globaldelay25%12) ? globaldelay25/2-25 : globaldelay25/2;
    step= delay/6+2;
  }
  // just a test:
  //  if (step==0) step=3;
  //else if (step==3) step=0;
  std::pair<int,int> ret(step,delay);
  return ret;
}



std::pair<int,int> getStepDelayCollision(int orbitnumber, int run=9999) {
  int step=0;
  int delay=NOVAL_I;
  //  if (run==9999) {
    if (orbitnumber>=0 && orbitnumber<1e8)        { delay=-12; step=0; }
    else if (orbitnumber>=1e8 && orbitnumber<2e8) { delay=-6; step=1; }
    else if (orbitnumber>=2e8 && orbitnumber<3e8) { delay=0; step=2; }
    else if (orbitnumber>=3e8 && orbitnumber<4e8) { delay=6; step=3; }
    else if (orbitnumber>=4e8 && orbitnumber<5e8) { delay=12; step=4; }
    else if (orbitnumber>=5e8 && orbitnumber<6e8) { delay=15; step=5; }
    else if (orbitnumber>=6e8)                    { delay=18; step=6; }
    //  }
  std::pair<int,int> ret(step,delay);
  return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////



///////////////////////////////////////////////////////////////////////////////////////////////////
//
// RecHit based plots
//

int doRecHitPlots(TChain *trajTree, int data) { // data 0:cosmics else:collision

  if (trajTree->GetEntries()==0) {
    std::cout<<"No hit found in trajTree. Returning.\n";
    return 0;
  }

  std::string plotType="RecHits";
  std::map<int,int> delays;
  float min_delay, max_delay;
  int n_delay=0;
  

  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Preprocessing cycle
  //
  
  // Set the tree for reading
  EventData evt;
  TrajMeasurement trajmeas;
  trajTree->SetBranchAddress("event", &evt);
  trajTree->SetBranchAddress("traj", &trajmeas);
  trajTree->SetBranchAddress("module", &trajmeas.mod);
  trajTree->SetBranchAddress("module_on", &trajmeas.mod_on);
  trajTree->SetBranchAddress("clust", &trajmeas.clu);
  trajTree->SetBranchAddress("track", &trajmeas.trk);

  std::cout<<"Reading trajTree of entries: "<<trajTree->GetEntries()<<std::endl;

  std::string prevFileName="";
  std::vector<float> tempDelayBins;

  for (Long64_t i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);
    std::string fname=trajTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.orb, evt.run);
    }
    if (stepDelay.first==NOVAL_I || stepDelay.second==NOVAL_I) continue;
    if (delays.find(stepDelay.first)==delays.end()) {
      std::cout<<"Step "<<stepDelay.first<<" Delay "<<stepDelay.second<<std::endl;
      delays.insert(stepDelay);
      tempDelayBins.push_back(stepDelay.second);
    }
  }

  std::sort(tempDelayBins.begin(), tempDelayBins.end(), std::less<float>());
  float min_sep=99999;
  for (int i=1; i<tempDelayBins.size(); i++) {
    if (min_sep>tempDelayBins[i]-tempDelayBins[i-1]) min_sep=tempDelayBins[i]-tempDelayBins[i-1];
  }
  if (tempDelayBins.size()>0) {
    if (tempDelayBins.size()==1) min_sep=1;
    min_delay=tempDelayBins[0]-min_sep/2.;
    max_delay=tempDelayBins[tempDelayBins.size()-1]+min_sep/2.;
    n_delay=(max_delay-min_delay)/min_sep;
  };


  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Booking histograms
  //

  // FOLLOWS SHELL NUMBERING IN DATASTRUCTURES.H:
  std::vector<std::string> partitions;
  partitions.push_back("BpI");
  partitions.push_back("BpO");
  partitions.push_back("BmI");
  partitions.push_back("BmO");

  std::cout<< "Making histos\n";
  // 1. Hit Efficiency vs DT time
  deb::Plot<TH1F> hitEffVsDTtimeBarrel(600, 600, "%s_hitEffVsDTtimeBarrel\nTDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    hitEffVsDTtimeBarrel.add(35,-35.,35., "[%d]!delay %d ns;Hit efficiency in the Barrel pixel at"
			     " %d ns;DT time [ns];Efficiency", it->first, it->second, it->second);
  }
  hitEffVsDTtimeBarrel.efficiency();


  // 2. Hit Efficiency vs Delay
  deb::Plot<TH1F> hitEffVsDelay(600, 600, "%s_hitEffVsDelay\nTDR", plotType.data());
  hitEffVsDelay.add(n_delay, min_delay, max_delay, "BPix;Hit efficiency in the Barrel"
		    " pixel;Delay [ns];Efficiency");
  hitEffVsDelay.add(n_delay, min_delay, max_delay, "FPix;Hit efficiency in the Forward"
		    " pixel;Delay [ns];Efficiency");
  hitEffVsDelay.efficiency();


  deb::Plot<TH1F> hitEffVsDelayBPixShell(600,600,"%s_hitEffVsDelayBPixShell\nTDR",plotType.data());
  deb::Plot<TH1F> hitEffVsDelayFPixShell(600,600,"%s_hitEffVsDelayFPixShell\nTDR",plotType.data());
  for (size_t i=0; i<partitions.size(); i++) {
    hitEffVsDelayBPixShell.add(n_delay, min_delay, max_delay, "%s;Hit efficiency in the Barrel"
			       " pixel;Delay [ns];Efficiency", partitions[i].data());
    hitEffVsDelayFPixShell.add(n_delay, min_delay, max_delay, "%s;Hit efficiency in the Forward"
			       " pixel;Delay [ns];Efficiency", partitions[i].data());
  }
  hitEffVsDelayBPixShell.efficiency();
  hitEffVsDelayFPixShell.efficiency();


  std::vector<deb::Plot<TH1F> > hitEffVsDelayBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t prt=0; prt<2; prt++) {
      deb::Plot<TH1F> p(600,600,"%s_hitEffVsDelayBPix_%s_PRT%d\nTDR",
			plotType.data(), partitions[i].data(), prt+1);
      for (size_t sec=0; sec<8; sec++) {
	p.add(n_delay, min_delay, max_delay, "SEC%d;Hit efficiency in %s PRT%d SEC%d;"
	      "Delay [ns];Efficiency", sec+1, partitions[i].data(), prt+1, sec+1);
      }
      p.efficiency();
      hitEffVsDelayBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F> > hitEffVsDelayFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F> p(600,600,"%s_hitEffVsDelayFPix_%s\nTDR",
		      plotType.data(), partitions[i].data());
    for (size_t disk=0; disk<2; disk++) {
      for (size_t prt=0; prt<4; prt++) {
	p.add(n_delay, min_delay, max_delay, "Disk%d_PRT%d;Hit efficiency in %s Disk%d PRT%d;"
	      "Delay [ns];Efficiency", disk+1, prt+1, partitions[i].data(), disk+1, prt+1);
      }
    }
    p.efficiency();
    hitEffVsDelayFPix.push_back(p);
  }

 
  // 3. Cluster Charge
 
  deb::Plot<TH1F> clusCharge(600, 600, "%s_clusCharge\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    clusCharge.add(140, 0., 140., "BPix_[%d]!delay %d ns;RecHit charge in the Barrel"
		   " pixel;Charge [ke]", it->first, it->second);
    clusCharge.add(140, 0., 140., "FPix_[%d]!delay %d ns;RecHit charge in the Forward"
		   " pixel;Charge [ke]", it->first, it->second);
  }
  clusCharge.efficiency();


  deb::Plot<TH1F> clusChargeBPixShell(600,600,"%s_clusChargeBPixShell\nTDR",plotType.data());
  deb::Plot<TH1F> clusChargeFPixShell(600,600,"%s_clusChargeFPixShell\nTDR",plotType.data());
  for (size_t i=0; i<partitions.size(); i++) {
    for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
      clusChargeBPixShell.add(140, 0., 140., "%s_[%d]!delay %d ns;RecHit charge in the Barrel"
			      " pixel;Charge [ke]", partitions[i].data(), it->first, it->second);
      clusChargeFPixShell.add(140, 0., 140., "%s_[%d]!delay %d ns;RecHit charge in the Forward"
			      " pixel;Charge [ke]", partitions[i].data(), it->first, it->second);
    }
  }
  clusChargeBPixShell.efficiency();
  clusChargeFPixShell.efficiency();


  std::vector<deb::Plot<TH1F> > clusChargeBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t prt=0; prt<2; prt++) {
      deb::Plot<TH1F> p(600,600,"%s_clusChargeBPix_%s_PRT%d\nTDR",
			plotType.data(), partitions[i].data(), prt+1);
      for (size_t sec=0; sec<8; sec++) {
	for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
	  p.add(140, 0., 140., "SEC%d_[%d]!delay %d ns;Hit efficiency in %s PRT%d SEC%d;"
		"Delay [ns];Efficiency", sec+1,  it->first, it->second, 
		partitions[i].data(), prt+1, sec+1);
	}
      }
      p.efficiency();
      clusChargeBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F> > clusChargeFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F> p(600,600,"%s_clusChargeFPix_%s\nTDR",
		      plotType.data(), partitions[i].data());
    for (size_t disk=0; disk<2; disk++) {
      for (size_t prt=0; prt<4; prt++) {
	for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
	  p.add(140, 0., 140., "Disk%d_PRT%d_[%d]!delay %d ns;Hit efficiency in %s Disk%d PRT%d;"
		"Delay [ns];Efficiency", disk+1, prt+1, it->first, it->second, 
		partitions[i].data(), disk+1, prt+1);
	}
      }
    }
    p.efficiency();
    clusChargeFPix.push_back(p);
  }



  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //
  std::cout<< "Filling histos\n";
  for (long i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);

    // Delay step and delay
    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.orb, evt.run);
    }
    int step=stepDelay.first;
    int delay=stepDelay.second;
    if (step==NOVAL_I || delay==NOVAL_I) continue;
    std::ostringstream ss;
    ss << "_[" << step << "]";
    std::string sstep=ss.str();

    // Detector coordinate
    std::string detector= (trajmeas.mod_on.det==0) ? "BPix" : "FPix";
    std::string shell = trajmeas.mod_on.shell();
    int group=0;
    std::string rog;
    std::ostringstream srog;
    if (trajmeas.mod_on.det==0) {
      group = trajmeas.mod_on.shell_num()*2+trajmeas.mod_on.prt-1;
      srog<<"SEC"<<trajmeas.mod_on.sec;
    } else if (trajmeas.mod_on.det==1) {
      group = trajmeas.mod_on.shell_num();
      srog<<"Disk"<<abs(trajmeas.mod_on.disk)<<"_PRT"<<trajmeas.mod_on.prt;
    }
    rog=srog.str();


    // 1. Hit Efficiency vs DT time
    //    if (trajmeas.telescope && trajmeas.mod_on.det==0 &&
    if (trajmeas.mod_on.det==0 &&
	(trajmeas.validhit || trajmeas.missing) ) {
      if (step!=NOVAL_I) {
	hitEffVsDTtimeBarrel(step).den()->Fill(evt.tmuon);
	if (trajmeas.validhit) hitEffVsDTtimeBarrel(step).num()->Fill(evt.tmuon);
      }
    }
    // ---------------------------------------


    // 2. Hit Efficiency vs delay
    //    if (trajmeas.telescope && (trajmeas.validhit || trajmeas.missing) ) {
    if ( (trajmeas.validhit || trajmeas.missing) ) {
      
      if (trajmeas.mod_on.det==0) {
	hitEffVsDelay[detector].den()->Fill(delay);
	hitEffVsDelayBPixShell[shell].den()->Fill(delay);
	hitEffVsDelayBPix[group][rog].den()->Fill(delay);
	if (trajmeas.validhit) {
	  hitEffVsDelay[detector].num()->Fill(delay);
	  hitEffVsDelayBPixShell[shell].num()->Fill(delay);
	  hitEffVsDelayBPix[group][rog].num()->Fill(delay);
	}

      } else {
	hitEffVsDelay[detector].den()->Fill(delay);
	hitEffVsDelayFPixShell[shell].den()->Fill(delay);
	hitEffVsDelayFPix[group][rog].den()->Fill(delay);
	if (trajmeas.validhit) {
	  hitEffVsDelay[detector].num()->Fill(delay);
	  hitEffVsDelayFPixShell[shell].num()->Fill(delay);
	  hitEffVsDelayFPix[group][rog].num()->Fill(delay);
	}
      }

    }
    // ---------------------------------------


    // 3. Cluster charge
    if (trajmeas.validhit) {
      clusCharge[detector+sstep].Fill(trajmeas.clu.charge);
      if (trajmeas.mod_on.det==0) {
	clusChargeBPixShell[shell+sstep].Fill(trajmeas.clu.charge);
	clusChargeBPix[group][rog+sstep].Fill(trajmeas.clu.charge);
      } else {
	clusChargeFPixShell[shell+sstep].Fill(trajmeas.clu.charge);
	clusChargeFPix[group][rog+sstep].Fill(trajmeas.clu.charge);
      }
    }

  }



  // ----------------------------------------------------------------------------------------------
  // Post-processing and drawing histograms
  //
  std::cout<< "Drawing histos\n";
  // 1. Hit Efficiency vs DT time
  hitEffVsDTtimeBarrel.efficiency(".*");
  hitEffVsDTtimeBarrel.scaleMaximumTo(".*.den", 1);
  hitEffVsDTtimeBarrel.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDTtimeBarrel.setColor(".*.den", "LineMarker", 1, 1);
  hitEffVsDTtimeBarrel.setAxisRange(".*", 0., 1.1, "Y");
  hitEffVsDTtimeBarrel.Draw(".*!;overlayHIST;Hit efficiency in BPix\n"
			    ".*.den!;overlayHISTL;DT time distribution of muons;DT time [ns]");
  hitEffVsDTtimeBarrel.Write();


  // 2. Hit Efficiency vs Delay
  hitEffVsDelay.efficiency();
  //hitEffVsDelay.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelay.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelay.Draw("FPix!& BPix!;overlayPE");
  hitEffVsDelay.Write();

  hitEffVsDelayBPixShell.efficiency();
  //hitEffVsDelayBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelayBPixShell.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelayBPixShell.Draw("BpO!BpO& BpI!BpI& BmO!BmO& BmI!BmI;PE",2);
  hitEffVsDelayBPixShell.Write();

  hitEffVsDelayFPixShell.efficiency();
  //hitEffVsDelayFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelayFPixShell.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelayFPixShell.Draw("BpO!BpO& BpI!BpI& BmO!BmO& BmI!BmI;PE",2);
  hitEffVsDelayFPixShell.Write();

  for (size_t i=0; i<hitEffVsDelayBPix.size(); i++) {
    hitEffVsDelayBPix[i].efficiency();
    //hitEffVsDelayBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    hitEffVsDelayBPix[i].setColor(".*", "LineMarker", 1, 1);
    hitEffVsDelayBPix[i].Draw("SEC[1-8]!;PE",3);
    hitEffVsDelayBPix[i].Write();
  }

  for (size_t i=0; i<hitEffVsDelayFPix.size(); i++) {
    hitEffVsDelayFPix[i].efficiency();
    //hitEffVsDelayFPix[i]->setAxisRange(".*", 0, 1.0, "Y");
    hitEffVsDelayFPix[i].setColor(".*", "LineMarker", 1, 1);
    hitEffVsDelayFPix[i].Draw("Disk[1,2]_PRT[1-4]!;PE",3);
    hitEffVsDelayFPix[i].Write();
  }


  // 3. Cluster charge
  clusCharge.setColor("BPix_.*", "LineMarker", 1, 1);
  clusCharge.setColor("FPix_.*", "LineMarker", 1, 1);
  clusCharge.scaleAreaTo(".*", 1.);
  clusCharge.Draw("BPix_.*!;overlayHISTL\nFPix_.*!;overlayHISTL");
  clusCharge.Write();

  //clusChargeBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  clusChargeBPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  clusChargeBPixShell.scaleAreaTo(".*", 1.);
  clusChargeBPixShell.Draw("BpO_.*!;overlayHISTL\nBpI_.*!;overlayHISTL\n"
			   "BmO_.*!;overlayHISTL\nBmI_.*!;overlayHISTL\n", 2);
  clusChargeBPixShell.Write();


  //clusChargeFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  clusChargeFPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  clusChargeFPixShell.scaleAreaTo(".*", 1.);
  clusChargeFPixShell.Draw("BpO_.*!;overlayHISTL\nBpI_.*!;overlayHISTL\n"
			   "BmO_.*!;overlayHISTL\nBmI_.*!;overlayHISTL\n", 2);
  clusChargeFPixShell.Write();


  for (size_t i=0; i<clusChargeBPix.size(); i++) {
    //clusChargeBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    for (int j=1; j<9; j++) {
      std::ostringstream color;
      color<<"SEC"<<j<<".*";
      clusChargeBPix[i].setColor(color.str(), "LineMarker", 1, 1);
    }
    clusChargeBPix[i].scaleAreaTo(".*", 1.);
    clusChargeBPix[i].Draw("SEC1_.*!;overlayHISTL\n"
			   "SEC2_.*!;overlayHISTL\n"
			   "SEC3_.*!;overlayHISTL\n"
			   "SEC4_.*!;overlayHISTL\n"
			   "SEC5_.*!;overlayHISTL\n"
			   "SEC6_.*!;overlayHISTL\n"
			   "SEC7_.*!;overlayHISTL\n"
			   "SEC8_.*!;overlayHISTL",3);
    clusChargeBPix[i].Write();
  }

  for (size_t i=0; i<clusChargeFPix.size(); i++) {
    //clusChargeFPix[i]->setAxisRange(".*", 0, 1.0, "Y");
    clusChargeFPix[i].setColor("Disk1_PRT1.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk1_PRT2.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk1_PRT3.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk1_PRT4.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk2_PRT1.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk2_PRT2.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk2_PRT3.*", "LineMarker", 1, 1);
    clusChargeFPix[i].setColor("Disk2_PRT4.*", "LineMarker", 1, 1);
    clusChargeFPix[i].scaleAreaTo(".*", 1.);
    clusChargeFPix[i].Draw("Disk1_PRT1_.*!;overlayHISTL\n"
			   "Disk1_PRT2_.*!;overlayHISTL\n"
			   "Disk1_PRT3_.*!;overlayHISTL\n"
			   "Disk1_PRT4_.*!;overlayHISTL\n"
			   "Disk2_PRT1_.*!;overlayHISTL\n"
			   "Disk2_PRT2_.*!;overlayHISTL\n"
			   "Disk2_PRT3_.*!;overlayHISTL\n"
			   "Disk2_PRT4_.*!;overlayHISTL",3);
    clusChargeFPix[i].Write();
  }
  
  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // The end
  //

  return 1;
}




//
// TrackTree based plots
//





////////////////////////////////////////////////////////////////////////////////
//
// ClustTree based plots
//


int doClusterPlots(TChain *clustTree, int data) {

  if (clustTree->GetEntries()==0) {
    std::cout<<"No hit found in trajTree. Returning.\n";
    return 0;
  }

  std::string plotType="Cluster";
  std::map<int,int> delays;
  float min_delay, max_delay;
  int n_delay=0;
  std::ostringstream drawAll;
  std::ostringstream drawAll_den;



  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Preprocessing cycle
  //
  
  // Set the tree for reading
  EventData evt;
  Cluster clust;
  clustTree->SetBranchAddress("event", &evt);
  clustTree->SetBranchAddress("clust", &clust);
  clustTree->SetBranchAddress("module", &clust.mod);
  clustTree->SetBranchAddress("module_on", &clust.mod_on);

  std::cout<<"Reading clustTree of entries: "<<clustTree->GetEntries()<<std::endl;

  std::string prevFileName="";
  std::vector<float> tempDelayBins;

  for (Long64_t i=0; i<clustTree->GetEntries(); i++) {
    clustTree->GetEntry(i);
    std::string fname=clustTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.orb, evt.run);
    }
    if (stepDelay.first==NOVAL_I || stepDelay.second==NOVAL_I) continue;
    if (delays.find(stepDelay.first)==delays.end()) {
      std::cout<<"Step "<<stepDelay.first<<" Delay "<<stepDelay.second<<std::endl;
      delays.insert(stepDelay);
      tempDelayBins.push_back(stepDelay.second);
    }
  }

  std::sort(tempDelayBins.begin(), tempDelayBins.end(), std::less<float>());
  float min_sep=99999;
  for (int i=1; i<tempDelayBins.size(); i++) {
    if (min_sep>tempDelayBins[i]-tempDelayBins[i-1]) min_sep=tempDelayBins[i]-tempDelayBins[i-1];
  }
  if (tempDelayBins.size()>0) {
    if (tempDelayBins.size()==1) min_sep=1;
    min_delay=tempDelayBins[0]-min_sep/2.;
    max_delay=tempDelayBins[tempDelayBins.size()-1]+min_sep/2.;
    n_delay=(max_delay-min_delay)/min_sep;
  };


  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Booking histograms
  //

  // FOLLOWS SHELL NUMBERING IN DATASTRUCTURES.H:
  std::vector<std::string> partitions;
  partitions.push_back("BpI");
  partitions.push_back("BpO");
  partitions.push_back("BmI");
  partitions.push_back("BmO");


  // 1. Average cluster size
  deb::Plot<TH1F> clusMeanSizeVsDelay(600, 600, "%s_clusMeanSizeVsDelay\nTDR", plotType.data());
  clusMeanSizeVsDelay.add(n_delay, min_delay, max_delay, "BPix;Mean cluster size in the Barrel"
			  " pixel;Delay [ns];Size");
  clusMeanSizeVsDelay.add(n_delay, min_delay, max_delay, "FPix;Mean cluster size in the Forward"
			  " pixel;Delay [ns];Size");
  clusMeanSizeVsDelay.efficiency();
 

  std::vector<deb::Plot<TH1F> > clusMeanSizeVsDelayBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t prt=0; prt<2; prt++) {
      deb::Plot<TH1F> p(600,600,"%s_clusMeanSizeVsDelayBPix_%s_PRT%d\nTDR",
			plotType.data(), partitions[i].data(), prt+1);
      for (size_t sec=0; sec<8; sec++) {
	p.add(n_delay, min_delay, max_delay, "SEC%d;Mean cluster size in %s PRT%d SEC%d;"
	       "Delay [ns];Size", sec+1, partitions[i].data(), prt+1, sec+1);
      }
      p.efficiency();
      clusMeanSizeVsDelayBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F> > clusMeanSizeVsDelayFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F> p(600,600,"%s_clusMeanSizeVsDelayFPix_%s\nTDR",
		      plotType.data(), partitions[i].data());
    for (size_t disk=0; disk<2; disk++) {
      for (size_t prt=0; prt<4; prt++) {
	p.add(n_delay, min_delay, max_delay, "Disk%d_PRT%d;Mean cluster size in %s Disk%d PRT%d;"
	       "Delay [ns];Size", disk+1, prt+1, partitions[i].data(), disk+1, prt+1);
      }
    }
    p.efficiency();
    clusMeanSizeVsDelayFPix.push_back(p);
  }
 
  // 2. Average cluster charge
  deb::Plot<TH1F> clusMeanChargeVsDelay(600, 600,"%s_clusMeanChargeVsDelay\nTDR", plotType.data());
  clusMeanChargeVsDelay.add(n_delay, min_delay, max_delay, "BPix;Mean cluster charge in the Barrel"
			    " pixel;Delay [ns];Charge [ke]");
  clusMeanChargeVsDelay.add(n_delay, min_delay, max_delay,"FPix;Mean cluster charge in the Forward"
			    " pixel;Delay [ns];Charge [ke]");
  clusMeanChargeVsDelay.efficiency();
 

  std::vector<deb::Plot<TH1F> > clusMeanChargeVsDelayBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t prt=0; prt<2; prt++) {
      deb::Plot<TH1F> p(600,600,"%s_clusMeanChargeVsDelayBPix_%s_PRT%d\nTDR",
			plotType.data(), partitions[i].data(), prt+1);
      for (size_t sec=0; sec<8; sec++) {
	p.add(n_delay, min_delay, max_delay, "SEC%d;Mean cluster charge in %s PRT%d SEC%d;"
	       "Delay [ns];Charge [ke]", sec+1, partitions[i].data(), prt+1, sec+1);
      }
      p.efficiency();
      clusMeanChargeVsDelayBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F> > clusMeanChargeVsDelayFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F> p(600,600,"%s_clusMeanChargeVsDelayFPix_%s\nTDR",
		      plotType.data(), partitions[i].data());
    for (size_t disk=0; disk<2; disk++) {
      for (size_t prt=0; prt<4; prt++) {
	p.add(n_delay, min_delay, max_delay,"Disk%d_PRT%d;Mean cluster charge in %s Disk%d PRT%d;"
	       "Delay [ns];Charge [ke]", disk+1, prt+1, partitions[i].data(), disk+1, prt+1);
      }
    }
    p.efficiency();
    clusMeanChargeVsDelayFPix.push_back(p);
  }
 

  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //

  int prevevt=NOVAL_I;
  int prevevtNumClus=NOVAL_I;
  int prevevtDelay=NOVAL_I;

  for (long i=0; i<clustTree->GetEntries(); i++) {
    clustTree->GetEntry(i);

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.orb, evt.run);
    }
    int step=stepDelay.first;
    int delay=stepDelay.second;
    if (step==NOVAL_I || delay==NOVAL_I) continue;

    // Detector coordinate
    std::string detector= (clust.mod_on.det==0) ? "BPix" : "FPix";
    std::string shell = clust.mod_on.shell();
    int group=0;
    std::string rog;
    std::ostringstream srog;
    if (clust.mod_on.det==0) {
      group = clust.mod_on.shell_num()*2+clust.mod_on.prt-1;
      srog<<"SEC"<<clust.mod_on.sec;
    } else if (clust.mod_on.det==1) {
      group = clust.mod_on.shell_num();
      srog<<"Disk"<<abs(clust.mod_on.disk)<<"_PRT"<<clust.mod_on.prt;
    }
    rog=srog.str();


    // 1. Mean cluster size
    if (clust.charge>12.) {
      if (clust.mod_on.det==0) {
	clusMeanSizeVsDelay[detector].num()->Fill(delay, clust.size);
	clusMeanSizeVsDelay[detector].den()->Fill(delay);
	clusMeanSizeVsDelayBPix[group][rog].num()->Fill(delay, clust.size);
	clusMeanSizeVsDelayBPix[group][rog].den()->Fill(delay);
      } else if (clust.mod_on.det==1) {
	clusMeanSizeVsDelay[detector].num()->Fill(delay, clust.size);
	clusMeanSizeVsDelay[detector].den()->Fill(delay);
	clusMeanSizeVsDelayFPix[group][rog].num()->Fill(delay, clust.size);
	clusMeanSizeVsDelayFPix[group][rog].den()->Fill(delay);
      } 
    }


    // 2. Mean cluster charge
    if (clust.size>1) {
      if (clust.mod_on.det==0) {
	clusMeanChargeVsDelay[detector].num()->Fill(delay, clust.charge);
	clusMeanChargeVsDelay[detector].den()->Fill(delay);
	clusMeanChargeVsDelayBPix[group][rog].num()->Fill(delay, clust.charge);
	clusMeanChargeVsDelayBPix[group][rog].den()->Fill(delay);
      } else if (clust.mod_on.det==1) {
	clusMeanChargeVsDelay[detector].num()->Fill(delay, clust.charge);
	clusMeanChargeVsDelay[detector].den()->Fill(delay);
	clusMeanChargeVsDelayFPix[group][rog].num()->Fill(delay, clust.charge);
	clusMeanChargeVsDelayFPix[group][rog].den()->Fill(delay);
      } 
    }


    if (evt.evt!=prevevt) {
      
      if (prevevtDelay!=NOVAL_I) {
      }
      prevevtDelay=delay;
      prevevtNumClus=0;
      prevevt=evt.evt;
    }
    prevevtNumClus++;
    
  }
  
  
  // ----------------------------------------------------------------------------------------------
  // Post-processing and drawing histograms
  //
  
  // 1. Mean cluster size
  clusMeanSizeVsDelay.efficiency();
  //clusMeanSizeVsDelay.setAxisRange(".*", 0, 1.0, "Y");
  clusMeanSizeVsDelay.setColor(".*", "LineMarker", 1, 1);
  clusMeanSizeVsDelay.Draw("FPix!FPix & BPix!BPix;overlayPE");
  clusMeanSizeVsDelay.Write();

  for (size_t i=0; i<clusMeanSizeVsDelayBPix.size(); i++) {
    clusMeanSizeVsDelayBPix[i].efficiency();
    //clusMeanSizeVsDelayBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    clusMeanSizeVsDelayBPix[i].setColor(".*", "LineMarker", 1, 1);
    clusMeanSizeVsDelayBPix[i].Draw("SEC1 & SEC2 & SEC3 & SEC4 & SEC5 & SEC6 & SEC7 & SEC8;PE",3);
    clusMeanSizeVsDelayBPix[i].Write();
  }

  for (size_t i=0; i<clusMeanSizeVsDelayFPix.size(); i++) {
    clusMeanSizeVsDelayFPix[i].efficiency();
    //clusMeanSizeVsDelayFPix[i].setAxisRange(".*", 0, 1.0, "Y");
    clusMeanSizeVsDelayFPix[i].setColor(".*", "LineMarker", 1, 1);
    clusMeanSizeVsDelayFPix[i].Draw("Disk1_PRT1 & Disk2_PRT1 & Disk1_PRT2 & Disk2_PRT2 & "
			       "Disk1_PRT3 & Disk2_PRT3 & Disk1_PRT4 & Disk2_PRT4;PE",3);
    clusMeanSizeVsDelayFPix[i].Write();
  }


  // 2. Mean cluster charge
  clusMeanChargeVsDelay.efficiency();
  //clusMeanChargeVsDelay.setAxisRange(".*", 0, 1.0, "Y");
  clusMeanChargeVsDelay.setColor(".*", "LineMarker", 1, 1);
  clusMeanChargeVsDelay.Draw("FPix!FPix & BPix!BPix;overlayPE");
  clusMeanChargeVsDelay.Write();

  for (size_t i=0; i<clusMeanChargeVsDelayBPix.size(); i++) {
    clusMeanChargeVsDelayBPix[i].efficiency();
    //clusMeanChargeVsDelayBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    clusMeanChargeVsDelayBPix[i].setColor(".*", "LineMarker", 1, 1);
    clusMeanChargeVsDelayBPix[i].Draw("SEC1 & SEC2 & SEC3 & SEC4 & SEC5 & SEC6 & SEC7 &SEC8;PE",3);
    clusMeanChargeVsDelayBPix[i].Write();
  }

  for (size_t i=0; i<clusMeanChargeVsDelayFPix.size(); i++) {
    clusMeanChargeVsDelayFPix[i].efficiency();
    //clusMeanChargeVsDelayFPix[i].setAxisRange(".*", 0, 1.0, "Y");
    clusMeanChargeVsDelayFPix[i].setColor(".*", "LineMarker", 1, 1);
    clusMeanChargeVsDelayFPix[i].Draw("Disk1_PRT1 & Disk2_PRT1 & Disk1_PRT2 & Disk2_PRT2 & "
			       "Disk1_PRT3 & Disk2_PRT3 & Disk1_PRT4 & Disk2_PRT4;PE",3);
    clusMeanChargeVsDelayFPix[i].Write();
  }





  //
  // The end
  //

  return 1;
}




//______________________________________________________________________________
//
//

//TStyle gStyle;
//TROOT TRoot;


int main(int argc, char* argv[]) 
{

  if (argc<2) {
    std::cout << "Usage: TimingAnalysis ";
    std::cout << "<input file> [<output file>]" << std::endl;
    return 0;
  }

  std::string infilename=argv[1];
  std::string outfilename = (argc>2) ? argv[2] : "TimingAnalysis.root";
  std::string ntuplename =  (argc>3) ? argv[3] : "ctfNtuple";

  std::ostringstream addname;
  TChain *trajTree=new TChain("trajTree");
  addname.str("");
  addname << infilename << "/" << ntuplename << "/trajTree";
  trajTree->Add(addname.str().data());
  std::cout << "Collecting files " << addname.str().data() << std::endl;

  TChain *trackTree=new TChain("trackTree");
  addname.str("");
  addname << infilename << "/" << ntuplename << "/trackTree";
  trackTree->Add(addname.str().data());

  TChain *clustTree=new TChain("clustTree");
  addname.str("");
  addname << infilename << "/" << ntuplename << "/clustTree";
  clustTree->Add(addname.str().data());

  TFile outfile(outfilename.data(), "RECREATE");

  if (ntuplename.find("ctf")!=std::string::npos) {
    doRecHitPlots(trajTree, 0);
    //doTrackPlots();
    doClusterPlots(clustTree, 0);
  } else {
    doRecHitPlots(trajTree, 1);
    //doTrackPlots();
    doClusterPlots(clustTree, 1);    
  }

  outfile.Close();

  delete trajTree;
  delete trackTree;
  delete clustTree;

  return 1;   
}

#endif
