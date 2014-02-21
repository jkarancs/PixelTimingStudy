#include <map>

TH1D* getplot(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH1D *h= (TH1D*)f.FindObjectAny(hname.Data());
  TH1D *c=h->Clone();
  c->SetDirectory(0);
  return c;
}

TH2D* getplot2d(TString fname, TString hname) {
  TFile f(fname.Data(), "READ");
  TH2D *h= (TH2D*)f.FindObjectAny(hname.Data());
  TH2D *c=h->Clone();
  c->SetDirectory(0);
  return c;
}

int get_online_ladder(int layer, int ladder) {
  // copied exactly from TimingStudy::getModuleData
  int on_ladder;
  if (layer==1) {
    if (ladder<=5 && ladder>=1) on_ladder = 6-ladder;
    else if (ladder<=15 && ladder>=6) on_ladder = 5-ladder;
    else if (ladder<=20 && ladder>=16) on_ladder = 26-ladder;
  } else if (layer==2) {
    if (ladder<=8 && ladder>=1) on_ladder = 9-ladder;
    else if (ladder<=24 && ladder>=9) on_ladder = 8-ladder;
    else if (ladder<=32 && ladder>=25) on_ladder = 41-ladder;
  } if (layer==3) {
    if (ladder<=11 && ladder>=1) on_ladder = 12-ladder;
    else if (ladder<=33 && ladder>=12) on_ladder = 11-ladder;
    else if (ladder<=44 && ladder>=34) on_ladder = 56-ladder;
  }
  return on_ladder;
}

void fitter(bool draw_fits = 1, bool print_factors = 1) {

  std::string data_input_file = "PHM_out/data_201278_Jan22RR.root";
  std::string mc_input_dir = "PHM_out/dcol_scan_CMSSW_53X/";
  
  const int nfile = 10;
  double dcol_eff[nfile] = { 1, 0.999, 0.996, 0.993, 0.99, 0.98, 0.97, 0.95, 0.90, 0.80 };
  std::string mc_input_file[nfile] = { "dcol100.root", "dcol99_9.root", "dcol99_6.root", "dcol99_3.root", "dcol99.root", 
				       "dcol98.root", "dcol97.root", "dcol95.root", "dcol90.root", "dcol80.root" };
  for (int i=0; i<nfile; i++) mc_input_file[i] = (mc_input_dir+mc_input_file[i]).c_str();
  
  // Data
  TH1D *data_lad_eff[3];
  TH1D *data_mod_eff[3];
  TH1D *data_mod_den[3];
  TH1D *data_instlumi_eff[3];
  TH1D *instlumi_scale[3];
  const char* layname[3] = {"_l1", "_l2","_l3"};
  for (int lay=0; lay<3; lay++) {
    data_lad_eff[lay]=getplot(data_input_file,(std::string("dynamic_ineff_ladder")+layname[lay]+"_Data_eff"));
    data_mod_eff[lay]=getplot(data_input_file,(std::string("dynamic_ineff_module")+layname[lay]+"_Data_eff"));
    data_mod_den[lay]=getplot(data_input_file,(std::string("dynamic_ineff_module")+layname[lay]+"_Data_dist"));
    data_instlumi_eff[lay]=getplot(data_input_file,(std::string("dynamic_ineff_instlumi")+layname[lay]+"_Data_eff"));
  }
  
  // MC
  TH1D *mc_det_eff[nfile];
  TH1D *mc_mod_eff[3][nfile];
  TH1D *mc_mod_den[3][nfile];
  TH1D *mc_dcol100_modmap_den[3];
  for (int i=0; i<nfile; i++) {
    mc_det_eff[i] = getplot(mc_input_file[i], "det_eff");
    for (int lay=0; lay<3; lay++) {
      mc_mod_eff[lay][i] = getplot(mc_input_file[i], (std::string("dynamic_ineff_module")+layname[lay]+"_MC_eff"));
      mc_mod_den[lay][i] = getplot(mc_input_file[i], (std::string("dynamic_ineff_module")+layname[lay]+"_MC_dist"));
      if (i==0) mc_dcol100_modmap_den[lay] = getplot(mc_input_file[i], (std::string("mod")+layname[lay]+"_dist"));
    }
  }
  
  const char* name[3] = {"hiteff_vs_dcol_l1", "hiteff_vs_dcol_l2", "hiteff_vs_dcol_l3"};
  TF1 *avg_fit[3];
  TH1D *hiteff_vs_dcol[3];
  const char* ringname[4] = {"_ring1", "_ring2", "_ring3", "_ring4"};
  TF1 *ring_fit[3][4];
  TH1D *hiteff_vs_dcol_ring[3][4];
  TF1 *instlumi_fit[3];
  TH1D *instlumi_scale[3];
  TCanvas * c; 
  if (draw_fits) {
    gStyle->SetOptStat(0);
    c = new TCanvas("c", "c",800,800);
    c->Divide(2,2);
  }
  // Scale factor arrays
  std::vector<double> mc_ladder_on_dcol[3];
  std::vector<double> mc_ladder_off_dcol[3];
  double data_ring_eff[3][4];
  double data_ring_dcol[3][4];
  double mc_ring_eff[3][4];
  double mc_ring_dcol[3][4];
  double mc_ring_scale[3][4];
  double mc_lay_eff[3];
  double mc_lay_dcol[3];

  std::map<int,int> on_to_off_map[3];
  
  for (int lay=0; lay<3; lay++) {
    // Hit Eff vs Dcol Eff Fitting
    const char* axis_title = ";DCol Efficiency;Hit Efficiency";
    hiteff_vs_dcol[lay] = new TH1D(name[lay],axis_title,251,0.7495,1.0005);
    for (int ring=0; ring<4; ring++)
      hiteff_vs_dcol_ring[lay][ring] = new TH1D((std::string(name[lay])+ringname[ring]).c_str(),axis_title,251,0.7495,1.0005);
    for (int i=0; i<nfile; i++) {
      hiteff_vs_dcol[lay]->Fill(dcol_eff[i],mc_det_eff[i]->GetBinContent(lay+1));
      for (int ring=0; ring<4; ring++) {
	double mc_val = 0, mc_mis = 0;
	double data_val = 0, data_mis = 0;
	for (int side=0; side<=1; side++) {
	  double mc_eff = mc_mod_eff[lay][i]->GetBinContent(side?4-ring:6+ring);
	  double mc_den = mc_mod_den[lay][i]->GetBinContent(side?4-ring:6+ring);
	  mc_val += mc_eff*mc_den;
	  mc_mis += (1-mc_eff)*mc_den;
	  if (i==0) {
	    double data_eff = data_mod_eff[lay]->GetBinContent(side?4-ring:6+ring);
	    double data_den = data_mod_den[lay]->GetBinContent(side?4-ring:6+ring);
	    data_val += data_eff*data_den;
	    data_mis += (1-data_eff)*data_den;
	  }
	}
	hiteff_vs_dcol_ring[lay][ring]->Fill(dcol_eff[i],mc_val/(mc_val+mc_mis));
	if (i==0) data_ring_eff[lay][ring] = data_val/(data_val+data_mis);
      }
    }
    avg_fit[lay] = new TF1(name[lay],"pol2",0.75,1);
    hiteff_vs_dcol[lay]->Fit(name[lay],"RQ0");
    for (int ring=0; ring<4; ring++) {
      ring_fit[lay][ring] = new TF1((std::string(name[lay])+ringname[ring]).c_str(),"pol2",0.75,1);
      hiteff_vs_dcol_ring[lay][ring]->Fit((std::string(name[lay])+ringname[ring]).c_str(),"RQ0");
    }
    if (draw_fits) {
      c->cd(1);
      hiteff_vs_dcol[lay]->SetAxisRange(0.8, 1.05, "Y");
      hiteff_vs_dcol[lay]->SetMarkerColor(1+lay);
      hiteff_vs_dcol[lay]->SetMarkerStyle(20+lay);
      hiteff_vs_dcol[lay]->Draw(lay?"PSAME":"P");
      avg_fit[lay]->SetLineColor(lay+1);
      avg_fit[lay]->Draw("SAME");
      c->cd(2+lay);
      hiteff_vs_dcol_ring[lay][0]->SetAxisRange(0.8, 1.05, "Y");
      for (int ring=0; ring<4; ring++) {
	int color = (ring==0) ? 2 : ((ring==1) ? 6 : ((ring==2) ? 4 : 7));
	hiteff_vs_dcol_ring[lay][ring]->SetMarkerColor(color);
	hiteff_vs_dcol_ring[lay][ring]->SetMarkerStyle(20+ring);
	hiteff_vs_dcol_ring[lay][ring]->Draw(ring?"PSAME":"P");
	ring_fit[lay][ring]->SetLineColor(color);
	ring_fit[lay][ring]->Draw("SAME");
      }
    }
    // Ladder factors
    int nlad = (lay==0) ? 20 : (lay==1 ? 32 : 44);
    for (int ladder=1; ladder<=nlad; ladder++) {
      int on_ladder = get_online_ladder(lay+1,ladder);
      on_to_off_map[lay][on_ladder] = ladder;
      //std::cout<<"offline ladder "<<ladder<<" is online ladder "<<on_ladder<<std::endl;
      //std::cout<<"online ladder "<<on_ladder<<" is offline ladder "<<on_to_off_map[lay][on_ladder]<<std::endl;
    }
    mc_ladder_off_dcol[lay].resize(nlad);
    mc_ladder_on_dcol[lay].resize(nlad+1);
    for (int on_ladder=-nlad/2; on_ladder<=nlad/2; on_ladder++) {
      // Online scale factors
      if (on_ladder!=0) {
	double hit_eff = data_lad_eff[lay]->GetBinContent(on_ladder+23);
	double dcol_factor = avg_fit[lay]->GetX(hit_eff);
	int off_ladder = on_to_off_map[lay][on_ladder];
	mc_ladder_off_dcol[lay][off_ladder-1] = dcol_factor; // Offline module dcol efficiency factors
	mc_ladder_on_dcol[lay][on_ladder+nlad/2] = dcol_factor; // Online module dcol efficiency factors
      }
    }
    if (print_factors) {
      std::cout<<"theLadderEfficiency_BPix"<<(lay+1)<<" = cms.vdouble(\n";
      for (int off_ladder=1; off_ladder<=nlad; off_ladder++) std::cout<<"    "<<mc_ladder_off_dcol[lay][off_ladder-1]<<",\n";
      std::cout<<")\n";
    }
    
    // Module and Instlumi factors
    mc_lay_eff[lay] = 0;
    double lay_den = 0;
    for (int ring=0; ring<4; ring++) {
      double ring_den = 0;
      mc_ring_eff[lay][ring] = 0;
      for (int side=0; side<=1; side++) {
	for (int ilad=0; ilad<=nlad; ilad++) if (ilad!=nlad/2) {
	  double module_den = mc_dcol100_modmap_den[lay]->GetBinContent(side?4-ring:6+ring, ilad+1);
	  ring_den += module_den;
	  mc_ring_eff[lay][ring] += module_den * ring_fit[lay][ring]->Eval(mc_ladder_on_dcol[lay][ilad]);
        }
      }
      mc_ring_eff[lay][ring] /= ring_den;
      mc_ring_dcol[lay][ring] = ring_fit[lay][ring]->GetX(mc_ring_eff[lay][ring]);
      data_ring_dcol[lay][ring] = ring_fit[lay][ring]->GetX(data_ring_eff[lay][ring]);
      mc_ring_scale[lay][ring] = data_ring_dcol[lay][ring] / mc_ring_dcol[lay][ring];
      //std::cout<<"Layer "<<(lay+1)<<" Ring "<<(ring+1)<<" average Data hit eff: "<<data_ring_eff[lay][ring]<<std::endl;
      //std::cout<<"Layer "<<(lay+1)<<" Ring "<<(ring+1)<<" average Data dcol eff: "<<data_ring_dcol[lay][ring]<<std::endl;
      //std::cout<<"Layer "<<(lay+1)<<" Ring "<<(ring+1)<<" average MC dcol eff: "<<mc_ring_dcol[lay][ring]<<std::endl;
      //std::cout<<"Layer "<<(lay+1)<<" Ring "<<(ring+1)<<" Scale factor: "<<mc_ring_scale[lay][ring]<<std::endl;
      for (int side=0; side<=1; side++) {
	for (int ilad=0; ilad<=nlad; ilad++) if (ilad!=nlad/2) {
	  double module_den = mc_dcol100_modmap_den[lay]->GetBinContent(side?4-ring:6+ring, ilad+1);
	  lay_den += module_den;
	  mc_lay_eff[lay] += module_den * ring_fit[lay][ring]->Eval(mc_ladder_on_dcol[lay][ilad]*mc_ring_scale[lay][ring]);
        }
      }
    }
    if (print_factors) {
      std::cout<<"theModuleEfficiency_BPix"<<(lay+1)<<" = cms.vdouble("<<std::endl;
      for (int ring=0; ring<4; ring++) std::cout<<"    "<<mc_ring_scale[lay][ring]<<",\n";
      std::cout<<")\n";
    }
    
    mc_lay_eff[lay] /= lay_den;
    mc_lay_dcol[lay] = avg_fit[lay]->GetX(mc_lay_eff[lay]);
    
    instlumi_scale[lay]=(TH1D*)data_instlumi_eff[lay]->Clone();
    
    //instlumi scaling
    for (int i=1; i<=instlumi_scale[0]->GetNbinsX(); i++) {
      double data_instlumi_lay_eff = data_instlumi_eff[lay]->GetBinContent(i);
      double data_instlumi_lay_dcol = avg_fit[lay]->GetX(data_instlumi_lay_eff);
      double instlumi_scale_factor = (data_instlumi_lay_eff>0) ? data_instlumi_lay_dcol/mc_lay_dcol[lay] : 0;
      instlumi_scale[lay]->SetBinContent(i,instlumi_scale_factor);
    }
    instlumi_fit[lay] = new TF1((std::string("instlumi_fit")+layname[lay]).c_str(),"pol2",1900, 7000);
    instlumi_scale[lay]->Fit((std::string("instlumi_fit")+layname[lay]).c_str(), "RQ0");
    //instlumi_fit[lay]->Print();
    if (print_factors) {
      std::cout<<"thePUEfficiency_BPix"<<(lay+1)<<" = cms.vdouble("<<std::endl;
      for (int par=0; par<3; par++) std::cout<<"    "<<instlumi_fit[lay]->GetParameter(par)<<",\n";
      std::cout<<")\n\n";
    }
    
  }
  TCanvas * proba = new TCanvas("proba", "proba",600,600);
  proba->cd();
  instlumi_scale[0]->Draw("P");
  instlumi_fit[0]->Draw("SAME");

}
