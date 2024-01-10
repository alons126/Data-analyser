//
// Created by alons on 07/04/2023.
//

#ifndef HPLOT2D_H
#define HPLOT2D_H

class hPlot2D {
protected:
    /* 2D histogram declaration */
    TH2D *Histogram2D;

    /* Histogram titles & labels.
     * contains HistogramStatTitle, HistogramTitle, XaxisTitle, YaxisTitle, Histogram2DTitleReactions, FinalState and DetectorRegion. */
    map <std::string, std::string> Histogram2DTitles{{"FinalState",                ""},
                                                     {"DetectorRegion",            ""},
                                                     {"Histogram2DTitleReactions", ""}};
    std::string HistogramStatsTitle;

    bool Title2 = false;

    /* Histogram xAxis limits and #bins */
    int HistogramNumberOfXBins; // default #bins for 2D histogram is 250 (see constructor)
    int HistogramNumberOfYBins; // default #bins for 2D histogram is 250 (see constructor)
    vector<double> HistogramXAxisLimits;  // {LowerXlim, UpperXlim}
    vector<double> HistogramYAxisLimits;  // {LowerYlim, UpperYlim}

//    /* Histogram normalization setup */
//    bool NormalizeHistogram;
//    bool CustomNormalization;
//    double CustomNormalizationFactor;

    /* Histogram appearance setup */
    int LineColor = 1;
    int LineWidth = 2;
    vector<double> Histogram2DTitleSizes = {0.06, 0.0425, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey, LabelSizez}
    bool CenterTitle = true;
    bool ShowStats = true;

    /* Histogram stack stuff */
//    THStack *Histogram2DStack;
    bool AddToStack = false;

    /* Histogram cuts setup */
    bool ShowPlotCuts = false;
    //vector<double> - for cuts
    double PlotCuts = 0;
    double PlotXmax = 0;
    bool PlotHistogramMax = true;

    /* Histogram scale setup */
    bool ZLogScalePlot = true;
    bool ZLinearScalePlot = true;

    /* Histogram save name and path */
    std::string Histogram2DSaveName = "Histogram2D"; // default Histogram2DSaveName
    std::string Histogram2DSaveNamePath = "./"; // default Histogram2DSaveNamePath
public:
    // Constructor declaration ------------------------------------------------------------------------------------------------------------------------------------------
    hPlot2D() {} // Default constructor

    hPlot2D(std::string h2DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, double LowerXlim,
            double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250);

    hPlot2D(std::string h2DtReactions, std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat,
            std::string sPath, std::string sName, double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250);

    hPlot2D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, double LowerXlim, double UpperXlim,
            double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250);

    hPlot2D(std::string fState, std::string dRegion, std::string hst, std::string ht, std::string xat, std::string yat, std::string sPath, std::string sName,
            double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250, int hnoYb = 250);

    hPlot2D(std::string hst, std::string ht, std::string xat, std::string yat, double LowerXlim, double UpperXlim, double LowerYlim, double UpperYlim, int hnoXb = 250,
            int hnoYb = 250);

    // histPlotter2D function -------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="histPlotter2D function (regular)">
    /* histPlotter2D function (regular) */
    void histPlotter2D(const std::string &SampleName, TCanvas *Histogram2DCanvas, TH2D *Histogram2D, TList *Histogram_list, std::string Histogram2DSaveNameDir,
                       std::string Histogram2DSaveName, bool showStats = true);

    /* Corresponding hDrawAndSave function (regular) */
    void hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, bool showStats = false);
    //</editor-fold>

    //<editor-fold desc="histPlotter2D function (Beta vs. P plots, all particles)">
    /* histPlotter2D function (Beta vs. P plots, all particles) */
    void histPlotter2D(const std::string &SampleName, TCanvas *Histogram2DCanvas, TH2D *Histogram2D, TList *Histogram_list, std::string Histogram2DSaveNameDir,
                       std::string Histogram2DSaveName, TF1 *Beta_function1, TF1 *Beta_function2, TF1 *Beta_function3, TF1 *Beta_function4, TF1 *Beta_function5,
                       TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function9, bool showStats = true);

    /* Corresponding hDrawAndSave function (Beta vs. P plots, all particles) */
    void hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, TF1 *Beta_function1, TF1 *Beta_function2, TF1 *Beta_function3,
                      TF1 *Beta_function4, TF1 *Beta_function5, TF1 *Beta_function6, TF1 *Beta_function7, TF1 *Beta_function8, TF1 *Beta_function, bool showStats = true);
    //</editor-fold>

    //<editor-fold desc="histPlotter2D function (Beta vs. P plots, single particle)">
    /* histPlotter2D function (Beta vs. P plots, single particle) */
    void histPlotter2D(const std::string &SampleName, TCanvas *Histogram2DCanvas, TH2D *Histogram2D, TList *Histogram_list, std::string Histogram2DSaveNameDir,
                       std::string Histogram2DSaveName, TF1 *Beta_function1, std::string particle1, bool showStats = true, bool plot_legend = true);

    /* Corresponding hDrawAndSave function (Beta vs. P plots, single particle) */
    void
    hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, TF1 *Beta_function1, std::string particle1, bool showStats, bool plot_legend = true);
    //</editor-fold>

    //<editor-fold desc="histPlotter2D function (Beta vs. P plots, 3 particles (by charge))">
    /* histPlotter2D function (Beta vs. P plots, 3 particles (by charge)) */
    void histPlotter2D(const std::string &SampleName, TCanvas *Histogram2DCanvas, TH2D *Histogram2D, TList *Histogram_list, std::string Histogram2DSaveNameDir,
                       std::string Histogram2DSaveName, TF1 *Beta_function1, std::string particle1, TF1 *Beta_function2, std::string particle2, TF1 *Beta_function3,
                       std::string particle3, bool showStats = true, bool plot_legend = true);

    /* Corresponding hDrawAndSave function (Beta vs. P plots, 3 particles (by charge)) */
    void hDrawAndSave(const std::string &SampleName, TCanvas *h2DCanvas, TList *hList, TF1 *Beta_function1, std::string particle1, TF1 *Beta_function2,
                      std::string particle2, TF1 *Beta_function3, std::string particle3, bool showStats = true, bool plot_legend = true);
    //</editor-fold>

    // hDivision function -----------------------------------------------------------------------------------------------------------------------------------------------

    void hAdd(TH2D *h) { Histogram2D->Add(h); }

    void hDivision(TH2D *hDenominator, bool SetLogZLimits = true, double MaxZLim = 1.25);

// Other histogram methods:
    void ApplyZMaxLim(double ZMaxLim) { Histogram2D->SetMaximum(ZMaxLim); }

    void ApplyZMinLim(double ZMinLim) { Histogram2D->SetMinimum(ZMinLim); }

    void hFill(double Xdata, double Ydata) { Histogram2D->Fill(Xdata, Ydata); }

    void hFill(double Xdata, double Ydata, double weight) { Histogram2D->Fill(Xdata, Ydata, weight); }

    void hFillByBin(int BinX, int BinY, double Content) { Histogram2D->SetBinContent(BinX, BinY, Content); }

    void SimpleDraw() { Histogram2D->Draw(); }

    void SimpleSave(TCanvas *Histogram2DCanvas) { Histogram2DCanvas->SaveAs((Histogram2DSaveNamePath + "/" + Histogram2DSaveName + ".png").c_str()); }

    //  Set methods:
    void SetHistogram2D(TH2D *Histogram) { Histogram2D = Histogram; }

    void SetHistogramTitle(std::string hTitle) { Histogram2DTitles["HistogramTitle"] = hTitle; }

    void SetHistogramStatTitle(std::string sTitle) { Histogram2DTitles["HistogramStatTitle"] = sTitle; }

    void SetXaxisTitle(std::string xTitle) { Histogram2DTitles["XaxisTitle"] = xTitle; }

    void SetYaxisTitle(std::string yTitle) { Histogram2DTitles["YaxisTitle"] = yTitle; }

    void SetHistogramHeadTitles(std::string hTitle, std::string sTitle) {
        Histogram2DTitles["HistogramTitle"] = hTitle, Histogram2DTitles["HistogramStatTitle"] = sTitle;
    }

    void SetHistogramTitles(std::string hTitle, std::string sTitle, std::string xTitle, std::string yTitle) {
        Histogram2DTitles["HistogramTitle"] = hTitle;
        Histogram2DTitles["HistogramStatTitle"] = sTitle;
        Histogram2DTitles["XaxisTitle"] = xTitle;
        Histogram2DTitles["YaxisTitle"] = yTitle;
    }

    void SetHistogramTitles(std::string hTitle, std::string sTitle, std::string xTitle) {
        Histogram2DTitles["HistogramTitle"] = hTitle;
        Histogram2DTitles["HistogramStatTitle"] = sTitle;
        Histogram2DTitles["XaxisTitle"] = xTitle;
    }

    void SetAxisTitles(std::string xTitle, std::string yTitle) { Histogram2DTitles["XaxisTitle"] = xTitle, Histogram2DTitles["YaxisTitle"] = yTitle; }

    void SetHistogramNumberOfXBins(int NumberOfXBins) { HistogramNumberOfXBins = NumberOfXBins; }

    void SetUpperXlim(double uXlim) { HistogramXAxisLimits.at(1) = uXlim; }

    void SetLowerXlim(double lXlim) { HistogramXAxisLimits.at(0) = lXlim; }

    void SetUpperYlim(double uYlim) { HistogramYAxisLimits.at(1) = uYlim; }

    void SetLowerYlim(double lYlim) { HistogramYAxisLimits.at(0) = lYlim; }

    void SetXAxisLimits(double uXlim, double lXlim) { HistogramXAxisLimits.at(1) = uXlim, HistogramXAxisLimits.at(0) = lXlim; }

    void SetYAxisLimits(double uYlim, double lYlim) { HistogramYAxisLimits.at(1) = uYlim, HistogramYAxisLimits.at(0) = lYlim; }

    void SetAxisLimits(double uXlim, double lXlim, double uYlim, double lYlim) {
        HistogramXAxisLimits.at(1) = uXlim, HistogramXAxisLimits.at(0) = lXlim;
        HistogramYAxisLimits.at(1) = uYlim, HistogramYAxisLimits.at(0) = lYlim;
    }

//    void SetNormalizeHistogram(bool nHistogram) { NormalizeHistogram = nHistogram; }

//    void SetCustomNormalization(bool cNormalization) { CustomNormalization = cNormalization; }

//    void SetCustomNormalizationFactor(bool cnFactor) { CustomNormalizationFactor = cnFactor; }

    void SetHistogram2DTitleReactions(std::string h2DrReactions) { Histogram2DTitles["Histogram2DTitleReactions"] = h2DrReactions; }

    void SetTitleSize(double tSize) { Histogram2DTitleSizes.at(0) = tSize; }

    void SetLabelSizex(double lSizex) { Histogram2DTitleSizes.at(1) = lSizex; }

    void SetLabelSizey(double lSizey) { Histogram2DTitleSizes.at(2) = lSizey; }

    void SetLineWidth(int lWidth) { LineWidth = lWidth; }

    void SetZLogScalePlot(bool ZlsPlot) { ZLogScalePlot = ZlsPlot; }

    void SetZLinearScalePlot(bool lsPlot) { ZLinearScalePlot = lsPlot; }

    void SetHistogram2DSaveName(std::string h2DsName) { Histogram2DSaveName = h2DsName; }

    void SetHistogram2DSaveNamePath(std::string h2DsNamePath) { Histogram2DSaveNamePath = h2DsNamePath; }

    void SetFinalState(std::string fState) { Histogram2DTitles["FinalState"] = fState; }

    void SetDetectorRegion(std::string dRegion) { Histogram2DTitles["DetectorRegion"] = dRegion; }

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
    TH2D *GetHistogram2D() { return Histogram2D; } // used in AMaps class

    TH2D *GetHistogram() const { return Histogram2D; } // used in final state ratio (FSR) plots

    TH2D *GetHistogram2DConst() const { return Histogram2D; }

    std::string GetHistogramTitle() { return Histogram2DTitles["HistogramTitle"]; }

    std::string GetHistTitle() { return Histogram2D->GetTitle(); }

    std::string GetHistogramStatTitle() const { return HistogramStatsTitle; }

    std::string GetHistogramName() { return Histogram2D->GetName(); } // GetHistogramStatTitle (the ROOT version)

    std::string GetXaxisTitle() { return Histogram2DTitles["XaxisTitle"]; }

    std::string GetYaxisTitle() { return Histogram2DTitles["YaxisTitle"]; }

    int GetHistogramNumberOfXBins() { return HistogramNumberOfXBins; }

    int GetHistogramNumberOfYBins() { return HistogramNumberOfYBins; }

    double GetUpperXlim() const { return HistogramXAxisLimits.at(1); }

    double GetLowerXlim() const { return HistogramXAxisLimits.at(0); }

    double GetUpperYlim() const { return HistogramYAxisLimits.at(1); }

    double GetLowerYlim() const { return HistogramYAxisLimits.at(0); }

    double GetXBinCenter(int xBin) { return Histogram2D->GetXaxis()->GetBinCenter(xBin); }

    double GetYBinCenter(int yBin) { return Histogram2D->GetYaxis()->GetBinCenter(yBin); }

//    bool GetNormalizeHistogram() { return NormalizeHistogram; }

//    bool GetCustomNormalization() { return CustomNormalization; }

//    bool GetCustomNormalizationFactor() { return CustomNormalizationFactor; }

    std::string GetHistogram2DTitleReactions() { return Histogram2DTitles["Histogram2DTitleReactions"]; }

    double GetTitleSize() { return Histogram2DTitleSizes.at(0); }

    double GetLabelSizeX() { return Histogram2DTitleSizes.at(1); }

    double GetLabelSizeY() { return Histogram2DTitleSizes.at(2); }

    int GetLineWidth() { return LineWidth; }

    bool GetZLogScalePlot() { return ZLogScalePlot; }

    bool GetZLinearScalePlot() { return ZLinearScalePlot; }

    std::string GetHistogram2DSaveName() { return Histogram2DSaveName; }

    std::string GetHistogram2DSaveNamePath() const { return Histogram2DSaveNamePath; }

    std::string GetFinalState() { return Histogram2DTitles["FinalState"]; }

    std::string GetDetectorRegion() { return Histogram2DTitles["DetectorRegion"]; }

    int GetkColor() { return LineColor; }

    bool GetCenterTitle() { return CenterTitle; }

    bool GetAddToStack() { return AddToStack; }

    bool GetShowStats() { return ShowStats; }

    bool GetTitle2() { return Title2; }

    bool GetShowPlotCuts() { return ShowPlotCuts; }

    double GetPlotCuts() { return PlotCuts; }

    double GetPlotXmax() { return PlotXmax; }

    bool GetPlotHistogramMax() { return PlotHistogramMax; }
};

#endif //HPLOT2D_H
