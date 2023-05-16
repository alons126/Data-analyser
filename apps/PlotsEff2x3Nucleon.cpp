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

void PlotsEff2x3Nucleon() {

/*
////void PlotsEff2x3Nucleon(string SampleName) {
//
//    TFile *f;
//
//    if (SampleName == "C12_simulation_6GeV_T5_first_10)") {
//        f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
////        f = new TFile("/home/alon/project/plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//    } else if (SampleName == "C12_simulation_6GeV_T5_first_100)"){
//        f = new TFile("/home/alon/project/plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    } else if (SampleName == "C12_simulation_6GeV_T5_first_250)"){
//        f = new TFile("/home/alon/project/plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//    } else if (SampleName == "C12_simulation_6GeV_T5)"){
//        f = new TFile("/home/alon/project/plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    }
//
//    if (!f) { cout << "\n\nEmpty file!\n\n\n"; }
*/

    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
    string SampleName = "C12_simulation_6GeV_T5_first_10";

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_100";

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_250";

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_WithBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5";

    cout << "\nSample is:\t" << SampleName << "\n\n";

    //<editor-fold desc="Momentum efficiency plots">
    string MomEffMomEffSaveName;

    if (SampleName == "C12_simulation_6GeV_T5_first_10") {
        MomEffMomEffSaveName = "./01_MomentumEff10Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_100") {
        MomEffMomEffSaveName = "./01_MomentumEff100Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_250") {
        MomEffMomEffSaveName = "./01_MomentumEff250Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5") {
        MomEffMomEffSaveName = "./01_MomentumEffNucleon.png";
    }

    TCanvas *Canvas2x6 = new TCanvas("Canvas2x6", "Canvas2x6", 1000 * 3, 750 * 2); // normal res

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas2x6->Divide(3, 2);

    TH1D *RecPlot_mom_1p = (TH1D *) f->Get("Proton momentum APID (1p, FD)");
    RecPlot_mom_1p->SetLineColor(kBlue);
    RecPlot_mom_1p->SetStats(1);
    RecPlot_mom_1p->Sumw2();
    RecPlot_mom_1p->Rebin(2);

    TH1D *TruthPlot_mom_1p = (TH1D *) f->Get("TL Proton momentum AC (1p, FD)");
    TruthPlot_mom_1p->SetLineColor(kBlue);
    TruthPlot_mom_1p->SetStats(1);
    TruthPlot_mom_1p->Sumw2();
    TruthPlot_mom_1p->Rebin(2);

    TH1D *EffPlot_mom_1p = (TH1D *) f->Get("Proton momentum #epsilon (1p)");
    EffPlot_mom_1p->SetLineColor(kBlue);
//    EffPlot_mom_1p->Sumw2();
//    EffPlot_mom_1p->Rebin(2);
//    EffPlot_mom_1p->Divide(TruthPlot_mom_1p);

    TH1D *RecPlot_mom_1n = (TH1D *) f->Get("Neutron momentum APID (1n, FD)");
    RecPlot_mom_1n->SetLineColor(kBlue);
    RecPlot_mom_1n->SetStats(1);
    RecPlot_mom_1n->Sumw2();
    RecPlot_mom_1n->Rebin(2);

    TH1D *TruthPlot_mom_1n = (TH1D *) f->Get("TL Neutron momentum AC (1n, FD)");
    TruthPlot_mom_1n->SetLineColor(kBlue);
    TruthPlot_mom_1n->SetStats(1);
    TruthPlot_mom_1n->Sumw2();
    TruthPlot_mom_1n->Rebin(2);

    TH1D *EffPlot_mom_1n = (TH1D *) f->Get("Neutron momentum #epsilon (1n)");
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

    Canvas2x6->cd(1);
    Canvas2x6->cd(1)->SetGrid();
    Canvas2x6->cd(1)->SetBottomMargin(0.14);
    Canvas2x6->cd(1)->SetLeftMargin(0.17);
    Canvas2x6->cd(1)->SetRightMargin(0.12);
    RecPlot_mom_1p->Draw();

    Canvas2x6->cd(2);
    Canvas2x6->cd(2)->SetGrid();
    Canvas2x6->cd(2)->SetBottomMargin(0.14);
    Canvas2x6->cd(2)->SetLeftMargin(0.17);
    Canvas2x6->cd(2)->SetRightMargin(0.12);
    TruthPlot_mom_1p->Draw();

    Canvas2x6->cd(3);
    Canvas2x6->cd(3)->SetGrid();
    Canvas2x6->cd(3)->SetBottomMargin(0.14);
    Canvas2x6->cd(3)->SetLeftMargin(0.17);
    Canvas2x6->cd(3)->SetRightMargin(0.12);
    EffPlot_mom_1p->Draw();

    Canvas2x6->cd(4);
    Canvas2x6->cd(4)->SetGrid();
    Canvas2x6->cd(4)->SetBottomMargin(0.14);
    Canvas2x6->cd(4)->SetLeftMargin(0.17);
    Canvas2x6->cd(4)->SetRightMargin(0.12);
    RecPlot_mom_1n->Draw();

    Canvas2x6->cd(5);
    Canvas2x6->cd(5)->SetGrid();
    Canvas2x6->cd(5)->SetBottomMargin(0.14);
    Canvas2x6->cd(5)->SetLeftMargin(0.17);
    Canvas2x6->cd(5)->SetRightMargin(0.12);
    TruthPlot_mom_1n->Draw();

    Canvas2x6->cd(6);
    Canvas2x6->cd(6)->SetGrid();
    Canvas2x6->cd(6)->SetBottomMargin(0.14);
    Canvas2x6->cd(6)->SetLeftMargin(0.17);
    Canvas2x6->cd(6)->SetRightMargin(0.12);
    EffPlot_mom_1n->Draw();

    Canvas2x6->SaveAs(MomEffMomEffSaveName.c_str());
    Canvas2x6->Clear();
    //</editor-fold>

    //<editor-fold desc="Theta efficiency plots">
    string ThetaEffSaveName;

    if (SampleName == "C12_simulation_6GeV_T5_first_10") {
        ThetaEffSaveName = "./02_ThetaEff10Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_100") {
        ThetaEffSaveName = "./02_ThetaEff100Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_250") {
        ThetaEffSaveName = "./02_ThetaEff250Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5") {
        ThetaEffSaveName = "./02_ThetaEffNucleon.png";
    }

    Canvas2x6->Divide(3, 2);

    TH1D *RecPlot_Theta_1p = (TH1D *) f->Get("#theta_{p} (All Int., 1p, FD)");
    RecPlot_Theta_1p->SetLineColor(kBlue);
    RecPlot_Theta_1p->SetStats(1);
    RecPlot_Theta_1p->Sumw2();
    RecPlot_Theta_1p->Rebin(2);

    TH1D *TruthPlot_Theta_1p = (TH1D *) f->Get("TL #theta^{truth}_{p} AC (1p, FD)");
    TruthPlot_Theta_1p->SetLineColor(kBlue);
    TruthPlot_Theta_1p->SetStats(1);
    TruthPlot_Theta_1p->Sumw2();
    TruthPlot_Theta_1p->Rebin(2);

    TH1D *EffPlot_Theta_1p = (TH1D *) f->Get("Proton theta #epsilon (1p)");
    EffPlot_Theta_1p->SetLineColor(kBlue);
//    EffPlot_Theta_1p->Sumw2();
//    EffPlot_Theta_1p->Rebin(2);
//    EffPlot_Theta_1p->Divide(TruthPlot_Theta_1p);

    TH1D *RecPlot_Theta_1n = (TH1D *) f->Get("#theta_{n} (All Int., 1n, FD)");
    RecPlot_Theta_1n->SetLineColor(kBlue);
    RecPlot_Theta_1n->SetStats(1);
    RecPlot_Theta_1n->Sumw2();
    RecPlot_Theta_1n->Rebin(2);

    TH1D *TruthPlot_Theta_1n = (TH1D *) f->Get("TL #theta^{truth}_{n} AC (1n, FD)");
    TruthPlot_Theta_1n->SetLineColor(kBlue);
    TruthPlot_Theta_1n->SetStats(1);
    TruthPlot_Theta_1n->Sumw2();
    TruthPlot_Theta_1n->Rebin(2);

    TH1D *EffPlot_Theta_1n = (TH1D *) f->Get("Neutron theta #epsilon (1n)");
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

    Canvas2x6->cd(1);
    Canvas2x6->cd(1)->SetGrid();
    Canvas2x6->cd(1)->SetBottomMargin(0.14);
    Canvas2x6->cd(1)->SetLeftMargin(0.17);
    Canvas2x6->cd(1)->SetRightMargin(0.12);
    RecPlot_Theta_1p->Draw();

    Canvas2x6->cd(2);
    Canvas2x6->cd(2)->SetGrid();
    Canvas2x6->cd(2)->SetBottomMargin(0.14);
    Canvas2x6->cd(2)->SetLeftMargin(0.17);
    Canvas2x6->cd(2)->SetRightMargin(0.12);
    TruthPlot_Theta_1p->Draw();

    Canvas2x6->cd(3);
    Canvas2x6->cd(3)->SetGrid();
    Canvas2x6->cd(3)->SetBottomMargin(0.14);
    Canvas2x6->cd(3)->SetLeftMargin(0.17);
    Canvas2x6->cd(3)->SetRightMargin(0.12);
    EffPlot_Theta_1p->Draw();

    Canvas2x6->cd(4);
    Canvas2x6->cd(4)->SetGrid();
    Canvas2x6->cd(4)->SetBottomMargin(0.14);
    Canvas2x6->cd(4)->SetLeftMargin(0.17);
    Canvas2x6->cd(4)->SetRightMargin(0.12);
    RecPlot_Theta_1n->Draw();

    Canvas2x6->cd(5);
    Canvas2x6->cd(5)->SetGrid();
    Canvas2x6->cd(5)->SetBottomMargin(0.14);
    Canvas2x6->cd(5)->SetLeftMargin(0.17);
    Canvas2x6->cd(5)->SetRightMargin(0.12);
    TruthPlot_Theta_1n->Draw();

    Canvas2x6->cd(6);
    Canvas2x6->cd(6)->SetGrid();
    Canvas2x6->cd(6)->SetBottomMargin(0.14);
    Canvas2x6->cd(6)->SetLeftMargin(0.17);
    Canvas2x6->cd(6)->SetRightMargin(0.12);
    EffPlot_Theta_1n->Draw();

    Canvas2x6->SaveAs(ThetaEffSaveName.c_str());
    Canvas2x6->Clear();
    //</editor-fold>

    //<editor-fold desc="Phi efficiency plots">
    string PhiEffSaveName;

    if (SampleName == "C12_simulation_6GeV_T5_first_10") {
        PhiEffSaveName = "./03_PhiEff10Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_100") {
        PhiEffSaveName = "./03_PhiEff100Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5_first_250") {
        PhiEffSaveName = "./03_PhiEff250Nucleon.png";
    } else if (SampleName == "C12_simulation_6GeV_T5") {
        PhiEffSaveName = "./03_PhiEffNucleon.png";
    }

    Canvas2x6->Divide(3, 2);

    TH1D *RecPlot_Phi_1p = (TH1D *) f->Get("#phi_{p} (All Int., 1p, FD)");
    RecPlot_Phi_1p->SetLineColor(kBlue);
    RecPlot_Phi_1p->SetStats(1);
    RecPlot_Phi_1p->Sumw2();
    RecPlot_Phi_1p->Rebin(2);

    TH1D *TruthPlot_Phi_1p = (TH1D *) f->Get("TL #phi^{truth}_{p} AC (1p, FD)");
    TruthPlot_Phi_1p->SetLineColor(kBlue);
    TruthPlot_Phi_1p->SetStats(1);
    TruthPlot_Phi_1p->Sumw2();
    TruthPlot_Phi_1p->Rebin(2);

    TH1D *EffPlot_Phi_1p = (TH1D *) f->Get("Proton phi #epsilon (1p)");
    EffPlot_Phi_1p->SetLineColor(kBlue);

//    EffPlot_Phi_1p->Sumw2();
//    EffPlot_Phi_1p->Rebin(2);
//    EffPlot_Phi_1p->Divide(TruthPlot_Phi_1p);

    TH1D *RecPlot_Phi_1n = (TH1D *) f->Get("#phi_{n} (All Int., 1n, FD)");
    RecPlot_Phi_1n->SetLineColor(kBlue);
    RecPlot_Phi_1n->SetStats(1);
    RecPlot_Phi_1n->Sumw2();
    RecPlot_Phi_1n->Rebin(2);

    TH1D *TruthPlot_Phi_1n = (TH1D *) f->Get("TL #phi^{truth}_{n} AC (1n, FD)");
    TruthPlot_Phi_1n->SetLineColor(kBlue);
    TruthPlot_Phi_1n->SetStats(1);
    TruthPlot_Phi_1n->Sumw2();
    TruthPlot_Phi_1n->Rebin(2);

    TH1D *EffPlot_Phi_1n = (TH1D *) f->Get("Neutron phi #epsilon (1n)");
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

    Canvas2x6->cd(1);
    Canvas2x6->cd(1)->SetGrid();
    Canvas2x6->cd(1)->SetBottomMargin(0.14);
    Canvas2x6->cd(1)->SetLeftMargin(0.17);
    Canvas2x6->cd(1)->SetRightMargin(0.12);
    RecPlot_Phi_1p->Draw();

    Canvas2x6->cd(2);
    Canvas2x6->cd(2)->SetGrid();
    Canvas2x6->cd(2)->SetBottomMargin(0.14);
    Canvas2x6->cd(2)->SetLeftMargin(0.17);
    Canvas2x6->cd(2)->SetRightMargin(0.12);
    TruthPlot_Phi_1p->Draw();

    Canvas2x6->cd(3);
    Canvas2x6->cd(3)->SetGrid();
    Canvas2x6->cd(3)->SetBottomMargin(0.14);
    Canvas2x6->cd(3)->SetLeftMargin(0.17);
    Canvas2x6->cd(3)->SetRightMargin(0.12);
    EffPlot_Phi_1p->Draw();

    Canvas2x6->cd(4);
    Canvas2x6->cd(4)->SetGrid();
    Canvas2x6->cd(4)->SetBottomMargin(0.14);
    Canvas2x6->cd(4)->SetLeftMargin(0.17);
    Canvas2x6->cd(4)->SetRightMargin(0.12);
    RecPlot_Phi_1n->Draw();

    Canvas2x6->cd(5);
    Canvas2x6->cd(5)->SetGrid();
    Canvas2x6->cd(5)->SetBottomMargin(0.14);
    Canvas2x6->cd(5)->SetLeftMargin(0.17);
    Canvas2x6->cd(5)->SetRightMargin(0.12);
    TruthPlot_Phi_1n->Draw();

    Canvas2x6->cd(6);
    Canvas2x6->cd(6)->SetGrid();
    Canvas2x6->cd(6)->SetBottomMargin(0.14);
    Canvas2x6->cd(6)->SetLeftMargin(0.17);
    Canvas2x6->cd(6)->SetRightMargin(0.12);
    EffPlot_Phi_1n->Draw();

    Canvas2x6->SaveAs(PhiEffSaveName.c_str());
    Canvas2x6->Clear();
    //</editor-fold>

}

