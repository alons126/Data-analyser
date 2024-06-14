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

#if IndependentStackdraw
#include "../../source/classes/hData/hData.cpp"
#include "Histofinder1D.cpp"
#include "TitleAligner.cpp"
#endif

using namespace std;

#if IndependentStackdraw
const string ConfigRegion1D(const string &Histogram2DNameCopy) {
    if (findSubstring(Histogram2DNameCopy, "CD")) {
        return "CD";
    } else {
        return "FD";
    }
}

const string ConfigSName1D(const string &SampleName) {
    if (findSubstring(SampleName, "sim")) {
        return "s";
    } else if (findSubstring(SampleName, "data")) {
        return "d";
    }
}

double SetxOffset1D(const bool &ShowStats) {
    double xOffset;

    if (!ShowStats) {
        xOffset = -0.15;
    } else {
        xOffset = 0;
    }

    return xOffset;
}

double SetyOffset1D(const bool &ShowStats) {
    double yOffset;

    if (!ShowStats) {
        yOffset = 0.125;
    } else {
        yOffset = 0;
    }

    return yOffset;
}

void DrawPlot(TCanvas *HistogramCanvas, TH1D *Histogram1D, const bool LogScalePlot, const bool LinearScalePlot, const string &SavePath, const string &SaveName,
              const string &DrawRange) {
    if (DrawRange == "") {
        if (LogScalePlot) {
            HistogramCanvas->SetLogy(1);
            string SaveNameDir = SavePath + "/" + SaveName + "_log_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        if (LinearScalePlot) {
            HistogramCanvas->SetLogy(0);
            string SaveNameDir = SavePath + "/" + SaveName + "_linear_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }
    } else {
        if (LogScalePlot) {
            HistogramCanvas->SetLogy(1);
            string SaveNameDir = SavePath + "/" + SaveName + "_" + DrawRange + "_log_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        if (LinearScalePlot) {
            HistogramCanvas->SetLogy(0);
            string SaveNameDir = SavePath + "/" + SaveName + "_" + DrawRange + "_linear_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }
    }
}
#else

void DrawPlot(TCanvas *HistogramCanvas, const bool LogScalePlot, const bool LinearScalePlot, const string &SavePath, const string &SaveName,
              const string &DrawRange) {
    if (DrawRange == "") {
        if (LogScalePlot) {
            HistogramCanvas->SetLogy(1);
            string SaveNameDir = SavePath + "/" + SaveName + "_log_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        if (LinearScalePlot) {
            HistogramCanvas->SetLogy(0);
            string SaveNameDir = SavePath + "/" + SaveName + "_linear_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }
    } else {
        if (LogScalePlot) {
            HistogramCanvas->SetLogy(1);
            string SaveNameDir = SavePath + "/" + SaveName + "_" + DrawRange + "_log_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }

        if (LinearScalePlot) {
            HistogramCanvas->SetLogy(0);
            string SaveNameDir = SavePath + "/" + SaveName + "_" + DrawRange + "_linear_scale.png";
            const char *SaveDir = SaveNameDir.c_str();
            HistogramCanvas->SaveAs(SaveDir);
        }
    }
}

#endif

void HistPlotterStack(hData &particles, TCanvas *HistogramCanvas, TList *MScThesisPlotsList, const char *Sim_filename, const char *Data_filename, const char *Histogram1DName,
                      const string &SampleName, const string &SavePath, const string &SaveName, const bool TLmom = false) {
    cout << "\n\n";

    bool PosterModePlots = false, PosterModePlotsColorblind = false;

#if PosterMode
    PosterModePlots = true;
#endif

#if ColorblindMode
    PosterModePlotsColorblind = true;
#endif

    HistogramCanvas->Clear();

    TFile *Sim_file = TFile::Open(Sim_filename);
    TH1D *Sim_Histogram1D;

    TFile *Data_file = TFile::Open(Data_filename);
    TH1D *Data_Histogram1D;

    if (!findSubstring(Histogram1DName, "FSRatio")) {
        if (findSubstring(Histogram1DName, "W distribution") ||
            findSubstring(Histogram1DName, "#theta_{pFD,pCD}") || findSubstring(Histogram1DName, "#theta_{nFD,pCD}")) {
            Sim_Histogram1D = Histofinder1D(Sim_file, Histogram1DName);
            Data_Histogram1D = Histofinder1D(Data_file, Histogram1DName);
        } else {
            if (Sim_file->Get(Histogram1DName) == nullptr) {
                Sim_Histogram1D = Histofinder1D(Sim_file, Histogram1DName, TLmom);
            } else {
                Sim_Histogram1D = (TH1D *) Sim_file->Get(Histogram1DName);
            }

            if (Data_file->Get(Histogram1DName) == nullptr) {
                Data_Histogram1D = Histofinder1D(Data_file, Histogram1DName, TLmom);
            } else {
                Data_Histogram1D = (TH1D *) Data_file->Get(Histogram1DName);
            }
        }
    } else if (findSubstring(Histogram1DName, "deltaAlpha_T_tot FSRatio")) {
        string Histogram1DNameTemp0 = Histogram1DName, Histogram1DNameTemp = Histogram1DNameTemp0.substr(0, Histogram1DNameTemp0.find_last_of("(") + 1) + ")";
        Sim_Histogram1D = Histofinder1D(Sim_file, Histogram1DNameTemp.c_str(), TLmom);
        Data_Histogram1D = Histofinder1D(Data_file, Histogram1DName, TLmom);
    } else {
        Sim_Histogram1D = Histofinder1D(Sim_file, Histogram1DName, TLmom);
        Data_Histogram1D = Histofinder1D(Data_file, Histogram1DName, TLmom);
    }

    double Legend_x1_BaseLine = gStyle->GetStatX(), Legend_y1_BaseLine = gStyle->GetStatY(); // Top right
    double Legend_x2_BaseLine = gStyle->GetStatX(), Legend_y2_BaseLine = gStyle->GetStatY(); // Bottom left

    double Legend_x1_OneLine = Legend_x1_BaseLine, Legend_y1_OneLine = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_OneLine = Legend_x2_BaseLine - 0.2, Legend_y2_OneLine = Legend_y2_BaseLine - 0.25; // Bottom left

    double Legend_x1_TwoLines = Legend_x1_BaseLine, Legend_y1_TwoLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_TwoLines = Legend_x2_BaseLine - 0.2, Legend_y2_TwoLines = Legend_y2_BaseLine - 0.3; // Bottom left

    double Legend_x1_ThreeLines = Legend_x1_BaseLine, Legend_y1_ThreeLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_ThreeLines = Legend_x2_BaseLine - 0.2, Legend_y2_ThreeLines = Legend_y2_BaseLine - 0.35; // Bottom left

    /* Histogram normalization setup */
    bool NormalizeHistogram;
    bool CustomNormalization;
    double CustomNormalizationFactor;

    /* Histogram appearance setup */
    const string Histogram1DNameCopy = Histogram1DName;
    int LineWidth;
    if (!PosterModePlots) {
        LineWidth = 6;
    } else {
        LineWidth = 12;
    }
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

    HistogramCanvas->cd();

    const double beamE = 5.98636;
    const string Region = ConfigRegion1D(Histogram1DNameCopy);
    string sNameFlag = ConfigSName1D(SampleName);

    double Histogram1D_integral; // To be calculated only if normalize_Histogram
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Sim_Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Sim_Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D->GetXaxis()->CenterTitle(true);
    Sim_Histogram1D->GetYaxis()->SetTitle("Number of events");
    Sim_Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Sim_Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D->GetYaxis()->CenterTitle(true);
    Sim_Histogram1D->SetLineWidth(LineWidth);
    Sim_Histogram1D->SetLineStyle(0); // Original
    if (PosterModePlots) {
        Sim_Histogram1D->SetMarkerStyle(22);
        Sim_Histogram1D->SetMarkerSize(6.5);
    }
    if (!PosterModePlots && !PosterModePlotsColorblind) {
        Sim_Histogram1D->SetLineColor(kBlue); // Original
        Sim_Histogram1D->SetMarkerColor(kBlue);
    } else if (PosterModePlots && !PosterModePlotsColorblind) {
        Sim_Histogram1D->SetLineColor(kBlue); // Original
        Sim_Histogram1D->SetMarkerColor(kBlue);
    } else if (PosterModePlots && PosterModePlotsColorblind) {
        int colorIndex_Sim = TColor::GetFreeColorIndex();  // Get a free color index
        TColor *color_Sim = new TColor(colorIndex_Sim, 0, 0.45, 0.7);  // RGB (Blue)
        Sim_Histogram1D->SetLineColor(colorIndex_Sim);
        Sim_Histogram1D->SetMarkerColor(colorIndex_Sim);
    }
    Sim_Histogram1D->SetStats(0);
    MScThesisPlotsList->Add(Sim_Histogram1D);
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
        Sim_Histogram1D->Scale(Data_Histogram1D->Integral() / Sim_Histogram1D->Integral());
        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 1.1 * max(Data_Histogram1D->GetMaximum(), Sim_Histogram1D->GetMaximum()));
    }

    Data_Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Data_Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Data_Histogram1D->GetXaxis()->CenterTitle(true);
    Data_Histogram1D->GetYaxis()->SetTitle("Number of events");
    Data_Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Data_Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Data_Histogram1D->GetYaxis()->CenterTitle(true);
    if (!PosterModePlots) {
        Data_Histogram1D->SetLineWidth(LineWidth + 2);
    } else {
        Data_Histogram1D->SetLineWidth(LineWidth + 4);
    }
    Data_Histogram1D->SetLineStyle(0); // Original
    Data_Histogram1D->SetMarkerStyle(8);
    if (!PosterModePlots) {
        Data_Histogram1D->SetMarkerSize(2.5);
    } else {
        Data_Histogram1D->SetMarkerSize(6.5);
    }
    if (!PosterModePlots && !PosterModePlotsColorblind) {
        Data_Histogram1D->SetLineColor(kRed + 1);
        Data_Histogram1D->SetMarkerColor(kRed + 1);
    } else if (PosterModePlots) {
        Data_Histogram1D->SetLineColor(kBlack);
        Data_Histogram1D->SetMarkerColor(kBlack);
    }
    Data_Histogram1D->SetStats(0);
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 1.1 * max(Data_Histogram1D->GetMaximum(), Sim_Histogram1D->GetMaximum()));
    }
    MScThesisPlotsList->Add(Data_Histogram1D);

    if (Sim_Histogram1D->Integral() == 0. || Data_Histogram1D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Sim_Histogram1D->Draw();
        displayText->Draw();
    } else if (Sim_Histogram1D->Integral() != 0. && Data_Histogram1D->Integral() != 0.) {
        string Histogram1D_Title = Sim_Histogram1D->GetTitle();
        string Histogram1D_xLabel = Sim_Histogram1D->GetXaxis()->GetTitle(), Histogram1D_yLabel = Sim_Histogram1D->GetYaxis()->GetTitle();
        string FSRyLabel;

        if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
            cout << "\n\n\nTitle = " << Histogram1D_Title << "\n";
            cout << "Sim peak = " << Sim_Histogram1D->GetBinCenter(Sim_Histogram1D->GetMaximumBin()) << "\n";
            cout << "Data peak = " << Data_Histogram1D->GetBinCenter(Data_Histogram1D->GetMaximumBin()) << "\n";
            cout << "========================================================================================\n\n";
        }

        string RatioVar = Histogram1D_xLabel.substr(0, Histogram1D_xLabel.find_last_of('[') - 1);
        string RatioTopology;

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
//            cout << "Histogram1D_Title1 = " << Histogram1D_Title << "\n";

            if (findSubstring(Histogram1D_Title, "1n/1p") || findSubstring(Histogram1D_Title, "1nFD/1pFD")) {
                RatioTopology = "1N";
            } else if (findSubstring(Histogram1D_Title, "nFDpCD/pFDpCD") || findSubstring(Histogram1D_Title, "1nFD1pCD/1pFD1pCD")) {
                RatioTopology = "2N";
            }

#if PresentationMode
            if (RatioTopology == "1N") {
                FSRyLabel = "r^{" + RatioVar + "}_{" + RatioTopology + "} = #frac{1n}{1p}";
            } else if (RatioTopology == "2N") {
                FSRyLabel = "r_{" + RatioVar + "} = #frac{1n1p}{2p}";
            }
#else
            if (RatioTopology == "1N") {
                FSRyLabel = "r^{" + RatioVar + "}_{" + RatioTopology + "} = #frac{1nFD}{1pFD}";
            } else if (RatioTopology == "2N") {
                FSRyLabel = "r_{" + RatioVar + "} = #frac{1nFD1pCD}{1pFD1pCD}";
            }
#endif

        } else {
            if (findSubstring(Histogram1D_Title, "1n") || findSubstring(Histogram1D_Title, "1p") ||
                findSubstring(Histogram1D_Title, "1nFD") || findSubstring(Histogram1D_Title, "1pFD")) {
                RatioTopology = "1N";
            } else if (findSubstring(Histogram1D_Title, "nFDpCD/pFDpCD") || findSubstring(Histogram1D_Title, "1nFD1pCD/1pFD1pCD")) {
                RatioTopology = "2N";
            }
        }

//        cout << "Histogram1D_Title = " << Histogram1D_Title << "\n";
//        cout << "RatioTopology = " << RatioTopology << "\n";
//        cout << "FSRyLabel = " << FSRyLabel << "\n";

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
            string StringTemp0 = Sim_Histogram1D->GetTitle();
            string StringTemp = StringTemp0.substr(StringTemp0.find_last_of("ratio") + 1);
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, StringTemp, "");

            if (RatioTopology == "1N") { TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "Leading nucleon", "Nucleon"); }
        }

//        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "ratio- 1nFD/1pFD", "ratio");
//        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "ratio - 1n/1p", "ratio");
//        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "ratio - 1nFD/1pFD", "ratio");
//        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "ratio- 1nFD1pCD/1pFD1pCD", "ratio");
//        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "ratio - 1nFD1pCD/1pFD1pCD", "ratio");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1nFD, FD)", " in 1nFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "1p", "1pFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "1n", "1nFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "pFDpCD", "1pFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "nFDpCD", "1nFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (All Int., 1pFD1pCD)", " in 1pFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (All Int., 1nFD1pCD)", " in 1nFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (All Int., 1pFD1pCD, FD)", " in 1pFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (All Int., 1nFD1pCD, FD)", " in 1nFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1pFD, FD)", " in 1pFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1nFD, FD)", " in 1nFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1pFD0pCD, FD)", " in 1pFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1nFD0pCD, FD)", " in 1nFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1pFD1pCD, FD)", " in 1pFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " (1nFD1pCD, FD)", " in 1nFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "(1pFD0pCD)", "in 1pFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "(1nFD0pCD)", "in 1nFD0pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "(1pFD1pCD)", "in 1pFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "(1nFD1pCD)", "in 1nFD1pCD");

        if (RatioTopology == "1N") {
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD proton", "Proton");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "Leading FD neutron", "Neutron");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD nucleon", "Nucleon");

#if PresentationMode
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{p} [GeV/c]", "P_{pFD} [GeV/c]");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{n} [GeV/c]", "P_{nFD} [GeV/c]");
#else
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{p} [GeV/c]", "P_{pFD} [GeV]");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{n} [GeV/c]", "P_{nFD} [GeV]");
#endif

        }

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
            Sim_Histogram1D->GetYaxis()->SetTitle(FSRyLabel.c_str());
            Data_Histogram1D->GetYaxis()->SetTitle(FSRyLabel.c_str());
        }

#if PresentationMode
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "1pFD1pCD", "2p");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "1nFD1pCD", "1n1p");
#endif

        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD neutron", "Forward-going neutron");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD Neutron", "Forward-going neutron");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD Proton", "Forward-going proton");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD proton", "Forward-going proton");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "CD Proton", "Central-going proton");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "CD proton", "Central-going proton");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "FD nucleon", "Forward-going nucleon");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "of Forward-going", "of forward-going");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "of Central-going", "of central-going");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " P^{truth}_{n}", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " P^{truth}_{p}", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " P_{nFD}", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " P_{pFD}", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " P_{pCD}", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " APID&NC", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " APID&PS", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{pL} [GeV/c]", "P_{L} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{nL} [GeV/c]", "P_{L} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{nucL}", "P_{L}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{pR} [GeV/c]", "P_{R} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{nR} [GeV/c]", "P_{R} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{nucR}", "P_{R}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}|", "P_{tot}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}|", "P_{tot}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2", "P_{rel}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2", "P_{rel}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "3-momentum", "momentum");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " Histogram", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " distribution", "");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "E_{cal} = E_{e} + T_{nFD} + T_{pCD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "E_{cal} = E_{e} + T_{pFD} + T_{pCD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "E_{cal} = E_{e} + T_{nucFD} + T_{nucCD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "E_{cal} Reconstruction", "Reconstructed energy");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nucL} + #vec{P}_{nucR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#vec{P}_{tot}-#vec{q}", "Missing momentum");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "Missing momentum [GeV/c]", "P_{miss} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}} ", "W");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}} ", "W");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#vec{P}", "#font[62]{P}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#vec{q}", "#font[62]{q}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#vec{q}", "#font[62]{q}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "W [GeV]", "W [GeV/c^{2}]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "W distribution", "Hadronic mass");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "W ratio", "Hadronic mass ratio");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "P_{nucCD} [GeV/c]", "P_{pCD} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{nucFD,nucCD} [Deg]", "#theta_{nucFD,pCD} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{#font[62]{q},#font[62]{P}_{nuc,FD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{nucFD}} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{#font[62]{q},#font[62]{P}_{nuc,FD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{nucFD}} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{#font[62]{q},#font[62]{P}_{nuc,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{#font[62]{q},#font[62]{P}_{p,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{#font[62]{q},#font[62]{P}_{p,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#font[62]{P}_{tot}| = |#font[62]{P}_{nucL} + #font[62]{P}_{nucR}|", "P_{tot}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "|#font[62]{P}_{rel}| = |#font[62]{P}_{nucL} - #font[62]{P}_{nucR}|/2", "P_{rel}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{e} of Outgoing Electron", "Electron scattering angle");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{nFD} of FD neutron", "Scattering angle of forward-going neutron");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{pFD} of FD proton", "Scattering angle of forward-going proton");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta_{pCD} of CD proton", "Scattering angle of central-going proton");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "[Deg]", "[#circ]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#deltaP_{T,tot} by Momentum Sum", "Transverse momentum imbalance");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#delta#alpha_{T,tot} by Momentum Sum", "Transverse boosting angle");

#if PresentationMode
#else
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "[GeV/c]", "[GeV]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "[GeV/c^{2}]", "[GeV]");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "[GeV^{2}/c^{2}]", "[GeV^{2}]");
#endif

        if ((findSubstring(Sim_Histogram1D->GetTitle(), "Electron") || findSubstring(Sim_Histogram1D->GetTitle(), "electron")) &&
            findSubstring(Sim_Histogram1D->GetTitle(), "#theta")) {
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta [Deg]", "#theta_{e} [#circ]");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta [#circ]", "#theta_{e} [#circ]");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "Electron #theta", "Electron scattering angle");
        } else if (findSubstring(Sim_Histogram1D->GetTitle(), "Forward-going nucleon") && findSubstring(Sim_Histogram1D->GetTitle(), "#theta")) {
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta", "#theta_{nucFD}");
        } else if (findSubstring(Sim_Histogram1D->GetTitle(), "Central-going proton") &&
                   !findSubstring(Sim_Histogram1D->GetTitle(), "#theta_{pCD}") &&
                   !findSubstring(Histogram1DNameCopy, "#theta_{#vec{q},#vec{P}_{pCD}}") &&
                   findSubstring(Sim_Histogram1D->GetTitle(), "#theta")) {
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "#theta", "#theta_{pCD}");
        }

        if (findSubstring(Sim_Histogram1D->GetTitle(), "Opening Angle") && !findSubstring(Histogram1DNameCopy, "FSRatio")) {
            string StringTemp00 = Sim_Histogram1D->GetTitle();
            string OpeningAngle = StringTemp00.substr(0, StringTemp00.find("} -") + 2);
            string Title = StringTemp00.substr(0, StringTemp00.find_last_of(" in") - 4);
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, Title, OpeningAngle);
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " } ", " ");
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " s ", " ");
            /*
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, Title, OpeningAngle);
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, " } ", " ");
*/
        } else if (findSubstring(Sim_Histogram1D->GetTitle(), "Opening angle") && findSubstring(Histogram1DNameCopy, "FSRatio")) {
            string TitleTemp = Sim_Histogram1D->GetTitle();
            string OpeningAngleTemp = Sim_Histogram1D->GetXaxis()->GetTitle();
            string OpeningAngle = OpeningAngleTemp.substr(0, OpeningAngleTemp.find(" ["));
            string Title = TitleTemp.substr(0, TitleTemp.find_last_of("ratio") - 5);

//            cout << "OpeningAngle = " << OpeningAngle << "\n";
//            cout << "Title = " << Title << "\n";
//            exit(0);

            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, Title, OpeningAngle);

//            Sim_Histogram1D->SetTitle((OpeningAngle + " histogram ratio").c_str());
//            Data_Histogram1D->SetTitle((OpeningAngle + " histogram ratio").c_str());
        }

        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "ratio", "- histogram ratio");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "nucCD", "pCD");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "{nuc,FD}", "{nucFD}");
        TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "{p,CD}", "{pCD}");

        if (findSubstring(Histogram1DNameCopy, "FSRatio") && PosterModePlots) {
//        if (findSubstring(Sim_Histogram1D->GetTitle(), "Central-going proton") && findSubstring(Histogram1DNameCopy, "FSRatio") && PosterModePlots) {
            double yLabelOffset = 1.3;
//            double yLabelOffset = 1.2;
            Sim_Histogram1D->GetYaxis()->SetTitleOffset(yLabelOffset);
            Data_Histogram1D->GetYaxis()->SetTitleOffset(yLabelOffset);
        }

        if (findSubstring(Sim_Histogram1D->GetYaxis()->GetTitle(), "r_{#theta}") && findSubstring(Sim_Histogram1D->GetTitle(), "Electron")) {
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, "r_{#theta}", "r_{#theta_{e}}");
        }

        if (PosterModePlots && findSubstring(Histogram1DNameCopy, "FSRatio")) {
            int CustomColor1n1p_index = 9997;
            TColor *CustomColor1n1p;

            int CustomColor2p_index = 9998;
            TColor *CustomColor2p;

            int CustomColorRatio_index = 9999;
            TColor *CustomColorRatio;

            if (PosterModePlotsColorblind) {
                CustomColor1n1p = new TColor(CustomColor1n1p_index, 0, 0.45, 0.7); // Color index 1000, with RGB (0.5, 0.2, 0.7)
                CustomColor2p = new TColor(CustomColor2p_index, 0.9, 0.6, 0); // Color index 1000, with RGB (0.5, 0.2, 0.7)
                CustomColorRatio = new TColor(CustomColorRatio_index, 0, 0.6, 0.5); // Color index 1000, with RGB (0.5, 0.2, 0.7)
            } else {
                CustomColor1n1p = new TColor(CustomColor1n1p_index, 0, 0.45, 0.7); // Color index 1000, with RGB (0.5, 0.2, 0.7)
                CustomColor2p = new TColor(CustomColor2p_index, 0.9, 0.6, 0); // Color index 1000, with RGB (0.5, 0.2, 0.7)
                CustomColorRatio = new TColor(CustomColorRatio_index, 0, 0.6, 0.5); // Color index 1000, with RGB (0.5, 0.2, 0.7)
            }

            string Poster_y_label_temp = Sim_Histogram1D->GetYaxis()->GetTitle();
            string Poster_y_label_temp_ratio_indicator = Poster_y_label_temp.substr(0, Poster_y_label_temp.find_last_of(" =") - 2);

            string Poster_y_label = "#font[62]{#color[" + to_string(CustomColorRatio_index) + "]{" + Poster_y_label_temp_ratio_indicator + "}} = " +
                                    "#font[62]{#frac{#color[" + to_string(CustomColor1n1p_index) + "]{1n1p}}{#color[" + to_string(CustomColor2p_index) + "]{2p}}}";
            TitleAligner(particles, Sim_Histogram1D, Data_Histogram1D, (Poster_y_label_temp_ratio_indicator + " = " + "#frac{1n1p}{2p}").c_str(), Poster_y_label);
        }

        Sim_Histogram1D->Draw(), gPad->Update();
        Data_Histogram1D->Draw("same"), gPad->Update();
        Sim_Histogram1D->Draw("same"), gPad->Update();

        double xOffset = SetxOffset1D(false), yOffset = SetyOffset1D(false);

        TLegend *Comparison_legend;

        double Custom_x1Offset = 0;

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) { Custom_x1Offset = -0.085; }

        if (findSubstring(Histogram1D_Title, "W ") ||
            findSubstring(Histogram1D_Title, "#theta_{nFD}") || findSubstring(Histogram1D_Title, "#theta_{pFD}") ||
            findSubstring(Histogram1D_Title, "E_{cal}")) {
            double Custom_xOffset = -0.41;

            if (!PosterModePlots) {
                Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_xOffset + Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                                Legend_x2_TwoLines - 0.05 + xOffset + Custom_xOffset, Legend_y2_TwoLines + yOffset);
            } else {
                if (findSubstring(Histogram1D_Title, "W ") && findSubstring(Histogram1DNameCopy, "FSRatio")) {
                    double Custom_xOffset_poster = 0.21;
                    double Custom_yOffset_poster = 0.01;

                    Comparison_legend = new TLegend(Legend_x1_TwoLines + Custom_xOffset + Custom_xOffset_poster,
                                                    Legend_y1_TwoLines + yOffset + Custom_yOffset_poster,
                                                    Legend_x2_TwoLines - 0.05 + Custom_xOffset_poster,
                                                    Legend_y2_TwoLines + yOffset + Custom_yOffset_poster);
//                    Comparison_legend = new TLegend(Legend_x1_TwoLines + Custom_xOffset + Custom_x1Offset + Custom_xOffset_poster, // Left corner
//                                                    Legend_y1_TwoLines + yOffset + Custom_yOffset_poster,
//                                                    Legend_x2_TwoLines - 0.05 + Custom_xOffset + Custom_xOffset_poster,
//                                                    Legend_y2_TwoLines + yOffset + Custom_yOffset_poster);
                } else {
                    Comparison_legend = new TLegend(Legend_x1_TwoLines + Custom_xOffset + Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                                    Legend_x2_TwoLines - 0.05 + Custom_xOffset, Legend_y2_TwoLines + yOffset);
                }
            }
        } else if (findSubstring(Histogram1D_Title, "#delta#alpha_{T,tot}") && !findSubstring(Histogram1DNameCopy, "FSRatio")) {
            double Custom_xOffset = -0.41;

            if (!PosterModePlots) {
                Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_xOffset + Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                                Legend_x2_TwoLines - 0.05 + xOffset + Custom_xOffset, Legend_y2_TwoLines + yOffset);
            } else {
                Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_xOffset + Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                                Legend_x2_TwoLines - 0.05 + xOffset + Custom_xOffset, Legend_y2_TwoLines + yOffset);
            }
        } else {
            if (!PosterModePlots) {
                Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_x1Offset - Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                                Legend_x2_TwoLines - 0.05 + xOffset - Custom_x1Offset, Legend_y2_TwoLines + yOffset);
            } else {
                if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
                    Comparison_legend = new TLegend(Legend_x1_TwoLines + Custom_x1Offset - Custom_x1Offset - 0.04, Legend_y1_TwoLines + yOffset + 0.01,
                                                    Legend_x2_TwoLines - 0.05 - Custom_x1Offset - 0.06, Legend_y2_TwoLines + yOffset + 0.01);
                } else {
                    Comparison_legend = new TLegend(Legend_x1_TwoLines + Custom_x1Offset - Custom_x1Offset - 0.04, Legend_y1_TwoLines + yOffset + 0.01,
                                                    Legend_x2_TwoLines - 0.05 - Custom_x1Offset - 0.04, Legend_y2_TwoLines + yOffset + 0.01);
                }
            }
        }

        if (!PosterModePlots) {
            if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
                TLegendEntry *Sim_Entry = Comparison_legend->AddEntry(Sim_Histogram1D, "Simulation (scaled)", "l");
                TLegendEntry *Data_Entry = Comparison_legend->AddEntry(Data_Histogram1D, "Data");
            } else {
                TLegendEntry *Sim_Entry = Comparison_legend->AddEntry(Sim_Histogram1D, "Simulation", "l");
                TLegendEntry *Data_Entry = Comparison_legend->AddEntry(Data_Histogram1D, "Data");
            }
        } else {
            if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
                TLegendEntry *Sim_Entry = Comparison_legend->AddEntry(Sim_Histogram1D, "Simulation (scaled)");
                TLegendEntry *Data_Entry = Comparison_legend->AddEntry(Data_Histogram1D, "Data");
            } else {
                TLegendEntry *Sim_Entry = Comparison_legend->AddEntry(Sim_Histogram1D, "Simulation");
                TLegendEntry *Data_Entry = Comparison_legend->AddEntry(Data_Histogram1D, "Data");
            }
        }

        Comparison_legend->SetTextSize(0.03), Comparison_legend->SetTextAlign(12), Comparison_legend->Draw("same");

        /*
        if (PosterModePlots) {
            if (findSubstring(Sim_Histogram1D->GetTitle(), "Central-going proton momentum")) {
                if (findSubstring(Sim_Histogram1D->GetTitle(), "1n1p")) {
                    string Sim_Histogram1D_CloneName = Sim_Histogram1D->GetName();
                    TH1D *Sim_Histogram1D_ZoomClone = (TH1D *) Sim_Histogram1D->Clone((Sim_Histogram1D_CloneName + "_zoomin").c_str());
                    string Sim_Histogram1D_CloneTitle = Sim_Histogram1D_ZoomClone->GetTitle();
                    Sim_Histogram1D_ZoomClone->SetTitle((Sim_Histogram1D_CloneTitle + " (zoom-in)").c_str());
                    Sim_Histogram1D_ZoomClone->SetLineWidth(3);

                    string Data_Histogram1D_CloneName = Data_Histogram1D->GetName();
                    TH1D *Data_Histogram1D_ZoomClone = (TH1D *) Data_Histogram1D->Clone((Data_Histogram1D_CloneName + "_zoomin").c_str());
                    string Data_Histogram1D_CloneTitle = Data_Histogram1D_ZoomClone->GetTitle();
                    Data_Histogram1D_ZoomClone->SetTitle((Data_Histogram1D_CloneTitle + " (zoom-in)").c_str());
                    Data_Histogram1D_ZoomClone->SetLineWidth(3);
                    Data_Histogram1D_ZoomClone->SetMarkerSize(1.5);

                    // Define the region to zoom in
                    Double_t x1 = 1.0;  // x-axis lower bound
                    Double_t x2 = 3.0;  // x-axis upper bound
                    Double_t y1 = 0.0;  // y-axis lower bound
                    Double_t y2 = 100.0; // y-axis upper bound

                    // Coordinates of the pad in the main canvas
                    double y_max = 1.1 * max(Data_Histogram1D->GetMaximum(), Sim_Histogram1D->GetMaximum());
                    double x_max = 3.0;

                    Double_t padX1 = (Comparison_legend->GetX1() - 0.125 + 0.01) * x_max;
//                    Double_t padX1 = (Comparison_legend->GetX1() - 0.10) * 1.1 * max(Data_Histogram1D->GetMaximum(), Sim_Histogram1D->GetMaximum());
                    Double_t padY1 = (Comparison_legend->GetY1() - 0.35 - 0.15 + 0.045) * y_max;
                    Double_t padX2 = (Comparison_legend->GetX2() + 0.15 + 0.01) * x_max;
//                    Double_t padX2 = (Comparison_legend->GetX2() - 0.00) * 1.1 * max(Data_Histogram1D->GetMaximum(), Sim_Histogram1D->GetMaximum());
                    Double_t padY2 = (Comparison_legend->GetY2() - 0.15 - 0.10 + 0.025) * y_max;

                    // Draw a box around the TPad to simulate a frame
                    TBox *box = new TBox(padX1, padY1, padX2, padY2);
                    box->SetLineColor(kRed);  // Set box color to red
                    box->SetLineWidth(2);     // Set box line width
                    box->SetFillStyle(0);     // No fill
                    box->Draw();

                    // Create a new pad for the zoomed-in area
                    TPad *pad = new TPad("pad", "Zoomed-In Region",
                                         Comparison_legend->GetX1() - 0.1, Comparison_legend->GetY1() - 0.35,
                                         Comparison_legend->GetX2(), Comparison_legend->GetY2() - 0.15);
//                    TPad *pad = new TPad("pad", "Zoomed-In Region",
//                                         Comparison_legend->GetX1() - 0.1, Comparison_legend->GetX2() - 0.1,
//                                         Comparison_legend->GetY1() - 0.1, Comparison_legend->GetY2() - 0.1);
//                    pad->SetFillColor(0);  // Set pad color to white
                    pad->Draw("same");
                    pad->cd();
                    pad->cd()->SetGrid();
                    pad->cd()->SetBottomMargin(0.14), pad->cd()->SetLeftMargin(0.16), pad->cd()->SetTopMargin(0.12);

                    double yMax = max(Sim_Histogram1D_ZoomClone->GetMaximum(), Data_Histogram1D_ZoomClone->GetMaximum());

                    // Draw the zoomed-in histogram
                    Sim_Histogram1D_ZoomClone->GetXaxis()->SetRangeUser(x1, x2);
                    Sim_Histogram1D_ZoomClone->GetYaxis()->SetRangeUser(0., yMax * 0.1);
                    Data_Histogram1D_ZoomClone->GetXaxis()->SetRangeUser(x1, x2);
                    Data_Histogram1D_ZoomClone->GetYaxis()->SetRangeUser(0., yMax * 0.1);

                    Sim_Histogram1D_ZoomClone->Draw(), gPad->Update();;
                    Data_Histogram1D_ZoomClone->Draw("same"), gPad->Update();;
                    Sim_Histogram1D_ZoomClone->Draw("same"), gPad->Update();;

                    // Return to the main canvas
                    HistogramCanvas->cd();
                }
            }
        }
*/
    }

    if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 1.2);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 1.2);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range1");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 2);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 2);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range2");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 3);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 3);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range3");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 4);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 4);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range4");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 5);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 5);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range5");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 7);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 7);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range7");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 10);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 10);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range10");

        Sim_Histogram1D->GetYaxis()->SetRangeUser(0., 20);
        Data_Histogram1D->GetYaxis()->SetRangeUser(0., 20);
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range20");
    } else {
        DrawPlot(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "");
    }

    HistogramCanvas->Clear();
    delete Sim_file;
    delete Data_file;
}
