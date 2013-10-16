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



bool BLINDED=true;


void createSingleDatacard( const std::string& channel, const std::string& additionalSelection, TTree* tree_data, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_ggh, TTree* tree_vbf, TTree* tree_wzh, TTree* tree_tth, const std::string& suffix="" );
void createSingleDatacard_twoRegions( const std::string& channel, const std::string& additionalSelection, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_tth);
void drawMassPlot( const std::string& category, TH1D* h1_data, TH1D* h1_thq, TH1D* h1_ggh, TH1D* h1_vbf, TH1D* h1_wzh, TH1D* h1_tth, const std::string& suffix="" );
void drawSignalBGData( const std::string& channel, const std::string& var, const std::string& varName, const std::string& units, int nBins, float xMin, float xMax, TTree* tree_sig, TTree* tree_bg, TTree* tree_data, const std::string& fullSelection, const std::string& fullSelection_sidebands, const std::string& suffix="" );
std::pair<float,float> getJECsyst(const std::string& dataset, char* fullSelection, int binMin, int binMax, TFile* file_jecUp, TFile* file_jecDown, TH1D* h1_mgg_central, int nBins, float xMin, float xMax );
void compareSystVariable( const std::string& channel, const std::string& dataset, TTree* tree_central, TFile* file_systUp, TFile* file_systDown, const std::string& fullSelection, const std::string& systName, const std::string& varName, const std::string& axisTitle, const std::string& units, int nBins, float xMin, float xMax, const std::string& suffix="");


int main() {

  system("mkdir -p plots_dataMC");

  //TFile* file_thqLept = TFile::Open("../histograms_CMS-HGG_thqLeptonic.root");
  //TFile* file_thqHadr = TFile::Open("../histograms_CMS-HGG_thqHadronic.root");
  std::string fileName = "../batchOutput6/histograms_CMS-HGG.root";
  TFile* file = TFile::Open(fileName.c_str());
  std::cout << "-> Opening file: " << fileName << std::endl;

  TTree* tree_thqLept = (TTree*)file->Get("thqLeptonic_m125_8TeV");
  TTree* tree_thqHadr = (TTree*)file->Get("thqHadronic_m125_8TeV");
  TTree* tree_tth     = (TTree*)file->Get("tth_m125_8TeV");
  TTree* tree_wzh     = (TTree*)file->Get("wzh_m125_8TeV");
  TTree* tree_ggh     = (TTree*)file->Get("ggh_m125_8TeV");
  TTree* tree_vbf     = (TTree*)file->Get("vbf_m125_8TeV");

  TTree* tree_data    = (TTree*)file->Get("Data");


  createSingleDatacard( "leptonic", "", tree_data, tree_thqLept, tree_thqHadr, tree_ggh, tree_vbf, tree_wzh, tree_tth, "noLDcut" );
  createSingleDatacard( "leptonic", "thqLD_lept>0.25", tree_data, tree_thqLept, tree_thqHadr, tree_ggh, tree_vbf, tree_wzh, tree_tth );
  createSingleDatacard( "hadronic", "", tree_data, tree_thqLept, tree_thqHadr, tree_ggh, tree_vbf, tree_wzh, tree_tth );

  createSingleDatacard_twoRegions( "leptonic", "thqLD_lept>0.25", tree_thqLept, tree_thqHadr, tree_tth );
  createSingleDatacard_twoRegions( "hadronic", "", tree_thqLept, tree_thqHadr, tree_tth );

  return 0;

}



void createSingleDatacard( const std::string& channel, const std::string& additionalSelection, TTree* tree_data, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_ggh, TTree* tree_vbf, TTree* tree_wzh, TTree* tree_tth, const std::string& suffix ) {

  std::string mkdir_command = "mkdir -p plots_dataMC/" + channel;
  system(mkdir_command.c_str());

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
    sprintf( fullSelection,           "weight*( category==%d && %s )", category, additionalSelection.c_str() );
    sprintf( fullSelection_sidebands, "weight*( category==%d && %s && (PhotonsMass<115. || PhotonsMass>135.) )", category, additionalSelection.c_str() );
  } else {
    sprintf( fullSelection,           "weight*( category==%d       )", category );
    sprintf( fullSelection_sidebands, "weight*( category==%d       && (PhotonsMass<115. || PhotonsMass>135.) )", category );
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
  TH1D* h1_mgg_data        = new TH1D("mgg_data", "", nBins, xMin, xMax);
  TH1D* h1_mgg_data_sidebands= new TH1D("mgg_data_sidebands", "", nBins, xMin, xMax);

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

  if( BLINDED )
    tree_data->Project( "mgg_data", "PhotonsMass", fullSelection_sidebands );
  else
    tree_data->Project( "mgg_data", "PhotonsMass", fullSelection );
  tree_data->Project( "mgg_data_sidebands", "PhotonsMass", fullSelection_sidebands );


  float sidebands_yield = h1_mgg_data_sidebands->Integral();

  std::cout << "Data sideband events: " << h1_mgg_data_sidebands->Integral() << std::endl;

  TH1D* h1_mgg_thq = new TH1D(*h1_mgg_thqLeptonic);
  h1_mgg_thq->Add(h1_mgg_thqHadronic);
  h1_mgg_thq->SetName("mgg_thq");

  drawMassPlot( channel, h1_mgg_data, h1_mgg_thq, h1_mgg_ggh, h1_mgg_vbf, h1_mgg_wzh, h1_mgg_tth, suffix );
  if( isLeptonic ) {
    drawSignalBGData( channel, "lept_charge", "Lepton Charge", "", 3, -1.5, 1.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "isLep_mu", "is Muon Event", "", 2, -0.5, 1.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "njets", "Jet Multiplicity", "", 8, 1.5, 9.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "njets_OutsideEtaCut", "Central Jet Multiplicity", "", 8, -0.5, 7.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "njets_InsideEtaCut", "Forward Jet Multiplicity", "", 8, -0.5, 7.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "topMt", "Top Transverse Mass", "", 25, 0., 500., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "qJetPt", "qJet p_{T}", "GeV", 25, 0., 250., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "bJetPt", "bJet p_{T}", "GeV", 25, 0., 250., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "deltaEta_lept_qJet", "#Delta#eta (Lepton-qJet)", "", 25, 0.5, 5.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "thqLD_lept", "tHq Leptonic LD", "", 20, 0., 1.00001, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "met_pfmet", "Particle Flow Missing E_{T}", "GeV", 20, 0., 200., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
  } else {
    drawSignalBGData( channel, "met_pfmet", "Particle Flow Missing E_{T}", "GeV", 20, 0., 200., tree_thqHadronic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "njets", "Jet Multiplicity", "", 8, 1.5, 9.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "njets_OutsideEtaCut", "Central Jet Multiplicity", "", 8, -0.5, 7.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "njets_InsideEtaCut", "Forward Jet Multiplicity", "", 8, -0.5, 7.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "topM", "Top Mass", "", 25, 0., 500., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "qJetPt", "qJet p_{T}", "GeV", 25, 0., 250., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "bJetPt", "bJet p_{T}", "GeV", 25, 0., 250., tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
    drawSignalBGData( channel, "deltaEta_bJet_qJet", "#Delta#eta (bJet-qJet)", "", 25, 0.5, 5.5, tree_thqLeptonic, tree_tth, tree_data, fullSelection, fullSelection_sidebands, suffix );
  }
  

  float massWindow = 5.;

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
  float k_BG = massWindow*2./60.;


  char datacardName[300];
  if( suffix!="" )
    sprintf(datacardName, "datacard_thq_%s_%s.txt", channel.c_str(), suffix.c_str() );
  else
    sprintf(datacardName, "datacard_thq_%s.txt", channel.c_str() );
  std::ofstream datacard(datacardName);

 
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
  datacard << "lumi       lnN    1.044 - 1.044 1.044";
  if( !isLeptonic ) datacard <<  " 1.044";
  datacard << std::endl;

  datacard << "bg         gmN " << (int)sidebands_yield << "    - " << k_BG << "   - - - -" << std::endl;

  datacard << "PDF_ttH   lnN    -     -         0.922/1.078 - -";
  if( !isLeptonic ) datacard <<  " - ";
  datacard << std::endl;
  datacard << "QCDscale_ttH   lnN    -     -    0.86/1.11 - - " << std::endl;
  if( !isLeptonic ) datacard <<  " - ";
  datacard << std::endl;


  TFile* file_jecUp   = TFile::Open("../thq_jetsyst_v5/jecUp/histograms_CMS-HGG.root");
  TFile* file_jecDown = TFile::Open("../thq_jetsyst_v5/jecDown/histograms_CMS-HGG.root");
  TFile* file_jerUp   = TFile::Open("../thq_jetsyst_v7/jerUp/histograms_CMS-HGG.root");

  std::pair<float,float> thqLept_jecsyst = getJECsyst("thqLeptonic_m125_8TeV", fullSelection, binMin, binMax, file_jecUp, file_jecDown, h1_mgg_thqLeptonic, nBins, xMin, xMax);
  std::pair<float,float> thqHadr_jecsyst = getJECsyst("thqHadronic_m125_8TeV", fullSelection, binMin, binMax, file_jecUp, file_jecDown, h1_mgg_thqHadronic, nBins, xMin, xMax);
  std::pair<float,float> tth_jecsyst     = getJECsyst("tth_m125_8TeV",         fullSelection, binMin, binMax, file_jecUp, file_jecDown, h1_mgg_tth,         nBins, xMin, xMax);
  std::pair<float,float> ggh_jecsyst     = getJECsyst("ggh_m125_8TeV",         fullSelection, binMin, binMax, file_jecUp, file_jecDown, h1_mgg_ggh,         nBins, xMin, xMax);
  std::pair<float,float> vbf_jecsyst     = getJECsyst("vbf_m125_8TeV",         fullSelection, binMin, binMax, file_jecUp, file_jecDown, h1_mgg_vbf,         nBins, xMin, xMax);
  std::pair<float,float> wzh_jecsyst     = getJECsyst("wzh_m125_8TeV",         fullSelection, binMin, binMax, file_jecUp, file_jecDown, h1_mgg_wzh,         nBins, xMin, xMax);

  std::pair<float,float> thq_jecsyst;
  thq_jecsyst.first = (yield_thqLept*thqLept_jecsyst.first + yield_thqHadr*thqHadr_jecsyst.first)/(yield_thqLept+yield_thqHadr); 
  thq_jecsyst.second = (yield_thqLept*thqLept_jecsyst.second + yield_thqHadr*thqHadr_jecsyst.second)/(yield_thqLept+yield_thqHadr); 

  datacard << "JEC            lnN   " << thq_jecsyst.first << "/" << thq_jecsyst.second;
  datacard << " - "; //BG from sidebands
  datacard << tth_jecsyst.first << "/" << tth_jecsyst.second << " "; 
  if( isLeptonic ) {
    datacard << wzh_jecsyst.first << "/" << wzh_jecsyst.second << " "; 
  } else {
    datacard << ggh_jecsyst.first << "/" << ggh_jecsyst.second << " "; 
    datacard << vbf_jecsyst.first << "/" << vbf_jecsyst.second << " "; 
  }
  datacard << std::endl;


  std::pair<float,float> thqLept_jersyst = getJECsyst("thqLeptonic_m125_8TeV", fullSelection, binMin, binMax, file_jerUp, 0, h1_mgg_thqLeptonic, nBins, xMin, xMax);
  std::pair<float,float> thqHadr_jersyst = getJECsyst("thqHadronic_m125_8TeV", fullSelection, binMin, binMax, file_jerUp, 0, h1_mgg_thqHadronic, nBins, xMin, xMax);
  std::pair<float,float> tth_jersyst     = getJECsyst("tth_m125_8TeV",         fullSelection, binMin, binMax, file_jerUp, 0, h1_mgg_tth,         nBins, xMin, xMax);
  std::pair<float,float> ggh_jersyst     = getJECsyst("ggh_m125_8TeV",         fullSelection, binMin, binMax, file_jerUp, 0, h1_mgg_ggh,         nBins, xMin, xMax);
  std::pair<float,float> vbf_jersyst     = getJECsyst("vbf_m125_8TeV",         fullSelection, binMin, binMax, file_jerUp, 0, h1_mgg_vbf,         nBins, xMin, xMax);
  std::pair<float,float> wzh_jersyst     = getJECsyst("wzh_m125_8TeV",         fullSelection, binMin, binMax, file_jerUp, 0, h1_mgg_wzh,         nBins, xMin, xMax);

  std::pair<float,float> thq_jersyst;
  thq_jersyst.first  = (yield_thqLept*thqLept_jersyst.first  + yield_thqHadr*thqHadr_jersyst.first) /(yield_thqLept+yield_thqHadr); 
  thq_jersyst.second = (yield_thqLept*thqLept_jersyst.second + yield_thqHadr*thqHadr_jersyst.second)/(yield_thqLept+yield_thqHadr); 

  datacard << "JER            lnN   " << thq_jersyst.second;
  datacard << " - "; //BG from sidebands
  datacard << tth_jersyst.second << " "; 
  if( isLeptonic ) {
    datacard << wzh_jersyst.second << " "; 
  } else {
    datacard << ggh_jersyst.second << " "; 
    datacard << vbf_jersyst.second << " "; 
  }
  datacard << std::endl;


  compareSystVariable( channel, "tth_m125_8TeV", tree_tth, file_jecUp, file_jecDown, fullSelection, "JEC", "bJetPt", "bJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  compareSystVariable( channel, "tth_m125_8TeV", tree_tth, file_jecUp, file_jecDown, fullSelection, "JEC", "qJetPt", "qJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  compareSystVariable( channel, "tth_m125_8TeV", tree_tth, file_jecUp, file_jecDown, fullSelection, "JEC", "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., suffix);

  //if( isLeptonic ) {
  //  compareSystVariable( channel, "thq_m125_8TeV", tree_thqLeptonic, file_jerUp, 0, fullSelection, "JER", "bJetPt", "bJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  //  compareSystVariable( channel, "thq_m125_8TeV", tree_thqLeptonic, file_jerUp, 0, fullSelection, "JER", "qJetPt", "qJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  //  compareSystVariable( channel, "thq_m125_8TeV", tree_thqLeptonic, file_jerUp, 0, fullSelection, "JER", "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., suffix);
  //} else {
  //  compareSystVariable( channel, "thq_m125_8TeV", tree_thqHadronic, file_jerUp, 0, fullSelection, "JER", "bJetPt", "bJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  //  compareSystVariable( channel, "thq_m125_8TeV", tree_thqHadronic, file_jerUp, 0, fullSelection, "JER", "qJetPt", "qJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  //  compareSystVariable( channel, "thq_m125_8TeV", tree_thqHadronic, file_jerUp, 0, fullSelection, "JER", "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., suffix);
  //}

  compareSystVariable( channel, "tth_m125_8TeV", tree_tth, file_jerUp, 0, fullSelection, "JER", "bJetPt", "bJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  compareSystVariable( channel, "tth_m125_8TeV", tree_tth, file_jerUp, 0, fullSelection, "JER", "qJetPt", "qJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
  compareSystVariable( channel, "tth_m125_8TeV", tree_tth, file_jerUp, 0, fullSelection, "JER", "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., suffix);

  if( isLeptonic ) {
    compareSystVariable( channel, "wzh_m125_8TeV", tree_wzh, file_jerUp, 0, fullSelection, "JER", "bJetPt", "bJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
    compareSystVariable( channel, "wzh_m125_8TeV", tree_wzh, file_jerUp, 0, fullSelection, "JER", "qJetPt", "qJet Transverse Momentum", "GeV", 25, 0., 250., suffix);
    compareSystVariable( channel, "wzh_m125_8TeV", tree_wzh, file_jerUp, 0, fullSelection, "JER", "qJetEta", "qJet Pseudorapidity", "", 25, -5., 5., suffix);
  }

  datacard << "Btag           lnN    -     -    1.01  1.01  1.01  1.01 ";
  if(!isLeptonic ) datacard <<  " 1.01";
  datacard << std::endl;
  if(isLeptonic) {
    datacard << "leptEff        lnN    -     -    1.025 1.025 1.025" << std::endl;
    datacard << "BG_shape       lnN    -     1.25  - - -" << std::endl;
  }
  

  datacard.close();
  std::cout << "-> Created datacard: " << datacardName << std::endl;


  delete h1_mgg_thqLeptonic;
  delete h1_mgg_thqHadronic;
  delete h1_mgg_ggh        ;
  delete h1_mgg_vbf        ;
  delete h1_mgg_wzh        ;
  delete h1_mgg_tth        ;
  delete h1_mgg_data       ;

}




void createSingleDatacard_twoRegions( const std::string& channel, const std::string& additionalSelection, TTree* tree_thqLeptonic, TTree* tree_thqHadronic, TTree* tree_tth ) {


  int category=-1;
  if( channel=="leptonic" ) {
    category=11;
  } else if( channel=="hadronic" ) {
    category=12;
  } else {
    std::cout << "-> Only 'leptonic' and 'hadronic' are supported channels. Exiting." << std::endl;
    exit(11111);
  }

  

  float massWindow = 3.;



  char fullSelection_HS[500];
  if( additionalSelection!="" )
    sprintf( fullSelection_HS, "weight*( category==%d && %s && PhotonsMass>%f && PhotonsMass<%f )", category, additionalSelection.c_str(), 125.-massWindow, 125.+massWindow );
  else
    sprintf( fullSelection_HS, "weight*( category==%d &&       PhotonsMass>%f && PhotonsMass<%f )", category, 125.-massWindow, 125.+massWindow );


  char fullSelection_LS[500];
  if( additionalSelection!="" )
    sprintf( fullSelection_LS, "weight*( category==%d && %s && ((PhotonsMass>%f && PhotonsMass<%f) || (PhotonsMass>%f && PhotonsMass<%f)) )", category, additionalSelection.c_str(), 125.-2.*massWindow, 125.-massWindow, 125.+massWindow, 125.+2.*massWindow );
  else
    sprintf( fullSelection_LS, "weight*( category==%d &&       ((PhotonsMass>%f && PhotonsMass<%f) || (PhotonsMass>%f && PhotonsMass<%f)) )", category, 125.-2.*massWindow, 125.-massWindow, 125.+massWindow, 125.+2.*massWindow );


  TH1D* h1_mgg_HS_thqLeptonic = new TH1D("mgg_HS_thqLeptonic", "", 160, 100., 180.);
  TH1D* h1_mgg_HS_thqHadronic = new TH1D("mgg_HS_thqHadronic", "", 160, 100., 180.);
  TH1D* h1_mgg_HS_tth         = new TH1D("mgg_HS_tth", "", 160, 100., 180.);

  h1_mgg_HS_thqLeptonic->Sumw2();
  h1_mgg_HS_thqHadronic->Sumw2();
  h1_mgg_HS_tth->Sumw2();

  tree_thqLeptonic->Project( "mgg_HS_thqLeptonic", "PhotonsMass", fullSelection_HS );
  tree_thqHadronic->Project( "mgg_HS_thqHadronic", "PhotonsMass", fullSelection_HS );
  tree_tth        ->Project( "mgg_HS_tth",         "PhotonsMass", fullSelection_HS );


  TH1D* h1_mgg_LS_thqLeptonic = new TH1D("mgg_LS_thqLeptonic", "", 160, 100., 180.);
  TH1D* h1_mgg_LS_thqHadronic = new TH1D("mgg_LS_thqHadronic", "", 160, 100., 180.);
  TH1D* h1_mgg_LS_tth         = new TH1D("mgg_LS_tth", "", 160, 100., 180.);

  h1_mgg_LS_thqLeptonic->Sumw2();
  h1_mgg_LS_thqHadronic->Sumw2();
  h1_mgg_LS_tth->Sumw2();

  tree_thqLeptonic->Project( "mgg_LS_thqLeptonic", "PhotonsMass", fullSelection_LS );
  tree_thqHadronic->Project( "mgg_LS_thqHadronic", "PhotonsMass", fullSelection_LS );
  tree_tth        ->Project( "mgg_LS_tth",         "PhotonsMass", fullSelection_LS );



  float yield_HS_thqLeptonic = h1_mgg_HS_thqLeptonic->Integral();
  float yield_HS_thqHadronic = h1_mgg_HS_thqHadronic->Integral();
  float yield_HS_tth         = h1_mgg_HS_tth->Integral();

  float yield_HS_signal = yield_HS_thqLeptonic + yield_HS_thqHadronic;
  float yield_HS_SMH = yield_HS_tth;
  if( channel=="leptonic" ) { // quick fix for now
    yield_HS_SMH += 0.01;
  } else if( channel=="hadronic" ) {
    yield_HS_SMH *= 2.;
  }


  float yield_LS_thqLeptonic = h1_mgg_LS_thqLeptonic->Integral();
  float yield_LS_thqHadronic = h1_mgg_LS_thqHadronic->Integral();
  float yield_LS_tth         = h1_mgg_LS_tth->Integral();

  float yield_LS_signal = yield_LS_thqLeptonic + yield_LS_thqHadronic;
  float yield_LS_SMH = yield_LS_tth;
  if( channel=="leptonic" ) { // quick fix for now
    yield_LS_SMH += 0.01;
  } else if( channel=="hadronic" ) {
    yield_LS_SMH *= 2.;
  }


  float k_BG = massWindow*2./60.;


  char datacardName[300];
  sprintf(datacardName, "datacard_2R_thq_%s.txt", channel.c_str() );
  std::ofstream datacard(datacardName);

 
  datacard << "imax 2" << std::endl;
  datacard << "jmax 2 " << std::endl;
  datacard << "kmax *" << std::endl;
  datacard << "    " << std::endl;
  datacard << "    " << std::endl;
  datacard << "bin  1   2" << std::endl;
  datacard << "observation 0  0" << std::endl;
  datacard << "" << std::endl;
  datacard << "    " << std::endl;
  datacard << "bin            1        1       1        2        2       2   "       << std::endl;
  datacard << "process        s        b       bSMH     s        b       bSMH"    << std::endl;
  datacard << "process        0        1       2        0        1       2   "       << std::endl;
  datacard << "rate   " << yield_HS_signal <<  "  0. "   <<  yield_HS_SMH   <<  "   " << yield_LS_signal <<  "  0. "   <<  yield_LS_SMH   << std::endl;
  datacard << "      " << std::endl;
  datacard << "      " << std::endl;
  datacard << "#syst " << std::endl;
  datacard << "lumi       lnN    1.044 - 1.044   1.044 - 1.044" << std::endl;

  datacard << "bg         gmN 0    - " << k_BG << "   -  - " << k_BG << "   -" << std::endl;

  datacard << "PDF_ttH   lnN    -     -         0.922/1.078    -     -         0.922/1.078" << std::endl;
  datacard << "QCDscale_ttH   lnN    -     -    0.86/1.11    -     -    0.86/1.11" << std::endl;

  datacard << "JEC            lnN    -     -    1.0226   -     -    1.0226" << std::endl;
  datacard << "JER            lnN    -     -    1.0027   -     -    1.0027" << std::endl;
  datacard << "Btag           lnN    -     -    1.01   -     -    1.01" << std::endl;
  if( channel=="leptonic" )
    datacard << "leptEff        lnN    -     -    1.025   -     -    1.025" << std::endl;
  

  datacard.close();


  delete h1_mgg_HS_thqLeptonic;
  delete h1_mgg_HS_thqHadronic;
  delete h1_mgg_HS_tth        ;

  delete h1_mgg_LS_thqLeptonic;
  delete h1_mgg_LS_thqHadronic;
  delete h1_mgg_LS_tth        ;

}



void drawMassPlot( const std::string& category, TH1D* h1_data, TH1D* h1_thq, TH1D* h1_ggh, TH1D* h1_vbf, TH1D* h1_wzh, TH1D* h1_tth, const std::string& suffix ) {

  DrawBase* db = new DrawBase("mgg");
  db->set_lumi(19700.);
  TFile* dummyFile = TFile::Open("dummy.root", "recreate");
  db->add_dataFile(dummyFile, "Data");


  TGraphAsymmErrors* gr_data = fitTools::getGraphPoissonErrors( h1_data, false );
  gr_data->SetMarkerStyle(20);  
  gr_data->SetMarkerSize(1.3);  

  
  h1_thq->SetFillColor(kWhite);
  h1_tth->SetFillColor(kGray);
  h1_wzh->SetFillColor(28);
  h1_vbf->SetFillColor(90);
  h1_ggh->SetFillColor(46);

  h1_thq->SetLineColor(kBlack);
  h1_tth->SetLineColor(kBlack);
  h1_wzh->SetLineColor(kBlack);
  h1_vbf->SetLineColor(kBlack);
  h1_ggh->SetLineColor(kBlack);


  TString category_tstr(category);
  bool isLeptonic = category_tstr.Contains("leptonic");

  THStack stack;
  if( !isLeptonic ) {
    stack.Add(h1_ggh);
    stack.Add(h1_vbf);
  }
  stack.Add(h1_wzh);
  stack.Add(h1_tth);
  stack.Add(h1_thq);



  std::string channelName_legend = (isLeptonic) ? "Leptonic Channel" : "Hadronic Channel";

  float yMin_leg = (isLeptonic) ? 0.9-4.*0.07 : 0.9-6.*0.07;
  TLegend* legend = new TLegend( 0.62, yMin_leg, 0.9, 0.92, channelName_legend.c_str() );
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.038);
  legend->SetTextColor(kBlack);
  legend->AddEntry( gr_data, "Data", "P" );
  legend->AddEntry( h1_thq, "tHq (Ct = -1)", "F" );
  legend->AddEntry( h1_tth, "ttH (125)", "F" );
  legend->AddEntry( h1_wzh, "VH (125)", "F" );
  if( !isLeptonic ) {
    legend->AddEntry( h1_vbf, "VBF H (125)", "F" );
    legend->AddEntry( h1_ggh, "ggF H (125)", "F" );
  }


  float yMax = 0.;
  float yMax_data = h1_data->GetMaximum();
  if( yMax_data>0. ) {
    if( yMax_data < 1.5 ) yMax=3.5;
    else if( yMax_data < 2.5 ) yMax=6.;
    else                       yMax=yMax_data*2.;
  } else {
    float yMax_mc = h1_thq->GetMaximum()*1.2;
    yMax = 1.8*yMax_mc;
  }


  TH2D* h2_axes = new TH2D( "axes", "", 10, 100., 180., 10, 0., yMax);
  h2_axes->SetXTitle( "Diphoton Mass [GeV]");
  h2_axes->SetYTitle( "Events / (1 GeV)");

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();


  TPaveText* labelTop = db->get_labelTop();

  h2_axes->Draw();
  legend->Draw("same");   
  labelTop->Draw("same");
  stack.Draw("histo same");

  if( BLINDED ) {
    TLine* lineBlind_low = new TLine( 115., 0., 115., yMax );
    TLine* lineBlind_hi  = new TLine( 135., 0., 135., yMax );
    lineBlind_low->SetLineColor(46);
    lineBlind_hi ->SetLineColor(46);
    lineBlind_low->SetLineStyle(2);
    lineBlind_hi ->SetLineStyle(2);
    lineBlind_low->SetLineWidth(2);
    lineBlind_hi ->SetLineWidth(2);
    lineBlind_low->Draw("same");
    lineBlind_hi ->Draw("same");
  }

  gr_data->Draw("p same");

  gPad->RedrawAxis();


  std::string canvasName = "plots_dataMC/" + category + "/mggPlot_" + category;
  if( suffix!="" ) canvasName = canvasName + "_" + suffix;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";

  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  delete c1;
  delete h2_axes;
  delete legend;

}



void drawSignalBGData( const std::string& channel, const std::string& var, const std::string& varName, const std::string& units, int nBins, float xMin, float xMax, TTree* tree_sig, TTree* tree_bg, TTree* tree_data, const std::string& fullSelection, const std::string& fullSelection_sidebands, const std::string& suffix ) {


  DrawBase* db = new DrawBase("sigbfdata" );

  TH1D* h1_data = new TH1D("data", "", nBins, xMin, xMax ); 
  TH1D* h1_sig = new TH1D("sig", "", nBins, xMin, xMax ); 
  TH1D* h1_bg = new TH1D("bg", "", nBins, xMin, xMax ); 

  h1_data->Sumw2();
  h1_sig->Sumw2();
  h1_bg->Sumw2();

  h1_sig->SetFillColor(46);
  h1_bg->SetFillColor(38);
  h1_sig->SetLineColor(46);
  h1_bg->SetLineColor(38);

  h1_sig->SetFillStyle(3004);
  h1_bg->SetFillStyle(3005);


  tree_data->Project( "data", var.c_str(), fullSelection_sidebands.c_str() );
  tree_sig->Project(  "sig",  var.c_str(), fullSelection.c_str() );
  tree_bg ->Project(  "bg",   var.c_str(), fullSelection.c_str() );


  float data_integral = h1_data->Integral();
  if( data_integral < 2 ) data_integral=3.;
  h1_sig->Scale( data_integral/h1_sig->Integral() );
  h1_bg->Scale( data_integral/h1_bg->Integral() );
  


  TGraphAsymmErrors* gr_data = fitTools::getGraphPoissonErrors( h1_data, false );
  gr_data->SetMarkerStyle(20);  
  gr_data->SetMarkerSize(1.3);  
   

  TString category_tstr(channel);
  bool isLeptonic = category_tstr.Contains("leptonic");

  std::string channelName_legend = (isLeptonic) ? "Leptonic Channel" : "Hadronic Channel";


  TLegend* legend = new TLegend( 0.55, 0.66, 0.9, 0.92, channelName_legend.c_str() );
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.038);
  legend->SetTextColor(kBlack);
  legend->AddEntry( gr_data, "Sidebands Data", "P" );
  legend->AddEntry( h1_sig, "tHq (125)", "F" );
  legend->AddEntry( h1_bg, "ttH (125)", "F" );


  float yMax = 0.;
  float yMax_data = h1_data->GetMaximum();
  if( yMax_data>0. ) {
    if( yMax_data < 1.5 ) yMax=3.5;
    else if( yMax_data < 2.5 ) yMax=6.;
    else                       yMax=yMax_data*2.;
  } else {
    float yMax_mc = h1_sig->GetMaximum()*1.2;
    yMax = 1.8*yMax_mc;
  }


  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., yMax);
  std::string xTitle = varName;
  if( units!="" ) xTitle = xTitle + " [" + units + "]";
  h2_axes->SetXTitle(xTitle.c_str());
  h2_axes->SetYTitle( "Arbitrary Units" );

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();


  TPaveText* labelTop = db->get_labelTop();

  h2_axes->Draw();
  legend->Draw("same");   
  labelTop->Draw("same");
  h1_bg->Draw("h same");
  h1_sig->Draw("h same");
  gr_data->Draw("p same");


  gPad->RedrawAxis();

  std::string canvasName = "plots_dataMC/" + channel + "/" + var + "_SignalBGData_" + channel;
  if( suffix!="" ) canvasName = canvasName + "_" + suffix;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";

  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());

  delete h1_data;
  delete h1_sig;
  delete h1_bg;

  delete h2_axes;

  delete legend;
  delete c1;

}



std::pair<float,float> getJECsyst(const std::string& dataset, char* fullSelection, int binMin, int binMax, TFile* file_jecUp, TFile* file_jecDown, TH1D* h1_mgg_central, int nBins, float xMin, float xMax ) {
//std::pair<float,float> getJECsyst(const std::string& dataset, char* fullSelection, int binMin, int binMax, TFile* file_jecUp, TFile* file_jecDown, float yield_central, int nBins, float xMin, float xMax ) {

  TTree* tree_jecUp = (TTree*)file_jecUp->Get(dataset.c_str());
  TTree* tree_jecDown = (file_jecDown!=0) ? (TTree*)file_jecDown->Get(dataset.c_str()) : 0;

  TH1D* h1_mgg_jecUp = new TH1D("mgg_jecUp", "", nBins, xMin, xMax);
  TH1D* h1_mgg_jecDown = new TH1D("mgg_jecDown", "", nBins, xMin, xMax);
  h1_mgg_jecUp->Sumw2();
  h1_mgg_jecDown->Sumw2();

  tree_jecUp->Project( "mgg_jecUp", "PhotonsMass", fullSelection );
  if( tree_jecDown!=0 )
    tree_jecDown->Project( "mgg_jecDown", "PhotonsMass", fullSelection );

  float yield_central = h1_mgg_central->Integral(binMin, binMax-1);
  float yield_jecUp   = h1_mgg_jecUp->Integral(binMin, binMax-1);
  float yield_jecDown = (tree_jecDown!=0) ? h1_mgg_jecDown->Integral(binMin, binMax-1) : 0.;

  float systUp = 1.+((yield_jecUp-yield_central)/yield_central);
  float systDown = (tree_jecDown!=0) ? 1.+((yield_jecDown-yield_central)/yield_central) : 0.;


  std::pair<float,float> systPair;
  systPair.first = systDown;
  systPair.second = systUp;

  delete h1_mgg_jecUp;
  delete h1_mgg_jecDown;

  return systPair;

}


void compareSystVariable( const std::string& channel, const std::string& dataset, TTree* tree_central, TFile* file_systUp, TFile* file_systDown, const std::string& fullSelection, const std::string& systName, const std::string& varName, const std::string& axisTitle, const std::string& units, int nBins, float xMin, float xMax, const std::string& suffix) {


  DrawBase* db = new DrawBase("syst" );

  TTree* tree_systUp = (TTree*)file_systUp->Get(dataset.c_str());
  TTree* tree_systDown = (file_systDown!=0) ? (TTree*)file_systDown->Get(dataset.c_str()) : 0;

  TH1D* h1_central = new TH1D("central", "", nBins, xMin, xMax);
  TH1D* h1_systUp = new TH1D("systUp", "", nBins, xMin, xMax);
  TH1D* h1_systDown = new TH1D("systDown", "", nBins, xMin, xMax);

  h1_central->Sumw2();
  h1_systUp->Sumw2();
  h1_systDown->Sumw2();

  tree_central->Project( "central", varName.c_str(), fullSelection.c_str() );
  tree_systUp->Project( "systUp", varName.c_str(), fullSelection.c_str() );
  if( tree_systDown!=0 )
    tree_systDown->Project( "systDown", varName.c_str(), fullSelection.c_str() );

  h1_central ->SetLineColor(kBlack);
  h1_systUp  ->SetLineColor(kGreen);
  h1_systDown->SetLineColor(kRed);

  h1_central ->SetLineWidth(2);
  h1_systUp  ->SetLineWidth(2);
  h1_systDown->SetLineWidth(2);

  //h1_systUp  ->SetLineStyle(2);
  //h1_systDown->SetLineStyle(2);


  TLegend* legend = new TLegend( 0.6, 0.66, 0.9, 0.92, dataset.c_str() );
  legend->SetTextFont(42);
  legend->SetFillColor(kWhite);
  legend->SetTextSize(0.038);
  legend->SetTextColor(kBlack);
  legend->AddEntry( h1_central, "Central", "L" );
  std::string systUp_text = systName + " +1#sigma";
  std::string systDown_text = systName + " -1#sigma";
  legend->AddEntry( h1_systUp, systUp_text.c_str(), "L" );
  if( tree_systDown!=0 )
    legend->AddEntry( h1_systDown, systDown_text.c_str(), "L" );


  float yMax = 1.6*h1_central->GetMaximum();


  TH2D* h2_axes = new TH2D( "axes", "", 10, xMin, xMax, 10, 0., yMax);
  std::string xTitle = axisTitle;
  if( units!="" ) xTitle = xTitle + " [" + units + "]";
  h2_axes->SetXTitle(xTitle.c_str());
  h2_axes->SetYTitle( "Events" );

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();


  TPaveText* labelTop = db->get_labelTop();

  h2_axes->Draw();
  legend->Draw("same");   
  labelTop->Draw("same");
  h1_central->Draw("h same");
  h1_systUp->Draw("h same");
  if( tree_systDown!=0 )
    h1_systDown->Draw("h same");


  gPad->RedrawAxis();

  std::string canvasName = "plots_dataMC/" + channel + "/syst" + systName + "_" + varName + "_" + dataset + "_" + channel;
  if( suffix!="" ) canvasName = canvasName + "_" + suffix;
  std::string canvasName_eps = canvasName + ".eps";
  std::string canvasName_png = canvasName + ".png";

  c1->SaveAs(canvasName_eps.c_str());
  c1->SaveAs(canvasName_png.c_str());


  delete db;

  delete h1_central;
  delete h1_systUp;
  delete h1_systDown;

  delete h2_axes;

  delete legend;
  delete c1;



}
