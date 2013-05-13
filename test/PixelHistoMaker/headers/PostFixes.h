class PostFixes {

 public:
  PostFixes()  { build_(); }
  ~PostFixes() {}

  std::vector<std::string> cumeff;
  std::vector<std::string> federr_type;
  std::vector<std::string> federr;
  std::vector<std::string> il;
  std::vector<std::string> inac;
  std::vector<std::string> effclu;
  std::vector<std::string> effcluch;
  std::vector<std::string> def;
  std::vector<std::string> effmpv;
  std::vector<std::string> mod2;
  std::vector<std::string> det;
  std::vector<std::string> det5;
  std::vector<std::string> Det5;
  std::vector<std::string> fpixIO;
  std::vector<std::string> mod;
  std::vector<std::string> shl;
  std::vector<std::string> sec;
  std::vector<std::string> chn;
  std::vector<std::string> dsk;
  std::vector<std::string> rog;
  std::vector<std::string> Rog;
  std::vector<std::string> module;
  std::vector<std::string> ladder;
  std::vector<std::string> badcol;
  std::vector<std::string> trigtest;
  std::vector<std::string> stat;
  std::vector<std::string> scan;
  std::vector<std::string> range;
  std::vector<std::string> fill;
  std::vector<std::string> filter;
  std::vector<std::string> totlumi;
  std::vector<std::string> Totlumi;
  std::vector<std::string> corr;
  std::vector<std::string> corr2;
  std::vector<std::string> Corr2;
  std::vector<std::string> avg;
  std::vector<std::string> Avg;
  std::vector<std::string> occup;
  std::vector<std::string> spec;
  std::vector<std::string> Spec;
  std::vector<std::string> angle;
  std::vector<std::string> Angle;
  std::vector<std::string> year;
  std::vector<std::string> Year;
  std::vector<std::string> vturnon;
  std::vector<std::string> nstrip;
  std::vector<std::string> valmis;
  std::vector<std::string> Valmis;
  
  std::vector<std::string> dynamic;
  std::vector<std::string> ineff;
  std::vector<std::string> Ineff;
  
  std::vector<std::string> Detshl;
  std::vector<std::string> detshl;
  std::vector<std::string> seclyrldr;
  std::vector<std::string> modroc;
  std::vector<std::string> dskbld;
  std::vector<std::string> pnlplqroc;
  std::vector<std::string> pnlplq;
  std::vector<std::string> mod3;

  std::vector<std::string> Cumeff;
  std::vector<std::string> Federr_type;
  std::vector<std::string> Effclu;
  std::vector<std::string> Mod2;
  std::vector<std::string> Det;
  std::vector<std::string> FpixIO;
  std::vector<std::string> Mod;
  std::vector<std::string> Shl;
  std::vector<std::string> Sec;
  std::vector<std::string> Chn;
  std::vector<std::string> Dsk;
/*   std::vector<std::string> Rog; */


  std::vector<std::string> Fill;
  std::vector<std::string> Fill2;
  std::vector<std::string> Filter;
  std::vector<std::string> Range;

  std::vector<std::string> hv_l1;
  std::vector<std::string> hv_l2;
  std::vector<std::string> hv_l3;
  std::vector<std::string> hv_d1;
  std::vector<std::string> hv_l1full;
  std::vector<std::string> hv_l2full;
  std::vector<std::string> hv_l3full;
  std::vector<std::string> hv_d1full;
  std::vector<std::string> hv_d2full;
  std::vector<std::string> fpix_grp;
  std::vector<std::string> bpix_grp;

  std::vector<std::string> hv;
  std::vector<std::string> hv_full_bpix;
  std::vector<std::string> hv_full_fpix;

  std::vector<std::string> Hv_l1;
  std::vector<std::string> Hv_l2;
  std::vector<std::string> Hv_l3;
  std::vector<std::string> Hv_d1;
  std::vector<std::string> Hv_l1full;
  std::vector<std::string> Hv_l2full;
  std::vector<std::string> Hv_l3full;
  std::vector<std::string> Hv_d1full;
  std::vector<std::string> Hv_d2full;

  std::vector<std::string> Fpix_grp;

 private:
  void build_() {
    // Efficiency
    cumeff.push_back("_eff");
    cumeff.push_back("_dist");
    cumeff.push_back("_cumeff");
    cumeff.push_back("_cumdist");

    federr.push_back("_eff");
    federr.push_back("_dist");
    federr.push_back("_moderrrate");
    federr.push_back("_hitfrac");
    federr.push_back("_nhiterrevt");
    federr.push_back("_errrate");
    federr.push_back("_errmult");
    federr.push_back("_errmultvar");

    federr_type.push_back("");
    federr_type.push_back("_noerr");
    federr_type.push_back("_allerr");
    federr_type.push_back("_rocof25");
    federr_type.push_back("_gapword");
    federr_type.push_back("_dummyword");
    federr_type.push_back("_fifofull");
    federr_type.push_back("_timeout");
    federr_type.push_back("_tbmtrailer");
    federr_type.push_back("_eventnumber");
    federr_type.push_back("_slinkheader");
    federr_type.push_back("_slinktrailer");
    federr_type.push_back("_eventsize");
    federr_type.push_back("_invchanno");
    federr_type.push_back("_rocvalue");
    federr_type.push_back("_dcol");
    federr_type.push_back("_readotorder");
    federr_type.push_back("_crcerror");
    federr_type.push_back("_overflow");
    federr_type.push_back("_errevt");
    federr_type.push_back("_noerrevt");

    Federr_type.push_back("All Hits");
    Federr_type.push_back("NO Errors");
    Federr_type.push_back("All FED Errors");
    Federr_type.push_back("ROC of 25");
    Federr_type.push_back("Gap word");
    Federr_type.push_back("Dummy word");
    Federr_type.push_back("FIFO full");
    Federr_type.push_back("Timeout");
    Federr_type.push_back("TBM trailer");
    Federr_type.push_back("Event number");
    Federr_type.push_back("Slink header");
    Federr_type.push_back("Slink trailer");
    Federr_type.push_back("Event size");
    Federr_type.push_back("Invalid channel#");
    Federr_type.push_back("ROC value");
    Federr_type.push_back("Dcol or pixel value");
    Federr_type.push_back("Readout order");
    Federr_type.push_back("CRC error");
    Federr_type.push_back("Overflow");

    il.push_back("_il");
    il.push_back("_nls");

    Cumeff.push_back("Efficiency");
    Cumeff.push_back("Hit Distribution");
    Cumeff.push_back("Cumulative Efficiency");
    Cumeff.push_back("Cumulative Hit Distribution");

    inac.push_back("_eff");
    inac.push_back("_dist");
    inac.push_back("_inac");
    inac.push_back("_excl");

    // Avg cluster size
    effclu.push_back("_eff");
    effclu.push_back("_dist");
    effclu.push_back("_onclusize");
    effclu.push_back("_onnclu");
    effclu.push_back("_mpv");
    effclu.push_back("_nccd");

    effcluch.push_back("_eff");
    effcluch.push_back("_dist");
    effcluch.push_back("_oncluch");
    effcluch.push_back("_onnclu");

    effmpv.push_back("_eff");
    effmpv.push_back("_dist");
    effmpv.push_back("_nccdmpv");
    effmpv.push_back("_pcdmpv");
    effmpv.push_back("_avgsize");
    effmpv.push_back("_nclu");
    effmpv.push_back("_avgsize_leta");
    effmpv.push_back("_nclu_leta");
    effmpv.push_back("_avgsize_heta");
    effmpv.push_back("_nclu_heta");

    def.push_back("_eff");
    def.push_back("_dist");
    def.push_back("_nclu");
    def.push_back("_avgch");
    def.push_back("_avgnormch");
    def.push_back("_mpv");
    def.push_back("_alpha");
    def.push_back("_beta");
    def.push_back("_aoi");
    def.push_back("_avgsize");
    def.push_back("_ncluperevt");
    def.push_back("_npixperevt");

    mod2.push_back("");
    mod2.push_back("_mod1");
    mod2.push_back("_mod2");
    mod2.push_back("_mod3");
    mod2.push_back("_mod4");

    Mod2.push_back("All Module");
    Mod2.push_back("Module 1");
    Mod2.push_back("Module 2");
    Mod2.push_back("Module 3");
    Mod2.push_back("Module 4");

    Effclu.push_back("Efficiency");
    Effclu.push_back("Hit Distribution");
    Effclu.push_back("Average On-Track Cluster Size");
    Effclu.push_back("Number of On-Track Clusters");
    Effclu.push_back("Average Cluster Size");
    Effclu.push_back("Number of Clusters");

    // Detector parts
    det.push_back("");
    det.push_back("_bpix");
    det.push_back("_fpix");
    det.push_back("_l1");
    det.push_back("_l2");
    det.push_back("_l3");
    det.push_back("_dm2");
    det.push_back("_dm1");
    det.push_back("_dp1");
    det.push_back("_dp2");
    det.push_back("_d1");
    det.push_back("_d2");
    det.push_back("_d1_in");
    det.push_back("_d1_out");
    det.push_back("_d2_in");
    det.push_back("_d2_out");
    det.push_back("_dm2_in");
    det.push_back("_dm2_out");
    det.push_back("_dm1_in");
    det.push_back("_dm1_out");
    det.push_back("_dp1_in");
    det.push_back("_dp1_out");
    det.push_back("_dp2_in");
    det.push_back("_dp2_out");

    Det.push_back("Pixel Detector");
    Det.push_back("Barrel Pixel");
    Det.push_back("Forward Pixel");
    Det.push_back("Layer 1");
    Det.push_back("Layer 2");
    Det.push_back("Layer 3");
    Det.push_back("Disk -2");
    Det.push_back("Disk -1");
    Det.push_back("Disk +1");
    Det.push_back("Disk +2");
    Det.push_back("Disk 1");
    Det.push_back("Disk 2");
    Det.push_back("Disk 1 Inner");
    Det.push_back("Disk 1 Outer");
    Det.push_back("Disk 2 Inner");
    Det.push_back("Disk 2 Outer");
    Det.push_back("Disk -2 Inner");
    Det.push_back("Disk -2 Outer");
    Det.push_back("Disk -1 Inner");
    Det.push_back("Disk -1 Outer");
    Det.push_back("Disk +1 Inner");
    Det.push_back("Disk +1 Outer");
    Det.push_back("Disk +2 Inner");
    Det.push_back("Disk +2 Outer");

    det5.push_back("_l1");
    det5.push_back("_l2");
    det5.push_back("_l3");
    det5.push_back("_d1");
    det5.push_back("_d2");

    Det5.push_back("Layer 1");
    Det5.push_back("Layer 2");
    Det5.push_back("Layer 3");
    Det5.push_back("Disk 1");
    Det5.push_back("Disk 2");

    fpixIO.push_back("_fpixI");
    fpixIO.push_back("_fpixO");

    FpixIO.push_back("FPix (+X)");
    FpixIO.push_back("FPix (-X)");

    // Module types
    mod.push_back("_full");
    mod.push_back("_half");
    mod.push_back("_p1_m1");
    mod.push_back("_p1_m2");
    mod.push_back("_p1_m3");
    mod.push_back("_p1_m4");
    mod.push_back("_p2_m1");
    mod.push_back("_p2_m2");
    mod.push_back("_p2_m3");

    // Mod.push_back("Full");
    // Mod.push_back("Half");
    // Mod.push_back("PNL1 PLQ1");
    // Mod.push_back("PNL1 PLQ2");
    // Mod.push_back("PNL1 PLQ3");
    // Mod.push_back("PNL1 PLQ4");
    // Mod.push_back("PNL2 PLQ1");
    // Mod.push_back("PNL2 PLQ2");
    // Mod.push_back("PNL2 PLQ3");

    Mod.push_back("Full Modules (2x8)");
    Mod.push_back("Half Modules (1x8)");
    Mod.push_back("Panel 1 Plaquette 1 (1x2)");
    Mod.push_back("Panel 1 Plaquette 2 (2x3)");
    Mod.push_back("Panel 1 Plaquette 3 (2x4)");
    Mod.push_back("Panel 1 Plaquette 4 (1x5)");
    Mod.push_back("Panel 2 Plaquette 1 (2x3)");
    Mod.push_back("Panel 2 Plaquette 2 (2x4)");
    Mod.push_back("Panel 2 Plaquette 3 (2x5)");

    // Readout groups
    shl.push_back("_BpI"); // shl 0
    shl.push_back("_BpO"); // shl 1
    shl.push_back("_BmI"); // shl 2
    shl.push_back("_BmO"); // shl 3

    Shl.push_back("BpI");
    Shl.push_back("BpO");
    Shl.push_back("BmI");
    Shl.push_back("BmO");

    sec.push_back("_Sec1");
    sec.push_back("_Sec2");
    sec.push_back("_Sec3");
    sec.push_back("_Sec4");
    sec.push_back("_Sec5");
    sec.push_back("_Sec6");
    sec.push_back("_Sec7");
    sec.push_back("_Sec8");

    Sec.push_back("SEC1");
    Sec.push_back("SEC2");
    Sec.push_back("SEC3");
    Sec.push_back("SEC4");
    Sec.push_back("SEC5");
    Sec.push_back("SEC6");
    Sec.push_back("SEC7");
    Sec.push_back("SEC8");

    chn.push_back("_Lyr12");
    chn.push_back("_Lyr3");

    Chn.push_back("Layer1-2");
    Chn.push_back("Layer3");

    dsk.push_back("_D1");
    dsk.push_back("_D2");

    Dsk.push_back("Disk1");
    Dsk.push_back("Disk2");

    rog.push_back("_Rog1");
    rog.push_back("_Rog2");
    rog.push_back("_Rog3");
    rog.push_back("_Rog4");

    Rog.push_back("ROG1");
    Rog.push_back("ROG2");
    Rog.push_back("ROG3");
    Rog.push_back("ROG4");

    module.push_back("_modm4");
    module.push_back("_modm3");
    module.push_back("_modm2");
    module.push_back("_modm1");
    module.push_back("_modp1");
    module.push_back("_modp2");
    module.push_back("_modp3");
    module.push_back("_modp4");

    ladder.push_back("_ladm22");
    ladder.push_back("_ladm21");
    ladder.push_back("_ladm20");
    ladder.push_back("_ladm19");
    ladder.push_back("_ladm18");
    ladder.push_back("_ladm17");
    ladder.push_back("_ladm16");
    ladder.push_back("_ladm15");
    ladder.push_back("_ladm14");
    ladder.push_back("_ladm13");
    ladder.push_back("_ladm12");
    ladder.push_back("_ladm11");
    ladder.push_back("_ladm10");
    ladder.push_back("_ladm9");
    ladder.push_back("_ladm8");
    ladder.push_back("_ladm7");
    ladder.push_back("_ladm6");
    ladder.push_back("_ladm5");
    ladder.push_back("_ladm4");
    ladder.push_back("_ladm3");
    ladder.push_back("_ladm2");
    ladder.push_back("_ladm1");
    ladder.push_back("_ladp1");
    ladder.push_back("_ladp2");
    ladder.push_back("_ladp3");
    ladder.push_back("_ladp4");
    ladder.push_back("_ladp5");
    ladder.push_back("_ladp6");
    ladder.push_back("_ladp7");
    ladder.push_back("_ladp8");
    ladder.push_back("_ladp9");
    ladder.push_back("_ladp10");
    ladder.push_back("_ladp11");
    ladder.push_back("_ladp12");
    ladder.push_back("_ladp13");
    ladder.push_back("_ladp14");
    ladder.push_back("_ladp15");
    ladder.push_back("_ladp16");
    ladder.push_back("_ladp17");
    ladder.push_back("_ladp18");
    ladder.push_back("_ladp19");
    ladder.push_back("_ladp20");
    ladder.push_back("_ladp21");
    ladder.push_back("_ladp22");
    
    badcol.push_back("_BpI_l1_ldr6_mod1");
    badcol.push_back("_BpO_l1_ldr2_mod1");
    badcol.push_back("_BpO_l1_ldr4_mod2");
    badcol.push_back("_BmI_l1_ldr4_mod2");
    badcol.push_back("_BmI_l1_ldr5_mod3");
    badcol.push_back("_BmO_l1_ldr1_mod1");
    badcol.push_back("_BpI_l2_ldr12_mod3");
    badcol.push_back("_BpI_l2_ldr14_mod1");
    badcol.push_back("_BpI_l2_ldr16_mod1");
    badcol.push_back("_BpO_l2_ldr2_mod1");
    badcol.push_back("_BpO_l2_ldr3_mod3");
    badcol.push_back("_BpO_l2_ldr5_mod1");
    badcol.push_back("_BmI_l2_ldr7_mod1");
    badcol.push_back("_BmO_l2_ldr13_mod2");
    badcol.push_back("_BmO_l2_ldr15_mod2");
    badcol.push_back("_BmO_l2_ldr16_mod1");
    badcol.push_back("_BpO_l3_ldr3_mod2");
    badcol.push_back("_BpO_l3_ldr6_mod2");
    badcol.push_back("_BpO_l3_ldr13_mod1");
    badcol.push_back("_BpO_l3_ldr19_mod3");
    badcol.push_back("_BpI_l3_ldr19_mod1");
    badcol.push_back("_BmO_l3_ldr5_mod1");
    badcol.push_back("_BmO_l3_ldr6_mod1");
    badcol.push_back("_BmO_l3_ldr8_mod1");

    stat.push_back("_nhit");
    stat.push_back("_nclu");

    scan.push_back("_10apr");
    scan.push_back("_10oct");
    scan.push_back("_11mar");
    scan.push_back("_11may");
    scan.push_back("_11jul_l1");
    scan.push_back("_11jul_l3");
    scan.push_back("_11aug_l2");
    scan.push_back("_11sep");
    scan.push_back("_11oct12");
    scan.push_back("_11oct27");
    scan.push_back("_11oct30");
    scan.push_back("_12apr");

    Detshl.push_back("BPix_BpI");
    Detshl.push_back("BPix_BpO");
    Detshl.push_back("BPix_BmI");
    Detshl.push_back("BPix_BmO");
    Detshl.push_back("FPix_BpI");
    Detshl.push_back("FPix_BpO");
    Detshl.push_back("FPix_BmI");
    Detshl.push_back("FPix_BmO");

    detshl.push_back("_BPix_BpI");
    detshl.push_back("_BPix_BpO");
    detshl.push_back("_BPix_BmI");
    detshl.push_back("_BPix_BmO");
    detshl.push_back("_FPix_BpI");
    detshl.push_back("_FPix_BpO");
    detshl.push_back("_FPix_BmI");
    detshl.push_back("_FPix_BmO");
    
    seclyrldr.push_back("_SEC1_LYR1_LDR1H");
    seclyrldr.push_back("_SEC1_LYR1_LDR2F");
    seclyrldr.push_back("_SEC2_LYR1_LDR3F");
    seclyrldr.push_back("_SEC3_LYR1_LDR4F");
    seclyrldr.push_back("_SEC4_LYR1_LDR5F");
    seclyrldr.push_back("_SEC5_LYR1_LDR6F");
    seclyrldr.push_back("_SEC6_LYR1_LDR7F");
    seclyrldr.push_back("_SEC7_LYR1_LDR8F");
    seclyrldr.push_back("_SEC8_LYR1_LDR9F");
    seclyrldr.push_back("_SEC8_LYR1_LDR10H");
    seclyrldr.push_back("_SEC1_LYR2_LDR1H");
    seclyrldr.push_back("_SEC1_LYR2_LDR2F");
    seclyrldr.push_back("_SEC2_LYR2_LDR3F");
    seclyrldr.push_back("_SEC2_LYR2_LDR4F");
    seclyrldr.push_back("_SEC3_LYR2_LDR5F");
    seclyrldr.push_back("_SEC3_LYR2_LDR6F");
    seclyrldr.push_back("_SEC4_LYR2_LDR7F");
    seclyrldr.push_back("_SEC4_LYR2_LDR8F");
    seclyrldr.push_back("_SEC5_LYR2_LDR9F");
    seclyrldr.push_back("_SEC5_LYR2_LDR10F");
    seclyrldr.push_back("_SEC6_LYR2_LDR11F");
    seclyrldr.push_back("_SEC6_LYR2_LDR12F");
    seclyrldr.push_back("_SEC7_LYR2_LDR13F");
    seclyrldr.push_back("_SEC7_LYR2_LDR14F");
    seclyrldr.push_back("_SEC8_LYR2_LDR15F");
    seclyrldr.push_back("_SEC8_LYR2_LDR16H");
    seclyrldr.push_back("_SEC1_LYR3_LDR1H");
    seclyrldr.push_back("_SEC1_LYR3_LDR2F");
    seclyrldr.push_back("_SEC1_LYR3_LDR3F");
    seclyrldr.push_back("_SEC2_LYR3_LDR4F");
    seclyrldr.push_back("_SEC2_LYR3_LDR5F");
    seclyrldr.push_back("_SEC2_LYR3_LDR6F");
    seclyrldr.push_back("_SEC3_LYR3_LDR7F");
    seclyrldr.push_back("_SEC3_LYR3_LDR8F");
    seclyrldr.push_back("_SEC3_LYR3_LDR9F");
    seclyrldr.push_back("_SEC4_LYR3_LDR10F");
    seclyrldr.push_back("_SEC4_LYR3_LDR11F");
    seclyrldr.push_back("_SEC5_LYR3_LDR12F");
    seclyrldr.push_back("_SEC5_LYR3_LDR13F");
    seclyrldr.push_back("_SEC6_LYR3_LDR14F");
    seclyrldr.push_back("_SEC6_LYR3_LDR15F");
    seclyrldr.push_back("_SEC6_LYR3_LDR16F");
    seclyrldr.push_back("_SEC7_LYR3_LDR17F");
    seclyrldr.push_back("_SEC7_LYR3_LDR18F");
    seclyrldr.push_back("_SEC7_LYR3_LDR19F");
    seclyrldr.push_back("_SEC8_LYR3_LDR20F");
    seclyrldr.push_back("_SEC8_LYR3_LDR21F");
    seclyrldr.push_back("_SEC8_LYR3_LDR22H");

    modroc.push_back("_MOD1_ROC0");
    modroc.push_back("_MOD1_ROC1");
    modroc.push_back("_MOD1_ROC2");
    modroc.push_back("_MOD1_ROC3");
    modroc.push_back("_MOD1_ROC4");
    modroc.push_back("_MOD1_ROC5");
    modroc.push_back("_MOD1_ROC6");
    modroc.push_back("_MOD1_ROC7");
    modroc.push_back("_MOD1_ROC8");
    modroc.push_back("_MOD1_ROC9");
    modroc.push_back("_MOD1_ROC10");
    modroc.push_back("_MOD1_ROC11");
    modroc.push_back("_MOD1_ROC12");
    modroc.push_back("_MOD1_ROC13");
    modroc.push_back("_MOD1_ROC14");
    modroc.push_back("_MOD1_ROC15");
    modroc.push_back("_MOD2_ROC0");
    modroc.push_back("_MOD2_ROC1");
    modroc.push_back("_MOD2_ROC2");
    modroc.push_back("_MOD2_ROC3");
    modroc.push_back("_MOD2_ROC4");
    modroc.push_back("_MOD2_ROC5");
    modroc.push_back("_MOD2_ROC6");
    modroc.push_back("_MOD2_ROC7");
    modroc.push_back("_MOD2_ROC8");
    modroc.push_back("_MOD2_ROC9");
    modroc.push_back("_MOD2_ROC10");
    modroc.push_back("_MOD2_ROC11");
    modroc.push_back("_MOD2_ROC12");
    modroc.push_back("_MOD2_ROC13");
    modroc.push_back("_MOD2_ROC14");
    modroc.push_back("_MOD2_ROC15");
    modroc.push_back("_MOD3_ROC0");
    modroc.push_back("_MOD3_ROC1");
    modroc.push_back("_MOD3_ROC2");
    modroc.push_back("_MOD3_ROC3");
    modroc.push_back("_MOD3_ROC4");
    modroc.push_back("_MOD3_ROC5");
    modroc.push_back("_MOD3_ROC6");
    modroc.push_back("_MOD3_ROC7");
    modroc.push_back("_MOD3_ROC8");
    modroc.push_back("_MOD3_ROC9");
    modroc.push_back("_MOD3_ROC10");
    modroc.push_back("_MOD3_ROC11");
    modroc.push_back("_MOD3_ROC12");
    modroc.push_back("_MOD3_ROC13");
    modroc.push_back("_MOD3_ROC14");
    modroc.push_back("_MOD3_ROC15");
    modroc.push_back("_MOD4_ROC0");
    modroc.push_back("_MOD4_ROC1");
    modroc.push_back("_MOD4_ROC2");
    modroc.push_back("_MOD4_ROC3");
    modroc.push_back("_MOD4_ROC4");
    modroc.push_back("_MOD4_ROC5");
    modroc.push_back("_MOD4_ROC6");
    modroc.push_back("_MOD4_ROC7");
    modroc.push_back("_MOD4_ROC8");
    modroc.push_back("_MOD4_ROC9");
    modroc.push_back("_MOD4_ROC10");
    modroc.push_back("_MOD4_ROC11");
    modroc.push_back("_MOD4_ROC12");
    modroc.push_back("_MOD4_ROC13");
    modroc.push_back("_MOD4_ROC14");
    modroc.push_back("_MOD4_ROC15");

    dskbld.push_back("_D1_BLD1");
    dskbld.push_back("_D1_BLD2");
    dskbld.push_back("_D1_BLD3");
    dskbld.push_back("_D1_BLD4");
    dskbld.push_back("_D1_BLD5");
    dskbld.push_back("_D1_BLD6");
    dskbld.push_back("_D1_BLD7");
    dskbld.push_back("_D1_BLD8");
    dskbld.push_back("_D1_BLD9");
    dskbld.push_back("_D1_BLD10");
    dskbld.push_back("_D1_BLD11");
    dskbld.push_back("_D1_BLD12");
    dskbld.push_back("_D2_BLD1");
    dskbld.push_back("_D2_BLD2");
    dskbld.push_back("_D2_BLD3");
    dskbld.push_back("_D2_BLD4");
    dskbld.push_back("_D2_BLD5");
    dskbld.push_back("_D2_BLD6");
    dskbld.push_back("_D2_BLD7");
    dskbld.push_back("_D2_BLD8");
    dskbld.push_back("_D2_BLD9");
    dskbld.push_back("_D2_BLD10");
    dskbld.push_back("_D2_BLD11");
    dskbld.push_back("_D2_BLD12");

    pnlplqroc.push_back("_PNL1_PLQ1_ROC0");
    pnlplqroc.push_back("_PNL1_PLQ1_ROC1");
    pnlplqroc.push_back("_PNL1_PLQ2_ROC0");
    pnlplqroc.push_back("_PNL1_PLQ2_ROC1");
    pnlplqroc.push_back("_PNL1_PLQ2_ROC2");
    pnlplqroc.push_back("_PNL1_PLQ2_ROC3");
    pnlplqroc.push_back("_PNL1_PLQ2_ROC4");
    pnlplqroc.push_back("_PNL1_PLQ2_ROC5");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC0");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC1");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC2");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC3");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC4");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC5");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC6");
    pnlplqroc.push_back("_PNL1_PLQ3_ROC7");
    pnlplqroc.push_back("_PNL1_PLQ4_ROC0");
    pnlplqroc.push_back("_PNL1_PLQ4_ROC1");
    pnlplqroc.push_back("_PNL1_PLQ4_ROC2");
    pnlplqroc.push_back("_PNL1_PLQ4_ROC3");
    pnlplqroc.push_back("_PNL1_PLQ4_ROC4");
    pnlplqroc.push_back("_PNL2_PLQ1_ROC0");
    pnlplqroc.push_back("_PNL2_PLQ1_ROC1");
    pnlplqroc.push_back("_PNL2_PLQ1_ROC2");
    pnlplqroc.push_back("_PNL2_PLQ1_ROC3");
    pnlplqroc.push_back("_PNL2_PLQ1_ROC4");
    pnlplqroc.push_back("_PNL2_PLQ1_ROC5");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC0");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC1");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC2");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC3");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC4");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC5");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC6");
    pnlplqroc.push_back("_PNL2_PLQ2_ROC7");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC0");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC1");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC2");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC3");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC4");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC5");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC6");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC7");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC8");
    pnlplqroc.push_back("_PNL2_PLQ3_ROC9");

    pnlplq.push_back("_PNL1_PLQ1");
    pnlplq.push_back("_PNL1_PLQ2");
    pnlplq.push_back("_PNL1_PLQ3");
    pnlplq.push_back("_PNL1_PLQ4");
    pnlplq.push_back("_PNL2_PLQ1");
    pnlplq.push_back("_PNL2_PLQ2");
    pnlplq.push_back("_PNL2_PLQ3");

    mod3.push_back("_MOD1");
    mod3.push_back("_MOD2");
    mod3.push_back("_MOD3");
    mod3.push_back("_MOD4");

    range.push_back("");
    range.push_back("_1");
    range.push_back("_2");
    range.push_back("_3");
    range.push_back("_4");

    Range.push_back("");
    Range.push_back(" 0:00- 5:54 - 2.1-1.5 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back(" 5:54-11:48 - 1.5-1.1 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back("11:48-17:37 - 1.1-0.8 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back("0:00-2:42 - 3.1-2.6 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back("2:42-5:24 - 2.6-2.2 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back("5:24-8:11 - 2.2-1.9 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back(" 0:00- 5:30 - 3.2-2.3 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back(" 3:30-11:00 - 2.3-1.7 #times10^{33}cm^{-2}s^{-1}");
    Range.push_back("11:00-16:35 - 1.7-1.3 #times10^{33}cm^{-2}s^{-1}");
    
    fill.push_back("");
    Fill.push_back("");
    Fill2.push_back("");
    // Fill.push_back("Fill 2040");
    // Fill.push_back("Fill 2103");
    // Fill.push_back("Fill 2105");

    filter.push_back("_nofilter");
    filter.push_back("_noscrap");
    filter.push_back("_oneGV");
    filter.push_back("_trkFF");
    filter.push_back("_allfilter");

    Filter.push_back("No Filters");
    Filter.push_back("noscraping");
    Filter.push_back("oneGoodVertex");
    Filter.push_back("trackingFailureFilter");
    Filter.push_back("All Filters");

    totlumi.push_back("_1");
    totlumi.push_back("_2");
    totlumi.push_back("_3");
    totlumi.push_back("_4");
    totlumi.push_back("_5");

    corr.push_back("_l2_l1");
    corr.push_back("_l3_l1");

    corr2.push_back("_l2_l1_nclucorr");
    corr2.push_back("_l3_l1_nclucorr");
    corr2.push_back("_l2_l1_npixcorr");
    corr2.push_back("_l3_l1_npixcorr");

    Corr2.push_back("Nclu - L2/L1");
    Corr2.push_back("Nclu - L3/L1");
    Corr2.push_back("Npix - L2/L1");
    Corr2.push_back("Npix - L3/L1");

    avg.push_back("_ratio");
    avg.push_back("_bxavg");
    avg.push_back("_avg");
    avg.push_back("_nevt");

    occup.push_back("_npix");
    occup.push_back("_nclu");

    spec.push_back("_Fill2807");
    spec.push_back("_Fill2816");

    Spec.push_back("After +12 BX Latency Change ");
    Spec.push_back("Default settings");

    angle.push_back("_alpha");
    angle.push_back("_beta");
    angle.push_back("_incidence");
    angle.push_back("_theta");
    angle.push_back("_phi");
    angle.push_back("_theta_phi");

    Angle.push_back("Alpha [#circ]");
    Angle.push_back("Beta [#circ]");
    Angle.push_back("Angle of Incidence [#circ]");
    Angle.push_back("Trajectory #theta");
    Angle.push_back("Trajectory #phi");

    Avg.push_back("Ratio");
    Avg.push_back("BX Average /[100 LS]");
    Avg.push_back("Average /[1 LS]");
    Avg.push_back("Number of Events");

    Totlumi.push_back("0-1 fb^{-1}");
    Totlumi.push_back("5-6 fb^{-1}");
    Totlumi.push_back("10-11 fb^{-1}");
    Totlumi.push_back("15-16 fb^{-1}");
    Totlumi.push_back("22-23 fb^{-1}");

    year.push_back("");
    year.push_back("_year2010");
    year.push_back("_year2011");
    year.push_back("_year2012");

    Year.push_back("");
    Year.push_back("2010");
    Year.push_back("2011");
    Year.push_back("2012");
    
    vturnon.push_back("");
    vturnon.push_back("_mpv");
    vturnon.push_back("_cce");

    nstrip.push_back("_old");
    nstrip.push_back("_nlost0");
    nstrip.push_back("_nlayover5");
    nstrip.push_back("_nlayover7");
    nstrip.push_back("_nlayover9");
    nstrip.push_back("_nlayover7_nlost0");

    valmis.push_back("");
    valmis.push_back("_val");
    valmis.push_back("_mis");

    Valmis.push_back("All hits");
    Valmis.push_back("Valid Hits");
    Valmis.push_back("Missing Hits");

    dynamic.push_back("_ladder");
    dynamic.push_back("_module");
    dynamic.push_back("_l1rate");
    dynamic.push_back("_instlumi");
    
    ineff.push_back("_Data");
    ineff.push_back("_MC");
    ineff.push_back("_MC_corr");

    Ineff.push_back("Data");
    Ineff.push_back("MC");
    Ineff.push_back("MC Corr (Lad, Mod, Inst)");
    
    hv.push_back("_eff");
    hv.push_back("_dist");
    hv.push_back("_mpv");
    hv.push_back("_nccd");

    hv_full_bpix.push_back("");
    hv_full_bpix.push_back("_BpI_SEC1");
    hv_full_bpix.push_back("_BpI_SEC2");
    hv_full_bpix.push_back("_BpI_SEC3");
    hv_full_bpix.push_back("_BpI_SEC4");
    hv_full_bpix.push_back("_BpI_SEC5");
    hv_full_bpix.push_back("_BpI_SEC6");
    hv_full_bpix.push_back("_BpI_SEC7");
    hv_full_bpix.push_back("_BpI_SEC8");
    hv_full_bpix.push_back("_BpO_SEC1");
    hv_full_bpix.push_back("_BpO_SEC2");
    hv_full_bpix.push_back("_BpO_SEC3");
    hv_full_bpix.push_back("_BpO_SEC4");
    hv_full_bpix.push_back("_BpO_SEC5");
    hv_full_bpix.push_back("_BpO_SEC6");
    hv_full_bpix.push_back("_BpO_SEC7");
    hv_full_bpix.push_back("_BpO_SEC8");
    hv_full_bpix.push_back("_BmI_SEC1");
    hv_full_bpix.push_back("_BmI_SEC2");
    hv_full_bpix.push_back("_BmI_SEC3");
    hv_full_bpix.push_back("_BmI_SEC4");
    hv_full_bpix.push_back("_BmI_SEC5");
    hv_full_bpix.push_back("_BmI_SEC6");
    hv_full_bpix.push_back("_BmI_SEC7");
    hv_full_bpix.push_back("_BmI_SEC8");
    hv_full_bpix.push_back("_BmO_SEC1");
    hv_full_bpix.push_back("_BmO_SEC2");
    hv_full_bpix.push_back("_BmO_SEC3");
    hv_full_bpix.push_back("_BmO_SEC4");
    hv_full_bpix.push_back("_BmO_SEC5");
    hv_full_bpix.push_back("_BmO_SEC6");
    hv_full_bpix.push_back("_BmO_SEC7");
    hv_full_bpix.push_back("_BmO_SEC8");
    hv_full_bpix.push_back("_MODM4");
    hv_full_bpix.push_back("_MODM3");
    hv_full_bpix.push_back("_MODM2");
    hv_full_bpix.push_back("_MODM1");
    hv_full_bpix.push_back("_MODP1");
    hv_full_bpix.push_back("_MODP2");
    hv_full_bpix.push_back("_MODP3");
    hv_full_bpix.push_back("_MODP4");
    hv_full_bpix.push_back("_LDRM1");
    hv_full_bpix.push_back("_LDRP1");
    hv_full_bpix.push_back("_LDRM2");
    hv_full_bpix.push_back("_LDRP2");
    hv_full_bpix.push_back("_LDRM3");
    hv_full_bpix.push_back("_LDRP3");
    hv_full_bpix.push_back("_LDRM4");
    hv_full_bpix.push_back("_LDRP4");
    hv_full_bpix.push_back("_LDRM5");
    hv_full_bpix.push_back("_LDRP5");
    hv_full_bpix.push_back("_LDRM6");
    hv_full_bpix.push_back("_LDRP6");
    hv_full_bpix.push_back("_LDRM7");
    hv_full_bpix.push_back("_LDRP7");
    hv_full_bpix.push_back("_LDRM8");
    hv_full_bpix.push_back("_LDRP8");
    hv_full_bpix.push_back("_LDRM9");
    hv_full_bpix.push_back("_LDRP9");
    hv_full_bpix.push_back("_LDRM10");
    hv_full_bpix.push_back("_LDRP10");
    hv_full_bpix.push_back("_LDRM11");
    hv_full_bpix.push_back("_LDRP11");
    hv_full_bpix.push_back("_LDRM12");
    hv_full_bpix.push_back("_LDRP12");
    hv_full_bpix.push_back("_LDRM13");
    hv_full_bpix.push_back("_LDRP13");
    hv_full_bpix.push_back("_LDRM14");
    hv_full_bpix.push_back("_LDRP14");
    hv_full_bpix.push_back("_LDRM15");
    hv_full_bpix.push_back("_LDRP15");
    hv_full_bpix.push_back("_LDRM16");
    hv_full_bpix.push_back("_LDRP16");
    hv_full_bpix.push_back("_LDRM17");
    hv_full_bpix.push_back("_LDRP17");
    hv_full_bpix.push_back("_LDRM18");
    hv_full_bpix.push_back("_LDRP18");
    hv_full_bpix.push_back("_LDRM19");
    hv_full_bpix.push_back("_LDRP19");
    hv_full_bpix.push_back("_LDRM20");
    hv_full_bpix.push_back("_LDRP20");
    hv_full_bpix.push_back("_LDRM21");
    hv_full_bpix.push_back("_LDRP21");
    hv_full_bpix.push_back("_LDRM22");
    hv_full_bpix.push_back("_LDRP22");

    hv_full_fpix.push_back("");
    hv_full_fpix.push_back("_BpI_HV1I");
    hv_full_fpix.push_back("_BpI_HV1O");
    hv_full_fpix.push_back("_BpI_HV2I");
    hv_full_fpix.push_back("_BpI_HV2O");
    hv_full_fpix.push_back("_BpI_HV3I");
    hv_full_fpix.push_back("_BpI_HV3O");
    hv_full_fpix.push_back("_BpI_HV4I");
    hv_full_fpix.push_back("_BpI_HV4O");
    hv_full_fpix.push_back("_BpO_HV1I");
    hv_full_fpix.push_back("_BpO_HV1O");
    hv_full_fpix.push_back("_BpO_HV2I");
    hv_full_fpix.push_back("_BpO_HV2O");
    hv_full_fpix.push_back("_BpO_HV3I");
    hv_full_fpix.push_back("_BpO_HV3O");
    hv_full_fpix.push_back("_BpO_HV4I");
    hv_full_fpix.push_back("_BpO_HV4O");
    hv_full_fpix.push_back("_BmI_HV1I");
    hv_full_fpix.push_back("_BmI_HV1O");
    hv_full_fpix.push_back("_BmI_HV2I");
    hv_full_fpix.push_back("_BmI_HV2O");
    hv_full_fpix.push_back("_BmI_HV3I");
    hv_full_fpix.push_back("_BmI_HV3O");
    hv_full_fpix.push_back("_BmI_HV4I");
    hv_full_fpix.push_back("_BmI_HV4O");
    hv_full_fpix.push_back("_BmO_HV1I");
    hv_full_fpix.push_back("_BmO_HV1O");
    hv_full_fpix.push_back("_BmO_HV2I");
    hv_full_fpix.push_back("_BmO_HV2O");
    hv_full_fpix.push_back("_BmO_HV3I");
    hv_full_fpix.push_back("_BmO_HV3O");
    hv_full_fpix.push_back("_BmO_HV4I");
    hv_full_fpix.push_back("_BmO_HV4O");
    hv_full_fpix.push_back("_PNL1_PLQ1");
    hv_full_fpix.push_back("_PNL2_PLQ1");
    hv_full_fpix.push_back("_PNL1_PLQ2");
    hv_full_fpix.push_back("_PNL2_PLQ2");
    hv_full_fpix.push_back("_PNL1_PLQ3");
    hv_full_fpix.push_back("_PNL2_PLQ3");
    hv_full_fpix.push_back("_PNL1_PLQ4");
    hv_full_fpix.push_back("_BLDM12");
    hv_full_fpix.push_back("_BLDM11");
    hv_full_fpix.push_back("_BLDM10");
    hv_full_fpix.push_back("_BLDM9");
    hv_full_fpix.push_back("_BLDM8");
    hv_full_fpix.push_back("_BLDM7");
    hv_full_fpix.push_back("_BLDM6");
    hv_full_fpix.push_back("_BLDM5");
    hv_full_fpix.push_back("_BLDM4");
    hv_full_fpix.push_back("_BLDM3");
    hv_full_fpix.push_back("_BLDM2");
    hv_full_fpix.push_back("_BLDM1");
    hv_full_fpix.push_back("_BLDP1");
    hv_full_fpix.push_back("_BLDP2");
    hv_full_fpix.push_back("_BLDP3");
    hv_full_fpix.push_back("_BLDP4");
    hv_full_fpix.push_back("_BLDP5");
    hv_full_fpix.push_back("_BLDP6");
    hv_full_fpix.push_back("_BLDP7");
    hv_full_fpix.push_back("_BLDP8");
    hv_full_fpix.push_back("_BLDP9");
    hv_full_fpix.push_back("_BLDP10");
    hv_full_fpix.push_back("_BLDP11");
    hv_full_fpix.push_back("_BLDP12");
    
    fpix_grp.push_back("_D1I");
    fpix_grp.push_back("_D1O");
    fpix_grp.push_back("_D2I");
    fpix_grp.push_back("_D2O");
    fpix_grp.push_back("_D1plq");
    fpix_grp.push_back("_D2plq");

    bpix_grp.push_back("_SEC");
    bpix_grp.push_back("_RING");


    Fpix_grp.push_back("Disk 1 - Inner Radius");
    Fpix_grp.push_back("Disk 1 - Outer Radius");
    Fpix_grp.push_back("Disk 2 - Inner Radius");
    Fpix_grp.push_back("Disk 2 - Outer Radius");
    Fpix_grp.push_back("Disk 1");
    Fpix_grp.push_back("Disk 2");
  }

};
