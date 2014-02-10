#include <iostream>

#include "TFile.h"
#include "TH1D.h"
#include "TTree.h"

#include "CommonTools/DrawBase.h"



void compareSingleVariable( DrawBase* db, TTree* tree_thq, TTree* tree_tth, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, const std::string& additionalCuts="", const std::string suffix=""  );



int main() {


  DrawBase* db = new DrawBase("kk");
  db->set_outputdir("plotsCheckLD");


  std::string fileName = "../thq_jetsyst_v11/jerCentral/histograms_CMS-HGG.root"; 
  TFile* file = TFile::Open(fileName.c_str());
  TTree* tree_thq = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");


  compareSingleVariable( db, tree_thq, tree_tth, "njets_InsideEtaCut", 9, -0.5, 8.5,  "Central Jet Multiplicity (#|eta|<1)", "" );
  compareSingleVariable( db, tree_thq, tree_tth, "njets_InsideEtaCut", 9, -0.5, 8.5,  "Central Jet Multiplicity (#|eta|<1)", "", "thqLD_lept>0.25", "LDcut" );
  compareSingleVariable( db, tree_thq, tree_tth, "njets",              9, -0.5, 8.5,  "Jet Multiplicity", "" );
  compareSingleVariable( db, tree_thq, tree_tth, "qJetEta",            30, 0., 5.,    "qJet Pseudorapidity", "" );
  compareSingleVariable( db, tree_thq, tree_tth, "topMt",              50, 0., 1000., "Top Transverse Mass", "GeV" );
  compareSingleVariable( db, tree_thq, tree_tth, "lept_charge",        3, -1.5, 1.5,  "Lepton Charge", "" );
  compareSingleVariable( db, tree_thq, tree_tth, "deltaEta_lept_qJet", 35, 0., 7.,    "#Delta#eta(lepton-qJet)", "" );

  compareTwoTaggers( db, tree_thq, tree_tth, "thqLD_lept_2", "thqLD_lept_2_central", "Old LD (all jets)", "New LD (central jets)" );

  return 0;

}


void compareSingleVariable( DrawBase* db, TTree* tree_thq, TTree* tree_tth, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, const std::string& additionalCuts, const std::string suffix  ) {

  TH1D* h1_thq = new TH1D( "thq", "", nBins, xMin, xMax );
  TH1D* h1_tth = new TH1D( "tth", "", nBins, xMin, xMax );

  h1_thq->Sumw2();
  h1_tth->Sumw2();

  std::string fullSelection;
  if( additionalCuts=="" ) fullSelection = "evweight*( category==11 )";
  else                     fullSelection = "evweight*( category==11 && " + additionalCuts + ")";

  tree_thq->Project("thq", varName.c_str(), fullSelection.c_str() );
  tree_tth->Project("tth", varName.c_str(), fullSelection.c_str() );

  h1_thq->Scale(1./h1_thq->Integral());
  h1_tth->Scale(1./h1_tth->Integral());

  h1_thq->SetLineColor( 46 );
  h1_tth->SetLineColor( 38 );  

  h1_thq->SetFillColor( 46 );
  h1_tth->SetFillColor( 38 );  

  h1_thq->SetFillStyle( 3005 );
  h1_tth->SetFillStyle( 3004 );  

  h1_thq->SetLineWidth( 2 );  
  h1_tth->SetLineWidth( 2 );  


  float yMax = h1_thq->GetMaximum();
  if( h1_tth->GetMaximum()>yMax ) yMax = h1_tth->GetMaximum();
  yMax *= 1.3;

  std::string fullAxisTitle = axisName;
  if( units!="" ) fullAxisTitle = fullAxisTitle + " [" + units + "]";


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();


  TLegend* legend = new TLegend( 0.55, 0.7, 0.9, 0.9, "Leptonic Channel" );
  legend->SetTextSize(0.040);
  legend->SetFillColor(0);
  legend->AddEntry( h1_thq, "tHq", "F" );
  legend->AddEntry( h1_tth, "ttH / tt+#gamma#gamma", "F" );

  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, xMax, 10, 0., yMax );
  h2_axes->SetXTitle( fullAxisTitle.c_str() );
  h2_axes->SetYTitle( "Normalized to Unity" );

  h2_axes->Draw("");
  legend->Draw("same");
  
  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  h1_tth->DrawNormalized( "histo same" );  
  h1_thq->DrawNormalized( "histo same" );  

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/"  + varName;
  if( suffix!="" ) canvasName = canvasName + "_" + suffix;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
 
  c1->SaveAs( canvasName_eps.c_str() );
  c1->SaveAs( canvasName_png.c_str() );

  delete c1;
  delete legend;
  delete h2_axes;
  delete h1_thq;
  delete h1_tth;

}


void compareTwoTaggers( db, tree_thq, tree_tth, "thqLD_lept_2", "thqLD_lept_2_central", "Old LD (all jets)", "New LD (central jets)" );
