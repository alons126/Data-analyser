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

#include "source/functions/GeneralFunctions.h"
#include "source/constants.h"
#include "Histofinder1D.cpp"

using namespace std;

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
        xOffset = -0.11;
    } else {
        xOffset = 0;
    }

    return xOffset;
}

double SetyOffset1D(const bool &ShowStats) {
    double yOffset;

    if (!ShowStats) {
        yOffset = 0.15;
    } else {
        yOffset = 0;
    }

    return yOffset;
}

void HistPlotter1D(TCanvas *HistogramCanvas, TList *MScThesisPlotsList, const char *filename, const char *Histogram1DName,
                   const string &SampleName, const string &SavePath, const string &SaveName) {
    cout << "\n\n";

    HistogramCanvas->Clear();

    TFile *file = new TFile(filename);
    TH1D *Histogram1D;

    if (file->Get(Histogram1DName) == nullptr) {
        Histogram1D = Histofinder1D(filename, Histogram1DName);
    } else {
        Histogram1D = (TH1D *) file->Get(Histogram1DName);
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
    int LineColor = 1;
    int LineWidth = 2;
    vector<double> Histogram1DTitleSizes = {0.06, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey}
    bool CenterTitle = true;
    bool ShowStats = true;

    /* Histogram stack stuff */
//    THStack *Histogram1DStack;
    bool AddToStack = false;

    /* Histogram cuts setup */
    bool ApplyPlotCuts = true;
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

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitle("Arbitrary units (#events)");
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(LineWidth);
    MScThesisPlotsList->Add(Histogram1D);

    if (Histogram1D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram1D->Sumw2();
        Histogram1D->Draw();
        displayText->Draw();
    } else if (Histogram1D->Integral() != 0.) {

        if (findSubstring(Histogram1DNameCopy, "V_{z}^{")) {
            LogScalePlot = true;

            double UpperCut = 3., LowerCut = -8.;

            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *UpperVzCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperVzCut->SetLineWidth(2);
            UpperVzCut->SetLineColor(kBlue);
            UpperVzCut->Draw("same");

            TLine *LowerVzCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerVzCut->SetLineWidth(2);
            LowerVzCut->SetLineColor(kRed);
            LowerVzCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines, Legend_y1_TwoLines, Legend_x2_TwoLines, Legend_y2_TwoLines);
            TLegendEntry *UpperVzCutEntry = Legend->AddEntry(UpperVzCut, ("Upper cut = " + to_string_with_precision(UpperCut, 0) + " [cm]").c_str(), "l");
            TLegendEntry *LowerVzCutEntry = Legend->AddEntry(LowerVzCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0) + " [cm]").c_str(), "l");
            Legend->Draw("same");
        }

        if (findSubstring(Histogram1DNameCopy, "Vertex corr. dV^{")) {
            LogScalePlot = true;

            double UpperCut = 4., LowerCut = -5.;

            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *UpperVzCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperVzCut->SetLineWidth(2);
            UpperVzCut->SetLineColor(kBlue);
            UpperVzCut->Draw("same");

            TLine *LowerVzCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerVzCut->SetLineWidth(2);
            LowerVzCut->SetLineColor(kRed);
            LowerVzCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines, Legend_y1_TwoLines, Legend_x2_TwoLines, Legend_y2_TwoLines);
            TLegendEntry *UpperVzCutEntry = Legend->AddEntry(UpperVzCut, ("Upper cut = " + to_string_with_precision(UpperCut, 0) + " [cm]").c_str(), "l");
            TLegendEntry *LowerVzCutEntry = Legend->AddEntry(LowerVzCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0) + " [cm]").c_str(), "l");
            Legend->Draw("same");
        }

        if (Histogram1DNameCopy == "N_{phe} in HTCC BC (1e cut)" || Histogram1DNameCopy == "N_{phe} in HTCC (1e cut)") {
            double LowerCut = 2.;

            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *LowerNpheCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerNpheCut->SetLineWidth(2);
            LowerNpheCut->SetLineColor(kRed);
            LowerNpheCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine, Legend_y1_OneLine, Legend_x2_OneLine, Legend_y2_OneLine);
            TLegendEntry *LowerNpheCutEntry = Legend->AddEntry(LowerNpheCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0)).c_str(), "l");
            Legend->Draw("same");
        }

        if ((Histogram1DNameCopy == "FD Proton momentum #epsilon_{eff} (1e cut)") ||
            (Histogram1DNameCopy == "CD Proton momentum #epsilon_{eff} (1e cut)")) {
            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            double LowerMomentumTh = 0.4;

            Histogram1D->GetYaxis()->SetRangeUser(0., 0.2);
            Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, 0.2);
            LowerMomTh->SetLineWidth(2);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.2 + xOffset, Legend_y2_OneLine + yOffset);

            TLegendEntry *LowerMomThEntry;

            if (Histogram1DNameCopy == "FD Proton momentum #epsilon_{eff} (1e cut)") {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{pFD} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " GeV/c").c_str(), "l");
                Histogram1D->GetXaxis()->SetTitle("P_{pFD} [GeV/c]");
            } else {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{pCD} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " GeV/c").c_str(), "l");
                Histogram1D->GetXaxis()->SetTitle("P_{pCD} [GeV/c]");
            }

            Legend->Draw("same");
        }

        if (Histogram1DNameCopy == "#theta_{p_{1},p_{2}} (All Int., 2p)") {
            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();
            Histogram1D->SetLineWidth(LineWidth);
            Histogram1D->SetLineColor(kBlue);
            Histogram1D->SetLineStyle(1);
        }

        if (findSubstring(Histogram1DNameCopy, "#delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN")) {
            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            double PeakLocation = -2.4, Cut = 15.;
            double UpperCut = Cut + PeakLocation, LowerCut = -Cut + PeakLocation;

            Histogram1D->SetTitle("#Delta#phi for small #Delta#theta_{pFD/pCD}");
            Histogram1D->GetXaxis()->SetTitle("#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg]");
            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *PeakLoc = new TLine(PeakLocation, gPad->GetUymin(), PeakLocation, gPad->GetUymax());
            PeakLoc->SetLineWidth(2);
            PeakLoc->SetLineColor(kGreen);
            PeakLoc->Draw("same");

            TLine *UpperdPhiCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperdPhiCut->SetLineWidth(2);
            UpperdPhiCut->SetLineColor(kBlue);
            UpperdPhiCut->Draw("same");

            TLine *LowerdPhiCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerdPhiCut->SetLineWidth(2);
            LowerdPhiCut->SetLineColor(kRed);
            LowerdPhiCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_ThreeLines + xOffset, Legend_y1_ThreeLines + yOffset, Legend_x2_ThreeLines - 0.1 + xOffset, Legend_y2_ThreeLines + yOffset);
            TLegendEntry *PeakLocEntry = Legend->AddEntry(PeakLoc, ("Peak location = " + to_string_with_precision(PeakLocation, 1)).c_str(), "l");
            TLegendEntry *UpperdPhiCutEntry = Legend->AddEntry(UpperdPhiCut, ("Upper cut = " + to_string_with_precision(UpperCut, 1)).c_str(), "l");
            TLegendEntry *LowerdPhiCutEntry = Legend->AddEntry(LowerdPhiCut, ("Lower cut = " + to_string_with_precision(LowerCut, 1)).c_str(), "l");
            Legend->Draw("same");
        }

        if ((Histogram1DNameCopy == "FD Leading neutron momentum #epsilon_{eff} (1e cut)") ||
            (Histogram1DNameCopy == "FD Neutron momentum #epsilon_{eff} (1e cut)")) {
            double UpperCut = beamE, LowerCut = 0.4;

            Histogram1D->GetYaxis()->SetRangeUser(0., 1.);
            Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, 1.);
            UpperMomTh->SetLineWidth(2);
            UpperMomTh->SetLineColor(kBlue);
            UpperMomTh->Draw("same");

            TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, 1.);
            LowerMomTh->SetLineWidth(2);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines, Legend_y1_TwoLines, Legend_x2_TwoLines, Legend_y2_TwoLines);
            TLegendEntry *UpperMomThEntry = Legend->AddEntry(UpperMomTh, "Upper P_{n} th. = E_{beam}/c", "l");
            TLegendEntry *LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " GeV/c").c_str(), "l");
            Legend->Draw("same");
        }

        if (Histogram1DNameCopy == "Reco FD neutrons momentum (1e cut, FD)") {
            double UpperCut = beamE, LowerCut = 0.4;

            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, 1.);
            UpperMomTh->SetLineWidth(2);
            UpperMomTh->SetLineColor(kBlue);
            UpperMomTh->Draw("same");

            TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, 1.);
            LowerMomTh->SetLineWidth(2);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines, Legend_y1_TwoLines, Legend_x2_TwoLines, Legend_y2_TwoLines);
            TLegendEntry *UpperMomThEntry = Legend->AddEntry(UpperMomTh, "Upper P_{nFD} th. = E_{beam}/c", "l");
            TLegendEntry *LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{nFD} th. = " + to_string_with_precision(LowerCut, 1) + " GeV/c").c_str(), "l");
            Legend->Draw("same");
        }

        if ((findSubstring(Histogram1DNameCopy, "Leading FD neutron momentum APID")) ||
            (Histogram1DNameCopy == "Reco FD neutrons momentum (1e cut, FD)")) {
            double UpperCut = beamE, LowerCut = 0.4;

            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperMomTh->SetLineWidth(2);
            UpperMomTh->SetLineColor(kBlue);
            UpperMomTh->Draw("same");

            TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerMomTh->SetLineWidth(2);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines, Legend_y1_TwoLines, Legend_x2_TwoLines, Legend_y2_TwoLines);
            TLegendEntry *UpperMomThEntry = Legend->AddEntry(UpperMomTh, "Upper P_{nFD} th. = E_{beam}/c", "l");
            TLegendEntry *LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{nFD} th. = " + to_string_with_precision(LowerCut, 1) + " GeV/c").c_str(), "l");
            Legend->Draw("same");
        }

        if ((Histogram1DNameCopy == "CD & FD Piplus momentum #epsilon_{eff} (1e cut)") ||
            (Histogram1DNameCopy == "CD & FD Piminus momentum #epsilon_{eff} (1e cut)")) {
            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            double LowerMomentumTh = 0.2;

            Histogram1D->GetYaxis()->SetRangeUser(0., 0.1);
            Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
            Histogram1D->Sumw2(), Histogram1D->Draw(), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, 0.1);
            LowerMomTh->SetLineWidth(2);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.1 + xOffset, Legend_y2_OneLine + yOffset);

            if (Histogram1DNameCopy == "CD & FD Piplus momentum #epsilon_{eff} (1e cut)") {
                TLegendEntry *LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = "
                                                                              + to_string_with_precision(LowerMomentumTh, 1) + " GeV/c").c_str(), "l");
            } else {
                TLegendEntry *LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = "
                                                                              + to_string_with_precision(LowerMomentumTh, 1) + " GeV/c").c_str(), "l");
            }

            Legend->Draw("same");
        }
    }

    if (!ShowStats) { Histogram1D->SetStats(0); }

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
}
