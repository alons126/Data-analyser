//
// Created by alons on 27/01/2023.
//

#ifndef DETECTORSIMULATIONANALYSER_C_CLASSES_H
#define DETECTORSIMULATIONANALYSER_C_CLASSES_H

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>

// ======================================================================================================================================================================
// Classes definitions
// ======================================================================================================================================================================

//double beamE; // electron energy declaration

// TargetParameters class -----------------------------------------------------------------------------------------------------------------------------------------------

class TargetParameters {
protected:
    string TargetElement;
    int TargetElementPDG, ProbePDG;
    double BindingEnergyToNucleus;

public:
    void SetTargetElement(string TargetElementName) { TargetElement = TargetElementName; }

    void SetTargetElementPDG(int TargetElementPDGNumber) { TargetElementPDG = TargetElementPDGNumber; }

    void SetProbePDG(int ProbePDGNumber) { ProbePDG = ProbePDGNumber; }

    void SetBindingEnergyToNucleus(double BindingEnergy) { BindingEnergyToNucleus = BindingEnergy; }

    string GetTargetElement() { return TargetElement; }

    int GetTargetElementPDG() { return TargetElementPDG; }

    int GetProbePDG() { return ProbePDG; }

    double GetBindingEnergyToNucleus() { return BindingEnergyToNucleus; }
};

// Histogram class ------------------------------------------------------------------------------------------------------------------------------------------------------

class Histogram {
protected:
    TH1D Histogram1D;
    TH2D Histogram2D;
    string HistogramTitle, HistogramStatTitle, XaxisTitle, YaxisTitle;
    int HistogramNumberOfXBins, HistogramNumberOfYBins;
    double UpperXlim, LowerXlim, UpperYlim, LowerYlim, UpperZlim, LowerZlim;
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;
//    string Histogram1DTitle;
    string Histogram1DTitleReactions;
    double TitleSize;
    double LabelSizex;
    double LabelSizey;
    TList *HistogramList;
    int LineWidth;
    bool LogScalePlot;
    bool LinearScalePlot;
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
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

public:
//  Set methods:
    void SetHistogram1D(TH1D Histogram) { Histogram1D = Histogram; }

    void SetHistogram2D(TH2D Histogram) { Histogram2D = Histogram; }

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

    void SetHistogramNumberOfYBins(int NumberOfYBins) { HistogramNumberOfYBins = NumberOfYBins; }

    void SetHistogramNumberOfBins(int NumberOfXBins, int NumberOfYBins) {
        HistogramNumberOfXBins = NumberOfXBins;
        HistogramNumberOfYBins = NumberOfYBins;
    }

    void SetUpperXlim(double uXlim) { UpperXlim = uXlim; }

    void SetLowerXlim(double lXlim) { LowerXlim = lXlim; }

    void SetUpperYlim(double uYlim) { UpperYlim = uYlim; }

    void SetLowerYlim(double lYlim) { LowerYlim = lYlim; }

    void SetUpperZlim(double uZlim) { UpperZlim = uZlim; }

    void SetLowerZlim(double lZlim) { LowerZlim = lZlim; }

    void SetXAxisLimits(double uXlim, double lXlim) {
        UpperXlim = uXlim;
        LowerXlim = lXlim;
    }

    void SetYAxisLimits(double uYlim, double lYlim) {
        UpperYlim = uYlim;
        LowerYlim = lYlim;
    }

    void SetZAxisLimits(double uZlim, double lZlim) {
        UpperZlim = uZlim;
        LowerZlim = lZlim;
    }

    void SetAxisLimits(double uXlim, double lXlim, double uYlim, double lYlim, double uZlim, double lZlim) {
        UpperZlim = uZlim;
        LowerZlim = lZlim;
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
    TH1D GetHistogram1D() { return Histogram1D; }

    TH2D GetHistogram2D() { return Histogram2D; }

    string GetHistogramTitle() { return HistogramTitle; }

    string GetHistogramStatTitle(string sTitle) { return HistogramStatTitle; }

    string GetXaxisTitle(string xTitle) { return XaxisTitle = xTitle; }

    string GetYaxisTitle(string yTitle) { return YaxisTitle = yTitle; }

    int GetHistogramNumberOfXBins(int NumberOfXBins) { return HistogramNumberOfXBins; }

    int GetHistogramNumberOfYBins(int NumberOfYBins) { return HistogramNumberOfYBins; }

    double GetUpperXlim() { return UpperXlim; }

    double GetLowerXlim() { return LowerXlim; }

    double GetUpperYlim() { return UpperYlim; }

    double GetLowerYlim() { return LowerYlim; }

    double GetUpperZlim() { return UpperZlim; }

    double GetLowerZlim() { return LowerZlim; }

    bool GetNormalizeHistogram() { return NormalizeHistogram; }

    bool GetCustomNormalization() { return CustomNormalization; }

    bool GetCustomNormalizationFactor() { return CustomNormalizationFactor; }

    string GetHistogram1DTitleReactions() { return Histogram1DTitleReactions; }

    double GetTitleSize() { return TitleSize; }

    double GetLabelSizex() { return LabelSizex; }

    double GetLabelSizey() { return LabelSizey; }

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

#endif //DETECTORSIMULATIONANALYSER_C_CLASSES_H
