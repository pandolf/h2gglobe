#include <iostream>

#include "CommonTools/DrawBase.h"




void compareSingleVariable( DrawBase* db, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units="", const std::string& additionalCuts="", const std::string suffix=""  );
void drawRoc( DrawBase* db );
void compareTaggers( DrawBase* db, const std::string& varName1, const std::string& varName2, const std::string& legendName1, const std::string& legendName2, int nBins=0.25, float xMin=0., float xMax=1.0001 );



int main() {


  DrawBase* db = new DrawBase("rocs_and_shit");

  TFile* file = TFile::Open("../batchOutput_provaLD2/histograms_CMS-HGG.root");
  db->add_mcFile( file, "thefile", "tHq", kBlack, 0);
  db->set_outputdir("RoCs_andShit");

  

  compareSingleVariable( db, "njets_InsideEtaCut", 9, -0.5, 8.5,  "Central Jet Multiplicity (|#eta|<1)", "" );
  compareSingleVariable( db, "njets_InsideEtaCut", 9, -0.5, 8.5,  "Central Jet Multiplicity (|#eta|<1)", "", "thqLD_lept>0.25", "LDcut" );
  compareSingleVariable( db, "njets",              9, -0.5, 8.5,  "Jet Multiplicity", "" );
  compareSingleVariable( db, "qJetEta",            30, 0., 5.,    "qJet Pseudorapidity", "" );
  compareSingleVariable( db, "topMt",              50, 0., 1000., "Top Transverse Mass", "GeV" );
  compareSingleVariable( db, "lept_charge",        3, -1.5, 1.5,  "Lepton Charge", "" );
  compareSingleVariable( db, "deltaEta_lept_qJet", 35, 0., 7.,    "#Delta#eta(lepton-qJet)", "" );

  compareSingleVariable( db, "thqLD_lept", 25, 0., 1.0001,    "tHq Leptonic LD" );
  compareSingleVariable( db, "thqLD_lept_2", 25, 0., 1.0001,    "tHq Leptonic LD" );
  compareSingleVariable( db, "thqLD_lept_2_central", 25, 0., 1.0001,    "tHq Leptonic LD (central jets)" );
  compareSingleVariable( db, "thqBDT_lept", 25, -1., 1.0001,    "tHq Leptonic BDT" );

  compareTaggers( db, "thqLD_lept_2", "thqLD_lept_2_central", "Old LD", "New LD" );

  drawRoc(db);

  return 0;

}



void compareSingleVariable( DrawBase* db, const std::string& varName, int nBins, float xMin, float xMax, const std::string& axisName, const std::string& units, const std::string& additionalCuts, const std::string suffix  ) {

  TFile* file = db->get_mcFile("thefile").file;

  TTree* tree_thq = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");

  TH1D* h1_thq = new TH1D( "thq", "", nBins, xMin, xMax );
  TH1D* h1_tth = new TH1D( "tth", "", nBins, xMin, xMax );

  h1_thq->Sumw2();
  h1_tth->Sumw2();

  std::string fullSelection;
  if( additionalCuts=="" ) fullSelection = "weight*( category==11 )";
  else                     fullSelection = "weight*( category==11 && " + additionalCuts + ")";

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




void drawRoc( DrawBase* db ) {

  TH1F::AddDirectory(kTRUE);

  TFile* file = db->get_mcFile("thefile").file;

  TTree* tree_thq = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");

  TH1F* h1_LD_signal = new TH1F("LD_signal", "", 100, 0., 1.0001);
  TH1F* h1_LD_bg = new TH1F("LD_bg", "", 100, 0., 1.0001);

  TH1F* h1_LD_new_signal = new TH1F("LD_new_signal", "", 100, 0., 1.0001);
  TH1F* h1_LD_new_bg     = new TH1F("LD_new_bg", "", 100, 0., 1.0001);

  TH1F* h1_LD_central_signal = new TH1F("LD_central_signal", "", 100, 0., 1.0001);
  TH1F* h1_LD_central_bg = new TH1F("LD_central_bg", "", 100, 0., 1.0001);

  TH1F* h1_BDT_signal = new TH1F("BDT_signal", "", 100, -1, 1.0001);
  TH1F* h1_BDT_bg = new TH1F("BDT_bg", "", 100, -1, 1.0001);

  tree_thq->Project( "LD_signal", "thqLD_lept", "weight*(category==11)" );
  tree_thq->Project( "LD_new_signal", "thqLD_lept_2", "weight*(category==11)" );
  tree_thq->Project( "LD_central_signal", "thqLD_lept_2_central", "weight*(category==11)" );
  tree_thq->Project( "BDT_signal", "thqBDT_lept", "weight*(category==11)" );

  tree_tth->Project( "LD_bg", "thqLD_lept", "weight*(category==11)" );
  tree_tth->Project( "LD_new_bg", "thqLD_lept_2", "weight*(category==11)" );
  tree_tth->Project( "LD_central_bg", "thqLD_lept_2_central", "weight*(category==11)" );
  tree_tth->Project( "BDT_bg", "thqBDT_lept", "weight*(category==11)" );



  TGraph* gr_RoC_LD = new TGraph(0);
  TGraph* gr_RoC_LD_new = new TGraph(0);
  TGraph* gr_RoC_LD_central = new TGraph(0);
  TGraph* gr_RoC_BDT = new TGraph(0);

  int nbins = h1_LD_signal->GetNbinsX();

  for( unsigned int ibin=1; ibin<nbins+1; ++ibin ) {

    //float eff_s_BDT = h1_BDT_signal->Integral( nbins-ibin, nbins )/h1_BDT_signal->Integral( 1, nbins );
    //float eff_b_BDT = h1_BDT_bg->Integral( nbins-ibin, nbins )/h1_BDT_bg->Integral( 1, nbins );

    //gr_RoC_BDT->SetPoint( ibin-1, 1.-eff_b_BDT, eff_s_BDT );
  
    float eff_s_LD = h1_LD_signal->Integral( nbins-ibin, nbins )/h1_LD_signal->Integral( 1, nbins );
    float eff_b_LD = h1_LD_bg->Integral( nbins-ibin, nbins )/h1_LD_bg->Integral( 1, nbins );
  
    gr_RoC_LD->SetPoint( ibin-1, 1.-eff_b_LD, eff_s_LD );

    float eff_s_LD_new = h1_LD_new_signal->Integral( nbins-ibin, nbins )/h1_LD_new_signal->Integral( 1, nbins );
    float eff_b_LD_new = h1_LD_new_bg->Integral( nbins-ibin, nbins )/h1_LD_new_bg->Integral( 1, nbins );
  
    gr_RoC_LD_new->SetPoint( ibin-1, 1.-eff_b_LD_new, eff_s_LD_new );

    float eff_s_LD_central = h1_LD_central_signal->Integral( nbins-ibin, nbins )/h1_LD_central_signal->Integral( 1, nbins );
    float eff_b_LD_central = h1_LD_central_bg->Integral( nbins-ibin, nbins )/h1_LD_central_bg->Integral( 1, nbins );
  
    gr_RoC_LD_central->SetPoint( ibin-1, 1.-eff_b_LD_central, eff_s_LD_central );

    float eff_s_BDT = h1_BDT_signal->Integral( nbins-ibin, nbins )/h1_BDT_signal->Integral( 1, nbins );
    float eff_b_BDT = h1_BDT_bg->Integral( nbins-ibin, nbins )/h1_BDT_bg->Integral( 1, nbins );
  
    gr_RoC_BDT->SetPoint( ibin-1, 1.-eff_b_BDT, eff_s_BDT );

  }


  //gr_RoC_LD->SetMarkerSize(1.3);
  //gr_RoC_LD->SetMarkerStyle(21);
  //gr_RoC_LD->SetMarkerColor(29);

  gr_RoC_LD_new->SetMarkerSize(1.3);
  gr_RoC_LD_new->SetMarkerStyle(20);
  gr_RoC_LD_new->SetMarkerColor(kOrange+1);

  gr_RoC_LD_central->SetMarkerSize(1.3);
  gr_RoC_LD_central->SetMarkerStyle(24);
  gr_RoC_LD_central->SetMarkerColor(kRed+3);

  gr_RoC_BDT->SetMarkerSize(1.3);
  gr_RoC_BDT->SetMarkerStyle(21);
  gr_RoC_BDT->SetMarkerColor(29);



  TCanvas* c1 = new TCanvas("c1_roc", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes_roc", "", 10, 0., 1.0001, 10, 0., 1.0001);
  h2_axes->SetXTitle( "t#bar{t}H Rejection" );
  h2_axes->SetYTitle( "tHq Efficiency" );

  h2_axes->Draw();

  TLine* diag = new TLine(0., 1., 1., 0.);
  diag->Draw("same");


  TLegend* legend = new TLegend( 0.2, 0.2, 0.45, 0.45, "Leptonic Channel" );
  legend->SetFillColor(0);
  legend->SetTextSize(0.04);
  legend->AddEntry( gr_RoC_LD_new, "Nominal LD", "P");
  legend->AddEntry( gr_RoC_LD_central, "New LD (central jets)", "P");
  legend->AddEntry( gr_RoC_BDT, "BDT", "P");
  legend->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");


  
  gr_RoC_BDT->Draw("p same");
  //gr_RoC_LD->Draw("p same");
  gr_RoC_LD_new->Draw("p same");
  gr_RoC_LD_central->Draw("p same");

  gPad->RedrawAxis();

  char canvasName[500];
  sprintf( canvasName, "%s/RoC.eps", db->get_outputdir().c_str() );
  c1->SaveAs(canvasName);

  delete c1;
  delete h2_axes;
  delete legend;
}




void compareTaggers( DrawBase* db, const std::string& varName1, const std::string& varName2, const std::string& legendName1, const std::string& legendName2, int nBins, float xMin, float xMax ) {


  TFile* file = db->get_mcFile("thefile").file;

  TTree* tree_thq = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");

  TH1F* h1_signal1 = new TH1F("signal1", "", 25, 0., 1.0001);
  TH1F* h1_bg1 = new TH1F("bg1", "", 25, 0., 1.0001);

  TH1F* h1_signal2 = new TH1F("signal2", "", 25, 0., 1.0001);
  TH1F* h1_bg2 = new TH1F("bg2", "", 25, 0., 1.0001);


  tree_thq->Project( "signal1", varName1.c_str(), "weight*(category==11)" );
  tree_thq->Project( "signal2", varName2.c_str(), "weight*(category==11)" );

  tree_tth->Project( "bg1", varName1.c_str(), "weight*(category==11)" );
  tree_tth->Project( "bg2", varName2.c_str(), "weight*(category==11)" );


  h1_signal1->SetFillColor(46);
  h1_signal1->SetLineColor(46);
  h1_signal1->SetLineWidth(2);
  h1_signal1->SetFillStyle(3004);

  h1_bg1->SetFillColor(38);
  h1_bg1->SetLineColor(38);
  h1_bg1->SetLineWidth(2);
  h1_bg1->SetFillStyle(3005);

  h1_signal2->SetLineColor(kRed+1);
  h1_signal2->SetLineWidth(2);
  h1_signal2->SetLineStyle(2);

  h1_bg2->SetLineColor(kBlue+1);
  h1_bg2->SetLineWidth(2);
  h1_bg2->SetLineStyle(2);



  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  std::string legendName_bg1 = "ttH / tt+#gamma#gamma (" + legendName1 + ")";
  std::string legendName_bg2 = "ttH / tt+#gamma#gamma (" + legendName2 + ")";
  std::string legendName_signal1 = "tHq (" + legendName1 + ")";
  std::string legendName_signal2 = "tHq (" + legendName2 + ")";

  TLegend* legend1 = new TLegend( 0.3, 0.6, 0.7, 0.9 );
  legend1->SetTextSize(0.04);
  legend1->SetFillColor(0);
  legend1->AddEntry( h1_bg1, legendName_bg1.c_str(), "F" );
  legend1->AddEntry( h1_bg2, legendName_bg2.c_str(), "L" );
  legend1->AddEntry( h1_signal1, legendName_signal1.c_str(), "F" );
  legend1->AddEntry( h1_signal2, legendName_signal2.c_str(), "L" );

  TLegend* legend2 = new TLegend( 0.55, 0.7, 0.9, 0.9 );
  legend2->SetTextSize(0.035);
  legend2->SetFillColor(0);
  legend2->AddEntry( h1_signal1, legendName_signal1.c_str(), "F" );
  legend2->AddEntry( h1_signal2, legendName_signal2.c_str(), "L" );

  float yMax = h1_signal1->GetMaximum();
  if( h1_signal2->GetMaximum()>yMax ) yMax = h1_signal2->GetMaximum();
  if( h1_bg1->GetMaximum()>yMax ) yMax = h1_bg1->GetMaximum();
  if( h1_bg2->GetMaximum()>yMax ) yMax = h1_bg2->GetMaximum();
  yMax *= 1.3;

  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, xMax, 10, 0., yMax );
  h2_axes->SetXTitle( "Tagger Value" );
  h2_axes->SetYTitle( "Normalized to Unity" );

  h2_axes->Draw("");
  legend1->Draw("same");
  //legend2->Draw("same");
  
  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  h1_bg1->DrawNormalized( "histo same" );  
  h1_signal1->DrawNormalized( "histo same" );  
  h1_bg2->DrawNormalized( "histo same" );  
  h1_signal2->DrawNormalized( "histo same" );  

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/compare_"  + varName1 + "_vs_" + varName2;
  //if( suffix!="" ) canvasName = canvasName + "_" + suffix;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
 
  c1->SaveAs( canvasName_eps.c_str() );
  c1->SaveAs( canvasName_png.c_str() );

  delete c1;
  delete legend1;
  delete legend2;
  delete h2_axes;
  delete h1_signal1;
  delete h1_signal2;
  delete h1_bg1;
  delete h1_bg2;

}

