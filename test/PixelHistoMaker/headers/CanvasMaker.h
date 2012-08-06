#include "TStyle.h"
#include "TLine.h"
#include "TLatex.h"
#include "TGaxis.h"
#include "TVirtualPad.h"
#include "TColor.h"
#include "TF1.h"
#include "TPaletteAxis.h"

class Functions {

 public:

  Functions() {}
  ~Functions() {}
  
  Double_t hvTurnon(Double_t *x, Double_t *par) {
    Double_t arg = 0;
    if (par[1] != 0) arg = (par[0]-x[0])/par[1];
    return 1.0/(1.0+exp(arg));
  }

  //TF1* f = new TF1("turnon", "1/(1+exp(([0]-x)/[1]))", 15, 155);

};

class CanvasMaker {
  
 public: 
   
  CanvasMaker(int create_noteff, int create_eff, int create_scans) {
    create_noteff_ = create_noteff;
    create_eff_ = create_eff;
    create_scans_ = create_scans;
  }
  ~CanvasMaker() { }

 private:

  int create_noteff_;
  int create_eff_;
  int create_scans_;

  PostFixes postfix_;

  std::string year_;
  std::string date_;

  // color schemes
  std::vector<Int_t> col12_;
  std::vector<Int_t> col4_;
  std::vector<Int_t> col3x4_;
  std::vector<Int_t> col3x3p2_;
  std::vector<Int_t> col_;
  std::vector<Int_t> defcol_;
  std::vector<Int_t> defcol2_;
  std::vector<Int_t> defcol3_;
  std::vector<Int_t> run_col1_;
  std::vector<Int_t> run_col2_;
  std::vector<Int_t> hot_col_;

  std::vector<Int_t> marker_;

  void color_schemes_() {
    col12_.push_back(1);
    col12_.push_back(4);
    col12_.push_back(6);
    col12_.push_back(2);
    col12_.push_back(TColor::GetColor("#ffcc00"));
    col12_.push_back(TColor::GetColor("#cccc00"));
    col12_.push_back(TColor::GetColor("#00cc00"));
    col12_.push_back(TColor::GetColor("#00cccc"));
    col12_.push_back(9);
    col12_.push_back(TColor::GetColor("#990099"));
    col12_.push_back(TColor::GetColor("#cc0000"));
    col12_.push_back(TColor::GetColor("#be7d00"));

    col4_.push_back(3);
    col4_.push_back(4);
    col4_.push_back(6);
    col4_.push_back(2);

    col3x4_.push_back(TColor::GetColor("#ff8f8f"));
    col3x4_.push_back(TColor::GetColor("#fe4951"));
    col3x4_.push_back(TColor::GetColor("#de000a"));
    col3x4_.push_back(TColor::GetColor("#6c0000"));
    col3x4_.push_back(TColor::GetColor("#a3a3ff"));
    col3x4_.push_back(TColor::GetColor("#6666ff"));
    col3x4_.push_back(TColor::GetColor("#0000d6"));
    col3x4_.push_back(TColor::GetColor("#000082"));
    col3x4_.push_back(TColor::GetColor("#66ffcc"));
    col3x4_.push_back(TColor::GetColor("#00cc00"));
    col3x4_.push_back(TColor::GetColor("#339933"));
    col3x4_.push_back(TColor::GetColor("#336600"));
    
    col3x3p2_.push_back(TColor::GetColor("#ff8f8f"));
    col3x3p2_.push_back(TColor::GetColor("#fe1919"));
    col3x3p2_.push_back(TColor::GetColor("#6c0000"));
    col3x3p2_.push_back(TColor::GetColor("#ff6633"));
    col3x3p2_.push_back(TColor::GetColor("#ff9900"));
    col3x3p2_.push_back(TColor::GetColor("#a3a3ff"));
    col3x3p2_.push_back(TColor::GetColor("#2b2bff"));
    col3x3p2_.push_back(TColor::GetColor("#000082"));
    col3x3p2_.push_back(TColor::GetColor("#ff66ff"));
    col3x3p2_.push_back(TColor::GetColor("#990099"));
    col3x3p2_.push_back(TColor::GetColor("#66ffcc"));
    col3x3p2_.push_back(TColor::GetColor("#00cc00"));
    col3x3p2_.push_back(TColor::GetColor("#336600"));
    col3x3p2_.push_back(7);
    col3x3p2_.push_back(TColor::GetColor("#009999"));

    marker_.push_back(20); // full circle
    marker_.push_back(21); // full square
    marker_.push_back(22); // full triangle up
    marker_.push_back(23); // full triangle down
    marker_.push_back(24); // open circle
    marker_.push_back(25); // open square
    marker_.push_back(26); // open triangle up
    marker_.push_back(28); // open cross

    col_.push_back(1); // black
    col_.push_back(2); // red
    col_.push_back(4); // blue
    col_.push_back(3); // green
    col_.push_back(1); // black
    col_.push_back(2); // red
    col_.push_back(4); // blue
    col_.push_back(3); // green

    defcol_.push_back(1); // black
    defcol_.push_back(2); // red
    defcol_.push_back(3); // green
    defcol_.push_back(6); // purple
    defcol_.push_back(TColor::GetColor("#ff9900")); // Orange
    defcol_.push_back(4); // blue
    defcol_.push_back(7); // cyan
    defcol_.push_back(28); // brown

    defcol2_.push_back(2); // red
    defcol2_.push_back(3); // green
    defcol2_.push_back(6); // purple
    defcol2_.push_back(TColor::GetColor("#ff9900")); // Orange
    defcol2_.push_back(4); // blue
    defcol2_.push_back(7); // cyan
    defcol2_.push_back(28); // brown

    defcol3_.push_back(1); // black
    defcol3_.push_back(2); // red
    defcol3_.push_back(4); // blue
    defcol3_.push_back(3); // green
    defcol3_.push_back(TColor::GetColor("#ff9900")); // Orange
    defcol3_.push_back(6); // purple
    defcol3_.push_back(28); // brown
    defcol3_.push_back(7); // cyan

    run_col1_.push_back(4); // blue
    run_col1_.push_back(6); // purple
    run_col1_.push_back(2); // red

    hot_col_.push_back(2); // red
    hot_col_.push_back(6); // purple
    hot_col_.push_back(4); // blue

    run_col2_.push_back(6);
    run_col2_.push_back(4);
    run_col2_.push_back(2);
    run_col2_.push_back(8);
  }

  void setstyle_(TH1D* h, std::string xtitle="", std::string ytitle="", 
		 Float_t xoffset=0.9, Float_t yoffset=1.4, 
		 Style_t fontstyle=42, Float_t titlesize=0.06, Float_t labelsize =0.04, Bool_t dec=kTRUE) {
    h->GetXaxis()->SetTitle(xtitle.c_str());
    h->GetXaxis()->SetTitleOffset(xoffset);
    h->GetXaxis()->SetLabelFont(fontstyle);
    h->GetXaxis()->SetLabelSize(labelsize);
    h->GetXaxis()->SetTitleFont(fontstyle);
    h->GetXaxis()->SetTitleSize(titlesize);
    h->GetYaxis()->SetTitle(ytitle.c_str());
    h->GetYaxis()->SetTitleOffset(yoffset);
    h->GetYaxis()->SetLabelFont(fontstyle);
    h->GetYaxis()->SetLabelSize(labelsize);
    h->GetYaxis()->SetTitleFont(fontstyle);
    h->GetYaxis()->SetTitleSize(titlesize);
    h->GetYaxis()->SetDecimals(dec);
  }

  void setstyle_(TH2D* h, std::string xtitle="", std::string ytitle="", std::string ztitle="", 
		 Float_t xoffset=0.9, Float_t yoffset=1.4, Float_t zoffset=1.0,
		 Style_t fontstyle=42, Float_t titlesize=0.06, Bool_t dec=kTRUE) {
    h->GetXaxis()->SetTitle(xtitle.c_str());
    h->GetXaxis()->SetTitleOffset(xoffset);
    h->GetXaxis()->SetLabelFont(fontstyle);
    h->GetXaxis()->SetTitleFont(fontstyle);
    h->GetXaxis()->SetTitleSize(titlesize);
    h->GetYaxis()->SetTitle(ytitle.c_str());
    h->GetYaxis()->SetTitleOffset(yoffset);
    h->GetYaxis()->SetLabelFont(fontstyle);
    h->GetYaxis()->SetTitleFont(fontstyle);
    h->GetYaxis()->SetTitleSize(titlesize);
    h->GetYaxis()->SetDecimals(dec);
    h->GetZaxis()->SetTitle(ztitle.c_str());
    h->GetZaxis()->SetTitleOffset(zoffset);
    h->GetZaxis()->SetLabelFont(fontstyle);
    h->GetZaxis()->SetTitleFont(fontstyle);
    h->GetZaxis()->SetTitleSize(titlesize);
  }

  std::string title_(Histograms* h, int i) {
    std::string title="";
    if (h->eff()==1) {
      if (i==0) title = "Efficiency";
      else if (i==1) title = "Arbitrary"; }
    if (h->cumul()!=0) {
      if (i==2) title = "Cumulative Efficiency";
      else if (i==3) title = "Cumulative Hit Distribution"; }
    if (h->clusize()!=0) {
      if (i==2) title = "Average On-Track Cluster Size";
      else if (i==3) title = "Number of Clusters"; }
    if (h->def()!=0) {
      // if (i==2) title = "Number of clusters";
      if (i==2) title = "MPV of Normalized Cluster Charge"; 
      else if (i==3) title = "Average Cluster Charge";
      else if (i==4) title = "Average Normalized Cluster Charge";
      else if (i==5) title = "MPV of Normalized Cluster Charge";
      else if (i==6) title = "Average |Alpha|";
      else if (i==7) title = "Average |Beta|";
      else if (i==8) title = "Average Angle of Incidence"; 
      else if (i==9) title = "Average Cluster Size along r#phi";
      else if (i==10) title = "Number of Clusters per event";
      else if (i==11) title = "Number of Pixels per event";
    }
    return title;
  }
  
  // 1D dress
  void dress_all_(Histograms* h, std::string xtitle, float xoffset, float yoffset) {
    if (h->dimensions()==1) {
      if (h->npostfix()==1) for (size_t i=0; i<h->h1d_1p().size(); i++) 
	setstyle_(h->h1d(i), xtitle, title_(h,i), xoffset, yoffset);
      else if (h->npostfix()==2) for (size_t i=0; i<h->h1d_2p().size(); i++) 
	for (size_t j=0; j<h->h1d_2p()[i].size(); j++)
	  setstyle_(h->h1d(i,j), xtitle, title_(h,j), xoffset, yoffset);
      else if (h->npostfix()==3) for (size_t i=0; i<h->h1d_3p().size(); i++) 
	for (size_t j=0; j<h->h1d_3p()[i].size(); j++) for (size_t k=0; k<h->h1d_3p()[i][j].size(); k++)
	  setstyle_(h->h1d(i,j,k), xtitle, title_(h,k), xoffset, yoffset);
      else if (h->npostfix()==4) for (size_t i=0; i<h->h1d_4p().size(); i++) for (size_t j=0; j<h->h1d_4p()[i].size(); j++) 
	for (size_t k=0; k<h->h1d_4p()[i][j].size(); k++) for (size_t l=0; l<h->h1d_4p()[i][j][k].size(); l++)
	  setstyle_(h->h1d(i,j,k,l), xtitle, title_(h,l), xoffset, yoffset);
    }
  }

  // 2D
  void dress_all_(Histograms* h, std::string xtitle, std::string ytitle, float xoffset, float yoffset, float zoffset) {
    if (h->dimensions()==2) {
      if (h->npostfix()==1) for (size_t i=0; i<h->h2d_1p().size(); i++) 
	setstyle_(h->h2d(i), xtitle, ytitle, title_(h,i), xoffset, yoffset, zoffset);
      else if (h->npostfix()==2) for (size_t i=0; i<h->h2d_2p().size(); i++) 
	for (size_t j=0; j<h->h2d_2p()[i].size(); j++)
	  setstyle_(h->h2d(i,j), xtitle, ytitle, title_(h,j), xoffset, yoffset, zoffset);
      else if (h->npostfix()==3) for (size_t i=0; i<h->h2d_3p().size(); i++) 
	for (size_t j=0; j<h->h2d_3p()[i].size(); j++) for (size_t k=0; k<h->h2d_3p()[i][j].size(); k++)
	  setstyle_(h->h2d(i,j,k), xtitle, ytitle, title_(h,k), xoffset, yoffset, zoffset);
      else if (h->npostfix()==4) for (size_t i=0; i<h->h2d_4p().size(); i++) for (size_t j=0; j<h->h2d_4p()[i].size(); j++) 
	for (size_t k=0; k<h->h2d_4p()[i][j].size(); k++) for (size_t l=0; l<h->h2d_4p()[i][j][k].size(); l++)
	  setstyle_(h->h2d(i,j,k,l), xtitle, ytitle, title_(h,l), xoffset, yoffset, zoffset);
    }
  }
  
  TCanvas* custom_can_(TH1D* h, std::string canname, std::string title,
		       std::string xtitle, std::string ytitle, int gx = 0, int gy = 0, 
		       int histosize_x = 480, int histosize_y = 480, int mar_left = 80, int mar_right = 20) {
    int mar_top = 20 + (title.size()>0)*20;
    int mar_bottom = 60;
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
  
  TCanvas* custom_can_(TH2D* h, std::string canname, std::string title,
		       std::string xtitle, std::string ytitle, std::string ztitle, int gx = 0, int gy = 0, 
		       int histosize_x = 480, int histosize_y = 480, int mar_left = 80, int mar_right = 120) {
    int mar_top = 20 + (title.size()>0)*20;
    int mar_bottom = 60;
    int titlefontsize = 32;
    int labelfontsize = 20;
    int pal_offset_x = 5;
    int pal_width_x = 25;
    int xoffset_y = mar_bottom - titlefontsize - 4;
    int yoffset_x = mar_left - titlefontsize - 4;
    int zoffset_x = mar_right - pal_offset_x - pal_width_x - titlefontsize;
    int padsize_x = histosize_x + mar_left + mar_right;
    int padsize_y = histosize_y + mar_top + mar_bottom;
    int padsize = ((padsize_x<=padsize_y) ? padsize_x : padsize_y);
    float padratio_yx = (Float_t)padsize_y/padsize_x > 1 ? 1 : (Float_t)padsize_y/padsize_x;
    float padratio_xy = (Float_t)padsize_x/padsize_y > 1 ? 1 : (Float_t)padsize_x/padsize_y;
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
    h->GetZaxis()->SetTitle(ztitle.c_str());
    h->GetXaxis()->SetTitleOffset(xoffset);
    h->GetYaxis()->SetTitleOffset(yoffset);
    h->GetZaxis()->SetTitleOffset(zoffset);
    h->GetZaxis()->RotateTitle(1);
    h->GetYaxis()->SetDecimals(1);
    h->GetZaxis()->SetDecimals(1);
    if (histosize_y<250) h->GetZaxis()->SetNdivisions(505);
    gStyle->SetPadLeftMargin((Float_t)mar_left/padsize_x);
    gStyle->SetPadRightMargin((Float_t)mar_right/padsize_x);
    gStyle->SetPadTopMargin((Float_t)mar_top/padsize_y);
    gStyle->SetPadBottomMargin((Float_t)mar_bottom/padsize_y);
    gStyle->SetOptTitle(1);
    gStyle->SetTitleH(titlefontsize/padsize);
    gStyle->SetTitleFontSize(titlesize);
    TCanvas* canvas = new TCanvas(canname.c_str(), title.c_str(), padsize_x + 4, padsize_y + 26);
    canvas->SetGrid(gx,gy);
    h->Draw("COLZ");
    if (h->GetEntries()>0) {
      gPad->Update();
      TPaletteAxis* palette = (TPaletteAxis*)h->GetListOfFunctions()->FindObject("palette");
      palette->SetX1NDC(1 - (Float_t)(mar_right - pal_offset_x)/padsize_x);
      palette->SetX2NDC(1 - (Float_t)(mar_right - pal_offset_x - pal_width_x)/padsize_x);
      palette->SetY1NDC((Float_t)mar_bottom/padsize_y);
      palette->SetY2NDC(1 - (Float_t)mar_top/padsize_y);
    }
    gStyle->SetOptTitle(0);
    return canvas;
  }
  
  void draw_with_right_yaxis_(TH1D* hleft, TH1D* hright, std::string opt, std::string ytitle_right,
			      Double_t xmin = NOVAL_F, Double_t xmax = NOVAL_F,
			      Double_t left_ymin = NOVAL_F, Double_t left_ymax = NOVAL_F,
			      Double_t right_ymin = NOVAL_F, Double_t right_ymax = NOVAL_F,
			      Double_t left_localymax_x = NOVAL_F, Double_t shift_maximum = 1.0) {
    if (hleft->GetEntries()>0&&hleft->GetMaximum()!=0&&hright->GetEntries()>0&&hright->GetMaximum()!=0) {
      // Set X axis range
      Int_t binx_min = hleft->GetXaxis()->FindBin(xmin);
      Int_t binx_max = hleft->GetXaxis()->FindBin(xmax);
      xmin = (xmin == NOVAL_F) ? hleft->GetXaxis()->GetXmin()
	: hleft->GetXaxis()->GetBinLowEdge(binx_min);
      xmax = xmax == NOVAL_F ? hleft->GetXaxis()->GetXmax()
	: hleft->GetXaxis()->GetBinUpEdge(binx_max);
      binx_min = hleft->GetXaxis()->FindBin(xmin);
      binx_max = hleft->GetXaxis()->FindBin(xmax);
      hleft->GetXaxis()->SetRange(binx_min,binx_max-1);
      hright->GetXaxis()->SetRange(binx_min,binx_max-1);
      // Set Styles and Draw First histogram with left y axis
      if (opt.find("P")!=std::string::npos) {
	hleft->SetMarkerStyle(21);
	hright->SetMarkerStyle(20);
	hright->SetMarkerColor(2);
      } else hright->SetLineColor(2);
      hleft->Draw(opt.c_str());
      // Rescale Right Histogram according to specified Right Y axis ranges
      // If ranges unspecified, will set histogram to same minimum/maximum
      // (On both X and right Y axis)
      Double_t left_maximum = left_localymax_x==NOVAL_F ? hleft->GetMaximum() :
	hleft->GetBinContent(hleft->GetXaxis()->FindBin(left_localymax_x));
      Double_t right_maximum = hright->GetMaximum();
      left_ymin = (left_ymin==NOVAL_F) ? hleft->GetMinimum() : left_ymin;
      left_ymax = (left_ymax==NOVAL_F) ? left_maximum *1.05 : left_ymax;
      hleft->GetYaxis()->SetRangeUser(left_ymin,left_ymax);
      // Find Minimum Plotted value
      Double_t left_minimum = left_maximum;
      Double_t right_minimum = right_maximum;
      for (int binx = binx_min; binx<binx_max; binx++) {
	Double_t value_left = hleft->GetBinContent(binx);
	if (value_left<left_minimum&&value_left>xmin) left_minimum = value_left;
	Double_t value_right = hright->GetBinContent(binx);
	if (value_right<right_maximum&&value_right>0) right_minimum = value_right;
      }
      // Rescale Right Histogram
      if (right_ymin==NOVAL_F&&right_ymax==NOVAL_F) {
	// No ranges specified 
	// Minimums, Maximums forced to same height (or shifted by factor)
	Double_t rescale_factor = (left_maximum - left_minimum) / (right_maximum - right_minimum) * shift_maximum;
	for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
	  Double_t value = hright->GetBinContent(binx);
	  if (value) hright->SetBinContent(binx,left_minimum + (value - right_minimum) * rescale_factor);
	}
	right_ymin = right_minimum - (left_minimum - left_ymin) / rescale_factor;
	right_ymax = right_minimum + (left_ymax - left_minimum) / rescale_factor;
      } else if (right_ymax==NOVAL_F) {
	// Right Y-axis low range Specified
	// Maximums forced to same height (or shifted by factor)
	Double_t rescale_factor = (left_maximum - left_ymin) / (right_maximum - right_ymin) * shift_maximum;
	for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
	  Double_t value = hright->GetBinContent(binx);
	  if (value) hright->SetBinContent(binx,left_ymin + (value - right_ymin) * rescale_factor);
	}
	right_ymax = right_ymin + (left_ymax-left_ymin) / rescale_factor;
      } else if (right_ymin==NOVAL_F) {
	// Right Y-axis upper range Specified 
	// Maximums forced to same height (or shifted by factor)
	Double_t rescale_factor = (left_ymax - left_maximum) / (right_ymax - right_maximum) / shift_maximum;
	for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
	  Double_t value = hright->GetBinContent(binx);
	  if (value) hright->SetBinContent(binx,left_ymax - (right_ymax - value) * rescale_factor);
	}
	right_ymin = right_ymax - (left_ymax-left_ymin) / rescale_factor;
      } else {
	// Right Y-axis ranges Specified (Manual mode)
	// Rescale accordingl, no shift possible
	Double_t rescale_factor = (left_ymax - left_ymin) / (right_ymax - right_ymin);
	for (int binx = 1; binx<=hright->GetNbinsX(); binx++) {
	  Double_t value = hright->GetBinContent(binx);
	  if (value) hright->SetBinContent(binx,left_ymin + (value - right_ymin) * rescale_factor);
	}
      }
      // Draw Rescaled histo and Paint Right axis
      hright->Draw("SAMEP");
      Double_t smallshift = hleft->GetBinWidth(binx_max)/1000;
      TGaxis* axis = new TGaxis(xmax+smallshift,left_ymin,xmax+smallshift,left_ymax,right_ymin,right_ymax,510,"+L");
      axis->SetDecimals(hleft->GetZaxis()->GetDecimals());
      axis->SetLabelFont(hleft->GetZaxis()->GetLabelFont());
      axis->SetLabelSize(hleft->GetZaxis()->GetLabelSize());
      axis->SetLabelColor(2);
      axis->SetLineColor(2);
      axis->SetTitleColor(2);
      axis->SetTitle(ytitle_right.c_str());
      axis->SetTitleFont(hleft->GetZaxis()->GetTitleFont());
      axis->SetTitleSize(hleft->GetZaxis()->GetTitleSize());
      axis->SetTitleOffset(hleft->GetZaxis()->GetTitleOffset());
      axis->Draw();
    }
  }

  void save_canvas_(TCanvas* can, std::string out) {
    std::string title = can->GetName();
    out += title + ".png";
    can->SaveAs(out.c_str());
  }
  
  
  TCanvas* default_can_(std::string name, std::string title, int xsize, int ysize, float l, float r, float t, float b, int tx, int ty, int gx, int gy) {
    gStyle->SetErrorX(0);
    gStyle->SetPadLeftMargin(l/100.0);
    gStyle->SetPadRightMargin(r/100.0);
    gStyle->SetPadTopMargin(t/100.0);
    gStyle->SetPadBottomMargin(b/100.0);
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), xsize, ysize);
    canvas->SetFillColor(0);
    canvas->SetTicks(tx,ty);
    canvas->SetGrid(gx,gy);
    return canvas;
  }
  
  TCanvas* square_can_1d_(Histograms* h, std::string xtitle, std::string name, std::string title="", int gx = 0, int gy = 0) {
    h->dress_all_1d(xtitle);
    // 480 x 480 plot, 598 x 563 canvas
    gStyle->SetPadLeftMargin(0.17);
    gStyle->SetPadRightMargin(0.03);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadBottomMargin(0.12);
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), 602, 589);
    canvas->SetFillColor(0);
    canvas->SetTicks(1,1);
    canvas->SetGrid(gx,gy);
    return canvas;
  }

  TCanvas* square_can2_1d_(std::string name, std::string title="", int gx = 0, int gy = 0) {
    // 480 x 480 plot, 598 x 563 canvas
    gStyle->SetPadLeftMargin(0.17);
    gStyle->SetPadRightMargin(0.03);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadBottomMargin(0.12);
    title = (title.size()) ? title : name;
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), 602, 589);
    canvas->SetFillColor(0);
    canvas->SetTicks(1,1);
    canvas->SetGrid(gx,gy);
    return canvas;
  }

  TCanvas* square_can_2d_(Histograms* h, std::string xtitle, std::string ytitle, std::string name, std::string title="", int gx = 0, int gy = 0) {
    h->dress_all_2d(xtitle, ytitle);
    // 480 x 480 plot, 683 x 563 canvas
    gStyle->SetPadLeftMargin(0.17);
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadBottomMargin(0.12);
    title = (title.size()) ? title : name;
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), 687, 589);
    canvas->SetFillColor(0);
    canvas->SetTicks(1,1);
    canvas->SetGrid(gx,gy);
    return canvas;
  }

  TCanvas* square_can2_2d_(std::string name, std::string title="", int gx = 0, int gy = 0) {
    // 480 x 480 plot, 683 x 563 canvas
    gStyle->SetPadLeftMargin(0.17);
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadBottomMargin(0.12);
    title = (title.size()) ? title : name;
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), 687, 589);
    canvas->SetFillColor(0);
    canvas->SetTicks(1,1);
    canvas->SetGrid(gx,gy);
    return canvas;
  }

  TCanvas* rectangle_can2_1d_(std::string name, std::string title="", int gx = 0, int gy = 0) {
    // 0.9, 0.7
    // 960 x 480 plot, 1101 x 563 canvas
    gStyle->SetPadLeftMargin(0.1);
    gStyle->SetPadRightMargin(0.03);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadBottomMargin(0.12);
    title = (title.size()) ? title : name;
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), 1105, 589);
    canvas->SetFillColor(0);
    canvas->SetTicks(1,1);
    canvas->SetGrid(gx,gy);
    return canvas;
  }

  TCanvas* rectangle_can_2d_(Histograms* h, std::string xtitle, std::string ytitle, std::string name, std::string title="", int gx = 0, int gy = 0) {
    h->dress_all_2d(xtitle, ytitle, "", 0.9, 0.5, 0.7);
    // 960 x 480 plot, 1198 x 563 canvas
    gStyle->SetPadLeftMargin(0.07);
    gStyle->SetPadRightMargin(0.13);
    gStyle->SetPadTopMargin(0.03);
    gStyle->SetPadBottomMargin(0.12);
    title = (title.size()) ? title : name;
    TCanvas* canvas = new TCanvas(name.c_str(), title.c_str(), 1202, 589);
    canvas->SetFillColor(0);
    canvas->SetTicks(1,1);
    canvas->SetGrid(gx,gy);
    return canvas;
  }
    
  void year_lat_(double xmax, double ymin, double ymax) {
    std::string year = year_ + " Data";
    TLatex* year_lat= new TLatex(xmax,  ymax+(ymax-ymin)/40.0, year.c_str()); 
    year_lat->SetLineWidth(2); year_lat->SetTextSize(0.04); year_lat->SetTextAlign(31); year_lat->Draw();
  }
  
  void prelim_lat_(double xmin, double xmax, double ymin, double ymax, bool in, bool date) {
    std::string prelim = "CMS preliminary";
    if (date) prelim = "CMS preliminary " + date_;
    if (in) { TLatex* cms_lat = new TLatex(xmin+(xmax-xmin)/20.0, ymax-(ymax-ymin)/10.0, prelim.c_str()); cms_lat->SetLineWidth(2); cms_lat->Draw(); }
    else { TLatex* cms_lat = new TLatex(xmin, ymax+(ymax-ymin)/40.0, prelim.c_str()); cms_lat->SetLineWidth(2); cms_lat->Draw(); }
  }

  std::vector<int> string_to_vector_(std::string val) {
    std::vector<int> vec;
    // Reading values from string separated by commas, (ranges x-y can be specified too)
    // eg. "3-6,20,22,24-27,11," - add comma at the end of the string
    std::stringstream ss(val);
    while (ss) {
      while (ss && !isdigit(ss.peek ())) ss.get ();
      if (! ss)	break;
      int i = 0;
      int j = 0;
      ss >> i;
      while (ss && !ispunct(ss.peek ())) ss.get ();
      char a; ss>>a;
      if (a=='-') {
	while (ss && !isdigit(ss.peek ())) ss.get ();
	ss >> j;
      } else j=i;
      for (int a=i; a<=j; a++) vec.push_back(a);
    }
    return vec;
  }
  
  bool char_exist_in_string_(std::string& s, char c) {
    std::stringstream ss(s);
    char testchar;
    while (ss) {
      ss.get(testchar);
      if (testchar==c) return 1;
      if (! ss)	return 0;
    } return 0;
  }

  void set_color_(TH1D* h, Int_t col, std::string& opt) {
    if (char_exist_in_string_(opt,'P')) h->SetMarkerColor(col);
    else {
      h->SetLineColor(col);
      h->SetLineWidth(2);
    }
  }
  void set_marker_(TH1D* h, Int_t mar, std::string& opt) {
    if (char_exist_in_string_(opt,'P')) h->SetMarkerStyle(mar);
  }
  
  const char* legmark_(std::string& opt) {
    if (char_exist_in_string_(opt,'P')) return "P";
    else return "L";
  }

  void multidraw_with_legend_(Histograms* h, std::string val, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t> col,
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t i=0; i<vec.size(); i++) {
      set_color_(h->h1d(vec[i]),col[i],opt);
      set_marker_(h->h1d(vec[i]),marker_[i],opt);
      if (i==0) h->h1d(vec[i])->Draw(opt.c_str());
      else h->h1d(vec[i])->Draw(same.c_str());
      leg->AddEntry(h->h1d(vec[i]), p[vec[i]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

void multidraw_with_legend_(Histograms* h, std::string val, int j, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col,
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t i=0; i<vec.size(); i++) {
      set_color_(h->h1d(vec[i],j),col[i],opt);
      set_marker_(h->h1d(vec[i],j),marker_[i],opt);
      if (i==0) h->h1d(vec[i],j)->Draw(opt.c_str());
      else h->h1d(vec[i],j)->Draw(same.c_str());
      leg->AddEntry(h->h1d(vec[i],j), p[vec[i]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

  void multidrawnorm_with_legend_(Histograms* h, std::string val, int j, std::string opt, std::string title,
				  std::vector<std::string>& p, std::vector<Int_t>& col, 
				  float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    // Normalize by the number of entries and set range according to the highest histogram
    float max = 0.0;
    for (size_t i=0; i<vec.size(); i++) {
      h->h1d(vec[i],j)->Scale(1/h->h1d(vec[i],j)->GetEntries());
      float m = h->h1d(vec[i],j)->GetMaximum();
      if (m>max) max = m;
    }
    h->h1d(vec[0],j)->GetYaxis()->SetRangeUser(0,max*1.1);
    for (size_t i=0; i<vec.size(); i++) {
      set_color_(h->h1d(vec[i],j),col[i],opt);
      set_marker_(h->h1d(vec[i],j),col[i],opt);
      if (i==0) h->h1d(vec[i],j)->Draw(opt.c_str());
      else h->h1d(vec[i],j)->Draw(same.c_str());
      leg->AddEntry(h->h1d(vec[i],j), p[vec[i]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

  void multidraw_with_legend_(Histograms* h, std::string val, int j, int k, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col, 
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t i=0; i<vec.size(); i++) {
      set_color_(h->h1d(vec[i],j,k),col[i],opt);
      set_marker_(h->h1d(vec[i],j,k),marker_[i],opt);
      if (i==0) h->h1d(vec[i],j,k)->Draw(opt.c_str());
      else h->h1d(vec[i],j,k)->Draw(same.c_str());
      leg->AddEntry(h->h1d(vec[i],j,k), p[vec[i]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

  void multidraw_with_legend_(Histograms* h, int i, std::string val, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col, 
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t j=0; j<vec.size(); j++) {
      set_color_(h->h1d(i,vec[j],0),col[j],opt);
      set_marker_(h->h1d(i,vec[j],0),marker_[j],opt);
      if (j==0) h->h1d(i,vec[j],0)->Draw(opt.c_str());
      else h->h1d(i,vec[j],0)->Draw(same.c_str());
      leg->AddEntry(h->h1d(i,vec[j],0), p[vec[j]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

  void multidraw_with_legend_(Histograms* h, int i, std::string val, int k, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col, 
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t j=0; j<vec.size(); j++) {
      set_color_(h->h1d(i,vec[j],k),col[j],opt);
      set_marker_(h->h1d(i,vec[j],k),marker_[j],opt);
      if (j==0) h->h1d(i,vec[j],k)->Draw(opt.c_str());
      else h->h1d(i,vec[j],k)->Draw(same.c_str());
      leg->AddEntry(h->h1d(i,vec[j],k), p[vec[j]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }
  
  void multidraw_with_legend_(Histograms* h, int i, std::string val, int k, int l, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col, 
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t j=0; j<vec.size(); j++) {
      set_color_(h->h1d(i,vec[j],k,l),col[j],opt);
      set_marker_(h->h1d(i,vec[j],k,l),marker_[j],opt);
      if (j==0) h->h1d(i,vec[j],k,l)->Draw(opt.c_str());
      else h->h1d(i,vec[j],k,l)->Draw(same.c_str());
      leg->AddEntry(h->h1d(i,vec[j],k,l), p[vec[j]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }
  
  void multidraw_with_legend_(Histograms* h, int i, int j, std::string val, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col, 
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t k=0; k<vec.size(); k++) {
      set_color_(h->h1d(i,j,vec[k],0),col[k],opt);
      set_marker_(h->h1d(i,j,vec[k],0),marker_[k],opt);
      if (k==0) h->h1d(i,j,vec[k],0)->Draw(opt.c_str());
      else h->h1d(i,j,vec[k],0)->Draw(same.c_str());
      leg->AddEntry(h->h1d(i,j,vec[k],0), p[vec[k]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

  void multidraw_with_legend_(Histograms* h, int i, int j, std::string val, int l, std::string opt, std::string title,
			      std::vector<std::string>& p, std::vector<Int_t>& col, 
			      float x1, float x2, float y1, float y2) {
    std::string same = "SAME" + opt;
    TLegend* leg = new TLegend(x1,y1,x2,y2,title.c_str());
    std::vector<int> vec = string_to_vector_(val);
    for (size_t k=0; k<vec.size(); k++) {
      set_color_(h->h1d(i,j,vec[k],l),col[k],opt);
      set_marker_(h->h1d(i,j,vec[k],l),marker_[k],opt);
      if (k==0) h->h1d(i,j,vec[k],l)->Draw(opt.c_str());
      else h->h1d(i,j,vec[k],l)->Draw(same.c_str());
      leg->AddEntry(h->h1d(i,j,vec[k],l), p[vec[k]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
  }

  void draw_line_(double x1, double y1, double x2, double y2, Color_t col = 1, Style_t s = 1, Width_t w = 1) { 
    TLine* line = new TLine(x1,y1,x2,y2); if (col!=1) line->SetLineColor(col); if (s!=1) line->SetLineStyle(s); line->SetLineWidth(w); line->Draw(); }

  void draw_lat_(double x, double y, const char* text, Int_t col, double size=0.04, double angle=0.0, short al=11) { 
    TLatex* l = new TLatex(x,y,text); l->SetTextColor(col); l->SetTextSize(size); l->SetTextAngle(angle); l->SetTextAlign(al); l->Draw(); }

  void draw_def_(TH1D* h, std::string drawopt, Int_t col) {
    h->SetMarkerStyle(8);
    h->SetMarkerSize(1.0);
    h->SetMarkerColor(col);
    h->Draw(drawopt.c_str());
  }

  void draw_with_lat_(TH1D* h, std::string drawopt, Int_t col, std::string lat, float x, float y) {
    if (char_exist_in_string_(drawopt,'P')) {
      h->SetMarkerStyle(8);
      h->SetMarkerSize(1.0);
      h->SetMarkerColor(col);
    } else h->SetLineColor(col);
    h->Draw(drawopt.c_str());
    double max = 1.05*h->GetMaximum();
    double xmin = h->GetXaxis()->GetXmin();
    double xmax = h->GetXaxis()->GetXmax();
    draw_lat_(xmin+(xmax-xmin)*x, y*max, lat.c_str(), col, 0.04, 0.0, 12);
  }


  //_________________________________________________________________________________________________________
  //                                         Not Efficiency Plots
  //_________________________________________________________________________________________________________

  void ncd_can_(std::vector<TCanvas*>& c, Histograms* h) {
    TCanvas* can = custom_can_(h->h1d(0,2), h->histoname()+"_lay", "2012 April Data", "Normalized Cluster Charge [ke]", "Arbitrary", 0,0, 480,480, 90);
    h->multidraw_with_legend_(0, "2-5", "NORM", postfix_.Det, "1,2,4,3", "Fill 2469", 0.5,0.7, 0.6,0.8); 
    
    //for (int i=0; i<4; i++) setstyle_(h->h1d(i,0),"Normalized Cluster Charge [ke]","Arbitrary",1.0,1.3);
    //TCanvas* can = default_can_(h->histoname(), "Perf - Normalized Cluster Charge Distribution", 650,600, 13,7,9,11, 0,0, 0,0);
    //multidrawnorm_with_legend_(h, 0, "2-5", "", "Fill 2469", postfix_.Det, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void pcd_can_(std::vector<TCanvas*>& c, Histograms* h) {
    for (int i=0; i<4; i++) {
      setstyle_(h->h1d(i,0),"Pixel Charge [ke]","Arbitrary",1.0,1.3);
    }
    TCanvas* can = default_can_(h->histoname(), "Perf - Pixel Charge Distribution", 650,600, 13,7,9,11, 0,0, 0,0);
    multidrawnorm_with_legend_(h, "0-3,", 0, "", "", postfix_.Totlumi, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void clu_size_can_(std::vector<TCanvas*>& c, Histograms* h) {
    for (int i=0; i<4; i++) {
      setstyle_(h->h1d(i,0),"Cluster Size [Pixel]","Arbitrary",1.0,1.3);
    }
    TCanvas* can = default_can_(h->histoname(), "Perf - Cluster Size Distribution", 650,600, 13,7,9,11, 0,0, 0,0);
    multidrawnorm_with_legend_(h, "0-3,", 0, "", "", postfix_.Totlumi, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void clu_sizeX_can_(std::vector<TCanvas*>& c, Histograms* h) {
    for (int i=0; i<4; i++) {
      setstyle_(h->h1d(i,0),"Cluster SizeX [Pixel]","Arbitrary",1.0,1.3);
    }
    TCanvas* can = default_can_(h->histoname(), "Perf - Cluster SizeX Distribution", 650,600, 13,7,9,11, 0,0, 0,0);
    multidrawnorm_with_legend_(h, "0-3,", 0, "", "", postfix_.Totlumi, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void clu_sizeY_can_(std::vector<TCanvas*>& c, Histograms* h) {
    for (int i=0; i<4; i++) {
      setstyle_(h->h1d(i,0),"Cluster SizeY [Pixel]","Arbitrary",1.0,1.3);
    }
    TCanvas* can = default_can_(h->histoname(), "Perf - Cluster SizeY Distribution", 650,600, 13,7,9,11, 0,0, 0,0);
    multidrawnorm_with_legend_(h, "0-3,", 0, "", "", postfix_.Totlumi, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void alpha_can_(std::vector<TCanvas*>& c, Histograms* h) {
    for (int i=0; i<4; i++) {
      setstyle_(h->h1d(i,0),"Alpha","Arbitrary",1.0,1.3);
    }
    TCanvas* can = default_can_(h->histoname(), "Perf - Alpha", 650,600, 13,7,9,11, 0,0, 0,0);
    multidrawnorm_with_legend_(h, "0-3,", 0, "", "", postfix_.Totlumi, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void beta_can_(std::vector<TCanvas*>& c, Histograms* h) {
    for (int i=0; i<4; i++) {
      setstyle_(h->h1d(i,0),"Beta","Arbitrary",1.0,1.3);
    }
    TCanvas* can = default_can_(h->histoname(), "Perf - Beta", 650,600, 13,7,9,11, 0,0, 0,0);
    multidrawnorm_with_legend_(h, "0-3,", 0, "", "", postfix_.Totlumi, defcol_, 0.5,0.7, 0.6,0.8);
    c.push_back(can);
  }

  void draw_error_range_lines_(TH1D* h) {
    Color_t color = h->GetLineColor();
    Style_t style = h->GetLineStyle();
    Width_t width = h->GetLineWidth();
    for (int binx=1; binx<=h->GetNbinsX(); binx++) {
      double bincont = h->GetBinContent(binx);
      double binerr = h->GetBinError(binx);
      if (bincont>0) {
	double lowy = bincont - binerr;
	double highy = bincont + binerr;
	double x1 = h->GetBinLowEdge(binx);
	double x2 = x1 + h->GetBinWidth(binx);
	draw_line_(x1, lowy, x2, lowy, color, style, width);
	draw_line_(x1, highy, x2, highy, color, style, width);
      }
    }
  }

  void linear_fit_2d_(TH2D* h, int from_origo=0) {
    if (h->GetEntries()) {
      double xmin = h->GetXaxis()->GetXmin();
      double xmax = h->GetXaxis()->GetXmax();
      double ymin = h->GetYaxis()->GetXmin();
      double ymax = h->GetYaxis()->GetXmax();
      TF1* f;
      if (from_origo) f = new TF1("linear", "[0]*x", xmin, xmax);
      else f = new TF1("linear", "[0]*x+[1]", xmin, xmax);
      TProfile* prof = h->ProfileX();
      prof->Fit("linear","RMQ0");
      f->Draw("same");
      std::stringstream fiteq;
      fiteq.precision(4);
      if (from_origo) fiteq<<"Slope: "<<f->GetParameter(0);
      else fiteq<<"Fit: "<<f->GetParameter(0)<<" x + "<<f->GetParameter(1);
      TLatex* lat= new TLatex(xmin+(xmax-xmin)/2, ymin+(ymax-ymin)/10, fiteq.str().c_str()); 
      lat->SetLineWidth(2); lat->SetTextSize(0.04); lat->Draw();
    }
  }

  void nclu_plots_can_(std::vector<TCanvas*>& c, AllHistos& ah, Variables& v, PostFixes& p) {
    // Plot 1 - Nclu/Npix Correllation
    for (int fill=0; fill<=v.lumi_nfill; fill++) {
      for (int i=0; i<2; i++) {
	// Nclu
	setstyle_(ah.nclu_corr->h2d(fill,i),"N_{Cluster} - Layer 1","N_{Cluster} - "+p.Det[4+i],"",0.9,1.4,1.0);
	TCanvas* can = square_can2_2d_(ah.nclu_corr->histoname()+p.fill[fill]+p.corr[i], "N_Cluster Correllations - Fill"+p.Fill[fill]);
	ah.nclu_corr->h2d(fill,i)->Draw("COLZ");
	linear_fit_2d_(ah.nclu_corr->h2d(fill,i),1);
	c.push_back(can);
	// Npix
	setstyle_(ah.npix_corr->h2d(fill,i),"N_{Pixel} - Layer 1","N_{Pixel} - "+p.Det[4+i],"",0.9,1.4,1.0);
	can = square_can2_2d_(ah.npix_corr->histoname()+p.fill[fill]+p.corr[i], "N_Pixel Correllations - Fill"+p.Fill[fill]);
	ah.npix_corr->h2d(fill,i)->Draw("COLZ");
	linear_fit_2d_(ah.npix_corr->h2d(fill,i),1);
	c.push_back(can);
      }
    }
    // Plot 1 Alternative - Nclu/Npix Correllation Fitted Slopes:
    for (int fill=0; fill<=v.lumi_nfill; fill++) {
      ah.instlumi_corr->h1d(fill,0)->GetXaxis()->SetRangeUser(1250,3450);
      ah.instlumi_corr->h1d(fill,0)->GetYaxis()->SetRangeUser(0.55,0.9);
      setstyle_(ah.instlumi_corr->h1d(fill,0),"Instantaneous Luminosity [#mub^{-1}s^{-1}]","L_{N}/L_{1} Correlation Slope",0.9,1.4);
      TCanvas* can = square_can2_1d_(ah.instlumi_corr->histoname()+p.fill[fill], "Nclu/Npix Correllation Slopes vs Inst Lumi - Fill"+p.Fill[fill]);
      ah.instlumi_corr->multidraw_with_legend_(fill, "0-3", "PE1", postfix_.Corr2, "1,2,4,3","Fill "+p.Fill[fill], 0.6, 0.8, 0.3, 0.5);
      c.push_back(can);
    }
    // Plot 2 - N_cluster/N_Event vs LS (Bx Avg/100LS, Avg/1LS)
    ah.ls_fill->dress_all_1d("LumiSection","N_{cluster}/N_{Event}");
    for (int fill=1; fill<=v.lumi_nfill; fill++) {
      int maxbin = 0;
      for (int binx=1; binx<=ah.ls_fill->h1d(fill,0,1)->GetNbinsX(); binx++)
	if (ah.ls_fill->h1d(fill,0,1)->GetBinContent(binx)>0&&binx>maxbin) maxbin = binx;
      maxbin *= 100;
      for (int det=0; det<10; det++) {
	double min = 99999;
	double max = 0;
	for (int binx=1; binx<=ah.ls_fill->h1d(fill,det,1)->GetNbinsX(); binx++) {
	  double bincont = ah.ls_fill->h1d(fill,det,1)->GetBinContent(binx);
	  double binerr = ah.ls_fill->h1d(fill,det,1)->GetBinError(binx);
	  if (bincont>0) {
	    if (binx>maxbin) maxbin = binx;
	    if (bincont+binerr>max) max = bincont+binerr;
	    if (bincont-binerr<min) min = bincont-binerr;
	  }
	}
	if (min<0) min = 0;
	max = max + (max-min) * 0.1;
	min = (min - (max-min) * 0.1 >= 0) ? min - (max-min) * 0.1 : min;
	ah.ls_fill->h1d(fill,det,2)->GetXaxis()->SetRange(1,maxbin);
	ah.ls_fill->h1d(fill,det,2)->GetYaxis()->SetRangeUser(min,max);
	ah.ls_fill->h1d(fill,det,1)->SetLineWidth(2);
	ah.ls_fill->h1d(fill,det,2)->SetMarkerSize(0.5);
	//TCanvas* can = rectangle_can2_1d_(ah.ls_fill->histoname()+p.fill[fill], " N_Cluster/N_Event - Fill"+p.Fill[fill]);
	TCanvas* can = square_can2_1d_(ah.ls_fill->histoname()+p.fill[fill]+p.det[det], " N_Cluster/N_Event - Fill"+p.Fill[fill]+" "+p.Det[det]);
	ah.ls_fill->multidraw_with_legend_(fill, det, "2,1", "P9", postfix_.Avg, "2,1","Fill "+p.Fill[fill]+" - "+postfix_.Det[det], 0.5,0.7, 0.75,0.95);
	draw_error_range_lines_(ah.ls_fill->h1d(fill, det, 1));
	c.push_back(can);
      }
    }
    // Plot 2 Alternative - N_cluster/N_Event vs BX (LS [1-100]+N*100)
    int nmergex = 4;
    int nmergey = 12;
    double nbinsy = ah.bx_ls->h2d(0, 0, 0)->GetNbinsY()/nmergey;
    double nbinsx = ah.bx_ls->h2d(0, 0, 0)->GetNbinsX()/nmergex;
    double xlow   = ah.bx_ls->h2d(0, 0, 0)->GetXaxis()->GetXmin();
    double xhigh  = ah.bx_ls->h2d(0, 0, 0)->GetXaxis()->GetXmax();
    int binw = (int)(xhigh - xlow) / nbinsx;
    Histograms* bx_nclu = new Histograms("bx_nclu", 3, 1);
    std::vector<std::string> pf_ls;
    std::vector<std::string> Pf_ls;
    pf_ls.resize(nbinsx);
    Pf_ls.resize(nbinsx);
    for (int i=1; i<=nbinsx; i++) {
      std::stringstream ss1,ss2;
      int low = (int)xlow+(i-1)*binw+1;
      int high = (int)xlow+i*binw;
      ss1<<"_ls"<<low<<"-"<<high;
      ss2<<"LS "<<low<<"-"<<high;
      pf_ls[i-1] = ss1.str();
      Pf_ls[i-1] = ss2.str();
    }
    std::stringstream ytitle;
    ytitle<<"N_{cluster}/N_{Event} [/"<<nmergey<<" BX]";
    bx_nclu->add(p.fill, 1, 1, v.lumi_nfill+1, 0, p.det, 1, 1, 10, 0, pf_ls, 1, 1, nbinsx,  nbinsy, 0, 3600);
    bx_nclu->dress_all_1d("Bunch Crossing",ytitle.str());
    for (int fill=1; fill<=v.lumi_nfill; fill++) {
      for (int det=0; det<10; det++) {
	std::map<int, int> has_enough_stat;
	for (int binx=1; binx<=nbinsx; binx++) {
	  TH1D* h = (TH1D*)ah.ls_fill->h1d(fill,0,3)->Rebin(binw,"rebinned_statistics");
	  double stat = h->GetBinContent(binx);
	  delete h;
	  if (stat>5000) has_enough_stat[binx-1]=(int)stat;
	  for (int biny=1; biny<=nbinsy; biny++) {
	    double nclu = 0;
	    double nevt = 0;
	    for (int j=nmergey-1; j>=0; j--) {
	      for (int i=nmergex-1; i>=0; i--) {
		nclu += ah.bx_ls->h2d(fill, det, 0)->GetBinContent(nmergex*binx-i,nmergey*biny-j);
		nevt += ah.bx_ls->h2d(fill, det, 1)->GetBinContent(nmergex*binx-i,nmergey*biny-j);
	      }
	    }
	    double avg = nclu/nevt;
	    if (nevt) bx_nclu->h1d(fill, det, binx-1)->SetBinContent(biny, avg);
	  }
	}
	int last=0;
	for (int i=0; i<nbinsx; i++) if (has_enough_stat.count(i)) last=i;
	int n = 4;
	std::map<int, int> fillpoints;
	for (int i=0; i<n; i++) {
	  int plot = (int)(float)i*last/(n-1);
	  while (!has_enough_stat.count(plot)&&plot<last) plot++;
	  if (has_enough_stat.count(plot)) fillpoints[plot]=1;
	}
	std::stringstream plot_these;
	for (int i=0; i<nbinsx; i++) if (fillpoints.count(i)) {
	  plot_these<<i;
	  if (i!=last) plot_these<<",";
	}
	TCanvas* can = square_can2_1d_(bx_nclu->histoname()+p.fill[fill]+p.det[det], "N_Cluster/N_Event vs BX - LS - Fill"+p.Fill[fill]+" "+p.Det[det]);
	bx_nclu->multidraw_with_legend_(fill, det, plot_these.str(), "P9", Pf_ls, "800,2,616,4,3","Fill "+p.Fill[fill]+" - "+postfix_.Det[det], 0.53,0.73, 0.12,0.32);
	for (int i=0; i<nbinsx; i++) {
	  bx_nclu->h1d(fill,det,i)->SetMarkerStyle(20);
	}
	c.push_back(can);
      }
    }
    // Plot 3 - Nclu Ratio
    // Tot Int Lumi
    ah.totlumi_ratio->h1d(2)->GetYaxis()->SetRangeUser(0.18,0.3);
    setstyle_(ah.totlumi_ratio->h1d(2),"Total Int. Luminosity - 2011 [pb^{-1}]","N_{Cluster} Ratio",0.9,1.4);
    TCanvas* can = square_can2_1d_(ah.totlumi_ratio->histoname(), "N_Cluster Ratio vs Total Int Lumi - Layers", 1,1);
    ah.totlumi_ratio->multidraw_with_legend_("2-5", "P", postfix_.Det, "1,2,4,3"); c.push_back(can);
    for (int fill=1; fill<=v.lumi_nfill; fill++) {
      // time (fill)
      int nbin= ah.time->h1d(fill,0,2)->GetNbinsX();
      int max = nbin;
      for (int binx=1; binx<=nbin; binx++) {
	double bincont = ah.time->h1d(fill,0,2)->GetBinContent(binx);
	if (bincont>0&&binx<nbin) max = binx+1;
      }
      ah.time->h1d(fill, 2, 2)->GetXaxis()->SetRange(1,max);
      setstyle_(ah.time->h1d(fill, 2, 2),"Time Since Start of Fill [h]","N_{Cluster} Ratio",0.9,1.4);
      ah.time->h1d(fill, 2, 2)->GetYaxis()->SetRangeUser(0.18,0.3);
      TCanvas* can = square_can2_1d_(ah.time->histoname()+"_ratio"+p.fill[fill], "N_Cluster Ratio vs Time - Layers - Fill "+p.Fill[fill], 1,1);
      ah.time->multidraw_with_legend_(fill, "2-5", 2, "P", postfix_.Det, "1,2,4,3", p.Fill2[fill]); c.push_back(can);
      // instlumi (fill)
      int min = 1;
      nbin= ah.instlumi->h1d(fill,0,2)->GetNbinsX();
      max = nbin;
      for (int binx=1; binx<=nbin; binx++) {
	double bincont = ah.instlumi->h1d(fill,0,2)->GetBinContent(binx);
	if (binx>1&&bincont>0&&min==1) min = binx -1;
	if (bincont>0&&binx<nbin) max = binx+1;
      }
      ah.instlumi->h1d(fill, 2, 2)->GetXaxis()->SetRange(min,max);
      setstyle_(ah.instlumi->h1d(fill, 2, 2),"Instantaneous Luminosity [#mub^{-1}s^{-1}]","N_{Cluster} Ratio",0.9,1.4);
      ah.instlumi->h1d(fill, 2, 2)->GetYaxis()->SetRangeUser(0.18,0.3);
      can = square_can2_1d_(ah.instlumi->histoname()+"_ratio"+p.fill[fill], "N_Cluster Ratio vs Time - Layers - Fill "+p.Fill[fill], 1,1);
      ah.instlumi->multidraw_with_legend_(fill, "2-5", 2, "P", postfix_.Det, "1,2,4,3", "Fill "+p.Fill[fill]); c.push_back(can);
    }
  }


  //_________________________________________________________________________________________________________
  //                                             Efficiency Plots
  //_________________________________________________________________________________________________________

  void lay_can_(std::vector<TCanvas*>& c, TH1D* h, std::string& fill, std::string& Fill) {
    //setstyle_(h, "", "Efficiency", 1.0, 1.4);
    //TCanvas* can = default_can_("lay"+fill, "Eff - Layer - Fill "+Fill, 800,750, 15,10,10,10, 1,1, 0,0);
    c.push_back(custom_can_(h, "lay"+fill, "", "", "Efficiency", 0, 0, 480,480, 90));
    h->SetMarkerStyle(8);
    h->SetMarkerSize(1.3);
    h->GetXaxis()->SetBinLabel(1, "Layer 1");
    h->GetXaxis()->SetBinLabel(2, "Layer 2");
    h->GetXaxis()->SetBinLabel(3, "Layer 3");
    h->GetXaxis()->SetBinLabel(4, "Disk -2");
    h->GetXaxis()->SetBinLabel(5, "Disk -1");
    h->GetXaxis()->SetBinLabel(6, "Disk +1");
    h->GetXaxis()->SetBinLabel(7, "Disk +2");
    h->GetXaxis()->LabelsOption("h");
    h->GetXaxis()->SetLabelSize(0.05);
    h->GetYaxis()->SetRangeUser(0.98,1.005);
    gStyle->SetErrorX(0);
    h->Draw("PE1X0");
    //TLatex lat(4.0, 0.97667, "2011 Collision Data"); lat.SetLineWidth(2); lat.SetTextSize(0.04); lat.Draw();
    //prelim_lat_(0.5,7.5,0.96,1.005, true, false);
    //year_lat_(7.5, 0.96, 1.01);
    // for (int i=1; i<=7; i++) {
    //   double min = h->GetBinContent(i) - 0.002;
    //   double max = h->GetBinContent(i) + 0.002;
    //   if (max>=1.0) max = 1.0;
    //   // Draw error bars
    //   draw_line_(i,min,i,max);
    //   draw_line_(i-0.1,min,i+0.1,min);
    //   draw_line_(i-0.1,max,i+0.1,max);
    // }
  }

  void mod_can_(std::vector<TCanvas*>& c, TH2D* h, int i, std::string& fill, std::string& Fill) {
    std::string name = "mod" + postfix_.det[i+3+(i==3)*-4];
    std::string title = "Eff - ROC - " + postfix_.Det[i+3+(i==3)*-4] + " - Fill ";
    TCanvas* can = default_can_(name+fill, title+Fill, 800,750, 12,17,10,10, 0,0, 0,0);
    if (i<3) setstyle_(h,"Modules","Ladders","Efficiency",0.9,1.0,1.2);
    else {
      setstyle_(h,"","Blades","Efficiency",1.0,1.0,1.2);
      gStyle->SetPadRightMargin(0.2);
      h->GetXaxis()->SetBinLabel(1, "Disk-2 Pnl2");
      h->GetXaxis()->SetBinLabel(2, "Disk-2 Pnl1");
      h->GetXaxis()->SetBinLabel(3, "Disk-1 Pnl2");
      h->GetXaxis()->SetBinLabel(4, "Disk-1 Pnl1");
      h->GetXaxis()->SetBinLabel(6, "Disk+1 Pnl1");
      h->GetXaxis()->SetBinLabel(7, "Disk+1 Pnl2");
      h->GetXaxis()->SetBinLabel(8, "Disk+2 Pnl1");
      h->GetXaxis()->SetBinLabel(9, "Disk+2 Pnl2");
      h->GetXaxis()->LabelsOption("d");
    }
    h->GetXaxis()->SetTickLength(0.0);
    h->GetYaxis()->SetTickLength(0.0);
    h->GetZaxis()->SetRangeUser(0.95,1.0);
    h->Draw("COLZ");
    float ylow[4] = { -10.5, -16.5, -22.5, -12.5 };
    float yhigh[4] = { 10.5, 16.5, 22.5, 12.5 };
    year_lat_(4.5, ylow[i], yhigh[i]);
    c.push_back(can);
  }
  
  void roc_can_(std::vector<TCanvas*>& c, TH2D* h, TH2D* h1, TH2D* h2, int i, std::string& fill, std::string& Fill) {
    std::string name, title;
    if (i<3) { name = "roc" + postfix_.det[i+3]; title = "Eff - ROC - " + postfix_.Det[i+3] + " - Fill "; }
    else { name = "roc" + postfix_.fpixIO[i-3]; title = "Eff - ROC - " + postfix_.FpixIO[i-3] + " - Fill "; }
    TCanvas* can = default_can_(name+fill, title+Fill, 800,750, 12,17,10,10, 0,0, 0,0);
    if (i<3) setstyle_(h,"Modules","Ladders","Efficiency",0.9,1.0,1.2);
    else {
      setstyle_(h,"","Blades","Efficiency",1.0,1.0,1.2);
      gStyle->SetPadRightMargin(0.2);
      h->GetXaxis()->SetBinLabel(1, "Disk-2 Pnl2");
      h->GetXaxis()->SetBinLabel(9, "Disk-2 Pnl1");
      h->GetXaxis()->SetBinLabel(19, "Disk-1 Pnl2");
      h->GetXaxis()->SetBinLabel(27, "Disk-1 Pnl1");
      h->GetXaxis()->SetBinLabel(41, "Disk+1 Pnl1");
      h->GetXaxis()->SetBinLabel(49, "Disk+1 Pnl2");
      h->GetXaxis()->SetBinLabel(59, "Disk+2 Pnl1");
      h->GetXaxis()->SetBinLabel(67, "Disk+2 Pnl2");
      h->GetXaxis()->LabelsOption("d");
    }
    h->GetXaxis()->SetTickLength(0.0);
    h->GetYaxis()->SetTickLength(0.0);
    h->Draw("COLZ");
    // Inactive modules - create hatched areas
    int ybins[5] = { 42, 66, 90, 144, 144 };
    float ylow[5] = { -10.5, -16.5, -22.5, 0.5, -12.5 };
    float yhigh[5] = { 10.5, 16.5, 22.5, 12.5, -0.5 };
    for (int j=1; j<=72; j++) for (int k=1; k<=ybins[i]; k++) 
      if (h1->GetBinContent(j,k)>0) 
	h2->SetBinContent(j,k,0);
    h2->SetFillColor(1); h2->SetFillStyle(3004); 
    h2->Draw("SAMEBOX");
    // Shade out irrelevant areas
    std::stringstream outname;
    outname<<"roc_out"<<i<<fill;
    TH2D* roc_out = new TH2D(outname.str().c_str(),outname.str().c_str(),72,-4.5,4.5,ybins[i],ylow[i],yhigh[i]);
    roc_out->SetFillColor(1); roc_out->SetFillStyle(3002);
    if (i<3) {
      for (int j=1; j<=72; j++) {
	roc_out->SetBinContent(j,1,10); roc_out->SetBinContent(j,ybins[i]/2-1,10); roc_out->SetBinContent(j,ybins[i]/2,10);
	roc_out->SetBinContent(j,ybins[i]/2+1,10); roc_out->SetBinContent(j,ybins[i]/2+2,10); roc_out->SetBinContent(j,ybins[i],10); }
      for (int j=1; j<=ybins[i]; j++) for (int k=33; k<=40; k++)  roc_out->SetBinContent(k,j,10);
    } else {
      for (int j=1; j<=144; j++) {
	roc_out->SetBinContent(7,j,10); roc_out->SetBinContent(8,j,10); roc_out->SetBinContent(15,j,10); roc_out->SetBinContent(16,j,10);
	roc_out->SetBinContent(17,j,10); roc_out->SetBinContent(18,j,10);roc_out->SetBinContent(25,j,10); roc_out->SetBinContent(26,j,10);
	for (int k=33; k<=40; k++) roc_out->SetBinContent(k,j,10);
	roc_out->SetBinContent(47,j,10); roc_out->SetBinContent(48,j,10); roc_out->SetBinContent(55,j,10); roc_out->SetBinContent(56,j,10);
	roc_out->SetBinContent(57,j,10); roc_out->SetBinContent(58,j,10); roc_out->SetBinContent(65,j,10); roc_out->SetBinContent(66,j,10); }
      for (int l=0; l<12; l++) for (int j=0; j<2; j++) {
	for (int k=1; k<=6; k++) {
	  roc_out->SetBinContent(k+j*18,1+12*l,10); roc_out->SetBinContent(k+j*18,12+12*l,10);
	  roc_out->SetBinContent(73-(k+j*18),1+12*l,10); roc_out->SetBinContent(73-(k+j*18),12+12*l,10);
	  roc_out->SetBinContent(8+k+j*18,1+12*l,10); roc_out->SetBinContent(8+k+j*18,12+12*l,10);
	  roc_out->SetBinContent(-8+73-(k+j*18),1+12*l,10); roc_out->SetBinContent(-8+73-(k+j*18),12+12*l,10); }
	for (int k=3; k<=6; k++) {
	  roc_out->SetBinContent(k+j*18,2+12*l,10); roc_out->SetBinContent(k+j*18,11+12*l,10);
	  roc_out->SetBinContent(73-(k+j*18),2+12*l,10); roc_out->SetBinContent(73-(k+j*18),11+12*l,10); }
	for (int k=2; k<=6; k++) {
	  roc_out->SetBinContent(8+k+j*18,2+12*l,10); roc_out->SetBinContent(8+k+j*18,11+12*l,10);
	  roc_out->SetBinContent(-8+73-(k+j*18),2+12*l,10); roc_out->SetBinContent(-8+73-(k+j*18),11+12*l,10); }
	for (int k=5; k<=6; k++) {
	  roc_out->SetBinContent(k+j*18,3+12*l,10); roc_out->SetBinContent(k+j*18,10+12*l,10);
	  roc_out->SetBinContent(73-(k+j*18),3+12*l,10); roc_out->SetBinContent(73-(k+j*18),10+12*l,10); }
	for (int k=4; k<=6; k++) {
	  roc_out->SetBinContent(8+k+j*18,3+12*l,10); roc_out->SetBinContent(8+k+j*18,10+12*l,10);
	  roc_out->SetBinContent(-8+73-(k+j*18),3+12*l,10); roc_out->SetBinContent(-8+73-(k+j*18),10+12*l,10); }
	roc_out->SetBinContent(8+6+j*18,4+12*l,10); roc_out->SetBinContent(8+6+j*18,9+12*l,10);
	roc_out->SetBinContent(-8+73-(6+j*18),4+12*l,10); roc_out->SetBinContent(-8+73-(6+j*18),9+12*l,10);
      }
    }
    roc_out->Draw("SAMEBOX");
    // Draw Lines around modules
    if (i<3) {
      int lad[3] = { 10, 16, 22 };
      for (int j=0; j<=5; j+=5) {
	draw_line_(-4.5+j,-1*lad[i],-0.5+j,-1*lad[i],12); draw_line_(-4.5+j,-1.0,-0.5+j,-1.0,12);
	draw_line_(-4.5+j,1.0,-0.5+j,1.0,12); draw_line_(-4.5+j,lad[i],-0.5+j,lad[i],12);
	for (int k=0; k<lad[i]-1; k++) { draw_line_(-4.5+j,-1*lad[i]+0.5+k,-0.5+j,-1*lad[i]+0.5+k,12); draw_line_(-4.5+j,1.5+k,-0.5+j,1.5+k,12); }
	for (float k=-1.0*lad[i]; k<=-1.0; k+=0.5) {
	  draw_line_(-4.5,k,-0.5,k,12, 3); draw_line_(-4.5+j,k,-0.5+j,k,12, 3);
	  draw_line_(-4.5,k+lad[i]+1,-0.5,k+lad[i]+1,12, 3); draw_line_(-4.5+j,k+lad[i]+1,-0.5+j,k+lad[i]+1,12, 3); } }
      for (int j=0; j<=9; j++) { draw_line_(-4.5+j,-1*lad[i],-4.5+j,-1.0,12); draw_line_(-4.5+j,1.0,-4.5+j,lad[i],12); }
    } else {
      for (int blade=0; blade<12; blade++) for (int side=-1; side<=1; side+=2)  for (int panel=0; panel<=1; panel++) for (int disk=0; disk<=1; disk++) {
	//Horizontal lines
	draw_line_(side*(-4.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,side*(-4.5+(2.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,12);
	draw_line_(side*(-4.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,side*(-4.5+(2.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,12);
	draw_line_(side*(-4.5+(2.0/8)+panel*18.0/8),(i==4)*-13.0+8.0/12+blade,side*(-4.5+(4.0/8)+panel*18.0/8),(i==4)*-13.0+8.0/12+blade,12);
	draw_line_(side*(-4.5+(2.0/8)+panel*18.0/8),(i==4)*-13.0+16.0/12+blade,side*(-4.5+(4.0/8)+panel*18.0/8),(i==4)*-13.0+16.0/12+blade,12);
	draw_line_(side*(-4.5+(4.0/8)+panel*18.0/8),(i==4)*-13.0+9.0/12+blade,side*(-4.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+9.0/12+blade,12);
	draw_line_(side*(-4.5+(4.0/8)+panel*18.0/8),(i==4)*-13.0+15.0/12+blade,side*(-4.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+15.0/12+blade,12);
	draw_line_(side*(-3.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,side*(-3.5+(1.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,12);
	draw_line_(side*(-3.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,side*(-3.5+(1.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,12);
	draw_line_(side*(-3.5+(1.0/8)+panel*18.0/8),(i==4)*-13.0+8.0/12+blade,side*(-3.5+(3.0/8)+panel*18.0/8),(i==4)*-13.0+8.0/12+blade,12);
	draw_line_(side*(-3.5+(1.0/8)+panel*18.0/8),(i==4)*-13.0+16.0/12+blade,side*(-3.5+(3.0/8)+panel*18.0/8),(i==4)*-13.0+16.0/12+blade,12);
	draw_line_(side*(-3.5+(3.0/8)+panel*18.0/8),(i==4)*-13.0+9.0/12+blade,side*(-3.5+(5.0/8)+panel*18.0/8),(i==4)*-13.0+9.0/12+blade,12);
	draw_line_(side*(-3.5+(3.0/8)+panel*18.0/8),(i==4)*-13.0+15.0/12+blade,side*(-3.5+(5.0/8)+panel*18.0/8),(i==4)*-13.0+15.0/12+blade,12);
	draw_line_(side*(-3.5+(5.0/8)+panel*18.0/8),(i==4)*-13.0+10.0/12+blade,side*(-3.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+10.0/12+blade,12);
	draw_line_(side*(-3.5+(5.0/8)+panel*18.0/8),(i==4)*-13.0+14.0/12+blade,side*(-3.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+14.0/12+blade,12);
	// Vertical lines
	draw_line_(side*(-4.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,side*(-4.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,12);
	draw_line_(side*(-4.5+(2.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,side*(-4.5+(2.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,12);
	draw_line_(side*(-4.5+(4.0/8)+panel*18.0/8),(i==4)*-13.0+8.0/12+blade,side*(-4.5+(4.0/8)+panel*18.0/8),(i==4)*-13.0+16.0/12+blade,12);
	draw_line_(side*(-4.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+9.0/12+blade,side*(-4.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+15.0/12+blade,12);
	draw_line_(side*(-3.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,side*(-3.5+(0.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,12);
	draw_line_(side*(-3.5+(1.0/8)+panel*18.0/8),(i==4)*-13.0+7.0/12+blade,side*(-3.5+(1.0/8)+panel*18.0/8),(i==4)*-13.0+17.0/12+blade,12);
	draw_line_(side*(-3.5+(3.0/8)+panel*18.0/8),(i==4)*-13.0+8.0/12+blade,side*(-3.5+(3.0/8)+panel*18.0/8),(i==4)*-13.0+16.0/12+blade,12);
	draw_line_(side*(-3.5+(5.0/8)+panel*18.0/8),(i==4)*-13.0+9.0/12+blade,side*(-3.5+(5.0/8)+panel*18.0/8),(i==4)*-13.0+15.0/12+blade,12);
	draw_line_(side*(-3.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+10.0/12+blade,side*(-3.5+(6.0/8)+panel*18.0/8),(i==4)*-13.0+14.0/12+blade,12);
      }
    }
    year_lat_(4.5, ylow[i], yhigh[i]);
    /*       prelim_lat_(-4.5,4.5,ylow[i],yhigh[i],false,false); */
    c.push_back(can);
  }
  
  /*   void run_can_(std::vector<TCanvas*>& c, Histograms* h) { */
  /*     dress_all_(h,"Run Number",1.5,1.0); */
  /*     for (int i=0; i<10; i++) { */
  /*       for (int j=1; j<=h->h1d(i,0)->GetNbinsX(); j++) if (j%5!=0) h->h1d(i,0)->GetXaxis()->SetBinLabel(j, ""); */
  /*       h->h1d(i,0)->GetYaxis()->SetRangeUser(0.99,1.002); */
  /*       h->h1d(i,0)->SetMarkerStyle(8); */
  /*       h->h1d(i,0)->SetMarkerSize(1.0); */
  /*     } */
  /*     for (int i=0; i<2; i++) { */
  /*       TCanvas* can = default_can_(h->histoname()+postfix_.det[i+1], "Eff - Run - "+postfix_.Det[i+1], 1030,600, 10,3,10,15, 0,0, 0,1); */
  /*       if (i==0)	multidraw_with_legend_(h, "3-5,", "P", year_+" Data", postfix_.Det, run_col1_, 0.2,0.4, 0.2,0.4); */
  /*       else	multidraw_with_legend_(h, "6-9,", "P", year_+" Data", postfix_.Det, run_col2_, 0.2,0.4, 0.2,0.4); */
  /*       c.push_back(can); */
  /*     } */
  /*   } */
  
  void run_can_(std::vector<TCanvas*>& c, Histograms* h) {
    dress_all_(h,"Run Number",1.5,1.0);
    for (int i=0; i<10; i++) {
      for (int j=0; j<5; j++) {
	for (int k=0; k<12; k++) {
/* 	  for (int l=1; l<=h->h1d(i,j,k)->GetNbinsX(); l++) if (l%5!=0) h->h1d(i,j,k)->GetXaxis()->SetBinLabel(l, ""); */
	  if (k==0) h->h1d(i,j,k)->GetYaxis()->SetRangeUser(0.99,1.002);
	  if (k==4) h->h1d(i,j,k)->GetYaxis()->SetRangeUser(25.0,30.0);
	  if (k==5) h->h1d(i,j,k)->GetYaxis()->SetRangeUser(20.0,25.0);
	  if (k==8) h->h1d(i,j,k)->GetYaxis()->SetRangeUser(0.4,0.9);
	  if (k==9) h->h1d(i,j,k)->GetYaxis()->SetRangeUser(6.5,9.5);
	  h->h1d(i,j,k)->SetMarkerStyle(8);
	  h->h1d(i,j,k)->SetMarkerSize(1.0);
	}
      }
    }
    for (int i=0; i<2; i++) {
      TCanvas* can = default_can_(h->histoname()+postfix_.det[i+1], "Eff - Run - "+postfix_.Det[i+1], 1030,600, 10,3,10,15, 0,0, 0,1);
      if (i==0)	multidraw_with_legend_(h, "3-5,", 0, 0, "PE1", year_+" Data", postfix_.Det, run_col1_, 0.7,0.9, 0.7,0.9);
      else	multidraw_with_legend_(h, "6-9,", 0, 0, "PE1", year_+" Data", postfix_.Det, run_col2_, 0.2,0.4, 0.2,0.4);
      c.push_back(can);
    }
    for (int i=4; i<10; i++) {
      TCanvas* can = default_can_(h->histoname()+postfix_.det[1]+postfix_.def[i], "Eff - Run - "+postfix_.Det[1]+postfix_.def[i], 1030,600, 10,3,10,15, 0,0, 0,1);
      float xadd=0.0; if (i==4||i==5) xadd = 0.55;
      if (i!=6&&i!=7) {
	multidraw_with_legend_(h, "3-5,", 0, i, "PX0", year_+" Data", postfix_.Det, run_col1_, 0.2+xadd,0.4+xadd, 0.2,0.4);
	/*       else multidraw_with_legend_(h, "3,", 0, i, "PX0", year_+" Data", postfix_.Det, run_col1_, 0.2,0.4, 0.2,0.4); */
	c.push_back(can);
      }
    }
    TCanvas* can = default_can_(h->histoname()+"_l1_mpv_mod", "Eff - Run - L1 MPV (Modules)", 1030,600, 10,3,10,15, 0,0, 0,1);
    multidraw_with_legend_(h, 3, "1-4,", 5, "PX0", "Layer 1", postfix_.Mod2, run_col2_, 0.2+0.55,0.4+0.55, 0.2,0.4);
    c.push_back(can);
  }

  // void totlumi_can_(std::vector<TCanvas*>& c, Histograms* h) {
  //   dress_all_(h,"Total Integrated Luminosity [/pb]",1.5,1.0);
  //   for (int i=0; i<10; i++) {
  //     for (int k=0; k<12; k++) {
  //       if (k==0) h->h1d(i,k)->GetYaxis()->SetRangeUser(0.99,1.002);
  //       if (k==4) h->h1d(i,k)->GetYaxis()->SetRangeUser(26.0,30.0);
  //       if (k==5) h->h1d(i,k)->GetYaxis()->SetRangeUser(20.0,25.0);
  //       if (k==8) h->h1d(i,k)->GetYaxis()->SetRangeUser(0.4,0.9);
  //       if (k==9) h->h1d(i,k)->GetYaxis()->SetRangeUser(6.5,9.5);
  //       h->h1d(i,k)->SetMarkerStyle(8);
  //       h->h1d(i,k)->SetMarkerSize(1.0);
  //     }
  //   }
  //   for (int i=0; i<2; i++) {
  //     TCanvas* can = default_can_(h->histoname()+postfix_.det[i+1], "Eff - Run - "+postfix_.Det[i+1], 1030,600, 10,3,10,15, 0,0, 0,1);
  //     if (i==0)	multidraw_with_legend_(h, "3-5,", "PE1", year_+" Data", postfix_.Det, run_col1_, 0.7,0.9, 0.7,0.9);
  //     else	multidraw_with_legend_(h, "6-9,", "PE1", year_+" Data", postfix_.Det, run_col2_, 0.2,0.4, 0.2,0.4);
  //     c.push_back(can);
  //   }
  //   for (int i=4; i<10; i++) {
  //     TCanvas* can = default_can_(h->histoname()+postfix_.def[i], "Eff - Run - "+postfix_.def[i], 1030,600, 10,3,10,15, 0,0, 0,1);
  //     float xadd=0.0; if (i==4||i==5||i==8) xadd = 0.55;
  //     if (i!=6&&i!=7) {
  //       std::string opt = "P";
  //       if (i==5) opt = "PEX0";
  //       multidraw_with_legend_(h, "3-5,", i, opt, year_+" Data", postfix_.Det, run_col1_, 0.2+xadd,0.4+xadd, 0.2,0.4);
  //       c.push_back(can);
  //     }
  //   }
  //   std::vector<TH1D*> hist;
  //   hist.push_back(getslice_(h->h2d(3,1),"",1));
  //   hist.push_back(getslice_(h->h2d(3,1),"",7));
  //   hist.push_back(getslice_(h->h2d(3,1),"",14));
  //   hist.push_back(getslice_(h->h2d(3,1),"",21));
  //   hist[0]->SetLineColor(3);
  //   hist[1]->SetLineColor(4);
  //   hist[2]->SetLineColor(6);
  //   hist[3]->SetLineColor(2);
  //   TCanvas* can = default_can_("norm_pcd", "normalized pixel charge distribution", 1030,600, 12,3,10,15, 0,0, 0,1);
  //   for (size_t i=0; i<hist.size(); i++) {
  //     setstyle_(hist[i],"Pixel Charge [ke]","Arbitrary",1.0,1.2);
  //     hist[i]->GetYaxis()->SetTitle("Arbitrary");
  //     if (i==0) hist[i]->DrawNormalized();
  //     if (i>0) hist[i]->DrawNormalized("same");
  //   }
  //   TLegend* leg = new TLegend(0.523,0.267,0.723,0.467,"Total Int Lumi (2011) - L1");
  //   leg->AddEntry(hist[0], "0.1 /fb", "l");
  //   leg->AddEntry(hist[1], "0.7 /fb", "l");
  //   leg->AddEntry(hist[2], "1.4 /fb", "l");
  //   leg->AddEntry(hist[3], "2.1 /fb", "l");
  //   leg->SetFillColor(0);
  //   leg->SetFillStyle(0);
  //   leg->SetBorderSize(0);
  //   leg->SetTextSize(0.04);
  //   leg->Draw();
  //   c.push_back(can);
  // 
  //   std::vector<TH1D*> hist2;
  //   hist2.push_back(getslice_(h->h2d(3,2),"",1));
  //   hist2.push_back(getslice_(h->h2d(3,2),"",7));
  //   hist2.push_back(getslice_(h->h2d(3,2),"",14));
  //   hist2.push_back(getslice_(h->h2d(3,2),"",21));
  //   hist2[0]->SetLineColor(3);
  //   hist2[1]->SetLineColor(4);
  //   hist2[2]->SetLineColor(6);
  //   hist2[3]->SetLineColor(2);
  //   can = default_can_("gc_pcd", "gain corrected pcd", 1030,600, 12,3,10,15, 0,0, 0,1);
  //   for (size_t i=0; i<hist2.size(); i++) {
  //     setstyle_(hist2[i],"Corrected Pixel Charge [ke]","Arbitrary",1.0,1.2);
  //     hist2[i]->GetYaxis()->SetTitle("Arbitrary");
  //     if (i==0) hist2[i]->DrawNormalized();
  //     if (i>0) hist2[i]->DrawNormalized("same");
  //   }
  //   TLegend* leg2 = new TLegend(0.523,0.267,0.723,0.467,"Total Int Lumi (2011) - L1");
  //   leg2->AddEntry(hist2[0], "0.1 /fb", "l");
  //   leg2->AddEntry(hist2[1], "0.7 /fb", "l");
  //   leg2->AddEntry(hist2[2], "1.4 /fb", "l");
  //   leg2->AddEntry(hist2[3], "2.1 /fb", "l");
  //   leg2->SetFillColor(0);
  //   leg2->SetFillStyle(0);
  //   leg2->SetBorderSize(0);
  //   leg2->SetTextSize(0.04);
  //   leg2->Draw();
  //   c.push_back(can);
  // 
  //   std::vector<TH1D*> hist3;
  //   hist3.push_back(getslice_(h->h2d(3,3),"",1));
  //   hist3.push_back(getslice_(h->h2d(3,3),"",7));
  //   hist3.push_back(getslice_(h->h2d(3,3),"",14));
  //   hist3.push_back(getslice_(h->h2d(3,3),"",21));
  //   hist3[0]->SetLineColor(3);
  //   hist3[1]->SetLineColor(4);
  //   hist3[2]->SetLineColor(6);
  //   hist3[3]->SetLineColor(2);
  //   can = default_can_("aoid", "aoid", 1030,600, 12,3,10,15, 0,0, 0,1);
  //   for (size_t i=0; i<hist3.size(); i++) {
  //     setstyle_(hist3[i],"Angle of Incidence","Arbitrary",1.0,1.2);
  //     hist3[i]->GetYaxis()->SetTitle("Arbitrary");
  //     if (i==0) hist3[i]->DrawNormalized();
  //     if (i>0) hist3[i]->DrawNormalized("same");
  //   }
  //   TLegend* leg3 = new TLegend(0.523,0.267,0.723,0.467,"Total Int Lumi (2011) - L1");
  //   leg3->AddEntry(hist3[0], "0.1 /fb", "l");
  //   leg3->AddEntry(hist3[1], "0.7 /fb", "l");
  //   leg3->AddEntry(hist3[2], "1.4 /fb", "l");
  //   leg3->AddEntry(hist3[3], "2.1 /fb", "l");
  //   leg3->SetFillColor(0);
  //   leg3->SetFillStyle(0);
  //   leg3->SetBorderSize(0);
  //   leg3->SetTextSize(0.04);
  //   leg3->Draw();
  //   c.push_back(can);
  // 
  // }
  

  void totlumi_can_(std::vector<TCanvas*>& c, Histograms* h) {
    int max = 2;
    for (int i=1; i<h->h1d(0,1)->GetNbinsX(); i++) {
      if (h->h1d(0,1)->GetBinContent(i)>0) max = i;
      for (int j=0; j<10; j++) h->h1d(j,4)->SetBinError(i,0);
    }
    for (int i=0; i<10; i++) for (int j=0; j<10; j++) h->h1d(i,j)->GetXaxis()->SetRange(1,max);
    // Rebin efficiency
    for (int i=0; i<10; i++) {
      h->rebin_eff(h->h1d(i,0),h->h1d(i,1),2);
      h->h1d(i,0)->GetXaxis()->SetRange(1,max/2);
    }
    h->h1d(2,0)->GetYaxis()->SetRangeUser(0.98,1.005);
    c.push_back(custom_can_(h->h1d(2,0), h->histoname()+"_eff", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "Efficiency",  1,1));
    h->multidraw_with_legend("2-5", 0, "PE1", postfix_.Det, "1,2,4,3", "", 0.2,0.4, 0.15,0.35);

    h->h1d(3,2)->GetYaxis()->SetRangeUser(18.0,25.0);
    c.push_back(custom_can_(h->h1d(3,2), h->histoname()+"_bpix_mpv", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "MPV of Normalized Cluster Charge",  1,1));
    h->multidraw_with_legend("3-5", 2, "PE1", postfix_.Det, "2,4,3", "", 0.4,0.6, 0.4,0.6);
    h->h1d(6,2)->GetYaxis()->SetRangeUser(16.0,21.0);
    c.push_back(custom_can_(h->h1d(6,2), h->histoname()+"_fpix_mpv", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "MPV of Normalized Cluster Charge",  1,1));
    h->multidraw_with_legend("6-9", 2, "PE1", postfix_.Det, "3,4,6,2", "", 0.4,0.6, 0.55,0.75);

    h->h1d(3,4)->GetYaxis()->SetRangeUser(2.5,5.5);
    c.push_back(custom_can_(h->h1d(3,4), h->histoname()+"_bpix_size", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "Average Cluster Size",  1,1));
    h->multidraw_with_legend("3-5", 4, "P", postfix_.Det, "2,4,3", "", 0.4,0.6, 0.75,0.95);
    h->h1d(6,4)->GetYaxis()->SetRangeUser(1.5,2.0);
    c.push_back(custom_can_(h->h1d(6,4), h->histoname()+"_fpix_size", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "Average Cluster Size",  1,1));
    h->multidraw_with_legend("6-9", 4, "P", postfix_.Det, "3,4,6,2", "", 0.4,0.6, 0.7,0.9);

    h->h1d(3,6)->GetYaxis()->SetRangeUser(1.7,2.0);
    h->h1d(3,8)->GetYaxis()->SetRangeUser(4.5,5.3);
    c.push_back(custom_can_(h->h1d(2,6), h->histoname()+"_size_leta", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "Average Cluster Size (|#eta| < 0.1) [Pixel]",  1,1));
    h->multidraw_with_legend("3-5", 6, "P", postfix_.Det, "2,4,3", "", 0.2,0.4, 0.6,0.8);
    c.push_back(custom_can_(h->h1d(2,8), h->histoname()+"_size_heta", "", "Total Integrated Luminosity - 2010-12 [fb^{-1}]", "Average Cluster Size (1.5 < |#eta| < 1.6) [Pixel]",  1,1));
    h->multidraw_with_legend("3-5", 8, "P", postfix_.Det, "2,4,3", "", 0.2,0.4, 0.6,0.8);
  }
  


  // void trigger_can_(std::vector<TCanvas*>& c, Histograms* h) {
  //   dress_all_(h,"",1.0,1.4);
  //   for (int j=0; j<4; j++) {
  //     for (int i=0; i<10; i++) {
  //       h->h1d(j,i,0)->GetYaxis()->SetRangeUser(0.98,1.005);
  //       h->h1d(j,i,0)->GetXaxis()->SetRange(2,3500);
  //       h->h1d(j,i,0)->SetMarkerStyle(8);
  //       h->h1d(j,i,0)->SetMarkerSize(1.0);
  //     }
  //   }
  //   TCanvas* can = default_can_(h->histoname(), "Effloss - Intlumi Fill 2040,2103,2105", 650,600, 14,10,9,11, 1,0, 1,1);
  //   multidraw_with_legend_(h, "1-3,", 3, 0, "PE1", "", postfix_.Fill, defcol_, 0.2,0.4, 0.2,0.4); c.push_back(can);
  // }


  void il_can_(std::vector<TCanvas*>& c, Histograms* h, int f, std::string& fill, std::string& Fill) {
    int min=0; int max=h->h1d(f,2,0)->GetNbinsX();
    for (int binx=1; binx<=h->h1d(f,2,0)->GetNbinsX(); binx++) {
      double stat = h->h1d(f,2,1)->GetBinContent(binx);
      if (stat>0) { max = binx;	if (min==0) min=binx; } }
    if (min!=0) h->h1d(f,2,0)->GetXaxis()->SetRange(min,max+1);
    h->h1d(f,2,0)->GetYaxis()->SetRangeUser(0.98,1.005);
    c.push_back(custom_can_(h->h1d(f,2,0), h->histoname()+fill, "2010-2011 Collisions Data, #sqrt{s} = 7 TeV", "Instantaneous Luminosity [#mub^{-1}s^{-1}]","Efficiency", 1,1));
    h->multidraw_with_legend_(f, "2-5", 0, "PE1", postfix_.Det, "1,2,4,3", "", 0.2, 0.4, 0.15, 0.35);
    // Additionally RAW layer efficiency plot
    if (std::string(h->histoname()).find("_raw")!=std::string::npos) {
      TH1D* lay_raw = new TH1D(std::string("lay_raw"+fill).c_str(),std::string("lay_raw"+fill).c_str(),7,0.5,7.5);
      for (int i=1; i<=7; i++) {
	double val = 0; double mis = 0;
	for (int binx=0; binx<=h->h1d(f,2+i,0)->GetNbinsX()+1; binx++) {
	  double eff = h->h1d(f,2+i,0)->GetBinContent(binx);
	  double dist = h->h1d(f,2+i,1)->GetBinContent(binx);
	  val+=eff*dist; mis+=(1-eff)*dist; 
	}
	// error bar (Wilson Score Interval) - z=1
	double p = val/(val+mis);
	double n = val+mis;
	double cen = (p+1/(2*n)) / (1.0 + 1/n);
	double halfwidth = sqrt( p*(1.0-p)/n + 1/(4*n*n) ) / (1.0 + 1/n);
	double err = halfwidth + fabs(cen-p); // Assymmetric error -> Choose larger for a conservative error estimate
	lay_raw->SetBinContent(i,p);
	lay_raw->SetBinError(i,err);
      }
      lay_raw->SetEntries(h->h1d(f,0,1)->GetEntries());
      c.push_back(custom_can_(lay_raw, "lay_raw"+fill, "2010-2011 Collisions Data, #sqrt{s} = 7 TeV", "", "Efficiency", 0, 0, 480,480, 90));
      lay_raw->SetMarkerStyle(8); lay_raw->SetMarkerSize(1.3);
      lay_raw->GetXaxis()->SetBinLabel(1, "Layer 1");
      lay_raw->GetXaxis()->SetBinLabel(2, "Layer 2");
      lay_raw->GetXaxis()->SetBinLabel(3, "Layer 3");
      lay_raw->GetXaxis()->SetBinLabel(4, "Disk -2");
      lay_raw->GetXaxis()->SetBinLabel(5, "Disk -1");
      lay_raw->GetXaxis()->SetBinLabel(6, "Disk +1");
      lay_raw->GetXaxis()->SetBinLabel(7, "Disk +2");
      lay_raw->GetXaxis()->LabelsOption("h");
      lay_raw->GetXaxis()->SetLabelSize(0.045);
      lay_raw->GetYaxis()->SetRangeUser(0.98,1.005);
      lay_raw->Draw("PE1X0");
    }
  }

  void il_l1rate_can_(std::vector<TCanvas*>& c, Histograms* h, int f, std::string& fill, std::string& Fill) {
    for (int i=0; i<10; i++) {
      setstyle_(h->h2d(f,i,0),"Instantaneous Luminosity [#mub^{-1}s^{-1}]","L1 Trigger Rate [Hz]","Efficiency",0.9,1.0,1.2);
      h->h2d(f,i,0)->GetZaxis()->SetRangeUser(0.98,1.00);
    }
    TCanvas* can = default_can_(h->histoname()+fill, "Effloss - Instlumi, L1 Rate - Layer 1 - Fill "+Fill, 650,600, 12,17,10,10, 1,0, 1,1);
    h->h2d(f,3,0)->Draw("COLZ");
    c.push_back(can);
  }

  void bx_can_(std::vector<TCanvas*>& c, Histograms* h, int f, std::string& fill, std::string& Fill) {
    for (int i=0; i<10; i++) {
      setstyle_(h->h1d(f,i,0),"Bunch crossing","Efficiency",0.9,1.0);
      h->h1d(f,i,0)->GetYaxis()->SetRangeUser(0.99,1.002);
      h->h1d(f,i,0)->GetYaxis()->SetTitle("Efficiency / 60 bunch");
      h->h1d(f,i,0)->SetMarkerStyle(8);
      h->h1d(f,i,0)->SetMarkerSize(1.0);
    }
    TCanvas* can = default_can_(h->histoname()+fill, "Effloss - Bx - Fill "+Fill, 1030,600, 10,3,10,10, 1,0, 1,1);
    multidraw_with_legend_(h, f, "2-5,", 0, "PE1", "", postfix_.Det, defcol_, 0.2,0.4, 0.225,0.425); c.push_back(can);
  }
  
  void time_can_(std::vector<TCanvas*>& c, Histograms* h, int f, std::string& fill, std::string& Fill) {
    for (int i=0; i<10; i++) {
      setstyle_(h->h1d(f,i,0),"Time Since Start of Fill [h]","Efficiency",1.0,1.2);
      h->h1d(f,i,0)->GetYaxis()->SetRangeUser(0.98,1.0);
      /*       h->h1d(f,i,0)->GetYaxis()->SetRangeUser(0.95,1.0); */
      /*       h->h1d(f,i,0)->GetXaxis()->SetRangeUser(0.0,8.99); */
      h->h1d(f,i,0)->SetMarkerStyle(8);
      h->h1d(f,i,0)->SetMarkerSize(1.0);
    }
    TCanvas* can = default_can_(h->histoname()+fill, "Eff - Time - Fill "+Fill, 900,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, "2-5,", 0, "PE1", "", postfix_.Det, defcol_, 0.2,0.4, 0.2,0.4);
    std::string fill_lat = "Fill "+Fill;
    draw_lat_(0, 1.001, fill_lat.c_str(), 1, 0.04, 0.0, 12);
    /*     std::string fill_lat = "Fill 2208"; */
    /*     draw_lat_(0, 1.0025, fill_lat.c_str(), 1, 0.04, 0.0, 12); */
    c.push_back(can);
  }

  void time2_can_(std::vector<TCanvas*>& c, Histograms* h, Variables& v, PostFixes& p) {
    for (int i=0; i<10; i++) {
      h->h1d(1,i,0)->GetXaxis()->SetRangeUser(0,12.99);
      h->h1d(1,i,0)->GetYaxis()->SetRangeUser(0.99,1.00);
      c.push_back(custom_can_(h->h1d(1,i,0), "latency"+p.det[i], "", "Time Since Start of Fill [h]", "Efficiency",  0,0));
      h->multidraw_with_legend("1,0", i, 0, "PE1", p.Spec, "4,2", p.Det[i], 0.4,0.6, 0.15,0.35);
    }
  }

  void federr_can_(std::vector<TCanvas*>& c, Histograms* h, int f, std::string& fill, std::string& Fill, int norm) {
    if (norm==1) { 
      setstyle_(h->h1d(f,0),"Error Type","N_{FED Error}/N_{Event}",1.5,1.4);
      h->h1d(f,0)->Scale(1/(h->h1d(f,0)->GetBinContent(1)));
    } else {
      setstyle_(h->h1d(f,0),"Error Type","Efficiency",1.5,1.4);
      h->h1d(f,0)->GetYaxis()->SetRangeUser(0.0,1.0);
      h->h1d(f,0)->SetMarkerStyle(8);
      h->h1d(f,0)->SetMarkerSize(1.0);
    }
    h->h1d(f,0)->GetXaxis()->SetBinLabel(27, "ROC of 25");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(28, "Gap word");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(29, "Dummy word");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(30, "FIFO full");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(31, "Timeout");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(32, "TBM Trailer");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(33, "Event number");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(34, "Slink header");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(35, "Slink trailer");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(36, "Event size");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(37, "Invalid channel#");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(38, "ROC value");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(39, "Dcol or pixel value");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(40, "Readout order");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(41, "CRC error");
    h->h1d(f,0)->GetXaxis()->SetBinLabel(42, "Overflow");
    h->h1d(f,0)->GetXaxis()->SetRange(27,42);
    float max = ((norm) ? h->h1d(f,0)->GetMaximum() : 1) * 1.05;
    TCanvas* can = default_can_(h->histoname()+fill, "Effloss - Fed error - Fill "+Fill, 650,600, 15,10,5,15, 0,0, 0,0);
    if (norm!=1) draw_with_lat_(h->h1d(f,0),"PE1",1,"Fill "+Fill,26.5,max);
    else draw_with_lat_(h->h1d(f,0),"",1,"Fill "+Fill,26.5,max);
    c.push_back(can);
  }

  void time_federr_can_(std::vector<TCanvas*>& c, Histograms* h, int f, std::string& fill, std::string& Fill) {
    std::stringstream ss0;
    std::stringstream ss2;
    std::stringstream ss3;
    std::stringstream ss5;
    std::stringstream ss6;
    float max2 = 0;
    float max3 = 0;
    float max3a = 0;
    float max3b = 0;
    float max3c = 0;
    float max5 = 0;
    float max6 = 0;
    std::string fill_lat = "Fill "+Fill;
    for (int type=0; type<20; type++) {
      for (int det=0; det<10; det++) {
	setstyle_(h->h1d(f,type,det,0),"Time Since Start of Fill [h]","Efficiency",1.0,1.2);
	if (type<18) {
	  setstyle_(h->h1d(f,type,det,2),"Time Since Start of Fill [h]","#splitline{ Module Error Rate }{ All Events}",1.0,2.0);
	  setstyle_(h->h1d(f,type,det,3),"Time Since Start of Fill [h]","#splitline{ Fraction of Affected Hits }{ - in Events with FED error}",1.0,2.0);
	  for (int i=0; (det>0) ? i<5 : i<7; i++) {
	    h->h1d(f,type,det,i)->SetMarkerStyle(8);
	    h->h1d(f,type,det,i)->SetMarkerSize(1.0);
	  }
	  if (det==0&&type>2)
	    h->h1d(f,type,det,0)->GetYaxis()->SetRangeUser(0,1.2);
	  else
	    h->h1d(f,type,det,0)->GetYaxis()->SetRangeUser(0.95,1.0);
	  if (det>=2&&det<=5&&h->h1d(f,type,det,3)->GetMaximum()>max3a&&type==2) max3a = 1.05 * h->h1d(f,type,det,3)->GetMaximum();
	  if (det>=2&&det<=5&&h->h1d(f,type,det,3)->GetMaximum()>max3b&&type==7) max3b = 1.05 * h->h1d(f,type,det,3)->GetMaximum();
	  if (det>=2&&det<=5&&h->h1d(f,type,det,3)->GetMaximum()>max3c&&type==8) max3c = 1.05 * h->h1d(f,type,det,3)->GetMaximum();
	} else {
	  h->h1d(f,type,det,0)->SetMarkerStyle(8);
	  h->h1d(f,type,det,0)->SetMarkerSize(1.0);
	  h->h1d(f,type,det,0)->GetYaxis()->SetRangeUser(0.95,1.0);
	}
      }
      if (type<18) {
	setstyle_(h->h1d(f,type,0,5),"Time Since Start of Fill [h]","N_{FED Error}/N_{Event}",1.0,1.2);
	setstyle_(h->h1d(f,type,0,6),"Time Since Start of Fill [h]","N_{FED Error}/N_{Event with Same Error}",1.0,1.2);
	if (h->h1d(f,type,0,1)->GetEntries()>0&&type>2) ss0<<type<<",";
	if (h->h1d(f,type,0,2)->GetEntries()>0&&type>2) ss2<<type<<",";
	if (h->h1d(f,type,0,3)->GetEntries()>0&&type>2) ss3<<type<<",";
	if (h->h1d(f,type,0,5)->GetEntries()>0&&type>1) ss5<<type<<",";
	if (h->h1d(f,type,0,6)->GetEntries()>0&&type>1) ss6<<type<<",";
	if (h->h1d(f,type,3,2)->GetMaximum()>max2&&type>2) max2 = 1.05 * h->h1d(f,type,3,2)->GetMaximum();
	if (h->h1d(f,type,3,3)->GetMaximum()>max3&&type>2) max3 = 1.05 * h->h1d(f,type,3,3)->GetMaximum();
	if (h->h1d(f,type,0,5)->GetMaximum()>max5&&type>1) max5 = 1.05 * h->h1d(f,type,0,5)->GetMaximum();
	if (h->h1d(f,type,0,6)->GetMaximum()>max6&&type>2) max6 = 2 * h->h1d(f,type,0,6)->GetMaximum();
      }
    }
    for (int det=2; det<=5; det++) {
      h->h1d(f,2,det,3)->GetYaxis()->SetRangeUser(0,max3a);
      h->h1d(f,7,det,3)->GetYaxis()->SetRangeUser(0,max3b);
      h->h1d(f,8,det,3)->GetYaxis()->SetRangeUser(0,max3c);
    }
    for (int type=2; type<18; type++) {
      h->h1d(f,type,0,2)->GetYaxis()->SetRangeUser(0,max2);
      h->h1d(f,type,0,2)->GetYaxis()->SetDecimals(1);
      h->h1d(f,type,0,3)->GetYaxis()->SetRangeUser(0,max3);
      h->h1d(f,type,0,5)->GetYaxis()->SetRangeUser(0,max5);
      h->h1d(f,type,0,6)->GetYaxis()->SetRangeUser(0,max6);
    }
    // Layer 1 - (Errors) - Err Rate/Frac (per hit)
    TCanvas* can = default_can_(h->histoname()+fill+"_moderrrate", "Effloss - Module FED Error Event Rate vs Time - FED Errors - Fill "+Fill, 700,600, 20,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, ss2.str(), 3, 2, "P", "Layer 1", postfix_.Federr_type, defcol2_, 0.55,0.75, 0.7,0.9);
    draw_lat_(0, max2*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    can = default_can_(h->histoname()+fill+"_hitfrac", "Effloss - Fraction of Hits Affected by FED Error vs Time - FED Errors - Fill "+Fill, 700,600, 20,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, ss3.str(), 3, 3, "P", "Layer 1", postfix_.Federr_type, defcol2_, 0.55,0.75, 0.7,0.9);
    draw_lat_(0, max3*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    // // (Layers) - Spec Err - Err Frac (per hit)
    // can = default_can_(h->histoname()+fill+"_hitfrac_lay_allerr", "Effloss - Fraction of Hits Affected by FED Error vs Time - Layers - All Errors - Fill "+Fill, 700,600, 20,7,9,11, 1,0, 1,1);
    // multidraw_with_legend_(h, f, 2, "2-5,", 3, "P", "All FED Errors", postfix_.Det, defcol_, 0.55,0.75, 0.7,0.9);
    // draw_lat_(0, max3a*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    // can = default_can_(h->histoname()+fill+"_hitfrac_lay_timeout", "Effloss - Fraction of Hits Affected by FED Error vs Time - Layers - Timeout - Fill "+Fill, 700,600, 20,7,9,11, 1,0, 1,1);
    // multidraw_with_legend_(h, f, 7, "2-5,", 3, "P", "Timeout", postfix_.Det, defcol_, 0.55,0.75, 0.7,0.9);
    // draw_lat_(0, max3b*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    // can = default_can_(h->histoname()+fill+"_hitfrac_lay_overflow", "Effloss - Fraction of Hits Affected by FED Error vs Time - Layers - Overflow - Fill "+Fill, 700,600, 20,7,9,11, 1,0, 1,1);
    // multidraw_with_legend_(h, f, 8, "2-5,", 3, "P", "Overflow", postfix_.Det, defcol_, 0.55,0.75, 0.7,0.9);
    // draw_lat_(0, max3c*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    // FED Error Rate/Multiplicity (per event)
    can = default_can_(h->histoname()+fill+"_evtrate", "Effloss - FED Error Event Rate vs Time - FED Errors - Fill "+Fill, 650,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, ss5.str(), 0, 5, "P", "", postfix_.Federr_type, defcol_, 0.55,0.75, 0.7,0.9);
    draw_lat_(0, max5*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    can = default_can_(h->histoname()+fill+"_evtmult", "Effloss - FED Error Event Multiplicity vs Time - FED Errors - Fill "+Fill, 650,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, ss6.str(), 0, 6, "PE1", "", postfix_.Federr_type, defcol_, 0.55,0.75, 0.7,0.9);
    draw_lat_(0, max6*1.05, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    // Layer Efficiency vs time plots - Event with/without FED Error - FED error on module - Layer Eff without excluding FED error
    can = default_can_(h->histoname()+fill+"_eff_errevt", "Effloss - Efficiency vs Time - FED Error Event - Layers - Fill "+Fill, 650,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, 18, "2-5,", 0, "PE1", "Event with FED Error", postfix_.Det, defcol_, 0.2,0.4, 0.2,0.4);
    draw_lat_(0, 1.0025, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    can = default_can_(h->histoname()+fill+"_eff_noerrevt", "Effloss - Efficiency vs Time - NO FED Errors Event - Layers - Fill "+Fill, 650,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, 19, "2-5,", 0, "PE1", "Event without FED Error", postfix_.Det, defcol_, 0.2,0.4, 0.2,0.4);
    draw_lat_(0, 1.0025, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    can = default_can_(h->histoname()+fill+"_eff_federr", "Effloss - Efficiency vs Time - FED Error on Module - Fill "+Fill, 650,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, ss0.str(), 0, 0, "PE1", "FED error on module - All Layers", postfix_.Federr_type, defcol2_, 0.2,0.4, 0.2,0.4);
    draw_lat_(0, 1.26, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
    can = default_can_(h->histoname()+fill+"_eff_layer", "Effloss - Efficiency  vs Time - Layers - Fill "+Fill, 650,600, 13,7,9,11, 1,0, 1,1);
    multidraw_with_legend_(h, f, 0, "2-5,", 0, "PE1", "Efficiency with FED errors", postfix_.Det, defcol_, 0.2,0.4, 0.2,0.4);
    draw_lat_(0, 1.0025, fill_lat.c_str(), 1, 0.04, 0.0, 12); c.push_back(can);
  }

  void fill_multican_(std::vector<TCanvas*>& c, AllHistos& ah, Variables& v, PostFixes& p) {
    for (int fill=0; fill<=v.lumi_nfill; fill++) {
      lay_can_(c, ah.det->h1d(fill,0), p.fill[fill], p.Fill[fill]);
      for (int i=0; i<4; i++) mod_can_(c, ah.mod->h2d(fill,i,0), i, p.fill[fill], p.Fill[fill]);
      for (int i=0; i<5; i++) roc_can_(c, ah.roc->h2d(fill,i,0), ah.roc->h2d(fill,i,1), ah.roc->h2d(fill,i,2), i, p.fill[fill], p.Fill[fill]);
      time_can_       (c, ah.time,         fill, p.fill[fill], p.Fill[fill]);
      il_can_         (c, ah.instlumi,     fill, p.fill[fill], p.Fill[fill]);
      il_can_         (c, ah.instlumi_raw, fill, p.fill[fill], p.Fill[fill]);
      il_l1rate_can_  (c, ah.il_l1rate,    fill, p.fill[fill], p.Fill[fill]);
      bx_can_         (c, ah.bx,           fill, p.fill[fill], p.Fill[fill]);
#if FEDERRPLOTS == 1
      federr_can_     (c, ah.federr_evt,   fill, p.fill[fill], p.Fill[fill], 1);
      federr_can_     (c, ah.federr,       fill, p.fill[fill], p.Fill[fill], 0);
      time_federr_can_(c, ah.time_federr,  fill, p.fill[fill], p.Fill[fill]);
#endif
    }
  }

  void instlumi_compare_can_(std::vector<TCanvas*>& c, Histograms* h, Variables& v, PostFixes& p) {
    for (int fill=1; fill<=v.lumi_nfill; fill++) for (int i=0; i<10; i++) {
      h->h1d(fill,i,0)->GetYaxis()->SetRangeUser(0.988,1.000);
      h->h1d(fill,i,0)->GetXaxis()->SetRangeUser(500,3600); 
    }
    for (int i=2; i<=5; i++) {
      TCanvas* can = square_can_1d_(h, "Instantaneous Luminosity [#mub^{-1}s^{-1}]", h->histoname()+"_fills"+p.det[i], "Effloss - Inst Lumi - Fill 2040,2103,2105,2208 - "+p.Det[i], 1, 1);
      h->multidraw_with_legend_("1,2,4,5", i, 0, "PE1", p.Fill2, "1,2,3,4", p.Det[i], 0.2,0.4, 0.2,0.4); c.push_back(can); 
    }
  }

  void occup_can_(std::vector<TCanvas*>& c, AllHistos& ah, Variables& v, PostFixes& p) {
    // Roc Occupancy
    //  - Npix
    ah.occup_roc->h1d(0, 2,0)->GetXaxis()->SetRange(2,30);
    ah.occup_roc->h1d(0, 2,0)->GetYaxis()->SetRangeUser(0.8,1.1);
    TCanvas* can = custom_can_(ah.occup_roc->h1d(0, 2,0), ah.occup_roc->histoname(), "", "ROC occupancy [pixel]", "Efficiency", 1,1);
    ah.occup_roc->multidraw_with_legend_(0, "2-5", 0, "PE1", p.Det, "1,2,4,3", "Fill 2178", 0.2, 0.4, 0.15, 0.35);
    c.push_back(can);
  }

  void nbadroc_can_(std::vector<TCanvas*>& c, Histograms* h, Variables& v, PostFixes& p) {
//     c.push_back(custom_can_(h->h1d(6,0),h->histoname(), "", "Time Since Start of Fill [h]", "No of ROCs under 10% Eff",  1,1));
//     h->multidraw_with_legend("6,7,9", 0, "P", p.Fill, "1,2,4,3", "Fill Number", 0.2,0.4, 0.6,0.8);
    // N_BadROC plot
    for (int fill=0; fill<v.lumi_nfill; fill++) {
      c.push_back(custom_can_(h->h1d(fill,0),h->histoname()+p.fill[fill+1], "2012 Data", "Time Since Start of Fill [h]", "No of ROCs under 10% Eff",  0,0));
      h->multidraw_with_legend(fill, "0", "P", p.Det, "1,2,4,3", "Fill "+p.Fill[fill+1], 0.2,0.4, 0.6,0.8);
      // // 2040
      // draw_line_(2.46, 0.0, 2.46, max, 1, 3);
      // draw_lat_(2.46, max/2.0, "#splitline{ PAUSE }{RESUME}", 1, 0.03, 90.0, 22);
      // // 2103
      // draw_line_(4.31, 0.0, 4.31, max);
      // /*     draw_lat_(4.31, max/2.0, "176202", 2, 0.03, 90.0, 12); */
      // draw_line_(6.26, 0.0, 6.26, max);
      // /*     draw_lat_(6.26, max/2.0, "176206", 2, 0.03, 90.0, 12); */
      // draw_line_(6.97, 0.0, 6.97, max);
      // /*     draw_lat_(6.97, max/2.0, "176207", 2, 0.03, 90.0, 12); */
      // // 2105
      // draw_line_(4.0, 0.0, 4.0, max, 3);
      // /*     draw_lat_(4.0, max/2.0, "176308", 3, 0.03, 90.0, 12); */
      // draw_line_(6.0, 0.0, 6.0, max, 3);
      // /*     draw_lat_(6.0, max/2.0, "176309", 3, 0.03, 90.0, 12); */
      // draw_line_(9.25, 0.0, 9.25, max, 3, 3);
      // draw_lat_(9.25, max/2.0, "#splitline{ PAUSE }{RESUME}", 3, 0.03, 90.0, 22);
      // draw_line_(14.4, 0.0, 14.4, max, 3, 3);
      // draw_lat_(14.4, max/2.0, "#splitline{ PAUSE }{RESUME}", 3, 0.03, 90.0, 22);
    }
  }

  void roceff_dist_can_(std::vector<TCanvas*>& c, Histograms* h, Variables& v, PostFixes& p) {
    // ROCeff Dist
    for (int fill=0; fill<v.lumi_nfill; fill++) {
      setstyle_(h->h1d(fill), "Efficiency", "Number of ROCs",1.0,1.0);
      TCanvas* can = default_can_(h->histoname()+p.fill[fill+1], "Effloss - ROCeff Distribution"+p.Fill[fill+1], 900,600, 10,10,9,11, 0,0, 0,0);
      // Fitting data
      std::string fname = "f"+p.fill[fill+1];
      TF1* f = h->h1d(fill)->GetFunction(fname.c_str());
      float cut=0.95;
      if (f!=0) {
	double mean = f->GetParameter(1);
	double sigma = f->GetParameter(2);
	if ((mean-10*sigma)>0.95) cut = mean-10*sigma;
      }
      if (cut>0.95) {
	h->h1d(fill)->GetXaxis()->SetRangeUser(0.95,1.0);
	draw_with_lat_(h->h1d(fill),"",1,"Fill "+p.Fill[fill+1],0.95,1.05);
      } else {
	h->h1d(fill)->GetXaxis()->SetRangeUser(0.9,1.0);
	draw_with_lat_(h->h1d(fill),"",1,"Fill "+p.Fill[fill+1],0.9,1.05);
      }
      double max = 1.05*h->h1d(fill)->GetMaximum();
      draw_line_(cut, 0.0, cut, max, 2);
      draw_lat_(cut-0.0002, max/2.0, "EXCLUDE", 2, 0.04, 90.0, 21);
      draw_lat_(cut+0.0002, max/2.0, "INCLUDE", 1, 0.04, 90.0, 23);
      if (f!=0) f->Draw("same");
      c.push_back(can);
    }
  }

  void badroc_can_(std::vector<TCanvas*>& c, Histograms* h, Variables& v, PostFixes& p) {
    // BAD ROC List
    for (int fill=0; fill<v.lumi_nfill; fill++) {
      for (int lay=0; lay<=3; lay++) {
	setstyle_(h->h2d(fill,lay),"Time Since Start of Fill [h]","","Efficiency",0.9,1.0,0.6);
	h->h2d(fill,lay)->GetYaxis()->SetLabelFont(100);
	TCanvas* can = default_can_(h->histoname()+p.fill[fill+1]+p.det[lay+2],"Effloss - Bad ROC List - "+p.Fill[fill+1]+" "+p.Det[lay+2], 1100,600, 30,10,10,10, 0,1, 0,1);
	h->h2d(fill,lay)->Draw("COLZ");
	if (h->h2d(fill,lay)->GetEntries()>0) {
	  gPad->Update();
	  TPaletteAxis* palette = (TPaletteAxis*)h->h2d(fill,lay)->GetListOfFunctions()->FindObject("palette");
	  palette->SetX2NDC(0.93);
	}
	// // 2040
	// if (fill==1) {
	//   draw_line_(2.46, 0.0, 2.46, max);
	//   draw_lat_(2.46, max/2.0, "#splitline{ PAUSE }{RESUME}", 0, 0.03, 90.0, 22);
	// }
	// // 2103
	// else if (fill==2) {
	/* 	double max = h->h2d(fill,lay)->GetYaxis()->GetXmax(); */
	/* 	draw_line_(4.31, 0.0, 4.31, max); */
	/* 	draw_lat_(4.31, max/2.0, "176202", 0, 0.03, 90.0, 12); */
	/* 	draw_line_(6.26, 0.0, 6.26, max); */
	/* 	draw_lat_(6.26, max/2.0, "176206", 0, 0.03, 90.0, 12); */
	/* 	draw_line_(6.97, 0.0, 6.97, max); */
	/* 	draw_lat_(6.97, max/2.0, "176207", 0, 0.03, 90.0, 12); */
	// }
	// // 2105
	// else if (fill==3) {
	//   draw_line_(4.0, 0.0, 4.0, max);
	//   draw_lat_(4.0, max/2.0, "176308", 0, 0.03, 90.0, 12);
	//   draw_line_(6.0, 0.0, 6.0, max);
	//   draw_lat_(6.0, max/2.0, "176309", 0, 0.03, 90.0, 12);
	// }
	c.push_back(can);
      }
    }
  }

  //_________________________________________________________________________________________________________
  //                                       Efficiency N-1 Cut Plots
  //_________________________________________________________________________________________________________
  
  void nstrip_can_(std::vector<TCanvas*>& c, Histograms* h) {
    TCanvas* can = square_can_1d_(h, "N_{Strip Hits}", h->histoname(), "N-1 - Eff vs Nstrip - layers", 1,1);
    h->h1d(2,0)->GetYaxis()->SetRangeUser(0.97,1.01);
    h->multidraw_with_legend_("2-5", 0, "PE1", postfix_.Det, "1,2,4,3", "", 0.6,0.8, 0.15,0.35);
    c.push_back(can);
    can = square_can_1d_(h, "N_{Strip Hits}", h->histoname()+"_dist", "N-1 - Dist vs Nstrip - layers", 1,1);
    h->multidraw_with_legend_("2-5", 1, "NORM", postfix_.Det, "1,2,4,3,", "", 0.5,0.7, 0.25,0.45);
    h->h1d(2,1)->GetYaxis()->SetRangeUser(0.00001,1.0); can->SetLogy(1);
    c.push_back(can);
  }

  void pt_can_(std::vector<TCanvas*>& c, Histograms* h) {
    TCanvas* can = square_can_1d_(h, "P_{T} [GeV]", h->histoname(), "N-1 - Eff vs Pt - layers", 1,1);
    h->h1d(2,0)->GetYaxis()->SetRangeUser(0.99,1.002);
    h->multidraw_with_legend_("2-5", 0, "PE1", postfix_.Det, "1,2,4,3", "", 0.6,0.8, 0.15,0.35);
    c.push_back(can);
    can = square_can_1d_(h, "P_{T} [GeV]", h->histoname()+"_dist", "N-1 - Dist vs Pt - layers", 1,1);
    h->multidraw_with_legend_("2-5", 1, "NORM", postfix_.Det, "1,2,4,3,", "", 0.65,0.85, 0.65,0.85);
    h->h1d(2,1)->GetYaxis()->SetRangeUser(0.00001,1.0); can->SetLogy(1);
    c.push_back(can);
  }

  void d0_can_(std::vector<TCanvas*>& c, Histograms* h) {
    TCanvas* can = square_can_1d_(h, "|D0| [cm]", h->histoname(), "N-1 - Eff vs D0 - layers", 1,1);
    h->h1d(2,0)->GetYaxis()->SetRangeUser(0.97,1.01);
    h->multidraw_with_legend_("2-5", 0, "PE1", postfix_.Det, "1,2,4,3", "", 0.6,0.8, 0.15,0.35);
    c.push_back(can);
    can = square_can_1d_(h, "|D0| [cm]", h->histoname()+"_dist", "N-1 - Dist  vs D0 - layers", 1,0);
    h->multidraw_with_legend_("2-5", 1, "NORM", postfix_.Det, "1,2,4,3", "", 0.65,0.85, 0.65,0.85);
    h->h1d(2,1)->GetYaxis()->SetRangeUser(0.00001,1.0); can->SetLogy(1);
    c.push_back(can);
  }

  void dz_can_(std::vector<TCanvas*>& c, Histograms* h) {
    TCanvas* can = square_can_1d_(h, "|DZ| [cm]", h->histoname(), "N-1 - DZ - layers", 1,1);
    h->h1d(2,0)->GetYaxis()->SetRangeUser(0.97,1.01);
    h->multidraw_with_legend_("2-5", 0, "PE1", postfix_.Det, "1,2,4,3", "", 0.6,0.8, 0.15,0.35);
    c.push_back(can);
    can = square_can_1d_(h, "|DZ| [cm]", h->histoname()+"_dist", "N-1 - Dist vs DZ - layers", 1,0);
    h->multidraw_with_legend_("2-5", 1, "NORM", postfix_.Det, "1,2,4,3", "", 0.65,0.85, 0.65,0.85);
    h->h1d(2,1)->GetYaxis()->SetRangeUser(0.00001,1.0); can->SetLogy(1);
    c.push_back(can);
  }

  void fid_can_(std::vector<TCanvas*>& c, Histograms* lxly, Histograms* lx, Histograms* ly) {
    dress_all_(lxly,"Local Y [cm]","Local X [cm]",1.0,1.0,1.0);
    dress_all_(lx,"Local X [cm]",1.0,1.0);
    dress_all_(ly,"Local Y [cm]",1.0,1.0);
    for (int i=3; i<10; i++) {
      if (i<6) {
	for (int j=0; j<2; j++) {
	  TCanvas* can = default_can_("fid"+postfix_.det[i]+postfix_.mod[j],"N-1 - Fid - "+postfix_.Det[i]+" "+postfix_.Mod[j],1200,800, 12,12,10,10, 0,0, 0,0);
	  can->Divide(1,2); can->cd(1);
	  lxly->h2d(i-3,j,0)->GetZaxis()->SetRangeUser(0.99,1.00);
	  lxly->h2d(i-3,j,0)->Draw("COLZ");
	  TVirtualPad* can_2 = can->cd(2); can_2->Divide(2); can_2->cd(1);
	  lx->h1d(i-3,j,0)->GetYaxis()->SetRangeUser(0.98,1.005);
	  lx->h1d(i-3,j,0)->Draw();
	  can_2->cd(2);
	  ly->h1d(i-3,j,0)->GetYaxis()->SetRangeUser(0.98,1.005);
	  ly->h1d(i-3,j,0)->Draw();
	  c.push_back(can);
	}
      }	else {
	for (int j=2; j<9; j++) {
	  TCanvas* can = default_can_("fid"+postfix_.det[i]+postfix_.mod[j],"N-1 - Fid - "+postfix_.Det[i]+" "+postfix_.Mod[j],1200,800, 12,12,10,10, 0,0, 0,0);
	  can->Divide(1,2); can->cd(1);
	  lxly->h2d(i-3,j-2,0)->GetZaxis()->SetRangeUser(0.99,1.00);
	  lxly->h2d(i-3,j-2,0)->Draw("COLZ");
	  TVirtualPad* can_2 = can->cd(2); can_2->Divide(2); can_2->cd(1);
	  lx->h1d(i-3,j-2,0)->GetYaxis()->SetRangeUser(0.98,1.005);
	  lx->h1d(i-3,j-2,0)->Draw();
	  can_2->cd(2);
	  ly->h1d(i-3,j-2,0)->GetYaxis()->SetRangeUser(0.98,1.005);
	  ly->h1d(i-3,j-2,0)->Draw();
	  c.push_back(can);
	}
      }
    }
  }
  
  void cut_lines_(float cut1, float cut2, int lxly, float repl, int swap = 0, int style = 9) {
    float low = 0.98;
    float high = 1.005;
    if (lxly) {
      low = -1.0 * repl;
      high = repl;
    }
    if (swap==0) {
      draw_line_(cut1, low, cut1, high, 1+(lxly==0), style, 1+(lxly>0));
      draw_line_(cut2, low, cut2, high, 1+(lxly==0), style, 1+(lxly>0));
    } else {
      draw_line_(low, cut1, high, cut1, 1+(lxly==0), style, 1+(lxly>0));
      draw_line_(low, cut2, high, cut2, 1+(lxly==0), style, 1+(lxly>0));
    }
  }

  void roccut_lines_(int nrocly, int lxly, float repl, int swap = 0) {
    float low = 0.98;
    float high = 1.005;
    if (lxly) {
      low = -1.0 * repl;
      high = repl;
    }    
    for (int i=0; i<nrocly-1; i++) {
      float rocedge = (i-nrocly/2.0+1.0) * 0.81;
      if (swap==0) {
	draw_line_(rocedge - 0.06, low, rocedge - 0.06, high, 1, 2, 1+(lxly>0));
	draw_line_(rocedge + 0.06, low, rocedge + 0.06, high, 1, 2, 1+(lxly>0));
      } else {
	draw_line_(low, rocedge - 0.06, high, rocedge - 0.06, 1, 2, 1+(lxly>0));
	draw_line_(low, rocedge + 0.06, high, rocedge + 0.06, 1, 2, 1+(lxly>0));
      }
    }
  }

  void fid_lx_cut_lines_(int i, int j, float ly_range, int lxly = 0) {
    switch (i) {
    case 3: case 4: case 5: // Layer 1-2-3
      if (j==0) { // Full Modules
	// cut_lines_(-0.6,0.6, lxly, ly_range, lxly);
	cut_lines_(-0.65,0.65, lxly, ly_range, lxly);
	roccut_lines_(2, lxly, ly_range, lxly); // Between ROCs
      }
      // else cut_lines_(-0.3,0.2, lxly, ly_range, lxly); // Half Modules
      else cut_lines_(-0.3,0.25, lxly, ly_range, lxly); // Half Modules
      break;
    default: // FPix
      switch (j) {
      case 2: // Panel 1 Module 1 (1x2)
	// cut_lines_(-0.2,0.2, lxly, ly_range, lxly); break;
	cut_lines_(-0.15,0.3, lxly, ly_range, lxly); break;
      case 3: // Panel 1 Module 2 (2x3)
	roccut_lines_(2, lxly, ly_range, lxly); // Between ROCs
	// if (i==6) cut_lines_(-0.5,0.2, lxly, ly_range, lxly); // Disk -2
	// else if (i==9) cut_lines_(-0.5,0.0, lxly, ly_range, lxly); // Disk +2
	// else cut_lines_(-0.55,0.55, lxly, ly_range, lxly); // Disk +-1
	if (i==7||i==8) cut_lines_(-0.55,0.6, lxly, ly_range, lxly); // Disk +-1
	else cut_lines_(-0.6,0.3, lxly, ly_range, lxly); // Disk +-2
	break;
      case 4: // Panel 1 Module 3 (2x4)
	roccut_lines_(2, lxly, ly_range, lxly); // Between ROCs
	// cut_lines_(-0.6,0.5, lxly, ly_range, lxly); 
	if (i==7||i==8) cut_lines_(0.3,0.6, lxly, ly_range, lxly); // Disk +-1
	else cut_lines_(-0.6,0.5, lxly, ly_range, lxly); // Disk +-2
	break;
      case 5: // Panel 1 Module 4 (1x5)
	cut_lines_(-0.3,0.15, lxly, ly_range, lxly); break;
      case 6: // Panel 2 Module 1 (2x3)
	roccut_lines_(2, lxly, ly_range, lxly); // Between ROCs
	// cut_lines_(-0.6,0.6, lxly, ly_range, lxly); 
	cut_lines_(-0.55,0.6, lxly, ly_range, lxly); 
	break;
      case 7: // Panel 2 Module 2 (2x4)
	roccut_lines_(2, lxly, ly_range, lxly); // Between ROCs
	// if (i==7||i==8)	cut_lines_(-0.55,0.55, lxly, ly_range, lxly); // Disk +-1
	// else cut_lines_(-0.6,0.5, lxly, ly_range, lxly); // Disk +-2
	if (i==7||i==8)	cut_lines_(-0.55,0.55, lxly, ly_range, lxly); // Disk +-1
	else cut_lines_(-0.6,0.55, lxly, ly_range, lxly); // Disk +-2
	break;
      case 8: // Panel 2 Module 3 (2x5)	
	roccut_lines_(2, lxly, ly_range, lxly); // Between ROCs
	// cut_lines_(-0.5,0.5, lxly, ly_range, lxly); 
	cut_lines_(-0.55,0.55, lxly, ly_range, lxly); 
	break;
      default: 
	break;
      }
      break;
    }
  }

  void fid_ly_cut_lines_(int i, int j, float lx_range, int lxly = 0) {
    // Extra lx dependent cuts
    // P1 M2
    float lxmin = 0.25;
    const char* lat = "LX > 0.25";
    float ly_dm1 =  0.75;
    float ly_dp1 = -0.75;
    float cut = 1.1;
    // P1 M3
    float lxmin2 = 0.15;
    const char* lat2 = "LX > 0.15";
    float ly_dm2 =  1.1;
    float ly_dp2 = -1.1;
    float cut2 = 1.5;
    switch (i) {
    case 3: case 4: case 5: // Layer 1-2-3
      cut_lines_(-3.1,3.1, lxly, lx_range); 
      roccut_lines_(8, lxly, lx_range); // Between ROCs
      break;
    default: // FPix
      switch (j) {
      case 2: // Panel 1 Module 1 (1x2)
	roccut_lines_(2, lxly, lx_range); // Between ROCs
	cut_lines_(-0.7,0.7, lxly, lx_range);
	break;
      case 3: // Panel 1 Module 2 (2x3)
        roccut_lines_(3, lxly, lx_range); // Between ROCs
	if (i==6||i==9) cut_lines_(-cut,cut, lxly, lx_range);
	else if (lxly==0) { // Disk 1 + Extra Overlap
	  cut_lines_(-cut,cut, lxly, lx_range);
	  if (i==7) {
	    draw_line_(ly_dm1, 0.98, ly_dm1, 1.005, 4, 9, 1);
	    draw_lat_(ly_dm1, 1.001, lat, 4, 0.035, 0, 21);
	  } else {
	    draw_line_(ly_dp1, 0.98, ly_dp1, 1.005, 4, 9, 1);
	    draw_lat_(ly_dp1, 1.001, lat, 4, 0.035, 0, 21);
	  }
	} else {
	  if (i==7) { // Disk -1
	    draw_line_(  -cut, -lx_range,   -cut, lx_range, 1, 9, 2);
	    draw_line_(ly_dm1,  lxmin, ly_dm1, lx_range, 1, 9, 2);
	    draw_line_(   cut, -lx_range,    cut, lxmin, 1, 9, 2);
	  } else if (i==8) { // Disk +1
	    draw_line_(ly_dp1,  lxmin, ly_dp1, lx_range, 1, 9, 2);
	    draw_line_(  -cut, -lx_range,   -cut, lxmin, 1, 9, 2);
	    draw_line_(   cut, -lx_range,    cut, lx_range, 1, 9, 2);
	  }
	}
	break;
      case 4: // Panel 1 Module 3 (2x4)
        roccut_lines_(4, lxly, lx_range); // Between ROCs
	if (i==7) cut_lines_(-cut2,1.1, lxly, lx_range); // Disk -1
	else if (i==8) cut_lines_(-1.0,cut2, lxly, lx_range); // Disk +1
	else if (lxly==0) { // Disk 2 + Extra Overlap
	  cut_lines_(-cut2,cut2, lxly, lx_range);
	  if (i==6) {
	    draw_line_(ly_dm2, 0.98, ly_dm2, 1.005, 4, 9, 1);
	    draw_lat_(ly_dm2, 1.001, lat2, 4, 0.035, 0, 21);
	  } else {
	    draw_line_(ly_dp2, 0.98, ly_dp2, 1.005, 4, 9, 1);
	    draw_lat_(ly_dp2, 1.001, lat2, 4, 0.035, 0, 21);
	  }
	} else {
	  if (i==6) { // Disk -2
	    draw_line_( -cut2, -lx_range,  -cut2,  lx_range, 1, 9, 2);
	    draw_line_(ly_dm2, lxmin2, ly_dm2,  lx_range, 1, 9, 2);
	    draw_line_(  cut2, -lx_range,   cut2, lxmin2, 1, 9, 2);
	  } else if (i==9) { // Disk +2
	    draw_line_(ly_dp2, lxmin2, ly_dp2,  lx_range, 1, 9, 2);
	    draw_line_(-cut2,  -lx_range,  -cut2, lxmin2, 1, 9, 2);
	    draw_line_( cut2,  -lx_range,   cut2,  lx_range, 1, 9, 2);
	  }
	}
        break;
      case 5: // Panel 1 Module 4 (1x5)
        roccut_lines_(5, lxly, lx_range); // Between ROCs
        if (i==6) cut_lines_(-1.9,1.5, lxly, lx_range); // Disk -2
        else if (i==9) cut_lines_(-1.5,1.9, lxly, lx_range); // Disk +2
	else cut_lines_(-1.9,1.9, lxly, lx_range); // Disk 1
	break;
      case 6: // Panel 2 Module 1 (2x3)
        roccut_lines_(3, lxly, lx_range); // Between ROCs
        cut_lines_(-0.7,0.7, lxly, lx_range);
        break;
      case 7: // Panel 2 Module 2 (2x4)
        roccut_lines_(4, lxly, lx_range); // Between ROCs
	cut_lines_(-1.1,1.1, lxly, lx_range);
        break;
      case 8: // Panel 2 Module 3 (2x5)	
	roccut_lines_(5, lxly, lx_range); // Between ROCs
        if (i==6) cut_lines_(-1.5,1.6, lxly, lx_range); // Disk -2
        else if (i==6) cut_lines_(-1.6,1.5, lxly, lx_range); // Disk +2
	else cut_lines_(-1.5,1.5, lxly, lx_range);
        break;
      default: 
        break;	
      }
      break;
    }
  }

  void fid2_can_(std::vector<TCanvas*>& c, Histograms* lxly, Histograms* lx, Histograms* ly) {
    for (int i=3; i<10; i++) {
      for (int j=0; j<9; j++) {
	if ((i<6&&j<2)||(i>=6&&j>=2)) {
	  int lx_nroc = (j==1||j==2||j==5) ? 1 : 2;
	  int ly_nroc = (j<2) ? 8 : (j==2) ? 2 : (j==3||j==6) ? 3 : (j==4||j==7) ? 4 : 5;
	  float lx_range = lx_nroc / 2.0 * 1.1 * 0.8164; // Same as rocsizelx in Variables::binmaker
	  float ly_range = ly_nroc / 2.0 * 1.1 * 0.81; // Same as rocsizely in Variables::binmaker
	  // float lx_range = (j==1||j==2||j==5) ? 0.61 : 1.02;
	  // float lx_range = ((j<2) ? 4.05 : (j==2) ? 1.62 : (j==3||j==6) ? 2.025 : (j==4||j==7) ? 2.43 : 2.835 ;
	  TCanvas* can = square_can_1d_(lx, "Local X [cm]", lx->histoname()+postfix_.det[i]+postfix_.mod[j], "N-1 - Fid LX - "+postfix_.Det[i]+" "+postfix_.Mod[j], 0, 0);
	  lx->h1d(i-3,j-2*(i>5),0)->GetXaxis()->SetRangeUser(-lx_range, lx_range);
	  lx->h1d(i-3,j-2*(i>5),0)->GetYaxis()->SetRangeUser(0.98, 1.005);
	  lx->h1d(i-3,j-2*(i>5),0)->Draw("PE1");
	  fid_lx_cut_lines_(i,j,ly_range);
	  c.push_back(can);
	  can = square_can_1d_(ly, "Local Y [cm]", ly->histoname()+postfix_.det[i]+postfix_.mod[j], "N-1 - Fid LY - "+postfix_.Det[i]+" "+postfix_.Mod[j], 0, 0);
	  ly->h1d(i-3,j-2*(i>5),0)->GetXaxis()->SetRangeUser(-ly_range,ly_range);
	  ly->h1d(i-3,j-2*(i>5),0)->GetYaxis()->SetRangeUser(0.98,1.005);
	  ly->h1d(i-3,j-2*(i>5),0)->Draw("PE1");
	  fid_ly_cut_lines_(i,j,lx_range);
	  c.push_back(can);
	  lxly->h2d(i-3,j-2*(i>5),0)->GetXaxis()->SetRangeUser(-ly_range,ly_range);
	  lxly->h2d(i-3,j-2*(i>5),0)->GetYaxis()->SetRangeUser(-lx_range,lx_range);
	  lxly->h2d(i-3,j-2*(i>5),0)->GetZaxis()->SetRangeUser(i==3 ? 0.98: 0.99,1.00);
	  can = custom_can_(lxly->h2d(i-3,j-2*(i>5),0), lxly->histoname()+postfix_.det[i]+postfix_.mod[j],
			    postfix_.Det[i]+" "+postfix_.Mod[j], "Local Y [cm]", "Local X [cm]", "Efficiency", 0,0, ly_nroc*150, lx_nroc*150);
	  //can = rectangle_can_2d_(lxly, "Local Y [cm]", "Local X [cm]", lxly->histoname()+postfix_.det[i]+postfix_.mod[j], "N-1 - Fid LX LY - "+postfix_.Det[i]+" "+postfix_.Mod[j]);
	  //lxly->h2d(i-3,j-2*(i>5),0)->Draw("COLZ");
	  //if (lxly->h2d(i-3,j-2*(i>5),0)->GetEntries()>0) {
	  //  gPad->Update();
	  //  TPaletteAxis* palette = (TPaletteAxis*)lxly->h2d(i-3,j-2*(i>5),0)->GetListOfFunctions()->FindObject("palette");
	  //  palette->SetX2NDC(0.91);
	  //}
	  fid_lx_cut_lines_(i,j,ly_range,1);
	  fid_ly_cut_lines_(i,j,lx_range,1);
	  c.push_back(can);
	}
      }
    }
  }

  
  //_________________________________________________________________________________________________________
  //                                                   Scans
  //_________________________________________________________________________________________________________
  

  TF1* fit_turnon_(double p1, double p2, TH1D *hv) {
    if (hv->GetEntries()) {
      TF1* f = new TF1("turnon", "1/(1+exp(([0]-x)/[1]))", 15, 155);
      f->SetParameters(p1,p2);
      hv->Fit("turnon","RMQ0");
      return hv->GetFunction("turnon");
    } else return 0;
  }

  TF1* fit_turnon_improved_(double p1, double p2, TH1D *hv) {
    if (hv->GetEntries()>0) {
      TF1* f = new TF1("turnon_imp", "[0]+[1]/(1+exp(([2]-x)/[3]))", 5, 150);
      f->SetParameters(0,1,p1,p2);
      f->SetParLimits(0,0,1);
      f->SetParLimits(1,0,1);
      f->SetParLimits(2,0,100);
      f->SetParLimits(3,0.1,10);      
      hv->Fit("turnon_imp","RMQ0");
      return hv->GetFunction("turnon_imp");
    } else return 0;
  }


  void set_dv_(TH1D* h, int binx, TH1D* hv) {
    TF1* f = fit_turnon_(30.0,3.4,hv);
    if (f!=0) {
      double dv =  f->GetParameter(0) + 2.94443898 * f->GetParameter(1);
      double dv_err =  sqrt(f->GetParError(0)*f->GetParError(0) + 8.6697209 * f->GetParError(1)*f->GetParError(1));
      h->SetBinContent(binx,dv);
      h->SetBinError(binx,dv_err);
    }
  }

  double dv_(TH1D* h) {
    TF1* f = fit_turnon_(30.0,3.4,h);
    if (f!=0) {
      double dv =  f->GetParameter(0) + 2.94443898 * f->GetParameter(1);
      return dv;
    } else return 0;
  }

  TH1D* hv_11jul_l1;
  TH1D* rog_11jul_BmO_Sec6;

  void hv_lad_ring_can_(Histograms* h, int i, int j, std::string name, std::string title, int nlad, std::vector<TCanvas*>& c) {
    // lad
    std::string hn = name + "_lad";
    TH1D* lad = new TH1D(hn.c_str(),hn.c_str(),2*nlad+1,-1*nlad-0.5,nlad+0.5);
    for (int a=0; a<nlad*2; a++) set_dv_(lad, a+1+(a>nlad-1), h->h1d(i,j,a+9,0));
    setstyle_(lad, "Ladders", "V_{95} [V]",1.0,1.0);
    std::string cn = "lad_" + name;
    std::string t = title + " Ladder";
    TCanvas* lad_can = default_can_(cn.c_str(), t.c_str(), 650,600, 13,10,9,11, 1,1, 1,1);
    lad->SetMarkerStyle(8);
    lad->SetMarkerSize(1.0);
    lad->Draw("PE1X0");
    c.push_back(lad_can);
    // ring
    hn = name + "_ring";
    TH1D* ring_l1 = new TH1D(hn.c_str(),hn.c_str(),9,-4.5,4.5);
    for (int a=1; a<=8-4*(i==5); a++) set_dv_(ring_l1, a+(a>4), h->h1d(i,j,a,0));
    setstyle_(ring_l1, "Rings", "V_{95} [V]",1.0,1.0);
    cn = "ring_" + name;
    t = title + " Rings";
    TCanvas* ring_can = default_can_(cn.c_str(), t.c_str(), 650,600, 13,10,9,11, 1,1, 1,1);
    ring_l1->SetMarkerStyle(8);
    ring_l1->SetMarkerSize(1.0);
    ring_l1->Draw("PE1X0");
    c.push_back(ring_can);
  }

  void lad_ring_l1_can_(Histograms* h, std::vector<TCanvas*>& c) {
    // lad
    TH1D* lad1 = new TH1D("lad_l1jul","lad_l1jul",21,-10.5,10.5);
    TH1D* lad2 = new TH1D("lad_l1sep","lad_l1sep",21,-10.5,10.5);
    for (int a=0; a<20; a++) set_dv_(lad1, a+1+(a>9), h->h1d(4,0,a+9,0));
    for (int a=0; a<20; a++) set_dv_(lad2, a+1+(a>9), h->h1d(7,1,a+9,0));
    setstyle_(lad1, "Ladders", "V_{95} [V]",1.0,1.0);
    setstyle_(lad2, "Ladders", "V_{95} [V]",1.0,1.0);
    TCanvas* lad_can = default_can_("lad_l1_compare","Scans - Ladders L1 Full Scans", 650,600, 13,10,9,11, 1,1, 1,1);
    lad1->SetMarkerStyle(8); lad1->SetMarkerSize(1.0); lad1->SetMarkerColor(4); lad1->Draw("PE1X0");
    lad2->SetMarkerStyle(8); lad2->SetMarkerSize(1.0); lad2->SetMarkerColor(2);lad2->Draw("SAMEPE1X0");
    TLegend* lad_leg = new TLegend(0.4,0.3,0.6,0.5,"Full Layer 1 scans");
    lad_leg->AddEntry(lad1, "2011 Jul 14 #intL dt = 1.3 fb^{-1}", "p");
    lad_leg->AddEntry(lad2, "2011 Sep 07 #intL dt = 2.7 fb^{-1}", "p");
    lad_leg->SetFillColor(0); lad_leg->SetFillStyle(0); lad_leg->SetBorderSize(0); lad_leg->SetTextSize(0.04); lad_leg->Draw();
    c.push_back(lad_can);
    // ring
    TH1D* ring1 = new TH1D("ring_l1jul","ring_l1jul",9,-4.5,4.5);
    TH1D* ring2 = new TH1D("ring_l1sep","ring_l1sep",9,-4.5,4.5);
    for (int a=1; a<=8; a++) set_dv_(ring1, a+(a>4), h->h1d(4,0,a,0));
    for (int a=1; a<=8; a++) set_dv_(ring2, a+(a>4), h->h1d(7,1,a,0));
    setstyle_(ring1, "Rings", "V_{95} [V]",1.0,1.0);
    setstyle_(ring2, "Rings", "V_{95} [V]",1.0,1.0);
    TCanvas* ring_can = default_can_("ring_l1_compare","Scans - Rings L1 Full Scans", 650,600, 13,10,9,11, 1,1, 1,1);
    ring1->SetMarkerStyle(8); ring1->SetMarkerSize(1.0); ring1->SetMarkerColor(4); ring1->Draw("PE1X0");
    ring2->SetMarkerStyle(8); ring2->SetMarkerSize(1.0); ring2->SetMarkerColor(2);ring2->Draw("SAMEPE1X0");
    TLegend* ring_leg = new TLegend(0.4,0.3,0.6,0.5,"Full Layer 1 scans");
    ring_leg->AddEntry(ring1, "2011 Jul 14 #intL dt = 1.3 fb^{-1}", "p");
    ring_leg->AddEntry(ring2, "2011 Sep 07 #intL dt = 2.7 fb^{-1}", "p");
    ring_leg->SetFillColor(0); ring_leg->SetFillStyle(0); ring_leg->SetBorderSize(0); ring_leg->SetTextSize(0.04); ring_leg->Draw();
    c.push_back(ring_can);
  }
  
  void draw_with_fit_(double p1, double p2, TH1D* h, const char* drawopt, int c) {
    h->GetYaxis()->SetRangeUser(0.0,1.2);
    h->SetMarkerStyle(8);
    h->SetMarkerColor(c);
    h->SetMarkerSize(1.0);
    h->Draw(drawopt);
    TF1* f = fit_turnon_improved_(p1,p2,h);
    if (f!=0) {
      f->SetLineColor(c);
      f->Draw("same");
    }
  }

  void draw_with_mpvcurve_(TH1D* h, const char* drawopt, Color_t c) {
    h->SetMarkerStyle(8);
    h->SetMarkerColor(c);
    h->SetMarkerSize(1.0);
    h->Draw(drawopt);
  }

  void hv_can_(std::vector<TCanvas*>& c, AllHistos& ah, Histograms* h, Variables& v) {
    // Special graphs
    ah.hv_graphs(v);
    TCanvas* v95 = default_can_("totlumi_v95", "Scans - Scans History - V_95", 650,600, 13,10,9,11, 1,1, 1,1);
    ah.hv_totlumi[9]->Draw("AP");
    ah.hv_totlumi[9]->GetXaxis()->SetRangeUser(-100.0,6000.0);
    ah.hv_totlumi[9]->GetYaxis()->SetRangeUser(00.0,70.0);
    ah.hv_totlumi[9]->GetYaxis()->SetTitle("V_{95} [V]");
    ah.hv_totlumi[9]->GetXaxis()->SetTitle("Total Int. Luminosity - 2010/11 [pb^{-1}]");
    ah.hv_totlumi[0]->Draw("SAMEP");
    ah.hv_totlumi[3]->Draw("SAMEP");
    ah.hv_totlumi[6]->Draw("SAMEP");
    // TLine* line = new TLine(44,0,44,70); line->Draw();
    // TLatex* lat= new TLatex(20.0, 75.0, "2010"); 
    // lat->SetLineWidth(2); lat->SetTextSize(0.04); lat->SetTextAlign(21); lat->Draw();
    // lat= new TLatex(1500.0, 75.0, "2011"); 
    // lat->SetLineWidth(2); lat->SetTextSize(0.04); lat->SetTextAlign(21); lat->Draw();
    // lat= new TLatex(0.0, 48.0, "2010 Apr - 132599, +"); lat->SetTextSize(0.02); lat->Draw();
    // lat= new TLatex(30.0, 65.0, "2010 Oct - 149182"); lat->SetTextSize(0.02); lat->Draw();
    // lat= new TLatex(50.0, 57.0, "2011 Mar - 160578, +"); lat->SetTextSize(0.02); lat->Draw();
    // lat= new TLatex(400.0, 53.0, "2011 May 15 - 165098, +"); lat->SetTextSize(0.02); lat->Draw();
    // lat= new TLatex(1340.0, 44.0, "2011 Jul 14 - 170000"); lat->SetTextSize(0.02); lat->SetTextAlign(21); lat->Draw();
    // lat= new TLatex(1690.0, 56.0, "2011 Jul 28 - 171897"); lat->SetTextSize(0.02); lat->SetTextAlign(21); lat->Draw();
    // lat= new TLatex(1920.0, 51.0, "2011 Aug 03 - 172488"); lat->SetTextSize(0.02); lat->SetTextAlign(21); lat->Draw();
    // lat= new TLatex(2680.0, 35.0, "2011 Sep 07 - 175834"); lat->SetTextSize(0.02); lat->SetTextAlign(21); lat->Draw();
    TLegend* v95_leg = new TLegend(0.4,0.2,0.6,0.4,"");
    v95_leg->AddEntry(ah.hv_totlumi[0], "Layer 1", "p");
    v95_leg->AddEntry(ah.hv_totlumi[3], "Layer 2", "p");
    v95_leg->AddEntry(ah.hv_totlumi[6], "Layer 3", "p");
    v95_leg->AddEntry(ah.hv_totlumi[9], "Disk -1", "p");
    v95_leg->SetFillColor(0); v95_leg->SetFillStyle(0); v95_leg->SetBorderSize(0); v95_leg->SetTextSize(0.04); v95_leg->Draw();
    c.push_back(v95);
    TCanvas* v50 = default_can_("totlumi_v50", "Scans - Scans History - V_50", 650,600, 13,10,9,11, 1,1, 1,1);
    ah.hv_totlumi[10]->Draw("AP");
    ah.hv_totlumi[10]->GetXaxis()->SetRangeUser(-100.0,6000.0);
    ah.hv_totlumi[10]->GetYaxis()->SetRangeUser(0.0,70.0);
    ah.hv_totlumi[10]->GetYaxis()->SetTitle("V_{50} [V]");
    ah.hv_totlumi[10]->GetXaxis()->SetTitle("Total Int. Luminosity - 2010/11 [pb^{-1}]");
    ah.hv_totlumi[1]->Draw("SAMEP");
    ah.hv_totlumi[4]->Draw("SAMEP");
    ah.hv_totlumi[7]->Draw("SAMEP");
    TLegend* v50_leg = new TLegend(0.4,0.2,0.6,0.4,"");
    v50_leg->AddEntry(ah.hv_totlumi[1], "Layer 1", "p");
    v50_leg->AddEntry(ah.hv_totlumi[4], "Layer 2", "p");
    v50_leg->AddEntry(ah.hv_totlumi[7], "Layer 3", "p");
    v50_leg->AddEntry(ah.hv_totlumi[10], "Disk -1", "p");
    v50_leg->SetFillColor(0); v50_leg->SetFillStyle(0); v50_leg->SetBorderSize(0); v50_leg->SetTextSize(0.04); v50_leg->Draw();
    c.push_back(v50);
    TCanvas* tw = default_can_("totlumi_tw", "Scans - Scans History - TW", 650,600, 13,10,9,11, 1,1, 1,1);
    ah.hv_totlumi[11]->Draw("AP");
    ah.hv_totlumi[11]->GetXaxis()->SetRangeUser(-100.0,6000.0);
    ah.hv_totlumi[11]->GetYaxis()->SetRangeUser(0.0,10.0);
    ah.hv_totlumi[11]->GetYaxis()->SetTitle("Turnon Width [V]");
    ah.hv_totlumi[11]->GetXaxis()->SetTitle("Total Int. Luminosity - 2010/11 [pb^{-1}]");
    ah.hv_totlumi[2]->Draw("SAMEP");
    ah.hv_totlumi[5]->Draw("SAMEP");
    ah.hv_totlumi[8]->Draw("SAMEP");
    TLegend* tw_leg = new TLegend(0.4,0.2,0.6,0.4,"");
    tw_leg->AddEntry(ah.hv_totlumi[2], "Layer 1", "p");
    tw_leg->AddEntry(ah.hv_totlumi[5], "Layer 2", "p");
    tw_leg->AddEntry(ah.hv_totlumi[8], "Layer 3", "p");
    tw_leg->AddEntry(ah.hv_totlumi[11], "Disk -1", "p");
    tw_leg->SetFillColor(0); tw_leg->SetFillStyle(0); tw_leg->SetBorderSize(0); tw_leg->SetTextSize(0.04); tw_leg->Draw();
    c.push_back(tw);
    // Compare layer 1 Full scans
    dress_all_(h,"Voltage [V]",1.0,1.2);
    TCanvas* l1_can = default_can_("hv_l1_compare", "Scans - 2011 Sep HV", 650,600, 13,10,9,11, 1,1, 1,1);
    draw_with_fit_(31.0,4.0,h->h1d(4,0,0,0),"PE1X0",1);
    draw_with_fit_(16.0,5.0,h->h1d(7,1,0,0),"SAMEPE1X0",4);
    draw_with_fit_(16.0,5.0,h->h1d(11,0,0,0),"SAMEPE1X0",2);
    TLegend* leg_l1 = new TLegend(0.4,0.3,0.6,0.5,"Full Layer 1 scans");
    leg_l1->AddEntry(h->h1d( 4,0,0,0), "2011 Jul 14 - 1.46 fb^{-1}", "p");
    leg_l1->AddEntry(h->h1d( 7,1,0,0), "2011 Sep 07 - 2.93 fb^{-1}", "p");
    leg_l1->AddEntry(h->h1d(11,0,0,0), "2012 Apr 06 - 6.20 fb^{-1}", "p");
    leg_l1->SetFillColor(0); leg_l1->SetFillStyle(0); leg_l1->SetBorderSize(0); leg_l1->SetTextSize(0.04); leg_l1->Draw();
    c.push_back(l1_can);
    // Compare layer 1 scans - (module)
    TCanvas* l1rog_can = default_can_("hv_l1rog_compare", "Scans - L1 (rog) compare", 650,600, 13,10,9,11, 1,1, 1,1);
    draw_with_fit_(46.0,5.8,h->h1d(1,1,0,0),"PE1X0",1); // Black
    draw_with_fit_(44.0,5.8,h->h1d(2,1,0,0),"SAMEPE1X0",28); // Brown
    draw_with_fit_(42.0,4.0,h->h1d(3,0,0,0),"SAMEPE1X0",3); // Green
    draw_with_fit_(31.0,4.0,h->h1d(4,4,5,0),"SAMEPE1X0",4); // Blue
    draw_with_fit_(16.0,5.0,h->h1d(7,5,5,0),"SAMEPE1X0",6); // Purple
    draw_with_fit_(16.0,3.0,h->h1d(8,0,0,0),"SAMEPE1X0",TColor::GetColor("#ff9900")); // Orange
    draw_with_fit_(15.0,3.3,h->h1d(9,0,0,0),"SAMEPE1X0",2); // Red
    TLegend* leg_l1rog = new TLegend(0.4,0.2,0.6,0.5,"Bpix_BmO_SEC6_LYR1_HV1 *");
    leg_l1rog->AddEntry(h->h1d(1,1,0,0), "2010 Oct 28 - 0.00 fb^{-1} - *SEC2", "p");
    leg_l1rog->AddEntry(h->h1d(2,1,0,0), "2011 Mar 16 - 0.05 fb^{-1}", "p");
    leg_l1rog->AddEntry(h->h1d(3,0,0,0), "2011 May 15 - 0.33 fb^{-1}", "p");
    leg_l1rog->AddEntry(h->h1d(4,4,5,0), "2011 Jul 14 - 1.46 fb^{-1}", "p");
    leg_l1rog->AddEntry(h->h1d(7,5,5,0), "2011 Sep 07 - 2.93 fb^{-1}", "p");
    leg_l1rog->AddEntry(h->h1d(8,0,0,0), "2011 Oct 12 - 5.17 fb^{-1}", "p");
    leg_l1rog->AddEntry(h->h1d(9,0,0,0), "2011 Oct 27 - 6.07 fb^{-1}", "p");
    leg_l1rog->SetFillColor(0); leg_l1rog->SetFillStyle(0); leg_l1rog->SetBorderSize(0); leg_l1rog->SetTextSize(0.035); leg_l1rog->Draw();
    c.push_back(l1rog_can);
    // Compare layer 2 Full scans
    TCanvas* l2_can = default_can_("hv_l2_compare", "Scans - L2 compare", 650,600, 13,10,9,11, 1,1, 1,1);
    draw_with_fit_(36.0,3.4,h->h1d( 6,0,0,0),"PE1X0",4);
    draw_with_fit_(36.0,3.4,h->h1d(11,1,0,0),"SAMEPE1X0",2);
    TLegend* leg_l2 = new TLegend(0.4,0.35,0.6,0.5,"Layer 2 (full) - Run: 171897");
    leg_l2->AddEntry(h->h1d( 6,0,0,0), "2011 Aug 03 - 2.09 fb^{-1}", "p");
    leg_l2->AddEntry(h->h1d(11,1,0,0), "2012 Apr 06 - 6.20 fb^{-1}", "p");
    leg_l2->SetFillColor(0); leg_l2->SetFillStyle(0); leg_l2->SetBorderSize(0); leg_l2->SetTextSize(0.04); leg_l2->Draw();
    c.push_back(l2_can);
    // Compare layer 3 Full scans
    TCanvas* l3_can = default_can_("hv_l3_compare", "Scans - L3 compare", 650,600, 13,10,9,11, 1,1, 1,1);
    draw_with_fit_(36.0,3.4,h->h1d( 5,0,0,0),"PE1X0",4);
    draw_with_fit_(36.0,3.4,h->h1d(11,2,0,0),"SAMEPE1X0",2);
    TLegend* leg_l3 = new TLegend(0.4,0.35,0.6,0.5,"Full Layer 3 scans");
    leg_l3->AddEntry(h->h1d( 5,0,0,0), "2011 Jul 28 - 1.84 fb^{-1}", "p");
    leg_l3->AddEntry(h->h1d(11,2,0,0), "2012 Apr 06 - 6.20 fb^{-1}", "p");
    leg_l3->SetFillColor(0); leg_l3->SetFillStyle(0); leg_l3->SetBorderSize(0); leg_l3->SetTextSize(0.04); leg_l3->Draw();
    c.push_back(l3_can);
    // Compare layer 3 scans - (module)
    TCanvas* l3rog_can = default_can_("hv_l3rog_compare", "Scans - L3 (rog) compare", 650,600, 13,10,9,11, 1,1, 1,1);
    draw_with_fit_(32.0,7.5,h->h1d(0,1,0,0),"PE1X0",1);
    draw_with_fit_(32.0,7.5,h->h1d(1,2,0,0),"SAMEPE1X0",4);
    draw_with_fit_(32.0,7.5,h->h1d(2,2,0,0),"SAMEPE1X0",6);
    draw_with_fit_(39.0,5.2,h->h1d(5,2,1,0),"SAMEPE1X0",2);
    TLegend* leg_l3rog = new TLegend(0.4,0.2,0.6,0.5,"Bpix_BpO_SEC2_LYR3_HV1");
    leg_l3rog->AddEntry(h->h1d(0,1,0,0), "2010 Apr 05 #intL dt = 0.0 fb^{-1}", "p");
    leg_l3rog->AddEntry(h->h1d(1,2,0,0), "2010 Oct 28 #intL dt = 0.043 fb^{-1}", "p");
    leg_l3rog->AddEntry(h->h1d(2,2,0,0), "2011 Mar 16 #intL dt = 0.045 fb^{-1}", "p");
    leg_l3rog->AddEntry(h->h1d(5,2,1,0), "2011 Jul 28 #intL dt = 1.7 fb^{-1}", "p");
    leg_l3rog->SetFillColor(0); leg_l3rog->SetFillStyle(0); leg_l3rog->SetBorderSize(0); leg_l3rog->SetTextSize(0.04); leg_l3rog->Draw();
    c.push_back(l3rog_can);
    // Compare FPix scans - (module)
    TCanvas* fpixrog_can = default_can_("hv_fpixrog_compare", "Scans - FPix (rog) compare", 650,600, 13,10,9,11, 1,1, 1,1);
    h->h1d(0,0,0,0)->GetXaxis()->SetRangeUser(0,150);
    draw_with_fit_(30.0,4.0,h->h1d(0,0,0,0),"PE1X0",1); // Black
    draw_with_fit_(30.0,1.0,h->h1d(1,0,0,0),"SAMEPE1X0",3); // Green
    draw_with_fit_(30.0,3.6,h->h1d(2,0,0,0),"SAMEPE1X0",4); // Blue
    draw_with_fit_(24.0,3.6,h->h1d(7,0,0,0),"SAMEPE1X0",28); // Brown
    draw_with_fit_(24.0,3.6,h->h1d(10,0,0,0),"SAMEPE1X0",797); // Orange
    draw_with_fit_(24.0,3.6,h->h1d(11,3,0,0),"SAMEPE1X0",2); // Red
    TLegend* leg_fpixrog = new TLegend(0.4,0.2,0.6,0.5,"FPIX_BmI_D1_ROG1 HV1");
    leg_fpixrog->AddEntry(h->h1d( 0,0,0,0), "2010 Apr 05 - 0.00 fb^{-1}", "p");
    leg_fpixrog->AddEntry(h->h1d( 1,0,0,0), "2010 Oct 28 - 0.04 fb^{-1}", "p");
    leg_fpixrog->AddEntry(h->h1d( 2,0,0,0), "2011 Mar 16 - 0.05 fb^{-1}", "p");
    leg_fpixrog->AddEntry(h->h1d( 7,0,0,0), "2011 Sep 07 - 2.93 fb^{-1}", "p");
    leg_fpixrog->AddEntry(h->h1d(10,0,0,0), "2011 Oct 30 - 6.17 fb^{-1}", "p");
    leg_fpixrog->AddEntry(h->h1d(11,0,0,0), "2012 Apr 06 - 6.20 fb^{-1}", "p");
    leg_fpixrog->SetFillColor(0); leg_fpixrog->SetFillStyle(0); leg_fpixrog->SetBorderSize(0); leg_fpixrog->SetTextSize(0.04); leg_fpixrog->Draw();
    c.push_back(fpixrog_can);
    // Ladder and Ring plots
    hv_lad_ring_can_(h,5,0,"11jul_l3","Scans - 11/7 - L3",22,c);
    hv_lad_ring_can_(h,6,0,"11aug_l2","Scans - 11/8 - L2",16,c);
    lad_ring_l1_can_(h,c);
    // MPV Plots
    // L1
    TCanvas* can = default_can_(h->histoname()+"_l1_mpv", "Scans - MPV of NCCD vs Bias Voltage - Layer 1 - All Scans", 650,600, 13,7,9,11, 1,0, 1,1);
    h->h1d(1,1,0,2)->GetYaxis()->SetRangeUser(0,25);
    draw_with_mpvcurve_(h->h1d(1,1,0,2),"PE1X0",1); // Black
    draw_with_mpvcurve_(h->h1d(2,1,0,2),"SAMEPE1X0",28); // Brown
    draw_with_mpvcurve_(h->h1d(3,0,0,2),"SAMEPE1X0",3); // Green
    draw_with_mpvcurve_(h->h1d(4,4,5,2),"SAMEPE1X0",4); // Blue
    draw_with_mpvcurve_(h->h1d(7,5,5,2),"SAMEPE1X0",6); // Purple
    draw_with_mpvcurve_(h->h1d(8,0,0,2),"SAMEPE1X0",TColor::GetColor("#ff9900")); // Orange
    draw_with_mpvcurve_(h->h1d(9,0,0,2),"SAMEPE1X0",2); // Red
    TLegend* leg_l1rog2 = new TLegend(0.5,0.15,0.7,0.55,"Bpix_BmO_SEC6_LYR1_HV1 *");
    leg_l1rog2->AddEntry(h->h1d(1,1,0,2), "2010 Oct 28 #intL dt = 0.043 fb^{-1} - *SEC2", "p");
    leg_l1rog2->AddEntry(h->h1d(2,1,0,2), "2011 Mar 16 #intL dt = 0.045 fb^{-1}", "p");
    leg_l1rog2->AddEntry(h->h1d(3,0,0,2), "2011 May 15 #intL dt = 0.31 fb^{-1}", "p");
    leg_l1rog2->AddEntry(h->h1d(4,4,5,2), "2011 Jul 14 #intL dt = 1.3 fb^{-1}", "p");
    leg_l1rog2->AddEntry(h->h1d(7,5,5,2), "2011 Sep 07 #intL dt = 2.7 fb^{-1}", "p");
    leg_l1rog2->AddEntry(h->h1d(8,0,0,2), "2011 Oct 12 #intL dt = 4.8 fb^{-1}", "p");
    leg_l1rog2->AddEntry(h->h1d(9,0,0,2), "2011 Oct 27 #intL dt = 5.7 fb^{-1}", "p");
    leg_l1rog2->SetFillColor(0); leg_l1rog2->SetFillStyle(0); leg_l1rog2->SetBorderSize(0); leg_l1rog2->SetTextSize(0.03); leg_l1rog2->Draw();
    c.push_back(can);
    can = default_can_(h->histoname()+"_fpix_mpv", "Scans - MPV of NCCD vs Bias Voltage - Layer 1 - All Scans", 650,600, 13,7,9,11, 1,0, 1,1);
    h->h1d(0,0,0,2)->GetXaxis()->SetRangeUser(0,150);
    h->h1d(0,0,0,2)->GetYaxis()->SetRangeUser(0,25);
    draw_with_mpvcurve_(h->h1d(0,0,0,2),"PE1X0",1); // Black
    draw_with_mpvcurve_(h->h1d(1,0,0,2),"SAMEPE1X0",3); // Green
    draw_with_mpvcurve_(h->h1d(2,0,0,2),"SAMEPE1X0",4); // Blue
    draw_with_mpvcurve_(h->h1d(7,0,0,2),"SAMEPE1X0",28); // Brown
    draw_with_mpvcurve_(h->h1d(10,0,0,2),"SAMEPE1X0",2); // Red
    TLegend* leg_fpixrog2 = new TLegend(0.4,0.2,0.6,0.5,"FPIX_BmI_D1_ROG1 HV1");
    leg_fpixrog2->AddEntry(h->h1d(0,0,0,2), "2010 Apr 05 #intL dt = 0.0 fb^{-1}", "p");
    leg_fpixrog2->AddEntry(h->h1d(1,0,0,2), "2010 Oct 28 #intL dt = 0.043 fb^{-1}", "p");
    leg_fpixrog2->AddEntry(h->h1d(2,0,0,2), "2011 Mar 16 #intL dt = 0.045 fb^{-1}", "p");
    leg_fpixrog2->AddEntry(h->h1d(7,0,0,2), "2011 Sep 07 #intL dt = 2.7 fb^{-1}", "p");
    leg_fpixrog2->AddEntry(h->h1d(10,0,0,2), "2011 Oct 30 #intL dt = 5.7 fb^{-1}", "p");
    leg_fpixrog2->SetFillColor(0); leg_fpixrog2->SetFillStyle(0); leg_fpixrog2->SetBorderSize(0); leg_fpixrog2->SetTextSize(0.04); leg_fpixrog2->Draw();
    c.push_back(can);
  }

  void rog_11jul_can_(std::vector<TCanvas*>& c, Histograms* h) {
    dress_all_(h,"Voltage [V]",1.0,1.2);
    for (int i=0; i<4; i++) {
      for (int j=0; j<8; j++) {
	h->h1d(0,i,j,0)->GetYaxis()->SetRangeUser(0.0,1.2);
	h->h1d(0,i,j,0)->SetMarkerStyle(8);
	h->h1d(0,i,j,0)->SetMarkerSize(1.0);
	h->h1d(1,i,j,0)->GetYaxis()->SetRangeUser(0.0,1.2);
	h->h1d(1,i,j,0)->SetMarkerStyle(8);
	h->h1d(1,i,j,0)->SetMarkerSize(1.0);
      }
      TCanvas* can = default_can_(h->histoname()+postfix_.shl[i]+"_l1", "Scans - 2011 July ROG - "+postfix_.Shl[i], 650,600, 13,10,9,11, 1,0, 1,1);
      multidraw_with_legend_(h, 0, i, "0-7,", "PE1X0", "Layer 1 - "+postfix_.Shl[i], postfix_.Sec, defcol_, 0.4,0.6, 0.2,0.5);
      c.push_back(can);
      TCanvas* can2 = default_can_(h->histoname()+postfix_.shl[i]+"_l3", "Scans - 2011 July ROG - "+postfix_.Shl[i], 650,600, 13,10,9,11, 1,0, 1,1);
      multidraw_with_legend_(h, 1, i, "0-7,", "PE1X0", "Layer 3 - "+postfix_.Shl[i], postfix_.Sec, defcol_, 0.4,0.6, 0.2,0.5);
      c.push_back(can2);
    }
  }

  TF1* Param(TH1D* h) {
    Functions* func = new Functions();
    TF1* fitFcn = new TF1("fitFcn",func,&Functions::hvTurnon,15,155,2,"Functions","hvTurnon");
    fitFcn->SetParameters(30.0,3.4);
    h->Fit("fitFcn","RMQ0");
    return h->GetFunction("fitFcn");
    delete func;
    delete fitFcn;
  }


  TF1* Paramroc(TH1D* h, int i) {
    Functions* func = new Functions();
    TF1* fitFcn = new TF1("fitFcn",func,&Functions::hvTurnon,15,155,i,"Functions","hvTurnon");
    fitFcn->SetParameters(30.0,3.4);
    h->Fit("fitFcn","RMQ0");
    return h->GetFunction("fitFcn");
    delete func;
    delete fitFcn;
  }

  void SetBin(TH1D* p1, TH1D* p2, int binx, TF1* f) {
    p1->SetBinContent(binx, f->GetParameter(0));
    p1->SetBinError(binx, f->GetParError(0));
    p2->SetBinContent(binx, f->GetParameter(1));
    p2->SetBinError(binx, f->GetParError(1));
  }

  void SetBin(TH2D* p1, TH2D* p2, int binx, int biny, TF1* f) {
    p1->SetBinContent(binx, biny, f->GetParameter(0));
    p1->SetBinError(binx, biny, f->GetParError(0));
    p2->SetBinContent(binx, biny, f->GetParameter(1));
    p2->SetBinError(binx, biny, f->GetParError(1));
  }

  void hv_degrad_rog_can_(std::vector<TCanvas*>& c, Histograms* h) {
    // l1
    TH1D* p1_plus_l1 = new TH1D("p1_plus_l1","p1_plus_l1",17,-8.5,8.5);
    TH1D* p1_minus_l1 = new TH1D("p1_minus_l1","p1_minus_l1",17,-8.5,8.5);
    TH1D* p2_plus_l1 = new TH1D("p2_plus_l1","p2_plus_l1",17,-8.5,8.5);
    TH1D* p2_minus_l1 = new TH1D("p2_minus_l1","p2_minus_l1",17,-8.5,8.5);
    setstyle_(p1_plus_l1, "Sector", "Point of Half Efficiency [V]", 1.0,1.0);
    setstyle_(p1_minus_l1, "Sector", "Point of Half Efficiency [V]", 1.0,1.0);
    setstyle_(p2_plus_l1, "Sector", "Turnon Width [V]", 1.0,1.0);
    setstyle_(p2_minus_l1, "Sector", "Turnon Width [V]", 1.0,1.0);
    for (int i=0; i<8; i++) {
      SetBin(p1_plus_l1,  p2_plus_l1,  8-i,  Param(h->h1d(0,1,i,0)));
      SetBin(p1_plus_l1,  p2_plus_l1,  10+i, Param(h->h1d(0,0,i,0)));
      SetBin(p1_minus_l1, p2_minus_l1, 8-i,  Param(h->h1d(0,3,i,0)));
      SetBin(p1_minus_l1, p2_minus_l1, 10+i, Param(h->h1d(0,2,i,0)));
    }
    TCanvas* can_l1 = default_can_("hv_degrad_rog_p1_l1", "Scans - 2011 July - Rog (P1) l1", 650,600, 13,10,9,11, 1,1, 1,1);
    p1_plus_l1->SetMarkerColor(2);
    p1_plus_l1->SetMarkerStyle(8);
    p1_plus_l1->Draw("PE1X0");
    p1_minus_l1->SetMarkerColor(4);
    p1_minus_l1->SetMarkerStyle(8);
    p1_minus_l1->Draw("SAMEPE1X0");
    TLegend* leg = new TLegend(0.6,0.2,0.8,0.4,"HV groups");
    leg->AddEntry(p1_plus_l1, "+Z side", "p");
    leg->AddEntry(p1_minus_l1, "-Z side", "p");
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw();
    c.push_back(can_l1);
    TCanvas* can2_l1 = default_can_("hv_degrad_rog_p2_l1", "Scans - 2011 July - Rog (P1) l1", 650,600, 13,10,9,11, 1,1, 1,1);
    p2_plus_l1->SetMarkerColor(2);
    p2_plus_l1->SetMarkerStyle(8);
    p2_plus_l1->Draw("PE1X0");
    p2_minus_l1->SetMarkerColor(4);
    p2_minus_l1->SetMarkerStyle(8);
    p2_minus_l1->Draw("SAMEPE1X0");
    TLegend* leg2 = new TLegend(0.6,0.2,0.8,0.4,"HV groups");
    leg2->AddEntry(p2_plus_l1, "+Z side", "p");
    leg2->AddEntry(p2_minus_l1, "-Z side", "p");
    leg2->SetFillColor(0);
    leg2->SetFillStyle(0);
    leg2->SetBorderSize(0);
    leg2->SetTextSize(0.04);
    leg2->Draw();
    c.push_back(can2_l1);
    // l3
    TH1D* p1_plus_l3 = new TH1D("p1_plus_l3","p1_plus_l3",17,-8.5,8.5);
    TH1D* p1_minus_l3 = new TH1D("p1_minus_l3","p1_minus_l3",17,-8.5,8.5);
    TH1D* p2_plus_l3 = new TH1D("p2_plus_l3","p2_plus_l3",17,-8.5,8.5);
    TH1D* p2_minus_l3 = new TH1D("p2_minus_l3","p2_minus_l3",17,-8.5,8.5);
    setstyle_(p1_plus_l3, "Sector", "Point of Half Efficiency [V]", 1.0,1.0);
    setstyle_(p1_minus_l3, "Sector", "Point of Half Efficiency [V]", 1.0,1.0);
    setstyle_(p2_plus_l3, "Sector", "Turnon Width [V]", 1.0,1.0);
    setstyle_(p2_minus_l3, "Sector", "Turnon Width [V]", 1.0,1.0);
    for (int i=0; i<8; i++) {
      SetBin(p1_plus_l3,  p2_plus_l3,  8-i,  Param(h->h1d(2,1,i,0)));
      SetBin(p1_plus_l3,  p2_plus_l3,  10+i, Param(h->h1d(2,0,i,0)));
      SetBin(p1_minus_l3, p2_minus_l3, 8-i,  Param(h->h1d(2,3,i,0)));
      SetBin(p1_minus_l3, p2_minus_l3, 10+i, Param(h->h1d(2,2,i,0)));
    }
    TCanvas* can_l3 = default_can_("hv_degrad_rog_p1_l3", "Scans - 2011 July - Rog (P1) l3", 650,600, 13,10,9,11, 1,1, 1,1);
    p1_plus_l3->SetMarkerColor(2);
    p1_plus_l3->SetMarkerStyle(8);
    p1_plus_l3->Draw("PE1X0");
    p1_minus_l3->SetMarkerColor(4);
    p1_minus_l3->SetMarkerStyle(8);
    p1_minus_l3->Draw("SAMEPE1X0");
    TLegend* leg3 = new TLegend(0.6,0.2,0.8,0.4,"HV groups");
    leg3->AddEntry(p1_plus_l3, "+Z side", "p");
    leg3->AddEntry(p1_minus_l3, "-Z side", "p");
    leg3->SetFillColor(0);
    leg3->SetFillStyle(0);
    leg3->SetBorderSize(0);
    leg3->SetTextSize(0.04);
    leg3->Draw();
    c.push_back(can_l3);
    TCanvas* can2_l3 = default_can_("hv_degrad_rog_p2_l3", "Scans - 2011 July - Rog (P1) l3", 650,600, 13,10,9,11, 1,1, 1,1);
    p2_plus_l3->SetMarkerColor(2);
    p2_plus_l3->SetMarkerStyle(8);
    p2_plus_l3->Draw("PE1X0");
    p2_minus_l3->SetMarkerColor(4);
    p2_minus_l3->SetMarkerStyle(8);
    p2_minus_l3->Draw("SAMEPE1X0");
    TLegend* leg4 = new TLegend(0.6,0.2,0.8,0.4,"HV groups");
    leg4->AddEntry(p2_plus_l3, "+Z side", "p");
    leg4->AddEntry(p2_minus_l3, "-Z side", "p");
    leg4->SetFillColor(0);
    leg4->SetFillStyle(0);
    leg4->SetBorderSize(0);
    leg4->SetTextSize(0.04);
    leg4->Draw();
    c.push_back(can2_l3);
    // l2
    TH1D* p1_plus_l2 = new TH1D("p1_plus_l2","p1_plus_l2",17,-8.5,8.5);
    TH1D* p1_minus_l2 = new TH1D("p1_minus_l2","p1_minus_l2",17,-8.5,8.5);
    TH1D* p2_plus_l2 = new TH1D("p2_plus_l2","p2_plus_l2",17,-8.5,8.5);
    TH1D* p2_minus_l2 = new TH1D("p2_minus_l2","p2_minus_l2",17,-8.5,8.5);
    setstyle_(p1_plus_l2, "Sector", "Point of Half Efficiency [V]", 1.0,1.0);
    setstyle_(p1_minus_l2, "Sector", "Point of Half Efficiency [V]", 1.0,1.0);
    setstyle_(p2_plus_l2, "Sector", "Turnon Width [V]", 1.0,1.0);
    setstyle_(p2_minus_l2, "Sector", "Turnon Width [V]", 1.0,1.0);
    for (int i=0; i<8; i++) {
      SetBin(p1_plus_l2,  p2_plus_l2,  8-i,  Param(h->h1d(1,1,i,0)));
      SetBin(p1_plus_l2,  p2_plus_l2,  10+i, Param(h->h1d(1,0,i,0)));
      SetBin(p1_minus_l2, p2_minus_l2, 8-i,  Param(h->h1d(1,3,i,0)));
      SetBin(p1_minus_l2, p2_minus_l2, 10+i, Param(h->h1d(1,2,i,0)));
    }
    TCanvas* can_l2 = default_can_("hv_degrad_rog_p1_l2", "Scans - 2011 July - Rog (P1) l2", 650,600, 13,10,9,11, 1,1, 1,1);
    p1_plus_l2->SetMarkerColor(2);
    p1_plus_l2->SetMarkerStyle(8);
    p1_plus_l2->Draw("PE1X0");
    p1_minus_l2->SetMarkerColor(4);
    p1_minus_l2->SetMarkerStyle(8);
    p1_minus_l2->Draw("SAMEPE1X0");
    TLegend* leg5 = new TLegend(0.6,0.2,0.8,0.4,"HV groups");
    leg5->AddEntry(p1_plus_l2, "+Z side", "p");
    leg5->AddEntry(p1_minus_l2, "-Z side", "p");
    leg5->SetFillColor(0);
    leg5->SetFillStyle(0);
    leg5->SetBorderSize(0);
    leg5->SetTextSize(0.04);
    leg5->Draw();
    c.push_back(can_l2);
    TCanvas* can2_l2 = default_can_("hv_degrad_rog_p2_l2", "Scans - 2011 July - Rog (P1) l2", 650,600, 13,10,9,11, 1,1, 1,1);
    p2_plus_l2->SetMarkerColor(2);
    p2_plus_l2->SetMarkerStyle(8);
    p2_plus_l2->Draw("PE1X0");
    p2_minus_l2->SetMarkerColor(4);
    p2_minus_l2->SetMarkerStyle(8);
    p2_minus_l2->Draw("SAMEPE1X0");
    TLegend* leg6 = new TLegend(0.6,0.2,0.8,0.4,"HV groups");
    leg6->AddEntry(p2_plus_l2, "+Z side", "p");
    leg6->AddEntry(p2_minus_l2, "-Z side", "p");
    leg6->SetFillColor(0);
    leg6->SetFillStyle(0);
    leg6->SetBorderSize(0);
    leg6->SetTextSize(0.04);
    leg6->Draw();
    c.push_back(can2_l2);
  }

  void hv_degrad_mod_can_(std::vector<TCanvas*>& c, Histograms* h) {
    // l1
    TH2D* p1_l1 = new TH2D("p1_l1","p1_l1",9,-4.5,4.5,21,-10.5,10.5);
    TH2D* p2_l1 = new TH2D("p2_l1","p2_l1",9,-4.5,4.5,21,-10.5,10.5);
    setstyle_(p1_l1, "Modules", "Ladders", "Point of Half Efficiency [V]",0.9,1.0,1.2);
    setstyle_(p2_l1, "Modules", "Ladders", "Turnon Width [V]",0.9,1.0,1.2);
    for (int i=0; i<8; i++) {
      for (int j=0; j<20; j++) {
	if (!((i==0&&j==13)||(i==5&&j==18)||(i==7&&j==13))) {
	  if (!(i==7&&j==0)) {
	    SetBin(p1_l1,  p2_l1,  i+1+(i>3), j+1+(j>9),  Param(h->h1d(0,i,j,0)));
	  }
	}
      }
    }
    TCanvas* can_l1 = default_can_("hv_degrad_mod_p1_l1", "Scans - 11/6 Module (P1) l1", 650,600, 12,17,10,10, 0,0, 0,0);
    p1_l1->Draw("COLZ");
    c.push_back(can_l1);
    TCanvas* can2_l1 = default_can_("hv_degrad_mod_p2_l1", "Scans - 11/6 Module (P2) l1", 650,600, 12,17,10,10, 0,0, 0,0);
    p2_l1->Draw("COLZ");
    c.push_back(can2_l1);
    // l3
    TH2D* p1_l3 = new TH2D("p1_l3","p1_l3",9,-4.5,4.5,45,-22.5,22.5);
    TH2D* p2_l3 = new TH2D("p2_l3","p2_l3",9,-4.5,4.5,45,-22.5,22.5);
    setstyle_(p1_l3, "Modules", "Ladders", "Point of Half Efficiency [V]",0.9,1.0,1.2);
    setstyle_(p2_l3, "Modules", "Ladders", "Turnon Width [V]",0.9,1.0,1.2);
    for (int i=0; i<8; i++) {
      for (int j=0; j<44; j++) {
/* 	SetBin(p1_l3,  p2_l3,  i+1+(i>3), j+1+(j>21),  Param(h->h1d(2,i,j,0))); */
      }
    }
    TCanvas* can_l3 = default_can_("hv_degrad_mod_p1_l3", "Scans - 11/6 Module (P1) l3", 650,600, 12,17,10,10, 0,0, 0,0);
    p1_l3->Draw("COLZ");
    c.push_back(can_l3);
    TCanvas* can2_l3 = default_can_("hv_degrad_mod_p2_l3", "Scans - 11/6 Module (P2) l3", 650,600, 12,17,10,10, 0,0, 0,0);
    p2_l3->Draw("COLZ");
    c.push_back(can2_l3);
  }

  void hv_degrad_roc_can_(std::vector<TCanvas*>& c, Histograms* h) {
    // l1
    TH1D* roc_p1_l1 = new TH1D("roc_p1_l1","roc_p1_l1",16,-0.5,15.5);
    TH1D* roc_p2_l1 = new TH1D("roc_p2_l1","roc_p2_l1",16,-0.5,15.5);
    setstyle_(roc_p1_l1, "ROC #", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(roc_p2_l1, "ROC #", "Turnon Width [V]",1.0,1.0);
    for (int j=0; j<16; j++) {
      int i=3; if(j==4) i=2;
      SetBin(roc_p1_l1, roc_p2_l1, j+1,  Paramroc(h->h1d(0,j,0),i));
    }
    TCanvas* can_l1 = default_can_("hv_degrad_roc_p1_l1", "Scans - 11/6 ROC (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    roc_p1_l1->SetMarkerStyle(8);
    roc_p1_l1->SetMarkerSize(1.0);
    roc_p1_l1->Draw();
    c.push_back(can_l1);
    TCanvas* can2_l1 = default_can_("hv_degrad_roc_p2_l1", "Scans - 11/6 ROC (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    roc_p2_l1->SetMarkerStyle(8);
    roc_p2_l1->SetMarkerSize(1.0);
    roc_p2_l1->Draw();
    c.push_back(can2_l1);
    // l3
    TH1D* roc_p1_l3 = new TH1D("roc_p1_l3","roc_p1_l3",16,-0.5,15.5);
    TH1D* roc_p2_l3 = new TH1D("roc_p2_l3","roc_p2_l3",16,-0.5,15.5);
    setstyle_(roc_p1_l3, "ROC #", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(roc_p2_l3, "ROC #", "Turnon Width [V]",1.0,1.0);
    for (int j=0; j<16; j++) {
      int i=3; if(j==4) i=2;
      SetBin(roc_p1_l3, roc_p2_l3, j+1,  Paramroc(h->h1d(2,j,0),i));
    }
    TCanvas* can_l3 = default_can_("hv_degrad_roc_p1_l3", "Scans - 11/6 ROC (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    roc_p1_l3->SetMarkerStyle(8);
    roc_p1_l3->SetMarkerSize(1.0);
    roc_p1_l3->Draw();
    c.push_back(can_l3);
    TCanvas* can2_l3 = default_can_("hv_degrad_roc_p2_l3", "Scans - 11/6 ROC (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    roc_p2_l3->SetMarkerStyle(8);
    roc_p2_l3->SetMarkerSize(1.0);
    roc_p2_l3->Draw();
    c.push_back(can2_l3);
  }

  void hv_degrad_lad_can_(std::vector<TCanvas*>& c, Histograms* h) {
    // l1
    TH1D* lad_p1_l1 = new TH1D("lad_p1_l1","lad_p1_l1",21,-10.5,10.5);
    TH1D* lad_p2_l1 = new TH1D("lad_p2_l1","lad_p2_l1",21,-10.5,10.5);
    setstyle_(lad_p1_l1, "Ladders", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(lad_p2_l1, "Ladders", "Turnon Width [V]",1.0,1.0);
    for (int j=0; j<20; j++) {
      SetBin(lad_p1_l1, lad_p2_l1, j+1+(j>9),  Param(h->h1d(0,j,0)));
    }
    TCanvas* can_l1 = default_can_("hv_degrad_lad_p1_l1", "Scans - 11/6 Ladder (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    lad_p1_l1->SetMarkerStyle(8);
    lad_p1_l1->SetMarkerSize(1.0);
    lad_p1_l1->Draw();
    TLatex* p1_l1_lat= new TLatex(0.0,15.0, "MODULE 1 Only"); p1_l1_lat->SetLineWidth(2); p1_l1_lat->SetTextSize(0.04); p1_l1_lat->Draw();
    c.push_back(can_l1);
    TCanvas* can2_l1 = default_can_("hv_degrad_lad_p2_l1", "Scans - 11/6 Ladder (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    lad_p2_l1->SetMarkerStyle(8);
    lad_p2_l1->SetMarkerSize(1.0);
    lad_p2_l1->Draw();
    TLatex* p2_l1_lat= new TLatex(0.0,1.0, "MODULE 1 Only"); p2_l1_lat->SetLineWidth(2); p2_l1_lat->SetTextSize(0.04); p2_l1_lat->Draw();
    c.push_back(can2_l1);
    // l3
    TH1D* lad_p1_l3 = new TH1D("lad_p1_l3","lad_p1_l3",45,-22.5,22.5);
    TH1D* lad_p2_l3 = new TH1D("lad_p2_l3","lad_p2_l3",45,-22.5,22.5);
    setstyle_(lad_p1_l3, "Ladders", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(lad_p2_l3, "Ladders", "Turnon Width [V]",1.0,1.0);
    for (int j=0; j<44; j++) {
      SetBin(lad_p1_l3, lad_p2_l3, j+1+(j>21),  Param(h->h1d(2,j,0)));
    }
    TCanvas* can_l3 = default_can_("hv_degrad_lad_p1_l3", "Scans - 11/6 Ladder (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    lad_p1_l3->SetMarkerStyle(8);
    lad_p1_l3->SetMarkerSize(1.0);
    lad_p1_l3->Draw();
    TLatex* p1_l3_lat= new TLatex(0.0,15.0, "MODULE 1 Only"); p1_l3_lat->SetLineWidth(2); p1_l3_lat->SetTextSize(0.04); p1_l3_lat->Draw();
    c.push_back(can_l3);
    TCanvas* can2_l3 = default_can_("hv_degrad_lad_p2_l3", "Scans - 11/6 Ladder (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    lad_p2_l3->SetMarkerStyle(8);
    lad_p2_l3->SetMarkerSize(1.0);
    lad_p2_l3->Draw();
    TLatex* p2_l3_lat= new TLatex(0.0,1.0, "MODULE 1 Only"); p2_l3_lat->SetLineWidth(2); p2_l3_lat->SetTextSize(0.04); p2_l3_lat->Draw();
    c.push_back(can2_l3);
    // l2
    TH1D* lad_p1_l2 = new TH1D("lad_p1_l2","lad_p1_l2",33,-16.5,16.5);
    TH1D* lad_p2_l2 = new TH1D("lad_p2_l2","lad_p2_l2",33,-16.5,16.5);
    setstyle_(lad_p1_l2, "Ladders", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(lad_p2_l2, "Ladders", "Turnon Width [V]",1.0,1.0);
    for (int j=0; j<32; j++) {
      SetBin(lad_p1_l2, lad_p2_l2, j+1+(j>15),  Param(h->h1d(1,j,0)));
    }
    TCanvas* can_l2 = default_can_("hv_degrad_lad_p1_l2", "Scans - 11/6 Ladder (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    lad_p1_l2->SetMarkerStyle(8);
    lad_p1_l2->SetMarkerSize(1.0);
    lad_p1_l2->Draw();
    TLatex* p1_l2_lat= new TLatex(0.0,15.0, "MODULE 1 Only"); p1_l2_lat->SetLineWidth(2); p1_l2_lat->SetTextSize(0.04); p1_l2_lat->Draw();
    c.push_back(can_l2);
    TCanvas* can2_l2 = default_can_("hv_degrad_lad_p2_l2", "Scans - 11/6 Ladder (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    lad_p2_l2->SetMarkerStyle(8);
    lad_p2_l2->SetMarkerSize(1.0);
    lad_p2_l2->Draw();
    TLatex* p2_l2_lat= new TLatex(0.0,1.0, "MODULE 1 Only"); p2_l2_lat->SetLineWidth(2); p2_l2_lat->SetTextSize(0.04); p2_l2_lat->Draw();
    c.push_back(can2_l2);
  }

  void hv_degrad_ring_can_(std::vector<TCanvas*>& c, Histograms* h) {
    // l1
    TH1D* ring_p1_l1 = new TH1D("ring_p1_l1","ring_p1_l1",9,-4.5,4.5);
    TH1D* ring_p2_l1 = new TH1D("ring_p2_l1","ring_p2_l1",9,-4.5,4.5);
    setstyle_(ring_p1_l1, "Rings", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(ring_p2_l1, "Rings", "Turnon Width [V]",1.0,1.0);
    for (int i=0; i<8; i++) {
      SetBin(ring_p1_l1, ring_p2_l1, i+1+(i>3),  Param(h->h1d(0,i,0)));
    }
    TCanvas* can_l1 = default_can_("hv_degrad_ring_p1_l1", "Scans - 11/6 Ring (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    ring_p1_l1->SetMarkerStyle(8);
    ring_p1_l1->SetMarkerSize(1.0);
    ring_p1_l1->Draw("PE1X0");
    c.push_back(can_l1);
    TCanvas* can2_l1 = default_can_("hv_degrad_ring_p2_l1", "Scans - 11/6 Ring (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    ring_p2_l1->SetMarkerStyle(8);
    ring_p2_l1->SetMarkerSize(1.0);
    ring_p2_l1->Draw();
    c.push_back(can2_l1);
    // l3
    TH1D* ring_p1_l3 = new TH1D("ring_p1_l3","ring_p1_l3",9,-4.5,4.5);
    TH1D* ring_p2_l3 = new TH1D("ring_p2_l3","ring_p2_l3",9,-4.5,4.5);
    setstyle_(ring_p1_l3, "Rings", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(ring_p2_l3, "Rings", "Turnon Width [V]",1.0,1.0);
    for (int i=0; i<8; i++) {
      SetBin(ring_p1_l3, ring_p2_l3, i+1+(i>3),  Param(h->h1d(2,i,0)));
    }
    TCanvas* can_l3 = default_can_("hv_degrad_ring_p1_l3", "Scans - 11/6 Ring (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    ring_p1_l3->SetMarkerStyle(8);
    ring_p1_l3->SetMarkerSize(1.0);
    ring_p1_l3->Draw("PE1X0");
    c.push_back(can_l3);
    TCanvas* can2_l3 = default_can_("hv_degrad_ring_p2_l3", "Scans - 11/6 Ring (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    ring_p2_l3->SetMarkerStyle(8);
    ring_p2_l3->SetMarkerSize(1.0);
    ring_p2_l3->Draw();
    c.push_back(can2_l3);
    // l2
    TH1D* ring_p1_l2 = new TH1D("ring_p1_l2","ring_p1_l2",9,-4.5,4.5);
    TH1D* ring_p2_l2 = new TH1D("ring_p2_l2","ring_p2_l2",9,-4.5,4.5);
    setstyle_(ring_p1_l2, "Rings", "Point of Half Efficiency [V]",1.0,1.0);
    setstyle_(ring_p2_l2, "Rings", "Turnon Width [V]",1.0,1.0);
    for (int i=0; i<8; i++) {
      SetBin(ring_p1_l2, ring_p2_l2, i+1+(i>3),  Param(h->h1d(1,i,0)));
    }
    TCanvas* can_l2 = default_can_("hv_degrad_ring_p1_l2", "Scans - 11/6 Ring (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    ring_p1_l2->SetMarkerStyle(8);
    ring_p1_l2->SetMarkerSize(1.0);
    ring_p1_l2->Draw("PE1X0");
    c.push_back(can_l2);
    TCanvas* can2_l2 = default_can_("hv_degrad_ring_p2_l2", "Scans - 11/6 Ring (P1)", 650,600, 13,10,9,11, 1,1, 1,1);
    ring_p2_l2->SetMarkerStyle(8);
    ring_p2_l2->SetMarkerSize(1.0);
    ring_p2_l2->Draw();
    c.push_back(can2_l2);
  }

  float delay_turnoff_(TH1D* h, float end) {
    float optimal = -9999.0;
    if (h->GetEntries()) {
      //TCanvas tempcan("tempcan");
      // Find  the end of Plateau
      double max = h->GetBinContent(h->GetMaximumBin());
      float eop = 0;
      float eopcont = 0;
      TH1D* plateau = new TH1D("plateau","plateau",10001,0,1.0001);
      for (int i=1; i<=h->GetNbinsX(); i++) {
	double cont = h->GetBinContent(i);
	if (h->GetBinContent(i)>max-0.0065) {
	  eopcont = cont;
	  eop = h->GetBinCenter(i);
	  plateau->Fill(cont);
	}
      }
      TF1* f = new TF1("turnoff", "[0]/(1+exp((x-[1])/[2]))", eop-8, eop+5);
      f->SetParameter(0,max);
      f->SetParLimits(0,0,1);
      f->SetParameter(1,eop+5.8);
      f->SetParLimits(1,eop+2,eop+9.0);
      f->SetParameter(2,1);
      f->SetParLimits(2,0.4,2.5);
      h->Fit("turnoff","RQ0");
      if (f!=0) {
	double fmax = f->GetMaximum();
	optimal = f->GetX(fmax-0.01);
	if ((eop==end&&!((plateau->GetMean()-eopcont)/plateau->GetRMS()>2))||(f->GetMinimum()>=fmax-0.03)) {
	  // Flat, no turnoff
	  optimal = -99.0;
	} else {
	  // Refit if Width near Limit (too few points near end)
	  // This is a good average of many fits
	  double Width = f->GetParameter(2);
	  if (Width<0.41) {
	    f->SetParameter(2,1.007);
	    f->SetParLimits(2,1.007,1.008);
	    h->Fit("turnoff","RQ0");
	    optimal = f->GetX(fmax-0.01);
	  }
	}
      } else optimal = -999.0;
      delete plateau;
    } else optimal = -9999.0;
    return optimal;
  }
  
  float delay_turnon_(TH1D* h, float beg = -6.0) {
    float optimal = -9999.0;
    if (h->GetEntries()) {
      // Find  the beginning of the Plateau
      float bop = -99;
      double max = h->GetBinContent(h->GetMaximumBin());
      int first = 1;
      double thiscont = 0;
      double prevcont = 0;
      double prev2cont = 0;
      for (int i=h->GetNbinsX(); i>0; i--) {
	double cont = h->GetBinContent(i);
	if (cont>0) {
	  prev2cont = prevcont;
	  prevcont = thiscont;
	  thiscont = cont;
	  if ((cont>max-0.0065)&&(prevcont>max-0.0065)&&(prev2cont>max-0.0065)) {
	    bop = h->GetBinCenter(i);
	    first = i;
	  }
	}
      }
      if (bop==-99) {
	for (int i=h->GetNbinsX(); i>0; i--) {
	  double cont = h->GetBinContent(i);
	  if (cont>0) {
	    prev2cont = prevcont;
	    prevcont = thiscont;
	    thiscont = cont;
	    if (cont>max-0.01) {
	      bop = h->GetBinCenter(i);
	      first = i;
	    }
	  }
	}
      }
      // Find last fitted value
      int lowrange = 0;
      for (int i=first; i>=1&&i>=first-10; i--) {
	double cont = h->GetBinContent(i);
	if (cont>0&&cont<max-0.0035) lowrange = first - i;
      }
      TF1* f = new TF1("turnon", "[0]/(1+exp(([1]-x)/[2]))", bop-lowrange, bop+8);
      f->SetParameter(0,max);
      f->SetParLimits(0,0,1);
      f->SetParameter(1,bop-4.9); // -0.77
      f->SetParLimits(1,bop-20,bop-2); // -4, 2.2
      f->SetParameter(2,0.92);
      f->SetParLimits(2,0.3,2.0);
      h->Fit("turnon","RQ0");
      if (f!=0) {
	double fmax = f->GetMaximum();
	optimal = f->GetX(fmax-0.01);
	if (bop==beg||(f->GetMinimum()>=fmax-0.015)) {
	  optimal = -99.0; // Flat, no turnon
	} else {
	  // Refit if Width near Limit (too few points near end)
	  // This is a good average of many fits
	  double Width = f->GetParameter(2);
	  if (Width<0.31) {
	    f->SetParameter(2,0.92);
	    f->SetParLimits(2,0.92,0.925);
	    h->Fit("turnon","RQ0");
	    optimal = f->GetX(fmax-0.01);
	  }
	}
      } else optimal = -999.0;
    } else optimal = -9999.0;
    return optimal;
  }
  
  
  float find_turnoff_(TH1D* h, float end, float plat, int scan) {
    float beg = -6;
    float turnoff = delay_turnoff_(h, end);
    float turnon = (scan<2) ? delay_turnon_(h, beg) : -99.0;
    return (turnoff>-99.0) ? turnoff : ((turnon>-99.0) ? turnon + plat : -99.0);
  }
  
  double find_mpvmax_(TH1D* h) {
    double mpvmax = -999.0;
    if (h->GetEntries()) {
      double max = 0;
      for (int i=1; i<=h->GetNbinsX(); i++) {
	double cont = h->GetBinContent(i);
	double cen = h->GetBinCenter(i);
	if (cont>max&&cen>0&&cen<27) {
	  max = cont;
	  mpvmax = cen;
	}
      }
    } else mpvmax = -99.0;
    return mpvmax;
  }
  
  double fit_mpvmax_(TH1D* h) {
    double mpvmax = -9999.0;
    if (h->GetEntries()>1) {
      TF1* f = new TF1("mpvmax", "[0]+([1]+[2]*x)/(1+exp((x-[3])/[4]))", 8, 24);
      f->SetParameter(0,0.08);
      f->SetParLimits(0,0,0.5);
      f->SetParameter(1,0.88);
      f->SetParLimits(1,0.5,1);
      f->SetParameter(2,0.0025);
      f->SetParLimits(2,0.0001,0.01);
      f->SetParameter(3,21);
      f->SetParLimits(3,16,24);
      f->SetParameter(4,1.0);
      f->SetParLimits(4,0.1,2);
      h->Fit("mpvmax","RMQ0");
      if (f!=0) mpvmax = f->GetX(f->GetMaximum());
      else mpvmax = -999.0;
    } else mpvmax = -9999.0;
    return mpvmax;
  }


  // float find_turnoff_bpix_(TH1D* h, float end = 20, float plat=18.91) {
  //   float beg = -6;
  //   float turnoff = delay_turnoff_(h, end);
  //   float turnon = delay_turnon_bpix_(h, beg);
  //   return (turnoff>-99.0) ? turnoff : ((turnon>-99.0) ? turnon + plat : -99.0);
  // }


  float get_efficiency_(TH1D* h, float delay) {
    Int_t bin1 = h->FindBin(delay);
    Float_t bin1cent = h->GetBinCenter(bin1);
    Float_t eff1 = h->GetBinContent(bin1);
    Float_t eff2 = (delay-bin1cent>0) ? h->GetBinContent(bin1+1) : h->GetBinContent(bin1-1);
    return (eff1 + (eff2-eff1)*fabs(delay-bin1cent));
  }
  
  void timing_can_(std::vector<TCanvas*>& c, AllHistos& ah, Variables& v, PostFixes& p) {
    std::vector<std::string> scan_pf;
    scan_pf.push_back("_11");
    scan_pf.push_back("_12");
    scan_pf.push_back("_3");
    scan_pf.push_back("_4");
    std::vector<std::string> rog_pf;
    rog_pf.push_back("_lyr12");
    rog_pf.push_back("_lyr3");
    rog_pf.push_back("_fpix");
    std::vector<std::string> rog2_pf;
    rog2_pf.push_back("_lyr1");
    rog2_pf.push_back("_lyr2");
    rog2_pf.push_back("_lyr3");
    rog2_pf.push_back("_fpix");
    rog2_pf.push_back("_dm2");
    rog2_pf.push_back("_dm1");
    rog2_pf.push_back("_dp1");
    rog2_pf.push_back("_dp2");
    std::vector<std::string> pf1;
    pf1.push_back("_rog");
    pf1.push_back("_mod");
    pf1.push_back("_roc");
    std::vector<std::string> pf2;
    pf2.push_back("_modrog");
    pf2.push_back("_rocmod");
    pf2.push_back("_rocrog");
    std::vector<std::string> Scan;
    Scan.push_back("2011 March");
    Scan.push_back("2012 April");
    Scan.push_back("2012 July - New Vana");
    Scan.push_back("2012 July - Old Vana");
    std::vector<std::string> Rog;
    Rog.push_back("Layer 1-2");
    Rog.push_back("Layer 3");
    Rog.push_back("Forward Pixel");
    std::vector<std::string> Rog2;
    Rog2.push_back("Layer 1");
    Rog2.push_back("Layer 2");
    Rog2.push_back("Layer 3");
    Rog2.push_back("Forward Pixel");
    Rog2.push_back("Disk -2");
    Rog2.push_back("Disk -1");
    Rog2.push_back("Disk +1");
    Rog2.push_back("Disk +2");
    std::vector<std::string> Pf1;
    Pf1.push_back("ROG Turnoff");
    Pf1.push_back("Module Turnoff");
    Pf1.push_back("ROC Turnoff");
    std::vector<std::string> YTitle1;
    YTitle1.push_back("N_{Readout Group}[/0.25ns]");
    YTitle1.push_back("N_{module}[/0.25ns]");
    YTitle1.push_back("N_{Readout Chip}[/0.25ns]");
    std::vector<std::string> YTitle2;
    YTitle2.push_back("N_{module}[/0.5ns]");
    YTitle2.push_back("N_{Readout Chip}[/0.5ns]");
    YTitle2.push_back("N_{Readout Chip}[/0.5ns]");
    std::vector<std::string> Pf2;
    Pf2.push_back("Module Variation (wrt Readout Group)");
    Pf2.push_back("ROC Variation (wrt Module)");
    Pf2.push_back("ROC Variation (wrt Readout Group)");
    Histograms *turnoff   = new Histograms("turnoff",  3,1);
    Histograms *mpvmax    = new Histograms("mpvmax",   3,1);
    Histograms *diff      = new Histograms("diff",     3,1);
    Histograms *variation = new Histograms("variation",3,1);
    // Histograms *eta       = new Histograms("eta",      2,1);
    // Histograms *phi       = new Histograms("phi",      2,1);
    turnoff  ->add(scan_pf, 1, 1, 4, 0, pf1,  1, 1, 3, 0, rog_pf,  1, 1, 3, 100,  5, 30);
    mpvmax   ->add(scan_pf, 1, 1, 4, 0, pf1,  1, 1, 3, 0, rog_pf,  1, 1, 3, 100,  5, 30);
    diff     ->add(scan_pf, 1, 1, 4, 0, pf1,  1, 1, 3, 0, rog_pf,  1, 1, 3, 40, -10, 10);
    variation->add(scan_pf, 1, 1, 4, 0, pf2,  1, 1, 1, 0, rog2_pf, 1, 1, 4, 40, -10, 10);
    variation->add(scan_pf, 0, 1, 4, 0, pf2,  1, 2, 3, 0, rog_pf,  1, 1, 3, 40, -10, 10);
    // eta      ->add(scan_pf, 1, 1, 4, 0, rog2_pf, 1, 1, 3,  9, -4.5, 4.5);
    // eta      ->add(scan_pf, 0, 1, 4, 0, rog2_pf, 1, 4, 8,  7,  0.5, 7.5);
    // eta      ->add(scan_pf, 1, 1, 4, 0, rog2_pf, 1, 1, 3,  9, -4.5, 4.5, 3000, 10, 30);
    // eta      ->add(scan_pf, 0, 1, 4, 0, rog2_pf, 1, 4, 8,  7,  0.5, 7.5, 3000, 10, 30);
    // phi      ->add(scan_pf, 1, 1, 4, 0, rog2_pf, 1, 1, 3,  45, -22.5, 22.5);
    // phi      ->add(scan_pf, 0, 1, 4, 0, rog2_pf, 1, 4, 8,  25, -12.5, 12.5);
    // phi      ->add(scan_pf, 1, 1, 4, 0, rog2_pf, 1, 1, 3,  45, -22.5, 22.5, 3000, 10, 30);
    // phi      ->add(scan_pf, 0, 1, 4, 0, rog2_pf, 1, 4, 8,  25, -12.5, 12.5, 3000, 10, 30);
    
    float rog_bpix[4][4][8][2];
    float rog_fpix[4][4][2][4];
    float mod_bpix[4][4][48][4];
    float roc_bpix[4][4][48][64];
    float mod_fpix[4][4][24][7];
    float roc_fpix[4][4][24][45];
    float mpv_rog_bpix[4][4][8][2];
    float mpv_rog_fpix[4][4][2][4];
    float mpv_mod_bpix[4][4][48][4];
    float mpv_roc_bpix[4][4][48][64];
    float mpv_mod_fpix[4][4][24][7];
    float mpv_roc_fpix[4][4][24][45];
    for (int scan=0; scan<2; scan++) {
      for (int i=0; i<4; i++) {
	for (int j=0; j<8; j++) for (int k=0; k<2; k++) {
	  rog_bpix[scan][i][j][k] = -99.0;
	  mpv_rog_bpix[scan][i][j][k] = -99.0;
	}
	for (int j=0; j<2; j++) for (int k=0; k<4; k++) {
	  rog_fpix[scan][i][j][k] = -99.0;
	  mpv_rog_fpix[scan][i][j][k] = -99.0;
	}
	for (int j=0; j<48; j++) {
	  for (int k=0; k<4;  k++) {
	    mod_bpix[scan][i][j][k] = -99.0;
	    mpv_mod_bpix[scan][i][j][k] = -99.0;
	  }
	  for (int k=0; k<64; k++) {
	    roc_bpix[scan][i][j][k] = -99.0;
	    mpv_roc_bpix[scan][i][j][k] = -99.0;
	  }
	}     
	for (int j=0; j<24; j++) {
	  for (int k=0; k<7;  k++) {
	    mod_fpix[scan][i][j][k] = -99.0;
	    mpv_mod_fpix[scan][i][j][k] = -99.0;
	  }
	  for (int k=0; k<45; k++) {
	    roc_fpix[scan][i][j][k] = -99.0;
	    roc_fpix[scan][i][j][k] = -99.0;
	  }
	}
      }
    }
    for  (int scan = 0; scan<4; scan++) {
      //Histograms *delay = (scan==0) ? ah.delay11 : (scan==1) ? ah.delay12 : (scan==2) ? ah.delay3 : ah.delay4;
      Histograms *delay_rog = (scan==0) ? ah.delay11_rog : (scan==1) ? ah.delay12_rog : (scan==2) ? ah.delay3_rog : ah.delay4_rog;
      Histograms *delay_mod = (scan==0) ? ah.delay11_mod : (scan==1) ? ah.delay12_mod : (scan==2) ? ah.delay3_mod : ah.delay4_mod;
      Histograms *delay_roc = (scan==0) ? ah.delay11_roc : (scan==1) ? ah.delay12_roc : (scan==2) ? ah.delay3_roc : ah.delay4_roc;
      gStyle->SetPadTickY(0);
      for (int det=0; det<10; det++) {
	// if (scan==1) {
	//   // With Clusize
	//   // Float_t localmax = (det==2||det>7) ? 14 : (det>5) ? 17 : NOVAL_F;
	//   // Float_t xmin[10]  = {  -9,   -9,   -9,    12,    12,    12,    -9,    -9,    -9,    -9 };
	//   // Float_t xmax[10]  = {  24,   24,   24,    17,    17,    17,    29,    29,    29,    29 };
	//   // Float_t lymin[10] = { 2.4,  2.5,  1.6,   4.7,   3.85,  3.3,   1.5,   1.5,   1.5,   1.5 };
	//   // Float_t lymax[10] = { 4.4,  5.0,  2.1,   5.3,   4.15,  3.6,   2.1,   2.1,   2.1,   2.1 };
	//   // Float_t rymin[10] = { 0.3, 0.25,  0.6,  0.96,   0.96, 0.96,  0.25,  0.25,  0.25,  0.25 };
	//   // c.push_back(custom_can_(delay->h1d(det,2), "delay_clusize"+p.det[det], "2012 April Data - "+p.Det[det], "Delay [ns]", "On-Track Cluster Size [pixel]", 1, 1, 480, 480, 80,80));
	//   // draw_with_right_yaxis_(delay->h1d(det,2), delay->h1d(det,0), "P", "Efficiency", xmin[det],xmax[det], lymin[det],lymax[det], rymin[det], NOVAL_F, localmax, 1.005); 
	//   // save_canvas_(can,"/home/common/Plotok_Viktornak/TimingScan12/");
	//   // save_canvas_(can,"/afs/kfki.hu/home/jkarancs/public/Scans/TimingScan12/");
	//   // With MPV - Zoomed
	//   Float_t xmin[10]     = {   -5,   -5,   -5,    11,     11,   11,    10,    10,    12,    12 };
	//   Float_t xmax[10]     = {   21,   21,   21,    17,     17,   17,    16,    18,    18,    18 };
	//   Float_t lymin[10]    = {   19, 19.5, 16.8,  20.4,   21.5, 21.5, 17.25, 17.05,  17.7, 17.45 };
	//   Float_t lymax[10]    = {   22, 22.5, 17.8,  21.1,   22.2, 22.2,  17.6,  17.4, 17.95,  17.7 };
	//   Float_t rymin[10]    = { 0.75, 0.75, 0.65,  0.97,   0.97, 0.97,  0.94,  0.98,  0.98,  0.98 };
	//   Float_t localmax[10] = {   15,   14,   13,    14,     14,   14,    12,    13,    16,    17 };
	//   // Float_t rymax = det==2||det>5 ? 1.1 : NOVAL_F; 
	//   c.push_back(custom_can_(delay->h1d(det,4), delay->histoname()+p.det[det], "2012 April Data - "+p.Det[det], "Delay [ns]", "Normalized Cluster Charge MPV [ke]", 1, 1, 480, 480, 80,80));
	//   draw_with_right_yaxis_(delay->h1d(det,4), delay->h1d(det,0), "P", "Efficiency", xmin[det],xmax[det], lymin[det],lymax[det], rymin[det], NOVAL_F, localmax[det], 1.005);
	//   // save_canvas_(can,"/home/common/Plotok_Viktornak/TimingScan12/");
	//   // save_canvas_(can,"/afs/kfki.hu/home/jkarancs/public/Scans/TimingScan12/");
	// }
      }
      gStyle->SetPadTickY(1);
      float cableing_corr = (scan==1) ? 6.0 : 0.0; // Set to 6 for 2012 April
      float end = (scan==0) ? 25 : (scan==1) ? 20 : 24; // Last measured point
      for (int shl=0; shl<4; shl++) {
	//Optimal timing for Readout groups
	for (int sec=0; sec<8; sec++) { // BPix
	  rog_bpix[scan][shl][sec][0] = find_turnoff_(delay_rog->h1d(shl,sec,0,0), end, 17.69, scan);
	  rog_bpix[scan][shl][sec][1] = find_turnoff_(delay_rog->h1d(shl,sec,1,0), end, 17.21, scan);
	  if (rog_bpix[scan][shl][sec][0]>-99.0) turnoff->h1d(scan,0,0)->Fill(rog_bpix[scan][shl][sec][0]);
	  if (rog_bpix[scan][shl][sec][1]>-99.0) turnoff->h1d(scan,0,1)->Fill(rog_bpix[scan][shl][sec][1]);
	  if (scan!=0&&rog_bpix[0][shl][sec][0]>-99&&rog_bpix[scan][shl][sec][0]>-99) diff->h1d(scan,0,0)->Fill(rog_bpix[scan][shl][sec][0]-rog_bpix[0][shl][sec][0]);
	  if (scan!=0&&rog_bpix[0][shl][sec][1]>-99&&rog_bpix[scan][shl][sec][1]>-99) diff->h1d(scan,0,1)->Fill(rog_bpix[scan][shl][sec][1]-rog_bpix[0][shl][sec][1]);
	  // mpv
	  mpv_rog_bpix[scan][shl][sec][0] = fit_mpvmax_(delay_rog->h1d(shl,sec,0,4));
	  mpv_rog_bpix[scan][shl][sec][1] = fit_mpvmax_(delay_rog->h1d(shl,sec,1,4));
	  if (mpv_rog_bpix[scan][shl][sec][0]>-99.0) mpvmax->h1d(scan,0,0)->Fill(mpv_rog_bpix[scan][shl][sec][0]);
	  if (mpv_rog_bpix[scan][shl][sec][1]>-99.0) mpvmax->h1d(scan,0,1)->Fill(mpv_rog_bpix[scan][shl][sec][1]);
	}
	for (int rog=0; rog<4; rog++) { // Fpix
	  rog_fpix[scan][shl][0][rog] = find_turnoff_(delay_rog->h1d(shl,8,rog,0), end, 17.15, scan);
	  rog_fpix[scan][shl][1][rog] = find_turnoff_(delay_rog->h1d(shl,9,rog,0), end, 17.15, scan)+(shl==3&&rog==0)*cableing_corr;
	  if (rog_fpix[scan][shl][0][rog]>-99.0) turnoff->h1d(scan,0,2)->Fill(rog_fpix[scan][shl][0][rog]);
	  if (rog_fpix[scan][shl][1][rog]>-99.0) turnoff->h1d(scan,0,2)->Fill(rog_fpix[scan][shl][1][rog]);
	  if (scan!=0&&rog_fpix[0][shl][0][rog]>-99&&rog_fpix[scan][shl][0][rog]>-99) diff->h1d(scan,0,2)->Fill(rog_fpix[scan][shl][0][rog]-rog_fpix[0][shl][0][rog]);
	  if (scan!=0&&rog_fpix[0][shl][1][rog]>-99&&rog_fpix[scan][shl][1][rog]>-99) diff->h1d(scan,0,2)->Fill(rog_fpix[scan][shl][1][rog]-rog_fpix[0][shl][1][rog]);
	  // mpv
	  mpv_rog_fpix[scan][shl][0][rog] = fit_mpvmax_(delay_rog->h1d(shl,8,rog,4));
	  mpv_rog_fpix[scan][shl][1][rog] = fit_mpvmax_(delay_rog->h1d(shl,9,rog,4))+(shl==3&&rog==0)*cableing_corr;
	  if (mpv_rog_fpix[scan][shl][0][rog]>-99.0) mpvmax->h1d(scan,0,2)->Fill(mpv_rog_fpix[scan][shl][0][rog]);
	  if (mpv_rog_fpix[scan][shl][1][rog]>-99.0) mpvmax->h1d(scan,0,2)->Fill(mpv_rog_fpix[scan][shl][1][rog]);
	}
	// Modules/ROCs
	for (int lad=0; lad<48; lad++) { // BPix
	  int sec = (lad<10) ? lad - 1 + (lad==0) - (lad==9) : (lad<26) ? (lad-10)/2 : (lad-26+(lad>36)*2)/3;
	  for (int mod=0; mod<4; mod++) {
	    float plat = mod==0 ? (lad<10 ? 17.34 : lad<26 ? 17.1 : 16.72) : mod==1 ? (lad<10 ? 18.58 : lad<26 ? 18.02 : 17.45)
	      : mod==2 ? (lad<10 ? 20.63 : lad<26 ? 20.36 : 18.96) : (lad<10 ? 21.15 : lad<26 ? 22.67 : 20.58);
	    mod_bpix[scan][shl][lad][mod] = find_turnoff_(delay_mod->h1d(shl,lad,mod,0), end, plat, scan);
	    if (mod_bpix[scan][shl][lad][mod]>-99.0&&rog_bpix[scan][shl][sec][lad>=26]>-99.0) {
	      turnoff->h1d(scan,1,lad>=26)->Fill(mod_bpix[scan][shl][lad][mod]);
	      variation->h1d(scan,0,lad<10 ? 0 : lad<26 ? 1 : 2)->Fill(mod_bpix[scan][shl][lad][mod]-rog_bpix[scan][shl][sec][lad>=26]);
	      // eta->h2d(scan,(lad>=10)+(lad>=26))->Fill(shl<2 ? mod+1 : -1-mod, mod_bpix[scan][shl][lad][mod]);
	      // int ldr = (lad<10) ? lad + 1 : (lad<26) ? lad - 9 : lad - 25;
	      // if (mod==0) phi->h2d(scan,(lad>=10)+(lad>=26))->Fill(shl%2==0 ? ldr : -ldr, mod_bpix[scan][shl][lad][mod]);
	    }
	    if (scan!=0&&mod_bpix[0][shl][lad][mod]>-99&&mod_bpix[scan][shl][lad][mod]>-99) diff->h1d(scan,1,lad>=26)->Fill(mod_bpix[scan][shl][lad][mod]-mod_bpix[0][shl][lad][mod]);
	    // mpv
	    mpv_mod_bpix[scan][shl][lad][mod] = fit_mpvmax_(delay_mod->h1d(shl,lad,mod,4));
	    if (mpv_mod_bpix[scan][shl][lad][mod]>-99.0&&mpv_rog_bpix[scan][shl][sec][lad>=26]>-99.0)
	      mpvmax->h1d(scan,1,lad>=26)->Fill(mpv_mod_bpix[scan][shl][lad][mod]);
	  }
	  for (int modroc=0; modroc<64; modroc++) {
	    float plat = modroc<16 ? (lad<10 ? 17.34 : lad<26 ? 17.1 : 16.72) : modroc<32 ? (lad<10 ? 18.58 : lad<26 ? 18.02 : 17.45)
	      : modroc<48 ? (lad<10 ? 20.63 : lad<26 ? 20.36 : 18.96) : (lad<10 ? 21.15 : lad<26 ? 22.67 : 20.58);
	    roc_bpix[scan][shl][lad][modroc] = find_turnoff_(delay_roc->h1d(shl,lad,modroc,0), end, plat, scan);
	    if (roc_bpix[scan][shl][lad][modroc]>-99.0&&rog_bpix[scan][shl][sec][lad>25]>-99.0) {
	      turnoff->h1d(scan,2,lad>=26)->Fill(roc_bpix[scan][shl][lad][modroc]);
	      variation->h1d(scan,1,lad>=26)->Fill(roc_bpix[scan][shl][lad][modroc]-mod_bpix[scan][shl][lad][modroc/16]);
	      variation->h1d(scan,2,lad>=26)->Fill(roc_bpix[scan][shl][lad][modroc]-rog_bpix[scan][shl][sec][lad>25]);
	    }
	    if (scan!=0&&roc_bpix[0][shl][lad][modroc]>-99&&roc_bpix[scan][shl][lad][modroc]>-99) diff->h1d(scan,2,lad>=26)->Fill(roc_bpix[scan][shl][lad][modroc]-roc_bpix[0][shl][lad][modroc]);
	    // mpv
	    mpv_roc_bpix[scan][shl][lad][modroc] = fit_mpvmax_(delay_roc->h1d(shl,lad,modroc,4));
	    if (mpv_roc_bpix[scan][shl][lad][modroc]>-99.0&&mpv_rog_bpix[scan][shl][sec][lad>25]>-99.0)
	      mpvmax->h1d(scan,2,lad>=26)->Fill(mpv_roc_bpix[scan][shl][lad][modroc]);
	  }
	}
	for (int dskbld=0; dskbld<24; dskbld++) { // FPix
	  int rog = (dskbld%12)/3;
	  for (int pnlplq=0; pnlplq<7; pnlplq++) {
	    mod_fpix[scan][shl][dskbld][pnlplq] = find_turnoff_(delay_mod->h1d(shl+4,dskbld,pnlplq,0), end, 17.95, scan)+(shl==3&&dskbld>=12&&dskbld<15)*cableing_corr;
	    if (mod_fpix[scan][shl][dskbld][pnlplq]>-99.0&&rog_fpix[scan][shl][dskbld>11][rog]>-99.0) {
	      turnoff->h1d(scan,1,2)->Fill(mod_fpix[scan][shl][dskbld][pnlplq]);
	      variation->h1d(scan,0,3)->Fill(mod_fpix[scan][shl][dskbld][pnlplq]-rog_fpix[scan][shl][dskbld>11][rog]);
	      //int plq_eta = (pnlplq<4) * pnlplq * 2 + (pnlplq>3) * ((pnlplq-3)*2 -1) + 1;
	      //int disk = (shl<2) ? 6 + (dskbld>=12) : 4 + (dskbld<12);
	      //eta->h2d(scan,disk)->Fill(plq_eta, mod_fpix[scan][shl][dskbld][pnlplq]);
	      //phi->h2d(scan,disk)->Fill(shl%2==0 ? dskbld%12 +1 : - dskbld%12 -1, mod_fpix[scan][shl][dskbld][pnlplq]);
	    }
	    if (scan!=0&&mod_fpix[0][shl][dskbld][pnlplq]>-99&&mod_fpix[scan][shl][dskbld][pnlplq]>-99) diff->h1d(scan,1,2)->Fill(mod_fpix[scan][shl][dskbld][pnlplq]-mod_fpix[0][shl][dskbld][pnlplq]);
	    // mpv
	    mpv_mod_fpix[scan][shl][dskbld][pnlplq] = fit_mpvmax_(delay_mod->h1d(shl+4,dskbld,pnlplq,4))+(shl==3&&dskbld>=12&&dskbld<15)*cableing_corr;
	    if (mpv_mod_fpix[scan][shl][dskbld][pnlplq]>-99.0&&mpv_rog_fpix[scan][shl][dskbld>11][rog]>-99.0)
	      mpvmax->h1d(scan,1,2)->Fill(mpv_mod_fpix[scan][shl][dskbld][pnlplq]);
	  }
	  for (int pnlplqroc=0; pnlplqroc<45; pnlplqroc++) {
	    roc_fpix[scan][shl][dskbld][pnlplqroc] = find_turnoff_(delay_roc->h1d(shl+4,dskbld,pnlplqroc,0), end, 17.95, scan)+(shl==3&&dskbld>=12&&dskbld<15)*cableing_corr;
	    if (roc_fpix[scan][shl][dskbld][pnlplqroc]>-99.0&&rog_fpix[scan][shl][dskbld>11][rog]>-99.0) {
	      turnoff->h1d(scan,2,2)->Fill(roc_fpix[scan][shl][dskbld][pnlplqroc]);
	      int pnlplq = (pnlplqroc>=2) + (pnlplqroc>=8) + (pnlplqroc>=16) + (pnlplqroc>=21) + (pnlplqroc>=27) + (pnlplqroc>=35);
	      variation->h1d(scan,1,2)->Fill(roc_fpix[scan][shl][dskbld][pnlplqroc]-mod_fpix[scan][shl][dskbld][pnlplq]);
	      variation->h1d(scan,2,2)->Fill(roc_fpix[scan][shl][dskbld][pnlplqroc]-rog_fpix[scan][shl][dskbld>11][rog]);
	    }
	    if (scan!=0&&roc_fpix[0][shl][dskbld][pnlplqroc]>-99&&roc_fpix[scan][shl][dskbld][pnlplqroc]>-99) diff->h1d(scan,2,2)->Fill(roc_fpix[scan][shl][dskbld][pnlplqroc]-roc_fpix[0][shl][dskbld][pnlplqroc]);
	    // mpv
	    mpv_roc_fpix[scan][shl][dskbld][pnlplqroc] = fit_mpvmax_(delay_roc->h1d(shl+4,dskbld,pnlplqroc,4))+(shl==3&&dskbld>=12&&dskbld<15)*cableing_corr;
	    if (mpv_roc_fpix[scan][shl][dskbld][pnlplqroc]>-99.0&&mpv_rog_fpix[scan][shl][dskbld>11][rog]>-99.0)
	      mpvmax->h1d(scan,2,2)->Fill(mpv_roc_fpix[scan][shl][dskbld][pnlplqroc]);
	  }
	}
      }
      // for (int scan = 0; scan<2; scan++) {
      //   for (int lay = 0; lay<8; lay++) {
      //     if (lay!=3) {
      //       TProfile *p = eta->h2d(scan,lay)->ProfileX();
      //       for (int i=1; i<=p->GetNbinsX(); i++) {
      //         eta->h1d(scan,lay)->SetBinContent(i, p->GetBinContent(i));
      //         eta->h1d(scan,lay)->SetBinError  (i, p->GetBinError(i));
      //       }
      //       delete p;
      //       eta->h1d(scan,lay)->GetYaxis()->SetRangeUser(14,22);
      //       p = phi->h2d(scan,lay)->ProfileX();
      //       for (int i=1; i<=p->GetNbinsX(); i++) {
      //         phi->h1d(scan,lay)->SetBinContent(i, p->GetBinContent(i));
      //         phi->h1d(scan,lay)->SetBinError  (i, p->GetBinError(i));
      //       }
      //       delete p;
      //       phi->h1d(scan,lay)->GetYaxis()->SetRangeUser(14,22);
      //     }
      //   }
      // }
      // Readout Group Plots - Efficiency
      for (int shl=0; shl<4; shl++) {
	for (int chn_dsk=0; chn_dsk<2; chn_dsk++) {
	  // BPix
	  delay_rog->h1d(shl,0,chn_dsk,0)->GetXaxis()->SetRangeUser(8,21);
	  delay_rog->h1d(shl,0,chn_dsk,0)->GetYaxis()->SetRangeUser(0.9,1.02);
	  TCanvas* can = custom_can_(delay_rog->h1d(shl,0,chn_dsk,0),"eff_vs_"+delay_rog->histoname()+p.shl[shl]+p.chn[chn_dsk], "2012 April Data", "Delay [ns]", "Efficiency", 1,1);
	  delay_rog->multidraw_with_legend_(shl, "0-7", chn_dsk, 0, "PE1", p.Sec, "1,2,4,3,1,2,4,3", p.Shl[shl]+" "+p.Chn[chn_dsk], 0.15,0.35, 0.15,0.5);
	  c.push_back(can);
	  // save_canvas_(can,"/home/common/Plotok_Viktornak/TimingScan12/");
	  // save_canvas_(can,"/afs/kfki.hu/home/jkarancs/public/Scans/TimingScan12/");
	  // FPix
	  delay_rog->h1d(shl,8+chn_dsk,0,0)->GetXaxis()->SetRangeUser(8,21);
	  delay_rog->h1d(shl,8+chn_dsk,0,0)->GetYaxis()->SetRangeUser(0.9,1.02);
	  can = custom_can_(delay_rog->h1d(shl,8+chn_dsk,0,0),"eff_vs_"+delay_rog->histoname()+p.shl[shl]+p.dsk[chn_dsk], "2012 April Data", "Delay [ns]", "Efficiency", 1,1);
	  delay_rog->multidraw_with_legend_(shl, 8+chn_dsk, "0-3", 0, "PE1", p.Rog, "1,2,4,3", p.Shl[shl]+" "+p.Dsk[chn_dsk], 0.15,0.35, 0.15,0.5);
	  c.push_back(can);
	  // save_canvas_(can,"/home/common/Plotok_Viktornak/TimingScan12/");
	  // save_canvas_(can,"/afs/kfki.hu/home/jkarancs/public/Scans/TimingScan12/");
	}
      }
    }
    // Scan comparison plots:
    for (int det=0; det<10; det++) {
      ah.delay11->h1d(det,0)->GetXaxis()->SetRangeUser(6,28);
      ah.delay11->h1d(det,0)->GetYaxis()->SetRangeUser(0.5,1.1);
      c.push_back(custom_can_(ah.delay11->h1d(det,0),"scans_eff"+p.det[det], "Timing Scans 2011-2012", "Delay [ns]", "Efficiency", 1,1));
      TLegend* leg = ah.delay11->multidraw_with_legend_(det, "0", "PE1", Scan, "1", p.Det[det], 0.2,0.4, 0.3,0.5);
      ah.delay12->h1d(det,0)->SetMarkerStyle(21);
      ah.delay3->h1d(det,0)->SetMarkerStyle(22);
      ah.delay4->h1d(det,0)->SetMarkerStyle(23);
      ah.delay12->h1d(det,0)->SetMarkerColor(4);
      ah.delay3->h1d(det,0)->SetMarkerColor(2);
      ah.delay4->h1d(det,0)->SetMarkerColor(3);
      ah.delay12->h1d(det,0)->Draw("SAMEPE1");
      ah.delay3->h1d(det,0)->Draw("SAMEPE1");
      ah.delay4->h1d(det,0)->Draw("SAMEPE1");
      leg->AddEntry(ah.delay12->h1d(det,0), Scan[1].c_str(), "P");
      leg->AddEntry(ah.delay3->h1d(det,0), Scan[2].c_str(), "P");
      leg->AddEntry(ah.delay4->h1d(det,0), Scan[3].c_str(), "P");
    }
    // Turnoff and Diff Plots
    for (int i=0; i<3; i++) {
      for (int j=0; j<3; j++) {
	// Turnoff - Eff
	turnoff->h1d(0,i,j)->GetXaxis()->SetRangeUser(10,24.9);
	turnoff->h1d(0,i,j)->GetYaxis()->SetRangeUser(0, (i==0) ? 12 : (i==1) ? 60 : (j==2) ? 350 : 500);
	c.push_back(custom_can_(turnoff->h1d(0,i,j),"turnoff"+pf1[i]+rog_pf[j], Pf1[i], "Turnoff Point [ns]", YTitle1[i],  0,0));
	turnoff->multidraw_with_legend("0-2", i, j, "STAT", Scan, "1,2,4", Rog[j], 0.2,0.4, 0.6,0.8);
	// Maximum MPV
	mpvmax->h1d(0,i,j)->GetXaxis()->SetRangeUser(10,24.9);
	mpvmax->h1d(0,i,j)->GetYaxis()->SetRangeUser(0, (i==0) ? 12 : (i==1) ? 60 : (j==2) ? 350 : 500);
	c.push_back(custom_can_(mpvmax->h1d(0,i,j),"mpvmax"+pf1[i]+rog_pf[j], Pf1[i], "Maximum MPV Point [ns]", YTitle1[i],  0,0));
	mpvmax->multidraw_with_legend("0-2", i, j, "STAT", Scan, "1,2,4", Rog[j], 0.2,0.4, 0.6,0.8);
	// Diff
	c.push_back(custom_can_(diff->h1d(1,i,j),"diff"+pf1[i]+rog_pf[j], "", "Timing Change (wrt 2011) [ns]", YTitle1[i],  0,0));
	diff->multidraw_with_legend("1,2", i, j, "", Scan, "1,2,4", Rog[j], 0.2,0.4, 0.6,0.8);
      }
    }
    // Variation Plots
    for (int i=0; i<3; i++) {
      for (int j=0; (i==0) ? j<4 : j<3; j++) {
	int var_ymax = (i==0) ? ((j==0) ? 40 : (j==1) ? 60 : (j==2) ? 100 : 150) : (i==1) ? (j==2 ? 700 : 1500) : (j<2 ? 1200 : 600);
	variation->h1d(0,i,j)->GetYaxis()->SetRangeUser(0, var_ymax);
	if (i==0) {
	  c.push_back(custom_can_(variation->h1d(0,i,j),"variation"+pf2[i]+rog2_pf[j], Pf2[i], "#Deltat [ns]", YTitle2[i],  0,0));
	  variation->multidraw_with_legend("0-2", i, j, "STAT", Scan, "1,2,4", Rog2[j], 0.2,0.4, 0.6,0.8);
	} else {
	  c.push_back(custom_can_(variation->h1d(0,i,j),"variation"+pf2[i]+rog_pf[j], Pf2[i], "#Deltat [ns]", YTitle2[i],  0,0));
	  variation->multidraw_with_legend("0-2", i, j, "STAT", Scan, "1,2,4", Rog[j], 0.2,0.4, 0.6,0.8);
	}
      }
    }
    // Eta/Phi Plots
    // for (int scan = 0; scan<2; scan++) {
    //   // BPix - Modules
    //   c.push_back(custom_can_(eta->h1d(scan,0),"eta_bpix"+scan_pf[scan], "", "Barrel Ring", "Module Turnoff [ns]",  0,0));
    //   eta->multidraw_with_legend(scan, "0-2", "PE1", Rog2, "2,4,3", Scan[scan], 0.4,0.6, 0.15,0.35);
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(1,"P1 PLQ1");
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(2,"P2 PLQ1");
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(3,"P1 PLQ2");
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(4,"P2 PLQ2");
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(5,"P1 PLQ3");
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(6,"P2 PLQ3");
    //   eta->h1d(scan,4)->GetXaxis()->SetBinLabel(7,"P1 PLQ4");
    //   // Fix - Plaquettes
    //   c.push_back(custom_can_(eta->h1d(scan,4),"eta_fpix"+scan_pf[scan], "", "", "Module Turnoff [ns]",  0,0));
    //   eta->multidraw_with_legend(scan, "4-7", "PE1", Rog2, "1,2,4,3", Scan[scan], 0.4,0.6, 0.15,0.35);
    //   // BPix - MOD1 Ladders
    //   c.push_back(custom_can_(phi->h1d(scan,0),"phi_bpix"+scan_pf[scan], "", "Ladder", "Module Turnoff [ns]",  0,0));
    //   phi->multidraw_with_legend(scan, "0-2", "PE1", Rog2, "2,4,3", Scan[scan], 0.4,0.6, 0.15,0.35);
    //   // FPix - Blades
    //   c.push_back(custom_can_(phi->h1d(scan,4),"phi_fpix"+scan_pf[scan], "", "Blade", "Module Turnoff [ns]",  0,0));
    //   phi->multidraw_with_legend(scan, "4-7", "PE1", Rog2, "1,2,4,3", Scan[scan], 0.4,0.6, 0.15,0.35);
    // }
  }
  
  void hv_scan_can_(std::vector<TCanvas*>& c, AllHistos& ah, Variables& v, PostFixes& p) {
    std::stringstream hv_l1;
    std::stringstream hv_l3;
    std::stringstream hv_d1;
    std::stringstream hv_l1full;
    std::stringstream hv_l2full;
    std::stringstream hv_l3full;
    std::stringstream hv_d1full;
    std::stringstream hv_d2full;
    hv_l1<<"0-"<<(p.hv_l1.size()-1);
    hv_l3<<"0-"<<(p.hv_l3.size()-1);
    hv_d1<<"0-"<<(p.hv_d1.size()-1);
    hv_l1full<<"0-"<<(p.hv_l1full.size()-1);
    hv_l2full<<"0-"<<(p.hv_l2full.size()-1);
    hv_l3full<<"0-"<<(p.hv_l3full.size()-1);
    hv_d1full<<"0-"<<(p.hv_d1full.size()-1);
    hv_d2full<<"0-"<<(p.hv_d2full.size()-1);
    std::string col = "1,2,4,3,92,1,2,4,3,92,1,2,4,3,92,1,2,4,3,92,1,2,4,3,92";
    for (int i=0; i<2; i++) {
      std::string pf = (i==0) ? "_eff" : "_mpv";
      std::string title = (i==0) ? "Efficiency" : "MPV of Normalized Cluster Charge [ke]";
      std::string opt = (i==0) ? "PE1" : "P";
      // 1 HV Group Scans
      c.push_back(custom_can_(ah.hv_l1->h1d(0,i*2),"hv_l1"+pf, "Layer 1 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_l1->multidraw_with_legend_(hv_l1.str(), i*2, opt, p.Hv_l1, col, "Bpix_BmO_SEC6_LYR1-2_HV1", 0.4,0.6, 0.12,0.12+(p.hv_l1.size()+1)*0.04);
      c.push_back(custom_can_(ah.hv_l3->h1d(0,i*2),"hv_l3"+pf, "Layer 3 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_l3->multidraw_with_legend_(hv_l3.str(), i*2, opt, p.Hv_l3, col, "Bpix_BpO_SEC2_LYR3_HV1", 0.4,0.6, 0.12,0.12+(p.hv_l3.size()+1)*0.04);
      ah.hv_d1->h1d(0,0)->GetXaxis()->SetRangeUser(0,150);
      c.push_back(custom_can_(ah.hv_d1->h1d(0,i*2),"hv_d1"+pf, "Disk -1 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_d1->multidraw_with_legend_(hv_d1.str(), i*2, opt, p.Hv_d1, col, "FPix_BmO_D1_ROG1_HV1", 0.4,0.6, 0.12,0.12+(p.hv_d1.size()+1)*0.04);
      // Full layer Scans
      c.push_back(custom_can_(ah.hv_l1full->h1d(0,0,i*2),"hv_l1full"+pf, "Full Layer 1 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_l1full->multidraw_with_legend_(hv_l1full.str(), 0, i*2, opt, p.Hv_l1full, col, "Layer 1", 0.4,0.6, 0.12,0.12+(p.hv_l1full.size()+1)*0.04);
      c.push_back(custom_can_(ah.hv_l2full->h1d(0,0,i*2),"hv_l2full"+pf, "Full Layer 2 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_l2full->multidraw_with_legend_(hv_l2full.str(), 0, i*2, opt, p.Hv_l2full, col, "Layer 2", 0.4,0.6, 0.12,0.12+(p.hv_l2full.size()+1)*0.04);
      c.push_back(custom_can_(ah.hv_l3full->h1d(0,0,i*2),"hv_l3full"+pf, "Full Layer 3 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_l3full->multidraw_with_legend_(hv_l3full.str(), 0, i*2, opt, p.Hv_l3full, col, "Layer 3", 0.4,0.6, 0.12,0.12+(p.hv_l3full.size()+1)*0.04);
      c.push_back(custom_can_(ah.hv_d1full->h1d(0,0,i*2),"hv_d1full"+pf, "Full Disk 1 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_d1full->multidraw_with_legend_(hv_d1full.str(), 0, i*2, opt, p.Hv_d1full, col, "Disk 1", 0.4,0.6, 0.12,0.12+(p.hv_d1full.size()+1)*0.04);
      c.push_back(custom_can_(ah.hv_d2full->h1d(0,0,i*2),"hv_d2full"+pf, "Full Disk 2 HV Bias Scans", "Bias Voltage [V]", title, 1,1));
      ah.hv_d2full->multidraw_with_legend_(hv_d2full.str(), 0, i*2, opt, p.Hv_d2full, col, "Disk 2", 0.4,0.6, 0.12,0.12+(p.hv_d2full.size()+1)*0.04);
    }
    // V_Turnon Plot
    ah.calc_hv_scans(v,p);
    c.push_back(custom_can_(ah.vturnon_totlumi->h1d(0),"vturnon_totlumi", "All HV Bias Scans", "Total Int. Luminosity - 2010-2012 [pb^{-1}]", "V_{Turnon}", 1,1));
    ah.vturnon_totlumi->multidraw_with_legend_("0-4", "P9E1", p.Det5, "2,4,3,1,6", "", 0.7,0.9, 0.7,0.9);
    // FPix HV Groups - Turnon
    for (size_t d1scan=0; d1scan<p.hv_d1full.size(); d1scan++) {
      ah.vturnon_fpix->h1d(d1scan, 0)->GetYaxis()->SetRangeUser(0,40);
      c.push_back(custom_can_(ah.vturnon_fpix->h1d(d1scan, 0),"vturnon_fpix_hv"+p.hv_d1full[d1scan], "", "HV Groups", "V_{Turnon}", 0,1));
      ah.vturnon_fpix->multidraw_with_legend_(d1scan, "0-3", "P", p.Fpix_grp, "1,2,4,3", p.Hv_d1full[d1scan], 0.5,0.7, 0.12,0.32);
    }
    for (int lay=0; lay<3; lay++) {
      std::string scanrange = (lay==0) ? hv_l1full.str() : (lay==1) ? hv_l2full.str() : hv_l3full.str();
      std::vector<std::string> legendvec = (lay==0) ? p.Hv_l1full : (lay==1) ? p.Hv_l2full : p.Hv_l3full;
      // Sector
      ah.vturnon_bpix->h1d(lay, 0, 0)->GetYaxis()->SetRangeUser(0,70);
      c.push_back(custom_can_(ah.vturnon_bpix->h1d(lay, 0, 0),"vturnon_sec"+p.det[3+lay], "", "Sector", "V_{Turnon}", 0,1));
      ah.vturnon_bpix->multidraw_with_legend_(lay, scanrange, 0, "P", legendvec, col, p.Det[3+lay], 0.5,0.7, 0.12,0.32);
      // Ring
      ah.vturnon_bpix->h1d(lay, 0, 1)->GetYaxis()->SetRangeUser(0,70);
      c.push_back(custom_can_(ah.vturnon_bpix->h1d(lay, 0, 1),"vturnon_ring"+p.det[3+lay], "", "Ring", "V_{Turnon}", 0,1));
      ah.vturnon_bpix->multidraw_with_legend_(lay, scanrange, 1, "P", legendvec, col, p.Det[3+lay], 0.5,0.7, 0.12,0.32);
    }
  }


public:
  
  void load_from_file(AllHistos& ah, std::string inaddress) {
    TFile f_in(inaddress.c_str());
    if (create_noteff_==1) {
      // ah.totlumi_ratio->load(f_in);
      ah.ncd->load(f_in);
      // ah.ncd_corr->load(f_in);
      // ah.pcd->load(f_in);
      // ah.pcd_corr->load(f_in);
      // ah.pcd_heta->load(f_in);
      // ah.pcd_corr_heta->load(f_in);
      // ah.clu_size->load(f_in);
      // ah.clu_sizeX->load(f_in);
      // ah.clu_sizeY->load(f_in);
      // ah.clu_size_heta->load(f_in);
      // ah.clu_sizeX_heta->load(f_in);
      // ah.clu_sizeY_heta->load(f_in);
      // ah.alpha->load(f_in);
      // ah.beta->load(f_in);
#ifdef COMPLETE
      ah.nclu_corr->load(f_in);
      ah.npix_corr->load(f_in);
      ah.instlumi_corr->load(f_in);
      ah.bx_ls->load(f_in);
      ah.ls_fill->load(f_in);
      if (create_eff_!=1) {
	ah.instlumi->load(f_in);
	ah.time->load(f_in);
      }
#endif
    }
    if (create_eff_==1) {
      ah.det->load(f_in);
      ah.mod->load(f_in);
      ah.roc->load(f_in);
      ah.time->load(f_in);
      ah.run->load(f_in);
      ah.totlumi->load(f_in);
      ah.nvtx->load(f_in);
      ah.instlumi->load(f_in);
      ah.instlumi_raw->load(f_in);
      ah.bx->load(f_in);
      ah.il_l1rate->load(f_in);
      //ah.time2->load(f_in);
#ifdef COMPLETE
      ah.occup->load(f_in);
      ah.occup_mod->load(f_in);
      ah.occup_roc->load(f_in);
#endif
#if FEDERRPLOTS == 1
      ah.federr_evt->load(f_in);
      ah.federr->load(f_in);
      ah.time_federr->load(f_in); // takes a long time
#endif
#if BADROC != 0
      ah.roceff_dist->load(f_in);
      ah.nbadroc->load(f_in);
      ah.badroc->load(f_in);
#endif
      ah.pt->load(f_in);
      ah.nstrip->load(f_in);
      ah.dz->load(f_in);
      ah.d0->load(f_in);
      ah.dz_d0->load(f_in);
      ah.normchi2->load(f_in);
      ah.fid_lxly->load(f_in);
      ah.fid_lx->load(f_in);
      ah.fid_ly->load(f_in);
      // Complete
      // ah.vtxd0->load(f_in);
      // ah.vtxz->load(f_in);
    }
    if (create_scans_==1) {
      ah.delay11->load(f_in);
      ah.delay12->load(f_in);
      ah.delay3->load(f_in);
      //ah.delay4->load(f_in);
      ah.delay11_rog->load(f_in);
      ah.delay12_rog->load(f_in);
      ah.delay3_rog->load(f_in);
      //ah.delay4_rog->load(f_in);
      
      ah.delay11_mod->load(f_in);
      //ah.delay11_roc->load(f_in);
      ah.delay12_mod->load(f_in);
      //ah.delay12_roc->load(f_in);
      ah.delay3_mod->load(f_in);
      //ah.delay3_roc->load(f_in);
      //ah.delay4_mod->load(f_in);
      //ah.delay4_roc->load(f_in);

      // ah.hv_l1->load(f_in);
      // ah.hv_l3->load(f_in);
      // ah.hv_d1->load(f_in);
      // ah.hv_l1full->load(f_in);
      // ah.hv_l2full->load(f_in);
      // ah.hv_l3full->load(f_in);
      // ah.hv_d1full->load(f_in);
      // ah.hv_d2full->load(f_in);
    }
    f_in.Close();
//     TFile f_hv1_11("/afs/kfki.hu/home/jkarancs/public/ROOT_output/v3029/TEST/HV1-11_MB_RECO_INC_SPL0_nstrip0.root");
//     for (int i=0; i<11; i++) ah.hv->partial_load(f_hv1_11,i);
//     f_hv1_11.Close();

    // TFile f_add("/afs/kfki.hu/home/jkarancs/public/Approval/alldata/root_final/finalcuts_allscans_neweff_v1816mod.root");
    // ah.hv.partial_load(f_add,2,1); // refill was needed
/*     TFile f_add("/afs/kfki.hu/home/jkarancs/Reference_HV_scans/reference_HV1-8_v2525.root"); */
/*     for (int i=0; i<8; i++) ah.hv.partial_load(f_add,i,1); */
/*     f_add.Close(); */
//     TFile f_add("/afs/kfki.hu/home/jkarancs/public/ROOT_output/v2625/scan_mpv_100entryfit_HV1-8_v2625.root");
//     for (int i=0; i<8; i++) if(i!=4) ah.hv->partial_load(f_add,i,0);
//     f_add.Close();
/*     TFile f_add2("/afs/kfki.hu/home/jkarancs/public/ROOT_output/v2625/scan_mpv_HV8_v2625.root"); */
/*     ah.hv.partial_load(f_add2,7,0); */
/*     f_add2.Close(); */
  }

  void run(AllHistos& ah, std::string outaddress, Variables& v, PostFixes& p) {
    // TFile f_add("/afs/kfki.hu/home/jkarancs/public/ROOT_output/v2525/federr_timeout_02_2010Band2011Aruns_jul6JSON_neweff_v2525.root");
    // TH1D* federr_eff = (TH1D*)f_add.Get("federr_eff");
    // federr_eff->SetDirectory(NULL);
    // TH1D* federr_dist = (TH1D*)f_add.Get("federr_dist");
    // federr_dist->SetDirectory(NULL);
    // federr_eff->Multiply(federr_dist);
    // federr_dist->Add(federr_eff,-1.0);
    // f_add.Close();
    // ah.federr->h1d(0)->Multiply(ah.federr->h1d(1));
    // ah.federr->h1d(1)->Add(ah.federr->h1d(0),-1.0);
    // ah.federr->h1d(0)->Add(federr_eff);
    // ah.federr->h1d(1)->Add(federr_dist);
    // ah.federr.calc();
    // 
    // TFile f_add("/afs/kfki.hu/home/jkarancs/public/Approval/alldata/root_final/finalcuts_allscans_neweff_v1816mod.root"); // module only
    // TH1D* hv_11mar_l1_eff = (TH1D*)f_add.Get("hv_11mar_l1_eff");
    // TH1D* hv_11mar_l3_eff = (TH1D*)f_add.Get("hv_11mar_l3_eff");
    // hv_11mar_l1_eff->SetDirectory(NULL);
    // hv_11mar_l3_eff->SetDirectory(NULL);
    // f_add.Close();
    // hv_11mar_l1 = new TH1D("hv_11mar_l1","hv_11mar_l1",14, 15.0, 155.0);
    // hv_11mar_l3 = new TH1D("hv_11mar_l3","hv_11mar_l3",14, 15.0, 155.0);
    // for (int i=1; i<=13; i++) {
    //   hv_11mar_l1 -> SetBinContent(i+1,hv_11mar_l1_eff->GetBinContent(i));
    //   hv_11mar_l1 -> SetBinError(i+1,hv_11mar_l1_eff->GetBinError(i));
    //   hv_11mar_l3 -> SetBinContent(i+1,hv_11mar_l3_eff->GetBinContent(i));
    //   hv_11mar_l3 -> SetBinError(i+1,hv_11mar_l3_eff->GetBinError(i));
    // }
    // 
    // TFile f_add2("/afs/kfki.hu/home/jkarancs/public/ROOT_output/v2525/TEST/HV_rog_170000_neweff_v2525.root"); // full layer
    // TH1D* hv_11jul_l1_eff = (TH1D*)f_add2.Get("hv_11jul_l1_eff");
    // hv_11jul_l1_eff->SetDirectory(NULL);
    // TH1D* rog_11jul_BmO_Sec6_eff = (TH1D*)f_add2.Get("rog_11jul_BmO_Sec6_eff");
    // rog_11jul_BmO_Sec6_eff->SetDirectory(NULL);
    // f_add2.Close();
    // hv_11jul_l1 = new TH1D("hv_11jul_l1","hv_11jul_l1",16, -5.0, 155.0);
    // for (int i=1; i<=13; i++) {
    //   hv_11jul_l1 -> SetBinContent(i+2,hv_11jul_l1_eff->GetBinContent(i));
    //   hv_11jul_l1 -> SetBinError(i+2,hv_11jul_l1_eff->GetBinError(i));
    // }
    // rog_11jul_BmO_Sec6 = new TH1D("rog_11jul_BmO_Sec6","rog_11jul_BmO_Sec6",16, -5.0, 155.0);
    // for (int i=1; i<=14; i++) {
    //   rog_11jul_BmO_Sec6 -> SetBinContent(i+2,rog_11jul_BmO_Sec6_eff->GetBinContent(i));
    //   rog_11jul_BmO_Sec6 -> SetBinError(i+2,rog_11jul_BmO_Sec6_eff->GetBinError(i));
    // }

    year_ = "2011";
    date_ = "2011 March";

    color_schemes_();
/* hh_dist->DrawNormalized() */
/* TFile f("/afs/kfki.hu/home/jkarancs/public/ROOT_output/v2525/TEST/MB_test_neweff_v2525.root") */
/* TH1D* h = (TH1D*)f.Get("hh_dist") */
/* h->DrawNormalized("same") */
/* f.Close(); */


    //TStyle* myStyle = new TStyle("myStyle","histo Styling");
    gStyle->SetTitleFont(42,"xyz");
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetCanvasColor(0);
    gStyle->SetErrorX(0);
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetFrameFillStyle(0);
    gStyle->SetFrameLineWidth(2);
    //gStyle->SetLineWidth(2);
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
    //gROOT->SetStyle("myStyle");

    // TStyle* defStyle = new TStyle("defStyle","old styling");
    // defStyle->SetPalette(1);
    // defStyle->SetOptStat(0);
    // defStyle->SetOptTitle(0);
    // defStyle->SetFrameFillColor(0);
    // defStyle->SetFrameFillStyle(0);
    // defStyle->SetFrameBorderMode(0);
    // defStyle->SetCanvasBorderMode(0);
    // defStyle->SetCanvasBorderSize(0);
    // defStyle->SetPadBorderMode(0);
    // defStyle->SetPadColor(0);
    // defStyle->SetErrorX(0);
    // gROOT->SetStyle("defStyle");

    std::vector<TCanvas*> Noteff;

    // Not Efficiency
    if (create_noteff_==1) {
      ncd_can_(Noteff,ah.ncd);
      ncd_can_(Noteff,ah.ncd_corr);
      pcd_can_(Noteff,ah.pcd);
      pcd_can_(Noteff,ah.pcd_heta);
      pcd_can_(Noteff,ah.pcd_corr);
      pcd_can_(Noteff,ah.pcd_corr_heta);
      clu_size_can_(Noteff,ah.clu_size);
      clu_sizeX_can_(Noteff,ah.clu_sizeX);
      clu_sizeY_can_(Noteff,ah.clu_sizeY);
      clu_size_can_(Noteff,ah.clu_size_heta);
      clu_sizeX_can_(Noteff,ah.clu_sizeX_heta);
      clu_sizeY_can_(Noteff,ah.clu_sizeY_heta);
      alpha_can_(Noteff,ah.alpha);
      beta_can_(Noteff,ah.beta);
#ifdef COMPLETE
      nclu_plots_can_(Noteff,ah, v, p);
#endif
    }

    std::vector<TCanvas*> Eff;
    // Efficiency
    if (create_eff_==1) {
      fill_multican_(Eff, ah, v, p);
      run_can_(Eff,ah.run);
      totlumi_can_(Eff,ah.totlumi);
      pt_can_(Eff,ah.pt);
      nstrip_can_(Eff,ah.nstrip);
      dz_can_(Eff,ah.dz);
      d0_can_(Eff,ah.d0);
      fid2_can_(Eff,ah.fid_lxly,ah.fid_lx,ah.fid_ly);
      // fid_can_(Eff,ah.fid_lxly,ah.fid_lx,ah.fid_ly);
#if BADROC != 0
      roceff_dist_can_(Eff, ah.roceff_dist, v, p);
      nbadroc_can_(Eff, ah.nbadroc, v, p);
      badroc_can_(Eff, ah.badroc, v, p);
#endif
#ifdef COMPLETE
      occup_can_(Eff, ah, v, p);
#endif
      //time2_can_(Eff, ah.time2, v, p);
      // lay_can_(Eff,ah.det);
      // mod_can_(Eff,ah.mod);
      // roc_can_(Eff,ah.roc);
      // time_can_(Eff,ah.time);
      // bx_can_(Eff, ah.bx);
      // il_can_(Eff, ah.instlumi, ah.il_l1rate);
    }

    std::vector<TCanvas*> Scans;
    if (create_scans_==1) {
//       hv_can_(Scans, ah, ah.hv, v);
      //hv_scan_can_(Scans, ah, v, p);
      timing_can_(Scans, ah, v, p);
    }
    TFile f_out(outaddress.c_str(),"recreate");
    for (size_t i=0; i<Noteff.size(); i++) Noteff[i]->Write();
    for (size_t i=0; i<Eff.size(); i++) Eff[i]->Write();
    for (size_t i=0; i<Scans.size(); i++) Scans[i]->Write();
    f_out.Close();
    std::cout<<"Canvas File: "<<outaddress<<" ready."<<std::endl;
  }
  
};
