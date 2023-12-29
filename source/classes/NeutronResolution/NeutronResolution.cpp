//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//TODO: add a software that creates momentum slices with at least 1000 events each automatically

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const string &Particle, const double &beamE,
                                     const DSCuts &FD_nucleon_momentum_cut, double const &ParticleMomTh, bool const &Calculate_momResS2, bool const &Run_in_momResS2,
                                     const string &NeutronResolutionDirectory, const string &SavePath, double DeltaSlices, bool VaryingDelta, const string &SmearM,
                                     const string &ShiftM, bool momRes_test) {
    SliceUpperMomLimPC = FD_nucleon_momentum_cut.GetUpperCutConst(), SliceLowerMomLimPC = FD_nucleon_momentum_cut.GetLowerCutConst();
    momResS2CalcMode = Calculate_momResS2, momResS2RunMode = Run_in_momResS2;
    SlicesSavePath = SavePath;
    delta = DeltaSlices;
    SmearMode = SmearM, ShiftMode = ShiftM;
    momResTestMode = momRes_test;

    //<editor-fold desc="Safty check">
    if (momResS2CalcMode && momResS2RunMode) {
        cout << "NeutronResolution::NeutronResolution: calculating and running on momResS2 is illegal! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    double Delta = delta, SliceLowerLim = ParticleMomTh, SliceUpperLim;
    SetUpperMomCut(SampleName, NucleonCutsDirectory);

    if (!VaryingDelta) {
        SliceUpperLim = SliceLowerLim + delta;
    } else {
        if (beamE == 5.98636) {
            if ((SampleName == "C12_simulation_G18_Q204_6GeV") || (SampleName == "C12x4_simulation_G18_Q204_6GeV")) {
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
            cout << "\n\nSliceLowerLim = " << SliceLowerLim << "\n";
            cout << "SliceUpperLim = " << SliceUpperLim << "\n";
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

            if (!momRes_test) {
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
            SliceAndDice = false;
        } else {
            SliceLowerLim = SliceUpperLim;

            if (VaryingDelta) {
                if (beamE == 5.98636) {
                    if (SampleName == "C12_simulation_G18_Q204_6GeV") {
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
                    } else if (SampleName == "C12x4_simulation_G18_Q204_6GeV") { // Slices by option 2
                        if ((SliceLowerLim >= 0.40) && (SliceLowerLim < 0.60)) { // 0.4-0.65
                            Delta = delta * 5;
                        } else if ((SliceLowerLim >= 0.60) && (SliceLowerLim < 1.10)) { // 0.65-1.15
                            Delta = delta * 2;
                        } else if ((SliceLowerLim >= 1.10) && (SliceLowerLim < 1.55)) { // 1.15-1.55
                            Delta = delta * 1;
                        } else if ((SliceLowerLim >= 1.55) && (SliceLowerLim < 2.25)) { // 1.55-2.25
                            Delta = delta * 2;
                        } else if ((SliceLowerLim >= 2.25) && (SliceLowerLim < 2.55)) { // 2.25-2.55
                            Delta = delta * 3;
                        } else if ((SliceLowerLim >= 2.55) && (SliceLowerLim < 2.75)) { // 2.55-2.75
                            Delta = delta * 4;
                        } else if ((SliceLowerLim >= 2.75) && (SliceLowerLim < 3.00)) { // 2.75-3.00
                            Delta = delta * 5;
                        } else if (SliceLowerLim >= 3.00) { // 3.00-SliceUpperMomLim
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
            } else {
                SliceUpperLim = SliceLowerLim + Delta;
            }
        }
    }

    if (LimitsPrintOut && LimitsPrintOutAndExit) { exit(0); }

    NumberOfSlices = SliceNumber;

    if (Particle == "Neutron") {
        if (momResS2CalcMode && !momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 calculation mode. Loading momResS1 variables...\n";

            ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(),
                             Calculate_momResS2, SampleName, NucleonCutsDirectory);

            cout << "Done.\n";
        } else if (!momResS2CalcMode && momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 run mode.\n";
            cout << "Loading correction from momResS1 variables & smearing from momResS2 variables...\n";

            /* Load neutron correction */
            ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par").c_str(),
                             Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            /* Load proton smearing */
            ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par").c_str(),
                             Calculate_momResS2, SampleName, NucleonCutsDirectory, false, true);
        }
    }
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
    TCanvas *SliceFitCanvas = new TCanvas("SliceFitCanvas", "SliceFitCanvas", 1000, 750); // normal res
    SliceFitCanvas->SetGrid();
    SliceFitCanvas->SetBottomMargin(0.14);
    SliceFitCanvas->SetLeftMargin(0.18);
    SliceFitCanvas->SetRightMargin(0.12);
    SliceFitCanvas->cd();

    for (int i = 0; i < NumberOfSlices; i++) {

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
            cout << "\n\n";

            double FitUlim, FitLlim;

            if (momResTestMode) { // In smear & shift test mode
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

            if (momResTestMode) { // In smear & shift test mode
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
            double FitStd = func->GetParameter(2);  // get p2

            ResSlicesFitVar.at(i).SetMean(FitMean); // For neutron correction
            ResSlicesFitVar.at(i).SetUpperCut(FitStd); // For proton smearing
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

            FittedSlices.push_back(i); // Log slices that were fitted
        } else {
            continue;
        }
    }

    if (Particle == "Neutron") {
        //TODO: figure out if this should be here
        Fitter_Std_pol1(), Fitter_Std_pol2(), Fitter_Std_pol3();
        Fitter_Std_pol1_wPC(), Fitter_Std_pol2_wPC(), Fitter_Std_pol3_wPC();
        Fitter_Corr_pol1(), Fitter_Corr_pol2(), Fitter_Corr_pol3();
        Fitter_Corr_pol1_wPC(), Fitter_Corr_pol2_wPC(), Fitter_Corr_pol3_wPC();
    }
}
//</editor-fold>

// Fitter functions -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Fitter functions">
void NeutronResolution::Fitter_Std_pol1() {
    cout << "\n\nFitter_Std_pol1 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Std = ResSlicesFitVar.at(i).GetUpperCut();

        MeanPn.push_back(Mean);
        Pn_Std.push_back(Std); //TODO: add a mechanism to ignore failed fits

        if (PrintOut) {
            cout << "\nMean = " << Mean << "\n";
            cout << "Std = " << Std << "\n";
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Std.size() = " << Pn_Std.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol1: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol1 = new TGraph();
    g_Std_pol1->SetName("g_Std_pol1");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol1->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol1->GetListOfFunctions()->Add(latex);
        }
    }

    g_Std_pol1->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol1->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol1->GetXaxis()->CenterTitle(true);
    g_Std_pol1->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Std_pol1->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol1->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol1->GetYaxis()->CenterTitle(true);
    g_Std_pol1->GetYaxis()->SetTitle("R_{n} fit width");
    g_Std_pol1->SetTitle("Proton smear fit (noPC, linear fit)");

    TF1 *f_Std_pol1 = new TF1("f_Std_pol1", "[0] * x + [1]"); // A*x + B

    g_Std_pol1->Fit(f_Std_pol1);
    g_Std_pol1->Draw("ap");
    g_Std_pol1->SetMarkerStyle(21);

    A_Std_pol1 = f_Std_pol1->GetParameter(0); // get [0]
    A_Std_pol1_Error = f_Std_pol1->GetParError(0); // get [0]
    B_Std_pol1 = f_Std_pol1->GetParameter(1); // get [1]
    B_Std_pol1_Error = f_Std_pol1->GetParError(1); // get [1]
    ChiSquare_Std_pol1 = f_Std_pol1->GetChisquare(); // ChiSquare
    NDF_Std_pol1 = f_Std_pol1->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025;

    auto Std_pol1_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol1_legend_fit = Std_pol1_legend->AddEntry(f_Std_pol1, "f(#bar{P}_{n}) = A#bar{P}_{n} + B", "l");
    Std_pol1_legend->SetTextFont(12);
    Std_pol1_legend->SetTextSize(0.03);
    Std_pol1_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Std_pol1) + " #pm " + to_string(A_Std_pol1_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Std_pol1) + " #pm " + to_string(B_Std_pol1_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol1 / NDF_Std_pol1)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol1->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol1_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesWidth->Add(g_Std_pol1);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "01_Fit_Std_pol1.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol1_wPC() {
    cout << "\n\nFitter_Std_pol1_wPC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Std = ResSlicesFitVar.at(i).GetUpperCut();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimPC) && (Mean <= SliceUpperMomLimPC)) {
            MeanPn.push_back(Mean);
            Pn_Std.push_back(Std); //TODO: add a mechanism to ignore failed fits

            if (PrintOut) {
                cout << "Mean = " << Mean << "\n";
                cout << "Std = " << Std << "\n";
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Std.size() = " << Pn_Std.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol1_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol1_wPC = new TGraph();
    g_Std_pol1_wPC->SetName("g_Std_pol1_wPC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol1_wPC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol1_wPC->GetListOfFunctions()->Add(latex);
        }
    }

    g_Std_pol1_wPC->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol1_wPC->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol1_wPC->GetXaxis()->CenterTitle(true);
    g_Std_pol1_wPC->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Std_pol1_wPC->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol1_wPC->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol1_wPC->GetYaxis()->CenterTitle(true);
    g_Std_pol1_wPC->GetYaxis()->SetTitle("R_{n} fit width");
    g_Std_pol1_wPC->SetTitle("Proton smear fit (wPC, linear fit)");

    TF1 *f_Std_pol1_wPC = new TF1("f_Std_pol1_wPC", "[0] * x + [1]"); // A*x + B

    g_Std_pol1_wPC->Fit(f_Std_pol1_wPC);
    g_Std_pol1_wPC->Draw("ap");
    g_Std_pol1_wPC->SetMarkerStyle(21);

    A_Std_pol1_wPC = f_Std_pol1_wPC->GetParameter(0); // get [0]
    A_Std_pol1_wPC_Error = f_Std_pol1_wPC->GetParError(0); // get [0]
    B_Std_pol1_wPC = f_Std_pol1_wPC->GetParameter(1); // get [1]
    B_Std_pol1_wPC_Error = f_Std_pol1_wPC->GetParError(1); // get [1]
    ChiSquare_Std_pol1_wPC = f_Std_pol1_wPC->GetChisquare(); // ChiSquare
    NDF_Std_pol1_wPC = f_Std_pol1_wPC->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025;

    auto Std_pol1_wPC_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol1_wPC_legend_fit = Std_pol1_wPC_legend->AddEntry(f_Std_pol1_wPC, "f(#bar{P}_{n}) = A#bar{P}_{n} + B", "l");
    Std_pol1_wPC_legend->SetTextFont(12);
    Std_pol1_wPC_legend->SetTextSize(0.03);
    Std_pol1_wPC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Std_pol1_wPC) + " #pm " + to_string(A_Std_pol1_wPC_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Std_pol1_wPC) + " #pm " + to_string(B_Std_pol1_wPC_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol1_wPC / NDF_Std_pol1_wPC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol1_wPC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol1_wPC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesWidth->Add(g_Std_pol1_wPC);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "01_Fit_Std_pol1_wPC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol2() {
    cout << "\n\nFitter_Std_pol2 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Std = ResSlicesFitVar.at(i).GetUpperCut();

        MeanPn.push_back(Mean);
        Pn_Std.push_back(Std); //TODO: add a mechanism to ignore failed fits

        if (PrintOut) {
            cout << "\nMean = " << Mean << "\n";
            cout << "Std = " << Std << "\n";
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Std.size() = " << Pn_Std.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol2: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol2 = new TGraph();
    g_Std_pol2->SetName("g_Std_pol2");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol2->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol2->GetListOfFunctions()->Add(latex);
        }
    }

    g_Std_pol2->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol2->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol2->GetXaxis()->CenterTitle(true);
    g_Std_pol2->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Std_pol2->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol2->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol2->GetYaxis()->CenterTitle(true);
    g_Std_pol2->GetYaxis()->SetTitle("R_{n} fit width");
    g_Std_pol2->SetTitle("Proton smear fit (noPC, quadratic fit)");

    TF1 *f_Std_pol2 = new TF1("f_Std_pol2", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g_Std_pol2->Fit(f_Std_pol2);
    g_Std_pol2->Draw("ap");
    g_Std_pol2->SetMarkerStyle(21);

    A_Std_pol2 = f_Std_pol2->GetParameter(0); // get [0]
    A_Std_pol2_Error = f_Std_pol2->GetParError(0); // get [0]
    B_Std_pol2 = f_Std_pol2->GetParameter(1); // get [1]
    B_Std_pol2_Error = f_Std_pol2->GetParError(1); // get [1]
    C_Std_pol2 = f_Std_pol2->GetParameter(2); // get [2]
    C_Std_pol2_Error = f_Std_pol2->GetParError(2); // get [2]
    ChiSquare_Std_pol2 = f_Std_pol2->GetChisquare(); // ChiSquare
    NDF_Std_pol2 = f_Std_pol2->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std + 0.1, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025 - 0.05;

    auto Std_pol2_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol2_legend_fit = Std_pol2_legend->AddEntry(f_Std_pol2, "f(#bar{P}_{n}) = A#bar{P}_{n}^{2} + B#bar{P}_{n} + C", "l");
    Std_pol2_legend->SetTextFont(12);
    Std_pol2_legend->SetTextSize(0.03);
    Std_pol2_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Std_pol2) + " #pm " + to_string(A_Std_pol2_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Std_pol2) + " #pm " + to_string(B_Std_pol2_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Std_pol2) + " #pm " + to_string(C_Std_pol2_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol2 / NDF_Std_pol2)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol2->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol2_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesWidth->Add(g_Std_pol2);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "02_Fit_Std_pol2.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol2_wPC() {
    cout << "\n\nFitter_Std_pol2_wPC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Std = ResSlicesFitVar.at(i).GetUpperCut();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimPC) && (Mean <= SliceUpperMomLimPC)) {
            MeanPn.push_back(Mean);
            Pn_Std.push_back(Std); //TODO: add a mechanism to ignore failed fits

            if (PrintOut) {
                cout << "Mean = " << Mean << "\n";
                cout << "Std = " << Std << "\n";
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Std.size() = " << Pn_Std.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol2_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol2_wPC = new TGraph();
    g_Std_pol2_wPC->SetName("g_Std_pol2_wPC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol2_wPC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol2_wPC->GetListOfFunctions()->Add(latex);
        }
    }

    g_Std_pol2_wPC->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol2_wPC->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol2_wPC->GetXaxis()->CenterTitle(true);
    g_Std_pol2_wPC->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Std_pol2_wPC->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol2_wPC->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol2_wPC->GetYaxis()->CenterTitle(true);
    g_Std_pol2_wPC->GetYaxis()->SetTitle("R_{n} fit width");
    g_Std_pol2_wPC->SetTitle("Proton smear fit (wPC, quadratic fit)");

    TF1 *f_Std_pol2_wPC = new TF1("f_Std_pol2_wPC", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g_Std_pol2_wPC->Fit(f_Std_pol2_wPC);
    g_Std_pol2_wPC->Draw("ap");
    g_Std_pol2_wPC->SetMarkerStyle(21);

    A_Std_pol2_wPC = f_Std_pol2_wPC->GetParameter(0); // get [0]
    A_Std_pol2_wPC_Error = f_Std_pol2_wPC->GetParError(0); // get [0]
    B_Std_pol2_wPC = f_Std_pol2_wPC->GetParameter(1); // get [1]
    B_Std_pol2_wPC_Error = f_Std_pol2_wPC->GetParError(1); // get [1]
    C_Std_pol2_wPC = f_Std_pol2_wPC->GetParameter(2); // get [2]
    C_Std_pol2_wPC_Error = f_Std_pol2_wPC->GetParError(2); // get [2]
    ChiSquare_Std_pol2_wPC = f_Std_pol2_wPC->GetChisquare(); // ChiSquare
    NDF_Std_pol2_wPC = f_Std_pol2_wPC->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std + 0.1, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025 - 0.05;

    auto Std_pol2_wPC_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol2_wPC_legend_fit = Std_pol2_wPC_legend->AddEntry(f_Std_pol2_wPC, "f(#bar{P}_{n}) = A#bar{P}_{n}^{2} + B#bar{P}_{n} + C", "l");
    Std_pol2_wPC_legend->SetTextFont(12);
    Std_pol2_wPC_legend->SetTextSize(0.03);
    Std_pol2_wPC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Std_pol2_wPC) + " #pm " + to_string(A_Std_pol2_wPC_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Std_pol2_wPC) + " #pm " + to_string(B_Std_pol2_wPC_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Std_pol2_wPC) + " #pm " + to_string(C_Std_pol2_wPC_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol2_wPC / NDF_Std_pol2_wPC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol2_wPC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol2_wPC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesWidth->Add(g_Std_pol2_wPC);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "02_Fit_Std_pol2_wPC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol3() {
    cout << "\n\nFitter_Std_pol3 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Std = ResSlicesFitVar.at(i).GetUpperCut();

        MeanPn.push_back(Mean);
        Pn_Std.push_back(Std); //TODO: add a mechanism to ignore failed fits

        if (PrintOut) {
            cout << "\nMean = " << Mean << "\n";
            cout << "Std = " << Std << "\n";
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Std.size() = " << Pn_Std.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol3: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol3 = new TGraph();
    g_Std_pol3->SetName("g_Std_pol3");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol3->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol3->GetListOfFunctions()->Add(latex);
        }
    }

    g_Std_pol3->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol3->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol3->GetXaxis()->CenterTitle(true);
    g_Std_pol3->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Std_pol3->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol3->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol3->GetYaxis()->CenterTitle(true);
    g_Std_pol3->GetYaxis()->SetTitle("R_{n} fit width");
    g_Std_pol3->SetTitle("Proton smear fit (noPC, cubic fit)");

    TF1 *f_Std_pol3 = new TF1("f_Std_pol3", "[0] * x * x * x + [1] * x * x + [2] * x + [3]"); // A*x*x*x + B*x*x + C*x + D

    g_Std_pol3->Fit(f_Std_pol3);
    g_Std_pol3->Draw("ap");
    g_Std_pol3->SetMarkerStyle(21);

    A_Std_pol3 = f_Std_pol3->GetParameter(0); // get [0]
    A_Std_pol3_Error = f_Std_pol3->GetParError(0); // get [0]
    B_Std_pol3 = f_Std_pol3->GetParameter(1); // get [1]
    B_Std_pol3_Error = f_Std_pol3->GetParError(1); // get [1]
    C_Std_pol3 = f_Std_pol3->GetParameter(2); // get [2]
    C_Std_pol3_Error = f_Std_pol3->GetParError(2); // get [2]
    D_Std_pol3 = f_Std_pol3->GetParameter(3); // get [3]
    D_Std_pol3_Error = f_Std_pol3->GetParError(3); // get [3]
    ChiSquare_Std_pol3 = f_Std_pol3->GetChisquare(); // ChiSquare
    NDF_Std_pol3 = f_Std_pol3->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std + 0.1 + 0.075, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025 - 0.05 - 0.05;

    auto Std_pol3_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol3_legend_fit = Std_pol3_legend->AddEntry(f_Std_pol3, "f(#bar{P}_{n}) = A#bar{P}_{n}^{3} + B#bar{P}_{n}^{2} + C#bar{P}_{n} + D", "l");
    Std_pol3_legend->SetTextFont(12);
    Std_pol3_legend->SetTextSize(0.03);
    Std_pol3_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Std_pol3) + " #pm " + to_string(A_Std_pol3_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Std_pol3) + " #pm " + to_string(B_Std_pol3_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Std_pol3) + " #pm " + to_string(C_Std_pol3_Error)).c_str());
    FitParam->AddText(("D = " + to_string(D_Std_pol3) + " #pm " + to_string(D_Std_pol3_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol3 / NDF_Std_pol3)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol3->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol3_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesWidth->Add(g_Std_pol3);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "03_Fit_Std_pol3.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol3_wPC() {
    cout << "\n\nFitter_Std_pol3_wPC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Std = ResSlicesFitVar.at(i).GetUpperCut();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimPC) && (Mean <= SliceUpperMomLimPC)) {
            MeanPn.push_back(Mean);
            Pn_Std.push_back(Std); //TODO: add a mechanism to ignore failed fits

            if (PrintOut) {
                cout << "Mean = " << Mean << "\n";
                cout << "Std = " << Std << "\n";
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Std.size() = " << Pn_Std.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol3_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol3_wPC = new TGraph();
    g_Std_pol3_wPC->SetName("g_Std_pol3_wPC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol3_wPC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol3_wPC->GetListOfFunctions()->Add(latex);
        }
    }

    g_Std_pol3_wPC->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol3_wPC->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol3_wPC->GetXaxis()->CenterTitle(true);
    g_Std_pol3_wPC->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Std_pol3_wPC->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol3_wPC->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol3_wPC->GetYaxis()->CenterTitle(true);
    g_Std_pol3_wPC->GetYaxis()->SetTitle("R_{n} fit width");
    g_Std_pol3_wPC->SetTitle("Proton smear fit (wPC, cubic fit)");

    TF1 *f_Std_pol3_wPC = new TF1("f_Std_pol3_wPC", "[0] * x * x * x + [1] * x * x + [2] * x + [3]"); // A*x*x*x + B*x*x + C*x + D

    g_Std_pol3_wPC->Fit(f_Std_pol3_wPC);
    g_Std_pol3_wPC->Draw("ap");
    g_Std_pol3_wPC->SetMarkerStyle(21);

    A_Std_pol3_wPC = f_Std_pol3_wPC->GetParameter(0); // get [0]
    A_Std_pol3_wPC_Error = f_Std_pol3_wPC->GetParError(0); // get [0]
    B_Std_pol3_wPC = f_Std_pol3_wPC->GetParameter(1); // get [1]
    B_Std_pol3_wPC_Error = f_Std_pol3_wPC->GetParError(1); // get [1]
    C_Std_pol3_wPC = f_Std_pol3_wPC->GetParameter(2); // get [2]
    C_Std_pol3_wPC_Error = f_Std_pol3_wPC->GetParError(2); // get [2]
    D_Std_pol3_wPC = f_Std_pol3_wPC->GetParameter(3); // get [3]
    D_Std_pol3_wPC_Error = f_Std_pol3_wPC->GetParError(3); // get [3]
    ChiSquare_Std_pol3_wPC = f_Std_pol3_wPC->GetChisquare(); // ChiSquare
    NDF_Std_pol3_wPC = f_Std_pol3_wPC->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std + 0.1 + 0.075, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025 - 0.05 - 0.05;

    auto Std_pol3_wPC_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol3_wPC_legend_fit = Std_pol3_wPC_legend->AddEntry(f_Std_pol3_wPC, "f(#bar{P}_{n}) = A#bar{P}_{n}^{3} + B#bar{P}_{n}^{2} + C#bar{P}_{n} + D",
                                                                          "l");
    Std_pol3_wPC_legend->SetTextFont(12);
    Std_pol3_wPC_legend->SetTextSize(0.03);
    Std_pol3_wPC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Std_pol3_wPC) + " #pm " + to_string(A_Std_pol3_wPC_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Std_pol3_wPC) + " #pm " + to_string(B_Std_pol3_wPC_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Std_pol3_wPC) + " #pm " + to_string(C_Std_pol3_wPC_Error)).c_str());
    FitParam->AddText(("D = " + to_string(D_Std_pol3_wPC) + " #pm " + to_string(D_Std_pol3_wPC_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol3_wPC / NDF_Std_pol3_wPC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol3_wPC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol3_wPC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesWidth->Add(g_Std_pol3_wPC);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "03_Fit_Std_pol3_wPC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol1() {
    cout << "\n\nFitter_Corr_pol1 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResSlicesFitVar.at(i).GetMean();

        MeanPn.push_back(Mean);
        Pn_Corr.push_back(Corr); //TODO: add a mechanism to ignore failed fits

        if (PrintOut) {
            cout << "\nMean = " << Mean << "\n";
            cout << "Corr = " << Corr << "\n";
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Corr.size() = " << Pn_Corr.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol1_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol1 = new TGraph();
    g_Corr_pol1->SetName("g_Corr_pol1");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol1->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol1->GetListOfFunctions()->Add(latex);
        }
    }

    g_Corr_pol1->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol1->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol1->GetXaxis()->CenterTitle(true);
    g_Corr_pol1->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Corr_pol1->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol1->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol1->GetYaxis()->CenterTitle(true);
    g_Corr_pol1->GetYaxis()->SetTitle("R_{n} fit mean");
    g_Corr_pol1->SetTitle("Neutron correction fit (noPC, linear fit)");

    TF1 *f_Corr_pol1 = new TF1("f_Corr_pol1", "[0] * x + [1]"); // A*x + B

    g_Corr_pol1->Fit(f_Corr_pol1);
    g_Corr_pol1->Draw("ap");
    g_Corr_pol1->SetMarkerStyle(21);

    A_Corr_pol1 = f_Corr_pol1->GetParameter(0); // get [0]
    A_Corr_pol1_Error = f_Corr_pol1->GetParError(0); // get [0]
    B_Corr_pol1 = f_Corr_pol1->GetParameter(1); // get [1]
    B_Corr_pol1_Error = f_Corr_pol1->GetParError(1); // get [1]
    ChiSquare_Corr_pol1 = f_Corr_pol1->GetChisquare(); // ChiSquare
    NDF_Corr_pol1 = f_Corr_pol1->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025;

    auto Corr_pol1_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol1_legend_fit = Corr_pol1_legend->AddEntry(f_Corr_pol1, "f(#bar{P}_{n}) = A#bar{P}_{n} + B", "l");
    Corr_pol1_legend->SetTextFont(12);
    Corr_pol1_legend->SetTextSize(0.03);
    Corr_pol1_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Corr_pol1) + " #pm " + to_string(A_Corr_pol1_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Corr_pol1) + " #pm " + to_string(B_Corr_pol1_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol1 / NDF_Corr_pol1)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol1->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol1_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesMean->Add(g_Corr_pol1);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "04_Fit_Corr_pol1.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol1_wPC() {
    cout << "\n\nFitter_Corr_pol1_wPC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResSlicesFitVar.at(i).GetMean();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimPC) && (Mean <= SliceUpperMomLimPC)) {
            MeanPn.push_back(Mean);
            Pn_Corr.push_back(Corr); //TODO: add a mechanism to ignore failed fits

            if (PrintOut) {
                cout << "Mean = " << Mean << "\n";
                cout << "Corr = " << Corr << "\n";
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Corr.size() = " << Pn_Corr.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol1_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol1_wPC = new TGraph();
    g_Corr_pol1_wPC->SetName("g_Corr_pol1_wPC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol1_wPC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol1_wPC->GetListOfFunctions()->Add(latex);
        }
    }

    g_Corr_pol1_wPC->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol1_wPC->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol1_wPC->GetXaxis()->CenterTitle(true);
    g_Corr_pol1_wPC->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Corr_pol1_wPC->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol1_wPC->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol1_wPC->GetYaxis()->CenterTitle(true);
    g_Corr_pol1_wPC->GetYaxis()->SetTitle("R_{n} fit mean");
    g_Corr_pol1_wPC->SetTitle("Neutron correction fit (wPC, linear fit)");

    TF1 *f_Corr_pol1_wPC = new TF1("f_Corr_pol1_wPC", "[0] * x + [1]"); // A*x + B

    g_Corr_pol1_wPC->Fit(f_Corr_pol1_wPC);
    g_Corr_pol1_wPC->Draw("ap");
    g_Corr_pol1_wPC->SetMarkerStyle(21);

    A_Corr_pol1_wPC = f_Corr_pol1_wPC->GetParameter(0); // get [0]
    A_Corr_pol1_wPC_Error = f_Corr_pol1_wPC->GetParError(0); // get [0]
    B_Corr_pol1_wPC = f_Corr_pol1_wPC->GetParameter(1); // get [1]
    B_Corr_pol1_wPC_Error = f_Corr_pol1_wPC->GetParError(1); // get [1]
    ChiSquare_Corr_pol1_wPC = f_Corr_pol1_wPC->GetChisquare(); // ChiSquare
    NDF_Corr_pol1_wPC = f_Corr_pol1_wPC->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025;

    auto Corr_pol1_wPC_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol1_wPC_legend_fit = Corr_pol1_wPC_legend->AddEntry(f_Corr_pol1_wPC, "f(#bar{P}_{n}) = A#bar{P}_{n} + B", "l");
    Corr_pol1_wPC_legend->SetTextFont(12);
    Corr_pol1_wPC_legend->SetTextSize(0.03);
    Corr_pol1_wPC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Corr_pol1_wPC) + " #pm " + to_string(A_Corr_pol1_wPC_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Corr_pol1_wPC) + " #pm " + to_string(B_Corr_pol1_wPC_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol1_wPC / NDF_Corr_pol1_wPC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol1_wPC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol1_wPC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesMean->Add(g_Corr_pol1_wPC);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "04_Fit_Corr_pol1_wPC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol2() {
    cout << "\n\nFitter_Corr_pol2 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResSlicesFitVar.at(i).GetMean();

        MeanPn.push_back(Mean);
        Pn_Corr.push_back(Corr); //TODO: add a mechanism to ignore failed fits

        if (PrintOut) {
            cout << "\nMean = " << Mean << "\n";
            cout << "Corr = " << Corr << "\n";
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Corr.size() = " << Pn_Corr.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol2: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol2 = new TGraph();
    g_Corr_pol2->SetName("g_Corr_pol2");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol2->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol2->GetListOfFunctions()->Add(latex);
        }
    }

    g_Corr_pol2->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol2->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol2->GetXaxis()->CenterTitle(true);
    g_Corr_pol2->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Corr_pol2->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol2->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol2->GetYaxis()->CenterTitle(true);
    g_Corr_pol2->GetYaxis()->SetTitle("R_{n} fit mean");
    g_Corr_pol2->SetTitle("Neutron correction fit (noPC, quadratic fit)");

    TF1 *f_Corr_pol2 = new TF1("f_Corr_pol2", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g_Corr_pol2->Fit(f_Corr_pol2);
    g_Corr_pol2->Draw("ap");
    g_Corr_pol2->SetMarkerStyle(21);

    A_Corr_pol2 = f_Corr_pol2->GetParameter(0); // get [0]
    A_Corr_pol2_Error = f_Corr_pol2->GetParError(0); // get [0]
    B_Corr_pol2 = f_Corr_pol2->GetParameter(1); // get [1]
    B_Corr_pol2_Error = f_Corr_pol2->GetParError(1); // get [1]
    C_Corr_pol2 = f_Corr_pol2->GetParameter(2); // get [2]
    C_Corr_pol2_Error = f_Corr_pol2->GetParError(2); // get [2]
    ChiSquare_Corr_pol2 = f_Corr_pol2->GetChisquare(); // ChiSquare
    NDF_Corr_pol2 = f_Corr_pol2->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr + 0.1, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025 - 0.05;

    auto Corr_pol2_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol2_legend_fit = Corr_pol2_legend->AddEntry(f_Corr_pol2, "f(#bar{P}_{n}) = A#bar{P}_{n}^{2} + B#bar{P}_{n} + C", "l");
    Corr_pol2_legend->SetTextFont(12);
    Corr_pol2_legend->SetTextSize(0.03);
    Corr_pol2_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Corr_pol2) + " #pm " + to_string(A_Corr_pol2_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Corr_pol2) + " #pm " + to_string(B_Corr_pol2_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Corr_pol2) + " #pm " + to_string(C_Corr_pol2_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol2 / NDF_Corr_pol2)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol2->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol2_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesMean->Add(g_Corr_pol2);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "05_Fit_Corr_pol2.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol2_wPC() {
    cout << "\n\nFitter_Corr_pol2_wPC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResSlicesFitVar.at(i).GetMean();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimPC) && (Mean <= SliceUpperMomLimPC)) {
            MeanPn.push_back(Mean);
            Pn_Corr.push_back(Corr); //TODO: add a mechanism to ignore failed fits

            if (PrintOut) {
                cout << "Mean = " << Mean << "\n";
                cout << "Corr = " << Corr << "\n";
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Corr.size() = " << Pn_Corr.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol2_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol2_wPC = new TGraph();
    g_Corr_pol2_wPC->SetName("g_Corr_pol2_wPC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol2_wPC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol2_wPC->GetListOfFunctions()->Add(latex);
        }
    }

    g_Corr_pol2_wPC->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol2_wPC->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol2_wPC->GetXaxis()->CenterTitle(true);
    g_Corr_pol2_wPC->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Corr_pol2_wPC->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol2_wPC->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol2_wPC->GetYaxis()->CenterTitle(true);
    g_Corr_pol2_wPC->GetYaxis()->SetTitle("R_{n} fit mean");
    g_Corr_pol2_wPC->SetTitle("Neutron correction fit (wPC, quadratic fit)");

    TF1 *f_Corr_pol2_wPC = new TF1("f_Corr_pol2_wPC", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g_Corr_pol2_wPC->Fit(f_Corr_pol2_wPC);
    g_Corr_pol2_wPC->Draw("ap");
    g_Corr_pol2_wPC->SetMarkerStyle(21);

    A_Corr_pol2_wPC = f_Corr_pol2_wPC->GetParameter(0); // get [0]
    A_Corr_pol2_wPC_Error = f_Corr_pol2_wPC->GetParError(0); // get [0]
    B_Corr_pol2_wPC = f_Corr_pol2_wPC->GetParameter(1); // get [1]
    B_Corr_pol2_wPC_Error = f_Corr_pol2_wPC->GetParError(1); // get [1]
    C_Corr_pol2_wPC = f_Corr_pol2_wPC->GetParameter(2); // get [2]
    C_Corr_pol2_wPC_Error = f_Corr_pol2_wPC->GetParError(2); // get [2]
    ChiSquare_Corr_pol2_wPC = f_Corr_pol2_wPC->GetChisquare(); // ChiSquare
    NDF_Corr_pol2_wPC = f_Corr_pol2_wPC->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr + 0.1, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025 - 0.05;

    auto Corr_pol2_wPC_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol2_wPC_legend_fit = Corr_pol2_wPC_legend->AddEntry(f_Corr_pol2_wPC, "f(#bar{P}_{n}) = A#bar{P}_{n}^{2} + B#bar{P}_{n} + C", "l");
    Corr_pol2_wPC_legend->SetTextFont(12);
    Corr_pol2_wPC_legend->SetTextSize(0.03);
    Corr_pol2_wPC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Corr_pol2_wPC) + " #pm " + to_string(A_Corr_pol2_wPC_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Corr_pol2_wPC) + " #pm " + to_string(B_Corr_pol2_wPC_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Corr_pol2_wPC) + " #pm " + to_string(C_Corr_pol2_wPC_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol2_wPC / NDF_Corr_pol2_wPC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol2_wPC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol2_wPC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesMean->Add(g_Corr_pol2_wPC);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "05_Fit_Corr_pol2_wPC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol3() {
    cout << "\n\nFitter_Corr_pol3 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResSlicesFitVar.at(i).GetMean();

        MeanPn.push_back(Mean);
        Pn_Corr.push_back(Corr); //TODO: add a mechanism to ignore failed fits

        if (PrintOut) {
            cout << "\nMean = " << Mean << "\n";
            cout << "Corr = " << Corr << "\n";
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Corr.size() = " << Pn_Corr.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol3: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol3 = new TGraph();
    g_Corr_pol3->SetName("g_Corr_pol3");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol3->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol3->GetListOfFunctions()->Add(latex);
        }
    }

    g_Corr_pol3->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol3->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol3->GetXaxis()->CenterTitle(true);
    g_Corr_pol3->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Corr_pol3->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol3->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol3->GetYaxis()->CenterTitle(true);
    g_Corr_pol3->GetYaxis()->SetTitle("R_{n} fit mean");
    g_Corr_pol3->SetTitle("Neutron correction fit (noPC, cubic fit)");

    TF1 *f_Corr_pol3 = new TF1("f_Corr_pol3", "[0] * x * x * x + [1] * x * x + [2] * x + [3]"); // A*x*x*x + B*x*x + C*x + D

    g_Corr_pol3->Fit(f_Corr_pol3);
    g_Corr_pol3->Draw("ap");
    g_Corr_pol3->SetMarkerStyle(21);

    A_Corr_pol3 = f_Corr_pol3->GetParameter(0); // get [0]
    A_Corr_pol3_Error = f_Corr_pol3->GetParError(0); // get [0]
    B_Corr_pol3 = f_Corr_pol3->GetParameter(1); // get [1]
    B_Corr_pol3_Error = f_Corr_pol3->GetParError(1); // get [1]
    C_Corr_pol3 = f_Corr_pol3->GetParameter(2); // get [2]
    C_Corr_pol3_Error = f_Corr_pol3->GetParError(2); // get [2]
    D_Corr_pol3 = f_Corr_pol3->GetParameter(3); // get [3]
    D_Corr_pol3_Error = f_Corr_pol3->GetParError(3); // get [3]
    ChiSquare_Corr_pol3 = f_Corr_pol3->GetChisquare(); // ChiSquare
    NDF_Corr_pol3 = f_Corr_pol3->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr + 0.1 + 0.075, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025 - 0.05 - 0.05;

    auto Corr_pol3_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol3_legend_fit = Corr_pol3_legend->AddEntry(f_Corr_pol3, "f(#bar{P}_{n}) = A#bar{P}_{n}^{3} + B#bar{P}_{n}^{2} + C#bar{P}_{n} + D", "l");
    Corr_pol3_legend->SetTextFont(12);
    Corr_pol3_legend->SetTextSize(0.03);
    Corr_pol3_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Corr_pol3) + " #pm " + to_string(A_Corr_pol3_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Corr_pol3) + " #pm " + to_string(B_Corr_pol3_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Corr_pol3) + " #pm " + to_string(C_Corr_pol3_Error)).c_str());
    FitParam->AddText(("D = " + to_string(D_Corr_pol3) + " #pm " + to_string(D_Corr_pol3_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol3 / NDF_Corr_pol3)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol3->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol3_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesMean->Add(g_Corr_pol3);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "06_Fit_Corr_pol3.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol3_wPC() {
    cout << "\n\nFitter_Corr_pol3_wPC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResSlicesFitVar.at(i).GetMean();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimPC) && (Mean <= SliceUpperMomLimPC)) {
            MeanPn.push_back(Mean);
            Pn_Corr.push_back(Corr); //TODO: add a mechanism to ignore failed fits

            if (PrintOut) {
                cout << "Mean = " << Mean << "\n";
                cout << "Corr = " << Corr << "\n";
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_Corr.size() = " << Pn_Corr.size() << "\n\n";
    }

    //<editor-fold desc="Safty check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol3_wPC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol3_wPC = new TGraph();
    g_Corr_pol3_wPC->SetName("g_Corr_pol3_wPC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol3_wPC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol3_wPC->GetListOfFunctions()->Add(latex);
        }
    }

    g_Corr_pol3_wPC->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol3_wPC->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol3_wPC->GetXaxis()->CenterTitle(true);
    g_Corr_pol3_wPC->GetXaxis()->SetTitle("#bar{P}_{n} [GeV/c]");
    g_Corr_pol3_wPC->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol3_wPC->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol3_wPC->GetYaxis()->CenterTitle(true);
    g_Corr_pol3_wPC->GetYaxis()->SetTitle("R_{n} fit mean");
    g_Corr_pol3_wPC->SetTitle("Neutron correction fit (wPC, cubic fit)");

    TF1 *f_Corr_pol3_wPC = new TF1("f_Corr_pol3_wPC", "[0] * x * x * x + [1] * x * x + [2] * x + [3]"); // A*x*x*x + B*x*x + C*x + D

    g_Corr_pol3_wPC->Fit(f_Corr_pol3_wPC);
    g_Corr_pol3_wPC->Draw("ap");
    g_Corr_pol3_wPC->SetMarkerStyle(21);

    A_Corr_pol3_wPC = f_Corr_pol3_wPC->GetParameter(0); // get [0]
    A_Corr_pol3_wPC_Error = f_Corr_pol3_wPC->GetParError(0); // get [0]
    B_Corr_pol3_wPC = f_Corr_pol3_wPC->GetParameter(1); // get [1]
    B_Corr_pol3_wPC_Error = f_Corr_pol3_wPC->GetParError(1); // get [1]
    C_Corr_pol3_wPC = f_Corr_pol3_wPC->GetParameter(2); // get [2]
    C_Corr_pol3_wPC_Error = f_Corr_pol3_wPC->GetParError(2); // get [2]
    D_Corr_pol3_wPC = f_Corr_pol3_wPC->GetParameter(3); // get [3]
    D_Corr_pol3_wPC_Error = f_Corr_pol3_wPC->GetParError(3); // get [3]
    ChiSquare_Corr_pol3_wPC = f_Corr_pol3_wPC->GetChisquare(); // ChiSquare
    NDF_Corr_pol3_wPC = f_Corr_pol3_wPC->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr + 0.1 + 0.075, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025 - 0.05 - 0.05;

    auto Corr_pol3_wPC_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol3_wPC_legend_fit = Corr_pol3_wPC_legend->AddEntry(f_Corr_pol3_wPC, "f(#bar{P}_{n}) = A#bar{P}_{n}^{3} + B#bar{P}_{n}^{2} + C#bar{P}_{n} + D",
                                                                            "l");
    Corr_pol3_wPC_legend->SetTextFont(12);
    Corr_pol3_wPC_legend->SetTextSize(0.03);
    Corr_pol3_wPC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("A = " + to_string(A_Corr_pol3_wPC) + " #pm " + to_string(A_Corr_pol3_wPC_Error)).c_str());
    FitParam->AddText(("B = " + to_string(B_Corr_pol3_wPC) + " #pm " + to_string(B_Corr_pol3_wPC_Error)).c_str());
    FitParam->AddText(("C = " + to_string(C_Corr_pol3_wPC) + " #pm " + to_string(C_Corr_pol3_wPC_Error)).c_str());
    FitParam->AddText(("D = " + to_string(D_Corr_pol3_wPC) + " #pm " + to_string(D_Corr_pol3_wPC_Error)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol3_wPC / NDF_Corr_pol3_wPC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol3_wPC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol3_wPC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    FittedNeutronResSlicesMean->Add(g_Corr_pol3_wPC);

    Fit_Canvas->SaveAs((SlicesSavePath + "/" + "06_Fit_Corr_pol3_wPC.png").c_str());
    Fit_Canvas->Clear();
}
//</editor-fold>

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveResSlices function">
void NeutronResolution::DrawAndSaveResSlices(const string &SampleName, const string &Particle, TCanvas *h1DCanvas, const string &plots_path,
                                             const string &NeutronResolutionDirectory) {
    string SampleNameTemp = SampleName;

    ResSlicePlots->Add(FittedNeutronResSlices);
    ResSlicePlots->Add(FittedNeutronResSlicesWidth);
    ResSlicePlots->Add(FittedNeutronResSlicesMean);

    for (int i = 0; i < NumberOfSlices; i++) { ResSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, false, true, 1., 9999, 9999, 0, false); }

    /* Save res and fitted res plots to plots directory: */
    TFile *PlotsFolder_fout = new TFile((plots_path + "/" + Particle + "_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
    PlotsFolder_fout->cd();
    ResSlicePlots->Write();
    PlotsFolder_fout->Write();
    PlotsFolder_fout->Close();
}
//</editor-fold>

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogResDataToFile function">
void NeutronResolution::LogResDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    //TODO: reorder file save in test mode properly
    string SaveDateDir = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/";

    if (!momResTestMode) {
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

    if (momResS2CalcMode) {
        if (!momResTestMode) {
            Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + Particle + "_momResS2_fit_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_fit_paramFilePath = plots_path + "/" + Particle + "_momResS2_fit_param_-_" + SampleName + ".par";
        }
    } else {
        if (!momResTestMode) {
            Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + Particle + "_momResS1_fit_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_fit_paramFilePath = plots_path + "/" + Particle + "_momResS1_fit_param_-_" + SampleName + ".par";
        }
    }

    //<editor-fold desc="Writing log header">
    string momRes_calculation_mode;

    if (momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS2";
    } else if (!momResS2CalcMode && momResS2RunMode) {
        momRes_calculation_mode = "Run on loaded momResS2";
    } else if (!momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS1";
    }

    Neutron_res_fit_param.open(Neutron_res_fit_paramFilePath);
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian fit) #\n";
    Neutron_res_fit_param << "######################################################################\n";
    Neutron_res_fit_param << "\n";
    Neutron_res_fit_param << "# " + Particle + " resolution parameters are for:\n";
    Neutron_res_fit_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_fit_param << "#Analysis_run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_fit_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
    //</editor-fold>

    //<editor-fold desc="Logging slice fit results">
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n\n";

    for (int FittedSlice: FittedSlices) {
        DSCuts TempCut = ResSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":"
                              << TempCut.GetSliceUpperb() << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }
    //</editor-fold>

    //<editor-fold desc="Logging Stdection and smear fit variables">
    if (Particle == "Neutron") {
        Neutron_res_fit_param << "\n\n#correction and smear fit variables:\n\n";

        Neutron_res_fit_param << "SliceUpperMomLimPC" << "\t\t\t" << SliceUpperMomLimPC << "\n";
        Neutron_res_fit_param << "SliceLowerMomLimPC" << "\t\t\t" << SliceLowerMomLimPC << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol1" << "\t\t\t\t\t" << A_Corr_pol1 << "\n";
        Neutron_res_fit_param << "A_Corr_pol1_Error" << "\t\t\t" << A_Corr_pol1_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol1" << "\t\t\t\t\t" << B_Corr_pol1 << "\n";
        Neutron_res_fit_param << "B_Corr_pol1_Error" << "\t\t\t" << B_Corr_pol1_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol1" << "\t\t\t" << ChiSquare_Corr_pol1 << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol1" << "\t\t\t\t" << NDF_Corr_pol1 << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol1_wPC" << "\t\t\t\t" << A_Corr_pol1_wPC << "\n";
        Neutron_res_fit_param << "A_Corr_pol1_wPC_Error" << "\t\t" << A_Corr_pol1_wPC_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol1_wPC" << "\t\t\t\t" << B_Corr_pol1_wPC << "\n";
        Neutron_res_fit_param << "B_Corr_pol1_wPC_Error" << "\t\t" << B_Corr_pol1_wPC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol1_wPC" << "\t\t" << ChiSquare_Corr_pol1_wPC << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol1_wPC" << "\t\t\t" << NDF_Corr_pol1_wPC << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol2" << "\t\t\t\t\t" << A_Corr_pol2 << "\n";
        Neutron_res_fit_param << "A_Corr_pol2_Error" << "\t\t\t" << A_Corr_pol2_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol2" << "\t\t\t\t\t" << B_Corr_pol2 << "\n";
        Neutron_res_fit_param << "B_Corr_pol2_Error" << "\t\t\t" << B_Corr_pol2_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol2" << "\t\t\t\t\t" << C_Corr_pol2 << "\n";
        Neutron_res_fit_param << "C_Corr_pol2_Error" << "\t\t\t" << C_Corr_pol2_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol2" << "\t\t\t" << ChiSquare_Corr_pol2 << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol2" << "\t\t\t\t" << NDF_Corr_pol2 << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol2_wPC" << "\t\t\t\t" << A_Corr_pol2_wPC << "\n";
        Neutron_res_fit_param << "A_Corr_pol2_wPC_Error" << "\t\t" << A_Corr_pol2_wPC_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol2_wPC" << "\t\t\t\t" << B_Corr_pol2_wPC << "\n";
        Neutron_res_fit_param << "B_Corr_pol2_wPC_Error" << "\t\t" << B_Corr_pol2_wPC_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol2_wPC" << "\t\t\t\t" << C_Corr_pol2_wPC << "\n";
        Neutron_res_fit_param << "C_Corr_pol2_wPC_Error" << "\t\t" << C_Corr_pol2_wPC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol2_wPC" << "\t\t" << ChiSquare_Corr_pol2_wPC << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol2_wPC" << "\t\t\t" << NDF_Corr_pol2_wPC << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol3" << "\t\t\t\t\t" << A_Corr_pol3 << "\n";
        Neutron_res_fit_param << "A_Corr_pol3_Error" << "\t\t\t" << A_Corr_pol3_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol3" << "\t\t\t\t\t" << B_Corr_pol3 << "\n";
        Neutron_res_fit_param << "B_Corr_pol3_Error" << "\t\t\t" << B_Corr_pol3_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol3" << "\t\t\t\t\t" << C_Corr_pol3 << "\n";
        Neutron_res_fit_param << "C_Corr_pol3_Error" << "\t\t\t" << C_Corr_pol3_Error << "\n";
        Neutron_res_fit_param << "D_Corr_pol3" << "\t\t\t\t\t" << D_Corr_pol3 << "\n";
        Neutron_res_fit_param << "D_Corr_pol3_Error" << "\t\t\t" << D_Corr_pol3_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol3" << "\t\t\t" << ChiSquare_Corr_pol3 << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol3" << "\t\t\t\t" << NDF_Corr_pol3 << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol3_wPC" << "\t\t\t\t" << A_Corr_pol3_wPC << "\n";
        Neutron_res_fit_param << "A_Corr_pol3_wPC_Error" << "\t\t" << A_Corr_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol3_wPC" << "\t\t\t\t" << B_Corr_pol3_wPC << "\n";
        Neutron_res_fit_param << "B_Corr_pol3_wPC_Error" << "\t\t" << B_Corr_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol3_wPC" << "\t\t\t\t" << C_Corr_pol3_wPC << "\n";
        Neutron_res_fit_param << "C_Corr_pol3_wPC_Error" << "\t\t" << C_Corr_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "D_Corr_pol3_wPC" << "\t\t\t\t" << D_Corr_pol3_wPC << "\n";
        Neutron_res_fit_param << "D_Corr_pol3_wPC_Error" << "\t\t" << D_Corr_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol3_wPC" << "\t\t" << ChiSquare_Corr_pol3_wPC << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol3_wPC" << "\t\t\t" << NDF_Corr_pol3_wPC << "\n\n";

        Neutron_res_fit_param << "A_Std_pol1" << "\t\t\t\t\t" << A_Std_pol1 << "\n";
        Neutron_res_fit_param << "A_Std_pol1_Error" << "\t\t\t" << A_Std_pol1_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol1" << "\t\t\t\t\t" << B_Std_pol1 << "\n";
        Neutron_res_fit_param << "B_Std_pol1_Error" << "\t\t\t" << B_Std_pol1_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol1" << "\t\t\t" << ChiSquare_Std_pol1 << "\n";
        Neutron_res_fit_param << "NDF_Std_pol1" << "\t\t\t\t" << NDF_Std_pol1 << "\n\n";

        Neutron_res_fit_param << "A_Std_pol1_wPC" << "\t\t\t\t" << A_Std_pol1_wPC << "\n";
        Neutron_res_fit_param << "A_Std_pol1_wPC_Error" << "\t\t" << A_Std_pol1_wPC_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol1_wPC" << "\t\t\t\t" << B_Std_pol1_wPC << "\n";
        Neutron_res_fit_param << "B_Std_pol1_wPC_Error" << "\t\t" << B_Std_pol1_wPC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol1_wPC" << "\t\t" << ChiSquare_Std_pol1_wPC << "\n";
        Neutron_res_fit_param << "NDF_Std_pol1_wPC" << "\t\t\t" << NDF_Std_pol1_wPC << "\n\n";

        Neutron_res_fit_param << "A_Std_pol2" << "\t\t\t\t\t" << A_Std_pol2 << "\n";
        Neutron_res_fit_param << "A_Std_pol2_Error" << "\t\t\t" << A_Std_pol2_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol2" << "\t\t\t\t\t" << B_Std_pol2 << "\n";
        Neutron_res_fit_param << "B_Std_pol2_Error" << "\t\t\t" << B_Std_pol2_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol2" << "\t\t\t\t\t" << C_Std_pol2 << "\n";
        Neutron_res_fit_param << "C_Std_pol2_Error" << "\t\t\t" << C_Std_pol2_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol2" << "\t\t\t" << ChiSquare_Std_pol2 << "\n";
        Neutron_res_fit_param << "NDF_Std_pol2" << "\t\t\t\t" << NDF_Std_pol2 << "\n\n";

        Neutron_res_fit_param << "A_Std_pol2_wPC" << "\t\t\t\t" << A_Std_pol2_wPC << "\n";
        Neutron_res_fit_param << "A_Std_pol2_wPC_Error" << "\t\t" << A_Std_pol2_wPC_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol2_wPC" << "\t\t\t\t" << B_Std_pol2_wPC << "\n";
        Neutron_res_fit_param << "B_Std_pol2_wPC_Error" << "\t\t" << B_Std_pol2_wPC_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol2_wPC" << "\t\t\t\t" << C_Std_pol2_wPC << "\n";
        Neutron_res_fit_param << "C_Std_pol2_wPC_Error" << "\t\t" << C_Std_pol2_wPC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol2_wPC" << "\t\t" << ChiSquare_Std_pol2_wPC << "\n";
        Neutron_res_fit_param << "NDF_Std_pol2_wPC" << "\t\t\t" << NDF_Std_pol2_wPC << "\n\n";

        Neutron_res_fit_param << "A_Std_pol3" << "\t\t\t\t\t" << A_Std_pol3 << "\n";
        Neutron_res_fit_param << "A_Std_pol3_Error" << "\t\t\t" << A_Std_pol3_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol3" << "\t\t\t\t\t" << B_Std_pol3 << "\n";
        Neutron_res_fit_param << "B_Std_pol3_Error" << "\t\t\t" << B_Std_pol3_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol3" << "\t\t\t\t\t" << C_Std_pol3 << "\n";
        Neutron_res_fit_param << "C_Std_pol3_Error" << "\t\t\t" << C_Std_pol3_Error << "\n";
        Neutron_res_fit_param << "D_Std_pol3" << "\t\t\t\t\t" << D_Std_pol3 << "\n";
        Neutron_res_fit_param << "D_Std_pol3_Error" << "\t\t\t" << D_Std_pol3_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol3" << "\t\t\t" << ChiSquare_Std_pol3 << "\n";
        Neutron_res_fit_param << "NDF_Std_pol3" << "\t\t\t\t" << NDF_Std_pol3 << "\n\n";

        Neutron_res_fit_param << "A_Std_pol3_wPC" << "\t\t\t\t" << A_Std_pol3_wPC << "\n";
        Neutron_res_fit_param << "A_Std_pol3_wPC_Error" << "\t\t" << A_Std_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol3_wPC" << "\t\t\t\t" << B_Std_pol3_wPC << "\n";
        Neutron_res_fit_param << "B_Std_pol3_wPC_Error" << "\t\t" << B_Std_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol3_wPC" << "\t\t\t\t" << C_Std_pol3_wPC << "\n";
        Neutron_res_fit_param << "C_Std_pol3_wPC_Error" << "\t\t" << C_Std_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "D_Std_pol3_wPC" << "\t\t\t\t" << D_Std_pol3_wPC << "\n";
        Neutron_res_fit_param << "D_Std_pol3_wPC_Error" << "\t\t" << D_Std_pol3_wPC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol3_wPC" << "\t\t" << ChiSquare_Std_pol3_wPC << "\n";
        Neutron_res_fit_param << "NDF_Std_pol3_wPC" << "\t\t\t" << NDF_Std_pol3_wPC << "\n\n";
    }
    //</editor-fold>

    Neutron_res_fit_param.close();

    if (!momResTestMode) {
        /* Copy fitted parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str());
    }
}
//</editor-fold>

// LogHistDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogHistDataToFile function">
void NeutronResolution::LogHistDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    ofstream Neutron_res_Hist_param;
    std::string Neutron_res_Hist_paramFilePath;

    if (momResS2CalcMode) {
        if (!momResTestMode) {
            Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + Particle + "_momResS2_hist_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_Hist_paramFilePath = plots_path + "/" + Particle + "_momResS2_hist_param_-_" + SampleName + ".par";
        }
    } else {
        if (!momResTestMode) {
            Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + Particle + "_momResS1_hist_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_Hist_paramFilePath = plots_path + "/" + Particle + "_momResS1_hist_param_-_" + SampleName + ".par";
        }
    }

    //<editor-fold desc="Writing log header">
    string momRes_calculation_mode;

    if (momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS2";
    } else if (!momResS2CalcMode && momResS2RunMode) {
        momRes_calculation_mode = "Run on loaded momResS2";
    } else if (!momResS2CalcMode && !momResS2RunMode) {
        momRes_calculation_mode = "Calculate momResS1";
    }

    Neutron_res_Hist_param.open(Neutron_res_Hist_paramFilePath);
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "# CLAS12 analysis cuts and parameters file (after nRes Gaussian Hist) #\n";
    Neutron_res_Hist_param << "######################################################################\n";
    Neutron_res_Hist_param << "\n";
    Neutron_res_Hist_param << "# " + Particle + " resolution parameters are for:\n";
    Neutron_res_Hist_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_Hist_param << "#Analysis_run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_Hist_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_Hist_param << "#delta:\t\t" << delta << "\n";
    //</editor-fold>

    for (DSCuts ResSlice: ResSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":"
                               << ResSlice.GetSliceUpperb()
                               << ":" << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param.close();

    if (!momResTestMode) {
        /* Copy histogram parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_Hist_paramFilePath + " " + plots_path).c_str());
    }
}
//</editor-fold>

// ReadResDataParam function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ReadResDataParam function">
void NeutronResolution::ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const string &SampleName, const string &NucleonCutsDirectory,
                                         const bool &Load_correction, const bool &Load_smearing) {
    ifstream infile;
    infile.open(filename);

    momResS2CalcMode = Calculate_momResS2;
    SName = SampleName;
    SetUpperMomCut(SampleName, NucleonCutsDirectory);

    if (Load_correction) { cout << "\n\nLoading neutron correction from:\n" << filename << "\n"; }

    if (Load_smearing) { cout << "\nLoading proton smearing from:\n" << filename << "\n"; }

    if (infile.is_open()) {
        string tp;

        // remove 3 lines of header
        for (int i = 0; i < 3; i++) { getline(infile, tp); }

        // getline(infile, tp) = read data from file object and put it into string.
        while (getline(infile, tp)) {
            stringstream ss(tp);
            string parameter, parameter2;
            ss >> parameter; // get cut identifier

            if (!Load_correction && !Load_smearing) {
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
            } else {
                ss >> parameter2;
                stringstream ss2(parameter2);

                //TODO: reorganize these into vectors!
                if (Load_correction && findSubstring(parameter, "Corr")) {
                    Loaded_Corr_coefficients_path = filename;

                    if (findSubstring(parameter, "pol1") && findSubstring(ShiftMode, "pol1")) {
                        if (findSubstring(parameter, "pol1_wPC") && findSubstring(ShiftMode, "pol1_wPC")) {
                            if (parameter == "A_Corr_pol1_wPC") {
                                Loaded_A_Corr_pol1_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol1_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol1_wPC");
                            } else if (parameter == "A_Corr_pol1_wPC_Error") {
                                Loaded_A_Corr_pol1_wPC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol1_wPC") {
                                Loaded_B_Corr_pol1_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol1_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol1_wPC");
                            } else if (parameter == "B_Corr_pol1_wPC_Error") {
                                Loaded_B_Corr_pol1_wPC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol1_wPC") {
                                Loaded_ChiSquare_Corr_pol1_wPC = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol1_wPC") {
                                Loaded_NDF_Corr_pol1_wPC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol1") && !findSubstring(ShiftMode, "pol1_wPC")) {
                            if (parameter == "A_Corr_pol1") {
                                Loaded_A_Corr_pol1 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol1), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol1");
                            } else if (parameter == "A_Corr_pol1_Error") {
                                Loaded_A_Corr_pol1_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol1") {
                                Loaded_B_Corr_pol1 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol1), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol1");
                            } else if (parameter == "B_Corr_pol1_Error") {
                                Loaded_B_Corr_pol1_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol1") {
                                Loaded_ChiSquare_Corr_pol1 = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol1") {
                                Loaded_NDF_Corr_pol1 = stod(parameter2);
                            }
                        }
                    } else if (findSubstring(parameter, "pol2") && findSubstring(ShiftMode, "pol2")) {
                        if (findSubstring(parameter, "pol2_wPC") && findSubstring(ShiftMode, "pol2_wPC")) {
                            if (parameter == "A_Corr_pol2_wPC") {
                                Loaded_A_Corr_pol2_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol2_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol2_wPC");
                            } else if (parameter == "A_Corr_pol2_wPC_Error") {
                                Loaded_A_Corr_pol2_wPC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol2_wPC") {
                                Loaded_B_Corr_pol2_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol2_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol2_wPC");
                            } else if (parameter == "B_Corr_pol2_wPC_Error") {
                                Loaded_B_Corr_pol2_wPC_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol2_wPC") {
                                Loaded_C_Corr_pol2_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol2_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_C_Corr_pol2_wPC");
                            } else if (parameter == "C_Corr_pol2_wPC_Error") {
                                Loaded_C_Corr_pol2_wPC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol2_wPC") {
                                Loaded_ChiSquare_Corr_pol2_wPC = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol2_wPC") {
                                Loaded_NDF_Corr_pol2_wPC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol2") && !findSubstring(ShiftMode, "pol2_wPC")) {
                            if (parameter == "A_Corr_pol2") {
                                Loaded_A_Corr_pol2 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol2), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol2");
                            } else if (parameter == "A_Corr_pol2_Error") {
                                Loaded_A_Corr_pol2_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol2") {
                                Loaded_B_Corr_pol2 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol2), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol2");
                            } else if (parameter == "B_Corr_pol2_Error") {
                                Loaded_B_Corr_pol2_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol2") {
                                Loaded_C_Corr_pol2 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol2), Loaded_Corr_coefficients_names.push_back("Loaded_C_Corr_pol2");
                            } else if (parameter == "C_Corr_pol2_Error") {
                                Loaded_C_Corr_pol2_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol2") {
                                Loaded_ChiSquare_Corr_pol2 = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol2") {
                                Loaded_NDF_Corr_pol2 = stod(parameter2);
                            }
                        }
                    } else if (findSubstring(parameter, "pol3") && findSubstring(ShiftMode, "pol3")) {
                        if (findSubstring(parameter, "pol3_wPC") && findSubstring(ShiftMode, "pol3_wPC")) {
                            if (parameter == "A_Corr_pol3_wPC") {
                                Loaded_A_Corr_pol3_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol3_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol3_wPC");
                            } else if (parameter == "A_Corr_pol3_wPC_Error") {
                                Loaded_A_Corr_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol3_wPC") {
                                Loaded_B_Corr_pol3_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol3_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol3_wPC");
                            } else if (parameter == "B_Corr_pol3_wPC_Error") {
                                Loaded_B_Corr_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol3_wPC") {
                                Loaded_C_Corr_pol3_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol3_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_C_Corr_pol3_wPC");
                            } else if (parameter == "C_Corr_pol3_wPC_Error") {
                                Loaded_C_Corr_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "D_Corr_pol3_wPC") {
                                Loaded_D_Corr_pol3_wPC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_D_Corr_pol3_wPC), Loaded_Corr_coefficients_names.push_back("Loaded_D_Corr_pol3_wPC");
                            } else if (parameter == "D_Corr_pol3_wPC_Error") {
                                Loaded_D_Corr_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol3_wPC") {
                                Loaded_ChiSquare_Corr_pol3_wPC = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol3_wPC") {
                                Loaded_NDF_Corr_pol3_wPC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol3") && !findSubstring(ShiftMode, "pol3_wPC")) {
                            if (parameter == "A_Corr_pol3") {
                                Loaded_A_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol3), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol3");
                            } else if (parameter == "A_Corr_pol3_Error") {
                                Loaded_A_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol3") {
                                Loaded_B_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol3), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol3");
                            } else if (parameter == "B_Corr_pol3_Error") {
                                Loaded_B_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol3") {
                                Loaded_C_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol3), Loaded_Corr_coefficients_names.push_back("Loaded_C_Corr_pol3");
                            } else if (parameter == "C_Corr_pol3_Error") {
                                Loaded_C_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "D_Corr_pol3") {
                                Loaded_D_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_D_Corr_pol3), Loaded_Corr_coefficients_names.push_back("Loaded_D_Corr_pol3");
                            } else if (parameter == "D_Corr_pol3_Error") {
                                Loaded_D_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol3") {
                                Loaded_ChiSquare_Corr_pol3 = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol3") {
                                Loaded_NDF_Corr_pol3 = stod(parameter2);
                            }
                        }
                    }
                } else if (Load_smearing && findSubstring(parameter, "Std")) {
                    Loaded_Std_coefficients_path = filename;

                    if (findSubstring(parameter, "pol1") && findSubstring(SmearMode, "pol1")) {
                        if (findSubstring(parameter, "pol1_wPC") && findSubstring(SmearMode, "pol1_wPC")) {
                            if (parameter == "A_Std_pol1_wPC") {
                                Loaded_A_Std_pol1_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol1_wPC), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol1_wPC");
                            } else if (parameter == "A_Std_pol1_wPC_Error") {
                                Loaded_A_Std_pol1_wPC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol1_wPC") {
                                Loaded_B_Std_pol1_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol1_wPC), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol1_wPC");
                            } else if (parameter == "B_Std_pol1_wPC_Error") {
                                Loaded_B_Std_pol1_wPC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol1_wPC") {
                                Loaded_ChiSquare_Std_pol1_wPC = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol1_wPC") {
                                Loaded_NDF_Std_pol1_wPC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol1") && !findSubstring(SmearMode, "pol1_wPC")) {
                            if (parameter == "A_Std_pol1") {
                                Loaded_A_Std_pol1 = stod(parameter2);
                                cout << "\nLoaded_A_Std_pol1 = " << Loaded_A_Std_pol1 << "\n";
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol1), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol1");
                            } else if (parameter == "A_Std_pol1_Error") {
                                Loaded_A_Std_pol1_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol1") {
                                Loaded_B_Std_pol1 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol1), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol1");
                            } else if (parameter == "B_Std_pol1_Error") {
                                Loaded_B_Std_pol1_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol1") {
                                Loaded_ChiSquare_Std_pol1 = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol1") {
                                Loaded_NDF_Std_pol1 = stod(parameter2);
                            }
                        }
                    } else if (findSubstring(parameter, "pol2") && findSubstring(SmearMode, "pol2")) {
                        if (findSubstring(parameter, "pol2_wPC") && findSubstring(SmearMode, "pol2_wPC")) {
                            if (parameter == "A_Std_pol2_wPC") {
                                Loaded_A_Std_pol2_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol2_wPC), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol2_wPC");
                            } else if (parameter == "A_Std_pol2_wPC_Error") {
                                Loaded_A_Std_pol2_wPC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol2_wPC") {
                                Loaded_B_Std_pol2_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol2_wPC), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol2_wPC");
                            } else if (parameter == "B_Std_pol2_wPC_Error") {
                                Loaded_B_Std_pol2_wPC_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol2_wPC") {
                                Loaded_C_Std_pol2_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_C_Std_pol2_wPC), Loaded_Std_coefficients_names.push_back("Loaded_C_Std_pol2_wPC");
                            } else if (parameter == "C_Std_pol2_wPC_Error") {
                                Loaded_C_Std_pol2_wPC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol2_wPC") {
                                Loaded_ChiSquare_Std_pol2_wPC = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol2_wPC") {
                                Loaded_NDF_Std_pol2_wPC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol2") && !findSubstring(SmearMode, "pol2_wPC")) {
                            if (parameter == "A_Std_pol2") {
                                Loaded_A_Std_pol2 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol2), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol2");
                            } else if (parameter == "A_Std_pol2_Error") {
                                Loaded_A_Std_pol2_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol2") {
                                Loaded_B_Std_pol2 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol2), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol2");
                            } else if (parameter == "B_Std_pol2_Error") {
                                Loaded_B_Std_pol2_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol2") {
                                Loaded_C_Std_pol2 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_C_Std_pol2), Loaded_Std_coefficients_names.push_back("Loaded_C_Std_pol2");
                            } else if (parameter == "C_Std_pol2_Error") {
                                Loaded_C_Std_pol2_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol2") {
                                Loaded_ChiSquare_Std_pol2 = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol2") {
                                Loaded_NDF_Std_pol2 = stod(parameter2);
                            }
                        }
                    } else if (findSubstring(parameter, "pol3") && findSubstring(SmearMode, "pol3")) {
                        if (findSubstring(parameter, "pol3_wPC") && findSubstring(SmearMode, "pol3_wPC")) {
                            if (parameter == "A_Std_pol3_wPC") {
                                Loaded_A_Std_pol3_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol3_wPC), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol3_wPC");
                            } else if (parameter == "A_Std_pol3_wPC_Error") {
                                Loaded_A_Std_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol3_wPC") {
                                Loaded_B_Std_pol3_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol3_wPC), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol3_wPC");
                            } else if (parameter == "B_Std_pol3_wPC_Error") {
                                Loaded_B_Std_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol3_wPC") {
                                Loaded_C_Std_pol3_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_C_Std_pol3_wPC), Loaded_Std_coefficients_names.push_back("Loaded_C_Std_pol3_wPC");
                            } else if (parameter == "C_Std_pol3_wPC_Error") {
                                Loaded_C_Std_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "D_Std_pol3_wPC") {
                                Loaded_D_Std_pol3_wPC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_D_Std_pol3_wPC), Loaded_Std_coefficients_names.push_back("Loaded_D_Std_pol3_wPC");
                            } else if (parameter == "D_Std_pol3_wPC_Error") {
                                Loaded_D_Std_pol3_wPC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol3_wPC") {
                                Loaded_ChiSquare_Std_pol3_wPC = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol3_wPC") {
                                Loaded_NDF_Std_pol3_wPC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol3") && !findSubstring(SmearMode, "pol3_wPC")) {
                            if (parameter == "A_Std_pol3") {
                                Loaded_A_Std_pol3 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol3), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol3");
                            } else if (parameter == "A_Std_pol3_Error") {
                                Loaded_A_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol3") {
                                Loaded_B_Std_pol3 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol3), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol3");
                            } else if (parameter == "B_Std_pol3_Error") {
                                Loaded_B_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol3") {
                                Loaded_C_Std_pol3 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_C_Std_pol3), Loaded_Std_coefficients_names.push_back("Loaded_C_Std_pol3");
                            } else if (parameter == "C_Std_pol3_Error") {
                                Loaded_C_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "D_Std_pol3") {
                                Loaded_D_Std_pol3 = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_D_Std_pol3), Loaded_Std_coefficients_names.push_back("Loaded_D_Std_pol3");
                            } else if (parameter == "D_Std_pol3_Error") {
                                Loaded_D_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol3") {
                                Loaded_ChiSquare_Std_pol3 = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol3") {
                                Loaded_NDF_Std_pol3 = stod(parameter2);
                            }
                        }
                    }
                }

                //<editor-fold desc="Safty checks">
                if (Loaded_Std_coefficients_values.size() != Loaded_Std_coefficients_names.size()) {
                    cout << "\n\nNeutronResolution::ReadResDataParam: smearing coefficients load registered improperly! Exiting...\n\n", exit(0);
                }
                if (Loaded_Corr_coefficients_values.size() != Loaded_Corr_coefficients_names.size()) {
                    cout << "\n\nNeutronResolution::ReadResDataParam: correction coefficients load registered improperly! Exiting...\n\n", exit(0);
                }
                //</editor-fold>

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
        /* Smearing and correction are disabled */
        return Momentum;
    } else {
        /* Smearing and correction are enabled */

        /* Setting momentum powers */
        double Momentum2 = Momentum * Momentum;
        double Momentum3 = Momentum2 * Momentum;

        if (SmearMode == "slices") {
            /* Smear using slice fit results */

            //<editor-fold desc="Original (smearing from loaded nRes fit variables)">
            if (Momentum < SliceUpperMomLim) { // NOTE: changed according to upper neutron mom. th.
                for (DSCuts Loaded_res_slice: Loaded_Res_Slices_FitVar) {
                    if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
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

        } else if ((SmearMode == "pol1") || (SmearMode == "pol2") || (SmearMode == "pol3") ||
                   (SmearMode == "pol1_wPC") || (SmearMode == "pol2_wPC") || (SmearMode == "pol3_wPC")) {
            /* Smear using pol fit results */
            double Smearing, Arg;

            if (SmearMode == "pol1") {
                Arg = Loaded_A_Std_pol1 * Momentum + Loaded_B_Std_pol1;
                Smearing = Rand->Gaus(1, Arg);
            } else if (SmearMode == "pol1_wPC") {
                Arg = Loaded_A_Std_pol1_wPC * Momentum + Loaded_B_Std_pol1_wPC;
                Smearing = Rand->Gaus(1, Arg);
            } else if (SmearMode == "pol2") {
                Arg = Loaded_A_Std_pol2 * Momentum2 + Loaded_B_Std_pol2 * Momentum + Loaded_C_Std_pol2;
                Smearing = Rand->Gaus(1, Arg);
            } else if (SmearMode == "pol2_wPC") {
                Arg = Loaded_A_Std_pol2_wPC * Momentum2 + Loaded_B_Std_pol2_wPC * Momentum + Loaded_C_Std_pol2_wPC;
                Smearing = Rand->Gaus(1, Arg);
            } else if (SmearMode == "pol3") {
                Arg = Loaded_A_Std_pol3 * Momentum3 + Loaded_B_Std_pol3 * Momentum2 + Loaded_C_Std_pol3 * Momentum + Loaded_D_Std_pol3;
                Smearing = Rand->Gaus(1, Arg);
            } else if (SmearMode == "pol3_wPC") {
                Arg = Loaded_A_Std_pol3_wPC * Momentum3 + Loaded_B_Std_pol3_wPC * Momentum2 + Loaded_C_Std_pol3_wPC * Momentum + Loaded_D_Std_pol3_wPC;
                Smearing = Rand->Gaus(1, Arg);
            }

            double SmearedMomentum = Smearing * Momentum; // minus for protons and plus for protons

            if (Printout) {
                cout << "\n\nSmearMode = " << SmearMode << "\n";
                cout << "Smearing = " << Smearing << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "SmearedMomentum = " << SmearedMomentum << "\n\n";
            }

            return SmearedMomentum;

            //<editor-fold desc="Original code">
            ///*
//        } else if ((SmearMode == "pol1") || (SmearMode == "pol3")) {
//            */
///* Proton smear from *excel* linear fit *//*
//
//            //TODO: automate the proton mom smear into this class for future uses
//            double Smearing;
//
//            if (findSubstring(SName, "C12_simulation_6GeV_T5")) { // Old sample
//                if (SmearMode == "pol1") {
//                    Smearing = Rand->Gaus(1, 0.0738 * Momentum - 0.0304); // new smear between 1 and 3 GeV/c
////                    Smearing = Rand->Gaus(1, 0.0583 * Momentum - 0.0045); // old smear between 0.4 to 4.09 GeV/c
//                } else {
//                    cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
//                }
//            } else { // New sample (24M, 1-foil)
//                if (SmearMode == "pol1") {
//                    */
///* nRes 1 *//*
//
////                    Smearing = Rand->Gaus(1, 0.0692 * Momentum - 0.0233); // new smear between 1 and 3 GeV/c
//////                    Smearing = Rand->Gaus(1, 0.0566 * Momentum - 0.0026); // old smear between 0.4 to 4.09 GeV/c
//
//                    */
///* nRes 2 *//*
//
//                    Smearing = Rand->Gaus(1, 0.0885 * Momentum - 0.0382); // new smear between 1 and 3 GeV/c
//                } else if (SmearMode == "pol3") {
//                    double Momentum2 = Momentum * Momentum;
//                    double Momentum3 = Momentum * Momentum * Momentum;
//
//                    Smearing = Rand->Gaus(1, -0.0136 * Momentum3 + 0.0789 * Momentum2 - 0.0758 * Momentum + 0.0606); // old smear between 0.4 to 4.09 GeV/c
//                } else {
//                    cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
//                }
//            }
//
//            double SmearedMomentum = Smearing * Momentum; // minus for protons and plus for protons
//
//            if (Printout) {
//                cout << "\n\nSmearMode = " << SmearMode << "\n";
//                cout << "Smearing = " << Smearing << "\n";
//                cout << "Momentum = " << Momentum << "\n";
//                cout << "SmearedMomentum = " << SmearedMomentum << "\n\n";
//            }
//
//            return SmearedMomentum;
//*/
            //</editor-fold>

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

    //TODO: rename as correction, not shift

    if (!apply_nucleon_SmearAndShift) {
        /* Smearing and correction are disabled */
        return Momentum;
    } else {
        /* Smearing and correction are enabled */

        /* Setting momentum powers */
        double Momentum2 = Momentum * Momentum;
        double Momentum3 = Momentum2 * Momentum;

        if (ShiftMode == "slices") {
            /* Correction using slice fit results */

            //<editor-fold desc="Original (shift from loaded nRes fit variables)">
            for (DSCuts Loaded_res_slice: Loaded_Res_Slices_HistVar) {
                if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
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

        } else if ((SmearMode == "pol1") || (SmearMode == "pol2") || (SmearMode == "pol3") ||
                   (SmearMode == "pol1_wPC") || (SmearMode == "pol2_wPC") || (SmearMode == "pol3_wPC")) {
            /* Correction using pol fit results */
            double shift;

            if (ShiftMode == "pol1") {
                shift = Loaded_A_Corr_pol1 * Momentum + Loaded_B_Corr_pol1;
            } else if (ShiftMode == "pol1_wPC") {
                shift = Loaded_A_Corr_pol1_wPC * Momentum + Loaded_B_Corr_pol1_wPC;
            } else if (ShiftMode == "pol2") {
                shift = Loaded_A_Corr_pol2 * Momentum2 + Loaded_B_Corr_pol2 * Momentum + Loaded_C_Corr_pol2;
            } else if (ShiftMode == "pol2_wPC") {
                shift = Loaded_A_Corr_pol2_wPC * Momentum2 + Loaded_B_Corr_pol2_wPC * Momentum + Loaded_C_Corr_pol2_wPC;
            } else if (ShiftMode == "pol3") {
                shift = Loaded_A_Corr_pol3 * Momentum3 + Loaded_B_Corr_pol3 * Momentum2 + Loaded_C_Corr_pol3 * Momentum + Loaded_D_Corr_pol3;
            } else if (ShiftMode == "pol3_wPC") {
                shift = Loaded_A_Corr_pol3_wPC * Momentum3 + Loaded_B_Corr_pol3_wPC * Momentum2 + Loaded_C_Corr_pol3_wPC * Momentum + Loaded_D_Corr_pol3_wPC;
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

        //<editor-fold desc="Original code">
        //        } else if ((ShiftMode == "pol1") || (ShiftMode == "pol3")) {
//            /* Neutron shift from *excel* linear fit */
//            //TODO: automate the neutron mom shift into this class for future uses
//            //TODO: rename as correction, not shift
//            double shift;
//
//            if (findSubstring(SName, "C12_simulation_6GeV_T5")) { // Old sample
//                if (ShiftMode == "pol1") {
////                    shift = 0.0579 * Momentum - 0.0146; // new shift between 1 and 3 GeV/c
//                    shift = 0.0583 * Momentum - 0.0127; // old shift between 0.4 to 4.09 GeV/c
//                } else {
//                    cout << "\n\nNeutronResolution::NShift: ShiftMode illegal (no pol3 for C12_simulation_6GeV_T5)! Exiting...", exit(0);
//                }
//            } else { // New sample (24M, 1-foil)
//                if (ShiftMode == "pol1") {
////                    shift = 0.068 * Momentum - 0.0256; // new shift between 1 and 3 GeV/c
//                    shift = 0.0674 * Momentum - 0.0218; // old shift between 0.4 to 4.09 GeV/c
//                } else if (ShiftMode == "pol3") {
//                    double Momentum2 = Momentum * Momentum;
//                    double Momentum3 = Momentum * Momentum * Momentum;
//
//                    shift = -0.0017 * Momentum3 + 0.0205 * Momentum2 + 0.0081 * Momentum + 0.0219; // old shift between 0.4 to 4.09 GeV/c
//                } else {
//                    cout << "\n\nNeutronResolution::NShift: ShiftMode illegal! Exiting...", exit(0);
//                }
//            }
//
//            double ShiftedMomentum = Momentum * (1 + shift); // minus for protons and plus for neutrons
//
//            if (Printout) {
//                cout << "\n\nShiftMode = " << ShiftMode << "\n";
//                cout << "Shift = " << shift << "\n";
//                cout << "Momentum = " << Momentum << "\n";
//                cout << "ShiftedMomentum = " << ShiftedMomentum << "\n\n";
//            }
//
//            return ShiftedMomentum;
//        } else {
//            cout << "\n\nNeutronResolution::NShift: ShiftMode illegal! Exiting...", exit(0);
//        }
        //</editor-fold>

    }

    return Momentum;
}
//</editor-fold>
