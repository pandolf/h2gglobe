{
  TFile *_file0 = TFile::Open("/afs/cern.ch/user/m/micheli/scratch1/public/4Markus/CMS-HGG_sigdata_interpolated_interpolated.root");
  float nominal=th1f_sig_tth_mass_m125_cat6->Integral(45,57);
  cout<<"nominal value in 122-128:"<<  nominal<<endl;
  cout<<"-----------systematics---------------------"<<endl;
  //Eres
  float Eresup=th1f_sig_tth_mass_m125_cat6_E_resUp01_sigma->Integral(45,57);
  float Eresdown=th1f_sig_tth_mass_m125_cat6_E_resDown01_sigma->Integral(45,57);
  cout<<"Eres Up in 122-128: "<<  Eresup<<endl;
  cout<<"Eres Down in 122-128: "<<  Eresdown<<endl;
  cout<<"E_res "<<1-0.3333*fabs(nominal-Eresdown)/nominal<<"/"<<1+0.3333*fabs(nominal-Eresup)/nominal<<endl;
  
  //E_scale
  float E_scaleup=th1f_sig_tth_mass_m125_cat6_E_scaleUp01_sigma->Integral(45,57);
  float E_scaledown=th1f_sig_tth_mass_m125_cat6_E_scaleDown01_sigma->Integral(45,57);
  cout<<"E_scale Up in 122-128: "<<  E_scaleup<<endl;
  cout<<"E_scale Down in 122-128: "<<  E_scaledown<<endl;
  cout<<"E_scale "<<1-0.3333*fabs(nominal-E_scaledown)/nominal<<"/"<<1+0.3333*fabs(nominal-E_scaleup)/nominal<<endl;
  //idEff
  float idEffup=th1f_sig_tth_mass_m125_cat6_idEffUp01_sigma->Integral(45,57);
  float idEffdown=th1f_sig_tth_mass_m125_cat6_idEffDown01_sigma->Integral(45,57);
  cout<<"idEff Up in 122-128: "<<  idEffup<<endl;
  cout<<"idEff Down in 122-128: "<<  idEffdown<<endl;
  cout<<"idEff "<<1-0.3333*fabs(nominal-idEffdown)/nominal<<"/"<<1+0.3333*fabs(nominal-idEffup)/nominal<<endl;
  //triggerEff
  float triggerEffup=th1f_sig_tth_mass_m125_cat6_triggerEffUp01_sigma->Integral(45,57);
  float triggerEffdown=th1f_sig_tth_mass_m125_cat6_triggerEffDown01_sigma->Integral(45,57);
  cout<<"triggerEff Up in 122-128: "<<  triggerEffup<<endl;
  cout<<"triggerEff Down in 122-128: "<<  triggerEffdown<<endl;
  cout<<"triggerEff "<<1-0.3333*fabs(nominal-triggerEffdown)/nominal<<"/"<<1+0.3333*fabs(nominal-triggerEffup)/nominal<<endl;
  //vtxEff
  float vtxEffup=th1f_sig_tth_mass_m125_cat6_vtxEffUp01_sigma->Integral(45,57);
  float vtxEffdown=th1f_sig_tth_mass_m125_cat6_vtxEffDown01_sigma->Integral(45,57);
  cout<<"vtxEff Up in 122-128: "<<  vtxEffup<<endl;
  cout<<"vtxEff Down in 122-128: "<<  vtxEffdown<<endl;
  cout<<"vtxEff "<<1-0.3333*fabs(nominal-vtxEffdown)/nominal<<"/"<<1+0.3333*fabs(nominal-vtxEffup)/nominal<<endl;


}
