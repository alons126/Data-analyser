//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//TODO: add a software that creates momentum slices with at least 1000 events each automatically

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const string &Particle, double beamE, double ParticleMomTh,
                                     const string &SavePath, double DeltaSlices, bool VaryingDelta, const string &SmearM, const string &ShiftM, bool nRes_test) {
    SlicesSavePath = SavePath;
    delta = DeltaSlices;
    nResTestMode = nRes_test;
    SmearMode = SmearM, ShiftMode = ShiftM;

    double Delta = delta, SliceLowerLim = ParticleMomTh, SliceUpperLim;
    SetUpperMomCut(SampleName, NucleonCutsDirectory);

    if (!VaryingDelta) {
        SliceUpperLim = SliceLowerLim + delta;
    } else {
        if (beamE == 5.98636) {
            if ((SampleName == "C12_simulation_G18_Q204_6GeV") || (SampleName == "C12x4_simulation_G18_Q204_6GeV")) {
//            if (SampleName == "C12_simulation_G18_Q204_6GeV") {
                SliceUpperLim = SliceLowerLim + 0.25;
            } else {
                SliceUpperLim = SliceLowerLim + 0.3;
            }
        } else if (beamE == 2.07052) {
            SliceUpperLim = SliceLowerLim + 0.15;
        }
    }

    bool SliceAndDice = true;
    int SliceNumber = 0;

    /* Variables for debugging purposes: */
    bool LimitsPrintOut = false;
    bool LimitsPrintOutAndExit = false;

    while (SliceAndDice) {
        ++SliceNumber;

        if (LimitsPrintOut) {
            cout << "\n\n\nSliceLowerLim = " << SliceLowerLim << "\n";
            cout << "SliceUpperLim = " << SliceUpperLim << "\n\n";
//            cout << "SliceNumber = " << SliceNumber << "\n";
//            cout << "Delta = " << Delta << "\n";
        }

        //<editor-fold desc="Safty check">
        if ((abs(SliceUpperLim) > beamE * 1.5) || (abs(SliceLowerLim) > beamE * 1.5)) {
            cout << "NeutronResolution::NeutronResolution: slice limits are not defined properly! Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        int SliceUpperLimPrecision;
        if (SliceUpperLim == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

        string hStatsTitle, hTitle, hSaveName, hCutName;

        hPlot1D hResolutionSlice;

        DSCuts ResSliceFitCuts;

        if (Particle == "Neutron") {
            hStatsTitle = "n res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Neutron resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{n}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_n_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "R_{n} = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName,
                                       hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            ResSliceFitCuts = DSCuts(("fit_" + hCutName), "FD", "Neutron", "1n", 0, -9999, 9999);
        } else if (Particle == "Proton") {
            hStatsTitle = "p res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Proton resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{p}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_p_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            if (!nRes_test) {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                           -0.75, 0.75, hSliceNumOfBin);
            } else {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            }

            ResSliceFitCuts = DSCuts(("fit_" + hCutName), "FD", "Proton", "1n", 0, -9999, 9999);
        }

        ResSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResSliceFitCuts.SetSliceNumber(SliceNumber);

        ResSlices.push_back(hResolutionSlice);
        ResSlicesLimits.push_back({SliceLowerLim, SliceUpperLim});
        ResSlicesFitVar.push_back(ResSliceFitCuts);

        ResSliceFitCuts.SetCutVariable(("hist_" + hCutName));
        ResSlicesHistVar.push_back(ResSliceFitCuts);

        if (SliceUpperLim == SliceUpperMomLim) {
//        if (SliceUpperLim == beamE) {
            SliceAndDice = false;
        } else {
            SliceLowerLim = SliceUpperLim;

            if (VaryingDelta) {
                if (beamE == 5.98636) {
                    if ((SampleName == "C12_simulation_G18_Q204_6GeV") || (SampleName == "C12x4_simulation_G18_Q204_6GeV")) {
//                    if (SampleName == "C12_simulation_G18_Q204_6GeV") {
                        if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.60)) { // 0.4-0.65
                            Delta = delta * 5;
                        } else if ((SliceLowerLim >= 0.60) && (SliceLowerLim < 1.00)) { // 0.65-1.05
                            Delta = delta * 2;
                        } else if ((SliceLowerLim >= 1.00) && (SliceLowerLim < 1.70)) { // 1.05-1.70
                            Delta = delta * 1;
                        } else if ((SliceLowerLim >= 1.70) && (SliceLowerLim < 2.25)) { // 1.70-2.30
                            Delta = delta * 2;
                        } else if ((SliceLowerLim >= 2.25) && (SliceLowerLim < 2.55)) { // 2.30-2.60
                            Delta = delta * 3;
                        } else if ((SliceLowerLim >= 2.55) && (SliceLowerLim < 2.95)) { // 2.60-3.00
                            Delta = delta * 4;
                        } else if ((SliceLowerLim >= 2.95) && (SliceLowerLim < 3.25)) { // 3.00-3.30
                            Delta = delta * 6;
                        } else if (SliceLowerLim >= 3.25) { // 3.00-SliceUpperMomLim
                            Delta = SliceUpperMomLim - SliceLowerLim;
                        }
                    } else {
                        if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.65)) { // 0.4-0.7
                            Delta = delta * 6;
                        } else if ((SliceLowerLim >= 0.65) && (SliceLowerLim < 0.85)) { // 0.7-0.9
                            Delta = delta * 4;
                        } else if ((SliceLowerLim >= 0.85) && (SliceLowerLim < 1.85)) { // 0.9-1.90
                            Delta = delta * 2;
                        } else if ((SliceLowerLim >= 1.85) && (SliceLowerLim < 2.15)) { // 1.90-2.20
                            Delta = delta * 3;
                        } else if ((SliceLowerLim >= 2.15) && (SliceLowerLim < 2.35)) { // 2.20-2.40
                            Delta = delta * 4;
                        } else if ((SliceLowerLim >= 2.35) && (SliceLowerLim < 2.60)) { // 2.40-2.65
                            Delta = delta * 5;
                        } else if ((SliceLowerLim >= 2.60) && (SliceLowerLim < 2.95)) { // 2.65-3.00
                            Delta = delta * 7;
                        } else if (SliceLowerLim >= 2.95) { // 3.00-SliceUpperMomLim
                            Delta = SliceUpperMomLim - SliceLowerLim;
                        }
                    }
                } else if (beamE == 2.07052) {
                    if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.50)) { // 0.4-0.55
                        Delta = delta * 3;
                    } else if ((SliceLowerLim >= 0.50) && (SliceLowerLim < 1.10)) { // 0.55-1.15
                        Delta = delta * 2;
                    } else if ((SliceLowerLim >= 1.10) && (SliceLowerLim < 1.25)) { // 1.15-1.30
                        Delta = delta * 3;
                    } else if (SliceLowerLim >= 1.25) { // 1.30-beamE
                        Delta = beamE - SliceLowerLim;
                    }
                }
            }

            if ((SliceUpperLim + Delta) > SliceUpperMomLim) {
                SliceUpperLim = SliceUpperMomLim;
//            if ((SliceUpperLim + Delta) > beamE) {
//                SliceUpperLim = beamE;
            } else {
                SliceUpperLim = SliceLowerLim + Delta;
            }
        }
    }

    if (LimitsPrintOut && LimitsPrintOutAndExit) { exit(0); }

    NumberOfSlices = SliceNumber;
}
//</editor-fold>

// ReadInputParam function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadInputParam function">
/* This function reads nucleon cuts (especially neutron upper th.). It was imported from the clas12ana class */

void NeutronResolution::ReadInputParam(const char *filename) {
    ifstream infile;
    infile.open(filename);

    if (infile.is_open()) {
        string tp;

        //remove 3 lines of header
        for (int i = 0; i < 3; i++)
            getline(infile, tp);

        while (getline(infile, tp))  //read data from file object and put it into string.
        {
            stringstream ss(tp);
            string parameter, parameter2;
            double value;
            //get cut identifier
            ss >> parameter;

            if (parameter == "nRes_Momentum_cut") {
//            if (parameter == "Momentum_cuts_ECAL") {
                ss >> parameter2;
                stringstream ss2(parameter2);
                string pid_v;
                int count = 0;
                string pid = "";
                vector<double> par;

                while (getline(ss2, pid_v, ':')) {
                    if (count == 0) { pid = pid_v; }
                    else { par.push_back(atof(pid_v.c_str())); }

                    count++;
                }

                if (pid != "") { Neutron_Momentum_cut = par.at(1); }
            }
        }
    } else
        cout << "Parameter file didn't read in " << endl;

//    return;
}
//</editor-fold>

// SetUpperMomCut function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetUpperMomCut function">
void NeutronResolution::SetUpperMomCut(const string &SampleName, const string &NucleonCutsDirectory) {
    ReadInputParam((NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str()); // load sample-appropreate cuts file from CutsDirectory

    SliceUpperMomLim = Neutron_Momentum_cut;
}
//</editor-fold>

// hFillResPlots function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillResPlots function">
void NeutronResolution::hFillResPlots(double Momentum, double Resolution, double Weight) {
    bool Printout = false;

    for (int i = 0; i < NumberOfSlices; i++) {
        if ((Momentum >= ResSlicesLimits.at(i).at(0)) && (Momentum < ResSlicesLimits.at(i).at(1))) {
            ResSlices.at(i).hFill(Resolution, Weight);

            if (Printout) {
                cout << "\n\nResSlicesLimits.at(i).at(0) = " << ResSlicesLimits.at(i).at(0) << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "ResSlicesLimits.at(i).at(1) = " << ResSlicesLimits.at(i).at(1) << "\n";
            }

            break; // no need to keep the loop going after filling histogram
        }
    }
}
//</editor-fold>

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SliceFitDrawAndSave function">
/* CFitFunction function for costume fit */
Double_t CFitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}

/* SliceFitDrawAndSave function for the fit */
void NeutronResolution::SliceFitDrawAndSave(const string &SampleName, const string &Particle, double beamE) {
    for (int i = 0; i < NumberOfSlices; i++) {
        cout << "\n\n";

        TCanvas *SliceFitCanvas = new TCanvas("SliceFitCanvas", "SliceFitCanvas", 1000, 750); // normal res
        SliceFitCanvas->SetGrid();
        SliceFitCanvas->SetBottomMargin(0.14);
        SliceFitCanvas->SetLeftMargin(0.18);
        SliceFitCanvas->SetRightMargin(0.12);
        SliceFitCanvas->cd();

        //<editor-fold desc="Setting sNameFlag">
        string sNameFlag;

        if (findSubstring(SampleName, "sim")) {
            sNameFlag = "s";
        } else if (findSubstring(SampleName, "data")) {
            sNameFlag = "d";
        }
        //</editor-fold>

        TH1D *hSlice = (TH1D *) ResSlices.at(i).GetHistogram();
        hSlice->GetXaxis()->SetTitleSize(0.06);
        hSlice->GetXaxis()->SetLabelSize(0.0425);
        hSlice->GetXaxis()->CenterTitle(true);
        hSlice->GetYaxis()->SetTitle("Arbitrary units (#events)");
        hSlice->GetYaxis()->SetTitleSize(0.06);
        hSlice->GetYaxis()->SetLabelSize(0.0425);
        hSlice->GetYaxis()->CenterTitle(true);
        hSlice->Sumw2();

        if (hSlice->Integral() != 0.) { // Fit only the non-empty histograms
            FittedSlices.push_back(i); // Log slices that were fitted

            double FitUlim, FitLlim;

            if (nResTestMode) { // In smear & shift test mode
                FitUlim = 1., FitLlim = -1.; // For both neutrons and protons
            } else {
                if (Particle == "Neutron") {
                    FitUlim = 1., FitLlim = -1.;
                } else if (Particle == "Proton") {
                    FitUlim = 0.5, FitLlim = -0.5;
                }
            }

            TF1 *func = new TF1("fit", CFitFunction, FitLlim, FitUlim, 3); // create a function with 3 parameters in the range [-3,3]
            func->SetLineColor(kRed);

            double SliceMax = hSlice->GetMaximum();
            double SliceMean = hSlice->GetMean();
            double SliceStd = hSlice->GetRMS();

            func->SetParameters(SliceMax, SliceMean, 0.5); // start fit with histogram's max and mean
            func->SetParNames("Constant", "Mean_value", "Sigma");

            if (nResTestMode) { // In smear & shift test mode
                func->SetParLimits(1, -1.5, 1.5); // Mean limits
                func->SetParLimits(2, 0.001, 0.35); // Sigma limits
            } else {
                if (Particle == "Neutron") {
                    func->SetParLimits(1, -1.5, 1.5); // Mean limits
                    func->SetParLimits(2, 0.001, 0.35); // Sigma limits
                } else if (Particle == "Proton") {
                    func->SetParLimits(1, -1.5, 1.5); // Mean limits
                    func->SetParLimits(2, 0.0000000001, 0.35); // Sigma limits
                }
            }

            hSlice->Fit("fit");

            double FitAmp = func->GetParameter(0);  // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2);  // get p3

            ResSlicesFitVar.at(i).SetMean(FitMean);
            ResSlicesFitVar.at(i).SetUpperCut(FitStd);
            ResSlicesHistVar.at(i).SetMean(SliceMean);
            ResSlicesHistVar.at(i).SetUpperCut(SliceStd);

            double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
            double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
            double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
            double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
            FitParam->SetBorderSize(1);
            FitParam->SetTextFont(0);
            FitParam->SetFillColor(0);
            FitParam->SetTextAlign(12);
            FitParam->AddText(("Fit amp = " + to_string(FitAmp)).c_str());
            FitParam->AddText(("Fit mean = " + to_string(FitMean)).c_str());
            FitParam->AddText(("Fit std = " + to_string(FitStd)).c_str());
//            FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 8)).c_str());
//            FitParam->AddText(("Fit mean = " + to_string_with_precision(FitMean, 8)).c_str());
//            FitParam->AddText(("Fit std = " + to_string_with_precision(FitStd, 8)).c_str());
            ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
            FitParam->Draw("same");

            int SliceUpperLimPrecision;
            if (ResSlicesLimits.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

            string hSlice_CloneSaveDir = ResSlices.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_res_slices/";
            string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + ResSlices.at(i).GetHistogram1DSaveName() + "_fitted.png";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

            FittedNeutronResSlices->Add(hSlice);

            SliceFitCanvas->Clear();
            delete SliceFitCanvas;
        } else {
            continue;
        }
    }

    if (Particle == "Neutron") {
        //TODO: figure out if this should be here
        FitterPol1_Corr();
    }
}
//</editor-fold>

// FitterPol1 functions -------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="FitterPol1_Corr function">
void NeutronResolution::FitterPol1_Corr() {

    //<editor-fold desc="Setting plot x and y data">
    double MeanPn[NumberOfSlices], Pn_Corr[NumberOfSlices];

    for (int i = 0; i < NumberOfSlices; i++) {
        MeanPn[i] = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        Pn_Corr[i] = ResSlicesFitVar.at(i).GetMean();
//        Pn_Std[i] = ResSlicesFitVar.at(i).GetUpperCut();
    }
    //</editor-fold>

    Canvas_Corr_pol1 = new TCanvas("Canvas_Corr_pol1", "Canvas_Corr_pol1", 1000, 750);
    Canvas_Corr_pol1->SetGrid();
    Canvas_Corr_pol1->SetBottomMargin(0.14);
    Canvas_Corr_pol1->SetLeftMargin(0.16);
    Canvas_Corr_pol1->SetRightMargin(0.12);

    g_Corr_pol1 = new TGraph((sizeof(MeanPn) / sizeof(double)), MeanPn, Pn_Corr);
    g_Corr_pol1->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol1->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol1->GetXaxis()->CenterTitle(true);
    g_Corr_pol1->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
//    g_Corr_pol1->GetXaxis()->SetTitle("P_{n} slice mean [GeV/c]");
    g_Corr_pol1->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol1->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol1->GetYaxis()->CenterTitle(true);
    g_Corr_pol1->GetYaxis()->SetTitle("Fitted R_{n} mean (%)");
//    g_Corr_pol1->GetYaxis()->SetTitle("P_{n} correction (%)");
    g_Corr_pol1->SetTitle("Neutron currection vs. #bar{P}_{n}");

//    g_Corr_pol1->SetMaximum(MeanPn[NumberOfSlices] * 1.1);
//    g_Corr_pol1->SetMinimum(MeanPn[0] * 0.9);

    f_Corr_pol1 = new TF1("f_Corr_pol1", "[0] * x + [1]"); // A*x + B

    g_Corr_pol1->Fit(f_Corr_pol1);
    g_Corr_pol1->Draw("AL");

    double A_Corr_pol1 = f_Corr_pol1->GetParameter(0); // get [0]
    double B_Corr_pol1 = f_Corr_pol1->GetParameter(1); // get [1]

    double x_1_Corr_legend = gStyle->GetStatX(), y_1_Corr_legend = gStyle->GetStatY() - 0.2;
    double x_2_Corr_legend = gStyle->GetStatX() - 0.2, y_2_Corr_legend = gStyle->GetStatY() - 0.3;
    double x_1_FitParam = x_1_Corr_legend, y_1_FitParam = y_1_Corr_legend;
    double x_2_FitParam = x_2_Corr_legend, y_2_FitParam = y_2_Corr_legend;

    auto Corr_pol1_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.05, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
//    auto Corr_pol1_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
    TLegendEntry *Corr_pol1_legend_fit = Corr_pol1_legend->AddEntry(f_Corr_pol1, "f(#bar{P}_{n}) = A#bar{P}_{n} + B", "l");
    Corr_pol1_legend->SetTextFont(12);
    Corr_pol1_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetTextFont(0);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("Fit A = " + to_string(A_Corr_pol1)).c_str());
    FitParam->AddText(("Fit B = " + to_string(B_Corr_pol1)).c_str());
    FitParam->Draw("same");

    Canvas_Corr_pol1->SaveAs("TEST_Corr_pol1.png");

//    exit(0);
}
//</editor-fold>

////<editor-fold desc="FitterPol1_Corr function">
//void NeutronResolution::FitterPol1_Std() {
//    double MeanPn[NumberOfSlices], Pn_Corr[NumberOfSlices], Pn_Std[NumberOfSlices], y[NumberOfSlices];
//
//    for (int i = 0; i < NumberOfSlices; i++) {
//        MeanPn[i] = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
//        Pn_Corr[i] = ResSlicesFitVar.at(i).GetMean();
//        Pn_Std[i] = ResSlicesFitVar.at(i).GetUpperCut();
//    }
//
//    TGraph *g_Corr = new TGraph((sizeof(MeanPn) / sizeof(double)), MeanPn, Pn_Corr);
//    TGraph *g_Std = new TGraph((sizeof(MeanPn) / sizeof(double)), MeanPn, Pn_Std);
//
//    TF1 *f_Corr = new TF1("f_Corr", "[0] * x + [1]"); // A*x + B
//    TF1 *f_Std = new TF1("f_Std", "[0] * x + [1]"); // A*x + B
//
//    g_Corr->Fit(f_Corr);
//    g_Std->Fit(f_Std);
//
//    g_Corr->Draw("AL");
//
//    double A_Corr_pol1 = f_Corr->GetParameter(0); // get [0]
//    double B_Corr_pol1 = f_Corr->GetParameter(1); // get [1]
//    double C_Corr = f_Corr->GetParameter(2); // get [2]
//    double A_Std = f_Std->GetParameter(0); // get [0]
//    double B_Std = f_Std->GetParameter(1); // get [1]
//    double C_Std = f_Std->GetParameter(2); // get [2]
//
//    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
//    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
//
//    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
//    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;
//
//    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
//    FitParam->SetBorderSize(1);
//    FitParam->SetTextFont(0);
//    FitParam->SetFillColor(0);
//    FitParam->SetTextAlign(12);
//    FitParam->AddText(("Fit A = " + to_string(A)).c_str());
//    FitParam->AddText(("Fit B = " + to_string(B)).c_str());
//    FitParam->AddText(("Fit C = " + to_string(C)).c_str());
////    FitParam->AddText(("Fit A = " + to_string_with_precision(A, 8)).c_str());
////    FitParam->AddText(("Fit B = " + to_string_with_precision(B, 8)).c_str());
////    FitParam->AddText(("Fit C = " + to_string_with_precision(C, 8)).c_str());
////    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
//    FitParam->Draw("same");
//
////    exit(0);
//    }
////</editor-fold>

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveResSlices function">
void NeutronResolution::DrawAndSaveResSlices(const string &SampleName, const string &Particle, TCanvas *h1DCanvas, const string &plots_path,
                                             const string &NeutronResolutionDirectory) {
    string SampleNameTemp = SampleName;

    ResSlicePlots->Add(FittedNeutronResSlices);

    for (int i = 0; i < NumberOfSlices; i++) { ResSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, false, true, 1., 9999, 9999, 0, false); }

    /* Save res and fitted res plots to plots directory: */
    TFile *PlotsFolder_fout = new TFile((plots_path + "/" + Particle + "_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
    PlotsFolder_fout->cd();
    ResSlicePlots->Write();
    PlotsFolder_fout->Write();
    PlotsFolder_fout->Close();

//    TFile *CutsDirectory_fout = new TFile((CutsDirectory + "Neutron_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
//    CutsDirectory_fout->cd();
//    ResSlicePlots->Write();
//    CutsDirectory_fout->Write();
//    CutsDirectory_fout->Close();
}
//</editor-fold>

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogResDataToFile function">
void NeutronResolution::LogResDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    //TODO: reorder file save in test mode properly
    string SaveDateDir = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/";

    if (!nResTestMode) {
        system(("mkdir -p " + SaveDateDir).c_str());

        LogFitDataToFile(SampleName, Particle, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
        LogHistDataToFile(SampleName, Particle, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
    } else {
        LogFitDataToFile(SampleName, Particle, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
        LogHistDataToFile(SampleName, Particle, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
    }
}
//</editor-fold>

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogFitDataToFile function">
void NeutronResolution::LogFitDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {

    ofstream Neutron_res_fit_param;
    std::string Neutron_res_fit_paramFilePath;

    if (!nResTestMode) {
        Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + Particle + "_res_fit_param_-_" + SampleName + ".par";
    } else {
        Neutron_res_fit_paramFilePath = plots_path + "/" + Particle + "_res_fit_param_-_" + SampleName + ".par";
    }

    Neutron_res_fit_param.open(Neutron_res_fit_paramFilePath);
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian fit) #\n";
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# " + Particle + " resolution parameters are for:\n";
    Neutron_res_fit_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_fit_param << "#run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n";
    Neutron_res_fit_param << "\n";

    for (int FittedSlice: FittedSlices) {
        DSCuts TempCut = ResSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":"
                              << TempCut.GetSliceUpperb()
                              << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param.close();

    if (!nResTestMode) {
        /* Copy fitted parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str());
    }

    /*
    ofstream Neutron_res_fit_param;
    std::string Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + Particle + "_res_fit_param_-_" + SampleName + ".par";

    Neutron_res_fit_param.open(Neutron_res_fit_paramFilePath);
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian fit) #\n";
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# " + Particle + " resolution parameters are for:\n";
    Neutron_res_fit_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_fit_param << "#run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n";
    Neutron_res_fit_param << "\n";

    for (int FittedSlice: FittedSlices) {
        DSCuts TempCut = ResSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":" << TempCut.GetSliceUpperb()
                              << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param.close();

    system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str()); // Copy fitted parameters file to plots folder for easy download from the ifarm
*/
}
//</editor-fold>

// LogHistDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogHistDataToFile function">
void NeutronResolution::LogHistDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    ofstream Neutron_res_Hist_param;
    std::string Neutron_res_Hist_paramFilePath;

    if (!nResTestMode) {
        Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + Particle + "_res_hist_param_-_" + SampleName + ".par";
    } else {
        Neutron_res_Hist_paramFilePath = plots_path + "/" + Particle + "_res_hist_param_-_" + SampleName + ".par";
    }

    Neutron_res_Hist_param.open(Neutron_res_Hist_paramFilePath);
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian Hist) #\n";
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "\n";
    Neutron_res_Hist_param << "# " + Particle + " resolution parameters are for:\n";
    Neutron_res_Hist_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_Hist_param << "#run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_Hist_param << "#delta:\t\t" << delta << "\n";
    Neutron_res_Hist_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:HistMean:HistSigma\n";
    Neutron_res_Hist_param << "\n";

    for (DSCuts ResSlice: ResSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":"
                               << ResSlice.GetSliceUpperb()
                               << ":" << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param.close();

    if (!nResTestMode) {
        /* Copy histogram parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_Hist_paramFilePath + " " + plots_path).c_str());
    }

    /*
    ofstream Neutron_res_Hist_param;
    std::string Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + Particle + "_res_hist_param_-_" + SampleName + ".par";

    Neutron_res_Hist_param.open(Neutron_res_Hist_paramFilePath);
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian Hist) #\n";
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "\n";
    Neutron_res_Hist_param << "# " + Particle + " resolution parameters are for:\n";
    Neutron_res_Hist_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_Hist_param << "#run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_Hist_param << "#delta:\t\t" << delta << "\n";
    Neutron_res_Hist_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:HistMean:HistSigma\n";
    Neutron_res_Hist_param << "\n";

    for (DSCuts ResSlice: ResSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":"
                               << ResSlice.GetSliceUpperb()
                               << ":" << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param.close();

    system(("cp " + Neutron_res_Hist_paramFilePath + " " + plots_path).c_str()); // Copy histogram parameters file to plots folder for easy download from the ifarm
*/
}
//</editor-fold>

// ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadResDataParam function">
void NeutronResolution::ReadResDataParam(const char *filename, const string &SampleName, const string &NucleonCutsDirectory) {
    SName = SampleName;

    ifstream infile;
    infile.open(filename);
    SetUpperMomCut(SampleName, NucleonCutsDirectory);

    if (infile.is_open()) {
        string tp;

        // remove 3 lines of header
        for (int i = 0; i < 3; i++) { getline(infile, tp); }

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (findSubstring(parameter, "fit_Slice_#")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);
                string SliceParam;
                int count = 0; // parameter number

                string CutNameTemp = parameter;
                int SliceNumberTemp;
                double SliceLowerBoundaryTemp, SliceUpperBoundaryTemp, FitMeanTemp, FitSigmaTemp;

                while (getline(ss2, SliceParam, ':')) {
                    if (count == 0) {
                        SliceNumberTemp = stoi(SliceParam);
                    } else if (count == 1) {
                        SliceLowerBoundaryTemp = stod(SliceParam);
                    } else if (count == 2) {
                        SliceUpperBoundaryTemp = stod(SliceParam);
                    } else if (count == 3) {
                        FitMeanTemp = stod(SliceParam);
                    } else if (count == 4) {
                        FitSigmaTemp = stod(SliceParam);
                    }

                    count++;
                }

                DSCuts TempFitCut = DSCuts(CutNameTemp, "FD", "Neutron", "1n", FitMeanTemp, -9999, FitSigmaTemp);
                TempFitCut.SetSliceLowerb(SliceLowerBoundaryTemp);
                TempFitCut.SetSliceUpperb(SliceUpperBoundaryTemp);
                TempFitCut.SetSliceNumber(SliceNumberTemp);

                Loaded_Res_Slices_FitVar.push_back(TempFitCut);
            } else if (findSubstring(parameter, "hist_Slice_#")) {
                // get cut values
                ss >> parameter2;
                stringstream ss2(parameter2);
                string SliceParam;
                int count = 0; // parameter number

                string CutNameTemp = parameter;
                int SliceNumberTemp;
                double SliceLowerBoundaryTemp, SliceUpperBoundaryTemp, HistMeanTemp, HistSigmaTemp;

                while (getline(ss2, SliceParam, ':')) {
                    if (count == 0) {
                        SliceNumberTemp = stoi(SliceParam);
                    } else if (count == 1) {
                        SliceLowerBoundaryTemp = stod(SliceParam);
                    } else if (count == 2) {
                        SliceUpperBoundaryTemp = stod(SliceParam);
                    } else if (count == 3) {
                        HistMeanTemp = stod(SliceParam);
                    } else if (count == 4) {
                        HistSigmaTemp = stod(SliceParam);
                    }

                    count++;
                }

                DSCuts TempHistCut = DSCuts(CutNameTemp, "FD", "Neutron", "1n", HistMeanTemp, -9999, HistSigmaTemp);
                TempHistCut.SetSliceLowerb(SliceLowerBoundaryTemp);
                TempHistCut.SetSliceUpperb(SliceUpperBoundaryTemp);
                TempHistCut.SetSliceNumber(SliceNumberTemp);

                Loaded_Res_Slices_HistVar.push_back(TempHistCut);
            }
        }
    } else {
        cout << "\n\nNeutronResolution::ReadResDataParam: file not found! Exiting...\n\n", exit(0);
    }
}
//</editor-fold>

// PSmear function ------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="PSmear function">
/* A function to smear protons by fitted neutron resolution */

double NeutronResolution::PSmear(bool apply_nucleon_SmearAndShift, double Momentum) {
    bool Printout = false;

    if (!apply_nucleon_SmearAndShift) {
        return Momentum;
    } else {
        if (SmearMode == "slices") {

            //<editor-fold desc="Original (smearing from loaded nRes fit variables)">
            if (Momentum < SliceUpperMomLim) { // NOTE: changed according to upper neutron mom. th.
//            if (Momentum <= SliceUpperMomLim) { // NOTE: changed according to upper neutron mom. th.
                for (DSCuts Loaded_res_slice: Loaded_Res_Slices_FitVar) {
                    if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                    if ((Loaded_res_slice.GetSliceLowerb() <= Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                    if ((Loaded_res_slice.GetSliceLowerb() <= Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
                        double Smearing = Rand->Gaus(1, Loaded_res_slice.GetUpperCut());

                        if (Printout) {
                            cout << "\n\nLoaded_res_slice.GetUpperCut() = " << Loaded_res_slice.GetUpperCut() << "\n";
                            cout << "Momentum = " << Momentum << "\n";
                            cout << "Smearing = " << Smearing << "\n";
                            cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
                        }

                        return Smearing * Momentum;
                    }
                }
            } else {
                double Smearing = Rand->Gaus(1, Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut());

                if (Printout) {
                    cout << "\n\nLoaded_Res_Slices_FitVar.GetUpperCut() = " << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut() << "\n";
                    cout << "Loaded_res_slice.GetSliceLowerb() = " << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceLowerb() << "\n";
                    cout << "Loaded_res_slice.GetSliceUpperb() = " << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceUpperb() << "\n";
                    cout << "Momentum = " << Momentum << "\n";
                    cout << "Smearing = " << Smearing << "\n";
                    cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
                }

                return Smearing * Momentum;
            }

            //        //<editor-fold desc="Original smearing code by slices">
//        for (DSCuts Loaded_res_slice: Loaded_Res_Slices_FitVar) {
//            if ((Loaded_res_slice.GetSliceLowerb() <= Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                double Smearing = Rand->Gaus(1, Loaded_res_slice.GetUpperCut());
//
//                if (Printout) {
//                    cout << "\n\nLoaded_res_slice.GetUpperCut() = " << Loaded_res_slice.GetUpperCut() << "\n";
//                    cout << "Momentum = " << Momentum << "\n";
//                    cout << "Smearing = " << Smearing << "\n";
//                    cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
//                }
//
//                return Smearing * Momentum;
//            }
//        }
//        //</editor-fold>

            //</editor-fold>

        } else if ((SmearMode == "pol1") || (SmearMode == "pol3")) {
            /* Proton smear from *excel* linear fit */
            //TODO: automate the proton mom smear into this class for future uses
            double Smearing;

            if (findSubstring(SName, "C12_simulation_6GeV_T5")) { // Old sample
                if (SmearMode == "pol1") {
                    Smearing = Rand->Gaus(1, 0.0738 * Momentum - 0.0304); // new smear between 1 and 3 GeV/c
//                    Smearing = Rand->Gaus(1, 0.0583 * Momentum - 0.0045); // old smear between 0.4 to 4.09 GeV/c
                } else {
                    cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
                }
            } else { // New sample (24M, 1-foil)
                if (SmearMode == "pol1") {
                    /* nRes 1 */
//                    Smearing = Rand->Gaus(1, 0.0692 * Momentum - 0.0233); // new smear between 1 and 3 GeV/c
////                    Smearing = Rand->Gaus(1, 0.0566 * Momentum - 0.0026); // old smear between 0.4 to 4.09 GeV/c

                    /* nRes 2 */
                    Smearing = Rand->Gaus(1, 0.0885 * Momentum - 0.0382); // new smear between 1 and 3 GeV/c
                } else if (SmearMode == "pol3") {
                    double Momentum2 = Momentum * Momentum;
                    double Momentum3 = Momentum * Momentum * Momentum;

                    Smearing = Rand->Gaus(1, -0.0136 * Momentum3 + 0.0789 * Momentum2 - 0.0758 * Momentum + 0.0606); // old smear between 0.4 to 4.09 GeV/c
                } else {
                    cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
                }
            }

            double SmearedMomentum = Smearing * Momentum; // minus for protons and plus for protons

            if (Printout) {
                cout << "\n\nSmearMode = " << SmearMode << "\n";
                cout << "Smearing = " << Smearing << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "SmearedMomentum = " << SmearedMomentum << "\n\n";
            }

            return SmearedMomentum;
        } else {
            cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
        }
    }

    return Momentum;
}
//</editor-fold>

// NShift function ------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NShift function">
/* A function to shift (calibrate) neutron momentum by fitted neutron shift */

double NeutronResolution::NShift(bool apply_nucleon_SmearAndShift, double Momentum) {
    bool Printout = false;

    if (!apply_nucleon_SmearAndShift) {
        return Momentum;
    } else {
        if (ShiftMode == "slices") {

            //<editor-fold desc="Original (shift from loaded nRes fit variables)">
            for (DSCuts Loaded_res_slice: Loaded_Res_Slices_HistVar) {
                if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                if ((Loaded_res_slice.GetSliceLowerb() <= Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
                    double ShiftedMomentum = Momentum * (1 + Loaded_res_slice.GetMean()); // minus for protons and plus for neutrons

                    if (Printout) {
                        cout << "\n\nLoaded_res_slice.GetMean() = " << Loaded_res_slice.GetMean() << "\n";
                        cout << "Momentum = " << Momentum << "\n";
                        cout << "shift = " << Loaded_res_slice.GetMean() << "\n";
                        cout << "ShiftedMomentum = " << ShiftedMomentum << "\n\n";
                    }

                    return ShiftedMomentum;
                }
            }
            //</editor-fold>

        } else if ((ShiftMode == "pol1") || (ShiftMode == "pol3")) {
            /* Neutron shift from *excel* linear fit */
            //TODO: automate the neutron mom shift into this class for future uses
            //TODO: rename as correction, not shift
            double shift;

            if (findSubstring(SName, "C12_simulation_6GeV_T5")) { // Old sample
                if (ShiftMode == "pol1") {
//                    shift = 0.0579 * Momentum - 0.0146; // new shift between 1 and 3 GeV/c
                    shift = 0.0583 * Momentum - 0.0127; // old shift between 0.4 to 4.09 GeV/c
                } else {
                    cout << "\n\nNeutronResolution::NShift: ShiftMode illegal (no pol3 for C12_simulation_6GeV_T5)! Exiting...", exit(0);
                }
            } else { // New sample (24M, 1-foil)
                if (ShiftMode == "pol1") {
//                    shift = 0.068 * Momentum - 0.0256; // new shift between 1 and 3 GeV/c
                    shift = 0.0674 * Momentum - 0.0218; // old shift between 0.4 to 4.09 GeV/c
                } else if (ShiftMode == "pol3") {
                    double Momentum2 = Momentum * Momentum;
                    double Momentum3 = Momentum * Momentum * Momentum;

                    shift = -0.0017 * Momentum3 + 0.0205 * Momentum2 + 0.0081 * Momentum + 0.0219; // old shift between 0.4 to 4.09 GeV/c
                } else {
                    cout << "\n\nNeutronResolution::NShift: ShiftMode illegal! Exiting...", exit(0);
                }
            }

            double ShiftedMomentum = Momentum * (1 + shift); // minus for protons and plus for neutrons

            if (Printout) {
                cout << "\n\nShiftMode = " << ShiftMode << "\n";
                cout << "Shift = " << shift << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "ShiftedMomentum = " << ShiftedMomentum << "\n\n";
            }

            return ShiftedMomentum;
        } else {
            cout << "\n\nNeutronResolution::NShift: ShiftMode illegal! Exiting...", exit(0);
        }
    }

    return Momentum;
}
//</editor-fold>
