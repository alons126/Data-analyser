/*
root -b -q PlotsBeta4x4.cpp
*/

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

////#include "source/functions/GeneralFunctions.h"
//#include "source/functions/to_string_with_precision.h"
//#include "source/functions/findSubstring.h"
//#include "source/classes/DSCuts/DSCuts.h"
//#include "source/classes/hPlots/hPlot1D.h"
//#include "source/constants.h"
////#include "source/functions/Math_func/poly34.cpp"

using namespace std;

void PlotsBeta4x4() {
//void PlotsBeta4x4(hPlot1D Beta1, hPlot1D Beta2, hPlot1D Beta3, hPlot1D Beta4) {

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_10";

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_100";
//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_100";

    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
    string SampleName = "C12_simulation_6GeV_T5";

    string saveName;

    if (SampleName == "C12_simulation_6GeV_T5_first_10") {
        saveName = "./All_neutrals_10_NOPDG0.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_100") {
        saveName = "./All_neutrals_100_NOPDG0.png";
    } else if (SampleName == "C12_simulation_6GeV_T5") {
        saveName = "./All_neutrals_ALL_NOPDG0.png";
    }

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas4x4 = new TCanvas("Canvas4x4", "Canvas4x4", 1000 * 4, 750 * 4); // normal res
//    TCanvas *Canvas = new TCanvas("Canvas4x4", "Canvas4x4", 2000*4, 1500*4); // high res
//    TCanvas *Canvas = new TCanvas("Canvas4x4", "Canvas4x4", 1650*4, 1150*4);

//    Canvas->cd();
//    Canvas4x4->SetGrid();
//    Canvas4x4->SetBottomMargin(0.14);
//
//    Canvas4x4->SetLeftMargin(0.17);
////    Canvas4x4->SetLeftMargin(0.16);
//    Canvas4x4->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas4x4->Divide(2, 2);

    TH1D *Beta1Plot = (TH1D *) f->Get("h1 (1n, FD)");
//    TH1D *Beta1Plot = (TH1D *) f1->Get("FD neutrals (no PDG=0) #beta (1n, FD)");
    TH1D *Beta2Plot = (TH1D *) f->Get("h2 (1n, FD)");
//    TH1D *Beta2Plot = (TH1D *) f->Get("FD neutrals (no PDG=0) #beta w/o PCAL hit (1n, FD)");
    TH1D *Beta3Plot = (TH1D *) f->Get("h3 (1n, FD)");
//    TH1D *Beta3Plot = (TH1D *) f->Get("FD neutrals (no PDG=0) #beta w/o PCAL & w/ ECIN (1n, FD)");
    TH1D *Beta4Plot = (TH1D *) f->Get("h4 (1n, FD)");
//    TH1D *Beta4Plot = (TH1D *) f->Get("FD neutrals (no PDG=0) #beta w/o PCAL/ECIN & w/ ECOUT (1n, FD)");

    if (!Beta1Plot) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!Beta2Plot) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!Beta3Plot) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!Beta4Plot) { cout << "\n\nEmpty hist\n\n\n"; }

    Canvas4x4->cd(1);
    Canvas4x4->cd(1)->SetGrid();
    Canvas4x4->cd(1)->SetBottomMargin(0.14);
    Canvas4x4->cd(1)->SetLeftMargin(0.17);
    Canvas4x4->cd(1)->SetRightMargin(0.12);
//    Beta1Plot->Rebin(2);
    Beta1Plot->Draw();

    Canvas4x4->cd(2);
    Canvas4x4->cd(2)->SetGrid();
    Canvas4x4->cd(2)->SetBottomMargin(0.14);
    Canvas4x4->cd(2)->SetLeftMargin(0.17);
    Canvas4x4->cd(2)->SetRightMargin(0.12);
//    Beta2Plot->Rebin(2);
    Beta2Plot->Draw();

    Canvas4x4->cd(3);
    Canvas4x4->cd(3)->SetGrid();
    Canvas4x4->cd(3)->SetBottomMargin(0.14);
    Canvas4x4->cd(3)->SetLeftMargin(0.17);
    Canvas4x4->cd(3)->SetRightMargin(0.12);
//    Beta3Plot->Rebin(2);
    Beta3Plot->Draw();

    Canvas4x4->cd(4);
    Canvas4x4->cd(4)->SetGrid();
    Canvas4x4->cd(4)->SetBottomMargin(0.14);
    Canvas4x4->cd(4)->SetLeftMargin(0.17);
    Canvas4x4->cd(4)->SetRightMargin(0.12);
//    Beta4Plot->Rebin(2);
    Beta4Plot->Draw();

    Canvas4x4->SaveAs(saveName.c_str());
//    Canvas4x4->SaveAs("./All_neutrals_NOPDG0.png");
    Canvas4x4->Clear();
    /*    TFile *f1 = new TFile("./plots_C12_simulation_6GeV_T5_first_10_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
    TFile *f = new TFile("./plots_C12_simulation_6GeV_T5_first_10_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
    string SampleName = "C12_simulation_6GeV_T5";

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas4x4 = new TCanvas("Canvas4x4", "Canvas4x4", 1000*4, 750*4); // normal res
//    TCanvas *Canvas = new TCanvas("Canvas4x4", "Canvas4x4", 2000*4, 1500*4); // high res
//    TCanvas *Canvas = new TCanvas("Canvas4x4", "Canvas4x4", 1650*4, 1150*4);
//    Canvas->cd();
    Canvas4x4->SetGrid();
    Canvas4x4->SetBottomMargin(0.14);

    Canvas4x4->SetLeftMargin(0.17);
//    Canvas4x4->SetLeftMargin(0.16);
    Canvas4x4->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas4x4->Divide(2, 2);

    TH1D *Beta1Plot = (TH1D *) f1->Get("h1 (1n, FD)");
//    TH1D *Beta1Plot = (TH1D *) f->Get("h1 (1n, FD)");
//    TH1D *Beta1Plot = (TH1D *) f1->Get("FD neutrals (no PDG=0) #beta (1n, FD)");
    TH1D *Beta2Plot = (TH1D *) f->Get("h2 (1n, FD)");
//    TH1D *Beta2Plot = (TH1D *) f->Get("FD neutrals (no PDG=0) #beta w/o PCAL hit (1n, FD)");
    TH1D *Beta3Plot = (TH1D *) f->Get("h3 (1n, FD)");
//    TH1D *Beta3Plot = (TH1D *) f->Get("FD neutrals (no PDG=0) #beta w/o PCAL & w/ ECIN (1n, FD)");
    TH1D *Beta4Plot = (TH1D *) f->Get("h4 (1n, FD)");
//    TH1D *Beta4Plot = (TH1D *) f->Get("FD neutrals (no PDG=0) #beta w/o PCAL/ECIN & w/ ECOUT (1n, FD)");

    if (!Beta2Plot) {
        cout << "empty hist\n\n\n";
    }

    Canvas4x4->cd(1);
    Canvas4x4->cd(1)->SetGrid();
    Beta1Plot->Draw();
//    Beta2Plot->Draw();

    Canvas4x4->cd(2);
    Canvas4x4->cd(2)->SetGrid();
//    Beta1Plot->Draw();
    Beta2Plot->Draw();

    Canvas4x4->cd(3);
    Canvas4x4->cd(3)->SetGrid();
//    Beta1Plot->Draw();
    Beta3Plot->Draw();

    Canvas4x4->cd(4);
    Canvas4x4->cd(4)->SetGrid();
//    Beta1Plot->Draw();
    Beta4Plot->Draw();

    Canvas4x4->SaveAs("./All_neutrals_NOPDG0.png");
    Canvas4x4->Clear();*/
    //</editor-fold>

}

