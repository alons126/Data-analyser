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

#define MomResDebugMacro true

#include "../source/functions/GeneralFunctions.h"
#include "../source/constants.h"
#include "../source/classes/MomentumResolution/MomentumResolution.cpp"

using namespace std;

/* clas12root macros/MomResDebug_2.cpp -q */

vector<TH1D *> SliceLoader(const char *filename, vector<int> &SliceNumbers, const string &Variant = "reco") {
    bool PrintOut = false;

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nMomResDebug_2::SliceLoader: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    vector < TH1D * > MomResSlices;
    vector<int> SliceNumbers0;
    int Counter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while ((Key = (TKey *) Next())) {

        if (PrintOut) { cout << "Key name: " << Key->GetName() << "|\t Type: " << Key->GetClassName() << "\n\n"; }

        if ((Key->GetClassName() == classname("TH1D")) && (findSubstring(Key->GetName(), Variant))) {
            ++Counter;

            TH1D *Histogram1DTempFromKey = (TH1D *) Key->ReadObj();
            string CloneName = Histogram1DTempFromKey->GetName();
            TH1D *Histogram1DTemp = (TH1D *) Histogram1DTempFromKey->Clone((CloneName + " - CLONED").c_str());

            string Histogram1DTempName = Histogram1DTemp->GetName();

            auto FuncList = Histogram1DTemp->GetListOfFunctions();
            FuncList->Clear();

            MomResSlices.push_back(Histogram1DTemp);
            SliceNumbers0.push_back(Counter);
        }
    }

    SliceNumbers = SliceNumbers0;
    return MomResSlices;
}

void DrawAndSave(TCanvas *Canvas, const string &MomResDebugSaveDir, vector<TH1D *> &MomResSlices, vector<int> &SliceNumbers) {
    Canvas->cd();

    for (int i = 0; i < MomResSlices.size(); i++) {
        MomResSlices.at(i)->Draw();
        Canvas->SaveAs((MomResDebugSaveDir + "/" + "TL_Slice_" + to_string(SliceNumbers.at(i)) + ".png").c_str());
        Canvas->Clear();
    }
}

bool IsNeutron(const string &source) {
    if (findSubstring(source, "Neutron") || findSubstring(source, "neutron")) {
        return true;
    } else {
        return false;
    }
}

bool IsProton(const string &source) {
    if (findSubstring(source, "Proton") || findSubstring(source, "proton")) {
        return true;
    } else {
        return false;
    }
}

void MomResDebug_2() {

    //<editor-fold desc="Initial stuff">
    TCanvas *c = new TCanvas("c1", "c2", 1000 * 1.25, 750 * 1.25); // normal res
    c->cd()->SetGrid();
    c->cd()->SetBottomMargin(0.14), c->cd()->SetLeftMargin(0.16), c->cd()->SetRightMargin(0.16), c->cd()->SetTopMargin(0.12);
    c->cd();

    const string MomResDebugSaveDir = "MomRes_Fitted_TL_Slices";
    system(("rm -r " + MomResDebugSaveDir).c_str());
    system(("mkdir -p " + MomResDebugSaveDir).c_str());

    vector<int> SliceNumbers;
    const char *filename = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1_v3"
                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    vector < TH1D * > MomResSlices = SliceLoader(filename, SliceNumbers, "truth");
//    vector < TH1D * > MomResSlices = SliceLoader(filename, SliceNumbers);
    //</editor-fold>

    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1., 2.5); // new upper cut, following Larry meeting (10/08/23)
    DSCuts FD_nucleon_momentum_cut_nRes = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1., 2.2); // new upper cut, following Larry meeting (10/08/23)

    DSCuts MomRes_mu_cuts = DSCuts("MomRes_mu_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2); // E4nu meeting (29/01/24)
    DSCuts MomRes_sigma_cuts = DSCuts("MomRes_sigma_cuts", "FD", "", "Protons and neutrons", 0, FD_nucleon_momentum_cut.GetLowerCut(), 2.2); // Adi meeting after (29/01/24)


    double beamE = 5.98636;
    MomentumResolution nRes("Proton");

    nRes.MomResInit(true, false, false, "C12x4_simulation_G18_Q204_6GeV", MomResDebugSaveDir, beamE, MomRes_mu_cuts, MomRes_sigma_cuts,
                    0.4, MomResDebugSaveDir, MomResDebugSaveDir, 0.05, true, "pol1_wKC", "pol3_wKC", false, false, true);

    string MomentumType = "truth";
    vector <vector<double>> ResTLMomSlicesLimits = nRes.GetResTLMomSlicesLimits();
    vector <DSCuts> ResTLMomSlicesFitVar = nRes.GetResTLMomSlicesFitVar();
    vector <DSCuts> ResTLMomSlicesHistVar = nRes.GetResTLMomSlicesHistVar();
    vector<int> FittedTLMomSlices = nRes.GetFittedTLMomSlices();
    vector <vector<double>> TL_FitParam;
    vector <vector<double>> TL_FitParam_Smear_pol1, TL_FitParam_Smear_pol1_wKC;
    vector <vector<double>> TL_FitParam_Smear_pol2, TL_FitParam_Smear_pol2_wKC;
    vector <vector<double>> TL_FitParam_Smear_pol3, TL_FitParam_Smear_pol3_wKC;

    for (int i = 0; i < MomResSlices.size(); i++) {
        TH1D *hSlice = MomResSlices.at(i);
        hSlice->GetXaxis()->SetTitleSize(0.06), hSlice->GetXaxis()->SetLabelSize(0.0425), hSlice->GetXaxis()->CenterTitle(true);
        hSlice->GetYaxis()->SetTitle("Number of events");
        hSlice->GetYaxis()->SetTitleSize(0.06), hSlice->GetYaxis()->SetLabelSize(0.0425), hSlice->GetYaxis()->CenterTitle(true);
        hSlice->Sumw2();

        bool isNeutron = IsNeutron(hSlice->GetTitle()), isProton = IsProton(hSlice->GetTitle());

        if (hSlice->Integral() != 0.) {
            if (true) { // Fit only the non-empty histograms
//            if (i >= 5 && i <= 21) { // Fit only the non-empty histograms
                cout << "\n\n";

                double FitUlim, FitLlim;

                if (isNeutron) {
                    FitUlim = 0.75, FitLlim = -0.75;
//                FitUlim = 1., FitLlim = -1.;

//                //<editor-fold desc="Original">
//                FitUlim = 1., FitLlim = -1.;
//                //</editor-fold>
                } else if (isProton) {
                    FitUlim = 0.15, FitLlim = -0.15;
                }
                /*
                if (momResTestMode) { // In smear & shift test mode
                    FitUlim = 1., FitLlim = -1.; // For both neutrons and protons
                } else {
                    if (isNeutron) {
                        FitUlim = 1., FitLlim = -1.;
                    } else if (isProton) {
                        FitUlim = 0.5, FitLlim = -0.5;
                    }
                }
    */

                TF1 *fit;
                TF1 *func = new TF1("fit", CFitFunction, FitLlim, FitUlim, 3); // create a function with 3 parameters in the range [-3,3]
                func->SetLineColor(kMagenta);

                double SliceMax = hSlice->GetMaximum(), SliceMean = hSlice->GetMean(), SliceStd = hSlice->GetRMS();

                func->SetParameters(SliceMax, SliceMean, 0.5); // start fit with histogram's max and mean
                func->SetParNames("Constant", "Mean_value", "Sigma");

                if (isNeutron) {
//                func->SetParLimits(0, 0.925 * SliceMax, 1.075 * SliceMax); // Amp limits

                    double PeakLocation = hSlice->GetBinCenter(hSlice->GetMaximumBin());

//                func->SetParLimits(1, (PeakLocation - 0.5 * SliceMean), (PeakLocation + 0.5 * SliceMean)); // Mean limits
//                func->SetParLimits(1, (SliceMean - 0.25 * SliceStd), (SliceMean + 0.25 * SliceStd)); // Mean limits
//                func->SetParLimits(1, (PeakLocation - 0.25 * SliceStd), (PeakLocation + 0.25 * SliceStd)); // Mean limits
                    func->SetParLimits(1, FitLlim, FitUlim); // Mean limits
//                func->SetParLimits(1, -1.5, 1.5); // Mean limits

                    func->SetParLimits(2, 0.00001, 0.2); // Sigma limits
//                func->SetParLimits(2, 0.00001, 0.35); // Sigma limits

                    //                //<editor-fold desc="Original">
//                func->SetParLimits(1, -1.5, 1.5); // Mean limits
//                func->SetParLimits(2, 0.001, 0.35); // Sigma limits
//                //</editor-fold>

                } else if (isProton) {
                    func->SetParLimits(0, 0.95 * SliceMax, 1.05 * SliceMax); // Amp limits
                    func->SetParLimits(1, -1.05 * SliceMean, 1.05 * SliceMean); // Mean limits
                    func->SetParLimits(2, 0.95 * SliceStd, 1.05 * SliceStd); // Sigma limits
//                func->SetParLimits(2, 0.0000000001, 0.35); // Sigma limits
//                    func->SetParLimits(0, 0.925 * SliceMax, 1.075 * SliceMax); // Amp limits
//                    func->SetParLimits(1, -1.075 * SliceMean, 1.075 * SliceMean); // Mean limits
//                    func->SetParLimits(2, 0.925 * SliceStd, 1.075 * SliceStd); // Sigma limits
////                func->SetParLimits(2, 0.0000000001, 0.35); // Sigma limits
                }
                /*
                            if (momResTestMode) { // In smear & shift test mode
                //                func->SetParLimits(1, -0.5, 0.5); // Mean limits
                                func->SetParLimits(1, -1.5, 1.5); // Mean limits
                //                func->SetParLimits(2, 0.0001, 0.35); // Sigma limits
                                func->SetParLimits(2, 0.001, 0.35); // Sigma limits
                            } else {
                                if (isNeutron) {
                //                    func->SetParLimits(1, -0.5, 0.5); // Mean limits
                                    func->SetParLimits(1, -1.5, 1.5); // Mean limits
                //                    func->SetParLimits(2, 0.0001, 0.35); // Sigma limits
                                    func->SetParLimits(2, 0.001, 0.35); // Sigma limits
                                } else if (isProton) {
                //                    func->SetParLimits(1, -0.5, 0.5); // Mean limits
                                    func->SetParLimits(1, -1.5, 1.5); // Mean limits
                                    func->SetParLimits(2, 0.0000000001, 0.35); // Sigma limits
                                }
                            }
                */

                hSlice->Fit("fit");
//            hSlice->Fit("fit", "W");
                hSlice->SetLineColor(kBlue);
                hSlice->SetLineWidth(2);

                double FitAmp = func->GetParameter(0);  // get p0
                double FitMean = func->GetParameter(1); // get p1
                double FitStd = func->GetParameter(2);  // get p2

                ResTLMomSlicesFitVar.at(i).SetMean(FitMean); // For neutron correction
                ResTLMomSlicesFitVar.at(i).SetUpperCut(FitStd); // For proton smearing
                ResTLMomSlicesHistVar.at(i).SetMean(SliceMean);
                ResTLMomSlicesHistVar.at(i).SetUpperCut(SliceStd);

                auto FuncList = hSlice->GetListOfFunctions();
                FuncList->Clear();
                func->Draw("same && c");
                func->SetLineColor(kMagenta);

                double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
                double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
                double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
                double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

                TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam - 0.025, "NDC");
                FitParam->SetBorderSize(1), FitParam->SetFillColor(0);
                FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);
                FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 4)).c_str());
                FitParam->AddText(("Fit #mu = " + to_string_with_precision(FitMean, 4)).c_str());
                FitParam->AddText(("Fit #sigma = " + to_string_with_precision(FitStd, 4)).c_str());
                FitParam->Draw("same");

                int SliceUpperLimPrecision;
                if (ResTLMomSlicesLimits.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

                string hSlice_CloneSaveDir = MomResDebugSaveDir;
                string hSlice_CloneSaveName = MomResDebugSaveDir + "/" + "Slice_" + to_string(SliceNumbers.at(i)) + "_fitted.png";
                system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

                auto ListOfFunctions = hSlice->GetListOfFunctions();
                ListOfFunctions->Add((TObject *) FitParam);

                cout << "\n", c->SaveAs(hSlice_CloneSaveName.c_str());

                /*
                            if (MomentumType == "truth") {
                                if (isNeutron) {
                                    FittedTLNeutronResSlices->Add(hSlice);
                                } else if (isProton) {
                                    FittedTLProtonResSlices->Add(hSlice);
                                }
                            } else if (MomentumType == "TL") {
                                if (isNeutron) {
                                    FittedTLNeutronResSlices->Add(hSlice);
                                } else if (isProton) {
                                    FittedTLProtonResSlices->Add(hSlice);
                                }
                            }
                */

                c->Clear();

                FittedTLMomSlices.push_back(i); // Log slices that were fitted
            }
        } else {
            continue;
        }
    }

    nRes.SetResTLMomSlicesLimits(ResTLMomSlicesLimits);
    nRes.SetResTLMomSlicesFitVar(ResTLMomSlicesFitVar);
    nRes.SetResTLMomSlicesHistVar(ResTLMomSlicesHistVar);
    nRes.SetFittedTLMomSlices(FittedTLMomSlices);
    nRes.OverwriteResTLMomSlices(MomResSlices);

    nRes.PolyFitter(MomentumType, 1, "Smear", "noKC", TL_FitParam_Smear_pol1);
    nRes.PolyFitter(MomentumType, 1, "Smear", "wKC", TL_FitParam_Smear_pol1_wKC);
    nRes.PolyFitter(MomentumType, 2, "Smear", "noKC", TL_FitParam_Smear_pol2);
    nRes.PolyFitter(MomentumType, 2, "Smear", "wKC", TL_FitParam_Smear_pol2_wKC);
    nRes.PolyFitter(MomentumType, 3, "Smear", "noKC", TL_FitParam_Smear_pol3);
    nRes.PolyFitter(MomentumType, 3, "Smear", "wKC", TL_FitParam_Smear_pol3_wKC);

    nRes.DrawAndSaveResSlices("C12x4_simulation_G18_Q204_6GeV", c, MomResDebugSaveDir, "");
    //    //<editor-fold desc="Final stuff">
//    DrawAndSave(c, MomResDebugSaveDir, MomResSlices, SliceNumbers);
//    //</editor-fold>

}
