#include "DataStruct.h"
#include "TChain.h"
#include "TCanvas.h"
#include <cmath>
#include <iostream>
#include "TH3.h"
#include "TF1.h"

//Binning parameter, lumisection range
int orbmin;
int orbmax;
int plq;

//Declare trees
TChain *trajTree;
TChain *clustTree;

// Declare branches
TBranch *b_t_evt;
TBranch *b_t_traj;
TBranch *b_t_mod;
TBranch *b_t_mod_on;
TBranch *b_t_clu;
TBranch *b_t_trk;

TBranch *b_c_evt;
TBranch *b_c_mod;
TBranch *b_c_mod_on;
TBranch *b_c_clu;

//Declare datasets
EventData evt,cluevt;
TrajMeasurement trajmeas;
// TrackData track;
Cluster clust;
// Digi digi;

float theta;
Int_t lscheck=0;
Int_t nvtxcheck=0;
Int_t runcheck=0;
Float_t vtxndofcheck=0.0;
Int_t evtcheck=0;
Int_t nfile=0;
Int_t nlumi=0;
Int_t nevent=0;
Int_t nhit=0;
Int_t totallumi=0;
Int_t totalevent=0;
Int_t totalentries=0;
Int_t totalhit=0;
Int_t nrun=0;
Int_t npix_bpix_evt=0;
Int_t npix_l1_evt=0;
Int_t npix_l2_evt=0;
Int_t npix_l3_evt=0;
Int_t nhitsperevt=0;
Int_t cl_roc_binx, cl_roc_biny;
Int_t npix_mod_l1_evt[9][21];
Int_t npix_mod_l2_evt[9][33];
Int_t npix_mod_l3_evt[9][45];
Int_t npix_roc_l1_evt[72][42];
Int_t npix_roc_l2_evt[72][66];
Int_t npix_roc_l3_evt[72][90];

Int_t latestgoodrange;

// Declare histograms
Int_t wbc, voltage_bpix, voltage_fpix;
Int_t stat_l1_m4_alldr=0;
Int_t delay_fine = -9999;
Int_t delay_coarse = -9999;
Int_t delaycheck = -9999;
Int_t lscompare = -9999;
Int_t lscount=0;
Int_t effloss, clustloop;

// TH1D *coarsedelay_l1_eff, *coarsedelay_l1_dist;
// TH1D *coarsedelay_l2_eff, *coarsedelay_l2_dist;
// TH1D *coarsedelay_l3_eff, *coarsedelay_l3_dist;
// TH1D *coarsedelay_dm2_eff, *coarsedelay_dm2_dist;
// TH1D *coarsedelay_dm1_eff, *coarsedelay_dm1_dist;
// TH1D *coarsedelay_dp1_eff, *coarsedelay_dp1_dist;
// TH1D *coarsedelay_dp2_eff, *coarsedelay_dp2_dist;
// TH1D *coarsedelay_bpix_eff, *coarsedelay_bpix_dist;
// TH1D *coarsedelay_fpix_eff, *coarsedelay_fpix_dist;
// TH1D *coarsedelay_eff, *coarsedelay_dist;

TH1D *coarsedelay_l1_avgclusize, *coarsedelay_l1_nclust;
TH1D *coarsedelay_l2_avgclusize, *coarsedelay_l2_nclust;
TH1D *coarsedelay_l3_avgclusize, *coarsedelay_l3_nclust;
TH1D *coarsedelay_dm2_avgclusize, *coarsedelay_dm2_nclust;
TH1D *coarsedelay_dm1_avgclusize, *coarsedelay_dm1_nclust;
TH1D *coarsedelay_dp1_avgclusize, *coarsedelay_dp1_nclust;
TH1D *coarsedelay_dp2_avgclusize, *coarsedelay_dp2_nclust;
TH1D *coarsedelay_bpix_avgclusize, *coarsedelay_bpix_nclust;
TH1D *coarsedelay_fpix_avgclusize, *coarsedelay_fpix_nclust;
TH1D *coarsedelay_avgclusize, *coarsedelay_nclust;

TH1D *coarsedelay_ontrack_l1_avgclusize, *coarsedelay_ontrack_l1_nclust;
TH1D *coarsedelay_ontrack_l2_avgclusize, *coarsedelay_ontrack_l2_nclust;
TH1D *coarsedelay_ontrack_l3_avgclusize, *coarsedelay_ontrack_l3_nclust;
TH1D *coarsedelay_ontrack_dm2_avgclusize, *coarsedelay_ontrack_dm2_nclust;
TH1D *coarsedelay_ontrack_dm1_avgclusize, *coarsedelay_ontrack_dm1_nclust;
TH1D *coarsedelay_ontrack_dp1_avgclusize, *coarsedelay_ontrack_dp1_nclust;
TH1D *coarsedelay_ontrack_dp2_avgclusize, *coarsedelay_ontrack_dp2_nclust;
TH1D *coarsedelay_ontrack_bpix_avgclusize, *coarsedelay_ontrack_bpix_nclust;
TH1D *coarsedelay_ontrack_fpix_avgclusize, *coarsedelay_ontrack_fpix_nclust;
TH1D *coarsedelay_ontrack_avgclusize, *coarsedelay_ontrack_nclust;

TH1D *coarsedelay_ls;

TH1D *finedelay_l1_eff, *finedelay_l1_dist;
TH1D *finedelay_l2_eff, *finedelay_l2_dist;
TH1D *finedelay_l3_eff, *finedelay_l3_dist;
TH1D *finedelay_dm2_eff, *finedelay_dm2_dist;
TH1D *finedelay_dm1_eff, *finedelay_dm1_dist;
TH1D *finedelay_dp1_eff, *finedelay_dp1_dist;
TH1D *finedelay_dp2_eff, *finedelay_dp2_dist;
TH1D *finedelay_bpix_eff, *finedelay_bpix_dist;
TH1D *finedelay_fpix_eff, *finedelay_fpix_dist;
TH1D *finedelay_eff, *finedelay_dist;

TH1D *avghitperls_l1_dist;
TH1D *avghitperls_l2_dist;
TH1D *avghitperls_l3_dist;
TH1D *avghitperls_dm2_dist;
TH1D *avghitperls_dm1_dist;
TH1D *avghitperls_dp1_dist;
TH1D *avghitperls_dp2_dist;
TH1D *avghitperls_bpix_dist;
TH1D *avghitperls_fpix_dist;
TH1D *avghitperls_dist;

TH1D *wbc_l1_eff, *wbc_l1_dist;
TH1D *wbc_l2_eff, *wbc_l2_dist;
TH1D *wbc_l3_eff, *wbc_l3_dist;
TH1D *wbc_dm2_eff, *wbc_dm2_dist;
TH1D *wbc_dm1_eff, *wbc_dm1_dist;
TH1D *wbc_dp1_eff, *wbc_dp1_dist;
TH1D *wbc_dp2_eff, *wbc_dp2_dist;
TH1D *wbc_bpix_eff, *wbc_bpix_dist;
TH1D *wbc_fpix_eff, *wbc_fpix_dist;
TH1D *wbc_eff, *wbc_dist;

TH1D *voltage_l1_eff, *voltage_l1_dist;
TH1D *voltage_l3_eff, *voltage_l3_dist;
TH1D *voltage_bpix_eff, *voltage_bpix_dist;
TH1D *voltage_fpix_eff, *voltage_fpix_dist;

TH1D *hv_bpix_eff, *hv_bpix_dist;
TH1D *hv_fpix_eff, *hv_fpix_dist;

TH1D *bx_eff, *bx_dist;
TH1D *npix_mod_eff, *npix_mod_dist;
TH1D *npix_roc_eff, *npix_roc_dist;
TH1D *npix_bpix_eff, *npix_bpix_dist;
TH1D *npix_l1_eff, *npix_l1_dist;
TH1D *npix_l2_eff, *npix_l2_dist;
TH1D *npix_l3_eff, *npix_l3_dist;

// TH1D *eff_eff, *eff_dist;
std::vector<TH1D*> eff;

TH2D *mod_l1_eff, *mod_l1_dist;
TH2D *mod_l2_eff, *mod_l2_dist;
TH2D *mod_l3_eff, *mod_l3_dist;
TH2D *mod_fpixI_eff, *mod_fpixI_dist;
TH2D *mod_fpixO_eff, *mod_fpixO_dist;

Int_t roc_binx, roc_biny;
TH2D *roc_l1_eff, *roc_l1_dist;
TH2D *roc_l2_eff, *roc_l2_dist;
TH2D *roc_l3_eff, *roc_l3_dist;
TH2D *roc_fpixI_eff, *roc_fpixI_dist;
TH2D *roc_fpixO_eff, *roc_fpixO_dist;

TH3D *rocsel_l1_eff, *rocsel_l1_dist;
TH3D *rocsel_l2_eff, *rocsel_l2_dist;
TH3D *rocsel_l3_eff, *rocsel_l3_dist;
TH3D *rocsel_fpixI_eff, *rocsel_fpixI_dist;
TH3D *rocsel_fpixO_eff, *rocsel_fpixO_dist;
TH1D *rocsel_bpix_stat;
TH1D *rocsel_fpix_p1_stat;
TH1D *rocsel_fpix_p2_stat;
TH1I *rocsel_runno;

Float_t nval = 0.0;
Float_t nvp = 0.0;
Float_t ratio = 0.0;
TH2D *roc_l1_eff2, *roc_l1_dist2, *roc_l1_ratio;

TH1D *run_l1_eff, *run_l1_dist;
TH1D *run_l2_eff, *run_l2_dist;
TH1D *run_l3_eff, *run_l3_dist;
TH1D *run_dm2_eff, *run_dm2_dist;
TH1D *run_dm1_eff, *run_dm1_dist;
TH1D *run_dp1_eff, *run_dp1_dist;
TH1D *run_dp2_eff, *run_dp2_dist;
TH1D *run_bpix_eff, *run_bpix_dist;
TH1D *run_fpix_eff, *run_fpix_dist;
TH1D *run_eff, *run_dist;

TH1D *run_ordered_l1_eff, *run_ordered_l1_dist;
TH1D *run_ordered_l2_eff, *run_ordered_l2_dist;
TH1D *run_ordered_l3_eff, *run_ordered_l3_dist;
TH1D *run_ordered_dm2_eff, *run_ordered_dm2_dist;
TH1D *run_ordered_dm1_eff, *run_ordered_dm1_dist;
TH1D *run_ordered_dp1_eff, *run_ordered_dp1_dist;
TH1D *run_ordered_dp2_eff, *run_ordered_dp2_dist;
TH1D *run_ordered_bpix_eff, *run_ordered_bpix_dist;
TH1D *run_ordered_fpix_eff, *run_ordered_fpix_dist;
TH1D *run_ordered_eff, *run_ordered_dist;

TH1D *pt_bpix_eff, *pt_bpix_dist;
TH1D *pt_bpix_cumeff, *pt_bpix_cumdist;
TH1D *pt_fpix_eff, *pt_fpix_dist;
TH1D *pt_fpix_cumeff, *pt_fpix_cumdist;

TH1D *algo4_pt_bpix_eff, *algo4_pt_bpix_dist;
TH1D *algo4_pt_bpix_cumeff, *algo4_pt_bpix_cumdist;
TH1D *algo4_pt_fpix_eff, *algo4_pt_fpix_dist;
TH1D *algo4_pt_fpix_cumeff, *algo4_pt_fpix_cumdist;
TH1D *algo5_pt_bpix_eff, *algo5_pt_bpix_dist;
TH1D *algo5_pt_bpix_cumeff, *algo5_pt_bpix_cumdist;
TH1D *algo5_pt_fpix_eff, *algo5_pt_fpix_dist;
TH1D *algo5_pt_fpix_cumeff, *algo5_pt_fpix_cumdist;
TH1D *algo6_pt_bpix_eff, *algo6_pt_bpix_dist;
TH1D *algo6_pt_bpix_cumeff, *algo6_pt_bpix_cumdist;
TH1D *algo6_pt_fpix_eff, *algo6_pt_fpix_dist;
TH1D *algo6_pt_fpix_cumeff, *algo6_pt_fpix_cumdist;
TH1D *algo7_pt_bpix_eff, *algo7_pt_bpix_dist;
TH1D *algo7_pt_bpix_cumeff, *algo7_pt_bpix_cumdist;
TH1D *algo7_pt_fpix_eff, *algo7_pt_fpix_dist;
TH1D *algo7_pt_fpix_cumeff, *algo7_pt_fpix_cumdist;
TH1D *algo8_pt_bpix_eff, *algo8_pt_bpix_dist;
TH1D *algo8_pt_bpix_cumeff, *algo8_pt_bpix_cumdist;
TH1D *algo8_pt_fpix_eff, *algo8_pt_fpix_dist;
TH1D *algo8_pt_fpix_cumeff, *algo8_pt_fpix_cumdist;
TH1D *algo9_pt_bpix_eff, *algo9_pt_bpix_dist;
TH1D *algo9_pt_bpix_cumeff, *algo9_pt_bpix_cumdist;
TH1D *algo9_pt_fpix_eff, *algo9_pt_fpix_dist;
TH1D *algo9_pt_fpix_cumeff, *algo9_pt_fpix_cumdist;
TH1D *algo45_pt_bpix_eff, *algo45_pt_bpix_dist;
TH1D *algo45_pt_bpix_cumeff, *algo45_pt_bpix_cumdist;
TH1D *algo45_pt_fpix_eff, *algo45_pt_fpix_dist;
TH1D *algo45_pt_fpix_cumeff, *algo45_pt_fpix_cumdist;

TH1D *dz_20_l1_eff, *dz_20_l1_dist;
TH1D *dz_20_l2_eff, *dz_20_l2_dist;
TH1D *dz_20_l3_eff, *dz_20_l3_dist;
TH1D *dz_20_fpix_eff, *dz_20_fpix_dist;
TH1D *dz_2_l1_eff, *dz_2_l1_dist;
TH1D *dz_2_l2_eff, *dz_2_l2_dist;
TH1D *dz_2_l3_eff, *dz_2_l3_dist;
TH1D *dz_2_fpix_eff, *dz_2_fpix_dist;
TH1D *dz_02_l1_eff, *dz_02_l1_dist;
TH1D *dz_02_l2_eff, *dz_02_l2_dist;
TH1D *dz_02_l3_eff, *dz_02_l3_dist;
TH1D *dz_02_fpix_eff, *dz_02_fpix_dist;
TH1D *dz_002_l1_eff, *dz_002_l1_dist;
TH1D *dz_002_l2_eff, *dz_002_l2_dist;
TH1D *dz_002_l3_eff, *dz_002_l3_dist;
TH1D *dz_002_fpix_eff, *dz_002_fpix_dist;

TH1D *d0_2_l1_eff, *d0_2_l1_dist;
TH1D *d0_2_l2_eff, *d0_2_l2_dist;
TH1D *d0_2_l3_eff, *d0_2_l3_dist;
TH1D *d0_2_fpix_eff, *d0_2_fpix_dist;
TH1D *d0_02_l1_eff, *d0_02_l1_dist;
TH1D *d0_02_l2_eff, *d0_02_l2_dist;
TH1D *d0_02_l3_eff, *d0_02_l3_dist;
TH1D *d0_02_fpix_eff, *d0_02_fpix_dist;
TH1D *d0_002_l1_eff, *d0_002_l1_dist;
TH1D *d0_002_l2_eff, *d0_002_l2_dist;
TH1D *d0_002_l3_eff, *d0_002_l3_dist;
TH1D *d0_002_fpix_eff, *d0_002_fpix_dist;

TH1D *dz_l1_cumeff, *dz_l1_cumdist;
TH1D *dz_l2_cumeff, *dz_l2_cumdist;
TH1D *dz_l3_cumeff, *dz_l3_cumdist;
TH1D *dz_fpix_cumeff, *dz_fpix_cumdist;
TH1D *d0_l1_cumeff, *d0_l1_cumdist;
TH1D *d0_l2_cumeff, *d0_l2_cumdist;
TH1D *d0_l3_cumeff, *d0_l3_cumdist;
TH1D *d0_fpix_cumeff, *d0_fpix_cumdist;

Int_t cut_binx, cut_biny;
TH2D *dz_d0_l1_cumeff, *dz_d0_l1_cumdist;
TH2D *dz_d0_l1_eff, *dz_d0_l1_dist;
TH2D *dz_d0_l2_cumeff, *dz_d0_l2_cumdist;
TH2D *dz_d0_l2_eff, *dz_d0_l2_dist;
TH2D *dz_d0_l3_cumeff, *dz_d0_l3_cumdist;
TH2D *dz_d0_l3_eff, *dz_d0_l3_dist;
TH2D *dz_d0_fpix_cumeff, *dz_d0_fpix_cumdist;
TH2D *dz_d0_fpix_eff, *dz_d0_fpix_dist;

TH1D *nstrip_l1_eff, *nstrip_l1_dist;
TH1D *nstrip_l1_cumeff, *nstrip_l1_cumdist;
TH1D *nstrip_l2_eff, *nstrip_l2_dist;
TH1D *nstrip_l2_cumeff, *nstrip_l2_cumdist;
TH1D *nstrip_l3_eff, *nstrip_l3_dist;
TH1D *nstrip_l3_cumeff, *nstrip_l3_cumdist;
TH1D *nstrip_fpix_eff, *nstrip_fpix_dist;
TH1D *nstrip_fpix_cumeff, *nstrip_fpix_cumdist;

TH1D *normchi2_l1_eff, *normchi2_l1_dist;
TH1D *normchi2_l2_eff, *normchi2_l2_dist;
TH1D *normchi2_l3_eff, *normchi2_l3_dist;
TH1D *normchi2_fpix_eff, *normchi2_fpix_dist;
TH1D *normchi2_l1_cumeff, *normchi2_l1_cumdist;
TH1D *normchi2_l2_cumeff, *normchi2_l2_cumdist;
TH1D *normchi2_l3_cumeff, *normchi2_l3_cumdist;
TH1D *normchi2_fpix_cumeff, *normchi2_fpix_cumdist;

TH1D *normchi2_withpt_l1_eff, *normchi2_withpt_l1_dist;
TH1D *normchi2_withpt_l2_eff, *normchi2_withpt_l2_dist;
TH1D *normchi2_withpt_l3_eff, *normchi2_withpt_l3_dist;
TH1D *normchi2_withpt_fpix_eff, *normchi2_withpt_fpix_dist;
TH1D *normchi2_withpt_l1_cumeff, *normchi2_withpt_l1_cumdist;
TH1D *normchi2_withpt_l2_cumeff, *normchi2_withpt_l2_cumdist;
TH1D *normchi2_withpt_l3_cumeff, *normchi2_withpt_l3_cumdist;
TH1D *normchi2_withpt_fpix_cumeff, *normchi2_withpt_fpix_cumdist;

Float_t drcl;
TH1D *drcl_l1_eff, *drcl_l1_dist;
TH1D *drcl_l2_eff, *drcl_l2_dist;
TH1D *drcl_l3_eff, *drcl_l3_dist;
TH1D *drcl_fpix_eff, *drcl_fpix_dist;
TH1D *drcl_l1_cumeff, *drcl_l1_cumdist;
TH1D *drcl_l2_cumeff, *drcl_l2_cumdist;
TH1D *drcl_l3_cumeff, *drcl_l3_cumdist;
TH1D *drcl_fpix_cumeff, *drcl_fpix_cumdist;

Float_t drcl12;
TH1D *drcl12_l1_eff, *drcl12_l1_dist;
TH1D *drcl12_l2_eff, *drcl12_l2_dist;
TH1D *drcl12_l3_eff, *drcl12_l3_dist;
TH1D *drcl12_fpix_eff, *drcl12_fpix_dist;
TH1D *drcl12_l1_cumeff, *drcl12_l1_cumdist;
TH1D *drcl12_l2_cumeff, *drcl12_l2_cumdist;
TH1D *drcl12_l3_cumeff, *drcl12_l3_cumdist;
TH1D *drcl12_fpix_cumeff, *drcl12_fpix_cumdist;

TH2D *drcl12_drcl_l1_eff, *drcl12_drcl_l1_dist;
TH2D *drcl12_drcl_l2_eff, *drcl12_drcl_l2_dist;
TH2D *drcl12_drcl_l3_eff, *drcl12_drcl_l3_dist;
TH2D *drcl12_drcl_fpix_eff, *drcl12_drcl_fpix_dist;
TH2D *drcl12_drcl_l1_cumeff, *drcl12_drcl_l1_cumdist;
TH2D *drcl12_drcl_l2_cumeff, *drcl12_drcl_l2_cumdist;
TH2D *drcl12_drcl_l3_cumeff, *drcl12_drcl_l3_cumdist;
TH2D *drcl12_drcl_fpix_cumeff, *drcl12_drcl_fpix_cumdist;

TH1D *neweff_l1_eff, *neweff_l1_dist;
TH1D *neweff_l2_eff, *neweff_l2_dist;
TH1D *neweff_l3_eff, *neweff_l3_dist;
TH1D *neweff_dm2_eff, *neweff_dm2_dist;
TH1D *neweff_dm1_eff, *neweff_dm1_dist;
TH1D *neweff_dp1_eff, *neweff_dp1_dist;
TH1D *neweff_dp2_eff, *neweff_dp2_dist;
TH1D *neweff_bpix_eff, *neweff_bpix_dist;
TH1D *neweff_fpix_eff, *neweff_fpix_dist;
TH1D *neweff_eff, *neweff_dist;

TH1D *neweff2_l1_eff, *neweff2_l1_dist;
TH1D *neweff2_l2_eff, *neweff2_l2_dist;
TH1D *neweff2_l3_eff, *neweff2_l3_dist;
TH1D *neweff2_dm2_eff, *neweff2_dm2_dist;
TH1D *neweff2_dm1_eff, *neweff2_dm1_dist;
TH1D *neweff2_dp1_eff, *neweff2_dp1_dist;
TH1D *neweff2_dp2_eff, *neweff2_dp2_dist;
TH1D *neweff2_bpix_eff, *neweff2_bpix_dist;
TH1D *neweff2_fpix_eff, *neweff2_fpix_dist;
TH1D *neweff2_eff, *neweff2_dist;

Float_t drclx, drcly;
TH2D *mod1_clx_cly_l1_neweff, *mod1_clx_cly_l1_newdist;
TH2D *mod4_clx_cly_l1_neweff, *mod4_clx_cly_l1_newdist;
TH1D *mod1_clx_l1_neweff, *mod1_clx_l1_newdist;
TH1D *mod4_clx_l1_neweff, *mod4_clx_l1_newdist;
TH1D *mod1_cly_l1_neweff, *mod1_cly_l1_newdist;
TH1D *mod4_cly_l1_neweff, *mod4_cly_l1_newdist;
TH2D *mod1_clx_cly_l2_neweff, *mod1_clx_cly_l2_newdist;
TH2D *mod4_clx_cly_l2_neweff, *mod4_clx_cly_l2_newdist;
TH1D *mod1_clx_l2_neweff, *mod1_clx_l2_newdist;
TH1D *mod4_clx_l2_neweff, *mod4_clx_l2_newdist;
TH1D *mod1_cly_l2_neweff, *mod1_cly_l2_newdist;
TH1D *mod4_cly_l2_neweff, *mod4_cly_l2_newdist;
TH2D *mod1_clx_cly_l3_neweff, *mod1_clx_cly_l3_newdist;
TH2D *mod4_clx_cly_l3_neweff, *mod4_clx_cly_l3_newdist;
TH1D *mod1_clx_l3_neweff, *mod1_clx_l3_newdist;
TH1D *mod4_clx_l3_neweff, *mod4_clx_l3_newdist;
TH1D *mod1_cly_l3_neweff, *mod1_cly_l3_newdist;
TH1D *mod4_cly_l3_neweff, *mod4_cly_l3_newdist;

Int_t drhit_binx;
TH2D *drhit_drcl_neweff_eff, *drhit_drcl_neweff_dist;
TH2D *drhit_drcl_neweff_bpix_eff, *drhit_drcl_neweff_bpix_dist;
TH2D *drhit_drcl_neweff_fpix_eff, *drhit_drcl_neweff_fpix_dist;

Float_t drhit;
TH1D *drhit_l1_eff, *drhit_l1_dist;
TH1D *drhit_l2_eff, *drhit_l2_dist;
TH1D *drhit_l3_eff, *drhit_l3_dist;
TH1D *drhit_fpix_eff, *drhit_fpix_dist;
TH1D *drhit_l1_cumeff, *drhit_l1_cumdist;
TH1D *drhit_l2_cumeff, *drhit_l2_cumdist;
TH1D *drhit_l3_cumeff, *drhit_l3_cumdist;
TH1D *drhit_fpix_cumeff, *drhit_fpix_cumdist;

TH1D *outerfid_l1_p_eff, *outerfid_l1_p_dist;
TH1D *outerfid_l1_m_eff, *outerfid_l1_m_dist;
TH1D *outerfid_l2_p_eff, *outerfid_l2_p_dist;
TH1D *outerfid_l2_m_eff, *outerfid_l2_m_dist;
TH1D *outerfid_l3_p_eff, *outerfid_l3_p_dist;
TH1D *outerfid_l3_m_eff, *outerfid_l3_m_dist;

// Histogram parameters for binmaker
Int_t nbinsx;
Int_t nbinsy;
Double_t xlow;
Double_t xup;
Double_t ylow;
Double_t yup;

Double_t rocsizelx, rocsizely;
Int_t nbinslx, nbinsly;
Int_t noutlx, noutly;
Int_t roclevel;

TH1D *fid_l1_full_lx_eff, *fid_l1_full_lx_dist;
TH1D *fid_l1_full_ly_eff, *fid_l1_full_ly_dist;
TH1D *fid_l1_htI_lx_eff, *fid_l1_htI_lx_dist;
TH1D *fid_l1_htI_ly_eff, *fid_l1_htI_ly_dist;
TH1D *fid_l1_htO_lx_eff, *fid_l1_htO_lx_dist;
TH1D *fid_l1_htO_ly_eff, *fid_l1_htO_ly_dist;
TH1D *fid_l1_hbI_lx_eff, *fid_l1_hbI_lx_dist;
TH1D *fid_l1_hbI_ly_eff, *fid_l1_hbI_ly_dist;
TH1D *fid_l1_hbO_lx_eff, *fid_l1_hbO_lx_dist;
TH1D *fid_l1_hbO_ly_eff, *fid_l1_hbO_ly_dist;
TH1D *fid_l2_full_lx_eff, *fid_l2_full_lx_dist;
TH1D *fid_l2_full_ly_eff, *fid_l2_full_ly_dist;
TH1D *fid_l2_htI_lx_eff, *fid_l2_htI_lx_dist;
TH1D *fid_l2_htI_ly_eff, *fid_l2_htI_ly_dist;
TH1D *fid_l2_htO_lx_eff, *fid_l2_htO_lx_dist;
TH1D *fid_l2_htO_ly_eff, *fid_l2_htO_ly_dist;
TH1D *fid_l2_hbI_lx_eff, *fid_l2_hbI_lx_dist;
TH1D *fid_l2_hbI_ly_eff, *fid_l2_hbI_ly_dist;
TH1D *fid_l2_hbO_lx_eff, *fid_l2_hbO_lx_dist;
TH1D *fid_l2_hbO_ly_eff, *fid_l2_hbO_ly_dist;
TH1D *fid_l3_full_lx_eff, *fid_l3_full_lx_dist;
TH1D *fid_l3_full_ly_eff, *fid_l3_full_ly_dist;
TH1D *fid_l3_htI_lx_eff, *fid_l3_htI_lx_dist;
TH1D *fid_l3_htI_ly_eff, *fid_l3_htI_ly_dist;
TH1D *fid_l3_htO_lx_eff, *fid_l3_htO_lx_dist;
TH1D *fid_l3_htO_ly_eff, *fid_l3_htO_ly_dist;
TH1D *fid_l3_hbI_lx_eff, *fid_l3_hbI_lx_dist;
TH1D *fid_l3_hbI_ly_eff, *fid_l3_hbI_ly_dist;
TH1D *fid_l3_hbO_lx_eff, *fid_l3_hbO_lx_dist;
TH1D *fid_l3_hbO_ly_eff, *fid_l3_hbO_ly_dist;

TH2D *fid_l1_full_eff, *fid_l1_full_dist;
TH2D *fid_l1_htI_eff, *fid_l1_htI_dist;
TH2D *fid_l1_hbI_eff, *fid_l1_hbI_dist;
TH2D *fid_l1_htO_eff, *fid_l1_htO_dist;
TH2D *fid_l1_hbO_eff, *fid_l1_hbO_dist;
TH2D *fid_l2_full_eff, *fid_l2_full_dist;
TH2D *fid_l2_htI_eff, *fid_l2_htI_dist;
TH2D *fid_l2_hbI_eff, *fid_l2_hbI_dist;
TH2D *fid_l2_htO_eff, *fid_l2_htO_dist;
TH2D *fid_l2_hbO_eff, *fid_l2_hbO_dist;
TH2D *fid_l3_full_eff, *fid_l3_full_dist;
TH2D *fid_l3_htI_eff, *fid_l3_htI_dist;
TH2D *fid_l3_hbI_eff, *fid_l3_hbI_dist;
TH2D *fid_l3_htO_eff, *fid_l3_htO_dist;
TH2D *fid_l3_hbO_eff, *fid_l3_hbO_dist;

TH1D *fid_dm2_p1_m1_lx_eff, *fid_dm2_p1_m1_lx_dist;
TH1D *fid_dm2_p1_m1_ly_eff, *fid_dm2_p1_m1_ly_dist;
TH1D *fid_dm2_p1_m2_lx_eff, *fid_dm2_p1_m2_lx_dist;
TH1D *fid_dm2_p1_m2_ly_eff, *fid_dm2_p1_m2_ly_dist;
TH1D *fid_dm2_p1_m3_lx_eff, *fid_dm2_p1_m3_lx_dist;
TH1D *fid_dm2_p1_m3_ly_eff, *fid_dm2_p1_m3_ly_dist;
TH1D *fid_dm2_p1_m4_lx_eff, *fid_dm2_p1_m4_lx_dist;
TH1D *fid_dm2_p1_m4_ly_eff, *fid_dm2_p1_m4_ly_dist;
TH1D *fid_dm2_p2_m1_lx_eff, *fid_dm2_p2_m1_lx_dist;
TH1D *fid_dm2_p2_m1_ly_eff, *fid_dm2_p2_m1_ly_dist;
TH1D *fid_dm2_p2_m2_lx_eff, *fid_dm2_p2_m2_lx_dist;
TH1D *fid_dm2_p2_m2_ly_eff, *fid_dm2_p2_m2_ly_dist;
TH1D *fid_dm2_p2_m3_lx_eff, *fid_dm2_p2_m3_lx_dist;
TH1D *fid_dm2_p2_m3_ly_eff, *fid_dm2_p2_m3_ly_dist;
TH1D *fid_dm1_p1_m1_lx_eff, *fid_dm1_p1_m1_lx_dist;
TH1D *fid_dm1_p1_m1_ly_eff, *fid_dm1_p1_m1_ly_dist;
TH1D *fid_dm1_p1_m2_lx_eff, *fid_dm1_p1_m2_lx_dist;
TH1D *fid_dm1_p1_m2_ly_eff, *fid_dm1_p1_m2_ly_dist;
TH1D *fid_dm1_p1_m3_lx_eff, *fid_dm1_p1_m3_lx_dist;
TH1D *fid_dm1_p1_m3_ly_eff, *fid_dm1_p1_m3_ly_dist;
TH1D *fid_dm1_p1_m4_lx_eff, *fid_dm1_p1_m4_lx_dist;
TH1D *fid_dm1_p1_m4_ly_eff, *fid_dm1_p1_m4_ly_dist;
TH1D *fid_dm1_p2_m1_lx_eff, *fid_dm1_p2_m1_lx_dist;
TH1D *fid_dm1_p2_m1_ly_eff, *fid_dm1_p2_m1_ly_dist;
TH1D *fid_dm1_p2_m2_lx_eff, *fid_dm1_p2_m2_lx_dist;
TH1D *fid_dm1_p2_m2_ly_eff, *fid_dm1_p2_m2_ly_dist;
TH1D *fid_dm1_p2_m3_lx_eff, *fid_dm1_p2_m3_lx_dist;
TH1D *fid_dm1_p2_m3_ly_eff, *fid_dm1_p2_m3_ly_dist;
TH1D *fid_dp1_p1_m1_lx_eff, *fid_dp1_p1_m1_lx_dist;
TH1D *fid_dp1_p1_m1_ly_eff, *fid_dp1_p1_m1_ly_dist;
TH1D *fid_dp1_p1_m2_lx_eff, *fid_dp1_p1_m2_lx_dist;
TH1D *fid_dp1_p1_m2_ly_eff, *fid_dp1_p1_m2_ly_dist;
TH1D *fid_dp1_p1_m3_lx_eff, *fid_dp1_p1_m3_lx_dist;
TH1D *fid_dp1_p1_m3_ly_eff, *fid_dp1_p1_m3_ly_dist;
TH1D *fid_dp1_p1_m4_lx_eff, *fid_dp1_p1_m4_lx_dist;
TH1D *fid_dp1_p1_m4_ly_eff, *fid_dp1_p1_m4_ly_dist;
TH1D *fid_dp1_p2_m1_lx_eff, *fid_dp1_p2_m1_lx_dist;
TH1D *fid_dp1_p2_m1_ly_eff, *fid_dp1_p2_m1_ly_dist;
TH1D *fid_dp1_p2_m2_lx_eff, *fid_dp1_p2_m2_lx_dist;
TH1D *fid_dp1_p2_m2_ly_eff, *fid_dp1_p2_m2_ly_dist;
TH1D *fid_dp1_p2_m3_lx_eff, *fid_dp1_p2_m3_lx_dist;
TH1D *fid_dp1_p2_m3_ly_eff, *fid_dp1_p2_m3_ly_dist;
TH1D *fid_dp2_p1_m1_lx_eff, *fid_dp2_p1_m1_lx_dist;
TH1D *fid_dp2_p1_m1_ly_eff, *fid_dp2_p1_m1_ly_dist;
TH1D *fid_dp2_p1_m2_lx_eff, *fid_dp2_p1_m2_lx_dist;
TH1D *fid_dp2_p1_m2_ly_eff, *fid_dp2_p1_m2_ly_dist;
TH1D *fid_dp2_p1_m3_lx_eff, *fid_dp2_p1_m3_lx_dist;
TH1D *fid_dp2_p1_m3_ly_eff, *fid_dp2_p1_m3_ly_dist;
TH1D *fid_dp2_p1_m4_lx_eff, *fid_dp2_p1_m4_lx_dist;
TH1D *fid_dp2_p1_m4_ly_eff, *fid_dp2_p1_m4_ly_dist;
TH1D *fid_dp2_p2_m1_lx_eff, *fid_dp2_p2_m1_lx_dist;
TH1D *fid_dp2_p2_m1_ly_eff, *fid_dp2_p2_m1_ly_dist;
TH1D *fid_dp2_p2_m2_lx_eff, *fid_dp2_p2_m2_lx_dist;
TH1D *fid_dp2_p2_m2_ly_eff, *fid_dp2_p2_m2_ly_dist;
TH1D *fid_dp2_p2_m3_lx_eff, *fid_dp2_p2_m3_lx_dist;
TH1D *fid_dp2_p2_m3_ly_eff, *fid_dp2_p2_m3_ly_dist;

TH2D *fid_dm2_p1_m1_eff, *fid_dm2_p1_m1_dist;
TH2D *fid_dm2_p1_m2_eff, *fid_dm2_p1_m2_dist;
TH2D *fid_dm2_p1_m3_eff, *fid_dm2_p1_m3_dist;
TH2D *fid_dm2_p1_m4_eff, *fid_dm2_p1_m4_dist;
TH2D *fid_dm2_p2_m1_eff, *fid_dm2_p2_m1_dist;
TH2D *fid_dm2_p2_m2_eff, *fid_dm2_p2_m2_dist;
TH2D *fid_dm2_p2_m3_eff, *fid_dm2_p2_m3_dist;
TH2D *fid_dm1_p1_m1_eff, *fid_dm1_p1_m1_dist;
TH2D *fid_dm1_p1_m2_eff, *fid_dm1_p1_m2_dist;
TH2D *fid_dm1_p1_m3_eff, *fid_dm1_p1_m3_dist;
TH2D *fid_dm1_p1_m4_eff, *fid_dm1_p1_m4_dist;
TH2D *fid_dm1_p2_m1_eff, *fid_dm1_p2_m1_dist;
TH2D *fid_dm1_p2_m2_eff, *fid_dm1_p2_m2_dist;
TH2D *fid_dm1_p2_m3_eff, *fid_dm1_p2_m3_dist;
TH2D *fid_dp1_p1_m1_eff, *fid_dp1_p1_m1_dist;
TH2D *fid_dp1_p1_m2_eff, *fid_dp1_p1_m2_dist;
TH2D *fid_dp1_p1_m3_eff, *fid_dp1_p1_m3_dist;
TH2D *fid_dp1_p1_m4_eff, *fid_dp1_p1_m4_dist;
TH2D *fid_dp1_p2_m1_eff, *fid_dp1_p2_m1_dist;
TH2D *fid_dp1_p2_m2_eff, *fid_dp1_p2_m2_dist;
TH2D *fid_dp1_p2_m3_eff, *fid_dp1_p2_m3_dist;
TH2D *fid_dp2_p1_m1_eff, *fid_dp2_p1_m1_dist;
TH2D *fid_dp2_p1_m2_eff, *fid_dp2_p1_m2_dist;
TH2D *fid_dp2_p1_m3_eff, *fid_dp2_p1_m3_dist;
TH2D *fid_dp2_p1_m4_eff, *fid_dp2_p1_m4_dist;
TH2D *fid_dp2_p2_m1_eff, *fid_dp2_p2_m1_dist;
TH2D *fid_dp2_p2_m2_eff, *fid_dp2_p2_m2_dist;
TH2D *fid_dp2_p2_m3_eff, *fid_dp2_p2_m3_dist;

Float_t pos, col, roc, row;
Int_t sdpx;
Float_t roceff, rocstat;
Int_t roceffsel, rocdistsel;
Int_t neweff;
// with statcut (low 10% excluded)
Float_t bpix_rocstatcut = 635;
Float_t fpix_rocstatcut = 240;
Float_t bpix_roceff_mean = 0.985274;
Float_t bpix_roceff_sigma = 0.00283231;
Float_t fpix_roceff_mean = 0.988413;
Float_t fpix_roceff_sigma = 0.00466385;
// with statcut (under 1000 excluded)
// Float_t bpix_roceff_mean = 0.98547;
// Float_t bpix_roceff_sigma = 0.00339778;
// Float_t fpix_roceff_mean = 0.98834;
// Float_t fpix_roceff_sigma = 0.00467433;
Float_t  bpix_loweff = bpix_roceff_mean - (2*bpix_roceff_sigma);
Float_t  bpix_higheff = bpix_roceff_mean + (2*bpix_roceff_sigma);
Float_t  fpix_loweff = fpix_roceff_mean - (2*fpix_roceff_sigma);
Float_t  fpix_higheff = fpix_roceff_mean + (2*fpix_roceff_sigma);

TH2D *full2010runs_roc_l1_eff;
TH2D *full2010runs_roc_l2_eff;
TH2D *full2010runs_roc_l3_eff;
TH2D *full2010runs_roc_fpixI_eff;
TH2D *full2010runs_roc_fpixO_eff;
TH2D *full2010runs_roc_l1_dist;
TH2D *full2010runs_roc_l2_dist;
TH2D *full2010runs_roc_l3_dist;
TH2D *full2010runs_roc_fpixI_dist;
TH2D *full2010runs_roc_fpixO_dist;



std::vector<std::vector<TH1D*> > coarsedelay;

void declare_TH1D_1postfix(std::vector<TH1D*> &histo, std::string histoname,
			 std::vector<std::string> &postfix1,
			 Int_t nbins_x, Double_t low_x, Double_t high_x) {
  for (size_t i=0; i<postfix1.size();i++) {
    std::string name=histoname;
    name+=postfix1[i];
    TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
    histo.push_back(h);
  }
}


void declare_TH1D_2postfix(std::vector<std::vector<TH1D*> > &histo, std::string histoname, 
			 std::vector<std::string> &postfix1, std::vector<std::string> &postfix2, 
			 Int_t nbins_x, Double_t low_x, Double_t high_x) {
  for (size_t i=0; i<postfix1.size();i++) {
    histo.push_back(std::vector<TH1D*>());
    for (size_t j=0; j<postfix2.size();j++) {
      std::string name=histoname;
      name+=postfix1[i];
      name+=postfix2[j];
      TH1D* h = new TH1D(name.c_str(), name.c_str(), nbins_x, low_x, high_x);
      histo[i].push_back(h);
    }
  }
}

// Cumulative sum (x) - for highcut
inline void xcum_sum(TH1D* h) {
  Float_t x=0.0;
  for (Int_t i=0; i<=h->GetNbinsX(); i++) {
    if (i==0&&h->GetBinContent(0)>0)
       std::cout<<"Warning! Underflow bin in histo found."<<std::endl;
    x+=h->GetBinContent(i);
    h->SetBinContent(i,x);
  }
}

// Complementary Cumulative sum (x) - for lowcut
inline void xcompcum_sum(TH1D* h) {
  Float_t x=0.0;
  for (Int_t i=h->GetNbinsX()+1; i>0; i--) {
    x+=h->GetBinContent(i);
    h->SetBinContent(i,x);
  }
}

// Cumulative sum (x,y) - for highcut
inline void xcum_ycum_sum(TH2D* h) {
  Float_t x;
  for (Int_t j=0; j<=h->GetNbinsY(); j++) {
    x=0.0;
    for (Int_t i=0; i<=h->GetNbinsX(); i++) {
      x+=h->GetBinContent(i,j);
      h->SetBinContent(i,j,x);
    }
  }
  Float_t y;
  for (Int_t i=0; i<=h->GetNbinsX(); i++) {
    y=0.0;
    for (Int_t j=0; j<=h->GetNbinsY(); j++) {
      y+=h->GetBinContent(i,j);
      h->SetBinContent(i,j,y);
    }
  }
}

// Complementary Cumulative sum (x), Cumulative sum (y) - for lowcut x, highcut y
inline void xcompcum_ycum_sum(TH2D* h) {
  Float_t x;
  for (Int_t j=0; j<=h->GetNbinsY(); j++) {
    x=0.0;
    for (Int_t i=h->GetNbinsX()+1; i>0; i--) {
      x+=h->GetBinContent(i,j);
      h->SetBinContent(i,j,x);
    }
  }
  Float_t y;
  for (Int_t i=0; i<=h->GetNbinsX(); i++) {
    y=0.0;
    for (Int_t j=0; j<=h->GetNbinsY(); j++) {
      y+=h->GetBinContent(i,j);
      h->SetBinContent(i,j,y);
    }
  }
}


inline void order_runs(TH1D* run,TH1D* run_ordered) {
  int n=0;
  for (Int_t i=1; i<=run->GetNbinsX(); i++) {
    if (run->GetBinContent(i)!=0) {
      n++;
      run_ordered->SetBinContent(n,run->GetBinContent(i));
      std::stringstream sstr;
      sstr<<i+135000;
      run_ordered->GetXaxis()->SetBinLabel(n, sstr.str().c_str());
    }
  }
}

// Calculate distribution (denumerator), efficiency and write them to file
inline void eff_dist_write(TH1* eff,TH1* dist) {
  dist->Add(eff);
  eff->Divide(dist);
  eff->Write();
  dist->Write();
}

// Calculate distribution (denumerator), efficiency
inline void eff_dist_error_write(TH1* eff,TH1* dist) {
  dist->Add(eff);
  eff->Divide(dist);
  for (Int_t i=1; i<=eff->GetNbinsX(); i++) {
    if (eff->GetBinContent(i)!=0)
      eff->SetBinError(i,sqrt(eff->GetBinContent(i)
			      *(1-eff->GetBinContent(i))
			      /dist->GetBinContent(i)));
  }
  eff->Write();
  dist->Write();
}

#ifdef COMPLETE
void effloss_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    for (Int_t i=1;i<=8;i++) {
      if (trajmeas.ly>=((4-i)*0.8) && trajmeas.ly<((5-i)*0.8))
	roc_binx = (trajmeas.mod_on.module+4)*8 + i;
    }
    if (trajmeas.mod_on.layer==1)
      roc_biny = (trajmeas.mod_on.ladder+10)*2 + 1;
    else if (trajmeas.mod_on.layer==2)
      roc_biny = (trajmeas.mod_on.ladder+16)*2 + 1;
    else if (trajmeas.mod_on.layer==3)
      roc_biny = (trajmeas.mod_on.ladder+22)*2 + 1;
    if (((trajmeas.mod_on.half==1||(trajmeas.mod_on.half==0&&trajmeas.lx>=0.0))
	 &&(trajmeas.mod_on.ladder%2==1||(trajmeas.mod_on.ladder%2==0&&trajmeas.mod_on.ladder<0)))
	||((trajmeas.mod_on.half==0&&trajmeas.lx<0.0)
	   &&(trajmeas.mod_on.ladder%2==-1||(trajmeas.mod_on.ladder%2==0&&trajmeas.mod_on.ladder>0))))
      roc_biny+=1;
    if (trajmeas.validhit==1) {
      bx_eff->Fill(evt.bx);
      npix_bpix_eff->Fill(npix_bpix_evt);
      if (trajmeas.mod_on.layer==1) {
	npix_l1_eff->Fill(npix_l1_evt);
	npix_mod_eff->Fill(npix_mod_l1_evt[trajmeas.mod_on.module+4][trajmeas.mod_on.ladder+10]);
	npix_roc_eff->Fill(npix_roc_l1_evt[roc_binx-1][roc_biny-1]);
      } else if (trajmeas.mod_on.layer==2) {
	npix_l2_eff->Fill(npix_l2_evt);
	npix_mod_eff->Fill(npix_mod_l2_evt[trajmeas.mod_on.module+4][trajmeas.mod_on.ladder+16]);
	npix_roc_eff->Fill(npix_roc_l2_evt[roc_binx-1][roc_biny-1]);
      } else if (trajmeas.mod_on.layer==3) {
	npix_l3_eff->Fill(npix_l3_evt);
	npix_mod_eff->Fill(npix_mod_l3_evt[trajmeas.mod_on.module+4][trajmeas.mod_on.ladder+22]);
	npix_roc_eff->Fill(npix_roc_l3_evt[roc_binx-1][roc_biny-1]);
      }
    } else if (trajmeas.missing==1) {
      bx_dist->Fill(evt.bx);
      npix_bpix_dist->Fill(npix_bpix_evt);
      if (trajmeas.mod_on.layer==1) {
	npix_l1_dist->Fill(npix_l1_evt);
	npix_mod_dist->Fill(npix_mod_l1_evt[trajmeas.mod_on.module+4][trajmeas.mod_on.ladder+10]);
	npix_roc_dist->Fill(npix_roc_l1_evt[roc_binx-1][roc_biny-1]);
      } else if (trajmeas.mod_on.layer==2) {
	npix_l2_dist->Fill(npix_l2_evt);
	npix_mod_dist->Fill(npix_mod_l2_evt[trajmeas.mod_on.module+4][trajmeas.mod_on.ladder+16]);
	npix_roc_dist->Fill(npix_roc_l2_evt[roc_binx-1][roc_biny-1]);
      } else if (trajmeas.mod_on.layer==3) {
	npix_l3_dist->Fill(npix_l3_evt);
	npix_mod_dist->Fill(npix_mod_l3_evt[trajmeas.mod_on.module+4][trajmeas.mod_on.ladder+22]);
	npix_roc_dist->Fill(npix_roc_l3_evt[roc_binx-1][roc_biny-1]);
      }
    }
  }
}
#endif


void fill_Int_TH1D_det_eff(std::vector<std::vector<TH1D*> > &histo, Int_t &fillwith, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1||trajmeas.missing==1) {
    histo[0][trajmeas.missing]->Fill(fillwith);
    if (trajmeas.mod_on.det==0) {
      histo[1][trajmeas.missing]->Fill(fillwith);
      if (trajmeas.mod_on.layer==1) {
	histo[2][trajmeas.missing]->Fill(fillwith);
      } else if (trajmeas.mod_on.layer==2) {
	histo[3][trajmeas.missing]->Fill(fillwith);
      } else if (trajmeas.mod_on.layer==3) {
	histo[4][trajmeas.missing]->Fill(fillwith);
      }
    } else if (trajmeas.mod_on.det==1) {
      histo[5][trajmeas.missing]->Fill(fillwith);
      if (trajmeas.mod_on.disk==-2) {
	histo[6][trajmeas.missing]->Fill(fillwith);
      } else if (trajmeas.mod_on.disk==-1) {
	histo[7][trajmeas.missing]->Fill(fillwith);
      } else if (trajmeas.mod_on.disk==1) {
	histo[8][trajmeas.missing]->Fill(fillwith);
      } else if (trajmeas.mod_on.disk==2) {
	histo[9][trajmeas.missing]->Fill(fillwith);
      }
    }
  }
}
  


void scans_fill(EventData &evt, TrajMeasurement &trajmeas) {
  // Delay scans
  if (evt.run==160413||evt.run==160497||evt.run==160577||evt.run==160578
      ||(evt.run>=160431&&evt.run<=160437)||(evt.run>=160442&&evt.run<=160463)) {
    if (evt.run==160413) {
      if (evt.orb<11531874) {
        delay_coarse = 25;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=11531874&&evt.orb<22836667) {
        delay_coarse = 0;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=22836667&&evt.orb<36525473) {
        // delay = 50, ignore
        delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=36525473&&evt.orb<47699601) {
        delay_coarse = 7;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=47699601&&evt.orb<60471838) {
        delay_coarse = 8;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=60471838&&evt.orb<71445138) {
        delay_coarse = 10;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=71445138&&evt.orb<84930036) {
        delay_coarse = -25;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=84930036&&evt.orb<100147197) {
        // delay = 12, ignore
        delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=100147197&&evt.orb<114600000) {
        delay_coarse = 14;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=114600000&&evt.orb<129039651) {
        // corrected to 1146XXXXX, plotting orb vs ls
        delay_coarse = 16;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=129039651&&evt.orb<145100000) {
        delay_coarse = -12;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=145100000&&evt.orb<160586081) {
        // corrected to 1451XXXXX, plotting orb vs ls
        delay_coarse = 10;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=160586081&&evt.orb<174462084) {
        delay_coarse = -6;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=174462084&&evt.orb<191550000) {
        // corrected to 19155XXXX, plotting orb vs ls
        delay_coarse = -8;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=191550000) {
        delay_coarse = 12;
	delay_fine = 12;
      }
    }
    if (evt.run==160497) {
      if (evt.orb<11923615) {
        delay_coarse = NOVAL_I;
      } else if (evt.orb>=11923615&&evt.orb<23009846) {
        delay_coarse = -4;
      } else if (evt.orb>=23009846&&evt.orb<34154357) {
        delay_coarse = -2;
      } else {
	delay_coarse = NOVAL_I;
      }
      delay_fine = NOVAL_I;
    }
    if (evt.run==160577) {
      if (evt.orb<10359519) {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=10359519&&evt.orb<26677701) {
	delay_coarse = 4;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=26677701&&evt.orb<40220000) {
	delay_coarse = 18;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=40220000&&evt.orb<53440000) {
	delay_coarse = 20;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=53440000&&evt.orb<66319978) {
	delay_coarse = 22;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=66319978) {
	delay_coarse = 14;
	delay_fine = 14;
      }
    }
    if (evt.run==160578) {
      if (evt.orb<1245694) {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=1245694&&evt.orb<14309529) {
	delay_coarse = 14;
	delay_fine = 14;
      } else if (evt.orb>=14309529&&evt.orb<43223561) {
	delay_coarse = 16;
	delay_fine = 16;
      } else if (evt.orb>=43223561&&evt.orb<71862094) {
	delay_coarse = 15;
	delay_fine = 15;
      } else if (evt.orb>=71862094&&evt.orb<108617422) {
	delay_coarse = 13;
	delay_fine = 13;
      } else {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      }
    }
    if ((evt.run>=160431&&evt.run<=160437)||(evt.run>=160442&&evt.run<=160463)) {
      delay_coarse = 12;
      delay_fine = 12;
    }
    if (delay_coarse!=delaycheck) {
      if (delay_coarse!=NOVAL_I)
	coarsedelay_ls->Fill(delay_coarse,lscount);
      lscount=0;
    }
    if (lscompare!=evt.ls) {
      lscount+=1;
      lscompare = evt.ls;
    }
    delaycheck = delay_coarse;
    if (delay_fine!=NOVAL_I) {
      if (trajmeas.validhit==1) {
        finedelay_eff->Fill(delay_fine);
        if (trajmeas.mod_on.det==0) {
	  finedelay_bpix_eff->Fill(delay_fine);
	  if (trajmeas.mod_on.layer==1) {
	    finedelay_l1_eff->Fill(delay_fine);
	  } else if (trajmeas.mod_on.layer==2) {
	    finedelay_l2_eff->Fill(delay_fine);
	  } else if (trajmeas.mod_on.layer==3) {
	    finedelay_l3_eff->Fill(delay_fine);
	  }
        } else if (trajmeas.mod_on.det==1) {
	  finedelay_fpix_eff->Fill(delay_fine);
	  if (trajmeas.mod_on.disk==-2) {
	    finedelay_dm2_eff->Fill(delay_fine);
	  } else if (trajmeas.mod_on.disk==-1) {
	    finedelay_dm1_eff->Fill(delay_fine);
	  } else if (trajmeas.mod_on.disk==1) {
	    finedelay_dp1_eff->Fill(delay_fine);
	  } else if (trajmeas.mod_on.disk==2) {
	    finedelay_dp2_eff->Fill(delay_fine);
	  }
        }
      } else if (trajmeas.missing==1) {
        finedelay_dist->Fill(delay_fine);
        if (trajmeas.mod_on.det==0) {
	  finedelay_bpix_dist->Fill(delay_fine);
	  if (trajmeas.mod_on.layer==1) {
	    finedelay_l1_dist->Fill(delay_fine);
	  } else if (trajmeas.mod_on.layer==2) {
	    finedelay_l2_dist->Fill(delay_fine);
	  } else if (trajmeas.mod_on.layer==3) {
	    finedelay_l3_dist->Fill(delay_fine);
	  }
        } else if (trajmeas.mod_on.det==1) {
	  finedelay_fpix_dist->Fill(delay_fine);
	  if (trajmeas.mod_on.disk==-2) {
	    finedelay_dm2_dist->Fill(delay_fine);
	  } else if (trajmeas.mod_on.disk==-1) {
	    finedelay_dm1_dist->Fill(delay_fine);
	  } else if (trajmeas.mod_on.disk==1) {
	    finedelay_dp1_dist->Fill(delay_fine);
	  } else if (trajmeas.mod_on.disk==2) {
	    finedelay_dp2_dist->Fill(delay_fine);
	  }
        }
      }
    }
    if (delay_coarse!=NOVAL_I) {
//       if (trajmeas.validhit==1) {
//         coarsedelay[0][0]->Fill(delay_coarse);
//         if (trajmeas.mod_on.det==0) {
// 	  coarsedelay[1][0]->Fill(delay_coarse);
// 	  if (trajmeas.mod_on.layer==1) {
// 	    coarsedelay[2][0]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.layer==2) {
// 	    coarsedelay[3][0]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.layer==3) {
// 	    coarsedelay[4][0]->Fill(delay_coarse);
// 	  }
//         }
//         else if (trajmeas.mod_on.det==1) {
// 	  coarsedelay[5][0]->Fill(delay_coarse);
// 	  if (trajmeas.mod_on.disk==-2) {
// 	    coarsedelay[6][0]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.disk==-1) {
// 	    coarsedelay[7][0]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.disk==1) {
// 	    coarsedelay[8][0]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.disk==2) {
// 	    coarsedelay[9][0]->Fill(delay_coarse);
// 	  }
//         }
//       } else if (trajmeas.missing==1) {
//         coarsedelay[0][1]->Fill(delay_coarse);
//         if (trajmeas.mod_on.det==0) {
// 	  coarsedelay[1][1]->Fill(delay_coarse);
// 	  if (trajmeas.mod_on.layer==1) {
// 	    coarsedelay[2][1]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.layer==2) {
// 	    coarsedelay[3][1]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.layer==3) {
// 	    coarsedelay[4][1]->Fill(delay_coarse);
// 	  }
//         } else if (trajmeas.mod_on.det==1) {
// 	  coarsedelay[5][1]->Fill(delay_coarse);
// 	  if (trajmeas.mod_on.disk==-2) {
// 	    coarsedelay[6][1]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.disk==-1) {
// 	    coarsedelay[7][1]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.disk==1) {
// 	    coarsedelay[8][1]->Fill(delay_coarse);
// 	  } else if (trajmeas.mod_on.disk==2) {
// 	    coarsedelay[9][1]->Fill(delay_coarse);
// 	  }
//         }
//       }
      fill_Int_TH1D_det_eff(coarsedelay, delay_coarse, trajmeas);
    }
  }
  // voltage scan
  if (evt.run==160578||evt.run==160579||evt.run==160819||evt.run==160827||evt.run==160835) {
    if (evt.run==160579) {
      if (evt.orb<10060929) {
	voltage_fpix = 30;
	voltage_bpix = 40;
      } else if (evt.orb>=10060929&&evt.orb<20603100) {
	voltage_fpix = 35;
	voltage_bpix = 50;
      } else if (evt.orb>=20603100&&evt.orb<30368455) {
	voltage_fpix = 40;
	voltage_bpix = 60;
      } else if (evt.orb>=30368455&&evt.orb<36726096) {
	voltage_fpix = 50;
	voltage_bpix = 70;
      } else if (evt.orb>=36726096&&evt.orb<41127055) {
	voltage_fpix = 60;
	voltage_bpix = 80;
      } else if (evt.orb>=41127055) {
	voltage_fpix = 70;
	voltage_bpix = 90;
      }
    } else if (evt.run==160578) {
      if (evt.orb>=108617422) {
	voltage_fpix = 20;
	voltage_bpix = 30;
      } else {
	voltage_fpix = NOVAL_I;
	voltage_bpix = NOVAL_I;
      }
    } else if (evt.run==160819) {
      if (evt.orb<734481) {
	voltage_fpix = NOVAL_I;
	voltage_bpix = NOVAL_I;
      } else if (evt.orb>=734481&&evt.orb<4945473) {
	voltage_fpix = 300;
	voltage_bpix = 150;
      } else if (evt.orb>=4945473&&evt.orb<9184218) {
	voltage_fpix = 50;
	voltage_bpix = 70;
      } else if (evt.orb>=9184218&&evt.orb<13531251) {
	voltage_fpix = 60;
	voltage_bpix = 80;
      } else if (evt.orb>=13531251&&evt.orb<21375662) {
	voltage_fpix = 70;
	voltage_bpix = 90;
      } else if (evt.orb>=21375662&&evt.orb<27850887) {
	voltage_fpix = 80;
	voltage_bpix = 100;
      } else {
	voltage_fpix = NOVAL_I;
	voltage_bpix = NOVAL_I;
      }
    } else if (evt.run==160827) {
      voltage_fpix = 120;
      voltage_bpix = 130;
    } else if (evt.run==160835) {
      if (evt.orb<22377196) {
	voltage_fpix = 120;
	voltage_bpix = 150;
      } else {
	voltage_fpix = 300;
	voltage_bpix = 150;
      }
    }
    if (voltage_bpix!=NOVAL_I) {
      if ( (trajmeas.mod_on.det==0
	    && ((trajmeas.mod_on.module<0 && trajmeas.mod_on.layer==1 && trajmeas.mod_on.ladder==-7)
		|| (trajmeas.mod_on.module>0 && trajmeas.mod_on.layer==3 && (trajmeas.mod_on.ladder==-4 || trajmeas.mod_on.ladder==-5))))
	   || (trajmeas.mod_on.det==1
	       && trajmeas.mod_on.disk==-1
	       && (trajmeas.mod_on.blade==1
		   || trajmeas.mod_on.blade==2
		   || trajmeas.mod_on.blade==3 )
	       && ( (trajmeas.mod_on.panel==1 && (trajmeas.mod_on.module==1||trajmeas.mod_on.module==2))
		    || (trajmeas.mod_on.panel==2 && trajmeas.mod_on.module==1) ) ) ) {
	if (trajmeas.validhit==1) {
	  if (trajmeas.mod_on.det==0) {
	    voltage_bpix_eff->Fill(voltage_bpix);
	    if (trajmeas.mod_on.layer==1)
	      voltage_l1_eff->Fill(voltage_bpix);
	    else if (trajmeas.mod_on.layer==3)
	      voltage_l3_eff->Fill(voltage_bpix);
	  }
	  else if (trajmeas.mod_on.det==1)
	    voltage_fpix_eff->Fill(voltage_fpix);
	}
	else if (trajmeas.missing==1) {
	  if (trajmeas.mod_on.det==0) {
	    voltage_bpix_dist->Fill(voltage_bpix);
	    if (trajmeas.mod_on.layer==1)
	      voltage_l1_dist->Fill(voltage_bpix);
	    else if (trajmeas.mod_on.layer==3)
	      voltage_l3_dist->Fill(voltage_bpix);
	  }
	  else if (trajmeas.mod_on.det==1)
	    voltage_fpix_dist->Fill(voltage_fpix);
	}
      }
    }
  }
  // voltage scan 2010 april
  if (evt.run==132599||evt.run==132601) {
    Int_t hv = NOVAL_I;
    if (evt.run==132599) {
      if (evt.ls>=1 && evt.ls<75) { hv= (trajmeas.mod_on.det) ? 300 : 150;
      } else if (evt.ls>=75 && evt.ls<126) { hv= (trajmeas.mod_on.det) ? 250 : 140;
      } else if (evt.ls>=126 && evt.ls<178) { hv= (trajmeas.mod_on.det) ? 200 : 130;
      } else if (evt.ls>=178 && evt.ls<233) { hv= (trajmeas.mod_on.det) ? 150 : 120;
      } else if (evt.ls>=233 && evt.ls<283) { hv= (trajmeas.mod_on.det) ? 110 : 110;
      } else if (evt.ls>=283 && evt.ls<332) { hv= (trajmeas.mod_on.det) ? 100 : 100;
      } else if (evt.ls>=332 && evt.ls<385) { hv= (trajmeas.mod_on.det) ? 90 : 90;
      } else if (evt.ls>=385 && evt.ls<999) { hv= (trajmeas.mod_on.det) ? 80 : 80;
      } else { hv=NOVAL_I; }
    } else if (evt.run==132601) {
      if (evt.ls>=1 && evt.ls<50) { hv= (trajmeas.mod_on.det) ? 70 : 70;
      } else if (evt.ls>=50 && evt.ls<105) { hv= (trajmeas.mod_on.det) ? 60 : 60;
      } else if (evt.ls>=105 && evt.ls<155) { hv= (trajmeas.mod_on.det) ? 50 : 50;
      } else if (evt.ls>=155 && evt.ls<207) { hv= (trajmeas.mod_on.det) ? 40 : 40;
      } else if (evt.ls>=207 && evt.ls<258) { hv= (trajmeas.mod_on.det) ? 30 : 30;
      } else { hv=NOVAL_I; }
    }
    if (hv!=NOVAL_I) {
      if ( (trajmeas.mod_on.det==0
	    && (trajmeas.mod_on.module>0 && trajmeas.mod_on.layer==3 && (trajmeas.mod_on.ladder==-4 || trajmeas.mod_on.ladder==-5)))
	   || (trajmeas.mod_on.det==1
	       && trajmeas.mod_on.disk==-1
	       && (trajmeas.mod_on.blade==1
		   || trajmeas.mod_on.blade==2
		   || trajmeas.mod_on.blade==3 )
	       && ( (trajmeas.mod_on.panel==1 && (trajmeas.mod_on.module==1||trajmeas.mod_on.module==2))
		    || (trajmeas.mod_on.panel==2 && trajmeas.mod_on.module==1) ) ) ) {
	if (trajmeas.validhit==1) {
	  if (trajmeas.mod_on.det==0)
	      hv_bpix_eff->Fill(hv);
	  else if (trajmeas.mod_on.det==1)
	    hv_fpix_eff->Fill(hv);
	} else if (trajmeas.missing==1) {
	  if (trajmeas.mod_on.det==0)
	    hv_bpix_dist->Fill(hv);
	  else if (trajmeas.mod_on.det==1)
	    hv_fpix_dist->Fill(hv);
	}
      }
    }
  }
}

void coarsedelay_clusize_fill(EventData &cluevt, Cluster &clust) {
  // fine delay scan
  if (cluevt.run==160413||cluevt.run==160497||cluevt.run==160577||cluevt.run==160578
      ||(cluevt.run>=160431&&cluevt.run<=160437)||(cluevt.run>=160442&&cluevt.run<=160463)) {
    if (cluevt.run==160413) {
      if (cluevt.orb<11531874) {
        delay_coarse = 25;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=11531874&&cluevt.orb<22836667) {
        delay_coarse = 0;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=22836667&&cluevt.orb<36525473) {
        // delay = 50, ignore
        delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=36525473&&cluevt.orb<47699601) {
        delay_coarse = 7;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=47699601&&cluevt.orb<60471838) {
        delay_coarse = 8;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=60471838&&cluevt.orb<71445138) {
        delay_coarse = 10;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=71445138&&cluevt.orb<84930036) {
        delay_coarse = -25;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=84930036&&cluevt.orb<100147197) {
        // delay = 12, ignore
        delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=100147197&&cluevt.orb<114600000) {
        delay_coarse = 14;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=114600000&&cluevt.orb<129039651) {
        // corrected to 1146XXXXX, plotting orb vs ls
        delay_coarse = 16;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=129039651&&cluevt.orb<145100000) {
        delay_coarse = -12;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=145100000&&cluevt.orb<160586081) {
        // corrected to 1451XXXXX, plotting orb vs ls
        delay_coarse = 10;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=160586081&&cluevt.orb<174462084) {
        delay_coarse = -6;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=174462084&&cluevt.orb<191550000) {
        // corrected to 19155XXXX, plotting orb vs ls
        delay_coarse = -8;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=191550000) {
        delay_coarse = 12;
	delay_fine = 12;
      }
    }
    if (cluevt.run==160497) {
      if (cluevt.orb<11923615) {
        delay_coarse = NOVAL_I;
      } else if (cluevt.orb>=11923615&&cluevt.orb<23009846) {
        delay_coarse = -4;
      } else if (cluevt.orb>=23009846&&cluevt.orb<34154357) {
        delay_coarse = -2;
      } else {
	delay_coarse = NOVAL_I;
      }
      delay_fine = NOVAL_I;
    }
    if (cluevt.run==160577) {
      if (cluevt.orb<10359519) {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=10359519&&cluevt.orb<26677701) {
	delay_coarse = 4;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=26677701&&cluevt.orb<40220000) {
	delay_coarse = 18;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=40220000&&cluevt.orb<53440000) {
	delay_coarse = 20;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=53440000&&cluevt.orb<66319978) {
	delay_coarse = 22;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=66319978) {
	delay_coarse = 14;
	delay_fine = 14;
      }
    }
    if (cluevt.run==160578) {
      if (cluevt.orb<1245694) {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (cluevt.orb>=1245694&&cluevt.orb<14309529) {
	delay_coarse = 14;
	delay_fine = 14;
      } else if (cluevt.orb>=14309529&&cluevt.orb<43223561) {
	delay_coarse = 16;
	delay_fine = 16;
      } else if (cluevt.orb>=43223561&&cluevt.orb<71862094) {
	delay_coarse = 15;
	delay_fine = 15;
      } else if (cluevt.orb>=71862094&&cluevt.orb<108617422) {
	delay_coarse = 13;
	delay_fine = 13;
      } else {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      }
    }
    if ((cluevt.run>=160431&&cluevt.run<=160437)||(cluevt.run>=160442&&cluevt.run<=160463)) {
      delay_coarse = 12;
      delay_fine = 12;
    }
    if (delay_coarse!=NOVAL_I) {
      if (clust.charge>12.0) {
	coarsedelay_nclust->Fill(delay_coarse);
	coarsedelay_avgclusize->Fill(delay_coarse,clust.size);
	if (clust.mod_on.det==0) {
	  coarsedelay_bpix_nclust->Fill(delay_coarse);
	  coarsedelay_bpix_avgclusize->Fill(delay_coarse,clust.size);
	  if (clust.mod_on.layer==1) {
	    coarsedelay_l1_nclust->Fill(delay_coarse);
	    coarsedelay_l1_avgclusize->Fill(delay_coarse,clust.size);
	  } else if (clust.mod_on.layer==2) {
	    coarsedelay_l2_nclust->Fill(delay_coarse);
	    coarsedelay_l2_avgclusize->Fill(delay_coarse,clust.size);
	  } else if (clust.mod_on.layer==3) {
	    coarsedelay_l3_nclust->Fill(delay_coarse);
	    coarsedelay_l3_avgclusize->Fill(delay_coarse,clust.size);
	  }
	} else if (clust.mod_on.det==1) {
	  coarsedelay_fpix_nclust->Fill(delay_coarse);
	  coarsedelay_fpix_avgclusize->Fill(delay_coarse,clust.size);
	  if (clust.mod_on.disk==-2) {
	    coarsedelay_dm2_nclust->Fill(delay_coarse);
	    coarsedelay_dm2_avgclusize->Fill(delay_coarse,clust.size);
	  } else if (clust.mod_on.disk==-1) {
	    coarsedelay_dm1_nclust->Fill(delay_coarse);
	    coarsedelay_dm1_avgclusize->Fill(delay_coarse,clust.size);
	  } else if (clust.mod_on.disk==1) {
	    coarsedelay_dp1_nclust->Fill(delay_coarse);
	    coarsedelay_dp1_avgclusize->Fill(delay_coarse,clust.size);
	  } else if (clust.mod_on.disk==2) {
	    coarsedelay_dp2_nclust->Fill(delay_coarse);
	    coarsedelay_dp2_avgclusize->Fill(delay_coarse,clust.size);
	  }
	}
      }
    }
  }
}

void coarsedelay_ontrack_clusize_fill(EventData &evt, TrajMeasurement &trajmeas) {
  // fine delay scan
  if (evt.run==160413||evt.run==160497||evt.run==160577||evt.run==160578
      ||(evt.run>=160431&&evt.run<=160437)||(evt.run>=160442&&evt.run<=160463)) {
    if (evt.run==160413) {
      if (evt.orb<11531874) {
        delay_coarse = 25;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=11531874&&evt.orb<22836667) {
        delay_coarse = 0;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=22836667&&evt.orb<36525473) {
        // delay = 50, ignore
        delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=36525473&&evt.orb<47699601) {
        delay_coarse = 7;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=47699601&&evt.orb<60471838) {
        delay_coarse = 8;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=60471838&&evt.orb<71445138) {
        delay_coarse = 10;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=71445138&&evt.orb<84930036) {
        delay_coarse = -25;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=84930036&&evt.orb<100147197) {
        // delay = 12, ignore
        delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=100147197&&evt.orb<114600000) {
        delay_coarse = 14;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=114600000&&evt.orb<129039651) {
        // corrected to 1146XXXXX, plotting orb vs ls
        delay_coarse = 16;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=129039651&&evt.orb<145100000) {
        delay_coarse = -12;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=145100000&&evt.orb<160586081) {
        // corrected to 1451XXXXX, plotting orb vs ls
        delay_coarse = 10;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=160586081&&evt.orb<174462084) {
        delay_coarse = -6;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=174462084&&evt.orb<191550000) {
        // corrected to 19155XXXX, plotting orb vs ls
        delay_coarse = -8;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=191550000) {
        delay_coarse = 12;
	delay_fine = 12;
      }
    }
    if (evt.run==160497) {
      if (evt.orb<11923615) {
        delay_coarse = NOVAL_I;
      } else if (evt.orb>=11923615&&evt.orb<23009846) {
        delay_coarse = -4;
      } else if (evt.orb>=23009846&&evt.orb<34154357) {
        delay_coarse = -2;
      } else {
	delay_coarse = NOVAL_I;
      }
      delay_fine = NOVAL_I;
    }
    if (evt.run==160577) {
      if (evt.orb<10359519) {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=10359519&&evt.orb<26677701) {
	delay_coarse = 4;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=26677701&&evt.orb<40220000) {
	delay_coarse = 18;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=40220000&&evt.orb<53440000) {
	delay_coarse = 20;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=53440000&&evt.orb<66319978) {
	delay_coarse = 22;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=66319978) {
	delay_coarse = 14;
	delay_fine = 14;
      }
    }
    if (evt.run==160578) {
      if (evt.orb<1245694) {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      } else if (evt.orb>=1245694&&evt.orb<14309529) {
	delay_coarse = 14;
	delay_fine = 14;
      } else if (evt.orb>=14309529&&evt.orb<43223561) {
	delay_coarse = 16;
	delay_fine = 16;
      } else if (evt.orb>=43223561&&evt.orb<71862094) {
	delay_coarse = 15;
	delay_fine = 15;
      } else if (evt.orb>=71862094&&evt.orb<108617422) {
	delay_coarse = 13;
	delay_fine = 13;
      } else {
	delay_coarse = NOVAL_I;
	delay_fine = NOVAL_I;
      }
    }
    if ((evt.run>=160431&&evt.run<=160437)||(evt.run>=160442&&evt.run<=160463)) {
      delay_coarse = 12;
      delay_fine = 12;
    }
    if (delay_coarse!=NOVAL_I) {
      if (trajmeas.clu.charge>12.0) {
	coarsedelay_ontrack_nclust->Fill(delay_coarse);
	coarsedelay_ontrack_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	if (trajmeas.mod_on.det==0) {
	  coarsedelay_ontrack_bpix_nclust->Fill(delay_coarse);
	  coarsedelay_ontrack_bpix_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  if (trajmeas.mod_on.layer==1) {
	    coarsedelay_ontrack_l1_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_l1_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  } else if (trajmeas.mod_on.layer==2) {
	    coarsedelay_ontrack_l2_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_l2_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  } else if (trajmeas.mod_on.layer==3) {
	    coarsedelay_ontrack_l3_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_l3_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  }
	} else if (trajmeas.mod_on.det==1) {
	  coarsedelay_ontrack_fpix_nclust->Fill(delay_coarse);
	  coarsedelay_ontrack_fpix_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  if (trajmeas.mod_on.disk==-2) {
	    coarsedelay_ontrack_dm2_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_dm2_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  } else if (trajmeas.mod_on.disk==-1) {
	    coarsedelay_ontrack_dm1_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_dm1_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  } else if (trajmeas.mod_on.disk==1) {
	    coarsedelay_ontrack_dp1_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_dp1_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  } else if (trajmeas.mod_on.disk==2) {
	    coarsedelay_ontrack_dp2_nclust->Fill(delay_coarse);
	    coarsedelay_ontrack_dp2_avgclusize->Fill(delay_coarse,trajmeas.clu.size);
	  }
	}
      }
    }
  }
}



void eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0)
      //      eff_eff->Fill(trajmeas.mod_on.layer);
      eff[0]->Fill(trajmeas.mod_on.layer);
    else if (trajmeas.mod_on.disk<0)
      //      eff_eff->Fill(trajmeas.mod_on.disk+6);
      eff[0]->Fill(trajmeas.mod_on.disk+6);
    else if (trajmeas.mod_on.disk>0)
      //      eff_eff->Fill(trajmeas.mod_on.disk+5);
      eff[0]->Fill(trajmeas.mod_on.disk+5);
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0)
      //      eff_dist->Fill(trajmeas.mod_on.layer);
      eff[1]->Fill(trajmeas.mod_on.layer);
    else if (trajmeas.mod_on.disk<0)
      //      eff_dist->Fill(trajmeas.mod_on.disk+6);
      eff[1]->Fill(trajmeas.mod_on.disk+6);
    else if (trajmeas.mod_on.disk>0)
      //      eff_dist->Fill(trajmeas.mod_on.disk+5);
      eff[1]->Fill(trajmeas.mod_on.disk+5);
  }
}

void mod_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1)
        mod_l1_eff->Fill(trajmeas.mod_on.module, trajmeas.mod_on.ladder);
      else if (trajmeas.mod_on.layer==2)
        mod_l2_eff->Fill(trajmeas.mod_on.module, trajmeas.mod_on.ladder);
      else if (trajmeas.mod_on.layer==3)
        mod_l3_eff->Fill(trajmeas.mod_on.module, trajmeas.mod_on.ladder);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0)
        mod_fpixI_eff->Fill(sdpx, trajmeas.mod_on.blade); 
      else if (trajmeas.mod_on.blade<0)
        mod_fpixO_eff->Fill(sdpx, trajmeas.mod_on.blade); 
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1)
        mod_l1_dist->Fill(trajmeas.mod_on.module, trajmeas.mod_on.ladder);
      else if (trajmeas.mod_on.layer==2)
        mod_l2_dist->Fill(trajmeas.mod_on.module, trajmeas.mod_on.ladder);
      else if (trajmeas.mod_on.layer==3)
        mod_l3_dist->Fill(trajmeas.mod_on.module, trajmeas.mod_on.ladder);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0)
        mod_fpixI_dist->Fill(sdpx, trajmeas.mod_on.blade); 
      else if (trajmeas.mod_on.blade<0)
        mod_fpixO_dist->Fill(sdpx, trajmeas.mod_on.blade); 
    }
  }
}

void roc_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1)
	roc_l1_eff->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
      else if (trajmeas.mod_on.layer==2)
	roc_l2_eff->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
      else if (trajmeas.mod_on.layer==3)
	roc_l3_eff->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	roc_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      } else if (trajmeas.mod_on.blade<0) {
	roc_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      }
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1)
	roc_l1_dist->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
      else if (trajmeas.mod_on.layer==2)
	roc_l2_dist->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
      else if (trajmeas.mod_on.layer==3)
	roc_l3_dist->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	roc_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      } else if (trajmeas.mod_on.blade<0) {
	roc_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      }
    }
  }
}

void roc2_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    for (Int_t i=1;i<=8;i++) {
      if (trajmeas.ly>=((4-i)*0.8) && trajmeas.ly<((5-i)*0.8))
	roc_binx = (trajmeas.mod_on.module+4)*8 + i;
    }
    if (trajmeas.mod_on.layer==1)
      roc_biny = (trajmeas.mod_on.ladder+10)*2 + 1;
    else if (trajmeas.mod_on.layer==2)
      roc_biny = (trajmeas.mod_on.ladder+16)*2 + 1;
    else if (trajmeas.mod_on.layer==3)
      roc_biny = (trajmeas.mod_on.ladder+22)*2 + 1;
    if (((trajmeas.mod_on.half==1||(trajmeas.mod_on.half==0&&trajmeas.lx>=0.0))
	 &&(trajmeas.mod_on.ladder%2==1||(trajmeas.mod_on.ladder%2==0&&trajmeas.mod_on.ladder<0)))
	||((trajmeas.mod_on.half==0&&trajmeas.lx<0.0)
	   &&(trajmeas.mod_on.ladder%2==-1||(trajmeas.mod_on.ladder%2==0&&trajmeas.mod_on.ladder>0))))
      roc_biny+=1;
  }
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1)
	roc_l1_eff->SetBinContent(roc_binx,roc_biny,roc_l1_eff->GetBinContent(roc_binx,roc_biny)+1);
      else if (trajmeas.mod_on.layer==2)
	roc_l2_eff->SetBinContent(roc_binx,roc_biny,roc_l2_eff->GetBinContent(roc_binx,roc_biny)+1);
      else if (trajmeas.mod_on.layer==3)
	roc_l3_eff->SetBinContent(roc_binx,roc_biny,roc_l3_eff->GetBinContent(roc_binx,roc_biny)+1);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	roc_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      } else if (trajmeas.mod_on.blade<0) {
	roc_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      }
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1)
	roc_l1_dist->SetBinContent(roc_binx,roc_biny,roc_l1_dist->GetBinContent(roc_binx,roc_biny)+1);
      else if (trajmeas.mod_on.layer==2)
	roc_l2_dist->SetBinContent(roc_binx,roc_biny,roc_l2_dist->GetBinContent(roc_binx,roc_biny)+1);
      else if (trajmeas.mod_on.layer==3)
	roc_l3_dist->SetBinContent(roc_binx,roc_biny,roc_l3_dist->GetBinContent(roc_binx,roc_biny)+1);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	roc_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      } else if (trajmeas.mod_on.blade<0) {
	roc_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05);
	roc_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05);
      }
    }
  }
}

void rocsel2_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    for (Int_t i=1;i<=8;i++) {
      if (trajmeas.ly>=((4-i)*0.8) && trajmeas.ly<((5-i)*0.8))
	roc_binx = (trajmeas.mod_on.module+4)*8 + i;
    }
    if (trajmeas.mod_on.layer==1)
      roc_biny = (trajmeas.mod_on.ladder+10)*2 + 1;
    else if (trajmeas.mod_on.layer==2)
      roc_biny = (trajmeas.mod_on.ladder+16)*2 + 1;
    else if (trajmeas.mod_on.layer==3)
      roc_biny = (trajmeas.mod_on.ladder+22)*2 + 1;
    if (((trajmeas.mod_on.half==1||(trajmeas.mod_on.half==0&&trajmeas.lx>=0.0))
	 &&(trajmeas.mod_on.ladder%2==1||(trajmeas.mod_on.ladder%2==0&&trajmeas.mod_on.ladder<0)))
	||((trajmeas.mod_on.half==0&&trajmeas.lx<0.0)
	   &&(trajmeas.mod_on.ladder%2==-1||(trajmeas.mod_on.ladder%2==0&&trajmeas.mod_on.ladder>0))))
      roc_biny+=1;
  }
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      rocsel_bpix_stat->Fill(nrun);
      if (trajmeas.mod_on.layer==1)
	rocsel_l1_eff->SetBinContent(roc_binx,roc_biny,nrun,rocsel_l1_eff->GetBinContent(roc_binx,roc_biny,nrun)+1);
      else if (trajmeas.mod_on.layer==2)
	rocsel_l2_eff->SetBinContent(roc_binx,roc_biny,nrun,rocsel_l2_eff->GetBinContent(roc_binx,roc_biny,nrun)+1);
      else if (trajmeas.mod_on.layer==3)
	rocsel_l3_eff->SetBinContent(roc_binx,roc_biny,nrun,rocsel_l3_eff->GetBinContent(roc_binx,roc_biny,nrun)+1);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	rocsel_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      } else if (trajmeas.mod_on.blade<0) {
	rocsel_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      }
      if (trajmeas.mod_on.panel==1)
	rocsel_fpix_p1_stat->Fill(nrun);
      else if (trajmeas.mod_on.panel==2)
	rocsel_fpix_p2_stat->Fill(nrun);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      rocsel_bpix_stat->Fill(nrun);
      if (trajmeas.mod_on.layer==1)
	rocsel_l1_dist->SetBinContent(roc_binx,roc_biny,nrun,rocsel_l1_dist->GetBinContent(roc_binx,roc_biny,nrun)+1);
      else if (trajmeas.mod_on.layer==2)
	rocsel_l2_dist->SetBinContent(roc_binx,roc_biny,nrun,rocsel_l2_dist->GetBinContent(roc_binx,roc_biny,nrun)+1);
      else if (trajmeas.mod_on.layer==3)
	rocsel_l3_dist->SetBinContent(roc_binx,roc_biny,nrun,rocsel_l3_dist->GetBinContent(roc_binx,roc_biny,nrun)+1);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	rocsel_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      } else if (trajmeas.mod_on.blade<0) {
	rocsel_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      }
      if (trajmeas.mod_on.panel==1)
	rocsel_fpix_p1_stat->Fill(nrun);
      else if (trajmeas.mod_on.panel==2)
	rocsel_fpix_p2_stat->Fill(nrun);
    }
  }
}

void rocsel_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      rocsel_bpix_stat->Fill(nrun);
      if (trajmeas.mod_on.layer==1)
	rocsel_l1_eff->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos,nrun);
      else if (trajmeas.mod_on.layer==2)
	rocsel_l2_eff->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos,nrun);
      else if (trajmeas.mod_on.layer==3)
	rocsel_l3_eff->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos,nrun);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	rocsel_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixI_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      } else if (trajmeas.mod_on.blade<0) {
	rocsel_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixO_eff->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      }
      if (trajmeas.mod_on.panel==1)
	rocsel_fpix_p1_stat->Fill(nrun);
      else if (trajmeas.mod_on.panel==2)
	rocsel_fpix_p2_stat->Fill(nrun);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      rocsel_bpix_stat->Fill(nrun);
      if (trajmeas.mod_on.layer==1)
	rocsel_l1_dist->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos,nrun);
      else if (trajmeas.mod_on.layer==2)
	rocsel_l2_dist->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos,nrun);
      else if (trajmeas.mod_on.layer==3)
	rocsel_l3_dist->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos,nrun);
    } else if (trajmeas.mod_on.det==1) {
      if (trajmeas.mod_on.blade>0) {
	rocsel_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixI_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      } else if (trajmeas.mod_on.blade<0) {
	rocsel_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos-0.05,nrun);
	rocsel_fpixO_dist->Fill(sdpx+col,trajmeas.mod_on.blade+pos+0.05,nrun);
      }
      if (trajmeas.mod_on.panel==1)
	rocsel_fpix_p1_stat->Fill(nrun);
      else if (trajmeas.mod_on.panel==2)
	rocsel_fpix_p2_stat->Fill(nrun);
    }
  }
}

void roc_l1_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    for (Int_t i=-4;i<4;i++) {
      if (trajmeas.ly>=(i*0.8) && trajmeas.ly<((i+1)*0.8))
	roc = i+0.0;
    }
    col = (roc+0.5)/8;
    if (trajmeas.mod_on.half==1)
      pos = 0.25;
    else if (trajmeas.mod_on.half==0) {
      if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	pos = -0.25;
      else if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	pos = 0.25;
    }
    if (trajmeas.mod_on.ladder==-1 || trajmeas.mod_on.ladder==-3 
	|| trajmeas.mod_on.ladder==-5 || trajmeas.mod_on.ladder==-7 
	|| trajmeas.mod_on.ladder==-9 || trajmeas.mod_on.ladder==-11
	|| trajmeas.mod_on.ladder==-13 || trajmeas.mod_on.ladder==-15
	|| trajmeas.mod_on.ladder==-17 || trajmeas.mod_on.ladder==-19
	|| trajmeas.mod_on.ladder==-21 || trajmeas.mod_on.ladder==2 
	|| trajmeas.mod_on.ladder==4 || trajmeas.mod_on.ladder==6 
	|| trajmeas.mod_on.ladder==8 || trajmeas.mod_on.ladder==10
	|| trajmeas.mod_on.ladder==12 || trajmeas.mod_on.ladder==14
	|| trajmeas.mod_on.ladder==16 || trajmeas.mod_on.ladder==18
	|| trajmeas.mod_on.ladder==20 || trajmeas.mod_on.ladder==22)
      pos = -pos;
    if (trajmeas.mod_on.layer==1) {
      if (trajmeas.validhit==1) {
	nval = nval + 1.0;
	roc_l1_eff2->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
	if (trajmeas.lxmatch!=NOVAL_F)
	  nvp = nvp + 1.0;
      }
      else if (trajmeas.missing==1)
	roc_l1_dist2->Fill(trajmeas.mod_on.module-col,trajmeas.mod_on.ladder+pos);
    }
  }
}

void run_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    run_eff->Fill(evt.run);
    if (trajmeas.mod_on.det==0) {
      run_bpix_eff->Fill(evt.run);
      if (trajmeas.mod_on.layer==1)
	run_l1_eff->Fill(evt.run);
      else if (trajmeas.mod_on.layer==2)
	run_l2_eff->Fill(evt.run);
      else if (trajmeas.mod_on.layer==3)
	run_l3_eff->Fill(evt.run);
    } else if (trajmeas.mod_on.det==1) {
      run_fpix_eff->Fill(evt.run);
      if (trajmeas.mod_on.disk==-2)
	run_dm2_eff->Fill(evt.run);
      else if (trajmeas.mod_on.disk==-1)
	run_dm1_eff->Fill(evt.run);
      else if (trajmeas.mod_on.disk==1)
	run_dp1_eff->Fill(evt.run);
      else if (trajmeas.mod_on.disk==2)
	run_dp2_eff->Fill(evt.run);
    }
  } else if (trajmeas.missing==1) {
    run_dist->Fill(evt.run);
    if (trajmeas.mod_on.det==0) {
      run_bpix_dist->Fill(evt.run);
      if (trajmeas.mod_on.layer==1)
	run_l1_dist->Fill(evt.run);
      else if (trajmeas.mod_on.layer==2)
	run_l2_dist->Fill(evt.run);
      else if (trajmeas.mod_on.layer==3)
	run_l3_dist->Fill(evt.run);
    } else if (trajmeas.mod_on.det==1) {
      run_fpix_dist->Fill(evt.run);
      if (trajmeas.mod_on.disk==-2)
	run_dm2_dist->Fill(evt.run);
      else if (trajmeas.mod_on.disk==-1)
	run_dm1_dist->Fill(evt.run);
      else if (trajmeas.mod_on.disk==1)
	run_dp1_dist->Fill(evt.run);
      else if (trajmeas.mod_on.disk==2)
	run_dp2_dist->Fill(evt.run);
    }
  }
}

// Efficiency vs pt graphs (splitted)
void pt_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      pt_bpix_eff->Fill(trajmeas.trk.pt);
      pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      if (trajmeas.trk.algo==4) {
        algo4_pt_bpix_eff->Fill(trajmeas.trk.pt);
	algo4_pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==5) {
        algo5_pt_bpix_eff->Fill(trajmeas.trk.pt);
	algo5_pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==6) {
        algo6_pt_bpix_eff->Fill(trajmeas.trk.pt);
	algo6_pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo7_pt_bpix_eff->Fill(trajmeas.trk.pt);
	algo7_pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo8_pt_bpix_eff->Fill(trajmeas.trk.pt);
	algo8_pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==8) {
        algo9_pt_bpix_eff->Fill(trajmeas.trk.pt);
	algo9_pt_bpix_cumeff->Fill(trajmeas.trk.pt);
      }
    } else if (trajmeas.mod_on.det==1) {
      pt_fpix_eff->Fill(trajmeas.trk.pt);
      pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      if (trajmeas.trk.algo==4) {
        algo4_pt_fpix_eff->Fill(trajmeas.trk.pt);
	algo4_pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==5) {
        algo5_pt_fpix_eff->Fill(trajmeas.trk.pt);
	algo5_pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==6) {
        algo6_pt_fpix_eff->Fill(trajmeas.trk.pt);
	algo6_pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo7_pt_fpix_eff->Fill(trajmeas.trk.pt);
	algo7_pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo8_pt_fpix_eff->Fill(trajmeas.trk.pt);
	algo8_pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==8) {
        algo9_pt_fpix_eff->Fill(trajmeas.trk.pt);
	algo9_pt_fpix_cumeff->Fill(trajmeas.trk.pt);
      }
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      pt_bpix_dist->Fill(trajmeas.trk.pt);
      pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      if (trajmeas.trk.algo==4) {
        algo4_pt_bpix_dist->Fill(trajmeas.trk.pt);
	algo4_pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==5) {
        algo5_pt_bpix_dist->Fill(trajmeas.trk.pt);
	algo5_pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==6) {
        algo6_pt_bpix_dist->Fill(trajmeas.trk.pt);
	algo6_pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo7_pt_bpix_dist->Fill(trajmeas.trk.pt);
	algo7_pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo8_pt_bpix_dist->Fill(trajmeas.trk.pt);
	algo8_pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==8) {
        algo9_pt_bpix_dist->Fill(trajmeas.trk.pt);
	algo9_pt_bpix_cumdist->Fill(trajmeas.trk.pt);
      }
    } else if (trajmeas.mod_on.det==1) {
      pt_fpix_dist->Fill(trajmeas.trk.pt);
      pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      if (trajmeas.trk.algo==4) {
        algo4_pt_fpix_dist->Fill(trajmeas.trk.pt);
	algo4_pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==5) {
        algo5_pt_fpix_dist->Fill(trajmeas.trk.pt);
	algo5_pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==6) {
        algo6_pt_fpix_dist->Fill(trajmeas.trk.pt);
	algo6_pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo7_pt_fpix_dist->Fill(trajmeas.trk.pt);
	algo7_pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==7) {
        algo8_pt_fpix_dist->Fill(trajmeas.trk.pt);
	algo8_pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      } else if (trajmeas.trk.algo==8) {
        algo9_pt_fpix_dist->Fill(trajmeas.trk.pt);
	algo9_pt_fpix_cumdist->Fill(trajmeas.trk.pt);
      }
    }
  }
}

void dz_d0_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	if (fabs(trajmeas.trk.d0)<0.01) {
	  dz_20_l1_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_2_l1_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_02_l1_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_002_l1_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_l1_cumeff->Fill(fabs(trajmeas.trk.dz));
	}
	if (fabs(trajmeas.trk.dz)<0.1) {
	  d0_2_l1_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_02_l1_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_002_l1_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_l1_cumeff->Fill(fabs(trajmeas.trk.d0));
	}
	dz_d0_l1_eff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
	dz_d0_l1_cumeff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      } else if (trajmeas.mod_on.layer==2) {
	if (fabs(trajmeas.trk.d0)<0.02) {
	  dz_20_l2_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_2_l2_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_02_l2_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_002_l2_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_l2_cumeff->Fill(fabs(trajmeas.trk.dz));
	}
	if (fabs(trajmeas.trk.dz)<0.1) {
	  d0_2_l2_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_02_l2_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_002_l2_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_l2_cumeff->Fill(fabs(trajmeas.trk.d0));
	}
	dz_d0_l2_eff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
	dz_d0_l2_cumeff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      } else if (trajmeas.mod_on.layer==3) {
	if (fabs(trajmeas.trk.d0)<0.02) {
	  dz_20_l3_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_2_l3_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_02_l3_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_002_l3_eff->Fill(fabs(trajmeas.trk.dz));
	  dz_l3_cumeff->Fill(fabs(trajmeas.trk.dz));
	}
	if (fabs(trajmeas.trk.dz)<0.1) {
	  d0_2_l3_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_02_l3_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_002_l3_eff->Fill(fabs(trajmeas.trk.d0));
	  d0_l3_cumeff->Fill(fabs(trajmeas.trk.d0));
	}
	dz_d0_l3_eff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
	dz_d0_l3_cumeff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      }
    } else if (trajmeas.mod_on.det==1) {
      if (fabs(trajmeas.trk.d0)<0.05) {
	dz_20_fpix_eff->Fill(fabs(trajmeas.trk.dz));
	dz_2_fpix_eff->Fill(fabs(trajmeas.trk.dz));
	dz_02_fpix_eff->Fill(fabs(trajmeas.trk.dz));
	dz_002_fpix_eff->Fill(fabs(trajmeas.trk.dz));
	dz_fpix_cumeff->Fill(fabs(trajmeas.trk.dz));
      }
      if (fabs(trajmeas.trk.dz)<0.5) {
	d0_2_fpix_eff->Fill(fabs(trajmeas.trk.d0));
	d0_02_fpix_eff->Fill(fabs(trajmeas.trk.d0));
	d0_002_fpix_eff->Fill(fabs(trajmeas.trk.d0));
	d0_fpix_cumeff->Fill(fabs(trajmeas.trk.d0));
      }
      dz_d0_fpix_eff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      dz_d0_fpix_cumeff->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	if (fabs(trajmeas.trk.d0)<0.01) {
	  dz_20_l1_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_2_l1_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_02_l1_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_002_l1_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_l1_cumdist->Fill(fabs(trajmeas.trk.dz));
	}
	if (fabs(trajmeas.trk.dz)<0.1) {
	  d0_2_l1_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_02_l1_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_002_l1_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_l1_cumdist->Fill(fabs(trajmeas.trk.d0));
	}
	dz_d0_l1_dist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
	dz_d0_l1_cumdist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      } else if (trajmeas.mod_on.layer==2) {
	if (fabs(trajmeas.trk.d0)<0.02) {
	  dz_20_l2_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_2_l2_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_02_l2_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_002_l2_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_l2_cumdist->Fill(fabs(trajmeas.trk.dz));
	}
	if (fabs(trajmeas.trk.dz)<0.1) {
	  d0_2_l2_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_02_l2_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_002_l2_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_l2_cumdist->Fill(fabs(trajmeas.trk.d0));
	}
	dz_d0_l2_dist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
	dz_d0_l2_cumdist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      } else if (trajmeas.mod_on.layer==3) {
	if (fabs(trajmeas.trk.d0)<0.02) {
	  dz_20_l3_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_2_l3_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_02_l3_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_002_l3_dist->Fill(fabs(trajmeas.trk.dz));
	  dz_l3_cumdist->Fill(fabs(trajmeas.trk.dz));
	}
	if (fabs(trajmeas.trk.dz)<0.1) {
	  d0_2_l3_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_02_l3_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_002_l3_dist->Fill(fabs(trajmeas.trk.d0));
	  d0_l3_cumdist->Fill(fabs(trajmeas.trk.d0));
	}
	dz_d0_l3_dist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
	dz_d0_l3_cumdist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      }
    } else if (trajmeas.mod_on.det==1) {
      if (fabs(trajmeas.trk.d0)<0.05) {
	dz_20_fpix_dist->Fill(fabs(trajmeas.trk.dz));
	dz_2_fpix_dist->Fill(fabs(trajmeas.trk.dz));
	dz_02_fpix_dist->Fill(fabs(trajmeas.trk.dz));
	dz_002_fpix_dist->Fill(fabs(trajmeas.trk.dz));
	dz_fpix_cumdist->Fill(fabs(trajmeas.trk.dz));
      }
      if (fabs(trajmeas.trk.dz)<0.5) {
	d0_2_fpix_dist->Fill(fabs(trajmeas.trk.d0));
	d0_02_fpix_dist->Fill(fabs(trajmeas.trk.d0));
	d0_002_fpix_dist->Fill(fabs(trajmeas.trk.d0));
	d0_fpix_cumdist->Fill(fabs(trajmeas.trk.d0));
      }
      dz_d0_fpix_dist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
      dz_d0_fpix_cumdist->Fill(fabs(trajmeas.trk.dz),fabs(trajmeas.trk.d0));
    }
  }
}

void nstrip_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	nstrip_l1_eff->Fill(trajmeas.trk.strip);
	nstrip_l1_cumeff->Fill(trajmeas.trk.strip);
      } else if (trajmeas.mod_on.layer==2) {
	nstrip_l2_eff->Fill(trajmeas.trk.strip);
	nstrip_l2_cumeff->Fill(trajmeas.trk.strip);
      } else if (trajmeas.mod_on.layer==3) {
	nstrip_l3_eff->Fill(trajmeas.trk.strip);
	nstrip_l3_cumeff->Fill(trajmeas.trk.strip);
      }
    } else if (trajmeas.mod_on.det==1) {
      nstrip_fpix_eff->Fill(trajmeas.trk.strip);
      nstrip_fpix_cumeff->Fill(trajmeas.trk.strip);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	nstrip_l1_dist->Fill(trajmeas.trk.strip);
	nstrip_l1_cumdist->Fill(trajmeas.trk.strip);
      } else if (trajmeas.mod_on.layer==2) {
	nstrip_l2_dist->Fill(trajmeas.trk.strip);
	nstrip_l2_cumdist->Fill(trajmeas.trk.strip);
      } else if (trajmeas.mod_on.layer==3) {
	nstrip_l3_dist->Fill(trajmeas.trk.strip);
	nstrip_l3_cumdist->Fill(trajmeas.trk.strip);
      }
    } else if (trajmeas.mod_on.det==1) {
      nstrip_fpix_dist->Fill(trajmeas.trk.strip);
      nstrip_fpix_cumdist->Fill(trajmeas.trk.strip);
    }
  }
}

void normchi2_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	normchi2_l1_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_l1_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==2) {
	normchi2_l2_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_l2_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==3) {
	normchi2_l3_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_l3_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      }
    } else if (trajmeas.mod_on.det==1) {
      normchi2_fpix_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      normchi2_fpix_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	normchi2_l1_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_l1_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==2) {
	normchi2_l2_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_l2_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==3) {
	normchi2_l3_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_l3_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      }
    } else if (trajmeas.mod_on.det==1) {
      normchi2_fpix_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      normchi2_fpix_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
    }
  }
}

void normchi2_withpt_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	normchi2_withpt_l1_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_withpt_l1_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==2) {
	normchi2_withpt_l2_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_withpt_l2_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==3) {
	normchi2_withpt_l3_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_withpt_l3_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      }
    } else if (trajmeas.mod_on.det==1) {
      normchi2_withpt_fpix_eff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      normchi2_withpt_fpix_cumeff->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	normchi2_withpt_l1_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_withpt_l1_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==2) {
	normchi2_withpt_l2_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_withpt_l2_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      } else if (trajmeas.mod_on.layer==3) {
	normchi2_withpt_l3_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
	normchi2_withpt_l3_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      }
    } else if (trajmeas.mod_on.det==1) {
      normchi2_withpt_fpix_dist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
      normchi2_withpt_fpix_cumdist->Fill(trajmeas.trk.chi2/trajmeas.trk.ndof);
    }
  }
}

void drcl_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  drcl = sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]+trajmeas.dy_cl[0]*trajmeas.dy_cl[0]);
  drcl12 = sqrt((trajmeas.dx_cl[1]-trajmeas.dx_cl[0])*(trajmeas.dx_cl[1]-trajmeas.dx_cl[0])
		+(trajmeas.dy_cl[1]-trajmeas.dy_cl[0])*(trajmeas.dy_cl[1]-trajmeas.dy_cl[0]));
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	drcl_l1_eff->Fill(drcl);
	drcl_l1_cumeff->Fill(drcl);
	drcl12_l1_eff->Fill(drcl12);
	drcl12_l1_cumeff->Fill(drcl12);
	drcl12_drcl_l1_eff->Fill(drcl12,drcl);
	drcl12_drcl_l1_cumeff->Fill(drcl12,drcl);
      } else if (trajmeas.mod_on.layer==2) {
	drcl_l2_eff->Fill(drcl);
	drcl_l2_cumeff->Fill(drcl);
	drcl12_l2_eff->Fill(drcl12);
	drcl12_l2_cumeff->Fill(drcl12);
	drcl12_drcl_l2_eff->Fill(drcl12,drcl);
	drcl12_drcl_l2_cumeff->Fill(drcl12,drcl);
      } else if (trajmeas.mod_on.layer==3) {
	drcl_l3_eff->Fill(drcl);
	drcl_l3_cumeff->Fill(drcl);
	drcl12_l3_eff->Fill(drcl12);
	drcl12_l3_cumeff->Fill(drcl12);
	drcl12_drcl_l3_eff->Fill(drcl12,drcl);
	drcl12_drcl_l3_cumeff->Fill(drcl12,drcl);
      }
    } else if (trajmeas.mod_on.det==1) {
      drcl_fpix_eff->Fill(drcl);
      drcl_fpix_cumeff->Fill(drcl);
      drcl12_fpix_eff->Fill(drcl12);
      drcl12_fpix_cumeff->Fill(drcl12);
      drcl12_drcl_fpix_eff->Fill(drcl12,drcl);
      drcl12_drcl_fpix_cumeff->Fill(drcl12,drcl);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	drcl_l1_dist->Fill(drcl);
	drcl_l1_cumdist->Fill(drcl);
	drcl12_l1_dist->Fill(drcl12);
	drcl12_l1_cumdist->Fill(drcl12);
	drcl12_drcl_l1_dist->Fill(drcl12,drcl);
	drcl12_drcl_l1_cumdist->Fill(drcl12,drcl);
      } else if (trajmeas.mod_on.layer==2) {
	drcl_l2_dist->Fill(drcl);
	drcl_l2_cumdist->Fill(drcl);
	drcl12_l2_dist->Fill(drcl12);
	drcl12_l2_cumdist->Fill(drcl12);
	drcl12_drcl_l2_dist->Fill(drcl12,drcl);
	drcl12_drcl_l2_cumdist->Fill(drcl12,drcl);
      } else if (trajmeas.mod_on.layer==3) {
	drcl_l3_dist->Fill(drcl);
	drcl_l3_cumdist->Fill(drcl);
	drcl12_l3_dist->Fill(drcl12);
	drcl12_l3_cumdist->Fill(drcl12);
	drcl12_drcl_l3_dist->Fill(drcl12,drcl);
	drcl12_drcl_l3_cumdist->Fill(drcl12,drcl);
      }
    } else if (trajmeas.mod_on.det==1) {
      drcl_fpix_dist->Fill(drcl);
      drcl_fpix_cumdist->Fill(drcl);
      drcl12_fpix_dist->Fill(drcl12);
      drcl12_fpix_cumdist->Fill(drcl12);
      drcl12_drcl_fpix_dist->Fill(drcl12,drcl);
      drcl12_drcl_fpix_cumdist->Fill(drcl12,drcl);
    }
  }
}

void neweff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  drcl = sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]+trajmeas.dy_cl[0]*trajmeas.dy_cl[0]);
  if (trajmeas.validhit==1) {
    neweff_eff->SetBinContent(1,neweff_eff->GetBinContent(1)+1);
    neweff_dist->SetBinContent(1,neweff_dist->GetBinContent(1)+1);
    if (trajmeas.mod_on.det==0) {
      neweff_bpix_eff->SetBinContent(1,neweff_bpix_eff->GetBinContent(1)+1);
      neweff_bpix_dist->SetBinContent(1,neweff_bpix_dist->GetBinContent(1)+1);
      if (trajmeas.mod_on.layer==1) {
	neweff_l1_eff->SetBinContent(1,neweff_l1_eff->GetBinContent(1)+1);
	neweff_l1_dist->SetBinContent(1,neweff_l1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.layer==2) {
	neweff_l2_eff->SetBinContent(1,neweff_l2_eff->GetBinContent(1)+1);
	neweff_l2_dist->SetBinContent(1,neweff_l2_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.layer==3) {
	neweff_l3_eff->SetBinContent(1,neweff_l3_eff->GetBinContent(1)+1);
	neweff_l3_dist->SetBinContent(1,neweff_l3_dist->GetBinContent(1)+1);
      }
    } else if (trajmeas.mod_on.det==1) {
      neweff_fpix_eff->SetBinContent(1,neweff_fpix_eff->GetBinContent(1)+1);
      neweff_fpix_dist->SetBinContent(1,neweff_fpix_dist->GetBinContent(1)+1);
      if (trajmeas.mod_on.disk==-2) {
	neweff_dm2_eff->SetBinContent(1,neweff_dm2_eff->GetBinContent(1)+1);
	neweff_dm2_dist->SetBinContent(1,neweff_dm2_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==-1) {
	neweff_dm1_eff->SetBinContent(1,neweff_dm1_eff->GetBinContent(1)+1);
	neweff_dm1_dist->SetBinContent(1,neweff_dm1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==1) {
	neweff_dp1_eff->SetBinContent(1,neweff_dp1_eff->GetBinContent(1)+1);
	neweff_dp1_dist->SetBinContent(1,neweff_dp1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==2) {
	neweff_dp2_eff->SetBinContent(1,neweff2_dp2_eff->GetBinContent(1)+1);
	neweff_dp2_dist->SetBinContent(1,neweff2_dp2_dist->GetBinContent(1)+1);
      }
    }
  } else if (trajmeas.missing==1) {
    neweff_eff->Fill(drcl);
    neweff_dist->SetBinContent(1,neweff_dist->GetBinContent(1)+1);
    if (trajmeas.mod_on.det==0) {
      neweff_bpix_eff->Fill(drcl);
      neweff_bpix_dist->SetBinContent(1,neweff_bpix_dist->GetBinContent(1)+1);
      if (trajmeas.mod_on.layer==1) {
	neweff_l1_eff->Fill(drcl);
	neweff_l1_dist->SetBinContent(1,neweff_l1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.layer==2) {
	neweff_l2_eff->Fill(drcl);
	neweff_l2_dist->SetBinContent(1,neweff_l2_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.layer==3) {
	neweff_l3_eff->Fill(drcl);
	neweff_l3_dist->SetBinContent(1,neweff_l3_dist->GetBinContent(1)+1);
      }
    } else if (trajmeas.mod_on.det==1) {
      neweff_fpix_eff->Fill(drcl);
      neweff_fpix_dist->SetBinContent(1,neweff_fpix_dist->GetBinContent(1)+1);
      if (trajmeas.mod_on.disk==-2) {
	neweff_dm2_eff->Fill(drcl);
	neweff_dm2_dist->SetBinContent(1,neweff_dm2_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==-1) {
	neweff_dm1_eff->Fill(drcl);
	neweff_dm1_dist->SetBinContent(1,neweff_dm1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==1) {
	neweff_dp1_eff->Fill(drcl);
	neweff_dp1_dist->SetBinContent(1,neweff_dp1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==2) {
	neweff_dp2_eff->Fill(drcl);
	neweff_dp2_dist->SetBinContent(1,neweff2_dp2_dist->GetBinContent(1)+1);
      }
    }
  }
  if (trajmeas.validhit==1||trajmeas.missing==1) {
    neweff2_eff->Fill(drcl);
    neweff2_dist->SetBinContent(1,neweff2_dist->GetBinContent(1)+1);
    if (trajmeas.mod_on.det==0) {
      neweff2_bpix_eff->Fill(drcl);
      neweff2_bpix_dist->SetBinContent(1,neweff2_bpix_dist->GetBinContent(1)+1);
      if (trajmeas.mod_on.layer==1) {
	neweff2_l1_eff->Fill(drcl);
	neweff2_l1_dist->SetBinContent(1,neweff2_l1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.layer==2) {
	neweff2_l2_eff->Fill(drcl);
	neweff2_l2_dist->SetBinContent(1,neweff2_l2_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.layer==3) {
	neweff2_l3_eff->Fill(drcl);
	neweff2_l3_dist->SetBinContent(1,neweff2_l3_dist->GetBinContent(1)+1);
      }
    } else if (trajmeas.mod_on.det==1) {
      neweff2_fpix_eff->Fill(drcl);
      neweff2_fpix_dist->SetBinContent(1,neweff2_fpix_dist->GetBinContent(1)+1);
      if (trajmeas.mod_on.disk==-2) {
	neweff2_dm2_eff->Fill(drcl);
	neweff2_dm2_dist->SetBinContent(1,neweff2_dm2_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==-1) {
	neweff2_dm1_eff->Fill(drcl);
	neweff2_dm1_dist->SetBinContent(1,neweff2_dm1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==1) {
	neweff2_dp1_eff->Fill(drcl);
	neweff2_dp1_dist->SetBinContent(1,neweff2_dp1_dist->GetBinContent(1)+1);
      } else if (trajmeas.mod_on.disk==2) {
	neweff2_dp2_eff->Fill(drcl);
	neweff2_dp2_dist->SetBinContent(1,neweff2_dp2_dist->GetBinContent(1)+1);
      }
    }
  }
}

void clx_cly_fill(EventData &evt, TrajMeasurement &trajmeas) {
  drclx = sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]);
  drcly = sqrt(trajmeas.dy_cl[0]*trajmeas.dy_cl[0]);
  if (trajmeas.mod_on.det==0) {
    if (trajmeas.mod_on.layer==1) {
      if (trajmeas.validhit==1) {
        if (abs(trajmeas.mod_on.module)==1) {
          mod1_clx_cly_l1_neweff->SetBinContent(1,1,mod1_clx_cly_l1_neweff->GetBinContent(1,1)+1);
          mod1_clx_cly_l1_newdist->SetBinContent(1,1,mod1_clx_cly_l1_newdist->GetBinContent(1,1)+1);
          mod1_clx_l1_neweff->SetBinContent(1,mod1_clx_l1_neweff->GetBinContent(1)+1);
          mod1_clx_l1_newdist->SetBinContent(1,mod1_clx_l1_newdist->GetBinContent(1)+1);
          mod1_cly_l1_neweff->SetBinContent(1,mod1_cly_l1_neweff->GetBinContent(1)+1);
          mod1_cly_l1_newdist->SetBinContent(1,mod1_cly_l1_newdist->GetBinContent(1)+1);
        } else if (abs(trajmeas.mod_on.module)==4) {
          mod4_clx_cly_l1_neweff->SetBinContent(1,1,mod4_clx_cly_l1_neweff->GetBinContent(1,1)+1);
          mod4_clx_cly_l1_newdist->SetBinContent(1,1,mod4_clx_cly_l1_newdist->GetBinContent(1,1)+1);
          mod4_clx_l1_neweff->SetBinContent(1,mod4_clx_l1_neweff->GetBinContent(1)+1);
          mod4_clx_l1_newdist->SetBinContent(1,mod4_clx_l1_newdist->GetBinContent(1)+1);
          mod4_cly_l1_neweff->SetBinContent(1,mod4_cly_l1_neweff->GetBinContent(1)+1);
          mod4_cly_l1_newdist->SetBinContent(1,mod4_cly_l1_newdist->GetBinContent(1)+1);
        }
      } else if (trajmeas.missing==1) {
        if (abs(trajmeas.mod_on.module)==1) {
          mod1_clx_cly_l1_neweff->Fill(drcly,drclx);
          mod1_clx_cly_l1_newdist->SetBinContent(1,1,mod1_clx_cly_l1_newdist->GetBinContent(1,1)+1);
          mod1_clx_l1_neweff->Fill(drclx);
          mod1_clx_l1_newdist->SetBinContent(1,mod1_clx_l1_newdist->GetBinContent(1)+1);
          mod1_cly_l1_neweff->Fill(drcly);
          mod1_cly_l1_newdist->SetBinContent(1,mod1_cly_l1_newdist->GetBinContent(1)+1);
        } else if (abs(trajmeas.mod_on.module)==4) {
          mod4_clx_cly_l1_neweff->Fill(drcly,drclx);
          mod4_clx_cly_l1_newdist->SetBinContent(1,1,mod4_clx_cly_l1_newdist->GetBinContent(1,1)+1);
          mod4_clx_l1_neweff->Fill(drclx);
          mod4_clx_l1_newdist->SetBinContent(1,mod4_clx_l1_newdist->GetBinContent(1)+1);
          mod4_cly_l1_neweff->Fill(drcly);
          mod4_cly_l1_newdist->SetBinContent(1,mod4_cly_l1_newdist->GetBinContent(1)+1);
        }
      }
    } else if (trajmeas.mod_on.layer==2) {
      if (trajmeas.validhit==1) {
        if (abs(trajmeas.mod_on.module)==1) {
          mod1_clx_cly_l2_neweff->SetBinContent(1,1,mod1_clx_cly_l2_neweff->GetBinContent(1,1)+1);
          mod1_clx_cly_l2_newdist->SetBinContent(1,1,mod1_clx_cly_l2_newdist->GetBinContent(1,1)+1);
          mod1_clx_l2_neweff->SetBinContent(1,mod1_clx_l2_neweff->GetBinContent(1)+1);
          mod1_clx_l2_newdist->SetBinContent(1,mod1_clx_l2_newdist->GetBinContent(1)+1);
          mod1_cly_l2_neweff->SetBinContent(1,mod1_cly_l2_neweff->GetBinContent(1)+1);
          mod1_cly_l2_newdist->SetBinContent(1,mod1_cly_l2_newdist->GetBinContent(1)+1);
        } else if (abs(trajmeas.mod_on.module)==4) {
          mod4_clx_cly_l2_neweff->SetBinContent(1,1,mod4_clx_cly_l2_neweff->GetBinContent(1,1)+1);
          mod4_clx_cly_l2_newdist->SetBinContent(1,1,mod4_clx_cly_l2_newdist->GetBinContent(1,1)+1);
          mod4_clx_l2_neweff->SetBinContent(1,mod4_clx_l2_neweff->GetBinContent(1)+1);
          mod4_clx_l2_newdist->SetBinContent(1,mod4_clx_l2_newdist->GetBinContent(1)+1);
          mod4_cly_l2_neweff->SetBinContent(1,mod4_cly_l2_neweff->GetBinContent(1)+1);
          mod4_cly_l2_newdist->SetBinContent(1,mod4_cly_l2_newdist->GetBinContent(1)+1);
        }
      } else if (trajmeas.missing==1) {
        if (abs(trajmeas.mod_on.module)==1) {
          mod1_clx_cly_l2_neweff->Fill(drcly,drclx);
          mod1_clx_cly_l2_newdist->SetBinContent(1,1,mod1_clx_cly_l2_newdist->GetBinContent(1,1)+1);
          mod1_clx_l2_neweff->Fill(drclx);
          mod1_clx_l2_newdist->SetBinContent(1,mod1_clx_l2_newdist->GetBinContent(1)+1);
          mod1_cly_l2_neweff->Fill(drcly);
          mod1_cly_l2_newdist->SetBinContent(1,mod1_cly_l2_newdist->GetBinContent(1)+1);
        } else if (abs(trajmeas.mod_on.module)==4) {
          mod4_clx_cly_l2_neweff->Fill(drcly,drclx);
          mod4_clx_cly_l2_newdist->SetBinContent(1,1,mod4_clx_cly_l2_newdist->GetBinContent(1,1)+1);
          mod4_clx_l2_neweff->Fill(drclx);
          mod4_clx_l2_newdist->SetBinContent(1,mod4_clx_l2_newdist->GetBinContent(1)+1);
          mod4_cly_l2_neweff->Fill(drcly);
          mod4_cly_l2_newdist->SetBinContent(1,mod4_cly_l2_newdist->GetBinContent(1)+1);
        }
      }
    } else if (trajmeas.mod_on.layer==3) {
      if (trajmeas.validhit==1) {
        if (abs(trajmeas.mod_on.module)==1) {
          mod1_clx_cly_l3_neweff->SetBinContent(1,1,mod1_clx_cly_l3_neweff->GetBinContent(1,1)+1);
          mod1_clx_cly_l3_newdist->SetBinContent(1,1,mod1_clx_cly_l3_newdist->GetBinContent(1,1)+1);
          mod1_clx_l3_neweff->SetBinContent(1,mod1_clx_l3_neweff->GetBinContent(1)+1);
          mod1_clx_l3_newdist->SetBinContent(1,mod1_clx_l3_newdist->GetBinContent(1)+1);
          mod1_cly_l3_neweff->SetBinContent(1,mod1_cly_l3_neweff->GetBinContent(1)+1);
          mod1_cly_l3_newdist->SetBinContent(1,mod1_cly_l3_newdist->GetBinContent(1)+1);
        } else if (abs(trajmeas.mod_on.module)==4) {
          mod4_clx_cly_l3_neweff->SetBinContent(1,1,mod4_clx_cly_l3_neweff->GetBinContent(1,1)+1);
          mod4_clx_cly_l3_newdist->SetBinContent(1,1,mod4_clx_cly_l3_newdist->GetBinContent(1,1)+1);
          mod4_clx_l3_neweff->SetBinContent(1,mod4_clx_l3_neweff->GetBinContent(1)+1);
          mod4_clx_l3_newdist->SetBinContent(1,mod4_clx_l3_newdist->GetBinContent(1)+1);
          mod4_cly_l3_neweff->SetBinContent(1,mod4_cly_l3_neweff->GetBinContent(1)+1);
          mod4_cly_l3_newdist->SetBinContent(1,mod4_cly_l3_newdist->GetBinContent(1)+1);
        }
      } else if (trajmeas.missing==1) {
        if (abs(trajmeas.mod_on.module)==1) {
          mod1_clx_cly_l3_neweff->Fill(drcly,drclx);
          mod1_clx_cly_l3_newdist->SetBinContent(1,1,mod1_clx_cly_l3_newdist->GetBinContent(1,1)+1);
          mod1_clx_l3_neweff->Fill(drclx);
          mod1_clx_l3_newdist->SetBinContent(1,mod1_clx_l3_newdist->GetBinContent(1)+1);
          mod1_cly_l3_neweff->Fill(drcly);
          mod1_cly_l3_newdist->SetBinContent(1,mod1_cly_l3_newdist->GetBinContent(1)+1);
        } else if (abs(trajmeas.mod_on.module)==4) {
          mod4_clx_cly_l3_neweff->Fill(drcly,drclx);
          mod4_clx_cly_l3_newdist->SetBinContent(1,1,mod4_clx_cly_l3_newdist->GetBinContent(1,1)+1);
          mod4_clx_l3_neweff->Fill(drclx);
          mod4_clx_l3_newdist->SetBinContent(1,mod4_clx_l3_newdist->GetBinContent(1)+1);
          mod4_cly_l3_neweff->Fill(drcly);
          mod4_cly_l3_newdist->SetBinContent(1,mod4_cly_l3_newdist->GetBinContent(1)+1);
        }
      }
    }
  }
}

void drhit_drcl_neweff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  drhit = sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit);
  drcl = sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]+trajmeas.dy_cl[0]*trajmeas.dy_cl[0]);
  if (trajmeas.validhit==1) {
    drhit_binx = drhit_drcl_neweff_dist->GetXaxis()->FindBin(drhit);
    drhit_drcl_neweff_eff->SetBinContent(drhit_binx,1,drhit_drcl_neweff_eff->GetBinContent(drhit_binx,1)+1);
    drhit_drcl_neweff_dist->SetBinContent(drhit_binx,1,drhit_drcl_neweff_dist->GetBinContent(drhit_binx,1)+1);
    if (trajmeas.mod_on.det==0) {
      drhit_binx = drhit_drcl_neweff_bpix_dist->GetXaxis()->FindBin(drhit);
      drhit_drcl_neweff_bpix_eff->SetBinContent(drhit_binx,1,drhit_drcl_neweff_bpix_eff->GetBinContent(drhit_binx,1)+1);
      drhit_drcl_neweff_bpix_dist->SetBinContent(drhit_binx,1,drhit_drcl_neweff_bpix_dist->GetBinContent(drhit_binx,1)+1);
    } else if (trajmeas.mod_on.det==1) {
      drhit_binx = drhit_drcl_neweff_fpix_dist->GetXaxis()->FindBin(drhit);
      drhit_drcl_neweff_fpix_eff->SetBinContent(drhit_binx,1,drhit_drcl_neweff_fpix_eff->GetBinContent(drhit_binx,1)+1);
      drhit_drcl_neweff_fpix_dist->SetBinContent(drhit_binx,1,drhit_drcl_neweff_fpix_dist->GetBinContent(drhit_binx,1)+1);
    }
  } else if (trajmeas.missing==1) {
    drhit_drcl_neweff_eff->Fill(drhit,drcl);
    drhit_binx = drhit_drcl_neweff_dist->GetXaxis()->FindBin(drhit);
    drhit_drcl_neweff_dist->SetBinContent(drhit_binx,1,drhit_drcl_neweff_dist->GetBinContent(drhit_binx,1)+1);
    if (trajmeas.mod_on.det==0) {
      drhit_drcl_neweff_bpix_eff->Fill(drhit,drcl);
      drhit_binx = drhit_drcl_neweff_bpix_dist->GetXaxis()->FindBin(drhit);
      drhit_drcl_neweff_bpix_dist->SetBinContent(drhit_binx,1,drhit_drcl_neweff_bpix_dist->GetBinContent(drhit_binx,1)+1);
    } else if (trajmeas.mod_on.det==1) {
      drhit_drcl_neweff_fpix_eff->Fill(drhit,drcl);
      drhit_binx = drhit_drcl_neweff_fpix_dist->GetXaxis()->FindBin(drhit);
      drhit_drcl_neweff_fpix_dist->SetBinContent(drhit_binx,1,drhit_drcl_neweff_fpix_dist->GetBinContent(drhit_binx,1)+1);
    }
  }
}

void drhit_eff_fill(EventData &evt, TrajMeasurement &trajmeas) {
  drhit = sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit);
  if (trajmeas.validhit==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	drhit_l1_eff->Fill(drhit);
	drhit_l1_cumeff->Fill(drhit);
      } else if (trajmeas.mod_on.layer==2) {
	drhit_l2_eff->Fill(drhit);
	drhit_l2_cumeff->Fill(drhit);
      } else if (trajmeas.mod_on.layer==3) {
	drhit_l3_eff->Fill(drhit);
	drhit_l3_cumeff->Fill(drhit);
      }
    } else if (trajmeas.mod_on.det==1) {
      drhit_fpix_eff->Fill(drhit);
      drhit_fpix_cumeff->Fill(drhit);
    }
  } else if (trajmeas.missing==1) {
    if (trajmeas.mod_on.det==0) {
      if (trajmeas.mod_on.layer==1) {
	drhit_l1_dist->Fill(drhit);
	drhit_l1_cumdist->Fill(drhit);
      } else if (trajmeas.mod_on.layer==2) {
	drhit_l2_dist->Fill(drhit);
	drhit_l2_cumdist->Fill(drhit);
      } else if (trajmeas.mod_on.layer==3) {
	drhit_l3_dist->Fill(drhit);
	drhit_l3_cumdist->Fill(drhit);
      }
    } else if (trajmeas.mod_on.det==1) {
      drhit_fpix_dist->Fill(drhit);
      drhit_fpix_cumdist->Fill(drhit);
    }
  }
}

//binmaker for fid_calc functions
void binmaker(Int_t nroclx, Int_t nrocly, Int_t dimensions) {
  rocsizelx = 0.8; 
  rocsizely = 0.8;
  if (dimensions==2) {
    if (roclevel==1) {
      nbinslx = 1;
      noutlx = 0;
      nbinsly = 1;
      noutly = 0;
    } else {
      nbinslx = 32;
      noutlx = 8;
      nbinsly = 32;
      noutly = 32;
      if (nroclx==1&&nrocly==8) {
	nbinslx = 16;
	noutlx = 4;
	nbinsly = 16;
	noutly = 16;
      }
    }
  } else if (dimensions==1) {
    nbinslx = 64;
    noutlx = 16;
    nbinsly = 16;
    noutly = 16;
  }
  nbinsx = (nrocly * nbinsly + 2 * noutly);
  nbinsy = (nroclx * nbinslx + 2 * noutlx);
  xlow = -1.0 * nbinsx / 2 * rocsizely / nbinsly;
  xup = 1.0 * nbinsx / 2 * rocsizely / nbinsly;
  ylow = -1.0 * nbinsy / 2 * rocsizelx / nbinslx;
  yup = 1.0 * nbinsy / 2 * rocsizelx / nbinslx;
}

void outerfid_calc(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.layer==1) {
	if (trajmeas.mod_on.module==4)
	  outerfid_l1_p_eff->Fill(trajmeas.ly);
	else if (trajmeas.mod_on.module==-4)
	  outerfid_l1_m_eff->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==2) {
	if (trajmeas.mod_on.module==4)
	  outerfid_l2_p_eff->Fill(trajmeas.ly);
	else if (trajmeas.mod_on.module==-4)
	  outerfid_l2_m_eff->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==3) {
	if (trajmeas.mod_on.module==4)
	  outerfid_l3_p_eff->Fill(trajmeas.ly);
	else if (trajmeas.mod_on.module==-4)
	  outerfid_l3_m_eff->Fill(trajmeas.ly);
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.layer==1) {
	if (trajmeas.mod_on.module==4)
	  outerfid_l1_p_dist->Fill(trajmeas.ly);
	else if (trajmeas.mod_on.module==-4)
	  outerfid_l1_m_dist->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==2) {
	if (trajmeas.mod_on.module==4)
	  outerfid_l2_p_dist->Fill(trajmeas.ly);
	else if (trajmeas.mod_on.module==-4)
	  outerfid_l2_m_dist->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==3) {
	if (trajmeas.mod_on.module==4)
	  outerfid_l3_p_dist->Fill(trajmeas.ly);
	else if (trajmeas.mod_on.module==-4)
	  outerfid_l3_m_dist->Fill(trajmeas.ly);
      }
    }
  }
}



void lyfid_calc(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.layer==1) {
        if (trajmeas.mod_on.half==0)
          fid_l1_full_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==1)
          fid_l1_htI_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==10)
          fid_l1_hbI_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l1_htO_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-10)
          fid_l1_hbO_ly_eff->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==2) {
        if (trajmeas.mod_on.half==0)
          fid_l2_full_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==1)
          fid_l2_htI_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==16)
          fid_l2_hbI_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l2_htO_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-16)
          fid_l2_hbO_ly_eff->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==3) {
        if (trajmeas.mod_on.half==0)
          fid_l3_full_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==1)
          fid_l3_htI_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==22)
          fid_l3_hbI_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l3_htO_ly_eff->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-22)
          fid_l3_hbO_ly_eff->Fill(trajmeas.ly);
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.layer==1) {
        if (trajmeas.mod_on.half==0)
          fid_l1_full_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==1)
          fid_l1_htI_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==10)
          fid_l1_hbI_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l1_htO_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-10)
          fid_l1_hbO_ly_dist->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==2) {
        if (trajmeas.mod_on.half==0)
          fid_l2_full_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==1)
          fid_l2_htI_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==16)
          fid_l2_hbI_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l2_htO_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-16)
          fid_l2_hbO_ly_dist->Fill(trajmeas.ly);
      } else if (trajmeas.mod_on.layer==3) {
        if (trajmeas.mod_on.half==0)
          fid_l3_full_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==1)
          fid_l3_htI_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==22)
          fid_l3_hbI_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l3_htO_ly_dist->Fill(trajmeas.ly);
        else if (trajmeas.mod_on.ladder==-22)
          fid_l3_hbO_ly_dist->Fill(trajmeas.ly);
      }
    }
  } else if (trajmeas.mod_on.det==1) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.disk==-2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p1_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p1_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p1_m3_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dm2_p1_m4_ly_eff->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p2_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p2_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p2_m3_ly_eff->Fill(trajmeas.ly);
        }
      } else if (trajmeas.mod_on.disk==-1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p1_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p1_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p1_m3_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dm1_p1_m4_ly_eff->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p2_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p2_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p2_m3_ly_eff->Fill(trajmeas.ly);
        }
      } else if (trajmeas.mod_on.disk==1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p1_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p1_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p1_m3_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dp1_p1_m4_ly_eff->Fill(trajmeas.ly);
        }  else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p2_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p2_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p2_m3_ly_eff->Fill(trajmeas.ly);
        }
      } else if (trajmeas.mod_on.disk==2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p1_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p1_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p1_m3_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dp2_p1_m4_ly_eff->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p2_m1_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p2_m2_ly_eff->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p2_m3_ly_eff->Fill(trajmeas.ly);
        }
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.disk==-2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p1_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p1_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p1_m3_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dm2_p1_m4_ly_dist->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p2_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p2_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p2_m3_ly_dist->Fill(trajmeas.ly);
        }
      } else if (trajmeas.mod_on.disk==-1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p1_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p1_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p1_m3_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dm1_p1_m4_ly_dist->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p2_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p2_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p2_m3_ly_dist->Fill(trajmeas.ly);
        }
      } else if (trajmeas.mod_on.disk==1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p1_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p1_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p1_m3_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dp1_p1_m4_ly_dist->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p2_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p2_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p2_m3_ly_dist->Fill(trajmeas.ly);
        }
      } else if (trajmeas.mod_on.disk==2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p1_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p1_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p1_m3_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==4)
            fid_dp2_p1_m4_ly_dist->Fill(trajmeas.ly);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p2_m1_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p2_m2_ly_dist->Fill(trajmeas.ly);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p2_m3_ly_dist->Fill(trajmeas.ly);
        }
      }
    }
  }
}

void lxfid_calc(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.layer==1) {
        if (trajmeas.mod_on.half==0)
          fid_l1_full_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l1_htI_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==10)
          fid_l1_hbI_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l1_htO_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-10)
          fid_l1_hbO_lx_eff->Fill(trajmeas.lx);
      } else if (trajmeas.mod_on.layer==2) {
        if (trajmeas.mod_on.half==0)
          fid_l2_full_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l2_htI_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==16)
          fid_l2_hbI_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l2_htO_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-16)
          fid_l2_hbO_lx_eff->Fill(trajmeas.lx);
      } else if (trajmeas.mod_on.layer==3) {
        if (trajmeas.mod_on.half==0)
          fid_l3_full_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l3_htI_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==22)
          fid_l3_hbI_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l3_htO_lx_eff->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-22)
          fid_l3_hbO_lx_eff->Fill(trajmeas.lx);
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.layer==1) {
        if (trajmeas.mod_on.half==0)
          fid_l1_full_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l1_htI_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==10)
          fid_l1_hbI_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l1_htO_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-10)
          fid_l1_hbO_lx_dist->Fill(trajmeas.lx);
      } else if (trajmeas.mod_on.layer==2) {
        if (trajmeas.mod_on.half==0)
          fid_l2_full_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l2_htI_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==16)
          fid_l2_hbI_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l2_htO_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-16)
          fid_l2_hbO_lx_dist->Fill(trajmeas.lx);
      } else if (trajmeas.mod_on.layer==3) {
        if (trajmeas.mod_on.half==0)
          fid_l3_full_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l3_htI_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==22)
          fid_l3_hbI_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l3_htO_lx_dist->Fill(trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-22)
          fid_l3_hbO_lx_dist->Fill(trajmeas.lx);
      }
    }
  } else if (trajmeas.mod_on.det==1) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.disk==-2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p1_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p1_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p1_m3_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm2_p1_m4_lx_eff->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p2_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p2_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p2_m3_lx_eff->Fill(trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==-1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p1_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p1_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p1_m3_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm1_p1_m4_lx_eff->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p2_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p2_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p2_m3_lx_eff->Fill(trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p1_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p1_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p1_m3_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp1_p1_m4_lx_eff->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p2_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p2_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p2_m3_lx_eff->Fill(trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p1_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p1_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p1_m3_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp2_p1_m4_lx_eff->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p2_m1_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p2_m2_lx_eff->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p2_m3_lx_eff->Fill(trajmeas.lx);
        }
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.disk==-2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p1_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p1_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p1_m3_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm2_p1_m4_lx_dist->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p2_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p2_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p2_m3_lx_dist->Fill(trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==-1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p1_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p1_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p1_m3_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm1_p1_m4_lx_dist->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p2_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p2_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p2_m3_lx_dist->Fill(trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p1_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p1_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p1_m3_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp1_p1_m4_lx_dist->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p2_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p2_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p2_m3_lx_dist->Fill(trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p1_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p1_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p1_m3_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp2_p1_m4_lx_dist->Fill(trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p2_m1_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p2_m2_lx_dist->Fill(trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p2_m3_lx_dist->Fill(trajmeas.lx);
        }
      }
    }
  }
}

//Fiducial region calculations
void lxlyfid_calc(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.layer==1) {
        if (trajmeas.mod_on.half==0)
          fid_l1_full_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l1_htI_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==10)
          fid_l1_hbI_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l1_htO_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-10)
          fid_l1_hbO_eff->Fill(trajmeas.ly, trajmeas.lx);
      } else if (trajmeas.mod_on.layer==2) {
        if (trajmeas.mod_on.half==0)
          fid_l2_full_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l2_htI_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==16)
          fid_l2_hbI_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l2_htO_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-16)
          fid_l2_hbO_eff->Fill(trajmeas.ly, trajmeas.lx);
      } else if (trajmeas.mod_on.layer==3) {
        if (trajmeas.mod_on.half==0)
          fid_l3_full_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l3_htI_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==22)
          fid_l3_hbI_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l3_htO_eff->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-22)
          fid_l3_hbO_eff->Fill(trajmeas.ly, trajmeas.lx);
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.layer==1) {
        if (trajmeas.mod_on.half==0)
          fid_l1_full_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l1_htI_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==10)
          fid_l1_hbI_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l1_htO_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-10)
          fid_l1_hbO_dist->Fill(trajmeas.ly, trajmeas.lx);
      } else if (trajmeas.mod_on.layer==2) {
        if (trajmeas.mod_on.half==0)
          fid_l2_full_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l2_htI_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==16)
          fid_l2_hbI_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l2_htO_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-16)
          fid_l2_hbO_dist->Fill(trajmeas.ly, trajmeas.lx);
      } else if (trajmeas.mod_on.layer==3) {
        if (trajmeas.mod_on.half==0)
          fid_l3_full_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==1)
          fid_l3_htI_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==22)
          fid_l3_hbI_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-1)
          fid_l3_htO_dist->Fill(trajmeas.ly, trajmeas.lx);
        else if (trajmeas.mod_on.ladder==-22)
          fid_l3_hbO_dist->Fill(trajmeas.ly, trajmeas.lx);
      }
    }
  } else if (trajmeas.mod_on.det==1) {
    if (trajmeas.validhit==1) {
      if (trajmeas.mod_on.disk==-2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p1_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p1_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p1_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm2_p1_m4_eff->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p2_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p2_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p2_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==-1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p1_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p1_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p1_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm1_p1_m4_eff->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p2_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p2_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p2_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p1_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p1_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p1_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp1_p1_m4_eff->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p2_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p2_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p2_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p1_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p1_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p1_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp2_p1_m4_eff->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p2_m1_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p2_m2_eff->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p2_m3_eff->Fill(trajmeas.ly, trajmeas.lx);
        }
      }
    } else if (trajmeas.missing==1) {
      if (trajmeas.mod_on.disk==-2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p1_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p1_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p1_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm2_p1_m4_dist->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm2_p2_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm2_p2_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm2_p2_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==-1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p1_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p1_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p1_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dm1_p1_m4_dist->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dm1_p2_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dm1_p2_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dm1_p2_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==1) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p1_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p1_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p1_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp1_p1_m4_dist->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp1_p2_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp1_p2_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp1_p2_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
        }
      } else if (trajmeas.mod_on.disk==2) {
        if (trajmeas.mod_on.panel==1) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p1_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p1_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p1_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==4)
            fid_dp2_p1_m4_dist->Fill(trajmeas.ly, trajmeas.lx);
        } else if (trajmeas.mod_on.panel==2) {
          if (trajmeas.mod_on.module==1)
            fid_dp2_p2_m1_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==2)
            fid_dp2_p2_m2_dist->Fill(trajmeas.ly, trajmeas.lx);
          else if (trajmeas.mod_on.module==3)
            fid_dp2_p2_m3_dist->Fill(trajmeas.ly, trajmeas.lx);
        }
      }
    }
  }
} 

void ly_fid(EventData &evt, TrajMeasurement &trajmeas) {
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
       )
    lxfid_calc(evt, trajmeas);
}

void lx_fid(EventData &evt, TrajMeasurement &trajmeas) {
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
       )
    lyfid_calc(evt, trajmeas);
}

// Barrel pixel fiducial region
void bpix_fid(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0
      && fabs(trajmeas.ly)<3.0
      && ((trajmeas.mod_on.half==0&&fabs(trajmeas.lx)<0.55)
	  ||(trajmeas.mod_on.half==1&&fabs(trajmeas.lx)<0.35))) {
    // eff_fill(evt, trajmeas);
    // delay_fill(evt, trajmeas);
  }
}

void fpix_fid_new(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.panel==1) {
    plq = trajmeas.mod_on.module;
  } else if (trajmeas.mod_on.panel==2) {
    plq = (trajmeas.mod_on.module==3) ? 5 : trajmeas.mod_on.module+1;
  }
  if (trajmeas.mod_on.det==1
      && !(trajmeas.mod_on.panel==1&&trajmeas.mod_on.module==1)
      && !(trajmeas.mod_on.panel==1&&trajmeas.mod_on.module==4)
      && !(plq==2 &&fabs(trajmeas.ly)>=1.05)
      && !(plq==3 &&fabs(trajmeas.ly)>=1.4)
      && !(plq==5 &&fabs(trajmeas.ly)>=1.75)
      && fabs(trajmeas.lx)<0.7) {
    // eff_fill(evt, trajmeas);
    // delay_fill(evt, trajmeas);
  }
}

// Forward pixel fiducial region
void fpix_fid(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==1
      && fabs(trajmeas.ly)<3.0
      && fabs(trajmeas.lx)<0.55
      && (trajmeas.mod_on.panel==2
	  || (trajmeas.mod_on.panel==1
	      && (trajmeas.mod_on.module==2
		  || trajmeas.mod_on.module==3)))) {
    // eff_fill(evt, trajmeas);
    // delay_fill(evt, trajmeas);
  }
}

void fid_latest(EventData &evt, TrajMeasurement &trajmeas) {
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
//       if( (trajmeas.mod_on.det==0 && fabs(trajmeas.trk.dz)<0.1
// 	   &&( (trajmeas.mod_on.layer==1 && fabs(trajmeas.trk.d0)<0.01)
// 	       || (trajmeas.mod_on.layer!=1 && fabs(trajmeas.trk.d0)<0.02) ) )
// 	  || (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.dz)<0.5 && fabs(trajmeas.trk.d0)<0.05) ) {
// 	if (trajmeas.trk.strip>10) {
// 	  if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
// 	      ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2)) {
// 	    roc_l1_eff_fill(evt, trajmeas);
// 	  }
// 	}
//       }
//       if ((((trajmeas.mod_on.layer==1&&trajmeas.lxmatch!=NOVAL_F)||trajmeas.mod_on.layer!=1)&&trajmeas.validhit==1)||trajmeas.validhit!=1) {
      if( (trajmeas.mod_on.det==0 && fabs(trajmeas.trk.dz)<0.1
	   &&( (trajmeas.mod_on.layer==1 && fabs(trajmeas.trk.d0)<0.01)
	       || (trajmeas.mod_on.layer!=1 && fabs(trajmeas.trk.d0)<0.02) ) )
	  || (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.dz)<0.5 && fabs(trajmeas.trk.d0)<0.05) ) {
	if (trajmeas.trk.strip>10) {
	  if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
	      ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2)) {
	    // No Dr_hit cut for dr_hit plots
	    drhit_eff_fill(evt, trajmeas);
	    drhit_drcl_neweff_fill(evt, trajmeas);
// 	    if (abs(trajmeas.mod_on.module)==4) {
// 	      drhit_eff_fill(evt, trajmeas);
// 	      drhit_drcl_neweff_fill(evt, trajmeas);
// 	    }
	  }
	}
	if (trajmeas.dx_hit==NOVAL_F||sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit)>0.5) {
	  if( (trajmeas.mod_on.det==0 && fabs(trajmeas.trk.dz)<0.1
	       &&( (trajmeas.mod_on.layer==1 && fabs(trajmeas.trk.d0)<0.01)
		   || (trajmeas.mod_on.layer!=1 && fabs(trajmeas.trk.d0)<0.02) ) )
	      || (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.dz)<0.5 && fabs(trajmeas.trk.d0)<0.05) ) {
	    if (trajmeas.trk.strip>10) {
	      if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
		  ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2)) {
		// Old definition only (not works for neweff=1), also for dr_cl
		neweff_fill(evt, trajmeas);
		clx_cly_fill(evt, trajmeas);
		drcl_eff_fill(evt, trajmeas);
// 		if (abs(trajmeas.mod_on.module)==4) {
// 		  neweff_fill(evt, trajmeas);
// 		  drcl_eff_fill(evt, trajmeas);
// 		}
	      }
	    }
	  }
	}
      }
      if (neweff==1) {
// 	Float_t factor = 1.0;
// 	if (trajmeas.mod_on.det==0)
// 	  factor = (trajmeas.glx*trajmeas.glx+trajmeas.gly*trajmeas.gly+trajmeas.glz*trajmeas.glz)
// 	    / (trajmeas.glx*trajmeas.glx+trajmeas.gly*trajmeas.gly);
	if (trajmeas.missing==1&&sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]+trajmeas.dy_cl[0]*trajmeas.dy_cl[0])<0.05) {
	  trajmeas.missing=0;
	  trajmeas.validhit=1;
	}
      }
//       if ((((trajmeas.mod_on.layer==1&&trajmeas.lxmatch!=NOVAL_F)||trajmeas.mod_on.layer!=1)&&trajmeas.validhit==1)||trajmeas.validhit!=1) {
      if (trajmeas.dx_hit==NOVAL_F||sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit)>0.5) {
	if (trajmeas.trk.strip>10) {
	  if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
	      ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2))
	    dz_d0_eff_fill(evt, trajmeas);
	}
	if( (trajmeas.mod_on.det==0 && fabs(trajmeas.trk.dz)<0.1
	     &&( (trajmeas.mod_on.layer==1 && fabs(trajmeas.trk.d0)<0.01)
		 || (trajmeas.mod_on.layer!=1 && fabs(trajmeas.trk.d0)<0.02) ) )
	    || (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.dz)<0.5 && fabs(trajmeas.trk.d0)<0.05) ) {
	  if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
	      ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2))
	    nstrip_eff_fill(evt, trajmeas);
	  if (trajmeas.trk.strip>10) {
	    pt_eff_fill(evt, trajmeas);
	    normchi2_eff_fill(evt, trajmeas);
	    if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
		||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2)) {
// 	      if (evt.run==160431||(evt.run==160578&&evt.orb<108617422&&evt.orb>1245694)) {
	      eff_fill(evt, trajmeas);
	      mod_eff_fill(evt, trajmeas);
	      roc2_eff_fill(evt, trajmeas);
	      rocsel2_eff_fill(evt, trajmeas);
// 	      }
	      run_eff_fill(evt, trajmeas);
	      normchi2_withpt_eff_fill(evt, trajmeas);
	      nhit++;
	      outerfid_calc(evt, trajmeas);
#ifdef COMPLETE
	      if (effloss==1)
		effloss_fill(evt, trajmeas);
#endif
	      scans_fill(evt, trajmeas);
	    }
	  }
	}
      }
    }
  }
}


void trk_sel(EventData &evt, TrajMeasurement &trajmeas) {
  if ((trajmeas.trk.quality&4)!=0) {
    fid_latest(evt, trajmeas);
    if (trajmeas.dx_hit==NOVAL_F||sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit)>0.5) {
      if( (trajmeas.mod_on.det==0 && fabs(trajmeas.trk.dz)<0.1
	   &&( (trajmeas.mod_on.layer==1 && fabs(trajmeas.trk.d0)<0.01)
	       || (trajmeas.mod_on.layer!=1 && fabs(trajmeas.trk.d0)<0.02) ) )
	  || (trajmeas.mod_on.det==1 && fabs(trajmeas.trk.dz)<0.5 && fabs(trajmeas.trk.d0)<0.05) ) {
	if (trajmeas.trk.strip>10) {
	  if ((trajmeas.mod_on.det==0&&trajmeas.trk.pt>1.0)
	      ||(trajmeas.mod_on.det==1&&trajmeas.trk.pt>1.2)) {
	    if ((((trajmeas.mod_on.layer==1&&trajmeas.lxmatch!=NOVAL_F)
		  ||trajmeas.mod_on.layer!=1)&&trajmeas.validhit==1)||trajmeas.validhit!=1) {
	      if (neweff==1) {
// 		Float_t factor = 1.0;
// 		if (trajmeas.mod_on.det==0)
// 		  factor = (trajmeas.glx*trajmeas.glx+trajmeas.gly*trajmeas.gly+trajmeas.glz*trajmeas.glz)
// 		    / (trajmeas.glx*trajmeas.glx+trajmeas.gly*trajmeas.gly);
		if (trajmeas.missing==1&&sqrt(trajmeas.dx_cl[0]*trajmeas.dx_cl[0]+trajmeas.dy_cl[0]*trajmeas.dy_cl[0])<0.05) {
		  trajmeas.missing=0;
		  trajmeas.validhit=1;
		}
	      }
	      lxlyfid_calc(evt, trajmeas);
	      ly_fid(evt, trajmeas);
	      lx_fid(evt, trajmeas);
	    }
	  }
	}
      }
    } 
  }
}


void roc_sel(EventData &evt, TrajMeasurement &trajmeas) {
  if (trajmeas.mod_on.det==0) {
    for (Int_t i=-4;i<4;i++) {
      if (trajmeas.ly>=(i*0.8) && trajmeas.ly<((i+1)*0.8))
	roc = i+0.0;
    }
    col = (roc+0.5)/8;
    if (trajmeas.mod_on.half==1)
      pos = 0.25;
    else if (trajmeas.mod_on.half==0) {
      if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	pos = -0.25;
      else if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	pos = 0.25;
    }
    if (trajmeas.mod_on.ladder==-1 || trajmeas.mod_on.ladder==-3 
	|| trajmeas.mod_on.ladder==-5 || trajmeas.mod_on.ladder==-7 
	|| trajmeas.mod_on.ladder==-9 || trajmeas.mod_on.ladder==-11
	|| trajmeas.mod_on.ladder==-13 || trajmeas.mod_on.ladder==-15
	|| trajmeas.mod_on.ladder==-17 || trajmeas.mod_on.ladder==-19
	|| trajmeas.mod_on.ladder==-21 || trajmeas.mod_on.ladder==2 
	|| trajmeas.mod_on.ladder==4 || trajmeas.mod_on.ladder==6 
	|| trajmeas.mod_on.ladder==8 || trajmeas.mod_on.ladder==10
	|| trajmeas.mod_on.ladder==12 || trajmeas.mod_on.ladder==14
	|| trajmeas.mod_on.ladder==16 || trajmeas.mod_on.ladder==18
	|| trajmeas.mod_on.ladder==20 || trajmeas.mod_on.ladder==22)
      pos = -pos;
  } else if (trajmeas.mod_on.det==1) {
    if (trajmeas.mod_on.panel==1) {
      if (trajmeas.mod_on.module==1) {
	if (fabs(trajmeas.lx)<=0.4)
	  row = 2.0;
	for (Int_t i=-1;i<1;i++) {
	  if (trajmeas.ly>=(i*0.8) && trajmeas.ly<((i+1)*0.8))
	    roc = i+0.0;
	}
      } else if (trajmeas.mod_on.module==2) {
	if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	  row = 0.0;
	if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	  row = 1.0;
	for (Int_t i=-1;i<2;i++) {
	  if (trajmeas.ly>=((i-0.5)*0.8) && trajmeas.ly<((i+0.5)*0.8))
	    roc = i-0.5;
	}
      } else if (trajmeas.mod_on.module==3) {
	if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	  row = -2.0;
	if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	  row = -1.0;
	for (Int_t i=-2;i<2;i++) {
	  if (trajmeas.ly>=(i*0.8) && trajmeas.ly<((i+1)*0.8))
	    roc = i+0.0;
	}
      } else if (trajmeas.mod_on.module==4) {
	if (fabs(trajmeas.lx)<=0.4)
	  row = -3.0;
	for (Int_t i=-2;i<3;i++) {
	  if (trajmeas.ly>=((i-0.5)*0.8) && trajmeas.ly<((i+0.5)*0.8))
	    roc = i-0.5;
	}
      }
    }
    if (trajmeas.mod_on.panel==2) {
      if (trajmeas.mod_on.module==1) {
	if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	  row = 2.0;
	if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	  row = 1.0;
	for (Int_t i=-1;i<2;i++) {
	  if (trajmeas.ly>=((i-0.5)*0.8) && trajmeas.ly<((i+0.5)*0.8))
	    roc = i-0.5;
	}
      } else if (trajmeas.mod_on.module==2) {
	if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	  row = 0.0;
	if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	  row = -1.0;
	for (Int_t i=-2;i<2;i++) {
	  if (trajmeas.ly>=(i*0.8) && trajmeas.ly<((i+1)*0.8))
	    roc = i+0.0;
	}
      } else if (trajmeas.mod_on.module==3) {
	if (trajmeas.lx>=-0.8 && trajmeas.lx<0.0)
	  row = -2.0;
	if (trajmeas.lx>=0.0 && trajmeas.lx<=0.8)
	  row = -3.0;
	for (Int_t i=-2;i<3;i++) {
	  if (trajmeas.ly>=((i-0.5)*0.8) && trajmeas.ly<((i+0.5)*0.8))
	    roc = i-0.5;
	}
      }
    }
    pos = (roc+0.5)/5;
    if (trajmeas.mod_on.disk>0)
      sdpx = 2 * trajmeas.mod_on.disk - 2 + trajmeas.mod_on.panel;
    if (trajmeas.mod_on.disk<0)
      sdpx = 2 * trajmeas.mod_on.disk + 2 - trajmeas.mod_on.panel;
    col = (row+0.5)/6;
    if (trajmeas.mod_on.disk>0)
      col = -col;
  }
  if (trajmeas.mod_on.det==0) {
    if (trajmeas.mod_on.layer==1) {
      roceff = full2010runs_roc_l1_eff->GetBinContent(full2010runs_roc_l1_eff->GetXaxis()->FindBin(trajmeas.mod_on.module-col)
						 ,full2010runs_roc_l1_eff->GetYaxis()->FindBin(trajmeas.mod_on.ladder+pos));
      rocstat = full2010runs_roc_l1_dist->GetBinContent(full2010runs_roc_l1_dist->GetXaxis()->FindBin(trajmeas.mod_on.module-col)
						 ,full2010runs_roc_l1_dist->GetYaxis()->FindBin(trajmeas.mod_on.ladder+pos));
    } else if (trajmeas.mod_on.layer==2) {
      roceff = full2010runs_roc_l2_eff->GetBinContent(full2010runs_roc_l2_eff->GetXaxis()->FindBin(trajmeas.mod_on.module-col)
						 ,full2010runs_roc_l2_eff->GetYaxis()->FindBin(trajmeas.mod_on.ladder+pos));
      rocstat = full2010runs_roc_l2_dist->GetBinContent(full2010runs_roc_l2_dist->GetXaxis()->FindBin(trajmeas.mod_on.module-col)
						 ,full2010runs_roc_l2_dist->GetYaxis()->FindBin(trajmeas.mod_on.ladder+pos));
    } else if (trajmeas.mod_on.layer==3) {
      roceff = full2010runs_roc_l3_eff->GetBinContent(full2010runs_roc_l3_eff->GetXaxis()->FindBin(trajmeas.mod_on.module-col)
						 ,full2010runs_roc_l3_eff->GetYaxis()->FindBin(trajmeas.mod_on.ladder+pos));
      rocstat = full2010runs_roc_l3_dist->GetBinContent(full2010runs_roc_l3_dist->GetXaxis()->FindBin(trajmeas.mod_on.module-col)
						 ,full2010runs_roc_l3_dist->GetYaxis()->FindBin(trajmeas.mod_on.ladder+pos));
    }
  } else if (trajmeas.mod_on.det==1) {
    if (trajmeas.mod_on.blade>0) {
      roceff = full2010runs_roc_fpixI_eff->GetBinContent(full2010runs_roc_fpixI_eff->GetXaxis()->FindBin(sdpx+col)
						    ,full2010runs_roc_fpixI_eff->GetYaxis()->FindBin(trajmeas.mod_on.blade+pos-0.05));
      rocstat = full2010runs_roc_fpixI_dist->GetBinContent(full2010runs_roc_fpixI_eff->GetXaxis()->FindBin(sdpx+col)
						     ,full2010runs_roc_fpixI_eff->GetYaxis()->FindBin(trajmeas.mod_on.blade+pos-0.05));
    } else if (trajmeas.mod_on.blade<0) {
      roceff = full2010runs_roc_fpixO_eff->GetBinContent(full2010runs_roc_fpixO_eff->GetXaxis()->FindBin(sdpx+col)
						    ,full2010runs_roc_fpixO_eff->GetYaxis()->FindBin(trajmeas.mod_on.blade+pos-0.05));
      rocstat = full2010runs_roc_fpixO_dist->GetBinContent(full2010runs_roc_fpixO_eff->GetXaxis()->FindBin(sdpx+col)
						     ,full2010runs_roc_fpixO_eff->GetYaxis()->FindBin(trajmeas.mod_on.blade+pos-0.05));
    }
  }
  if (roceffsel!=1)
    trk_sel(evt, trajmeas);
  else if (roceffsel==1) {
    if ( (trajmeas.mod_on.det==0 && roceff<=bpix_higheff && roceff>=bpix_loweff)
	 || (trajmeas.mod_on.det==1 && roceff<=fpix_higheff && roceff>=fpix_loweff) ) {
      if (rocdistsel!=1)
	trk_sel(evt, trajmeas);
      else if (rocdistsel==1) {
	if ( (trajmeas.mod_on.det==0 && rocstat>bpix_rocstatcut)
	     || (trajmeas.mod_on.det==1 && rocstat>fpix_rocstatcut) ) {
	  trk_sel(evt, trajmeas);
	}
      }
    }
  }
}

void eff_cond(EventData &evt, TrajMeasurement &trajmeas) {
  if ((((trajmeas.mod_on.layer==1&&trajmeas.lxmatch!=NOVAL_F)||trajmeas.mod_on.layer!=1)&&trajmeas.validhit==1)||trajmeas.validhit!=1) {
    if (!(trajmeas.mod_on.disk==-1&&trajmeas.mod_on.blade==6&&trajmeas.mod_on.panel==1)) {
      if (!(trajmeas.mod_on.disk==-2&&trajmeas.mod_on.blade==10&&trajmeas.mod_on.panel==1)) {
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
	  roc_sel(evt, trajmeas);
	}
      }
    }
  }
}

void clu_sel(EventData &evt, TrajMeasurement &trajmeas) {
  eff_cond(evt, trajmeas);
//   if (trajmeas.dx_hit==NOVAL_F
//       || sqrt(trajmeas.dx_hit*trajmeas.dx_hit+trajmeas.dy_hit*trajmeas.dy_hit)>0.2)
//     eff_cond(evt, trajmeas);
}


// Event selection
void evt_sel(EventData &evt, TrajMeasurement &trajmeas) {
  // Event selection
  if (evt.orb>=orbmin
      && evt.orb<=orbmax
      && evt.nvtx>=1
      && evt.vtxndof>4) {
    clu_sel(evt, trajmeas);
  }
}

std::string filedir;

// Select which histograms to calculate
void fill_histo(std::string filename) {
  nfile++;
  std::string trajaddress = filedir + filename + "/trajTree";
  // load the tree from specified address and get their branches, 
  // then set their address compatible with data structures
  trajTree=new TChain("trajTree");
  trajTree->Add(trajaddress.c_str());
  b_t_evt = trajTree->GetBranch("event");
  b_t_evt->SetAddress(&evt);
  b_t_traj = trajTree->GetBranch("traj");
  b_t_traj->SetAddress(&trajmeas);
  b_t_mod_on = trajTree->GetBranch("module_on");
  b_t_mod_on->SetAddress(&trajmeas.mod_on);
  b_t_clu = trajTree->GetBranch("clust");
  b_t_clu->SetAddress(&trajmeas.clu);
  b_t_trk = trajTree->GetBranch("track");
  b_t_trk->SetAddress(&trajmeas.trk);
  Int_t clusters = 0;
#ifdef COMPLETE
  b_t_mod = trajTree->GetBranch("module");
  b_t_mod->SetAddress(&trajmeas.mod);
  std::string clustaddress = filedir + filename + "/clustTree";
  clustTree=new TChain("clustTree");
  clustTree->Add(clustaddress.c_str());
  b_c_evt = clustTree->GetBranch("event");
  b_c_evt->SetAddress(&cluevt);
  b_c_mod = clustTree->GetBranch("module");
  b_c_mod->SetAddress(&clust.mod);
  b_c_mod_on = clustTree->GetBranch("module_on");
  b_c_mod_on->SetAddress(&clust.mod_on);
  b_c_clu = clustTree->GetBranch("clust");
  b_c_clu->SetAddress(&clust);
  clusters = (Int_t)clustTree->GetEntries();
  Int_t clustentry = 0;
#endif
  // Fill Histograms
  Int_t trajhits = (Int_t)trajTree->GetEntries();
  for (Int_t i=0;i<trajhits;i++) {
    if (i==0) {
      std::cout<<"Opened file "<<nfile<<" which contains "<<trajhits<<" hits and "<<clusters<<" clusters."<<std::endl;
      nlumi=0;
      nevent=0;
      nhit=0;
    }
    trajTree->GetEntry(i);
    // do on first hit of an event
    // trajTree event number
#ifdef COMPLETE
    if (effloss==1) {
      if (evt.evt!=evtcheck) {
 	clustTree->GetEntry(clustentry);
	// get same event as in trajTree
	while (cluevt.evt!=evt.evt) {
	  if (clustentry!=clusters-1) {
	    clustentry++;
	    clustTree->GetEntry(clustentry);
	  } else {
	    break;
	  }
	}
	// count pixels in event - bpix
	npix_bpix_evt = 0;
	npix_l1_evt = 0;
	npix_l2_evt = 0;
	npix_l3_evt = 0;
	for (Int_t j=0;j<9;j++) {
	  for (Int_t k=0;k<45;k++) {
	    npix_mod_l3_evt[j][k]=0;
	    if (k<33)
	      npix_mod_l2_evt[j][k]=0;
	    if (k<21)
	      npix_mod_l1_evt[j][k]=0;
	  }
	}
	for (Int_t j=0;j<72;j++) {
	  for (Int_t k=0;k<90;k++) {
	    npix_roc_l3_evt[j][k]=0;
	    if (k<66)
	      npix_roc_l2_evt[j][k]=0;
	    if (k<42)
	      npix_roc_l1_evt[j][k]=0;
	  }
	}
	while (cluevt.evt==evt.evt) {
	  if (clustentry!=clusters-1) {
	    if (clust.mod_on.det==0) {
	      for (Int_t j=1;j<=8;j++) {
		if (clust.y>=((8-j)*52)&&clust.y<((9-j)*52))
		  cl_roc_binx = (clust.mod_on.module+4)*8 + j;
	      }
	      if (clust.mod_on.layer==1)
		cl_roc_biny = (clust.mod_on.ladder+10)*2 + 1;
	      else if (clust.mod_on.layer==2)
		cl_roc_biny = (clust.mod_on.ladder+16)*2 + 1;
	      else if (clust.mod_on.layer==3)
		cl_roc_biny = (clust.mod_on.ladder+22)*2 + 1;
	      if (((clust.mod_on.half==1||(clust.mod_on.half==0&&clust.x>=80.0))
		   &&(clust.mod_on.ladder%2==1||(clust.mod_on.ladder%2==0&&clust.mod_on.ladder<0)))
		  ||(clust.mod_on.half==0&&clust.x<80.0
		     &&(clust.mod_on.ladder%2==-1||(clust.mod_on.ladder%2==0&&clust.mod_on.ladder>0))))
		cl_roc_biny+=1;
	      if (clust.mod_on.layer==1) {
		npix_l1_evt+=clust.size;
		npix_mod_l1_evt[clust.mod_on.module+4][clust.mod_on.ladder+10]+=clust.size;
		npix_roc_l1_evt[cl_roc_binx-1][cl_roc_biny-1]+=clust.size;
	      } else if (clust.mod_on.layer==2) {
		npix_l2_evt+=clust.size;
		npix_mod_l2_evt[clust.mod_on.module+4][clust.mod_on.ladder+16]+=clust.size;
		npix_roc_l2_evt[cl_roc_binx-1][cl_roc_biny-1]+=clust.size;
	      } else if (clust.mod_on.layer==3) {
		npix_l3_evt+=clust.size;
		npix_mod_l3_evt[clust.mod_on.module+4][clust.mod_on.ladder+22]+=clust.size;
		npix_roc_l3_evt[cl_roc_binx-1][cl_roc_biny-1]+=clust.size;
	      }
	      npix_bpix_evt+=clust.size;
	    }
	    clustentry++;
	    clustTree->GetEntry(clustentry);
	  } else {
	    break;
	  }
	}	
	nevent++;
	totalevent++;
	if (evt.ls!=lscheck)  {
	  nlumi++;
	  totallumi++;
	}
	if (evt.run!=runcheck) {
	  nrun++;
	  rocsel_runno->SetBinContent(nrun,evt.run);
	}
	for (Int_t j=1;j<10;j++) {
	  if (totalevent==j*1000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*10000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*100000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*1000000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*10000000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	}
      }
    } else {
      if (evt.evt!=evtcheck) {
	nevent++;
	totalevent++;
	if (evt.ls!=lscheck)  {
	  nlumi++;
	  totallumi++;
	}
	if (evt.run!=runcheck) {
	  nrun++;
	  rocsel_runno->SetBinContent(nrun,evt.run);
	}
	for (Int_t j=1;j<10;j++) {
	  if (totalevent==j*1000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*10000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*100000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*1000000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	  else if (totalevent==j*10000000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	}
      }
    }
    evtcheck = evt.evt;
#else
    if (!(evt.ls==lscheck&&evt.nvtx==nvtxcheck&&evt.run==runcheck&&evt.vtxndof==vtxndofcheck)) {
      nevent++;
      totalevent++;
      if (evt.ls!=lscheck)  {
	nlumi++;
	totallumi++;
      }
      if (evt.run!=runcheck) {
	nrun++;
	rocsel_runno->SetBinContent(nrun,evt.run);
      }
      for (Int_t j=1;j<10;j++) {
	if (totalevent==j*1000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	else if (totalevent==j*10000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	else if (totalevent==j*100000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	else if (totalevent==j*1000000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
	else if (totalevent==j*10000000) std::cout<<"processed "<<totalevent<<" events and "<<i+1<<" hits"<<std::endl;
      }
    }
    nvtxcheck = evt.nvtx;
    vtxndofcheck = evt.vtxndof;
#endif
    lscheck = evt.ls;
    runcheck = evt.run;
    // selected range where efficiency doesn't fluctuate
    if (evt.run>=146804&&evt.run<=147390&&evt.run!=147116&&evt.run!=147196&&latestgoodrange==1)
      evt_sel(evt, trajmeas);
    else if (latestgoodrange!=1) {
      evt_sel(evt, trajmeas);
      coarsedelay_ontrack_clusize_fill(evt, trajmeas);
    }
    if (i==trajhits-1) {
      totalentries+=trajhits;
      totalhit+=nhit;
    }
  }
  delete trajTree;
#ifdef COMPLETE
  if (effloss==0&&clustloop==1) {
    for (Int_t i=0;i<clusters;i++) {
      clustTree->GetEntry(i);
      coarsedelay_clusize_fill(cluevt, clust);
    }
  }
  delete clustTree;
#endif
}

int main() {
  Int_t pcatomki = 0;
  neweff = 1;

  std::string strcomp = "";
#ifdef COMPLETE
  strcomp = "_comp";
#endif
  std::string streff;
  if (neweff==1)
    streff = "_neweff";
  else
    streff = "_oldeff";

  Int_t rocselstudy = 0;
  const char* rocselstudyin = "";
  std::string rocseloutfn = "rocselout_newrocdef" + streff + ".root";
  const char* rocselstudyout = rocseloutfn.c_str();

  Int_t effstudy = 1;
  Int_t fullrun = 0;
  latestgoodrange = 0;
  effloss = 0;
  clustloop = 1;
  roceffsel = 0;
  rocdistsel = 0;
  orbmin = 0;
  orbmax = 999999999;
  std::string strrundesc = "goodruns";
  std::string strrunname = "_full";
  std::string effoutfn = strrundesc + strrunname + streff + "_v1815" + strcomp + ".root";
  const char* effstudyout = effoutfn.c_str();

  Int_t roceffstudy = 0;
  const char* roceffstudyin = "oldeff_latest2010_newrocimput_v1815.root";
  std::string roceffoutfn = "roceffout_newrocdef" + streff + ".root";
  const char* roceffstudyout = roceffoutfn.c_str();

  if (rocselstudy==1) {
  
    TH3D *rocsel2_l1_eff;
    TH3D *rocsel2_l2_eff;
    TH3D *rocsel2_l3_eff;
    TH3D *rocsel2_fpixI_eff;
    TH3D *rocsel2_fpixO_eff;
    TH3D *rocsel2_l1_dist;
    TH3D *rocsel2_l2_dist;
    TH3D *rocsel2_l3_dist;
    TH3D *rocsel2_fpixI_dist;
    TH3D *rocsel2_fpixO_dist;
  
    TH1D *rocsel2_bpix_stat;
    TH1D *rocsel2_fpix_p1_stat;
    TH1D *rocsel2_fpix_p2_stat;
  
    TH1D *run2_ordered_bpix_dist;
    TH1D *run2_ordered_bpix_eff;
  
    TFile frocimput(rocselstudyin);
    rocsel2_l1_eff = (TH3D*)frocimput.Get("rocsel_l1_eff");
    rocsel2_l2_eff = (TH3D*)frocimput.Get("rocsel_l2_eff");
    rocsel2_l3_eff = (TH3D*)frocimput.Get("rocsel_l3_eff");
    rocsel2_fpixI_eff = (TH3D*)frocimput.Get("rocsel_fpixI_eff");
    rocsel2_fpixO_eff = (TH3D*)frocimput.Get("rocsel_fpixO_eff");
    rocsel2_l1_dist = (TH3D*)frocimput.Get("rocsel_l1_dist");
    rocsel2_l2_dist = (TH3D*)frocimput.Get("rocsel_l2_dist");
    rocsel2_l3_dist = (TH3D*)frocimput.Get("rocsel_l3_dist");
    rocsel2_fpixI_dist = (TH3D*)frocimput.Get("rocsel_fpixI_dist");
    rocsel2_fpixO_dist = (TH3D*)frocimput.Get("rocsel_fpixO_dist");
    rocsel2_l1_eff->SetDirectory(NULL);
    rocsel2_l2_eff->SetDirectory(NULL);
    rocsel2_l3_eff->SetDirectory(NULL);
    rocsel2_fpixI_eff->SetDirectory(NULL);
    rocsel2_fpixO_eff->SetDirectory(NULL);
    rocsel2_l1_dist->SetDirectory(NULL);
    rocsel2_l2_dist->SetDirectory(NULL);
    rocsel2_l3_dist->SetDirectory(NULL);
    rocsel2_fpixI_dist->SetDirectory(NULL);
    rocsel2_fpixO_dist->SetDirectory(NULL);
    rocsel2_bpix_stat = (TH1D*)frocimput.Get("rocsel_bpix_stat");
    rocsel2_fpix_p1_stat = (TH1D*)frocimput.Get("rocsel_fpix_p1_stat");
    rocsel2_fpix_p2_stat = (TH1D*)frocimput.Get("rocsel_fpix_p2_stat");
    rocsel2_bpix_stat->SetDirectory(NULL);
    rocsel2_fpix_p1_stat->SetDirectory(NULL);
    rocsel2_fpix_p2_stat->SetDirectory(NULL);
    run2_ordered_bpix_dist = (TH1D*)frocimput.Get("run_ordered_bpix_dist");
    run2_ordered_bpix_dist->SetDirectory(NULL);
    run2_ordered_bpix_eff = (TH1D*)frocimput.Get("run_ordered_bpix_eff");
    run2_ordered_bpix_eff->SetDirectory(NULL);
    frocimput.Close();
  
    Int_t nruns = 0;
    for (Int_t i=1;i<=1000;i++) {
      if (rocsel2_bpix_stat->GetBinContent(i)!=0)
        nruns++;
    }
  
//     TH1D *run_ordered = new TH1D("run_ordered","run_ordered",1000,0,1000);
//     TH1D *run_no = new TH1D("run_no","run_no",1000,0,1000);
//  
//     for (Int_t i=1;i<=1000;i++) {
//       if (i<=nruns) {
//         Int_t compare;
//         Int_t smallestvalue = 999999;
//         Int_t smallest;
//         for (Int_t j=1;j<=1000;j++) {
//   	compare = (Int_t)run2_order->GetBinContent(j);
//   	if (compare<smallestvalue) {
//   	  smallestvalue = compare;
//   	  smallest = j;
//   	}
//         }
//         run_ordered->SetBinContent(smallest,i);
//         run_no->SetBinContent(smallest,smallestvalue);
//         run2_order->SetBinContent(smallest,999999);
//       } else {
//         run_ordered->SetBinContent(i,i);
//       }
//     }
//  
//     TH3D *rocsel_ordered_l1_eff = new TH3D("rocsel_ordered_l1_eff","rocsel_ordered_l1_eff",72,-4.5,4.5,42,-10.5,10.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_l1_dist = new TH3D("rocsel_ordered_l1_dist","rocsel_ordered_l1_dist",72,-4.5,4.5,42,-10.5,10.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_l2_eff = new TH3D("rocsel_ordered_l2_eff","rocsel_ordered_l2_eff",72,-4.5,4.5,66,-16.5,16.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_l2_dist = new TH3D("rocsel_ordered_l2_dist","rocsel_ordered_l2_dist",72,-4.5,4.5,66,-16.5,16.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_l3_eff = new TH3D("rocsel_ordered_l3_eff","rocsel_ordered_l3_eff",72,-4.5,4.5,90,-22.5,22.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_l3_dist = new TH3D("rocsel_ordered_l3_dist","rocsel_ordered_l3_dist",72,-4.5,4.5,90,-22.5,22.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_fpixI_eff = new TH3D("rocsel_ordered_fpixI_eff", "rocsel_ordered_fpixI_eff", 54,-4.5,4.5,120,0.5,12.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_fpixI_dist = new TH3D("rocsel_ordered_fpixI_dist", "rocsel_ordered_fpixI_dist", 54,-4.5,4.5,120,0.5,12.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_fpixO_eff = new TH3D("rocsel_ordered_fpixO_eff", "rocsel_ordered_fpixO_eff", 54,-4.5,4.5,120,-12.5,-0.5,1000,0.5,1000.5);
//     TH3D *rocsel_ordered_fpixO_dist = new TH3D("rocsel_ordered_fpixO_dist", "rocsel_ordered_fpixO_dist", 54,-4.5,4.5,120,-12.5,-0.5,1000,0.5,1000.5);
//  
//     TH1D *rocsel_ordered_bpix_stat = new TH1D("rocsel_ordered_bpix_stat","rocsel_ordered_bpix_stat",1000,0.5,1000.5);
//     TH1D *rocsel_ordered_fpix_p1_stat = new TH1D("rocsel_ordered_fpix_p1_stat","rocsel_ordered_fpix_p1_stat",1000,0.5,1000.5);
//     TH1D *rocsel_ordered_fpix_p2_stat = new TH1D("rocsel_ordered_fpix_p2_stat","rocsel_ordered_fpix_p2_stat",1000,0.5,1000.5);
//  
//     for (Int_t k=1;k<=1000;k++) {
//       Int_t runbin = (Int_t)run_ordered->GetBinContent(k);
//       rocsel_ordered_bpix_stat->SetBinContent(runbin,rocsel2_bpix_stat->GetBinContent(k));
//       rocsel_ordered_fpix_p1_stat->SetBinContent(runbin,rocsel2_fpix_p1_stat->GetBinContent(k));
//       rocsel_ordered_fpix_p2_stat->SetBinContent(runbin,rocsel2_fpix_p2_stat->GetBinContent(k));
//       for (Int_t i=1;i<=72;i++) {
//         for (Int_t j=1;j<=42;j++) {
//   	rocsel_ordered_l1_eff->SetBinContent(i,j,runbin,rocsel2_l1_eff->GetBinContent(i,j,k));
//   	rocsel_ordered_l1_dist->SetBinContent(i,j,runbin,rocsel2_l1_dist->GetBinContent(i,j,k));
//         }
//         for (Int_t j=1;j<=66;j++) {
//   	rocsel_ordered_l2_eff->SetBinContent(i,j,runbin,rocsel2_l2_eff->GetBinContent(i,j,k));
//   	rocsel_ordered_l2_dist->SetBinContent(i,j,runbin,rocsel2_l2_dist->GetBinContent(i,j,k));
//  
//         }
//         for (Int_t j=1;j<=90;j++) {
//   	rocsel_ordered_l3_eff->SetBinContent(i,j,runbin,rocsel2_l3_eff->GetBinContent(i,j,k));
//   	rocsel_ordered_l3_dist->SetBinContent(i,j,runbin,rocsel2_l3_dist->GetBinContent(i,j,k));
//         }
//       }
//       for (Int_t i=1;i<=54;i++) {
//         for (Int_t j=1;j<=120;j++) {
//   	rocsel_ordered_fpixI_eff->SetBinContent(i,j,runbin,rocsel2_fpixI_eff->GetBinContent(i,j,k));
//   	rocsel_ordered_fpixI_dist->SetBinContent(i,j,runbin,rocsel2_fpixI_dist->GetBinContent(i,j,k));
//   	rocsel_ordered_fpixO_eff->SetBinContent(i,j,runbin,rocsel2_fpixO_eff->GetBinContent(i,j,k));
//   	rocsel_ordered_fpixO_dist->SetBinContent(i,j,runbin,rocsel2_fpixO_dist->GetBinContent(i,j,k));
//         }
//       }
//     }
  
  
    
    run2_ordered_bpix_eff->Multiply(run2_ordered_bpix_dist);
    run2_ordered_bpix_dist->Add(run2_ordered_bpix_eff,-1);
  
    TH1D *upstop_bpix = new TH1D("upstop_bpix","upstop_bpix",nruns,0.5,nruns+0.5);
    for (Int_t l=nruns;l>=1;l--) {
      Int_t done = 0;
      Int_t upstat = 0;
      Float_t up_val = 0;
      Float_t up_mis = 0;
      Float_t up_eff = 0;
      Float_t up_err = 0;
      for (Int_t k=l;k<nruns&&done==0;k++) {
        upstat += (Int_t)rocsel2_bpix_stat->GetBinContent(k+1);
        up_val += run2_ordered_bpix_eff->GetBinContent(k+1);
        up_mis += run2_ordered_bpix_dist->GetBinContent(k+1);
        if (upstat>5000000) {
  	done=1;
  	up_eff = up_val / (up_val + up_mis);
  	up_err = sqrt(up_eff*(1-up_eff)/(up_val+up_mis));
        }
      }
      done = 0;
      Int_t downstat = 0;
      Float_t down_val = 0;
      Float_t down_mis = 0;
      Float_t down_eff = 0;
      Float_t down_err = 0;
      for (Int_t k=l;k>=1&&done==0;k--) {
        downstat += (Int_t)rocsel2_bpix_stat->GetBinContent(k);
        down_val += run2_ordered_bpix_eff->GetBinContent(k);
        down_mis += run2_ordered_bpix_dist->GetBinContent(k);
        if (downstat>5000000) {
  	done=1;
  	down_eff = down_val / (down_val + down_mis);
  	down_err = sqrt(down_eff*(1-down_eff)/(down_val+down_mis));
        }
      }
      if (fabs(up_eff-down_eff)>0.0005&&down_eff!=0&&up_eff!=0)
  	upstop_bpix->Fill(l);
    }
  
    TH1D *downstop_bpix = new TH1D("downstop_bpix","downstop_bpix",nruns,0.5,nruns+0.5);
    for (Int_t l=nruns;l>=1;l--) {
      Int_t done = 0;
      Int_t upstat = 0;
      Float_t up_val = 0;
      Float_t up_mis = 0;
      Float_t up_eff = 0;
      Float_t up_err = 0;
      for (Int_t k=l;k<=nruns&&done==0;k++) {
        upstat += (Int_t)rocsel2_bpix_stat->GetBinContent(k);
        up_val += run2_ordered_bpix_eff->GetBinContent(k);
        up_mis += run2_ordered_bpix_dist->GetBinContent(k);
        if (upstat>5000000) {
  	done=1;
  	up_eff = up_val / (up_val + up_mis);
  	up_err = sqrt(up_eff*(1-up_eff)/(up_val+up_mis));
        }
      }
      done = 0;
      Int_t downstat = 0;
      Float_t down_val = 0;
      Float_t down_mis = 0;
      Float_t down_eff = 0;
      Float_t down_err = 0;
      for (Int_t k=l;k>1&&done==0;k--) {
        downstat += (Int_t)rocsel2_bpix_stat->GetBinContent(k-1);
        down_val += run2_ordered_bpix_eff->GetBinContent(k-1);
        down_mis += run2_ordered_bpix_dist->GetBinContent(k-1);
        if (downstat>5000000) {
  	done=1;
  	down_eff = down_val / (down_val + down_mis);
  	down_err = sqrt(down_eff*(1-down_eff)/(down_val+down_mis));
        }
      }
      if (fabs(up_eff-down_eff)>0.0005&&down_eff!=0&&up_eff!=0)
        downstop_bpix->Fill(l);
    }
  
    TH1D *mean_fit_bpix = new TH1D("mean_fit_bpix","mean_fit_bpix",nruns,0.5,nruns+0.5);
    TH1D *sigma_fit_bpix = new TH1D("sigma_fit_bpix","sigma_fit_bpix",nruns,0.5,nruns+0.5);
    TH1D *mean_rocstatcut_bpix = new TH1D("mean_rocstatcut_bpix","mean_rocstatcut_bpix",nruns,0.5,nruns+0.5);
    TH1D *sigma_rocstatcut_bpix = new TH1D("sigma_rocstatcut_bpix","sigma_rocstatcut_bpix",nruns,0.5,nruns+0.5);
    TH1D *mergedruns_bpix = new TH1D("mergedruns_bpix","mergedruns_bpix",nruns,0.5,nruns+0.5);
    TH1D *statistics_bpix = new TH1D("statistics_bpix","statistics_bpix",nruns,0.5,nruns+0.5);
  
    for (Int_t l=nruns;l>=1;l--) {
      TH2D *roctest_l1_eff = new TH2D("roctest_l1_eff","roctest_l1_eff",72,-4.5,4.5,42,-10.5,10.5);
      TH2D *roctest_l1_dist = new TH2D("roctest_l1_dist","roctest_l1_dist",72,-4.5,4.5,42,-10.5,10.5);
      TH2D *roctest_l2_eff = new TH2D("roctest_l2_eff","roctest_l2_eff",72,-4.5,4.5,66,-16.5,16.5);
      TH2D *roctest_l2_dist = new TH2D("roctest_l2_dist","roctest_l2_dist",72,-4.5,4.5,66,-16.5,16.5);
      TH2D *roctest_l3_eff = new TH2D("roctest_l3_eff","roctest_l3_eff",72,-4.5,4.5,90,-22.5,22.5);
      TH2D *roctest_l3_dist = new TH2D("roctest_l3_dist","roctest_l3_dist",72,-4.5,4.5,90,-22.5,22.5);
  
      Int_t done = 0;
      Int_t goup = 0;
      Int_t godown = 0;
      Int_t up = 0;
      Int_t down = 0;
      Int_t upstat = 0;
      Int_t downstat = 0;
      Int_t downstop = 0;
      Int_t upstop = 0;
      Float_t statistics = 0;
      for (Int_t k=1;k<=100&&done==0;k++) {
        Int_t m = l + goup * up - godown * down;
        downstat += godown * (Int_t)rocsel2_bpix_stat->GetBinContent(m);
        upstat += goup * (Int_t)rocsel2_bpix_stat->GetBinContent(m);
        if (m==1) {
  	downstop=1;
        }
        if (m==nruns) {
  	upstop=1;
        }
        if (downstat<=upstat&&downstop!=1) {
  	godown = 1;
  	goup = 0;
  	down++;
        } else if (upstat<downstat&&upstop!=1) {
  	goup = 1;
  	godown = 0;
  	up++;
        } else if (downstop==1) {
  	goup = 1;
  	godown = 0;
  	up++;
        } else if (upstop==1) {
  	godown = 1;
  	goup = 0;
  	down++;
        }
        for (Int_t i=1;i<=72;i++) {
          for (Int_t j=1;j<=42;j++) {
  	  roctest_l1_eff->SetBinContent(i,j,roctest_l1_eff->GetBinContent(i,j)+rocsel2_l1_eff->GetBinContent(i,j,m));
  	  roctest_l1_dist->SetBinContent(i,j,roctest_l1_dist->GetBinContent(i,j)+rocsel2_l1_dist->GetBinContent(i,j,m));
          }
          for (Int_t j=1;j<=66;j++) {
  	  roctest_l2_eff->SetBinContent(i,j,roctest_l2_eff->GetBinContent(i,j)+rocsel2_l2_eff->GetBinContent(i,j,m));
  	  roctest_l2_dist->SetBinContent(i,j,roctest_l2_dist->GetBinContent(i,j)+rocsel2_l2_dist->GetBinContent(i,j,m));
          }
          for (Int_t j=1;j<=90;j++) {
  	  roctest_l3_eff->SetBinContent(i,j,roctest_l3_eff->GetBinContent(i,j)+rocsel2_l3_eff->GetBinContent(i,j,m));
  	  roctest_l3_dist->SetBinContent(i,j,roctest_l3_dist->GetBinContent(i,j)+rocsel2_l3_dist->GetBinContent(i,j,m));
          }
        }
        statistics += rocsel2_bpix_stat->GetBinContent(m);
        if (statistics>10000000) {
  	std::cout<<"Run "<<l<<" "<<down<<" "<<up<<" "<<downstat<<" "<<upstat<<std::endl;
          done = 1;
  
          mergedruns_bpix->Fill(l,down+up+1);
          statistics_bpix->Fill(l,statistics);
  
          roctest_l1_dist->Add(roctest_l1_eff);
          roctest_l1_eff->Divide(roctest_l1_dist);
          roctest_l2_dist->Add(roctest_l2_eff);
          roctest_l2_eff->Divide(roctest_l2_dist);
          roctest_l3_dist->Add(roctest_l3_eff);
          roctest_l3_eff->Divide(roctest_l3_dist);
  
          // Roceff Dist - testing fitting
          TH1D *test_roceff_bpix = new TH1D("test_roceff_bpix","test_roceff_bpix",999,0.001,1.0);
          TH1D *test_rocdist_bpix = new TH1D("test_rocdist_bpix","test_rocdist_bpix",300,0.0,15000.0);
    
          Int_t maxroc_bpix = 0;
  
          for (Int_t i=1;i<=72;i++) {
            for (Int_t j=1;j<=42;j++) {
              if (roctest_l1_dist->GetBinContent(i,j)!=0) {
                test_roceff_bpix->Fill(roctest_l1_eff->GetBinContent(i,j));
                test_rocdist_bpix->Fill(roctest_l1_dist->GetBinContent(i,j));
  	      maxroc_bpix++;
              }
            }
            for (Int_t j=1;j<=66;j++) {
              if (roctest_l2_dist->GetBinContent(i,j)!=0) {
                test_roceff_bpix->Fill(roctest_l2_eff->GetBinContent(i,j));
                test_rocdist_bpix->Fill(roctest_l2_dist->GetBinContent(i,j));
  	      maxroc_bpix++;
              }
            }
            for (Int_t j=1;j<=90;j++) {
              if (roctest_l3_dist->GetBinContent(i,j)!=0) {
                test_roceff_bpix->Fill(roctest_l3_eff->GetBinContent(i,j));
                test_rocdist_bpix->Fill(roctest_l3_dist->GetBinContent(i,j));
  	      maxroc_bpix++;
              }
            }
          }
  
          TF1 *gausfit_bpix = new TF1("gausfit_bpix","gaus",0.9,1.0);
          test_roceff_bpix->Fit("gausfit_bpix","RMWQ");
          Float_t mean_bpix = test_roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(1);
          Float_t sigma_bpix = test_roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(2);
          Float_t minstat_bpix = (1 - mean_bpix) * mean_bpix / (sigma_bpix * sigma_bpix);
  
          mean_fit_bpix->Fill(l,mean_bpix);
          sigma_fit_bpix->Fill(l,sigma_bpix);
  
          TH1D *test2_roceff_bpix = new TH1D("test2_roceff_bpix","test2_roceff_bpix",999,0.001,1.0);
          TH1D *test2_rocdist_bpix = new TH1D("test2_rocdist_bpix","test2_rocdist_bpix",300,0.0,15000.0);
  
          Int_t statroc_bpix = 0;
  
          for (Int_t i=1;i<=72;i++) {
            for (Int_t j=1;j<=42;j++) {
              if (roctest_l1_dist->GetBinContent(i,j)>minstat_bpix) {
                test2_roceff_bpix->Fill(roctest_l1_eff->GetBinContent(i,j));
                test2_rocdist_bpix->Fill(roctest_l1_dist->GetBinContent(i,j));
  	      statroc_bpix++;
              }
            }
            for (Int_t j=1;j<=66;j++) {
              if (roctest_l2_dist->GetBinContent(i,j)>minstat_bpix) {
                test2_roceff_bpix->Fill(roctest_l2_eff->GetBinContent(i,j));
                test2_rocdist_bpix->Fill(roctest_l2_dist->GetBinContent(i,j));
  	      statroc_bpix++;
              }
            }
            for (Int_t j=1;j<=90;j++) {
              if (roctest_l3_dist->GetBinContent(i,j)>minstat_bpix) {
                test2_roceff_bpix->Fill(roctest_l3_eff->GetBinContent(i,j));
                test2_rocdist_bpix->Fill(roctest_l3_dist->GetBinContent(i,j));
  	      statroc_bpix++;
              }
            }
          }
  
          TF1 *gausfit_statcut_bpix = new TF1("gausfit_statcut_bpix","gaus",0.9,1.0);
          test2_roceff_bpix->Fit("gausfit_statcut_bpix","RMWQ");
          Float_t mean_statcut_bpix = test2_roceff_bpix->GetFunction("gausfit_statcut_bpix")->GetParameter(1);
          Float_t sigma_statcut_bpix = test2_roceff_bpix->GetFunction("gausfit_statcut_bpix")->GetParameter(2);
  
          mean_rocstatcut_bpix->Fill(l,mean_statcut_bpix);
          sigma_rocstatcut_bpix->Fill(l,sigma_statcut_bpix);
  
          // Float_t statroc = statroc_bpix;
          // Float_t maxroc = maxroc_bpix;
          // Float_t ratio_bpix = statroc/maxroc;
  
//           std::cout<<"Run: "<<l<<" mean: "<<mean_bpix<<" sigma: "<<sigma_bpix<<" statistics: "<<statistics<<" ratio: "<<ratio_bpix<<std::endl;
    
  	delete gausfit_bpix;
          delete gausfit_statcut_bpix;
          delete test_roceff_bpix;
          delete test_rocdist_bpix;
          delete test2_roceff_bpix;
          delete test2_rocdist_bpix;
        }
      }
      delete roctest_l1_eff;
      delete roctest_l1_dist;
      delete roctest_l2_eff;
      delete roctest_l2_dist;
      delete roctest_l3_eff;
      delete roctest_l3_dist;
    }
  
  
//     for (Int_t l=nruns;l>=1;l--) {
//       TH2D *roctest_l1_eff = new TH2D("roctest_l1_eff","roctest_l1_eff",72,-4.5,4.5,42,-10.5,10.5);
//       TH2D *roctest_l1_dist = new TH2D("roctest_l1_dist","roctest_l1_dist",72,-4.5,4.5,42,-10.5,10.5);
//       TH2D *roctest_l2_eff = new TH2D("roctest_l2_eff","roctest_l2_eff",72,-4.5,4.5,66,-16.5,16.5);
//       TH2D *roctest_l2_dist = new TH2D("roctest_l2_dist","roctest_l2_dist",72,-4.5,4.5,66,-16.5,16.5);
//       TH2D *roctest_l3_eff = new TH2D("roctest_l3_eff","roctest_l3_eff",72,-4.5,4.5,90,-22.5,22.5);
//       TH2D *roctest_l3_dist = new TH2D("roctest_l3_dist","roctest_l3_dist",72,-4.5,4.5,90,-22.5,22.5);
//  
//       Int_t enoughstat = 0;
//       Float_t statistics = 0;
//       for (Int_t k=l;k>=1&&enoughstat==0;k--) {
//         for (Int_t i=1;i<=72;i++) {
//           for (Int_t j=1;j<=42;j++) {
//   	  roctest_l1_eff->SetBinContent(i,j,roctest_l1_eff->GetBinContent(i,j)+rocsel2_l1_eff->GetBinContent(i,j,k));
//   	  roctest_l1_dist->SetBinContent(i,j,roctest_l1_dist->GetBinContent(i,j)+rocsel2_l1_dist->GetBinContent(i,j,k));
//           }
//           for (Int_t j=1;j<=66;j++) {
//   	  roctest_l2_eff->SetBinContent(i,j,roctest_l2_eff->GetBinContent(i,j)+rocsel2_l2_eff->GetBinContent(i,j,k));
//   	  roctest_l2_dist->SetBinContent(i,j,roctest_l2_dist->GetBinContent(i,j)+rocsel2_l2_dist->GetBinContent(i,j,k));
//           }
//           for (Int_t j=1;j<=90;j++) {
//   	  roctest_l3_eff->SetBinContent(i,j,roctest_l3_eff->GetBinContent(i,j)+rocsel2_l3_eff->GetBinContent(i,j,k));
//   	  roctest_l3_dist->SetBinContent(i,j,roctest_l3_dist->GetBinContent(i,j)+rocsel2_l3_dist->GetBinContent(i,j,k));
//           }
//         }
//         statistics += rocsel2_bpix_stat->GetBinContent(k);
//         if (statistics>10000000) {
//           enoughstat = 1;
//  
//           mergedruns_bpix->Fill(l,l-k+1);
//           statistics_bpix->Fill(l,statistics);
//  
//           roctest_l1_dist->Add(roctest_l1_eff);
//           roctest_l1_eff->Divide(roctest_l1_dist);
//           roctest_l2_dist->Add(roctest_l2_eff);
//           roctest_l2_eff->Divide(roctest_l2_dist);
//           roctest_l3_dist->Add(roctest_l3_eff);
//           roctest_l3_eff->Divide(roctest_l3_dist);
//  
//           // Roceff Dist - testing fitting
//           TH1D *test_roceff_bpix = new TH1D("test_roceff_bpix","test_roceff_bpix",999,0.001,1.0);
//           TH1D *test_rocdist_bpix = new TH1D("test_rocdist_bpix","test_rocdist_bpix",300,0.0,15000.0);
//  
//           Int_t maxroc_bpix = 0;
//  
//           for (Int_t i=1;i<=72;i++) {
//             for (Int_t j=1;j<=42;j++) {
//               if (roctest_l1_dist->GetBinContent(i,j)!=0) {
//                 test_roceff_bpix->Fill(roctest_l1_eff->GetBinContent(i,j));
//                 test_rocdist_bpix->Fill(roctest_l1_dist->GetBinContent(i,j));
//   	      maxroc_bpix++;
//               }
//             }
//             for (Int_t j=1;j<=66;j++) {
//               if (roctest_l2_dist->GetBinContent(i,j)!=0) {
//                 test_roceff_bpix->Fill(roctest_l2_eff->GetBinContent(i,j));
//                 test_rocdist_bpix->Fill(roctest_l2_dist->GetBinContent(i,j));
//   	      maxroc_bpix++;
//               }
//             }
//             for (Int_t j=1;j<=90;j++) {
//               if (roctest_l3_dist->GetBinContent(i,j)!=0) {
//                 test_roceff_bpix->Fill(roctest_l3_eff->GetBinContent(i,j));
//                 test_rocdist_bpix->Fill(roctest_l3_dist->GetBinContent(i,j));
//   	      maxroc_bpix++;
//               }
//             }
//           }
//  
//           TF1 *gausfit_bpix = new TF1("gausfit_bpix","gaus",0.9,1.0);
//           test_roceff_bpix->Fit("gausfit_bpix","RMWQ");
//           Float_t mean_bpix = test_roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(1);
//           Float_t sigma_bpix = test_roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(2);
//           Float_t minstat_bpix = (1 - mean_bpix) * mean_bpix / (sigma_bpix * sigma_bpix);
//  
//           mean_fit_bpix->Fill(l,mean_bpix);
//           sigma_fit_bpix->Fill(l,sigma_bpix);
//  
//           TH1D *test2_roceff_bpix = new TH1D("test2_roceff_bpix","test2_roceff_bpix",999,0.001,1.0);
//           TH1D *test2_rocdist_bpix = new TH1D("test2_rocdist_bpix","test2_rocdist_bpix",300,0.0,15000.0);
//  
//           Int_t statroc_bpix = 0;
//  
//           for (Int_t i=1;i<=72;i++) {
//             for (Int_t j=1;j<=42;j++) {
//               if (roctest_l1_dist->GetBinContent(i,j)>minstat_bpix) {
//                 test2_roceff_bpix->Fill(roctest_l1_eff->GetBinContent(i,j));
//                 test2_rocdist_bpix->Fill(roctest_l1_dist->GetBinContent(i,j));
//   	      statroc_bpix++;
//               }
//             }
//             for (Int_t j=1;j<=66;j++) {
//               if (roctest_l2_dist->GetBinContent(i,j)>minstat_bpix) {
//                 test2_roceff_bpix->Fill(roctest_l2_eff->GetBinContent(i,j));
//                 test2_rocdist_bpix->Fill(roctest_l2_dist->GetBinContent(i,j));
//   	      statroc_bpix++;
//               }
//             }
//             for (Int_t j=1;j<=90;j++) {
//               if (roctest_l3_dist->GetBinContent(i,j)>minstat_bpix) {
//                 test2_roceff_bpix->Fill(roctest_l3_eff->GetBinContent(i,j));
//                 test2_rocdist_bpix->Fill(roctest_l3_dist->GetBinContent(i,j));
//   	      statroc_bpix++;
//               }
//             }
//           }
//  
//           TF1 *gausfit_statcut_bpix = new TF1("gausfit_statcut_bpix","gaus",0.9,1.0);
//           test2_roceff_bpix->Fit("gausfit_statcut_bpix","RMWQ");
//           Float_t mean_statcut_bpix = test2_roceff_bpix->GetFunction("gausfit_statcut_bpix")->GetParameter(1);
//           Float_t sigma_statcut_bpix = test2_roceff_bpix->GetFunction("gausfit_statcut_bpix")->GetParameter(2);
//  
//           mean_rocstatcut_bpix->Fill(l,mean_statcut_bpix);
//           sigma_rocstatcut_bpix->Fill(l,sigma_statcut_bpix);
//  
//           Float_t statroc = statroc_bpix;
//           Float_t maxroc = maxroc_bpix;
//           Float_t ratio_bpix = statroc/maxroc;
//  
//           std::cout<<"Run: "<<l<<" mean: "<<mean_bpix<<" sigma: "<<sigma_bpix<<" statistics: "<<statistics<<" ratio: "<<ratio_bpix<<std::endl;
//  
//   	delete gausfit_bpix;
//           delete gausfit_statcut_bpix;
//           delete test_roceff_bpix;
//           delete test_rocdist_bpix;
//           delete test2_roceff_bpix;
//           delete test2_rocdist_bpix;
//         }
//       }
//       delete roctest_l1_eff;
//       delete roctest_l1_dist;
//       delete roctest_l2_eff;
//       delete roctest_l2_dist;
//       delete roctest_l3_eff;
//       delete roctest_l3_dist;
//     }
  
    TFile *froceff= new TFile(rocselstudyout,"recreate");
    downstop_bpix->Write();
    upstop_bpix->Write();
    eff_dist_error_write(run2_ordered_bpix_eff,run2_ordered_bpix_dist);
    mergedruns_bpix->Write();
    statistics_bpix->Write();
    sigma_fit_bpix->Write();
    mean_fit_bpix->Write();
    mean_rocstatcut_bpix->Write();
    sigma_rocstatcut_bpix->Write();
    froceff->Close();
  }

  // Create 2D (Roceff_dist, nrun) from 3D Rocmap
//   TH2D *dist_roceff = new TH2D("dist_roceff","dist_roceff",999,0.001,1.0,1000,0.5,1000.5);
//   TH2D *dist_roceff_bpix = new TH2D("dist_roceff_bpix","dist_roceff_bpix",999,0.001,1.0,1000,0.5,1000.5);
//   TH2D *dist_roceff_fpix = new TH2D("dist_roceff_fpix","dist_roceff_fpix",999,0.001,1.0,1000,0.5,1000.5);
//   TH2D *dist_roceff_fpix_p1 = new TH2D("dist_roceff_fpix_p1","dist_roceff_fpix_p1",999,0.001,1.0,1000,0.5,1000.5);
//   TH2D *dist_roceff_fpix_p2 = new TH2D("dist_roceff_fpix_p2","dist_roceff_fpix_p2",999,0.001,1.0,1000,0.5,1000.5);

//   TH2D *dist_rocdist = new TH2D("dist_rocdist","dist_rocdist",300,0.0,15000.0,1000,0.5,1000.5);
//   TH2D *dist_rocdist_bpix = new TH2D("dist_rocdist_bpix","dist_rocdist_bpix",300,0.0,15000.0,1000,0.5,1000.5);
//   TH2D *dist_rocdist_fpix = new TH2D("dist_rocdist_fpix","dist_rocdist_fpix",70,0.0,3500.0,1000,0.5,1000.5);
//   TH2D *dist_rocdist_fpix_p1 = new TH2D("dist_rocdist_fpix_p1","dist_rocdist_fpix_p1",70,0.0,3500.0,1000,0.5,1000.5);
//   TH2D *dist_rocdist_fpix_p2 = new TH2D("dist_rocdist_fpix_p2","dist_rocdist_fpix_p2",70,0.0,3500.0,1000,0.5,1000.5);

//   for (Int_t k=1;k<=1000;k++) {
//     for (Int_t i=1;i<=72;i++) {
//       for (Int_t j=1;j<=42;j++) {
// 	if (rocsel2_l1_dist->GetBinContent(i,j,k)!=0) {
// 	  dist_roceff->Fill(rocsel2_l1_eff->GetBinContent(i,j,k),k);
// 	  dist_roceff_bpix->Fill(rocsel2_l1_eff->GetBinContent(i,j,k),k);
// 	  dist_rocdist->Fill(rocsel2_l1_dist->GetBinContent(i,j,k),k);
// 	  dist_rocdist_bpix->Fill(rocsel2_l1_dist->GetBinContent(i,j,k),k);
// 	}
//       }
//       for (Int_t j=1;j<=66;j++) {
// 	if (rocsel2_l2_dist->GetBinContent(i,j,k)!=0) {
// 	  dist_roceff->Fill(rocsel2_l2_eff->GetBinContent(i,j,k),k);
// 	  dist_roceff_bpix->Fill(rocsel2_l2_eff->GetBinContent(i,j,k),k);
// 	  dist_rocdist->Fill(rocsel2_l2_dist->GetBinContent(i,j,k),k);
// 	  dist_rocdist_bpix->Fill(rocsel2_l2_dist->GetBinContent(i,j,k),k);
// 	}
//       }
//       for (Int_t j=1;j<=90;j++) {
// 	if (rocsel2_l3_dist->GetBinContent(i,j,k)!=0) {
// 	  dist_roceff->Fill(rocsel2_l3_eff->GetBinContent(i,j,k),k);
// 	  dist_roceff_bpix->Fill(rocsel2_l3_eff->GetBinContent(i,j,k),k);
// 	  dist_rocdist->Fill(rocsel2_l3_dist->GetBinContent(i,j,k),k);
// 	  dist_rocdist_bpix->Fill(rocsel2_l3_dist->GetBinContent(i,j,k),k);
// 	}
//       }
//     }
//     for (Int_t i=0;i<9;i++) {
//       for (Int_t l=1;l<=6;l++) {
// 	for (Int_t j=1;j<=60;j++) {
// 	  if (rocsel2_fpixI_dist->GetBinContent(i,j,k)!=0) {
// 	    dist_roceff->Fill(rocsel2_fpixI_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	    dist_rocdist->Fill(rocsel2_fpixI_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	    dist_roceff_fpix->Fill(rocsel2_fpixI_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	    dist_rocdist_fpix->Fill(rocsel2_fpixI_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	  }
// 	  if (rocsel2_fpixO_dist->GetBinContent(i,j,k)!=0) {
// 	    dist_roceff->Fill(rocsel2_fpixO_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	    dist_rocdist->Fill(rocsel2_fpixO_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	    dist_roceff_fpix->Fill(rocsel2_fpixO_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	    dist_rocdist_fpix->Fill(rocsel2_fpixO_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	  }
// 	  // panel 2
// 	  if (i==0||i==2||i==6||i==9) {
// 	    if (rocsel2_fpixI_dist->GetBinContent(i,j,k)!=0) {
// 	      dist_roceff_fpix_p2->Fill(rocsel2_fpixI_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	      dist_rocdist_fpix_p2->Fill(rocsel2_fpixI_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	    }
// 	    if (rocsel2_fpixO_dist->GetBinContent(i,j,k)!=0) {
// 	      dist_roceff_fpix_p2->Fill(rocsel2_fpixO_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	      dist_rocdist_fpix_p2->Fill(rocsel2_fpixO_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	    }
// 	  } else {
// 	    if (rocsel2_fpixI_dist->GetBinContent(i,j,k)!=0) {
// 	      dist_roceff_fpix_p1->Fill(rocsel2_fpixI_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	      dist_rocdist_fpix_p1->Fill(rocsel2_fpixI_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	    }
// 	    if (rocsel2_fpixO_dist->GetBinContent(i,j,k)!=0) {
// 	      dist_roceff_fpix_p1->Fill(rocsel2_fpixO_eff->GetBinContent(i*6+l,j*2-1,k),k);
// 	      dist_rocdist_fpix_p1->Fill(rocsel2_fpixO_dist->GetBinContent(i*6+l,j*2-1,k),k);
// 	    }
// 	  }
// 	} 
//       }
//     }
//   }


  // Merge 3D rocmap into a 2D with enough data to test fit (nrun --> 1)
//   TH2D *roctest_l1_eff = new TH2D("roctest_l1_eff","roctest_l1_eff",72,-4.5,4.5,42,-10.5,10.5);
//   TH2D *roctest_l1_dist = new TH2D("roctest_l1_dist","roctest_l1_dist",72,-4.5,4.5,42,-10.5,10.5);
//   TH2D *roctest_l2_eff = new TH2D("roctest_l2_eff","roctest_l2_eff",72,-4.5,4.5,66,-16.5,16.5);
//   TH2D *roctest_l2_dist = new TH2D("roctest_l2_dist","roctest_l2_dist",72,-4.5,4.5,66,-16.5,16.5);
//   TH2D *roctest_l3_eff = new TH2D("roctest_l3_eff","roctest_l3_eff",72,-4.5,4.5,90,-22.5,22.5);
//   TH2D *roctest_l3_dist = new TH2D("roctest_l3_dist","roctest_l3_dist",72,-4.5,4.5,90,-22.5,22.5);
//   TH2D *roctest_fpixI_eff = new TH2D("roctest_fpixI_eff", "roctest_fpixI_eff", 54,-4.5,4.5,120,0.5,12.5);
//   TH2D *roctest_fpixI_dist = new TH2D("roctest_fpixI_dist", "roctest_fpixI_dist", 54,-4.5,4.5,120,0.5,12.5);
//   TH2D *roctest_fpixO_eff = new TH2D("roctest_fpixO_eff", "roctest_fpixO_eff", 54,-4.5,4.5,120,-12.5,-0.5);
//   TH2D *roctest_fpixO_dist = new TH2D("roctest_fpixO_dist", "roctest_fpixO_dist", 54,-4.5,4.5,120,-12.5,-0.5);

//   Int_t initial = 1;
//   Float_t minstat_bpix = 0;
//   Int_t enoughstat = 0;

//   for (Int_t k=1;k>=1&&enoughstat==0;k--) {
//     for (Int_t i=1;i<=72;i++) {
//       for (Int_t j=1;j<=42;j++) {
// 	roctest_l1_eff->SetBinContent(i,j,roctest_l1_eff->GetBinContent(i,j)+rocsel_l1_eff->GetBinContent(i,j,k));
// 	roctest_l1_dist->SetBinContent(i,j,roctest_l1_dist->GetBinContent(i,j)+rocsel_l1_dist->GetBinContent(i,j,k));
//       }
//       for (Int_t j=1;j<=66;j++) {
// 	roctest_l2_eff->SetBinContent(i,j,roctest_l2_eff->GetBinContent(i,j)+rocsel_l2_eff->GetBinContent(i,j,k));
// 	roctest_l2_dist->SetBinContent(i,j,roctest_l2_dist->GetBinContent(i,j)+rocsel_l2_dist->GetBinContent(i,j,k));
//       }
//       for (Int_t j=1;j<=90;j++) {
// 	roctest_l3_eff->SetBinContent(i,j,roctest_l3_eff->GetBinContent(i,j)+rocsel_l3_eff->GetBinContent(i,j,k));
// 	roctest_l3_dist->SetBinContent(i,j,roctest_l3_dist->GetBinContent(i,j)+rocsel_l3_dist->GetBinContent(i,j,k));
//       }
//     }
//     for (Int_t i=1;i<=54;i++) {
//       for (Int_t j=1;j<=120;j++) {
// 	roctest_fpixI_eff->SetBinContent(i,j,roctest_fpixI_eff->GetBinContent(i,j)+rocsel_fpixI_eff->GetBinContent(i,j,k));
// 	roctest_fpixI_dist->SetBinContent(i,j,roctest_fpixI_dist->GetBinContent(i,j)+rocsel_fpixI_dist->GetBinContent(i,j,k));
// 	roctest_fpixO_eff->SetBinContent(i,j,roctest_fpixO_eff->GetBinContent(i,j)+rocsel_fpixO_eff->GetBinContent(i,j,k));
// 	roctest_fpixO_dist->SetBinContent(i,j,roctest_fpixO_dist->GetBinContent(i,j)+rocsel_fpixO_dist->GetBinContent(i,j,k));
//       }
//     }
//     roctest_l1_dist->Add(roctest_l1_eff);
//     roctest_l1_eff->Divide(roctest_l1_dist);
//     roctest_l2_dist->Add(roctest_l2_eff);
//     roctest_l2_eff->Divide(roctest_l2_dist);
//     roctest_l3_dist->Add(roctest_l3_eff);
//     roctest_l3_eff->Divide(roctest_l3_dist);
//     roctest_fpixI_dist->Add(roctest_fpixI_eff);
//     roctest_fpixI_eff->Divide(roctest_fpixI_dist);
//     roctest_fpixO_dist->Add(roctest_fpixO_eff);
//     roctest_fpixO_eff->Divide(roctest_fpixO_dist);

//     // Roceff Dist - testing fitting
//     TH1D *test_roceff = new TH1D("test_roceff","test_roceff",999,0.001,1.0);
//     TH1D *test_roceff_bpix = new TH1D("test_roceff_bpix","test_roceff_bpix",999,0.001,1.0);
//     TH1D *test_roceff_fpix = new TH1D("test_roceff_fpix","test_roceff_fpix",999,0.001,1.0);
//     TH1D *test_roceff_fpix_p1 = new TH1D("test_roceff_fpix_p1","test_roceff_fpix_p1",999,0.001,1.0);
//     TH1D *test_roceff_fpix_p2 = new TH1D("test_roceff_fpix_p2","test_roceff_fpix_p2",999,0.001,1.0);
//     TH1D *test_rocdist = new TH1D("test_rocdist","test_rocdist",300,0.0,15000.0);
//     TH1D *test_rocdist_bpix = new TH1D("test_rocdist_bpix","test_rocdist_bpix",300,0.0,15000.0);
//     TH1D *test_rocdist_fpix = new TH1D("test_rocdist_fpix","test_rocdist_fpix",70,0.0,3500.0);
//     TH1D *test_rocdist_fpix_p1 = new TH1D("test_rocdist_fpix_p1","test_rocdist_fpix_p1",70,0.0,3500.0);
//     TH1D *test_rocdist_fpix_p2 = new TH1D("test_rocdist_fpix_p2","test_rocdist_fpix_p2",70,0.0,3500.0);

//     Int_t maxroc = 0;
//     Int_t maxroc_bpix = 0;
//     Int_t maxroc_fpix = 0;
//     Int_t maxroc_fpix_p1 = 0;
//     Int_t maxroc_fpix_p2 = 0;

//     for (Int_t i=1;i<=72;i++) {
//       for (Int_t j=1;j<=42;j++) {
//         if (roctest_l1_dist->GetBinContent(i,j)!=0) {
//           test_roceff->Fill(roctest_l1_eff->GetBinContent(i,j));
//           test_roceff_bpix->Fill(roctest_l1_eff->GetBinContent(i,j));
//           test_rocdist->Fill(roctest_l1_dist->GetBinContent(i,j));
//           test_rocdist_bpix->Fill(roctest_l1_dist->GetBinContent(i,j));
// 	  maxroc_bpix++;
// 	  maxroc++;
//         }
//       }
//       for (Int_t j=1;j<=66;j++) {
//         if (roctest_l2_dist->GetBinContent(i,j)!=0) {
//           test_roceff->Fill(roctest_l2_eff->GetBinContent(i,j));
//           test_roceff_bpix->Fill(roctest_l2_eff->GetBinContent(i,j));
//           test_rocdist->Fill(roctest_l2_dist->GetBinContent(i,j));
//           test_rocdist_bpix->Fill(roctest_l2_dist->GetBinContent(i,j));
// 	  maxroc_bpix++;
// 	  maxroc++;
//         }
//       }
//       for (Int_t j=1;j<=90;j++) {
//         if (roctest_l3_dist->GetBinContent(i,j)!=0) {
//           test_roceff->Fill(roctest_l3_eff->GetBinContent(i,j));
//           test_roceff_bpix->Fill(roctest_l3_eff->GetBinContent(i,j));
//           test_rocdist->Fill(roctest_l3_dist->GetBinContent(i,j));
//           test_rocdist_bpix->Fill(roctest_l3_dist->GetBinContent(i,j));
// 	  maxroc_bpix++;
// 	  maxroc++;
//         }
//       }
//     }
//     for (Int_t i=0;i<9;i++) {
//       for (Int_t l=1;l<=6;l++) {
//         for (Int_t j=1;j<=60;j++) {
//           // panel 2
//           if (i==0||i==2||i==6||i==9) {
//             if (roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1)!=0) {
// 	      test_roceff->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
//               test_roceff_fpix_p2->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
//               test_rocdist_fpix_p2->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      maxroc++;
// 	      maxroc_fpix++;
// 	      maxroc_fpix_p2++;
//             }
//             if (roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1)!=0) {
// 	      test_roceff->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
//               test_roceff_fpix_p2->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
//               test_rocdist_fpix_p2->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      maxroc++;
// 	      maxroc_fpix++;
// 	      maxroc_fpix_p2++;
//             }
//           } else {
//             if (roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1)!=0) {
// 	      test_roceff->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix_p1->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix_p1->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      maxroc++;
// 	      maxroc_fpix++;
// 	      maxroc_fpix_p1++;
//             }
//             if (roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1)!=0) {
// 	      test_roceff->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
//               test_roceff_fpix_p1->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
//               test_rocdist_fpix_p1->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      maxroc++;
// 	      maxroc_fpix++;
// 	      maxroc_fpix_p1++;
//             }
//           }
//         } 
//       }
//     }


//     new TF1("gausfit_bpix","gaus",0.9,1.0);
//     test_roceff_bpix->Fit("gausfit_bpix","RMWQ");
//     Float_t mean_bpix = test_roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(1);
//     Float_t sigma_bpix = test_roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(2);
//     if (initial==1) {
//       minstat_bpix = (1 - mean_bpix) * mean_bpix / (sigma_bpix * sigma_bpix);
//       initial = 0;
//     }

//     TH1D *test2_roceff = new TH1D("test2_roceff","test2_roceff",999,0.001,1.0);
//     TH1D *test2_roceff_bpix = new TH1D("test2_roceff_bpix","test2_roceff_bpix",999,0.001,1.0);
//     TH1D *test2_roceff_fpix = new TH1D("test2_roceff_fpix","test2_roceff_fpix",999,0.001,1.0);
//     TH1D *test2_roceff_fpix_p1 = new TH1D("test2_roceff_fpix_p1","test2_roceff_fpix_p1",999,0.001,1.0);
//     TH1D *test2_roceff_fpix_p2 = new TH1D("test2_roceff_fpix_p2","test2_roceff_fpix_p2",999,0.001,1.0);
//     TH1D *test2_rocdist = new TH1D("test2_rocdist","test2_rocdist",300,0.0,15000.0);
//     TH1D *test2_rocdist_bpix = new TH1D("test2_rocdist_bpix","test2_rocdist_bpix",300,0.0,15000.0);
//     TH1D *test2_rocdist_fpix = new TH1D("test2_rocdist_fpix","test2_rocdist_fpix",70,0.0,3500.0);
//     TH1D *test2_rocdist_fpix_p1 = new TH1D("test2_rocdist_fpix_p1","test2_rocdist_fpix_p1",70,0.0,3500.0);
//     TH1D *test2_rocdist_fpix_p2 = new TH1D("test2_rocdist_fpix_p2","test2_rocdist_fpix_p2",70,0.0,3500.0);

//     Int_t statroc = 0;
//     Int_t statroc_bpix = 0;
//     Int_t statroc_fpix = 0;
//     Int_t statroc_fpix_p1 = 0;
//     Int_t statroc_fpix_p2 = 0;

//     for (Int_t i=1;i<=72;i++) {
//       for (Int_t j=1;j<=42;j++) {
//         if (roctest_l1_dist->GetBinContent(i,j)>minstat_bpix) {
//           test_roceff->Fill(roctest_l1_eff->GetBinContent(i,j));
//           test_roceff_bpix->Fill(roctest_l1_eff->GetBinContent(i,j));
//           test_rocdist->Fill(roctest_l1_dist->GetBinContent(i,j));
//           test_rocdist_bpix->Fill(roctest_l1_dist->GetBinContent(i,j));
// 	  statroc_bpix++;
// 	  statroc++;
//         }
//       }
//       for (Int_t j=1;j<=66;j++) {
//         if (roctest_l2_dist->GetBinContent(i,j)>minstat_bpix) {
//           test_roceff->Fill(roctest_l2_eff->GetBinContent(i,j));
//           test_roceff_bpix->Fill(roctest_l2_eff->GetBinContent(i,j));
//           test_rocdist->Fill(roctest_l2_dist->GetBinContent(i,j));
//           test_rocdist_bpix->Fill(roctest_l2_dist->GetBinContent(i,j));
// 	  statroc_bpix++;
// 	  statroc++;
//         }
//       }
//       for (Int_t j=1;j<=90;j++) {
//         if (roctest_l3_dist->GetBinContent(i,j)>minstat_bpix) {
//           test_roceff->Fill(roctest_l3_eff->GetBinContent(i,j));
//           test_roceff_bpix->Fill(roctest_l3_eff->GetBinContent(i,j));
//           test_rocdist->Fill(roctest_l3_dist->GetBinContent(i,j));
//           test_rocdist_bpix->Fill(roctest_l3_dist->GetBinContent(i,j));
// 	  statroc_bpix++;
// 	  statroc++;
//         }
//       }
//     }
//     for (Int_t i=0;i<9;i++) {
//       for (Int_t l=1;l<=6;l++) {
//         for (Int_t j=1;j<=60;j++) {
//           // panel 2
//           if (i==0||i==2||i==6||i==9) {
//             if (roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1)>minstat_bpix) {
// 	      test_roceff->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
//               test_roceff_fpix_p2->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
//               test_rocdist_fpix_p2->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      statroc++;
// 	      statroc_fpix++;
// 	      statroc_fpix_p2++;
//             }
//             if (roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1)>minstat_bpix) {
// 	      test_roceff->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
//               test_roceff_fpix_p2->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
//               test_rocdist_fpix_p2->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      statroc++;
// 	      statroc_fpix++;
// 	      statroc_fpix_p2++;
//             }
//           } else {
//             if (roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1)>minstat_bpix) {
// 	      test_roceff->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix_p1->Fill(roctest_fpixI_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix_p1->Fill(roctest_fpixI_dist->GetBinContent(i*6+l,j*2-1));
// 	      statroc++;
// 	      statroc_fpix++;
// 	      statroc_fpix_p1++;
//             }
//             if (roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1)>minstat_bpix) {
// 	      test_roceff->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      test_roceff_fpix->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
// 	      test_rocdist_fpix->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
//               test_roceff_fpix_p1->Fill(roctest_fpixO_eff->GetBinContent(i*6+l,j*2-1));
//               test_rocdist_fpix_p1->Fill(roctest_fpixO_dist->GetBinContent(i*6+l,j*2-1));
// 	      statroc++;
// 	      statroc_fpix++;
// 	      statroc_fpix_p1++;
//             }
//           }
//         } 
//       }
//     }

//     new TF1("gausfit_statcut_bpix","gaus",0.9,1.0);
//     test2_roceff_bpix->Fit("gausfit_statcut_bpix","RMWQ");
//     Float_t mean_statcut_bpix = test2_roceff_bpix->GetFunction("gausfit_statcut_bpix")->GetParameter(1);
//     Float_t sigma_statcut_bpix = test2_roceff_bpix->GetFunction("gausfit_statcut_bpix")->GetParameter(2);
//     minstat_bpix = (1 - mean_statcut_bpix) * mean_statcut_bpix / (sigma_statcut_bpix * sigma_statcut_bpix);

//     if (statroc_bpix/maxroc_bpix>0.9)
//       enoughstat = 1;

//   }


  if (effstudy==1) {
    
    // Load Histo
    TFile fi("oldeff_completerun_v1815.root");
    full2010runs_roc_l1_eff = (TH2D*)fi.Get("roc_l1_eff");
    full2010runs_roc_l2_eff = (TH2D*)fi.Get("roc_l2_eff");
    full2010runs_roc_l3_eff = (TH2D*)fi.Get("roc_l3_eff");
    full2010runs_roc_fpixI_eff = (TH2D*)fi.Get("roc_fpixI_eff");
    full2010runs_roc_fpixO_eff = (TH2D*)fi.Get("roc_fpixO_eff");
    full2010runs_roc_l1_dist = (TH2D*)fi.Get("roc_l1_dist");
    full2010runs_roc_l2_dist = (TH2D*)fi.Get("roc_l2_dist");
    full2010runs_roc_l3_dist = (TH2D*)fi.Get("roc_l3_dist");
    full2010runs_roc_fpixI_dist = (TH2D*)fi.Get("roc_fpixI_dist");
    full2010runs_roc_fpixO_dist = (TH2D*)fi.Get("roc_fpixO_dist");
    full2010runs_roc_l1_eff->SetDirectory(NULL);
    full2010runs_roc_l2_eff->SetDirectory(NULL);
    full2010runs_roc_l3_eff->SetDirectory(NULL);
    full2010runs_roc_fpixI_eff->SetDirectory(NULL);
    full2010runs_roc_fpixO_eff->SetDirectory(NULL);
    full2010runs_roc_l1_dist->SetDirectory(NULL);
    full2010runs_roc_l2_dist->SetDirectory(NULL);
    full2010runs_roc_l3_dist->SetDirectory(NULL);
    full2010runs_roc_fpixI_dist->SetDirectory(NULL);
    full2010runs_roc_fpixO_dist->SetDirectory(NULL);
    fi.Close();
    
    // Create Histograms
//     coarsedelay_eff = new TH1D("coarsedelay_eff", "coarsedelay_eff",51,-25.5,25.5);
//     coarsedelay_dist = new TH1D("coarsedelay_dist", "coarsedelay_dist",51,-25.5,25.5);
//     coarsedelay_bpix_eff = new TH1D("coarsedelay_bpix_eff", "coarsedelay_bpix_eff",51,-25.5,25.5);
//     coarsedelay_bpix_dist = new TH1D("coarsedelay_bpix_dist", "coarsedelay_bpix_dist",51,-25.5,25.5);
//     coarsedelay_fpix_eff = new TH1D("coarsedelay_fpix_eff", "coarsedelay_fpix_eff",51,-25.5,25.5);
//     coarsedelay_fpix_dist = new TH1D("coarsedelay_fpix_dist", "coarsedelay_fpix_dist",51,-25.5,25.5);
//     coarsedelay_l1_eff = new TH1D("coarsedelay_l1_eff", "coarsedelay_l1_eff",51,-25.5,25.5);
//     coarsedelay_l1_dist = new TH1D("coarsedelay_l1_dist", "coarsedelay_l1_dist",51,-25.5,25.5);
//     coarsedelay_l2_eff = new TH1D("coarsedelay_l2_eff", "coarsedelay_l2_eff",51,-25.5,25.5);
//     coarsedelay_l2_dist = new TH1D("coarsedelay_l2_dist", "coarsedelay_l2_dist",51,-25.5,25.5);
//     coarsedelay_l3_eff = new TH1D("coarsedelay_l3_eff", "coarsedelay_l3_eff",51,-25.5,25.5);
//     coarsedelay_l3_dist = new TH1D("coarsedelay_l3_dist", "coarsedelay_l3_dist",51,-25.5,25.5);
//     coarsedelay_dm2_eff = new TH1D("coarsedelay_dm2_eff", "coarsedelay_dm2_eff",51,-25.5,25.5);
//     coarsedelay_dm2_dist = new TH1D("coarsedelay_dm2_dist", "coarsedelay_dm2_dist",51,-25.5,25.5);
//     coarsedelay_dm1_eff = new TH1D("coarsedelay_dm1_eff", "coarsedelay_dm1_eff",51,-25.5,25.5);
//     coarsedelay_dm1_dist = new TH1D("coarsedelay_dm1_dist", "coarsedelay_dm1_dist",51,-25.5,25.5);
//     coarsedelay_dp1_eff = new TH1D("coarsedelay_dp1_eff", "coarsedelay_dp1_eff",51,-25.5,25.5);
//     coarsedelay_dp1_dist = new TH1D("coarsedelay_dp1_dist", "coarsedelay_dp1_dist",51,-25.5,25.5);
//     coarsedelay_dp2_eff = new TH1D("coarsedelay_dp2_eff", "coarsedelay_dp2_eff",51,-25.5,25.5);
//     coarsedelay_dp2_dist = new TH1D("coarsedelay_dp2_dist", "coarsedelay_dp2_dist",51,-25.5,25.5);

    // Detector postfixes
    std::vector<std::string> postfix_det;
    postfix_det.push_back("");
    postfix_det.push_back("_bpix");
    postfix_det.push_back("_l1");
    postfix_det.push_back("_l2");
    postfix_det.push_back("_l3");
    postfix_det.push_back("_fpix");
    postfix_det.push_back("_dm2");
    postfix_det.push_back("_dm1");
    postfix_det.push_back("_dp1");
    postfix_det.push_back("_dp2");
    // Efficiency postfixes
    std::vector<std::string> postfix_eff;
    postfix_eff.push_back("_eff");
    postfix_eff.push_back("_dist");

    declare_TH1D_2postfix(coarsedelay, "coarsedelay", postfix_det, postfix_eff, 51, -25.5, 25.5);

    coarsedelay_avgclusize = new TH1D("coarsedelay_avgclusize", "coarsedelay_avgclusize",51,-25.5,25.5);
    coarsedelay_nclust = new TH1D("coarsedelay_nclust", "coarsedelay_nclust",51,-25.5,25.5);
    coarsedelay_bpix_avgclusize = new TH1D("coarsedelay_bpix_avgclusize", "coarsedelay_bpix_avgclusize",51,-25.5,25.5);
    coarsedelay_bpix_nclust = new TH1D("coarsedelay_bpix_nclust", "coarsedelay_bpix_nclust",51,-25.5,25.5);
    coarsedelay_fpix_avgclusize = new TH1D("coarsedelay_fpix_avgclusize", "coarsedelay_fpix_avgclusize",51,-25.5,25.5);
    coarsedelay_fpix_nclust = new TH1D("coarsedelay_fpix_nclust", "coarsedelay_fpix_nclust",51,-25.5,25.5);
    coarsedelay_l1_avgclusize = new TH1D("coarsedelay_l1_avgclusize", "coarsedelay_l1_avgclusize",51,-25.5,25.5);
    coarsedelay_l1_nclust = new TH1D("coarsedelay_l1_nclust", "coarsedelay_l1_nclust",51,-25.5,25.5);
    coarsedelay_l2_avgclusize = new TH1D("coarsedelay_l2_avgclusize", "coarsedelay_l2_avgclusize",51,-25.5,25.5);
    coarsedelay_l2_nclust = new TH1D("coarsedelay_l2_nclust", "coarsedelay_l2_nclust",51,-25.5,25.5);
    coarsedelay_l3_avgclusize = new TH1D("coarsedelay_l3_avgclusize", "coarsedelay_l3_avgclusize",51,-25.5,25.5);
    coarsedelay_l3_nclust = new TH1D("coarsedelay_l3_nclust", "coarsedelay_l3_nclust",51,-25.5,25.5);
    coarsedelay_dm2_avgclusize = new TH1D("coarsedelay_dm2_avgclusize", "coarsedelay_dm2_avgclusize",51,-25.5,25.5);
    coarsedelay_dm2_nclust = new TH1D("coarsedelay_dm2_nclust", "coarsedelay_dm2_nclust",51,-25.5,25.5);
    coarsedelay_dm1_avgclusize = new TH1D("coarsedelay_dm1_avgclusize", "coarsedelay_dm1_avgclusize",51,-25.5,25.5);
    coarsedelay_dm1_nclust = new TH1D("coarsedelay_dm1_nclust", "coarsedelay_dm1_nclust",51,-25.5,25.5);
    coarsedelay_dp1_avgclusize = new TH1D("coarsedelay_dp1_avgclusize", "coarsedelay_dp1_avgclusize",51,-25.5,25.5);
    coarsedelay_dp1_nclust = new TH1D("coarsedelay_dp1_nclust", "coarsedelay_dp1_nclust",51,-25.5,25.5);
    coarsedelay_dp2_avgclusize = new TH1D("coarsedelay_dp2_avgclusize", "coarsedelay_dp2_avgclusize",51,-25.5,25.5);
    coarsedelay_dp2_nclust = new TH1D("coarsedelay_dp2_nclust", "coarsedelay_dp2_nclust",51,-25.5,25.5);

    coarsedelay_ontrack_avgclusize = new TH1D("coarsedelay_ontrack_avgclusize", "coarsedelay_ontrack_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_nclust = new TH1D("coarsedelay_ontrack_nclust", "coarsedelay_ontrack_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_bpix_avgclusize = new TH1D("coarsedelay_ontrack_bpix_avgclusize", "coarsedelay_ontrack_bpix_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_bpix_nclust = new TH1D("coarsedelay_ontrack_bpix_nclust", "coarsedelay_ontrack_bpix_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_fpix_avgclusize = new TH1D("coarsedelay_ontrack_fpix_avgclusize", "coarsedelay_ontrack_fpix_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_fpix_nclust = new TH1D("coarsedelay_ontrack_fpix_nclust", "coarsedelay_ontrack_fpix_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_l1_avgclusize = new TH1D("coarsedelay_ontrack_l1_avgclusize", "coarsedelay_ontrack_l1_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_l1_nclust = new TH1D("coarsedelay_ontrack_l1_nclust", "coarsedelay_ontrack_l1_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_l2_avgclusize = new TH1D("coarsedelay_ontrack_l2_avgclusize", "coarsedelay_ontrack_l2_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_l2_nclust = new TH1D("coarsedelay_ontrack_l2_nclust", "coarsedelay_ontrack_l2_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_l3_avgclusize = new TH1D("coarsedelay_ontrack_l3_avgclusize", "coarsedelay_ontrack_l3_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_l3_nclust = new TH1D("coarsedelay_ontrack_l3_nclust", "coarsedelay_ontrack_l3_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_dm2_avgclusize = new TH1D("coarsedelay_ontrack_dm2_avgclusize", "coarsedelay_ontrack_dm2_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_dm2_nclust = new TH1D("coarsedelay_ontrack_dm2_nclust", "coarsedelay_ontrack_dm2_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_dm1_avgclusize = new TH1D("coarsedelay_ontrack_dm1_avgclusize", "coarsedelay_ontrack_dm1_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_dm1_nclust = new TH1D("coarsedelay_ontrack_dm1_nclust", "coarsedelay_ontrack_dm1_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_dp1_avgclusize = new TH1D("coarsedelay_ontrack_dp1_avgclusize", "coarsedelay_ontrack_dp1_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_dp1_nclust = new TH1D("coarsedelay_ontrack_dp1_nclust", "coarsedelay_ontrack_dp1_nclust",51,-25.5,25.5);
    coarsedelay_ontrack_dp2_avgclusize = new TH1D("coarsedelay_ontrack_dp2_avgclusize", "coarsedelay_ontrack_dp2_avgclusize",51,-25.5,25.5);
    coarsedelay_ontrack_dp2_nclust = new TH1D("coarsedelay_ontrack_dp2_nclust", "coarsedelay_ontrack_dp2_nclust",51,-25.5,25.5);

    coarsedelay_ls = new TH1D("coarsedelay_ls", "coarsedelay_ls",51,-25.5,25.5);

    finedelay_eff = new TH1D("finedelay_eff", "finedelay_eff",5,11.5,16.5);
    finedelay_dist = new TH1D("finedelay_dist", "finedelay_dist",5,11.5,16.5);
    finedelay_bpix_eff = new TH1D("finedelay_bpix_eff", "finedelay_bpix_eff",5,11.5,16.5);
    finedelay_bpix_dist = new TH1D("finedelay_bpix_dist", "finedelay_bpix_dist",5,11.5,16.5);
    finedelay_fpix_eff = new TH1D("finedelay_fpix_eff", "finedelay_fpix_eff",5,11.5,16.5);
    finedelay_fpix_dist = new TH1D("finedelay_fpix_dist", "finedelay_fpix_dist",5,11.5,16.5);
    finedelay_l1_eff = new TH1D("finedelay_l1_eff", "finedelay_l1_eff",5,11.5,16.5);
    finedelay_l1_dist = new TH1D("finedelay_l1_dist", "finedelay_l1_dist",5,11.5,16.5);
    finedelay_l2_eff = new TH1D("finedelay_l2_eff", "finedelay_l2_eff",5,11.5,16.5);
    finedelay_l2_dist = new TH1D("finedelay_l2_dist", "finedelay_l2_dist",5,11.5,16.5);
    finedelay_l3_eff = new TH1D("finedelay_l3_eff", "finedelay_l3_eff",5,11.5,16.5);
    finedelay_l3_dist = new TH1D("finedelay_l3_dist", "finedelay_l3_dist",5,11.5,16.5);
    finedelay_dm2_eff = new TH1D("finedelay_dm2_eff", "finedelay_dm2_eff",5,11.5,16.5);
    finedelay_dm2_dist = new TH1D("finedelay_dm2_dist", "finedelay_dm2_dist",5,11.5,16.5);
    finedelay_dm1_eff = new TH1D("finedelay_dm1_eff", "finedelay_dm1_eff",5,11.5,16.5);
    finedelay_dm1_dist = new TH1D("finedelay_dm1_dist", "finedelay_dm1_dist",5,11.5,16.5);
    finedelay_dp1_eff = new TH1D("finedelay_dp1_eff", "finedelay_dp1_eff",5,11.5,16.5);
    finedelay_dp1_dist = new TH1D("finedelay_dp1_dist", "finedelay_dp1_dist",5,11.5,16.5);
    finedelay_dp2_eff = new TH1D("finedelay_dp2_eff", "finedelay_dp2_eff",5,11.5,16.5);
    finedelay_dp2_dist = new TH1D("finedelay_dp2_dist", "finedelay_dp2_dist",5,11.5,16.5);

    avghitperls_dist = new TH1D("avghitperls_dist", "avghitperls_dist",51,-25.5,25.5);
    avghitperls_bpix_dist = new TH1D("avghitperls_bpix_dist", "avghitperls_bpix_dist",51,-25.5,25.5);
    avghitperls_fpix_dist = new TH1D("avghitperls_fpix_dist", "avghitperls_fpix_dist",51,-25.5,25.5);
    avghitperls_l1_dist = new TH1D("avghitperls_l1_dist", "avghitperls_l1_dist",51,-25.5,25.5);
    avghitperls_l2_dist = new TH1D("avghitperls_l2_dist", "avghitperls_l2_dist",51,-25.5,25.5);
    avghitperls_l3_dist = new TH1D("avghitperls_l3_dist", "avghitperls_l3_dist",51,-25.5,25.5);
    avghitperls_dm2_dist = new TH1D("avghitperls_dm2_dist", "avghitperls_dm2_dist",51,-25.5,25.5);
    avghitperls_dm1_dist = new TH1D("avghitperls_dm1_dist", "avghitperls_dm1_dist",51,-25.5,25.5);
    avghitperls_dp1_dist = new TH1D("avghitperls_dp1_dist", "avghitperls_dp1_dist",51,-25.5,25.5);
    avghitperls_dp2_dist = new TH1D("avghitperls_dp2_dist", "avghitperls_dp2_dist",51,-25.5,25.5);

    wbc_eff = new TH1D("wbc_eff", "wbc_eff",4,153.5,157.5);
    wbc_dist = new TH1D("wbc_dist", "wbc_dist",4,153.5,157.5);
    wbc_bpix_eff = new TH1D("wbc_bpix_eff", "wbc_bpix_eff",4,153.5,157.5);
    wbc_bpix_dist = new TH1D("wbc_bpix_dist", "wbc_bpix_dist",4,153.5,157.5);
    wbc_fpix_eff = new TH1D("wbc_fpix_eff", "wbc_fpix_eff",4,153.5,157.5);
    wbc_fpix_dist = new TH1D("wbc_fpix_dist", "wbc_fpix_dist",4,153.5,157.5);
    wbc_l1_eff = new TH1D("wbc_l1_eff", "wbc_l1_eff",4,153.5,157.5);
    wbc_l1_dist = new TH1D("wbc_l1_dist", "wbc_l1_dist",4,153.5,157.5);
    wbc_l2_eff = new TH1D("wbc_l2_eff", "wbc_l2_eff",4,153.5,157.5);
    wbc_l2_dist = new TH1D("wbc_l2_dist", "wbc_l2_dist",4,153.5,157.5);
    wbc_l3_eff = new TH1D("wbc_l3_eff", "wbc_l3_eff",4,153.5,157.5);
    wbc_l3_dist = new TH1D("wbc_l3_dist", "wbc_l3_dist",4,153.5,157.5);
    wbc_dm2_eff = new TH1D("wbc_dm2_eff", "wbc_dm2_eff",4,153.5,157.5);
    wbc_dm2_dist = new TH1D("wbc_dm2_dist", "wbc_dm2_dist",4,153.5,157.5);
    wbc_dm1_eff = new TH1D("wbc_dm1_eff", "wbc_dm1_eff",4,153.5,157.5);
    wbc_dm1_dist = new TH1D("wbc_dm1_dist", "wbc_dm1_dist",4,153.5,157.5);
    wbc_dp1_eff = new TH1D("wbc_dp1_eff", "wbc_dp1_eff",4,153.5,157.5);
    wbc_dp1_dist = new TH1D("wbc_dp1_dist", "wbc_dp1_dist",4,153.5,157.5);
    wbc_dp2_eff = new TH1D("wbc_dp2_eff", "wbc_dp2_eff",4,153.5,157.5);
    wbc_dp2_dist = new TH1D("wbc_dp2_dist", "wbc_dp2_dist",4,153.5,157.5);

    voltage_bpix_eff = new TH1D("voltage_bpix_eff", "voltage_bpix_eff", 13,25.0,155.0);
    voltage_bpix_dist = new TH1D("voltage_bpix_dist", "voltage_bpix_dist", 13,25.0,155.0);
    voltage_l1_eff = new TH1D("voltage_l1_eff", "voltage_l1_eff", 13,25.0,155.0);
    voltage_l1_dist = new TH1D("voltage_l1_dist", "voltage_l1_dist", 13,25.0,155.0);
    voltage_l3_eff = new TH1D("voltage_l3_eff", "voltage_l3_eff", 13,25.0,155.0);
    voltage_l3_dist = new TH1D("voltage_l3_dist", "voltage_l3_dist", 13,25.0,155.0);
    voltage_fpix_eff = new TH1D("voltage_fpix_eff", "voltage_fpix_eff", 29,15.0,305.0);
    voltage_fpix_dist = new TH1D("voltage_fpix_dist", "voltage_fpix_dist", 29,15.0,305.0);

    hv_bpix_eff = new TH1D("hv_bpix_eff", "hv_bpix_eff", 13,25.0,155.0);
    hv_bpix_dist = new TH1D("hv_bpix_dist", "hv_bpix_dist", 13,25.0,155.0);
    hv_fpix_eff = new TH1D("hv_fpix_eff", "hv_fpix_eff", 29,15.0,305.0);
    hv_fpix_dist = new TH1D("hv_fpix_dist", "hv_fpix_dist", 29,15.0,305.0);


    // Efficiency plots
    bx_eff = new TH1D("bx_eff", "bx_eff",3600,0.0,3600);
    bx_dist = new TH1D("bx_dist", "bx_dist",3600,0.0,3600);
  
    npix_mod_eff = new TH1D("npix_mod_eff", "npix_mod_eff",200,0.0,200);
    npix_mod_dist = new TH1D("npix_mod_dist", "npix_mod_dist",200,0.0,200);
    npix_roc_eff = new TH1D("npix_roc_eff", "npix_roc_eff",200,0.0,200);
    npix_roc_dist = new TH1D("npix_roc_dist", "npix_roc_dist",200,0.0,200);
    npix_bpix_eff = new TH1D("npix_bpix_eff", "npix_bpix_eff",50,0.0,50000);
    npix_bpix_dist = new TH1D("npix_bpix_dist", "npix_bpix_dist",50,0.0,50000);
    npix_l1_eff = new TH1D("npix_l1_eff", "npix_l1_eff",100,0.0,50000);
    npix_l1_dist = new TH1D("npix_l1_dist", "npix_l1_dist",100,0.0,50000);
    npix_l3_eff = new TH1D("npix_l3_eff", "npix_l3_eff",100,0.0,50000);
    npix_l3_dist = new TH1D("npix_l3_dist", "npix_l3_dist",100,0.0,50000);
  
//     eff_eff = new TH1D("eff_eff", "eff_eff", 7,0.5,7.5);
//     eff_dist = new TH1D("eff_dist", "eff_dist", 7,0.5,7.5);

//     std::vector<std::string> eff_names; //postfix
//     eff_names.push_back("_eff");
//     eff_names.push_back("_dist");

//     for (size_t i=0; i<eff_names.size();i++) {
//       std::string name="eff";
//       name+=eff_names[i];
//       TH1D* h = new TH1D(name.c_str(), name.c_str(), 7, 0.5, 7.5);
//       eff.push_back(h);
//     }

    declare_TH1D_1postfix(eff, "eff", postfix_eff, 7, 0.5, 7.5);
  
    mod_l1_eff = new TH2D("mod_l1_eff", "mod_l1_eff", 9,-4.5,4.5,21,-10.5,10.5);
    mod_l1_dist = new TH2D("mod_l1_dist", "mod_l1_dist", 9,-4.5,4.5,21,-10.5,10.5);
    mod_l2_eff = new TH2D("mod_l2_eff", "mod_l2_eff", 9,-4.5,4.5,33,-16.5,16.5);
    mod_l2_dist = new TH2D("mod_l2_dist", "mod_l2_dist", 9,-4.5,4.5,33,-16.5,16.5);
    mod_l3_eff = new TH2D("mod_l3_eff", "mod_l3_eff", 9,-4.5,4.5,45,-22.5,22.5);
    mod_l3_dist = new TH2D("mod_l3_dist", "mod_l3_dist", 9,-4.5,4.5,45,-22.5,22.5);
    mod_fpixI_eff = new TH2D("mod_fpixI_eff", "mod_fpixI_eff", 9,-4.5,4.5,12,0.5,12.5);
    mod_fpixI_dist = new TH2D("mod_fpixI_dist", "mod_fpixI_dist", 9,-4.5,4.5,12,0.5,12.5);
    mod_fpixO_eff = new TH2D("mod_fpixO_eff", "mod_fpixO_eff", 9,-4.5,4.5,12,-12.5,-0.5);
    mod_fpixO_dist = new TH2D("mod_fpixO_dist", "mod_fpixO_dist", 9,-4.5,4.5,12,-12.5,-0.5);
  
    roc_l1_eff = new TH2D("roc_l1_eff","roc_l1_eff",72,-4.5,4.5,42,-10.5,10.5);
    roc_l1_dist = new TH2D("roc_l1_dist","roc_l1_dist",72,-4.5,4.5,42,-10.5,10.5);
    roc_l2_eff = new TH2D("roc_l2_eff","roc_l2_eff",72,-4.5,4.5,66,-16.5,16.5);
    roc_l2_dist = new TH2D("roc_l2_dist","roc_l2_dist",72,-4.5,4.5,66,-16.5,16.5);
    roc_l3_eff = new TH2D("roc_l3_eff","roc_l3_eff",72,-4.5,4.5,90,-22.5,22.5);
    roc_l3_dist = new TH2D("roc_l3_dist","roc_l3_dist",72,-4.5,4.5,90,-22.5,22.5);
    roc_fpixI_eff = new TH2D("roc_fpixI_eff", "roc_fpixI_eff", 54,-4.5,4.5,120,0.5,12.5);
    roc_fpixI_dist = new TH2D("roc_fpixI_dist", "roc_fpixI_dist", 54,-4.5,4.5,120,0.5,12.5);
    roc_fpixO_eff = new TH2D("roc_fpixO_eff", "roc_fpixO_eff", 54,-4.5,4.5,120,-12.5,-0.5);
    roc_fpixO_dist = new TH2D("roc_fpixO_dist", "roc_fpixO_dist", 54,-4.5,4.5,120,-12.5,-0.5);
  
    rocsel_l1_eff = new TH3D("rocsel_l1_eff","rocsel_l1_eff",72,-4.5,4.5,42,-10.5,10.5,1000,0.5,1000.5);
    rocsel_l1_dist = new TH3D("rocsel_l1_dist","rocsel_l1_dist",72,-4.5,4.5,42,-10.5,10.5,1000,0.5,1000.5);
    rocsel_l2_eff = new TH3D("rocsel_l2_eff","rocsel_l2_eff",72,-4.5,4.5,66,-16.5,16.5,1000,0.5,1000.5);
    rocsel_l2_dist = new TH3D("rocsel_l2_dist","rocsel_l2_dist",72,-4.5,4.5,66,-16.5,16.5,1000,0.5,1000.5);
    rocsel_l3_eff = new TH3D("rocsel_l3_eff","rocsel_l3_eff",72,-4.5,4.5,90,-22.5,22.5,1000,0.5,1000.5);
    rocsel_l3_dist = new TH3D("rocsel_l3_dist","rocsel_l3_dist",72,-4.5,4.5,90,-22.5,22.5,1000,0.5,1000.5);
    rocsel_fpixI_eff = new TH3D("rocsel_fpixI_eff", "rocsel_fpixI_eff", 54,-4.5,4.5,120,0.5,12.5,1000,0.5,1000.5);
    rocsel_fpixI_dist = new TH3D("rocsel_fpixI_dist", "rocsel_fpixI_dist", 54,-4.5,4.5,120,0.5,12.5,1000,0.5,1000.5);
    rocsel_fpixO_eff = new TH3D("rocsel_fpixO_eff", "rocsel_fpixO_eff", 54,-4.5,4.5,120,-12.5,-0.5,1000,0.5,1000.5);
    rocsel_fpixO_dist = new TH3D("rocsel_fpixO_dist", "rocsel_fpixO_dist", 54,-4.5,4.5,120,-12.5,-0.5,1000,0.5,1000.5);
    rocsel_runno = new TH1I("rocsel_runno","rocsel_runno",1000,0.5,1000.5);
  
    rocsel_bpix_stat = new TH1D("rocsel_bpix_stat","rocsel_bpix_stat",1000,0.5,1000.5);
    rocsel_fpix_p1_stat = new TH1D("rocsel_fpix_p1_stat","rocsel_fpix_p1_stat",1000,0.5,1000.5);
    rocsel_fpix_p2_stat = new TH1D("rocsel_fpix_p2_stat","rocsel_fpix_p2_stat",1000,0.5,1000.5);
  
    roc_l1_eff2 = new TH2D("roc_l1_eff2","roc_l1_eff2",72,-4.5,4.5,42,-10.5,10.5);
    roc_l1_dist2 = new TH2D("roc_l1_dist2","roc_l1_dist2",72,-4.5,4.5,42,-10.5,10.5);
    roc_l1_ratio = new TH2D("roc_l1_ratio","roc_l1_ratio",72,-4.5,4.5,42,-10.5,10.5);
  
    run_eff = new TH1D("run_eff", "run_eff", 15000,135000.5,150000.5);
    run_dist = new TH1D("run_dist", "run_dist", 15000,135000.5,150000.5);
    run_bpix_eff = new TH1D("run_bpix_eff", "run_bpix_eff", 15000,135000.5,150000.5);
    run_bpix_dist = new TH1D("run_bpix_dist", "run_bpix_dist", 15000,135000.5,150000.5);
    run_fpix_eff = new TH1D("run_fpix_eff", "run_fpix_eff", 15000,135000.5,150000.5);
    run_fpix_dist = new TH1D("run_fpix_dist", "run_fpix_dist", 15000,135000.5,150000.5);
    run_l1_eff = new TH1D("run_l1_eff", "run_l1_eff", 15000,135000.5,150000.5);
    run_l1_dist = new TH1D("run_l1_dist", "run_l1_dist", 15000,135000.5,150000.5);
    run_l2_eff = new TH1D("run_l2_eff", "run_l2_eff", 15000,135000.5,150000.5);
    run_l2_dist = new TH1D("run_l2_dist", "run_l2_dist", 15000,135000.5,150000.5);
    run_l3_eff = new TH1D("run_l3_eff", "run_l3_eff", 15000,135000.5,150000.5);
    run_l3_dist = new TH1D("run_l3_dist", "run_l3_dist", 15000,135000.5,150000.5);
    run_dm2_eff = new TH1D("run_dm2_eff", "run_dm2_eff", 15000,135000.5,150000.5);
    run_dm2_dist = new TH1D("run_dm2_dist", "run_dm2_dist", 15000,135000.5,150000.5);
    run_dm1_eff = new TH1D("run_dm1_eff", "run_dm1_eff", 15000,135000.5,150000.5);
    run_dm1_dist = new TH1D("run_dm1_dist", "run_dm1_dist", 15000,135000.5,150000.5);
    run_dp1_eff = new TH1D("run_dp1_eff", "run_dp1_eff", 15000,135000.5,150000.5);
    run_dp1_dist = new TH1D("run_dp1_dist", "run_dp1_dist", 15000,135000.5,150000.5);
    run_dp2_eff = new TH1D("run_dp2_eff", "run_dp2_eff", 15000,135000.5,150000.5);
    run_dp2_dist = new TH1D("run_dp2_dist", "run_dp2_dist", 15000,135000.5,150000.5);
  
    // Pt cut plots
    pt_bpix_eff = new TH1D("pt_bpix_eff", "pt_bpix_eff", 100,0.0,10.0);
    pt_bpix_dist = new TH1D("pt_bpix_dist", "pt_bpix_dist", 100,0.0,10.0);
    pt_fpix_eff = new TH1D("pt_fpix_eff", "pt_fpix_eff", 100,0.0,10.0);
    pt_fpix_dist = new TH1D("pt_fpix_dist", "pt_fpix_dist", 100,0.0,10.0);
    pt_bpix_cumeff = new TH1D("pt_bpix_cumeff", "pt_bpix_cumeff", 100,0.0,10.0);
    pt_bpix_cumdist = new TH1D("pt_bpix_cumdist", "pt_bpix_cumdist", 100,0.0,10.0);
    pt_fpix_cumeff = new TH1D("pt_fpix_cumeff", "pt_fpix_cumeff", 100,0.0,10.0);
    pt_fpix_cumdist = new TH1D("pt_fpix_cumdist", "pt_fpix_cumdist", 100,0.0,10.0);
  
    algo4_pt_bpix_eff = new TH1D("algo4_pt_bpix_eff", "algo4_pt_bpix_eff", 100,0.0,10.0);
    algo4_pt_bpix_dist = new TH1D("algo4_pt_bpix_dist", "algo4_pt_bpix_dist", 100,0.0,10.0);
    algo4_pt_bpix_cumeff = new TH1D("algo4_pt_bpix_cumeff", "algo4_pt_bpix_cumeff", 100,0.0,10.0);
    algo4_pt_bpix_cumdist = new TH1D("algo4_pt_bpix_cumdist", "algo4_pt_bpix_cumdist", 100,0.0,10.0);
    algo4_pt_fpix_eff = new TH1D("algo4_pt_fpix_eff", "algo4_pt_fpix_eff", 100,0.0,10.0);
    algo4_pt_fpix_dist = new TH1D("algo4_pt_fpix_dist", "algo4_pt_fpix_dist", 100,0.0,10.0);
    algo4_pt_fpix_cumeff = new TH1D("algo4_pt_fpix_cumeff", "algo4_pt_fpix_cumeff", 100,0.0,10.0);
    algo4_pt_fpix_cumdist = new TH1D("algo4_pt_fpix_cumdist", "algo4_pt_fpix_cumdist", 100,0.0,10.0);
    algo5_pt_bpix_eff = new TH1D("algo5_pt_bpix_eff", "algo5_pt_bpix_eff", 100,0.0,10.0);
    algo5_pt_bpix_dist = new TH1D("algo5_pt_bpix_dist", "algo5_pt_bpix_dist", 100,0.0,10.0);
    algo5_pt_bpix_cumeff = new TH1D("algo5_pt_bpix_cumeff", "algo5_pt_bpix_cumeff", 100,0.0,10.0);
    algo5_pt_bpix_cumdist = new TH1D("algo5_pt_bpix_cumdist", "algo5_pt_bpix_cumdist", 100,0.0,10.0);
    algo5_pt_fpix_eff = new TH1D("algo5_pt_fpix_eff", "algo5_pt_fpix_eff", 100,0.0,10.0);
    algo5_pt_fpix_dist = new TH1D("algo5_pt_fpix_dist", "algo5_pt_fpix_dist", 100,0.0,10.0);
    algo5_pt_fpix_cumeff = new TH1D("algo5_pt_fpix_cumeff", "algo5_pt_fpix_cumeff", 100,0.0,10.0);
    algo5_pt_fpix_cumdist = new TH1D("algo5_pt_fpix_cumdist", "algo5_pt_fpix_cumdist", 100,0.0,10.0);
    algo6_pt_bpix_eff = new TH1D("algo6_pt_bpix_eff", "algo6_pt_bpix_eff", 100,0.0,10.0);
    algo6_pt_bpix_dist = new TH1D("algo6_pt_bpix_dist", "algo6_pt_bpix_dist", 100,0.0,10.0);
    algo6_pt_bpix_cumeff = new TH1D("algo6_pt_bpix_cumeff", "algo6_pt_bpix_cumeff", 100,0.0,10.0);
    algo6_pt_bpix_cumdist = new TH1D("algo6_pt_bpix_cumdist", "algo6_pt_bpix_cumdist", 100,0.0,10.0);
    algo6_pt_fpix_eff = new TH1D("algo6_pt_fpix_eff", "algo6_pt_fpix_eff", 100,0.0,10.0);
    algo6_pt_fpix_dist = new TH1D("algo6_pt_fpix_dist", "algo6_pt_fpix_dist", 100,0.0,10.0);
    algo6_pt_fpix_cumeff = new TH1D("algo6_pt_fpix_cumeff", "algo6_pt_fpix_cumeff", 100,0.0,10.0);
    algo6_pt_fpix_cumdist = new TH1D("algo6_pt_fpix_cumdist", "algo6_pt_fpix_cumdist", 100,0.0,10.0);
    algo7_pt_bpix_eff = new TH1D("algo7_pt_bpix_eff", "algo7_pt_bpix_eff", 100,0.0,10.0);
    algo7_pt_bpix_dist = new TH1D("algo7_pt_bpix_dist", "algo7_pt_bpix_dist", 100,0.0,10.0);
    algo7_pt_bpix_cumeff = new TH1D("algo7_pt_bpix_cumeff", "algo7_pt_bpix_cumeff", 100,0.0,10.0);
    algo7_pt_bpix_cumdist = new TH1D("algo7_pt_bpix_cumdist", "algo7_pt_bpix_cumdist", 100,0.0,10.0);
    algo7_pt_fpix_eff = new TH1D("algo7_pt_fpix_eff", "algo7_pt_fpix_eff", 100,0.0,10.0);
    algo7_pt_fpix_dist = new TH1D("algo7_pt_fpix_dist", "algo7_pt_fpix_dist", 100,0.0,10.0);
    algo7_pt_fpix_cumeff = new TH1D("algo7_pt_fpix_cumeff", "algo7_pt_fpix_cumeff", 100,0.0,10.0);
    algo7_pt_fpix_cumdist = new TH1D("algo7_pt_fpix_cumdist", "algo7_pt_fpix_cumdist", 100,0.0,10.0);
    algo8_pt_bpix_eff = new TH1D("algo8_pt_bpix_eff", "algo8_pt_bpix_eff", 100,0.0,10.0);
    algo8_pt_bpix_dist = new TH1D("algo8_pt_bpix_dist", "algo8_pt_bpix_dist", 100,0.0,10.0);
    algo8_pt_bpix_cumeff = new TH1D("algo8_pt_bpix_cumeff", "algo8_pt_bpix_cumeff", 100,0.0,10.0);
    algo8_pt_bpix_cumdist = new TH1D("algo8_pt_bpix_cumdist", "algo8_pt_bpix_cumdist", 100,0.0,10.0);
    algo8_pt_fpix_eff = new TH1D("algo8_pt_fpix_eff", "algo8_pt_fpix_eff", 100,0.0,10.0);
    algo8_pt_fpix_dist = new TH1D("algo8_pt_fpix_dist", "algo8_pt_fpix_dist", 100,0.0,10.0);
    algo8_pt_fpix_cumeff = new TH1D("algo8_pt_fpix_cumeff", "algo8_pt_fpix_cumeff", 100,0.0,10.0);
    algo8_pt_fpix_cumdist = new TH1D("algo8_pt_fpix_cumdist", "algo8_pt_fpix_cumdist", 100,0.0,10.0);
    algo9_pt_bpix_eff = new TH1D("algo9_pt_bpix_eff", "algo9_pt_bpix_eff", 100,0.0,10.0);
    algo9_pt_bpix_dist = new TH1D("algo9_pt_bpix_dist", "algo9_pt_bpix_dist", 100,0.0,10.0);
    algo9_pt_bpix_cumeff = new TH1D("algo9_pt_bpix_cumeff", "algo9_pt_bpix_cumeff", 100,0.0,10.0);
    algo9_pt_bpix_cumdist = new TH1D("algo9_pt_bpix_cumdist", "algo9_pt_bpix_cumdist", 100,0.0,10.0);
    algo9_pt_fpix_eff = new TH1D("algo9_pt_fpix_eff", "algo9_pt_fpix_eff", 100,0.0,10.0);
    algo9_pt_fpix_dist = new TH1D("algo9_pt_fpix_dist", "algo9_pt_fpix_dist", 100,0.0,10.0);
    algo9_pt_fpix_cumeff = new TH1D("algo9_pt_fpix_cumeff", "algo9_pt_fpix_cumeff", 100,0.0,10.0);
    algo9_pt_fpix_cumdist = new TH1D("algo9_pt_fpix_cumdist", "algo9_pt_fpix_cumdist", 100,0.0,10.0);
    algo45_pt_bpix_eff = new TH1D("algo45_pt_bpix_eff", "algo45_pt_bpix_eff", 100,0.0,10.0);
    algo45_pt_bpix_dist = new TH1D("algo45_pt_bpix_dist", "algo45_pt_bpix_dist", 100,0.0,10.0);
    algo45_pt_bpix_cumeff = new TH1D("algo45_pt_bpix_cumeff", "algo45_pt_bpix_cumeff", 100,0.0,10.0);
    algo45_pt_bpix_cumdist = new TH1D("algo45_pt_bpix_cumdist", "algo45_pt_bpix_cumdist", 100,0.0,10.0);
    algo45_pt_fpix_eff = new TH1D("algo45_pt_fpix_eff", "algo45_pt_fpix_eff", 100,0.0,10.0);
    algo45_pt_fpix_dist = new TH1D("algo45_pt_fpix_dist", "algo45_pt_fpix_dist", 100,0.0,10.0);
    algo45_pt_fpix_cumeff = new TH1D("algo45_pt_fpix_cumeff", "algo45_pt_fpix_cumeff", 100,0.0,10.0);
    algo45_pt_fpix_cumdist = new TH1D("algo45_pt_fpix_cumdist", "algo45_pt_fpix_cumdist", 100,0.0,10.0);
  
    dz_20_l1_eff = new TH1D("dz_20_l1_eff", "dz_20_l1_eff", 20,0.0,20.0);
    dz_20_l1_dist = new TH1D("dz_20_l1_dist", "dz_20_l1_dist", 20,0.0,20.0);
    dz_20_l2_eff = new TH1D("dz_20_l2_eff", "dz_20_l2_eff", 20,0.0,20.0);
    dz_20_l2_dist = new TH1D("dz_20_l2_dist", "dz_20_l2_dist", 20,0.0,20.0);
    dz_20_l3_eff = new TH1D("dz_20_l3_eff", "dz_20_l3_eff", 20,0.0,20.0);
    dz_20_l3_dist = new TH1D("dz_20_l3_dist", "dz_20_l3_dist", 20,0.0,20.0);
    dz_20_fpix_eff = new TH1D("dz_20_fpix_eff", "dz_20_fpix_eff", 20,0.0,20.0);
    dz_20_fpix_dist = new TH1D("dz_20_fpix_dist", "dz_20_fpix_dist", 20,0.0,20.0);
    dz_2_l1_eff = new TH1D("dz_2_l1_eff", "dz_2_l1_eff", 20,0.0,2.0);
    dz_2_l1_dist = new TH1D("dz_2_l1_dist", "dz_2_l1_dist", 20,0.0,2.0);
    dz_2_l2_eff = new TH1D("dz_2_l2_eff", "dz_2_l2_eff", 20,0.0,2.0);
    dz_2_l2_dist = new TH1D("dz_2_l2_dist", "dz_2_l2_dist", 20,0.0,2.0);
    dz_2_l3_eff = new TH1D("dz_2_l3_eff", "dz_2_l3_eff", 20,0.0,2.0);
    dz_2_l3_dist = new TH1D("dz_2_l3_dist", "dz_2_l3_dist", 20,0.0,2.0);
    dz_2_fpix_eff = new TH1D("dz_2_fpix_eff", "dz_2_fpix_eff", 20,0.0,2.0);
    dz_2_fpix_dist = new TH1D("dz_2_fpix_dist", "dz_2_fpix_dist", 20,0.0,2.0);
    dz_02_l1_eff = new TH1D("dz_02_l1_eff", "dz_02_l1_eff", 20,0.0,0.2);
    dz_02_l1_dist = new TH1D("dz_02_l1_dist", "dz_02_l1_dist", 20,0.0,0.2);
    dz_02_l2_eff = new TH1D("dz_02_l2_eff", "dz_02_l2_eff", 20,0.0,0.2);
    dz_02_l2_dist = new TH1D("dz_02_l2_dist", "dz_02_l2_dist", 20,0.0,0.2);
    dz_02_l3_eff = new TH1D("dz_02_l3_eff", "dz_02_l3_eff", 20,0.0,0.2);
    dz_02_l3_dist = new TH1D("dz_02_l3_dist", "dz_02_l3_dist", 20,0.0,0.2);
    dz_02_fpix_eff = new TH1D("dz_02_fpix_eff", "dz_02_fpix_eff", 20,0.0,0.2);
    dz_02_fpix_dist = new TH1D("dz_02_fpix_dist", "dz_02_fpix_dist", 20,0.0,0.2);
    dz_002_l1_eff = new TH1D("dz_002_l1_eff", "dz_002_l1_eff", 20,0.0,0.02);
    dz_002_l1_dist = new TH1D("dz_002_l1_dist", "dz_002_l1_dist", 20,0.0,0.02);
    dz_002_l2_eff = new TH1D("dz_002_l2_eff", "dz_002_l2_eff", 20,0.0,0.02);
    dz_002_l2_dist = new TH1D("dz_002_l2_dist", "dz_002_l2_dist", 20,0.0,0.02);
    dz_002_l3_eff = new TH1D("dz_002_l3_eff", "dz_002_l3_eff", 20,0.0,0.02);
    dz_002_l3_dist = new TH1D("dz_002_l3_dist", "dz_002_l3_dist", 20,0.0,0.02);
    dz_002_fpix_eff = new TH1D("dz_002_fpix_eff", "dz_002_fpix_eff", 20,0.0,0.02);
    dz_002_fpix_dist = new TH1D("dz_002_fpix_dist", "dz_002_fpix_dist", 20,0.0,0.02);
  
    d0_2_l1_eff = new TH1D("d0_2_l1_eff", "d0_2_l1_eff", 20,0.0,2.0);
    d0_2_l1_dist = new TH1D("d0_2_l1_dist", "d0_2_l1_dist", 20,0.0,2.0);
    d0_2_l2_eff = new TH1D("d0_2_l2_eff", "d0_2_l2_eff", 20,0.0,2.0);
    d0_2_l2_dist = new TH1D("d0_2_l2_dist", "d0_2_l2_dist", 20,0.0,2.0);
    d0_2_l3_eff = new TH1D("d0_2_l3_eff", "d0_2_l3_eff", 20,0.0,2.0);
    d0_2_l3_dist = new TH1D("d0_2_l3_dist", "d0_2_l3_dist", 20,0.0,2.0);
    d0_2_fpix_eff = new TH1D("d0_2_fpix_eff", "d0_2_fpix_eff", 20,0.0,2.0);
    d0_2_fpix_dist = new TH1D("d0_2_fpix_dist", "d0_2_fpix_dist", 20,0.0,2.0);
    d0_02_l1_eff = new TH1D("d0_02_l1_eff", "d0_02_l1_eff", 20,0.0,0.2);
    d0_02_l1_dist = new TH1D("d0_02_l1_dist", "d0_02_l1_dist", 20,0.0,0.2);
    d0_02_l2_eff = new TH1D("d0_02_l2_eff", "d0_02_l2_eff", 20,0.0,0.2);
    d0_02_l2_dist = new TH1D("d0_02_l2_dist", "d0_02_l2_dist", 20,0.0,0.2);
    d0_02_l3_eff = new TH1D("d0_02_l3_eff", "d0_02_l3_eff", 20,0.0,0.2);
    d0_02_l3_dist = new TH1D("d0_02_l3_dist", "d0_02_l3_dist", 20,0.0,0.2);
    d0_02_fpix_eff = new TH1D("d0_02_fpix_eff", "d0_02_fpix_eff", 20,0.0,0.2);
    d0_02_fpix_dist = new TH1D("d0_02_fpix_dist", "d0_02_fpix_dist", 20,0.0,0.2);
    d0_002_l1_eff = new TH1D("d0_002_l1_eff", "d0_002_l1_eff", 20,0.0,0.02);
    d0_002_l1_dist = new TH1D("d0_002_l1_dist", "d0_002_l1_dist", 20,0.0,0.02);
    d0_002_l2_eff = new TH1D("d0_002_l2_eff", "d0_002_l2_eff", 20,0.0,0.02);
    d0_002_l2_dist = new TH1D("d0_002_l2_dist", "d0_002_l2_dist", 20,0.0,0.02);
    d0_002_l3_eff = new TH1D("d0_002_l3_eff", "d0_002_l3_eff", 20,0.0,0.02);
    d0_002_l3_dist = new TH1D("d0_002_l3_dist", "d0_002_l3_dist", 20,0.0,0.02);
    d0_002_fpix_eff = new TH1D("d0_002_fpix_eff", "d0_002_fpix_eff", 20,0.0,0.02);
    d0_002_fpix_dist = new TH1D("d0_002_fpix_dist", "d0_002_fpix_dist", 20,0.0,0.02);
  
    dz_l1_cumeff = new TH1D("dz_l1_cumeff", "dz_l1_cumeff", 100,0.0,1.0);
    dz_l1_cumdist = new TH1D("dz_l1_cumdist", "dz_l1_cumdist", 100,0.0,1.0);
    dz_l2_cumeff = new TH1D("dz_l2_cumeff", "dz_l2_cumeff", 100,0.0,1.0);
    dz_l2_cumdist = new TH1D("dz_l2_cumdist", "dz_l2_cumdist", 100,0.0,1.0);
    dz_l3_cumeff = new TH1D("dz_l3_cumeff", "dz_l3_cumeff", 100,0.0,1.0);
    dz_l3_cumdist = new TH1D("dz_l3_cumdist", "dz_l3_cumdist", 100,0.0,1.0);
    dz_fpix_cumeff = new TH1D("dz_fpix_cumeff", "dz_fpix_cumeff", 100,0.0,1.0);
    dz_fpix_cumdist = new TH1D("dz_fpix_cumdist", "dz_fpix_cumdist", 100,0.0,1.0);
    d0_l1_cumeff = new TH1D("d0_l1_cumeff", "d0_l1_cumeff", 100,0.0,0.1);
    d0_l1_cumdist = new TH1D("d0_l1_cumdist", "d0_l1_cumdist", 100,0.0,0.1);
    d0_l2_cumeff = new TH1D("d0_l2_cumeff", "d0_l2_cumeff", 100,0.0,0.1);
    d0_l2_cumdist = new TH1D("d0_l2_cumdist", "d0_l2_cumdist", 100,0.0,0.1);
    d0_l3_cumeff = new TH1D("d0_l3_cumeff", "d0_l3_cumeff", 100,0.0,0.1);
    d0_l3_cumdist = new TH1D("d0_l3_cumdist", "d0_l3_cumdist", 100,0.0,0.1);
    d0_fpix_cumeff = new TH1D("d0_fpix_cumeff", "d0_fpix_cumeff", 100,0.0,0.1);
    d0_fpix_cumdist = new TH1D("d0_fpix_cumdist", "d0_fpix_cumdist", 100,0.0,0.1);
  
    dz_d0_l1_cumeff = new TH2D("dz_d0_l1_cumeff", "dz_d0_l1_cumeff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l1_cumdist = new TH2D("dz_d0_l1_cumdist", "dz_d0_l1_cumdist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l1_eff = new TH2D("dz_d0_l1_eff", "dz_d0_l1_eff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l1_dist = new TH2D("dz_d0_l1_dist", "dz_d0_l1_dist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l2_cumeff = new TH2D("dz_d0_l2_cumeff", "dz_d0_l2_cumeff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l2_cumdist = new TH2D("dz_d0_l2_cumdist", "dz_d0_l2_cumdist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l2_eff = new TH2D("dz_d0_l2_eff", "dz_d0_l2_eff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l2_dist = new TH2D("dz_d0_l2_dist", "dz_d0_l2_dist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l3_cumeff = new TH2D("dz_d0_l3_cumeff", "dz_d0_l3_cumeff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l3_cumdist = new TH2D("dz_d0_l3_cumdist", "dz_d0_l3_cumdist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l3_eff = new TH2D("dz_d0_l3_eff", "dz_d0_l3_eff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_l3_dist = new TH2D("dz_d0_l3_dist", "dz_d0_l3_dist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_fpix_cumeff = new TH2D("dz_d0_fpix_cumeff", "dz_d0_fpix_cumeff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_fpix_cumdist = new TH2D("dz_d0_fpix_cumdist", "dz_d0_fpix_cumdist", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_fpix_eff = new TH2D("dz_d0_fpix_eff", "dz_d0_fpix_eff", 100,0.0,1.0,100,0.0,0.1);
    dz_d0_fpix_dist = new TH2D("dz_d0_fpix_dist", "dz_d0_fpix_dist", 100,0.0,1.0,100,0.0,0.1);
  
    nstrip_l1_eff = new TH1D("nstrip_l1_eff","nstrip_l1_eff",28,0.0,28.0);
    nstrip_l1_dist = new TH1D("nstrip_l1_dist","nstrip_l1_dist",28,0.0,28.0);
    nstrip_l1_cumeff = new TH1D("nstrip_l1_cumeff","nstrip_l1_cumeff",28,0.0,28.0);
    nstrip_l1_cumdist = new TH1D("nstrip_l1_cumdist","nstrip_l1_cumdist",28,0.0,28.0);
    nstrip_l2_eff = new TH1D("nstrip_l2_eff","nstrip_l2_eff",28,0.0,28.0);
    nstrip_l2_dist = new TH1D("nstrip_l2_dist","nstrip_l2_dist",28,0.0,28.0);
    nstrip_l2_cumeff = new TH1D("nstrip_l2_cumeff","nstrip_l2_cumeff",28,0.0,28.0);
    nstrip_l2_cumdist = new TH1D("nstrip_l2_cumdist","nstrip_l2_cumdist",28,0.0,28.0);
    nstrip_l3_eff = new TH1D("nstrip_l3_eff","nstrip_l3_eff",28,0.0,28.0);
    nstrip_l3_dist = new TH1D("nstrip_l3_dist","nstrip_l3_dist",28,0.0,28.0);
    nstrip_l3_cumeff = new TH1D("nstrip_l3_cumeff","nstrip_l3_cumeff",28,0.0,28.0);
    nstrip_l3_cumdist = new TH1D("nstrip_l3_cumdist","nstrip_l3_cumdist",28,0.0,28.0);
    nstrip_fpix_eff = new TH1D("nstrip_fpix_eff","nstrip_fpix_eff",28,0.0,28.0);
    nstrip_fpix_dist = new TH1D("nstrip_fpix_dist","nstrip_fpix_dist",28,0.0,28.0);
    nstrip_fpix_cumeff = new TH1D("nstrip_fpix_cumeff","nstrip_fpix_cumeff",28,0.0,28.0);
    nstrip_fpix_cumdist = new TH1D("nstrip_fpix_cumdist","nstrip_fpix_cumdist",28,0.0,28.0);
  
    normchi2_l1_eff = new TH1D("normchi2_l1_eff","normchi2_l1_eff",500,0.0,5.0);
    normchi2_l1_dist = new TH1D("normchi2_l1_dist","normchi2_l1_dist",500,0.0,5.0);
    normchi2_l2_eff = new TH1D("normchi2_l2_eff","normchi2_l2_eff",500,0.0,5.0);
    normchi2_l2_dist = new TH1D("normchi2_l2_dist","normchi2_l2_dist",500,0.0,5.0);
    normchi2_l3_eff = new TH1D("normchi2_l3_eff","normchi2_l3_eff",500,0.0,5.0);
    normchi2_l3_dist = new TH1D("normchi2_l3_dist","normchi2_l3_dist",500,0.0,5.0);
    normchi2_fpix_eff = new TH1D("normchi2_fpix_eff","normchi2_fpix_eff",500,0.0,5.0);
    normchi2_fpix_dist = new TH1D("normchi2_fpix_dist","normchi2_fpix_dist",500,0.0,5.0);
    normchi2_l1_cumeff = new TH1D("normchi2_l1_cumeff","normchi2_l1_cumeff",500,0.0,5.0);
    normchi2_l1_cumdist = new TH1D("normchi2_l1_cumdist","normchi2_l1_cumdist",500,0.0,5.0);
    normchi2_l2_cumeff = new TH1D("normchi2_l2_cumeff","normchi2_l2_cumeff",500,0.0,5.0);
    normchi2_l2_cumdist = new TH1D("normchi2_l2_cumdist","normchi2_l2_cumdist",500,0.0,5.0);
    normchi2_l3_cumeff = new TH1D("normchi2_l3_cumeff","normchi2_l3_cumeff",500,0.0,5.0);
    normchi2_l3_cumdist = new TH1D("normchi2_l3_cumdist","normchi2_l3_cumdist",500,0.0,5.0);
    normchi2_fpix_cumeff = new TH1D("normchi2_fpix_cumeff","normchi2_fpix_cumeff",500,0.0,5.0);
    normchi2_fpix_cumdist = new TH1D("normchi2_fpix_cumdist","normchi2_fpix_cumdist",500,0.0,5.0);
  
    normchi2_withpt_l1_eff = new TH1D("normchi2_withpt_l1_eff","normchi2_withpt_l1_eff",500,0.0,5.0);
    normchi2_withpt_l1_dist = new TH1D("normchi2_withpt_l1_dist","normchi2_withpt_l1_dist",500,0.0,5.0);
    normchi2_withpt_l2_eff = new TH1D("normchi2_withpt_l2_eff","normchi2_withpt_l2_eff",500,0.0,5.0);
    normchi2_withpt_l2_dist = new TH1D("normchi2_withpt_l2_dist","normchi2_withpt_l2_dist",500,0.0,5.0);
    normchi2_withpt_l3_eff = new TH1D("normchi2_withpt_l3_eff","normchi2_withpt_l3_eff",500,0.0,5.0);
    normchi2_withpt_l3_dist = new TH1D("normchi2_withpt_l3_dist","normchi2_withpt_l3_dist",500,0.0,5.0);
    normchi2_withpt_fpix_eff = new TH1D("normchi2_withpt_fpix_eff","normchi2_withpt_fpix_eff",500,0.0,5.0);
    normchi2_withpt_fpix_dist = new TH1D("normchi2_withpt_fpix_dist","normchi2_withpt_fpix_dist",500,0.0,5.0);
    normchi2_withpt_l1_cumeff = new TH1D("normchi2_withpt_l1_cumeff","normchi2_withpt_l1_cumeff",500,0.0,5.0);
    normchi2_withpt_l1_cumdist = new TH1D("normchi2_withpt_l1_cumdist","normchi2_withpt_l1_cumdist",500,0.0,5.0);
    normchi2_withpt_l2_cumeff = new TH1D("normchi2_withpt_l2_cumeff","normchi2_withpt_l2_cumeff",500,0.0,5.0);
    normchi2_withpt_l2_cumdist = new TH1D("normchi2_withpt_l2_cumdist","normchi2_withpt_l2_cumdist",500,0.0,5.0);
    normchi2_withpt_l3_cumeff = new TH1D("normchi2_withpt_l3_cumeff","normchi2_withpt_l3_cumeff",500,0.0,5.0);
    normchi2_withpt_l3_cumdist = new TH1D("normchi2_withpt_l3_cumdist","normchi2_withpt_l3_cumdist",500,0.0,5.0);
    normchi2_withpt_fpix_cumeff = new TH1D("normchi2_withpt_fpix_cumeff","normchi2_withpt_fpix_cumeff",500,0.0,5.0);
    normchi2_withpt_fpix_cumdist = new TH1D("normchi2_withpt_fpix_cumdist","normchi2_withpt_fpix_cumdist",500,0.0,5.0);
  
    drcl_l1_eff = new TH1D("drcl_l1_eff","drcl_l1_eff",100,0.0,0.1);
    drcl_l1_dist = new TH1D("drcl_l1_dist","drcl_l1_dist",100,0.0,0.1);
    drcl_l2_eff = new TH1D("drcl_l2_eff","drcl_l2_eff",100,0.0,0.1);
    drcl_l2_dist = new TH1D("drcl_l2_dist","drcl_l2_dist",100,0.0,0.1);
    drcl_l3_eff = new TH1D("drcl_l3_eff","drcl_l3_eff",100,0.0,0.1);
    drcl_l3_dist = new TH1D("drcl_l3_dist","drcl_l3_dist",100,0.0,0.1);
    drcl_fpix_eff = new TH1D("drcl_fpix_eff","drcl_fpix_eff",100,0.0,0.1);
    drcl_fpix_dist = new TH1D("drcl_fpix_dist","drcl_fpix_dist",100,0.0,0.1);
    drcl_l1_cumeff = new TH1D("drcl_l1_cumeff","drcl_l1_cumeff",100,0.0,0.1);
    drcl_l1_cumdist = new TH1D("drcl_l1_cumdist","drcl_l1_cumdist",100,0.0,0.1);
    drcl_l2_cumeff = new TH1D("drcl_l2_cumeff","drcl_l2_cumeff",100,0.0,0.1);
    drcl_l2_cumdist = new TH1D("drcl_l2_cumdist","drcl_l2_cumdist",100,0.0,0.1);
    drcl_l3_cumeff = new TH1D("drcl_l3_cumeff","drcl_l3_cumeff",100,0.0,0.1);
    drcl_l3_cumdist = new TH1D("drcl_l3_cumdist","drcl_l3_cumdist",100,0.0,0.1);
    drcl_fpix_cumeff = new TH1D("drcl_fpix_cumeff","drcl_fpix_cumeff",100,0.0,0.1);
    drcl_fpix_cumdist = new TH1D("drcl_fpix_cumdist","drcl_fpix_cumdist",100,0.0,0.1);
  
    drcl12_l1_eff = new TH1D("drcl12_l1_eff","drcl12_l1_eff",1260,0.0,6.3);
    drcl12_l1_dist = new TH1D("drcl12_l1_dist","drcl12_l1_dist",1260,0.0,6.3);
    drcl12_l2_eff = new TH1D("drcl12_l2_eff","drcl12_l2_eff",1260,0.0,6.3);
    drcl12_l2_dist = new TH1D("drcl12_l2_dist","drcl12_l2_dist",1260,0.0,6.3);
    drcl12_l3_eff = new TH1D("drcl12_l3_eff","drcl12_l3_eff",1260,0.0,6.3);
    drcl12_l3_dist = new TH1D("drcl12_l3_dist","drcl12_l3_dist",1260,0.0,6.3);
    drcl12_fpix_eff = new TH1D("drcl12_fpix_eff","drcl12_fpix_eff",800,0.0,4.0);
    drcl12_fpix_dist = new TH1D("drcl12_fpix_dist","drcl12_fpix_dist",800,0.0,4.0);
    drcl12_l1_cumeff = new TH1D("drcl12_l1_cumeff","drcl12_l1_cumeff",1260,0.0,6.3);
    drcl12_l1_cumdist = new TH1D("drcl12_l1_cumdist","drcl12_l1_cumdist",1260,0.0,6.3);
    drcl12_l2_cumeff = new TH1D("drcl12_l2_cumeff","drcl12_l2_cumeff",1260,0.0,6.3);
    drcl12_l2_cumdist = new TH1D("drcl12_l2_cumdist","drcl12_l2_cumdist",1260,0.0,6.3);
    drcl12_l3_cumeff = new TH1D("drcl12_l3_cumeff","drcl12_l3_cumeff",1260,0.0,6.3);
    drcl12_l3_cumdist = new TH1D("drcl12_l3_cumdist","drcl12_l3_cumdist",1260,0.0,6.3);
    drcl12_fpix_cumeff = new TH1D("drcl12_fpix_cumeff","drcl12_fpix_cumeff",800,0.0,4.0);
    drcl12_fpix_cumdist = new TH1D("drcl12_fpix_cumdist","drcl12_fpix_cumdist",800,0.0,4.0);
  
    drcl12_drcl_l1_eff = new TH2D("drcl12_drcl_l1_eff","drcl12_drcl_l1_eff",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l1_dist = new TH2D("drcl12_drcl_l1_dist","drcl12_drcl_l1_dist",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l2_eff = new TH2D("drcl12_drcl_l2_eff","drcl12_drcl_l2_eff",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l2_dist = new TH2D("drcl12_drcl_l2_dist","drcl12_drcl_l2_dist",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l3_eff = new TH2D("drcl12_drcl_l3_eff","drcl12_drcl_l3_eff",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l3_dist = new TH2D("drcl12_drcl_l3_dist","drcl12_drcl_l3_dist",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_fpix_eff = new TH2D("drcl12_drcl_fpix_eff","drcl12_drcl_fpix_eff",800,0.0,4.0,100,0.0,0.1);
    drcl12_drcl_fpix_dist = new TH2D("drcl12_drcl_fpix_dist","drcl12_drcl_fpix_dist",800,0.0,4.0,100,0.0,0.1);
    drcl12_drcl_l1_cumeff = new TH2D("drcl12_drcl_l1_cumeff","drcl12_drcl_l1_cumeff",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l1_cumdist = new TH2D("drcl12_drcl_l1_cumdist","drcl12_drcl_l1_cumdist",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l2_cumeff = new TH2D("drcl12_drcl_l2_cumeff","drcl12_drcl_l2_cumeff",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l2_cumdist = new TH2D("drcl12_drcl_l2_cumdist","drcl12_drcl_l2_cumdist",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l3_cumeff = new TH2D("drcl12_drcl_l3_cumeff","drcl12_drcl_l3_cumeff",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_l3_cumdist = new TH2D("drcl12_drcl_l3_cumdist","drcl12_drcl_l3_cumdist",1260,0.0,6.3,100,0.0,0.1);
    drcl12_drcl_fpix_cumeff = new TH2D("drcl12_drcl_fpix_cumeff","drcl12_drcl_fpix_cumeff",800,0.0,4.0,100,0.0,0.1);
    drcl12_drcl_fpix_cumdist = new TH2D("drcl12_drcl_fpix_cumdist","drcl12_drcl_fpix_cumdist",800,0.0,4.0,100,0.0,0.1);
  
    neweff_eff = new TH1D("neweff_eff","neweff_eff",200,0.0,1.0);
    neweff_dist = new TH1D("neweff_dist","neweff_dist",200,0.0,1.0);
    neweff_bpix_eff = new TH1D("neweff_bpix_eff","neweff_bpix_eff",200,0.0,1.0);
    neweff_bpix_dist = new TH1D("neweff_bpix_dist","neweff_bpix_dist",200,0.0,1.0);
    neweff_fpix_eff = new TH1D("neweff_fpix_eff","neweff_fpix_eff",200,0.0,1.0);
    neweff_fpix_dist = new TH1D("neweff_fpix_dist","neweff_fpix_dist",200,0.0,1.0);
    neweff_l1_eff = new TH1D("neweff_l1_eff","neweff_l1_eff",200,0.0,1.0);
    neweff_l1_dist = new TH1D("neweff_l1_dist","neweff_l1_dist",200,0.0,1.0);
    neweff_l2_eff = new TH1D("neweff_l2_eff","neweff_l2_eff",200,0.0,1.0);
    neweff_l2_dist = new TH1D("neweff_l2_dist","neweff_l2_dist",200,0.0,1.0);
    neweff_l3_eff = new TH1D("neweff_l3_eff","neweff_l3_eff",200,0.0,1.0);
    neweff_l3_dist = new TH1D("neweff_l3_dist","neweff_l3_dist",200,0.0,1.0);
    neweff_dm2_eff = new TH1D("neweff_dm2_eff","neweff_dm2_eff",200,0.0,1.0);
    neweff_dm2_dist = new TH1D("neweff_dm2_dist","neweff_dm2_dist",200,0.0,1.0);
    neweff_dm1_eff = new TH1D("neweff_dm1_eff","neweff_dm1_eff",200,0.0,1.0);
    neweff_dm1_dist = new TH1D("neweff_dm1_dist","neweff_dm1_dist",200,0.0,1.0);
    neweff_dp1_eff = new TH1D("neweff_dp1_eff","neweff_dp1_eff",200,0.0,1.0);
    neweff_dp1_dist = new TH1D("neweff_dp1_dist","neweff_dp1_dist",200,0.0,1.0);
    neweff_dp2_eff = new TH1D("neweff_dp2_eff","neweff_dp2_eff",200,0.0,1.0);
    neweff_dp2_dist = new TH1D("neweff_dp2_dist","neweff_dp2_dist",200,0.0,1.0);
  
    neweff2_eff = new TH1D("neweff2_eff","neweff2_eff",200,0.0,1.0);
    neweff2_dist = new TH1D("neweff2_dist","neweff2_dist",200,0.0,1.0);
    neweff2_bpix_eff = new TH1D("neweff2_bpix_eff","neweff2_bpix_eff",200,0.0,1.0);
    neweff2_bpix_dist = new TH1D("neweff2_bpix_dist","neweff2_bpix_dist",200,0.0,1.0);
    neweff2_fpix_eff = new TH1D("neweff2_fpix_eff","neweff2_fpix_eff",200,0.0,1.0);
    neweff2_fpix_dist = new TH1D("neweff2_fpix_dist","neweff2_fpix_dist",200,0.0,1.0);
    neweff2_l1_eff = new TH1D("neweff2_l1_eff","neweff2_l1_eff",200,0.0,1.0);
    neweff2_l1_dist = new TH1D("neweff2_l1_dist","neweff2_l1_dist",200,0.0,1.0);
    neweff2_l2_eff = new TH1D("neweff2_l2_eff","neweff2_l2_eff",200,0.0,1.0);
    neweff2_l2_dist = new TH1D("neweff2_l2_dist","neweff2_l2_dist",200,0.0,1.0);
    neweff2_l3_eff = new TH1D("neweff2_l3_eff","neweff2_l3_eff",200,0.0,1.0);
    neweff2_l3_dist = new TH1D("neweff2_l3_dist","neweff2_l3_dist",200,0.0,1.0);
    neweff2_dm2_eff = new TH1D("neweff2_dm2_eff","neweff2_dm2_eff",200,0.0,1.0);
    neweff2_dm2_dist = new TH1D("neweff2_dm2_dist","neweff2_dm2_dist",200,0.0,1.0);
    neweff2_dm1_eff = new TH1D("neweff2_dm1_eff","neweff2_dm1_eff",200,0.0,1.0);
    neweff2_dm1_dist = new TH1D("neweff2_dm1_dist","neweff2_dm1_dist",200,0.0,1.0);
    neweff2_dp1_eff = new TH1D("neweff2_dp1_eff","neweff2_dp1_eff",200,0.0,1.0);
    neweff2_dp1_dist = new TH1D("neweff2_dp1_dist","neweff2_dp1_dist",200,0.0,1.0);
    neweff2_dp2_eff = new TH1D("neweff2_dp2_eff","neweff2_dp2_eff",200,0.0,1.0);
    neweff2_dp2_dist = new TH1D("neweff2_dp2_dist","neweff2_dp2_dist",200,0.0,1.0);
  
    mod1_clx_cly_l1_neweff = new TH2D("mod1_clx_cly_l1_neweff","mod1_clx_cly_l1_neweff",200,0.0,1.0,200,0.0,1.0);
    mod1_clx_cly_l1_newdist = new TH2D("mod1_clx_cly_l1_newdist","mod1_clx_cly_l1_newdist",200,0.0,1.0,200,0.0,1.0);
    mod4_clx_cly_l1_neweff = new TH2D("mod4_clx_cly_l1_neweff","mod4_clx_cly_l1_neweff",200,0.0,1.0,200,0.0,1.0);
    mod4_clx_cly_l1_newdist = new TH2D("mod4_clx_cly_l1_newdist","mod4_clx_cly_l1_newdist",200,0.0,1.0,200,0.0,1.0);
    mod1_clx_l1_neweff = new TH1D("mod1_clx_l1_neweff","mod1_clx_l1_neweff",200,0.0,1.0);
    mod1_clx_l1_newdist = new TH1D("mod1_clx_l1_newdist","mod1_clx_l1_newdist",200,0.0,1.0);
    mod4_clx_l1_neweff = new TH1D("mod4_clx_l1_neweff","mod4_clx_l1_neweff",200,0.0,1.0);
    mod4_clx_l1_newdist = new TH1D("mod4_clx_l1_newdist","mod4_clx_l1_newdist",200,0.0,1.0);
    mod1_cly_l1_neweff = new TH1D("mod1_cly_l1_neweff","mod1_cly_l1_neweff",200,0.0,1.0);
    mod1_cly_l1_newdist = new TH1D("mod1_cly_l1_newdist","mod1_cly_l1_newdist",200,0.0,1.0);
    mod4_cly_l1_neweff = new TH1D("mod4_cly_l1_neweff","mod4_cly_l1_neweff",200,0.0,1.0);
    mod4_cly_l1_newdist = new TH1D("mod4_cly_l1_newdist","mod4_cly_l1_newdist",200,0.0,1.0);
    mod1_clx_cly_l2_neweff = new TH2D("mod1_clx_cly_l2_neweff","mod1_clx_cly_l2_neweff",200,0.0,1.0,200,0.0,1.0);
    mod1_clx_cly_l2_newdist = new TH2D("mod1_clx_cly_l2_newdist","mod1_clx_cly_l2_newdist",200,0.0,1.0,200,0.0,1.0);
    mod4_clx_cly_l2_neweff = new TH2D("mod4_clx_cly_l2_neweff","mod4_clx_cly_l2_neweff",200,0.0,1.0,200,0.0,1.0);
    mod4_clx_cly_l2_newdist = new TH2D("mod4_clx_cly_l2_newdist","mod4_clx_cly_l2_newdist",200,0.0,1.0,200,0.0,1.0);
    mod1_clx_l2_neweff = new TH1D("mod1_clx_l2_neweff","mod1_clx_l2_neweff",200,0.0,1.0);
    mod1_clx_l2_newdist = new TH1D("mod1_clx_l2_newdist","mod1_clx_l2_newdist",200,0.0,1.0);
    mod4_clx_l2_neweff = new TH1D("mod4_clx_l2_neweff","mod4_clx_l2_neweff",200,0.0,1.0);
    mod4_clx_l2_newdist = new TH1D("mod4_clx_l2_newdist","mod4_clx_l2_newdist",200,0.0,1.0);
    mod1_cly_l2_neweff = new TH1D("mod1_cly_l2_neweff","mod1_cly_l2_neweff",200,0.0,1.0);
    mod1_cly_l2_newdist = new TH1D("mod1_cly_l2_newdist","mod1_cly_l2_newdist",200,0.0,1.0);
    mod4_cly_l2_neweff = new TH1D("mod4_cly_l2_neweff","mod4_cly_l2_neweff",200,0.0,1.0);
    mod4_cly_l2_newdist = new TH1D("mod4_cly_l2_newdist","mod4_cly_l2_newdist",200,0.0,1.0);
    mod1_clx_cly_l3_neweff = new TH2D("mod1_clx_cly_l3_neweff","mod1_clx_cly_l3_neweff",200,0.0,1.0,200,0.0,1.0);
    mod1_clx_cly_l3_newdist = new TH2D("mod1_clx_cly_l3_newdist","mod1_clx_cly_l3_newdist",200,0.0,1.0,200,0.0,1.0);
    mod4_clx_cly_l3_neweff = new TH2D("mod4_clx_cly_l3_neweff","mod4_clx_cly_l3_neweff",200,0.0,1.0,200,0.0,1.0);
    mod4_clx_cly_l3_newdist = new TH2D("mod4_clx_cly_l3_newdist","mod4_clx_cly_l3_newdist",200,0.0,1.0,200,0.0,1.0);
    mod1_clx_l3_neweff = new TH1D("mod1_clx_l3_neweff","mod1_clx_l3_neweff",200,0.0,1.0);
    mod1_clx_l3_newdist = new TH1D("mod1_clx_l3_newdist","mod1_clx_l3_newdist",200,0.0,1.0);
    mod4_clx_l3_neweff = new TH1D("mod4_clx_l3_neweff","mod4_clx_l3_neweff",200,0.0,1.0);
    mod4_clx_l3_newdist = new TH1D("mod4_clx_l3_newdist","mod4_clx_l3_newdist",200,0.0,1.0);
    mod1_cly_l3_neweff = new TH1D("mod1_cly_l3_neweff","mod1_cly_l3_neweff",200,0.0,1.0);
    mod1_cly_l3_newdist = new TH1D("mod1_cly_l3_newdist","mod1_cly_l3_newdist",200,0.0,1.0);
    mod4_cly_l3_neweff = new TH1D("mod4_cly_l3_neweff","mod4_cly_l3_neweff",200,0.0,1.0);
    mod4_cly_l3_newdist = new TH1D("mod4_cly_l3_newdist","mod4_cly_l3_newdist",200,0.0,1.0);
  
    drhit_drcl_neweff_eff = new TH2D("drhit_drcl_neweff_eff","drhit_drcl_neweff_eff",200,0.0,2.0,100,0.0,0.1);
    drhit_drcl_neweff_dist = new TH2D("drhit_drcl_neweff_dist","drhit_drcl_neweff_dist",200,0.0,2.0,100,0.0,0.1);
    drhit_drcl_neweff_bpix_eff = new TH2D("drhit_drcl_neweff_bpix_eff","drhit_drcl_neweff_bpix_eff",200,0.0,2.0,100,0.0,0.1);
    drhit_drcl_neweff_bpix_dist = new TH2D("drhit_drcl_neweff_bpix_dist","drhit_drcl_neweff_bpix_dist",200,0.0,2.0,100,0.0,0.1);
    drhit_drcl_neweff_fpix_eff = new TH2D("drhit_drcl_neweff_fpix_eff","drhit_drcl_neweff_fpix_eff",200,0.0,2.0,100,0.0,0.1);
    drhit_drcl_neweff_fpix_dist = new TH2D("drhit_drcl_neweff_fpix_dist","drhit_drcl_neweff_fpix_dist",200,0.0,2.0,100,0.0,0.1);
  
    drhit_l1_eff = new TH1D("drhit_l1_eff","drhit_l1_eff",200,0.0,2.0);
    drhit_l1_dist = new TH1D("drhit_l1_dist","drhit_l1_dist",200,0.0,2.0);
    drhit_l2_eff = new TH1D("drhit_l2_eff","drhit_l2_eff",200,0.0,2.0);
    drhit_l2_dist = new TH1D("drhit_l2_dist","drhit_l2_dist",200,0.0,2.0);
    drhit_l3_eff = new TH1D("drhit_l3_eff","drhit_l3_eff",200,0.0,2.0);
    drhit_l3_dist = new TH1D("drhit_l3_dist","drhit_l3_dist",200,0.0,2.0);
    drhit_fpix_eff = new TH1D("drhit_fpix_eff","drhit_fpix_eff",200,0.0,2.0);
    drhit_fpix_dist = new TH1D("drhit_fpix_dist","drhit_fpix_dist",200,0.0,2.0);
    drhit_l1_cumeff = new TH1D("drhit_l1_cumeff","drhit_l1_cumeff",200,0.0,2.0);
    drhit_l1_cumdist = new TH1D("drhit_l1_cumdist","drhit_l1_cumdist",200,0.0,2.0);
    drhit_l2_cumeff = new TH1D("drhit_l2_cumeff","drhit_l2_cumeff",200,0.0,2.0);
    drhit_l2_cumdist = new TH1D("drhit_l2_cumdist","drhit_l2_cumdist",200,0.0,2.0);
    drhit_l3_cumeff = new TH1D("drhit_l3_cumeff","drhit_l3_cumeff",200,0.0,2.0);
    drhit_l3_cumdist = new TH1D("drhit_l3_cumdist","drhit_l3_cumdist",200,0.0,2.0);
    drhit_fpix_cumeff = new TH1D("drhit_fpix_cumeff","drhit_fpix_cumeff",200,0.0,2.0);
    drhit_fpix_cumdist = new TH1D("drhit_fpix_cumdist","drhit_fpix_cumdist",200,0.0,2.0);
  
    outerfid_l1_p_eff = new TH1D("outerfid_l1_p_eff","outerfid_l1_p_eff",640,-3.2,3.2);
    outerfid_l1_p_dist = new TH1D("outerfid_l1_p_dist","outerfid_l1_p_dist",640,-3.2,3.2);
    outerfid_l2_p_eff = new TH1D("outerfid_l2_p_eff","outerfid_l2_p_eff",640,-3.2,3.2);
    outerfid_l2_p_dist = new TH1D("outerfid_l2_p_dist","outerfid_l2_p_dist",640,-3.2,3.2);
    outerfid_l3_p_eff = new TH1D("outerfid_l3_p_eff","outerfid_l3_p_eff",640,-3.2,3.2);
    outerfid_l3_p_dist = new TH1D("outerfid_l3_p_dist","outerfid_l3_p_dist",640,-3.2,3.2);
    outerfid_l1_m_eff = new TH1D("outerfid_l1_m_eff","outerfid_l1_m_eff",640,-3.2,3.2);
    outerfid_l1_m_dist = new TH1D("outerfid_l1_m_dist","outerfid_l1_m_dist",640,-3.2,3.2);
    outerfid_l2_m_eff = new TH1D("outerfid_l2_m_eff","outerfid_l2_m_eff",640,-3.2,3.2);
    outerfid_l2_m_dist = new TH1D("outerfid_l2_m_dist","outerfid_l2_m_dist",640,-3.2,3.2);
    outerfid_l3_m_eff = new TH1D("outerfid_l3_m_eff","outerfid_l3_m_eff",640,-3.2,3.2);
    outerfid_l3_m_dist = new TH1D("outerfid_l3_m_dist","outerfid_l3_m_dist",640,-3.2,3.2);
  
    // Fiducial histograms
    // nroclx, nrocly, dimensions (1/2), roclevel (1/0)
      // ROC level binning (1/0)
    roclevel = 0;
  
    // Bpix
    // 2x8 modules
    // 2D
    // Binmaker: nroclx, nrocly, dimensions (1/2)
    binmaker(2, 8, 2);
    fid_l1_full_eff =  new TH2D("fid_l1_full_eff", "fid_l1_full_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_full_dist =  new TH2D("fid_l1_full_dist", "fid_l1_full_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_full_eff =  new TH2D("fid_l2_full_eff", "fid_l2_full_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_full_dist =  new TH2D("fid_l2_full_dist", "fid_l2_full_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_full_eff =  new TH2D("fid_l3_full_eff", "fid_l3_full_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_full_dist =  new TH2D("fid_l3_full_dist", "fid_l3_full_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    // 1D
    binmaker(2, 8, 1);
    fid_l1_full_lx_eff = new TH1D("fid_l1_full_lx_eff", "fid_l1_full_lx_eff", nbinsy, ylow, yup);
    fid_l1_full_lx_dist = new TH1D("fid_l1_full_lx_dist", "fid_l1_full_lx_dist", nbinsy, ylow, yup);
    fid_l1_full_ly_eff = new TH1D("fid_l1_full_ly_eff", "fid_l1_full_ly_eff", nbinsx, xlow, xup);
    fid_l1_full_ly_dist = new TH1D("fid_l1_full_ly_dist", "fid_l1_full_ly_dist", nbinsx, xlow, xup);
    fid_l2_full_lx_eff = new TH1D("fid_l2_full_lx_eff", "fid_l2_full_lx_eff", nbinsy, ylow, yup);
    fid_l2_full_lx_dist = new TH1D("fid_l2_full_lx_dist", "fid_l2_full_lx_dist", nbinsy, ylow, yup);
    fid_l2_full_ly_eff = new TH1D("fid_l2_full_ly_eff", "fid_l2_full_ly_eff", nbinsx, xlow, xup);
    fid_l2_full_ly_dist = new TH1D("fid_l2_full_ly_dist", "fid_l2_full_ly_dist", nbinsx, xlow, xup);
    fid_l3_full_lx_eff = new TH1D("fid_l3_full_lx_eff", "fid_l3_full_lx_eff", nbinsy, ylow, yup);
    fid_l3_full_lx_dist = new TH1D("fid_l3_full_lx_dist", "fid_l3_full_lx_dist", nbinsy, ylow, yup);
    fid_l3_full_ly_eff = new TH1D("fid_l3_full_ly_eff", "fid_l3_full_ly_eff", nbinsx, xlow, xup);
    fid_l3_full_ly_dist = new TH1D("fid_l3_full_ly_dist", "fid_l3_full_ly_dist", nbinsx, xlow, xup);
    // 1x8 modules
    binmaker(1, 8, 2);
    fid_l1_htI_eff =  new TH2D("fid_l1_htI_eff", "fid_l1_htI_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_htI_dist =  new TH2D("fid_l1_htI_dist", "fid_l1_htI_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_hbI_eff =  new TH2D("fid_l1_hbI_eff", "fid_l1_hbI_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_hbI_dist =  new TH2D("fid_l1_hbI_dist", "fid_l1_hbI_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_htO_eff =  new TH2D("fid_l1_htO_eff", "fid_l1_htO_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_htO_dist =  new TH2D("fid_l1_htO_dist", "fid_l1_htO_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_hbO_eff =  new TH2D("fid_l1_hbO_eff", "fid_l1_hbO_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l1_hbO_dist =  new TH2D("fid_l1_hbO_dist", "fid_l1_hbO_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_htI_eff =  new TH2D("fid_l2_htI_eff", "fid_l2_htI_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_htI_dist =  new TH2D("fid_l2_htI_dist", "fid_l2_htI_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_hbI_eff =  new TH2D("fid_l2_hbI_eff", "fid_l2_hbI_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_hbI_dist =  new TH2D("fid_l2_hbI_dist", "fid_l2_hbI_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_htO_eff =  new TH2D("fid_l2_htO_eff", "fid_l2_htO_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_htO_dist =  new TH2D("fid_l2_htO_dist", "fid_l2_htO_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_hbO_eff =  new TH2D("fid_l2_hbO_eff", "fid_l2_hbO_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l2_hbO_dist =  new TH2D("fid_l2_hbO_dist", "fid_l2_hbO_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_htI_eff =  new TH2D("fid_l3_htI_eff", "fid_l3_htI_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_htI_dist =  new TH2D("fid_l3_htI_dist", "fid_l3_htI_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_hbI_eff =  new TH2D("fid_l3_hbI_eff", "fid_l3_hbI_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_hbI_dist =  new TH2D("fid_l3_hbI_dist", "fid_l3_hbI_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_htO_eff =  new TH2D("fid_l3_htO_eff", "fid_l3_htO_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_htO_dist =  new TH2D("fid_l3_htO_dist", "fid_l3_htO_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_hbO_eff =  new TH2D("fid_l3_hbO_eff", "fid_l3_hbO_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_l3_hbO_dist =  new TH2D("fid_l3_hbO_dist", "fid_l3_hbO_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    binmaker(1, 8, 1);
    fid_l1_htI_lx_eff = new TH1D("fid_l1_htI_lx_eff", "fid_l1_htI_lx_eff", nbinsy, ylow, yup);
    fid_l1_htI_lx_dist = new TH1D("fid_l1_htI_lx_dist", "fid_l1_htI_lx_dist", nbinsy, ylow, yup);
    fid_l1_htI_ly_eff = new TH1D("fid_l1_htI_ly_eff", "fid_l1_htI_ly_eff", nbinsx, xlow, xup);
    fid_l1_htI_ly_dist = new TH1D("fid_l1_htI_ly_dist", "fid_l1_htI_ly_dist", nbinsx, xlow, xup);
    fid_l1_htO_lx_eff = new TH1D("fid_l1_htO_lx_eff", "fid_l1_htO_lx_eff", nbinsy, ylow, yup);
    fid_l1_htO_lx_dist = new TH1D("fid_l1_htO_lx_dist", "fid_l1_htO_lx_dist", nbinsy, ylow, yup);
    fid_l1_htO_ly_eff = new TH1D("fid_l1_htO_ly_eff", "fid_l1_htO_ly_eff", nbinsx, xlow, xup);
    fid_l1_htO_ly_dist = new TH1D("fid_l1_htO_ly_dist", "fid_l1_htO_ly_dist", nbinsx, xlow, xup);
    fid_l1_hbI_lx_eff = new TH1D("fid_l1_hbI_lx_eff", "fid_l1_hbI_lx_eff", nbinsy, ylow, yup);
    fid_l1_hbI_lx_dist = new TH1D("fid_l1_hbI_lx_dist", "fid_l1_hbI_lx_dist", nbinsy, ylow, yup);
    fid_l1_hbI_ly_eff = new TH1D("fid_l1_hbI_ly_eff", "fid_l1_hbI_ly_eff", nbinsx, xlow, xup);
    fid_l1_hbI_ly_dist = new TH1D("fid_l1_hbI_ly_dist", "fid_l1_hbI_ly_dist", nbinsx, xlow, xup);
    fid_l1_hbO_lx_eff = new TH1D("fid_l1_hbO_lx_eff", "fid_l1_hbO_lx_eff", nbinsy, ylow, yup);
    fid_l1_hbO_lx_dist = new TH1D("fid_l1_hbO_lx_dist", "fid_l1_hbO_lx_dist", nbinsy, ylow, yup);
    fid_l1_hbO_ly_eff = new TH1D("fid_l1_hbO_ly_eff", "fid_l1_hbO_ly_eff", nbinsx, xlow, xup);
    fid_l1_hbO_ly_dist = new TH1D("fid_l1_hbO_ly_dist", "fid_l1_hbO_ly_dist", nbinsx, xlow, xup);
    fid_l2_htI_lx_eff = new TH1D("fid_l2_htI_lx_eff", "fid_l2_htI_lx_eff", nbinsy, ylow, yup);
    fid_l2_htI_lx_dist = new TH1D("fid_l2_htI_lx_dist", "fid_l2_htI_lx_dist", nbinsy, ylow, yup);
    fid_l2_htI_ly_eff = new TH1D("fid_l2_htI_ly_eff", "fid_l2_htI_ly_eff", nbinsx, xlow, xup);
    fid_l2_htI_ly_dist = new TH1D("fid_l2_htI_ly_dist", "fid_l2_htI_ly_dist", nbinsx, xlow, xup);
    fid_l2_htO_lx_eff = new TH1D("fid_l2_htO_lx_eff", "fid_l2_htO_lx_eff", nbinsy, ylow, yup);
    fid_l2_htO_lx_dist = new TH1D("fid_l2_htO_lx_dist", "fid_l2_htO_lx_dist", nbinsy, ylow, yup);
    fid_l2_htO_ly_eff = new TH1D("fid_l2_htO_ly_eff", "fid_l2_htO_ly_eff", nbinsx, xlow, xup);
    fid_l2_htO_ly_dist = new TH1D("fid_l2_htO_ly_dist", "fid_l2_htO_ly_dist", nbinsx, xlow, xup);
    fid_l2_hbI_lx_eff = new TH1D("fid_l2_hbI_lx_eff", "fid_l2_hbI_lx_eff", nbinsy, ylow, yup);
    fid_l2_hbI_lx_dist = new TH1D("fid_l2_hbI_lx_dist", "fid_l2_hbI_lx_dist", nbinsy, ylow, yup);
    fid_l2_hbI_ly_eff = new TH1D("fid_l2_hbI_ly_eff", "fid_l2_hbI_ly_eff", nbinsx, xlow, xup);
    fid_l2_hbI_ly_dist = new TH1D("fid_l2_hbI_ly_dist", "fid_l2_hbI_ly_dist", nbinsx, xlow, xup);
    fid_l2_hbO_lx_eff = new TH1D("fid_l2_hbO_lx_eff", "fid_l2_hbO_lx_eff", nbinsy, ylow, yup);
    fid_l2_hbO_lx_dist = new TH1D("fid_l2_hbO_lx_dist", "fid_l2_hbO_lx_dist", nbinsy, ylow, yup);
    fid_l2_hbO_ly_eff = new TH1D("fid_l2_hbO_ly_eff", "fid_l2_hbO_ly_eff", nbinsx, xlow, xup);
    fid_l2_hbO_ly_dist = new TH1D("fid_l2_hbO_ly_dist", "fid_l2_hbO_ly_dist", nbinsx, xlow, xup);
    fid_l3_htI_lx_eff = new TH1D("fid_l3_htI_lx_eff", "fid_l3_htI_lx_eff", nbinsy, ylow, yup);
    fid_l3_htI_lx_dist = new TH1D("fid_l3_htI_lx_dist", "fid_l3_htI_lx_dist", nbinsy, ylow, yup);
    fid_l3_htI_ly_eff = new TH1D("fid_l3_htI_ly_eff", "fid_l3_htI_ly_eff", nbinsx, xlow, xup);
    fid_l3_htI_ly_dist = new TH1D("fid_l3_htI_ly_dist", "fid_l3_htI_ly_dist", nbinsx, xlow, xup);
    fid_l3_htO_lx_eff = new TH1D("fid_l3_htO_lx_eff", "fid_l3_htO_lx_eff", nbinsy, ylow, yup);
    fid_l3_htO_lx_dist = new TH1D("fid_l3_htO_lx_dist", "fid_l3_htO_lx_dist", nbinsy, ylow, yup);
    fid_l3_htO_ly_eff = new TH1D("fid_l3_htO_ly_eff", "fid_l3_htO_ly_eff", nbinsx, xlow, xup);
    fid_l3_htO_ly_dist = new TH1D("fid_l3_htO_ly_dist", "fid_l3_htO_ly_dist", nbinsx, xlow, xup);
    fid_l3_hbI_lx_eff = new TH1D("fid_l3_hbI_lx_eff", "fid_l3_hbI_lx_eff", nbinsy, ylow, yup);
    fid_l3_hbI_lx_dist = new TH1D("fid_l3_hbI_lx_dist", "fid_l3_hbI_lx_dist", nbinsy, ylow, yup);
    fid_l3_hbI_ly_eff = new TH1D("fid_l3_hbI_ly_eff", "fid_l3_hbI_ly_eff", nbinsx, xlow, xup);
    fid_l3_hbI_ly_dist = new TH1D("fid_l3_hbI_ly_dist", "fid_l3_hbI_ly_dist", nbinsx, xlow, xup);
    fid_l3_hbO_lx_eff = new TH1D("fid_l3_hbO_lx_eff", "fid_l3_hbO_lx_eff", nbinsy, ylow, yup);
    fid_l3_hbO_lx_dist = new TH1D("fid_l3_hbO_lx_dist", "fid_l3_hbO_lx_dist", nbinsy, ylow, yup);
    fid_l3_hbO_ly_eff = new TH1D("fid_l3_hbO_ly_eff", "fid_l3_hbO_ly_eff", nbinsx, xlow, xup);
    fid_l3_hbO_ly_dist = new TH1D("fid_l3_hbO_ly_dist", "fid_l3_hbO_ly_dist", nbinsx, xlow, xup);
    // Fpix
    // 1x2 modules
    binmaker(1, 2, 2);
    fid_dm2_p1_m1_eff =  new TH2D("fid_dm2_p1_m1_eff", "fid_dm2_p1_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p1_m1_dist =  new TH2D("fid_dm2_p1_m1_dist", "fid_dm2_p1_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m1_eff =  new TH2D("fid_dm1_p1_m1_eff", "fid_dm1_p1_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m1_dist =  new TH2D("fid_dm1_p1_m1_dist", "fid_dm1_p1_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m1_eff =  new TH2D("fid_dp1_p1_m1_eff", "fid_dp1_p1_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m1_dist =  new TH2D("fid_dp1_p1_m1_dist", "fid_dp1_p1_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m1_eff =  new TH2D("fid_dp2_p1_m1_eff", "fid_dp2_p1_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m1_dist =  new TH2D("fid_dp2_p1_m1_dist", "fid_dp2_p1_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    binmaker(1, 2, 1);
    fid_dm2_p1_m1_lx_eff = new TH1D("fid_dm2_p1_m1_lx_eff", "fid_dm2_p1_m1_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p1_m1_lx_dist = new TH1D("fid_dm2_p1_m1_lx_dist", "fid_dm2_p1_m1_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p1_m1_ly_eff = new TH1D("fid_dm2_p1_m1_ly_eff", "fid_dm2_p1_m1_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p1_m1_ly_dist = new TH1D("fid_dm2_p1_m1_ly_dist", "fid_dm2_p1_m1_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p1_m1_lx_eff = new TH1D("fid_dm1_p1_m1_lx_eff", "fid_dm1_p1_m1_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p1_m1_lx_dist = new TH1D("fid_dm1_p1_m1_lx_dist", "fid_dm1_p1_m1_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p1_m1_ly_eff = new TH1D("fid_dm1_p1_m1_ly_eff", "fid_dm1_p1_m1_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p1_m1_ly_dist = new TH1D("fid_dm1_p1_m1_ly_dist", "fid_dm1_p1_m1_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p1_m1_lx_eff = new TH1D("fid_dp1_p1_m1_lx_eff", "fid_dp1_p1_m1_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p1_m1_lx_dist = new TH1D("fid_dp1_p1_m1_lx_dist", "fid_dp1_p1_m1_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p1_m1_ly_eff = new TH1D("fid_dp1_p1_m1_ly_eff", "fid_dp1_p1_m1_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p1_m1_ly_dist = new TH1D("fid_dp1_p1_m1_ly_dist", "fid_dp1_p1_m1_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p1_m1_lx_eff = new TH1D("fid_dp2_p1_m1_lx_eff", "fid_dp2_p1_m1_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p1_m1_lx_dist = new TH1D("fid_dp2_p1_m1_lx_dist", "fid_dp2_p1_m1_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p1_m1_ly_eff = new TH1D("fid_dp2_p1_m1_ly_eff", "fid_dp2_p1_m1_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p1_m1_ly_dist = new TH1D("fid_dp2_p1_m1_ly_dist", "fid_dp2_p1_m1_ly_dist", nbinsx, xlow, xup);
    // 2x3 modules
    binmaker(2, 3, 2);
    fid_dm2_p1_m2_eff =  new TH2D("fid_dm2_p1_m2_eff", "fid_dm2_p1_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p1_m2_dist =  new TH2D("fid_dm2_p1_m2_dist", "fid_dm2_p1_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p2_m1_eff =  new TH2D("fid_dm2_p2_m1_eff", "fid_dm2_p2_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p2_m1_dist =  new TH2D("fid_dm2_p2_m1_dist", "fid_dm2_p2_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m2_eff =  new TH2D("fid_dm1_p1_m2_eff", "fid_dm1_p1_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m2_dist =  new TH2D("fid_dm1_p1_m2_dist", "fid_dm1_p1_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p2_m1_eff =  new TH2D("fid_dm1_p2_m1_eff", "fid_dm1_p2_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p2_m1_dist =  new TH2D("fid_dm1_p2_m1_dist", "fid_dm1_p2_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m2_eff =  new TH2D("fid_dp1_p1_m2_eff", "fid_dp1_p1_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m2_dist =  new TH2D("fid_dp1_p1_m2_dist", "fid_dp1_p1_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p2_m1_eff =  new TH2D("fid_dp1_p2_m1_eff", "fid_dp1_p2_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p2_m1_dist =  new TH2D("fid_dp1_p2_m1_dist", "fid_dp1_p2_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m2_eff =  new TH2D("fid_dp2_p1_m2_eff", "fid_dp2_p1_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m2_dist =  new TH2D("fid_dp2_p1_m2_dist", "fid_dp2_p1_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p2_m1_eff =  new TH2D("fid_dp2_p2_m1_eff", "fid_dp2_p2_m1_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p2_m1_dist =  new TH2D("fid_dp2_p2_m1_dist", "fid_dp2_p2_m1_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    binmaker(2, 3, 1);
    fid_dm2_p1_m2_lx_eff = new TH1D("fid_dm2_p1_m2_lx_eff", "fid_dm2_p1_m2_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p1_m2_lx_dist = new TH1D("fid_dm2_p1_m2_lx_dist", "fid_dm2_p1_m2_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p1_m2_ly_eff = new TH1D("fid_dm2_p1_m2_ly_eff", "fid_dm2_p1_m2_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p1_m2_ly_dist = new TH1D("fid_dm2_p1_m2_ly_dist", "fid_dm2_p1_m2_ly_dist", nbinsx, xlow, xup);
    fid_dm2_p2_m1_lx_eff = new TH1D("fid_dm2_p2_m1_lx_eff", "fid_dm2_p2_m1_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p2_m1_lx_dist = new TH1D("fid_dm2_p2_m1_lx_dist", "fid_dm2_p2_m1_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p2_m1_ly_eff = new TH1D("fid_dm2_p2_m1_ly_eff", "fid_dm2_p2_m1_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p2_m1_ly_dist = new TH1D("fid_dm2_p2_m1_ly_dist", "fid_dm2_p2_m1_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p1_m2_lx_eff = new TH1D("fid_dm1_p1_m2_lx_eff", "fid_dm1_p1_m2_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p1_m2_lx_dist = new TH1D("fid_dm1_p1_m2_lx_dist", "fid_dm1_p1_m2_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p1_m2_ly_eff = new TH1D("fid_dm1_p1_m2_ly_eff", "fid_dm1_p1_m2_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p1_m2_ly_dist = new TH1D("fid_dm1_p1_m2_ly_dist", "fid_dm1_p1_m2_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p2_m1_lx_eff = new TH1D("fid_dm1_p2_m1_lx_eff", "fid_dm1_p2_m1_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p2_m1_lx_dist = new TH1D("fid_dm1_p2_m1_lx_dist", "fid_dm1_p2_m1_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p2_m1_ly_eff = new TH1D("fid_dm1_p2_m1_ly_eff", "fid_dm1_p2_m1_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p2_m1_ly_dist = new TH1D("fid_dm1_p2_m1_ly_dist", "fid_dm1_p2_m1_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p1_m2_lx_eff = new TH1D("fid_dp1_p1_m2_lx_eff", "fid_dp1_p1_m2_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p1_m2_lx_dist = new TH1D("fid_dp1_p1_m2_lx_dist", "fid_dp1_p1_m2_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p1_m2_ly_eff = new TH1D("fid_dp1_p1_m2_ly_eff", "fid_dp1_p1_m2_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p1_m2_ly_dist = new TH1D("fid_dp1_p1_m2_ly_dist", "fid_dp1_p1_m2_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p2_m1_lx_eff = new TH1D("fid_dp1_p2_m1_lx_eff", "fid_dp1_p2_m1_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p2_m1_lx_dist = new TH1D("fid_dp1_p2_m1_lx_dist", "fid_dp1_p2_m1_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p2_m1_ly_eff = new TH1D("fid_dp1_p2_m1_ly_eff", "fid_dp1_p2_m1_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p2_m1_ly_dist = new TH1D("fid_dp1_p2_m1_ly_dist", "fid_dp1_p2_m1_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p1_m2_lx_eff = new TH1D("fid_dp2_p1_m2_lx_eff", "fid_dp2_p1_m2_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p1_m2_lx_dist = new TH1D("fid_dp2_p1_m2_lx_dist", "fid_dp2_p1_m2_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p1_m2_ly_eff = new TH1D("fid_dp2_p1_m2_ly_eff", "fid_dp2_p1_m2_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p1_m2_ly_dist = new TH1D("fid_dp2_p1_m2_ly_dist", "fid_dp2_p1_m2_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p2_m1_lx_eff = new TH1D("fid_dp2_p2_m1_lx_eff", "fid_dp2_p2_m1_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p2_m1_lx_dist = new TH1D("fid_dp2_p2_m1_lx_dist", "fid_dp2_p2_m1_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p2_m1_ly_eff = new TH1D("fid_dp2_p2_m1_ly_eff", "fid_dp2_p2_m1_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p2_m1_ly_dist = new TH1D("fid_dp2_p2_m1_ly_dist", "fid_dp2_p2_m1_ly_dist", nbinsx, xlow, xup);
    // 2x4 modules
    binmaker(2, 4, 2);
    fid_dm2_p1_m3_eff =  new TH2D("fid_dm2_p1_m3_eff", "fid_dm2_p1_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p1_m3_dist =  new TH2D("fid_dm2_p1_m3_dist", "fid_dm2_p1_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m3_eff =  new TH2D("fid_dm1_p1_m3_eff", "fid_dm1_p1_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m3_dist =  new TH2D("fid_dm1_p1_m3_dist", "fid_dm1_p1_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m3_eff =  new TH2D("fid_dp1_p1_m3_eff", "fid_dp1_p1_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m3_dist =  new TH2D("fid_dp1_p1_m3_dist", "fid_dp1_p1_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m3_eff =  new TH2D("fid_dp2_p1_m3_eff", "fid_dp2_p1_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m3_dist =  new TH2D("fid_dp2_p1_m3_dist", "fid_dp2_p1_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p2_m2_eff =  new TH2D("fid_dm2_p2_m2_eff", "fid_dm2_p2_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p2_m2_dist =  new TH2D("fid_dm2_p2_m2_dist", "fid_dm2_p2_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p2_m2_eff =  new TH2D("fid_dm1_p2_m2_eff", "fid_dm1_p2_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p2_m2_dist =  new TH2D("fid_dm1_p2_m2_dist", "fid_dm1_p2_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p2_m2_eff =  new TH2D("fid_dp1_p2_m2_eff", "fid_dp1_p2_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p2_m2_dist =  new TH2D("fid_dp1_p2_m2_dist", "fid_dp1_p2_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p2_m2_eff =  new TH2D("fid_dp2_p2_m2_eff", "fid_dp2_p2_m2_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p2_m2_dist =  new TH2D("fid_dp2_p2_m2_dist", "fid_dp2_p2_m2_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    binmaker(2, 4, 1);
    fid_dm2_p1_m3_lx_eff = new TH1D("fid_dm2_p1_m3_lx_eff", "fid_dm2_p1_m3_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p1_m3_lx_dist = new TH1D("fid_dm2_p1_m3_lx_dist", "fid_dm2_p1_m3_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p1_m3_ly_eff = new TH1D("fid_dm2_p1_m3_ly_eff", "fid_dm2_p1_m3_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p1_m3_ly_dist = new TH1D("fid_dm2_p1_m3_ly_dist", "fid_dm2_p1_m3_ly_dist", nbinsx, xlow, xup);
    fid_dm2_p2_m2_lx_eff = new TH1D("fid_dm2_p2_m2_lx_eff", "fid_dm2_p2_m2_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p2_m2_lx_dist = new TH1D("fid_dm2_p2_m2_lx_dist", "fid_dm2_p2_m2_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p2_m2_ly_eff = new TH1D("fid_dm2_p2_m2_ly_eff", "fid_dm2_p2_m2_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p2_m2_ly_dist = new TH1D("fid_dm2_p2_m2_ly_dist", "fid_dm2_p2_m2_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p1_m3_lx_eff = new TH1D("fid_dm1_p1_m3_lx_eff", "fid_dm1_p1_m3_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p1_m3_lx_dist = new TH1D("fid_dm1_p1_m3_lx_dist", "fid_dm1_p1_m3_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p1_m3_ly_eff = new TH1D("fid_dm1_p1_m3_ly_eff", "fid_dm1_p1_m3_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p1_m3_ly_dist = new TH1D("fid_dm1_p1_m3_ly_dist", "fid_dm1_p1_m3_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p2_m2_lx_eff = new TH1D("fid_dm1_p2_m2_lx_eff", "fid_dm1_p2_m2_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p2_m2_lx_dist = new TH1D("fid_dm1_p2_m2_lx_dist", "fid_dm1_p2_m2_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p2_m2_ly_eff = new TH1D("fid_dm1_p2_m2_ly_eff", "fid_dm1_p2_m2_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p2_m2_ly_dist = new TH1D("fid_dm1_p2_m2_ly_dist", "fid_dm1_p2_m2_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p1_m3_lx_eff = new TH1D("fid_dp1_p1_m3_lx_eff", "fid_dp1_p1_m3_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p1_m3_lx_dist = new TH1D("fid_dp1_p1_m3_lx_dist", "fid_dp1_p1_m3_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p1_m3_ly_eff = new TH1D("fid_dp1_p1_m3_ly_eff", "fid_dp1_p1_m3_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p1_m3_ly_dist = new TH1D("fid_dp1_p1_m3_ly_dist", "fid_dp1_p1_m3_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p2_m2_lx_eff = new TH1D("fid_dp1_p2_m2_lx_eff", "fid_dp1_p2_m2_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p2_m2_lx_dist = new TH1D("fid_dp1_p2_m2_lx_dist", "fid_dp1_p2_m2_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p2_m2_ly_eff = new TH1D("fid_dp1_p2_m2_ly_eff", "fid_dp1_p2_m2_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p2_m2_ly_dist = new TH1D("fid_dp1_p2_m2_ly_dist", "fid_dp1_p2_m2_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p1_m3_lx_eff = new TH1D("fid_dp2_p1_m3_lx_eff", "fid_dp2_p1_m3_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p1_m3_lx_dist = new TH1D("fid_dp2_p1_m3_lx_dist", "fid_dp2_p1_m3_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p1_m3_ly_eff = new TH1D("fid_dp2_p1_m3_ly_eff", "fid_dp2_p1_m3_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p1_m3_ly_dist = new TH1D("fid_dp2_p1_m3_ly_dist", "fid_dp2_p1_m3_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p2_m2_lx_eff = new TH1D("fid_dp2_p2_m2_lx_eff", "fid_dp2_p2_m2_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p2_m2_lx_dist = new TH1D("fid_dp2_p2_m2_lx_dist", "fid_dp2_p2_m2_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p2_m2_ly_eff = new TH1D("fid_dp2_p2_m2_ly_eff", "fid_dp2_p2_m2_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p2_m2_ly_dist = new TH1D("fid_dp2_p2_m2_ly_dist", "fid_dp2_p2_m2_ly_dist", nbinsx, xlow, xup);
    // 1x5 modules
    binmaker(1, 5, 2);
    fid_dm2_p1_m4_eff =  new TH2D("fid_dm2_p1_m4_eff", "fid_dm2_p1_m4_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p1_m4_dist =  new TH2D("fid_dm2_p1_m4_dist", "fid_dm2_p1_m4_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m4_eff =  new TH2D("fid_dm1_p1_m4_eff", "fid_dm1_p1_m4_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p1_m4_dist =  new TH2D("fid_dm1_p1_m4_dist", "fid_dm1_p1_m4_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m4_eff =  new TH2D("fid_dp1_p1_m4_eff", "fid_dp1_p1_m4_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p1_m4_dist =  new TH2D("fid_dp1_p1_m4_dist", "fid_dp1_p1_m4_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m4_eff =  new TH2D("fid_dp2_p1_m4_eff", "fid_dp2_p1_m4_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p1_m4_dist =  new TH2D("fid_dp2_p1_m4_dist", "fid_dp2_p1_m4_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    binmaker(1, 5, 1);
    fid_dm2_p1_m4_lx_eff = new TH1D("fid_dm2_p1_m4_lx_eff", "fid_dm2_p1_m4_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p1_m4_lx_dist = new TH1D("fid_dm2_p1_m4_lx_dist", "fid_dm2_p1_m4_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p1_m4_ly_eff = new TH1D("fid_dm2_p1_m4_ly_eff", "fid_dm2_p1_m4_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p1_m4_ly_dist = new TH1D("fid_dm2_p1_m4_ly_dist", "fid_dm2_p1_m4_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p1_m4_lx_eff = new TH1D("fid_dm1_p1_m4_lx_eff", "fid_dm1_p1_m4_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p1_m4_lx_dist = new TH1D("fid_dm1_p1_m4_lx_dist", "fid_dm1_p1_m4_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p1_m4_ly_eff = new TH1D("fid_dm1_p1_m4_ly_eff", "fid_dm1_p1_m4_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p1_m4_ly_dist = new TH1D("fid_dm1_p1_m4_ly_dist", "fid_dm1_p1_m4_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p1_m4_lx_eff = new TH1D("fid_dp1_p1_m4_lx_eff", "fid_dp1_p1_m4_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p1_m4_lx_dist = new TH1D("fid_dp1_p1_m4_lx_dist", "fid_dp1_p1_m4_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p1_m4_ly_eff = new TH1D("fid_dp1_p1_m4_ly_eff", "fid_dp1_p1_m4_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p1_m4_ly_dist = new TH1D("fid_dp1_p1_m4_ly_dist", "fid_dp1_p1_m4_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p1_m4_lx_eff = new TH1D("fid_dp2_p1_m4_lx_eff", "fid_dp2_p1_m4_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p1_m4_lx_dist = new TH1D("fid_dp2_p1_m4_lx_dist", "fid_dp2_p1_m4_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p1_m4_ly_eff = new TH1D("fid_dp2_p1_m4_ly_eff", "fid_dp2_p1_m4_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p1_m4_ly_dist = new TH1D("fid_dp2_p1_m4_ly_dist", "fid_dp2_p1_m4_ly_dist", nbinsx, xlow, xup);
    // 2x5 modules
    binmaker(2, 5, 2);
    fid_dm2_p2_m3_eff =  new TH2D("fid_dm2_p2_m3_eff", "fid_dm2_p2_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm2_p2_m3_dist =  new TH2D("fid_dm2_p2_m3_dist", "fid_dm2_p2_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p2_m3_eff =  new TH2D("fid_dm1_p2_m3_eff", "fid_dm1_p2_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dm1_p2_m3_dist =  new TH2D("fid_dm1_p2_m3_dist", "fid_dm1_p2_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p2_m3_eff =  new TH2D("fid_dp1_p2_m3_eff", "fid_dp1_p2_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp1_p2_m3_dist =  new TH2D("fid_dp1_p2_m3_dist", "fid_dp1_p2_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p2_m3_eff =  new TH2D("fid_dp2_p2_m3_eff", "fid_dp2_p2_m3_eff", nbinsx, xlow, xup, nbinsy, ylow, yup);
    fid_dp2_p2_m3_dist =  new TH2D("fid_dp2_p2_m3_dist", "fid_dp2_p2_m3_dist", nbinsx, xlow, xup, nbinsy, ylow, yup);
    binmaker(2, 5, 1);
    fid_dm2_p2_m3_lx_eff = new TH1D("fid_dm2_p2_m3_lx_eff", "fid_dm2_p2_m3_lx_eff", nbinsy, ylow, yup);
    fid_dm2_p2_m3_lx_dist = new TH1D("fid_dm2_p2_m3_lx_dist", "fid_dm2_p2_m3_lx_dist", nbinsy, ylow, yup);
    fid_dm2_p2_m3_ly_eff = new TH1D("fid_dm2_p2_m3_ly_eff", "fid_dm2_p2_m3_ly_eff", nbinsx, xlow, xup);
    fid_dm2_p2_m3_ly_dist = new TH1D("fid_dm2_p2_m3_ly_dist", "fid_dm2_p2_m3_ly_dist", nbinsx, xlow, xup);
    fid_dm1_p2_m3_lx_eff = new TH1D("fid_dm1_p2_m3_lx_eff", "fid_dm1_p2_m3_lx_eff", nbinsy, ylow, yup);
    fid_dm1_p2_m3_lx_dist = new TH1D("fid_dm1_p2_m3_lx_dist", "fid_dm1_p2_m3_lx_dist", nbinsy, ylow, yup);
    fid_dm1_p2_m3_ly_eff = new TH1D("fid_dm1_p2_m3_ly_eff", "fid_dm1_p2_m3_ly_eff", nbinsx, xlow, xup);
    fid_dm1_p2_m3_ly_dist = new TH1D("fid_dm1_p2_m3_ly_dist", "fid_dm1_p2_m3_ly_dist", nbinsx, xlow, xup);
    fid_dp1_p2_m3_lx_eff = new TH1D("fid_dp1_p2_m3_lx_eff", "fid_dp1_p2_m3_lx_eff", nbinsy, ylow, yup);
    fid_dp1_p2_m3_lx_dist = new TH1D("fid_dp1_p2_m3_lx_dist", "fid_dp1_p2_m3_lx_dist", nbinsy, ylow, yup);
    fid_dp1_p2_m3_ly_eff = new TH1D("fid_dp1_p2_m3_ly_eff", "fid_dp1_p2_m3_ly_eff", nbinsx, xlow, xup);
    fid_dp1_p2_m3_ly_dist = new TH1D("fid_dp1_p2_m3_ly_dist", "fid_dp1_p2_m3_ly_dist", nbinsx, xlow, xup);
    fid_dp2_p2_m3_lx_eff = new TH1D("fid_dp2_p2_m3_lx_eff", "fid_dp2_p2_m3_lx_eff", nbinsy, ylow, yup);
    fid_dp2_p2_m3_lx_dist = new TH1D("fid_dp2_p2_m3_lx_dist", "fid_dp2_p2_m3_lx_dist", nbinsy, ylow, yup);
    fid_dp2_p2_m3_ly_eff = new TH1D("fid_dp2_p2_m3_ly_eff", "fid_dp2_p2_m3_ly_eff", nbinsx, xlow, xup);
    fid_dp2_p2_m3_ly_dist = new TH1D("fid_dp2_p2_m3_ly_dist", "fid_dp2_p2_m3_ly_dist", nbinsx, xlow, xup);

    if (pcatomki==0) {
      if (fullrun==1) {
	// COMPLETE
	filedir = "/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/finedelay/";

	fill_histo("MB_Run2011A_RECO_001.root");
	fill_histo("MB_Run2011A_RECO_002.root");
	fill_histo("MB_Run2011A_RECO_003.root");
	fill_histo("MB_Run2011A_RECO_004.root");
	fill_histo("MB_Run2011A_RECO_005.root");
	fill_histo("MB_Run2011A_RECO_006.root");
	fill_histo("MB_Run2011A_RECO_007.root");
	fill_histo("MB_Run2011A_RECO_008.root");
	fill_histo("MB_Run2011A_RECO_009.root");
	fill_histo("MB_Run2011A_RECO_010.root");
	fill_histo("MB_Run2011A_RECO_011.root");
	fill_histo("MB_Run2011A_RECO_012.root");
	fill_histo("MB_Run2011A_RECO_013.root");
	fill_histo("MB_Run2011A_RECO_014.root");
	fill_histo("MB_Run2011A_RECO_015.root");
	fill_histo("MB_Run2011A_RECO_016.root");
	fill_histo("MB_Run2011A_RECO_017.root");
	fill_histo("MB_Run2011A_RECO_018.root");
	fill_histo("MB_Run2011A_RECO_019.root");
	fill_histo("MB_Run2011A_RECO_020.root");
	fill_histo("MB_Run2011A_RECO_021.root");
	fill_histo("MB_Run2011A_RECO_022.root");
	fill_histo("MB_Run2011A_RECO_023.root");
	fill_histo("MB_Run2011A_RECO_024.root");
	fill_histo("MB_Run2011A_RECO_025.root");
	fill_histo("MB_Run2011A_RECO_026.root");
	fill_histo("MB_Run2011A_RECO_027.root");
	fill_histo("MB_Run2011A_RECO_028.root");
	fill_histo("MB_Run2011A_RECO_029.root");
	fill_histo("MB_Run2011A_RECO_030.root");
	fill_histo("MB_Run2011A_RECO_031.root");
	fill_histo("MB_Run2011A_RECO_032.root");
	fill_histo("MB_Run2011A_RECO_033.root");
	fill_histo("MB_Run2011A_RECO_034.root");
	fill_histo("MB_Run2011A_RECO_035.root");
	fill_histo("MB_Run2011A_RECO_036.root");
	fill_histo("MB_Run2011A_RECO_037.root");
	fill_histo("MB_Run2011A_RECO_038.root");
	fill_histo("MB_Run2011A_RECO_039.root");
	fill_histo("MB_Run2011A_RECO_040.root");
	fill_histo("MB_Run2011A_RECO_041.root");
	fill_histo("MB_Run2011A_RECO_042.root");
	fill_histo("MB_Run2011A_RECO_043.root");
	fill_histo("MB_Run2011A_RECO_044.root");
	fill_histo("MB_Run2011A_RECO_045.root");
	fill_histo("MB_Run2011A_RECO_046.root");
	fill_histo("MB_Run2011A_RECO_047.root");
	fill_histo("MB_Run2011A_RECO_048.root");
	fill_histo("MB_Run2011A_RECO_049.root");
	fill_histo("MB_Run2011A_RECO_050.root");
	fill_histo("MB_Run2011A_RECO_051.root");
	fill_histo("MB_Run2011A_RECO_052.root");
	fill_histo("MB_Run2011A_RECO_053.root");
	fill_histo("MB_Run2011A_RECO_054.root");
	fill_histo("MB_Run2011A_RECO_055.root");
	fill_histo("MB_Run2011A_RECO_056.root");
	fill_histo("MB_Run2011A_RECO_057.root");
	fill_histo("MB_Run2011A_RECO_058.root");
	fill_histo("MB_Run2011A_RECO_059.root");
	fill_histo("MB_Run2011A_RECO_060.root");
	fill_histo("MB_Run2011A_RECO_061.root");
	fill_histo("MB_Run2011A_RECO_062.root");
	fill_histo("MB_Run2011A_RECO_063.root");
	fill_histo("MB_Run2011A_RECO_064.root");
	fill_histo("MB_Run2011A_RECO_065.root");
	fill_histo("MB_Run2011A_RECO_066.root");
	fill_histo("MB_Run2011A_RECO_067.root");
	fill_histo("MB_Run2011A_RECO_068.root");
	fill_histo("MB_Run2011A_RECO_069.root");
	fill_histo("MB_Run2011A_RECO_070.root");
	fill_histo("MB_Run2011A_RECO_071.root");
	fill_histo("MB_Run2011A_RECO_072.root");
	fill_histo("MB_Run2011A_RECO_073.root");
	fill_histo("MB_Run2011A_RECO_074.root");
	fill_histo("MB_Run2011A_RECO_075.root");
	fill_histo("MB_Run2011A_RECO_076.root");
	fill_histo("MB_Run2011A_RECO_077.root");
	fill_histo("MB_Run2011A_RECO_078.root");
	fill_histo("MB_Run2011A_RECO_079.root");
	fill_histo("MB_Run2011A_RECO_080.root");
	fill_histo("MB_Run2011A_RECO_081.root");
	fill_histo("MB_Run2011A_RECO_082.root");
	fill_histo("MB_Run2011A_RECO_083.root");
	fill_histo("MB_Run2011A_RECO_084.root");
	fill_histo("MB_Run2011A_RECO_085.root");
	fill_histo("MB_Run2011A_RECO_086.root");
	fill_histo("MB_Run2011A_RECO_087.root");
	fill_histo("MB_Run2011A_RECO_088.root");
	fill_histo("MB_Run2011A_RECO_089.root");
	fill_histo("MB_Run2011A_RECO_090.root");
	fill_histo("MB_Run2011A_RECO_091.root");
	fill_histo("MB_Run2011A_RECO_092.root");
	fill_histo("MB_Run2011A_RECO_093.root");
	fill_histo("MB_Run2011A_RECO_094.root");
	fill_histo("MB_Run2011A_RECO_095.root");
	fill_histo("MB_Run2011A_RECO_096.root");
	fill_histo("MB_Run2011A_RECO_097.root");
	fill_histo("MB_Run2011A_RECO_098.root");
	fill_histo("MB_Run2011A_RECO_099.root");
	fill_histo("MB_Run2011A_RECO_100.root");
	fill_histo("MB_Run2011A_RECO_101.root");
	fill_histo("MB_Run2011A_RECO_102.root");
	fill_histo("MB_Run2011A_RECO_103.root");
	fill_histo("MB_Run2011A_RECO_104.root");
	fill_histo("MB_Run2011A_RECO_105.root");
	fill_histo("MB_Run2011A_RECO_106.root");
	fill_histo("MB_Run2011A_RECO_107.root");
	fill_histo("MB_Run2011A_RECO_108.root");
	fill_histo("MB_Run2011A_RECO_109.root");
	fill_histo("MB_Run2011A_RECO_110.root");
	fill_histo("MB_Run2011A_RECO_111.root");
	fill_histo("MB_Run2011A_RECO_112.root");
	fill_histo("MB_Run2011A_RECO_113.root");
	fill_histo("MB_Run2011A_RECO_114.root");
	fill_histo("MB_Run2011A_RECO_115.root");
	fill_histo("MB_Run2011A_RECO_116.root");
	fill_histo("MB_Run2011A_RECO_117.root");
	fill_histo("MB_Run2011A_RECO_118.root");
	fill_histo("MB_Run2011A_RECO_119.root");
	fill_histo("MB_Run2011A_RECO_120.root");
	fill_histo("MB_Run2011A_RECO_121.root");
	fill_histo("MB_Run2011A_RECO_122.root");
	fill_histo("MB_Run2011A_RECO_123.root");
	fill_histo("MB_Run2011A_RECO_124.root");
	fill_histo("MB_Run2011A_RECO_125.root");
	fill_histo("MB_Run2011A_RECO_126.root");
	fill_histo("MB_Run2011A_RECO_127.root");
	fill_histo("MB_Run2011A_RECO_128.root");
	fill_histo("MB_Run2011A_RECO_129.root");
	fill_histo("MB_Run2011A_RECO_130.root");
	fill_histo("MB_Run2011A_RECO_131.root");
	fill_histo("MB_Run2011A_RECO_132.root");
	fill_histo("MB_Run2011A_RECO_133.root");
	fill_histo("MB_Run2011A_RECO_134.root");
	fill_histo("MB_Run2011A_RECO_135.root");
	fill_histo("MB_Run2011A_RECO_136.root");
	fill_histo("MB_Run2011A_RECO_137.root");
	fill_histo("MB_Run2011A_RECO_138.root");
	fill_histo("MB_Run2011A_RECO_139.root");
	fill_histo("MB_Run2011A_RECO_140.root");
	fill_histo("MB_Run2011A_RECO_141.root");
	fill_histo("MB_Run2011A_RECO_142.root");
	fill_histo("MB_Run2011A_RECO_143.root");
	fill_histo("MB_Run2011A_RECO_144.root");
	fill_histo("MB_Run2011A_RECO_145.root");
	fill_histo("MB_Run2011A_RECO_146.root");
	fill_histo("MB_Run2011A_RECO_147.root");
	fill_histo("MB_Run2011A_RECO_148.root");
	fill_histo("MB_Run2011A_RECO_149.root");
	fill_histo("MB_Run2011A_RECO_150.root");
	fill_histo("MB_Run2011A_RECO_151.root");
	fill_histo("MB_Run2011A_RECO_152.root");
	fill_histo("MB_Run2011A_RECO_153.root");
	fill_histo("MB_Run2011A_RECO_154.root");
	fill_histo("MB_Run2011A_RECO_155.root");
	fill_histo("MB_Run2011A_RECO_156.root");
	fill_histo("MB_Run2011A_RECO_157.root");
	fill_histo("MB_Run2011A_RECO_158.root");
	fill_histo("MB_Run2011A_RECO_159.root");
	fill_histo("MB_Run2011A_RECO_160.root");
	fill_histo("MB_Run2011A_RECO_161.root");
	fill_histo("MB_Run2011A_RECO_162.root");
	fill_histo("MB_Run2011A_RECO_163.root");
	fill_histo("MB_Run2011A_RECO_164.root");
	fill_histo("MB_Run2011A_RECO_165.root");
	fill_histo("MB_Run2011A_RECO_166.root");
	fill_histo("MB_Run2011A_RECO_167.root");
	fill_histo("MB_Run2011A_RECO_168.root");
	fill_histo("MB_Run2011A_RECO_169.root");
	fill_histo("MB_Run2011A_RECO_170.root");
	fill_histo("MB_Run2011A_RECO_171.root");
	fill_histo("MB_Run2011A_RECO_172.root");
	fill_histo("MB_Run2011A_RECO_173.root");
	fill_histo("MB_Run2011A_RECO_174.root");
	fill_histo("MB_Run2011A_RECO_175.root");
	fill_histo("MB_Run2011A_RECO_176.root");
	fill_histo("MB_Run2011A_RECO_177.root");
	fill_histo("MB_Run2011A_RECO_178.root");
	fill_histo("MB_Run2011A_RECO_179.root");
	fill_histo("MB_Run2011A_RECO_180.root");
	fill_histo("MB_Run2011A_RECO_181.root");
	fill_histo("MB_Run2011A_RECO_182.root");
	fill_histo("MB_Run2011A_RECO_183.root");
	fill_histo("MB_Run2011A_RECO_184.root");
	fill_histo("MB_Run2011A_RECO_185.root");
	fill_histo("MB_Run2011A_RECO_186.root");
	fill_histo("MB_Run2011A_RECO_187.root");
	fill_histo("MB_Run2011A_RECO_188.root");
	fill_histo("MB_Run2011A_RECO_189.root");
	fill_histo("MB_Run2011A_RECO_190.root");
	fill_histo("MB_Run2011A_RECO_191.root");
	fill_histo("MB_Run2011A_RECO_192.root");
	fill_histo("MB_Run2011A_RECO_193.root");
	fill_histo("MB_Run2011A_RECO_194.root");
	fill_histo("MB_Run2011A_RECO_195.root");
	fill_histo("MB_Run2011A_RECO_196.root");
	fill_histo("MB_Run2011A_RECO_197.root");
	fill_histo("MB_Run2011A_RECO_198.root");
	fill_histo("MB_Run2011A_RECO_199.root");
	fill_histo("MB_Run2011A_RECO_200.root");
	fill_histo("MB_Run2011A_RECO_201.root");
	fill_histo("MB_Run2011A_RECO_202.root");
	fill_histo("MB_Run2011A_RECO_203.root");
	fill_histo("MB_Run2011A_RECO_204.root");
	fill_histo("MB_Run2011A_RECO_205.root");
	fill_histo("MB_Run2011A_RECO_206.root");
	fill_histo("MB_Run2011A_RECO_207.root");
	fill_histo("MB_Run2011A_RECO_208.root");
	fill_histo("MB_Run2011A_RECO_209.root");
	fill_histo("MB_Run2011A_RECO_210.root");
	fill_histo("MB_Run2011A_RECO_211.root");
	fill_histo("MB_Run2011A_RECO_212.root");
	fill_histo("MB_Run2011A_RECO_213.root");
	fill_histo("MB_Run2011A_RECO_214.root");
	fill_histo("MB_Run2011A_RECO_215.root");
	fill_histo("MB_Run2011A_RECO_216.root");
	fill_histo("MB_Run2011A_RECO_217.root");
	fill_histo("MB_Run2011A_RECO_218.root");
	fill_histo("MB_Run2011A_RECO_219.root");
	fill_histo("MB_Run2011A_RECO_220.root");
	fill_histo("MB_Run2011A_RECO_221.root");
	fill_histo("MB_Run2011A_RECO_222.root");
	fill_histo("MB_Run2011A_RECO_223.root");
	fill_histo("MB_Run2011A_RECO_224.root");
	fill_histo("MB_Run2011A_RECO_225.root");
	fill_histo("MB_Run2011A_RECO_226.root");
	fill_histo("MB_Run2011A_RECO_227.root");
	fill_histo("MB_Run2011A_RECO_228.root");
	fill_histo("MB_Run2011A_RECO_229.root");
	fill_histo("MB_Run2011A_RECO_230.root");
	fill_histo("MB_Run2011A_RECO_231.root");
	fill_histo("MB_Run2011A_RECO_232.root");
	fill_histo("MB_Run2011A_RECO_233.root");
	fill_histo("MB_Run2011A_RECO_234.root");
	fill_histo("MB_Run2011A_RECO_235.root");
	fill_histo("MB_Run2011A_RECO_236.root");
	fill_histo("MB_Run2011A_RECO_237.root");
	fill_histo("MB_Run2011A_RECO_238.root");
	fill_histo("MB_Run2011A_RECO_239.root");
	fill_histo("MB_Run2011A_RECO_240.root");
	fill_histo("MB_Run2011A_RECO_241.root");
	fill_histo("MB_Run2011A_RECO_242.root");
	fill_histo("MB_Run2011A_RECO_243.root");
	fill_histo("MB_Run2011A_RECO_244.root");
	fill_histo("MB_Run2011A_RECO_245.root");
	fill_histo("MB_Run2011A_RECO_246.root");
	fill_histo("MB_Run2011A_RECO_247.root");
	fill_histo("MB_Run2011A_RECO_248.root");
	fill_histo("MB_Run2011A_RECO_249.root");
	fill_histo("MB_Run2011A_RECO_250.root");
	fill_histo("MB_Run2011A_RECO_251.root");
	fill_histo("MB_Run2011A_RECO_252.root");
	fill_histo("MB_Run2011A_RECO_253.root");
	fill_histo("MB_Run2011A_RECO_254.root");
	fill_histo("MB_Run2011A_RECO_255.root");
	fill_histo("MB_Run2011A_RECO_256.root");
	fill_histo("MB_Run2011A_RECO_257.root");
	fill_histo("MB_Run2011A_RECO_258.root");
	fill_histo("MB_Run2011A_RECO_259.root");
	fill_histo("MB_Run2011A_RECO_260.root");
	fill_histo("MB_Run2011A_RECO_261.root");
	fill_histo("MB_Run2011A_RECO_262.root");
	fill_histo("MB_Run2011A_RECO_263.root");
	fill_histo("MB_Run2011A_RECO_264.root");

	filedir = "/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/restdelay/";

	fill_histo("MB_Run2011A_RECO_001.root");
	fill_histo("MB_Run2011A_RECO_002.root");
	fill_histo("MB_Run2011A_RECO_003.root");
	fill_histo("MB_Run2011A_RECO_004.root");
	fill_histo("MB_Run2011A_RECO_005.root");
	fill_histo("MB_Run2011A_RECO_006.root");
	fill_histo("MB_Run2011A_RECO_007.root");
	fill_histo("MB_Run2011A_RECO_008.root");
	fill_histo("MB_Run2011A_RECO_009.root");
	fill_histo("MB_Run2011A_RECO_010.root");
	fill_histo("MB_Run2011A_RECO_011.root");
	fill_histo("MB_Run2011A_RECO_012.root");
	fill_histo("MB_Run2011A_RECO_013.root");
	fill_histo("MB_Run2011A_RECO_014.root");
	fill_histo("MB_Run2011A_RECO_015.root");
	fill_histo("MB_Run2011A_RECO_016.root");
	fill_histo("MB_Run2011A_RECO_017.root");
	fill_histo("MB_Run2011A_RECO_018.root");
	fill_histo("MB_Run2011A_RECO_019.root");
	fill_histo("MB_Run2011A_RECO_020.root");
	fill_histo("MB_Run2011A_RECO_021.root");
	fill_histo("MB_Run2011A_RECO_022.root");
	fill_histo("MB_Run2011A_RECO_023.root");
	fill_histo("MB_Run2011A_RECO_024.root");
	fill_histo("MB_Run2011A_RECO_025.root");
	fill_histo("MB_Run2011A_RECO_026.root");
	fill_histo("MB_Run2011A_RECO_027.root");
	fill_histo("MB_Run2011A_RECO_028.root");
	fill_histo("MB_Run2011A_RECO_029.root");
	fill_histo("MB_Run2011A_RECO_030.root");
	fill_histo("MB_Run2011A_RECO_031.root");
	fill_histo("MB_Run2011A_RECO_032.root");
	fill_histo("MB_Run2011A_RECO_033.root");
	fill_histo("MB_Run2011A_RECO_034.root");
	fill_histo("MB_Run2011A_RECO_035.root");
	fill_histo("MB_Run2011A_RECO_036.root");
	fill_histo("MB_Run2011A_RECO_037.root");
	fill_histo("MB_Run2011A_RECO_038.root");
	fill_histo("MB_Run2011A_RECO_039.root");
	fill_histo("MB_Run2011A_RECO_040.root");
	fill_histo("MB_Run2011A_RECO_041.root");
	fill_histo("MB_Run2011A_RECO_042.root");
	fill_histo("MB_Run2011A_RECO_043.root");
	fill_histo("MB_Run2011A_RECO_044.root");
	fill_histo("MB_Run2011A_RECO_045.root");
	fill_histo("MB_Run2011A_RECO_046.root");
	fill_histo("MB_Run2011A_RECO_047.root");
	fill_histo("MB_Run2011A_RECO_048.root");
	fill_histo("MB_Run2011A_RECO_049.root");
	fill_histo("MB_Run2011A_RECO_050.root");
	fill_histo("MB_Run2011A_RECO_051.root");
	fill_histo("MB_Run2011A_RECO_052.root");
	fill_histo("MB_Run2011A_RECO_053.root");
	fill_histo("MB_Run2011A_RECO_054.root");
	fill_histo("MB_Run2011A_RECO_055.root");
	fill_histo("MB_Run2011A_RECO_056.root");
	fill_histo("MB_Run2011A_RECO_057.root");
	fill_histo("MB_Run2011A_RECO_058.root");
	fill_histo("MB_Run2011A_RECO_059.root");
	fill_histo("MB_Run2011A_RECO_060.root");
	fill_histo("MB_Run2011A_RECO_061.root");
	fill_histo("MB_Run2011A_RECO_062.root");
	fill_histo("MB_Run2011A_RECO_063.root");
	fill_histo("MB_Run2011A_RECO_064.root");
	fill_histo("MB_Run2011A_RECO_065.root");
	fill_histo("MB_Run2011A_RECO_066.root");
	fill_histo("MB_Run2011A_RECO_067.root");
	fill_histo("MB_Run2011A_RECO_068.root");
	fill_histo("MB_Run2011A_RECO_069.root");
	fill_histo("MB_Run2011A_RECO_070.root");
	fill_histo("MB_Run2011A_RECO_071.root");
	fill_histo("MB_Run2011A_RECO_072.root");
	fill_histo("MB_Run2011A_RECO_073.root");
	fill_histo("MB_Run2011A_RECO_074.root");
	fill_histo("MB_Run2011A_RECO_075.root");
	fill_histo("MB_Run2011A_RECO_076.root");
	fill_histo("MB_Run2011A_RECO_077.root");
	fill_histo("MB_Run2011A_RECO_078.root");
	fill_histo("MB_Run2011A_RECO_079.root");
	fill_histo("MB_Run2011A_RECO_080.root");
	fill_histo("MB_Run2011A_RECO_081.root");
	fill_histo("MB_Run2011A_RECO_082.root");
	fill_histo("MB_Run2011A_RECO_083.root");
	fill_histo("MB_Run2011A_RECO_084.root");
	fill_histo("MB_Run2011A_RECO_085.root");
	fill_histo("MB_Run2011A_RECO_086.root");
	fill_histo("MB_Run2011A_RECO_087.root");
	fill_histo("MB_Run2011A_RECO_088.root");
	fill_histo("MB_Run2011A_RECO_089.root");
	fill_histo("MB_Run2011A_RECO_090.root");
	fill_histo("MB_Run2011A_RECO_091.root");
	fill_histo("MB_Run2011A_RECO_092.root");
	fill_histo("MB_Run2011A_RECO_093.root");
	fill_histo("MB_Run2011A_RECO_094.root");
	fill_histo("MB_Run2011A_RECO_095.root");
	fill_histo("MB_Run2011A_RECO_096.root");
	fill_histo("MB_Run2011A_RECO_097.root");
	fill_histo("MB_Run2011A_RECO_098.root");
	fill_histo("MB_Run2011A_RECO_099.root");
	fill_histo("MB_Run2011A_RECO_100.root");
	fill_histo("MB_Run2011A_RECO_101.root");
	fill_histo("MB_Run2011A_RECO_102.root");
	fill_histo("MB_Run2011A_RECO_103.root");
	fill_histo("MB_Run2011A_RECO_104.root");
	fill_histo("MB_Run2011A_RECO_105.root");
	fill_histo("MB_Run2011A_RECO_106.root");
	fill_histo("MB_Run2011A_RECO_107.root");
	fill_histo("MB_Run2011A_RECO_108.root");
	fill_histo("MB_Run2011A_RECO_109.root");
	fill_histo("MB_Run2011A_RECO_110.root");
	fill_histo("MB_Run2011A_RECO_111.root");
	fill_histo("MB_Run2011A_RECO_112.root");
	fill_histo("MB_Run2011A_RECO_113.root");
	fill_histo("MB_Run2011A_RECO_114.root");
	fill_histo("MB_Run2011A_RECO_115.root");
	fill_histo("MB_Run2011A_RECO_116.root");
	fill_histo("MB_Run2011A_RECO_117.root");
	fill_histo("MB_Run2011A_RECO_118.root");
	fill_histo("MB_Run2011A_RECO_119.root");
	fill_histo("MB_Run2011A_RECO_120.root");
	fill_histo("MB_Run2011A_RECO_121.root");
	fill_histo("MB_Run2011A_RECO_122.root");
	fill_histo("MB_Run2011A_RECO_123.root");
	fill_histo("MB_Run2011A_RECO_124.root");
	fill_histo("MB_Run2011A_RECO_125.root");
	fill_histo("MB_Run2011A_RECO_126.root");
	fill_histo("MB_Run2011A_RECO_127.root");
	fill_histo("MB_Run2011A_RECO_128.root");
	fill_histo("MB_Run2011A_RECO_129.root");
	fill_histo("MB_Run2011A_RECO_130.root");
	fill_histo("MB_Run2011A_RECO_131.root");
	fill_histo("MB_Run2011A_RECO_132.root");
	fill_histo("MB_Run2011A_RECO_133.root");
	fill_histo("MB_Run2011A_RECO_134.root");
	fill_histo("MB_Run2011A_RECO_135.root");
	fill_histo("MB_Run2011A_RECO_136.root");
	fill_histo("MB_Run2011A_RECO_137.root");
	fill_histo("MB_Run2011A_RECO_138.root");
	fill_histo("MB_Run2011A_RECO_139.root");
	fill_histo("MB_Run2011A_RECO_140.root");
	fill_histo("MB_Run2011A_RECO_141.root");
	fill_histo("MB_Run2011A_RECO_142.root");
	fill_histo("MB_Run2011A_RECO_143.root");
	fill_histo("MB_Run2011A_RECO_144.root");
	fill_histo("MB_Run2011A_RECO_145.root");
	fill_histo("MB_Run2011A_RECO_146.root");
	fill_histo("MB_Run2011A_RECO_147.root");
	fill_histo("MB_Run2011A_RECO_148.root");
	fill_histo("MB_Run2011A_RECO_149.root");
	fill_histo("MB_Run2011A_RECO_150.root");
	fill_histo("MB_Run2011A_RECO_151.root");
	fill_histo("MB_Run2011A_RECO_152.root");
	fill_histo("MB_Run2011A_RECO_153.root");
	fill_histo("MB_Run2011A_RECO_154.root");
	fill_histo("MB_Run2011A_RECO_155.root");
	fill_histo("MB_Run2011A_RECO_156.root");
	fill_histo("MB_Run2011A_RECO_157.root");
	fill_histo("MB_Run2011A_RECO_158.root");
	fill_histo("MB_Run2011A_RECO_159.root");
	fill_histo("MB_Run2011A_RECO_160.root");
	fill_histo("MB_Run2011A_RECO_161.root");
	fill_histo("MB_Run2011A_RECO_162.root");
	fill_histo("MB_Run2011A_RECO_163.root");
	fill_histo("MB_Run2011A_RECO_164.root");
	fill_histo("MB_Run2011A_RECO_165.root");
	fill_histo("MB_Run2011A_RECO_166.root");
	fill_histo("MB_Run2011A_RECO_167.root");
	fill_histo("MB_Run2011A_RECO_168.root");
	fill_histo("MB_Run2011A_RECO_169.root");
	fill_histo("MB_Run2011A_RECO_170.root");
	fill_histo("MB_Run2011A_RECO_171.root");
	fill_histo("MB_Run2011A_RECO_172.root");
	fill_histo("MB_Run2011A_RECO_173.root");
	fill_histo("MB_Run2011A_RECO_174.root");
	fill_histo("MB_Run2011A_RECO_175.root");
	fill_histo("MB_Run2011A_RECO_176.root");
	fill_histo("MB_Run2011A_RECO_177.root");
	fill_histo("MB_Run2011A_RECO_178.root");
	fill_histo("MB_Run2011A_RECO_179.root");
	fill_histo("MB_Run2011A_RECO_180.root");
	fill_histo("MB_Run2011A_RECO_181.root");
	fill_histo("MB_Run2011A_RECO_182.root");
	fill_histo("MB_Run2011A_RECO_183.root");
	fill_histo("MB_Run2011A_RECO_184.root");
	fill_histo("MB_Run2011A_RECO_185.root");
	fill_histo("MB_Run2011A_RECO_186.root");
	fill_histo("MB_Run2011A_RECO_187.root");
	fill_histo("MB_Run2011A_RECO_188.root");
	fill_histo("MB_Run2011A_RECO_189.root");
	fill_histo("MB_Run2011A_RECO_190.root");
	fill_histo("MB_Run2011A_RECO_191.root");
	fill_histo("MB_Run2011A_RECO_192.root");
	fill_histo("MB_Run2011A_RECO_193.root");
	fill_histo("MB_Run2011A_RECO_194.root");
	fill_histo("MB_Run2011A_RECO_195.root");
	fill_histo("MB_Run2011A_RECO_196.root");
	fill_histo("MB_Run2011A_RECO_197.root");
	fill_histo("MB_Run2011A_RECO_198.root");
	fill_histo("MB_Run2011A_RECO_199.root");
	fill_histo("MB_Run2011A_RECO_200.root");
	fill_histo("MB_Run2011A_RECO_201.root");
	fill_histo("MB_Run2011A_RECO_202.root");
	fill_histo("MB_Run2011A_RECO_203.root");
	fill_histo("MB_Run2011A_RECO_204.root");
	fill_histo("MB_Run2011A_RECO_205.root");
	fill_histo("MB_Run2011A_RECO_206.root");
	fill_histo("MB_Run2011A_RECO_207.root");
	fill_histo("MB_Run2011A_RECO_208.root");
	fill_histo("MB_Run2011A_RECO_209.root");
	fill_histo("MB_Run2011A_RECO_210.root");
	fill_histo("MB_Run2011A_RECO_211.root");
	fill_histo("MB_Run2011A_RECO_212.root");
	fill_histo("MB_Run2011A_RECO_213.root");
	fill_histo("MB_Run2011A_RECO_214.root");
	fill_histo("MB_Run2011A_RECO_215.root");
	fill_histo("MB_Run2011A_RECO_216.root");
	fill_histo("MB_Run2011A_RECO_217.root");
	fill_histo("MB_Run2011A_RECO_218.root");
	fill_histo("MB_Run2011A_RECO_219.root");
	fill_histo("MB_Run2011A_RECO_220.root");
	fill_histo("MB_Run2011A_RECO_221.root");
	fill_histo("MB_Run2011A_RECO_222.root");
	fill_histo("MB_Run2011A_RECO_223.root");
	fill_histo("MB_Run2011A_RECO_224.root");
	fill_histo("MB_Run2011A_RECO_225.root");
	fill_histo("MB_Run2011A_RECO_226.root");
	fill_histo("MB_Run2011A_RECO_227.root");
	fill_histo("MB_Run2011A_RECO_228.root");
	fill_histo("MB_Run2011A_RECO_229.root");
	fill_histo("MB_Run2011A_RECO_230.root");
	fill_histo("MB_Run2011A_RECO_231.root");
	fill_histo("MB_Run2011A_RECO_232.root");
	fill_histo("MB_Run2011A_RECO_233.root");
	fill_histo("MB_Run2011A_RECO_234.root");
	fill_histo("MB_Run2011A_RECO_235.root");
	fill_histo("MB_Run2011A_RECO_236.root");
	fill_histo("MB_Run2011A_RECO_237.root");
	fill_histo("MB_Run2011A_RECO_238.root");
	fill_histo("MB_Run2011A_RECO_239.root");
	fill_histo("MB_Run2011A_RECO_240.root");
	fill_histo("MB_Run2011A_RECO_241.root");
	fill_histo("MB_Run2011A_RECO_242.root");
	fill_histo("MB_Run2011A_RECO_243.root");
	fill_histo("MB_Run2011A_RECO_244.root");
	fill_histo("MB_Run2011A_RECO_245.root");
	fill_histo("MB_Run2011A_RECO_246.root");
	fill_histo("MB_Run2011A_RECO_247.root");
	fill_histo("MB_Run2011A_RECO_248.root");
	fill_histo("MB_Run2011A_RECO_249.root");
	fill_histo("MB_Run2011A_RECO_250.root");
	fill_histo("MB_Run2011A_RECO_251.root");
	fill_histo("MB_Run2011A_RECO_252.root");
	fill_histo("MB_Run2011A_RECO_253.root");
	fill_histo("MB_Run2011A_RECO_254.root");
	fill_histo("MB_Run2011A_RECO_255.root");
	fill_histo("MB_Run2011A_RECO_256.root");
	fill_histo("MB_Run2011A_RECO_257.root");
	fill_histo("MB_Run2011A_RECO_258.root");
	fill_histo("MB_Run2011A_RECO_259.root");
	fill_histo("MB_Run2011A_RECO_260.root");
	fill_histo("MB_Run2011A_RECO_261.root");
	fill_histo("MB_Run2011A_RECO_262.root");
	fill_histo("MB_Run2011A_RECO_263.root");
	fill_histo("MB_Run2011A_RECO_264.root");
	fill_histo("MB_Run2011A_RECO_265.root");
	fill_histo("MB_Run2011A_RECO_266.root");
	fill_histo("MB_Run2011A_RECO_267.root");
	fill_histo("MB_Run2011A_RECO_268.root");
	fill_histo("MB_Run2011A_RECO_269.root");
	fill_histo("MB_Run2011A_RECO_270.root");
	fill_histo("MB_Run2011A_RECO_271.root");
	fill_histo("MB_Run2011A_RECO_272.root");
	fill_histo("MB_Run2011A_RECO_273.root");
	fill_histo("MB_Run2011A_RECO_274.root");
	fill_histo("MB_Run2011A_RECO_275.root");
	fill_histo("MB_Run2011A_RECO_276.root");
	fill_histo("MB_Run2011A_RECO_277.root");
	fill_histo("MB_Run2011A_RECO_278.root");
	fill_histo("MB_Run2011A_RECO_279.root");
	fill_histo("MB_Run2011A_RECO_280.root");
	fill_histo("MB_Run2011A_RECO_281.root");
	fill_histo("MB_Run2011A_RECO_282.root");
	fill_histo("MB_Run2011A_RECO_283.root");
	fill_histo("MB_Run2011A_RECO_284.root");
	fill_histo("MB_Run2011A_RECO_285.root");
	fill_histo("MB_Run2011A_RECO_286.root");
	fill_histo("MB_Run2011A_RECO_287.root");
	fill_histo("MB_Run2011A_RECO_288.root");
	fill_histo("MB_Run2011A_RECO_289.root");
	fill_histo("MB_Run2011A_RECO_290.root");
	fill_histo("MB_Run2011A_RECO_291.root");
	fill_histo("MB_Run2011A_RECO_292.root");
	fill_histo("MB_Run2011A_RECO_293.root");
	fill_histo("MB_Run2011A_RECO_294.root");
	fill_histo("MB_Run2011A_RECO_295.root");
	fill_histo("MB_Run2011A_RECO_296.root");
	fill_histo("MB_Run2011A_RECO_297.root");
	fill_histo("MB_Run2011A_RECO_298.root");
	fill_histo("MB_Run2011A_RECO_299.root");
	fill_histo("MB_Run2011A_RECO_300.root");
	fill_histo("MB_Run2011A_RECO_301.root");
	fill_histo("MB_Run2011A_RECO_302.root");
	fill_histo("MB_Run2011A_RECO_303.root");
	fill_histo("MB_Run2011A_RECO_304.root");
	fill_histo("MB_Run2011A_RECO_305.root");
	fill_histo("MB_Run2011A_RECO_306.root");
	fill_histo("MB_Run2011A_RECO_307.root");
	fill_histo("MB_Run2011A_RECO_308.root");
	fill_histo("MB_Run2011A_RECO_309.root");
	fill_histo("MB_Run2011A_RECO_310.root");
	fill_histo("MB_Run2011A_RECO_311.root");
	fill_histo("MB_Run2011A_RECO_312.root");
	fill_histo("MB_Run2011A_RECO_313.root");
	fill_histo("MB_Run2011A_RECO_314.root");
	fill_histo("MB_Run2011A_RECO_315.root");
	fill_histo("MB_Run2011A_RECO_316.root");
	fill_histo("MB_Run2011A_RECO_317.root");
	fill_histo("MB_Run2011A_RECO_318.root");
	fill_histo("MB_Run2011A_RECO_319.root");
	fill_histo("MB_Run2011A_RECO_320.root");
	fill_histo("MB_Run2011A_RECO_321.root");
	fill_histo("MB_Run2011A_RECO_322.root");
	fill_histo("MB_Run2011A_RECO_323.root");
	fill_histo("MB_Run2011A_RECO_324.root");
	fill_histo("MB_Run2011A_RECO_325.root");
	fill_histo("MB_Run2011A_RECO_326.root");
	fill_histo("MB_Run2011A_RECO_327.root");
	fill_histo("MB_Run2011A_RECO_328.root");
	fill_histo("MB_Run2011A_RECO_329.root");
	fill_histo("MB_Run2011A_RECO_330.root");
	fill_histo("MB_Run2011A_RECO_331.root");
	fill_histo("MB_Run2011A_RECO_332.root");
	fill_histo("MB_Run2011A_RECO_333.root");
	fill_histo("MB_Run2011A_RECO_334.root");
	fill_histo("MB_Run2011A_RECO_335.root");
	fill_histo("MB_Run2011A_RECO_336.root");
	fill_histo("MB_Run2011A_RECO_337.root");
	fill_histo("MB_Run2011A_RECO_338.root");
	fill_histo("MB_Run2011A_RECO_339.root");
	fill_histo("MB_Run2011A_RECO_340.root");
	fill_histo("MB_Run2011A_RECO_341.root");
	fill_histo("MB_Run2011A_RECO_342.root");
	fill_histo("MB_Run2011A_RECO_343.root");
	fill_histo("MB_Run2011A_RECO_344.root");
	fill_histo("MB_Run2011A_RECO_345.root");
	fill_histo("MB_Run2011A_RECO_346.root");
	fill_histo("MB_Run2011A_RECO_347.root");
	fill_histo("MB_Run2011A_RECO_348.root");
	fill_histo("MB_Run2011A_RECO_349.root");
	fill_histo("MB_Run2011A_RECO_350.root");
	fill_histo("MB_Run2011A_RECO_351.root");
	fill_histo("MB_Run2011A_RECO_352.root");
	fill_histo("MB_Run2011A_RECO_353.root");
	fill_histo("MB_Run2011A_RECO_354.root");
	fill_histo("MB_Run2011A_RECO_355.root");
	fill_histo("MB_Run2011A_RECO_356.root");
	fill_histo("MB_Run2011A_RECO_357.root");
	fill_histo("MB_Run2011A_RECO_358.root");
	fill_histo("MB_Run2011A_RECO_359.root");
	fill_histo("MB_Run2011A_RECO_360.root");
	fill_histo("MB_Run2011A_RECO_361.root");
	fill_histo("MB_Run2011A_RECO_362.root");
	fill_histo("MB_Run2011A_RECO_363.root");
	fill_histo("MB_Run2011A_RECO_364.root");
	fill_histo("MB_Run2011A_RECO_365.root");
	fill_histo("MB_Run2011A_RECO_366.root");
	fill_histo("MB_Run2011A_RECO_367.root");
	fill_histo("MB_Run2011A_RECO_368.root");
	fill_histo("MB_Run2011A_RECO_369.root");
	fill_histo("MB_Run2011A_RECO_370.root");
	fill_histo("MB_Run2011A_RECO_371.root");
	fill_histo("MB_Run2011A_RECO_372.root");
	fill_histo("MB_Run2011A_RECO_373.root");
	fill_histo("MB_Run2011A_RECO_374.root");
	fill_histo("MB_Run2011A_RECO_375.root");
	fill_histo("MB_Run2011A_RECO_376.root");
	fill_histo("MB_Run2011A_RECO_377.root");
	fill_histo("MB_Run2011A_RECO_378.root");
	fill_histo("MB_Run2011A_RECO_379.root");
	fill_histo("MB_Run2011A_RECO_380.root");
	fill_histo("MB_Run2011A_RECO_381.root");
	fill_histo("MB_Run2011A_RECO_382.root");
	fill_histo("MB_Run2011A_RECO_383.root");
	fill_histo("MB_Run2011A_RECO_384.root");
	fill_histo("MB_Run2011A_RECO_385.root");
	fill_histo("MB_Run2011A_RECO_386.root");
	fill_histo("MB_Run2011A_RECO_387.root");
	fill_histo("MB_Run2011A_RECO_388.root");
	fill_histo("MB_Run2011A_RECO_389.root");
	fill_histo("MB_Run2011A_RECO_390.root");
	fill_histo("MB_Run2011A_RECO_391.root");
	fill_histo("MB_Run2011A_RECO_392.root");
	fill_histo("MB_Run2011A_RECO_393.root");
	fill_histo("MB_Run2011A_RECO_394.root");
	fill_histo("MB_Run2011A_RECO_395.root");
	fill_histo("MB_Run2011A_RECO_396.root");
	fill_histo("MB_Run2011A_RECO_397.root");
	fill_histo("MB_Run2011A_RECO_398.root");
	fill_histo("MB_Run2011A_RECO_399.root");
	fill_histo("MB_Run2011A_RECO_400.root");
	fill_histo("MB_Run2011A_RECO_401.root");
	fill_histo("MB_Run2011A_RECO_402.root");
	fill_histo("MB_Run2011A_RECO_403.root");
	fill_histo("MB_Run2011A_RECO_404.root");
	fill_histo("MB_Run2011A_RECO_405.root");
	fill_histo("MB_Run2011A_RECO_406.root");
	fill_histo("MB_Run2011A_RECO_407.root");
	fill_histo("MB_Run2011A_RECO_408.root");
	fill_histo("MB_Run2011A_RECO_409.root");
	fill_histo("MB_Run2011A_RECO_410.root");
	fill_histo("MB_Run2011A_RECO_411.root");
	fill_histo("MB_Run2011A_RECO_412.root");
	fill_histo("MB_Run2011A_RECO_413.root");
	fill_histo("MB_Run2011A_RECO_414.root");
	fill_histo("MB_Run2011A_RECO_415.root");
	fill_histo("MB_Run2011A_RECO_416.root");
	fill_histo("MB_Run2011A_RECO_417.root");
	fill_histo("MB_Run2011A_RECO_418.root");
	fill_histo("MB_Run2011A_RECO_419.root");
	fill_histo("MB_Run2011A_RECO_420.root");
	fill_histo("MB_Run2011A_RECO_421.root");
	fill_histo("MB_Run2011A_RECO_422.root");
	fill_histo("MB_Run2011A_RECO_423.root");
	fill_histo("MB_Run2011A_RECO_424.root");
	fill_histo("MB_Run2011A_RECO_425.root");
	fill_histo("MB_Run2011A_RECO_426.root");
	fill_histo("MB_Run2011A_RECO_427.root");
	fill_histo("MB_Run2011A_RECO_428.root");
	fill_histo("MB_Run2011A_RECO_429.root");
	fill_histo("MB_Run2011A_RECO_430.root");
	fill_histo("MB_Run2011A_RECO_431.root");
	fill_histo("MB_Run2011A_RECO_432.root");
	fill_histo("MB_Run2011A_RECO_433.root");
	fill_histo("MB_Run2011A_RECO_434.root");
	fill_histo("MB_Run2011A_RECO_435.root");
	fill_histo("MB_Run2011A_RECO_436.root");
	fill_histo("MB_Run2011A_RECO_437.root");
	fill_histo("MB_Run2011A_RECO_438.root");
	fill_histo("MB_Run2011A_RECO_439.root");
	fill_histo("MB_Run2011A_RECO_440.root");
	fill_histo("MB_Run2011A_RECO_441.root");
	fill_histo("MB_Run2011A_RECO_442.root");
	fill_histo("MB_Run2011A_RECO_443.root");
	fill_histo("MB_Run2011A_RECO_444.root");
	fill_histo("MB_Run2011A_RECO_445.root");
	fill_histo("MB_Run2011A_RECO_446.root");
	fill_histo("MB_Run2011A_RECO_447.root");
	fill_histo("MB_Run2011A_RECO_448.root");
	fill_histo("MB_Run2011A_RECO_449.root");
	fill_histo("MB_Run2011A_RECO_450.root");
	fill_histo("MB_Run2011A_RECO_451.root");
	fill_histo("MB_Run2011A_RECO_452.root");
	fill_histo("MB_Run2011A_RECO_453.root");
	fill_histo("MB_Run2011A_RECO_454.root");
	fill_histo("MB_Run2011A_RECO_455.root");
	fill_histo("MB_Run2011A_RECO_456.root");
	fill_histo("MB_Run2011A_RECO_457.root");
	fill_histo("MB_Run2011A_RECO_458.root");
	fill_histo("MB_Run2011A_RECO_459.root");
	fill_histo("MB_Run2011A_RECO_460.root");
	fill_histo("MB_Run2011A_RECO_461.root");
	fill_histo("MB_Run2011A_RECO_462.root");
	fill_histo("MB_Run2011A_RECO_463.root");
	fill_histo("MB_Run2011A_RECO_464.root");
	fill_histo("MB_Run2011A_RECO_465.root");
	fill_histo("MB_Run2011A_RECO_466.root");
	fill_histo("MB_Run2011A_RECO_467.root");
	fill_histo("MB_Run2011A_RECO_468.root");
	fill_histo("MB_Run2011A_RECO_469.root");
	fill_histo("MB_Run2011A_RECO_470.root");
	fill_histo("MB_Run2011A_RECO_471.root");
	fill_histo("MB_Run2011A_RECO_472.root");
	fill_histo("MB_Run2011A_RECO_473.root");
	fill_histo("MB_Run2011A_RECO_474.root");
	fill_histo("MB_Run2011A_RECO_475.root");
	fill_histo("MB_Run2011A_RECO_476.root");
	fill_histo("MB_Run2011A_RECO_477.root");
	fill_histo("MB_Run2011A_RECO_478.root");
	fill_histo("MB_Run2011A_RECO_479.root");
	fill_histo("MB_Run2011A_RECO_480.root");
	fill_histo("MB_Run2011A_RECO_481.root");
	fill_histo("MB_Run2011A_RECO_482.root");
	fill_histo("MB_Run2011A_RECO_483.root");
	fill_histo("MB_Run2011A_RECO_484.root");
	fill_histo("MB_Run2011A_RECO_485.root");
// // 	fill_histo("MB_Run2011A_RECO_486.root");
	fill_histo("MB_Run2011A_RECO_487.root");
	fill_histo("MB_Run2011A_RECO_488.root");
	fill_histo("MB_Run2011A_RECO_489.root");
	fill_histo("MB_Run2011A_RECO_490.root");
	fill_histo("MB_Run2011A_RECO_491.root");
	fill_histo("MB_Run2011A_RECO_492.root");
	fill_histo("MB_Run2011A_RECO_493.root");
	fill_histo("MB_Run2011A_RECO_494.root");
	fill_histo("MB_Run2011A_RECO_495.root");
	fill_histo("MB_Run2011A_RECO_496.root");
	fill_histo("MB_Run2011A_RECO_497.root");
	fill_histo("MB_Run2011A_RECO_498.root");
	fill_histo("MB_Run2011A_RECO_499.root");
	fill_histo("MB_Run2011A_RECO_500.root");
	fill_histo("MB_Run2011A_RECO_501.root");
	fill_histo("MB_Run2011A_RECO_502.root");
	fill_histo("MB_Run2011A_RECO_503.root");
	fill_histo("MB_Run2011A_RECO_504.root");
	fill_histo("MB_Run2011A_RECO_505.root");
	fill_histo("MB_Run2011A_RECO_506.root");
	fill_histo("MB_Run2011A_RECO_507.root");
	fill_histo("MB_Run2011A_RECO_508.root");
	fill_histo("MB_Run2011A_RECO_509.root");
	fill_histo("MB_Run2011A_RECO_510.root");
	fill_histo("MB_Run2011A_RECO_511.root");
	fill_histo("MB_Run2011A_RECO_512.root");
	fill_histo("MB_Run2011A_RECO_513.root");
	fill_histo("MB_Run2011A_RECO_514.root");
	fill_histo("MB_Run2011A_RECO_515.root");
	fill_histo("MB_Run2011A_RECO_516.root");
	fill_histo("MB_Run2011A_RECO_517.root");
	fill_histo("MB_Run2011A_RECO_518.root");
	fill_histo("MB_Run2011A_RECO_519.root");
	fill_histo("MB_Run2011A_RECO_520.root");
	fill_histo("MB_Run2011A_RECO_521.root");
	fill_histo("MB_Run2011A_RECO_522.root");
	fill_histo("MB_Run2011A_RECO_523.root");
	fill_histo("MB_Run2011A_RECO_524.root");
	fill_histo("MB_Run2011A_RECO_525.root");
	fill_histo("MB_Run2011A_RECO_526.root");
	fill_histo("MB_Run2011A_RECO_527.root");
	fill_histo("MB_Run2011A_RECO_528.root");
	fill_histo("MB_Run2011A_RECO_529.root");
	fill_histo("MB_Run2011A_RECO_530.root");
	fill_histo("MB_Run2011A_RECO_531.root");
	fill_histo("MB_Run2011A_RECO_532.root");
	fill_histo("MB_Run2011A_RECO_533.root");
	fill_histo("MB_Run2011A_RECO_534.root");
	fill_histo("MB_Run2011A_RECO_535.root");
	fill_histo("MB_Run2011A_RECO_536.root");
	fill_histo("MB_Run2011A_RECO_537.root");
	fill_histo("MB_Run2011A_RECO_538.root");
	fill_histo("MB_Run2011A_RECO_539.root");
	fill_histo("MB_Run2011A_RECO_540.root");
	fill_histo("MB_Run2011A_RECO_541.root");
	fill_histo("MB_Run2011A_RECO_542.root");
	fill_histo("MB_Run2011A_RECO_543.root");
	fill_histo("MB_Run2011A_RECO_544.root");
	fill_histo("MB_Run2011A_RECO_545.root");
	fill_histo("MB_Run2011A_RECO_546.root");
	fill_histo("MB_Run2011A_RECO_547.root");
	fill_histo("MB_Run2011A_RECO_548.root");
	fill_histo("MB_Run2011A_RECO_549.root");
	fill_histo("MB_Run2011A_RECO_550.root");
	fill_histo("MB_Run2011A_RECO_551.root");
	fill_histo("MB_Run2011A_RECO_552.root");
	fill_histo("MB_Run2011A_RECO_553.root");
	fill_histo("MB_Run2011A_RECO_554.root");
	fill_histo("MB_Run2011A_RECO_555.root");
	fill_histo("MB_Run2011A_RECO_556.root");
	fill_histo("MB_Run2011A_RECO_557.root");
	fill_histo("MB_Run2011A_RECO_558.root");
	fill_histo("MB_Run2011A_RECO_559.root");
	fill_histo("MB_Run2011A_RECO_560.root");
	fill_histo("MB_Run2011A_RECO_561.root");
	fill_histo("MB_Run2011A_RECO_562.root");
	fill_histo("MB_Run2011A_RECO_563.root");
	fill_histo("MB_Run2011A_RECO_564.root");
	fill_histo("MB_Run2011A_RECO_565.root");
	fill_histo("MB_Run2011A_RECO_566.root");
	fill_histo("MB_Run2011A_RECO_567.root");
	fill_histo("MB_Run2011A_RECO_568.root");
	fill_histo("MB_Run2011A_RECO_569.root");
	fill_histo("MB_Run2011A_RECO_570.root");
	fill_histo("MB_Run2011A_RECO_571.root");
	fill_histo("MB_Run2011A_RECO_572.root");
	fill_histo("MB_Run2011A_RECO_573.root");
	fill_histo("MB_Run2011A_RECO_574.root");
	fill_histo("MB_Run2011A_RECO_575.root");
	fill_histo("MB_Run2011A_RECO_576.root");
	fill_histo("MB_Run2011A_RECO_577.root");
	fill_histo("MB_Run2011A_RECO_578.root");
	fill_histo("MB_Run2011A_RECO_579.root");
	fill_histo("MB_Run2011A_RECO_580.root");
	fill_histo("MB_Run2011A_RECO_581.root");
	fill_histo("MB_Run2011A_RECO_582.root");
	fill_histo("MB_Run2011A_RECO_583.root");
	fill_histo("MB_Run2011A_RECO_584.root");
	fill_histo("MB_Run2011A_RECO_585.root");
	fill_histo("MB_Run2011A_RECO_586.root");
	fill_histo("MB_Run2011A_RECO_587.root");
	fill_histo("MB_Run2011A_RECO_588.root");
	fill_histo("MB_Run2011A_RECO_589.root");
	fill_histo("MB_Run2011A_RECO_590.root");
	fill_histo("MB_Run2011A_RECO_591.root");
	fill_histo("MB_Run2011A_RECO_592.root");
	fill_histo("MB_Run2011A_RECO_593.root");
	fill_histo("MB_Run2011A_RECO_594.root");
	fill_histo("MB_Run2011A_RECO_595.root");
	fill_histo("MB_Run2011A_RECO_596.root");
	fill_histo("MB_Run2011A_RECO_597.root");
	fill_histo("MB_Run2011A_RECO_598.root");
	fill_histo("MB_Run2011A_RECO_599.root");
	fill_histo("MB_Run2011A_RECO_600.root");
	fill_histo("MB_Run2011A_RECO_601.root");
	fill_histo("MB_Run2011A_RECO_602.root");
	fill_histo("MB_Run2011A_RECO_603.root");
	fill_histo("MB_Run2011A_RECO_604.root");
	fill_histo("MB_Run2011A_RECO_605.root");
	fill_histo("MB_Run2011A_RECO_606.root");
	fill_histo("MB_Run2011A_RECO_607.root");
	fill_histo("MB_Run2011A_RECO_608.root");
	fill_histo("MB_Run2011A_RECO_609.root");
	fill_histo("MB_Run2011A_RECO_610.root");
	fill_histo("MB_Run2011A_RECO_611.root");
	fill_histo("MB_Run2011A_RECO_612.root");
	fill_histo("MB_Run2011A_RECO_613.root");
	fill_histo("MB_Run2011A_RECO_614.root");
	fill_histo("MB_Run2011A_RECO_615.root");
	fill_histo("MB_Run2011A_RECO_616.root");
	fill_histo("MB_Run2011A_RECO_617.root");
	fill_histo("MB_Run2011A_RECO_618.root");
	fill_histo("MB_Run2011A_RECO_619.root");
	fill_histo("MB_Run2011A_RECO_620.root");
	fill_histo("MB_Run2011A_RECO_621.root");
	fill_histo("MB_Run2011A_RECO_622.root");
	fill_histo("MB_Run2011A_RECO_623.root");
	fill_histo("MB_Run2011A_RECO_624.root");
	fill_histo("MB_Run2011A_RECO_625.root");
	fill_histo("MB_Run2011A_RECO_626.root");
	fill_histo("MB_Run2011A_RECO_627.root");
	fill_histo("MB_Run2011A_RECO_628.root");
	fill_histo("MB_Run2011A_RECO_629.root");

	filedir = "/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/HVscans/";

	fill_histo("MB_Run2011A_RECO_001.root");
	fill_histo("MB_Run2011A_RECO_002.root");
	fill_histo("MB_Run2011A_RECO_003.root");
	fill_histo("MB_Run2011A_RECO_004.root");
	fill_histo("MB_Run2011A_RECO_005.root");
	fill_histo("MB_Run2011A_RECO_006.root");
	fill_histo("MB_Run2011A_RECO_007.root");
	fill_histo("MB_Run2011A_RECO_008.root");
	fill_histo("MB_Run2011A_RECO_009.root");
	fill_histo("MB_Run2011A_RECO_010.root");
	fill_histo("MB_Run2011A_RECO_011.root");
	fill_histo("MB_Run2011A_RECO_012.root");
	fill_histo("MB_Run2011A_RECO_013.root");
	fill_histo("MB_Run2011A_RECO_014.root");
	fill_histo("MB_Run2011A_RECO_015.root");
	fill_histo("MB_Run2011A_RECO_016.root");
	fill_histo("MB_Run2011A_RECO_017.root");
	fill_histo("MB_Run2011A_RECO_018.root");
	fill_histo("MB_Run2011A_RECO_019.root");
	fill_histo("MB_Run2011A_RECO_020.root");
	fill_histo("MB_Run2011A_RECO_021.root");
	fill_histo("MB_Run2011A_RECO_022.root");
	fill_histo("MB_Run2011A_RECO_023.root");
	fill_histo("MB_Run2011A_RECO_024.root");
	fill_histo("MB_Run2011A_RECO_025.root");
	fill_histo("MB_Run2011A_RECO_026.root");
	fill_histo("MB_Run2011A_RECO_027.root");
	fill_histo("MB_Run2011A_RECO_028.root");
	fill_histo("MB_Run2011A_RECO_029.root");
	fill_histo("MB_Run2011A_RECO_030.root");
	fill_histo("MB_Run2011A_RECO_031.root");
	fill_histo("MB_Run2011A_RECO_032.root");
	fill_histo("MB_Run2011A_RECO_033.root");
	fill_histo("MB_Run2011A_RECO_034.root");
	fill_histo("MB_Run2011A_RECO_035.root");
	fill_histo("MB_Run2011A_RECO_036.root");
	fill_histo("MB_Run2011A_RECO_037.root");
	fill_histo("MB_Run2011A_RECO_038.root");
	fill_histo("MB_Run2011A_RECO_039.root");
	fill_histo("MB_Run2011A_RECO_040.root");
	fill_histo("MB_Run2011A_RECO_041.root");
	fill_histo("MB_Run2011A_RECO_042.root");
	fill_histo("MB_Run2011A_RECO_043.root");
	fill_histo("MB_Run2011A_RECO_044.root");
	fill_histo("MB_Run2011A_RECO_045.root");
	fill_histo("MB_Run2011A_RECO_046.root");
	fill_histo("MB_Run2011A_RECO_047.root");
	fill_histo("MB_Run2011A_RECO_048.root");
	fill_histo("MB_Run2011A_RECO_049.root");
	fill_histo("MB_Run2011A_RECO_050.root");
	fill_histo("MB_Run2011A_RECO_051.root");
	fill_histo("MB_Run2011A_RECO_052.root");
	fill_histo("MB_Run2011A_RECO_053.root");
	fill_histo("MB_Run2011A_RECO_054.root");
	fill_histo("MB_Run2011A_RECO_055.root");
	fill_histo("MB_Run2011A_RECO_056.root");
	fill_histo("MB_Run2011A_RECO_057.root");
	fill_histo("MB_Run2011A_RECO_058.root");
	fill_histo("MB_Run2011A_RECO_059.root");
	fill_histo("MB_Run2011A_RECO_060.root");
	fill_histo("MB_Run2011A_RECO_061.root");
	fill_histo("MB_Run2011A_RECO_062.root");
	fill_histo("MB_Run2011A_RECO_063.root");
	fill_histo("MB_Run2011A_RECO_064.root");
	fill_histo("MB_Run2011A_RECO_065.root");
	fill_histo("MB_Run2011A_RECO_066.root");
	fill_histo("MB_Run2011A_RECO_067.root");
	fill_histo("MB_Run2011A_RECO_068.root");
	fill_histo("MB_Run2011A_RECO_069.root");
	fill_histo("MB_Run2011A_RECO_070.root");
	fill_histo("MB_Run2011A_RECO_071.root");
	fill_histo("MB_Run2011A_RECO_072.root");
	fill_histo("MB_Run2011A_RECO_073.root");
	fill_histo("MB_Run2011A_RECO_074.root");
	fill_histo("MB_Run2011A_RECO_075.root");
	fill_histo("MB_Run2011A_RECO_076.root");
	fill_histo("MB_Run2011A_RECO_077.root");
	fill_histo("MB_Run2011A_RECO_078.root");
	fill_histo("MB_Run2011A_RECO_079.root");
	fill_histo("MB_Run2011A_RECO_080.root");
	fill_histo("MB_Run2011A_RECO_081.root");
	fill_histo("MB_Run2011A_RECO_082.root");
	fill_histo("MB_Run2011A_RECO_083.root");
	fill_histo("MB_Run2011A_RECO_084.root");
	fill_histo("MB_Run2011A_RECO_085.root");
	fill_histo("MB_Run2011A_RECO_086.root");
	fill_histo("MB_Run2011A_RECO_087.root");
	fill_histo("MB_Run2011A_RECO_088.root");
	fill_histo("MB_Run2011A_RECO_089.root");
	fill_histo("MB_Run2011A_RECO_090.root");
	fill_histo("MB_Run2011A_RECO_091.root");
	fill_histo("MB_Run2011A_RECO_092.root");
	fill_histo("MB_Run2011A_RECO_093.root");
	fill_histo("MB_Run2011A_RECO_094.root");
	fill_histo("MB_Run2011A_RECO_095.root");
	fill_histo("MB_Run2011A_RECO_096.root");
	fill_histo("MB_Run2011A_RECO_097.root");
	fill_histo("MB_Run2011A_RECO_098.root");
	fill_histo("MB_Run2011A_RECO_099.root");
	fill_histo("MB_Run2011A_RECO_100.root");
	fill_histo("MB_Run2011A_RECO_101.root");
	fill_histo("MB_Run2011A_RECO_102.root");
	fill_histo("MB_Run2011A_RECO_103.root");
	fill_histo("MB_Run2011A_RECO_104.root");
	fill_histo("MB_Run2011A_RECO_105.root");
	fill_histo("MB_Run2011A_RECO_106.root");
	fill_histo("MB_Run2011A_RECO_107.root");
	fill_histo("MB_Run2011A_RECO_108.root");
	fill_histo("MB_Run2011A_RECO_109.root");
	fill_histo("MB_Run2011A_RECO_110.root");
	fill_histo("MB_Run2011A_RECO_111.root");
	fill_histo("MB_Run2011A_RECO_112.root");
	fill_histo("MB_Run2011A_RECO_113.root");
	fill_histo("MB_Run2011A_RECO_114.root");
	fill_histo("MB_Run2011A_RECO_115.root");
	fill_histo("MB_Run2011A_RECO_116.root");
	fill_histo("MB_Run2011A_RECO_117.root");
	fill_histo("MB_Run2011A_RECO_118.root");
	fill_histo("MB_Run2011A_RECO_119.root");
	fill_histo("MB_Run2011A_RECO_120.root");
	fill_histo("MB_Run2011A_RECO_121.root");
	fill_histo("MB_Run2011A_RECO_122.root");
	fill_histo("MB_Run2011A_RECO_123.root");
	fill_histo("MB_Run2011A_RECO_124.root");
	fill_histo("MB_Run2011A_RECO_125.root");
	fill_histo("MB_Run2011A_RECO_126.root");
	fill_histo("MB_Run2011A_RECO_127.root");
	fill_histo("MB_Run2011A_RECO_128.root");
	fill_histo("MB_Run2011A_RECO_129.root");
	fill_histo("MB_Run2011A_RECO_130.root");
	fill_histo("MB_Run2011A_RECO_131.root");
	fill_histo("MB_Run2011A_RECO_132.root");
	fill_histo("MB_Run2011A_RECO_133.root");
	fill_histo("MB_Run2011A_RECO_134.root");
	fill_histo("MB_Run2011A_RECO_135.root");
	fill_histo("MB_Run2011A_RECO_136.root");
	fill_histo("MB_Run2011A_RECO_137.root");
	fill_histo("MB_Run2011A_RECO_138.root");
	fill_histo("MB_Run2011A_RECO_139.root");
	fill_histo("MB_Run2011A_RECO_140.root");
	fill_histo("MB_Run2011A_RECO_141.root");
	fill_histo("MB_Run2011A_RECO_142.root");
	fill_histo("MB_Run2011A_RECO_143.root");
	fill_histo("MB_Run2011A_RECO_144.root");
	fill_histo("MB_Run2011A_RECO_145.root");
	fill_histo("MB_Run2011A_RECO_146.root");
	fill_histo("MB_Run2011A_RECO_147.root");
	fill_histo("MB_Run2011A_RECO_148.root");
	fill_histo("MB_Run2011A_RECO_149.root");
	fill_histo("MB_Run2011A_RECO_150.root");
	fill_histo("MB_Run2011A_RECO_151.root");
	fill_histo("MB_Run2011A_RECO_152.root");
	fill_histo("MB_Run2011A_RECO_153.root");
	fill_histo("MB_Run2011A_RECO_154.root");
	fill_histo("MB_Run2011A_RECO_155.root");
	fill_histo("MB_Run2011A_RECO_156.root");
	fill_histo("MB_Run2011A_RECO_157.root");
	fill_histo("MB_Run2011A_RECO_158.root");
	fill_histo("MB_Run2011A_RECO_159.root");
	fill_histo("MB_Run2011A_RECO_160.root");
	fill_histo("MB_Run2011A_RECO_161.root");
	fill_histo("MB_Run2011A_RECO_162.root");
	fill_histo("MB_Run2011A_RECO_163.root");
	fill_histo("MB_Run2011A_RECO_164.root");
	fill_histo("MB_Run2011A_RECO_165.root");
	fill_histo("MB_Run2011A_RECO_166.root");
	fill_histo("MB_Run2011A_RECO_167.root");
	fill_histo("MB_Run2011A_RECO_168.root");
	fill_histo("MB_Run2011A_RECO_169.root");
	fill_histo("MB_Run2011A_RECO_170.root");
	fill_histo("MB_Run2011A_RECO_171.root");
	fill_histo("MB_Run2011A_RECO_172.root");
	fill_histo("MB_Run2011A_RECO_173.root");
	fill_histo("MB_Run2011A_RECO_174.root");
	fill_histo("MB_Run2011A_RECO_175.root");
	fill_histo("MB_Run2011A_RECO_176.root");
	fill_histo("MB_Run2011A_RECO_177.root");
	fill_histo("MB_Run2011A_RECO_178.root");
	fill_histo("MB_Run2011A_RECO_179.root");
	fill_histo("MB_Run2011A_RECO_180.root");
	fill_histo("MB_Run2011A_RECO_181.root");
	fill_histo("MB_Run2011A_RECO_182.root");
	fill_histo("MB_Run2011A_RECO_183.root");
	fill_histo("MB_Run2011A_RECO_184.root");
	fill_histo("MB_Run2011A_RECO_185.root");
	fill_histo("MB_Run2011A_RECO_186.root");
// // 	fill_histo("MB_Run2011A_RECO_187.root");
	fill_histo("MB_Run2011A_RECO_188.root");
	fill_histo("MB_Run2011A_RECO_189.root");
	fill_histo("MB_Run2011A_RECO_190.root");
	fill_histo("MB_Run2011A_RECO_191.root");
	fill_histo("MB_Run2011A_RECO_192.root");
	fill_histo("MB_Run2011A_RECO_193.root");
	fill_histo("MB_Run2011A_RECO_194.root");
	fill_histo("MB_Run2011A_RECO_195.root");
	fill_histo("MB_Run2011A_RECO_196.root");
	fill_histo("MB_Run2011A_RECO_197.root");
	fill_histo("MB_Run2011A_RECO_198.root");
	fill_histo("MB_Run2011A_RECO_199.root");
	fill_histo("MB_Run2011A_RECO_200.root");
	fill_histo("MB_Run2011A_RECO_201.root");
	fill_histo("MB_Run2011A_RECO_202.root");
	fill_histo("MB_Run2011A_RECO_203.root");
	fill_histo("MB_Run2011A_RECO_204.root");
	fill_histo("MB_Run2011A_RECO_205.root");
	fill_histo("MB_Run2011A_RECO_206.root");
	fill_histo("MB_Run2011A_RECO_207.root");
	fill_histo("MB_Run2011A_RECO_208.root");
	fill_histo("MB_Run2011A_RECO_209.root");
	fill_histo("MB_Run2011A_RECO_210.root");
	fill_histo("MB_Run2011A_RECO_211.root");
	fill_histo("MB_Run2011A_RECO_212.root");
	fill_histo("MB_Run2011A_RECO_213.root");
	fill_histo("MB_Run2011A_RECO_214.root");
	fill_histo("MB_Run2011A_RECO_215.root");
	fill_histo("MB_Run2011A_RECO_216.root");
	fill_histo("MB_Run2011A_RECO_217.root");
	fill_histo("MB_Run2011A_RECO_218.root");
	fill_histo("MB_Run2011A_RECO_219.root");
	fill_histo("MB_Run2011A_RECO_220.root");
	fill_histo("MB_Run2011A_RECO_221.root");
	fill_histo("MB_Run2011A_RECO_222.root");
	fill_histo("MB_Run2011A_RECO_223.root");
	fill_histo("MB_Run2011A_RECO_224.root");
	fill_histo("MB_Run2011A_RECO_225.root");
	fill_histo("MB_Run2011A_RECO_226.root");
	fill_histo("MB_Run2011A_RECO_227.root");
	fill_histo("MB_Run2011A_RECO_228.root");
	fill_histo("MB_Run2011A_RECO_229.root");
	fill_histo("MB_Run2011A_RECO_230.root");
	fill_histo("MB_Run2011A_RECO_231.root");
	fill_histo("MB_Run2011A_RECO_232.root");
	fill_histo("MB_Run2011A_RECO_233.root");
	fill_histo("MB_Run2011A_RECO_234.root");
	fill_histo("MB_Run2011A_RECO_235.root");
	fill_histo("MB_Run2011A_RECO_236.root");
	fill_histo("MB_Run2011A_RECO_237.root");
	fill_histo("MB_Run2011A_RECO_238.root");
	fill_histo("MB_Run2011A_RECO_239.root");
	fill_histo("MB_Run2011A_RECO_240.root");
	fill_histo("MB_Run2011A_RECO_241.root");
	fill_histo("MB_Run2011A_RECO_242.root");
	fill_histo("MB_Run2011A_RECO_243.root");
	fill_histo("MB_Run2011A_RECO_244.root");
	fill_histo("MB_Run2011A_RECO_245.root");
	fill_histo("MB_Run2011A_RECO_246.root");
	fill_histo("MB_Run2011A_RECO_247.root");
	fill_histo("MB_Run2011A_RECO_248.root");
	fill_histo("MB_Run2011A_RECO_249.root");
	fill_histo("MB_Run2011A_RECO_250.root");
	fill_histo("MB_Run2011A_RECO_251.root");
	fill_histo("MB_Run2011A_RECO_252.root");
	fill_histo("MB_Run2011A_RECO_253.root");
	fill_histo("MB_Run2011A_RECO_254.root");
	fill_histo("MB_Run2011A_RECO_255.root");
	fill_histo("MB_Run2011A_RECO_256.root");
	fill_histo("MB_Run2011A_RECO_257.root");
	fill_histo("MB_Run2011A_RECO_258.root");
	fill_histo("MB_Run2011A_RECO_259.root");
	fill_histo("MB_Run2011A_RECO_260.root");
	fill_histo("MB_Run2011A_RECO_261.root");

      } else if (fullrun==2) {
	filedir = "/data/common/jkarancs/data/gridout/CMSSW_3_8_3/MinimumBias_Commissioning10-Sep17ReReco_v2_RECO/";

	fill_histo("Comm10_Sep17ReReco_01.root");
// 	fill_histo("Comm10_Sep17ReReco_02.root");
// 	fill_histo("Comm10_Sep17ReReco_03.root");
// 	fill_histo("Comm10_Sep17ReReco_04.root");
// 	fill_histo("Comm10_Sep17ReReco_05.root");
// 	fill_histo("Comm10_Sep17ReReco_06.root");
// 	fill_histo("Comm10_Sep17ReReco_07.root");
// 	fill_histo("Comm10_Sep17ReReco_08.root");
// 	fill_histo("Comm10_Sep17ReReco_09.root");
// 	fill_histo("Comm10_Sep17ReReco_10.root");
// 	fill_histo("Comm10_Sep17ReReco_11.root");
// 	fill_histo("Comm10_Sep17ReReco_12.root");
// 	fill_histo("Comm10_Sep17ReReco_13.root");
// 	fill_histo("Comm10_Sep17ReReco_14.root");
// 	fill_histo("Comm10_Sep17ReReco_15.root");
// 	fill_histo("Comm10_Sep17ReReco_16.root");
// 	fill_histo("Comm10_Sep17ReReco_17.root");
// 	fill_histo("Comm10_Sep17ReReco_18.root");
// 	fill_histo("Comm10_Sep17ReReco_19.root");
// 	fill_histo("Comm10_Sep17ReReco_20.root");
// 	fill_histo("Comm10_Sep17ReReco_21.root");
// 	fill_histo("Comm10_Sep17ReReco_22.root");
// 	fill_histo("Comm10_Sep17ReReco_23.root");
// 	fill_histo("Comm10_Sep17ReReco_24.root");
// 	fill_histo("Comm10_Sep17ReReco_25.root");
// 	fill_histo("Comm10_Sep17ReReco_26.root");
// 	fill_histo("Comm10_Sep17ReReco_27.root");
// 	fill_histo("Comm10_Sep17ReReco_28.root");
// 	fill_histo("Comm10_Sep17ReReco_29.root");
// 	fill_histo("Comm10_Sep17ReReco_30.root");
// 	fill_histo("Comm10_Sep17ReReco_31.root");
// 	fill_histo("Comm10_Sep17ReReco_32.root");
// 	fill_histo("Comm10_Sep17ReReco_33.root");
// 	fill_histo("Comm10_Sep17ReReco_34.root");
// 	fill_histo("Comm10_Sep17ReReco_35.root");
// 	fill_histo("Comm10_Sep17ReReco_36.root");
// 	fill_histo("Comm10_Sep17ReReco_37.root");
// 	fill_histo("Comm10_Sep17ReReco_38.root");
// 	fill_histo("Comm10_Sep17ReReco_39.root");
// 	fill_histo("Comm10_Sep17ReReco_40.root");
// 	fill_histo("Comm10_Sep17ReReco_41.root");
// 	fill_histo("Comm10_Sep17ReReco_42.root");
// 	fill_histo("Comm10_Sep17ReReco_43.root");
// 	fill_histo("Comm10_Sep17ReReco_44.root");
// 	fill_histo("Comm10_Sep17ReReco_45.root");
// 	fill_histo("Comm10_Sep17ReReco_46.root");
// 	fill_histo("Comm10_Sep17ReReco_47.root");
// 	fill_histo("Comm10_Sep17ReReco_48.root");
// 	fill_histo("Comm10_Sep17ReReco_49.root");
// 	fill_histo("Comm10_Sep17ReReco_50.root");
// 	fill_histo("Comm10_Sep17ReReco_51.root");
// 	fill_histo("Comm10_Sep17ReReco_52.root");
// 	fill_histo("Comm10_Sep17ReReco_53.root");
// 	fill_histo("Comm10_Sep17ReReco_54.root");
// 	fill_histo("Comm10_Sep17ReReco_55.root");
// 	fill_histo("Comm10_Sep17ReReco_56.root");
// 	fill_histo("Comm10_Sep17ReReco_57.root");

      } else {

	filedir = "/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/";
	filedir = "/data/common/jkarancs/data/gridout/CMSSW_4_1_2/MinimumBias_Run2011A-PromptReco-v1_RECO/INCOMPLETE/goodruns/";

	fill_histo("MB_Run2011A_RECO_01.root");
	fill_histo("MB_Run2011A_RECO_02.root");
	fill_histo("MB_Run2011A_RECO_03.root");
	fill_histo("MB_Run2011A_RECO_04.root");
	fill_histo("MB_Run2011A_RECO_05.root");
	fill_histo("MB_Run2011A_RECO_06.root");
	fill_histo("MB_Run2011A_RECO_07.root");
	fill_histo("MB_Run2011A_RECO_08.root");
	fill_histo("MB_Run2011A_RECO_09.root");
	fill_histo("MB_Run2011A_RECO_10.root");
	fill_histo("MB_Run2011A_RECO_11.root");
	fill_histo("MB_Run2011A_RECO_12.root");
	fill_histo("MB_Run2011A_RECO_13.root");
	fill_histo("MB_Run2011A_RECO_14.root");
	fill_histo("MB_Run2011A_RECO_15.root");
	fill_histo("MB_Run2011A_RECO_16.root");
	fill_histo("MB_Run2011A_RECO_17.root");
	fill_histo("MB_Run2011A_RECO_18.root");
	fill_histo("MB_Run2011A_RECO_19.root");
	fill_histo("MB_Run2011A_RECO_20.root");
	fill_histo("MB_Run2011A_RECO_21.root");
	fill_histo("MB_Run2011A_RECO_22.root");
	fill_histo("MB_Run2011A_RECO_23.root");
	fill_histo("MB_Run2011A_RECO_24.root");
	fill_histo("MB_Run2011A_RECO_25.root");
	fill_histo("MB_Run2011A_RECO_26.root");
	fill_histo("MB_Run2011A_RECO_27.root");
	fill_histo("MB_Run2011A_RECO_28.root");
	fill_histo("MB_Run2011A_RECO_29.root");
	fill_histo("MB_Run2011A_RECO_30.root");
	fill_histo("MB_Run2011A_RECO_31.root");
	fill_histo("MB_Run2011A_RECO_32.root");
	fill_histo("MB_Run2011A_RECO_33.root");
	fill_histo("MB_Run2011A_RECO_34.root");
	fill_histo("MB_Run2011A_RECO_35.root");
	fill_histo("MB_Run2011A_RECO_36.root");
	fill_histo("MB_Run2011A_RECO_37.root");
	fill_histo("MB_Run2011A_RECO_38.root");
	fill_histo("MB_Run2011A_RECO_39.root");
	fill_histo("MB_Run2011A_RECO_40.root");
	fill_histo("MB_Run2011A_RECO_41.root");
	fill_histo("MB_Run2011A_RECO_42.root");
	fill_histo("MB_Run2011A_RECO_43.root");
	fill_histo("MB_Run2011A_RECO_44.root");
	fill_histo("MB_Run2011A_RECO_45.root");
	fill_histo("MB_Run2011A_RECO_46.root");
	fill_histo("MB_Run2011A_RECO_47.root");
	fill_histo("MB_Run2011A_RECO_48.root");
	fill_histo("MB_Run2011A_RECO_49.root");
	fill_histo("MB_Run2011A_RECO_50.root");
	fill_histo("MB_Run2011A_RECO_51.root");
	fill_histo("MB_Run2011A_RECO_52.root");
	fill_histo("MB_Run2011A_RECO_53.root");
	fill_histo("MB_Run2011A_RECO_54.root");
	fill_histo("MB_Run2011A_RECO_55.root");
	fill_histo("MB_Run2011A_RECO_56.root");
	fill_histo("MB_Run2011A_RECO_57.root");
	fill_histo("MB_Run2011A_RECO_58.root");
	fill_histo("MB_Run2011A_RECO_59.root");
	fill_histo("MB_Run2011A_RECO_60.root");
	fill_histo("MB_Run2011A_RECO_61.root");
	fill_histo("MB_Run2011A_RECO_62.root");
	fill_histo("MB_Run2011A_RECO_63.root");
	fill_histo("MB_Run2011A_RECO_64.root");
	fill_histo("MB_Run2011A_RECO_65.root");
	fill_histo("MB_Run2011A_RECO_66.root");
	fill_histo("MB_Run2011A_RECO_67.root");
	fill_histo("MB_Run2011A_RECO_68.root");
	fill_histo("MB_Run2011A_RECO_69.root");
	fill_histo("MB_Run2011A_RECO_70.root");
	fill_histo("MB_Run2011A_RECO_71.root");
	fill_histo("MB_Run2011A_RECO_72.root");
	fill_histo("MB_Run2011A_RECO_73.root");
	fill_histo("MB_Run2011A_RECO_74.root");



// 	fill_histo("MB_Run2011A_RECO_001.root");
// 	fill_histo("MB_Run2011A_RECO_002.root");
// 	fill_histo("MB_Run2011A_RECO_003.root");
// 	fill_histo("MB_Run2011A_RECO_004.root");
// 	fill_histo("MB_Run2011A_RECO_005.root");
// 	fill_histo("MB_Run2011A_RECO_006.root");
// 	fill_histo("MB_Run2011A_RECO_007.root");
// 	fill_histo("MB_Run2011A_RECO_008.root");
// 	fill_histo("MB_Run2011A_RECO_009.root");
// 	fill_histo("MB_Run2011A_RECO_010.root");
// 	fill_histo("MB_Run2011A_RECO_011.root");
// 	fill_histo("MB_Run2011A_RECO_012.root");
// 	fill_histo("MB_Run2011A_RECO_013.root");
// 	fill_histo("MB_Run2011A_RECO_014.root");
// 	fill_histo("MB_Run2011A_RECO_015.root");
// 	fill_histo("MB_Run2011A_RECO_016.root");
// 	fill_histo("MB_Run2011A_RECO_017.root");
// 	fill_histo("MB_Run2011A_RECO_018.root");
// 	fill_histo("MB_Run2011A_RECO_019.root");
// 	fill_histo("MB_Run2011A_RECO_020.root");
// 	fill_histo("MB_Run2011A_RECO_021.root");
// 	fill_histo("MB_Run2011A_RECO_022.root");
// 	fill_histo("MB_Run2011A_RECO_023.root");
// 	fill_histo("MB_Run2011A_RECO_024.root");
// 	fill_histo("MB_Run2011A_RECO_025.root");
// 	fill_histo("MB_Run2011A_RECO_026.root");
// 	fill_histo("MB_Run2011A_RECO_027.root");
// 	fill_histo("MB_Run2011A_RECO_028.root");
// 	fill_histo("MB_Run2011A_RECO_029.root");
// 	fill_histo("MB_Run2011A_RECO_030.root");
// 	fill_histo("MB_Run2011A_RECO_031.root");
// 	fill_histo("MB_Run2011A_RECO_032.root");
// 	fill_histo("MB_Run2011A_RECO_033.root");
// 	fill_histo("MB_Run2011A_RECO_034.root");
// 	fill_histo("MB_Run2011A_RECO_035.root");
// 	fill_histo("MB_Run2011A_RECO_036.root");
// 	fill_histo("MB_Run2011A_RECO_037.root");
// 	fill_histo("MB_Run2011A_RECO_038.root");
// 	fill_histo("MB_Run2011A_RECO_039.root");
// 	fill_histo("MB_Run2011A_RECO_040.root");
// 	fill_histo("MB_Run2011A_RECO_041.root");
// 	fill_histo("MB_Run2011A_RECO_042.root");
// 	fill_histo("MB_Run2011A_RECO_043.root");
// 	fill_histo("MB_Run2011A_RECO_044.root");
// 	fill_histo("MB_Run2011A_RECO_045.root");
// 	fill_histo("MB_Run2011A_RECO_046.root");
// 	fill_histo("MB_Run2011A_RECO_047.root");
// 	fill_histo("MB_Run2011A_RECO_048.root");

// 	fill_histo("MB_Run2011A_RECO_049.root");
// 	fill_histo("MB_Run2011A_RECO_050.root");
// 	fill_histo("MB_Run2011A_RECO_051.root");
// 	fill_histo("MB_Run2011A_RECO_052.root");
// 	fill_histo("MB_Run2011A_RECO_053.root");
// 	fill_histo("MB_Run2011A_RECO_054.root");
// 	fill_histo("MB_Run2011A_RECO_055.root");
// 	fill_histo("MB_Run2011A_RECO_056.root");
// 	fill_histo("MB_Run2011A_RECO_057.root");
// // 	fill_histo("MB_Run2011A_RECO_058.root");
// // 	fill_histo("MB_Run2011A_RECO_059.root");
// // 	fill_histo("MB_Run2011A_RECO_060.root");
// // 	fill_histo("MB_Run2011A_RECO_061.root");
// // 	fill_histo("MB_Run2011A_RECO_062.root");
// // 	fill_histo("MB_Run2011A_RECO_063.root");
// // 	fill_histo("MB_Run2011A_RECO_064.root");
// // 	fill_histo("MB_Run2011A_RECO_065.root");
// // 	fill_histo("MB_Run2011A_RECO_066.root");
// // 	fill_histo("MB_Run2011A_RECO_067.root");
// 	fill_histo("MB_Run2011A_RECO_068.root");
// 	fill_histo("MB_Run2011A_RECO_069.root");
// 	fill_histo("MB_Run2011A_RECO_070.root");
// 	fill_histo("MB_Run2011A_RECO_071.root");
// 	fill_histo("MB_Run2011A_RECO_072.root");
// 	fill_histo("MB_Run2011A_RECO_073.root");
// 	fill_histo("MB_Run2011A_RECO_074.root");
// 	fill_histo("MB_Run2011A_RECO_075.root");
// 	fill_histo("MB_Run2011A_RECO_076.root");
// 	fill_histo("MB_Run2011A_RECO_077.root");
// 	fill_histo("MB_Run2011A_RECO_078.root");
// 	fill_histo("MB_Run2011A_RECO_079.root");
// 	fill_histo("MB_Run2011A_RECO_080.root");
// 	fill_histo("MB_Run2011A_RECO_081.root");
// 	fill_histo("MB_Run2011A_RECO_082.root");
// 	fill_histo("MB_Run2011A_RECO_083.root");
// 	fill_histo("MB_Run2011A_RECO_084.root");
// // 	fill_histo("MB_Run2011A_RECO_085.root");
// // 	fill_histo("MB_Run2011A_RECO_086.root");
// // 	fill_histo("MB_Run2011A_RECO_087.root");
// // 	fill_histo("MB_Run2011A_RECO_088.root");
// // 	fill_histo("MB_Run2011A_RECO_089.root");
// // 	fill_histo("MB_Run2011A_RECO_090.root");
// // 	fill_histo("MB_Run2011A_RECO_091.root");
// // 	fill_histo("MB_Run2011A_RECO_092.root");
// // 	fill_histo("MB_Run2011A_RECO_093.root");
// // 	fill_histo("MB_Run2011A_RECO_094.root");
// // 	fill_histo("MB_Run2011A_RECO_095.root");
// // 	fill_histo("MB_Run2011A_RECO_096.root");
// // 	fill_histo("MB_Run2011A_RECO_097.root");
// // 	fill_histo("MB_Run2011A_RECO_098.root");
// // 	fill_histo("MB_Run2011A_RECO_099.root");
// // 	fill_histo("MB_Run2011A_RECO_100.root");
// // 	fill_histo("MB_Run2011A_RECO_101.root");
// // 	fill_histo("MB_Run2011A_RECO_102.root");
// // 	fill_histo("MB_Run2011A_RECO_103.root");
// // 	fill_histo("MB_Run2011A_RECO_104.root");
// // 	fill_histo("MB_Run2011A_RECO_105.root");
// // 	fill_histo("MB_Run2011A_RECO_106.root");
// // 	fill_histo("MB_Run2011A_RECO_107.root");
// // 	fill_histo("MB_Run2011A_RECO_108.root");
// // 	fill_histo("MB_Run2011A_RECO_109.root");
// // 	fill_histo("MB_Run2011A_RECO_110.root");



      }
    } else if (pcatomki==2) {
      if (fullrun==1) {
	//Scans
	filedir = "/home/jkarancs/data/gridout/CMSSW_4_1_2/ExpressPhysics_Run2011A-Express-v1_FEVT/160413/";
// 	filedir = "/tmp/jkarancs/160413/";


// 	orbmin = 191550000;

	fill_histo("EP_Run2011A_FEVT_1_1_QMT.root");
	fill_histo("EP_Run2011A_FEVT_2_1_cbc.root");
	fill_histo("EP_Run2011A_FEVT_3_1_Sys.root");
	fill_histo("EP_Run2011A_FEVT_4_1_mPH.root");
	fill_histo("EP_Run2011A_FEVT_5_1_5Zi.root");
	fill_histo("EP_Run2011A_FEVT_6_1_9BG.root");
	fill_histo("EP_Run2011A_FEVT_7_1_kSo.root");
	fill_histo("EP_Run2011A_FEVT_8_1_Bux.root");
	fill_histo("EP_Run2011A_FEVT_9_1_I8x.root");
	fill_histo("EP_Run2011A_FEVT_10_1_Hfl.root");
// 	fill_histo("EP_Run2011A_FEVT_11_1_Tmx.root");
// 	fill_histo("EP_Run2011A_FEVT_12_1_bsa.root");
// 	fill_histo("EP_Run2011A_FEVT_13_1_GiH.root");
// 	fill_histo("EP_Run2011A_FEVT_14_1_eYd.root");
// 	fill_histo("EP_Run2011A_FEVT_15_1_Op1.root");
// 	fill_histo("EP_Run2011A_FEVT_16_1_GWW.root");
// 	fill_histo("EP_Run2011A_FEVT_17_1_UHk.root");
// 	fill_histo("EP_Run2011A_FEVT_18_1_7uf.root");
// 	fill_histo("EP_Run2011A_FEVT_19_1_0jd.root");
// 	fill_histo("EP_Run2011A_FEVT_20_1_6FV.root");
// 	fill_histo("EP_Run2011A_FEVT_21_1_eTF.root");
// 	fill_histo("EP_Run2011A_FEVT_22_1_0re.root");
// 	fill_histo("EP_Run2011A_FEVT_23_1_6Cl.root");
// 	fill_histo("EP_Run2011A_FEVT_24_1_hcP.root");
// 	fill_histo("EP_Run2011A_FEVT_25_1_PFs.root");
// 	fill_histo("EP_Run2011A_FEVT_26_1_AHn.root");
// 	fill_histo("EP_Run2011A_FEVT_27_1_xMp.root");
// 	fill_histo("EP_Run2011A_FEVT_28_1_QYP.root");
// 	fill_histo("EP_Run2011A_FEVT_29_1_6kB.root");
// 	fill_histo("EP_Run2011A_FEVT_30_1_Lh0.root");
// 	fill_histo("EP_Run2011A_FEVT_31_1_xX3.root");
// 	fill_histo("EP_Run2011A_FEVT_32_1_lZu.root");
// 	fill_histo("EP_Run2011A_FEVT_33_1_lA3.root");
// 	fill_histo("EP_Run2011A_FEVT_34_1_2nA.root");
// 	fill_histo("EP_Run2011A_FEVT_35_1_G3E.root");
// 	fill_histo("EP_Run2011A_FEVT_36_1_Bns.root");
// 	fill_histo("EP_Run2011A_FEVT_37_1_54c.root");
// 	fill_histo("EP_Run2011A_FEVT_38_1_9Zc.root");
// 	fill_histo("EP_Run2011A_FEVT_39_1_8NX.root");
// 	fill_histo("EP_Run2011A_FEVT_40_1_jLx.root");
// 	fill_histo("EP_Run2011A_FEVT_41_1_lZh.root");
// 	fill_histo("EP_Run2011A_FEVT_42_1_WqR.root");
// 	fill_histo("EP_Run2011A_FEVT_43_1_uO3.root");
// 	fill_histo("EP_Run2011A_FEVT_44_1_qUH.root");
// 	fill_histo("EP_Run2011A_FEVT_45_1_l1Y.root");
// 	fill_histo("EP_Run2011A_FEVT_46_1_UXZ.root");
// 	fill_histo("EP_Run2011A_FEVT_47_1_kbM.root");
// 	fill_histo("EP_Run2011A_FEVT_48_1_4f4.root");
// 	fill_histo("EP_Run2011A_FEVT_49_1_mlf.root");
// 	fill_histo("EP_Run2011A_FEVT_50_1_t4B.root");
// 	fill_histo("EP_Run2011A_FEVT_51_1_CDs.root");
// 	fill_histo("EP_Run2011A_FEVT_52_1_dkh.root");
// 	fill_histo("EP_Run2011A_FEVT_53_1_vBo.root");
// 	fill_histo("EP_Run2011A_FEVT_54_1_YXH.root");
// 	fill_histo("EP_Run2011A_FEVT_55_1_K9i.root");
// 	fill_histo("EP_Run2011A_FEVT_56_1_NjF.root");
// 	fill_histo("EP_Run2011A_FEVT_57_1_n6A.root");
// 	fill_histo("EP_Run2011A_FEVT_58_1_mcW.root");
// 	fill_histo("EP_Run2011A_FEVT_59_1_fC5.root");
// 	fill_histo("EP_Run2011A_FEVT_60_1_mCq.root");
// 	fill_histo("EP_Run2011A_FEVT_61_1_Ghr.root");
// 	fill_histo("EP_Run2011A_FEVT_62_1_KdO.root");
// 	fill_histo("EP_Run2011A_FEVT_63_1_UaR.root");
// 	fill_histo("EP_Run2011A_FEVT_64_1_TFs.root");
// 	fill_histo("EP_Run2011A_FEVT_65_1_aFH.root");
// 	fill_histo("EP_Run2011A_FEVT_66_1_gJY.root");
// 	fill_histo("EP_Run2011A_FEVT_67_1_Gf3.root");
// 	fill_histo("EP_Run2011A_FEVT_68_1_7z8.root");
// 	fill_histo("EP_Run2011A_FEVT_69_1_NwG.root");
// 	fill_histo("EP_Run2011A_FEVT_70_1_L0U.root");
// 	fill_histo("EP_Run2011A_FEVT_71_1_4pR.root");
// 	fill_histo("EP_Run2011A_FEVT_72_1_CQa.root");
// 	fill_histo("EP_Run2011A_FEVT_73_1_EBU.root");
// 	fill_histo("EP_Run2011A_FEVT_74_1_80n.root");
// 	fill_histo("EP_Run2011A_FEVT_75_1_4TM.root");
// 	fill_histo("EP_Run2011A_FEVT_76_1_s0r.root");
// 	fill_histo("EP_Run2011A_FEVT_77_1_Sun.root");
// 	fill_histo("EP_Run2011A_FEVT_78_1_9Wz.root");
// 	fill_histo("EP_Run2011A_FEVT_79_1_1Xx.root");
// 	fill_histo("EP_Run2011A_FEVT_80_1_1DC.root");
// 	fill_histo("EP_Run2011A_FEVT_81_1_kfU.root");
// 	fill_histo("EP_Run2011A_FEVT_82_1_9Wz.root");
// 	fill_histo("EP_Run2011A_FEVT_83_1_zTY.root");
// 	fill_histo("EP_Run2011A_FEVT_84_1_zcI.root");
// 	fill_histo("EP_Run2011A_FEVT_85_1_skE.root");
// 	fill_histo("EP_Run2011A_FEVT_86_1_S5v.root");
// 	fill_histo("EP_Run2011A_FEVT_87_1_npu.root");
// 	fill_histo("EP_Run2011A_FEVT_88_1_xem.root");
// 	fill_histo("EP_Run2011A_FEVT_89_1_Ejo.root");
// 	fill_histo("EP_Run2011A_FEVT_90_1_NoK.root");
// 	fill_histo("EP_Run2011A_FEVT_91_1_PF7.root");
// 	fill_histo("EP_Run2011A_FEVT_92_1_cMr.root");
// 	fill_histo("EP_Run2011A_FEVT_93_1_2Gf.root");
// 	fill_histo("EP_Run2011A_FEVT_94_1_iR6.root");
// 	fill_histo("EP_Run2011A_FEVT_95_1_5VL.root");
// 	fill_histo("EP_Run2011A_FEVT_96_1_ktf.root");
// 	fill_histo("EP_Run2011A_FEVT_97_1_dLS.root");
// 	fill_histo("EP_Run2011A_FEVT_98_1_UeJ.root");
// 	fill_histo("EP_Run2011A_FEVT_99_1_LxV.root");
// 	fill_histo("EP_Run2011A_FEVT_100_1_XaB.root");
// 	fill_histo("EP_Run2011A_FEVT_101_1_3eM.root");
// 	fill_histo("EP_Run2011A_FEVT_102_1_xZF.root");
// 	fill_histo("EP_Run2011A_FEVT_103_1_R1y.root");
// 	fill_histo("EP_Run2011A_FEVT_104_1_ztJ.root");
// 	fill_histo("EP_Run2011A_FEVT_105_1_xni.root");
// 	fill_histo("EP_Run2011A_FEVT_106_1_F2i.root");
// 	fill_histo("EP_Run2011A_FEVT_107_1_WA2.root");
// 	fill_histo("EP_Run2011A_FEVT_108_1_i6d.root");
// 	fill_histo("EP_Run2011A_FEVT_109_1_Bg6.root");
// 	fill_histo("EP_Run2011A_FEVT_110_1_8eM.root");
// 	fill_histo("EP_Run2011A_FEVT_111_1_9Q5.root");
// 	fill_histo("EP_Run2011A_FEVT_112_1_TyN.root");
// 	fill_histo("EP_Run2011A_FEVT_113_1_Zs1.root");
// 	fill_histo("EP_Run2011A_FEVT_114_1_A4s.root");
// 	fill_histo("EP_Run2011A_FEVT_115_1_tfl.root");
// 	fill_histo("EP_Run2011A_FEVT_116_1_yxH.root");
// 	fill_histo("EP_Run2011A_FEVT_117_1_isK.root");
// 	fill_histo("EP_Run2011A_FEVT_118_1_m5P.root");
// 	fill_histo("EP_Run2011A_FEVT_119_1_HvV.root");
// 	fill_histo("EP_Run2011A_FEVT_120_1_nMn.root");
// 	fill_histo("EP_Run2011A_FEVT_121_1_ZPg.root");
// 	fill_histo("EP_Run2011A_FEVT_122_1_G38.root");
// 	fill_histo("EP_Run2011A_FEVT_123_1_QJ2.root");
// 	fill_histo("EP_Run2011A_FEVT_124_1_GUW.root");
// 	fill_histo("EP_Run2011A_FEVT_125_1_Qt8.root");
// 	fill_histo("EP_Run2011A_FEVT_126_1_V2P.root");
// 	fill_histo("EP_Run2011A_FEVT_127_1_9M6.root");
// 	fill_histo("EP_Run2011A_FEVT_128_1_gft.root");

	filedir = "/home/jkarancs/data/gridout/CMSSW_4_1_2/ExpressPhysics_Run2011A-Express-v1_FEVT/rest_scans/";
// 	filedir = "/tmp/jkarancs/rest_scans/";
// 	filedir = "rfio:/castor/cern.ch/cms/store/caf/user/jkarancs/crab/rest_412_EP_Run2011A_FEVT_v1816_INCOMP/";

// 	fill_histo("EP_Run2011A_FEVT_1_1_sWv.root");
// 	fill_histo("EP_Run2011A_FEVT_2_1_eqD.root");
// 	fill_histo("EP_Run2011A_FEVT_3_1_crT.root");
// 	fill_histo("EP_Run2011A_FEVT_4_1_xd5.root");
// 	fill_histo("EP_Run2011A_FEVT_5_1_bLm.root");
// 	fill_histo("EP_Run2011A_FEVT_6_1_1tq.root");
// 	fill_histo("EP_Run2011A_FEVT_7_1_1Wc.root");
// 	fill_histo("EP_Run2011A_FEVT_8_1_Esm.root");
// 	fill_histo("EP_Run2011A_FEVT_9_1_c3C.root");
// 	fill_histo("EP_Run2011A_FEVT_10_1_H4V.root");
// 	fill_histo("EP_Run2011A_FEVT_11_1_1qI.root");
// 	fill_histo("EP_Run2011A_FEVT_12_1_Urq.root");
// 	fill_histo("EP_Run2011A_FEVT_13_1_Nq3.root");
// 	fill_histo("EP_Run2011A_FEVT_14_1_t8X.root");
// 	fill_histo("EP_Run2011A_FEVT_15_1_QVA.root");
// 	fill_histo("EP_Run2011A_FEVT_16_1_cc1.root");
// 	fill_histo("EP_Run2011A_FEVT_17_1_Vl8.root");
// 	fill_histo("EP_Run2011A_FEVT_18_1_Tz8.root");
// 	fill_histo("EP_Run2011A_FEVT_19_1_qOk.root");
// 	fill_histo("EP_Run2011A_FEVT_20_1_BVJ.root");
// 	fill_histo("EP_Run2011A_FEVT_21_1_t9O.root");
// 	fill_histo("EP_Run2011A_FEVT_22_1_kXK.root");
// 	fill_histo("EP_Run2011A_FEVT_23_1_RZx.root");
// 	fill_histo("EP_Run2011A_FEVT_24_1_XbH.root");
// 	fill_histo("EP_Run2011A_FEVT_25_1_5Wz.root");
// 	fill_histo("EP_Run2011A_FEVT_26_1_qy2.root");
// 	fill_histo("EP_Run2011A_FEVT_27_1_EEE.root");
// 	fill_histo("EP_Run2011A_FEVT_28_1_GtC.root");
// 	fill_histo("EP_Run2011A_FEVT_29_1_atr.root");
// 	fill_histo("EP_Run2011A_FEVT_30_1_iKw.root");
// 	fill_histo("EP_Run2011A_FEVT_31_1_ibD.root");
// 	fill_histo("EP_Run2011A_FEVT_32_1_I8C.root");
// 	fill_histo("EP_Run2011A_FEVT_33_1_W6W.root");
// 	fill_histo("EP_Run2011A_FEVT_34_1_o9s.root");
// 	fill_histo("EP_Run2011A_FEVT_35_1_HfI.root");
// 	fill_histo("EP_Run2011A_FEVT_36_1_03A.root");
// 	fill_histo("EP_Run2011A_FEVT_37_1_1sk.root");
// 	fill_histo("EP_Run2011A_FEVT_38_1_bK6.root");
// 	fill_histo("EP_Run2011A_FEVT_39_1_O50.root");
// 	fill_histo("EP_Run2011A_FEVT_40_1_aaf.root");
// 	fill_histo("EP_Run2011A_FEVT_41_1_os1.root");
// 	fill_histo("EP_Run2011A_FEVT_42_1_0pn.root");
// 	fill_histo("EP_Run2011A_FEVT_43_1_Yjw.root");
// 	fill_histo("EP_Run2011A_FEVT_44_1_cxB.root");
// 	fill_histo("EP_Run2011A_FEVT_45_1_GmN.root");
// 	fill_histo("EP_Run2011A_FEVT_46_1_DV0.root");
// 	fill_histo("EP_Run2011A_FEVT_47_1_IH1.root");
// 	fill_histo("EP_Run2011A_FEVT_48_1_0xO.root");
// 	fill_histo("EP_Run2011A_FEVT_49_1_j24.root");
// 	fill_histo("EP_Run2011A_FEVT_50_1_945.root");
// 	fill_histo("EP_Run2011A_FEVT_51_1_mhb.root");
// 	fill_histo("EP_Run2011A_FEVT_52_1_UnI.root");
// 	fill_histo("EP_Run2011A_FEVT_53_1_2Ua.root");
// 	fill_histo("EP_Run2011A_FEVT_54_1_q4Q.root");
// 	fill_histo("EP_Run2011A_FEVT_55_1_9tH.root");
// 	fill_histo("EP_Run2011A_FEVT_56_1_PH2.root");
// 	fill_histo("EP_Run2011A_FEVT_57_1_Fcm.root");
// 	fill_histo("EP_Run2011A_FEVT_58_1_Vga.root");
// 	fill_histo("EP_Run2011A_FEVT_59_1_fZ6.root");
// 	fill_histo("EP_Run2011A_FEVT_60_1_NSm.root");
// 	fill_histo("EP_Run2011A_FEVT_61_1_q4W.root");
// 	fill_histo("EP_Run2011A_FEVT_62_1_5vL.root");
// 	fill_histo("EP_Run2011A_FEVT_63_1_2Ca.root");
// 	fill_histo("EP_Run2011A_FEVT_64_1_CcV.root");
// 	fill_histo("EP_Run2011A_FEVT_65_1_yUr.root");
// 	fill_histo("EP_Run2011A_FEVT_66_1_kW5.root");
// 	fill_histo("EP_Run2011A_FEVT_67_1_fn8.root");
// 	fill_histo("EP_Run2011A_FEVT_68_1_sNN.root");
// 	fill_histo("EP_Run2011A_FEVT_69_1_UHP.root");
// 	fill_histo("EP_Run2011A_FEVT_70_1_Y2M.root");
// 	fill_histo("EP_Run2011A_FEVT_71_1_G9U.root");
// 	fill_histo("EP_Run2011A_FEVT_72_1_pwU.root");
// 	fill_histo("EP_Run2011A_FEVT_73_1_6br.root");
// 	fill_histo("EP_Run2011A_FEVT_74_1_4u0.root");
// 	fill_histo("EP_Run2011A_FEVT_75_1_5nk.root");
// 	fill_histo("EP_Run2011A_FEVT_76_1_KKn.root");
// 	fill_histo("EP_Run2011A_FEVT_77_1_tSs.root");
// 	fill_histo("EP_Run2011A_FEVT_78_1_cXC.root");
// 	fill_histo("EP_Run2011A_FEVT_79_1_LLp.root");
// 	fill_histo("EP_Run2011A_FEVT_80_1_ZaJ.root");
// 	fill_histo("EP_Run2011A_FEVT_81_1_Xa7.root");
// 	fill_histo("EP_Run2011A_FEVT_82_1_k3Q.root");
// 	fill_histo("EP_Run2011A_FEVT_83_1_eQl.root");
// 	fill_histo("EP_Run2011A_FEVT_84_1_cUj.root");
// 	fill_histo("EP_Run2011A_FEVT_85_1_GgN.root");
// 	fill_histo("EP_Run2011A_FEVT_86_1_pPT.root");
// 	fill_histo("EP_Run2011A_FEVT_87_1_37q.root");
// 	fill_histo("EP_Run2011A_FEVT_88_1_TsG.root");
// 	fill_histo("EP_Run2011A_FEVT_89_1_94g.root");
// 	fill_histo("EP_Run2011A_FEVT_90_1_6d1.root");
// 	fill_histo("EP_Run2011A_FEVT_91_1_4B7.root");
// 	fill_histo("EP_Run2011A_FEVT_92_1_mXU.root");
// 	fill_histo("EP_Run2011A_FEVT_93_1_sLT.root");
// 	fill_histo("EP_Run2011A_FEVT_94_1_xtj.root");
// 	fill_histo("EP_Run2011A_FEVT_95_1_e5p.root");
// 	fill_histo("EP_Run2011A_FEVT_96_1_ceP.root");
// 	fill_histo("EP_Run2011A_FEVT_97_1_zvQ.root");
// 	fill_histo("EP_Run2011A_FEVT_98_1_yE5.root");
// 	fill_histo("EP_Run2011A_FEVT_99_1_zdI.root");
// 	fill_histo("EP_Run2011A_FEVT_100_1_rVX.root");
// 	fill_histo("EP_Run2011A_FEVT_101_1_1ne.root");
// 	fill_histo("EP_Run2011A_FEVT_102_1_wQ9.root");
// 	fill_histo("EP_Run2011A_FEVT_103_1_9gS.root");
// 	fill_histo("EP_Run2011A_FEVT_104_1_YKX.root");
// 	fill_histo("EP_Run2011A_FEVT_105_1_MBL.root");
// 	fill_histo("EP_Run2011A_FEVT_106_1_WqS.root");
// 	fill_histo("EP_Run2011A_FEVT_107_1_QSO.root");
// 	fill_histo("EP_Run2011A_FEVT_108_1_C2z.root");
// 	fill_histo("EP_Run2011A_FEVT_109_1_ZZ5.root");
// 	fill_histo("EP_Run2011A_FEVT_110_1_DKe.root");
// 	fill_histo("EP_Run2011A_FEVT_111_1_8NQ.root");
// 	fill_histo("EP_Run2011A_FEVT_112_1_JaP.root");
// 	fill_histo("EP_Run2011A_FEVT_113_1_pLu.root");
// 	fill_histo("EP_Run2011A_FEVT_114_1_yc5.root");
// 	fill_histo("EP_Run2011A_FEVT_115_1_9a4.root");
// 	fill_histo("EP_Run2011A_FEVT_116_1_Xh4.root");
// 	fill_histo("EP_Run2011A_FEVT_117_1_4HN.root");
// 	fill_histo("EP_Run2011A_FEVT_118_1_CZb.root");
// 	fill_histo("EP_Run2011A_FEVT_119_1_K6B.root");
// 	fill_histo("EP_Run2011A_FEVT_120_1_ThO.root");
// 	fill_histo("EP_Run2011A_FEVT_121_1_6wQ.root");
// 	fill_histo("EP_Run2011A_FEVT_122_1_rwv.root");
// 	fill_histo("EP_Run2011A_FEVT_123_1_I0f.root");
// 	fill_histo("EP_Run2011A_FEVT_124_1_cYE.root");
// 	fill_histo("EP_Run2011A_FEVT_125_1_ohf.root");
// 	fill_histo("EP_Run2011A_FEVT_126_1_YEZ.root");
// 	fill_histo("EP_Run2011A_FEVT_127_1_wU9.root");
// 	fill_histo("EP_Run2011A_FEVT_128_1_Jr1.root");
// 	fill_histo("EP_Run2011A_FEVT_129_1_7KT.root");
// 	fill_histo("EP_Run2011A_FEVT_130_1_ojw.root");
// 	fill_histo("EP_Run2011A_FEVT_131_1_hyK.root");
// 	fill_histo("EP_Run2011A_FEVT_132_1_wmE.root");
// 	fill_histo("EP_Run2011A_FEVT_133_1_cog.root");
// 	fill_histo("EP_Run2011A_FEVT_134_1_ZBJ.root");
// 	fill_histo("EP_Run2011A_FEVT_135_1_fsD.root");
// 	fill_histo("EP_Run2011A_FEVT_136_1_Ltd.root");
// 	fill_histo("EP_Run2011A_FEVT_137_1_EKt.root");
// 	fill_histo("EP_Run2011A_FEVT_138_1_EJp.root");
// 	fill_histo("EP_Run2011A_FEVT_139_1_2Jf.root");
// 	fill_histo("EP_Run2011A_FEVT_140_1_H19.root");
// 	fill_histo("EP_Run2011A_FEVT_141_1_mn3.root");
// 	fill_histo("EP_Run2011A_FEVT_142_1_DYU.root");
// 	fill_histo("EP_Run2011A_FEVT_143_1_Xb5.root");
// 	fill_histo("EP_Run2011A_FEVT_144_1_y2n.root");
// 	fill_histo("EP_Run2011A_FEVT_145_2_DLP.root");
// 	fill_histo("EP_Run2011A_FEVT_146_1_H60.root");
// 	fill_histo("EP_Run2011A_FEVT_147_1_TG1.root");
// 	fill_histo("EP_Run2011A_FEVT_148_1_29z.root");
// 	fill_histo("EP_Run2011A_FEVT_149_1_258.root");
// 	fill_histo("EP_Run2011A_FEVT_150_1_pfd.root");
// 	fill_histo("EP_Run2011A_FEVT_151_1_WRZ.root");
// 	fill_histo("EP_Run2011A_FEVT_152_1_vF9.root");
// 	fill_histo("EP_Run2011A_FEVT_153_1_fu8.root");
// 	fill_histo("EP_Run2011A_FEVT_154_1_OJM.root");
// 	fill_histo("EP_Run2011A_FEVT_155_1_CiX.root");
// 	fill_histo("EP_Run2011A_FEVT_156_1_L0c.root");
// 	fill_histo("EP_Run2011A_FEVT_157_1_FpX.root");
// 	fill_histo("EP_Run2011A_FEVT_158_1_5db.root");
// 	fill_histo("EP_Run2011A_FEVT_159_1_jRu.root");
// 	fill_histo("EP_Run2011A_FEVT_160_1_hGf.root");
// 	fill_histo("EP_Run2011A_FEVT_161_1_TCS.root");
// 	fill_histo("EP_Run2011A_FEVT_162_1_7u6.root");
// 	fill_histo("EP_Run2011A_FEVT_163_1_Wss.root");
// 	fill_histo("EP_Run2011A_FEVT_164_1_u1s.root");
// 	fill_histo("EP_Run2011A_FEVT_165_1_2NH.root");
// 	fill_histo("EP_Run2011A_FEVT_166_1_ZVw.root");
// 	fill_histo("EP_Run2011A_FEVT_167_1_qqk.root");
// 	fill_histo("EP_Run2011A_FEVT_168_1_mDq.root");
// 	fill_histo("EP_Run2011A_FEVT_169_1_PvK.root");
// 	fill_histo("EP_Run2011A_FEVT_170_1_tGM.root");
// 	fill_histo("EP_Run2011A_FEVT_171_1_HZl.root");
// 	fill_histo("EP_Run2011A_FEVT_172_1_T51.root");
// 	fill_histo("EP_Run2011A_FEVT_173_1_5Pf.root");
// 	fill_histo("EP_Run2011A_FEVT_174_1_kC6.root");
// 	fill_histo("EP_Run2011A_FEVT_175_1_fNB.root");
// 	fill_histo("EP_Run2011A_FEVT_176_1_uW4.root");
// 	fill_histo("EP_Run2011A_FEVT_177_1_Cyh.root");
// 	fill_histo("EP_Run2011A_FEVT_178_1_rOX.root");
// 	fill_histo("EP_Run2011A_FEVT_179_1_ka7.root");
// 	fill_histo("EP_Run2011A_FEVT_180_1_Ngt.root");
// 	fill_histo("EP_Run2011A_FEVT_181_1_yAn.root");
// 	fill_histo("EP_Run2011A_FEVT_182_1_MT8.root");
// 	fill_histo("EP_Run2011A_FEVT_183_1_AQx.root");
// 	fill_histo("EP_Run2011A_FEVT_184_1_4vD.root");
// 	fill_histo("EP_Run2011A_FEVT_185_1_4TQ.root");
// 	fill_histo("EP_Run2011A_FEVT_186_1_hVv.root");
// 	fill_histo("EP_Run2011A_FEVT_187_1_108.root");
// 	fill_histo("EP_Run2011A_FEVT_188_1_7ry.root");

	filedir = "/home/jkarancs/data/gridout/CMSSW_4_1_2/ExpressPhysics_Run2011A-Express-v1_FEVT/12ns_runs/";
// 	filedir = "/tmp/jkarancs/12ns_runs/";

// 	orbmin = 0;

// 	fill_histo("EP_Run2011A_FEVT_1_1_oab.root");
// 	fill_histo("EP_Run2011A_FEVT_2_1_hjn.root");
// 	fill_histo("EP_Run2011A_FEVT_3_1_d7g.root");
// 	fill_histo("EP_Run2011A_FEVT_4_1_1Xt.root");
// 	fill_histo("EP_Run2011A_FEVT_5_1_tf6.root");
// 	fill_histo("EP_Run2011A_FEVT_6_1_uvd.root");
// 	fill_histo("EP_Run2011A_FEVT_7_1_U7V.root");
// 	fill_histo("EP_Run2011A_FEVT_8_1_z7i.root");
// 	fill_histo("EP_Run2011A_FEVT_9_1_GPJ.root");
// 	fill_histo("EP_Run2011A_FEVT_10_1_LPw.root");
// 	fill_histo("EP_Run2011A_FEVT_11_1_oIl.root");
// 	fill_histo("EP_Run2011A_FEVT_12_1_RQU.root");
// 	fill_histo("EP_Run2011A_FEVT_13_1_feQ.root");
// 	fill_histo("EP_Run2011A_FEVT_14_1_ymG.root");
// 	fill_histo("EP_Run2011A_FEVT_15_1_0pN.root");
// 	fill_histo("EP_Run2011A_FEVT_16_1_NzE.root");
// 	fill_histo("EP_Run2011A_FEVT_17_1_JvG.root");
// 	fill_histo("EP_Run2011A_FEVT_18_1_UUg.root");
// 	fill_histo("EP_Run2011A_FEVT_19_1_aNa.root");
// 	fill_histo("EP_Run2011A_FEVT_20_1_Qtl.root");
// 	fill_histo("EP_Run2011A_FEVT_21_1_Vwx.root");
// 	fill_histo("EP_Run2011A_FEVT_22_1_aUk.root");
// 	fill_histo("EP_Run2011A_FEVT_23_1_d4K.root");
// 	fill_histo("EP_Run2011A_FEVT_24_1_WQV.root");
// 	fill_histo("EP_Run2011A_FEVT_25_1_00j.root");
// 	fill_histo("EP_Run2011A_FEVT_26_1_Ycv.root");
// 	fill_histo("EP_Run2011A_FEVT_27_1_dtR.root");
// 	fill_histo("EP_Run2011A_FEVT_28_1_y1y.root");
// 	fill_histo("EP_Run2011A_FEVT_29_1_TWt.root");
// 	fill_histo("EP_Run2011A_FEVT_30_1_XXs.root");
// 	fill_histo("EP_Run2011A_FEVT_31_1_NV0.root");
// 	fill_histo("EP_Run2011A_FEVT_32_1_Y9W.root");
// 	fill_histo("EP_Run2011A_FEVT_33_1_5Wz.root");
// 	fill_histo("EP_Run2011A_FEVT_34_1_DdG.root");
// 	fill_histo("EP_Run2011A_FEVT_35_1_bNA.root");
// 	fill_histo("EP_Run2011A_FEVT_36_1_1qi.root");
// 	fill_histo("EP_Run2011A_FEVT_37_1_ROL.root");
// 	fill_histo("EP_Run2011A_FEVT_38_1_PZc.root");
// 	fill_histo("EP_Run2011A_FEVT_39_1_Qha.root");
// 	fill_histo("EP_Run2011A_FEVT_40_1_83z.root");
// 	fill_histo("EP_Run2011A_FEVT_41_1_k7W.root");
// 	fill_histo("EP_Run2011A_FEVT_42_1_wPE.root");
// 	fill_histo("EP_Run2011A_FEVT_43_1_mnJ.root");
// 	fill_histo("EP_Run2011A_FEVT_44_1_LRT.root");
// 	fill_histo("EP_Run2011A_FEVT_45_1_znP.root");
// 	fill_histo("EP_Run2011A_FEVT_46_1_Sic.root");
// 	fill_histo("EP_Run2011A_FEVT_47_1_yho.root");
// 	fill_histo("EP_Run2011A_FEVT_48_1_UYb.root");
// 	fill_histo("EP_Run2011A_FEVT_49_1_IWy.root");
// 	fill_histo("EP_Run2011A_FEVT_50_1_eO0.root");
// 	fill_histo("EP_Run2011A_FEVT_51_1_zLg.root");
// 	fill_histo("EP_Run2011A_FEVT_52_1_e14.root");
// 	fill_histo("EP_Run2011A_FEVT_53_1_Bnb.root");
// 	fill_histo("EP_Run2011A_FEVT_54_1_WA2.root");
// 	fill_histo("EP_Run2011A_FEVT_55_1_vn7.root");
// 	fill_histo("EP_Run2011A_FEVT_56_1_Jsz.root");
// 	fill_histo("EP_Run2011A_FEVT_57_1_OUN.root");
// 	fill_histo("EP_Run2011A_FEVT_58_1_Cty.root");
// 	fill_histo("EP_Run2011A_FEVT_59_1_i17.root");
// 	fill_histo("EP_Run2011A_FEVT_60_1_dXx.root");
// 	fill_histo("EP_Run2011A_FEVT_61_1_LOa.root");
// 	fill_histo("EP_Run2011A_FEVT_62_1_ZuB.root");
// 	fill_histo("EP_Run2011A_FEVT_63_1_9Us.root");
// 	fill_histo("EP_Run2011A_FEVT_64_1_1A5.root");
// 	fill_histo("EP_Run2011A_FEVT_65_1_qHQ.root");
// 	fill_histo("EP_Run2011A_FEVT_66_1_ykg.root");
// 	fill_histo("EP_Run2011A_FEVT_67_1_qj0.root");
// 	fill_histo("EP_Run2011A_FEVT_68_1_HVY.root");
// 	fill_histo("EP_Run2011A_FEVT_69_1_hs1.root");
// 	fill_histo("EP_Run2011A_FEVT_70_1_K9N.root");
// 	fill_histo("EP_Run2011A_FEVT_71_1_EgS.root");
// 	fill_histo("EP_Run2011A_FEVT_72_1_fGH.root");
// 	fill_histo("EP_Run2011A_FEVT_73_1_pov.root");
// 	fill_histo("EP_Run2011A_FEVT_74_1_6P8.root");
// 	fill_histo("EP_Run2011A_FEVT_75_1_1XN.root");
// 	fill_histo("EP_Run2011A_FEVT_76_1_y27.root");
// 	fill_histo("EP_Run2011A_FEVT_77_1_ILB.root");
// 	fill_histo("EP_Run2011A_FEVT_78_1_aPs.root");
// 	fill_histo("EP_Run2011A_FEVT_79_1_GsM.root");
// 	fill_histo("EP_Run2011A_FEVT_80_1_FJH.root");
// 	fill_histo("EP_Run2011A_FEVT_81_1_9fh.root");
// 	fill_histo("EP_Run2011A_FEVT_82_1_6KT.root");
// 	fill_histo("EP_Run2011A_FEVT_83_1_Q6b.root");
// 	fill_histo("EP_Run2011A_FEVT_84_1_RfP.root");
// 	fill_histo("EP_Run2011A_FEVT_85_1_Csy.root");
// 	fill_histo("EP_Run2011A_FEVT_86_1_UsW.root");
// 	fill_histo("EP_Run2011A_FEVT_87_1_ho5.root");
// 	fill_histo("EP_Run2011A_FEVT_88_1_q1m.root");
// 	fill_histo("EP_Run2011A_FEVT_89_1_akH.root");
// 	fill_histo("EP_Run2011A_FEVT_90_1_Fcz.root");
// 	fill_histo("EP_Run2011A_FEVT_91_1_0sT.root");
// 	fill_histo("EP_Run2011A_FEVT_92_1_LOY.root");
// 	fill_histo("EP_Run2011A_FEVT_93_1_ULs.root");
// 	fill_histo("EP_Run2011A_FEVT_94_1_zzD.root");
// 	fill_histo("EP_Run2011A_FEVT_95_1_kD1.root");
// 	fill_histo("EP_Run2011A_FEVT_96_1_Dzg.root");
// 	fill_histo("EP_Run2011A_FEVT_97_1_xtp.root");
// 	fill_histo("EP_Run2011A_FEVT_98_1_1tY.root");
// 	fill_histo("EP_Run2011A_FEVT_99_1_G09.root");
// 	fill_histo("EP_Run2011A_FEVT_100_1_DxR.root");
// 	fill_histo("EP_Run2011A_FEVT_101_1_XX8.root");
// 	fill_histo("EP_Run2011A_FEVT_102_1_iTF.root");
// 	fill_histo("EP_Run2011A_FEVT_103_1_XAj.root");
// 	fill_histo("EP_Run2011A_FEVT_104_1_mD2.root");
// 	fill_histo("EP_Run2011A_FEVT_105_1_oHh.root");
// 	fill_histo("EP_Run2011A_FEVT_106_1_6sn.root");
// 	fill_histo("EP_Run2011A_FEVT_107_1_H5P.root");
// 	fill_histo("EP_Run2011A_FEVT_108_1_GoC.root");
// 	fill_histo("EP_Run2011A_FEVT_109_1_59v.root");
// 	fill_histo("EP_Run2011A_FEVT_110_1_jlA.root");
// 	fill_histo("EP_Run2011A_FEVT_111_1_Tw8.root");
// 	fill_histo("EP_Run2011A_FEVT_112_1_kdI.root");
// 	fill_histo("EP_Run2011A_FEVT_113_1_u3g.root");
// 	fill_histo("EP_Run2011A_FEVT_114_1_jgg.root");
// 	fill_histo("EP_Run2011A_FEVT_115_1_7kf.root");
// 	fill_histo("EP_Run2011A_FEVT_116_1_t1Y.root");
// 	fill_histo("EP_Run2011A_FEVT_117_1_UQP.root");
// 	fill_histo("EP_Run2011A_FEVT_118_1_kUH.root");
// 	fill_histo("EP_Run2011A_FEVT_119_1_Zzv.root");
// 	fill_histo("EP_Run2011A_FEVT_120_1_HMB.root");
// 	fill_histo("EP_Run2011A_FEVT_121_1_Q9T.root");
// 	fill_histo("EP_Run2011A_FEVT_122_1_ncN.root");
// 	fill_histo("EP_Run2011A_FEVT_123_1_JW8.root");
// 	fill_histo("EP_Run2011A_FEVT_124_1_l3U.root");
// 	fill_histo("EP_Run2011A_FEVT_125_1_pTi.root");
// 	fill_histo("EP_Run2011A_FEVT_126_1_QqR.root");
// 	fill_histo("EP_Run2011A_FEVT_127_1_US9.root");
// 	fill_histo("EP_Run2011A_FEVT_128_1_5YT.root");
// 	fill_histo("EP_Run2011A_FEVT_129_1_Fu3.root");
// 	fill_histo("EP_Run2011A_FEVT_130_1_jL8.root");
// 	fill_histo("EP_Run2011A_FEVT_131_1_bet.root");
// 	fill_histo("EP_Run2011A_FEVT_132_1_AvH.root");
// 	fill_histo("EP_Run2011A_FEVT_133_1_BmJ.root");
// 	fill_histo("EP_Run2011A_FEVT_134_1_HFm.root");
// 	fill_histo("EP_Run2011A_FEVT_135_1_RK5.root");
// 	fill_histo("EP_Run2011A_FEVT_136_1_HCe.root");
// 	fill_histo("EP_Run2011A_FEVT_137_1_anC.root");
// 	fill_histo("EP_Run2011A_FEVT_138_1_tcd.root");
// 	fill_histo("EP_Run2011A_FEVT_139_1_uPH.root");
// 	fill_histo("EP_Run2011A_FEVT_140_1_44i.root");
// 	fill_histo("EP_Run2011A_FEVT_141_1_YSL.root");
// 	fill_histo("EP_Run2011A_FEVT_142_1_TwQ.root");
// 	fill_histo("EP_Run2011A_FEVT_143_1_F12.root");
// 	fill_histo("EP_Run2011A_FEVT_144_1_bT6.root");
// 	fill_histo("EP_Run2011A_FEVT_145_1_dQr.root");
// 	fill_histo("EP_Run2011A_FEVT_146_1_Htf.root");
// 	fill_histo("EP_Run2011A_FEVT_147_1_0Af.root");

	filedir = "/home/jkarancs/data/gridout/CMSSW_4_1_2/ExpressPhysics_Run2011A-Express-v1_FEVT/HV2/";
// 	filedir = "/tmp/jkarancs/HV2/";

// 	fill_histo("EP_Run2011A_FEVT_1_1_wsH.root");
// 	fill_histo("EP_Run2011A_FEVT_2_1_Gvz.root");
// 	fill_histo("EP_Run2011A_FEVT_3_1_7lh.root");
// 	fill_histo("EP_Run2011A_FEVT_4_1_PEV.root");
// 	fill_histo("EP_Run2011A_FEVT_5_1_hxj.root");
// 	fill_histo("EP_Run2011A_FEVT_6_1_DDg.root");
// 	fill_histo("EP_Run2011A_FEVT_7_1_0jB.root");
// 	fill_histo("EP_Run2011A_FEVT_8_1_3RG.root");
// 	fill_histo("EP_Run2011A_FEVT_9_1_NTe.root");
// 	fill_histo("EP_Run2011A_FEVT_10_1_CEy.root");
// 	fill_histo("EP_Run2011A_FEVT_11_1_tfB.root");
// 	fill_histo("EP_Run2011A_FEVT_12_1_OjP.root");
// 	fill_histo("EP_Run2011A_FEVT_13_1_5m1.root");


	// MC - incomplete
//      filedir = "/home/jkarancs/data/gridout/CMSSW_3_8_5/MinBias_TuneD6T_7TeV-pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO/v1815/";
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_1_4_bon.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_2_4_AXA.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_3_4_iLC.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_4_3_czT.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_5_4_TKn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_6_4_mI5.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_7_4_7lF.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_8_4_lot.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_9_4_Cmg.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_10_3_HRu.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_11_4_oAW.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_12_4_R9y.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_13_3_Er9.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_14_4_Adh.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_15_3_hYT.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_16_4_Pcw.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_17_3_mP6.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_18_4_8vn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_19_4_iLD.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_20_4_LsE.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_21_4_Z16.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_22_4_62h.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_23_3_MmF.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_24_4_07U.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_25_4_68q.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_26_4_oBl.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_27_4_wAw.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_28_4_9Of.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_29_4_vqO.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_30_4_Fzn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_31_3_Gbb.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_32_4_GFg.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_33_4_8eO.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_34_4_pOX.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_36_4_LKk.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_37_3_eEQ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_38_4_6gH.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_39_4_pvg.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_40_4_U1X.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_41_4_yTE.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_42_4_WBv.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_43_4_wyx.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_44_4_wMR.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_45_4_0P4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_46_4_v00.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_47_3_8p8.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_48_4_UIn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_49_4_ynP.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_50_4_reD.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_51_4_fZV.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_52_4_mtf.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_53_4_nXS.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_54_4_jZ7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_55_4_6yE.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_56_3_nhk.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_57_4_NCG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_58_4_ECq.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_59_4_zFV.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_61_4_DcM.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_62_3_wlB.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_63_3_P6w.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_64_3_jUd.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_65_4_hQa.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_66_4_2kU.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_67_4_EW4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_68_4_oIG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_69_4_zZX.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_70_4_sui.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_71_4_Ght.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_72_4_dIy.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_73_4_wE8.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_74_3_usG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_75_4_Bim.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_76_3_cPn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_77_3_2K8.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_78_4_lLQ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_79_4_O2G.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_80_4_fFm.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_81_4_m3E.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_82_4_Ms4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_83_4_5b3.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_84_4_zOd.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_86_4_uTF.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_87_4_Bnp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_88_4_ukz.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_89_4_7TS.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_90_4_3uk.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_91_4_FvZ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_92_4_GDM.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_93_4_a0T.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_94_4_cbs.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_95_4_4NU.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_96_4_sVb.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_97_4_zJG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_98_4_mrP.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_99_4_7ht.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_100_4_zMo.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_101_3_VvX.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_102_4_kjb.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_103_4_QWZ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_104_4_4zA.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_105_3_ywD.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_106_4_5Qv.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_107_4_Tke.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_108_4_fH4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_109_4_bZq.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_110_4_Mwb.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_111_4_C1c.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_112_4_MzX.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_113_4_n0v.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_114_4_9tR.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_115_4_XTT.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_116_4_13R.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_117_4_ahz.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_118_4_x8X.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_119_4_s7T.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_120_4_a8o.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_121_4_2aH.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_122_3_Vse.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_123_4_saa.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_124_4_ir7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_125_3_l5U.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_126_4_knk.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_127_3_7u3.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_128_4_boy.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_129_4_Bzp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_130_4_von.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_131_4_EKA.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_132_4_Gei.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_133_4_Me7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_134_4_UDu.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_135_4_ZKY.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_136_4_3Z1.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_137_4_ZE4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_138_4_dHM.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_139_3_9WQ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_140_3_YOT.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_141_4_Hk6.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_142_4_XAD.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_143_4_Hdn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_144_4_xvB.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_145_4_MjU.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_146_3_srS.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_147_4_Ggn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_148_4_Pra.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_149_4_FVV.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_150_4_odZ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_151_4_GtM.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_152_4_AvB.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_153_4_tos.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_154_4_5hV.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_155_4_183.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_156_3_YSp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_157_4_AyJ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_158_3_nxB.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_159_4_44w.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_160_4_60Y.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_161_4_mKc.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_162_4_fuO.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_163_3_kyZ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_164_4_kFt.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_165_4_Ex4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_166_3_Ko9.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_167_4_WMp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_168_4_UyK.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_169_4_urA.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_170_3_G42.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_171_4_Jch.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_172_4_n2t.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_173_4_naT.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_174_4_wAT.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_175_4_wQk.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_176_4_jED.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_177_4_B87.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_178_4_kJ7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_180_3_RPM.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_181_4_IMp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_182_4_2tY.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_183_4_nQn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_184_3_1Jr.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_185_4_te7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_186_4_dPL.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_187_4_Jk7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_189_4_IZw.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_190_4_RZy.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_191_4_9Zt.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_192_4_Us7.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_193_4_oIG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_194_4_aNn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_195_4_0IB.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_196_4_GAg.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_197_4_HqK.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_198_4_zKC.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_199_4_h7Z.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_200_4_FME.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_201_3_PtD.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_202_3_di4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_203_4_F10.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_204_4_ubZ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_205_4_DXp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_206_4_hKN.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_207_4_JBG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_208_4_cPg.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_209_4_l7V.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_210_4_9qp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_211_3_5P8.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_212_4_BCX.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_213_4_PFv.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_214_4_irY.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_215_4_7gn.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_216_4_t91.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_217_3_H8w.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_218_4_yZe.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_219_4_kGS.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_220_3_yqW.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_221_4_QNy.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_222_4_bBu.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_223_4_9Ex.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_224_4_3A3.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_225_4_ai6.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_226_4_9Jz.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_227_4_OmG.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_228_4_bF8.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_229_4_aRq.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_230_3_QhC.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_231_4_3Cl.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_232_4_Ou9.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_233_4_0pq.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_234_4_GRw.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_235_4_H11.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_236_4_upy.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_237_4_xPN.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_239_4_ao4.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_240_4_FTv.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_241_3_ywp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_242_4_q4U.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_244_4_gH6.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_245_4_n1Z.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_246_3_dhp.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_246_4_Bsu.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_247_3_ltJ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_248_4_S5w.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_249_4_18Z.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_250_4_fYa.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_251_3_qPl.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_252_3_7Hi.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_253_4_XWM.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_254_4_Dzx.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_255_4_XDh.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_256_4_eoa.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_257_4_VIJ.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_258_4_9ed.root");
//         fill_histo("MC_MB_TuneD6T_7TeV-pythia6_259_4_Eg0.root");
      } else {
	// newfill 146804-147390, excluding 147116, 147196 - #define COMPLETE needed in datastruct
	filedir = "/home/jkarancs/data/gridout/CMSSW_3_9_7/MinimumBias_Run2010B-Dec22ReReco_v1_RECO/v1815/";
        fill_histo("2010B_Dec22ReReco_1_1_fmC.root");
//         fill_histo("2010B_Dec22ReReco_2_1_uMs.root");
//         fill_histo("2010B_Dec22ReReco_3_1_Kep.root");
//         fill_histo("2010B_Dec22ReReco_4_1_MUm.root");
//         fill_histo("2010B_Dec22ReReco_5_1_Hij.root");
//         fill_histo("2010B_Dec22ReReco_6_1_kon.root");
//         fill_histo("2010B_Dec22ReReco_7_1_ehU.root");
//         fill_histo("2010B_Dec22ReReco_8_1_MWv.root");
//         fill_histo("2010B_Dec22ReReco_9_1_IfK.root");
//         fill_histo("2010B_Dec22ReReco_10_1_9Tq.root");
//         fill_histo("2010B_Dec22ReReco_11_1_YhX.root");
//         fill_histo("2010B_Dec22ReReco_12_1_0sw.root");
//         fill_histo("2010B_Dec22ReReco_13_1_h7C.root");
//         fill_histo("2010B_Dec22ReReco_14_1_aKd.root");
//         fill_histo("2010B_Dec22ReReco_15_1_Pa8.root");
//         fill_histo("2010B_Dec22ReReco_16_1_uwc.root");
//         fill_histo("2010B_Dec22ReReco_17_1_tiU.root");
//         fill_histo("2010B_Dec22ReReco_18_1_LE0.root");
//         fill_histo("2010B_Dec22ReReco_19_1_9f8.root");
//         fill_histo("2010B_Dec22ReReco_20_1_68l.root");
//         fill_histo("2010B_Dec22ReReco_21_1_cOO.root");
//         fill_histo("2010B_Dec22ReReco_22_1_KT4.root");
//         fill_histo("2010B_Dec22ReReco_23_1_Ehs.root");
//         fill_histo("2010B_Dec22ReReco_24_1_XnN.root");
//         fill_histo("2010B_Dec22ReReco_25_1_ZSv.root");
//         fill_histo("2010B_Dec22ReReco_26_1_S3G.root");
//         fill_histo("2010B_Dec22ReReco_27_1_FdB.root");
//         fill_histo("2010B_Dec22ReReco_28_1_cgN.root");
//         fill_histo("2010B_Dec22ReReco_29_1_3FP.root");
//         fill_histo("2010B_Dec22ReReco_30_1_Yfc.root");
//         fill_histo("2010B_Dec22ReReco_31_1_LH6.root");
//         fill_histo("2010B_Dec22ReReco_32_1_dOY.root");
//         fill_histo("2010B_Dec22ReReco_33_1_GkN.root");
//         fill_histo("2010B_Dec22ReReco_34_1_avE.root");
//         fill_histo("2010B_Dec22ReReco_35_1_9L3.root");
//         fill_histo("2010B_Dec22ReReco_36_1_Bsx.root");
//         fill_histo("2010B_Dec22ReReco_37_1_B1U.root");
//         fill_histo("2010B_Dec22ReReco_38_1_1Ve.root");
//         fill_histo("2010B_Dec22ReReco_39_1_6IL.root");
//         fill_histo("2010B_Dec22ReReco_40_1_sof.root");
//         fill_histo("2010B_Dec22ReReco_41_1_GVY.root");
//         fill_histo("2010B_Dec22ReReco_42_1_T3O.root");
//         fill_histo("2010B_Dec22ReReco_43_1_1CS.root");
//         fill_histo("2010B_Dec22ReReco_44_1_PAH.root");
//         fill_histo("2010B_Dec22ReReco_45_1_m6D.root");
//         fill_histo("2010B_Dec22ReReco_46_1_8Vc.root");
//         fill_histo("2010B_Dec22ReReco_47_1_3ps.root");
//         fill_histo("2010B_Dec22ReReco_48_1_brl.root");
//         fill_histo("2010B_Dec22ReReco_49_1_MGS.root");
//         fill_histo("2010B_Dec22ReReco_50_1_11x.root");
//         fill_histo("2010B_Dec22ReReco_51_1_Hj4.root");
//         fill_histo("2010B_Dec22ReReco_52_1_ter.root");
//         fill_histo("2010B_Dec22ReReco_53_1_jRM.root");
//         fill_histo("2010B_Dec22ReReco_54_1_YYS.root");
//         fill_histo("2010B_Dec22ReReco_55_1_ER9.root");
//         fill_histo("2010B_Dec22ReReco_56_1_J7w.root");
//         fill_histo("2010B_Dec22ReReco_57_1_Lpu.root");
//         fill_histo("2010B_Dec22ReReco_58_1_elx.root");
//         fill_histo("2010B_Dec22ReReco_59_1_L5m.root");
//         fill_histo("2010B_Dec22ReReco_60_1_hOQ.root");
//         fill_histo("2010B_Dec22ReReco_61_1_H7F.root");
//         fill_histo("2010B_Dec22ReReco_62_1_3RG.root");
      }
    } else if (pcatomki==1) {
      if (fullrun!=1) {
	// MC-test
	filedir = "/home/jkarancs/data/gridout/CMSSW_3_8_5/MinBias_TuneD6T_7TeV-pythia6_Fall10-START38_V12-v1_GEN-SIM-RECO/v1815/";
// 	fill_histo("MC_MB_TuneD6T_7TeV-pythia6_1_1_cQg.root");
// 	fill_histo("MC_MB_TuneD6T_7TeV-pythia6_2_1_5OD.root");

	// Commissioning10
        // 135537 - 361p4 - probcut=-16
	filedir = "/data/jkarancs/data/gridout/CMSSW_3_6_1_patch4/MinimumBias_Commissioning10-Jun14thReReco_v1_RECO/v1815/";
//        fill_histo("Comm10_361p4_v1815_1_1_hU0.root");
//        fill_histo("Comm10_361p4_v1815_2_1_H3y.root");
//        fill_histo("Comm10_361p4_v1815_3_1_qW4.root");
//        fill_histo("Comm10_361p4_v1815_4_1_UZe.root");
//        fill_histo("Comm10_361p4_v1815_5_1_qsP.root");
//        fill_histo("Comm10_361p4_v1815_6_1_DHO.root");
//        fill_histo("Comm10_361p4_v1815_7_1_isA.root");
//        fill_histo("Comm10_361p4_v1815_8_1_cZT.root");
//        fill_histo("Comm10_361p4_v1815_9_2_XkO.root");
//        fill_histo("Comm10_361p4_v1815_10_1_t1z.root");
//        fill_histo("Comm10_361p4_v1815_11_1_IUx.root");
//        fill_histo("Comm10_361p4_v1815_12_1_4s4.root");
  
    // 135537 361p4 - default
//        fill_histo("Comm10_361p4_v1815_256_1_Heq.root");
//        fill_histo("Comm10_361p4_v1815_257_1_Wxb.root");

	// 2010A
  	filedir = "/home/jkarancs/data/gridout/CMSSW_3_8_6/MinimumBias_Run2010A-Nov4ReReco_v1_RECO/v1815/";
//         fill_histo("2010A_Nov4ReReco_1_2_cTq.root");
//         fill_histo("2010A_Nov4ReReco_2_2_TFk.root");
//         fill_histo("2010A_Nov4ReReco_3_2_JJY.root");
//         fill_histo("2010A_Nov4ReReco_4_2_OYw.root");
//         fill_histo("2010A_Nov4ReReco_5_1_GRh.root");
//         fill_histo("2010A_Nov4ReReco_6_2_pd5.root");
//         fill_histo("2010A_Nov4ReReco_7_2_6oo.root");
//         fill_histo("2010A_Nov4ReReco_8_2_50s.root");
//         fill_histo("2010A_Nov4ReReco_9_2_Nw2.root");
//         fill_histo("2010A_Nov4ReReco_10_2_Ch7.root");
//         fill_histo("2010A_Nov4ReReco_11_2_pHj.root");
//         fill_histo("2010A_Nov4ReReco_12_4_uGf.root");
//         fill_histo("2010A_Nov4ReReco_13_3_kG6.root");
//         fill_histo("2010A_Nov4ReReco_14_2_Abc.root");
//         fill_histo("2010A_Nov4ReReco_15_2_tJW.root");
//         fill_histo("2010A_Nov4ReReco_16_2_4DZ.root");
//         fill_histo("2010A_Nov4ReReco_17_2_KZW.root");
//         fill_histo("2010A_Nov4ReReco_18_4_UUq.root");
//         fill_histo("2010A_Nov4ReReco_19_2_A8M.root");
//         fill_histo("2010A_Nov4ReReco_20_3_yzB.root");
//         fill_histo("2010A_Nov4ReReco_21_2_35t.root");
//         fill_histo("2010A_Nov4ReReco_22_2_QiO.root");
//         fill_histo("2010A_Nov4ReReco_23_3_3Aj.root");
//         fill_histo("2010A_Nov4ReReco_24_2_qdU.root");
//         fill_histo("2010A_Nov4ReReco_25_3_VZu.root");
//         fill_histo("2010A_Nov4ReReco_26_2_nG9.root");
//         fill_histo("2010A_Nov4ReReco_27_2_fSX.root");
//         fill_histo("2010A_Nov4ReReco_28_1_xvL.root");
//         fill_histo("2010A_Nov4ReReco_29_3_SzQ.root");
//         fill_histo("2010A_Nov4ReReco_30_2_4TK.root");
//         fill_histo("2010A_Nov4ReReco_31_2_2Xp.root");
//         fill_histo("2010A_Nov4ReReco_32_2_zkZ.root");
//         fill_histo("2010A_Nov4ReReco_33_2_Ypk.root");
//         fill_histo("2010A_Nov4ReReco_34_2_VmX.root");
//         fill_histo("2010A_Nov4ReReco_35_3_U3g.root");
//         fill_histo("2010A_Nov4ReReco_36_2_gQp.root");
//         fill_histo("2010A_Nov4ReReco_37_2_BTK.root");
//         fill_histo("2010A_Nov4ReReco_38_2_knE.root");
//         fill_histo("2010A_Nov4ReReco_39_2_FNG.root");
//         fill_histo("2010A_Nov4ReReco_40_2_pWo.root");
//         fill_histo("2010A_Nov4ReReco_41_2_SSm.root");
//         fill_histo("2010A_Nov4ReReco_42_2_E7x.root");
//         fill_histo("2010A_Nov4ReReco_43_2_jDu.root");
//         fill_histo("2010A_Nov4ReReco_44_2_jV2.root");
//         fill_histo("2010A_Nov4ReReco_45_3_PRW.root");
//         fill_histo("2010A_Nov4ReReco_46_2_ZTK.root");
//         fill_histo("2010A_Nov4ReReco_47_2_Bad.root");
//         fill_histo("2010A_Nov4ReReco_48_2_b2p.root");
//         fill_histo("2010A_Nov4ReReco_49_1_1aU.root");
//         fill_histo("2010A_Nov4ReReco_50_3_NAx.root");
//         fill_histo("2010A_Nov4ReReco_51_2_uLI.root");
//         fill_histo("2010A_Nov4ReReco_52_2_6H8.root");
//         fill_histo("2010A_Nov4ReReco_53_2_k2X.root");
//         fill_histo("2010A_Nov4ReReco_54_2_4IL.root");
//         fill_histo("2010A_Nov4ReReco_55_4_9O3.root");
//         fill_histo("2010A_Nov4ReReco_56_2_XlW.root");
//         fill_histo("2010A_Nov4ReReco_57_2_QRl.root");
//         fill_histo("2010A_Nov4ReReco_58_3_mCB.root");
//         fill_histo("2010A_Nov4ReReco_59_2_oyi.root");
//         fill_histo("2010A_Nov4ReReco_60_2_HLk.root");
//         fill_histo("2010A_Nov4ReReco_61_3_AAY.root");
//         fill_histo("2010A_Nov4ReReco_62_1_XJs.root");
//         fill_histo("2010A_Nov4ReReco_63_2_aGX.root");
//         fill_histo("2010A_Nov4ReReco_64_3_Xpv.root");
//         fill_histo("2010A_Nov4ReReco_65_2_dWv.root");
//         fill_histo("2010A_Nov4ReReco_66_2_xHR.root");
//         fill_histo("2010A_Nov4ReReco_67_1_Kvd.root");
//         fill_histo("2010A_Nov4ReReco_68_2_tBi.root");
//         fill_histo("2010A_Nov4ReReco_69_2_ldj.root");
//         fill_histo("2010A_Nov4ReReco_70_2_Y2Y.root");
//         fill_histo("2010A_Nov4ReReco_71_2_S7S.root");
//         fill_histo("2010A_Nov4ReReco_72_3_2V8.root");
//         fill_histo("2010A_Nov4ReReco_73_2_9QV.root");
//         fill_histo("2010A_Nov4ReReco_74_3_K1H.root");
//         fill_histo("2010A_Nov4ReReco_75_2_zxb.root");
//         fill_histo("2010A_Nov4ReReco_76_2_2XC.root");
//         fill_histo("2010A_Nov4ReReco_77_1_mly.root");
//         fill_histo("2010A_Nov4ReReco_78_2_rSI.root");
//         fill_histo("2010A_Nov4ReReco_79_1_WNl.root");
//         fill_histo("2010A_Nov4ReReco_80_2_Umf.root");
//         fill_histo("2010A_Nov4ReReco_81_2_jBw.root");
//         fill_histo("2010A_Nov4ReReco_82_2_JPA.root");
//         fill_histo("2010A_Nov4ReReco_83_2_TQf.root");
//         fill_histo("2010A_Nov4ReReco_84_2_nxD.root");
//         fill_histo("2010A_Nov4ReReco_85_2_BDc.root");
//         fill_histo("2010A_Nov4ReReco_86_2_6sN.root");
//         fill_histo("2010A_Nov4ReReco_87_2_lcY.root");
//         fill_histo("2010A_Nov4ReReco_88_2_lO7.root");
//         fill_histo("2010A_Nov4ReReco_89_3_6aE.root");
//         fill_histo("2010A_Nov4ReReco_90_2_QRA.root");
//         fill_histo("2010A_Nov4ReReco_91_2_JA8.root");
//         fill_histo("2010A_Nov4ReReco_92_2_Eow.root");
//         fill_histo("2010A_Nov4ReReco_93_2_2j3.root");
//         fill_histo("2010A_Nov4ReReco_94_3_4bJ.root");
//         fill_histo("2010A_Nov4ReReco_95_2_XCs.root");
//         fill_histo("2010A_Nov4ReReco_96_1_Q9c.root");
//         fill_histo("2010A_Nov4ReReco_97_2_BC3.root");
//         fill_histo("2010A_Nov4ReReco_98_2_hqR.root");
//         fill_histo("2010A_Nov4ReReco_99_2_LXl.root");
//         fill_histo("2010A_Nov4ReReco_100_2_vwI.root");

	// 2010B
  	filedir = "/data/jkarancs/data/gridout/CMSSW_3_8_6/MinimumBias_Run2010B-Nov4ReReco_v1_RECO/v1815/";
//         fill_histo("2010A_Nov4ReReco_1_1_uV9.root");
//         fill_histo("2010A_Nov4ReReco_2_1_iZj.root");
//         fill_histo("2010A_Nov4ReReco_3_1_EEJ.root");
//         fill_histo("2010A_Nov4ReReco_4_1_qQs.root");
//         fill_histo("2010A_Nov4ReReco_5_1_Lo6.root");
//         fill_histo("2010A_Nov4ReReco_6_1_NQ5.root");
//         fill_histo("2010A_Nov4ReReco_7_1_lba.root");
//         fill_histo("2010A_Nov4ReReco_8_1_QJ4.root");
//         fill_histo("2010A_Nov4ReReco_9_1_gCe.root");
//         fill_histo("2010A_Nov4ReReco_10_1_GL3.root");
//         fill_histo("2010A_Nov4ReReco_11_1_Krt.root");
//         fill_histo("2010A_Nov4ReReco_12_1_0qx.root");
//         fill_histo("2010A_Nov4ReReco_13_1_JuD.root");
//         fill_histo("2010A_Nov4ReReco_14_1_0QZ.root");
//         fill_histo("2010A_Nov4ReReco_15_1_2rq.root");
//         fill_histo("2010A_Nov4ReReco_16_1_2Jp.root");
//         fill_histo("2010A_Nov4ReReco_17_1_6KV.root");
//         fill_histo("2010A_Nov4ReReco_18_1_VQ5.root");
//         fill_histo("2010A_Nov4ReReco_19_1_BW0.root");
//         fill_histo("2010A_Nov4ReReco_20_1_iEt.root");
//         fill_histo("2010A_Nov4ReReco_21_1_zqR.root");
//         fill_histo("2010A_Nov4ReReco_22_1_1yk.root");
//         fill_histo("2010A_Nov4ReReco_23_1_Bav.root");
//         fill_histo("2010A_Nov4ReReco_24_1_Uai.root");
//         fill_histo("2010A_Nov4ReReco_25_1_szV.root");
//         fill_histo("2010A_Nov4ReReco_26_1_OZD.root");
//         fill_histo("2010A_Nov4ReReco_27_1_hN1.root");
//         fill_histo("2010A_Nov4ReReco_28_1_Gsy.root");
//         fill_histo("2010A_Nov4ReReco_29_1_com.root");


	// Selected Range - (latestgoodrange)
        fill_histo("2010A_Nov4ReReco_30_1_zqq.root");
        fill_histo("2010A_Nov4ReReco_31_1_lNE.root");
        fill_histo("2010A_Nov4ReReco_32_1_kR9.root");
        fill_histo("2010A_Nov4ReReco_33_1_nVN.root");
        fill_histo("2010A_Nov4ReReco_34_1_18i.root");
        fill_histo("2010A_Nov4ReReco_35_1_ZOw.root");
//         fill_histo("2010A_Nov4ReReco_36_2_w5E.root");
//         fill_histo("2010A_Nov4ReReco_37_1_ie5.root");
//         fill_histo("2010A_Nov4ReReco_38_1_AGU.root");
//         fill_histo("2010A_Nov4ReReco_39_1_NRR.root");
//         fill_histo("2010A_Nov4ReReco_40_1_y6f.root");
//         fill_histo("2010A_Nov4ReReco_41_1_Dym.root");
//         fill_histo("2010A_Nov4ReReco_42_1_3EW.root");
//         fill_histo("2010A_Nov4ReReco_43_1_Gpx.root");
//         fill_histo("2010A_Nov4ReReco_44_1_yzr.root");
//         fill_histo("2010A_Nov4ReReco_45_1_Sww.root");
//         fill_histo("2010A_Nov4ReReco_46_1_6Rk.root");
//         fill_histo("2010A_Nov4ReReco_47_1_5zC.root");
//         fill_histo("2010A_Nov4ReReco_48_1_WhA.root");
//         fill_histo("2010A_Nov4ReReco_49_1_jsj.root");
//         fill_histo("2010A_Nov4ReReco_50_1_oMU.root");
//         fill_histo("2010A_Nov4ReReco_51_1_aOo.root");
//         fill_histo("2010A_Nov4ReReco_52_1_BRv.root");
//         fill_histo("2010A_Nov4ReReco_53_1_cBx.root");
//         fill_histo("2010A_Nov4ReReco_54_1_BnH.root");
//         fill_histo("2010A_Nov4ReReco_55_1_3Rz.root");
//         fill_histo("2010A_Nov4ReReco_56_1_ptQ.root");
//         fill_histo("2010A_Nov4ReReco_57_1_XA1.root");
//         fill_histo("2010A_Nov4ReReco_58_1_k4C.root");
//         fill_histo("2010A_Nov4ReReco_59_1_Vv9.root");
//         fill_histo("2010A_Nov4ReReco_60_1_gYL.root");
//         fill_histo("2010A_Nov4ReReco_61_1_vJZ.root");
//         fill_histo("2010A_Nov4ReReco_62_1_9lL.root");
//         fill_histo("2010A_Nov4ReReco_63_1_sMB.root");
//         fill_histo("2010A_Nov4ReReco_64_1_wx6.root");
//         fill_histo("2010A_Nov4ReReco_65_1_1V4.root");
//         fill_histo("2010A_Nov4ReReco_66_1_dpY.root");
//         fill_histo("2010A_Nov4ReReco_67_1_vx5.root");
//         fill_histo("2010A_Nov4ReReco_68_1_B0T.root");
//         fill_histo("2010A_Nov4ReReco_69_1_11R.root");
//         fill_histo("2010A_Nov4ReReco_70_1_OP9.root");
//         fill_histo("2010A_Nov4ReReco_71_1_6ZI.root");
//         fill_histo("2010A_Nov4ReReco_72_1_gzM.root");
//         fill_histo("2010A_Nov4ReReco_73_1_z0m.root");
//         fill_histo("2010A_Nov4ReReco_74_1_Lc5.root");
//         fill_histo("2010A_Nov4ReReco_75_1_Qby.root");
//         fill_histo("2010A_Nov4ReReco_76_1_xub.root");
//         fill_histo("2010A_Nov4ReReco_77_1_4mR.root");
//         fill_histo("2010A_Nov4ReReco_78_1_n53.root");
//         fill_histo("2010A_Nov4ReReco_79_1_VDL.root");
//         fill_histo("2010A_Nov4ReReco_80_1_J6v.root");
//         fill_histo("2010A_Nov4ReReco_81_1_voN.root");
//         fill_histo("2010A_Nov4ReReco_82_1_7jR.root");
//         fill_histo("2010A_Nov4ReReco_83_1_c80.root");
//         fill_histo("2010A_Nov4ReReco_84_1_hBg.root");
//         fill_histo("2010A_Nov4ReReco_85_1_Lfa.root");
//         fill_histo("2010A_Nov4ReReco_86_1_TTp.root");
//         fill_histo("2010A_Nov4ReReco_87_1_3Wi.root");
//         fill_histo("2010A_Nov4ReReco_88_1_sGz.root");
//         fill_histo("2010A_Nov4ReReco_89_1_I4V.root");
//         fill_histo("2010A_Nov4ReReco_90_1_ABX.root");
//         fill_histo("2010A_Nov4ReReco_91_1_1sj.root");
//         fill_histo("2010A_Nov4ReReco_92_1_x7O.root");
//         fill_histo("2010A_Nov4ReReco_93_1_lNu.root");
//         fill_histo("2010A_Nov4ReReco_94_1_wdZ.root");
//         fill_histo("2010A_Nov4ReReco_95_1_YkE.root");
//         fill_histo("2010A_Nov4ReReco_96_1_E9B.root");
//         fill_histo("2010A_Nov4ReReco_97_1_RMV.root");
//         fill_histo("2010A_Nov4ReReco_98_1_hPR.root");
//         fill_histo("2010A_Nov4ReReco_99_1_3vA.root");
//         fill_histo("2010A_Nov4ReReco_100_1_mT5.root");
//         fill_histo("2010A_Nov4ReReco_101_1_KMz.root");
//         fill_histo("2010A_Nov4ReReco_102_1_EJz.root");
//         fill_histo("2010A_Nov4ReReco_103_1_HGl.root");
//         fill_histo("2010A_Nov4ReReco_104_1_tzP.root");
//         fill_histo("2010A_Nov4ReReco_105_1_zPE.root");
  
      } else if (fullrun==1) {
        //669 file atm
        // Commsisioning 10 - Run 135537 - CMSSW_3_6_1_patch4
// 	filedir = "/data/jkarancs/data/gridout/CMSSW_3_6_1_patch4/MinimumBias_Commissioning10-Jun14thReReco_v1_RECO/v1815/";
// 	fill_histo("Comm10_361p4_v1815_256_1_Heq.root");
// 	fill_histo("Comm10_361p4_v1815_257_1_Wxb.root");

	filedir = "/data/jkarancs/data/gridout/CMSSW_3_6_1_patch4/MinimumBias_Commissioning10-Jun14thReReco_v1_RECO/v1815/masked/";
        fill_histo("Comm10_361p4_v1815_1_1_hU0.root");
        fill_histo("Comm10_361p4_v1815_2_1_H3y.root");
        fill_histo("Comm10_361p4_v1815_3_1_2BC.root");
        fill_histo("Comm10_361p4_v1815_4_1_nVM.root");
        fill_histo("Comm10_361p4_v1815_5_1_1Fg.root");
        fill_histo("Comm10_361p4_v1815_6_1_kZS.root");
        fill_histo("Comm10_361p4_v1815_7_1_fs2.root");
        fill_histo("Comm10_361p4_v1815_8_1_7Ny.root");
        fill_histo("Comm10_361p4_v1815_9_1_8oA.root");
        fill_histo("Comm10_361p4_v1815_10_1_x20.root");
        fill_histo("Comm10_361p4_v1815_12_1_IfI.root");
        fill_histo("Comm10_361p4_v1815_13_1_o9i.root");
        fill_histo("Comm10_361p4_v1815_14_1_rtm.root");
        fill_histo("Comm10_361p4_v1815_15_1_y3V.root");
        fill_histo("Comm10_361p4_v1815_16_1_79A.root");
        fill_histo("Comm10_361p4_v1815_17_1_Q3D.root");
        fill_histo("Comm10_361p4_v1815_18_1_5tO.root");
        fill_histo("Comm10_361p4_v1815_19_1_S04.root");
        fill_histo("Comm10_361p4_v1815_20_1_vNI.root");
        fill_histo("Comm10_361p4_v1815_21_1_QfJ.root");
        fill_histo("Comm10_361p4_v1815_22_1_aLI.root");
        fill_histo("Comm10_361p4_v1815_23_1_HDe.root");
        fill_histo("Comm10_361p4_v1815_24_1_QPS.root");
        fill_histo("Comm10_361p4_v1815_25_1_LPM.root");
        fill_histo("Comm10_361p4_v1815_26_1_Uyw.root");
        fill_histo("Comm10_361p4_v1815_27_1_hDh.root");
        fill_histo("Comm10_361p4_v1815_28_1_zB5.root");
        fill_histo("Comm10_361p4_v1815_29_1_n0S.root");
        fill_histo("Comm10_361p4_v1815_30_1_HMv.root");
        fill_histo("Comm10_361p4_v1815_31_1_80E.root");
        fill_histo("Comm10_361p4_v1815_32_1_xaZ.root");
        fill_histo("Comm10_361p4_v1815_33_1_brE.root");
        fill_histo("Comm10_361p4_v1815_34_1_bhB.root");
        fill_histo("Comm10_361p4_v1815_35_1_T7i.root");
        fill_histo("Comm10_361p4_v1815_36_1_1EZ.root");
        fill_histo("Comm10_361p4_v1815_37_1_UDi.root");
        fill_histo("Comm10_361p4_v1815_38_1_fSV.root");
        fill_histo("Comm10_361p4_v1815_39_1_ldn.root");
        fill_histo("Comm10_361p4_v1815_40_1_syc.root");
        fill_histo("Comm10_361p4_v1815_41_1_zl1.root");
        fill_histo("Comm10_361p4_v1815_42_1_awF.root");
        fill_histo("Comm10_361p4_v1815_43_1_l8w.root");
        fill_histo("Comm10_361p4_v1815_44_1_SNk.root");
        fill_histo("Comm10_361p4_v1815_45_1_6Au.root");
        fill_histo("Comm10_361p4_v1815_46_1_UB6.root");
        fill_histo("Comm10_361p4_v1815_47_2_7FY.root");
        fill_histo("Comm10_361p4_v1815_48_1_Buy.root");
        fill_histo("Comm10_361p4_v1815_49_1_1kL.root");
        fill_histo("Comm10_361p4_v1815_50_1_36K.root");
        fill_histo("Comm10_361p4_v1815_51_1_2bR.root");
        fill_histo("Comm10_361p4_v1815_52_1_ipI.root");
        fill_histo("Comm10_361p4_v1815_53_1_oMA.root");
        fill_histo("Comm10_361p4_v1815_54_1_CO5.root");
        fill_histo("Comm10_361p4_v1815_55_1_547.root");
        fill_histo("Comm10_361p4_v1815_56_1_cLs.root");
        fill_histo("Comm10_361p4_v1815_57_1_JA0.root");
        fill_histo("Comm10_361p4_v1815_58_1_ylb.root");
        fill_histo("Comm10_361p4_v1815_60_1_kqA.root");
        fill_histo("Comm10_361p4_v1815_61_1_IH8.root");
        fill_histo("Comm10_361p4_v1815_62_1_WW0.root");
        fill_histo("Comm10_361p4_v1815_63_1_u9V.root");
        fill_histo("Comm10_361p4_v1815_64_1_zmd.root");
        fill_histo("Comm10_361p4_v1815_66_1_hvK.root");
        fill_histo("Comm10_361p4_v1815_67_1_QnV.root");
        fill_histo("Comm10_361p4_v1815_68_1_lno.root");
        fill_histo("Comm10_361p4_v1815_69_1_0xd.root");
        fill_histo("Comm10_361p4_v1815_59_1_mSc.root");
        fill_histo("Comm10_361p4_v1815_70_1_7oh.root");
        fill_histo("Comm10_361p4_v1815_71_1_4rR.root");
        fill_histo("Comm10_361p4_v1815_72_1_jOY.root");
        fill_histo("Comm10_361p4_v1815_73_1_DEb.root");
        fill_histo("Comm10_361p4_v1815_74_1_deG.root");
        fill_histo("Comm10_361p4_v1815_75_1_Cgq.root");
        fill_histo("Comm10_361p4_v1815_76_1_Nbd.root");
        fill_histo("Comm10_361p4_v1815_77_1_WfX.root");
        fill_histo("Comm10_361p4_v1815_78_1_UcS.root");
        fill_histo("Comm10_361p4_v1815_79_1_9bb.root");
        fill_histo("Comm10_361p4_v1815_80_1_uzw.root");
        fill_histo("Comm10_361p4_v1815_81_1_TFb.root");
        fill_histo("Comm10_361p4_v1815_82_1_Uyb.root");
        fill_histo("Comm10_361p4_v1815_83_1_m9g.root");
        fill_histo("Comm10_361p4_v1815_84_1_kmw.root");
        fill_histo("Comm10_361p4_v1815_85_1_pTZ.root");
        fill_histo("Comm10_361p4_v1815_86_1_OaU.root");
        fill_histo("Comm10_361p4_v1815_87_1_xu5.root");
        fill_histo("Comm10_361p4_v1815_88_1_JG1.root");
        fill_histo("Comm10_361p4_v1815_89_1_7XX.root");
        fill_histo("Comm10_361p4_v1815_90_1_DJI.root");
        fill_histo("Comm10_361p4_v1815_91_1_U8A.root");
        fill_histo("Comm10_361p4_v1815_92_1_sCB.root");
        fill_histo("Comm10_361p4_v1815_93_1_RfI.root");
        fill_histo("Comm10_361p4_v1815_94_1_ReY.root");
        fill_histo("Comm10_361p4_v1815_95_1_NOs.root");
        fill_histo("Comm10_361p4_v1815_96_1_KlG.root");
      
	filedir = "/home/jkarancs/data/gridout/CMSSW_3_8_6/MinimumBias_Run2010A-Nov4ReReco_v1_RECO/v1815/";
        fill_histo("2010A_Nov4ReReco_1_2_cTq.root");
        fill_histo("2010A_Nov4ReReco_2_2_TFk.root");
        fill_histo("2010A_Nov4ReReco_3_2_JJY.root");
        fill_histo("2010A_Nov4ReReco_4_2_OYw.root");
        fill_histo("2010A_Nov4ReReco_5_1_GRh.root");
        fill_histo("2010A_Nov4ReReco_6_2_pd5.root");
        fill_histo("2010A_Nov4ReReco_7_2_6oo.root");
        fill_histo("2010A_Nov4ReReco_8_2_50s.root");
        fill_histo("2010A_Nov4ReReco_9_2_Nw2.root");
        fill_histo("2010A_Nov4ReReco_10_2_Ch7.root");
        fill_histo("2010A_Nov4ReReco_11_2_pHj.root");
        fill_histo("2010A_Nov4ReReco_12_4_uGf.root");
        fill_histo("2010A_Nov4ReReco_13_3_kG6.root");
        fill_histo("2010A_Nov4ReReco_14_2_Abc.root");
        fill_histo("2010A_Nov4ReReco_15_2_tJW.root");
        fill_histo("2010A_Nov4ReReco_16_2_4DZ.root");
        fill_histo("2010A_Nov4ReReco_17_2_KZW.root");
        fill_histo("2010A_Nov4ReReco_18_4_UUq.root");
        fill_histo("2010A_Nov4ReReco_19_2_A8M.root");
        fill_histo("2010A_Nov4ReReco_20_3_yzB.root");
        fill_histo("2010A_Nov4ReReco_21_2_35t.root");
        fill_histo("2010A_Nov4ReReco_22_2_QiO.root");
        fill_histo("2010A_Nov4ReReco_23_3_3Aj.root");
        fill_histo("2010A_Nov4ReReco_24_2_qdU.root");
        fill_histo("2010A_Nov4ReReco_25_3_VZu.root");
        fill_histo("2010A_Nov4ReReco_26_2_nG9.root");
        fill_histo("2010A_Nov4ReReco_27_2_fSX.root");
        fill_histo("2010A_Nov4ReReco_28_1_xvL.root");
        fill_histo("2010A_Nov4ReReco_29_3_SzQ.root");
        fill_histo("2010A_Nov4ReReco_30_2_4TK.root");
        fill_histo("2010A_Nov4ReReco_31_2_2Xp.root");
        fill_histo("2010A_Nov4ReReco_32_2_zkZ.root");
        fill_histo("2010A_Nov4ReReco_33_2_Ypk.root");
        fill_histo("2010A_Nov4ReReco_34_2_VmX.root");
        fill_histo("2010A_Nov4ReReco_35_3_U3g.root");
        fill_histo("2010A_Nov4ReReco_36_2_gQp.root");
        fill_histo("2010A_Nov4ReReco_37_2_BTK.root");
        fill_histo("2010A_Nov4ReReco_38_2_knE.root");
        fill_histo("2010A_Nov4ReReco_39_2_FNG.root");
        fill_histo("2010A_Nov4ReReco_40_2_pWo.root");
        fill_histo("2010A_Nov4ReReco_41_2_SSm.root");
        fill_histo("2010A_Nov4ReReco_42_2_E7x.root");
        fill_histo("2010A_Nov4ReReco_43_2_jDu.root");
        fill_histo("2010A_Nov4ReReco_44_2_jV2.root");
        fill_histo("2010A_Nov4ReReco_45_3_PRW.root");
        fill_histo("2010A_Nov4ReReco_46_2_ZTK.root");
        fill_histo("2010A_Nov4ReReco_47_2_Bad.root");
        fill_histo("2010A_Nov4ReReco_48_2_b2p.root");
        fill_histo("2010A_Nov4ReReco_49_1_1aU.root");
        fill_histo("2010A_Nov4ReReco_50_3_NAx.root");
        fill_histo("2010A_Nov4ReReco_51_2_uLI.root");
        fill_histo("2010A_Nov4ReReco_52_2_6H8.root");
        fill_histo("2010A_Nov4ReReco_53_2_k2X.root");
        fill_histo("2010A_Nov4ReReco_54_2_4IL.root");
        fill_histo("2010A_Nov4ReReco_55_4_9O3.root");
        fill_histo("2010A_Nov4ReReco_56_2_XlW.root");
        fill_histo("2010A_Nov4ReReco_57_2_QRl.root");
        fill_histo("2010A_Nov4ReReco_58_3_mCB.root");
        fill_histo("2010A_Nov4ReReco_59_2_oyi.root");
        fill_histo("2010A_Nov4ReReco_60_2_HLk.root");
        fill_histo("2010A_Nov4ReReco_61_3_AAY.root");
        fill_histo("2010A_Nov4ReReco_62_1_XJs.root");
        fill_histo("2010A_Nov4ReReco_63_2_aGX.root");
        fill_histo("2010A_Nov4ReReco_64_3_Xpv.root");
        fill_histo("2010A_Nov4ReReco_65_2_dWv.root");
        fill_histo("2010A_Nov4ReReco_66_2_xHR.root");
        fill_histo("2010A_Nov4ReReco_67_1_Kvd.root");
        fill_histo("2010A_Nov4ReReco_68_2_tBi.root");
        fill_histo("2010A_Nov4ReReco_69_2_ldj.root");
        fill_histo("2010A_Nov4ReReco_70_2_Y2Y.root");
        fill_histo("2010A_Nov4ReReco_71_2_S7S.root");
        fill_histo("2010A_Nov4ReReco_72_3_2V8.root");
        fill_histo("2010A_Nov4ReReco_73_2_9QV.root");
        fill_histo("2010A_Nov4ReReco_74_3_K1H.root");
        fill_histo("2010A_Nov4ReReco_75_2_zxb.root");
        fill_histo("2010A_Nov4ReReco_76_2_2XC.root");
        fill_histo("2010A_Nov4ReReco_77_1_mly.root");
        fill_histo("2010A_Nov4ReReco_78_2_rSI.root");
        fill_histo("2010A_Nov4ReReco_79_1_WNl.root");
        fill_histo("2010A_Nov4ReReco_80_2_Umf.root");
        fill_histo("2010A_Nov4ReReco_81_2_jBw.root");
        fill_histo("2010A_Nov4ReReco_82_2_JPA.root");
        fill_histo("2010A_Nov4ReReco_83_2_TQf.root");
        fill_histo("2010A_Nov4ReReco_84_2_nxD.root");
        fill_histo("2010A_Nov4ReReco_85_2_BDc.root");
        fill_histo("2010A_Nov4ReReco_86_2_6sN.root");
        fill_histo("2010A_Nov4ReReco_87_2_lcY.root");
        fill_histo("2010A_Nov4ReReco_88_2_lO7.root");
        fill_histo("2010A_Nov4ReReco_89_3_6aE.root");
        fill_histo("2010A_Nov4ReReco_90_2_QRA.root");
        fill_histo("2010A_Nov4ReReco_91_2_JA8.root");
        fill_histo("2010A_Nov4ReReco_92_2_Eow.root");
        fill_histo("2010A_Nov4ReReco_93_2_2j3.root");
        fill_histo("2010A_Nov4ReReco_94_3_4bJ.root");
        fill_histo("2010A_Nov4ReReco_95_2_XCs.root");
        fill_histo("2010A_Nov4ReReco_96_1_Q9c.root");
        fill_histo("2010A_Nov4ReReco_97_2_BC3.root");
        fill_histo("2010A_Nov4ReReco_98_2_hqR.root");
        fill_histo("2010A_Nov4ReReco_99_2_LXl.root");
        fill_histo("2010A_Nov4ReReco_100_2_vwI.root");
        fill_histo("2010A_Nov4ReReco_101_3_CRl.root");
        fill_histo("2010A_Nov4ReReco_102_2_HVg.root");
        fill_histo("2010A_Nov4ReReco_103_2_hmf.root");
        fill_histo("2010A_Nov4ReReco_104_2_O8M.root");
        fill_histo("2010A_Nov4ReReco_105_1_HTb.root");
        fill_histo("2010A_Nov4ReReco_106_1_DLB.root");
        fill_histo("2010A_Nov4ReReco_107_1_3Q6.root");
        fill_histo("2010A_Nov4ReReco_108_1_pFK.root");
        fill_histo("2010A_Nov4ReReco_109_2_nwk.root");
        fill_histo("2010A_Nov4ReReco_110_2_mW4.root");
        fill_histo("2010A_Nov4ReReco_111_2_hcg.root");
        fill_histo("2010A_Nov4ReReco_112_2_X8T.root");
        fill_histo("2010A_Nov4ReReco_113_2_99L.root");
        fill_histo("2010A_Nov4ReReco_114_2_3o2.root");
        fill_histo("2010A_Nov4ReReco_115_2_Qku.root");
        fill_histo("2010A_Nov4ReReco_116_2_kty.root");
        fill_histo("2010A_Nov4ReReco_117_2_pJy.root");
        fill_histo("2010A_Nov4ReReco_118_3_1XK.root");
        fill_histo("2010A_Nov4ReReco_119_2_bFM.root");
        fill_histo("2010A_Nov4ReReco_120_2_BNF.root");
        fill_histo("2010A_Nov4ReReco_121_2_mak.root");
        fill_histo("2010A_Nov4ReReco_122_2_91m.root");
        fill_histo("2010A_Nov4ReReco_123_2_pOV.root");
        fill_histo("2010A_Nov4ReReco_124_2_gbd.root");
        fill_histo("2010A_Nov4ReReco_125_2_lpJ.root");
        fill_histo("2010A_Nov4ReReco_126_2_V9p.root");
        fill_histo("2010A_Nov4ReReco_127_2_XLW.root");
        fill_histo("2010A_Nov4ReReco_128_2_QJx.root");
        fill_histo("2010A_Nov4ReReco_129_2_1H0.root");
        fill_histo("2010A_Nov4ReReco_130_1_dyq.root");
        fill_histo("2010A_Nov4ReReco_131_2_zss.root");
        fill_histo("2010A_Nov4ReReco_132_2_qjX.root");
        fill_histo("2010A_Nov4ReReco_133_2_QE1.root");
        fill_histo("2010A_Nov4ReReco_134_2_KWn.root");
        fill_histo("2010A_Nov4ReReco_135_3_iei.root");
        fill_histo("2010A_Nov4ReReco_136_2_hE4.root");
        fill_histo("2010A_Nov4ReReco_137_2_JuC.root");
        fill_histo("2010A_Nov4ReReco_138_2_n29.root");
        fill_histo("2010A_Nov4ReReco_139_1_bpP.root");
        fill_histo("2010A_Nov4ReReco_140_1_bN5.root");
        fill_histo("2010A_Nov4ReReco_141_2_URy.root");
        fill_histo("2010A_Nov4ReReco_142_1_xec.root");
        fill_histo("2010A_Nov4ReReco_143_2_lTv.root");
        fill_histo("2010A_Nov4ReReco_144_2_KCT.root");
        fill_histo("2010A_Nov4ReReco_145_2_9Ah.root");
        fill_histo("2010A_Nov4ReReco_146_2_Kqy.root");
        fill_histo("2010A_Nov4ReReco_147_2_4Rg.root");
        fill_histo("2010A_Nov4ReReco_148_2_BlB.root");
        fill_histo("2010A_Nov4ReReco_149_2_lWA.root");
        fill_histo("2010A_Nov4ReReco_150_3_T4J.root");
        fill_histo("2010A_Nov4ReReco_151_2_v0B.root");
        fill_histo("2010A_Nov4ReReco_152_2_shq.root");
        fill_histo("2010A_Nov4ReReco_153_2_iSW.root");
        fill_histo("2010A_Nov4ReReco_154_1_eIl.root");
        fill_histo("2010A_Nov4ReReco_155_1_EIP.root");
        fill_histo("2010A_Nov4ReReco_156_2_MhN.root");
        fill_histo("2010A_Nov4ReReco_157_1_74h.root");
        fill_histo("2010A_Nov4ReReco_158_2_4h7.root");
        fill_histo("2010A_Nov4ReReco_159_2_2Ky.root");
        fill_histo("2010A_Nov4ReReco_160_2_2z3.root");
        fill_histo("2010A_Nov4ReReco_161_2_XoZ.root");
        fill_histo("2010A_Nov4ReReco_162_2_xlO.root");
        fill_histo("2010A_Nov4ReReco_163_2_lVH.root");
        fill_histo("2010A_Nov4ReReco_164_3_zrg.root");
        fill_histo("2010A_Nov4ReReco_165_2_7t9.root");
        fill_histo("2010A_Nov4ReReco_166_2_1OW.root");
        fill_histo("2010A_Nov4ReReco_167_3_lKM.root");
        fill_histo("2010A_Nov4ReReco_168_2_6Kt.root");
        fill_histo("2010A_Nov4ReReco_169_3_CZL.root");
        fill_histo("2010A_Nov4ReReco_170_2_lkn.root");
        fill_histo("2010A_Nov4ReReco_171_2_sD1.root");
        fill_histo("2010A_Nov4ReReco_172_2_uQ8.root");
        fill_histo("2010A_Nov4ReReco_173_2_TxM.root");
        fill_histo("2010A_Nov4ReReco_174_2_EUU.root");
        fill_histo("2010A_Nov4ReReco_175_2_IWS.root");
        fill_histo("2010A_Nov4ReReco_176_2_9yo.root");
        fill_histo("2010A_Nov4ReReco_177_2_WcH.root");
        fill_histo("2010A_Nov4ReReco_178_2_ewp.root");
        fill_histo("2010A_Nov4ReReco_179_3_a2R.root");
        fill_histo("2010A_Nov4ReReco_180_2_5i3.root");
        fill_histo("2010A_Nov4ReReco_181_2_e63.root");
        fill_histo("2010A_Nov4ReReco_182_4_N8o.root");
        fill_histo("2010A_Nov4ReReco_183_2_G7a.root");
        fill_histo("2010A_Nov4ReReco_184_3_7dD.root");
        fill_histo("2010A_Nov4ReReco_185_2_u56.root");
        fill_histo("2010A_Nov4ReReco_186_2_21c.root");
        fill_histo("2010A_Nov4ReReco_187_2_ARV.root");
        fill_histo("2010A_Nov4ReReco_188_2_iWh.root");
        fill_histo("2010A_Nov4ReReco_189_2_Cfs.root");
        fill_histo("2010A_Nov4ReReco_190_2_8ZE.root");
        fill_histo("2010A_Nov4ReReco_191_2_nJr.root");
        fill_histo("2010A_Nov4ReReco_192_2_npf.root");
        fill_histo("2010A_Nov4ReReco_193_3_Y1b.root");
        fill_histo("2010A_Nov4ReReco_194_2_mF4.root");
        fill_histo("2010A_Nov4ReReco_195_2_mvE.root");
        fill_histo("2010A_Nov4ReReco_196_2_YCr.root");
        fill_histo("2010A_Nov4ReReco_197_2_byM.root");
        fill_histo("2010A_Nov4ReReco_198_2_Rf0.root");
        fill_histo("2010A_Nov4ReReco_199_2_CjI.root");
        fill_histo("2010A_Nov4ReReco_200_2_hqk.root");
        fill_histo("2010A_Nov4ReReco_201_1_H5R.root");
        fill_histo("2010A_Nov4ReReco_202_1_EMK.root");
        fill_histo("2010A_Nov4ReReco_203_1_Wj4.root");
        fill_histo("2010A_Nov4ReReco_204_1_T48.root");
        fill_histo("2010A_Nov4ReReco_205_1_0az.root");
        fill_histo("2010A_Nov4ReReco_206_1_INy.root");
        fill_histo("2010A_Nov4ReReco_207_1_WKw.root");
        fill_histo("2010A_Nov4ReReco_208_1_mda.root");
        fill_histo("2010A_Nov4ReReco_209_1_hxy.root");
        fill_histo("2010A_Nov4ReReco_210_1_V4k.root");
        fill_histo("2010A_Nov4ReReco_211_1_nKu.root");
        fill_histo("2010A_Nov4ReReco_212_1_qL7.root");
        fill_histo("2010A_Nov4ReReco_213_1_KPl.root");
        fill_histo("2010A_Nov4ReReco_214_1_KBP.root");
        fill_histo("2010A_Nov4ReReco_215_1_EBp.root");
        fill_histo("2010A_Nov4ReReco_216_1_IOg.root");
        fill_histo("2010A_Nov4ReReco_217_1_JG2.root");
        fill_histo("2010A_Nov4ReReco_218_1_ehf.root");
        fill_histo("2010A_Nov4ReReco_219_1_Fe3.root");
        fill_histo("2010A_Nov4ReReco_220_1_lMs.root");
        fill_histo("2010A_Nov4ReReco_221_1_zA9.root");
        fill_histo("2010A_Nov4ReReco_222_3_GMh.root");
        fill_histo("2010A_Nov4ReReco_223_1_ORX.root");
        fill_histo("2010A_Nov4ReReco_224_1_3xj.root");
        fill_histo("2010A_Nov4ReReco_225_1_VmK.root");
        fill_histo("2010A_Nov4ReReco_226_1_WBa.root");
        fill_histo("2010A_Nov4ReReco_227_1_E6g.root");
        fill_histo("2010A_Nov4ReReco_228_1_zGj.root");
        fill_histo("2010A_Nov4ReReco_229_1_LZG.root");
        fill_histo("2010A_Nov4ReReco_230_1_ZRE.root");
        fill_histo("2010A_Nov4ReReco_231_1_PNz.root");
        fill_histo("2010A_Nov4ReReco_232_1_2bO.root");
        fill_histo("2010A_Nov4ReReco_233_1_4O9.root");
        fill_histo("2010A_Nov4ReReco_234_1_drh.root");
        fill_histo("2010A_Nov4ReReco_235_1_5RO.root");
        fill_histo("2010A_Nov4ReReco_236_1_Acb.root");
        fill_histo("2010A_Nov4ReReco_237_1_ebw.root");
        fill_histo("2010A_Nov4ReReco_238_1_XqA.root");
        fill_histo("2010A_Nov4ReReco_239_1_ubV.root");
        fill_histo("2010A_Nov4ReReco_240_1_el4.root");
        fill_histo("2010A_Nov4ReReco_241_1_VFC.root");
        fill_histo("2010A_Nov4ReReco_242_1_rnp.root");
        fill_histo("2010A_Nov4ReReco_243_1_clA.root");
        fill_histo("2010A_Nov4ReReco_244_1_hsb.root");
        fill_histo("2010A_Nov4ReReco_245_1_sX4.root");
        fill_histo("2010A_Nov4ReReco_246_1_lAv.root");
        fill_histo("2010A_Nov4ReReco_247_1_M2h.root");
        fill_histo("2010A_Nov4ReReco_248_1_7Ln.root");
        fill_histo("2010A_Nov4ReReco_249_1_CWL.root");
        fill_histo("2010A_Nov4ReReco_250_1_Duy.root");
        fill_histo("2010A_Nov4ReReco_251_1_7Oi.root");
        fill_histo("2010A_Nov4ReReco_252_1_22g.root");
        fill_histo("2010A_Nov4ReReco_253_1_w5G.root");
        fill_histo("2010A_Nov4ReReco_254_1_sQK.root");
        fill_histo("2010A_Nov4ReReco_255_1_69Z.root");
        fill_histo("2010A_Nov4ReReco_256_1_xpK.root");
        fill_histo("2010A_Nov4ReReco_257_1_Ymm.root");
        fill_histo("2010A_Nov4ReReco_258_1_DXZ.root");
        fill_histo("2010A_Nov4ReReco_259_1_Q3g.root");
        fill_histo("2010A_Nov4ReReco_260_1_PcZ.root");
        fill_histo("2010A_Nov4ReReco_261_1_d9U.root");
        fill_histo("2010A_Nov4ReReco_262_1_9qj.root");
        fill_histo("2010A_Nov4ReReco_263_1_bXP.root");
        fill_histo("2010A_Nov4ReReco_264_3_VHz.root");
        fill_histo("2010A_Nov4ReReco_265_1_wCD.root");
        fill_histo("2010A_Nov4ReReco_266_1_nw3.root");
        fill_histo("2010A_Nov4ReReco_267_1_tOD.root");
        fill_histo("2010A_Nov4ReReco_268_2_kkk.root");
        fill_histo("2010A_Nov4ReReco_269_1_CDW.root");
        fill_histo("2010A_Nov4ReReco_270_1_Iwx.root");
        fill_histo("2010A_Nov4ReReco_271_1_xdC.root");
        fill_histo("2010A_Nov4ReReco_272_1_BoP.root");
        fill_histo("2010A_Nov4ReReco_273_1_z8S.root");
        fill_histo("2010A_Nov4ReReco_274_1_uLp.root");
        fill_histo("2010A_Nov4ReReco_275_1_AKg.root");
        fill_histo("2010A_Nov4ReReco_276_1_KqH.root");
        fill_histo("2010A_Nov4ReReco_277_1_x7T.root");
        fill_histo("2010A_Nov4ReReco_278_1_iJy.root");
        fill_histo("2010A_Nov4ReReco_279_2_g39.root");
        fill_histo("2010A_Nov4ReReco_280_1_4mC.root");
        fill_histo("2010A_Nov4ReReco_281_1_fq3.root");
        fill_histo("2010A_Nov4ReReco_282_1_F0Z.root");
        fill_histo("2010A_Nov4ReReco_283_1_YHV.root");
        fill_histo("2010A_Nov4ReReco_284_1_hrU.root");
        fill_histo("2010A_Nov4ReReco_285_1_CC6.root");
        fill_histo("2010A_Nov4ReReco_286_1_sx9.root");
        fill_histo("2010A_Nov4ReReco_287_1_Sox.root");
        fill_histo("2010A_Nov4ReReco_288_1_Sao.root");
        fill_histo("2010A_Nov4ReReco_289_1_Lmm.root");
        fill_histo("2010A_Nov4ReReco_290_1_BA5.root");
        fill_histo("2010A_Nov4ReReco_291_1_nuy.root");
        fill_histo("2010A_Nov4ReReco_292_1_QoF.root");
        fill_histo("2010A_Nov4ReReco_293_1_5MG.root");
        fill_histo("2010A_Nov4ReReco_294_1_KsQ.root");
        fill_histo("2010A_Nov4ReReco_295_1_q52.root");
        fill_histo("2010A_Nov4ReReco_296_1_rsx.root");
        fill_histo("2010A_Nov4ReReco_297_1_VqS.root");
        fill_histo("2010A_Nov4ReReco_298_1_Z8S.root");
        fill_histo("2010A_Nov4ReReco_299_1_1Yv.root");
        fill_histo("2010A_Nov4ReReco_300_1_RgH.root");
        fill_histo("2010A_Nov4ReReco_301_1_NWR.root");
        fill_histo("2010A_Nov4ReReco_302_1_gs8.root");
        fill_histo("2010A_Nov4ReReco_303_2_wVT.root");
        fill_histo("2010A_Nov4ReReco_304_1_hEE.root");
        fill_histo("2010A_Nov4ReReco_305_1_ZUE.root");
        fill_histo("2010A_Nov4ReReco_306_2_aX0.root");
        fill_histo("2010A_Nov4ReReco_307_1_h2u.root");
        fill_histo("2010A_Nov4ReReco_308_1_iek.root");
        fill_histo("2010A_Nov4ReReco_309_1_C2k.root");
        fill_histo("2010A_Nov4ReReco_310_2_bW9.root");
        fill_histo("2010A_Nov4ReReco_311_1_Hcw.root");
        fill_histo("2010A_Nov4ReReco_312_1_ZrI.root");
        fill_histo("2010A_Nov4ReReco_313_1_GHu.root");
        fill_histo("2010A_Nov4ReReco_314_1_FIy.root");
        fill_histo("2010A_Nov4ReReco_315_1_Jhu.root");
        fill_histo("2010A_Nov4ReReco_316_1_Mwj.root");
        fill_histo("2010A_Nov4ReReco_317_1_duH.root");
        fill_histo("2010A_Nov4ReReco_318_1_oOh.root");
        fill_histo("2010A_Nov4ReReco_319_2_jsf.root");
        fill_histo("2010A_Nov4ReReco_320_1_BGX.root");
        fill_histo("2010A_Nov4ReReco_321_1_24K.root");
        fill_histo("2010A_Nov4ReReco_322_1_vvI.root");
        fill_histo("2010A_Nov4ReReco_323_1_ZwG.root");
        fill_histo("2010A_Nov4ReReco_324_1_vxp.root");
        fill_histo("2010A_Nov4ReReco_325_1_ufd.root");
        fill_histo("2010A_Nov4ReReco_326_1_EvK.root");
        fill_histo("2010A_Nov4ReReco_327_1_sJr.root");
        fill_histo("2010A_Nov4ReReco_328_1_JgG.root");
        fill_histo("2010A_Nov4ReReco_329_1_uL7.root");
        fill_histo("2010A_Nov4ReReco_330_1_UbO.root");
        fill_histo("2010A_Nov4ReReco_331_1_3nQ.root");
        fill_histo("2010A_Nov4ReReco_332_1_TBU.root");
        fill_histo("2010A_Nov4ReReco_333_1_HAO.root");
        fill_histo("2010A_Nov4ReReco_334_1_IPv.root");
        fill_histo("2010A_Nov4ReReco_335_1_DPT.root");
        fill_histo("2010A_Nov4ReReco_336_1_5Jk.root");
        fill_histo("2010A_Nov4ReReco_337_1_qyk.root");
        fill_histo("2010A_Nov4ReReco_338_1_p1e.root");
        fill_histo("2010A_Nov4ReReco_339_1_naT.root");
        fill_histo("2010A_Nov4ReReco_340_1_MBM.root");
        fill_histo("2010A_Nov4ReReco_341_1_uHu.root");
        fill_histo("2010A_Nov4ReReco_342_1_PKj.root");
        fill_histo("2010A_Nov4ReReco_343_1_WIt.root");
        fill_histo("2010A_Nov4ReReco_344_1_dky.root");
        fill_histo("2010A_Nov4ReReco_345_1_93W.root");
        fill_histo("2010A_Nov4ReReco_346_1_559.root");
        fill_histo("2010A_Nov4ReReco_347_1_SlO.root");
        fill_histo("2010A_Nov4ReReco_348_1_7Ug.root");
        fill_histo("2010A_Nov4ReReco_349_1_maJ.root");
        fill_histo("2010A_Nov4ReReco_350_1_odN.root");
        fill_histo("2010A_Nov4ReReco_351_1_8Xk.root");
        fill_histo("2010A_Nov4ReReco_352_1_q3n.root");
        fill_histo("2010A_Nov4ReReco_353_1_qjy.root");
        fill_histo("2010A_Nov4ReReco_354_1_VeR.root");
        fill_histo("2010A_Nov4ReReco_355_1_2n0.root");
        fill_histo("2010A_Nov4ReReco_356_1_XCl.root");
        fill_histo("2010A_Nov4ReReco_357_1_3fY.root");
        fill_histo("2010A_Nov4ReReco_358_1_hdp.root");
        fill_histo("2010A_Nov4ReReco_359_1_On8.root");
        fill_histo("2010A_Nov4ReReco_360_1_wUB.root");
        fill_histo("2010A_Nov4ReReco_361_1_Bbt.root");
        fill_histo("2010A_Nov4ReReco_362_1_1Yf.root");
        fill_histo("2010A_Nov4ReReco_363_1_Pr2.root");
        fill_histo("2010A_Nov4ReReco_364_1_fe0.root");
        fill_histo("2010A_Nov4ReReco_365_1_V7H.root");
        fill_histo("2010A_Nov4ReReco_366_1_vC2.root");
        fill_histo("2010A_Nov4ReReco_367_1_82k.root");
        fill_histo("2010A_Nov4ReReco_368_2_GbU.root");
        fill_histo("2010A_Nov4ReReco_369_1_u7L.root");
        fill_histo("2010A_Nov4ReReco_370_1_39t.root");
        fill_histo("2010A_Nov4ReReco_371_1_1PZ.root");
        fill_histo("2010A_Nov4ReReco_372_1_0vk.root");
        fill_histo("2010A_Nov4ReReco_373_1_cmV.root");
        fill_histo("2010A_Nov4ReReco_374_1_ywL.root");
        fill_histo("2010A_Nov4ReReco_375_1_nFc.root");
        fill_histo("2010A_Nov4ReReco_376_1_ilV.root");
        fill_histo("2010A_Nov4ReReco_377_1_YZZ.root");
        fill_histo("2010A_Nov4ReReco_378_1_NWQ.root");
        fill_histo("2010A_Nov4ReReco_379_1_Ozz.root");
        fill_histo("2010A_Nov4ReReco_380_1_5d2.root");
        fill_histo("2010A_Nov4ReReco_381_1_Qds.root");
        fill_histo("2010A_Nov4ReReco_382_1_yMF.root");
        fill_histo("2010A_Nov4ReReco_383_1_hSL.root");
        fill_histo("2010A_Nov4ReReco_384_1_V8A.root");
        fill_histo("2010A_Nov4ReReco_385_1_0gX.root");
        fill_histo("2010A_Nov4ReReco_386_1_mHy.root");
        fill_histo("2010A_Nov4ReReco_387_1_xQP.root");
        fill_histo("2010A_Nov4ReReco_388_1_qjU.root");
        fill_histo("2010A_Nov4ReReco_389_1_zeE.root");
        fill_histo("2010A_Nov4ReReco_390_1_F9k.root");
        fill_histo("2010A_Nov4ReReco_391_1_GQ1.root");
        fill_histo("2010A_Nov4ReReco_392_1_O5F.root");
        fill_histo("2010A_Nov4ReReco_393_1_EtB.root");
        fill_histo("2010A_Nov4ReReco_394_1_4a9.root");
        fill_histo("2010A_Nov4ReReco_395_1_GiP.root");
        fill_histo("2010A_Nov4ReReco_396_1_nmE.root");
        fill_histo("2010A_Nov4ReReco_397_1_EFN.root");
        fill_histo("2010A_Nov4ReReco_398_1_Wim.root");
        fill_histo("2010A_Nov4ReReco_399_1_F2u.root");
        fill_histo("2010A_Nov4ReReco_400_1_ZmF.root");
        fill_histo("2010A_Nov4ReReco_401_1_V7U.root");
        fill_histo("2010A_Nov4ReReco_402_1_eMB.root");
        fill_histo("2010A_Nov4ReReco_403_1_pvN.root");
        fill_histo("2010A_Nov4ReReco_404_1_76V.root");
        fill_histo("2010A_Nov4ReReco_405_1_RBp.root");
        fill_histo("2010A_Nov4ReReco_406_1_lxx.root");
        fill_histo("2010A_Nov4ReReco_407_1_kQm.root");
        fill_histo("2010A_Nov4ReReco_408_1_6j2.root");
        fill_histo("2010A_Nov4ReReco_409_3_Bfs.root");
        fill_histo("2010A_Nov4ReReco_410_1_jM1.root");
        fill_histo("2010A_Nov4ReReco_411_2_2ez.root");
        fill_histo("2010A_Nov4ReReco_412_1_odf.root");
        fill_histo("2010A_Nov4ReReco_413_1_bCn.root");
        fill_histo("2010A_Nov4ReReco_414_1_DcH.root");
        fill_histo("2010A_Nov4ReReco_415_1_NMF.root");
        fill_histo("2010A_Nov4ReReco_416_1_KjO.root");
        fill_histo("2010A_Nov4ReReco_417_1_Cvh.root");
        fill_histo("2010A_Nov4ReReco_418_2_qpa.root");
        fill_histo("2010A_Nov4ReReco_419_1_hay.root");
        fill_histo("2010A_Nov4ReReco_420_1_MY1.root");
        fill_histo("2010A_Nov4ReReco_421_1_vWS.root");
        fill_histo("2010A_Nov4ReReco_422_1_feH.root");
        fill_histo("2010A_Nov4ReReco_423_1_Ezd.root");
        fill_histo("2010A_Nov4ReReco_424_1_jFS.root");
        fill_histo("2010A_Nov4ReReco_425_1_4yM.root");
        fill_histo("2010A_Nov4ReReco_426_1_vVT.root");
        fill_histo("2010A_Nov4ReReco_427_1_0WB.root");
        fill_histo("2010A_Nov4ReReco_428_1_9TE.root");
        fill_histo("2010A_Nov4ReReco_429_1_W6y.root");
        fill_histo("2010A_Nov4ReReco_430_1_YcP.root");
        fill_histo("2010A_Nov4ReReco_431_1_DwQ.root");
        fill_histo("2010A_Nov4ReReco_432_1_tX6.root");
        fill_histo("2010A_Nov4ReReco_433_1_OBe.root");
        fill_histo("2010A_Nov4ReReco_434_1_2ny.root");
        fill_histo("2010A_Nov4ReReco_435_1_nBh.root");
        fill_histo("2010A_Nov4ReReco_436_1_8bp.root");
        fill_histo("2010A_Nov4ReReco_437_1_yxu.root");
        fill_histo("2010A_Nov4ReReco_438_1_Dqb.root");
        fill_histo("2010A_Nov4ReReco_439_1_7J1.root");
        fill_histo("2010A_Nov4ReReco_440_1_MaN.root");
        fill_histo("2010A_Nov4ReReco_441_1_GOM.root");
        fill_histo("2010A_Nov4ReReco_442_1_5gx.root");
        fill_histo("2010A_Nov4ReReco_443_1_T3n.root");
        fill_histo("2010A_Nov4ReReco_444_1_upb.root");
        fill_histo("2010A_Nov4ReReco_445_1_IYT.root");
        fill_histo("2010A_Nov4ReReco_446_1_Vj5.root");
        fill_histo("2010A_Nov4ReReco_447_1_6Or.root");
        fill_histo("2010A_Nov4ReReco_448_1_3ie.root");
        fill_histo("2010A_Nov4ReReco_449_1_iKQ.root");
        fill_histo("2010A_Nov4ReReco_450_1_GoN.root");
        fill_histo("2010A_Nov4ReReco_451_1_Aef.root");
        fill_histo("2010A_Nov4ReReco_452_1_eSi.root");
        fill_histo("2010A_Nov4ReReco_453_1_CqF.root");
        fill_histo("2010A_Nov4ReReco_454_1_zHt.root");
        fill_histo("2010A_Nov4ReReco_455_1_Pew.root");
        fill_histo("2010A_Nov4ReReco_456_1_b8w.root");
        fill_histo("2010A_Nov4ReReco_457_1_4jA.root");
        fill_histo("2010A_Nov4ReReco_458_2_7pK.root");
        fill_histo("2010A_Nov4ReReco_459_1_DcZ.root");
        fill_histo("2010A_Nov4ReReco_460_1_pQh.root");
        fill_histo("2010A_Nov4ReReco_461_1_BPe.root");
        fill_histo("2010A_Nov4ReReco_462_1_osn.root");
        fill_histo("2010A_Nov4ReReco_463_1_rm5.root");
        fill_histo("2010A_Nov4ReReco_464_1_knB.root");
        fill_histo("2010A_Nov4ReReco_465_1_YC5.root");
        fill_histo("2010A_Nov4ReReco_466_1_qSJ.root");
        fill_histo("2010A_Nov4ReReco_467_1_cOc.root");
        fill_histo("2010A_Nov4ReReco_468_1_HfZ.root");
        fill_histo("2010A_Nov4ReReco_469_1_eCl.root");
	
	filedir = "/data/jkarancs/data/gridout/CMSSW_3_8_6/MinimumBias_Run2010B-Nov4ReReco_v1_RECO/v1815/";
        fill_histo("2010A_Nov4ReReco_1_1_uV9.root");
        fill_histo("2010A_Nov4ReReco_2_1_iZj.root");
        fill_histo("2010A_Nov4ReReco_3_1_EEJ.root");
        fill_histo("2010A_Nov4ReReco_4_1_qQs.root");
        fill_histo("2010A_Nov4ReReco_5_1_Lo6.root");
        fill_histo("2010A_Nov4ReReco_6_1_NQ5.root");
        fill_histo("2010A_Nov4ReReco_7_1_lba.root");
        fill_histo("2010A_Nov4ReReco_8_1_QJ4.root");
        fill_histo("2010A_Nov4ReReco_9_1_gCe.root");
        fill_histo("2010A_Nov4ReReco_10_1_GL3.root");
        fill_histo("2010A_Nov4ReReco_11_1_Krt.root");
        fill_histo("2010A_Nov4ReReco_12_1_0qx.root");
        fill_histo("2010A_Nov4ReReco_13_1_JuD.root");
        fill_histo("2010A_Nov4ReReco_14_1_0QZ.root");
        fill_histo("2010A_Nov4ReReco_15_1_2rq.root");
        fill_histo("2010A_Nov4ReReco_16_1_2Jp.root");
        fill_histo("2010A_Nov4ReReco_17_1_6KV.root");
        fill_histo("2010A_Nov4ReReco_18_1_VQ5.root");
        fill_histo("2010A_Nov4ReReco_19_1_BW0.root");
        fill_histo("2010A_Nov4ReReco_20_1_iEt.root");
        fill_histo("2010A_Nov4ReReco_21_1_zqR.root");
        fill_histo("2010A_Nov4ReReco_22_1_1yk.root");
        fill_histo("2010A_Nov4ReReco_23_1_Bav.root");
        fill_histo("2010A_Nov4ReReco_24_1_Uai.root");
        fill_histo("2010A_Nov4ReReco_25_1_szV.root");
        fill_histo("2010A_Nov4ReReco_26_1_OZD.root");
        fill_histo("2010A_Nov4ReReco_27_1_hN1.root");
        fill_histo("2010A_Nov4ReReco_28_1_Gsy.root");
        fill_histo("2010A_Nov4ReReco_29_1_com.root");
        fill_histo("2010A_Nov4ReReco_30_1_zqq.root");
        fill_histo("2010A_Nov4ReReco_31_1_lNE.root");
        fill_histo("2010A_Nov4ReReco_32_1_kR9.root");
        fill_histo("2010A_Nov4ReReco_33_1_nVN.root");
        fill_histo("2010A_Nov4ReReco_34_1_18i.root");
        fill_histo("2010A_Nov4ReReco_35_1_ZOw.root");
        fill_histo("2010A_Nov4ReReco_36_2_w5E.root");
        fill_histo("2010A_Nov4ReReco_37_1_ie5.root");
        fill_histo("2010A_Nov4ReReco_38_1_AGU.root");
        fill_histo("2010A_Nov4ReReco_39_1_NRR.root");
        fill_histo("2010A_Nov4ReReco_40_1_y6f.root");
        fill_histo("2010A_Nov4ReReco_41_1_Dym.root");
        fill_histo("2010A_Nov4ReReco_42_1_3EW.root");
        fill_histo("2010A_Nov4ReReco_43_1_Gpx.root");
        fill_histo("2010A_Nov4ReReco_44_1_yzr.root");
        fill_histo("2010A_Nov4ReReco_45_1_Sww.root");
        fill_histo("2010A_Nov4ReReco_46_1_6Rk.root");
        fill_histo("2010A_Nov4ReReco_47_1_5zC.root");
        fill_histo("2010A_Nov4ReReco_48_1_WhA.root");
        fill_histo("2010A_Nov4ReReco_49_1_jsj.root");
        fill_histo("2010A_Nov4ReReco_50_1_oMU.root");
        fill_histo("2010A_Nov4ReReco_51_1_aOo.root");
        fill_histo("2010A_Nov4ReReco_52_1_BRv.root");
        fill_histo("2010A_Nov4ReReco_53_1_cBx.root");
        fill_histo("2010A_Nov4ReReco_54_1_BnH.root");
        fill_histo("2010A_Nov4ReReco_55_1_3Rz.root");
        fill_histo("2010A_Nov4ReReco_56_1_ptQ.root");
        fill_histo("2010A_Nov4ReReco_57_1_XA1.root");
        fill_histo("2010A_Nov4ReReco_58_1_k4C.root");
        fill_histo("2010A_Nov4ReReco_59_1_Vv9.root");
        fill_histo("2010A_Nov4ReReco_60_1_gYL.root");
        fill_histo("2010A_Nov4ReReco_61_1_vJZ.root");
        fill_histo("2010A_Nov4ReReco_62_1_9lL.root");
        fill_histo("2010A_Nov4ReReco_63_1_sMB.root");
        fill_histo("2010A_Nov4ReReco_64_1_wx6.root");
        fill_histo("2010A_Nov4ReReco_65_1_1V4.root");
        fill_histo("2010A_Nov4ReReco_66_1_dpY.root");
        fill_histo("2010A_Nov4ReReco_67_1_vx5.root");
        fill_histo("2010A_Nov4ReReco_68_1_B0T.root");
        fill_histo("2010A_Nov4ReReco_69_1_11R.root");
        fill_histo("2010A_Nov4ReReco_70_1_OP9.root");
        fill_histo("2010A_Nov4ReReco_71_1_6ZI.root");
        fill_histo("2010A_Nov4ReReco_72_1_gzM.root");
        fill_histo("2010A_Nov4ReReco_73_1_z0m.root");
        fill_histo("2010A_Nov4ReReco_74_1_Lc5.root");
        fill_histo("2010A_Nov4ReReco_75_1_Qby.root");
        fill_histo("2010A_Nov4ReReco_76_1_xub.root");
        fill_histo("2010A_Nov4ReReco_77_1_4mR.root");
        fill_histo("2010A_Nov4ReReco_78_1_n53.root");
        fill_histo("2010A_Nov4ReReco_79_1_VDL.root");
        fill_histo("2010A_Nov4ReReco_80_1_J6v.root");
        fill_histo("2010A_Nov4ReReco_81_1_voN.root");
        fill_histo("2010A_Nov4ReReco_82_1_7jR.root");
        fill_histo("2010A_Nov4ReReco_83_1_c80.root");
        fill_histo("2010A_Nov4ReReco_84_1_hBg.root");
        fill_histo("2010A_Nov4ReReco_85_1_Lfa.root");
        fill_histo("2010A_Nov4ReReco_86_1_TTp.root");
        fill_histo("2010A_Nov4ReReco_87_1_3Wi.root");
        fill_histo("2010A_Nov4ReReco_88_1_sGz.root");
        fill_histo("2010A_Nov4ReReco_89_1_I4V.root");
        fill_histo("2010A_Nov4ReReco_90_1_ABX.root");
        fill_histo("2010A_Nov4ReReco_91_1_1sj.root");
        fill_histo("2010A_Nov4ReReco_92_1_x7O.root");
        fill_histo("2010A_Nov4ReReco_93_1_lNu.root");
        fill_histo("2010A_Nov4ReReco_94_1_wdZ.root");
        fill_histo("2010A_Nov4ReReco_95_1_YkE.root");
        fill_histo("2010A_Nov4ReReco_96_1_E9B.root");
        fill_histo("2010A_Nov4ReReco_97_1_RMV.root");
        fill_histo("2010A_Nov4ReReco_98_1_hPR.root");
        fill_histo("2010A_Nov4ReReco_99_1_3vA.root");
        fill_histo("2010A_Nov4ReReco_100_1_mT5.root");
        fill_histo("2010A_Nov4ReReco_101_1_KMz.root");
        fill_histo("2010A_Nov4ReReco_102_1_EJz.root");
        fill_histo("2010A_Nov4ReReco_103_1_HGl.root");
        fill_histo("2010A_Nov4ReReco_104_1_tzP.root");
        fill_histo("2010A_Nov4ReReco_105_1_zPE.root");
      }
    }
  }

  if (effstudy==1) {
    std::cout<<"Finished processing "<<nfile<<" files."<<std::endl;
    std::cout<<"Total lumisections = "<<totallumi<<std::endl;  
    std::cout<<"Total Processed events = "<<totalevent<<" Total Processed hits = "<<totalentries<<std::endl;
    std::cout<<"Total hits filled to histograms = "<<totalhit<<std::endl;
    std::cout<<"---------------------------------------------------------"<<std::endl;
    std::cout<<"Layer 1 module 4 all ladder statistics: "<<stat_l1_m4_alldr<<std::endl;
    coarsedelay_ls->Fill(12,lscount);
  }

  run_ordered_eff = new TH1D("run_ordered_eff", "run_ordered_eff", nrun,0.5,nrun+0.5);
  run_ordered_dist = new TH1D("run_ordered_dist", "run_ordered_dist", nrun,0.5,nrun+0.5);
  run_ordered_bpix_eff = new TH1D("run_ordered_bpix_eff", "run_ordered_bpix_eff", nrun,0.5,nrun+0.5);
  run_ordered_bpix_dist = new TH1D("run_ordered_bpix_dist", "run_ordered_bpix_dist", nrun,0.5,nrun+0.5);
  run_ordered_fpix_eff = new TH1D("run_ordered_fpix_eff", "run_ordered_fpix_eff", nrun,0.5,nrun+0.5);
  run_ordered_fpix_dist = new TH1D("run_ordered_fpix_dist", "run_ordered_fpix_dist", nrun,0.5,nrun+0.5);
  run_ordered_l1_eff = new TH1D("run_ordered_l1_eff", "run_ordered_l1_eff", nrun,0.5,nrun+0.5);
  run_ordered_l1_dist = new TH1D("run_ordered_l1_dist", "run_ordered_l1_dist", nrun,0.5,nrun+0.5);
  run_ordered_l2_eff = new TH1D("run_ordered_l2_eff", "run_ordered_l2_eff", nrun,0.5,nrun+0.5);
  run_ordered_l2_dist = new TH1D("run_ordered_l2_dist", "run_ordered_l2_dist", nrun,0.5,nrun+0.5);
  run_ordered_l3_eff = new TH1D("run_ordered_l3_eff", "run_ordered_l3_eff", nrun,0.5,nrun+0.5);
  run_ordered_l3_dist = new TH1D("run_ordered_l3_dist", "run_ordered_l3_dist", nrun,0.5,nrun+0.5);
  run_ordered_dm2_eff = new TH1D("run_ordered_dm2_eff", "run_ordered_dm2_eff", nrun,0.5,nrun+0.5);
  run_ordered_dm2_dist = new TH1D("run_ordered_dm2_dist", "run_ordered_dm2_dist", nrun,0.5,nrun+0.5);
  run_ordered_dm1_eff = new TH1D("run_ordered_dm1_eff", "run_ordered_dm1_eff", nrun,0.5,nrun+0.5);
  run_ordered_dm1_dist = new TH1D("run_ordered_dm1_dist", "run_ordered_dm1_dist", nrun,0.5,nrun+0.5);
  run_ordered_dp1_eff = new TH1D("run_ordered_dp1_eff", "run_ordered_dp1_eff", nrun,0.5,nrun+0.5);
  run_ordered_dp1_dist = new TH1D("run_ordered_dp1_dist", "run_ordered_dp1_dist", nrun,0.5,nrun+0.5);
  run_ordered_dp2_eff = new TH1D("run_ordered_dp2_eff", "run_ordered_dp2_eff", nrun,0.5,nrun+0.5);
  run_ordered_dp2_dist = new TH1D("run_ordered_dp2_dist", "run_ordered_dp2_dist", nrun,0.5,nrun+0.5);

 //   TFile fi("newrocseltest_completetest_v1815.root");
//   full2010runs_roc_l1_eff = (TH3D*)fi.Get("rocsel_l1_eff");
//   full2010runs_roc_l2_eff = (TH3D*)fi.Get("rocsel_l2_eff");
//   full2010runs_roc_l3_eff = (TH3D*)fi.Get("rocsel_l3_eff");
//   full2010runs_roc_fpixI_eff = (TH3D*)fi.Get("rocsel_fpixI_eff");
//   full2010runs_roc_fpixO_eff = (TH3D*)fi.Get("rocsel_fpixO_eff");
//   full2010runs_roc_l1_dist = (TH3D*)fi.Get("rocsel_l1_dist");
//   full2010runs_roc_l2_dist = (TH3D*)fi.Get("rocsel_l2_dist");
//   full2010runs_roc_l3_dist = (TH3D*)fi.Get("rocsel_l3_dist");
//   full2010runs_roc_fpixI_dist = (TH3D*)fi.Get("rocsel_fpixI_dist");
//   full2010runs_roc_fpixO_dist = (TH3D*)fi.Get("rocsel_fpixO_dist");
//   full2010runs_roc_l1_eff->SetDirectory(NULL);
//   full2010runs_roc_l2_eff->SetDirectory(NULL);
//   full2010runs_roc_l3_eff->SetDirectory(NULL);
//   full2010runs_roc_fpixI_eff->SetDirectory(NULL);
//   full2010runs_roc_fpixO_eff->SetDirectory(NULL);
//   full2010runs_roc_l1_dist->SetDirectory(NULL);
//   full2010runs_roc_l2_dist->SetDirectory(NULL);
//   full2010runs_roc_l3_dist->SetDirectory(NULL);
//   full2010runs_roc_fpixI_dist->SetDirectory(NULL);
//   full2010runs_roc_fpixO_dist->SetDirectory(NULL);
//   fi.Close();

//   rocsel_l1_dist->Add(rocsel_l1_eff);
//   rocsel_l1_eff->Divide(rocsel_l1_dist);
//   rocsel_l2_dist->Add(rocsel_l2_eff);
//   rocsel_l2_eff->Divide(rocsel_l2_dist);
//   rocsel_l3_dist->Add(rocsel_l3_eff);
//   rocsel_l3_eff->Divide(rocsel_l3_dist);
//   rocsel_fpixI_dist->Add(rocsel_fpixI_eff);
//   rocsel_fpixI_eff->Divide(rocsel_fpixI_dist);
//   rocsel_fpixO_dist->Add(rocsel_fpixO_eff);
//   rocsel_fpixO_eff->Divide(rocsel_fpixO_dist);

  if (roceffstudy==1) {
    TH2D *roc2_l1_eff;
    TH2D *roc2_l2_eff;
    TH2D *roc2_l3_eff;
    TH2D *roc2_fpixI_eff;
    TH2D *roc2_fpixO_eff;
    TH2D *roc2_l1_dist;
    TH2D *roc2_l2_dist;
    TH2D *roc2_l3_dist;
    TH2D *roc2_fpixI_dist;
    TH2D *roc2_fpixO_dist;

    TFile frocimput(roceffstudyin);
    roc2_l1_eff = (TH2D*)frocimput.Get("roc_l1_eff");
    roc2_l2_eff = (TH2D*)frocimput.Get("roc_l2_eff");
    roc2_l3_eff = (TH2D*)frocimput.Get("roc_l3_eff");
    roc2_fpixI_eff = (TH2D*)frocimput.Get("roc_fpixI_eff");
    roc2_fpixO_eff = (TH2D*)frocimput.Get("roc_fpixO_eff");
    roc2_l1_dist = (TH2D*)frocimput.Get("roc_l1_dist");
    roc2_l2_dist = (TH2D*)frocimput.Get("roc_l2_dist");
    roc2_l3_dist = (TH2D*)frocimput.Get("roc_l3_dist");
    roc2_fpixI_dist = (TH2D*)frocimput.Get("roc_fpixI_dist");
    roc2_fpixO_dist = (TH2D*)frocimput.Get("roc_fpixO_dist");
    roc2_l1_eff->SetDirectory(NULL);
    roc2_l2_eff->SetDirectory(NULL);
    roc2_l3_eff->SetDirectory(NULL);
    roc2_fpixI_eff->SetDirectory(NULL);
    roc2_fpixO_eff->SetDirectory(NULL);
    roc2_l1_dist->SetDirectory(NULL);
    roc2_l2_dist->SetDirectory(NULL);
    roc2_l3_dist->SetDirectory(NULL);
    roc2_fpixI_dist->SetDirectory(NULL);
    roc2_fpixO_dist->SetDirectory(NULL);
    frocimput.Close();

    Int_t xbinno;
    Float_t xlowbin;

    if (neweff==1) {
      xbinno = 10000;
      xlowbin = 0.0001;
    } else {
      xbinno = 1000;
      xlowbin = 0.001;    
    }
    TH1D *roceff_bpix = new TH1D("roceff_bpix","roceff_bpix",xbinno,xlowbin,1.0+xlowbin);
    TH1D *rocdist_bpix = new TH1D("rocdist_bpix","rocdist_bpix",300,0.0,15000.0);
    TH1D *roceff_fpix = new TH1D("roceff_fpix","roceff_fpix",xbinno,xlowbin,1.0+xlowbin);
    TH1D *rocdist_fpix = new TH1D("rocdist_fpix","rocdist_fpix",300,0.0,15000.0);
    TH1D *roceff_fpix_p1 = new TH1D("roceff_fpix_p1","roceff_fpix_p1",xbinno,xlowbin,1.0+xlowbin);
    TH1D *rocdist_fpix_p1 = new TH1D("rocdist_fpix_p1","rocdist_fpix_p1",300,0.0,15000.0);
    TH1D *roceff_fpix_p2 = new TH1D("roceff_fpix_p2","roceff_fpix_p2",xbinno,xlowbin,1.0+xlowbin);
    TH1D *rocdist_fpix_p2 = new TH1D("rocdist_fpix_p2","rocdist_fpix_p2",300,0.0,15000.0);

    for (Int_t i=1;i<=72;i++) {
      for (Int_t j=1;j<=42;j++) {
	if (roc2_l1_dist->GetBinContent(i,j)!=0) {
	  roceff_bpix->Fill(roc2_l1_eff->GetBinContent(i,j));
	  rocdist_bpix->Fill(roc2_l1_dist->GetBinContent(i,j));
	}
      }
      for (Int_t j=1;j<=66;j++) {
	if (roc2_l2_dist->GetBinContent(i,j)!=0) {
	  roceff_bpix->Fill(roc2_l2_eff->GetBinContent(i,j));
	  rocdist_bpix->Fill(roc2_l2_dist->GetBinContent(i,j));
	}
      }
      for (Int_t j=1;j<=90;j++) {
	if (roc2_l3_dist->GetBinContent(i,j)!=0) {
	  roceff_bpix->Fill(roc2_l3_eff->GetBinContent(i,j));
	  rocdist_bpix->Fill(roc2_l3_dist->GetBinContent(i,j));
	}
      }
    }
    for (Int_t i=0;i<9;i++) {
      for (Int_t l=1;l<=6;l++) {
	for (Int_t j=1;j<=60;j++) {
	  if (roc2_fpixI_dist->GetBinContent(i,j)!=0) {
	    roceff_fpix->Fill(roc2_fpixI_eff->GetBinContent(i*6+l,j*2-1));
	    rocdist_fpix->Fill(roc2_fpixI_dist->GetBinContent(i*6+l,j*2-1));
	  }
	  if (roc2_fpixO_dist->GetBinContent(i,j)!=0) {
	    roceff_fpix->Fill(roc2_fpixO_eff->GetBinContent(i*6+l,j*2-1));
	    rocdist_fpix->Fill(roc2_fpixO_dist->GetBinContent(i*6+l,j*2-1));
	  }
	  // panel 2
	  if (i==0||i==2||i==6||i==9) {
	    if (roc2_fpixI_dist->GetBinContent(i,j)!=0) {
	      roceff_fpix_p2->Fill(roc2_fpixI_eff->GetBinContent(i*6+l,j*2-1));
	      rocdist_fpix_p2->Fill(roc2_fpixI_dist->GetBinContent(i*6+l,j*2-1));
	    }
	    if (roc2_fpixO_dist->GetBinContent(i,j)!=0) {
	      roceff_fpix_p2->Fill(roc2_fpixO_eff->GetBinContent(i*6+l,j*2-1));
	      rocdist_fpix_p2->Fill(roc2_fpixO_dist->GetBinContent(i*6+l,j*2-1));
	    }
	  } else {
	    if (roc2_fpixI_dist->GetBinContent(i,j)!=0) {
	      roceff_fpix_p1->Fill(roc2_fpixI_eff->GetBinContent(i*6+l,j*2-1));
	      rocdist_fpix_p1->Fill(roc2_fpixI_dist->GetBinContent(i*6+l,j*2-1));
	    }
	    if (roc2_fpixO_dist->GetBinContent(i,j)!=0) {
	      roceff_fpix_p1->Fill(roc2_fpixO_eff->GetBinContent(i*6+l,j*2-1));
	      rocdist_fpix_p1->Fill(roc2_fpixO_dist->GetBinContent(i*6+l,j*2-1));
	    }
	  }
	}
      }
    }
    
    Float_t fitrangelow;
    if (neweff==1)
      fitrangelow = 0.998;
    else
      fitrangelow = 0.9;

    TF1 *gausfit_bpix = new TF1("gausfit_bpix","gaus",fitrangelow,1.0);
    roceff_bpix->Fit("gausfit_bpix","RMWQ");
    Float_t mean_bpix = roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(1);
    Float_t sigma_bpix = roceff_bpix->GetFunction("gausfit_bpix")->GetParameter(2);
    delete gausfit_bpix;

    TF1 *gausfit_fpix = new TF1("gausfit_fpix","gaus",fitrangelow,1.0);
    roceff_fpix->Fit("gausfit_fpix","RMWQ");
    Float_t mean_fpix = roceff_fpix->GetFunction("gausfit_fpix")->GetParameter(1);
    Float_t sigma_fpix = roceff_fpix->GetFunction("gausfit_fpix")->GetParameter(2);
    delete gausfit_fpix;
    
    TH2D *significance_l1 = new TH2D("significance_l1","significance_l1",72,-4.5,4.5,42,-10.5,10.5);
    TH2D *significance_l2 = new TH2D("significance_l2","significance_l2",72,-4.5,4.5,66,-16.5,16.5);
    TH2D *significance_l3 = new TH2D("significance_l3","significance_l3",72,-4.5,4.5,90,-22.5,22.5);
    TH2D *significance_fpixI = new TH2D("significance_fpixI", "significance_fpixI", 54,-4.5,4.5,120,0.5,12.5);
    TH2D *significance_fpixO = new TH2D("significance_fpixO", "significance_fpixO", 54,-4.5,4.5,120,-12.5,-0.5);
    
    for (Int_t i=1;i<=72;i++) {
      for (Int_t j=1;j<=42;j++) {
	if (roc2_l1_dist->GetBinContent(i,j)!=0) {
	  significance_l1->SetBinContent(i,j,fabs(roc2_l1_eff->GetBinContent(i,j)-mean_bpix)/sigma_bpix);
	}
      }
      for (Int_t j=1;j<=66;j++) {
	if (roc2_l2_dist->GetBinContent(i,j)!=0) {
	  significance_l2->SetBinContent(i,j,fabs(roc2_l2_eff->GetBinContent(i,j)-mean_bpix)/sigma_bpix);
	}
      }
      for (Int_t j=1;j<=90;j++) {
	if (roc2_l3_dist->GetBinContent(i,j)!=0) {
	  significance_l3->SetBinContent(i,j,fabs(roc2_l3_eff->GetBinContent(i,j)-mean_bpix)/sigma_bpix);
	}
      }
    }
    for (Int_t i=1;i<=54;i++) {
      for (Int_t j=1;j<=120;j++) {
	if (roc2_fpixI_dist->GetBinContent(i,j)!=0)
	  significance_fpixI->SetBinContent(i,j,fabs(roc2_fpixI_eff->GetBinContent(i,j)-mean_fpix)/sigma_fpix);
	if (roc2_fpixO_dist->GetBinContent(i,j)!=0)
	  significance_fpixO->SetBinContent(i,j,fabs(roc2_fpixO_eff->GetBinContent(i,j)-mean_fpix)/sigma_fpix);
      }
    }

    TH1D *sigdist_bpix = new TH1D("sigdist_bpix","sigdist_bpix",1000,0.0,100.0);
    TH1D *sigdist_fpix = new TH1D("sigdist_fpix","sigdist_fpix",1000,0.0,100.0);

    for (Int_t i=1;i<=72;i++) {
      for (Int_t j=1;j<=42;j++) {
	if (significance_l1->GetBinContent(i,j)!=0) {
	  sigdist_bpix->Fill(significance_l1->GetBinContent(i,j));
	}
      }
      for (Int_t j=1;j<=66;j++) {
	if (significance_l2->GetBinContent(i,j)!=0) {
	  sigdist_bpix->Fill(significance_l2->GetBinContent(i,j));
	}
      }
      for (Int_t j=1;j<=90;j++) {
	if (significance_l3->GetBinContent(i,j)!=0) {
	  sigdist_bpix->Fill(significance_l3->GetBinContent(i,j));
	}
      }
    }
    for (Int_t i=1;i<=54;i++) {
      for (Int_t j=1;j<=120;j++) {
	if (significance_fpixI->GetBinContent(i,j)!=0)
	  sigdist_fpix->Fill(significance_fpixI->GetBinContent(i,j));
	if (significance_fpixO->GetBinContent(i,j)!=0)
	  sigdist_fpix->Fill(significance_fpixO->GetBinContent(i,j));
      }
    }

    TH2D *color_l1_eff = new TH2D("color_l1_eff","color_l1_eff",72,-4.5,4.5,42,-10.5,10.5);
    TH2D *color_l2_eff = new TH2D("color_l2_eff","color_l2_eff",72,-4.5,4.5,66,-16.5,16.5);
    TH2D *color_l3_eff = new TH2D("color_l3_eff","color_l3_eff",72,-4.5,4.5,90,-22.5,22.5);
//     TH2D *color_fpixI_eff = new TH2D("color_fpixI_eff","color_fpixI_eff",72,-4.5,4.5,66,-16.5,16.5);
//     TH2D *color_fpixO_eff = new TH2D("color_fpixO_eff","color_fpixO_eff",72,-4.5,4.5,90,-22.5,22.5);

    for (Int_t i=1;i<=72;i++) {
      for (Int_t j=1;j<=42;j++) {
	Float_t sig = significance_l1->GetBinContent(i,j);
	if (sig>0.0&&sig<2.0)
	  color_l1_eff->SetBinContent(i,j,0.5);
	else if (sig>=2.0&&sig<10.0)
	  color_l1_eff->SetBinContent(i,j,0.75);
	else if (sig>=10.0)
	  color_l1_eff->SetBinContent(i,j,1.0);
      }
      for (Int_t j=1;j<=66;j++) {
	Float_t sig = significance_l2->GetBinContent(i,j);
	if (sig>0.0&&sig<2.0)
	  color_l2_eff->SetBinContent(i,j,0.5);
	else if (sig>=2.0&&sig<10.0)
	  color_l2_eff->SetBinContent(i,j,0.75);
	else if (sig>=10.0)
	  color_l2_eff->SetBinContent(i,j,1.0);
      }
      for (Int_t j=1;j<=90;j++) {
	Float_t sig = significance_l3->GetBinContent(i,j);
	if (sig>0.0&&sig<2.0)
	  color_l3_eff->SetBinContent(i,j,0.5);
	else if (sig>=2.0&&sig<10.0)
	  color_l3_eff->SetBinContent(i,j,0.75);
	else if (sig>=10.0)
	  color_l3_eff->SetBinContent(i,j,1.0);
      }
    }

    Int_t efforsig = 0;
    Float_t sigmin = 7.0;
    Float_t effmin = 0.3;
    Float_t effmax = 0.9;

    // NEW ROC DEF
    // BmO
    for (Int_t lay=1;lay<=3;lay++) {
      Int_t maxladder = lay*6+4;
      // BmO, BmI
      for (Int_t j=2*maxladder;j>(-2*maxladder);j--) {
	// Outer (-x side)
	Int_t row = j;
	// Inner (+x side)
	if (j<=0)
	  row = 2*maxladder + 3 - j;
	// half (online/offline)
	Int_t half = -1;
	if (row==1||row==(2*maxladder-1)||row==(2*maxladder+3)||row==(4*maxladder+1))
	  half = 1;
	else if (row!=2||row!=(2*maxladder)||row!=(2*maxladder+4)||row!=(4*maxladder+2))
	  half = 0;
	// ladder (offline)
	Int_t ldr;
	for (Int_t m=-1*maxladder;m<=maxladder;m++) {
	  if (row==m*2+(maxladder*2+1)||row==m*2+(maxladder*2+2))
	    ldr = abs(m);
	}
	// sector (online/offline)
	Int_t sec;
	if (lay==1) {
	  sec = ldr - 1;
	  if (ldr==1)
	    sec = 1;
	  else if (ldr==10)
	    sec = 8;
	} else if (lay==2) {
	  sec = (ldr + (ldr%2)) / 2;
	} else if (lay==3) {
	  sec = (ldr-(ldr%3))+1;
	  if (ldr>=12)
	    sec = ((ldr+1)-((ldr+1)%3))/3+1;
	}
	// m: (-z side)
	for (Int_t col=32;col>=1;col--) {
	  Float_t eff, sig;
	  if (lay==1) {
	    sig = significance_l1->GetBinContent(col,row);
	    eff = roc2_l1_eff->GetBinContent(col,row);
	  } else if (lay==2) {
	    sig = significance_l2->GetBinContent(col,row);
	    eff = roc2_l2_eff->GetBinContent(col,row);
	  } else if (lay==3) {
	    sig = significance_l3->GetBinContent(col,row);
	    eff = roc2_l3_eff->GetBinContent(col,row);
	  }
	  if (efforsig==0&&sig>=sigmin) {
	    // module (offline)
	    Int_t mod;
	    for (Int_t k=-4;k<=4;k++) {
	      if (col>((k+4)*8)&&col<=((k+5)*8))
		mod = abs(k);
	    }
	    // roc number
	    Int_t roc = (col-1)%8;
	    if (row%2==0)
	      roc = 15 - roc;
	    if (j>0) {
	      if (half==1)
		std::cout<<"BPix_BmO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BmO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	    } else if (j<=0) {
	      if (half==1)
		std::cout<<"BPix_BmI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BmI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	    }
	  }
	  if (efforsig==1&&eff>effmin&&eff<=effmax) {
	    // module (offline)
	    Int_t mod;
	    for (Int_t k=-4;k<=4;k++) {
	      if (col>((k+4)*8)&&col<=((k+5)*8))
		mod = abs(k);
	    }
	    // roc number
	    Int_t roc = (col-1)%8;
	    if (row%2==0)
	      roc = 15 - roc;
	    if (j>0) {
	      if (half==1)
		std::cout<<"BPix_BmO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BmO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	    } else if (j<=0) {
	      if (half==1)
		std::cout<<"BPix_BmI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BmI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	    }
	  }
	}
      }
      // BpO, BpI
      for (Int_t j=2*maxladder;j>(-2*maxladder);j--) {
	// Outer (-x side)
	Int_t row = j;
	// Inner (+x side)
	if (j<=0)
	  row = 2*maxladder + 3 - j;
	// half (online/offline)
	Int_t half = -1;
	if (row==2||row==(2*maxladder)||row==(2*maxladder+4)||row==(4*maxladder+2))
	  half = 1;
	else if (row!=1||row!=(2*maxladder-1)||row!=(2*maxladder+3)||row!=(4*maxladder+1))
	  half = 0;
	// ladder (offline)
	Int_t ldr;
	for (Int_t m=-1*maxladder;m<=maxladder;m++) {
	  if (row==m*2+(maxladder*2+1)||row==m*2+(maxladder*2+2))
	    ldr = abs(m);
	}
	// sector (online/offline)
	Int_t sec;
	if (lay==1) {
	  sec = ldr - 1;
	  if (ldr==1)
	    sec = 1;
	  else if (ldr==10)
	    sec = 8;
	} else if (lay==2) {
	  sec = (ldr + (ldr%2)) / 2;
	} else if (lay==3) {
	  sec = ((ldr+2)-((ldr+2)%3))/3;
	  if (ldr>11)
	    sec = (ldr+1-((ldr+1)%3))/3+1;
	}
	// p: (+z side)
	for (Int_t col=41;col<=72;col++) {
	  Float_t eff, sig;
	  if (lay==1) {
	    sig = significance_l1->GetBinContent(col,row);
	    eff = roc2_l1_eff->GetBinContent(col,row);
	  } else if (lay==2) {
	    sig = significance_l2->GetBinContent(col,row);
	    eff = roc2_l2_eff->GetBinContent(col,row);
	  } else if (lay==3) {
	    sig = significance_l3->GetBinContent(col,row);
	    eff = roc2_l3_eff->GetBinContent(col,row);
	  }
	  if (efforsig==0&&sig>=sigmin) {
	    // module (offline)
	    Int_t mod;
	    for (Int_t k=-4;k<=4;k++) {
	      if (col>((k+4)*8)&&col<=((k+5)*8))
		mod = abs(k);
	    }
	    // roc number
	    Int_t roc = (col-1)%8+8;
	    if (row%2==0)
	      roc = 15 - roc;
	    if (j>0) {
	      if (half==1)
		std::cout<<"BPix_BpO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BpO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	    } else if (j<=0) {
	      if (half==1)
		std::cout<<"BPix_BpI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BpI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<" "<<sig<<std::endl;
	    }
	  }
	  if (efforsig==1&&eff>effmin&&eff<=effmax) {
	    // module (offline)
	    Int_t mod;
	    for (Int_t k=-4;k<=4;k++) {
	      if (col>((k+4)*8)&&col<=((k+5)*8))
		mod = abs(k);
	    }
	    // roc number
	    Int_t roc = (col-1)%8+8;
	    if (row%2==0)
	      roc = 15 - roc;
	    if (j>0) {
	      if (half==1)
		std::cout<<"BPix_BpO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BpO_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	    } else if (j<=0) {
	      if (half==1)
		std::cout<<"BPix_BpI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"H_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	      else if (half==0)
		std::cout<<"BPix_BpI_SEC"<<sec<<"_LYR"<<lay<<"_LDR"<<ldr<<"F_MOD"<<mod<<"_ROC"<<roc<<" "<<eff<<std::endl;
	    }
	  }
	}
      }
    }
    
    TFile *froceff= new TFile(roceffstudyout,"recreate");
    roc2_l1_eff->Write();
    roc2_l2_eff->Write();
    roc2_l3_eff->Write();
//     roc2_fpixI_eff->Write();
//     roc2_fpixO_eff->Write();
    significance_l1->Write();
    significance_l2->Write();
    significance_l3->Write();
//     significance_fpixI->Write();
//     significance_fpixO->Write();
//     color_l1_eff->Write();
//     color_l2_eff->Write();
//     color_l3_eff->Write();
//     color_fpixI_eff->Write();
//     color_fpixO_eff->Write();
    sigdist_bpix->Write();
//     sigdist_fpix->Write();
    roceff_bpix->Write();
//     rocdist_bpix->Write();
//     roceff_fpix->Write();
//     rocdist_fpix->Write();
    froceff->Close();
  }

  // Write histograms to an output file
  if (effstudy==1) {
    TFile *fo= new TFile(effstudyout,"recreate");

#ifdef COMPLETE
    if (effloss==1) {
      eff_dist_error_write(bx_eff,bx_dist);
      eff_dist_error_write(npix_mod_eff,npix_mod_dist);
      eff_dist_error_write(npix_roc_eff,npix_roc_dist);
      eff_dist_error_write(npix_bpix_eff,npix_bpix_dist);
      eff_dist_error_write(npix_l1_eff,npix_l1_dist);
      eff_dist_error_write(npix_l2_eff,npix_l2_dist);
      eff_dist_error_write(npix_l3_eff,npix_l3_dist);
    }
#endif

    rocsel_l1_eff->Write();
    rocsel_l1_dist->Write();
    rocsel_l2_eff->Write();
    rocsel_l2_dist->Write();
    rocsel_l3_eff->Write();
    rocsel_l3_dist->Write();
    rocsel_fpixI_eff->Write();
    rocsel_fpixI_dist->Write();
    rocsel_fpixO_eff->Write();
    rocsel_fpixO_dist->Write();
    
    rocsel_runno->Write();
    
    rocsel_bpix_stat->Write();
    rocsel_fpix_p1_stat->Write();
    rocsel_fpix_p2_stat->Write();
    
    eff_dist_error_write(eff[0],eff[1]);
   
    eff_dist_write(mod_l1_eff,mod_l1_dist);
    eff_dist_write(mod_l2_eff,mod_l2_dist);
    eff_dist_write(mod_l3_eff,mod_l3_dist);
    eff_dist_write(mod_fpixI_eff,mod_fpixI_dist);
    eff_dist_write(mod_fpixO_eff,mod_fpixO_dist);
    
    eff_dist_write(roc_l1_eff,roc_l1_dist);
    eff_dist_write(roc_l2_eff,roc_l2_dist);
    eff_dist_write(roc_l3_eff,roc_l3_dist);
    eff_dist_write(roc_fpixI_eff,roc_fpixI_dist);
    eff_dist_write(roc_fpixO_eff,roc_fpixO_dist);

    ratio = nval / nvp;
    roc_l1_dist2->Scale(ratio);
    roc_l1_dist2->Add(roc_l1_eff2);
    roc_l1_eff2->Divide(roc_l1_dist2);
    roc_l1_ratio->Add(roc_l1_eff);
    roc_l1_ratio->Divide(roc_l1_eff2);
    
//     roc_l1_eff2->Write();
//     roc_l1_dist2->Write();
//     roc_l1_ratio->Write();
  
    order_runs(run_eff,run_ordered_eff);
    order_runs(run_dist,run_ordered_dist);
    order_runs(run_bpix_eff,run_ordered_bpix_eff);
    order_runs(run_bpix_dist,run_ordered_bpix_dist);
    order_runs(run_fpix_eff,run_ordered_fpix_eff);
    order_runs(run_fpix_dist,run_ordered_fpix_dist);
    order_runs(run_l1_eff,run_ordered_l1_eff);
    order_runs(run_l1_dist,run_ordered_l1_dist);
    order_runs(run_l2_eff,run_ordered_l2_eff);
    order_runs(run_l2_dist,run_ordered_l2_dist);
    order_runs(run_l3_eff,run_ordered_l3_eff);
    order_runs(run_l3_dist,run_ordered_l3_dist);
    order_runs(run_dm2_eff,run_ordered_dm2_eff);
    order_runs(run_dm2_dist,run_ordered_dm2_dist);
    order_runs(run_dm1_eff,run_ordered_dm1_eff);
    order_runs(run_dm1_dist,run_ordered_dm1_dist);
    order_runs(run_dp1_eff,run_ordered_dp1_eff);
    order_runs(run_dp1_dist,run_ordered_dp1_dist);
    order_runs(run_dp2_eff,run_ordered_dp2_eff);
    order_runs(run_dp2_dist,run_ordered_dp2_dist);

    eff_dist_error_write(run_l1_eff,run_l1_dist);
    eff_dist_error_write(run_l2_eff,run_l2_dist);
    eff_dist_error_write(run_l3_eff,run_l3_dist);
    eff_dist_error_write(run_dm2_eff,run_dm2_dist);
    eff_dist_error_write(run_dm1_eff,run_dm1_dist);
    eff_dist_error_write(run_dp1_eff,run_dp1_dist);
    eff_dist_error_write(run_dp2_eff,run_dp2_dist);
    eff_dist_error_write(run_bpix_eff,run_bpix_dist);
    eff_dist_error_write(run_fpix_eff,run_fpix_dist);
    eff_dist_error_write(run_eff,run_dist);
    eff_dist_error_write(run_ordered_l1_eff,run_ordered_l1_dist);
    eff_dist_error_write(run_ordered_l2_eff,run_ordered_l2_dist);
    eff_dist_error_write(run_ordered_l3_eff,run_ordered_l3_dist);
    eff_dist_error_write(run_ordered_dm2_eff,run_ordered_dm2_dist);
    eff_dist_error_write(run_ordered_dm1_eff,run_ordered_dm1_dist);
    eff_dist_error_write(run_ordered_dp1_eff,run_ordered_dp1_dist);
    eff_dist_error_write(run_ordered_dp2_eff,run_ordered_dp2_dist);
    eff_dist_error_write(run_ordered_bpix_eff,run_ordered_bpix_dist);
    eff_dist_error_write(run_ordered_fpix_eff,run_ordered_fpix_dist);
    eff_dist_error_write(run_ordered_eff,run_ordered_dist);
    
    // Eff vs pt plots
    xcompcum_sum(pt_bpix_cumeff);
    xcompcum_sum(pt_bpix_cumdist);
    xcompcum_sum(pt_fpix_cumeff);
    xcompcum_sum(pt_fpix_cumdist);
    eff_dist_write(pt_bpix_eff,pt_bpix_dist);
    eff_dist_write(pt_fpix_eff,pt_fpix_dist);
    eff_dist_write(pt_bpix_cumeff,pt_bpix_cumdist);
    eff_dist_write(pt_fpix_cumeff,pt_fpix_cumdist);
    
//     xcompcum_sum(algo4_pt_bpix_cumeff);
//     xcompcum_sum(algo4_pt_bpix_cumdist);
//     xcompcum_sum(algo4_pt_fpix_cumeff);
//     xcompcum_sum(algo4_pt_fpix_cumdist);
//     xcompcum_sum(algo5_pt_bpix_cumeff);
//     xcompcum_sum(algo5_pt_bpix_cumdist);
//     xcompcum_sum(algo5_pt_fpix_cumeff);
//     xcompcum_sum(algo5_pt_fpix_cumdist);
//     xcompcum_sum(algo6_pt_bpix_cumeff);
//     xcompcum_sum(algo6_pt_bpix_cumdist);
//     xcompcum_sum(algo6_pt_fpix_cumeff);
//     xcompcum_sum(algo6_pt_fpix_cumdist);
//     xcompcum_sum(algo7_pt_bpix_cumeff);
//     xcompcum_sum(algo7_pt_bpix_cumdist);
//     xcompcum_sum(algo7_pt_fpix_cumeff);
//     xcompcum_sum(algo7_pt_fpix_cumdist);
//     xcompcum_sum(algo8_pt_bpix_cumeff);
//     xcompcum_sum(algo8_pt_bpix_cumdist);
//     xcompcum_sum(algo8_pt_fpix_cumeff);
//     xcompcum_sum(algo8_pt_fpix_cumdist);
//     xcompcum_sum(algo9_pt_bpix_cumeff);
//     xcompcum_sum(algo9_pt_bpix_cumdist);
//     xcompcum_sum(algo9_pt_fpix_cumeff);
//     xcompcum_sum(algo9_pt_fpix_cumdist);
//     algo45_pt_bpix_dist->Add(algo4_pt_bpix_dist);
//     algo45_pt_bpix_eff->Add(algo4_pt_bpix_eff);
//     algo45_pt_bpix_cumdist->Add(algo4_pt_bpix_cumdist);
//     algo45_pt_bpix_cumeff->Add(algo4_pt_bpix_cumeff);
//     algo45_pt_fpix_dist->Add(algo4_pt_fpix_dist);
//     algo45_pt_fpix_eff->Add(algo4_pt_fpix_eff);
//     algo45_pt_fpix_cumdist->Add(algo4_pt_fpix_cumdist);
//     algo45_pt_fpix_cumeff->Add(algo4_pt_fpix_cumeff);
//     algo45_pt_bpix_dist->Add(algo5_pt_bpix_dist);
//     algo45_pt_bpix_eff->Add(algo5_pt_bpix_eff);
//     algo45_pt_bpix_cumdist->Add(algo5_pt_bpix_cumdist);
//     algo45_pt_bpix_cumeff->Add(algo5_pt_bpix_cumeff);
//     algo45_pt_fpix_dist->Add(algo5_pt_fpix_dist);
//     algo45_pt_fpix_eff->Add(algo5_pt_fpix_eff);
//     algo45_pt_fpix_cumdist->Add(algo5_pt_fpix_cumdist);
//     algo45_pt_fpix_cumeff->Add(algo5_pt_fpix_cumeff);
//     eff_dist_write(algo4_pt_bpix_eff,algo4_pt_bpix_dist);
//     eff_dist_write(algo4_pt_fpix_eff,algo4_pt_fpix_dist);
//     eff_dist_write(algo4_pt_bpix_cumeff,algo4_pt_bpix_cumdist);
//     eff_dist_write(algo4_pt_fpix_cumeff,algo4_pt_fpix_cumdist);
//     eff_dist_write(algo5_pt_bpix_eff,algo5_pt_bpix_dist);
//     eff_dist_write(algo5_pt_fpix_eff,algo5_pt_fpix_dist);
//     eff_dist_write(algo5_pt_bpix_cumeff,algo5_pt_bpix_cumdist);
//     eff_dist_write(algo5_pt_fpix_cumeff,algo5_pt_fpix_cumdist);
//     eff_dist_write(algo6_pt_bpix_eff,algo6_pt_bpix_dist);
//     eff_dist_write(algo6_pt_fpix_eff,algo6_pt_fpix_dist);
//     eff_dist_write(algo6_pt_bpix_cumeff,algo6_pt_bpix_cumdist);
//     eff_dist_write(algo6_pt_fpix_cumeff,algo6_pt_fpix_cumdist);
//     eff_dist_write(algo7_pt_bpix_eff,algo7_pt_bpix_dist);
//     eff_dist_write(algo7_pt_fpix_eff,algo7_pt_fpix_dist);
//     eff_dist_write(algo7_pt_bpix_cumeff,algo7_pt_bpix_cumdist);
//     eff_dist_write(algo7_pt_fpix_cumeff,algo7_pt_fpix_cumdist);
//     eff_dist_write(algo8_pt_bpix_eff,algo8_pt_bpix_dist);
//     eff_dist_write(algo8_pt_fpix_eff,algo8_pt_fpix_dist);
//     eff_dist_write(algo8_pt_bpix_cumeff,algo8_pt_bpix_cumdist);
//     eff_dist_write(algo8_pt_fpix_cumeff,algo8_pt_fpix_cumdist);
//     eff_dist_write(algo9_pt_bpix_eff,algo9_pt_bpix_dist);
//     eff_dist_write(algo9_pt_fpix_eff,algo9_pt_fpix_dist);
//     eff_dist_write(algo9_pt_bpix_cumeff,algo9_pt_bpix_cumdist);
//     eff_dist_write(algo9_pt_fpix_cumeff,algo9_pt_fpix_cumdist);
//     eff_dist_write(algo45_pt_bpix_eff,algo45_pt_bpix_dist);
//     eff_dist_write(algo45_pt_fpix_eff,algo45_pt_fpix_dist);
//     eff_dist_write(algo45_pt_bpix_cumeff,algo45_pt_bpix_cumdist);
//     eff_dist_write(algo45_pt_fpix_cumeff,algo45_pt_fpix_cumdist);
        
    eff_dist_write(dz_20_l1_eff,dz_20_l1_dist);
    eff_dist_write(dz_20_l2_eff,dz_20_l2_dist);
    eff_dist_write(dz_20_l3_eff,dz_20_l3_dist);
    eff_dist_write(dz_20_fpix_eff,dz_20_fpix_dist);
    eff_dist_write(dz_2_l1_eff,dz_2_l1_dist);
    eff_dist_write(dz_2_l2_eff,dz_2_l2_dist);
    eff_dist_write(dz_2_l3_eff,dz_2_l3_dist);
    eff_dist_write(dz_2_fpix_eff,dz_2_fpix_dist);
    eff_dist_write(dz_02_l1_eff,dz_02_l1_dist);
    eff_dist_write(dz_02_l2_eff,dz_02_l2_dist);
    eff_dist_write(dz_02_l3_eff,dz_02_l3_dist);
    eff_dist_write(dz_02_fpix_eff,dz_02_fpix_dist);
    eff_dist_write(dz_002_l1_eff,dz_002_l1_dist);
    eff_dist_write(dz_002_l2_eff,dz_002_l2_dist);
    eff_dist_write(dz_002_l3_eff,dz_002_l3_dist);
    eff_dist_write(dz_002_fpix_eff,dz_002_fpix_dist);
    eff_dist_write(d0_2_l1_eff,d0_2_l1_dist);
    eff_dist_write(d0_2_l2_eff,d0_2_l2_dist);
    eff_dist_write(d0_2_l3_eff,d0_2_l3_dist);
    eff_dist_write(d0_2_fpix_eff,d0_2_fpix_dist);
    eff_dist_write(d0_02_l1_eff,d0_02_l1_dist);
    eff_dist_write(d0_02_l2_eff,d0_02_l2_dist);
    eff_dist_write(d0_02_l3_eff,d0_02_l3_dist);
    eff_dist_write(d0_02_fpix_eff,d0_02_fpix_dist);
    eff_dist_write(d0_002_l1_eff,d0_002_l1_dist);
    eff_dist_write(d0_002_l2_eff,d0_002_l2_dist);
    eff_dist_write(d0_002_l3_eff,d0_002_l3_dist);
    eff_dist_write(d0_002_fpix_eff,d0_002_fpix_dist);
    
    xcum_sum(dz_l1_cumdist);
    xcum_sum(dz_l1_cumeff);
    xcum_sum(dz_l2_cumdist);
    xcum_sum(dz_l2_cumeff);
    xcum_sum(dz_l3_cumdist);
    xcum_sum(dz_l3_cumeff);
    xcum_sum(dz_fpix_cumdist);
    xcum_sum(dz_fpix_cumeff);
    xcum_sum(d0_l1_cumdist);
    xcum_sum(d0_l1_cumeff);
    xcum_sum(d0_l2_cumdist);
    xcum_sum(d0_l2_cumeff);
    xcum_sum(d0_l3_cumdist);
    xcum_sum(d0_l3_cumeff);
    xcum_sum(d0_fpix_cumdist);
    xcum_sum(d0_fpix_cumeff);
    eff_dist_write(dz_l1_cumeff,dz_l1_cumdist);
    eff_dist_write(dz_l2_cumeff,dz_l2_cumdist);
    eff_dist_write(dz_l3_cumeff,dz_l3_cumdist);
    eff_dist_write(dz_fpix_cumeff,dz_fpix_cumdist);
    eff_dist_write(d0_l1_cumeff,d0_l1_cumdist);
    eff_dist_write(d0_l2_cumeff,d0_l2_cumdist);
    eff_dist_write(d0_l3_cumeff,d0_l3_cumdist);
    eff_dist_write(d0_fpix_cumeff,d0_fpix_cumdist);
    
    xcum_ycum_sum(dz_d0_l1_cumeff);
    xcum_ycum_sum(dz_d0_l1_cumdist);
    xcum_ycum_sum(dz_d0_l2_cumeff);
    xcum_ycum_sum(dz_d0_l2_cumdist);
    xcum_ycum_sum(dz_d0_l3_cumeff);
    xcum_ycum_sum(dz_d0_l3_cumdist);
    xcum_ycum_sum(dz_d0_fpix_cumeff);
    xcum_ycum_sum(dz_d0_fpix_cumdist);
    eff_dist_write(dz_d0_l1_eff,dz_d0_l1_dist);
    eff_dist_write(dz_d0_l2_eff,dz_d0_l2_dist);
    eff_dist_write(dz_d0_l3_eff,dz_d0_l3_dist);
    eff_dist_write(dz_d0_fpix_eff,dz_d0_fpix_dist);
    eff_dist_write(dz_d0_l1_cumeff,dz_d0_l1_cumdist);
    eff_dist_write(dz_d0_l2_cumeff,dz_d0_l2_cumdist);
    eff_dist_write(dz_d0_l3_cumeff,dz_d0_l3_cumdist);
    eff_dist_write(dz_d0_fpix_cumeff,dz_d0_fpix_cumdist);
    
    xcompcum_sum(nstrip_l1_cumeff);
    xcompcum_sum(nstrip_l1_cumdist);
    xcompcum_sum(nstrip_l2_cumeff);
    xcompcum_sum(nstrip_l2_cumdist);
    xcompcum_sum(nstrip_l3_cumeff);
    xcompcum_sum(nstrip_l3_cumdist);
    xcompcum_sum(nstrip_fpix_cumeff);
    xcompcum_sum(nstrip_fpix_cumdist);
    eff_dist_write(nstrip_l1_eff,nstrip_l1_dist);
    eff_dist_write(nstrip_l2_eff,nstrip_l2_dist);
    eff_dist_write(nstrip_l3_eff,nstrip_l3_dist);
    eff_dist_write(nstrip_fpix_eff,nstrip_fpix_dist);
    eff_dist_write(nstrip_l1_cumeff,nstrip_l1_cumdist);
    eff_dist_write(nstrip_l2_cumeff,nstrip_l2_cumdist);
    eff_dist_write(nstrip_l3_cumeff,nstrip_l3_cumdist);
    eff_dist_write(nstrip_fpix_cumeff,nstrip_fpix_cumdist);
    
    xcum_sum(normchi2_l1_cumeff);
    xcum_sum(normchi2_l1_cumdist);
    xcum_sum(normchi2_l2_cumeff);
    xcum_sum(normchi2_l2_cumdist);
    xcum_sum(normchi2_l3_cumeff);
    xcum_sum(normchi2_l3_cumdist);
    xcum_sum(normchi2_fpix_cumeff);
    xcum_sum(normchi2_fpix_cumdist);
    eff_dist_write(normchi2_l1_eff,normchi2_l1_dist);
    eff_dist_write(normchi2_l2_eff,normchi2_l2_dist);
    eff_dist_write(normchi2_l3_eff,normchi2_l3_dist);
    eff_dist_write(normchi2_fpix_eff,normchi2_fpix_dist);
    eff_dist_write(normchi2_l1_cumeff,normchi2_l1_cumdist);
    eff_dist_write(normchi2_l2_cumeff,normchi2_l2_cumdist);
    eff_dist_write(normchi2_l3_cumeff,normchi2_l3_cumdist);
    eff_dist_write(normchi2_fpix_cumeff,normchi2_fpix_cumdist);
    
    xcum_sum(normchi2_withpt_l1_cumeff);
    xcum_sum(normchi2_withpt_l1_cumdist);
    xcum_sum(normchi2_withpt_l2_cumeff);
    xcum_sum(normchi2_withpt_l2_cumdist);
    xcum_sum(normchi2_withpt_l3_cumeff);
    xcum_sum(normchi2_withpt_l3_cumdist);
    xcum_sum(normchi2_withpt_fpix_cumeff);
    xcum_sum(normchi2_withpt_fpix_cumdist);
    eff_dist_write(normchi2_withpt_l1_eff,normchi2_withpt_l1_dist);
    eff_dist_write(normchi2_withpt_l2_eff,normchi2_withpt_l2_dist);
    eff_dist_write(normchi2_withpt_l3_eff,normchi2_withpt_l3_dist);
    eff_dist_write(normchi2_withpt_fpix_eff,normchi2_withpt_fpix_dist);
    eff_dist_write(normchi2_withpt_l1_cumeff,normchi2_withpt_l1_cumdist);
    eff_dist_write(normchi2_withpt_l2_cumeff,normchi2_withpt_l2_cumdist);
    eff_dist_write(normchi2_withpt_l3_cumeff,normchi2_withpt_l3_cumdist);
    eff_dist_write(normchi2_withpt_fpix_cumeff,normchi2_withpt_fpix_cumdist);

    xcum_sum(drcl_l1_cumeff);
    xcum_sum(drcl_l1_cumdist);
    xcum_sum(drcl_l2_cumeff);
    xcum_sum(drcl_l2_cumdist);
    xcum_sum(drcl_l3_cumeff);
    xcum_sum(drcl_l3_cumdist);
    xcum_sum(drcl_fpix_cumeff);
    xcum_sum(drcl_fpix_cumdist);
    eff_dist_write(drcl_l1_eff,drcl_l1_dist);
    eff_dist_write(drcl_l2_eff,drcl_l2_dist);
    eff_dist_write(drcl_l3_eff,drcl_l3_dist);
    eff_dist_write(drcl_fpix_eff,drcl_fpix_dist);
    eff_dist_write(drcl_l1_cumeff,drcl_l1_cumdist);
    eff_dist_write(drcl_l2_cumeff,drcl_l2_cumdist);
    eff_dist_write(drcl_l3_cumeff,drcl_l3_cumdist);
    eff_dist_write(drcl_fpix_cumeff,drcl_fpix_cumdist);
    
    xcompcum_sum(drcl12_l1_cumeff);
    xcompcum_sum(drcl12_l1_cumdist);
    xcompcum_sum(drcl12_l2_cumeff);
    xcompcum_sum(drcl12_l2_cumdist);
    xcompcum_sum(drcl12_l3_cumeff);
    xcompcum_sum(drcl12_l3_cumdist);
    xcompcum_sum(drcl12_fpix_cumeff);
    xcompcum_sum(drcl12_fpix_cumdist);
    eff_dist_write(drcl12_l1_eff,drcl12_l1_dist);
    eff_dist_write(drcl12_l2_eff,drcl12_l2_dist);
    eff_dist_write(drcl12_l3_eff,drcl12_l3_dist);
    eff_dist_write(drcl12_fpix_eff,drcl12_fpix_dist);
    eff_dist_write(drcl12_l1_cumeff,drcl12_l1_cumdist);
    eff_dist_write(drcl12_l2_cumeff,drcl12_l2_cumdist);
    eff_dist_write(drcl12_l3_cumeff,drcl12_l3_cumdist);
    eff_dist_write(drcl12_fpix_cumeff,drcl12_fpix_cumdist);
    
    xcompcum_ycum_sum(drcl12_drcl_l1_cumeff);
    xcompcum_ycum_sum(drcl12_drcl_l1_cumdist);
    xcompcum_ycum_sum(drcl12_drcl_l2_cumeff);
    xcompcum_ycum_sum(drcl12_drcl_l2_cumdist);
    xcompcum_ycum_sum(drcl12_drcl_l3_cumeff);
    xcompcum_ycum_sum(drcl12_drcl_l3_cumdist);
    xcompcum_ycum_sum(drcl12_drcl_fpix_cumeff);
    xcompcum_ycum_sum(drcl12_drcl_fpix_cumdist);
    eff_dist_write(drcl12_drcl_l1_eff,drcl12_drcl_l1_dist);
    eff_dist_write(drcl12_drcl_l2_eff,drcl12_drcl_l2_dist);
    eff_dist_write(drcl12_drcl_l3_eff,drcl12_drcl_l3_dist);
    eff_dist_write(drcl12_drcl_fpix_eff,drcl12_drcl_fpix_dist);
    eff_dist_write(drcl12_drcl_l1_cumeff,drcl12_drcl_l1_cumdist);
    eff_dist_write(drcl12_drcl_l2_cumeff,drcl12_drcl_l2_cumdist);
    eff_dist_write(drcl12_drcl_l3_cumeff,drcl12_drcl_l3_cumdist);
    eff_dist_write(drcl12_drcl_fpix_cumeff,drcl12_drcl_fpix_cumdist);
    
    xcum_sum(neweff_eff);
    xcum_sum(neweff_dist);
    xcum_sum(neweff_bpix_eff);
    xcum_sum(neweff_bpix_dist);
    xcum_sum(neweff_fpix_eff);
    xcum_sum(neweff_fpix_dist);
    xcum_sum(neweff_l1_eff);
    xcum_sum(neweff_l1_dist);
    xcum_sum(neweff_l2_eff);
    xcum_sum(neweff_l2_dist);
    xcum_sum(neweff_l3_eff);
    xcum_sum(neweff_l3_dist);
    xcum_sum(neweff_dm2_eff);
    xcum_sum(neweff_dm2_dist);
    xcum_sum(neweff_dm1_eff);
    xcum_sum(neweff_dm1_dist);
    xcum_sum(neweff_dp1_eff);
    xcum_sum(neweff_dp1_dist);
    xcum_sum(neweff_dp2_eff);
    xcum_sum(neweff_dp2_dist);
    neweff_eff->Divide(neweff_dist);
    neweff_bpix_eff->Divide(neweff_bpix_dist);
    neweff_fpix_eff->Divide(neweff_fpix_dist);
    neweff_l1_eff->Divide(neweff_l1_dist);
    neweff_l2_eff->Divide(neweff_l2_dist);
    neweff_l3_eff->Divide(neweff_l3_dist);
    neweff_dm2_eff->Divide(neweff_dm2_dist);
    neweff_dm1_eff->Divide(neweff_dm1_dist);
    neweff_dp1_eff->Divide(neweff_dp1_dist);
    neweff_dp2_eff->Divide(neweff_dp2_dist);
    neweff_eff->Write();
    neweff_bpix_eff->Write();
    neweff_bpix_eff->Write();
    neweff_l1_eff->Write();
    neweff_l2_eff->Write();
    neweff_l3_eff->Write();
    neweff_dm2_eff->Write();
    neweff_dm1_eff->Write();
    neweff_dp1_eff->Write();
    neweff_dp2_eff->Write();
  
    xcum_ycum_sum(mod1_clx_cly_l1_neweff);
    xcum_ycum_sum(mod1_clx_cly_l1_newdist);
    xcum_ycum_sum(mod4_clx_cly_l1_neweff);
    xcum_ycum_sum(mod4_clx_cly_l1_newdist);
    mod1_clx_cly_l1_neweff->Divide(mod1_clx_cly_l1_newdist);
    mod1_clx_cly_l1_neweff->Write();
    mod4_clx_cly_l1_neweff->Divide(mod4_clx_cly_l1_newdist);
    mod4_clx_cly_l1_neweff->Write();
    xcum_sum(mod1_clx_l1_neweff);
    xcum_sum(mod1_clx_l1_newdist);
    xcum_sum(mod4_clx_l1_neweff);
    xcum_sum(mod4_clx_l1_newdist);
    mod1_clx_l1_neweff->Divide(mod1_clx_l1_newdist);
    mod1_clx_l1_neweff->Write();
    mod4_clx_l1_neweff->Divide(mod4_clx_l1_newdist);
    mod4_clx_l1_neweff->Write();
    xcum_sum(mod1_cly_l1_neweff);
    xcum_sum(mod1_cly_l1_newdist);
    xcum_sum(mod4_cly_l1_neweff);
    xcum_sum(mod4_cly_l1_newdist);
    mod1_cly_l1_neweff->Divide(mod1_cly_l1_newdist);
    mod1_cly_l1_neweff->Write();
    mod4_cly_l1_neweff->Divide(mod4_cly_l1_newdist);
    mod4_cly_l1_neweff->Write();
    xcum_ycum_sum(mod1_clx_cly_l2_neweff);
    xcum_ycum_sum(mod1_clx_cly_l2_newdist);
    xcum_ycum_sum(mod4_clx_cly_l2_neweff);
    xcum_ycum_sum(mod4_clx_cly_l2_newdist);
    mod1_clx_cly_l2_neweff->Divide(mod1_clx_cly_l2_newdist);
    mod1_clx_cly_l2_neweff->Write();
    mod4_clx_cly_l2_neweff->Divide(mod4_clx_cly_l2_newdist);
    mod4_clx_cly_l2_neweff->Write();
    xcum_sum(mod1_clx_l2_neweff);
    xcum_sum(mod1_clx_l2_newdist);
    xcum_sum(mod4_clx_l2_neweff);
    xcum_sum(mod4_clx_l2_newdist);
    mod1_clx_l2_neweff->Divide(mod1_clx_l2_newdist);
    mod1_clx_l2_neweff->Write();
    mod4_clx_l2_neweff->Divide(mod4_clx_l2_newdist);
    mod4_clx_l2_neweff->Write();
    xcum_sum(mod1_cly_l2_neweff);
    xcum_sum(mod1_cly_l2_newdist);
    xcum_sum(mod4_cly_l2_neweff);
    xcum_sum(mod4_cly_l2_newdist);
    mod1_cly_l2_neweff->Divide(mod1_cly_l2_newdist);
    mod1_cly_l2_neweff->Write();
    mod4_cly_l2_neweff->Divide(mod4_cly_l2_newdist);
    mod4_cly_l2_neweff->Write();
    xcum_ycum_sum(mod1_clx_cly_l3_neweff);
    xcum_ycum_sum(mod1_clx_cly_l3_newdist);
    xcum_ycum_sum(mod4_clx_cly_l3_neweff);
    xcum_ycum_sum(mod4_clx_cly_l3_newdist);
    mod1_clx_cly_l3_neweff->Divide(mod1_clx_cly_l3_newdist);
    mod1_clx_cly_l3_neweff->Write();
    mod4_clx_cly_l3_neweff->Divide(mod4_clx_cly_l3_newdist);
    mod4_clx_cly_l3_neweff->Write();
    xcum_sum(mod1_clx_l3_neweff);
    xcum_sum(mod1_clx_l3_newdist);
    xcum_sum(mod4_clx_l3_neweff);
    xcum_sum(mod4_clx_l3_newdist);
    mod1_clx_l3_neweff->Divide(mod1_clx_l3_newdist);
    mod1_clx_l3_neweff->Write();
    mod4_clx_l3_neweff->Divide(mod4_clx_l3_newdist);
    mod4_clx_l3_neweff->Write();
    xcum_sum(mod1_cly_l3_neweff);
    xcum_sum(mod1_cly_l3_newdist);
    xcum_sum(mod4_cly_l3_neweff);
    xcum_sum(mod4_cly_l3_newdist);
    mod1_cly_l3_neweff->Divide(mod1_cly_l3_newdist);
    mod1_cly_l3_neweff->Write();
    mod4_cly_l3_neweff->Divide(mod4_cly_l3_newdist);
    mod4_cly_l3_neweff->Write();
    
    xcum_sum(neweff2_eff);
    xcum_sum(neweff2_dist);
    xcum_sum(neweff2_bpix_eff);
    xcum_sum(neweff2_bpix_dist);
    xcum_sum(neweff2_fpix_eff);
    xcum_sum(neweff2_fpix_dist);
    xcum_sum(neweff2_l1_eff);
    xcum_sum(neweff2_l1_dist);
    xcum_sum(neweff2_l2_eff);
    xcum_sum(neweff2_l2_dist);
    xcum_sum(neweff2_l3_eff);
    xcum_sum(neweff2_l3_dist);
    xcum_sum(neweff2_dm2_eff);
    xcum_sum(neweff2_dm2_dist);
    xcum_sum(neweff2_dm1_eff);
    xcum_sum(neweff2_dm1_dist);
    xcum_sum(neweff2_dp1_eff);
    xcum_sum(neweff2_dp1_dist);
    xcum_sum(neweff2_dp2_eff);
    xcum_sum(neweff2_dp2_dist);
    neweff2_eff->Divide(neweff2_dist);
    neweff2_bpix_eff->Divide(neweff2_bpix_dist);
    neweff2_fpix_eff->Divide(neweff2_fpix_dist);
    neweff2_l1_eff->Divide(neweff2_l1_dist);
    neweff2_l2_eff->Divide(neweff2_l2_dist);
    neweff2_l3_eff->Divide(neweff2_l3_dist);
    neweff2_dm2_eff->Divide(neweff2_dm2_dist);
    neweff2_dm1_eff->Divide(neweff2_dm1_dist);
    neweff2_dp1_eff->Divide(neweff2_dp1_dist);
    neweff2_dp2_eff->Divide(neweff2_dp2_dist);
    neweff2_eff->Write();
    neweff2_bpix_eff->Write();
    neweff2_bpix_eff->Write();
    neweff2_l1_eff->Write();
    neweff2_l2_eff->Write();
    neweff2_l3_eff->Write();
    neweff2_dm2_eff->Write();
    neweff2_dm1_eff->Write();
    neweff2_dp1_eff->Write();
    neweff2_dp2_eff->Write();
    
    xcompcum_ycum_sum(drhit_drcl_neweff_eff);
    xcompcum_ycum_sum(drhit_drcl_neweff_dist);
    xcompcum_ycum_sum(drhit_drcl_neweff_bpix_eff);
    xcompcum_ycum_sum(drhit_drcl_neweff_bpix_dist);
    xcompcum_ycum_sum(drhit_drcl_neweff_fpix_eff);
    xcompcum_ycum_sum(drhit_drcl_neweff_fpix_dist);
    drhit_drcl_neweff_eff->Divide(drhit_drcl_neweff_dist);
    drhit_drcl_neweff_bpix_eff->Divide(drhit_drcl_neweff_bpix_dist);
    drhit_drcl_neweff_fpix_eff->Divide(drhit_drcl_neweff_fpix_dist);
    drhit_drcl_neweff_eff->Write();
    drhit_drcl_neweff_bpix_eff->Write();
    drhit_drcl_neweff_fpix_eff->Write();
  
    xcompcum_sum(drhit_l1_cumeff);
    xcompcum_sum(drhit_l1_cumdist);
    xcompcum_sum(drhit_l2_cumeff);
    xcompcum_sum(drhit_l2_cumdist);
    xcompcum_sum(drhit_l3_cumeff);
    xcompcum_sum(drhit_l3_cumdist);
    xcompcum_sum(drhit_fpix_cumeff);
    xcompcum_sum(drhit_fpix_cumdist);
    eff_dist_write(drhit_l1_eff,drhit_l1_dist);
    eff_dist_write(drhit_l2_eff,drhit_l2_dist);
    eff_dist_write(drhit_l3_eff,drhit_l3_dist);
    eff_dist_write(drhit_fpix_eff,drhit_fpix_dist);
    eff_dist_write(drhit_l1_cumeff,drhit_l1_cumdist);
    eff_dist_write(drhit_l2_cumeff,drhit_l2_cumdist);
    eff_dist_write(drhit_l3_cumeff,drhit_l3_cumdist);
    eff_dist_write(drhit_fpix_cumeff,drhit_fpix_cumdist);
    
    eff_dist_error_write(outerfid_l1_p_eff,outerfid_l1_p_dist);
    eff_dist_error_write(outerfid_l1_m_eff,outerfid_l1_m_dist);
    eff_dist_error_write(outerfid_l2_p_eff,outerfid_l2_p_dist);
    eff_dist_error_write(outerfid_l2_m_eff,outerfid_l2_m_dist);
    eff_dist_error_write(outerfid_l3_p_eff,outerfid_l3_p_dist);
    eff_dist_error_write(outerfid_l3_m_eff,outerfid_l3_m_dist);
    
    // fid region calc
    eff_dist_write(fid_l1_full_lx_eff,fid_l1_full_lx_dist);
    eff_dist_write(fid_l1_htI_lx_eff,fid_l1_htI_lx_dist);
    eff_dist_write(fid_l1_htO_lx_eff,fid_l1_htO_lx_dist);
    eff_dist_write(fid_l1_hbI_lx_eff,fid_l1_hbI_lx_dist);
    eff_dist_write(fid_l1_hbO_lx_eff,fid_l1_hbO_lx_dist);
    eff_dist_write(fid_l1_full_ly_eff,fid_l1_full_ly_dist);
    eff_dist_write(fid_l1_htI_ly_eff,fid_l1_htI_ly_dist);
    eff_dist_write(fid_l1_htO_ly_eff,fid_l1_htO_ly_dist);
    eff_dist_write(fid_l1_hbI_ly_eff,fid_l1_hbI_ly_dist);
    eff_dist_write(fid_l1_hbO_ly_eff,fid_l1_hbO_ly_dist);
    eff_dist_write(fid_l1_full_eff,fid_l1_full_dist);
    eff_dist_write(fid_l1_htI_eff,fid_l1_htI_dist);
    eff_dist_write(fid_l1_htO_eff,fid_l1_htO_dist);
    eff_dist_write(fid_l1_hbI_eff,fid_l1_hbI_dist);
    eff_dist_write(fid_l1_hbO_eff,fid_l1_hbO_dist);
    eff_dist_write(fid_l2_full_lx_eff,fid_l2_full_lx_dist);
    eff_dist_write(fid_l2_htI_lx_eff,fid_l2_htI_lx_dist);
    eff_dist_write(fid_l2_htO_lx_eff,fid_l2_htO_lx_dist);
    eff_dist_write(fid_l2_hbI_lx_eff,fid_l2_hbI_lx_dist);
    eff_dist_write(fid_l2_hbO_lx_eff,fid_l2_hbO_lx_dist);
    eff_dist_write(fid_l2_full_ly_eff,fid_l2_full_ly_dist);
    eff_dist_write(fid_l2_htI_ly_eff,fid_l2_htI_ly_dist);
    eff_dist_write(fid_l2_htO_ly_eff,fid_l2_htO_ly_dist);
    eff_dist_write(fid_l2_hbI_ly_eff,fid_l2_hbI_ly_dist);
    eff_dist_write(fid_l2_hbO_ly_eff,fid_l2_hbO_ly_dist);
    eff_dist_write(fid_l2_full_eff,fid_l2_full_dist);
    eff_dist_write(fid_l2_htI_eff,fid_l2_htI_dist);
    eff_dist_write(fid_l2_htO_eff,fid_l2_htO_dist);
    eff_dist_write(fid_l2_hbI_eff,fid_l2_hbI_dist);
    eff_dist_write(fid_l2_hbO_eff,fid_l2_hbO_dist);
    eff_dist_write(fid_l3_full_lx_eff,fid_l3_full_lx_dist);
    eff_dist_write(fid_l3_htI_lx_eff,fid_l3_htI_lx_dist);
    eff_dist_write(fid_l3_htO_lx_eff,fid_l3_htO_lx_dist);
    eff_dist_write(fid_l3_hbI_lx_eff,fid_l3_hbI_lx_dist);
    eff_dist_write(fid_l3_hbO_lx_eff,fid_l3_hbO_lx_dist);
    eff_dist_write(fid_l3_full_ly_eff,fid_l3_full_ly_dist);
    eff_dist_write(fid_l3_htI_ly_eff,fid_l3_htI_ly_dist);
    eff_dist_write(fid_l3_htO_ly_eff,fid_l3_htO_ly_dist);
    eff_dist_write(fid_l3_hbI_ly_eff,fid_l3_hbI_ly_dist);
    eff_dist_write(fid_l3_hbO_ly_eff,fid_l3_hbO_ly_dist);
    eff_dist_write(fid_l3_full_eff,fid_l3_full_dist);
    eff_dist_write(fid_l3_htI_eff,fid_l3_htI_dist);
    eff_dist_write(fid_l3_htO_eff,fid_l3_htO_dist);
    eff_dist_write(fid_l3_hbI_eff,fid_l3_hbI_dist);
    eff_dist_write(fid_l3_hbO_eff,fid_l3_hbO_dist);
    
    eff_dist_write(fid_dm2_p1_m1_lx_eff,fid_dm2_p1_m1_lx_dist);
    eff_dist_write(fid_dm2_p1_m2_lx_eff,fid_dm2_p1_m2_lx_dist);
    eff_dist_write(fid_dm2_p1_m3_lx_eff,fid_dm2_p1_m3_lx_dist);
    eff_dist_write(fid_dm2_p1_m4_lx_eff,fid_dm2_p1_m4_lx_dist);
    eff_dist_write(fid_dm2_p2_m1_lx_eff,fid_dm2_p2_m1_lx_dist);
    eff_dist_write(fid_dm2_p2_m2_lx_eff,fid_dm2_p2_m2_lx_dist);
    eff_dist_write(fid_dm2_p2_m3_lx_eff,fid_dm2_p2_m3_lx_dist);
    eff_dist_write(fid_dm2_p1_m1_ly_eff,fid_dm2_p1_m1_ly_dist);
    eff_dist_write(fid_dm2_p1_m2_ly_eff,fid_dm2_p1_m2_ly_dist);
    eff_dist_write(fid_dm2_p1_m3_ly_eff,fid_dm2_p1_m3_ly_dist);
    eff_dist_write(fid_dm2_p1_m4_ly_eff,fid_dm2_p1_m4_ly_dist);
    eff_dist_write(fid_dm2_p2_m1_ly_eff,fid_dm2_p2_m1_ly_dist);
    eff_dist_write(fid_dm2_p2_m2_ly_eff,fid_dm2_p2_m2_ly_dist);
    eff_dist_write(fid_dm2_p2_m3_ly_eff,fid_dm2_p2_m3_ly_dist);
    eff_dist_write(fid_dm2_p1_m1_eff,fid_dm2_p1_m1_dist);
    eff_dist_write(fid_dm2_p1_m2_eff,fid_dm2_p1_m2_dist);
    eff_dist_write(fid_dm2_p1_m3_eff,fid_dm2_p1_m3_dist);
    eff_dist_write(fid_dm2_p1_m4_eff,fid_dm2_p1_m4_dist);
    eff_dist_write(fid_dm2_p2_m1_eff,fid_dm2_p2_m1_dist);
    eff_dist_write(fid_dm2_p2_m2_eff,fid_dm2_p2_m2_dist);
    eff_dist_write(fid_dm2_p2_m3_eff,fid_dm2_p2_m3_dist);
    eff_dist_write(fid_dm1_p1_m1_lx_eff,fid_dm1_p1_m1_lx_dist);
    eff_dist_write(fid_dm1_p1_m2_lx_eff,fid_dm1_p1_m2_lx_dist);
    eff_dist_write(fid_dm1_p1_m3_lx_eff,fid_dm1_p1_m3_lx_dist);
    eff_dist_write(fid_dm1_p1_m4_lx_eff,fid_dm1_p1_m4_lx_dist);
    eff_dist_write(fid_dm1_p2_m1_lx_eff,fid_dm1_p2_m1_lx_dist);
    eff_dist_write(fid_dm1_p2_m2_lx_eff,fid_dm1_p2_m2_lx_dist);
    eff_dist_write(fid_dm1_p2_m3_lx_eff,fid_dm1_p2_m3_lx_dist);
    eff_dist_write(fid_dm1_p1_m1_ly_eff,fid_dm1_p1_m1_ly_dist);
    eff_dist_write(fid_dm1_p1_m2_ly_eff,fid_dm1_p1_m2_ly_dist);
    eff_dist_write(fid_dm1_p1_m3_ly_eff,fid_dm1_p1_m3_ly_dist);
    eff_dist_write(fid_dm1_p1_m4_ly_eff,fid_dm1_p1_m4_ly_dist);
    eff_dist_write(fid_dm1_p2_m1_ly_eff,fid_dm1_p2_m1_ly_dist);
    eff_dist_write(fid_dm1_p2_m2_ly_eff,fid_dm1_p2_m2_ly_dist);
    eff_dist_write(fid_dm1_p2_m3_ly_eff,fid_dm1_p2_m3_ly_dist);
    eff_dist_write(fid_dm1_p1_m1_eff,fid_dm1_p1_m1_dist);
    eff_dist_write(fid_dm1_p1_m2_eff,fid_dm1_p1_m2_dist);
    eff_dist_write(fid_dm1_p1_m3_eff,fid_dm1_p1_m3_dist);
    eff_dist_write(fid_dm1_p1_m4_eff,fid_dm1_p1_m4_dist);
    eff_dist_write(fid_dm1_p2_m1_eff,fid_dm1_p2_m1_dist);
    eff_dist_write(fid_dm1_p2_m2_eff,fid_dm1_p2_m2_dist);
    eff_dist_write(fid_dm1_p2_m3_eff,fid_dm1_p2_m3_dist);
    eff_dist_write(fid_dp1_p1_m1_lx_eff,fid_dp1_p1_m1_lx_dist);
    eff_dist_write(fid_dp1_p1_m2_lx_eff,fid_dp1_p1_m2_lx_dist);
    eff_dist_write(fid_dp1_p1_m3_lx_eff,fid_dp1_p1_m3_lx_dist);
    eff_dist_write(fid_dp1_p1_m4_lx_eff,fid_dp1_p1_m4_lx_dist);
    eff_dist_write(fid_dp1_p2_m1_lx_eff,fid_dp1_p2_m1_lx_dist);
    eff_dist_write(fid_dp1_p2_m2_lx_eff,fid_dp1_p2_m2_lx_dist);
    eff_dist_write(fid_dp1_p2_m3_lx_eff,fid_dp1_p2_m3_lx_dist);
    eff_dist_write(fid_dp1_p1_m1_ly_eff,fid_dp1_p1_m1_ly_dist);
    eff_dist_write(fid_dp1_p1_m2_ly_eff,fid_dp1_p1_m2_ly_dist);
    eff_dist_write(fid_dp1_p1_m3_ly_eff,fid_dp1_p1_m3_ly_dist);
    eff_dist_write(fid_dp1_p1_m4_ly_eff,fid_dp1_p1_m4_ly_dist);
    eff_dist_write(fid_dp1_p2_m1_ly_eff,fid_dp1_p2_m1_ly_dist);
    eff_dist_write(fid_dp1_p2_m2_ly_eff,fid_dp1_p2_m2_ly_dist);
    eff_dist_write(fid_dp1_p2_m3_ly_eff,fid_dp1_p2_m3_ly_dist);
    eff_dist_write(fid_dp1_p1_m1_eff,fid_dp1_p1_m1_dist);
    eff_dist_write(fid_dp1_p1_m2_eff,fid_dp1_p1_m2_dist);
    eff_dist_write(fid_dp1_p1_m3_eff,fid_dp1_p1_m3_dist);
    eff_dist_write(fid_dp1_p1_m4_eff,fid_dp1_p1_m4_dist);
    eff_dist_write(fid_dp1_p2_m1_eff,fid_dp1_p2_m1_dist);
    eff_dist_write(fid_dp1_p2_m2_eff,fid_dp1_p2_m2_dist);
    eff_dist_write(fid_dp1_p2_m3_eff,fid_dp1_p2_m3_dist);
    eff_dist_write(fid_dp2_p1_m1_lx_eff,fid_dp2_p1_m1_lx_dist);
    eff_dist_write(fid_dp2_p1_m2_lx_eff,fid_dp2_p1_m2_lx_dist);
    eff_dist_write(fid_dp2_p1_m3_lx_eff,fid_dp2_p1_m3_lx_dist);
    eff_dist_write(fid_dp2_p1_m4_lx_eff,fid_dp2_p1_m4_lx_dist);
    eff_dist_write(fid_dp2_p2_m1_lx_eff,fid_dp2_p2_m1_lx_dist);
    eff_dist_write(fid_dp2_p2_m2_lx_eff,fid_dp2_p2_m2_lx_dist);
    eff_dist_write(fid_dp2_p2_m3_lx_eff,fid_dp2_p2_m3_lx_dist);
    eff_dist_write(fid_dp2_p1_m1_ly_eff,fid_dp2_p1_m1_ly_dist);
    eff_dist_write(fid_dp2_p1_m2_ly_eff,fid_dp2_p1_m2_ly_dist);
    eff_dist_write(fid_dp2_p1_m3_ly_eff,fid_dp2_p1_m3_ly_dist);
    eff_dist_write(fid_dp2_p1_m4_ly_eff,fid_dp2_p1_m4_ly_dist);
    eff_dist_write(fid_dp2_p2_m1_ly_eff,fid_dp2_p2_m1_ly_dist);
    eff_dist_write(fid_dp2_p2_m2_ly_eff,fid_dp2_p2_m2_ly_dist);
    eff_dist_write(fid_dp2_p2_m3_ly_eff,fid_dp2_p2_m3_ly_dist);
    eff_dist_write(fid_dp2_p1_m1_eff,fid_dp2_p1_m1_dist);
    eff_dist_write(fid_dp2_p1_m2_eff,fid_dp2_p1_m2_dist);
    eff_dist_write(fid_dp2_p1_m3_eff,fid_dp2_p1_m3_dist);
    eff_dist_write(fid_dp2_p1_m4_eff,fid_dp2_p1_m4_dist);
    eff_dist_write(fid_dp2_p2_m1_eff,fid_dp2_p2_m1_dist);
    eff_dist_write(fid_dp2_p2_m2_eff,fid_dp2_p2_m2_dist);
    eff_dist_write(fid_dp2_p2_m3_eff,fid_dp2_p2_m3_dist);

    eff_dist_error_write(finedelay_l1_eff,finedelay_l1_dist);
    eff_dist_error_write(finedelay_l2_eff,finedelay_l2_dist);
    eff_dist_error_write(finedelay_l3_eff,finedelay_l3_dist);
    eff_dist_error_write(finedelay_dm2_eff,finedelay_dm2_dist);
    eff_dist_error_write(finedelay_dm1_eff,finedelay_dm1_dist);
    eff_dist_error_write(finedelay_dp1_eff,finedelay_dp1_dist);
    eff_dist_error_write(finedelay_dp2_eff,finedelay_dp2_dist);
    eff_dist_error_write(finedelay_bpix_eff,finedelay_bpix_dist);
    eff_dist_error_write(finedelay_fpix_eff,finedelay_fpix_dist);
    eff_dist_error_write(finedelay_eff,finedelay_dist);

    eff_dist_error_write(coarsedelay[0][0],coarsedelay[0][1]);
    eff_dist_error_write(coarsedelay[1][0],coarsedelay[1][1]);
    eff_dist_error_write(coarsedelay[2][0],coarsedelay[2][1]);
    eff_dist_error_write(coarsedelay[3][0],coarsedelay[3][1]);
    eff_dist_error_write(coarsedelay[4][0],coarsedelay[4][1]);
    eff_dist_error_write(coarsedelay[5][0],coarsedelay[5][1]);
    eff_dist_error_write(coarsedelay[6][0],coarsedelay[6][1]);
    eff_dist_error_write(coarsedelay[7][0],coarsedelay[7][1]);
    eff_dist_error_write(coarsedelay[8][0],coarsedelay[8][1]);
    eff_dist_error_write(coarsedelay[9][0],coarsedelay[9][1]);

    coarsedelay_l1_avgclusize->Divide(coarsedelay_l1_nclust);
    coarsedelay_l2_avgclusize->Divide(coarsedelay_l2_nclust);
    coarsedelay_l3_avgclusize->Divide(coarsedelay_l3_nclust);
    coarsedelay_dm2_avgclusize->Divide(coarsedelay_dm2_nclust);
    coarsedelay_dm1_avgclusize->Divide(coarsedelay_dm1_nclust);
    coarsedelay_dp1_avgclusize->Divide(coarsedelay_dp1_nclust);
    coarsedelay_dp2_avgclusize->Divide(coarsedelay_dp2_nclust);
    coarsedelay_bpix_avgclusize->Divide(coarsedelay_bpix_nclust);
    coarsedelay_fpix_avgclusize->Divide(coarsedelay_fpix_nclust);
    coarsedelay_avgclusize->Divide(coarsedelay_nclust);
    coarsedelay_l1_avgclusize->Write();
    coarsedelay_l2_avgclusize->Write();
    coarsedelay_l3_avgclusize->Write();
    coarsedelay_dm2_avgclusize->Write();
    coarsedelay_dm1_avgclusize->Write();
    coarsedelay_dp1_avgclusize->Write();
    coarsedelay_dp2_avgclusize->Write();
    coarsedelay_bpix_avgclusize->Write();
    coarsedelay_fpix_avgclusize->Write();
    coarsedelay_avgclusize->Write();
    coarsedelay_l1_nclust->Write();
    coarsedelay_l2_nclust->Write();
    coarsedelay_l3_nclust->Write();
    coarsedelay_dm2_nclust->Write();
    coarsedelay_dm1_nclust->Write();
    coarsedelay_dp1_nclust->Write();
    coarsedelay_dp2_nclust->Write();
    coarsedelay_bpix_nclust->Write();
    coarsedelay_fpix_nclust->Write();
    coarsedelay_nclust->Write();

    coarsedelay_ontrack_l1_avgclusize->Divide(coarsedelay_ontrack_l1_nclust);
    coarsedelay_ontrack_l2_avgclusize->Divide(coarsedelay_ontrack_l2_nclust);
    coarsedelay_ontrack_l3_avgclusize->Divide(coarsedelay_ontrack_l3_nclust);
    coarsedelay_ontrack_dm2_avgclusize->Divide(coarsedelay_ontrack_dm2_nclust);
    coarsedelay_ontrack_dm1_avgclusize->Divide(coarsedelay_ontrack_dm1_nclust);
    coarsedelay_ontrack_dp1_avgclusize->Divide(coarsedelay_ontrack_dp1_nclust);
    coarsedelay_ontrack_dp2_avgclusize->Divide(coarsedelay_ontrack_dp2_nclust);
    coarsedelay_ontrack_bpix_avgclusize->Divide(coarsedelay_ontrack_bpix_nclust);
    coarsedelay_ontrack_fpix_avgclusize->Divide(coarsedelay_ontrack_fpix_nclust);
    coarsedelay_ontrack_avgclusize->Divide(coarsedelay_ontrack_nclust);
    coarsedelay_ontrack_l1_avgclusize->Write();
    coarsedelay_ontrack_l2_avgclusize->Write();
    coarsedelay_ontrack_l3_avgclusize->Write();
    coarsedelay_ontrack_dm2_avgclusize->Write();
    coarsedelay_ontrack_dm1_avgclusize->Write();
    coarsedelay_ontrack_dp1_avgclusize->Write();
    coarsedelay_ontrack_dp2_avgclusize->Write();
    coarsedelay_ontrack_bpix_avgclusize->Write();
    coarsedelay_ontrack_fpix_avgclusize->Write();
    coarsedelay_ontrack_avgclusize->Write();
    coarsedelay_ontrack_l1_nclust->Write();
    coarsedelay_ontrack_l2_nclust->Write();
    coarsedelay_ontrack_l3_nclust->Write();
    coarsedelay_ontrack_dm2_nclust->Write();
    coarsedelay_ontrack_dm1_nclust->Write();
    coarsedelay_ontrack_dp1_nclust->Write();
    coarsedelay_ontrack_dp2_nclust->Write();
    coarsedelay_ontrack_bpix_nclust->Write();
    coarsedelay_ontrack_fpix_nclust->Write();
    coarsedelay_ontrack_nclust->Write();

    eff_dist_error_write(wbc_l1_eff,wbc_l1_dist);
    eff_dist_error_write(wbc_l2_eff,wbc_l2_dist);
    eff_dist_error_write(wbc_l3_eff,wbc_l3_dist);
    eff_dist_error_write(wbc_dm2_eff,wbc_dm2_dist);
    eff_dist_error_write(wbc_dm1_eff,wbc_dm1_dist);
    eff_dist_error_write(wbc_dp1_eff,wbc_dp1_dist);
    eff_dist_error_write(wbc_dp2_eff,wbc_dp2_dist);
    eff_dist_error_write(wbc_bpix_eff,wbc_bpix_dist);
    eff_dist_error_write(wbc_fpix_eff,wbc_fpix_dist);
    eff_dist_error_write(wbc_eff,wbc_dist);

    eff_dist_error_write(voltage_l1_eff,voltage_l1_dist);
    eff_dist_error_write(voltage_l3_eff,voltage_l3_dist);
    eff_dist_error_write(voltage_bpix_eff,voltage_bpix_dist);
    eff_dist_error_write(voltage_fpix_eff,voltage_fpix_dist);

//     avghitperls_l1_dist->Add(coarsedelay_l1_dist);
//     avghitperls_l1_dist->Divide(coarsedelay_ls);
//     avghitperls_l2_dist->Add(coarsedelay_l2_dist);
//     avghitperls_l2_dist->Divide(coarsedelay_ls);
//     avghitperls_l3_dist->Add(coarsedelay_l3_dist);
//     avghitperls_l3_dist->Divide(coarsedelay_ls);
//     avghitperls_dm1_dist->Add(coarsedelay_dm1_dist);
//     avghitperls_dm1_dist->Divide(coarsedelay_ls);
//     avghitperls_dm2_dist->Add(coarsedelay_dm2_dist);
//     avghitperls_dm2_dist->Divide(coarsedelay_ls);
//     avghitperls_dp1_dist->Add(coarsedelay_dp1_dist);
//     avghitperls_dp1_dist->Divide(coarsedelay_ls);
//     avghitperls_dp2_dist->Add(coarsedelay_dp2_dist);
//     avghitperls_dp2_dist->Divide(coarsedelay_ls);
//     avghitperls_bpix_dist->Add(coarsedelay_bpix_dist);
//     avghitperls_bpix_dist->Divide(coarsedelay_ls);
//     avghitperls_fpix_dist->Add(coarsedelay_fpix_dist);
//     avghitperls_fpix_dist->Divide(coarsedelay_ls);
//     avghitperls_dist->Add(coarsedelay_dist);
//     avghitperls_dist->Divide(coarsedelay_ls);

    coarsedelay_ls->Write();
    avghitperls_l1_dist->Write();
    avghitperls_l2_dist->Write();
    avghitperls_l3_dist->Write();
    avghitperls_dp2_dist->Write();
    avghitperls_dp1_dist->Write();
    avghitperls_dm1_dist->Write();
    avghitperls_dm2_dist->Write();
    avghitperls_bpix_dist->Write();
    avghitperls_fpix_dist->Write();
    avghitperls_dist->Write();

    fo->Close();
  }
  if (fullrun==2) {
    TFile *fo2= new TFile("hv2010+hv2011.root","update");
    
    eff_dist_error_write(hv_bpix_eff,hv_bpix_dist);
    eff_dist_error_write(hv_fpix_eff,hv_fpix_dist);

    fo2->Close();
  }

  return 1;
}
