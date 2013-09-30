#ifndef THqLeptonicLikelihoodCalculator_h
#define THqLeptonicLikelihoodCalculator_h

#include <string>

#include "TH1D.h"



class THqLeptonicLikelihoodCalculator {

 public:

  THqLeptonicLikelihoodCalculator( const std::string& tHqFileName, const std::string& ttHFileName );
   ~THqLeptonicLikelihoodCalculator();

  void open_tHqFile( const std::string& fileName );
  void open_ttHFile( const std::string& fileName );


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

