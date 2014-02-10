#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"




struct  ULSignif_struct {
  float signif;
  float s_err_plus;
  float s_err_minus;
  float ul;
  float ul_err;
};




ULSignif_struct createSingleDatacard( const std::string& batchProd, const std::string& channel, const std::string& additionalSelection, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_ggh, TTree* tree_vbf, TTree* tree_wzh, TTree* tree_tth, const std::string& suffix="" );


int main( int argc, char* argv[] ) {


  std::string batchProd = "qJetEtaFix_v3";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }


  DrawBase* db = new DrawBase("optLD");

  std::string fileName = "../batchOutput_" + batchProd + "/nominal/histograms_CMS-HGG.root";
  TFile* file   = TFile::Open(fileName.c_str());
  std::cout << "-> Opening file: " << fileName << std::endl;

  TTree* tree_thqLept = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_thqHadr = (TTree*)file->Get("thqHadronic_m125_8TeV");
  TTree* tree_tth     = (TTree*)file->Get("tth_m125_8TeV");
  TTree* tree_wzh     = (TTree*)file->Get("wzh_m125_8TeV");
  TTree* tree_ggh     = (TTree*)file->Get("ggh_m125_8TeV");
  TTree* tree_vbf     = (TTree*)file->Get("vbf_m125_8TeV");


  std::string dirname = "datacards_optLD_" + batchProd;
  std::string mkdirCommand = "mkdir -p " + dirname;
  system( mkdirCommand.c_str() );




  TGraphAsymmErrors* gr_signif = new TGraphAsymmErrors(0);
  TGraphAsymmErrors* gr_ul     = new TGraphAsymmErrors(0);

  int iPoint=0;

  for( float iCut = 0.; iCut < 1.; iCut += 0.05 ) {
    char ldcut[100];
    sprintf( ldcut, "thqLD_lept>%f", iCut );
    std::string ldcutString(ldcut);
    char ldcutName[100];
    sprintf( ldcutName, "LD%.0f", iCut*100 );
    std::string ldcutNameString(ldcutName);
    ULSignif_struct thisStruct = createSingleDatacard( batchProd, "leptonic", ldcutString, tree_thqLept, tree_thqHadr, tree_ggh, tree_vbf, tree_wzh, tree_tth, ldcutNameString );

    gr_signif->SetPoint( iPoint, iCut, thisStruct.signif );
    gr_signif->SetPointEYhigh( iPoint, thisStruct.s_err_plus );
    gr_signif->SetPointEYlow( iPoint, -thisStruct.s_err_minus );

    gr_ul ->SetPoint( iPoint, iCut, thisStruct.ul );
    gr_ul->SetPointEYhigh( iPoint, thisStruct.ul_err );
    gr_ul->SetPointEYlow( iPoint, thisStruct.ul_err );

    iPoint++;

  }


  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D("axes", "", 10, 0., 1.00001, 10, 0., 10.);
  h2_axes->SetXTitle("Cut on LD");
  h2_axes->Draw("");

  gr_signif->SetMarkerSize( 1.6 );
  gr_signif->SetMarkerStyle( 20 );
  gr_signif->SetMarkerColor( 46 );

  gr_ul    ->SetMarkerSize( 1.6 );
  gr_ul    ->SetMarkerStyle( 24 );
  gr_ul    ->SetMarkerColor( kRed+3 );


  TLegend* legend = new TLegend( 0.2, 0.5, 0.5, 0.9 );
  legend->SetTextSize( 0.038 );
  legend->SetFillColor(0);
  legend->AddEntry( gr_signif, "Expected significance", "P" );
  legend->AddEntry( gr_ul, "Expected 95% Upper Limit", "P" );
  legend->Draw("same");

  gr_signif->Draw("p same");
  gr_ul->Draw("p same");

  gPad->RedrawAxis();

  std::string canvasName = dirname + "/signif_ul_vs_LDcut";
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";
  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  return 0;

}



ULSignif_struct createSingleDatacard( const std::string& batchProd, const std::string& channel, const std::string& additionalSelection, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_ggh, TTree* tree_vbf, TTree* tree_wzh, TTree* tree_tth, const std::string& suffix ) {


  std::cout << "-> Creating datacard for channel: " << channel;
  if( suffix!="" ) std::cout << " (" << suffix << ")";
  std::cout << std::endl;

  int category=-1;
  if( channel=="leptonic" ) {
    category=11;
  } else if( channel=="hadronic" ) {
    category=12;
  } else {
    std::cout << "-> Only 'leptonic' and 'hadronic' are supported channels. Exiting." << std::endl;
    exit(11111);
  }





  char fullSelection[500];
  char fullSelection_sidebands[500];
  if( additionalSelection!="" ) {
    sprintf( fullSelection,           "evweight*( category==%d && %s )", category, additionalSelection.c_str() );
    sprintf( fullSelection_sidebands, "evweight*( category==%d && %s && (PhotonsMass<115. || PhotonsMass>135.) )", category, additionalSelection.c_str() );
  } else {
    sprintf( fullSelection,           "evweight*( category==%d       )", category );
    sprintf( fullSelection_sidebands, "evweight*( category==%d       && (PhotonsMass<115. || PhotonsMass>135.) )", category );
  }


  TString category_tstr(channel);
  bool isLeptonic = category_tstr.Contains("leptonic");



  int nBins = 80;
  float xMin = 100.;
  float xMax = 180.;

  TH1D* h1_mgg_thqLeptonic = new TH1D("mgg_thqLeptonic", "", nBins, xMin, xMax);
  TH1D* h1_mgg_thqHadronic = new TH1D("mgg_thqHadronic", "", nBins, xMin, xMax);
  TH1D* h1_mgg_ggh         = new TH1D("mgg_ggh", "", nBins, xMin, xMax);
  TH1D* h1_mgg_vbf         = new TH1D("mgg_vbf", "", nBins, xMin, xMax);
  TH1D* h1_mgg_wzh         = new TH1D("mgg_wzh", "", nBins, xMin, xMax);
  TH1D* h1_mgg_tth         = new TH1D("mgg_tth", "", nBins, xMin, xMax);

  h1_mgg_thqLeptonic->Sumw2();
  h1_mgg_thqHadronic->Sumw2();
  h1_mgg_ggh->Sumw2();
  h1_mgg_vbf->Sumw2();
  h1_mgg_wzh->Sumw2();
  h1_mgg_tth->Sumw2();


  tree_thqLeptonic->Project( "mgg_thqLeptonic", "PhotonsMass", fullSelection );
  tree_thqHadronic->Project( "mgg_thqHadronic", "PhotonsMass", fullSelection );
  tree_ggh->Project( "mgg_ggh", "PhotonsMass", fullSelection );
  tree_vbf->Project( "mgg_vbf", "PhotonsMass", fullSelection );
  tree_wzh->Project( "mgg_wzh", "PhotonsMass", fullSelection );
  tree_tth->Project( "mgg_tth", "PhotonsMass", fullSelection );



  float sidebands_yield = 0.; // assume zero for now


  TH1D* h1_mgg_thq = new TH1D(*h1_mgg_thqLeptonic);
  h1_mgg_thq->Add(h1_mgg_thqHadronic);
  h1_mgg_thq->SetName("mgg_thq");



  float massWindow = 3.;

  int binMin = h1_mgg_thqLeptonic->FindBin( 125.-massWindow );
  int binMax = h1_mgg_thqLeptonic->FindBin( 125.+massWindow );

  float yield_thq = h1_mgg_thq->Integral(binMin, binMax-1);
  float yield_thqLept = h1_mgg_thqLeptonic->Integral(binMin, binMax-1);
  float yield_thqHadr = h1_mgg_thqHadronic->Integral(binMin, binMax-1);
  float yield_ggh = h1_mgg_ggh->Integral(binMin, binMax-1);
  float yield_vbf = h1_mgg_vbf->Integral(binMin, binMax-1);
  float yield_wzh = h1_mgg_wzh->Integral(binMin, binMax-1);
  float yield_tth = h1_mgg_tth->Integral(binMin, binMax-1);



  //float k_BG = massWindow*2./70.;
  float k_BG = massWindow*2./(80.-massWindow*2.);
  //float k_BG = 0.10; // for expo BG


  char datacardName[300];
  if( suffix!="" )
    sprintf(datacardName, "datacards_optLD_%s/datacard_thq_%s_%s.txt", batchProd.c_str(), channel.c_str(), suffix.c_str() );
  else
    sprintf(datacardName, "datacards_optLD_%s/datacard_thq_%s.txt",    batchProd.c_str(), channel.c_str() );
  std::ofstream datacard(datacardName);
  std::string datacardName_str(datacardName);

 
  datacard << "imax 1" << std::endl;
  datacard << "jmax ";
  if( isLeptonic ) datacard << "3 " << std::endl; // of SMH, only tth and wzh (ignore ggh and vbf)
  else             datacard << "4 " << std::endl; // of SMH, only tth, ggh, and vbf (ignore wzh)
  datacard << "kmax *" << std::endl;
  datacard << "    " << std::endl;
  datacard << "    " << std::endl;
  datacard << "bin  1" << std::endl;
  datacard << "observation 0" << std::endl;
  datacard << std::endl;
  datacard << "    " << std::endl;
  datacard << "bin            1        1       1     1"; 
  if( !isLeptonic ) datacard <<  "    1 ";
  datacard << std::endl;
  datacard << "process        s        b       tth  "; 
  if( isLeptonic ) datacard << " wzh"     << std::endl;
  else             datacard << " ggh   vbf  "     << std::endl;
  datacard << "process        0        1       2     3"; 
  if( !isLeptonic )  datacard << " 4";
  datacard << std::endl;


  datacard << "rate   " << yield_thq <<  "  " << sidebands_yield*k_BG << "  " << yield_tth;
  if( isLeptonic )
    datacard << " " << yield_wzh   << std::endl;
  else
    datacard << " " << yield_ggh << " " << yield_vbf << std::endl;
  datacard << "      " << std::endl;
  datacard << "      " << std::endl;
  datacard << "#syst " << std::endl;
  datacard << "lumi       lnN    1.026 - 1.026 1.026";
  if( !isLeptonic ) datacard <<  " 1.026";
  datacard << std::endl;

  datacard << "bg         gmN " << (int)sidebands_yield << "    - " << k_BG << "   - - - -" << std::endl;

  datacard << "PDF   lnN    -     -         0.922/1.078 ";
  if( !isLeptonic ) datacard <<  " -  - ";
  else              datacard << " 0.99/1.01";
  datacard << std::endl;
  datacard << "QCDscale   lnN    -     -    0.86/1.11 ";
  if( !isLeptonic ) datacard <<  " - - ";
  else              datacard << " 0.977/1.023";
  datacard << std::endl;

  datacard << "JEC            lnN   0.997213/1.00634 - 1.00523/0.992612 1.06054/0.941012 " << std::endl;
  datacard << "JER            lnN   1.01205/0.998054 - 0.963968/1.04624 1/1.06059 " << std::endl;
  datacard << "Btag            lnN   1.02 - 1.015 1.001" << std::endl;
  datacard << "leptEff        lnN    1.025     -    1.025 1.025 1.025" << std::endl;
  datacard << "BG_shape       lnN    -     1.25  - - -" << std::endl;

  

  datacard.close();
  std::cout << "-> Created datacard: " << datacardName << std::endl;


  std::string logfile_signif = "logfile_signif_tmp.log";
  std::string logfile_ul     = "logfile_ul_tmp.log";

  std::string combineComand_signif = "combine -M HybridNew " + datacardName_str + " --significance  --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >& " + logfile_signif;
  std::string combineComand_ul     = "combine -M HybridNew " + datacardName_str + "                 --expectedFromGrid=0.5  --saveToys --fullBToys --saveHybridResult -T 2000 -i 10 -s13 >& " + logfile_ul;

  std::cout << "-> combine: computing significance" << std::endl;
  system( combineComand_signif.c_str() );
  std::cout << "-> combine: computing UL" << std::endl;
  system( combineComand_ul    .c_str() );




  // first scan signif file

  ifstream ifs_signif(logfile_signif);
  std::cout << "-> Scanning logfile_signif: " << logfile_signif << std::endl;
  
  float signif, s_err_plus, s_err_minus;

  while( ifs_signif.good() && !ifs_signif.eof() ) {

    std::string line;
    std::getline(ifs_signif, line);
    TString line_tstr(line);
    if( !(line_tstr.BeginsWith( "Significance: " )) ) continue;

    sscanf( line.c_str(), "Significance: %f %f/%f", &signif, &s_err_plus, &s_err_minus );
    break;

  }

  ifs_signif.close();

  std::cout << "## found significance: " << signif << " +" << s_err_minus << " " << s_err_plus << std::endl;

  // then scan UL file

  ifstream ifs_ul(logfile_ul);
  
  float ul, ul_err;

  while( ifs_ul.good() && !ifs_ul.eof() ) {

    std::string line;
    std::getline(ifs_ul, line);
    TString line_tstr(line);
    if( !(line_tstr.BeginsWith( "Limit: " )) ) continue;

    sscanf( line.c_str(), "Limit: r < %f +/- %f @ 95% CL", &ul, &ul_err );

    break;

  }

  ifs_ul.close();



  delete h1_mgg_thqLeptonic;
  delete h1_mgg_thqHadronic;
  delete h1_mgg_ggh        ;
  delete h1_mgg_vbf        ;
  delete h1_mgg_wzh        ;
  delete h1_mgg_tth        ;

  

  ULSignif_struct returnStruct;
  returnStruct.signif = signif;
  returnStruct.s_err_plus = s_err_plus;
  returnStruct.s_err_minus = s_err_minus;
  returnStruct.ul = ul;
  returnStruct.ul_err = ul_err;
  
  return returnStruct;

}





