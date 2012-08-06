#define VERSION1 23

class TreeReader {
 
 public:
  TreeReader(int tg_traj_evt, int tg_traj_traj, int tg_traj_mod_on, int tg_traj_clu, int tg_traj_trk, int tg_traj_mod,
	     int tg_clu_evt,int tg_clu_mod_on, int tg_clu_mod, int tg_clu_clu) {
    tg_traj_evt_        = tg_traj_evt;
    tg_traj_traj_       = tg_traj_traj;
    tg_traj_mod_on_     = tg_traj_mod_on;
    tg_traj_clu_        = tg_traj_clu;
    tg_traj_trk_        = tg_traj_trk;
    tg_traj_mod_        = tg_traj_mod;
    tg_clu_evt_         = tg_clu_evt;
    tg_clu_mod_on_      = tg_clu_mod_on;
    tg_clu_mod_         = tg_clu_mod;
    tg_clu_clu_         = tg_clu_clu;
  }
  ~TreeReader() { }

 private:
  TFile f_;

  // Toggle reading branches on-off
  int tg_traj_evt_;
  int tg_traj_traj_; 
  int tg_traj_mod_on_; 
  int tg_traj_clu_; 
  int tg_traj_trk_; 
  int tg_traj_mod_;
  int tg_clu_evt_;
  int tg_clu_mod_on_; 
  int tg_clu_mod_;
  int tg_clu_clu_;
  
  // Trees
  TTree *lumiTree_;
  TTree *eventTree_;
  TTree *trajTree_;
#ifdef COMPLETE
  TTree *clustTree_;
#endif
  
  // Data structures
  LumiData        lumi_;
  EventData       evt_;
  EventData       traj_evt_;
  TrajMeasurement traj_;
  EventData       clu_evt_;
  Cluster         clu_;
  TrackData       trk_;
  Digi            digi_;
  
  // Number of entries
  long long int nls_;
  long long int nevt_;
  long long int nhit_;
  long long int nclu_;
  
  void read_(TFile &f) {
    // lumiTree
    lumiTree_ = (TTree*)f.Get("lumiTree");
    nls_ = (long long int)lumiTree_->GetEntries();
    TBranch *b_l_lumi    = lumiTree_->GetBranch("lumi");
    b_l_lumi    ->SetAddress(&lumi_);
    // eventTree
    eventTree_ = (TTree*)f.Get("eventTree");
    nevt_ = (long long int)eventTree_->GetEntries();
    TBranch *b_e_evt    = eventTree_->GetBranch("event");
    b_e_evt     ->SetAddress(&evt_);
    // trajTree
    trajTree_ = (TTree*)f.Get("trajTree");
    nhit_ = (long long int)trajTree_->GetEntries();
#ifndef SPLIT
    TBranch *b_t_evt     = trajTree_->GetBranch("event");
    TBranch *b_t_traj    = trajTree_->GetBranch("traj");
    TBranch *b_t_mod_on  = trajTree_->GetBranch("module_on");
    TBranch *b_t_clu     = trajTree_->GetBranch("clust");
    TBranch *b_t_trk     = trajTree_->GetBranch("track");
    b_t_evt     ->SetAddress(&traj_evt_);
    b_t_traj    ->SetAddress(&traj_);
    b_t_mod_on  ->SetAddress(&traj_.mod_on);
    b_t_clu     ->SetAddress(&traj_.clu);
    b_t_trk     ->SetAddress(&traj_.trk);
    trajTree_   ->SetBranchStatus("event",     tg_traj_evt_);
    trajTree_   ->SetBranchStatus("traj",      tg_traj_traj_);
    trajTree_   ->SetBranchStatus("module_on", tg_traj_mod_on_);
    trajTree_   ->SetBranchStatus("clust",     tg_traj_clu_);
    trajTree_   ->SetBranchStatus("track",     tg_traj_trk_);
    nclu_ = 0;
#ifdef COMPLETE
    TBranch *b_t_mod     = trajTree_->GetBranch("module");
    b_t_mod     ->SetAddress(&traj_.mod);
    trajTree_   ->SetBranchStatus("module",    tg_traj_mod_);
    // clustTree
    clustTree_ = (TTree*)f.Get("clustTree");
    nclu_  = (long long int)clustTree_->GetEntries();
    TBranch *b_c_evt     = clustTree_->GetBranch("event");
    TBranch *b_c_mod     = clustTree_->GetBranch("module");
    TBranch *b_c_mod_on  = clustTree_->GetBranch("module_on");
    TBranch *b_c_clu     = clustTree_->GetBranch("clust");
    b_c_evt     ->SetAddress(&clu_evt_);
    b_c_mod     ->SetAddress(&clu_.mod);
    b_c_mod_on  ->SetAddress(&clu_.mod_on);
    b_c_clu     ->SetAddress(&clu_);
    clustTree_  ->SetBranchStatus("event",     tg_clu_evt_);
    clustTree_  ->SetBranchStatus("module",    tg_clu_mod_);
    clustTree_  ->SetBranchStatus("module_on", tg_clu_mod_on_);
    clustTree_  ->SetBranchStatus("clust",     tg_clu_clu_);
#endif
#else
    /************************************ Split mode - GetBranch ********************************/
    // Non-splitted branches
    TBranch *traj_event                = trajTree_->GetBranch("event");
    TBranch *traj_module_on            = trajTree_->GetBranch("module_on");
    
    // traj
    // Non-splitted branch
    TBranch *traj_traj                 = trajTree_->GetBranch("traj");
    // Paired branches
    TBranch *traj_traj_alphabeta       = trajTree_->GetBranch("traj_alphabeta");
    // Split-mode branches
#ifdef COMPLETE
    TBranch *traj_traj_lz              = trajTree_->GetBranch("traj_lz");
    TBranch *traj_traj_glx             = trajTree_->GetBranch("traj_glx");
    TBranch *traj_traj_gly             = trajTree_->GetBranch("traj_gly");
    TBranch *traj_traj_glz             = trajTree_->GetBranch("traj_glz");
    TBranch *traj_traj_lxmatch         = trajTree_->GetBranch("traj_lxmatch");
    TBranch *traj_traj_lymatch         = trajTree_->GetBranch("traj_lymatch");
    TBranch *traj_traj_norm_charge     = trajTree_->GetBranch("traj_norm_charge");
    TBranch *traj_traj_i               = trajTree_->GetBranch("traj_i");
    TBranch *traj_traj_onedge          = trajTree_->GetBranch("traj_onedge");
    TBranch *traj_traj_inactive        = trajTree_->GetBranch("traj_inactive");
    TBranch *traj_traj_badhit          = trajTree_->GetBranch("traj_badhit");
    //   TBranch *traj_traj_telescope       = trajTree_->GetBranch("traj_telescope");
    //   TBranch *traj_traj_telescope_valid = trajTree_->GetBranch("traj_telescope_valid");
    //   TBranch *traj_traj_dmodule         = trajTree_->GetBranch("traj_dmodule");
    //   TBranch *traj_traj_dladder         = trajTree_->GetBranch("traj_dladder");
    //   TBranch *traj_traj_glmatch         = trajTree_->GetBranch("traj_glmatch");
    //   TBranch *traj_traj_lx_err          = trajTree_->GetBranch("traj_lx_err");
    //   TBranch *traj_traj_ly_err          = trajTree_->GetBranch("traj_ly_err");
    //   TBranch *traj_traj_lz_err          = trajTree_->GetBranch("traj_lz_err");
    //   TBranch *traj_traj_lxymatch        = trajTree_->GetBranch("traj_lxymatch");
    //   TBranch *traj_traj_res_hit         = trajTree_->GetBranch("traj_res_hit");
    //   TBranch *traj_traj_sig_hit         = trajTree_->GetBranch("traj_sig_hit");
    //   TBranch *traj_traj_d_cl            = trajTree_->GetBranch("traj_d_cl");
#endif
    
    // clust
    // Paired branches
    TBranch *traj_clust_xy             = trajTree_->GetBranch("clust_xy");
    TBranch *traj_clust_sizeXY         = trajTree_->GetBranch("clust_sizeXY");
    // Split-mode branches
#ifdef COMPLETE
    //   TBranch *traj_clust_i              = trajTree_->GetBranch("clust_i");
    TBranch *traj_clust_edge           = trajTree_->GetBranch("clust_edge");
    TBranch *traj_clust_badpix         = trajTree_->GetBranch("clust_badpix");
    TBranch *traj_clust_tworoc         = trajTree_->GetBranch("clust_tworoc");
#endif
    TBranch *traj_clust_size           = trajTree_->GetBranch("clust_size");
    TBranch *traj_clust_charge         = trajTree_->GetBranch("clust_charge");
    TBranch *traj_clust_pix            = trajTree_->GetBranch("clust_pix");
    
    // track
    // Non-splitted branch
    TBranch *traj_track                = trajTree_->GetBranch("track");
    // Paired branches
    TBranch *traj_track_ndofchi2       = trajTree_->GetBranch("track_ndofchi2");
    // Split-mode branches
    TBranch *traj_track_eta            = trajTree_->GetBranch("track_eta");
    TBranch *traj_track_phi            = trajTree_->GetBranch("track_phi");
#ifdef COMPLETE
    TBranch *traj_track_theta          = trajTree_->GetBranch("track_theta");
    TBranch *traj_track_p              = trajTree_->GetBranch("track_p");
    TBranch *traj_track_algo           = trajTree_->GetBranch("track_algo");
    TBranch *traj_track_i              = trajTree_->GetBranch("track_i");
    TBranch *traj_track_pix            = trajTree_->GetBranch("track_pix");
    TBranch *traj_track_pixhit         = trajTree_->GetBranch("track_pixhit");
    TBranch *traj_track_validpixhit    = trajTree_->GetBranch("track_validpixhit");
    TBranch *traj_track_fpix           = trajTree_->GetBranch("track_fpix");
    TBranch *traj_track_bpix           = trajTree_->GetBranch("track_bpix");
    TBranch *traj_track_highPurity     = trajTree_->GetBranch("track_highPurity");
    //   TBranch *traj_track_fromVtx        = trajTree_->GetBranch("track_fromVtx");
#endif
    
#ifdef COMPLETE
    // Non-splitted branches
    TBranch *clust_event               = clustTree_->GetBranch("event");
    TBranch *clust_module_on           = clustTree_->GetBranch("mod_on");
    
    // clust
    // Paired branches
    TBranch *clust_clust_xy            = clustTree_->GetBranch("clust_xy");
    TBranch *clust_clust_sizeXY        = clustTree_->GetBranch("clust_sizeXY");
    // Split-mode branches
    TBranch *clust_clust_i             = clustTree_->GetBranch("clust_i");
    TBranch *clust_clust_edge          = clustTree_->GetBranch("clust_edge");
    TBranch *clust_clust_badpix        = clustTree_->GetBranch("clust_badpix");
    TBranch *clust_clust_tworoc        = clustTree_->GetBranch("clust_tworoc");
    TBranch *clust_clust_size          = clustTree_->GetBranch("clust_size");
    TBranch *clust_clust_charge        = clustTree_->GetBranch("clust_charge");
    TBranch *clust_clust_pix           = clustTree_->GetBranch("clust_pix");
#endif
    
    /************************************ SetAddress ******************************************/
    // Non-splitted branches
    traj_event                 -> SetAddress(&traj_evt_);
    traj_module_on             -> SetAddress(&traj_.mod_on);
    
    // traj
    // Non-splitted branch
    traj_traj                  -> SetAddress(&traj_);
    // Paired branches
    traj_traj_alphabeta        -> SetAddress(&traj_.alpha);
    // Split-mode branches
#ifdef COMPLETE
    traj_traj_lz              -> SetAddress(&traj_.lz);
    traj_traj_glx             -> SetAddress(&traj_.glx);
    traj_traj_gly             -> SetAddress(&traj_.gly);
    traj_traj_glz             -> SetAddress(&traj_.glz);
    traj_traj_lxmatch         -> SetAddress(&traj_.lxmatch);
    traj_traj_lymatch         -> SetAddress(&traj_.lymatch);
    traj_traj_norm_charge     -> SetAddress(&traj_.norm_charge);
    traj_traj_i               -> SetAddress(&traj_.i);
    traj_traj_onedge          -> SetAddress(&traj_.onedge);
    traj_traj_inactive        -> SetAddress(&traj_.inactive);
    traj_traj_badhit          -> SetAddress(&traj_.badhit);
    //   traj_traj_telescope       -> SetAddress(&traj_.telescope);
    //   traj_traj_telescope_valid -> SetAddress(&traj_.telescope_valid);
    //   traj_traj_dmodule         -> SetAddress(&traj_.dmodule);
    //   traj_traj_dladder         -> SetAddress(&traj_.dladder);
    //   traj_traj_glmatch         -> SetAddress(&traj_.glmatch);
    //   traj_traj_lx_err          -> SetAddress(&traj_.lx_err);
    //   traj_traj_ly_err          -> SetAddress(&traj_.ly_err);
    //   traj_traj_lz_err          -> SetAddress(&traj_.lz_err);
    //   traj_traj_lxymatch        -> SetAddress(&traj_.lxymatch);
    //   traj_traj_res_hit         -> SetAddress(&traj_.res_hit);
    //   traj_traj_sig_hit         -> SetAddress(&traj_.sig_hit);
    //   traj_traj_d_cl            -> SetAddress(&traj_.d_cl);
#endif
    
    // clust
    // Paired branches
    traj_clust_xy             -> SetAddress(&traj_.clu.x);
    traj_clust_sizeXY         -> SetAddress(&traj_.clu.sizeX);
    // Split-mode branches
#ifdef COMPLETE
    //   traj_clust_i              -> SetAddress(&traj_.clu.i);
    traj_clust_edge           -> SetAddress(&traj_.clu.edge);
    traj_clust_badpix         -> SetAddress(&traj_.clu.badpix);
    traj_clust_tworoc         -> SetAddress(&traj_.clu.tworoc);
#endif
    traj_clust_size           -> SetAddress(&traj_.clu.size);
    traj_clust_charge         -> SetAddress(&traj_.clu.charge);
    traj_clust_pix            -> SetAddress(&traj_.clu.pix);
    
    // track
    // Non-splitted branch
    traj_track                -> SetAddress(&traj_.trk);
    // Paired branches
    traj_track_ndofchi2       -> SetAddress(&traj_.trk.ndof);
    // Split-mode branches
    traj_track_eta            -> SetAddress(&traj_.trk.eta);
    traj_track_phi            -> SetAddress(&traj_.trk.phi);
#ifdef COMPLETE
    traj_track_theta          -> SetAddress(&traj_.trk.theta);
    traj_track_p              -> SetAddress(&traj_.trk.p);
    traj_track_algo           -> SetAddress(&traj_.trk.algo);
    traj_track_i              -> SetAddress(&traj_.trk.i);
    traj_track_pix            -> SetAddress(&traj_.trk.pix);
    traj_track_pixhit         -> SetAddress(&traj_.trk.pixhit);
    traj_track_validpixhit    -> SetAddress(&traj_.trk.validpixhit);
    traj_track_fpix           -> SetAddress(&traj_.trk.fpix);
    traj_track_bpix           -> SetAddress(&traj_.trk.bpix);
    traj_track_highPurity     -> SetAddress(&traj_.trk.highPurity);
    //   traj_track_fromVtx        -> SetAddress(&traj_.trk.fromVtx);
#endif
    
#ifdef COMPLETE
    // Non-splitted branches
    clust_event               -> SetAddress(&clu_evt_);
    clust_module_on           -> SetAddress(&clu_.mod_on);
    
    // clust
    // Paired branches
    clust_clust_xy            -> SetAddress(&clu_.x);
    clust_clust_sizeXY        -> SetAddress(&clu_.sizeX);
    // Split-mode branches
    clust_clust_i             -> SetAddress(&clu_.i);
    clust_clust_edge          -> SetAddress(&clu_.edge);
    clust_clust_badpix        -> SetAddress(&clu_.badpix);
    clust_clust_tworoc        -> SetAddress(&clu_.tworoc);
    clust_clust_size          -> SetAddress(&clu_.size);
    clust_clust_charge        -> SetAddress(&clu_.charge);
    clust_clust_pix           -> SetAddress(&clu_.pix);
#endif

    /******************* SetBranchStatus - Uncomment to disable certain branches  *******************/
    
    /*     trajTree_->SetBranchStatus("event",0); */
    /*     trajTree_->SetBranchStatus("module_on",0); */

    /*     trajTree_->SetBranchStatus("traj",0); */
    /*     trajTree_->SetBranchStatus("traj_alphabeta",0); */
#ifdef COMPLETE
    /*     trajTree_->SetBranchStatus("traj_lz",0); */
    /*     trajTree_->SetBranchStatus("traj_glx",0); */
    /*     trajTree_->SetBranchStatus("traj_gly",0); */
    /*     trajTree_->SetBranchStatus("traj_glz",0); */
    /*     trajTree_->SetBranchStatus("traj_lxmatch",0); */
    /*     trajTree_->SetBranchStatus("traj_lymatch",0); */
    /*     trajTree_->SetBranchStatus("traj_norm_charge",0); */
    /*     trajTree_->SetBranchStatus("traj_i",0); */
    /*     trajTree_->SetBranchStatus("traj_onedge",0); */
    /*     trajTree_->SetBranchStatus("traj_inactive",0); */
    /*     trajTree_->SetBranchStatus("traj_badhit",0); */
    /* /\*     trajTree_->SetBranchStatus("traj_telescope",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_telescope_valid",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_dmodule",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_dladder",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_glmatch",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_lx_err",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_ly_err",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_lz_err",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_lxymatch",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_res_hit",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_sig_hit",0); *\/ */
    /* /\*     trajTree_->SetBranchStatus("traj_d_cl",0); *\/ */
#endif
    
    /*     trajTree_->SetBranchStatus("clust_xy",0); */
    /*     trajTree_->SetBranchStatus("clust_sizeXY",0); */
#ifdef COMPLETE
    /*     //   trajTree_->SetBranchStatus("clust_i",0); */
    /*     trajTree_->SetBranchStatus("clust_edge",0); */
    /*     trajTree_->SetBranchStatus("clust_badpix",0); */
    /*     trajTree_->SetBranchStatus("clust_tworoc",0); */
#endif
    /*     trajTree_->SetBranchStatus("clust_size",0); */
    /*     trajTree_->SetBranchStatus("clust_charge",0); */
    /*     trajTree_->SetBranchStatus("clust_pix",0); */
    
    /*     trajTree_->SetBranchStatus("track",0); */
    /*     trajTree_->SetBranchStatus("track_ndofchi2",0); */
    /*     trajTree_->SetBranchStatus("track_eta",0); */
    /*     trajTree_->SetBranchStatus("track_phi",0); */
#ifdef COMPLETE
    /*     trajTree_->SetBranchStatus("track_theta",0); */
    /*     trajTree_->SetBranchStatus("track_p",0); */
    /*     trajTree_->SetBranchStatus("track_algo",0); */
    /*     trajTree_->SetBranchStatus("track_i",0); */
    /*     trajTree_->SetBranchStatus("track_pix",0); */
    /*     trajTree_->SetBranchStatus("track_pixhit",0); */
    /*     trajTree_->SetBranchStatus("track_validpixhit",0); */
    /*     trajTree_->SetBranchStatus("track_fpix",0); */
    /*     trajTree_->SetBranchStatus("track_bpix",0); */
    /*     trajTree_->SetBranchStatus("track_highPurity",0); */
    /*     /\*     trajTree_->SetBranchStatus("track_fromVtx",0); *\/ */
#endif
    
#ifdef COMPLETE
    /*     clustTree_->SetBranchStatus("event",0); */
    /*     clustTree_->SetBranchStatus("mod_on",0); */
    
    /*     clustTree_->SetBranchStatus("clust_xy",0); */
    /*     clustTree_->SetBranchStatus("clust_sizeXY",0); */
    /*     clustTree_->SetBranchStatus("clust_i",0); */
    /*     clustTree_->SetBranchStatus("clust_edge",0); */
    /*     clustTree_->SetBranchStatus("clust_badpix",0); */
    /*     clustTree_->SetBranchStatus("clust_tworoc",0); */
    /*     clustTree_->SetBranchStatus("clust_size",0); */
    /*     clustTree_->SetBranchStatus("clust_charge",0); */
    /*     clustTree_->SetBranchStatus("clust_pix",0); */
#endif
#endif
  }
  
 public:
  //Accessors
  // Entries
  long long int nls()  { return nls_; }
  long long int nevt() { return nevt_; }
  long long int nhit() { return nhit_; }
  long long int nclu() { return nclu_; }
  // Read Entries
  void lumi_read(long long int i) { lumiTree_->GetEntry(i); }
  void evt_read(long long int i)  { eventTree_->GetEntry(i); }
  void traj_read(long long int i) { trajTree_->GetEntry(i); }
#ifdef COMPLETE
  void clu_read(long long int i) { clustTree_->GetEntry(i); }
#endif
  // Memory adress of entry
  const LumiData&        lumi()     { return lumi_; }
  const EventData&       evt()      { return evt_; }
  const EventData&       traj_evt() { return traj_evt_; }
  const TrajMeasurement& traj()     { return traj_; }
#ifdef COMPLETE
  const EventData&       clu_evt()  { return clu_evt_; }
  const Cluster&         clu()      { return clu_; }
#endif
  // Modifiers
  void readtrees(TFile &f) { read_(f); }
  void neweff_redef(int &neweff) {
    if (neweff==1&&traj_.missing==1&&sqrt(traj_.dx_cl[0]*traj_.dx_cl[0]+traj_.dy_cl[0]*traj_.dy_cl[0])<0.05) {
      traj_.missing=0;
      traj_.validhit=1;
    }
  }
  
};
