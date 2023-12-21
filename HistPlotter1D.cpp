/* root BetaFitAndSave.cpp -q -b */

#include <iostream>
#include <fstream>
#include <string>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

using namespace std;

void HistPlotter1D() {
    cout << "\n\n";

    TCanvas *c = new TCanvas("c1", "c2", 1000, 750 * 4.5); // normal res
    c->cd()->SetGrid();
    c->cd()->SetBottomMargin(0.14);
    c->cd()->SetLeftMargin(0.18);
    c->cd()->SetRightMargin(0.12);
    c->cd();

    TFile *f_Reco = new TFile("02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root");
    TH1D *nFD_AMap_Reco = (TH1D *) f_Reco->Get("Reco Neutron Hit Map");
    nFD_AMap_Reco->Draw("colz");

//    //<editor-fold desc="old">
//    TCanvas *c = new TCanvas("c1", "c2", 1000, 750 * 4.5); // normal res
//    c->Divide(3, 1);
//
//    TFile *f_Reco = new TFile("02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root");
//    TH2D *nFD_AMap_Reco = (TH2D *) f_Reco->Get("Reco Neutron Hit Map");
//    c->cd(1);
//    c->cd(1)->SetGrid();
//    c->cd(1)->SetBottomMargin(0.14);
//    c->cd(1)->SetLeftMargin(0.18);
//    c->cd(1)->SetRightMargin(0.12);
//    nFD_AMap_Reco->Draw("colz");
//
//    TFile *f_TL = new TFile("01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root");
//    TH2D *nFD_AMap_TL = (TH2D *) f_TL->Get("TL Neutron Hit Map");
//    c->cd(2);
//    c->cd(2)->SetGrid();
//    c->cd(2)->SetBottomMargin(0.14);
//    c->cd(2)->SetLeftMargin(0.18);
//    c->cd(2)->SetRightMargin(0.12);
//    nFD_AMap_TL->Draw("colz");
//
//    TFile *f_Div = new TFile("02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root");
//    TH2D *nFD_AMap_Div = (TH2D *) f_Div->Get("Reco Neutron Hit Map");
//    nFD_AMap_Div->Divide(nFD_AMap_TL);
//    c->cd(3);
//    c->cd(3)->SetGrid();
//    c->cd(3)->SetBottomMargin(0.14);
//    c->cd(3)->SetLeftMargin(0.18);
//    c->cd(3)->SetRightMargin(0.12);
//    nFD_AMap_Div->SetTitle("divided");
//    nFD_AMap_Div->Draw("colz");
//    //</editor-fold>

}
