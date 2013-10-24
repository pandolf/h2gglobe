#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"



bool BLINDED=true;


void drawPlotsCS( DrawBase* db, const std::string& dataset, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID );
TF1* fitHisto( TH1D* h1, int lineColor, int lineStyle=1 );


int main() {


  DrawBase* db_data = new DrawBase("csStudies");
  db_data->set_outputdir("csStudiesPlots");
  db_data->set_lumi(19700.);
  TFile* dummyFile = TFile::Open("dummy.root", "recreate");
  db_data->add_dataFile(dummyFile, "Data");

  DrawBase* db = new DrawBase("csStudies");
  db->set_outputdir("csStudiesPlots");

  //TFile* file_invID          = TFile::Open("../batchOutputCS1/histograms_CMS-HGG.root");
  TFile* file_btagZero       = TFile::Open("../thqCS_v2/btagZero/histograms_CMS-HGG.root");
  TFile* file_btagLoose      = TFile::Open("../thqCS_v2/btagLoose/histograms_CMS-HGG.root");
  TFile* file_btagZeroInvID  = TFile::Open("../thqCS_v2/btagZeroInvID/histograms_CMS-HGG.root");
  TFile* file_btagLooseInvID = TFile::Open("../thqCS_v2/btagLooseInvID/histograms_CMS-HGG.root");


  drawPlotsCS( db_data, "Data", file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  drawPlotsCS( db, "diphojet_sherpa_8TeV", file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );

  return 0;

}




void drawPlotsCS( DrawBase* db, const std::string& dataset, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID ) {


  TTree* tree_btagZero        = (TTree*)file_btagZero->Get(dataset.c_str());
  TTree* tree_btagLoose       = (TTree*)file_btagLoose->Get(dataset.c_str());
  TTree* tree_btagZeroInvID   = (TTree*)file_btagZeroInvID->Get(dataset.c_str());
  TTree* tree_btagLooseInvID  = (TTree*)file_btagLooseInvID->Get(dataset.c_str());


  int category=11;
  std::string additionalSelection = "thqLD_lept>0.25";
  //std::string additionalSelection = "";

  char fullSelection[500];
  char fullSelection_sidebands[500];
  if( additionalSelection!="" ) {
    sprintf( fullSelection,           "weight*( category==%d && %s )", category, additionalSelection.c_str() );
    sprintf( fullSelection_sidebands, "weight*( category==%d && %s && (PhotonsMass<115. || PhotonsMass>135.) )", category, additionalSelection.c_str() );
  } else {
    sprintf( fullSelection,           "weight*( category==%d       )", category );
    sprintf( fullSelection_sidebands, "weight*( category==%d       && (PhotonsMass<115. || PhotonsMass>135.) )", category );
  }


  int nBins = 80;
  float xMin = 100.;
  float xMax = 180.;

  TH1D* h1_mgg_invID           = new TH1D("mgg_invID", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagLoose       = new TH1D("mgg_btagLoose", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagZero        = new TH1D("mgg_btagZero", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagLooseInvID  = new TH1D("mgg_btagLooseInvID", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagZeroInvID   = new TH1D("mgg_btagZeroInvID", "", nBins, xMin, xMax);

  h1_mgg_invID           ->Sumw2();
  h1_mgg_btagLoose       ->Sumw2();
  h1_mgg_btagZero        ->Sumw2();
  h1_mgg_btagLooseInvID  ->Sumw2();
  h1_mgg_btagZeroInvID   ->Sumw2();

  if( BLINDED ) {
    //tree_invID->Project( "mgg_invID", "PhotonsMass", fullSelection_sidebands );
    tree_btagLoose->Project( "mgg_btagLoose", "PhotonsMass", fullSelection_sidebands );
    tree_btagZero->Project( "mgg_btagZero", "PhotonsMass", fullSelection_sidebands );
    tree_btagLooseInvID->Project( "mgg_btagLooseInvID", "PhotonsMass", fullSelection_sidebands );
    tree_btagZeroInvID->Project( "mgg_btagZeroInvID", "PhotonsMass", fullSelection_sidebands );
  } else {
    std::cout << "NOPE" << std::endl;
    exit(11);
  }

  std::cout << "Sideband Events: " << std::endl;
  //std::cout << "InvID: " << h1_mgg_invID->GetEntries() << std::endl;
  std::cout << "btagLoose: " << h1_mgg_btagLoose->GetEntries() << std::endl;
  std::cout << "btagZero: " << h1_mgg_btagZero->GetEntries() << std::endl;
  std::cout << "btagLooseInvID: " << h1_mgg_btagLooseInvID->GetEntries() << std::endl;
  std::cout << "btagZeroInvID: " << h1_mgg_btagZeroInvID->GetEntries() << std::endl;

  TF1* f1_btagLoose = fitHisto( h1_mgg_btagLoose, kBlack, 1 );
  TF1* f1_btagZero = fitHisto( h1_mgg_btagZero, 29, 1 );
  TF1* f1_btagLooseInvID = fitHisto( h1_mgg_btagLooseInvID, 39, 2 );
  TF1* f1_btagZeroInvID = fitHisto( h1_mgg_btagZeroInvID, kRed+1, 2 );

  
  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 100., 180., 10, 0., 0.06);
  h2_axes->SetXTitle( "Diphoton Mass [GeV]");
  h2_axes->SetYTitle( "Arbitrary Units");
  //h2_axes->SetYTitle( "Events / (1 GeV)");

  h2_axes->Draw();

  char text_btagLoose[300];
  sprintf( text_btagLoose, "CSVL (%.0f evts)", h1_mgg_btagLoose->GetEntries() );
  char text_btagZero[300];
  sprintf( text_btagZero, "No Btag (%.0f evts)", h1_mgg_btagZero->GetEntries() );
  char text_btagLooseInvID[300];
  sprintf( text_btagLooseInvID, "CSVL + InvID (%.0f evts)", h1_mgg_btagLooseInvID->GetEntries() );
  char text_btagZeroInvID[300];
  sprintf( text_btagZeroInvID, "No Btag + InvID (%.0f evts)", h1_mgg_btagZeroInvID->GetEntries() );

  TLegend* legend = new TLegend( 0.37, 0.6, 0.9, 0.9, dataset.c_str() );
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  legend->AddEntry( f1_btagLoose, text_btagLoose, "L" );
  legend->AddEntry( f1_btagZero, text_btagZero, "L" );
  legend->AddEntry( f1_btagLooseInvID, text_btagLooseInvID, "L" );
  legend->AddEntry( f1_btagZeroInvID, text_btagZeroInvID, "L" );
  legend->Draw("same");

  f1_btagLoose->Draw("l same");
  f1_btagZero->Draw("l same");
  f1_btagLooseInvID->Draw("l same");
  f1_btagZeroInvID->Draw("l same");

  //h1_mgg_btagLoose->Draw("p same");
  //h1_mgg_btagZero->Draw("p same");
  //h1_mgg_btagLooseInvID->Draw("p same");
  //h1_mgg_btagZeroInvID->Draw("p same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/mgg_" + dataset + ".eps";
  c1->SaveAs(canvasName.c_str());


  delete c1;
  delete legend;
  delete h2_axes;
  delete h1_mgg_btagLoose;
  delete h1_mgg_btagZero;
  delete h1_mgg_btagLooseInvID;
  delete h1_mgg_btagZeroInvID;

}




TF1* fitHisto( TH1D* h1, int lineColor, int lineStyle ) {

  std::string fName(h1->GetName());
  fName = "f1_" + fName;
  TF1* f1 = new TF1(fName.c_str(), "exp([0]+[1]*x)", 100., 180.);

  f1->SetLineColor(lineColor);
  f1->SetLineStyle(lineStyle);
  f1->SetLineWidth(2);

  h1->Scale(1./h1->Integral());
  h1->SetLineColor(lineColor);
  h1->SetMarkerColor(lineColor);
  h1->SetMarkerStyle(20);

  h1->Fit(f1, "RL");

  return f1;

}
