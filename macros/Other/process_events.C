#include<iostream>
#include<cstdio>
#include<fstream>
#include<cmath>
#include<TH1.h>
#include<TLine.h>
#include<THStack.h>
#include<TLatex.h>
#include<TFile.h>
#include<TDirectoryFile.h>
#include<TGraph.h>
#include<TTree.h>
#include "TCanvas.h"
#include "TLegend.h"

using namespace std;

void process_events()
{
  ///////////////////////////////////////////////////////////////////////////
  //  This script calculates the dsigma/dW for different neutrino energies //
  //  Requires events in gst format & xsec splines                         //
  ///////////////////////////////////////////////////////////////////////////
  TFile * output_file = new TFile("NuEnergy_xsec.root","RECREATE");
  
  string fileE1 = "gntp.1.gst.root"; // 1GeV
  string fileE2 = "gntp.3.gst.root"; // 2GeV
  string fileE5 = "gntp.2.gst.root"; // 5GeV

  string fileXSecP = "xsec_nup.root"; 
  string fileXSecN = "xsec_nup.root"; 

  TFile * file1 = new TFile(fileE1.c_str(),"fileE1");
  TTree * gst1 = (TTree*)file1->Get("gst");
  TFile * file2 = new TFile(fileE2.c_str(),"fileE2");
  TTree * gst2 = (TTree*)file2->Get("gst");
  TFile * file3 = new TFile(fileE5.c_str(),"fileE3");
  TTree * gst3 = (TTree*)file3->Get("gst");

  const int bins = 200 ; 
  const double xmax = 3.2 ;
  const double xmin = 1.0 ; 
  const double binwidth = (xmax-xmin)/bins ;
  
  TFile * filexsecp = TFile::Open(fileXSecP.c_str() );
  TFile * filexsecn = TFile::Open(fileXSecN.c_str() );
  
  TDirectoryFile * numuH_p = (TDirectoryFile*) filexsecp->Get("nu_mu_H1");
  TDirectoryFile * numuH_n = (TDirectoryFile*) filexsecn->Get("nu_mu_H1");
  
  TGraph * gr_p = (TGraph*) numuH_p -> Get("tot_cc");
  TGraph * gr_n = (TGraph*) numuH_n -> Get("tot_cc");
  
  double xsecH_E1 = 0.5*(gr_p->Eval(1) + gr_n->Eval(1));
  double xsecH_E2 = 0.5*(gr_p->Eval(2) + gr_n->Eval(2));
  double xsecH_E5 = 0.5*(gr_p->Eval(5) + gr_n->Eval(5));

  TLine * Line_wcut = new TLine( 1.7, 0, 1.7, 3.5 ) ;
  Line_wcut->SetLineStyle(kDashed);

  TH1D * H_E1 = new TH1D("E1", "E1", bins, xmin, xmax ) ;  
  TH1D * H_qel_E1 = new TH1D("qelE1", "E1", bins, xmin, xmax ) ;  
  TH1D * H_qelc_E1 = new TH1D("qelcE1", "E1", bins, xmin, xmax ) ;  
  THStack * H_stack_E1 = new THStack("stackE1", "E1");
  TH1D * H_res_E1 = new TH1D("resE1", "E1", bins, xmin, xmax ) ;  
  TH1D * H_nrb_E1 = new TH1D("nrbE1", "E1", bins, xmin, xmax ) ;  
  TH1D * H_dis_E1 = new TH1D("disE1", "E1", bins, xmin, xmax ) ;  
  TH1D * H_disc_E1 = new TH1D("discE1", "E1", bins, xmin, xmax ) ;  

  TH1D * H_E2 = new TH1D("E2", "E2", bins, xmin, xmax ) ;  
  TH1D * H_qel_E2 = new TH1D("qelE2", "E2", bins, xmin, xmax ) ;  
  TH1D * H_qelc_E2 = new TH1D("qelcE2", "E2", bins, xmin, xmax ) ;  
  THStack * H_stack_E2 = new THStack("stackE2", "E2");
  TH1D * H_res_E2 = new TH1D("resE2", "E2", bins, xmin, xmax ) ;  
  TH1D * H_nrb_E2 = new TH1D("nrbE2", "E2", bins, xmin, xmax ) ;  
  TH1D * H_dis_E2 = new TH1D("disE2", "E2", bins, xmin, xmax ) ;  
  TH1D * H_disc_E2 = new TH1D("discE2", "E2", bins, xmin, xmax ) ;  
  
  TH1D * H_E5 = new TH1D("E5", "E5", bins, xmin, xmax ) ;  
  TH1D * H_qel_E5 = new TH1D("qelE5", "E5", bins, xmin, xmax ) ;  
  TH1D * H_qelc_E5 = new TH1D("qelcE5", "E5", bins, xmin, xmax ) ;  
  THStack * H_stack_E5 = new THStack("stackE5", "E5");
  TH1D * H_res_E5 = new TH1D("resE5", "E5", bins, xmin, xmax ) ;  
  TH1D * H_dis_E5 = new TH1D("disE5", "E5", bins, xmin, xmax ) ;  
  TH1D * H_disc_E5 = new TH1D("discE5", "E5", bins, xmin, xmax ) ;  
  TH1D * H_nrb_E5 = new TH1D("nrbE5", "E5", bins, xmin, xmax ) ;  
 
  long ccevents_E1 = 0 ,ccevents_E2 = 0 , ccevents_E5 = 0 ;
  const long nevents_E1 = gst1->GetEntries() ;
  const long nevents_E2 = gst2->GetEntries() ;
  const long nevents_E5 = gst3->GetEntries() ;

  double W_e1, W_e2, W_e5 ; 
  bool dis_e1, qel_e1, res_e1, cc_e1, charm_e1 ;
  bool dis_e2, qel_e2, res_e2, cc_e2, charm_e2 ;
  bool dis_e5, qel_e5, res_e5, cc_e5, charm_e5 ;

  gst1->SetBranchAddress("cc", &cc_e1);
  gst1->SetBranchAddress("res",&res_e1);
  gst1->SetBranchAddress("qel",&qel_e1);
  gst1->SetBranchAddress("dis",&dis_e1);
  gst1->SetBranchAddress("charm",&charm_e1);
  gst1->SetBranchAddress("Ws",&W_e1);

  gst2->SetBranchAddress("cc", &cc_e2);
  gst2->SetBranchAddress("res",&res_e2);
  gst2->SetBranchAddress("qel",&qel_e2);
  gst2->SetBranchAddress("dis",&dis_e2);
  gst2->SetBranchAddress("charm",&charm_e2);
  gst2->SetBranchAddress("Ws",&W_e2);

  gst3->SetBranchAddress("cc", &cc_e5);
  gst3->SetBranchAddress("res",&res_e5);
  gst3->SetBranchAddress("qel",&qel_e5);
  gst3->SetBranchAddress("dis",&dis_e5);
  gst3->SetBranchAddress("charm",&charm_e5);
  gst3->SetBranchAddress("Ws",&W_e5);

  for ( int i = 0 ; i < nevents_E1 ; ++i ){
    gst1 -> GetEntry(i) ;
    if( cc_e1  == true && W_e1 > 0) {
      H_E1 -> Fill( W_e1 ) ;
      ++ccevents_E1 ; 
      if( qel_e1 == true && charm_e1 == false) H_qel_E1 -> Fill( W_e1 ) ;
      if( qel_e1 == true && charm_e1 == true ) H_qelc_E1 -> Fill( W_e1 ) ;
      if( res_e1 == true ) H_res_E1 -> Fill( W_e1 ) ;
      if( dis_e1 == true && W_e1 < 1.7 && charm_e1 == false ) H_nrb_E1 -> Fill( W_e1 ) ;
      if( dis_e1 == true && W_e1 > 1.7 && charm_e1 == false ) H_dis_E1 -> Fill( W_e1 ) ;
      if( dis_e1 == true && W_e1 > 1.7 && charm_e1 == true ) H_disc_E1 -> Fill( W_e1 ) ;
    }
  }

  for ( int i = 0 ; i < nevents_E2 ; ++i ){
    gst2 -> GetEntry(i) ;
    if( cc_e2  == true ) {
      H_E2 -> Fill( W_e2 ) ;
      ++ccevents_E2 ; 
      if( qel_e2 == true && charm_e2 == false ) H_qel_E2 -> Fill( W_e2 ) ;
      if( qel_e2 == true && charm_e2 == true ) H_qelc_E2 -> Fill( W_e2 ) ;
      if( res_e2 == true ) H_res_E2 -> Fill( W_e2 ) ;
      if( dis_e2 == true && W_e2 < 1.7 && charm_e2 == false) H_nrb_E2 -> Fill( W_e2 ) ;
      if( dis_e2 == true && W_e2 > 1.7 && charm_e2 == false) H_dis_E2 -> Fill( W_e2 ) ;
      if( dis_e2 == true && W_e2 > 1.7 && charm_e2 == true ) H_disc_E2 -> Fill( W_e2 ) ;
    }
  }

  for ( int i = 0 ; i < nevents_E5 ; ++i ){
    gst3 -> GetEntry(i) ;
    if( cc_e5  == true ) {
      H_E5 -> Fill( W_e5 ) ;
      ++ccevents_E5 ; 
      if( qel_e5 == true && charm_e5 == false ) H_qel_E5 -> Fill( W_e5 ) ;
      if( qel_e5 == true && charm_e5 == true ) H_qelc_E5 -> Fill( W_e5 ) ;
      if( res_e5 == true ) H_res_E5 -> Fill( W_e5 ) ;
      if( dis_e5 == true && W_e5 <= 1.7 && charm_e5 == false ) H_nrb_E5 -> Fill( W_e5 ) ;
      if( dis_e5 == true && W_e5 > 1.7 && charm_e5 == false) H_dis_E5 -> Fill( W_e5 ) ;
      if( dis_e5 == true && W_e5 > 1.7 && charm_e5 == true) H_disc_E5 -> Fill( W_e5 ) ;
    }
  }

  TCanvas * c  = new TCanvas("c","c",800,600);
  c->Divide(1,3,1E-11, 1E-11);
  TLegend* leg = new TLegend(.5,.75, 0.95,0.95,"");

  H_E1->SetStats(0);
  H_E1->SetLineStyle(1);
  H_E1->SetLineColor(kBlack);
  H_E1->SetLineWidth(2);
  H_E1->SetFillStyle(4050);

  H_qelc_E1->SetStats(0);
  H_qelc_E1->SetLineStyle(1);
  H_qelc_E1->SetLineColor(kRed);
  H_qelc_E1->SetFillColor(kRed);
  H_qelc_E1->SetLineWidth(1);
  H_qelc_E1->SetFillStyle(3444);

  H_res_E1->SetStats(0);
  H_res_E1->SetLineStyle(1);
  H_res_E1->SetLineColor(kBlue+1);
  H_res_E1->SetFillColor(kBlue+1);
  H_res_E1->SetFillStyle(3444);
  
  H_nrb_E1->SetStats(0);
  H_nrb_E1->SetLineStyle(1);
  H_nrb_E1->SetLineColor(kMagenta-3);
  H_nrb_E1->SetFillColor(kMagenta-3);
  H_nrb_E1->SetFillStyle(3354);
  
  H_dis_E1->SetStats(0);
  H_dis_E1->SetLineStyle(1);
  H_dis_E1->SetLineColor(kGreen);
  H_dis_E1->SetFillColor(kGreen);
  H_dis_E1->SetFillStyle(3365);

  H_disc_E1->SetStats(0);
  H_disc_E1->SetLineWidth(2);
  H_disc_E1->SetLineColor(kCyan+3);
  H_disc_E1->SetFillColor(kCyan+3);
  H_disc_E1->SetFillStyle(3366);

  double scaling_e1 = xsecH_E1 / ( ccevents_E1 * binwidth ) ;  

  H_qel_E1 -> Scale(  scaling_e1 , "nosw2"); 
  H_qelc_E1 -> Scale(  scaling_e1 , "nosw2"); 
  H_res_E1 -> Scale(  scaling_e1 , "nosw2"); 
  H_nrb_E1 -> Scale(  scaling_e1 , "nosw2"); 
  H_dis_E1 -> Scale(  scaling_e1 , "nosw2"); 
  H_disc_E1 -> Scale(  scaling_e1 , "nosw2"); 
  H_E1     -> Scale(  scaling_e1 , "nosw2");  

  H_stack_E1->Add(H_nrb_E1);  
  H_stack_E1->Add(H_res_E1);
  H_stack_E1->Add(H_qelc_E1);
  H_stack_E1->Add(H_disc_E1);
  H_stack_E1->Add(H_dis_E1);
  
  leg->AddEntry(H_E1,"CC Inclusive","LF");
  //leg->AddEntry(H_qel_E1,"CC QEL","LF");
  leg->AddEntry(H_res_E1,"CC RES","LF");
  leg->AddEntry(H_nrb_E1,"CC NonRES","LF");
  leg->AddEntry(H_qelc_E1,"CC QEL Charm","LF");
  leg->AddEntry(H_dis_E1,"CC DIS","LF");
  leg->AddEntry(H_disc_E1,"CC DIS Charm","LF");
  
  c->cd(1);
  H_E1->Draw();
  H_E1->GetYaxis() -> SetRangeUser( 0., 3.5 ) ;
  H_E1 -> SetTitle("E_{#nu} = 1 GeV" ); 
  H_E1 -> GetYaxis() -> SetTitle("d#sigma/dW [10^{-38}cm^{2}c^{2}/GeV]");
  H_stack_E1->Draw("same");
  H_E1->Draw("same");
  Line_wcut->Draw("same");

  TLatex wcut_text ;
  wcut_text.SetTextSize(0.025);
  wcut_text.SetTextAlign(12);
  wcut_text.DrawLatex(1.8, 3., "W_{cut}");

  leg->Draw();
  
  ////// E2

  H_nrb_E2->SetMaximum(3.5);

  H_E2->SetStats(0);
  H_E2->SetLineStyle(1);
  H_E2->SetLineColor(kBlack);
  H_E2->SetLineWidth(2);

  H_qelc_E2->SetStats(0);
  H_qelc_E2->SetLineStyle(1);
  H_qelc_E2->SetLineColor(kRed);
  H_qelc_E2->SetFillColor(kRed);
  H_qelc_E2->SetLineWidth(1);
  H_qelc_E2->SetFillStyle(3444);

  H_res_E2->SetStats(0);
  H_res_E2->SetLineStyle(1);
  H_res_E2->SetLineColor(kBlue+1);
  H_res_E2->SetFillColor(kBlue+1);
  H_res_E2->SetFillStyle(3444);
  
  H_nrb_E2->SetStats(0);
  H_nrb_E2->SetLineStyle(1);
  H_nrb_E2->SetLineColor(kMagenta-3);
  H_nrb_E2->SetFillColor(kMagenta-3);
  H_nrb_E2->SetFillStyle(3354);
  
  H_dis_E2->SetStats(0);
  H_dis_E2->SetLineStyle(1);
  H_dis_E2->SetLineColor(kGreen+2);
  H_dis_E2->SetFillColor(kGreen+2);
  H_dis_E2->SetFillStyle(3365);

  H_disc_E2->SetStats(0);
  H_disc_E2->SetLineStyle(1);
  H_disc_E2->SetLineWidth(2);
  H_disc_E2->SetLineColor(kCyan+3);
  H_disc_E2->SetFillColor(kCyan+3);
  H_disc_E2->SetFillStyle(3366);
  
  double scaling_E2 = xsecH_E2 / ( ccevents_E2 * binwidth ) ; 
  H_qel_E2 -> Scale( scaling_E2 , "nosw2" ); 
  H_qelc_E2 -> Scale( scaling_E2 , "nosw2" ); 
  H_res_E2 -> Scale( scaling_E2 , "nosw2" ); 
  H_nrb_E2 -> Scale( scaling_E2 , "nosw2" ); 
  H_disc_E2 -> Scale( scaling_E2 , "nosw2" ); 
  H_dis_E2 -> Scale( scaling_E2 , "nosw2" ); 
  H_E2     -> Scale( scaling_E2 , "nosw2" ); 

  H_stack_E2->Add(H_nrb_E2);  
  H_stack_E2->Add(H_res_E2);
  H_stack_E2->Add(H_qelc_E2);
  H_stack_E2->Add(H_disc_E2);
  H_stack_E2->Add(H_dis_E2);

  c->cd(2);
  H_E2->Draw();
  H_E2->GetYaxis() -> SetRangeUser( 0., 3.5 ) ;
  H_E2 -> SetTitle("E_{#nu} = 2 GeV" ); 
  H_E2 -> GetYaxis() -> SetTitle("d#sigma/dW [10^{-38}cm^{2}c^{2}/GeV]");
  H_stack_E2->Draw("same");
  H_E2->Draw("same");
  Line_wcut->Draw("same");


  //// E 5

  H_E5->SetStats(0);
  H_E5->SetLineStyle(1);
  H_E5->SetLineColor(kBlack);
  H_E5->SetLineWidth(2);

  H_qelc_E5->SetStats(0);
  H_qelc_E5->SetLineStyle(1);
  H_qelc_E5->SetLineColor(kRed);
  H_qelc_E5->SetFillColor(kRed);
  H_qelc_E5->SetLineWidth(1);
  H_qelc_E5->SetFillStyle(3444);

  H_res_E5->SetStats(0);
  H_res_E5->SetLineStyle(1);
  H_res_E5->SetLineColor(kBlue+1);
  H_res_E5->SetFillColor(kBlue+1);
  H_res_E5->SetFillStyle(3444);
  
  H_nrb_E5->SetStats(0);
  H_nrb_E5->SetLineStyle(1);
  H_nrb_E5->SetLineColor(kMagenta-3);
  H_nrb_E5->SetFillColor(kMagenta-3);
  H_nrb_E5->SetFillStyle(3354);
  
  H_dis_E5->SetStats(0);
  H_dis_E5->SetLineStyle(1);
  H_dis_E5->SetLineColor(kGreen+2);
  H_dis_E5->SetFillColor(kGreen+2);
  H_dis_E5->SetFillStyle(3465);
  

  H_disc_E5->SetStats(0);
  H_disc_E5->SetLineStyle(1);
  H_disc_E5->SetLineColor(kCyan+3);
  H_disc_E5->SetFillColor(kCyan+3);
  H_disc_E5->SetFillStyle(3466);
  H_disc_E5->SetLineWidth(2);  

  double scaling_E5 = xsecH_E5 / ( ccevents_E5 * binwidth ) ;
  H_qel_E5 -> Scale( scaling_E5 , "nosw2" ); 
  H_qelc_E5 -> Scale( scaling_E5 , "nosw2" ); 
  H_res_E5 -> Scale( scaling_E5 , "nosw2" ); 
  H_nrb_E5 -> Scale( scaling_E5 , "nosw2" ); 
  H_dis_E5 -> Scale( scaling_E5 , "nosw2" ); 
  H_disc_E5 -> Scale( scaling_E5 , "nosw2" ); 
  H_E5     -> Scale( scaling_E5 , "nosw2" ); 

  H_stack_E5->Add(H_nrb_E5); 
  H_stack_E5->Add(H_res_E5); 
  H_stack_E5->Add(H_qelc_E5); 
  H_stack_E5->Add(H_disc_E5); 
  H_stack_E5->Add(H_dis_E5); 
  
  c->cd(3);
  H_E5->Draw();
  H_E5->GetYaxis() -> SetRangeUser( 0., 3.5 ) ;
  H_E5 -> SetTitle("E_{#nu} = 5 GeV" ); 
  H_E5 -> GetXaxis() -> SetTitle("W [GeV/c^{2}]");
  H_E5 -> GetYaxis() -> SetTitle("d#sigma/dW [10^{-38}cm^{2}c^{2}/GeV]");
  H_stack_E5->Draw("same");
  H_E5->Draw("same");
  Line_wcut->Draw("same");
  c->SaveAs("NuEnergystudy.root");
  c->Clear();

}
