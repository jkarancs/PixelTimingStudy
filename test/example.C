//#define COMPLETE 0
#define SPLIT 1

void example() {

  gROOT->SetStyle("Plain");
  gStyle->SetOptStat(0);

  //ah.rocmap         ->add(p.det,    1,  4, 4,  3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5,  42, -10.5,  10.5);
  //ah.rocmap         ->add(p.det,    1,  5, 5,  3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5,  66, -16.5,  16.5);
  //ah.rocmap         ->add(p.det,    1,  6, 6,  3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5,  90, -22.5,  22.5);
  //ah.rocmap         ->add(p.fpixIO, 1,  1, 1, -3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5, 144,   0.5,  12.5);
  //ah.rocmap         ->add(p.fpixIO, 1,  2, 2, -3, p.ineff, 1, 1, 3, 0, p.inac, 1, 1, 3,  72, -4.5, 4.5, 144, -12.5,  -0.5);

  TH2D *l1_module_map_num = new TH2D("l1_module_map_num","Module Efficiency Map - Layer 1;Module;Ladder;Hit Finding Efficiency", 9,-4.5,4.5, 21,-10.5,10.5);
  TH2D *l1_module_map_den = new TH2D("l1_module_map_den","Module Efficiency Map - Layer 1;Module;Ladder;Hit Finding Efficiency", 9,-4.5,4.5, 21,-10.5,10.5);
  
  trajTree->Draw("ladder:module>>l1_module_map_num","layer==1&&pass_effcuts&&validhit");
  trajTree->Draw("ladder:module>>l1_module_map_den","layer==1&&pass_effcuts");

  
  TCanvas* l1_modmap = new TCanvas("l1_modmap","Module Efficiency Map - Layer 1");
  l1_module_map_num->Divide(l1_module_map_den);
  l1_module_map_num->GetZaxis()->SetRangeUser(0.9,1.0);
  l1_module_map_num->Draw("COLZ");
  
  //if (SPLIT>0) {  }

}
