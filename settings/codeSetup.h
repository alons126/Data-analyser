
#ifndef ANALYSER_CODESETUP_H
#define ANALYSER_CODESETUP_H

#include "../source/constants.h"
#include "../source/classes/GeneralClasses.h"
#include "../source/classes/Ana/clas12ana.h"
#include "../source/cuts/DetectorSimulationCuts.h"
#include "../source/functions/GeneralFunctions.h"
#include "../source/functions/HistogramPlottingFunctions.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code version                                                                                //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

std::string Ver = "DetSim testings";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                    File & path definitions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ======================================================================================================================================================================
// Path definitions
// ======================================================================================================================================================================

//<editor-fold desc="path definitions">
/* Histogram plots output directory */
std::string WorkingDirectory = GetCurrentDirectory() + "/";
//std::string plots_path = WorkingDirectory + "plots_t5tot_mycuts" + "/";
//std::string plots_path = WorkingDirectory + "plots_t5tot_mycuts_false" + "/";
//std::string plots_path = WorkingDirectory + "plots_t5tot_mycuts_false_ana" + "/";
std::string plots_path = WorkingDirectory + "plots" + "/";
//std::string plots_path = "./plots/";

/* settings directory and directories */
std::string SettingsDirectory = WorkingDirectory + "settings" + "/";

/* source directory and directories */
std::string SourcesDirectory = WorkingDirectory + "source" + "/";
std::string CutsDirectory = SourcesDirectory + "cuts" + "/";

std::string plots_file_type = "_plots.root";
std::string plots_log_save_Directory = plots_path + "/" + "Run_log.txt";
//</editor-fold>

// ======================================================================================================================================================================
// file_name definitions and selection
// ======================================================================================================================================================================

// file_name definition -------------------------------------------------------------------------------------------------------------------------------------------------

std::string file_name = "general_file";

// hipo files -----------------------------------------------------------------------------------------------------------------------------------------------------------

// Local files:
std::string AnalyseFilePath = "mnt/d/e4nu/hipo_data_files";
////std::string AnalyseFileSample = "recon_c12_6gev"; // Justin's ~1M
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_1_first_10"; // my test with Justin's code; no pion data saved (e,p,n only)
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_1";          // my test with Justin's code; no pion data saved (e,p,n only)
//std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10"; // my test with Justin's code; all particle data saved
std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5";            // my test with Justin's code; all particle data saved

std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
std::string AnalyseFile = AnalyseFileDir + "*.hipo";

//// ifarm files (Justin's MC files):
//std::string AnalyseFilePath = "volatile/clas12/users/esteejus/Simulation_sigmaCM";
//std::string AnalyseFileSample = "reconhipo"; // Justin's MC files
//std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
//std::string AnalyseFile = AnalyseFileDir + "recon_qe_gcf_c_5.98gev_sigmacm_200_1*.hipo";

//// root files -----------------------------------------------------------------------------------------------------------------------------------------------------------
////
////std::string AnalyseFile = "./e_on_1000060120_598636MeV.gst.root";
////
////std::string AnalyseFilePath = "mnt/d/e4nu/hipo_data_files";
////std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
////std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
////
//////std::string AnalyseFileSample = "recon_qe_GENIE_C_598636MeV_Q2_0_4_test_1";
//////std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/" + "first_50_test" + "/";
////
//////std::string AnalyseFileDir = "/home/alon/project/temp/";
//
//std::string AnalyseFilePath = "mnt/d/e4nu/root_data_files/Detector_sim_truth_level_files";
//std::string AnalyseFileSample = "598636MeV_Q2_0_5";
//std::string AnalyseFileDir = "/" + AnalyseFilePath + "/" + AnalyseFileSample + "/";
//
////std::string AnalyseFile = AnalyseFileDir + "e_on_1000060120_598636MeV_Q2_0_5.gst.root";
////std::string AnalyseFile = "./nu_SuSAv2_fix_test_12C_2222GeV_CC_myBranch_10M.root";
//std::string AnalyseFile = "./nu_SuSAv2_fix_test_12C_2222GeV_CCMEC_myBranch_10M.root";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                    Other parameters                                                                                 //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//<editor-fold desc="Other parameters">

// ======================================================================================================================================================================
// BEnergyToNucleus definition
// ======================================================================================================================================================================

double BEnergyToNucleus;

// ======================================================================================================================================================================
// Simulation parameters extraction
// ======================================================================================================================================================================

int Target_pdg, Probe_pdg;
std::string Target_nucleus, Probe;
double BeamEnergy;

// ======================================================================================================================================================================
// Histogram ranges
// ======================================================================================================================================================================

//<editor-fold desc="Histogram range variables">

//<editor-fold desc="Chi2 plots">
double Chi2_upper_lim, Chi2_lower_lim;
//</editor-fold>

//<editor-fold desc="Vertex plots">
double Vertex_upper_lim, Vertex_lower_lim, dV_upper_lim, dV_lower_lim;
//</editor-fold>

//<editor-fold desc="Theta histograms">

//<editor-fold desc="Theta histograms (2p)">
double theta_lp_upper_lim_2p, theta_lp_lower_lim_2p, theta_p1_upper_lim_2p, theta_p1_lower_lim_2p, theta_p2_upper_lim_2p, theta_p2_lower_lim_2p;
double dtheta_upper_lim_2p, dtheta_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Theta histograms (1n1p)">
double theta_lp_upper_lim_1n1p, theta_lp_lower_lim_1n1p, theta_p_upper_lim_1n1p, theta_p_lower_lim_1n1p, theta_n_upper_lim_1n1p, theta_n_lower_lim_1n1p;
double dtheta_upper_lim_1n1p, dtheta_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Phi histograms">

//<editor-fold desc="Phi histograms (2p)">
double phi_lp_upper_lim_2p, phi_lp_lower_lim_2p, phi_p1_upper_lim_2p, phi_p1_lower_lim_2p, phi_p2_upper_lim_2p, phi_p2_lower_lim_2p;
double dphi_upper_lim_2p, dphi_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Phi histograms (1n1p)">
double phi_lp_upper_lim_1n1p, phi_lp_lower_lim_1n1p, phi_p_upper_lim_1n1p, phi_p_lower_lim_1n1p, phi_n_upper_lim_1n1p, phi_n_lower_lim_1n1p;
double dphi_upper_lim_1n1p, dphi_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy histograms">

//<editor-fold desc="Energy histograms (2p)">
double fsEl_upper_lim_2p, fsEl_lower_lim_2p;
double fsEl_QEL_upper_lim_2p, fsEl_QEL_lower_lim_2p, fsEl_MEC_upper_lim_2p, fsEl_MEC_lower_lim_2p;
double fsEl_RES_upper_lim_2p, fsEl_RES_lower_lim_2p, fsEl_DIS_upper_lim_2p, fsEl_DIS_lower_lim_2p;
double fsEl_VS_theta_lp_upper_lim_2p_x, fsEl_VS_theta_lp_lower_lim_2p_x, fsEl_VS_theta_lp_upper_lim_2p_y, fsEl_VS_theta_lp_lower_lim_2p_y;
//</editor-fold>

//<editor-fold desc="Energy histograms (1n1p)">
double fsEl_upper_lim_1n1p, fsEl_lower_lim_1n1p;
double fsEl_QEL_upper_lim_1n1p, fsEl_QEL_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p, fsEl_MEC_lower_lim_1n1p;
double fsEl_RES_upper_lim_1n1p, fsEl_RES_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p, fsEl_DIS_lower_lim_1n1p;
double fsEl_VS_theta_lp_upper_lim_1n1p_x, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_y, fsEl_VS_theta_lp_lower_lim_1n1p_y;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms">

//<editor-fold desc="Energy Transfer histograms (all interactions)">

//<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
double E_Trans_all_ang_all_int_upper_lim_2p, E_Trans_all_ang_all_int_lower_lim_2p;
double E_Trans15_all_upper_lim_2p, E_Trans15_all_lower_lim_2p, E_Trans45_all_upper_lim_2p, E_Trans45_all_lower_lim_2p, E_Trans90_all_upper_lim_2p, E_Trans90_all_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
double E_Trans_all_ang_all_int_upper_lim_1n1p, E_Trans_all_ang_all_int_lower_lim_1n1p;
double E_Trans15_all_upper_lim_1n1p, E_Trans15_all_lower_lim_1n1p, E_Trans45_all_upper_lim_1n1p, E_Trans45_all_lower_lim_1n1p, E_Trans90_all_upper_lim_1n1p, E_Trans90_all_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only)">

//<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
double E_Trans15_QEL_upper_lim_2p, E_Trans15_QEL_lower_lim_2p, E_Trans45_QEL_upper_lim_2p, E_Trans45_QEL_lower_lim_2p, E_Trans90_QEL_upper_lim_2p, E_Trans90_QEL_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
double E_Trans15_QEL_upper_lim_1n1p, E_Trans15_QEL_lower_lim_1n1p, E_Trans45_QEL_upper_lim_1n1p, E_Trans45_QEL_lower_lim_1n1p, E_Trans90_QEL_upper_lim_1n1p, E_Trans90_QEL_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only)">

//<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
double E_Trans15_MEC_upper_lim_2p, E_Trans15_MEC_lower_lim_2p, E_Trans45_MEC_upper_lim_2p, E_Trans45_MEC_lower_lim_2p, E_Trans90_MEC_upper_lim_2p, E_Trans90_MEC_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
double E_Trans15_MEC_upper_lim_1n1p, E_Trans15_MEC_lower_lim_1n1p, E_Trans45_MEC_upper_lim_1n1p, E_Trans45_MEC_lower_lim_1n1p, E_Trans90_MEC_upper_lim_1n1p, E_Trans90_MEC_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only)">

//<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
double E_Trans15_RES_upper_lim_2p, E_Trans15_RES_lower_lim_2p, E_Trans45_RES_upper_lim_2p, E_Trans45_RES_lower_lim_2p, E_Trans90_RES_upper_lim_2p, E_Trans90_RES_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
double E_Trans15_RES_upper_lim_1n1p, E_Trans15_RES_lower_lim_1n1p, E_Trans45_RES_upper_lim_1n1p, E_Trans45_RES_lower_lim_1n1p, E_Trans90_RES_upper_lim_1n1p, E_Trans90_RES_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only)">

//<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
double E_Trans15_DIS_upper_lim_2p, E_Trans15_DIS_lower_lim_2p, E_Trans45_DIS_upper_lim_2p, E_Trans45_DIS_lower_lim_2p, E_Trans90_DIS_upper_lim_2p, E_Trans90_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
double E_Trans15_DIS_upper_lim_1n1p, E_Trans15_DIS_lower_lim_1n1p, E_Trans45_DIS_upper_lim_1n1p, E_Trans45_DIS_lower_lim_1n1p, E_Trans90_DIS_upper_lim_1n1p, E_Trans90_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//<editor-fold desc="Inclusive Energy Transfer histograms">
double E_Trans15_all_inclusive_upper_lim, E_Trans15_all_inclusive_lower_lim;
double E_Trans15_QEL_inclusive_upper_lim, E_Trans15_QEL_inclusive_lower_lim, E_Trans15_MEC_inclusive_upper_lim, E_Trans15_MEC_inclusive_lower_lim;
double E_Trans15_RES_inclusive_upper_lim, E_Trans15_RES_inclusive_lower_lim, E_Trans15_DIS_inclusive_upper_lim, E_Trans15_DIS_inclusive_lower_lim;
double E_Trans_VS_q_all_inclusive_upper_lim_x, E_Trans_VS_q_all_inclusive_lower_lim_x, E_Trans_VS_q_all_inclusive_upper_lim_y, E_Trans_VS_q_all_inclusive_lower_lim_y;
double E_Trans_VS_q_QEL_inclusive_upper_lim_x, E_Trans_VS_q_QEL_inclusive_lower_lim_x, E_Trans_VS_q_QEL_inclusive_upper_lim_y, E_Trans_VS_q_QEL_inclusive_lower_lim_y;
double E_Trans_VS_q_MEC_inclusive_upper_lim_x, E_Trans_VS_q_MEC_inclusive_lower_lim_x, E_Trans_VS_q_MEC_inclusive_upper_lim_y, E_Trans_VS_q_MEC_inclusive_lower_lim_y;

double E_Trans_VS_q3_all_upper_lim_x_2p, E_Trans_VS_q3_all_lower_lim_x_2p, E_Trans_VS_q3_all_upper_lim_y_2p, E_Trans_VS_q3_all_lower_lim_y_2p;
double E_Trans_VS_q3_QEL_upper_lim_x_2p, E_Trans_VS_q3_QEL_lower_lim_x_2p, E_Trans_VS_q3_QEL_upper_lim_y_2p, E_Trans_VS_q3_QEL_lower_lim_y_2p;
double E_Trans_VS_q3_MEC_upper_lim_x_2p, E_Trans_VS_q3_MEC_lower_lim_x_2p, E_Trans_VS_q3_MEC_upper_lim_y_2p, E_Trans_VS_q3_MEC_lower_lim_y_2p;

double E_Trans_VS_q3_all_upper_lim_x_1n1p, E_Trans_VS_q3_all_lower_lim_x_1n1p, E_Trans_VS_q3_all_upper_lim_y_1n1p, E_Trans_VS_q3_all_lower_lim_y_1n1p;
double E_Trans_VS_q3_QEL_upper_lim_x_1n1p, E_Trans_VS_q3_QEL_lower_lim_x_1n1p, E_Trans_VS_q3_QEL_upper_lim_y_1n1p, E_Trans_VS_q3_QEL_lower_lim_y_1n1p;
double E_Trans_VS_q3_MEC_upper_lim_x_1n1p, E_Trans_VS_q3_MEC_lower_lim_x_1n1p, E_Trans_VS_q3_MEC_upper_lim_y_1n1p, E_Trans_VS_q3_MEC_lower_lim_y_1n1p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms">

//<editor-fold desc="E_cal range">
double E_cal_QEL_upper_lim_range, E_cal_QEL_lower_lim_range, E_cal_MEC_upper_lim_range, E_cal_MEC_lower_lim_range;
double E_cal_RES_upper_lim_range, E_cal_RES_lower_lim_range, E_cal_DIS_upper_lim_range, E_cal_DIS_lower_lim_range;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (2p)">
double E_cal_QEL_upper_lim_2p, E_cal_QEL_lower_lim_2p, E_cal_MEC_upper_lim_2p, E_cal_MEC_lower_lim_2p;
double E_cal_RES_upper_lim_2p, E_cal_RES_lower_lim_2p, E_cal_DIS_upper_lim_2p, E_cal_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (1n1p)">
double E_cal_QEL_upper_lim_1n1p, E_cal_QEL_lower_lim_1n1p, E_cal_MEC_upper_lim_1n1p, E_cal_MEC_lower_lim_1n1p;
double E_cal_RES_upper_lim_1n1p, E_cal_RES_lower_lim_1n1p, E_cal_DIS_upper_lim_1n1p, E_cal_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Momentum histograms">

//<editor-fold desc="Momentum histograms (2p)">
double P_L_hist_upper_lim_2p, P_L_hist_lower_lim_2p, P_R_hist_upper_lim_2p, P_R_hist_lower_lim_2p, P_lp_hist_upper_lim_2p, P_lp_hist_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Momentum histograms (1n1p)">
double P_p_hist_upper_lim_1n1p, P_p_hist_lower_lim_1n1p, P_n_hist_upper_lim_1n1p, P_n_hist_lower_lim_1n1p, P_lp_hist_upper_lim_1n1p, P_lp_hist_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="MicroBooNE plots">

//<editor-fold desc="MicroBooNE gamma plots (unweighted)">
double gamma_Lab_hist_upper_lim_2p, gamma_Lab_hist_lower_lim_2p, gamma_mu_p_tot_upper_lim_2p, gamma_mu_p_tot_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
double gamma_Lab_hist_weighted_upper_lim_1n1p, gamma_Lab_hist_weighted_lower_lim_1n1p, gamma_mu_p_tot_weighted_upper_lim_1n1p, gamma_mu_p_tot_weighted_lower_lim_1n1p;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (no pions, for every interaction)">
double gamma_Lab_all_hist_upper_lim, gamma_Lab_all_hist_lower_lim, gamma_Lab_all_hist_weighted_upper_lim, gamma_Lab_all_hist_weighted_lower_lim;
double gamma_Lab_QEL_hist_upper_lim, gamma_Lab_QEL_hist_lower_lim, gamma_Lab_QEL_hist_weighted_upper_lim, gamma_Lab_QEL_hist_weighted_lower_lim;
double gamma_Lab_MEC_hist_upper_lim, gamma_Lab_MEC_hist_lower_lim, gamma_Lab_MEC_hist_weighted_upper_lim, gamma_Lab_MEC_hist_weighted_lower_lim;
double gamma_Lab_RES_hist_upper_lim, gamma_Lab_RES_hist_lower_lim, gamma_Lab_RES_hist_weighted_upper_lim, gamma_Lab_RES_hist_weighted_lower_lim;
double gamma_Lab_DIS_hist_upper_lim, gamma_Lab_DIS_hist_lower_lim, gamma_Lab_DIS_hist_weighted_upper_lim, gamma_Lab_DIS_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE dP_T plots">
double dP_T_hist_upper_lim, dP_T_hist_lower_lim, dP_T_hist_weighted_upper_lim, dP_T_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
double P_L_hist_upper_lim, P_L_hist_lower_lim, P_R_hist_upper_lim, P_R_hist_lower_lim, P_lp_hist_upper_lim, P_lp_hist_lower_lim, P_pion_hist_upper_lim, P_pion_hist_lower_lim;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//</editor-fold>

#endif //ANALYSER_CODESETUP_H
