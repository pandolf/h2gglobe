#include <iostream>
#include <fstream>
#include <cmath>

#include "TTree.h"
#include "TFile.h"
#include "TH1D.h"




void computeSingleSyst( const std::string& batchProd, TFile* weightsFile, const std::string& dataset );
float getWeight( float qJetPt, TH1D* h1_weights );


int main( int argc, char* argv[] ) {

  std::string batchProd = "preNatal_v1";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }


  std::cout << "-> Batch prod: " << batchProd << std::endl;

  TFile* weightsFile = TFile::Open("../../AnalysisScripts/aux/JetIdScaleFactor_ZmumuJets_legacypaper.root");


  computeSingleSyst( batchProd, weightsFile, "thqLeptonic_m125_8TeV" );
  computeSingleSyst( batchProd, weightsFile, "tth_m125_8TeV" );
  computeSingleSyst( batchProd, weightsFile, "wzh_m125_8TeV" );

  return 0;

}


void computeSingleSyst( const std::string& batchProd, TFile* weightsFile, const std::string& dataset ) {

  TFile* file = TFile::Open(Form("dbFiles_%s/%s.root", batchProd.c_str(), dataset.c_str()));

  TTree* tree = (TTree*)file->Get("tree_passedEvents");

  Int_t category;
  tree->SetBranchAddress("category", &category);
  
  Int_t itype;
  tree->SetBranchAddress("itype", &itype);
  
  Float_t mgg;
  tree->SetBranchAddress("PhotonsMass", &mgg);

  Float_t weight;
  tree->SetBranchAddress("weight", &weight);

  Float_t qJetEta;
  tree->SetBranchAddress("qJetEta", &qJetEta);

  Float_t qJetPt;
  tree->SetBranchAddress("qJetPt", &qJetPt);

  Float_t thqLD_lept;
  tree->SetBranchAddress("thqLD_lept", &thqLD_lept);



  TH1D* h1_weightsTK = (TH1D*)weightsFile->Get("hJetIdScaleFactor_TK");
  TH1D* h1_weightsHEin = (TH1D*)weightsFile->Get("hJetIdScaleFactor_HEin");
  TH1D* h1_weightsHEout = (TH1D*)weightsFile->Get("hJetIdScaleFactor_HEout");
  TH1D* h1_weightsHF = (TH1D*)weightsFile->Get("hJetIdScaleFactor_HF");

  int nentries = tree->GetEntries();

  float yield = 0.;
  float yieldPU = 0.;

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);

    if( category!=11 ) continue;
    if( thqLD_lept<0.25 ) continue;
    if( mgg<122. || mgg>128. ) continue;

    yield += weight;

    float puweight = 1.;
    if( fabs(qJetEta)<2.5 )
      puweight = getWeight( qJetPt, h1_weightsTK );
    else if( fabs(qJetEta)<2.75 )
      puweight = getWeight( qJetPt, h1_weightsHEin );
    else if( fabs(qJetEta)<3.0 )
      puweight = getWeight( qJetPt, h1_weightsHEout );
    else 
      puweight = getWeight( qJetPt, h1_weightsHF );

    yieldPU += weight*puweight;

  } 

  std::cout << std::endl;
  std::cout << "-> Dataset: " << dataset << std::endl;
  std::cout << "yield: " << yield << std::endl;
  std::cout << "yieldPU: " << yieldPU << std::endl;
  std::cout << "syst: " << fabs(yield-yieldPU)/yield << std::endl;

}




float getWeight( float qJetPt, TH1D* h1_weights ) {

  int bin = h1_weights->FindBin(qJetPt);
  if( bin>h1_weights->GetNbinsX() ) return 1.;

  return h1_weights->GetBinContent(bin);

}
