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

#include "../../source/classes/hData/hData.cpp"
#include "../../source/functions/GeneralFunctions.h"
#include "../../source/constants.h"
#include "Histofinder1D.cpp"

#if Independent1DHistDraw
#include "TitleAligner.cpp"
#endif

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

    return "";
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

void HistPlotter1D(TCanvas *HistogramCanvas, TList *MScThesisPlotsList, const char *filename, const char *Histogram1DName,
                   const string &SampleName, const string &SavePath, const string &SaveName, const bool TLmom = false) {
    bool PresMode = false, ExamPresMode = false;

#if PresentationMode
    PresMode = true;
#endif

#if ExamPresentationMode
    ExamPresMode = true;
#endif

    cout << "\n\n";

    hData particles;

    HistogramCanvas->Clear();

    TFile *file = new TFile(filename);
    TH1D *Histogram1D;

    if (!findSubstring(Histogram1DName, "FSRatio")) {
        if (findSubstring(Histogram1DName, "W distribution") ||
            findSubstring(Histogram1DName, "#theta_{pFD,pCD}") || findSubstring(Histogram1DName, "#theta_{nFD,pCD}")) {
            Histogram1D = Histofinder1D(file, Histogram1DName);
        } else {
            if (file->Get(Histogram1DName) == nullptr) {
                Histogram1D = Histofinder1D(file, Histogram1DName, TLmom);
            } else {
                Histogram1D = (TH1D *) file->Get(Histogram1DName);
            }
        }
    } else {
        Histogram1D = Histofinder1D(file, Histogram1DName, TLmom);
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
//    int LineWidth = 3; // original
//    int LineWidth = 6; // size used in results part
    int LineWidth = 3;
//    int LineWidth = 4;
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

    Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Histogram1D->GetXaxis()->CenterTitle(true);
    Histogram1D->GetYaxis()->SetTitle("Number of events");
    Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Histogram1D->GetYaxis()->CenterTitle(true);
    Histogram1D->SetLineWidth(LineWidth);
    Histogram1D->SetLineStyle(0);
    Histogram1D->SetLineColor(kBlack);
    MScThesisPlotsList->Add(Histogram1D);

    if (Histogram1D->Integral() == 0.) {
        TPaveText *displayText = new TPaveText(x_1, y_1, x_2, y_2, "NDC");
        displayText->SetTextSize(diplayTextSize);
        displayText->SetFillColor(0);
        displayText->AddText("Empty histogram");
        displayText->SetTextAlign(22);
        Histogram1D->Draw();
        displayText->Draw();
    } else if (Histogram1D->Integral() != 0.) {
        string Histogram1D_Title = Histogram1D->GetTitle();
        string Histogram1D_Title1 = Histogram1D_Title;
        string Histogram1D_xLabel = Histogram1D->GetXaxis()->GetTitle(), Histogram1D_yLabel = Histogram1D->GetYaxis()->GetTitle();
        string FSRyLabel;

        string RatioVar = Histogram1D_xLabel.substr(0, Histogram1D_xLabel.find_last_of('[') - 1);
        string RatioTopology;

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
            if (findSubstring(Histogram1D_Title, "1n/1p") || findSubstring(Histogram1D_Title, "1nFD/1pFD")) {
                RatioTopology = "1N";
            } else if (findSubstring(Histogram1D_Title, "nFDpCD/pFDpCD") || findSubstring(Histogram1D_Title, "1nFD1pCD/1pFD1pCD")) {
                RatioTopology = "2N";
            }

            if (PresMode) {
                if (ExamPresMode) {
                    if (findSubstring(Histogram1D_xLabel, "Momentum [GeV]")) { RatioVar = "P^{truth}_{nucFD}"; }

                    if (RatioTopology == "1N") {
                        FSRyLabel = "r^{" + RatioVar + "}_{" + RatioTopology + "} = #frac{1nFD}{1pFD}";
                    } else if (RatioTopology == "2N") {
                        FSRyLabel = "r_{" + RatioVar + "} = #frac{1nFD1pCD}{1pFD1pCD}";
                    }
                } else {
                    if (findSubstring(Histogram1D_xLabel, "Momentum [GeV/c]")) { RatioVar = "P^{truth}_{nucFD}"; }

                    if (RatioTopology == "1N") {
                        FSRyLabel = "r^{" + RatioVar + "}_{" + RatioTopology + "} = #frac{1n}{1p}";
                    } else if (RatioTopology == "2N") {
                        FSRyLabel = "r_{" + RatioVar + "} = #frac{1n1p}{2p}";
                    }

                }
            } else {
                if (findSubstring(Histogram1D_xLabel, "Momentum [GeV]")) { RatioVar = "P^{truth}_{nucFD}"; }

                if (RatioTopology == "1N") {
                    FSRyLabel = "r^{" + RatioVar + "}_{" + RatioTopology + "} = #frac{1nFD}{1pFD}";
                } else if (RatioTopology == "2N") {
                    FSRyLabel = "r_{" + RatioVar + "} = #frac{1nFD1pCD}{1pFD1pCD}";
                }
            }
        } else {
            if (findSubstring(Histogram1D_Title, "1n") || findSubstring(Histogram1D_Title, "1p") ||
                findSubstring(Histogram1D_Title, "1nFD") || findSubstring(Histogram1D_Title, "1pFD")) {
                RatioTopology = "1N";
            } else if (findSubstring(Histogram1D_Title, "nFDpCD/pFDpCD") || findSubstring(Histogram1D_Title, "1nFD1pCD/1pFD1pCD")) {
                RatioTopology = "2N";
            }
        }

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
            string StringTemp0 = Histogram1D->GetTitle();
            string StringTemp = StringTemp0.substr(StringTemp0.find_last_of("ratio") + 1);
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, StringTemp, "");

            if (RatioTopology == "1N") { TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "Leading nucleon", "Nucleon"); }
        }

        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1nFD, FD)", " in 1nFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1p", "1pFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1n", "1nFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "pFDpCD", "1pFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "nFDpCD", "1nFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (All Int., 1pFD1pCD)", " in 1pFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (All Int., 1nFD1pCD)", " in 1nFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (All Int., 1pFD1pCD, FD)", " in 1pFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (All Int., 1nFD1pCD, FD)", " in 1nFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1pFD, FD)", " in 1pFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1nFD, FD)", " in 1nFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1pFD0pCD, FD)", " in 1pFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1nFD0pCD, FD)", " in 1nFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1pFD1pCD, FD)", " in 1pFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1nFD1pCD, FD)", " in 1nFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1e_cut, FD)", "");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1e cut, FD)", "");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1e_cut)", "");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (1e cut)", "");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1pFD0pCD)", "in 1pFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1nFD0pCD)", "in 1nFD0pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1pFD1pCD)", "in 1pFD1pCD");
        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1nFD1pCD)", "in 1nFD1pCD");

        if (PresMode) {
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1pFD0pCD", "1p");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1nFD0pCD", "1n");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1pFD1pCD", "2p");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1nFD1pCD", "1n1p");
        }

        TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[Deg]", "[#circ]");

        if (!PresMode) {
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[GeV/c]", "[GeV]");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[GeV/c^{2}]", "[GeV]");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[GeV^{2}/c^{2}]", "[GeV^{2}]");
        } else if (PresMode && ExamPresMode) {
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[GeV/c]", "[GeV]");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[GeV/c^{2}]", "[GeV]");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "[GeV^{2}/c^{2}]", "[GeV^{2}]");
        }

        if (RatioTopology == "1N" && findSubstring(Histogram1D_Title, "truth")) {
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD proton", "Proton");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD neutron", "Neutron");

            if (PresMode) {
                if (ExamPresMode) {
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P^{truth}_{p} [GeV]", "P^{truth}_{pFD} [GeV]");
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P^{truth}_{n} [GeV]", "P^{truth}_{nFD} [GeV]");
                } else {
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P^{truth}_{p} [GeV/c]", "P^{truth}_{pFD} [GeV/c]");
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P^{truth}_{n} [GeV/c]", "P^{truth}_{nFD} [GeV/c]");
                }
            } else {
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P^{truth}_{p} [GeV]", "P^{truth}_{pFD} [GeV]");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P^{truth}_{n} [GeV]", "P^{truth}_{nFD} [GeV]");
            }
        }

        if (findSubstring(Histogram1DNameCopy, "V_{z}^{")) {
            LogScalePlot = true, ShowStats = false;

            string HistTitle = Histogram1D_Title1;
            string ParticleShort = particles.GetParticleNameShortFromSubscript(HistTitle);
            string ParticlesLC;
            string Region = particles.GetDRegionExplicit(HistTitle, true);

            if (ParticleShort == "#pi^{+}" || ParticleShort == "#pi^{-}" || ParticleShort == "K^{+}" || ParticleShort == "K^{-}") {
                ParticlesLC = particles.GetParticleNameLCFromSubscript(HistTitle, false);
            } else {
                ParticlesLC = particles.GetParticleNameLCFromSubscript(HistTitle, true);
            }

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats), UpperCut, LowerCut;

            if (findSubstring(Histogram1DNameCopy, "FD") || (ParticleShort == "e")) {
                UpperCut = 3., LowerCut = -8.;
            } else if (findSubstring(Histogram1DNameCopy, "CD")) {
                UpperCut = 2., LowerCut = -7.;
            }

            if (ParticleShort == "e") {
                Histogram1D->SetTitle(("V_{z}^{" + ParticleShort + "} of " + ParticlesLC + "").c_str());
            } else {
                Histogram1D->SetTitle(("V_{z}^{" + ParticleShort + "} of " + Region + " " + ParticlesLC + "").c_str());
            }

            Histogram1D->Draw(), gPad->Update();

            TLine *UpperVzCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperVzCut->SetLineWidth(5), UpperVzCut->SetLineColor(kBlue), UpperVzCut->Draw("same");

            TLine *LowerVzCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerVzCut->SetLineWidth(5), LowerVzCut->SetLineColor(kRed), LowerVzCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines - 0.05 + xOffset, Legend_y2_TwoLines + yOffset);
            TLegendEntry *UpperVzCutEntry = Legend->AddEntry(UpperVzCut, ("Upper cut = " + to_string_with_precision(UpperCut, 0) + " [cm]").c_str(), "l");
            TLegendEntry *LowerVzCutEntry = Legend->AddEntry(LowerVzCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0) + " [cm]").c_str(), "l");
            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram1DNameCopy, "Vertex corr. dV^{")) {
            LogScalePlot = true, ShowStats = false;

            string HistTitle = Histogram1D_Title1;
            string ParticleShort = particles.GetParticleNameShortFromSubscript(HistTitle.substr(0, HistTitle.find_last_of('=') + 1));
            string ParticlesLC;
            string Region = particles.GetDRegionExplicit(HistTitle, true);

            if (ParticleShort == "#pi^{+}" || ParticleShort == "#pi^{-}" || ParticleShort == "K^{+}" || ParticleShort == "K^{-}") {
                ParticlesLC = particles.GetParticleNameLCFromSubscript("{" + ParticleShort + "}", false);
            } else {
                ParticlesLC = particles.GetParticleNameLCFromSubscript("{" + ParticleShort + "}", true);
            }

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats), UpperCut = 4., LowerCut = -5.;

            if (ParticleShort == "e") {
                Histogram1D->SetTitle(("dV_{z}^{" + ParticleShort + "} of " + ParticlesLC + "").c_str());
            } else {
                Histogram1D->SetTitle(("dV_{z}^{" + ParticleShort + "} of " + Region + " " + ParticlesLC + "").c_str());
            }

            Histogram1D->Draw(), gPad->Update();

            TLine *UpperVzCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperVzCut->SetLineWidth(5), UpperVzCut->SetLineColor(kBlue), UpperVzCut->Draw("same");

            TLine *LowerVzCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerVzCut->SetLineWidth(5), LowerVzCut->SetLineColor(kRed), LowerVzCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines - 0.05 + xOffset, Legend_y2_TwoLines + yOffset);
            TLegendEntry *UpperVzCutEntry = Legend->AddEntry(UpperVzCut, ("Upper cut = " + to_string_with_precision(UpperCut, 0) + " [cm]").c_str(), "l");
            TLegendEntry *LowerVzCutEntry = Legend->AddEntry(LowerVzCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0) + " [cm]").c_str(), "l");
            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (Histogram1DNameCopy == "N_{phe} in HTCC BC (1e cut)" || Histogram1DNameCopy == "N_{phe} in HTCC (1e cut)") {
            ShowStats = false;

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            double LowerCut = 2.;

            Histogram1D->SetTitle("Number of photo-electrons in the HTCC for ^{12}C(e,e')");
            Histogram1D->Draw(), gPad->Update();

            TLine *LowerNpheCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerNpheCut->SetLineWidth(5);
            LowerNpheCut->SetLineColor(kRed);
            LowerNpheCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine + xOffset, Legend_y2_OneLine + yOffset);
            TLegendEntry *LowerNpheCutEntry = Legend->AddEntry(LowerNpheCut, ("Lower cut = " + to_string_with_precision(LowerCut, 0)).c_str(), "l");
            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram1DNameCopy, "#chi^{2}")) {
            string HistTitle = Histogram1D_Title1;
            string ParticleShort = particles.GetParticleNameShortFromSubscript(HistTitle);
            string ParticlesLC;
            string Region = particles.GetDRegionExplicit(HistTitle, true);

            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            if (ParticleShort == "e") {
                string ParticlesLC = particles.GetParticleNameLCFromSubscript(HistTitle, true);
                Histogram1D->SetTitle(("#chi^{2}_{" + ParticleShort + "} of " + ParticlesLC + "").c_str());
            } else {
                if ((ParticleShort != "p") || (ParticleShort != "D")) {
                    ParticlesLC = particles.GetParticleNameLCFromSubscript(HistTitle, false);
                } else {
                    ParticlesLC = particles.GetParticleNameLCFromSubscript(HistTitle, true);
                }

                Histogram1D->SetTitle(("#chi^{2}_{" + ParticleShort + "} of " + Region + " hadrons").c_str());

                if (findSubstring(Histogram1DNameCopy, "{D}") ||
                    findSubstring(Histogram1DNameCopy, "{K^{+}}") || findSubstring(Histogram1DNameCopy, "{K^{-}}")) {
                    auto FuncList = Histogram1D->GetListOfFunctions();
                    FuncList->Clear();
                } else {
                    auto ListOfFunctions = Histogram1D->GetListOfFunctions();
                    auto *Legend = (TLegend *) ListOfFunctions->At(5);
                    auto *FitParam = (TPaveText *) ListOfFunctions->At(6);

                    Legend->SetX1NDC(Legend->GetX1NDC() + xOffset), Legend->SetX2NDC(Legend->GetX2NDC() + xOffset);
                    Legend->SetY1NDC(Legend->GetY1NDC() + yOffset), Legend->SetY2NDC(Legend->GetY2NDC() + yOffset);
                    FitParam->SetX1NDC(FitParam->GetX1NDC() + xOffset), FitParam->SetX2NDC(FitParam->GetX2NDC() + xOffset);
                    FitParam->SetY1NDC(FitParam->GetY1NDC() + yOffset), FitParam->SetY2NDC(FitParam->GetY2NDC() + yOffset);
                }
            }

            Histogram1D->Draw(), gPad->Update();
        } else if ((Histogram1DNameCopy == "FD Proton momentum #epsilon_{eff} (1e cut)") ||
                   (Histogram1DNameCopy == "CD Proton momentum #epsilon_{eff} (1e cut)")) {
            ShowStats = false;

            string HistTitle = Histogram1D_Title1;
            string ParticlesLC = particles.GetParticleNameLC(HistTitle, true);
            string Region = particles.GetDRegionExplicit(HistTitle, true);

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            double LowerMomentumTh = 0.4;

            Histogram1D->GetYaxis()->SetRangeUser(0., 1.);
            Histogram1D->SetTitle(("Momentum efficiency of " + Region + " " + ParticlesLC + "").c_str());
            Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
            Histogram1D->Draw(), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, 1.);
            LowerMomTh->SetLineWidth(5);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + xOffset, Legend_y2_OneLine + yOffset);

            TLegendEntry *LowerMomThEntry;

            if (PresMode) {
                if (ExamPresMode) {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                    Histogram1D->GetXaxis()->SetTitle("P_{p} [GeV]");
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                    Histogram1D->GetXaxis()->SetTitle("P_{p} [GeV/c]");
                }
            } else {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                Histogram1D->GetXaxis()->SetTitle("P_{p} [GeV]");
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (Histogram1DNameCopy == "#theta_{p_{1},p_{2}} (All Int., 2p)") {
            Histogram1D->SetTitle("Opening angle between #font[12]{2p} protons");
            Histogram1D->Draw(), gPad->Update();
            Histogram1D->SetLineWidth(LineWidth);
            Histogram1D->SetLineColor(kBlack);
            Histogram1D->SetLineStyle(1);
        } else if (findSubstring(Histogram1DNameCopy, "#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN")) {
            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            double PeakLocation = -2.4, Cut = 15.;
            double UpperCut = Cut + PeakLocation, LowerCut = -Cut + PeakLocation;

            Histogram1D->SetTitle("#Delta#phi for small #Delta#theta_{pFD/pCD} in 1pFD1pCD");
            Histogram1D->GetXaxis()->SetTitle("#Delta#phi [#circ]");
            Histogram1D->Draw(), gPad->Update();

            TLine *PeakLoc = new TLine(PeakLocation, gPad->GetUymin(), PeakLocation, gPad->GetUymax());
            PeakLoc->SetLineWidth(5), PeakLoc->SetLineColor(kGreen), PeakLoc->Draw("same");

            TLine *UpperdPhiCut = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperdPhiCut->SetLineWidth(5), UpperdPhiCut->SetLineColor(kBlue), UpperdPhiCut->Draw("same");

            TLine *LowerdPhiCut = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerdPhiCut->SetLineWidth(5), LowerdPhiCut->SetLineColor(kRed), LowerdPhiCut->Draw("same");

            auto Legend = new TLegend(Legend_x1_ThreeLines + xOffset, Legend_y1_ThreeLines + yOffset, Legend_x2_ThreeLines - 0.1 + xOffset, Legend_y2_ThreeLines + yOffset);
            TLegendEntry *PeakLocEntry = Legend->AddEntry(PeakLoc, ("Peak location = " + to_string_with_precision(PeakLocation, 1) + "#circ").c_str(), "l");
            TLegendEntry *UpperdPhiCutEntry = Legend->AddEntry(UpperdPhiCut, ("Upper cut = " + to_string_with_precision(UpperCut, 1) + "#circ").c_str(), "l");
            TLegendEntry *LowerdPhiCutEntry = Legend->AddEntry(LowerdPhiCut, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + "#circ").c_str(), "l");
            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");

//            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "pFD", "#tilde{pFD}");
//            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "pCD", "#tilde{pCD}");

        } else if ((Histogram1DNameCopy == "FD Leading neutron momentum #epsilon_{eff} (1e cut)") ||
                   (Histogram1DNameCopy == "FD Neutron momentum #epsilon_{eff} (1e cut)")) {
            double UpperCut = beamE, LowerCut = 0.4;

            Histogram1D->GetYaxis()->SetRangeUser(0., 1.);
            Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
            Histogram1D->Draw(), gPad->Update();

            TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, 1.);
            UpperMomTh->SetLineWidth(5), UpperMomTh->SetLineColor(kBlue), UpperMomTh->Draw("same");

            TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, 1.);
            LowerMomTh->SetLineWidth(5), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines, Legend_y1_TwoLines, Legend_x2_TwoLines, Legend_y2_TwoLines);
            TLegendEntry *UpperMomThEntry = Legend->AddEntry(UpperMomTh, "Upper P_{n} th. = E_{beam}", "l");
            TLegendEntry *LowerMomThEntry;

            if (PresMode) {
                if (ExamPresMode) {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " [GeV/c]").c_str(), "l");
                }
            } else {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (((Histogram1DNameCopy == "Reco FD neutrons momentum (1e cut, FD)") || (Histogram1DNameCopy == "Reco FD neutron momentum - ZOOMOUT (1e cut, FD)")) ||
                   ((Histogram1DNameCopy == "FD neutron momentum BPID (1e_cut, FD)") || (Histogram1DNameCopy == "FD neutron momentum BPID - ZOOMOUT (1e_cut, FD)")) ||
                   ((Histogram1DNameCopy == "FD neutron momentum APID (1e_cut, FD)") || (Histogram1DNameCopy == "FD neutron momentum APID - ZOOMOUT (1e_cut, FD)"))) {
            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            double UpperCut = beamE, LowerCut = 0.4;

            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD Neutron momentum", "Momentum of forward-going neutrons");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD neutron momentum", "Momentum of forward-going neutrons");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P_{n} BPID", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "ZOOMOUT", "zoomout");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "  ", " ");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "   ", " ");

            Histogram1D->Draw(), gPad->Update();

            TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
            UpperMomTh->SetLineWidth(5), UpperMomTh->SetLineColor(kBlue), UpperMomTh->Draw("same");

            TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
            LowerMomTh->SetLineWidth(5), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines + xOffset - 0.135 + 0.05 - 0.015, Legend_y2_TwoLines +
            yOffset);
//            auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines + xOffset - 0.135, Legend_y2_TwoLines + yOffset);
            TLegendEntry *UpperMomThEntry = Legend->AddEntry(UpperMomTh, "Upper P_{n} th. = E_{beam}", "l");
            TLegendEntry *LowerMomThEntry;

            if (PresMode) {
                if (ExamPresMode) {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " [GeV/c]").c_str(), "l");
                }
            } else {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{n} th. = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram1DNameCopy, "Multiplicity of FD neutrons by redef.")) {
            ShowStats = false, LogScalePlot = true;

            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " APID&AV", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " by redef.", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD neutrons", "forward-going neutrons");

            Histogram1D->Draw(), gPad->Update();
        } else if ((Histogram1DNameCopy == "CD & FD Piplus momentum #epsilon_{eff} (1e cut)") || (Histogram1DNameCopy == "CD & FD Piminus momentum #epsilon_{eff} (1e cut)")) {
            ShowStats = false;
            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

            double LowerMomentumTh = 0.2;

            Histogram1D->GetYaxis()->SetRangeUser(0., 1.);
            Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
            Histogram1D->Draw(), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, 1.);
            LowerMomTh->SetLineWidth(5), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + xOffset, Legend_y2_OneLine + yOffset);

            if (Histogram1DNameCopy == "CD & FD Piplus momentum #epsilon_{eff} (1e cut)") {
                TLegendEntry *LowerMomThEntry;

                if (PresMode) {
                    if (ExamPresMode) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                    } else {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                    }
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                }
            } else {
                TLegendEntry *LowerMomThEntry;

                if (PresMode) {
                    if (ExamPresMode) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                    } else {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                    }
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV]").c_str(), "l");
                }
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram1DNameCopy, "used in AMaps")) {
            if (findSubstring(Histogram1DNameCopy, " P_{e} ")) {
                Histogram1D->Draw(), gPad->Update();
            } else if (findSubstring(Histogram1DNameCopy, " P_{pFD} ")) {
                ShowStats = false;

                double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
                double LowerCut = 0.4;

                Histogram1D->Draw(), gPad->Update();

                TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
                LowerMomTh->SetLineWidth(5), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

                auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine + xOffset - 0.15, Legend_y2_OneLine + yOffset);
                TLegendEntry *LowerMomThEntry;

                if (PresMode) {
                    if (ExamPresMode) {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
                    } else {
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + " [GeV/c]").c_str(), "l");
                    }
                } else {
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
                }

                Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
            } else if (findSubstring(Histogram1DNameCopy, " P_{nFD} ")) {
                ShowStats = false;

                double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
                double UpperCut = beamE, LowerCut = 0.4;

                Histogram1D->Draw(), gPad->Update();

                TLine *UpperMomTh = new TLine(UpperCut, gPad->GetUymin(), UpperCut, gPad->GetUymax());
                UpperMomTh->SetLineWidth(5), UpperMomTh->SetLineColor(kBlue), UpperMomTh->Draw("same");

                TLine *LowerMomTh = new TLine(LowerCut, gPad->GetUymin(), LowerCut, gPad->GetUymax());
                LowerMomTh->SetLineWidth(5), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

                auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines + xOffset - 0.15, Legend_y2_TwoLines + yOffset);
                TLegendEntry *UpperMomThEntry, *LowerMomThEntry;

                if (PresMode) {
                    if (ExamPresMode) {
                        UpperMomThEntry = Legend->AddEntry(UpperMomTh, ("Upper cut = " + to_string_with_precision(UpperCut, 3) + " [GeV]").c_str(), "l");
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
                    } else {
                        UpperMomThEntry = Legend->AddEntry(UpperMomTh, ("Upper cut = " + to_string_with_precision(UpperCut, 3) + " [GeV/c]").c_str(), "l");
                        LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + " [GeV/c]").c_str(), "l");
                    }
                } else {
                    UpperMomThEntry = Legend->AddEntry(UpperMomTh, ("Upper cut = " + to_string_with_precision(UpperCut, 3) + " [GeV]").c_str(), "l");
                    LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower cut = " + to_string_with_precision(LowerCut, 1) + " [GeV]").c_str(), "l");
                }

                Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
            }
        } else if (findSubstring(Histogram1DNameCopy, "#epsilon_{eff} (1p)") || findSubstring(Histogram1DNameCopy, "#epsilon_{eff} (1n)")) {
            double LowerMomentumKCut = 1.0, UpperMomentumKCut = 2.5, UpperThetaKCut = 32., upperLim = 1.;

            if (findSubstring(Histogram1DNameCopy, "momentum") &&
                !(findSubstring(Histogram1DNameCopy, "Electron") || findSubstring(Histogram1DNameCopy, "{e}}"))) {
                ShowStats = false;
                double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1pFD)", "");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1nFD)", "");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD Proton momentum", "P_{pFD}");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD Leading neutron momentum", "P_{nFD}");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P_{p}", "P_{pFD}");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P_{n}", "P_{nFD}");

                Histogram1D->GetYaxis()->SetRangeUser(0., upperLim);
                Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
                Histogram1D->SetLineColor(kBlack);
                Histogram1D->Draw(), gPad->Update();

                TLine *UpperMomKCut = new TLine(UpperMomentumKCut, gPad->GetUymin(), UpperMomentumKCut, upperLim);
                UpperMomKCut->SetLineWidth(5), UpperMomKCut->SetLineColor(kBlue), UpperMomKCut->Draw("same");

                TLine *LowerMomKCut = new TLine(LowerMomentumKCut, gPad->GetUymin(), LowerMomentumKCut, upperLim);
                LowerMomKCut->SetLineWidth(5), LowerMomKCut->SetLineColor(kRed), LowerMomKCut->Draw("same");

                auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines - 0.1 + 0.05 - 0.015 + xOffset, Legend_y2_TwoLines +
                yOffset);
//                auto Legend = new TLegend(Legend_x1_TwoLines + xOffset, Legend_y1_TwoLines + yOffset, Legend_x2_TwoLines - 0.1 + xOffset, Legend_y2_TwoLines + yOffset);
                TLegendEntry *LowerMomKCutEntry, *UpperMomKCutEntry;

                if (PresMode) {
                    if (ExamPresMode) {
                        LowerMomKCutEntry = Legend->AddEntry(LowerMomKCut, ("Lower cut = " + to_string_with_precision(LowerMomentumKCut, 1) + " [GeV]").c_str(), "l");
                        UpperMomKCutEntry = Legend->AddEntry(UpperMomKCut, ("Upper cut = " + to_string_with_precision(UpperMomentumKCut, 1) + " [GeV]").c_str(), "l");
                    } else {
                        LowerMomKCutEntry = Legend->AddEntry(LowerMomKCut, ("Lower cut = " + to_string_with_precision(LowerMomentumKCut, 1) + " [GeV/c]").c_str(), "l");
                        UpperMomKCutEntry = Legend->AddEntry(UpperMomKCut, ("Upper cut = " + to_string_with_precision(UpperMomentumKCut, 1) + " [GeV/c]").c_str(), "l");
                    }
                } else {
                    LowerMomKCutEntry = Legend->AddEntry(LowerMomKCut, ("Lower cut = " + to_string_with_precision(LowerMomentumKCut, 1) + " [GeV]").c_str(), "l");
                    UpperMomKCutEntry = Legend->AddEntry(UpperMomKCut, ("Upper cut = " + to_string_with_precision(UpperMomentumKCut, 1) + " [GeV]").c_str(), "l");
                }

                Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
            } else if (findSubstring(Histogram1DNameCopy, "theta") &&
                       !(findSubstring(Histogram1DNameCopy, "Electron") || findSubstring(Histogram1DNameCopy, "{e}}"))) {
                ShowStats = false;
                double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);

                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1pFD)", "");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1nFD)", "");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "#theta_{p}", "#theta_{pFD}");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "#theta_{n}", "#theta_{nFD}");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD Proton", "#theta_{pFD}");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "FD Leading neutron", "#theta_{nFD}");

                Histogram1D->GetYaxis()->SetRangeUser(0., upperLim);
                Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
                Histogram1D->SetLineColor(kBlack);
                Histogram1D->Draw(), gPad->Update();

                TLine *upperThetaKCut = new TLine(UpperThetaKCut, gPad->GetUymin(), UpperThetaKCut, upperLim);
                upperThetaKCut->SetLineWidth(5), upperThetaKCut->SetLineColor(kRed), upperThetaKCut->Draw("same");

                auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.05 + 0.05 - 0.015 + xOffset, Legend_y2_OneLine + yOffset);
//                auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.05 + xOffset, Legend_y2_OneLine + yOffset);
                TLegendEntry *UpperThetaKCutEntry = Legend->AddEntry(upperThetaKCut, ("Upper cut = " + to_string_with_precision(UpperThetaKCut, 0) + " [#circ]").c_str(), "l");
                Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
            } else if ((findSubstring(Histogram1DNameCopy, "Electron") || findSubstring(Histogram1DNameCopy, "{e}}"))) {
                double upperLim = 1.0;

                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1pFD)", "");
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "(1nFD)", "");

                Histogram1D->GetYaxis()->SetRangeUser(0., upperLim);
                Histogram1D->GetYaxis()->SetTitle("#epsilon_{eff}");
                Histogram1D->SetLineColor(kBlack);
                Histogram1D->Draw(), gPad->Update();
            }
        } else if (findSubstring(Histogram1DNameCopy, "Neutron resolution match multiplicity (1n, FD)")) {
            LogScalePlot = true;
            ShowStats = false;
            Histogram1D->GetXaxis()->SetRangeUser(1., 4.);
            Histogram1D->GetXaxis()->SetNdivisions(3);
            Histogram1D->Draw(), gPad->Update();
        } else if (findSubstring(Histogram1DNameCopy, "TL P_{nFD} used in nRes (1n, FD)")) {
            ShowStats = false;
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "TL P_{nFD} used in nRes", "Matched P^{truth}_{nFD} used in R_{nFD} calculation");

            if (PresMode) {
                if (ExamPresMode) {
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P_{nFD} [GeV]", "P^{truth}_{nFD} [GeV]");
                } else {
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P_{nFD} [GeV/c]", "P^{truth}_{nFD} [GeV/c]");
                }
            } else {
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "P_{nFD} [GeV]", "P^{truth}_{nFD} [GeV]");
            }

            Histogram1D->Draw(), gPad->Update();
        } else {
            Histogram1D->SetLineColor(kBlue);
            Histogram1D->SetStats(0);

            if (findSubstring(Histogram1DNameCopy, "FSRatio")) { Histogram1D->GetYaxis()->SetTitle(FSRyLabel.c_str()); }

            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "FD Neutron", "Forward-going neutron");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "FD Proton", "Forward-going proton");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "FD nucleon", "Forward-going nucleon");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         " P^{truth}_{n}", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         " P^{truth}_{p}", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         " P_{nFD}", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         " P_{pFD}", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         " P_{pCD}", "");

            if (PresMode) {
                if (ExamPresMode) {
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "Momentum [GeV]", "P^{truth}_{nucFD} [GeV]");
                } else {
                    TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "Momentum [GeV/c]", "P^{truth}_{nucFD} [GeV/c]");
                }
            } else {
                TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "Momentum [GeV]", "P^{truth}_{nucFD} [GeV]");
            }

            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         " AC", "");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}|", "P_{tot}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}|", "P_{tot}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2", "P_{rel}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2", "P_{rel}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "E_{cal} = E_{e} + T_{nFD} + T_{pCD}", "E_{cal}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "E_{cal} = E_{e} + T_{pFD} + T_{pCD}", "E_{cal}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "E_{cal} = E_{e} + T_{nucFD} + T_{nucCD}", "E_{cal}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD}", "E_{cal}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}|", "#deltaP_{T,tot}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}|", "P_{miss}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nucL} + #vec{P}_{nucR}- #vec{q}|", "P_{miss}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "#vec{P}_{tot}-#vec{q}", "P_{miss}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}} ", "W");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}} ", "W");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "#vec{P}", "#font[62]{P}");
            TitleAligner(particles, Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                         "#vec{q}", "#font[62]{q}");

            Histogram1D->Draw(), gPad->Update();
        }
    }

    if (!ShowStats) { Histogram1D->SetStats(0); }

    if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
        Histogram1D->GetYaxis()->SetRangeUser(0., 1.2);
        DrawPlot(HistogramCanvas, Histogram1D, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range1");

        Histogram1D->GetYaxis()->SetRangeUser(0., 2);
        DrawPlot(HistogramCanvas, Histogram1D, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range2");

        Histogram1D->GetYaxis()->SetRangeUser(0., 3);
        DrawPlot(HistogramCanvas, Histogram1D, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range3");

        Histogram1D->GetYaxis()->SetRangeUser(0., 4);
        DrawPlot(HistogramCanvas, Histogram1D, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range4");

        Histogram1D->GetYaxis()->SetRangeUser(0., 5);
        DrawPlot(HistogramCanvas, Histogram1D, LogScalePlot, LinearScalePlot, SavePath, SaveName, "Range5");
    } else {
        DrawPlot(HistogramCanvas, Histogram1D, LogScalePlot, LinearScalePlot, SavePath, SaveName, "");
    }

    HistogramCanvas->Clear();
    delete Histogram1D;
    delete file;
}
