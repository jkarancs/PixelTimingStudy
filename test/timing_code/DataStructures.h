#ifndef DataStructures_h
#define DataStructures_h

#include <sstream>

#define NOVAL_I -9999
#define NOVAL_F -9999.0

/* // Event info */
/* class EventData { */
/*  public: */
/*   int run; */
/*   int evt; */
/*   float tmuon; */
/*   float tmuon_err; */
/*   float tecal; */
/*   float tecal_raw; */
/*   float tecal_err; */
/*   float field; */
/*   int wbc; */
/*   int delay; */
/*   int bx; */
/*   int orb; */
/*   int ntracks; */
  
/*   std::string list; */
  
/*   EventData() { init(); }; */
/*   void init() { */
/*     run=NOVAL_I; */
/*     evt=NOVAL_I; */
/*     tmuon=NOVAL_F; */
/*     tmuon_err=NOVAL_F; */
/*     tecal=NOVAL_F; */
/*     tecal_raw=NOVAL_F; */
/*     tecal_err=NOVAL_F; */
/*     field=NOVAL_F; */
/*     wbc=NOVAL_I; */
/*     delay=NOVAL_I; */
/*     bx=NOVAL_I; */
/*     orb=NOVAL_I; */
/*     ntracks=NOVAL_I; */
/*     list="run/I:evt:tmuon/F:tmuon_err:tecal:tecal_raw:tecal_err:field:wbc/I:delay:bx:orb:ntracks"; */
/*   } */
/* }; */


/* // Track info */
/* class TrackData { */
/*  public: */
/*   int i; */
/*   int pix; // total valid hits */
/*   int strip; // total valid hits */
/*   int pixhit[2]; // 0: top, 1: bottom */
/*   int validpixhit[2]; // 0: top, 1: bottom */
/*   float ndof; */
/*   float chi2; */
/*   float d0; */
/*   float dz; */
/*   float pt; */
  
/*   std::string list; */
  
/*   TrackData() { init(); } */
/*   void init() { */
/*     i=NOVAL_I; */
/*     pix=NOVAL_I; */
/*     strip=NOVAL_I; */
/*     pixhit[0]=pixhit[1]=NOVAL_I; */
/*     validpixhit[0]=validpixhit[1]=NOVAL_I; */
/*     ndof=NOVAL_F; */
/*     chi2=NOVAL_F; */
/*     d0=NOVAL_F; */
/*     dz=NOVAL_F; */
/*     pt=NOVAL_F; */
/*     list="i/I:pix:strip/I:pixhit[2]/I:validpixhit[2]/I:ndof/F:chi2:d0:dz:pt"; */
/*   } */
/* }; */


/* // Module info */
/* class ModuleData { */
/*  public: */
/*   unsigned int rawid; */
/*   int det; */
  
/*   int layer; */
/*   int ladder; */
/*   int sec; */
  
/*   int side; */
/*   int disk; */
/*   int blade; */
/*   int panel; */
  
/*   int module; */
/*   int prt; */
/*   int shl; */
  
/*   std::string list; */
  
/*   ModuleData() { init(); } */
/*   void init() { */
/*     rawid=abs(NOVAL_I); */
/*     det=NOVAL_I; */
/*     layer=NOVAL_I; */
/*     ladder=NOVAL_I; */
/*     sec=NOVAL_I; */
/*     side=NOVAL_I; */
/*     disk=NOVAL_I; */
/*     blade=NOVAL_I; */
/*     panel=NOVAL_I; */
/*     module=NOVAL_I; */
/*     prt=NOVAL_I; */
/*     shl=NOVAL_I; */
/*     list="rawid/i:det/I:layer:ladder:sec:side:disk:blade:panel:module:prt:shl"; */
/*   } */
  
/*   std::string shell() { */
/*     std::ostringstream ss; */
/*     if (det==0) { */
/*       ss << "B" << ((module>0) ? "p" : "m") << ((ladder>0) ? "I" : "O"); */
/*     } else if (det==1) { */
/*       ss << "B" << ((disk>0) ? "p" : "m") << ((blade>0) ? "I" : "O"); */
/*     } */
/*     return ss.str(); */
/*   } */
  
/*   int shell_num() { */
/*     if (det==0)      return ((module>0) ? 0 : 2) + ((ladder>0) ? 0 : 1); */
/*     else if (det==1) return ((disk>0) ? 0 : 2) + ((blade>0) ? 0 : 1); */
/*     return -1; */
/*   } */
  
/* }; */


/* // Cluster info */
/* class ClustData { */
/*  public: */
/*   int i; // serial num of cluster in the given module */
/*   float charge; */
/*   int size; */
/*   int edge;     // set if there is a valid hit */
/*   int badpix;   // set if there is a valid hit */
/*   int tworoc;   // set if there is a valid hit */
/*   // must be the last part of the object */
/*   float adc[1000]; */

/*   std::string list; */
  
/*   ClustData() { init(); } */
/*   void init() { */
/*     i=NOVAL_I; */
/*     charge=NOVAL_F; */
/*     size=NOVAL_I; */
/*     edge=NOVAL_I; */
/*     badpix=NOVAL_I; */
/*     tworoc=NOVAL_I; */
/*     for (size_t i=0; i<1000; i++) adc[i]=NOVAL_F; */
/*     list="i/I:charge/F:size/I:edge:badpix:tworoc:adc[size]/F"; */
/*   } */
/* }; */


/* // Trajectory info */
/* class TrajMeasData { */
/*  public: */
/*   int i;//serial num of trajectory measurement on the (single)track of the event */
/*   int validhit; */
/*   int missing; */
/*   int inactive; */
/*   int badhit; */
/*   float alpha; */
/*   float beta; */
/*   float norm_charge; */
/*   float glx; */
/*   float gly; */
/*   float glz; */
/*   float res_hit; */
/*   float sig_hit; */
/*   int telescope; */
/*   int telescope_valid; */
  
/*   std::string list; */
  
/*     TrajMeasData() { init(); } */
/*     void init() { */
/*       i=NOVAL_I; */
/*       validhit=NOVAL_I; */
/*       missing=NOVAL_I; */
/*       inactive=NOVAL_I; */
/*       badhit=NOVAL_I; */
/*       alpha=NOVAL_F; */
/*       beta=NOVAL_F; */
/*       norm_charge=NOVAL_F; */
/*       glx=NOVAL_F; */
/*       gly=NOVAL_F; */
/*       glz=NOVAL_F; */
/*       res_hit=NOVAL_F; */
/*       sig_hit=NOVAL_F; */
/*       telescope=NOVAL_I; */
/*       telescope_valid=NOVAL_I; */
/*       list="i/I:validhit:missing:inactive:badhit:alpha/F:beta:norm_charge:" */
/* 	"glx:gly:glz:res_hit:sig_hit:telescope/I:telescope_valid"; */
/*     } */
/* }; */


/* class Cluster : public ClustData { */
/*  public: */
  
/*   ModuleData mod; // offline module number */
/*   ModuleData mod_on; // online module number */
  
/*   Cluster() { mod.init(); mod_on.init(); } */
/*   void init() { */
/*     ClustData::init(); */
/*     mod.init(); */
/*     mod_on.init(); */
/*   } */
/* }; */


/* class TrajMeasurement : public TrajMeasData { */
/*  public: */
/*   ModuleData mod; // offline module number */
/*   ModuleData mod_on; // online module number */
/*   ClustData clu; */
/*   TrackData trk; */
  
/*   TrajMeasurement() { mod.init(); mod_on.init(); clu.init(); trk.init(); } */
/*   void init() { */
/*     TrajMeasData::init(); */
/*     mod.init(); */
/*     mod_on.init(); */
/*     clu.init(); */
/*     trk.init(); */
/*   } */
/* }; */
  // Event info

  class EventData {
   public:
    int run;
    int evt;
    float tmuon;
    float tmuon_err;
    float tecal;
    float tecal_raw;
    float tecal_err;
    float field;
    int wbc;
    int delay;
    int bx;
    int orb;
    int ntracks;
    int ntrackFPix[2]; // tracks crossing the pixels
    int ntrackBPix[3]; // tracks crossing the pixels
    int ntrackFPixvalid[2]; // tracks crossing the pixels with valid hits
    int ntrackBPixvalid[3]; // tracks crossing the pixels with valid hits

    std::string list;

    EventData() { init(); };
    void init() {
      run=NOVAL_I;
      evt=NOVAL_I;
      tmuon=NOVAL_F;
      tmuon_err=NOVAL_F;
      tecal=NOVAL_F;
      tecal_raw=NOVAL_F;
      tecal_err=NOVAL_F;
      field=NOVAL_F;
      wbc=NOVAL_I;
      delay=NOVAL_I;
      bx=NOVAL_I;
      orb=NOVAL_I;
      ntracks=NOVAL_I;
      ntrackFPix[0]=ntrackFPix[1]=NOVAL_I;
      ntrackBPix[0]=ntrackBPix[1]=ntrackBPix[2]=NOVAL_I;
      ntrackFPixvalid[0]=ntrackFPixvalid[1]=NOVAL_I;
      ntrackBPixvalid[0]=ntrackBPixvalid[1]=ntrackBPixvalid[2]=NOVAL_I;
      list="run/I:evt:tmuon/F:tmuon_err:tecal:tecal_raw:tecal_err:field:wbc/I:delay:bx:orb:"
	"ntracks:ntrackFPix[2]:ntrackBPix[3]:ntrackFPixvalid[2]:ntrackBPixvalid[3]";
    }

  };


  // Track info
  class TrackData {
   public:
    int i;
    int pix; // total valid hits
    int strip; // total valid hits
    int pixhit[2]; // number of recHits in: 0: top, 1: bottom
    int validpixhit[2]; // 0: top, 1: bottom
    float ndof;
    float chi2;
    float d0;
    float dz;
    float pt;
    float eta;
    float theta;
    float phi;
    int fpix[2]; // recHits in Diks1,2 
    int bpix[3]; // recHits in Layer 1,2,3
    int validfpix[2]; // valid recHits in Diks1,2 
    int validbpix[3]; // valid recHits in Layer 1,2,3
    

    std::string list;
   
    TrackData() { init(); }
    void init() {
      i=NOVAL_I;
      pix=NOVAL_I;
      strip=NOVAL_I;
      pixhit[0]=pixhit[1]=NOVAL_I;
      validpixhit[0]=validpixhit[1]=NOVAL_I;
      ndof=NOVAL_F;
      chi2=NOVAL_F;
      d0=NOVAL_F;
      dz=NOVAL_F;
      pt=NOVAL_F;
      eta=NOVAL_F;
      theta=NOVAL_F;
      phi=NOVAL_F;
      fpix[0]=fpix[1]=NOVAL_I;
      bpix[0]=bpix[1]=bpix[2]=NOVAL_I;
      validfpix[0]=validfpix[1]=NOVAL_I;
      validbpix[0]=validbpix[1]=validbpix[2]=NOVAL_I;

      list="i/I:pix:strip/I:pixhit[2]/I:validpixhit[2]/I:ndof/F:chi2:d0:dz:pt:eta:theta:phi:"
	"fpix[2]/I:bpix[3]:validfpix[2]:validbpix[3]";
    }

  };


  // Module info
  class ModuleData {
   public:
    unsigned int rawid;
    int det;

    int layer;
    int ladder;
    int sec;

    int side;
    int disk;
    int blade;
    int panel;

    int module;
    int prt;
    int shl;

    std::string list;

    ModuleData() { init(); }
    void init() {
      rawid=abs(NOVAL_I);
      det=NOVAL_I;
      layer=NOVAL_I;
      ladder=NOVAL_I;
      sec=NOVAL_I;
      side=NOVAL_I;
      disk=NOVAL_I;
      blade=NOVAL_I;
      panel=NOVAL_I;
      module=NOVAL_I;
      prt=NOVAL_I;
      shl=NOVAL_I;
      list="rawid/i:det/I:layer:ladder:sec:side:disk:blade:panel:module:prt:shl";
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
    // must be the last part of the object
    float adc[1000];

    std::string list;

    ClustData() { init(); }
    void init() {
      i=NOVAL_I;
      charge=NOVAL_F;
      size=NOVAL_I;
      edge=NOVAL_I;
      badpix=NOVAL_I;
      tworoc=NOVAL_I;
      for (size_t i=0; i<1000; i++) adc[i]=NOVAL_F;
      list="i/I:charge/F:size/I:edge:badpix:tworoc:adc[size]/F";
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
    int i; // serial num of trajectory measurement on the (single) track of the event
    int validhit;
    int missing;
    int inactive;
    int badhit;
    float alpha;
    float beta;
    float norm_charge;
    float glx;
    float gly;
    float glz;
    float res_hit;
    float sig_hit;
    int telescope;
    int telescope_valid;

    std::string list;

    TrajMeasData() { init(); }
    void init() {
      i=NOVAL_I;
      validhit=NOVAL_I;
      missing=NOVAL_I;
      inactive=NOVAL_I;
      badhit=NOVAL_I;
      alpha=NOVAL_F;
      beta=NOVAL_F;
      norm_charge=NOVAL_F;
      glx=NOVAL_F;
      gly=NOVAL_F;
      glz=NOVAL_F;
      res_hit=NOVAL_F;
      sig_hit=NOVAL_F;
      telescope=NOVAL_I;
      telescope_valid=NOVAL_I;
      list="i/I:validhit:missing:inactive:badhit:alpha/F:beta:norm_charge:glx:gly:glz:"
	"res_hit:sig_hit:telescope/I:telescope_valid";
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
    ModuleData mod; // offline module number
    ModuleData mod_on; // online module number
    ClustData clu;
    TrackData trk;

    TrajMeasurement() { mod.init(); mod_on.init(); clu.init(); trk.init(); }
    void init() {
      TrajMeasData::init();
      mod.init();
      mod_on.init();
      clu.init();
      trk.init();
    }
  };

#endif
