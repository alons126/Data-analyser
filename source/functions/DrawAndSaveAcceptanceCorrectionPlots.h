//
// Created by alons on 17/05/2023.
//

#ifndef DRAWANDSAVEACCEPTANCECORRECTIONPLOTS_H
#define DRAWANDSAVEACCEPTANCECORRECTIONPLOTS_H

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

#include "GeneralFunctions.h"
#include "GetParticleProperties/GetParticleName.h"
#include "GetParticleProperties/GetParticleNameLC.h"
#include "GetParticleProperties/GetParticleNameShort.h"
#include "../classes/hPlots/hPlot1D.h"

using namespace std;

void DrawAndSaveAcceptanceCorrectionPlots(string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list) {

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
    string RPlot_Clone_StatsTitle = "Rec. " + RPlot.GetHistogramStatTitle() + " - cloned";
    TH1D *RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_StatsTitle).c_str());
    string RPlot_Clone_test_StatsTitle = "Rec. " + RPlot.GetHistogramStatTitle() + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *) Histogram1D_REC->Clone((RPlot_Clone_test_StatsTitle).c_str());
    string RPlot_Clone_test_rebined_StatsTitle = "Rec. " + RPlot.GetHistogramStatTitle() + " - cloned test rebined";
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
    string ACorrectionRecTitle = RPlot_Clone->GetTitle();
    string ACorrectionParticle = GetParticleName(ACorrectionRecTitle);
    string ACorrectionParticleLC = GetParticleNameLC(ACorrectionRecTitle);
    string ACorrectionParticleShort = GetParticleNameShort(ACorrectionRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string ACorrectionType;

    if (findSubstring(ACorrectionRecTitle, "momentum")) { // for momentum ACorrection plots
        ACorrectionType = "momentum";
    }

    string ACorrectionTitle = ACorrectionParticle + " " + ACorrectionType;
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string ACorrectionXLabel;

    if (findSubstring(ACorrectionRecTitle, "momentum")) { // for momentum ACorrection plots
        ACorrectionXLabel = "P_{" + ACorrectionParticleShort + "} [GeV/c]";
    } else if (findSubstring(ACorrectionRecTitle, "#theta")) { // for momentum ACorrection plots
        ACorrectionXLabel = "#theta [Deg]";
    } else if (findSubstring(ACorrectionRecTitle, "#phi")) { // for momentum ACorrection plots
        ACorrectionXLabel = "#phi [Deg]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string ACorrectionYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
//    string ACorrectionYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    string ACorrectionFS;

    if (findSubstring(ACorrectionRecTitle, "1e_cut") || findSubstring(ACorrectionRecTitle, "1e cut") || findSubstring(ACorrectionRecTitle, "1e Cut")) {
        ACorrectionFS = "1e cut";
    } else if (findSubstring(ACorrectionRecTitle, "1p")) {
        ACorrectionFS = "1p";
    } else if (findSubstring(ACorrectionRecTitle, "1n")) {
        ACorrectionFS = "1n";
    } else if (findSubstring(ACorrectionRecTitle, "1n1p")) {
        ACorrectionFS = "1n1p";
    } else if (findSubstring(ACorrectionRecTitle, "1e2p")) {
        ACorrectionFS = "1e2p";
    } else if (findSubstring(ACorrectionRecTitle, "2p")) {
        ACorrectionFS = "2p";
    } else if (findSubstring(ACorrectionRecTitle, "pFDpCD")) {
        ACorrectionFS = "pFDpCD";
    } else if (findSubstring(ACorrectionRecTitle, "nFDpCD")) {
        ACorrectionFS = "nFDpCD";
    }
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string ACorrectionSaveDir, ACorrectionTestSaveDir;

    if (findSubstring(ACorrectionRecTitle, "Electron") || findSubstring(ACorrectionRecTitle, "electron")) {
        ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
        ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(ACorrectionRecTitle, ", FD)") ||
            findSubstring(ACorrectionParticle, "FD " + ACorrectionParticle) ||
            findSubstring(ACorrectionParticle, "FD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" +
                                 ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else if (findSubstring(ACorrectionRecTitle, ", CD)") ||
                   findSubstring(ACorrectionParticle, "CD " + ACorrectionParticle) ||
                   findSubstring(ACorrectionParticle, "CD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" +
                                 ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + ACorrectionSaveDir).c_str());
    system(("mkdir -p " + ACorrectionTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string RPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone.png";
    string RPlot_Clone_test_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "01a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test.png";
    string RPlot_Clone_test_rebined_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "01b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test_rebined.png";
    string TLPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone.png";
    string TLPlot_Clone_test_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "02a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test.png";
    string TLPlot_Clone_test_rebined_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "02b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test_rebined.png";
    string ACorrection_plot_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_" + ACorrectionFS + ".png";
    //</editor-fold>

    TH1D *ACorrection_plot = (TH1D *) TLPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #alpha" + " (" + ACorrectionFS + ")").c_str());
//    TH1D *ACorrection_plot = (TH1D *) RPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #epsilon_{eff}" + " (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->SetTitle((ACorrectionTitle + " acceptance correction #alpha = 1/#epsilon_{eff} (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->GetYaxis()->SetTitle((ACorrectionYLabel).c_str());
    ACorrection_plot->GetXaxis()->SetTitle((ACorrectionXLabel).c_str());

    //<editor-fold desc="Plotting and saving RPlot_Clone_test">
    RPlot_Clone_test->SetLineStyle(1);
    RPlot_Clone_test->SetLineColor(kBlue);
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

    //<editor-fold desc="Plotting and saving ACorrection_plot">
    ACorrection_plot->SetLineStyle(1);
    ACorrection_plot->SetLineColor(kBlue);

    if (weighted_plots) { ACorrection_plot->Sumw2(); }

    ACorrection_plot->Rebin(2);
    ACorrection_plot->Divide(RPlot_Clone);
//    ACorrection_plot->Divide(TLPlot_Clone);
    ACorrection_plot->Draw();
    ACorrection_plot->SetStats(0);
    Histogram_list->Add(ACorrection_plot);
    Canvas->SaveAs((ACorrection_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}

void DrawAndSaveAcceptanceCorrectionPlots(string &SampleName, const hPlot1D &TLPlot, TH1D *RPlot, TList *Histogram_list) {

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
    string ACorrectionRecTitle = RPlot->GetTitle();
    string ACorrectionParticle = GetParticleName(ACorrectionRecTitle);
    string ACorrectionParticleLC = GetParticleNameLC(ACorrectionRecTitle);
    string ACorrectionParticleShort = GetParticleNameShort(ACorrectionRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    string ACorrectionFS;

    if (findSubstring(ACorrectionRecTitle, "1e_cut") || findSubstring(ACorrectionRecTitle, "1e cut") || findSubstring(ACorrectionRecTitle, "1e Cut")) {
        ACorrectionFS = "1e cut";
    } else if (findSubstring(ACorrectionRecTitle, "1p")) {
        ACorrectionFS = "1p";
    } else if (findSubstring(ACorrectionRecTitle, "1n")) {
        ACorrectionFS = "1n";
    } else if (findSubstring(ACorrectionRecTitle, "1n1p")) {
        ACorrectionFS = "1n1p";
    } else if (findSubstring(ACorrectionRecTitle, "1e2p")) {
        ACorrectionFS = "1e2p";
    } else if (findSubstring(ACorrectionRecTitle, "2p")) {
        ACorrectionFS = "2p";
    } else if (findSubstring(ACorrectionRecTitle, "pFDpCD")) {
        ACorrectionFS = "pFDpCD";
    } else if (findSubstring(ACorrectionRecTitle, "nFDpCD")) {
        ACorrectionFS = "nFDpCD";
    }
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    string ACorrectionStatsType;

    if (findSubstring(ACorrectionRecTitle, "#theta")) { // for momentum ACorrection plots
        ACorrectionStatsType = "#theta_{" + ACorrectionParticleShort + "}";
    } else if (findSubstring(ACorrectionRecTitle, "#phi")) { // for momentum ACorrection plots
        ACorrectionStatsType = "#phi_{" + ACorrectionParticleShort + "}";
    }

    string ACorrectionStatsTitle = ACorrectionStatsType + " (" + ACorrectionFS + ")";
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string RPlot_Clone_StatsTitle = "Rec. " + ACorrectionStatsTitle + " - cloned";
    TH1D *RPlot_Clone = (TH1D *) RPlot->Clone((RPlot_Clone_StatsTitle).c_str());
    string RPlot_Clone_test_StatsTitle = "Rec. " + ACorrectionStatsTitle + " - cloned test";
    TH1D *RPlot_Clone_test = (TH1D *) RPlot->Clone((RPlot_Clone_test_StatsTitle).c_str());
    string RPlot_Clone_test_rebined_StatsTitle = "Rec. " + ACorrectionStatsTitle + " - cloned test rebined";
    TH1D *RPlot_Clone_test_rebined = (TH1D *) RPlot->Clone((RPlot_Clone_test_rebined_StatsTitle).c_str());
    RPlot_Clone_test_rebined->Rebin(2);

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    string TLPlot_Clone_StatsTitle = "Truth " + ACorrectionStatsTitle + " - cloned";
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_StatsTitle).c_str());
    string TLPlot_Clone_test_StatsTitle = "Truth " + ACorrectionStatsTitle + " - cloned test";
    TH1D *TLPlot_Clone_test = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_StatsTitle).c_str());
    string TLPlot_Clone_test_rebined_StatsTitle = "Truth " + TLPlot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *TLPlot_Clone_test_rebined = (TH1D *) Histogram1D_Truth->Clone((TLPlot_Clone_test_rebined_StatsTitle).c_str());
    TLPlot_Clone_test_rebined->Rebin(2);
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string ACorrectionType, ACorrectionTitle;

    if (findSubstring(ACorrectionRecTitle, "momentum")) { // for momentum ACorrection plots
        ACorrectionType = "momentum";
    } else if (findSubstring(ACorrectionRecTitle, "#theta")) { // for theta ACorrection plots
        ACorrectionType = "theta";
    } else if (findSubstring(ACorrectionRecTitle, "#phi")) { // for phi ACorrection plots
        ACorrectionType = "phi";
    }

    if (ACorrectionType == "momentum") {
        ACorrectionTitle = ACorrectionParticle + " " + ACorrectionType;
    } else {
        ACorrectionTitle = "#" + ACorrectionType + "_{" + ACorrectionParticleShort + "}";
    }

//    string ACorrectionTitle = ACorrectionParticle + " " + ACorrectionType;
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string ACorrectionXLabel;

    if (findSubstring(ACorrectionRecTitle, "momentum")) { // for momentum ACorrection plots
        ACorrectionXLabel = "P_{" + ACorrectionParticleShort + "} [GeV/c]";
    } else if (findSubstring(ACorrectionRecTitle, "#theta")) { // for momentum ACorrection plots
        ACorrectionXLabel = ACorrectionTitle + " [Deg]";
//        ACorrectionXLabel = "#theta [Deg]";
    } else if (findSubstring(ACorrectionRecTitle, "#phi")) { // for momentum ACorrection plots
        ACorrectionXLabel = ACorrectionTitle + " [Deg]";
//        ACorrectionXLabel = "#phi [Deg]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string ACorrectionYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
//    string ACorrectionYLabel = "#epsilon_{eff} = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string ACorrectionSaveDir, ACorrectionTestSaveDir;

    if (findSubstring(ACorrectionRecTitle, "Electron") || findSubstring(ACorrectionRecTitle, "electron")) {
        ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/00_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
        ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(ACorrectionRecTitle, ", FD)") ||
            findSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticle) ||
            findSubstring(ACorrectionRecTitle, "FD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/01_FD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_"
                                 + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else if (findSubstring(ACorrectionRecTitle, ", CD)") ||
                   findSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticle) ||
                   findSubstring(ACorrectionRecTitle, "CD " + ACorrectionParticleLC)) {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/02_CD_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_"
                                 + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test/";
        } else {
            ACorrectionSaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_plots_" + ACorrectionFS + "/";
            ACorrectionTestSaveDir = ACorrectionSaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + ACorrectionSaveDir).c_str());
    system(("mkdir -p " + ACorrectionTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string RPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone.png";
    string RPlot_Clone_test_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "01a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test.png";
    string RPlot_Clone_test_rebined_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "01b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Rec_Clone_test_rebined.png";
    string TLPlot_Clone_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone.png";
    string TLPlot_Clone_test_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "02a_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test.png";
    string TLPlot_Clone_test_rebined_SaveName =
            ACorrectionTestSaveDir + sNameFlag + "02b_" + ACorrectionParticle + "_" + ACorrectionType + "_" + ACorrectionFS + "_" + "Truth_Clone_test_rebined.png";
    string ACorrection_plot_SaveName = ACorrectionSaveDir + sNameFlag + "_" + ACorrectionParticle + "_" + ACorrectionType + "_ACorrection_" + ACorrectionFS + ".png";
    //</editor-fold>

    TH1D *ACorrection_plot = (TH1D *) TLPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #alpha" + " (" + ACorrectionFS + ")").c_str());
//    TH1D *ACorrection_plot = (TH1D *) RPlot_Clone->Clone((ACorrectionParticle + " " + ACorrectionType + " #epsilon_{eff}" + " (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->SetTitle((ACorrectionTitle + " acceptance correction #alpha = 1/#epsilon_{eff} (" + ACorrectionFS + ")").c_str());
    ACorrection_plot->GetYaxis()->SetTitle((ACorrectionYLabel).c_str());
    ACorrection_plot->GetXaxis()->SetTitle((ACorrectionXLabel).c_str());

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

    //<editor-fold desc="Plotting and saving ACorrection_plot">
    ACorrection_plot->SetLineStyle(1);
    ACorrection_plot->SetLineColor(kBlue);

    ACorrection_plot->GetXaxis()->SetTitleSize(0.06);
    ACorrection_plot->GetXaxis()->SetLabelSize(0.0425);
    ACorrection_plot->GetXaxis()->CenterTitle(true);
    ACorrection_plot->GetYaxis()->SetTitleSize(0.06);
    ACorrection_plot->GetYaxis()->SetLabelSize(0.0425);
    ACorrection_plot->GetYaxis()->CenterTitle(true);
    ACorrection_plot->SetLineWidth(2);

    if (weighted_plots) { ACorrection_plot->Sumw2(); }

    ACorrection_plot->Rebin(2);
    ACorrection_plot->Divide(RPlot_Clone);
//    ACorrection_plot->Divide(TLPlot_Clone);
    ACorrection_plot->SetStats(0);
    ACorrection_plot->Draw();

    Histogram_list->Add(ACorrection_plot);
    Canvas->SaveAs((ACorrection_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}

#endif //DRAWANDSAVEACCEPTANCECORRECTIONPLOTS_H
