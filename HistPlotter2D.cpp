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

void HistPlotter2D(TCanvas *HistogramCanvas, TList *MScThesisPlotsList, const char *filename, const char *Histogram2DName,
                   const string &SampleName, const string &SavePath, const string &SaveName) {
    cout << "\n\n";

    system(("mkdir -p " + SavePath).c_str());

    HistogramCanvas->Clear(); // Clear previous plot

    TFile *file = new TFile(filename);
    TH2D *Histogram2D = (TH2D *) file->Get(Histogram2DName);

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
        Histogram2D->Draw("colz");

        gPad->Update();

        if (findSubstring(Histogram2DNameCopy,"dc_hitmap")) {
            ShowStats = false;

            Histogram2D->GetXaxis()->SetTitle("x [cm]");
            Histogram2D->GetYaxis()->SetTitle("y [cm]");
        }

        if (findSubstring(Histogram2DNameCopy,"#beta vs. P")) {
            if ((Histogram2DNameCopy == "#beta vs. P (all particles, no #(e) cut, CD)") ||
                (Histogram2DNameCopy == "#beta vs. P (all particles, no #(e) cut, FD)") ||
                (Histogram2DNameCopy == "#beta vs. P (all particles, 1e cut, CD)") ||
                (Histogram2DNameCopy == "#beta vs. P (all particles, 1e cut, FD)")) {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

                TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
                EquiLine->SetLineWidth(2);
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
        }

        if (Histogram2DNameCopy == "SF vs. P_{e} BC (1e cut, FD)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *UpperFScut = new TLine(gPad->GetUxmin(), 0.28, gPad->GetUxmax(), 0.28);
            UpperFScut->SetLineWidth(2);
            UpperFScut->SetLineColor(kRed);
            UpperFScut->Draw("same");

            TLine *LowerFScut = new TLine(gPad->GetUxmin(), 0.2, gPad->GetUxmax(), 0.2);
            LowerFScut->SetLineWidth(2);
            LowerFScut->SetLineColor(kRed);
            LowerFScut->Draw("same");
        }

        if (Histogram2DNameCopy == "Vcal vs. SF BC (1e cut, PCAL)" || Histogram2DNameCopy == "Wcal vs. SF BC (1e cut, PCAL)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *LowerECALcoorCut = new TLine(14., gPad->GetUymin(), 14., gPad->GetUymax());
            LowerECALcoorCut->SetLineWidth(2);
            LowerECALcoorCut->SetLineColor(kRed);
            LowerECALcoorCut->Draw("same");
        }

        if (Histogram2DNameCopy == "#beta vs. P (electrons only, 1e cut)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
            EquiLine->SetLineWidth(2);
            EquiLine->SetLineColor(kBlack);
            EquiLine->Draw("same");

            TLine *UpperBetaElectronCut = new TLine(gPad->GetUxmin(), 1.2, gPad->GetUxmax(), 1.2);
            UpperBetaElectronCut->SetLineWidth(2);
            UpperBetaElectronCut->SetLineColor(kRed);
            UpperBetaElectronCut->Draw("same");
        }

        if (Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)" ||
                   Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TGraph *ClusterCenter = new TGraph();
            ClusterCenter->AddPoint(40., 40.);
            ClusterCenter->Draw("p");
            ClusterCenter->SetMarkerStyle(22);
            ClusterCenter->SetMarkerColor(kMagenta);
            ClusterCenter->SetMarkerSize(3);

            TLine *UpperThetapFDcut = new TLine(gPad->GetUxmin(), 45., gPad->GetUxmax(), 45.);
            UpperThetapFDcut->SetLineWidth(2);
            UpperThetapFDcut->SetLineColor(kBlue);
            UpperThetapFDcut->Draw("same");

            TLine *LowerThetapFDcut = new TLine(gPad->GetUxmin(), 35., gPad->GetUxmax(), 35.);
            LowerThetapFDcut->SetLineWidth(2);
            LowerThetapFDcut->SetLineColor(kBlue);
            LowerThetapFDcut->Draw("same");

            TLine *UpperThetapCDcut = new TLine(45., gPad->GetUymin(), 45., gPad->GetUymax());
            UpperThetapCDcut->SetLineWidth(2);
            UpperThetapCDcut->SetLineColor(kRed);
            UpperThetapCDcut->Draw("same");

            TLine *LowerThetapCDcut = new TLine(35., gPad->GetUymin(), 35., gPad->GetUymax());
            LowerThetapCDcut->SetLineWidth(2);
            LowerThetapCDcut->SetLineColor(kRed);
            LowerThetapCDcut->Draw("same");
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
}
