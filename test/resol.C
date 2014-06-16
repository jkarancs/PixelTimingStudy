// Taken from:
// Daniel Pitzl, Dec 2010
// fit Student's t + p0
// .x fittp0.C("h012")
//

// Student's t function:
Double_t tp0Fit( Double_t *x, Double_t *par5 ) {

  static int nn = 0;
  nn++;
  static double dx = 0.1;
  static double b1 = 0;
  if( nn == 1 ) b1 = x[0];
  if( nn == 2 ) dx = x[0] - b1;
  //
  //--  Mean and width:
  //
  double xm = par5[0];
  double t = ( x[0] - xm ) / par5[1];
  double tt = t*t;
  //
  //--  exponent:
  //
  double rn = par5[2];
  double xn = 0.5 * ( rn + 1.0 );
  //
  //--  Normalization needs Gamma function:
  //
  double pk = 0.0;

  if( rn > 0.0 ) {

    double pi = 3.14159265358979323846;
    double aa = dx / par5[1] / sqrt(rn*pi) * TMath::Gamma(xn) / TMath::Gamma(0.5*rn);

    pk = par5[3] * aa * exp( -xn * log( 1.0 + tt/rn ) );
  }

  return pk + par5[4];
}
//
//----------------------------------------------------------------------
//
float fittp0( TH1D *h ) {
  h->SetMarkerStyle(21);
  h->SetMarkerSize(0.8);
  h->SetStats(1);
  gStyle->SetOptFit(101);
  
  gROOT->ForceStyle();
  
  double dx = h->GetBinWidth(1);
  double nmax = h->GetBinContent(h->GetMaximumBin());
  double xmax = h->GetBinCenter(h->GetMaximumBin());
  double nn = 7*nmax;
  
  int nb = h->GetNbinsX();
  double n1 = h->GetBinContent(1);
  double n9 = h->GetBinContent(nb);
  double bg = 0.5*(n1+n9);

  double x1 = h->GetBinCenter(1);
  double x9 = h->GetBinCenter(nb);
  cout << h->GetName() << ": " << x1 << " - " << x9 << endl;

  // create a TF1 with the range from x1 to x9 and 5 parameters

  TF1 *tp0Fcn = new TF1( "tp0Fcn", tp0Fit, x1, x9, 5 );

  tp0Fcn->SetParName( 0, "mean" );
  tp0Fcn->SetParName( 1, "sigma" );
  tp0Fcn->SetParName( 2, "nu" );
  tp0Fcn->SetParName( 3, "area" );
  tp0Fcn->SetParName( 4, "BG" );

  tp0Fcn->SetNpx(500);
  tp0Fcn->SetLineWidth(4);
  tp0Fcn->SetLineColor(kMagenta);
  tp0Fcn->SetLineColor(kGreen);
  
  // set start values for some parameters:

  cout << h->GetName() << " " << dx << ", " << nn << ", " << xmax << endl;

  tp0Fcn->SetParameter( 0, xmax ); // peak position
  tp0Fcn->SetParameter( 1, 4*dx ); // width
  tp0Fcn->SetParameter( 2, 2.2 ); // nu
  tp0Fcn->SetParameter( 3, nn ); // N
  tp0Fcn->SetParameter( 4, bg );
  
  h->Fit( "tp0Fcn", "R", "ep" );
  // h->Fit("tp0Fcn","V+","ep");

  h->Draw("histepsame");  // data again on top

  std::cout<<"Sigma: "<<tp0Fcn->GetParameter(1)<<std::endl;
  return tp0Fcn->GetParameter(1);
}


void resol() {
  TChain* trajTree = new TChain("trajTree");
  trajTree->Add("/data/jkarancs/CMSSW/TimingStudy/CMSSW_7_1_0_pre9/src/DPGAnalysis/PixelTimingStudy/Ntuple.root/trajTree");
  
  TH1D* dx_l1 = new TH1D("dx_l1", "PXB2 residuals #Deltax, p_{t} > 12, lever 1;PXB2 #Deltax [#mum];hits", 100, -150, 150); // h420_1
  TH1D* dx_l2 = new TH1D("dx_l2", "PXB1 residuals #Deltax, p_{t} > 12, lever 3;PXB1 #Deltax [#mum];hits", 100, -150, 150); // h520_3
  TH1D* dx_l3 = new TH1D("dx_l3", "PXB3 residuals #Deltax, p_{t} > 12, lever 3;PXB3 #Deltax [#mum];hits", 100, -150, 150); // i520_3
  
  TCanvas c("c");
  trajTree->Draw("res_dx>>dx_l1","layer==1&&pt>12&&lev>1.43&&lev<1.57");
  trajTree->Draw("res_dx>>dx_l2","layer==2&&pt>12&&lev>0&&lev<0.015","SAME");
  trajTree->Draw("res_dx>>dx_l3","layer==3&&pt>12&&lev>1.43&&lev<1.57","SAME");
  c.Close();
  
  double sigma_dx_l1 = fittp0(dx_l1);
  double sigma_dx_l2 = fittp0(dx_l2);
  double sigma_dx_l3 = fittp0(dx_l3);
  
  double res_dx_l1=sqrt((-sigma_dx_l1*sigma_dx_l1-10*sigma_dx_l2*sigma_dx_l2+5*sigma_dx_l3*sigma_dx_l3))/3;
  double res_dx_l2=sqrt((-sigma_dx_l1*sigma_dx_l1+14*sigma_dx_l2*sigma_dx_l2-sigma_dx_l3*sigma_dx_l3))/3;
  double res_dx_l3=sqrt((5*sigma_dx_l1*sigma_dx_l1-10*sigma_dx_l2*sigma_dx_l2-sigma_dx_l3*sigma_dx_l3))/3;
  
  std::cout<<"Resolution dx: Lay1 = "<<res_dx_l1<<" Lay2 = "<<res_dx_l2<<" Lay3 = "<<res_dx_l3<<std::endl;
  
  // Comparison with PixelTriplets:
  // PixelTimingStudy Resolution dx: Lay1 = 10.4437 Lay2 = 10.7449 Lay3 = 10.2808
  // PixelTriplets    Resolution dx: Lay1 = 10.5472 Lay2 = 10.7889 Lay3 = 10.4725
}
