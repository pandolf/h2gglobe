#include <iostream>
#include <fstream>

#include "TROOT.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"



bool BLINDED=true;



//void printYields( DrawBase* db, const std::string& suffix, float massWindow=3. );
void drawYieldsCascade( DrawBase* db, std::vector< std::pair<std::string,std::string> > cuts );



int main( int argc, char* argv[] ) {


  std::string batchProd = "preselectionLoose_v1";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }

  
  DrawBase* db = new DrawBase("THq");

  //db->set_isCMSArticle(true);
  //db_inclusiveData->set_isCMSArticle(true);

  std::string outputdir_str = "THq_plots_dataMC_"+batchProd;
  db->set_outputdir( outputdir_str );


  std::string inputDir = "dbFiles_"+batchProd+"/";


  // add categories 11-12 data for final plots:
  std::string dataFileName = inputDir + "data.root";
  TFile* file_data = TFile::Open( dataFileName.c_str() );
  std::string dataName = (BLINDED) ? "Blinded Data" : "Data";
  db->add_dataFile( file_data, "data", dataName );




  // MC files:


  std::string diphojet_sherpa_8TeVFileName = inputDir + "diphojet_sherpa_8TeV.root";
  TFile* file_diphojet_sherpa_8TeV = TFile::Open( diphojet_sherpa_8TeVFileName.c_str() );
  db->add_mcFile( file_diphojet_sherpa_8TeV, "diphojet_sherpa_8TeV", "Diphoton", 39 );


  std::string gjet_pfFileName = inputDir + "gjet.root";
  TFile* file_gjet_pf = TFile::Open( gjet_pfFileName.c_str() );
  db->add_mcFile( file_gjet_pf, "gjet", "#gamma + Jet", 42 );

  std::string qcd_FileName = inputDir + "qcd.root";
  TFile* file_qcd = TFile::Open( qcd_FileName.c_str() );
  db->add_mcFile( file_qcd, "qcd", "QCD", 44 );

  std::string tt_FileName = inputDir + "TTJets.root";
  TFile* file_tt = TFile::Open( tt_FileName.c_str() );
  db->add_mcFile( file_tt, "tt", "t#bar{t}", 45 );

  std::string tggFileName = inputDir + "tgg.root";
  TFile* file_tgg = TFile::Open( tggFileName.c_str() );
  db->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 29 );
  //db_inclusiveData->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 29 );

  std::string ttggFileName = inputDir + "ttgg.root";
  TFile* file_ttgg = TFile::Open( ttggFileName.c_str() );
  db->add_mcFile( file_ttgg, "ttgg", "tt#gamma#gamma", 38 );


//int ii=19;
//std::string ZGToLLGFileName = inputDir + "ZGToLLG.root";
//TFile* file_ZGToLLG = TFile::Open( ZGToLLGFileName.c_str() );
//db->add_mcFile( file_ZGToLLG, "ZGToLLG", "ZGToLLG", ii+1 );

//std::string WGToLNuGFileName = inputDir + "WGToLNuG.root";
//TFile* file_WGToLNuG = TFile::Open( WGToLNuGFileName.c_str() );
//db->add_mcFile( file_WGToLNuG, "WGToLNuG", "WGToLNuG", ii+1 );

//std::string WJetsToLNuFileName = inputDir + "WJetsToLNu.root";
//TFile* file_WJetsToLNu = TFile::Open( WJetsToLNuFileName.c_str() );
//db->add_mcFile( file_WJetsToLNu, "WJetsToLNu", "WJetsToLNu", ii+1 );

//std::string WWJetsTo2L2NuFileName = inputDir + "WWJetsTo2L2Nu.root";
//TFile* file_WWJetsTo2L2Nu = TFile::Open( WWJetsTo2L2NuFileName.c_str() );
//db->add_mcFile( file_WWJetsTo2L2Nu, "WWJetsTo2L2Nu", "WWJetsTo2L2Nu", ii+1 );

//std::string WZJetsTo3LNuFileName = inputDir + "WZJetsTo3LNu.root";
//TFile* file_WZJetsTo3LNu = TFile::Open( WZJetsTo3LNuFileName.c_str() );
//db->add_mcFile( file_WZJetsTo3LNu, "WZJetsTo3LNu", "WZJetsTo3LNu", ii+1 );

//std::string ZZJetsTo2L2NuFileName = inputDir + "ZZJetsTo2L2Nu.root";
//TFile* file_ZZJetsTo2L2Nu = TFile::Open( ZZJetsTo2L2NuFileName.c_str() );
//db->add_mcFile( file_ZZJetsTo2L2Nu, "ZZJetsTo2L2Nu", "ZZJetsTo2L2Nu", ii+1 );

//std::string ZZJetsTo2L2QFileName = inputDir + "ZZJetsTo2L2Q.root";
//TFile* file_ZZJetsTo2L2Q = TFile::Open( ZZJetsTo2L2QFileName.c_str() );
//db->add_mcFile( file_ZZJetsTo2L2Q, "ZZJetsTo2L2Q", "ZZJetsTo2L2Q", ii+1 );

//std::string ZZJetsTo4LFileName = inputDir + "ZZJetsTo4L.root";
//TFile* file_ZZJetsTo4L = TFile::Open( ZZJetsTo4LFileName.c_str() );
//db->add_mcFile( file_ZZJetsTo4L, "ZZJetsTo4L", "ZZJetsTo4L", ii+1 );

//std::string Wpgg_dR02FileName = inputDir + "Wpgg_dR02.root";
//TFile* file_Wpgg_dR02 = TFile::Open( Wpgg_dR02FileName.c_str() );
//db->add_mcFile( file_Wpgg_dR02, "Wpgg_dR02", "Wpgg_dR02", ii+1 );

//std::string Wmgg_dR02FileName = inputDir + "Wmgg_dR02.root";
//TFile* file_Wmgg_dR02 = TFile::Open( Wmgg_dR02FileName.c_str() );
//db->add_mcFile( file_Wmgg_dR02, "Wmgg_dR02", "Wmgg_dR02", ii+1 );

//std::string Zgg_dR02FileName = inputDir + "Zgg_dR02.root";
//TFile* file_Zgg_dR02 = TFile::Open( Zgg_dR02FileName.c_str() );
//db->add_mcFile( file_Zgg_dR02, "Zgg_dR02", "Zgg_dR02", ii+1 );



  // first some shape norm plots:
  float lumi = 19715.;
  db->set_lumiNormalization(lumi);
  db->set_drawZeros(false);


  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && (PhotonsMass<115. || PhotonsMass>135.) )"), 40, 100., 180., "mgg", "Diphoton Mass", "GeV", "Events", true );

  std::vector< std::pair<std::string,std::string> > cuts;
  cuts.push_back( std::pair<std::string,std::string>( "(PhotonsMass<115. || PhotonsMass>135.)", "Two Photons, two jets") );
  cuts.push_back( std::pair<std::string,std::string>( "ph1_pt > 50.*PhotonsMass/120.", "p_{T}1 > 50 m_{#gamma#gamma}/120 GeV") );
  cuts.push_back( std::pair<std::string,std::string>( "leptPt>10.", "Lepton") );
  cuts.push_back( std::pair<std::string,std::string>( "nbjets_loose>1", "CSVL") );
  cuts.push_back( std::pair<std::string,std::string>( "nbjets_medium>1", "CSVM") );
  cuts.push_back( std::pair<std::string,std::string>( "abs(qJetEta)>1.", "|#eta(qJet)|>1") );
  cuts.push_back( std::pair<std::string,std::string>( "thqLD_lept>0.25", "LD>0.25") );
  
  drawYieldsCascade( db, cuts );

  return 0;

}



void drawYieldsCascade( DrawBase* db, std::vector< std::pair<std::string,std::string> > cuts ) {


  int nBins = cuts.size();
//  TH2D* h2_yieldsCascade = new TH2D("yields_cascade", "", nBins, 0., nBins, 10, 0., 30000. );
  TH2D* h2_yieldsCascade = new TH2D("yields_cascade", "", nBins, 0., nBins, 10, 0.1, 60000. );

  std::string selection = "dbWeight*( category==11 ";

  for( unsigned ibin=0; ibin<nBins; ++ibin )
    h2_yieldsCascade->GetXaxis()->SetBinLabel(ibin+1, cuts[ibin].second.c_str());

  TCanvas* cc = new TCanvas("cc", "", 600, 600);
  cc->cd();
  cc->SetLogy();
  
  h2_yieldsCascade->SetYTitle( "Events" );
  h2_yieldsCascade->Draw("");


  TLegend* legend;

  for( unsigned ibin=0; ibin<nBins; ++ibin ) {

    selection = selection + " && (" + cuts[ibin].first + ")";
    std::string this_selection = selection + " )";
    std::cout << "selection: " << this_selection << std::endl;
    TCanvas* c3 = db->drawHisto_fromTree( "tree_passedEvents", Form("thqLD_lept + %f", (float)ibin), this_selection, 1, ibin, ibin+1., Form("tmp%d", ibin), "", "", "Events", true );
    legend = (TLegend*)gROOT->FindObject("dbLegend");
    TGraphAsymmErrors* gr_data = new TGraphAsymmErrors(*(db->get_lastHistos_dataGraph()));
    gr_data->SetName(Form("gr_%d", ibin));
    THStack* mcStack = new THStack(*(db->get_lastHistos_mcStack()));
    mcStack->SetName(Form("stack_%d", ibin));
    
    cc->cd();
    mcStack->Draw("histo same");
    gr_data->Draw("p same");

  }

  legend->Draw("same");

  gPad->RedrawAxis();
  cc->SaveAs(Form("%s/yieldsCascade.eps", db->get_outputdir().c_str()));
  cc->SaveAs(Form("%s/yieldsCascade.png", db->get_outputdir().c_str()));



}
