//
// Created by alons on 14/02/2023.
//

#ifndef HPLOT1D_H
#define HPLOT1D_H

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

#include "../hData/hData.h"

using namespace std;

class hPlot1D {
protected:
    /* 1D histogram declaration */
    TH1D *Histogram1D;

    /* Histogram titles & labels.
     * contains HistogramStatTitle, HistogramTitle, XaxisTitle, YaxisTitle, Histogram1DTitleReactions, FinalState and DetectorRegion. */
    map <std::string, std::string> Histogram1DTitles{{"FinalState",                ""},
                                                     {"DetectorRegion",            ""},
                                                     {"Histogram1DTitleReactions", ""}};
    std::string HistogramStatsTitle, HistogramTitle, XaxisTitle, FinalState, DetectorRegion;
    bool Title2 = false;

    /* Histogram xAxis limits and #bins */
    int HistogramNumberOfXBins; // default #bins for 1D histogram is 100 (see constructor)
    vector<double> HistogramXAxisLimits;  // {LowerXlim, UpperXlim}

    /* Histogram normalization setup */
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;

    /* Histogram appearance setup */
    int LineColor = 1;
    int LineWidth = 2;
    vector<double> Histogram1DTitleSizes = {0.06, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey}
    bool CenterTitle = true;
    bool ShowStats = true;

    /* Histogram stack stuff */
//    THStack *Histogram1DStack;
    bool AddToStack = false;

    /* Histogram cuts setup */
    bool ShowPlotCuts = true;
    //vector<double> - for cuts
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

    /* Histogram scale setup */
    bool LogScalePlot = false, LinearScalePlot = true;

    /* Histogram save name and path */
    std::string Histogram1DSaveNamePath = "./"; // default Histogram1DSaveNamePath
    std::string Histogram1DSaveName = "Histogram1D"; // default Histogram1DSaveName

    bool findSubstring(std::string string1, std::string string2);

public:
    //TODO: add function to integrate histogram

    // Constructor declaration:
    hPlot1D() {} // Default constructor

    hPlot1D(std::string h1DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat,
            double LowerXlim, double UpperXlim, int hnob = 50);
//    hPlot1D(std::string h1DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat,
//            double LowerXlim, double UpperXlim, int hnob = 100);

    hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string sPath, std::string sName, double LowerXlim,
            double UpperXlim, int hnob = 50);
//    hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string sPath, std::string sName, double LowerXlim,
//            double UpperXlim, int hnob = 100);

    hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 50);
//    hPlot1D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 100);

    hPlot1D(std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 50);
//    hPlot1D(std::string hst, std::string ht, std::string xat, double LowerXlim, double UpperXlim, int hnob = 100);

    // histPlotter1D function -------------------------------------------------------------------------------------------------------------------------------------------

    // histPlotter1D function (old):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       std::string Histogram1DTitle, std::string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                       int lineWidth, bool LogScalePlot, bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName,
                       std::string Histogram1DSaveNamePath, std::string finalState, int kColor = 1, bool centerTitle = true, bool AddToStack = false,
                       bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (old, stackless):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       string Histogram1DTitle, string Histogram1DTitleReactions, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list,
                       int lineWidth, bool LogScalePlot, bool LinearScalePlot, string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState,
                       bool centerTitle = true, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1D function (unsymmetric cuts for SF plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                       THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                       bool AddToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_upper_cut = 0,
                       double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (unsymmetric cuts for SF plots, stackless):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                       string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState, bool showStats = true, bool title2 = false,
                       bool apply_plot_cuts = false, double plot_upper_cut = 0, double plot_lower_cut = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (one-sided cuts for Nphe plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       std::string Histogram1DTitle, std::string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                       THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, std::string finalState, int kColor = 1,
                       bool AddToStack = false,
                       bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1D function (one-sided cuts for Nphe plots, stackless):
    void histPlotter1D(TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization, double custom_normalization_factor,
                       string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                       string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState, bool showStats = true, bool title2 = false,
                       bool apply_plot_cuts = false, double plot_cut = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (Chi2 for separate plots):
    void histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, std::string Histogram1DTitle1, std::string Histogram1DTitle2, std::string Histogram1DTitle3,
                       std::string Particle, double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool LogScalePlot,
                       bool LinearScalePlot, THStack *Histogram1DStack, std::string Histogram1DSaveName, std::string Histogram1DSaveNamePath, int kColor = 1,
                       bool centerTitle = true, bool AddToStack = false, bool showStats = true, bool title2 = false, bool apply_plot_cuts = false, double plot_cuts = 0,
                       double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (Chi2 for separate plots, stackless):
    void histPlotter1D(TCanvas *Histogram1DCanvas1, TCanvas *Histogram1DCanvas2, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, string Histogram1DTitle1, string Histogram1DTitle2, string Histogram1DTitle3, string Particle,
                       double titleSize, double labelSizex, double labelSizey, TList *Histogram_list, int lineWidth, bool LogScalePlot, bool LinearScalePlot,
                       string Histogram1DSaveName, const string &Histogram1DSaveNamePath, bool centerTitle = true, bool showStats = true, bool title2 = false,
                       bool apply_plot_cuts = false, double plot_cuts = 0, double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (unified):
    void histPlotter1D(const std::string &SampleName, TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, TList *Histogram_list, THStack *Histogram1DStack, double plot_lower_cut = -9999, double plot_upper_cut = 9999,
                       double plot_Xmax = 0, bool plot_max = true);

    // histPlotter1D function (unified, stackless):
    void histPlotter1D(const std::string &SampleName, TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                       double custom_normalization_factor, TList *Histogram_list, double plot_lower_cut = -9999, double plot_upper_cut = 9999, double plot_Xmax = 0,
                       bool plot_max = true);

    // histPlotter1DwFit function:
    void histPlotter1DwFit(const std::string SampleName, TCanvas *Histogram1DCanvas, TH1D *Histogram1D, bool normalize_Histogram, bool custom_normalization,
                           double custom_normalization_factor, string Histogram1DTitle, string Histogram1DTitleReactions, TList *Histogram_list,
                           string Histogram1DSaveName, const string &Histogram1DSaveNamePath, string finalState, double &plot_Xmax, double &plot_lcut,
                           double &plot_ucut, double factor, bool plot_max = true, string particle = "");

    // FitFunction function:
    Double_t FitFunction(Double_t *v, Double_t *par);

    // LogEventCuts function:
    void hLogEventCuts(clas12::region_part_ptr Particle, double Lower_cut, double Upper_cut, double CutCenter = 0, double Weight = 1);

// Histogram methods:
    void hFill(double data) { Histogram1D->Fill(data); }

    void hFill(double data, double weight) { Histogram1D->Fill(data, weight); }

    void SimpleDraw() { Histogram1D->Draw(); }

    void SimpleSave(TCanvas *Histogram1DCanvas) { Histogram1DCanvas->SaveAs((Histogram1DSaveNamePath + "/" + Histogram1DSaveName + ".png").c_str()); }

    void hDrawAndSave(const std::string &SampleName, TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor,
                      double plot_lower_cut = -9999, double plot_upper_cut = 9999, double plot_Xmax = 0, bool plotMax = false);

    void hDrawAndSave(TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor);

    void hDrawAndSaveWFit(const std::string &SampleName, TCanvas *h1DCanvas, TList *hList, bool normHistogram, bool cNormalization, double cNormalizationFactor,
                          double factor, double &plot_lower_cut, double &plot_upper_cut, double &plot_Xmax, bool plotMax = false);

//    void hDrawAndSave(TCanvas *h1DCanvas, TList *hList, bool nHistogram, bool cNormalization, double cNormalizationFactor, std::string FinalState);

    void ClearListOfFunctions() {
        auto FuncList = Histogram1D->GetListOfFunctions();
        FuncList->Clear();
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

    void SetHistogram1DTitleReactions(std::string h1drReactions) { Histogram1DTitles["Histogram1DTitleReactions"] = h1drReactions; }

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

    void SetShowPlotCuts(bool apCuts = false) { ShowPlotCuts = apCuts; }

    void SetPlotCuts(double pCuts = 0) { PlotCuts = pCuts; }

    void SetPlotXmax(double pXmax = 0) { PlotXmax = pXmax; }

    void SetPlotHistogramMax(bool phMax = true) { PlotHistogramMax = phMax; }

//  Get methods:
    TH1D GetHistogram1D() { return *Histogram1D; }

    TH1D *GetHistogram() const { return Histogram1D; }

    std::string GetHistogramTitle() const { return HistogramTitle; }
//    std::string GetHistogramTitle() { return Histogram1DTitles["HistogramTitle"]; }

    std::string GetHistogramStatTitle() const { return HistogramStatsTitle; }
//    std::string GetHistogramStatTitle() const { return Histogram1DTitles["HistogramStatTitle"]; }

    std::string GetHistogramName() { return Histogram1D->GetName(); } // GetHistogramStatTitle (the ROOT version)

    std::string GetXaxisTitle() const { return XaxisTitle; }

    std::string GetYaxisTitle() { return Histogram1DTitles["YaxisTitle"]; }

    int GetHistogramNumberOfXBins() { return HistogramNumberOfXBins; }

    double GetUpperXlim() { return HistogramXAxisLimits.at(1); }

    double GetLowerXlim() { return HistogramXAxisLimits.at(0); }

    bool GetNormalizeHistogram() const { return NormalizeHistogram; }

    bool GetCustomNormalization() { return CustomNormalization; }

    bool GetCustomNormalizationFactor() { return CustomNormalizationFactor; }

    std::string GetHistogram1DTitleReactions() { return Histogram1DTitles["Histogram1DTitleReactions"]; }

    double GetTitleSize() const { return Histogram1DTitleSizes.at(0); }

    double GetLabelSizeX() const { return Histogram1DTitleSizes.at(1); }

    double GetLabelSizeY() { return Histogram1DTitleSizes.at(2); }

    int GetLineWidth() { return LineWidth; }

    bool GetLogScalePlot() { return LogScalePlot; }

    bool GetLinearScalePlot() { return LinearScalePlot; }

    std::string GetHistogram1DSaveName() const { return Histogram1DSaveName; }

    std::string GetHistogram1DSaveNamePath() const { return Histogram1DSaveNamePath; }
//    std::string GetHistogram1DSaveNamePath() { return Histogram1DSaveNamePath; }

    std::string GetFinalState() const { return FinalState; }
//    std::string GetFinalState() { return Histogram1DTitles["FinalState"]; }

    std::string GetDetectorRegion() const { return DetectorRegion; }

    int GetkColor() { return LineColor; }

    bool GetCenterTitle() const { return CenterTitle; }

    bool GetAddToStack() { return AddToStack; }

    bool GetShowStats() { return ShowStats; }

    bool GetTitle2() { return Title2; }

    bool GetShowPlotCuts() { return ShowPlotCuts; }

    double GetPlotCuts() { return PlotCuts; }

    double GetPlotXmax() { return PlotXmax; }

    bool GetPlotHistogramMax() { return PlotHistogramMax; }
};

#endif //HPLOT1D_H
