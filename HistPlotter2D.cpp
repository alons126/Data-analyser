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

const string ConfigRegion(const string &Histogram2DNameCopy) {
    if (findSubstring(Histogram2DNameCopy, "CD"))
        return "CD";
    else {
        return "FD";
    }
}

const string ConfigSName(const string &SampleName) {
    if (findSubstring(SampleName, "sim")) {
        return "s";
    } else if (findSubstring(SampleName, "data")) {
        return "d";
    }
}

void HistPlotter2D(TCanvas *Histogram2DCanvas, TList *MScThesisPlotsList, const char *filename, const char *Histogram2DName,
                   const string &SampleName, const string &SavePath, const string &SaveName) {
    cout << "\n\n";

    system(("mkdir -p " + SavePath).c_str());

    Histogram2DCanvas->Clear();

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

    Histogram2DCanvas->cd();

    const double beamE = 5.98636;
    const string Region = ConfigRegion(Histogram2DNameCopy);
    string sNameFlag = ConfigSName(SampleName);

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

        if ((Histogram2DNameCopy == "#beta vs. P (all particles, no #(e) cut, CD)") || (Histogram2DNameCopy == "#beta vs. P (all particles, no #(e) cut, FD)") ||
            (Histogram2DNameCopy == "#beta vs. P (all particles, 1e cut, CD)") || (Histogram2DNameCopy == "#beta vs. P (all particles, 1e cut, FD)")) {
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

        } else if (Histogram2DNameCopy == "SF vs. P_{e} BC (1e cut, FD)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *UpperFScut = new TLine(gPad->GetUxmin(), 0.28, gPad->GetUxmax(), 0.28);
            UpperFScut->SetLineWidth(2);
            UpperFScut->SetLineColor(kRed);
            UpperFScut->Draw("same");

            TLine *LowerFScut = new TLine(gPad->GetUxmin(), 0.2, gPad->GetUxmax(), 0.2);
            LowerFScut->SetLineWidth(2);
            LowerFScut->SetLineColor(kRed);
            LowerFScut->Draw("same");
        } else if (Histogram2DNameCopy == "Vcal vs. SF BC (1e cut, PCAL)" || Histogram2DNameCopy == "Wcal vs. SF BC (1e cut, PCAL)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *LowerECALcoorCut = new TLine(14., gPad->GetUymin(), 14., gPad->GetUymax());
            LowerECALcoorCut->SetLineWidth(2);
            LowerECALcoorCut->SetLineColor(kRed);
            LowerECALcoorCut->Draw("same");
        } else if (Histogram2DNameCopy == "#beta vs. P (electrons only, 1e cut)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *EquiLine = new TLine(gPad->GetUxmin(), 1, gPad->GetUxmax(), 1);
            EquiLine->SetLineWidth(2);
            EquiLine->SetLineColor(kBlack);
            EquiLine->Draw("same");

            TLine *UpperBetaElectronCut = new TLine(gPad->GetUxmin(), 1.2, gPad->GetUxmax(), 1.2);
            UpperBetaElectronCut->SetLineWidth(2);
            UpperBetaElectronCut->SetLineColor(kRed);
            UpperBetaElectronCut->Draw("same");
        } else if (Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)" ||
                   Histogram2DNameCopy == "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)") {
//            Histogram2D->SetTitle(("#beta vs. P of all particles in the " + Region).c_str());

            TLine *UpperThetapFDcut = new TLine(gPad->GetUxmin(), 45., gPad->GetUxmax(), 45.);
            UpperThetapFDcut->SetLineWidth(2);
            UpperThetapFDcut->SetLineColor(kRed);
            UpperThetapFDcut->Draw("same");

            TLine *LowerThetapFDcut = new TLine(gPad->GetUxmin(), 35., gPad->GetUxmax(), 35.);
            LowerThetapFDcut->SetLineWidth(2);
            LowerThetapFDcut->SetLineColor(kRed);
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
        Histogram2DCanvas->SetLogz(1);
        Histogram2DCanvas->SaveAs((SavePath + "/" + sNameFlag + SaveName + "_zLogScale.png").c_str());
    }

    if (ZLinearScalePlot) {
        Histogram2DCanvas->SetLogz(0);
        system(("mkdir -p " + SavePath + "/z_Linear_Scale_plots").c_str());
        Histogram2DCanvas->SaveAs((SavePath + "/z_Linear_Scale_plots/" + sNameFlag + SaveName + "_zLinearScale.png").c_str());
    }

    gStyle->SetStatX(DefStatX);
    gStyle->SetStatY(DefStatY);
//        Histogram2DCanvas->Clear();

//    //<editor-fold desc="old">
//    TCanvas *c = new TCanvas("c1", "c2", 1000, 750 * 4.5); // normal res
//    c->Divide(3, 1);
//
//    TFile *f_Reco = new TFile("02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root");
//    TH2D *nFD_AMap_Reco = (TH2D *) f_Reco->Get("Reco Neutron Hit Map");
//    c->cd(1);
//    c->cd(1)->SetGrid();
//    c->cd(1)->SetBottomMargin(0.14);
//    c->cd(1)->SetLeftMargin(0.18);
//    c->cd(1)->SetRightMargin(0.12);
//    nFD_AMap_Reco->Draw("colz");
//
//    TFile *f_TL = new TFile("01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root");
//    TH2D *nFD_AMap_TL = (TH2D *) f_TL->Get("TL Neutron Hit Map");
//    c->cd(2);
//    c->cd(2)->SetGrid();
//    c->cd(2)->SetBottomMargin(0.14);
//    c->cd(2)->SetLeftMargin(0.18);
//    c->cd(2)->SetRightMargin(0.12);
//    nFD_AMap_TL->Draw("colz");
//
//    TFile *f_Div = new TFile("02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root");
//    TH2D *nFD_AMap_Div = (TH2D *) f_Div->Get("Reco Neutron Hit Map");
//    nFD_AMap_Div->Divide(nFD_AMap_TL);
//    c->cd(3);
//    c->cd(3)->SetGrid();
//    c->cd(3)->SetBottomMargin(0.14);
//    c->cd(3)->SetLeftMargin(0.18);
//    c->cd(3)->SetRightMargin(0.12);
//    nFD_AMap_Div->SetTitle("divided");
//    nFD_AMap_Div->Draw("colz");
//    //</editor-fold>

}
