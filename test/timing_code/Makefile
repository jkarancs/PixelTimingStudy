include Makefile.arch

# CXXFLAGS += -I $(CMSSW_BASE) -I /afs/cern.ch/cms/sw/slc4_ia32_gcc345/external/boost/1.38.0/include
# CXXFLAGS += -I $(CMSSW_BASE)

#ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
#ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
#ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
#ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)

#------------------------------------------------------------------------------
EFFHISTOO      = EffHisto.$(ObjSuf)
EFFHISTOS      = EffHisto.$(SrcSuf)
EFFHISTO       = EffHisto$(ExeSuf)

OBJS          = $(EFFHISTOO)
PROGRAMS      = $(EFFHISTO)

#------------------------------------------------------------------------------
CALCHISTOO      = CalcHisto.$(ObjSuf)
CALCHISTOS      = CalcHisto.$(SrcSuf)
CALCHISTO       = CalcHisto$(ExeSuf)

OBJS          += $(CALCHISTOO)
PROGRAMS      += $(CALCHISTO)

#------------------------------------------------------------------------------
TESTHISTOGRAMO      = TestHistogram.$(ObjSuf)
TESTHISTOGRAMS      = TestHistogram.$(SrcSuf)
TESTHISTOGRAM       = TestHistogram$(ExeSuf)

OBJS          += $(TESTHISTOGRAMO)
PROGRAMS      += $(TESTHISTOGRAM)

#------------------------------------------------------------------------------


.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(EFFHISTO):     $(EFFHISTOO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(CALCHISTO):     $(CALCHISTOO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(TESTHISTOGRAM):     $(TESTHISTOGRAMO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

clean:
		@rm -f $(OBJS) core


distclean:      clean
		@rm -f $(PROGRAMS) *Dict.* *.def *.exp \
		   *.root *.ps *.so *.lib *.dll *.d *.log .def so_locations
		@rm -rf cxx_repository

.SUFFIXES: .$(SrcSuf)

###

.$(SrcSuf).$(ObjSuf):
	$(CXX) $(CXXFLAGS) -c $<

