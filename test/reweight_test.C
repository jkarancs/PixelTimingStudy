void reweight_test() {
  const char* ntuple_data = "/data/jkarancs/gridout/v3735/MinimumBias__Run2012C-22Jan2013-v1__RECO/*.root";
  const char* ntuple_mc = "/data/jkarancs/gridout/v3735/p1/*.root";
  const char* pileuphisto = "/home/common/CMSSW/TimingStudy/CMSSW_5_3_15/src/DPGAnalysis/PixelTimingStudy/PileupHistogram_201278.root";

  gROOT->SetStyle("Plain");
  TChain *evt_data = new TChain("evt_data");
  TChain *evt_mc = new TChain("evt_mc");
  evt_data->Add((std::string(ntuple_data)+"/eventTree").c_str());
  evt_mc->Add((std::string(ntuple_mc)+"/eventTree").c_str());
  TChain *traj_data = new TChain("traj_data");
  TChain *traj_mc = new TChain("traj_mc");
  traj_data->Add((std::string(ntuple_data)+"/trajTree").c_str());
  traj_mc->Add((std::string(ntuple_mc)+"/trajTree").c_str());  

  TCanvas *c = new TCanvas("c","c",800,800);
  c->Divide(2,2);
  c->cd(1);
  // MC normaliziation factor
  TH1D *nevt_data = new TH1D("nevt_data",";pileup",40,0,40);
  TH1D *nevt_mc   = new TH1D("nevt_mc",  ";pileup",40,0,40);
  evt_data->Draw("nvtx>>nevt_data","(trig&1)*(nvtx>0)");
  evt_mc  ->Draw("nvtx>>nevt_mc",  "(trig&1)*(nvtx>0)");
  int n_mc   = nevt_mc  ->Integral();
  int n_data = nevt_data->Integral();
  int norm_factor = n_data / n_mc;
  // Pileup
  TH1D *pileup_mc = new TH1D("pileup_mc",";pileup",50,0,50);
  evt_mc->Draw("pileup>>pileup_mc","(trig&1)*(nvtx>0)*weight");
  TFile *f = new TFile(pileuphisto);
  TH1D *hdata = (TH1D*)f->Get("pileup");
  hdata->SetDirectory(0);
  hdata->SetMarkerStyle(20);
  hdata->Draw("PSAME");
  pileup_mc->Scale(hdata->Integral()/n_mc);
  gPad->Update();
  c->cd(2); // Nvertex
  TH1D *nvtx_data = new TH1D("nvtx_data",";N_{Vertex}",40,0,40);
  TH1D *nvtx_mc   = new TH1D("nvtx_mc",  ";N_{Vertex}",40,0,40);
  //evt_data->Draw("nvtx>>nvtx_data","(nvtx>0)*weight","P");
  //evt_mc  ->Draw("nvtx>>nvtx_mc",  "(nvtx>0)*weight","SAME");
  //nvtx_mc->Scale(nvtx_data->Integral()/nvtx_mc->Integral());
  evt_data->Draw("nvtx>>nvtx_data","(trig&1)*(nvtx>0)*weight","P");
  evt_mc  ->Draw("nvtx>>nvtx_mc",  "(trig&1)*(nvtx>0)*weight","SAME");
  nvtx_mc->Scale(norm_factor);
  nvtx_data->SetMarkerStyle(20);
  gPad->Update();
  c->cd(3); // Number of clusters
  TH1D *nclu_data = new TH1D("nclu_data",";N_{Cluster}",40,0,4000);
  TH1D *nclu_mc   = new TH1D("nclu_mc",  ";N_{Cluster}",40,0,4000);
  evt_mc  ->Draw("nclu[]>>nclu_mc",  "(trig&1)*(nvtx>0)*weight");
  evt_data->Draw("nclu[]>>nclu_data","(trig&1)*(nvtx>0)*weight","PSAME");
  nclu_mc->Scale(norm_factor);
  nclu_data->SetMarkerStyle(20);
  gPad->Update();
  c->cd(4); // Normalized Cluster charge
  TH1D *normch_data = new TH1D("normch_data",";Norm. Cluster Charge (ke)",50,0,50);
  TH1D *normch_mc   = new TH1D("normch_mc",  ";Norm. Cluster Charge (ke)",50,0,50);
  traj_mc  ->Draw("norm_charge>>normch_mc",  "(trig&1)*(nvtx>0)*(size>0)*weight"); gPad->Update();
  traj_data->Draw("norm_charge>>normch_data","(trig&1)*(nvtx>0)*(size>0)*weight","PSAME");
  normch_mc->Scale(norm_factor);
  normch_data->SetMarkerStyle(20);

}
