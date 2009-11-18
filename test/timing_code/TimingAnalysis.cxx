
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

#include "DataStructures.h"
#include "debDataMaker/interface/Histogram.hh"
#include "debDataMaker/interface/Plot.hh"


////////////////////////////////////////////////////////////////////////////////
//
// RecHit based plots
//


int doRecHitPlots(TChain *trajTree) {

  std::string plotType="RecHits";
  std::ostringstream plotName;
  std::ostringstream canvasName;
  std::ostringstream histName;


  //
  // Set the tree for reading
  //

  EventData evt;
  TrajMeasurement trajmeas;
  trajTree->SetBranchAddress("event", &evt);
  trajTree->SetBranchAddress("traj", &trajmeas);
  trajTree->SetBranchAddress("module", &trajmeas.mod);
  trajTree->SetBranchAddress("module_on", &trajmeas.mod_on);
  trajTree->SetBranchAddress("clust", &trajmeas.clu);
  trajTree->SetBranchAddress("track", &trajmeas.trk);

  std::cout<<"Reading trajTree of entries: "<<trajTree->GetEntries()<<std::endl;


  //
  // Preprocessing cycle
  //

  std::vector<int> runs;
  std::map<int, int> delays;
  std::string prevFileName="";

  for (Long64_t i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);
    std::string fname=trajTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }
    if (find(runs.begin(), runs.end(), evt.run)==runs.end()) {
      runs.push_back(evt.run);
    
      int delay=NOVAL_I;
      if (evt.run<111140) delay=0;
      if (evt.run>112080) {
	delay= (evt.delay%12) ? evt.delay/2-25 : evt.delay/2;
	delays[evt.run]=delay;
      }
    }
  }

  std::sort(runs.begin(), runs.end(), std::less<int>());
  std::cout<<"Processing runs "<<runs.front()<<" to "<<runs.back()<<std::endl;


  //
  // Booking histograms
  //

  // 1. Hit Efficiency vs DT time
  deb::Plot<deb::Histogram<TH1F> > dttimeeff(100, 100, 
					     "%s_dttimeeff\\TDR", plotType.data());
  dttimeeff.add(35,-35.,35., "@6;Hit efficiency in the @#1. Barrel @#2. pixel;"
		"DT time [ns];Efficiency");
  dttimeeff.efficiency();
  std::cout << "dttimeeff created" <<std::endl;



  // 2. Efficiency vs run
  plotName.str("");
  plotName << plotType << "_runeff";
  canvasName.str("");
  canvasName << plotName.str() << "_can";
  TCanvas* runeff_can = new TCanvas(canvasName.str().data(), 
				    canvasName.str().data(), 600,600);
  runeff_can->SetTitle("Hit efficiency in the Barrel pixel in CRAFT09 runs");
  TH1D *runeff_num=NULL, *runeff_den=NULL;

  for (int i=0; i<2; i++) {
    histName.str("");
    histName << plotName.str() << ((i==0) ? "_num" : "_den");
    TH1D *h = new TH1D(histName.str().data(), histName.str().data(), 
		       (Int_t)(runs.back()-runs.front()), 
		       runs.front(), runs.back());
    h->Sumw2();
    if (i==0) runeff_num=h; else runeff_den=h;
  }
  runeff_num->SetTitle("Hit efficiency in the Barrel pixel in CRAFT09 runs;Run Number;Efficiency");

  // 3. Module Efficiency
  std::vector<deb::Histogram<TH2F>* > modeff;
  for (size_t i=0; i<6; i++) {    
    modeff.push_back(new deb::Histogram<TH2F> ("","Hit efficiency in the Barrel pixel;"\
					       "Ladder;Module in Layer 1-3", 44,0.5,44.5, 30,0.5,30.5));
    modeff[i]->setLabel("Name", "%s_modeff_%d", plotType.data(), i);
    modeff[i]->efficiency();
  }

  TCanvas modeff_can(modeff[0]->getLabel("Name", "$_canvas").data(), 
		     modeff[0]->getLabel("Title").data(), 550,900);
  modeff_can.Divide(2,3);


//   plotName.str("");
//   plotName << plotType << "_modeff";
//   canvasName.str("");
//   canvasName << plotName.str() << "_can";
//   TCanvas* modeff_can = new TCanvas(canvasName.str().data(), 
// 				    canvasName.str().data(), 600,600);
//   TH2D *modeff_num=NULL, *modeff_den=NULL;

//   for (int i=0; i<2; i++) {
//     histName.str("");
//     histName << plotName.str() << ((i==0) ? "_num" : "_den");
//     TH2D *h = new TH2D(histName.str().data(), histName.str().data(), 
// 		       44, 0.5, 44.5, 30, 0.5, 30.5);
//     h->Sumw2();
//     if (i==0) modeff_num=h; else modeff_den=h;
//   }

  // 4. ROG Efficiency
  std::vector<deb::Histogram<TH2F>* > rogeff;
  for (size_t i=0; i<6; i++) {    
    rogeff.push_back(new deb::Histogram<TH2F> ("","Hit efficiency in the Barrel pixel;"\
					       "Sector;PRT1 and PRT2 in BpI/BpO/BmI/BmO", 
					       8,0.5,8.5, 12,-0.5,11.5));
    rogeff[i]->setLabel("Name", "%s_rogeff_%d", plotType.data(), i);
    rogeff[i]->efficiency();
  }

  TCanvas rogeff_can(rogeff[0]->getLabel("Name", "$_canvas").data(), 
		     rogeff[0]->getLabel("Title").data(), 550,900);
  rogeff_can.Divide(2,3);

  // 4. ROG Efficiency
//   plotName.str("");
//   plotName << plotType << "_rogeff";
//   canvasName.str("");
//   canvasName << plotName.str() << "_can";
//   TCanvas* rogeff_can = new TCanvas(canvasName.str().data(), 
// 				    canvasName.str().data(), 600,600);
//   TH2D *rogeff_num=NULL, *rogeff_den=NULL;

//   for (int i=0; i<2; i++) {
//     histName.str("");
//     histName << plotName.str() << ((i==0) ? "_num" : "_den");
//     TH2D *h = new TH2D(histName.str().data(), histName.str().data(), 
// 		       8, 0.5, 8.5, 12, -0.5, 11.5);
//     h->Sumw2();
//     if (i==0) rogeff_num=h; else rogeff_den=h;
//   }


  // 5. Delay vs run
  deb::Histogram<TH1F> deltime("", "Delay of the Barrel pixel;Runnumber;Delay", 
			       (Int_t)(runs.back()-runs.front()),runs.front(),runs.back());
  deltime.setLabel("Name", "%s_deltime", plotType.data());
  deltime.efficiency();

  TCanvas deltime_can(deltime.getLabel("Name", "$_canvas").data(), 
		      deltime.getLabel("Title").data(), 600,600);


  // 6. Hit Efficiency vs delay
  deb::Histogram<TH1F> deleff("","Hit efficiency in the Barrel pixel;Delay[ns];Efficiency",
			      6, -15, 21);
  deleff.setLabel("Name", "%s_deleff", plotType.data());
  deleff.efficiency();

  TCanvas deleff_can(deleff.getLabel("Name", "$_canvas").data(), 
		     deleff.getLabel("Title").data(), 600,600);


  // 7. normalized Cluster charge vs delay
  deb::Histogram<TH1F> delnormcluschrg("","Average normalized cluster charge of valid hits in the Barrel pixel;Delay[ns];Charge [ke]",
			      6, -15, 21);
  delnormcluschrg.setLabel("Name", "%s_delnormcluschrg", plotType.data());
  delnormcluschrg.efficiency();

  TCanvas delnormcluschrg_can(delnormcluschrg.getLabel("Name", "$_canvas").data(), 
			      delnormcluschrg.getLabel("Title").data(), 600,600);



  // 8. average Cluster size vs delay
  deb::Histogram<TH1F> delclussize("","Average cluster size of valid hits in the Barrel pixel;Delay[ns];Size",
			      6, -15, 21);
  delclussize.setLabel("Name", "%s_delclussize", plotType.data());
  delclussize.efficiency();

  TCanvas delclussize_can(delclussize.getLabel("Name", "$_canvas").data(), 
		      delclussize.getLabel("Title").data(), 600,600);



  //
  // Filling histograms
  //

  for (long i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);
    int step=(delays[evt.run]!=NOVAL_I) ? delays[evt.run]/6+2 : NOVAL_I;
    //if (evt.run>111500) continue;
    deltime.num()->Fill(evt.run, delays[evt.run]);
    deltime.den()->Fill(evt.run);

    if (trajmeas.telescope && trajmeas.mod_on.det==0) {

      if (step!=NOVAL_I) {
	dttimeeff(step).den()->Fill(evt.tmuon);
	if (trajmeas.validhit) dttimeeff(step).num()->Fill(evt.tmuon);

	int modeff_y=trajmeas.mod.module+(trajmeas.mod.layer-1)*10;
	modeff[step]->den()->Fill(trajmeas.mod.ladder, modeff_y);
	if (trajmeas.validhit) modeff[step]->num()->Fill(trajmeas.mod.ladder, modeff_y);

	int rogeff_y=trajmeas.mod_on.shl*3+trajmeas.mod_on.prt-1;
	rogeff[step]->den()->Fill(trajmeas.mod_on.sec, rogeff_y);
	if (trajmeas.validhit) rogeff[step]->num()->Fill(trajmeas.mod_on.sec, rogeff_y);
      }

      runeff_den->Fill(evt.run);
      if (trajmeas.validhit) runeff_num->Fill(evt.run);

//       int modeff_y=trajmeas.mod.module+(trajmeas.mod.layer-1)*10;
//       modeff_den->Fill(trajmeas.mod.ladder, modeff_y);
//       if (trajmeas.validhit) {
// 	modeff_num->Fill(trajmeas.mod.ladder, modeff_y);
//       }

//       int rogeff_y=trajmeas.mod_on.shl*3+trajmeas.mod_on.prt-1;
//       rogeff_den->Fill(trajmeas.mod_on.sec, rogeff_y);
//       if (trajmeas.validhit) {
// 	rogeff_num->Fill(trajmeas.mod_on.sec, rogeff_y);
//       }

      deleff.den()->Fill(delays[evt.run]);
      if (trajmeas.validhit) deleff.num()->Fill(delays[evt.run]);
    }

    if (trajmeas.validhit) {
      delnormcluschrg.den()->Fill(delays[evt.run]);
      delnormcluschrg.num()->Fill(delays[evt.run], trajmeas.norm_charge);
      delclussize.den()->Fill(delays[evt.run]);
      delclussize.num()->Fill(delays[evt.run], trajmeas.clu.size);
    }

  }


  //
  // Post-processing and drawing histograms
  //
  dttimeeff.efficiency();
  dttimeeff.scaleMaximumTo("denominator");
  dttimeeff.setColor("Line", 1);
  dttimeeff.setAxisRange(0., 1.1, "Y");
  dttimeeff.Draw("[0],[1],[2], [3], [4], [5];overlay HIST\\"
		 "[0].den,[1].den,[2].den,[3].den,[4].den,[5].den;overlayHISTL",
		 1);
  dttimeeff.Write();


  modeff_can.SetBorderSize(0);
  modeff_can.SetFillColor(0);
  modeff_can.Draw();
  for (size_t i=0; i<6; i++) {
    modeff[i]->efficiency();
    modeff[i]->SetAxisRange(0, 1.0, "Z");
    modeff_can.cd(i+1);
    //modeff[i].SetBorderSize(0);
    modeff[i]->SetFillColor(0);
    modeff[i]->Draw("COLZ");
    modeff[i]->Write();
  }
  modeff_can.Write();


  rogeff_can.SetBorderSize(0);
  rogeff_can.SetFillColor(0);
  rogeff_can.Draw();
  for (size_t i=0; i<6; i++) {
    rogeff[i]->efficiency();
    rogeff[i]->SetAxisRange(0, 1.0, "Z");
    rogeff_can.cd(i+1);
    //rogeff[i].SetBorderSize(0);
    rogeff[i]->SetFillColor(0);
    rogeff[i]->Draw("COLZ");
    rogeff[i]->Write();
  }
  rogeff_can.Write();


  if (runeff_can!=NULL && runeff_num!=NULL && runeff_den!=NULL) {
    runeff_can->Draw();
    runeff_can->cd();
    runeff_num->Divide(runeff_den);
    runeff_num->SetAxisRange(0, 1.1, "Y");
    runeff_num->GetXaxis()->SetNoExponent();
    runeff_num->Draw("EP");

    runeff_num->Write();
    runeff_den->Write();
    runeff_can->Write();
  }


//   if (modeff_can!=NULL && modeff_num!=NULL && modeff_den!=NULL) {
//     modeff_can->Draw();
//     modeff_can->cd();
//     modeff_num->Divide(modeff_den);
//     modeff_num->SetAxisRange(0, 1.1, "Z");
//     modeff_num->Draw("COLZ");

//     modeff_num->Write();
//     modeff_den->Write();
//     modeff_can->Write();
//   }


//   if (rogeff_can!=NULL && rogeff_num!=NULL && rogeff_den!=NULL) {
//     rogeff_can->Draw();
//     rogeff_can->cd();
//     rogeff_num->Divide(rogeff_den);
//     rogeff_num->SetAxisRange(0, 1.1, "Z");
//     rogeff_num->Draw("COLZ");

//     rogeff_num->Write();
//     rogeff_den->Write();
//     rogeff_can->Write();
//   }


  deltime.efficiency();
  deltime_can.Draw();
  deltime_can.cd();
  deltime.Draw();
  deltime.Write();
  deltime_can.Write();


  deleff.efficiency();
  deleff_can.Draw();
  deleff_can.cd();
  deleff.Draw();
  deleff.Write();
  deleff_can.Write();


  delnormcluschrg.efficiency();
  delnormcluschrg_can.Draw();
  delnormcluschrg_can.cd();
  delnormcluschrg.Draw();
  delnormcluschrg.Write();
  delnormcluschrg_can.Write();


  delclussize.efficiency();
  delclussize_can.Draw();
  delclussize_can.cd();
  delclussize.Draw();
  delclussize.Write();
  delclussize_can.Write();

  //
  // The end
  //

  for (int i=0; i<modeff.size(); i++) {
    delete modeff[i];
  }

  for (int i=0; i<rogeff.size(); i++) {
    delete rogeff[i];
  }

  return 1;
}


//
// TrackTree based plots
//


////////////////////////////////////////////////////////////////////////////////
//
// ClustTree based plots
//


int doClusterPlots(TChain *clustTree) {

  std::string plotType="Cluster";
  std::ostringstream plotName;
  std::ostringstream canvasName;
  std::ostringstream histName;


  //
  // Set the tree for reading
  //

  EventData evt;
  Cluster clust;
  clustTree->SetBranchAddress("event", &evt);
  clustTree->SetBranchAddress("clust", &clust);
  clustTree->SetBranchAddress("module", &clust.mod);
  clustTree->SetBranchAddress("module_on", &clust.mod_on);

  std::cout<<"Reading clustTree of entries: "<<clustTree->GetEntries()<<std::endl;


  //
  // Preprocessing cycle
  //

  std::vector<int> runs;
  std::map<int, int> delays;
  std::string prevFileName="";

  for (Long64_t i=0; i<clustTree->GetEntries(); i++) {
    clustTree->GetEntry(i);
    std::string fname=clustTree->GetFile()->GetName();
    if (fname!=prevFileName) {
      std::cout<<"Processing "<<fname<<std::endl;
      prevFileName=fname;
    }
    if (find(runs.begin(), runs.end(), evt.run)==runs.end()) {
      runs.push_back(evt.run);
    
      int delay=NOVAL_I;
      if (evt.run<111140) delay=0;
      if (evt.run>112080) {
	delay= (evt.delay%12) ? evt.delay/2-25 : evt.delay/2;
	delays[evt.run]=delay;
      }
    }
  }

  std::sort(runs.begin(), runs.end(), std::less<int>());
  std::cout<<"Processing runs "<<runs.front()<<" to "<<runs.back()<<std::endl;


  //
  // Booking histograms
  //

  // 1. Hit Efficiency vs DT time
//   std::vector<deb::Histogram<TH1F>* > dttimeeff;
//   for (size_t i=0; i<6; i++) {    
//     dttimeeff.push_back(new deb::Histogram<TH1F> ("","Hit efficiency in the Barrel pixel;"\
// 						  "DT time [ns];Efficiency", 35,-35.,35.));
//     dttimeeff[i]->setLabel("Name", "%s_dttimeeff_%d", plotType.data(), i);
//     dttimeeff[i]->efficiency();
//   }

//   TCanvas dttimeeff_can(dttimeeff[0]->getLabel("Name", "$_canvas").data(), 
// 			dttimeeff[0]->getLabel("Title").data(), 550,900);
//   dttimeeff_can.Divide(1,2);


  // 1. Total cluster charge per event vs delay
  deb::Histogram<TH1F> delcluschrg("","Total cluster charge per event (track) in the Barrel pixel;Delay[ns];Charge [ke]",
			      6, -15, 21);
  delcluschrg.setLabel("Name", "%s_delcluschrg", plotType.data());
  delcluschrg.efficiency();

  TCanvas delcluschrg_can(delcluschrg.getLabel("Name", "$_canvas").data(), 
			  delcluschrg.getLabel("Title").data(), 600,600);


  // 2. Total cluster size per event vs delay
  deb::Histogram<TH1F> delclussize("","Total cluster size per event (track) in the Barrel pixel;Delay[ns];Size",
			      6, -15, 21);
  delclussize.setLabel("Name", "%s_delclussize", plotType.data());
  delclussize.efficiency();

  TCanvas delclussize_can(delclussize.getLabel("Name", "$_canvas").data(), 
			  delclussize.getLabel("Title").data(), 600,600);

  // 3. Num of clusters per event vs delay
  deb::Histogram<TH1F> delnumclus("","Number of clusters per event (track) in the Barrel pixel;Delay[ns];Size",
			      6, -15, 21);
  delnumclus.setLabel("Name", "%s_delnumclus", plotType.data());
  delnumclus.efficiency();

  TCanvas delnumclus_can(delnumclus.getLabel("Name", "$_canvas").data(), 
			  delnumclus.getLabel("Title").data(), 600,600);


  // 4. Average cluster size per event vs delay
  deb::Histogram<TH1F> delavgclussize("","Average cluster size in the Barrel pixel (for clusters >12 ke);Delay[ns];<Size>",
			      6, -15, 21);
  delavgclussize.setLabel("Name", "%s_delavgclussize", plotType.data());
  delavgclussize.efficiency();

  TCanvas delavgclussize_can(delavgclussize.getLabel("Name", "$_canvas").data(), 
			  delavgclussize.getLabel("Title").data(), 600,600);


  // 5. Average cluster charge per event vs delay
  deb::Histogram<TH1F> delavgcluschrg("","Average cluster charge in the Barrel pixel (for clusters >12 ke);Delay[ns];<Charge> [ke]",
			      6, -15, 21);
  delavgcluschrg.setLabel("Name", "%s_delavgcluschrg", plotType.data());
  delavgcluschrg.efficiency();

  TCanvas delavgcluschrg_can(delavgcluschrg.getLabel("Name", "$_canvas").data(), 
			     delavgcluschrg.getLabel("Title").data(), 600,600);


  // 6. Cluster multiplicity vs delay
  deb::Histogram<TH2F> numClusVsDelay(6, -15, 21, 10, 0.5, 10.5, "%s_numClusVsDelay;"
	        "Fraction of events (tracks) with N clusters in the Barrel pixel;Delay[ns];Nclusters",
		plotType.data());
  numClusVsDelay.efficiency();
  TCanvas numClusVsDelay_can(numClusVsDelay.getLabel("Name", "$_canvas").data(), 
			     numClusVsDelay.getLabel("Title").data(), 600,600);


  // 7. Cluster size vs delay
  deb::Histogram<TH2F> sizeClusVsDelay(6, -15, 21, 20, 0.5, 20.5, "%s_sizeClusVsDelay;"
	        "Rate of clusters per event vs cluster size and delay in the Barrel pixel;Delay[ns];Size",
		plotType.data());
  sizeClusVsDelay.efficiency();
  TCanvas sizeClusVsDelay_can(sizeClusVsDelay.getLabel("Name", "$_canvas").data(), 
			      sizeClusVsDelay.getLabel("Title").data(), 600,600);


  // 8. Cluster charge vs delay
  deb::Histogram<TH2F> chrgClusVsDelay(6, -15, 21, 45, 0., 45., "%s_chrgClusVsDelay;"
	          "Rate of clusters per event vs cluster charge and delay in the Barrel pixel;"
		  "Delay[ns];Charge[ke]", plotType.data());
  chrgClusVsDelay.efficiency();
  TCanvas chrgClusVsDelay_can(chrgClusVsDelay.getLabel("Name", "$_canvas").data(), 
			      chrgClusVsDelay.getLabel("Title").data(), 600,600);
  

  // 9. Cluster charge
  deb::Plot<deb::Histogram<TH1F> > chrgClus(600, 600, 
					    "%s_chrgClus", plotType.data());
  chrgClus.add(200, 0., 200., 
	       "@6;Cluster charge at delay step @#1;Charge [ke];Nclus");


  // 10. pixel charge in clusters
  deb::Plot<deb::Histogram<TH1F> > chrgPix(600, 600, 
					   "%s_chrgPix", plotType.data());
  chrgPix.add(200, 0., 200., 
	      "@6;Pixel charge at delay step @#1 in Clusters;"
	      "Charge [ke];Nclus");


  //
  // Filling histograms
  //

  int prevevt=NOVAL_I;
  int prevevtNumClus=NOVAL_I;
  int prevevtDelay=NOVAL_I;

  for (long i=0; i<clustTree->GetEntries(); i++) {
    clustTree->GetEntry(i);
    int step=(delays[evt.run]!=NOVAL_I) ? delays[evt.run]/6+2 : NOVAL_I;

    if (evt.evt!=prevevt) {
      delcluschrg.den()->Fill(delays[evt.run]);
      delclussize.den()->Fill(delays[evt.run]);
      delnumclus.den()->Fill(delays[evt.run]);

      chrgClusVsDelay.fillDenX(delays[evt.run]);
      sizeClusVsDelay.fillDenX(delays[evt.run]);
      
      if (prevevtDelay!=NOVAL_I) {
	numClusVsDelay.fillDenX(prevevtDelay);
	numClusVsDelay.num()->Fill(prevevtDelay, prevevtNumClus);
      }
      prevevtDelay=delays[evt.run];
      prevevtNumClus=0;
      prevevt=evt.evt;
    }
    prevevtNumClus++;

    delcluschrg.num()->Fill(delays[evt.run], clust.charge);
    delclussize.num()->Fill(delays[evt.run], clust.size);
    delnumclus.num()->Fill(delays[evt.run]);

    if (clust.mod_on.sec!=4&&clust.mod_on.sec!=5&&clust.charge>12.) {
      delavgclussize.num()->Fill(delays[evt.run], clust.size);
      delavgclussize.den()->Fill(delays[evt.run]);
      if (clust.charge>12.) {
	delavgcluschrg.num()->Fill(delays[evt.run], clust.charge);
	delavgcluschrg.den()->Fill(delays[evt.run]);
      }
    }
    sizeClusVsDelay.num()->Fill(delays[evt.run], clust.size);
    chrgClusVsDelay.num()->Fill(delays[evt.run], clust.charge);

    chrgClus(step).Fill(clust.charge);
    for (int p=0; p<clust.size; p++) {
      chrgPix(step).Fill(clust.adc[p]);
    }
  }


  //
  // Post-processing and drawing histograms
  // 

  delcluschrg.efficiency();
  delcluschrg_can.Draw();
  delcluschrg_can.cd();
  delcluschrg.Draw();
  delcluschrg.Write();
  delcluschrg_can.Write();


  delclussize.efficiency();
  delclussize_can.Draw();
  delclussize_can.cd();
  delclussize.Draw();
  delclussize.Write();
  delclussize_can.Write();


  delnumclus.efficiency();
  delnumclus_can.Draw();
  delnumclus_can.cd();
  delnumclus.Draw();
  delnumclus.Write();
  delnumclus_can.Write();


  delavgclussize.efficiency();
  delavgclussize_can.Draw();
  delavgclussize_can.cd();
  delavgclussize.Draw();
  delavgclussize.Write();
  delavgclussize_can.Write();


  delavgcluschrg.efficiency();
  delavgcluschrg_can.Draw();
  delavgcluschrg_can.cd();
  delavgcluschrg.Draw();
  delavgcluschrg.Write();
  delavgcluschrg_can.Write();

  numClusVsDelay.efficiency();
  numClusVsDelay_can.Draw();
  numClusVsDelay_can.cd();
  numClusVsDelay.Draw("COLZ");
  numClusVsDelay.Write();
  numClusVsDelay_can.Write();

  sizeClusVsDelay.efficiency();
  sizeClusVsDelay_can.Draw();
  sizeClusVsDelay_can.cd();
  sizeClusVsDelay.Draw("COLZ");
  sizeClusVsDelay.Write();
  sizeClusVsDelay_can.Write();

  chrgClusVsDelay.efficiency();
  chrgClusVsDelay_can.Draw();
  chrgClusVsDelay_can.cd();
  chrgClusVsDelay.Draw("COLZ");
  chrgClusVsDelay.Write();
  chrgClusVsDelay_can.Write();
  

  chrgClus.scaleMaximumTo();
  chrgClus.setAxisRange(0., 1.1, "Y");
  chrgClus.Draw("[0],[1],[2], [3], [4], [5]", 2);
  chrgClus.Write();

  //  chrgPix.scaleMaximumTo();
  //  chrgPix.setAxisRange(0., 1.1, "Y");
  chrgPix.Draw("[0],[1],[2], [3], [4], [5]", 2);
  chrgPix.Write();
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

  doRecHitPlots(trajTree);
  //doTrackPlots();
  doClusterPlots(clustTree);

  //outfile.Write();
  outfile.Close();

  delete trajTree;
  delete trackTree;
  delete clustTree;

  return 1;   
}

#endif
