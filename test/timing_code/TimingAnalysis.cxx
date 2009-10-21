
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

#include "DataStructures.h"


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

  std::vector<Double_t> runs;

  for (Long64_t i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);
    if (find(runs.begin(), runs.end(), evt.run)==runs.end()) {
      runs.push_back(evt.run);
    }
  }

  std::sort(runs.begin(), runs.end(), std::less<Double_t>());
  std::cout<<"Processing runs "<<runs.front()<<" to "<<runs.back()<<std::endl;


  //
  // Booking histograms
  //

  // 1. Efficiency vs DT time
  plotName.str("");
  plotName << plotType << "_dttimeeff";
  canvasName.str("");
  canvasName << plotName.str() << "_can";
  TCanvas* dttimeeff_can = new TCanvas(canvasName.str().data(), 
				       canvasName.str().data(), 600,600);
  TH1F *dttimeeff_num=NULL, *dttimeeff_den=NULL;

  for (int i=0; i<2; i++) {
    histName.str("");
    histName << plotName.str() << ((i==0) ? "_num" : "_den");
    TH1F *h = new TH1F(histName.str().data(), histName.str().data(), 
		       40, -40., 40.);
    h->Sumw2();
    if (i==0) dttimeeff_num=h; else dttimeeff_den=h;
  }

  // 2. Efficiency vs run
  plotName.str("");
  plotName << plotType << "_runeff";
  canvasName.str("");
  canvasName << plotName.str() << "_can";
  TCanvas* runeff_can = new TCanvas(canvasName.str().data(), 
				    canvasName.str().data(), 600,600);
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

  // 3. Module Efficiency
  plotName.str("");
  plotName << plotType << "_modeff";
  canvasName.str("");
  canvasName << plotName.str() << "_can";
  TCanvas* modeff_can = new TCanvas(canvasName.str().data(), 
				    canvasName.str().data(), 600,600);
  TH2D *modeff_num=NULL, *modeff_den=NULL;

  for (int i=0; i<2; i++) {
    histName.str("");
    histName << plotName.str() << ((i==0) ? "_num" : "_den");
    TH2D *h = new TH2D(histName.str().data(), histName.str().data(), 
		       44, 0.5, 44.5, 30, 0.5, 30.5);
    h->Sumw2();
    if (i==0) modeff_num=h; else modeff_den=h;
  }


  // 4. ROG Efficiency
  plotName.str("");
  plotName << plotType << "_rogeff";
  canvasName.str("");
  canvasName << plotName.str() << "_can";
  TCanvas* rogeff_can = new TCanvas(canvasName.str().data(), 
				    canvasName.str().data(), 600,600);
  TH2D *rogeff_num=NULL, *rogeff_den=NULL;

  for (int i=0; i<2; i++) {
    histName.str("");
    histName << plotName.str() << ((i==0) ? "_num" : "_den");
    TH2D *h = new TH2D(histName.str().data(), histName.str().data(), 
		       8, 0.5, 8.5, 12, -0.5, 11.5);
    h->Sumw2();
    if (i==0) rogeff_num=h; else rogeff_den=h;
  }


  //
  // Filling histograms
  //

  for (long i=0; i<trajTree->GetEntries(); i++) {
    trajTree->GetEntry(i);
    
    if (trajmeas.telescope && trajmeas.mod_on.det==0) {
      dttimeeff_den->Fill(evt.tmuon);
      if (trajmeas.validhit) dttimeeff_num->Fill(evt.tmuon);

      runeff_den->Fill(evt.run);
      if (trajmeas.validhit) runeff_num->Fill(evt.run);

      int modeff_y=trajmeas.mod.module+(trajmeas.mod.layer-1)*10;
      modeff_den->Fill(trajmeas.mod.ladder, modeff_y);
      if (trajmeas.validhit) {
	modeff_num->Fill(trajmeas.mod.ladder, modeff_y);
      }

      int rogeff_y=trajmeas.mod_on.shl*3+trajmeas.mod_on.prt-1;
      rogeff_den->Fill(trajmeas.mod_on.sec, rogeff_y);
      if (trajmeas.validhit) {
	rogeff_num->Fill(trajmeas.mod_on.sec, rogeff_y);
      }
    }
  }



  //
  // Post-processing and drawing histograms
  // 

  if (dttimeeff_can!=NULL && dttimeeff_num!=NULL && dttimeeff_den!=NULL) {
    dttimeeff_can->Draw();
    dttimeeff_can->cd();
    dttimeeff_num->Divide(dttimeeff_den);
    dttimeeff_num->SetLineColor(2);
    dttimeeff_num->SetAxisRange(0, 1.1, "Y");
    if (dttimeeff_den->Integral()!=0) {
      dttimeeff_den->Scale(1./dttimeeff_den->GetMaximum());
    }
    dttimeeff_num->Draw("EP");
    dttimeeff_den->Draw("SAMEEP");

    dttimeeff_num->Write();
    dttimeeff_den->Write();
    dttimeeff_can->Write();
  }


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


  if (modeff_can!=NULL && modeff_num!=NULL && modeff_den!=NULL) {
    modeff_can->Draw();
    modeff_can->cd();
    modeff_num->Divide(modeff_den);
    modeff_num->SetAxisRange(0, 1.1, "Z");
    modeff_num->Draw("COLZ");

    modeff_num->Write();
    modeff_den->Write();
    modeff_can->Write();
  }


  if (rogeff_can!=NULL && rogeff_num!=NULL && rogeff_den!=NULL) {
    rogeff_can->Draw();
    rogeff_can->cd();
    rogeff_num->Divide(rogeff_den);
    rogeff_num->SetAxisRange(0, 1.1, "Z");
    rogeff_num->Draw("COLZ");

    rogeff_num->Write();
    rogeff_den->Write();
    rogeff_can->Write();
  }


  //
  // The end
  //

  return 1;
}


//
// TrackTree based plots
//



//
// ClustTree based plots
//



//______________________________________________________________________________
//
//

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
  //doClusterPlots();

  //outfile.Write();
  outfile.Close();

  delete trajTree;
  delete trackTree;
  delete clustTree;

  return 1;   
}

#endif
