#include <iostream>

#include "TCanvas.h"
#include "TH2D.h"
#include "TF1.h"
#include "TFile.h"
#include "TTree.h"
#include "TGraph.h"


float getScalingFactor(TF1* f1, float deltaM);
TF1* getExpoFromFit( const std::string& fileName );

int main() {


  TF1* flat = new TF1("uniform", "[0]", 10.0, 180.);
  flat->SetParameter(0, 1./80.);

  TF1* expo = getExpoFromFit("../batchOutput8/histograms_CMS-HGG.root");
  //TF1* expo = new TF1("expo", "exp([0]+[1]*x)", 100., 180.);
  //expo->SetParameter(0, 8. );
  //expo->SetParameter(1, -0.1 );

  float sf_flat = getScalingFactor(flat, 3.);
  float sf_expo = getScalingFactor(expo, 3.);

  std::cout << "SF: " << std::endl;
  std::cout << "flat: " << sf_flat << std::endl;
  std::cout << "expo: " << sf_expo << std::endl;


  return 0;

}


float getScalingFactor(TF1* f1, float deltaM) {

  float s1 = f1->Integral( 100., 125.-deltaM );
  float s2 = f1->Integral( 125.+deltaM, 180.);

  float sR = f1->Integral( 125.-deltaM, 125.+deltaM);

  float alpha = sR/(s1+s2);

  return alpha;

}


TF1* getExpoFromFit( const std::string& fileName ) {

  TFile* file = TFile::Open(fileName.c_str());
  TTree* tree = (TTree*)file->Get("Data");

  int category;
  float PhotonsMass;
  float thqLD_lept;

  tree->SetBranchAddress( "category", &category );
  tree->SetBranchAddress( "PhotonsMass", &PhotonsMass );
  tree->SetBranchAddress( "thqLD_lept", &thqLD_lept );

  int nentries=tree->GetEntries();
  TH1D* h1_data = new TH1D( "data", "", 40, 100., 180.);

  for( unsigned ientry=0; ientry<nentries; ++ientry ) {

    tree->GetEntry(ientry);

    if( category!=11 ) continue;
    if( PhotonsMass>115. && PhotonsMass<135. ) continue;
    if( thqLD_lept>0.25 ) continue; //inverted LD cut

    h1_data->Fill(PhotonsMass);

  }

  TF1* expo = new TF1("expo", "exp([0]+[1]*x)", 100., 180.);
  h1_data->Fit(expo, "RL");

  h1_data->SetMarkerStyle(20);
  h1_data->SetMarkerSize(2.);

  float integral = expo->Integral(100., 180.);
  float c_value = integral/80.;

  TF1* unif = new TF1("unif", "[0]", 100., 180.);
  unif->SetParameter( 0, c_value );
  unif->SetLineColor( kMagenta );
  unif->SetLineStyle( 2 );

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, 100., 180., 10, 0., 2.);
  
  h2_axes->Draw();
  h1_data->Draw("P same");
  unif->Draw("same");

  c1->SaveAs("fitData.eps");

  return expo;

}
