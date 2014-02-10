#include <fstream>
#include <iostream>
#include <cstdlib>

#include "TString.h"
#include "TFile.h"
#include "TTree.h"
#include "TH1D.h"
#include "THStack.h"
#include "TCanvas.h"
#include "TH2D.h"

#include "RooDataSet.h"
#include "RooExponential.h"
#include "RooRealVar.h"
#include "RooPlot.h"


#include "CommonTools/DrawBase.h"
#include "CommonTools/fitTools.h"






int main() {

  DrawBase* db = new DrawBase("ll");

  TFile* file = TFile::Open("/afs/cern.ch/work/p/pandolf/public/histograms_CMS-HGG.root");
  TTree* tree = (TTree*)file->Get("Data");



  float mgg;
  tree->SetBranchAddress( "PhotonsMass", &mgg );
  int category;
  tree->SetBranchAddress( "category", &category );
  float weight;
  tree->SetBranchAddress( "weight", &weight );
  float evweight;
  tree->SetBranchAddress( "evweight", &evweight );
  float thqLD;
  tree->SetBranchAddress( "thqLD_lept", &thqLD );
std::cout << "xxx2" << std::endl;

  RooRealVar mgg_roo("mgg_roo", "mgg_roo",100.,100.,180.);
  RooArgSet aset(mgg_roo, "argset");
  RooDataSet* dataset = new RooDataSet("dataset", "dataset", aset);

  int nentries = tree->GetEntries();

std::cout << "xxx3" << std::endl;
  for( unsigned iEntry=0; iEntry<nentries; ++iEntry ) {

    tree->GetEntry(iEntry);
    if( iEntry%10000 == 0 ) std::cout << "Entry: " << iEntry << " / " << nentries << std::endl;

    if( category!=11 ) continue;
    if( mgg<100. || mgg>180. ) continue;
    if( mgg>115. && mgg<135. ) continue;
    if( thqLD<0.25 ) continue;

    //argset.setRealValue("mgg", mgg);
    //argset.setRealValue("weight", weight);
    mgg_roo.setVal(mgg);
    aset.setRealValue("mgg_roo", mgg);
    //aset.setRealValue("mgg_roo", weight);

std::cout << "zzz1" << std::endl;
    dataset->add(aset);
std::cout << "zzz2" << std::endl;

  }

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  c1->cd();

  TH2D* h2_axes = new TH2D( "axes", "", 10, 100., 180., 10, 0., 0.06);
  h2_axes->SetXTitle( "Diphoton Mass [GeV]");
  h2_axes->SetYTitle( "Normalized to Unity");
  h2_axes->Draw();


  float xMin = mgg_roo.getMin();
  float xMax = mgg_roo.getMax();

  float p0_init=-3.E-2;
  float p0_min =-1.;
  float p0_max = 0.;

  // define exponential
  RooRealVar p0("p0_name", "p0_name",p0_init, p0_min, p0_max);
  RooExponential* exp = new RooExponential("name","exp",mgg_roo,p0);

  exp->fitTo(*dataset, RooFit::Range(xMin,xMax));


  RooPlot* xframe = mgg_roo.frame(xMin, xMax, 50) ;
  //xframe->Draw("");
  exp   ->plotOn( xframe, RooFit::LineColor(kRed+1), RooFit::LineStyle(2) );
  xframe->Draw("Same");

  c1->SaveAs("prova.eps");

  return 0;

}


//std::cout << "x1" << std::endl;
//
//  double xInit = 125.;
//  double xMin = 100.;
//  double xMax = 180. ;
//  
//  //stringstream weight;
//  //weight << "weight";
//  RooRealVar x("mgg","M_{#gamma#gamma}", 100., 100., 180.,"GeV");
//  //RooRealVar w("weight", "weight", 1.0, -1000., 1000.);
//
//  //RooArgSet varset(x,w,"argset_obs");
//  //RooDataSet dataset("dataset","dataset",varset,WeightVar(weight.str().c_str()));
//
//  float p0_init=-3.E-2;
//  float p0_min =-1.;
//  float p0_max = 0.;
//
//  // define exponential
//  RooRealVar p0("p0","p0",p0_init, p0_min, p0_max);
//  RooExponential* exp = new RooExponential(name.c_str(),"exp",x,p0);
//std::cout << "x2" << std::endl;
//
//  exp->fitTo(*dataset);
//  //exp.fitTo(dataset,SumW2Error(1),Range(xMin,xMax),Strategy(2),NumCPU(8));
//
//  std::cout << "found p0: " << std::endl;
//  p0.Print();
//
//  return exp;
//
//}
//
//
//
//
//  return dataset;
//
//}
//
