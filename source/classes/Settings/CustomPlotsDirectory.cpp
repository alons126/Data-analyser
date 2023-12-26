//
// Created by alons on 23/12/2023.
//

#include "CustomPlotsDirectory.h"

// ConfigureStatuses function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ConfigureStatuses">
void CustomPlotsDirectory::ConfigureStatuses(const bool &apply_cuts, const bool &only_preselection_cuts, const bool &apply_chi2_cuts_1e_cut,
                                             const bool &only_electron_quality_cuts, const bool &apply_nucleon_cuts, const bool &Enable_FD_photons,
                                             const bool &apply_nucleon_SmearAndShift, const bool &apply_kinematical_cuts, const bool &apply_kinematical_weights,
                                             const bool &apply_fiducial_cuts, const bool &Generate_AMaps, const bool &plot_and_fit_MomRes, const bool &VaryingDelta,
                                             const bool &Calculate_momResS2, const bool &Run_in_momResS2, const bool &nRes_test, const bool &Rec_wTL_ES,
                                             const bool &ZoomIn_On_mom_th_plots) {
    if (Custom_cuts_naming) {

        //<editor-fold desc="Status additions">
        if (only_preselection_cuts) {
            Preselection_Cuts_Status = "_Preselection_Cuts";
        } else if (apply_cuts) {
            Preselection_Cuts_Status = "";
        }

        if (only_electron_quality_cuts) {
            Electron_Quality_Cuts_Status = "_eQC";
        } else if (apply_cuts) {
            Electron_Quality_Cuts_Status = "";
        }

        if (apply_nucleon_cuts) {
            Nucleon_Cuts_Status = "NC_";
        } else {
            Nucleon_Cuts_Status = "noNC";
        }

        if (!apply_nucleon_cuts) {
            if (Enable_FD_photons) {
                FD_photons_Status = "wFDph";
            } else {
                FD_photons_Status = "";
            }
        } else {
            if (Enable_FD_photons) {
                FD_photons_Status = "wFDph_";
            } else {
                FD_photons_Status = "";
            }
        }

        if (!apply_nucleon_SmearAndShift) {
            PSmearing_Status = "";
        } else {
            PSmearing_Status = "wNSaS_";
        }

        if (!apply_kinematical_cuts && !apply_kinematical_weights) {
            KinCuts_Status = "";
            KinWei_Status = "";
        } else if (apply_kinematical_cuts && !apply_kinematical_weights) {
            KinCuts_Status = "wKC_";
            KinWei_Status = "";
        } else if (!apply_kinematical_cuts && apply_kinematical_weights) {
            KinCuts_Status = "";
            KinWei_Status = "wKW_";
        } else if (apply_kinematical_cuts && apply_kinematical_weights) {
            KinCuts_Status = "wKC";
            KinWei_Status = "wKW_";
        }

        if (!apply_fiducial_cuts) {
            FiducialCuts_Status = "";
        } else {
            FiducialCuts_Status = "wFC_";
        }

        if (apply_chi2_cuts_1e_cut) {
            if (!Generate_AMaps && !plot_and_fit_MomRes) {
                Additional_Status = "";
            } else if (Generate_AMaps && !plot_and_fit_MomRes) {
                Additional_Status = "AMaps_";
            } else if (!Generate_AMaps && plot_and_fit_MomRes) {
                if (!VaryingDelta) {
                    Additional_Status = "nResSS_";
                } else {
                    if (!Calculate_momResS2) {
                        if (!nRes_test) {
                            if (!Run_in_momResS2) {
                                Additional_Status = "nResS1_";
                            } else {
                                Additional_Status = "nResS2R_"; // R for Run
                            }
                        } else {
                            if (!Run_in_momResS2) {
                                Additional_Status = "nResS1T_";
                            } else {
                                Additional_Status = "nResS2RT_"; // R for Run; T for Test
                            }
                        }
                    } else {
                        if (!nRes_test) {
                            Additional_Status = "nResS2_";
                        } else {
                            Additional_Status = "nResS2T_"; // T for Test
                        }
                    }
                }
            } else if (Generate_AMaps && plot_and_fit_MomRes) {
                if (!VaryingDelta) {
                    Additional_Status = "nResSS_AMaps_";
                } else {
                    if (!Calculate_momResS2) {
                        if (!nRes_test) {
                            if (!Run_in_momResS2) {
                                Additional_Status = "nResS1_";
                            } else {
                                Additional_Status = "nResS2R_";
                            }
                        } else {
                            if (!Run_in_momResS2) {
                                Additional_Status = "nResS1T_";
                            } else {
                                Additional_Status = "nResS2RT_";
                            }
                        }
                    } else {
                        if (!nRes_test) {
                            Additional_Status = "nResS2_";
                        } else {
                            Additional_Status = "nResS2T_";
                        }
                    }
                }
            }
        } else {
            Additional_Status = "";
        }

        if (!apply_nucleon_cuts) {
            Efficiency_Status = "";
        } else {
            if (Rec_wTL_ES) {
                if (ZoomIn_On_mom_th_plots) {
                    Efficiency_Status = "Eff2_ZoomIn";
                } else {
                    Efficiency_Status = "Eff2";
                }
            } else {
                Efficiency_Status = "Eff1";
            }
        }
        //</editor-fold>

    }

    Added_PreStatuses = Preselection_Cuts_Status + Electron_Quality_Cuts_Status;

    Added_Statuses = Preselection_Cuts_Status + Electron_Quality_Cuts_Status +
                     Nucleon_Cuts_Status + FD_photons_Status + PSmearing_Status +
                     FiducialCuts_Status + KinCuts_Status + KinWei_Status +
                     Additional_Status + Efficiency_Status;
}
//</editor-fold>

// SetPaths function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetPaths function">
void CustomPlotsDirectory::SetPaths(const string &WorkingDirectory, const string &SampleName, const string &plots_path,
                                    const bool &apply_cuts, const bool &apply_chi2_cuts_1e_cut, const bool &apply_nucleon_cuts) {
    string run;

    if (!apply_cuts) { // Stage 0 - no cuts
        run = SampleName + Stage0_prefix + Added_PreStatuses + "";
        Plots_path = WorkingDirectory + run;
        Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
    } else {
        if (!apply_chi2_cuts_1e_cut) { // Stage 1 - with cuts except PID (chi2) cuts
            run = SampleName + Stage1_prefix + Added_PreStatuses + "";
            Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
        } else if (apply_chi2_cuts_1e_cut) {
            if (!apply_nucleon_cuts) { // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
                run = SampleName + Stage2_prefix + Added_Statuses + "";
                Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
            } else {                   // Stage 3 - other runs
                run = SampleName + Stage3_prefix + Added_Statuses + "";
                Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
            }
        }
    }
}
//</editor-fold>