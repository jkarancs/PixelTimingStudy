class Histograms {
  
 public:
  
  Histograms(const char* histoname, int npostfix, int dimensions) {
    histoname_ = histoname;
    npostfix_ = npostfix;
    dimensions_ = dimensions;
  }
  ~Histograms() { }
  
 private:
  
  std::string histoname_;
  int npostfix_;
  int dimensions_;

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

 // Modifiers
 public:

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
      if (opt1==1) h1d_2p_.push_back(std::vector<TH1D*>());
      if (opt2==1) {
	for (int j=low2-1; j<high2;j++) {
	  std::string name=histoname_;
	  name+=postfix1[i];
	  name+=postfix2[j];
	  TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
	  h1d_2p_[i-skip1].push_back(h);
	}
      }
    }
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
      if (opt1==1) h1d_3p_.push_back(std::vector<std::vector<TH1D*> >());
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) h1d_3p_[i-skip1].push_back(std::vector<TH1D*>());
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
      }
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
      if (opt1==1) h1d_4p_.push_back(std::vector<std::vector<std::vector<TH1D*> > >());
      for (int j=low2-1; j<high2;j++) {
	if (opt2==1) h1d_4p_[i-skip1].push_back(std::vector<std::vector<TH1D*> >());
	for (int k=low3-1; k<high3;k++) {
	  if (opt3==1) h1d_4p_[i-skip1][j-skip2].push_back(std::vector<TH1D*>());
	  for (int l=low4-1; l<high4;l++) {
	    if (opt4==1) { 
	      std::string name=histoname_;
	      name+=postfix1[i];
	      name+=postfix2[j];
	      name+=postfix3[k];
	      name+=postfix4[l];
	      TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
	      h1d_4p_[i-skip1][j-skip2][k-skip3].push_back(h);
	    }
	  }
	}
      }
    }
  }

  // Fill functions
  
  void eff_increasebin(const TrajMeasurement &trajmeas, int binx, int biny) {
    if (npostfix_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h2d_1p_[trajmeas.missing]->SetBinContent(binx, biny, h2d_1p_[trajmeas.missing]->GetBinContent(binx,biny)+1);
    }
  }

  void eff_fill(const TrajMeasurement &trajmeas, double fillwith_x) {
    if (npostfix_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	  h1d_1p_[trajmeas.missing]->Fill(fillwith_x);
    }
  }

  void eff_fill(const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y) {
    if (npostfix_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h2d_1p_[trajmeas.missing]->Fill(fillwith_x, fillwith_y);
    }
  }

  void eff_fill(const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y, double fillwith_z) {
    if (npostfix_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h3d_1p_[trajmeas.missing]->Fill(fillwith_x, fillwith_y, fillwith_z);
    }
  }

  // Det eff
  // 1D
  void det_eff_fill(const TrajMeasurement &trajmeas, double fillwith_x) {
    if (npostfix_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1) {
	h1d_2p_[0][trajmeas.missing]->Fill(fillwith_x);
	h1d_2p_[1+trajmeas.mod_on.det][trajmeas.missing]->Fill(fillwith_x);
	h1d_2p_[(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)]
	  [trajmeas.missing]->Fill(fillwith_x);
      }
    }
  }

  // 2D
  void det_eff_fill(const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y) {
    if (npostfix_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1) {
	h2d_2p_[0][trajmeas.missing]->Fill(fillwith_x, fillwith_y);
	h2d_2p_[1+trajmeas.mod_on.det][trajmeas.missing]->Fill(fillwith_x, fillwith_y);
	h2d_2p_[(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)]
	  [trajmeas.missing]->Fill(fillwith_x, fillwith_y);
      }
    }
  }

  // 3D
  void det_eff_fill(const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y, double fillwith_z) {
    if (npostfix_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1) {
	h3d_2p_[0][trajmeas.missing]->Fill(fillwith_x, fillwith_y, fillwith_z);
	h3d_2p_[1+trajmeas.mod_on.det][trajmeas.missing]->Fill(fillwith_x, fillwith_y, fillwith_z);
	h3d_2p_[(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)]
	  [trajmeas.missing]->Fill(fillwith_x, fillwith_y, fillwith_z);
      }
    }
  }

  // Histogram [p1][p2][missing] filled with x (,y , z)
  // 2 postfix
  void hist_eff_fill(int p1, const TrajMeasurement &trajmeas, double fillwith_x) {
    if (npostfix_==2&&dimensions_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h1d_2p_[p1][trajmeas.missing]->Fill(fillwith_x);
    }
  }

  void hist_eff_fill(int p1, const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y) {
    if (npostfix_==2&&dimensions_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h2d_2p_[p1][trajmeas.missing]->Fill(fillwith_x, fillwith_y);
    }
  }

  // 3 postfix
  void hist_eff_fill(int p1, int p2, const TrajMeasurement &trajmeas, double fillwith_x) {
    if (npostfix_==3&&dimensions_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h1d_3p_[p1][p2][trajmeas.missing]->Fill(fillwith_x);
    }
  }

  void hist_eff_fill(int p1, int p2, const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y) {
    if (npostfix_==3&&dimensions_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h2d_3p_[p1][p2][trajmeas.missing]->Fill(fillwith_x, fillwith_y);
    }
  }

  void hist_det_eff_fill(int p1, const TrajMeasurement &trajmeas, double fillwith_x) {
    if (npostfix_==3) {
      if (trajmeas.validhit==1||trajmeas.missing==1) {
	h1d_3p_[p1][0][trajmeas.missing]->Fill(fillwith_x);
	h1d_3p_[p1][1+trajmeas.mod_on.det][trajmeas.missing]->Fill(fillwith_x);
	h1d_3p_[p1][(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)]
	  [trajmeas.missing]->Fill(fillwith_x);
      }
    }
  }

  // 4 postfix
  void hist_eff_fill(int p1, int p2, int p3, const TrajMeasurement &trajmeas, double fillwith_x) {
    if (npostfix_==4&&dimensions_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h1d_4p_[p1][p2][p3][trajmeas.missing]->Fill(fillwith_x);
    }
  }

  // increase bin by 1
  // 2 postfix
  void hist_eff_increasebin(int p1, const TrajMeasurement &trajmeas, int binx) {
    if (npostfix_==2&&dimensions_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h1d_2p_[p1][trajmeas.missing]->SetBinContent(binx, h1d_2p_[p1][trajmeas.missing]->GetBinContent(binx)+1);
    }
  }

  void hist_eff_increasebin(int p1, const TrajMeasurement &trajmeas, int binx, int biny) {
    if (npostfix_==2&&dimensions_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h2d_2p_[p1][trajmeas.missing]->SetBinContent(binx, biny, h2d_2p_[p1][trajmeas.missing]->GetBinContent(binx,biny)+1);
    }
  }

  void hist_setbincontent(int p1, int p2, int binx, double content) {
    h1d_2p_[p1][p2]->SetBinContent(binx, content);
  }

  void hist_setbinlabelx(int p1, int p2, int binx, const char* label) {
    h1d_2p_[p1][p2]->GetXaxis()->SetBinLabel(binx, label);
  }

  // 3 postfix
  void hist_eff_increasebin(int p1, int p2, const TrajMeasurement &trajmeas, int binx) {
    if (npostfix_==2&&dimensions_==1) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h1d_3p_[p1][p2][trajmeas.missing]->SetBinContent(binx, h1d_3p_[p1][p2][trajmeas.missing]->GetBinContent(binx)+1);
    }
  }

  void hist_eff_increasebin(int p1, int p2, const TrajMeasurement &trajmeas, int binx, int biny) {
    if (npostfix_==2&&dimensions_==2) {
      if (trajmeas.validhit==1||trajmeas.missing==1)
	h2d_3p_[p1][p2][trajmeas.missing]->SetBinContent(binx, biny, h2d_3p_[p1][p2][trajmeas.missing]->GetBinContent(binx,biny)+1);
    }
  }

  // Histogram [p1][p2][p3] filled with x (,y , z)
  // 2 postfix - 2D
  void hist_fill(int p1, int p2, const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_y) {
    if (npostfix_==2&&dimensions_==2)
      h2d_2p_[p1][p2]->Fill(fillwith_x, fillwith_y);
  }

  void hist_increasebin(int p1, int p2, const TrajMeasurement &trajmeas, int binx, int biny) {
    if (npostfix_==2&&dimensions_==2)
      h2d_2p_[p1][p2]->SetBinContent(binx, biny, h2d_2p_[p1][p2]->GetBinContent(binx,biny)+1);
  }

  // Det clusize
  void det_clusize_fill(const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_w) {
    if (npostfix_==2) {
      h1d_2p_[0][2]->Fill(fillwith_x, fillwith_w);
      h1d_2p_[0][3]->Fill(fillwith_x);
      h1d_2p_[trajmeas.mod_on.det+1][2]->Fill(fillwith_x, fillwith_w);
      h1d_2p_[trajmeas.mod_on.det+1][3]->Fill(fillwith_x);
      h1d_2p_[(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)][2]->Fill(fillwith_x, fillwith_w);
      h1d_2p_[(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)][3]->Fill(fillwith_x);
    }
  }

  void det_clusize_fill(const Cluster &clust, double fillwith_x, double fillwith_w) {
    if (npostfix_==2) {
      h1d_2p_[0][4]->Fill(fillwith_x, fillwith_w);
      h1d_2p_[0][5]->Fill(fillwith_x);
      h1d_2p_[clust.mod_on.det+1][4]->Fill(fillwith_x, fillwith_w);
      h1d_2p_[clust.mod_on.det+1][5]->Fill(fillwith_x);
      h1d_2p_[(clust.mod_on.det==0) ? 2+clust.mod_on.layer : 7+clust.mod_on.disk+(clust.mod_on.disk<0)][4]->Fill(fillwith_x, fillwith_w);
      h1d_2p_[(clust.mod_on.det==0) ? 2+clust.mod_on.layer : 7+clust.mod_on.disk+(clust.mod_on.disk<0)][5]->Fill(fillwith_x);
    }
  }

  void hist_clusize_fill(int p1, int p2, int p3, const TrajMeasurement &trajmeas, double fillwith_x, double fillwith_w) {
    if (npostfix_==4) {
      h1d_4p_[p1][p2][p3][2]->Fill(fillwith_x, fillwith_w);
      h1d_4p_[p1][p2][p3][3]->Fill(fillwith_x);
    }
  }

  void hist_clusize_fill(int p1, int p2, int p3, const Cluster &clust, double fillwith_x, double fillwith_w) {
    if (npostfix_==4) {
      h1d_4p_[p1][p2][p3][4]->Fill(fillwith_x, fillwith_w);
      h1d_4p_[p1][p2][p3][5]->Fill(fillwith_x);
    }
  }

  void cumeff_error_write(bool cumx) {
    if (npostfix_==1) {
      if (dimensions_==1) {
	for (int j=2; j<=3; j++) {
	  h1d_1p_[j]->Add(h1d_1p_[j-2]);
	  double x=0.0;
	  if (cumx) {
	    for (int i=0; i<=h1d_1p_[j]->GetNbinsX(); i++) {
	      x+=h1d_1p_[j]->GetBinContent(i);
	      h1d_1p_[j]->SetBinContent(i,x);
	    }
	  } else {
	    for (int i=h1d_1p_[j]->GetNbinsX()+1; i>0; i--) {
	      x+=h1d_1p_[j]->GetBinContent(i);
	      h1d_1p_[j]->SetBinContent(i,x);
	    }
	  }
	}
	for (int j=0; j<=1; j++) {
	  h1d_1p_[1+2*j]->Add(h1d_1p_[0+2*j]);
	  h1d_1p_[0+2*j]->Divide(h1d_1p_[1+2*j]);
	  for (int i=1; i<=h1d_1p_[0+2*j]->GetNbinsX(); i++) {
	    if (h1d_1p_[0+2*j]->GetBinContent(i)!=0)
	      h1d_1p_[0+2*j]->SetBinError(i,sqrt(h1d_1p_[0+2*j]->GetBinContent(i)
						 *(1-h1d_1p_[0+2*j]->GetBinContent(i))
						 /h1d_1p_[1+2*j]->GetBinContent(i)));
	  }
	  h1d_1p_[0+2*j]->Write();
	  h1d_1p_[1+2*j]->Write();
	}
      }
    } else if (npostfix_==2) {
      if (dimensions_==1) {
	for (size_t k=0; k<h1d_2p_.size(); k++) {
	  for (int j=2; j<=3; j++) {
	    h1d_2p_[k][j]->Add(h1d_2p_[k][j-2]);
	    double x=0.0;
	    if (cumx) {
	      for (int i=0; i<=h1d_2p_[k][j]->GetNbinsX(); i++) {
		x+=h1d_2p_[k][j]->GetBinContent(i);
		h1d_2p_[k][j]->SetBinContent(i,x);
	      }
	    } else {
	      for (int i=h1d_2p_[k][j]->GetNbinsX()+1; i>0; i--) {
		x+=h1d_2p_[k][j]->GetBinContent(i);
		h1d_2p_[k][j]->SetBinContent(i,x);
	      }
	    }
	  }
	  for (int j=0; j<=1; j++) {
	    h1d_2p_[k][1+2*j]->Add(h1d_2p_[k][0+2*j]);
	    h1d_2p_[k][0+2*j]->Divide(h1d_2p_[k][1+2*j]);
	    for (int i=1; i<=h1d_2p_[k][0+2*j]->GetNbinsX(); i++) {
	      if (h1d_2p_[k][0+2*j]->GetBinContent(i)!=0)
		h1d_2p_[k][0+2*j]->SetBinError(i,sqrt(h1d_2p_[k][0+2*j]->GetBinContent(i)
						      *(1-h1d_2p_[k][0+2*j]->GetBinContent(i))
						      /h1d_2p_[k][1+2*j]->GetBinContent(i)));
	    }
	    h1d_2p_[k][0+2*j]->Write();
	    h1d_2p_[k][1+2*j]->Write();
	  }
	}
      }
    } else if (npostfix_==3) {
      if (dimensions_==1) {
	for (size_t k=0; k<h1d_3p_.size(); k++) {
	  for (size_t l=0; l<h1d_3p_[k].size(); l++) {
	    for (int j=2; j<=3; j++) {
	      h1d_3p_[k][l][j]->Add(h1d_3p_[k][l][j-2]);
	      double x=0.0;
	      if (cumx) {
		for (int i=0; i<=h1d_3p_[k][l][j]->GetNbinsX(); i++) {
		  x+=h1d_3p_[k][l][j]->GetBinContent(i);
		  h1d_3p_[k][l][j]->SetBinContent(i,x);
		}
	      } else {
		for (int i=h1d_3p_[k][l][j]->GetNbinsX()+1; i>0; i--) {
		  x+=h1d_3p_[k][l][j]->GetBinContent(i);
		  h1d_3p_[k][l][j]->SetBinContent(i,x);
		}
	      }
	    }
	    for (int j=0; j<=1; j++) {
	      h1d_3p_[k][l][1+2*j]->Add(h1d_3p_[k][l][0+2*j]);
	      h1d_3p_[k][l][0+2*j]->Divide(h1d_3p_[k][l][1+2*j]);
	      for (int i=1; i<=h1d_3p_[k][l][0+2*j]->GetNbinsX(); i++) {
		if (h1d_3p_[k][l][0+2*j]->GetBinContent(i)!=0)
		  h1d_3p_[k][l][0+2*j]->SetBinError(i,sqrt(h1d_3p_[k][l][0+2*j]->GetBinContent(i)
						     *(1-h1d_3p_[k][l][0+2*j]->GetBinContent(i))
						     /h1d_3p_[k][l][1+2*j]->GetBinContent(i)));
	      }
	      h1d_3p_[k][l][0+2*j]->Write();
	      h1d_3p_[k][l][1+2*j]->Write();
	    }
	  }
	}
      }
    }
  }

  void cumeff_error_write(bool cumx, bool cumy) {
    if (npostfix_==1) {
      if (dimensions_==2) {
	for (int k=2; k<=3; k++) {
	  h2d_1p_[k]->Add(h2d_1p_[k-2]);
	  for (int j=0; j<=h2d_1p_[k]->GetNbinsY(); j++) {
	    double x=0.0;
	    if (cumx) {
	      for (int i=0; i<=h2d_1p_[k]->GetNbinsX(); i++) {
		x+=h2d_1p_[k]->GetBinContent(i,j);
		h2d_1p_[k]->SetBinContent(i,j,x);
	      }
	    } else {
	      for (int i=h2d_1p_[k]->GetNbinsX()+1; i>0; i--) {
		x+=h2d_1p_[k]->GetBinContent(i,j);
		h2d_1p_[k]->SetBinContent(i,j,x);
	      }
	    }
	  }
	  for (int i=0; i<=h2d_1p_[k]->GetNbinsX(); i++) {
	    double y=0.0;
	    if (cumy) {
	      for (int j=0; j<=h2d_1p_[k]->GetNbinsY(); j++) {
		y+=h2d_1p_[k]->GetBinContent(i,j);
		h2d_1p_[k]->SetBinContent(i,j,y);
	      }
	    } else {
	      for (int j=h2d_1p_[k]->GetNbinsY()+1; j>0; j--) {
		y+=h2d_1p_[k]->GetBinContent(i,j);
		h2d_1p_[k]->SetBinContent(i,j,y);
	      }
	    }
	  }
	}
	for (int j=0; j<=1; j++) {
	  h2d_1p_[1+2*j]->Add(h1d_1p_[0+2*j]);
	  h2d_1p_[0+2*j]->Divide(h1d_1p_[1+2*j]);
	  h2d_1p_[0+2*j]->Write();
	  h2d_1p_[1+2*j]->Write();
	}
      }
    } else if (npostfix_==2) {
      if (dimensions_==2) {
	for (size_t l=0; l<h2d_2p_.size(); l++) {
	  for (int k=2; k<=3; k++) {
	    h2d_2p_[l][k]->Add(h2d_2p_[l][k-2]);
	    for (int j=0; j<=h2d_2p_[l][k]->GetNbinsY(); j++) {
	      double x=0.0;
	      if (cumx) {
		for (int i=0; i<=h2d_2p_[l][k]->GetNbinsX(); i++) {
		  x+=h2d_2p_[l][k]->GetBinContent(i,j);
		  h2d_2p_[l][k]->SetBinContent(i,j,x);
		}
	      } else {
		for (int i=h2d_2p_[l][k]->GetNbinsX()+1; i>0; i--) {
		  x+=h2d_2p_[l][k]->GetBinContent(i,j);
		  h2d_2p_[l][k]->SetBinContent(i,j,x);
		}
	      }
	    }
	    for (int i=0; i<=h2d_2p_[l][k]->GetNbinsX(); i++) {
	      double y=0.0;
	      if (cumy) {
		for (int j=0; j<=h2d_2p_[l][k]->GetNbinsY(); j++) {
		  y+=h2d_2p_[l][k]->GetBinContent(i,j);
		  h2d_2p_[l][k]->SetBinContent(i,j,y);
		}
	      } else {
		for (int j=h2d_2p_[l][k]->GetNbinsY()+1; j>0; j--) {
		  y+=h2d_2p_[l][k]->GetBinContent(i,j);
		  h2d_2p_[l][k]->SetBinContent(i,j,y);
		}
	      }
	    }
	  }
	  for (int j=0; j<=2; j+=2) {
	    h2d_2p_[l][1+j]->Add(h2d_2p_[l][0+j]);
	    h2d_2p_[l][0+j]->Divide(h2d_2p_[l][1+j]);
	    h2d_2p_[l][0+j]->Write();
	    h2d_2p_[l][1+j]->Write();
	  }
	}
      }
    } else if (npostfix_==3) {
      if (dimensions_==2) {
	for (size_t l=0; l<h2d_3p_.size(); l++) {
	  for (size_t m=0; m<h2d_3p_[l].size(); m++) {
	    for (int k=2; k<=3; k++) {
	      h2d_3p_[l][m][k]->Add(h2d_3p_[l][m][k-2]);
	      for (int j=0; j<=h2d_3p_[l][m][k]->GetNbinsY(); j++) {
		double x=0.0;
		if (cumx) {
		  for (int i=0; i<=h2d_3p_[l][m][k]->GetNbinsX(); i++) {
		    x+=h2d_3p_[l][m][k]->GetBinContent(i,j);
		    h2d_3p_[l][m][k]->SetBinContent(i,j,x);
		  }
		} else {
		  for (int i=h2d_3p_[l][m][k]->GetNbinsX()+1; i>0; i--) {
		    x+=h2d_3p_[l][m][k]->GetBinContent(i,j);
		    h2d_3p_[l][m][k]->SetBinContent(i,j,x);
		  }
		}
	      }
	      for (int i=0; i<=h2d_3p_[l][m][k]->GetNbinsX(); i++) {
		double y=0.0;
		if (cumy) {
		  for (int j=0; j<=h2d_3p_[l][m][k]->GetNbinsY(); j++) {
		    y+=h2d_3p_[l][m][k]->GetBinContent(i,j);
		    h2d_3p_[l][m][k]->SetBinContent(i,j,y);
		  }
		} else {
		  for (int j=h2d_3p_[l][m][k]->GetNbinsY()+1; j>0; j--) {
		    y+=h2d_3p_[l][m][k]->GetBinContent(i,j);
		    h2d_3p_[l][m][k]->SetBinContent(i,j,y);
		  }
		}
	      }
	    }
	    for (int j=0; j<=1; j++) {
	      h2d_3p_[l][m][1+2*j]->Add(h2d_3p_[l][m][0+2*j]);
	      h2d_3p_[l][m][0+2*j]->Divide(h2d_3p_[l][m][1+2*j]);
	      h2d_3p_[l][m][0+2*j]->Write();
	      h2d_3p_[l][m][1+2*j]->Write();
	    }
	  }
	}
      }
    }
  }
  
  void eff_error_write() {
    if (npostfix_==1) {
      if (dimensions_==1) {
	h1d_1p_[1]->Add(h1d_1p_[0]);
	h1d_1p_[0]->Divide(h1d_1p_[1]);
	for (int i=1; i<=h1d_1p_[0]->GetNbinsX(); i++) {
	  if (h1d_1p_[0]->GetBinContent(i)!=0)
	    h1d_1p_[0]->SetBinError(i,sqrt(h1d_1p_[0]->GetBinContent(i)
					   *(1-h1d_1p_[0]->GetBinContent(i))
					   /h1d_1p_[1]->GetBinContent(i)));
	}
	h1d_1p_[0]->Write();
	h1d_1p_[1]->Write();
      } else if (dimensions_==2) {
	h2d_1p_[1]->Add(h2d_1p_[0]);
	h2d_1p_[0]->Divide(h2d_1p_[1]);
	h2d_1p_[0]->Write();
	h2d_1p_[1]->Write();
      } else if (dimensions_==3) {
	h3d_1p_[1]->Add(h3d_1p_[0]);
	h3d_1p_[0]->Divide(h3d_1p_[1]);
	h3d_1p_[0]->Write();
	h3d_1p_[1]->Write();
      }
    } else if (npostfix_==2) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_2p_.size(); i++) {
	  h1d_2p_[i][1]->Add(h1d_2p_[i][0]);
	  h1d_2p_[i][0]->Divide(h1d_2p_[i][1]);
	  for (int a=1; a<=h1d_2p_[i][0]->GetNbinsX(); a++) {
	    if (h1d_2p_[i][0]->GetBinContent(a)!=0)
	      h1d_2p_[i][0]->SetBinError(a,sqrt(h1d_2p_[i][0]->GetBinContent(a)
					     *(1-h1d_2p_[i][0]->GetBinContent(a))
					     /h1d_2p_[i][1]->GetBinContent(a)));
	  }
	  h1d_2p_[i][0]->Write();
	  h1d_2p_[i][1]->Write();
	}
      } else if (dimensions_==2) {
	for (size_t i=0; i<h2d_2p_.size(); i++) {
	  h2d_2p_[i][1]->Add(h2d_2p_[i][0]);
	  h2d_2p_[i][0]->Divide(h2d_2p_[i][1]);
	  for (size_t j=0; j<h2d_2p_[i].size(); j++) {
	    h2d_2p_[i][j]->Write();
	    h2d_2p_[i][j]->Write();
	  }
	}
      } else if (dimensions_==3) {
	for (size_t i=0; i<h3d_2p_.size(); i++) {
	  h3d_2p_[i][1]->Add(h3d_2p_[i][0]);
	  h3d_2p_[i][0]->Divide(h3d_2p_[i][1]);
	  h3d_2p_[i][0]->Write();
	  h3d_2p_[i][1]->Write();
	}
      }
    } else if (npostfix_==3) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_3p_.size(); i++) {
	  for (size_t j=0; j<h1d_3p_[i].size(); j++) {
	    h1d_3p_[i][j][1]->Add(h1d_3p_[i][j][0]);
	    h1d_3p_[i][j][0]->Divide(h1d_3p_[i][j][1]);
	    for (int a=1; a<=h1d_3p_[i][j][0]->GetNbinsX(); a++) {
	      if (h1d_3p_[i][j][0]->GetBinContent(a)!=0)
		h1d_3p_[i][j][0]->SetBinError(a,sqrt(h1d_3p_[i][j][0]->GetBinContent(a)
						     *(1-h1d_3p_[i][j][0]->GetBinContent(a))
						     /h1d_3p_[i][j][1]->GetBinContent(a)));
	    }
	    h1d_3p_[i][j][0]->Write();
	    h1d_3p_[i][j][1]->Write();
	  }
	}
      } else if (dimensions_==2) {
	for (size_t i=0; i<h2d_3p_.size(); i++) {
	  for (size_t j=0; j<h2d_3p_[i].size(); j++) {
	    h2d_3p_[i][j][1]->Add(h2d_3p_[i][j][0]);
	    h2d_3p_[i][j][0]->Divide(h2d_3p_[i][j][1]);
	    h2d_3p_[i][j][0]->Write();
	    h2d_3p_[i][j][1]->Write();
	  }
	}
      } else if (dimensions_==3) {
	for (size_t i=0; i<h3d_3p_.size(); i++) {
	  for (size_t j=0; j<h3d_3p_[i].size(); j++) {
	    h3d_3p_[i][j][1]->Add(h3d_3p_[i][j][0]);
	    h3d_3p_[i][j][0]->Divide(h3d_3p_[i][j][1]);
	    h3d_3p_[i][j][0]->Write();
	    h3d_3p_[i][j][1]->Write();
	  }
	}
      }
    }
  }

  void effclu_error_write() {
    if (npostfix_==2) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_2p_.size(); i++) {
	  h1d_2p_[i][1]->Add(h1d_2p_[i][0]);
	  h1d_2p_[i][0]->Divide(h1d_2p_[i][1]);
	  for (int a=1; a<=h1d_2p_[i][0]->GetNbinsX(); a++) {
	    if (h1d_2p_[i][0]->GetBinContent(a)!=0)
	      h1d_2p_[i][0]->SetBinError(a,sqrt(h1d_2p_[i][0]->GetBinContent(a)
					     *(1-h1d_2p_[i][0]->GetBinContent(a))
					     /h1d_2p_[i][1]->GetBinContent(a)));
	  }
	  h1d_2p_[i][0]->Write();
	  h1d_2p_[i][1]->Write();
	  h1d_2p_[i][2]->Divide(h1d_2p_[i][3]);
	  h1d_2p_[i][2]->Write();
	  h1d_2p_[i][3]->Write();
#ifdef COMPLETE
	  h1d_2p_[i][4]->Divide(h1d_2p_[i][5]);
	  h1d_2p_[i][4]->Write();
	  h1d_2p_[i][5]->Write();
#endif
	}
      }
    }
    if (npostfix_==4) {
      if (dimensions_==1) {
	for (size_t i=0; i<h1d_4p_.size(); i++) {
	  for (size_t j=0; j<h1d_4p_[i].size(); j++) {
	    for (size_t k=0; k<h1d_4p_[i][j].size(); k++) {
	      h1d_4p_[i][j][k][1]->Add(h1d_4p_[i][j][k][0]);
	      h1d_4p_[i][j][k][0]->Divide(h1d_4p_[i][j][k][1]);
	      for (int a=1; a<=h1d_4p_[i][j][k][0]->GetNbinsX(); a++) {
		if (h1d_4p_[i][j][k][0]->GetBinContent(a)!=0)
		  h1d_4p_[i][j][k][0]->SetBinError(a,sqrt(h1d_4p_[i][j][k][0]->GetBinContent(a)
						    *(1-h1d_4p_[i][j][k][0]->GetBinContent(a))
						    /h1d_4p_[i][j][k][1]->GetBinContent(a)));
	      }
	      h1d_4p_[i][j][k][0]->Write();
	      h1d_4p_[i][j][k][1]->Write();
	      h1d_4p_[i][j][k][2]->Divide(h1d_4p_[i][j][k][3]);
	      h1d_4p_[i][j][k][2]->Write();
	      h1d_4p_[i][j][k][3]->Write();
#ifdef COMPLETE
	      h1d_4p_[i][j][k][4]->Divide(h1d_4p_[i][j][k][5]);
	      h1d_4p_[i][j][k][4]->Write();
	      h1d_4p_[i][j][k][5]->Write();
#endif
	    }
	  }
	}
      }
    }
  }

  // Accessors
  int npostfix() { return npostfix_; }
  int dimensions() { return dimensions_; }

  std::vector<TH1D*> h1d_1p() { return h1d_1p_; }
  std::vector<TH2D*> h2d_1p() { return h2d_1p_; }
  std::vector<TH3D*> h3d_1p() { return h3d_1p_; }
  std::vector<std::vector<TH1D*> > h1d_2p() { return h1d_2p_; }
  std::vector<std::vector<TH2D*> > h2d_2p() { return h2d_2p_; }
  std::vector<std::vector<TH3D*> > h3d_2p() { return h3d_2p_; }
  std::vector<std::vector<std::vector<TH1D*> > > h1d_3p() { return h1d_3p_; }
  std::vector<std::vector<std::vector<TH2D*> > > h2d_3p() { return h2d_3p_; }
  std::vector<std::vector<std::vector<TH3D*> > > h3d_3p() { return h3d_3p_; }
  std::vector<std::vector<std::vector<std::vector<TH1D*> > > > h1d_4p() { return h1d_4p_; }
  std::vector<std::vector<std::vector<std::vector<TH2D*> > > > h2d_4p() { return h2d_4p_; }
  std::vector<std::vector<std::vector<std::vector<TH3D*> > > > h3d_4p() { return h3d_4p_; }

};
