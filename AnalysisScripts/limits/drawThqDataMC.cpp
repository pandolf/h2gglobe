#include <iostream>
#include <fstream>

#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"



bool BLINDED=true;



//void printYields( DrawBase* db, const std::string& suffix, float massWindow=3. );



int main( int argc, char* argv[] ) {


  std::string batchProd = "preNatal_v5";
  if( argc>1 ) {
    std::string batchProd_str(argv[1]);
    batchProd = batchProd_str;
  }

  
  DrawBase* db = new DrawBase("THq");
  DrawBase* db_inclusiveData = new DrawBase("THq_inclusive");
  DrawBase* db_csData = new DrawBase("THq_CS");
  DrawBase* db_nostack = new DrawBase("THq_nostack");

  //db->set_isCMSArticle(true);
  //db_inclusiveData->set_isCMSArticle(true);

  std::string outputdir_str = "THq_plots_dataMC_"+batchProd;
  db->set_outputdir( outputdir_str );
  db_csData->set_outputdir( outputdir_str );
  db_inclusiveData->set_outputdir( outputdir_str );
  db_nostack->set_outputdir( outputdir_str );


  std::string inputDir = "dbFiles_"+batchProd+"/";


  // add categories 11-12 data for final plots:
  std::string dataFileName = inputDir + "data.root";
  TFile* file_data = TFile::Open( dataFileName.c_str() );
  std::string dataName = (BLINDED) ? "Blinded Data" : "Data";
  db->add_dataFile( file_data, "data", dataName );
  db_nostack->add_dataFile( file_data, "data", dataName );

  // use inclusive categories to check PU and shit:
  std::string dataInclusiveFileName = inputDir + "data_inclusive.root";
  TFile* file_dataInclusive = TFile::Open( dataInclusiveFileName.c_str() );
  db_inclusiveData->add_dataFile( file_dataInclusive, "data", "Data (inclusive)" );

  // btagZero control sample data:
  std::string dataCSFileName = inputDir + "dataCS_btagZero.root";
  TFile* file_dataCS = TFile::Open( dataCSFileName.c_str() );
  db_csData->add_dataFile( file_dataCS, "data", "Data (CS)" );



  // MC files:

  std::string thqLeptonicFileName = inputDir + "thqLeptonic_m125_8TeV.root";
  TFile* file_thq = TFile::Open( thqLeptonicFileName.c_str() );
  db->add_mcFile( file_thq, "thq", "tHq (Ct = -1)", kWhite );
  db_inclusiveData->add_mcFile( file_thq, "thq", "tHq (Ct = -1)", kBlack );
  db_nostack->add_mcFile( file_thq, "thq", "tHq (125)", 38 );


  std::string ttH2FileName = inputDir + "tth_m125_8TeV.root";
  TFile* file_ttH2 = TFile::Open( ttH2FileName.c_str() );
  db->add_mcFile( file_ttH2, "ttH2", "Extra ttH (Ct = -1)", 46, 3004 );
  db->set_mcWeight( "ttH2", 1.4 ); //extra contribution for Ct=-1


  std::string tthFileName = inputDir + "tth_m125_8TeV.root";
  TFile* file_tth = TFile::Open( tthFileName.c_str() );
  db->add_mcFile( file_tth, "tth", "ttH (125)", 46 );
  db_nostack->add_mcFile( file_tth, "tth", "ttH (125)", 46 );
  db_inclusiveData->add_mcFile( file_tth, "tth", "ttH (125)", 46 );


  std::string wzhFileName = inputDir + "wzh_m125_8TeV.root";
  TFile* file_wzh = TFile::Open( wzhFileName.c_str() );
  db->add_mcFile( file_wzh, "wzh", "VH (125)", 28 );
  db->set_mcWeight( "wzh", 2.4 ); //extra contribution for Ct=-1

  std::string diphojet_sherpa_8TeVFileName = inputDir + "diphojet_sherpa_8TeV.root";
  TFile* file_diphojet_sherpa_8TeV = TFile::Open( diphojet_sherpa_8TeVFileName.c_str() );
  //db->add_mcFile( file_diphojet_sherpa_8TeV, "diphojet_sherpa_8TeV", "Diphoton", 39 );

  std::string diphojet_sherpa_8TeVFileName_CS = inputDir + "diphojet_sherpa_8TeVCS_btagZero.root";
  TFile* file_diphojet_sherpa_8TeV_CS = TFile::Open( diphojet_sherpa_8TeVFileName_CS.c_str() );
  db_csData->add_mcFile( file_diphojet_sherpa_8TeV_CS, "diphojet_sherpa_8TeV_CS", "Diphoton (CS)", 39 );

  //std::string gjet_40_8TeV_pfFileName = inputDir + "gjet_40_8TeV_pf_m125_8TeV.root";
  //TFile* file_gjet_40_8TeV_pf = TFile::Open( gjet_40_8TeV_pfFileName.c_str() );
  //db->add_mcFile( file_gjet_40_8TeV_pf, "gjet_40_8TeV_pf", "#gamma + Jet", kMagenta );

  std::string tggFileName = inputDir + "tgg.root";
  TFile* file_tgg = TFile::Open( tggFileName.c_str() );
  //db->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 29 );
  //db_inclusiveData->add_mcFile( file_tgg, "tgg", "t#gamma#gamma", 29 );

  //std::string ttggFileName = inputDir + "ttgg.root";
  //TFile* file_ttgg = TFile::Open( ttggFileName.c_str() );
  //db->add_mcFile( file_ttgg, "ttgg", "tt#gamma#gamma", 38 );


  // first some shape norm plots:
  float lumi = 19715.;
  db_inclusiveData->set_lumi(lumi);
  db_inclusiveData->set_shapeNormalization();
  db_inclusiveData->set_noStack();

  db_nostack->set_lumi(lumi);
  db_nostack->set_shapeNormalization();
  db_nostack->set_noStack();
  db_nostack->set_drawZeros(false);

  db_csData->set_lumi(lumi);
  db_csData->set_shapeNormalization();
  db_csData->set_noStack();
  db_csData->set_drawZeros(false);

  db->set_lumiNormalization(lumi);
  db->set_drawZeros(false);

  db_inclusiveData->drawHisto_fromTree( "tree_passedEvents", "nvtx", "weight*( category==11 || itype==0 )", 40, 0.5, 40.5, "nvtx_inclusive", "Number of Reconstructed Vertexes" );
  db_nostack      ->drawHisto_fromTree( "tree_passedEvents", "nvtx", "weight*( category==11             )", 40, 0.5, 40.5, "nvtx", "Number of Reconstructed Vertexes" );
  db_inclusiveData->drawHisto_fromTree( "tree_passedEvents", "rho",  "weight*( category==11 || itype==0)", 30, 0., 50., "rho_inclusive", "Particle Flow Energy Density (#rho)", "GeV" );

  db_nostack      ->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept", "dbWeight*( category==11 )", 50, 0., 1.0001, "thqLD_lept_shape", "tHq Leptonic LD");


  // then some lumi norm plots:

  db->set_yAxisMax(3.5);
  db       ->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept", Form("dbWeight*( category==11 )"), 40, 0., 1.0001, "thqLD_lept",    "tHq Leptonic LD", "", "Events", true );
  db_csData->drawHisto_fromTree( "tree_passedEvents", "thqLD_lept", Form("dbWeight*( category==11 )"), 40, 0., 1.0001, "thqLD_lept_CS", "tHq Leptonic LD", "", "Events", true );

  float massWindow = 3.;
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 )"), 40, 100., 180., "mgg", "Diphoton Mass", "GeV", "Events", true );
  db->set_yAxisMax();
  db->drawHisto_fromTree( "tree_passedEvents", "PhotonsMass", Form("dbWeight*( category==11 && thqLD_lept>0.25 )"), 40, 100., 180., "mgg_LDcut", "Diphoton Mass", "GeV", "Events", true );

  return 0;

}



/*
void printYields( DrawBase* db, const std::string& suffix, float massWindow ) {


  TH1F::AddDirectory(kTRUE);

  float xMin = 125.-massWindow;
  float xMax = 125.+massWindow;


  char yieldsFileName[1000];
  sprintf( yieldsFileName, "%s/yields_%s.txt", db->get_outputdir().c_str(), suffix.c_str() );
  //if( massWindow<1. )
  //  sprintf( yieldsFileName, "%s/yields_%s_0%.0f.txt", db->get_outputdir().c_str(), suffix.c_str(), massWindow*10. );
  //else
  //  sprintf( yieldsFileName, "%s/yields_%s_%.0f.txt", db->get_outputdir().c_str(), suffix.c_str(), massWindow*10. );
  ofstream yieldsFile(yieldsFileName);


  std::vector< InputFile > mcFiles = db->get_mcFiles();

  float massRange_min = 100.;
  float massRange_max = 180.;
  float massRange = massRange_max-massRange_min;

  char selection_total[1000];
  sprintf( selection_total, "weight*( category==11 && thqLD_lept>0.25 && PhotonsMass>100. && PhotonsMass<180. )" );
  char selection_massWindow[1000];
  sprintf( selection_massWindow, "weight*( category==11 && thqLD_lept>0.25 && PhotonsMass>%f && PhotonsMass<%f )", xMin, xMax );

  bool foundSignal = false;
  float totalBG = 0.;
  float totalBG_ave = 0.;
  float totalBG_allSMH = 0.;
  float signal = 0.;
  float signal_noSMH = 0.;


  yieldsFile << std::endl << "Yields (@ 20 fb-1): " << std::endl;

  for( unsigned int ii=0; ii<mcFiles.size(); ++ii ) {

    std::string dataset = db->get_mcFile(ii).datasetName;
    if( dataset=="ttH2" ) continue;

    TH1D* h1_mgg_massWindow = new TH1D("mgg_massWindow", "", 160, 100., 180.);
    h1_mgg_massWindow->Sumw2();
    TH1D* h1_mgg_total = new TH1D("mgg_total", "", 160, 100., 180.);
    h1_mgg_total->Sumw2();

    TFile* thisFile = db->get_mcFile(ii).file;
    TTree* thisTree = (TTree*)thisFile->Get("tree_passedEvents");

    thisTree->Project( "mgg_massWindow", "PhotonsMass", selection_massWindow );
    thisTree->Project( "mgg_total", "PhotonsMass", selection_total );

    float integral_massWindow = h1_mgg_massWindow->Integral();
    float integral_total = h1_mgg_total->Integral();


    bool isSMH = (dataset=="tth"||dataset=="wzh"||dataset=="ggh"||dataset=="vbfh");

    if( db->get_mcFile(ii).datasetName != "thq" ) {

      if( isSMH ) {
    
        // also SM H BR is enhanced by 2.4, but the enhancement over SM expectation is signal for us:
        float BG_SF = 1.;
        float S_SF = 1.4; //extra Higgs due to Ct=-1

        float BG = integral_massWindow*BG_SF;
        float s = integral_massWindow*S_SF;

        totalBG += BG;
        totalBG_ave += BG;
        totalBG_allSMH += BG;

        signal += s;

        yieldsFile << db->get_mcFile(ii).datasetName << " (SM) " << BG << std::endl;
        yieldsFile << db->get_mcFile(ii).datasetName << " (extra) " << s << std::endl;

      } else {

        totalBG += integral_massWindow;

      
        float SF = (xMax-xMin)/massRange;
        float BG_ave = integral_total*SF;
        totalBG_ave += BG_ave;
        yieldsFile << db->get_mcFile(ii).datasetName << " " << BG_ave << std::endl;

      }

    } else {

      foundSignal = true;
      float thq = integral_massWindow;
      signal += thq;
      signal_noSMH += thq;
      yieldsFile << db->get_mcFile(ii).datasetName << " " << thq << std::endl;
      //if( Ct_minus1 ) signal/=34.;

    }

    delete h1_mgg_total;
    delete h1_mgg_massWindow;

  } //for datasets

  if( !foundSignal ) std::cout << "WARNING!!! DIDN'T FIND SIGNAL tHq!" << std::endl;


  yieldsFile << "Total signal: " << signal << " w/o SMH: " << signal_noSMH << std::endl;
  //yieldsFile << "Total BG / GeV: " << totalBG_ave/(xMax-xMin) << std::endl;
  yieldsFile << "Total BG (averaged): " << totalBG_ave << std::endl;
  yieldsFile << "Total SMH BG under peak: " << totalBG_allSMH << std::endl;
  yieldsFile << "Total extra SMH under peak: " << signal-signal_noSMH << std::endl;

//float signal_xsec = 2.28E-03*(0.0152*34. + 1.4*(19.37 + 1.573 + 0.6966 + 0.3943 + 0.1302));
//float signal_xsec_noSMH = 2.28E-03*(0.0152*34.);
//float total_signal = signal_xsec*db->get_lumi();
//float total_signal_noSMH = signal_xsec_noSMH*db->get_lumi();
//float effS = signal/total_signal;
//float effS_noSMH = signal_noSMH/total_signal_noSMH;
//yieldsFile << "Signal efficiency: " << effS << " w/o SMH: " << effS_noSMH << std::endl;


  
  //if( doUL && foundSignal ) {

  //  float ul_bgave = CLA( db->get_lumi(), 0., effS, 0., totalBG_ave, 0. );
  //  float ul_bgave_noSMH = CLA( db->get_lumi(), 0., effS_noSMH, 0., totalBG_allSMH, 0. ); //fix this
  //  yieldsFile << std::endl << "No error on BG:" << std::endl;
  //  yieldsFile << "UL: " << ul_bgave << " all SMH in BG: " << ul_bgave_noSMH << std::endl;
  //  yieldsFile << "UL/SM: " << ul_bgave/signal_xsec << " all SMH in BG: " << ul_bgave_noSMH/signal_xsec_noSMH << std::endl;//fix this

  //  float ul_bgerrave = CLA( db->get_lumi(), 0., effS, 0., totalBG_ave, 0.30*totalBG_ave );
  //  float ul_bgerrave_noSMH = CLA( db->get_lumi(), 0., effS_noSMH, 0., totalBG_allSMH, 0.30*totalBG_allSMH );//fix this
  //  yieldsFile << std::endl << "30\% error on BG:" << std::endl;
  //  yieldsFile << "UL: " << ul_bgerrave << " all SMH in BG: " << ul_bgerrave_noSMH << std::endl;
  //  yieldsFile << "UL/SM: " << ul_bgerrave/signal_xsec << " all SMH in BG: " << ul_bgerrave_noSMH/signal_xsec_noSMH << std::endl;

  //}


  std::cout << "-> Saved yields in " << yieldsFileName << std::endl;

}


*/
