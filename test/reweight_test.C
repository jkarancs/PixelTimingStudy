oid reweight_test() {
  const char* ntuple_data = "Data.root";
  const char* ntuple_mc = "MC.root";
  const char* pileuphisto = "data/PileupHistogram_test.root";

  gROOT->SetStyle("Plain");
  TFile *f = new TFile(ntuple_mc);
  TCanvas *c = new TCanvas("c","c",800,400);
  c->Divide(2); c->cd(1);
  TTree *eventTree = (TTree*)f->Get("eventTree");
  eventTree->Draw("pileup>>pileup_mc(50,0,50)","weight");
  c->cd(2);
  eventTree->Draw("nvtx>>nvtx_mc(40,0,40)","(nvtx>0)*weight");
  int nevt = eventTree->GetEntries();
  f = new TFile(pileuphisto);
  TH1D *data = (TH1D*)f->Get("pileup");
  data->SetDirectory(0);
  data->SetMarkerStyle(20);
  data->Scale(nevt/data->Integral());
  c->cd(1);
  data->Draw("PSAME");
  f = new TFile(ntuple_data);
  eventTree = (TTree*)f->Get("eventTree");
  c->cd(2);
  eventTree->Draw("nvtx>>nvtx_data(50,0,50)","nvtx>0","PSAME");
  nvtx_data->SetMarkerStyle(20);
  nvtx_mc->Scale(nvtx_data->Integral()/nevt);
}
