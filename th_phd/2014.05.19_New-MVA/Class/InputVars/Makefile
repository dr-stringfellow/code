INCLUDE = -I$(PEC_FWK_INSTALL)/core/include -I$(PEC_FWK_INSTALL)/extensions/include -I./ \
 -I$(TH_ANALYSIS_INSTALL)/plugins/include \
 -I$(shell root-config --incdir) -I$(BOOST_ROOT)/include/boost-1_50
OPFLAGS = -O2
CFLAGS = -Wall -Wextra -std=c++11 $(INCLUDE) $(OPFLAGS) $(MACROS)
LDFLAGS = $(shell root-config --libs) -lTreePlayer -lHistPainter -lTMVA \
 -L$(BOOST_ROOT)/lib -lboost_filesystem-gcc46-mt-1_50 \
 -Wl,-rpath=$(BOOST_ROOT)/lib/

vpath %.cpp ./:$(TH_ANALYSIS_INSTALL)/plugins/src

all: run

run: run.o GlobalRecoTHPlugin.o GlobalRecoTTbarPlugin.o ClassInputVarsPlugin.o \
 TopPtWeightPlugin.o $(PEC_FWK_INSTALL)/lib/libpecfwk.a
	@ g++ $(CFLAGS) $(LDFLAGS) $+ -o $@

%.o: %.cpp
	@ g++ $(CFLAGS) -c $< -o $@