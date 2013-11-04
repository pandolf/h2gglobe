#include <iostream>

#include "CommonTools/DrawBase.h"




void findVariableThreshold( DrawBase* db, const std::string& varName, float fraction );



int main() {


  DrawBase* db = new DrawBase("rocs_and_shit");
  

  TFile* file = TFile::Open("../batchOutput_provaLD1/histograms_CMS-HGG.root");
  db->add_mcFile( file, "thefile", "tHq", kBlack, 0);
  db->set_outputdir("RoCs_andShit");

  findVariableThreshold(db, "thqLD_lept", 0.1);
  findVariableThreshold(db, "thqLD_lept_2", 0.1);
  findVariableThreshold(db, "thqLD_lept_2_central", 0.1);
  //findVariableThreshold(db, "thqBDT_lept", 0.1);

  return 0;

}





void findVariableThreshold( DrawBase* db, const std::string& varName, float fraction ) {

  TH1F::AddDirectory(kTRUE);

  TFile* file = db->get_mcFile("thefile").file;

  TTree* tree_thq = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");

  TString varName_tstr(varName);
  bool isBDT = varName_tstr.Contains("BDT");
  std::string taggerName = (isBDT) ? "BDT" : "LD";
  float xMin = (isBDT) ? -1. : 0.;

  int nBins = 100;
  TH1F* h1_LD_signal = new TH1F("LD_signal", "", nBins, xMin, 1.0001);
  TH1F* h1_LD_bg = new TH1F("LD_bg", "", nBins, xMin, 1.0001);


  tree_thq->Project( "LD_signal", varName.c_str(), "weight*(category==11)" );
  tree_tth->Project( "LD_bg", varName.c_str(), "weight*(category==11)" );


  TH1D* h1_fraction_vs_cut = new TH1D("fraction_vs_cut", "", nBins, xMin, 1.0001 );

  float foundCut=0.;
  bool firstTime = true;
  
  for( unsigned iBin=1; iBin<nBins+1; ++iBin ) {

    float sYield = h1_LD_signal->Integral( iBin, nBins );
    float bgYield = h1_LD_bg->Integral( iBin, nBins );

    float bgFrac = bgYield/sYield;

    h1_fraction_vs_cut->SetBinContent( iBin, bgFrac );

    if( bgFrac<0.1 && firstTime ) {
      foundCut = h1_fraction_vs_cut->GetBinLowEdge(iBin);
      firstTime=false;
    }

  } 


  h1_fraction_vs_cut->SetMarkerStyle(20);
  h1_fraction_vs_cut->SetMarkerSize(1.6);
  h1_fraction_vs_cut->SetMarkerColor(46);

  float yMax = h1_LD_bg->Integral()/h1_LD_signal->Integral();
  yMax *= 1.1;


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 1.0001, 10, 0., yMax );
  h2_axes->SetYTitle( "ttH Contamination" );
  char xAxisTitle[200];
  sprintf( xAxisTitle, "%s Cut Threshold", taggerName.c_str() );
  h2_axes->SetXTitle( xAxisTitle );

  h2_axes->Draw();


  TLine* lineThresh = new TLine( xMin, fraction, 1., fraction );
  lineThresh->Draw("same");

  TLine* lineFoundThresh = new TLine( foundCut, 0., foundCut, fraction );
  lineFoundThresh->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  char threshFoundText[200];
  sprintf( threshFoundText, "ttH < 10%% tHq requires %s > %.2f", taggerName.c_str(), foundCut );

  TPaveText* threshFoundLabel = new TPaveText( 0.45, 0.5, 0.7, 0.6, "brNDC" );
  threshFoundLabel->SetTextSize(0.035);
  threshFoundLabel->SetFillColor(0);
  threshFoundLabel->AddText( threshFoundText );
  threshFoundLabel->Draw("same");

  h1_fraction_vs_cut->Draw("p same");
  
  gPad->RedrawAxis();

  char canvasName[500];
  sprintf( canvasName, "%s/fractionVsCut_%s.eps", db->get_outputdir().c_str(), varName.c_str() );
  c1->SaveAs(canvasName);

  std::cout << "-> Variable: " << varName << " found threshold: " << foundCut << " (" << 100.*fraction << "% ttH contamination)" << std::endl;

  delete c1;
  delete h2_axes;
  delete h1_fraction_vs_cut;
  delete h1_LD_bg;
  delete h1_LD_signal;

}


