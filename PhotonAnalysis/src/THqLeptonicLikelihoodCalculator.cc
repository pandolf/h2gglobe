#include "../interface/THqLeptonicLikelihoodCalculator.h"

#include "TROOT.h"
#include <iostream>
#include <cmath>
#include <cstdlib>




THqLeptonicLikelihoodCalculator::THqLeptonicLikelihoodCalculator( const std::string& tHqFileName, const std::string& ttHFileName) {


  file_tHq_ = TFile::Open(tHqFileName.c_str());
  tree_tHq_ = (TTree*)file_tHq_->Get("tree_passedEvents");

  file_ttH_ = TFile::Open(ttHFileName.c_str());
  tree_ttH_ = (TTree*)file_ttH_->Get("tree_passedEvents");


  gROOT->cd();

  h1_nJets_tHq_              = new TH1D( "nJets_tHq", "", 9, -0.5, 8.5 );
  h1_eta_qJet_tHq_           = new TH1D( "eta_qJet_tHq", "", 30, 0., 5. );
  h1_mt_top_tHq_             = new TH1D( "mt_top_tHq", "", 50, 0., 1000. );
  h1_lept_charge_tHq_        = new TH1D( "lept_charge_tHq", "", 3, -1.5, 1.5 );
  h1_deltaEta_lept_qJet_tHq_ = new TH1D( "deltaEta_lept_qJet_tHq", "", 35, 0., 7. );


  tree_tHq_->Project("nJets_tHq", "njets", "isLeptonic*eventWeight");
  tree_tHq_->Project("eta_qJet_tHq", "eta_qJet", "isLeptonic*eventWeight");
  tree_tHq_->Project("mt_top_tHq", "mt_top", "isLeptonic*eventWeight");
  tree_tHq_->Project("lept_charge_tHq", "charge_lept", "isLeptonic*eventWeight");
  tree_tHq_->Project("deltaEta_lept_qJet_tHq", "deltaEta_lept_qJet", "isLeptonic*eventWeight");


  h1_nJets_tHq_              ->Scale(1./h1_nJets_tHq_->Integral("width"));
  h1_eta_qJet_tHq_           ->Scale(1./h1_eta_qJet_tHq_->Integral("width"));
  h1_mt_top_tHq_             ->Scale(1./h1_mt_top_tHq_->Integral("width"));
  h1_lept_charge_tHq_        ->Scale(1./h1_lept_charge_tHq_->Integral("width"));
  h1_deltaEta_lept_qJet_tHq_ ->Scale(1./h1_deltaEta_lept_qJet_tHq_->Integral("width"));






  h1_nJets_ttH_              = new TH1D( "nJets_ttH", "", 9, -0.5, 8.5 );
  h1_eta_qJet_ttH_           = new TH1D( "eta_qJet_ttH", "", 30, 0., 5. );
  h1_mt_top_ttH_             = new TH1D( "mt_top_ttH", "", 50, 0., 1000. );
  h1_lept_charge_ttH_        = new TH1D( "lept_charge_ttH", "", 3, -1.5, 1.5 );
  h1_deltaEta_lept_qJet_ttH_ = new TH1D( "deltaEta_lept_qJet_ttH", "", 35, 0., 7. );


  tree_ttH_->Project("nJets_ttH", "njets", "isLeptonic*eventWeight");
  tree_ttH_->Project("eta_qJet_ttH", "eta_qJet", "isLeptonic*eventWeight");
  tree_ttH_->Project("mt_top_ttH", "mt_top", "isLeptonic*eventWeight");
  tree_ttH_->Project("lept_charge_ttH", "charge_lept", "isLeptonic*eventWeight");
  tree_ttH_->Project("deltaEta_lept_qJet_ttH", "deltaEta_lept_qJet", "isLeptonic*eventWeight");


  h1_nJets_ttH_              ->Scale(1./h1_nJets_ttH_->Integral("width"));
  h1_eta_qJet_ttH_           ->Scale(1./h1_eta_qJet_ttH_->Integral("width"));
  h1_mt_top_ttH_             ->Scale(1./h1_mt_top_ttH_->Integral("width"));
  h1_lept_charge_ttH_        ->Scale(1./h1_lept_charge_ttH_->Integral("width"));
  h1_deltaEta_lept_qJet_ttH_ ->Scale(1./h1_deltaEta_lept_qJet_ttH_->Integral("width"));




  file_tHq_->Close();
  file_ttH_->Close();


}


THqLeptonicLikelihoodCalculator::~THqLeptonicLikelihoodCalculator() {


  delete h1_nJets_tHq_              ;
  delete h1_eta_qJet_tHq_           ;
  delete h1_mt_top_tHq_             ;
  delete h1_lept_charge_tHq_        ;
  delete h1_deltaEta_lept_qJet_tHq_ ;

  delete h1_nJets_ttH_              ;
  delete h1_eta_qJet_ttH_           ;
  delete h1_mt_top_ttH_             ;
  delete h1_lept_charge_ttH_        ;
  delete h1_deltaEta_lept_qJet_ttH_ ;

  h1_nJets_tHq_              = 0;
  h1_eta_qJet_tHq_           = 0;
  h1_mt_top_tHq_             = 0;
  h1_lept_charge_tHq_        = 0;
  h1_deltaEta_lept_qJet_tHq_ = 0;

  h1_nJets_ttH_              = 0;
  h1_eta_qJet_ttH_           = 0;
  h1_mt_top_ttH_             = 0;
  h1_lept_charge_ttH_        = 0;
  h1_deltaEta_lept_qJet_ttH_ = 0;


  file_tHq_=0;
  file_ttH_=0;

  tree_ttH_=0;
  tree_tHq_=0;

}



double THqLeptonicLikelihoodCalculator::computeLikelihood( int nJets, float eta_qJet, float mt_top, int lept_charge, float deltaEta_lept_qJet ) {

  eta_qJet = fabs(eta_qJet);
  deltaEta_lept_qJet = fabs(deltaEta_lept_qJet);


  double likeliProd_tHq = 1.;
  likeliProd_tHq *= h1_nJets_tHq_             ->GetBinContent(h1_nJets_tHq_             ->FindBin(nJets));
  likeliProd_tHq *= h1_eta_qJet_tHq_          ->GetBinContent(h1_eta_qJet_tHq_          ->FindBin(eta_qJet));
  likeliProd_tHq *= h1_mt_top_tHq_            ->GetBinContent(h1_mt_top_tHq_            ->FindBin(mt_top));
  likeliProd_tHq *= h1_lept_charge_tHq_       ->GetBinContent(h1_lept_charge_tHq_       ->FindBin(lept_charge));
  likeliProd_tHq *= h1_deltaEta_lept_qJet_tHq_->GetBinContent(h1_deltaEta_lept_qJet_tHq_->FindBin(deltaEta_lept_qJet));

  double likeliProd_ttH = 1.;
  likeliProd_ttH *= h1_nJets_ttH_             ->GetBinContent(h1_nJets_ttH_             ->FindBin(nJets));
  likeliProd_ttH *= h1_eta_qJet_ttH_          ->GetBinContent(h1_eta_qJet_ttH_          ->FindBin(eta_qJet));
  likeliProd_ttH *= h1_mt_top_ttH_            ->GetBinContent(h1_mt_top_ttH_            ->FindBin(mt_top));
  likeliProd_ttH *= h1_lept_charge_ttH_       ->GetBinContent(h1_lept_charge_ttH_       ->FindBin(lept_charge));
  likeliProd_ttH *= h1_deltaEta_lept_qJet_ttH_->GetBinContent(h1_deltaEta_lept_qJet_ttH_->FindBin(deltaEta_lept_qJet));

  
  double likelihood = (likeliProd_tHq>0.) ? likeliProd_tHq / ( likeliProd_tHq + likeliProd_ttH ) : 0.;


  return likelihood;

}
