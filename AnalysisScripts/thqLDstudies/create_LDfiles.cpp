#include <iostream>

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"


bool use_centralJets = false;



int main( int argc, char* argv[] ) {


  //std::string fileName = "../thq_jetsyst_v11/jerCentral/histograms_CMS-HGG.root"; 
  std::string fileName = "../batchOutput_preNatal_v5/histograms_CMS-HGG.root"; 
  std::cout << "-> Opening file: " << fileName << std::endl;
  TFile* file = TFile::Open(fileName.c_str());
  TTree* tree_thq = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");


  std::string njetsVar = (use_centralJets) ? "njets_InsideEtaCut" : "njets";


  //TH1D* h1_nJetsCentral_thq       = new TH1D( "nJetsCentral_thq", "", 9, -0.5, 8.5 );
  TH1D* h1_nJets_thq              = new TH1D( "nJets_thq", "", 11, -0.5, 10.5 );
  TH1D* h1_eta_qJet_thq           = new TH1D( "eta_qJet_thq", "", 30, 0., 5. );
  TH1D* h1_mt_top_thq             = new TH1D( "mt_top_thq", "", 50, 0., 1000. );
  TH1D* h1_lept_charge_thq        = new TH1D( "lept_charge_thq", "", 3, -1.5, 1.5 );
  TH1D* h1_deltaEta_lept_qJet_thq = new TH1D( "deltaEta_lept_qJet_thq", "", 35, 0., 7. );


  //tree_thq->Project("nJetsCentral_thq", "njets_InsideEtaCut", "weight*( category==11 )" );
  tree_thq->Project("nJets_thq", njetsVar.c_str(), "weight*( category==11 )" );
  tree_thq->Project("eta_qJet_thq", "qJetEta", "weight*( category==11 )" );
  tree_thq->Project("mt_top_thq", "topMt", "weight*( category==11 )" );
  tree_thq->Project("lept_charge_thq", "lept_charge", "weight*( category==11 )" );
  tree_thq->Project("deltaEta_lept_qJet_thq", "deltaEta_lept_qJet", "weight*( category==11 )" );


  //h1_nJetsCentral_thq       ->Scale(1./h1_nJetsCentral_thq->Integral());
  h1_nJets_thq              ->Scale(1./h1_nJets_thq->Integral());
  h1_eta_qJet_thq           ->Scale(1./h1_eta_qJet_thq->Integral());
  h1_mt_top_thq             ->Scale(1./h1_mt_top_thq->Integral());
  h1_lept_charge_thq        ->Scale(1./h1_lept_charge_thq->Integral());
  h1_deltaEta_lept_qJet_thq ->Scale(1./h1_deltaEta_lept_qJet_thq->Integral());



  //TH1D* h1_nJetsCentral_tth       = new TH1D( "nJetsCentral_tth", "", 9, -0.5, 8.5 );
  TH1D* h1_nJets_tth              = new TH1D( "nJets_tth", "", 11, -0.5, 10.5 );
  TH1D* h1_eta_qJet_tth           = new TH1D( "eta_qJet_tth", "", 30, 0., 5. );
  TH1D* h1_mt_top_tth             = new TH1D( "mt_top_tth", "", 50, 0., 1000. );
  TH1D* h1_lept_charge_tth        = new TH1D( "lept_charge_tth", "", 3, -1.5, 1.5 );
  TH1D* h1_deltaEta_lept_qJet_tth = new TH1D( "deltaEta_lept_qJet_tth", "", 35, 0., 7. );


  //tree_tth->Project("nJetsCentral_tth", "njets_InsideEtaCut", "weight*( category==11 )" );
  tree_tth->Project("nJets_tth", njetsVar.c_str(), "weight*( category==11 )" );
  tree_tth->Project("eta_qJet_tth", "qJetEta", "weight*( category==11 )" );
  tree_tth->Project("mt_top_tth", "topMt", "weight*( category==11 )" );
  tree_tth->Project("lept_charge_tth", "lept_charge", "weight*( category==11 )" );
  tree_tth->Project("deltaEta_lept_qJet_tth", "deltaEta_lept_qJet", "weight*( category==11 )" );


  //h1_nJetsCentral_tth              ->Scale(1./h1_nJetsCentral_tth->Integral());
  h1_nJets_tth              ->Scale(1./h1_nJets_tth->Integral());
  h1_eta_qJet_tth           ->Scale(1./h1_eta_qJet_tth->Integral());
  h1_mt_top_tth             ->Scale(1./h1_mt_top_tth->Integral());
  h1_lept_charge_tth        ->Scale(1./h1_lept_charge_tth->Integral());
  h1_deltaEta_lept_qJet_tth ->Scale(1./h1_deltaEta_lept_qJet_tth->Integral());


  std::string outFileName = "tHqLD_leptonic_PDFs";
  if( use_centralJets ) outFileName = outFileName + "_central";
  outFileName += ".root";
  if( argc>1 ) {
    std::string outFileName_str(argv[1]);
    outFileName = outFileName_str;
  }
  std::cout << "-> Writing PDFs in file: " << outFileName << std::endl;
  TFile* outfile = TFile::Open(outFileName.c_str(), "recreate");
  outfile->cd();

  h1_nJets_thq              ->Write();
  h1_eta_qJet_thq           ->Write();
  h1_mt_top_thq             ->Write();
  h1_lept_charge_thq        ->Write();
  h1_deltaEta_lept_qJet_thq ->Write();

  h1_nJets_tth              ->Write();
  h1_eta_qJet_tth           ->Write();
  h1_mt_top_tth             ->Write();
  h1_lept_charge_tth        ->Write();
  h1_deltaEta_lept_qJet_tth ->Write();

  outfile->Close();

  return 0;

}
