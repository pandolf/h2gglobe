#include <iostream>

#include "TFile.h"
#include "TTree.h"




void createSingleFile( const std::string& dataset, TFile* file, const std::string& outdir );


int main( int argc, char* argv[] ) {


  std::string batchProd = "qJetEtaFix_newLD_v1";
  if( argc > 1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }


  std::string fileName = "../batchOutput_" + batchProd + "/histograms_CMS-HGG.root";
  TFile* file = TFile::Open( fileName.c_str() );


  std::string outdir = "dbFiles_" + batchProd;
  std::string mkdir_command = "mkdir -p " + outdir;
  system( mkdir_command.c_str() );

  std::cout << "-> Opening file: " << fileName << std::endl;

  std::vector<std::string> datasets;
  datasets.push_back("thqLeptonic_m125_8TeV");
  datasets.push_back("tth_m125_8TeV");
  datasets.push_back("wzh_m125_8TeV");


  for( unsigned i=0; i<datasets.size(); ++i )
    createSingleFile( datasets[i], file, outdir );


  return 0;

}


void createSingleFile( const std::string& dataset, TFile* file, const std::string& outdir ) {

  std::cout << "-> Creating file for dataset: " << dataset << std::endl;

  std::string treeName = dataset;
  TTree* oldTree = (TTree*)file->Get(treeName.c_str());

  Int_t category;
  oldTree->SetBranchAddress("category", &category);


  std::string newFileName = outdir + "/" + dataset + ".root";
  TFile* newFile = TFile::Open(newFileName.c_str(), "recreate");
  newFile->cd();

  TTree* newTree = oldTree->CloneTree(0);
  newTree->SetName("tree_passedEvents");


  int nentries=oldTree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    oldTree->GetEntry(iEntry);

    if( category!=11 && category!=12 ) continue;

    newTree->Fill();

  }

  newFile->cd();
  newTree->Write();
  newFile->Close();

}
