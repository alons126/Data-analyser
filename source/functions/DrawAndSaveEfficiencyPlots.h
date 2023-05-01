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

#include "../classes/hPlots/hPlot1D.h"

using namespace std;

void DrawAndSaveEfficiencyPlots(string &SampleName, const hPlot1D &TLPlot, const hPlot1D &RPlot, TList *Histogram_list, string SaveDir) {

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

    TH1D *Histogram1D_REC = RPlot.GetHistogram();
//    TH1D *RPlot_Clone = Histogram1D_REC->Clone("RPlot_Clone");
    TH1D *RPlot_Clone = (TH1D *) Histogram1D_REC->Clone((RPlot.GetHistogramStatTitle() + " Cloned").c_str());
    TH1D *Efficiency_plot = (TH1D *) Histogram1D_REC->Clone("Efficiency");

//    Histogram1D_REC->Draw();
////    Histogram_list->Add(Histogram1D_REC);
//    Canvas->SaveAs("Histogram1D_REC.png");
//    Canvas->Clear();

    string Efficiency_Type = Histogram1D_REC->GetXaxis()->GetTitle();
    cout << "Efficiency_Type = " << Efficiency_Type << "\n";

    RPlot_Clone->Draw();
    Histogram_list->Add(RPlot_Clone);
    Canvas->SaveAs("RPlot_Clone.png");
    Canvas->Clear();

    TH1D *Histogram1D_Truth = TLPlot.GetHistogram();
    TH1D *TLPlot_Clone = (TH1D *) Histogram1D_Truth->Clone((TLPlot.GetHistogramStatTitle() + " Cloned").c_str());

    TLPlot_Clone->Draw();
    Histogram_list->Add(TLPlot_Clone);
    Canvas->SaveAs("TLPlot_Clone.png");
    Canvas->Clear();

//    Histogram1D_Truth->Draw();
////    Histogram_list->Add(Histogram1D_Truth);
//    Canvas->SaveAs("Histogram1D_Truth.png");
//    Canvas->Clear();

    Efficiency_plot->Divide(Histogram1D_Truth);
    Efficiency_plot->Draw();
    Histogram_list->Add(Efficiency_plot);
    Canvas->SaveAs("Efficiency.png");
    Canvas->Clear();
}

#endif //DRAWANDSAVEEFFICIENCYPLOTS_H
