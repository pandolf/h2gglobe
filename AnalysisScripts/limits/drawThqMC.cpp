#include <iostream>

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"





int main( int argc, char* argv[] ) {


  std::string batchProd = "qJetEtaFix_newLD_v1";

  
  DrawBase* db = new DrawBase("THq");
  DrawBase* db_nostack = new DrawBase("THq_nostack");


  std::string outputdir_str = "THq_plots_"+batchProd;
  db->set_outputdir( outputdir_str );
  db_nostack->set_outputdir( outputdir_str );


  std::string inputDir = "dbFiles_"+batchProd+"/";


  std::string thqLeptonicFileName = inputDir + "thqLeptonic_m125_8TeV.root";
  TFile* file_thq = TFile::Open( thqLeptonicFileName.c_str() );
  db->add_mcFile( file_thq, "thq", "tHq (C_{t}=-1)", kWhite );
  db_nostack->add_mcFile( file_thq, "thq", "tHq (C_{t}=-1)", kBlack );

  std::string tthFileName = inputDir + "tth_m125_8TeV.root";
  TFile* file_tth = TFile::Open( tthFileName.c_str() );
  db->add_mcFile( file_tth, "tth", "ttH (125)", 46 );
  db_nostack->add_mcFile( file_tth, "tth", "ttH (125)", 46 );

  std::string wzhFileName = inputDir + "wzh_m125_8TeV.root";
  TFile* file_wzh = TFile::Open( wzhFileName.c_str() );
  db->add_mcFile( file_wzh, "wzh", "VH (125)", 28 );

  //std::string diphojet_sherpa_8TeVFileName = inputDir + "diphojet_sherpa_8TeV_m125_8TeV.root";
  //TFile* file_diphojet_sherpa_8TeV = TFile::Open( diphojet_sherpa_8TeVFileName.c_str() );
  //db->add_mcFile( file_diphojet_sherpa_8TeV, "diphojet_sherpa_8TeV", "Diphoton", 39 );

  //std::string gjet_40_8TeV_pfFileName = inputDir + "gjet_40_8TeV_pf_m125_8TeV.root";
  //TFile* file_gjet_40_8TeV_pf = TFile::Open( gjet_40_8TeV_pfFileName.c_str() );
  //db->add_mcFile( file_gjet_40_8TeV_pf, "gjet_40_8TeV_pf", "#gamma + Jet", kMagenta );

  std::string tggFileName = inputDir + "tgg.root";
  TFile* file_tgg = TFile::Open( tggFileName.c_str() );
  db->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 29 );
  db_nostack->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 29 );

  std::string ttggFileName = inputDir + "ttgg.root";
  TFile* file_ttgg = TFile::Open( ttggFileName.c_str() );
  db->add_mcFile( file_ttgg, "ttgg", "tt#gamma#gamma", 38 );


  // first some shape norm plots:
  db_nostack->set_shapeNormalization();

  db_nostack->drawHisto_fromTree( "tree_passedEvents", "nvtx", "weight*( category==11 )", 40, 0.5, 40.5, "npu", "Number of Reconstructed Vertexes" );
  db_nostack->drawHisto_fromTree( "tree_passedEvents", "rho", "weight*( category==11 )", 30, 0., 50., "rho", "Particle Flow Energy Density (#rho)", "GeV" );



  // then some lumi norm plots:
  float lumi = 19715.;
  db->set_lumiNormalization(lumi);

  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("weight*( category==11 )/%f", lumi), 40, 100., 180., "mgg", "Diphoton Mass", "GeV", "Events", true );
  db->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept", Form("weight*( category==11 )/%f", lumi), 40, 0., 1.0001, "thqLD_lept", "tHq Leptonic LD", "", "Events", true );

  return 0;

}


