//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor ----------------------------------------------------------------------------------------------------------------------------------------

//TODO: add a software that creates momentum slices with at least 1000 events each automatically

// NeutronResolution constructors ----------------------------------------------------------------------------------------------------------------------------------------

// NeutronResolution constructor -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(const string &Particle) {
    bool PrintOut = false;

    if (Particle == "Neutron") {
        isNeutron = true, isProton = false;
        MomResParticle = Particle;

        if (PrintOut) {
            cout << "\nParticle = " << Particle << "\n";
            cout << "MomResParticle = " << MomResParticle << "\n";
        }
    } else if (Particle == "Proton") {
        isNeutron = false, isProton = true;
        MomResParticle = Particle;

        if (PrintOut) {
            cout << "\nParticle = " << Particle << "\n";
            cout << "MomResParticle = " << MomResParticle << "\n";
        }
    } else {
        cout << "NeutronResolution::NeutronResolution: entered particle is illegal! Exiting...\n\n", exit(0);
    }

    //<editor-fold desc="Safety checks">
    if (isNeutron == isProton) {
        cout << "NeutronResolution::NeutronResolution: unclear nucleon selection! Exiting...\n\n", exit(0);
    }

    if (MomResParticle != "Neutron" && MomResParticle != "Proton") {
        cout << "\nNeutronResolution::NeutronResolution: nucleon have not been set!\n";
        cout << "Particle = " << Particle << "\n";
        cout << "MomResParticle = " << MomResParticle << "\n";
        cout << "Exiting...\n\n", exit(0);
    }
    //</editor-fold>

}
//</editor-fold>

//<editor-fold desc="NeutronResolution constructor">
NeutronResolution::NeutronResolution(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                                     const DSCuts &FD_nucleon_momentum_cut, const double &ParticleMomTh, bool const &Calculate_momResS2, bool const &Run_in_momResS2,
                                     const string &NeutronResolutionDirectory, const string &SavePath, const double &DeltaSlices, const bool &VaryingDelta,
                                     const string &SmearM, const string &CorrM, bool momRes_test, bool ForceSmallpResLimits) {
    SliceUpperMomLimKC = FD_nucleon_momentum_cut.GetUpperCutConst(), SliceLowerMomLimKC = FD_nucleon_momentum_cut.GetLowerCutConst();
    momResS2CalcMode = Calculate_momResS2, momResS2RunMode = Run_in_momResS2;
    SlicesSavePath = SavePath;
    delta = DeltaSlices;
    SmearMode = SmearM, CorrMode = CorrM;
    momResTestMode = momRes_test;
    ForceSmallProtonResLimits = ForceSmallpResLimits;

    //<editor-fold desc="Safety check">
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
        }

        //<editor-fold desc="Safety check">
        if ((abs(SliceUpperLim) > beamE * 1.5) || (abs(SliceLowerLim) > beamE * 1.5)) {
            cout << "NeutronResolution::NeutronResolution: slice limits are not defined properly! Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        int SliceUpperLimPrecision;
        if (SliceUpperLim == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

        string hStatsTitle, hTitle, hSaveName, hCutName;

        hPlot1D hResolutionSlice;

        DSCuts ResSliceFitCuts;

        if (isNeutron) {
            hStatsTitle = "n res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{nFD}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Neutron resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{nFD}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_n_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "R_{nFD} = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName,
                                       hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            ResSliceFitCuts = DSCuts(("fit_" + hCutName), "FD", "Neutron", "1n", 0, -9999, 9999);
        } else if (isProton) {
            hStatsTitle = "p res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{pFD}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Proton resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{pFD}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_p_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            if (!momRes_test || ForceSmallpResLimits) {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                           -0.2, 0.2, hSliceNumOfBin);
//                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
//                                           -0.75, 0.75, hSliceNumOfBin);
            } else {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            }

            ResSliceFitCuts = DSCuts(("fit_" + hCutName), "FD", "Proton", "1n", 0, -9999, 9999);
        }

        ResSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResSliceFitCuts.SetSliceNumber(SliceNumber);

        ResTLMomSlices.push_back(hResolutionSlice);
        ResTLMomSlicesLimits.push_back({SliceLowerLim, SliceUpperLim});
        ResTLMomSlicesFitVar.push_back(ResSliceFitCuts);

        ResSliceFitCuts.SetCutVariable(("hist_" + hCutName));
        ResTLMomSlicesHistVar.push_back(ResSliceFitCuts);

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
                        } else if ((SliceLowerLim >= 3.00) && (SliceLowerLim < 3.50)) { // 3.00-3.50
                            Delta = delta * 10;
                        } else if (SliceLowerLim >= 3.50) { // 3.00-SliceUpperMomLim
                            Delta = SliceUpperMomLim - SliceLowerLim;
//                        } else if (SliceLowerLim >= 3.00) { // 3.00-SliceUpperMomLim
//                            Delta = SliceUpperMomLim - SliceLowerLim;
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

    if (isNeutron) {
        if (momResS2CalcMode && !momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 calculation mode. Loading momResS1 variables...\n";

            string NeutronCorrectionDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1), but not smearing variables */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            //<editor-fold desc="Safety checks for data files">
            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                cout << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(0);
            }
            //</editor-fold>

            cout << "\nDone.\n";
        } else if (!momResS2CalcMode && momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 run mode.\n";
            cout << "Loading correction from momResS1 variables & smearing from momResS2 variables...\n";

            string NeutronCorrectionDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
            string ProtonSmearingDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1) */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            /* Load proton smearing variables (from momResS2) */
            ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, false, true);

            //<editor-fold desc="Safety checks for data files">
            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                cout << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(0);
            }

            if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
                cout << "\n\nNeutronResolution::NeutronResolution: proton smearing variables are not being loaded from neutron data! Exiting...\n\n", exit(0);
            }
            //</editor-fold>

            cout << "\nDone.\n";
        }
    }
}
//</editor-fold>

// SetMomResCalculations function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetMomResCalculations function">
void NeutronResolution::SetMomResCalculations(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                                              const DSCuts &FD_nucleon_momentum_cut, const double &ParticleMomTh, bool const &Calculate_momResS2, bool const &Run_in_momResS2,
                                              const string &NeutronResolutionDirectory, const string &SavePath, const double &DeltaSlices, const bool &VaryingDelta,
                                              const string &SmearM, const string &CorrM, bool momRes_test, bool ForceSmallpResLimits) {
    SliceUpperMomLimKC = FD_nucleon_momentum_cut.GetUpperCutConst(), SliceLowerMomLimKC = FD_nucleon_momentum_cut.GetLowerCutConst();
    momResS2CalcMode = Calculate_momResS2, momResS2RunMode = Run_in_momResS2;
    SlicesSavePath = SavePath;
    delta = DeltaSlices;
    SmearMode = SmearM, CorrMode = CorrM;
    momResTestMode = momRes_test;
    ForceSmallProtonResLimits = ForceSmallpResLimits;

    //<editor-fold desc="Safety check">
    if (momResS2CalcMode && momResS2RunMode) {
        cout << "NeutronResolution::NeutronResolution: calculating and running on momResS2 is illegal! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    SetMomResSlicesByType(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, "TL", SavePath, VaryingDelta, momRes_test, ForceSmallpResLimits);
    LoadFitParam(SampleName, NucleonCutsDirectory, Calculate_momResS2, NeutronResolutionDirectory);
}
//</editor-fold>

// SetMomResSlicesByType function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetMomResSlicesByType function">
void NeutronResolution::SetMomResSlicesByType(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh,
                                              const string &MomentumType, const string &SavePath, const bool &VaryingDelta, const bool &momRes_test,
                                              const bool &ForceSmallpResLimits) {
    SetMomResSlices(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, SavePath, VaryingDelta, momRes_test, ForceSmallpResLimits,
                    ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar, NumberOfSlices);
}
//</editor-fold>

// SetMomResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetMomResSlices function">
void NeutronResolution::SetMomResSlices(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE, const double &ParticleMomTh, const string &SavePath,
                                        const bool &VaryingDelta, const bool &momRes_test, const bool &ForceSmallpResLimits, vector <hPlot1D> &ResSlices0,
                                        vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0,
                                        int &NumberOfSlices0) {
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
        }

        //<editor-fold desc="Safety check">
        if ((abs(SliceUpperLim) > beamE * 1.5) || (abs(SliceLowerLim) > beamE * 1.5)) {
            cout << "NeutronResolution::NeutronResolution: slice limits are not defined properly! Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        int SliceUpperLimPrecision;
        if (SliceUpperLim == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

        string hStatsTitle, hTitle, hSaveName, hCutName;

        hPlot1D hResolutionSlice;

        DSCuts ResSliceFitCuts;

        if (isNeutron) {
            hStatsTitle = "n res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{nFD}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Neutron resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{nFD}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_n_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "R_{nFD} = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}", SlicesSavePath, hSaveName,
                                       hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            ResSliceFitCuts = DSCuts(("fit_" + hCutName), "FD", "Neutron", "1n", 0, -9999, 9999);
        } else if (isProton) {
            hStatsTitle = "p res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{pFD}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Proton resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{truth}_{pFD}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName = to_string(SliceNumber) + "_res_plot_for_TL_P_p_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                        to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            if (!momRes_test || ForceSmallpResLimits) {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                           -0.2, 0.2, hSliceNumOfBin);
//                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
//                                           -0.75, 0.75, hSliceNumOfBin);
            } else {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}", SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            }

            ResSliceFitCuts = DSCuts(("fit_" + hCutName), "FD", "Proton", "1n", 0, -9999, 9999);
        }

        ResSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResSliceFitCuts.SetSliceNumber(SliceNumber);

        ResSlices0.push_back(hResolutionSlice);
        ResSlicesLimits0.push_back({SliceLowerLim, SliceUpperLim});
        ResSlicesFitVar0.push_back(ResSliceFitCuts);

        ResSliceFitCuts.SetCutVariable(("hist_" + hCutName));
        ResSlicesHistVar0.push_back(ResSliceFitCuts);

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
                        } else if ((SliceLowerLim >= 3.00) && (SliceLowerLim < 3.50)) { // 3.00-3.50
                            Delta = delta * 10;
                        } else if (SliceLowerLim >= 3.50) { // 3.00-SliceUpperMomLim
                            Delta = SliceUpperMomLim - SliceLowerLim;
//                        } else if (SliceLowerLim >= 3.00) { // 3.00-SliceUpperMomLim
//                            Delta = SliceUpperMomLim - SliceLowerLim;
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

    NumberOfSlices0 = SliceNumber;
}
//</editor-fold>

// LoadFitParam function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LoadFitParam function">
void NeutronResolution::LoadFitParam(const string &SampleName, const string &NucleonCutsDirectory, bool const &Calculate_momResS2, const string &NeutronResolutionDirectory) {
    if (isNeutron) {
        if (momResS2CalcMode && !momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 calculation mode. Loading momResS1 variables...\n";

            string NeutronCorrectionDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1), but not smearing variables */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            //<editor-fold desc="Safety checks for data files">
            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                cout << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(0);
            }
            //</editor-fold>

            cout << "\nDone.\n";
        } else if (!momResS2CalcMode && momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 run mode.\n";
            cout << "Loading correction from momResS1 variables & smearing from momResS2 variables...\n";

            string NeutronCorrectionDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
            string ProtonSmearingDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1) */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            /* Load proton smearing variables (from momResS2) */
            ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, false, true);

            //<editor-fold desc="Safety checks for data files">
            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                cout << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(0);
            }

            if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
                cout << "\n\nNeutronResolution::NeutronResolution: proton smearing variables are not being loaded from neutron data! Exiting...\n\n", exit(0);
            }
            //</editor-fold>

            cout << "\nDone.\n";
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
    } else {
        cout << "Parameter file didn't read in " << endl;
    }
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
void NeutronResolution::hFillResPlots(const double &TL_momentum, const double &Resolution, const double &Weight) {
    bool Printout = false;

    for (int i = 0; i < NumberOfSlices; i++) {
        if ((TL_momentum >= ResTLMomSlicesLimits.at(i).at(0)) && (TL_momentum < ResTLMomSlicesLimits.at(i).at(1))) {
            ResTLMomSlices.at(i).hFill(Resolution, Weight);

            if (Printout) {
                cout << "\n\nResTLMomSlicesLimits.at(" << i << ").at(0) = " << ResTLMomSlicesLimits.at(i).at(0) << "\n";
                cout << "TL_momentum = " << TL_momentum << "\n";
                cout << "ResTLMomSlicesLimits.at(" << i << ").at(1) = " << ResTLMomSlicesLimits.at(i).at(1) << "\n";
            }

            break; // no need to keep the loop going after filling histogram
        }
    }
}
//</editor-fold>

//<editor-fold desc="hFillResPlots function">
void NeutronResolution::hFillResPlots(const double &Momentum, const double &Resolution, const double &Weight, vector <hPlot1D> &ResSlices0,
                                      vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0, int &NumberOfSlices0) {
    bool Printout = false;

    for (int i = 0; i < NumberOfSlices0; i++) {
        if ((Momentum >= ResSlicesLimits0.at(i).at(0)) && (Momentum < ResSlicesLimits0.at(i).at(1))) {
            ResSlices0.at(i).hFill(Resolution, Weight);

            if (Printout) {
                cout << "\n\nResTLMomSlicesLimits.at(" << i << ").at(0) = " << ResSlicesLimits0.at(i).at(0) << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "ResSlicesLimits.at(" << i << ").at(1) = " << ResSlicesLimits0.at(i).at(1) << "\n";
            }

            break; // no need to keep the loop going after filling histogram
        }
    }
}
//</editor-fold>

// hFillResPlotsByType function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillResPlotsByType function">
void NeutronResolution::hFillResPlotsByType(const double &TL_momentum, const double &Reco_momentum, const double &Resolution, const double &Weight) {
    hFillResPlots(TL_momentum, Resolution, Weight, ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar, NumberOfSlices);
}
//</editor-fold>

// SliceFitDrawAndSave function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="CFitFunction">
/* CFitFunction function for costume fit */
Double_t CFitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}
//</editor-fold>

//<editor-fold desc="SliceFitDrawAndSave function (original)">
void NeutronResolution::SliceFitDrawAndSave(const string &SampleName, const double &beamE) {
    TCanvas *SliceFitCanvas = new TCanvas("SliceFitCanvas", "SliceFitCanvas", 1000, 750); // normal res
    SliceFitCanvas->SetGrid();
    SliceFitCanvas->SetBottomMargin(0.14), SliceFitCanvas->SetLeftMargin(0.18), SliceFitCanvas->SetRightMargin(0.12);
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

        TH1D *hSlice = (TH1D *) ResTLMomSlices.at(i).GetHistogram();
        hSlice->GetXaxis()->SetTitleSize(0.06), hSlice->GetXaxis()->SetLabelSize(0.0425), hSlice->GetXaxis()->CenterTitle(true);
        hSlice->GetYaxis()->SetTitle("Arbitrary units (#events)");
        hSlice->GetYaxis()->SetTitleSize(0.06), hSlice->GetYaxis()->SetLabelSize(0.0425), hSlice->GetYaxis()->CenterTitle(true);
        hSlice->Sumw2();

        if (hSlice->Integral() != 0.) { // Fit only the non-empty histograms
            cout << "\n\n";

            double FitUlim, FitLlim;

            if (momResTestMode) { // In smear & shift test mode
                FitUlim = 1., FitLlim = -1.; // For both neutrons and protons
            } else {
                if (isNeutron) {
                    FitUlim = 1., FitLlim = -1.;
                } else if (isProton) {
                    FitUlim = 0.5, FitLlim = -0.5;
                }
            }

            TF1 *func = new TF1("fit", CFitFunction, FitLlim, FitUlim, 3); // create a function with 3 parameters in the range [-3,3]
            func->SetLineColor(kRed);

            double SliceMax = hSlice->GetMaximum(), SliceMean = hSlice->GetMean(), SliceStd = hSlice->GetRMS();

            func->SetParameters(SliceMax, SliceMean, 0.5); // start fit with histogram's max and mean
            func->SetParNames("Constant", "Mean_value", "Sigma");

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

            hSlice->Fit("fit");
            hSlice->SetLineColor(kBlue);
            hSlice->SetLineWidth(2);

            double FitAmp = func->GetParameter(0);  // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2);  // get p2

            ResTLMomSlicesFitVar.at(i).SetMean(FitMean); // For neutron correction
            ResTLMomSlicesFitVar.at(i).SetUpperCut(FitStd); // For proton smearing
            ResTLMomSlicesHistVar.at(i).SetMean(SliceMean);
            ResTLMomSlicesHistVar.at(i).SetUpperCut(SliceStd);

            double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
            double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
            double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
            double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam - 0.025, "NDC");
            FitParam->SetBorderSize(1), FitParam->SetFillColor(0), FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);
            FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 4)).c_str());
            FitParam->AddText(("Fit #mu = " + to_string_with_precision(FitMean, 4)).c_str());
            FitParam->AddText(("Fit #sigma = " + to_string_with_precision(FitStd, 4)).c_str());
            FitParam->Draw("same");

            int SliceUpperLimPrecision;
            if (ResTLMomSlicesLimits.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

            string hSlice_CloneSaveDir = ResTLMomSlices.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_res_slices/";
            string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + ResTLMomSlices.at(i).GetHistogram1DSaveName() + "_fitted.png";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            auto ListOfFunctions = hSlice->GetListOfFunctions();
            ListOfFunctions->Add((TObject *) FitParam);

            cout << "\n", SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

            if (isNeutron) {
                FittedNeutronResSlices->Add(hSlice);
            } else if (isProton) {
                FittedProtonResSlices->Add(hSlice);
            }

            SliceFitCanvas->Clear();

            FittedTLMomSlices.push_back(i); // Log slices that were fitted
        } else {
            continue;
        }
    }

    Fitter_Std_pol1(), Fitter_Std_pol2(), Fitter_Std_pol3();
    Fitter_Std_pol1_wKC(), Fitter_Std_pol2_wKC(), Fitter_Std_pol3_wKC();
    Fitter_Corr_pol1(), Fitter_Corr_pol2(), Fitter_Corr_pol3();
    Fitter_Corr_pol1_wKC(), Fitter_Corr_pol2_wKC(), Fitter_Corr_pol3_wKC();
}
//</editor-fold>

//<editor-fold desc="SliceFitDrawAndSave function">
void NeutronResolution::SliceFitDrawAndSave(const string &SampleName, const double &beamE, vector <hPlot1D> &ResSlices0, vector <vector<double>> &ResSlicesLimits0,
                                            vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0, vector<int> &FittedSlices0, int &NumberOfSlices0) {
    TCanvas *SliceFitCanvas = new TCanvas("SliceFitCanvas", "SliceFitCanvas", 1000, 750); // normal res
    SliceFitCanvas->SetGrid();
    SliceFitCanvas->SetBottomMargin(0.14), SliceFitCanvas->SetLeftMargin(0.18), SliceFitCanvas->SetRightMargin(0.12);
    SliceFitCanvas->cd();

    for (int i = 0; i < NumberOfSlices0; i++) {

        //<editor-fold desc="Setting sNameFlag">
        string sNameFlag;

        if (findSubstring(SampleName, "sim")) {
            sNameFlag = "s";
        } else if (findSubstring(SampleName, "data")) {
            sNameFlag = "d";
        }
        //</editor-fold>

        TH1D *hSlice = (TH1D *) ResTLMomSlices.at(i).GetHistogram();
        hSlice->GetXaxis()->SetTitleSize(0.06), hSlice->GetXaxis()->SetLabelSize(0.0425), hSlice->GetXaxis()->CenterTitle(true);
        hSlice->GetYaxis()->SetTitle("Arbitrary units (#events)");
        hSlice->GetYaxis()->SetTitleSize(0.06), hSlice->GetYaxis()->SetLabelSize(0.0425), hSlice->GetYaxis()->CenterTitle(true);
        hSlice->Sumw2();

        if (hSlice->Integral() != 0.) { // Fit only the non-empty histograms
            cout << "\n\n";

            double FitUlim, FitLlim;

            if (momResTestMode) { // In smear & shift test mode
                FitUlim = 1., FitLlim = -1.; // For both neutrons and protons
            } else {
                if (isNeutron) {
                    FitUlim = 1., FitLlim = -1.;
                } else if (isProton) {
                    FitUlim = 0.5, FitLlim = -0.5;
                }
            }

            TF1 *func = new TF1("fit", CFitFunction, FitLlim, FitUlim, 3); // create a function with 3 parameters in the range [-3,3]
            func->SetLineColor(kRed);

            double SliceMax = hSlice->GetMaximum(), SliceMean = hSlice->GetMean(), SliceStd = hSlice->GetRMS();

            func->SetParameters(SliceMax, SliceMean, 0.5); // start fit with histogram's max and mean
            func->SetParNames("Constant", "Mean_value", "Sigma");

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

            hSlice->Fit("fit");
            hSlice->SetLineColor(kBlue);
            hSlice->SetLineWidth(2);

            double FitAmp = func->GetParameter(0);  // get p0
            double FitMean = func->GetParameter(1); // get p1
            double FitStd = func->GetParameter(2);  // get p2

            ResSlicesFitVar0.at(i).SetMean(FitMean); // For neutron correction
            ResSlicesFitVar0.at(i).SetUpperCut(FitStd); // For proton smearing
            ResSlicesHistVar0.at(i).SetMean(SliceMean);
            ResSlicesHistVar0.at(i).SetUpperCut(SliceStd);

            double x_1_Cut_legend = gStyle->GetStatX(), y_1_Cut_legend = gStyle->GetStatY() - 0.2;
            double x_2_Cut_legend = gStyle->GetStatX() - 0.2, y_2_Cut_legend = gStyle->GetStatY() - 0.3;
            double x_1_FitParam = x_1_Cut_legend, y_1_FitParam = y_1_Cut_legend;
            double x_2_FitParam = x_2_Cut_legend, y_2_FitParam = y_2_Cut_legend;

            TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam - 0.025, "NDC");
            FitParam->SetBorderSize(1), FitParam->SetFillColor(0), FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);
            FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 4)).c_str());
            FitParam->AddText(("Fit #mu = " + to_string_with_precision(FitMean, 4)).c_str());
            FitParam->AddText(("Fit #sigma = " + to_string_with_precision(FitStd, 4)).c_str());
            FitParam->Draw("same");

            int SliceUpperLimPrecision;
            if (ResSlicesLimits0.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

            string hSlice_CloneSaveDir = ResSlices0.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_res_slices/";
            string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + ResSlices0.at(i).GetHistogram1DSaveName() + "_fitted.png";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            auto ListOfFunctions = hSlice->GetListOfFunctions();
            ListOfFunctions->Add((TObject *) FitParam);

            cout << "\n", SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

            if (isNeutron) {
                FittedNeutronResSlices->Add(hSlice);
            } else if (isProton) {
                FittedProtonResSlices->Add(hSlice);
            }

            SliceFitCanvas->Clear();

            FittedSlices0.push_back(i); // Log slices that were fitted
        } else {
            continue;
        }
    }

    Fitter_Std_pol1(), Fitter_Std_pol2(), Fitter_Std_pol3();
    Fitter_Std_pol1_wKC(), Fitter_Std_pol2_wKC(), Fitter_Std_pol3_wKC();
    Fitter_Corr_pol1(), Fitter_Corr_pol2(), Fitter_Corr_pol3();
    Fitter_Corr_pol1_wKC(), Fitter_Corr_pol2_wKC(), Fitter_Corr_pol3_wKC();
}
//</editor-fold>

// SliceFitDrawAndSaveByType function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SliceFitDrawAndSaveByType function">
void NeutronResolution::SliceFitDrawAndSaveByType(const string &SampleName, const double &beamE) {
    SliceFitDrawAndSave(SampleName, beamE, ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar, FittedTLMomSlices, NumberOfSlices);
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
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Std = ResTLMomSlicesFitVar.at(i).GetUpperCut();

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

    //<editor-fold desc="Safety check">
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

    if (isNeutron) {
        g_Std_pol1->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Std_pol1->GetYaxis()->SetTitle("R_{nFD} fit width");
    } else if (isProton) {
        g_Std_pol1->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Std_pol1->GetYaxis()->SetTitle("R_{pFD} fit width");
    }

    g_Std_pol1->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol1->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol1->GetXaxis()->CenterTitle(true);
    g_Std_pol1->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol1->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol1->GetYaxis()->CenterTitle(true);
    g_Std_pol1->SetTitle("Proton smear fit (no KC, linear fit)");

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

    auto Std_pol1_legend = new TLegend(x_1_Std_legend + 0.075, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
//    auto Std_pol1_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol1_legend_fit;

    if (isNeutron) {
        Std_pol1_legend_fit = Std_pol1_legend->AddEntry(f_Std_pol1, "#sigma(#bar{P}_{nFD}) = A#bar{P}_{nFD} + B", "l");
    } else if (isProton) {
        Std_pol1_legend_fit = Std_pol1_legend->AddEntry(f_Std_pol1, "#sigma(#bar{P}_{pFD}) = A#bar{P}_{pFD} + B", "l");
    }

    Std_pol1_legend->SetTextFont(42);
    Std_pol1_legend->SetTextSize(0.03);
    Std_pol1_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Std_pol1, 4) + " #pm " + to_string_with_precision(A_Std_pol1_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Std_pol1, 4) + " #pm " + to_string_with_precision(B_Std_pol1_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol1 / NDF_Std_pol1)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol1->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol1_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesWidth->Add(g_Std_pol1); } else if (isProton) { FittedProtonResSlicesWidth->Add(g_Std_pol1); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "01_Fit_Std_pol1.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol1_wKC() {
    cout << "\n\nFitter_Std_pol1_wKC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Std = ResTLMomSlicesFitVar.at(i).GetUpperCut();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol1_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol1_wKC = new TGraph();
    g_Std_pol1_wKC->SetName("g_Std_pol1_wKC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol1_wKC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol1_wKC->GetListOfFunctions()->Add(latex);
        }
    }

    if (isNeutron) {
        g_Std_pol1_wKC->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Std_pol1_wKC->GetYaxis()->SetTitle("R_{nFD} fit width");
    } else if (isProton) {
        g_Std_pol1_wKC->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Std_pol1_wKC->GetYaxis()->SetTitle("R_{pFD} fit width");
    }

    g_Std_pol1_wKC->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol1_wKC->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol1_wKC->GetXaxis()->CenterTitle(true);
    g_Std_pol1_wKC->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol1_wKC->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol1_wKC->GetYaxis()->CenterTitle(true);
    g_Std_pol1_wKC->SetTitle("Proton smear fit (wKC, linear fit)");

    TF1 *f_Std_pol1_wKC = new TF1("f_Std_pol1_wKC", "[0] * x + [1]"); // A*x + B

    g_Std_pol1_wKC->Fit(f_Std_pol1_wKC);
    g_Std_pol1_wKC->Draw("ap");
    g_Std_pol1_wKC->SetMarkerStyle(21);

    A_Std_pol1_wKC = f_Std_pol1_wKC->GetParameter(0); // get [0]
    A_Std_pol1_wKC_Error = f_Std_pol1_wKC->GetParError(0); // get [0]
    B_Std_pol1_wKC = f_Std_pol1_wKC->GetParameter(1); // get [1]
    B_Std_pol1_wKC_Error = f_Std_pol1_wKC->GetParError(1); // get [1]
    ChiSquare_Std_pol1_wKC = f_Std_pol1_wKC->GetChisquare(); // ChiSquare
    NDF_Std_pol1_wKC = f_Std_pol1_wKC->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025;

    auto Std_pol1_wKC_legend = new TLegend(x_1_Std_legend + 0.075, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
//    auto Std_pol1_wKC_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol1_wKC_legend_fit;

    if (isNeutron) {
        Std_pol1_wKC_legend_fit = Std_pol1_wKC_legend->AddEntry(f_Std_pol1_wKC, "#sigma(#bar{P}_{nFD}) = A#bar{P}_{nFD} + B", "l");
    } else if (isProton) {
        Std_pol1_wKC_legend_fit = Std_pol1_wKC_legend->AddEntry(f_Std_pol1_wKC, "#sigma(#bar{P}_{pFD}) = A#bar{P}_{pFD} + B", "l");
    }

    Std_pol1_wKC_legend->SetTextFont(42);
    Std_pol1_wKC_legend->SetTextSize(0.03);
    Std_pol1_wKC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Std_pol1_wKC, 4) + " #pm " + to_string_with_precision(A_Std_pol1_wKC_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Std_pol1_wKC, 4) + " #pm " + to_string_with_precision(B_Std_pol1_wKC_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol1_wKC / NDF_Std_pol1_wKC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol1_wKC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol1_wKC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesWidth->Add(g_Std_pol1_wKC); } else if (isProton) { FittedProtonResSlicesWidth->Add(g_Std_pol1_wKC); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "01_Fit_Std_pol1_wKC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol2() {
    cout << "\n\nFitter_Std_pol2 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Std = ResTLMomSlicesFitVar.at(i).GetUpperCut();

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

    //<editor-fold desc="Safety check">
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

    if (isNeutron) {
        g_Std_pol2->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Std_pol2->GetYaxis()->SetTitle("R_{nFD} fit width");
    } else if (isProton) {
        g_Std_pol2->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Std_pol2->GetYaxis()->SetTitle("R_{pFD} fit width");
    }

    g_Std_pol2->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol2->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol2->GetXaxis()->CenterTitle(true);
    g_Std_pol2->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol2->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol2->GetYaxis()->CenterTitle(true);
    g_Std_pol2->SetTitle("Proton smear fit (no KC, quadratic fit)");

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

    auto Std_pol2_legend = new TLegend(x_1_Std_legend + 0.075, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
//    auto Std_pol2_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol2_legend_fit;

    if (isNeutron) {
        Std_pol2_legend_fit = Std_pol2_legend->AddEntry(f_Std_pol2, "#sigma(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{2} + B#bar{P}_{nFD} + C", "l");
    } else if (isProton) {
        Std_pol2_legend_fit = Std_pol2_legend->AddEntry(f_Std_pol2, "#sigma(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{2} + B#bar{P}_{pFD} + C", "l");
    }

    Std_pol2_legend->SetTextFont(42);
    Std_pol2_legend->SetTextSize(0.03);
    Std_pol2_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Std_pol2, 4) + " #pm " + to_string_with_precision(A_Std_pol2_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Std_pol2, 4) + " #pm " + to_string_with_precision(B_Std_pol2_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Std_pol2, 4) + " #pm " + to_string_with_precision(C_Std_pol2_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol2 / NDF_Std_pol2)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol2->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol2_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesWidth->Add(g_Std_pol2); } else if (isProton) { FittedProtonResSlicesWidth->Add(g_Std_pol2); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "02_Fit_Std_pol2.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol2_wKC() {
    cout << "\n\nFitter_Std_pol2_wKC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Std = ResTLMomSlicesFitVar.at(i).GetUpperCut();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol2_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol2_wKC = new TGraph();
    g_Std_pol2_wKC->SetName("g_Std_pol2_wKC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol2_wKC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol2_wKC->GetListOfFunctions()->Add(latex);
        }
    }

    if (isNeutron) {
        g_Std_pol2_wKC->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Std_pol2_wKC->GetYaxis()->SetTitle("R_{nFD} fit width");
    } else if (isProton) {
        g_Std_pol2_wKC->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Std_pol2_wKC->GetYaxis()->SetTitle("R_{pFD} fit width");
    }

    g_Std_pol2_wKC->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol2_wKC->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol2_wKC->GetXaxis()->CenterTitle(true);
    g_Std_pol2_wKC->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol2_wKC->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol2_wKC->GetYaxis()->CenterTitle(true);
    g_Std_pol2_wKC->SetTitle("Proton smear fit (wKC, quadratic fit)");

    TF1 *f_Std_pol2_wKC = new TF1("f_Std_pol2_wKC", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g_Std_pol2_wKC->Fit(f_Std_pol2_wKC);
    g_Std_pol2_wKC->Draw("ap");
    g_Std_pol2_wKC->SetMarkerStyle(21);

    A_Std_pol2_wKC = f_Std_pol2_wKC->GetParameter(0); // get [0]
    A_Std_pol2_wKC_Error = f_Std_pol2_wKC->GetParError(0); // get [0]
    B_Std_pol2_wKC = f_Std_pol2_wKC->GetParameter(1); // get [1]
    B_Std_pol2_wKC_Error = f_Std_pol2_wKC->GetParError(1); // get [1]
    C_Std_pol2_wKC = f_Std_pol2_wKC->GetParameter(2); // get [2]
    C_Std_pol2_wKC_Error = f_Std_pol2_wKC->GetParError(2); // get [2]
    ChiSquare_Std_pol2_wKC = f_Std_pol2_wKC->GetChisquare(); // ChiSquare
    NDF_Std_pol2_wKC = f_Std_pol2_wKC->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std + 0.1, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025 - 0.05;

    auto Std_pol2_wKC_legend = new TLegend(x_1_Std_legend + 0.075, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
//    auto Std_pol2_wKC_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol2_wKC_legend_fit;

    if (isNeutron) {
        Std_pol2_wKC_legend_fit = Std_pol2_wKC_legend->AddEntry(f_Std_pol2_wKC, "#sigma(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{2} + B#bar{P}_{nFD} + C", "l");
    } else if (isProton) {
        Std_pol2_wKC_legend_fit = Std_pol2_wKC_legend->AddEntry(f_Std_pol2_wKC, "#sigma(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{2} + B#bar{P}_{pFD} + C", "l");
    }

    Std_pol2_wKC_legend->SetTextFont(42);
    Std_pol2_wKC_legend->SetTextSize(0.03);
    Std_pol2_wKC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Std_pol2_wKC, 4) + " #pm " + to_string_with_precision(A_Std_pol2_wKC_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Std_pol2_wKC, 4) + " #pm " + to_string_with_precision(B_Std_pol2_wKC_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Std_pol2_wKC, 4) + " #pm " + to_string_with_precision(C_Std_pol2_wKC_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol2_wKC / NDF_Std_pol2_wKC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol2_wKC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol2_wKC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesWidth->Add(g_Std_pol2_wKC); } else if (isProton) { FittedProtonResSlicesWidth->Add(g_Std_pol2_wKC); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "02_Fit_Std_pol2_wKC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol3() {
    cout << "\n\nFitter_Std_pol3 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Std = ResTLMomSlicesFitVar.at(i).GetUpperCut();

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

    //<editor-fold desc="Safety check">
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

    if (isNeutron) {
        g_Std_pol3->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Std_pol3->GetYaxis()->SetTitle("R_{nFD} fit width");
    } else if (isProton) {
        g_Std_pol3->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Std_pol3->GetYaxis()->SetTitle("R_{pFD} fit width");
    }

    g_Std_pol3->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol3->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol3->GetXaxis()->CenterTitle(true);
    g_Std_pol3->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol3->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol3->GetYaxis()->CenterTitle(true);
    g_Std_pol3->SetTitle("Proton smear fit (no KC, cubic fit)");

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

    auto Std_pol3_legend = new TLegend(x_1_Std_legend + 0.075, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
//    auto Std_pol3_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol3_legend_fit;

    if (isNeutron) {
        Std_pol3_legend_fit = Std_pol3_legend->AddEntry(f_Std_pol3, "#sigma(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{3} + B#bar{P}_{nFD}^{2} + C#bar{P}_{nFD} + D", "l");
    } else if (isProton) {
        Std_pol3_legend_fit = Std_pol3_legend->AddEntry(f_Std_pol3, "#sigma(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{3} + B#bar{P}_{pFD}^{2} + C#bar{P}_{pFD} + D", "l");
    }

    Std_pol3_legend->SetTextFont(42);
    Std_pol3_legend->SetTextSize(0.03);
    Std_pol3_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Std_pol3, 4) + " #pm " + to_string_with_precision(A_Std_pol3_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Std_pol3, 4) + " #pm " + to_string_with_precision(B_Std_pol3_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Std_pol3, 4) + " #pm " + to_string_with_precision(C_Std_pol3_Error, 4)).c_str());
    FitParam->AddText(("D = " + to_string_with_precision(D_Std_pol3, 4) + " #pm " + to_string_with_precision(D_Std_pol3_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol3 / NDF_Std_pol3)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol3->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol3_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesWidth->Add(g_Std_pol3); } else if (isProton) { FittedProtonResSlicesWidth->Add(g_Std_pol3); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "03_Fit_Std_pol3.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Std_pol3_wKC() {
    cout << "\n\nFitter_Std_pol3_wKC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Std;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Std = ResTLMomSlicesFitVar.at(i).GetUpperCut();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Std.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol3_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Std_pol3_wKC = new TGraph();
    g_Std_pol3_wKC->SetName("g_Std_pol3_wKC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Std.at(i);

        g_Std_pol3_wKC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Std_pol3_wKC->GetListOfFunctions()->Add(latex);
        }
    }

    if (isNeutron) {
        g_Std_pol3_wKC->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Std_pol3_wKC->GetYaxis()->SetTitle("R_{nFD} fit width");
    } else if (isProton) {
        g_Std_pol3_wKC->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Std_pol3_wKC->GetYaxis()->SetTitle("R_{pFD} fit width");
    }

    g_Std_pol3_wKC->GetXaxis()->SetTitleSize(0.06);
    g_Std_pol3_wKC->GetXaxis()->SetLabelSize(0.0425);
    g_Std_pol3_wKC->GetXaxis()->CenterTitle(true);
    g_Std_pol3_wKC->GetYaxis()->SetTitleSize(0.06);
    g_Std_pol3_wKC->GetYaxis()->SetLabelSize(0.0425);
    g_Std_pol3_wKC->GetYaxis()->CenterTitle(true);
    g_Std_pol3_wKC->SetTitle("Proton smear fit (wKC, cubic fit)");

    TF1 *f_Std_pol3_wKC = new TF1("f_Std_pol3_wKC", "[0] * x * x * x + [1] * x * x + [2] * x + [3]"); // A*x*x*x + B*x*x + C*x + D

    g_Std_pol3_wKC->Fit(f_Std_pol3_wKC);
    g_Std_pol3_wKC->Draw("ap");
    g_Std_pol3_wKC->SetMarkerStyle(21);

    A_Std_pol3_wKC = f_Std_pol3_wKC->GetParameter(0); // get [0]
    A_Std_pol3_wKC_Error = f_Std_pol3_wKC->GetParError(0); // get [0]
    B_Std_pol3_wKC = f_Std_pol3_wKC->GetParameter(1); // get [1]
    B_Std_pol3_wKC_Error = f_Std_pol3_wKC->GetParError(1); // get [1]
    C_Std_pol3_wKC = f_Std_pol3_wKC->GetParameter(2); // get [2]
    C_Std_pol3_wKC_Error = f_Std_pol3_wKC->GetParError(2); // get [2]
    D_Std_pol3_wKC = f_Std_pol3_wKC->GetParameter(3); // get [3]
    D_Std_pol3_wKC_Error = f_Std_pol3_wKC->GetParError(3); // get [3]
    ChiSquare_Std_pol3_wKC = f_Std_pol3_wKC->GetChisquare(); // ChiSquare
    NDF_Std_pol3_wKC = f_Std_pol3_wKC->GetNDF(); // NDF

    double x_1_Std = gStyle->GetStatX() - 0.6, y_1_Std = gStyle->GetStatY() - 0.2;
    double x_2_Std = gStyle->GetStatX() - 0.2 - 0.6, y_2_Std = gStyle->GetStatY() - 0.3;
    double x_1_Std_legend = x_1_Std + 0.1 + 0.075, y_1_Std_legend = y_1_Std + 0.125;
    double x_2_Std_legend = x_2_Std, y_2_Std_legend = y_2_Std + 0.15;
    double x_1_FitParam = x_1_Std, y_1_FitParam = y_1_Std + 0.025;
    double x_2_FitParam = x_2_Std, y_2_FitParam = y_2_Std + 0.025 - 0.05 - 0.05;

    auto Std_pol3_wKC_legend = new TLegend(x_1_Std_legend + 0.075, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
//    auto Std_pol3_wKC_legend = new TLegend(x_1_Std_legend, y_1_Std_legend, x_2_Std_legend, y_2_Std_legend);
    TLegendEntry *Std_pol3_wKC_legend_fit;

    if (isNeutron) {
        Std_pol3_wKC_legend_fit = Std_pol3_wKC_legend->AddEntry(f_Std_pol3_wKC,
                                                                "#sigma(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{3} + B#bar{P}_{nFD}^{2} + C#bar{P}_{nFD} + D", "l");
    } else if (isProton) {
        Std_pol3_wKC_legend_fit = Std_pol3_wKC_legend->AddEntry(f_Std_pol3_wKC,
                                                                "#sigma(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{3} + B#bar{P}_{pFD}^{2} + C#bar{P}_{pFD} + D", "l");
    }

    Std_pol3_wKC_legend->SetTextFont(42);
    Std_pol3_wKC_legend->SetTextSize(0.03);
    Std_pol3_wKC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Std_pol3_wKC, 4) + " #pm " + to_string_with_precision(A_Std_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Std_pol3_wKC, 4) + " #pm " + to_string_with_precision(B_Std_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Std_pol3_wKC, 4) + " #pm " + to_string_with_precision(C_Std_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("D = " + to_string_with_precision(D_Std_pol3_wKC, 4) + " #pm " + to_string_with_precision(D_Std_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Std_pol3_wKC / NDF_Std_pol3_wKC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Std_pol3_wKC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Std_pol3_wKC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesWidth->Add(g_Std_pol3_wKC); } else if (isProton) { FittedProtonResSlicesWidth->Add(g_Std_pol3_wKC); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "03_Fit_Std_pol3_wKC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol1() {
    cout << "\n\nFitter_Corr_pol1 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResTLMomSlicesFitVar.at(i).GetMean();

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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol1_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
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

    if (isNeutron) {
        g_Corr_pol1->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Corr_pol1->GetYaxis()->SetTitle("R_{nFD} fit mean");
        g_Corr_pol1->SetTitle("Neutron correction fit (no KC, linear fit)");
    } else if (isProton) {
        g_Corr_pol1->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Corr_pol1->GetYaxis()->SetTitle("R_{pFD} fit mean");
        g_Corr_pol1->SetTitle("Proton correction fit (no KC, linear fit)");
    }

    g_Corr_pol1->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol1->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol1->GetXaxis()->CenterTitle(true);
    g_Corr_pol1->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol1->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol1->GetYaxis()->CenterTitle(true);

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

    auto Corr_pol1_legend = new TLegend(x_1_Corr_legend + 0.075, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
//    auto Corr_pol1_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol1_legend_fit;

    if (isNeutron) {
        Corr_pol1_legend_fit = Corr_pol1_legend->AddEntry(f_Corr_pol1, "#mu(#bar{P}_{nFD}) = A#bar{P}_{nFD} + B", "l");
    } else if (isProton) {
        Corr_pol1_legend_fit = Corr_pol1_legend->AddEntry(f_Corr_pol1, "#mu(#bar{P}_{pFD}) = A#bar{P}_{pFD} + B", "l");
    }

    Corr_pol1_legend->SetTextFont(42);
    Corr_pol1_legend->SetTextSize(0.03);
    Corr_pol1_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Corr_pol1, 4) + " #pm " + to_string_with_precision(A_Corr_pol1_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Corr_pol1, 4) + " #pm " + to_string_with_precision(B_Corr_pol1_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol1 / NDF_Corr_pol1)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol1->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol1_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesMean->Add(g_Corr_pol1); } else if (isProton) { FittedProtonResSlicesMean->Add(g_Corr_pol1); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "04_Fit_Corr_pol1.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol1_wKC() {
    cout << "\n\nFitter_Corr_pol1_wKC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResTLMomSlicesFitVar.at(i).GetMean();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol1_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol1_wKC = new TGraph();
    g_Corr_pol1_wKC->SetName("g_Corr_pol1_wKC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol1_wKC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol1_wKC->GetListOfFunctions()->Add(latex);
        }
    }

    if (isNeutron) {
        g_Corr_pol1_wKC->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Corr_pol1_wKC->GetYaxis()->SetTitle("R_{nFD} fit mean");
        g_Corr_pol1_wKC->SetTitle("Neutron correction fit (wKC, linear fit)");
    } else if (isProton) {
        g_Corr_pol1_wKC->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Corr_pol1_wKC->GetYaxis()->SetTitle("R_{pFD} fit mean");
        g_Corr_pol1_wKC->SetTitle("Proton correction fit (wKC, linear fit)");
    }

    g_Corr_pol1_wKC->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol1_wKC->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol1_wKC->GetXaxis()->CenterTitle(true);
    g_Corr_pol1_wKC->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol1_wKC->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol1_wKC->GetYaxis()->CenterTitle(true);

    TF1 *f_Corr_pol1_wKC = new TF1("f_Corr_pol1_wKC", "[0] * x + [1]"); // A*x + B

    g_Corr_pol1_wKC->Fit(f_Corr_pol1_wKC);
    g_Corr_pol1_wKC->Draw("ap");
    g_Corr_pol1_wKC->SetMarkerStyle(21);

    A_Corr_pol1_wKC = f_Corr_pol1_wKC->GetParameter(0); // get [0]
    A_Corr_pol1_wKC_Error = f_Corr_pol1_wKC->GetParError(0); // get [0]
    B_Corr_pol1_wKC = f_Corr_pol1_wKC->GetParameter(1); // get [1]
    B_Corr_pol1_wKC_Error = f_Corr_pol1_wKC->GetParError(1); // get [1]
    ChiSquare_Corr_pol1_wKC = f_Corr_pol1_wKC->GetChisquare(); // ChiSquare
    NDF_Corr_pol1_wKC = f_Corr_pol1_wKC->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025;

    auto Corr_pol1_wKC_legend = new TLegend(x_1_Corr_legend + 0.075, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
//    auto Corr_pol1_wKC_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol1_wKC_legend_fit;

    if (isNeutron) {
        Corr_pol1_wKC_legend_fit = Corr_pol1_wKC_legend->AddEntry(f_Corr_pol1_wKC, "#mu(#bar{P}_{nFD}) = A#bar{P}_{nFD} + B", "l");
    } else if (isProton) {
        Corr_pol1_wKC_legend_fit = Corr_pol1_wKC_legend->AddEntry(f_Corr_pol1_wKC, "#mu(#bar{P}_{pFD}) = A#bar{P}_{pFD} + B", "l");
    }

    Corr_pol1_wKC_legend->SetTextFont(42);
    Corr_pol1_wKC_legend->SetTextSize(0.03);
    Corr_pol1_wKC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Corr_pol1_wKC, 4) + " #pm " + to_string_with_precision(A_Corr_pol1_wKC_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Corr_pol1_wKC, 4) + " #pm " + to_string_with_precision(B_Corr_pol1_wKC_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol1_wKC / NDF_Corr_pol1_wKC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol1_wKC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol1_wKC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesMean->Add(g_Corr_pol1_wKC); } else if (isProton) { FittedProtonResSlicesMean->Add(g_Corr_pol1_wKC); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "04_Fit_Corr_pol1_wKC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol2() {
    cout << "\n\nFitter_Corr_pol2 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResTLMomSlicesFitVar.at(i).GetMean();

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

    //<editor-fold desc="Safety check">
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

    if (isNeutron) {
        g_Corr_pol2->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Corr_pol2->GetYaxis()->SetTitle("R_{nFD} fit mean");
        g_Corr_pol2->SetTitle("Neutron correction fit (no KC, quadratic fit)");
    } else if (isProton) {
        g_Corr_pol2->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Corr_pol2->GetYaxis()->SetTitle("R_{pFD} fit mean");
        g_Corr_pol2->SetTitle("Proton correction fit (no KC, quadratic fit)");
    }

    g_Corr_pol2->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol2->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol2->GetXaxis()->CenterTitle(true);
    g_Corr_pol2->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol2->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol2->GetYaxis()->CenterTitle(true);

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

    auto Corr_pol2_legend = new TLegend(x_1_Corr_legend + 0.075, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
//    auto Corr_pol2_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol2_legend_fit;

    if (isNeutron) {
        Corr_pol2_legend_fit = Corr_pol2_legend->AddEntry(f_Corr_pol2, "#mu(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{2} + B#bar{P}_{nFD} + C", "l");
    } else if (isProton) {
        Corr_pol2_legend_fit = Corr_pol2_legend->AddEntry(f_Corr_pol2, "#mu(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{2} + B#bar{P}_{pFD} + C", "l");
    }

    Corr_pol2_legend->SetTextFont(42);
    Corr_pol2_legend->SetTextSize(0.03);
    Corr_pol2_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Corr_pol2, 4) + " #pm " + to_string_with_precision(A_Corr_pol2_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Corr_pol2, 4) + " #pm " + to_string_with_precision(B_Corr_pol2_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Corr_pol2, 4) + " #pm " + to_string_with_precision(C_Corr_pol2_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol2 / NDF_Corr_pol2)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol2->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol2_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesMean->Add(g_Corr_pol2); } else if (isProton) { FittedProtonResSlicesMean->Add(g_Corr_pol2); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "05_Fit_Corr_pol2.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol2_wKC() {
    cout << "\n\nFitter_Corr_pol2_wKC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResTLMomSlicesFitVar.at(i).GetMean();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol2_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol2_wKC = new TGraph();
    g_Corr_pol2_wKC->SetName("g_Corr_pol2_wKC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol2_wKC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol2_wKC->GetListOfFunctions()->Add(latex);
        }
    }

    if (isNeutron) {
        g_Corr_pol2_wKC->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Corr_pol2_wKC->GetYaxis()->SetTitle("R_{nFD} fit mean");
        g_Corr_pol2_wKC->SetTitle("Neutron correction fit (wKC, quadratic fit)");
    } else if (isProton) {
        g_Corr_pol2_wKC->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Corr_pol2_wKC->GetYaxis()->SetTitle("R_{pFD} fit mean");
        g_Corr_pol2_wKC->SetTitle("Proton correction fit (wKC, quadratic fit)");
    }

    g_Corr_pol2_wKC->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol2_wKC->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol2_wKC->GetXaxis()->CenterTitle(true);
    g_Corr_pol2_wKC->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol2_wKC->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol2_wKC->GetYaxis()->CenterTitle(true);

    TF1 *f_Corr_pol2_wKC = new TF1("f_Corr_pol2_wKC", "[0] * x * x + [1] * x + [2]"); // A*x*x + B*x + C

    g_Corr_pol2_wKC->Fit(f_Corr_pol2_wKC);
    g_Corr_pol2_wKC->Draw("ap");
    g_Corr_pol2_wKC->SetMarkerStyle(21);

    A_Corr_pol2_wKC = f_Corr_pol2_wKC->GetParameter(0); // get [0]
    A_Corr_pol2_wKC_Error = f_Corr_pol2_wKC->GetParError(0); // get [0]
    B_Corr_pol2_wKC = f_Corr_pol2_wKC->GetParameter(1); // get [1]
    B_Corr_pol2_wKC_Error = f_Corr_pol2_wKC->GetParError(1); // get [1]
    C_Corr_pol2_wKC = f_Corr_pol2_wKC->GetParameter(2); // get [2]
    C_Corr_pol2_wKC_Error = f_Corr_pol2_wKC->GetParError(2); // get [2]
    ChiSquare_Corr_pol2_wKC = f_Corr_pol2_wKC->GetChisquare(); // ChiSquare
    NDF_Corr_pol2_wKC = f_Corr_pol2_wKC->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr + 0.1, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025 - 0.05;

    auto Corr_pol2_wKC_legend = new TLegend(x_1_Corr_legend + 0.075, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
//    auto Corr_pol2_wKC_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol2_wKC_legend_fit;

    if (isNeutron) {
        Corr_pol2_wKC_legend_fit = Corr_pol2_wKC_legend->AddEntry(f_Corr_pol2_wKC, "#mu(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{2} + B#bar{P}_{nFD} + C", "l");
    } else if (isProton) {
        Corr_pol2_wKC_legend_fit = Corr_pol2_wKC_legend->AddEntry(f_Corr_pol2_wKC, "#mu(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{2} + B#bar{P}_{pFD} + C", "l");
    }

    Corr_pol2_wKC_legend->SetTextFont(42);
    Corr_pol2_wKC_legend->SetTextSize(0.03);
    Corr_pol2_wKC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Corr_pol2_wKC, 4) + " #pm " + to_string_with_precision(A_Corr_pol2_wKC_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Corr_pol2_wKC, 4) + " #pm " + to_string_with_precision(B_Corr_pol2_wKC_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Corr_pol2_wKC, 4) + " #pm " + to_string_with_precision(C_Corr_pol2_wKC_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol2_wKC / NDF_Corr_pol2_wKC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol2_wKC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol2_wKC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesMean->Add(g_Corr_pol2_wKC); } else if (isProton) { FittedProtonResSlicesMean->Add(g_Corr_pol2_wKC); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "05_Fit_Corr_pol2_wKC.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol3() {
    cout << "\n\nFitter_Corr_pol3 variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResTLMomSlicesFitVar.at(i).GetMean();

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

    //<editor-fold desc="Safety check">
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

    if (isNeutron) {
        g_Corr_pol3->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Corr_pol3->GetYaxis()->SetTitle("R_{nFD} fit mean");
        g_Corr_pol3->SetTitle("Neutron correction fit (no KC, cubic fit)");
    } else if (isProton) {
        g_Corr_pol3->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Corr_pol3->GetYaxis()->SetTitle("R_{pFD} fit mean");
        g_Corr_pol3->SetTitle("Proton correction fit (no KC, cubic fit)");
    }

    g_Corr_pol3->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol3->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol3->GetXaxis()->CenterTitle(true);
    g_Corr_pol3->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol3->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol3->GetYaxis()->CenterTitle(true);

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

    auto Corr_pol3_legend = new TLegend(x_1_Corr_legend + 0.075, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
//    auto Corr_pol3_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol3_legend_fit;

    if (isNeutron) {
        Corr_pol3_legend_fit = Corr_pol3_legend->AddEntry(f_Corr_pol3, "#mu(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{3} + B#bar{P}_{nFD}^{2} + C#bar{P}_{nFD} + D", "l");
    } else if (isProton) {
        Corr_pol3_legend_fit = Corr_pol3_legend->AddEntry(f_Corr_pol3, "#mu(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{3} + B#bar{P}_{pFD}^{2} + C#bar{P}_{pFD} + D", "l");
    }

    Corr_pol3_legend->SetTextFont(42);
    Corr_pol3_legend->SetTextSize(0.03);
    Corr_pol3_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Corr_pol3, 4) + " #pm " + to_string_with_precision(A_Corr_pol3_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Corr_pol3, 4) + " #pm " + to_string_with_precision(B_Corr_pol3_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Corr_pol3, 4) + " #pm " + to_string_with_precision(C_Corr_pol3_Error, 4)).c_str());
    FitParam->AddText(("D = " + to_string_with_precision(D_Corr_pol3, 4) + " #pm " + to_string_with_precision(D_Corr_pol3_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol3 / NDF_Corr_pol3)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol3->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol3_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesMean->Add(g_Corr_pol3); } else if (isProton) { FittedProtonResSlicesMean->Add(g_Corr_pol3); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "06_Fit_Corr_pol3.png").c_str());
    Fit_Canvas->Clear();
}

void NeutronResolution::Fitter_Corr_pol3_wKC() {
    cout << "\n\nFitter_Corr_pol3_wKC variables:\n";
    bool PlotPoints = false;

    //<editor-fold desc="Setting plot x and y data">
    bool PrintOut = false;

    vector<double> MeanPn, Pn_Corr;

    for (int i = 0; i < NumberOfSlices; i++) {
        double Mean = (ResTLMomSlicesLimits.at(i).at(1) + ResTLMomSlicesLimits.at(i).at(0)) / 2;
        double Corr = ResTLMomSlicesFitVar.at(i).GetMean();

        if (PrintOut) { cout << "\n"; }

        if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
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

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_Corr.size()) {
        cout << "\n\nNeutronResolution::Fitter_Corr_pol3_wKC: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14);
    Fit_Canvas->SetLeftMargin(0.16);
    Fit_Canvas->SetRightMargin(0.12);

    TGraph *g_Corr_pol3_wKC = new TGraph();
    g_Corr_pol3_wKC->SetName("g_Corr_pol3_wKC");

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_Corr.at(i);

        g_Corr_pol3_wKC->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            g_Corr_pol3_wKC->GetListOfFunctions()->Add(latex);
        }
    }

    if (isNeutron) {
        g_Corr_pol3_wKC->GetXaxis()->SetTitle("#bar{P}_{nFD} [GeV/c]");
        g_Corr_pol3_wKC->GetYaxis()->SetTitle("R_{nFD} fit mean");
        g_Corr_pol3_wKC->SetTitle("Neutron correction fit (wKC, cubic fit)");
    } else if (isProton) {
        g_Corr_pol3_wKC->GetXaxis()->SetTitle("#bar{P}_{pFD} [GeV/c]");
        g_Corr_pol3_wKC->GetYaxis()->SetTitle("R_{pFD} fit mean");
        g_Corr_pol3_wKC->SetTitle("Proton correction fit (wKC, cubic fit)");
    }

    g_Corr_pol3_wKC->GetXaxis()->SetTitleSize(0.06);
    g_Corr_pol3_wKC->GetXaxis()->SetLabelSize(0.0425);
    g_Corr_pol3_wKC->GetXaxis()->CenterTitle(true);
    g_Corr_pol3_wKC->GetYaxis()->SetTitleSize(0.06);
    g_Corr_pol3_wKC->GetYaxis()->SetLabelSize(0.0425);
    g_Corr_pol3_wKC->GetYaxis()->CenterTitle(true);

    TF1 *f_Corr_pol3_wKC = new TF1("f_Corr_pol3_wKC", "[0] * x * x * x + [1] * x * x + [2] * x + [3]"); // A*x*x*x + B*x*x + C*x + D

    g_Corr_pol3_wKC->Fit(f_Corr_pol3_wKC);
    g_Corr_pol3_wKC->Draw("ap");
    g_Corr_pol3_wKC->SetMarkerStyle(21);

    A_Corr_pol3_wKC = f_Corr_pol3_wKC->GetParameter(0); // get [0]
    A_Corr_pol3_wKC_Error = f_Corr_pol3_wKC->GetParError(0); // get [0]
    B_Corr_pol3_wKC = f_Corr_pol3_wKC->GetParameter(1); // get [1]
    B_Corr_pol3_wKC_Error = f_Corr_pol3_wKC->GetParError(1); // get [1]
    C_Corr_pol3_wKC = f_Corr_pol3_wKC->GetParameter(2); // get [2]
    C_Corr_pol3_wKC_Error = f_Corr_pol3_wKC->GetParError(2); // get [2]
    D_Corr_pol3_wKC = f_Corr_pol3_wKC->GetParameter(3); // get [3]
    D_Corr_pol3_wKC_Error = f_Corr_pol3_wKC->GetParError(3); // get [3]
    ChiSquare_Corr_pol3_wKC = f_Corr_pol3_wKC->GetChisquare(); // ChiSquare
    NDF_Corr_pol3_wKC = f_Corr_pol3_wKC->GetNDF(); // NDF

    double x_1_Corr = gStyle->GetStatX() - 0.6, y_1_Corr = gStyle->GetStatY() - 0.2;
    double x_2_Corr = gStyle->GetStatX() - 0.2 - 0.6, y_2_Corr = gStyle->GetStatY() - 0.3;
    double x_1_Corr_legend = x_1_Corr + 0.1 + 0.075, y_1_Corr_legend = y_1_Corr + 0.125;
    double x_2_Corr_legend = x_2_Corr, y_2_Corr_legend = y_2_Corr + 0.15;
    double x_1_FitParam = x_1_Corr, y_1_FitParam = y_1_Corr + 0.025;
    double x_2_FitParam = x_2_Corr, y_2_FitParam = y_2_Corr + 0.025 - 0.05 - 0.05;

    auto Corr_pol3_wKC_legend = new TLegend(x_1_Corr_legend + 0.075, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
//    auto Corr_pol3_wKC_legend = new TLegend(x_1_Corr_legend, y_1_Corr_legend, x_2_Corr_legend, y_2_Corr_legend);
    TLegendEntry *Corr_pol3_wKC_legend_fit;
    if (isNeutron) {
        Corr_pol3_wKC_legend_fit = Corr_pol3_wKC_legend->AddEntry(f_Corr_pol3_wKC,
                                                                  "#mu(#bar{P}_{nFD}) = A#bar{P}_{nFD}^{3} + B#bar{P}_{nFD}^{2} + C#bar{P}_{nFD} + D", "l");
    } else if (isProton) {
        Corr_pol3_wKC_legend_fit = Corr_pol3_wKC_legend->AddEntry(f_Corr_pol3_wKC,
                                                                  "#mu(#bar{P}_{pFD}) = A#bar{P}_{pFD}^{3} + B#bar{P}_{pFD}^{2} + C#bar{P}_{pFD} + D", "l");
    }

    Corr_pol3_wKC_legend->SetTextFont(42);
    Corr_pol3_wKC_legend->SetTextSize(0.03);
    Corr_pol3_wKC_legend->Draw("same");

    TPaveText *FitParam = new TPaveText(x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1);
    FitParam->SetFillColor(0);
    FitParam->SetTextAlign(12);
    FitParam->SetTextFont(42);
    FitParam->SetTextSize(0.03);
    FitParam->AddText(("A = " + to_string_with_precision(A_Corr_pol3_wKC, 4) + " #pm " + to_string_with_precision(A_Corr_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("B = " + to_string_with_precision(B_Corr_pol3_wKC, 4) + " #pm " + to_string_with_precision(B_Corr_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("C = " + to_string_with_precision(C_Corr_pol3_wKC, 4) + " #pm " + to_string_with_precision(C_Corr_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("D = " + to_string_with_precision(D_Corr_pol3_wKC, 4) + " #pm " + to_string_with_precision(D_Corr_pol3_wKC_Error, 4)).c_str());
    FitParam->AddText(("#chi^{2}/NDF = " + to_string(ChiSquare_Corr_pol3_wKC / NDF_Corr_pol3_wKC)).c_str());
    FitParam->Draw("same");

    cout << "\n\n";

    auto ListOfFunctions = g_Corr_pol3_wKC->GetListOfFunctions();
    ListOfFunctions->Add((TObject *) Corr_pol3_wKC_legend);
    ListOfFunctions->Add((TObject *) FitParam);

    if (isNeutron) { FittedNeutronResSlicesMean->Add(g_Corr_pol3_wKC); } else if (isProton) { FittedProtonResSlicesMean->Add(g_Corr_pol3_wKC); }

    cout << "\n", Fit_Canvas->SaveAs((SlicesSavePath + "/" + "06_Fit_Corr_pol3_wKC.png").c_str());
    Fit_Canvas->Clear();
}
//</editor-fold>

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveResSlices function">
void NeutronResolution::DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path,
                                             const string &NeutronResolutionDirectory) {
    string SampleNameTemp = SampleName;

    if (isNeutron) {
        ResSlicePlots->Add(FittedNeutronResSlices), ResSlicePlots->Add(FittedNeutronResSlicesWidth), ResSlicePlots->Add(FittedNeutronResSlicesMean);
    } else if (isProton) {
        ResSlicePlots->Add(FittedProtonResSlices), ResSlicePlots->Add(FittedProtonResSlicesWidth), ResSlicePlots->Add(FittedProtonResSlicesMean);
    }

//    for (int i = 0; i < NumberOfSlices; i++) {
////        ResSlices.at(i).ClearListOfFunctions(), ResSlices.at(i).SetShowPlotCuts();
//        ResSlices.at(i).hDrawAndSave(SampleNameTemp, h1DCanvas, ResSlicePlots, false, true, 1., 9999, 9999, 0, false);
//    }

    /* Save res and fitted res plots to plots directory: */
    TFile *PlotsFolder_fout = new TFile((plots_path + "/" + MomResParticle + "_resolution_plots_-_" + SampleName + ".root").c_str(), "recreate");
    PlotsFolder_fout->cd();
    ResSlicePlots->Write();
    PlotsFolder_fout->Write();
    PlotsFolder_fout->Close();
}
//</editor-fold>

// LogResDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogResDataToFile function">
void NeutronResolution::LogResDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    //TODO: reorder file save in test mode properly
    string SaveDateDir = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/";

    if (!momResTestMode) {
        system(("mkdir -p " + SaveDateDir).c_str());

        LogFitDataToFile(SampleName, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
        LogHistDataToFile(SampleName, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
    } else {
        LogFitDataToFile(SampleName, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
        LogHistDataToFile(SampleName, plots_path, SaveDateDir, Nucleon_Cuts_Status, FD_photons_Status, Efficiency_Status);
    }
}
//</editor-fold>

// LogFitDataToFile function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogFitDataToFile function">
void NeutronResolution::LogFitDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                                         const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    ofstream Neutron_res_fit_param;
    std::string Neutron_res_fit_paramFilePath;

    if (momResS2CalcMode) {
        if (!momResTestMode) {
            Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_fit_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
        }
    } else {
        if (momResS2RunMode) {
            if (!momResTestMode) {
                Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_fit_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_fit_param_-_" + SampleName + ".par";
            }
        } else {
            if (!momResTestMode) {
                Neutron_res_fit_paramFilePath = NeutronResolutionDirectory + MomResParticle + "_momResS1_fit_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_fit_paramFilePath = plots_path + "/" + MomResParticle + "_momResS1_fit_param_-_" + SampleName + ".par";
            }
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
    Neutron_res_fit_param << "# " + MomResParticle + " resolution parameters are for:\n";
    Neutron_res_fit_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_fit_param << "#Analysis_run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_fit_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
    //</editor-fold>

    //<editor-fold desc="Logging slice fit results">
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n\n";

    for (int FittedSlice: FittedTLMomSlices) {
        DSCuts TempCut = ResTLMomSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":"
                              << TempCut.GetSliceUpperb() << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }
    //</editor-fold>

    //<editor-fold desc="Logging Stdection and smear fit variables">
    if (isNeutron) {
        Neutron_res_fit_param << "\n\n#correction and smear fit variables:\n\n";

        Neutron_res_fit_param << "SliceUpperMomLimKC" << "\t\t\t" << SliceUpperMomLimKC << "\n";
        Neutron_res_fit_param << "SliceLowerMomLimKC" << "\t\t\t" << SliceLowerMomLimKC << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol1" << "\t\t\t\t\t" << A_Corr_pol1 << "\n";
        Neutron_res_fit_param << "A_Corr_pol1_Error" << "\t\t\t" << A_Corr_pol1_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol1" << "\t\t\t\t\t" << B_Corr_pol1 << "\n";
        Neutron_res_fit_param << "B_Corr_pol1_Error" << "\t\t\t" << B_Corr_pol1_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol1" << "\t\t\t" << ChiSquare_Corr_pol1 << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol1" << "\t\t\t\t" << NDF_Corr_pol1 << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol1_wKC" << "\t\t\t\t" << A_Corr_pol1_wKC << "\n";
        Neutron_res_fit_param << "A_Corr_pol1_wKC_Error" << "\t\t" << A_Corr_pol1_wKC_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol1_wKC" << "\t\t\t\t" << B_Corr_pol1_wKC << "\n";
        Neutron_res_fit_param << "B_Corr_pol1_wKC_Error" << "\t\t" << B_Corr_pol1_wKC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol1_wKC" << "\t\t" << ChiSquare_Corr_pol1_wKC << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol1_wKC" << "\t\t\t" << NDF_Corr_pol1_wKC << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol2" << "\t\t\t\t\t" << A_Corr_pol2 << "\n";
        Neutron_res_fit_param << "A_Corr_pol2_Error" << "\t\t\t" << A_Corr_pol2_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol2" << "\t\t\t\t\t" << B_Corr_pol2 << "\n";
        Neutron_res_fit_param << "B_Corr_pol2_Error" << "\t\t\t" << B_Corr_pol2_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol2" << "\t\t\t\t\t" << C_Corr_pol2 << "\n";
        Neutron_res_fit_param << "C_Corr_pol2_Error" << "\t\t\t" << C_Corr_pol2_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol2" << "\t\t\t" << ChiSquare_Corr_pol2 << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol2" << "\t\t\t\t" << NDF_Corr_pol2 << "\n\n";

        Neutron_res_fit_param << "A_Corr_pol2_wKC" << "\t\t\t\t" << A_Corr_pol2_wKC << "\n";
        Neutron_res_fit_param << "A_Corr_pol2_wKC_Error" << "\t\t" << A_Corr_pol2_wKC_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol2_wKC" << "\t\t\t\t" << B_Corr_pol2_wKC << "\n";
        Neutron_res_fit_param << "B_Corr_pol2_wKC_Error" << "\t\t" << B_Corr_pol2_wKC_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol2_wKC" << "\t\t\t\t" << C_Corr_pol2_wKC << "\n";
        Neutron_res_fit_param << "C_Corr_pol2_wKC_Error" << "\t\t" << C_Corr_pol2_wKC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol2_wKC" << "\t\t" << ChiSquare_Corr_pol2_wKC << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol2_wKC" << "\t\t\t" << NDF_Corr_pol2_wKC << "\n\n";

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

        Neutron_res_fit_param << "A_Corr_pol3_wKC" << "\t\t\t\t" << A_Corr_pol3_wKC << "\n";
        Neutron_res_fit_param << "A_Corr_pol3_wKC_Error" << "\t\t" << A_Corr_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "B_Corr_pol3_wKC" << "\t\t\t\t" << B_Corr_pol3_wKC << "\n";
        Neutron_res_fit_param << "B_Corr_pol3_wKC_Error" << "\t\t" << B_Corr_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "C_Corr_pol3_wKC" << "\t\t\t\t" << C_Corr_pol3_wKC << "\n";
        Neutron_res_fit_param << "C_Corr_pol3_wKC_Error" << "\t\t" << C_Corr_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "D_Corr_pol3_wKC" << "\t\t\t\t" << D_Corr_pol3_wKC << "\n";
        Neutron_res_fit_param << "D_Corr_pol3_wKC_Error" << "\t\t" << D_Corr_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Corr_pol3_wKC" << "\t\t" << ChiSquare_Corr_pol3_wKC << "\n";
        Neutron_res_fit_param << "NDF_Corr_pol3_wKC" << "\t\t\t" << NDF_Corr_pol3_wKC << "\n\n";

        Neutron_res_fit_param << "A_Std_pol1" << "\t\t\t\t\t" << A_Std_pol1 << "\n";
        Neutron_res_fit_param << "A_Std_pol1_Error" << "\t\t\t" << A_Std_pol1_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol1" << "\t\t\t\t\t" << B_Std_pol1 << "\n";
        Neutron_res_fit_param << "B_Std_pol1_Error" << "\t\t\t" << B_Std_pol1_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol1" << "\t\t\t" << ChiSquare_Std_pol1 << "\n";
        Neutron_res_fit_param << "NDF_Std_pol1" << "\t\t\t\t" << NDF_Std_pol1 << "\n\n";

        Neutron_res_fit_param << "A_Std_pol1_wKC" << "\t\t\t\t" << A_Std_pol1_wKC << "\n";
        Neutron_res_fit_param << "A_Std_pol1_wKC_Error" << "\t\t" << A_Std_pol1_wKC_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol1_wKC" << "\t\t\t\t" << B_Std_pol1_wKC << "\n";
        Neutron_res_fit_param << "B_Std_pol1_wKC_Error" << "\t\t" << B_Std_pol1_wKC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol1_wKC" << "\t\t" << ChiSquare_Std_pol1_wKC << "\n";
        Neutron_res_fit_param << "NDF_Std_pol1_wKC" << "\t\t\t" << NDF_Std_pol1_wKC << "\n\n";

        Neutron_res_fit_param << "A_Std_pol2" << "\t\t\t\t\t" << A_Std_pol2 << "\n";
        Neutron_res_fit_param << "A_Std_pol2_Error" << "\t\t\t" << A_Std_pol2_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol2" << "\t\t\t\t\t" << B_Std_pol2 << "\n";
        Neutron_res_fit_param << "B_Std_pol2_Error" << "\t\t\t" << B_Std_pol2_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol2" << "\t\t\t\t\t" << C_Std_pol2 << "\n";
        Neutron_res_fit_param << "C_Std_pol2_Error" << "\t\t\t" << C_Std_pol2_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol2" << "\t\t\t" << ChiSquare_Std_pol2 << "\n";
        Neutron_res_fit_param << "NDF_Std_pol2" << "\t\t\t\t" << NDF_Std_pol2 << "\n\n";

        Neutron_res_fit_param << "A_Std_pol2_wKC" << "\t\t\t\t" << A_Std_pol2_wKC << "\n";
        Neutron_res_fit_param << "A_Std_pol2_wKC_Error" << "\t\t" << A_Std_pol2_wKC_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol2_wKC" << "\t\t\t\t" << B_Std_pol2_wKC << "\n";
        Neutron_res_fit_param << "B_Std_pol2_wKC_Error" << "\t\t" << B_Std_pol2_wKC_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol2_wKC" << "\t\t\t\t" << C_Std_pol2_wKC << "\n";
        Neutron_res_fit_param << "C_Std_pol2_wKC_Error" << "\t\t" << C_Std_pol2_wKC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol2_wKC" << "\t\t" << ChiSquare_Std_pol2_wKC << "\n";
        Neutron_res_fit_param << "NDF_Std_pol2_wKC" << "\t\t\t" << NDF_Std_pol2_wKC << "\n\n";

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

        Neutron_res_fit_param << "A_Std_pol3_wKC" << "\t\t\t\t" << A_Std_pol3_wKC << "\n";
        Neutron_res_fit_param << "A_Std_pol3_wKC_Error" << "\t\t" << A_Std_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "B_Std_pol3_wKC" << "\t\t\t\t" << B_Std_pol3_wKC << "\n";
        Neutron_res_fit_param << "B_Std_pol3_wKC_Error" << "\t\t" << B_Std_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "C_Std_pol3_wKC" << "\t\t\t\t" << C_Std_pol3_wKC << "\n";
        Neutron_res_fit_param << "C_Std_pol3_wKC_Error" << "\t\t" << C_Std_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "D_Std_pol3_wKC" << "\t\t\t\t" << D_Std_pol3_wKC << "\n";
        Neutron_res_fit_param << "D_Std_pol3_wKC_Error" << "\t\t" << D_Std_pol3_wKC_Error << "\n";
        Neutron_res_fit_param << "ChiSquare_Std_pol3_wKC" << "\t\t" << ChiSquare_Std_pol3_wKC << "\n";
        Neutron_res_fit_param << "NDF_Std_pol3_wKC" << "\t\t\t" << NDF_Std_pol3_wKC << "\n\n";
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
void NeutronResolution::LogHistDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                                          const string &Nucleon_Cuts_Status, const string &FD_photons_Status, const string &Efficiency_Status) {
    ofstream Neutron_res_Hist_param;
    std::string Neutron_res_Hist_paramFilePath;

    if (momResS2CalcMode) {
        if (!momResTestMode) {
            Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
        } else {
            Neutron_res_Hist_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
        }
    } else {
        if (momResS2RunMode) {
            if (!momResTestMode) {
                Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_Hist_paramFilePath = plots_path + "/" + MomResParticle + "_momResS2_hist_param_-_" + SampleName + ".par";
            }
        } else {
            if (!momResTestMode) {
                Neutron_res_Hist_paramFilePath = NeutronResolutionDirectory + MomResParticle + "_momResS1_hist_param_-_" + SampleName + ".par";
            } else {
                Neutron_res_Hist_paramFilePath = plots_path + "/" + MomResParticle + "_momResS1_hist_param_-_" + SampleName + ".par";
            }
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
    Neutron_res_Hist_param << "# " + MomResParticle + " resolution parameters are for:\n";
    Neutron_res_Hist_param << "#sample:\t" << SampleName << "\n";
    Neutron_res_Hist_param << "#Analysis_run_mode:\t" << Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status << "\n";
    Neutron_res_Hist_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_Hist_param << "#delta:\t\t" << delta << "\n";
    //</editor-fold>

    Neutron_res_Hist_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:HistMean:HistSigma\n\n"; //TODO: confirm order!

    for (DSCuts ResSlice: ResTLMomSlicesHistVar) {
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

                    //<editor-fold desc="Safety checks for loading correction variables">
                    if (Loaded_Corr_coefficients_path == "") {
                        cout << "\n\nNeutronResolution::ReadResDataParam: Loaded_Corr_coefficients_path is empty!\n";
                        cout << "The file:\n" << filename << "\n";
                        cout << "is not found! Exiting...\n\n", exit(0);
                    }
                    //</editor-fold>

                    if (findSubstring(parameter, "pol1") && findSubstring(CorrMode, "pol1")) {
                        if (findSubstring(parameter, "pol1_wKC") && findSubstring(CorrMode, "pol1_wKC")) {
                            if (parameter == "A_Corr_pol1_wKC") {
                                Loaded_A_Corr_pol1_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol1_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol1_wKC");
                            } else if (parameter == "A_Corr_pol1_wKC_Error") {
                                Loaded_A_Corr_pol1_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol1_wKC") {
                                Loaded_B_Corr_pol1_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol1_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol1_wKC");
                            } else if (parameter == "B_Corr_pol1_wKC_Error") {
                                Loaded_B_Corr_pol1_wKC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol1_wKC") {
                                Loaded_ChiSquare_Corr_pol1_wKC = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol1_wKC") {
                                Loaded_NDF_Corr_pol1_wKC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol1") && !findSubstring(CorrMode, "pol1_wKC")) {
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
                    } else if (findSubstring(parameter, "pol2") && findSubstring(CorrMode, "pol2")) {
                        if (findSubstring(parameter, "pol2_wKC") && findSubstring(CorrMode, "pol2_wKC")) {
                            if (parameter == "A_Corr_pol2_wKC") {
                                Loaded_A_Corr_pol2_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol2_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol2_wKC");
                            } else if (parameter == "A_Corr_pol2_wKC_Error") {
                                Loaded_A_Corr_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol2_wKC") {
                                Loaded_B_Corr_pol2_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol2_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol2_wKC");
                            } else if (parameter == "B_Corr_pol2_wKC_Error") {
                                Loaded_B_Corr_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol2_wKC") {
                                Loaded_C_Corr_pol2_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol2_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_C_Corr_pol2_wKC");
                            } else if (parameter == "C_Corr_pol2_wKC_Error") {
                                Loaded_C_Corr_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol2_wKC") {
                                Loaded_ChiSquare_Corr_pol2_wKC = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol2_wKC") {
                                Loaded_NDF_Corr_pol2_wKC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol2") && !findSubstring(CorrMode, "pol2_wKC")) {
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
                    } else if (findSubstring(parameter, "pol3") && findSubstring(CorrMode, "pol3")) {
                        if (findSubstring(parameter, "pol3_wKC") && findSubstring(CorrMode, "pol3_wKC")) {
                            if (parameter == "A_Corr_pol3_wKC") {
                                Loaded_A_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_A_Corr_pol3_wKC");
                            } else if (parameter == "A_Corr_pol3_wKC_Error") {
                                Loaded_A_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol3_wKC") {
                                Loaded_B_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_B_Corr_pol3_wKC");
                            } else if (parameter == "B_Corr_pol3_wKC_Error") {
                                Loaded_B_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol3_wKC") {
                                Loaded_C_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_C_Corr_pol3_wKC");
                            } else if (parameter == "C_Corr_pol3_wKC_Error") {
                                Loaded_C_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "D_Corr_pol3_wKC") {
                                Loaded_D_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_D_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back("Loaded_D_Corr_pol3_wKC");
                            } else if (parameter == "D_Corr_pol3_wKC_Error") {
                                Loaded_D_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol3_wKC") {
                                Loaded_ChiSquare_Corr_pol3_wKC = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol3_wKC") {
                                Loaded_NDF_Corr_pol3_wKC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol3") && !findSubstring(CorrMode, "pol3_wKC")) {
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

                    //<editor-fold desc="Safety checks for loading smearing variables">
                    if (Load_smearing && Loaded_Std_coefficients_path == "") {
                        cout << "\n\nNeutronResolution::ReadResDataParam: Loaded_Std_coefficients_path is empty!\n";
                        cout << "The file:\n" << filename << "\n";
                        cout << "is not found! Exiting...\n\n", exit(0);
                    }
                    //</editor-fold>

                    if (findSubstring(parameter, "pol1") && findSubstring(SmearMode, "pol1")) {
                        if (findSubstring(parameter, "pol1_wKC") && findSubstring(SmearMode, "pol1_wKC")) {
                            if (parameter == "A_Std_pol1_wKC") {
                                Loaded_A_Std_pol1_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol1_wKC), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol1_wKC");
                            } else if (parameter == "A_Std_pol1_wKC_Error") {
                                Loaded_A_Std_pol1_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol1_wKC") {
                                Loaded_B_Std_pol1_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol1_wKC), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol1_wKC");
                            } else if (parameter == "B_Std_pol1_wKC_Error") {
                                Loaded_B_Std_pol1_wKC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol1_wKC") {
                                Loaded_ChiSquare_Std_pol1_wKC = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol1_wKC") {
                                Loaded_NDF_Std_pol1_wKC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol1") && !findSubstring(SmearMode, "pol1_wKC")) {
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
                        if (findSubstring(parameter, "pol2_wKC") && findSubstring(SmearMode, "pol2_wKC")) {
                            if (parameter == "A_Std_pol2_wKC") {
                                Loaded_A_Std_pol2_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol2_wKC), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol2_wKC");
                            } else if (parameter == "A_Std_pol2_wKC_Error") {
                                Loaded_A_Std_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol2_wKC") {
                                Loaded_B_Std_pol2_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol2_wKC), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol2_wKC");
                            } else if (parameter == "B_Std_pol2_wKC_Error") {
                                Loaded_B_Std_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol2_wKC") {
                                Loaded_C_Std_pol2_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_C_Std_pol2_wKC), Loaded_Std_coefficients_names.push_back("Loaded_C_Std_pol2_wKC");
                            } else if (parameter == "C_Std_pol2_wKC_Error") {
                                Loaded_C_Std_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol2_wKC") {
                                Loaded_ChiSquare_Std_pol2_wKC = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol2_wKC") {
                                Loaded_NDF_Std_pol2_wKC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol2") && !findSubstring(SmearMode, "pol2_wKC")) {
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
                        if (findSubstring(parameter, "pol3_wKC") && findSubstring(SmearMode, "pol3_wKC")) {
                            if (parameter == "A_Std_pol3_wKC") {
                                Loaded_A_Std_pol3_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_A_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back("Loaded_A_Std_pol3_wKC");
                            } else if (parameter == "A_Std_pol3_wKC_Error") {
                                Loaded_A_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol3_wKC") {
                                Loaded_B_Std_pol3_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_B_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back("Loaded_B_Std_pol3_wKC");
                            } else if (parameter == "B_Std_pol3_wKC_Error") {
                                Loaded_B_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol3_wKC") {
                                Loaded_C_Std_pol3_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_C_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back("Loaded_C_Std_pol3_wKC");
                            } else if (parameter == "C_Std_pol3_wKC_Error") {
                                Loaded_C_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "D_Std_pol3_wKC") {
                                Loaded_D_Std_pol3_wKC = stod(parameter2);
                                Loaded_Std_coefficients_values.push_back(Loaded_D_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back("Loaded_D_Std_pol3_wKC");
                            } else if (parameter == "D_Std_pol3_wKC_Error") {
                                Loaded_D_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Std_pol3_wKC") {
                                Loaded_ChiSquare_Std_pol3_wKC = stod(parameter2);
                            } else if (parameter == "NDF_Std_pol3_wKC") {
                                Loaded_NDF_Std_pol3_wKC = stod(parameter2);
                            }
                        } else if (findSubstring(parameter, "pol3") && !findSubstring(SmearMode, "pol3_wKC")) {
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

                //<editor-fold desc="Safety checks">
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

double NeutronResolution::PSmear(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
    bool Printout = false;
    bool Printout_Std_Variables = false;

    if (!apply_nucleon_SmearAndCorr) {
        /* Smearing and correction are disabled */
        return Momentum;
    } else {
        /* Smearing and correction are enabled */

        /* Setting momentum powers */
        double SmearedMomentum, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;

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
            //</editor-fold>

        } else if ((SmearMode == "pol1") || (SmearMode == "pol2") || (SmearMode == "pol3") ||
                   (SmearMode == "pol1_wKC") || (SmearMode == "pol2_wKC") || (SmearMode == "pol3_wKC")) {
            /* Smear using pol fit results */
            double Smearing, Arg;

            if (SmearMode == "pol1") {
                Arg = Loaded_A_Std_pol1 * Momentum + Loaded_B_Std_pol1;

                if (Printout_Std_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Std_pol1 = " << Loaded_A_Std_pol1 << "\n";
                    cout << "Loaded_B_Std_pol1 = " << Loaded_B_Std_pol1 << "\n\n";
                }
            } else if (SmearMode == "pol1_wKC") {
                Arg = Loaded_A_Std_pol1_wKC * Momentum + Loaded_B_Std_pol1_wKC;

                if (Printout_Std_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Std_pol1_wKC = " << Loaded_A_Std_pol1_wKC << "\n";
                    cout << "Loaded_B_Std_pol1_wKC = " << Loaded_B_Std_pol1_wKC << "\n\n";
                }
            } else if (SmearMode == "pol2") {
                Arg = Loaded_A_Std_pol2 * Momentum2 + Loaded_B_Std_pol2 * Momentum + Loaded_C_Std_pol2;

                if (Printout_Std_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Std_pol2 = " << Loaded_A_Std_pol2 << "\n";
                    cout << "Loaded_B_Std_pol2 = " << Loaded_B_Std_pol2 << "\n";
                    cout << "Loaded_C_Std_pol2 = " << Loaded_C_Std_pol2 << "\n\n";
                }
            } else if (SmearMode == "pol2_wKC") {
                Arg = Loaded_A_Std_pol2_wKC * Momentum2 + Loaded_B_Std_pol2_wKC * Momentum + Loaded_C_Std_pol2_wKC;

                if (Printout_Std_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Std_pol2_wKC = " << Loaded_A_Std_pol2_wKC << "\n";
                    cout << "Loaded_B_Std_pol2_wKC = " << Loaded_B_Std_pol2_wKC << "\n";
                    cout << "Loaded_C_Std_pol2_wKC = " << Loaded_C_Std_pol2_wKC << "\n\n";
                }
            } else if (SmearMode == "pol3") {
                Arg = Loaded_A_Std_pol3 * Momentum3 + Loaded_B_Std_pol3 * Momentum2 + Loaded_C_Std_pol3 * Momentum + Loaded_D_Std_pol3;

                if (Printout_Std_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Std_pol3 = " << Loaded_A_Std_pol3 << "\n";
                    cout << "Loaded_B_Std_pol3 = " << Loaded_B_Std_pol3 << "\n";
                    cout << "Loaded_C_Std_pol3 = " << Loaded_C_Std_pol3 << "\n";
                    cout << "Loaded_D_Std_pol3 = " << Loaded_D_Std_pol3 << "\n\n";
                }
            } else if (SmearMode == "pol3_wKC") {
                Arg = Loaded_A_Std_pol3_wKC * Momentum3 + Loaded_B_Std_pol3_wKC * Momentum2 + Loaded_C_Std_pol3_wKC * Momentum + Loaded_D_Std_pol3_wKC;

                if (Printout_Std_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Std_pol3_wKC = " << Loaded_A_Std_pol3_wKC << "\n";
                    cout << "Loaded_B_Std_pol3_wKC = " << Loaded_B_Std_pol3_wKC << "\n";
                    cout << "Loaded_C_Std_pol3_wKC = " << Loaded_C_Std_pol3_wKC << "\n";
                    cout << "Loaded_D_Std_pol3_wKC = " << Loaded_D_Std_pol3_wKC << "\n\n";
                }
            }

            Smearing = Rand->Gaus(1, Arg);
            SmearedMomentum = Smearing * Momentum; // minus for protons and plus for protons

            if (Printout) {
                cout << "\n\nSmearMode = " << SmearMode << "\n";
                cout << "Arg = " << Arg << "\n";
                cout << "Smearing = " << Smearing << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "SmearedMomentum = " << SmearedMomentum << "\n\n";
            }

        } else {
            cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
        }

        return SmearedMomentum;
    }
}
//</editor-fold>

// NCorr function -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NCorr function">
/* A function to correction (calibrate) neutron momentum by fitted neutron correction */

double NeutronResolution::NCorr(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
    bool Printout = false;
    bool Printout_Corr_Variables = false;

    if (!apply_nucleon_SmearAndCorr) {
        /* Smearing and correction are disabled */
        return Momentum;
    } else {
        /* Smearing and correction are enabled */

        /* Setting corrected momentum & momentum powers */
        double CorrectedMomentum, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;

        if (CorrMode == "slices") {
            /* Correction using slice fit results */

            //<editor-fold desc="Original (correction from loaded nRes fit variables)">
            for (DSCuts Loaded_res_slice: Loaded_Res_Slices_HistVar) {
                if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
                    CorrectedMomentum = Momentum * (1 + Loaded_res_slice.GetMean()); // minus for protons and plus for neutrons

                    if (Printout) {
                        cout << "\n\nLoaded_res_slice.GetMean() = " << Loaded_res_slice.GetMean() << "\n";
                        cout << "Momentum = " << Momentum << "\n";
                        cout << "Correction = " << Loaded_res_slice.GetMean() << "\n";
                        cout << "CorrectedMomentum = " << CorrectedMomentum << "\n\n";
                    }
                }
            }
            //</editor-fold>

        } else if ((CorrMode == "pol1") || (CorrMode == "pol2") || (CorrMode == "pol3") ||
                   (CorrMode == "pol1_wKC") || (CorrMode == "pol2_wKC") || (CorrMode == "pol3_wKC")) {
            /* Correction using pol fit results */
            double Mu, CorrectionFactor;

            if (CorrMode == "pol1") {
                Mu = Loaded_A_Corr_pol1 * Momentum + Loaded_B_Corr_pol1;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol1 = " << Loaded_A_Corr_pol1 << "\n";
                    cout << "Loaded_B_Corr_pol1 = " << Loaded_B_Corr_pol1 << "\n\n";
                }
            } else if (CorrMode == "pol1_wKC") {
                Mu = Loaded_A_Corr_pol1_wKC * Momentum + Loaded_B_Corr_pol1_wKC;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol1_wKC = " << Loaded_A_Corr_pol1_wKC << "\n";
                    cout << "Loaded_B_Corr_pol1_wKC = " << Loaded_B_Corr_pol1_wKC << "\n\n";
                }
            } else if (CorrMode == "pol2") {
                Mu = Loaded_A_Corr_pol2 * Momentum2 + Loaded_B_Corr_pol2 * Momentum + Loaded_C_Corr_pol2;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol2 = " << Loaded_A_Corr_pol2 << "\n";
                    cout << "Loaded_B_Corr_pol2 = " << Loaded_B_Corr_pol2 << "\n";
                    cout << "Loaded_C_Corr_pol2 = " << Loaded_C_Corr_pol2 << "\n\n";
                }
            } else if (CorrMode == "pol2_wKC") {
                Mu = Loaded_A_Corr_pol2_wKC * Momentum2 + Loaded_B_Corr_pol2_wKC * Momentum + Loaded_C_Corr_pol2_wKC;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol2_wKC = " << Loaded_A_Corr_pol2_wKC << "\n";
                    cout << "Loaded_B_Corr_pol2_wKC = " << Loaded_B_Corr_pol2_wKC << "\n";
                    cout << "Loaded_C_Corr_pol2_wKC = " << Loaded_C_Corr_pol2_wKC << "\n\n";
                }
            } else if (CorrMode == "pol3") {
                Mu = Loaded_A_Corr_pol3 * Momentum3 + Loaded_B_Corr_pol3 * Momentum2 + Loaded_C_Corr_pol3 * Momentum + Loaded_D_Corr_pol3;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol3 = " << Loaded_A_Corr_pol3 << "\n";
                    cout << "Loaded_B_Corr_pol3 = " << Loaded_B_Corr_pol3 << "\n";
                    cout << "Loaded_C_Corr_pol3 = " << Loaded_C_Corr_pol3 << "\n";
                    cout << "Loaded_D_Corr_pol3 = " << Loaded_D_Corr_pol3 << "\n\n";
                }
            } else if (CorrMode == "pol3_wKC") {
                Mu = Loaded_A_Corr_pol3_wKC * Momentum3 + Loaded_B_Corr_pol3_wKC * Momentum2 + Loaded_C_Corr_pol3_wKC * Momentum + Loaded_D_Corr_pol3_wKC;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol3_wKC = " << Loaded_A_Corr_pol3_wKC << "\n";
                    cout << "Loaded_B_Corr_pol3_wKC = " << Loaded_B_Corr_pol3_wKC << "\n";
                    cout << "Loaded_C_Corr_pol3_wKC = " << Loaded_C_Corr_pol3_wKC << "\n";
                    cout << "Loaded_D_Corr_pol3_wKC = " << Loaded_D_Corr_pol3_wKC << "\n\n";
                }
            }

            CorrectionFactor = 1 / (1 - Mu);
            CorrectedMomentum = Momentum * CorrectionFactor;

            if (Printout) {
                cout << "\n\nCorrMode = " << CorrMode << "\n";
                cout << "Mu = " << Mu << "\n";
                cout << "CorrectionFactor = " << CorrectionFactor << "\n";
                cout << "Momentum = " << Momentum << "\n";
                cout << "CorrectedMomentum = " << CorrectedMomentum << "\n\n";
            }
        } else {
            cout << "\n\nNeutronResolution::NCorr: CorrMode illegal! Exiting...", exit(0);
        }

        return CorrectedMomentum;
    }
}
//</editor-fold>

// GetMomResMu function -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetMomResMu function">
/* A function to correction (calibrate) neutron momentum by fitted neutron correction */

double NeutronResolution::GetMomResMu(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
    bool Printout = false;
    bool Printout_Corr_Variables = false;

    if (!apply_nucleon_SmearAndCorr) {
        /* Smearing and correction are disabled */
        return 0;
    } else {
        /* Smearing and correction are enabled */

        /* Setting corrected momentum & momentum powers */
        double Mu, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;

        if ((CorrMode == "pol1") || (CorrMode == "pol2") || (CorrMode == "pol3") ||
            (CorrMode == "pol1_wKC") || (CorrMode == "pol2_wKC") || (CorrMode == "pol3_wKC")) {
            /* Correction using pol fit results */
            if (CorrMode == "pol1") {
                Mu = Loaded_A_Corr_pol1 * Momentum + Loaded_B_Corr_pol1;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol1 = " << Loaded_A_Corr_pol1 << "\n";
                    cout << "Loaded_B_Corr_pol1 = " << Loaded_B_Corr_pol1 << "\n\n";
                }
            } else if (CorrMode == "pol1_wKC") {
                Mu = Loaded_A_Corr_pol1_wKC * Momentum + Loaded_B_Corr_pol1_wKC;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol1_wKC = " << Loaded_A_Corr_pol1_wKC << "\n";
                    cout << "Loaded_B_Corr_pol1_wKC = " << Loaded_B_Corr_pol1_wKC << "\n\n";
                }
            } else if (CorrMode == "pol2") {
                Mu = Loaded_A_Corr_pol2 * Momentum2 + Loaded_B_Corr_pol2 * Momentum + Loaded_C_Corr_pol2;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol2 = " << Loaded_A_Corr_pol2 << "\n";
                    cout << "Loaded_B_Corr_pol2 = " << Loaded_B_Corr_pol2 << "\n";
                    cout << "Loaded_C_Corr_pol2 = " << Loaded_C_Corr_pol2 << "\n\n";
                }
            } else if (CorrMode == "pol2_wKC") {
                Mu = Loaded_A_Corr_pol2_wKC * Momentum2 + Loaded_B_Corr_pol2_wKC * Momentum + Loaded_C_Corr_pol2_wKC;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol2_wKC = " << Loaded_A_Corr_pol2_wKC << "\n";
                    cout << "Loaded_B_Corr_pol2_wKC = " << Loaded_B_Corr_pol2_wKC << "\n";
                    cout << "Loaded_C_Corr_pol2_wKC = " << Loaded_C_Corr_pol2_wKC << "\n\n";
                }
            } else if (CorrMode == "pol3") {
                Mu = Loaded_A_Corr_pol3 * Momentum3 + Loaded_B_Corr_pol3 * Momentum2 + Loaded_C_Corr_pol3 * Momentum + Loaded_D_Corr_pol3;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol3 = " << Loaded_A_Corr_pol3 << "\n";
                    cout << "Loaded_B_Corr_pol3 = " << Loaded_B_Corr_pol3 << "\n";
                    cout << "Loaded_C_Corr_pol3 = " << Loaded_C_Corr_pol3 << "\n";
                    cout << "Loaded_D_Corr_pol3 = " << Loaded_D_Corr_pol3 << "\n\n";
                }
            } else if (CorrMode == "pol3_wKC") {
                Mu = Loaded_A_Corr_pol3_wKC * Momentum3 + Loaded_B_Corr_pol3_wKC * Momentum2 + Loaded_C_Corr_pol3_wKC * Momentum + Loaded_D_Corr_pol3_wKC;

                if (Printout_Corr_Variables) {
                    cout << "\n";
                    cout << "\n\nLoaded_A_Corr_pol3_wKC = " << Loaded_A_Corr_pol3_wKC << "\n";
                    cout << "Loaded_B_Corr_pol3_wKC = " << Loaded_B_Corr_pol3_wKC << "\n";
                    cout << "Loaded_C_Corr_pol3_wKC = " << Loaded_C_Corr_pol3_wKC << "\n";
                    cout << "Loaded_D_Corr_pol3_wKC = " << Loaded_D_Corr_pol3_wKC << "\n\n";
                }
            }

            if (Printout) {
                cout << "\n\nCorrMode = " << CorrMode << "\n";
                cout << "Mu = " << Mu << "\n";
                cout << "Momentum = " << Momentum << "\n";
            }
        } else {
            cout << "\n\nNeutronResolution::NCorr: CorrMode illegal! Exiting...", exit(0);
        }

        return Mu;
    }
}
//</editor-fold>
