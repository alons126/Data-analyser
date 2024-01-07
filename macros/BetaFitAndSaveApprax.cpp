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

#include "../source/classes/DSCuts/DSCuts.h"
#include "../source/functions/Math_func/poly34.cpp"
#include "../source/functions/findSubstring.h"
#include "../source/functions/to_string_with_precision.h"
#include "../source/constants.h"

using namespace std;

void drawtext() {
    Int_t i, n;
    Double_t x, y;
    TLatex l;

    l.SetTextSize(0.025);
    l.SetTextFont(0);
//    l.SetTextFont(42);
    l.SetTextAlign(21);
    l.SetTextColor(kMagenta);
    l.SetLineColor(kWhite);

    auto g = (TGraph *) gPad->GetListOfPrimitives()->FindObject("Graph");
    n = g->GetN();

    for (i = 0; i < n; i++) {
        g->GetPoint(i, x, y);
        l.PaintText(x, y + 0.02, Form("(%4.5f,%4.1f)", x, y));
    }
}

Double_t FitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
//    if (par[1] != 0) { arg = (v[0] - 1) / par[1]; } // 2 parameters
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}

void BetaFitAndSaveApprax() {
    cout << "\n\n";

    //    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-_ALL_CUTS/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_10";

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5_first_100";

    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
    string SampleName = "C12_simulation_6GeV_T5_first_250";

//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_NoBetaCut_wPhotonsFD/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5";

    cout << "\nSampleName:\t" << SampleName << "\n\n";

    DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, -9999, 9999); /* Beta cut (1n, FD) */
    DSCuts n_momentum_cuts = DSCuts("Momentum", "", "Neutron", "", 0, 0.3, 9999); /* Momentum cuts (thresholds) */

    double W_yLLim = -0.1, W_yULim = 0.1, W_xLLim = 0.9, W_xULim = 1.;
    double deltaPRel_UncertaintyU = 0.2, deltaPRel_UncertaintyL = 0.1;

    //<editor-fold desc="Canvas definitions">
    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000*2, 750*2); // normal res
    Canvas->SetGrid();
    Canvas->SetBottomMargin(0.14);

    Canvas->SetLeftMargin(0.18);
    Canvas->SetRightMargin(0.12);

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

    Canvas->cd();
    //</editor-fold>

    //<editor-fold desc="Setting sNameFlag">
    string sNameFlag;

    if (findSubstring(SampleName, "sim")) {
        sNameFlag = "s";
    } else if (findSubstring(SampleName, "data")) {
        sNameFlag = "d";
    }
    //</editor-fold>

    //<editor-fold desc="Setting particle">
    string BetaTitle = "Neutron #beta from 'photons'";

    string BetaParticle, BetaParticleShort;

    if (findSubstring(BetaTitle, "Electron") || findSubstring(BetaTitle, "electron")) {
        BetaParticle = "Electron";
        BetaParticleShort = "e";
    } else if (findSubstring(BetaTitle, "Proton") || findSubstring(BetaTitle, "proton")) {
        BetaParticle = "Proton";
        BetaParticleShort = "p";
    } else if (findSubstring(BetaTitle, "Neutron") || findSubstring(BetaTitle, "neutron")) {
        BetaParticle = "Neutron";
        BetaParticleShort = "n";
    } else if (findSubstring(BetaTitle, "#pi^{+}")) {
        BetaParticle = "Piplus";
        BetaParticleShort = "#pi^{+}";
    } else if (findSubstring(BetaTitle, "#pi^{-}")) {
        BetaParticle = "Piminus";
        BetaParticleShort = "#pi^{-}";
    } else if (findSubstring(BetaTitle, "K^{+}")) {
        BetaParticle = "Kplus";
        BetaParticleShort = "K^{+}";
    } else if (findSubstring(BetaTitle, "K^{-}")) {
        BetaParticle = "Kminus";
        BetaParticleShort = "K^{-}";
    } else if (findSubstring(BetaTitle, "#gamma") || findSubstring(BetaTitle, "photon")
               || findSubstring(BetaTitle, "Photon")) {
        BetaParticle = "Photon";
        BetaParticleShort = "#gamma";
    }
    //</editor-fold>

    //<editor-fold desc="Setting final State">
    string BetaFinalState = "1n";
    //</editor-fold>

    //<editor-fold desc="Setting histogram and preforming a fit">
    TH1D *hpx = (TH1D *) f->Get("#beta of n from '#gamma' (1n, FD)");
    TH1D *hBeta_Clone = (TH1D *) hpx->Clone("#beta of n from '#gamma' - fitted");
    Int_t Color = hBeta_Clone->GetLineColor();

    TF1 *func = new TF1("fit", FitFunction, 0, 2, 3); // create a function with 3 parameters in the range [-3,3]
    func->SetLineColor(kRed);

    double BetaMax = hBeta_Clone->GetMaximum();
    double BetaMean = hBeta_Clone->GetMean();

    func->SetParameters(BetaMax, BetaMean, 0.001); // start fit with histogram's max and mean
    func->SetParNames("Constant", "Mean_value", "Sigma");

    cout << "Beta histogram {max, mean}:\t{" << BetaMax << ", " << BetaMean << "}\n\n\n\n";

////    // Adding limits to "Constant"
////    double BetaConstantUlim = 1.2 * BetaMax;
////    double BetaConstantLlim = 0.95 * BetaMax;
////    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
////    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
//    // Adding limits to "Constant"
//    double BetaConstantUlim = 1.2 * BetaMax;
//    double BetaConstantLlim = BetaMax;
////    double BetaConstantLlim = 0.90 * BetaMax;
////    double BetaConstantLlim = 0.67 * BetaMax;
//    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
//    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";

    // Adding limits to "Constant"
    double BetaConstantUlim = 1.5 * BetaMax;
//    double BetaConstantLlim = BetaMax;
    double BetaConstantLlim = 0.90 * BetaMax;
//    double BetaConstantLlim = 0.67 * BetaMax;
    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";


    // Adding limits to "Mean_value"
    double BetaMean_valueUlim = 1.01;
//    double BetaMean_valueLlim = 1.008;
    double BetaMean_valueLlim = 1.005;
    func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
    cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";
//    // Adding limits to "Mean_value"
//    double BetaMean_valueUlim = 1.02;
//    double BetaMean_valueLlim = 1.005;
//    func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
//    cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";


//    // Adding limits to "Sigma"
////    double BetaSigmaUlim = 0.015;
//    double BetaSigmaUlim = 0.01;
//    double BetaSigmaLlim = 0.00001;
//    func->SetParLimits(2, BetaSigmaLlim, BetaSigmaUlim);
//    cout << "Beta Sigma {Llim, Ulim}:\t{" << BetaSigmaLlim << ", " << BetaSigmaUlim << "}\n\n";



    cout << "\n";
    hBeta_Clone->Fit("fit");
    hBeta_Clone->Draw();
    cout << "\n";

    TF1 *fit = hBeta_Clone->GetFunction("fit");
    double FitAmp = fit->GetParameter(0); // get p0
    double FitMean = fit->GetParameter(1); // get p1
    double FitStd = fit->GetParameter(2); // get p2

    Beta_cut.SetUpperCut(fit->GetParameter(2));
    //</editor-fold>

    //<editor-fold desc="Drawing fit parameters and saving">
    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetTextFont(0);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 8)).c_str());
    FitParam->AddText(("Fit mean = " + to_string_with_precision(FitMean, 8)).c_str());
    FitParam->AddText(("Fit std = " + to_string_with_precision(FitStd, 8)).c_str());
    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
    FitParam->Draw("same");

    string hBeta_CloneSaveNameDir = "./" + sNameFlag + "01_Beta_Neutron_from_photons_1n" + "_fitted.png";
    const char *hBeta_CloneSaveDir = hBeta_CloneSaveNameDir.c_str();
    Canvas->SaveAs(hBeta_CloneSaveDir);

    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Plot deltaP as function of beta">
    string deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
    string deltaPTitle = BetaParticle + " momentum uncertainty #deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
    string deltaPfunc = to_string(m_n * FitStd) + "/ ( (1 - x*x) * sqrt(1 - x*x) )";

    auto *deltaP = new TF1(deltaPStatsTitle.c_str(), deltaPfunc.c_str(), 0.9, 1);
    deltaP->SetTitle(deltaPTitle.c_str());
    deltaP->GetXaxis()->SetTitleSize(0.06);
    deltaP->GetXaxis()->SetLabelSize(0.0425);
    deltaP->GetXaxis()->CenterTitle(true);
    deltaP->GetXaxis()->SetTitle("#beta");
    deltaP->GetYaxis()->SetRangeUser(0., 30.);
    deltaP->GetYaxis()->SetTitleSize(0.06);
    deltaP->GetYaxis()->SetLabelSize(0.0425);
    deltaP->GetYaxis()->CenterTitle(true);
    deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#delta#beta}{(1 - #beta^{2})^{3/2}}").c_str());

    deltaP->SetLineColor(Color);
    deltaP->Draw();

    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05 + 0.15, x_2_FitParam, y_2_FitParam + 0.15, "NDC");
//    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.1, x_2_FitParam, y_2_FitParam, "NDC");
//    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    deltaPParam->SetBorderSize(1);
    deltaPParam->SetTextFont(0);
    deltaPParam->SetTextSize(0.03);
    deltaPParam->SetFillColor(0);
    deltaPParam->SetTextAlign(12);
    deltaPParam->AddText(("d#beta = " + to_string(FitStd)).c_str());
//    deltaPParam->AddText(("#delta#beta = " + to_string_with_precision(FitStd, 8)).c_str());
    deltaPParam->Draw("same");

    string deltaPSaveNameDir = "./" + sNameFlag + "02a_P_" + BetaParticleShort + "_uncertainty_" + BetaFinalState + ".png";
    const char *deltaPSaveDir = deltaPSaveNameDir.c_str();
    Canvas->SaveAs(deltaPSaveDir);

    Canvas->Clear();
    //</editor-fold>

    //<editor-fold desc="Solve deltaP/P for beta in range 0.9<=beta<1">
    double Beta_Max_Apprax, P_Beta_Max_Apprax, Beta_Min_Apprax, P_Beta_Min_Apprax;

    cout << "\nSolutions for deltaP/P = 20%:\n";

    Beta_Max_Apprax = sqrt(1 - FitStd / deltaPRel_UncertaintyU);
    P_Beta_Max_Apprax = m_n * Beta_Max_Apprax / sqrt(1 - Beta_Max_Apprax * Beta_Max_Apprax);

    cout << "Beta_Max_Apprax = " << Beta_Max_Apprax << " is chosen\n";
    cout << "P(Beta_Max_Apprax) = " << P_Beta_Max_Apprax << "\n\n";

    cout << "Solutions for deltaP/P = 10%:\n";

    Beta_Min_Apprax = sqrt(1 - FitStd / deltaPRel_UncertaintyL);
    P_Beta_Min_Apprax = m_n * Beta_Min_Apprax / sqrt(1 - Beta_Min_Apprax * Beta_Min_Apprax);

    cout << "Beta_Min_Apprax = " << Beta_Min_Apprax << " is chosen\n";
    cout << "P(Beta_Min_Apprax) = " << P_Beta_Min_Apprax << "\n\n";

////        cout << "\n\n\n\n" << BetaPlot.GetHistogram1DSaveNamePath() << "Approximatied_beta/" << "\n\n\n\n";
//        exit(0);

    n_momentum_cuts.SetUpperCut(P_Beta_Max_Apprax);
    //</editor-fold>

    //<editor-fold desc="Plot deltaP/P as function of beta">
    string Rel_deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
    string Rel_deltaPTitle = BetaParticle + " relative uncertainty #deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
    string Rel_deltaPfunc = to_string(FitStd) +  + "/ (1 - x*x)";

    auto *Rel_deltaP = new TF1(Rel_deltaPStatsTitle.c_str(), Rel_deltaPfunc.c_str(), 0.9, 1);
    Rel_deltaP->SetTitle(Rel_deltaPTitle.c_str());
    Rel_deltaP->GetXaxis()->SetTitleSize(0.06);
    Rel_deltaP->GetXaxis()->SetLabelSize(0.0425);
    Rel_deltaP->GetXaxis()->CenterTitle(true);
    Rel_deltaP->GetXaxis()->SetTitle("#beta");
    Rel_deltaP->GetYaxis()->SetRangeUser(0., 0.5);
    Rel_deltaP->GetYaxis()->SetTitleSize(0.06);
    Rel_deltaP->GetYaxis()->SetLabelSize(0.0425);
    Rel_deltaP->GetYaxis()->CenterTitle(true);
    Rel_deltaP->GetYaxis()->SetTitle(("#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}} = #frac{#delta#beta}{1 - #beta^{2}}").c_str());
    Rel_deltaP->SetLineColor(kBlack);
    Rel_deltaP->SetLineWidth(2);
    Rel_deltaP->Draw();

    TPaveText *deltaPRel_deltaP = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05, x_2_FitParam, y_2_FitParam, "NDC");
    deltaPRel_deltaP->SetBorderSize(1);
    deltaPRel_deltaP->SetTextFont(0);
    deltaPRel_deltaP->SetTextSize(0.03);
    deltaPRel_deltaP->SetFillColor(0);
    deltaPRel_deltaP->SetTextAlign(12);
    deltaPRel_deltaP->AddText(("d#beta = " + to_string(FitStd)).c_str());
    deltaPRel_deltaP->Draw("same");

    TLine *upper_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
    upper_cut->SetLineWidth(2);
    upper_cut->SetLineColor(kBlue);
    upper_cut->Draw("same");

    TLine *lower_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyL, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyL);
    lower_cut->SetLineWidth(2);
    lower_cut->SetLineColor(kRed);
    lower_cut->Draw("same");

    auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
    TLegendEntry *Cut_legend_deltaPRel_deltaP = Cut_legend->AddEntry(deltaPRel_deltaP, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}").c_str(),
                                                                     "l");
    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, "20% cut", "l");
    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, "10% cut", "l");
    Cut_legend->Draw("same");

    string Rel_deltaPSaveNameDir = "./" + sNameFlag + "02b_P_" + BetaParticleShort + "_apprax_rel_uncertainty_" + BetaFinalState + ".png";

    const Int_t n = 2;
    auto gr = new TGraph(n);
    gr->SetMarkerStyle(20);
    gr->SetMarkerSize(20);
    auto ex = new TExec("ex", "drawtext();");
    gr->GetListOfFunctions()->Add(ex);
    gr->SetPoint(0, Beta_Max_Apprax, deltaPRel_UncertaintyU);
    gr->SetPoint(1, Beta_Min_Apprax, deltaPRel_UncertaintyL);
    gr->Draw("same");

    const char *Rel_deltaPSaveDir = Rel_deltaPSaveNameDir.c_str();
    Canvas->SaveAs(Rel_deltaPSaveDir);
    Canvas->Clear();
    //</editor-fold>

}
