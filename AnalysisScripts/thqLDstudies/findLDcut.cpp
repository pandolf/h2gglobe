#include <iostream>

#include "CommonTools/DrawBase.h"


struct LDCutStruct {

  float thresh;
  float thq;
  float tth;

};


LDCutStruct findVariableThreshold( DrawBase* db, const std::string& varName, float fraction, float tthThresh=-1., float thqThresh=-1. );




int main( int argc, char* argv[] ) {


  std::string batchProd = "qJetEtaFix_v3";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }
    

  DrawBase* db = new DrawBase("rocs_and_shit");
  

  std::string fileName = "../batchOutput_" + batchProd + "/histograms_CMS-HGG.root";
  TFile* file = TFile::Open(fileName.c_str());
  db->add_mcFile( file, "thefile", "tHq", kBlack, 0);
  std::string outputdir = "RoCs_andShit_" + batchProd;
  db->set_outputdir(outputdir);

  LDCutStruct cutStruct_LD = findVariableThreshold(db, "thqLD_lept", 0.1);
  //LDCutStruct cutStruct_LD_2 = findVariableThreshold(db, "thqLD_lept_2", 0.1);
  //LDCutStruct cutStruct_LD_2_central = findVariableThreshold(db, "thqLD_lept_2_central", 0.1);
  LDCutStruct cutStruct_LD_BDT = findVariableThreshold(db, "thqBDT_lept", -1., cutStruct_LD.tth );

  return 0;

}





LDCutStruct findVariableThreshold( DrawBase* db, const std::string& varName, float fraction, float tthThresh, float thqThresh ) {

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
  TH1D* h1_thq_vs_cut = new TH1D("thq_vs_cut", "", nBins, xMin, 1.0001 );
  TH1D* h1_tth_vs_cut = new TH1D("tth_vs_cut", "", nBins, xMin, 1.0001 );


  LDCutStruct returnStruct;

  float foundCut=0.;
  bool firstTime = true;
  
  float found_tth=0.;
  bool firstTime_tth = true;
  
  for( unsigned iBin=1; iBin<nBins+1; ++iBin ) {

    float sYield = h1_LD_signal->Integral( iBin, nBins );
    float bgYield = h1_LD_bg->Integral( iBin, nBins );
    bgYield *= 2.4; //ct=-1

    float bgFrac = bgYield/sYield;

    h1_fraction_vs_cut->SetBinContent( iBin, bgFrac );
    h1_thq_vs_cut->SetBinContent( iBin, sYield );
    h1_tth_vs_cut->SetBinContent( iBin, bgYield );

    //if( bgYield<0.1 && firstTime ) {
    if( bgFrac<0.1 && firstTime ) {
      foundCut = h1_fraction_vs_cut->GetBinLowEdge(iBin);
      if( fraction>0. ) {
        returnStruct.thq = sYield;
        returnStruct.tth = bgYield;
        returnStruct.thresh = foundCut;
      }
      firstTime=false;
    }

    if( bgYield<tthThresh && firstTime_tth ) {
      found_tth = h1_tth_vs_cut->GetBinLowEdge(iBin);
      returnStruct.thq = sYield;
      returnStruct.tth = bgYield;
      returnStruct.thresh = found_tth;
      firstTime_tth=false;
    }

  } 


  h1_fraction_vs_cut->SetMarkerStyle(24);
  h1_fraction_vs_cut->SetMarkerSize(1.6);
  h1_fraction_vs_cut->SetMarkerColor(kRed+3);

  h1_thq_vs_cut->SetMarkerStyle(20);
  h1_thq_vs_cut->SetMarkerSize(1.6);
  h1_thq_vs_cut->SetMarkerColor(46);

  h1_tth_vs_cut->SetMarkerStyle(21);
  h1_tth_vs_cut->SetMarkerSize(1.6);
  h1_tth_vs_cut->SetMarkerColor(38);

  //float yMax = h1_LD_bg->Integral()/h1_LD_signal->Integral();
  float yMax = h1_thq_vs_cut->GetMaximum();
  yMax *= 1.15;


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, xMin, 1.0001, 10, 0., yMax );
  //h2_axes->SetYTitle( "ttH Contamination" );
  h2_axes->SetYTitle( "Arbitrary Units");
  char xAxisTitle[200];
  sprintf( xAxisTitle, "%s Cut Threshold", taggerName.c_str() );
  h2_axes->SetXTitle( xAxisTitle );

  TLegend* legend = new TLegend( 0.25, 0.4, 0.55, 0.6 );
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.038);
  legend->AddEntry( h1_thq_vs_cut, "tHq Yield", "P" );
  legend->AddEntry( h1_tth_vs_cut, "ttH Yield", "P" );
  legend->AddEntry( h1_fraction_vs_cut, "ttH / tHq", "P" );

  h2_axes->Draw();
  legend->Draw("same");

  h1_tth_vs_cut->Draw("p same");
  h1_thq_vs_cut->Draw("p same");
  h1_fraction_vs_cut->Draw("p same");

  TLine* lineThresh = new TLine( xMin, 0.1, 1., 0.1 );
  lineThresh->Draw("same");

  TLine* lineFoundThresh = new TLine( foundCut, 0., foundCut, 0.1 );
  lineFoundThresh->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  char threshFoundText[200];
  sprintf( threshFoundText, "ttH < 10%% tHq requires %s > %.2f", taggerName.c_str(), foundCut );

  TPaveText* threshFoundLabel = new TPaveText( 0.5, 0.85, 0.8, 0.9, "brNDC" );
  threshFoundLabel->SetTextSize(0.035);
  threshFoundLabel->SetFillColor(0);
  threshFoundLabel->AddText( threshFoundText );
  threshFoundLabel->Draw("same");

  
  gPad->RedrawAxis();

  char canvasName[500];
  sprintf( canvasName, "%s/fractionVsCut_%s.eps", db->get_outputdir().c_str(), varName.c_str() );
  c1->SaveAs(canvasName);

  if( fraction<0. )
    std::cout << "-> Variable: " << varName << " found threshold: " << found_tth << " (" << tthThresh << " ttH yield)" << std::endl;
  else
    std::cout << "-> Variable: " << varName << " found threshold: " << foundCut << " (" << 100.*fraction << "% ttH contamination)" << std::endl;

  delete c1;
  delete h2_axes;
  delete h1_fraction_vs_cut;
  delete h1_thq_vs_cut;
  delete h1_tth_vs_cut;
  delete h1_LD_bg;
  delete h1_LD_signal;
  delete legend;



  return returnStruct;

}


