#include <iostream>

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"




void drawOneEffVsPU( DrawBase* db, const std::string& batchProd, const std::string& dataset, bool applyLDcut=false );


int main( int argc, char* argv[] ) {

  
  std::string batchProd = "qJetEtaFix_newLD_v1";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }



  DrawBase* db = new DrawBase("THq");
  std::string outputdir_str = "THq_plots_"+batchProd;
  db->set_outputdir( outputdir_str );

  drawOneEffVsPU( db, batchProd, "thqLeptonic_m125_8TeV" );
  drawOneEffVsPU( db, batchProd, "thqLeptonic_m125_8TeV", true );
  drawOneEffVsPU( db, batchProd, "tth_m125_8TeV" );
  drawOneEffVsPU( db, batchProd, "tth_m125_8TeV", true );

  return 0;

}





void drawOneEffVsPU( DrawBase* db, const std::string& batchProd, const std::string& dataset, bool applyLDcut ) {

  std::string inputDir = "dbFiles_"+batchProd+"/";

  std::string filename = inputDir + dataset + ".root";
  TFile* file = TFile::Open( filename.c_str() );
  TTree* tree = (TTree*)file->Get("tree_passedEvents");

  int category;
  tree->SetBranchAddress( "category", &category ); 

  float nvtx;
  tree->SetBranchAddress( "nvtx", &nvtx ); 

  float weight;
  tree->SetBranchAddress( "weight", &weight ); 

  float evweight;
  tree->SetBranchAddress( "evweight", &evweight ); 

  float thqLD_lept;
  tree->SetBranchAddress( "thqLD_lept", &thqLD_lept ); 


  int nBins = 6;
  float xMin = 4.5;
  float xMax = 35.5;

  TH1D* h1_effNum_vs_nvtx = new TH1D( "effNum_vs_nvtx", "", nBins, xMin, xMax );
  h1_effNum_vs_nvtx->Sumw2();

  TH1D* h1_effDenom_vs_nvtx = new TH1D( "effDenom_vs_nvtx", "", nBins, xMin, xMax );
  h1_effDenom_vs_nvtx->Sumw2();

  TH1D* h1_Ntot_vs_nvtx = new TH1D( "Ntot_vs_nvtx", "", nBins, xMin, xMax );

  int nentries = tree->GetEntries();
  float effMeanDenom = 0.;
  float effMeanNum = 0.;

  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);

    h1_effDenom_vs_nvtx->Fill( nvtx, evweight );
    effMeanDenom += evweight;

    h1_Ntot_vs_nvtx->Fill( nvtx );

    if( category!=11 ) continue;
    if( applyLDcut && thqLD_lept<0.25 ) continue;

    h1_effNum_vs_nvtx->Fill( nvtx, evweight );
    effMeanNum += evweight;

  }

  float effMean = effMeanNum/effMeanDenom;

  TH1D* h1_eff_vs_nvtx = new TH1D( "eff_vs_nvtx", "", nBins, xMin, xMax );
  float effMax = 0.;
  float effMin = 1.;
  

  for( unsigned iBin=1; iBin<h1_eff_vs_nvtx->GetNbinsX()+1; ++iBin ) {

    float effNum   = h1_effNum_vs_nvtx->GetBinContent(iBin);
    float effDenom = h1_effDenom_vs_nvtx->GetBinContent(iBin);
    float Ntot = h1_Ntot_vs_nvtx->GetBinContent(iBin);
  
    float eff = effNum/effDenom;
    float effErr = sqrt( eff*(1.-eff) / Ntot );

    if( eff>effMax ) effMax = eff;
    if( eff<effMin ) effMin = eff;

    h1_eff_vs_nvtx->SetBinContent( iBin, eff );
    h1_eff_vs_nvtx->SetBinError( iBin, effErr );

  }


  TCanvas* c1 = new TCanvas("c1", "", 600, 600 );
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, effMin*0.8, effMax*1.2 );
  h2_axes->SetXTitle( "Number of Reconstructed Vertexes" );
  h2_axes->SetYTitle( "Efficiency" );
  h2_axes->Draw();

  h1_eff_vs_nvtx->SetMarkerStyle(20);
  h1_eff_vs_nvtx->SetMarkerSize(2);
  h1_eff_vs_nvtx->SetMarkerColor(46);

  h1_eff_vs_nvtx->Draw("P same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  TPaveText* effMaxText = new TPaveText( 0.23, 0.3, 0.5, 0.35, "brNDC" );
  effMaxText->SetFillColor(0);
  effMaxText->SetTextSize(0.035);
  effMaxText->SetTextAlign(11);
  effMaxText->AddText(Form("Max efficiency = %.3f",effMax));
  effMaxText->Draw("same");

  TPaveText* effMinText = new TPaveText( 0.23, 0.25, 0.5, 0.3, "brNDC" );
  effMinText->SetFillColor(0);
  effMinText->SetTextSize(0.035);
  effMinText->SetTextAlign(11);
  effMinText->AddText(Form("Min efficiency = %.3f",effMin));
  effMinText->Draw("same");

  TPaveText* effSystText = new TPaveText( 0.23, 0.2, 0.5, 0.25, "brNDC" );
  effSystText->SetFillColor(0);
  effSystText->SetTextSize(0.035);
  effSystText->SetTextAlign(11);
  effSystText->AddText(Form("Max difference = %.1f%%",100.*(effMax-effMin)/effMean));
  effSystText->Draw("same");

  gPad->RedrawAxis();

  std::string canvasName = db->get_outputdir() + "/eff_vs_nvtx_" + dataset;
  if( applyLDcut ) canvasName += "_LDcut";
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";

  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  delete c1;
  delete h2_axes;
  delete h1_eff_vs_nvtx;
  delete h1_effNum_vs_nvtx;
  delete h1_effDenom_vs_nvtx;

}
