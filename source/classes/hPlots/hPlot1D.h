//
// Created by alons on 14/02/2023.
//

#ifndef PROJECT_HPLOT1D_H
#define PROJECT_HPLOT1D_H

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

using namespace std;

class hPlot1D {
protected:
    TH1D *Histogram1D;
    string HistogramStatTitle, HistogramTitle, XaxisTitle, YaxisTitle;
    int HistogramNumberOfXBins;
    //vector<double> - for x,y limits
    double UpperXlim, LowerXlim, UpperYlim, LowerYlim;
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;
    string Histogram1DTitleReactions;
    //vector<double> - for x,y label sizes
    double TitleSize = 0.06, LabelSizex = 0.0425, LabelSizey = 0.0425;
    TList *HistogramTList;
    TFolder *HistogramTFolder;
    int LineWidth = 2;
    bool LogScalePlot = false, LinearScalePlot = true;
    THStack *Histogram1DStack;
    string Histogram1DSaveName;
    string Histogram1DSaveNamePath;
    string FinalState;
    int kColor = 1;
    bool CenterTitle = true;
    bool AddToStack = false;
    bool ShowStats = true;
    bool Title2 = false;
    bool ApplyPlotCuts = false;
    //vector<double> - for cuts
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

public:
// Constructor declaration:
    hPlot1D(string hst, string ht, string xat);

    hPlot1D(string hst, string ht, string xat, int hnoxb, double lxl, double uxl);

    hPlot1D(string hst, string ht, string xat, string h1dsn, string h1dsnp);

    hPlot1D(string hst, string ht, string xat, int hnoxb, double lxl, double uxl, string h1dsn, string h1dsnp);

// Histogram methods:
    void hFill(double data) { Histogram1D->Fill(data); }

    // histPlotter1D function (old):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       string Histogram1DTitle, string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                       int lineWidth, bool logScalePlot, bool linearScalePlot, THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath,
                       string finalState, int kColor = 1, bool centerTitle = true, bool addToStack = false, bool showStats = true, bool title2 = false,
                       bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (unsymmetric cuts for SF plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                       THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, string finalState, int kColor = 1, bool addToStack = false,
                       bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0,
                       double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (one-sided cuts for nphe plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                       THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, string finalState, int kColor = 1, bool addToStack = false,
                       bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1D function (Chi2 for separate plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, string Histogram1DTitle1, string Histogram1DTitle2, string Histogram1DTitle3, string Particle,
                       double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                       THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true,
                       bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1D function (applying all Chi2 cuts):
    void histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, string Histogram1DTitle1, string Histogram1DTitle2, string Histogram1DTitle3, string Particle1,
                       string Particle2, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool logScalePlot,
                       bool linearScalePlot, THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, int kColor = 1,
                       bool centerTitle = true, bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts1 = 0,
                       double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true);

    void hDraw() { Histogram1D->Draw(); }

//  Set methods:
    void SetHistogram1D(TH1D *Histogram) { Histogram1D = Histogram; }

    void SetHistogramTitle(string hTitle) { HistogramTitle = hTitle; }

    void SetHistogramStatTitle(string sTitle) { HistogramStatTitle = sTitle; }

    void SetXaxisTitle(string xTitle) { XaxisTitle = xTitle; }

    void SetYaxisTitle(string yTitle) { YaxisTitle = yTitle; }

    void SetHistogramHeadTitles(string hTitle, string sTitle) {
        HistogramTitle = hTitle;
        HistogramStatTitle = sTitle;
    }

    void SetHistogramTitles(string hTitle, string sTitle, string xTitle, string yTitle) {
        HistogramTitle = hTitle;
        HistogramStatTitle = sTitle;
        XaxisTitle = xTitle;
        YaxisTitle = yTitle;
    }

    void SetHistogramTitles(string hTitle, string sTitle, string xTitle) {
        HistogramTitle = hTitle;
        HistogramStatTitle = sTitle;
        XaxisTitle = xTitle;
    }

    void SetAxisTitles(string xTitle, string yTitle) {
        XaxisTitle = xTitle;
        YaxisTitle = yTitle;
    }

    void SetHistogramNumberOfXBins(int NumberOfXBins) { HistogramNumberOfXBins = NumberOfXBins; }

    void SetUpperXlim(double uXlim) { UpperXlim = uXlim; }

    void SetLowerXlim(double lXlim) { LowerXlim = lXlim; }

    void SetUpperYlim(double uYlim) { UpperYlim = uYlim; }

    void SetLowerYlim(double lYlim) { LowerYlim = lYlim; }

    void SetXAxisLimits(double uXlim, double lXlim) {
        UpperXlim = uXlim;
        LowerXlim = lXlim;
    }

    void SetYAxisLimits(double uYlim, double lYlim) {
        UpperYlim = uYlim;
        LowerYlim = lYlim;
    }

    void SetAxisLimits(double uXlim, double lXlim, double uYlim, double lYlim) {
        UpperXlim = uXlim;
        LowerXlim = lXlim;
        UpperYlim = uYlim;
        LowerYlim = lYlim;
    }

    void SetNormalizeHistogram(bool nHistogram) { NormalizeHistogram = nHistogram; }

    void SetCustomNormalization(bool cNormalization) { CustomNormalization = cNormalization; }

    void SetCustomNormalizationFactor(bool cnFactor) { CustomNormalizationFactor = cnFactor; }

    void SetHistogram1DTitleReactions(string h1drReactions) { Histogram1DTitleReactions = h1drReactions; }

    void SetTitleSize(double tSize) { TitleSize = tSize; }

    void SetLabelSizex(double lSizex) { LabelSizex = lSizex; }

    void SetLabelSizey(double lSizey) { LabelSizey = lSizey; }

//    void SetHistogramList(TList *hList) { *HistogramList = hList; }

    void SetLineWidth(int lWidth) { LineWidth = lWidth; }

    void SetLogScalePlot(bool lsPlot) { LogScalePlot = lsPlot; }

    void SetLinearScalePlot(bool lsPlot) { LinearScalePlot = lsPlot; }

//    void SetHistogram1DStack(THStack *h1dStack) { *Histogram1DStack = h1dStack; }

    void SetHistogram1DSaveName(string h1dsName) { Histogram1DSaveName = h1dsName; }

    void SetHistogram1DSaveNamePath(string h1dsNamePath) { Histogram1DSaveNamePath = h1dsNamePath; }

    void SetFinalState(string fState) { FinalState = fState; }

    void SetkColor(int kCol = 1) { kColor = kCol; }

    void SetCenterTitle(bool cTitle = true) { CenterTitle = cTitle; }

    void SetAddToStack(bool atStack = false) { AddToStack = atStack; }

    void SetShowStats(bool sStats = true) { ShowStats = sStats; }

    void SetTitle2(bool T2 = false) { Title2 = T2; }

    void SetApplyPlotCuts(bool apCuts = false) { ApplyPlotCuts = apCuts; }

    void SetPlotCuts(double pCuts = 0) { PlotCuts = pCuts; }

    void SetPlotXmax(double pXmax = 0) { PlotXmax = pXmax; }

    void SetPlotHistogramMax(bool phMax = true) { PlotHistogramMax = phMax; }

//  Get methods:
    TH1D GetHistogram1D() { return *Histogram1D; }

    string GetHistogramTitle() { return HistogramTitle; }

    string GetHistogramStatTitle(string sTitle) { return HistogramStatTitle; }

    string GetXaxisTitle(string xTitle) { return XaxisTitle = xTitle; }

    string GetYaxisTitle(string yTitle) { return YaxisTitle = yTitle; }

    int GetHistogramNumberOfXBins(int NumberOfXBins) { return HistogramNumberOfXBins; }

    double GetUpperXlim() { return UpperXlim; }

    double GetLowerXlim() { return LowerXlim; }

    bool GetNormalizeHistogram() { return NormalizeHistogram; }

    bool GetCustomNormalization() { return CustomNormalization; }

    bool GetCustomNormalizationFactor() { return CustomNormalizationFactor; }

    string GetHistogram1DTitleReactions() { return Histogram1DTitleReactions; }

    double GetTitleSize() { return TitleSize; }

    double GetLabelSizeX() { return LabelSizex; }

    double GetLabelSizeY() { return LabelSizey; }

//    TList GetHistogramList() { return *HistogramList; }

    int GetLineWidth() { return LineWidth; }

    bool GetLogScalePlot() { return LogScalePlot; }

    bool GetLinearScalePlot() { return LinearScalePlot; }

//    THStack GetHistogram1DStack() { return *Histogram1DStack; }

    string GetHistogram1DSaveName() { return Histogram1DSaveName; }

    string GetHistogram1DSaveNamePath() { return Histogram1DSaveNamePath; }

    string GetFinalState() { return FinalState; }

    int GetkColor() { return kColor; }

    bool GetCenterTitle() { return CenterTitle; }

    bool GetAddToStack() { return AddToStack; }

    bool GetShowStats() { return ShowStats; }

    bool GetTitle2() { return Title2; }

    bool GetApplyPlotCuts() { return ApplyPlotCuts; }

    double GetPlotCuts() { return PlotCuts; }

    double GetPlotXmax() { return PlotXmax; }

    bool GetPlotHistogramMax() { return PlotHistogramMax; }
};

//<editor-fold desc="Constructor definitions">
hPlot1D::hPlot1D(string hst, string ht, string xat) {
    HistogramStatTitle = hst;
    HistogramTitle = ht;
    XaxisTitle = xat;
    string TandX = HistogramTitle + ";" + XaxisTitle + ";";
    Histogram1D = new TH1D(HistogramStatTitle.c_str(), TandX.c_str(), HistogramNumberOfXBins, LowerXlim, UpperXlim);
}

hPlot1D::hPlot1D(string hst, string ht, string xat, int hnoxb, double lxl, double uxl) {
    HistogramStatTitle = hst;
    HistogramTitle = ht;
    XaxisTitle = xat;
    HistogramNumberOfXBins = hnoxb;
    LowerXlim = lxl;
    UpperXlim = uxl;

    string TandX = HistogramTitle + ";" + XaxisTitle + ";";
    Histogram1D = new TH1D(HistogramStatTitle.c_str(), TandX.c_str(), HistogramNumberOfXBins, LowerXlim, UpperXlim);
}

hPlot1D::hPlot1D(string hst, string ht, string xat, string h1dsn, string h1dsnp) {
    HistogramStatTitle = hst;
    HistogramTitle = ht;
    XaxisTitle = xat;
    Histogram1DSaveName = h1dsn;
    Histogram1DSaveNamePath = h1dsnp;

    string TandX = HistogramTitle + ";" + XaxisTitle + ";";
    Histogram1D = new TH1D(HistogramStatTitle.c_str(), TandX.c_str(), HistogramNumberOfXBins, LowerXlim, UpperXlim);
}

hPlot1D::hPlot1D(string hst, string ht, string xat, int hnoxb, double lxl, double uxl, string h1dsn, string h1dsnp) {
    HistogramStatTitle = hst;
    HistogramTitle = ht;
    XaxisTitle = xat;
    HistogramNumberOfXBins = hnoxb;
    LowerXlim = lxl;
    UpperXlim = uxl;
    Histogram1DSaveName = h1dsn;
    Histogram1DSaveNamePath = h1dsnp;

    string TandX = HistogramTitle + ";" + XaxisTitle + ";";
    Histogram1D = new TH1D(HistogramStatTitle.c_str(), TandX.c_str(), HistogramNumberOfXBins, LowerXlim, UpperXlim);
}
//</editor-fold>

//<editor-fold desc="histPlotter1D definitions">

//<editor-fold desc="histPlotter1D function (old)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            string Histogram1DTitle, string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                            int lineWidth, bool logScalePlot, bool linearScalePlot, THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath,
                            string finalState, int kColor = 1, bool centerTitle = true, bool addToStack = false, bool showStats = true, bool title2 = false,
                            bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
//    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.9, y_2 = 0.7;
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
//    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

//    if (showStats == false) {
//        Histogram1D->SetStats(0);
//        gStyle->SetOptStat(000001111);
//        gROOT->ForceStyle();
////        gStyle->SetOptStat(111110);
////        Histogram1D->SetOptStat(111110);
//    } else if (showStats == true) {
////        gStyle->SetOptStat(000001111);
//        gStyle->SetOptStat(111110);
//        gROOT->ForceStyle();
//    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

// histPlotter1D function (unsymmetric cuts for SF plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (unsymmetric cuts for SF plots)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                            THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, string finalState, int kColor = 1,
                            bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0,
                            double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
//    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.9, y_2 = 0.7;
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
//    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_upper_cut;
        double Lower_cut = plot_lower_cut;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            if (plot_upper_cut != -1) { upper_cut->Draw("same"); }
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);

            if (plot_lower_cut != -1) { lower_cut->Draw("same"); }
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);

            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

//    if (showStats == false) {
//        Histogram1D->SetStats(0);
//        gStyle->SetOptStat(000001111);
//        gROOT->ForceStyle();
////        gStyle->SetOptStat(111110);
////        Histogram1D->SetOptStat(111110);
//    } else if (showStats == true) {
////        gStyle->SetOptStat(000001111);
//        gStyle->SetOptStat(111110);
//        gROOT->ForceStyle();
//    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

// histPlotter1D function (one-sided cuts for nphe plots) ---------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (one-sided cuts for nphe plots)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                            string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                            THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, string finalState, int kColor = 1,
                            bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0,
                            bool plot_max = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
//    double x_1 = 0.2, y_1 = 0.3, x_2 = 0.9, y_2 = 0.7;
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
//    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title;

        if (title2 == false) {
            title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        } else {
            title = Histogram1DTitle + " (" + finalState + ")";
        }
//        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Cut = plot_cut;
        double plot_xmax = plot_Xmax;
        TLine *plot_cut = new TLine(Cut, 0., Cut, gPad->GetFrame()->GetY2());
        plot_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            plot_cut->Draw("same");
            plot_cut->SetLineColor(kRed);
//            plot_cut->SetLineColor(kMagenta);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            plot_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(plot_cut, ("Lower cut = " + to_string_with_precision(Cut, 0)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(plot_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

//    if (showStats == false) {
//        Histogram1D->SetStats(0);
//        gStyle->SetOptStat(000001111);
//        gROOT->ForceStyle();
////        gStyle->SetOptStat(111110);
////        Histogram1D->SetOptStat(111110);
//    } else if (showStats == true) {
////        gStyle->SetOptStat(000001111);
//        gStyle->SetOptStat(111110);
//        gROOT->ForceStyle();
//    }

    if (logScalePlot == true) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();
}
//</editor-fold>

// histPlotter1D function (Chi2 for separate plots) ---------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (Chi2 for separate plots)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                            double custom_normalization_factor, string Histogram1DTitle1, string Histogram1DTitle2, string Histogram1DTitle3, string Particle,
                            double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                            THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true,
                            bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                            bool plot_max = true) {

    double TitleFontSize = gStyle->GetTitleFontSize();
    Histogram1DCanvas2->cd();

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        string HistogramTitle =
                "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - #bar{#chi^{2}}_{" + Particle + "," + Histogram1DTitle3 +
                "}| #leq " + to_string_with_precision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }

    } else if (normalize_Histogram == false) {
        string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle =
                "#splitline{" + title + "}{        (|#chi^{2}_{" + Particle + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle + "," + Histogram1DTitle3 +
                "})_{max}| #leq " + to_string_with_precision(plot_cuts) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_cuts + plot_Xmax;
        double Lower_cut = -plot_cuts + plot_Xmax;
        double plot_xmax = plot_Xmax;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax, 0., plot_Xmax, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
//            upper_cut->SetLineColor(kMagenta);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
//            lower_cut->SetLineColor(kMagenta);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
//            lower_cut->SetLineColor(kMagenta);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax)).c_str(), "l");
//            TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(lower_cut, ("Peak location = " + to_string_with_precision(max_location)).c_str(), "l");
            }

            Cut_legend->Draw("same");
        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas2->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas2->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas2->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    Histogram1DCanvas1->cd();
}
//</editor-fold>

// histPlotter1D function (applying all Chi2 cuts) ----------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="histPlotter1D function (applying all Chi2 cuts)">
void hPlot1D::histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                            double custom_normalization_factor, string Histogram1DTitle1, string Histogram1DTitle2, string Histogram1DTitle3, string Particle1,
                            string Particle2, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool logScalePlot,
                            bool linearScalePlot, THStack *Histogram1DStack, string Histogram1DSaveName, string Histogram1DSaveNamePath, int kColor = 1,
                            bool centerTitle = true, bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false,
                            double plot_cuts1 = 0, double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true) {

    double TitleFontSize = gStyle->GetTitleFontSize();
    Histogram1DCanvas2->cd();

    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.175, y_1 = 0.3, x_2 = 0.875, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

//    TPaveText *PrametesrBox1 = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
//    PrametesrBox1->SetTextSize(12);
//    PrametesrBox1->SetFillColor(0);
////            PrametesrBox1->SetTextAlign(12);
//    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1)).c_str());
//    PrametesrBox1->AddText(("|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2)).c_str());

//    TPaveText *PrametesrBox = new TPaveText(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 - 0.4, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 - 0.05, "NDC");
//    PrametesrBox->SetTextSize(12);
//    PrametesrBox->SetFillColor(0);
////            PrametesrBox->SetTextAlign(12);
//    PrametesrBox->AddText(("|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle1 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1)).c_str());
//    PrametesrBox->AddText(("|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "} - ({#chi^{2}}_{" + Particle2 + "," +
//                           Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2)).c_str());

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")" + " - Normalized";
        string HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," +
                                Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," +
                                Histogram1DTitle3 + "} -(#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2) +
                                ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
//            PrametesrBox1->Draw();
        }

    } else if (normalize_Histogram == false) {
        string HistogramTitle, title, subtitle;

        if (title2 == false) {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ", " + Histogram1DTitle3 + ")";
        } else {
            title = Histogram1DTitle1 + " (" + Histogram1DTitle2 + ")";
        }

        HistogramTitle = "#splitline{" + title + "}{" + "(|#chi^{2}_{" + Particle1 + "," + Histogram1DTitle3 + "} - (#chi^{2}_{" + Particle1 + "," +
                         Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts1) + ", " + "|#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 +
                         "} - (#chi^{2}_{" + Particle2 + "," + Histogram1DTitle3 + "})_{max}| #leq " + to_string_with_precision(plot_cuts2) + ")}";
        Histogram1D->SetTitle(HistogramTitle.c_str());
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
        gStyle->SetTitleFontSize(0.04);
        gROOT->ForceStyle();

        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
//            PrametesrBox1->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram1D->SetLineWidth(lineWidth);
    Histogram_list->Add(Histogram1D);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (apply_plot_cuts == true) {
        gPad->Update();
        double Upper_cut = plot_cuts1 + plot_Xmax1;
        double Lower_cut = -plot_cuts1 + plot_Xmax1;
        double plot_xmax1 = plot_Xmax1;
        TLine *upper_cut = new TLine(Upper_cut, 0., Upper_cut, gPad->GetFrame()->GetY2());
        upper_cut->SetLineWidth(lineWidth);
        TLine *lower_cut = new TLine(Lower_cut, 0., Lower_cut, gPad->GetFrame()->GetY2());
        lower_cut->SetLineWidth(lineWidth);
        TLine *max_location = new TLine(plot_Xmax1, 0., plot_Xmax1, gPad->GetFrame()->GetY2());
        max_location->SetLineWidth(lineWidth + 1);
        auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);

        if (Histogram1D->Integral() != 0.) {
            upper_cut->Draw("same");
            upper_cut->SetLineColor(kBlue);
            lower_cut->Draw("same");
            lower_cut->SetLineColor(kRed);
            if (plot_max == true) {
                max_location->Draw("same");
                max_location->SetLineColor(kGreen);
            }

            TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, ("Upper cut = " + to_string_with_precision(Upper_cut)).c_str(), "l");
            TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, ("Lower cut = " + to_string_with_precision(Lower_cut)).c_str(), "l");

            if (plot_max == true) {
                TLegendEntry *Cut_max_location_lim = Cut_legend->AddEntry(max_location, ("Peak location = " + to_string_with_precision(plot_xmax1)).c_str(), "l");
            }

            Cut_legend->Draw("same");

        }
    }

    if (logScalePlot == true) {
        Histogram1DCanvas2->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (linearScalePlot == true) {
        Histogram1DCanvas2->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + Histogram1DTitle3 + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas2->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas2->Clear();
    gStyle->SetTitleFontSize(TitleFontSize);
    gROOT->ForceStyle();

    Histogram1DCanvas1->cd();
}
//</editor-fold>

//</editor-fold>

#endif //PROJECT_HPLOT1D_H
