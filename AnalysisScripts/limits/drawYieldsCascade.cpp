#include <iostream>
#include <fstream>

#include "TROOT.h"

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"



bool BLINDED=true;



void comparePhotonsMass( DrawBase* db, std::vector< std::pair<std::string,std::string> > cuts );
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
  db->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 21 );
  //db->set_mcWeight( "tgg", 3.37e-3/0.001316 );

  std::string topFileName = inputDir + "tt_tgg.root";
  system( Form("hadd -f %s %s %s", topFileName.c_str(), tt_FileName.c_str(), tggFileName.c_str()) );
  TFile* file_top = TFile::Open( topFileName.c_str() );
  //db->add_mcFile( file_top, "top", "t#bar{t} / t#gamma#gamma", 45 );


  std::string ttggFileName = inputDir + "ttgg.root";
  //std::string ttggFileName = "dbFiles_ttgg_tmp/ttgg.root";
  TFile* file_ttgg = TFile::Open( ttggFileName.c_str() );
  db->add_mcFile( file_ttgg, "ttgg", "tt#gamma#gamma", 38 );
  db->set_mcWeight( "ttgg", 9.07045 );

  std::string ttgFileName = inputDir + "ttg.root";
  TFile* file_ttg = TFile::Open( ttgFileName.c_str() );
  db->add_mcFile( file_ttg, "ttg", "tt#gamma", 18 );


  int ii=25;
  std::string ZGToLLGFileName = inputDir + "ZGToLLG.root";
  TFile* file_ZGToLLG = TFile::Open( ZGToLLGFileName.c_str() );
  db->add_mcFile( file_ZGToLLG, "ZGToLLG", "Z#gamma", 29 );

  //std::string WGToLNuGFileName = inputDir + "WGToLNuG.root";
  //TFile* file_WGToLNuG = TFile::Open( WGToLNuGFileName.c_str() );
  //db->add_mcFile( file_WGToLNuG, "WGToLNuG", "WGToLNuG", ii+=2 );

  //std::string wz_gFileName = inputDir + "zg_wg.root";
  //system( Form("hadd -f %s %s %s", wz_gFileName.c_str(), ZGToLLGFileName.c_str(), WGToLNuGFileName.c_str()) );
  //TFile* file_wz_g = TFile::Open( wz_gFileName.c_str() );
  //db->add_mcFile( file_wz_g, "wz_g", "W/Z + #gamma", 29 );

  //std::string WJetsToLNuFileName = inputDir + "WJetsToLNu.root";
  //TFile* file_WJetsToLNu = TFile::Open( WJetsToLNuFileName.c_str() );
  ////db->add_mcFile( file_WJetsToLNu, "WJetsToLNu", "WJetsToLNu", ii++ );

  //std::string WWJetsTo2L2NuFileName = inputDir + "WWJetsTo2L2Nu.root";
  //TFile* file_WWJetsTo2L2Nu = TFile::Open( WWJetsTo2L2NuFileName.c_str() );
  //db->add_mcFile( file_WWJetsTo2L2Nu, "WWJetsTo2L2Nu", "WWJetsTo2L2Nu", ii+=2 );

  //std::string WZJetsTo3LNuFileName = inputDir + "WZJetsTo3LNu.root";
  //TFile* file_WZJetsTo3LNu = TFile::Open( WZJetsTo3LNuFileName.c_str() );
  //db->add_mcFile( file_WZJetsTo3LNu, "WZJetsTo3LNu", "WZJetsTo3LNu", ii+=2 );

  //std::string ZZJetsTo2L2NuFileName = inputDir + "ZZJetsTo2L2Nu.root";
  //TFile* file_ZZJetsTo2L2Nu = TFile::Open( ZZJetsTo2L2NuFileName.c_str() );
  //db->add_mcFile( file_ZZJetsTo2L2Nu, "ZZJetsTo2L2Nu", "ZZJetsTo2L2Nu", ii+=2 );

  //std::string ZZJetsTo2L2QFileName = inputDir + "ZZJetsTo2L2Q.root";
  //TFile* file_ZZJetsTo2L2Q = TFile::Open( ZZJetsTo2L2QFileName.c_str() );
  //db->add_mcFile( file_ZZJetsTo2L2Q, "ZZJetsTo2L2Q", "ZZJetsTo2L2Q", ii+=2 );

  //std::string ZZJetsTo4LFileName = inputDir + "ZZJetsTo4L.root";
  //TFile* file_ZZJetsTo4L = TFile::Open( ZZJetsTo4LFileName.c_str() );
  //db->add_mcFile( file_ZZJetsTo4L, "ZZJetsTo4L", "ZZJetsTo4L", ii+=2 );

  //std::string Wpgg_dR02FileName = inputDir + "Wpgg_dR02.root";
  //TFile* file_Wpgg_dR02 = TFile::Open( Wpgg_dR02FileName.c_str() );
  //db->add_mcFile( file_Wpgg_dR02, "Wpgg_dR02", "Wpgg_dR02", ii+=2 );

  //std::string Wmgg_dR02FileName = inputDir + "Wmgg_dR02.root";
  //TFile* file_Wmgg_dR02 = TFile::Open( Wmgg_dR02FileName.c_str() );
  //db->add_mcFile( file_Wmgg_dR02, "Wmgg_dR02", "Wmgg_dR02", ii+=2 );

  //std::string Zgg_dR02FileName = inputDir + "Zgg_dR02.root";
  //TFile* file_Zgg_dR02 = TFile::Open( Zgg_dR02FileName.c_str() );
  //db->add_mcFile( file_Zgg_dR02, "Zgg_dR02", "Zgg_dR02", ii+=2 );



  // first some shape norm plots:
  float lumi = 19715.;
  db->set_lumiNormalization(lumi);
  db->set_drawZeros(false);

  std::string blindCondition = "(PhotonsMass<115. || PhotonsMass>135.)";
  //std::string blindCondition = "PhotonsMass>100.";

  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s )", blindCondition.c_str()), 40, 100., 180., "mgg_presel", "Diphoton Mass", "GeV", "Events", true );

  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && leptPt>10. )", blindCondition.c_str()), 16, 100., 180., "mgg_preselLept", "Diphoton Mass", "GeV");
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1.)", blindCondition.c_str()), 16, 100., 180., "mgg_CSV0_noLDcut", "Diphoton Mass", "GeV");
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && nbjets_loose>0)", blindCondition.c_str()), 16, 100., 180., "mgg_CSVL_noLDcut", "Diphoton Mass", "GeV");
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && nbjets_medium>0)", blindCondition.c_str()), 16, 100., 180., "mgg_CSVM_noLDcut", "Diphoton Mass", "GeV");
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && thqLD_lept>0.25)", blindCondition.c_str()), 16, 100., 180., "mgg_CSV0", "Diphoton Mass", "GeV");
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && thqLD_lept>0.25 && nbjets_loose>0)", blindCondition.c_str()), 16, 100., 180., "mgg_CSVL", "Diphoton Mass", "GeV");
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && thqLD_lept>0.25 && nbjets_medium>0)", blindCondition.c_str()), 16, 100., 180., "mgg_CSVM", "Diphoton Mass", "GeV");

  db->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept",  Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1.)", blindCondition.c_str()), 10, 0., 1.0001, "thqLD_lept_CSV0", "thqLD" );
  db->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept",  Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && nbjets_loose>0)", blindCondition.c_str()), 10, 0., 1.0001, "thqLD_lept_CSVL", "thqLD" );
  db->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept",  Form("dbWeight*( category==11 && %s && ph1_pt > 50.*PhotonsMass/120. && leptPt>10. && abs(qJetEta)>1. && nbjets_medium>0)", blindCondition.c_str()), 10, 0., 1.0001, "thqLD_lept_CSVM", "thqLD" );

  std::vector< std::pair<std::string,std::string> > cuts;
  cuts.push_back( std::pair<std::string,std::string>( blindCondition, "2#gamma + 2j") );
  cuts.push_back( std::pair<std::string,std::string>( "leptPt>10.", "Lepton") );
  cuts.push_back( std::pair<std::string,std::string>( "isMu || abs(m_lept_phot2-91.19)>10.", "|M(e#gamma)-M(Z)|>10") );
  cuts.push_back( std::pair<std::string,std::string>( "ph1_pt > 50.*PhotonsMass/120.", "p_{T1} > 50 m_{#gamma#gamma}/120") );
  cuts.push_back( std::pair<std::string,std::string>( "nbjets_loose>0", "CSVL") );
  cuts.push_back( std::pair<std::string,std::string>( "nbjets_medium>0", "CSVM") );
  //cuts.push_back( std::pair<std::string,std::string>( "nbjets_medium>0 && nbjets_loose>1", "CSVL + CSVM") );
  cuts.push_back( std::pair<std::string,std::string>( "abs(qJetEta)>1.", "|#eta(qJet)|>1") );
  cuts.push_back( std::pair<std::string,std::string>( "thqLD_lept>0.25", "LD>0.25") );
  //cuts.push_back( std::pair<std::string,std::string>( "thqLD_lept<0.25", "LD<0.25") );
  
  
  drawYieldsCascade( db, cuts );


  std::vector< std::pair<std::string,std::string> > cuts2;
  cuts2.push_back( std::pair<std::string,std::string>( "ph1_pt > 50.*PhotonsMass/120.", "2#gamma + 2j") );
  cuts2.push_back( std::pair<std::string,std::string>( "nbjets_loose>0", "CSVL") );
  cuts2.push_back( std::pair<std::string,std::string>( "nbjets_medium>0", "CSVM") );
  comparePhotonsMass( db, cuts2 );

  return 0;

}



void drawYieldsCascade( DrawBase* db, std::vector< std::pair<std::string,std::string> > cuts ) {


  db->set_legendBox_xMin(0.62);

  int nBins = cuts.size();
//  TH2D* h2_yieldsCascade = new TH2D("yields_cascade", "", nBins, 0., nBins, 10, 0., 30000. );
  TH2D* h2_yieldsCascade = new TH2D("yields_cascade", "", nBins, 0., nBins, 10, 0.1, 1000010. );

  std::string selection = "dbWeight*( category==11 ";

  for( unsigned ibin=0; ibin<nBins; ++ibin )
    h2_yieldsCascade->GetXaxis()->SetBinLabel(ibin+1, cuts[ibin].second.c_str());

  TCanvas* cc = new TCanvas("cc", "", 600, 600);
  cc->cd();
  cc->SetLogy();
  
  h2_yieldsCascade->SetYTitle( "Events" );
  h2_yieldsCascade->Draw("");


  TLegend* legend;
  float   previousYield_mc = 0.;
  float       thisYield_mc = 0.;
  float previousYield_data = 0.;
  float     thisYield_data = 0.;

  for( unsigned ibin=0; ibin<nBins; ++ibin ) {

    selection = selection + " && (" + cuts[ibin].first + ")";
    std::string this_selection = selection + " )";
    db->drawHisto_fromTree( "tree_passedEvents", Form("thqLD_lept + %f", (float)ibin), this_selection, 1, ibin, ibin+1., Form("tobedeleted%d", ibin), "", "", "Events", true );
    legend = (TLegend*)gROOT->FindObject("dbLegend");
    TGraphAsymmErrors* gr_data = new TGraphAsymmErrors(*(db->get_lastHistos_dataGraph()));
    gr_data->SetName(Form("gr_%d", ibin));
    THStack* mcStack = new THStack(*(db->get_lastHistos_mcStack()));
    mcStack->SetName(Form("stack_%d", ibin));
    std::vector<TH1D*> mcHistos = db->get_lastHistos_mc();
    std::vector<TH1D*> dataHistos = db->get_lastHistos_data();
    
    previousYield_mc = thisYield_mc;
    thisYield_mc=0.;
    float ttggYield = 0.;
    for( unsigned imc=0; imc<mcHistos.size(); ++imc ) {
      if( db->get_mcFile(imc).datasetName == "ttgg" ) ttggYield = mcHistos[imc]->Integral();
      thisYield_mc += mcHistos[imc]->Integral();
    }
    float otherMcYield = thisYield_mc-ttggYield;

    previousYield_data = thisYield_data;
    thisYield_data = dataHistos[0]->Integral();

    float data_corr = thisYield_data-otherMcYield;
    float ttggSF = data_corr/ttggYield;

    std::cout << "Cut: " << cuts[ibin].second << std::endl;
    std::cout << "Data: " << thisYield_data << std::endl;
    std::cout << "MC: " << thisYield_mc << std::endl;
    std::cout << "Data/MC: " << thisYield_data/thisYield_mc << std::endl;
    std::cout << "ttgg SF: " << ttggSF << std::endl;
    if( ibin>0 ) {
      std::cout << "Efficiency (mc): " << thisYield_mc/previousYield_mc << std::endl;
      std::cout << "Efficiency (data): " << thisYield_data/previousYield_data << std::endl;
    }

    cc->cd();
    if( ibin==0 ) {
      legend->Draw("same");
    }
    mcStack->Draw("E same");
    mcStack->Draw("h same");
    gr_data->Draw("p same");

  }

  TPaveText* labelTop = db->get_labelTop();
  labelTop->Draw("same");

  gPad->RedrawAxis();
  cc->SaveAs(Form("%s/yieldsCascade.eps", db->get_outputdir().c_str()));
  cc->SaveAs(Form("%s/yieldsCascade.png", db->get_outputdir().c_str()));

  std::string rmCommand = "rm " + db->get_outputdir() + "/tobedeleted*";
  system( rmCommand.c_str() );

}



void comparePhotonsMass( DrawBase* db, std::vector< std::pair<std::string,std::string> > cuts ) {


  int nBins = 16;
  float xMin = 100.;
  float xMax = 180.;

  TH2D* h2_axes = new TH2D("axes", "", nBins, xMin, xMax, 10, 0., 0.16 );
  h2_axes->SetYTitle( "Normalized to Unity" );
  h2_axes->SetXTitle( "Diphoton Invariant Mass [GeV]" );

  std::string selection = "dbWeight*( category==11 ";


  TCanvas* cmc = new TCanvas("cmc", "", 600, 600);
  cmc->cd();
  
  h2_axes->Draw("");


  TCanvas* cdata = new TCanvas("cdata", "", 600, 600);
  cdata->cd();
  
  h2_axes->Draw("");


  int iColor = 1;

  TLegend* legend = new TLegend( 0.6, 0.7, 0.9, 0.9 );
  legend->SetTextSize(0.035);
  legend->SetFillColor(kWhite);

  for( unsigned ibin=0; ibin<cuts.size(); ++ibin ) {

    selection = selection + " && (" + cuts[ibin].first + ")";
    std::string this_selection = selection + " )";
    db->drawHisto_fromTree( "tree_passedEvents", Form("PhotonsMass", (float)ibin), this_selection, nBins, xMin, xMax, Form("tobedeleted2%d", ibin), "", "", "Events", true );

    std::vector<TH1D*> mcHistos = db->get_lastHistos_mc();
    std::vector<TH1D*> dataHistos = db->get_lastHistos_data();

    TH1D* h1_mgg = new TH1D(*(mcHistos[0]));
    h1_mgg->Sumw2();
    for( unsigned iHisto=1; iHisto<mcHistos.size(); ++iHisto ) 
      h1_mgg->Add( mcHistos[iHisto] );


    float sf_mc = 1./h1_mgg->Integral();
    float sf_data = 1./dataHistos[0]->Integral();
    sf_data *= 3./4.;
    
    h1_mgg->Scale( sf_mc );
    for( unsigned i=0; i<dataHistos[0]->GetNbinsX(); ++i )
      dataHistos[0]->SetBinError( i+1, sqrt(dataHistos[0]->GetBinError(i+1)) );
      //dataHistos[0]->SetBinError( i+1, dataHistos[0]->GetBinError(i+1)*sf_data );
    dataHistos[0]->Scale( sf_data );


    //if( ibin==0 ) {
    //  h1_reference = new TH1D(*h1_mgg);
    //} else {
    //  TH1D* h1_ratio = new

    h1_mgg->SetLineColor( iColor );
    h1_mgg->SetLineWidth( 2 );
    
    dataHistos[0]->SetMarkerSize(0);
    dataHistos[0]->SetMarkerStyle();
    dataHistos[0]->SetLineColor( iColor );
    dataHistos[0]->SetLineWidth( 2 );

    iColor+=1;
    
    cmc->cd();
    h1_mgg->Draw("e same");

    cdata->cd();
    dataHistos[0]->Draw("e same");

    legend->AddEntry( h1_mgg, cuts[ibin].second.c_str(), "L" );

    //delete h1_mgg;

  }

  cmc->cd();
  legend->Draw("same");

  cdata->cd();
  legend->Draw("same");

  TPaveText* labelTop = db->get_labelTop();
  cmc->cd();
  labelTop->Draw("same");
  gPad->RedrawAxis();
  cdata->cd();
  labelTop->Draw("same");
  gPad->RedrawAxis();

  cmc->SaveAs(Form("%s/mggShapeCheck_mc.eps", db->get_outputdir().c_str()));
  cmc->SaveAs(Form("%s/mggShapeCheck_mc.png", db->get_outputdir().c_str()));

  cdata->SaveAs(Form("%s/mggShapeCheck_data.eps", db->get_outputdir().c_str()));
  cdata->SaveAs(Form("%s/mggShapeCheck_data.png", db->get_outputdir().c_str()));

  std::string rmCommand = "rm " + db->get_outputdir() + "/tobedeleted*";
  system( rmCommand.c_str() );

}
