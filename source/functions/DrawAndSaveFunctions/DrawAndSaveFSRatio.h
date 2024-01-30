//
// Created by alons on 04/06/2023.
//

/*
to change FSR, change:

type function
title function
SettingSaveNames
SetSaveDir
SetXAxisTitle
SetYAxisTitle

 */

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

#include "../GeneralFunctions.h"
#include "../EventProperties/SetFSRatioSaveDir.h"
#include "../EventProperties/SetStatsTitle.h"
#include "../EventProperties/SettingSaveNames.h"
#include "../EventProperties/SetTitle.h"
#include "../EventProperties/SetType.h"
#include "../EventProperties/SetXAxisTitle.h"
#include "../EventProperties/SetYAxisTitle.h"
#include "../../classes/hData/hData.h"
#include "../../classes/hPlots/hPlot1D.h"

using namespace std;

//TODO: move this into a class with the proper functions

//TODO: finish debugging TFolder addition!

const bool Equi_z_2D = true;

// DrawAndSaveFSRatio in 1D plots ---------------------------------------------------------------------------------------------------------------------------------------

void DrawAndSaveFSRatio(const string &SampleName, const hPlot1D &pFDpCD_Plot, const hPlot1D &nFDpCD_Plot, TList *Histogram_list) {
    hData Propeties;

    bool plot_errorbars = true;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
    Canvas->SetGrid();

    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

//    Canvas->cd();

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6000, 1500);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 4000, 1000);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 1000*3, 750);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid();
    CanvasMulti->cd(2)->SetGrid();
    CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14);
    CanvasMulti->cd(1)->SetLeftMargin(0.16);
    CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14);
    CanvasMulti->cd(2)->SetLeftMargin(0.16);
    CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14);
    CanvasMulti->cd(3)->SetLeftMargin(0.16);
    CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    TH1D *Histogram1D_nFDpCD = nFDpCD_Plot.GetHistogram();
    string FSRatioFSNumerator = Propeties.GetFS(Histogram1D_nFDpCD->GetTitle());
    string nFDpCD_Plot_Clone_StatsTitle = "FSR " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone = (TH1D *) Histogram1D_nFDpCD->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "FSR " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned test (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone_test = (TH1D *) Histogram1D_nFDpCD->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone_test_rebined = (TH1D *) Histogram1D_nFDpCD->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { nFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    TH1D *Histogram1D_pFDpCD = pFDpCD_Plot.GetHistogram();
    string FSRatioFSDenominator = Propeties.GetFS(Histogram1D_pFDpCD->GetTitle());
    string pFDpCD_Plot_Clone_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone_test = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone_test_rebined = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { pFDpCD_Plot_Clone_test_rebined->Rebin(2); }
    //</editor-fold>

    //<editor-fold desc="Setting variables">
    string FSRatioRecTitle = nFDpCD_Plot_Clone->GetTitle();

    string FSRatioParticle = Propeties.GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = Propeties.GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = Propeties.GetParticleNameShort(FSRatioRecTitle);

    string FSRatioType = Propeties.GetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioDRegion = Propeties.GetDRegion(FSRatioRecTitle);
    string FSRatioTitle = Propeties.GetFSRTitle(FSRatioRecTitle, FSRatioPlotsT);
    string FSRatioFS = Propeties.GetFS(FSRatioRecTitle);
    string FSTopology = Propeties.GetTopology(FSRatioRecTitle);

    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);
    string FSRatioYLabel = SetYAxisTitle("FSRatio", FSRatioFS, nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle());

    string FSRatioSaveDir, FSRatioTestSaveDir;
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, FSRatioFS, pFDpCD_Plot.GetHistogram1DSaveNamePath(), FSRatioPlotsT, FSRatioDRegion,
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);

    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_1D_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_1D_SaveName, FSRatioDRegion, FSRatioFS);
    //</editor-fold>

    TH1D *FSRatio_plot_1D = (TH1D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")").c_str());

    string FSRatio_plot_1D_Name = FSRatio_plot_1D->GetName();
    string TFolder_Name = FSRatio_plot_1D_Name + " folder";
    if (FSTopology == "2N") { TFolder_Name = TFolder_Name + " (" + FSRatioDRegion + ")"; }
    TFolder *FSRatio_plot_1D_TFolder = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    if (FSRatioFS == "1p" || FSRatioFS == "1n") {
        FSRatio_plot_1D->SetTitle((FSRatioTitle + " 1nFD/1pFD").c_str());
    } else if (FSRatioFS == "pFDpCD" || FSRatioFS == "nFDpCD") {
        FSRatio_plot_1D->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    }

    FSRatio_plot_1D->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot_1D->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->SetLineStyle(1);
    nFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test->Draw();
    nFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone_test);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->SetLineStyle(1);
    pFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test->Draw();
    pFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone_test);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test_rebined">
    nFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    nFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test_rebined->Draw();
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    pFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test_rebined->Draw();
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone">
    nFDpCD_Plot_Clone->SetLineStyle(1);
    nFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { nFDpCD_Plot_Clone->Sumw2(); }

    if (rebin_plots) { nFDpCD_Plot_Clone->Rebin(2); }
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone);
//    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(1);
    nFDpCD_Plot_Clone->SetLineWidth(3);
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    Canvas->cd();
    pFDpCD_Plot_Clone->SetLineStyle(1);
    pFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { pFDpCD_Plot_Clone->Sumw2(); }

    if (rebin_plots) { pFDpCD_Plot_Clone->Rebin(2); }
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone);
//    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(2);
    pFDpCD_Plot_Clone->SetLineWidth(3);
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot_1D">
    Canvas->cd();
    FSRatio_plot_1D->SetLineStyle(1);
    FSRatio_plot_1D->SetLineColor(kBlue);

    if (plot_errorbars) { FSRatio_plot_1D->Sumw2(); }

    if (rebin_plots) { FSRatio_plot_1D->Rebin(2); }
    FSRatio_plot_1D->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot_1D->SetMinimum(0);
    FSRatio_plot_1D->Draw();
    FSRatio_plot_1D->SetStats(0);

    if (FSRatio_plot_1D->GetMaximum() > 10) { FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 10); }

    TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    EquiLine->SetLineWidth(2);
    EquiLine->SetLineColor(kBlack);
    EquiLine->Draw("same");

    Canvas->SetLogy(1);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Log.png").c_str());
    Canvas->SetLogy(0);

    auto ListOfFunctions = FSRatio_plot_1D->GetListOfFunctions();
    ListOfFunctions->Add((TLine *) EquiLine);

    Histogram_list->Add(FSRatio_plot_1D_TFolder);
    Histogram_list->Add(FSRatio_plot_1D);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName).c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 5);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range5.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 4);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range4.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 3);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range3.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 2);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range2.png").c_str());

    Canvas->Clear();

    CanvasMulti->cd(3);
    FSRatio_plot_1D->SetLineWidth(3);
    FSRatio_plot_1D->Draw();
    EquiLine->SetLineWidth(3);
    EquiLine->Draw("same");
//    Histogram_list->Add(FSRatio_plot_1D);
    //TODO: RegularM is useless - move or delete!
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_RegularM.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 5);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range5M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 4);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range4M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 3);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range3M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 2);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range2M.png").c_str());


    CanvasMulti->Clear();

//    cout << "\n\n\nFSRatioType = " << FSRatioType << "\n\n\n";
//    cout << "\n\n\nFSRatioRecTitle = " << FSRatioRecTitle << "\n\n\n";
//    cout << "\n\n\nFSRatio_plot_1D_SaveName = " << FSRatio_plot_1D_SaveName << "\n\n\n";
    //</editor-fold>

    delete Canvas;
    delete CanvasMulti;
}

void DrawAndSaveFSRatio(const string &SampleName, const hPlot1D &pFDpCD_Plot, TH1D *nFDpCD_Plot, TList *Histogram_list) {
    hData Propeties;

    bool plot_errorbars = true;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

//    Canvas->cd();

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6000, 1500);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 4000, 1000);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 1000*3, 750);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid();
    CanvasMulti->cd(2)->SetGrid();
    CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14);
    CanvasMulti->cd(1)->SetLeftMargin(0.16);
    CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14);
    CanvasMulti->cd(2)->SetLeftMargin(0.16);
    CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14);
    CanvasMulti->cd(3)->SetLeftMargin(0.16);
    CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting variables">
    string FSRatioRecTitle = nFDpCD_Plot->GetTitle();

    string FSRatioParticle = Propeties.GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = Propeties.GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = Propeties.GetParticleNameShort(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    string FSRatioStatsTitle = SetStatsTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string FSRatioFSNumerator = Propeties.GetFS(nFDpCD_Plot->GetTitle());
    string nFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone_test = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test rebined (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone_test_rebined = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { nFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    TH1D *Histogram1D_pFDpCD = pFDpCD_Plot.GetHistogram();
    string FSRatioFSDenominator = Propeties.GetFS(Histogram1D_pFDpCD->GetTitle());
    string pFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone_test = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone_test_rebined = (TH1D *) Histogram1D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { pFDpCD_Plot_Clone_test_rebined->Rebin(2); }
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string FSRatioType = Propeties.GetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioDRegion = Propeties.GetDRegion(FSRatioRecTitle);
    string FSRatioTitle = Propeties.GetFSRTitle(FSRatioRecTitle, FSRatioPlotsT);
    string FSRatioFS = Propeties.GetFS(FSRatioRecTitle);
    string FSTopology = Propeties.GetTopology(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string FSRatioYLabel = SetYAxisTitle("FSRatio", FSRatioFS, nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle());
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string FSRatioSaveDir, FSRatioTestSaveDir;
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, FSRatioFS, pFDpCD_Plot.GetHistogram1DSaveNamePath(), FSRatioPlotsT, FSRatioDRegion,
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);

    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_1D_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_1D_SaveName, FSRatioDRegion, FSRatioFS);
    //</editor-fold>

    TH1D *FSRatio_plot_1D = (TH1D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")").c_str());
    FSRatio_plot_1D->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    FSRatio_plot_1D->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot_1D->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    string FSRatio_plot_1D_Name = FSRatio_plot_1D->GetName();
    string TFolder_Name = FSRatio_plot_1D_Name + " folder";
    if (FSTopology == "2N") { TFolder_Name = TFolder_Name + " (" + FSRatioDRegion + ")"; }
    TFolder *FSRatio_plot_1D_TFolder = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->SetLineStyle(1);
    nFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test->SetStats(1);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitle("Number of events");
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->SetLineWidth(2);
    nFDpCD_Plot_Clone_test->Draw();
    nFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone_test);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->SetLineStyle(1);
    pFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test->Draw();
    pFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone_test);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test);

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
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitle("Number of events");
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->SetLineWidth(2);
    nFDpCD_Plot_Clone_test_rebined->Draw();
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    pFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test_rebined->Draw();
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

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
    nFDpCD_Plot_Clone->GetYaxis()->SetTitle("Number of events");
    nFDpCD_Plot_Clone->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone->SetLineWidth(2);

    if (plot_errorbars) { nFDpCD_Plot_Clone->Sumw2(); }

    if (rebin_plots) { nFDpCD_Plot_Clone->Rebin(2); }
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone);
//    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(1);
    nFDpCD_Plot_Clone->SetLineWidth(3);
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    Canvas->cd();
    pFDpCD_Plot_Clone->SetLineStyle(1);
    pFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { pFDpCD_Plot_Clone->Sumw2(); }

    if (rebin_plots) { pFDpCD_Plot_Clone->Rebin(2); }
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone);
//    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(2);
    pFDpCD_Plot_Clone->SetLineWidth(3);
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot_1D">
    Canvas->cd();
    FSRatio_plot_1D->SetLineStyle(1);
    FSRatio_plot_1D->SetLineColor(kBlue);

    FSRatio_plot_1D->GetXaxis()->SetTitleSize(0.06);
    FSRatio_plot_1D->GetXaxis()->SetLabelSize(0.0425);
    FSRatio_plot_1D->GetXaxis()->CenterTitle(true);
    FSRatio_plot_1D->GetYaxis()->SetTitleSize(0.06);
    FSRatio_plot_1D->GetYaxis()->SetLabelSize(0.0425);
    FSRatio_plot_1D->GetYaxis()->CenterTitle(true);
    FSRatio_plot_1D->SetLineWidth(2);

    if (plot_errorbars) { FSRatio_plot_1D->Sumw2(); }

    if (rebin_plots) { FSRatio_plot_1D->Rebin(2); }
    FSRatio_plot_1D->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot_1D->SetMinimum(0);
    FSRatio_plot_1D->Draw();
    FSRatio_plot_1D->SetStats(0);

    if (FSRatio_plot_1D->GetMaximum() > 10) { FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 10); }

    TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    EquiLine->SetLineWidth(2);
    EquiLine->SetLineColor(kBlack);
    EquiLine->Draw("same");

    Canvas->SetLogy(1);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Log.png").c_str());
    Canvas->SetLogy(0);

    auto ListOfFunctions = FSRatio_plot_1D->GetListOfFunctions();
    ListOfFunctions->Add((TLine *) EquiLine);

    Histogram_list->Add(FSRatio_plot_1D_TFolder);
    Histogram_list->Add(FSRatio_plot_1D);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName).c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 5);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range5.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 4);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range4.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 3);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range3.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 2);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range2.png").c_str());

    Canvas->Clear();

    CanvasMulti->cd(3);
    FSRatio_plot_1D->SetLineWidth(3);
    FSRatio_plot_1D->Draw();
    EquiLine->SetLineWidth(3);
    EquiLine->Draw("same");
//    Histogram_list->Add(FSRatio_plot_1D);
    //TODO: RegularM is useless - move or delete!
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_RegularM.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 5);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range5M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 4);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range4M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 3);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range3M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 2);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range2M.png").c_str());

    CanvasMulti->Clear();

//    cout << "\n\n\nFSRatioType = " << FSRatioType << "\n\n\n";
//    cout << "\n\n\nFSRatioRecTitle = " << FSRatioRecTitle << "\n\n\n";
//    cout << "\n\n\nFSRatio_plot_1D_SaveName = " << FSRatio_plot_1D_SaveName << "\n\n\n";
    // </editor-fold>

    delete Canvas;
    delete CanvasMulti;
}

void DrawAndSaveFSRatio(const string &SampleName, TH1D *pFDpCD_Plot, const string &pFDpCD_PlotSaveNamePath, TH1D *nFDpCD_Plot, TList *Histogram_list) {
    hData Propeties;

    bool plot_errorbars = true;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

//    Canvas->cd();

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6000, 1500);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 4000, 1000);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 1000*3, 750);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid();
    CanvasMulti->cd(2)->SetGrid();
    CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14);
    CanvasMulti->cd(1)->SetLeftMargin(0.16);
    CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14);
    CanvasMulti->cd(2)->SetLeftMargin(0.16);
    CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14);
    CanvasMulti->cd(3)->SetLeftMargin(0.16);
    CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting variables">
    string FSRatioRecTitle = nFDpCD_Plot->GetTitle();

    string FSRatioParticle = Propeties.GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = Propeties.GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = Propeties.GetParticleNameShort(FSRatioRecTitle);
    string FSRatioFS = Propeties.GetFS(FSRatioRecTitle);
    string FSTopology = Propeties.GetTopology(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    string FSRatioStatsTitle = SetStatsTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string FSRatioFSNumerator = Propeties.GetFS(nFDpCD_Plot->GetTitle());
    string nFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " " + FSTopology + " - cloned (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " " + FSTopology + " - cloned test (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone_test = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + FSRatioStatsTitle + " " + FSTopology + " - cloned test rebined (" + FSRatioFSNumerator + ")";
    TH1D *nFDpCD_Plot_Clone_test_rebined = (TH1D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { nFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    string FSRatioFSDenominator = Propeties.GetFS(pFDpCD_Plot->GetTitle());
    string pFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " " + FSTopology + " - cloned (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone = (TH1D *) pFDpCD_Plot->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " " + FSTopology + " - cloned test (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone_test = (TH1D *) pFDpCD_Plot->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + FSRatioStatsTitle + " " + FSTopology + " - cloned test rebined (" + FSRatioFSDenominator + ")";
    TH1D *pFDpCD_Plot_Clone_test_rebined = (TH1D *) pFDpCD_Plot->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { pFDpCD_Plot_Clone_test_rebined->Rebin(2); }
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string FSRatioType = Propeties.GetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioDRegion = Propeties.GetDRegion(FSRatioRecTitle);
    string FSRatioTitle = Propeties.GetFSRTitle(FSRatioRecTitle, FSRatioPlotsT);
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string FSRatioYLabel = SetYAxisTitle("FSRatio", FSRatioFS, nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle());
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string FSRatioSaveDir, FSRatioTestSaveDir;
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, FSRatioFS, pFDpCD_PlotSaveNamePath, FSRatioPlotsT, FSRatioDRegion,
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_1D_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_1D_SaveName, FSRatioDRegion, FSRatioFS);
    //</editor-fold>

    TH1D *FSRatio_plot_1D = (TH1D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")").c_str());
    FSRatio_plot_1D->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    FSRatio_plot_1D->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot_1D->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    string FSRatio_plot_1D_Name = FSRatio_plot_1D->GetName();
    string TFolder_Name = FSRatio_plot_1D_Name + " folder";
    if (FSTopology == "2N") { TFolder_Name = TFolder_Name + " (" + FSRatioDRegion + ")"; }
    TFolder *FSRatio_plot_1D_TFolder = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

//    if (Histogram_list->FindObject(TFolder_Name.c_str())) {
//        cout << "\n\nTFolder_Name is in TList!\n\n";
//        cout << "\n\nFSRatioFSNumerator = " << FSRatioFSNumerator << "\n\n";
//        cout << "\n\nFSRatioFSDenominator = " << FSRatioFSDenominator << "\n\n";
//        cout << "\n\nFSRatioDRegion = " << FSRatioDRegion << "\n\n";
//        cout << "\n\nFSTopology = " << FSTopology << "\n\n";
//        cout << "\n\nExiting...\n\n", exit(0);
//    }

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->SetLineStyle(1);
    nFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    nFDpCD_Plot_Clone_test->SetStats(1);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitle("Number of events");
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->SetLineWidth(2);
    nFDpCD_Plot_Clone_test->Draw();
    nFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone_test);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->SetLineStyle(1);
    pFDpCD_Plot_Clone_test->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test->Draw();
    pFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone_test);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test);

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
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitle("Number of events");
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->SetLineWidth(2);
    nFDpCD_Plot_Clone_test_rebined->Draw();
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->SetLineStyle(1);
    pFDpCD_Plot_Clone_test_rebined->SetLineColor(kBlue);
    pFDpCD_Plot_Clone_test_rebined->Draw();
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

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
    nFDpCD_Plot_Clone->GetYaxis()->SetTitle("Number of events");
    nFDpCD_Plot_Clone->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone->SetLineWidth(2);

    if (plot_errorbars) { nFDpCD_Plot_Clone->Sumw2(); }

    if (rebin_plots) { nFDpCD_Plot_Clone->Rebin(2); }
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(nFDpCD_Plot_Clone);
//    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(1);
    nFDpCD_Plot_Clone->SetLineWidth(3);
    nFDpCD_Plot_Clone->Draw();
    nFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    Canvas->cd();
    pFDpCD_Plot_Clone->SetLineStyle(1);
    pFDpCD_Plot_Clone->SetLineColor(kBlue);

    if (plot_errorbars) { pFDpCD_Plot_Clone->Sumw2(); }

    if (rebin_plots) { pFDpCD_Plot_Clone->Rebin(2); }
    pFDpCD_Plot_Clone->SetLineWidth(2);
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_1D_TFolder->Add(pFDpCD_Plot_Clone);
//    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(2);
    pFDpCD_Plot_Clone->SetLineWidth(3);
    pFDpCD_Plot_Clone->Draw();
    pFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot_1D">
    Canvas->cd();
    FSRatio_plot_1D->SetLineStyle(1);
    FSRatio_plot_1D->SetLineColor(kBlue);

    FSRatio_plot_1D->GetXaxis()->SetTitleSize(0.06);
    FSRatio_plot_1D->GetXaxis()->SetLabelSize(0.0425);
    FSRatio_plot_1D->GetXaxis()->CenterTitle(true);
    FSRatio_plot_1D->GetYaxis()->SetTitleSize(0.06);
    FSRatio_plot_1D->GetYaxis()->SetLabelSize(0.0425);
    FSRatio_plot_1D->GetYaxis()->CenterTitle(true);
    FSRatio_plot_1D->SetLineWidth(2);

    if (plot_errorbars) { FSRatio_plot_1D->Sumw2(); }

    if (rebin_plots) { FSRatio_plot_1D->Rebin(2); }
    FSRatio_plot_1D->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot_1D->SetMinimum(0);
    FSRatio_plot_1D->Draw();
    FSRatio_plot_1D->SetStats(0);

    if (FSRatio_plot_1D->GetMaximum() > 10) { FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 10); }

    TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
    EquiLine->SetLineWidth(2);
    EquiLine->SetLineColor(kBlack);
    EquiLine->Draw("same");

    Canvas->SetLogy(1);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Log.png").c_str());
    Canvas->SetLogy(0);

    auto ListOfFunctions = FSRatio_plot_1D->GetListOfFunctions();
    ListOfFunctions->Add((TLine *) EquiLine);

    Histogram_list->Add(FSRatio_plot_1D_TFolder);
    Histogram_list->Add(FSRatio_plot_1D);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName).c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 5);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range5.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 4);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range4.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 3);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range3.png").c_str());

    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 2);
    Canvas->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range2.png").c_str());

    Canvas->Clear();

    CanvasMulti->cd(3);
    FSRatio_plot_1D->SetLineWidth(3);
    FSRatio_plot_1D->Draw();
    EquiLine->SetLineWidth(3);
    EquiLine->Draw("same");
//    Histogram_list->Add(FSRatio_plot_1D);
    //TODO: RegularM is useless - move or delete!
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_RegularM.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 5);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range5M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 4);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range4M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 3);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range3M.png").c_str());
    FSRatio_plot_1D->GetYaxis()->SetRangeUser(0, 2);
    CanvasMulti->SaveAs((FSRatio_plot_1D_SaveName.substr(0, FSRatio_plot_1D_SaveName.find_last_of(".png") - 3) + "_Range2M.png").c_str());

    CanvasMulti->Clear();

//    cout << "\n\n\nFSRatioType = " << FSRatioType << "\n\n\n";
//    cout << "\n\n\nFSRatioRecTitle = " << FSRatioRecTitle << "\n\n\n";
//    cout << "\n\n\nFSRatio_plot_1D_SaveName = " << FSRatio_plot_1D_SaveName << "\n\n\n";
    //</editor-fold>

    delete Canvas;
    delete CanvasMulti;
}

// DrawAndSaveFSRatio in 2D plots ---------------------------------------------------------------------------------------------------------------------------------------

void DrawAndSaveFSRatio(const string &SampleName, const hPlot2D &pFDpCD_Plot, const hPlot2D &nFDpCD_Plot, TList *Histogram_list) {
    hData Propeties;

    bool plot_errorbars = true;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
    Canvas->SetGrid();

    Canvas->SetBottomMargin(0.14);
    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

//    Canvas->cd();

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6000, 1500);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 4000, 1000);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 1000*3, 750);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid();
    CanvasMulti->cd(2)->SetGrid();
    CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14);
    CanvasMulti->cd(1)->SetLeftMargin(0.16);
    CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14);
    CanvasMulti->cd(2)->SetLeftMargin(0.16);
    CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14);
    CanvasMulti->cd(3)->SetLeftMargin(0.16);
    CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    TH2D *Histogram2D_nFDpCD = nFDpCD_Plot.GetHistogram();
    string FSRatioFSNumerator = Propeties.GetFS(Histogram2D_nFDpCD->GetTitle());

    string nFDpCD_Plot_Clone_StatsTitle = "FSR " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone = (TH2D *) Histogram2D_nFDpCD->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "FSR " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned test (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone_test = (TH2D *) Histogram2D_nFDpCD->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + nFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone_test_rebined = (TH2D *) Histogram2D_nFDpCD->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
//    nFDpCD_Plot_Clone_test_rebined->Rebin(2);

    TH2D *Histogram2D_pFDpCD = pFDpCD_Plot.GetHistogram();
    string FSRatioFSDenominator = Propeties.GetFS(Histogram2D_pFDpCD->GetTitle());
    string pFDpCD_Plot_Clone_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone = (TH2D *) Histogram2D_pFDpCD->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone_test = (TH2D *) Histogram2D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone_test_rebined = (TH2D *) Histogram2D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
//    pFDpCD_Plot_Clone_test_rebined->Rebin(2);

    double Zmax = 1.1 * max(pFDpCD_Plot_Clone->GetMaximum(), nFDpCD_Plot_Clone->GetMaximum());

    if (Equi_z_2D) {
        pFDpCD_Plot_Clone->SetMaximum(Zmax);
        nFDpCD_Plot_Clone->SetMaximum(Zmax);
    }
    //</editor-fold>

    //<editor-fold desc="Setting variables">
    string FSRatioRecTitle = nFDpCD_Plot_Clone->GetTitle();

    string FSRatioParticle = Propeties.GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = Propeties.GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = Propeties.GetParticleNameShort(FSRatioRecTitle);

    string FSRatioType = Propeties.GetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioDRegion = Propeties.GetDRegion(FSRatioRecTitle);
    string FSRatioTitle = Propeties.GetFSRTitle(FSRatioRecTitle, FSRatioPlotsT);
    string FSRatioFS = Propeties.GetFS(FSRatioRecTitle);
    string FSTopology = Propeties.GetTopology(FSRatioRecTitle);

    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);
    string FSRatioYLabel = SetYAxisTitle("FSRatio", FSRatioFS, nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle(), FSRatioRecTitle);

    string FSRatioSaveDir, FSRatioTestSaveDir;
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, FSRatioFS, pFDpCD_Plot.GetHistogram2DSaveNamePath(), FSRatioPlotsT, FSRatioDRegion,
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);

    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_2D_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_2D_SaveName, FSRatioDRegion, FSRatioFS);
    //</editor-fold>

    TH2D *FSRatio_plot_2D = (TH2D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")").c_str());
    FSRatio_plot_2D->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    FSRatio_plot_2D->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot_2D->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    string FSRatio_plot_2D_Name = FSRatio_plot_2D->GetName();
    string TFolder_Name = FSRatio_plot_2D_Name + " folder";
    if (FSTopology == "2N") { TFolder_Name = TFolder_Name + " (" + FSRatioDRegion + ")"; }
    TFolder *FSRatio_plot_2D_TFolder = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->SetStats(1);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->Draw("colz");
    nFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone_test);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->Draw("colz");
    pFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone_test);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test_rebined">
    nFDpCD_Plot_Clone_test_rebined->Draw("colz");
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->Draw("colz");
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone">
    nFDpCD_Plot_Clone->Draw("colz");

    if (FSRatioType == "P_nucFD_vs_P_nucCD" || FSRatioType == "P_nucL_vs_P_nucR") {
        double Lowerlim_nFDpCD = max(nFDpCD_Plot.GetLowerXlim(), nFDpCD_Plot.GetLowerYlim());
        double Upperlim_nFDpCD = min(nFDpCD_Plot.GetUpperXlim(), nFDpCD_Plot.GetUpperYlim());

        TLine *EquiLine2D_nFDpCD = new TLine(Lowerlim_nFDpCD, Lowerlim_nFDpCD, Upperlim_nFDpCD, Upperlim_nFDpCD);
        EquiLine2D_nFDpCD->SetLineWidth(2);
        EquiLine2D_nFDpCD->SetLineColor(kRed);
        EquiLine2D_nFDpCD->Draw("same");

        auto nFDpCD_Plot_Clone_legend = new TLegend(0.87, 0.865 - 0.25, 0.87 - 0.2, 0.865 - 0.3);
        TLegendEntry *EquiLine2D_nFDpCD_entry = nFDpCD_Plot_Clone_legend->AddEntry(EquiLine2D_nFDpCD, "y(x) = x", "l");
        nFDpCD_Plot_Clone_legend->Draw("same");

        auto ListOfFunctions_nFDpCD = nFDpCD_Plot_Clone->GetListOfFunctions();
        ListOfFunctions_nFDpCD->Add((TLine *) EquiLine2D_nFDpCD);
        ListOfFunctions_nFDpCD->Add((TLegend *) nFDpCD_Plot_Clone_legend);
    }

    nFDpCD_Plot_Clone->SetStats(1);

    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone);
//    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(1);
    nFDpCD_Plot_Clone->Draw("colz");

    if (FSRatioType == "P_nucFD_vs_P_nucCD" || FSRatioType == "P_nucL_vs_P_nucR") {
        double Lowerlim_nFDpCD = max(nFDpCD_Plot.GetLowerXlim(), nFDpCD_Plot.GetLowerYlim());
        double Upperlim_nFDpCD = min(nFDpCD_Plot.GetUpperXlim(), nFDpCD_Plot.GetUpperYlim());

        TLine *EquiLine2D_nFDpCD = new TLine(Lowerlim_nFDpCD, Lowerlim_nFDpCD, Upperlim_nFDpCD, Upperlim_nFDpCD);
        EquiLine2D_nFDpCD->SetLineWidth(2);
        EquiLine2D_nFDpCD->SetLineColor(kRed);
        EquiLine2D_nFDpCD->Draw("same");

        auto nFDpCD_Plot_Clone_legend = new TLegend(0.87, 0.865 - 0.25, 0.87 - 0.2, 0.865 - 0.3);
        TLegendEntry *EquiLine2D_nFDpCD_entry = nFDpCD_Plot_Clone_legend->AddEntry(EquiLine2D_nFDpCD, "y(x) = x", "l");
        nFDpCD_Plot_Clone_legend->Draw("same");

        auto ListOfFunctions_nFDpCD = nFDpCD_Plot_Clone->GetListOfFunctions();
        ListOfFunctions_nFDpCD->Add((TLine *) EquiLine2D_nFDpCD);
        ListOfFunctions_nFDpCD->Add((TLegend *) nFDpCD_Plot_Clone_legend);
    }

    nFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    Canvas->cd();
    pFDpCD_Plot_Clone->Draw("colz");

    if (FSRatioType == "P_nucFD_vs_P_nucCD" || FSRatioType == "P_nucL_vs_P_nucR") {
        double Lowerlim_pFDpCD = max(pFDpCD_Plot.GetLowerXlim(), pFDpCD_Plot.GetLowerYlim());
        double Upperlim_pFDpCD = min(pFDpCD_Plot.GetUpperXlim(), pFDpCD_Plot.GetUpperYlim());

        TLine *EquiLine2D_pFDpCD = new TLine(Lowerlim_pFDpCD, Lowerlim_pFDpCD, Upperlim_pFDpCD, Upperlim_pFDpCD);
        EquiLine2D_pFDpCD->SetLineWidth(2);
        EquiLine2D_pFDpCD->SetLineColor(kRed);
        EquiLine2D_pFDpCD->Draw("same");

        auto pFDpCD_Plot_Clone_legend = new TLegend(0.87, 0.865 - 0.25, 0.87 - 0.2, 0.865 - 0.3);
        TLegendEntry *EquiLine2D_pFDpCD_entry = pFDpCD_Plot_Clone_legend->AddEntry(EquiLine2D_pFDpCD, "y(x) = x", "l");
        pFDpCD_Plot_Clone_legend->Draw("same");

        auto ListOfFunctions_pFDpCD = pFDpCD_Plot_Clone->GetListOfFunctions();
        ListOfFunctions_pFDpCD->Add((TLine *) EquiLine2D_pFDpCD);
        ListOfFunctions_pFDpCD->Add((TLegend *) pFDpCD_Plot_Clone_legend);
    }

    pFDpCD_Plot_Clone->SetStats(1);

    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone);
//    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(2);
    pFDpCD_Plot_Clone->Draw("colz");

    if (FSRatioType == "P_nucFD_vs_P_nucCD" || FSRatioType == "P_nucL_vs_P_nucR") {
        double Lowerlim_pFDpCD = max(pFDpCD_Plot.GetLowerXlim(), pFDpCD_Plot.GetLowerYlim());
        double Upperlim_pFDpCD = min(pFDpCD_Plot.GetUpperXlim(), pFDpCD_Plot.GetUpperYlim());

        TLine *EquiLine2D_pFDpCD = new TLine(Lowerlim_pFDpCD, Lowerlim_pFDpCD, Upperlim_pFDpCD, Upperlim_pFDpCD);
        EquiLine2D_pFDpCD->SetLineWidth(2);
        EquiLine2D_pFDpCD->SetLineColor(kRed);
        EquiLine2D_pFDpCD->Draw("same");

        auto pFDpCD_Plot_Clone_legend = new TLegend(0.87, 0.865 - 0.25, 0.87 - 0.2, 0.865 - 0.3);
        TLegendEntry *EquiLine2D_pFDpCD_entry = pFDpCD_Plot_Clone_legend->AddEntry(EquiLine2D_pFDpCD, "y(x) = x", "l");
        pFDpCD_Plot_Clone_legend->Draw("same");

        auto ListOfFunctions_pFDpCD = pFDpCD_Plot_Clone->GetListOfFunctions();
        ListOfFunctions_pFDpCD->Add((TLine *) EquiLine2D_pFDpCD);
        ListOfFunctions_pFDpCD->Add((TLegend *) pFDpCD_Plot_Clone_legend);
    }

    pFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot_2D">
    Canvas->cd();
    FSRatio_plot_2D->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot_2D->SetStats(0);
    FSRatio_plot_2D->SetMaximum(10);
    FSRatio_plot_2D->SetMinimum(0.1);
    FSRatio_plot_2D->Draw("colz");

    if (FSRatioType == "P_nucFD_vs_P_nucCD" || FSRatioType == "P_nucL_vs_P_nucR") {
        double Lowerlim_FSRatio = max(nFDpCD_Plot.GetLowerXlim(), nFDpCD_Plot.GetLowerYlim());
        double Upperlim_FSRatio = min(nFDpCD_Plot.GetUpperXlim(), nFDpCD_Plot.GetUpperYlim());

        TLine *EquiLine2D_FSRatio = new TLine(Lowerlim_FSRatio, Lowerlim_FSRatio, Upperlim_FSRatio, Upperlim_FSRatio);
        EquiLine2D_FSRatio->SetLineWidth(2);
        EquiLine2D_FSRatio->SetLineColor(kRed);
        EquiLine2D_FSRatio->Draw("same");

        auto FSRatio_plot_2D_Clone_legend = new TLegend(0.87, 0.825, 0.87 - 0.2, 0.825 - 0.05);
        TLegendEntry *EquiLine2D_FSRatio_entry = FSRatio_plot_2D_Clone_legend->AddEntry(EquiLine2D_FSRatio, "y(x) = x", "l");
        FSRatio_plot_2D_Clone_legend->Draw("same");

        auto ListOfFunctions_FSRatio = FSRatio_plot_2D->GetListOfFunctions();
        ListOfFunctions_FSRatio->Add((TLine *) EquiLine2D_FSRatio);
        ListOfFunctions_FSRatio->Add((TLegend *) FSRatio_plot_2D_Clone_legend);
    }

    Canvas->SetLogz(1);

    Histogram_list->Add(FSRatio_plot_2D_TFolder);
    Histogram_list->Add(FSRatio_plot_2D);
    Canvas->SaveAs((FSRatio_plot_2D_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(3);
    CanvasMulti->cd(3)->SetLogz(1);
    FSRatio_plot_2D->Draw("colz");

    if (FSRatioType == "P_nucFD_vs_P_nucCD" || FSRatioType == "P_nucL_vs_P_nucR") {
        double Lowerlim_FSRatio = max(nFDpCD_Plot.GetLowerXlim(), nFDpCD_Plot.GetLowerYlim());
        double Upperlim_FSRatio = min(nFDpCD_Plot.GetUpperXlim(), nFDpCD_Plot.GetUpperYlim());

        TLine *EquiLine2D_FSRatio = new TLine(Lowerlim_FSRatio, Lowerlim_FSRatio, Upperlim_FSRatio, Upperlim_FSRatio);
        EquiLine2D_FSRatio->SetLineWidth(2);
        EquiLine2D_FSRatio->SetLineColor(kRed);
        EquiLine2D_FSRatio->Draw("same");

        auto FSRatio_plot_2D_Clone_legend = new TLegend(0.87, 0.825, 0.87 - 0.2, 0.825 - 0.05);
        TLegendEntry *EquiLine2D_FSRatio_entry = FSRatio_plot_2D_Clone_legend->AddEntry(EquiLine2D_FSRatio, "y(x) = x", "l");
        FSRatio_plot_2D_Clone_legend->Draw("same");
    }

    FSRatio_plot_2D->SetStats(0);
    FSRatio_plot_2D->SetMaximum(10);
    FSRatio_plot_2D->SetMinimum(0.1);
    CanvasMulti->SaveAs((FSRatio_plot_2D_SaveName.substr(0, FSRatio_plot_2D_SaveName.find_last_of(".png") - 3) + "_RegularM.png").c_str());
    CanvasMulti->Clear();
    //</editor-fold>

//    cout << "\n\n\nFSRatioType = " << FSRatioType << "\n\n\n";
//    cout << "\n\n\nFSRatioRecTitle = " << FSRatioRecTitle << "\n\n\n";
//    cout << "\n\n\nFSRatio_plot_2D_SaveName = " << FSRatio_plot_2D_SaveName << "\n\n\n";

    delete Canvas;
    delete CanvasMulti;
}

void DrawAndSaveFSRatio(const string &SampleName, const hPlot2D &pFDpCD_Plot, TH2D *nFDpCD_Plot, TList *Histogram_list) {
    hData Propeties;

    bool plot_errorbars = true;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

//    Canvas->cd();

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6000, 1500);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 4000, 1000);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 1000*3, 750);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid();
    CanvasMulti->cd(2)->SetGrid();
    CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14);
    CanvasMulti->cd(1)->SetLeftMargin(0.16);
    CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14);
    CanvasMulti->cd(2)->SetLeftMargin(0.16);
    CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14);
    CanvasMulti->cd(3)->SetLeftMargin(0.16);
    CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting variables">
    string FSRatioRecTitle = nFDpCD_Plot->GetTitle();

    string FSRatioParticle = Propeties.GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = Propeties.GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = Propeties.GetParticleNameShort(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting stats box title">
    string FSRatioStatsTitle = SetStatsTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string FSRatioFSNumerator = Propeties.GetFS(nFDpCD_Plot->GetTitle());
    string nFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone = (TH2D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone_test = (TH2D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test rebined (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone_test_rebined = (TH2D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { nFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    TH2D *Histogram2D_pFDpCD = pFDpCD_Plot.GetHistogram();
    string FSRatioFSDenominator = Propeties.GetFS(Histogram2D_pFDpCD->GetTitle());
    string pFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone = (TH2D *) Histogram2D_pFDpCD->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone_test = (TH2D *) Histogram2D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + pFDpCD_Plot.GetHistogramStatTitle() + " - cloned test rebined (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone_test_rebined = (TH2D *) Histogram2D_pFDpCD->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
    if (rebin_plots) { pFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    double Zmax = 1.1 * max(pFDpCD_Plot_Clone->GetMaximum(), nFDpCD_Plot_Clone->GetMaximum());

    if (Equi_z_2D) {
        pFDpCD_Plot_Clone->SetMaximum(Zmax);
        nFDpCD_Plot_Clone->SetMaximum(Zmax);
    }
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string FSRatioType = Propeties.GetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioDRegion = Propeties.GetDRegion(FSRatioRecTitle);
    string FSRatioTitle = Propeties.GetFSRTitle(FSRatioRecTitle, FSRatioPlotsT);
    string FSRatioFS = Propeties.GetFS(FSRatioRecTitle);
    string FSTopology = Propeties.GetTopology(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string FSRatioYLabel = SetYAxisTitle("FSRatio", FSRatioFS, nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle(), FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string FSRatioSaveDir, FSRatioTestSaveDir;
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, FSRatioFS, pFDpCD_Plot.GetHistogram2DSaveNamePath(), FSRatioPlotsT, FSRatioDRegion,
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_2D_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_2D_SaveName, FSRatioDRegion, FSRatioFS);
    //</editor-fold>

    TH2D *FSRatio_plot_2D = (TH2D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")").c_str());
    FSRatio_plot_2D->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    FSRatio_plot_2D->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot_2D->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    string FSRatio_plot_2D_Name = FSRatio_plot_2D->GetName();
    string TFolder_Name = FSRatio_plot_2D_Name + " folder";
    if (FSTopology == "2N") { TFolder_Name = TFolder_Name + " (" + FSRatioDRegion + ")"; }
    TFolder *FSRatio_plot_2D_TFolder = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->Draw("colz");
    nFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone_test);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->Draw("colz");
    pFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone_test);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test_rebined">
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->Draw("colz");
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->Draw("colz");
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone">
    nFDpCD_Plot_Clone->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetYaxis()->CenterTitle(true);

    nFDpCD_Plot_Clone->Draw("colz");
    nFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone);
//    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(1);
    nFDpCD_Plot_Clone->Draw("colz");
    nFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    Canvas->cd();
    pFDpCD_Plot_Clone->Draw("colz");
    pFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone);
//    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(2);
    pFDpCD_Plot_Clone->Draw("colz");
    pFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot_2D">
    Canvas->cd();
    FSRatio_plot_2D->GetXaxis()->SetTitleSize(0.06);
    FSRatio_plot_2D->GetXaxis()->SetLabelSize(0.0425);
    FSRatio_plot_2D->GetXaxis()->CenterTitle(true);
    FSRatio_plot_2D->GetYaxis()->SetTitleSize(0.06);
    FSRatio_plot_2D->GetYaxis()->SetLabelSize(0.0425);
    FSRatio_plot_2D->GetYaxis()->CenterTitle(true);

    FSRatio_plot_2D->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot_2D->SetStats(0);
    FSRatio_plot_2D->SetMaximum(10);
    FSRatio_plot_2D->SetMinimum(0.1);
    FSRatio_plot_2D->Draw("colz");
    Canvas->SetLogz(1);

    Histogram_list->Add(FSRatio_plot_2D_TFolder);
    Histogram_list->Add(FSRatio_plot_2D);
    Canvas->SaveAs((FSRatio_plot_2D_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(3);
    CanvasMulti->cd(3)->SetLogz(1);
    FSRatio_plot_2D->Draw("colz");
    FSRatio_plot_2D->SetStats(0);
    FSRatio_plot_2D->SetMaximum(10);
    FSRatio_plot_2D->SetMinimum(0.1);
    CanvasMulti->SaveAs((FSRatio_plot_2D_SaveName.substr(0, FSRatio_plot_2D_SaveName.find_last_of(".png") - 3) + "_RegularM.png").c_str());
    CanvasMulti->Clear();
    //</editor-fold>

//    cout << "\n\n\nFSRatioType = " << FSRatioType << "\n\n\n";
//    cout << "\n\n\nFSRatioRecTitle = " << FSRatioRecTitle << "\n\n\n";
//    cout << "\n\n\nFSRatio_plot_2D_SaveName = " << FSRatio_plot_2D_SaveName << "\n\n\n";

    delete Canvas;
    delete CanvasMulti;
}

void DrawAndSaveFSRatio(const string &SampleName, TH2D *pFDpCD_Plot, const string &pFDpCD_PlotSaveNamePath, TH2D *nFDpCD_Plot, TList *Histogram_list) {
    hData Propeties;

    bool plot_errorbars = false;
    bool rebin_plots = false;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 2000, 1500); // high res
//    TCanvas *Canvas = new TCanvas("canvas", "canvas", 1650, 1150);
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.16);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

//    Canvas->cd();

    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 6000, 1500);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 4000, 1000);
//    TCanvas *CanvasMulti = new TCanvas("CanvasMulti", "CanvasMulti", 1000*3, 750);
    CanvasMulti->Divide(3, 1);
    CanvasMulti->cd(1)->SetGrid();
    CanvasMulti->cd(2)->SetGrid();
    CanvasMulti->cd(3)->SetGrid();

    CanvasMulti->cd(1)->SetBottomMargin(0.14);
    CanvasMulti->cd(1)->SetLeftMargin(0.16);
    CanvasMulti->cd(1)->SetRightMargin(0.12);
    CanvasMulti->cd(2)->SetBottomMargin(0.14);
    CanvasMulti->cd(2)->SetLeftMargin(0.16);
    CanvasMulti->cd(2)->SetRightMargin(0.12);
    CanvasMulti->cd(3)->SetBottomMargin(0.14);
    CanvasMulti->cd(3)->SetLeftMargin(0.16);
    CanvasMulti->cd(3)->SetRightMargin(0.12);

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting variables">
    string FSRatioRecTitle = nFDpCD_Plot->GetTitle();

    string FSRatioParticle = Propeties.GetParticleName(FSRatioRecTitle);
    string FSRatioParticleLC = Propeties.GetParticleNameLC(FSRatioRecTitle);
    string FSRatioParticleShort = Propeties.GetParticleNameShort(FSRatioRecTitle);
    string FSRatioStatsTitle = SetStatsTitle(FSRatioRecTitle);
    string FSTopology = Propeties.GetTopology(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Cloning histograms">
    string FSRatioFSNumerator = Propeties.GetFS(nFDpCD_Plot->GetTitle());
    string nFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone = (TH2D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone_test = (TH2D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string nFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test rebined (" + FSRatioFSNumerator + ")";
    TH2D *nFDpCD_Plot_Clone_test_rebined = (TH2D *) nFDpCD_Plot->Clone((nFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
//    if (rebin_plots) { nFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    string FSRatioFSDenominator = Propeties.GetFS(pFDpCD_Plot->GetTitle());
    string pFDpCD_Plot_Clone_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone = (TH2D *) pFDpCD_Plot->Clone((pFDpCD_Plot_Clone_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone_test = (TH2D *) pFDpCD_Plot->Clone((pFDpCD_Plot_Clone_test_StatsTitle).c_str());
    string pFDpCD_Plot_Clone_test_rebined_StatsTitle = "FSR " + FSRatioStatsTitle + " - cloned test rebined (" + FSRatioFSDenominator + ")";
    TH2D *pFDpCD_Plot_Clone_test_rebined = (TH2D *) pFDpCD_Plot->Clone((pFDpCD_Plot_Clone_test_rebined_StatsTitle).c_str());
//    if (rebin_plots) { pFDpCD_Plot_Clone_test_rebined->Rebin(2); }

    double Zmax = 1.1 * max(pFDpCD_Plot_Clone->GetMaximum(), nFDpCD_Plot_Clone->GetMaximum());

    if (Equi_z_2D) {
        pFDpCD_Plot_Clone->SetMaximum(Zmax);
        nFDpCD_Plot_Clone->SetMaximum(Zmax);
    }
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string FSRatioType = Propeties.GetType(FSRatioRecTitle);
    string FSRatioPlotsT = "FSRatio";
    string FSRatioDRegion = Propeties.GetDRegion(FSRatioRecTitle);
    string FSRatioTitle = Propeties.GetFSRTitle(FSRatioRecTitle, FSRatioPlotsT);
    string FSRatioFS = Propeties.GetFS(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string FSRatioXLabel = SetXAxisTitle(FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string FSRatioYLabel = SetYAxisTitle("FSRatio", FSRatioFS, nFDpCD_Plot_Clone->GetXaxis()->GetTitle(), pFDpCD_Plot_Clone->GetXaxis()->GetTitle(), FSRatioRecTitle);
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string FSRatioSaveDir, FSRatioTestSaveDir;
    SetFSRatioSaveDir(FSRatioSaveDir, FSRatioTestSaveDir, FSRatioRecTitle, FSRatioFS, pFDpCD_PlotSaveNamePath, FSRatioPlotsT, FSRatioDRegion,
                      FSRatioParticle, FSRatioParticleLC, FSRatioParticleShort, FSRatioType);
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName;
    string pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName;
    string sNameFlag, FSRatio_plot_2D_SaveName;

    SettingSaveNames(SampleName, FSRatioType, FSRatioParticle, FSRatioSaveDir, FSRatioTestSaveDir, FSRatioPlotsT,
                     nFDpCD_Plot_Clone_SaveName, nFDpCD_Plot_Clone_test_SaveName, nFDpCD_Plot_Clone_test_rebined_SaveName,
                     pFDpCD_Plot_Clone_SaveName, pFDpCD_Plot_Clone_test_SaveName, pFDpCD_Plot_Clone_test_rebined_SaveName,
                     sNameFlag, FSRatio_plot_2D_SaveName, FSRatioDRegion, FSRatioFS);
    //</editor-fold>

    TH2D *FSRatio_plot_2D = (TH2D *) nFDpCD_Plot_Clone->Clone((FSRatioParticle + " " + FSRatioType + " FSRatio (" + FSTopology + ")").c_str());
    FSRatio_plot_2D->SetTitle((FSRatioTitle + " nFDpCD/pFDpCD").c_str());
    FSRatio_plot_2D->GetYaxis()->SetTitle((FSRatioYLabel).c_str());
    FSRatio_plot_2D->GetXaxis()->SetTitle((FSRatioXLabel).c_str());

    string FSRatio_plot_2D_Name = FSRatio_plot_2D->GetName();
    string TFolder_Name = FSRatio_plot_2D_Name + " folder";
    if (FSTopology == "2N") { TFolder_Name = TFolder_Name + " (" + FSRatioDRegion + ")"; }
    TFolder *FSRatio_plot_2D_TFolder = new TFolder(TFolder_Name.c_str(), TFolder_Name.c_str());

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test">
    nFDpCD_Plot_Clone_test->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test->Draw("colz");
    nFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone_test);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test">
    pFDpCD_Plot_Clone_test->Draw("colz");
    pFDpCD_Plot_Clone_test->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone_test);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone_test_rebined">
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone_test_rebined->GetYaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone_test_rebined->Draw("colz");
    nFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(nFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((nFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone_test_rebined">
    pFDpCD_Plot_Clone_test_rebined->Draw("colz");
    pFDpCD_Plot_Clone_test_rebined->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone_test_rebined);
//    Histogram_list->Add(pFDpCD_Plot_Clone_test_rebined);

    Canvas->SaveAs((pFDpCD_Plot_Clone_test_rebined_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving nFDpCD_Plot_Clone">
    nFDpCD_Plot_Clone->SetStats(1);

    nFDpCD_Plot_Clone->GetXaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetXaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetXaxis()->CenterTitle(true);
    nFDpCD_Plot_Clone->GetYaxis()->SetTitleSize(0.06);
    nFDpCD_Plot_Clone->GetYaxis()->SetLabelSize(0.0425);
    nFDpCD_Plot_Clone->GetYaxis()->CenterTitle(true);

    nFDpCD_Plot_Clone->Draw("colz");
    nFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(nFDpCD_Plot_Clone);
//    Histogram_list->Add(nFDpCD_Plot_Clone);

    Canvas->SaveAs((nFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(1);
    nFDpCD_Plot_Clone->Draw("colz");
    nFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving pFDpCD_Plot_Clone">
    Canvas->cd();
    pFDpCD_Plot_Clone->Draw("colz");
    pFDpCD_Plot_Clone->SetStats(1);
    FSRatio_plot_2D_TFolder->Add(pFDpCD_Plot_Clone);
//    Histogram_list->Add(pFDpCD_Plot_Clone);

    Canvas->SaveAs((pFDpCD_Plot_Clone_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(2);
    pFDpCD_Plot_Clone->Draw("colz");
    pFDpCD_Plot_Clone->SetStats(1);
    //</editor-fold>

    //<editor-fold desc="Plotting and saving FSRatio_plot_2D">
    Canvas->cd();
    FSRatio_plot_2D->GetXaxis()->SetTitleSize(0.06);
    FSRatio_plot_2D->GetXaxis()->SetLabelSize(0.0425);
    FSRatio_plot_2D->GetXaxis()->CenterTitle(true);
    FSRatio_plot_2D->GetYaxis()->SetTitleSize(0.06);
    FSRatio_plot_2D->GetYaxis()->SetLabelSize(0.0425);
    FSRatio_plot_2D->GetYaxis()->CenterTitle(true);

    FSRatio_plot_2D->Divide(pFDpCD_Plot_Clone);
    FSRatio_plot_2D->SetStats(0);
    FSRatio_plot_2D->SetMaximum(10);
    FSRatio_plot_2D->SetMinimum(0.1);
    FSRatio_plot_2D->Draw("colz");
    Canvas->SetLogz(1);

    Histogram_list->Add(FSRatio_plot_2D_TFolder);
    Histogram_list->Add(FSRatio_plot_2D);
    Canvas->SaveAs((FSRatio_plot_2D_SaveName).c_str());
    Canvas->Clear();

    CanvasMulti->cd(3);
    CanvasMulti->cd(3)->SetLogz(1);
    FSRatio_plot_2D->Draw("colz");
    FSRatio_plot_2D->SetStats(0);
    FSRatio_plot_2D->SetMaximum(10);
    FSRatio_plot_2D->SetMinimum(0.1);
    CanvasMulti->SaveAs((FSRatio_plot_2D_SaveName.substr(0, FSRatio_plot_2D_SaveName.find_last_of(".png") - 3) + "_RegularM.png").c_str());
    CanvasMulti->Clear();
    //</editor-fold>

//    cout << "\n\n\nFSRatioType = " << FSRatioType << "\n\n\n";
//    cout << "\n\n\nFSRatioRecTitle = " << FSRatioRecTitle << "\n\n\n";
//    cout << "\n\n\nFSRatio_plot_2D_SaveName = " << FSRatio_plot_2D_SaveName << "\n\n\n";

    delete Canvas;
    delete CanvasMulti;
}

#endif //DRAWANDSAVEFSRATIO_H
