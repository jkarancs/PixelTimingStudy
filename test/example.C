TCanvas* custom_can_(TH1D* h, std::string canname, std::string title,
      	       std::string xtitle, std::string ytitle, int gx = 0, int gy = 0, 
      	       int histosize_x = 500, int histosize_y = 500,
      	       int mar_left = 80, int mar_right = 20, int mar_top = 20, int mar_bottom = 60) {
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

void example() {
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


  //ah.rocmap         ->add(p.det,    1,  4, 4,  3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5,  42, -10.5,  10.5);
  //ah.rocmap         ->add(p.det,    1,  5, 5,  3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5,  66, -16.5,  16.5);
  //ah.rocmap         ->add(p.det,    1,  6, 6,  3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5,  90, -22.5,  22.5);
  //ah.rocmap         ->add(p.fpixIO, 1,  1, 1, -3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5, 144,   0.5,  12.5);
  //ah.rocmap         ->add(p.fpixIO, 1,  2, 2, -3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5, 144, -12.5,  -0.5);
  
  // TCanvas* l1_modmap = new TCanvas("l1_modmap","Module Efficiency Map - Layer 1");
  // TH2D *l1_module_map_num = new TH2D("l1_module_map_num","Module Efficiency Map - Layer 1;Module;Ladder;Hit Efficiency", 9,-4.5,4.5, 21,-10.5,10.5);
  // TH2D *l1_module_map_den = new TH2D("l1_module_map_den","Module Efficiency Map - Layer 1;Module;Ladder;Hit Efficiency", 9,-4.5,4.5, 21,-10.5,10.5);
  // trajTree->Draw("ladder:module>>l1_module_map_num","layer==1&&pass_effcuts&&validhit");
  // trajTree->Draw("ladder:module>>l1_module_map_den","layer==1&&pass_effcuts");
  // l1_module_map_num->Divide(l1_module_map_den);
  // l1_module_map_num->GetZaxis()->SetRangeUser(0.9,1.0);
  // l1_module_map_num->Draw("COLZ");
  
  TH1D *lay_num = new TH1D("lay_num","Efficiency vs Layers/Disks;;Hit Efficiency", 7,0.5,7.5);
  TH1D *lay_den = new TH1D("lay_den","Efficiency vs Layers/Disks;;Hit Efficiency", 7,0.5,7.5);
  custom_can_(lay_num, "lay_eff", "#sqrt{s} = 8 TeV", "", "Efficiency", 0, 0, 490,500, 90);
  trajTree->Draw("(det==0)?layer:disk+5+(disk<0)>>lay_num","pass_effcuts&&validhit");
  trajTree->Draw("(det==0)?layer:disk+5+(disk<0)>>lay_den","pass_effcuts");
  lay_num->Divide(lay_den);
  lay_num->GetYaxis()->SetRangeUser(0.98,1.0);
  lay_num->SetMarkerStyle(8);
  lay_num->SetMarkerSize(1.3);
  lay_num->GetXaxis()->SetBinLabel(1, "Layer 1");
  lay_num->GetXaxis()->SetBinLabel(2, "Layer 2");
  lay_num->GetXaxis()->SetBinLabel(3, "Layer 3");
  lay_num->GetXaxis()->SetBinLabel(4, "Disk -2");
  lay_num->GetXaxis()->SetBinLabel(5, "Disk -1");
  lay_num->GetXaxis()->SetBinLabel(6, "Disk +1");
  lay_num->GetXaxis()->SetBinLabel(7, "Disk +2");
  lay_num->GetXaxis()->LabelsOption("h");
  lay_num->GetXaxis()->SetLabelSize(0.05);
  lay_num->GetYaxis()->SetRangeUser(0.98,1.005);
  gStyle->SetErrorX(0);
  lay_num->Draw("PE1X0");
  lay_num->Draw("P");
  
}
