#include "CommonTools/DrawBase.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TCanvas.h"
#include <iostream>
#include <fstream>



struct Limits {

  float limit;
  float limit_m1s;
  float limit_p1s;
  float limit_m2s;
  float limit_p2s;

};



Limits getLimits( const std::string& suffix, bool hybrid );
float getSingleLimit( const std::string& filename );


int main( int argc, char* argv[]) {


  bool hybrid=false;
  if( argc>1 ) {
    std::string hybrid_str(argv[1]);
    TString hybrid_tstr(hybrid_str);
    hybrid = hybrid_tstr.Contains("hybr");
  }

  DrawBase* db = new DrawBase("drawLimit2");

  

  Limits l_0001 = getLimits("0001", hybrid);
  Limits l_001 = getLimits("001", hybrid);
  Limits l_01 = getLimits("01", hybrid);
  Limits l_1 = getLimits("1", hybrid);
  Limits l_2 = getLimits("2", hybrid);
  Limits l_10 = getLimits("10", hybrid);

std::cout << l_001.limit << std::endl;
std::cout << l_001.limit_m1s << std::endl;
std::cout << l_001.limit_p1s << std::endl;
std::cout << l_001.limit_m2s << std::endl;
std::cout << l_001.limit_p2s << std::endl;

  TGraphAsymmErrors* gr_central = new TGraphAsymmErrors(0);
  TGraphAsymmErrors* gr_onesigma = new TGraphAsymmErrors(0);
  TGraphAsymmErrors* gr_twosigma = new TGraphAsymmErrors(0);

  gr_central->SetPoint( gr_central->GetN(), 0.001, l_0001.limit );
  gr_central->SetPoint( gr_central->GetN(), 0.01, l_001.limit );
  gr_central->SetPoint( gr_central->GetN(), 0.1, l_01.limit );
  gr_central->SetPoint( gr_central->GetN(), 1, l_1.limit );
  gr_central->SetPoint( gr_central->GetN(), 10, l_10.limit );

  gr_onesigma->SetPoint( gr_onesigma->GetN(), 0.001, l_0001.limit );
  gr_onesigma->SetPoint( gr_onesigma->GetN(), 0.01, l_001.limit );
  gr_onesigma->SetPoint( gr_onesigma->GetN(), 0.1, l_01.limit );
  gr_onesigma->SetPoint( gr_onesigma->GetN(), 1, l_1.limit );
  gr_onesigma->SetPoint( gr_onesigma->GetN(), 10, l_10.limit );

  gr_twosigma->SetPoint( gr_twosigma->GetN(), 0.001, l_0001.limit );
  gr_twosigma->SetPoint( gr_twosigma->GetN(), 0.01, l_001.limit );
  gr_twosigma->SetPoint( gr_twosigma->GetN(), 0.1, l_01.limit );
  gr_twosigma->SetPoint( gr_twosigma->GetN(), 1, l_1.limit );
  gr_twosigma->SetPoint( gr_twosigma->GetN(), 10, l_10.limit );

  gr_onesigma->SetPointError( 0, 0., 0., -l_0001.limit_m1s+l_0001.limit, l_0001.limit_p1s-l_0001.limit );
  gr_onesigma->SetPointError( 1, 0., 0., -l_001.limit_m1s+l_001.limit, l_001.limit_p1s-l_001.limit );
  gr_onesigma->SetPointError( 2, 0., 0., -l_01.limit_m1s+l_01.limit, l_01.limit_p1s-l_01.limit );
  gr_onesigma->SetPointError( 3, 0., 0., -l_1.limit_m1s+l_1.limit, l_1.limit_p1s-l_1.limit );
  gr_onesigma->SetPointError( 4, 0., 0., -l_10.limit_m1s+l_10.limit, l_10.limit_p1s-l_10.limit );

  gr_twosigma->SetPointError( 0, 0., 0., -l_0001.limit_m2s+l_0001.limit, l_0001.limit_p2s-l_0001.limit );
  gr_twosigma->SetPointError( 1, 0., 0., -l_001.limit_m2s+l_001.limit,  l_001.limit_p2s-l_001.limit );
  gr_twosigma->SetPointError( 2, 0., 0., -l_01.limit_m2s+l_01.limit,   l_01.limit_p2s-l_01.limit );
  gr_twosigma->SetPointError( 3, 0., 0., -l_1.limit_m2s+l_1.limit,    l_1.limit_p2s-l_1.limit );
  gr_twosigma->SetPointError( 4, 0., 0., -l_10.limit_m2s+l_10.limit,   l_10.limit_p2s-l_10.limit );

  gr_central->SetMarkerSize(1.4);
  gr_central->SetMarkerStyle(20);
  gr_central->SetLineStyle(2);
  gr_central->SetLineColor(kBlack);
  gr_central->SetLineWidth(3);

  gr_onesigma->SetFillColor(kGreen);
  gr_onesigma->SetFillStyle(1001);

  gr_twosigma->SetFillColor(kYellow);
  gr_twosigma->SetFillStyle(1001);


  TH2D* h2_axes = new TH2D("axes", "", 10, 0.0001, 100., 10, 0., 15.);
  h2_axes->SetXTitle("#alpha");
  h2_axes->SetYTitle("95% UL");

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();
  c1->SetLogx();
  c1->SetGrid(true);

  h2_axes->Draw();
  gr_twosigma->Draw("L3 same");
  gr_onesigma->Draw("L3 same");
  gr_central->Draw("L same");

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  if( hybrid )
    c1->SaveAs("checkUL2_hybrid.eps");
  else
    c1->SaveAs("checkUL2.eps");

  return 0;

}



Limits getLimits( const std::string& suffix, bool hybrid ) {

  std::string hybrid_str = (hybrid) ? "Hybrid" : "";

  std::string filename = "log" + hybrid_str + "_" + suffix + ".log";
  std::string filename_m1s = "log" + hybrid_str + "_" + suffix + "_m1s.log";
  std::string filename_p1s = "log" + hybrid_str + "_" + suffix + "_p1s.log";
  std::string filename_m2s = "log" + hybrid_str + "_" + suffix + "_m2s.log";
  std::string filename_p2s = "log" + hybrid_str + "_" + suffix + "_p2s.log";

  Limits l;
  l.limit = getSingleLimit(filename);
  l.limit_m1s = getSingleLimit(filename_m1s);
  l.limit_p1s = getSingleLimit(filename_p1s);
  l.limit_m2s = getSingleLimit(filename_m2s);
  l.limit_p2s = getSingleLimit(filename_p2s);

  return l;

}



float getSingleLimit( const std::string& filename ) {

  ifstream ifs(filename.c_str());

  float limit = -1.;
  bool nextOne = false;

  while( ifs.good() && !ifs.eof() && limit<0. ) {

    std::string thisline;
    getline( ifs, thisline );
    TString thisline_tstr(thisline);
      
    if( thisline_tstr.BeginsWith(" -- Hybrid New -- ") ) {
      nextOne = true;
    } else {
      if( !nextOne ) continue;
    }

    float limit_err;
    sscanf( thisline.c_str(), "Limit: r < %f +/- %f @ 95%% CL", &limit, &limit_err );

  }

  ifs.close();

  return limit;

}
