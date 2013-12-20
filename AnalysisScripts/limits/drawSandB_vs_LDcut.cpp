#include <iostream>
#include <fstream>


#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"




int main( int argc, char* argv[] ) {


  std::string batchProd = "preNatal_v1";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }



  TGraph* gr_s = new TGraph(0);
  TGraph* gr_b = new TGraph(0);
  TGraph* gr_tth = new TGraph(0);

  int iPoint=0;

  for( float iCut = 0.; iCut < 1.; iCut += 0.05 ) {

    char datacardName[500];
    sprintf(datacardName, "datacards_optLD_%s/datacard_thq_leptonic_LD%.0f.txt", batchProd.c_str(), 100.*iCut);

    
    ifstream ifs(datacardName);
    
    float thq, tth, b;

    while( ifs.good() && !ifs.eof() ) {

      std::string line;
      std::getline(ifs, line);
      TString line_tstr(line);
      if( !(line_tstr.BeginsWith( "rate " )) ) continue;

      sscanf( line.c_str(), "rate %f %f%f", &thq, &b, &tth );
      break;

    }

    ifs.close();

    gr_s->SetPoint( iPoint, iCut, thq );
    gr_b->SetPoint( iPoint, iCut, b );
    gr_tth->SetPoint( iPoint, iCut, tth );

    iPoint++;

  }


  DrawBase* db = new DrawBase("ll");

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 1.0001, 10, 0., 1.);
  h2_axes->SetXTitle("LD Cut");
  h2_axes->SetYTitle("Yield");
  h2_axes->Draw();

  gr_s->SetMarkerStyle(20);
  gr_s->SetMarkerSize(1.6);
  gr_s->SetMarkerColor(46);

  gr_b->SetMarkerStyle(21);
  gr_b->SetMarkerSize(1.6);
  gr_b->SetMarkerColor(kGray+1);

  gr_tth->SetMarkerStyle(22);
  gr_tth->SetMarkerSize(1.6);
  gr_tth->SetMarkerColor(29);


  TLegend* legend = new TLegend( 0.6, 0.75, 0.9, 0.9 );
  legend->SetTextSize(0.038);
  legend->SetFillColor(0);
  legend->AddEntry( gr_s, "tHq", "P" );
  legend->AddEntry( gr_tth, "ttH", "P" );
  legend->AddEntry( gr_b, "Continuous BG", "P" );
  legend->Draw("same");

  gr_s->Draw("p same");
  gr_b->Draw("p same");
  gr_tth->Draw("p same");


  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();

  c1->SaveAs(Form("datacards_optLD_%s/SandB_vs_LDcut.eps", batchProd.c_str()));

  delete c1;
  delete h2_axes;
  delete legend;
  delete gr_s;
  delete gr_b;
  delete gr_tth;

  return 0;

}
