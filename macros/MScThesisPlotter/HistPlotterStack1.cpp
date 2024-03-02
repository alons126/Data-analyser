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

#if IndependentStack1draw
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

void DrawPlot1(TCanvas *HistogramCanvas, const bool LogScalePlot, const bool LinearScalePlot, const string &SavePath, const string &SaveName,
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

void HistPlotterStack1(hData &particles, TCanvas *HistogramCanvas, TList *MScThesisPlotsList, const char *Sim_filename, const char *Data_filename, const char *Histogram1DName,
                       const string &SampleName, const string &SavePath, const string &SaveName, const bool TLmom = false) {
    cout << "\n\n";

    HistogramCanvas->Clear();

    THStack *Stack1D;

    TFile *Sim_file = TFile::Open(Sim_filename);
    TH1D *Sim_Histogram1D, *Sim_Histogram1D_QE, *Sim_Histogram1D_MEC, *Sim_Histogram1D_RES, *Sim_Histogram1D_DIS;

    TFile *Data_file = TFile::Open(Data_filename);
    TH1D *Data_Histogram1D;

    if (!findSubstring(Histogram1DName, "FSRatio")) {
        if (findSubstring(Histogram1DName, "W distribution") ||
            findSubstring(Histogram1DName, "E_{cal} reco.") ||
            findSubstring(Histogram1DName, "E_{e}") ||
            findSubstring(Histogram1DName, "#omega around #theta_{e} = 15#circ")) {
            if (findSubstring(Histogram1DName, "pFDpCD")) {
                string Histogram1DName0 = Histogram1DName;
                string Histogram1DName1 = Histogram1DName0.substr(0, Histogram1DName0.find_last_of('(') - 1);

                if (findSubstring(Histogram1DName, "W distribution")) {
                    Stack1D = new THStack("W distribution in 1pFD1pCD", "W distribution in 1pFD1pCD;W [GeV/c^{2}];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., pFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL only, pFDpCD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC only, pFDpCD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES only, pFDpCD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS only, pFDpCD)").c_str());
                } else if (findSubstring(Histogram1DName, "E_{cal} reco.")) {
                    Stack1D = new THStack("E_{cal} distribution in 1pFD1pCD", "E_{cal} distribution in 1pFD1pCD;E_{cal} [GeV];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., pFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL only, pFDpCD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC only, pFDpCD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES only, pFDpCD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS only, pFDpCD)").c_str());
                } else if (findSubstring(Histogram1DName, "E_{e}")) {
                    Stack1D = new THStack("E_{e} distribution in 1pFD1pCD", "E_{e} distribution in 1pFD1pCD;E_{e} [GeV];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., pFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL Only, pFDpCD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC Only, pFDpCD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES Only, pFDpCD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS Only, pFDpCD)").c_str());
                } else if (findSubstring(Histogram1DName, "#omega around #theta_{e} = 15#circ")) {
                    Stack1D = new THStack("#omega around #theta_{e} = 15#circ distribution in 1pFD1pCD",
                                          "#omega around #theta_{e} = 15#circ distribution in 1pFD1pCD;#omega [GeV];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., pFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL Only, pFDpCD, FD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC Only, pFDpCD, FD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES Only, pFDpCD, FD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS Only, pFDpCD, FD)").c_str());
                }

            } else if (findSubstring(Histogram1DName, "nFDpCD")) {
                string Histogram1DName0 = Histogram1DName;
                string Histogram1DName1 = Histogram1DName0.substr(0, Histogram1DName0.find_last_of('(') - 1);

                if (findSubstring(Histogram1DName, "W distribution")) {
                    Stack1D = new THStack("W distribution in 1nFD1pCD", "W distribution in 1nFD1pCD;W [GeV/c^{2}];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., nFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL only, nFDpCD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC only, nFDpCD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES only, nFDpCD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS only, nFDpCD)").c_str());
                } else if (findSubstring(Histogram1DName, "E_{cal} reco.")) {
                    Stack1D = new THStack("E_{cal} distribution in 1nFD1pCD", "E_{cal} distribution in 1nFD1pCD;E_{cal} [GeV];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., nFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL only, nFDpCD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC only, nFDpCD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES only, nFDpCD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS only, nFDpCD)").c_str());
                } else if (findSubstring(Histogram1DName, "E_{e}")) {
                    Stack1D = new THStack("E_{e} distribution in 1nFD1pCD", "E_{e} distribution in 1nFD1pCD;E_{e} [GeV];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., nFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL Only, nFDpCD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC Only, nFDpCD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES Only, nFDpCD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS Only, nFDpCD)").c_str());
                } else if (findSubstring(Histogram1DName, "#omega around #theta_{e} = 15#circ")) {
                    Stack1D = new THStack("#omega around #theta_{e} = 15#circ distribution in 1nFD1pCD",
                                          "#omega around #theta_{e} = 15#circ distribution in 1nFD1pCD;#omega [GeV];");
                    Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., nFDpCD)").c_str());
                    Sim_Histogram1D_QE = Histofinder1D(Sim_file, (Histogram1DName1 + " (QEL Only, nFDpCD, FD)").c_str());
                    Sim_Histogram1D_MEC = Histofinder1D(Sim_file, (Histogram1DName1 + " (MEC Only, nFDpCD, FD)").c_str());
                    Sim_Histogram1D_RES = Histofinder1D(Sim_file, (Histogram1DName1 + " (RES Only, nFDpCD, FD)").c_str());
                    Sim_Histogram1D_DIS = Histofinder1D(Sim_file, (Histogram1DName1 + " (DIS Only, nFDpCD, FD)").c_str());
                }

            }

            Data_Histogram1D = Histofinder1D(Data_file, Histogram1DName);
        }
    } else {
        string Histogram1DName1 = Histogram1DName;

        Sim_Histogram1D = Histofinder1D(Sim_file, (Histogram1DName1 + " (All Int., ").c_str(), TLmom);
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
    int LineWidth = 3;
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

//    Stack1D->GetXaxis()->SetTitleSize(0.06);
//    Stack1D->GetXaxis()->SetLabelSize(0.0425);
//    Stack1D->GetXaxis()->CenterTitle(true);
//    Stack1D->GetYaxis()->SetTitle("Number of events");
//    Stack1D->GetYaxis()->SetTitleSize(0.06);
//    Stack1D->GetYaxis()->SetLabelSize(0.0425);
//    Stack1D->GetYaxis()->CenterTitle(true);
    MScThesisPlotsList->Add(Stack1D);

    Sim_Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Sim_Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D->GetXaxis()->CenterTitle(true);
    Sim_Histogram1D->GetYaxis()->SetTitle("Number of events");
    Sim_Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Sim_Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D->GetYaxis()->CenterTitle(true);
    Sim_Histogram1D->SetLineWidth(LineWidth);
    Sim_Histogram1D->SetLineStyle(0);
    Sim_Histogram1D->SetLineColor(kBlue);
    Sim_Histogram1D->SetStats(0);
    MScThesisPlotsList->Add(Sim_Histogram1D);
    double Sim_Scale = Sim_Histogram1D->Integral();
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) { Sim_Histogram1D->Scale(Data_Histogram1D->Integral() / Sim_Scale); }
    Stack1D->Add(Sim_Histogram1D);

    Sim_Histogram1D_QE->GetXaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_QE->GetXaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_QE->GetXaxis()->CenterTitle(true);
    Sim_Histogram1D_QE->GetYaxis()->SetTitle("Number of events");
    Sim_Histogram1D_QE->GetYaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_QE->GetYaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_QE->GetYaxis()->CenterTitle(true);
    Sim_Histogram1D_QE->SetLineWidth(LineWidth);
    Sim_Histogram1D_QE->SetLineStyle(7);
    Sim_Histogram1D_QE->SetLineColor(kAzure + 10);
    Sim_Histogram1D_QE->SetStats(0);
    MScThesisPlotsList->Add(Sim_Histogram1D_QE);
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) { Sim_Histogram1D_QE->Scale(Data_Histogram1D->Integral() / Sim_Scale); }
    Stack1D->Add(Sim_Histogram1D_QE);

    Sim_Histogram1D_MEC->GetXaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_MEC->GetXaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_MEC->GetXaxis()->CenterTitle(true);
    Sim_Histogram1D_MEC->GetYaxis()->SetTitle("Number of events");
    Sim_Histogram1D_MEC->GetYaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_MEC->GetYaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_MEC->GetYaxis()->CenterTitle(true);
    Sim_Histogram1D_MEC->SetLineWidth(LineWidth);
    Sim_Histogram1D_MEC->SetLineStyle(7);
    Sim_Histogram1D_MEC->SetLineColor(kViolet);
    Sim_Histogram1D_MEC->SetStats(0);
    MScThesisPlotsList->Add(Sim_Histogram1D_MEC);
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) { Sim_Histogram1D_MEC->Scale(Data_Histogram1D->Integral() / Sim_Scale); }
    Stack1D->Add(Sim_Histogram1D_MEC);

    Sim_Histogram1D_RES->GetXaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_RES->GetXaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_RES->GetXaxis()->CenterTitle(true);
    Sim_Histogram1D_RES->GetYaxis()->SetTitle("Number of events");
    Sim_Histogram1D_RES->GetYaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_RES->GetYaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_RES->GetYaxis()->CenterTitle(true);
    Sim_Histogram1D_RES->SetLineWidth(LineWidth);
    Sim_Histogram1D_RES->SetLineStyle(7);
    Sim_Histogram1D_RES->SetLineColor(kGreen + 1);
    Sim_Histogram1D_RES->SetStats(0);
    MScThesisPlotsList->Add(Sim_Histogram1D_RES);
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) { Sim_Histogram1D_RES->Scale(Data_Histogram1D->Integral() / Sim_Scale); }
    Stack1D->Add(Sim_Histogram1D_RES);

    Sim_Histogram1D_DIS->GetXaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_DIS->GetXaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_DIS->GetXaxis()->CenterTitle(true);
    Sim_Histogram1D_DIS->GetYaxis()->SetTitle("Number of events");
    Sim_Histogram1D_DIS->GetYaxis()->SetTitleSize(0.06);
    Sim_Histogram1D_DIS->GetYaxis()->SetLabelSize(0.0425);
    Sim_Histogram1D_DIS->GetYaxis()->CenterTitle(true);
    Sim_Histogram1D_DIS->SetLineWidth(LineWidth);
    Sim_Histogram1D_DIS->SetLineStyle(7);
    Sim_Histogram1D_DIS->SetLineColor(kOrange + 7);
    Sim_Histogram1D_DIS->SetStats(0);
    MScThesisPlotsList->Add(Sim_Histogram1D_DIS);
    if (!findSubstring(Histogram1DNameCopy, "FSRatio")) { Sim_Histogram1D_DIS->Scale(Data_Histogram1D->Integral() / Sim_Scale); }
    Stack1D->Add(Sim_Histogram1D_DIS);

    Data_Histogram1D->GetXaxis()->SetTitleSize(0.06);
    Data_Histogram1D->GetXaxis()->SetLabelSize(0.0425);
    Data_Histogram1D->GetXaxis()->CenterTitle(true);
    Data_Histogram1D->GetYaxis()->SetTitle("Number of events");
    Data_Histogram1D->GetYaxis()->SetTitleSize(0.06);
    Data_Histogram1D->GetYaxis()->SetLabelSize(0.0425);
    Data_Histogram1D->GetYaxis()->CenterTitle(true);
    Data_Histogram1D->SetLineWidth(LineWidth + 1);
    Data_Histogram1D->SetLineStyle(0);
    Data_Histogram1D->SetLineColor(kRed);
    Data_Histogram1D->SetStats(0);
    MScThesisPlotsList->Add(Data_Histogram1D);
    Stack1D->Add(Data_Histogram1D);

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
            cout << "Title = " << Histogram1D_Title << "\n";
            cout << "Sim peak = " << Sim_Histogram1D->GetBinCenter(Sim_Histogram1D->GetMaximumBin()) << "\n";
            cout << "Data peak = " << Data_Histogram1D->GetBinCenter(Data_Histogram1D->GetMaximumBin()) << "\n";
        }

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
            string RatioVar = Histogram1D_xLabel.substr(0, Histogram1D_xLabel.find_last_of('[') - 1);
            string RatioTopology;

            if (findSubstring(Histogram1D_Title, "1n/1p")) {
                RatioTopology = "1N";
            } else if (findSubstring(Histogram1D_Title, "nFDpCD/pFDpCD")) {
                RatioTopology = "2N";
            }

            FSRyLabel = "r^{" + RatioVar + "}_{" + RatioTopology + "}";
        }

//        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "pFDpCD", "1pFD1pCD");
//        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "nFDpCD", "1nFD1pCD");
//        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1p", "1pFD");
//        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1n", "1nFD");
//        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "pFDpCD", "1pFD1pCD");
//        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "nFDpCD", "1nFD1pCD");
//        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1p", "1pFD");
//        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, "1n", "1nFD");

        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (All Int., nFDpCD)", " in 1nFD1pCD");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel, " (All Int., pFDpCD)", " in 1pFD1pCD");

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) {
            Sim_Histogram1D->GetYaxis()->SetTitle(FSRyLabel.c_str());
            Data_Histogram1D->GetYaxis()->SetTitle(FSRyLabel.c_str());
        }

        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}|", "P_{tot}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}|", "P_{tot}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2", "P_{rel}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2", "P_{rel}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{nFD} + T_{pCD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{pFD} + T_{pCD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{nucFD} + T_{nucCD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD}", "E_{cal}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nucL} + #vec{P}_{nucR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#vec{P}_{tot}-#vec{q}", "P_{miss}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}} ", "W");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}} ", "W");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#vec{P}", "#font[62]{P}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#vec{q}", "#font[62]{q}");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "W [GeV]", "W [GeV/c^{2}]");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "P_{nucCD} [GeV/c]", "P_{pCD} [GeV/c]");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{nucFD,nucCD} [Deg]", "#theta_{nucFD,pCD} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{#font[62]{q},#font[62]{P}_{nuc,FD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{nucFD}} [Deg]");
        TitleAligner(particles, Sim_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{#font[62]{q},#font[62]{P}_{nuc,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{#font[62]{q},#font[62]{P}_{p,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}|", "P_{tot}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}|", "P_{tot}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2", "P_{rel}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2", "P_{rel}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{nFD} + T_{pCD}", "E_{cal}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{pFD} + T_{pCD}", "E_{cal}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{nucFD} + T_{nucCD}", "E_{cal}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD}", "E_{cal}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nucFD} + #vec{p}_{T,nucCD}|", "#deltaP_{T,tot}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nucL} + #vec{P}_{nucR}- #vec{q}|", "P_{miss}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#vec{P}_{tot}-#vec{q}", "P_{miss}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}} ", "W");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}} ", "W");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#vec{P}", "#font[62]{P}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#vec{q}", "#font[62]{q}");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "W [GeV]", "W [GeV/c^{2}]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "P_{nucCD} [GeV/c]", "P_{pCD} [GeV/c]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{nucFD,nucCD} [Deg]", "#theta_{nucFD,pCD} [Deg]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{#font[62]{q},#font[62]{P}_{nuc,FD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{nucFD}} [Deg]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{#font[62]{q},#font[62]{P}_{nuc,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");
        TitleAligner(particles, Data_Histogram1D, Histogram1D_Title, Histogram1D_xLabel,
                     "#theta_{#font[62]{q},#font[62]{P}_{p,CD}} [Deg]", "#theta_{#font[62]{q},#font[62]{P}_{pCD}} [Deg]");

////        Stack1D->Draw("nostack");
//        Sim_Histogram1D->Draw();
//        Sim_Histogram1D_QE->Draw("same"), Sim_Histogram1D_MEC->Draw("same"), Sim_Histogram1D_RES->Draw("same"), Sim_Histogram1D_DIS->Draw("same");
//        Data_Histogram1D->Draw("same"), gPad->Update();
        Sim_Histogram1D->Draw(), Data_Histogram1D->Draw("same"), Sim_Histogram1D->Draw("same");
        Sim_Histogram1D_QE->Draw("same"), Sim_Histogram1D_MEC->Draw("same"), Sim_Histogram1D_RES->Draw("same"), Sim_Histogram1D_DIS->Draw("same");
        gPad->Update();

        double xOffset = SetxOffset1D(false), yOffset = SetyOffset1D(false);

        TLegend *Comparison_legend;

        double Custom_x1Offset = 0;

        if (findSubstring(Histogram1DNameCopy, "FSRatio")) { Custom_x1Offset = -0.085; }

        if (findSubstring(Histogram1D_Title, "W ") || findSubstring(Histogram1D_Title, "E_{cal} reco.")) {
            double Custom_xOffset = -0.41;

            Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_xOffset + Custom_x1Offset - 0.025, Legend_y1_TwoLines + yOffset,
                                            Legend_x2_TwoLines - 0.05 + xOffset + Custom_xOffset, Legend_y2_TwoLines + yOffset - 0.15);
        } else {
            Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_x1Offset - Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                            Legend_x2_TwoLines - 0.05 + xOffset - Custom_x1Offset, Legend_y2_TwoLines + yOffset);
        }

        /*
        if (!findSubstring(Histogram1D_Title, "W ")) {
            Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_x1Offset - Custom_x1Offset, Legend_y1_TwoLines + yOffset,
                                            Legend_x2_TwoLines - 0.05 + xOffset - Custom_x1Offset, Legend_y2_TwoLines + yOffset);
        } else {
            double Custom_xOffset = -0.41;

            Comparison_legend = new TLegend(Legend_x1_TwoLines + xOffset + Custom_xOffset, Legend_y1_TwoLines + yOffset,
                                            Legend_x2_TwoLines - 0.05 + xOffset + Custom_xOffset, Legend_y2_TwoLines + yOffset);
        }
*/

        if (!findSubstring(Histogram1DNameCopy, "FSRatio")) {
            TLegendEntry *Sim_Entry = Comparison_legend->AddEntry(Sim_Histogram1D, "All int. (simulation; scaled)", "l");
            TLegendEntry *Sim_Entry_QE = Comparison_legend->AddEntry(Sim_Histogram1D_QE, "QE (simulation; scaled)", "l");
            TLegendEntry *Sim_Entry_MEC = Comparison_legend->AddEntry(Sim_Histogram1D_MEC, "MEC (simulation; scaled)", "l");
            TLegendEntry *Sim_Entry_RES = Comparison_legend->AddEntry(Sim_Histogram1D_RES, "RES (simulation; scaled)", "l");
            TLegendEntry *Sim_Entry_DIS = Comparison_legend->AddEntry(Sim_Histogram1D_DIS, "DIS (simulation; scaled)", "l");
            TLegendEntry *Data_Entry = Comparison_legend->AddEntry(Data_Histogram1D, "Data", "l");
        } else {
            TLegendEntry *Sim_Entry = Comparison_legend->AddEntry(Sim_Histogram1D, "Simulation", "l");
            TLegendEntry *Data_Entry = Comparison_legend->AddEntry(Data_Histogram1D, "Data", "l");
        }

        Comparison_legend->SetTextSize(0.02), Comparison_legend->SetTextAlign(12), Comparison_legend->Draw("same");
//        Comparison_legend->SetTextSize(0.03), Comparison_legend->SetTextAlign(12), Comparison_legend->Draw("same");
    }

    DrawPlot1(HistogramCanvas, LogScalePlot, LinearScalePlot, SavePath, SaveName, "");

    HistogramCanvas->Clear();
    delete Sim_file;
    delete Data_file;
}
