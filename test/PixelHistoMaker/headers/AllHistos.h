class AllHistos {

 public:

  AllHistos()  { debug_=0; }
  ~AllHistos() {}

  // Not Efficiency Plots
  Histograms* totlumi_ratio;
  Histograms* nclu_corr;
  Histograms* npix_corr;
  Histograms* bx_ls;
  Histograms* ls_fill;
  Histograms* instlumi_corr;

  Histograms* ncd;
  Histograms* ncd_corr;
  Histograms* pcd;
  Histograms* pcd_corr;
  Histograms* pcd_heta;
  Histograms* pcd_corr_heta;
  Histograms* clu_size;
  Histograms* clu_sizeX;
  Histograms* clu_sizeY;
  Histograms* clu_size_heta;
  Histograms* clu_sizeX_heta;
  Histograms* clu_sizeY_heta;
  Histograms* alpha;
  Histograms* beta;

  // Efficiency
  Histograms* federr;
  Histograms* federr_evt;
  Histograms* time_federr;

  Histograms* run;
  Histograms* totlumi;
  Histograms* trigger;
  Histograms* lxly;
  // N-1
  Histograms* pt;
  Histograms* nstrip;
  Histograms* dz;
  Histograms* d0;
  Histograms* dz_d0;
  Histograms* normchi2;
  Histograms* fid_lxly;
  Histograms* fid_lx;
  Histograms* fid_ly;
  Histograms* vtxd0;
  Histograms* vtxz;
  Histograms* hitclu;
  Histograms* cluclu;
  Histograms* hithit;
  Histograms* clcl_hcl;
  Histograms* hh_hcl;
  Histograms* neweff;
  Histograms* neweff2;;
  Histograms* angle;
  Histograms* angle2d;
  Histograms* nstrip_new;

  Histograms* instlumi_raw;
  Histograms* roceff_time;
  Histograms* roceff_fill;
  Histograms* fill_stat;
  Histograms* roceff_dist;
  Histograms* badroc;
  Histograms* nbadroc;
  
  
  Histograms* nvtx;
  Histograms* pileup;
  Histograms* ls;
  Histograms* occup;
  Histograms* occup_mod;
  Histograms* occup_roc;
  Histograms* occup_dcol;
  Histograms* det;
  Histograms* mod;
  Histograms* roc;
  Histograms* time;
  Histograms* time2;
  Histograms* bx;
  Histograms* instlumi;
  Histograms* il_l1rate;
  Histograms* l1rate;  

  Histograms* dynamic_ineff;
  Histograms* rocmap;
  TH2D* l1rate_vs_instlumi;
  
  // Scans
  Histograms* delay11;
  Histograms* delay11_rog;
  Histograms* delay11_mod;
  Histograms* delay11_roc;
  Histograms* delay12;
  Histograms* delay12_rog;
  Histograms* delay12_mod;
  Histograms* delay12_roc;
  Histograms* delay3;
  Histograms* delay3_rog;
  Histograms* delay3_mod;
  Histograms* delay3_roc;
  Histograms* delay4;
  Histograms* delay4_rog;
  Histograms* delay4_mod;
  Histograms* delay4_roc;
  Histograms* trkloss;
  Histograms* hv_l1;
  Histograms* hv_l2;
  Histograms* hv_l3;
  Histograms* hv_d1;
  Histograms* hv_l1full;
  Histograms* hv_l2full;
  Histograms* hv_l3full;
  Histograms* hv_d1full;
  Histograms* hv_d2full;
  std::vector<TGraph*> hv_totlumi;
  Histograms* vturnon_totlumi;
  Histograms* vturnon_fpix;
  Histograms* vturnon_bpix;
  
  
  //__________________________________________________________________________________________
  //                                   Starting Sequences
  //__________________________________________________________________________________________
  
  
  // eventTree 1st loop
  void fill_evt(TreeReader &tr, Variables &var) {
    // if ((tr.evt().trig>>0)&1) // ZeroBias
    evthistos_fill_(var, tr.evt());
  }
  
  // eventTree 2nd loop
  void postfill_evt(TreeReader &tr, Variables &var) {
    // if ((tr.evt().trig>>0)&1) // ZeroBias
#if FEDERRPLOTS == 1
    federr_evt_postfill_(var, tr.evt());
#endif
  }
  
  // trajTree loop
  void fill_traj(TreeReader &tr, Variables &var) {
    tr.recover_missing_cluster();
#if NONEFFPLOTS ==1
    non_efficiency_fill_(var, tr.traj(), tr.traj_evt());
#endif
    efficiency_fill_(var, tr.traj(), tr.traj_evt());
#if SCANPLOTS !=0
    scans_traj_fill_(var, tr.traj(), tr.traj_evt());
#endif
  }
  
#ifdef COMPLETE
  void fill_clust(const EventData &cluevt, const Cluster &clust, Variables &var) {
  }
#endif
  
 private:
  
  int debug_;
  
  //__________________________________________________________________________________________
  //                          Filling functions for eventTree loop
  //__________________________________________________________________________________________
  
  void evthistos_fill_(Variables &v, const EventData &e) {
    // FED error plots
#if FEDERRPLOTS == 1
    federr_evt_fill_(v, e);
#endif
#if VERSION2 >= 31
    nvtx->h1d(0,2)->Fill(e.nvtx, e.instlumi_ext);
    nvtx->h1d(0,3)->Fill(e.nvtx);
    l1rate_vs_instlumi->Fill(e.instlumi_ext, e.l1_rate);
#endif
  }
  
  //__________________________________________________________________________________________
  //                          Filling functions for trajTree loop
  //__________________________________________________________________________________________
  
  
  void non_efficiency_fill_(Variables &v, const TrajMeasurement &t, const EventData &e) {
    //________________________________________________________________________________________
    //                                  Not Efficiency Plots:
    //  Nclu plots: nclu/npix_corr, bx_ls, ls_fill, totlumi_ratio, time/instlumi (nclu part)
    //                   Performance plots: pcd, ncd, clu_size, alpha, beta
#ifdef COMPLETE
    // Nclu plots
    if (v.traj_newevt&&v.traj_p1_fill!=NOVAL_I&&(e.trig&1)) {
      nclu_corr->h2d(v.traj_p1_fill, 0)->Fill(v.nclu_lay(1), v.nclu_lay(2));
      nclu_corr->h2d(v.traj_p1_fill, 1)->Fill(v.nclu_lay(1), v.nclu_lay(3));
      npix_corr->h2d(v.traj_p1_fill, 0)->Fill(v.npix_lay(1), v.npix_lay(2));
      npix_corr->h2d(v.traj_p1_fill, 1)->Fill(v.npix_lay(1), v.npix_lay(3));
      instlumi->h3d(v.traj_p1_fill, 0)->Fill(v.lumi_instlumi, v.nclu_lay(1), v.nclu_lay(2));
      instlumi->h3d(v.traj_p1_fill, 1)->Fill(v.lumi_instlumi, v.nclu_lay(1), v.nclu_lay(3));
      instlumi->h3d(v.traj_p1_fill, 2)->Fill(v.lumi_instlumi, v.npix_lay(1), v.npix_lay(2));
      instlumi->h3d(v.traj_p1_fill, 3)->Fill(v.lumi_instlumi, v.npix_lay(1), v.npix_lay(3));
      for (int i=0; i<10; i++) {
        bx_ls->h2d(v.traj_p1_fill, i, 0)->Fill(e.ls, e.bx, v.nclu_det(i));
        bx_ls->h2d(v.traj_p1_fill, i, 1)->Fill(e.ls, e.bx);
        ls_fill->h1d(v.traj_p1_fill, i, 2)->Fill(e.ls, v.nclu_det(i));
        ls_fill->h1d(v.traj_p1_fill, i, 3)->Fill(e.ls);	
	totlumi_ratio->h1d(i)->Fill(v.lumi_totlumi/1000.0, v.nclu_det(i));
	time      ->h1d(v.traj_p1_fill, i, 2)->Fill(v.lumi_time, v.nclu_det(i));
	instlumi      ->h1d(0,              i, 2)->Fill(v.lumi_instlumi, v.nclu_det(i));
	instlumi      ->h1d(v.traj_p1_fill, i, 2)->Fill(v.lumi_instlumi, v.nclu_det(i));
      }
    }
#endif
    // Performance plots
    // Cuts:
    // - pt, nstrip:  cut fakes, curly tracks
    // - vtx:         exclude beam halo, secondaries - central vertex
    // - trigger:     ZeroBias
    if (v.p1_totlumi!=-1&&t.trk.pt>1.0&&e.nvtx>1&&e.vtxndof>4&&fabs(e.vtxZ)<6.0&&fabs(e.vtxD0)<0.3&&(e.trig&1)) {
      int a = -1;
      if (t.clu.size>0) { 
        int loop = 0; 
        // Randomly exclude a pixel with charge > 3.0 ke
        while (a==-1&&loop<100) {loop++; a = rand()%t.clu.size; if (t.clu.pix[a][0]<3.0) a=-1; } 
        for (int i=0; i<t.clu.size; i++) {
          if (i!=a) {
            if (fabs(t.trk.eta)<0.1) {
              pcd     ->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.adc[i]);
              pcd_corr->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.adc[i]/v.gaincorr);
            } else if (fabs(t.trk.eta)>1.5&&fabs(t.trk.eta)<1.6) {
              pcd_heta->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.adc[i]);
              pcd_corr_heta->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.adc[i]/v.gaincorr);
            }
          }
        }
      }
      if (fabs(t.trk.eta)<0.1) { // constant angle - low eta
        clu_size      ->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.size);
        clu_sizeX     ->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.sizeX);
        clu_sizeY     ->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.sizeY);
      } else if (fabs(t.trk.eta)>1.5&&fabs(t.trk.eta)<1.6) { // high eta
        clu_size_heta ->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.size);
        clu_sizeX_heta->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.sizeX);
        clu_sizeY_heta->det_fill_1d(v.p1_totlumi, t.mod_on, t.clu.sizeY);
      }
      ncd     ->det_fill_1d(v.p1_totlumi, t.mod_on, t.norm_charge);
      ncd_corr->det_fill_1d(v.p1_totlumi, t.mod_on, t.norm_charge/v.gaincorr);
      alpha   ->det_fill_1d(v.p1_totlumi, t.mod_on, fabs(t.alpha));
      beta    ->det_fill_1d(v.p1_totlumi, t.mod_on, fabs(t.beta));
    }
  }
  
  void efficiency_fill_(Variables &v, const TrajMeasurement &t, const EventData &e) {

    //________________________________________________________________________________________
    //                                    Efficiency Plots
    //________________________________________________________________________________________
    //                               Plots without general cuts:
    //       federr, time_federr, run, totlumi, trigger, roc (inactive, excluded parts)

#if FEDERRPLOTS == 1
    federr_traj_fill_(v, t, e);
#endif
    
    if (FILLS) {
      run->det_hist_def_fill(0,           v.effcut_all, t, v.traj_run_binx, v.traj_gaincorr);
      run->det_hist_def_fill(v.traj_mod2, v.effcut_all, t, v.traj_run_binx, v.traj_gaincorr);
    }
    // Efficiency and MPV of NCCD vs total integrated luminosity
    // totlumi.det_effmpv_fill(v.effcut_all, t, v.lumi_totlumi, v.traj_gaincorr);
    bool c1 = v.effcut_all; // Efficiency Cut
    bool c2 = t.trk.pt>1.0&&v.goodroc; // Normalized Cluster Charge Distribution Cut
    totlumi->det_fillw_1d(t.mod_on, 4, v.lumi_totlumi_2012, t.clu.size);
    totlumi->det_fill_1d (t.mod_on, 5, v.lumi_totlumi_2012);
    // t.trk.strip>10 is also default (except for these: )
    // Don't use vtx cut as it only works for primary vertex (high pileup)
    if (t.trk.pt>1.0&&e.nvtx>1&&e.vtxndof>4&&fabs(e.vtxZ)<6.0&&fabs(e.vtxD0)<0.3&&(e.trig&1)) {
      totlumi->det_effmpv_fill(c1, c2, t, v.lumi_totlumi_2012);
      if (fabs(t.trk.eta)<0.1) { // constant angle - low eta
        totlumi->det_fillw_1d(t.mod_on, 6, v.lumi_totlumi_2012, t.clu.size);
        totlumi->det_fill_1d (t.mod_on, 7, v.lumi_totlumi_2012);
      } else if (fabs(t.trk.eta)>1.5&&fabs(t.trk.eta)<1.6) { // high eta
        totlumi->det_fillw_1d(t.mod_on, 8, v.lumi_totlumi_2012, t.clu.size);
        totlumi->det_fill_1d (t.mod_on, 9, v.lumi_totlumi_2012);
      }
    }
    //trigger->det_fill_1d(0, t.mod_on, t.missing, -2);
    //if (e.trig==0) trigger->det_fill_1d(0, t.mod_on, t.missing, 0);
    //if (v.effcut_trig) { 
    //  trigger->det_fill_1d(0, t.mod_on, t.missing, -1);
    //  for (int i=0; i<32; i++) if (((e.trig>>i)&1)) trigger->det_fill_1d(0, t.mod_on, t.missing, i+1);
    //}
    //if (v.traj_p1_fill!=NOVAL_I) {
    //  trigger->det_fill_1d(v.traj_p1_fill, t.mod_on, t.missing, -2);
    //  if (e.trig==0) trigger->det_fill_1d(v.traj_p1_fill, t.mod_on, t.missing, 0);
    //  if (v.effcut_trig) {
    //    for (int i=0; i<32; i++) if (((e.trig>>i)&1)) trigger->det_fill_1d(v.traj_p1_fill, t.mod_on, t.missing, i+1);
    //    trigger->det_fill_1d(v.traj_p1_fill, t.mod_on, t.missing, -1);
    //  }
    //}
    //if (v.effcut_lxly_fid.cut&&v.p1_badcol!=NOVAL_I) lxly->h1d(v.p1_badcol, t.missing)->Fill(t.ly, t.lx);
    if (v.traj_roc_binx!=NOVAL_I&&v.traj_roc_biny!=NOVAL_I) {
      if (v.inac==1) { // Inactive ROCs
        roc->    increasebin_2d(0,              v.traj_roc_p1, 2, v.traj_roc_binx, v.traj_roc_biny);
        if (t.mod_on.det==1)
          roc->  increasebin_2d(0,              v.traj_roc_p1, 2, v.traj_roc_binx, v.traj_roc_biny+1);
        if (v.traj_p1_fill!=NOVAL_I) {
            roc->increasebin_2d(v.traj_p1_fill, v.traj_roc_p1, 2, v.traj_roc_binx, v.traj_roc_biny);
          if (t.mod_on.det==1)
            roc->increasebin_2d(v.traj_p1_fill, v.traj_roc_p1, 2, v.traj_roc_binx, v.traj_roc_biny+1);
        }
      }
    }

    //                            Dynamic Efficiency Loss Plots
    //                                 (By Jozsef Krizsan)
    
    if (v.effcut_all) {
      if (v.zb) {
	if (v.traj_instlumi>2000) {
	  if (e.run!=1) { // Data
	    if (!t.mod_on.det) {
	      dynamic_ineff ->h1d(0, t.mod_on.layer-1,  0, t.missing)->Fill(t.mod_on.ladder);
	      dynamic_ineff ->h1d(1, t.mod_on.layer-1,  0, t.missing)->Fill(t.mod_on.module);
	      dynamic_ineff ->h1d(2, t.mod_on.layer-1,  0, t.missing)->Fill(v.traj_instlumi);
	      dynamic_ineff ->h1d(3, t.mod_on.layer-1,  0, t.missing)->Fill(v.traj_l1rate);
	    }
	    rocmap->   increasebin_2d(v.traj_roc_p1, 0, t.missing, v.traj_roc_binx, v.traj_roc_biny);
	    if (t.mod_on.det==1)
	      rocmap-> increasebin_2d(v.traj_roc_p1, 0, t.missing, v.traj_roc_binx, v.traj_roc_biny+1);
	  } else { // MC
	    if (!t.mod_on.det) {
	      dynamic_ineff ->h1d(0, t.mod_on.layer-1,  1, t.missing)->Fill(t.mod_on.ladder);
	      dynamic_ineff ->h1d(1, t.mod_on.layer-1,  1, t.missing)->Fill(t.mod_on.module);
	      dynamic_ineff ->h1d(2, t.mod_on.layer-1,  1, t.missing)->Fill(v.traj_instlumi);
	      dynamic_ineff ->h1d(3, t.mod_on.layer-1,  1, t.missing)->Fill(v.traj_l1rate);
	      dynamic_ineff ->h1d(0, t.mod_on.layer-1,  2, v.gen_missing)->Fill(t.mod_on.ladder);
	      dynamic_ineff ->h1d(1, t.mod_on.layer-1,  2, v.gen_missing)->Fill(t.mod_on.module);
	      dynamic_ineff ->h1d(2, t.mod_on.layer-1,  2, v.gen_missing)->Fill(v.traj_instlumi);
	      dynamic_ineff ->h1d(3, t.mod_on.layer-1,  2, v.gen_missing)->Fill(v.traj_l1rate);
	    }
	    rocmap->   increasebin_2d(v.traj_roc_p1, 1, t.missing, v.traj_roc_binx, v.traj_roc_biny);
	    if (t.mod_on.det==1)
	      rocmap-> increasebin_2d(v.traj_roc_p1, 1, t.missing, v.traj_roc_binx, v.traj_roc_biny+1);
	    rocmap->   increasebin_2d(v.traj_roc_p1, 2, v.gen_missing, v.traj_roc_binx, v.traj_roc_biny);
	    if (t.mod_on.det==1)
	      rocmap-> increasebin_2d(v.traj_roc_p1, 2, v.gen_missing, v.traj_roc_binx, v.traj_roc_biny+1);
	  }
	}
      }
    }

    //________________________________________________________________________________________
    //                                Efficiency N-1 Cut Plots:
    //    pt, nstrip, d0/dz, (normchi2,) fid_lx/ly, vtxd0/z, hithit, hitclu, angle
    
    if (v.effcut_lxly_fid) fid_lxly->h2d(v.p1_fid, v.p2_fid, t.missing)->Fill(t.ly, t.lx);
    if (v.effcut_lx_fid)   fid_lx  ->h1d(v.p1_fid, v.p2_fid, t.missing)->Fill(t.lx);
    if (v.effcut_ly_fid)   fid_ly  ->h1d(v.p1_fid, v.p2_fid, t.missing)->Fill(t.ly);
    
    if (!(abs(t.mod_on.disk)==1&&(t.mod_on.panel+t.mod_on.module)<4)) { // Exclude inner part of Disk 1
      if (v.effcut_pt)      pt      ->det2_fill_1d(t.mod_on, t.missing, t.trk.pt);
      if (v.effcut_nstrip)  nstrip  ->det2_fill_1d(t.mod_on, t.missing, t.trk.strip);
      if (v.effcut_dz)      dz      ->det2_fill_1d(t.mod_on, t.missing, fabs(t.trk.dz));
      if (v.effcut_d0)      d0      ->det2_fill_1d(t.mod_on, t.missing, fabs(t.trk.d0));
      if (v.effcut_d0_dz)   dz_d0   -> det_fill_2d(t.mod_on, t.missing, fabs(t.trk.dz),fabs(t.trk.d0));
      if (v.effcut_nvtx) {
        vtxd0->h1d(0, t.missing)->Fill(e.vtxD0);
        vtxd0->h1d(t.mod_on.det+1, t.missing)->Fill(e.vtxD0);
        vtxz ->h1d(0, t.missing)->Fill(e.vtxZ);
        vtxz ->h1d(t.mod_on.det+1, t.missing)->Fill(e.vtxZ);
      }
      if (v.traj_dr_hit!=NOVAL_F) {
	if (v.effcut_hitsep) {
	  hithit->det_fill_1d(t.mod_on, 0,               0, v.traj_dr_hit);
	  hithit->det_fill_1d(t.mod_on, 1+t.clu.size==0, 0, v.traj_dr_hit);
	}
	hithit->det_fill_1d(t.mod_on, 0,               1, v.traj_dr_hit);
	hithit->det_fill_1d(t.mod_on, 1+t.clu.size==0, 1, v.traj_dr_hit);
      }
      if (v.traj_dr_clu!=NOVAL_F) {
	if (v.effcut_all) {
	  hitclu->det_fill_1d(t.mod_on, 0,               0, v.traj_dr_clu);
	  hitclu->det_fill_1d(t.mod_on, 1+t.clu.size==0, 0, v.traj_dr_clu);
	}
	hitclu->det_fill_1d(t.mod_on, 0,               1, v.traj_dr_clu);
	hitclu->det_fill_1d(t.mod_on, 1+t.clu.size==0, 1, v.traj_dr_clu);
      }
      if (v.traj_dr_cluclu!=NOVAL_F&&t.clu.size!=0) {
	if (v.effcut_all) {
	  cluclu->det_fill_1d(t.mod_on, 0,               0, v.traj_dr_cluclu);
	  cluclu->det_fill_1d(t.mod_on, 1+t.clu.size==0, 0, v.traj_dr_cluclu);
	}
	cluclu->det_fill_1d(t.mod_on, 0,               1, v.traj_dr_cluclu);
	cluclu->det_fill_1d(t.mod_on, 1+t.clu.size==0, 1, v.traj_dr_cluclu);
      }
      if (v.effcut_all) {
        normchi2->det2_fill_1d(t.mod_on, t.missing, (t.trk.chi2/t.trk.ndof));
	angle->det_fill_1d(0, t.mod_on, t.missing, v.angle_alpha);
        angle->det_fill_1d(1, t.mod_on, t.missing, v.angle_beta);
        angle->det_fill_1d(2, t.mod_on, t.missing, v.angle_incidence);
	angle->det_fill_1d(3, t.mod_on, t.missing, v.traj_theta);
	angle->det_fill_1d(4, t.mod_on, t.missing, v.traj_phi);
	angle2d->det_fill_2d(0, t.mod_on, t.missing, v.traj_theta, v.traj_phi);
        if (t.clu.size>0) { // Valid hits
          angle->det_fill_1d(0, t.mod_on, 2, v.angle_alpha);
          angle->det_fill_1d(1, t.mod_on, 2, v.angle_beta);
          angle->det_fill_1d(2, t.mod_on, 2, v.angle_incidence);
	  angle->det_fill_1d(3, t.mod_on, 2, v.traj_theta);
	  angle->det_fill_1d(4, t.mod_on, 2, v.traj_phi);
	  angle2d->det_fill_2d(0, t.mod_on, 2, v.traj_theta, v.traj_phi);
          angle->det_fillw_1d(0, t.mod_on, 3, v.angle_alpha,            t.norm_charge);
          angle->det_fillw_1d(1, t.mod_on, 3, v.angle_beta,             t.norm_charge);
          angle->det_fillw_1d(2, t.mod_on, 3, v.angle_incidence,        t.norm_charge);
	  angle->det_fillw_1d(3, t.mod_on, 2, v.traj_theta,             t.norm_charge);
	  angle->det_fillw_1d(4, t.mod_on, 2, v.traj_phi,               t.norm_charge);
	  angle2d->det_fillw_2d(0, t.mod_on, 2, v.traj_theta, v.traj_phi, t.norm_charge);
        }
      }
    }
    // New strip detector valuables
#if VERSION2 >=32
    if (v.effcut_nstrip) {
      nstrip_new->det_fill_1d (0, t.mod_on, t.missing, t.trk.strip);
      if (t.trk.nstriplost==0) nstrip_new->det_fill_1d (1, t.mod_on, t.missing, t.trk.strip);
      if (t.trk.nstriplayer>5) nstrip_new->det_fill_1d (2, t.mod_on, t.missing, t.trk.strip);
      if (t.trk.nstriplayer>7) nstrip_new->det_fill_1d (3, t.mod_on, t.missing, t.trk.strip);
      if (t.trk.nstriplayer>9) nstrip_new->det_fill_1d (4, t.mod_on, t.missing, t.trk.strip);
      if (t.trk.nstriplayer>7&&t.trk.nstriplost==0) nstrip_new->det_fill_1d (5, t.mod_on, t.missing, t.trk.strip);
    }
#endif


    //________________________________________________________________________________________
    //                            Plots with FED errors, and SEUs:
    //                   instlumi_raw, roceff_time, roceff_fill, fill_stat
    if (v.effcut_raw) {
      //if (v.good_instlumi) {
      instlumi_raw  ->det2_fill_1d(0,              0,        t.mod_on, t.missing, v.lumi_instlumi);
      instlumi_raw  ->det2_fill_1d(0,              v.year,   t.mod_on, t.missing, v.lumi_instlumi);
      instlumi_raw ->det2_fillw_1d(0,              v.year,   t.mod_on,         2, v.lumi_instlumi, e.l1_rate);
      instlumi_raw  ->det2_fill_1d(0,              v.year,   t.mod_on,         3, v.lumi_instlumi);
      if (v.traj_p1_fill!=NOVAL_I)
      	instlumi_raw->det2_fill_1d(v.traj_p1_fill, 0,        t.mod_on, t.missing, v.lumi_instlumi);
      // Badroc list plots
#if BADROC != 0
      roceff_time  ->h3d(0, v.p1_rocid, t.missing)->Fill(v.lumi_time, v.p2_rocid, v.p3_rocid);
      if (v.traj_p1_fill_badroc!=NOVAL_I)
	roceff_time->h3d(v.traj_p1_fill_badroc, v.p1_rocid, t.missing)->Fill(v.lumi_time, v.p2_rocid, v.p3_rocid);
#if BADROC == 2
      if (v.traj_p1_fill_badroc!=NOVAL_I) {
	roceff_fill->h3d(v.p1_rocid, t.missing)->Fill(v.traj_p1_fill_badroc, v.p2_rocid, v.p3_rocid);
	fill_stat->h1d(t.missing)->Fill(v.traj_p1_fill);
      }
#endif
#endif
    }
    
    //________________________________________________________________________________________
    //                            Plots without FED errors, SEUs:
    //                                    ls, occup, nvtx
    //     Fill-by-Fill plots: det, mod, roc (eff part), time, bx, instlumi, il_l1rate
    if (v.effcut_all) {
      ls->det_fill_1d(t.mod_on, t.missing, e.ls);
      float spectime = (e.run>=198268 && e.run<=198272) ? e.ls*0.0064754 + (e.run==198269)*0.45361 
	+ (e.run==198270)*1.79528 + (e.run==198271)*1.84833 + (e.run==198272)*7.04889 :
	(e.run>=198485 && e.run<=198487) ? e.ls*0.0064754 + (e.run==198486)*1.70528 + (e.run==198487)*1.79861 : 0;	
      time2->det_fill_1d((e.run>198272), t.mod_on, t.missing, spectime);
#ifdef COMPLETE
      occup->h1d(0, 0,              t.missing)->Fill(v.npix());
      occup->h1d(0, v.traj_det_x,   t.missing)->Fill(v.npix_det());
      occup->h1d(0, v.traj_lay_x+2, t.missing)->Fill(v.npix_lay());
      occup_mod->det_fill_1d(0, t.mod_on, t.missing, v.npix_mod());
      occup_roc->det_fill_1d(0, t.mod_on, t.missing, v.npix_roc());
      occup_dcol->det_fill_1d(0, t.mod_on, t.missing, v.npix_dcol());
      occup->h1d(1, 0,              t.missing)->Fill(v.nclu());
      occup->h1d(1, v.traj_det_x,   t.missing)->Fill(v.nclu_det());
      occup->h1d(1, v.traj_lay_x+2, t.missing)->Fill(v.nclu_lay());
      occup_mod->det_fill_1d(1, t.mod_on, t.missing, v.nclu_mod());
      occup_roc->det_fill_1d(1, t.mod_on, t.missing, v.nclu_roc());
      occup_dcol->det_fill_1d(1, t.mod_on, t.missing, v.nclu_dcol());
#elif VERSION2 >= 34 && SPLIT > 0
      // npix
      occup->h1d(0, 0, t.missing)->Fill(e.npix[0]+e.npix[1]+e.npix[2]+e.npix[3]);
      if (t.mod_on.det==0) occup->h1d(0, 1, t.missing)->Fill(e.npix[1]+e.npix[2]+e.npix[3]);
      occup->h1d(0, 2+(t.mod_on.det==0)*t.mod_on.layer, t.missing)->Fill(e.npix[(t.mod_on.det==0)*t.mod_on.layer]);
      occup_mod->det_fill_1d(0, t.mod_on, t.missing, t.npix_mod);
      occup_roc->det_fill_1d(0, t.mod_on, t.missing, t.npix_roc);
      // nclu
      occup->h1d(1, 0, t.missing)->Fill(e.nclu[0]+e.nclu[1]+e.nclu[2]+e.nclu[3]);
      if (t.mod_on.det==0) occup->h1d(1, 1, t.missing)->Fill(e.nclu[1]+e.nclu[2]+e.nclu[3]);
      occup->h1d(1, 2+(t.mod_on.det==0)*t.mod_on.layer, t.missing)->Fill(e.nclu[(t.mod_on.det==0)*t.mod_on.layer]);
      occup_mod->det_fill_1d(1, t.mod_on, t.missing, t.nclu_mod);
      occup_roc->det_fill_1d(1, t.mod_on, t.missing, t.nclu_roc);
#endif
      nvtx->det_fill_1d(t.mod_on, t.missing, e.nvtx);
      // Fill-by-Fill plots
      if (!(abs(t.mod_on.disk)==1&&(t.mod_on.panel+t.mod_on.module)<4))
	det->              h1d(0,                t.missing)->Fill(v.traj_lay_x);
      mod->              h2d(0, v.traj_mod_p1, t.missing)->Fill(v.traj_mod_x, v.traj_mod_y);
      roc->   increasebin_2d(0, v.traj_roc_p1, t.missing, v.traj_roc_binx, v.traj_roc_biny);
      if (t.mod_on.det==1)
	roc-> increasebin_2d(0, v.traj_roc_p1, t.missing, v.traj_roc_binx, v.traj_roc_biny+1); // fpix
      time->    det2_fill_1d(0, t.mod_on,      t.missing, v.lumi_time);
      if (v.main_filling_scheme) {
	bx->      det2_fill_1d(0, t.mod_on,      t.missing, e.bx);
	instlumi->det2_fill_1d(0, t.mod_on,      t.missing, v.lumi_instlumi);
	l1rate  ->det2_fill_1d(0, t.mod_on,      t.missing, e.l1_rate/1000.0);
#if VERSION2 >=27
	il_l1rate->det_fill_2d(0, t.mod_on,      t.missing, v.lumi_instlumi, e.l1_rate/1000.0);
#endif
      }
      if (v.traj_p1_fill!=NOVAL_I) {
	if (!(abs(t.mod_on.disk)==1&&(t.mod_on.panel+t.mod_on.module)<4))
	  det->              h1d(v.traj_p1_fill,                t.missing)->Fill(v.traj_lay_x);
	mod->              h2d(v.traj_p1_fill, v.traj_mod_p1, t.missing)->Fill(v.traj_mod_x, v.traj_mod_y);
	roc->   increasebin_2d(v.traj_p1_fill, v.traj_roc_p1, t.missing, v.traj_roc_binx, v.traj_roc_biny);
	if (t.mod_on.det==1)
	  roc-> increasebin_2d(v.traj_p1_fill, v.traj_roc_p1, t.missing, v.traj_roc_binx, v.traj_roc_biny+1); // fpix
	time->    det2_fill_1d(v.traj_p1_fill, t.mod_on,      t.missing, v.lumi_time);
	bx->      det2_fill_1d(v.traj_p1_fill, t.mod_on,      t.missing, e.bx);
	instlumi->det2_fill_1d(v.traj_p1_fill, t.mod_on,      t.missing, v.lumi_instlumi);
#if VERSION2 >=27
	il_l1rate->det_fill_2d(v.traj_p1_fill, t.mod_on,      t.missing, v.lumi_instlumi, e.l1_rate/1000.0);
#endif
	l1rate->  det2_fill_1d(v.traj_p1_fill, t.mod_on,      t.missing, e.l1_rate/1000.0);
      }
    }
  }

  void scans_traj_fill_(Variables &v, const TrajMeasurement &t, const EventData &e) {
    //________________________________________________________________________________________
    //                                      Delay Scan Plots:
    // delay11, delay11_rog, delay11_mod, delay11_roc, delay11, delay11_rog, delay11_mod, delay11_roc
#if SCANPLOTS == 2
    if (v.delay!=NOVAL_I) {
      if (e.run<190411) { // 2011 March - Timing Scan
	if (v.effcut_scans) { // Efficiency
	  delay11->det_fill_1d(t.mod_on, t.missing, v.delay);
	  delay11_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, t.missing)->Fill(v.delay);
	  delay11_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  t.missing)->Fill(v.delay);
	  delay11_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  t.missing)->Fill(v.delay);
	}
	if (t.clu.charge>11.0) { // Cluster Size
	  delay11->det_fillw_1d(t.mod_on, 2, v.delay, t.clu.size);
	  delay11->det_fill_1d (t.mod_on, 3, v.delay);
	  delay11_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 2)->Fill(v.delay, t.clu.size);
	  delay11_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 3)->Fill(v.delay);
	  delay11_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  2)->Fill(v.delay, t.clu.size);
	  delay11_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  3)->Fill(v.delay);
	  delay11_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  2)->Fill(v.delay, t.clu.size);
	  delay11_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  3)->Fill(v.delay);
	}
	if (t.trk.pt>0.6&&t.clu.size>0) { // Norm Clu Charge MPV
	  delay11->det_fill_2d(t.mod_on, 0, v.delay, t.norm_charge);
	  delay11_rog->h2d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 0)->Fill(v.delay, t.norm_charge);
	  delay11_mod->h2d(v.p1_modid, v.p2_modid,   v.p3_modid,  0)->Fill(v.delay, t.norm_charge);
	  if (v.lx_fid&&v.ly_fid)
	    delay11_roc->h2d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  0)->Fill(v.delay, t.norm_charge);
	}	
      } else if (e.run<198022) { // 2012 April - Timing Scan
	if (v.effcut_scans) { // Efficiency
	  delay12->det_fill_1d(t.mod_on, t.missing, v.delay);
	  delay12_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, t.missing)->Fill(v.delay);
	  delay12_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  t.missing)->Fill(v.delay);
	  delay12_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  t.missing)->Fill(v.delay);
	}
	if (t.clu.charge>12.0) { // Cluster Size
	  delay12->det_fillw_1d(t.mod_on, 2, v.delay, t.clu.size);
	  delay12->det_fill_1d (t.mod_on, 3, v.delay);
	  delay12_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 2)->Fill(v.delay, t.clu.size);
	  delay12_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 3)->Fill(v.delay);
	  delay12_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  2)->Fill(v.delay, t.clu.size);
	  delay12_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  3)->Fill(v.delay);
	  delay12_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  2)->Fill(v.delay, t.clu.size);
	  delay12_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  3)->Fill(v.delay);
	}
	if (t.trk.pt>0.6&&t.clu.size>0) { // Norm Clu Charge MPV
	  delay12->det_fill_2d(t.mod_on, 0, v.delay, t.norm_charge);
	  delay12_rog->h2d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 0)->Fill(v.delay, t.norm_charge);
	  delay12_mod->h2d(v.p1_modid, v.p2_modid,   v.p3_modid,  0)->Fill(v.delay, t.norm_charge);
	  if (v.lx_fid&&v.ly_fid)
	    delay12_roc->h2d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  0)->Fill(v.delay, t.norm_charge);
	}
	// 2012 July Scans
      } else if (e.run<198048) { // New Vana Settings
	// track/evt * 1/Eff_l1 - track loss
	trkloss->det_fill_1d(t.mod_on, 0, v.delay);
	if (v.traj_newevt) trkloss->h1d(0, 1)->Fill(v.delay);
	if (v.effcut_scans) { // Efficiency
	  delay3->det_fill_1d(t.mod_on, t.missing, v.delay);
	  delay3_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, t.missing)->Fill(v.delay);
	  delay3_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  t.missing)->Fill(v.delay);
	  delay3_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  t.missing)->Fill(v.delay);
	}
	if (t.clu.charge>12.0) { // Cluster Size
	  delay3->det_fillw_1d(t.mod_on, 2, v.delay, t.clu.size);
	  delay3->det_fill_1d (t.mod_on, 3, v.delay);
	  delay3_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 2)->Fill(v.delay, t.clu.size);
	  delay3_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 3)->Fill(v.delay);
	  delay3_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  2)->Fill(v.delay, t.clu.size);
	  delay3_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  3)->Fill(v.delay);
	  delay3_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  2)->Fill(v.delay, t.clu.size);
	  delay3_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  3)->Fill(v.delay);
	}
	if (t.trk.pt>0.6&&t.clu.size>0) { // Norm Clu Charge MPV
	  delay3->det_fill_2d(t.mod_on, 0, v.delay, t.norm_charge);
	  delay3_rog->h2d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 0)->Fill(v.delay, t.norm_charge);
	  delay3_mod->h2d(v.p1_modid, v.p2_modid,   v.p3_modid,  0)->Fill(v.delay, t.norm_charge);
	  if (v.lx_fid&&v.ly_fid)
	    delay3_roc->h2d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  0)->Fill(v.delay, t.norm_charge);
	}
      } else { // Old Vana Settings
	if (v.effcut_scans) { // Efficiency
	  delay4->det_fill_1d(t.mod_on, t.missing, v.delay);
	  delay4_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, t.missing)->Fill(v.delay);
	  delay4_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  t.missing)->Fill(v.delay);
	  delay4_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  t.missing)->Fill(v.delay);
	}
	if (t.clu.charge>12.0) { // Cluster Size
	  delay4->det_fillw_1d(t.mod_on, 2, v.delay, t.clu.size);
	  delay4->det_fill_1d (t.mod_on, 3, v.delay);
	  delay4_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 2)->Fill(v.delay, t.clu.size);
	  delay4_rog->h1d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 3)->Fill(v.delay);
	  delay4_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  2)->Fill(v.delay, t.clu.size);
	  delay4_mod->h1d(v.p1_modid, v.p2_modid,   v.p3_modid,  3)->Fill(v.delay);
	  delay4_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  2)->Fill(v.delay, t.clu.size);
	  delay4_roc->h1d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  3)->Fill(v.delay);
	}
	if (t.trk.pt>0.6&&t.clu.size>0) { // Norm Clu Charge MPV
	  delay4->det_fill_2d(t.mod_on, 0, v.delay, t.norm_charge);
	  delay4_rog->h2d(v.traj_shl, v.p2_secdisk, v.p3_layrog, 0)->Fill(v.delay, t.norm_charge);
	  delay4_mod->h2d(v.p1_modid, v.p2_modid,   v.p3_modid,  0)->Fill(v.delay, t.norm_charge);
	  if (v.lx_fid&&v.ly_fid)
	    delay4_roc->h2d(v.p1_rocid, v.p2_rocid,   v.p3_rocid,  0)->Fill(v.delay, t.norm_charge);
	}
      }
    }

    //________________________________________________________________________________________
    //                              High Voltage Bias Scan Plots:
    //                                          hv
    
    // HV Scans
#elif SCANPLOTS == 1
    if (v.hv_bias!=NOVAL_I) {
      int hvg_p2 = (t.mod_on.det==0) ? v.traj_shl*8+v.traj_sec : v.traj_shl*8 + v.hv_group*2 - 1 + v.hv_IO;
      int eta_p2 = (t.mod_on.det==0) ? 33+v.p1_mod : 32 + t.mod_on.module*2 - 2 + t.mod_on.panel;
      int phi_p2 = (t.mod_on.det==0) ? 39+2*abs(t.mod_on.ladder)+(t.mod_on.ladder>0) : 52 + t.mod_on.blade - (t.mod_on.blade>0);
      /*
      if (v.traj_newevt&&e.orb%100==0) {
	std::cout<<"bias: "<<v.hv_bias<<" orb: "<<e.orb/1000000.0<<std::endl;
	if (!v.effcut_scans)
	  std::cout<<v.nvtx<<" "<<v.federr<<" "<<v.hp<<" "<<v.pt<<" "<<v.nstrip<<" "<<v.d0<<" "<<v.dz<<" "<<v.pixhit<<" "<<v.goodmod<<" "<<v.lx_fid<<" "<<v.ly_fid<<" "<<v.valmis<<" "<<v.hitsep<<std::endl;
	else if (v.hv_l1_p1!=NOVAL_I||v.hv_l2_p1!=NOVAL_I||v.hv_l3_p1!=NOVAL_I||v.hv_d1_p1!=NOVAL_I) {
	  std::cout<<"ok"<<std::endl;
	}
      }
      */
      if (v.effcut_scans) { // Efficiency
	if (v.hv_l1_p1!=NOVAL_I) hv_l1->h1d(v.hv_l1_p1, t.missing)->Fill(v.hv_bias);
        if (v.hv_l2_p1!=NOVAL_I) hv_l2->h1d(v.hv_l2_p1, t.missing)->Fill(v.hv_bias);
        if (v.hv_l3_p1!=NOVAL_I) hv_l3->h1d(v.hv_l3_p1, t.missing)->Fill(v.hv_bias);
        if (v.hv_d1_p1!=NOVAL_I) hv_d1->h1d(v.hv_d1_p1, t.missing)->Fill(v.hv_bias);
        if (v.hv_l1full_p1!=NOVAL_I) {
          hv_l1full ->h1d(v.hv_l1full_p1, 0,      t.missing)->Fill(v.hv_bias);
          hv_l1full ->h1d(v.hv_l1full_p1, hvg_p2, t.missing)->Fill(v.hv_bias);
          hv_l1full ->h1d(v.hv_l1full_p1, eta_p2, t.missing)->Fill(v.hv_bias);
          hv_l1full ->h1d(v.hv_l1full_p1, phi_p2, t.missing)->Fill(v.hv_bias);
        } else if (v.hv_l2full_p1!=NOVAL_I) {
          hv_l2full ->h1d(v.hv_l2full_p1, 0,      t.missing)->Fill(v.hv_bias);
          hv_l2full ->h1d(v.hv_l2full_p1, hvg_p2, t.missing)->Fill(v.hv_bias);
          hv_l2full ->h1d(v.hv_l2full_p1, eta_p2, t.missing)->Fill(v.hv_bias);
          hv_l2full ->h1d(v.hv_l2full_p1, phi_p2, t.missing)->Fill(v.hv_bias);
        } else if (v.hv_l3full_p1!=NOVAL_I) {
          hv_l3full ->h1d(v.hv_l3full_p1, 0,      t.missing)->Fill(v.hv_bias);
          hv_l3full ->h1d(v.hv_l3full_p1, hvg_p2, t.missing)->Fill(v.hv_bias);
          hv_l3full ->h1d(v.hv_l3full_p1, eta_p2, t.missing)->Fill(v.hv_bias);
          hv_l3full ->h1d(v.hv_l3full_p1, phi_p2, t.missing)->Fill(v.hv_bias);
        } else if (v.hv_d1full_p1!=NOVAL_I) {
          hv_d1full ->h1d(v.hv_d1full_p1, 0,      t.missing)->Fill(v.hv_bias);
          hv_d1full ->h1d(v.hv_d1full_p1, hvg_p2, t.missing)->Fill(v.hv_bias);
          hv_d1full ->h1d(v.hv_d1full_p1, eta_p2, t.missing)->Fill(v.hv_bias);
          hv_d1full ->h1d(v.hv_d1full_p1, phi_p2, t.missing)->Fill(v.hv_bias);
        } else if (v.hv_d2full_p1!=NOVAL_I) {
          hv_d2full ->h1d(v.hv_d2full_p1, 0,      t.missing)->Fill(v.hv_bias);
          hv_d2full ->h1d(v.hv_d2full_p1, hvg_p2, t.missing)->Fill(v.hv_bias);
          hv_d2full ->h1d(v.hv_d2full_p1, eta_p2, t.missing)->Fill(v.hv_bias);
          hv_d2full ->h1d(v.hv_d2full_p1, phi_p2, t.missing)->Fill(v.hv_bias);
        }
      }
      if (t.trk.pt>0.6&&t.clu.size>0) { // Norm Clu Charge MPV
	if (v.hv_l1_p1!=NOVAL_I) hv_l1->h2d(v.hv_l1_p1, 0)->Fill(v.hv_bias, t.norm_charge);
	if (v.hv_l2_p1!=NOVAL_I) hv_l2->h2d(v.hv_l2_p1, 0)->Fill(v.hv_bias, t.norm_charge);
	if (v.hv_l3_p1!=NOVAL_I) hv_l3->h2d(v.hv_l3_p1, 0)->Fill(v.hv_bias, t.norm_charge);
	if (v.hv_d1_p1!=NOVAL_I) hv_d1->h2d(v.hv_d1_p1, 0)->Fill(v.hv_bias, t.norm_charge);
        if (v.hv_l1full_p1!=NOVAL_I) {
          hv_l1full ->h2d(v.hv_l1full_p1, 0,      0)->Fill(v.hv_bias, t.norm_charge);
          hv_l1full ->h2d(v.hv_l1full_p1, hvg_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_l1full ->h2d(v.hv_l1full_p1, eta_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_l1full ->h2d(v.hv_l1full_p1, phi_p2, 0)->Fill(v.hv_bias, t.norm_charge);
        } else if (v.hv_l2full_p1!=NOVAL_I) {
          hv_l2full ->h2d(v.hv_l2full_p1, 0,      0)->Fill(v.hv_bias, t.norm_charge);
          hv_l2full ->h2d(v.hv_l2full_p1, hvg_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_l2full ->h2d(v.hv_l2full_p1, eta_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_l2full ->h2d(v.hv_l2full_p1, phi_p2, 0)->Fill(v.hv_bias, t.norm_charge);
        } else if (v.hv_l3full_p1!=NOVAL_I) {
          hv_l3full ->h2d(v.hv_l3full_p1, 0,      0)->Fill(v.hv_bias, t.norm_charge);
          hv_l3full ->h2d(v.hv_l3full_p1, hvg_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_l3full ->h2d(v.hv_l3full_p1, eta_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_l3full ->h2d(v.hv_l3full_p1, phi_p2, 0)->Fill(v.hv_bias, t.norm_charge);
        } else if (v.hv_d1full_p1!=NOVAL_I) {
          hv_d1full ->h2d(v.hv_d1full_p1, 0,      0)->Fill(v.hv_bias, t.norm_charge);
          hv_d1full ->h2d(v.hv_d1full_p1, hvg_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_d1full ->h2d(v.hv_d1full_p1, eta_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_d1full ->h2d(v.hv_d1full_p1, phi_p2, 0)->Fill(v.hv_bias, t.norm_charge);
        } else if (v.hv_d2full_p1!=NOVAL_I) {
          hv_d2full ->h2d(v.hv_d2full_p1, 0,      0)->Fill(v.hv_bias, t.norm_charge);
          hv_d2full ->h2d(v.hv_d2full_p1, hvg_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_d2full ->h2d(v.hv_d2full_p1, eta_p2, 0)->Fill(v.hv_bias, t.norm_charge);
          hv_d2full ->h2d(v.hv_d2full_p1, phi_p2, 0)->Fill(v.hv_bias, t.norm_charge);
        }
      }
    }
#endif
  }

  //__________________________________________________________________________________________
  //                       FED Error Related Filling/Calculation Functions
  //__________________________________________________________________________________________


  void federr_traj_fill_(Variables &v, const TrajMeasurement &t, const EventData &e) {
    // efficiency
    // ROC Selection On - Excluded SEUs (N-1 plot for FED errors)
    if (v.effcut_federr) {
      federr->h1d(0, t.missing)->Fill(-1);
      federr->h1d(0, t.missing)->Fill(t.mod_on.federr);
      if (v.traj_p1_fill!=NOVAL_I) {
	federr->h1d(v.traj_p1_fill, t.missing)->Fill(-1);
	federr->h1d(v.traj_p1_fill, t.missing)->Fill(t.mod_on.federr);
      }
      // Default time plot for any/all fill
      time_federr    ->det_fill_1d(0,              0,                      t.mod_on, t.missing, v.lumi_time);
      if (t.mod_on.federr==0)
	time_federr  ->det_fill_1d(0,              1,                      t.mod_on, t.missing, v.lumi_time);
      else {
	time_federr  ->det_fill_1d(0,              2,                      t.mod_on, t.missing, v.lumi_time);
	time_federr  ->det_fill_1d(0,              t.mod_on.federr-22,     t.mod_on, t.missing, v.lumi_time);
      }
      time_federr  ->det_fill_1d(0,                19+(e.federrs_size==0), t.mod_on, t.missing, v.lumi_time);
      // fill postfix
      if (v.traj_p1_fill!=NOVAL_I) {
	time_federr  ->det_fill_1d(v.traj_p1_fill, 0,                      t.mod_on, t.missing, v.lumi_time);
	if (t.mod_on.federr==0)
	  time_federr->det_fill_1d(v.traj_p1_fill, 1,                      t.mod_on, t.missing, v.lumi_time);
	else {
	  time_federr->det_fill_1d(v.traj_p1_fill, 2,                      t.mod_on, t.missing, v.lumi_time);
	  time_federr->det_fill_1d(v.traj_p1_fill, t.mod_on.federr-22,     t.mod_on, t.missing, v.lumi_time);
	}
	time_federr  ->det_fill_1d(v.traj_p1_fill, 19+(e.federrs_size==0), t.mod_on, t.missing, v.lumi_time);
      }
    }
    // Module Fed error rate
    // Numerator:   Nhit_with_federr (h[p1][n][p3][2], where n=1,2, ... 16,17)
    // Denumerator: Nhit             (h[p1][0][p3][2])
    // Default plot for any/all fill
    time_federr    ->det_fill_1d(0,              0,                  t.mod_on, 2, v.lumi_time);
    if (t.mod_on.federr==0)
      time_federr  ->det_fill_1d(0,              1,                  t.mod_on, 2, v.lumi_time);
    else {
      time_federr  ->det_fill_1d(0,              2,                  t.mod_on, 2, v.lumi_time);
      time_federr  ->det_fill_1d(0,              t.mod_on.federr-22, t.mod_on, 2, v.lumi_time);
    }
    // fill postfix
    if (v.traj_p1_fill!=NOVAL_I) {
      time_federr  ->det_fill_1d(v.traj_p1_fill, 0,                  t.mod_on, 2, v.lumi_time);
      if (t.mod_on.federr==0)
        time_federr->det_fill_1d(v.traj_p1_fill, 1,                  t.mod_on, 2, v.lumi_time);
      else {
        time_federr->det_fill_1d(v.traj_p1_fill, 2,                  t.mod_on, 2, v.lumi_time);
        time_federr->det_fill_1d(v.traj_p1_fill, t.mod_on.federr-22, t.mod_on, 2, v.lumi_time);
      }
    }
    // 3:
    // Fraction of affected hits in event with Fed error of type x
    // Numerator is the same as for Module Fed error rate, but only selecting events with Fed error x
    // Don't fill 3) but copy from 2): h[p1][p2][p3][3]->Add(h[p1][p2][p3][2])
    // !! Copy before Normalizing Nhit with fed error by Nhits
    // Numerator   (3): Nhit_with_federr        (h[p1][n][p3][3], where n=2,3, ... 16,17)
    // Denumerator (4): [Nhit in event with FE] (h[p1][n][p3][4], where n=2,3, ... 16,17)
    //  4:
    // Number of hits in event with Fed error of type x:
    // Denumerator of 3
    for (int i=0; i<19; i++) if (v.traj_evt_federr_type[i]==1) {
      time_federr  ->det_fill_1d(0,              i,                  t.mod_on, 4, v.lumi_time);
      if (v.traj_p1_fill!=NOVAL_I)
        time_federr->det_fill_1d(v.traj_p1_fill, i,                  t.mod_on, 4, v.lumi_time);
    }
  }
  
  void federr_evt_fill_(Variables &v, const EventData &e) {
    federr_evt->h1d(0,0)->Fill(-1);
    if (e.federrs_size==0) federr_evt->h1d(0,0)->Fill(0);
    else for (int i=0; i<e.federrs_size; i++) federr_evt->h1d(0,0)->Fill(e.federrs[i][1]);
    if (v.evt_p1_fill!=NOVAL_I) {
      federr_evt->h1d(v.evt_p1_fill, 0)->Fill(-1);
      if (e.federrs_size==0) federr_evt->h1d(v.evt_p1_fill, 0)->Fill(0);
      else for (int i=0; i<e.federrs_size; i++) federr_evt->h1d(v.evt_p1_fill, 0)->Fill(e.federrs[i][1]);
    }
    // Fed Error Rate
    // Numerator:   Sum(federr_mult) (h[p1][n][0][5], where n=2,3, ... 16,17)
    // Denumerator: Nevents          (h[p1][0][0][5])
    // Numerator
    for (int i=0; i<19; i++) if (v.evt_federr_mult[i]!=0) {
      time_federr->h1d(0,i,0,5)->Fill(v.lumi_time,v.evt_federr_mult[i]);
      if (v.evt_p1_fill!=NOVAL_I)
	time_federr->h1d(v.evt_p1_fill,i,0,5)->Fill(v.lumi_time,v.evt_federr_mult[i]);
    }
    // Fed error Multiplicity
    // Number of FED errors per Event with FED error
    // Numerator:   Sum(federr_mult) (h[p1][n][0][5], where n=2,3, ... 16,17)
    // Denumerator: Nevents_with_FE  (h[p1][n][0][6], where n=2,3, ... 16,17)
    // Denumerator:
    for (int i=0; i<19; i++) if (v.evt_federr_mult[i]!=0) {
      time_federr->h1d(0,i,0,6)->Fill(v.lumi_time);
      if (v.evt_p1_fill!=NOVAL_I)
	time_federr->h1d(v.evt_p1_fill,i,0,6)->Fill(v.lumi_time);
    }
  }

  void federr_evt_postfill_(Variables &v, const EventData &e) {
    // Variance of Fed error Multiplicity
    // Number of FED errors per Event with FED error
    // Multiplicity:
    // Numerator:   federrs_size   (h[p1][n][0][5], where n=2,3, ... 16,17)
    // Denumerator: Nevent_with_FE (h[p1][n][0][6], where n=2,3, ... 16,17)
    Int_t binx = time_federr->h1d(0,0,0,0)->FindBin(v.lumi_time);
    for (int i=0; i<19; i++) if (v.evt_federr_mult[i]!=0) {
      double SumMult = time_federr->h1d(0,i,0,5)->GetBinContent(binx);
      double N = time_federr->h1d(0,i,0,6)->GetBinContent(binx);
      double Diff = (v.evt_federr_mult[i] - SumMult / N);
      time_federr->h1d(0,i,0,7)->Fill(v.lumi_time, Diff*Diff/N);
      if (v.evt_p1_fill!=NOVAL_I)
	time_federr->h1d(v.evt_p1_fill,i,0,7)->Fill(v.lumi_time, Diff*Diff/N);
    }
  }
  
  void federr_calc_(Variables &v) {
    for (int p1=0; p1<=v.lumi_nfill; p1++) {
      for (int p3=0; p3<10; p3++) {
	// copy: num of 3 -> num of 2
	for (int n=0; n<19; n++) time_federr->h1d(p1,n,p3,3)->Add(time_federr->h1d(p1,n,p3,2));
	// divide num of 2 by den of 2
	for (int n=1; n<19; n++) time_federr->h1d(p1,n,p3,2)->Divide(time_federr->h1d(p1,0,p3,2));
	// divide num of 3 by den of 4
	for (int n=2; n<19; n++) time_federr->h1d(p1,n,p3,3)->Divide(time_federr->h1d(p1,n,p3,4));
      }
      // first 6: divide h[p1][n][0][5] by h[p1][n][0][6]
      // then  5: divide h[p1][n][0][5] by h[p1][0][0][5]
      for (int n=2; n<19; n++) {
	time_federr->h1d(p1,n,0,6)->Divide(time_federr->h1d(p1,n,0,5),time_federr->h1d(p1,n,0,6));
	time_federr->h1d(p1,n,0,5)->Divide(time_federr->h1d(p1,0,0,5));
	// Set Errors of Multiplicity plot to Standard Deviation of Multiplicities - sqrt(var)
	for (int binx=1; binx<=time_federr->h1d(p1,n,0,6)->GetNbinsX(); binx++) {
	  double Var = time_federr->h1d(p1,n,0,7)->GetBinContent(binx);
	  time_federr->h1d(p1,n,0,6)->SetBinError(binx,sqrt(Var));
	}
      }
    }
  }

  //__________________________________________________________________________________________
  //                                   Finishing Sequences
  //__________________________________________________________________________________________

  void run_label_reorder_(Variables &var) {
    int last = 0;
    for (size_t k=0; k<var.lumi_run.size(); k++) if (run->h1d(0,0,1)->GetBinContent(k+1)>0) {
      // check for previous empty bins
      size_t prev = k;
      // find skippable bins (nhit==0)
      while (prev!=0&&run->h1d(0,0,1)->GetBinContent(prev)==0) prev--;
      prev++;
      std::stringstream sstr;
      sstr<<var.lumi_run[k];
      for (int i=0; i<10; i++) for (int mod=0; mod<5; mod++) for (int j=0; j<11; j++) {
	// remove skippable bins
	if (prev!=k+1) {
	  run->h1d(i,mod,j)->SetBinContent(prev,run->h1d(i,mod,j)->GetBinContent(k+1));
	  run->h1d(i,mod,j)->SetBinContent(k+1,0);
	}
	run->h1d(i,mod,j)->GetXaxis()->SetBinLabel(prev,sstr.str().c_str());
      }
      last = prev;
    }
    for (int i=0; i<10; i++) for (int mod=0; mod<5; mod++) for (int j=0; j<11; j++)
      run->h1d(i,mod,j)->GetXaxis()->SetRange(1,last);
  }
  
  void create_graph_(int n, int c) {
    TGraph *v95 = new TGraph(n);
    v95->SetMarkerColor(c);
    v95->SetMarkerStyle(8);
    TGraph *v50 = new TGraph(n);
    v50->SetMarkerColor(c);
    v50->SetMarkerStyle(8);
    TGraph *tw = new TGraph(n);
    tw->SetMarkerColor(c);
    tw->SetMarkerStyle(8);
    hv_totlumi.push_back(v95);
    hv_totlumi.push_back(v50);
    hv_totlumi.push_back(tw);
  }

  TF1* fit_turnon_(double p1, double p2, TH1D *hv) {
    if (hv->GetEntries()>0) {
      TF1* f = new TF1("turnon", "1/(1+exp(([0]-x)/[1]))", 15, 155);
      f->SetParameters(p1,p2);
      hv->Fit("turnon","RMQ0");
      return hv->GetFunction("turnon");
    } else return NULL;
  }
  double v95_(double p1, double p2, TH1D *h) { 
    TF1 *f = fit_turnon_(p1,p2,h); 
    if (f!=0) {
      double v95 =  f->GetParameter(0) + 2.94443898 * f->GetParameter(1); 
      return v95;
    } else return 0;
  }
  double v50_(double p1, double p2, TH1D *h) { 
    TF1 *f = fit_turnon_(p1,p2,h);
    if (f!=0) {
      double v50 =  f->GetParameter(0);
      return v50;
    } else return 0;
  }
  double tw_(double p1, double p2, TH1D *h) { 
    TF1 *f = fit_turnon_(p1,p2,h); 
    if (f!=0) {
      double tw =  f->GetParameter(1);
      return tw;
    } else return 0;
  }

  void linear_yzfit_slope_(TH3D *h, TH1D *slopex, int from_origo=0) {
    if (h->GetEntries()) {
      Int_t nx = h->GetNbinsX();
      Int_t ny = h->GetNbinsY();
      Double_t ymin = h->GetYaxis()->GetXmin();
      Double_t ymax = h->GetYaxis()->GetXmax();
      Int_t nz = h->GetNbinsZ();
      Double_t zmin = h->GetZaxis()->GetXmin();
      Double_t zmax = h->GetZaxis()->GetXmax();
      for (int i=1; i<nx; i++) {
	TH2D *yz_slice = new TH2D("yz_slice","yz_slice", ny, ymin, ymax, nz, zmin, zmax);
	for (int j=1; j<ny; j++) for (int k=1; k<nz; k++) {
	  double bincont = h->GetBinContent(i,j,k);
	  if (bincont) yz_slice->SetBinContent(j,k,bincont);
	}
	if (yz_slice->GetEntries()) {
	  TF1 *f;
	  if (from_origo) f = new TF1("linear", "[0]*x", ymin, ymax);
	  else f = new TF1("linear", "[0]*x+[1]", ymin, ymax);
	  TProfile *prof = yz_slice->ProfileX();
	  prof->Fit("linear","RMQ0");
	  slopex->SetBinContent(i,f->GetParameter(0));
	  slopex->SetBinError(i,f->GetParError(0));
	  delete f;
	  delete prof;
	}
	delete yz_slice;
      }
    }
  }

  void calc_nclu_plots_(Variables &v) {
    for (int fill=1; fill<=v.lumi_nfill; fill++) {
      for (int det=0; det<10; det++) {
	for (int binx=1; binx<bx_ls->h2d(fill,det,0)->GetNbinsX(); binx++) {
	  int nbx = 0;
	  double allavgnclu = 0;
	  for (int biny=1; biny<=3600; biny++) {
	    double nclu = bx_ls->h2d(fill,det,0)->GetBinContent(binx, biny);
	    double nevt = bx_ls->h2d(fill,det,1)->GetBinContent(binx, biny);
	    if (nclu>0) {
	      double bxavg = nclu/nevt;
	      allavgnclu += bxavg;
	      nbx++;
	    }
	  }
	  if (nbx>0) {
	    double avgnclu = allavgnclu/nbx;
	    double sumdiffsq = 0;
	    for (int biny=1; biny<=3600; biny++) {
	      double nclu = bx_ls->h2d(fill,det,0)->GetBinContent(binx, biny);
	      double nevt = bx_ls->h2d(fill,det,1)->GetBinContent(binx, biny);
	      if (nclu>0) {
		double bxavg = nclu/nevt;
		sumdiffsq += (bxavg-avgnclu) * (bxavg-avgnclu);
	      }
	    }
	    double stdev = sqrt(sumdiffsq/nbx);
	    ls_fill->h1d(fill,det,1)->SetBinContent(binx,avgnclu);
	    ls_fill->h1d(fill,det,1)->SetBinError(binx,stdev);
	  }
	}
	ls_fill->h1d(fill,det,2)->Divide(ls_fill->h1d(fill,det,3));
	//ls_fill->h1d(fill,det,0)->Divide(ls_fill->h1d(fill,det,2),ls_fill->h1d(fill,det,1));
      }
    }
    for (int fill=0; fill<=v.lumi_nfill; fill++) {
      for (int det=1; det<24; det++) {
	if (det<10) {
	  if (fill==0) totlumi_ratio->h1d(det)->Divide(totlumi_ratio->h1d(0));
	  // time
	  time    ->h1d(fill, det, 2)->Divide(time    ->h1d(fill, 0, 2));
	}
	instlumi->h1d(fill, det, 2)->Divide(instlumi->h1d(fill, 0, 2));
      }
      for (int i=0; i<4; i++)
	linear_yzfit_slope_(instlumi->h3d(fill, i),instlumi_corr->h1d(fill, i),1);
    }
  }

  void get_gaus_mean_(TH2D* dist, TH1D* mean, bool debug = 0, bool with_error = 0) {
    Int_t nbiny = dist->GetNbinsY();
    double ylow = dist->GetYaxis()->GetXmin();
    double yup = dist->GetYaxis()->GetXmax();
    for (int i=1; i<=dist->GetNbinsX(); i++) {
      TH1D *yfit = new TH1D("yfit","yfit",nbiny,ylow,yup);
      double entries = 0;
      int maxbin = 2;
      for (int j=1; j<=nbiny; j++) {
	double cont = dist->GetBinContent(i,j);
	if (cont&&maxbin<j) maxbin=j;
	entries += cont;
	yfit->SetBinContent(j,cont);
      }
      if (entries>10) {
	TF1 *f = new TF1("gaus","gaus",ylow,yfit->GetXaxis()->GetBinUpEdge(maxbin));
	yfit->Fit("gaus","RMWQ");
	mean->SetBinContent(i,yfit->GetMean());
	if (with_error) mean->SetBinError(i,f->GetParameter(2));
	if (debug) {
	  std::stringstream ss;
	  ss<<mean->GetTitle()<<"_bin"<<i;
	  TCanvas c(ss.str().c_str());
	  yfit->GetXaxis()->SetRange(1,maxbin);
	  yfit->Draw();
	  f->Draw("same");
	  c.SaveAs(std::string("~/Make_histogram/Fit_samples/"+ss.str()+".png").c_str());
	  c.Write();
	  std::cout<<ss.str()<<" mean: "<<yfit->GetMean()<<" gaus fit: "<<f->GetParameter(1)<<" +- "<<f->GetParameter(2)<<std::endl;
	}
	delete f;
      }
      delete yfit;
    }
  }

  void get_landau_mpv_(TH2D* ncdist, TH1D* mpv) {
    if (ncdist->GetEntries()) {
      Int_t nbiny = ncdist->GetNbinsY();
      double ylow = ncdist->GetYaxis()->GetXmin();
      double yup = ncdist->GetYaxis()->GetXmax();
      for (int i=1; i<=ncdist->GetNbinsX(); i++) {
	TH1D *yfit = new TH1D("yfit","yfit",nbiny,ylow,yup);
	double entries = 0;
	int maxbin = 2;
	for (int j=1; j<=nbiny; j++) {
	  double cont = ncdist->GetBinContent(i,j);
	  if (cont&&maxbin<j) maxbin=j;
	  entries += cont;
	  yfit->SetBinContent(j,cont);
	}
	if (entries>100) {
	  TF1 *f = new TF1("landau","landau",ylow, yfit->GetXaxis()->GetBinUpEdge(maxbin));
	  yfit->Fit("landau","RMWQ0");
	  while (f->GetParameter(1)<ylow&&maxbin>1) {
	    maxbin--;
	    delete f;
	    f = new TF1("landau","landau",ylow, yfit->GetXaxis()->GetBinUpEdge(maxbin));
	    yfit->Fit("landau","RMWQ0");
	  }
	  mpv->SetBinContent(i,f->GetParameter(1));
	  mpv->SetBinError(i,f->GetParError(1));
	  delete f;
	  }
	delete yfit;
      }
    }
  }


  void get_landau_mpv_(TH2D* ncdist, TH1D* mpv, TH1D* help, bool debug = 0) {
    if (ncdist->GetEntries()) {
      Int_t nbiny = ncdist->GetNbinsY();
      double ylow = ncdist->GetYaxis()->GetXmin();
      double yup = ncdist->GetYaxis()->GetXmax();
      for (int i=1; i<=ncdist->GetNbinsX(); i++) {
	if (help->GetBinContent(i)) { // A helping plot to see if point had any data at all, saves time
	  TH1D *yfit = new TH1D("yfit","yfit",nbiny,ylow,yup);
	  double entries = 0;
	  int maxbin = 2;
	  for (int j=1; j<=nbiny; j++) {
	    double cont = ncdist->GetBinContent(i,j);
	    if (cont&&maxbin<j) maxbin=j;
	    entries += cont;
	    yfit->SetBinContent(j,cont);
	  }
	  if (entries>100) {
	    TF1 *f = new TF1("landau","landau",ylow, yfit->GetXaxis()->GetBinUpEdge(maxbin));
	    yfit->Fit("landau","RMWQ0");
	    while (f->GetParameter(1)<ylow&&maxbin>1) {
	      maxbin--;
	      delete f;
	      f = new TF1("landau","landau",ylow, yfit->GetXaxis()->GetBinUpEdge(maxbin));
	      yfit->Fit("landau","RMWQ0");
	    }
	    mpv->SetBinContent(i,f->GetParameter(1));
	    mpv->SetBinError(i,f->GetParError(1));
	    if (debug) {
	      std::stringstream ss;
	      ss<<mpv->GetTitle()<<"_bin"<<i;
	      TCanvas c(ss.str().c_str());
	      yfit->GetXaxis()->SetRange(1,maxbin);
	      yfit->Draw();
	      f->Draw("same");
	      c.SaveAs(std::string("Fit_samples/"+ss.str()+".png").c_str());
	      c.Write();
	      std::cout<<ss.str()<<" Mean: "<<yfit->GetMean()<<" Landau MPV: "<<f->GetParameter(1)<<" Sigma: "<<f->GetParameter(2)<<std::endl;
	    }
	    delete f;
	  }
	  delete yfit;
	}
      }
    }
  }

  void get_mpv_(TH2D* ncdist, TH1D* mpv, TH1D* help) {
    if (ncdist->GetEntries()) {
      Int_t nbiny = ncdist->GetNbinsY();
      double ylow = ncdist->GetYaxis()->GetXmin();
      double yup = ncdist->GetYaxis()->GetXmax();
      for (int i=1; i<=ncdist->GetNbinsX(); i++) {
	if (help->GetBinContent(i)) { // A helping plot to see if point had any data at all, saves time
	  TH1D *yfit = new TH1D("yfit","yfit",nbiny,ylow,yup);
	  double entries = 0;
	  for (int j=1; j<=nbiny; j++) {
	    double cont = ncdist->GetBinContent(i,j);
	    entries += cont;
	    yfit->SetBinContent(j,cont);
	  }
	  if (entries>100) {
	    double xmax = yfit->GetBinCenter(yfit->GetMaximumBin());
	    double ymax = yfit->GetMaximum();
	    TF1 *f = new TF1("fit","landau", (xmax<25 ? 0 : xmax-25), xmax+25);
	    f->SetParameter(0, 5.5*ymax);
	    f->SetParLimits(0, 4*ymax,7*ymax);
	    f->SetParameter(1, xmax);
	    f->SetParLimits(1, xmax<5 ? 0: xmax-5, xmax + 5);
	    f->SetParameter(2, 2.3);
	    f->SetParLimits(2, 0.1, 5);
	    yfit->Fit("fit","RQ0");
// 	    double Chi2NDoF = f->GetChisquare()/f->GetNDF();
	    double Const = f->GetParameter(0);
//  	    double MPV = f->GetParameter(1);
	    double Sigma = f->GetParameter(2);
// 	    double YMax = f->GetMaximum();
	    double XMax = f->GetMaximumX();
	    //std::cout<<printf( "Chi2/NDoF: %10.1f  Const: %2.1f  MPV: %3.1f  Sigma: %2.2f", Chi2NDoF, Const/ymax, MPV, Sigma)<<std::endl;
	    delete f;
	    f = new TF1("fit","landau(0)+gaus(3)", (XMax<25 ? 0 : XMax-25), XMax+25);
	    f->SetParameter(0, 1.4*Const);
	    f->SetParLimits(0, 2*ymax,10*ymax);
	    f->SetParameter(1, XMax);
	    f->SetParLimits(1, XMax<6.5 ? 1.5: XMax-5, XMax + 5);
	    f->SetParameter(2, Sigma);
	    f->SetParLimits(2, 0.1,10);
	    f->SetParameter(3, -0.75*ymax);
	    f->SetParLimits(3, -1.5*ymax,ymax);
	    f->SetParameter(4, XMax);
	    f->SetParLimits(4, 1, 45);
	    f->SetParameter(5, Sigma);
	    f->SetParLimits(5, 0, 8);
	    yfit->Fit("fit","RQ0");
// 	    Chi2NDoF = f->GetChisquare()/f->GetNDF();
// 	    double old_Const = Const;
// 	    Const = f->GetParameter(0);
// 	    MPV = f->GetParameter(1);
	    Sigma = f->GetParameter(2);
// 	    double Const2 = f->GetParameter(3);
// 	    double Mean = f->GetParameter(4);
// 	    double Sigma2 = f->GetParameter(5);
// 	    YMax = f->GetMaximum();
	    XMax = f->GetMaximumX();
	    //         std::cout<<printf( "Chi2/NDoF: %4.1f  C1: %2.1f  MPV: %3.1f  S1: %2.2f  C2: %5.2f  M: %3.1f  S2: %2.2f", Chi2NDoF, Const/ymax, MPV, Sigma, Const2/ymax, Mean, Sigma2)<<std::endl;
	    mpv->SetBinContent(i,XMax);
	    mpv->SetBinError(i,Sigma);
	  }
	  delete yfit;
	}
      }
    }
  }
  
  TF1* fit_turnon_improved_(TH1D *h, TH1D* dist) {
    if (h->GetEntries()>0) {
      // Estimate Parameters
      float p0 = 0;
      float first_point = 0;
      for (int i=h->GetNbinsX(); i>0; i--) if (dist->GetBinContent(i)>0) {
	p0 = h->GetBinContent(i);
	first_point = h->GetBinCenter(i);
      }
      float p1 = h->GetBinContent(h->GetMaximumBin()) - p0;
      int bin1 = 0;
      int bin2=9999;
      float mid = (p0 + p1) / 2.0;
      for (int i=h->GetNbinsX(); i>0; i--) {
        double cont = h->GetBinContent(i);
        if (cont<mid&&cont>0&&bin1==0) bin1 =i;
        if (cont>mid) bin2 = i;
      }
      double x1 = h->GetBinCenter(bin1);
      double x2 = h->GetBinCenter(bin2);
      double y1 = h->GetBinContent(bin1);
      double y2 = h->GetBinContent(bin2);
      float p2 = x1+ (x2-x1)*(mid-y1)/(y2-y1);
      // Fit
      if (std::string(h->GetName()).find("hv_l1_HV16_eff")==0) first_point = 7.5;
      TF1* f = new TF1("turnon", "[0]+[1]/(1+exp(([2]-x)/[3]))", first_point, 150);
      f->SetParameter(0,p0);
      f->SetParameter(1,p1);
      f->SetParameter(2,p2);
      f->SetParameter(3,3.1);
      f->SetParLimits(0,0,0.6);
      f->SetParLimits(1,0,1);
      //f->SetParLimits(2,0,1);
      f->SetParLimits(3,0.25,10);
      f->SetLineColor(h->GetMarkerColor());
      h->Fit("turnon","RMQ");
      // double Chi2NDoF = f->GetChisquare()/f->GetNDF();
      // double Const = f->GetParameter(0);
      // double Rise = f->GetParameter(1);
      // double Mid = f->GetParameter(2);
      // double Width = f->GetParameter(3);
      // std::cout<<printf( "Chi2/NDoF: %10.1f  Const: %1.3f  Rise: %1.3f  Mid: %3.1f  Width: %2.2lf", Chi2NDoF, Const, Rise, Mid, Width)<<std::endl;
      // gPad->Update();
      // if (fabs(Mid-p2)>4) sleep(5);
      return h->GetFunction("turnon");
    } else return NULL;
  }

  double vturnon_(TH1D* h) { 
    TF1 *f = h->GetFunction("turnon");
    if (f!=0) return f->GetX(f->GetMaximum()-0.01);
    else return NOVAL_F;
  }

  void calc_hv_scans_(Variables &v, PostFixes &p) {
    calc_mpv_for_hv_scans(p); // Recalc MPV

    TCanvas c("c");
    //-------------------------------
    //   Vturnon vs totlumi plots  //
    //-------------------------------
    // Turnon fits
    for (size_t i=0; i<p.hv_l1.size(); i++) fit_turnon_improved_(hv_l1->h1d(i,0),hv_l1->h1d(i,1));
    for (size_t i=0; i<p.hv_l2.size(); i++) fit_turnon_improved_(hv_l2->h1d(i,0),hv_l2->h1d(i,1));
    for (size_t i=0; i<p.hv_l3.size(); i++) fit_turnon_improved_(hv_l3->h1d(i,0),hv_l3->h1d(i,1));
    for (size_t i=0; i<p.hv_d1.size(); i++) fit_turnon_improved_(hv_d1->h1d(i,0),hv_d1->h1d(i,1));
    for (size_t i=0; i<p.hv_l1full.size(); i++) for (int j=0; j<61; j++) fit_turnon_improved_(hv_l1full->h1d(i,j,0),hv_l1full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_l2full.size(); i++) for (int j=0; j<73; j++) fit_turnon_improved_(hv_l2full->h1d(i,j,0),hv_l2full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_l3full.size(); i++) for (int j=0; j<85; j++) fit_turnon_improved_(hv_l3full->h1d(i,j,0),hv_l3full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_d1full.size(); i++) for (int j=0; j<64; j++) fit_turnon_improved_(hv_d1full->h1d(i,j,0),hv_d1full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_d2full.size(); i++) for (int j=0; j<64; j++) fit_turnon_improved_(hv_d2full->h1d(i,j,0),hv_d2full->h1d(i,j,1));
    
    // Vturnon plots
    bool drawMpvFit = true;

    size_t nscan[5] = { p.hv_l1.size(), p.hv_l2.size(), p.hv_l3.size(), p.hv_d1.size(), p.hv_d2full.size() };
    Histograms* AllHv[5] = { hv_l1, hv_l2, hv_l3, hv_d1, hv_d2full };
    for (int det=0; det<5; ++det) for (size_t i=0; i<nscan[det]; ++i) {
      TH1D* eff = det<4 ? AllHv[det]->h1d(i,0) : AllHv[det]->h1d(i,0,0);
      TH1D* mpv = det<4 ? AllHv[det]->h1d(i,2) : AllHv[det]->h1d(i,0,2);
      double vturnon = vturnon_(eff);
      double maximum = mpv->GetMaximum();
      mpv->Scale(1.0/maximum);
      TF1* f = new TF1("mpvturnon", "([0]+[1]*x)/(1+exp(([2]-x)/[3]))", det==4 ? 10 : det==2&&i<3 ? 50 : 20,150);
      f->SetParLimits(0,0.9,1);
      f->SetParLimits(1,0.0001,0.01);
      f->SetParLimits(2,0,70);
      f->SetParLimits(3,0,50);
      mpv->Fit("mpvturnon","RMQ0");
      vturnon_totlumi->h1d(det,0)->Fill(v.totlumi_scan[det][i], vturnon);
      if (!(det==3&&(i>=0&&i<=4))) {
	vturnon_totlumi->h1d(det,1)->Fill(v.totlumi_scan[det][i], f->Eval(vturnon)*maximum);
	vturnon_totlumi->h1d(det,2)->Fill(v.totlumi_scan[det][i], f->Eval(vturnon));
      }
      mpv->Scale(maximum);
      // Redraw the function after rescaling
      if (drawMpvFit) {
	f = new TF1("mpvturnon2", "([0]+[1]*x)/(1+exp(([2]-x)/[3]))", det==4 ? 10 : det==2&&i<3 ? 50 : 20,150);
	f->SetParLimits(0,0.9*maximum,1*maximum);
	f->SetParLimits(1,0.0001*maximum,0.01*maximum);
	f->SetParLimits(2,0,70);
	f->SetParLimits(3,0,50);
	mpv->Fit("mpvturnon2","RMQ");
	if (det==3&&i<p.hv_d1full.size()) { // Draw the fit for d1full plot too
	  mpv = hv_d1full->h1d(i,0,2);
	  maximum = mpv->GetMaximum();
	  f = new TF1("mpvturnon2", "([0]+[1]*x)/(1+exp(([2]-x)/[3]))", 20,150);
	  f->SetParLimits(0,0.9*maximum,1*maximum);
	  f->SetParLimits(1,0.0001*maximum,0.01*maximum);
	  f->SetParLimits(2,0,70);
	  f->SetParLimits(3,0,50);
	  mpv->Fit("mpvturnon2","RMQ");
	}
      }
      //TCanvas *can = new TCanvas(mpv->GetName());
      //TF1* g = new TF1("mpvturnon2", "([0]+[1]*x)/(1+exp(([2]-x)/[3]))", det==4 ? 10 : det==2&&i<3 ? 50 : 20,150);
      //for (int i=0; i<4; ++i) g->SetParameter(i,f->GetParameter(i));
      //g->SetParLimits(0,0.9*maximum,1*maximum);
      //g->SetParLimits(1,0.0001*maximum,0.01*maximum);
      //g->SetParLimits(2,0,70);
      //g->SetParLimits(3,0,50);
      //mpv->Fit("mpvturnon2","RMQ0");
      //mpv->Draw();
      //g->Draw("same");
      //can->SaveAs(std::string("fit_plots/"+std::string(mpv->GetName())+".png").c_str());
    }

    // Delete error bars on Mpv plots
    for (size_t i=0; i<p.hv_l1.size(); i++) for (int j=1; j<=hv_l1->h1d(i,2)->GetNbinsX(); ++j) hv_l1->h1d(i,2)->SetBinError(j,0);
    for (size_t i=0; i<p.hv_l2.size(); i++) for (int j=1; j<=hv_l2->h1d(i,2)->GetNbinsX(); ++j) hv_l2->h1d(i,2)->SetBinError(j,0);
    for (size_t i=0; i<p.hv_l3.size(); i++) for (int j=1; j<=hv_l3->h1d(i,2)->GetNbinsX(); ++j) hv_l3->h1d(i,2)->SetBinError(j,0);
    for (size_t i=0; i<p.hv_d1.size(); i++) for (int j=1; j<=hv_d1->h1d(i,2)->GetNbinsX(); ++j) hv_d1->h1d(i,2)->SetBinError(j,0);
    for (size_t i=0; i<p.hv_l1full.size(); i++) for (int j=0; j<61; j++) for (int k=1; k<=hv_l1full->h1d(i,j,2)->GetNbinsX(); ++k) hv_l1full->h1d(i,j,2)->SetBinError(k,0);
    for (size_t i=0; i<p.hv_l2full.size(); i++) for (int j=0; j<73; j++) for (int k=1; k<=hv_l2full->h1d(i,j,2)->GetNbinsX(); ++k) hv_l2full->h1d(i,j,2)->SetBinError(k,0);
    for (size_t i=0; i<p.hv_l3full.size(); i++) for (int j=0; j<85; j++) for (int k=1; k<=hv_l3full->h1d(i,j,2)->GetNbinsX(); ++k) hv_l3full->h1d(i,j,2)->SetBinError(k,0);
    for (size_t i=0; i<p.hv_d1full.size(); i++) for (int j=0; j<64; j++) for (int k=1; k<=hv_d1full->h1d(i,j,2)->GetNbinsX(); ++k) hv_d1full->h1d(i,j,2)->SetBinError(k,0);
    for (size_t i=0; i<p.hv_d2full.size(); i++) for (int j=0; j<64; j++) for (int k=1; k<=hv_d2full->h1d(i,j,2)->GetNbinsX(); ++k) hv_d2full->h1d(i,j,2)->SetBinError(k,0);
    
    // Set error to 0, so no error bars are drawn with P9E1
    for (int i=0; i<5; i++) for (int j=0; j<3; ++j) for (int k=1; k<=vturnon_totlumi->h1d(i,j)->GetNbinsX(); ++k)
      if (vturnon_totlumi->h1d(i,j)->GetBinContent(k)>0) vturnon_totlumi->h1d(i,j)->SetBinError(k,0.00001);
    //-------------------------------
    //      HV Group Plots         //
    //-------------------------------
    
    // FPix Inner/Outer Blade plots
    //fpix
    //0 - all
    //1-32 - HV Groups
    //  1-8 - 1I,1O,2I,2O ... - BpI,BpO,BmI,BmO
    //33-39 - Pnl1Plq1 --> Pnl1Plq4
    //40-63 - Bld -12 .. +12
    vturnon_fpix = new Histograms("vturnon_fpix",  2,1); // scan, histo
    // Assume D1 and D2 Fulls scans are done at the same time
    if (p.hv_d1full.size()!=p.hv_d2full.size()) std::cout<<"Allhistos - Warning: Full Disk1/2 scan not taken at the same time"<<std::endl;
    vturnon_fpix ->add(p.hv_d1full, 1, 1, p.hv_d1full.size(), 0, p.fpix_grp,  1, 1, 4,  9, -4.5, 4.5);
    vturnon_fpix ->add(p.hv_d1full, 0, 1, p.hv_d1full.size(), 0, p.fpix_grp,  1, 5, 6,  7,  0.5, 7.5);
    // fpix
    for (size_t scan=0; scan<p.hv_d1full.size(); scan++) {
      // Merge histograms
      for (int dsk=0; dsk<2; dsk++) {
	Histograms *H = (dsk==0) ? hv_d1full : hv_d2full;
	for (int shlIO=0; shlIO<2; shlIO++) {
	  for (int IO=0; IO<2; IO++) {
	    for (int HV=0; HV<4; HV++) {
	      // Merging
	      TH1D *h = new TH1D("h","h", 62, -2.5, 307.5);
	      TH1D *h2 = new TH1D("h2","h2", 62, -2.5, 307.5);
	      for (int bin=1; bin<=62; bin++) {
		double val = 0;
		double mis = 0;
		for (int shlpm=0; shlpm<2; shlpm++) {
		  double eff  = H->h1d(scan, 1+shlpm*16+shlIO*8+HV*2+IO, 0)->GetBinContent(bin);
		  double stat = H->h1d(scan, 1+shlpm*16+shlIO*8+HV*2+IO, 1)->GetBinContent(bin);
		  val+= eff*stat;
		  mis+= (1-eff)*stat;
		}
		h->SetBinContent(bin, val/(val+mis));
		h2->SetBinContent(bin, val+mis);
	      }
	      fit_turnon_improved_(h, h2);
	      vturnon_fpix->h1d(scan, dsk*2+IO)->Fill((shlIO==0) ? HV+1 : -HV-1, vturnon_(h));
	      delete h;
	      delete h2;
	    }
	  }
	}
      }
    }
    //bpix
    vturnon_bpix = new Histograms("vturnon_bpix",  3,1); // layer, scan, histo
    vturnon_bpix ->add(p.det, 1, 4, 4, 3, p.hv_l1full, 1, 1, p.hv_l1full.size(), 0, p.bpix_grp,  1, 1, 1,  17, -8.5, 8.5);
    vturnon_bpix ->add(p.det, 0, 4, 4, 3, p.hv_l1full, 0, 1, p.hv_l1full.size(), 0, p.bpix_grp,  1, 2, 2,  17, -4.5, 4.5);
    vturnon_bpix ->add(p.det, 1, 5, 5, 3, p.hv_l2full, 1, 1, p.hv_l2full.size(), 0, p.bpix_grp,  1, 1, 1,  17, -8.5, 8.5);
    vturnon_bpix ->add(p.det, 0, 5, 5, 3, p.hv_l2full, 0, 1, p.hv_l2full.size(), 0, p.bpix_grp,  1, 2, 2,  17, -4.5, 4.5);
    vturnon_bpix ->add(p.det, 1, 6, 6, 3, p.hv_l3full, 1, 1, p.hv_l3full.size(), 0, p.bpix_grp,  1, 1, 1,  17, -8.5, 8.5);
    vturnon_bpix ->add(p.det, 0, 6, 6, 3, p.hv_l3full, 0, 1, p.hv_l3full.size(), 0, p.bpix_grp,  1, 2, 2,  17, -4.5, 4.5);
    //0 - all
    //1-32 - HV Groups
    //  1-8 + N*8 (Sec1-8) - BpI,BpO,BmI,BmO
    //33-40 - Mod -4 to +4
    //41-84 - -1,+1,-2,+2 ...
    for (int lay=0; lay<3; lay++) {
      size_t nscan = (lay==0) ? p.hv_l1full.size() : (lay==1) ? p.hv_l2full.size() : p.hv_l3full.size();
      Histograms *H = (lay==0) ? hv_l1full : (lay==1) ? hv_l2full : hv_l3full;
      for (size_t scan=0; scan<nscan; scan++) {
	// Sector Plots
	for (int shlIO=0; shlIO<2; shlIO++) {
	  for (int sec=0; sec<8; sec++) {
	    // Merging
	    TH1D *h = new TH1D("h","h", 31, -2.5, 152.5);
	    TH1D *h2 = new TH1D("h2","h2", 31, -2.5, 152.5);
	    for (int bin=1; bin<=31; bin++) {
	      double val = 0;
	      double mis = 0;
	      for (int shlpm=0; shlpm<2; shlpm++) {
		double eff  = H->h1d(scan, 1+shlpm*16+shlIO*8+sec, 0)->GetBinContent(bin);
		double stat = H->h1d(scan, 1+shlpm*16+shlIO*8+sec, 1)->GetBinContent(bin);
		val+= eff*stat;
		mis+= (1-eff)*stat;
	      }
	      h->SetBinContent(bin, val/(val+mis));
	      h2->SetBinContent(bin, val+mis);
	    }
	    fit_turnon_improved_(h, h2);
	    vturnon_bpix->h1d(lay, scan, 0)->Fill((shlIO==0) ? sec+1 : -sec-1, vturnon_(h));
	    delete h;
	    delete h2;
	  }
	}
	// Ring Plots
	for (int ring=0; ring<8; ring++) {
	  fit_turnon_improved_(H->h1d(scan, 33+ring, 0), H->h1d(scan, 33+ring, 1));
	  vturnon_bpix->h1d(lay, scan, 1)->Fill(ring-4 + (ring>3), vturnon_(H->h1d(scan, 33+ring, 0)));
	}
      }
    }
  }
  
  void load_delay_scans_() {
    TFile f_in("/home/jkarancs/Prev_scans_data/Timing_scan11_12_INC_SPL0_nstrip0.root");
    delay11->load(f_in);
    delay11_rog->load(f_in);
    delay11_mod->load(f_in);
    delay11_roc->load(f_in);
    delay12->load(f_in);
    delay12_rog->load(f_in);
    delay12_mod->load(f_in);
    delay12_roc->load(f_in);
    // Revert efficiency calc
    delay11->revert_eff();
    delay11_rog->revert_eff();
    delay11_mod->revert_eff();
    delay11_roc->revert_eff();
    delay12->revert_eff();
    delay12_rog->revert_eff();
    delay12_mod->revert_eff();
    delay12_roc->revert_eff();
    f_in.Close();
  }
  
public:
  
  void load_HV_scans(const char* input, PostFixes &p) {
    TFile f_in(input);
    for (size_t i=0; i<p.hv_l1.size()-1; i++) hv_l1->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_l2.size()-1; i++) hv_l2->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_l3.size()-1; i++) hv_l3->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_d1.size()-1; i++) hv_d1->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_l1full.size(); i++) hv_l1full->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_l2full.size(); i++) hv_l2full->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_l3full.size(); i++) hv_l3full->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_d1full.size(); i++) hv_d1full->partial_load(f_in, i);
    for (size_t i=0; i<p.hv_d2full.size(); i++) hv_d2full->partial_load(f_in, i);
    hv_l1->revert_eff();
    hv_l2->revert_eff();
    hv_l3->revert_eff();
    hv_d1->revert_eff();
    hv_l1full->revert_eff();
    hv_l2full->revert_eff();
    hv_l3full->revert_eff();
    hv_d1full->revert_eff();
    hv_d2full->revert_eff();
    f_in.Close();
  }
  
  void calc_mpv_for_hv_scans(PostFixes &p) {
    // HV Scans
    std::cout<<"Start Fitting MPV plots"<<std::endl;
    for (size_t i=0; i<p.hv_l1.size(); i++) get_mpv_(hv_l1->h2d(i,0),hv_l1->h1d(i,2),hv_l1->h1d(i,1));
    for (size_t i=0; i<p.hv_l2.size(); i++) get_mpv_(hv_l2->h2d(i,0),hv_l2->h1d(i,2),hv_l2->h1d(i,1));
    for (size_t i=0; i<p.hv_l3.size(); i++) get_mpv_(hv_l3->h2d(i,0),hv_l3->h1d(i,2),hv_l3->h1d(i,1));
    for (size_t i=0; i<p.hv_d1.size(); i++) get_mpv_(hv_d1->h2d(i,0),hv_d1->h1d(i,2),hv_d1->h1d(i,1));
    for (size_t i=0; i<p.hv_l1full.size(); i++) for (int j=0; j<61; j++) get_mpv_(hv_l1full->h2d(i,j,0),hv_l1full->h1d(i,j,2),hv_l1full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_l2full.size(); i++) for (int j=0; j<73; j++) get_mpv_(hv_l2full->h2d(i,j,0),hv_l2full->h1d(i,j,2),hv_l2full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_l3full.size(); i++) for (int j=0; j<85; j++) get_mpv_(hv_l3full->h2d(i,j,0),hv_l3full->h1d(i,j,2),hv_l3full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_d1full.size(); i++) for (int j=0; j<64; j++) get_mpv_(hv_d1full->h2d(i,j,0),hv_d1full->h1d(i,j,2),hv_d1full->h1d(i,j,1));
    for (size_t i=0; i<p.hv_d2full.size(); i++) for (int j=0; j<64; j++) get_mpv_(hv_d2full->h2d(i,j,0),hv_d2full->h1d(i,j,2),hv_d2full->h1d(i,j,1));
    std::cout<<"MPV fitting ready"<<std::endl;
  }

  void load_totlumi() {
    TFile f_in("/home/jkarancs/ROOT_output/v3029/TEST/Run2012A_May11JSON_MB_RECO_INC_SPL0_nstrip11.root");
    totlumi->load(f_in);
    totlumi->revert_eff();
    f_in.Close();
  }

  void calc_hv_scans(Variables &v, PostFixes &p) { calc_hv_scans_(v,p); }
  
  void postprocess(Variables &v, PostFixes &p) {
    // BAD ROC selection
#if BADROC != 0
    bool debug = 0;
    Int_t nbin = roceff_time->h3d(0,0,0)->GetNbinsX();
    Double_t low = roceff_time->h3d(0,0,0)->GetXaxis()->GetXmin();
    Double_t up = roceff_time->h3d(0,0,0)->GetXaxis()->GetXmax();
    // Filling ROC efficiency Distributions for each fill
    for (int fill=1; fill<=v.lumi_nfill; fill++) {
      if (debug) std::cout<<"Starting Fill "<<p.Fill[fill]<<std::endl;
      for (int p1=0; p1<8; p1++)
        for (int p2=0; p2<roceff_time->h3d(fill,p1,0)->GetNbinsY(); p2++)
          for (int p3=0; p3<roceff_time->h3d(fill,p1,0)->GetNbinsZ(); p3++) {
            double val = 0.0;
            double mis = 0.0;
            for (int t=1; t<=roceff_time->h3d(fill,p1,0)->GetNbinsX(); t++) {
              val += roceff_time->h3d(fill,p1,0)->GetBinContent(t,p2+1,p3+1);
              mis += roceff_time->h3d(fill,p1,1)->GetBinContent(t,p2+1,p3+1);
            }
            if (val+mis>0.0) roceff_dist->h1d(fill-1)->Fill(val/(val+mis));
          }
      if (debug) std::cout<<"0"<<std::endl;
      std::string fname = "f"+p.fill[fill];
      TF1 *f = new TF1(fname.c_str(),"gaus",0.9,1.0);
      roceff_dist->h1d(fill-1)->Fit(fname.c_str(),"RMWQ0");
      f = roceff_dist->h1d(fill-1)->GetFunction(fname.c_str());
      float mean = 0.995;
      float sigma = 0.0045;
      bool fit_succ = 0;
      if (f!=0) {
        fit_succ = 1;
        mean = f->GetParameter(1);
        sigma = f->GetParameter(2);
        if ((mean-10*sigma)<0.95) {
          std::cout<<"Not enough statistics for fill"<<p.fill[fill]
      	     <<" Continuing by excluding ROCs under 95%"<<std::endl;
          fit_succ = 0;
        }
      } else {
        std::cout<<"Fitting Unsuccessful for fill"<<p.fill[fill]
      	   <<" Excluding ROCs under 95% eff"<<std::endl;
      }
      if (debug) std::cout<<"1"<<std::endl;
      // Creating a BAD ROC List
      std::vector<std::vector<std::vector<int> > > list;
      list.resize(4); for (int i=0; i<4; i++) list[i].resize(3);
      for (int p1=0; p1<8; p1++)
        for (int p2=0; p2<roceff_time->h3d(fill,p1,0)->GetNbinsY(); p2++)
          for (int p3=0; p3<roceff_time->h3d(fill,p1,0)->GetNbinsZ(); p3++) {
            double val = 0.0;
            double mis = 0.0;
            for (int t=1; t<=roceff_time->h3d(fill,p1,0)->GetNbinsX(); t++) {
              val += roceff_time->h3d(fill,p1,0)->GetBinContent(t,p2+1,p3+1);
              mis += roceff_time->h3d(fill,p1,1)->GetBinContent(t,p2+1,p3+1);
            }
            double Eff = val/(val+mis);
            double Stat = val+mis;
            float sig = fabs(Eff - mean) / sigma;
            // 10 sigma, at least 1 hit, exclude BPix_BpI_SEC4_LYR3_LDR11F_MOD1,3 for fill 2040
            if (((fit_succ&&sig>10.0)||(!fit_succ&&Eff<=0.95))&&Stat>0) {
              // FPix
              if (p1>3) { list[0][0].push_back(p1); list[0][1].push_back(p2); list[0][2].push_back(p3); }
              // BPix
              else {
		if (p2<=10) { list[1][0].push_back(p1); list[1][1].push_back(p2); list[1][2].push_back(p3); }
		else if (p2<=26) { list[2][0].push_back(p1); list[2][1].push_back(p2); list[2][2].push_back(p3); }
		else { list[3][0].push_back(p1); list[3][1].push_back(p2); list[3][2].push_back(p3); }
              }
            }
          }
      if (debug) std::cout<<"2"<<std::endl;
      // Creating eff vs time plots for BAD ROCS
      // Also 
      int nbin_filled =0;
      int last_bin = 0;
      for (int lay=0; lay<=3; lay++) {
	if (debug) std::cout<<"3 Starting "<<p.Det[lay+2]<<std::endl;
        if (list[lay][0].size()>100) {
          std::cout<<"Warning!! Number of Bad ROCs over 100 for fill "
        	   <<p.Fill[fill]<<" on "<<p.Det[lay+2]<<std::endl;
        }
        badroc->h2d(fill-1,lay)->SetBins(nbin,low,up,list[lay][0].size(),0,list[lay][0].size());
        for (size_t nbad=0; nbad<list[lay][0].size(); nbad++) {
          int fil =0;
          for (int binx=1; binx<=roceff_time->h3d(fill,list[lay][0][nbad],0)->GetNbinsX(); binx++) {
            double val = roceff_time->h3d(fill,list[lay][0][nbad],0)->GetBinContent(binx,list[lay][1][nbad]+1,list[lay][2][nbad]+1);
            double mis = roceff_time->h3d(fill,list[lay][0][nbad],1)->GetBinContent(binx,list[lay][1][nbad]+1,list[lay][2][nbad]+1);
            double eff = val/ (val+mis);
            double stat = val+mis;
            if (stat>0) {
              fil++;
              if (binx>last_bin) last_bin = binx;
              if (eff==0.0) badroc->h2d(fill-1,lay)->SetBinContent(binx,list[lay][0].size()-nbad,0.0001);
              else badroc->h2d(fill-1,lay)->SetBinContent(binx,list[lay][0].size()-nbad,eff);
            }
            if (stat>0&&eff<0.1) {
	      nbadroc->h1d(fill-1,0)->SetBinContent(binx,nbadroc->h1d(fill-1,0)->GetBinContent(binx)+1);
	      if (lay>0) nbadroc->h1d(fill-1,1)->SetBinContent(binx,nbadroc->h1d(fill-1,1)->GetBinContent(binx)+1);
	      nbadroc->h1d(fill-1,2+lay)->SetBinContent(binx,nbadroc->h1d(fill-1,lay+2)->GetBinContent(binx)+1);
	    }
          }
          if (fil>nbin_filled) nbin_filled = fil;
          std::string rocname;
          if (lay!=0) {
            rocname = p.Detshl[list[lay][0][nbad]] + p.seclyrldr[list[lay][1][nbad]] + p.modroc[list[lay][2][nbad]];
            if (list[lay][2][nbad]%16<10) rocname = rocname + " ";
            if (list[lay][1][nbad]<9||(list[lay][1][nbad]>9&&list[lay][1][nbad]<19)||(list[lay][1][nbad]>25&&list[lay][1][nbad]<35))
              rocname = rocname + " ";
          } else {
            rocname = p.Detshl[list[lay][0][nbad]] + p.dskbld[list[lay][1][nbad]] + p.pnlplqroc[list[lay][2][nbad]];
            if (list[lay][1][nbad]%12<9) rocname = rocname + " ";
          }
          badroc->h2d(fill-1,lay)->GetYaxis()->SetBinLabel(list[lay][0].size()-nbad,rocname.c_str());
        }
      }
      if (debug) std::cout<<"4"<<std::endl;
      // Set ranges for time plots
      if (last_bin>1) {
	nbadroc->h1d(fill-1,0)->GetXaxis()->SetRange(1,last_bin);
	nbadroc->h1d(fill-1,1)->GetXaxis()->SetRange(1,last_bin);
        for (int lay=0; lay<=3; lay++) {
	  badroc->h2d(fill-1,lay)->GetXaxis()->SetRange(1,last_bin);
	  nbadroc->h1d(fill-1,2+lay)->GetXaxis()->SetRange(1,last_bin);
	}
	if (FILLS==2) {
	  for (int det=0; det<10; det++) {
#ifdef COMPLETE
	    for (int i=0; i<3; i++) time->h1d(fill,det,i)->GetXaxis()->SetRange(1,last_bin*2);
#else
	    for (int i=0; i<2; i++) time->h1d(fill,det,i)->GetXaxis()->SetRange(1,last_bin*2);
#endif
#if FEDERRPLOTS == 1
	    for (int type=0; type<21; type++) for (int p=0; p<7; p++) if (!((p>=5&&det>0)||(p>=2&&type>=19)))
	      time_federr->h1d(fill,type,det,p)->GetXaxis()->SetRange(1,last_bin*2);
#endif
	  }
	}
      }
      if (debug) std::cout<<"5"<<std::endl;
      // Checking for existing list and if it does not exist 
      // or current range is larger than the previous creating a new list
      FILE *output;
      std::string fillname;
      fillname = p.fill[fill];
      fillname.erase(0,1);
      std::stringstream ss_info,ss;
      ss_info<<"input/BADROC_EXCL/INFO/"
             <<fillname<<"_info.txt";
      ss<<"input/BADROC_EXCL/"
        <<fillname<<".txt";
      output = fopen(ss_info.str().c_str(),"r");
      if (output!=NULL) {
	int nbin_exist = 0;
        fscanf(output, "%d bins filled\n", &nbin_exist);
        fclose(output);
        if (nbin_exist>nbin_filled) {
          std::cout<<"Better Bad ROC List already exists for Fill "<<fillname<<std::endl;
        } else if (nbin_exist<nbin_filled) {
          std::cout<<"Bad ROC List exists for Fill "<<fillname
      	     <<" but larger statistics is available. Replacing..."<<std::endl;
          // same as below
          output = fopen(ss_info.str().c_str(),"w");
          fprintf(output, "%d bins filled\n",nbin_filled);
          fclose(output);
          output = fopen(ss.str().c_str(),"w");
          for (int lay=0; lay<4; lay++) {
            int ord = (lay<3) ? lay + 1 : 0;
            for (size_t nbad=0; nbad<list[ord][0].size(); nbad++)
              fprintf(output, "%d %d %d\n",list[ord][0][nbad],list[ord][1][nbad],list[ord][2][nbad]);
          }
          fclose(output);
        }
      } else {
        std::cout<<"No Bad ROC List exists for Fill "<<fillname<<". Creating a new... "<<std::endl;
        output = fopen(ss_info.str().c_str(),"w");
        fprintf(output, "%d bins filled\n",nbin_filled);
        fclose(output);
        output = fopen(ss.str().c_str(),"w");
        for (int lay=0; lay<4; lay++) {
          int ord = (lay<3) ? lay + 1 : 0;
          for (size_t nbad=0; nbad<list[ord][0].size(); nbad++)
            fprintf(output, "%d %d %d\n",list[ord][0][nbad],list[ord][1][nbad],list[ord][2][nbad]);
        }
        fclose(output);
      }
      if (debug) std::cout<<"6"<<std::endl;
    }
#if BADROC == 2
    // Create list of Persistent Bad ROCs
    TH3D *sum_eff  = new TH3D("sum_eff", "sum_eff",  8,-0.5,7.5, 48,-0.5,47.5, 64,-0.5,63.5);
    TH3D *sum_dist = new TH3D("sum_dist","sum_dist", 8,-0.5,7.5, 48,-0.5,47.5, 64,-0.5,63.5);
    int fill_high = 0;
    int last_fill = 0;
    double remaining_stat =0;
    double sumstat = 0;
    fill_stat->calc();
    for (int binx=1; binx<fill_stat->h1d(1)->GetNbinsX(); binx++) {
      double fillstat = fill_stat->h1d(1)->GetBinContent(binx);
      remaining_stat+= fillstat;
      if (fillstat>0) last_fill = binx;
    }
    std::cout<<"---------------------------------------------------------"<<std::endl;
    std::cout<<"Starting to Create Persistent BAD ROC LIST"<<std::endl;
    std::cout<<"Total statistics is: "<<remaining_stat<<std::endl;
    for (int fill=1; fill<=v.lumi_nfill; fill++) {
      if (fill_high==0) fill_high = fill;
      double fillstat = fill_stat->h1d(1)->GetBinContent(fill);
      if (fillstat>0) {
        remaining_stat -= fillstat;
        for (int p1=0; p1<8; p1++) {
          for (int p2=0; p2<roceff_fill->h3d(p1,0)->GetNbinsY(); p2++) {
            for (int p3=0; p3<roceff_fill->h3d(p1,0)->GetNbinsZ(); p3++) {
	      double val = roceff_fill->h3d(p1,0)->GetBinContent(fill,p2+1,p3+1);
	      double mis = roceff_fill->h3d(p1,1)->GetBinContent(fill,p2+1,p3+1);
	      sum_eff ->Fill(p1,p2,p3,val);
	      sum_dist->Fill(p1,p2,p3,mis);
	      sumstat += val+mis;
            }
          }
        }
        // If enough statistics (1 billion hit) look for persistent bad rocs
        // Look for persistent bad ROCs and create a list of them
        // In case of the last fills merge data with the previous
        if ((sumstat>1000000000&&remaining_stat>1000000000)||fill==last_fill) {
          sum_dist->Add(sum_eff);
          sum_eff->Divide(sum_dist);
          /*
            TH1D *roceff_pers = new TH1D("roceff_pers","roceff_pers", 1000,0,1);
            for (int p1=0; p1<8; p1++) for (int p2=0; p2<roceff_fill->h3d(p1,0)->GetNbinsY(); p2++)
            for (int p3=0; p3<roceff_fill->h3d(p1,0)->GetNbinsZ(); p3++) {
            double eff  = sum_eff ->GetBinContent(p1+1,p2+1,p3+1);
            double stat = sum_dist->GetBinContent(p1+1,p2+1,p3+1);
            if (stat>0) roceff_per->Fill(eff);
            }
            TF1 *f = new TF1("gaus_pers","gaus",0.9,1.0);
            roceff_pers->Fit("gaus_pers","RMWQ0");
            f = roceff_pers->GetFunction("gaus_pers");
            float mean = 0;
            float sigma = 0;
            if (f!=0) { mean = f->GetParameter(1); sigma = f->GetParameter(2); }
            delete roceff_pers, f;
          */
          // Creating a List of Persistent Low efficiency ROCs
          std::vector<std::vector<std::vector<int> > > list;
          list.resize(4); for (int i=0; i<4; i++) list[i].resize(3);
          for (int p1=0; p1<8; p1++) for (int p2=0; p2<roceff_fill->h3d(p1,0)->GetNbinsY(); p2++)
            for (int p3=0; p3<roceff_fill->h3d(p1,0)->GetNbinsZ(); p3++) {
	      double eff  = sum_eff ->GetBinContent(p1+1,p2+1,p3+1);
	      double stat = sum_dist->GetBinContent(p1+1,p2+1,p3+1);
	      // float sig = fabs(Eff - mean) / sigma;
	      // if (sig>10.0&&Stat>0) { // 10 sigma, at least 1 hit
	      if (eff<0.95&&stat>0) {
		if (p1>3) { list[0][0].push_back(p1); list[0][1].push_back(p2); list[0][2].push_back(p3); } // FPix
		else { // BPix
		  if (p2<=10) { list[1][0].push_back(p1); list[1][1].push_back(p2); list[1][2].push_back(p3); }
		  else if (p2<=26) { list[2][0].push_back(p1); list[2][1].push_back(p2); list[2][2].push_back(p3); }
		  else { list[3][0].push_back(p1); list[3][1].push_back(p2); list[3][2].push_back(p3); }
		}
	      }
            }
          FILE *pers_out;
          std::stringstream ss;
          ss<<"input/PERSISTENT_BADROC/"<<fill_high<<".txt";
          pers_out = fopen(ss.str().c_str(),"r");
          if (pers_out!=NULL) {
            std::cout<<"Warning!! Overwriting previous PERSISTENT BADROC List"<<std::endl;
            fclose(pers_out);
          }
          std::cout<<"Creating a new Persistent Bad ROC List for Fill "<<fill<<"-"<<fill_high<<std::endl;
          pers_out = fopen(ss.str().c_str(),"w");
          for (int lay=0; lay<4; lay++) {
            int ord = (lay<3) ? lay + 1 : 0;
            for (size_t nbad=0; nbad<list[ord][0].size(); nbad++)
	      fprintf(pers_out, "%d %d %d\n",list[ord][0][nbad],list[ord][1][nbad],list[ord][2][nbad]);
          }
          fclose(pers_out);
          delete sum_eff;
          delete sum_dist;
          sum_eff  = new TH3D("sum_eff", "sum_eff",  8,-0.5,7.5, 48,-0.5,47.5, 64,-0.5,63.5);
          sum_dist = new TH3D("sum_dist","sum_dist", 8,-0.5,7.5, 48,-0.5,47.5, 64,-0.5,63.5);
          fill_high = 0;
          sumstat = 0;
        }
      }
    }
#endif
#endif
#if FEDERRPLOTS == 1
    federr_calc_(v);
#endif
    for (int i=0; i<10; i++) {
      totlumi->h1d(i,4)->Divide(totlumi->h1d(i,5));
      totlumi->h1d(i,6)->Divide(totlumi->h1d(i,7));
      totlumi->h1d(i,8)->Divide(totlumi->h1d(i,9));
    }
#if SCANPLOTS == 1
    calc_mpv_for_hv_scans(p);
#elif SCANPLOTS == 2
    // Timing Scans
    std::cout<<"Start Fitting MPV plots"<<std::endl;
    bool fit_debug=0;
    // TFile* f=0;
    // if (fit_debug) f = TFile::Open("Fit_samples.root","recreate");
    load_delay_scans_();
    for (int i=0; i<10; i++) {
      get_landau_mpv_(delay11->h2d(i,0),delay11->h1d(i,4),delay11->h1d(i,1), fit_debug);
      get_landau_mpv_(delay12->h2d(i,0),delay12->h1d(i,4),delay12->h1d(i,1), fit_debug);
      get_landau_mpv_(delay3->h2d(i,0),delay3->h1d(i,4),delay3->h1d(i,1), fit_debug);
      get_landau_mpv_(delay4->h2d(i,0),delay4->h1d(i,4),delay4->h1d(i,1), fit_debug);
    }
    for (int i=0; i<4; i++) for (int j=0; j<10; j++) for (int k=0; (j<8) ? k<2 : k<4; k++) {
      get_landau_mpv_(delay11_rog->h2d(i,j,k,0),delay11_rog->h1d(i,j,k,4),delay11_rog->h1d(i,j,k,1), fit_debug);
      get_landau_mpv_(delay12_rog->h2d(i,j,k,0),delay12_rog->h1d(i,j,k,4),delay12_rog->h1d(i,j,k,1), fit_debug);
      get_landau_mpv_(delay3_rog->h2d(i,j,k,0),delay3_rog->h1d(i,j,k,4),delay3_rog->h1d(i,j,k,1), fit_debug);
      get_landau_mpv_(delay4_rog->h2d(i,j,k,0),delay4_rog->h1d(i,j,k,4),delay4_rog->h1d(i,j,k,1), fit_debug);
    }
    for (int i=0; i<8; i++) for (int j=0; (i<4) ? j<48 : j<24; j++) {
      for (int k=0; (i<4) ? k<4 : k<7; k++) {
        get_landau_mpv_(delay11_mod->h2d(i,j,k,0),delay11_mod->h1d(i,j,k,4),delay11_mod->h1d(i,j,k,1), fit_debug);
        get_landau_mpv_(delay12_mod->h2d(i,j,k,0),delay12_mod->h1d(i,j,k,4),delay12_mod->h1d(i,j,k,1), fit_debug);
        get_landau_mpv_(delay3_mod->h2d(i,j,k,0),delay3_mod->h1d(i,j,k,4),delay3_mod->h1d(i,j,k,1), fit_debug);
        get_landau_mpv_(delay4_mod->h2d(i,j,k,0),delay4_mod->h1d(i,j,k,4),delay4_mod->h1d(i,j,k,1), fit_debug);
      }
      for (int k=0; (i<4) ? k<64 : k<45; k++) {
        get_landau_mpv_(delay11_roc->h2d(i,j,k,0),delay11_roc->h1d(i,j,k,4),delay11_roc->h1d(i,j,k,1), fit_debug);
        get_landau_mpv_(delay12_roc->h2d(i,j,k,0),delay12_roc->h1d(i,j,k,4),delay12_roc->h1d(i,j,k,1), fit_debug);
        get_landau_mpv_(delay3_roc->h2d(i,j,k,0),delay3_roc->h1d(i,j,k,4),delay3_roc->h1d(i,j,k,1), fit_debug);
        get_landau_mpv_(delay4_roc->h2d(i,j,k,0),delay4_roc->h1d(i,j,k,4),delay4_roc->h1d(i,j,k,1), fit_debug);
      }
    }
    if (fit_debug) f->Close();
    // Track loss
    for (int i=0; i<10; i++) {
      trkloss->h1d(i,0)->Divide(trkloss->h1d(0,1));
      trkloss->h1d(i,0)->Divide(delay3->h1d(3,0));
    }
#endif

    // Incidence angles - avg norm charge
    for (int i=0; i<5; i++) for (int j=0; j<10; j++)
      angle->h1d(i,j,3)->Divide(angle->h1d(i,j,2));
    for (int j=0; j<10; j++) angle2d->h2d(0,j,3)->Divide(angle2d->h2d(0,j,2));
    
    // Average L1 trigger rate
    for (int i=1; i<=3; i++) for (int j=0; j<24; j++)
      instlumi_raw->h1d(0,i,j,2)->Divide(instlumi_raw->h1d(0,i,j,3));
  }

  void calc_write(Variables &var) {
    // Efficiency
    ls->finish();
    totlumi->finish();
    nvtx->finish();
#ifdef COMPLETE
    occup->finish();
    occup_mod->finish();
    occup_roc->finish();
    occup_dcol->finish();
#elif VERSION2 >= 34 && SPLIT > 0
    occup->finish();
    occup_mod->finish();
    occup_roc->finish();
    occup_dcol->finish();
#endif
    run_label_reorder_(var);
    run->finish();
    det->finish();
    mod->finish();
    roc->finish();
    time->finish();
    time2->finish();
    bx->finish();
    instlumi->finish();
    instlumi_raw->finish();
    il_l1rate->finish();
    l1rate->finish();
    trigger->finish();
#if FEDERRPLOTS == 1
    federr->finish();
    federr_evt->finish();
    time_federr->finish();
#endif
#if BADROC ==1
    // roceff_time->finish(); // File too large to write out
    // roceff_fill->finish(); // Do not need to write out
    // fill_stat->finish(); // Do not need to write out
    roceff_dist->finish();
    badroc->finish();
    nbadroc->finish();
#endif

    l1rate_vs_instlumi->Write();
    dynamic_ineff->finish();
    rocmap->finish();

    // N-1
    pt->finish();
    nstrip->finish();
    dz->finish();
    d0->finish();
    dz_d0->finish();
    normchi2->finish();
    fid_lxly->finish();
    fid_lx->finish();
    fid_ly->finish();
    vtxd0->finish();
    vtxz->finish();
    hitclu->finish();
    cluclu->finish();
    hithit->finish();
    //   clcl_hcl->finish();
    //   hh_hcl->finish();
    //   neweff->finish();
    //   neweff2->finish();
    angle->finish();
    angle2d->finish();
    nstrip_new->finish();

    // Not Efficiency
#if NONEFFPLOTS == 1
#ifdef COMPLETE
    calc_nclu_plots_(var);
    nclu_corr->finish();
    npix_corr->finish();
    bx_ls->finish();
    ls_fill->finish();
    totlumi_ratio->finish();
    instlumi_corr->finish();
#endif
    ncd->finish();
    ncd_corr->finish();
    pcd->finish();
    pcd_corr->finish();
    pcd_heta->finish();
    pcd_corr_heta->finish();
    clu_size->finish();
    clu_sizeX->finish();
    clu_sizeY->finish();
    clu_size_heta->finish();
    clu_sizeX_heta->finish();
    clu_sizeY_heta->finish();
    alpha->finish();
    beta->finish();
#endif
    
    // Scans
#if SCANPLOTS == 1
    hv_l1->finish();
    hv_l2->finish();
    hv_l3->finish();
    hv_d1->finish();
    hv_l1full->finish();
    hv_l2full->finish();
    hv_l3full->finish();
    hv_d1full->finish();
    hv_d2full->finish();
#elif SCANPLOTS == 2
    delay11->finish();
    delay11_rog->finish();
    delay11_mod->finish();
    delay11_roc->finish();
    delay12->finish();
    delay12_rog->finish();
    delay12_mod->finish();
    delay12_roc->finish();
    delay3->finish();
    delay3_rog->finish();
    delay3_mod->finish();
    delay3_roc->finish();
    delay4->finish();
    delay4_rog->finish();
    delay4_mod->finish();
    delay4_roc->finish();
    trkloss->finish();
#endif

    std::cout<<"Final Calculations Ready!"<<std::endl;
  }
  
};
