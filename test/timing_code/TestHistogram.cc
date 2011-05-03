#include "DataStruct.h"
#include "TChain.h"
#include "TChainElement.h"
#include "TCanvas.h"
#include "TH3.h"
#include "TF1.h"
#include <cmath>
#include <iostream>
#include "Histograms.h"
#include "CanvasMaker.h"

class AllHistos {

 public: 
  
  AllHistos(int trajhistos, int clusthistos) {
    trajhistos_=trajhistos;
    clusthistos_=clusthistos;
    create_();
  }
  ~AllHistos() { }
  
 private:
  int trajhistos_;
  int clusthistos_;

  std::vector<Histograms*> Efficiency_;
  std::vector<Histograms*> Nminusone_;
  std::vector<Histograms*> Neweff;
  std::vector<Histograms*> Scans_;

//   std::vector<Histograms*> coarsedelay_;
//   std::vector<Histograms*> voltage_;
//   std::vector<Histograms*> hv_;

  // Cuts
  bool allcuts_;
  bool cut_roc_;
  bool cut_run_;
  bool nminus_pt_;
  bool nminus_nstrip_;
  bool nminus_dz_;
  bool nminus_d0_;
  bool nminus_dz_d0_;
  bool nminus_lxlyfid_;
  bool nminus_lxfid_;
  bool nminus_lyfid_;
  bool nminus_nvtx_;
  bool cut_scans_;

  // Det
  int det_x_;
  int modroc_p1_;
  int sdpx_;
  int mod_x_;
  int mod_y_;
  int roc_binx_;
  int roc_biny_;
  float roc_x_;
  float roc_y_;
  // N-1
  int p1_fid_;
  int p2_fid_;
  // Scans
  int delay_x_;
  int p1_bmpio_;
  int p2_secdisk_;
  int p3_layrog_;
  int hv_10apr_x_;
  int p1_hv_10apr_;
  int hv_11mar_x_;
  int p1_hv_11mar_;
  // Variables
  std::vector<std::string> postfix_cumeff_;
  std::vector<std::string> postfix_det_;
  int inac_;
  int debug_;
  int nrun_;
  std::vector<std::vector<std::vector<double> > > run_det_valmis_; 
  std::vector<int> runno_;

  int delay11_(EventData &evt) {
    int delay = NOVAL_I;
    // Delay scans
    if (evt.run==160413||evt.run==160497||evt.run==160577||evt.run==160578) {
      if (evt.run==160413) {
	if (evt.orb<11531874) delay = 25;
	else if (evt.orb>=11531874&&evt.orb<22836667) delay = 0;
	else if (evt.orb>=22836667&&evt.orb<36525473) delay = NOVAL_I; // wbc 154 - ignore
	else if (evt.orb>=36525473&&evt.orb<47699601) delay = 7; 
	else if (evt.orb>=47699601&&evt.orb<60471838) delay = 8;
	else if (evt.orb>=60471838&&evt.orb<71445138) delay = 10;
	else if (evt.orb>=71445138&&evt.orb<84930036) delay = -25;
	else if (evt.orb>=84930036&&evt.orb<100147197) delay = NOVAL_I; // 12 - ignore
	else if (evt.orb>=100147197&&evt.orb<114600000) delay = 14;
	else if (evt.orb>=114600000&&evt.orb<129039651) delay = 16;
	else if (evt.orb>=129039651&&evt.orb<145100000) delay = -12;
	else if (evt.orb>=145100000&&evt.orb<160586081) delay = 10;
	else if (evt.orb>=160586081&&evt.orb<174462084) delay = -6;
	else if (evt.orb>=174462084&&evt.orb<191550000) delay = -8;
	else if (evt.orb>=191550000) delay = 12;
      }
      else if (evt.run==160497) {
	if (evt.orb<11923615) delay = NOVAL_I;
	else if (evt.orb>=11923615&&evt.orb<23009846) delay = -4;
	else if (evt.orb>=23009846&&evt.orb<34154357) delay = -2;
	else delay = NOVAL_I;
      }
      else if (evt.run==160577) {
	if (evt.orb<10359519) delay = NOVAL_I;
	else if (evt.orb>=10359519&&evt.orb<26677701) delay = 4;
	else if (evt.orb>=26677701&&evt.orb<40220000) delay = 18;
	else if (evt.orb>=40220000&&evt.orb<53440000) delay = 20;
	else if (evt.orb>=53440000&&evt.orb<66319978) delay = 22;
	else if (evt.orb>=66319978) delay = 14;
      }
      else if (evt.run==160578) {
	if (evt.orb<1245694) delay = NOVAL_I;
	else if (evt.orb>=1245694&&evt.orb<14309529) delay = 14;
	else if (evt.orb>=14309529&&evt.orb<43223561) delay = 16;
	else if (evt.orb>=43223561&&evt.orb<71862094) delay = 15;
	else if (evt.orb>=71862094&&evt.orb<108617422) delay = 13;
	else delay = NOVAL_I;
      }
    }
    else if ((!(evt.run==160579||evt.run==160819||evt.run==160827||evt.run==160835))
	     &&evt.run<161684&&evt.run>161413) delay = 12;
    return delay;
  }

  int clust_delay11_(EventData &cluevt) {
    int delay = NOVAL_I;
    // Delay scans
    if (cluevt.run==160413||cluevt.run==160497||cluevt.run==160577||cluevt.run==160578) {
      if (cluevt.run==160413) {
	if (cluevt.orb<11531874) delay = 25;
	else if (cluevt.orb>=11531874&&cluevt.orb<22836667) delay = 0;
	else if (cluevt.orb>=22836667&&cluevt.orb<36525473) delay = NOVAL_I; // wbc 154 - ignore
	else if (cluevt.orb>=36525473&&cluevt.orb<47699601) delay = 7; 
	else if (cluevt.orb>=47699601&&cluevt.orb<60471838) delay = 8;
	else if (cluevt.orb>=60471838&&cluevt.orb<71445138) delay = 10;
	else if (cluevt.orb>=71445138&&cluevt.orb<84930036) delay = -25;
	else if (cluevt.orb>=84930036&&cluevt.orb<100147197) delay = NOVAL_I; // 12 - ignore
	else if (cluevt.orb>=100147197&&cluevt.orb<114600000) delay = 14;
	else if (cluevt.orb>=114600000&&cluevt.orb<129039651) delay = 16;
	else if (cluevt.orb>=129039651&&cluevt.orb<145100000) delay = -12;
	else if (cluevt.orb>=145100000&&cluevt.orb<160586081) delay = 10;
	else if (cluevt.orb>=160586081&&cluevt.orb<174462084) delay = -6;
	else if (cluevt.orb>=174462084&&cluevt.orb<191550000) delay = -8;
	else if (cluevt.orb>=191550000) delay = 12;
      }
      else if (cluevt.run==160497) {
	if (cluevt.orb<11923615) delay = NOVAL_I;
	else if (cluevt.orb>=11923615&&cluevt.orb<23009846) delay = -4;
	else if (cluevt.orb>=23009846&&cluevt.orb<34154357) delay = -2;
	else delay = NOVAL_I;
      }
      else if (cluevt.run==160577) {
	if (cluevt.orb<10359519) delay = NOVAL_I;
	else if (cluevt.orb>=10359519&&cluevt.orb<26677701) delay = 4;
	else if (cluevt.orb>=26677701&&cluevt.orb<40220000) delay = 18;
	else if (cluevt.orb>=40220000&&cluevt.orb<53440000) delay = 20;
	else if (cluevt.orb>=53440000&&cluevt.orb<66319978) delay = 22;
	else if (cluevt.orb>=66319978) delay = 14;
      }
      else if (cluevt.run==160578) {
	if (cluevt.orb<1245694) delay = NOVAL_I;
	else if (cluevt.orb>=1245694&&cluevt.orb<14309529) delay = 14;
	else if (cluevt.orb>=14309529&&cluevt.orb<43223561) delay = 16;
	else if (cluevt.orb>=43223561&&cluevt.orb<71862094) delay = 15;
	else if (cluevt.orb>=71862094&&cluevt.orb<108617422) delay = 13;
	else delay = NOVAL_I;
      }
    }
    else if ((!(cluevt.run==160579||cluevt.run==160819||cluevt.run==160827||cluevt.run==160835))
	     &&cluevt.run<161684&&cluevt.run>161413) delay = 12;
    return delay;
  }

  int voltage10_(EventData &evt, TrajMeasurement &trajmeas) {
    int hv = NOVAL_I;
    // hv scan 2010 April
    if (evt.run==132599||evt.run==132601) {
      if (evt.run==132599) {
        if (evt.ls>=1 && evt.ls<75) { hv = (trajmeas.mod_on.det) ? 300 : 150;
        } else if (evt.ls>=75 && evt.ls<126) { hv = (trajmeas.mod_on.det) ? 250 : 140;
        } else if (evt.ls>=126 && evt.ls<178) { hv = (trajmeas.mod_on.det) ? 200 : 130;
        } else if (evt.ls>=178 && evt.ls<233) { hv = (trajmeas.mod_on.det) ? 150 : 120;
        } else if (evt.ls>=233 && evt.ls<283) { hv = (trajmeas.mod_on.det) ? 110 : 110;
        } else if (evt.ls>=283 && evt.ls<332) { hv = (trajmeas.mod_on.det) ? 100 : 100;
        } else if (evt.ls>=332 && evt.ls<385) { hv = (trajmeas.mod_on.det) ? 90 : 90;
        } else if (evt.ls>=385 && evt.ls<999) { hv = (trajmeas.mod_on.det) ? 80 : 80;
        } else { hv =NOVAL_I; }
      } else if (evt.run==132601) {
        if (evt.ls>=1 && evt.ls<50) { hv = (trajmeas.mod_on.det) ? 70 : 70;
        } else if (evt.ls>=50 && evt.ls<105) { hv = (trajmeas.mod_on.det) ? 60 : 60;
        } else if (evt.ls>=105 && evt.ls<155) { hv = (trajmeas.mod_on.det) ? 50 : 50;
        } else if (evt.ls>=155 && evt.ls<207) { hv = (trajmeas.mod_on.det) ? 40 : 40;
        } else if (evt.ls>=207 && evt.ls<258) { hv = (trajmeas.mod_on.det) ? 30 : 30;
        } else { hv =NOVAL_I; }
      }
    }
    else hv = (trajmeas.mod_on.det) ? 300 : 150;
    return hv;
  }

  int voltage11_(EventData &evt, TrajMeasurement &trajmeas) {
    int voltage = NOVAL_I;
    // voltage scan 2011 March
    if (evt.run==160578||evt.run==160579||evt.run==160819||evt.run==160827||evt.run==160835) {
      if (evt.run==160579) {
	if (evt.orb<10060929) voltage = (trajmeas.mod_on.det) ? 30 : 40;
	else if (evt.orb>=10060929&&evt.orb<20603100) voltage = (trajmeas.mod_on.det) ? 35 : 50;
	else if (evt.orb>=20603100&&evt.orb<30368455) voltage = (trajmeas.mod_on.det) ? 40 : 60;
	else if (evt.orb>=30368455&&evt.orb<36726096) voltage = (trajmeas.mod_on.det) ? 50 : 70;
	else if (evt.orb>=36726096&&evt.orb<41127055) voltage = (trajmeas.mod_on.det) ? 60 : 80;
        else if (evt.orb>=41127055) voltage = (trajmeas.mod_on.det) ? 70 : 90;
      }
      else if (evt.run==160578) {
        if (evt.orb>=108617422) voltage = (trajmeas.mod_on.det) ? 20 : 30;
	else voltage = NOVAL_I;
      } 
      else if (evt.run==160819) {
        if (evt.orb<734481) voltage = NOVAL_I;
	else if (evt.orb>=734481&&evt.orb<4945473) voltage = (trajmeas.mod_on.det) ? 300 : 150;
	else if (evt.orb>=4945473&&evt.orb<9184218) voltage = (trajmeas.mod_on.det) ? 50 : 70;
        else if (evt.orb>=9184218&&evt.orb<13531251) voltage = (trajmeas.mod_on.det) ? 60 : 80;
        else if (evt.orb>=13531251&&evt.orb<21375662) voltage = (trajmeas.mod_on.det) ? 70 : 90;
        else if (evt.orb>=21375662&&evt.orb<27850887) voltage = (trajmeas.mod_on.det) ? 80 : 100;
        else voltage = NOVAL_I;
      }
      else if (evt.run==160827) voltage = (trajmeas.mod_on.det) ? 120 : 130;
      else if (evt.run==160835) {
        if (evt.orb<22377196) voltage = (trajmeas.mod_on.det) ? 120 : 150;
        else voltage = (trajmeas.mod_on.det) ? 300 : 150;
      }
    } 
    else if (evt.run==160431) voltage = (trajmeas.mod_on.det) ? 300 : 150;
    return voltage;
  }

  void variables(EventData &evt, TrajMeasurement &trajmeas) {
    // Efficiency
    // det
    det_x_ = (trajmeas.mod_on.det==0) ? trajmeas.mod_on.layer :  trajmeas.mod_on.disk + 5 + (trajmeas.mod_on.disk<0);
    // mod
    modroc_p1_ = (trajmeas.mod_on.det) ? ((trajmeas.mod_on.blade>0) ? 3 : 4 ) : trajmeas.mod_on.layer - 1;
    sdpx_ = ((trajmeas.mod_on.disk>0) ? 1 : -1) * (2 * (abs(trajmeas.mod_on.disk) - 1) + trajmeas.mod_on.panel);
    mod_x_ = (trajmeas.mod_on.det) ? sdpx_ : trajmeas.mod_on.module;
    mod_y_ = (trajmeas.mod_on.det) ? trajmeas.mod_on.blade : trajmeas.mod_on.ladder;
    // roc
    if (trajmeas.validhit!=1&&trajmeas.missing!=1) inac_ = 1;
    else inac_ = 0;
    if (trajmeas.mod_on.det==0) {
      if (fabs(trajmeas.ly)<3.2) {
	for (int i=1;i<=8;i++) {
	  if (trajmeas.ly>=((4-i)*0.8) && trajmeas.ly<((5-i)*0.8))
	    roc_binx_ = (trajmeas.mod_on.module+4)*8 + i;
	}
      } else {
	roc_binx_ = NOVAL_I;
      }
      if ((trajmeas.mod_on.half==0&&fabs(trajmeas.lx)<0.8)||(trajmeas.mod_on.half==1&&fabs(trajmeas.lx)<0.4)) {
	int binsely = ((trajmeas.mod_on.half==1&&((trajmeas.mod_on.ladder<0&&trajmeas.mod_on.ladder%2==0)||trajmeas.mod_on.ladder%2==1))
		       ||(trajmeas.mod_on.half==0&&((trajmeas.mod_on.ladder<0 &&((trajmeas.mod_on.ladder%2==-1&&trajmeas.lx<0.0)
										 ||(trajmeas.mod_on.ladder%2==0&&trajmeas.lx>0.0)))
						    ||(trajmeas.mod_on.ladder>0 &&((trajmeas.mod_on.ladder%2==0&&trajmeas.lx<0.0)
										   ||(trajmeas.mod_on.ladder%2==1&&trajmeas.lx>0.0))))));
	roc_biny_ = (trajmeas.mod_on.layer*6 + trajmeas.mod_on.ladder + 4) * 2 + 1 + binsely;
      } else {
	roc_biny_ = NOVAL_I;
      }
    } else if (trajmeas.mod_on.det==1) {
      int nrocly = trajmeas.mod_on.module + trajmeas.mod_on.panel;
      if (fabs(trajmeas.ly)<(nrocly*0.4)) {
	for (int i=0; i<nrocly; i++) {
	  // ROC number = nrocly - 1 - i for + LX and nrocly + i for -LX.
	  int j = (trajmeas.mod_on.disk<0) ? i : nrocly - 1 - i;
	  if (trajmeas.ly>=((2*j-nrocly)*0.4)&&trajmeas.ly<((2*(j+1)-nrocly)*0.4))
	    roc_biny_ = 6 - nrocly + 2 * i + ((trajmeas.mod_on.blade>0) ? trajmeas.mod_on.blade-1 : trajmeas.mod_on.blade + 12)*12 + 1;
	}
      } else roc_biny_ = NOVAL_I;
      int binselx = (trajmeas.mod_on.panel==1&&(trajmeas.mod_on.module==1||trajmeas.mod_on.module==4)) ?
	((fabs(trajmeas.lx)<0.4) ? (trajmeas.mod_on.module==1) : NOVAL_I ) 
	: ((fabs(trajmeas.lx)<0.8) ? ((trajmeas.mod_on.panel==1&&trajmeas.lx<0.0)||(trajmeas.mod_on.panel==2&&trajmeas.lx>=0.0)) : NOVAL_I);
      int nperpan = 2 * trajmeas.mod_on.module + trajmeas.mod_on.panel - 1 + binselx;
      roc_binx_ = (binselx!=NOVAL_I) ? ((trajmeas.mod_on.disk>0) ? nperpan : 9 - nperpan) + (sdpx_ + 4) * 8 
	- 2 * ((abs(trajmeas.mod_on.disk)==1) ? trajmeas.mod_on.disk : 0) : NOVAL_I;
    }
    // N-1 plots
    // fid
    p1_fid_ = det_x_ - 1;
    p2_fid_ = (trajmeas.mod_on.det==0) ? trajmeas.mod_on.half : trajmeas.mod_on.module -1 + 4*(trajmeas.mod_on.panel-1);
    // Scans
    delay_x_ = delay11_(evt);
    hv_11mar_x_ = voltage11_(evt, trajmeas);
    hv_10apr_x_ = voltage10_(evt, trajmeas);
    p1_hv_11mar_ = (trajmeas.mod_on.det==0) ? (trajmeas.mod_on.layer==3) : 2;
    p1_hv_10apr_ = trajmeas.mod_on.det;
    // ROG variables
    p1_bmpio_ = (trajmeas.mod_on.det==0) ? 2 * (trajmeas.mod_on.module<0) + (trajmeas.mod_on.ladder>0) 
      : 2 * (trajmeas.mod_on.disk<0) + (trajmeas.mod_on.blade>0);
    p2_secdisk_ = (trajmeas.mod_on.det==0) ? trajmeas.mod_on.sec - 1 : 7 + abs(trajmeas.mod_on.disk) ;
    p3_layrog_ = (trajmeas.mod_on.det==0) ? (trajmeas.mod_on.layer==3) : (abs(trajmeas.mod_on.blade)-1)/3;
  }

  int nbinsx_, nbinsy_;
  double xlow_, xup_, ylow_, yup_;
  //binmaker for fid_calc functions
  void binmaker_(int nroclx, int nrocly) {
    float rocsizelx = 0.8; 
    float rocsizely = 0.8;
    int nbinslx = 32;
    int noutlx = 8;
    int nbinsly = 32;
    int noutly = 32;
    if (nroclx==1&&nrocly==8) {
      nbinslx = 16;
      noutlx = 4;
      nbinsly = 16;
      noutly = 16;
    }
    nbinsx_ = (nrocly * nbinsly + 2 * noutly);
    nbinsy_ = (nroclx * nbinslx + 2 * noutlx);
    xlow_ = -1.0 * nbinsx_ / 2 * rocsizely / nbinsly;
    xup_ = 1.0 * nbinsx_ / 2 * rocsizely / nbinsly;
    ylow_ = -1.0 * nbinsy_ / 2 * rocsizelx / nbinslx;
    yup_ = 1.0 * nbinsy_ / 2 * rocsizelx / nbinslx;
  }

  void create_() {
    debug_ = 0;
    std::vector<std::string> null;

    // Efficiency postfixes
    // Cumulative Efficiency postfixes
    postfix_cumeff_.push_back("_eff");
    postfix_cumeff_.push_back("_dist");
    postfix_cumeff_.push_back("_cumeff");
    postfix_cumeff_.push_back("_cumdist");

    std::vector<std::string> postfix_inac;
    postfix_inac.push_back("_eff");
    postfix_inac.push_back("_dist");
    postfix_inac.push_back("_inac");

    std::vector<std::string> postfix_eff2;
    postfix_eff2.push_back("_eff");
    postfix_eff2.push_back("_dist");
    postfix_eff2.push_back("_lx_eff");
    postfix_eff2.push_back("_lx_dist");
    postfix_eff2.push_back("_ly_eff");
    postfix_eff2.push_back("_ly_dist");

    // Avg clusize postfixes
    std::vector<std::string> postfix_effclu;
    postfix_effclu.push_back("_eff");
    postfix_effclu.push_back("_dist");
    postfix_effclu.push_back("_onclusize");
    postfix_effclu.push_back("_onnclu");
    postfix_effclu.push_back("_clusize");
    postfix_effclu.push_back("_nclu");

    // Detector postfixes
    postfix_det_.push_back("");
    postfix_det_.push_back("_bpix");
    postfix_det_.push_back("_fpix");
    postfix_det_.push_back("_l1");
    postfix_det_.push_back("_l2");
    postfix_det_.push_back("_l3");
    postfix_det_.push_back("_dm2");
    postfix_det_.push_back("_dm1");
    postfix_det_.push_back("_dp1");
    postfix_det_.push_back("_dp2");

    std::vector<std::string> postfix_fpixIO;
    postfix_fpixIO.push_back("_fpixI");
    postfix_fpixIO.push_back("_fpixO");

    std::vector<std::string> postfix_mod;
    postfix_mod.push_back("_full");
    postfix_mod.push_back("_half");
    postfix_mod.push_back("_p1_m1");
    postfix_mod.push_back("_p1_m2");
    postfix_mod.push_back("_p1_m3");
    postfix_mod.push_back("_p1_m4");
    postfix_mod.push_back("_p2_m1");
    postfix_mod.push_back("_p2_m2");
    postfix_mod.push_back("_p2_m3");

    // Readout groups
    std::vector<std::string> postfix_BmpIO;
    postfix_BmpIO.push_back("_BpO");
    postfix_BmpIO.push_back("_BpI");
    postfix_BmpIO.push_back("_BmO");
    postfix_BmpIO.push_back("_BmI");

    std::vector<std::string> postfix_sec;
    postfix_sec.push_back("_Sec1");
    postfix_sec.push_back("_Sec2");
    postfix_sec.push_back("_Sec3");
    postfix_sec.push_back("_Sec4");
    postfix_sec.push_back("_Sec5");
    postfix_sec.push_back("_Sec6");
    postfix_sec.push_back("_Sec7");
    postfix_sec.push_back("_Sec8");

    std::vector<std::string> postfix_chn;
    postfix_chn.push_back("_Lyr12");
    postfix_chn.push_back("_Lyr3");

    std::vector<std::string> postfix_dsk;
    postfix_dsk.push_back("_D1");
    postfix_dsk.push_back("_D2");

    std::vector<std::string> postfix_rog;
    postfix_rog.push_back("_Rog1");
    postfix_rog.push_back("_Rog2");
    postfix_rog.push_back("_Rog3");
    postfix_rog.push_back("_Rog4");    

    if (trajhistos_==1) {

      // Detector efficiency histograms
      Efficiency_.push_back(new Histograms("det",      1, 1));
      Efficiency_.push_back(new Histograms("mod",      2, 2));
      Efficiency_.push_back(new Histograms("roc",      2, 2));

      Efficiency_[0] -> add(postfix_cumeff_, 1, 1, 2,  7, 0.5, 7.5);

      Efficiency_[1] -> add(postfix_det_,   1, 4, 4,  3,  postfix_cumeff_, 1, 1, 2,  9, -4.5, 4.5,  21, -10.5, 10.5);
      Efficiency_[1] -> add(postfix_det_,   1, 5, 5,  3,  postfix_cumeff_, 1, 1, 2,  9, -4.5, 4.5,  21, -16.5, 16.5);
      Efficiency_[1] -> add(postfix_det_,   1, 6, 6,  3,  postfix_cumeff_, 1, 1, 2,  9, -4.5, 4.5,  21, -22.5, 22.5);
      Efficiency_[1] -> add(postfix_fpixIO, 1, 1, 1, -3,  postfix_cumeff_, 1, 1, 2,  9, -4.5, 4.5,  12,   0.5, 12.5);
      Efficiency_[1] -> add(postfix_fpixIO, 1, 2, 2, -3,  postfix_cumeff_, 1, 1, 2,  9, -4.5, 4.5,  12, -12.5, -0.5);

      Efficiency_[2] -> add(postfix_det_,   1, 4, 4,  3,  postfix_inac,   1, 1, 3, 72, -4.5, 4.5,  42, -10.5, 10.5);
      Efficiency_[2] -> add(postfix_det_,   1, 5, 5,  3,  postfix_inac,   1, 1, 3, 72, -4.5, 4.5,  66, -16.5, 16.5);
      Efficiency_[2] -> add(postfix_det_,   1, 6, 6,  3,  postfix_inac,   1, 1, 3, 72, -4.5, 4.5,  90, -22.5, 22.5);
      Efficiency_[2] -> add(postfix_fpixIO, 1, 1, 1, -3,  postfix_inac,   1, 1, 3, 72, -4.5, 4.5, 144,   0.5, 12.5);
      Efficiency_[2] -> add(postfix_fpixIO, 1, 2, 2, -3,  postfix_inac,   1, 1, 3, 72, -4.5, 4.5, 144, -12.5, -0.5);

      Nminusone_.push_back(new Histograms("pt",       2, 1));
      Nminusone_.push_back(new Histograms("nstrip",   2, 1));
      Nminusone_.push_back(new Histograms("dz",       2, 1));
      Nminusone_.push_back(new Histograms("d0",       2, 1));
      Nminusone_.push_back(new Histograms("dz_d0",    2, 2));
      Nminusone_.push_back(new Histograms("normchi2", 2, 1));
      Nminusone_.push_back(new Histograms("fid",      3, 2));
      Nminusone_.push_back(new Histograms("fid",      3, 1));
      Nminusone_.push_back(new Histograms("fid",      3, 1));
#ifdef COMPLETE
      Nminusone_.push_back(new Histograms("vtxd0",    2, 1));
      Nminusone_.push_back(new Histograms("vtxz",     2, 1));
#endif
      Nminusone_[0]  -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  100, 0.0, 10.0);
      Nminusone_[1]  -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,   28, 0.0, 28.0);
      Nminusone_[2]  -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  100, 0.0,  1.0);
      Nminusone_[3]  -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  100, 0.0,  0.1);
      Nminusone_[4]  -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  100, 0.0,  1.0, 100, 0.0, 0.1);
      Nminusone_[5]  -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  500, 0.0,  5.0);

      binmaker_(2,8);
      Nminusone_[6]->add(postfix_det_, 1, 4,  6, 3, postfix_mod, 1, 1, 1, 0, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 1, 4,  6, 3, postfix_mod, 1, 1, 1, 0, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 1, 4,  6, 3, postfix_mod, 1, 1, 1, 0, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(1,8);
      Nminusone_[6]->add(postfix_det_, 0, 4,  6, 3, postfix_mod, 1, 2, 2, 0, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 4,  6, 3, postfix_mod, 1, 2, 2, 0, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 4,  6, 3, postfix_mod, 1, 2, 2, 0, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(1,2);
      Nminusone_[6]->add(postfix_det_, 1, 7, 10, 3, postfix_mod, 1, 3, 3, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 1, 7, 10, 3, postfix_mod, 1, 3, 3, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 1, 7, 10, 3, postfix_mod, 1, 3, 3, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(2,3);
      Nminusone_[6]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 4, 4, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 4, 4, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 4, 4, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(2,4);
      Nminusone_[6]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 5, 5, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 5, 5, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 5, 5, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(1,5);
      Nminusone_[6]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 6, 6, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 6, 6, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 6, 6, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(2,3);
      Nminusone_[6]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 7, 7, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 7, 7, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 7, 7, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(2,4);
      Nminusone_[6]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 8, 8, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 8, 8, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 8, 8, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);
      binmaker_(2,5);
      Nminusone_[6]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 9, 9, 2, postfix_eff2, 1, 1, 2, nbinsx_, xlow_, xup_, nbinsy_, ylow_, yup_);
      Nminusone_[7]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 9, 9, 2, postfix_eff2, 1, 3, 4, nbinsy_*2, ylow_, yup_);
      Nminusone_[8]->add(postfix_det_, 0, 7, 10, 3, postfix_mod, 1, 9, 9, 2, postfix_eff2, 1, 5, 6, nbinsx_/2, xlow_, xup_);

#ifdef COMPLETE
      Nminusone_[9]-> add(postfix_det_, 1, 1,  3, 0,  postfix_cumeff_, 1, 1, 2,  100, 0.0,  1.0);
      Nminusone_[10]-> add(postfix_det_, 1, 1,  3, 0,  postfix_cumeff_, 1, 1, 2,  250, 0.0, 25.0);
#endif

      Neweff.push_back(new Histograms("hcl",      2, 1));
      Neweff.push_back(new Histograms("clcl",     2, 1));
      Neweff.push_back(new Histograms("hh",       2, 1));
      Neweff.push_back(new Histograms("clcl_hcl", 2, 2));
      Neweff.push_back(new Histograms("hh_hcl",   2, 2));
      Neweff.push_back(new Histograms("neweff",   2, 1));
      Neweff.push_back(new Histograms("neweff2",  2, 1));

      Neweff[0] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  100, 0.0,  0.1);
      Neweff[1] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4, 1260, 0.0,  6.3);
      Neweff[2] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  200, 0.0,  2.0);
      Neweff[3] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4, 1260, 0.0,  6.3, 100, 0.0, 0.1);
      Neweff[4] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 4,  200, 0.0,  2.0, 100, 0.0, 0.1);
      Neweff[5] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 2,  200, 0.0,  1.0);
      Neweff[6] -> add(postfix_det_, 1, 1, 10, 0,  postfix_cumeff_, 1, 1, 2,  200, 0.0,  1.0);

      // Scan histograms
      
      Scans_.push_back(new Histograms("delay",    2, 1));
      Scans_.push_back(new Histograms("rog",      4, 1));
      Scans_.push_back(new Histograms("hv_11mar", 2, 1));
      Scans_.push_back(new Histograms("hv_10apr", 2, 1));
      
      // Timing scans
      // eff
#ifdef COMPLETE
      Scans_[0] -> add(postfix_det_, 1, 1, 10, 0, postfix_effclu,  1, 1, 6,  34, -7.5, 26.5);
#else
      Scans_[0] -> add(postfix_det_, 1, 1, 10, 0, postfix_effclu,  1, 1, 4,  34, -7.5, 26.5);
#endif

      // rog level plots
#ifdef COMPLETE
      Scans_[1] -> add(postfix_BmpIO, 1, 1, 4,  0, postfix_sec,  1, 1, 8,  0, postfix_chn, 1, 1, 2, 0, postfix_effclu, 1, 1, 6,  34, -7.5, 26.5);
      Scans_[1] -> add(postfix_BmpIO, 0, 1, 4,  0, postfix_dsk,  1, 1, 2, -8, postfix_rog, 1, 1, 4, 0, postfix_effclu, 1, 1, 6,  34, -7.5, 26.5);
#else
      Scans_[1] -> add(postfix_BmpIO, 1, 1, 4,  0, postfix_sec,  1, 1, 8,  0, postfix_chn, 1, 1, 2, 0, postfix_effclu, 1, 1, 4,  34, -7.5, 26.5);
      Scans_[1] -> add(postfix_BmpIO, 0, 1, 4,  0, postfix_dsk,  1, 1, 2, -8, postfix_rog, 1, 1, 4, 0, postfix_effclu, 1, 1, 4,  34, -7.5, 26.5);
#endif
      // HV scans
      Scans_[2] -> add(postfix_det_,  1, 4,  4, 3, postfix_cumeff_,  1, 1, 2,  13, 25.0, 155.0);
      Scans_[2] -> add(postfix_det_,  1, 6,  6, 4, postfix_cumeff_,  1, 1, 2,  13, 25.0, 155.0);
      Scans_[2] -> add(postfix_det_,  1, 3,  3, 0, postfix_cumeff_,  1, 1, 2,  59, 12.5, 307.5);

      Scans_[3] -> add(postfix_det_,  1, 6,  6, 5, postfix_cumeff_,  1, 1, 2,  13, 25.0, 155.0);
      Scans_[3] -> add(postfix_det_,  1, 3,  3, 1, postfix_cumeff_,  1, 1, 2,  59, 12.5, 307.5);
    }
    if (clusthistos_==1) {
    }
  }

  bool cuts_traj_(EventData &evt, TrajMeasurement &trajmeas, 
		  int nvtx_sel, int vtxndof_sel, int trkq_sel, int pt_sel,  int nstrip_sel,
		  int d0_sel, int dz_sel, int eff_cond, int pixhit_cond, int excl_badmod, 
		  int excl_scans, int roc_sel, int ly_fid, int lx_fid, int neweff) {
    bool cut = true;
    if (nvtx_sel==1)
      cut = (evt.nvtx>=1) ? true : false;
    if (cut) {
      if (vtxndof_sel==1) cut = (evt.vtxndof>4) ? true : false;
    }
    if (cut) {
      if (trkq_sel==1)
        cut = ((trajmeas.trk.quality&4)!=0) ? true : false;
    }
    if (cut) {
      if (pt_sel==1)
        cut = ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
	       ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2)) ? true: false;
      if (pt_sel==2)
	cut = (trajmeas.trk.pt>0.6) ? true: false;
    }
    if (cut) {
      if (nstrip_sel==1)
        cut = (trajmeas.trk.strip>10) ? true : false;
    }
    if (cut) {
      if (d0_sel==1)
        cut = (((trajmeas.mod_on.det==0 && ((trajmeas.mod_on.layer==1 && fabs(trajmeas.trk.d0)<0.01) 
					    || (trajmeas.mod_on.layer!=1 && fabs(trajmeas.trk.d0)<0.02)))
		|| (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.d0)<0.05))) ? true: false;
    }
    if (cut) {
      if (dz_sel==1)
        cut = ((trajmeas.mod_on.det==0 && fabs(trajmeas.trk.dz)<0.1)
	       || (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.dz)<0.5)) ? true: false;
    }
    if (cut) {
      if (eff_cond==1) {
        cut = false;
        if ((((trajmeas.mod_on.layer==1&&trajmeas.lxmatch!=NOVAL_F)||trajmeas.mod_on.layer!=1)&&trajmeas.validhit==1)||trajmeas.missing==1) {
	  cut = true;
        }
      }
    }
    if (cut) {
      if (pixhit_cond==1) {
        cut = false;
	if (
	    ( trajmeas.mod_on.det==0
	      && ( 
		  ( trajmeas.mod_on.layer==1
		    && ( (trajmeas.trk.validbpix[1]==1&&trajmeas.trk.validbpix[2]==1)
			 || (trajmeas.trk.validbpix[1]==1&&trajmeas.trk.validfpix[0]==1)
			 || (trajmeas.trk.validfpix[0]==1&&trajmeas.trk.validfpix[1]==1) ) )
		  || 
		  ( trajmeas.mod_on.layer==2
		    && ( (trajmeas.trk.validbpix[0]==1&&trajmeas.trk.validbpix[2]==1)
			 || (trajmeas.trk.validbpix[0]==1&&trajmeas.trk.validfpix[0]==1) ) )
		  || 
		  ( trajmeas.mod_on.layer==3
		    && (trajmeas.trk.validbpix[0]==1&&trajmeas.trk.validbpix[1]==1) ) 
		  )
	      )
	    ||
	    ( trajmeas.mod_on.det==1
	      && ( 
		  ( ( trajmeas.mod_on.disk==1 || trajmeas.mod_on.disk==-1 )
		    && ( (trajmeas.trk.validbpix[0]==1&&trajmeas.trk.validfpix[1]==1)
			 || (trajmeas.trk.validbpix[1]==1&&trajmeas.trk.validfpix[1]==1) ) )
		  || 
		  ( ( trajmeas.mod_on.disk==2 || trajmeas.mod_on.disk==-2 )
		    && (trajmeas.trk.validbpix[0]==1&&trajmeas.trk.validfpix[0]==1) ) ) )
	    ) {
	  cut = true;
	}
      }
    }
    if (cut) {
      if (excl_badmod==1) {
        cut = false;
        if (!(trajmeas.mod_on.disk==-1&&trajmeas.mod_on.blade==6&&trajmeas.mod_on.panel==1))
	  if (!(trajmeas.mod_on.disk==-2&&trajmeas.mod_on.blade==10&&trajmeas.mod_on.panel==1&&evt.run>=146428))
	    if (!(trajmeas.mod_on.disk==2&&(trajmeas.mod_on.blade==4||trajmeas.mod_on.blade==5||trajmeas.mod_on.blade==6)&&evt.run>=147752))
	      cut = true;
      }
      if (excl_badmod==2) {
        cut = false;
	// FPix
	if (!(trajmeas.mod_on.disk==-1&&trajmeas.mod_on.blade==6&&trajmeas.mod_on.panel==1)) {
	  if (!(trajmeas.mod_on.disk==-2&&trajmeas.mod_on.blade==10&&trajmeas.mod_on.panel==1&&(evt.run>=143953||evt.run==137027||evt.run==137028))) {
	    // A whole readout group out
	    if (!(trajmeas.mod_on.disk==2&&(trajmeas.mod_on.blade==4||trajmeas.mod_on.blade==5||trajmeas.mod_on.blade==6)&&evt.run>=147752)) {
	      // Bpix
	      // Module with a problematic ROC, recovered
	      if(!(trajmeas.mod_on.layer==3&&trajmeas.mod_on.ladder==14&&trajmeas.mod_on.module==-4&&evt.run>=141956&&evt.run<160404)) {
		if(!(trajmeas.mod_on.layer==3&&((trajmeas.mod_on.ladder==13&&(trajmeas.mod_on.module==-1||trajmeas.mod_on.module==-2||trajmeas.mod_on.module==-3))
						||(trajmeas.mod_on.ladder==12&&(trajmeas.mod_on.module==-1||trajmeas.mod_on.module==-2))
						||(trajmeas.mod_on.ladder==-17&&trajmeas.mod_on.module==-3))&&evt.run>=162713)) {
		  // Excluded TBM-A/B
		  if(!(trajmeas.mod_on.layer==2&&trajmeas.mod_on.ladder==-13&&trajmeas.mod_on.module==3&&trajmeas.lx<0.0)) {
		    if(!(trajmeas.mod_on.layer==2&&trajmeas.mod_on.ladder==-8&&trajmeas.mod_on.module==1&&trajmeas.lx>0.0)) {
		      if(!(trajmeas.mod_on.layer==1&&trajmeas.mod_on.ladder==4&&trajmeas.mod_on.module==-4&&trajmeas.lx>0.0)) {
			cut = true;
		      }
		    }
		  }
		}
	      }
	    }
	  }
	}
      }
    }
    if (cut) {
      if (excl_scans==1) {
        cut = false;
	// HV scans
	if (!(((trajmeas.mod_on.det==0.&& ((trajmeas.mod_on.layer==1 && trajmeas.mod_on.module<0 &&  trajmeas.mod_on.ladder==-7)
					   || (trajmeas.mod_on.layer==3 && trajmeas.mod_on.module>0
					       && (trajmeas.mod_on.ladder==-4 || trajmeas.mod_on.ladder==-5))) ) 
	       || (trajmeas.mod_on.det==1 && trajmeas.mod_on.disk==-1
		   && (trajmeas.mod_on.blade==1 || trajmeas.mod_on.blade==2 || trajmeas.mod_on.blade==3 )
		   && ( (   trajmeas.mod_on.panel==1 && (trajmeas.mod_on.module==1 || trajmeas.mod_on.module==2) )
			|| (trajmeas.mod_on.panel==2 && trajmeas.mod_on.module==1) ) ))
	      &&(evt.run==160578||evt.run==160579||evt.run==160819||evt.run==160827||evt.run==160835))) {
	  // WBC scan
	  if(!(trajmeas.mod_on.layer==2&&trajmeas.mod_on.ladder==-5&&trajmeas.mod_on.module==2&&(evt.run==161310||evt.run==161311))) {
	    // delay scans
	    if (!(evt.run==160413||evt.run==160497||evt.run==160577||evt.run==160578))
	      cut = true;
	  }
	}
      }
    }
    if (cut) {
      if (roc_sel==1)
        cut = true;
    }
    if (cut) {
      if (ly_fid==1) {
        cut = false;
        if ( (trajmeas.mod_on.det==0 && fabs(trajmeas.ly)<3.0)
  	   || 
  	   (trajmeas.mod_on.det==1 
  	    && (
  		(trajmeas.mod_on.panel==1 
  		 && (
  		     ( (trajmeas.mod_on.module==1 && fabs(trajmeas.ly)<0.7)
  		       && !(trajmeas.mod_on.disk==-1 && trajmeas.ly>0.35) )
  		     || ( (trajmeas.mod_on.module==2 && fabs(trajmeas.ly)<1.1)
  			  && !(trajmeas.mod_on.disk==-1 && fabs(trajmeas.ly)>1.0)
  			  && !(trajmeas.mod_on.disk==-1 && trajmeas.lx<0.0 && trajmeas.ly>0.75) )
  		     || ( (trajmeas.mod_on.module==3 && fabs(trajmeas.ly)<1.5)
  			  && !(trajmeas.mod_on.disk==-1 && fabs(trajmeas.ly)>1.4)
  			  && !(trajmeas.mod_on.disk==-1 && trajmeas.lx<0.0 && trajmeas.ly>1.1) )
  		     || ( (trajmeas.mod_on.module==4 && fabs(trajmeas.ly)<1.9) )
  		     )
  		 )
  		|| 
  		(trajmeas.mod_on.panel==2
  		 && (
  		     (trajmeas.mod_on.module==1 && fabs(trajmeas.ly)<1.1)
  		     || (trajmeas.mod_on.module==2 && fabs(trajmeas.ly)<1.5)
  		     || (trajmeas.mod_on.module==3 && fabs(trajmeas.ly)<1.9)
  		     )
  		 )
  		)
  	    )
  	   ) {
  	cut = true;
        }
      }
    }
    if (cut) {
      if (lx_fid==1) {
        cut = false;
        if ( (trajmeas.mod_on.det==0 
  	    && ( 
  		(trajmeas.mod_on.layer==1
  		 && ( 
  		     (trajmeas.mod_on.half==0 && (fabs(trajmeas.lx)<0.6 && fabs(trajmeas.lx)>0.05))
  		     || (trajmeas.mod_on.half==1 && (trajmeas.lx<0.3 && trajmeas.lx>-0.38)) 
  		     ) 
  		 )
  		|| 
  		(trajmeas.mod_on.layer==2
  		 && (
  		     (trajmeas.mod_on.half==0 && (fabs(trajmeas.lx)<0.6 && fabs(trajmeas.lx)>0.05))
  		     || (trajmeas.mod_on.half==1 && (trajmeas.lx<0.36 && trajmeas.lx>-0.38)) 
  		     ) 
  		 )
  		|| 
  		(trajmeas.mod_on.layer==3
  		 && (
  		     (trajmeas.mod_on.half==0 && (fabs(trajmeas.lx)<0.6 && fabs(trajmeas.lx)>0.05))
  		     || (trajmeas.mod_on.half==1 && (trajmeas.lx<0.36 && trajmeas.lx>-0.38)) 
  		     ) 
  		 )
  		)
  	    )
  	   ||
  	   (trajmeas.mod_on.det==1
  	    && (
  		(trajmeas.mod_on.panel==1 
  		 && (
  		     (trajmeas.mod_on.disk==-2
  		      && (
  			  (trajmeas.mod_on.module==1 && trajmeas.lx<0.38 && trajmeas.lx>-0.38)
  			  || (trajmeas.mod_on.module==2 && trajmeas.lx<0.75 && trajmeas.lx>-0.75)
  			  || (trajmeas.mod_on.module==3 && trajmeas.lx<0.75 && trajmeas.lx>-0.75)
  			  || (trajmeas.mod_on.module==4 && trajmeas.lx<0.38 && trajmeas.lx>-0.38)
  			  )
  		      )
  		     ||
  		     (trajmeas.mod_on.disk==-1
  		      && (
  			  (trajmeas.mod_on.module==1 && trajmeas.lx<0.38 && trajmeas.lx>-0.15)
  			  || (trajmeas.mod_on.module==2 && trajmeas.lx<0.6 && trajmeas.lx>-0.55)
  			  || (trajmeas.mod_on.module==3 && trajmeas.lx<0.6 && trajmeas.lx>-0.5)
  			  || (trajmeas.mod_on.module==4 && trajmeas.lx<0.2 && trajmeas.lx>-0.38)
  			  )
  		      )
  		     ||
  		     (trajmeas.mod_on.disk==1
  		      && (
  			  (trajmeas.mod_on.module==1 && trajmeas.lx<0.38 && trajmeas.lx>-0.38)
  			  || (trajmeas.mod_on.module==2 && trajmeas.lx<0.75 && trajmeas.lx>-0.75)
  			  || (trajmeas.mod_on.module==3 && trajmeas.lx<0.75 && trajmeas.lx>-0.75)
  			  || (trajmeas.mod_on.module==4 && trajmeas.lx<0.38 && trajmeas.lx>-0.38)
  			  )
  		      )
  		     ||
  		     (trajmeas.mod_on.disk==2
  		      && (
  			  (trajmeas.mod_on.module==1 && trajmeas.lx<0.38 && trajmeas.lx>-0.38)
  			  || (trajmeas.mod_on.module==2 && trajmeas.lx<0.75 && trajmeas.lx>-0.75)
  			  || (trajmeas.mod_on.module==3 && trajmeas.lx<0.75 && trajmeas.lx>-0.75)
  			  || (trajmeas.mod_on.module==4 && trajmeas.lx<0.38 && trajmeas.lx>-0.38)
  			  )
  		      )
  		     )
  		 )
  		|| 
  		(trajmeas.mod_on.panel==2 
  		 &&(
  		    fabs(trajmeas.lx)<0.75
  		    && !(trajmeas.mod_on.disk==-1 && trajmeas.mod_on.module==3 && trajmeas.ly>0.7)
  		    )
  		 )
  		)
  	    )
  	   ) {
  	cut = true;
        }
      }
    }
    if (cut) {
      if (neweff==1)
        cut = (trajmeas.dx_hit==NOVAL_F||sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit)>0.5) ? true : false;
    }
    return cut;
  }

  void efficiency_fill_(EventData &evt, TrajMeasurement &trajmeas) {
    if (allcuts_) {
      Efficiency_[0]->eff_fill(trajmeas, det_x_);
      Efficiency_[1]->hist_eff_fill(modroc_p1_, trajmeas, mod_x_, mod_y_);
    }
    if (inac_==1&&roc_binx_!=NOVAL_I&&roc_biny_!=NOVAL_I) {
      Efficiency_[2]->hist_increasebin(modroc_p1_, 2, trajmeas, roc_binx_, roc_biny_);
      if (trajmeas.mod_on.det==1) Efficiency_[2]->hist_increasebin(modroc_p1_, 2, trajmeas, roc_binx_, roc_biny_+1);
    }
    if (cut_roc_) {
      Efficiency_[2]->hist_eff_increasebin(modroc_p1_, trajmeas, roc_binx_, roc_biny_);
      if (trajmeas.mod_on.det==1) Efficiency_[2]->hist_eff_increasebin(modroc_p1_, trajmeas, roc_binx_, roc_biny_+1);
    }
    if (cut_run_) {
      if (runno_.size()!=0) {
	if (evt.run!=runno_[runno_.size()-1]) {
	  runno_.push_back(evt.run);
	  run_det_valmis_.push_back(std::vector<std::vector<double> >());
	  for (size_t i=0; i<10; i++) {
	    // size of posfix_det_
	    run_det_valmis_[runno_.size()-1].push_back(std::vector<double>());
	    for (size_t j=0; j<2; j++) {
	      // 0: val 1: mis
	      run_det_valmis_[runno_.size()-1][i].push_back(0.0);  
	    }
	  }
	}
      } else {
	runno_.push_back(evt.run);
	run_det_valmis_.push_back(std::vector<std::vector<double> >());
	for (size_t i=0; i<10; i++) {
	  // size of posfix_det_
	  run_det_valmis_[0].push_back(std::vector<double>());
	  for (size_t j=0; j<2; j++) {
	    // 0: val 1: mis
	    run_det_valmis_[0][i].push_back(0.0);  
	  }
	}
      }
      nrun_ = runno_.size();
      if (trajmeas.validhit==1||trajmeas.missing==1) {
	run_det_valmis_[nrun_-1][0][trajmeas.missing]+=1.0;
	run_det_valmis_[nrun_-1][1+trajmeas.mod_on.det][trajmeas.missing]+=1.0;
	run_det_valmis_[nrun_-1][(trajmeas.mod_on.det==0) ? 2+trajmeas.mod_on.layer : 7+trajmeas.mod_on.disk+(trajmeas.mod_on.disk<0)][trajmeas.missing]+=1.0;
      }
    }
  }
  
  void run_calc_() {
    Efficiency_.push_back(new Histograms("run",      2, 1));
    Efficiency_[3] -> add(postfix_det_,   1, 1, 10, 0,  postfix_cumeff_, 1, 1, 2, nrun_, 0.5, nrun_+0.5);
    int lastfilled = 0;
    for (int k=0; k<nrun_; k++) {
      int lowest = nrun_-1;
      int lowestrun = 999999;
      for (int j=0; j<nrun_; j++) {
	if (runno_[j]<lowestrun&&runno_[j]>lastfilled) {
	  lowest = j;
	  lowestrun = runno_[j];
	}
      }
      lastfilled = lowestrun;
      std::stringstream sstr;
      sstr<<lowestrun;
      for (int i=0; i<10; i++) { 
	for (int j=0; j<2; j++) {
	  Efficiency_[3]->hist_setbincontent(i, j, k+1, run_det_valmis_[lowest][i][j]);
	  Efficiency_[3]->hist_setbinlabelx(i, j, k+1, sstr.str().c_str());
	}
      }
    }
  }

  // N-1
  void nminusone_fill_(EventData &evt, TrajMeasurement &trajmeas) {
    if (nminus_pt_)      Nminusone_[0]  -> det_eff_fill(trajmeas, trajmeas.trk.pt);
    if (nminus_nstrip_)  Nminusone_[1]  -> det_eff_fill(trajmeas, trajmeas.trk.strip);
    if (nminus_dz_)      Nminusone_[2]  -> det_eff_fill(trajmeas, fabs(trajmeas.trk.dz));
    if (nminus_d0_)      Nminusone_[3]  -> det_eff_fill(trajmeas, fabs(trajmeas.trk.d0));
    if (nminus_dz_d0_)   Nminusone_[4]  -> det_eff_fill(trajmeas, fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
    if (nminus_lxlyfid_) Nminusone_[6]  -> hist_eff_fill(p1_fid_, p2_fid_, trajmeas, trajmeas.ly, trajmeas.lx);
    if (nminus_lxfid_)   Nminusone_[7]  -> hist_eff_fill(p1_fid_, p2_fid_, trajmeas, trajmeas.lx);
    if (nminus_lyfid_)   Nminusone_[8]  -> hist_eff_fill(p1_fid_, p2_fid_, trajmeas, trajmeas.ly);
#ifdef COMPLETE
    if (nminus_nvtx_) {
      Nminusone_[9] -> hist_eff_fill(0, trajmeas, fabs(evt.vtxD0));
      Nminusone_[9] -> hist_eff_fill(trajmeas.mod_on.det+1, trajmeas, fabs(evt.vtxD0));
      Nminusone_[10]  -> hist_eff_fill(0, trajmeas, fabs(evt.vtxZ));
      Nminusone_[10]  -> hist_eff_fill(trajmeas.mod_on.det+1, trajmeas, fabs(evt.vtxZ));
    }
#endif
  }

  // Scans_
  void scans_traj_fill_(EventData &evt, TrajMeasurement &trajmeas) {
    // delay scan 2011 March
    if (delay_x_!=NOVAL_I) {
      // Coarse
      if (cut_scans_)               Scans_[0]->det_eff_fill(trajmeas, delay_x_);
      if (trajmeas.clu.charge>12.0) Scans_[0]->det_clusize_fill(trajmeas, delay_x_, trajmeas.clu.size);
      // Fine - by ROG
      if (cut_scans_)               Scans_[1]->hist_eff_fill(p1_bmpio_, p2_secdisk_, p3_layrog_, trajmeas, delay_x_);
      if (trajmeas.clu.charge>12.0) Scans_[1]->hist_clusize_fill(p1_bmpio_, p2_secdisk_, p3_layrog_, trajmeas, delay_x_, trajmeas.clu.size);
    }
    // voltage scan 2011 March
    if (hv_11mar_x_!=NOVAL_I) {
      if (cut_scans_) {
	if ((trajmeas.mod_on.det==0.&& ((trajmeas.mod_on.layer==1 && trajmeas.mod_on.module<0 &&  trajmeas.mod_on.ladder==-7)
					|| (trajmeas.mod_on.layer==3 && trajmeas.mod_on.module>0 
					    && (trajmeas.mod_on.ladder==-4 || trajmeas.mod_on.ladder==-5))) ) 
	    || (trajmeas.mod_on.det==1 && trajmeas.mod_on.disk==-1
		&& (trajmeas.mod_on.blade==1 || trajmeas.mod_on.blade==2 || trajmeas.mod_on.blade==3 )
		&& ( (   trajmeas.mod_on.panel==1 && (trajmeas.mod_on.module==1 || trajmeas.mod_on.module==2) )
		     || (trajmeas.mod_on.panel==2 && trajmeas.mod_on.module==1) ) ) ) {
	  Scans_[2]->hist_eff_fill(p1_hv_11mar_, trajmeas, hv_11mar_x_);
	}
      }
    }
    // voltage scan 2010 April
    if (hv_10apr_x_!=NOVAL_I) {
      if (cut_scans_) {
	if ((trajmeas.mod_on.det==0  && trajmeas.mod_on.layer==3 
	     && trajmeas.mod_on.module>0  && (trajmeas.mod_on.ladder==-4 || trajmeas.mod_on.ladder==-5))
	    || (trajmeas.mod_on.det==1 && trajmeas.mod_on.disk==-1
		&& (trajmeas.mod_on.blade==1 || trajmeas.mod_on.blade==2 || trajmeas.mod_on.blade==3 )
		&& ( (   trajmeas.mod_on.panel==1 && (trajmeas.mod_on.module==1 || trajmeas.mod_on.module==2) )
		     || (trajmeas.mod_on.panel==2 && trajmeas.mod_on.module==1) ) ) ) {
	  Scans_[3]->hist_eff_fill(p1_hv_10apr_, trajmeas, hv_10apr_x_);
	}
      }
    }
  }

#ifdef COMPLETE
  void scans_clust_fill_(EventData &cluevt, Cluster &clust) {
    int delay_x = clust_delay11_(cluevt);
    int p1_bmpio = (clust.mod_on.det==0) ? 2 * (clust.mod_on.module<0) + (clust.mod_on.ladder>0) 
      : 2 * (clust.mod_on.disk<0) + (clust.mod_on.blade>0);
    int p2_secdisk = (clust.mod_on.det==0) ? clust.mod_on.sec - 1 : 7 + abs(clust.mod_on.disk) ;
    int p3_layrog = (clust.mod_on.det==0) ? (clust.mod_on.layer==3) : (abs(clust.mod_on.blade)-1)/3;
    if (delay_x!=NOVAL_I&&clust.charge>12.0) {
      Scans_[0]->det_clusize_fill(clust, delay_x, clust.size);
      if (delay_x_>=12&&delay_x_<=18) Scans_[1]->hist_clusize_fill(p1_bmpio, p2_secdisk, p3_layrog, clust, delay_x, clust.size);
    }
  }
#endif

  void neweff_redef(EventData &evt, TrajMeasurement &trajmeas) {
    if (trajmeas.missing==1&&sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]+trajmeas.dy_cl[0]*trajmeas.dy_cl[0])<0.05) {
      trajmeas.missing=0;
      trajmeas.validhit=1;
    }
  }

 public:

  void apply_cuts_traj(EventData &evt, TrajMeasurement &trajmeas, int neweff) {
    // Efficiency                               nvtx, vtxndof, trkq, pt, nstrip, d0, dz, eff_cond, pixhit_cond, excl_badmod, excl_scans, roc_sel, ly_fid, lx_fid
    allcuts_        = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        1,           2,           1,          1,       1,      1, neweff);
    cut_roc_        = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        0,           2,           1,          1,       1,      1, neweff);
    cut_run_        = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        1,           2,           0,          1,       1,      1, neweff);
    // N-1
    nminus_pt_      = cuts_traj_(evt, trajmeas, 1,    1,       1,    0,  1,      1,  1,  1,        1,           2,           1,          1,       1,      1, neweff);
    nminus_nstrip_  = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  0,      1,  1,  1,        1,           2,           1,          1,       1,      1, neweff);
    nminus_d0_      = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      0,  1,  1,        1,           2,           1,          1,       1,      1, neweff);
    nminus_dz_      = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  0,  1,        1,           2,           1,          1,       1,      1, neweff);
    nminus_dz_d0_   = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      0,  0,  1,        1,           2,           1,          1,       1,      1, neweff);
    nminus_lxlyfid_ = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        1,           2,           1,          1,       0,      0, neweff);
    nminus_lxfid_   = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        1,           2,           1,          1,       1,      0, neweff);
    nminus_lyfid_   = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        1,           2,           1,          1,       0,      1, neweff);
    nminus_nvtx_    = cuts_traj_(evt, trajmeas, 0,    1,       1,    1,  1,      1,  1,  1,        1,           2,           1,          1,       1,      1, neweff);
    // Scans
    cut_scans_      = cuts_traj_(evt, trajmeas, 1,    1,       1,    2,  1,      1,  1,  1,        1,           2,           0,          1,       1,      1, neweff);
    // Neweff
//     neweff_hh_      = cuts_traj_(evt, trajmeas, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0);
  }

  void fill_traj(EventData &evt, TrajMeasurement &trajmeas, int neweff) {
    variables(evt, trajmeas);
    if (neweff==1) neweff_redef(evt, trajmeas);
    efficiency_fill_(evt, trajmeas);
    nminusone_fill_(evt, trajmeas);
    scans_traj_fill_(evt, trajmeas);
  }

#ifdef COMPLETE
  void fill_clust(EventData &cluevt, Cluster &clust, int neweff) {
    scans_clust_fill_(cluevt, clust);
  }
#endif

  void calc_write() {
    // Efficiency
    run_calc_();
    Efficiency_[0] -> eff_error_write();
    Efficiency_[1] -> eff_error_write();
    Efficiency_[2] -> eff_error_write();
    Efficiency_[3] -> eff_error_write();
    // N-1
    Nminusone_[0]  -> cumeff_error_write(false);
    Nminusone_[1]  -> cumeff_error_write(false);
    Nminusone_[2]  -> cumeff_error_write(true);
    Nminusone_[3]  -> cumeff_error_write(true);
    Nminusone_[4]  -> cumeff_error_write(true, true);
    Nminusone_[6]  -> eff_error_write();
    Nminusone_[7]  -> eff_error_write();
    Nminusone_[8]  -> eff_error_write();
#ifdef COMPLETE
    Nminusone_[9]  -> eff_error_write();
    Nminusone_[10] -> eff_error_write();
#endif
    // Scans
    Scans_[0]      -> effclu_error_write();
    Scans_[1]      -> effclu_error_write();
    Scans_[2]      -> eff_error_write();
    Scans_[3]      -> eff_error_write();
  }

  // Accessors
 public:
  std::vector<Histograms*> Efficiency() { return Efficiency_; }
  std::vector<Histograms*> Nminusone() { return Nminusone_; }
  std::vector<Histograms*> Scans() { return Scans_; }
  
};

class Analysis {

 public:

  Analysis(int trajloop, int clustloop, int neweff) {
    trajloop_=trajloop;
    clustloop_=clustloop;
    neweff_=neweff;
    build_();
  }
  ~Analysis() { }

 private:
  
  int trajloop_;
  int clustloop_;
  int neweff_;
  TChain* filechain_;
  TChain* trajchain_;
  TChain* eventchain_;
  AllHistos* allhistos_;

  //Declare datasets
  EventData evt_,cluevt_;
  TrajMeasurement trajmeas_;
  // TrackData track;
  Cluster clust_;
  // Digi digi;

  long long int trajhits_;
  long long int clusters_;

  int nfile_;
  long long int allhits_;
  long long int allevents_;
  int totalevent_;
  int totallumi_;
  long long int totalhit_;
  int percentdone_;
  int lscheck_;
#ifdef COMPLETE
  int evtcheck_;
#else
  int nvtxcheck_;
  int runcheck_;
  float vtxndofcheck_;
#endif
  int nlumi_;
  int nevent_;

  const char* effstudyout_;

  void build_() {
    allhistos_ = new AllHistos(trajloop_,clustloop_);
    filechain_ = new TChain("filechain");
    trajchain_ = new TChain("trajchain");
    eventchain_ = new TChain("eventchain");
    nfile_ = 0;
    totalevent_ = 0;
    totallumi_ = 0;
    totalhit_ = 0;
    percentdone_ = 0;
    lscheck_ = 0;
#ifdef COMPLETE
    evtcheck_ = 0;
#else
    nvtxcheck_ = 0;
    runcheck_ = 0;
    vtxndofcheck_ = 0.0;
#endif
    nlumi_ = 0;
    nevent_ = 0;
  }

  void run_progress(int progress) {
    if (progress==0) {
      allhits_ = (long long int)trajchain_->GetEntries();
      delete trajchain_;
      allevents_ = (long long int)eventchain_->GetEntries();
      delete eventchain_;
      std::cout<<"--------------------------- Total number of events : "<<allevents_<<std::endl;
      std::cout<<"--------------------------- Total number of hits   : "<<allhits_<<std::endl;
#ifdef COMPLETE
      // /home/jkarancs/data/COMPLETE/MB_Run2011A_RECO_049.root - 1140 MB - 4520670 hits
      // unbuffered - 0:40.57 - 28.1 MB/s
      //   buffered - 0:38.57, 0:35.14, 0:41.98, 0:35.58, 0:35.20 - up to 32.4 Mb/s
      float totaltime   = allhits_ / 111429.0;
      float totaltimeb  = allhits_ / 121217.0;
#else
      // /home/jkarancs/data/MB_Run2011A_RECO_054.root - 2890.8 MB - 34926737 hits
      // unbuffered - 2:57.99          -       16.2 MB/s
      //   buffered - 2:53.77, 2:52.80 - up to 16.7 Mb/s
      // same file on safedata - 2 processes
      // unbuffered - 4:21.66          -       11.0 MB/s
      //   buffered - 3:06.86, 3:01.10 - up to 16.0 MB/s
      // 053 on safedata - 2 processes - 2937.7 MB - 35484563 hits
      // unbuffered - 5:03.08          -        9.7 MB/s
      // 055 on safedata - 2 processes - 2838.0 MB - 34301109 hits
      // unbuffered - 3:40.57          -       12.9 MB/s
      float totaltime   = allhits_ / 196228.0;
      float totaltimeb  = allhits_ / 201557.0;
      float totaltime2  = allhits_ / 133339.0;
      float totaltime2b = allhits_ / 189840.0;
      // previous unbuffered long test on /data 196635.0
      int time2_h = 0;
      int time2_m = 0;
      int time2_s = 0;
      int time2b_h = 0;
      int time2b_m = 0;
      int time2b_s = 0;
      if (totaltime2 > 3600.0) {
	while (totaltime2>3600.0) { totaltime2 -= 3600.0; time2_h++; }
	while (totaltime2>60.0) { totaltime2 -= 60.0; time2_m++; }
	while (totaltime2b>3600.0) { totaltime2b -= 3600.0; time2b_h++; }
	while (totaltime2b>60.0) { totaltime2b -= 60.0; time2b_m++; }
	std::cout<<"--------------------------- Time  needed (2 proc)  : "<<time2_h<<" hours "<<time2_m<<" minutes ("<<time2b_h<<":"<<time2b_m<<" )"<<std::endl;
      } else if (totaltime2 > 60.0) {
	while (totaltime2>60.0) { totaltime2 -= 60.0; time2_m++; }
	while (totaltime2>1.0) { totaltime2 -= 1.0; time2_s++;}
	while (totaltime2b>60.0) { totaltime2b -= 60.0; time2b_m++; }
	while (totaltime2b>1.0) { totaltime2b -= 1.0; time2b_s++;}
	std::cout<<"--------------------------- Time  needed (2 proc)  : "<<time2_m<<" minutes "<<time2_s<<" seconds ( "<<time2b_m<<":"<<time2b_s<<" )"<<std::endl;
      } else {
	while (totaltime2>1.0) { totaltime2 -= 1.0; time2_s++; }
	while (totaltime2b>1.0) { totaltime2b -= 1.0; time2b_s++; }
	std::cout<<"--------------------------- Time  needed (2 proc)  : "<<time2_s<<" seconds ( "<<time2b_s<<" )"<<std::endl;
      }
#endif
      int time_h = 0;
      int time_m = 0;
      int time_s = 0;
      int timeb_h = 0;
      int timeb_m = 0;
      int timeb_s = 0;
      if (totaltime > 3600.0) {
	while (totaltime>3600.0) { totaltime -= 3600.0; time_h++; }
	while (totaltime>60.0) { totaltime -= 60.0; time_m++; }
	while (totaltimeb>3600.0) { totaltimeb -= 3600.0; timeb_h++; }
	while (totaltimeb>60.0) { totaltimeb -= 60.0; timeb_m++; }
	std::cout<<"--------------------------- Time  needed (1 proc)  : "<<time_h<<" hours "<<time_m<<" minutes ("<<timeb_h<<":"<<timeb_m<<" )"<<std::endl;
      } else if (totaltime > 60.0) {
	while (totaltime>60.0) { totaltime -= 60.0; time_m++; }
	while (totaltime>1.0) { totaltime -= 1.0; time_s++;}
	while (totaltimeb>60.0) { totaltimeb -= 60.0; timeb_m++; }
	while (totaltimeb>1.0) { totaltimeb -= 1.0; timeb_s++;}
	std::cout<<"--------------------------- Time  needed (1 proc)  : "<<time_m<<" minutes "<<time_s<<" seconds ( "<<timeb_m<<":"<<timeb_s<<" )"<<std::endl;
      } else {
	while (totaltime>1.0) { totaltime -= 1.0; time_s++; }
	while (totaltimeb>1.0) { totaltimeb -= 1.0; timeb_s++; }
	std::cout<<"--------------------------- Time  needed (1 proc)  : "<<time_s<<" seconds ( "<<timeb_s<<" )"<<std::endl;
      }
    } else if (progress==1) {
      nfile_++;
      // std::cout<<"Opened file "<<nfile_<<" which contains "<<trajhits_<<" hits and "<<clusters_<<" clusters."<<std::endl;
    } else if (progress==2) {
      totalhit_++;
      if (allhits_>300000000) {
	if (totalhit_>(allhits_/100*(percentdone_+1))) {
	  percentdone_++;
	  std::cout<<"--------------------------- "<<percentdone_<<"% done ---------------------------"<<std::endl;
	}
      } else {
	if (totalhit_>(allhits_/100*(percentdone_+10))) {
	  percentdone_+=10;
	  std::cout<<"--------------------------- "<<percentdone_<<"% done ---------------------------"<<std::endl;
	}
      }
#ifdef COMPLETE
      if (evt_.evt!=evtcheck_) {
	nevent_++;
	totalevent_++;
	if (evt_.ls!=lscheck_)  {
	  nlumi_++;
	  totallumi_++;
	}
      }
      evtcheck_ = evt_.evt;
      lscheck_ = evt_.ls;
#else
      if (!(evt_.ls==lscheck_&&evt_.nvtx==nvtxcheck_&&evt_.run==runcheck_&&evt_.vtxndof==vtxndofcheck_)) {
	nevent_++;
	totalevent_++;
	if (evt_.ls!=lscheck_)  {
	  nlumi_++;
	  totallumi_++;
	}
      }
      nvtxcheck_ = evt_.nvtx;
      vtxndofcheck_ = evt_.vtxndof;
      lscheck_ = evt_.ls;
      runcheck_ = evt_.run;
#endif
    } else if (progress==3) {
      std::cout<<"Finished processing "<<nfile_<<" files."<<std::endl;
      std::cout<<"Total lumisections = "<<totallumi_<<std::endl;  
      std::cout<<"Total Processed events = "<<totalevent_<<" Total Processed hits = "<<totalhit_<<std::endl;
      std::cout<<"---------------------------------------------------------"<<std::endl;
    }
  }

 public:

  void addfile(std::string fileaddress) {
    filechain_->Add(fileaddress.c_str());
    std::string trajaddress = fileaddress + "/trajTree";
    trajchain_->Add(trajaddress.c_str());
    std::string eventaddress = fileaddress + "/eventTree";
    eventchain_->Add(eventaddress.c_str());
  }

  void fill_histograms() {
    run_progress(0);
    TObjArray *fileElements=filechain_->GetListOfFiles();
    TIter next(fileElements);
    TChainElement *chEl=0;
    while (( chEl=(TChainElement*)next() )) {
      TFile f(chEl->GetTitle());
      // load the tree from specified address and get their branches,
      // then set their address compatible with data structures
      TTree *trajTree = (TTree*)f.Get("trajTree");
      trajhits_ = (long long int)trajTree->GetEntries();
      TBranch *b_t_evt_ = trajTree->GetBranch("event");
      b_t_evt_->SetAddress(&evt_);
      TBranch *b_t_traj_ = trajTree->GetBranch("traj");
      b_t_traj_->SetAddress(&trajmeas_);
      TBranch *b_t_mod_on_ = trajTree->GetBranch("module_on");
      b_t_mod_on_->SetAddress(&trajmeas_.mod_on);
      TBranch *b_t_clu_ = trajTree->GetBranch("clust");
      b_t_clu_->SetAddress(&trajmeas_.clu);
      TBranch *b_t_trk_ = trajTree->GetBranch("track");
      b_t_trk_->SetAddress(&trajmeas_.trk);
      clusters_ = 0;
#ifdef COMPLETE
      TBranch *b_t_mod = trajTree->GetBranch("module");
      b_t_mod->SetAddress(&trajmeas_.mod);
      TTree *clustTree = (TTree*)f.Get("clustTree");
      clusters_ = (long long int)clustTree->GetEntries();
      TBranch *b_c_evt = clustTree->GetBranch("event");
      b_c_evt->SetAddress(&cluevt_);
      TBranch *b_c_mod = clustTree->GetBranch("module");
      b_c_mod->SetAddress(&clust_.mod);
      TBranch *b_c_mod_on = clustTree->GetBranch("module_on");
      b_c_mod_on->SetAddress(&clust_.mod_on);
      TBranch *b_c_clu = clustTree->GetBranch("clust");
      b_c_clu->SetAddress(&clust_);
#endif
      run_progress(1);
      if (trajloop_==1) {
	for (long long int i=0; i<trajhits_; i++) {
	  trajTree->GetEntry(i);
	  run_progress(2);
	  allhistos_->apply_cuts_traj(evt_,trajmeas_,neweff_);
	  allhistos_->fill_traj(evt_,trajmeas_,neweff_);
	}
      }
#ifdef COMPLETE
      if (clustloop_==1) {
	for (long long int i=0;i<clusters_;i++) {
	  clustTree->GetEntry(i);
	  allhistos_->fill_clust(cluevt_,clust_,neweff_);
	}
      }
#else
      if (clustloop_==1)
	std::cout<<"Error: COMPLETE not defined. Not running on clustTree"<<std::endl;
#endif
    }
    run_progress(3);
    delete filechain_;
  }

  void write_output(std::string strrundesc, std::string strrunname, std::string version) {
    std::string strcomp = "";
#ifdef COMPLETE
    strcomp = "_comp";
#endif
    std::string streff;
    if (neweff_==1)
      streff = "_neweff";
    else
      streff = "_oldeff";
    
    std::string effoutfn = strrundesc + "_" + strrunname + streff + "_" + version + strcomp + ".root";
    effstudyout_ = effoutfn.c_str();
    
    TFile *fo= new TFile(effstudyout_,"recreate");
    allhistos_->calc_write();
    fo->Close();
    std::cout<<"File: "<<effoutfn<<" ready."<<std::endl;
  }

  // Accessors
 public:
  AllHistos* allhistos() { return allhistos_; }
  const char* getoutputfn() { return effstudyout_; }

};

int main() {

  // trajloop, clustloop, neweff
  Analysis effanalysis(1, 0, 1);




  // FIRE

  // // TEST
  // // // INCOMPLETE
//   effanalysis.addfile("/afs/cern.ch/user/j/jkarancs/Grid/CMSSW_4_1_2/src/DPGAnalysis/PixelTimingStudy/HLT_test.root");
  // effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/v1816modtraj/INCOMPLETE/allscans/MB_Run2011A_RECO_057.root");
  // effanalysis.addfile("/home/jkarancs/data/MB_Run2011A_RECO_054.root");
  //  effanalysis.addfile("/home/jkarancs/data/MB_Run2011A_RECO_049.root");
  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/test/*.root");
  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/test/MB_Run2011A_RECO_049.root");


  // // // COMPLETE
  // effanalysis.addfile("/home/jkarancs/data/COMPLETE/MB_Run2011A_RECO_049.root");



  // // DATA
  // // // COMPLETE
  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/finedelay/*.root");


  // // // INCOMPLETE
  /* 2010A & 2010B */
//   effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010A-Dec22ReReco_v1_RECO/v1816modtraj/INCOMPLETE/2010A_Dec22ReReco_0*.root");
  effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010A-Dec22ReReco_v1_RECO/v1816modtraj/INCOMPLETE/*.root");
  effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/v1816modtraj/INCOMPLETE/*.root");

  /* full 2011 */
  effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/v1816modtraj/INCOMPLETE/allscans/*.root");
  effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/v1816modtraj/INCOMPLETE/goodruns1/*.root");
  effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/v1816modtraj/INCOMPLETE/goodruns2/*.root");
  effanalysis.addfile("/safedata/jkarancs/data/gridout/CMSSW_4_1_4_patch2/MinimumBias_Run2011A-PromptReco-v2_RECO/v1816modtraj/INCOMPLETE/goodruns3/*.root");

  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/*.root");

  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/goodruns/*.root");
  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/goodruns2/*.root");
  //  effanalysis.addfile("/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/goodruns3/*.root");




  // Pcatomki01  - 2010A - 2010B
  // // // INCOMPLETE

  /* 2010A - 2010B */
  //   effanalysis.addfile("/home/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010A-Dec22ReReco_v1_RECO/v1816modtraj/*.root");
  //   effanalysis.addfile("/data/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010A-Dec22ReReco_v1_RECO/v1816modtraj/*.root");
  //   effanalysis.addfile("/data/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/v1816modtraj/*.root");
  



//   effanalysis.fill_histograms();

//   effanalysis.write_output("2010and2011runs","total7TeV","v1816mod");

  CanvasMaker allcanvas(effanalysis.getoutputfn(),"canvastest.root");
  allcanvas.gethistodata(effanalysis.allhistos()->Efficiency());
  allcanvas.gethistodata(effanalysis.allhistos()->Nminusone());
  allcanvas.gethistodata(effanalysis.allhistos()->Scans());
  allcanvas.create_canvases();
  //   CanvasMaker allcanvas(effanalysis.getoutputfn(),effanalysis.getoutputfn());
  //   CanvasMaker allcanvas("2011runs_excl_allscans_goodruns_neweff_v1816mod.root","2011runs_excl_allscans_goodruns_neweff_v1816mod.root");
  // effanalysis.getoutputfn()

  return 1;
}


