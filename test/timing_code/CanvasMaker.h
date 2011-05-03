#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TVirtualPad.h"

class CanvasMaker {
  
 public: 
   
  CanvasMaker(const char* fileread, const char* filewrite) {
    fileread_ = fileread;
    filewrite_ = filewrite;
/*     create_(); */
  }
  ~CanvasMaker() { }

 private:

  const char* fileread_;
  const char* filewrite_;
  std::vector<std::vector<std::vector<const char*> > > histoname_;
  std::vector<std::vector<Histograms*> > histos_;

  void setstyle_(TH2D *h) {
    h->GetXaxis()->SetLabelFont(42);
    h->GetXaxis()->SetTitleFont(42);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetLabelFont(42);
    h->GetYaxis()->SetTitleFont(42);
    h->GetYaxis()->SetTitleSize(0.05);
    h->GetZaxis()->SetLabelFont(42);
    h->GetZaxis()->SetTitleFont(42);
    h->GetZaxis()->SetTitleSize(0.05);
  }

  void set_det_cumeff_(std::vector<TH1D*> h, const char* xtitle) {
    for (int i=0; i<40; i++) {
      setstyle_(h[i]);
      h[i]->GetXaxis()->SetTitle(xtitle);
      h[i]->GetYaxis()->SetTitleOffset(1.2);
    }
    for (int i=0; i<10; i++) {
      h[0+4*i]->GetZaxis()->SetTitle("Efficiency");
      h[1+4*i]->GetZaxis()->SetTitle("Distribution");
      h[2+4*i]->GetYaxis()->SetTitle("Cumulative Efficiency");
      h[3+4*i]->GetYaxis()->SetTitle("Cumulative Distribution");
    }
  }

  TCanvas* det_can_(std::vector<TH1D*> h, int i, const char* name, const char* title) {
    std::vector<std::string> postfix_det;
    postfix_det.push_back("");
    postfix_det.push_back("_bpix");
    postfix_det.push_back("_fpix");
    postfix_det.push_back("_l1");
    postfix_det.push_back("_l2");
    postfix_det.push_back("_l3");
    postfix_det.push_back("_dm2");
    postfix_det.push_back("_dm1");
    postfix_det.push_back("_dp1");
    postfix_det.push_back("_dp2");
    std::vector<std::string> detname;
    detname.push_back(" - Pixel Detector");
    detname.push_back(" - Barrel Detector");
    detname.push_back(" - Forward Detector");
    detname.push_back(" - Layer 1");
    detname.push_back(" - Layer 2");
    detname.push_back(" - Layer 3");
    detname.push_back(" - Disk -2");
    detname.push_back(" - Disk -1");
    detname.push_back(" - Disk +1");
    detname.push_back(" - Disk +2");
    std::string canvasname = name + postfix_det[i] + "_can";
    std::string canvastitle = title + detname[i];
    TCanvas* canvas = new TCanvas(canvasname.c_str(), canvastitle.c_str(), 1200, 800);
    canvas->SetFillColor(0);
    canvas->Divide(2,2);
    canvas->cd(1);
    h[4*i+1]->Draw();
    canvas->cd(2);
    h[4*i]->Draw();
    canvas->cd(3);
    h[4*i+3]->Draw();
    canvas->cd(4);
    h[4*i+2]->Draw();
    return canvas;
  }

  void setstyle_(TH1D *h) {
    h->GetXaxis()->SetLabelFont(42);
    h->GetXaxis()->SetTitleFont(42);
    h->GetXaxis()->SetTitleSize(0.05);
    h->GetYaxis()->SetLabelFont(42);
    h->GetYaxis()->SetTitleFont(42);
    h->GetYaxis()->SetTitleSize(0.05);
  }

  void delay_eff_onclusize_merge_(TH1D *eff, TH1D *onclusize, int clu_localmax, float min, float max, float relshift, float rescaleeff) {
    onclusize->SetMarkerStyle(8);
    onclusize->SetMarkerSize(1.3);
    onclusize->GetXaxis()->SetTitle("Delay [ns]");
    onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    onclusize->GetYaxis()->SetTitleOffset(1.2);
    Float_t rangemin = -7.5;
    Float_t rangemax = 26.5;
    onclusize->GetXaxis()->SetRangeUser(rangemin,rangemax);
    Float_t leftmax = onclusize->GetMaximum();
    if (clu_localmax!=-1) leftmax = onclusize->GetBinContent(clu_localmax);
    Float_t rightmax = eff->GetMaximum();
    onclusize->GetYaxis()->SetRangeUser(min,max);
    onclusize->Draw("PX0");
    eff->SetMarkerStyle(8);
    eff->SetMarkerSize(1.3);
    eff->SetMarkerColor(kRed);
    Float_t scale = relshift*leftmax/rightmax;
    eff->Scale(scale);
    Float_t submax = eff->GetMaximum();
    for (Int_t i=1;i<eff->GetNbinsX();i++) {
      Float_t bin = eff->GetBinContent(i);
      Float_t rescaled = rescaleeff*bin-(rescaleeff-1)*submax;
      if (bin!=0.0) eff->SetBinContent(i,rescaled);
    }
    eff->Draw("SAMEPX0");
    Float_t unscaled_right_min = min/scale;
    Float_t unscaled_right_max = max/scale;
    Float_t rescaled_right_max = rightmax + (unscaled_right_max - rightmax) / rescaleeff;
    Float_t rescaled_right_min = rightmax - (rightmax - unscaled_right_min) / rescaleeff;
    TGaxis *axis = new TGaxis(rangemax,min,rangemax,max,rescaled_right_min,rescaled_right_max,510,"+L");
    axis->SetLineColor(2);
    axis->SetLabelColor(2);
    axis->SetTitle("Efficiency");
    axis->SetTitleColor(2);
    axis->SetLabelFont(42);
    axis->SetTitleFont(42);
    axis->SetTitleSize(0.05);
    axis->SetTitleOffset(1.2);
    axis->Draw();

  }

 public:
  
  void gethistodata(std::vector<Histograms*> histos) {
    histos_.push_back(histos);
  }

  void gethistonames_() {
    for (size_t h = 0; h<histos_.size(); h++) {
      histoname_.push_back(std::vector<std::vector<const char*> >());
      for (size_t i =0; i<histos_[h].size(); i++) {
	histoname_[h].push_back(std::vector<const char*>());
	int npf = histos_[h][i]->npostfix();
	if (npf==1) {
	  for (size_t j = 0; j<histos_[h][i]->h1d_1p().size(); j++) histoname_[h][i].push_back(histos_[h][i]->h1d_1p()[j]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h2d_1p().size(); j++) histoname_[h][i].push_back(histos_[h][i]->h2d_1p()[j]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h3d_1p().size(); j++) histoname_[h][i].push_back(histos_[h][i]->h3d_1p()[j]->GetName());
	} else if (npf==2) {
	  for (size_t j = 0; j<histos_[h][i]->h1d_2p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h1d_2p()[j].size(); k++) 
	    histoname_[h][i].push_back(histos_[h][i]->h1d_2p()[j][k]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h2d_2p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h2d_2p()[j].size(); k++) 
	    histoname_[h][i].push_back(histos_[h][i]->h2d_2p()[j][k]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h3d_2p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h3d_2p()[j].size(); k++) 
	    histoname_[h][i].push_back(histos_[h][i]->h3d_2p()[j][k]->GetName());
	} else if (npf==3) {
	  for (size_t j = 0; j<histos_[h][i]->h1d_3p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h1d_3p()[j].size(); k++) 
	    for (size_t l = 0; l<histos_[h][i]->h1d_3p()[j][k].size(); l++) histoname_[h][i].push_back(histos_[h][i]->h1d_3p()[j][k][l]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h2d_3p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h2d_3p()[j].size(); k++)
	    for (size_t l = 0; l<histos_[h][i]->h2d_3p()[j][k].size(); l++) histoname_[h][i].push_back(histos_[h][i]->h2d_3p()[j][k][l]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h3d_3p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h3d_3p()[j].size(); k++)
	    for (size_t l = 0; l<histos_[h][i]->h3d_3p()[j][k].size(); l++) histoname_[h][i].push_back(histos_[h][i]->h3d_3p()[j][k][l]->GetName());
	} else if (npf==4) {
	  for (size_t j = 0; j<histos_[h][i]->h1d_4p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h1d_4p()[j].size(); k++) 
	    for (size_t l = 0; l<histos_[h][i]->h1d_4p()[j][k].size(); l++) for (size_t m = 0; m<histos_[h][i]->h1d_4p()[j][k][l].size(); m++)
	      histoname_[h][i].push_back(histos_[h][i]->h1d_4p()[j][k][l][m]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h2d_4p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h2d_4p()[j].size(); k++)
	    for (size_t l = 0; l<histos_[h][i]->h2d_4p()[j][k].size(); l++) for (size_t m = 0; m<histos_[h][i]->h2d_4p()[j][k][l].size(); m++)
	      histoname_[h][i].push_back(histos_[h][i]->h2d_4p()[j][k][l][m]->GetName());
	  for (size_t j = 0; j<histos_[h][i]->h3d_4p().size(); j++) for (size_t k = 0; k<histos_[h][i]->h3d_4p()[j].size(); k++)
	    for (size_t l = 0; l<histos_[h][i]->h3d_4p()[j][k].size(); l++) for (size_t m = 0; m<histos_[h][i]->h3d_4p()[j][k][l].size(); m++)
	      histoname_[h][i].push_back(histos_[h][i]->h3d_4p()[j][k][l][m]->GetName());
	}

      }
    }
  }

  void create_canvases() {
    gethistonames_();
    TFile fi(fileread_);
    // Efficiency
    std::vector<TH1D*> det;
    std::vector<TH2D*> mod;
    std::vector<TH2D*> roc;
    for (size_t i=0; i<histoname_[0][0].size(); i++) {
      det.push_back((TH1D*)fi.Get(histoname_[0][0][i]));
      det[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[0][1].size(); i++) { 
      mod.push_back((TH2D*)fi.Get(histoname_[0][1][i])); 
      mod[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[0][2].size(); i++) { 
      roc.push_back((TH2D*)fi.Get(histoname_[0][2][i])); 
      roc[i]->SetDirectory(NULL);
    }
    // Nminusone
    std::vector<TH1D*> pt;
    std::vector<TH1D*> nstrip;
    std::vector<TH1D*> dz;
    std::vector<TH1D*> d0;
    std::vector<TH2D*> dz_d0;
    std::vector<TH1D*> normchi2;
    std::vector<TH2D*> fid;
    std::vector<TH1D*> fidlx;
    std::vector<TH1D*> fidly;
#ifdef COMPLETE
    std::vector<TH1D*> vtxd0;
    std::vector<TH1D*> vtxz;
#endif

    for (size_t i=0; i<histoname_[1][0].size(); i++) {
      pt.push_back((TH1D*)fi.Get(histoname_[1][0][i]));
      pt[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][1].size(); i++) {
      nstrip.push_back((TH1D*)fi.Get(histoname_[1][1][i]));
      nstrip[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][2].size(); i++) {
      dz.push_back((TH1D*)fi.Get(histoname_[1][2][i]));
      dz[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][3].size(); i++) {
      d0.push_back((TH1D*)fi.Get(histoname_[1][3][i]));
      d0[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][4].size(); i++) {
      dz_d0.push_back((TH2D*)fi.Get(histoname_[1][4][i]));
      dz_d0[i]->SetDirectory(NULL);
    }
/*     for (size_t i=0; i<histoname_[1][5].size(); i++) { */
/*       normchi2.push_back((TH1D*)fi.Get(histoname_[1][5][i])); */
/*       normchi2[i]->SetDirectory(NULL); */
/*     } */
    for (size_t i=0; i<histoname_[1][6].size(); i++) {
      fid.push_back((TH2D*)fi.Get(histoname_[1][6][i]));
      fid[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][7].size(); i++) {
      fidlx.push_back((TH1D*)fi.Get(histoname_[1][7][i]));
      fidlx[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][8].size(); i++) {
      fidly.push_back((TH1D*)fi.Get(histoname_[1][8][i]));
      fidly[i]->SetDirectory(NULL);
    }
#ifdef COMPLETE
    for (size_t i=0; i<histoname_[1][9].size(); i++) {
      vtxd0.push_back((TH1D*)fi.Get(histoname_[1][9][i]));
      vtxd0[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[1][10].size(); i++) {
      vtxz.push_back((TH1D*)fi.Get(histoname_[1][10][i]));
      vtxz[i]->SetDirectory(NULL);
    }
#endif

    // Scans
    std::vector<TH1D*> delay;
    std::vector<TH1D*> rog;
    std::vector<TH1D*> hv_11mar;
    std::vector<TH1D*> hv_10apr;
    for (size_t i=0; i<histoname_[2][0].size(); i++) {
      delay.push_back((TH1D*)fi.Get(histoname_[2][0][i]));
      delay[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[2][1].size(); i++) {
      rog.push_back((TH1D*)fi.Get(histoname_[2][1][i]));
      rog[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[2][2].size(); i++) {
      hv_11mar.push_back((TH1D*)fi.Get(histoname_[2][2][i]));
      hv_11mar[i]->SetDirectory(NULL);
    }
    for (size_t i=0; i<histoname_[2][3].size(); i++) {
      hv_10apr.push_back((TH1D*)fi.Get(histoname_[2][3][i]));
      hv_10apr[i]->SetDirectory(NULL);
    }
    fi.Close();

    gStyle->SetPalette(1);
    gStyle->SetOptStat(0);
    gStyle->SetOptTitle(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetFrameFillStyle(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasBorderSize(0);
    gStyle->SetPadBorderMode(0);
    gStyle->SetPadColor(0);

    gStyle->SetTitleFont(42);
    gStyle->SetLabelFont(42);
    
    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.17);
    TCanvas roc_l1_can("l1", "Layer 1 ROC Efficiency", 650,600);
    roc_l1_can.SetFillColor(0);
    setstyle_(roc[0]);
    roc[0]->GetXaxis()->SetTickLength(0.0);
    roc[0]->GetYaxis()->SetTickLength(0.0);
    roc[0]->GetXaxis()->SetTitle("Modules");
    roc[0]->GetYaxis()->SetTitle("Ladders");
    roc[0]->GetZaxis()->SetTitle("Efficiency");
    roc[0]->GetZaxis()->SetTitleOffset(1.2);
    roc[0]->GetZaxis()->SetRangeUser(0.0,1.0);
    roc[0]->Draw("COLZ");
    for (int i=1; i<=72; i++) {
      for (int j=1; j<=42; j++) {
	if (roc[1]->GetBinContent(i,j)>0)
	  roc[2]->SetBinContent(i,j,0);
      }
    }
    roc[2]->SetFillColor(1);
    roc[2]->SetFillStyle(3004);
    roc[2]->Draw("SAMEBOX");
    TH2D *roc_l1_out = new TH2D("roc_l1_out","roc_l1_out",72,-4.5,4.5,42,-10.5,10.5);
    roc_l1_out->SetFillColor(1);
    roc_l1_out->SetFillStyle(3002);
    for (int i=1; i<=72; i++) {
      roc_l1_out->SetBinContent(i,1,10);
      roc_l1_out->SetBinContent(i,20,10);
      roc_l1_out->SetBinContent(i,21,10);
      roc_l1_out->SetBinContent(i,22,10);
      roc_l1_out->SetBinContent(i,23,10);
      roc_l1_out->SetBinContent(i,42,10);
    }
    for (int i=1; i<=42; i++) {
      for (int j=33; j<=40; j++)
	roc_l1_out->SetBinContent(j,i,1);
    }
    roc_l1_out->Draw("SAMEBOX");
    TLine *line;
    int lad=10;
    for (int j=0; j<=5; j+=5) {
      line = new TLine(-4.5+j,-1*lad,-0.5+j,-1*lad); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,-1.0,-0.5+j,-1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,1.0,-0.5+j,1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,lad,-0.5+j,lad); line->SetLineColor(12); line->Draw();
      for (int i=0; i<lad-1; i++) {
        line = new TLine(-4.5+j,-1*lad+0.5+i,-0.5+j,-1*lad+0.5+i); line->SetLineColor(12); line->Draw();
        line = new TLine(-4.5+j,1.5+i,-0.5+j,1.5+i); line->SetLineColor(12); line->Draw();
      }
      for (float k=-1.0*lad; k<=-1.0; k+=0.5) {
	line = new TLine(-4.5,k,-0.5,k); line->SetLineColor(12); line->SetLineStyle(3); line->Draw(); 
	line = new TLine(-4.5+j,k,-0.5+j,k); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
	line = new TLine(-4.5,k+lad+1,-0.5,k+lad+1); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
	line = new TLine(-4.5+j,k+lad+1,-0.5+j,k+lad+1); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
      }
    }
    for (int j=0; j<=9; j++) {
      line = new TLine(-4.5+j,-1*lad,-4.5+j,-1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,1.0,-4.5+j,lad); line->SetLineColor(12); line->Draw();
    }
    TLatex *lat;
    lat = new TLatex(-3.5,1.0, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    for (int i=2; i<lad; i+=2) {
      lat = new TLatex(-4.49,i+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(13); lat->Draw();
      lat = new TLatex(-4.49,i+0.51, "0"); lat->SetTextSize(0.015); lat->Draw();
    }
    lat = new TLatex(-3.5,lad-0.5, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    lat = new TLatex(-3.5,-1*lad, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    for (int i=-2; i>-1*lad; i-=2) {
      lat = new TLatex(-4.49,i-1+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(13); lat->Draw();
      lat = new TLatex(-4.49,i-1+0.51, "0"); lat->SetTextSize(0.015); lat->Draw();
    }
    lat = new TLatex(-3.5,-1.5, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    for (int i=0; i<lad; i+=2) {
      lat = new TLatex(4.5,i+1+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(33); lat->Draw();
      lat = new TLatex(4.5,i+1+0.51, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(31); lat->Draw();
    }
    for (int i=0; i>-1*lad; i-=2) {
      lat = new TLatex(4.5,i-2+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(33); lat->Draw();
      lat = new TLatex(4.5,i-2+0.51, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(31); lat->Draw();
    }
   
    TCanvas roc_l2_can("l2", "Layer 2 ROC Efficiency", 650,600);
    roc_l2_can.SetFillColor(0);
    setstyle_(roc[3]);
    roc[3]->GetXaxis()->SetTickLength(0.0);
    roc[3]->GetYaxis()->SetTickLength(0.0);
    roc[3]->GetXaxis()->SetTitle("Modules");
    roc[3]->GetYaxis()->SetTitle("Ladders");
    roc[3]->GetZaxis()->SetTitle("Efficiency");
    roc[3]->GetZaxis()->SetTitleOffset(1.2);
    roc[3]->GetZaxis()->SetRangeUser(0.8,1.0);
    roc[3]->Draw("COLZ");
    for (int i=1; i<=72; i++) {
      for (int j=1; j<=66; j++) {
	if (roc[4]->GetBinContent(i,j)>0)
	  roc[5]->SetBinContent(i,j,0);
      }
    }
    roc[5]->SetFillColor(1);
    roc[5]->SetFillStyle(3004);
    roc[5]->Draw("SAMEBOX");
    TH2D *roc_l2_out = new TH2D("roc_l2_out","roc_l2_out",72,-4.5,4.5,66,-16.5,16.5);
    roc_l2_out->SetFillColor(1);
    roc_l2_out->SetFillStyle(3002);
    for (int i=1; i<=72; i++) {
      roc_l2_out->SetBinContent(i,1,10);
      roc_l2_out->SetBinContent(i,32,10);
      roc_l2_out->SetBinContent(i,33,10);
      roc_l2_out->SetBinContent(i,34,10);
      roc_l2_out->SetBinContent(i,35,10);
      roc_l2_out->SetBinContent(i,66,10);
    }
    for (int i=1; i<=66; i++) {
      for (int j=33; j<=40; j++)
	roc_l2_out->SetBinContent(j,i,1);
    }
    roc_l2_out->Draw("SAMEBOX");
    lad=16;
    for (int j=0; j<=5; j+=5) {
      line = new TLine(-4.5+j,-1*lad,-0.5+j,-1*lad); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,-1.0,-0.5+j,-1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,1.0,-0.5+j,1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,lad,-0.5+j,lad); line->SetLineColor(12); line->Draw();
      for (int i=0; i<lad-1; i++) {
        line = new TLine(-4.5+j,-1*lad+0.5+i,-0.5+j,-1*lad+0.5+i); line->SetLineColor(12); line->Draw();
        line = new TLine(-4.5+j,1.5+i,-0.5+j,1.5+i); line->SetLineColor(12); line->Draw();
      }
      for (float k=-1.0*lad; k<=-1.0; k+=0.5) {
	line = new TLine(-4.5,k,-0.5,k); line->SetLineColor(12); line->SetLineStyle(3); line->Draw(); 
	line = new TLine(-4.5+j,k,-0.5+j,k); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
	line = new TLine(-4.5,k+lad+1,-0.5,k+lad+1); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
	line = new TLine(-4.5+j,k+lad+1,-0.5+j,k+lad+1); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
      }
    }
    for (int j=0; j<=9; j++) {
      line = new TLine(-4.5+j,-1*lad,-4.5+j,-1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,1.0,-4.5+j,lad); line->SetLineColor(12); line->Draw();
    }
    lat = new TLatex(-3.5,1.0, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    for (int i=2; i<lad; i+=2) {
      lat = new TLatex(-4.49,i+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(13); lat->Draw();
      lat = new TLatex(-4.49,i+0.51, "0"); lat->SetTextSize(0.015); lat->Draw();
    }
    lat = new TLatex(-3.5,lad-0.5, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    lat = new TLatex(-3.5,-1*lad, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    for (int i=-2; i>-1*lad; i-=2) {
      lat = new TLatex(-4.49,i-1+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(13); lat->Draw();
      lat = new TLatex(-4.49,i-1+0.51, "0"); lat->SetTextSize(0.015); lat->Draw();
    }
    lat = new TLatex(-3.5,-1.5, "8"); lat->SetTextSize(0.017); lat->SetTextAlign(31); lat->Draw();
    for (int i=0; i<lad; i+=2) {
      lat = new TLatex(4.5,i+1+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(33); lat->Draw();
      lat = new TLatex(4.5,i+1+0.51, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(31); lat->Draw();
    }
    for (int i=0; i>-1*lad; i-=2) {
      lat = new TLatex(4.5,i-2+0.49, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(33); lat->Draw();
      lat = new TLatex(4.5,i-2+0.51, "0"); lat->SetTextSize(0.015); lat->SetTextAlign(31); lat->Draw();
    }

    TCanvas roc_l3_can("l3", "Layer 3 ROC Efficiency", 650,600);
    roc_l3_can.SetFillColor(0);
    setstyle_(roc[6]);
    roc[6]->GetXaxis()->SetTickLength(0.0);
    roc[6]->GetYaxis()->SetTickLength(0.0);
    roc[6]->GetXaxis()->SetTitle("Modules");
    roc[6]->GetYaxis()->SetTitle("Ladders");
    roc[6]->GetZaxis()->SetTitle("Efficiency");
    roc[6]->GetZaxis()->SetTitleOffset(1.2);
    roc[6]->GetZaxis()->SetRangeUser(0.0,1.0);
    roc[6]->Draw("COLZ");
    for (int i=1; i<=72; i++) {
      for (int j=1; j<=90; j++) {
	if (roc[7]->GetBinContent(i,j)>0)
	  roc[8]->SetBinContent(i,j,0);
      }
    }
    roc[8]->SetFillColor(1);
    roc[8]->SetFillStyle(3004);
    roc[8]->Draw("SAMEBOX");
    TH2D *roc_l3_out = new TH2D("roc_l3_out","roc_l3_out",72,-4.5,4.5,90,-22.5,22.5);
    roc_l3_out->SetFillColor(1);
    roc_l3_out->SetFillStyle(3002);
    for (int i=1; i<=72; i++) {
      roc_l3_out->SetBinContent(i,1,10);
      roc_l3_out->SetBinContent(i,44,10);
      roc_l3_out->SetBinContent(i,45,10);
      roc_l3_out->SetBinContent(i,46,10);
      roc_l3_out->SetBinContent(i,47,10);
      roc_l3_out->SetBinContent(i,90,10);
    }
    for (int i=1; i<=90; i++) {
      for (int j=33; j<=40; j++)
	roc_l3_out->SetBinContent(j,i,1);
    }
    roc_l3_out->Draw("SAMEBOX");
    lad=22;
    for (int j=0; j<=5; j+=5) {
      line = new TLine(-4.5+j,-1*lad,-0.5+j,-1*lad); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,-1.0,-0.5+j,-1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,1.0,-0.5+j,1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,lad,-0.5+j,lad); line->SetLineColor(12); line->Draw();
      for (int i=0; i<lad-1; i++) {
        line = new TLine(-4.5+j,-1*lad+0.5+i,-0.5+j,-1*lad+0.5+i); line->SetLineColor(12); line->Draw();
        line = new TLine(-4.5+j,1.5+i,-0.5+j,1.5+i); line->SetLineColor(12); line->Draw();
      }
      for (float k=-1.0*lad; k<=-1.0; k+=0.5) {
	line = new TLine(-4.5,k,-0.5,k); line->SetLineColor(12); line->SetLineStyle(3); line->Draw(); 
	line = new TLine(-4.5+j,k,-0.5+j,k); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
	line = new TLine(-4.5,k+lad+1,-0.5,k+lad+1); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
	line = new TLine(-4.5+j,k+lad+1,-0.5+j,k+lad+1); line->SetLineColor(12); line->SetLineStyle(3); line->Draw();
      }
    }
    for (int j=0; j<=9; j++) {
      line = new TLine(-4.5+j,-1*lad,-4.5+j,-1.0); line->SetLineColor(12); line->Draw();
      line = new TLine(-4.5+j,1.0,-4.5+j,lad); line->SetLineColor(12); line->Draw();
    }
    lat = new TLatex(-3.5,1.0, "8"); lat->SetTextSize(0.012); lat->SetTextAlign(31); lat->Draw();
    for (int i=2; i<lad; i+=2) {
      lat = new TLatex(-4.49,i+0.49, "0"); lat->SetTextSize(0.012); lat->SetTextAlign(13); lat->Draw();
      lat = new TLatex(-4.49,i+0.51, "0"); lat->SetTextSize(0.012); lat->Draw();
    }
    lat = new TLatex(-3.5,lad-0.5, "8"); lat->SetTextSize(0.012); lat->SetTextAlign(31); lat->Draw();
    lat = new TLatex(-3.5,-1*lad, "8"); lat->SetTextSize(0.012); lat->SetTextAlign(31); lat->Draw();
    for (int i=-2; i>-1*lad; i-=2) {
      lat = new TLatex(-4.49,i-1+0.49, "0"); lat->SetTextSize(0.012); lat->SetTextAlign(13); lat->Draw();
      lat = new TLatex(-4.49,i-1+0.51, "0"); lat->SetTextSize(0.012); lat->Draw();
    }
    lat = new TLatex(-3.5,-1.5, "8"); lat->SetTextSize(0.012); lat->SetTextAlign(31); lat->Draw();
    for (int i=0; i<lad; i+=2) {
      lat = new TLatex(4.5,i+1+0.49, "0"); lat->SetTextSize(0.012); lat->SetTextAlign(33); lat->Draw();
      lat = new TLatex(4.5,i+1+0.51, "0"); lat->SetTextSize(0.012); lat->SetTextAlign(31); lat->Draw();
    }
    for (int i=0; i>-1*lad; i-=2) {
      lat = new TLatex(4.5,i-2+0.49, "0"); lat->SetTextSize(0.012); lat->SetTextAlign(33); lat->Draw();
      lat = new TLatex(4.5,i-2+0.51, "0"); lat->SetTextSize(0.012); lat->SetTextAlign(31); lat->Draw();
    }
    
    gStyle->SetPadRightMargin(0.2);
    TCanvas roc_fpixI_can("fpixI", "Inner FPIX ROC Efficiency", 650,600);
    roc_fpixI_can.SetFillColor(0);
    setstyle_(roc[9]);
    roc[9]->GetXaxis()->SetBinLabel(1, "Disk-2 Pnl2");
    roc[9]->GetXaxis()->SetBinLabel(9, "Disk-2 Pnl1");
    roc[9]->GetXaxis()->SetBinLabel(19, "Disk-1 Pnl2");
    roc[9]->GetXaxis()->SetBinLabel(27, "Disk-1 Pnl1");
    roc[9]->GetXaxis()->SetBinLabel(41, "Disk+1 Pnl1");
    roc[9]->GetXaxis()->SetBinLabel(49, "Disk+1 Pnl2");
    roc[9]->GetXaxis()->SetBinLabel(59, "Disk+2 Pnl1");
    roc[9]->GetXaxis()->SetBinLabel(67, "Disk+2 Pnl2");
    roc[9]->GetXaxis()->SetTickLength(0.0);
    roc[9]->GetYaxis()->SetTickLength(0.0);
    roc[9]->GetXaxis()->LabelsOption("d");
    roc[9]->GetYaxis()->SetTitle("Blades");
    roc[9]->GetZaxis()->SetTitle("Efficiency");
    roc[9]->GetZaxis()->SetTitleOffset(1.5);
    roc[9]->GetZaxis()->SetRangeUser(0.95,1.0);
    roc[9]->Draw("COLZ");
    for (int i=1; i<=72; i++) {
      for (int j=1; j<=144; j++) {
        if (roc[10]->GetBinContent(i,j)>0)
          roc[11]->SetBinContent(i,j,0);
      }
    }
    roc[11]->SetFillColor(1);
    roc[11]->SetFillStyle(3004);
    roc[11]->Draw("SAMEBOX");
    TH2D *roc_fpixI_out = new TH2D("roc_fpixI_out","roc_fpixI_out",72,-4.5,4.5,144,0.5,12.5);
    roc_fpixI_out->SetFillColor(11);
    roc_fpixI_out->SetFillStyle(3002);
    for (int i=1; i<=144; i++) {
      roc_fpixI_out->SetBinContent(7,i,10);
      roc_fpixI_out->SetBinContent(8,i,10);
      roc_fpixI_out->SetBinContent(15,i,10);
      roc_fpixI_out->SetBinContent(16,i,10);
      roc_fpixI_out->SetBinContent(17,i,10);
      roc_fpixI_out->SetBinContent(18,i,10);
      roc_fpixI_out->SetBinContent(25,i,10);
      roc_fpixI_out->SetBinContent(26,i,10);
      for (int j=33; j<=40; j++) roc_fpixI_out->SetBinContent(j,i,10);
      roc_fpixI_out->SetBinContent(47,i,10);
      roc_fpixI_out->SetBinContent(48,i,10);
      roc_fpixI_out->SetBinContent(55,i,10);
      roc_fpixI_out->SetBinContent(56,i,10);
      roc_fpixI_out->SetBinContent(57,i,10);
      roc_fpixI_out->SetBinContent(58,i,10);
      roc_fpixI_out->SetBinContent(65,i,10);
      roc_fpixI_out->SetBinContent(66,i,10);
    }
    for (int i=0; i<12; i++) {
      for (int j=0; j<2; j++) {
        for (int k=1; k<=6; k++) {
          roc_fpixI_out->SetBinContent(k+j*18,1+12*i,10);
          roc_fpixI_out->SetBinContent(k+j*18,12+12*i,10);
          roc_fpixI_out->SetBinContent(73-(k+j*18),1+12*i,10);
          roc_fpixI_out->SetBinContent(73-(k+j*18),12+12*i,10);
          roc_fpixI_out->SetBinContent(8+k+j*18,1+12*i,10);
          roc_fpixI_out->SetBinContent(8+k+j*18,12+12*i,10);
          roc_fpixI_out->SetBinContent(-8+73-(k+j*18),1+12*i,10);
          roc_fpixI_out->SetBinContent(-8+73-(k+j*18),12+12*i,10);
        }
        for (int k=3; k<=6; k++) {
          roc_fpixI_out->SetBinContent(k+j*18,2+12*i,10);
          roc_fpixI_out->SetBinContent(k+j*18,11+12*i,10);
          roc_fpixI_out->SetBinContent(73-(k+j*18),2+12*i,10);
          roc_fpixI_out->SetBinContent(73-(k+j*18),11+12*i,10);
        }
        for (int k=2; k<=6; k++) {
          roc_fpixI_out->SetBinContent(8+k+j*18,2+12*i,10);
          roc_fpixI_out->SetBinContent(8+k+j*18,11+12*i,10);
          roc_fpixI_out->SetBinContent(-8+73-(k+j*18),2+12*i,10);
          roc_fpixI_out->SetBinContent(-8+73-(k+j*18),11+12*i,10);
        }
        for (int k=5; k<=6; k++) {
          roc_fpixI_out->SetBinContent(k+j*18,3+12*i,10);
          roc_fpixI_out->SetBinContent(k+j*18,10+12*i,10);
          roc_fpixI_out->SetBinContent(73-(k+j*18),3+12*i,10);
          roc_fpixI_out->SetBinContent(73-(k+j*18),10+12*i,10);
        }
        for (int k=4; k<=6; k++) {
          roc_fpixI_out->SetBinContent(8+k+j*18,3+12*i,10);
          roc_fpixI_out->SetBinContent(8+k+j*18,10+12*i,10);
          roc_fpixI_out->SetBinContent(-8+73-(k+j*18),3+12*i,10);
          roc_fpixI_out->SetBinContent(-8+73-(k+j*18),10+12*i,10);
        }
        roc_fpixI_out->SetBinContent(8+6+j*18,4+12*i,10);
        roc_fpixI_out->SetBinContent(8+6+j*18,9+12*i,10);
        roc_fpixI_out->SetBinContent(-8+73-(6+j*18),4+12*i,10);
        roc_fpixI_out->SetBinContent(-8+73-(6+j*18),9+12*i,10);
      }
    }
    roc_fpixI_out->Draw("SAMEBOX");
    for (int i=0; i<12; i++) {
      for (int side=-1; side<=1; side+=2) {
	for (int panel=0; panel<=1; panel++) {
	  for (int disk=0; disk<=1; disk++) {
	    //Horizontal lines
	    line = new TLine(side*(-4.5+(0.0/8)+panel*18.0/8),7.0/12+i,side*(-4.5+(2.0/8)+panel*18.0/8),7.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(0.0/8)+panel*18.0/8),17.0/12+i,side*(-4.5+(2.0/8)+panel*18.0/8),17.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(2.0/8)+panel*18.0/8),8.0/12+i,side*(-4.5+(4.0/8)+panel*18.0/8),8.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(2.0/8)+panel*18.0/8),16.0/12+i,side*(-4.5+(4.0/8)+panel*18.0/8),16.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(4.0/8)+panel*18.0/8),9.0/12+i,side*(-4.5+(6.0/8)+panel*18.0/8),9.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(4.0/8)+panel*18.0/8),15.0/12+i,side*(-4.5+(6.0/8)+panel*18.0/8),15.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(0.0/8)+panel*18.0/8),7.0/12+i,side*(-3.5+(1.0/8)+panel*18.0/8),7.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(0.0/8)+panel*18.0/8),17.0/12+i,side*(-3.5+(1.0/8)+panel*18.0/8),17.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(1.0/8)+panel*18.0/8),8.0/12+i,side*(-3.5+(3.0/8)+panel*18.0/8),8.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(1.0/8)+panel*18.0/8),16.0/12+i,side*(-3.5+(3.0/8)+panel*18.0/8),16.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(3.0/8)+panel*18.0/8),9.0/12+i,side*(-3.5+(5.0/8)+panel*18.0/8),9.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(3.0/8)+panel*18.0/8),15.0/12+i,side*(-3.5+(5.0/8)+panel*18.0/8),15.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(5.0/8)+panel*18.0/8),10.0/12+i,side*(-3.5+(6.0/8)+panel*18.0/8),10.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(5.0/8)+panel*18.0/8),14.0/12+i,side*(-3.5+(6.0/8)+panel*18.0/8),14.0/12+i); line->SetLineColor(12); line->Draw();
	    // Vertical lines
	    line = new TLine(side*(-4.5+(0.0/8)+panel*18.0/8),7.0/12+i,side*(-4.5+(0.0/8)+panel*18.0/8),17.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(2.0/8)+panel*18.0/8),7.0/12+i,side*(-4.5+(2.0/8)+panel*18.0/8),17.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(4.0/8)+panel*18.0/8),8.0/12+i,side*(-4.5+(4.0/8)+panel*18.0/8),16.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(6.0/8)+panel*18.0/8),9.0/12+i,side*(-4.5+(6.0/8)+panel*18.0/8),15.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(0.0/8)+panel*18.0/8),7.0/12+i,side*(-3.5+(0.0/8)+panel*18.0/8),17.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(1.0/8)+panel*18.0/8),7.0/12+i,side*(-3.5+(1.0/8)+panel*18.0/8),17.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(3.0/8)+panel*18.0/8),8.0/12+i,side*(-3.5+(3.0/8)+panel*18.0/8),16.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(5.0/8)+panel*18.0/8),9.0/12+i,side*(-3.5+(5.0/8)+panel*18.0/8),15.0/12+i); line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(6.0/8)+panel*18.0/8),10.0/12+i,side*(-3.5+(6.0/8)+panel*18.0/8),14.0/12+i); line->SetLineColor(12); line->Draw();
	  }
	}
      }
    }
    lat = new TLatex(-4.48+1.0/8 ,1.0-5.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-4.48+3.0/8 ,1.0-4.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-4.48+5.0/8 ,1.0-3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48       ,1.0-5.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48+2.0/8 ,1.0-4.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48+4.0/8 ,1.0-3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48+5.0/8 ,1.0-2.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(3.52+2.0/8 ,1.0+1.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(3.52+4.0/8 ,1.0+2.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(3.52+6.0/8 ,1.0+3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+2.0/8 ,1.0,        "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+3.0/8 ,1.0+1.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+5.0/8 ,1.0+2.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+7.0/8 ,1.0+3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();


    TCanvas roc_fpixO_can("fpixO", "Outer FPIX ROC Efficiency", 650,600);
    roc_fpixO_can.SetFillColor(0);
    setstyle_(roc[12]);
    roc[12]->GetXaxis()->SetBinLabel(1, "Disk-2 Pnl2");
    roc[12]->GetXaxis()->SetBinLabel(9, "Disk-2 Pnl1");
    roc[12]->GetXaxis()->SetBinLabel(19, "Disk-1 Pnl2");
    roc[12]->GetXaxis()->SetBinLabel(27, "Disk-1 Pnl1");
    roc[12]->GetXaxis()->SetBinLabel(41, "Disk+1 Pnl1");
    roc[12]->GetXaxis()->SetBinLabel(49, "Disk+1 Pnl2");
    roc[12]->GetXaxis()->SetBinLabel(59, "Disk+2 Pnl1");
    roc[12]->GetXaxis()->SetBinLabel(67, "Disk+2 Pnl2");
    roc[12]->GetXaxis()->SetTickLength(0.0);
    roc[12]->GetYaxis()->SetTickLength(0.0);
    roc[12]->GetXaxis()->LabelsOption("d");
    roc[12]->GetYaxis()->SetTitle("Blades");
    roc[12]->GetZaxis()->SetTitle("Efficiency");
    roc[12]->GetZaxis()->SetTitleOffset(1.5);
    roc[12]->GetZaxis()->SetRangeUser(0.95,1.0);
    roc[12]->Draw("COLZ");
    for (int i=1; i<=72; i++) {
      for (int j=1; j<=144; j++) {
        if (roc[13]->GetBinContent(i,j)>0)
          roc[14]->SetBinContent(i,j,0);
      }
    }
    roc[14]->SetFillColor(1);
    roc[14]->SetFillStyle(3004);
    roc[14]->Draw("SAMEBOX");
    TH2D *roc_fpixO_out = new TH2D("roc_fpixO_out","roc_fpixO_out",72,-4.5,4.5,144,-12.5,-0.5);
    roc_fpixO_out->SetFillColor(11);
    roc_fpixO_out->SetFillStyle(3002);
    for (int i=1; i<=144; i++) {
      roc_fpixO_out->SetBinContent(7,i,10);
      roc_fpixO_out->SetBinContent(8,i,10);
      roc_fpixO_out->SetBinContent(15,i,10);
      roc_fpixO_out->SetBinContent(16,i,10);
      roc_fpixO_out->SetBinContent(17,i,10);
      roc_fpixO_out->SetBinContent(18,i,10);
      roc_fpixO_out->SetBinContent(25,i,10);
      roc_fpixO_out->SetBinContent(26,i,10);
      for (int j=33; j<=40; j++) roc_fpixO_out->SetBinContent(j,i,10);
      roc_fpixO_out->SetBinContent(47,i,10);
      roc_fpixO_out->SetBinContent(48,i,10);
      roc_fpixO_out->SetBinContent(55,i,10);
      roc_fpixO_out->SetBinContent(56,i,10);
      roc_fpixO_out->SetBinContent(57,i,10);
      roc_fpixO_out->SetBinContent(58,i,10);
      roc_fpixO_out->SetBinContent(65,i,10);
      roc_fpixO_out->SetBinContent(66,i,10);
    }
    for (int i=0; i<12; i++) {
      for (int j=0; j<2; j++) {
        for (int k=1; k<=6; k++) {
          roc_fpixO_out->SetBinContent(k+j*18,1+12*i,10);
          roc_fpixO_out->SetBinContent(k+j*18,12+12*i,10);
          roc_fpixO_out->SetBinContent(73-(k+j*18),1+12*i,10);
          roc_fpixO_out->SetBinContent(73-(k+j*18),12+12*i,10);
          roc_fpixO_out->SetBinContent(8+k+j*18,1+12*i,10);
          roc_fpixO_out->SetBinContent(8+k+j*18,12+12*i,10);
          roc_fpixO_out->SetBinContent(-8+73-(k+j*18),1+12*i,10);
          roc_fpixO_out->SetBinContent(-8+73-(k+j*18),12+12*i,10);
        }
        for (int k=3; k<=6; k++) {
          roc_fpixO_out->SetBinContent(k+j*18,2+12*i,10);
          roc_fpixO_out->SetBinContent(k+j*18,11+12*i,10);
          roc_fpixO_out->SetBinContent(73-(k+j*18),2+12*i,10);
          roc_fpixO_out->SetBinContent(73-(k+j*18),11+12*i,10);
        }
        for (int k=2; k<=6; k++) {
          roc_fpixO_out->SetBinContent(8+k+j*18,2+12*i,10);
          roc_fpixO_out->SetBinContent(8+k+j*18,11+12*i,10);
          roc_fpixO_out->SetBinContent(-8+73-(k+j*18),2+12*i,10);
          roc_fpixO_out->SetBinContent(-8+73-(k+j*18),11+12*i,10);
        }
        for (int k=5; k<=6; k++) {
          roc_fpixO_out->SetBinContent(k+j*18,3+12*i,10);
          roc_fpixO_out->SetBinContent(k+j*18,10+12*i,10);
          roc_fpixO_out->SetBinContent(73-(k+j*18),3+12*i,10);
          roc_fpixO_out->SetBinContent(73-(k+j*18),10+12*i,10);
        }
        for (int k=4; k<=6; k++) {
          roc_fpixO_out->SetBinContent(8+k+j*18,3+12*i,10);
          roc_fpixO_out->SetBinContent(8+k+j*18,10+12*i,10);
          roc_fpixO_out->SetBinContent(-8+73-(k+j*18),3+12*i,10);
          roc_fpixO_out->SetBinContent(-8+73-(k+j*18),10+12*i,10);
        }
        roc_fpixO_out->SetBinContent(8+6+j*18,4+12*i,10);
        roc_fpixO_out->SetBinContent(8+6+j*18,9+12*i,10);
        roc_fpixO_out->SetBinContent(-8+73-(6+j*18),4+12*i,10);
        roc_fpixO_out->SetBinContent(-8+73-(6+j*18),9+12*i,10);
      }
    }
    roc_fpixO_out->Draw("SAMEBOX");
    for (int i=0; i<12; i++) {
      for (int side=-1; side<=1; side+=2) {
	for (int panel=0; panel<=1; panel++) {
	  for (int disk=0; disk<=1; disk++) {
	    //Horizontal lines
	    line = new TLine(side*(-4.5+(0.0/8)+panel*18.0/8),-13+7.0/12+i,side*(-4.5+(2.0/8)+panel*18.0/8),-13+7.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(0.0/8)+panel*18.0/8),-13+17.0/12+i,side*(-4.5+(2.0/8)+panel*18.0/8),-13+17.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(2.0/8)+panel*18.0/8),-13+8.0/12+i,side*(-4.5+(4.0/8)+panel*18.0/8),-13+8.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(2.0/8)+panel*18.0/8),-13+16.0/12+i,side*(-4.5+(4.0/8)+panel*18.0/8),-13+16.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(4.0/8)+panel*18.0/8),-13+9.0/12+i,side*(-4.5+(6.0/8)+panel*18.0/8),-13+9.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(4.0/8)+panel*18.0/8),-13+15.0/12+i,side*(-4.5+(6.0/8)+panel*18.0/8),-13+15.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(0.0/8)+panel*18.0/8),-13+7.0/12+i,side*(-3.5+(1.0/8)+panel*18.0/8),-13+7.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(0.0/8)+panel*18.0/8),-13+17.0/12+i,side*(-3.5+(1.0/8)+panel*18.0/8),-13+17.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(1.0/8)+panel*18.0/8),-13+8.0/12+i,side*(-3.5+(3.0/8)+panel*18.0/8),-13+8.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(1.0/8)+panel*18.0/8),-13+16.0/12+i,side*(-3.5+(3.0/8)+panel*18.0/8),-13+16.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(3.0/8)+panel*18.0/8),-13+9.0/12+i,side*(-3.5+(5.0/8)+panel*18.0/8),-13+9.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(3.0/8)+panel*18.0/8),-13+15.0/12+i,side*(-3.5+(5.0/8)+panel*18.0/8),-13+15.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(5.0/8)+panel*18.0/8),-13+10.0/12+i,side*(-3.5+(6.0/8)+panel*18.0/8),-13+10.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(5.0/8)+panel*18.0/8),-13+14.0/12+i,side*(-3.5+(6.0/8)+panel*18.0/8),-13+14.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    // Vertical lines
	    line = new TLine(side*(-4.5+(0.0/8)+panel*18.0/8),-13+7.0/12+i,side*(-4.5+(0.0/8)+panel*18.0/8),-13+17.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(2.0/8)+panel*18.0/8),-13+7.0/12+i,side*(-4.5+(2.0/8)+panel*18.0/8),-13+17.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(4.0/8)+panel*18.0/8),-13+8.0/12+i,side*(-4.5+(4.0/8)+panel*18.0/8),-13+16.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-4.5+(6.0/8)+panel*18.0/8),-13+9.0/12+i,side*(-4.5+(6.0/8)+panel*18.0/8),-13+15.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(0.0/8)+panel*18.0/8),-13+7.0/12+i,side*(-3.5+(0.0/8)+panel*18.0/8),-13+17.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(1.0/8)+panel*18.0/8),-13+7.0/12+i,side*(-3.5+(1.0/8)+panel*18.0/8),-13+17.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(3.0/8)+panel*18.0/8),-13+8.0/12+i,side*(-3.5+(3.0/8)+panel*18.0/8),-13+16.0/12+i);
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(5.0/8)+panel*18.0/8),-13+9.0/12+i,side*(-3.5+(5.0/8)+panel*18.0/8),-13+15.0/12+i); 
	    line->SetLineColor(12); line->Draw();
	    line = new TLine(side*(-3.5+(6.0/8)+panel*18.0/8),-13+10.0/12+i,side*(-3.5+(6.0/8)+panel*18.0/8),-13+14.0/12+i);
	    line->SetLineColor(12); line->Draw();
	  }
	}
      }
    }
    lat = new TLatex(-4.48+1.0/8 ,-12.0-5.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-4.48+3.0/8 ,-12.0-4.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-4.48+5.0/8 ,-12.0-3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48       ,-12.0-5.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48+2.0/8 ,-12.0-4.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48+4.0/8 ,-12.0-3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(-3.48+5.0/8 ,-12.0-2.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(3.52+2.0/8 ,-12.0+1.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(3.52+4.0/8 ,-12.0+2.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(3.52+6.0/8 ,-12.0+3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+2.0/8 ,-12.0,        "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+3.0/8 ,-12.0+1.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+5.0/8 ,-12.0+2.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();
    lat = new TLatex(2.52+7.0/8 ,-12.0+3.0/12, "0"); lat->SetTextSize(0.015); lat->Draw();


    // N-1 plots

    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.12);
    set_det_cumeff_(pt, "Pt [GeV]");
    set_det_cumeff_(nstrip, "Number of Strip Hits");

    std::vector<std::vector<TCanvas*> > nminus_can;
    for (int i=0; i<2; i++) nminus_can.push_back(std::vector<TCanvas*>());
    for (int i=0; i<10; i++) {
      nminus_can[0].push_back(det_can_(pt, i, "pt", "N-1 - Eff vs Pt"));
      nminus_can[1].push_back(det_can_(nstrip, i, "nstrip", "N-1 - Eff vs Nstrip"));
    }


/*     TCanvas fid_l1_full_can("fid_l1_full_can", "Layer1 full module fiducial region", 1200,800); */
/*     fid_l1_full_can.SetFillColor(0); */
/*     fid_l1_full_can.Divide(1,2); */
/*     fid_l1_full_can.cd(1); */
/*     setstyle_(fid_l1_full_eff); */
/*     fid_l1_full_eff->GetXaxis()->SetTitle("local Y (cm)"); */
/*     fid_l1_full_eff->GetYaxis()->SetTitle("local X (cm)"); */
/*     fid_l1_full_eff->GetZaxis()->SetRangeUser(0.99,1.00); */
/*     fid_l1_full_eff->Draw("CONTZ"); */
/*     TVirtualPad* fid_l1_full_can_2 = fid_l1_full_can.cd(2); */
/*     fid_l1_full_can_2->Divide(2); */
/*     fid_l1_full_can_2->cd(1); */
/*     setstyle_(fid_l1_full_lx_eff); */
/*     fid_l1_full_lx_eff->GetXaxis()->SetTitle("local X (cm)"); */
/*     fid_l1_full_lx_eff->GetYaxis()->SetTitle("Efficiency"); */
/*     fid_l1_full_lx_eff->Draw(); */
/*     fid_l1_full_can_2->cd(2); */
/*     setstyle_(fid_l1_full_ly_eff); */
/*     fid_l1_full_ly_eff->GetXaxis()->SetTitle("local Y (cm)"); */
/*     fid_l1_full_ly_eff->GetYaxis()->SetTitle("Efficiency"); */
/*     fid_l1_full_ly_eff->Draw(); */
/*     TCanvas fid_l1_half_can("fid_l1_half_can", "Layer1 half module fiducial region", 1200,800); */
/*     fid_l1_half_can.SetFillColor(0); */
/*     fid_l1_half_can.Divide(1,2); */
/*     fid_l1_half_can.cd(1); */
/*     setstyle_(fid_l1_half_eff); */
/*     fid_l1_half_eff->GetXaxis()->SetTitle("local Y (cm)"); */
/*     fid_l1_half_eff->GetYaxis()->SetTitle("local X (cm)"); */
/*     fid_l1_half_eff->GetZaxis()->SetRangeUser(0.99,1.00); */
/*     fid_l1_half_eff->Draw("CONTZ"); */
/*     TVirtualPad* fid_l1_half_can_2 = fid_l1_half_can.cd(2); */
/*     fid_l1_half_can_2->Divide(2); */
/*     fid_l1_half_can_2->cd(1); */
/*     setstyle_(fid_l1_half_lx_eff); */
/*     fid_l1_half_lx_eff->GetXaxis()->SetTitle("local X (cm)"); */
/*     fid_l1_half_lx_eff->GetYaxis()->SetTitle("Efficiency"); */
/*     fid_l1_half_lx_eff->Draw(); */
/*     fid_l1_half_can_2->cd(2); */
/*     setstyle_(fid_l1_half_ly_eff); */
/*     fid_l1_half_ly_eff->GetXaxis()->SetTitle("local Y (cm)"); */
/*     fid_l1_half_ly_eff->GetYaxis()->SetTitle("Efficiency"); */
/*     fid_l1_half_ly_eff->Draw(); */

    TFile fo(filewrite_,"update");
    roc_l1_can.Write();
    roc_l2_can.Write();
    roc_l3_can.Write();
    roc_fpixI_can.Write();
    roc_fpixO_can.Write();
    for (size_t i=0; i<nminus_can.size(); i++) 
      for (int j=0; j<10; j++)  nminus_can[i][j]->Write();
/*     pt_bpix_can.Write(); */
/*     pt_fpix_can.Write(); */
/*     fid_l1_full_can.Write(); */
/*     fid_l1_half_can.Write(); */
/*     delay_onclusize_can.Write(); */
/*     delay_bpix_onclusize_can.Write(); */
/*     delay_fpix_onclusize_can.Write(); */
/*     delay_l1_onclusize_can.Write(); */
/*     delay_l2_onclusize_can.Write(); */
/*     delay_l3_onclusize_can.Write(); */
/*     delay_dm2_onclusize_can.Write(); */
/*     delay_dm1_onclusize_can.Write(); */
/*     delay_dp1_onclusize_can.Write(); */
/*     delay_dp2_onclusize_can.Write(); */
    fo.Close();

  }


  void create_() {
    TFile fi(fileread_);
    TH2D *roc_l1_eff = (TH2D*)fi.Get("roc_l1_eff");
    TH2D *roc_l2_eff = (TH2D*)fi.Get("roc_l2_eff");
    TH2D *roc_l3_eff = (TH2D*)fi.Get("roc_l3_eff");
    TH2D *roc_fpixI_eff = (TH2D*)fi.Get("roc_fpixI_eff");
    TH2D *roc_fpixO_eff = (TH2D*)fi.Get("roc_fpixO_eff");
    TH2D *roc_l1_dist = (TH2D*)fi.Get("roc_l1_dist");
    TH2D *roc_l2_dist = (TH2D*)fi.Get("roc_l2_dist");
    TH2D *roc_l3_dist = (TH2D*)fi.Get("roc_l3_dist");
    TH2D *roc_fpixI_dist = (TH2D*)fi.Get("roc_fpixI_dist");
    TH2D *roc_fpixO_dist = (TH2D*)fi.Get("roc_fpixO_dist");
    TH2D *roc_l1_inac = (TH2D*)fi.Get("roc_l1_inac");
    TH2D *roc_l2_inac = (TH2D*)fi.Get("roc_l2_inac");
    TH2D *roc_l3_inac = (TH2D*)fi.Get("roc_l3_inac");
    TH2D *roc_fpixI_inac = (TH2D*)fi.Get("roc_fpixI_inac");
    TH2D *roc_fpixO_inac = (TH2D*)fi.Get("roc_fpixO_inac");
    roc_l1_eff->SetDirectory(NULL);
    roc_l2_eff->SetDirectory(NULL);
    roc_l3_eff->SetDirectory(NULL);
    roc_fpixI_eff->SetDirectory(NULL);
    roc_fpixO_eff->SetDirectory(NULL);
    roc_l1_dist->SetDirectory(NULL);
    roc_l2_dist->SetDirectory(NULL);
    roc_l3_dist->SetDirectory(NULL);
    roc_fpixI_dist->SetDirectory(NULL);
    roc_fpixO_dist->SetDirectory(NULL);
    roc_l1_inac->SetDirectory(NULL);
    roc_l2_inac->SetDirectory(NULL);
    roc_l3_inac->SetDirectory(NULL);
    roc_fpixI_inac->SetDirectory(NULL);
    roc_fpixO_inac->SetDirectory(NULL);
    TH2D *mod_l1_eff = (TH2D*)fi.Get("mod_l1_eff");
    TH2D *mod_l2_eff = (TH2D*)fi.Get("mod_l2_eff");
    TH2D *mod_l3_eff = (TH2D*)fi.Get("mod_l3_eff");
    TH2D *mod_fpixI_eff = (TH2D*)fi.Get("mod_fpixI_eff");
    TH2D *mod_fpixO_eff = (TH2D*)fi.Get("mod_fpixO_eff");
    TH2D *mod_l1_dist = (TH2D*)fi.Get("mod_l1_dist");
    TH2D *mod_l2_dist = (TH2D*)fi.Get("mod_l2_dist");
    TH2D *mod_l3_dist = (TH2D*)fi.Get("mod_l3_dist");
    TH2D *mod_fpixI_dist = (TH2D*)fi.Get("mod_fpixI_dist");
    TH2D *mod_fpixO_dist = (TH2D*)fi.Get("mod_fpixO_dist");
    mod_l1_eff->SetDirectory(NULL);
    mod_l2_eff->SetDirectory(NULL);
    mod_l3_eff->SetDirectory(NULL);
    mod_fpixI_eff->SetDirectory(NULL);
    mod_fpixO_eff->SetDirectory(NULL);
    mod_l1_dist->SetDirectory(NULL);
    mod_l2_dist->SetDirectory(NULL);
    mod_l3_dist->SetDirectory(NULL);
    mod_fpixI_dist->SetDirectory(NULL);
    mod_fpixO_dist->SetDirectory(NULL);
    TH2D *pt_eff = (TH2D*)fi.Get("pt_eff");
    TH2D *pt_bpix_eff = (TH2D*)fi.Get("pt_bpix_eff");
    TH2D *pt_fpix_eff = (TH2D*)fi.Get("pt_fpix_eff");
    TH2D *pt_l1_eff = (TH2D*)fi.Get("pt_l1_eff");
    TH2D *pt_l2_eff = (TH2D*)fi.Get("pt_l2_eff");
    TH2D *pt_l3_eff = (TH2D*)fi.Get("pt_l3_eff");
    TH2D *pt_dm2_eff = (TH2D*)fi.Get("pt_dm2_eff");
    TH2D *pt_dm1_eff = (TH2D*)fi.Get("pt_dm1_eff");
    TH2D *pt_dp1_eff = (TH2D*)fi.Get("pt_dp1_eff");
    TH2D *pt_dp2_eff = (TH2D*)fi.Get("pt_dp2_eff");
    TH2D *pt_cumeff = (TH2D*)fi.Get("pt_cumeff");
    TH2D *pt_bpix_cumeff = (TH2D*)fi.Get("pt_bpix_cumeff");
    TH2D *pt_fpix_cumeff = (TH2D*)fi.Get("pt_fpix_cumeff");
    TH2D *pt_l1_cumeff = (TH2D*)fi.Get("pt_l1_cumeff");
    TH2D *pt_l2_cumeff = (TH2D*)fi.Get("pt_l2_cumeff");
    TH2D *pt_l3_cumeff = (TH2D*)fi.Get("pt_l3_cumeff");
    TH2D *pt_dm2_cumeff = (TH2D*)fi.Get("pt_dm2_cumeff");
    TH2D *pt_dm1_cumeff = (TH2D*)fi.Get("pt_dm1_cumeff");
    TH2D *pt_dp1_cumeff = (TH2D*)fi.Get("pt_dp1_cumeff");
    TH2D *pt_dp2_cumeff = (TH2D*)fi.Get("pt_dp2_cumeff");
    TH2D *pt_dist = (TH2D*)fi.Get("pt_dist");
    TH2D *pt_bpix_dist = (TH2D*)fi.Get("pt_bpix_dist");
    TH2D *pt_fpix_dist = (TH2D*)fi.Get("pt_fpix_dist");
    TH2D *pt_l1_dist = (TH2D*)fi.Get("pt_l1_dist");
    TH2D *pt_l2_dist = (TH2D*)fi.Get("pt_l2_dist");
    TH2D *pt_l3_dist = (TH2D*)fi.Get("pt_l3_dist");
    TH2D *pt_dm2_dist = (TH2D*)fi.Get("pt_dm2_dist");
    TH2D *pt_dm1_dist = (TH2D*)fi.Get("pt_dm1_dist");
    TH2D *pt_dp1_dist = (TH2D*)fi.Get("pt_dp1_dist");
    TH2D *pt_dp2_dist = (TH2D*)fi.Get("pt_dp2_dist");
    TH2D *pt_cumdist = (TH2D*)fi.Get("pt_cumdist");
    TH2D *pt_bpix_cumdist = (TH2D*)fi.Get("pt_bpix_cumdist");
    TH2D *pt_fpix_cumdist = (TH2D*)fi.Get("pt_fpix_cumdist");
    TH2D *pt_l1_cumdist = (TH2D*)fi.Get("pt_l1_cumdist");
    TH2D *pt_l2_cumdist = (TH2D*)fi.Get("pt_l2_cumdist");
    TH2D *pt_l3_cumdist = (TH2D*)fi.Get("pt_l3_cumdist");
    TH2D *pt_dm2_cumdist = (TH2D*)fi.Get("pt_dm2_cumdist");
    TH2D *pt_dm1_cumdist = (TH2D*)fi.Get("pt_dm1_cumdist");
    TH2D *pt_dp1_cumdist = (TH2D*)fi.Get("pt_dp1_cumdist");
    TH2D *pt_dp2_cumdist = (TH2D*)fi.Get("pt_dp2_cumdist");
    pt_eff->SetDirectory(NULL);
    pt_bpix_eff->SetDirectory(NULL);
    pt_fpix_eff->SetDirectory(NULL);
    pt_l1_eff->SetDirectory(NULL);
    pt_l2_eff->SetDirectory(NULL);
    pt_l3_eff->SetDirectory(NULL);
    pt_dm2_eff->SetDirectory(NULL);
    pt_dm1_eff->SetDirectory(NULL);
    pt_dp1_eff->SetDirectory(NULL);
    pt_dp2_eff->SetDirectory(NULL);
    pt_cumeff->SetDirectory(NULL);
    pt_bpix_cumeff->SetDirectory(NULL);
    pt_fpix_cumeff->SetDirectory(NULL);
    pt_l1_cumeff->SetDirectory(NULL);
    pt_l2_cumeff->SetDirectory(NULL);
    pt_l3_cumeff->SetDirectory(NULL);
    pt_dm2_cumeff->SetDirectory(NULL);
    pt_dm1_cumeff->SetDirectory(NULL);
    pt_dp1_cumeff->SetDirectory(NULL);
    pt_dp2_cumeff->SetDirectory(NULL);
    pt_dist->SetDirectory(NULL);
    pt_bpix_dist->SetDirectory(NULL);
    pt_fpix_dist->SetDirectory(NULL);
    pt_l1_dist->SetDirectory(NULL);
    pt_l2_dist->SetDirectory(NULL);
    pt_l3_dist->SetDirectory(NULL);
    pt_dm2_dist->SetDirectory(NULL);
    pt_dm1_dist->SetDirectory(NULL);
    pt_dp1_dist->SetDirectory(NULL);
    pt_dp2_dist->SetDirectory(NULL);
    pt_cumdist->SetDirectory(NULL);
    pt_bpix_cumdist->SetDirectory(NULL);
    pt_fpix_cumdist->SetDirectory(NULL);
    pt_l1_cumdist->SetDirectory(NULL);
    pt_l2_cumdist->SetDirectory(NULL);
    pt_l3_cumdist->SetDirectory(NULL);
    pt_dm2_cumdist->SetDirectory(NULL);
    pt_dm1_cumdist->SetDirectory(NULL);
    pt_dp1_cumdist->SetDirectory(NULL);
    pt_dp2_cumdist->SetDirectory(NULL);
    TH2D *fid_l1_full_eff = (TH2D*)fi.Get("fid_l1_full_eff");
    TH2D *fid_l1_full_lx_eff = (TH2D*)fi.Get("fid_l1_full_lx_eff");
    TH2D *fid_l1_full_ly_eff = (TH2D*)fi.Get("fid_l1_full_ly_eff");
    TH2D *fid_l1_half_eff = (TH2D*)fi.Get("fid_l1_half_eff");
    TH2D *fid_l1_half_lx_eff = (TH2D*)fi.Get("fid_l1_half_lx_eff");
    TH2D *fid_l1_half_ly_eff = (TH2D*)fi.Get("fid_l1_half_ly_eff");
    fid_l1_full_eff->SetDirectory(NULL);
    fid_l1_full_lx_eff->SetDirectory(NULL);
    fid_l1_full_ly_eff->SetDirectory(NULL);
    fid_l1_half_eff->SetDirectory(NULL);
    fid_l1_half_lx_eff->SetDirectory(NULL);
    fid_l1_half_ly_eff->SetDirectory(NULL);
    TH1D *delay_eff = (TH1D*)fi.Get("delay_eff");
    TH1D *delay_bpix_eff = (TH1D*)fi.Get("delay_bpix_eff");
    TH1D *delay_fpix_eff = (TH1D*)fi.Get("delay_fpix_eff");
    TH1D *delay_l1_eff = (TH1D*)fi.Get("delay_l1_eff");
    TH1D *delay_l2_eff = (TH1D*)fi.Get("delay_l2_eff");
    TH1D *delay_l3_eff = (TH1D*)fi.Get("delay_l3_eff");
    TH1D *delay_dm2_eff = (TH1D*)fi.Get("delay_dm2_eff");
    TH1D *delay_dm1_eff = (TH1D*)fi.Get("delay_dm1_eff");
    TH1D *delay_dp1_eff = (TH1D*)fi.Get("delay_dp1_eff");
    TH1D *delay_dp2_eff = (TH1D*)fi.Get("delay_dp2_eff");
    TH1D *delay_onclusize = (TH1D*)fi.Get("delay_onclusize");
    TH1D *delay_bpix_onclusize = (TH1D*)fi.Get("delay_bpix_onclusize");
    TH1D *delay_fpix_onclusize = (TH1D*)fi.Get("delay_fpix_onclusize");
    TH1D *delay_l1_onclusize = (TH1D*)fi.Get("delay_l1_onclusize");
    TH1D *delay_l2_onclusize = (TH1D*)fi.Get("delay_l2_onclusize");
    TH1D *delay_l3_onclusize = (TH1D*)fi.Get("delay_l3_onclusize");
    TH1D *delay_dm2_onclusize = (TH1D*)fi.Get("delay_dm2_onclusize");
    TH1D *delay_dm1_onclusize = (TH1D*)fi.Get("delay_dm1_onclusize");
    TH1D *delay_dp1_onclusize = (TH1D*)fi.Get("delay_dp1_onclusize");
    TH1D *delay_dp2_onclusize = (TH1D*)fi.Get("delay_dp2_onclusize");
    delay_eff->SetDirectory(NULL);
    delay_bpix_eff->SetDirectory(NULL);
    delay_fpix_eff->SetDirectory(NULL);
    delay_l1_eff->SetDirectory(NULL);
    delay_l2_eff->SetDirectory(NULL);
    delay_l3_eff->SetDirectory(NULL);
    delay_dm2_eff->SetDirectory(NULL);
    delay_dm1_eff->SetDirectory(NULL);
    delay_dp1_eff->SetDirectory(NULL);
    delay_dp2_eff->SetDirectory(NULL);
    delay_onclusize->SetDirectory(NULL);
    delay_bpix_onclusize->SetDirectory(NULL);
    delay_fpix_onclusize->SetDirectory(NULL);
    delay_l1_onclusize->SetDirectory(NULL);
    delay_l2_onclusize->SetDirectory(NULL);
    delay_l3_onclusize->SetDirectory(NULL);
    delay_dm2_onclusize->SetDirectory(NULL);
    delay_dm1_onclusize->SetDirectory(NULL);
    delay_dp1_onclusize->SetDirectory(NULL);
    delay_dp2_onclusize->SetDirectory(NULL);
    fi.Close();

    // N-1 plots




    gStyle->SetPadLeftMargin(0.12);
    gStyle->SetPadRightMargin(0.12);
    TCanvas pt_bpix_can("pt_bpix_can", "pt_bpix_can", 1200, 800);
    pt_bpix_can.SetFillColor(0);
    pt_bpix_can.Divide(2,2);
    pt_bpix_can.cd(1);
    setstyle_(pt_bpix_dist);
    pt_bpix_dist->GetXaxis()->SetTitle("Pt [GeV]");
    pt_bpix_dist->GetZaxis()->SetTitle("Distribution");
    pt_bpix_dist->GetYaxis()->SetTitleOffset(1.2);
    pt_bpix_dist->Draw();
    pt_bpix_can.cd(2);
    setstyle_(pt_bpix_eff);
    pt_bpix_eff->GetXaxis()->SetTitle("Pt [GeV]");
    pt_bpix_dist->GetZaxis()->SetTitle("Efficiency");
    pt_bpix_eff->GetYaxis()->SetTitleOffset(1.2);
    pt_bpix_eff->Draw();
    pt_bpix_can.cd(3);
    setstyle_(pt_bpix_cumdist);
    pt_bpix_cumdist->GetXaxis()->SetTitle("Pt lowcut [GeV]");
    pt_bpix_cumdist->GetYaxis()->SetTitle("Cumulative Distribution");
    pt_bpix_cumdist->GetYaxis()->SetTitleOffset(1.2);
    pt_bpix_cumdist->Draw();
    pt_bpix_can.cd(4);
    setstyle_(pt_bpix_cumeff);
    pt_bpix_cumeff->GetXaxis()->SetTitle("Pt lowcut [GeV]");
    pt_bpix_cumeff->GetYaxis()->SetTitle("Cumulative Efficiency");
    pt_bpix_cumeff->GetYaxis()->SetTitleOffset(1.2);
    pt_bpix_cumeff->Draw();
    TCanvas pt_fpix_can("pt_fpix_can", "pt_fpix_can", 1200, 800);
    pt_fpix_can.SetFillColor(0);
    pt_fpix_can.Divide(2,2);
    pt_fpix_can.cd(1);
    setstyle_(pt_fpix_dist);
    pt_fpix_dist->GetXaxis()->SetTitle("Pt [GeV]");
    pt_fpix_dist->GetZaxis()->SetTitle("Distribution");
    pt_fpix_dist->GetYaxis()->SetTitleOffset(1.2);
    pt_fpix_dist->Draw();
    pt_fpix_can.cd(2);
    setstyle_(pt_fpix_eff);
    pt_fpix_eff->GetXaxis()->SetTitle("Pt [GeV]");
    pt_fpix_dist->GetZaxis()->SetTitle("Efficiency");
    pt_fpix_eff->GetYaxis()->SetTitleOffset(1.2);
    pt_fpix_eff->Draw();
    pt_fpix_can.cd(3);
    setstyle_(pt_fpix_cumdist);
    pt_fpix_cumdist->GetXaxis()->SetTitle("Pt lowcut [GeV]");
    pt_fpix_cumdist->GetYaxis()->SetTitle("Cumulative Distribution");
    pt_fpix_cumdist->GetYaxis()->SetTitleOffset(1.2);
    pt_fpix_cumdist->Draw();
    pt_fpix_can.cd(4);
    setstyle_(pt_fpix_cumeff);
    pt_fpix_cumeff->GetXaxis()->SetTitle("Pt lowcut [GeV]");
    pt_fpix_cumeff->GetYaxis()->SetTitle("Cumulative Efficiency");
    pt_fpix_cumeff->GetYaxis()->SetTitleOffset(1.2);
    pt_fpix_cumeff->Draw();

    TCanvas fid_l1_full_can("fid_l1_full_can", "Layer1 full module fiducial region", 1200,800);
    fid_l1_full_can.SetFillColor(0);
    fid_l1_full_can.Divide(1,2);
    fid_l1_full_can.cd(1);
    setstyle_(fid_l1_full_eff);
    fid_l1_full_eff->GetXaxis()->SetTitle("local Y (cm)");
    fid_l1_full_eff->GetYaxis()->SetTitle("local X (cm)");
    fid_l1_full_eff->GetZaxis()->SetRangeUser(0.99,1.00);
    fid_l1_full_eff->Draw("CONTZ");
    TVirtualPad* fid_l1_full_can_2 = fid_l1_full_can.cd(2);
    fid_l1_full_can_2->Divide(2);
    fid_l1_full_can_2->cd(1);
    setstyle_(fid_l1_full_lx_eff);
    fid_l1_full_lx_eff->GetXaxis()->SetTitle("local X (cm)");
    fid_l1_full_lx_eff->GetYaxis()->SetTitle("Efficiency");
    fid_l1_full_lx_eff->Draw();
    fid_l1_full_can_2->cd(2);
    setstyle_(fid_l1_full_ly_eff);
    fid_l1_full_ly_eff->GetXaxis()->SetTitle("local Y (cm)");
    fid_l1_full_ly_eff->GetYaxis()->SetTitle("Efficiency");
    fid_l1_full_ly_eff->Draw();
    TCanvas fid_l1_half_can("fid_l1_half_can", "Layer1 half module fiducial region", 1200,800);
    fid_l1_half_can.SetFillColor(0);
    fid_l1_half_can.Divide(1,2);
    fid_l1_half_can.cd(1);
    setstyle_(fid_l1_half_eff);
    fid_l1_half_eff->GetXaxis()->SetTitle("local Y (cm)");
    fid_l1_half_eff->GetYaxis()->SetTitle("local X (cm)");
    fid_l1_half_eff->GetZaxis()->SetRangeUser(0.99,1.00);
    fid_l1_half_eff->Draw("CONTZ");
    TVirtualPad* fid_l1_half_can_2 = fid_l1_half_can.cd(2);
    fid_l1_half_can_2->Divide(2);
    fid_l1_half_can_2->cd(1);
    setstyle_(fid_l1_half_lx_eff);
    fid_l1_half_lx_eff->GetXaxis()->SetTitle("local X (cm)");
    fid_l1_half_lx_eff->GetYaxis()->SetTitle("Efficiency");
    fid_l1_half_lx_eff->Draw();
    fid_l1_half_can_2->cd(2);
    setstyle_(fid_l1_half_ly_eff);
    fid_l1_half_ly_eff->GetXaxis()->SetTitle("local Y (cm)");
    fid_l1_half_ly_eff->GetYaxis()->SetTitle("Efficiency");
    fid_l1_half_ly_eff->Draw();

    // Scans
/*     gStyle->SetPadLeftMargin(0.13); */
/*     gStyle->SetPadRightMargin(0.12); */
/*     gStyle->SetErrorX(0); */
/*     TCanvas delay_onclusize_can("delay_onclusize_can", "Average on-track cluster size vs Delay", 650,600); */
/*     delay_onclusize_can.SetFillColor(0); */
/*     delay_onclusize_can.SetTickx(1); */
/*     delay_onclusize_can.SetGridx(1); */
/*     delay_onclusize_can.SetGridy(1); */
/*     delay_onclusize->SetMarkerStyle(8); */
/*     delay_onclusize->SetMarkerSize(1.3); */
/*     delay_onclusize->GetXaxis()->SetTitle("Delay [ns]"); */
/*     delay_onclusize->GetXaxis()->SetLabelFont(42); */
/*     delay_onclusize->GetXaxis()->SetTitleFont(42); */
/*     delay_onclusize->GetXaxis()->SetTitleSize(0.05); */
/*     delay_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]"); */
/*     delay_onclusize->GetYaxis()->SetLabelFont(42); */
/*     delay_onclusize->GetYaxis()->SetTitleFont(42); */
/*     delay_onclusize->GetYaxis()->SetTitleSize(0.05); */
/*     delay_onclusize->GetYaxis()->SetTitleOffset(1.2); */
/*     delay_onclusize->GetXaxis()->SetRangeUser(-6.0,25.0); */
/*     // 41 -> 15 ns */
/*     Int_t clu_localmax = 22; */
/*     // 36 -> 10 ns */
/*     Int_t eff_localmax = 17; */
/*     Float_t leftmax = delay_onclusize->GetBinContent(clu_localmax); */
/*     Float_t rightmax = delay_eff->GetBinContent(eff_localmax); */
/*     Float_t min = 1.5; */
/*     Float_t max = 4.5; */
/*     delay_onclusize->GetYaxis()->SetRangeUser(min,max); */
/*     delay_onclusize->Draw("PX0"); */
/*     delay_eff->SetMarkerStyle(8); */
/*     delay_eff->SetMarkerSize(1.3); */
/*     delay_eff->SetMarkerColor(kRed); */
/*     Float_t scale = 1.005*leftmax/rightmax; */
/*     delay_eff->Scale(scale); */
/*     Float_t rescaleeff = 0.603; */
/*     Float_t submax = delay_eff->GetBinContent(eff_localmax); */
/*     for (Int_t i=1;i<51;i++) { */
/*       Float_t bin = delay_eff->GetBinContent(i); */
/*       Float_t rescaled = rescaleeff*bin-(rescaleeff-1)*submax; */
/*       if (bin!=0.0) */
/* 	delay_eff->SetBinContent(i,rescaled); */
/*     } */
/*     delay_eff->Draw("SAMEPX0"); */
/*     Float_t unscaled_right_min = min/scale; */
/*     Float_t unscaled_right_max = max/scale; */
/*     Float_t rescaled_right_max = rightmax + (unscaled_right_max - rightmax) / rescaleeff; */
/*     Float_t rescaled_right_min = rightmax - (rightmax - unscaled_right_min) / rescaleeff; */
/*     TGaxis *axis = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min,rescaled_right_max,510,"+L"); */
/*     axis->SetLineColor(kRed); */
/*     axis->SetLabelColor(kRed); */
/*     axis->SetTitle("Efficiency"); */
/*     axis->SetTitleColor(2); */
/*     axis->SetLabelFont(42); */
/*     axis->SetTitleFont(42); */
/*     axis->SetTitleSize(0.05); */
/*     axis->SetTitleOffset(1.2); */
/*     axis->Draw(); */
/*     TLatex delay_lat(4, 2.8, "Pixel Detector"); */
/*     delay_lat.Draw("same"); */


    gStyle->SetPadLeftMargin(0.13);
    gStyle->SetPadRightMargin(0.12);
    gStyle->SetErrorX(0);
    TCanvas delay_onclusize_can("delay_onclusize_can", "Average on-track cluster size vs Delay", 650,600);
    delay_onclusize_can.SetFillColor(0);
    delay_onclusize_can.SetTickx(1);
    delay_onclusize_can.SetGridx(1);
    delay_onclusize_can.SetGridy(1);
    setstyle_(delay_eff);
    setstyle_(delay_onclusize);
    delay_eff_onclusize_merge_(delay_eff, delay_onclusize, -1, 1.5, 4.5, 1.005, 0.603);
    TLatex delay_lat(5, 2.5, "Pixel Detector");
    delay_lat.Draw();

    TCanvas delay_bpix_onclusize_can("delay_bpix_onclusize_can", "Average on-track cluster size vs Delay", 650,600);
    delay_bpix_onclusize_can.SetFillColor(0);
    delay_bpix_onclusize_can.SetTickx(1);
    delay_bpix_onclusize_can.SetGridx(1);
    delay_bpix_onclusize_can.SetGridy(1);
    setstyle_(delay_bpix_eff);
    setstyle_(delay_bpix_onclusize);
    delay_eff_onclusize_merge_(delay_bpix_eff, delay_bpix_onclusize, -1, 2.0, 5.0, 1.005, 0.75);
    TLatex delay_bpix_lat(5, 3.0, "Barrel Pixel");
    delay_bpix_lat.Draw();

    TCanvas delay_fpix_onclusize_can("delay_fpix_onclusize_can", "Average on-track cluster size vs Delay", 650,600);
    delay_fpix_onclusize_can.SetFillColor(0);
    delay_fpix_onclusize_can.SetTickx(1);
    delay_fpix_onclusize_can.SetGridx(1);
    delay_fpix_onclusize_can.SetGridy(1);
    setstyle_(delay_fpix_eff);
    setstyle_(delay_fpix_onclusize);
    // 24 --> 16 ns
    delay_eff_onclusize_merge_(delay_fpix_eff, delay_fpix_onclusize, 24, 1.5, 2.15, 1.005, 0.75);
    TLatex delay_fpix_lat(5, 1.72, "Forward Pixel");
    delay_fpix_lat.Draw();
    

/*     gStyle->SetPadLeftMargin(0.13); */
/*     gStyle->SetPadRightMargin(0.12); */
/*     gStyle->SetErrorX(0); */
/*     TCanvas delay_bpix_onclusize_can("delay_bpix_onclusize_can", "Average on-track cluster size vs Delay - bpix", 650,600); */
/*     delay_bpix_onclusize_can.SetFillColor(0); */
/*     delay_bpix_onclusize_can.SetTickx(1); */
/*     delay_bpix_onclusize_can.SetGridx(1); */
/*     delay_bpix_onclusize_can.SetGridy(1); */
/*     delay_bpix_onclusize->SetMarkerStyle(8); */
/*     delay_bpix_onclusize->SetMarkerSize(1.3); */
/*     delay_bpix_onclusize->GetXaxis()->SetTitle("Delay [ns]"); */
/*     delay_bpix_onclusize->GetXaxis()->SetLabelFont(42); */
/*     delay_bpix_onclusize->GetXaxis()->SetTitleFont(42); */
/*     delay_bpix_onclusize->GetXaxis()->SetTitleSize(0.05); */
/*     delay_bpix_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]"); */
/*     delay_bpix_onclusize->GetYaxis()->SetLabelFont(42); */
/*     delay_bpix_onclusize->GetYaxis()->SetTitleFont(42); */
/*     delay_bpix_onclusize->GetYaxis()->SetTitleSize(0.05); */
/*     delay_bpix_onclusize->GetYaxis()->SetTitleOffset(1.2); */
/*     delay_bpix_onclusize->GetXaxis()->SetRangeUser(-6.0,25.0); */
/*     // 41 -> 15 ns */
/*     Int_t clu_localmax_bpix = 22; */
/*     Float_t leftmax_bpix = delay_bpix_onclusize->GetBinContent(clu_localmax_bpix); */
/*     Float_t rightmax_bpix = delay_bpix_eff->GetMaximum(); */
/*     Float_t min_bpix = 2.0; */
/*     Float_t max_bpix = 5.0; */
/*     delay_bpix_onclusize->GetYaxis()->SetRangeUser(min_bpix,max_bpix); */
/*     delay_bpix_onclusize->Draw("PX0"); */
/*     delay_bpix_eff->SetMarkerStyle(8); */
/*     delay_bpix_eff->SetMarkerSize(1.3); */
/*     delay_bpix_eff->SetMarkerColor(kRed); */
/*     Float_t scale_bpix = 1.005*leftmax_bpix/rightmax_bpix; */
/*     delay_bpix_eff->Scale(scale_bpix); */
/*     Float_t rescaleeff_bpix = 0.75; */
/*     Float_t submax_bpix = delay_bpix_eff->GetMaximum(); */
/*     for (Int_t i=1;i<51;i++) { */
/*       Float_t bin_bpix = delay_bpix_eff->GetBinContent(i); */
/*       Float_t rescaled_bpix = rescaleeff_bpix*bin_bpix-(rescaleeff_bpix-1)*submax_bpix; */
/*       delay_bpix_eff->SetBinContent(i,rescaled_bpix); */
/*      } */
/*     delay_bpix_eff->Draw("SAMEPX0"); */
/*     Float_t unscaled_right_min_bpix = min_bpix/scale_bpix; */
/*     Float_t unscaled_right_max_bpix = max_bpix/scale_bpix; */
/*     Float_t rescaled_right_max_bpix = rightmax_bpix + (unscaled_right_max_bpix - rightmax_bpix) / rescaleeff_bpix; */
/*     Float_t rescaled_right_min_bpix = rightmax_bpix - (rightmax_bpix - unscaled_right_min_bpix) / rescaleeff_bpix; */
/*     TGaxis *axis_bpix = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_bpix,rescaled_right_max_bpix,510,"+L"); */
/*     axis_bpix->SetLineColor(kRed); */
/*     axis_bpix->SetLabelColor(kRed); */
/*     axis_bpix->SetTitle("Efficiency"); */
/*     axis_bpix->SetTitleColor(2); */
/*     axis_bpix->SetLabelFont(42); */
/*     axis_bpix->SetTitleFont(42); */
/*     axis_bpix->SetTitleSize(0.05); */
/*     axis_bpix->SetTitleOffset(1.2); */
/*     axis_bpix->Draw(); */
/*     TLatex delay_bpix_lat(5, 3.2, "Barrel Pixel"); */
/*     delay_bpix_lat.Draw(); */
    
    
/*     TCanvas delay_fpix_onclusize_can("delay_fpix_onclusize_can", "Average on-track cluster size vs Delay - fpix", 650,600); */
/*     delay_fpix_onclusize_can.SetFillColor(0); */
/*     delay_fpix_onclusize_can.SetTickx(1); */
/*     delay_fpix_onclusize_can.SetGridx(1); */
/*     delay_fpix_onclusize_can.SetGridy(1); */
/*     delay_fpix_onclusize->SetMarkerStyle(8); */
/*     delay_fpix_onclusize->SetMarkerSize(1.3); */
/*     delay_fpix_onclusize->GetXaxis()->SetTitle("Delay [ns]"); */
/*     delay_fpix_onclusize->GetXaxis()->SetLabelFont(42); */
/*     delay_fpix_onclusize->GetXaxis()->SetTitleFont(42); */
/*     delay_fpix_onclusize->GetXaxis()->SetTitleSize(0.05); */
/*     delay_fpix_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]"); */
/*     delay_fpix_onclusize->GetYaxis()->SetLabelFont(42); */
/*     delay_fpix_onclusize->GetYaxis()->SetTitleFont(42); */
/*     delay_fpix_onclusize->GetYaxis()->SetTitleSize(0.05); */
/*     delay_fpix_onclusize->GetYaxis()->SetTitleOffset(1.2); */
/*     delay_fpix_onclusize->GetXaxis()->SetRangeUser(-6.0,24.0); */
/*     // 42 -> 16 ns */
/*     Int_t clu_localmax_fpix = 23; */
/*     Float_t leftmax_fpix = delay_fpix_onclusize->GetBinContent(clu_localmax_fpix); */
/*     Float_t rightmax_fpix = delay_fpix_eff->GetMaximum(); */
/*     Float_t min_fpix = 1.5; */
/*     Float_t max_fpix = 2.15; */
/*     delay_fpix_onclusize->GetYaxis()->SetRangeUser(min_fpix,max_fpix); */
/*     delay_fpix_onclusize->Draw("PX0"); */
/*     delay_fpix_eff->SetMarkerStyle(8); */
/*     delay_fpix_eff->SetMarkerSize(1.3); */
/*     delay_fpix_eff->SetMarkerColor(kRed); */
/*     Float_t scale_fpix = 1.005*leftmax_fpix/rightmax_fpix; */
/*     delay_fpix_eff->Scale(scale_fpix); */
/*     Float_t rescaleeff_fpix = 0.25; */
/*     Float_t submax_fpix = delay_fpix_eff->GetMaximum(); */
/*     for (Int_t i=1;i<51;i++) { */
/*       Float_t bin_fpix = delay_fpix_eff->GetBinContent(i); */
/*       Float_t rescaled_fpix = rescaleeff_fpix*bin_fpix-(rescaleeff_fpix-1)*submax_fpix; */
/*       delay_fpix_eff->SetBinContent(i,rescaled_fpix); */
/*      } */
/*     delay_fpix_eff->Draw("SAMEPX0"); */
/*     Float_t unscaled_right_min_fpix = min_fpix/scale_fpix; */
/*     Float_t unscaled_right_max_fpix = max_fpix/scale_fpix; */
/*     Float_t rescaled_right_max_fpix = rightmax_fpix + (unscaled_right_max_fpix - rightmax_fpix) / rescaleeff_fpix; */
/*     Float_t rescaled_right_min_fpix = rightmax_fpix - (rightmax_fpix - unscaled_right_min_fpix) / rescaleeff_fpix; */
/*     TGaxis *axis_fpix = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_fpix,rescaled_right_max_fpix,510,"+L"); */
/*     axis_fpix->SetLineColor(kRed); */
/*     axis_fpix->SetLabelColor(kRed); */
/*     axis_fpix->SetTitle("Efficiency"); */
/*     axis_fpix->SetTitleColor(2); */
/*     axis_fpix->SetLabelFont(42); */
/*     axis_fpix->SetTitleFont(42); */
/*     axis_fpix->SetTitleSize(0.05); */
/*     axis_fpix->SetTitleOffset(1.2); */
/*     axis_fpix->Draw(); */
/*     TLatex delay_fpix_lat(5, 1.72, "Forward Pixel"); */
/*     delay_fpix_lat.Draw(); */
    
    gStyle->SetPadRightMargin(0.15);
    TCanvas delay_l1_onclusize_can("delay_l1_onclusize_can", "Average on-track cluster size vs Delay - l1", 650,600);
    delay_l1_onclusize_can.SetFillColor(0);
    delay_l1_onclusize_can.SetTickx(1);
    delay_l1_onclusize_can.SetGridx(1);
    delay_l1_onclusize_can.SetGridy(1);
    delay_l1_onclusize->SetMarkerStyle(8);
    delay_l1_onclusize->SetMarkerSize(1.3);
    delay_l1_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_l1_onclusize->GetXaxis()->SetLabelFont(42);
    delay_l1_onclusize->GetXaxis()->SetTitleFont(42);
    delay_l1_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_l1_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_l1_onclusize->GetYaxis()->SetLabelFont(42);
    delay_l1_onclusize->GetYaxis()->SetTitleFont(42);
    delay_l1_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_l1_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_l1_onclusize->GetXaxis()->SetRangeUser(12,16);
    // 41 -> 15 ns
    Int_t clu_localmax_l1 = 22;
    // 39 -> 13 ns
    Int_t eff_localmax_l1 = 20;
    Float_t leftmax_l1 = delay_l1_onclusize->GetBinContent(clu_localmax_l1);
    Float_t rightmax_l1 = delay_l1_eff->GetBinContent(eff_localmax_l1);
    Float_t min_l1 = 5.05;
    Float_t max_l1 = 5.35;
    delay_l1_onclusize->GetYaxis()->SetRangeUser(min_l1,max_l1);
    delay_l1_onclusize->Draw("PX0");
    delay_l1_eff->SetMarkerStyle(8);
    delay_l1_eff->SetMarkerSize(1.3);
    delay_l1_eff->SetMarkerColor(kRed);
    Float_t scale_l1 = 1.003*leftmax_l1/rightmax_l1;
    delay_l1_eff->Scale(scale_l1);
    Float_t rescaleeff_l1 = 5.0;
    Float_t submax_l1 = delay_l1_eff->GetBinContent(eff_localmax_l1);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_l1 = delay_l1_eff->GetBinContent(i);
      Float_t rescaled_l1 = rescaleeff_l1*bin_l1-(rescaleeff_l1-1)*submax_l1;
      delay_l1_eff->SetBinContent(i,rescaled_l1);
     }
    delay_l1_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_l1 = min_l1/scale_l1;
    Float_t unscaled_right_max_l1 = max_l1/scale_l1;
    Float_t rescaled_right_max_l1 = rightmax_l1 + (unscaled_right_max_l1 - rightmax_l1) / rescaleeff_l1;
    Float_t rescaled_right_min_l1 = rightmax_l1 - (rightmax_l1 - unscaled_right_min_l1) / rescaleeff_l1;
    TGaxis *axis_l1 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_l1,rescaled_right_max_l1,510,"+L");
    axis_l1->SetLineColor(kRed);
    axis_l1->SetLabelColor(kRed);
    axis_l1->SetTitle("Efficiency");
    axis_l1->SetTitleColor(2);
    axis_l1->SetLabelFont(42);
    axis_l1->SetTitleFont(42);
    axis_l1->SetTitleSize(0.05);
    axis_l1->SetTitleOffset(1.6);
    axis_l1->Draw();
    TLatex delay_l1_lat(13.5, 5.15, "Layer 1");
    delay_l1_lat.Draw();
    
    TCanvas delay_l2_onclusize_can("delay_l2_onclusize_can", "Average on-track cluster size vs Delay - l2", 650,600);
    delay_l2_onclusize_can.SetFillColor(0);
    delay_l2_onclusize_can.SetTickx(1);
    delay_l2_onclusize_can.SetGridx(1);
    delay_l2_onclusize_can.SetGridy(1);
    delay_l2_onclusize->SetMarkerStyle(8);
    delay_l2_onclusize->SetMarkerSize(1.3);
    delay_l2_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_l2_onclusize->GetXaxis()->SetLabelFont(42);
    delay_l2_onclusize->GetXaxis()->SetTitleFont(42);
    delay_l2_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_l2_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_l2_onclusize->GetYaxis()->SetLabelFont(42);
    delay_l2_onclusize->GetYaxis()->SetTitleFont(42);
    delay_l2_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_l2_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_l2_onclusize->GetXaxis()->SetRangeUser(12,16);
    // 41 -> 15 ns
    Int_t clu_localmax_l2 = 22;
    // 39 -> 13 ns
    Int_t eff_localmax_l2 = 20;
    Float_t leftmax_l2 = delay_l2_onclusize->GetBinContent(clu_localmax_l2);
    Float_t rightmax_l2 = delay_l2_eff->GetBinContent(eff_localmax_l2);
    Float_t min_l2 = 4.1;
    Float_t max_l2 = 4.4;
    delay_l2_onclusize->GetYaxis()->SetRangeUser(min_l2,max_l2);
    delay_l2_onclusize->Draw("PX0");
    delay_l2_eff->SetMarkerStyle(8);
    delay_l2_eff->SetMarkerSize(1.3);
    delay_l2_eff->SetMarkerColor(kRed);
    Float_t scale_l2 = 1.003*leftmax_l2/rightmax_l2;
    delay_l2_eff->Scale(scale_l2);
    Float_t rescaleeff_l2 = 5.0;
    Float_t submax_l2 = delay_l2_eff->GetBinContent(eff_localmax_l2);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_l2 = delay_l2_eff->GetBinContent(i);
      Float_t rescaled_l2 = rescaleeff_l2*bin_l2-(rescaleeff_l2-1)*submax_l2;
      delay_l2_eff->SetBinContent(i,rescaled_l2);
     }
    delay_l2_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_l2 = min_l2/scale_l2;
    Float_t unscaled_right_max_l2 = max_l2/scale_l2;
    Float_t rescaled_right_max_l2 = rightmax_l2 + (unscaled_right_max_l2 - rightmax_l2) / rescaleeff_l2;
    Float_t rescaled_right_min_l2 = rightmax_l2 - (rightmax_l2 - unscaled_right_min_l2) / rescaleeff_l2;
    TGaxis *axis_l2 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_l2,rescaled_right_max_l2,510,"+L");
    axis_l2->SetLineColor(kRed);
    axis_l2->SetLabelColor(kRed);
    axis_l2->SetTitle("Efficiency");
    axis_l2->SetTitleColor(2);
    axis_l2->SetLabelFont(42);
    axis_l2->SetTitleFont(42);
    axis_l2->SetTitleSize(0.05);
    axis_l2->SetTitleOffset(1.6);
    axis_l2->Draw();
    TLatex delay_l2_lat(13.5, 4.2, "Layer 2");
    delay_l2_lat.Draw();
    
    TCanvas delay_l3_onclusize_can("delay_l3_onclusize_can", "Average on-track cluster size vs Delay - l3", 650,600);
    delay_l3_onclusize_can.SetFillColor(0);
    delay_l3_onclusize_can.SetTickx(1);
    delay_l3_onclusize_can.SetGridx(1);
    delay_l3_onclusize_can.SetGridy(1);
    delay_l3_onclusize->SetMarkerStyle(8);
    delay_l3_onclusize->SetMarkerSize(1.3);
    delay_l3_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_l3_onclusize->GetXaxis()->SetLabelFont(42);
    delay_l3_onclusize->GetXaxis()->SetTitleFont(42);
    delay_l3_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_l3_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_l3_onclusize->GetYaxis()->SetLabelFont(42);
    delay_l3_onclusize->GetYaxis()->SetTitleFont(42);
    delay_l3_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_l3_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_l3_onclusize->GetXaxis()->SetRangeUser(12,16);
    // 41 -> 15 ns
    Int_t clu_localmax_l3 = 22;
    // 38 -> 12 ns
    Int_t eff_localmax_l3 = 19;
    Float_t leftmax_l3 = delay_l3_onclusize->GetBinContent(clu_localmax_l3);
    Float_t rightmax_l3 = delay_l3_eff->GetBinContent(eff_localmax_l3);
    Float_t min_l3 = 3.35;
    Float_t max_l3 = 3.65;
    delay_l3_onclusize->GetYaxis()->SetRangeUser(min_l3,max_l3);
    delay_l3_onclusize->Draw("PX0");
    delay_l3_eff->SetMarkerStyle(8);
    delay_l3_eff->SetMarkerSize(1.3);
    delay_l3_eff->SetMarkerColor(kRed);
    Float_t scale_l3 = 1.003*leftmax_l3/rightmax_l3;
    delay_l3_eff->Scale(scale_l3);
    Float_t rescaleeff_l3 = 5.0;
    Float_t submax_l3 = delay_l3_eff->GetBinContent(eff_localmax_l3);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_l3 = delay_l3_eff->GetBinContent(i);
      Float_t rescaled_l3 = rescaleeff_l3*bin_l3-(rescaleeff_l3-1)*submax_l3;
      delay_l3_eff->SetBinContent(i,rescaled_l3);
     }
    delay_l3_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_l3 = min_l3/scale_l3;
    Float_t unscaled_right_max_l3 = max_l3/scale_l3;
    Float_t rescaled_right_max_l3 = rightmax_l3 + (unscaled_right_max_l3 - rightmax_l3) / rescaleeff_l3;
    Float_t rescaled_right_min_l3 = rightmax_l3 - (rightmax_l3 - unscaled_right_min_l3) / rescaleeff_l3;
    TGaxis *axis_l3 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_l3,rescaled_right_max_l3,510,"+L");
    axis_l3->SetLineColor(kRed);
    axis_l3->SetLabelColor(kRed);
    axis_l3->SetTitle("Efficiency");
    axis_l3->SetTitleColor(2);
    axis_l3->SetLabelFont(42);
    axis_l3->SetTitleFont(42);
    axis_l3->SetTitleSize(0.05);
    axis_l3->SetTitleOffset(1.6);
    axis_l3->Draw();
    TLatex delay_l3_lat(13.5, 3.45, "Layer 3");
    delay_l3_lat.Draw();
    
    TCanvas delay_dm2_onclusize_can("delay_dm2_onclusize_can", "Average on-track cluster size vs Delay - dm2", 650,600);
    delay_dm2_onclusize_can.SetFillColor(0);
    delay_dm2_onclusize_can.SetTickx(1);
    delay_dm2_onclusize_can.SetGridx(1);
    delay_dm2_onclusize_can.SetGridy(1);
    delay_dm2_onclusize->SetMarkerStyle(8);
    delay_dm2_onclusize->SetMarkerSize(1.3);
    delay_dm2_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_dm2_onclusize->GetXaxis()->SetLabelFont(42);
    delay_dm2_onclusize->GetXaxis()->SetTitleFont(42);
    delay_dm2_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_dm2_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_dm2_onclusize->GetYaxis()->SetLabelFont(42);
    delay_dm2_onclusize->GetYaxis()->SetTitleFont(42);
    delay_dm2_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_dm2_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_dm2_onclusize->GetXaxis()->SetRangeUser(12,18);
    // 42 -> 16 ns
    Int_t clu_localmax_dm2 = 23;
    // 38 -> 12 ns
    Int_t eff_localmax_dm2 = 19;
    Float_t leftmax_dm2 = delay_dm2_onclusize->GetBinContent(clu_localmax_dm2);
    Float_t rightmax_dm2 = delay_dm2_eff->GetBinContent(eff_localmax_dm2);
    Float_t min_dm2 = 1.85;
    Float_t max_dm2 = 1.97;
    delay_dm2_onclusize->GetYaxis()->SetRangeUser(min_dm2,max_dm2);
    delay_dm2_onclusize->Draw("PX0");
    delay_dm2_eff->SetMarkerStyle(8);
    delay_dm2_eff->SetMarkerSize(1.3);
    delay_dm2_eff->SetMarkerColor(kRed);
    Float_t scale_dm2 = 1.003*leftmax_dm2/rightmax_dm2;
    delay_dm2_eff->Scale(scale_dm2);
    Float_t rescaleeff_dm2 = 2.0;
    Float_t submax_dm2 = delay_dm2_eff->GetBinContent(eff_localmax_dm2);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_dm2 = delay_dm2_eff->GetBinContent(i);
      Float_t rescaled_dm2 = rescaleeff_dm2*bin_dm2-(rescaleeff_dm2-1)*submax_dm2;
      delay_dm2_eff->SetBinContent(i,rescaled_dm2);
     }
    delay_dm2_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_dm2 = min_dm2/scale_dm2;
    Float_t unscaled_right_max_dm2 = max_dm2/scale_dm2;
    Float_t rescaled_right_max_dm2 = rightmax_dm2 + (unscaled_right_max_dm2 - rightmax_dm2) / rescaleeff_dm2;
    Float_t rescaled_right_min_dm2 = rightmax_dm2 - (rightmax_dm2 - unscaled_right_min_dm2) / rescaleeff_dm2;
    TGaxis *axis_dm2 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_dm2,rescaled_right_max_dm2,510,"+L");
    axis_dm2->SetLineColor(kRed);
    axis_dm2->SetLabelColor(kRed);
    axis_dm2->SetTitle("Efficiency");
    axis_dm2->SetTitleColor(2);
    axis_dm2->SetLabelFont(42);
    axis_dm2->SetTitleFont(42);
    axis_dm2->SetTitleSize(0.05);
    axis_dm2->SetTitleOffset(1.6);
    axis_dm2->Draw();
    TLatex delay_dm2_lat(14.5, 1.89, "Disk -2");
    delay_dm2_lat.Draw();
    
    TCanvas delay_dm1_onclusize_can("delay_dm1_onclusize_can", "Average on-track cluster size vs Delay - dm1", 650,600);
    delay_dm1_onclusize_can.SetFillColor(0);
    delay_dm1_onclusize_can.SetTickx(1);
    delay_dm1_onclusize_can.SetGridx(1);
    delay_dm1_onclusize_can.SetGridy(1);
    delay_dm1_onclusize->SetMarkerStyle(8);
    delay_dm1_onclusize->SetMarkerSize(1.3);
    delay_dm1_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_dm1_onclusize->GetXaxis()->SetLabelFont(42);
    delay_dm1_onclusize->GetXaxis()->SetTitleFont(42);
    delay_dm1_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_dm1_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_dm1_onclusize->GetYaxis()->SetLabelFont(42);
    delay_dm1_onclusize->GetYaxis()->SetTitleFont(42);
    delay_dm1_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_dm1_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_dm1_onclusize->GetXaxis()->SetRangeUser(12,18);
    // 42 -> 16 ns
    Int_t clu_localmax_dm1 = 23;
    // 38 -> 12 ns
    Int_t eff_localmax_dm1 = 19;
    Float_t leftmax_dm1 = delay_dm1_onclusize->GetBinContent(clu_localmax_dm1);
    Float_t rightmax_dm1 = delay_dm1_eff->GetBinContent(eff_localmax_dm1);
    Float_t min_dm1 = 1.93;
    Float_t max_dm1 = 2.05;
    delay_dm1_onclusize->GetYaxis()->SetRangeUser(min_dm1,max_dm1);
    delay_dm1_onclusize->Draw("PX0");
    delay_dm1_eff->SetMarkerStyle(8);
    delay_dm1_eff->SetMarkerSize(1.3);
    delay_dm1_eff->SetMarkerColor(kRed);
    Float_t scale_dm1 = 1.003*leftmax_dm1/rightmax_dm1;
    delay_dm1_eff->Scale(scale_dm1);
    Float_t rescaleeff_dm1 = 2.0;
    Float_t submax_dm1 = delay_dm1_eff->GetBinContent(eff_localmax_dm1);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_dm1 = delay_dm1_eff->GetBinContent(i);
      Float_t rescaled_dm1 = rescaleeff_dm1*bin_dm1-(rescaleeff_dm1-1)*submax_dm1;
      delay_dm1_eff->SetBinContent(i,rescaled_dm1);
     }
    delay_dm1_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_dm1 = min_dm1/scale_dm1;
    Float_t unscaled_right_max_dm1 = max_dm1/scale_dm1;
    Float_t rescaled_right_max_dm1 = rightmax_dm1 + (unscaled_right_max_dm1 - rightmax_dm1) / rescaleeff_dm1;
    Float_t rescaled_right_min_dm1 = rightmax_dm1 - (rightmax_dm1 - unscaled_right_min_dm1) / rescaleeff_dm1;
    TGaxis *axis_dm1 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_dm1,rescaled_right_max_dm1,510,"+L");
    axis_dm1->SetLineColor(kRed);
    axis_dm1->SetLabelColor(kRed);
    axis_dm1->SetTitle("Efficiency");
    axis_dm1->SetTitleColor(2);
    axis_dm1->SetLabelFont(42);
    axis_dm1->SetTitleFont(42);
    axis_dm1->SetTitleSize(0.05);
    axis_dm1->SetTitleOffset(1.6);
    axis_dm1->Draw();
    TLatex delay_dm1_lat(14.5, 1.97, "Disk -1");
    delay_dm1_lat.Draw();
    
    TCanvas delay_dp1_onclusize_can("delay_dp1_onclusize_can", "Average on-track cluster size vs Delay - dp1", 650,600);
    delay_dp1_onclusize_can.SetFillColor(0);
    delay_dp1_onclusize_can.SetTickx(1);
    delay_dp1_onclusize_can.SetGridx(1);
    delay_dp1_onclusize_can.SetGridy(1);
    delay_dp1_onclusize->SetMarkerStyle(8);
    delay_dp1_onclusize->SetMarkerSize(1.3);
    delay_dp1_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_dp1_onclusize->GetXaxis()->SetLabelFont(42);
    delay_dp1_onclusize->GetXaxis()->SetTitleFont(42);
    delay_dp1_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_dp1_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_dp1_onclusize->GetYaxis()->SetLabelFont(42);
    delay_dp1_onclusize->GetYaxis()->SetTitleFont(42);
    delay_dp1_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_dp1_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_dp1_onclusize->GetXaxis()->SetRangeUser(12,18);
    // 42 -> 16 ns
    Int_t clu_localmax_dp1 = 23;
    // 38 -> 12 ns
    Int_t eff_localmax_dp1 = 19;
    Float_t leftmax_dp1 = delay_dp1_onclusize->GetBinContent(clu_localmax_dp1);
    Float_t rightmax_dp1 = delay_dp1_eff->GetBinContent(eff_localmax_dp1);
    Float_t min_dp1 = 1.93;
    Float_t max_dp1 = 2.05;
    delay_dp1_onclusize->GetYaxis()->SetRangeUser(min_dp1,max_dp1);
    delay_dp1_onclusize->Draw("PX0");
    delay_dp1_eff->SetMarkerStyle(8);
    delay_dp1_eff->SetMarkerSize(1.3);
    delay_dp1_eff->SetMarkerColor(kRed);
    Float_t scale_dp1 = 1.003*leftmax_dp1/rightmax_dp1;
    delay_dp1_eff->Scale(scale_dp1);
    Float_t rescaleeff_dp1 = 2.0;
    Float_t submax_dp1 = delay_dp1_eff->GetBinContent(eff_localmax_dp1);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_dp1 = delay_dp1_eff->GetBinContent(i);
      Float_t rescaled_dp1 = rescaleeff_dp1*bin_dp1-(rescaleeff_dp1-1)*submax_dp1;
      delay_dp1_eff->SetBinContent(i,rescaled_dp1);
     }
    delay_dp1_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_dp1 = min_dp1/scale_dp1;
    Float_t unscaled_right_max_dp1 = max_dp1/scale_dp1;
    Float_t rescaled_right_max_dp1 = rightmax_dp1 + (unscaled_right_max_dp1 - rightmax_dp1) / rescaleeff_dp1;
    Float_t rescaled_right_min_dp1 = rightmax_dp1 - (rightmax_dp1 - unscaled_right_min_dp1) / rescaleeff_dp1;
    TGaxis *axis_dp1 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_dp1,rescaled_right_max_dp1,510,"+L");
    axis_dp1->SetLineColor(kRed);
    axis_dp1->SetLabelColor(kRed);
    axis_dp1->SetTitle("Efficiency");
    axis_dp1->SetTitleColor(2);
    axis_dp1->SetLabelFont(42);
    axis_dp1->SetTitleFont(42);
    axis_dp1->SetTitleSize(0.05);
    axis_dp1->SetTitleOffset(1.6);
    axis_dp1->Draw();
    TLatex delay_dp1_lat(14.5, 1.97, "Disk +1");
    delay_dp1_lat.Draw();
    
    TCanvas delay_dp2_onclusize_can("delay_dp2_onclusize_can", "Average on-track cluster size vs Delay - dp2", 650,600);
    delay_dp2_onclusize_can.SetFillColor(0);
    delay_dp2_onclusize_can.SetTickx(1);
    delay_dp2_onclusize_can.SetGridx(1);
    delay_dp2_onclusize_can.SetGridy(1);
    delay_dp2_onclusize->SetMarkerStyle(8);
    delay_dp2_onclusize->SetMarkerSize(1.3);
    delay_dp2_onclusize->GetXaxis()->SetTitle("Delay [ns]");
    delay_dp2_onclusize->GetXaxis()->SetLabelFont(42);
    delay_dp2_onclusize->GetXaxis()->SetTitleFont(42);
    delay_dp2_onclusize->GetXaxis()->SetTitleSize(0.05);
    delay_dp2_onclusize->GetYaxis()->SetTitle("Average on-track cluster size [pixel]");
    delay_dp2_onclusize->GetYaxis()->SetLabelFont(42);
    delay_dp2_onclusize->GetYaxis()->SetTitleFont(42);
    delay_dp2_onclusize->GetYaxis()->SetTitleSize(0.05);
    delay_dp2_onclusize->GetYaxis()->SetTitleOffset(1.2);
    delay_dp2_onclusize->GetXaxis()->SetRangeUser(12,18);
    // 42 -> 16 ns
    Int_t clu_localmax_dp2 = 23;
    // 38 -> 12 ns
    Int_t eff_localmax_dp2 = 19;
    Float_t leftmax_dp2 = delay_dp2_onclusize->GetBinContent(clu_localmax_dp2);
    Float_t rightmax_dp2 = delay_dp2_eff->GetBinContent(eff_localmax_dp2);
    Float_t min_dp2 = 1.85;
    Float_t max_dp2 = 1.97;
    delay_dp2_onclusize->GetYaxis()->SetRangeUser(min_dp2,max_dp2);
    delay_dp2_onclusize->Draw("PX0");
    delay_dp2_eff->SetMarkerStyle(8);
    delay_dp2_eff->SetMarkerSize(1.3);
    delay_dp2_eff->SetMarkerColor(kRed);
    Float_t scale_dp2 = 1.003*leftmax_dp2/rightmax_dp2;
    delay_dp2_eff->Scale(scale_dp2);
    Float_t rescaleeff_dp2 = 2.0;
    Float_t submax_dp2 = delay_dp2_eff->GetBinContent(eff_localmax_dp2);
    for (Int_t i=1;i<51;i++) {
      Float_t bin_dp2 = delay_dp2_eff->GetBinContent(i);
      Float_t rescaled_dp2 = rescaleeff_dp2*bin_dp2-(rescaleeff_dp2-1)*submax_dp2;
      delay_dp2_eff->SetBinContent(i,rescaled_dp2);
     }
    delay_dp2_eff->Draw("SAMEPX0");
    Float_t unscaled_right_min_dp2 = min_dp2/scale_dp2;
    Float_t unscaled_right_max_dp2 = max_dp2/scale_dp2;
    Float_t rescaled_right_max_dp2 = rightmax_dp2 + (unscaled_right_max_dp2 - rightmax_dp2) / rescaleeff_dp2;
    Float_t rescaled_right_min_dp2 = rightmax_dp2 - (rightmax_dp2 - unscaled_right_min_dp2) / rescaleeff_dp2;
    TGaxis *axis_dp2 = new TGaxis(gPad->GetUxmax(),gPad->GetUymin(),gPad->GetUxmax(),gPad->GetUymax(),rescaled_right_min_dp2,rescaled_right_max_dp2,510,"+L");
    axis_dp2->SetLineColor(kRed);
    axis_dp2->SetLabelColor(kRed);
    axis_dp2->SetTitle("Efficiency");
    axis_dp2->SetTitleColor(2);
    axis_dp2->SetLabelFont(42);
    axis_dp2->SetTitleFont(42);
    axis_dp2->SetTitleSize(0.05);
    axis_dp2->SetTitleOffset(1.6);
    axis_dp2->Draw();
    TLatex delay_dp2_lat(14.5, 1.89, "Disk +2");
    delay_dp2_lat.Draw();

    TFile fo(filewrite_,"update");
/*     roc_l1_can.Write(); */
/*     roc_l2_can.Write(); */
/*     roc_l3_can.Write(); */
/*     roc_fpixI_can.Write(); */
/*     roc_fpixO_can.Write(); */
    pt_bpix_can.Write();
    pt_fpix_can.Write();
    fid_l1_full_can.Write();
    fid_l1_half_can.Write();
    delay_onclusize_can.Write();
    delay_bpix_onclusize_can.Write();
    delay_fpix_onclusize_can.Write();
    delay_l1_onclusize_can.Write();
    delay_l2_onclusize_can.Write();
    delay_l3_onclusize_can.Write();
    delay_dm2_onclusize_can.Write();
    delay_dm1_onclusize_can.Write();
    delay_dp1_onclusize_can.Write();
    delay_dp2_onclusize_can.Write();
    fo.Close();
  }

};
