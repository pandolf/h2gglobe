CC = g++
CFLAGS = -Wall -c -g

ROOFIT_INCLUDE := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep INCLUDE= | sed 's|INCLUDE=||')
ROOFIT_LIBDIR := $(shell cd $(CMSSW_BASE); scram tool info roofitcore | grep LIBDIR= | sed 's|LIBDIR=||')

INCLUDES = -I. -I$(ROOTSYS)/include  -I$(ROOFIT_INCLUDE)/ -I$(CMSSW_BASE)/src -I$(CMSSW_BASE)/src/CommonTools -I$(CMSSW_BASE)/src/UserCode/pandolf/ -I$(CMSSW_BASE)/src/CondFormats/JetMETObjects/interface

ROOTSYS  ?= ERROR_RootSysIsNotDefined

ROOTFLAG = `${ROOTSYS}/bin/root-config --cflags --libs` 

EXTRALIBS  :=  -L$(ROOTSYS)/lib -L$(ROOFIT_LIBDIR)/ -L$(CMSSW_BASE)/lib/slc5_amd64_gcc462/ -lHtml -lMathCore -lGenVector -lMinuit -lEG -lRooFitCore -lRooFit




validateLDvars: validateLDvars.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o validateLDvars validateLDvars.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

checkLD: checkLD.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o checkLD checkLD.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

create_LDfiles: create_LDfiles.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o create_LDfiles create_LDfiles.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

drawRoCs_andShit: drawRoCs_andShit.cpp DrawBase.o fitTools.o
	$(CC) -Wall $(INCLUDES) -o drawRoCs_andShit drawRoCs_andShit.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

testLD: testLD.cpp DrawBase.o fitTools.o THqLeptonicLikelihoodCalculator2.o
	$(CC) -Wall $(INCLUDES) -o testLD testLD.cpp DrawBase.o fitTools.o THqLeptonicLikelihoodCalculator2.o $(ROOTFLAG) $(EXTRALIBS)

findLDcut: findLDcut.cpp DrawBase.o fitTools.o 
	$(CC) -Wall $(INCLUDES) -o findLDcut findLDcut.cpp DrawBase.o fitTools.o $(ROOTFLAG) $(EXTRALIBS)

checkLD_onEvents: checkLD_onEvents.cpp DrawBase.o fitTools.o THqLeptonicLikelihoodCalculator.o
	$(CC) -Wall $(INCLUDES) -o checkLD_onEvents checkLD_onEvents.cpp DrawBase.o fitTools.o THqLeptonicLikelihoodCalculator.o $(ROOTFLAG) $(EXTRALIBS)






DrawBase.o: $(CMSSW_BASE)/src/CommonTools/DrawBase.C fitTools.o
	$(CC) $(CFLAGS) $(INCLUDES) fitTools.o $(CMSSW_BASE)/src/CommonTools/DrawBase.C $(ROOTFLAG) $(EXTRALIBS) 

fitTools.o: $(CMSSW_BASE)/src/CommonTools/fitTools.C
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/CommonTools/fitTools.C $(ROOTFLAG) $(EXTRALIBS)

THqLeptonicLikelihoodCalculator.o: $(CMSSW_BASE)/src/h2gglobe/PhotonAnalysis/src/THqLeptonicLikelihoodCalculator.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/h2gglobe/PhotonAnalysis/src/THqLeptonicLikelihoodCalculator.cc $(ROOTFLAG) $(EXTRALIBS)

THqLeptonicLikelihoodCalculator2.o: $(CMSSW_BASE)/src/h2gglobe/PhotonAnalysis/src/THqLeptonicLikelihoodCalculator2.cc
	$(CC) $(CFLAGS) $(INCLUDES) $(CMSSW_BASE)/src/h2gglobe/PhotonAnalysis/src/THqLeptonicLikelihoodCalculator2.cc $(ROOTFLAG) $(EXTRALIBS)


clean:
	rm *.o
