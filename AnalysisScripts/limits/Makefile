CC = g++
CFLAGS = -Wall -c -g

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I$(ROOTSYS)/include  -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src -I$(CMSSW_BASE)/src/CommonTools -I$(CMSSW_BASE)/src/UserCode/pandolf/ -I$(CMSSW_BASE)/src/CondFormats/JetMETObjects/interface

ROOTSYS  ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs` 

EXTRALIBS  :=  -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -L$(CMSSW_BASE)/lib/slc5_amd64_gcc462/ -lHtml -lMathCore -lGenVector -lMinuit -lEG -lRooFitCore -lRooFit




create_datacards: create_datacards.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o create_datacards create_datacards.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

create_datacards_optLD: create_datacards_optLD.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o create_datacards_optLD create_datacards_optLD.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

createDrawBaseFiles: createDrawBaseFiles.cpp 
	$(CC) -Wall $(INCLUDES) -o createDrawBaseFiles createDrawBaseFiles.cpp $(ROOTFLAG) $(EXTRALIBS)

doShapeAnalysis: doShapeAnalysis.cpp
	$(CC) -Wall $(INCLUDES) -o doShapeAnalysis doShapeAnalysis.cpp $(ROOTFLAG) $(EXTRALIBS)

drawSignif_vs_LD: drawSignif_vs_LD.cpp DrawBase.o fitTools.o StatTools.o
	$(CC) -Wall $(INCLUDES) -o drawSignif_vs_LD drawSignif_vs_LD.cpp DrawBase.o fitTools.o StatTools.o $(ROOTFLAG) $(EXTRALIBS)

optimizeWindowDummy: optimizeWindowDummy.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o optimizeWindowDummy optimizeWindowDummy.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

computeShapeSyst: computeShapeSyst.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o computeShapeSyst computeShapeSyst.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

csStudies: csStudies.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o csStudies csStudies.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

csStudiesDummy: csStudiesDummy.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o csStudiesDummy csStudiesDummy.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

compareJERsyst: compareJERsyst.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o compareJERsyst compareJERsyst.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

validateLDvars: validateLDvars.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o validateLDvars validateLDvars.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)






DrawBase.o: $(CMSSW_BASE)/src/CommonTools/DrawBase.C fitTools.o
	$(CC) $(CFLAGS) $(INCLUDES) fitTools.o $(CMSSW_BASE)/src/CommonTools/DrawBase.C $(ROOTFLAG) $(EXTRALIBS) 

fitTools.o: $(CMSSW_BASE)/src/CommonTools/fitTools.C
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/CommonTools/fitTools.C $(ROOTFLAG) $(EXTRALIBS)

StatTools.o: $(CMSSW_BASE)/src/CommonTools/StatTools.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/CommonTools/StatTools.cc $(ROOTFLAG) $(EXTRALIBS)


clean:
	rm *.o
