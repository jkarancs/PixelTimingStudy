
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

  std::string plotType="RecHits";
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

  int istep=0;
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    drawAll<<"["<<it->first<<"]|delay "<<it->second<<" ns";
    drawAll_den<<"["<<it->first<<"].den|delay "<<it->second<<" ns";
    if (istep<delays.size()-1) {
      drawAll<<",";
      drawAll_den<<",";
    }
    istep++;
  }
  std::cout<<"Draw all: "<<drawAll.str()<<std::endl;
  std::cout<<"Draw all den: "<<drawAll_den.str()<<std::endl;

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


  // 1. Hit Efficiency vs DT time
  deb::Plot<TH1F> hitEffVsDTtimeBarrel(600, 600, "%s_hitEffVsDTtimeBarrel\\TDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    hitEffVsDTtimeBarrel.add(35,-35.,35., "[%d];Hit efficiency in the Barrel pixel at %d ns;"
			     "DT time [ns];Efficiency", it->first, it->second);
  }
  hitEffVsDTtimeBarrel.efficiency();


  // 2. Hit Efficiency vs Delay
  deb::Plot<TH1F> hitEffVsDelay(600, 600, "%s_hitEffVsDelay\\TDR", plotType.data());
  hitEffVsDelay.add(n_delay, min_delay, max_delay, "BPix;Hit efficiency in the Barrel"
		    " pixel;Delay [ns];Efficiency");
  hitEffVsDelay.add(n_delay, min_delay, max_delay, "FPix;Hit efficiency in the Forward"
		    " pixel;Delay [ns];Efficiency");
  hitEffVsDelay.efficiency();


  deb::Plot<TH1F> hitEffVsDelayBPixShell(600,600,"%s_hitEffVsDelayBPixShell\\TDR",plotType.data());
  deb::Plot<TH1F> hitEffVsDelayFPixShell(600,600,"%s_hitEffVsDelayFPixShell\\TDR",plotType.data());
  for (size_t i=0; i<partitions.size(); i++) {
    hitEffVsDelayBPixShell.add(n_delay, min_delay, max_delay, "%s;Hit efficiency in the Barrel"
			       " pixel;Delay [ns];Efficiency", partitions[i].data());
    hitEffVsDelayFPixShell.add(n_delay, min_delay, max_delay, "%s;Hit efficiency in the Forward"
			       " pixel;Delay [ns];Efficiency", partitions[i].data());
  }
  hitEffVsDelayBPixShell.efficiency();
  hitEffVsDelayFPixShell.efficiency();


  std::vector<deb::Plot<TH1F>* > hitEffVsDelayBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t j=0; j<2; j++) {
      deb::Plot<TH1F>* p = new deb::Plot<TH1F> (600,600,"%s_hitEffVsDelayBPix_%s_PRT%d\\TDR",
						plotType.data(), partitions[i].data(), j+1);
      for (size_t sec=0; sec<8; sec++) {
	p->add(n_delay, min_delay, max_delay, "[%d];Hit efficiency in %s PRT%d SEC%d;"
	       "Delay [ns];Efficiency", sec+1, partitions[i].data(), j+1, sec+1);
      }
      p->efficiency();
      hitEffVsDelayBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F>* > hitEffVsDelayFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F>* p = new deb::Plot<TH1F> (600,600,"%s_hitEffVsDelayFPix_%s\\TDR",
					      plotType.data(), partitions[i].data());
    for (size_t j=0; j<2; j++) {
      for (size_t prt=0; prt<4; prt++) {
	p->add(n_delay, min_delay, max_delay, "Disk%d_PRT%d;Hit efficiency in %s Disk%d PRT%d;"
	       "Delay [ns];Efficiency", j+1, prt+1, partitions[i].data(), j+1, prt+1);
      }
    }
    p->efficiency();
    hitEffVsDelayFPix.push_back(p);
  }



  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //

  for (long i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.orb, evt.run);
    }
    int step=stepDelay.first;
    int delay=stepDelay.second;
    if (step==NOVAL_I || delay==NOVAL_I) continue;

    // 1. Hit Efficiency vs DT time
    if (trajmeas.telescope && trajmeas.mod_on.det==0 &&
	(trajmeas.validhit || trajmeas.missing) ) {
      if (step!=NOVAL_I) {
	hitEffVsDTtimeBarrel(step).den()->Fill(evt.tmuon);
	if (trajmeas.validhit) hitEffVsDTtimeBarrel(step).num()->Fill(evt.tmuon);
      }
    }
    // ---------------------------------------

    // 2. Hit Efficiency vs delay
    if (trajmeas.telescope && (trajmeas.validhit || trajmeas.missing) ) {
      std::string partition=trajmeas.mod_on.shell();
      int partnum=trajmeas.mod_on.shell_num();

      if (trajmeas.mod_on.det==0) {
	hitEffVsDelay["BPix"].den()->Fill(delay);
	hitEffVsDelayBPixShell[partition].den()->Fill(delay);
	(*hitEffVsDelayBPix[partnum*2+trajmeas.mod_on.prt-1])(trajmeas.mod_on.sec).den()->Fill(delay);
	if (trajmeas.validhit) {
	  hitEffVsDelay["BPix"].num()->Fill(delay);
	  hitEffVsDelayBPixShell[partition].num()->Fill(delay);
	  (*hitEffVsDelayBPix[partnum*2+trajmeas.mod_on.prt-1])(trajmeas.mod_on.sec).num()->Fill(delay);
	}

      } else {
	hitEffVsDelay["FPix"].den()->Fill(delay);
	hitEffVsDelayFPixShell[partition].den()->Fill(delay);
	(*hitEffVsDelayFPix[partnum])("Disk%d_PRT%d", abs(trajmeas.mod_on.disk), trajmeas.mod_on.prt).
	  den()->Fill(delay);
	if (trajmeas.validhit) {
	  hitEffVsDelay["FPix"].num()->Fill(delay);
	  hitEffVsDelayFPixShell[partition].num()->Fill(delay);
	  (*hitEffVsDelayFPix[partnum])("Disk%d_PRT%d", abs(trajmeas.mod_on.disk), trajmeas.mod_on.prt).
	    num()->Fill(delay);
	}
      }

    }
    // ---------------------------------------


  }



  // ----------------------------------------------------------------------------------------------
  // Post-processing and drawing histograms
  //

  // 1. Hit Efficiency vs DT time
  hitEffVsDTtimeBarrel.efficiency();
  hitEffVsDTtimeBarrel.scaleMaximumTo("denominator", 1);
  hitEffVsDTtimeBarrel.setColor("Line", 1);
  hitEffVsDTtimeBarrel.setAxisRange(0., 1.1, "Y");
  std::ostringstream hitEffVsDTtimeBarrel_draw;
  hitEffVsDTtimeBarrel_draw<<drawAll.str()<<";overlayHIST;Hit efficiency in BPix\\"
			   <<drawAll_den.str()<<";overlayHISTL;DT time distribution"
			   <<" of muons;DT time [ns]";
  hitEffVsDTtimeBarrel.Draw(hitEffVsDTtimeBarrel_draw.str(), 1);
  hitEffVsDTtimeBarrel.Write();


  // 2. Hit Efficiency vs Delay
  hitEffVsDelay.efficiency();
  //hitEffVsDelay.setAxisRange(0, 1.0, "Y");
  hitEffVsDelay.setColor("Line", 1);
  hitEffVsDelay.Draw("FPix|FPix, BPix|BPix;overlayP");
  hitEffVsDelay.Write();

  hitEffVsDelayBPixShell.efficiency();
  //hitEffVsDelayBPixShell.setAxisRange(0, 1.0, "Y");
  hitEffVsDelayBPixShell.setColor("Line", 1);
  hitEffVsDelayBPixShell.Draw("BpO|BpO, BpI|BpI, BmO|BmO, BmI|BmI;PE",2);
  hitEffVsDelayBPixShell.Write();

  hitEffVsDelayFPixShell.efficiency();
  //hitEffVsDelayFPixShell.setAxisRange(0, 1.0, "Y");
  hitEffVsDelayFPixShell.setColor("Line", 1);
  hitEffVsDelayFPixShell.Draw("BpO|BpO, BpI|BpI, BmO|BmO, BmI|BmI;PE",2);
  hitEffVsDelayFPixShell.Write();

  for (size_t i=0; i<hitEffVsDelayBPix.size(); i++) {
    hitEffVsDelayBPix[i]->efficiency();
    //hitEffVsDelayBPix[i]->setAxisRange(0, 1.0, "Y");
    hitEffVsDelayBPix[i]->setColor("Line", 1);
    hitEffVsDelayBPix[i]->Draw("[1],[2],[3],[4],[5],[6],[7],[8];PE",3);
    hitEffVsDelayBPix[i]->Write();
    delete hitEffVsDelayBPix[i];
  }

  for (size_t i=0; i<hitEffVsDelayFPix.size(); i++) {
    hitEffVsDelayFPix[i]->efficiency();
    //hitEffVsDelayFPix[i]->setAxisRange(0, 1.0, "Y");
    hitEffVsDelayFPix[i]->setColor("Line", 1);
    hitEffVsDelayFPix[i]->Draw("Disk1_PRT1,Disk2_PRT1,Disk1_PRT2,Disk2_PRT2,"
			       "Disk1_PRT3,Disk2_PRT3,Disk1_PRT4,Disk2_PRT4;PE",3);
    hitEffVsDelayFPix[i]->Write();
    delete hitEffVsDelayFPix[i];
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

  int istep=0;
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    drawAll<<"["<<it->first<<"]|delay "<<it->second<<" ns";
    drawAll_den<<"["<<it->first<<"].den|delay "<<it->second<<" ns";
    if (istep<delays.size()-1) {
      drawAll<<",";
      drawAll_den<<",";
    }
    istep++;
  }
  std::cout<<"Draw all: "<<drawAll.str()<<std::endl;
  std::cout<<"Draw all den: "<<drawAll_den.str()<<std::endl;

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
  deb::Plot<TH1F> clusMeanSizeVsDelay(600, 600, "%s_clusMeanSizeVsDelay\\TDR", plotType.data());
  clusMeanSizeVsDelay.add(n_delay, min_delay, max_delay, "BPix;Mean cluster size in the Barrel"
			  " pixel;Delay [ns];Size");
  clusMeanSizeVsDelay.add(n_delay, min_delay, max_delay, "FPix;Mean cluster size in the Forward"
			  " pixel;Delay [ns];Size");
  clusMeanSizeVsDelay.efficiency();
 

  std::vector<deb::Plot<TH1F>* > clusMeanSizeVsDelayBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t j=0; j<2; j++) {
      deb::Plot<TH1F>* p = new deb::Plot<TH1F>(600,600,"%s_clusMeanSizeVsDelayBPix_%s_PRT%d\\TDR",
					       plotType.data(), partitions[i].data(), j+1);
      for (size_t sec=0; sec<8; sec++) {
	p->add(n_delay, min_delay, max_delay, "[%d];Mean cluster size in %s PRT%d SEC%d;"
	       "Delay [ns];Size", sec+1, partitions[i].data(), j+1, sec+1);
      }
      p->efficiency();
      clusMeanSizeVsDelayBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F>* > clusMeanSizeVsDelayFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F>* p = new deb::Plot<TH1F> (600,600,"%s_clusMeanSizeVsDelayFPix_%s\\TDR",
					      plotType.data(), partitions[i].data());
    for (size_t j=0; j<2; j++) {
      for (size_t prt=0; prt<4; prt++) {
	p->add(n_delay, min_delay, max_delay, "Disk%d_PRT%d;Mean cluster size in %s Disk%d PRT%d;"
	       "Delay [ns];Size", j+1, prt+1, partitions[i].data(), j+1, prt+1);
      }
    }
    p->efficiency();
    clusMeanSizeVsDelayFPix.push_back(p);
  }
 
  // 2. Average cluster charge
  deb::Plot<TH1F> clusMeanChargeVsDelay(600, 600,"%s_clusMeanChargeVsDelay\\TDR", plotType.data());
  clusMeanChargeVsDelay.add(n_delay, min_delay, max_delay, "BPix;Mean cluster charge in the Barrel"
			    " pixel;Delay [ns];Charge [ke]");
  clusMeanChargeVsDelay.add(n_delay, min_delay, max_delay,"FPix;Mean cluster charge in the Forward"
			    " pixel;Delay [ns];Charge [ke]");
  clusMeanChargeVsDelay.efficiency();
 

  std::vector<deb::Plot<TH1F>* > clusMeanChargeVsDelayBPix;
  for (size_t i=0; i<partitions.size(); i++) {
    for (size_t j=0; j<2; j++) {
      deb::Plot<TH1F>* p =new deb::Plot<TH1F>(600,600,"%s_clusMeanChargeVsDelayBPix_%s_PRT%d\\TDR",
					       plotType.data(), partitions[i].data(), j+1);
      for (size_t sec=0; sec<8; sec++) {
	p->add(n_delay, min_delay, max_delay, "[%d];Mean cluster charge in %s PRT%d SEC%d;"
	       "Delay [ns];Charge [ke]", sec+1, partitions[i].data(), j+1, sec+1);
      }
      p->efficiency();
      clusMeanChargeVsDelayBPix.push_back(p);
    }
  }

  std::vector<deb::Plot<TH1F>* > clusMeanChargeVsDelayFPix;
  for (size_t i=0; i<partitions.size(); i++) {
    deb::Plot<TH1F>* p = new deb::Plot<TH1F> (600,600,"%s_clusMeanChargeVsDelayFPix_%s\\TDR",
					      plotType.data(), partitions[i].data());
    for (size_t j=0; j<2; j++) {
      for (size_t prt=0; prt<4; prt++) {
	p->add(n_delay, min_delay, max_delay,"Disk%d_PRT%d;Mean cluster charge in %s Disk%d PRT%d;"
	       "Delay [ns];Charge [ke]", j+1, prt+1, partitions[i].data(), j+1, prt+1);
      }
    }
    p->efficiency();
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

    std::string partition=clust.mod_on.shell();
    int partnum=clust.mod_on.shell_num();

    // 1. Mean cluster size
    if (clust.charge>12.) {
      if (clust.mod_on.det==0) {
	clusMeanSizeVsDelay["BPix"].num()->Fill(delay, clust.size);
	clusMeanSizeVsDelay["BPix"].den()->Fill(delay);
	(*clusMeanSizeVsDelayBPix[partnum*2+clust.mod_on.prt-1])(clust.mod_on.sec).
	  num()->Fill(delay, clust.size);
	(*clusMeanSizeVsDelayBPix[partnum*2+clust.mod_on.prt-1])(clust.mod_on.sec).
	  den()->Fill(delay);
      } else if (clust.mod_on.det==1) {
	clusMeanSizeVsDelay["FPix"].num()->Fill(delay, clust.size);
	clusMeanSizeVsDelay["FPix"].den()->Fill(delay);
	(*clusMeanSizeVsDelayFPix[partnum])("Disk%d_PRT%d", abs(clust.mod_on.disk), 
					    clust.mod_on.prt).num()->Fill(delay, clust.size);
	(*clusMeanSizeVsDelayFPix[partnum])("Disk%d_PRT%d", abs(clust.mod_on.disk), 
					    clust.mod_on.prt).den()->Fill(delay);
      } 
    }


    // 2. Mean cluster charge
    if (clust.size>1) {
      if (clust.mod_on.det==0) {
	clusMeanChargeVsDelay["BPix"].num()->Fill(delay, clust.charge);
	clusMeanChargeVsDelay["BPix"].den()->Fill(delay);
	(*clusMeanChargeVsDelayBPix[partnum*2+clust.mod_on.prt-1])(clust.mod_on.sec).
	  num()->Fill(delay, clust.charge);
	(*clusMeanChargeVsDelayBPix[partnum*2+clust.mod_on.prt-1])(clust.mod_on.sec).
	  den()->Fill(delay);
      } else if (clust.mod_on.det==1) {
	clusMeanChargeVsDelay["FPix"].num()->Fill(delay, clust.charge);
	clusMeanChargeVsDelay["FPix"].den()->Fill(delay);
	(*clusMeanChargeVsDelayFPix[partnum])("Disk%d_PRT%d", abs(clust.mod_on.disk), 
					      clust.mod_on.prt).num()->Fill(delay, clust.charge);
	(*clusMeanChargeVsDelayFPix[partnum])("Disk%d_PRT%d", abs(clust.mod_on.disk), 
					      clust.mod_on.prt).den()->Fill(delay);
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
  //clusMeanSizeVsDelay.setAxisRange(0, 1.0, "Y");
  clusMeanSizeVsDelay.setColor("Line", 1);
  clusMeanSizeVsDelay.Draw("FPix|FPix, BPix|BPix;overlayPE");
  clusMeanSizeVsDelay.Write();

  for (size_t i=0; i<clusMeanSizeVsDelayBPix.size(); i++) {
    clusMeanSizeVsDelayBPix[i]->efficiency();
    //clusMeanSizeVsDelayBPix[i]->setAxisRange(0, 1.0, "Y");
    clusMeanSizeVsDelayBPix[i]->setColor("Line", 1);
    clusMeanSizeVsDelayBPix[i]->Draw("[1],[2],[3],[4],[5],[6],[7],[8];PE",3);
    clusMeanSizeVsDelayBPix[i]->Write();
    delete clusMeanSizeVsDelayBPix[i];
  }

  for (size_t i=0; i<clusMeanSizeVsDelayFPix.size(); i++) {
    clusMeanSizeVsDelayFPix[i]->efficiency();
    //clusMeanSizeVsDelayFPix[i]->setAxisRange(0, 1.0, "Y");
    clusMeanSizeVsDelayFPix[i]->setColor("Line", 1);
    clusMeanSizeVsDelayFPix[i]->Draw("Disk1_PRT1,Disk2_PRT1,Disk1_PRT2,Disk2_PRT2,"
			       "Disk1_PRT3,Disk2_PRT3,Disk1_PRT4,Disk2_PRT4;PE",3);
    clusMeanSizeVsDelayFPix[i]->Write();
    delete clusMeanSizeVsDelayFPix[i];
  }


  // 2. Mean cluster charge
  clusMeanChargeVsDelay.efficiency();
  //clusMeanChargeVsDelay.setAxisRange(0, 1.0, "Y");
  clusMeanChargeVsDelay.setColor("Line", 1);
  clusMeanChargeVsDelay.Draw("FPix|FPix, BPix|BPix;overlayPE");
  clusMeanChargeVsDelay.Write();

  for (size_t i=0; i<clusMeanChargeVsDelayBPix.size(); i++) {
    clusMeanChargeVsDelayBPix[i]->efficiency();
    //clusMeanChargeVsDelayBPix[i]->setAxisRange(0, 1.0, "Y");
    clusMeanChargeVsDelayBPix[i]->setColor("Line", 1);
    clusMeanChargeVsDelayBPix[i]->Draw("[1],[2],[3],[4],[5],[6],[7],[8];PE",3);
    clusMeanChargeVsDelayBPix[i]->Write();
    delete clusMeanChargeVsDelayBPix[i];
  }

  for (size_t i=0; i<clusMeanChargeVsDelayFPix.size(); i++) {
    clusMeanChargeVsDelayFPix[i]->efficiency();
    //clusMeanChargeVsDelayFPix[i]->setAxisRange(0, 1.0, "Y");
    clusMeanChargeVsDelayFPix[i]->setColor("Line", 1);
    clusMeanChargeVsDelayFPix[i]->Draw("Disk1_PRT1,Disk2_PRT1,Disk1_PRT2,Disk2_PRT2,"
			       "Disk1_PRT3,Disk2_PRT3,Disk1_PRT4,Disk2_PRT4;PE",3);
    clusMeanChargeVsDelayFPix[i]->Write();
    delete clusMeanChargeVsDelayFPix[i];
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


  std::ostringstream addname;
  TChain *trajTree=new TChain("trajTree");
  addname.str("");
  addname << infilename << "/ctfNtuple/trajTree";
  trajTree->Add(addname.str().data());
  std::cout << "Collecting files " << addname.str().data() << std::endl;

  TChain *trackTree=new TChain("trackTree");
  addname.str("");
  addname << infilename << "/ctfNtuple/trackTree";
  trackTree->Add(addname.str().data());

  TChain *clustTree=new TChain("clustTree");
  addname.str("");
  addname << infilename << "/ctfNtuple/clustTree";
  clustTree->Add(addname.str().data());

  TFile outfile(outfilename.data(), "RECREATE");

  doRecHitPlots(trajTree, 1);
  //doTrackPlots();
  doClusterPlots(clustTree, 1);

  //outfile.Write();
  outfile.Close();

  delete trajTree;
  delete trackTree;
  delete clustTree;

  return 1;   
}

#endif
