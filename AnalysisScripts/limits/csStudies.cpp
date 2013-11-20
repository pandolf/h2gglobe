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


void drawPlotsCS( DrawBase* db, const std::string& dataset, TFile* file_invID, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID );
TF1* fitHisto( TH1D* h1, int lineColor, int lineStyle=1 );
void compareVariableCS( DrawBase* db, const std::string& dataset, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, TFile* file_invID, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID );


int main() {


  DrawBase* db_data = new DrawBase("csStudies");
  db_data->set_outputdir("csStudiesPlots");
  db_data->set_lumi(19700.);
  TFile* dummyFile = TFile::Open("dummy.root", "recreate");
  db_data->add_dataFile(dummyFile, "Data");

  DrawBase* db = new DrawBase("csStudies");
  db->set_outputdir("csStudiesPlots");

  TFile* file_nominal        = TFile::Open("../batchOutputBG2/histograms_CMS-HGG.root");
  TFile* file_invID_data     = TFile::Open("../batchOutputCS1/histograms_CMS-HGG.root");
  TFile* file_invID          = TFile::Open("../batchOutputCS3/histograms_CMS-HGG.root");
  TFile* file_btagZero       = TFile::Open("../thqCS_v2/btagZero/histograms_CMS-HGG.root");
  TFile* file_btagLoose      = TFile::Open("../thqCS_v2/btagLoose/histograms_CMS-HGG.root");
  TFile* file_btagZeroInvID  = TFile::Open("../thqCS_v2/btagZeroInvID/histograms_CMS-HGG.root");
  TFile* file_btagLooseInvID = TFile::Open("../thqCS_v2/btagLooseInvID/histograms_CMS-HGG.root");


  compareVariableCS( db_data, "Data", "ph1_pt", 25, 25., 225., "Lead Photon p_{T}", "GeV", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  compareVariableCS( db_data, "Data", "ph2_pt", 25, 25., 125., "Sublead Photon p_{T}", "GeV", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  compareVariableCS( db_data, "Data", "ph1_eta", 25, -2.5, 2.5, "Lead Pseudorapidity", "", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  compareVariableCS( db_data, "Data", "ph2_eta", 25, -2.5, 2.5, "Sublead Pseudorapidity", "", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );

  compareVariableCS( db, "diphojet_sherpa_8TeV", "ph1_pt", 25, 25., 225., "Lead Photon p_{T}", "GeV", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  compareVariableCS( db, "diphojet_sherpa_8TeV", "ph2_pt", 25, 25., 125., "Sublead Photon p_{T}", "GeV", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  compareVariableCS( db, "diphojet_sherpa_8TeV", "ph1_eta", 25, -2.5, 2.5, "Lead Pseudorapidity", "", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  compareVariableCS( db, "diphojet_sherpa_8TeV", "ph2_eta", 25, -2.5, 2.5, "Sublead Pseudorapidity", "", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );

  drawPlotsCS( db_data,            "Data", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
  drawPlotsCS( db, "diphojet_sherpa_8TeV", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );

  return 0;

}




void drawPlotsCS( DrawBase* db, const std::string& dataset, TFile* file_invID, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID ) {


  TTree* tree_invID           = (TTree*)file_invID->Get(dataset.c_str());
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

  bool isData = dataset=="Data";

  if( BLINDED && isData ) {
    tree_invID->Project( "mgg_invID", "PhotonsMass", fullSelection_sidebands );
    tree_btagLoose->Project( "mgg_btagLoose", "PhotonsMass", fullSelection_sidebands );
    tree_btagZero->Project( "mgg_btagZero", "PhotonsMass", fullSelection_sidebands );
    tree_btagLooseInvID->Project( "mgg_btagLooseInvID", "PhotonsMass", fullSelection_sidebands );
    tree_btagZeroInvID->Project( "mgg_btagZeroInvID", "PhotonsMass", fullSelection_sidebands );
  } else {
    tree_invID->Project( "mgg_invID", "PhotonsMass", fullSelection );
    tree_btagLoose->Project( "mgg_btagLoose", "PhotonsMass", fullSelection );
    tree_btagZero->Project( "mgg_btagZero", "PhotonsMass", fullSelection );
    tree_btagLooseInvID->Project( "mgg_btagLooseInvID", "PhotonsMass", fullSelection );
    tree_btagZeroInvID->Project( "mgg_btagZeroInvID", "PhotonsMass", fullSelection );
    //std::cout << "NOPE" << std::endl;
    //exit(11);
  }

  std::cout << "Sideband Events: " << std::endl;
  std::cout << "InvID: " << h1_mgg_invID->GetEntries() << std::endl;
  std::cout << "btagLoose: " << h1_mgg_btagLoose->GetEntries() << std::endl;
  std::cout << "btagZero: " << h1_mgg_btagZero->GetEntries() << std::endl;
  std::cout << "btagLooseInvID: " << h1_mgg_btagLooseInvID->GetEntries() << std::endl;
  std::cout << "btagZeroInvID: " << h1_mgg_btagZeroInvID->GetEntries() << std::endl;

  TF1* f1_invID = fitHisto( h1_mgg_invID, kRed, 1 );
  TF1* f1_btagLoose = fitHisto( h1_mgg_btagLoose, kBlack, 1 );
  TF1* f1_btagZero = fitHisto( h1_mgg_btagZero, 29, 1 );
  TF1* f1_btagLooseInvID = fitHisto( h1_mgg_btagLooseInvID, 39, 2 );
  TF1* f1_btagZeroInvID = fitHisto( h1_mgg_btagZeroInvID, kRed+1, 2 );

  TH1D* h1_band_invID          = fitTools::getBand( f1_invID, "invID" );
  TH1D* h1_band_btagLoose      = fitTools::getBand( f1_btagLoose, "btagLoose" );
  TH1D* h1_band_btagZero       = fitTools::getBand( f1_btagZero, "btagZero" );
  TH1D* h1_band_btagLooseInvID = fitTools::getBand( f1_btagLooseInvID, "btagLooseInvID" );
  TH1D* h1_band_btagZeroInvID  = fitTools::getBand( f1_btagZeroInvID, "btagZeroInvID" );

  h1_band_invID          ->SetFillStyle(3003);
  h1_band_btagLoose      ->SetFillStyle(3003);
  h1_band_btagZero       ->SetFillStyle(3003);
  h1_band_btagLooseInvID ->SetFillStyle(3003);
  h1_band_btagZeroInvID  ->SetFillStyle(3003);

  h1_band_invID          ->SetFillColor(kRed);
  h1_band_btagLoose      ->SetFillColor(kGray);
  h1_band_btagZero       ->SetFillColor(29);
  h1_band_btagLooseInvID ->SetFillColor(39);
  h1_band_btagZeroInvID  ->SetFillColor(kRed+1);

  h1_band_invID          ->SetLineColor(kRed);
  h1_band_btagLoose      ->SetLineColor(kGray);
  h1_band_btagZero       ->SetLineColor(29);
  h1_band_btagLooseInvID ->SetLineColor(39);
  h1_band_btagZeroInvID  ->SetLineColor(kRed+1);

  
  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 100., 180., 10, 0., 0.06);
  h2_axes->SetXTitle( "Diphoton Mass [GeV]");
  h2_axes->SetYTitle( "Normalized to Unity");

  h2_axes->Draw();

  char text_invID[300];
  sprintf( text_invID, "InvID (%.0f evts)", h1_mgg_invID->GetEntries() );
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
  legend->AddEntry( f1_invID, text_invID, "L" );
  legend->AddEntry( f1_btagLoose, text_btagLoose, "L" );
  legend->AddEntry( f1_btagZero, text_btagZero, "L" );
  legend->AddEntry( f1_btagLooseInvID, text_btagLooseInvID, "L" );
  legend->AddEntry( f1_btagZeroInvID, text_btagZeroInvID, "L" );
  legend->Draw("same");

  f1_invID->Draw("l same");
  f1_btagLoose->Draw("l same");
  f1_btagZero->Draw("l same");
  f1_btagLooseInvID->Draw("l same");
  f1_btagZeroInvID->Draw("l same");

  //h1_band_invID          ->Draw("c e3 same");
  //h1_band_btagLoose      ->Draw("c e3 same");
  //h1_band_btagZero       ->Draw("c e3 same");
  //h1_band_btagLooseInvID ->Draw("c e3 same");
  //h1_band_btagZeroInvID  ->Draw("c e3 same");


  h1_mgg_invID->Rebin(10);
  //h1_mgg_btagLoose->Rebin(10);
  //h1_mgg_btagZero->Rebin(10);
  //h1_mgg_btagLooseInvID->Rebin(10);
  //h1_mgg_btagZeroInvID->Rebin(10);

  h1_mgg_invID->Scale(          1./h1_mgg_invID->Integral("width") );
  h1_mgg_btagLoose->Scale(      1./h1_mgg_btagLoose->Integral("width") );
  h1_mgg_btagZero->Scale(       1./h1_mgg_btagZero->Integral("width") );
  h1_mgg_btagLooseInvID->Scale( 1./h1_mgg_btagLooseInvID->Integral("width") );
  h1_mgg_btagZeroInvID->Scale(  1./h1_mgg_btagZeroInvID->Integral("width") );

  //h1_mgg_invID->Draw("p same");
  //h1_mgg_btagLoose->Draw("p same");
  //h1_mgg_btagZero->Draw("p same");
  //h1_mgg_btagLooseInvID->Draw("p same");
  //h1_mgg_btagZeroInvID->Draw("p same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/mgg_" + dataset;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());


  delete c1;
  delete legend;
  delete h2_axes;
  delete h1_mgg_invID;
  delete h1_mgg_btagLoose;
  delete h1_mgg_btagZero;
  delete h1_mgg_btagLooseInvID;
  delete h1_mgg_btagZeroInvID;

}




TF1* fitHisto( TH1D* h1, int lineColor, int lineStyle ) {

  std::string fName(h1->GetName());
  fName = "f1_" + fName;
  TF1* f1 = new TF1(fName.c_str(), "exp([0]*x)", 100., 180.);

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


void compareVariableCS( DrawBase* db, const std::string& dataset, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, TFile* file_invID, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID ) {

  TTree* tree_invID           = (TTree*)file_invID->Get(dataset.c_str());
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


  TH1D* h1_invID           = new TH1D("invID", "", nBins, xMin, xMax);
  TH1D* h1_btagLoose       = new TH1D("btagLoose", "", nBins, xMin, xMax);
  TH1D* h1_btagZero        = new TH1D("btagZero", "", nBins, xMin, xMax);
  TH1D* h1_btagLooseInvID  = new TH1D("btagLooseInvID", "", nBins, xMin, xMax);
  TH1D* h1_btagZeroInvID   = new TH1D("btagZeroInvID", "", nBins, xMin, xMax);

  h1_invID           ->Sumw2();
  h1_btagLoose       ->Sumw2();
  h1_btagZero        ->Sumw2();
  h1_btagLooseInvID  ->Sumw2();
  h1_btagZeroInvID   ->Sumw2();

  bool isData = dataset=="Data";

  if( BLINDED && isData ) {
    tree_invID->Project( "invID", varName.c_str(), fullSelection_sidebands );
    tree_btagLoose->Project( "btagLoose", varName.c_str(), fullSelection_sidebands );
    tree_btagZero->Project( "btagZero", varName.c_str(), fullSelection_sidebands );
    tree_btagLooseInvID->Project( "btagLooseInvID", varName.c_str(), fullSelection_sidebands );
    tree_btagZeroInvID->Project( "btagZeroInvID", varName.c_str(), fullSelection_sidebands );
  } else {
    tree_invID->Project( "invID", varName.c_str(), fullSelection );
    tree_btagLoose->Project( "btagLoose", varName.c_str(), fullSelection );
    tree_btagZero->Project( "btagZero", varName.c_str(), fullSelection );
    tree_btagLooseInvID->Project( "btagLooseInvID", varName.c_str(), fullSelection );
    tree_btagZeroInvID->Project( "btagZeroInvID", varName.c_str(), fullSelection );
    //std::cout << "NOPE" << std::endl;
    //exit(11);
  }

  h1_invID          ->SetLineWidth(2);
  h1_btagLoose      ->SetLineWidth(2);
  h1_btagZero       ->SetLineWidth(2);
  h1_btagLooseInvID ->SetLineWidth(2);
  h1_btagZeroInvID  ->SetLineWidth(2);

  h1_invID          ->SetLineColor(kRed);
  h1_btagLoose      ->SetLineColor(kGray);
  h1_btagZero       ->SetLineColor(29);
  h1_btagLooseInvID ->SetLineColor(39);
  h1_btagZeroInvID  ->SetLineColor(kRed+1);

  h1_invID          ->Scale(1./h1_invID          ->Integral());
  h1_btagLoose      ->Scale(1./h1_btagLoose      ->Integral());
  h1_btagZero       ->Scale(1./h1_btagZero       ->Integral());
  h1_btagLooseInvID ->Scale(1./h1_btagLooseInvID ->Integral());
  h1_btagZeroInvID  ->Scale(1./h1_btagZeroInvID  ->Integral());


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  float yMax = h1_invID->GetMaximum();
  if( h1_btagLoose     ->GetMaximum()>yMax ) yMax=h1_btagLoose     ->GetMaximum();
  if( h1_btagZero      ->GetMaximum()>yMax ) yMax=h1_btagZero      ->GetMaximum();
  if( h1_btagLooseInvID->GetMaximum()>yMax ) yMax=h1_btagLooseInvID->GetMaximum();
  if( h1_btagZeroInvID ->GetMaximum()>yMax ) yMax=h1_btagZeroInvID ->GetMaximum();
  yMax *= 1.3;


  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., yMax );
  std::string fullAxisName = axisName;
  if( units!="" ) fullAxisName = fullAxisName + " [" + units + "]";
  h2_axes->SetXTitle( fullAxisName.c_str() );
  h2_axes->SetYTitle( "Normalized to Unity");

  h2_axes->Draw();

  char text_invID[300];
  sprintf( text_invID, "InvID (%.0f evts)", h1_invID->GetEntries() );
  char text_btagLoose[300];
  sprintf( text_btagLoose, "CSVL (%.0f evts)", h1_btagLoose->GetEntries() );
  char text_btagZero[300];
  sprintf( text_btagZero, "No Btag (%.0f evts)", h1_btagZero->GetEntries() );
  char text_btagLooseInvID[300];
  sprintf( text_btagLooseInvID, "CSVL + InvID (%.0f evts)", h1_btagLooseInvID->GetEntries() );
  char text_btagZeroInvID[300];
  sprintf( text_btagZeroInvID, "No Btag + InvID (%.0f evts)", h1_btagZeroInvID->GetEntries() );

  TLegend* legend = new TLegend( 0.37, 0.6, 0.9, 0.9, dataset.c_str() );
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  //legend->AddEntry( h1_invID, text_invID, "L" );
  legend->AddEntry( h1_btagLoose, text_btagLoose, "L" );
  legend->AddEntry( h1_btagZero, text_btagZero, "L" );
  legend->AddEntry( h1_btagLooseInvID, text_btagLooseInvID, "L" );
  legend->AddEntry( h1_btagZeroInvID, text_btagZeroInvID, "L" );
  legend->Draw("same");

  //h1_invID->Draw("histo same");
  h1_btagLoose->Draw("histo same");
  h1_btagZero->Draw("histo same");
  h1_btagLooseInvID->Draw("histo same");
  h1_btagZeroInvID->Draw("histo same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/" + varName + "_" + dataset;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());


  delete c1;
  delete legend;
  delete h2_axes;
  delete h1_invID;
  delete h1_btagLoose;
  delete h1_btagZero;
  delete h1_btagLooseInvID;
  delete h1_btagZeroInvID;
  
}

