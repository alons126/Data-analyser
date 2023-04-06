//
// Created by alons on 14/02/2023.
//

#ifndef PROJECT_HPLOT1D_H
#define PROJECT_HPLOT1D_H

/*
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
*/

class hPlot1D {
protected:
    /* 1D histogram declaration */
    TH1D *Histogram1D;

    /* Histogram titles & lables */
    map<std::string, std::string> Histogram1DTitles{{"FinalState",""},{"DetectorRegion",""}}; // contains HistogramStatTitle, HistogramTitle, XaxisTitle, YaxisTitle, Histogram1DTitleReactions, FinalState and DetectorRegion
    std::string Histogram1DTitleReactions;
    std::string FinalState;
    std::string DetectorRegion;

    /* Histogram xAxis limits and #bins */
    int HistogramNumberOfXBins; // default #bins for 1D histogram is 100 (see constructor)
//    double LowerXlim, UpperXlim;
//    vector<double> = Histogram1DXLimits;
    vector<double> HistogramXAxisLimits;  // {LowerXlim, UpperXlim}

    /* Histogram normalization settings */
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;

    /* Histogram appearance settings */
    int LineColor = 1;
    int LineWidth = 2;
//    double TitleSize = 0.06, LabelSizex = 0.0425, LabelSizey = 0.0425;
    vector<double> Histogram1DTitleSizes = {0.06, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey}
    bool CenterTitle = true;
    bool ShowStats = true;

    /* Histogram stack stuff */
    THStack *Histogram1DStack;
    bool AddToStack = false;

    /* Histogram cuts settings */
    bool ApplyPlotCuts = false;
    //vector<double> - for cuts
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

    /* Histogram scale settings */
    bool LogScalePlot = false, LinearScalePlot = true;

    /* Histogram save name and path */
    std::string Histogram1DSaveName = "Histogram1D";
    std::string Histogram1DSaveNamePath = "./";


    TList *HistogramTList;
    TFolder *HistogramTFolder;

    bool Title2 = false;

public:
// Constructor declaration:
    hPlot1D(std::string h1DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 100);

    hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 100);

    hPlot1D(std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 100);

    // Histogram methods:
    void hFill(double data) { Histogram1D->Fill(data); }

    // histPlotter1D function (old):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                       int lineWidth, bool logScalePlot, bool linearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                       std::string Histogram1DSaveNamePath,
                       std::string finalState, int kColor = 1, bool centerTitle = true, bool addToStack = false, bool showStats = true, bool title2 = false,
                       bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (unsymmetric cuts for SF plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                       THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                       bool addToStack = false,
                       bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0,
                       double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (one-sided cuts for Nphe plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                       THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                       bool addToStack = false,
                       bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1D function (Chi2 for separate plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3,
                       std::string Particle,
                       double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool logScalePlot, bool linearScalePlot,
                       THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, int kColor = 1, bool centerTitle = true,
                       bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1D function (applying all Chi2 cuts):
    void histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3,
                       std::string Particle1,
                       std::string Particle2, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool logScalePlot,
                       bool linearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, int kColor = 1,
                       bool centerTitle = true, bool addToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts1 = 0,
                       double plot_Xmax1 = 0, double plot_cuts2 = 0, double plot_Xmax2 = 0, bool plot_max = true);

    void hDraw() { Histogram1D->Draw(); }

    void hSave(TCanvas *Histogram1DCanvas) {
        Histogram1DCanvas->SaveAs((Histogram1DSaveNamePath + "/" + Histogram1DSaveName + ".png").c_str());
    }

//  Set methods:
    void SetHistogram1D(TH1D *Histogram) { Histogram1D = Histogram; }

    void SetHistogramTitle(std::string hTitle) { Histogram1DTitles["HistogramTitle"] = hTitle; }

    void SetHistogramStatTitle(std::string sTitle) { Histogram1DTitles["HistogramStatTitle"] = sTitle; }

    void SetXaxisTitle(std::string xTitle) { Histogram1DTitles["XaxisTitle"] = xTitle; }

    void SetYaxisTitle(std::string yTitle) { Histogram1DTitles["YaxisTitle"] = yTitle; }

    void SetHistogramHeadTitles(std::string hTitle, std::string sTitle) {
        Histogram1DTitles["HistogramTitle"] = hTitle;
        Histogram1DTitles["HistogramStatTitle"] = sTitle;
    }

    void SetHistogramTitles(std::string hTitle, std::string sTitle, std::string xTitle, std::string yTitle) {
        Histogram1DTitles["HistogramTitle"] = hTitle;
        Histogram1DTitles["HistogramStatTitle"] = sTitle;
        Histogram1DTitles["XaxisTitle"] = xTitle;
        Histogram1DTitles["YaxisTitle"] = yTitle;
    }

    void SetHistogramTitles(std::string hTitle, std::string sTitle, std::string xTitle) {
        Histogram1DTitles["HistogramTitle"] = hTitle;
        Histogram1DTitles["HistogramStatTitle"] = sTitle;
        Histogram1DTitles["XaxisTitle"] = xTitle;
    }

    void SetAxisTitles(std::string xTitle, std::string yTitle) {
        Histogram1DTitles["XaxisTitle"] = xTitle;
        Histogram1DTitles["YaxisTitle"] = yTitle;
    }

    void SetHistogramNumberOfXBins(int NumberOfXBins) { HistogramNumberOfXBins = NumberOfXBins; }

    void SetUpperXlim(double uXlim) { HistogramXAxisLimits.at(1) = uXlim; }

    void SetLowerXlim(double lXlim) { HistogramXAxisLimits.at(0) = lXlim; }

//    void SetUpperYlim(double uYlim) { UpperYlim = uYlim; }

//    void SetLowerYlim(double lYlim) { LowerYlim = lYlim; }

    void SetXAxisLimits(double uXlim, double lXlim) {
        HistogramXAxisLimits.at(1) = uXlim;
        HistogramXAxisLimits.at(0) = lXlim;
    }

//    void SetYAxisLimits(double uYlim, double lYlim) {
//        UpperYlim = uYlim;
//        LowerYlim = lYlim;
//    }

//    void SetAxisLimits(double uXlim, double lXlim, double uYlim, double lYlim) {
//        HistogramXAxisLimits.at(1) = uXlim;
//        HistogramXAxisLimits.at(0) = lXlim;
//        UpperYlim = uYlim;
//        LowerYlim = lYlim;
//    }

    void SetNormalizeHistogram(bool nHistogram) { NormalizeHistogram = nHistogram; }

    void SetCustomNormalization(bool cNormalization) { CustomNormalization = cNormalization; }

    void SetCustomNormalizationFactor(bool cnFactor) { CustomNormalizationFactor = cnFactor; }

    void SetHistogram1DTitleReactions(std::string h1drReactions) { Histogram1DTitleReactions = h1drReactions; }

    void SetTitleSize(double tSize) { Histogram1DTitleSizes.at(0) = tSize; }

    void SetLabelSizex(double lSizex) { Histogram1DTitleSizes.at(1) = lSizex; }

//    void SetLabelSizey(double lSizey) { LabelSizey = lSizey; }

//    void SetHistogramList(TList *hList) { *HistogramList = hList; }

    void SetLineWidth(int lWidth) { LineWidth = lWidth; }

    void SetLogScalePlot(bool lsPlot) { LogScalePlot = lsPlot; }

    void SetLinearScalePlot(bool lsPlot) { LinearScalePlot = lsPlot; }

//    void SetHistogram1DStack(THStack *h1dStack) { *Histogram1DStack = h1dStack; }

    void SetHistogram1DSaveName(std::string h1dsName) { Histogram1DSaveName = h1dsName; }

    void SetHistogram1DSaveNamePath(std::string h1dsNamePath) { Histogram1DSaveNamePath = h1dsNamePath; }

    void SetFinalState(std::string fState) { Histogram1DTitles["FinalState"] = fState; }

    void SetDetectorRegion(std::string dRegion) { Histogram1DTitles["DetectorRegion"] = dRegion; }

    void SetkColor(int kCol = 1) { LineColor = kCol; }

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

    std::string GetHistogramTitle() { return Histogram1DTitles["HistogramTitle"]; }

    std::string GetHistogramStatTitle(std::string sTitle) { return Histogram1DTitles["HistogramStatTitle"]; }

    std::string GetXaxisTitle(std::string xTitle) { return Histogram1DTitles["XaxisTitle"] = xTitle; }

    std::string GetYaxisTitle(std::string yTitle) { return Histogram1DTitles["YaxisTitle"] = yTitle; }

    int GetHistogramNumberOfXBins(int NumberOfXBins) { return HistogramNumberOfXBins; }

    double GetUpperXlim() { return HistogramXAxisLimits.at(1); }

    double GetLowerXlim() { return HistogramXAxisLimits.at(0); }

    bool GetNormalizeHistogram() { return NormalizeHistogram; }

    bool GetCustomNormalization() { return CustomNormalization; }

    bool GetCustomNormalizationFactor() { return CustomNormalizationFactor; }

    std::string GetHistogram1DTitleReactions() { return Histogram1DTitleReactions; }

    double GetTitleSize() { return Histogram1DTitleSizes.at(0); }

    double GetLabelSizeX() { return Histogram1DTitleSizes.at(1); }

    double GetLabelSizeY() { return Histogram1DTitleSizes.at(2); }

    int GetLineWidth() { return LineWidth; }

    bool GetLogScalePlot() { return LogScalePlot; }

    bool GetLinearScalePlot() { return LinearScalePlot; }

    std::string GetHistogram1DSaveName() { return Histogram1DSaveName; }

    std::string GetHistogram1DSaveNamePath() { return Histogram1DSaveNamePath; }

    std::string GetFinalState() { return Histogram1DTitles["FinalState"]; }

    std::string GetDetectorRegion() { return Histogram1DTitles["DetectorRegion"]; }

    int GetkColor() { return LineColor; }

    bool GetCenterTitle() { return CenterTitle; }

    bool GetAddToStack() { return AddToStack; }

    bool GetShowStats() { return ShowStats; }

    bool GetTitle2() { return Title2; }

    bool GetApplyPlotCuts() { return ApplyPlotCuts; }

    double GetPlotCuts() { return PlotCuts; }

    double GetPlotXmax() { return PlotXmax; }

    bool GetPlotHistogramMax() { return PlotHistogramMax; }
};

#endif //PROJECT_HPLOT1D_H
