#include <iostream>

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

#include "CommonTools/DrawBase.h"


void compareSingleVariable( DrawBase* db, const std::string& varName, const std::string& axisName, const std::string& units, TH1D* h1_qJet_thq_old, TH1D* h1_qJet_tth_old, TH1D* h1_qJet_thq_new, TH1D* h1_qJet_tth_new );




int main() {


  DrawBase* db = new DrawBase("kk");
  db->set_outputdir("plotsLD");


  std::string fileName_new = "../batchOutput_postFreeze_v2/histograms_CMS-HGG.root"; 
  TFile* file_new = TFile::Open(fileName_new.c_str());
  TTree* tree_thq_new = (TTree*)file_new->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth_new = (TTree*)file_new->Get("tth_m125_8TeV");

  std::string fileName_old = "../batchOutput_preNatal_v5/histograms_CMS-HGG.root"; 
  TFile* file_old = TFile::Open(fileName_old.c_str());
  TTree* tree_thq_old = (TTree*)file_old->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth_old = (TTree*)file_old->Get("tth_m125_8TeV");

  //std::string fileName_thq_old = "/afs/cern.ch/work/p/pandolf/public/THq_tHqLeptonic_mH125_8TeV_testtest_presel_isLeptonic_CSV.root";
  //std::string fileName_tth_old = "/afs/cern.ch/work/p/pandolf/public/THq_TTH_HToGG_M-125_8TeV-pythia6_Summer12-PU_S7_START52_V9-v2_presel_isLeptonic_CSV.root";
  //TFile* file_thq_old = TFile::Open(fileName_thq_old.c_str());
  //TFile* file_tth_old = TFile::Open(fileName_tth_old.c_str());

  //TTree* tree_thq_old = (TTree*)file_thq_old->Get("tree_passedEvents");
  //TTree* tree_tth_old = (TTree*)file_tth_old->Get("tree_passedEvents");


  TH1D* h1_nJetsCentral_thq_old       = new TH1D( "nJetsCentral_thq_old", "", 9, -0.5, 8.5 );
  TH1D* h1_nJets_thq_old              = new TH1D( "nJets_thq_old", "", 9, -0.5, 8.5 );
  TH1D* h1_eta_qJet_thq_old           = new TH1D( "eta_qJet_thq_old", "", 30, 0., 5. );
  TH1D* h1_mt_top_thq_old             = new TH1D( "mt_top_thq_old", "", 50, 0., 1000. );
  TH1D* h1_lept_charge_thq_old        = new TH1D( "lept_charge_thq_old", "", 3, -1.5, 1.5 );
  TH1D* h1_deltaEta_lept_qJet_thq_old = new TH1D( "deltaEta_lept_qJet_thq_old", "", 35, 0., 7. );
  TH1D* h1_nvtx_thq_old = new TH1D( "nvtx_thq_old", "", 35, 0.5, 35.5 );


  //tree_thq_old->Project("nJetsCentral_thq_old", "nCentralJets", "isLeptonic*eventWeight");
  //tree_thq_old->Project("nJets_thq_old", "njets", "isLeptonic*eventWeight");
  //tree_thq_old->Project("eta_qJet_thq_old", "eta_qJet", "isLeptonic*eventWeight");
  //tree_thq_old->Project("mt_top_thq_old", "mt_top", "isLeptonic*eventWeight");
  //tree_thq_old->Project("lept_charge_thq_old", "charge_lept", "isLeptonic*eventWeight");
  //tree_thq_old->Project("deltaEta_lept_qJet_thq_old", "deltaEta_lept_qJet", "isLeptonic*eventWeight");

  tree_thq_old->Project("nJetsCentral_thq_old", "njets_InsideEtaCut", "evweight*( category==11 )" );
  tree_thq_old->Project("nJets_thq_old", "njets", "evweight*( category==11 )" );
  tree_thq_old->Project("eta_qJet_thq_old", "qJetEta", "evweight*( category==11 )" );
  tree_thq_old->Project("mt_top_thq_old", "topMt", "evweight*( category==11 )" );
  tree_thq_old->Project("lept_charge_thq_old", "lept_charge", "evweight*( category==11 )" );
  tree_thq_old->Project("deltaEta_lept_qJet_thq_old", "deltaEta_lept_qJet", "evweight*( category==11 )" );
  tree_thq_old->Project("nvtx_thq_old", "nvtx", "evweight*( category==11 )" );




  h1_nJetsCentral_thq_old              ->Scale(1./h1_nJetsCentral_thq_old->Integral());
  h1_nJets_thq_old              ->Scale(1./h1_nJets_thq_old->Integral());
  h1_eta_qJet_thq_old           ->Scale(1./h1_eta_qJet_thq_old->Integral());
  h1_mt_top_thq_old             ->Scale(1./h1_mt_top_thq_old->Integral());
  h1_lept_charge_thq_old        ->Scale(1./h1_lept_charge_thq_old->Integral());
  h1_deltaEta_lept_qJet_thq_old ->Scale(1./h1_deltaEta_lept_qJet_thq_old->Integral());
  h1_nvtx_thq_old ->Scale(1./h1_nvtx_thq_old->Integral());


  TH1D* h1_nJetsCentral_tth_old              = new TH1D( "nJetsCentral_tth_old", "", 9, -0.5, 8.5 );
  TH1D* h1_nJets_tth_old              = new TH1D( "nJets_tth_old", "", 9, -0.5, 8.5 );
  TH1D* h1_eta_qJet_tth_old           = new TH1D( "eta_qJet_tth_old", "", 30, 0., 5. );
  TH1D* h1_mt_top_tth_old             = new TH1D( "mt_top_tth_old", "", 50, 0., 1000. );
  TH1D* h1_lept_charge_tth_old        = new TH1D( "lept_charge_tth_old", "", 3, -1.5, 1.5 );
  TH1D* h1_deltaEta_lept_qJet_tth_old = new TH1D( "deltaEta_lept_qJet_tth_old", "", 35, 0., 7. );
  TH1D* h1_nvtx_tth_old = new TH1D( "nvtx_tth_old", "", 35, 0.5, 35.5 );


  //tree_tth_old->Project("nJetsCentral_tth_old", "nCentralJets", "isLeptonic*eventWeight");
  //tree_tth_old->Project("nJets_tth_old", "njets", "isLeptonic*eventWeight");
  //tree_tth_old->Project("eta_qJet_tth_old", "eta_qJet", "isLeptonic*eventWeight");
  //tree_tth_old->Project("mt_top_tth_old", "mt_top", "isLeptonic*eventWeight");
  //tree_tth_old->Project("lept_charge_tth_old", "charge_lept", "isLeptonic*eventWeight");
  //tree_tth_old->Project("deltaEta_lept_qJet_tth_old", "deltaEta_lept_qJet", "isLeptonic*eventWeight");

  tree_tth_old->Project("nJetsCentral_tth_old", "njets_InsideEtaCut", "evweight*( category==11 )" );
  tree_tth_old->Project("nJets_tth_old", "njets", "evweight*( category==11 )" );
  tree_tth_old->Project("eta_qJet_tth_old", "qJetEta", "evweight*( category==11 )" );
  tree_tth_old->Project("mt_top_tth_old", "topMt", "evweight*( category==11 )" );
  tree_tth_old->Project("lept_charge_tth_old", "lept_charge", "evweight*( category==11 )" );
  tree_tth_old->Project("deltaEta_lept_qJet_tth_old", "deltaEta_lept_qJet", "evweight*( category==11 )" );
  tree_tth_old->Project("nvtx_tth_old", "nvtx", "evweight*( category==11 )" );


  h1_nJetsCentral_tth_old              ->Scale(1./h1_nJetsCentral_tth_old->Integral());
  h1_nJets_tth_old              ->Scale(1./h1_nJets_tth_old->Integral());
  h1_eta_qJet_tth_old           ->Scale(1./h1_eta_qJet_tth_old->Integral());
  h1_mt_top_tth_old             ->Scale(1./h1_mt_top_tth_old->Integral());
  h1_lept_charge_tth_old        ->Scale(1./h1_lept_charge_tth_old->Integral());
  h1_deltaEta_lept_qJet_tth_old ->Scale(1./h1_deltaEta_lept_qJet_tth_old->Integral());
  h1_nvtx_tth_old ->Scale(1./h1_nvtx_tth_old->Integral());


  TH1D* h1_nJetsCentral_thq_new              = new TH1D( "nJetsCentral_thq_new", "", 9, -0.5, 8.5 );
  TH1D* h1_nJets_thq_new              = new TH1D( "nJets_thq_new", "", 9, -0.5, 8.5 );
  TH1D* h1_eta_qJet_thq_new           = new TH1D( "eta_qJet_thq_new", "", 30, 0., 5. );
  TH1D* h1_mt_top_thq_new             = new TH1D( "mt_top_thq_new", "", 50, 0., 1000. );
  TH1D* h1_lept_charge_thq_new        = new TH1D( "lept_charge_thq_new", "", 3, -1.5, 1.5 );
  TH1D* h1_deltaEta_lept_qJet_thq_new = new TH1D( "deltaEta_lept_qJet_thq_new", "", 35, 0., 7. );
  TH1D* h1_nvtx_thq_new = new TH1D( "nvtx_thq_new", "", 35, 0.5, 35.5 );


  tree_thq_new->Project("nJetsCentral_thq_new", "njets_InsideEtaCut", "evweight*( category==11 )" );
  tree_thq_new->Project("nJets_thq_new", "njets", "evweight*( category==11 )" );
  tree_thq_new->Project("eta_qJet_thq_new", "qJetEta", "evweight*( category==11 )" );
  tree_thq_new->Project("mt_top_thq_new", "topMt", "evweight*( category==11 )" );
  tree_thq_new->Project("lept_charge_thq_new", "lept_charge", "evweight*( category==11 )" );
  tree_thq_new->Project("deltaEta_lept_qJet_thq_new", "deltaEta_lept_qJet", "evweight*( category==11 )" );
  tree_thq_new->Project("nvtx_thq_new", "nvtx", "evweight*( category==11 )" );


  h1_nJetsCentral_thq_new              ->Scale(1./h1_nJetsCentral_thq_new->Integral());
  h1_nJets_thq_new              ->Scale(1./h1_nJets_thq_new->Integral());
  h1_eta_qJet_thq_new           ->Scale(1./h1_eta_qJet_thq_new->Integral());
  h1_mt_top_thq_new             ->Scale(1./h1_mt_top_thq_new->Integral());
  h1_lept_charge_thq_new        ->Scale(1./h1_lept_charge_thq_new->Integral());
  h1_deltaEta_lept_qJet_thq_new ->Scale(1./h1_deltaEta_lept_qJet_thq_new->Integral());
  h1_nvtx_thq_new ->Scale(1./h1_nvtx_thq_new->Integral());



  TH1D* h1_nJetsCentral_tth_new              = new TH1D( "nJetsCentral_tth_new", "", 9, -0.5, 8.5 );
  TH1D* h1_nJets_tth_new              = new TH1D( "nJets_tth_new", "", 9, -0.5, 8.5 );
  TH1D* h1_eta_qJet_tth_new           = new TH1D( "eta_qJet_tth_new", "", 30, 0., 5. );
  TH1D* h1_mt_top_tth_new             = new TH1D( "mt_top_tth_new", "", 50, 0., 1000. );
  TH1D* h1_lept_charge_tth_new        = new TH1D( "lept_charge_tth_new", "", 3, -1.5, 1.5 );
  TH1D* h1_deltaEta_lept_qJet_tth_new = new TH1D( "deltaEta_lept_qJet_tth_new", "", 35, 0., 7. );
  TH1D* h1_nvtx_tth_new = new TH1D( "nvtx_tth_new", "", 35, 0.5, 35.5 );


  tree_tth_new->Project("nJetsCentral_tth_new", "njets_InsideEtaCut", "evweight*( category==11 )" );
  tree_tth_new->Project("nJets_tth_new", "njets", "evweight*( category==11 )" );
  tree_tth_new->Project("eta_qJet_tth_new", "qJetEta", "evweight*( category==11 )" );
  tree_tth_new->Project("mt_top_tth_new", "topMt", "evweight*( category==11 )" );
  tree_tth_new->Project("lept_charge_tth_new", "lept_charge", "evweight*( category==11 )" );
  tree_tth_new->Project("deltaEta_lept_qJet_tth_new", "deltaEta_lept_qJet", "evweight*( category==11 )" );
  tree_tth_new->Project("nvtx_tth_new", "nvtx", "evweight*( category==11 )" );


  h1_nJetsCentral_tth_new              ->Scale(1./h1_nJetsCentral_tth_new->Integral());
  h1_nJets_tth_new              ->Scale(1./h1_nJets_tth_new->Integral());
  h1_eta_qJet_tth_new           ->Scale(1./h1_eta_qJet_tth_new->Integral());
  h1_mt_top_tth_new             ->Scale(1./h1_mt_top_tth_new->Integral());
  h1_lept_charge_tth_new        ->Scale(1./h1_lept_charge_tth_new->Integral());
  h1_deltaEta_lept_qJet_tth_new ->Scale(1./h1_deltaEta_lept_qJet_tth_new->Integral());
  h1_nvtx_tth_new ->Scale(1./h1_nvtx_tth_new->Integral());



  compareSingleVariable( db, "nJetsCentral",         "Central Jet Multiplicity", "", h1_nJetsCentral_thq_old, h1_nJetsCentral_tth_old, h1_nJetsCentral_thq_new, h1_nJetsCentral_tth_new );
  compareSingleVariable( db, "nJets",         "Jet Multiplicity", "", h1_nJets_thq_old, h1_nJets_tth_old, h1_nJets_thq_new, h1_nJets_tth_new );
  compareSingleVariable( db, "eta_qJet",      "qJet Pseudorapidity", "", h1_eta_qJet_thq_old, h1_eta_qJet_tth_old, h1_eta_qJet_thq_new, h1_eta_qJet_tth_new );
  compareSingleVariable( db, "mt_top",        "Top Transverse Mass", "GeV", h1_mt_top_thq_old, h1_mt_top_tth_old, h1_mt_top_thq_new, h1_mt_top_tth_new );
  compareSingleVariable( db, "lept_charge",   "Lepton Charge", "", h1_lept_charge_thq_old, h1_lept_charge_tth_old, h1_lept_charge_thq_new, h1_lept_charge_tth_new );
  compareSingleVariable( db, "deltaEta_lept_qJet", "#Delta#eta(lepton-qJet)", "", h1_deltaEta_lept_qJet_thq_old, h1_deltaEta_lept_qJet_tth_old, h1_deltaEta_lept_qJet_thq_new, h1_deltaEta_lept_qJet_tth_new );
  compareSingleVariable( db, "nvtx", "Number of Reconstructed Vertexes", "", h1_nvtx_thq_old, h1_nvtx_tth_old, h1_nvtx_thq_new, h1_nvtx_tth_new );

  return 0;

}


void compareSingleVariable( DrawBase* db, const std::string& varName, const std::string& axisName, const std::string& units, TH1D* h1_qJet_thq_old, TH1D* h1_qJet_tth_old, TH1D* h1_qJet_thq_new, TH1D* h1_qJet_tth_new ) {

  h1_qJet_thq_old->SetLineColor( 46 );  
  h1_qJet_thq_new->SetLineColor( kRed+1 );  
  h1_qJet_tth_old->SetLineColor( 38 );  
  h1_qJet_tth_new->SetLineColor( kBlue );  

  h1_qJet_thq_old->SetLineWidth( 2 );  
  h1_qJet_thq_new->SetLineWidth( 2 );  
  h1_qJet_tth_old->SetLineWidth( 2 );  
  h1_qJet_tth_new->SetLineWidth( 2 );  

  h1_qJet_thq_old->SetLineStyle( 2 );  
  h1_qJet_thq_new->SetLineStyle( 1 );  
  h1_qJet_tth_old->SetLineStyle( 2 );  
  h1_qJet_tth_new->SetLineStyle( 1 );  

  float yMax = h1_qJet_thq_old->GetMaximum();
  if( h1_qJet_thq_new->GetMaximum()>yMax ) yMax = h1_qJet_thq_new->GetMaximum();
  if( h1_qJet_tth_old->GetMaximum()>yMax ) yMax = h1_qJet_tth_old->GetMaximum();
  if( h1_qJet_tth_new->GetMaximum()>yMax ) yMax = h1_qJet_tth_new->GetMaximum();

  yMax *= 1.3;

  float xMin = h1_qJet_thq_old->GetXaxis()->GetXmin();
  float xMax = h1_qJet_thq_old->GetXaxis()->GetXmax();

  std::string fullAxisTitle = axisName;
  if( units!="" ) fullAxisTitle = fullAxisTitle + " [" + units + "]";


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();


  float xMin_leg = (varName=="lept_charge") ? 0.25 : 0.65;
  float xMax_leg = (varName=="lept_charge") ? 0.5 : 0.9;

  TLegend* legend = new TLegend( xMin_leg, 0.6, xMax_leg, 0.9 );
  legend->SetTextSize(0.040);
  legend->SetFillColor(0);
  legend->AddEntry( h1_qJet_thq_old, "tHq (old)", "L" );
  legend->AddEntry( h1_qJet_thq_new, "tHq (new)", "L" );
  legend->AddEntry( h1_qJet_tth_old, "ttH (old)", "L" );
  legend->AddEntry( h1_qJet_tth_new, "ttH (new)", "L" );

  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, xMax, 10, 0., yMax );
  h2_axes->SetXTitle( fullAxisTitle.c_str() );
  h2_axes->SetYTitle( "Normalized to Unity" );

  h2_axes->Draw("");
  legend->Draw("same");
  
  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  h1_qJet_thq_old->DrawNormalized( "histo same" );  
  h1_qJet_thq_new->DrawNormalized( "histo same" );  
  h1_qJet_tth_old->DrawNormalized( "histo same" );  
  h1_qJet_tth_new->DrawNormalized( "histo same" );  

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/"  + varName;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
 
  c1->SaveAs( canvasName_eps.c_str() );
  c1->SaveAs( canvasName_png.c_str() );

  delete c1;
  delete legend;
  delete h2_axes;

}
