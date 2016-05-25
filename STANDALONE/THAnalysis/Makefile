###############################################################################
#
# tHAnalysis Makefile
# ----------------------
#
#
#             24/05/2013 Christian Boeser <Christian.boser@cern.ch>
#			 Simon Fink <simon.fink@kit.edu>
#			 Benedikt Maier <benedikt.maier@kit.edu>
################################################################################
CXX         = g++

TMPDIR      = $(STANDALONE_DIR)/tmp
LIBDIR      = $(STANDALONE_DIR)/lib
BINDIR      = $(STANDALONE_DIR)/bin

ROOTCXXFLAGS= $(shell $(ROOTSYS)/bin/root-config --cflags)
CXXFLAGS    = -g -O3 -Wall -fPIC -I.  $(ROOTCXXFLAGS) $(INCLUDES) #-fdiagnostics-color=always

scramtag    = $(shell cd $(CMSSW_BASE); scram tool tag $(1) $(2))

ROOTLIBS      = $(shell $(ROOTSYS)/bin/root-config --libs) -lTMVA -lMathMore -lRooFitCore  -lGenVector -L/cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.22-cms4/lib #-L /cvmfs/cms.cern.ch/slc5_amd64_gcc462/lcg/roofit/5.32.03-cms16/lib

BOOST_INCDIR  = $(call scramtag,boost,INCLUDE)
BOOST_LIB     = $(call scramtag,boost,LIB) $(call scramtag,boost_filesystem,LIB) $(call scramtag,boost_regex,LIB) boost_iostreams boost_date_time
BOOST_LIB    := $(patsubst %,-l%,$(BOOST_LIB)) -L$(call scramtag,boost,LIBDIR)

PYTHON_INCDIR = $(call scramtag,python,INCLUDE)
PYTHON_LIB    = $(call scramtag,python,LIB)
PYTHON_LIB    := $(patsubst %,-l%,$(PYTHON_LIB)) -L$(call scramtag,python,LIBDIR)

DCAP_LIB      = $(call scramtag,dcap,LIB)
DCAP_LIB     := $(patsubst %,-l%,$(DCAP_LIB)) -L$(call scramtag,dcap,LIBDIR)
DCAP_INCDIR   = $(call scramtag,dcap,INCLUDE)

NB_INCDIR     = /usr/users/phit/neurobayes/include

#ROOFIT_INCDIR = /cvmfs/cms.cern.ch/slc6_amd64_gcc491/lcg/roofit/5.34.18-cms6/include/
#ROOFIT_INCDIR = /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/root/5.34.18-cms5/include
ROOFIT_INCDIR = /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.22-cms4/include
ROOFIT_LIB    = /cvmfs/cms.cern.ch/slc6_amd64_gcc481/lcg/roofit/5.34.22-cms4/lib
#add the current path to the list of includes as absolute paths, such that it also works if make goes into subdirs.

INCLUDES    = -I.. -I. -I$(BOOST_INCDIR) -I$(DCAP_INCDIR) -I$(PYTHON_INCDIR) -I$(CMSSW_BASE)/src -I$(NB_INCDIR) -I$(ROOFIT_INCDIR)

OBJS        = $(TMPDIR)/CommandLine.o \
	      $(TMPDIR)/AnaUtils_dict.o \
	      $(TMPDIR)/Histogram.o \
	      $(TMPDIR)/THStatus.o \
	      $(TMPDIR)/MCSample.o \
	      $(TMPDIR)/RunRangeManager.o \
	      $(TMPDIR)/DataSample.o \
	      $(TMPDIR)/MatrixMethod.o \
	      $(TMPDIR)/RootStyle.o 
#	      $(TMPDIR)/MCWeighters.o 
#	      $(TMPDIR)/TTbarHFFinder.o \

LIB         = libtHAnalysis.so

LIBS        = -L$(LIBDIR)  -ltHAnalysis    $(BOOST_LIB) $(DCAP_LIB) $(PYTHON_LIB)  #$(CMSSW_BASE)/lib/slc6_amd64_gcc491/libCondFormatsJetMETObjects.so 
#entries that used to be here:
# $(CMSSW_BASE)/lib/slc5_amd64_gcc462/libEgammaAnalysisElectronTools.so  $(CMSSW_BASE)/lib/slc5_amd64_gcc462/libCondFormatsPhysicsToolsObjects.so #/portal/ekpcms5/home/nfaltermann/tmva-neurobayes/libTMVANeuroBayes.so $(CMSSW_BASE)/lib/slc5_amd64_gcc462/libKITAKITAObjects.so -lTMVA -lCintex -lJetMETObjects

#mybin := $(patsubst %,th_%_x,$(BIN))

all:  setup lib bin #$(mybin)

setup:
	mkdir -p $(TMPDIR)
	mkdir -p $(LIBDIR)
	mkdir -p $(BINDIR)

lib: $(OBJS)
	$(CXX) $(CXXFLAGS) -shared $(OBJS) $(ROOTLIBS) -o $(LIBDIR)/$(LIB) 

# list of program code files that can be compiled. each of these is an abbreviation for th_*_x.cc
programnames = eventlist flavorschemematcher userinfo datatomc hypotheses skim recteacher overtrain recexpert tophypotheses regprepare regteacher regexpert datatodata bdtteacher bdtexpert roc makedatacards addsftodc scalejes scalejer  sigtoback maketemplates syscomparer toprecteacher addvariables addlepw toprecexpert  bdttagexpert correlation significance toppt csvreweight toprecexpertpop recexpertpop bdtexpertpop metrecalculator debug abcd abcdclosure lepscales transform addtransform roccomparisons datatomcsys # pileupshift  cutflow

# turn above list into list of full output paths, and then define it as the "bin" target
programlocations := $(patsubst %,$(BINDIR)/th_%_x,$(programnames))
bin: $(programlocations) 

# target for binaries; written in such a way that "make" understands whether the file has already been compiled
$(BINDIR)/th_%_x: bin/th_%_x.cc
	$(CXX) $(CXXFLAGS) $< $(LIBS) $(ROOTLIBS) \
	        -o $@  
# target for binaries; written in such a way that you can use the shorthand name of the program
%: bin/th_%_x.cc
	$(CXX) $(CXXFLAGS) bin/th_$@_x.cc $(LIBS) $(ROOTLIBS) \
        -o $(BINDIR)/th_$@_x  

clean:
	rm -rf $(OBJS) $(BINDIR)/*

################################################################################
# $(OBJS)
################################################################################
$(TMPDIR)/AnaUtils_dict.cc: src/Math.cc interface/AnaUtilsLinkDef.h
	rm -rf $(TMPDIR)/AnaUtils_dict.h
	rm -rf $(TMPDIR)/AnaUtils_dict.cc
	$(ROOTSYS)/bin/rootcint -f $(TMPDIR)/AnaUtils_dict.cc \
	-c src/Math.cc interface/AnaUtilsLinkDef.h
$(TMPDIR)/AnaUtils_dict.o: $(TMPDIR)/AnaUtils_dict.cc
	$(CXX) $(CXXFLAGS) -c $(TMPDIR)/AnaUtils_dict.cc -o $(TMPDIR)/AnaUtils_dict.o
# Dummy for all standard objs
$(TMPDIR)/%.o: src/%.cc interface/%.h
	@echo creating $@ from $<; $(CXX) $(CXXFLAGS) -c -o $@ $<
