#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"



void doShapeAnalysis( const std::string& channel );
TFile* createRootFile( const std::string& channel );
void createDatacard( const std::string& channel, TFile* file );







int main() {




  doShapeAnalysis( "leptonic" );



  return 0;

}







void doShapeAnalysis( const std::string& channel ) {

  if( channel!="leptonic" && channel!="hadronic" ) {
    std::cout << "Only 'leptonic' and 'hadronic' are accepted channels. Exiting." << std::endl;
    exit(11);
  }

  std::cout << "-> Creating files for shape analysis in channel: " << channel << std::endl;

  TFile* file_shapes = createRootFile( channel );
  createDatacard( channel, file_shapes );

  file_shapes->Close();

}



TFile* createRootFile( const std::string& channel ) {

  std::cout << "-> Creating rootfile..." << std::endl;

  std::string fileName = "shapes_" + channel + ".root";

  TFile* file_shapes = TFile::Open(fileName.c_str(), "recreate");
  file_shapes->cd();

  int nBins = 160;
  float xMin = 100.;
  float xMax = 180.;


  // BG histogram:

  TH1D* h1_bg = new TH1D("background", "", nBins, xMin, xMax );
  h1_bg->Sumw2();

  for( unsigned ibin=1; ibin<nBins+1; ++ibin )
    h1_bg->SetBinContent(ibin,1.);

  float bg_norm = 0.5; // two events is a reasonable estimate for both hadronic and leptonic channels

  h1_bg->Scale(bg_norm/h1_bg->Integral());
  h1_bg->Write();



  // signal histogram:
  int category=-1;
  if( channel=="leptonic" ) {
    category=11;
  } else if( channel=="hadronic" ) {
    category=12;
  } else {
    std::cout << "-> Only 'leptonic' and 'hadronic' are supported channels. Exiting." << std::endl;
    exit(11111);
  }


  TFile* file_thqLept = TFile::Open("../histograms_CMS-HGG_thqLeptonic.root");
  TFile* file_thqHadr = TFile::Open("../histograms_CMS-HGG_thqHadronic.root");
  TFile* file_tth     = TFile::Open("../histograms_CMS-HGG_tth.root");

  TTree* tree_thqLept = (TTree*)file_thqLept->Get("thqLeptonic_m125_8TeV");
  TTree* tree_thqHadr = (TTree*)file_thqHadr->Get("thqHadronic_m125_8TeV");
  TTree* tree_tth     = (TTree*)file_tth    ->Get("tth_m125_8TeV");

  char fullSelection[500];
  sprintf( fullSelection, "evweight*( category==%d )", category );


  TH1D* h1_mgg_thqLeptonic = new TH1D("mgg_thqLeptonic", "", nBins, xMin, xMax );
  TH1D* h1_mgg_thqHadronic = new TH1D("mgg_thqHadronic", "", nBins, xMin, xMax );

  h1_mgg_thqLeptonic->Sumw2();
  h1_mgg_thqHadronic->Sumw2();

  tree_thqLept->Project( "mgg_thqLeptonic", "PhotonsMass", fullSelection );
  tree_thqHadr->Project( "mgg_thqHadronic", "PhotonsMass", fullSelection );

  TH1D* h1_signal = new TH1D(*h1_mgg_thqLeptonic);
  h1_signal->Add(h1_mgg_thqHadronic);
  h1_signal->SetName("signal");

  file_shapes->cd();

  h1_signal->Write();



  // data histogram (dummy)

  TH1D* h1_data = new TH1D("data_obs", "", nBins, xMin, xMax );

  h1_data->Fill( 125. );
  h1_data->Fill( 126. );

  file_shapes->cd();
  h1_data->Write();

  //file_shapes->Close();

  return file_shapes;

}



void createDatacard( const std::string& channel, TFile* file ) {

  std::cout << "-> Creating datacard..." << std::endl;

  TH1D* h1_signal = (TH1D*)file->Get("signal"); 
  TH1D* h1_background = (TH1D*)file->Get("background"); 
  TH1D* h1_data = (TH1D*)file->Get("data_obs"); 


  float yield_signal = h1_signal->Integral();
  float yield_bg = h1_background->Integral();




  char datacardName[300];
  sprintf(datacardName, "datacard_shapes_thq_%s.txt", channel.c_str() );
  std::ofstream datacard(datacardName);


  datacard << "imax 1" << std::endl;
  datacard << "jmax 1" << std::endl;
  datacard << "kmax *" << std::endl;
  datacard << "---------------" << std::endl;
  datacard << "shapes * * shapes_" + channel + ".root $PROCESS $PROCESS_$SYSTEMATIC" << std::endl;
  datacard << "---------------" << std::endl;
  datacard << "bin 1" << std::endl;
  datacard << "observation " << h1_data->Integral() << std::endl;
  datacard << "------------------------------" << std::endl;
  datacard << "bin             1          1" << std::endl;
  datacard << "process         signal     background" << std::endl;
  datacard << "process         0          1" << std::endl;
  datacard << "rate    " << yield_signal << "        " << yield_bg << std::endl;
  datacard << "--------------------------------" << std::endl;
  datacard << "lumi     lnN    1.044       1.044" << std::endl;
  datacard << "bg_norm unif -1 1   -       1." << std::endl;
  //datacard << "alpha  shape    -           1   uncertainty on background shape and normalization" << std::endl;
  //datacard << "sigma  shape    0.5         -   uncertainty on signal resolution. Assume the histogram is a 2 sigma shift, " << std::endl;

  datacard.close();

}
