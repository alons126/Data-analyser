//
// Created by alons on 04/06/2023.
//

#ifndef DRAWANDSAVEFSRATIO_H
#define DRAWANDSAVEFSRATIO_H

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
#include "EventProperties/GetParticleName.h"
#include "EventProperties/GetParticleNameLC.h"
#include "EventProperties/GetParticleNameShort.h"
#include "EventProperties/SetFSRatioSaveDir.h"
#include "EventProperties/SetSaveDir.h"
#include "EventProperties/SetType.h"
#include "EventProperties/SetTitle.h"
#include "EventProperties/SetStatsTitle.h"
#include "EventProperties/SettingSaveNames.h"
#include "EventProperties/SetXAxisTitle.h"
#include "EventProperties/SetYAxisTitle.h"
#include "../classes/hPlots/hPlot1D.h"

using namespace std;

void DrawAndSaveFSRatio(string &SampleName, const hPlot1D &pFDpCD_Plot, const hPlot1D &nFDpCD_Plot, TList *Histogram_list) {

    bool weighted_plots = true;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
    Canvas->SetGrid();

    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    TH1D *Histogram1D_nFDpCD = nFDpCD_Plot.GetHistogram();
    string nFDpCD_Plot_Clone_StatsTitle = "Rec. " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned";
    TH1D *nFDpCD_Plot_Clone = (TH1D *) Histogram1D_nFDpCD->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "Rec. " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned test";
    TH1D *nFDpCD_Plot_Clone_test = (TH1D *) Histogram1D_nFDpCD->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "Rec. " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *nFDpCD_Plot_Clone_test_rebined = (TH1D *) Histogram1D_nFDpCD->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    nFDpCD_Plot_Clone_test_rebined->Rebin(2);

    TH1D *Histogram1D_pFDpCD = pFDpCD_Plot.GetHistogram();
    string pFDpCD_Plot_Clone_StatsTitle = "Truth " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned";
    TH1D *pFDpCD_Plot_Clone = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "Truth " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test";
    TH1D *pFDpCD_Plot_Clone_test = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "Truth " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *pFDpCD_Plot_Clone_test_rebined = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    pFDpCD_Plot_Clone_test_rebined->Rebin(2);
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    string FSRatioRecTitle = nFDpCD_Plot_Clone->GetTitle();
    string FSRatioParticle = GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = GetParticleNameShort(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string FSRatioType = SetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioTitle = SetTitle(FSRatioRecTitle);

    /*
    string FSRatioType;

    if (findSubstring(FSRatioRecTitle, "momentum")) { // for momentum FSRatio plots
        FSRatioType = "momentum";
    }

    string FSRatioTitle = FSRatioParticle + " " + FSRatioType;
*/
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);

    /*
    string FSRatioXLabel;

    if (findSubstring(FSRatioRecTitle, "momentum")) { // for momentum FSRatio plots
        FSRatioXLabel = "P_{" + FSRatioParticleShort + "} [GeV/c]";
    } else if (findSubstring(FSRatioRecTitle, "#theta")) { // for momentum FSRatio plots
        FSRatioXLabel = "#theta [Deg]";
    } else if (findSubstring(FSRatioRecTitle, "#phi")) { // for momentum FSRatio plots
        FSRatioXLabel = "#phi [Deg]";
    }
*/
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string FSRatioYLabel = SetYAxisTitle("FSRatio", nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle());

    /*
    string xLabel_REC_temp = nFDpCD_Plot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = pFDpCD_Plot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string FSRatioYLabel = "nFDpCD/pFDpCD";
//    string FSRatioYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
*/
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string FSRatioSaveDir, FSRatioTestSaveDir;
//    SetSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, pFDpCD_Plot.GetHistogram1DSaveNamePath(),
//                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, pFDpCD_Plot.GetHistogram1DSaveNamePath(),
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);

    /*
    string FSRatioSaveDir, FSRatioTestSaveDir;

    if (findSubstring(FSRatioRecTitle, "Electron") || findSubstring(FSRatioRecTitle, "electron")) {
        FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/00_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots/";
        FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(FSRatioRecTitle, ", FD)") ||
            findSubstring(FSRatioRecTitle, "FD " + FSRatioParticle) ||
            findSubstring(FSRatioRecTitle, "FD " + FSRatioParticleLC)) {
            FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/01_FD_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots/";
            FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test/";
        } else if (findSubstring(FSRatioRecTitle, ", CD)") ||
                   findSubstring(FSRatioRecTitle, "CD " + FSRatioParticle) ||
                   findSubstring(FSRatioRecTitle, "CD " + FSRatioParticleLC)) {
            FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/02_CD_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots/";
            FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test/";
        } else {
            FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots/";
            FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + FSRatioSaveDir).c_str());
    system(("mkdir -p " + FSRatioTestSaveDir).c_str());
*/
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_SaveName);

    /*
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string nFDpCD_Plot_Clone_SaveName = FSRatioSaveDir + sNameFlag + "_" + FSRatioParticle + "_" + FSRatioType + "_Rec_Clone.png";
    string nFDpCD_Plot_Clone_test_SaveName = FSRatioTestSaveDir + sNameFlag + "01a_" + FSRatioParticle + "_" + FSRatioType + "_Rec_Clone_test.png";
    string nFDpCD_Plot_Clone_test_rebined_SaveName = FSRatioTestSaveDir + sNameFlag + "01b_" + FSRatioParticle + "_" + FSRatioType + "_Rec_Clone_test_rebined.png";
    string pFDpCD_Plot_Clone_SaveName = FSRatioSaveDir + sNameFlag + "_" + FSRatioParticle + "_" + FSRatioType + "_Truth_Clone.png";
    string pFDpCD_Plot_Clone_test_SaveName = FSRatioTestSaveDir + sNameFlag + "02a_" + FSRatioParticle + "_" + FSRatioType + "_Truth_Clone_test.png";
    string pFDpCD_Plot_Clone_test_rebined_SaveName = FSRatioTestSaveDir + sNameFlag + "02b_" + FSRatioParticle + "_" + FSRatioType + "_Truth_Clone_test_rebined.png";
    string FSRatio_plot_SaveName = FSRatioSaveDir + sNameFlag + "_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio.png";
*/
    //</editor-fold>

    TH1D *FSRatio_plot = (TH1D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + "FSRatio").c_str());
    FSRatio_plot->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    FSRatio_plot->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->SetLineStyle(1);
    nFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test->Draw();
    nFDpCD_Plot_Clone_test->SetStats(1);
    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->SetLineStyle(1);
    pFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test->Draw();
    pFDpCD_Plot_Clone_test->SetStats(1);
    Histogram_list->Add(pFDpCD_Plot_Clone_test);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test_rebined">
    nFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    nFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test_rebined->Draw();
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    pFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test_rebined->Draw();
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone">
    nFDpCD_Plot_Clone->SetLineStyle(1);
    nFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (weighted_plots) { nFDpCD_Plot_Clone->Sumw2(); }

    nFDpCD_Plot_Clone->Rebin(2);
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    pFDpCD_Plot_Clone->SetLineStyle(1);
    pFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (weighted_plots) { pFDpCD_Plot_Clone->Sumw2(); }

    pFDpCD_Plot_Clone->Rebin(2);
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot">
    FSRatio_plot->SetLineStyle(1);
    FSRatio_plot->SetLineColor(kBlue);

    if (weighted_plots) { FSRatio_plot->Sumw2(); }

    FSRatio_plot->Rebin(2);
    FSRatio_plot->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot->Draw();
    FSRatio_plot->SetStats(0);
    Histogram_list->Add(FSRatio_plot);
    Canvas->SaveAs((FSRatio_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}

void DrawAndSaveFSRatio(string &SampleName, const hPlot1D &pFDpCD_Plot, TH1D *nFDpCD_Plot, TList *Histogram_list) {

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
    string FSRatioRecTitle = nFDpCD_Plot->GetTitle();
    string FSRatioParticle = GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = GetParticleNameShort(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    string FSRatioStatsType;

    if (findSubstring(FSRatioRecTitle, "#theta")) { // for momentum FSRatio plots
        FSRatioStatsType = "#theta_{" + FSRatioParticleShort + "}";
    } else if (findSubstring(FSRatioRecTitle, "#phi")) { // for momentum FSRatio plots
        FSRatioStatsType = "#phi_{" + FSRatioParticleShort + "}";
    }

    string FSRatioStatsTitle = FSRatioStatsType;
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string nFDpCD_Plot_Clone_StatsTitle = "Rec. " + FSRatioStatsTitle + " - cloned";
    TH1D *nFDpCD_Plot_Clone = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "Rec. " + FSRatioStatsTitle + " - cloned test";
    TH1D *nFDpCD_Plot_Clone_test = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "Rec. " + FSRatioStatsTitle + " - cloned test rebined";
    TH1D *nFDpCD_Plot_Clone_test_rebined = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    nFDpCD_Plot_Clone_test_rebined->Rebin(2);

    TH1D *Histogram1D_pFDpCD = pFDpCD_Plot.GetHistogram();
    string pFDpCD_Plot_Clone_StatsTitle = "Truth " + FSRatioStatsTitle + " - cloned";
    TH1D *pFDpCD_Plot_Clone = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "Truth " + FSRatioStatsTitle + " - cloned test";
    TH1D *pFDpCD_Plot_Clone_test = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "Truth " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined";
    TH1D *pFDpCD_Plot_Clone_test_rebined = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    pFDpCD_Plot_Clone_test_rebined->Rebin(2);
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string FSRatioType = SetType(FSRatioRecTitle);
    string FSRatioTitle = SetTitle(FSRatioRecTitle);

/*
    string FSRatioType, FSRatioTitle;

    if (findSubstring(FSRatioRecTitle, "momentum")) { // for momentum FSRatio plots
        FSRatioType = "momentum";
    } else if (findSubstring(FSRatioRecTitle, "#theta")) { // for theta FSRatio plots
        FSRatioType = "theta";
    } else if (findSubstring(FSRatioRecTitle, "#phi")) { // for phi FSRatio plots
        FSRatioType = "phi";
    }

    if (FSRatioType == "momentum") {
        FSRatioTitle = FSRatioParticle + " " + FSRatioType;
    } else {
        FSRatioTitle = "#" + FSRatioType + "_{" + FSRatioParticleShort + "}";
    }
*/
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);

    /*
    string FSRatioXLabel;

    if (findSubstring(FSRatioRecTitle, "momentum")) { // for momentum FSRatio plots
        FSRatioXLabel = "P_{" + FSRatioParticleShort + "} [GeV/c]";
    } else if (findSubstring(FSRatioRecTitle, "#theta")) { // for momentum FSRatio plots
        FSRatioXLabel = FSRatioTitle + " [Deg]";
    } else if (findSubstring(FSRatioRecTitle, "#phi")) { // for momentum FSRatio plots
        FSRatioXLabel = FSRatioTitle + " [Deg]";
    }
*/
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string FSRatioYLabel = SetYAxisTitle("FSRatio", nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle());

/*
    string xLabel_REC_temp = nFDpCD_Plot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = pFDpCD_Plot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string FSRatioYLabel = string("#alpha = ") + "#frac{1}{#epsilon_{eff}} = " + xLabel_Truth + "/" + xLabel_REC + "^{rec}";
*/
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string FSRatioSaveDir, FSRatioTestSaveDir;

    if (findSubstring(FSRatioRecTitle, "Electron") || findSubstring(FSRatioRecTitle, "electron")) {
        FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/00_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots_" + "/";
        FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test/";
    } else {
        if (findSubstring(FSRatioRecTitle, ", FD)") ||
            findSubstring(FSRatioRecTitle, "FD " + FSRatioParticle) ||
            findSubstring(FSRatioRecTitle, "FD " + FSRatioParticleLC)) {
            FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/01_FD_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots_" + "/";
            FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test/";
        } else if (findSubstring(FSRatioRecTitle, ", CD)") ||
                   findSubstring(FSRatioRecTitle, "CD " + FSRatioParticle) ||
                   findSubstring(FSRatioRecTitle, "CD " + FSRatioParticleLC)) {
            FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/02_CD_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots_" + "/";
            FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test/";
        } else {
            FSRatioSaveDir = pFDpCD_Plot.GetHistogram1DSaveNamePath() + "/" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_plots_" + "/";
            FSRatioTestSaveDir = FSRatioSaveDir + "Cloned_hist_test" + "/";
        }
    }

    system(("mkdir -p " + FSRatioSaveDir).c_str());
    system(("mkdir -p " + FSRatioTestSaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string nFDpCD_Plot_Clone_SaveName = FSRatioSaveDir + sNameFlag + "_" + FSRatioParticle + "_" + FSRatioType + "_" + "_" + "Rec_Clone.png";
    string nFDpCD_Plot_Clone_test_SaveName =
            FSRatioTestSaveDir + sNameFlag + "01a_" + FSRatioParticle + "_" + FSRatioType + "_" + "_" + "Rec_Clone_test.png";
    string nFDpCD_Plot_Clone_test_rebined_SaveName =
            FSRatioTestSaveDir + sNameFlag + "01b_" + FSRatioParticle + "_" + FSRatioType + "_" + "_" + "Rec_Clone_test_rebined.png";
    string pFDpCD_Plot_Clone_SaveName = FSRatioSaveDir + sNameFlag + "_" + FSRatioParticle + "_" + FSRatioType + "_" + "_" + "Truth_Clone.png";
    string pFDpCD_Plot_Clone_test_SaveName =
            FSRatioTestSaveDir + sNameFlag + "02a_" + FSRatioParticle + "_" + FSRatioType + "_" + "_" + "Truth_Clone_test.png";
    string pFDpCD_Plot_Clone_test_rebined_SaveName =
            FSRatioTestSaveDir + sNameFlag + "02b_" + FSRatioParticle + "_" + FSRatioType + "_" + "_" + "Truth_Clone_test_rebined.png";
    string FSRatio_plot_SaveName = FSRatioSaveDir + sNameFlag + "_" + FSRatioParticle + "_" + FSRatioType + "_FSRatio_" + ".png";
    //</editor-fold>

    TH1D *FSRatio_plot = (TH1D *) pFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " #alpha").c_str());
    FSRatio_plot->SetTitle((FSRatioTitle + " acceptance correction #alpha = 1/#epsilon_{eff}").c_str());
    FSRatio_plot->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->SetLineStyle(1);
    nFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test->SetStats(1);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitle("Arbitrary units (#events)");
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->SetLineWidth(2);
    nFDpCD_Plot_Clone_test->Draw();
    nFDpCD_Plot_Clone_test->SetStats(1);
    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->SetLineStyle(1);
    pFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test->Draw();
    pFDpCD_Plot_Clone_test->SetStats(1);
    Histogram_list->Add(pFDpCD_Plot_Clone_test);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test_rebined">
    nFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    nFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitle("Arbitrary units (#events)");
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->SetLineWidth(2);
    nFDpCD_Plot_Clone_test_rebined->Draw();
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    pFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test_rebined->Draw();
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone">
    nFDpCD_Plot_Clone->SetLineStyle(1);
    nFDpCD_Plot_Clone->SetLineColor(kBlue);
    nFDpCD_Plot_Clone->SetStats(1);

    nFDpCD_Plot_Clone->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone->GetYaxis()->SetTitle("Arbitrary units (#events)");
    nFDpCD_Plot_Clone->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone->SetLineWidth(2);

    if (weighted_plots) { nFDpCD_Plot_Clone->Sumw2(); }

    nFDpCD_Plot_Clone->Rebin(2);
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    pFDpCD_Plot_Clone->SetLineStyle(1);
    pFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (weighted_plots) { pFDpCD_Plot_Clone->Sumw2(); }

    pFDpCD_Plot_Clone->Rebin(2);
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot">
    FSRatio_plot->SetLineStyle(1);
    FSRatio_plot->SetLineColor(kBlue);

    FSRatio_plot->GetXaxis()->SetTitleSize(0.06);
    FSRatio_plot->GetXaxis()->SetLabelSize(0.0425);
    FSRatio_plot->GetXaxis()->CenterTitle(true);
    FSRatio_plot->GetYaxis()->SetTitleSize(0.06);
    FSRatio_plot->GetYaxis()->SetLabelSize(0.0425);
    FSRatio_plot->GetYaxis()->CenterTitle(true);
    FSRatio_plot->SetLineWidth(2);

    if (weighted_plots) { FSRatio_plot->Sumw2(); }

    FSRatio_plot->Rebin(2);
    FSRatio_plot->Divide(nFDpCD_Plot_Clone);
//    FSRatio_plot->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot->SetStats(0);
    FSRatio_plot->Draw();

    Histogram_list->Add(FSRatio_plot);
    Canvas->SaveAs((FSRatio_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    delete Canvas;
}


#endif //DRAWANDSAVEFSRATIO_H
