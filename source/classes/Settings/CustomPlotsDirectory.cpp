//
// Created by alons on 23/12/2023.
//

#include "CustomPlotsDirectory.h"

// ConfigureStatuses function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ConfigureStatuses">
void CustomPlotsDirectory::ConfigureStatuses(const bool &apply_cuts, const bool &clas12ana_particles, const bool &only_preselection_cuts, const bool &apply_chi2_cuts_1e_cut,
                                             const bool &only_electron_quality_cuts, const bool &apply_nucleon_cuts, const bool &Enable_FD_photons,
                                             const bool &apply_nucleon_SmearAndCorr, const bool &apply_kinematical_cuts, const bool &apply_kinematical_weights,
                                             const bool &apply_fiducial_cuts, const bool &Generate_AMaps, const bool &plot_and_fit_MomRes, const bool &VaryingDelta,
                                             const bool &Calculate_momResS2, const bool &Run_in_momResS2, const bool &nRes_test, const bool &Rec_wTL_ES,
                                             const bool &ZoomIn_On_mom_th_plots) {
    if (Custom_cuts_naming) {

        //<editor-fold desc="Status additions">
        if (!clas12ana_particles) { Clas12ana_particles_status = "_no_clas12ana"; }

        if (only_preselection_cuts) { Preselection_Cuts_Status = "_Preselection_Cuts"; }

        if (only_electron_quality_cuts) { Electron_Quality_Cuts_Status = "_eQC"; }

        if (apply_nucleon_cuts) {
            Nucleon_Cuts_Status = "_NC";
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
                FD_photons_Status = "_wFDph";
            } else {
                FD_photons_Status = "";
            }
        }

        if (!apply_nucleon_cuts) {
            Efficiency_Status = "";
        } else {
            if (Rec_wTL_ES) {
                if (ZoomIn_On_mom_th_plots) {
                    Efficiency_Status = "_EffwZoomIn";
                } else {
                    Efficiency_Status = "_Eff";
                }
            }
        }

        if (!apply_nucleon_SmearAndCorr) {
            PSmearing_Status = "";
        } else {
            PSmearing_Status = "_wNSaC";
        }

        if (!apply_kinematical_cuts && !apply_kinematical_weights) {
            KinCuts_Status = "";
            KinWei_Status = "";
        } else if (apply_kinematical_cuts && !apply_kinematical_weights) {
            KinCuts_Status = "_wKC";
            KinWei_Status = "";
        } else if (!apply_kinematical_cuts && apply_kinematical_weights) {
            KinCuts_Status = "";
            KinWei_Status = "_wKW";
        } else if (apply_kinematical_cuts && apply_kinematical_weights) {
            KinCuts_Status = "wKC";
            KinWei_Status = "_wKW";
        }

        if (!apply_fiducial_cuts) {
            FiducialCuts_Status = "";
        } else {
            FiducialCuts_Status = "_wFC";
        }

        if (apply_chi2_cuts_1e_cut) {
            if (!Generate_AMaps && !plot_and_fit_MomRes) {
                Additional_Status = "";
            } else if (Generate_AMaps && !plot_and_fit_MomRes) {
                Additional_Status = "_AMaps";
            } else if (!Generate_AMaps && plot_and_fit_MomRes) {
                if (!VaryingDelta) {
                    Additional_Status = "_momResSS";
                } else {
                    if (!Calculate_momResS2) {
                        if (!nRes_test) {
                            if (!Run_in_momResS2) {
                                Additional_Status = "_momResS1";
                            } else {
                                Additional_Status = "_momResS2R"; // R for Run
                            }
                        } else {
                            if (!Run_in_momResS2) {
                                Additional_Status = "_momResS1T";
                            } else {
                                Additional_Status = "_momResS2RT"; // R for Run; T for Test
                            }
                        }
                    } else {
                        if (!nRes_test) {
                            Additional_Status = "_momResS2";
                        } else {
                            Additional_Status = "_momResS2T"; // T for Test
                        }
                    }
                }
            } else if (Generate_AMaps && plot_and_fit_MomRes) {
                if (!VaryingDelta) {
                    Additional_Status = "_momResSS_AMaps";
                } else {
                    if (!Calculate_momResS2) {
                        if (!nRes_test) {
                            if (!Run_in_momResS2) {
                                Additional_Status = "_momResS1";
                            } else {
                                Additional_Status = "_momResS2R";
                            }
                        } else {
                            if (!Run_in_momResS2) {
                                Additional_Status = "_momResS1T";
                            } else {
                                Additional_Status = "_momResS2RT";
                            }
                        }
                    } else {
                        if (!nRes_test) {
                            Additional_Status = "_momResS2";
                        } else {
                            Additional_Status = "_momResS2T";
                        }
                    }
                }
            }
        } else {
            Additional_Status = "";
        }
        //</editor-fold>

    }

    Added_PreStatuses = Clas12ana_particles_status + Preselection_Cuts_Status + Electron_Quality_Cuts_Status;

    Added_Statuses = Preselection_Cuts_Status + Electron_Quality_Cuts_Status + Nucleon_Cuts_Status + FD_photons_Status + Efficiency_Status +
                     PSmearing_Status + FiducialCuts_Status + KinCuts_Status + KinWei_Status + Additional_Status;
}
//</editor-fold>

// SetPaths function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetPaths function">
void CustomPlotsDirectory::SetPaths(const string &WorkingDirectory, const string &SampleName, const string &plots_path, const bool &apply_cuts,
                                    const bool &apply_chi2_cuts_1e_cut, const bool &apply_nucleon_cuts) {
    string run, Global_Run_Status_Local;

    if (Global_Run_Status == "") {
        Global_Run_Status_Local = Global_Run_Status;
    } else {
        Global_Run_Status_Local = "_" + Global_Run_Status;
    }

    if (!apply_cuts) { // Stage 0 - no cuts
        run = SampleName + Stage0_prefix + Added_PreStatuses + Global_Run_Status_Local;
        Plots_path = WorkingDirectory + run;
        Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
    } else {
        if (!apply_chi2_cuts_1e_cut) { // Stage 1 - with cuts except PID (chi2) cuts
            run = SampleName + Stage1_prefix + Added_PreStatuses + Global_Run_Status_Local;
            Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
        } else if (apply_chi2_cuts_1e_cut) {
            if (!apply_nucleon_cuts) { // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
                run = SampleName + Stage2_prefix + Added_Statuses + Global_Run_Status_Local;
                Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
            } else {                   // Stage 3 - other runs
                run = SampleName + Stage3_prefix + Added_Statuses + Global_Run_Status_Local;
                Plots_path = WorkingDirectory + run, Plots_log_save_Directory = Plots_path + "/" + "Run_log_" + run + ".txt";
            }
        }
    }
}
//</editor-fold>