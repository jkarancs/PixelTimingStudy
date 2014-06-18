#include "TPaveStats.h"
#include "TStyle.h"
#include "TGraphAsymmErrors.h"
#include "TLatex.h"

class Histograms {
  
 public:
  
  Histograms() {}
  Histograms(std::string histoname, int npostfix, int dimensions,
             int eff = 0, int err = 0, int clusize = 0, int def = 0, int cumul = 0) 
    : histoname_(histoname), npostfix_(npostfix), dimensions_(dimensions), eff_(eff), err_(err), clusize_(clusize), def_(def), cumul_(cumul)
  {
    build_();
  }
  ~Histograms() {}

  void build_() {
    marker_.push_back(20); // full circle
    marker_.push_back(21); // full square
    marker_.push_back(22); // full triangle up
    marker_.push_back(23); // full triangle down
    marker_.push_back(29); // full star
    marker_.push_back(20); // full circle
    marker_.push_back(21); // full square
    marker_.push_back(22); // full triangle up
    marker_.push_back(23); // full triangle down
    //marker_.push_back(29); // full star
    marker_.push_back(20); // full circle
    marker_.push_back(21); // full square
    marker_.push_back(22); // full triangle up
    marker_.push_back(23); // full triangle down
    //marker_.push_back(29); // full star
    marker_.push_back(24); // open circle
    marker_.push_back(25); // open square
    marker_.push_back(26); // open triangle up
    marker_.push_back(28); // open cross
    //marker_.push_back(30); // open star
    debug_=0;
  }

 private:
  
  std::string histoname_;
  int npostfix_;
  int dimensions_;
  int eff_;
  int err_;
  int clusize_;
  int def_;
  int cumul_;
  int debug_;

  std::vector<Int_t> marker_;

  std::vector<TH1D*> h1d_1p_;
  std::vector<TH2D*> h2d_1p_;
  std::vector<TH3D*> h3d_1p_;
  std::vector<std::vector<TH1D*> > h1d_2p_;
  std::vector<std::vector<TH2D*> > h2d_2p_;
  std::vector<std::vector<TH3D*> > h3d_2p_;
  std::vector<std::vector<std::vector<TH1D*> > > h1d_3p_;
  std::vector<std::vector<std::vector<TH2D*> > > h2d_3p_;
  std::vector<std::vector<std::vector<TH3D*> > > h3d_3p_;
  std::vector<std::vector<std::vector<std::vector<TH1D*> > > > h1d_4p_;
  std::vector<std::vector<std::vector<std::vector<TH2D*> > > > h2d_4p_;
  std::vector<std::vector<std::vector<std::vector<TH3D*> > > > h3d_4p_;

  void calc_cumulative_() {
    if (npostfix_==1) {
      if (dimensions_==1) {
	for (int j=2; j<=3; j++) {
	  h1d_1p_[j]->Add(h1d_1p_[j-2]); double x=0.0;
	  if (cumul_==1) for (int i=0; i<=h1d_1p_[j]->GetNbinsX(); i++) { 
	    x+=h1d_1p_[j]->GetBinContent(i); h1d_1p_[j]->SetBinContent(i,x); }
	  else if (cumul_==-1) for (int i=h1d_1p_[j]->GetNbinsX()+1; i>0; i--) { 
	    x+=h1d_1p_[j]->GetBinContent(i); h1d_1p_[j]->SetBinContent(i,x);  }
	}
      } else if (dimensions_==2) {
	for (int k=2; k<=3; k++) {
	  h2d_1p_[k]->Add(h2d_1p_[k-2]);
	  for (int j=0; j<=h2d_1p_[k]->GetNbinsY(); j++) {
	    double x=0.0;
	    if (cumul_>0) for (int i=0; i<=h2d_1p_[k]->GetNbinsX(); i++) { 
	      x+=h2d_1p_[k]->GetBinContent(i,j); h2d_1p_[k]->SetBinContent(i,j,x); }
	    else if (cumul_<0) for (int i=h2d_1p_[k]->GetNbinsX()+1; i>0; i--) { 
	      x+=h2d_1p_[k]->GetBinContent(i,j); h2d_1p_[k]->SetBinContent(i,j,x); }
	  }
	  for (int i=0; i<=h2d_1p_[k]->GetNbinsX(); i++) {
	    double y=0.0;
	    if (cumul_==1||cumul_==-2) for (int j=0; j<=h2d_1p_[k]->GetNbinsY(); j++) { 
	      y+=h2d_1p_[k]->GetBinContent(i,j); h2d_1p_[k]->SetBinContent(i,j,y); }
	    else if (cumul_==2||cumul_==-1) for (int j=h2d_1p_[k]->GetNbinsY()+1; j>0; j--) { 
	      y+=h2d_1p_[k]->GetBinContent(i,j); h2d_1p_[k]->SetBinContent(i,j,y); }
	  }
	}
      }
    } else if (npostfix_==2) {
      if (dimensions_==1) {
	for (size_t k=0; k<h1d_2p_.size(); k++) for (int j=2; j<=3; j++) {
	  h1d_2p_[k][j]->Add(h1d_2p_[k][j-2]); double x=0.0;
	  if (cumul_==1) for (int i=0; i<=h1d_2p_[k][j]->GetNbinsX(); i++) { 
	    x+=h1d_2p_[k][j]->GetBinContent(i); h1d_2p_[k][j]->SetBinContent(i,x); }
	  else if (cumul_==-1) for (int i=h1d_2p_[k][j]->GetNbinsX()+1; i>0; i--) { 
	    x+=h1d_2p_[k][j]->GetBinContent(i); h1d_2p_[k][j]->SetBinContent(i,x); }
	}
      } else if (dimensions_==2) {
	for (size_t l=0; l<h2d_2p_.size(); l++) for (int k=2; k<=3; k++) {
	  h2d_2p_[l][k]->Add(h2d_2p_[l][k-2]);
	  for (int j=0; j<=h2d_2p_[l][k]->GetNbinsY(); j++) {
	    double x=0.0;
	    if (cumul_>0) for (int i=0; i<=h2d_2p_[l][k]->GetNbinsX(); i++) { 
	      x+=h2d_2p_[l][k]->GetBinContent(i,j); h2d_2p_[l][k]->SetBinContent(i,j,x); }
	    else if (cumul_<0) for (int i=h2d_2p_[l][k]->GetNbinsX()+1; i>0; i--) { 
	      x+=h2d_2p_[l][k]->GetBinContent(i,j); h2d_2p_[l][k]->SetBinContent(i,j,x); }
	  }
	  for (int i=0; i<=h2d_2p_[l][k]->GetNbinsX(); i++) {
	    double y=0.0;
	    if (cumul_==1||cumul_==-2) for (int j=0; j<=h2d_2p_[l][k]->GetNbinsY(); j++) { 
	      y+=h2d_2p_[l][k]->GetBinContent(i,j); h2d_2p_[l][k]->SetBinContent(i,j,y); }
	    else if (cumul_==2||cumul_==-1) for (int j=h2d_2p_[l][k]->GetNbinsY()+1; j>0; j--) { 
	      y+=h2d_2p_[l][k]->GetBinContent(i,j); h2d_2p_[l][k]->SetBinContent(i,j,y); }
	  }
	}
      }
    } else if (npostfix_==3) {
      if (dimensions_==1) {
	for (size_t k=0; k<h1d_3p_.size(); k++)  for (size_t l=0; l<h1d_3p_[k].size(); l++)  for (int j=2; j<=3; j++) {
	  h1d_3p_[k][l][j]->Add(h1d_3p_[k][l][j-2]); double x=0.0;
	  if (cumul_==1)  for (int i=0; i<=h1d_3p_[k][l][j]->GetNbinsX(); i++) { 
	    x+=h1d_3p_[k][l][j]->GetBinContent(i); h1d_3p_[k][l][j]->SetBinContent(i,x); }
	  else if (cumul_==-1) for (int i=h1d_3p_[k][l][j]->GetNbinsX()+1; i>0; i--) { 
	    x+=h1d_3p_[k][l][j]->GetBinContent(i); h1d_3p_[k][l][j]->SetBinContent(i,x); }
	}
      } else if (dimensions_==2) {
	for (size_t l=0; l<h2d_3p_.size(); l++) for (size_t m=0; m<h2d_3p_[l].size(); m++) for (int k=2; k<=3; k++) {
	  h2d_3p_[l][m][k]->Add(h2d_3p_[l][m][k-2]);
	  for (int j=0; j<=h2d_3p_[l][m][k]->GetNbinsY(); j++) {
	    double x=0.0;
	    if (cumul_>0) for (int i=0; i<=h2d_3p_[l][m][k]->GetNbinsX(); i++) { 
	      x+=h2d_3p_[l][m][k]->GetBinContent(i,j); h2d_3p_[l][m][k]->SetBinContent(i,j,x); }
	    else if (cumul_<0) for (int i=h2d_3p_[l][m][k]->GetNbinsX()+1; i>0; i--) { 
	      x+=h2d_3p_[l][m][k]->GetBinContent(i,j); h2d_3p_[l][m][k]->SetBinContent(i,j,x); }
	  }
	  for (int i=0; i<=h2d_3p_[l][m][k]->GetNbinsX(); i++) {
	    double y=0.0;
	    if (cumul_==1||cumul_==-2) for (int j=0; j<=h2d_3p_[l][m][k]->GetNbinsY(); j++) { 
	      y+=h2d_3p_[l][m][k]->GetBinContent(i,j); h2d_3p_[l][m][k]->SetBinContent(i,j,y); }
	    else if (cumul_==2||cumul_==-1) for (int j=h2d_3p_[l][m][k]->GetNbinsY()+1; j>0; j--) {
	      y+=h2d_3p_[l][m][k]->GetBinContent(i,j); h2d_3p_[l][m][k]->SetBinContent(i,j,y); }
	  }
	}
      }
    }
  }
  
  void calc_eff_() {
    for (int j=0; j<=(cumul_!=0); j++) {
      if (npostfix_==1) {
	if (dimensions_==1) { h1d_1p_[1+2*j]->Add(h1d_1p_[0+2*j]); h1d_1p_[0+2*j]->Divide(h1d_1p_[1+2*j]); }
	else if (dimensions_==2) { h2d_1p_[1+2*j]->Add(h2d_1p_[0+2*j]); h2d_1p_[0+2*j]->Divide(h2d_1p_[1+2*j]); }
	else if (dimensions_==3) { h3d_1p_[1+2*j]->Add(h3d_1p_[0+2*j]); h3d_1p_[0+2*j]->Divide(h3d_1p_[1+2*j]); }
      } else if (npostfix_==2) {
	if (dimensions_==1) for (size_t k=0; k<h1d_2p_.size(); k++) { 
	  h1d_2p_[k][1+2*j]->Add(h1d_2p_[k][0+2*j]); h1d_2p_[k][0+2*j]->Divide(h1d_2p_[k][1+2*j]); }
	else if (dimensions_==2) for (size_t k=0; k<h2d_2p_.size(); k++) { 
	  h2d_2p_[k][1+2*j]->Add(h2d_2p_[k][0+2*j]); h2d_2p_[k][0+2*j]->Divide(h2d_2p_[k][1+2*j]); }
	else if (dimensions_==3) for (size_t k=0; k<h3d_2p_.size(); k++) { 
	  h3d_2p_[k][1+2*j]->Add(h3d_2p_[k][0+2*j]); h3d_2p_[k][0+2*j]->Divide(h3d_2p_[k][1+2*j]); }
      } else if (npostfix_==3) {
	if (dimensions_==1) for (size_t k=0; k<h1d_3p_.size(); k++) for (size_t l=0; l<h1d_3p_[k].size(); l++) {
	  h1d_3p_[k][l][1+2*j]->Add(h1d_3p_[k][l][0+2*j]); h1d_3p_[k][l][0+2*j]->Divide(h1d_3p_[k][l][1+2*j]); }
	else if (dimensions_==2) for (size_t k=0; k<h2d_3p_.size(); k++) for (size_t l=0; l<h2d_3p_[k].size(); l++) {
	  h2d_3p_[k][l][1+2*j]->Add(h2d_3p_[k][l][0+2*j]); h2d_3p_[k][l][0+2*j]->Divide(h2d_3p_[k][l][1+2*j]); }
	else if (dimensions_==3) for (size_t k=0; k<h3d_3p_.size(); k++) for (size_t l=0; l<h3d_3p_[k].size(); l++) {
	  h3d_3p_[k][l][1+2*j]->Add(h3d_3p_[k][l][0+2*j]); h3d_3p_[k][l][0+2*j]->Divide(h3d_3p_[k][l][1+2*j]); }
      } else if (npostfix_==4) {
	if (dimensions_==1) for (size_t k=0; k<h1d_4p_.size(); k++) for (size_t l=0; l<h1d_4p_[k].size(); l++) for (size_t m=0; m<h1d_4p_[k][l].size(); m++) {
	  h1d_4p_[k][l][m][1+2*j]->Add(h1d_4p_[k][l][m][0+2*j]); h1d_4p_[k][l][m][0+2*j]->Divide(h1d_4p_[k][l][m][1+2*j]); }
	else if (dimensions_==2) for (size_t k=0; k<h2d_4p_.size(); k++) for (size_t l=0; l<h2d_4p_[k].size(); l++) for (size_t m=0; m<h2d_4p_[k][l].size(); m++) {
	  h2d_4p_[k][l][m][1+2*j]->Add(h2d_4p_[k][l][m][0+2*j]); h2d_4p_[k][l][m][0+2*j]->Divide(h2d_4p_[k][l][m][1+2*j]); }
	else if (dimensions_==3) for (size_t k=0; k<h3d_4p_.size(); k++) for (size_t l=0; l<h3d_4p_[k].size(); l++) for (size_t m=0; m<h3d_4p_[k][l].size(); m++) {
	  h3d_4p_[k][l][m][1+2*j]->Add(h3d_4p_[k][l][m][0+2*j]); h3d_4p_[k][l][m][0+2*j]->Divide(h3d_4p_[k][l][m][1+2*j]); }
      }
    }
  }

  void calc_clusize_() {
    if (clusize_==1&&dimensions_==1) {
      if (npostfix_==2) { for (size_t i=0; i<h1d_2p_.size(); i++) h1d_2p_[i][2]->Divide(h1d_2p_[i][3]); }
      else if (npostfix_==4) { 
	for (size_t i=0; i<h1d_4p_.size(); i++) for (size_t j=0; j<h1d_4p_[i].size(); j++) 
	  for (size_t k=0; k<h1d_4p_[i][j].size(); k++) h1d_4p_[i][j][k][2]->Divide(h1d_4p_[i][j][k][3]); }
    }
  }

  void error_(TH1D* eff, TH1D* dist) {
    double z = 1; // 1 Sigma confidence
    if (err_==1) { // Normal Approximation interval
      for (int i=1; i<=eff->GetNbinsX(); i++) if (dist->GetBinContent(i)!=0) {
	double p = eff->GetBinContent(i);
	double n = dist->GetBinContent(i);
	double err = z*sqrt(p*(1.0-p)/n);
	eff->SetBinError(i,err);
      }
    } else if (err_==2) { // Wilson Score Interval
      for (int i=1; i<=eff->GetNbinsX(); i++) if (dist->GetBinContent(i)!=0) {
	double p = eff->GetBinContent(i);
	double n = dist->GetBinContent(i);
	double cen = (p+(z*z/(2*n))) / (1.0 + (z*z/n));
	double halfwidth = z*sqrt( p*(1.0-p)/n + (z*z/(4*n*n)) ) / (1.0 + (z*z/n));
	double err = halfwidth + fabs(cen-p); // Assymmetric error -> Choose larger for a conservative error estimate
	eff->SetBinError(i,err);
      }
    }
  }

  void calc_error_() {
    if (dimensions_==1) {
      for (int j=0; j<=(cumul_!=0); j++) {
	if (npostfix_==1) {
	  error_(h1d_1p_[0+2*j],h1d_1p_[1+2*j]);
	} else if (npostfix_==2) {
	  for (size_t k=0; k<h1d_2p_.size(); k++) 
	    error_(h1d_2p_[k][0+2*j],h1d_2p_[k][1+2*j]);
	} else if (npostfix_==3) { 
	  for (size_t k=0; k<h1d_3p_.size(); k++) 
	    for (size_t l=0; l<h1d_3p_[k].size(); l++) 
	      error_(h1d_3p_[k][l][0+2*j],h1d_3p_[k][l][1+2*j]);
	} else if (npostfix_==4) {
	  for (size_t k=0; k<h1d_4p_.size(); k++) 
	    for (size_t l=0; l<h1d_4p_[k].size(); l++) 
	      for (size_t m=0; m<h1d_4p_[k][l].size(); m++) 
		error_(h1d_4p_[k][l][m][0+2*j],h1d_4p_[k][l][m][1+2*j]);
	}
      }
    }
  }

  void fit_treshold_(TH2D* pcdist) {
    Int_t nbiny = pcdist->GetNbinsY();
    double ylow = pcdist->GetYaxis()->GetXmin();
    double yup = pcdist->GetYaxis()->GetXmax();
    TFile fe("pcd.root","recreate");
    for (int i=1; i<=pcdist->GetNbinsX(); i++) {
      std::stringstream name;
      name<<"yfit_"<<i;
      TH1D *yfit = new TH1D(name.str().c_str(),name.str().c_str(),nbiny,ylow,yup);
      yfit->SetDirectory(NULL);
      for (int j=1; j<=nbiny; j++) yfit->SetBinContent(j,pcdist->GetBinContent(i,j));
      yfit->Write();
      delete yfit;
    }
    fe.Close();
  }

  void fit_landau_(TH2D* ncdist, TH1D* mpv) {
    Int_t nbiny = ncdist->GetNbinsY();
    double ylow = ncdist->GetYaxis()->GetXmin();
    double yup = ncdist->GetYaxis()->GetXmax();
    for (int i=1; i<=ncdist->GetNbinsX(); i++) {
      TH1D *yfit = new TH1D("yfit","yfit",nbiny,ylow,yup);
      double entries = 0;
      for (int j=1; j<=nbiny; j++) {
	double cont = ncdist->GetBinContent(i,j);
	entries += cont;
	yfit->SetBinContent(j,cont);
      }
      if (entries>100) {
	TF1 *f = new TF1("landau","landau",0.0,100.0);
	yfit->Fit("landau","RMWQ0");
	mpv->SetBinContent(i,f->GetParameter(1));
	mpv->SetBinError(i,f->GetParError(1));
	delete f;
      }
      delete yfit;
    }
  }

  void calc_def_() {
    if (dimensions_==1) {
      if (npostfix_==2) { 
	for (size_t i=0; i<h1d_2p_.size(); i++) {
	  fit_landau_(h2d_2p_[i][0],h1d_2p_[i][2]);
	  // h1d_2p_[i][3]->Divide(h1d_2p_[i][2]);
	  // h1d_2p_[i][4]->Divide(h1d_2p_[i][2]);
	  // fit_landau_(h2d_2p_[i][0],h1d_2p_[i][5]);
	  // h1d_2p_[i][6]->Divide(h1d_2p_[i][2]);
	  // h1d_2p_[i][7]->Divide(h1d_2p_[i][2]);
	  // h1d_2p_[i][8]->Divide(h1d_2p_[i][2]);
	  // h1d_2p_[i][9]->Divide(h1d_2p_[i][10]);
	  // h1d_2p_[i][10]->Divide(h1d_1p_[0]);
	  // h1d_2p_[i][11]->Divide(h1d_1p_[0]);
	  // // if (i==3) fit_treshold_(h2d_2p_[i][1]);
	  // // if (i==3) fit_treshold_(h2d_2p_[i][2]);
	}
      }
      else if (npostfix_==3) {
	for (size_t i=0; i<h1d_3p_.size(); i++) for (size_t j=0; j<h1d_3p_[i].size(); j++) {
	  h1d_3p_[i][j][3]->Divide(h1d_3p_[i][j][2]);
	  h1d_3p_[i][j][4]->Divide(h1d_3p_[i][j][2]);
	  fit_landau_(h2d_3p_[i][j][0],h1d_3p_[i][j][5]);
	  h1d_3p_[i][j][6]->Divide(h1d_3p_[i][j][2]);
	  h1d_3p_[i][j][7]->Divide(h1d_3p_[i][j][2]);
	  h1d_3p_[i][j][8]->Divide(h1d_3p_[i][j][2]);
	  h1d_3p_[i][j][9]->Divide(h1d_3p_[i][j][10]);
	  h1d_3p_[i][j][10]->Divide(h1d_1p_[0]);
	  h1d_3p_[i][j][11]->Divide(h1d_1p_[0]);
/* 	  if (i==3&&j==0) fit_treshold_(h2d_3p_[i][j][1]); */
/* 	  if (i==3&&j==0) fit_treshold_(h2d_3p_[i][j][2]); */
	}
      }
      else if (npostfix_==4) { 
	for (size_t i=0; i<h1d_4p_.size(); i++) for (size_t j=0; j<h1d_4p_[i].size(); j++) 
	  for (size_t k=0; k<h1d_4p_[i][j].size(); k++) {
	    fit_landau_(h2d_4p_[i][j][k][0],h1d_4p_[i][j][k][2]);
	    fit_landau_(h2d_4p_[i][j][k][1],h1d_4p_[i][j][k][3]);
	  }
      }
    }
  }

  std::vector<int> string_to_vector_(std::string val) {
    std::vector<int> vec;
    // Reading values from string separated by commas, (ranges x-y can be specified too)
    // eg. "3-6,20,22,24-27,11"
    val += ","; // needed to terminate loop properly
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
  
  bool char_exist_in_string_(std::string &s, char c) {
    std::stringstream ss(s);
    char testchar;
    while (ss) {
      ss.get(testchar);
      if (testchar==c) return 1;
      if (! ss)	return 0;
    } return 0;
  }

  void set_color_(TH1D* h, Color_t col, std::string &opt) {
    if (char_exist_in_string_(opt,'P')) h->SetMarkerColor(col);
    else {
      h->SetLineColor(col);
      h->SetLineWidth(2);
    }
    TList *l = h->GetListOfFunctions();
    for (Int_t i=0; i<l->GetSize(); ++i) 
      h->GetFunction(l->At(i)->GetName())->SetLineColor(col);
  }
  void set_marker_(TH1D* h, Int_t mar, std::string &opt) {
    if (char_exist_in_string_(opt,'P')) h->SetMarkerStyle(mar);
  }
  
  const char* legmark_(std::string &opt) {
    if (char_exist_in_string_(opt,'P')) return "P";
    else return "L";
  }

  void setstyle_(TH1D *h, std::string xtitle="", std::string ytitle="", 
		 Float_t xoffset=0.9, Float_t yoffset=1.4, 
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
  }

  void setstyle_(TH2D *h, std::string xtitle="", std::string ytitle="", std::string ztitle="", 
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

  std::string title_(int i) {
    std::string title="";
    if (eff_==1) {
      if (i==0) title = "Efficiency";
      else if (i==1) title = "Arbitrary"; }
    if (cumul_!=0) {
      if (i==2) title = "Cumulative Efficiency";
      else if (i==3) title = "Cumulative Hit Distribution"; }
    if (clusize_!=0) {
      if (i==2) title = "Average On-Track Cluster Size";
      else if (i==3) title = "Number of Clusters"; }
    if (def_!=0) {
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

 // Modifiers
 public:

  //Delcarators

  // 1 postfix
  // 1D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1,
	   int nbins_x, double low_x, double high_x) {
    if (opt1==1) {
      for (int i=low1-1; i<high1; i++) {
	std::string name=histoname_;
	name+=postfix1[i];
	TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
	h1d_1p_.push_back(h);
      }
    }
  }

  // 2D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y) {
    if (opt1==1) {
      for (int i=low1-1; i<high1; i++) {
	std::string name=histoname_;
	name+=postfix1[i];
	TH2D* h = new TH2D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y);
	h2d_1p_.push_back(h);
      }
    }
  }

  // 3D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y,
	   int nbins_z, double low_z, double high_z) {
    if (opt1==1) {
      for (int i=low1; i<high1; i++) {
	std::string name=histoname_;
	name+=postfix1[i];
	TH3D* h = new TH3D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y, nbins_z, low_z, high_z);
	h3d_1p_.push_back(h);
      }
    }
  }

  // 2 postfix
  // 1D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2,
	   int nbins_x, double low_x, double high_x) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) { h1d_2p_.push_back(std::vector<TH1D*>()); 
	if (def_==1) h2d_2p_.push_back(std::vector<TH2D*>()); }
      if (opt2==1) {
	for (int j=low2-1; j<high2;j++) {
	  std::string name=histoname_ + postfix1[i] + postfix2[j];
	  TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
	  h1d_2p_[i-skip1].push_back(h);
	}
	// if (def_==1) {
	//   std::string name2 = histoname_ + postfix1[i] + "_ncdist";
	//   TH2D* h2 = new TH2D(name2.c_str(), name2.c_str(), nbins_x, low_x, high_x, 100, 0.0, 100.0);
	//   h2d_2p_[i-skip1].push_back(h2);
	//   std::string name3 = histoname_ + postfix1[i] +"_pcdist";
	//   TH2D* h3 = new TH2D(name3.c_str(), name3.c_str(), nbins_x, low_x, high_x, 500, 0.0, 50.0);
	//   h2d_2p_[i-skip1].push_back(h3);
	//   std::string name4 = histoname_ + postfix1[i] +"_corrpcdist";
	//   TH2D* h4 = new TH2D(name4.c_str(), name4.c_str(), nbins_x, low_x, high_x, 500, 0.0, 50.0);
	//   h2d_2p_[i-skip1].push_back(h4);
	//   std::string name6 = histoname_ + postfix1[i] +"_aoidist";
	//   TH2D* h6 = new TH2D(name6.c_str(), name6.c_str(), nbins_x, low_x, high_x, 170, 0.0, 1.7);
	//   h2d_2p_[i-skip1].push_back(h6);
	// }
      }
      if (def_==1) {
	std::string name2 = histoname_ + postfix1[i] + "_ncdist";
	TH2D* h2 = new TH2D(name2.c_str(), name2.c_str(), nbins_x, low_x, high_x, 100, 0.0, 100.0);
	h2d_2p_[i-skip1].push_back(h2);
      }
    }
    // if (def_==1) {
    //   std::string name5 = histoname_ + "_nevt";
    //   TH1D* h5 = new TH1D(name5.c_str(), name5.c_str(), nbins_x, low_x, high_x);
    //   h1d_1p_.push_back(h5);
    // }
  }

  // 2D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) h2d_2p_.push_back(std::vector<TH2D*>());
      if (opt2==1) {
	for (int j=low2-1; j<high2;j++) {
	  std::string name=histoname_;
	  name+=postfix1[i];
	  name+=postfix2[j];
	  TH2D* h = new TH2D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y);
	  h2d_2p_[i-skip1].push_back(h);
	}
      }
    }
  }

  // 3D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y,
	   int nbins_z, double low_z, double high_z) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) h3d_2p_.push_back(std::vector<TH3D*>());
      if (opt2==1) {
	for (int j=low2-1; j<high2;j++) {
	  std::string name=histoname_;
	  name+=postfix1[i];
	  name+=postfix2[j];
	  TH3D* h = new TH3D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y, nbins_z, low_z, high_z);
	  h3d_2p_[i-skip1].push_back(h);
	}
      }
    }
  }

  // 3 postfix
  // 1D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2, int skip2,
	   std::vector<std::string> &postfix3, int opt3, int low3, int high3,
	   int nbins_x, double low_x, double high_x) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) { h1d_3p_.push_back(std::vector<std::vector<TH1D*> >()); if (def_==1) h2d_3p_.push_back(std::vector<std::vector<TH2D*> >()); }
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) { h1d_3p_[i-skip1].push_back(std::vector<TH1D*>()); if (def_==1) h2d_3p_[i-skip1].push_back(std::vector<TH2D*>()); }
	for (int k=low3-1; k<high3;k++) {
	  if (opt3==1) {
	    std::string name=histoname_;
	    name+=postfix1[i];
	    name+=postfix2[j];
	    name+=postfix3[k];
	    TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
	    h1d_3p_[i-skip1][j-skip2].push_back(h);
	  }
	}
	if (def_==1) {
	  std::string name2 = histoname_ + postfix1[i] + postfix2[j] +"_ncdist";
	  TH2D* h2 = new TH2D(name2.c_str(), name2.c_str(), nbins_x, low_x, high_x, 100, 0.0, 100.0);
	  h2d_3p_[i-skip1][j-skip2].push_back(h2);
	  std::string name3 = histoname_ + postfix1[i] + postfix2[j] +"_pcdist";
	  TH2D* h3 = new TH2D(name3.c_str(), name3.c_str(), nbins_x, low_x, high_x, 500, 0.0, 50.0);
	  h2d_3p_[i-skip1][j-skip2].push_back(h3);
	  std::string name4 = histoname_ + postfix1[i] + postfix2[j] +"_corrpcdist";
	  TH2D* h4 = new TH2D(name4.c_str(), name4.c_str(), nbins_x, low_x, high_x, 500, 0.0, 50.0);
	  h2d_3p_[i-skip1][j-skip2].push_back(h4);
	  std::string name6 = histoname_ + postfix1[i] + postfix2[j] +"_aoidist";
	  TH2D* h6 = new TH2D(name6.c_str(), name6.c_str(), nbins_x, low_x, high_x, 170, 0.0, 1.7);
	  h2d_3p_[i-skip1][j-skip2].push_back(h6);
	}
      }
    }
    if (def_==1) {
      std::string name5 = histoname_ + "_nevt";
      TH1D* h5 = new TH1D(name5.c_str(), name5.c_str(), nbins_x, low_x, high_x);
      h1d_1p_.push_back(h5);
    }
  }

  // 2D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2, int skip2,
	   std::vector<std::string> &postfix3, int opt3, int low3, int high3,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) h2d_3p_.push_back(std::vector<std::vector<TH2D*> >());
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) h2d_3p_[i-skip1].push_back(std::vector<TH2D*>());
	for (int k=low3-1; k<high3;k++) {
	  if (opt3==1) {
	    std::string name=histoname_;
	    name+=postfix1[i];
	    name+=postfix2[j];
	    name+=postfix3[k];
	    TH2D* h = new TH2D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y);
	    h2d_3p_[i-skip1][j-skip2].push_back(h);
	  }
	}
      }
    }
  }

  // 3D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2, int skip2,
	   std::vector<std::string> &postfix3, int opt3, int low3, int high3,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y,
	   int nbins_z, double low_z, double high_z) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) h3d_3p_.push_back(std::vector<std::vector<TH3D*> >());
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) h3d_3p_[i-skip1].push_back(std::vector<TH3D*>());
	for (int k=low3-1; k<high3;k++) {
	  if (opt3==1) {
	    std::string name=histoname_;
	    name+=postfix1[i];
	    name+=postfix2[j];
	    name+=postfix3[k];
	    TH3D* h = new TH3D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y, nbins_z, low_z, high_z);
	    h3d_3p_[i-skip1][j-skip2].push_back(h);
	  }
	}
      }
    }
  }

  // 4 postfix
  // 1D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2, int skip2,
	   std::vector<std::string> &postfix3, int opt3, int low3, int high3, int skip3,
	   std::vector<std::string> &postfix4, int opt4, int low4, int high4,
	   int nbins_x, double low_x, double high_x) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1) { h1d_4p_.push_back(std::vector<std::vector<std::vector<TH1D*> > >()); 
	if (def_==1) h2d_4p_.push_back(std::vector<std::vector<std::vector<TH2D*> > >()); }
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) { h1d_4p_[i-skip1].push_back(std::vector<std::vector<TH1D*> >());
	  if (def_==1) h2d_4p_[i-skip1].push_back(std::vector<std::vector<TH2D*> >()); }
	for (int k=low3-1; k<high3;k++) {
	  if (opt3==1) { h1d_4p_[i-skip1][j-skip2].push_back(std::vector<TH1D*>());
	    if (def_==1) h2d_4p_[i-skip1][j-skip2].push_back(std::vector<TH2D*>()); }
	  for (int l=low4-1; l<high4;l++) {
	    if (opt4==1) { 
	      std::string name = histoname_ + postfix1[i] + postfix2[j] + postfix3[k] + postfix4[l];
	      TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
	      h1d_4p_[i-skip1][j-skip2][k-skip3].push_back(h);
	    }
	  }
	  if (def_==1) {
	    std::string name2 = histoname_ + postfix1[i] + postfix2[j] + postfix3[k] +"_ncdist";
	    TH2D* h2 = new TH2D(name2.c_str(), name2.c_str(), nbins_x, low_x, high_x, 100, 0.0, 100.0);
	    h2d_4p_[i-skip1][j-skip2][k-skip3].push_back(h2);
	    std::string name3 = histoname_ + postfix1[i] + postfix2[j] + postfix3[k] +"_pcdist";
	    TH2D* h3 = new TH2D(name3.c_str(), name3.c_str(), nbins_x, low_x, high_x, 500, 0.0, 50.0);
	    h2d_4p_[i-skip1][j-skip2][k-skip3].push_back(h3);
	  }
	}
      }
    }
  }

  // 2D
  void add(std::vector<std::string> &postfix1, int opt1, int low1, int high1, int skip1,
	   std::vector<std::string> &postfix2, int opt2, int low2, int high2, int skip2,
	   std::vector<std::string> &postfix3, int opt3, int low3, int high3, int skip3,
	   std::vector<std::string> &postfix4, int opt4, int low4, int high4,
	   int nbins_x, double low_x, double high_x,
	   int nbins_y, double low_y, double high_y) {
    for (int i=low1-1; i<high1;i++) {
      if (opt1==1)  h2d_4p_.push_back(std::vector<std::vector<std::vector<TH2D*> > >()); 
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) h2d_4p_[i-skip1].push_back(std::vector<std::vector<TH2D*> >());
	for (int k=low3-1; k<high3;k++) {
	  if (opt3==1) h2d_4p_[i-skip1][j-skip2].push_back(std::vector<TH2D*>());
	  for (int l=low4-1; l<high4;l++) if (opt4==1) { 
	    std::string name = histoname_ + postfix1[i] + postfix2[j] + postfix3[k] + postfix4[l];
	    TH2D* h = new TH2D(name.c_str(), name.c_str(), nbins_x, low_x, high_x, nbins_y, low_y, high_y);
	    h2d_4p_[i-skip1][j-skip2][k-skip3].push_back(h);
	  }
	}
      }
    }
  }
  
  // Load
  void load(TFile &f) {
    std::cout<<"Loading histo: "<<histoname_<<std::endl;
    if (npostfix_==1) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_1p_.size(); i++) {
	  std::string name = h1d_1p_[i]->GetName();
	  delete h1d_1p_[i];
	  h1d_1p_[i] = (TH1D*)f.Get(name.c_str());
	  h1d_1p_[i]->SetDirectory(NULL);
	}
      } else if (dimensions_==2) {
	for (size_t i=0; i<h2d_1p_.size(); i++) {
	  std::string name = h2d_1p_[i]->GetName();
	  delete h2d_1p_[i];
	  h2d_1p_[i] = (TH2D*)f.Get(name.c_str());
	  h2d_1p_[i]->SetDirectory(NULL);
	}
      } else if (dimensions_==3) {
	for (size_t i=0; i<h3d_1p_.size(); i++) {
	  std::string name = h3d_1p_[i]->GetName();
	  delete h3d_1p_[i];
	  h3d_1p_[i] = (TH3D*)f.Get(name.c_str());
	  h3d_1p_[i]->SetDirectory(NULL);
	}
      }
    } else if (npostfix_==2) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_2p_.size(); i++) {
	  for (size_t j=0; j<h1d_2p_[i].size(); j++) {
	    std::string name = h1d_2p_[i][j]->GetName();
	    delete h1d_2p_[i][j];
	    h1d_2p_[i][j] = (TH1D*)f.Get(name.c_str());
	    h1d_2p_[i][j]->SetDirectory(NULL);
	  }

	  if (def_==1) {
	    for (size_t j=0; j<h2d_2p_[i].size(); j++) {
	      std::string name = h2d_2p_[i][j]->GetName();
	      delete h2d_2p_[i][j];
	      h2d_2p_[i][j] = (TH2D*)f.Get(name.c_str());
	      h2d_2p_[i][j]->SetDirectory(NULL);
	    }
	  }
	}
	// if (def_==1) {
	//   std::string name = h1d_1p_[0]->GetName();
	//   delete h1d_1p_[0];
	//   h1d_1p_[0] = (TH1D*)f.Get(name.c_str());
	//   h1d_1p_[0]->SetDirectory(NULL);
	// }
      } else if (dimensions_==2) {
	for (size_t i=0; i<h2d_2p_.size(); i++) {
	  for (size_t j=0; j<h2d_2p_[i].size(); j++) {
	    std::string name = h2d_2p_[i][j]->GetName();
	    delete h2d_2p_[i][j];
	    h2d_2p_[i][j] = (TH2D*)f.Get(name.c_str());
	    h2d_2p_[i][j]->SetDirectory(NULL);
	  }
	}
      } else if (dimensions_==3) {
	for (size_t i=0; i<h3d_2p_.size(); i++) {
	  for (size_t j=0; j<h3d_2p_[i].size(); j++) {
	    std::string name = h3d_2p_[i][j]->GetName();
	    delete h3d_2p_[i][j];
	    h3d_2p_[i][j] = (TH3D*)f.Get(name.c_str());
	    h3d_2p_[i][j]->SetDirectory(NULL);
	  }
	}
      }
    } else if (npostfix_==3) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_3p_.size(); i++) {
	  for (size_t j=0; j<h1d_3p_[i].size(); j++) {
	    for (size_t k=0; k<h1d_3p_[i][j].size(); k++) {
	      std::string name = h1d_3p_[i][j][k]->GetName();
	      delete h1d_3p_[i][j][k];
	      h1d_3p_[i][j][k] = (TH1D*)f.Get(name.c_str());
	      h1d_3p_[i][j][k]->SetDirectory(NULL);
	      if (def_==1) { 
		for (size_t k=0; k<h2d_3p_[i][j].size(); k++) {
		  std::string name = h2d_3p_[i][j][k]->GetName();
		  delete h2d_3p_[i][j][k];
		  h2d_3p_[i][j][k] = (TH2D*)f.Get(name.c_str());
		  h2d_3p_[i][j][k]->SetDirectory(NULL);
		}
	      }
	    }
	  }
	}
      	if (def_==1) {
	  std::string name = h1d_1p_[0]->GetName();
	  delete h1d_1p_[0];
	  h1d_1p_[0] = (TH1D*)f.Get(name.c_str());
	  h1d_1p_[0]->SetDirectory(NULL);
	}
      } else if (dimensions_==2) {
	for (size_t i=0; i<h2d_3p_.size(); i++) {
	  for (size_t j=0; j<h2d_3p_[i].size(); j++) {
	    for (size_t k=0; k<h2d_3p_[i][j].size(); k++) {
	      std::string name = h2d_3p_[i][j][k]->GetName();
	      delete h2d_3p_[i][j][k];
	      h2d_3p_[i][j][k] = (TH2D*)f.Get(name.c_str());
	      h2d_3p_[i][j][k]->SetDirectory(NULL);
	    }
	  }
	}
      } else if (dimensions_==3) {
	for (size_t i=0; i<h3d_3p_.size(); i++) {
	  for (size_t j=0; j<h3d_3p_[i].size(); j++) {
	    for (size_t k=0; k<h3d_3p_[i][j].size(); k++) {
	      std::string name = h3d_3p_[i][j][k]->GetName();
	      delete h3d_3p_[i][j][k];
	      h3d_3p_[i][j][k] = (TH3D*)f.Get(name.c_str());
	      h3d_3p_[i][j][k]->SetDirectory(NULL);
	    }
	  }
	}
      }
    } else if (npostfix_==4) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_4p_.size(); i++) {
	  for (size_t j=0; j<h1d_4p_[i].size(); j++) {
	    for (size_t k=0; k<h1d_4p_[i][j].size(); k++) {
	      for (size_t l=0; l<h1d_4p_[i][j][k].size(); l++) {
		std::string name = h1d_4p_[i][j][k][l]->GetName();
		TH1D *h = (TH1D*)f.Get(name.c_str());
		if (h!=0) {
		  delete h1d_4p_[i][j][k][l];
		  h1d_4p_[i][j][k][l] = h;
		  h1d_4p_[i][j][k][l]->SetDirectory(NULL);
		}
		// delete h1d_4p_[i][j][k][l];
		// h1d_4p_[i][j][k][l] = (TH1D*)f.Get(name.c_str());
		// h1d_4p_[i][j][k][l]->SetDirectory(NULL);
	      }
	    }
	  }
	}
      }
    }
  }

  void partial_load(TFile &f, int i, int refill=0) {
    if (npostfix_==1) {
      if (dimensions_==1) {
	std::string name = h1d_1p_[i]->GetName();
	if (refill!=1) {
	  delete h1d_1p_[i];
	  h1d_1p_[i] = (TH1D*)f.Get(name.c_str());
	  h1d_1p_[i]->SetDirectory(NULL);
	} else {
	  TH1D *h = (TH1D*)f.Get(name.c_str());
	  h->SetDirectory(NULL);
	  for (Int_t a=1; a<=h->GetNbinsX(); a++) {
	    Int_t binx = h1d_1p_[i]->FindBin(h->GetXaxis()->GetBinCenter(a));
	    h1d_1p_[i]->SetBinContent(binx,h->GetBinContent(a));
	    h1d_1p_[i]->SetBinError(binx,h->GetBinError(a));
	  }
	  delete h;
	}
      }
    } else if (npostfix_==2) {
      for (size_t j=0; j<h1d_2p_[i].size(); j++) {
	std::string name = h1d_2p_[i][j]->GetName();
	if (refill!=1) {
	  delete h1d_2p_[i][j];
	  h1d_2p_[i][j] = (TH1D*)f.Get(name.c_str());
	  h1d_2p_[i][j]->SetDirectory(NULL);
	} else {
	  TH1D *h = (TH1D*)f.Get(name.c_str());
	  h->SetDirectory(NULL);
	  for (Int_t a=1; a<=h->GetNbinsX(); a++) {
	    Int_t binx = h1d_2p_[i][j]->FindBin(h->GetXaxis()->GetBinCenter(a));
	      h1d_2p_[i][j]->SetBinContent(binx,h->GetBinContent(a));
	      h1d_2p_[i][j]->SetBinError(binx,h->GetBinError(a));
	  }
	  delete h;
	}
      }
      for (size_t j=0; j<h2d_2p_[i].size(); j++) {
	std::string name = h2d_2p_[i][j]->GetName();
	delete h2d_2p_[i][j];
	h2d_2p_[i][j] = (TH2D*)f.Get(name.c_str());
	h2d_2p_[i][j]->SetDirectory(NULL);
      }
    } else if (npostfix_==3) {
      for (size_t j=0; j<h1d_3p_[i].size(); j++) {
	for (size_t k=0; k<h1d_3p_[i][j].size(); k++) {
	  std::string name = h1d_3p_[i][j][k]->GetName();
	  if (refill!=1) {
	    delete h1d_3p_[i][j][k];
	    h1d_3p_[i][j][k] = (TH1D*)f.Get(name.c_str());
	    h1d_3p_[i][j][k]->SetDirectory(NULL);
	  } else {
	    TH1D *h = (TH1D*)f.Get(name.c_str());
	    h->SetDirectory(NULL);
	    for (Int_t a=1; a<=h->GetNbinsX(); a++) {
	      Int_t binx = h1d_3p_[i][j][k]->FindBin(h->GetXaxis()->GetBinCenter(a));
		h1d_3p_[i][j][k]->SetBinContent(binx,h->GetBinContent(a));
		h1d_3p_[i][j][k]->SetBinError(binx,h->GetBinError(a));
	    }
	    delete h;
	  }
	}
      }
      for (size_t j=0; j<h2d_3p_[i].size(); j++) {
	for (size_t k=0; k<h2d_3p_[i][j].size(); k++) {
	  std::string name = h2d_3p_[i][j][k]->GetName();
	  delete h2d_3p_[i][j][k];
	  h2d_3p_[i][j][k] = (TH2D*)f.Get(name.c_str());
	  h2d_3p_[i][j][k]->SetDirectory(NULL);
	}
      }
    } else if (npostfix_==4) {
      for (size_t j=0; j<h1d_4p_[i].size(); j++) {
	for (size_t k=0; k<h1d_4p_[i][j].size(); k++) {
	  for (size_t l=0; l<h1d_4p_[i][j][k].size(); l++) {
	    std::string name = h1d_4p_[i][j][k][l]->GetName();
	    if (refill!=1) {
	      TH1D *h = (TH1D*)f.Get(name.c_str());
	      if (h!=0) {
		delete h1d_4p_[i][j][k][l];
		h1d_4p_[i][j][k][l] = h;
		h1d_4p_[i][j][k][l]->SetDirectory(NULL);
	      }
	    } else {
	      TH1D *h = (TH1D*)f.Get(name.c_str());
	      if (h!=0) {
		h->SetDirectory(NULL);
		for (Int_t a=1; a<=h->GetNbinsX(); a++) {
		  Int_t binx = h1d_4p_[i][j][k][l]->FindBin(h->GetXaxis()->GetBinCenter(a));
		  h1d_4p_[i][j][k][l]->SetBinContent(binx,h->GetBinContent(a));
		  h1d_4p_[i][j][k][l]->SetBinError(binx,h->GetBinError(a));
		}
	      }
	      delete h;
	    }
	  }
	}
      }
      for (size_t j=0; j<h2d_4p_[i].size(); j++) {
	for (size_t k=0; k<h2d_4p_[i][j].size(); k++) {
	  for (size_t l=0; l<h2d_4p_[i][j][k].size(); l++) {
	    std::string name = h2d_4p_[i][j][k][l]->GetName();
	    TH2D *h = (TH2D*)f.Get(name.c_str());
	    if (h!=0) {
	      delete h2d_4p_[i][j][k][l];
	      h2d_4p_[i][j][k][l] = h;
	      h2d_4p_[i][j][k][l]->SetDirectory(NULL);
	    }
	  }
	}
      }
    }
  }
  
  void eff_revert(TH1D* eff, TH1D* dist) {
    eff->Multiply(dist);
    dist->Add(eff,-1);
  }

  void eff_revert(TH2D* eff, TH2D* dist) {
    eff->Multiply(dist);
    dist->Add(eff,-1);
  }
  
  // Reverting Efficiency plots
  void revert_eff() {
    if (eff_==1) {
      if (dimensions_==1) {
        if (npostfix_==1) {
          eff_revert(h1d_1p_[0],h1d_1p_[1]);
        } else if (npostfix_==2) {
          for (size_t i=0; i<h1d_2p_.size(); i++)
            eff_revert(h1d_2p_[i][0],h1d_2p_[i][1]);
        } else if (npostfix_==3) {
          for (size_t i=0; i<h1d_3p_.size(); i++) 
            for (size_t j=0; j<h1d_3p_[i].size(); j++)
              eff_revert(h1d_3p_[i][j][0],h1d_3p_[i][j][1]);
        } else if (npostfix_==4) {
          for (size_t i=0; i<h1d_4p_.size(); i++) 
            for (size_t j=0; j<h1d_4p_[i].size(); j++)
              for (size_t k=0; k<h1d_4p_[i][j].size(); k++)
                eff_revert(h1d_4p_[i][j][k][0],h1d_4p_[i][j][k][1]);
        }
      } else {
        if (npostfix_==1) {
          eff_revert(h2d_1p_[0],h2d_1p_[1]);
        } else if (npostfix_==2) {
          for (size_t i=0; i<h2d_2p_.size(); i++)
            eff_revert(h2d_2p_[i][0],h2d_2p_[i][1]);
        } else if (npostfix_==3) {
          for (size_t i=0; i<h2d_3p_.size(); i++) 
            for (size_t j=0; j<h2d_3p_[i].size(); j++)
              eff_revert(h2d_3p_[i][j][0],h2d_3p_[i][j][1]);
        } else if (npostfix_==4) {
          for (size_t i=0; i<h2d_4p_.size(); i++) 
            for (size_t j=0; j<h2d_4p_[i].size(); j++)
              for (size_t k=0; k<h2d_4p_[i][j].size(); k++)
                eff_revert(h2d_4p_[i][j][k][0],h2d_4p_[i][j][k][1]);
        }	
      }
    }
  }

  // Fill functions
  // 1 Postfix
  // Increasebin
  // 1D
  void increasebin_1d(int p1, int binx) {
    h1d_1p_[p1]->SetBinContent(binx, h1d_1p_[p1]->GetBinContent(binx)+1);
  }

  void increasebinw_1d(int p1, int binx, double w) {
    h1d_1p_[p1]->SetBinContent(binx, h1d_1p_[p1]->GetBinContent(binx)+w);
  }

  void det_fill_1d(const ModuleData &m, double x) {
    h1d_1p_[0]->Fill(x);
    h1d_1p_[1+m.det]->Fill(x);
    h1d_1p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)]->Fill(x);
  }

  // 2D
  void increasebin_2d(int p1, int binx, int biny) {
    h2d_1p_[p1]->SetBinContent(binx, biny, h2d_1p_[p1]->GetBinContent(binx,biny)+1);
  }

  void increasebinw_2d(int p1, int binx, int biny, double w) {
    h2d_1p_[p1]->SetBinContent(binx, biny, h2d_1p_[p1]->GetBinContent(binx,biny)+w);
  }
  
  // 2 Postfix
  // 1D
  void increasebin_1d(int p1, int p2, int binx) {
    h1d_2p_[p1][p2]->SetBinContent(binx, h1d_2p_[p1][p2]->GetBinContent(binx)+1);
  }

  void increasebinw_1d(int p1, int p2, int binx, double w) {
    h1d_2p_[p1][p2]->SetBinContent(binx, h1d_2p_[p1][p2]->GetBinContent(binx)+w);
  }
  
  void det_increasebin_1d(const ModuleData &m, int p2, int binx) {
    h1d_2p_[0][p2]->SetBinContent(binx, h1d_2p_[0][p2]->GetBinContent(binx)+1);
    h1d_2p_[1+m.det][p2]->SetBinContent(binx, h1d_2p_[1+m.det][p2]->GetBinContent(binx)+1);
    int lay = (m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0);
    h1d_2p_[lay][p2]->SetBinContent(binx, h1d_2p_[lay][p2]->GetBinContent(binx)+1);
  }

  void det_fill_1d(int p1, const ModuleData &m, double x) {
    h1d_2p_[p1][0]->Fill(x);
    h1d_2p_[p1][1+m.det]->Fill(x);
    h1d_2p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)]->Fill(x);
  }

  void det_fillw_1d(int p1, const ModuleData &m, double x, double w) {
    h1d_2p_[p1][0]->Fill(x, w);
    h1d_2p_[p1][1+m.det]->Fill(x, w);
    h1d_2p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)]->Fill(x, w);
  }

  void det_fill_1d(const ModuleData &m, int p2, double x) {
    h1d_2p_[0][p2]->Fill(x);
    h1d_2p_[1+m.det][p2]->Fill(x);
    h1d_2p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2]->Fill(x);
  }

  void det_fillw_1d(const ModuleData &m, int p2, double x, double w) {
    h1d_2p_[0][p2]->Fill(x,w);
    h1d_2p_[1+m.det][p2]->Fill(x,w);
    h1d_2p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2]->Fill(x,w);
  }

  void det2_fill_1d(const ModuleData &m, int p2, double x) {
    h1d_2p_[0][p2]->Fill(x);
    h1d_2p_[1+m.det][p2]->Fill(x);
    h1d_2p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2]->Fill(x);
    // FPix plots
    if (m.det) {
      int IO = m.panel+m.module > 3;
      h1d_2p_[9+abs(m.disk)][p2]->Fill(x);
      h1d_2p_[10+abs(m.disk)*2+IO][p2]->Fill(x);
      h1d_2p_[18+(m.disk+(m.disk<0))*2+IO][p2]->Fill(x);
    }
  }

  void det2_fillw_1d(const ModuleData &m, int p2, double x, double w) {
    h1d_2p_[0][p2]->Fill(x,w);
    h1d_2p_[1+m.det][p2]->Fill(x,w);
    h1d_2p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2]->Fill(x,w);
    // FPix plots
    if (m.det) {
      int IO = m.panel+m.module > 3;
      h1d_2p_[9+abs(m.disk)][p2]->Fill(x,w);
      h1d_2p_[10+abs(m.disk)*2+IO][p2]->Fill(x,w);
      h1d_2p_[18+(m.disk+(m.disk<0))*2+IO][p2]->Fill(x,w);
    }
  }

  // 2D
  void increasebin_2d(int p1, int p2, int binx, int biny) {
    h2d_2p_[p1][p2]->SetBinContent(binx, biny, h2d_2p_[p1][p2]->GetBinContent(binx,biny)+1);
  }

  void increasebinw_2d(int p1, int p2, int binx, int biny, double w) {
    h2d_2p_[p1][p2]->SetBinContent(binx, biny, h2d_2p_[p1][p2]->GetBinContent(binx,biny)+w);
  }

  void det_fill_2d(int p1, const ModuleData &m, double x, double y) {
    h2d_2p_[p1][0]->Fill(x, y);
    h2d_2p_[p1][1+m.det]->Fill(x, y);
    h2d_2p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)]->Fill(x, y);
  }

  void det_fillw_2d(int p1, const ModuleData &m, double x, double y, double w) {
    h2d_2p_[p1][0]->Fill(x, y, w);
    h2d_2p_[p1][1+m.det]->Fill(x, y, w);
    h2d_2p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)]->Fill(x, y, w);
  }

  void det_fill_2d(const ModuleData &m, int p2, double x, double y) {
    h2d_2p_[0][p2]->Fill(x, y);
    h2d_2p_[1+m.det][p2]->Fill(x, y);
    h2d_2p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2]->Fill(x, y);
  }

  void det_fillw_2d(const ModuleData &m, int p2, double x, double y, double w) {
    h2d_2p_[0][p2]->Fill(x, y, w);
    h2d_2p_[1+m.det][p2]->Fill(x, y, w);
    h2d_2p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2]->Fill(x, y, w);
  }

  // 3D
  void det_fill_3d(const ModuleData &m, int p2, double x, double y, double z) {
    h3d_2p_[0][p2]->Fill(x, y, z);
    h3d_2p_[1+m.det][p2]->Fill(x, y, z);
    int lay = (m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0);
    h3d_2p_[lay][p2]->Fill(x, y, z);
  }

  void det_fillw_3d(const ModuleData &m, int p2, double x, double y, double z, double w) {
    h3d_2p_[0][p2]->Fill(x, y, z, w);
    h3d_2p_[1+m.det][p2]->Fill(x, y, z, w);
    int lay = (m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0);
    h3d_2p_[lay][p2]->Fill(x, y, z, w);
  }
  
  // 3 Postfix
  // 1D
  void increasebin_1d(int p1, int p2, int p3, int binx) {
    h1d_3p_[p1][p2][p3]->SetBinContent(binx, h1d_3p_[p1][p2][p3]->GetBinContent(binx)+1);
  }

  void det_fill_1d(const ModuleData &m, int p2, int p3, double x) {
    h1d_3p_[0][p2][p3]->Fill(x);
    h1d_3p_[1+m.det][p2][p3]->Fill(x);
    h1d_3p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2][p3]->Fill(x);
  }

  void det_fill_1d(int p1, const ModuleData &m, int p3, double x) {
    h1d_3p_[p1][0][p3]->Fill(x);
    h1d_3p_[p1][1+m.det][p3]->Fill(x);
    h1d_3p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3]->Fill(x);
  }

  void det_fillw_1d(int p1, const ModuleData &m, int p3, double x, double w) {
    h1d_3p_[p1][0][p3]->Fill(x, w);
    h1d_3p_[p1][1+m.det][p3]->Fill(x, w);
    h1d_3p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3]->Fill(x, w);
  }
  
  void det2_fill_1d(int p1, const ModuleData &m, int p3, double x) {
    h1d_3p_[p1][0][p3]->Fill(x);
    h1d_3p_[p1][1+m.det][p3]->Fill(x);
    h1d_3p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3]->Fill(x);
    // FPix plots
    if (m.det) {
      int IO = m.panel+m.module > 3;
      h1d_3p_[p1][9+abs(m.disk)][p3]->Fill(x);
      h1d_3p_[p1][10+abs(m.disk)*2+IO][p3]->Fill(x);
      h1d_3p_[p1][18+(m.disk+(m.disk<0))*2+IO][p3]->Fill(x);
    }
  }
  
  void det2_fillw_1d(int p1, const ModuleData &m, int p3, double x, double w) {
    h1d_3p_[p1][0][p3]->Fill(x,w);
    h1d_3p_[p1][1+m.det][p3]->Fill(x,w);
    h1d_3p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3]->Fill(x,w);
    // FPix plots
    if (m.det) {
      int IO = m.panel+m.module > 3;
      h1d_3p_[p1][9+abs(m.disk)][p3]->Fill(x,w);
      h1d_3p_[p1][10+abs(m.disk)*2+IO][p3]->Fill(x,w);
      h1d_3p_[p1][18+(m.disk+(m.disk<0))*2+IO][p3]->Fill(x,w);
    }
  }
  
  // 2D
  void increasebin_2d(int p1, int p2, int p3, int binx, int biny) {
    h2d_3p_[p1][p2][p3]->SetBinContent(binx, biny, h2d_3p_[p1][p2][p3]->GetBinContent(binx,biny)+1);
  }

  void det_fill_2d(int p1, const ModuleData &m, int p3, double x, double y) {
    h2d_3p_[p1][0][p3]->Fill(x,y);
    h2d_3p_[p1][1+m.det][p3]->Fill(x,y);
    h2d_3p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3]->Fill(x,y);
  }

  void det_fillw_2d(int p1, const ModuleData &m, int p3, double x, double y, double w) {
    h2d_3p_[p1][0][p3]->Fill(x, y, w);
    h2d_3p_[p1][1+m.det][p3]->Fill(x, y, w);
    h2d_3p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3]->Fill(x, y, w);
  }

  // 4 postfix
  // 1D
  void det_fill_1d(const ModuleData &m, int p2, int p3, int p4, double x) {
    h1d_4p_[0][p2][p3][p4]->Fill(x);
    h1d_4p_[1+m.det][p2][p3][p4]->Fill(x);
    h1d_4p_[(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p2][p3][p4]->Fill(x);
  }

  void det_fill_1d(int p1, const ModuleData &m, int p3, int p4, double x) {
    h1d_4p_[p1][0][p3][p4]->Fill(x);
    h1d_4p_[p1][1+m.det][p3][p4]->Fill(x);
    h1d_4p_[p1][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p3][p4]->Fill(x);
  }

  void det_fill_1d(int p1, int p2, const ModuleData &m, int p4, double x) {
    h1d_4p_[p1][p2][0][p4]->Fill(x);
    h1d_4p_[p1][p2][1+m.det][p4]->Fill(x);
    h1d_4p_[p1][p2][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p4]->Fill(x);
  }

  void det_fillw_1d(int p1, int p2, const ModuleData &m, int p4, double x, double w) {
    h1d_4p_[p1][p2][0][p4]->Fill(x,w);
    h1d_4p_[p1][p2][1+m.det][p4]->Fill(x,w);
    h1d_4p_[p1][p2][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p4]->Fill(x,w);
  }
  
  void det2_fill_1d(int p1, int p2, const ModuleData &m, int p4, double x) {
    h1d_4p_[p1][p2][0][p4]->Fill(x);
    h1d_4p_[p1][p2][1+m.det][p4]->Fill(x);
    h1d_4p_[p1][p2][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p4]->Fill(x);
    // FPix plots
    if (m.det) {
      int IO = m.panel+m.module > 3;
      h1d_4p_[p1][p2][9+abs(m.disk)][p4]->Fill(x);
      h1d_4p_[p1][p2][10+abs(m.disk)*2+IO][p4]->Fill(x);
      h1d_4p_[p1][p2][18+(m.disk+(m.disk<0))*2+IO][p4]->Fill(x);
    }
  }
  
  void det2_fillw_1d(int p1, int p2, const ModuleData &m, int p4, double x, double w) {
    h1d_4p_[p1][p2][0][p4]->Fill(x,w);
    h1d_4p_[p1][p2][1+m.det][p4]->Fill(x,w);
    h1d_4p_[p1][p2][(m.det==0) ? 2+m.layer : 7+m.disk+(m.disk<0)][p4]->Fill(x,w);
    // FPix plots
    if (m.det) {
      int IO = m.panel+m.module > 3;
      h1d_4p_[p1][p2][9+abs(m.disk)][p4]->Fill(x,w);
      h1d_4p_[p1][p2][10+abs(m.disk)*2+IO][p4]->Fill(x,w);
      h1d_4p_[p1][p2][18+(m.disk+(m.disk<0))*2+IO][p4]->Fill(x,w);
    }
  }
  
  // Fill with mpv and others
  void det_effmpv_fill(bool effcut, bool nccdcut, const TrajMeasurement &t, double x) {
    for (int i=0; i<3; i++) {
      int p1 = (i>0) ? ((i==1) ? 1+t.mod_on.det : ((t.mod_on.det==0) ? 2+t.mod_on.layer : 7+t.mod_on.disk+(t.mod_on.disk<0)) ) : 0;
      if ((t.validhit==1||t.missing==1)&&effcut)
	h1d_2p_[p1][t.missing]->Fill(x);
      if (t.norm_charge!=NOVAL_F&&nccdcut)
	h2d_2p_[p1][0]->Fill(x, t.norm_charge);
    }
  }

  void hist_effmpv_fill(int p1, int p2, int p3, bool effcut, bool nccdcut, bool pcdcut, const TrajMeasurement &t, double x) {
    if ((t.validhit==1||t.missing==1)&&effcut)
      h1d_4p_[p1][p2][p3][t.missing]->Fill(x);
    if (t.clu.size>0) {
      if (nccdcut)
	h2d_4p_[p1][p2][p3][0]->Fill(x, t.norm_charge);
      if (pcdcut) for (int i=0; i<t.clu.size; i++)
	h2d_4p_[p1][p2][p3][1]->Fill(x, t.clu.pix[i][0]);
    }
  }

  void det_def_fill(bool cut, const TrajMeasurement &t, double x, float gc) {
    if (npostfix_==2) {
      int lay = (t.mod_on.det==0) ? 2+t.mod_on.layer : 7+t.mod_on.disk+(t.mod_on.disk<0);
      if (cut) {
	if (t.validhit==1||t.missing==1) {
	  h1d_2p_[0]             [t.missing]->Fill(x);
	  h1d_2p_[1+t.mod_on.det][t.missing]->Fill(x);
	  h1d_2p_[lay]           [t.missing]->Fill(x);
	}
      }
      if (t.clu.size>0) {
/* 	float drcl12 = sqrt((t.dx_cl[1]-t.dx_cl[0])*(t.dx_cl[1]-t.dx_cl[0]) */
/* 			    +(t.dy_cl[1]-t.dy_cl[0])*(t.dy_cl[1]-t.dy_cl[0])); */
/* 	if (t.dx_cl[1]!=NOVAL_F&&drcl12>0.5) { */
	if (t.trk.pt>1.0) {
	  // nclu
	  h1d_2p_[0]             [2]->Fill(x);
	  h1d_2p_[t.mod_on.det+1][2]->Fill(x);
	  h1d_2p_[lay]           [2]->Fill(x);
	  // avg charge
	  h1d_2p_[0]             [3]->Fill(x, t.clu.charge);
	  h1d_2p_[t.mod_on.det+1][3]->Fill(x, t.clu.charge);
	  h1d_2p_[lay]           [3]->Fill(x, t.clu.charge);
	  // avg norm charge
	  h1d_2p_[0]             [4]->Fill(x, t.norm_charge);
	  h1d_2p_[t.mod_on.det+1][4]->Fill(x, t.norm_charge);
	  h1d_2p_[lay]           [4]->Fill(x, t.norm_charge);
	  // ncd -> for mpv
	  h2d_2p_[0]             [0]->Fill(x, t.norm_charge);
	  h2d_2p_[t.mod_on.det+1][0]->Fill(x, t.norm_charge);
	  h2d_2p_[lay]           [0]->Fill(x, t.norm_charge);
	  // alpha
	  h1d_2p_[0]             [6]->Fill(x, fabs(t.alpha));
	  h1d_2p_[t.mod_on.det+1][6]->Fill(x, fabs(t.alpha));
	  h1d_2p_[lay]           [6]->Fill(x, fabs(t.alpha));
	  // beta
	  h1d_2p_[0]             [7]->Fill(x, 1.5708-fabs(1.5708-fabs(t.beta)));
	  h1d_2p_[t.mod_on.det+1][7]->Fill(x, 1.5708-fabs(1.5708-fabs(t.beta)));
	  h1d_2p_[lay]           [7]->Fill(x, 1.5708-fabs(1.5708-fabs(t.beta)));
	  // aoi
	  float aoi = atan(  sqrt( 1.0 / (1.0/(tan(t.alpha)*tan(t.alpha)) + 1.0/(tan(t.beta)*tan(t.beta)) ) )  );
	  h1d_2p_[0]             [8]->Fill(x, aoi);
	  h1d_2p_[t.mod_on.det+1][8]->Fill(x, aoi);
	  h1d_2p_[lay]           [8]->Fill(x, aoi);
	  // aoi dist
	  h2d_2p_[0]             [3]->Fill(x, aoi);
	  h2d_2p_[t.mod_on.det+1][3]->Fill(x, aoi);
	  h2d_2p_[lay]           [3]->Fill(x, aoi);
/* 	  if (aoi>0.2&&aoi<0.27) { */
	  if (fabs(t.trk.eta)<0.1) {
	    // avg size
	    h1d_2p_[0]             [9]->Fill(x, t.clu.sizeX);
	    h1d_2p_[t.mod_on.det+1][9]->Fill(x, t.clu.sizeX);
	    h1d_2p_[lay]           [9]->Fill(x, t.clu.sizeX);
	    // ncluperevt
	    h1d_2p_[0]             [10]->Fill(x);
	    h1d_2p_[t.mod_on.det+1][10]->Fill(x);
	    h1d_2p_[lay]           [10]->Fill(x);
	    // npixperevt
	    h1d_2p_[0]             [11]->Fill(x, t.clu.size);
	    h1d_2p_[t.mod_on.det+1][11]->Fill(x, t.clu.size);
	    h1d_2p_[lay]           [11]->Fill(x, t.clu.size);
	    int a = -1;
	    if (t.clu.size>0) { int loop = 0; 
	      while (a==-1&&loop<100) { loop++; a = rand()%t.clu.size; if (t.clu.pix[a][0]<3.0) a=-1; } }
	    for (int i=0; i<t.clu.size; i++) {
	      if (i!=a) {
		// pcd
		h2d_2p_[0]             [1]->Fill(x, t.clu.pix[i][0]);
		h2d_2p_[t.mod_on.det+1][1]->Fill(x, t.clu.pix[i][0]);
		h2d_2p_[lay]           [1]->Fill(x, t.clu.pix[i][0]);
		// gain corrected pcd
		h2d_2p_[0]             [2]->Fill(x, t.clu.pix[i][0]/gc);
		h2d_2p_[t.mod_on.det+1][2]->Fill(x, t.clu.pix[i][0]/gc);
		h2d_2p_[lay]           [2]->Fill(x, t.clu.pix[i][0]/gc);
	      }
	    }
	  }
	}
      }
    }
  }

  void det_hist_def_fill(int p1, bool cut, const TrajMeasurement &t, double x, float gc) {
    if (npostfix_==3) {
      int lay = (t.mod_on.det==0) ? 2+t.mod_on.layer : 7+t.mod_on.disk+(t.mod_on.disk<0);
      if (cut) {
	if (t.validhit==1||t.missing==1) {
	  h1d_3p_[0]             [p1][t.missing]->Fill(x);
	  h1d_3p_[1+t.mod_on.det][p1][t.missing]->Fill(x);
	  h1d_3p_[lay]           [p1][t.missing]->Fill(x);
	  // FPix plots
	  if (t.mod_on.det) {
	    int IO = t.mod_on.panel+t.mod_on.module > 3;
	    h1d_3p_[9+abs(t.mod_on.disk)][p1][t.missing]->Fill(x);
	    h1d_3p_[10+abs(t.mod_on.disk)*2+IO][p1][t.missing]->Fill(x);
	    h1d_3p_[18+(t.mod_on.disk+(t.mod_on.disk<0))*2+IO][p1][t.missing]->Fill(x);
	  }
	}
      }
      if (t.clu.size>0) {
/* 	float drcl12 = sqrt((t.dx_cl[1]-t.dx_cl[0])*(t.dx_cl[1]-t.dx_cl[0]) */
/* 			    +(t.dy_cl[1]-t.dy_cl[0])*(t.dy_cl[1]-t.dy_cl[0])); */
/* 	if (t.dx_cl[1]!=NOVAL_F&&drcl12>0.5) { */
	if (t.trk.pt>1.0) {
	  // nclu
	  h1d_3p_[0]             [p1][2]->Fill(x);
	  h1d_3p_[t.mod_on.det+1][p1][2]->Fill(x);
	  h1d_3p_[lay]           [p1][2]->Fill(x);
	  // avg charge
	  h1d_3p_[0]             [p1][3]->Fill(x, t.clu.charge);
	  h1d_3p_[t.mod_on.det+1][p1][3]->Fill(x, t.clu.charge);
	  h1d_3p_[lay]           [p1][3]->Fill(x, t.clu.charge);
	  // avg norm charge
	  h1d_3p_[0]             [p1][4]->Fill(x, t.norm_charge);
	  h1d_3p_[t.mod_on.det+1][p1][4]->Fill(x, t.norm_charge);
	  h1d_3p_[lay]           [p1][4]->Fill(x, t.norm_charge);
	  // ncd -> for mpv
	  h2d_3p_[0]             [p1][0]->Fill(x, t.norm_charge);
	  h2d_3p_[t.mod_on.det+1][p1][0]->Fill(x, t.norm_charge);
	  h2d_3p_[lay]           [p1][0]->Fill(x, t.norm_charge);
	  // alpha
	  h1d_3p_[0]             [p1][6]->Fill(x, fabs(t.alpha));
	  h1d_3p_[t.mod_on.det+1][p1][6]->Fill(x, fabs(t.alpha));
	  h1d_3p_[lay]           [p1][6]->Fill(x, fabs(t.alpha));
	  // beta
	  h1d_3p_[0]             [p1][7]->Fill(x, 1.5708-fabs(1.5708-fabs(t.beta)));
	  h1d_3p_[t.mod_on.det+1][p1][7]->Fill(x, 1.5708-fabs(1.5708-fabs(t.beta)));
	  h1d_3p_[lay]           [p1][7]->Fill(x, 1.5708-fabs(1.5708-fabs(t.beta)));
	  // aoi
	  float aoi = atan(  sqrt( 1.0 / (1.0/(tan(t.alpha)*tan(t.alpha)) + 1.0/(tan(t.beta)*tan(t.beta)) ) )  );
	  h1d_3p_[0]             [p1][8]->Fill(x, aoi);
	  h1d_3p_[t.mod_on.det+1][p1][8]->Fill(x, aoi);
	  h1d_3p_[lay]           [p1][8]->Fill(x, aoi);
	  // aoi dist
	  h2d_3p_[0]             [p1][3]->Fill(x, aoi);
	  h2d_3p_[t.mod_on.det+1][p1][3]->Fill(x, aoi);
	  h2d_3p_[lay]           [p1][3]->Fill(x, aoi);
/* 	  if (aoi>0.2&&aoi<0.27) { */
	  if (fabs(t.trk.eta)<0.1) {
	    // avg size
	    h1d_3p_[0]             [p1][9]->Fill(x, t.clu.sizeX);
	    h1d_3p_[t.mod_on.det+1][p1][9]->Fill(x, t.clu.sizeX);
	    h1d_3p_[lay]           [p1][9]->Fill(x, t.clu.sizeX);
	    // ncluperevt
	    h1d_3p_[0]             [p1][10]->Fill(x);
	    h1d_3p_[t.mod_on.det+1][p1][10]->Fill(x);
	    h1d_3p_[lay]           [p1][10]->Fill(x);
	    // npixperevt
	    h1d_3p_[0]             [p1][11]->Fill(x, t.clu.size);
	    h1d_3p_[t.mod_on.det+1][p1][11]->Fill(x, t.clu.size);
	    h1d_3p_[lay]           [p1][11]->Fill(x, t.clu.size);
	    // avgpixch
	    int a = -1;
	    if (t.clu.size>0) { int loop = 0; 
	      while (a==-1&&loop<100) {loop++; a = rand()%t.clu.size; if (t.clu.pix[a][0]<3.0) a=-1; } }
	    for (int i=0; i<t.clu.size; i++) {
	      if (i!=a) {
		// pcd
		h2d_3p_[0]             [p1][1]->Fill(x, t.clu.pix[i][0]);
		h2d_3p_[t.mod_on.det+1][p1][1]->Fill(x, t.clu.pix[i][0]);
		h2d_3p_[lay]           [p1][1]->Fill(x, t.clu.pix[i][0]);
		// gain corrected pcd
		h2d_3p_[0]             [p1][2]->Fill(x, t.clu.pix[i][0]/gc);
		h2d_3p_[t.mod_on.det+1][p1][2]->Fill(x, t.clu.pix[i][0]/gc);
		h2d_3p_[lay]           [p1][2]->Fill(x, t.clu.pix[i][0]/gc);
	      }
	    }
	  }
	}
      }
    }
  }

  // Labeling functions

  void hist_setbincontent(int p1, int p2, int binx, double content) {
    h1d_2p_[p1][p2]->SetBinContent(binx, content);
  }

  // Multidraw
  // New
  void set_stat_(TH1D* h, Color_t col, int i) {
    if (i<7) {
      gPad->Update();
      TPaveStats* stats = (TPaveStats*)h->FindObject("stats");
      stats->SetLineColor(col);
      stats->SetTextColor(col);
      stats->SetX1NDC(0.74);
      stats->SetX2NDC(0.94);
      stats->SetY1NDC(0.8-i*0.11);
      stats->SetY2NDC(0.9-i*0.11);
    }
  }

  std::vector<double> bincoordx_;
  std::vector<const char*> binlabels_;
  
  TGraphAsymmErrors* asym_(TH1D* eff, TH1D* den) {
    bincoordx_.clear();
    binlabels_.clear();
    //TGraphAsymmErrors* tgae = new TGraphAsymmErrors(eff);
    int n = 0;
    for (Int_t i=0; i<eff->GetNbinsX(); ++i) if (eff->GetBinContent(i+1)>0) n++;
    TGraphAsymmErrors* tgae = new TGraphAsymmErrors(n);
    n = 0;
    for (Int_t i=0; i<eff->GetNbinsX(); ++i) {
      double y = eff->GetBinContent(i+1);
      double x = eff->GetBinCenter(i+1);
      if (y>0) {
        n++;
        tgae->SetPoint(n-1,x,y);
      }
    }
    // Calculate the asymmetric wilson score interval
    double z = 1; // 1 Sigma confidence
    for (Int_t bin=1; bin<=eff->GetNbinsX(); ++bin) {
      double p = eff->GetBinContent(bin);
      double n = den->GetBinContent(bin);
      double cen = (p+(z*z/(2*n))) / (1.0 + (z*z/n));
      double halfwidth = z*sqrt( p*(1.0-p)/n + (z*z/(4*n*n)) ) / (1.0 + (z*z/n));
      tgae->SetPointEYhigh(bin-1,halfwidth-p+cen);
      tgae->SetPointEYlow (bin-1,halfwidth+p-cen);
    }
    // Set same ranges/settings etc
    double xlow = eff->GetXaxis()->GetBinLowEdge(eff->GetXaxis()->GetFirst());
    double xup  = eff->GetXaxis()->GetBinUpEdge(eff->GetXaxis()->GetLast());
    double ylow = eff->GetMinimum();
    double yup  = eff->GetMaximum();
    tgae->GetXaxis()->SetRangeUser(xlow,xup);
    tgae->GetYaxis()->SetRangeUser(ylow,yup);
    tgae->SetTitle(eff->GetTitle());
    tgae->GetXaxis()->SetTitle(eff->GetXaxis()->GetTitle());
    tgae->GetYaxis()->SetTitle(eff->GetYaxis()->GetTitle());
    tgae->GetXaxis()->SetTitleSize(eff->GetXaxis()->GetTitleSize());
    tgae->GetYaxis()->SetTitleSize(eff->GetYaxis()->GetTitleSize());
    tgae->GetXaxis()->SetTitleOffset(eff->GetXaxis()->GetTitleOffset());
    tgae->GetYaxis()->SetTitleOffset(eff->GetYaxis()->GetTitleOffset());
    tgae->GetXaxis()->SetTitleFont(eff->GetXaxis()->GetTitleFont());
    tgae->GetYaxis()->SetTitleFont(eff->GetYaxis()->GetTitleFont());
    tgae->GetXaxis()->SetNdivisions(eff->GetNdivisions("X"));
    tgae->GetYaxis()->SetNdivisions(eff->GetNdivisions("Y"));
    tgae->GetYaxis()->SetDecimals(1);
    tgae->SetMarkerStyle(eff->GetMarkerStyle());
    tgae->SetMarkerColor(eff->GetMarkerColor());
    tgae->SetMarkerSize(eff->GetMarkerSize());
    // If Bin Labels exist remove current X axis labels and draw similar bin labels but with TLatex
    for (Int_t bin=1; bin<=eff->GetNbinsX(); ++bin) {
      if (std::string(eff->GetXaxis()->GetBinLabel(bin)).size()) {
	bincoordx_.push_back(eff->GetXaxis()->GetBinCenter(bin));
	binlabels_.push_back(eff->GetXaxis()->GetBinLabel(bin));
      }
    }
    return tgae;
  }

  void asym_labels_(TH1D *orig, TGraphAsymmErrors* tgae, int hor_vert_up_down = 1) {
    int angle = hor_vert_up_down==0 ? 0 : hor_vert_up_down==1 ? 90 : hor_vert_up_down==2 ? 20 : -20 ;
    int align = hor_vert_up_down==0 ? 23 : hor_vert_up_down==1 ? 32 : hor_vert_up_down==2 ? 33 : 13;
    if (binlabels_.size()>0) tgae->GetXaxis()->SetLabelColor(0);
    double labelsize = orig->GetXaxis()->GetLabelSize();
    double offset = (gPad->GetY2()-gPad->GetY1()) * orig->GetXaxis()->GetLabelOffset();
    for (size_t i=0; i<binlabels_.size(); ++i) {
      TLatex *lat = new TLatex(bincoordx_[i], orig->GetMinimum()-offset, binlabels_[i]);
      lat->SetTextAlign(align);
      lat->SetTextAngle(angle);
      lat->SetTextFont(orig->GetXaxis()->GetLabelFont());
      lat->SetTextSize(labelsize);
      lat->Draw();
    }
  }
  
  TLegend* multidraw_(std::vector<TH1D*>& hvec, std::vector<TH1D*>& denvec, std::vector<int>& vec, std::string& opt, 
		      std::vector<std::string> &p, std::string& colz,
		      std::string& title, float x1, float x2, float y1, float y2) {
    // Normalize by the number of entries and set range according to the highest histogram
    if (opt.find("NORM")!=std::string::npos) {
      opt.erase(opt.find("NORM"),4);
      float max = 1.0;
      for (size_t i=0; i<hvec.size(); i++) {
	TH1D *h = hvec[i];
	h->Scale(1/h->GetEntries());
	float m = h->GetMaximum();
	if (m>max) max = m;
      }
      hvec[0]->GetYaxis()->SetRangeUser(0,max*1.1);
    }
    bool asym = false;
    if (opt.find("AsymmErr")!=std::string::npos) {
      opt.erase(opt.find("AsymmErr"),8);
      asym = true;
    }
    // Draw Stat boxes
    Int_t stat = 0;
    if (opt.find("STAT")!=std::string::npos) {
      opt.erase(opt.find("STAT"),4);
      stat = 1110;
    }
    gStyle->SetOptStat(stat);
    // Draw multiple histograms, set their marker/line color/style
    // Then Draw legend for all histo with titles from a postfix
    std::vector<int> col = string_to_vector_(colz);
    std::string same = (stat ? "SAMES" : "SAME") + opt;
    TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
    for (size_t i=0; i<hvec.size(); i++) {
      TH1D *h = hvec[i];
      Color_t c = (Color_t)col[i];
      Int_t m = marker_[i];
      set_color_(h,c,opt);
      set_marker_(h,m,opt);
      if (stat) h->SetStats(1);
      if (asym) {
	TGraphAsymmErrors* tgae = asym_(h,denvec[i]);
	std::string drawopt = opt;
	if (!i) drawopt.append("A");
	tgae->Draw(drawopt.c_str());
	gPad->Update();
	// Draw BinLabels if they exist
	if (!i) asym_labels_(h,tgae,1); // vertical up labels
	//if (binlabels_.size()>0) tgae->GetXaxis()->SetLabelColor(0);
	//double labelsize = h->GetXaxis()->GetLabelSize();
	//double offset = (gPad->GetY2()-gPad->GetY1()) * h->GetXaxis()->GetLabelOffset();
	//if (!i) for (size_t i=0; i<binlabels_.size(); ++i) {
	//  TLatex *lat = new TLatex(bincoordx_[i], h->GetMinimum()-offset, binlabels_[i]);
	//  lat->SetTextAlign(32);
	//  lat->SetTextAngle(90);
	//  lat->SetTextFont(h->GetXaxis()->GetLabelFont());
	//  lat->SetTextSize(labelsize);
	//  lat->Draw();
	//}
      } else {
	h->Draw(i ? same.c_str() : opt.c_str());
	if (stat) set_stat_(h,c,i);
      }
      leg->AddEntry(h, p[vec[i]].c_str(), legmark_(opt));
    }
    leg->SetFillColor(0);
    leg->SetFillStyle(0);
    leg->SetBorderSize(0);
    leg->SetTextSize(0.04);
    leg->Draw("SAME");
    return leg;
  }

  // 1 Postfix
  TLegend* multidraw_with_legend(std::string val, std::string opt,
				 std::vector<std::string> &p, std::string colz,
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t i=0; i<vec.size(); i++) hvec.push_back(h1d_1p_[vec[i]]);
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }

  // 2 Postfix
  TLegend* multidraw_with_legend(int i, std::string val, std::string opt,
				 std::vector<std::string> &p, std::string colz,
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t j=0; j<vec.size(); j++) hvec.push_back(h1d_2p_[i][vec[j]]);
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }

  TLegend* multidraw_with_legend(std::string val, int j, std::string opt,
				 std::vector<std::string> &p, std::string colz, 
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t i=0; i<vec.size(); i++) {
      hvec.push_back(h1d_2p_[vec[i]][j]);
      if (h1d_2p_[vec[i]].size()>1) denvec.push_back(h1d_2p_[vec[i]][1]);
    }
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }


  // 3 Postfix
  TLegend* multidraw_with_legend(std::string val, int j, int k, std::string opt,
				 std::vector<std::string> &p, std::string colz, 
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t i=0; i<vec.size(); i++) {
      hvec.push_back(h1d_3p_[vec[i]][j][k]);
      if (h1d_3p_[vec[i]][j].size()>1) denvec.push_back(h1d_3p_[vec[i]][j][1]);
    }
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }

  TLegend* multidraw_with_legend(int i, std::string val, int k, std::string opt,
				 std::vector<std::string> &p, std::string colz, 
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t j=0; j<vec.size(); j++) {
      hvec.push_back(h1d_3p_[i][vec[j]][k]);
      if (h1d_3p_[i][vec[j]].size()>1) denvec.push_back(h1d_3p_[i][vec[j]][1]);
    }
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }

  TLegend* multidraw_with_legend(int i, int j, std::string val, std::string opt,
				 std::vector<std::string> &p, std::string colz, 
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t k=0; k<vec.size(); k++) hvec.push_back(h1d_3p_[i][j][vec[j]]);
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }
  
  // 4 Postfix
  TLegend* multidraw_with_legend(int i, std::string val, int k, int l, std::string opt,
				 std::vector<std::string> &p, std::string colz, 
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t j=0; j<vec.size(); j++) {
      hvec.push_back(h1d_4p_[i][vec[j]][k][l]);
      if (h1d_4p_[i][vec[j]][k].size()>1) denvec.push_back(h1d_4p_[i][vec[j]][k][1]);
    }
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }

  TLegend* multidraw_with_legend(int i, int j, std::string val, int l, std::string opt,
				 std::vector<std::string> &p, std::string colz, 
				 std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    std::vector<int> vec = string_to_vector_(val);
    std::vector<TH1D*> hvec;
    std::vector<TH1D*> denvec;
    for (size_t k=0; k<vec.size(); k++) {
      hvec.push_back(h1d_4p_[i][j][vec[k]][l]);
      if (h1d_4p_[i][j][vec[k]].size()>1) denvec.push_back(h1d_4p_[i][j][vec[k]][1]);
    }
    return multidraw_(hvec, denvec, vec, opt, p, colz, title, x1,x2, y1,y2);
  }
  
  
  // Old
  // 1 postfix
  TLegend* multidraw_with_legend_(std::string val, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title = "", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==1) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t i=0; i<vec.size(); i++) {
	  TH1D *h = h1d_1p_[vec[i]];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_1p_[vec[0]]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t i=0; i<vec.size(); i++) {
	TH1D *h = h1d_1p_[vec[i]];
	Color_t c = (Color_t)col[i];
	Int_t m = marker_[i];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (i==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[i]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  // 2 postfix
  TLegend* multidraw_with_legend_(std::string val, int j, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title = "", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==2) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t i=0; i<vec.size(); i++) {
	  TH1D *h = h1d_2p_[vec[i]][j];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_2p_[vec[0]][j]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t i=0; i<vec.size(); i++) {
	TH1D *h = h1d_2p_[vec[i]][j];
	Color_t c = (Color_t)col[i];
	Int_t m = marker_[i];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (i==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[i]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  TLegend* multidraw_with_legend_(int i, std::string val, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title = "", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==2) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t j=0; j<vec.size(); j++) {
	  TH1D *h = h1d_2p_[i][vec[j]];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_2p_[i][vec[0]]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t j=0; j<vec.size(); j++) {
	TH1D *h = h1d_2p_[i][vec[j]];
	Color_t c = (Color_t)col[j];
	Int_t m = marker_[j];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (j==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[j]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  // 3 postfix
  TLegend* multidraw_with_legend_(std::string val, int j, int k, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==3) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t i=0; i<vec.size(); i++) {
	  TH1D *h = h1d_3p_[vec[i]][j][k];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_3p_[vec[0]][j][k]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t i=0; i<vec.size(); i++) {
	TH1D *h = h1d_3p_[vec[i]][j][k];
	Color_t c = (Color_t)col[i];
	Int_t m = marker_[i];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (i==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[i]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  TLegend* multidraw_with_legend_(int i, std::string val, int k, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==3) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t j=0; j<vec.size(); j++) {
	  TH1D *h = h1d_3p_[i][vec[j]][k];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_3p_[i][vec[0]][k]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t j=0; j<vec.size(); j++) {
	TH1D *h = h1d_3p_[i][vec[j]][k];
	Int_t c = col[j];
	Int_t m = marker_[j];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (j==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[j]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  TLegend* multidraw_with_legend_(int i, int j, std::string val, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==3) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t k=0; k<vec.size(); k++) {
	  TH1D *h = h1d_3p_[i][j][vec[k]];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_3p_[i][j][vec[0]]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t k=0; k<vec.size(); k++) {
	TH1D *h = h1d_3p_[i][j][vec[k]];
	Int_t c = col[k];
	Int_t m = marker_[k];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (k==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[k]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  // 4 postfix
  TLegend* multidraw_with_legend_(int i, std::string val, int k, int l, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==4) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t j=0; j<vec.size(); j++) {
	  TH1D *h = h1d_4p_[i][vec[j]][k][l];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_4p_[i][vec[0]][k][l]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t j=0; j<vec.size(); j++) {
	TH1D *h = h1d_4p_[i][vec[j]][k][l];
	Int_t c = col[j];
	Int_t m = marker_[j];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (j==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[j]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  TLegend* multidraw_with_legend_(int i, int j, std::string val, int l, std::string opt,
				  std::vector<std::string> &p, std::string colz, 
				  std::string title="", float x1=0.6, float x2=0.8, float y1=0.15, float y2=0.35) {
    if (dimensions_==1&&npostfix_==4) {
      std::vector<int> vec = string_to_vector_(val);
      // Normalize by the number of entries and set range according to the highest histogram
      if (opt.find("NORM")!=std::string::npos) {
	opt.erase(opt.find("NORM"),4);
	float max = 0.0;
	for (size_t k=0; k<vec.size(); k++) {
	  TH1D *h = h1d_4p_[i][j][vec[k]][l];
	  h->Scale(1/h->GetEntries());
	  float m = h->GetMaximum();
	  if (m>max) max = m;
	}
	h1d_4p_[i][j][vec[0]][l]->GetYaxis()->SetRangeUser(0,max*1.1);
      }
      // Draw multiple histograms, set their marker/line color/style
      // Then Draw legend for all histo with titles from a postfix
      std::vector<int> col = string_to_vector_(colz);
      std::string same = "SAME" + opt;
      TLegend *leg = new TLegend(x1,y1,x2,y2,title.c_str());
      for (size_t k=0; k<vec.size(); k++) {
	TH1D *h = h1d_4p_[i][j][vec[k]][l];
	Int_t c = col[k];
	Int_t m = marker_[k];
	set_color_(h,c,opt);
	set_marker_(h,m,opt);
	if (k==0) h->Draw(opt.c_str());
	else h->Draw(same.c_str());
	leg->AddEntry(h, p[vec[k]].c_str(), legmark_(opt));
      }
      leg->SetFillColor(0);
      leg->SetFillStyle(0);
      leg->SetBorderSize(0);
      leg->SetTextSize(0.04);
      leg->Draw("SAME");
      return leg;
    } else return NULL;
  }

  // Styling: fonts, axis titles and offset
  // 1D dress
  void dress_all_1d(std::string xtitle="", std::string ytitle="",
		    Float_t xoffset=0.9, Float_t yoffset=1.4, 
		    Style_t fontstyle=42, Float_t titlesize=0.06, Bool_t dec=kTRUE) {
    if (dimensions_==1) {
      if (npostfix_==1) for (size_t i=0; i<h1d_1p_.size(); i++) 
	setstyle_(h1d_1p_[i], xtitle, (ytitle.size()) ? ytitle : title_(i), xoffset, yoffset, fontstyle, titlesize, dec);
      else if (npostfix_==2) for (size_t i=0; i<h1d_2p_.size(); i++) 
	for (size_t j=0; j<h1d_2p_[i].size(); j++)
	  setstyle_(h1d_2p_[i][j], xtitle, (ytitle.size()) ? ytitle : title_(j), xoffset, yoffset, fontstyle, titlesize, dec);
      else if (npostfix_==3) for (size_t i=0; i<h1d_3p_.size(); i++) 
	for (size_t j=0; j<h1d_3p_[i].size(); j++) for (size_t k=0; k<h1d_3p_[i][j].size(); k++)
	  setstyle_(h1d_3p_[i][j][k], xtitle, (ytitle.size()) ? ytitle : title_(k), xoffset, yoffset, fontstyle, titlesize, dec);
      else if (npostfix_==4) for (size_t i=0; i<h1d_4p_.size(); i++) for (size_t j=0; j<h1d_4p_[i].size(); j++) 
	for (size_t k=0; k<h1d_4p_[i][j].size(); k++) for (size_t l=0; l<h1d_4p_[i][j][k].size(); l++)
	  setstyle_(h1d_4p_[i][j][k][l], xtitle, (ytitle.size()) ? ytitle : title_(l), xoffset, yoffset, fontstyle, titlesize, dec);
    }
  }

  // 2D
  void dress_all_2d(std::string xtitle="", std::string ytitle="", std::string ztitle="",
		 Float_t xoffset=0.9, Float_t yoffset=1.4, Float_t zoffset=1.0,
		 Style_t fontstyle=42, Float_t titlesize=0.06, Bool_t dec=kTRUE) {
    if (dimensions_==2) {
      if (npostfix_==1) for (size_t i=0; i<h2d_1p_.size(); i++) 
	setstyle_(h2d_1p_[i], xtitle, ytitle, (ztitle.size()) ? ztitle : title_(i), xoffset, yoffset, zoffset, fontstyle, titlesize, dec);
      else if (npostfix_==2) for (size_t i=0; i<h2d_2p_.size(); i++) 
	for (size_t j=0; j<h2d_2p_[i].size(); j++)
	  setstyle_(h2d_2p_[i][j], xtitle, ytitle, (ztitle.size()) ? ztitle : title_(j), xoffset, yoffset, zoffset, fontstyle, titlesize, dec);
      else if (npostfix_==3) for (size_t i=0; i<h2d_3p_.size(); i++) 
	for (size_t j=0; j<h2d_3p_[i].size(); j++) for (size_t k=0; k<h2d_3p_[i][j].size(); k++)
	  setstyle_(h2d_3p_[i][j][k], xtitle, ytitle, (ztitle.size()) ? ztitle : title_(k), xoffset, yoffset, zoffset, fontstyle, titlesize, dec);
      else if (npostfix_==4) for (size_t i=0; i<h2d_4p_.size(); i++) for (size_t j=0; j<h2d_4p_[i].size(); j++) 
	for (size_t k=0; k<h2d_4p_[i][j].size(); k++) for (size_t l=0; l<h2d_4p_[i][j][k].size(); l++)
	  setstyle_(h2d_4p_[i][j][k][l], xtitle, ytitle, (ztitle.size()) ? ztitle : title_(l), xoffset, yoffset, zoffset, fontstyle, titlesize, dec);
    }
  }


  // Calculate cumulative, efficiency, average cluster size, error and write to output file
  void finish() {
    if (cumul_!=0) calc_cumulative_();
    if (eff_==1) { calc_eff_(); }
    if (err_!=0) { calc_error_(); }
    if (clusize_==1) calc_clusize_();
    if (def_!=0) calc_def_();
    write();
  }

  void calc() {
    if (cumul_!=0) calc_cumulative_();
    if (eff_==1) { calc_eff_(); calc_error_(); }
    if (clusize_==1) calc_clusize_();
  }

  void write() {
    if (npostfix_==1) {
      for (size_t l=0; l<h1d_1p_.size(); l++) h1d_1p_[l]->Write();
      for (size_t l=0; l<h2d_1p_.size(); l++) h2d_1p_[l]->Write();
      for (size_t l=0; l<h3d_1p_.size(); l++) h3d_1p_[l]->Write();
    } else if (npostfix_==2) {
      for (size_t l=0; l<h1d_2p_.size(); l++) for (size_t m=0; m<h1d_2p_[l].size(); m++) h1d_2p_[l][m]->Write();
      for (size_t l=0; l<h2d_2p_.size(); l++) for (size_t m=0; m<h2d_2p_[l].size(); m++) h2d_2p_[l][m]->Write();
      for (size_t l=0; l<h3d_2p_.size(); l++) for (size_t m=0; m<h3d_2p_[l].size(); m++) h3d_2p_[l][m]->Write();
    } else if (npostfix_==3) {
      for (size_t l=0; l<h1d_3p_.size(); l++) for (size_t m=0; m<h1d_3p_[l].size(); m++) {
	for (size_t n=0; n<h1d_3p_[l][m].size(); n++) h1d_3p_[l][m][n]->Write();
	if (def_==1) { for (size_t n=0; n<h2d_3p_[l][m].size(); n++) h2d_3p_[l][m][n]->Write(); h1d_1p_[0]->Write(); } }
      for (size_t l=0; l<h2d_3p_.size(); l++) for (size_t m=0; m<h2d_3p_[l].size(); m++) 
	for (size_t n=0; n<h2d_3p_[l][m].size(); n++) h2d_3p_[l][m][n]->Write();
      for (size_t l=0; l<h3d_3p_.size(); l++) for (size_t m=0; m<h3d_3p_[l].size(); m++) 
	for (size_t n=0; n<h3d_3p_[l][m].size(); n++) h3d_3p_[l][m][n]->Write();
    } else if (npostfix_==4) {
      for (size_t l=0; l<h1d_4p_.size(); l++) for (size_t m=0; m<h1d_4p_[l].size(); m++) 
	for (size_t n=0; n<h1d_4p_[l][m].size(); n++) for (size_t o=0; o<h1d_4p_[l][m][n].size(); o++) h1d_4p_[l][m][n][o]->Write();
      for (size_t l=0; l<h2d_4p_.size(); l++) for (size_t m=0; m<h2d_4p_[l].size(); m++) 
	for (size_t n=0; n<h2d_4p_[l][m].size(); n++) for (size_t o=0; o<h2d_4p_[l][m][n].size(); o++) h2d_4p_[l][m][n][o]->Write();
      for (size_t l=0; l<h3d_4p_.size(); l++) for (size_t m=0; m<h3d_4p_[l].size(); m++) 
	for (size_t n=0; n<h3d_4p_[l][m].size(); n++) for (size_t o=0; o<h3d_4p_[l][m][n].size(); o++) h3d_4p_[l][m][n][o]->Write();
    }
  }

  // Accessors
  std::string& histoname() { return histoname_; }
  int npostfix() { return npostfix_; }
  int dimensions() { return dimensions_; }
  int eff() { return eff_; }
  int def() { return def_; }
  int clusize() { return clusize_; }
  int cumul() { return cumul_; }

  std::vector<TH1D*>& h1d_1p() { return h1d_1p_; }
  std::vector<TH2D*>& h2d_1p() { return h2d_1p_; }
  std::vector<TH3D*>& h3d_1p() { return h3d_1p_; }
  std::vector<std::vector<TH1D*> >& h1d_2p() { return h1d_2p_; }
  std::vector<std::vector<TH2D*> >& h2d_2p() { return h2d_2p_; }
  std::vector<std::vector<TH3D*> >& h3d_2p() { return h3d_2p_; }
  std::vector<std::vector<std::vector<TH1D*> > >& h1d_3p() { return h1d_3p_; }
  std::vector<std::vector<std::vector<TH2D*> > >& h2d_3p() { return h2d_3p_; }
  std::vector<std::vector<std::vector<TH3D*> > >& h3d_3p() { return h3d_3p_; }
  std::vector<std::vector<std::vector<std::vector<TH1D*> > > >& h1d_4p() { return h1d_4p_; }
  std::vector<std::vector<std::vector<std::vector<TH2D*> > > >& h2d_4p() { return h2d_4p_; }
  std::vector<std::vector<std::vector<std::vector<TH3D*> > > >& h3d_4p() { return h3d_4p_; }

  TH1D* h1d(int i) { return h1d_1p_[i]; }
  TH2D* h2d(int i) { return h2d_1p_[i]; }
  TH3D* h3d(int i) { return h3d_1p_[i]; }
  TH1D* h1d(int i, int j) { return h1d_2p_[i][j]; }
  TH2D* h2d(int i, int j) { return h2d_2p_[i][j]; }
  TH3D* h3d(int i, int j) { return h3d_2p_[i][j]; }
  TH1D* h1d(int i, int j, int k) { return h1d_3p_[i][j][k]; }
  TH2D* h2d(int i, int j, int k) { return h2d_3p_[i][j][k]; }
  TH3D* h3d(int i, int j, int k) { return h3d_3p_[i][j][k]; }
  TH1D* h1d(int i, int j, int k, int l) { return h1d_4p_[i][j][k][l]; }
  TH2D* h2d(int i, int j, int k, int l) { return h2d_4p_[i][j][k][l]; }
  TH3D* h3d(int i, int j, int k, int l) { return h3d_4p_[i][j][k][l]; }

  void error(TH1D* eff, TH1D* dist) { error_(eff, dist); }

  void rebin_eff(TH1D* eff, TH1D* dist, int n=2) {
    eff->Multiply(dist);
    dist->Add(eff,-1);
    eff->Rebin(n);
    dist->Rebin(n);
    dist->Add(eff);
    eff->Divide(dist);
    error(eff,dist);
  }

  TGraphAsymmErrors* asym1d(int i) { 
    return asym_(h1d_2p_[i][0],h1d_2p_[i][1]);
  }
  
  void asym1d_draw(int i, const char* opt = "AP", int hor_vert_up_down = 0) { 
    TGraphAsymmErrors* tgae = asym_(h1d_2p_[i][0],h1d_2p_[i][1]);
    tgae->Draw(opt);
    asym_labels_(h1d_2p_[i][0],tgae, hor_vert_up_down);
  }

};
