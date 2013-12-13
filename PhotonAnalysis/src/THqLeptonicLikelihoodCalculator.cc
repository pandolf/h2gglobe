#include "../interface/THqLeptonicLikelihoodCalculator.h"

#include "TROOT.h"
#include <iostream>
#include <cmath>
#include <cstdlib>




THqLeptonicLikelihoodCalculator::THqLeptonicLikelihoodCalculator( const std::string& fileName ) {


  TFile* file_ = TFile::Open(fileName.c_str());

  TH1D* h1_nJets_tHq_tmp              = (TH1D*)file_->Get( "nJets_thq" )->Clone();
  TH1D* h1_eta_qJet_tHq_tmp           = (TH1D*)file_->Get( "eta_qJet_thq" )->Clone();
  TH1D* h1_mt_top_tHq_tmp             = (TH1D*)file_->Get( "mt_top_thq" )->Clone();
  TH1D* h1_lept_charge_tHq_tmp        = (TH1D*)file_->Get( "lept_charge_thq" )->Clone();
  TH1D* h1_deltaEta_lept_qJet_tHq_tmp = (TH1D*)file_->Get( "deltaEta_lept_qJet_thq" )->Clone();

  TH1D* h1_nJets_ttH_tmp              = (TH1D*)file_->Get( "nJets_tth" )->Clone();
  TH1D* h1_eta_qJet_ttH_tmp           = (TH1D*)file_->Get( "eta_qJet_tth" )->Clone();
  TH1D* h1_mt_top_ttH_tmp             = (TH1D*)file_->Get( "mt_top_tth" )->Clone();
  TH1D* h1_lept_charge_ttH_tmp        = (TH1D*)file_->Get( "lept_charge_tth" )->Clone();
  TH1D* h1_deltaEta_lept_qJet_ttH_tmp = (TH1D*)file_->Get( "deltaEta_lept_qJet_tth" )->Clone();

  gROOT->cd();

  h1_nJets_tHq_               = new TH1D(*h1_nJets_tHq_tmp              );
  h1_eta_qJet_tHq_            = new TH1D(*h1_eta_qJet_tHq_tmp           );
  h1_mt_top_tHq_              = new TH1D(*h1_mt_top_tHq_tmp             );
  h1_lept_charge_tHq_         = new TH1D(*h1_lept_charge_tHq_tmp        );
  h1_deltaEta_lept_qJet_tHq_  = new TH1D(*h1_deltaEta_lept_qJet_tHq_tmp );

  h1_nJets_ttH_               = new TH1D(*h1_nJets_ttH_tmp              );
  h1_eta_qJet_ttH_            = new TH1D(*h1_eta_qJet_ttH_tmp           );
  h1_mt_top_ttH_              = new TH1D(*h1_mt_top_ttH_tmp             );
  h1_lept_charge_ttH_         = new TH1D(*h1_lept_charge_ttH_tmp        );
  h1_deltaEta_lept_qJet_ttH_  = new TH1D(*h1_deltaEta_lept_qJet_ttH_tmp );


  h1_nJets_tHq_              ->Scale(1./h1_nJets_tHq_->Integral("width"));
  h1_eta_qJet_tHq_           ->Scale(1./h1_eta_qJet_tHq_->Integral("width"));
  h1_mt_top_tHq_             ->Scale(1./h1_mt_top_tHq_->Integral("width"));
  h1_lept_charge_tHq_        ->Scale(1./h1_lept_charge_tHq_->Integral("width"));
  h1_deltaEta_lept_qJet_tHq_ ->Scale(1./h1_deltaEta_lept_qJet_tHq_->Integral("width"));

  h1_nJets_ttH_              ->Scale(1./h1_nJets_ttH_->Integral("width"));
  h1_eta_qJet_ttH_           ->Scale(1./h1_eta_qJet_ttH_->Integral("width"));
  h1_mt_top_ttH_             ->Scale(1./h1_mt_top_ttH_->Integral("width"));
  h1_lept_charge_ttH_        ->Scale(1./h1_lept_charge_ttH_->Integral("width"));
  h1_deltaEta_lept_qJet_ttH_ ->Scale(1./h1_deltaEta_lept_qJet_ttH_->Integral("width"));

  file_->Close();

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


}



double THqLeptonicLikelihoodCalculator::computeLikelihood( int nJets, float eta_qJet, float mt_top, int lept_charge, float deltaEta_lept_qJet ) {


  eta_qJet = fabs(eta_qJet);
  deltaEta_lept_qJet = fabs(deltaEta_lept_qJet);




  //gROOT->cd();



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


