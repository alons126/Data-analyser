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

void DrawAndSaveEfficiencyPlots(string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, string SaveDir, string Efficiency_Type1) {

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
    TH1D *RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot.GetHistogramStatTitle() + " Cloned").c_str());

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot.GetHistogramStatTitle() + " Cloned").c_str());
    //</editor-fold>

    //<editor-fold desc="Setting X axis label">
    string xLabel_REC_temp = RPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_REC = xLabel_REC_temp.substr(0, xLabel_REC_temp.find_last_of('[') - 1);
    string xLabel_Truth_temp = TLPlot_Clone->GetXaxis()->GetTitle();
    string xLabel_Truth = xLabel_Truth_temp.substr(0, xLabel_Truth_temp.find_last_of('[') - 1);
    string xLabel = xLabel_REC + "/" + xLabel_Truth;
    //</editor-fold>

    //<editor-fold desc="Setting title">
    string EfficiencyTitle = RPlot_Clone->GetTitle();
    string EfficiencyType;

    if (findSubstring(EfficiencyTitle, "momentum")) { // for momentum efficiency plots
        EfficiencyType = EfficiencyTitle.substr(0, EfficiencyTitle.find_last_of('m') + 1);
    }
    //</editor-fold>

    //<editor-fold desc="Setting Final state">
    string EfficiencyFS;

    if (findSubstring(EfficiencyTitle, "1e_cut") || findSubstring(EfficiencyTitle, "1e cut") || findSubstring(EfficiencyTitle, "1e Cut")) {
        EfficiencyFS = "1e cut";
    } else if (findSubstring(EfficiencyTitle, "1p")) {
        EfficiencyFS = "1p";
    } else if (findSubstring(EfficiencyTitle, "1n")) {
        EfficiencyFS = "1n";
    } else if (findSubstring(EfficiencyTitle, "1n1p")) {
        EfficiencyFS = "1n1p";
    } else if (findSubstring(EfficiencyTitle, "1e2p")) {
        EfficiencyFS = "1e2p";
    } else if (findSubstring(EfficiencyTitle, "2p")) {
        EfficiencyFS = "2p";
    }
    //</editor-fold>

    TH1D *Efficiency_plot = (TH1D *) Histogram1D_REC->Clone((EfficiencyType + " efficiency" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->SetTitle((EfficiencyType + " efficiency" + " (" + EfficiencyFS + ")").c_str());
    Efficiency_plot->GetXaxis()->SetTitle((xLabel).c_str());
    Efficiency_plot->GetYaxis()->SetTitle("Ratio");

    RPlot_Clone->Draw();
    Histogram_list->Add(RPlot_Clone);
    Canvas->SaveAs("RPlot_Clone.png");
    Canvas->Clear();

    TLPlot_Clone->Draw();
    Histogram_list->Add(TLPlot_Clone);
    Canvas->SaveAs("TLPlot_Clone.png");
    Canvas->Clear();

    Efficiency_plot->Divide(Histogram1D_Truth);
    Efficiency_plot->Draw();
    Histogram_list->Add(Efficiency_plot);
    Canvas->SaveAs("Efficiency.png");
    Canvas->Clear();
}

#endif //DRAWANDSAVEEFFICIENCYPLOTS_H
