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

TH1D* getplot(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH1D *h= (TH1D*)f.FindObjectAny(hname.Data());
  TH1D *c=h->Clone();
  c->SetDirectory(0);
  return c;
}

void rebin_eff(TH1D* eff, TH1D* dist, int n=2) {
  eff->Multiply(dist);
  dist->Add(eff,-1);
  eff->Rebin(n);
  dist->Rebin(n);
  dist->Add(eff);
  eff->Divide(dist);
  error(eff,dist);
}

void error(TH1D* eff, TH1D* dist) {
  double z = 1; // 1 Sigma confidence
  for (int i=1; i<=eff->GetNbinsX(); i++) if (dist->GetBinContent(i)!=0) {
    double p = eff->GetBinContent(i);
    double n = dist->GetBinContent(i);
    double cen = (p+(z*z/(2*n))) / (1.0 + (z*z/n));
    double halfwidth = z*sqrt( p*(1.0-p)/n + (z*z/(4*n*n)) ) / (1.0 + (z*z/n));
    double err = halfwidth + fabs(cen-p); // Assymmetric error -> Choose larger for a conservative error estimate
    eff->SetBinError(i,err);
  }
}


void result(int layer = 1, const char* input2 = "PHM_out/lad_mod_instlumi.root", const char* input1 = "PHM_out/data_201278_Jan22RR.root") {
  bool bothMC = strcmp(input1,"PHM_out/data_201278_Jan22RR.root");

  TH1D* h1_mod[3];
  if (bothMC) {
    h1_mod[0] = getplot(input1,"dynamic_ineff_module_l1_MC_eff");
    h1_mod[1] = getplot(input1,"dynamic_ineff_module_l2_MC_eff");
    h1_mod[2] = getplot(input1,"dynamic_ineff_module_l3_MC_eff");
  } else {
    h1_mod[0] = getplot(input1,"dynamic_ineff_module_l1_Data_eff");
    h1_mod[1] = getplot(input1,"dynamic_ineff_module_l2_Data_eff");
    h1_mod[2] = getplot(input1,"dynamic_ineff_module_l3_Data_eff");
  }
  TH1D* h2_mod[3];
  h2_mod[0] = getplot(input2,"dynamic_ineff_module_l1_MC_eff");
  h2_mod[1] = getplot(input2,"dynamic_ineff_module_l2_MC_eff");
  h2_mod[2] = getplot(input2,"dynamic_ineff_module_l3_MC_eff");
  TCanvas* c1=custom_can_(h1_mod[0], "mod_eff", "", "Modules", "Hit Efficiency");
  h1_mod[layer-1]->SetMarkerStyle(20);
  h1_mod[layer-1]->GetYaxis()->SetRangeUser(0.98,1.005);
  h1_mod[layer-1]->Draw("PE1");
  h2_mod[layer-1]->SetMarkerStyle(21);
  h2_mod[layer-1]->SetMarkerColor(2);
  h2_mod[layer-1]->Draw("SAMEPE1");
  
  TH1D* h1_lad[3];
  if (bothMC) {
    h1_lad[0] = getplot(input1,"dynamic_ineff_ladder_l1_MC_eff");
    h1_lad[1] = getplot(input1,"dynamic_ineff_ladder_l2_MC_eff");
    h1_lad[2] = getplot(input1,"dynamic_ineff_ladder_l3_MC_eff");
  } else {
    h1_lad[0] = getplot(input1,"dynamic_ineff_ladder_l1_Data_eff");
    h1_lad[1] = getplot(input1,"dynamic_ineff_ladder_l2_Data_eff");
    h1_lad[2] = getplot(input1,"dynamic_ineff_ladder_l3_Data_eff");
  }
  TH1D* h2_lad[3];
  h2_lad[0] = getplot(input2,"dynamic_ineff_ladder_l1_MC_eff");
  h2_lad[1] = getplot(input2,"dynamic_ineff_ladder_l2_MC_eff");
  h2_lad[2] = getplot(input2,"dynamic_ineff_ladder_l3_MC_eff");
  TCanvas* c2=custom_can_(h1_lad[0], "lad_eff", "", "Ladders", "Hit Efficiency");
  h1_lad[layer-1]->SetMarkerStyle(20);
  int nlad = (layer==1) ? 10 : ((layer==2) ? 16 : 22);
  h1_lad[layer-1]->GetXaxis()->SetRange(23-nlad,23+nlad);
  h1_lad[layer-1]->GetYaxis()->SetRangeUser(0.98,1.005);
  h1_lad[layer-1]->Draw("PE1");
  h2_lad[layer-1]->SetMarkerStyle(21);
  h2_lad[layer-1]->SetMarkerColor(2);
  h2_lad[layer-1]->Draw("SAMEPE1");

  TH1D* h1_inst[3];
  if (bothMC) {
    h1_inst[0] = getplot(input1,"dynamic_ineff_instlumi_l1_MC_eff");
    h1_inst[1] = getplot(input1,"dynamic_ineff_instlumi_l2_MC_eff");
    h1_inst[2] = getplot(input1,"dynamic_ineff_instlumi_l3_MC_eff");
  } else {
    h1_inst[0] = getplot(input1,"dynamic_ineff_instlumi_l1_Data_eff");
    h1_inst[1] = getplot(input1,"dynamic_ineff_instlumi_l2_Data_eff");
    h1_inst[2] = getplot(input1,"dynamic_ineff_instlumi_l3_Data_eff");
  }
  TH1D* h2_inst[3];
  h2_inst[0] = getplot(input2,"dynamic_ineff_instlumi_l1_MC_eff");
  h2_inst[1] = getplot(input2,"dynamic_ineff_instlumi_l2_MC_eff");
  h2_inst[2] = getplot(input2,"dynamic_ineff_instlumi_l3_MC_eff");
  TH1D* h1_inst_den[3];
  if (bothMC) {
    h1_inst_den[0] = getplot(input1,"dynamic_ineff_instlumi_l1_MC_dist");
    h1_inst_den[1] = getplot(input1,"dynamic_ineff_instlumi_l2_MC_dist");
    h1_inst_den[2] = getplot(input1,"dynamic_ineff_instlumi_l3_MC_dist");
  } else {
    h1_inst_den[0] = getplot(input1,"dynamic_ineff_instlumi_l1_Data_dist");
    h1_inst_den[1] = getplot(input1,"dynamic_ineff_instlumi_l2_Data_dist");
    h1_inst_den[2] = getplot(input1,"dynamic_ineff_instlumi_l3_Data_dist");
  }
  TH1D* h2_inst_den[3];
  h2_inst_den[0] = getplot(input2,"dynamic_ineff_instlumi_l1_MC_dist");
  h2_inst_den[1] = getplot(input2,"dynamic_ineff_instlumi_l2_MC_dist");
  h2_inst_den[2] = getplot(input2,"dynamic_ineff_instlumi_l3_MC_dist");
  
  int n_rebin = 4;
  rebin_eff(h1_inst[layer-1],h1_inst_den[layer-1], n_rebin);
  rebin_eff(h2_inst[layer-1],h2_inst_den[layer-1], n_rebin);

  TCanvas* c3=custom_can_(h1_inst[layer-1], "inst_eff", "", "Instantaneos Luminosty", "Hit Efficiency");
  h1_inst[layer-1]->SetMarkerStyle(20);
  h1_inst[layer-1]->GetYaxis()->SetRangeUser(0.97,1.005);
  h1_inst[layer-1]->Draw("PE1");
  h2_inst[layer-1]->SetMarkerStyle(21);
  h2_inst[layer-1]->SetMarkerColor(2);
  h2_inst[layer-1]->Draw("SAMEPE1");
  
  //c1->SaveAs("ResultPlots/module_l3.eps");
  //c2->SaveAs("ResultPlots/ladder_l3.eps");
  //c3->SaveAs("ResultPlots/instlumi_l3.eps");
  
}
