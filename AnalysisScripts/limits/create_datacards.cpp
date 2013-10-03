#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"



void createSingleDatacard( const std::string& channel, const std::string& additionalSelection, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_tth, const std::string& suffix="" );


int main() {


  TFile* file_thqLept = TFile::Open("../histograms_CMS-HGG_thqLeptonic.root");
  TFile* file_thqHadr = TFile::Open("../histograms_CMS-HGG_thqHadronic.root");
  TFile* file_tth     = TFile::Open("../histograms_CMS-HGG_tth.root");

  TTree* tree_thqLept = (TTree*)file_thqLept->Get("thqLeptonic_m125_8TeV");
  TTree* tree_thqHadr = (TTree*)file_thqHadr->Get("thqHadronic_m125_8TeV");
  TTree* tree_tth     = (TTree*)file_tth    ->Get("tth_m125_8TeV");


  createSingleDatacard( "leptonic", "thqLD_lept>0.25", tree_thqLept, tree_thqHadr, tree_tth );
  createSingleDatacard( "hadronic", "", tree_thqLept, tree_thqHadr, tree_tth );

  return 0;

}



void createSingleDatacard( const std::string& channel, const std::string& additionalSelection, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_tth, const std::string& suffix ) {


  int category=-1;
  if( channel=="leptonic" ) {
    category=11;
  } else if( channel=="hadronic" ) {
    category=12;
  } else {
    std::cout << "-> Only 'leptonic' and 'hadronic' are supported channels. Exiting." << std::endl;
    exit(11111);
  }

  

  float massWindow = 3.;



  char fullSelection[500];
  if( additionalSelection!="" )
    sprintf( fullSelection, "weight*( category==%d && %s && PhotonsMass>%f && PhotonsMass<%f )", category, additionalSelection.c_str(), 125.-massWindow, 125.+massWindow );
  else
    sprintf( fullSelection, "weight*( category==%d &&       PhotonsMass>%f && PhotonsMass<%f )", category, 125.-massWindow, 125.+massWindow );


  TH1D* h1_mgg_thqLeptonic = new TH1D("mgg_thqLeptonic", "", 160, 100., 180.);
  TH1D* h1_mgg_thqHadronic = new TH1D("mgg_thqHadronic", "", 160, 100., 180.);
  TH1D* h1_mgg_tth         = new TH1D("mgg_tth", "", 160, 100., 180.);

  h1_mgg_thqLeptonic->Sumw2();
  h1_mgg_thqHadronic->Sumw2();
  h1_mgg_tth->Sumw2();

  tree_thqLeptonic->Project( "mgg_thqLeptonic", "PhotonsMass", fullSelection );
  tree_thqHadronic->Project( "mgg_thqHadronic", "PhotonsMass", fullSelection );
  tree_tth->Project( "mgg_tth", "PhotonsMass", fullSelection );

  float yield_thqLeptonic = h1_mgg_thqLeptonic->Integral();
  float yield_thqHadronic = h1_mgg_thqHadronic->Integral();
  float yield_tth = h1_mgg_tth->Integral();

  float yield_signal = yield_thqLeptonic + yield_thqHadronic;
  float yield_SMH = yield_tth;
  if( channel=="leptonic" ) { // quick fix for now
    yield_SMH += 0.01;
  } else if( suffix=="hadronic" ) {
    yield_SMH *= 2.;
  }


  float k_BG = massWindow*2./60.;


  char datacardName[300];
  sprintf(datacardName, "datacard_thq_%s.txt", channel.c_str() );
  std::ofstream datacard(datacardName);

 
  datacard << "imax 1" << std::endl;
  datacard << "jmax 2 " << std::endl;
  datacard << "kmax *" << std::endl;
  datacard << "    " << std::endl;
  datacard << "    " << std::endl;
  datacard << "bin  1" << std::endl;
  datacard << "observation 0" << std::endl;
  datacard << "" << std::endl;
  datacard << "    " << std::endl;
  datacard << "bin            1        1       1"       << std::endl;
  datacard << "process        s        b       bSMH"    << std::endl;
  datacard << "process        0        1       2"       << std::endl;
  datacard << "rate   " << yield_signal <<  "  0. "   <<  yield_SMH   << std::endl;
  datacard << "      " << std::endl;
  datacard << "      " << std::endl;
  datacard << "#syst " << std::endl;
  datacard << "lumi       lnN    1.044 - 1.044" << std::endl;

  datacard << "bg         gmN 0    - " << k_BG << "   -" << std::endl;

  datacard << "PDF_ttH   lnN    -     -         0.922/1.078" << std::endl;
  datacard << "QCDscale_ttH   lnN    -     -    0.86/1.11" << std::endl;

  datacard << "JEC            lnN    -     -    1.0226" << std::endl;
  datacard << "JER            lnN    -     -    1.0027" << std::endl;
  datacard << "Btag           lnN    -     -    1.01" << std::endl;
  if( channel=="leptonic" )
    datacard << "leptEff        lnN    -     -    1.025" << std::endl;
  

  datacard.close();


  delete h1_mgg_thqLeptonic;
  delete h1_mgg_thqHadronic;
  delete h1_mgg_tth        ;

}


