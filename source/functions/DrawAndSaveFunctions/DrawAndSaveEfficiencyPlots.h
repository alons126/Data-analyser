//
// Created by alons on 30/04/2023.
//

#ifndef DRAWANDSAVEEFFICIENCYPLOTS_H
#define DRAWANDSAVEEFFICIENCYPLOTS_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

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

#include "../GeneralFunctions.h"
#include "../EventProperties/GetParticleName.h"
#include "../EventProperties/GetParticleNameLC.h"
#include "../EventProperties/GetParticleNameShort.h"
#include "../../classes/hPlots/hPlot1D.h"

using namespace std;

void DrawAndSaveEfficiencyPlots(string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list) {
    bool weighted_plots = true;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
//    Canvas->cd();
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    TH1D *Histogram1D_REC = RPlot.GetHistogram();
    string RPlot_Clone_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_StatsTitle).c_str());
    string RPlot_Clone_test_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_StatsTitle).c_str());
    string RPlot_Clone_test_rebined_StatsTitle = "reco. " + RPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    RPlot_Clone_test_rebined->Rebin(2);

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    string TLPlot_Clone_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    string TLPlot_Clone_test_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    TLPlot_Clone_test_rebined->Rebin(2);
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    string EfficiencyRecTitle = RPlot_Clone->GetTitle();
    string EfficiencyParticle = GetParticleName(EfficiencyRecTitle);
    string EfficiencyParticleLC = GetParticleNameLC(EfficiencyRecTitle);
    string EfficiencyParticleShort = GetParticleNameShort(EfficiencyRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string EfficiencyType;

    if (findSubstring(EfficiencyRecTitle, "momentum")) { // for momentum efficiency plots
        EfficiencyType = "momentum";
    }

    string EfficiencyTitle = EfficiencyParticle + " " + EfficiencyType;
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string EfficiencyXLabel;

    if (findSubstring(EfficiencyRecTitle, "momentum")) { // for momentum efficiency plots
        EfficiencyXLabel = "P_{" + EfficiencyParticleShort + "} [GeV/c]";
    } else if (findSubstring(EfficiencyRecTitle, "#theta")) { // for momentum efficiency plots
        EfficiencyXLabel = "#theta [Deg]";
    } else if (findSubstring(EfficiencyRecTitle, "#phi")) { // for momentum efficiency plots
        EfficiencyXLabel = "#phi [Deg]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string EfficiencyYLabel = "#epsilon_{eff} = #frac{reco.}{truth}";
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    string EfficiencyFS;

    if (findSubstring(EfficiencyRecTitle, "1e_cut") || findSubstring(EfficiencyRecTitle, "1e cut") || findSubstring(EfficiencyRecTitle, "1e Cut")) {
        EfficiencyFS = "1e cut";
    } else if (findSubstring(EfficiencyRecTitle, "1p")) {
        EfficiencyFS = "1p";
    } else if (findSubstring(EfficiencyRecTitle, "1n")) {
        EfficiencyFS = "1n";
    } else if (findSubstring(EfficiencyRecTitle, "1n1p")) {
        EfficiencyFS = "1n1p";
    } else if (findSubstring(EfficiencyRecTitle, "1e2p")) {
        EfficiencyFS = "1e2p";
    } else if (findSubstring(EfficiencyRecTitle, "2p")) {
        EfficiencyFS = "2p";
    } else if (findSubstring(EfficiencyRecTitle, "pFDpCD")) {
        EfficiencyFS = "pFDpCD";
    } else if (findSubstring(EfficiencyRecTitle, "nFDpCD")) {
        EfficiencyFS = "nFDpCD";
    }
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string EfficiencySaveDir, EfficiencyTestSaveDir;

    if (findSubstring(EfficiencyRecTitle, "Electron") || findSubstring(EfficiencyRecTitle, "electron")) {
        EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
        EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(EfficiencyRecTitle, ", FD)") ||
            findSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticle) ||
            findSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_FD/";
        } else if (findSubstring(EfficiencyRecTitle, ", CD)") ||
                   findSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticle) ||
                   findSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_CD/";
        } else {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + EfficiencySaveDir).c_str());
    system(("mkdir -p " + EfficiencyTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone.png";
    string RPlot_Clone_test_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "01a_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test.png";
    string RPlot_Clone_test_rebined_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "01b_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test_rebined.png";
    string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone.png";
    string TLPlot_Clone_test_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "02a_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test.png";
    string TLPlot_Clone_test_rebined_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "02b_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test_rebined.png";
    string Efficiency_plot_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_" + EfficiencyFS + ".png";
    //</editor-fold>

    TH1D *Efficiency_plot = (TH1D *) Histogram1D_REC->Clone((EfficiencyParticle + " " + EfficiencyType + " #epsilon_{eff}" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->SetTitle((EfficiencyTitle + " efficiency #epsilon_{eff} (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->GetYaxis()->SetTitle((EfficiencyYLabel).c_str());
    Efficiency_plot->GetXaxis()->SetTitle((EfficiencyXLabel).c_str());

    //<editor-fold desc="Plotting and saving RPlot_Clone_test">
    RPlot_Clone_test->SetLineStyle(1);
    RPlot_Clone_test->SetLineColor(kBlue);
    RPlot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test->GetXaxis()->CenterTitle(true);
    RPlot_Clone_test->GetYaxis()->SetTitle("Arbitrary units (#events)");
    RPlot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test->GetYaxis()->CenterTitle(true);
    RPlot_Clone_test->SetLineWidth(2);

    RPlot_Clone_test->Draw();
    RPlot_Clone_test->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test);

    Canvas->SaveAs((RPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test">
    TLPlot_Clone_test->SetLineStyle(1);
    TLPlot_Clone_test->SetLineColor(kBlue);
    TLPlot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test->GetXaxis()->CenterTitle(true);
    TLPlot_Clone_test->GetYaxis()->SetTitle("Arbitrary units (#events)");
    TLPlot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test->GetYaxis()->CenterTitle(true);
    TLPlot_Clone_test->SetLineWidth(2);


    TLPlot_Clone_test->Draw();
    TLPlot_Clone_test->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test);

    Canvas->SaveAs((TLPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone_test_rebined">
    RPlot_Clone_test_rebined->SetLineStyle(1);
    RPlot_Clone_test_rebined->SetLineColor(kBlue);
    RPlot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    RPlot_Clone_test_rebined->GetYaxis()->SetTitle("Arbitrary units (#events)");
    RPlot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    RPlot_Clone_test_rebined->SetLineWidth(2);

    RPlot_Clone_test_rebined->Draw();
    RPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test_rebined);

    Canvas->SaveAs((RPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test_rebined">
    TLPlot_Clone_test_rebined->SetLineStyle(1);
    TLPlot_Clone_test_rebined->SetLineColor(kBlue);
    TLPlot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    TLPlot_Clone_test_rebined->GetYaxis()->SetTitle("Arbitrary units (#events)");
    TLPlot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    TLPlot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    TLPlot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    TLPlot_Clone_test_rebined->SetLineWidth(2);

    TLPlot_Clone_test_rebined->Draw();
    TLPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test_rebined);

    Canvas->SaveAs((TLPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone">
    RPlot_Clone->SetLineStyle(1);
    RPlot_Clone->SetLineColor(kBlue);
    RPlot_Clone->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone->GetXaxis()->CenterTitle(true);
    RPlot_Clone->GetYaxis()->SetTitle("Arbitrary units (#events)");
    RPlot_Clone->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone->GetYaxis()->CenterTitle(true);
    RPlot_Clone->SetLineWidth(2);

    if (weighted_plots) { RPlot_Clone->Sumw2(); }

    RPlot_Clone->Rebin(2);
    RPlot_Clone->Draw();
    RPlot_Clone->SetStats(1);
    Histogram_list->Add(RPlot_Clone);

    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone">
    TLPlot_Clone->SetLineStyle(1);
    TLPlot_Clone->SetLineColor(kBlue);
    TLPlot_Clone->GetXaxis()->SetTitleSize(0.06);
    TLPlot_Clone->GetXaxis()->SetLabelSize(0.0425);
    TLPlot_Clone->GetXaxis()->CenterTitle(true);
    TLPlot_Clone->GetYaxis()->SetTitle("Arbitrary units (#events)");
    TLPlot_Clone->GetYaxis()->SetTitleSize(0.06);
    TLPlot_Clone->GetYaxis()->SetLabelSize(0.0425);
    TLPlot_Clone->GetYaxis()->CenterTitle(true);
    TLPlot_Clone->SetLineWidth(2);

    if (weighted_plots) { TLPlot_Clone->Sumw2(); }

    TLPlot_Clone->Rebin(2);
    TLPlot_Clone->Draw();
    TLPlot_Clone->SetStats(1);
    Histogram_list->Add(TLPlot_Clone);

    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving Efficiency_plot">
    Efficiency_plot->SetLineStyle(1);
    Efficiency_plot->SetLineColor(kBlue);
    Efficiency_plot->GetXaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetXaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetXaxis()->CenterTitle(true);
    Efficiency_plot->GetYaxis()->SetTitle("Arbitrary units (#events)");
    Efficiency_plot->GetYaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetYaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetYaxis()->CenterTitle(true);
    Efficiency_plot->SetLineWidth(2);

    Efficiency_plot->GetYaxis()->SetRangeUser(0., 1.);
/*
    if (EfficiencyFS == "1n") {
        Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.5);
    } else {
        Efficiency_plot->GetYaxis()->SetRangeUser(0., 1.);
    }
*/

    if (weighted_plots) { Efficiency_plot->Sumw2(); }

    Efficiency_plot->Rebin(2);
    Efficiency_plot->Divide(TLPlot_Clone);
    Efficiency_plot->Draw();
    Efficiency_plot->SetStats(0);
    Histogram_list->Add(Efficiency_plot);
    Canvas->SaveAs((Efficiency_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}

void DrawAndSaveEfficiencyPlots(string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list) {
    bool weighted_plots = true;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    string EfficiencyRecTitle = RPlot->GetTitle();
    string EfficiencyParticle = GetParticleName(EfficiencyRecTitle);
    string EfficiencyParticleLC = GetParticleNameLC(EfficiencyRecTitle);
    string EfficiencyParticleShort = GetParticleNameShort(EfficiencyRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    string EfficiencyFS;

    if (findSubstring(EfficiencyRecTitle, "1e_cut") || findSubstring(EfficiencyRecTitle, "1e cut") || findSubstring(EfficiencyRecTitle, "1e Cut")) {
        EfficiencyFS = "1e cut";
    } else if (findSubstring(EfficiencyRecTitle, "1p")) {
        EfficiencyFS = "1p";
    } else if (findSubstring(EfficiencyRecTitle, "1n")) {
        EfficiencyFS = "1n";
    } else if (findSubstring(EfficiencyRecTitle, "1n1p")) {
        EfficiencyFS = "1n1p";
    } else if (findSubstring(EfficiencyRecTitle, "1e2p")) {
        EfficiencyFS = "1e2p";
    } else if (findSubstring(EfficiencyRecTitle, "2p")) {
        EfficiencyFS = "2p";
    } else if (findSubstring(EfficiencyRecTitle, "pFDpCD")) {
        EfficiencyFS = "pFDpCD";
    } else if (findSubstring(EfficiencyRecTitle, "nFDpCD")) {
        EfficiencyFS = "nFDpCD";
    }
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    string EfficiencyStatsType;

    if (findSubstring(EfficiencyRecTitle, "#theta")) { // for momentum efficiency plots
        EfficiencyStatsType = "#theta_{" + EfficiencyParticleShort + "}";
    } else if (findSubstring(EfficiencyRecTitle, "#phi")) { // for momentum efficiency plots
        EfficiencyStatsType = "#phi_{" + EfficiencyParticleShort + "}";
    }

    string EfficiencyStatsTitle = EfficiencyStatsType + " (" + EfficiencyFS + ")";
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string RPlot_Clone_StatsTitle = "reco. " + EfficiencyStatsTitle + " - cloned";
    TH1D *RPlot_Clone = (TH1D *) RPlot->Clone((RPlot_Clone_StatsTitle).c_str());
    string RPlot_Clone_test_StatsTitle = "reco. " + EfficiencyStatsTitle + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *) RPlot->Clone((RPlot_Clone_test_StatsTitle).c_str());
    string RPlot_Clone_test_rebined_StatsTitle = "reco. " + EfficiencyStatsTitle + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *) RPlot->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    RPlot_Clone_test_rebined->Rebin(2);

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    string TLPlot_Clone_StatsTitle = "Truth " + EfficiencyStatsTitle + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    string TLPlot_Clone_test_StatsTitle = "Truth " + EfficiencyStatsTitle + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    TLPlot_Clone_test_rebined->Rebin(2);
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string EfficiencyType, EfficiencyTitle;

    if (findSubstring(EfficiencyRecTitle, "momentum")) { // for momentum efficiency plots
        EfficiencyType = "momentum";
    } else if (findSubstring(EfficiencyRecTitle, "#theta")) { // for theta efficiency plots
        EfficiencyType = "theta";
    } else if (findSubstring(EfficiencyRecTitle, "#phi")) { // for phi efficiency plots
        EfficiencyType = "phi";
    }

    if (EfficiencyType == "momentum") {
        EfficiencyTitle = EfficiencyParticle + " " + EfficiencyType;
    } else {
        EfficiencyTitle = "#" + EfficiencyType + "_{" + EfficiencyParticleShort + "}";
    }

//    string EfficiencyTitle = EfficiencyParticle + " " + EfficiencyType;
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string EfficiencyXLabel;

    if (findSubstring(EfficiencyRecTitle, "momentum")) { // for momentum efficiency plots
        EfficiencyXLabel = "P_{" + EfficiencyParticleShort + "} [GeV/c]";
    } else if (findSubstring(EfficiencyRecTitle, "#theta")) { // for momentum efficiency plots
        EfficiencyXLabel = EfficiencyTitle + " [Deg]";
//        EfficiencyXLabel = "#theta [Deg]";
    } else if (findSubstring(EfficiencyRecTitle, "#phi")) { // for momentum efficiency plots
        EfficiencyXLabel = EfficiencyTitle + " [Deg]";
//        EfficiencyXLabel = "#phi [Deg]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string EfficiencyYLabel = "#epsilon_{eff} = #frac{reco.}{truth}";
//    string EfficiencyYLabel = "#epsilon_{eff} = reco./truth";
//    string EfficiencyYLabel = "#epsilon_{eff} = recotruth";
//    string EfficiencyYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
//    string EfficiencyYLabel = "#epsilon_{eff} = " + xLabel_REC + "/" + xLabel_Truth;
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string EfficiencySaveDir, EfficiencyTestSaveDir;

    if (findSubstring(EfficiencyRecTitle, "Electron") || findSubstring(EfficiencyRecTitle, "electron")) {
        EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
        EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(EfficiencyRecTitle, ", FD)") ||
            findSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticle) ||
            findSubstring(EfficiencyRecTitle, "FD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_FD/";
        } else if (findSubstring(EfficiencyRecTitle, ", CD)") ||
                   findSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticle) ||
                   findSubstring(EfficiencyRecTitle, "CD " + EfficiencyParticleLC)) {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "_CD/";
        } else {
            EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";
            EfficiencyTestSaveDir = EfficiencySaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + EfficiencySaveDir).c_str());
    system(("mkdir -p " + EfficiencyTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone.png";
    string RPlot_Clone_test_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "01a_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test.png";
    string RPlot_Clone_test_rebined_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "01b_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test_rebined.png";
    string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone.png";
    string TLPlot_Clone_test_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "02a_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test.png";
    string TLPlot_Clone_test_rebined_SaveName =
            EfficiencyTestSaveDir + sNameFlag + "02b_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test_rebined.png";
    string Efficiency_plot_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_" + EfficiencyFS + ".png";
//    string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone.png";
//    string RPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone_test.png";
//    string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone.png";
//    string TLPlot_Clone_test_SaveName = EfficiencyTestSaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone_test.png";
//    string Efficiency_plot_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_" + EfficiencyFS + ".png";
    //</editor-fold>

    TH1D *Efficiency_plot = (TH1D *) RPlot_Clone->Clone((EfficiencyParticle + " " + EfficiencyType + " #epsilon_{eff}" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->SetTitle((EfficiencyTitle + " efficiency #epsilon_{eff} (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->GetYaxis()->SetTitle((EfficiencyYLabel).c_str());
    Efficiency_plot->GetXaxis()->SetTitle((EfficiencyXLabel).c_str());

    //<editor-fold desc="Plotting and saving RPlot_Clone_test">
    RPlot_Clone_test->SetLineStyle(1);
    RPlot_Clone_test->SetLineColor(kBlue);
    RPlot_Clone_test->SetStats(1);
    RPlot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test->GetXaxis()->CenterTitle(true);
    RPlot_Clone_test->GetYaxis()->SetTitle("Arbitrary units (#events)");
    RPlot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test->GetYaxis()->CenterTitle(true);
    RPlot_Clone_test->SetLineWidth(2);
    RPlot_Clone_test->Draw();
    RPlot_Clone_test->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test);

    Canvas->SaveAs((RPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test">
    TLPlot_Clone_test->SetLineStyle(1);
    TLPlot_Clone_test->SetLineColor(kBlue);
    TLPlot_Clone_test->Draw();
    TLPlot_Clone_test->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test);

    Canvas->SaveAs((TLPlot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone_test_rebined">
    RPlot_Clone_test_rebined->SetLineStyle(1);
    RPlot_Clone_test_rebined->SetLineColor(kBlue);
    RPlot_Clone_test_rebined->SetStats(1);
    RPlot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    RPlot_Clone_test_rebined->GetYaxis()->SetTitle("Arbitrary units (#events)");
    RPlot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    RPlot_Clone_test_rebined->SetLineWidth(2);
    RPlot_Clone_test_rebined->Draw();
    RPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(RPlot_Clone_test_rebined);

    Canvas->SaveAs((RPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone_test_rebined">
    TLPlot_Clone_test_rebined->SetLineStyle(1);
    TLPlot_Clone_test_rebined->SetLineColor(kBlue);
    TLPlot_Clone_test_rebined->Draw();
    TLPlot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(TLPlot_Clone_test_rebined);

    Canvas->SaveAs((TLPlot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving RPlot_Clone">
    RPlot_Clone->SetLineStyle(1);
    RPlot_Clone->SetLineColor(kBlue);
    RPlot_Clone->SetStats(1);

    RPlot_Clone->GetXaxis()->SetTitleSize(0.06);
    RPlot_Clone->GetXaxis()->SetLabelSize(0.0425);
    RPlot_Clone->GetXaxis()->CenterTitle(true);
    RPlot_Clone->GetYaxis()->SetTitle("Arbitrary units (#events)");
    RPlot_Clone->GetYaxis()->SetTitleSize(0.06);
    RPlot_Clone->GetYaxis()->SetLabelSize(0.0425);
    RPlot_Clone->GetYaxis()->CenterTitle(true);
    RPlot_Clone->SetLineWidth(2);

    if (weighted_plots) { RPlot_Clone->Sumw2(); }

    RPlot_Clone->Rebin(2);
    RPlot_Clone->Draw();
    RPlot_Clone->SetStats(1);
    Histogram_list->Add(RPlot_Clone);

    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone">
    TLPlot_Clone->SetLineStyle(1);
    TLPlot_Clone->SetLineColor(kBlue);

    if (weighted_plots) { TLPlot_Clone->Sumw2(); }

    TLPlot_Clone->Rebin(2);
    TLPlot_Clone->Draw();
    TLPlot_Clone->SetStats(1);
    Histogram_list->Add(TLPlot_Clone);

    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving Efficiency_plot">
    Efficiency_plot->SetLineStyle(1);
    Efficiency_plot->SetLineColor(kBlue);
    Efficiency_plot->GetXaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetXaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetXaxis()->CenterTitle(true);
    Efficiency_plot->GetYaxis()->SetTitleSize(0.06);
    Efficiency_plot->GetYaxis()->SetLabelSize(0.0425);
    Efficiency_plot->GetYaxis()->CenterTitle(true);
    Efficiency_plot->SetLineWidth(2);

    if (EfficiencyFS == "1n") {
        Efficiency_plot->GetYaxis()->SetRangeUser(0., 0.4);
    } else {
        Efficiency_plot->GetYaxis()->SetRangeUser(0., 1.);
    }

    if (weighted_plots) { Efficiency_plot->Sumw2(); }

    Efficiency_plot->Rebin(2);
    Efficiency_plot->Divide(TLPlot_Clone);
    Efficiency_plot->SetStats(0);
    Efficiency_plot->Draw();

    Histogram_list->Add(Efficiency_plot);
    Canvas->SaveAs((Efficiency_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}

#endif //DRAWANDSAVEEFFICIENCYPLOTS_H