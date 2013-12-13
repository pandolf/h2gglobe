#ifndef THqLeptonicLikelihoodCalculator_h
#define THqLeptonicLikelihoodCalculator_h

#include <string>

#include "TH1D.h"
#include "TTree.h"
#include "TFile.h"



class THqLeptonicLikelihoodCalculator {

 public:

  THqLeptonicLikelihoodCalculator( const std::string& fileName );
   ~THqLeptonicLikelihoodCalculator();


  double computeLikelihood( int nJets, float eta_qJet, float mt_top, int lept_charge, float deltaEta_lept_qJet ); 



 private:

  TH1D* h1_nJets_ttH_;
  TH1D* h1_eta_qJet_ttH_;
  TH1D* h1_mt_top_ttH_;
  TH1D* h1_lept_charge_ttH_;
  TH1D* h1_deltaEta_lept_qJet_ttH_;

  TH1D* h1_nJets_tHq_;
  TH1D* h1_eta_qJet_tHq_;
  TH1D* h1_mt_top_tHq_;
  TH1D* h1_lept_charge_tHq_;
  TH1D* h1_deltaEta_lept_qJet_tHq_;

};


#endif

