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
#include "Histofinder2D.cpp"

#if Independent2Ddraw
#include "TitleAligner.cpp"
#endif

using namespace std;

const string ConfigRegion2D(const string &Histogram2DNameCopy) {
    if (findSubstring(Histogram2DNameCopy, "CD")) {
        return "CD";
    } else {
        return "FD";
    }
}

const string ConfigSName2D(const string &SampleName) {
    if (findSubstring(SampleName, "sim")) {
        return "s";
    } else if (findSubstring(SampleName, "data")) {
        return "d";
    }

    return "";
}

double SetxOffset2D(const bool &ShowStats) {
    double xOffset;

    if (!ShowStats) {
        xOffset = -0.11;
    } else {
        xOffset = 0;
    }

    return xOffset;
}

double SetyOffset2D(const bool &ShowStats) {
    double yOffset;

    if (!ShowStats) {
        yOffset = 0.15;
    } else {
        yOffset = 0;
    }

    return yOffset;
}

void HistPlotter2D(TCanvas *HistogramCanvas, TList *MScThesisPlotsList, const char *filename, const char *Histogram2DName,
                   const string &SampleName, const string &SavePath, const string &SaveName, const bool &Results_plots = false,
                   const string &HistName_Denominator = "", const string &HistName_Numerator = "") {
    bool PresMode = false, ExamPresMode = false;

#if PresentationMode
    PresMode = true;
#endif

#if ExamPresentationMode
    ExamPresMode = true;
#endif

    cout << "\n\n";

    system(("mkdir -p " + SavePath).c_str());

    HistogramCanvas->Clear(); // Clear previous plot

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nHistPlotter2D: could not load root file! Exiting...\n", exit(0); }

    TH2D *Histogram2D;

    if (file->Get(Histogram2DName) == nullptr ||
        (findSubstring(Histogram2DName, "vs") || findSubstring(Histogram2DName, "VS"))) {
        Histogram2D = Histofinder2D(file, Histogram2DName, false);
    } else {
        Histogram2D = (TH2D *) file->Get(Histogram2DName);
    }

    double Legend_x1_BaseLine = gStyle->GetStatX(), Legend_y1_BaseLine = gStyle->GetStatY(); // Top right
    double Legend_x2_BaseLine = gStyle->GetStatX(), Legend_y2_BaseLine = gStyle->GetStatY(); // Bottom left

    double Legend_x1_OneLine = Legend_x1_BaseLine, Legend_y1_OneLine = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_OneLine = Legend_x2_BaseLine - 0.2, Legend_y2_OneLine = Legend_y2_BaseLine - 0.25; // Bottom left

    double Legend_x1_TwoLines = Legend_x1_BaseLine, Legend_y1_TwoLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_TwoLines = Legend_x2_BaseLine - 0.2, Legend_y2_TwoLines = Legend_y2_BaseLine - 0.3; // Bottom left

    double Legend_x1_ThreeLines = Legend_x1_BaseLine, Legend_y1_ThreeLines = Legend_y1_BaseLine - 0.2; // Top right
    double Legend_x2_ThreeLines = Legend_x2_BaseLine - 0.2, Legend_y2_ThreeLines = Legend_y2_BaseLine - 0.35; // Bottom left

    const string Histogram2DNameCopy = Histogram2DName;
    int LineColor = 1;
    int LineWidth = 2;
    vector<double> Histogram2DTitleSizes = {0.06, 0.0425, 0.0425, 0.0425}; // {TitleSize, LabelSizex, LabelSizey, LabelSizez}
    bool CenterTitle = true;
    bool ShowStats = false;

    /* Histogram scale setup */
    bool ZLogScalePlot = true;
    bool ZLinearScalePlot = true;

    HistogramCanvas->cd();

    const double beamE = 5.98636;
    const string Region = ConfigRegion2D(Histogram2DNameCopy);
    string sNameFlag = ConfigSName2D(SampleName);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
    double x_1 = 0.18, y_1 = 0.3, x_2 = 0.86, y_2 = 0.7;
    double diplayTextSize = 0.1;

    Histogram2D->SetTitleSize(Histogram2DTitleSizes.at(0), "xyz");
    Histogram2D->GetXaxis()->SetLabelSize(Histogram2DTitleSizes.at(1));
    Histogram2D->GetXaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetYaxis()->SetLabelSize(Histogram2DTitleSizes.at(2));
    Histogram2D->GetYaxis()->CenterTitle(CenterTitle);
    Histogram2D->GetZaxis()->SetLabelSize(Histogram2DTitleSizes.at(3));
    MScThesisPlotsList->Add(Histogram2D);

    if (Histogram2D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram2D->Draw();
        displayText->Draw();
    } else if (Histogram2D->Integral() != 0.) {
        string Histogram2D_Title = Histogram2D->GetTitle();
        string Histogram2D_xLabel = Histogram2D->GetXaxis()->GetTitle();
        string Histogram2D_yLabel = Histogram2D->GetYaxis()->GetTitle();

        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1nFD, FD)", " in 1nFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1p", "1pFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1n", "1nFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "pFDpCD", "1pFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "nFDpCD", "1nFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (All Int., 1pFD1pCD)", " in 1pFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (All Int., 1nFD1pCD)", " in 1nFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (All Int., 1pFD1pCD, FD)", " in 1pFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (All Int., 1nFD1pCD, FD)", " in 1nFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1pFD, FD)", " in 1pFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1nFD, FD)", " in 1nFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1pFD0pCD, FD)", " in 1pFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1nFD0pCD, FD)", " in 1nFD0pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1e_cut, FD)", "");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1e cut, FD)", "");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1e_cut)", "");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " (1e cut)", "");

        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[Deg]", "[#circ]");

        if (!PresMode) {
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[GeV/c]", "[GeV]");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[GeV/c^{2}]", "[GeV]");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[GeV^{2}/c^{2}]", "[GeV^{2}]");
        } else if (PresMode && ExamPresMode) {
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[GeV/c]", "[GeV]");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[GeV/c^{2}]", "[GeV]");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "[GeV^{2}/c^{2}]", "[GeV^{2}]");
        }

        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, " - ZOOMIN", " zoom-in");

        if (findSubstring(Histogram2DNameCopy, "dc_hitmap")) {

            ShowStats = false;

            Histogram2D->GetXaxis()->SetTitle("x [cm]");
            Histogram2D->GetYaxis()->SetTitle("y [cm]");
            Histogram2D->Draw("colz"), gPad->Update();
        } else if (findSubstring(Histogram2DNameCopy, "#beta vs. P") &&
                   !(Histogram2DNameCopy == "#beta vs. P (electrons only, 1e cut)")) {
            if ((Histogram2DNameCopy == "#beta vs. P (all particles, no #(e) cut, CD)") ||
                (Histogram2DNameCopy == "#beta vs. P (all particles, no #(e) cut, FD)") ||
                (Histogram2DNameCopy == "#beta vs. P (all particles, 1e cut, CD)") ||
                (Histogram2DNameCopy == "#beta vs. P (all particles, 1e cut, FD)")) {
                Histogram2D->Draw("colz"), gPad->Update();

                TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
                EquiLine->SetLineWidth(5);
                EquiLine->SetLineColor(kBlack);
                EquiLine->Draw("same");
            }
        } else if ((Histogram2DNameCopy == "SF vs. P_{e} BC (1e cut, FD)") || (Histogram2DNameCopy == "SF vs. P_{e} (1e cut, FD)")) {
            Histogram2D->SetTitle("Electron sampling fraction vs. momentum");
            Histogram2D->GetYaxis()->SetTitle("f_{e}");
            Histogram2D->Draw("colz"), gPad->Update();

            TLine *UpperFScut = new TLine(gPad->GetUxmin(), 0.28, gPad->GetUxmax(), 0.28);
            UpperFScut->SetLineWidth(5);
            UpperFScut->SetLineColor(kRed);
            UpperFScut->Draw("same");

            TLine *LowerFScut = new TLine(gPad->GetUxmin(), 0.2, gPad->GetUxmax(), 0.2);
            LowerFScut->SetLineWidth(5);
            LowerFScut->SetLineColor(kRed);
            LowerFScut->Draw("same");
        } else if ((Histogram2DNameCopy == "Vcal vs. SF BC (1e cut, PCAL)" || Histogram2DNameCopy == "Wcal vs. SF BC (1e cut, PCAL)") ||
                   (Histogram2DNameCopy == "Vcal vs. SF (1e cut, PCAL)" || Histogram2DNameCopy == "Wcal vs. SF (1e cut, PCAL)")) {
            if ((Histogram2DNameCopy == "Vcal vs. SF BC (1e cut, PCAL)") || (Histogram2DNameCopy == "Vcal vs. SF (1e cut, PCAL)")) {
                Histogram2D->SetTitle("PCAL #font[12]{l}_{V} coordinate vs. SF");
                Histogram2D->GetXaxis()->SetTitle("#font[12]{l}_{V} [cm]");
            } else if ((Histogram2DNameCopy == "Wcal vs. SF BC (1e cut, PCAL)") || (Histogram2DNameCopy == "Wcal vs. SF (1e cut, PCAL)")) {
                Histogram2D->SetTitle("PCAL #font[12]{l}_{W} coordinate vs. SF");
                Histogram2D->GetXaxis()->SetTitle("#font[12]{l}_{W} [cm]");
            }

            Histogram2D->GetYaxis()->SetTitle("f_{e}");
            Histogram2D->Draw("colz"), gPad->Update();

            TLine *LowerECALcoorCut = new TLine(14., gPad->GetUymin(), 14., gPad->GetUymax());
            LowerECALcoorCut->SetLineWidth(5);
            LowerECALcoorCut->SetLineColor(kRed);
            LowerECALcoorCut->Draw("same");
        } else if (Histogram2DNameCopy == "#beta vs. P (electrons only, 1e cut)") {
            Histogram2D->SetTitle("Electron #beta_{e} vs. momentum");

            if (PresMode) {
                if (ExamPresMode) {
                    Histogram2D->GetXaxis()->SetTitle("P_{e} [GeV]");
                } else {
                    Histogram2D->GetXaxis()->SetTitle("P_{e} [GeV/c]");
                }
            } else {
                Histogram2D->GetXaxis()->SetTitle("P_{e} [GeV]");
            }

            Histogram2D->GetYaxis()->SetTitle("#beta_{e}");
            Histogram2D->Draw("colz"), gPad->Update();

            TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
            EquiLine->SetLineWidth(5);
            EquiLine->SetLineColor(kBlack);
            EquiLine->Draw("same");

            TLine *UpperBetaElectronCut = new TLine(gPad->GetUxmin(), 1.2, gPad->GetUxmax(), 1.2);
            UpperBetaElectronCut->SetLineWidth(5);
            UpperBetaElectronCut->SetLineColor(kRed);
            UpperBetaElectronCut->Draw("same");
        } else if (Histogram2DNameCopy == "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)" ||
                   Histogram2DNameCopy == "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)") {
            if (Histogram2DNameCopy == "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)") {
                Histogram2D->SetTitle("#theta_{p_{1},p_{2}} vs. CTOF time-of-flight difference in #font[12]{2p}");
                Histogram2D->GetYaxis()->SetTitle("Time-of-flight difference [ns]");
            } else if (Histogram2DNameCopy == "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)") {
                Histogram2D->SetTitle("#theta_{p_{1},p_{2}} vs. CTOF hit position difference in #font[12]{2p}");
                Histogram2D->GetYaxis()->SetTitle("Hit position difference [cm]");
            }

            Histogram2D->Draw("colz"), gPad->Update();
        } else if (Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)" ||
                   Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)") {
            if (Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)") {
                Histogram2D->SetTitle("#theta_{pFD} vs. #theta_{pCD} in 1pFD1pCD");
//                Histogram2D->SetTitle("#theta_{pFD} vs. #theta_{pCD} for every #theta_{pFD,pCD} in 1pFD1pCD");
            } else if (Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)") {
                Histogram2D->SetTitle("#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD} < 20#circ in 1pFD1pCD");
            }

            Histogram2D->Draw("colz"), gPad->Update();

            TLine *UpperThetapFDcut = new TLine(gPad->GetUxmin(), 45., gPad->GetUxmax(), 45.);
            UpperThetapFDcut->SetLineWidth(5), UpperThetapFDcut->SetLineColor(kGreen), UpperThetapFDcut->Draw("same");

            TLine *LowerThetapFDcut = new TLine(gPad->GetUxmin(), 35., gPad->GetUxmax(), 35.);
            LowerThetapFDcut->SetLineWidth(5), LowerThetapFDcut->SetLineColor(kGreen), LowerThetapFDcut->Draw("same");

            TLine *UpperThetapCDcut = new TLine(45., gPad->GetUymin(), 45., gPad->GetUymax());
            UpperThetapCDcut->SetLineWidth(5), UpperThetapCDcut->SetLineColor(kRed), UpperThetapCDcut->Draw("same");

            TLine *LowerThetapCDcut = new TLine(35., gPad->GetUymin(), 35., gPad->GetUymax());
            LowerThetapCDcut->SetLineWidth(5), LowerThetapCDcut->SetLineColor(kRed), LowerThetapCDcut->Draw("same");

//            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
//                         "pFD", "#tilde{pFD}");
//            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
//                         "pCD", "#tilde{pCD}");

        } else if (findSubstring(Histogram2DNameCopy, "P_{p} vs. #theta_{p}")) {
            ShowStats = false;

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            double LowerMomentumTh = 0.4;

            if (findSubstring(Histogram2DNameCopy, "FD")) {
                Histogram2D->SetTitle("Forward-going proton momentum vs. scattering angle");
            } else if (findSubstring(Histogram2DNameCopy, "CD")) {
                Histogram2D->SetTitle("Central-going proton momentum vs. scattering angle");
            }

            Histogram2D->Draw("colz"), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, gPad->GetUymax());
            LowerMomTh->SetLineWidth(5);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + 0.05 + xOffset, Legend_y2_OneLine + yOffset);
//            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + xOffset, Legend_y2_OneLine + yOffset);

            TLegendEntry *LowerMomThEntry;

            if (PresMode) {
                if (ExamPresMode) {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                    Histogram2D->GetXaxis()->SetTitle("P_{p} [GeV]");
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                    Histogram2D->GetXaxis()->SetTitle("P_{p} [GeV/c]");
                }
            } else {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                Histogram2D->GetXaxis()->SetTitle("P_{p} [GeV]");
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram2DNameCopy, "P_{#pi^{+}} vs. #theta_{#pi^{+}}") ||
                   findSubstring(Histogram2DNameCopy, "P_{#pi^{-}} vs. #theta_{#pi^{-}}")) {
            ShowStats = false;

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            double LowerMomentumTh = 0.2;

            if (findSubstring(Histogram2DNameCopy, "P_{#pi^{+}} vs. #theta_{#pi^{+}}")) {
                if (findSubstring(Histogram2DNameCopy, "FD")) {
                    Histogram2D->SetTitle("Forward-going #pi^{+} momentum vs. scattering angle");
                } else if (findSubstring(Histogram2DNameCopy, "CD")) {
                    Histogram2D->SetTitle("Central-going #pi^{+} momentum vs. scattering angle");
                }
            } else if (findSubstring(Histogram2DNameCopy, "P_{#pi^{-}} vs. #theta_{#pi^{-}}")) {
                if (findSubstring(Histogram2DNameCopy, "FD")) {
                    Histogram2D->SetTitle("Forward-going #pi^{-} momentum vs. scattering angle");
                } else if (findSubstring(Histogram2DNameCopy, "CD")) {
                    Histogram2D->SetTitle("Central-going #pi^{-} momentum vs. scattering angle");
                }
            }

            Histogram2D->Draw("colz"), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, gPad->GetUymax());
            LowerMomTh->SetLineWidth(5), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + 0.05 + xOffset, Legend_y2_OneLine + yOffset);
//            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + xOffset, Legend_y2_OneLine + yOffset);

            TLegendEntry *LowerMomThEntry;

            if (PresMode) {
                if (ExamPresMode) {
                    if (findSubstring(Histogram2DNameCopy, "#pi^{+}")) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                        Histogram2D->GetXaxis()->SetTitle("P_{#pi^{+}} [GeV]");
                    } else if (findSubstring(Histogram2DNameCopy, "#pi^{-}")) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                        Histogram2D->GetXaxis()->SetTitle("P_{#pi^{-}} [GeV]");
                    }
                } else {
                    if (findSubstring(Histogram2DNameCopy, "#pi^{+}")) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                        Histogram2D->GetXaxis()->SetTitle("P_{#pi^{+}} [GeV/c]");
                    } else if (findSubstring(Histogram2DNameCopy, "#pi^{-}")) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                        Histogram2D->GetXaxis()->SetTitle("P_{#pi^{-}} [GeV/c]");
                    }
                }
            } else {
                if (findSubstring(Histogram2DNameCopy, "#pi^{+}")) {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                    Histogram2D->GetXaxis()->SetTitle("P_{#pi^{+}} [GeV]");
                } else if (findSubstring(Histogram2DNameCopy, "#pi^{-}")) {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                    Histogram2D->GetXaxis()->SetTitle("P_{#pi^{-}} [GeV]");
                }
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram2DNameCopy, "R_{nFD} vs. P^{reco}_{nFD}") ||
                   findSubstring(Histogram2DNameCopy, "R_{nFD} vs. P^{truth}_{nFD}") ||
                   findSubstring(Histogram2DNameCopy, "R_{pFD} vs. P^{reco}_{pFD}") ||
                   findSubstring(Histogram2DNameCopy, "R_{pFD} vs. P^{truth}_{pFD}")) {
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "Resolution = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}", "R_{nFD}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}", "R_{pFD}");
            ShowStats = false;
            Histogram2D->Draw("colz"), gPad->Update();
        } else if (findSubstring(Histogram2DNameCopy, "E_{cal} vs. #theta_{pCD} (All Int., nFDpCD)")) {
            ShowStats = false;

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            Histogram2D->SetTitle("E_{cal} vs. #theta_{pCD} in 1nFD1pCD");
            Histogram2D->Draw("colz"), gPad->Update();

            TLine *BeamELine = new TLine(gPad->GetUxmin(), beamE, gPad->GetUxmax(), beamE);
            BeamELine->SetLineWidth(5), BeamELine->SetLineColor(kBlack), BeamELine->Draw("same");

            TLine *LowerThetaLim = new TLine(35., gPad->GetUymin(), 35., gPad->GetUymax());
            LowerThetaLim->SetLineWidth(5), LowerThetaLim->SetLineColor(kRed), LowerThetaLim->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset,
                                      Legend_x2_TwoLines - 0.15 + xOffset + 0.025 * 4, Legend_y2_TwoLines + yOffset);
            TLegendEntry *BeamELineEntry;

            if (PresMode) {
                if (ExamPresMode) {
                    BeamELineEntry = Legend->AddEntry(BeamELine, ("E_{beam} = " + to_string_with_precision(beamE, 3) + " [GeV]").c_str(), "l");
                } else {
                    BeamELineEntry = Legend->AddEntry(BeamELine, ("E_{beam} = " + to_string_with_precision(beamE, 3) + " [GeV/c]").c_str(), "l");
                }
            } else {
                BeamELineEntry = Legend->AddEntry(BeamELine, ("E_{beam} = " + to_string_with_precision(beamE, 3) + " [GeV]").c_str(), "l");
            }

            TLegendEntry *LowerThetaLimEntry = Legend->AddEntry(LowerThetaLim, ("#theta_{pCD} = " + to_string(35) + " [#circ]").c_str(), "l");
            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram2DNameCopy, "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e}}") ||
                   findSubstring(Histogram2DNameCopy, "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD}") ||
                   findSubstring(Histogram2DNameCopy, "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e}") ||
                   findSubstring(Histogram2DNameCopy, "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD}")) {
            ShowStats = false;

            string TitleTemp = Histogram2D->GetTitle(), xLabelTemp = Histogram2D->GetXaxis()->GetTitle(), yLabelTemp = Histogram2D->GetYaxis()->GetTitle();
            string dTheta2Replace = yLabelTemp.substr(0, yLabelTemp.find_last_of("}") + 1);
            string dPhi2Replace = xLabelTemp.substr(0, xLabelTemp.find_last_of("}") + 1);
            string dTheta = yLabelTemp.substr(0, yLabelTemp.find("}") + 1);
            string dPhi = xLabelTemp.substr(0, xLabelTemp.find("}") + 1);

            string Title2Replace = TitleTemp.substr(0, TitleTemp.find("Veto") + 4), Title2Replacement = dTheta + " vs. " + dPhi;
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, Title2Replace, Title2Replacement);

            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, dTheta2Replace, dTheta);
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, dPhi2Replace, dPhi);
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "pFD", "p");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "  ", " ");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "   ", " ");

            if (!findSubstring(Histogram2DNameCopy, "LnFD")) {
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "nFD", "n");
            }

            if (findSubstring(Histogram2DNameCopy, "BV")) {
                string Title2Replacement = dTheta + " vs. " + dPhi;
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, Title2Replace, Title2Replacement);

                string TitleTemp1 = Histogram2D->GetTitle(), TitleTemp2 = TitleTemp1 + " before ECAL veto";
                Histogram2D->SetTitle(TitleTemp2.c_str()), gPad->Update();
            } else if (findSubstring(Histogram2DNameCopy, "AV")) {
                string Title2Replacement = dTheta + " vs. " + dPhi;
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, Title2Replace, Title2Replacement);

                string TitleTemp1 = Histogram2D->GetTitle(), TitleTemp2 = TitleTemp1 + " after ECAL veto";
                Histogram2D->SetTitle(TitleTemp2.c_str()), gPad->Update();
            }

            Histogram2D->Draw("colz"), gPad->Update();
        } else {
            Histogram2D->SetStats(0);

            //<editor-fold desc="Align Z axis of denominators and numerator">
            if (!findSubstring(Histogram2D_Title, "Veto")) {
                bool Equi_z_2D = false;

                if (!findSubstring(Histogram2DName, "FSRatio")) {
                    TH2D *Histogram2D_Denominator = Histofinder2D(file, HistName_Denominator.c_str(), false);
                    TH2D *Histogram2D_Numerator = Histofinder2D(file, HistName_Numerator.c_str(), false);

                    double Zmax = 1.1 * max(Histogram2D_Denominator->GetMaximum(), Histogram2D_Numerator->GetMaximum());

                    if (Equi_z_2D) { Histogram2D->SetMaximum(Zmax); }
                } else {
                    string StringTemp0 = Histogram2D->GetTitle();
                    string StringTemp = StringTemp0.substr(StringTemp0.find_last_of("ratio") + 1);
                    TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, StringTemp, "");

                    Histogram2D->SetMaximum(5.);
                    Histogram2D->SetMinimum(0.1);

                    auto FuncList = Histogram2D->GetListOfFunctions();
                    FuncList->Clear();
                }
            }
            //</editor-fold>

            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}|", "P_{tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}|", "P_{tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2", "P_{rel}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2", "P_{rel}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "E_{cal} = E_{e} + T_{nFD} + T_{pCD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "E_{cal} = E_{e} + T_{pFD} + T_{pCD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "E_{cal} = E_{e} + T_{nucFD} + T_{nucCD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}|", "#deltaP_{T,tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}|", "#deltaP_{T,tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}} ", "W");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}} ", "W");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}|", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nucL} + #vec{P}_{nucR}- #vec{q}|", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#vec{P}_{tot}-#vec{q}", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#vec{P}", "#font[62]{P}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "#vec{q}", "#font[62]{q}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "[Deg]", "[#circ]");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "vs. W", "vs. hadronic mass");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "E_{cal} vs. #delta#alpha_{T,tot}", "#delta#alpha_{T,tot} vs. E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "E_{cal} vs. #deltaP_{T,tot}", "#deltaP_{T,tot} vs. E_{cal}");

            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "ratio", "- 1nFD1pCD/1pFD1pCD");

            if (Results_plots) {
                string Filename = filename, Histogram2D_title_2 = Histogram2D->GetTitle();
                string Histogram2D_title_2_replacement;

                if (findSubstring(Filename, "simulation")) {
                    Histogram2D_title_2_replacement = Histogram2D_title_2 + " (simulation)";
                } else if (findSubstring(Filename, "data")) {
                    Histogram2D_title_2_replacement = Histogram2D_title_2 + " (data)";
                }

                TitleAligner(Histogram2D, Histogram2D_title_2, Histogram2D_xLabel, Histogram2D_yLabel, Histogram2D_title_2, Histogram2D_title_2_replacement);
            }

            if (PresMode) {
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1pFD0pCD", "1p");
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1nFD0pCD", "1n");
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1pFD1pCD", "2p");
                TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1nFD1pCD", "1n1p");
            }

            Histogram2D->Draw("colz"), gPad->Update();

            //<editor-fold desc="EquiLine2D for momentum 2D plots">
            if (findSubstring(Histogram2D_Title, "P_{pFD} vs. P_{pCD}") || findSubstring(Histogram2D_Title, "P_{nFD} vs. P_{pCD}") ||
                findSubstring(Histogram2D_Title, "P_{pL} vs. P_{pR}") || findSubstring(Histogram2D_Title, "P_{nL} vs. P_{nR}") ||
                findSubstring(Histogram2D_Title, "P_{nucFD} vs. P_{nucCD}") || findSubstring(Histogram2D_Title, "P_{nucL} vs. P_{nucR}")) {
                ShowStats = false;

                double Lowerlim_FSRatio = max(Histogram2D->GetXaxis()->GetXmin(), Histogram2D->GetYaxis()->GetXmin());
                double Upperlim_FSRatio = min(Histogram2D->GetXaxis()->GetXmax(), Histogram2D->GetYaxis()->GetXmax());
                double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

                TLine *EquiLine2D = new TLine(Lowerlim_FSRatio, Lowerlim_FSRatio, Upperlim_FSRatio, Upperlim_FSRatio);
                EquiLine2D->SetLineWidth(2), EquiLine2D->SetLineColor(kRed), EquiLine2D->Draw("same");

                auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine + xOffset + 0.08, Legend_y2_OneLine + yOffset);
                TLegendEntry *EquiLine2D_entry = Legend->AddEntry(EquiLine2D, "y(x) = x", "l");
                Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
            }
            //</editor-fold>

        }
    }

    gStyle->SetStatX(0.87);
    gStyle->SetStatY(0.4);

    if (!ShowStats || Histogram2D->Integral() == 0.) { Histogram2D->SetStats(0); }

    if (ZLogScalePlot) {
        HistogramCanvas->SetLogz(1);
        HistogramCanvas->SaveAs((SavePath + "/" + sNameFlag + SaveName + "_zLogScale.png").c_str());
    }

    if (ZLinearScalePlot) {
        HistogramCanvas->SetLogz(0);
        system(("mkdir -p " + SavePath + "/z_Linear_Scale_plots").c_str());
        HistogramCanvas->SaveAs((SavePath + "/z_Linear_Scale_plots/" + sNameFlag + SaveName + "_zLinearScale.png").c_str());
    }

    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);

    HistogramCanvas->Clear();
    file->Close();
    delete file;
}
