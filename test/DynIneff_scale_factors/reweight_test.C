TCanvas* custom_can_(TH1D* h, std::string canname, std::string title,
      	       std::string xtitle, std::string ytitle, int gx = 0, int gy = 0, 
      	       int histosize_x = 500, int histosize_y = 500,
      	       int mar_left = 80, int mar_right = 20, int mar_top = 20, int mar_bottom = 60) {
  gStyle->SetPaperSize(20.,20.);
  gStyle->SetTitleFont(42,"xyz");
  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasColor(0);
  gStyle->SetErrorX(0);
  gStyle->SetFrameBorderMode(0);
  gStyle->SetFrameFillColor(0);
  gStyle->SetFrameFillStyle(0);
  gStyle->SetFrameLineWidth(2);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadColor(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetPalette(1);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleFillColor(0);
  gStyle->SetTitleStyle(0);
  gStyle->SetTitleX(1);
  gStyle->SetTitleY(1);
  gStyle->SetTitleAlign(33);

  mar_top = mar_top + (title.size()>0)*20;
  int titlefontsize = 32;
  int labelfontsize = 20;
  int yoffset_x = mar_left - titlefontsize - 4;
  int xoffset_y = mar_bottom - titlefontsize - 4;
  int zoffset_x = mar_right - titlefontsize - 4;
  int padsize_x = histosize_x + mar_left + mar_right;
  int padsize_y = histosize_y + mar_top + mar_bottom;
  int padsize = ((padsize_x<=padsize_y) ? padsize_x : padsize_y);
  float padratio_yx = (float)padsize_y/padsize_x > 1 ? 1 : (float)padsize_y/padsize_x;
  float padratio_xy = (float)padsize_x/padsize_y > 1 ? 1 : (float)padsize_x/padsize_y;
  Float_t xoffset = ((Float_t)xoffset_y/titlefontsize+0.5) * padratio_xy /1.6;
  Float_t yoffset = ((Float_t)yoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t zoffset = ((Float_t)zoffset_x/titlefontsize+0.5) * padratio_yx /1.6;
  Float_t titlesize = (Float_t)titlefontsize/padsize;
  Float_t labelsize = (Float_t)labelfontsize/padsize;
  h->SetTitle(title.c_str());
  h->SetTitleFont(42,"xyz");
  h->SetLabelFont(42,"xyz");
  h->SetTitleSize(titlesize,"xyz");
  h->SetLabelSize(labelsize,"xyz");
  h->GetXaxis()->SetTitle(xtitle.c_str());
  h->GetYaxis()->SetTitle(ytitle.c_str());
  h->GetXaxis()->SetTitleOffset(xoffset);
  h->GetYaxis()->SetTitleOffset(yoffset);
  h->GetZaxis()->SetTitleOffset(zoffset);
  h->GetYaxis()->SetDecimals(1);
  h->GetZaxis()->SetDecimals(1);
  gStyle->SetPadLeftMargin((Float_t)mar_left/padsize_x);
  gStyle->SetPadRightMargin((Float_t)mar_right/padsize_x);
  gStyle->SetPadTopMargin((Float_t)mar_top/padsize_y);
  gStyle->SetPadBottomMargin((Float_t)mar_bottom/padsize_y);
  gStyle->SetOptTitle(1);
  gStyle->SetTitleH(titlefontsize/padsize);
  gStyle->SetTitleFontSize(titlesize);
  TCanvas* canvas = new TCanvas(canname.c_str(), title.c_str(), padsize_x + 4, padsize_y + 26);
  canvas->SetGrid(gx,gy);
  return canvas;
}

void add_legend(TH1D* data, TH1D* mc, TH1D* mc2) {
  TLegend *leg = new TLegend(0.55,0.65,0.75,0.85,"");
  mc2->SetLineColor(2);
  mc->SetLineColor(4);
  leg->AddEntry(data, "Data", "P");
  leg->AddEntry(mc2,   "MC", "L");
  leg->AddEntry(mc, "MC + Dynamic Ineff", "L");
  leg->SetFillColor(0);
  leg->SetFillStyle(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.04);
  leg->Draw("SAME");
  gPad->Update();
}

void reweight_test() {
  //const char* ntuple_data = "/data/jkarancs/gridout/v3735/MinimumBias__Run2012C-22Jan2013-v1__RECO/*.root";
  //const char* ntuple_mc = "/data/jkarancs/gridout/v3735/p1/*.root";
  //const char* pileuphisto = "/home/common/CMSSW/TimingStudy/CMSSW_5_3_15/src/DPGAnalysis/PixelTimingStudy/PileupHistogram_201278.root";
  const char* ntuple_data = "/data/gridout/TimingStudy/v3735_INC_SPL1_ns11_5314_MB_2012C_22JanRR_RECO_201278/*.root";
  const char* ntuple_mc = "/data/gridout/TimingStudy/v3735_INC_SPL1_ns11_710pre1_MB_GENSIM_Flat_lad_mod_instlumi/*.root";
  const char* ntuple_mc2 = "/data/gridout/TimingStudy/v3735_INC_SPL1_ns11_5314_MB_Summer12_GENSIM_201278_p1/*.root";
  const char* pileuphisto = "/data/jkarancs/CMSSW/TimingStudy/instlumi/CMSSW_7_1_0_pre1/src/DPGAnalysis/PixelTimingStudy/PileupHistogram_201278_flatpileupMC.root";
  
  TChain *evt_data = new TChain("evt_data");
  TChain *evt_mc = new TChain("evt_mc");
  TChain *evt_mc2 = new TChain("evt_mc2");
  evt_data->Add((std::string(ntuple_data)+"/eventTree").c_str());
  evt_mc->Add((std::string(ntuple_mc)+"/eventTree").c_str());
  evt_mc2->Add((std::string(ntuple_mc2)+"/eventTree").c_str());
  TChain *traj_data = new TChain("traj_data");
  TChain *traj_mc = new TChain("traj_mc");
  TChain *traj_mc2 = new TChain("traj_mc2");
  traj_data->Add((std::string(ntuple_data)+"/trajTree").c_str());
  traj_mc->Add((std::string(ntuple_mc)+"/trajTree").c_str());  
  traj_mc2->Add((std::string(ntuple_mc2)+"/trajTree").c_str());  

  const char* data_cuts = "(trig&1)*(nvtx>0)&&(run==201278)&&!(ls<62||(ls>163&&ls<166)||(ls>229&&ls<232)||(ls>256&&ls<259)||(ls>316&&ls<318)||(ls>595&&ls<598)||(ls>938&&ls<942)||(ls>974&&ls<976)||(ls>1160&&ls<1163)||(ls>1304&&ls<1306)||(ls>1793&&ls<1796)||(ls>1802&&ls<1805)||(ls>1906&&ls<1909)||(ls>1929&&ls<1932)||ls>2174)";
  
  // MC normaliziation factor
  TCanvas *c1 = new TCanvas("c1","c1",600,600);
  TH1D *nevt_data = new TH1D("nevt_data",";pileup",40,0,40);
  TH1D *nevt_mc   = new TH1D("nevt_mc",  ";pileup",40,0,40);
  TH1D *nevt_mc2   = new TH1D("nevt_mc2",  ";pileup",40,0,40);
  evt_data->Draw("nvtx>>nevt_data",data_cuts);
  evt_mc  ->Draw("nvtx>>nevt_mc",  "(trig&1)*(nvtx>0)");
  evt_mc2  ->Draw("nvtx>>nevt_mc2",  "(trig&1)*(nvtx>0)");
  int n_mc   = nevt_mc  ->Integral();
  int n_mc2   = nevt_mc2  ->Integral();
  int n_data = nevt_data->Integral();
  int norm_factor = n_data / n_mc;
  int norm_factor2 = n_data / n_mc2;
  c1->Close();
  // Pileup
  TH1D *pileup_mc = new TH1D("pileup_mc",";pileup",50,0,50);
  TH1D *pileup_mc2 = new TH1D("pileup_mc2",";pileup",50,0,50);
  TCanvas *c1=custom_can_(pileup_mc, "pileup", "", "pile-up", "");
  evt_mc->Draw("pileup>>pileup_mc","(trig&1)*(nvtx>0)*weight");
  evt_mc2->Draw("pileup+1>>pileup_mc2","(trig&1)*(nvtx>0)*weight","SAME");
  gPad->Update();
  TFile *f = new TFile(pileuphisto);
  TH1D *hdata = (TH1D*)f->Get("pileup");
  hdata->SetDirectory(0);
  hdata->SetMarkerStyle(20);
  hdata->Draw("PSAME");
  pileup_mc->Scale(hdata->Integral()/n_mc);
  pileup_mc2->Scale(hdata->Integral()/n_mc2);
  add_legend(hdata, pileup_mc, pileup_mc2);
  // Nvertex
  TH1D *nvtx_data = new TH1D("nvtx_data",";N_{Vertex}",40,0,40);
  TH1D *nvtx_mc   = new TH1D("nvtx_mc",  ";N_{Vertex}",40,0,40);
  TH1D *nvtx_mc2  = new TH1D("nvtx_mc2", ";N_{Vertex}",40,0,40);
  //evt_data->Draw("nvtx>>nvtx_data","(nvtx>0)*weight","P");
  //evt_mc  ->Draw("nvtx>>nvtx_mc",  "(nvtx>0)*weight","SAME");
  //nvtx_mc->Scale(nvtx_data->Integral()/nvtx_mc->Integral());
  TCanvas *c2=custom_can_(nvtx_data, "nvtx", "", "N_{Vertex}", "");
  evt_data->Draw("nvtx>>nvtx_data",,"P");
  evt_mc  ->Draw("nvtx>>nvtx_mc",  "(trig&1)*(nvtx>0)*weight","SAME");
  evt_mc2 ->Draw("nvtx>>nvtx_mc2",  "(trig&1)*(nvtx>0)*weight","SAME");
  //nvtx_mc->Scale(norm_factor);
  //nvtx_mc2->Scale(norm_factor2);
  nvtx_mc->Scale(nvtx_data->Integral()/nvtx_mc->Integral());
  nvtx_mc2->Scale(nvtx_data->Integral()/nvtx_mc2->Integral());
  nvtx_data->SetMarkerStyle(20);
  add_legend(nvtx_data, nvtx_mc, nvtx_mc2);
  // Number of clusters
  TH1D *nclu_data = new TH1D("nclu_data",";N_{Cluster}",40,0,4000);
  TH1D *nclu_mc   = new TH1D("nclu_mc",  ";N_{Cluster}",40,0,4000);
  TH1D *nclu_mc2  = new TH1D("nclu_mc2", ";N_{Cluster}",40,0,4000);
  TCanvas *c3=custom_can_(nclu_data, "nclu", "", "N_{Clusters}", "");
  evt_data->Draw("nclu[]>>nclu_data",data_cuts,"P");
  evt_mc  ->Draw("nclu[]>>nclu_mc",  "(trig&1)*(nvtx>0)*weight","SAME");
  evt_mc2 ->Draw("nclu[]>>nclu_mc2", "(trig&1)*(nvtx>0)*weight","SAME");
  //nclu_mc->Scale(norm_factor);
  //nclu_mc2->Scale(norm_factor2);
  nclu_mc->Scale(nclu_data->Integral()/nclu_mc->Integral());
  nclu_mc2->Scale(nclu_data->Integral()/nclu_mc2->Integral());
  nclu_data->SetMarkerStyle(20);
  add_legend(nclu_data, nclu_mc, nclu_mc2);

  c1->SaveAs("ResultPlots/pileup.eps");
  c2->SaveAs("ResultPlots/nvtx.eps");
  c3->SaveAs("ResultPlots/nclu.eps");
  
  // Normalized Cluster charge
  TH1D *normch_data = new TH1D("normch_data",";Norm. Cluster Charge (ke)",50,0,50);
  TH1D *normch_mc   = new TH1D("normch_mc",  ";Norm. Cluster Charge (ke)",50,0,50);
  TH1D *normch_mc2  = new TH1D("normch_mc2", ";Norm. Cluster Charge (ke)",50,0,50);
  TCanvas *c4=custom_can_(normch_mc, "normc", "", "Norm. Cluster Charge (ke)", "");
  traj_data->Draw("norm_charge>>normch_data",(std::string(data_cuts)+"&&(size>0)").c_str(),"P");
  traj_mc  ->Draw("norm_charge>>normch_mc",  "(trig&1)*(nvtx>0)*(size>0)*weight","SAME");
  traj_mc2 ->Draw("norm_charge>>normch_mc2", "(trig&1)*(nvtx>0)*(size>0)*weight","SAME"); gPad->Update();
  normch_mc->Scale(norm_factor);
  normch_mc2->Scale(norm_factor2);
  normch_data->SetMarkerStyle(20);
  add_legend(normch_data, normch_mc, normch_mc2);
  
  c4->SaveAs("ResultPlots/normc.eps");
}
