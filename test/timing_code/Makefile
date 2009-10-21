include Makefile.arch

#CXXFLAGS += -I $(BUILD_HOME)/pixel -I $(XDAQ_ROOT)/include
#LIBS  += -L$(XDAQ_ROOT)/lib -L$(BUILD_HOME)/pixel/lib -lPixelConfigDBInterface -lSiPixelObjects

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

#------------------------------------------------------------------------------

.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

$(TIMINGANALYSIS):     $(TIMINGANALYSISO)
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
