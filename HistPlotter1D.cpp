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

    //    TH1D *Histogram1D = (TH1D *) file->Get(Histogram1DName);

//    const char *Histogram1DTitle = Histogram1D->GetTitle();

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
        Histogram1D->Sumw2();
        Histogram1D->Draw();

        gPad->Update();

        if (findSubstring(Histogram1DNameCopy, "V_{z}^{")) {
            LogScalePlot = true;

            double UpperCut = 3., LowerCut = -8.;

            TLine *UpperVzCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperVzCut->SetLineWidth(2);
            UpperVzCut->SetLineColor(kBlue);
            UpperVzCut->Draw("same");

            TLine *LowerVzCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerVzCut->SetLineWidth(2);
            LowerVzCut->SetLineColor(kRed);
            LowerVzCut->Draw("same");

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);
            TLegendEntry *UpperVzCutEntry = Legend->AddEntry(UpperVzCut, ("Upper cut = " + to_string_with_precision(UpperCut, 0)).c_str(), "l");
            TLegendEntry *LowerVzCutEntry = Legend->AddEntry(LowerVzCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0)).c_str(), "l");
            Legend->Draw("same");
        }

        if (findSubstring(Histogram1DNameCopy, "Vertex corr. dV^{")) {
            LogScalePlot = true;

            double UpperCut = 4., LowerCut = -5.;

            TLine *UpperVzCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperVzCut->SetLineWidth(2);
            UpperVzCut->SetLineColor(kBlue);
            UpperVzCut->Draw("same");

            TLine *LowerVzCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerVzCut->SetLineWidth(2);
            LowerVzCut->SetLineColor(kRed);
            LowerVzCut->Draw("same");

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);
            TLegendEntry *UpperVzCutEntry = Legend->AddEntry(UpperVzCut, ("Upper cut = " + to_string_with_precision(UpperCut, 0)).c_str(), "l");
            TLegendEntry *LowerVzCutEntry = Legend->AddEntry(LowerVzCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0)).c_str(), "l");
            Legend->Draw("same");
        }

        if (Histogram1DNameCopy == "N_{phe} in HTCC BC (1e cut)") {
            double LowerCut = 2.;
            TLine *LowerNpheCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerNpheCut->SetLineWidth(2);
            LowerNpheCut->SetLineColor(kRed);
            LowerNpheCut->Draw("same");

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);
            TLegendEntry *LowerNpheCutEntry = Legend->AddEntry(LowerNpheCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0)).c_str(), "l");
            Legend->Draw("same");
        }

        if (findSubstring(Histogram1DNameCopy, "#delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN")) {
            double PeakLocation = -2.4, Cut = 15.;
            double UpperCut = Cut - PeakLocation, LowerCut = -Cut + PeakLocation;

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

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.35);
            TLegendEntry *PeakLocEntry = Legend->AddEntry(PeakLoc, ("Peak location = " + to_string_with_precision(UpperCut, 1)).c_str(), "l");
            TLegendEntry *UpperdPhiCutEntry = Legend->AddEntry(UpperdPhiCut, ("Upper cut = " + to_string_with_precision(UpperCut, 1)).c_str(), "l");
            TLegendEntry *LowerdPhiCutEntry = Legend->AddEntry(LowerdPhiCut, ("Lower cut = " + to_string_with_precision(LowerCut, 1)).c_str(), "l");
            Legend->Draw("same");
        }

        if (findSubstring(Histogram1DNameCopy, "Leading FD neutron momentum APID")) {
            double UpperCut = beamE, LowerCut = 0.3;

            TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperMomTh->SetLineWidth(2);
            UpperMomTh->SetLineColor(kBlue);
            UpperMomTh->Draw("same");

            TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerMomTh->SetLineWidth(2);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3);
            TLegendEntry *UpperMomThEntry = Legend->AddEntry(UpperMomTh, "Upper P_{n} th. = E_{beam}/c", "l");
            TLegendEntry *LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1)).c_str(), "l");
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
