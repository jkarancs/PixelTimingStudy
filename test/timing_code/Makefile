include Makefile.arch

CXXFLAGS += -I $(CMSSW_BASE) -I /afs/cern.ch/cms/sw/slc4_ia32_gcc345/external/boost/1.38.0/include
LIBS  += -L/afs/cern.ch/cms/sw/slc4_ia32_gcc345/external/boost/1.38.0/lib -lboost_regex-gcc34-mt-1_38


#ROOTCFLAGS   := $(shell $(ROOTCONFIG) --cflags)
#ROOTLDFLAGS  := $(shell $(ROOTCONFIG) --ldflags)
#ROOTLIBS     := $(shell $(ROOTCONFIG) --libs)
#ROOTGLIBS    := $(shell $(ROOTCONFIG) --glibs)

#------------------------------------------------------------------------------
TIMINGANALYSISO      = TimingAnalysis.$(ObjSuf)
TIMINGANALYSISS      = TimingAnalysis.$(SrcSuf)
TIMINGANALYSIS       = TimingAnalysis$(ExeSuf)

OBJS          = $(TIMINGANALYSISO)
PROGRAMS      = $(TIMINGANALYSIS)


TIMINGANALYSISDATAO      = TimingAnalysisData.$(ObjSuf)
TIMINGANALYSISDATAS      = TimingAnalysisData.$(SrcSuf)
TIMINGANALYSISDATA       = TimingAnalysisData$(ExeSuf)

OBJS          += $(TIMINGANALYSISDATAO)
PROGRAMS      += $(TIMINGANALYSISDATA)

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(TIMINGANALYSIS):     $(TIMINGANALYSISO)
		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
		$(MT_EXE)
		@echo "$@ done"

$(TIMINGANALYSISDATA):     $(TIMINGANALYSISDATAO)
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

#	$(CXX) $(CXXFLAGS) -I$(BUILD_HOME)/pixel -I $(XDAQ_ROOT)/include -c $<
