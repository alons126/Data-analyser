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

/* clas12root macros/MomResDebug.cpp -q */

vector<TH1D *> SliceLoader(const char *filename, vector<int> &SliceNumbers) {
    bool PrintOut = false;

    TFile *file = new TFile(filename);
    if (!file) { cout << "\n\nMomResDebug::SliceLoader: could not load Hit_Maps_TL root file! Exiting...\n", exit(0); }

    vector < TH1D * > MomResSlices;
    vector<int> SliceNumbers0;
    int Counter = 0;
    static TString classname("TH1D");

    TKey *Key;
    TIter Next((TList *) file->GetListOfKeys());

    while ((Key = (TKey *) Next())) {
//    while (Key = (TKey *) Next()) {

        if (PrintOut) { cout << "Key name: " << Key->GetName() << "|\t Type: " << Key->GetClassName() << "\n\n"; }

        if ((Key->GetClassName() == classname("TH1D")) && (findSubstring(Key->GetName(), "reco"))) {
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
        Canvas->SaveAs((MomResDebugSaveDir + "/" + "reco_Slice_" + to_string(SliceNumbers.at(i)) + ".png").c_str());
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

void MomResDebug() {

    //<editor-fold desc="Initial stuff">
    TCanvas *c = new TCanvas("c1", "c2", 1000 * 1.25, 750 * 1.25); // normal res
    c->cd()->SetGrid();
    c->cd()->SetBottomMargin(0.14), c->cd()->SetLeftMargin(0.16), c->cd()->SetRightMargin(0.16), c->cd()->SetTopMargin(0.12);
    c->cd();

    const string MomResDebugSaveDir = "MomRes_Fitted_Reco_Slices";
    system(("rm -r " + MomResDebugSaveDir).c_str());
    system(("mkdir -p " + MomResDebugSaveDir).c_str());

    vector<int> SliceNumbers;
    const char *filename = "Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *filename = "C12x4_simulation_G18_Q204_6GeV_S03ACNC_momResS1_Eff1"
//                           "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    vector < TH1D * > MomResSlices = SliceLoader(filename, SliceNumbers);
    //</editor-fold>

    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1., 2.5); // new upper cut, following Larry meeting (10/08/23)
    DSCuts FD_nucleon_momentum_cut_nRes = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1., 2.2); // new upper cut, following Larry meeting (10/08/23)
    double beamE = 5.98636;
    MomentumResolution nRes("Neutron");

    nRes.MomResInit(true, false, false, "C12x4_simulation_G18_Q204_6GeV", MomResDebugSaveDir, beamE, FD_nucleon_momentum_cut_nRes,
                    0.4, MomResDebugSaveDir, MomResDebugSaveDir, 0.05, true, "pol1_wKC", "pol3_wKC", false, false, true);

    string MomentumType = "reco";
    vector <vector<double>> ResRecoMomSlicesLimits = nRes.GetResRecoMomSlicesLimits();
    vector <DSCuts> ResRecoMomSlicesFitVar = nRes.GetResRecoMomSlicesFitVar();
    vector <DSCuts> ResRecoMomSlicesHistVar = nRes.GetResRecoMomSlicesHistVar();
    vector<int> FittedRecoMomSlices = nRes.GetFittedRecoMomSlices();
    vector <vector<double>> Reco_FitParam;
    vector <vector<double>> Reco_FitParam_Corr_pol1, Reco_FitParam_Corr_pol1_wKC;
    vector <vector<double>> Reco_FitParam_Corr_pol2, Reco_FitParam_Corr_pol2_wKC;
    vector <vector<double>> Reco_FitParam_Corr_pol3, Reco_FitParam_Corr_pol3_wKC;

    for (int i = 0; i < MomResSlices.size(); i++) {
        TH1D *hSlice = MomResSlices.at(i);
        hSlice->GetXaxis()->SetTitleSize(0.06), hSlice->GetXaxis()->SetLabelSize(0.0425), hSlice->GetXaxis()->CenterTitle(true);
        hSlice->GetYaxis()->SetTitle("Number of events");
        hSlice->GetYaxis()->SetTitleSize(0.06), hSlice->GetYaxis()->SetLabelSize(0.0425), hSlice->GetYaxis()->CenterTitle(true);
        hSlice->Sumw2();

        bool isNeutron = IsNeutron(hSlice->GetTitle()), isProton = IsProton(hSlice->GetTitle());

        if (hSlice->Integral() != 0.) { // Fit only the non-empty histograms
            cout << "\n\n";

            double FitUlim, FitLlim;

            if (isNeutron) {
                FitUlim = 0.75, FitLlim = -0.75;
//                FitUlim = 1., FitLlim = -1.;

//                //<editor-fold desc="Original">
//                FitUlim = 1., FitLlim = -1.;
//                //</editor-fold>
            } else if (isProton) {
                FitUlim = 0.5, FitLlim = -0.5;
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
                func->SetParLimits(1, -1.5, 1.5); // Mean limits
                func->SetParLimits(2, 0.0000000001, 0.35); // Sigma limits
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

//            hSlice->Fit("fit");
            hSlice->Fit("fit","W");
            hSlice->SetLineColor(kBlue);
            hSlice->SetLineWidth(2);

            double FitAmp = func->GetParameter(0);  // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2);  // get p2

            ResRecoMomSlicesFitVar.at(i).SetMean(FitMean); // For neutron correction
            ResRecoMomSlicesFitVar.at(i).SetUpperCut(FitStd); // For proton smearing
            ResRecoMomSlicesHistVar.at(i).SetMean(SliceMean);
            ResRecoMomSlicesHistVar.at(i).SetUpperCut(SliceStd);

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
            if (ResRecoMomSlicesLimits.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

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
                        } else if (MomentumType == "reco") {
                            if (isNeutron) {
                                FittedRecoNeutronResSlices->Add(hSlice);
                            } else if (isProton) {
                                FittedRecoProtonResSlices->Add(hSlice);
                            }
                        }
            */

            c->Clear();

            FittedRecoMomSlices.push_back(i); // Log slices that were fitted
        } else {
            continue;
        }
    }

    nRes.SetResRecoMomSlicesLimits(ResRecoMomSlicesLimits);
    nRes.SetResRecoMomSlicesFitVar(ResRecoMomSlicesFitVar);
    nRes.SetResRecoMomSlicesHistVar(ResRecoMomSlicesHistVar);
    nRes.SetFittedRecoMomSlices(FittedRecoMomSlices);
    nRes.OverwriteResRecoMomSlices(MomResSlices);

    nRes.PolyFitter(MomentumType, 1, "Corr", "noKC", Reco_FitParam_Corr_pol1);
    nRes.PolyFitter(MomentumType, 1, "Corr", "wKC", Reco_FitParam_Corr_pol1_wKC);
    nRes.PolyFitter(MomentumType, 2, "Corr", "noKC", Reco_FitParam_Corr_pol2);
    nRes.PolyFitter(MomentumType, 2, "Corr", "wKC", Reco_FitParam_Corr_pol2_wKC);
    nRes.PolyFitter(MomentumType, 3, "Corr", "noKC", Reco_FitParam_Corr_pol3);
    nRes.PolyFitter(MomentumType, 3, "Corr", "wKC", Reco_FitParam_Corr_pol3_wKC);

    //    //<editor-fold desc="Final stuff">
//    DrawAndSave(c, MomResDebugSaveDir, MomResSlices, SliceNumbers);
//    //</editor-fold>

}
