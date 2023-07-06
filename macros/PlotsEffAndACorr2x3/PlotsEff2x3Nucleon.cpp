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

#include "../../source/functions/findSubstring.h"

using namespace std;

void PlotsEff2x3Nucleon(string SampleName) {
    cout << "\nSaving efficiency plots for Nucleons...\n\n";

    //<editor-fold desc="Setting file">
    TFile *f;

    if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_10")) {
        if (SampleName == "C12_simulation_6GeV_T5_first_10_wNC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wNC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wNC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wNC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wNC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wNC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wNC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wNC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_100")) {
        if (SampleName == "C12_simulation_6GeV_T5_first_100_wNC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wNC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wNC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wNC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wNC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wNC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wNC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wNC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_250")) {
        if (SampleName == "C12_simulation_6GeV_T5_first_250_wNC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wNC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wNC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wNC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wNC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wNC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wNC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wNC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5")) {
        if (SampleName == "C12_simulation_6GeV_T5_wNC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wNC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wNC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wNC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wNC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wNC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wNC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wNC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wNC_wFC_Eff1") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wNC_wFC_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wNC_wFC_Eff2") {
            f = new TFile("00_plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wNC_wFC_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_G18_2GeV")) {
        if (SampleName == "C12_simulation_G18_2GeV_wNC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wNC_noFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wNC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wNC_noFDph_Eff2/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wNC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wNC_wFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wNC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wNC_wFDph_Eff2/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_noFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_noFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_noFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_noFDph_Eff2/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_wFDph_Eff1") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_wFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_wFDph_Eff2") {
            f = new TFile("00_plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_wFDph_Eff2/reconhipo_plots.root");
        }
    }
    //</editor-fold>

    //<editor-fold desc="Setting CanvasEff2x3Nucleon">
    TCanvas *CanvasEff2x3Nucleon = new TCanvas("CanvasEff2x3Nucleon", "CanvasEff2x3Nucleon", 1000 * 3, 750 * 2); // normal res

//    CanvasEff2x3Nucleon->cd();
//    CanvasEff2x3Nucleon->Divide(3, 2);
    //</editor-fold>

    //<editor-fold desc="Momentum efficiency plots">
    string MomEffMomEffSaveName = "./01_Nucleon_Mom_Eff_" + SampleName + ".png";

    CanvasEff2x3Nucleon->Divide(3, 2);

    TH1D *RecPlot_mom_1p = (TH1D *) f->Get("Proton momentum APID (1p, FD)");

    RecPlot_mom_1p->SetLineColor(kBlue);
    RecPlot_mom_1p->SetStats(1);
    RecPlot_mom_1p->Sumw2();
    RecPlot_mom_1p->Rebin(2);

    TH1D *TruthPlot_mom_1p = (TH1D *) f->Get("FD TL Proton momentum AC (1p, FD)");
    TruthPlot_mom_1p->SetLineColor(kBlue);
    TruthPlot_mom_1p->SetStats(1);
    TruthPlot_mom_1p->Sumw2();
    TruthPlot_mom_1p->Rebin(2);

    TH1D *EffPlot_mom_1p = (TH1D *) f->Get("Proton momentum #epsilon_{eff} (1p)");
    EffPlot_mom_1p->SetLineColor(kBlue);
//    EffPlot_mom_1p->Sumw2();
//    EffPlot_mom_1p->Rebin(2);
//    EffPlot_mom_1p->Divide(TruthPlot_mom_1p);

    TH1D *RecPlot_mom_1n = (TH1D *) f->Get("Neutron momentum APID (1n, FD)");
    RecPlot_mom_1n->SetLineColor(kBlue);
    RecPlot_mom_1n->SetStats(1);
    RecPlot_mom_1n->Sumw2();
    RecPlot_mom_1n->Rebin(2);

    TH1D *TruthPlot_mom_1n = (TH1D *) f->Get("FD TL Neutron momentum AC (1n, FD)");
    TruthPlot_mom_1n->SetLineColor(kBlue);
    TruthPlot_mom_1n->SetStats(1);
    TruthPlot_mom_1n->Sumw2();
    TruthPlot_mom_1n->Rebin(2);

    TH1D *EffPlot_mom_1n = (TH1D *) f->Get("Neutron momentum #epsilon_{eff} (1n)");
    EffPlot_mom_1n->SetLineColor(kBlue);
//    EffPlot_mom_1n->Sumw2();
//    EffPlot_mom_1n->Rebin(2);
//    EffPlot_mom_1n->Divide(TruthPlot_mom_1n);

    if (!RecPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasEff2x3Nucleon->cd(1);
    CanvasEff2x3Nucleon->cd(1)->SetGrid();
    CanvasEff2x3Nucleon->cd(1)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(1)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(1)->SetRightMargin(0.12);
    RecPlot_mom_1p->Draw();

    CanvasEff2x3Nucleon->cd(2);
    CanvasEff2x3Nucleon->cd(2)->SetGrid();
    CanvasEff2x3Nucleon->cd(2)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(2)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(2)->SetRightMargin(0.12);
    TruthPlot_mom_1p->Draw();

    CanvasEff2x3Nucleon->cd(3);
    CanvasEff2x3Nucleon->cd(3)->SetGrid();
    CanvasEff2x3Nucleon->cd(3)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(3)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(3)->SetRightMargin(0.12);
    EffPlot_mom_1p->Draw();

    CanvasEff2x3Nucleon->cd(4);
    CanvasEff2x3Nucleon->cd(4)->SetGrid();
    CanvasEff2x3Nucleon->cd(4)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(4)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(4)->SetRightMargin(0.12);
    RecPlot_mom_1n->Draw();

    CanvasEff2x3Nucleon->cd(5);
    CanvasEff2x3Nucleon->cd(5)->SetGrid();
    CanvasEff2x3Nucleon->cd(5)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(5)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(5)->SetRightMargin(0.12);
    TruthPlot_mom_1n->Draw();

    CanvasEff2x3Nucleon->cd(6);
    CanvasEff2x3Nucleon->cd(6)->SetGrid();
    CanvasEff2x3Nucleon->cd(6)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(6)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(6)->SetRightMargin(0.12);
    EffPlot_mom_1n->Draw();

    CanvasEff2x3Nucleon->SaveAs(MomEffMomEffSaveName.c_str());
    CanvasEff2x3Nucleon->Clear();
    //</editor-fold>

    //<editor-fold desc="Theta efficiency plots">
    string ThetaEffSaveName = "./02_Nucleon_Theta_Eff_" + SampleName + ".png";

    CanvasEff2x3Nucleon->Divide(3, 2);

    TH1D *RecPlot_Theta_1p = (TH1D *) f->Get("#theta_{p} (All Int., 1p, FD)");
    RecPlot_Theta_1p->SetLineColor(kBlue);
    RecPlot_Theta_1p->SetStats(1);
    RecPlot_Theta_1p->Sumw2();
    RecPlot_Theta_1p->Rebin(2);

    TH1D *TruthPlot_Theta_1p = (TH1D *) f->Get("FD TL #theta^{truth}_{p} AC (1p, FD)");
    TruthPlot_Theta_1p->SetLineColor(kBlue);
    TruthPlot_Theta_1p->SetStats(1);
    TruthPlot_Theta_1p->Sumw2();
    TruthPlot_Theta_1p->Rebin(2);

    TH1D *EffPlot_Theta_1p = (TH1D *) f->Get("Proton theta #epsilon_{eff} (1p)");
    EffPlot_Theta_1p->SetLineColor(kBlue);
//    EffPlot_Theta_1p->Sumw2();
//    EffPlot_Theta_1p->Rebin(2);
//    EffPlot_Theta_1p->Divide(TruthPlot_Theta_1p);

    TH1D *RecPlot_Theta_1n = (TH1D *) f->Get("#theta_{n} (All Int., 1n, FD)");
    RecPlot_Theta_1n->SetLineColor(kBlue);
    RecPlot_Theta_1n->SetStats(1);
    RecPlot_Theta_1n->Sumw2();
    RecPlot_Theta_1n->Rebin(2);

    TH1D *TruthPlot_Theta_1n = (TH1D *) f->Get("FD TL #theta^{truth}_{n} AC (1n, FD)");
    TruthPlot_Theta_1n->SetLineColor(kBlue);
    TruthPlot_Theta_1n->SetStats(1);
    TruthPlot_Theta_1n->Sumw2();
    TruthPlot_Theta_1n->Rebin(2);

    TH1D *EffPlot_Theta_1n = (TH1D *) f->Get("Neutron theta #epsilon_{eff} (1n)");
    EffPlot_Theta_1n->SetLineColor(kBlue);
//    EffPlot_Theta_1n->Sumw2();
//    EffPlot_Theta_1n->Rebin(2);
//    EffPlot_Theta_1n->Divide(TruthPlot_Theta_1n);

    if (!RecPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasEff2x3Nucleon->cd(1);
    CanvasEff2x3Nucleon->cd(1)->SetGrid();
    CanvasEff2x3Nucleon->cd(1)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(1)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(1)->SetRightMargin(0.12);
    RecPlot_Theta_1p->Draw();

    CanvasEff2x3Nucleon->cd(2);
    CanvasEff2x3Nucleon->cd(2)->SetGrid();
    CanvasEff2x3Nucleon->cd(2)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(2)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(2)->SetRightMargin(0.12);
    TruthPlot_Theta_1p->Draw();

    CanvasEff2x3Nucleon->cd(3);
    CanvasEff2x3Nucleon->cd(3)->SetGrid();
    CanvasEff2x3Nucleon->cd(3)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(3)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(3)->SetRightMargin(0.12);
    EffPlot_Theta_1p->Draw();

    CanvasEff2x3Nucleon->cd(4);
    CanvasEff2x3Nucleon->cd(4)->SetGrid();
    CanvasEff2x3Nucleon->cd(4)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(4)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(4)->SetRightMargin(0.12);
    RecPlot_Theta_1n->Draw();

    CanvasEff2x3Nucleon->cd(5);
    CanvasEff2x3Nucleon->cd(5)->SetGrid();
    CanvasEff2x3Nucleon->cd(5)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(5)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(5)->SetRightMargin(0.12);
    TruthPlot_Theta_1n->Draw();

    CanvasEff2x3Nucleon->cd(6);
    CanvasEff2x3Nucleon->cd(6)->SetGrid();
    CanvasEff2x3Nucleon->cd(6)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(6)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(6)->SetRightMargin(0.12);
    EffPlot_Theta_1n->Draw();

    CanvasEff2x3Nucleon->SaveAs(ThetaEffSaveName.c_str());
    CanvasEff2x3Nucleon->Clear();
    //</editor-fold>

    //<editor-fold desc="Phi efficiency plots">
    string PhiEffSaveName = "./03_Nucleon_Phi_Eff_" + SampleName + ".png";

    CanvasEff2x3Nucleon->Divide(3, 2);

    TH1D *RecPlot_Phi_1p = (TH1D *) f->Get("#phi_{p} (All Int., 1p, FD)");
    RecPlot_Phi_1p->SetLineColor(kBlue);
    RecPlot_Phi_1p->SetStats(1);
    RecPlot_Phi_1p->Sumw2();
    RecPlot_Phi_1p->Rebin(2);

    TH1D *TruthPlot_Phi_1p = (TH1D *) f->Get("FD TL #phi^{truth}_{p} AC (1p, FD)");
    TruthPlot_Phi_1p->SetLineColor(kBlue);
    TruthPlot_Phi_1p->SetStats(1);
    TruthPlot_Phi_1p->Sumw2();
    TruthPlot_Phi_1p->Rebin(2);

    TH1D *EffPlot_Phi_1p = (TH1D *) f->Get("Proton phi #epsilon_{eff} (1p)");
    EffPlot_Phi_1p->SetLineColor(kBlue);

//    EffPlot_Phi_1p->Sumw2();
//    EffPlot_Phi_1p->Rebin(2);
//    EffPlot_Phi_1p->Divide(TruthPlot_Phi_1p);

    TH1D *RecPlot_Phi_1n = (TH1D *) f->Get("#phi_{n} (All Int., 1n, FD)");
    RecPlot_Phi_1n->SetLineColor(kBlue);
    RecPlot_Phi_1n->SetStats(1);
    RecPlot_Phi_1n->Sumw2();
    RecPlot_Phi_1n->Rebin(2);

    TH1D *TruthPlot_Phi_1n = (TH1D *) f->Get("FD TL #phi^{truth}_{n} AC (1n, FD)");
    TruthPlot_Phi_1n->SetLineColor(kBlue);
    TruthPlot_Phi_1n->SetStats(1);
    TruthPlot_Phi_1n->Sumw2();
    TruthPlot_Phi_1n->Rebin(2);

    TH1D *EffPlot_Phi_1n = (TH1D *) f->Get("Neutron phi #epsilon_{eff} (1n)");
    EffPlot_Phi_1n->SetLineColor(kBlue);

//    EffPlot_Phi_1n->Sumw2();
//    EffPlot_Phi_1n->Rebin(2);
//    EffPlot_Phi_1n->Divide(TruthPlot_Phi_1n);

    if (!RecPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!EffPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasEff2x3Nucleon->cd(1);
    CanvasEff2x3Nucleon->cd(1)->SetGrid();
    CanvasEff2x3Nucleon->cd(1)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(1)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(1)->SetRightMargin(0.12);
    RecPlot_Phi_1p->Draw();

    CanvasEff2x3Nucleon->cd(2);
    CanvasEff2x3Nucleon->cd(2)->SetGrid();
    CanvasEff2x3Nucleon->cd(2)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(2)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(2)->SetRightMargin(0.12);
    TruthPlot_Phi_1p->Draw();

    CanvasEff2x3Nucleon->cd(3);
    CanvasEff2x3Nucleon->cd(3)->SetGrid();
    CanvasEff2x3Nucleon->cd(3)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(3)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(3)->SetRightMargin(0.12);
    EffPlot_Phi_1p->Draw();

    CanvasEff2x3Nucleon->cd(4);
    CanvasEff2x3Nucleon->cd(4)->SetGrid();
    CanvasEff2x3Nucleon->cd(4)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(4)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(4)->SetRightMargin(0.12);
    RecPlot_Phi_1n->Draw();

    CanvasEff2x3Nucleon->cd(5);
    CanvasEff2x3Nucleon->cd(5)->SetGrid();
    CanvasEff2x3Nucleon->cd(5)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(5)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(5)->SetRightMargin(0.12);
    TruthPlot_Phi_1n->Draw();

    CanvasEff2x3Nucleon->cd(6);
    CanvasEff2x3Nucleon->cd(6)->SetGrid();
    CanvasEff2x3Nucleon->cd(6)->SetBottomMargin(0.14);
    CanvasEff2x3Nucleon->cd(6)->SetLeftMargin(0.17);
    CanvasEff2x3Nucleon->cd(6)->SetRightMargin(0.12);
    EffPlot_Phi_1n->Draw();

    CanvasEff2x3Nucleon->SaveAs(PhiEffSaveName.c_str());
    CanvasEff2x3Nucleon->Clear();
    //</editor-fold>

    cout << "\n";

}

