#include <iostream>
#include <cmath>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"



void compareOneDataset( const std::string& dataset, TFile* file_noJER, TFile* file_JER, TFile* file_JERplus, TFile* file_JERminus );


int main() {


  TFile* file_noJER    = TFile::Open("../batchOutput8/histograms_CMS-HGG.root");
  TFile* file_JER      = TFile::Open("../thq_jetsyst_v11/jerCentral/histograms_CMS-HGG.root");
  TFile* file_JERplus  = TFile::Open("../thq_jetsyst_v11/jerUp/histograms_CMS-HGG.root");
  TFile* file_JERminus = TFile::Open("../thq_jetsyst_v11/jerDown/histograms_CMS-HGG.root");

  compareOneDataset( "thqLeptonic_m125_8TeV", file_noJER, file_JER, file_JERplus, file_JERminus);
  compareOneDataset( "tth_m125_8TeV"        , file_noJER, file_JER, file_JERplus, file_JERminus);
  compareOneDataset( "wzh_m125_8TeV"        , file_noJER, file_JER, file_JERplus, file_JERminus);

  return 0;

}



void compareOneDataset( const std::string& dataset, TFile* file_noJER, TFile* file_JER, TFile* file_JERplus, TFile* file_JERminus ) {


  TTree* tree_noJER    = (TTree*)file_noJER   ->Get(dataset.c_str());
  TTree* tree_JER      = (TTree*)file_JER     ->Get(dataset.c_str());
  TTree* tree_JERplus  = (TTree*)file_JERplus ->Get(dataset.c_str());
  TTree* tree_JERminus = (TTree*)file_JERminus->Get(dataset.c_str());

  
  int category=11;
  char fullSelection[500];
  sprintf( fullSelection,           "evweight*( category==%d       )", category );



  int nBins = 80;
  float xMin = 100.;
  float xMax = 180.;

  TH1D* h1_mgg_noJER    = new TH1D("mgg_noJER"   , "", nBins, xMin, xMax);
  TH1D* h1_mgg_JER      = new TH1D("mgg_JER"     , "", nBins, xMin, xMax);
  TH1D* h1_mgg_JERplus  = new TH1D("mgg_JERplus" , "", nBins, xMin, xMax);
  TH1D* h1_mgg_JERminus = new TH1D("mgg_JERminus", "", nBins, xMin, xMax);

  h1_mgg_noJER    ->Sumw2();
  h1_mgg_JER      ->Sumw2();
  h1_mgg_JERplus  ->Sumw2();
  h1_mgg_JERminus ->Sumw2();

  tree_noJER   ->Project( "mgg_noJER"   , "PhotonsMass", fullSelection );
  tree_JER     ->Project( "mgg_JER"     , "PhotonsMass", fullSelection );
  tree_JERplus ->Project( "mgg_JERplus" , "PhotonsMass", fullSelection );
  tree_JERminus->Project( "mgg_JERminus", "PhotonsMass", fullSelection );


  float noJER    = h1_mgg_noJER->Integral();
  float JER      = h1_mgg_JER->Integral();
  float JERplus  = h1_mgg_JERplus->Integral();
  float JERminus = h1_mgg_JERminus->Integral();

  float relDiffYield = (JER-noJER)/noJER;

  std::cout << std::endl << std::endl;
  std::cout << "@@@  " << dataset << std::endl;
  std::cout << "noJER: "    << noJER << std::endl;
  std::cout << "JER: "      << JER << " (";
  if( relDiffYield>0. ) std::cout << "+";
  std::cout <<  relDiffYield*100. << "%)" << std::endl;
  std::cout << "JERplus: "  << JERplus << std::endl;
  std::cout << "JERminus: " << JERminus << std::endl;

  float diff_noJER_JER = fabs( noJER - JER );
  float diff_noJER_JERplus = fabs( noJER - JERplus );
  float diff_noJER_JERminus = fabs( noJER - JERminus );
  float diff_JER_JERplus = fabs( JER - JERplus );
  float diff_JER_JERminus = fabs( JER - JERminus );

  float syst = diff_noJER_JER;
  if( diff_noJER_JERplus>syst ) syst = diff_noJER_JERplus;
  if( diff_noJER_JERminus>syst ) syst = diff_noJER_JERminus;
 
  std::cout << "++ Current approach: yield = " << noJER << " syst: " << syst/noJER << std::endl;
  
  float systnew = diff_JER_JERplus;
  if( diff_JER_JERminus>syst ) systnew = diff_JER_JERminus;
  float relDiffSyst = (systnew-syst)/syst;
  std::cout << "++ Correct approach: yield = " << JER << " (";
  if( relDiffYield>0. ) std::cout << "+";
  std::cout << relDiffYield*100. << "%)" << " syst: " << systnew/JER << " (";
  if( relDiffSyst>0. ) std::cout << "+";
  std::cout << relDiffSyst*100. << "%)" << std::endl;


  delete h1_mgg_noJER   ;
  delete h1_mgg_JER     ;
  delete h1_mgg_JERplus ;
  delete h1_mgg_JERminus;


}
