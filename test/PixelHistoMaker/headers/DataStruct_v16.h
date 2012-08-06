#include "TObject.h"
#include "TFile.h"
#include "TROOT.h"
#include "TTree.h"
#include "TH2.h"
#include <memory>
#include <cstdlib>
#include <string>
#include <sstream>

#define NOVAL_I -9999
#define NOVAL_F -9999.0
#define VERSION2 16

//Defining Dataset Structures

// Event info
class EventData {
 public:
  int orb;
  int ls;
  int nvtx;
  int run;
  float vtxndof;
#ifdef COMPLETE
  float vtxchi2;
  float vtxD0;
  float vtxX;
  float vtxY;
  float vtxZ;
  int vtxntrk;
  int evt;
  int good;
  float tmuon;
  float tmuon_err;
  float tecal;
  float tecal_raw;
  float tecal_err;
  float field;
  int wbc;
  int delay;
  int bx;
  int ntracks;
  int ntrackFPix[2]; // tracks crossing the pixels
  int ntrackBPix[3]; // tracks crossing the pixels
  int ntrackFPixvalid[2]; // tracks crossing the pixels with valid hits
  int ntrackBPixvalid[3]; // tracks crossing the pixels with valid hits
  float trackSep;
#endif
  std::string list;

  EventData() { init(); };
  void init() {
    orb=NOVAL_I;
    ls=NOVAL_I;
    nvtx=NOVAL_I;
    run=NOVAL_I;
    vtxndof=NOVAL_F;
#ifdef COMPLETE
    vtxchi2=vtxD0=vtxX=vtxY=vtxZ=NOVAL_F;
    vtxntrk=NOVAL_I;
    evt=NOVAL_I;
    good=NOVAL_I;
    tmuon=NOVAL_F;
    tmuon_err=NOVAL_F;
    tecal=NOVAL_F;
    tecal_raw=NOVAL_F;
    tecal_err=NOVAL_F;
    field=NOVAL_F;
    wbc=NOVAL_I;
    delay=NOVAL_I;
    bx=NOVAL_I;
    ntracks=NOVAL_I;
    ntrackFPix[0]=ntrackFPix[1]=NOVAL_I;
    ntrackBPix[0]=ntrackBPix[1]=ntrackBPix[2]=NOVAL_I;
    ntrackFPixvalid[0]=ntrackFPixvalid[1]=NOVAL_I;
    ntrackBPixvalid[0]=ntrackBPixvalid[1]=ntrackBPixvalid[2]=NOVAL_I;
    trackSep=NOVAL_F;
    list="orb/I:ls:nvtx:run:vtxndof/F:vtxchi2:vtxD0:vtxX:vtxY:vtxZ:vtxntrk/I:evt:good:"
      "tmuon/F:tmuon_err:tecal:tecal_raw:tecal_err:field:wbc/I:delay:bx:ntracks:"
      "ntrackFPix[2]:ntrackBPix[3]:ntrackFPixvalid[2]:ntrackBPixvalid[3]:trackSep/F";
#else
    list="ls/I:nvtx:run:vtxndof/F";
#endif
  }
  
};

// Track info
class TrackData {
 public:
  int validfpix[2]; // valid recHits in Diks1,2 
  int validbpix[3]; // valid recHits in Layer 1,2,3
  int algo;
  int strip; // total valid hits
  int quality;
  float d0;
  float dz;
  float pt;
  float p;
  float ndof;
  float chi2;
  float eta;
  float phi;
#ifdef COMPLETE
  float theta;
  int fromVtx;
  int i;
  int pix; // total valid hits
  int pixhit[2]; // 0: top, 1: bottom
  int validpixhit[2]; // 0: top, 1: bottom
  int fpix[2]; // recHits in Diks1,2 
  int bpix[3]; // recHits in Layer 1,2,3
  int highPurity;
#endif
  std::string list;
 
  TrackData() { init(); }
  void init() {
    validfpix[0]=validfpix[1]=NOVAL_I;
    validbpix[0]=validbpix[1]=validbpix[2]=NOVAL_I;
    algo=NOVAL_I;
    strip=NOVAL_I;
    quality=NOVAL_I;
    d0=NOVAL_F;
    dz=NOVAL_F;
    pt=NOVAL_F;
    p=NOVAL_F;
    ndof=NOVAL_F;
    chi2=NOVAL_F;
    eta=NOVAL_F;
    phi=NOVAL_F;
#ifdef COMPLETE
    theta=NOVAL_F;
    fromVtx=NOVAL_I;
    i=NOVAL_I;
    pix=NOVAL_I;
    pixhit[0]=pixhit[1]=NOVAL_I;
    validpixhit[0]=validpixhit[1]=NOVAL_I;
    fpix[0]=fpix[1]=NOVAL_I;
    bpix[0]=bpix[1]=bpix[2]=NOVAL_I;
    highPurity=NOVAL_I;
    list="validfpix[2]/I:validbpix[3]:algo:strip:quality:d0/F:dz:pt:p:ndof:chi2:eta:phi:theta:"
      "fromVtx/I:i:pix:pixhit[2]:validpixhit[2]:fpix[2]:bpix[3]:highPurity";
#else
    list="validfpix[2]/I:validbpix[3]:algo:strip:quality:d0/F:dz:pt:p:ndof:chi2:eta:phi";
#endif
  }
  
};

// Module info
class ModuleData {
 public:
  int det;

  int layer;
  int ladder;
  int half;

  int side;
  int disk;
  int blade;
  int panel;

  int module;
  int prt;
  int shl;
  int sec;

  int outer;

  unsigned int rawid;

  std::string list;

  ModuleData() { init(); }
  void init() {
    det=NOVAL_I;
    layer=NOVAL_I;
    ladder=NOVAL_I;
    half=NOVAL_I;
    side=NOVAL_I;
    disk=NOVAL_I;
    blade=NOVAL_I;
    panel=NOVAL_I;
    module=NOVAL_I;
    prt=NOVAL_I;
    shl=NOVAL_I;
    sec=NOVAL_I;
    outer=NOVAL_I;
    rawid=abs(NOVAL_I);
    list="det/I:layer:ladder:half:side:disk:blade:panel:module:prt:shl:sec:outer:"
      "rawid/i";
  }

  std::string shell() {
    std::ostringstream ss;
    if (det==0) {
      ss << "B" << ((module>0) ? "p" : "m") << ((ladder>0) ? "I" : "O");
    } else if (det==1) {
      ss << "B" << ((disk>0) ? "p" : "m") << ((blade>0) ? "I" : "O");
    }
    return ss.str();
  }

  int shell_num() {
    if (det==0)      return ((module>0) ? 0 : 2) + ((ladder>0) ? 0 : 1);
    else if (det==1) return ((disk>0) ? 0 : 2) + ((blade>0) ? 0 : 1);
    return -1;
  }

};

// Cluster info
class ClustData {
 public:
  int i; // serial num of cluster in the given module
  float charge;
  int size;
  int edge;     // set if there is a valid hit
  int badpix;   // set if there is a valid hit
  int tworoc;   // set if there is a valid hit
  int sizeX;
  int sizeY;
  float x;
  float y;
  // must be the last part of the object
#ifdef COMPLETE
  float adc[1000];
#endif
  std::string list;

  ClustData() { init(); }
  void init() {
    i=NOVAL_I;
    charge=NOVAL_F;
    size=NOVAL_I;
    edge=NOVAL_I;
    badpix=NOVAL_I;
    tworoc=NOVAL_I;
    sizeX=NOVAL_I;
    sizeY=NOVAL_I;
    x=NOVAL_F;
    y=NOVAL_F;
#ifdef COMPLETE
    for (size_t i=0; i<1000; i++) adc[i]=NOVAL_F;
    list="i/I:charge/F:size/I:edge:badpix:tworoc:sizeX:sizeY:x/F:y:adc[size]";
#else
    list="i/I:charge/F:size/I:edge:badpix:tworoc:sizeX:sizeY:x/F:y";
#endif
    }
  
};

// Digi info
  class DigiData {
  public:
    int i; // serial num of digi in the given module
    int row;
    int col;
    int adc;
    
    std::string list;
    
    DigiData() { init(); }
    void init() {
      i=NOVAL_I;
      row=NOVAL_I;
      col=NOVAL_I;
      adc=NOVAL_I;
      list="i/I:row:col:adc";
    }
    
  };

// Trajectory info
class TrajMeasData {
 public:
  int validhit;
  int missing;
  float lx;
  float ly;
  float lxmatch;
  float lymatch;
  float dx_cl[2];
  float dy_cl[2];
  float dx_hit;
  float dy_hit;
  float glx;
  float gly;
  float glz;
  int i; // serial num of trajectory measurement on the (single) track of the event
  float norm_charge;
#ifdef COMPLETE
  int onedge;
  int telescope;
  int telescope_valid;
  int dmodule; // D(propagated hit, valid hit)
  int dladder; // D(propagated hit, valid hit)
  int inactive;
  int badhit;
  float alpha;
  float beta;
  float glmatch;
  float lz;
  float lx_err;
  float ly_err;
  float lz_err;
  float lxymatch;
  float res_hit;
  float sig_hit;
  float d_cl[2];
#endif
  std::string list;

  TrajMeasData() { init(); }
  void init() {
    validhit=NOVAL_I;
    missing=NOVAL_I;
    lx=NOVAL_F;
    ly=NOVAL_F;
    lxmatch=NOVAL_F;
    lymatch=NOVAL_F;
    dx_cl[0]=dx_cl[1]=NOVAL_F;
    dy_cl[0]=dy_cl[1]=NOVAL_F;
    dx_hit=NOVAL_F;
    dy_hit=NOVAL_F;
    glx=NOVAL_F;
    gly=NOVAL_F;
    glz=NOVAL_F;
    i=NOVAL_I;
    norm_charge=NOVAL_F;
#ifdef COMPLETE
    onedge=NOVAL_I;
    telescope=NOVAL_I;
    telescope_valid=NOVAL_I;
    dmodule=NOVAL_I;
    dladder=NOVAL_I;
    inactive=NOVAL_I;
    badhit=NOVAL_I;
    alpha=NOVAL_F;
    beta=NOVAL_F;
    glmatch=NOVAL_F;
    lz=NOVAL_F;
    lx_err=NOVAL_F;
    ly_err=NOVAL_F;
    lz_err=NOVAL_F;
    lxymatch=NOVAL_F;
    res_hit=NOVAL_F;
    sig_hit=NOVAL_F;
    d_cl[0]=d_cl[1]=NOVAL_F;
    list="validhit/I:missing:lx/F:ly:lxmatch:lymatch:dx_cl[2]:dy_cl[2]:dx_hit:dy_hit:glx:gly:"
      "glz:i/I:norm_charge/F:onedge/I:telescope:telescope_valid:dmodule:dladder:inactive:"
      "badhit:alpha/F:beta:glmatch:lz:lx_err:ly_err:lz_err:lxymatch:res_hit:sig_hit:d_cl[2]";
#else
    list="validhit/I:missing:lx/F:ly:lxmatch:lymatch:dx_cl[2]:dy_cl[2]:dx_hit:dy_hit:glx:gly:"
      "glz:i/I:norm_charge/F";
#endif
  }
  
};

class Cluster : public ClustData {
 public:
  
  ModuleData mod; // offline module number
  ModuleData mod_on; // online module number
  
  Cluster() { mod.init(); mod_on.init(); }
  void init() {
    ClustData::init();
    mod.init();
    mod_on.init();
  }
  
};

class Digi : public DigiData {
 public:
  
  ModuleData mod; // offline module number
  ModuleData mod_on; // online module number
  
  Digi() { mod.init(); mod_on.init(); }
  void init() {
    DigiData::init();
    mod.init();
    mod_on.init();
  }
  
};

class TrajMeasurement : public TrajMeasData {
public:
#ifdef COMPLETE
  ModuleData mod; // offline module number
#endif
  ModuleData mod_on; // online module number
  ClustData clu;
  TrackData trk;
  
  TrajMeasurement() { 
#ifdef COMPLETE
    mod.init(); 
#endif
    mod_on.init(); clu.init(); trk.init(); }
  void init() {
    TrajMeasData::init();
#ifdef COMPLETE
    mod.init();
#endif
    mod_on.init();
    clu.init();
    trk.init();
  }
};

