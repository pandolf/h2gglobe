#include <iostream>
#include <cmath>

#include "TCanvas.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TF1.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/StatTools.h"



float getFWHM( const std::string& fileName );
float getAlpha(TF1* f1, float deltaM);

int main() {


  DrawBase* db = new DrawBase("kk");


  TFile* file = TFile::Open("../batchOutput_postFreezeNLO_v2/histograms_CMS-HGG.root");
  TTree* tree_sig = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_tth = (TTree*)file->Get("tth_m125_8TeV");

  TH1D* h1_mh_sig = new TH1D("mh_sig", "", 1600, 100., 180.);
  h1_mh_sig->Sumw2();
  TH1D* h1_mh_tth = new TH1D("mh_tth", "", 1600, 100., 180.);
  h1_mh_tth->Sumw2();

  tree_sig->Project( "mh_sig", "PhotonsMass", "evweight*(category==11 && thqLD_lept>0.25)");
  tree_tth->Project( "mh_tth", "PhotonsMass", "evweight*(category==11 && thqLD_lept>0.25)");

  std::cout << "Signal integral: " << h1_mh_sig->Integral(100., 180.) << std::endl;
  std::cout << "BG integral: " << h1_mh_tth->Integral(100., 180.) << std::endl;

  float m = 125.;
  ////float sigma = 1.5;
  //float fwhm = getFWHM("../batchOutput_preNatal_v5/histograms_CMS-HGG.root");
  //float sigma = fwhm/2.35;
  //std::cout << "Using: FWHM: " << fwhm << " sigma: " << sigma << std::endl;
  //float N = 0.884549/sqrt(3.14159*2.*sigma*sigma);
  //TF1* sig = new TF1("sig", "gaus", 100., 180.);
  //sig->SetParameters( N, m, sigma);

  float bg_integral = 1.5; // in 60 gev window

  //TF1* bg_exp = new TF1( "bg_exp", "1.5*exp([0]+[1]*x)", 100., 180.);
  //bg_exp->SetParameter(0, -3.71371e-01);
  //bg_exp->SetParameter(1, -3.03197e-02);
  //std::cout << "exp integral: " << bg_exp->Integral( 100., 180.) << " (should be " << bg_integral << ")" << std::endl;;

  TF1* bg_exp = new TF1( "bg_exp", "exp([0]*x)", 100., 180.);
  bg_exp->SetParameter(0, -0.005);


  TGraph* gr_signif = new TGraph(0);
  TGraph* gr_signif2 = new TGraph(0);

  TGraph* gr_signifexp = new TGraph(0);
  TGraph* gr_signifexp2 = new TGraph(0);

  TGraph* gr_Q = new TGraph(0);
  TGraph* gr_Qexp = new TGraph(0);

  TGraph* gr_ZBi = new TGraph(0);
  TGraph* gr_ZBiexp = new TGraph(0);

  int iPoint=0;
  float maxSignif=0.;
  float Qexpmax=0.;

  for( float iGev = 0.5; iGev<8.; iGev+=0.25 ) {
  
    int binMin =  h1_mh_sig->FindBin(m-iGev);
    int binMax =  h1_mh_sig->FindBin(m+iGev);
    float s = h1_mh_sig->Integral( binMin, binMax );
    float b_tth = h1_mh_tth->Integral( binMin, binMax );
    s += 1.4* b_tth; // exra signal due to Ct=-1

    //float b = bg_integral*2.*iGev/(80.-2.*iGev);
    //float bexp = bg_exp->Integral( m-iGev, m+iGev );

    float b = 0.;
    float bexp = 0.;

    float alpha_flat = 2.*iGev/(80.-2.*iGev);
    float alpha_exp = getAlpha(bg_exp, iGev);
  
    float signif = s/sqrt(b);
    float signif2 = s/sqrt(s+b);

    float signifexp = s/sqrt(bexp);
    float signifexp2 = s/sqrt(s+bexp);

    float ZBi = StatTools::computeZBiProper( s, b, 1./alpha_flat );
    float ZBiexp = StatTools::computeZBiProper( s, bexp, 1./alpha_exp );

    float Q = 2.*( sqrt(s+b+b_tth) - sqrt(b+b_tth) );
    float Qexp  = 2.*( sqrt(s+bexp+b_tth) - sqrt(bexp+b_tth) );
    //float Q = 2.*( sqrt(s+b) - sqrt(b) );
    //float Qexp  = 2.*( sqrt(s+bexp) - sqrt(bexp) );

    gr_signif ->SetPoint( iPoint, iGev, signif  );
    gr_signif2->SetPoint( iPoint, iGev, signif2 );

    gr_signifexp ->SetPoint( iPoint, iGev, signifexp  );
    gr_signifexp2->SetPoint( iPoint, iGev, signifexp2 );

    gr_ZBi ->SetPoint( iPoint, iGev, ZBi  );
    gr_ZBiexp ->SetPoint( iPoint, iGev, ZBiexp  );

    gr_Q ->SetPoint( iPoint, iGev, Q  );
    gr_Qexp ->SetPoint( iPoint, iGev, Qexp  );

    iPoint++;
    if( signif>maxSignif )
      maxSignif=signif;
    if( signifexp>maxSignif )
      maxSignif=signifexp;
    if( Qexp>Qexpmax )
      Qexpmax=Qexp;

  }


  gr_signif->SetMarkerStyle( 24 );
  gr_signif2->SetMarkerStyle( 24 );
  gr_signifexp->SetMarkerStyle( 20 );
  gr_signifexp2->SetMarkerStyle( 20 );

  gr_signif->SetMarkerSize( 2. );
  gr_signif2->SetMarkerSize( 2. );
  gr_signifexp->SetMarkerSize( 2. );
  gr_signifexp2->SetMarkerSize( 2. );

  gr_signif->SetMarkerColor( kGray+1 );
  gr_signif2->SetMarkerColor( 46 );
  gr_signifexp->SetMarkerColor( kGray+1 );
  gr_signifexp2->SetMarkerColor( 46 );

  gr_Q->SetMarkerStyle( 24 );
  gr_ZBi->SetMarkerStyle( 24 );
  gr_Qexp->SetMarkerStyle( 20 );
  gr_ZBiexp->SetMarkerStyle( 20 );

  gr_Q->SetMarkerSize( 2. );
  gr_ZBi->SetMarkerSize( 2. );
  gr_Qexp->SetMarkerSize( 2. );
  gr_ZBiexp->SetMarkerSize( 2. );

  gr_Q->SetMarkerColor( kGray+1 );
  gr_ZBi->SetMarkerColor( 46 );
  gr_Qexp->SetMarkerColor( kGray+1 );
  gr_ZBiexp->SetMarkerColor( 46 );


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  float yMax = 1.6;
  //float yMax = 1.3*Qexpmax;
  ////float yMax = 1.3*maxSignif;
  //if( yMax<2. ) yMax = 2.;

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 8., 10, 0., yMax );
  h2_axes->SetXTitle( "Signal Counting Window (125#pm#DeltaM) [GeV]");
  h2_axes->SetYTitle( "Significance" );
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.65, 0.75, 0.92, 0.92 );
  //TLegend* legend = new TLegend( 0.65, 0.69, 0.92, 0.92 );
  legend->SetTextSize(0.035);
  legend->SetFillColor(0);
  //legend->AddEntry( gr_signifexp, "s/#sqrt{b} (exp)", "P");
  //legend->AddEntry( gr_signif, "s/#sqrt{b} (flat)", "P");
  //legend->AddEntry( gr_signifexp2, "s/#sqrt{s+b} (exp)", "P");
  //legend->AddEntry( gr_signif2, "s/#sqrt{s+b} (flat)", "P");

  //legend->AddEntry( gr_Qexp, "Q (exp)", "P");
  //legend->AddEntry( gr_Q, "Q", "P");
  legend->AddEntry( gr_ZBiexp, "ZBi (exp)", "P");
  legend->AddEntry( gr_ZBi, "ZBi (flat)", "P");
  legend->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  TPaveText* sig_text    = new TPaveText( 0.16, 0.87, 0.6, 0.92, "brNDC" );
  TPaveText*  bgexp_text = new TPaveText( 0.45, 0.23, 0.9, 0.29, "brNDC" );
  TPaveText*  bg_text    = new TPaveText( 0.45, 0.19, 0.9, 0.23, "brNDC" );

    sig_text->SetTextAlign(12);
     bg_text->SetTextAlign(12);
  bgexp_text->SetTextAlign(12);
  
    sig_text->SetTextSize(0.032);
     bg_text->SetTextSize(0.032);
  bgexp_text->SetTextSize(0.032);
  
    sig_text->SetFillColor(0);
     bg_text->SetFillColor(0);
  bgexp_text->SetFillColor(0);
  
  bg_text->SetTextColor(kGray+1);

  //char sig_text_char[200];
  //sprintf(sig_text_char, "Gaussian signal (#sigma=%.1f GeV)", sigma);
  char bg_text_char[200];
  sprintf(bg_text_char, "Flat BG (%.2f ev/GeV)", bg_integral/60. );
  char bgexp_text_char[200];
  sprintf(bgexp_text_char, "Exp BG (p_{0}=%.3f/GeV)", bg_exp->GetParameter(0) );

  //sig_text->AddText(sig_text_char);
   bg_text->AddText( bg_text_char);
bgexp_text->AddText( bgexp_text_char);

  //sig_text->Draw("same");
   bg_text->Draw("same");
bgexp_text->Draw("same");

  TLine* choiceLine = new TLine( 3., 0., 3., yMax );
  choiceLine->SetLineStyle(2);
  choiceLine->SetLineWidth(2);
  choiceLine->Draw("same");


  //gr_signif->Draw("P same");
  //gr_signif2->Draw("P same");
  //gr_signifexp->Draw("P same");
  //gr_signifexp2->Draw("P same");

  //gr_Q->Draw("P same");
  //gr_Qexp->Draw("P same");
  gr_ZBi->Draw("P same");
  gr_ZBiexp->Draw("P same");


  gPad->RedrawAxis();

  c1->SaveAs("dummySignificance_ZBi.eps");
  c1->SaveAs("dummySignificance_ZBi.png");
  

  return 0;

}



float getFWHM( const std::string& fileName ) {

  TFile* file = TFile::Open(fileName.c_str());
  TTree* tree = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  
  int nbins = 320;
  TH1D* h1_mgg = new TH1D("mgg", "", nbins, 100., 180.);
  
  tree->Project("mgg", "PhotonsMass", "category==11");

  int maxBin = h1_mgg->GetMaximumBin();
  float max = h1_mgg->GetBinContent(maxBin);

  int binMin=0;
  for( unsigned i=0; maxBin-i>0; i++ ) {

    float thisBinContent = h1_mgg->GetBinContent(maxBin-i);

    if( thisBinContent<max/2. ) {
      binMin = maxBin-i;
      break;
    }

  }
  

  int binMax=0;
  for( unsigned i=0; maxBin+i<nbins; i++ ) {

    float thisBinContent = h1_mgg->GetBinContent(maxBin+i);

    if( thisBinContent<max/2. ) {
      binMax = maxBin+i;
      break;
    }

  }
  
  float xMin = h1_mgg->GetBinCenter(binMin);
  float xMax = h1_mgg->GetBinCenter(binMax);

  std::cout << "Interval: " << xMin << "-" << xMax << " GeV" << std::endl;

  return (xMax-xMin);

}


float getAlpha(TF1* f1, float deltaM) {

  float s1 = f1->Integral( 100., 125.-deltaM );
  float s2 = f1->Integral( 125.+deltaM, 180.);

  float sR = f1->Integral( 125.-deltaM, 125.+deltaM);

  float alpha = sR/(s1+s2);

  return alpha;

}

