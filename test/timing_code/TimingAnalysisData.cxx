
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



void setError(TH1F* h, TH1F* num, TH1F* den) {

  TH1F *unit=(TH1F*)h->Clone("unit");
  for(size_t i=0; i<=unit->GetNbinsX(); i++) unit->SetBinContent(i, 1);
  TH1F *n_tilde=(TH1F*)den->Clone("n_tilde"); // n_tilde= N
  n_tilde->Add(unit, 4.); // n_tilde= N + 4
  TH1F *p_tilde=(TH1F*)num->Clone("p_tilde"); // p_tilde = X
  p_tilde->Add(unit, 2.); // p_tilde = X + 2
  p_tilde->Divide(n_tilde); // p_tilde=(X+2)/n_tilde
  
  TH1F* err=(TH1F*)h->Clone("err"); // error
  err->Multiply(p_tilde, p_tilde, -3.84);
  err->Add(p_tilde, 3.84);
  err->Divide(n_tilde);
  for(size_t i=0; i<=err->GetNbinsX(); i++) {
    if (den->GetBinContent(i)!=0.) {
      h->SetBinError(i, sqrt(err->GetBinContent(i)));
    } else {
      h->SetBinError(i, 0.);
    }
  }
  
  delete unit;
  delete p_tilde;
  delete n_tilde;
  delete err;
}


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



std::pair<int,int> getStepDelayCollision(int run, int orbitnumber, int ls) {
  if (orbitnumber==-1) { // SIMULATION
    std::pair<int,int> ret(0,0);
    return ret;
  }
  int step=NOVAL_I;
  int delay=NOVAL_I;
  if (run==123592) { delay=0; step=3; }
//   if (run==123596) {
//     if (orbitnumber>=0 && orbitnumber<771931)               { delay=0; step=0; }
//     else if (orbitnumber>=771931   && orbitnumber<3627623)  { delay=0; step=0; }
//     else if (orbitnumber>=3627623  && orbitnumber<10615967) { delay=-6; step=1; }
//     else if (orbitnumber>=10615967 && orbitnumber<15496826) { delay=6; step=2; }
//     else if (orbitnumber>=15496826 && orbitnumber<21906239) { delay=12; step=3; }
//     else if (orbitnumber>=21906239 && orbitnumber<27497030) { delay=12; step=3; }
//     else if (orbitnumber>=27497030 && orbitnumber<69626910) { delay=6; step=2; }
//     else if (orbitnumber>=69626910 && orbitnumber<71554771) { delay=18; step=4; }
//     else if (orbitnumber>=71554771)                         { delay=6; step=2; }
//   }
  if (run==123596) {
    if (orbitnumber>=0 && orbitnumber<771931)               { delay=0; step=3; }
    else if (orbitnumber>=771931   && orbitnumber<3200000)  { delay=0; step=3; }
    else if (orbitnumber>=3200000  && orbitnumber<9800000)  { delay=-6; step=1; }
    else if (orbitnumber>=9800000 && orbitnumber<16500000)  { delay=6; step=4; }
    else if (orbitnumber>=16500000 && orbitnumber<22500000) { delay=12; step=6; }
    else if (orbitnumber>=22500000 && orbitnumber<28000000) { delay=12; step=6; } // wrong key set
    else if (orbitnumber>=28000000 && orbitnumber<69500000) { delay=6; step=4; }
    else if (orbitnumber>=69500000 && orbitnumber<71800000) { delay=18; step=8; }
    else if (orbitnumber>=71800000)                         { delay=6; step=4; }
  }
  if (run>=123603&&run<123970) { delay=6; step=4; }
  if (run==123970) {
    if (orbitnumber>=0 && orbitnumber<9500000)              { delay=12; step=6; }
    else { delay=0; step=3; }
  }
  if (run==123976) { delay=12; step=6; }
  if (run==123977) {
    // 123977   0-     +6ns,key=23686   -  1500000
    // 123977   1-8   +12ns,key=23694   - 10500000
    // 123977  10-19    0ns,key=223711  - 20900000
    // 123977  20-27  +15ns,key=223712  - 30000000
    // 123977  29-36   +9ns,key=223713  - 38800000
    // 123977  37-44   -3ns,key=223714  - 47800000
    // 123977  46-47    0ns,WBC154,key23688 - 49800000
    // 123977  47-49    0ns,WBC156,key23690 - 52500000
    // 123977  50-     +6ns, key=23686 BACK TO NORMAL OPERATIONS - END
    if (orbitnumber>=0 && orbitnumber<1500000)              { delay=6; step=4; }
    else if (orbitnumber>=1500000  && orbitnumber<10500000) { delay=12; step=6; }
    else if (orbitnumber>=10500000 && orbitnumber<20900000) { delay=0; step=3; }
    else if (orbitnumber>=20900000 && orbitnumber<30000000) { delay=15; step=7; }
    else if (orbitnumber>=30000000 && orbitnumber<38800000) { delay=9; step=5; }
    else if (orbitnumber>=38800000 && orbitnumber<47800000) { delay=-3; step=2; }
    else if (orbitnumber>=47800000 && orbitnumber<49800000) { delay=25; step=9; }
    else if (orbitnumber>=49800000 && orbitnumber<52500000) { delay=-25; step=0; }
    else if (orbitnumber>=52500000)                         { delay=6; step=4; }
  }
  if (run>=123978) { delay=6; step=4;}

  // Collisions 2010:
// RUN 132476
// DELAY       LS_START    LS_END     TIME_START TIME_END    CONFIG_KEY        STEP:
// DEFAULT               0         21        0:18       0:15      34214         
//         10           29         53        0:29       0:28      34174         
//         12           58 END_OF_RUN             END_OF_RUN      34143
// RUN 132477
//         14            6         33        0:48       0:58      34180
//         16           36         62        0:59       1:09      34205
//           8          65         90        1:10       1:20      34171
//           4          94        119        2:21       1:31      34195
//           2        122         147        1:32       1:42      34220
//          -2        150         175        1:43       1:53      34076
//          -4        180         224        1:55       2:12      34105
//         18         237         242        2:17       2:19      34166
//          -6        245         249        2:20       2:22      34172
//         20         252         261        2:23       2:26      34221
//           0        264         291        2:28       2:38      34123
//          -8        293         298        2:39       2:41      34077
// 0_WBC154           301         308        2:42       2:45      34128
// 0_WBC156           312         317        2:46       2:48      34184
//           6        321 END_OF_RUN         2:50 END_OF_RUN      34167

  if (run==132476) {
    if (ls>=0 && ls<21) {           delay=6;   step=9;
    } else if (ls>=29 && ls<53) {   delay=10;  step=11; 
    } else if (ls>=58) {            delay=12;  step=12;
    } else                        { delay=NOVAL_I;  step=NOVAL_I; }
  }
  if (run==132477) {
    if (ls>=6 && ls<33) {           delay=14;  step=13;
    } else if (ls>=36 && ls<62) {   delay=16;  step=14;
    } else if (ls>=65 && ls<90) {   delay=8;   step=10;
    } else if (ls>=94 && ls<119) {  delay=4;   step=8;
    } else if (ls>=122 && ls<147) { delay=2;   step=7;
    } else if (ls>=150 && ls<175) { delay=-2;  step=5;
    } else if (ls>=180 && ls<224) { delay=-4;  step=4;
    } else if (ls>=237 && ls<242) { delay=18;  step=15;
    } else if (ls>=245 && ls<249) { delay=-6;  step=3;
    } else if (ls>=252 && ls<261) { delay=20;  step=16;
    } else if (ls>=264 && ls<291) { delay=0;   step=6;
    } else if (ls>=293 && ls<298) { delay=-8;  step=2;
    } else if (ls>=301 && ls<308) { delay=25;  step=17; delay=NOVAL_I;  step=NOVAL_I;
    } else if (ls>=312 && ls<317) { delay=-25; step=1;  delay=NOVAL_I;  step=NOVAL_I;
    } else if (ls>=321)           { delay=6;   step=9;
    } else                        { delay=NOVAL_I;  step=NOVAL_I; }
  }


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
  float min_delay=0, max_delay=0;
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
    trajTree->GetEntry(i); //if (evt.run<123970 || evt.run>123977) continue;
    //if (evt.bx!=51 && evt.bx!=2824 && evt.bx!=-1) continue;

    std::string fname=trajTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.run, evt.orb, evt.ls);
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
  for (int i=1; i<int(tempDelayBins.size()); i++) {
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
  //hitEffVsDelay.add(n_delay, min_delay, max_delay, "BPix;Hit efficiency in the Barrel"
  //		    " pixel;Clock phase [ns];Efficiency");
  hitEffVsDelay.add(n_delay, min_delay, max_delay, "BPix_L1!BPix Layer 1;"
		    "Hit efficiency in the Barrel pixel Layer 1;Clock phase [ns];Efficiency"); {
    hitEffVsDelay["BPix_L1"].addAux("valid");
    hitEffVsDelay["BPix_L1"].addAux("validprop");
    hitEffVsDelay["BPix_L1"].addAux("miss");
  }

  hitEffVsDelay.add(n_delay, min_delay, max_delay, "BPix_L2!BPix Layer 2;"
		    "Hit efficiency in the Barrel pixel Layer 2;Clock phase [ns];Efficiency"); {
    hitEffVsDelay["BPix_L2"].addAux("miss");
  }

  hitEffVsDelay.add(n_delay, min_delay, max_delay, "BPix_L3!BPix Layer 3;Hit efficiency in Barrel"
		    " pixel Layer 3;Clock phase [ns];Efficiency");

  hitEffVsDelay.add(n_delay, min_delay, max_delay, "FPix_D1!FPix Disk 1;"
		    "Hit efficiency in the Forward pixel Disk 1;Clock phase [ns];Efficiency");
  hitEffVsDelay.add(n_delay, min_delay, max_delay, "FPix_D2!FPix Disk 2;"
		    "Hit efficiency in the Forward pixel Disk 2;Clock phase [ns];Efficiency");
  hitEffVsDelay.efficiency();
  hitEffVsDelay.print();


  deb::Plot<TH1F> hitEffVsDelayMod(600, 600, "%s_hitEffVsDelayMod\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    hitEffVsDelayMod.add(210, -10.5, 10.5, // 20
			 "BPix_L1_%d!delay %d ns;Hit efficiency of Barrel "
			 "pixel modules Layer 1;Ladders (subdiv. modules from -4 to 4) ;Efficiency",it->first,it->second);
    hitEffVsDelayMod("BPix_L1_%d", it->first).addAux("valid");
    hitEffVsDelayMod("BPix_L1_%d", it->first).addAux("validprop");
    hitEffVsDelayMod("BPix_L1_%d", it->first).addAux("miss");
    
    hitEffVsDelayMod.add(330, -16.5, 16.5, // 32
			 "BPix_L2_%d!delay %d ns;Hit efficiency of Barrel "
			 "pixel modules Layer 2;Ladders (subdiv. modules from -4 to 4) ;Efficiency",it->first,it->second);
    hitEffVsDelayMod("BPix_L2_%d", it->first).addAux("miss");
	
    hitEffVsDelayMod.add(440, -22.5, 22.5, // 44
			 "BPix_L3_%d!delay %d ns;Hit efficiency of Barrel "
			 "pixel modules Layer 3;Ladders (subdiv. modules from -4 to 4) ;Efficiency",it->first,it->second);
  }
  hitEffVsDelayMod.efficiency();
  hitEffVsDelayMod.print();



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
  


  // Layer efficiency
  double N_l1_validprop=0;
  double N_l1_valid=0;
  double N_l1_miss=0;
  double N_l2_den=0;
  double N_l2_num=0;
  double N_l2_miss=0;
  double N_l3_den=0;
  double N_l3_num=0;
  double N_fpix_num[2][2];
  double N_fpix_den[2][2];
  for (int i=0; i<2; i++) for (int j=0; j<2; j++) {
    N_fpix_num[i][j]=0;
    N_fpix_den[i][j]=0;
  }


  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //
  std::cout<< "Filling histos\n";
  for (long i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i); 

    //if (evt.run<123970 || evt.run>123977) continue;
    //if (evt.bx!=51 && evt.bx!=2824 && evt.bx!=-1) continue;

    //     if (! ( ((trajmeas.trk.fpix[0]!=0 && trajmeas.trk.fpix[1]!=0) ||
    // 	     (trajmeas.trk.bpix[0]!=0 && trajmeas.trk.bpix[1]!=0 && trajmeas.trk.bpix[2]!=0)) 
    // 	    && trajmeas.trk.strip>10) )
    //       continue;
    
   
    // Event and track selection

    if ( !(evt.vtxndof>4 && 
	   evt.nvtx>=1 &&
	   trajmeas.trk.fromVtx==1 &&
	   trajmeas.trk.strip>10 &&
	   trajmeas.trk.pt>0.9) 
	 ) continue;     
    

    // Detector coordinates

    int validrog=(trajmeas.mod_on.sec!=-9999) ? 1 : 0;
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


    //
    // Put here the overall efficiency for good runs
    //
    // if (evt.good==1) blah blah


    // Delay step and delay
    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.run, evt.orb, evt.ls);
    }
    int step=stepDelay.first;
    int delay=stepDelay.second;

    //
    // From here on this is only for the delay scan :
    //
    if (step==NOVAL_I || delay==NOVAL_I) continue;


    std::ostringstream ss;
    ss << "_[" << step << "]";
    std::string sstep=ss.str();


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
    if ( (trajmeas.validhit==1 || trajmeas.missing==1) ) {
      
      if (trajmeas.mod_on.det==0) { // BPix

	// hit eff vs delay per layer
	if (trajmeas.mod.layer==1 && 
	    fabs(trajmeas.ly)<3.0 && 
	    ((trajmeas.mod.half==0&&fabs(trajmeas.lx)<0.55)||
	     (trajmeas.mod.half==1&&fabs(trajmeas.lx)<0.35))
	    ) {
	  
	  deb::Histogram<TH1F>& h = hitEffVsDelay["BPix_L1"];
	  deb::Histogram<TH1F>& m = hitEffVsDelayMod("BPix_L1_%d", step);
	  float m_w=trajmeas.mod_on.ladder+trajmeas.mod_on.module/10.+0.05;
	  if (trajmeas.mod_on.module>0) m_w-=0.1;
	  if (trajmeas.validhit==1) {
	    h.aux("valid")->Fill(delay);
	    m.aux("valid")->Fill(m_w);
	    if (delay==6) N_l1_valid++;
	    if (trajmeas.lxmatch!=NOVAL_F) {
	      h.aux("validprop")->Fill(delay);
	      m.aux("validprop")->Fill(m_w);
	      if (delay==6) N_l1_validprop++;
	    }
	  } else {
	    h.aux("miss")->Fill(delay);
	    m.aux("miss")->Fill(m_w);
	    if (delay==6) N_l1_miss++;
	  }

	}
	if (trajmeas.mod.layer==2 &&
	    fabs(trajmeas.ly)<3.2 && 
	    ((trajmeas.mod.half==0&&fabs(trajmeas.lx)<0.6)||
	     (trajmeas.mod.half==1&&fabs(trajmeas.lx)<0.35))
	    ) {
	  
	  deb::Histogram<TH1F>& h = hitEffVsDelay["BPix_L2"];
	  deb::Histogram<TH1F>& m = hitEffVsDelayMod("BPix_L2_%d", step);
	  float m_w=trajmeas.mod_on.ladder+trajmeas.mod_on.module/10.+0.05;
	  if (trajmeas.mod_on.module>0) m_w-=0.1;
	  if (trajmeas.validhit==1)  {
	    h.num()->Fill(delay);
	    m.num()->Fill(m_w);
	    if (delay==6) N_l2_num++;
	  } else {
	    h.aux("miss")->Fill(delay);
	    m.aux("miss")->Fill(m_w);
	    if (delay==6) N_l2_miss++;
	  }
	  h.den()->Fill(delay);
	  m.den()->Fill(m_w);
	  if (delay==6) N_l2_den++;
	  
	}
	if (trajmeas.mod.layer==3 &&
	    fabs(trajmeas.ly)<3.1 && 
	    ((trajmeas.mod.half==0&&fabs(trajmeas.lx)<0.6)||
	     (trajmeas.mod.half==1&&fabs(trajmeas.lx)<0.35))
	    ) {
	  
	  deb::Histogram<TH1F>& h = hitEffVsDelay["BPix_L3"];
	  deb::Histogram<TH1F>& m = hitEffVsDelayMod("BPix_L3_%d", step);
	  float m_w=trajmeas.mod_on.ladder+trajmeas.mod_on.module/10.+0.05;
	  if (trajmeas.mod_on.module>0) m_w-=0.1;
	  if (trajmeas.validhit==1) {
	    h.num()->Fill(delay);
	    m.num()->Fill(m_w);
	    if (delay==6) N_l3_num++;
	  }
	  h.den()->Fill(delay);
	  m.den()->Fill(m_w);
	  if (delay==6) N_l3_den++;
	}
	
	if (validrog==1) hitEffVsDelayBPixShell[shell].den()->Fill(delay);
	if (validrog==1) hitEffVsDelayBPix[group][rog].den()->Fill(delay);
	if (trajmeas.validhit) {
	  //hitEffVsDelay[detector].num()->Fill(delay);
	  if (validrog==1) hitEffVsDelayBPixShell[shell].num()->Fill(delay);
	  if (validrog==1) hitEffVsDelayBPix[group][rog].num()->Fill(delay);
	}

      } else { // FPix
	if (trajmeas.mod.disk==1) {
	  hitEffVsDelay["FPix_D1"].den()->Fill(delay);
	} else {
	  hitEffVsDelay["FPix_D2"].den()->Fill(delay);
	}

	if (validrog==1) hitEffVsDelayFPixShell[shell].den()->Fill(delay);
	if (validrog==1) hitEffVsDelayFPix[group][rog].den()->Fill(delay);
	if (delay==6) N_fpix_den[trajmeas.mod.side-1][trajmeas.mod.disk-1]+=1;

	if (trajmeas.validhit) {
	  
	  if (trajmeas.mod.disk==1) {
	    hitEffVsDelay["FPix_D1"].num()->Fill(delay);
	  } else {
	    hitEffVsDelay["FPix_D2"].num()->Fill(delay);
	  }

	  if (validrog==1) hitEffVsDelayFPixShell[shell].num()->Fill(delay);
	  if (validrog==1) hitEffVsDelayFPix[group][rog].num()->Fill(delay);
	  if (delay==6) N_fpix_num[trajmeas.mod.side-1][trajmeas.mod.disk-1]+=1;
	}
      }

    }
    // ---------------------------------------


    // 3. Cluster charge
    if (trajmeas.validhit==1 && validrog==1) {
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
    if ((trajmeas.validhit==1||trajmeas.missing==1) && validrog==1) {
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

  // 0. Hit Efficiency vs layers
  deb::Plot<TH1F> layerEff(600, 600, "%s_layerEff\nTDR", plotType.data());
  layerEff.add(7,0.5,7.5,"layerEff;Rec Hit efficiency in the Pixels;;Efficiency");
  layerEff.efficiency();


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
  {
    deb::Histogram<TH1F>& h=hitEffVsDelay["BPix_L1"];
    h.num()->Add(h.aux("valid"));
    h.den()->Add(h.aux("valid"));
    h.aux("validprop")->Divide(h.aux("valid"));
    h.aux("miss")->Divide(h.aux("validprop"));
    h.den()->Add(h.aux("miss"));

    double N_l1_num=N_l1_valid;
    double N_l1_den=N_l1_valid;
    N_l1_validprop/=N_l1_valid;
    N_l1_miss/=N_l1_validprop;
    N_l1_den+=N_l1_miss;

    for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
      std::vector<deb::Plot<TH1F>::iterator> its=hitEffVsDelayMod.search("BPix_L1_%d", it->first);
      if (its.size()==0) printf("BPix_L1_%d not found\n", it->first);
      deb::Histogram<TH1F>& m = hitEffVsDelayMod("BPix_L1_%d", it->first);
      m.num()->Add(m.aux("valid"));
      m.den()->Add(m.aux("valid"));
      m.aux("validprop")->Divide(m.aux("valid"));
      m.aux("miss")->Divide(m.aux("validprop"));
      m.den()->Add(m.aux("miss"));
    }

    layerEff["layerEff"].num()->Fill (1, N_l1_num);
    layerEff["layerEff"].den()->Fill (1, N_l1_den);
  }

  {
    deb::Histogram<TH1F>& h=hitEffVsDelay["BPix_L2"];
    h.den()->Add(h.num(), (h.aux("miss")->Integral()*0.1)/h.num()->Integral());

    for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
      deb::Histogram<TH1F>& m = hitEffVsDelayMod("BPix_L2_%d", it->first);
      m.den()->Add(m.num(), (m.aux("miss")->Integral()*0.1)/m.num()->Integral());
    }

    N_l2_den+=N_l2_miss*0.1;
    layerEff["layerEff"].num()->Fill (2, N_l2_num);
    layerEff["layerEff"].den()->Fill (2, N_l2_den);
  }
  hitEffVsDelay.efficiency();
  hitEffVsDelayMod.efficiency();

  layerEff["layerEff"].num()->Fill (3, N_l3_num);
  layerEff["layerEff"].den()->Fill (3, N_l3_den);
  layerEff["layerEff"].num()->Fill (4, N_fpix_num[0][0]);
  layerEff["layerEff"].num()->Fill (5, N_fpix_num[0][1]);
  layerEff["layerEff"].num()->Fill (6, N_fpix_num[1][0]);
  layerEff["layerEff"].num()->Fill (7, N_fpix_num[1][1]);
  layerEff["layerEff"].den()->Fill (4, N_fpix_den[0][0]);
  layerEff["layerEff"].den()->Fill (5, N_fpix_den[0][1]);
  layerEff["layerEff"].den()->Fill (6, N_fpix_den[1][0]);
  layerEff["layerEff"].den()->Fill (7, N_fpix_den[1][1]);
  layerEff.efficiency();

  setError(&layerEff["layerEff"], layerEff["layerEff"].num(), layerEff["layerEff"].den());

  hitEffVsDelay.setAxisRange(".*", 0.4, 1.04, "Y");
  hitEffVsDelay.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelay.setColor("FPix_D2", "LineMarker", 6);
  hitEffVsDelay.Draw("BPix_L[1-3]!&FPix_D[1-2]!;overlay9PE");
  hitEffVsDelay.Write();

  hitEffVsDelayMod.setAxisRange(".*", 0.8, 1.04, "Y");
  hitEffVsDelayMod.setColor("BPix_L1_8", "LineMarker", 2);
  hitEffVsDelayMod.setColor("BPix_L1_9", "LineMarker", 1);
  hitEffVsDelayMod.setColor("BPix_L1_10", "LineMarker", 3);
  hitEffVsDelayMod.setColor("BPix_L2_8", "LineMarker", 2);
  hitEffVsDelayMod.setColor("BPix_L2_9", "LineMarker", 1);
  hitEffVsDelayMod.setColor("BPix_L2_10", "LineMarker", 3);
  hitEffVsDelayMod.setColor("BPix_L3_8", "LineMarker", 2);
  hitEffVsDelayMod.setColor("BPix_L3_9", "LineMarker", 1);
  hitEffVsDelayMod.setColor("BPix_L3_10", "LineMarker", 3);
  hitEffVsDelayMod.setStyle("BPix_L1.*", "Marker", 6);
  hitEffVsDelayMod.setStyle("BPix_L2.*", "Marker", 6);
  hitEffVsDelayMod.setStyle("BPix_L3.*", "Marker", 6);
  //hitEffVsDelayMod.Draw("BPix_L1_[8,9,10]!&BPix_L1_9;overlayPE",1);
  hitEffVsDelayMod.Draw("BPix_L1_8!&BPix_L1_9!&BPix_L1_10!&BPix_L1_9;overlayPE",1);
  hitEffVsDelayMod.Write();
  hitEffVsDelayMod.Draw("BPix_L2_8!&BPix_L2_9!&BPix_L2_10!&BPix_L2_9;overlayPE",1);
  //hitEffVsDelayMod.Draw("BPix_L2_[8,9,10]!&BPix_L2_9;overlayPE",1);
  hitEffVsDelayMod.canvas()->Write();
  hitEffVsDelayMod.Draw("BPix_L3_8!&BPix_L3_9!&BPix_L3_10!&BPix_L3_9;overlayPE",1);
  //hitEffVsDelayMod.Draw("BPix_L3_[8,9,10]!&BPix_L3_9;overlayPE",1);
  hitEffVsDelayMod.canvas()->Write();
  
  layerEff.setAxisRange(".*", 0.8, 1.05, "Y");
  layerEff.setStyle(".*", "Marker", 20);
  layerEff["layerEff"].SetMarkerSize(1);
  layerEff["layerEff"].GetXaxis()->SetBinLabel(1, "BPix Layer 1");
  layerEff["layerEff"].GetXaxis()->SetBinLabel(2, "BPix Layer 2");
  layerEff["layerEff"].GetXaxis()->SetBinLabel(3, "BPix Layer 3");
  layerEff["layerEff"].GetXaxis()->SetBinLabel(4, "FPix Side 1 Disk 1");
  layerEff["layerEff"].GetXaxis()->SetBinLabel(5, "FPix Side 1 Disk 2");
  layerEff["layerEff"].GetXaxis()->SetBinLabel(6, "FPix Side 2 Disk 1");
  layerEff["layerEff"].GetXaxis()->SetBinLabel(7, "FPix Side 2 Disk 2");
  layerEff.Draw("layerEff;overlayPEX0");
  layerEff.canvas()->GetPad(1)->SetRightMargin(0.159);
  layerEff.Write();


  hitEffVsDelayBPixShell.efficiency();
  hitEffVsDelayBPixShell.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelayBPixShell.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelayBPixShell.Draw("BpO!!& BpI!!& BmO!!& BmI!!;overlayPE",1);
  hitEffVsDelayBPixShell.Write();

  hitEffVsDelayFPixShell.efficiency();
  hitEffVsDelayFPixShell.setAxisRange(".*", 0, 1.0, "Y");
  hitEffVsDelayFPixShell.setColor(".*", "LineMarker", 1, 1);
  hitEffVsDelayFPixShell.Draw("BpO!!& BpI!!& BmO!!& BmI!!;overlayPE",1);
  hitEffVsDelayFPixShell.Write();

  for (size_t i=0; i<hitEffVsDelayBPix.size(); i++) {
    hitEffVsDelayBPix[i].efficiency();
    hitEffVsDelayBPix[i].setAxisRange(".*", 0.4, 1.05, "Y");
    hitEffVsDelayBPix[i].setColor(".*", "LineMarker", 1, 1);
    hitEffVsDelayBPix[i].Draw("SEC[1-8]!;overlayEL",1);
    hitEffVsDelayBPix[i].Write();
  }

  for (size_t i=0; i<hitEffVsDelayFPix.size(); i++) {
    hitEffVsDelayFPix[i].efficiency();
    hitEffVsDelayFPix[i].setAxisRange(".*", 0.4, 1.05, "Y");
    hitEffVsDelayFPix[i].setColor(".*", "LineMarker", 1, 1);
    hitEffVsDelayFPix[i].Draw("Disk[1,2]_PRT[1-4]!;overlayEL",1);
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
    trackTree->GetEntry(i); //if (evt.run<123970 || evt.run>123977) continue;
     //if (evt.bx!=51 && evt.bx!=2824 && evt.bx!=-1) continue;

   std::string fname=trackTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.run, evt.orb, evt.ls);
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
  Ntrk.efficiency();

  // 2. Npixhit/tracks at delays
  deb::Plot<TH1F> Nhits(600, 600, "%s_Nhits\nTDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    Nhits.add(30, -0.5, 29.5, "NStrip_[%d]!delay %d ns;Number of valid strip hits per track"
	      " at %d ns;N_{hits}", it->first, it->second, it->second);

    Nhits.add(15, -0.5, 14.5, "NPix_[%d]!delay %d ns;Number of valid pixel hits per track "
	      "crossing pixels at %d ns;N_{hits}", it->first, it->second, it->second);

    Nhits.add(15, -0.5, 14.5, "NBPix_[%d]!delay %d ns;Number of valid pixel hits per track "
	      "crossing Barrel pixels at %d ns;N_{hits}", it->first, it->second, it->second);

    Nhits.add(15, -0.5, 14.5, "NFPix_[%d]!delay %d ns;Number of valid pixel hits per track "
	      "crossing Forward pixels at %d ns;N_{hits}", it->first, it->second, it->second);

  }
  Nhits.efficiency();


  // 3. TrackD0, TrackDz, TrackPt, TrackChi2 at delays
  deb::Plot<TH1F> TrackParm(600, 600, "%s_TrackParm\nTDR", plotType.data());

  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    TrackParm.add(20, 0, 20., "pt_[%d]!delay %d ns;Transverse momentum of tracks at "
		  " %d ns;pt [GeV]", it->first, it->second, it->second);
    TrackParm.add(40, 0., 8., "chi2_[%d]!delay %d ns;Chi-square of tracks at "
		  " %d ns;chi2/ndof", it->first, it->second, it->second);
    TrackParm.add(400, -20, 20, "d0_[%d]!delay %d ns;Impact parameter of tracks at "
		  " %d ns;D0 [cm]", it->first, it->second, it->second);
    TrackParm.add(100, -200, 200, "dz_[%d]!delay %d ns;Delta Z of tracks at "
		  " %d ns;Dz [cm]", it->first, it->second, it->second);
  }
  TrackParm.efficiency();


  // ----------------------------------------------------------------------------------------------
  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //

  int prevevt=NOVAL_I;
  int prevevtDelay=NOVAL_I;
  int prevevtNumTracks=NOVAL_I;
  int prevevtNumPixTracks=NOVAL_I;

  for (long i=0; i<trackTree->GetEntries(); i++) {
    trackTree->GetEntry(i); //if (evt.run<123970 || evt.run>123977) continue;
    //if (evt.bx!=51 && evt.bx!=2824 && evt.bx!=-1) continue;


    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.run, evt.orb, evt.ls);
    }
    int step=stepDelay.first;
    int delay=stepDelay.second;
    if (step==NOVAL_I || delay==NOVAL_I) continue;
    std::ostringstream ss;
    ss << "_[" << step << "]";
    std::string sstep=ss.str();

    // 2. Npixhit/tracks at delays
    Nhits("NStrip_[%d]", step).Fill(track.strip);
    if (track.fpix[0]!=0 && track.fpix[1]!=0 && track.strip>=10) {
      Nhits("NFPix_[%d]", step).Fill(track.validfpix[0]+track.validfpix[1]);
    }
    if (track.bpix[0]!=0 && track.bpix[1]!=0 && track.bpix[2]!=0 && track.strip>10) {
      Nhits("NBPix_[%d]", step).Fill(track.validbpix[0]+track.validbpix[1]+
				     track.validbpix[2]);
    }
    if ( (track.fpix[0]!=0 && track.fpix[1]!=0) ||
	 (track.bpix[0]!=0 && track.bpix[1]!=0 && track.bpix[2]!=0) ) {
      Nhits("NPix_[%d]", step).Fill(track.pix); // track.pix?
    }

    // 3. TrackD0, TrackDz, TrackPt, TrackChi2 at delays
    if ( ((track.fpix[0]!=0 && track.fpix[1]!=0) ||
	  (track.bpix[0]!=0 && track.bpix[1]!=0 && track.bpix[2]!=0)) && track.strip>10) {
      TrackParm("pt_[%d]", step).Fill(track.pt);
      TrackParm("chi2_[%d]", step).Fill(track.chi2/track.ndof);
      TrackParm("d0_[%d]", step).Fill(track.d0);
      TrackParm("dz_[%d]", step).Fill(track.dz);
    }


    if (evt.evt!=prevevt) {
      
      if (prevevtDelay!=NOVAL_I) {
	// 1. Ntracks and Nhits/tracks at delays
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
  Ntrk.Draw("Ntrk_.*!;overlayPE;Number of tracks\n"
	    "Npixtrk_.*!;overlayPE;Number of tracks crossing pixels");
  Ntrk.Write();

  // 2. Nhits/tracks at delays
  Nhits.setColor("NStrip_.*!", "LineMarker", 1, 1);
  Nhits.setColor("NPix_.*!", "LineMarker", 1, 1);
  Nhits.setColor("NBPix_.*!", "LineMarker", 1, 1);
  Nhits.setColor("NFPix_.*!", "LineMarker", 1, 1);
  Nhits.scaleAreaTo("NStrip_.*!", 1.);
  Nhits.scaleAreaTo("NPix_.*!", 1.);
  Nhits.scaleAreaTo("NBPix_.*!", 1.);
  Nhits.scaleAreaTo("NFPix_.*!", 1.);
  Nhits.Draw("NStrip_.*!;overlayPE;Number of valid strip hits per track\n"
	     "NPix_.*!;overlayPE;Number of valid pixel hits per track crossing pixels\n"
	     "NBPix_.*!;overlayPE;Number of valid pixel hits per track crossing BPix (N_{strip hit}>10)\n"
	     "NFPix_.*!;overlayPE;Number of valid pixel hits per track crossing FPix (N_{strip hit}>10)\n", 2);
  Nhits.Write();

  // 3. TrackD0, TrackDz, TrackPt, TrackChi2 at delays
  TrackParm.setColor("pt_.*!", "LineMarker", 1, 1);
  TrackParm.setColor("chi2_.*!", "LineMarker", 1, 1);
  TrackParm.setColor("d0_.*!", "LineMarker", 1, 1);
  TrackParm.setColor("dz_.*!", "LineMarker", 1, 1);
  TrackParm.scaleAreaTo("pt_.*!", 1.);
  TrackParm.scaleAreaTo("chi2_.*!", 1.);
  TrackParm.scaleAreaTo("d0_.*!", 1.);
  TrackParm.scaleAreaTo("dz_.*!", 1.);
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
  float min_delay=0, max_delay=0;
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
    clustTree->GetEntry(i); //if (evt.run<123970 || evt.run>123977) continue;
     //if (evt.bx!=51 && evt.bx!=2824 && evt.bx!=-1) continue;

   std::string fname=clustTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.run, evt.orb, evt.ls);
    }
    if (stepDelay.first==NOVAL_I || stepDelay.second==NOVAL_I || abs(stepDelay.second)==25) continue;
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
			  " pixel;Clock phase [ns];Cluster size (pixels)");
  clusMeanSizeVsDelay.add(n_delay, min_delay, max_delay, "FPix;Mean cluster size in the Forward"
			  " pixel;Clock phase [ns];Cluster size (pixels)");
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
    clusCharge.add(70, 0., 210., "BPix_[%d]!clk %d ns!Barrel Pixel;Cluster charge in the Barrel"
		   " pixel;Cluster charge [ke];Arbitrary", it->first, it->second);
    clusCharge.add(70, 0., 210., "FPix_[%d]!clk %d ns!Forward Pixel;Cluster charge in the Forward"
		   " pixel;Cluster charge [ke];Arbitrary", it->first, it->second);
  }
  clusCharge.setSumw2();


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
  clusChargeBPixShell.setSumw2();
  clusChargeFPixShell.setSumw2();


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
      p.setSumw2();
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
    p.setSumw2();
    clusChargeFPix.push_back(p);
  }


  // 4.  Pixel Charge
 
  deb::Plot<TH1F> pixCharge(600, 600, "%s_pixCharge\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    pixCharge.add(50, 0., 50., "BPix_[%d]!clk %d ns!Barrel Pixel;Pixel charge in clusters in the Barrel"
		   " pixel;Pixel charge [ke];Arbitrary", it->first, it->second);
    pixCharge.add(50, 0., 50., "FPix_[%d]!clk %d ns!Forward Pixel;Pixel charge in clusters in the Forward"
		   " pixel;Pixel charge [ke];Arbitrary", it->first, it->second);
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


  deb::Plot<TH1F> sizeClusBPix(600, 600, "%s_sizeClusBPix\nTDR", plotType.data());
  deb::Plot<TH1F> sizeClusFPix(600, 600, "%s_sizeClusFPix\nTDR", plotType.data());
  for (std::map<int,int>::iterator it=delays.begin();it!=delays.end();it++) {
    sizeClusBPix.add(40, 0.5, 40.5, "BPix_[%d]!clk %d ns!Barrel Pixel;Cluster size in the Barrel"
		     " pixel;Cluster charge [ke];Arbitrary", it->first, it->second);
    sizeClusFPix.add(40, 0.5, 40.5, "FPix_[%d]!clk %d ns!Forward Pixel;Cluster size in the "
		     "Forward pixel;Cluster charge [ke];Arbitrary", it->first, it->second);
  }
  sizeClusBPix.setSumw2();
  sizeClusFPix.setSumw2();


  // ----------------------------------------------------------------------------------------------
  // Filling histograms
  //

  int prevevt=NOVAL_I;
  int prevevtNumClus=NOVAL_I;
  int prevevtNumClusFPix[5];
  int prevevtNumClusBPix[3];
  int prevevtDelay=NOVAL_I;

  for (long i=0; i<clustTree->GetEntries(); i++) {
    clustTree->GetEntry(i); //if (evt.run<123970 || evt.run>123977) continue;
    //if (evt.bx!=51 && evt.bx!=2824 && evt.bx!=-1) continue;
    

    std::pair<int,int> stepDelay;
    if (data==0) {
      stepDelay=getStepDelayCosmics(evt.delay, evt.run);
    } else {
      stepDelay=getStepDelayCollision(evt.run, evt.orb, evt.ls);
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

    if (fabs(delay)==25) continue;

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
      if (clust.charge>12.) sizeClusBPix[detector+sstep].Fill(clust.size);
    } else if (clust.mod_on.det==1) { 
      sizeClus("FPix_D%d_[%d]", clust.mod_on.disk, step).Fill(clust.size);
      if (clust.charge>12.) sizeClusFPix[detector+sstep].Fill(clust.size);
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
  std::cout<<"Drawing\n";
  // 1. Mean cluster size
  clusMeanSizeVsDelay.efficiency();
  //clusMeanSizeVsDelay.setAxisRange(".*", 0, 1.0, "Y");
  clusMeanSizeVsDelay.setColor(".*", "LineMarker", 1, 1);
  clusMeanSizeVsDelay.setStyle(".*", "Marker", 1);
  clusMeanSizeVsDelay.Draw("FPix!Forward Pixel& BPix!Barrel Pixel;overlay9PEX0");
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
  clusCharge.setWidth("BPix_.*", "Line", 3);
  clusCharge.setWidth("FPix_.*", "Line", 3);
  clusCharge.scaleAreaTo(".*", 1.);
  //   clusCharge.Draw("BPix_.*!!;overlay9HISTL\nFPix_.*!!;overlay9HISTL");
  // clusCharge.Draw("BPix_.*!!;overlay9HISTL");
  // clusCharge.Draw("FPix_.*!!;overlay9HISTL");

  // 2010. jan. 24.
  // FPix:
//   clusCharge.setColor("FPix_.1.", "LineMarker", 4);
//   clusCharge.setColor("FPix_.4.", "LineMarker", 1);
//   clusCharge.setColor("FPix_.8.", "LineMarker", 2);
//   clusCharge.Draw("FPix_.[1,4,8].!!;overlay9HISTL");
  // BPix:
  clusCharge.setColor("BPix_.1.", "LineMarker", 4);
  clusCharge.setColor("BPix_.4.", "LineMarker", 1);
  clusCharge.setColor("BPix_.8.", "LineMarker", 2);
  clusCharge.Draw("BPix_.[1,4,8].!!;overlay9HISTL");

//   clusCharge.canvas()->GetPad(1)->cd();
//   TPaveText* text = new TPaveText(0.3, 0.3, 0.5, 0.35);
//   text->AddText("CMS Preliminary 2009 (900 GeV)");
//   text->SetFillColor(0);
//   text->SetBorderSize(0);
//   text->SetFillStyle(0);
//   text->SetTextSize(0.06);
//   text->SetTextFont(42);
//   text->Draw();

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
  pixCharge.setWidth("BPix_.*", "Line", 3);
  pixCharge.setWidth("FPix_.*", "Line", 3);
  pixCharge.scaleAreaTo(".*", 1.);
  //   pixCharge.Draw("BPix_.*!!;overlay9HISTL\nFPix_.*!!;overlay9HISTL");
  //  pixCharge.Draw("BPix_.*!!;overlay9HISTL");
  // pixCharge.Draw("FPix_.*!!;overlay9HISTL");

  // 2010. jan. 24
  // FPix:
//   pixCharge.setColor("FPix_.1.", "LineMarker", 4);
//   pixCharge.setColor("FPix_.4.", "LineMarker", 1);
//   pixCharge.setColor("FPix_.8.", "LineMarker", 2);
//   pixCharge.Draw("FPix_.[1,4,8].!!;overlay9HISTL");

  // FPix:
  pixCharge.setColor("BPix_.1.", "LineMarker", 4);
  pixCharge.setColor("BPix_.4.", "LineMarker", 1);
  pixCharge.setColor("BPix_.8.", "LineMarker", 2);
  pixCharge.Draw("BPix_.[1,4,8].!!;overlay9HISTL");

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


  sizeClusBPix.setColor("BPix_.*", "LineMarker", 1, 1);
  sizeClusFPix.setColor("FPix_.*", "LineMarker", 1, 1);
  sizeClusBPix.setWidth("BPix_.*", "Line", 3);
  sizeClusFPix.setWidth("FPix_.*", "Line", 3);
  sizeClusBPix.scaleAreaTo(".*", 1.);
  sizeClusFPix.scaleAreaTo(".*", 1.);
  // 2010. jan. 24
  sizeClusFPix.setColor("FPix_.1.", "LineMarker", 4);
  sizeClusFPix.setColor("FPix_.4.", "LineMarker", 1);
  sizeClusFPix.setColor("FPix_.7.", "LineMarker", 2);
  sizeClusFPix.setColor("FPix_.0.", "LineMarker", 1);
  sizeClusFPix.Draw("FPix_.[0,1,4,7].!!;overlay9HIST");
  sizeClusBPix.setColor("BPix_.1.", "LineMarker", 4);
  sizeClusBPix.setColor("BPix_.4.", "LineMarker", 1);
  sizeClusBPix.setColor("BPix_.8.", "LineMarker", 2);
  sizeClusBPix.setColor("BPix_.0.", "LineMarker", 1);
  sizeClusBPix.Draw("BPix_.[0,1,4,8].!!;overlay9HIST");

  sizeClusBPix.Write();
  sizeClusFPix.Write();
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
    std::cout << "<input file>  [<output file>  [<ntuple name>]]" << std::endl;
    return 0;
  }

  std::string infilename=argv[1];
  std::string outfilename = (argc>2) ? argv[2] : "TimingAnalysis.root";
  std::string ntuplename =  (argc>3) ? argv[3] : "ctfNtuple";

  std::ostringstream addname;
  TChain *trajTree=new TChain("trajTree");
  addname.str("");
  //addname << infilename << "/" << ntuplename << "/trajTree";
  addname << infilename << "/trajTree";
  trajTree->Add(addname.str().data());
  std::cout << "Collecting files " << addname.str().data() << std::endl;

  TChain *trackTree=new TChain("trackTree");
  addname.str("");
  //addname << infilename << "/" << ntuplename << "/trackTree";
  addname << infilename << "/trackTree";
  trackTree->Add(addname.str().data());

  TChain *clustTree=new TChain("clustTree");
  addname.str("");
  //addname << infilename << "/" << ntuplename << "/clustTree";
  addname << infilename << "/clustTree";
  clustTree->Add(addname.str().data());

  TFile outfile(outfilename.data(), "RECREATE");

  if (ntuplename.find("ctf")!=std::string::npos) {
    doRecHitPlots(trajTree, 0);
    //doTrackPlots(trackTree, 0);
    //doClusterPlots(clustTree, 0);
  } else {
    doRecHitPlots(trajTree, 1);
    //doTrackPlots(trackTree, 1);
    //doClusterPlots(clustTree, 1);    
  }

  outfile.Close();

  delete trajTree;
  delete trackTree;
  delete clustTree;

  return 1;   
}

#endif
