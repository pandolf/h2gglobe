#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"
#include "TChain.h"

#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooRealVar.h"
#include "RooPlot.h"
#include "RooWorkspace.h"
#include "RooFitResult.h"
#include "RooArgList.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"



bool BLINDED=true;
bool FASAWEIGHTS=true;


struct ValAndErr {

  float val;
  float err;

};


void doPlotForOneSelection( const std::string& saveName, DrawBase* db, DrawBase* db_data, TFile* file_nominal, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID, bool LDsidebands=false, TH2D* h2_fasaWeights_pt=0, TH2D* h2_fasaWeights_eta=0 );
TGraphErrors* drawPlotsCS( DrawBase* db, std::vector<std::string> datasetNames, const std::string& saveName, TFile* file_nominal, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID, bool LDsidebands, TH2D* h2_fasaWeights_pt, TH2D* h2_fasaWeights_eta );
TF1* fitHisto( TH1D* h1, int lineColor, int lineStyle=1 );
//void compareVariableCS( DrawBase* db, const std::string& datasetName, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, TFile* file_invID, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID );
RooDataSet* getDatasetFromTree( const std::string& name, TTree* tree, RooRealVar& mgg_roo, bool LDsidebands=false, TH2D* h2_fasaWeights_pt=0, TH2D* h2_fasaWeights_eta=0 );
void fitTreeWithExponential( DrawBase* db, const std::string& name, TTree* tree, RooWorkspace* w, const std::string& varName, bool LDsidebands=false, TH2D* h2_fasaWeights_pt=0, TH2D* h2_fasaWeights_eta=0 );


int main( int argc, char* argv[] ) {


  std::string batchProd = "preNatal_v1";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }

  std::cout << "-> Using batchProd: " << batchProd << std::endl;

  DrawBase* db_data = new DrawBase("csStudies");
  std::string outputdir = "csStudiesPlots_" + batchProd;
  db_data->set_outputdir(outputdir);
  db_data->set_lumi(19715.);
  TFile* dummyFile = TFile::Open("dummy.root", "recreate");
  db_data->add_dataFile(dummyFile, "Data");

  DrawBase* db = new DrawBase("csStudies");
  db->set_outputdir(Form("csStudiesPlots_%s", batchProd.c_str()));

  TFile* file_nominal        = TFile::Open(Form("../batchOutput_%s/csNominal/histograms_CMS-HGG.root", batchProd.c_str()) );
  TFile* file_btagZero       = TFile::Open(Form("../batchOutput_%s/btagZero/histograms_CMS-HGG.root", batchProd.c_str()) );
  TFile* file_btagLoose      = TFile::Open(Form("../batchOutput_%s/btagLoose/histograms_CMS-HGG.root", batchProd.c_str()) );
  TFile* file_btagZeroInvID  = TFile::Open(Form("../batchOutput_%s/btagZeroInvID/histograms_CMS-HGG.root", batchProd.c_str()) );
  TFile* file_btagLooseInvID = TFile::Open(Form("../batchOutput_%s/btagLooseInvID/histograms_CMS-HGG.root", batchProd.c_str()) );

  //TFile* file_nominal        = TFile::Open(Form("../batchOutput_preselectionLoose_v6/histograms_CMS-HGG.root"));
  //TFile* file_btagZero       = TFile::Open(Form("../batchOutput_preselectionLoose_v6/histograms_CMS-HGG.root"));
  //TFile* file_btagZeroInvID       = TFile::Open(Form("../batchOutput_preselectionLoose_v6/histograms_CMS-HGG.root"));
  //TFile* file_btagLoose       = TFile::Open(Form("../batchOutput_preselectionLoose_v6/histograms_CMS-HGG.root"));
  //TFile* file_btagLooseInvID       = TFile::Open(Form("../batchOutput_preselectionLoose_v6/histograms_CMS-HGG.root"));


//compareVariableCS( db_data, "Data", "ph1_pt", 25, 25., 225., "Lead Photon p_{T}", "GeV", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
//compareVariableCS( db_data, "Data", "ph2_pt", 25, 25., 125., "Sublead Photon p_{T}", "GeV", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
//compareVariableCS( db_data, "Data", "ph1_eta", 25, -2.5, 2.5, "Lead Pseudorapidity", "", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
//compareVariableCS( db_data, "Data", "ph2_eta", 25, -2.5, 2.5, "Sublead Pseudorapidity", "", file_invID_data, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );

//compareVariableCS( db, "diphojet_sherpa_8TeV", "ph1_pt", 25, 25., 225., "Lead Photon p_{T}", "GeV", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
//compareVariableCS( db, "diphojet_sherpa_8TeV", "ph2_pt", 25, 25., 125., "Sublead Photon p_{T}", "GeV", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
//compareVariableCS( db, "diphojet_sherpa_8TeV", "ph1_eta", 25, -2.5, 2.5, "Lead Pseudorapidity", "", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );
//compareVariableCS( db, "diphojet_sherpa_8TeV", "ph2_eta", 25, -2.5, 2.5, "Sublead Pseudorapidity", "", file_invID, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID );


  TFile* file_fasaWeights_pt  = TFile::Open( "/afs/cern.ch/work/p/pandolf/CMSSW_6_1_1_HgglobeOOTB_reloaded/src/h2gglobe/AnalysisScripts/scales/scales_2D_pt_data_moriond.root" );
  TFile* file_fasaWeights_eta = TFile::Open( "/afs/cern.ch/work/p/pandolf/CMSSW_6_1_1_HgglobeOOTB_reloaded/src/h2gglobe/AnalysisScripts/scales/scales_2D_eta_data_moriond.root" );
  TH2D* h2_fasaWeights_pt  = (TH2D*)file_fasaWeights_pt ->Get("h2D_pt_data");
  TH2D* h2_fasaWeights_eta = (TH2D*)file_fasaWeights_eta->Get("h2D_eta_data");


  doPlotForOneSelection( "LDcut", db, db_data, file_nominal, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID, false, h2_fasaWeights_pt, h2_fasaWeights_eta );
  //doPlotForOneSelection( "LDsidebands", db, db_data, file_nominal, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID, true, h2_fasaWeights_pt, h2_fasaWeights_eta );

  return 0;

}



void doPlotForOneSelection( const std::string& saveName, DrawBase* db, DrawBase* db_data, TFile* file_nominal, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID, bool LDsidebands, TH2D* h2_fasaWeights_pt, TH2D* h2_fasaWeights_eta ) {


  std::vector<std::string> dataNames;
  dataNames.push_back("Data");

  std::vector<std::string> mcNames;
  mcNames.push_back("diphojet_sherpa_8TeV");
  mcNames.push_back("ZGToLLG");
  mcNames.push_back("ttgg");
  mcNames.push_back("ttg");

  TGraphErrors* gr_p0_data = drawPlotsCS( db_data, dataNames, saveName, file_nominal, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID, LDsidebands, h2_fasaWeights_pt, h2_fasaWeights_eta );
  TGraphErrors* gr_p0_mc   = drawPlotsCS( db,        mcNames, saveName, file_nominal, file_btagZero, file_btagLoose, file_btagZeroInvID, file_btagLooseInvID, LDsidebands, h2_fasaWeights_pt, h2_fasaWeights_eta );

  TCanvas* c2 = new TCanvas("c2", "c2", 600, 600);
  c2->cd();

  
  //TH2D* h2_axes = new TH2D("axes2", "", 5, 0., 5., 10, -0.1, 0.1);
  TH2D* h2_axes = new TH2D("axes2", "", 5, 0., 5., 10, -0.12, 0.05);
  //h2_axes->GetXaxis()->SetBinLabel( 1, "InvID" );
  h2_axes->GetXaxis()->SetBinLabel( 1, "Nominal" );
  h2_axes->GetXaxis()->SetBinLabel( 2, "CSVL" );
  h2_axes->GetXaxis()->SetBinLabel( 3, "CSV0" );
  h2_axes->GetXaxis()->SetBinLabel( 4, "InvID CSVL" );
  h2_axes->GetXaxis()->SetBinLabel( 5, "InvID CSV0" );
  h2_axes->SetYTitle( "p0 [GeV^{-1}]" );

  h2_axes->Draw();

  gr_p0_data->SetMarkerStyle(20);
  gr_p0_data->SetMarkerSize(1.6);
  gr_p0_data->SetMarkerColor(46);

  gr_p0_mc->SetMarkerStyle(24);
  gr_p0_mc->SetMarkerSize(1.6);
  gr_p0_mc->SetMarkerColor(kBlack);

  float p0_data_mean = gr_p0_data->GetMean(2);
  float p0_data_mean_err = gr_p0_data->GetRMS(2) / sqrt( gr_p0_data->GetN() ) ;

  float p0_mc_mean = gr_p0_mc->GetMean(2);
  float p0_mc_mean_err = gr_p0_mc->GetRMS(2) / sqrt( gr_p0_mc->GetN() ) ;

  //char dataText[300];
  //sprintf( dataText, "Data ( average p_{0} = %.3f #pm %.3f GeV^{-1} )", p0_data_mean, p0_data_mean_err );
  //char mcText[300];
  //sprintf( mcText, "MC ( average p_{0} = %.3f #pm %.3f GeV^{-1} )", p0_mc_mean, p0_mc_mean_err );
  TLegend* legend = new TLegend( 0.2, 0.2, 0.45, 0.4 );
  //TLegend* legend = new TLegend( 0.2, 0.7, 0.6, 0.9 );
  legend->SetTextSize( 0.038 );
  legend->SetFillColor( 0 );
  //legend->AddEntry( gr_p0_data, dataText, "P" );
  //legend->AddEntry( gr_p0_mc,   mcText, "P" );
  legend->AddEntry( gr_p0_data, "Data", "P" );
  legend->AddEntry( gr_p0_mc,   "Simulation", "P" );
  legend->Draw("same");

  TPaveText* labelTop = db_data->get_labelTop();
  labelTop->Draw("same");

  gr_p0_data->Draw("p same");
  gr_p0_mc->Draw("p same");

  if( LDsidebands ) {
    TPaveText* labelSidebands = new TPaveText( 0.7, 0.85, 0.9, 0.9, "brNDC" );
    labelSidebands->SetTextSize(0.038);
    labelSidebands->SetFillColor(0);
    labelSidebands->AddText("LD < 0.25");
    labelSidebands->Draw("same");
  }

  std::string canvasName = db->get_outputdir() + "/p0_" + saveName;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
  c2->SaveAs( canvasName_eps.c_str() );
  c2->SaveAs( canvasName_png.c_str() );

  delete c2;
  delete legend;
  delete h2_axes;
  delete gr_p0_data;
  delete gr_p0_mc;

}




TGraphErrors* drawPlotsCS( DrawBase* db, std::vector<std::string> datasetNames, const std::string& saveName, TFile* file_nominal, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID, bool LDsidebands, TH2D* h2_fasaWeights_pt, TH2D* h2_fasaWeights_eta ) {


  TChain* tree_nominal        = new TChain("chain_nominal");
  TChain* tree_btagLoose      = new TChain("chain_btagLoose");
  TChain* tree_btagZero       = new TChain("chain_btagZero");
  TChain* tree_btagZeroInvID  = new TChain("chain_btagZeroInvID");
  TChain* tree_btagLooseInvID = new TChain("chain_btagLooseInvID");

  for( unsigned idataset=0; idataset<datasetNames.size(); ++idataset ) {

    std::string treeName_nominal        (file_nominal->GetName());
    std::string treeName_btagLoose      (file_btagLoose->GetName());
    std::string treeName_btagLooseInvID (file_btagLooseInvID->GetName());
    std::string treeName_btagZero       (file_btagZero->GetName());
    std::string treeName_btagZeroInvID  (file_btagZeroInvID->GetName());

    treeName_nominal        = treeName_nominal        + "/" + datasetNames[idataset];
    treeName_btagLoose      = treeName_btagLoose      + "/" + datasetNames[idataset];
    treeName_btagLooseInvID = treeName_btagLooseInvID + "/" + datasetNames[idataset];
    treeName_btagZero       = treeName_btagZero       + "/" + datasetNames[idataset];
    treeName_btagZeroInvID  = treeName_btagZeroInvID  + "/" + datasetNames[idataset];

    tree_nominal       ->Add(treeName_nominal.c_str());
    tree_btagLoose     ->Add(treeName_btagLoose.c_str());
    tree_btagLooseInvID->Add(treeName_btagLooseInvID.c_str());
    tree_btagZero      ->Add(treeName_btagZero.c_str());
    tree_btagZeroInvID ->Add(treeName_btagZeroInvID.c_str());

  } // for datasets

  ////TTree* tree_invID           = (TTree*)file_invID->Get(datasetName.c_str());
  //TTree* tree_btagLoose       = (TTree*)file_btagLoose->Get(datasetName.c_str());
  //TTree* tree_btagZero        = (TTree*)file_btagZero->Get(datasetName.c_str());
  //TTree* tree_btagZeroInvID   = (TTree*)file_btagZeroInvID->Get(datasetName.c_str());
  //TTree* tree_btagLooseInvID  = (TTree*)file_btagLooseInvID->Get(datasetName.c_str());




  bool isData = (datasetNames[0]=="Data");
  int nBins = 80;
  float xMin = 100.;
  float xMax = 180.;
  float p0_init = -0.03;
  float p0_min = -0.5;
  float p0_max = 0.;
  RooRealVar* mgg_roo = new RooRealVar("mgg_roo","M_{#gamma#gamma}", 100., xMin, xMax, "GeV");
  //RooRealVar p0_btagZeroInvID("p0_btagZeroInvID","p_{0}", p0_init, p0_min, p0_max);


  RooWorkspace* w = new RooWorkspace("csworkspace");
  w->import( *mgg_roo );

  std::string datasetNameSave = (isData) ? "Data" : "MC";
  //fitTreeWithExponential( db, datasetName + "_" + saveName + "_invID",         tree_invID,          w, "mgg_roo", fullThqSelection );
  fitTreeWithExponential( db, datasetNameSave + "_" + saveName + "_nominal",       tree_nominal,      w, "mgg_roo", LDsidebands );
  fitTreeWithExponential( db, datasetNameSave + "_" + saveName + "_btagLoose",     tree_btagLoose,      w, "mgg_roo", LDsidebands );
  fitTreeWithExponential( db, datasetNameSave + "_" + saveName + "_btagZero",      tree_btagZero,       w, "mgg_roo", LDsidebands );
  fitTreeWithExponential( db, datasetNameSave + "_" + saveName + "_btagLooseInvID",tree_btagLooseInvID, w, "mgg_roo", LDsidebands, h2_fasaWeights_pt, h2_fasaWeights_eta );
  fitTreeWithExponential( db, datasetNameSave + "_" + saveName + "_btagZeroInvID", tree_btagZeroInvID,  w, "mgg_roo", LDsidebands, h2_fasaWeights_pt, h2_fasaWeights_eta );


  //RooRealVar* p0_invID          = (RooRealVar*)w->var( Form("p0_%s_%s_invID",         saveName.c_str(), datasetName.c_str()) );
  RooRealVar* p0_nominal        = (RooRealVar*)w->var( Form("p0_%s_%s_nominal",       datasetNameSave.c_str(), saveName.c_str()) );
  RooRealVar* p0_btagLoose      = (RooRealVar*)w->var( Form("p0_%s_%s_btagLoose",     datasetNameSave.c_str(), saveName.c_str()) );
  RooRealVar* p0_btagZero       = (RooRealVar*)w->var( Form("p0_%s_%s_btagZero",      datasetNameSave.c_str(), saveName.c_str()) );
  RooRealVar* p0_btagLooseInvID = (RooRealVar*)w->var( Form("p0_%s_%s_btagLooseInvID",datasetNameSave.c_str(), saveName.c_str()) );
  RooRealVar* p0_btagZeroInvID  = (RooRealVar*)w->var( Form("p0_%s_%s_btagZeroInvID", datasetNameSave.c_str(), saveName.c_str()) );

  //RooExponential* exp_invID          = (RooExponential*)w->pdf( Form("exp_%s_%s_invID",         saveName.c_str(), datasetName.c_str()) );
  RooExponential* exp_nominal        = (RooExponential*)w->pdf( Form("exp_%s_%s_nominal",       datasetNameSave.c_str(), saveName.c_str()) );
  RooExponential* exp_btagLoose      = (RooExponential*)w->pdf( Form("exp_%s_%s_btagLoose",     datasetNameSave.c_str(), saveName.c_str()) );
  RooExponential* exp_btagZero       = (RooExponential*)w->pdf( Form("exp_%s_%s_btagZero",      datasetNameSave.c_str(), saveName.c_str()) );
  RooExponential* exp_btagLooseInvID = (RooExponential*)w->pdf( Form("exp_%s_%s_btagLooseInvID",datasetNameSave.c_str(), saveName.c_str()) );
  RooExponential* exp_btagZeroInvID  = (RooExponential*)w->pdf( Form("exp_%s_%s_btagZeroInvID", datasetNameSave.c_str(), saveName.c_str()) );

  //RooFitResult* f_invID          = (RooFitResult*)w->obj( Form("f_%s_%s_invID",          saveName.c_str(), datasetName.c_str()) );
  RooFitResult* f_nominal        = (RooFitResult*)w->obj( Form("f_%s_%s_nominal",        datasetNameSave.c_str(), saveName.c_str()) );
  RooFitResult* f_btagLoose      = (RooFitResult*)w->obj( Form("f_%s_%s_btagLoose",      datasetNameSave.c_str(), saveName.c_str()) );
  RooFitResult* f_btagZero       = (RooFitResult*)w->obj( Form("f_%s_%s_btagZero",       datasetNameSave.c_str(), saveName.c_str()) );
  RooFitResult* f_btagLooseInvID = (RooFitResult*)w->obj( Form("f_%s_%s_btagLooseInvID", datasetNameSave.c_str(), saveName.c_str()) );
  RooFitResult* f_btagZeroInvID  = (RooFitResult*)w->obj( Form("f_%s_%s_btagZeroInvID",  datasetNameSave.c_str(), saveName.c_str()) );


  //TFile* file = TFile::Open("paologay.root", "recreate");
  //file->cd();
  //w->Write();
  //file->Close();






  int category=11;

  char fullSelection[500];
  char fullSelection_sidebands[500];
  if( LDsidebands ) {
    sprintf( fullSelection,           "evweight*( category==%d && thqLD_lept<0.25 )", category );
    sprintf( fullSelection_sidebands, "evweight*( category==%d && thqLD_lept<0.25 && (PhotonsMass<115. || PhotonsMass>135.) )", category );
  } else {
    sprintf( fullSelection,           "evweight*( category==%d && thqLD_lept>=0.25 )", category );
    sprintf( fullSelection_sidebands, "evweight*( category==%d && thqLD_lept>=0.25 && (PhotonsMass<115. || PhotonsMass>135.) )", category );
  }


  //TH1D* h1_mgg_invID           = new TH1D("mgg_invID", "", nBins, xMin, xMax);
  TH1D* h1_mgg_nominal         = new TH1D("mgg_nominal", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagLoose       = new TH1D("mgg_btagLoose", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagZero        = new TH1D("mgg_btagZero", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagLooseInvID  = new TH1D("mgg_btagLooseInvID", "", nBins, xMin, xMax);
  TH1D* h1_mgg_btagZeroInvID   = new TH1D("mgg_btagZeroInvID", "", nBins, xMin, xMax);

  //h1_mgg_invID           ->Sumw2();
  h1_mgg_nominal         ->Sumw2();
  h1_mgg_btagLoose       ->Sumw2();
  h1_mgg_btagZero        ->Sumw2();
  h1_mgg_btagLooseInvID  ->Sumw2();
  h1_mgg_btagZeroInvID   ->Sumw2();


  if( BLINDED && isData ) {
    //tree_invID->Project( "mgg_invID", "PhotonsMass", fullSelection_sidebands );
    tree_nominal->Project(        "mgg_nominal", "PhotonsMass", fullSelection_sidebands );
    tree_btagLoose->Project(      "mgg_btagLoose", "PhotonsMass", fullSelection_sidebands );
    tree_btagZero->Project(       "mgg_btagZero", "PhotonsMass", fullSelection_sidebands );
    tree_btagLooseInvID->Project( "mgg_btagLooseInvID", "PhotonsMass", fullSelection_sidebands );
    tree_btagZeroInvID->Project(  "mgg_btagZeroInvID", "PhotonsMass", fullSelection_sidebands );
  } else {
    //tree_invID->Project( "mgg_invID", "PhotonsMass", fullSelection );
    tree_nominal->Project(        "mgg_nominal", "PhotonsMass", fullSelection );
    tree_btagLoose->Project(      "mgg_btagLoose", "PhotonsMass", fullSelection );
    tree_btagZero->Project(       "mgg_btagZero", "PhotonsMass", fullSelection );
    tree_btagLooseInvID->Project( "mgg_btagLooseInvID", "PhotonsMass", fullSelection );
    tree_btagZeroInvID->Project(  "mgg_btagZeroInvID", "PhotonsMass", fullSelection );
  }


  ofstream ofs(Form("%s/events_%s_%s.txt", db->get_outputdir().c_str(), datasetNameSave.c_str(), saveName.c_str()) );
  ofs << "nominal: " << h1_mgg_nominal->GetEntries() << std::endl;
  ofs << "btagLoose: " << h1_mgg_btagLoose->GetEntries() << std::endl;
  ofs << "btagZero: " << h1_mgg_btagZero->GetEntries() << std::endl;
  ofs << "btagLooseInvID: " << h1_mgg_btagLooseInvID->GetEntries() << std::endl;
  ofs << "btagZeroInvID: " << h1_mgg_btagZeroInvID->GetEntries() << std::endl;
  ofs.close();

  //TF1* f1_invID = fitHisto( h1_mgg_invID, kRed, 1 );
  TF1* f1_nominal = fitHisto( h1_mgg_nominal, kBlack, 1 );
  TF1* f1_btagLoose = fitHisto( h1_mgg_btagLoose, kBlack, 1 );
  TF1* f1_btagZero = fitHisto( h1_mgg_btagZero, 29, 1 );
  TF1* f1_btagLooseInvID = fitHisto( h1_mgg_btagLooseInvID, 39, 2 );
  TF1* f1_btagZeroInvID = fitHisto( h1_mgg_btagZeroInvID, kRed+1, 2 );

  //TH1D* h1_band_invID          = fitTools::getBand( f1_invID, "invID" );
  TH1D* h1_band_nominal        = fitTools::getBand( f1_nominal, "band_nominal" );
  TH1D* h1_band_btagLoose      = fitTools::getBand( f1_btagLoose, "band_btagLoose" );
  TH1D* h1_band_btagZero       = fitTools::getBand( f1_btagZero, "band_btagZero" );
  TH1D* h1_band_btagLooseInvID = fitTools::getBand( f1_btagLooseInvID, "band_btagLooseInvID" );
  TH1D* h1_band_btagZeroInvID  = fitTools::getBand( f1_btagZeroInvID, "band_btagZeroInvID" );

  //h1_band_invID          ->SetFillStyle(3003);
  h1_band_nominal        ->SetFillStyle(3003);
  h1_band_btagLoose      ->SetFillStyle(3003);
  h1_band_btagZero       ->SetFillStyle(3003);
  h1_band_btagLooseInvID ->SetFillStyle(3003);
  h1_band_btagZeroInvID  ->SetFillStyle(3003);

  //h1_band_invID          ->SetFillColor(kRed);
  h1_band_nominal        ->SetFillColor(kGray+2);
  h1_band_btagLoose      ->SetFillColor(kGray);
  h1_band_btagZero       ->SetFillColor(29);
  h1_band_btagLooseInvID ->SetFillColor(39);
  h1_band_btagZeroInvID  ->SetFillColor(kRed+1);

  //h1_band_invID          ->SetLineColor(kRed);
  h1_band_nominal        ->SetLineColor(kGray);
  h1_band_btagLoose      ->SetLineColor(kGray);
  h1_band_btagZero       ->SetLineColor(29);
  h1_band_btagLooseInvID ->SetLineColor(39);
  h1_band_btagZeroInvID  ->SetLineColor(kRed+1);

  
  TCanvas* c2 = new TCanvas("c2", "", 600, 600);
  c2->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 100., 180., 10, 0., 0.4);
  h2_axes->SetXTitle( "Diphoton Mass [GeV]");
  h2_axes->SetYTitle( "Normalized to Unity");

  h2_axes->Draw();



  //char text_invID[300];
  //sprintf( text_invID, "InvID (%.0f evts)", h1_mgg_invID->GetEntries() );
  char text_nominal[300];
  sprintf( text_nominal, "Nominal (%.0f evts)", h1_mgg_nominal->GetEntries() );
  char text_btagLoose[300];
  sprintf( text_btagLoose, "CSVL (%.0f evts)", h1_mgg_btagLoose->GetEntries() );
  char text_btagZero[300];
  sprintf( text_btagZero, "No Btag (%.0f evts)", h1_mgg_btagZero->GetEntries() );
  char text_btagLooseInvID[300];
  sprintf( text_btagLooseInvID, "CSVL + InvID (%.0f evts)", h1_mgg_btagLooseInvID->GetEntries() );
  char text_btagZeroInvID[300];
  sprintf( text_btagZeroInvID, "No Btag + InvID (%.0f evts)", h1_mgg_btagZeroInvID->GetEntries() );

  TLegend* legend = new TLegend( 0.23, 0.6, 0.9, 0.9, datasetNameSave.c_str() );
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  //legend->AddEntry( f1_invID, text_invID, "L" );
  legend->AddEntry( f1_nominal, text_nominal, "L" );
  legend->AddEntry( f1_btagLoose, text_btagLoose, "L" );
  legend->AddEntry( f1_btagZero, text_btagZero, "L" );
  legend->AddEntry( f1_btagLooseInvID, text_btagLooseInvID, "L" );
  legend->AddEntry( f1_btagZeroInvID, text_btagZeroInvID, "L" );
  legend->Draw("same");

  //f1_invID->Draw("l same");
  f1_nominal->Draw("l same");
  f1_btagLoose->Draw("l same");
  f1_btagZero->Draw("l same");
  f1_btagLooseInvID->Draw("l same");
  f1_btagZeroInvID->Draw("l same");

  //h1_band_invID          ->Draw("c e3 same");
  //h1_band_btagLoose      ->Draw("c e3 same");
  //h1_band_btagZero       ->Draw("c e3 same");
  //h1_band_btagLooseInvID ->Draw("c e3 same");
  //h1_band_btagZeroInvID  ->Draw("c e3 same");


  //h1_mgg_invID->Rebin(10);
  //h1_mgg_btagLoose->Rebin(10);
  //h1_mgg_btagZero->Rebin(10);
  //h1_mgg_btagLooseInvID->Rebin(10);
  //h1_mgg_btagZeroInvID->Rebin(10);

  //h1_mgg_invID->Scale(          1./h1_mgg_invID->Integral("width") );
  h1_mgg_nominal->Scale(        1./h1_mgg_nominal->Integral("width") );
  h1_mgg_btagLoose->Scale(      1./h1_mgg_btagLoose->Integral("width") );
  h1_mgg_btagZero->Scale(       1./h1_mgg_btagZero->Integral("width") );
  h1_mgg_btagLooseInvID->Scale( 1./h1_mgg_btagLooseInvID->Integral("width") );
  h1_mgg_btagZeroInvID->Scale(  1./h1_mgg_btagZeroInvID->Integral("width") );

  //h1_mgg_invID->Draw("p same");
  h1_mgg_btagLoose->DrawNormalized("p same");
  //h1_mgg_btagZero->Draw("p same");
  //h1_mgg_btagLooseInvID->Draw("p same");
  //h1_mgg_btagZeroInvID->Draw("p same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/mgg_" + datasetNameSave + "_" + saveName;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
  c2->SaveAs(canvasName_eps.c_str());
  c2->SaveAs(canvasName_png.c_str());


  delete c2;
  delete legend;
  delete h2_axes;
  //delete h1_mgg_invID;
  delete h1_mgg_nominal;
  delete h1_mgg_btagLoose;
  delete h1_mgg_btagZero;
  delete h1_mgg_btagLooseInvID;
  delete h1_mgg_btagZeroInvID;

  delete h1_band_nominal     ;
  delete h1_band_btagLoose     ;
  delete h1_band_btagZero      ;
  delete h1_band_btagLooseInvID;
  delete h1_band_btagZeroInvID ;



  float stagger = (isData) ? 0.98 : 1.02;

  std::string gr_name = "p0_"+datasetNameSave+"_"+saveName;
  TGraphErrors* gr_p0 = new TGraphErrors(0);
  gr_p0->SetName(gr_name.c_str());
  //gr_p0->SetPoint( 0, 0.5, p0_invID         ->getVal() );
  gr_p0->SetPoint( 0, stagger*0.5, p0_nominal       ->getVal() );
  gr_p0->SetPoint( 1, stagger*1.5, p0_btagLoose     ->getVal() );
  gr_p0->SetPoint( 2, stagger*2.5, p0_btagZero      ->getVal() );
  gr_p0->SetPoint( 3, stagger*3.5, p0_btagLooseInvID->getVal() );
  gr_p0->SetPoint( 4, stagger*4.5, p0_btagZeroInvID ->getVal() );
  //gr_p0->SetPointError( 0, 0., p0_invID         ->getError() );
  gr_p0->SetPointError( 0, 0., p0_nominal     ->getError() );
  gr_p0->SetPointError( 1, 0., p0_btagLoose     ->getError() );
  gr_p0->SetPointError( 2, 0., p0_btagZero      ->getError() );
  gr_p0->SetPointError( 3, 0., p0_btagLooseInvID->getError() );
  gr_p0->SetPointError( 4, 0., p0_btagZeroInvID ->getError() );

  ofstream ofs_p0(Form("%s/p0_%s_%s.txt", db->get_outputdir().c_str(), datasetNameSave.c_str(), saveName.c_str()) );
  ofs_p0 << "nominal: " << p0_nominal->getVal() << " +- " << p0_nominal->getError() << std::endl;
  ofs_p0 << "btagLoose: " << p0_btagLoose->getVal() << " +- " << p0_btagLoose->getError() << std::endl;
  ofs_p0 << "btagZero: " << p0_btagZero->getVal() << " +- " << p0_btagZero->getError() << std::endl;
  ofs_p0 << "btagLooseInvID: " << p0_btagLooseInvID->getVal() << " +- " << p0_btagLooseInvID->getError() << std::endl;
  ofs_p0 << "btagZeroInvID: " << p0_btagZeroInvID->getVal() << " +- " << p0_btagZeroInvID->getError() << std::endl;
  ofs_p0.close();

  return gr_p0;

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

  //h1->Fit(f1, "R");
  h1->Fit(f1, "RL");

  return f1;

}


/*
void compareVariableCS( DrawBase* db, const std::string& datasetName, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, TFile* file_invID, TFile* file_btagZero, TFile* file_btagLoose, TFile* file_btagZeroInvID, TFile* file_btagLooseInvID ) {

  TTree* tree_invID           = (TTree*)file_invID->Get(datasetName.c_str());
  TTree* tree_btagZero        = (TTree*)file_btagZero->Get(datasetName.c_str());
  TTree* tree_btagLoose       = (TTree*)file_btagLoose->Get(datasetName.c_str());
  TTree* tree_btagZeroInvID   = (TTree*)file_btagZeroInvID->Get(datasetName.c_str());
  TTree* tree_btagLooseInvID  = (TTree*)file_btagLooseInvID->Get(datasetName.c_str());


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

  bool isData = datasetName=="Data";

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

  TLegend* legend = new TLegend( 0.37, 0.6, 0.9, 0.9, datasetName.c_str() );
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

  std::string canvasName = db->get_outputdir() + "/" + varName + "_" + datasetName;
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
*/




void fitTreeWithExponential( DrawBase* db, const std::string& name, TTree* tree, RooWorkspace* w, const std::string& varName, bool LDsidebands, TH2D* h2_fasaWeights_pt, TH2D* h2_fasaWeights_eta ) {


  RooRealVar* mgg_roo = w->var(varName.c_str());

  RooDataSet* dataset = getDatasetFromTree( name, tree, *mgg_roo, LDsidebands, h2_fasaWeights_pt, h2_fasaWeights_eta );

  dataset->Print();
  //stringstream weight;
  //weight << "weight";
  //RooRealVar w("weight", "weight", 1.0, -1000., 1000.);

  //RooArgSet varset(x,w,"argset_obs");
  //RooDataSet dataset("dataset","dataset",varset,WeightVar(weight.str().c_str()));

  float xMin = mgg_roo->getMin();
  float xMax = mgg_roo->getMax();

  float p0_init=-0.03;
  float p0_min =-1.;
  float p0_max = 0.1;

  // define exponential
  std::string p0_name = "p0_"+name;
  RooRealVar p0(p0_name.c_str(),p0_name.c_str(),p0_init, p0_min, p0_max);
  std::string exp_name = "exp_" + name;
  RooExponential* exp = new RooExponential(exp_name.c_str(),"exp",*mgg_roo,p0);

  RooFitResult* f = exp->fitTo(*dataset, RooFit::Save(), RooFit::Range(xMin,xMax), RooFit::SumW2Error(kTRUE));
  std::string f_name = "f_" + name;
  f->SetName(f_name.c_str());
  //exp.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));

  w->import( *f );
  w->import( *dataset );
  w->import( *exp );
  

  p0.Print();

  TCanvas* c1 = new TCanvas( "c1", "", 600, 600 );
  c1->cd();

  RooPlot* xframe = mgg_roo->frame(xMin, xMax, 50) ;
  dataset->plotOn( xframe );
  exp->plotOn( xframe );
  xframe->Draw();

  std::string canvasName = db->get_outputdir() + "/fit_" + name;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  //ValAndErr returnStruct;
  //returnStruct.val = p0.getVal();
  //returnStruct.err = p0.getError();

  //return returnStruct;
  //return exp;
  //return p0;

}



RooDataSet* getDatasetFromTree( const std::string& name, TTree* tree, RooRealVar& mgg_roo, bool LDsidebands, TH2D* h2_fasaWeights_pt, TH2D* h2_fasaWeights_eta ) {


  if( tree==0 ) {

    std::cout << std::endl;
    std::cout << "-> ERROR!! Emtpy tree. Please check: " << name << std::endl;
    exit(11);
  
  }


  int run;
  tree->SetBranchAddress( "run", &run );
  int itype;
  tree->SetBranchAddress( "itype", &itype );
  int event;
  tree->SetBranchAddress( "event", &event );
  bool isMu;
  tree->SetBranchAddress( "isMu", &isMu );
  float ph1_pt;
  tree->SetBranchAddress( "ph1_pt", &ph1_pt );
  float ph2_pt;
  tree->SetBranchAddress( "ph2_pt", &ph2_pt );
  float ph1_eta;
  tree->SetBranchAddress( "ph1_eta", &ph1_eta );
  float ph2_eta;
  tree->SetBranchAddress( "ph2_eta", &ph2_eta );
  float mgg;
  tree->SetBranchAddress( "PhotonsMass", &mgg );
  int category;
  tree->SetBranchAddress( "category", &category );
  float weight;
  tree->SetBranchAddress( "weight", &weight );
  float evweight;
  tree->SetBranchAddress( "evweight", &evweight );
  float thqLD;
  tree->SetBranchAddress( "thqLD_lept", &thqLD );
  float m_lept_phot2;
  tree->SetBranchAddress( "m_lept_phot2", &m_lept_phot2 );

  RooRealVar weight_roo("weight_roo", "weight_roo", 1., 0., 10.);
  RooArgSet aset(mgg_roo, weight_roo, "argset");
  RooDataSet* dataset = new RooDataSet(name.c_str(), name.c_str(), aset);

  int nentries = tree->GetEntries();

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);
  
    if( category!=11 ) continue;
    if( mgg<100. || mgg>180. ) continue;
    if( ph1_pt<50.*mgg/120. ) continue;
    if( BLINDED && itype==0 && (mgg>115. && mgg<135.) ) continue;
    if( LDsidebands ) {
      if( thqLD>0.25 ) continue;
    } else {
      if( thqLD<0.25 ) continue;
    }

    if( !isMu && fabs(m_lept_phot2 - 91.91)<5. ) continue; // cut on M(e,gamma2)
    //if( mgg<115. ) weight = 1000.;

    mgg_roo.setVal(mgg);

    float corrweight = evweight;
    if( FASAWEIGHTS ) {
      if( h2_fasaWeights_pt!=0 ) {
        int bin_pt1 = h2_fasaWeights_pt->GetYaxis()->FindBin(ph1_pt);
        int bin_pt2 = h2_fasaWeights_pt->GetXaxis()->FindBin(ph2_pt);
        float fasaWeight_pt = h2_fasaWeights_pt->GetBinContent( bin_pt2, bin_pt1 );
        corrweight *= fasaWeight_pt;
      }
      if( h2_fasaWeights_eta!=0 ) {
        int bin_eta1 = h2_fasaWeights_eta->GetYaxis()->FindBin(ph1_eta);
        int bin_eta2 = h2_fasaWeights_eta->GetXaxis()->FindBin(ph2_eta);
        float fasaWeight_eta = h2_fasaWeights_eta->GetBinContent( bin_eta2, bin_eta1 );
        corrweight *= fasaWeight_eta;
      }
    }

    if( itype==83 ) corrweight *= 9.; // ttgg

    weight_roo.setVal(corrweight);
    aset.setRealValue("mgg_roo", mgg);
    aset.setRealValue("weight_roo", corrweight);

    dataset->add(aset);

  }

  char w_datasetName[500];
  sprintf( w_datasetName, "w_%s", dataset->GetName());
  RooDataSet* wdata = new RooDataSet(w_datasetName, w_datasetName, dataset, *dataset->get(), 0, "weight_roo");

  return wdata;
  //return dataset;

}

