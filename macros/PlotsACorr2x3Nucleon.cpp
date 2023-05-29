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

#include "../source/functions/findSubstring.h"

using namespace std;

void PlotsACorr2x3Nucleon(string SampleName) {
    cout << "\nSaving acceptance correction plots for Nucleons...\n\n";

    //<editor-fold desc="Setting file">
    TFile *f;

    if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_10")) {
        if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_100")) {
        if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_250")) {
        if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5")) {
        if (SampleName == "C12_simulation_6GeV_T5_noBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_noBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_noBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_noBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
        }
    } else if (findSubstring(SampleName, "C12_simulation_G18_2GeV")) {
        if (SampleName == "C12_simulation_G18_2GeV_noBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_noFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_noBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_noFDph_Eff2/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_noBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_wFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_noBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_wFDph_Eff2/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_noFDph_Eff1") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_noFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_noFDph_Eff2") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_noFDph_Eff2/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_wFDph_Eff1") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_wFDph_Eff1/reconhipo_plots.root");
        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_wFDph_Eff2") {
            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_wFDph_Eff2/reconhipo_plots.root");
        }
    }
    //</editor-fold>

    //<editor-fold desc="Setting CanvasACorr2x3Nucleon">
    TCanvas *CanvasACorr2x3Nucleon = new TCanvas("CanvasACorr2x3Nucleon", "CanvasACorr2x3Nucleon", 1000 * 3, 750 * 2); // normal res

//    CanvasACorr2x3Nucleon->cd();
//    CanvasACorr2x3Nucleon->Divide(3, 2);
    //</editor-fold>

    //<editor-fold desc="Momentum acceptance correction plots">
    string MomACorrSaveName = "./01_Nucleon_Mom_ACorr_" + SampleName + ".png";

    CanvasACorr2x3Nucleon->Divide(3, 2);

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

    TH1D *ACorrPlot_mom_1p = (TH1D *) f->Get("FD TL Proton momentum AC (1p, FD)");
    TH1D *ACorrPlot_mom_1p_Clone = (TH1D *) ACorrPlot_mom_1p->Clone("Proton momentum #alpha (1p)");
    ACorrPlot_mom_1p_Clone->SetLineColor(kBlue);
    ACorrPlot_mom_1p_Clone->SetStats(0);
    ACorrPlot_mom_1p_Clone->Sumw2();
//    ACorrPlot_mom_1p_Clone->Rebin(2);
    ACorrPlot_mom_1p_Clone->Divide(RecPlot_mom_1p);

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

    TH1D *ACorrPlot_mom_1n = (TH1D *) f->Get("Neutron momentum #alpha (1n)");
    TH1D *ACorrPlot_mom_1n_Clone = (TH1D *) ACorrPlot_mom_1n->Clone("Neutron momentum #alpha (1n)");
    ACorrPlot_mom_1n_Clone->SetLineColor(kBlue);
    ACorrPlot_mom_1n_Clone->SetStats(0);
    ACorrPlot_mom_1n_Clone->Sumw2();
//    ACorrPlot_mom_1n_Clone->Rebin(2);
    ACorrPlot_mom_1n_Clone->Divide(TruthPlot_mom_1n);

    if (!RecPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_mom_1p_Clone) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_mom_1n_Clone) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasACorr2x3Nucleon->cd(1);
    CanvasACorr2x3Nucleon->cd(1)->SetGrid();
    CanvasACorr2x3Nucleon->cd(1)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(1)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(1)->SetRightMargin(0.12);
    RecPlot_mom_1p->Draw();

    CanvasACorr2x3Nucleon->cd(2);
    CanvasACorr2x3Nucleon->cd(2)->SetGrid();
    CanvasACorr2x3Nucleon->cd(2)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(2)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(2)->SetRightMargin(0.12);
    TruthPlot_mom_1p->Draw();

    CanvasACorr2x3Nucleon->cd(3);
    CanvasACorr2x3Nucleon->cd(3)->SetGrid();
    CanvasACorr2x3Nucleon->cd(3)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(3)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(3)->SetRightMargin(0.12);
    ACorrPlot_mom_1p_Clone->Draw();

    CanvasACorr2x3Nucleon->cd(4);
    CanvasACorr2x3Nucleon->cd(4)->SetGrid();
    CanvasACorr2x3Nucleon->cd(4)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(4)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(4)->SetRightMargin(0.12);
    RecPlot_mom_1n->Draw();

    CanvasACorr2x3Nucleon->cd(5);
    CanvasACorr2x3Nucleon->cd(5)->SetGrid();
    CanvasACorr2x3Nucleon->cd(5)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(5)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(5)->SetRightMargin(0.12);
    TruthPlot_mom_1n->Draw();

    CanvasACorr2x3Nucleon->cd(6);
    CanvasACorr2x3Nucleon->cd(6)->SetGrid();
    CanvasACorr2x3Nucleon->cd(6)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(6)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(6)->SetRightMargin(0.12);
    ACorrPlot_mom_1n_Clone->Draw();

    CanvasACorr2x3Nucleon->SaveAs(MomACorrSaveName.c_str());
    CanvasACorr2x3Nucleon->Clear();
    //</editor-fold>

    //<editor-fold desc="Theta acceptance correction plots">
    string ThetaACorrSaveName = "./02_Nucleon_Theta_ACorr_" + SampleName + ".png";

    CanvasACorr2x3Nucleon->Divide(3, 2);

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

    TH1D *ACorrPlot_Theta_1p = (TH1D *) f->Get("Proton theta #alpha (1p)");
    ACorrPlot_Theta_1p->SetLineColor(kBlue);
//    ACorrPlot_Theta_1p->Sumw2();
//    ACorrPlot_Theta_1p->Rebin(2);
//    ACorrPlot_Theta_1p->Divide(TruthPlot_Theta_1p);

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

    TH1D *ACorrPlot_Theta_1n = (TH1D *) f->Get("Neutron theta #alpha (1n)");
    ACorrPlot_Theta_1n->SetLineColor(kBlue);
//    ACorrPlot_Theta_1n->Sumw2();
//    ACorrPlot_Theta_1n->Rebin(2);
//    ACorrPlot_Theta_1n->Divide(TruthPlot_Theta_1n);

    if (!RecPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasACorr2x3Nucleon->cd(1);
    CanvasACorr2x3Nucleon->cd(1)->SetGrid();
    CanvasACorr2x3Nucleon->cd(1)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(1)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(1)->SetRightMargin(0.12);
    RecPlot_Theta_1p->Draw();

    CanvasACorr2x3Nucleon->cd(2);
    CanvasACorr2x3Nucleon->cd(2)->SetGrid();
    CanvasACorr2x3Nucleon->cd(2)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(2)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(2)->SetRightMargin(0.12);
    TruthPlot_Theta_1p->Draw();

    CanvasACorr2x3Nucleon->cd(3);
    CanvasACorr2x3Nucleon->cd(3)->SetGrid();
    CanvasACorr2x3Nucleon->cd(3)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(3)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(3)->SetRightMargin(0.12);
    ACorrPlot_Theta_1p->Draw();

    CanvasACorr2x3Nucleon->cd(4);
    CanvasACorr2x3Nucleon->cd(4)->SetGrid();
    CanvasACorr2x3Nucleon->cd(4)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(4)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(4)->SetRightMargin(0.12);
    RecPlot_Theta_1n->Draw();

    CanvasACorr2x3Nucleon->cd(5);
    CanvasACorr2x3Nucleon->cd(5)->SetGrid();
    CanvasACorr2x3Nucleon->cd(5)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(5)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(5)->SetRightMargin(0.12);
    TruthPlot_Theta_1n->Draw();

    CanvasACorr2x3Nucleon->cd(6);
    CanvasACorr2x3Nucleon->cd(6)->SetGrid();
    CanvasACorr2x3Nucleon->cd(6)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(6)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(6)->SetRightMargin(0.12);
    ACorrPlot_Theta_1n->Draw();

    CanvasACorr2x3Nucleon->SaveAs(ThetaACorrSaveName.c_str());
    CanvasACorr2x3Nucleon->Clear();
    //</editor-fold>

    //<editor-fold desc="Phi acceptance correction plots">
    string PhiACorrSaveName = "./03_Nucleon_Phi_ACorr_" + SampleName + ".png";

    CanvasACorr2x3Nucleon->Divide(3, 2);

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

    TH1D *ACorrPlot_Phi_1p = (TH1D *) f->Get("Proton phi #alpha (1p)");
    ACorrPlot_Phi_1p->SetLineColor(kBlue);

//    ACorrPlot_Phi_1p->Sumw2();
//    ACorrPlot_Phi_1p->Rebin(2);
//    ACorrPlot_Phi_1p->Divide(TruthPlot_Phi_1p);

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

    TH1D *ACorrPlot_Phi_1n = (TH1D *) f->Get("Neutron phi #alpha (1n)");
    ACorrPlot_Phi_1n->SetLineColor(kBlue);

//    ACorrPlot_Phi_1n->Sumw2();
//    ACorrPlot_Phi_1n->Rebin(2);
//    ACorrPlot_Phi_1n->Divide(TruthPlot_Phi_1n);

    if (!RecPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!RecPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!TruthPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
    if (!ACorrPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }

    CanvasACorr2x3Nucleon->cd(1);
    CanvasACorr2x3Nucleon->cd(1)->SetGrid();
    CanvasACorr2x3Nucleon->cd(1)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(1)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(1)->SetRightMargin(0.12);
    RecPlot_Phi_1p->Draw();

    CanvasACorr2x3Nucleon->cd(2);
    CanvasACorr2x3Nucleon->cd(2)->SetGrid();
    CanvasACorr2x3Nucleon->cd(2)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(2)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(2)->SetRightMargin(0.12);
    TruthPlot_Phi_1p->Draw();

    CanvasACorr2x3Nucleon->cd(3);
    CanvasACorr2x3Nucleon->cd(3)->SetGrid();
    CanvasACorr2x3Nucleon->cd(3)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(3)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(3)->SetRightMargin(0.12);
    ACorrPlot_Phi_1p->Draw();

    CanvasACorr2x3Nucleon->cd(4);
    CanvasACorr2x3Nucleon->cd(4)->SetGrid();
    CanvasACorr2x3Nucleon->cd(4)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(4)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(4)->SetRightMargin(0.12);
    RecPlot_Phi_1n->Draw();

    CanvasACorr2x3Nucleon->cd(5);
    CanvasACorr2x3Nucleon->cd(5)->SetGrid();
    CanvasACorr2x3Nucleon->cd(5)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(5)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(5)->SetRightMargin(0.12);
    TruthPlot_Phi_1n->Draw();

    CanvasACorr2x3Nucleon->cd(6);
    CanvasACorr2x3Nucleon->cd(6)->SetGrid();
    CanvasACorr2x3Nucleon->cd(6)->SetBottomMargin(0.14);
    CanvasACorr2x3Nucleon->cd(6)->SetLeftMargin(0.17);
    CanvasACorr2x3Nucleon->cd(6)->SetRightMargin(0.12);
    ACorrPlot_Phi_1n->Draw();

    CanvasACorr2x3Nucleon->SaveAs(PhiACorrSaveName.c_str());
    CanvasACorr2x3Nucleon->Clear();
    //</editor-fold>

    cout << "\n";

}

////<editor-fold desc="GOOD OLD">
//void PlotsACorr2x3Nucleon(string SampleName) {
//    cout << "\nSaving acceptance correction plots for Nucleons...\n\n";
//
//    //<editor-fold desc="Setting file">
//    TFile *f;
//
//    if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_10")) {
//        if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_noBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_10_wBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//        }
//    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_100")) {
//        if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_noBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_100_wBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//        }
//    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5_first_250")) {
//        if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_noBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_first_250_wBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//        }
//    } else if (findSubstring(SampleName, "C12_simulation_6GeV_T5")) {
//        if (SampleName == "C12_simulation_6GeV_T5_noBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_noBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_noBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_noBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_noBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_noFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_noFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_wFDph_Eff1/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        } else if (SampleName == "C12_simulation_6GeV_T5_wBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_wBC_wFDph_Eff2/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//        }
//    } else if (findSubstring(SampleName, "C12_simulation_G18_2GeV")) {
//        if (SampleName == "C12_simulation_G18_2GeV_noBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_noFDph_Eff1/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_noBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_noFDph_Eff2/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_noBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_wFDph_Eff1/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_noBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_noBC_wFDph_Eff2/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_noFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_noFDph_Eff1/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_noFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_noFDph_Eff2/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_wFDph_Eff1") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_wFDph_Eff1/reconhipo_plots.root");
//        } else if (SampleName == "C12_simulation_G18_2GeV_wBC_wFDph_Eff2") {
//            f = new TFile("plots_C12_simulation_G18_2GeV_-03_ALL_CUTS_wBC_wFDph_Eff2/reconhipo_plots.root");
//        }
//    }
//    //</editor-fold>
//
//    //<editor-fold desc="Setting CanvasACorr2x3Nucleon">
//    TCanvas *CanvasACorr2x3Nucleon = new TCanvas("CanvasACorr2x3Nucleon", "CanvasACorr2x3Nucleon", 1000 * 3, 750 * 2); // normal res
//
////    CanvasACorr2x3Nucleon->cd();
////    CanvasACorr2x3Nucleon->Divide(3, 2);
//    //</editor-fold>
//
//    //<editor-fold desc="Momentum acceptance correction plots">
//    string MomACorrSaveName = "./01_Nucleon_Mom_ACorr_" + SampleName + ".png";
//
//    CanvasACorr2x3Nucleon->Divide(3, 2);
//
//    TH1D *RecPlot_mom_1p = (TH1D *) f->Get("Proton momentum APID (1p, FD)");
//    RecPlot_mom_1p->SetLineColor(kBlue);
//    RecPlot_mom_1p->SetStats(1);
//    RecPlot_mom_1p->Sumw2();
//    RecPlot_mom_1p->Rebin(2);
//
//    TH1D *TruthPlot_mom_1p = (TH1D *) f->Get("FD TL Proton momentum AC (1p, FD)");
//    TruthPlot_mom_1p->SetLineColor(kBlue);
//    TruthPlot_mom_1p->SetStats(1);
//    TruthPlot_mom_1p->Sumw2();
//    TruthPlot_mom_1p->Rebin(2);
//
//    TH1D *ACorrPlot_mom_1p = (TH1D *) f->Get("Proton momentum #alpha (1p)");
//    ACorrPlot_mom_1p->SetLineColor(kBlue);
////    ACorrPlot_mom_1p->Sumw2();
////    ACorrPlot_mom_1p->Rebin(2);
////    ACorrPlot_mom_1p->Divide(TruthPlot_mom_1p);
//
//    TH1D *RecPlot_mom_1n = (TH1D *) f->Get("Neutron momentum APID (1n, FD)");
//    RecPlot_mom_1n->SetLineColor(kBlue);
//    RecPlot_mom_1n->SetStats(1);
//    RecPlot_mom_1n->Sumw2();
//    RecPlot_mom_1n->Rebin(2);
//
//    TH1D *TruthPlot_mom_1n = (TH1D *) f->Get("FD TL Neutron momentum AC (1n, FD)");
//    TruthPlot_mom_1n->SetLineColor(kBlue);
//    TruthPlot_mom_1n->SetStats(1);
//    TruthPlot_mom_1n->Sumw2();
//    TruthPlot_mom_1n->Rebin(2);
//
//    TH1D *ACorrPlot_mom_1n = (TH1D *) f->Get("Neutron momentum #alpha (1n)");
//    ACorrPlot_mom_1n->SetLineColor(kBlue);
////    ACorrPlot_mom_1n->Sumw2();
////    ACorrPlot_mom_1n->Rebin(2);
////    ACorrPlot_mom_1n->Divide(TruthPlot_mom_1n);
//
//    if (!RecPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_mom_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!RecPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_mom_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//
//    CanvasACorr2x3Nucleon->cd(1);
//    CanvasACorr2x3Nucleon->cd(1)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(1)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(1)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(1)->SetRightMargin(0.12);
//    RecPlot_mom_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(2);
//    CanvasACorr2x3Nucleon->cd(2)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(2)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(2)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(2)->SetRightMargin(0.12);
//    TruthPlot_mom_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(3);
//    CanvasACorr2x3Nucleon->cd(3)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(3)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(3)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(3)->SetRightMargin(0.12);
//    ACorrPlot_mom_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(4);
//    CanvasACorr2x3Nucleon->cd(4)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(4)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(4)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(4)->SetRightMargin(0.12);
//    RecPlot_mom_1n->Draw();
//
//    CanvasACorr2x3Nucleon->cd(5);
//    CanvasACorr2x3Nucleon->cd(5)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(5)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(5)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(5)->SetRightMargin(0.12);
//    TruthPlot_mom_1n->Draw();
//
//    CanvasACorr2x3Nucleon->cd(6);
//    CanvasACorr2x3Nucleon->cd(6)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(6)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(6)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(6)->SetRightMargin(0.12);
//    ACorrPlot_mom_1n->Draw();
//
//    CanvasACorr2x3Nucleon->SaveAs(MomACorrSaveName.c_str());
//    CanvasACorr2x3Nucleon->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Theta acceptance correction plots">
//    string ThetaACorrSaveName = "./02_Nucleon_Theta_ACorr_" + SampleName + ".png";
//
//    CanvasACorr2x3Nucleon->Divide(3, 2);
//
//    TH1D *RecPlot_Theta_1p = (TH1D *) f->Get("#theta_{p} (All Int., 1p, FD)");
//    RecPlot_Theta_1p->SetLineColor(kBlue);
//    RecPlot_Theta_1p->SetStats(1);
//    RecPlot_Theta_1p->Sumw2();
//    RecPlot_Theta_1p->Rebin(2);
//
//    TH1D *TruthPlot_Theta_1p = (TH1D *) f->Get("FD TL #theta^{truth}_{p} AC (1p, FD)");
//    TruthPlot_Theta_1p->SetLineColor(kBlue);
//    TruthPlot_Theta_1p->SetStats(1);
//    TruthPlot_Theta_1p->Sumw2();
//    TruthPlot_Theta_1p->Rebin(2);
//
//    TH1D *ACorrPlot_Theta_1p = (TH1D *) f->Get("Proton theta #alpha (1p)");
//    ACorrPlot_Theta_1p->SetLineColor(kBlue);
////    ACorrPlot_Theta_1p->Sumw2();
////    ACorrPlot_Theta_1p->Rebin(2);
////    ACorrPlot_Theta_1p->Divide(TruthPlot_Theta_1p);
//
//    TH1D *RecPlot_Theta_1n = (TH1D *) f->Get("#theta_{n} (All Int., 1n, FD)");
//    RecPlot_Theta_1n->SetLineColor(kBlue);
//    RecPlot_Theta_1n->SetStats(1);
//    RecPlot_Theta_1n->Sumw2();
//    RecPlot_Theta_1n->Rebin(2);
//
//    TH1D *TruthPlot_Theta_1n = (TH1D *) f->Get("FD TL #theta^{truth}_{n} AC (1n, FD)");
//    TruthPlot_Theta_1n->SetLineColor(kBlue);
//    TruthPlot_Theta_1n->SetStats(1);
//    TruthPlot_Theta_1n->Sumw2();
//    TruthPlot_Theta_1n->Rebin(2);
//
//    TH1D *ACorrPlot_Theta_1n = (TH1D *) f->Get("Neutron theta #alpha (1n)");
//    ACorrPlot_Theta_1n->SetLineColor(kBlue);
////    ACorrPlot_Theta_1n->Sumw2();
////    ACorrPlot_Theta_1n->Rebin(2);
////    ACorrPlot_Theta_1n->Divide(TruthPlot_Theta_1n);
//
//    if (!RecPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Theta_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!RecPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Theta_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//
//    CanvasACorr2x3Nucleon->cd(1);
//    CanvasACorr2x3Nucleon->cd(1)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(1)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(1)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(1)->SetRightMargin(0.12);
//    RecPlot_Theta_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(2);
//    CanvasACorr2x3Nucleon->cd(2)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(2)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(2)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(2)->SetRightMargin(0.12);
//    TruthPlot_Theta_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(3);
//    CanvasACorr2x3Nucleon->cd(3)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(3)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(3)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(3)->SetRightMargin(0.12);
//    ACorrPlot_Theta_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(4);
//    CanvasACorr2x3Nucleon->cd(4)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(4)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(4)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(4)->SetRightMargin(0.12);
//    RecPlot_Theta_1n->Draw();
//
//    CanvasACorr2x3Nucleon->cd(5);
//    CanvasACorr2x3Nucleon->cd(5)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(5)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(5)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(5)->SetRightMargin(0.12);
//    TruthPlot_Theta_1n->Draw();
//
//    CanvasACorr2x3Nucleon->cd(6);
//    CanvasACorr2x3Nucleon->cd(6)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(6)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(6)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(6)->SetRightMargin(0.12);
//    ACorrPlot_Theta_1n->Draw();
//
//    CanvasACorr2x3Nucleon->SaveAs(ThetaACorrSaveName.c_str());
//    CanvasACorr2x3Nucleon->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Phi acceptance correction plots">
//    string PhiACorrSaveName = "./03_Nucleon_Phi_ACorr_" + SampleName + ".png";
//
//    CanvasACorr2x3Nucleon->Divide(3, 2);
//
//    TH1D *RecPlot_Phi_1p = (TH1D *) f->Get("#phi_{p} (All Int., 1p, FD)");
//    RecPlot_Phi_1p->SetLineColor(kBlue);
//    RecPlot_Phi_1p->SetStats(1);
//    RecPlot_Phi_1p->Sumw2();
//    RecPlot_Phi_1p->Rebin(2);
//
//    TH1D *TruthPlot_Phi_1p = (TH1D *) f->Get("FD TL #phi^{truth}_{p} AC (1p, FD)");
//    TruthPlot_Phi_1p->SetLineColor(kBlue);
//    TruthPlot_Phi_1p->SetStats(1);
//    TruthPlot_Phi_1p->Sumw2();
//    TruthPlot_Phi_1p->Rebin(2);
//
//    TH1D *ACorrPlot_Phi_1p = (TH1D *) f->Get("Proton phi #alpha (1p)");
//    ACorrPlot_Phi_1p->SetLineColor(kBlue);
//
////    ACorrPlot_Phi_1p->Sumw2();
////    ACorrPlot_Phi_1p->Rebin(2);
////    ACorrPlot_Phi_1p->Divide(TruthPlot_Phi_1p);
//
//    TH1D *RecPlot_Phi_1n = (TH1D *) f->Get("#phi_{n} (All Int., 1n, FD)");
//    RecPlot_Phi_1n->SetLineColor(kBlue);
//    RecPlot_Phi_1n->SetStats(1);
//    RecPlot_Phi_1n->Sumw2();
//    RecPlot_Phi_1n->Rebin(2);
//
//    TH1D *TruthPlot_Phi_1n = (TH1D *) f->Get("FD TL #phi^{truth}_{n} AC (1n, FD)");
//    TruthPlot_Phi_1n->SetLineColor(kBlue);
//    TruthPlot_Phi_1n->SetStats(1);
//    TruthPlot_Phi_1n->Sumw2();
//    TruthPlot_Phi_1n->Rebin(2);
//
//    TH1D *ACorrPlot_Phi_1n = (TH1D *) f->Get("Neutron phi #alpha (1n)");
//    ACorrPlot_Phi_1n->SetLineColor(kBlue);
//
////    ACorrPlot_Phi_1n->Sumw2();
////    ACorrPlot_Phi_1n->Rebin(2);
////    ACorrPlot_Phi_1n->Divide(TruthPlot_Phi_1n);
//
//    if (!RecPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Phi_1p) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!RecPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!TruthPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//    if (!ACorrPlot_Phi_1n) { cout << "\n\nEmpty hist\n\n\n"; }
//
//    CanvasACorr2x3Nucleon->cd(1);
//    CanvasACorr2x3Nucleon->cd(1)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(1)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(1)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(1)->SetRightMargin(0.12);
//    RecPlot_Phi_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(2);
//    CanvasACorr2x3Nucleon->cd(2)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(2)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(2)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(2)->SetRightMargin(0.12);
//    TruthPlot_Phi_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(3);
//    CanvasACorr2x3Nucleon->cd(3)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(3)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(3)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(3)->SetRightMargin(0.12);
//    ACorrPlot_Phi_1p->Draw();
//
//    CanvasACorr2x3Nucleon->cd(4);
//    CanvasACorr2x3Nucleon->cd(4)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(4)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(4)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(4)->SetRightMargin(0.12);
//    RecPlot_Phi_1n->Draw();
//
//    CanvasACorr2x3Nucleon->cd(5);
//    CanvasACorr2x3Nucleon->cd(5)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(5)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(5)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(5)->SetRightMargin(0.12);
//    TruthPlot_Phi_1n->Draw();
//
//    CanvasACorr2x3Nucleon->cd(6);
//    CanvasACorr2x3Nucleon->cd(6)->SetGrid();
//    CanvasACorr2x3Nucleon->cd(6)->SetBottomMargin(0.14);
//    CanvasACorr2x3Nucleon->cd(6)->SetLeftMargin(0.17);
//    CanvasACorr2x3Nucleon->cd(6)->SetRightMargin(0.12);
//    ACorrPlot_Phi_1n->Draw();
//
//    CanvasACorr2x3Nucleon->SaveAs(PhiACorrSaveName.c_str());
//    CanvasACorr2x3Nucleon->Clear();
//    //</editor-fold>
//
//    cout << "\n";
//
//}
////</editor-fold>

