include Makefile.arch


##------------------------------------------------------------------------------
#EFFHISTOO      = EffHisto.$(ObjSuf)
#EFFHISTOS      = EffHisto.$(SrcSuf)
#EFFHISTO       = EffHisto$(ExeSuf)
#
#OBJS          = $(EFFHISTOO)
#PROGRAMS      = $(EFFHISTO)
#
#------------------------------------------------------------------------------
PIXELHISTOMAKERO      = PixelHistoMaker.$(ObjSuf)
PIXELHISTOMAKERS      = PixelHistoMaker.$(SrcSuf)
PIXELHISTOMAKER       = PixelHistoMaker$(ExeSuf)

OBJS          += $(PIXELHISTOMAKERO)
PROGRAMS      += $(PIXELHISTOMAKER)

#------------------------------------------------------------------------------


.SUFFIXES: .$(SrcSuf) .$(ObjSuf) .$(DllSuf)

all:            $(PROGRAMS)

#$(EFFHISTO):     $(EFFHISTOO)
#		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
#		$(MT_EXE)
#		@echo "$@ done"
#
#$(CALCHISTO):     $(CALCHISTOO)
#		$(LD) $(LDFLAGS) $^ $(LIBS) $(OutPutOpt)$@
#		$(MT_EXE)
#		@echo "$@ done"

$(PIXELHISTOMAKER):     $(PIXELHISTOMAKERO)
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

