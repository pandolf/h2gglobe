#include <iostream>
#include <cmath>

#include "TCanvas.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TF1.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/StatTools.h"



float getAlpha(TF1* f1, float deltaM);

int main() {


  DrawBase* db = new DrawBase("kk");


  TFile* file = TFile::Open("../batchOutput_postFreezeNLO_v2/histograms_CMS-HGG.root");
  TTree* tree_sig = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");

  int nBins = 100;
  TH1D* h1_thq = new TH1D("thq", "", nBins, 0., 1.);
  h1_thq->Sumw2();
  TH1D* h1_tth = new TH1D("tth", "", nBins, 0., 1.);
  h1_tth->Sumw2();

  tree_sig->Project( "thq", "thqLD_lept", "evweight*(category==11 && PhotonsMass>122. && PhotonsMass<128.)");
  tree_tth->Project( "tth", "thqLD_lept", "evweight*(category==11 && PhotonsMass>122. && PhotonsMass<128.)");


  float tth_int = h1_tth->Integral();
  TH1D* h1_bg = new TH1D(*h1_tth); //assume the continuous BG has same LD shape as tth
  float n_sideband_obs = 1.;
  h1_bg->Scale( n_sideband_obs/tth_int ); // and set integral to number of sideband events in data

  // assume exponential BG shape:
  TF1* bg_exp = new TF1( "bg_exp", "exp([0]*x)", 100., 180.);
  bg_exp->SetParameter(0, -0.005);


  TGraph* gr_thq = new TGraph(0);
  TGraph* gr_tth = new TGraph(0);
  TGraph* gr_ZBi_N0 = new TGraph(0);
  TGraph* gr_ZBi_N1 = new TGraph(0);
  TGraph* gr_ZBi_N2 = new TGraph(0);
  TGraph* gr_ZBi_N3 = new TGraph(0);
  TGraph* gr_ZBi_N4 = new TGraph(0);
  TGraph* gr_ZBi_N5 = new TGraph(0);


  int iPoint=0;
  float maxSignif=0.;
  float Qexpmax=0.;

  for( float i = 0.; i<1.; i+=0.01 ) {

    int binMin = h1_thq->FindBin(i);

    float thq = h1_thq->Integral( binMin, nBins+1 );
    float tth = h1_tth->Integral( binMin, nBins+1 );
    float bg = h1_bg->Integral( binMin, nBins+1 ); // this is for N_obs = 1

    // to get signal, take thq, add 1.4*tth, then subtract tth 
    // (we're assuming here tth is a perfectly well-known BG
    //float s = thq + 1.4*tth;
    float s = thq + 1.4*tth - tth;

    float iGev = 3.; //mass counting window is +/-3 gev
    //float alpha_flat = 2.*iGev/(80.-2.*iGev);
    float alpha_exp = getAlpha(bg_exp, iGev);
  
    float ZBi_N0 = StatTools::computeZBiProper( s, 0.*bg, 1./alpha_exp );
    float ZBi_N1 = StatTools::computeZBiProper( s, 1.*bg, 1./alpha_exp );
    float ZBi_N2 = StatTools::computeZBiProper( s, 2.*bg, 1./alpha_exp );
    float ZBi_N3 = StatTools::computeZBiProper( s, 3.*bg, 1./alpha_exp );
    float ZBi_N4 = StatTools::computeZBiProper( s, 4.*bg, 1./alpha_exp );
    float ZBi_N5 = StatTools::computeZBiProper( s, 5.*bg, 1./alpha_exp );

    gr_ZBi_N0->SetPoint( iPoint, i, ZBi_N0  );
    gr_ZBi_N1->SetPoint( iPoint, i, ZBi_N1  );
    gr_ZBi_N2->SetPoint( iPoint, i, ZBi_N2  );
    gr_ZBi_N3->SetPoint( iPoint, i, ZBi_N3  );
    gr_ZBi_N4->SetPoint( iPoint, i, ZBi_N4  );
    gr_ZBi_N5->SetPoint( iPoint, i, ZBi_N5  );

    gr_thq->SetPoint( iPoint, i, thq  );
    gr_tth->SetPoint( iPoint, i, tth  );

    iPoint++;

  }


  gr_ZBi_N0->SetMarkerStyle( 20 );
  gr_ZBi_N1->SetMarkerStyle( 20 );
  gr_ZBi_N2->SetMarkerStyle( 20 );
  gr_ZBi_N3->SetMarkerStyle( 20 );
  gr_ZBi_N4->SetMarkerStyle( 20 );
  gr_ZBi_N5->SetMarkerStyle( 20 );
  gr_thq->SetMarkerStyle( 20 );
  gr_tth->SetMarkerStyle( 20 );

  gr_ZBi_N0->SetLineWidth( 2.);
  gr_ZBi_N1->SetLineWidth( 2.);
  gr_ZBi_N2->SetLineWidth( 2.);
  gr_ZBi_N3->SetLineWidth( 2.);
  gr_ZBi_N4->SetLineWidth( 2.);
  gr_ZBi_N5->SetLineWidth( 2.);

  gr_ZBi_N0->SetLineColor( kRed+4 );
  gr_ZBi_N1->SetLineColor( kRed+2 );
  gr_ZBi_N2->SetLineColor( kRed+1 );
  gr_ZBi_N3->SetLineColor( kRed );
  gr_ZBi_N4->SetLineColor( kRed-4 );
  gr_ZBi_N5->SetLineColor( kRed-7 );

  gr_ZBi_N0->SetMarkerColor( kRed+4 );
  gr_ZBi_N1->SetMarkerColor( kRed+2 );
  gr_ZBi_N2->SetMarkerColor( kRed+1 );
  gr_ZBi_N3->SetMarkerColor( kRed );
  gr_ZBi_N4->SetMarkerColor( kRed-4 );
  gr_ZBi_N5->SetMarkerColor( kRed-7 );
  gr_thq->SetMarkerColor( 29 );
  gr_tth->SetMarkerColor( kGray+1 );

  gr_ZBi_N0->SetMarkerSize( 1.5 );
  gr_ZBi_N1->SetMarkerSize( 1.5 );
  gr_ZBi_N2->SetMarkerSize( 1.5 );
  gr_ZBi_N3->SetMarkerSize( 1.5 );
  gr_ZBi_N4->SetMarkerSize( 1.5 );
  gr_ZBi_N5->SetMarkerSize( 1.5 );
  gr_thq->SetMarkerSize( 1.5 );
  gr_tth->SetMarkerSize( 1.5 );



  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  float yMax = 1.4;
  //float yMax = 1.3*Qexpmax;
  ////float yMax = 1.3*maxSignif;
  //if( yMax<2. ) yMax = 2.;

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 1.001, 10, 0., yMax );
  h2_axes->SetXTitle( "thqLD Threshold" );
  h2_axes->SetYTitle( "Arbitrary Units" );
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.64, 0.58, 0.92, 0.92 );
  //TLegend* legend = new TLegend( 0.65, 0.69, 0.92, 0.92 );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);
  //legend->AddEntry( gr_signifexp, "s/#sqrt{b} (exp)", "P");
  //legend->AddEntry( gr_signif, "s/#sqrt{b} (flat)", "P");
  //legend->AddEntry( gr_signifexp2, "s/#sqrt{s+b} (exp)", "P");
  //legend->AddEntry( gr_signif2, "s/#sqrt{s+b} (flat)", "P");

  //legend->AddEntry( gr_Qexp, "Q (exp)", "P");
  //legend->AddEntry( gr_Q, "Q", "P");
  legend->AddEntry( gr_thq, "tHq Yield", "P");
  legend->AddEntry( gr_tth, "ttH Yield", "P");
  legend->AddEntry( gr_ZBi_N0, "ZBi (Nsb = 0)", "L");
  legend->AddEntry( gr_ZBi_N1, "ZBi (Nsb = 1)", "L");
  legend->AddEntry( gr_ZBi_N2, "ZBi (Nsb = 2)", "L");
  legend->AddEntry( gr_ZBi_N3, "ZBi (Nsb = 3)", "L");
  legend->AddEntry( gr_ZBi_N4, "ZBi (Nsb = 4)", "L");
  legend->AddEntry( gr_ZBi_N5, "ZBi (Nsb = 5)", "L");
  legend->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");


  //gr_signif->Draw("P same");
  //gr_signif2->Draw("P same");
  //gr_signifexp->Draw("P same");
  //gr_signifexp2->Draw("P same");

  //gr_Q->Draw("P same");
  //gr_Qexp->Draw("P same");
  gr_thq->Draw("P same");
  gr_tth->Draw("P same");
  gr_ZBi_N0->Draw("L same");
  gr_ZBi_N1->Draw("L same");
  gr_ZBi_N2->Draw("L same");
  gr_ZBi_N3->Draw("L same");
  gr_ZBi_N4->Draw("L same");
  gr_ZBi_N5->Draw("L same");

  TLine* choiceLine = new TLine( 0.25, 0., 0.25, yMax );
  choiceLine->SetLineStyle(2);
  choiceLine->SetLineWidth(2);
  choiceLine->Draw("same");



  gPad->RedrawAxis();

  c1->SaveAs("dummySignificance_vsLD_ZBi.eps");
  c1->SaveAs("dummySignificance_vsLD_ZBi.png");
  

  return 0;

}




float getAlpha(TF1* f1, float deltaM) {

  float s1 = f1->Integral( 100., 125.-deltaM );
  float s2 = f1->Integral( 125.+deltaM, 180.);

  float sR = f1->Integral( 125.-deltaM, 125.+deltaM);

  float alpha = sR/(s1+s2);

  return alpha;

}

