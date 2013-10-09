#include <iostream>

#include "TF1.h"


float getScalingFactor(TF1* f1, float deltaM);

int main() {


  TF1* flat = new TF1("uniform", "[0]", 10.0, 180.);
  flat->SetParameter(0, 1./80.);

  TF1* expo = new TF1("expo", "exp([0]+[1]*x)", 100., 180.);
  expo->SetParameter(0, 8. );
  expo->SetParameter(1, -0.1 );

  float sf_flat = getScalingFactor(flat, 3.);
  float sf_expo = getScalingFactor(expo, 3.);

  std::cout << "SF: " << std::endl;
  std::cout << "flat: " << sf_flat << std::endl;
  std::cout << "expo: " << sf_expo << std::endl;

  return 0;

}


float getScalingFactor(TF1* f1, float deltaM) {

  float s1 = f1->Integral( 100., 125.-deltaM );
  float s2 = f1->Integral( 125.+deltaM, 180.);

  float sR = f1->Integral( 125.-deltaM, 125.+deltaM);

  float alpha = sR/(s1+s2);

  return alpha;

}
