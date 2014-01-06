CC = g++
CFLAGS = -Wall -c -g

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I$(ROOTSYS)/include  -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src -I$(CMSSW_BASE)/src/CommonTools -I$(CMSSW_BASE)/src/ -I$(CMSSW_BASE)/src/CondFormats/JetMETObjects/interface

ROOTSYS  ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs` 

EXTRALIBS  :=  -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -L$(CMSSW_BASE)/lib/slc5_amd64_gcc462/ -lHtml -lMathCore -lGenVector -lMinuit -lEG -lRooFitCore -lRooFit



draw_limit: DrawBase.o fitTools.o draw_limit.cpp
	$(CC) -Wall $(INCLUDES) -o draw_limit draw_limit.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

draw_limit_2: DrawBase.o fitTools.o draw_limit_2.cpp
	$(CC) -Wall $(INCLUDES) -o draw_limit_2 draw_limit_2.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

create_datacards_realistic: create_datacards_realistic.cpp
	$(CC) -Wall $(INCLUDES) -o create_datacards_realistic create_datacards_realistic.cpp $(ROOTFLAG) $(EXTRALIBS)

draw_significance_massWindow: DrawBase.o fitTools.o draw_significance_massWindow.cpp
	$(CC) -Wall $(INCLUDES) -o draw_significance_massWindow draw_significance_massWindow.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)





DrawBase.o: $(CMSSW_BASE)/src/CommonTools/DrawBase.C fitTools.o
	$(CC) $(CFLAGS) $(INCLUDES) fitTools.o $(CMSSW_BASE)/src/CommonTools/DrawBase.C $(ROOTFLAG) $(EXTRALIBS) 

fitTools.o: $(CMSSW_BASE)/src/CommonTools/fitTools.C
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/CommonTools/fitTools.C $(ROOTFLAG) $(EXTRALIBS)


clean:
	rm *.o
