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
        PoliFitter();
    }
}
//</editor-fold>

// PoliFitter function --------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="PoliFitter function">
void NeutronResolution::PoliFitter() {

//    //<editor-fold desc="Original">
//    Double_t x[] = {1, 2, 3, 4, 5, 6, 7, 8};
//    Double_t y[] = {35, 57, 25, 22, 55, 47, 89, 44};
//    TGraph *g = new TGraph((sizeof(x) / sizeof(Double_t)), x, y);
//    TF1 *f = new TF1("f", "[2] * x * x + [1] * x + [0]");
//    g->Fit(f);
//    g->Draw("AL");
//    //</editor-fold>

//    //<editor-fold desc="First good">
//    Double_t x[] = {1, 2, 3, 4, 5, 6, 7, 8};
//    Double_t y[] = {35, 57, 25, 22, 55, 47, 89, 44};
//
//    TGraph *g = new TGraph((sizeof(x) / sizeof(Double_t)), x, y);
//
//    TF1 *f = new TF1("f", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C
//
//    g->Fit(f);
//    g->Draw("AL");
//
//    double A = f->GetParameter(0); // get [0]
//    double B = f->GetParameter(1); // get [1]
//    double C = f->GetParameter(2); // get [2]
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
//    //</editor-fold>

    //<editor-fold desc="First good">
    double MeanPn[NumberOfSlices], PnCorr[NumberOfSlices], PnStd[NumberOfSlices], y[NumberOfSlices];

    for (int i = 0; i < NumberOfSlices; i++) {
        MeanPn[i] = (ResSlicesLimits.at(i).at(1) + ResSlicesLimits.at(i).at(0)) / 2;

        cout << "MeanPn[" << i << "] = " << MeanPn[i] << "\n";
    }

    cout << "\n\n\n";

    for (int i = 0; i < NumberOfSlices; i++) {
        PnCorr[i] = ResSlicesFitVar.at(i).GetMean();

        cout << "PnCorr[" << i << "] = " << PnCorr[i] << "\n";
    }

    cout << "\n\n\n";

    for (int i = 0; i < NumberOfSlices; i++) {
        PnStd[i] = ResSlicesFitVar.at(i).GetUpperCut();

        cout << "PnStd[" << i << "] = " << PnStd[i] << "\n";
    }

//    exit(0);


/*//    double MeanPn[] = {1, 2, 3, 4, 5, 6, 7, 8};
//    double y[] = {35, 57, 25, 22, 55, 47, 89, 44};

    TGraph *g = new TGraph((sizeof(MeanPn) / sizeof(Double_t)), MeanPn, y);

    TF1 *f = new TF1("f", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g->Fit(f);
    g->Draw("AL");

    double A = f->GetParameter(0); // get [0]
    double B = f->GetParameter(1); // get [1]
    double C = f->GetParameter(2); // get [2]

    double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
    double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;

    double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
    double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetTextFont(0);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->AddText(("Fit A = " + to_string(A)).c_str());
    FitParam->AddText(("Fit B = " + to_string(B)).c_str());
    FitParam->AddText(("Fit C = " + to_string(C)).c_str());
//    FitParam->AddText(("Fit A = " + to_string_with_precision(A, 8)).c_str());
//    FitParam->AddText(("Fit B = " + to_string_with_precision(B, 8)).c_str());
//    FitParam->AddText(("Fit C = " + to_string_with_precision(C, 8)).c_str());
//    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
    FitParam->Draw("same");*/
    //</editor-fold>

//    //<editor-fold desc="Old stuff">
//    cout << "\n\n";
//
//    //    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_10_-_ALL_CUTS/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10_plots.root");
////    string SampleName = "C12_simulation_6GeV_T5_first_10";
//
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_100_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100_plots.root");
////    string SampleName = "C12_simulation_6GeV_T5_first_100";
//
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_first_250_-02_ALL_CUTS_NoBetaCut/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_250_plots.root");
////    string SampleName = "C12_simulation_6GeV_T5_first_250";
//
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_NoBetaCut_wPhotonsFD/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
////    TFile *f = new TFile("plots_C12_simulation_6GeV_T5_-03_ALL_CUTS_WithBetaCut_fixedTLcuts/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    TFile *f = new TFile("00_plots_C12_simulation_6GeV_T5_-02_ALL_CUTS_noNC_/recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_plots.root");
//    string SampleName = "C12_simulation_6GeV_T5";
//
//    cout << "\nSampleName:\t" << SampleName << "\n\n";
//
//    DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, -9999, 9999); /* Beta cut (1n, FD) */
//    DSCuts n_momentum_cuts = DSCuts("Momentum", "", "Neutron", "", 0, 0.3, 9999); /* Momentum cuts (thresholds) */
//
//    double W_yLLim = -0.1, W_yULim = 0.1, W_xLLim = 0.9, W_xULim = 1.;
//    double deltaPRel_UncertaintyU = 0.2, deltaPRel_UncertaintyL = 0.1;
//
//    //<editor-fold desc="Canvas definitions">
//    TCanvas *Canvas = new TCanvas("Canvas", "Canvas", 1000, 750); // normal res
//    Canvas->SetGrid();
//    Canvas->SetBottomMargin(0.14);
//
//    Canvas->SetLeftMargin(0.18);
//    Canvas->SetRightMargin(0.12);
//
//    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();
//
//    Canvas->cd();
//    //</editor-fold>
//
//    //<editor-fold desc="Setting sNameFlag">
//    string sNameFlag;
//
//    if (findSubstring(SampleName, "sim")) {
//        sNameFlag = "s";
//    } else if (findSubstring(SampleName, "data")) {
//        sNameFlag = "d";
//    }
//    //</editor-fold>
//
//    //<editor-fold desc="Setting particle">
//    string BetaTitle = "Neutron #beta from 'photons'";
//
//    string BetaParticle, BetaParticleShort;
//
//    if (findSubstring(BetaTitle, "Electron") || findSubstring(BetaTitle, "electron")) {
//        BetaParticle = "Electron";
//        BetaParticleShort = "e";
//    } else if (findSubstring(BetaTitle, "Proton") || findSubstring(BetaTitle, "proton")) {
//        BetaParticle = "Proton";
//        BetaParticleShort = "p";
//    } else if (findSubstring(BetaTitle, "Neutron") || findSubstring(BetaTitle, "neutron")) {
//        BetaParticle = "Neutron";
//        BetaParticleShort = "n";
//    } else if (findSubstring(BetaTitle, "#pi^{+}")) {
//        BetaParticle = "Piplus";
//        BetaParticleShort = "#pi^{+}";
//    } else if (findSubstring(BetaTitle, "#pi^{-}")) {
//        BetaParticle = "Piminus";
//        BetaParticleShort = "#pi^{-}";
//    } else if (findSubstring(BetaTitle, "K^{+}")) {
//        BetaParticle = "Kplus";
//        BetaParticleShort = "K^{+}";
//    } else if (findSubstring(BetaTitle, "K^{-}")) {
//        BetaParticle = "Kminus";
//        BetaParticleShort = "K^{-}";
//    } else if (findSubstring(BetaTitle, "#gamma") || findSubstring(BetaTitle, "photon")
//               || findSubstring(BetaTitle, "Photon")) {
//        BetaParticle = "Photon";
//        BetaParticleShort = "#gamma";
//    }
//    //</editor-fold>
//
//    //<editor-fold desc="Setting final State">
//    string BetaFinalState = "1n";
//    //</editor-fold>
//
//    //<editor-fold desc="Setting histogram and preforming a fit">
//    TH1D *hpx = (TH1D *) f->Get("#beta of n from '#gamma' (1n, FD)");
//    TH1D *hBeta_Clone = (TH1D *) hpx->Clone("#beta of n from '#gamma' - fitted");
//    Int_t Color = hBeta_Clone->GetLineColor();
//
//    TF1 *func = new TF1("fit", FitFunction, 0, 2, 3); // create a function with 3 parameters in the range [-3,3]
//    func->SetLineColor(kRed);
//
//    double BetaMax = hBeta_Clone->GetMaximum();
//    double BetaMean = hBeta_Clone->GetMean();
//
//    func->SetParameters(BetaMax, BetaMean, 0.001); // start fit with histogram's max and mean
//    func->SetParNames("Constant", "Mean_value", "Sigma");
//
//    cout << "Beta histogram {max, mean}:\t{" << BetaMax << ", " << BetaMean << "}\n\n\n\n";
//
//////    // Adding limits to "Constant"
//////    double BetaConstantUlim = 1.2 * BetaMax;
//////    double BetaConstantLlim = 0.95 * BetaMax;
//////    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
//////    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
////    // Adding limits to "Constant"
////    double BetaConstantUlim = 1.2 * BetaMax;
////    double BetaConstantLlim = BetaMax;
//////    double BetaConstantLlim = 0.90 * BetaMax;
//////    double BetaConstantLlim = 0.67 * BetaMax;
////    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
////    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
//
//    // Adding limits to "Constant"
//    double BetaConstantUlim = 1.5 * BetaMax;
////    double BetaConstantLlim = BetaMax;
//    double BetaConstantLlim = 0.90 * BetaMax;
////    double BetaConstantLlim = 0.67 * BetaMax;
//    func->SetParLimits(0, BetaConstantLlim, BetaConstantUlim);
//    cout << "Beta Constant {Llim, Ulim}:\t{" << BetaConstantLlim << ", " << BetaConstantUlim << "}\n\n";
//
//
//    // Adding limits to "Mean_value"
//    double BetaMean_valueUlim = 1.02;
//    double BetaMean_valueLlim = 1.01;
////    double BetaMean_valueLlim = 1.008;
////    double BetaMean_valueLlim = 1.0075;
//    func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
//    cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";
////    // Adding limits to "Mean_value"
////    double BetaMean_valueUlim = 1.02;
////    double BetaMean_valueLlim = 1.005;
////    func->SetParLimits(1, BetaMean_valueLlim, BetaMean_valueUlim);
////    cout << "Beta Mean_value {Llim, Ulim}:\t{" << BetaMean_valueLlim << ", " << BetaMean_valueUlim << "}\n\n";
//
//
////    // Adding limits to "Sigma"
//////    double BetaSigmaUlim = 0.015;
////    double BetaSigmaUlim = 0.01;
////    double BetaSigmaLlim = 0.00001;
////    func->SetParLimits(2, BetaSigmaLlim, BetaSigmaUlim);
////    cout << "Beta Sigma {Llim, Ulim}:\t{" << BetaSigmaLlim << ", " << BetaSigmaUlim << "}\n\n";
//
//
//
//    cout << "\n";
//    hBeta_Clone->Fit("fit");
//    hBeta_Clone->Draw();
//    cout << "\n";
//
//    TF1 *fit = hBeta_Clone->GetFunction("fit");
//    double FitAmp = fit->GetParameter(0); // get p0
//    double FitMean = fit->GetParameter(1); // get p1
//    double FitStd = fit->GetParameter(2); // get p2
//
//    Beta_cut.SetUpperCut(fit->GetParameter(2));
//    //</editor-fold>
//
//    //<editor-fold desc="Drawing fit parameters and saving">
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
//    FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 8)).c_str());
//    FitParam->AddText(("Fit mean = " + to_string_with_precision(FitMean, 8)).c_str());
//    FitParam->AddText(("Fit std = " + to_string_with_precision(FitStd, 8)).c_str());
//    ((TText *) FitParam->GetListOfLines()->Last())->SetTextColor(kRed);
//    FitParam->Draw("same");
//
//    string hBeta_CloneSaveNameDir = "./" + sNameFlag + "01_Beta_Neutron_from_photons_1n" + "_fitted.png";
//    const char *hBeta_CloneSaveDir = hBeta_CloneSaveNameDir.c_str();
//    Canvas->SaveAs(hBeta_CloneSaveDir);
//
//    Canvas->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Plot deltaP as function of beta">
//    string deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    string deltaPTitle = BetaParticle + " momentum uncertainty #deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    string deltaPfunc = to_string(m_n * FitStd) + "/ ( (1 - x*x) * sqrt(1 - x*x) )";
//
//    auto *deltaP = new TF1(deltaPStatsTitle.c_str(), deltaPfunc.c_str(), 0.9, 1);
//    deltaP->SetTitle(deltaPTitle.c_str());
//    deltaP->GetXaxis()->SetTitleSize(0.06);
//    deltaP->GetXaxis()->SetLabelSize(0.0425);
//    deltaP->GetXaxis()->CenterTitle(true);
//    deltaP->GetXaxis()->SetTitle("#beta");
//    deltaP->GetYaxis()->SetRangeUser(0., 30.);
//    deltaP->GetYaxis()->SetTitleSize(0.06);
//    deltaP->GetYaxis()->SetLabelSize(0.0425);
//    deltaP->GetYaxis()->CenterTitle(true);
//    deltaP->GetYaxis()->SetTitle(("#deltaP_{" + BetaParticleShort + "} = #frac{m_{" + BetaParticleShort + "}#delta#beta}{(1 - #beta^{2})^{3/2}}").c_str());
//
//    deltaP->SetLineColor(Color);
//    deltaP->Draw();
//
//    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05 + 0.15, x_2_FitParam, y_2_FitParam + 0.15, "NDC");
////    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam - 0.1, x_2_FitParam, y_2_FitParam, "NDC");
////    TPaveText *deltaPParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
//    deltaPParam->SetBorderSize(1);
//    deltaPParam->SetTextFont(0);
//    deltaPParam->SetTextSize(0.03);
//    deltaPParam->SetFillColor(0);
//    deltaPParam->SetTextAlign(12);
//    deltaPParam->AddText(("d#beta = " + to_string(FitStd)).c_str());
////    deltaPParam->AddText(("#delta#beta = " + to_string_with_precision(FitStd, 8)).c_str());
//    deltaPParam->Draw("same");
//
//    string deltaPSaveNameDir = "./" + sNameFlag + "02a_P_" + BetaParticleShort + "_uncertainty" + BetaFinalState + ".png";
//    const char *deltaPSaveDir = deltaPSaveNameDir.c_str();
//    Canvas->SaveAs(deltaPSaveDir);
//
//    Canvas->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Solve deltaP/P for beta in range 0.9<=beta<1">
//    double Beta_Max, P_Beta_Max, Beta_Min, P_Beta_Min;
//    double Beta_Max_sol[3], Beta_Min_sol[3];
//
//    SolveP3(Beta_Max_sol, 0, -1, FitStd / deltaPRel_UncertaintyU);
//    SolveP3(Beta_Min_sol, 0, -1, FitStd / deltaPRel_UncertaintyL);
//
//    cout << "\nSolutions for W(beta) = 0 for 20%:\n";
//
//    for (int i = 0; i < 3; i++) {
//        cout << "Beta_Max_sol[" << i << "] = " << Beta_Max_sol[i] << "\n";
//
//        //TODO: see if other checks for the solution are required!!!
//        if (Beta_Max_sol[i] >= 0.9 && Beta_Max_sol[i] < 1) { Beta_Max = Beta_Max_sol[i]; }
//    }
//
//    P_Beta_Max = m_n * Beta_Max / sqrt(1 - Beta_Max * Beta_Max);
//
//    cout << "W(beta) const Max = " << FitStd / deltaPRel_UncertaintyU << "\n";
//    cout << "Beta_Max = " << Beta_Max << " is chosen\n";
//    cout << "P(Beta_Max) = " << P_Beta_Max << "\n\n";
//
//    cout << "Solutions for W(beta) = 0 for 10%:\n";
//
//    for (int i = 0; i < 3; i++) {
//        cout << "Beta_Min_sol[" << i << "] = " << Beta_Min_sol[i] << "\n";
//
//        //TODO: see if other checks for the solution are required!!!
//        if (Beta_Min_sol[i] >= 0.9 && Beta_Min_sol[i] < 1) { Beta_Min = Beta_Min_sol[i]; }
//    }
//
//    P_Beta_Min = m_n * Beta_Min / sqrt(1 - Beta_Min * Beta_Min);
//
//    cout << "W(beta) const Max = " << FitStd / deltaPRel_UncertaintyL << "\n";
//    cout << "Beta_Min = " << Beta_Min << " is chosen\n";
//    cout << "P(Beta_Min) = " << P_Beta_Min << "\n\n";
//
//    n_momentum_cuts.SetUpperCut(P_Beta_Max);
//    //</editor-fold>
//
//    //<editor-fold desc="Plot deltaP/P as function of beta">
//    string Rel_deltaPStatsTitle = "#deltaP_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    string Rel_deltaPTitle = BetaParticle + " relative uncertainty #deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} (" + BetaFinalState + ")";
//    string Rel_deltaPfunc = to_string(FitStd) + "/ ( (1 - x*x) * x )";
//
//    auto *Rel_deltaP = new TF1(Rel_deltaPStatsTitle.c_str(), Rel_deltaPfunc.c_str(), 0.9, 1);
//    Rel_deltaP->SetTitle(Rel_deltaPTitle.c_str());
//    Rel_deltaP->GetXaxis()->SetTitleSize(0.06);
//    Rel_deltaP->GetXaxis()->SetLabelSize(0.0425);
//    Rel_deltaP->GetXaxis()->CenterTitle(true);
//    Rel_deltaP->GetXaxis()->SetTitle("#beta");
//    Rel_deltaP->GetYaxis()->SetRangeUser(0., 0.5);
//    Rel_deltaP->GetYaxis()->SetTitleSize(0.06);
//    Rel_deltaP->GetYaxis()->SetLabelSize(0.0425);
//    Rel_deltaP->GetYaxis()->CenterTitle(true);
//    Rel_deltaP->GetYaxis()->SetTitle(("#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}} = #frac{1}{(1 - #beta^{2})#beta}").c_str());
//    Rel_deltaP->SetLineColor(kBlack);
//    Rel_deltaP->SetLineWidth(2);
//    Rel_deltaP->Draw();
//
//    TPaveText *deltaPRel_deltaP = new TPaveText(x_1_FitParam, y_1_FitParam - 0.05, x_2_FitParam, y_2_FitParam, "NDC");
//    deltaPRel_deltaP->SetBorderSize(1);
//    deltaPRel_deltaP->SetTextFont(0);
//    deltaPRel_deltaP->SetTextSize(0.03);
//    deltaPRel_deltaP->SetFillColor(0);
//    deltaPRel_deltaP->SetTextAlign(12);
//    deltaPRel_deltaP->AddText(("d#beta = " + to_string(FitStd)).c_str());
//    deltaPRel_deltaP->Draw("same");
//
//    TLine *upper_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyU, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyU);
//    upper_cut->SetLineWidth(2);
//    upper_cut->SetLineColor(kBlue);
//    upper_cut->Draw("same");
//
//    TLine *lower_cut = new TLine(gPad->GetFrame()->GetX2() - 0.1, deltaPRel_UncertaintyL, gPad->GetFrame()->GetX2(), deltaPRel_UncertaintyL);
//    lower_cut->SetLineWidth(2);
//    lower_cut->SetLineColor(kRed);
//    lower_cut->Draw("same");
//
//    auto Cut_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.125, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
//    TLegendEntry *Cut_legend_deltaPRel_deltaP = Cut_legend->AddEntry(deltaPRel_deltaP, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "}").c_str(),
//                                                                     "l");
//    TLegendEntry *Cut_legend_upper_lim = Cut_legend->AddEntry(upper_cut, "20% cut", "l");
//    TLegendEntry *Cut_legend_lower_lim = Cut_legend->AddEntry(lower_cut, "10% cut", "l");
//    Cut_legend->Draw("same");
//
//    string Rel_deltaPSaveNameDir = "./" + sNameFlag + "02b_P_" + BetaParticleShort + "_rel_uncertainty" + BetaFinalState + ".png";
//
//    const Int_t n = 2;
//    auto gr = new TGraph(n);
//    gr->SetMarkerStyle(20);
//    gr->SetMarkerSize(20);
//    auto ex = new TExec("ex", "drawtext();");
//    gr->GetListOfFunctions()->Add(ex);
//    gr->SetPoint(0, Beta_Max, deltaPRel_UncertaintyU);
//    gr->SetPoint(1, Beta_Min, deltaPRel_UncertaintyL);
//    gr->Draw("same");
//
///*
//    const Int_t n_Max = 1;
//    auto gr_Max = new TGraph(n_Max);
//    gr_Max->SetMarkerStyle(20);
//    auto ex_Max = new TExec("ex_Max", "drawtext_Max();");
//    gr_Max->GetListOfFunctions()->Add(ex_Max);
//    gr_Max->SetPoint(0, Beta_Max, deltaPRel_UncertaintyU);
//    gr_Max->Draw("same");
//
//    const Int_t n_Min = 1;
//    auto gr_Min = new TGraph(n_Min);
//    gr_Min->SetMarkerStyle(20);
//    auto ex_Min = new TExec("ex_Min", "drawtext_Min();");
//    gr_Min->GetListOfFunctions()->Add(ex_Min);
//    gr_Min->SetPoint(0, Beta_Min, deltaPRel_UncertaintyL);
//    gr_Min->Draw("same");
//*/
//
//    const char *Rel_deltaPSaveDir = Rel_deltaPSaveNameDir.c_str();
//    Canvas->SaveAs(Rel_deltaPSaveDir);
//    Canvas->Clear();
//    //</editor-fold>
//
//    //<editor-fold desc="Plot w as function of beta">
//    string WStatsTitle = "W(#beta) (" + BetaFinalState + ")";
//    string WTitle = "The W(#beta) function (" + BetaFinalState + ")";
//    string W_Maxfunc = "x*x*x - x + " + to_string(FitStd / deltaPRel_UncertaintyU);
//    string W_Minfunc = "x*x*x - x + " + to_string(FitStd / deltaPRel_UncertaintyL);
//
//    auto *W_Max = new TF1(WStatsTitle.c_str(), W_Maxfunc.c_str(), 0.9, 1);
//    W_Max->SetLineWidth(2);
//    W_Max->SetLineStyle(2);
//    W_Max->SetTitle(WTitle.c_str());
//    W_Max->GetXaxis()->SetTitleSize(0.06);
//    W_Max->GetXaxis()->SetLabelSize(0.0425);
//    W_Max->GetXaxis()->CenterTitle(true);
//    W_Max->GetXaxis()->SetTitle("#beta");
//    W_Max->GetYaxis()->SetRangeUser(W_yLLim, W_yULim);
//    W_Max->GetYaxis()->SetTitleSize(0.06);
//    W_Max->GetYaxis()->SetLabelSize(0.0425);
//    W_Max->GetYaxis()->CenterTitle(true);
//    W_Max->GetYaxis()->SetTitle(("W(#beta) = #beta^{3} - #beta + #delta#beta(#frac{#deltaP_{" + BetaParticleShort + "}}{P_{" + BetaParticleShort + "}})^{-1}").c_str());
//    W_Max->SetLineColor(kBlack);
//    W_Max->SetLineWidth(2);
//    W_Max->Draw();
//
//    auto *W_Min = new TF1(WStatsTitle.c_str(), W_Minfunc.c_str(), 0.9, 1);
//    W_Min->SetLineWidth(2);
//    W_Min->SetLineStyle(10);
//    W_Min->SetLineColor(kBlack);
//    W_Min->SetLineWidth(2);
//    W_Min->Draw("same");
//
//    TLine *Beta_Max_cut = new TLine(Beta_Max, W_yULim, Beta_Max, W_yLLim);
//    Beta_Max_cut->SetLineWidth(2);
//    Beta_Max_cut->SetLineColor(kBlue);
//    Beta_Max_cut->Draw("same");
//
//    TLine *Beta_Min_cut = new TLine(Beta_Min, W_yULim, Beta_Min, W_yLLim);
//    Beta_Min_cut->SetLineWidth(2);
//    Beta_Min_cut->SetLineColor(kRed);
//    Beta_Min_cut->Draw("same");
//
//    TLine *XAxis = new TLine(W_xLLim, 0., W_xULim, 0.);
//    XAxis->SetLineColor(kBlack);
//    XAxis->Draw("same");
//
//    auto W_legend = new TLegend(gStyle->GetStatX(), gStyle->GetStatY() - 0.2 + 0.1, gStyle->GetStatX() - 0.2, gStyle->GetStatY() - 0.3 + 0.1);
//    TLegendEntry *W_legend_upper_lim = W_legend->AddEntry(Beta_Max_cut, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} = 0.2").c_str(), "l");
//    TLegendEntry *W_legend_lower_lim = W_legend->AddEntry(Beta_Min_cut, ("#deltaP_{" + BetaParticleShort + "}/P_{" + BetaParticleShort + "} = 0.1").c_str(), "l");
//    W_legend->Draw("same");
//
//    string WSaveNameDir = "./" + sNameFlag + "03_W_function.png";
//    const char *WSaveDir = WSaveNameDir.c_str();
//    Canvas->SaveAs(WSaveDir);
//    Canvas->Clear();
//    //</editor-fold>
//    //</editor-fold>

}
//</editor-fold>

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
