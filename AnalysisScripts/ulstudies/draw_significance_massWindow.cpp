#include <fstream>
#include <iostream>
#include <cstdlib>

#include "DrawBase.h"

#include "TString.h"
#include "TH2D.h"
#include "TGraphAsymmErrors.h"
#include "TLegend.h"
#include "TCanvas.h"





int main() {

  DrawBase* db = new DrawBase("signif");

  float massWindow = 0.5;

  TGraphAsymmErrors* graph = new TGraphAsymmErrors(0);
  int iPoint =0;


  for( ; massWindow<8; massWindow+=0.5 ) {

    char logFileName[300];
    if( massWindow<1. )
      sprintf(logFileName, "logHybrid_Signif_realistic_0%.0fGeV.log", massWindow*10.);
    else
      sprintf(logFileName, "logHybrid_Signif_realistic_%.0fGeV.log", massWindow*10.);

    std::ifstream logFile(logFileName);

    float significance;
    float serr_plus, serr_minus;

    bool found_significance=false;

    while( logFile.good() && !logFile.eof() ) {

      std::string thisline;
      getline( logFile, thisline );
      TString thisline_tstr(thisline);
        
      if( thisline_tstr.BeginsWith("Significance: ") ) {
        sscanf( thisline.c_str(), "Significance: %f  %f/%f", &significance, &serr_minus, &serr_plus);
        found_significance = true;
      } 

    } //while log file

    if( !found_significance ) {
      std::cout << "Didn't find significance in file: " << logFileName << std::endl;
      exit(33);
    }

    std::cout << "massWindow: " << massWindow << "\tsignificance: " << significance << " +" << serr_plus << "/" << serr_minus << std::endl;

    graph->SetPoint(iPoint, massWindow, significance);
    graph->SetPointError( iPoint, 0., 0., -serr_minus, serr_plus);
    iPoint++;
   
  } //for mass windows


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 8., 10, 0., 3.);
  h2_axes->SetXTitle("Signal Counting Window (125#pm#DeltaM) [GeV]");
  h2_axes->SetYTitle("Significance (#sigma)");

  h2_axes->Draw();

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(2);
  graph->SetMarkerColor(39);

  graph->Draw("P same");

  std::string canvasName = "significance_massWindow.eps";
  c1->SaveAs(canvasName.c_str());
  std::string pdfcommand = "epstopdf " + canvasName;
  system(pdfcommand.c_str());


  return 0;

}
