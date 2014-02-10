#include <iostream>

#include "DrawBase.h"







int main( int argc, char* argv[] ) {


  std::string batchProd = "postFreezeNLO_v2";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }


  DrawBase* db = new DrawBase("significance");

  std::string fileName = "q_" + batchProd + ".root";
  TFile* qFile = TFile::Open(fileName.c_str());
  TTree* qTree = (TTree*)qFile->Get("q");

  int nbins = 1000;
  TH1D* h1_q_signal = new TH1D("q_signal", "", nbins, -2., 16.);
  TH1D* h1_q_bg     = new TH1D("q_bg"    , "", nbins, -2., 16.);

  qTree->Project( "q_signal", "q", "type>0");
  qTree->Project( "q_bg"    , "q", "type<0");

  float int_signal = h1_q_signal->Integral();
  float int_bg     = h1_q_bg    ->Integral();

  h1_q_signal->Scale( 1./int_signal );
  h1_q_bg    ->Scale( 1./int_bg );


  int Nobs = 0;
  float cumulative_signal=0.;
  float cumulative_bg=0.;
  TH1D* h1_cumulative_signal = new TH1D("cumulative_signal", "", 6, -0.5, 5.5);
  TH1D* h1_cumulative_bg = new TH1D("cumulative_bg", "", 6, -0.5, 5.5);
  
  // first look for probabilities:
  for( unsigned ibin=1; ibin<nbins; ++ibin ) {

    // only some bins are filled, corresponding to N=0,1,2,3...
    if( h1_q_signal->GetBinContent(ibin)==0. ) continue; 

    std::cout << Nobs << " & " << h1_q_bg->GetBinContent(ibin) << " & " << h1_q_signal->GetBinContent(ibin) << " \\\\" << std::endl;

    cumulative_signal += h1_q_signal->GetBinContent(ibin); 
    cumulative_bg += h1_q_bg->GetBinContent(ibin); 
    h1_cumulative_signal->SetBinContent( Nobs+1, cumulative_signal );
    h1_cumulative_bg->SetBinContent( Nobs+1, cumulative_bg );

    Nobs += 1;

  }  //for bins



  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, -0.5, 5.5, 10, 0., 1.3);
  h2_axes->SetXTitle("Number of Observed Events");
  h2_axes->SetYTitle("Cumulative Probability");
  h2_axes->Draw("");


  TLegend* legend = new TLegend( 0.2, 0.8, 0.5, 0.9 );
  legend->SetTextSize(0.035);
  legend->SetFillColor(kWhite);
  //legend->AddEntry( h1_cumulative_bg, "B", "L");
  legend->AddEntry( h1_cumulative_signal, "S+B", "L");
  legend->Draw("same");

  TLine* line_one = new TLine( -0.5, 1., 5.5, 1. );
  line_one->Draw("same");


  h1_cumulative_signal->SetLineWidth(2.);
  h1_cumulative_bg    ->SetLineWidth(2.);

  h1_cumulative_signal->SetLineColor(46);
  h1_cumulative_bg    ->SetLineColor(38);

  h1_cumulative_bg    ->SetLineStyle(2);

  h1_cumulative_signal->Draw("same");
  //h1_cumulative_bg    ->Draw("same");

  TLine* line_50 = new TLine( -0.5, 0.5, 5.5, 0.5 );
  line_50->SetLineWidth(2.);
  line_50->Draw("same");

  TLine* line_16 = new TLine( -0.5, 0.16, 5.5, 0.16 );
  line_16->SetLineWidth(2.);
  line_16->SetLineStyle(3);
  line_16->SetLineColor(kGray+2);
  line_16->Draw("same");

  TLine* line_84 = new TLine( -0.5, 0.84, 5.5, 0.84 );
  line_84->SetLineWidth(2.);
  line_84->SetLineStyle(3);
  line_84->SetLineColor(kGray+2);
  line_84->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  TPaveText* medianText = new TPaveText( 3., 0.52, 5.5, 0.57 );
  medianText->SetTextSize(0.035);
  medianText->SetFillColor(0);
  medianText->AddText("Median");
  medianText->Draw("same");

  TPaveText* minusSigmaText = new TPaveText( 3., 0.18, 5.5, 0.23 );
  minusSigmaText->SetTextSize(0.035);
  minusSigmaText->SetTextColor(kGray+2);
  minusSigmaText->SetFillColor(0);
  minusSigmaText->AddText("-1#sigma");
  minusSigmaText->Draw("same");

  TPaveText* plusSigmaText = new TPaveText( 3., 0.77, 5.5, 0.82 );
  plusSigmaText->SetTextSize(0.035);
  plusSigmaText->SetTextColor(kGray+2);
  plusSigmaText->SetFillColor(0);
  plusSigmaText->AddText("+1#sigma");
  plusSigmaText->Draw("same");

  gPad->RedrawAxis();

  std::string canvasName = "prova.eps";
  c1->SaveAs(canvasName.c_str());


  return 0;

}
