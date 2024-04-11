//
// Created by alons on 11/08/2023.
//

#include "CLAS12DAnalyser.h"

//void CLAS12DAnalyser::InitSettings() {
//
//    //<editor-fold desc="Code setup">
//
//// ======================================================================================================================================================================
//// Input processing
//// ======================================================================================================================================================================
//
//    //<editor-fold desc="Input processing">
//    //</editor-fold>
//
//// ======================================================================================================================================================================
//// Event selection setup
//// ======================================================================================================================================================================
//
//    //<editor-fold desc="Event selection setup">
////    if (!calculate_2p) { calculate_pFDpCD = false; }
//    if (isData) { calculate_truth_level = false; }
//    if (!calculate_truth_level) { TL_with_one_reco_electron = fill_TL_plots = Rec_wTL_ES = false; }
//
////    if (!apply_chi2_cuts_1e_cut) { // for first run on new samples
////        Generate_AMaps = plot_and_fit_MomRes = true;
////        VaryingDelta = false;
////    }
//
////    if (Generate_AMaps) { Rec_wTL_ES = false; }
//    //</editor-fold>
//
//// ======================================================================================================================================================================
//// Cut setup
//// ======================================================================================================================================================================
//
//// Cut setup ---------------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Cuts setup">
//    /* Settings that allow to disable/enable every cut individually */
//
//    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Custom cuts naming & print out execution variables">
//
//    //<editor-fold desc="New samples setup">
////    if (!apply_chi2_cuts_1e_cut) { // for first run on new samples
////        Generate_AMaps = plot_and_fit_MomRes = true;
////        VaryingDelta = false;
////    }
////
////    if (Generate_AMaps) { Rec_wTL_ES = false; }
//    //</editor-fold>
//
//    //<editor-fold desc="Custom cuts naming">
//    /* Save plots to custom-named folders, to allow multi-sample runs at once. */
//
//    if (custom_cuts_naming) {
//
//        //<editor-fold desc="Status additions">
//        if (apply_nucleon_cuts) {
//            Nucleon_Cuts_Status = "wNC_";
//        } else {
//            Nucleon_Cuts_Status = "noNC";
//        }
//
//        if (!apply_nucleon_cuts) {
//            if (Enable_FD_photons) {
//                FD_photons_Status = "wFDph";
//            } else {
//                FD_photons_Status = "";
//            }
//        } else {
//            if (Enable_FD_photons) {
//                FD_photons_Status = "wFDph_";
//            } else {
//                FD_photons_Status = "";
//            }
//        }
//
//        if (!apply_nucleon_SmearAndCorr) {
//            PSmearing_Status = "";
//        } else {
//            PSmearing_Status = "wNSaS_";
//        }
//
//        if (!apply_kinematical_cuts && !apply_kinematical_weights) {
//            KinCuts_Status = "";
//            KinWei_Status = "";
//        } else if (apply_kinematical_cuts && !apply_kinematical_weights) {
//            KinCuts_Status = "wKC_";
//            KinWei_Status = "";
//        } else if (!apply_kinematical_cuts && apply_kinematical_weights) {
//            KinCuts_Status = "";
//            KinWei_Status = "wKW_";
//        } else if (apply_kinematical_cuts && apply_kinematical_weights) {
//            KinCuts_Status = "wKC";
//            KinWei_Status = "wKW_";
//        }
//
//        if (!apply_fiducial_cuts) {
//            FiducialCuts_Status = "";
//        } else {
//            FiducialCuts_Status = "wFC_";
//        }
//
//        if (apply_chi2_cuts_1e_cut) {
//            if (!Generate_AMaps && !plot_and_fit_MomRes) {
//                Additional_Status = "";
//            } else if (Generate_AMaps && !plot_and_fit_MomRes) {
//                Additional_Status = "AMaps_";
//            } else if (!Generate_AMaps && plot_and_fit_MomRes) {
//                if (!VaryingDelta) {
//                    Additional_Status = "nResSS_";
//                } else {
//                    Additional_Status = "nRes_";
//                }
//            } else if (Generate_AMaps && plot_and_fit_MomRes) {
//                if (!VaryingDelta) {
//                    Additional_Status = "nResSS_AMaps_";
//                } else {
//                    Additional_Status = "nRes_AMaps_";
//                }
//            }
//        } else {
//            Additional_Status = "";
//        }
//
//        if (!apply_nucleon_cuts) {
//            Efficiency_Status = "";
//        } else {
//            if (Rec_wTL_ES) {
//                Efficiency_Status = "Eff2";
//            } else {
//                Efficiency_Status = "Eff1";
//            }
//        }
//        //</editor-fold>
//
//        if (!apply_cuts) { // Stage 0 - no cuts
//            plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-00_NO_CUTS";
//            plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-00_NO_CUTS.txt";
//        } else {
//            string added_names = Nucleon_Cuts_Status + FD_photons_Status + PSmearing_Status + FiducialCuts_Status + KinCuts_Status + KinWei_Status + Additional_Status
//                                 + Efficiency_Status;
//
//            if (!apply_chi2_cuts_1e_cut) { // Stage 1 - with cuts except PID (chi2) cuts
//                plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-01_AC_woChi2";
//                plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-01_AC_woChi2.txt";
//            } else if (apply_chi2_cuts_1e_cut) {
//                if (!apply_nucleon_cuts) { // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
//                    plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-02_AC_" + added_names;
//                    plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-02_AC_" + added_names + ".txt";
//                } else {
//                    plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-03_AC_" + added_names;
//                    plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-03_AC_" + added_names + ".txt";
//                }
//            }
//        }
//    } else {
//        Nucleon_Cuts_Status = FD_photons_Status = Efficiency_Status = "";
//    }
//    //</editor-fold>
//
//    //<editor-fold desc="Print out execution variables">
//    /* Print out execution variables (for self observation) */
//    cout << "-- Execution variables ----------------------------------------------------\n";
//    cout << "WorkingDirectory:\t" << WorkingDirectory << "\n";
//    cout << "plots_path:\t\t" << plots_path << "\n\n";
//
//    cout << "AnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n";
//    cout << "AnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n";
//    cout << "AnalyseFile:\t\t" << AnalyseFile << "\n";
//    cout << "Settings mode:\t\t'" << file_name << "'\n\n";
//
//    cout << "SampleName:\t\t" << SampleName << "\n";
//    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
//    cout << "Beam Energy:\t\t" << beamE << " [GeV]\n\n\n\n";
//    //</editor-fold>
//
//    //<editor-fold desc="Cuts output">
//    /* Print out the cuts within the run (for self-observation) */
//    if (!apply_chi2_cuts_1e_cut) { apply_nucleon_cuts = false; }
//
//    if (!apply_nucleon_cuts) { apply_nucleon_physical_cuts = false; }
//
//    if (!apply_nucleon_physical_cuts) { apply_nBeta_fit_cuts = apply_fiducial_cuts = apply_kinematical_cuts = apply_kinematical_weights = apply_nucleon_SmearAndCorr = false; }
//
//    if (Generate_AMaps) { apply_fiducial_cuts = false; }
//
//    if (!apply_cuts) {
//        cout << "Cuts are disabled:\n";
//
//        apply_Nphe_cut = apply_chi2_cuts_1e_cut = apply_Vz_cuts = apply_dVz_cuts = false;
//        apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_DC_fiducial_cuts = false;
//    } else {
//        cout << "Cuts are enabled:\n";
//    }
//
//    cout << "apply_Nphe_cut:\t\t\t" << BoolToString(apply_Nphe_cut) << "\n";
//    cout << "apply_chi2_cuts_1e_cut:\t\t" << BoolToString(apply_chi2_cuts_1e_cut) << "\n";
//    cout << "apply_Vz_cuts:\t\t\t" << BoolToString(apply_Vz_cuts) << "\n";
//    cout << "apply_dVz_cuts:\t\t\t" << BoolToString(apply_dVz_cuts) << "\n";
//    cout << "apply_ECAL_SF_cuts:\t\t" << BoolToString(apply_ECAL_SF_cuts) << "\n";
//    cout << "apply_ECAL_P_cuts:\t\t" << BoolToString(apply_ECAL_P_cuts) << "\n";
//    cout << "apply_ECAL_fiducial_cuts:\t" << BoolToString(apply_ECAL_fiducial_cuts) << "\n";
//    cout << "apply_DC_fiducial_cuts:\t\t" << BoolToString(apply_DC_fiducial_cuts) << "\n";
//    cout << "apply_nucleon_cuts:\t\t" << BoolToString(apply_nucleon_cuts) << "\n";
//    cout << "apply_nucleon_physical_cuts:\t" << BoolToString(apply_nucleon_physical_cuts) << "\n";
//    cout << "apply_nBeta_fit_cuts:\t\t" << BoolToString(apply_nBeta_fit_cuts) << "\n";
//    cout << "apply_fiducial_cuts:\t\t" << BoolToString(apply_fiducial_cuts) << "\n";
//    cout << "apply_kinematical_cuts:\t\t" << BoolToString(apply_kinematical_cuts) << "\n";
//    cout << "apply_kinematical_weights:\t" << BoolToString(apply_kinematical_weights) << "\n";
//    cout << "apply_nucleon_SmearAndCorr:\t" << BoolToString(apply_nucleon_SmearAndCorr) << "\n\n";
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //</editor-fold>
//
//// Cut declarations -----------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Cuts declarations">
//    /* Log cut values to be used later when applying them. */
//
//    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="clas12ana cuts">
//    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
//
//    /* Chi2 cuts. NOTES:
//     * Values for mean and sigma are filled from fit variables (overating these values later).
//     * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */
//
//    //<editor-fold desc="Kaon PID cuts">
//    /* Kaon pid cuts. Not really applied in our analysis. */
//    //</editor-fold>
//
//    /* Vertex cuts */
//
//    if (SampleName == "LH2_data_6GeV_run_015032") {
//        Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
//    } else {
//        //TODO: these are good for Ca48 simulation and data. Check cuts for other targets!
//        Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 5);
//    }
//
//    /* Sampling Fraction (SF) cuts (electrons only, FD) */
////    DSCuts P_cuts;
//
//    /* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
//
//    /* DC edge cuts (fiducial cuts ,electrons only, FD) */
//
//    /* Momentum thresholds */
//
//    //<editor-fold desc="Momentum thresholds definition">
////    /* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
////    e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
////    p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.4, 9999);
////    n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.4, 9999);
////    pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
////    pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
////    ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
//    if (limless_mom_eff_plots) {
//        /* If we enforce TL cuts, don't use momentum thresholds on nucleons. */
//        e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
//        p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
//        n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, -9999, 9999);
//        pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
//        pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
//        ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
//    } else {
//        /* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
//        e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
//        p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.4, 9999);
//        n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.4, 9999);
//        pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
//        pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
//        ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
//    }
//    //</editor-fold>
//
//    //</editor-fold>
//
//    // Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Other cuts">
//    /* Neutron beta cuts (1n & nFDpCD, FD only) */
//
//    /* Neutron momentum cuts (1n & nFDpCD, FD only) */
//
//    /* Truth-level momentum cuts */
//
//    /* FD & CD theta range */
//
//    /* Beta cut (1n, FD) */
//
//    /* Neutron veto cut (1n & nFDpCD, FD) */
//
//    /* Ghost tracks handling (2p & pFDpCD, CD & FD) */
//
//    /* reco. kinematic cuts (based on nucleons' efficiency) */
//    //</editor-fold>
//
//    //</editor-fold>
//
//// ======================================================================================================================================================================
//// Other setup
//// ======================================================================================================================================================================
//
//// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="TList definition">
//    /* Definition of plots TLists used to save all plots to .root file. */
//
//    /* General plots TList */
//    //</editor-fold>
//
////  Creating directories ------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Creating directories">
//    /* Code for creating directories.
//     * Directory creation is done in the Directories class.
//     * Added for the case that plots out folder does not exist and for organization.
//     * All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */
//    cout << "\nCreating plot directories...";
//
//     directories = Directories(Plots_Folder);
//
//    cout << " done.\n\n";
//    //</editor-fold>
//
//// Plot setup --------------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Plot selector">
//    /* Here are boolean variables used to turn ON/OFF the different plots of the code.
//       Plot_selector_master must remain true, set it OFF only for debugging. */
//
//    //<editor-fold desc="Plot selector - plot all">
//    /* Master plots variable */
//
//    /* Cut variable plots */
//
//    /* Beta plots */
//
//    /* Beta plots */
//
//    /* Angle plots */
//
//    /* Momentum transfer plots */
//
//    /* E_e plots */
//
//    /* ET plots */
//
//    /* Ecal plots */
//
//    /* Transverse variables plots */
//
//    /* ToF plots */
//
//    /* Efficiency plots */
//
//    /* Resolution plots */
//
//    /* Resolution plots */
//
//    /* Final state ratio plots */
//    //</editor-fold>
//
////    //<editor-fold desc="Plot selector - selected plots">
////    /* Master plots variable */
////    bool Plot_selector_master = true; // Master plot selector for analysis
////
////    /* Cut variable plots */
////    bool Cut_plots_master = true; // Master cut plots selector
//////    bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true;
////    bool Nphe_plots = false, Chi2_plots = false, Vertex_plots = false, SF_plots = false, fiducial_plots = false;
//////
//////    bool Momentum_plots = false;
////    bool Momentum_plots = true;
//////
////
////    /* Beta plots */
//////    bool W_plots = true;
////    bool W_plots = false;
////
////    /* Beta plots */
//////    bool Beta_plots = true;
////    bool Beta_plots = false;
//////    bool Beta_vs_P_plots = true;
////    bool Beta_vs_P_plots = false;
////
////    /* Angle plots */
////    bool Angle_plots_master = true; // Master angle plots selector
////    bool Theta_e_plots = true, Phi_e_plots = true;
//////    bool Angle_plots_master = false; // Master angle plots selector
//////    bool Theta_e_plots = false, Phi_e_plots = false;
////
////    /* Momentum transfer plots */
//////    bool Momentum_transfer_plots = true;
////    bool Momentum_transfer_plots = false;
////
////    /* E_e plots */
//////    bool E_e_plots = true;
////    bool E_e_plots = false;
////
////    /* ET plots */
//////    bool ETrans_plots_master = true; // Master ET plots selector
////    bool ETrans_plots_master = false; // Master ET plots selector
////    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;
////
////    /* Ecal plots */
//////    bool Ecal_plots = true;
////    bool Ecal_plots = false;
////
////    /* Transverse variables plots */
//////    bool TKI_plots = true;
////    bool TKI_plots = false;
////
////    /* ToF plots */
//////    bool ToF_plots = true;
////    bool ToF_plots = false;
////
////    /* Efficiency plots */
//////    bool Efficiency_plots = true;
////    bool Efficiency_plots = false;
//////    bool TL_after_Acceptance_Maps_plots = true;
////    bool TL_after_Acceptance_Maps_plots = false;
////
////    /* Resolution plots */
//////    bool Hit_maps_plots = true;
////    bool Hit_maps_plots = false;
////
////    /* Resolution plots */
//////    bool Resolution_plots = true;
////    bool Resolution_plots = false;
////
////    /* Final state ratio plots */
//////    bool FSR_plots = true;
////    bool FSR_plots = false;
////    //</editor-fold>/
//
//    //<editor-fold desc="Turn off plots by master selectors">
//    if (!Plot_selector_master) {
//        Cut_plots_master = W_plots = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Momentum_transfer_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
//        TKI_plots = ToF_plots = Efficiency_plots = Hit_maps_plots = Resolution_plots = false;
//    }
//
//    if (!Cut_plots_master) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }
//    if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }
//    if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }
//    if (!fill_TL_plots) { Efficiency_plots = TL_after_Acceptance_Maps_plots = false; }
//    if (!Generate_AMaps) { Hit_maps_plots = false; }
//    if (!plot_and_fit_MomRes) { Resolution_plots = false; }
//    //</editor-fold>
//
//    /* Other setup variables */
//    //</editor-fold>
//
//// Normalization setup -----------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Normalization setup">
//    /* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
//     * Enable of presentations only, since event count is important otherwise. */
//    if (!normalize_master) { // Disable all normalizations if normalize_master == false
//        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = norm_Momentum_plots = false;
//        norm_Angle_plots_master = norm_Momentum_transfer_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TKI_plots = false;
//    }
//    //</editor-fold>
//
//// Delete setup ------------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Delete setup">
//    /* Clear files from previous runs (to prevent mix fo plots from different codes). */
//
//    /* Delete existing .txt files */
//    if (delete_txt_files) { system(("find " + plots_path + " -type f -iname '*.txt' -delete").c_str()); }
//
//    //<editor-fold desc="Deleting files by cases">
//    if (delete_png_files && !delete_root_files) {
//        cout << "\nClearing old plots...";
//        system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
//        cout << " done.\n\n";
//    } else if (!delete_png_files && delete_root_files) {
//        cout << "\nClearing old root files...";
//        system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
//        cout << " done.\n\n";
//    } else if (delete_png_files && delete_root_files) {
//        cout << "\nClearing old plots & root files...";
//        system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
//        system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
//        cout << " done.\n\n";
//    } else {
//        cout << "\nNo files were cleared.\n\n";
//    }
//    //</editor-fold>
//
//    //</editor-fold>
//
//// Histogram setup ---------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Histogram setup">
//    /* Histogram boundary variables. Used to unify histograms to the same boundaries. */
//
//    //<editor-fold desc="Number of histogram bins">
//    /* Default */
//
//    /* Beta plots */
//
//    /* W plots */
//
//    /* Angle plots */
//
//    /* Momentum transfer plots */
//
//    /* E_e plots */
//
//    /* ET plots */
//
//    /* E_cal plots */
//
//    /* TKI plots */
//
//    /* Efficiency % acceptance correction plots */
//
//    /* Acceptance maps plots */
//
//    /* Neutron resolution plots */
//    //</editor-fold>
//
//    //<editor-fold desc="Histogram limits">
//    /* Nphe boundries */
//
//    /* Chi2 boundries */
//    if (apply_cuts) { Chi2_boundary = 9; }
//
//    /* Vertex boundries */
//    if (apply_cuts) {
//        double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
//        Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
//        Vertex_boundary = Vertex_boundary / 5;
//    }
//
//    if (apply_cuts) { /* dV_boundary = 7.5; */ dV_boundary = dVz_cuts.GetUpperCut() * 1.4; }
//
//    /* SF boundries */
//
//    /* Momentum boundries */
//
//    /* W boundries */
//
//    /* Beta boundries */
//    if (apply_cuts) { Beta_boundary = 1.25, P_boundary = beamE * 1.1; }
//
//    /* Angle boundries */
//
//    /* TKI boundries */
//    //</editor-fold>
//
//    //</editor-fold>
//
//// Acceptance maps generation -------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Acceptance maps generation">
//    /* Acceptance maps are handled completely by the AMaps class */
//    cout << "\nSetting Acceptance maps...";
//
//    if (!calculate_truth_level) { Generate_AMaps = false; }
//    if (!Generate_AMaps) { Hit_maps_plots = false; }
//    if (reformat_e_bins) { varying_P_e_bins = false; }
//
//    /* Set Bins by case */
//    if (Generate_AMaps) {
//        aMaps = AMaps(reformat_e_bins, varying_P_e_bins, beamE, directories.AMaps_Directory_map["AMaps_1e_cut_Directory"],
//                      NumberNucOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);
//        wMaps = AMaps(reformat_e_bins, varying_P_e_bins, beamE, directories.AMaps_Directory_map["WMaps_1e_cut_Directory"],
//                      NumberNucOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);
//    } else {
//        aMaps = AMaps(AcceptanceMapsDirectory, SampleName);
//        wMaps = AMaps(AcceptanceWeightsDirectory, SampleName);
//    }
//
//    cout << " done.\n\n";
//    //</editor-fold>
//
//// Neutron resolution & proton smearing ---------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Neutron resolution">
//    /* Neutron resolution fits is handled completely by the MomentumResolution class */
//    cout << "\nSetting neutron resolution data...";
//
//    if (!calculate_truth_level) { plot_and_fit_MomRes = false; } // Disable resolution-realted operations if not calculating TL plots
//    if (apply_nucleon_SmearAndCorr) { plot_and_fit_MomRes = false; }  // Disable resolution-realted operations when applying proton smearing
//
//    //<editor-fold desc="Neutron resolution class declaration & definition">
//    MomentumResolution nRes, pRes;
//
//    if (plot_and_fit_MomRes) {
//        nRes = MomentumResolution(SampleName, NucleonCutsDirectory, "Neutron", beamE, n_mom_th.GetLowerCut(),
//                                 directories.Resolution_Directory_map["nRes_plots_1n_Directory"], DeltaSlices, VaryingDelta);
//        pRes = MomentumResolution(SampleName, NucleonCutsDirectory, "Proton", beamE, p_mom_th.GetLowerCut(),
//                                 directories.Resolution_Directory_map["pRes_plots_1p_Directory"], DeltaSlices, VaryingDelta);
//    } else {
//        nRes.ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_res_fit_param_-_" + SampleName + ".par").c_str(),
//                              SampleName, NucleonCutsDirectory);
//        nRes.ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_res_hist_param_-_" + SampleName + ".par").c_str(),
//                              SampleName, NucleonCutsDirectory);
//    }
//    //</editor-fold>
//
//    cout << " done.\n\n";
//    //</editor-fold>
//
//// Acceptance correction data -------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Acceptance correction">
//    /* Neutron resolution fits is handled completely by the MomentumResolution class */
//    cout << "\nSetting acceptance correction data...";
//
//    if (!calculate_truth_level) { save_ACorr_data = false; }
//
//    cout << " done.\n\n";
//    //</editor-fold>
//
//// Debugging setup ---------------------------------------------------------------------------------------------------------------------------------------------------
//
//    //<editor-fold desc="Debugging setup">
//    /* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */
//    SetGPMonitoringPlots(GoodProtonsMonitorPlots, directories.Angle_Directory_map["CToF_hits_monitoring_2p_Directory"],
//                         directories.Angle_Directory_map["Double_detection_monitoring_2p_Directory"]);
//    if (PrintEvents) {
//        if (!apply_chi2_cuts_1e_cut) {
//            EventPrint_save_Directory = plots_path + "/" + "Event_Print_without_chi2.txt";
//        } else if (apply_chi2_cuts_1e_cut) {
//            EventPrint_save_Directory = plots_path + "/" + "Event_Print_ALL_CUTS.txt";
//        }
//
//        EventPrint.open(EventPrint_save_Directory.c_str());
//
//        if (!apply_chi2_cuts_1e_cut) {
//            EventPrint << "//////////////////////////////////////////////////////////////////////\n";
//            EventPrint << "// Log of number of particles in event with all cuts except chi2    //\n";
//            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
//        } else if (apply_chi2_cuts_1e_cut) {
//            EventPrint << "//////////////////////////////////////////////////////////////////////\n";
//            EventPrint << "// Log of number of particles in event with all cuts including chi2 //\n";
//            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
//        }
//
//        EventPrint << "EVENT SELECTION:\n";
//        EventPrint << "#electrons in event:\t\t" << Ne_in_event << "\n";
//        EventPrint << "minimal #hadrons in event:\t" << Nf_in_event << "\n";
//        EventPrint << "#event upper bound:\t\t" << nEvents2print << "\n\n\n";
//    }
//    //</editor-fold>
//
//    //</editor-fold>
//
//}
