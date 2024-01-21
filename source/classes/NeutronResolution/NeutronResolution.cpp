//
// Created by alons on 20/06/2023.
//

#include "NeutronResolution.h"

// NeutronResolution constructor -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NeutronResolution constructor">
/* A constructor that sets the particle (neutron or proton) */
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

// MomResInit function ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MomResInit function">
void NeutronResolution::MomResInit(const bool &plot_and_fit_MomRes, const bool &Calculate_momResS2, const bool &Run_with_momResS2,
                                   const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                                   const DSCuts &FD_nucleon_momentum_cut, const double &ParticleMomTh, const string &NeutronResolutionDirectory,
                                   const string &SavePath, const double &DeltaSlices, const bool &VaryingDelta, const string &SmearMode,
                                   const string &CorrMode, const bool &momRes_test, const bool &ForceSmallpResLimits) {
    if (isNeutron) {
        if (plot_and_fit_MomRes) {
            SetMomResCalculations(SampleName, NucleonCutsDirectory, beamE, FD_nucleon_momentum_cut, ParticleMomTh, Calculate_momResS2,
                                  Run_with_momResS2, NeutronResolutionDirectory, SavePath, DeltaSlices, VaryingDelta, SmearMode, CorrMode,
                                  momRes_test);

            if (momRes_test) {
                if (Calculate_momResS2) { // if Calculate_momResS2=true => load everything from momResS1 files
                    /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                    ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
                                      SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                     "reco", true, false);

                    /* Load proton smearing fit parameters */
                    //TODO: figure out if I really need to load these at this stage
                    ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
                                      SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                     "truth", false, true);

                    //                    //<editor-fold desc="Original (load from TL only!)">
//                    /* Load neutron correction fit parameters */
//                    ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
//                                      SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                     "truth", true, false);
//
//                    /* Load proton smearing fit parameters */
//                    ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
//                                      SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                     "truth", false, true);
//                    //</editor-fold>

                } else { // if Calculate_momResS2=false => load everything from either momResS1 or momResS2
                    if (Run_with_momResS2) { // if Calculate_momResS2=false && Run_with_momResS2=true => load correction from momResS1 and
                        // smearing from momResS2
                        /* Load neutron correction fit parameters */
                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                         "reco", true, false);

                        /* Load proton smearing fit parameters */
                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" +
                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                         "truth", false, true);

                        //                        //<editor-fold desc="Original (load from TL only!)">
                        //                        /* Load neutron correction fit parameters */
                        //                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
                        //                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                        //                                         "truth", true, false);
                        //
                        //                        /* Load proton smearing fit parameters */
                        //                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" +
                        //                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                        //                                         "truth", false, true);
                        //                        //</editor-fold>

                    } else { // if Calculate_momResS2=false and Run_with_momResS2=false => load both correction and smearing from momResS1
                        /* Load neutron correction fit parameters (correction factor from momResS1 reco fits!) */
                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                         "reco", true, false);

                        /* Load proton smearing fit parameters */
                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                         "truth", false, true);

                        //                        //<editor-fold desc="Original (load from TL only!)">
//                        /* Load neutron correction fit parameters */
//                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
//                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                         "truth", true, false);
//
//                        /* Load proton smearing fit parameters */
//                        ReadResDataParam((NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" +
//                                          SampleName + ".par").c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                         "truth", false, true);
//                        //</editor-fold>

                    }
                }
            }
        } else { // if plot_and_fit_MomRes=false => Calculate_momResS2=false !!!
            SetSmearAndCorrModes(SmearMode, CorrMode);

            if (Run_with_momResS2) { // if Run_with_momResS2=true => load correction from momResS1 and smearing from momResS2
                string NeutronCorrectionDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName +
                                                   "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
                string ProtonSmearingDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" +
                                                SampleName + ".par";

                /* Load neutron correction fit parameters */
                ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                 "reco", true, false);

                /* Load proton smearing fit parameters */
                ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                 "truth", false, true);

                //                        //<editor-fold desc="Original (load from TL only!)">
//                /* Load neutron correction fit parameters */
//                ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                 "truth", true, false);
//
//                /* Load proton smearing fit parameters */
//                ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                 "truth", false, true);
//                        //</editor-fold>

                //<editor-fold desc="Safety checks for data files">
                if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                    cout << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! "
                            "Exiting...\n\n", exit(0);
                }

                if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
                    cout << "\n\nNeutronResolution::NeutronResolution: proton smearing variables are not being loaded from neutron data! "
                            "Exiting...\n\n", exit(0);
                }
                //</editor-fold>

            } else { // if Calculate_momResS2 = false and Run_with_momResS2 = false => load both correction and smearing from momResS1
                string NeutronCorrectionDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName +
                                                   "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
                string ProtonSmearingDataFile = NeutronResolutionDirectory + "Res_data_-_" + SampleName +
                                                "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";

                /* Load neutron correction fit parameters */
                ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                 "reco", true, false);

                /* Load proton smearing fit parameters */
                ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
                                 "truth", false, true);

                //                        //<editor-fold desc="Original (load from TL only!)">
//                /* Load neutron correction fit parameters */
//                ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                 "truth", true, false);
//
//                /* Load proton smearing fit parameters */
//                ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory,
//                                 "truth", false, true);
//                        //</editor-fold>

                //<editor-fold desc="Safety checks for data files">
                if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                    cout
                            << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(
                            0);
                }

                if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
                    cout
                            << "\n\nNeutronResolution::NeutronResolution: proton smearing variables are not being loaded from neutron data! Exiting...\n\n", exit(
                            0);
                }
                //</editor-fold>

            }
        }
    } else if (isProton) {
        SetMomResCalculations(SampleName, NucleonCutsDirectory, beamE, FD_nucleon_momentum_cut, ParticleMomTh, Calculate_momResS2,
                              Run_with_momResS2, NeutronResolutionDirectory, SavePath, DeltaSlices, VaryingDelta, SmearMode, CorrMode, momRes_test,
                              ForceSmallpResLimits);
    }
}
//</editor-fold>

//<editor-fold desc="SetMomResCalculations function">
void NeutronResolution::SetMomResCalculations(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                                              const DSCuts &FD_nucleon_momentum_cut, const double &ParticleMomTh, bool const &Calculate_momResS2,
                                              bool const &Run_in_momResS2, const string &NeutronResolutionDirectory, const string &SavePath,
                                              const double &DeltaSlices, const bool &VaryingDelta, const string &SmearM,
                                              const string &CorrM, const bool momRes_test, const bool ForceSmallpResLimits) {
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

    SetMomResSlicesByType(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, "truth", SavePath, VaryingDelta, momRes_test,
                          ForceSmallpResLimits);
    SetMomResSlicesByType(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, "reco", SavePath, VaryingDelta, momRes_test,
                          ForceSmallpResLimits);
    LoadFitParam(SampleName, NucleonCutsDirectory, Calculate_momResS2, NeutronResolutionDirectory);
}
//</editor-fold>

//<editor-fold desc="SetMomResSlicesByType function">
void NeutronResolution::SetMomResSlicesByType(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                                              const double &ParticleMomTh, const string &MomentumType, const string &SavePath,
                                              const bool &VaryingDelta, const bool &momRes_test, const bool &ForceSmallpResLimits) {
    if (MomentumType == "truth") {
        SetMomResSlices(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, MomentumType, SavePath, VaryingDelta, momRes_test,
                        ForceSmallpResLimits, ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar,
                        TL_NumberOfSlices);
    } else if (MomentumType == "reco") {
        SetMomResSlices(SampleName, NucleonCutsDirectory, beamE, ParticleMomTh, MomentumType, SavePath, VaryingDelta, momRes_test,
                        ForceSmallpResLimits, ResRecoMomSlices, ResRecoMomSlicesLimits, ResRecoMomSlicesFitVar, ResRecoMomSlicesHistVar,
                        Reco_NumberOfSlices);
    }
}
//</editor-fold>

//<editor-fold desc="SetMomResSlices function">
void NeutronResolution::SetMomResSlices(const string &SampleName, const string &NucleonCutsDirectory, const double &beamE,
                                        const double &ParticleMomTh, const string &MomentumType, const string &SavePath, const bool &VaryingDelta,
                                        const bool &momRes_test, const bool &ForceSmallpResLimits, vector <hPlot1D> &ResSlices0,
                                        vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0,
                                        vector <DSCuts> &ResSlicesHistVar0, int &NumberOfSlices0) {
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
            hStatsTitle = "n res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{nFD}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Neutron resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{nFD}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName =
                    to_string(SliceNumber) + "_res_plot_for_" + MomentumType + "_P_n_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                    to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            hResolutionSlice = hPlot1D("1n", "FD", hStatsTitle, hTitle, "R_{nFD} = (P^{truth}_{nFD} - P^{reco}_{nFD})/P^{truth}_{nFD}",
                                       SlicesSavePath, hSaveName,
                                       hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            ResSliceFitCuts = DSCuts(("fit_" + MomentumType + "_" + hCutName), "FD", "Neutron", "1n", 0, -9999, 9999);
        } else if (isProton) {
            hStatsTitle = "p res. - " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{pFD}#leq" +
                          to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hTitle = "Proton resolution for " + to_string_with_precision(SliceLowerLim, 2) + "#leqP^{" + MomentumType + "}_{pFD}#leq" +
                     to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision) + " [GeV/c]";
            hSaveName =
                    to_string(SliceNumber) + "_res_plot_for_" + MomentumType + "_P_p_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                    to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);
            hCutName = "Slice_#" + to_string(SliceNumber) + "_from_" + to_string_with_precision(SliceLowerLim, 2) + "_to_" +
                       to_string_with_precision(SliceUpperLim, SliceUpperLimPrecision);

            if (!momRes_test || ForceSmallpResLimits) {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                           SlicesSavePath, hSaveName,
                                           -0.2, 0.2, hSliceNumOfBin);
            } else {
                hResolutionSlice = hPlot1D("1p", "FD", hStatsTitle, hTitle, "Resolution = (P^{truth}_{pFD} - P^{reco}_{pFD})/P^{truth}_{pFD}",
                                           SlicesSavePath, hSaveName,
                                           hSliceLowerLim, hSliceUpperLim, hSliceNumOfBin);
            }

            ResSliceFitCuts = DSCuts(("fit_" + MomentumType + "_" + hCutName), "FD", "Proton", "1n", 0, -9999, 9999);
        }

        ResSliceFitCuts.SetSliceUpperb(SliceUpperLim);
        ResSliceFitCuts.SetSliceLowerb(SliceLowerLim);
        ResSliceFitCuts.SetSliceNumber(SliceNumber);

        ResSlices0.push_back(hResolutionSlice);
        ResSlicesLimits0.push_back({SliceLowerLim, SliceUpperLim});
        ResSlicesFitVar0.push_back(ResSliceFitCuts);

        ResSliceFitCuts.SetCutVariable(("hist_" + MomentumType + "_" + hCutName));
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

//<editor-fold desc="SetUpperMomCut function">
void NeutronResolution::SetUpperMomCut(const string &SampleName, const string &NucleonCutsDirectory) {
    ReadInputParam(
            (NucleonCutsDirectory + "Nucleon_Cuts_-_" + SampleName + ".par").c_str()); // load sample-appropreate cuts file from CutsDirectory

    SliceUpperMomLim = Neutron_Momentum_cut;
}
//</editor-fold>

//<editor-fold desc="LoadFitParam function">
void NeutronResolution::LoadFitParam(const string &SampleName, const string &NucleonCutsDirectory, bool const &Calculate_momResS2,
                                     const string &NeutronResolutionDirectory) {
    if (isNeutron) {
        if (momResS2CalcMode && !momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 calculation mode. Loading momResS1 variables...\n";

            string NeutronCorrectionDataFile =
                    NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1), but not smearing variables */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            //<editor-fold desc="Safety checks for data files">
            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                cout
                        << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(
                        0);
            }
            //</editor-fold>

            cout << "\nDone.\n";
        } else if (!momResS2CalcMode && momResS2RunMode) {
            cout << "\n\nNeutronResolution::NeutronResolution: running in momResS2 run mode.\n";
            cout << "Loading correction from momResS1 variables & smearing from momResS2 variables...\n";

            string NeutronCorrectionDataFile =
                    NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS1_fit_param_-_" + SampleName + ".par";
            string ProtonSmearingDataFile =
                    NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_momResS2_fit_param_-_" + SampleName + ".par";

            /* Load neutron correction variables (from momResS1) */
            ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, true, false);

            /* Load proton smearing variables (from momResS2) */
            ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, SampleName, NucleonCutsDirectory, false, true);

            //<editor-fold desc="Safety checks for data files">
            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
                cout
                        << "\n\nNeutronResolution::NeutronResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(
                        0);
            }

            if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
                cout
                        << "\n\nNeutronResolution::NeutronResolution: proton smearing variables are not being loaded from neutron data! Exiting...\n\n", exit(
                        0);
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

// hFillResPlotsByType function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="hFillResPlotsByType function">
void NeutronResolution::hFillResPlotsByType(const double &TL_momentum, const double &Reco_momentum, const double &Resolution,
                                            const double &Weight) {
    hFillResPlots(TL_momentum, Resolution, Weight, ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar,
                  TL_NumberOfSlices);
    hFillResPlots(Reco_momentum, Resolution, Weight, ResRecoMomSlices, ResRecoMomSlicesLimits, ResRecoMomSlicesFitVar, ResRecoMomSlicesHistVar,
                  Reco_NumberOfSlices);
}
//</editor-fold>

//<editor-fold desc="hFillResPlots function">
void NeutronResolution::hFillResPlots(const double &TL_momentum, const double &Resolution, const double &Weight) {
    bool Printout = false;

    for (int i = 0; i < TL_NumberOfSlices; i++) {
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
                                      vector <vector<double>> &ResSlicesLimits0, vector <DSCuts> &ResSlicesFitVar0,
                                      vector <DSCuts> &ResSlicesHistVar0, int &NumberOfSlices0) {
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

// SliceFitDrawAndSaveByType function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="CFitFunction">
/* CFitFunction function for costume fit */
Double_t CFitFunction(Double_t *v, Double_t *par) {
    Double_t arg = 0;
    if (par[2] != 0) { arg = (v[0] - par[1]) / par[2]; } // 3 parameters

    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
    return fitval;
}
//</editor-fold>

//<editor-fold desc="SliceFitDrawAndSaveByType function">
void NeutronResolution::SliceFitDrawAndSaveByType(const string &SampleName, const double &beamE) {
    SliceFitDrawAndSave(SampleName, beamE, "truth", ResTLMomSlices, ResTLMomSlicesLimits, ResTLMomSlicesFitVar, ResTLMomSlicesHistVar,
                        FittedTLMomSlices, TL_NumberOfSlices);
    SliceFitDrawAndSave(SampleName, beamE, "reco", ResRecoMomSlices, ResRecoMomSlicesLimits, ResRecoMomSlicesFitVar, ResRecoMomSlicesHistVar,
                        FittedRecoMomSlices, Reco_NumberOfSlices);
}
//</editor-fold>

//<editor-fold desc="SliceFitDrawAndSave function">
void NeutronResolution::SliceFitDrawAndSave(const string &SampleName, const double &beamE, const string &MomentumType,
                                            vector <hPlot1D> &ResSlices0, vector <vector<double>> &ResSlicesLimits0,
                                            vector <DSCuts> &ResSlicesFitVar0, vector <DSCuts> &ResSlicesHistVar0, vector<int> &FittedSlices0,
                                            int &NumberOfSlices0) {
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

        TH1D *hSlice = (TH1D *) ResSlices0.at(i).GetHistogram();
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
            FitParam->SetBorderSize(1), FitParam->SetFillColor(0);
            FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);
            FitParam->AddText(("Fit amp = " + to_string_with_precision(FitAmp, 4)).c_str());
            FitParam->AddText(("Fit #mu = " + to_string_with_precision(FitMean, 4)).c_str());
            FitParam->AddText(("Fit #sigma = " + to_string_with_precision(FitStd, 4)).c_str());
            FitParam->Draw("same");

            int SliceUpperLimPrecision;
            if (ResSlicesLimits0.at(i).at(1) == beamE) { SliceUpperLimPrecision = 3; } else { SliceUpperLimPrecision = 2; }

            string hSlice_CloneSaveDir = ResSlices0.at(i).GetHistogram1DSaveNamePath() + "00_Fitted_" + MomentumType + "_res_slices/";
            string hSlice_CloneSaveName = hSlice_CloneSaveDir + sNameFlag + "_" + MomentumType + "_" +
                                          ResSlices0.at(i).GetHistogram1DSaveName() + "_fitted.png";
            system(("mkdir -p " + hSlice_CloneSaveDir).c_str());

            auto ListOfFunctions = hSlice->GetListOfFunctions();
            ListOfFunctions->Add((TObject *) FitParam);

            cout << "\n", SliceFitCanvas->SaveAs(hSlice_CloneSaveName.c_str());

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

            SliceFitCanvas->Clear();

            FittedSlices0.push_back(i); // Log slices that were fitted
        } else {
            continue;
        }
    }

    if (MomentumType == "truth") {
        PolyFitterByType(MomentumType, 1, "Corr", "noKC", TL_FitParam_Corr_pol1);
        PolyFitterByType(MomentumType, 1, "Corr", "wKC", TL_FitParam_Corr_pol1_wKC);
        PolyFitterByType(MomentumType, 2, "Corr", "noKC", TL_FitParam_Corr_pol2);
        PolyFitterByType(MomentumType, 2, "Corr", "wKC", TL_FitParam_Corr_pol2_wKC);
        PolyFitterByType(MomentumType, 3, "Corr", "noKC", TL_FitParam_Corr_pol3);
        PolyFitterByType(MomentumType, 3, "Corr", "wKC", TL_FitParam_Corr_pol3_wKC);
        PolyFitterByType(MomentumType, 1, "Smear", "noKC", TL_FitParam_Smear_pol1);
        PolyFitterByType(MomentumType, 1, "Smear", "wKC", TL_FitParam_Smear_pol1_wKC);
        PolyFitterByType(MomentumType, 2, "Smear", "noKC", TL_FitParam_Smear_pol2);
        PolyFitterByType(MomentumType, 2, "Smear", "wKC", TL_FitParam_Smear_pol2_wKC);
        PolyFitterByType(MomentumType, 3, "Smear", "noKC", TL_FitParam_Smear_pol3);
        PolyFitterByType(MomentumType, 3, "Smear", "wKC", TL_FitParam_Smear_pol3_wKC);
    } else if (MomentumType == "reco") {
        PolyFitterByType(MomentumType, 1, "Corr", "noKC", Reco_FitParam_Corr_pol1);
        PolyFitterByType(MomentumType, 1, "Corr", "wKC", Reco_FitParam_Corr_pol1_wKC);
        PolyFitterByType(MomentumType, 2, "Corr", "noKC", Reco_FitParam_Corr_pol2);
        PolyFitterByType(MomentumType, 2, "Corr", "wKC", Reco_FitParam_Corr_pol2_wKC);
        PolyFitterByType(MomentumType, 3, "Corr", "noKC", Reco_FitParam_Corr_pol3);
        PolyFitterByType(MomentumType, 3, "Corr", "wKC", Reco_FitParam_Corr_pol3_wKC);
        PolyFitterByType(MomentumType, 1, "Smear", "noKC", Reco_FitParam_Smear_pol1);
        PolyFitterByType(MomentumType, 1, "Smear", "wKC", Reco_FitParam_Smear_pol1_wKC);
        PolyFitterByType(MomentumType, 2, "Smear", "noKC", Reco_FitParam_Smear_pol2);
        PolyFitterByType(MomentumType, 2, "Smear", "wKC", Reco_FitParam_Smear_pol2_wKC);
        PolyFitterByType(MomentumType, 3, "Smear", "noKC", Reco_FitParam_Smear_pol3);
        PolyFitterByType(MomentumType, 3, "Smear", "wKC", Reco_FitParam_Smear_pol3_wKC);
    }
}
//</editor-fold>

//<editor-fold desc="PolyFitterByType function">
void NeutronResolution::PolyFitterByType(const string &MomentumType, const int &PolynomialDegree, const string &FitType,
                                         const string &MomentumFitRange, vector <vector<double>> &FitParamResults) {
    cout << "\n\nPolyFitterByType variables:\n";
    bool PrintOut = false;
    bool PlotPoints = false;

    //<editor-fold desc="Setting fitter particle">
    string FitterParticle;

    if (isNeutron) {
        FitterParticle = "nFD";
    } else if (isProton) {
        FitterParticle = "pFD";
    }
    //</editor-fold>

    //<editor-fold desc="Setting polynomial">
    string PolynomialFuncName, PolynomialFuncStruct;

    if (MomentumFitRange == "noKC") {
        if (PolynomialDegree == 1) {
            PolynomialFuncName = MomentumType + "_f_" + FitType +
                                 "_pol1", PolynomialFuncStruct = "[0] * x + [1]"; // A*x + B
        } else if (PolynomialDegree == 2) {
            PolynomialFuncName = MomentumType + "_f_" + FitType +
                                 "_pol2", PolynomialFuncStruct = "[0] * x * x + [1] * x + [2]"; // A*x*x + B*x + C
        } else if (PolynomialDegree == 3) {
            PolynomialFuncName = MomentumType + "_f_" + FitType +
                                 "_pol3", PolynomialFuncStruct = "[0] * x * x * x + [1] * x * x + [2] * x + [3]"; // A*x*x*x + B*x*x + C*x + D
        }
    } else if (MomentumFitRange == "wKC") {
        if (PolynomialDegree == 1) {
            PolynomialFuncName = MomentumType + "_f_" + FitType +
                                 "_pol1_wKC", PolynomialFuncStruct = "[0] * x + [1]"; // A*x + B
        } else if (PolynomialDegree == 2) {
            PolynomialFuncName = MomentumType + "_f_" + FitType +
                                 "_pol2_wKC", PolynomialFuncStruct = "[0] * x * x + [1] * x + [2]"; // A*x*x + B*x + C
        } else if (PolynomialDegree == 3) {
            PolynomialFuncName = MomentumType + "_f_" + FitType +
                                 "_pol3_wKC", PolynomialFuncStruct = "[0] * x * x * x + [1] * x * x + [2] * x + [3]"; // A*x*x*x + B*x*x + C*x + D
        }
    } else {
        cout << "\n\nNeutronResolution::PolyFitterByType: MomentumFitRange is illegal! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //<editor-fold desc="Setting fitted slices">
    vector <hPlot1D> ResSlices0;
    vector <vector<double>> ResSlicesLimits0;
    vector <DSCuts> ResSlicesFitVar0, ResSlicesHistVar0;
    int NumberOfSlices0;
    vector<int> FittedSlices0;

    if (MomentumType == "truth") {
        ResSlices0 = ResTLMomSlices;
        ResSlicesLimits0 = ResTLMomSlicesLimits, ResSlicesFitVar0 = ResTLMomSlicesFitVar;
        ResSlicesHistVar0 = ResTLMomSlicesHistVar;
        NumberOfSlices0 = TL_NumberOfSlices, FittedSlices0 = FittedTLMomSlices;
    } else if (MomentumType == "reco") {
        ResSlices0 = ResRecoMomSlices;
        ResSlicesLimits0 = ResRecoMomSlicesLimits, ResSlicesFitVar0 = ResRecoMomSlicesFitVar;
        ResSlicesHistVar0 = ResRecoMomSlicesHistVar;
        NumberOfSlices0 = Reco_NumberOfSlices, FittedSlices0 = FittedRecoMomSlices;
    } else {
        cout << "\n\nNeutronResolution::PolyFitterByType: MomentumType is illegal! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //<editor-fold desc="Setting variable to fit">
    string FittedVar;

    if (FitType == "Smear") {
        FittedVar = "#sigma";
    } else if (FitType == "Corr") {
        FittedVar = "#mu";
    } else {
        cout << "\n\nNeutronResolution::PolyFitterByType: FitType is illegal! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //<editor-fold desc="Setting plot x and y data">
    vector<double> MeanPn, Pn_FitVar;

    for (int i = 0; i < NumberOfSlices0; i++) {
        double Mean = (ResSlicesLimits0.at(i).at(1) + ResSlicesLimits0.at(i).at(0)) / 2;

        double Std = ResSlicesFitVar0.at(i).GetUpperCut();
        double Corr = ResSlicesFitVar0.at(i).GetMean();

        if (MomentumFitRange == "wKC") {
            if ((Mean >= SliceLowerMomLimKC) && (Mean <= SliceUpperMomLimKC)) {
                if (FitType == "Smear") {
                    Pn_FitVar.push_back(Std); //TODO: add a mechanism to ignore failed fits
                    MeanPn.push_back(Mean);

                    if (PrintOut) {
                        cout << "\nMean = " << Mean << "\n";
                        cout << "Std = " << Std << "\n";
                        cout << "Corr = " << Corr << "\n";
                    }
                } else if (FitType == "Corr") {
                    Pn_FitVar.push_back(Corr); //TODO: add a mechanism to ignore failed fits
                    MeanPn.push_back(Mean);

                    if (PrintOut) {
                        cout << "\nMean = " << Mean << "\n";
                        cout << "Std = " << Std << "\n";
                        cout << "Corr = " << Corr << "\n";
                    }
                }
            }
        } else {
            if (FitType == "Smear") {
                Pn_FitVar.push_back(Std); //TODO: add a mechanism to ignore failed fits
                MeanPn.push_back(Mean);

                if (PrintOut) {
                    cout << "\nMean = " << Mean << "\n";
                    cout << "Std = " << Std << "\n";
                    cout << "Corr = " << Corr << "\n";
                }
            } else if (FitType == "Corr") {
                Pn_FitVar.push_back(Corr); //TODO: add a mechanism to ignore failed fits
                MeanPn.push_back(Mean);

                if (PrintOut) {
                    cout << "\nMean = " << Mean << "\n";
                    cout << "Std = " << Std << "\n";
                    cout << "Corr = " << Corr << "\n";
                }
            }
        }
    }

    if (PrintOut) {
        cout << "\nMeanPn.size() = " << MeanPn.size() << "\n";
        cout << "Pn_FitVar.size() = " << Pn_FitVar.size() << "\n";
        cout << "FitType = " << FitType << "\n\n";
    }

    //<editor-fold desc="Safety check">
    if (MeanPn.size() != Pn_FitVar.size()) {
        cout << "\n\nNeutronResolution::Fitter_Std_pol1: x and y data are of different lengths! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Setting fit canvas">
    TCanvas *Fit_Canvas = new TCanvas("Fit_Canvas", "Fit_Canvas", 1000, 750);
    Fit_Canvas->cd();
    Fit_Canvas->SetGrid();
    Fit_Canvas->SetBottomMargin(0.14), Fit_Canvas->SetLeftMargin(0.16), Fit_Canvas->SetRightMargin(0.12);
    //</editor-fold>

    //<editor-fold desc="Setting graph & preforming the fit">
    TGraph *Graph1D = new TGraph();
    Graph1D->SetName(PolynomialFuncName.c_str());

    for (int i = 0; i < MeanPn.size(); i++) {
        double x = MeanPn.at(i), y = Pn_FitVar.at(i);

        Graph1D->AddPoint(x, y);

        if (PlotPoints) {
            TLatex *latex = new TLatex(x, y, ("(" + to_string(x) + " , " + to_string(y) + ")").c_str());
            latex->SetTextSize(0.02);
            Graph1D->GetListOfFunctions()->Add(latex);
        }
    }

    Graph1D->GetXaxis()->SetTitleSize(0.06), Graph1D->GetXaxis()->SetLabelSize(0.0425), Graph1D->GetXaxis()->CenterTitle(true);
    Graph1D->GetYaxis()->SetTitleSize(0.06), Graph1D->GetYaxis()->SetLabelSize(0.0425), Graph1D->GetYaxis()->CenterTitle(true);

    Graph1D->GetXaxis()->SetTitle(("#bar{P}^{" + MomentumType + "}_{" + FitterParticle + "} [GeV/c]").c_str());

    if (FitType == "Smear") {
        Graph1D->SetTitle(("Proton smear fit (" + MomentumFitRange + ", linear fit)").c_str());
        Graph1D->GetYaxis()->SetTitle(("R_{" + FitterParticle + "} fit width").c_str());
    } else if (FitType == "Corr") {
        if (isNeutron) {
            Graph1D->GetYaxis()->SetTitle(("R_{" + FitterParticle + "} fit mean").c_str());
            Graph1D->SetTitle(("Neutron correction fit (" + MomentumFitRange + ", linear fit)").c_str());
        } else if (isProton) {
            Graph1D->GetYaxis()->SetTitle(("R_{" + FitterParticle + "} fit mean").c_str());
            Graph1D->SetTitle(("Proton correction fit (" + MomentumFitRange + ", linear fit)").c_str());
        }
    }

    TF1 *PolynomialFunc = new TF1(PolynomialFuncName.c_str(), PolynomialFuncStruct.c_str());

    Graph1D->Fit(PolynomialFunc);
    Graph1D->Draw("ap");
    Graph1D->SetMarkerStyle(21);
    //</editor-fold>

    //<editor-fold desc="Log fit results to variables">
    vector<double> FitVarResults, FitVarResultsErrors, FitVarResultsGoodness;

    if (PolynomialDegree == 1) {
        FitVarResults.push_back(PolynomialFunc->GetParameter(0)); // get [0]
        FitVarResults.push_back(PolynomialFunc->GetParameter(1)); // get [1]
        FitParamResults.push_back(FitVarResults);

        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(0)); // get [0]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(1)); // get [1]
        FitParamResults.push_back(FitVarResultsErrors);

        FitVarResultsGoodness.push_back(PolynomialFunc->GetChisquare()); // ChiSquare
        FitVarResultsGoodness.push_back(PolynomialFunc->GetNDF()); // NDF
        FitParamResults.push_back(FitVarResultsGoodness);
    } else if (PolynomialDegree == 2) {
        FitVarResults.push_back(PolynomialFunc->GetParameter(0)); // get [0]
        FitVarResults.push_back(PolynomialFunc->GetParameter(1)); // get [1]
        FitVarResults.push_back(PolynomialFunc->GetParameter(2)); // get [2]
        FitParamResults.push_back(FitVarResults);

        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(0)); // get [0]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(1)); // get [1]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(2)); // get [2]
        FitParamResults.push_back(FitVarResultsErrors);

        FitVarResultsGoodness.push_back(PolynomialFunc->GetChisquare()); // ChiSquare
        FitVarResultsGoodness.push_back(PolynomialFunc->GetNDF()); // NDF
        FitParamResults.push_back(FitVarResultsGoodness);
    } else if (PolynomialDegree == 3) {
        FitVarResults.push_back(PolynomialFunc->GetParameter(0)); // get [0]
        FitVarResults.push_back(PolynomialFunc->GetParameter(1)); // get [1]
        FitVarResults.push_back(PolynomialFunc->GetParameter(2)); // get [2]
        FitVarResults.push_back(PolynomialFunc->GetParameter(3)); // get [3]
        FitParamResults.push_back(FitVarResults);

        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(0)); // get [0]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(1)); // get [1]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(2)); // get [2]
        FitVarResultsErrors.push_back(PolynomialFunc->GetParError(3)); // get [3]
        FitParamResults.push_back(FitVarResultsErrors);

        FitVarResultsGoodness.push_back(PolynomialFunc->GetChisquare()); // ChiSquare
        FitVarResultsGoodness.push_back(PolynomialFunc->GetNDF()); // NDF
        FitParamResults.push_back(FitVarResultsGoodness);
    }
    //</editor-fold>

    //<editor-fold desc="Setting legened">

    //<editor-fold desc="Legened location">
    double x_1, y_1, x_2, y_2;
    double x_1_legend, y_1_legend, x_2_legend, y_2_legend;
    double x_1_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam;
    double x_1_Offset_Legened = 0.075, x_1_Offset_FitParam = 0.0375;

    if (PolynomialDegree == 3) { x_1_Offset_Legened = x_1_Offset_Legened * 2; }

    if (FitType == "Smear") {
        if (MomentumFitRange == "noKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        } else if (MomentumFitRange == "wKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        }
    } else if (FitType == "Corr") {
        if (MomentumFitRange == "noKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        } else if (MomentumFitRange == "wKC") {
            if (PolynomialDegree == 1) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025;
            } else if (PolynomialDegree == 2) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05;
            } else if (PolynomialDegree == 3) {
                x_1 = gStyle->GetStatX() - 0.6, y_1 = gStyle->GetStatY() - 0.2;
                x_2 = gStyle->GetStatX() - 0.2 - 0.6, y_2 = gStyle->GetStatY() - 0.3;
                x_1_legend = x_1 + 0.1 + 0.075, y_1_legend = y_1 + 0.125;
                x_2_legend = x_2, y_2_legend = y_2 + 0.15;
                x_1_FitParam = x_1, y_1_FitParam = y_1 + 0.025;
                x_2_FitParam = x_2, y_2_FitParam = y_2 + 0.025 - 0.05 - 0.05;
            }
        }
    }
    //</editor-fold>

    //<editor-fold desc="Setting fit variable legened">
    auto Graph1D_Legend = new TLegend(x_1_legend + x_1_Offset_Legened, y_1_legend, x_2_legend, y_2_legend);

    TLegendEntry *Graph1D_Legend_fit;

    string LegenedPolyStruct;

    if (PolynomialDegree == 1) {
        LegenedPolyStruct = FittedVar +
                            "(#bar{P}^{" + MomentumType + "}_{" + FitterParticle + "}) = A#bar{P}^{" + MomentumType + "}_{" + FitterParticle +
                            "} + B";
    } else if (PolynomialDegree == 2) {
        LegenedPolyStruct = FittedVar +
                            "(#bar{P}^{" + MomentumType + "}_{" + FitterParticle + "}) = A(#bar{P}^{" + MomentumType + "}_{" + FitterParticle +
                            "})^{2} + B#bar{P}^{" + MomentumType + "}_{" + FitterParticle + "} + C";
    } else if (PolynomialDegree == 3) {
        LegenedPolyStruct = FittedVar +
                            "(#bar{P}^{" + MomentumType + "}_{" + FitterParticle + "}) = A(#bar{P}^{" + MomentumType + "}_{" + FitterParticle +
                            "})^{3} + B(#bar{P}^{" + MomentumType + "}_{" + FitterParticle + "})^{2} + C#bar{P}^{" + MomentumType + "}_{" +
                            FitterParticle + "} + D";
    }

    Graph1D_Legend_fit = Graph1D_Legend->AddEntry(PolynomialFunc, LegenedPolyStruct.c_str(), "l");

    Graph1D_Legend->SetTextFont(42);
    Graph1D_Legend->SetTextSize(0.03);
    Graph1D_Legend->Draw("same");
    //</editor-fold>

    //<editor-fold desc="Setting fit variable chart">
    TPaveText *FitParam = new TPaveText(x_1_FitParam + x_1_Offset_FitParam, y_1_FitParam, x_2_FitParam, y_2_FitParam, "NDC");
    FitParam->SetBorderSize(1), FitParam->SetFillColor(0), FitParam->SetTextAlign(12), FitParam->SetTextFont(42), FitParam->SetTextSize(0.03);

    if (PolynomialDegree == 1) {
        FitParam->AddText(("A = " + to_string_with_precision(FitParamResults.at(0).at(0), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(0), 4)).c_str());
        FitParam->AddText(("B = " + to_string_with_precision(FitParamResults.at(0).at(1), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(1), 4)).c_str());
    } else if (PolynomialDegree == 2) {
        FitParam->AddText(("A = " + to_string_with_precision(FitParamResults.at(0).at(0), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(0), 4)).c_str());
        FitParam->AddText(("B = " + to_string_with_precision(FitParamResults.at(0).at(1), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(1), 4)).c_str());
        FitParam->AddText(("C = " + to_string_with_precision(FitParamResults.at(0).at(2), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(2), 4)).c_str());
    } else if (PolynomialDegree == 3) {
        FitParam->AddText(("A = " + to_string_with_precision(FitParamResults.at(0).at(0), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(0), 4)).c_str());
        FitParam->AddText(("B = " + to_string_with_precision(FitParamResults.at(0).at(1), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(1), 4)).c_str());
        FitParam->AddText(("C = " + to_string_with_precision(FitParamResults.at(0).at(2), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(2), 4)).c_str());
        FitParam->AddText(("D = " + to_string_with_precision(FitParamResults.at(0).at(3), 4) +
                           " #pm " + to_string_with_precision(FitParamResults.at(1).at(3), 4)).c_str());
    }

    FitParam->AddText(("#chi^{2}/NDF = " + to_string(FitParamResults.at(2).at(0) / FitParamResults.at(2).at(1))).c_str());
    FitParam->Draw("same");
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Save graph and exit">
    cout << "\n\n";

    auto ListOfFunctions = Graph1D->GetListOfFunctions();
    ListOfFunctions->Add((TLegend *) Graph1D_Legend);
    ListOfFunctions->Add((TPaveText *) FitParam);

    if (MomentumType == "truth") {
        if (isNeutron) {
            FittedTLNeutronResSlicesWidth->Add(Graph1D);
        } else if (isProton) {
            FittedTLProtonResSlicesWidth->Add(Graph1D);
        }
    } else if (MomentumType == "reco") {
        if (isNeutron) {
            FittedRecoNeutronResSlicesWidth->Add(Graph1D);
        } else if (isProton) {
            FittedRecoProtonResSlicesWidth->Add(Graph1D);
        }
    }

    string FitsDir = SlicesSavePath + "/" + "Graph1D_" + MomentumType + "_fits";
    string FitsDirByType = FitsDir + "/" + MomentumType + "_" + FitType + "_fits";
    TString GraphSaveName0 = FitsDirByType + "/" + "0" + PolynomialDegree + "_Fit_" + FitType + "_pol" + PolynomialDegree +
                             "_" + MomentumFitRange + ".png";

    system(("mkdir -p " + FitsDir).c_str());
    system(("mkdir -p " + FitsDirByType).c_str());

    cout << "\n", Fit_Canvas->SaveAs(GraphSaveName0);
    Fit_Canvas->Clear();
    delete Fit_Canvas;
    //</editor-fold>

}
//</editor-fold>

// DrawAndSaveResSlices function ----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="DrawAndSaveResSlices function">
void NeutronResolution::DrawAndSaveResSlices(const string &SampleName, TCanvas *h1DCanvas, const string &plots_path,
                                             const string &NeutronResolutionDirectory) {
    string SampleNameTemp = SampleName;

    if (isNeutron) {
        ResSlicePlots->Add(FittedTLNeutronResSlices), ResSlicePlots->Add(FittedTLNeutronResSlicesWidth), ResSlicePlots->Add(
                FittedTLNeutronResSlicesMean);
        ResSlicePlots->Add(FittedRecoNeutronResSlices), ResSlicePlots->Add(FittedRecoNeutronResSlicesWidth), ResSlicePlots->Add(
                FittedRecoNeutronResSlicesMean);
    } else if (isProton) {
        ResSlicePlots->Add(FittedTLProtonResSlices), ResSlicePlots->Add(FittedTLProtonResSlicesWidth), ResSlicePlots->Add(
                FittedTLProtonResSlicesMean);
        ResSlicePlots->Add(FittedRecoProtonResSlices), ResSlicePlots->Add(FittedRecoProtonResSlicesWidth), ResSlicePlots->Add(
                FittedRecoProtonResSlicesMean);
    }

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

//<editor-fold desc="LogResDataToFile function">
void NeutronResolution::LogResDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory) {
    //TODO: reorder file save in test mode properly
    string SaveDateDir = NeutronResolutionDirectory + "Res_data_-_" + SampleName + "/";

    if (!momResTestMode) {
        system(("mkdir -p " + SaveDateDir).c_str());

        LogFitDataToFile(SampleName, plots_path, SaveDateDir);
        LogHistDataToFile(SampleName, plots_path, SaveDateDir);
    } else {
        LogFitDataToFile(SampleName, plots_path, SaveDateDir);
        LogHistDataToFile(SampleName, plots_path, SaveDateDir);
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

    Neutron_res_fit_param << "#Fitted TL slices:\n";
    for (int FittedSlice: FittedTLMomSlices) {
        DSCuts TempCut = ResTLMomSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":"
                              << TempCut.GetSliceUpperb() << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param << "\n\n#Fitted Reco slices:\n";
    for (int FittedSlice: FittedRecoMomSlices) {
        DSCuts TempCut = ResRecoMomSlicesFitVar.at(FittedSlice);
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

//<editor-fold desc="LogFitDataToFile function">
void NeutronResolution::LogFitDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory) {
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
    Neutron_res_fit_param << "#particle:\t" << MomResParticle << "\n";
    Neutron_res_fit_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_fit_param << "#delta:\t\t" << delta << "\n";
    //</editor-fold>

    //<editor-fold desc="Logging slice fit results">
    Neutron_res_fit_param << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:FitMean:FitSigma\n\n";

    Neutron_res_fit_param << "#Fitted TL slices:\n";
    for (int FittedSlice: FittedTLMomSlices) {
        DSCuts TempCut = ResTLMomSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":"
                              << TempCut.GetSliceUpperb() << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }

    Neutron_res_fit_param << "\n\n#Fitted Reco slices:\n";
    for (int FittedSlice: FittedRecoMomSlices) {
        DSCuts TempCut = ResRecoMomSlicesFitVar.at(FittedSlice);
        Neutron_res_fit_param << TempCut.GetCutVariable() << "\t\t\t" << TempCut.GetSliceNumber() << ":" << TempCut.GetSliceLowerb() << ":"
                              << TempCut.GetSliceUpperb() << ":" << TempCut.GetMean() << ":" << TempCut.GetUpperCut() << "\n";
    }
    //</editor-fold>

    //<editor-fold desc="Logging correction and smear fit variables">
    if (isNeutron) {
        Neutron_res_fit_param << "\n\n#smearing fit variables:";

        Neutron_res_fit_param << "\n\n#pol1 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Smear_pol1", TL_FitParam_Smear_pol1, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Smear_pol1_wKC", Reco_FitParam_Smear_pol1_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol1 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Smear_pol1_wKC", TL_FitParam_Smear_pol1_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Smear_pol1_wKC", Reco_FitParam_Smear_pol1_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Smear_pol2", TL_FitParam_Smear_pol2, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Smear_pol2", Reco_FitParam_Smear_pol2, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Smear_pol2_wKC", TL_FitParam_Smear_pol2_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Smear_pol2_wKC", Reco_FitParam_Smear_pol2_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Smear_pol3", TL_FitParam_Smear_pol3, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Smear_pol3", Reco_FitParam_Smear_pol3, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Smear_pol3_wKC", TL_FitParam_Smear_pol3_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Smear_pol3_wKC", Reco_FitParam_Smear_pol3_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#correction fit variables:";

        Neutron_res_fit_param << "\n\n#pol1 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Corr_pol1", TL_FitParam_Corr_pol1, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Corr_pol1", Reco_FitParam_Corr_pol1, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol1 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Corr_pol1_wKC", TL_FitParam_Corr_pol1_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Corr_pol1_wKC", Reco_FitParam_Corr_pol1_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Corr_pol2", TL_FitParam_Corr_pol2, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Corr_pol2", Reco_FitParam_Corr_pol2, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol2 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Corr_pol2_wKC", TL_FitParam_Corr_pol2_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Corr_pol2_wKC", Reco_FitParam_Corr_pol2_wKC, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (no KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Corr_pol3", TL_FitParam_Corr_pol3, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Corr_pol3", Reco_FitParam_Corr_pol3, Neutron_res_fit_param);

        Neutron_res_fit_param << "\n\n#pol3 fit variables (with KC):\n";
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "truth_Corr_pol3_wKC", TL_FitParam_Corr_pol3_wKC, Neutron_res_fit_param);
        AutoLogger(SampleName, plots_path, NeutronResolutionDirectory, "reco_Corr_pol3_wKC", Reco_FitParam_Corr_pol3_wKC, Neutron_res_fit_param);
    }
    //</editor-fold>

    Neutron_res_fit_param.close();

    if (!momResTestMode) {
        /* Copy fitted parameters file to plots folder for easy download from the ifarm */
        system(("cp " + Neutron_res_fit_paramFilePath + " " + plots_path).c_str());
    }
}
//</editor-fold>

//<editor-fold desc="AutoLogger function">
void NeutronResolution::AutoLogger(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory,
                                   const string &LogHeader, const vector <vector<double>> &Vector2Log,
                                   ofstream &Neutron_res_fit_param) {
    if (Vector2Log.size() != 0) {
        vector <string> FitVarName = {"A", "B", "C", "D"}, FitVarErrName = {"A_Err", "B_Err", "C_Err", "D_Err"};
        string FitVarNameEnding = "# ", FitVarErrNameEnding = "# ";

        for (int i = 0; i < Vector2Log.at(0).size(); i++) {
            if (i > 0) {
                FitVarNameEnding = FitVarNameEnding + ":" + FitVarName.at(i);
                FitVarErrNameEnding = FitVarErrNameEnding + ":" + FitVarErrName.at(i);
            } else {
                FitVarNameEnding = FitVarNameEnding + FitVarName.at(i);
                FitVarErrNameEnding = FitVarErrNameEnding + FitVarErrName.at(i);
            }
        }

        for (int i = 0; i < Vector2Log.size(); i++) {
            if (i == 0) {
                Neutron_res_fit_param << LogHeader << ":" << "\t\t\t\t\t";

                for (int j = 0; j < Vector2Log.at(i).size(); j++) {
                    if (j == 0) {
                        Neutron_res_fit_param << Vector2Log.at(i).at(j);
                    } else if (j < (Vector2Log.at(i).size() - 1)) {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j);
                    } else {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j) << "     " << FitVarNameEnding << "\n";
                    }
                }
            } else if (i == 1) {
                Neutron_res_fit_param << LogHeader << "_error:" << "\t\t\t\t";

                for (int j = 0; j < Vector2Log.at(i).size(); j++) {
                    if (j == 0) {
                        Neutron_res_fit_param << Vector2Log.at(i).at(j);
                    } else if (j < (Vector2Log.at(i).size() - 1)) {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j);
                    } else {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j) << "     " << FitVarErrNameEnding << "\n";
                    }
                }
            } else if (i == 2) {
                Neutron_res_fit_param << LogHeader << "_FitGoodness:" << "\t\t";

                for (int j = 0; j < Vector2Log.at(i).size(); j++) {
                    if (j == 0) {
                        Neutron_res_fit_param << Vector2Log.at(i).at(j);
                    } else if (j < (Vector2Log.at(i).size() - 1)) {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j);
                    } else {
                        Neutron_res_fit_param << ":" << Vector2Log.at(i).at(j) << "     # CHI2:NDF\n";
                    }
                }
            }
        }
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

    Neutron_res_Hist_param
            << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:HistMean:HistSigma\n\n"; //TODO: confirm order!

    Neutron_res_Hist_param << "#Fitted TL slices:\n";
    for (DSCuts ResSlice: ResTLMomSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":"
                               << ResSlice.GetSliceUpperb()
                               << ":" << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param << "\n\n#Fitted Reco slices:\n";
    for (DSCuts ResSlice: ResRecoMomSlicesHistVar) {
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

//<editor-fold desc="LogHistDataToFile function">
void NeutronResolution::LogHistDataToFile(const string &SampleName, const string &plots_path, const string &NeutronResolutionDirectory) {
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
    Neutron_res_Hist_param << "#momRes_calculation_mode:\t" << momRes_calculation_mode << "\n";
    Neutron_res_Hist_param << "#delta:\t\t" << delta << "\n";
    //</editor-fold>

    Neutron_res_Hist_param
            << "#Parameters structure:\tSliceNumber:SliceLowerBoundary:SliceUpperBoundary:HistMean:HistSigma\n\n"; //TODO: confirm order!

    Neutron_res_Hist_param << "#Fitted TL slices:\n";
    for (DSCuts ResSlice: ResTLMomSlicesHistVar) {
        Neutron_res_Hist_param << ResSlice.GetCutVariable() << "\t\t\t" << ResSlice.GetSliceNumber() << ":" << ResSlice.GetSliceLowerb() << ":"
                               << ResSlice.GetSliceUpperb()
                               << ":" << ResSlice.GetMean() << ":" << ResSlice.GetUpperCut() << "\n";
    }

    Neutron_res_Hist_param << "\n\n#Fitted Reco slices:\n";
    for (DSCuts ResSlice: ResRecoMomSlicesHistVar) {
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
void NeutronResolution::ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const string &SampleName,
                                         const string &NucleonCutsDirectory, const bool &Load_correction, const bool &Load_smearing) {
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
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol1_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_A_Corr_pol1_wKC");
                            } else if (parameter == "A_Corr_pol1_wKC_Error") {
                                Loaded_A_Corr_pol1_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol1_wKC") {
                                Loaded_B_Corr_pol1_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol1_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_B_Corr_pol1_wKC");
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
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol1), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_A_Corr_pol1");
                            } else if (parameter == "A_Corr_pol1_Error") {
                                Loaded_A_Corr_pol1_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol1") {
                                Loaded_B_Corr_pol1 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol1), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_B_Corr_pol1");
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
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol2_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_A_Corr_pol2_wKC");
                            } else if (parameter == "A_Corr_pol2_wKC_Error") {
                                Loaded_A_Corr_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol2_wKC") {
                                Loaded_B_Corr_pol2_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol2_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_B_Corr_pol2_wKC");
                            } else if (parameter == "B_Corr_pol2_wKC_Error") {
                                Loaded_B_Corr_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol2_wKC") {
                                Loaded_C_Corr_pol2_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol2_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_C_Corr_pol2_wKC");
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
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol2), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_A_Corr_pol2");
                            } else if (parameter == "A_Corr_pol2_Error") {
                                Loaded_A_Corr_pol2_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol2") {
                                Loaded_B_Corr_pol2 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol2), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_B_Corr_pol2");
                            } else if (parameter == "B_Corr_pol2_Error") {
                                Loaded_B_Corr_pol2_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol2") {
                                Loaded_C_Corr_pol2 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol2), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_C_Corr_pol2");
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
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_A_Corr_pol3_wKC");
                            } else if (parameter == "A_Corr_pol3_wKC_Error") {
                                Loaded_A_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol3_wKC") {
                                Loaded_B_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_B_Corr_pol3_wKC");
                            } else if (parameter == "B_Corr_pol3_wKC_Error") {
                                Loaded_B_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol3_wKC") {
                                Loaded_C_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_C_Corr_pol3_wKC");
                            } else if (parameter == "C_Corr_pol3_wKC_Error") {
                                Loaded_C_Corr_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "D_Corr_pol3_wKC") {
                                Loaded_D_Corr_pol3_wKC = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_D_Corr_pol3_wKC), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_D_Corr_pol3_wKC");
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
                                Loaded_Corr_coefficients_values.push_back(Loaded_A_Corr_pol3), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_A_Corr_pol3");
                            } else if (parameter == "A_Corr_pol3_Error") {
                                Loaded_A_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "B_Corr_pol3") {
                                Loaded_B_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_B_Corr_pol3), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_B_Corr_pol3");
                            } else if (parameter == "B_Corr_pol3_Error") {
                                Loaded_B_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "C_Corr_pol3") {
                                Loaded_C_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_C_Corr_pol3), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_C_Corr_pol3");
                            } else if (parameter == "C_Corr_pol3_Error") {
                                Loaded_C_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "D_Corr_pol3") {
                                Loaded_D_Corr_pol3 = stod(parameter2);
                                Loaded_Corr_coefficients_values.push_back(Loaded_D_Corr_pol3), Loaded_Corr_coefficients_names.push_back(
                                        "Loaded_D_Corr_pol3");
                            } else if (parameter == "D_Corr_pol3_Error") {
                                Loaded_D_Corr_pol3_Error = stod(parameter2);
                            } else if (parameter == "ChiSquare_Corr_pol3") {
                                Loaded_ChiSquare_Corr_pol3 = stod(parameter2);
                            } else if (parameter == "NDF_Corr_pol3") {
                                Loaded_NDF_Corr_pol3 = stod(parameter2);
                            }
                        }
                    }
                } else if (Load_smearing && findSubstring(parameter, "Smear")) {
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
                                Loaded_Smear_coefficients_values.push_back(Loaded_A_Std_pol1_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_A_Std_pol1_wKC");
                            } else if (parameter == "A_Std_pol1_wKC_Error") {
                                Loaded_A_Std_pol1_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol1_wKC") {
                                Loaded_B_Std_pol1_wKC = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_B_Std_pol1_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_B_Std_pol1_wKC");
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
                                Loaded_Smear_coefficients_values.push_back(Loaded_A_Std_pol1), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_A_Std_pol1");
                            } else if (parameter == "A_Std_pol1_Error") {
                                Loaded_A_Std_pol1_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol1") {
                                Loaded_B_Std_pol1 = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_B_Std_pol1), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_B_Std_pol1");
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
                                Loaded_Smear_coefficients_values.push_back(Loaded_A_Std_pol2_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_A_Std_pol2_wKC");
                            } else if (parameter == "A_Std_pol2_wKC_Error") {
                                Loaded_A_Std_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol2_wKC") {
                                Loaded_B_Std_pol2_wKC = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_B_Std_pol2_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_B_Std_pol2_wKC");
                            } else if (parameter == "B_Std_pol2_wKC_Error") {
                                Loaded_B_Std_pol2_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol2_wKC") {
                                Loaded_C_Std_pol2_wKC = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_C_Std_pol2_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_C_Std_pol2_wKC");
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
                                Loaded_Smear_coefficients_values.push_back(Loaded_A_Std_pol2), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_A_Std_pol2");
                            } else if (parameter == "A_Std_pol2_Error") {
                                Loaded_A_Std_pol2_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol2") {
                                Loaded_B_Std_pol2 = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_B_Std_pol2), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_B_Std_pol2");
                            } else if (parameter == "B_Std_pol2_Error") {
                                Loaded_B_Std_pol2_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol2") {
                                Loaded_C_Std_pol2 = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_C_Std_pol2), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_C_Std_pol2");
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
                                Loaded_Smear_coefficients_values.push_back(Loaded_A_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_A_Std_pol3_wKC");
                            } else if (parameter == "A_Std_pol3_wKC_Error") {
                                Loaded_A_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol3_wKC") {
                                Loaded_B_Std_pol3_wKC = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_B_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_B_Std_pol3_wKC");
                            } else if (parameter == "B_Std_pol3_wKC_Error") {
                                Loaded_B_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol3_wKC") {
                                Loaded_C_Std_pol3_wKC = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_C_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_C_Std_pol3_wKC");
                            } else if (parameter == "C_Std_pol3_wKC_Error") {
                                Loaded_C_Std_pol3_wKC_Error = stod(parameter2);
                            } else if (parameter == "D_Std_pol3_wKC") {
                                Loaded_D_Std_pol3_wKC = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_D_Std_pol3_wKC), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_D_Std_pol3_wKC");
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
                                Loaded_Smear_coefficients_values.push_back(Loaded_A_Std_pol3), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_A_Std_pol3");
                            } else if (parameter == "A_Std_pol3_Error") {
                                Loaded_A_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "B_Std_pol3") {
                                Loaded_B_Std_pol3 = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_B_Std_pol3), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_B_Std_pol3");
                            } else if (parameter == "B_Std_pol3_Error") {
                                Loaded_B_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "C_Std_pol3") {
                                Loaded_C_Std_pol3 = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_C_Std_pol3), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_C_Std_pol3");
                            } else if (parameter == "C_Std_pol3_Error") {
                                Loaded_C_Std_pol3_Error = stod(parameter2);
                            } else if (parameter == "D_Std_pol3") {
                                Loaded_D_Std_pol3 = stod(parameter2);
                                Loaded_Smear_coefficients_values.push_back(Loaded_D_Std_pol3), Loaded_Std_coefficients_names.push_back(
                                        "Loaded_D_Std_pol3");
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
                if (Loaded_Smear_coefficients_values.size() != Loaded_Std_coefficients_names.size()) {
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

//<editor-fold desc="ReadResDataParam function">
void NeutronResolution::ReadResDataParam(const char *filename, const bool &Calculate_momResS2, const string &SampleName,
                                         const string &NucleonCutsDirectory, const string &MomentumType, const bool &Load_correction = false,
                                         const bool &Load_smearing = false) {
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
                if (findSubstring(parameter, "fit_" + MomentumType + "_" + "Slice_#")) {
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
                } else if (findSubstring(parameter, "hist_" + MomentumType + "_" + "Slice_#")) {
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
                if (Load_correction && findSubstring(parameter, MomentumType) && findSubstring(parameter, "Corr")) {
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
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Corr_pol1_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Corr_pol1_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (findSubstring(parameter, "pol1") && !findSubstring(CorrMode, "pol1_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Corr_pol1, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Corr_pol1, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    } else if (findSubstring(parameter, "pol2") && findSubstring(CorrMode, "pol2")) {
                        if (findSubstring(parameter, "pol2_wKC") && findSubstring(CorrMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Corr_pol2_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Corr_pol2_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (findSubstring(parameter, "pol2") && !findSubstring(CorrMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Corr_pol2, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Corr_pol2, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    } else if (findSubstring(parameter, "pol3") && findSubstring(CorrMode, "pol3")) {
                        if (findSubstring(parameter, "pol3_wKC") && findSubstring(CorrMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Corr_pol3_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Corr_pol3_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (findSubstring(parameter, "pol3") && !findSubstring(CorrMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Corr_pol3, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Corr_pol3, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    }
                } else if (Load_smearing && findSubstring(parameter, MomentumType) && findSubstring(parameter, "Smear")) {
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
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Smear_pol1_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Smear_pol1_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (findSubstring(parameter, "pol1") && !findSubstring(SmearMode, "pol1_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_TL_FitParam_Smear_pol1, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 1, parameter, ss2, Loaded_Reco_FitParam_Smear_pol1, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    } else if (findSubstring(parameter, "pol2") && findSubstring(SmearMode, "pol2")) {
                        if (findSubstring(parameter, "pol2_wKC") && findSubstring(SmearMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Smear_pol2_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Smear_pol2_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (findSubstring(parameter, "pol2") && !findSubstring(SmearMode, "pol2_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_TL_FitParam_Smear_pol2, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 2, parameter, ss2, Loaded_Reco_FitParam_Smear_pol2, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    } else if (findSubstring(parameter, "pol3") && findSubstring(SmearMode, "pol3")) {
                        if (findSubstring(parameter, "pol3_wKC") && findSubstring(SmearMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Smear_pol3_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Smear_pol3_wKC, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        } else if (findSubstring(parameter, "pol3") && !findSubstring(SmearMode, "pol3_wKC")) {
                            if (MomentumType == "truth") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_TL_FitParam_Smear_pol3, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            } else if (MomentumType == "reco") {
                                AutoReader(MomentumType, 3, parameter, ss2, Loaded_Reco_FitParam_Smear_pol3, Loaded_Corr_coefficients_values,
                                           Loaded_Corr_coefficients_names);
                            }
                        }
                    }
                }

                //<editor-fold desc="Safety checks">
                if (Loaded_Smear_coefficients_values.size() != Loaded_Std_coefficients_names.size()) {
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

//<editor-fold desc="AutoReader function">
void NeutronResolution::AutoReader(const string &MomentumType, const int &PolynomialDegree, const string &parameter, basic_istream<char> &ss2,
                                   vector <vector<double>> &Loading_Dest, vector<double> &Loaded_coefficients_values,
                                   vector <string> &Loaded_coefficients_names) {
    if (findSubstring(parameter, MomentumType)) {
        vector <string> VarNames = {"A", "B", "C", "D"};
        vector<double> Loaded_FitVarResults, Loaded_FitVarResultsErrors, Loaded_FitVarResultsGoodness;
        string Loaded_FitVar;

        int counter = 0;

        while (getline(ss2, Loaded_FitVar, ':')) {
            if (!findSubstring(parameter, "error") && !findSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResults.push_back(stod(Loaded_FitVar));
                Loaded_coefficients_values.push_back(stod(Loaded_FitVar));
                Loaded_coefficients_names.push_back((VarNames.at(counter) + " (" + MomentumType + ")"));
                ++counter;
            } else if (findSubstring(parameter, "error") && !findSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResultsErrors.push_back(stod(Loaded_FitVar));
            } else if (!findSubstring(parameter, "error") && findSubstring(parameter, "FitGoodness")) {
                Loaded_FitVarResultsGoodness.push_back(stod(Loaded_FitVar));
            }
        }

        Loading_Dest.push_back(Loaded_FitVarResults);
        Loading_Dest.push_back(Loaded_FitVarResultsErrors);
        Loading_Dest.push_back(Loaded_FitVarResultsGoodness);

        //<editor-fold desc="Saftey check">
        if ((Loaded_FitVarResults.size() != PolynomialDegree) || (Loaded_FitVarResultsErrors.size() != PolynomialDegree)) {
            cout << "\n\nNeutronResolution::AutoReader: the parameter " << parameter << " loaded improperly! Exiting...\n\n", exit(0);
        }
        //</editor-fold>
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
                    cout << "\n\nLoaded_Res_Slices_FitVar.GetUpperCut() = "
                         << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut() << "\n";
                    cout << "Loaded_res_slice.GetSliceLowerb() = "
                         << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceLowerb() << "\n";
                    cout << "Loaded_res_slice.GetSliceUpperb() = "
                         << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceUpperb() << "\n";
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
                Arg = Loaded_TL_FitParam_Smear_pol1.at(0).at(0) * Momentum +
                      Loaded_TL_FitParam_Smear_pol1.at(0).at(1);

                if (Printout_Std_Variables) {
                    cout << "\n\n\nLoaded_A_Std_pol1 = " << Loaded_TL_FitParam_Smear_pol1.at(0).at(0) << "\n";
                    cout << "Loaded_B_Std_pol1 = " << Loaded_TL_FitParam_Smear_pol1.at(0).at(1) << "\n\n";
                }
            } else if (SmearMode == "pol1_wKC") {
                Arg = Loaded_TL_FitParam_Smear_pol1_wKC.at(0).at(0) * Momentum +
                      Loaded_TL_FitParam_Smear_pol1_wKC.at(0).at(1);

                if (Printout_Std_Variables) {
                    cout << "\n\n\nLoaded_A_Std_pol1_wKC = " << Loaded_TL_FitParam_Smear_pol1_wKC.at(0).at(0) << "\n";
                    cout << "Loaded_B_Std_pol1_wKC = " << Loaded_TL_FitParam_Smear_pol1_wKC.at(0).at(1) << "\n\n";
                }
            } else if (SmearMode == "pol2") {
                Arg = Loaded_TL_FitParam_Smear_pol2.at(0).at(0) * Momentum2 +
                      Loaded_TL_FitParam_Smear_pol2.at(0).at(1) * Momentum +
                      Loaded_TL_FitParam_Smear_pol2.at(0).at(2);

                if (Printout_Std_Variables) {
                    cout << "\n\n\nLoaded_A_Std_pol2 = " << Loaded_TL_FitParam_Smear_pol2.at(0).at(0) << "\n";
                    cout << "Loaded_B_Std_pol2 = " << Loaded_TL_FitParam_Smear_pol2.at(0).at(1) << "\n";
                    cout << "Loaded_C_Std_pol2 = " << Loaded_TL_FitParam_Smear_pol2.at(0).at(2) << "\n\n";
                }
            } else if (SmearMode == "pol2_wKC") {
                Arg = Loaded_TL_FitParam_Smear_pol2_wKC.at(0).at(0) * Momentum2 +
                      Loaded_TL_FitParam_Smear_pol2_wKC.at(0).at(1) * Momentum +
                      Loaded_TL_FitParam_Smear_pol2_wKC.at(0).at(2);

                if (Printout_Std_Variables) {
                    cout << "\n\n\nLoaded_A_Std_pol2_wKC = " << Loaded_TL_FitParam_Smear_pol2_wKC.at(0).at(0) << "\n";
                    cout << "Loaded_B_Std_pol2_wKC = " << Loaded_TL_FitParam_Smear_pol2_wKC.at(0).at(1) << "\n";
                    cout << "Loaded_C_Std_pol2_wKC = " << Loaded_TL_FitParam_Smear_pol2_wKC.at(0).at(2) << "\n\n";
                }
            } else if (SmearMode == "pol3") {
                Arg = Loaded_TL_FitParam_Smear_pol3.at(0).at(0) * Momentum3 +
                      Loaded_TL_FitParam_Smear_pol3.at(0).at(1) * Momentum2 +
                      Loaded_TL_FitParam_Smear_pol3.at(0).at(2) * Momentum +
                      Loaded_TL_FitParam_Smear_pol3.at(0).at(3);

                if (Printout_Std_Variables) {
                    cout << "\n\n\nLoaded_A_Std_pol3 = " << Loaded_TL_FitParam_Smear_pol3.at(0).at(0) << "\n";
                    cout << "Loaded_B_Std_pol3 = " << Loaded_TL_FitParam_Smear_pol3.at(0).at(1) << "\n";
                    cout << "Loaded_C_Std_pol3 = " << Loaded_TL_FitParam_Smear_pol3.at(0).at(2) << "\n";
                    cout << "Loaded_D_Std_pol3 = " << Loaded_TL_FitParam_Smear_pol3.at(0).at(3) << "\n\n";
                }
            } else if (SmearMode == "pol3_wKC") {
                Arg = Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(0) * Momentum3 +
                      Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(1) * Momentum2 +
                      Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(2) * Momentum +
                      Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(3);

                if (Printout_Std_Variables) {
                    cout << "\n\n\nLoaded_A_Std_pol3_wKC = " << Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(0) << "\n";
                    cout << "Loaded_B_Std_pol3_wKC = " << Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(1) << "\n";
                    cout << "Loaded_C_Std_pol3_wKC = " << Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(2) << "\n";
                    cout << "Loaded_D_Std_pol3_wKC = " << Loaded_TL_FitParam_Smear_pol3_wKC.at(0).at(3) << "\n\n";
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

////<editor-fold desc="PSmear function (original)">
///* A function to smear protons by fitted neutron resolution */
//
//double NeutronResolution::PSmear(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
//    bool Printout = false;
//    bool Printout_Std_Variables = false;
//
//    if (!apply_nucleon_SmearAndCorr) {
//        /* Smearing and correction are disabled */
//        return Momentum;
//    } else {
//        /* Smearing and correction are enabled */
//
//        /* Setting momentum powers */
//        double SmearedMomentum, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;
//
//        if (SmearMode == "slices") {
//            /* Smear using slice fit results */
//
//            //<editor-fold desc="Original (smearing from loaded nRes fit variables)">
//            if (Momentum < SliceUpperMomLim) { // NOTE: changed according to upper neutron mom. th.
//                for (DSCuts Loaded_res_slice: Loaded_Res_Slices_FitVar) {
//                    if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                        double Smearing = Rand->Gaus(1, Loaded_res_slice.GetUpperCut());
//
//                        if (Printout) {
//                            cout << "\n\nLoaded_res_slice.GetUpperCut() = " << Loaded_res_slice.GetUpperCut() << "\n";
//                            cout << "Momentum = " << Momentum << "\n";
//                            cout << "Smearing = " << Smearing << "\n";
//                            cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
//                        }
//
//                        return Smearing * Momentum;
//                    }
//                }
//            } else {
//                double Smearing = Rand->Gaus(1, Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut());
//
//                if (Printout) {
//                    cout << "\n\nLoaded_Res_Slices_FitVar.GetUpperCut() = "
//                         << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetUpperCut() << "\n";
//                    cout << "Loaded_res_slice.GetSliceLowerb() = "
//                         << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceLowerb() << "\n";
//                    cout << "Loaded_res_slice.GetSliceUpperb() = "
//                         << Loaded_Res_Slices_FitVar.at(Loaded_Res_Slices_FitVar.size() - 1).GetSliceUpperb() << "\n";
//                    cout << "Momentum = " << Momentum << "\n";
//                    cout << "Smearing = " << Smearing << "\n";
//                    cout << "Smearing * Momentum = " << Smearing * Momentum << "\n\n";
//                }
//
//                return Smearing * Momentum;
//            }
//            //</editor-fold>
//
//        } else if ((SmearMode == "pol1") || (SmearMode == "pol2") || (SmearMode == "pol3") ||
//                   (SmearMode == "pol1_wKC") || (SmearMode == "pol2_wKC") || (SmearMode == "pol3_wKC")) {
//            /* Smear using pol fit results */
//            double Smearing, Arg;
//
//            if (SmearMode == "pol1") {
//                Arg = Loaded_A_Std_pol1 * Momentum + Loaded_B_Std_pol1;
//
//                if (Printout_Std_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Std_pol1 = " << Loaded_A_Std_pol1 << "\n";
//                    cout << "Loaded_B_Std_pol1 = " << Loaded_B_Std_pol1 << "\n\n";
//                }
//            } else if (SmearMode == "pol1_wKC") {
//                Arg = Loaded_A_Std_pol1_wKC * Momentum + Loaded_B_Std_pol1_wKC;
//
//                if (Printout_Std_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Std_pol1_wKC = " << Loaded_A_Std_pol1_wKC << "\n";
//                    cout << "Loaded_B_Std_pol1_wKC = " << Loaded_B_Std_pol1_wKC << "\n\n";
//                }
//            } else if (SmearMode == "pol2") {
//                Arg = Loaded_A_Std_pol2 * Momentum2 + Loaded_B_Std_pol2 * Momentum + Loaded_C_Std_pol2;
//
//                if (Printout_Std_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Std_pol2 = " << Loaded_A_Std_pol2 << "\n";
//                    cout << "Loaded_B_Std_pol2 = " << Loaded_B_Std_pol2 << "\n";
//                    cout << "Loaded_C_Std_pol2 = " << Loaded_C_Std_pol2 << "\n\n";
//                }
//            } else if (SmearMode == "pol2_wKC") {
//                Arg = Loaded_A_Std_pol2_wKC * Momentum2 + Loaded_B_Std_pol2_wKC * Momentum + Loaded_C_Std_pol2_wKC;
//
//                if (Printout_Std_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Std_pol2_wKC = " << Loaded_A_Std_pol2_wKC << "\n";
//                    cout << "Loaded_B_Std_pol2_wKC = " << Loaded_B_Std_pol2_wKC << "\n";
//                    cout << "Loaded_C_Std_pol2_wKC = " << Loaded_C_Std_pol2_wKC << "\n\n";
//                }
//            } else if (SmearMode == "pol3") {
//                Arg = Loaded_A_Std_pol3 * Momentum3 + Loaded_B_Std_pol3 * Momentum2 + Loaded_C_Std_pol3 * Momentum + Loaded_D_Std_pol3;
//
//                if (Printout_Std_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Std_pol3 = " << Loaded_A_Std_pol3 << "\n";
//                    cout << "Loaded_B_Std_pol3 = " << Loaded_B_Std_pol3 << "\n";
//                    cout << "Loaded_C_Std_pol3 = " << Loaded_C_Std_pol3 << "\n";
//                    cout << "Loaded_D_Std_pol3 = " << Loaded_D_Std_pol3 << "\n\n";
//                }
//            } else if (SmearMode == "pol3_wKC") {
//                Arg = Loaded_A_Std_pol3_wKC * Momentum3 + Loaded_B_Std_pol3_wKC * Momentum2 + Loaded_C_Std_pol3_wKC * Momentum +
//                      Loaded_D_Std_pol3_wKC;
//
//                if (Printout_Std_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Std_pol3_wKC = " << Loaded_A_Std_pol3_wKC << "\n";
//                    cout << "Loaded_B_Std_pol3_wKC = " << Loaded_B_Std_pol3_wKC << "\n";
//                    cout << "Loaded_C_Std_pol3_wKC = " << Loaded_C_Std_pol3_wKC << "\n";
//                    cout << "Loaded_D_Std_pol3_wKC = " << Loaded_D_Std_pol3_wKC << "\n\n";
//                }
//            }
//
//            Smearing = Rand->Gaus(1, Arg);
//            SmearedMomentum = Smearing * Momentum; // minus for protons and plus for protons
//
//            if (Printout) {
//                cout << "\n\nSmearMode = " << SmearMode << "\n";
//                cout << "Arg = " << Arg << "\n";
//                cout << "Smearing = " << Smearing << "\n";
//                cout << "Momentum = " << Momentum << "\n";
//                cout << "SmearedMomentum = " << SmearedMomentum << "\n\n";
//            }
//
//        } else {
//            cout << "\n\nNeutronResolution::PSmear: SmearMode illegal! Exiting...", exit(0);
//        }
//
//        return SmearedMomentum;
//    }
//}
////</editor-fold>

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

            //TODO: add choice mechanism between truth and reco correction factors

            if (CorrMode == "pol1") {
                Mu = Loaded_Reco_FitParam_Corr_pol1.at(0).at(0) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol1.at(0).at(1);

                if (Printout_Corr_Variables) {
                    cout << "\n\n\nLoaded_A_Corr_pol1 = " << Loaded_Reco_FitParam_Corr_pol1.at(0).at(0) << "\n";
                    cout << "Loaded_B_Corr_pol1 = " << Loaded_Reco_FitParam_Corr_pol1.at(0).at(1) << "\n\n";
                }
            } else if (CorrMode == "pol1_wKC") {
                Mu = Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(0) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(1);

                if (Printout_Corr_Variables) {
                    cout << "\n\n\nLoaded_A_Corr_pol1_wKC = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(0) << "\n";
                    cout << "Loaded_B_Corr_pol1_wKC = " << Loaded_Reco_FitParam_Corr_pol1_wKC.at(0).at(1) << "\n\n";
                }
            } else if (CorrMode == "pol2") {
                Mu = Loaded_Reco_FitParam_Corr_pol2.at(0).at(0) * Momentum2 +
                     Loaded_Reco_FitParam_Corr_pol2.at(0).at(1) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol2.at(0).at(2);

                if (Printout_Corr_Variables) {
                    cout << "\n\n\nLoaded_A_Corr_pol2 = " << Loaded_Reco_FitParam_Corr_pol2.at(0).at(0) << "\n";
                    cout << "Loaded_B_Corr_pol2 = " << Loaded_Reco_FitParam_Corr_pol2.at(0).at(1) << "\n";
                    cout << "Loaded_C_Corr_pol2 = " << Loaded_Reco_FitParam_Corr_pol2.at(0).at(2) << "\n\n";
                }
            } else if (CorrMode == "pol2_wKC") {
                Mu = Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(0) * Momentum2 +
                     Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(1) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(2);

                if (Printout_Corr_Variables) {
                    cout << "\n\n\nLoaded_A_Corr_pol2_wKC = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(0) << "\n";
                    cout << "Loaded_B_Corr_pol2_wKC = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(1) << "\n";
                    cout << "Loaded_C_Corr_pol2_wKC = " << Loaded_Reco_FitParam_Corr_pol2_wKC.at(0).at(2) << "\n\n";
                }
            } else if (CorrMode == "pol3") {
                Mu = Loaded_Reco_FitParam_Corr_pol3.at(0).at(0) * Momentum3 +
                     Loaded_Reco_FitParam_Corr_pol3.at(0).at(1) * Momentum2 +
                     Loaded_Reco_FitParam_Corr_pol3.at(0).at(2) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol3.at(0).at(3);

                if (Printout_Corr_Variables) {
                    cout << "\n\n\nLoaded_A_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(0) << "\n";
                    cout << "Loaded_B_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(1) << "\n";
                    cout << "Loaded_C_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(2) << "\n";
                    cout << "Loaded_D_Corr_pol3 = " << Loaded_Reco_FitParam_Corr_pol3.at(0).at(3) << "\n\n";
                }
            } else if (CorrMode == "pol3_wKC") {
                Mu = Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(0) * Momentum3 +
                     Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(1) * Momentum2 +
                     Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(2) * Momentum +
                     Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(3);

                if (Printout_Corr_Variables) {
                    cout << "\n\n\nLoaded_A_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(0) << "\n";
                    cout << "Loaded_B_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(1) << "\n";
                    cout << "Loaded_C_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(2) << "\n";
                    cout << "Loaded_D_Corr_pol3_wKC = " << Loaded_Reco_FitParam_Corr_pol3_wKC.at(0).at(3) << "\n\n";
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

////<editor-fold desc="NCorr function (original)">
///* A function to correction (calibrate) neutron momentum by fitted neutron correction */
//
//double NeutronResolution::NCorr(const bool &apply_nucleon_SmearAndCorr, const double &Momentum) {
//    bool Printout = false;
//    bool Printout_Corr_Variables = false;
//
//    if (!apply_nucleon_SmearAndCorr) {
//        /* Smearing and correction are disabled */
//        return Momentum;
//    } else {
//        /* Smearing and correction are enabled */
//
//        /* Setting corrected momentum & momentum powers */
//        double CorrectedMomentum, Momentum2 = Momentum * Momentum, Momentum3 = Momentum2 * Momentum;
//
//        if (CorrMode == "slices") {
//            /* Correction using slice fit results */
//
//            //<editor-fold desc="Original (correction from loaded nRes fit variables)">
//            for (DSCuts Loaded_res_slice: Loaded_Res_Slices_HistVar) {
//                if ((Loaded_res_slice.GetSliceLowerb() < Momentum) && (Loaded_res_slice.GetSliceUpperb() >= Momentum)) {
//                    CorrectedMomentum = Momentum * (1 + Loaded_res_slice.GetMean()); // minus for protons and plus for neutrons
//
//                    if (Printout) {
//                        cout << "\n\nLoaded_res_slice.GetMean() = " << Loaded_res_slice.GetMean() << "\n";
//                        cout << "Momentum = " << Momentum << "\n";
//                        cout << "Correction = " << Loaded_res_slice.GetMean() << "\n";
//                        cout << "CorrectedMomentum = " << CorrectedMomentum << "\n\n";
//                    }
//                }
//            }
//            //</editor-fold>
//
//        } else if ((CorrMode == "pol1") || (CorrMode == "pol2") || (CorrMode == "pol3") ||
//                   (CorrMode == "pol1_wKC") || (CorrMode == "pol2_wKC") || (CorrMode == "pol3_wKC")) {
//            /* Correction using pol fit results */
//            double Mu, CorrectionFactor;
//
//            if (CorrMode == "pol1") {
//                Mu = Loaded_A_Corr_pol1 * Momentum + Loaded_B_Corr_pol1;
//
//                if (Printout_Corr_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Corr_pol1 = " << Loaded_A_Corr_pol1 << "\n";
//                    cout << "Loaded_B_Corr_pol1 = " << Loaded_B_Corr_pol1 << "\n\n";
//                }
//            } else if (CorrMode == "pol1_wKC") {
//                Mu = Loaded_A_Corr_pol1_wKC * Momentum + Loaded_B_Corr_pol1_wKC;
//
//                if (Printout_Corr_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Corr_pol1_wKC = " << Loaded_A_Corr_pol1_wKC << "\n";
//                    cout << "Loaded_B_Corr_pol1_wKC = " << Loaded_B_Corr_pol1_wKC << "\n\n";
//                }
//            } else if (CorrMode == "pol2") {
//                Mu = Loaded_A_Corr_pol2 * Momentum2 + Loaded_B_Corr_pol2 * Momentum + Loaded_C_Corr_pol2;
//
//                if (Printout_Corr_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Corr_pol2 = " << Loaded_A_Corr_pol2 << "\n";
//                    cout << "Loaded_B_Corr_pol2 = " << Loaded_B_Corr_pol2 << "\n";
//                    cout << "Loaded_C_Corr_pol2 = " << Loaded_C_Corr_pol2 << "\n\n";
//                }
//            } else if (CorrMode == "pol2_wKC") {
//                Mu = Loaded_A_Corr_pol2_wKC * Momentum2 + Loaded_B_Corr_pol2_wKC * Momentum + Loaded_C_Corr_pol2_wKC;
//
//                if (Printout_Corr_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Corr_pol2_wKC = " << Loaded_A_Corr_pol2_wKC << "\n";
//                    cout << "Loaded_B_Corr_pol2_wKC = " << Loaded_B_Corr_pol2_wKC << "\n";
//                    cout << "Loaded_C_Corr_pol2_wKC = " << Loaded_C_Corr_pol2_wKC << "\n\n";
//                }
//            } else if (CorrMode == "pol3") {
//                Mu = Loaded_A_Corr_pol3 * Momentum3 + Loaded_B_Corr_pol3 * Momentum2 + Loaded_C_Corr_pol3 * Momentum + Loaded_D_Corr_pol3;
//
//                if (Printout_Corr_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Corr_pol3 = " << Loaded_A_Corr_pol3 << "\n";
//                    cout << "Loaded_B_Corr_pol3 = " << Loaded_B_Corr_pol3 << "\n";
//                    cout << "Loaded_C_Corr_pol3 = " << Loaded_C_Corr_pol3 << "\n";
//                    cout << "Loaded_D_Corr_pol3 = " << Loaded_D_Corr_pol3 << "\n\n";
//                }
//            } else if (CorrMode == "pol3_wKC") {
//                Mu = Loaded_A_Corr_pol3_wKC * Momentum3 + Loaded_B_Corr_pol3_wKC * Momentum2 + Loaded_C_Corr_pol3_wKC * Momentum +
//                     Loaded_D_Corr_pol3_wKC;
//
//                if (Printout_Corr_Variables) {
//                    cout << "\n";
//                    cout << "\n\nLoaded_A_Corr_pol3_wKC = " << Loaded_A_Corr_pol3_wKC << "\n";
//                    cout << "Loaded_B_Corr_pol3_wKC = " << Loaded_B_Corr_pol3_wKC << "\n";
//                    cout << "Loaded_C_Corr_pol3_wKC = " << Loaded_C_Corr_pol3_wKC << "\n";
//                    cout << "Loaded_D_Corr_pol3_wKC = " << Loaded_D_Corr_pol3_wKC << "\n\n";
//                }
//            }
//
//            CorrectionFactor = 1 / (1 - Mu);
//            CorrectedMomentum = Momentum * CorrectionFactor;
//
//            if (Printout) {
//                cout << "\n\nCorrMode = " << CorrMode << "\n";
//                cout << "Mu = " << Mu << "\n";
//                cout << "CorrectionFactor = " << CorrectionFactor << "\n";
//                cout << "Momentum = " << Momentum << "\n";
//                cout << "CorrectedMomentum = " << CorrectedMomentum << "\n\n";
//            }
//        } else {
//            cout << "\n\nNeutronResolution::NCorr: CorrMode illegal! Exiting...", exit(0);
//        }
//
//        return CorrectedMomentum;
//    }
//}
////</editor-fold>

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
                Mu = Loaded_A_Corr_pol3_wKC * Momentum3 + Loaded_B_Corr_pol3_wKC * Momentum2 + Loaded_C_Corr_pol3_wKC * Momentum +
                     Loaded_D_Corr_pol3_wKC;

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
