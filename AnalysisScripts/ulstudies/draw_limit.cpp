#include <iostream>
#include "TH1D.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "CommonTools/DrawBase.h"



int main() {

  DrawBase* db = new DrawBase("checkUL");

  TFile* file_0001 = TFile::Open("combinedLimit_SAVE0001.root");
  TFile* file_001  = TFile::Open("combinedLimit_SAVE001.root");
  TFile* file_01   = TFile::Open("combinedLimit_SAVE01.root");
  TFile* file_1    = TFile::Open("combinedLimit_SAVE1.root");
  TFile* file_10   = TFile::Open("combinedLimit_SAVE10.root");

  TTree* tree_0001 = (TTree*)file_0001->Get("limit");
  TTree* tree_001  = (TTree*)file_001 ->Get("limit");
  TTree* tree_01   = (TTree*)file_01  ->Get("limit");
  TTree* tree_1    = (TTree*)file_1   ->Get("limit");
  TTree* tree_10   = (TTree*)file_10  ->Get("limit");

  TH1D* h_0001 = new TH1D("limit_0001", "", 600, 0., 20.);
  TH1D* h_001  = new TH1D("limit_001", "", 600, 0., 20.);
  TH1D* h_01   = new TH1D("limit_01", "", 600, 0., 20.);
  TH1D* h_1    = new TH1D("limit_1", "", 600, 0., 20.);
  TH1D* h_10   = new TH1D("limit_10", "", 600, 0., 20.);

  tree_0001->Project("limit_0001", "limit");
  tree_001 ->Project("limit_001", "limit");
  tree_01  ->Project("limit_01", "limit");
  tree_1   ->Project("limit_1", "limit");
  tree_10  ->Project("limit_10", "limit");

  h_0001->SetLineColor(2);
  h_001 ->SetLineColor(3);
  h_01  ->SetLineColor(4);
  h_1   ->SetLineColor(6);
  h_10  ->SetLineColor(7);

  h_0001->SetLineWidth(2);
  h_001 ->SetLineWidth(2);
  h_01  ->SetLineWidth(2);
  h_1   ->SetLineWidth(2);
  h_10  ->SetLineWidth(2);

  TLegend* legend = new TLegend( 0.6, 0.6, 0.9, 0.9);
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  legend->AddEntry( h_0001, "k_BG = 0.001", "L" );
  legend->AddEntry( h_001,  "k_BG = 0.01", "L" );
  legend->AddEntry( h_01,   "k_BG = 0.1", "L" );
  legend->AddEntry( h_1,    "k_BG = 1", "L" );
  legend->AddEntry( h_10,   "k_BG = 10", "L" );


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();
  
  
  float ymax = h_0001->GetMaximum()/h_0001->Integral();


  TH2D* h2_axes = new TH2D("axes", "", 10, 2., 8., 10, 0., ymax*1.1);
  h2_axes->SetXTitle("95% UL");
  h2_axes->SetYTitle("Normalized to Unity");
  h2_axes->Draw();

  h_0001->DrawNormalized("same");
  h_001 ->DrawNormalized("same");
  h_01  ->DrawNormalized("same");
  h_1   ->DrawNormalized("same");
  h_10  ->DrawNormalized("same");

  legend->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("checkUL.eps");

  std::cout << "mean/rms: " << std::endl;
  std::cout << h_0001->GetMean() << " " << h_0001->GetRMS() << std::endl;
  std::cout << h_001 ->GetMean() << " " << h_001 ->GetRMS() << std::endl;
  std::cout << h_01  ->GetMean() << " " << h_01  ->GetRMS() << std::endl;
  std::cout << h_1   ->GetMean() << " " << h_1   ->GetRMS() << std::endl;
  std::cout << h_10  ->GetMean() << " " << h_10  ->GetRMS() << std::endl;


  return 0;

}
