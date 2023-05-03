//
// Created by alons on 02/05/2023.
//

#ifndef BETAFIT_H
#define BETAFIT_H

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
#include "../classes/DSCuts/DSCuts.h"
#include "../classes/hPlots/hPlot1D.h"

using namespace std;

Double_t FitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
//    if (par[1] != 0) { arg = (v[0] - 1) / par[1]; } // 2 parameters
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}

void BetaFit(const string &SampleName, DSCuts &Beta_cuts, const hPlot1D &BetaPlot, TList *Histogram_list) {

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

    //<editor-fold desc="Setting sNameFlag">
    string sNameFlag;

    if (findSubstring(SampleName, "simulation")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }
    //</editor-fold>

    TH1D *hBeta = BetaPlot.GetHistogram();
    TH1D *hBeta_Clone = (TH1D *) hBeta->Clone((BetaPlot.GetHistogramStatTitle() + " - fitted").c_str());

    TF1 *func = new TF1("fit", FitFunction, 0, 2, 3); // create a function with 3 parameters in the range [-3,3]
    func->SetLineColor(kRed);

    double max = hBeta_Clone->GetMaximum();
//    double max = hpx->GetBinCenter(hpx->GetMaximumBin());
    double mean = hBeta_Clone->GetMean();

    func->SetParameters(max, mean, 0.001);
//    func->SetParameters(25, 1, 0.001);
//    func->SetParameters(300, 1, 0.001);
    func->SetParNames("Constant", "Mean_value", "Sigma");
//    func->SetParLimits(0, 21, 50);
////    func->SetParLimits(0, 215, 500);
    func->SetParLimits(0, 0.7 * max, 1.2 * max); // amp limits
//    func->SetParLimits(1, 0.999, 1.05);
//    func->SetParLimits(2, 0.00001, 0.05);
    hBeta_Clone->Fit("fit");
    hBeta_Clone->SetLineColor(kBlack);
    hBeta_Clone->Draw();

    TF1 *fit = hBeta_Clone->GetFunction("fit");

    Beta_cuts.SetUpperCut(fit->GetParameter(2));

    Histogram_list->Add(hBeta_Clone);

    string hBeta_CloneSaveNameDir = BetaPlot.GetHistogram1DSaveNamePath() + sNameFlag + BetaPlot.GetHistogram1DSaveName() + +"_fitted.png";
    const char *SaveDir = hBeta_CloneSaveNameDir.c_str();
    Canvas->SaveAs(SaveDir);

    Canvas->Clear();

    /*//    Histogram1D->GetListOfFunctions()->Remove(Histogram1D->GetFunction("gaus"));

    TF1 *fit = hpx->GetFunction("gaus");
    double Amp = fit->GetParameter(0); // get p0
    double Mean = fit->GetParameter(1); // get p1
    double Std = fit->GetParameter(2); // get p2

    cout << "\n\n\nAmp:\t" << Amp << "\n";
    cout << "Mean:\t" << Mean << "\n";
    cout << "Std:\t" << Std << "\n\n\n\n";

    double titleSize = 0.06;
    double labelSizex = 0.0425;
    double labelSizey = 0.0425;

    int lineWidth = 2;
    bool logScalePlot = false;
    bool linearScalePlot = true;

    int kColor = kBlack;
    bool centerTitle = true;
    bool showStats = true;
    bool title2 = false;
    bool apply_plot_cuts = true;

    double factor = 2.;
    double plot_cuts = factor * Std;
    double plot_Xmax = Mean;

    double Upper_cut = plot_cuts + plot_Xmax;
    double Lower_cut = -plot_cuts + plot_Xmax;
    double plot_xmax = plot_Xmax;
    TLine *upper_cut = new
    TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
    upper_cut->SetLineWidth(lineWidth);
    TLine *lower_cut = new
    TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
    lower_cut->SetLineWidth(lineWidth);
    TLine *max_location = new
    TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
    max_location->SetLineWidth(lineWidth + 1);

    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

    auto Cut_legend = new
    TLegend(x_1_Cut_legend, y_1_Cut_legend, x_2_Cut_legend, y_2_Cut_legend);
    gPad->Update();
    upper_cut->Draw("same");
    upper_cut->SetLineColor(kBlue);

    lower_cut->Draw("same");
    lower_cut->SetLineColor(kRed);

    max_location->Draw("same");
    max_location->SetLineColor(kGreen);

    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("UC = " + to_string_with_precision(Upper_cut)).c_str(), "l");
    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("LC = " + to_string_with_precision(Lower_cut)).c_str(), "l");
    TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Mean = " + to_string_with_precision(plot_xmax)).c_str(), "l");

    Cut_legend->Draw("same");

    double x_1_FitParam = gStyle->GetStatX(), y_1_FitParam = y_1_Cut_legend - 0.14;
    double x_2_FitParam = gStyle->GetStatX() - 0.2, y_2_FitParam = y_1_Cut_legend - 0.41;

    TPaveText *FitParam = new
    TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "TL && NDC");
//            FitParam->SetTextStyle(1);
    FitParam->SetBorderSize(1);
//    FitParam->UseCurrentStyle();
//    FitParam->SetBBoxY1(1);
//    FitParam->SetTextFont(0);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(11);
    FitParam->AddText(("Fit amp = " + to_string_with_precision(Amp, 3)).c_str());
//    ((TText*)FitParam->GetListOfLines()->Last())->ETitlePos(kCenter);
//    gPad->Update();
    FitParam->AddText(("Fit std = " + to_string_with_precision(Std, 3)).c_str());
    FitParam->AddText(("Fit mean = " + to_string_with_precision(Mean, 3)).c_str());
    FitParam->AddText(("Cuts = std * " + to_string_with_precision(factor, 2)).c_str());
    FitParam->Draw("same");*/

}


#endif //BETAFIT_H
