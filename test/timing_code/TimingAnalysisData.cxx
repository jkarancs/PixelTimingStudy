
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
    if (orbitnumber>=0 && orbitnumber<21.6e6)        { delay=0; step=0; }
    else if (orbitnumber>=21.6e6 && orbitnumber<2e8) { delay=-6; step=1; }
    else if (orbitnumber>=2e8 && orbitnumber<3e8) { delay=-1; step=2; }
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
    hitEffVsDelayBPixShell.add(n_delay, min_delay, max_delay, "%s!!BPix;Hit efficiency in the Barrel"
			       " pixel;Delay [ns];Efficiency", partitions[i].data());
    hitEffVsDelayFPixShell.add(n_delay, min_delay, max_delay, "%s!!FPix;Hit efficiency in the Forward"
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
	p.add(n_delay, min_delay, max_delay, "SEC%d!!BPix %s PRT%d;Hit efficiency in BPix %s PRT%d SEC%d;"
	      "Delay [ns];Efficiency", sec+1, partitions[i].data(), prt+1, partitions[i].data(), prt+1, sec+1);
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
	p.add(n_delay, min_delay, max_delay, "Disk%d_PRT%d!!FPix %s;Hit efficiency in FPix %s Disk%d PRT%d;"
	      "Delay [ns];Efficiency", disk+1, prt+1, partitions[i].data(), partitions[i].data(), disk+1, prt+1);
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


  // ROG efficiency
  deb::Plot<TH2F> rogEffBPix(600, 600, "%s_rogEffBPix\nTDR", plotType.data());
  deb::Plot<TH2F> rogEffFPix(600, 600, "%s_rogEffFPix\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    rogEffBPix.add(8,0.5,8.5,12,-0.5,11.5,"BPix_[%d]!delay %d ns;Hit efficiency in the Barrel "
		   "pixel;Sector;PRT1 and PRT2 in BpI/BpO/BmI/BmO", it->first, it->second);
    rogEffFPix.add(8,0.5,8.5,12,-0.5,11.5,"FPix_[%d]!delay %d ns;Hit efficiency in the Forward "
		   "pixel;Sector;PRT1 and PRT2 in BpI/BpO/BmI/BmO", it->first, it->second);
  }
  rogEffBPix.efficiency();
  rogEffFPix.efficiency();
  


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

    // 4. ROG efficiency
    if (trajmeas.validhit||trajmeas.missing) {
      if (trajmeas.mod_on.det==0) {
	int rogeff_y=trajmeas.mod_on.shl*3+trajmeas.mod_on.prt-1;
	rogEffBPix[detector+sstep].den()->Fill(trajmeas.mod_on.sec, rogeff_y);
	if (trajmeas.validhit)rogEffBPix[detector+sstep].num()->Fill(trajmeas.mod_on.sec,rogeff_y);
      } else {
	int rogeff_y=trajmeas.mod_on.shl*3+abs(trajmeas.mod_on.disk)-1;
	rogEffFPix[detector+sstep].den()->Fill(abs(trajmeas.mod_on.sec), rogeff_y);
	if (trajmeas.validhit)rogEffFPix[detector+sstep].num()->Fill(abs(trajmeas.mod_on.sec),rogeff_y);
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
  hitEffVsDelay.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelay.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelay.Draw("FPix!& BPix!;overlayPE");
  hitEffVsDelay.Write();

  hitEffVsDelayBPixShell.efficiency();
  hitEffVsDelayBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelayBPixShell.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelayBPixShell.Draw("BpO!!& BpI!!& BmO!!& BmI!!;PE",2);
  hitEffVsDelayBPixShell.Write();

  hitEffVsDelayFPixShell.efficiency();
  hitEffVsDelayFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelayFPixShell.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelayFPixShell.Draw("BpO!!& BpI!!& BmO!!& BmI!!;PE",2);
  hitEffVsDelayFPixShell.Write();

  for (size_t i=0; i<hitEffVsDelayBPix.size(); i++) {
    hitEffVsDelayBPix[i].efficiency();
    hitEffVsDelayBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    hitEffVsDelayBPix[i].setColor(".*", "LineMarker", 1, 1);
    hitEffVsDelayBPix[i].Draw("SEC[1-8]!;PE",3);
    hitEffVsDelayBPix[i].Write();
  }

  for (size_t i=0; i<hitEffVsDelayFPix.size(); i++) {
    hitEffVsDelayFPix[i].efficiency();
    hitEffVsDelayFPix[i].setAxisRange(".*", 0, 1.0, "Y");
    hitEffVsDelayFPix[i].setColor(".*", "LineMarker", 1, 1);
    hitEffVsDelayFPix[i].Draw("Disk[1,2]_PRT[1-4]!;PE",3);
    hitEffVsDelayFPix[i].Write();
  }


  // 3. Cluster charge
  clusCharge.setColor("BPix_.*", "LineMarker", 1, 1);
  clusCharge.setColor("FPix_.*", "LineMarker", 1, 1);
  clusCharge.scaleAreaTo(".*", 1.);
  clusCharge.Draw("BPix_.*!!BPix;overlayHISTL\nFPix_.*!FPix;overlayHISTL");
  clusCharge.Write();

  //clusChargeBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  clusChargeBPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  clusChargeBPixShell.scaleAreaTo(".*", 1.);
  clusChargeBPixShell.Draw("BpO_.*!!BpO;overlayHISTL\nBpI_.*!!BpI;overlayHISTL\n"
			   "BmO_.*!!BmO;overlayHISTL\nBmI_.*!!BmI;overlayHISTL\n", 2);
  clusChargeBPixShell.Write();


  //clusChargeFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  clusChargeFPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  clusChargeFPixShell.scaleAreaTo(".*", 1.);
  clusChargeFPixShell.Draw("BpO_.*!!BpO;overlayHISTL\nBpI_.*!!BpI;overlayHISTL\n"
			   "BmO_.*!!BmO;overlayHISTL\nBmI_.*!!BmI;overlayHISTL\n", 2);
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
  
  // 4. ROG efficiency
  rogEffBPix.efficiency();
  rogEffBPix.setAxisRange(".*", 0., 1.0, "Z");
  rogEffBPix.Draw("BPix_.*;COLZ",2);
  rogEffBPix.Write();

  rogEffFPix.efficiency();
  rogEffFPix.setAxisRange(".*", 0., 1.0, "Z");
  rogEffFPix.Draw("FPix_.*;COLZ",2);
  rogEffFPix.Write();

  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // The end
  //

  return 1;
}





///////////////////////////////////////////////////////////////////////////////////////////////////
//
// TrackTree based plots
//


int doTrackPlots(TChain *trackTree, int data) { // data 0:cosmics else:collision

  if (trackTree->GetEntries()==0) {
    std::cout<<"No hit found in trackTree. Returning.\n";
    return 0;
  }

  std::string plotType="Tracks";
  std::map<int,int> delays;
  float min_delay, max_delay;
  int n_delay=0;
  

  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Preprocessing cycle
  //
  
  // Set the tree for reading
  EventData evt;
  TrackData track;
  trackTree->SetBranchAddress("event", &evt);
  trackTree->SetBranchAddress("track", &track);

  std::cout<<"Reading trackTree of entries: "<<trackTree->GetEntries()<<std::endl;

  std::string prevFileName="";
  std::vector<float> tempDelayBins;

  for (Long64_t i=0; i<trackTree->GetEntries(); i++) {
    trackTree->GetEntry(i);
    std::string fname=trackTree->GetFile()->GetName();
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

  // 1. Ntracks at delays
  deb::Plot<TH1F> Ntrk(600, 600, "%s_Ntrk\nTDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    Ntrk.add(60, -0.5, 59.5, "Ntrk_[%d]!delay %d ns;Number of tracks at "
	     " %d ns;N_{tracks}", it->first, it->second, it->second);
    Ntrk.add(60, -0.5, 59.5, "Npixtrk_[%d]!delay %d ns;Number of tracks crossing "
	     "pixels at %d ns;N_{tracks}", it->first, it->second, it->second);
  }


  // 2. Npixhit/tracks at delays
  deb::Plot<TH1F> Npixhit(600, 600, "%s_Npixhit\nTDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    Npixhit.add(10, -0.5, 9.5, "Npixhit_[%d]!delay %d ns;Number of valid hits per track at"
		" %d ns;N_{hits}", it->first, it->second, it->second);

    Npixhit.add(10, -0.5, 9.5, "Npixhit2_[%d]!delay %d ns;Number of valid hits per track crossing"
		"pixels at %d ns;N_{hits}", it->first, it->second, it->second);
  }


  // 3. TrackD0, TrackDz, TrackPt, TrackChi2 at delays
  deb::Plot<TH1F> TrackParm(600, 600, "%s_TrackParm\nTDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    TrackParm.add(40, -0.5, 39.5, "pt_[%d]!delay %d ns;Transverse momentum of tracks at "
		  " %d ns;pt [GeV]", it->first, it->second, it->second);
    TrackParm.add(40, -0.5, 39.5, "chi2_[%d]!delay %d ns;Chi-square of tracks at "
		  " %d ns;chi2/ndof", it->first, it->second, it->second);
    TrackParm.add(100, -100, 100, "d0_[%d]!delay %d ns;Impact parameter of tracks at "
		  " %d ns;D0 [cm]", it->first, it->second, it->second);
    TrackParm.add(100, -200, 200, "dz_[%d]!delay %d ns;Delta Z of tracks at "
		  " %d ns;Dz [cm]", it->first, it->second, it->second);
  }


  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //

  int prevevt=NOVAL_I;
  int prevevtDelay=NOVAL_I;
  int prevevtNumTracks=NOVAL_I;
  int prevevtNumPixTracks=NOVAL_I;

  for (long i=0; i<trackTree->GetEntries(); i++) {
    trackTree->GetEntry(i);

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

    // 1. Ntracks and Npixhit/tracks at delays
    Npixhit("Npixhit_[%d]", step).Fill(track.pix);
    if (track.pixhit[0]!=0 || track.pixhit[1]!=0) Npixhit("Npixhit2_[%d]", step).Fill(track.pix);

    // 3. TrackD0, TrackDz, TrackPt, TrackChi2 at delays
    TrackParm("pt_[%d]", step).Fill(track.pt);
    TrackParm("chi2_[%d]", step).Fill(track.chi2/track.ndof);
    TrackParm("d0_[%d]", step).Fill(track.d0);
    TrackParm("dz_[%d]", step).Fill(track.dz);



    if (evt.evt!=prevevt) {
      
      if (prevevtDelay!=NOVAL_I) {
	// 1. Ntracks and Npixhit/tracks at delays
	Ntrk("Ntrk_[%d]", step).Fill(prevevtNumTracks);
	Ntrk("Npixtrk_[%d]", step).Fill(prevevtNumPixTracks);
      }
      prevevtDelay=delay;
      prevevtNumTracks=0;
      prevevtNumPixTracks=0;
      prevevt=evt.evt;
    }
    
    prevevtNumTracks++;
    if (track.pixhit[0]!=0 || track.pixhit[1]!=0) prevevtNumPixTracks++;
    
  }
  

  // ----------------------------------------------------------------------------------------------
  // Post-processing and drawing histograms
  //  

  // 1. Ntracks at delays
  Ntrk.setColor("Ntrk_.*!", "LineMarker", 1, 1);
  Ntrk.setColor("Npixtrk_.*!", "LineMarker", 1, 1);
  Ntrk.Draw("Ntrk_.*!;overlayPE\nNpixtrk_.*!;overlayPE");
  Ntrk.Write();

  // 2. Npixhit/tracks at delays
  Npixhit.setColor("Npixhit_.*!", "LineMarker", 1, 1);
  Npixhit.setColor("Npixhit2_.*!", "LineMarker", 1, 1);
  Npixhit.Draw("Npixhit_.*!;overlayPE\nNpixhit2_.*!;overlayPE");
  Npixhit.Write();

  // 3. TrackD0, TrackDz, TrackPt, TrackChi2 at delays
  TrackParm.setColor("pt_.*!", "LineMarker", 1, 1);
  TrackParm.setColor("chi2_.*!", "LineMarker", 1, 1);
  TrackParm.setColor("d0_.*!", "LineMarker", 1, 1);
  TrackParm.setColor("dz_.*!", "LineMarker", 1, 1);
  TrackParm.Draw("pt_.*!;overlayPE\nchi2_.*!;overlayPE\nd0_.*!;overlayPE\ndz_.*!;overlayPE\n", 2);
  TrackParm.Write();

  return 1;
}




////////////////////////////////////////////////////////////////////////////////
//
// ClustTree based plots
//


int doClusterPlots(TChain *clustTree, int data) {

  if (clustTree->GetEntries()==0) {
    std::cout<<"No hit found in clustTree. Returning.\n";
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
 
  // 3. Cluster Charge
 
  deb::Plot<TH1F> clusCharge(600, 600, "%s_clusCharge\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    clusCharge.add(200, 0., 200., "BPix_[%d]!delay %d !BPix;Cluster charge in the Barrel"
		   " pixel;Charge [ke]", it->first, it->second);
    clusCharge.add(200, 0., 200., "FPix_[%d]!delay %d !FPix;Cluster charge in the Forward"
		   " pixel;Charge [ke]", it->first, it->second);
  }
  clusCharge.efficiency();


  deb::Plot<TH1F> clusChargeBPixShell(600,600,"%s_clusChargeBPixShell\nTDR",plotType.data());
  deb::Plot<TH1F> clusChargeFPixShell(600,600,"%s_clusChargeFPixShell\nTDR",plotType.data());
  for (size_t i=0; i<partitions.size(); i++) {
    for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
      clusChargeBPixShell.add(50, 0., 200., "%s_[%d]!delay %d !BPix %s;Cluster charge in the Barrel"
			      " pixel;Charge [ke]", partitions[i].data(), it->first, it->second, partitions[i].data());
      clusChargeFPixShell.add(50, 0., 200., "%s_[%d]!delay %d !FPix %s;Cluster charge in the Forward"
			      " pixel;Charge [ke]", partitions[i].data(), it->first, it->second, partitions[i].data());
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
	  p.add(30, 0., 180., "SEC%d_[%d]!delay %d !BPix;Cluster charge in %s PRT%d SEC%d;"
		"Delay [ns];Charge [ke]", sec+1,  it->first, it->second, 
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
	  p.add(30, 0., 180., "Disk%d_PRT%d_[%d]!delay %d !FPix;Cluster charge in %s Disk%d PRT%d;"
		"Delay [ns];Charge [ke]", disk+1, prt+1, it->first, it->second, 
		partitions[i].data(), disk+1, prt+1);
	}
      }
    }
    p.efficiency();
    clusChargeFPix.push_back(p);
  }


  // 4.  Pixel Charge
 
  deb::Plot<TH1F> pixCharge(600, 600, "%s_pixCharge\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    pixCharge.add(50, 0., 50., "BPix_[%d]!delay %d !BPix;Pixel charge in clusters in the Barrel"
		   " pixel;Charge [ke]", it->first, it->second);
    pixCharge.add(50, 0., 50., "FPix_[%d]!delay %d !FPix;Pixel charge in clusters in the Forward"
		   " pixel;Charge [ke]", it->first, it->second);
  }
  pixCharge.efficiency();


  deb::Plot<TH1F> pixChargeBPixShell(600,600,"%s_pixChargeBPixShell\nTDR",plotType.data());
  deb::Plot<TH1F> pixChargeFPixShell(600,600,"%s_pixChargeFPixShell\nTDR",plotType.data());
  for (size_t i=0; i<partitions.size(); i++) {
    for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
      pixChargeBPixShell.add(50, 0., 50., "%s_[%d]!delay %d ns!BPix %s;Pixel charge in clusters in the Barrel"
			     " pixel;Charge [ke]", partitions[i].data(), it->first, it->second, partitions[i].data());
      pixChargeFPixShell.add(50, 0., 50., "%s_[%d]!delay %d ns!FPix %s;Pixel charge in clusters in the Forward"
			     " pixel;Charge [ke]", partitions[i].data(), it->first, it->second, partitions[i].data());
    }
  }
  pixChargeBPixShell.efficiency();
  pixChargeFPixShell.efficiency();


  std::vector<deb::Plot<TH1F> > pixChargeBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t prt=0; prt<2; prt++) {
      deb::Plot<TH1F> p(600,600,"%s_pixChargeBPix_%s_PRT%d\nTDR",
			plotType.data(), partitions[i].data(), prt+1);
      for (size_t sec=0; sec<8; sec++) {
	for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
	  p.add(50, 0., 50., "SEC%d_[%d]!delay %d ns!BPix;Pixel charge in clusters in %s PRT%d SEC%d;"
		"Delay [ns];Charge [ke]", sec+1,  it->first, it->second, 
		partitions[i].data(), prt+1, sec+1);
	}
      }
      p.efficiency();
      pixChargeBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F> > pixChargeFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F> p(600,600,"%s_pixChargeFPix_%s\nTDR",
		      plotType.data(), partitions[i].data());
    for (size_t disk=0; disk<2; disk++) {
      for (size_t prt=0; prt<4; prt++) {
	for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
	  p.add(50, 0., 50., "Disk%d_PRT%d_[%d]!delay %d ns!FPix;Pixel charge in clusters in %s Disk%d PRT%d;"
		"Delay [ns];Charge [ke]", disk+1, prt+1, it->first, it->second, 
		partitions[i].data(), disk+1, prt+1);
	}
      }
    }
    p.efficiency();
    pixChargeFPix.push_back(p);
  }


  // 5. number of clusters in L1/L2/L3 and Disk (+-)1/(+-)2 per event

  deb::Plot<TH1F> Nclus(600, 600, "%s_Nclus\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    for (int i=1; i<4; i++) {
      Nclus.add(50, 0., 50., "BPix_L%d_[%d]!delay %d !BPix Layer %d;Number of clusters in Layer %d"
		" in the Barrel pixel per event;N_{clusters} / Event", 
		i, it->first, it->second, i, i);
    }
    for (int i=-2; i<3; i++) {
      if (i==0) continue;
      Nclus.add(50, 0., 50., "FPix_D%d_[%d]!delay %d !FPix Diks %d;Number of clusters in Disk %d"
		" in the Forward pixel per event;N_{clusters} / Event", 
		i, it->first, it->second, i, i);
    }
  }


  // 6. size of clusters in L1/L2/L3 and Disk (+-)1/(+-)2

  deb::Plot<TH1F> sizeClus(600, 600, "%s_sizeClus\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    for (int i=1; i<4; i++) {
      sizeClus.add(30, 0.5, 30.5, "BPix_L%d_[%d]!delay %d !BPix Layer %d;Size of clusters in Layer "
		   "%d in the Barrel pixel", i, it->first, it->second, i, i);
    }
    for (int i=-2; i<3; i++) {
      if (i==0) continue;
      sizeClus.add(30, 0.5, 30.5, "FPix_D%d_[%d]!delay %d !FPix Diks %d;Size of clusters in Disk %d"
		   " in the Forward pixel", i, it->first, it->second, i, i);
    }
  }


  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //

  int prevevt=NOVAL_I;
  int prevevtNumClus=NOVAL_I;
  int prevevtNumClusFPix[5];
  int prevevtNumClusBPix[3];
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
    std::ostringstream ss;
    ss << "_[" << step << "]";
    std::string sstep=ss.str();

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

    // 3. Cluster charge
    clusCharge[detector+sstep].Fill(clust.charge);
    if (clust.mod_on.det==0) {
      clusChargeBPixShell[shell+sstep].Fill(clust.charge);
      clusChargeBPix[group][rog+sstep].Fill(clust.charge);
    } else {
      clusChargeFPixShell[shell+sstep].Fill(clust.charge);
      clusChargeFPix[group][rog+sstep].Fill(clust.charge);
    }

    // 4. Pixel charge

    if (clust.size>0) {
      for (int p=0; p<clust.size; p++) {
	pixCharge[detector+sstep].Fill(clust.adc[p]);
	if (clust.mod_on.det==0) {
	  pixChargeBPixShell[shell+sstep].Fill(clust.adc[p]);
	  pixChargeBPix[group][rog+sstep].Fill(clust.adc[p]);
	} else {
	  pixChargeFPixShell[shell+sstep].Fill(clust.adc[p]);
	  pixChargeFPix[group][rog+sstep].Fill(clust.adc[p]);
	}
      }
    }


    // 6. size of clusters in L1/L2/L3 and Disk (+-)1/(+-)2
    if (clust.mod_on.det==0) {    
      sizeClus("BPix_L%d_[%d]", clust.mod_on.layer, step).Fill(clust.size);
    } else if (clust.mod_on.det==1) { 
      sizeClus("FPix_D%d_[%d]", clust.mod_on.disk, step).Fill(clust.size);
    }


    // Event level quantities
    //
    if (evt.evt!=prevevt) {
      
      // 5. number of clusters in L1/L2/L3 and Disk (+-)1/(+-)2
      for (int ilay=1; ilay<4; ilay++) {
	if (prevevtDelay!=NOVAL_I)
	  Nclus("BPix_L%d_[%d]", ilay, step).Fill(prevevtNumClusBPix[ilay]);
	prevevtNumClusBPix[ilay]=0;
      }
      for (int idsk=-2; idsk<3; idsk++) {
	if (idsk==0) continue;
	if (prevevtDelay!=NOVAL_I)
	  Nclus("FPix_D%d_[%d]", idsk, step).Fill(prevevtNumClusFPix[idsk+2]);
	prevevtNumClusFPix[idsk+2]=0;
      }
      
      prevevtDelay=delay;
      prevevtNumClus=0;
      prevevt=evt.evt;
    }
    prevevtNumClus++;
    
    if (clust.mod_on.det==0) {
      prevevtNumClusBPix[clust.mod_on.layer]++;
    } else if (clust.mod_on.det==1) {
      prevevtNumClusFPix[clust.mod_on.disk+2]++;
    }
  }
  
  
  // ----------------------------------------------------------------------------------------------
  // Post-processing and drawing histograms
  //
  
  // 1. Mean cluster size
  clusMeanSizeVsDelay.efficiency();
  //clusMeanSizeVsDelay.setAxisRange(".*", 0, 1.0, "Y");
  clusMeanSizeVsDelay.setColor(".*", "LineMarker", 1, 1);
  clusMeanSizeVsDelay.setStyle(".*", "Marker", 1);
  clusMeanSizeVsDelay.Draw("FPix!FPix & BPix!BPix;overlayPE");
  clusMeanSizeVsDelay.Write();

  for (size_t i=0; i<clusMeanSizeVsDelayBPix.size(); i++) {
    clusMeanSizeVsDelayBPix[i].efficiency();
    //clusMeanSizeVsDelayBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    clusMeanSizeVsDelayBPix[i].setColor(".*", "LineMarker", 1, 1);
    clusMeanSizeVsDelayBPix[i].setStyle(".*", "Marker", 1);
    clusMeanSizeVsDelayBPix[i].Draw("SEC1 & SEC2 & SEC3 & SEC4 & SEC5 & SEC6 & SEC7 & SEC8;PE",3);
    clusMeanSizeVsDelayBPix[i].Write();
  }

  for (size_t i=0; i<clusMeanSizeVsDelayFPix.size(); i++) {
    clusMeanSizeVsDelayFPix[i].efficiency();
    //clusMeanSizeVsDelayFPix[i].setAxisRange(".*", 0, 1.0, "Y");
    clusMeanSizeVsDelayFPix[i].setColor(".*", "LineMarker", 1, 1);
    clusMeanSizeVsDelayFPix[i].setStyle(".*", "Marker", 1);
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


  // 3. Cluster charge
  clusCharge.setColor("BPix_.*", "LineMarker", 1, 1);
  clusCharge.setColor("FPix_.*", "LineMarker", 1, 1);
  clusCharge.scaleAreaTo(".*", 1.);
  clusCharge.Draw("BPix_.*!!;overlayPE\nFPix_.*!!;overlayPE");
  clusCharge.Write();

  //clusChargeBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  clusChargeBPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  clusChargeBPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  clusChargeBPixShell.scaleAreaTo(".*", 1.);
  clusChargeBPixShell.Draw("BpO_.*!!;overlayPE\nBpI_.*!!;overlayPE\n"
			   "BmO_.*!!;overlayPE\nBmI_.*!!;overlayPE\n", 2);
  clusChargeBPixShell.Write();


  //clusChargeFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  clusChargeFPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  clusChargeFPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  clusChargeFPixShell.scaleAreaTo(".*", 1.);
  clusChargeFPixShell.Draw("BpO_.*!!;overlayPE\nBpI_.*!!;overlayPE\n"
			   "BmO_.*!!;overlayPE\nBmI_.*!!;overlayPE\n", 2);
  clusChargeFPixShell.Write();


  for (size_t i=0; i<clusChargeBPix.size(); i++) {
    //clusChargeBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    for (int j=1; j<9; j++) {
      std::ostringstream color;
      color<<"SEC"<<j<<".*";
      clusChargeBPix[i].setColor(color.str(), "LineMarker", 1, 1);
    }
    clusChargeBPix[i].scaleAreaTo(".*", 1.);
    clusChargeBPix[i].Draw("SEC1_.*!;overlayPE\n"
			   "SEC2_.*!;overlayPE\n"
			   "SEC3_.*!;overlayPE\n"
			   "SEC4_.*!;overlayPE\n"
			   "SEC5_.*!;overlayPE\n"
			   "SEC6_.*!;overlayPE\n"
			   "SEC7_.*!;overlayPE\n"
			   "SEC8_.*!;overlayPE",3);
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
    clusChargeFPix[i].Draw("Disk1_PRT1_.*!;overlayPE\n"
			   "Disk1_PRT2_.*!;overlayPE\n"
			   "Disk1_PRT3_.*!;overlayPE\n"
			   "Disk1_PRT4_.*!;overlayPE\n"
			   "Disk2_PRT1_.*!;overlayPE\n"
			   "Disk2_PRT2_.*!;overlayPE\n"
			   "Disk2_PRT3_.*!;overlayPE\n"
			   "Disk2_PRT4_.*!;overlayPE",3);
    clusChargeFPix[i].Write();
  }
  

  // 4. Pixel charge
  pixCharge.setColor("BPix_.*", "LineMarker", 1, 1);
  pixCharge.setColor("FPix_.*", "LineMarker", 1, 1);
  pixCharge.scaleAreaTo(".*", 1.);
  pixCharge.Draw("BPix_.*!!;overlayPE\nFPix_.*!!;overlayPE");
  pixCharge.Write();

  //pixChargeBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  pixChargeBPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  pixChargeBPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  pixChargeBPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  pixChargeBPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  pixChargeBPixShell.scaleAreaTo(".*", 1.);
  pixChargeBPixShell.Draw("BpO_.*!!;overlayPE\nBpI_.*!!;overlayPE\n"
			   "BmO_.*!!;overlayPE\nBmI_.*!!;overlayPE\n", 2);
  pixChargeBPixShell.Write();


  //pixChargeFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  pixChargeFPixShell.setColor("BpO.*", "LineMarker", 1, 1);
  pixChargeFPixShell.setColor("BpI.*", "LineMarker", 1, 1);
  pixChargeFPixShell.setColor("BmO.*", "LineMarker", 1, 1);
  pixChargeFPixShell.setColor("BmI.*", "LineMarker", 1, 1);
  pixChargeFPixShell.scaleAreaTo(".*", 1.);
  pixChargeFPixShell.Draw("BpO_.*!!;overlayPE\nBpI_.*!!;overlayPE\n"
			   "BmO_.*!!;overlayPE\nBmI_.*!!;overlayPE\n", 2);
  pixChargeFPixShell.Write();


  for (size_t i=0; i<pixChargeBPix.size(); i++) {
    //pixChargeBPix[i].setAxisRange(".*", 0, 1.0, "Y");
    for (int j=1; j<9; j++) {
      std::ostringstream color;
      color<<"SEC"<<j<<".*";
      pixChargeBPix[i].setColor(color.str(), "LineMarker", 1, 1);
    }
    pixChargeBPix[i].scaleAreaTo(".*", 1.);
    pixChargeBPix[i].Draw("SEC1_.*!;overlayPE\n"
			   "SEC2_.*!;overlayPE\n"
			   "SEC3_.*!;overlayPE\n"
			   "SEC4_.*!;overlayPE\n"
			   "SEC5_.*!;overlayPE\n"
			   "SEC6_.*!;overlayPE\n"
			   "SEC7_.*!;overlayPE\n"
			   "SEC8_.*!;overlayPE",3);
    pixChargeBPix[i].Write();
  }

  for (size_t i=0; i<pixChargeFPix.size(); i++) {
    //pixChargeFPix[i]->setAxisRange(".*", 0, 1.0, "Y");
    pixChargeFPix[i].setColor("Disk1_PRT1.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk1_PRT2.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk1_PRT3.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk1_PRT4.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk2_PRT1.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk2_PRT2.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk2_PRT3.*", "LineMarker", 1, 1);
    pixChargeFPix[i].setColor("Disk2_PRT4.*", "LineMarker", 1, 1);
    pixChargeFPix[i].scaleAreaTo(".*", 1.);
    pixChargeFPix[i].Draw("Disk1_PRT1_.*!;overlayPE\n"
			  "Disk1_PRT2_.*!;overlayPE\n"
			  "Disk1_PRT3_.*!;overlayPE\n"
			  "Disk1_PRT4_.*!;overlayPE\n"
			  "Disk2_PRT1_.*!;overlayPE\n"
			  "Disk2_PRT2_.*!;overlayPE\n"
			  "Disk2_PRT3_.*!;overlayPE\n"
			  "Disk2_PRT4_.*!;overlayPE",3);
    pixChargeFPix[i].Write();
  }
  
  // 5. number of clusters in L1/L2/L3 and Disk (+-)1/(+-)2
  Nclus.setColor("BPix_L1.*", "LineMarker", 1, 1);
  Nclus.setColor("BPix_L2.*", "LineMarker", 1, 1);
  Nclus.setColor("BPix_L3.*", "LineMarker", 1, 1);
  Nclus.setColor("FPix_D-1.*", "LineMarker", 1, 1);
  Nclus.setColor("FPix_D-2.*", "LineMarker", 1, 1);
  Nclus.setColor("FPix_D1.*", "LineMarker", 1, 1);
  Nclus.setColor("FPix_D2.*", "LineMarker", 1, 1);
  //Nclus.scaleAreaTo(".*", 1);
  Nclus.Draw("BPix_L1.*!!;overlayPE\nBPix_L2.*!!;overlayPE\nBPix_L3.*!!;overlayPE\n"
	     "FPix_D1.*!!;overlayPE\nFPix_D2.*!!;overlayPE\n"
	     "FPix_D-1.*!!;overlayPE\nFPix_D-2.*!!;overlayPE\n", 3);
  Nclus.Write();
  

  // 6. size of clusters in L1/L2/L3 and Disk (+-)1/(+-)2
  sizeClus.setColor("BPix_L1.*", "LineMarker", 1, 1);
  sizeClus.setColor("BPix_L2.*", "LineMarker", 1, 1);
  sizeClus.setColor("BPix_L3.*", "LineMarker", 1, 1);
  sizeClus.setColor("FPix_D-1.*", "LineMarker", 1, 1);
  sizeClus.setColor("FPix_D-2.*", "LineMarker", 1, 1);
  sizeClus.setColor("FPix_D1.*", "LineMarker", 1, 1);
  sizeClus.setColor("FPix_D2.*", "LineMarker", 1, 1);
  sizeClus.scaleAreaTo(".*", 1);
  sizeClus.Draw("BPix_L1.*!!;overlayHIST\nBPix_L2.*!!;overlayHIST\nBPix_L3.*!!;overlayHIST\n"
		"FPix_D1.*!!;overlayHIST\nFPix_D2.*!!;overlayHIST\n"
		"FPix_D-1.*!!;overlayHIST\nFPix_D-2.*!!;overlayHIST\n", 3);
  sizeClus.Write();

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
    doTrackPlots(trackTree, 0);
    doClusterPlots(clustTree, 0);
  } else {
    doRecHitPlots(trajTree, 1);
    doTrackPlots(trackTree, 1);
    doClusterPlots(clustTree, 1);    
  }

  outfile.Close();

  delete trajTree;
  delete trackTree;
  delete clustTree;

  return 1;   
}

#endif
