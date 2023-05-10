//
// Created by alons on 10/05/2023.
//

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

void PlotsEff2x3() {
//void PlotsEff2x3(hPlot1D Beta1, hPlot1D Beta2, hPlot1D Beta3, hPlot1D Beta4) {

//    TFile *f = new TFile("./plots_C12_simulation_6GeV_T5_first_10_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_10";
    TFile *f = new TFile("./plots_C12_simulation_6GeV_T5_first_100_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
    string SampleName = "C12_simulation_6GeV_T5_first_100";

    string saveName;

    if (SampleName == "C12_simulation_6GeV_T5_first_10") {
        saveName = "./All_neutrals_10_NOPDG0.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_100") {
        saveName = "./All_neutrals_100_NOPDG0.png";
    } else if (SampleName == "C12_simulation_6GeV_T5") {
        saveName = "./All_neutrals_ALL_NOPDG0.png";
    }

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas2x6 = new TCanvas("Canvas2x6", "Canvas2x6", 1000 * 2, 750 * 3); // normal res
//    TCanvas *Canvas = new TCanvas("Canvas2x6", "Canvas2x6", 2000*4, 1500*4); // high res
//    TCanvas *Canvas = new TCanvas("Canvas2x6", "Canvas2x6", 1650*4, 1150*4);

//    Canvas->cd();
//    Canvas2x6->SetGrid();
//    Canvas2x6->SetBottomMargin(0.14);
//
//    Canvas2x6->SetLeftMargin(0.17);
////    Canvas2x6->SetLeftMargin(0.16);
//    Canvas2x6->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas2x6->Divide(2, 3);

    TH1D *RecPlot_1p = (TH1D *) f->Get("h1 (1p, FD)");
    TH1D *TruthPlot_1p = (TH1D *) f->Get("Rec. Neutron momentum");
    TH1D *EffPlot_1p = (TH1D *) f->Get("h3 (1p, FD)");
    TH1D *RecPlot_1n = (TH1D *) f->Get("h4 (1n, FD)");
    TH1D *TruthPlot_1n = (TH1D *) f->Get("Neutron momentum (1n, FD)");
    TH1D *EffPlot_1n = (TH1D *) f->Get("h4 (1n, FD)");

    if (!RecPlot_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    Canvas2x6->cd(1);
    Canvas2x6->cd(1)->SetGrid();
    Canvas2x6->cd(1)->SetBottomMargin(0.14);
    Canvas2x6->cd(1)->SetLeftMargin(0.17);
    Canvas2x6->cd(1)->SetRightMargin(0.12);
    RecPlot_1p->Draw();

    Canvas2x6->cd(2);
    Canvas2x6->cd(2)->SetGrid();
    Canvas2x6->cd(2)->SetBottomMargin(0.14);
    Canvas2x6->cd(2)->SetLeftMargin(0.17);
    Canvas2x6->cd(2)->SetRightMargin(0.12);
    TruthPlot_1p->Draw();

    Canvas2x6->cd(3);
    Canvas2x6->cd(3)->SetGrid();
    Canvas2x6->cd(3)->SetBottomMargin(0.14);
    Canvas2x6->cd(3)->SetLeftMargin(0.17);
    Canvas2x6->cd(3)->SetRightMargin(0.12);
    EffPlot_1p->Draw();

    Canvas2x6->cd(4);
    Canvas2x6->cd(4)->SetGrid();
    Canvas2x6->cd(4)->SetBottomMargin(0.14);
    Canvas2x6->cd(4)->SetLeftMargin(0.17);
    Canvas2x6->cd(4)->SetRightMargin(0.12);
    RecPlot_1n->Draw();

    Canvas2x6->cd(5);
    Canvas2x6->cd(5)->SetGrid();
    Canvas2x6->cd(5)->SetBottomMargin(0.14);
    Canvas2x6->cd(5)->SetLeftMargin(0.17);
    Canvas2x6->cd(5)->SetRightMargin(0.12);
    TruthPlot_1n->Draw();

    Canvas2x6->cd(6);
    Canvas2x6->cd(6)->SetGrid();
    Canvas2x6->cd(6)->SetBottomMargin(0.14);
    Canvas2x6->cd(6)->SetLeftMargin(0.17);
    Canvas2x6->cd(6)->SetRightMargin(0.12);
    EffPlot_1n->Draw();

    Canvas2x6->SaveAs(saveName.c_str());
    Canvas2x6->Clear();
    //</editor-fold>

}

