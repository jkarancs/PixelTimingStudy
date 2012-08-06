//#define COMPLETE 0
#define SPLIT 0
#define NONEFFPLOTS 0
#define FEDERRPLOTS 0
#define FILLS 1 // It means also loading Lumi Tree (Disable for Express data)
#define BADROC 0 // Find SEU Candidates, Bad rocs create lots of histograms (Do it once)
#define SCANPLOTS 0
#define NTHFILE 1
// #include "headers/DataStruct_v16.h"
// #include "headers/DataStruct_v18.h"
// #include "headers/DataStruct_v20.h"
// #include "headers/DataStruct_v21.h"
// #include "headers/DataStruct_v23.h"
// #include "headers/DataStruct_v25.h"
// #include "headers/DataStruct_v27.h"
// #include "headers/DataStruct_v28.h"
#include "headers/DataStruct_v29.h"
// #include "headers/TreeReader_v23.h"
// #include "headers/TreeReader_v25.h"
// #include "headers/TreeReader_v26.h"
// #include "headers/TreeReader_v28.h"
// #include "headers/TreeReader_v29.h"
#include "headers/TreeReader_v30.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TCanvas.h"
#include "TH3.h"
#include "TF1.h"
#include "TGraph.h"
#include "TProfile.h"
#include "TStopwatch.h"
#include "TLegend.h"
#include <cmath>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <stdio.h>
#include <map>
#include "headers/Histograms.h"
#include "headers/PostFixes.h"
#include "headers/Cuts.h"
#include "headers/Variables.h"
#include "headers/AllHistos.h"
#include "headers/CanvasMaker.h"

class Analysis {

 public:

  Analysis(int evtloop, int trajloop, int clustloop) {
    evtloop_=evtloop;
    trajloop_=trajloop;
    clustloop_=clustloop;
    build_();
  }
  ~Analysis() { }

 private:
  int debug_;

  TStopwatch *sw_;

  int evtloop_;
  int trajloop_;
  int clustloop_;

  TChain* filechain_;
  TChain* trajchain_;
  TChain* eventchain_;

  int nfile_;
  long long int allhits_;
  long long int allevents_;
  int totalevent_;
  int totallumi_;
  long long int totalhit_;
  int percentdone_;
  int thousandthdone_;
  int lscheck_;
  int evtcheck_;
  int runcheck_;
  int evt_lscheck_;
  int evt_runcheck_;

  std::string outfile_;

  void build_() {
    debug_ = 0;
    filechain_ = new TChain("filechain");
    trajchain_ = new TChain("trajchain");
    eventchain_ = new TChain("eventchain");
    nfile_ = 0;
    totalevent_ = 0;
    totallumi_ = 0;
    totalhit_ = 0;
    percentdone_ = 0;
    thousandthdone_ = 0;
    lscheck_ = 0;
    evtcheck_ = 0;
    runcheck_ = 0;
    evt_lscheck_ = 0;
    evt_runcheck_ = 0;
    allhits_ = 0;
    allevents_ = 0;
  }

  void run_progress_(int progress, const EventData &evt) {
    if (progress==0) {
#if NTHFILE == 1
      allhits_ = (long long int)trajchain_->GetEntries();
      allevents_ = (long long int)eventchain_->GetEntries();
#else
      TObjArray *fileElements=filechain_->GetListOfFiles();
      TIter next(fileElements);
      TChainElement *chEl=0;
      int nfile = 0;
      while (( chEl=(TChainElement*)next() )) {
	if (nfile%NTHFILE==0) {
	  TFile f(chEl->GetTitle());
	  TTree *trajTree = (TTree*)f.Get("trajTree");
	  allhits_ += trajTree->GetEntries();
	  TTree *eventTree = (TTree*)f.Get("eventTree");
	  allevents_ += eventTree->GetEntries();
	}
	nfile++;
      }
#endif
      delete trajchain_;
      delete eventchain_;
      std::cout<<"------------------- Total Number of Hits           : "
	       <<allhits_<<std::endl;
      std::cout<<"------------------- Total Number of Events         : "
	       <<allevents_<<std::endl;
    } 
    else if (progress==1) {
      std::cout<<"------------------- Total Number of Lumisections   : "
	       <<totallumi_<<std::endl;
      sw_ = new TStopwatch;
      sw_->Start(kFALSE);
    } 
    else if (progress==2) {
      totalhit_++;
      if (allhits_<100000000&&totalhit_>(allhits_/100*(percentdone_+1))) {
	percentdone_++;
	sw_->Stop();
	double totaltime = sw_->RealTime();
	sw_->Continue();
	double hitspersec = totalhit_/totaltime;
	double timeleft = (allhits_-totalhit_) / hitspersec;
	int time_h = 0;
	int time_m = 0;
	int time_s = 0;
	if (timeleft > 3600.0) { 
	  while (timeleft>3600.0) { timeleft -= 3600.0; time_h++; } 
	  while (timeleft>60.0) { timeleft -= 60.0; time_m++; }
	  std::cout<<"\r-------------------- "<<std::setprecision(3)
		   <<percentdone_<<"% Done ( "<<time_h<<"h "
		   <<time_m<<"m Left ) --------------------"<<std::flush; 
	}
	else if (timeleft > 60.0) { 
	  while (timeleft>60.0) { timeleft -= 60.0; time_m++; }
	  while (timeleft>1.0) { timeleft -= 1.0; time_s++;}
	  std::cout<<"\r-------------------- "<<std::setprecision(3)
		   <<percentdone_<<"% Done ( "<<time_m<<"m "
		   <<time_s<<"s Left ) --------------------"<<std::flush; 
	}
	else {	while (timeleft>1.0) { timeleft -= 1.0; time_s++; }
	  std::cout<<"\r-------------------- "<<std::setprecision(3)
		   <<percentdone_<<"% Done ( "<<time_s
		   <<"s Left ) --------------------"<<std::flush; 
	}
      } 
      else if (allhits_>=100000000&&totalhit_>(allhits_/1000*(thousandthdone_+1))) {
	thousandthdone_++;
	double percent = thousandthdone_/10.0 ;
	sw_->Stop();
	double totaltime = sw_->RealTime();
	sw_->Continue();
	double hitspersec = totalhit_/totaltime;
	double timeleft = (allhits_-totalhit_) / hitspersec;
	int time_h = 0;
	int time_m = 0;
	int time_s = 0;
	if (timeleft > 3600.0) { 
	  while (timeleft>3600.0) { timeleft -= 3600.0; time_h++; } 
	  while (timeleft>60.0) { timeleft -= 60.0; time_m++; }
	  std::cout<<"\r-------------------- "<<percent<<"% Done ( "
		   <<time_h<<"h "<<time_m
		   <<"m Left ) --------------------"<<std::flush; 
	}
	else if (timeleft > 60.0) { 
	  while (timeleft>60.0) { timeleft -= 60.0; time_m++; }
	  while (timeleft>1.0) { timeleft -= 1.0; time_s++;}
	  std::cout<<"\r-------------------- "<<percent
		   <<"% Done ( "<<time_m<<"m "<<time_s
		   <<"s Left ) --------------------"<<std::flush; 
	}
	else {	while (timeleft>1.0) { timeleft -= 1.0; time_s++; }
	  std::cout<<"\r-------------------- "<<percent
		   <<"% Done ( "<<time_s<<"s Left ) --------------------"
		   <<std::flush; 
	}
      }
      if (evt.evt!=evtcheck_) totalevent_++;
      evtcheck_ = evt.evt;
      lscheck_ = evt.ls;
      runcheck_ = evt.run;
    } else if (progress==3) {
      std::cout<<std::endl;
      std::cout<<"Finished Processing "<<nfile_<<" Files."<<std::endl;
      std::cout<<"Total Lumisections = "<<totallumi_<<std::endl;  
      std::cout<<"Total Processed Events = "<<totalevent_
	       <<" Total Processed Hits = "<<totalhit_<<std::endl;
      std::cout<<"---------------------------------------------------------"
	       <<std::endl;
      delete sw_;
    }
  }

  void process_lumi_(long long int i, TreeReader &tr, Variables &var) {
    tr.lumi_read(i);
    var.load_lumi_var(tr.lumi());
  }

  void process_evt_(long long int i, long long int &lumientry, TreeReader &tr, 
		    Variables &var, AllHistos &ah) {
    tr.evt_read(i);
    var.load_evt_var(tr.evt());
    if (tr.evt().ls!=evt_lscheck_||tr.evt().run!=evt_runcheck_) { // if new lumisection found
      process_lumi_(lumientry, tr, var);
      // Get same ls as in eventTree
      while ((tr.lumi().ls!=tr.evt().ls||tr.lumi().run!=tr.evt().run)&&lumientry<tr.nls()-1) {
	lumientry++; process_lumi_(lumientry, tr, var); 
      }
    }
    evt_lscheck_ = tr.evt().ls;
    evt_runcheck_ = tr.evt().run;
    ah.fill_evt(tr,var);
  }

  void postprocess_evt_(long long int i, long long int &lumientry, TreeReader &tr,
			Variables &var, AllHistos &ah) {
    tr.evt_read(i);
    var.load_evt_var(tr.evt());
    if (tr.evt().ls!=evt_lscheck_||tr.evt().run!=evt_runcheck_) { // if new lumisection found
      process_lumi_(lumientry, tr, var);
      // Get same ls as in eventTree
      while ((tr.lumi().ls!=tr.evt().ls||tr.lumi().run!=tr.evt().run)&&lumientry<tr.nls()-1) {
	lumientry++; process_lumi_(lumientry, tr, var); 
      }
    }
    evt_lscheck_ = tr.evt().ls;
    evt_runcheck_ = tr.evt().run;
    ah.postfill_evt(tr,var);
  }

#ifdef COMPLETE
  Int_t process_clu_(long long int i, TreeReader &tr, 
		     Variables &var, AllHistos &ah) {
    Int_t succ = tr.clu_read(i);
    var.load_clu_var(tr.clu_evt(),tr.clu());
    ah.fill_clust(tr.clu_evt(),tr.clu(),var); 
    return succ;
  }
#endif

  Int_t process_hit_(long long int i, long long int &lumientry, 
		     long long int &evtentry, long long int &clustentry, 
		     TreeReader &tr, Variables &var, AllHistos &ah) {
    Int_t succ = tr.traj_read(i);
    var.load_traj_var(tr.traj_evt(),tr.traj());
    if (FILLS) {
      // Parallel loop on lumiTree (need for RAW)
      if (tr.traj_evt().ls!=lscheck_||tr.traj_evt().run!=runcheck_) { // if new lumisection found
	process_lumi_(lumientry, tr, var);
	// Get same ls as in trajTree
	while ((tr.lumi().ls!=tr.traj_evt().ls||tr.lumi().run!=tr.traj_evt().run)&&lumientry<tr.nls()-1) {
	  lumientry++; process_lumi_(lumientry, tr, var);
	}
      }
    }
    Int_t clust_succ = 1;
#ifdef COMPLETE
    if (clustloop_==2) {
      // For every new event, do a loop on the clustTree to count pixels
      if (tr.traj_evt().evt!=evtcheck_) {
	clust_succ = process_clu_(clustentry, tr, var, ah);
	// Get same event as in trajTree
	while (tr.clu_evt().evt!=tr.traj_evt().evt&&clustentry<tr.nclu()&&clust_succ!=-1) {
	  clustentry++; 
	  clust_succ = process_clu_(clustentry, tr, var, ah);
	}
	var.reset_counters(0);
	// Count number of pixels in the entire event
	while (tr.clu_evt().evt==tr.traj_evt().evt&&clust_succ!=-1) {
	  var.count_npix(tr.clu_evt(),tr.clu());
	  if (clustentry<tr.nclu()-1) {
	    clustentry++; 
	    clust_succ = process_clu_(clustentry, tr, var, ah);
	  } else break;
	}
      }
    }
#endif
    run_progress_(2,tr.traj_evt());
    // In a possibility for double processed lumisections
    // check if certain lumisection was processed earlier
    // and avoid processing again
    succ = succ==-1 ? -1 : clust_succ==-1 ? -1 : 1;
    if (succ!=-1) {
      ah.fill_traj(tr,var);
      // if (var.no_double_ls)  ah.fill_traj(tr,var);
      // else if (var.traj_isnewls==1&&!var.no_double_ls)
      //   std::cout<<"Double Lumisection Foundin Fill: "<<var.lumi_fill<<" Run: "
      //   	 <<tr.traj_evt().run<<" LS: "<<tr.traj_evt().ls<<std::endl;
    } else tr.severity++;
    return succ;
  }

 public:

  void addfile(std::string fileaddress) {
    filechain_->Add(fileaddress.c_str());
    std::string trajaddress = fileaddress + "/trajTree";
    trajchain_->Add(trajaddress.c_str());
    std::string eventaddress = fileaddress + "/eventTree";
    eventchain_->Add(eventaddress.c_str());
  }

  void addfile(std::string fileaddress,int low,int high) {
    for (int i=low; i<=high; i++) {
      std::stringstream fn;
      fn<<fileaddress<<"*";
      if (i<10) fn<<"000";
      else if (i<100) fn<<"00";
      else if (i<1000) fn<<"0";
      fn<<i<<".root";
      filechain_->Add(fn.str().c_str());
      std::string trajaddress = fn.str() + "/trajTree";
      trajchain_->Add(trajaddress.c_str());
      std::string eventaddress = fn.str() + "/eventTree";
      eventchain_->Add(eventaddress.c_str());
    }
  }

  void preprocess(TreeReader &tr, Variables &var,
		  AllHistos &ah, PostFixes &p) {
    run_progress_(0,tr.traj_evt());
    TObjArray *fileElements=filechain_->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;
    int nfile = 0;
    while (( chEl=(TChainElement*)next() )) {
      if (nfile%NTHFILE==0) {
	nfile_++;
	TFile f(chEl->GetTitle());
	tr.readtrees(f);
	for (long long int i=0; i<tr.nls(); i++) {
	  tr.lumi_read(i);
	  var.count_runs_fills_lumi(tr.lumi(),p);
	  totallumi_++;
	}
      }
      nfile++;
    }
    //     std::sort(var.lumi_run.begin(),var.lumi_run.end());
    var.init();
  }

  void process(TreeReader &tr, Variables &var,
	       AllHistos &ah) {
    run_progress_(1,tr.traj_evt());
    TObjArray *fileElements=filechain_->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;
    int nfile = 0;
    while (( chEl=(TChainElement*)next() )) {
      var.p1_filter = (nfile<190) ? nfile/38 : 0; // 2208
      // var.p1_filter = (nfile<71) ? 0 : 4; // 2105
      var.modout = (nfile<6) ? nfile/2 : 0;
      if (nfile%NTHFILE==0) {
	FILE *lastfile  = fopen("lastfile.txt","w");
	fprintf(lastfile, chEl->GetTitle());
	fclose(lastfile);
	// std::cout<<"Opening file: "<<chEl->GetTitle()<<std::endl;
	TFile f(chEl->GetTitle());
	tr.readtrees(f);
	long long int lumientry = 0;
	long long int evtentry = 0;
	long long int clustentry = 0;
	if (trajloop_==1) for (long long int i=0; i<tr.nhit(); i++) {
	  Int_t succ = process_hit_(i, lumientry, evtentry, clustentry, tr, var, ah);
	  if (succ==-1) {
	    std::cout<<"Problematic entry found: "<<i<<" in File: "<<chEl->GetTitle()<<std::endl;
	    i+=(int)pow(10,tr.severity+3);
	    clustentry+=(int)pow(10,tr.severity+3);
	  }
	}
	if (tr.severity) std::cout<<"Severity: "<<tr.severity<<std::endl;
	if (evtloop_>0) {
	  lumientry = 0;
	  for (long long int i=0; i<tr.nevt(); i++)
	    process_evt_(i, lumientry, tr, var, ah);
	}
#ifdef COMPLETE
	if (clustloop_==1) for (long long int i=0;i<tr.nclu();i++)
	  process_clu_(i, tr, var, ah);
#endif
      }
      nfile++;
    }
    run_progress_(3,tr.traj_evt());
  }

  void postprocess(TreeReader &tr, Variables &var,
		   AllHistos &ah, PostFixes &p) {
    std::cout<<"Postprocessing..."<<std::endl;
    if (evtloop_==2) {
      TObjArray *fileElements=filechain_->GetListOfFiles();
      TIter next(fileElements);
      TChainElement *chEl=0;
      int nfile = 0;
      while (( chEl=(TChainElement*)next() )) {
	if (nfile%NTHFILE==0) {
	  TFile f(chEl->GetTitle());
	  tr.readtrees(f);
	  long long int lumientry = 0;
	  for (long long int i=0; i<tr.nevt(); i++)
	    postprocess_evt_(i, lumientry, tr, var, ah);
	}
	nfile ++;
      }
    }
    delete filechain_;
    ah.postprocess(var,p);
    std::cout<<"Finished Postprocessing"<<std::endl;
  }
  
  void write_output(std::string subdir, std::string desc, 
		    std::string runname, AllHistos &ah, Variables &var) {
    std::stringstream nthfile;
    if (NTHFILE!=1)  nthfile<<"_"<<NTHFILE<<"th";
    
    std::stringstream version_prefix;
    version_prefix<<"v"<<VERSION1<<VERSION2<<"_";

    std::string output = version_prefix.str() + desc + "_" + runname + nthfile.str();
    outfile_ = subdir + output + ".root";

    TFile *fo= new TFile(outfile_.c_str(),"recreate");
    ah.calc_write(var);
    fo->Close();
    std::cout<<"File: "<<outfile_<<" ready."<<std::endl;
  }

  // Accessors
 public:
  std::string outfile() { return outfile_; }
  
};



int main() {
  
  // eventloop, trajloop, clustloop (0: off, 1:on separate, 2: parallel - for pixel count)
  Analysis effanalysis(FEDERRPLOTS*2, 1, 2); // loop on all trees
//   Analysis effanalysis(2, 0, 0); // loop only on event- and lumiTree
//   Analysis effanalysis(0, 0, 1); // loop only on clustTree
  TreeReader tr(1,   1,    1,      1,   1,   0,              1,   1,      0,   1);
  AllHistos ah;
  PostFixes p;
  Variables v;


  // Declare histograms
  // Cumulative:  0: no 1: cumx or (cumx, cumy) -1: compcumx or  (compcumx, compcum1)
  //                    2: cumx, compcum y      -2: compcumx, cum y
  //                                name,        npf, dim, eff, err, clusize, default, cumulative
  // Histograms *h = new Histograms("histoname", 2,   1,   1,   2,   0,       0,       0);

  //________________________________________________________________________________________
  //                                Not Fill-by-Fill Plots
  //________________________________________________________________________________________
  //                                 Not Efficiency Plots:
  //                   Performance plots: pcd, ncd, clu_size, alpha, beta
  //________________________________________________________________________________________

#if NONEFFPLOTS ==1
  ah.ncd            = new Histograms("ncd",             2,   1);
  ah.ncd_corr       = new Histograms("ncd_corr",        2,   1);
  ah.pcd            = new Histograms("pcd",             2,   1);
  ah.pcd_corr       = new Histograms("pcd_corr",        2,   1);
  ah.pcd_heta       = new Histograms("pcd_heta",        2,   1);
  ah.pcd_corr_heta  = new Histograms("pcd_corr_heta",   2,   1);
  ah.clu_size       = new Histograms("clu_size",        2,   1);
  ah.clu_sizeX      = new Histograms("clu_sizeX",       2,   1);
  ah.clu_sizeY      = new Histograms("clu_sizeY",       2,   1);
  ah.clu_size_heta  = new Histograms("clu_size_heta",   2,   1);
  ah.clu_sizeX_heta = new Histograms("clu_sizeX_heta",  2,   1);
  ah.clu_sizeY_heta = new Histograms("clu_sizeY_heta",  2,   1);
  ah.alpha          = new Histograms("alpha",           2,   1);
  ah.beta           = new Histograms("beta",            2,   1);

  ah.totlumi_ratio  = new Histograms("totlumi_ratio",   1,   1);

  ah.ncd           ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   200, 0.0, 100.0);
  ah.ncd_corr      ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   200, 0.0, 100.0);
  ah.pcd           ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   200, 0.0, 50.0);
  ah.pcd_corr      ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   200, 0.0, 50.0);
  ah.pcd_heta      ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   200, 0.0, 50.0);
  ah.pcd_corr_heta ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   200, 0.0, 50.0);
  ah.clu_size      ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   25,  0.0, 25.0);
  ah.clu_sizeX     ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   25,  0.0, 25.0);
  ah.clu_sizeY     ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   25,  0.0, 25.0);
  ah.clu_size_heta ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   25,  0.0, 25.0);
  ah.clu_sizeX_heta->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   25,  0.0, 25.0);
  ah.clu_sizeY_heta->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   25,  0.0, 25.0);
  ah.alpha         ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   315, 0.0, 3.15);
  ah.beta          ->add(p.totlumi, 1,  1, 5, 0, p.det, 1,  1,  10,   315, 0.0, 3.15);

  ah.totlumi_ratio ->add(p.det, 1,  1, 10,  600, 0.0, 6.0);
#endif

  //________________________________________________________________________________________
  //                                    Efficiency Plots
  //________________________________________________________________________________________

  // Efficiency                     name,             npf, dim, eff, err, clusize, default, cumulative
  ah.ls            = new Histograms("ls",             2,   1,   1,   2,   0,       0,       0);
  ah.totlumi       = new Histograms("totlumi",        2,   1,   1,   2,   0,       1,       0);
  ah.nvtx          = new Histograms("nvtx",           2,   1,   1,   2,   0,       0,      -1);
  ah.time2         = new Histograms("time2",          3,   1,   1,   2);
#ifdef COMPLETE
  ah.occup         = new Histograms("occup",          3,   1,   1,   2,   0,       0,      -1);
  ah.occup_mod     = new Histograms("occup_mod",      3,   1,   1,   2,   0,       0,      -1);
  ah.occup_roc     = new Histograms("occup_roc",      3,   1,   1,   2,   0,       0,      -1);
  ah.occup_dcol    = new Histograms("occup_dcol",     3,   1,   1,   2,   0,       0,      -1);
#endif

//ah.lxly          = new Histograms("lxly",           3,   2,   1,   2);
  ah.lxly          = new Histograms("lxly",           2,   2,   1,   2);

  ah.ls           ->add(p.det, 1,  1, 10, 0, p.cumeff, 1,  1,  2,   60,  0.0, 3000.0);
  ah.totlumi      ->add(p.det, 1,  1, 10, 0, p.effmpv, 1,  1, 10,  400,  0.0, 20.0);
  ah.nvtx         ->add(p.det, 1,  1, 10, 0, p.cumeff, 1,  1,  4,   21, -0.5, 20.5);
  ah.time2        ->add(p.spec,1,  1,  2, 0, p.det,    1,  1, 10, 0, p.cumeff, 1, 1, 2,  96, 0, 24);
#ifdef COMPLETE
  ah.occup      ->add(p.occup, 1, 1, 2, 0, p.det, 1, 1,  6, 0, p.cumeff, 1, 1, 4, 250,  0.0, 25000.0);
  ah.occup      ->add(p.occup, 0, 1, 2, 0, p.det, 1, 7, 10, 0, p.cumeff, 1, 1, 4, 250,  0.0,  5000.0);
  ah.occup_mod  ->add(p.occup, 1, 1, 2, 0, p.det, 1, 1, 10, 0, p.cumeff, 1, 1, 4, 250,  0.0,   250.0);
  ah.occup_roc  ->add(p.occup, 1, 1, 2, 0, p.det, 1, 1, 10, 0, p.cumeff, 1, 1, 4, 100,  0.0,   100.0);
  ah.occup_dcol ->add(p.occup, 1, 1, 2, 0, p.det, 1, 1, 10, 0, p.cumeff, 1, 1, 4,  50,  0.0,    50.0);
#endif

  // badcol modules
  v.binmaker(2,8);
  ah.lxly       ->add(p.badcol,    1, 1, 24, 0, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  // modules/ladders
  // v.binmaker(2,8);
  // ah.lxly      ->add(p.det, 1, 4,  4, 3, p.module, 1,  1,  8,  0, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  // ah.lxly      ->add(p.det, 0, 4,  4, 3, p.ladder, 1, 13, 32,  4, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  // ah.lxly      ->add(p.det, 1, 5,  5, 3, p.module, 1,  1,  8,  0, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  // ah.lxly      ->add(p.det, 0, 5,  5, 3, p.ladder, 1,  7, 38, -2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  // ah.lxly      ->add(p.det, 1, 6,  6, 3, p.module, 1,  1,  8,  0, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  // ah.lxly      ->add(p.det, 0, 6,  6, 3, p.ladder, 1,  1, 44, -8, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);



  //________________________________________________________________________________________
  //                                Efficiency N-1 Cut Plots:

  // 1 Hogyan számoljuk a hatásfokot
  // - Fiducial vágások
  // - hit-hit távolság eloszlás
  // - hit-cluster távolság
  // - (Efficiency vs Alpha/beta)
  // - (average normalized charge vs alpha/beta)
  // 
  // 2 Hogyan veszítünk hatásfokot
  // - FED error-os moduleok hatásfoka
  // - ROC hatásfok eloszlás, és szélessége (roceff_dist)
  // - Alacsony hatásfokú ROC-ok (Nbadroc) - SEU Candidates (badroc) 
  // 
  // 3 Ezek kizárásával a dinamikus hatásfok vesztés
  // - Dynamic Efficiency Loss (inst lumi, bx)
  // - Efficiency Mapek (FED error és SEU nélkül)
  // - Layer hatásfok

  // N-1 histos                name,       npf dim, eff, err, clusize, default, cumulative
  ah.pt       = new Histograms("pt",       2,  1,   1,   2,   0,       0,      -1);
  ah.nstrip   = new Histograms("nstrip",   2,  1,   1,   2,   0,       0,      -1);
  ah.dz       = new Histograms("dz",       2,  1,   1,   2,   0,       0,       1);
  ah.d0       = new Histograms("d0",       2,  1,   1,   2,   0,       0,       1);
  ah.dz_d0    = new Histograms("dz_d0",    2,  2,   1,   2,   0,       0,       1);
  ah.normchi2 = new Histograms("normchi2", 2,  1,   1,   2,   0,       0,      -1);
  ah.vtxd0    = new Histograms("vtxd0",    2,  1,   1,   2);
  ah.vtxz     = new Histograms("vtxz",     2,  1,   1,   2);
  ah.fid_lxly = new Histograms("fid_lxly", 3,  2,   1,   2);
  ah.fid_lx   = new Histograms("fid_lx",   3,  1,   1,   2);
  ah.fid_ly   = new Histograms("fid_ly",   3,  1,   1,   2);
//ah.vtxndof  = new Histograms("vtxndof",  2,  1,   1,   2);
  ah.hitclu   = new Histograms("hitclu",   2,  1,   1,   2,   0,       0,       1);
  ah.cluclu   = new Histograms("cluclu",   2,  1,   1,   2,   0,       0,      -1);
  ah.hithit   = new Histograms("hithit",   2,  1,   1,   2,   0,       0,      -1);
//ah.clcl_hcl = new Histograms("clcl_hcl", 2,  2,   1,   2,   0,       0,      -2);
//ah.hh_hcl   = new Histograms("hh_hcl",   2,  2,   1,   2,   0,       0,      -2);
//ah.neweff   = new Histograms("neweff",   2,  1,   1,   2); // needs different cumulation method
//ah.neweff2  = new Histograms("neweff2",  2,  1,   1,   2); // needs different cumulation method
  
  ah.pt      ->add(p.det, 1, 1, 16, 0,  p.cumeff, 1, 1, 4,  100, 0.0, 10.0);
  ah.nstrip  ->add(p.det, 1, 1, 16, 0,  p.cumeff, 1, 1, 4,   40, 0.0, 40.0);
  ah.dz      ->add(p.det, 1, 1, 16, 0,  p.cumeff, 1, 1, 4,  100, 0.0,  1.0);
  ah.d0      ->add(p.det, 1, 1, 16, 0,  p.cumeff, 1, 1, 4,  100, 0.0,  0.1);
  ah.dz_d0   ->add(p.det, 1, 1, 16, 0,  p.cumeff, 1, 1, 4,  100, 0.0,  1.0, 100, 0.0, 0.1);
  ah.normchi2->add(p.det, 1, 1, 16, 0,  p.cumeff, 1, 1, 4,  500, 0.0,  5.0);
  ah.vtxd0   ->add(p.det, 1, 1,  3, 0,  p.cumeff, 1, 1, 2,  1000,   0.0,  1.0);
  ah.vtxz    ->add(p.det, 1, 1,  3, 0,  p.cumeff, 1, 1, 2,  500, -25.0, 25.0);

  ah.hitclu  ->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 4,  100, 0.0,  0.1);
  ah.cluclu  ->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 4, 1260, 0.0,  6.3);
  ah.hithit  ->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 4, 1000, 0.0,  10.0);
//ah.clcl_hcl->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 4, 1260, 0.0,  6.3, 100, 0.0, 0.1);
//ah.hh_hcl  ->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 4,  200, 0.0,  2.0, 100, 0.0, 0.1);
//ah.neweff  ->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 2,  200, 0.0,  1.0);
//ah.neweff2 ->add(p.det, 1, 1, 10, 0,  p.cumeff, 1, 1, 2,  200, 0.0,  1.0);

  v.binmaker(2,8);
  ah.fid_lxly->add(p.det, 1, 4,  6, 3, p.mod, 1, 1, 1, 0, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 1, 4,  6, 3, p.mod, 1, 1, 1, 0, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 1, 4,  6, 3, p.mod, 1, 1, 1, 0, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(1,8);
  ah.fid_lxly->add(p.det, 0, 4,  6, 3, p.mod, 1, 2, 2, 0, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 4,  6, 3, p.mod, 1, 2, 2, 0, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 4,  6, 3, p.mod, 1, 2, 2, 0, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(1,2);
  ah.fid_lxly->add(p.det, 1, 7, 10, 3, p.mod, 1, 3, 3, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 1, 7, 10, 3, p.mod, 1, 3, 3, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 1, 7, 10, 3, p.mod, 1, 3, 3, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(2,3);
  ah.fid_lxly->add(p.det, 0, 7, 10, 3, p.mod, 1, 4, 4, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 4, 4, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 4, 4, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(2,4);
  ah.fid_lxly->add(p.det, 0, 7, 10, 3, p.mod, 1, 5, 5, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 5, 5, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 5, 5, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(1,5);
  ah.fid_lxly->add(p.det, 0, 7, 10, 3, p.mod, 1, 6, 6, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 6, 6, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 6, 6, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(2,3);
  ah.fid_lxly->add(p.det, 0, 7, 10, 3, p.mod, 1, 7, 7, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 7, 7, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 7, 7, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(2,4);
  ah.fid_lxly->add(p.det, 0, 7, 10, 3, p.mod, 1, 8, 8, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 8, 8, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 8, 8, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);
  v.binmaker(2,5);
  ah.fid_lxly->add(p.det, 0, 7, 10, 3, p.mod, 1, 9, 9, 2, p.cumeff, 1, 1, 2, v.nx, v.xlow, v.xup, v.ny, v.ylow, v.yup);
  ah.fid_lx  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 9, 9, 2, p.cumeff, 1, 1, 2, v.ny*2, v.ylow, v.yup);
  ah.fid_ly  ->add(p.det, 0, 7, 10, 3, p.mod, 1, 9, 9, 2, p.cumeff, 1, 1, 2, v.nx/2, v.xlow, v.xup);

  //________________________________________________________________________________________
  //                               Delay and HV Bias Scan Plots
  //________________________________________________________________________________________
#if SCANPLOTS == 1
  // Delay Scans                   name,           npf, dim, eff, err, clusize, default, cumulative
  ah.delay11      = new Histograms("delay11",      2,   1,   1,   2,   1);
  ah.delay11_rog  = new Histograms("delay11_rog",  4,   1,   1,   2,   1);
  ah.delay11_mod  = new Histograms("delay11_mod",  4,   1,   1,   2,   1);
  ah.delay11_roc  = new Histograms("delay11_roc",  4,   1,   1,   2,   1);
  ah.delay12      = new Histograms("delay12",      2,   1,   1,   2,   1);
  ah.delay12_rog  = new Histograms("delay12_rog",  4,   1,   1,   2,   1);
  ah.delay12_mod  = new Histograms("delay12_mod",  4,   1,   1,   2,   1);
  ah.delay12_roc  = new Histograms("delay12_roc",  4,   1,   1,   2,   1);
  ah.delay3       = new Histograms("delay3",       2,   1,   1,   2,   1);
  ah.delay3_rog   = new Histograms("delay3_rog",   4,   1,   1,   2,   1);
  ah.delay3_mod   = new Histograms("delay3_mod",   4,   1,   1,   2,   1);
  ah.delay3_roc   = new Histograms("delay3_roc",   4,   1,   1,   2,   1);
  ah.delay4       = new Histograms("delay4",       2,   1,   1,   2,   1);
  ah.delay4_rog   = new Histograms("delay4_rog",   4,   1,   1,   2,   1);
  ah.delay4_mod   = new Histograms("delay4_mod",   4,   1,   1,   2,   1);
  ah.delay4_roc   = new Histograms("delay4_roc",   4,   1,   1,   2,   1);

  ah.trkloss      = new Histograms("trkloss",  2,   1);

  ah.delay11     ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  1,  5,  68, -27.5, 40.5);
  ah.delay11     ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  6,  6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay11_rog ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay11_rog ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay11_mod ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay11_mod ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay11_roc ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay11_roc ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay11_rog ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay11_rog ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay11_mod ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay11_mod ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay11_roc ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay11_roc ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12     ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  1,  5,  68, -27.5, 40.5);
  ah.delay12     ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  6,  6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12_rog ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay12_rog ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay12_mod ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay12_mod ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay12_roc ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay12_roc ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay12_rog ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12_rog ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12_mod ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12_mod ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12_roc ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay12_roc ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3      ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  1,  5,  68, -27.5, 40.5);
  ah.delay3      ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  6,  6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3_rog  ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay3_rog  ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay3_mod  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay3_mod  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay3_roc  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay3_roc  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay3_rog  ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3_rog  ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3_mod  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3_mod  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3_roc  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay3_roc  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4      ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  1,  5,  68, -27.5, 40.5);
  ah.delay4      ->add(p.det,    1,  1, 10,  0, p.effclu,    1,  6,  6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4_rog  ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay4_rog  ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay4_mod  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay4_mod  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay4_roc  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay4_roc  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 1, 5,  68, -27.5, 40.5);
  ah.delay4_rog  ->add(p.shl,    1,  1, 4,   0, p.sec,       1,  1,  8,   0, p.chn,       1, 1,  2, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4_rog  ->add(p.shl,    0,  1, 4,   0, p.dsk,       1,  1,  2,  -8, p.rog,       1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4_mod  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.mod3,      1, 1,  4, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4_mod  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplq,    1, 1,  7, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4_roc  ->add(p.detshl, 1,  1, 4,   0, p.seclyrldr, 1,  1, 48,   0, p.modroc,    1, 1, 64, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);
  ah.delay4_roc  ->add(p.detshl, 1,  5, 8,   0, p.dskbld,    1,  1, 24,   0, p.pnlplqroc, 1, 1, 45, 0, p.effclu, 1, 6, 6,  68, -27.5, 40.5, 100, 0, 100);

  ah.trkloss  ->add(p.det, 1, 1, 10,   0, p.avg, 1, 3, 4,  68, -27.5, 40.5);

  // HV Scans                    name,         npf, dim, eff, err, clusize, default, cumulative
  ah.hv         = new Histograms("hv",         4,   1,   1,   2,   0,       1);
  // HV1 - 2010 April 05 - BPIX_BpO_SEC2 Layer 3 HV1, FPIX_BmI_D1_ROG1 HV1
  ah.hv        ->add(p.scan,   1,  1,  1,  0, p.det,     1,  3,  3,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);
  ah.hv        ->add(p.scan,   0,  1,  1,  0, p.det,     1,  6,  6,  4, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV2 - 2010 Oct 28 - BPIX_BmO_SEC2 Layer 1-2 HV1, BPIX_BpO_SEC2 Layer 3 HV1, FPIX_BmI_D1_ROG1 HV1
  ah.hv        ->add(p.scan,   1,  2,  2,  0, p.det,     1,  3,  3,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);
  ah.hv        ->add(p.scan,   0,  2,  2,  0, p.det,     1,  4,  4,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  2,  2,  0, p.det,     1,  6,  6,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV3 - 2011 Mar 14,16,18 - Bpix_BmO_SEC6_LYR1-2_HV1, Bpix_BpO_SEC2_LYR3_HV1, Fpix_BmI_D1_ROG1_HV1
  ah.hv        ->add(p.scan,   1,  3,  3,  0, p.det,     1,  3,  3,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);
  ah.hv        ->add(p.scan,   0,  3,  3,  0, p.det,     1,  4,  4,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  3,  3,  0, p.det,     1,  6,  6,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV4 - 2011 May 15 - Bpix_BmO_SEC6_LYR1-2_HV1
  ah.hv        ->add(p.scan,   1,  4,  4,  0, p.det,     1,  4,  4,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV5 - 2011 Jul 14 - Layer 1
  ah.hv        ->add(p.scan,   1,  5,  5,  0, p.det,     1,  1,  1,  0, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  5,  5,  0, p.det,     0,  1,  1,  0, p.module,  1,  1,  8, -1, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  5,  5,  0, p.det,     0,  1,  1,  0, p.ladder,  1, 13, 32,  3, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  5,  5,  0, p.shl,     1,  1,  4, -1, p.sec,     1,  1,  8,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV6 - 2011 Jul 28 - Layer 3
  ah.hv        ->add(p.scan,   1,  6,  6,  0, p.det,     1,  1,  1,  0, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  6,  6,  0, p.det,     0,  1,  1,  0, p.module,  1,  1,  8, -1, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  6,  6,  0, p.det,     0,  1,  1,  0, p.ladder,  1,  1, 44, -9, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  6,  6,  0, p.shl,     1,  1,  4, -1, p.sec,     1,  1,  8,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV7 - 2011 Aug 03 - Layer 2
  ah.hv        ->add(p.scan,   1,  7,  7,  0, p.det,     1,  1,  1,  0, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  7,  7,  0, p.det,     0,  1,  1,  0, p.module,  1,  1,  8, -1, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  7,  7,  0, p.det,     0,  1,  1,  0, p.ladder,  1,  7, 38, -3, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  7,  7,  0, p.shl,     1,  1,  4, -1, p.sec,     1,  1,  8,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV8 - 2011 Sep 07 - Layer 1, Fpix_BmI_D1_ROG1_HV1
  ah.hv        ->add(p.scan,   1,  8,  8,  0, p.det,     1,  3,  3,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);
  ah.hv        ->add(p.scan,   0,  8,  8,  0, p.det,     1,  4,  4,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  8,  8,  0, p.det,     0,  4,  4,  2, p.module,  1,  1,  8, -1, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  8,  8,  0, p.det,     0,  4,  4,  2, p.ladder,  1, 13, 32,  3, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0,  8,  8,  0, p.shl,     1,  1,  4, -2, p.sec,     1,  1,  8,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV9 - 2011 Oct 12 - Bpix_BmO_SEC6_LYR1-2_HV1
  ah.hv        ->add(p.scan,   1,  9,  9,  0, p.det,     1,  4,  4,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV10 - 2011 Oct 27 - Bpix_BmO_SEC6_LYR1-2_HV1
  ah.hv        ->add(p.scan,   1, 10, 10,  0, p.det,     1,  4,  4,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  // HV11 - 2011 Oct 30 - Fpix_BmI_D1_ROG1_HV1
  ah.hv        ->add(p.scan,   1, 11, 11,  0, p.det,     1,  3,  3,  2, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);
  // HV12 - 2012 Apr 06 - L1,L2,L3,D1,D2
  ah.hv        ->add(p.scan,   1, 12, 12,  0, p.det,     1,  4,  4,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0, 12, 12,  0, p.det,     1,  5,  5,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0, 12, 12,  0, p.det,     1,  6,  6,  3, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  31, -2.5, 152.5);
  ah.hv        ->add(p.scan,   0, 12, 12,  0, p.det,     1, 17, 17, 13, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);
  ah.hv        ->add(p.scan,   0, 12, 12,  0, p.det,     1, 18, 18, 13, p.det,     1,  1,  1,  0, p.effmpv, 1, 1, 4,  62, -2.5, 307.5);

  std::vector<std::string> HV_Scans;
  HV_Scans.push_back("2010/04/05 - 0.00 fb^{-1}");
  HV_Scans.push_back("2010/10/28 - 0.04 fb^{-1}");
  HV_Scans.push_back("2011/03/18 - 0.05 fb^{-1}");
  HV_Scans.push_back("2011/05/15 - 0.33 fb^{-1}");
  HV_Scans.push_back("2011/07/14 - 1.46 fb^{-1}");
  HV_Scans.push_back("2011/07/28 - 1.84 fb^{-1}");
  HV_Scans.push_back("2011/08/03 - 2.09 fb^{-1}");
  HV_Scans.push_back("2011/09/07 - 2.93 fb^{-1}");
  HV_Scans.push_back("2011/10/12 - 5.17 fb^{-1}");
  HV_Scans.push_back("2011/10/27 - 6.07 fb^{-1}");
  HV_Scans.push_back("2011/10/30 - 6.17 fb^{-1}");
  HV_Scans.push_back("2012/04/06 - 6.20 fb^{-1}");
  HV_Scans.push_back("2012/07/02 - 12.77 fb^{-1}");

  std::vector<std::string> hv_scans;
  for (size_t i=0; i<HV_Scans.size(); i++) {
    std::stringstream ss;
    ss<<"_HV"<<(i+1);
    hv_scans.push_back(ss.str());
  }

  int l1[]     = { 2, 3, 4,  5,  8,  9, 10, 12, 13 };
  int l3[]     = { 1, 2, 3,  6, 12, 13 };
  int d1[]     = { 1, 2, 3,  8, 11, 12, 13 };
  int l1full[] = { 5, 8, 12, 13 };
  int l2full[] = { 7, 12, 13 };
  int l3full[] = { 6, 12, 13 };
  int d1full[] = { 12, 13 };
  int d2full[] = { 12, 13 };

  std::vector<double> totlum_pb;
  totlum_pb.push_back(v.totallumi[132599]/1000000.0);
  totlum_pb.push_back(v.totallumi[149182]/1000000.0);
  totlum_pb.push_back(v.totallumi[160497]/1000000.0);
  totlum_pb.push_back(v.totallumi[165098]/1000000.0);
  totlum_pb.push_back(v.totallumi[170000]/1000000.0);
  totlum_pb.push_back(v.totallumi[171897]/1000000.0);
  totlum_pb.push_back(v.totallumi[172488]/1000000.0);
  totlum_pb.push_back(v.totallumi[175834]/1000000.0);
  totlum_pb.push_back(v.totallumi[178367]/1000000.0);
  totlum_pb.push_back(v.totallumi[180093]/1000000.0);
  totlum_pb.push_back(v.totallumi[180250]/1000000.0);
  totlum_pb.push_back(v.totallumi[190595]/1000000.0);
  totlum_pb.push_back(v.totallumi[198023]/1000000.0);
  v.totlumi_scan.resize(5);
  
  int col[12] = { 1,15,53,425,3,3,3,89,92,809,809,2 }; // root /afs/kfki.hu/home/jkarancs/public/ROOT_output/v3029/TEST/color.C
  for (size_t i=0; i<sizeof(l1)/sizeof(int); i++) {
    v.totlumi_scan[0].push_back(totlum_pb[l1[i]-1]);
    p.hv_l1.push_back(hv_scans[l1[i]-1]);
    p.Hv_l1.push_back(HV_Scans[l1[i]-1]);
    v.l1_col<<col[l1[i]-1]<<",";
    if (i==0) p.Hv_l1[0]+=" - SEC2";
  }
  for (size_t i=0; i<sizeof(l3)/sizeof(int); i++) {
    v.totlumi_scan[2].push_back(totlum_pb[l3[i]-1]);
    p.hv_l3.push_back(hv_scans[l3[i]-1]);
    p.Hv_l3.push_back(HV_Scans[l3[i]-1]);
    v.l3_col<<col[l3[i]-1]<<",";
  }
  for (size_t i=0; i<sizeof(d1)/sizeof(int); i++) {
    v.totlumi_scan[3].push_back(totlum_pb[d1[i]-1]);
    p.hv_d1.push_back(hv_scans[d1[i]-1]);
    p.Hv_d1.push_back(HV_Scans[d1[i]-1]);
    v.d1_col<<col[d1[i]-1]<<",";
  }
  for (size_t i=0; i<sizeof(l1full)/sizeof(int); i++) {
    p.hv_l1full.push_back(hv_scans[l1full[i]-1]);
    p.Hv_l1full.push_back(HV_Scans[l1full[i]-1]);
    v.l1full_col<<col[l1full[i]-1]<<",";
  }
  for (size_t i=0; i<sizeof(l2full)/sizeof(int); i++) {
    v.totlumi_scan[1].push_back(totlum_pb[l2full[i]-1]);
    p.hv_l2full.push_back(hv_scans[l2full[i]-1]);
    p.Hv_l2full.push_back(HV_Scans[l2full[i]-1]);
    v.l2full_col<<col[l2full[i]-1]<<",";
  }
  for (size_t i=0; i<sizeof(l3full)/sizeof(int); i++) {
    p.hv_l3full.push_back(hv_scans[l3full[i]-1]);
    p.Hv_l3full.push_back(HV_Scans[l3full[i]-1]);
    v.l3full_col<<col[l3full[i]-1]<<",";
  }
  for (size_t i=0; i<sizeof(d1full)/sizeof(int); i++) { 
    p.hv_d1full.push_back(hv_scans[d1full[i]-1]);
    p.Hv_d1full.push_back(HV_Scans[d1full[i]-1]);
    v.d1full_col<<col[d1full[i]-1]<<",";
  }
  for (size_t i=0; i<sizeof(d2full)/sizeof(int); i++) {
    v.totlumi_scan[4].push_back(totlum_pb[d2full[i]-1]);
    p.hv_d2full.push_back(hv_scans[d2full[i]-1]);
    p.Hv_d2full.push_back(HV_Scans[d2full[i]-1]);
    v.d2full_col<<col[d2full[i]-1]<<",";
  }  
  
  // HV Scans                         name,              npf, dim, eff, err, clusize, default, cumulative
  ah.hv_l1           = new Histograms("hv_l1",           2,   1,   1,   2);
  ah.hv_l3           = new Histograms("hv_l3",           2,   1,   1,   2);
  ah.hv_d1           = new Histograms("hv_d1",           2,   1,   1,   2);
  ah.hv_l1full       = new Histograms("hv_l1full",       3,   1,   1,   2);
  ah.hv_l2full       = new Histograms("hv_l2full",       3,   1,   1,   2);
  ah.hv_l3full       = new Histograms("hv_l3full",       3,   1,   1,   2);
  ah.hv_d1full       = new Histograms("hv_d1full",       3,   1,   1,   2);
  ah.hv_d2full       = new Histograms("hv_d2full",       3,   1,   1,   2);
  ah.vturnon_totlumi = new Histograms("vturnon_totlumi", 1,   1);

  ah.hv_l1     ->add(p.hv_l1,     1, 1, p.hv_l1.size(),     0, p.hv, 1, 1,  3,  31, -2.5, 152.5);
  ah.hv_l3     ->add(p.hv_l3,     1, 1, p.hv_l3.size(),     0, p.hv, 1, 1,  3,  31, -2.5, 152.5);
  ah.hv_d1     ->add(p.hv_d1,     1, 1, p.hv_d1.size(),     0, p.hv, 1, 1,  3,  62, -2.5, 307.5);
  ah.hv_l1full ->add(p.hv_l1full, 1, 1, p.hv_l1full.size(), 0, p.hv_full_bpix, 1, 1, 61, 0, p.hv, 1, 1, 3,  31, -2.5, 152.5);
  ah.hv_l2full ->add(p.hv_l2full, 1, 1, p.hv_l2full.size(), 0, p.hv_full_bpix, 1, 1, 73, 0, p.hv, 1, 1, 3,  31, -2.5, 152.5);
  ah.hv_l3full ->add(p.hv_l3full, 1, 1, p.hv_l3full.size(), 0, p.hv_full_bpix, 1, 1, 85, 0, p.hv, 1, 1, 3,  31, -2.5, 152.5);
  ah.hv_d1full ->add(p.hv_d1full, 1, 1, p.hv_d1full.size(), 0, p.hv_full_fpix, 1, 1, 64, 0, p.hv, 1, 1, 3,  62, -2.5, 307.5);
  ah.hv_d2full ->add(p.hv_d2full, 1, 1, p.hv_d2full.size(), 0, p.hv_full_fpix, 1, 1, 64, 0, p.hv, 1, 1, 3,  62, -2.5, 307.5);
  
  ah.hv_l1     ->add(p.hv_l1,     1, 1, p.hv_l1.size(),     0, p.hv, 1, 4,  4,  31, -2.5, 152.5, 100, 0, 100);
  ah.hv_l3     ->add(p.hv_l3,     1, 1, p.hv_l3.size(),     0, p.hv, 1, 4,  4,  31, -2.5, 152.5, 100, 0, 100);
  ah.hv_d1     ->add(p.hv_d1,     1, 1, p.hv_d1.size(),     0, p.hv, 1, 4,  4,  62, -2.5, 307.5, 100, 0, 100);
  ah.hv_l1full ->add(p.hv_l1full, 1, 1, p.hv_l1full.size(), 0, p.hv_full_bpix, 1, 1, 61, 0, p.hv, 1, 4, 4,  31, -2.5, 152.5, 100, 0, 100);
  ah.hv_l2full ->add(p.hv_l2full, 1, 1, p.hv_l2full.size(), 0, p.hv_full_bpix, 1, 1, 73, 0, p.hv, 1, 4, 4,  31, -2.5, 152.5, 100, 0, 100);
  ah.hv_l3full ->add(p.hv_l3full, 1, 1, p.hv_l3full.size(), 0, p.hv_full_bpix, 1, 1, 85, 0, p.hv, 1, 4, 4,  31, -2.5, 152.5, 100, 0, 100);
  ah.hv_d1full ->add(p.hv_d1full, 1, 1, p.hv_d1full.size(), 0, p.hv_full_fpix, 1, 1, 64, 0, p.hv, 1, 4, 4,  62, -2.5, 307.5, 100, 0, 100);
  ah.hv_d2full ->add(p.hv_d2full, 1, 1, p.hv_d2full.size(), 0, p.hv_full_fpix, 1, 1, 64, 0, p.hv, 1, 4, 4,  62, -2.5, 307.5, 100, 0, 100);

  ah.vturnon_totlumi ->add(p.det5, 1,  1, 5, 13100, -100, 13000);

#endif

  //________________________________________________________________________________________
  //                                          v2827

  // FILES
  //   effanalysis.addfile("/home/jkarancs/gridout/v2827/RECO/Run2011B-PromptReco-v1/ZeroBiasHPF0/*.root"); // hPU Fill 2201
  //   effanalysis.addfile("/home/jkarancs/gridout/v2827/RAW/ZeroBias_Run2011B-v1/*.root"); // 25ns RAW

  /* Previous Scans */
  // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV1/RECO/Commissioning10-May19ReReco-v1/MinimumBias/*.root");
  // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV2/RECO/Run2010B-Apr21ReReco-v1/MinimumBias/*.root");
  // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV3/RECO/Run2011A-PromptReco-v1/MinimumBias/*.root");
  // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV4/RECO/Run2011A-PromptReco-v4/MinimumBias/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2525/HV5/RECO/Run2011A-PromptReco-v5/MinBias0Tesla2/*.root"); // v2525
  // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/MinimumBias/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/Jet/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/MuHad/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/MultiJet/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/Photon/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/PhotonHad/*.root");
  // // effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV6-7/RECO/Run2011A-PromptReco-v5/SingleElectron/*.root");
  //   effanalysis.addfile("/home/jkarancs/gridout/Scans/v2625/HV8/RECO/Run2011B-PromptReco-v1/MinimumBias/MB_Run2011B_RECO_01*0.root");

  //________________________________________________________________________________________
  //                                          v2928

  // Inactivation test
  //   effanalysis.addfile("/home/jkarancs/localrun/RAW/CMSSW_4_4_0_patch3/src/DPGAnalysis/PixelTimingStudy/MB_Run2011B_RAW_newver.root");
  //   effanalysis.addfile("/home/jkarancs/localrun/RAW/CMSSW_4_4_0_patch3/src/DPGAnalysis/PixelTimingStudy/MB_Run2011B_RAW_newver_2.root");
  //   effanalysis.addfile("/home/jkarancs/localrun/RAW/l2_nomod/CMSSW_4_4_0_patch3/src/DPGAnalysis/PixelTimingStudy/MB_Run2011B_RAW_newver_nomod.root");
  //   effanalysis.addfile("/home/jkarancs/localrun/RAW/l2_nomod/CMSSW_4_4_0_patch3/src/DPGAnalysis/PixelTimingStudy/MB_Run2011B_RAW_newver_nomod_2.root");
  //   effanalysis.addfile("/home/jkarancs/localrun/RAW/inactive/CMSSW_4_4_0_patch3/src/DPGAnalysis/PixelTimingStudy/MB_Run2011B_RAW_newver_inactive.root");
  //   effanalysis.addfile("/home/jkarancs/localrun/RAW/inactive/CMSSW_4_4_0_patch3/src/DPGAnalysis/PixelTimingStudy/MB_Run2011B_RAW_newver_inactive_2.root");
  
  // NOTE DATA
  // All MinimumBias RECO for 2010/2011 - INC, SPLIT0, Nstrip>10
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2010A-Apr21ReReco-v1/MinimumBias/*.root");
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2010B-Apr21ReReco-v1/MinimumBias/*.root");
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-May10ReReco-v2/MinimumBias/*.root");
//     effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v4/MinimumBias/*.root");
//     effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v4/MinimumBias/*.root");
//     effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v5/MinimumBias/*.root");
//     effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v6/MinimumBias/*.root");
//     effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011B-PromptReco-v1/MinimumBias/*.root");
  
  //effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011B-PromptReco-v1/MinimumBias/*.root");
  
//   effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v6/MinimumBias/",416,476); //Fill 2040
  //   effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011B-PromptReco-v1/MinimumBias/",113,153); //Fill 2103
  //   effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011B-PromptReco-v1/MinimumBias/",172,242); //Fill 2105
  //   effanalysis.addfile("/data2/jkarancs/gridout/v2928/RECO/Run2011B-PromptReco-v1/MinimumBias/",1103,1157); //Fill 2208
  
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2010A-Apr21ReReco-v1/MinimumBias/",1,1);
  
  // NOTE DATA - for N-1 Plots
  // MinimumBias RECO samples for 2011 - INC, SPLIT2, Nstrip>0
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-May10ReReco-v2/MinimumBias/SPLIT2/*.root");
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v5/MinimumBias/SPLIT2/*.root");
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-PromptReco-v6/MinimumBias/SPLIT2/*.root");
  //   effanalysis.addfile("/home/jkarancs/gridout/v2928/RECO/Run2011B-PromptReco-v1/MinimumBias/SPLIT2/*.root");
  
  // effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-May10ReReco-v2/MinimumBias/SPLIT2/",1,1);
  
  
  // MinimumBias RECO - 4 long runs 2011 - COMPLETE, SPLIT2
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011A-08Nov2011-v1/MinimumBias/COMP_SPL2/*.root");
  //   effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011B-19Nov2011-v1/MinimumBias/COMP_SPL2/*.root");
  
  // effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011B-19Nov2011-v1/MinimumBias/COMP_SPL2/",1,265); // Fill 2178
  // effanalysis.addfile("/data/jkarancs/gridout/v2928/RECO/Run2011B-19Nov2011-v1/MinimumBias/COMP_SPL2/",1,1);
  
  //________________________________________________________________________________________
  //                                          v3029
  
  // 2012A May11_JSON
  // effanalysis.addfile("/data/jkarancs/gridout/v3029/MinimumBias_Run2012A-PromptReco-v1_RECO/*.root");
  
  // Fill 2533
//   effanalysis.addfile("/data/jkarancs/gridout/v3029/ZeroBias1_Run2012A-v1_RAW/*.root");
  
  // Timing Scans
  // 2011
//   effanalysis.addfile("/data/jkarancs/gridout/Scans/v3029/Timing_scan_2011/*.root");
  // 2012
//   effanalysis.addfile("/data/jkarancs/gridout/Scans/v3029/Timing_scan_2012/*.root");
  
  //effanalysis.addfile("/data/jkarancs/gridout/Scans/v3029/HV13_TimingScan3-4/ZeroBias1-4_Run2012C-v1_RAW/",1,528); //TS3
  //effanalysis.addfile("/data2/jkarancs/gridout/Scans/v3029/HV13_TimingScan3-4/ZeroBias1-4_Run2012C-v1_RAW/",1159,1278); //TS4
  


  // HV Scans
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV1/*.root");
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV2/*.root");
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV3_4_6_7/*.root");
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV5/*.root");
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV8-11/*.root");
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV12/*.root");
//   effanalysis.addfile("/data/jkarancs/gridout/Scans/v3029/HV12/*.root");
//   effanalysis.addfile("/data/jkarancs/gridout/Scans/v3029/HV13_TimingScan3-4/ZeroBias1-4_Run2012C-v1_RAW/",228,1000); // HV13 (1)
//   effanalysis.addfile("/data2/jkarancs/gridout/Scans/v3029/HV13_TimingScan3-4/ZeroBias1-4_Run2012C-v1_RAW/",1001,1158); // HV13 (2)
  
//   effanalysis.addfile("/home/jkarancs/gridout/Scans/v3029/HV8-11/",132,194); // HV11
  
  // effanalysis.addfile("/data/jkarancs/gridout/Scans/v3029/HV12/RECO/ZeroBias1_Run2012A-PromptReco-v1/INC_SPL2_nstrip0/",123,530);
  
  
  // effanalysis.addfile("/home/jkarancs/gridout/v3029/FEVT/ExpressPhysics_Run2012B-Express-v1_FEVT/*.root");
  
  // effanalysis.addfile("/data/jkarancs/gridout/v3029/ExpressPhysics_Run2012C-Express-v1_FEVT/Fill2807/*.root");
  // effanalysis.addfile("/data/jkarancs/gridout/v3029/ExpressPhysics_Run2012C-Express-v1_FEVT/Fill2816/*.root");
  
  //effanalysis.addfile("/data2/jkarancs/gridout/v3029/MinimumBias_Run2012B-PromptReco-v1_RECO/*.root");

  effanalysis.addfile("/data2/jkarancs/gridout/v3029/MinimumBias_Run2012B-PromptReco-v1_RECO/MB_RECO_0002.root");
  
  
  // Run on lumiTree to get run and lumi information
  effanalysis.preprocess(tr, v, ah, p);
#if FILLS != 1
  v.lumi_nrun = 1;
  v.lumi_nfill = 0;
#endif

  //________________________________________________________________________________________
  //                                  Fill-by-Fill Plots
  //________________________________________________________________________________________
  //                                    Efficiency Plots
  //________________________________________________________________________________________
  //                               name,             npf, dim, eff, err, clusize, default, cumulative
  ah.run          = new Histograms("run",            3,   1,   1,   2,   0,       1);
  ah.det          = new Histograms("det",            2,   1,   1,   2);
  ah.mod          = new Histograms("mod",            3,   2,   1,   2);
  ah.roc          = new Histograms("roc",            3,   2,   1,   2);
  ah.time         = new Histograms("time",           3,   1,   1,   2);
  ah.bx           = new Histograms("bx",             3,   1,   1,   2);
  ah.instlumi     = new Histograms("instlumi",       3,   1,   1,   2);
  ah.instlumi_raw = new Histograms("instlumi_raw",   3,   1,   1,   2);
  ah.il_l1rate    = new Histograms("il_l1rate",      3,   2,   1,   2);
  ah.trigger      = new Histograms("trigger",        3,   1,   1,   2);
#if BADROC != 0
  ah.roceff_time  = new Histograms("roceff_time",    3,   3,   1,   2);
  ah.roceff_fill  = new Histograms("roceff_fill",    2,   3,   1,   2);
  ah.fill_stat    = new Histograms("fill_stat",      1,   1,   1,   2);
  ah.badroc       = new Histograms("badroc",         2,   2);
  ah.nbadroc      = new Histograms("nbadroc",        2,   1);
  ah.roceff_dist  = new Histograms("roceff_dist",    1,   1);
#endif

  ah.run           ->add(p.det, 1, 1, 10, 0, p.mod2,   1, 1, 5, 0, p.def, 1, 1, 12, v.lumi_nrun, 0.5, v.lumi_nrun + 0.5);
//ah.run           ->add(p.det, 1, 1, 10, 0, p.cumeff, 1, 1, 2,                           v.lumi_nrun, 0.5, v.lumi_nrun + 0.5);
  ah.det           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.cumeff, 1,  1, 2,     7,0.5,7.5);
  ah.mod           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  4, 4,  3, p.cumeff, 1, 1, 2,  9, -4.5, 4.5,  21, -10.5,  10.5);
  ah.mod           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  5, 5,  3, p.cumeff, 1, 1, 2,  9, -4.5, 4.5,  33, -16.5,  16.5);
  ah.mod           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  6, 6,  3, p.cumeff, 1, 1, 2,  9, -4.5, 4.5,  45, -22.5,  22.5);
  ah.mod           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  3, 3, -1, p.cumeff, 1, 1, 2,  9, -4.5, 4.5,  25, -12.5,  12.5);
  ah.roc           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  4, 4,  3, p.inac,   1, 1, 3, 72, -4.5, 4.5,  42, -10.5,  10.5);
  ah.roc           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  5, 5,  3, p.inac,   1, 1, 3, 72, -4.5, 4.5,  66, -16.5,  16.5);
  ah.roc           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1,  6, 6,  3, p.inac,   1, 1, 3, 72, -4.5, 4.5,  90, -22.5,  22.5);
  ah.roc           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.fpixIO, 1,  1, 1, -3, p.inac,   1, 1, 3, 72, -4.5, 4.5, 144,   0.5,  12.5);
  ah.roc           ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.fpixIO, 1,  2, 2, -3, p.inac,   1, 1, 3, 72, -4.5, 4.5, 144, -12.5,  -0.5);
  ah.time          ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.cumeff, 1, 1, 2,  96,   0,  24);
  ah.bx            ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.cumeff, 1, 1, 2,  120,    0, 3600);
  ah.instlumi      ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.cumeff, 1, 1, 2,   80,    0, 4000);
  ah.instlumi_raw  ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.cumeff, 1, 1, 2,   80,    0, 4000);
  ah.il_l1rate     ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.cumeff, 1, 1, 2,   80,    0, 4000, 50,    0, 100000);
  ah.trigger       ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.cumeff, 1, 1, 2,   32, -0.5, 31.5);



#if BADROC != 0
  ah.roceff_time  ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.detshl, 1, 1,  4, 0, p.cumeff, 1, 1, 2,   48,    0,    24,  48, -0.5, 47.5,  64, -0.5, 63.5);
  ah.roceff_time  ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.detshl, 1, 5,  8, 0, p.cumeff, 1, 1, 2,   48,    0,    24,  24, -0.5, 23.5,  45, -0.5, 44.5);
  ah.roceff_fill  ->add(p.detshl, 1, 1,  4, 0, p.cumeff, 1, 1, 2,  9999, 0.5, 9999.5,  48, -0.5, 47.5,  64, -0.5, 63.5);
  ah.roceff_fill  ->add(p.detshl, 1, 5,  8, 0, p.cumeff, 1, 1, 2,  9999, 0.5, 9999.5,  24, -0.5, 23.5,  45, -0.5, 44.5);
  ah.fill_stat    ->add(p.cumeff, 1, 1, 2,  9999, 0.5, 9999.5);
  ah.badroc       ->add(p.fill,  1, 2, v.lumi_nfill+1, 1, p.det,    1, 3,  6,    48, 0, 24,  100, 0, 100);
  ah.nbadroc      ->add(p.fill,  1, 2, v.lumi_nfill+1, 1, p.det,    1, 1,  6,    48, 0, 24);
  ah.roceff_dist  ->add(p.fill,  1, 2, v.lumi_nfill+1,    1000, 0,  1);
#endif

#if FEDERRPLOTS == 1
  ah.time_federr   = new Histograms("time_federr",    4,   1,   1,   2);
  ah.federr        = new Histograms("federr",         2,   1,   1,   2);
  ah.federr_evt    = new Histograms("federr_evt",     2,   1);
  ah.federr        ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.cumeff, 1, 1, 2,  42, -1.5, 40.5);
  ah.federr_evt    ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.cumeff, 1, 2, 2,  42, -1.5, 40.5);
  ah.time_federr   ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.federr_type,  1, 1, 18, 0, p.det, 1, 1, 10, 0, p.federr, 1, 1, 5,  96,   0,  24);
  ah.time_federr   ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.federr_type,  0, 1, 18, 0, p.det, 0, 1,  1, 0, p.federr, 1, 6, 8,  96,   0,  24);
  ah.time_federr   ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.federr_type,  1,19, 20, 0, p.det, 1, 1, 10, 0, p.federr, 1, 1, 2,  96,   0,  24);
#endif

  //________________________________________________________________________________________
  //                                Not Efficiency Plots:
  //  Nclu plots: nclu/npix_corr, bx_ls, ls_fill, totlumi_ratio, time/instlumi (nclu part)
  //________________________________________________________________________________________
#ifdef COMPLETE
  ah.nclu_corr     = new Histograms("nclu_corr",      2,   2);
  ah.npix_corr     = new Histograms("npix_corr",      2,   2);
  ah.bx_ls         = new Histograms("bx_ls",          3,   2);
  ah.ls_fill       = new Histograms("ls_fill",        3,   1);
  ah.instlumi_corr = new Histograms("instlumi_corr",  2,   1);
  ah.nclu_corr     ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.corr,   1, 1, 2,  200,0,2000,  200,0,2000);
  ah.npix_corr     ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.corr,   1, 1, 2,  200,0,10000, 200,0,10000);
  ah.bx_ls         ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.avg,   1, 2, 2,  40,0,4000, 3600,0,3600);
  ah.bx_ls         ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.det,    0, 1, 10, 0, p.avg,   1, 4, 4,  40,0,4000, 3600,0,3600);
  ah.ls_fill       ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.det,    1, 1, 10, 0, p.avg,   1, 1, 1,  4000,0,4000);
  ah.ls_fill       ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.det,    0, 1, 10, 0, p.avg,   1, 2, 2,    40,0,4000);
  ah.ls_fill       ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.det,    0, 1, 10, 0, p.avg,   1, 3, 4,  4000,0,4000);
  ah.time          ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.det,    0, 1, 10, 0, p.avg,   1, 1, 1,  96,0,24);
  ah.instlumi      ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.det,    0, 1, 10, 0, p.avg,   1, 1, 1,  80,0,4000);
  ah.instlumi      ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.corr2,  1, 1, 2,  40,0,4000, 200,0, 2000, 200,0, 2000);
  ah.instlumi      ->add(p.fill,  0, 1, v.lumi_nfill+1, 0, p.corr2,  1, 3, 4,  40,0,4000, 200,0,10000, 200,0,10000);
  ah.instlumi_corr ->add(p.fill,  1, 1, v.lumi_nfill+1, 0, p.corr2,  1, 1, 4,  40,0,4000);
#endif
  
  // Load Previous plots
  // ah.load_HV_scans(p);
  // ah.load_totlumi();
  
  CanvasMaker canvasmaker(NONEFFPLOTS, 1, SCANPLOTS);

  effanalysis.process(tr, v, ah);
  effanalysis.postprocess(tr, v, ah, p);

  std::string workdir = "/afs/kfki.hu/home/jkarancs/public/ROOT_output/";

//   effanalysis.write_output(workdir+"v3029/TEST/","Note_histos","test", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","instlumi_raw","Fill_2040_2103_2105_2208_INC_SPL0", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","Note_histos_pt1GeV","test_del", ah, v);
//   effanalysis.write_output(workdir+"v3029/","Note_histos_pt1GeV_lx4_ly5","2010_2011_MinimumBias_RECO_428p3", ah, v);
//   effanalysis.write_output(workdir+"v3029/","Note_histos_pt1GeV","2010_2011_MinimumBias_RECO_428p3_SPL2_allNstrip", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","Performance_plots","2010_2011_MinimumBias_RECO_SPL2_allNstrip_test", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","Performance_plots","2010_2011_MinimumBias_RECO_428p3_001", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","MPV_fix","HV8", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","nclu_corr","2011_longruns_COMP_SPL2", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","FEDerr_validation","520_MB_RAW", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","new_cuts","test_del2", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","nclu_corr","2011_Fill2178_COMP_SPL2", ah, v);
//   effanalysis.write_output(workdir+"v3029/","roc_dcol_occupancy","2011_Fill2178_COMP_SPL2", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","Timing_scan11_12","INC_SPL0_nstrip0", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","HV1-12_newplots","INC_SPL0_nstrip0", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","HV12","ZBN_RAW_INC_SPL0_nstrip0", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","Fill2533","ZB1_RAW_INC_SPL2_nstrip0", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","Run2012A_May11JSON","MB_RECO_INC_SPL0_nstrip11", ah, v);
  //effanalysis.write_output(workdir+"v3029/TEST/","HV1-13","ZBN_RAW_INC_SPL0_nstrip0", ah, v);
  //effanalysis.write_output(workdir+"v3029/TEST/","Timing_scan1-4","ZBN_RAW_INC_SPL0_nstrip0", ah, v);
  //effanalysis.write_output(workdir+"v3029/TEST/","Latency_Change","EP_FEVT_INC_SPL2_nstrip0", ah, v);
  //effanalysis.write_output(workdir+"v3029/TEST/","totlumi_Run2012B","MB_RECO_INC_SPL0_nstrip11", ah, v);
  //effanalysis.write_output(workdir+"v3029/TEST/","Timing_scan3_trkloss","ZBN_RAW_INC_SPL0_nstrip0", ah, v);
  effanalysis.write_output("","example_output","test", ah, v);
  
  canvasmaker.run(ah, effanalysis.outfile().insert(effanalysis.outfile().find(".root"),"_can"),v,p);

//   canvasmaker.load_from_file(ah,workdir+"v2928/TEST/Instlumi_withSEU_withFEDerr_test_v2928.root");
//   canvasmaker.run(ah,workdir+"v2928/TEST/CANVASES/Instlumi_withSEU_withFEDerr_test_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2928/TEST/instlumi_raw_Fill_2040_2103_2105_2208_INC_SPL0_v2928.root");
//   canvasmaker.run(ah,workdir+"v2928/TEST/CANVASES/instlumi_raw_Fill_2040_2103_2105_2208_INC_SPL0_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2928/TEST/nclu_corr_2011_longruns_COMP_SPL2_v2928c.root");
//   canvasmaker.run(ah,workdir+"v2928/TEST/CANVASES/nclu_corr_12bx_400LS_fromorigo_2011_longruns_COMP_SPL2_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2928/TEST/nclu_corr_2011_Fill2178_COMP_SPL2.root");
//   canvasmaker.run(ah,workdir+"v2928/TEST/CANVASES/nclu_corr_2011_Fill2178_COMP_SPL2_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2928/Note_histos_pt1GeV_lx4_ly5_2010_2011_MinimumBias_RECO_428p3.root");
//   canvasmaker.run(ah,workdir+"v2928/CANVASES/Publication_2010_2011_RAW_efficiency_can.root",v,p);
//   canvasmaker.run(ah,workdir+"v2928/CANVASES/Note_histos_pt1GeV_2010_2011_MinimumBias_RECO_428p3_SPL2_allNstrip_can.root",v,p);
//   canvasmaker.run(ah,workdir+"v2928/TEST/CANVASES/Note_histos_test_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2928/roc_dcol_occupancy_2011_Fill2178_COMP_SPL2.root");
//   canvasmaker.run(ah,workdir+"v2928/CANVASES/roc_dcol_occupancy_2011_Fill2178_COMP_SPL2_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v3029/TEST/Timing_scan_ZBN_RAW_INC_SPL0_nstrip0.root");
//   canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/Timing_scan_ZBN_RAW_INC_SPL0_nstrip0_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v3029/TEST/Timing_scan11_12_no_pixhit_fpix_INC_SPL0_nstrip0.root");
//   canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/Timing_scan11_12_no_pixhit_fpix_INC_SPL0_nstrip0_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v3029/TEST/HV1-12_newplots_INC_SPL0_nstrip0.root");
//   canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/HV1-12_newplots_INC_SPL0_nstrip0_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v3029/TEST/Run2012A_May11JSON_MB_RECO_INC_SPL0_nstrip11.root");
//   canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/Run2012A_May11JSON_MB_RECO_INC_SPL0_nstrip11_can.root",v,p);
  // canvasmaker.load_from_file(ah,workdir+"v3029/TEST/Latency_Change_EP_FEVT_INC_SPL2_nstrip0.root");
  // canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/Latency_Change_EP_FEVT_INC_SPL2_nstrip0_can.root",v,p);
  //canvasmaker.load_from_file(ah,workdir+"v3029/TEST/Timing_scan1-4_ZBN_RAW_INC_SPL0_nstrip0.root");
  //canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/Timing_scan1-4_ZBN_RAW_INC_SPL2_nstrip0_can.root",v,p);
  // canvasmaker.load_from_file(ah,workdir+"v3029/TEST/HV1-13_ZBN_RAW_INC_SPL0_nstrip0.root");
  // canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/HV1-13_ZBN_RAW_INC_SPL0_nstrip0_can.root",v,p);

  // canvasmaker.load_from_file(ah,workdir+"v3029/TEST/totlumi_Run2012B_MB_RECO_INC_SPL0_nstrip11.root");
  // canvasmaker.run(ah,workdir+"v3029/TEST/CANVASES/totlumi_Run2012B_MB_RECO_INC_SPL0_nstrip11_can.root",v,p);

//   canvasmaker.load_from_file(ah,workdir+"v2928/Note_histos_2010_2011_MinimumBias_RECO_428p3_v2928.root ");
//   canvasmaker.run(ah,workdir+"v2928/CANVASES/Note_histos_2010_2011_MinimumBias_RECO_428p3_can.root",v,p);
//    canvasmaker.load_from_file(ah,workdir+"v2928/TEST/Timing_scan_SPL2_nstrip0_test.root");
//    canvasmaker.run(ah,workdir+"v2928/TEST/CANVASES/Timing_scan_SPL2_nstrip0_test_can.root",v,p);





//   effanalysis.write_output(workdir+"v3029/TEST/","rawtoreco","test", ah, v);
//   effanalysis.write_output(workdir+"v3029/","lxly","2010and2011runs_june3JSON", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","bx","2010Aand2011A_jul6JSON", ah, v);
//   effanalysis.write_output(workdir+"v3029/","federr_timeout_01","2010Band2011Aruns_jul6JSON", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","test_data","SingleElectron", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","pt_eta_avgXsize","2011A_aug13JSON_ZeroBias_1", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","test","HV1-8_MB", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","rocseloff_err2","Fill_2040_RAW", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","bx_rebin","Fill_2040_2103_2105", ah, v);
//   effanalysis.write_output(workdir+"v3029/","ROCON","2011B_5e33", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","test","2208_bkg_noscraping", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","test","RUN2011B_5e33", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","debug","bugged_file", ah, v);
//   effanalysis.write_output(workdir+"v3029/TEST/","SE_test","Fill_2208_SE_RAW", ah, v);
//   effanalysis.write_output(workdir+"v3029/","instlumi_federr_evtsel","Fill_2208_MB_RAW", ah, v);
//   effanalysis.write_output(workdir+"v3029/","allfilters_noRS_noFES","Fill_2208_allfilters_MB", ah, v);
//   effanalysis.write_output(workdir+"v3029/","federr_evtsel","Fill_2208_RAW_MB_2_4mis", ah, v);
//   effanalysis.write_output(workdir+"v3029/","federr_evtsel","Fill_2105_RAW_MB", ah, v);
//   effanalysis.write_output(workdir+"v3029/","scan_mpv_100entryfit","HV1-8", ah, v);
//   effanalysis.write_output(workdir+"v3029/","instlumi_l1rate","2011B", ah, v);

//   canvasmaker.load_from_file(ah,workdir+"v2321/TEST/splittest_test_neweff_v2321.root");
//   canvasmaker.run(ah,workdir+"v2321/TEST/CANVASES/splittest.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2220/pixelTracks_multiplicity70_2010runs_neweff_v2220.root");
//   canvasmaker.run(ah,workdir+"v2220/CANVASES/pixelTracks_multiplicity70.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2523/TEST/bx_2010Aand2011A_jul6JSON_neweff_v2523.root");
//   canvasmaker.run(ah,workdir+"v2523/TEST/CANVASES/bx_2010Aand2011A_jul6JSON_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2525/TEST/HVjul_aug_all_layer_EP_neweff_v2525.root");
//   canvasmaker.run(ah,workdir+"v2525/TEST/CANVASES/HVjul_aug_all_layer_EP_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/TEST/pt_aoi_avgXsize_2011A_aug13JSON_ZeroBias_01_neweff_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/TEST/CANVASES/pt_aoi_avgXsize_2011A_aug13JSON_ZeroBias_01_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/TEST/test_fill_2040_neweff_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/TEST/CANVASES/test_fill_2040_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2525/TEST/test_HV1-8_MB_neweff_v2525.root");
//   canvasmaker.run(ah,workdir+"v2525/TEST/CANVASES/test_HV1-8_MB_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/TEST/time_instlumi_Fill_2040_neweff_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/TEST/CANVASES/time_instlumi_Fill_2040_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/TEST/rocseloff_err2_Fill_2040_RAW_neweff_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/TEST/CANVASES/rocseloff_err2_Fill_2040_RAW_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/TEST/bx_rebin_Fill_2040_2103_2105_neweff_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/TEST/CANVASES/bx_rebin_Fill_2040_2103_2105_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/TEST/rocseloff_Fill_2105_neweff_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/TEST/CANVASES/rocseloff_Fill_2105_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2827/TEST/il_l1rate_test_neweff_v2827.root");
//   canvasmaker.run(ah,workdir+"v2827/TEST/CANVASES/il_l1rate_test_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2827/TEST/federr_rate_Fill_2208_SE_RAW_v2827.root");
//   canvasmaker.run(ah,workdir+"v2827/TEST/CANVASES/federr_rate_Fill_2208_SE_RAW_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2827/ROCON_2011B_hPUF_v2827.root");
//   canvasmaker.run(ah,workdir+"v2827/CANVASES/ROCON_2011B_hPUF_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2827/instlumi_l1rate_2011B_v2827.root");
//   canvasmaker.run(ah,workdir+"v2827/CANVASES/instlumi_l1rate_2011B_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2827/allfilters_noRS_noFES_Fill_2208_allfilters_MB_v2827.root");
//   canvasmaker.run(ah,workdir+"v2827/CANVASES/allfilters_noRS_noFES_Fill_2208_allfilters_MB_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2625/federror_evtsel_Fill_2040_2103_2105_RAW_v2625.root");
//   canvasmaker.run(ah,workdir+"v2625/CANVASES/federror_evtsel_Fill_2040_2103_2105_RAW_can.root",v,p);
//   canvasmaker.load_from_file(ah,workdir+"v2827/scan_mpv_HV9-11_v2827.root");
//   canvasmaker.run(ah,workdir+"v2827/CANVASES/scan_mpv_HV9-11_can.root",v,p);
  return 1;
}
