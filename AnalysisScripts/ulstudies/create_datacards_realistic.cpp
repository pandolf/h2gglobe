#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"





int main() {


  float massWindow = 0.5;

  for( ; massWindow<8; massWindow+=0.5 ) {

    char yieldsFileName_char[1000];
    if( massWindow<1. )
      sprintf( yieldsFileName_char, "../THq_plots/THq_postSync/THqPlots_MConly_sel4_CSV_CtMinusOne/yields_lept_LD_0%.0f.txt", 10.*massWindow );
    else
      sprintf( yieldsFileName_char, "../THq_plots/THq_postSync/THqPlots_MConly_sel4_CSV_CtMinusOne/yields_lept_LD_%.0f.txt", 10.*massWindow );

    std::ifstream yieldsFile(yieldsFileName_char);

    float signal;
    float bg_SMH;

    bool found_signal=false;
    bool found_bgSMH=false;

    while( yieldsFile.good() && !yieldsFile.eof() ) {

      std::string thisline;
      getline( yieldsFile, thisline );
      TString thisline_tstr(thisline);
        
      if( thisline_tstr.BeginsWith("Total signal: ") ) {
        float signal2;
        sscanf( thisline.c_str(), "Total signal: %f  w/o SMH: %f", &signal, &signal2 );
        found_signal = true;
      } else if( thisline_tstr.BeginsWith("Total SMH BG under peak:") ) {
        sscanf( thisline.c_str(), "Total SMH BG under peak: %f", &bg_SMH );
        found_bgSMH = true;
      } 

    } //while yields file

    if( !found_signal ) {
      std::cout << "Didn't find signal in file: " << yieldsFileName_char << std::endl;
      exit(33);
    }
    if( !found_bgSMH ) {
      std::cout << "Didn't find bgSMH in file: " << yieldsFileName_char << std::endl;
      exit(35);
    }


    float k_BG = massWindow*2./70.;


    char datacardName[300];
    if( massWindow<1. )
      sprintf(datacardName, "datacard_realistic_0%.0fGeV.txt", massWindow*10.);
    else 
      sprintf(datacardName, "datacard_realistic_%.0fGeV.txt", massWindow*10.);
    std::ofstream datacard(datacardName);

   
    datacard << "imax 1" << std::endl;
    datacard << "jmax 2 " << std::endl;
    datacard << "kmax *" << std::endl;
    datacard << "    " << std::endl;
    datacard << "    " << std::endl;
    datacard << "bin  1" << std::endl;
    datacard << "observation 0" << std::endl;
    datacard << "" << std::endl;
    datacard << "    " << std::endl;
    datacard << "bin            1        1       1"       << std::endl;
    datacard << "process        s        b       bSMH"    << std::endl;
    datacard << "process        0        1       2"       << std::endl;
    datacard << "rate   " << signal <<  "  0. "   <<  bg_SMH   << std::endl;
    datacard << "      " << std::endl;
    datacard << "      " << std::endl;
    datacard << "#syst " << std::endl;
    datacard << "bg_lept  gmN 0    - " << k_BG << "   -" << std::endl;
    datacard << "syst_smh   lnN    -     -         1.2" << std::endl;

    datacard.close();

  } //for mass windows


  return 0;

}
