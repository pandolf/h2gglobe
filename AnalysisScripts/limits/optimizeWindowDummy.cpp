#include <iostream>
#include <cmath>

#include "TCanvas.h"
#include "TH2D.h"
#include "TLegend.h"
#include "TF1.h"

#include "CommonTools/DrawBase.h"




int main() {


  DrawBase* db = new DrawBase("kk");


  float m = 125.;
  float sigma = 1.5;
  float N = 0.884549/sqrt(3.14159*2.*sigma*sigma);

  TF1* sig = new TF1("sig", "gaus", 100., 180.);
  sig->SetParameters( N, m, sigma);

  float bg_integral = 1.5; // in 60 gev window


  TGraph* gr_signif = new TGraph(0);
  TGraph* gr_signif2 = new TGraph(0);

  int iPoint=0;

  for( float iGev = 0.5; iGev<8.; iGev+=0.25 ) {
  
    float s = sig->Integral( m-iGev, m+iGev );
    float b = bg_integral*2.*iGev/60.;
    std::cout << "iGev: " << iGev << std::endl;
    std::cout << "s: " << s << " b: " << b << std::endl;

  
    float signif = s/sqrt(b);
    float signif2 = s/sqrt(s+b);

    gr_signif ->SetPoint( iPoint, iGev, signif  );
    gr_signif2->SetPoint( iPoint, iGev, signif2 );

    iPoint++;

  }


  gr_signif->SetMarkerStyle( 20 );
  gr_signif2->SetMarkerStyle( 24 );

  gr_signif->SetMarkerSize( 2. );
  gr_signif2->SetMarkerSize( 2. );

  gr_signif->SetMarkerColor( kGray+1 );
  gr_signif2->SetMarkerColor( kGray+1 );


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 8., 10, 0., 3.);
  h2_axes->SetXTitle( "Signal Counting Window (125#pm#DeltaM) [GeV]");
  h2_axes->SetYTitle( "Significance" );
  h2_axes->Draw();

  TLegend* legend = new TLegend( 0.6, 0.65, 0.9, 0.9 );
  legend->SetTextSize(0.04);
  legend->SetFillColor(0);
  legend->AddEntry( gr_signif, "s/#sqrt{b}", "P");
  legend->AddEntry( gr_signif2, "s/#sqrt{s+b}", "P");
  legend->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gr_signif->Draw("P same");
  gr_signif2->Draw("P same");

  TPaveText* sig_text = new TPaveText( 0.45, 0.25, 0.9, 0.3, "brNDC" );
  TPaveText*  bg_text = new TPaveText( 0.45, 0.2, 0.9, 0.25, "brNDC" );

  sig_text->SetTextAlign(12);
   bg_text->SetTextAlign(12);

  sig_text->SetTextSize(0.035);
   bg_text->SetTextSize(0.035);

  sig_text->SetFillColor(0);
   bg_text->SetFillColor(0);

  char sig_text_char[200];
  sprintf(sig_text_char, "Gaussian signal (#sigma=%.1f GeV)", sigma);
  char bg_text_char[200];
  sprintf(bg_text_char, "Flat BG (%.2f ev/GeV)", bg_integral/60. );

  sig_text->AddText(sig_text_char);
   bg_text->AddText( bg_text_char);

  sig_text->Draw("same");
   bg_text->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs("dummySignificance.eps");
  c1->SaveAs("dummySignificance.png");
  

  return 0;

}
