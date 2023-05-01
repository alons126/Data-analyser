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

#include "GeneralFunctions.h"
#include "../classes/hPlots/hPlot1D.h"

using namespace std;

void DrawAndSaveEfficiencyPlots(string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list) {

    //TODO: add a continue for the case of running over data?

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
    TH1D *RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot.GetHistogramStatTitle() + " - cloned").c_str());

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot.GetHistogramStatTitle() + " - cloned").c_str());
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    string EfficiencyRecTitle = RPlot_Clone->GetTitle();
    string EfficiencyParticle;

    if (findSubstring(EfficiencyRecTitle, "Electron") || findSubstring(EfficiencyRecTitle, "electron")) {
        EfficiencyParticle = "Electron";
    } else if (findSubstring(EfficiencyRecTitle, "Proton") || findSubstring(EfficiencyRecTitle, "proton")) {
        EfficiencyParticle = "Proton";
    } else if (findSubstring(EfficiencyRecTitle, "Neutron") || findSubstring(EfficiencyRecTitle, "neutron")) {
        EfficiencyParticle = "Neutron";
    } else if (findSubstring(EfficiencyRecTitle, "#pi^{+}")) {
        EfficiencyParticle = "Piplus";
    } else if (findSubstring(EfficiencyRecTitle, "#pi^{-}")) {
        EfficiencyParticle = "Piminus";
    } else if (findSubstring(EfficiencyRecTitle, "#K^{+}")) {
        EfficiencyParticle = "Kplus";
    } else if (findSubstring(EfficiencyRecTitle, "#K^{-}")) {
        EfficiencyParticle = "Kminus";
    } else if (findSubstring(EfficiencyRecTitle, "#gamma") || findSubstring(EfficiencyRecTitle, "photon")
               || findSubstring(EfficiencyRecTitle, "Photon")) {
        EfficiencyParticle = "Photon";
    }
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
        EfficiencyXLabel = "Momentum [GeV/c]";
    }
    //</editor-fold>

    //<editor-fold desc="Setting y axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);

    string EfficiencyYLabel = "#epsilon = " + xLabel_REC + "^{rec}" + "/" + xLabel_Truth;
//    string EfficiencyYLabel = "#epsilon = " + xLabel_REC + "/" + xLabel_Truth;
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
    }
    //</editor-fold>

    //<editor-fold desc="Setting save directory">
    string EfficiencySaveDir = TLPlot.GetHistogram1DSaveNamePath() + "/" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency_plots_" + EfficiencyFS + "/";

    system(("mkdir -p " + EfficiencySaveDir).c_str());
    //</editor-fold>

    //<editor-fold desc="Setting save name">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }

    string RPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Rec_Clone.png";
    string TLPlot_Clone_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_" + EfficiencyFS + "_" + "Truth_Clone.png";
    string Efficiency_plot_SaveName = EfficiencySaveDir + sNameFlag + "_" + EfficiencyParticle + "_" + EfficiencyType + "_efficiency.png";
    //</editor-fold>

    TH1D *Efficiency_plot = (TH1D *) Histogram1D_REC->Clone((EfficiencyParticle + " " + EfficiencyType + " #epsilon" + " (" + EfficiencyFS + ")").c_str());
//    TH1D *Efficiency_plot = (TH1D *) Histogram1D_REC->Clone((EfficiencyType + " efficiency" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->SetTitle((EfficiencyTitle + " efficiency #epsilon (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->GetYaxis()->SetTitle((EfficiencyYLabel).c_str());
    Efficiency_plot->GetXaxis()->SetTitle((EfficiencyXLabel).c_str());

    //<editor-fold desc="Plotting and saving RPlot_Clone">
    if (weighted_plots) {
        RPlot_Clone->Draw();
        RPlot_Clone->Sumw2();
    } else {
        RPlot_Clone->Draw();
    }

    Histogram_list->Add(RPlot_Clone);
    Canvas->SaveAs((RPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving TLPlot_Clone">
    if (weighted_plots) {
        TLPlot_Clone->Draw();
        TLPlot_Clone->Sumw2();
    } else {
        TLPlot_Clone->Draw();
    }

    Histogram_list->Add(TLPlot_Clone);
    Canvas->SaveAs((TLPlot_Clone_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plotting and saving Efficiency_plot">
    if (weighted_plots) {
        Efficiency_plot->Draw();
        Efficiency_plot->Sumw2();
    } else {
        Efficiency_plot->Draw();
    }

    Efficiency_plot->Divide(Histogram1D_Truth);
    Efficiency_plot->Draw();
    Histogram_list->Add(Efficiency_plot);
    Canvas->SaveAs((Efficiency_plot_SaveName).c_str());
    Canvas->Clear();
    //</editor-fold>
}

#endif //DRAWANDSAVEEFFICIENCYPLOTS_H
