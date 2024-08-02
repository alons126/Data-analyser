/* root BetaFitAndSave.cpp -q -b */

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

#include "../../source/functions/GeneralFunctions.h"
#include "../../source/constants.h"

#if Independent1DGraphDraw
#include "TitleAligner.cpp"
#endif

using namespace std;

const string ConfigRegion1D_(const string &Histogram2DNameCopy) {
    if (findSubstring(Histogram2DNameCopy, "CD")) {
        return "CD";
    } else {
        return "FD";
    }
}

const string ConfigSName1D_(const string &SampleName) {
    if (findSubstring(SampleName, "sim")) {
        return "s";
    } else if (findSubstring(SampleName, "data")) {
        return "d";
    }

    return "";
}

double SetxOffset1D_(const bool &ShowStats) {
    double xOffset;

    if (!ShowStats) {
        xOffset = -0.11;
    } else {
        xOffset = 0;
    }

    return xOffset;
}

double SetyOffset1D_(const bool &ShowStats) {
    double yOffset;

    if (!ShowStats) {
        yOffset = 0.15;
    } else {
        yOffset = 0;
    }

    return yOffset;
}

void GraphPlotter1D(TList *MScThesisPlotsList, const char *filename, const char *filenameDir, const char *Graph1DName, const string &SampleName, const string &SavePath,
                    const string &SaveName) {
    bool PresMode = false, ExamPresMode = false;

#if PresentationMode
    PresMode = true;
#endif

#if ExamPresentationMode
    ExamPresMode = true;
#endif

    cout << "\n\n";
    hData utilities;

    const string Graph1DNameCopy = Graph1DName;

    TCanvas *c1 = new TCanvas("can1", "can2", utilities.GetStandardCanvasWidth() * 2 * 0.6, utilities.GetStandardCanvasHeight() * 2 * 0.6); // normal res
//    TCanvas *c1 = new TCanvas("can1", "can2", utilities.GetStandardCanvasWidth() * 2, utilities.GetStandardCanvasHeight() * 2); // normal res
    c1->cd()->SetGrid(), c1->cd()->SetBottomMargin(0.14), c1->cd()->SetLeftMargin(0.18), c1->cd()->SetRightMargin(0.12), c1->cd()->SetTopMargin(0.12), c1->cd();

    TFile *file = new TFile(filename);
    if (!file) { cout << "\nInvalid file! Exiting...\n", exit(0); }

    TFolder *momResDir = (TFolder *) file->Get(filenameDir);
    if (!momResDir) { cout << "\nInvalid folder! Exiting...\n", exit(0); }

    TGraph *Graph1D = (TGraph *) momResDir->FindObject(Graph1DNameCopy.c_str());
    if (!Graph1D) { cout << "\nInvalid graph! Exiting...\n", exit(0); }

    double Legend_x1_BaseLine = gStyle->GetStatX(), Legend_y1_BaseLine = gStyle->GetStatY(); // Top right
    double Legend_x2_BaseLine = gStyle->GetStatX(), Legend_y2_BaseLine = gStyle->GetStatY(); // Bottom left

    double Legend_x1_OneLine = Legend_x1_BaseLine, Legend_y1_OneLine = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_OneLine = Legend_x2_BaseLine - 0.2, Legend_y2_OneLine = Legend_y2_BaseLine - 0.25; // Bottom left

    double Legend_x1_TwoLines = Legend_x1_BaseLine, Legend_y1_TwoLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_TwoLines = Legend_x2_BaseLine - 0.2, Legend_y2_TwoLines = Legend_y2_BaseLine - 0.3; // Bottom left

    double Legend_x1_ThreeLines = Legend_x1_BaseLine, Legend_y1_ThreeLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_ThreeLines = Legend_x2_BaseLine - 0.2, Legend_y2_ThreeLines = Legend_y2_BaseLine - 0.35; // Bottom left

    auto *funcList = Graph1D->GetListOfFunctions();

    auto *Legend = (TLegend *) funcList->At(1);
    double Legend_x1 = 0., Legend_y1 = 0.; // Top right
    double Legend_x2 = 0.2, Legend_y2 = 0.05; // Bottom left
    double Legend_xOffset = 0.2, Legend_yOffset = 0.825;
    Legend->SetX1(Legend_x1 + Legend_xOffset), Legend->SetY1(Legend_y1 + Legend_yOffset);
    Legend->SetX2(Legend_x2 + Legend_xOffset), Legend->SetY2(Legend_y2 + Legend_yOffset);
    Legend->SetTextSize(0.03), Legend->SetTextAlign(12);

    auto *FitParam = (TPaveText *) funcList->At(2);
    double FitParam_x1 = 0., FitParam_y1 = 0.; // Top right
    double FitParam_x2 = 0.275, FitParam_y2 = 0.125; // Bottom left
    double FitParam_xOffset = 0.2, FitParam_yOffset = 0.675;
    FitParam->SetX1(FitParam_x1 + FitParam_xOffset), FitParam->SetY1(FitParam_y1 + FitParam_yOffset);
    FitParam->SetX2(FitParam_x2 + FitParam_xOffset), FitParam->SetY2(FitParam_y2 + FitParam_yOffset);
    FitParam->SetTextSize(0.03), FitParam->SetTextAlign(12);

    /* Histogram normalization setup */
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;

    /* Histogram appearance setup */
    int LineColor = 1;
    int LineWidth = 2;
    vector<double> Graph1DTitleSizes = {0.06, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey}
    bool CenterTitle = true;
    bool ShowStats = true;

    /* Histogram stack stuff */
    bool AddToStack = false;

    /* Histogram cuts setup */
    bool ShowPlotCuts = true;
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

    /* Histogram scale setup */
    bool LogScalePlot = false, LinearScalePlot = true;

    c1->cd();

    const double beamE = 5.98636;
    const string Region = ConfigRegion1D_(Graph1DNameCopy);
    string sNameFlag = ConfigRegion1D_(SampleName);

    double Graph1D_integral; // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Graph1D->SetMarkerSize(1.5);
//    Graph1D->SetMarkerSize(2);

    string Graph1D_Title = Graph1D->GetTitle(), Graph1D_Title1 = Graph1D_Title;
    string Graph1D_xLabel = Graph1D->GetXaxis()->GetTitle(), Graph1D_yLabel = Graph1D->GetYaxis()->GetTitle();

    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "#bar{P}^{reco}_{nFD}", "#LTP^{reco}_{nFD}#GT");
    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "#bar{P}^{truth}_{nFD}", "#LTP^{truth}_{nFD}#GT");
    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "#bar{P}^{reco}_{pFD}", "#LTP^{reco}_{pFD}#GT");
    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "#bar{P}^{truth}_{pFD}", "#LTP^{truth}_{pFD}#GT");

    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "[GeV/c]", "[GeV]");
    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "[GeV/c^{2}]", "[GeV]");
    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, "[GeV^{2}/c^{2}]", "[GeV^{2}]");

    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, " (wKC, linear fit)", "");

    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, Graph1D_yLabel, "mean", "slice mean");
    TitleAligner(utilities, Graph1D, Graph1D_Title, Graph1D_xLabel, Graph1D_yLabel, "width", "slice width");

    auto ListOfFunctions = Graph1D->GetListOfFunctions();
    auto *LoadedLegend = (TLegend *) ListOfFunctions->At(1);
    auto *LoadedFitParam = (TPaveText *) ListOfFunctions->At(2);

    LoadedLegend->SetX2NDC(LoadedLegend->GetX2NDC() + 0.05);
    LoadedLegend->SetY1NDC(LoadedLegend->GetY1NDC() - 0.01);
    LoadedFitParam->SetX2NDC(LoadedFitParam->GetX2NDC() - 0.05);

    if (Graph1DNameCopy == "reco_f_Corr_pol1") {

        auto FuncList = Graph1D->GetListOfFunctions();
        FuncList->Clear();

        Graph1D->Draw("ap"), gPad->Update();

        TLine *UpperMomKCut = new TLine(2.2, gPad->GetUymin(), 2.2, gPad->GetUymax());
        UpperMomKCut->SetLineWidth(3), UpperMomKCut->SetLineColor(kBlue), UpperMomKCut->Draw("same");

        double xOffset = SetxOffset1D(false), yOffset = SetyOffset1D(false);
        auto Legend = new TLegend(Legend_x1_OneLine + xOffset + 0.04, Legend_y1_OneLine + yOffset,
                                  Legend_x2_OneLine - 0.05 + xOffset + 0.04, Legend_y2_OneLine + yOffset - 0.01);
        TLegendEntry *LowerMomKCutEntry = Legend->AddEntry(UpperMomKCut, ("#LTP^{reco}_{pFD}#GT = " + to_string_with_precision(2.2, 1) + " [GeV]").c_str(), "l");
        Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
    } else if ((Graph1DNameCopy == "reco_f_Corr_pol1_wKC") &&
               findSubstring(filename, "momResS1T_v3")) {

        string part;

        if (findSubstring(Graph1D->GetTitle(), "Neutron") || findSubstring(Graph1D->GetTitle(), "neutron")) {
            part = "nFD";
        } else if (findSubstring(Graph1D->GetTitle(), "Proton") || findSubstring(Graph1D->GetTitle(), "proton")) {
            part = "pFD";
        }

        string title0 = Graph1D->GetTitle();
        string title1 = " for 1 #leq#LTP^{reco}_{" + part + "}#GT#leq 2.2 [GeV]";
        string title = title0 + title1 + " (stage 1)";
        Graph1D->SetTitle(title.c_str());

        Graph1D->Draw("ap"), gPad->Update();
    } else if ((Graph1DNameCopy == "reco_f_Smear_pol1_wKC") &&
               findSubstring(filename, "momResS2T_v3")) {

        string part;

        if (findSubstring(Graph1D->GetTitle(), "Neutron") || findSubstring(Graph1D->GetTitle(), "neutron")) {
            part = "nFD";
        } else if (findSubstring(Graph1D->GetTitle(), "Proton") || findSubstring(Graph1D->GetTitle(), "proton")) {
            part = "pFD";
        }

        string title0 = Graph1D->GetTitle();
        string title1 = " for 1 #leq#LTP^{reco}_{" + part + "}#GT#leq 2.2 [GeV]";
        string title = title0 + title1 + " (stage 2)";
        Graph1D->SetTitle(title.c_str());

        Graph1D->Draw("ap"), gPad->Update();
    } else if ((Graph1DNameCopy == "truth_f_Smear_pol1_wKC") &&
               findSubstring(Graph1D_Title1, "Proton") &&
               findSubstring(filename, "momResS1T_v3")) {

        auto FuncList = Graph1D->GetListOfFunctions();
        FuncList->Clear();
        Graph1D->GetYaxis()->SetRangeUser(0., 0.02);

        string title0 = Graph1D->GetTitle();
        string title = title0 + " - before smearing";
        Graph1D->SetTitle(title.c_str());

        Graph1D->Draw("ap"), gPad->Update();
    } else if ((Graph1DNameCopy == "truth_f_Smear_pol1_wKC") &&
               findSubstring(Graph1D_Title1, "Proton") &&
               findSubstring(filename, "momResS2RT_v3")) {

        string title0 = Graph1D->GetTitle();
        string title = title0 + " - after smearing";
        Graph1D->SetTitle(title.c_str());

        Graph1D->Draw("ap"), gPad->Update();
    } else {
        Graph1D->Draw("ap"), gPad->Update();
    }

    string SaveNameDir = SavePath + "/" + SaveName + ".png";
    const char *SaveDir = SaveNameDir.c_str();
    c1->SaveAs(SaveDir);
    delete c1;
}
