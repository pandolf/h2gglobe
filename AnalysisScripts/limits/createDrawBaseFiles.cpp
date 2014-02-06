#include <iostream>

#include "TFile.h"
#include "TTree.h"




bool BLINDED=true;


void createSingleFile( int minCat, int maxCat, const std::string& dataset, const std::string& saveName, TFile* file, const std::string& outdir );


int main( int argc, char* argv[] ) {


  std::string batchProd = "preNatal_v5";
  if( argc > 1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }



  std::string outdir = "dbFiles_" + batchProd;
  std::string mkdir_command = "mkdir -p " + outdir;
  system( mkdir_command.c_str() );
  std::cout << "-> Will save files in: " << outdir << std::endl;

  std::string fileName = "../batchOutput_" + batchProd + "/histograms_CMS-HGG.root";
  TFile* file = TFile::Open( fileName.c_str() );
  std::cout << "-> Opening file: " << fileName << std::endl;


  std::vector<std::string> datasets;
  datasets.push_back("thqLeptonic_m125_8TeV");
  datasets.push_back("tth_m125_8TeV");
  datasets.push_back("wzh_m125_8TeV");
  datasets.push_back("tgg");
  datasets.push_back("ttgg");


  for( unsigned i=0; i<datasets.size(); ++i )
    if( file!=0 ) createSingleFile( 11, 12, datasets[i], datasets[i], file, outdir );


  std::vector<std::string> datasets2;
  datasets2.push_back("diphojet_sherpa_8TeV");
  datasets2.push_back("gjet_20_8TeV_pf");
  datasets2.push_back("gjet_40_8TeV_pf");
  datasets2.push_back("qcd_30_8TeV_ff");
  datasets2.push_back("qcd_30_8TeV_pf");
  datasets2.push_back("qcd_40_8TeV_ff");
  datasets2.push_back("qcd_40_8TeV_pf");
  datasets2.push_back("TTJets");

  std::string fileName2 = "../batchOutput_" + batchProd + "/csNominal/histograms_CMS-HGG.root";
  TFile* file2 = TFile::Open( fileName2.c_str() );
  std::cout << "-> Opening file: " << fileName2 << std::endl;


  for( unsigned i=0; i<datasets2.size(); ++i )
    if( file2!=0 )  createSingleFile( 11, 12, datasets2[i], datasets2[i], file2, outdir );


  if( file!=0 ) createSingleFile( 11, 12, "Data", "data", file, outdir );
  if( file!=0 ) createSingleFile( 0, 10, "Data", "data_inclusive", file, outdir );


  std::string fileNameCS = "../batchOutput_" + batchProd + "/btagZero/histograms_CMS-HGG.root";
  TFile* fileCS = TFile::Open( fileNameCS.c_str() );
  std::cout << "-> Opening file: " << fileNameCS << std::endl;
  if( fileCS!=0 ) createSingleFile( 11, 12, "Data", "dataCS_btagZero", fileCS, outdir );
  if( fileCS!=0 ) createSingleFile( 11, 12, "diphojet_sherpa_8TeV", "diphojet_sherpa_8TeVCS_btagZero", fileCS, outdir );

  return 0;

}


void createSingleFile( int minCat, int maxCat, const std::string& dataset, const std::string& saveName, TFile* file, const std::string& outdir ) {

  std::cout << "-> Creating file for dataset: " << dataset << std::endl;

  std::string treeName = dataset;
  TTree* oldTree = (TTree*)file->Get(treeName.c_str());

  if( oldTree==0 ) {
    std::cout << "WARNING! Didn't find dataset: " << dataset << ". Skipping" << std::endl;
    return;
  }

  Int_t category;
  oldTree->SetBranchAddress("category", &category);

  Int_t itype;
  oldTree->SetBranchAddress("itype", &itype);

  Float_t mgg;
  oldTree->SetBranchAddress("PhotonsMass", &mgg);

  Float_t weight;
  oldTree->SetBranchAddress("weight", &weight);


  float lumi = 19715.;


  std::string newFileName = outdir + "/" + saveName + ".root";
  TFile* newFile = TFile::Open(newFileName.c_str(), "recreate");
  newFile->cd();

  TTree* newTree = oldTree->CloneTree(0);
  newTree->SetName("tree_passedEvents");

  Float_t dbWeight;
  newTree->Branch( "dbWeight", &dbWeight, "dbWeight/F" );


  int nentries=oldTree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    oldTree->GetEntry(iEntry);

    if( category<minCat || category>maxCat ) continue;
    if( BLINDED && itype==0 ) { // blinding: for data only
      if( mgg>115. && mgg<135. ) continue; 
    }

    dbWeight = (itype==0) ? 1. : weight/lumi;   

    newTree->Fill();

  }

  newFile->cd();
  newTree->Write();
  newFile->Close();

}
