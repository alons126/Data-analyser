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
                   const string &SampleName, const string &SavePath, const string &SaveName) {
    cout << "\n\n";

    system(("mkdir -p " + SavePath).c_str());

    HistogramCanvas->Clear(); // Clear previous plot

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nHistPlotter2D: could not load root file! Exiting...\n", exit(0); }

    TH2D *Histogram2D;

    if (file->Get(Histogram2DName) == nullptr ||
        (findSubstring(Histogram2DName, "vs") || findSubstring(Histogram2DName, "VS"))) {
        Histogram2D = Histofinder2D(filename, Histogram2DName, false);
    } else {
        Histogram2D = (TH2D *) file->Get(Histogram2DName);
    }

    /*
    TH2D *Histogram2D = (TH2D *) file->Get(Histogram2DName);
*/

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

        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1p", "1pFD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1n", "1nFD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "pFDpCD", "1pFD1pCD");
        TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "nFDpCD", "1nFD1pCD");

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
                EquiLine->SetLineWidth(3);
                EquiLine->SetLineColor(kBlack);
                EquiLine->Draw("same");

//            auto *beta_neutron = new TF1("beta_neutron", ("x/sqrt(x*x + " + to_string(m_n * m_n) + ")").c_str(), 0, beamE);
//            beta_neutron->Draw("same");
//            beta_neutron->SetLineColor(2);
//
//            auto *beta_proton = new TF1("beta_proton", ("x/sqrt(x*x + " + to_string(m_p * m_p) + ")").c_str(), 0, beamE);
//            beta_proton->Draw("same");
//            beta_proton->SetLineColor(3);
//
//            auto *beta_Kplus = new TF1("beta_Kplus", ("x/sqrt(x*x + " + to_string(m_Kplus * m_Kplus) + ")").c_str(), 0, beamE);
//            beta_Kplus->Draw("same");
//            beta_Kplus->SetLineColor(4);
//
//            auto *beta_Kminus = new TF1("beta_Kminus", ("x/sqrt(x*x + " + to_string(m_Kminus * m_Kminus) + ")").c_str(), 0, beamE);
//            beta_Kminus->Draw("same");
//            beta_Kminus->SetLineColor(5);
//
//            auto *beta_piplus = new TF1("beta_piplus", ("x/sqrt(x*x + " + to_string(m_piplus * m_piplus) + ")").c_str(), 0, beamE);
//            beta_piplus->Draw("same");
//            beta_piplus->SetLineColor(6);
//
//            auto *beta_piminus = new TF1("beta_piminus", ("x/sqrt(x*x + " + to_string(m_piminus * m_piminus) + ")").c_str(), 0, beamE);
//            beta_piminus->Draw("same");
//            beta_piminus->SetLineColor(7);
//
//            auto *beta_electron = new TF1("beta_electron", ("x/sqrt(x*x + " + to_string(m_e * m_e) + ")").c_str(), 0, beamE);
//            beta_electron->Draw("same");
//            beta_electron->SetLineColor(8);

            }
        } else if ((Histogram2DNameCopy == "SF vs. P_{e} BC (1e cut, FD)") || (Histogram2DNameCopy == "SF vs. P_{e} (1e cut, FD)")) {
            Histogram2D->SetTitle("Electron sampling fraction vs. momentum in ^{12}C(e,e')");
            Histogram2D->GetYaxis()->SetTitle("f_{e}");
            Histogram2D->Draw("colz"), gPad->Update();

            TLine *UpperFScut = new TLine(gPad->GetUxmin(), 0.28, gPad->GetUxmax(), 0.28);
            UpperFScut->SetLineWidth(3);
            UpperFScut->SetLineColor(kRed);
            UpperFScut->Draw("same");

            TLine *LowerFScut = new TLine(gPad->GetUxmin(), 0.2, gPad->GetUxmax(), 0.2);
            LowerFScut->SetLineWidth(3);
            LowerFScut->SetLineColor(kRed);
            LowerFScut->Draw("same");
        } else if ((Histogram2DNameCopy == "Vcal vs. SF BC (1e cut, PCAL)" || Histogram2DNameCopy == "Wcal vs. SF BC (1e cut, PCAL)") ||
                   (Histogram2DNameCopy == "Vcal vs. SF (1e cut, PCAL)" || Histogram2DNameCopy == "Wcal vs. SF (1e cut, PCAL)")) {
            if ((Histogram2DNameCopy == "Vcal vs. SF BC (1e cut, PCAL)") || (Histogram2DNameCopy == "Vcal vs. SF (1e cut, PCAL)")) {
                Histogram2D->SetTitle("PCAL #font[12]{l}_{V} coordinate vs. SF in ^{12}C(e,e')");
                Histogram2D->GetXaxis()->SetTitle("#font[12]{l}_{V} [cm]");
            } else if ((Histogram2DNameCopy == "Wcal vs. SF BC (1e cut, PCAL)") || (Histogram2DNameCopy == "Wcal vs. SF (1e cut, PCAL)")) {
                Histogram2D->SetTitle("PCAL #font[12]{l}_{W} coordinate vs. SF in ^{12}C(e,e')");
                Histogram2D->GetXaxis()->SetTitle("#font[12]{l}_{W} [cm]");
            }

            Histogram2D->GetYaxis()->SetTitle("f_{e}");
            Histogram2D->Draw("colz"), gPad->Update();
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *LowerECALcoorCut = new TLine(14., gPad->GetUymin(), 14., gPad->GetUymax());
            LowerECALcoorCut->SetLineWidth(3);
            LowerECALcoorCut->SetLineColor(kRed);
            LowerECALcoorCut->Draw("same");
        } else if (Histogram2DNameCopy == "#beta vs. P (electrons only, 1e cut)") {
            Histogram2D->SetTitle("Electron #beta_{e} vs. momentum in ^{12}C(e,e')");
            Histogram2D->GetXaxis()->SetTitle("P_{e} [GeV/c]");
            Histogram2D->GetYaxis()->SetTitle("#beta_{e}");
            Histogram2D->Draw("colz"), gPad->Update();
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
            EquiLine->SetLineWidth(3);
            EquiLine->SetLineColor(kBlack);
            EquiLine->Draw("same");

            TLine *UpperBetaElectronCut = new TLine(gPad->GetUxmin(), 1.2, gPad->GetUxmax(), 1.2);
            UpperBetaElectronCut->SetLineWidth(3);
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
                Histogram2D->SetTitle("#theta_{pFD} vs. #theta_{pCD} for every #theta_{pFD,pCD} in 1pFD1pCD");
            } else if (Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)") {
                Histogram2D->SetTitle("#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD} < 20#circ in 1pFD1pCD");
            }

            Histogram2D->Draw("colz"), gPad->Update();

            TLine *UpperThetapFDcut = new TLine(gPad->GetUxmin(), 45., gPad->GetUxmax(), 45.);
            UpperThetapFDcut->SetLineWidth(3), UpperThetapFDcut->SetLineColor(kGreen), UpperThetapFDcut->Draw("same");

            TLine *LowerThetapFDcut = new TLine(gPad->GetUxmin(), 35., gPad->GetUxmax(), 35.);
            LowerThetapFDcut->SetLineWidth(3), LowerThetapFDcut->SetLineColor(kGreen), LowerThetapFDcut->Draw("same");

            TLine *UpperThetapCDcut = new TLine(45., gPad->GetUymin(), 45., gPad->GetUymax());
            UpperThetapCDcut->SetLineWidth(3), UpperThetapCDcut->SetLineColor(kRed), UpperThetapCDcut->Draw("same");

            TLine *LowerThetapCDcut = new TLine(35., gPad->GetUymin(), 35., gPad->GetUymax());
            LowerThetapCDcut->SetLineWidth(3), LowerThetapCDcut->SetLineColor(kRed), LowerThetapCDcut->Draw("same");
        } else if (findSubstring(Histogram2DNameCopy, "P_{p} vs. #theta_{p}")) {
            ShowStats = false;

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            double LowerMomentumTh = 0.4;

            if (findSubstring(Histogram2DNameCopy, "FD")) {
                Histogram2D->SetTitle("Forward-going proton momentum vs. scattering angle in ^{12}C(e,e')");
            } else if (findSubstring(Histogram2DNameCopy, "CD")) {
                Histogram2D->SetTitle("Central-going proton momentum vs. scattering angle in ^{12}C(e,e')");
            }

            Histogram2D->Draw("colz"), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, gPad->GetUymax());
            LowerMomTh->SetLineWidth(3);
            LowerMomTh->SetLineColor(kRed);
            LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + xOffset, Legend_y2_OneLine + yOffset);

            TLegendEntry *LowerMomThEntry;

            LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{p} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
            Histogram2D->GetXaxis()->SetTitle("P_{p} [GeV/c]");

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else if (findSubstring(Histogram2DNameCopy, "P_{#pi^{+}} vs. #theta_{#pi^{+}}") ||
                   findSubstring(Histogram2DNameCopy, "P_{#pi^{-}} vs. #theta_{#pi^{-}}")) {
            ShowStats = false;

            double xOffset = SetxOffset1D(ShowStats), yOffset = SetyOffset1D(ShowStats);
            double LowerMomentumTh = 0.2;

            if (findSubstring(Histogram2DNameCopy, "P_{#pi^{+}} vs. #theta_{#pi^{+}}")) {
                if (findSubstring(Histogram2DNameCopy, "FD")) {
                    Histogram2D->SetTitle("Forward-going #pi^{+} momentum vs. scattering angle in ^{12}C(e,e')");
                } else if (findSubstring(Histogram2DNameCopy, "CD")) {
                    Histogram2D->SetTitle("Central-going #pi^{+} momentum vs. scattering angle in ^{12}C(e,e')");
                }
            } else if (findSubstring(Histogram2DNameCopy, "P_{#pi^{-}} vs. #theta_{#pi^{-}}")) {
                if (findSubstring(Histogram2DNameCopy, "FD")) {
                    Histogram2D->SetTitle("Forward-going #pi^{-} momentum vs. scattering angle in ^{12}C(e,e')");
                } else if (findSubstring(Histogram2DNameCopy, "CD")) {
                    Histogram2D->SetTitle("Central-going #pi^{-} momentum vs. scattering angle in ^{12}C(e,e')");
                }
            }

            Histogram2D->Draw("colz"), gPad->Update();

            TLine *LowerMomTh = new TLine(LowerMomentumTh, gPad->GetUymin(), LowerMomentumTh, gPad->GetUymax());
            LowerMomTh->SetLineWidth(3), LowerMomTh->SetLineColor(kRed), LowerMomTh->Draw("same");

            auto Legend = new TLegend(Legend_x1_OneLine + xOffset, Legend_y1_OneLine + yOffset, Legend_x2_OneLine - 0.15 + xOffset, Legend_y2_OneLine + yOffset);

            TLegendEntry *LowerMomThEntry;

            if (findSubstring(Histogram2DNameCopy, "#pi^{+}")) {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{+}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                Histogram2D->GetXaxis()->SetTitle("P_{#pi^{+}} [GeV/c]");
            } else if (findSubstring(Histogram2DNameCopy, "#pi^{-}")) {
                LowerMomThEntry = Legend->AddEntry(LowerMomTh, ("Lower P_{#pi^{-}} th. = " + to_string_with_precision(LowerMomentumTh, 1) + " [GeV/c]").c_str(), "l");
                Histogram2D->GetXaxis()->SetTitle("P_{#pi^{-}} [GeV/c]");
            }

            Legend->SetTextSize(0.03), Legend->SetTextAlign(12), Legend->Draw("same");
        } else {
//            string Histogram2D_Title = Histogram2D->GetTitle();
//            string Histogram2D_xLabel = Histogram2D->GetXaxis()->GetTitle();
//            string Histogram2D_yLabel = Histogram2D->GetYaxis()->GetTitle();

            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}|", "P_{tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}|", "P_{tot}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2", "P_{rel}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2", "P_{rel}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "E_{cal} = E_{e} + T_{nFD} + T_{pCD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "E_{cal} = E_{e} + T_{pFD} + T_{pCD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "E_{cal} = E_{e} + T_{nucFD} + T_{nucCD}", "E_{cal}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "E_{cal} = E_{e} + T_{nuc,FD} + T_{nuc,CD}", "E_{cal}");
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
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "W = #sqrt{(#omega + m_{nuc})^{2} - #vec{q}^{2}} ", "W");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}} ", "W");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}|", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}|", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel,
                         "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nucL} + #vec{P}_{nucR}- #vec{q}|", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "#vec{P}_{tot}-#vec{q}", "P_{miss}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "#vec{P}", "#font[62]{P}");
            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "#vec{q}", "#font[62]{q}");

//            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1p", "1pFD");
//            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "1n", "1nFD");
//            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "pFDpCD", "1pFD1pCD");
//            TitleAligner(Histogram2D, Histogram2D_Title, Histogram2D_xLabel, Histogram2D_yLabel, "nFDpCD", "1nFD1pCD");

            Histogram2D->Draw("colz"), gPad->Update();
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
}
