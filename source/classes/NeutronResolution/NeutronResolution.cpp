//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//TODO: add a software that creates momentum slices with at least 1000 events each automatically

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const string &Particle, double beamE, double ParticleMomTh,
                                     const string &SavePath, double DeltaSlices, bool VaryingDelta) {
    SlicesSavePath = SavePath, delta = DeltaSlices;

    double Delta = delta, SliceLowerLim = ParticleMomTh, SliceUpperLim;
    SetUpperMomCut(SampleName, NucleonCutsDirectory);

    if (!VaryingDelta) {
        SliceUpperLim = SliceLowerLim + delta;
    } else {
        if (beamE == 5.98636) {
            if (SampleName == "C12_simulation_G18_Q204_6GeV") {
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
            cout << "SliceUpperLim = " << SliceUpperLim << "\n";
//            cout << "SliceNumber = " << SliceNumber << "\n";
//            cout << "Delta = " << Delta << "\n";
        }

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

            hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                       -0.75, 0.75, hSliceNumOfBin);
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

            if (parameter == "Momentum_cuts_ECAL") {
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

    return;
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
    for (int i = 0; i < NumberOfSlices; i++) {
        if ((Momentum >= ResSlicesLimits.at(i).at(0)) && (Momentum < ResSlicesLimits.at(i).at(1))) {
            ResSlices.at(i).hFill(Resolution, Weight);
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

        if (findSubstring(SampleName, "simulation")) {
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
            FittedSlices.push_back(i); // Log slices that are been fitted

            double FitUlim, FitLlim;

            if (Particle == "Neutron") {
                FitUlim = 1., FitLlim = -1.;
            } else if (Particle == "Proton") {
                FitUlim = 0.5, FitLlim = -0.5;
            }

            TF1 *func = new TF1("fit", CFitFunction, FitLlim, FitUlim, 3); // create a function with 3 parameters in the range [-3,3]
            func->SetLineColor(kRed);

            double SliceMax = hSlice->GetMaximum();
            double SliceMean = hSlice->GetMean();
            double SliceStd = hSlice->GetRMS();

            func->SetParameters(SliceMax, SliceMean, 0.5); // start fit with histogram's max and mean
            func->SetParNames("Constant", "Mean_value", "Sigma");

            if (Particle == "Neutron") {
                func->SetParLimits(1, -1.5, 1.5); // Mean limits
                func->SetParLimits(2, 0.001, 0.35); // Sigma limits
//            func->SetParLimits(2, 0.001, 0.325); // Sigma limits
//            func->SetParLimits(2, 0.001, 0.25); // Sigma limits
            } else if (Particle == "Proton") {
                func->SetParLimits(1, -1.5, 1.5); // Mean limits
                func->SetParLimits(2, 0.0000000001, 0.35); // Sigma limits
            }

            hSlice->Fit("fit");

            double FitAmp = func->GetParameter(0);  // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2);  // get p2

            ResSlicesFitVar.at(i).SetMean(func->GetParameter(1));
            ResSlicesFitVar.at(i).SetUpperCut(func->GetParameter(2));
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
            FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 8)).c_str());
            FitParam->AddText(("Fit mean = " + to_string_with_precision(FitMean, 8)).c_str());
            FitParam->AddText(("Fit std = " + to_string_with_precision(FitStd, 8)).c_str());
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
    string SaveDateDir = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/";

//    system(("rm -r " + SaveDateDir).c_str());
    system(("mkdir -p " + SaveDateDir).c_str());

    LogFitDataToFile(SampleName, Particle, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
    LogHistDataToFile(SampleName, Particle, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
}
//</editor-fold>

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogFitDataToFile function">
void NeutronResolution::LogFitDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
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
}
//</editor-fold>

// LogHistDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogHistDataToFile function">
void NeutronResolution::LogHistDataToFile(const string &SampleName, const string &Particle, const string &plots_path, const string &NeutronResolutionDirectory,
                                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
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
        cout << "\n\nReadResDataParam: file not found! Exiting...\n\n", exit(0);
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

//        //<editor-fold desc="Proton shift from excel linear fit">
//        //TODO: automate the proton mom shift into this class for future uses
//        //TODO: rename as correction, not shift
//        double Smearing;
//
//        if (findSubstring(SName,"C12_simulation_6GeV_T5")) {
//            Smearing = Rand->Gaus(1, 0.0738 * Momentum - 0.0304); // new shift between 1 and 3 GeV/c
////            Smearing = Rand->Gaus(1, 0.0583 * Momentum - 0.0045); // old shift between 0.4 to 4.09 GeV/c
//        } else {
//            Smearing = Rand->Gaus(1, 0.0683 * Momentum - 0.0262); // new shift between 1 and 3 GeV/c
////            Smearing = Rand->Gaus(1, 0.0681 * Momentum - 0.0229); // old shift between 0.4 to 4.09 GeV/c
//        }
//
//        double SmearedMomentum = Smearing * Momentum;; // minus for protons and plus for protons
//
//        if (Printout) {
//            cout << "\n\nSmearing (0.0583 * Momentum - 0.0127) = " << Smearing << "\n";
//            cout << "Momentum = " << Momentum << "\n";
//            cout << "SmearedMomentum = " << SmearedMomentum << "\n\n";
//        }
//
//        return SmearedMomentum;
//        //</editor-fold>

        //<editor-fold desc="Original (smearing from loaded nRes fit variables)">
        if (Momentum <= SliceUpperMomLim) { // NOTE: changed according to upper neutron mom. th.
            for (DSCuts Loaded_res_slice: Loaded_Res_Slices_FitVar) {
                if ((Loaded_res_slice.GetSliceLowerb() <= Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
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

//        //<editor-fold desc="Original">
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

    }

//    return Momentum;
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

        //<editor-fold desc="Neutron shift from excel linear fit">
        //TODO: automate the neutron mom shift into this class for future uses
        //TODO: rename as correction, not shift
        double shift;

        if (findSubstring(SName,"C12_simulation_6GeV_T5")) {
//            shift = 0.0579 * Momentum - 0.0146; // new shift between 1 and 3 GeV/c
            shift = 0.0583 * Momentum - 0.0127; // old shift between 0.4 to 4.09 GeV/c
        } else {
//            shift = 0.0694 * Momentum - 0.0236; // new shift between 1 and 3 GeV/c
            shift = 0.0571 * Momentum - 0.0034; // old shift between 0.4 to 4.09 GeV/c
        }

        double ShiftedMomentum = Momentum * (1 + shift); // minus for protons and plus for neutrons

        if (Printout) {
            cout << "\n\nshift (0.0583 * Momentum - 0.0127) = " << shift << "\n";
            cout << "Momentum = " << Momentum << "\n";
            cout << "ShiftedMomentum = " << ShiftedMomentum << "\n\n";
        }

        return ShiftedMomentum;
        //</editor-fold>

//        //<editor-fold desc="Original (shift from loaded nRes fit variables)">
//        for (DSCuts Loaded_res_slice: Loaded_Res_Slices_HistVar) {
//            if ((Loaded_res_slice.GetSliceLowerb() <= Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                double ShiftedMomentum = Momentum * (1 + Loaded_res_slice.GetMean()); // minus for protons and plus for neutrons
//
//                if (Printout) {
//                    cout << "\n\nLoaded_res_slice.GetMean() = " << Loaded_res_slice.GetMean() << "\n";
//                    cout << "Momentum = " << Momentum << "\n";
//                    cout << "shift = " << Loaded_res_slice.GetMean() << "\n";
//                    cout << "ShiftedMomentum = " << ShiftedMomentum << "\n\n";
//                }
//
//                return ShiftedMomentum;
//            }
//        }
//        //</editor-fold>

    }

//    return Momentum;
}
//</editor-fold>
