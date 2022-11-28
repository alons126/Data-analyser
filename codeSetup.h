
#ifndef ANALYSER_CODESETUP_H
#define ANALYSER_CODESETUP_H

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>

// Code version
// ======================================================================================================================================================================

std::string Ver = "7.0b";


// Functions definitions
// ======================================================================================================================================================================

//<editor-fold desc="Functions definitions">

//<editor-fold desc="BoolToString function">
inline const char *const BoolToString(bool b) {
    return b ? "true" : "false";
}
//</editor-fold>

//TODO:
//<editor-fold desc="histLimits function">
//std::tuple<double, double> histLimits(string file_name, string histogram_specification, string FS_specification) {
//    //<editor-fold desc="Histogram limits for every case">
//
//    //<editor-fold desc="Theta histograms">
//    if (histogram_specification == "Theta histograms" && FS_specification == "2p") {
//        //<editor-fold desc="Theta histograms (2p)">
//        theta_l_upper_lim_2p = 60;
//        theta_l_lower_lim_2p = 0;
//        theta_p1_upper_lim_2p = 190;
//        theta_p1_lower_lim_2p = -10;
//        theta_p2_upper_lim_2p = 190;
//        theta_p2_lower_lim_2p = -10;
//        dtheta_upper_lim_2p = 190;
//        dtheta_lower_lim_2p = -10;
//        //</editor-fold>
//    } else if (histogram_specification == "Theta histograms" && FS_specification == "1n1p") {
//        //<editor-fold desc="Theta histograms (1n1p)">
//        theta_l_upper_lim_1n1p = 60;
//        theta_l_lower_lim_1n1p = 0;
//        theta_p_upper_lim_1n1p = 190;
//        theta_p_lower_lim_1n1p = -10;
//        theta_n_upper_lim_1n1p = 190;
//        theta_n_lower_lim_1n1p = -10;
//        dtheta_upper_lim_1n1p = 190;
//        dtheta_lower_lim_1n1p = -10;
//        //</editor-fold>
//    }
//    //</editor-fold>
//
//    //<editor-fold desc="Phi histograms">
//    if (histogram_specification == "Phi histograms" && FS_specification == "2p") {
//
//        //<editor-fold desc="Phi histograms (2p)">
//        phi_l_upper_lim_2p = 60;
//        phi_l_lower_lim_2p = 0;
//        phi_p1_upper_lim_2p = 190;
//        phi_p1_lower_lim_2p = -10;
//        phi_p2_upper_lim_2p = 190;
//        phi_p2_lower_lim_2p = -10;
//        dphi_upper_lim_2p = 190;
//        dphi_lower_lim_2p = -10;
//        //</editor-fold>
//
//    } else if (histogram_specification == "Phi histograms" && FS_specification == "1n1p") {
//
//        //<editor-fold desc="Phi histograms (1n1p)">
//        phi_l_upper_lim_1n1p = 60;
//        phi_l_lower_lim_1n1p = 0;
//        phi_p_upper_lim_1n1p = 190;
//        phi_p_lower_lim_1n1p = -10;
//        phi_n_upper_lim_1n1p = 190;
//        phi_n_lower_lim_1n1p = -10;
//        dphi_upper_lim_1n1p = 190;
//        dphi_lower_lim_1n1p = -10;
//        //</editor-fold>
//
//    }
//
//    //<editor-fold desc="Phi histograms (2p)">
//    phi_l_upper_lim_2p = 200;
//    phi_l_lower_lim_2p = -200;
//    phi_p1_upper_lim_2p = 200;
//    phi_p1_lower_lim_2p = -200;
//    phi_p2_upper_lim_2p = 200;
//    phi_p2_lower_lim_2p = -200;
//    dphi_upper_lim_2p = 360;
//    dphi_lower_lim_2p = -360;
//    //</editor-fold>
//
//    //<editor-fold desc="Phi histograms (1n1p)">
//    phi_l_upper_lim_1n1p = 200;
//    phi_l_lower_lim_1n1p = -200;
//    phi_p_upper_lim_1n1p = 200;
//    phi_p_lower_lim_1n1p = -200;
//    phi_n_upper_lim_1n1p = 200;
//    phi_n_lower_lim_1n1p = -200;
//    dphi_upper_lim_1n1p = 360;
//    dphi_lower_lim_1n1p = -360;
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE plots">
//
//    //<editor-fold desc="MicroBooNE gamma plots (unweighted)">
//    gamma_Lab_hist_upper_lim_2p = 1;
//    gamma_Lab_hist_lower_lim_2p = -1;
//    gamma_mu_p_tot_upper_lim_2p = 1;
//    gamma_mu_p_tot_lower_lim_2p = -1;
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
//    gamma_Lab_hist_weighted_upper_lim_1n1p = 1;
//    gamma_Lab_hist_weighted_lower_lim_1n1p = -1;
//    gamma_mu_p_tot_weighted_upper_lim_1n1p = 1;
//    gamma_mu_p_tot_weighted_lower_lim_1n1p = -1;
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE gamma plots (no pions, for every interaction)">
//    gamma_Lab_all_hist_upper_lim = 1;
//    gamma_Lab_all_hist_lower_lim = -1;
//    gamma_Lab_all_hist_weighted_upper_lim = 1;
//    gamma_Lab_all_hist_weighted_lower_lim = -1;
//    gamma_Lab_QEL_hist_upper_lim = 1;
//    gamma_Lab_QEL_hist_lower_lim = -1;
//    gamma_Lab_QEL_hist_weighted_upper_lim = 1;
//    gamma_Lab_QEL_hist_weighted_lower_lim = -1;
//    gamma_Lab_MEC_hist_upper_lim = 1;
//    gamma_Lab_MEC_hist_lower_lim = -1;
//    gamma_Lab_MEC_hist_weighted_upper_lim = 1;
//    gamma_Lab_MEC_hist_weighted_lower_lim = -1;
//    gamma_Lab_RES_hist_upper_lim = 1;
//    gamma_Lab_RES_hist_lower_lim = -1;
//    gamma_Lab_RES_hist_weighted_upper_lim = 1;
//    gamma_Lab_RES_hist_weighted_lower_lim = -1;
//    gamma_Lab_DIS_hist_upper_lim = 1;
//    gamma_Lab_DIS_hist_lower_lim = -1;
//    gamma_Lab_DIS_hist_weighted_upper_lim = 1;
//    gamma_Lab_DIS_hist_weighted_lower_lim = -1;
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE dP_T plots">
//    if (FSI_status == true) {
//        dP_T_hist_upper_lim = 2;
//        dP_T_hist_lower_lim = 0;
//        dP_T_hist_weighted_upper_lim = 2;
//        dP_T_hist_weighted_lower_lim = 0;
//    } else if (FSI_status == false) {
//        dP_T_hist_upper_lim = 2;
//        dP_T_hist_lower_lim = 0;
//        dP_T_hist_weighted_upper_lim = 2;
//        dP_T_hist_weighted_lower_lim = 0;
////            dP_T_hist_upper_lim = 3;
////            dP_T_hist_lower_lim = 0;
////            dP_T_hist_weighted_upper_lim = 3;
////            dP_T_hist_weighted_lower_lim = 0;
//    }
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
//    P_L_hist_upper_lim = 3.0;
//    P_L_hist_lower_lim = 0;
//    P_R_hist_upper_lim = 3.0;
//    P_R_hist_lower_lim = 0;
//    P_l_hist_upper_lim = 3.0;
//    P_l_hist_lower_lim = 0;
//    P_pion_hist_upper_lim = 0.1;
//    P_pion_hist_lower_lim = 0;
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //<editor-fold desc="Histogram limits by cases">
//
//    if (file_name == "12C_056GeV_G18_10a_02_11a") {
//
//        //<editor-fold desc="12C_056GeV_G18_10a_02_11a histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        fsEl_upper_lim_2p = 0.6;
//        fsEl_lower_lim_2p = 0;
//        fsEl_QEL_upper_lim_2p = 0.6;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 0.6;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 0.6;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 0.6;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_2p_x = 0;
//        fsEl_VS_theta_l_upper_lim_2p_y = 0.6;
//        fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 0.6;
//        fsEl_lower_lim_1n1p = 0;
//        fsEl_QEL_upper_lim_1n1p = 0.6;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 0.6;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 0.6;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 0.6;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_y = 0.6;
//        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 0.6;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 0.6;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 0.6;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 0.6;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 0.6;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 0.6;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 0.6;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 0.6;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 0.6;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 0.6;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 0.6;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 0.6;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 0.6;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 0.6;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 0.6;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 0.6;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 0.6;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 0.6;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 0.6;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 0.6;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 0.6;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 0.6;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 0.6;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 0.6;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 0.6;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 0.6;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 0.6;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 0.6;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 0.6;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 0.6;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 0.6;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 0.6;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 0.11;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 0.11;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 0.11;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 0.11;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 0.11;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 0.9;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 0.6;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 0.9;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 0.6;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 0.9;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 0.6;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 0.9;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 0.6;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 0.9;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 0.6;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 0.9;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 0.6;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 0.9;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 0.6;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 0.9;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 0.6;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 0.9;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 0.6;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 0.65;
//        E_cal_QEL_lower_lim_range = 0.45;
//        E_cal_MEC_upper_lim_range = 0.65;
//        E_cal_MEC_lower_lim_range = 0.45;
//        E_cal_RES_upper_lim_range = 0.65;
//        E_cal_RES_lower_lim_range = 0.45;
//        E_cal_DIS_upper_lim_range = 0.65;
//        E_cal_DIS_lower_lim_range = 0.45;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 1.5;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 1.5;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 1.5;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 1.5;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 1.5;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 1.5;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "12C_0961GeV_G18_10a_02_11a") {
//
//        //<editor-fold desc="12C_0961GeV_G18_10a_02_11a histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        fsEl_upper_lim_2p = 1.0;
//        fsEl_lower_lim_2p = 0;
//        fsEl_QEL_upper_lim_2p = 1.0;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 1.0;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 1.0;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 1.0;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_2p_x = 0;
//        fsEl_VS_theta_l_upper_lim_2p_y = 1.0;
//        fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 1.0;
//        fsEl_lower_lim_1n1p = 0;
//        fsEl_QEL_upper_lim_1n1p = 1.0;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 1.0;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 1.0;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 1.0;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 0.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 0.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 0.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 0.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 0.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 0.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 0.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 0.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 0.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 0.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 0.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 0.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 0.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 0.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 0.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 0.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 0.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 0.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 0.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 0.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 0.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 0.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 0.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 0.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 0.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 0.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 0.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 0.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 0.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 0.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 0.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 0.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 0.75;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 0.75;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 0.75;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 0.75;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 0.75;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.5;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.0;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.5;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.0;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.5;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.0;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 1.5;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 1.0;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.5;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.0;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.5;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.0;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.5;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.0;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.5;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.0;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.5;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.0;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 1.1;
//        E_cal_QEL_lower_lim_range = 0.8;
//        E_cal_MEC_upper_lim_range = 1.1;
//        E_cal_MEC_lower_lim_range = 0.8;
//        E_cal_RES_upper_lim_range = 1.1;
//        E_cal_RES_lower_lim_range = 0.8;
//        E_cal_DIS_upper_lim_range = 1.1;
//        E_cal_DIS_lower_lim_range = 0.8;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            P_L_hist_upper_lim_2p = 1.5;
//            P_L_hist_lower_lim_2p = 0;
//            P_R_hist_upper_lim_2p = 1.5;
//            P_R_hist_lower_lim_2p = 0;
//            P_lp_hist_upper_lim_2p = 1.5;
//            P_lp_hist_lower_lim_2p = 0;
//        } else if (BEnergyToNucleusCon == false) {
//            P_L_hist_upper_lim_2p = 1.5;
//            P_L_hist_lower_lim_2p = 0;
//            P_R_hist_upper_lim_2p = 1.5;
//            P_R_hist_lower_lim_2p = 0;
//            P_lp_hist_upper_lim_2p = 1.5;
//            P_lp_hist_lower_lim_2p = 0;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 1.5;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 1.5;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 1.5;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "C12_1161GeV_EM+MEC_G18_02a_00_000_Q2_0_1") {
//
//        //<editor-fold desc="C12_1161GeV_EM+MEC_G18_02a_00_000_Q2_0_1 histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        fsEl_upper_lim_2p = 1.2;
//        fsEl_lower_lim_2p = 0;
//        fsEl_QEL_upper_lim_2p = 1.2;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 1.2;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 1.2;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 1.2;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_2p_x = 0;
//        fsEl_VS_theta_l_upper_lim_2p_y = 1.2;
//        fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 2.21;
//        fsEl_lower_lim_1n1p = 1;
//        fsEl_QEL_upper_lim_1n1p = 1.2;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 1.2;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 1.2;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 1.2;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.2;
//        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 2.0;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 2.0;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 2.0;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 2.0;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 2.0;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 2.0;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 2.0;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 2.0;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 2;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 2;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 2;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 2;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 2;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 2;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 2;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 2;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 2;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 2;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 2;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 2;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 2;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 2;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 2;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 2;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 2;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 2;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//        E_Trans15_DIS_upper_lim_2p = 2;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 2;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 2;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//        E_Trans15_DIS_upper_lim_1n1p = 2;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 2;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 2;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 1.2;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 1.2;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 1.2;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 1.2;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 1.2;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.8;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.2;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.8;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.2;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.8;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.2;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 1.8;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 1.2;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.8;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.2;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.8;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.2;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.8;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.2;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.8;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.2;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.8;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.2;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 1.2;
//        E_cal_QEL_lower_lim_range = 0;
//        E_cal_MEC_upper_lim_range = 1.2;
//        E_cal_MEC_lower_lim_range = 0;
//        E_cal_RES_upper_lim_range = 1.2;
//        E_cal_RES_lower_lim_range = 0;
//        E_cal_DIS_upper_lim_range = 1.2;
//        E_cal_DIS_lower_lim_range = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 1.2;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 1.2;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 1.2;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 1.2;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 1.2;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 1.2;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "12C_1299GeV_G18_10a_02_11a") {
//
//        //<editor-fold desc="12C_1299GeV_G18_10a_02_11a histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        fsEl_upper_lim_2p = 1.3;
//        fsEl_lower_lim_2p = 0;
//        fsEl_QEL_upper_lim_2p = 1.3;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 1.3;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 1.3;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 1.3;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_2p_x = 0;
//        fsEl_VS_theta_l_upper_lim_2p_y = 1.3;
//        fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 1.3;
//        fsEl_lower_lim_1n1p = 0;
//        fsEl_QEL_upper_lim_1n1p = 1.3;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 1.3;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 1.3;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 1.3;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.3;
//        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 0.8;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 0.8;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 0.8;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 0.8;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 0.8;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 0.8;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 0.8;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 0.8;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 0.8;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 0.8;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 0.8;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 0.8;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 0.8;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 0.8;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 0.8;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 0.8;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 0.8;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 0.8;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 0.8;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 0.8;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 0.8;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 0.8;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 0.8;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 0.8;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 0.8;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 0.8;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 0.8;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 0.8;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 0.8;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 0.8;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 0.8;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 0.8;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 1.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 1.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 1.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 1.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 1.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.9;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.4;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.9;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.4;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.9;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.4;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 1.9;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 1.4;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.9;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.4;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.9;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.4;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.9;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.4;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.9;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.4;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.9;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.4;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 1.4;
//        E_cal_QEL_lower_lim_range = 1.15;
//        E_cal_MEC_upper_lim_range = 1.4;
//        E_cal_MEC_lower_lim_range = 1.15;
//        E_cal_RES_upper_lim_range = 1.4;
//        E_cal_RES_lower_lim_range = 1.15;
//        E_cal_DIS_upper_lim_range = 1.4;
//        E_cal_DIS_lower_lim_range = 1.15;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 1.5;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 1.5;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 1.5;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 1.5;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 1.5;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 1.5;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "12C_2222GeV_G18_10a_02_11a") {
//
//        //<editor-fold desc="12C_2222GeV_G18_10a_02_11a histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
////                fsEl_upper_lim_2p = 2.3;
////                fsEl_lower_lim_2p = 1.6;
////                fsEl_QEL_upper_lim_2p = 2.3;
////                fsEl_QEL_lower_lim_2p = 1.6;
////                fsEl_MEC_upper_lim_2p = 2.3;
////                fsEl_MEC_lower_lim_2p = 1.6;
////                fsEl_RES_upper_lim_2p = 2.3;
////                fsEl_RES_lower_lim_2p = 1.6;
////                fsEl_DIS_upper_lim_2p = 2.3;
////                fsEl_DIS_lower_lim_2p = 1.6;
////                fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_2p_x = 0;
////                fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_2p_y = 0;
//
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
////                fsEl_upper_lim_1n1p = 2.3;
////                fsEl_lower_lim_1n1p = 1.6;
////                fsEl_QEL_upper_lim_1n1p = 2.3;
////                fsEl_QEL_lower_lim_1n1p = 1.6;
////                fsEl_MEC_upper_lim_1n1p = 2.3;
////                fsEl_MEC_lower_lim_1n1p = 1.6;
////                fsEl_RES_upper_lim_1n1p = 2.3;
////                fsEl_RES_lower_lim_1n1p = 1.6;
////                fsEl_DIS_upper_lim_1n1p = 2.3;
////                fsEl_DIS_lower_lim_1n1p = 1.6;
////                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
////                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 2.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 2.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 2.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 2.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 2.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 2.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 2.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 2.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 2.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 2.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 2.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "GENIE_with_fsi") {
//
//        //<editor-fold desc="GENIE_with_fsi histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
////                fsEl_upper_lim_2p = 2.3;
////                fsEl_lower_lim_2p = 1.6;
////                fsEl_QEL_upper_lim_2p = 2.3;
////                fsEl_QEL_lower_lim_2p = 1.6;
////                fsEl_MEC_upper_lim_2p = 2.3;
////                fsEl_MEC_lower_lim_2p = 1.6;
////                fsEl_RES_upper_lim_2p = 2.3;
////                fsEl_RES_lower_lim_2p = 1.6;
////                fsEl_DIS_upper_lim_2p = 2.3;
////                fsEl_DIS_lower_lim_2p = 1.6;
////                fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_2p_x = 0;
////                fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_2p_y = 0;
//
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
////                fsEl_upper_lim_1n1p = 2.3;
////                fsEl_lower_lim_1n1p = 1.6;
////                fsEl_QEL_upper_lim_1n1p = 2.3;
////                fsEl_QEL_lower_lim_1n1p = 1.6;
////                fsEl_MEC_upper_lim_1n1p = 2.3;
////                fsEl_MEC_lower_lim_1n1p = 1.6;
////                fsEl_RES_upper_lim_1n1p = 2.3;
////                fsEl_RES_lower_lim_1n1p = 1.6;
////                fsEl_DIS_upper_lim_1n1p = 2.3;
////                fsEl_DIS_lower_lim_1n1p = 1.6;
////                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
////                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 2.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 2.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 2.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 2.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 2.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 2.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 2.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 2.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 2.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 2.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 2.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "GENIE_no_fsi") {
//
//        //<editor-fold desc="GENIE_no_fsi histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
////                fsEl_upper_lim_2p = 2.3;
////                fsEl_lower_lim_2p = 1.6;
////                fsEl_QEL_upper_lim_2p = 2.3;
////                fsEl_QEL_lower_lim_2p = 1.6;
////                fsEl_MEC_upper_lim_2p = 2.3;
////                fsEl_MEC_lower_lim_2p = 1.6;
////                fsEl_RES_upper_lim_2p = 2.3;
////                fsEl_RES_lower_lim_2p = 1.6;
////                fsEl_DIS_upper_lim_2p = 2.3;
////                fsEl_DIS_lower_lim_2p = 1.6;
////                fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_2p_x = 0;
////                fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_2p_y = 0;
//
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
////                fsEl_upper_lim_1n1p = 2.3;
////                fsEl_lower_lim_1n1p = 1.6;
////                fsEl_QEL_upper_lim_1n1p = 2.3;
////                fsEl_QEL_lower_lim_1n1p = 1.6;
////                fsEl_MEC_upper_lim_1n1p = 2.3;
////                fsEl_MEC_lower_lim_1n1p = 1.6;
////                fsEl_RES_upper_lim_1n1p = 2.3;
////                fsEl_RES_lower_lim_1n1p = 1.6;
////                fsEl_DIS_upper_lim_1n1p = 2.3;
////                fsEl_DIS_lower_lim_1n1p = 1.6;
////                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
////                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 2.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 2.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 2.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 2.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 2.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 2.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 2.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 2.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 2.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 2.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 2.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "12C_2222GeV_GTEST19_10b_00_000") {
//
//        //<editor-fold desc="12C_2222GeV_GTEST19_10b_00_000 histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
////                fsEl_upper_lim_2p = 2.3;
////                fsEl_lower_lim_2p = 1.6;
////                fsEl_QEL_upper_lim_2p = 2.3;
////                fsEl_QEL_lower_lim_2p = 1.6;
////                fsEl_MEC_upper_lim_2p = 2.3;
////                fsEl_MEC_lower_lim_2p = 1.6;
////                fsEl_RES_upper_lim_2p = 2.3;
////                fsEl_RES_lower_lim_2p = 1.6;
////                fsEl_DIS_upper_lim_2p = 2.3;
////                fsEl_DIS_lower_lim_2p = 1.6;
////                fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_2p_x = 0;
////                fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_2p_y = 0;
//
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
////                fsEl_upper_lim_1n1p = 2.3;
////                fsEl_lower_lim_1n1p = 1.6;
////                fsEl_QEL_upper_lim_1n1p = 2.3;
////                fsEl_QEL_lower_lim_1n1p = 1.6;
////                fsEl_MEC_upper_lim_1n1p = 2.3;
////                fsEl_MEC_lower_lim_1n1p = 1.6;
////                fsEl_RES_upper_lim_1n1p = 2.3;
////                fsEl_RES_lower_lim_1n1p = 1.6;
////                fsEl_DIS_upper_lim_1n1p = 2.3;
////                fsEl_DIS_lower_lim_1n1p = 1.6;
////                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
////                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 2.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 2.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 2.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 2.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 2.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 2.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 2.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 2.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 2.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 2.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 2.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "adi_11_1000060120_2222_fsi.gst") {
//
//        //<editor-fold desc="adi_11_1000060120_2222_fsi.gst histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
////                fsEl_upper_lim_2p = 2.3;
////                fsEl_lower_lim_2p = 1.6;
////                fsEl_QEL_upper_lim_2p = 2.3;
////                fsEl_QEL_lower_lim_2p = 1.6;
////                fsEl_MEC_upper_lim_2p = 2.3;
////                fsEl_MEC_lower_lim_2p = 1.6;
////                fsEl_RES_upper_lim_2p = 2.3;
////                fsEl_RES_lower_lim_2p = 1.6;
////                fsEl_DIS_upper_lim_2p = 2.3;
////                fsEl_DIS_lower_lim_2p = 1.6;
////                fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_2p_x = 0;
////                fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_2p_y = 0;
//
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
////                fsEl_upper_lim_1n1p = 2.3;
////                fsEl_lower_lim_1n1p = 1.6;
////                fsEl_QEL_upper_lim_1n1p = 2.3;
////                fsEl_QEL_lower_lim_1n1p = 1.6;
////                fsEl_MEC_upper_lim_1n1p = 2.3;
////                fsEl_MEC_lower_lim_1n1p = 1.6;
////                fsEl_RES_upper_lim_1n1p = 2.3;
////                fsEl_RES_lower_lim_1n1p = 1.6;
////                fsEl_DIS_upper_lim_1n1p = 2.3;
////                fsEl_DIS_lower_lim_1n1p = 1.6;
////                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
////                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 2.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 2.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 2.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 2.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 2.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 2.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 2.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 2.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 2.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 2.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 2.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "asportes_11_1000060120_2222_nofsi_10M.gst") {
//
//        //<editor-fold desc="asportes_11_1000060120_2222_nofsi_10M.gst histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_2p_x = 0;
//            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_2p_y = 0;
////                fsEl_upper_lim_2p = 2.3;
////                fsEl_lower_lim_2p = 1.6;
////                fsEl_QEL_upper_lim_2p = 2.3;
////                fsEl_QEL_lower_lim_2p = 1.6;
////                fsEl_MEC_upper_lim_2p = 2.3;
////                fsEl_MEC_lower_lim_2p = 1.6;
////                fsEl_RES_upper_lim_2p = 2.3;
////                fsEl_RES_lower_lim_2p = 1.6;
////                fsEl_DIS_upper_lim_2p = 2.3;
////                fsEl_DIS_lower_lim_2p = 1.6;
////                fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_2p_x = 0;
////                fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_2p_y = 0;
//
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
////                fsEl_upper_lim_1n1p = 2.3;
////                fsEl_lower_lim_1n1p = 1.6;
////                fsEl_QEL_upper_lim_1n1p = 2.3;
////                fsEl_QEL_lower_lim_1n1p = 1.6;
////                fsEl_MEC_upper_lim_1n1p = 2.3;
////                fsEl_MEC_lower_lim_1n1p = 1.6;
////                fsEl_RES_upper_lim_1n1p = 2.3;
////                fsEl_RES_lower_lim_1n1p = 1.6;
////                fsEl_DIS_upper_lim_1n1p = 2.3;
////                fsEl_DIS_lower_lim_1n1p = 1.6;
////                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
////                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
////                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
////                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 2.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 2.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 2.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 2.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 2.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        if (BEnergyToNucleusCon == true) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + BEnergyToNucleus;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + BEnergyToNucleus;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + BEnergyToNucleus;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + BEnergyToNucleus;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + BEnergyToNucleus;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + BEnergyToNucleus;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + BEnergyToNucleus;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + BEnergyToNucleus;
//        } else if (BEnergyToNucleusCon == false) {
//            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        }
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 2.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 2.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 2.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 2.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 2.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 2.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "40Ar_BNBFluxGeV_EM+MEC_G18_02a_00_000_Q2_0_0") {
//
//        //<editor-fold desc="40Ar_BNBFluxGeV_EM+MEC_G18_02a_00_000_Q2_0_0 histogram limits">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        fsEl_upper_lim_2p = 3;
//        fsEl_lower_lim_2p = 0;
//        fsEl_QEL_upper_lim_2p = 3;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 3;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 3;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 3;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_2p_x = 0;
//        fsEl_VS_theta_l_upper_lim_2p_y = 3;
//        fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 2.21;
//        fsEl_lower_lim_1n1p = 1;
//        fsEl_QEL_upper_lim_1n1p = 3;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 3;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 3;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 3;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_y = 3;
//        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 3.0;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 3.0;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 3.0;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 3.0;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 3.0;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 3.0;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 3.0;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 3.0;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 3.0;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 3.0;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 3.0;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 3.0;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 3.0;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 3.0;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 3.0;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 3.0;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 3.0;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 3.0;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 3.0;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 3.0;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 3.0;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 3.0;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 3.0;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 3.0;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 3.0;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 3.0;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 3.0;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 3.0;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 3.0;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 3.0;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 3.0;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 3.0;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 3;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 3;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 3;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 3;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 3;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 3;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 3;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 3;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 3;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 3;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 3;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 3;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 3;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 3;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        E_cal_QEL_upper_lim_2p = 3.1;
//        E_cal_QEL_lower_lim_2p = 3.1;
//        E_cal_MEC_upper_lim_2p = 3.1;
//        E_cal_MEC_lower_lim_2p = 3.1;
//        E_cal_RES_upper_lim_2p = 3.1;
//        E_cal_RES_lower_lim_2p = 3.1;
//        E_cal_DIS_upper_lim_2p = 3.1;
//        E_cal_DIS_lower_lim_2p = 3.1;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        E_cal_QEL_upper_lim_1n1p = 3.1;
//        E_cal_QEL_lower_lim_1n1p = 3.1;
//        E_cal_MEC_upper_lim_1n1p = 3.1;
//        E_cal_MEC_lower_lim_1n1p = 3.1;
//        E_cal_RES_upper_lim_1n1p = 3.1;
//        E_cal_RES_lower_lim_1n1p = 3.1;
//        E_cal_DIS_upper_lim_1n1p = 3.1;
//        E_cal_DIS_lower_lim_1n1p = 3.1;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 3.0;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 3.0;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 3.0;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 3.0;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 3.0;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 3.0;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//    } else if (file_name == "56Fe_1299GeV_G18_10a_02_11a") {
//
//        //<editor-fold desc="56Fe_1299GeV_G18_10a_02_11a histogram limits">
//
//        //<editor-fold desc="Energy histograms (2p)">
//        fsEl_upper_lim_2p = 1.3;
//        fsEl_lower_lim_2p = 0;
//        fsEl_QEL_upper_lim_2p = 1.3;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 1.3;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 1.3;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 1.3;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_2p_x = 0;
//        fsEl_VS_theta_l_upper_lim_2p_y = 1.3;
//        fsEl_VS_theta_l_lower_lim_2p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 2.21;
//        fsEl_lower_lim_1n1p = 1;
//        fsEl_QEL_upper_lim_1n1p = 1.3;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 1.3;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 1.3;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 1.3;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
//        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.3;
//        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.3;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.3;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.3;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.3;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.3;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.3;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.3;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.3;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.3;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.3;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.3;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.3;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.3;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.3;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.3;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.3;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.3;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.3;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.3;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.3;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.3;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.3;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.3;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.3;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.3;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.3;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.3;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.3;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.3;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.3;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.3;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.3;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 1.3;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 1.3;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 1.3;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 1.3;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 1.3;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.3;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.4;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.3;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.4;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.3;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.4;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 1.3;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 1.4;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.3;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.4;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.3;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.4;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.3;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.4;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.3;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.4;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.3;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.4;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (2p)">
//        E_cal_QEL_upper_lim_2p = 1.5;
//        E_cal_QEL_lower_lim_2p = 1.5;
//        E_cal_MEC_upper_lim_2p = 1.5;
//        E_cal_MEC_lower_lim_2p = 1.5;
//        E_cal_RES_upper_lim_2p = 1.5;
//        E_cal_RES_lower_lim_2p = 1.5;
//        E_cal_DIS_upper_lim_2p = 1.5;
//        E_cal_DIS_lower_lim_2p = 1.5;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal restoration histograms (1n1p)">
//        E_cal_QEL_upper_lim_1n1p = 1.5;
//        E_cal_QEL_lower_lim_1n1p = 1.5;
//        E_cal_MEC_upper_lim_1n1p = 1.5;
//        E_cal_MEC_lower_lim_1n1p = 1.5;
//        E_cal_RES_upper_lim_1n1p = 1.5;
//        E_cal_RES_lower_lim_1n1p = 1.5;
//        E_cal_DIS_upper_lim_1n1p = 1.5;
//        E_cal_DIS_lower_lim_1n1p = 1.5;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 1.3;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 1.3;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 1.3;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 1.3;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 1.3;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_l_hist_upper_lim_1n1p = 1.3;
//        P_l_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//    }
//
//    //</editor-fold>
//
//    return  std::make_tuple(dividend / divisor, dividend % divisor);
//}
//</editor-fold>

//<editor-fold desc="rCalc function">
double rCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}
//</editor-fold>

//TODO: to finish
//<editor-fold desc="histPlotter1D function">
void histPlotter1D(TCanvas *Histogram1DCanvas, //The canvas
                   TH1D *Histogram1D, //The histogram
                   bool normalize_Histogram, //Normalize histogram or not
                   bool custom_normalization, //Normalize histogram or not
                   double custom_normalization_factor, //Normalize histogram or not
                   string Histogram1DTitle,
                   string Histogram1DTitleReactions,
                   double titleSize,
                   double labelSizex,
                   double labelSizey,
                   TList *Histogram_list,
                   int lineWidth,
                   bool logScalePlot,
                   bool linearScalePlot,
                   THStack *Histogram1DStack,
                   string Histogram1DSaveName,
                   string Histogram1DSaveNamePath,
                   string finalState,
                   int kColor = 1,
                   bool centerTitle = true,
                   bool addToStack = false,
                   bool showStats = true) {

//  Normalization factor:
    double Histogram1D_integral; // To be calculated only if normalize_Histogram == true
    double x_1 = 0.15, y_1 = 0.3, x_2 = 0.85, y_2 = 0.7;
    double diplayTextSize = 0.1225;

    if (normalize_Histogram == true && custom_normalization == false) {
        Histogram1D_integral = Histogram1D->Integral();
    } else if (normalize_Histogram == true && custom_normalization == true) {
        Histogram1D_integral = custom_normalization_factor;
    }

    if (normalize_Histogram == true) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")" + " - Normalized";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Probability (%)");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2,"NDC");
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Scale(100. / Histogram1D_integral, "nosw2");
            Histogram1D->Draw();
        }
    } else if (normalize_Histogram == false) {
        string title = Histogram1DTitle + " (" + Histogram1DTitleReactions + ", " + finalState + ")";
        const char *HistogramTitle = title.c_str();
        Histogram1D->SetTitle(HistogramTitle);
        Histogram1D->GetYaxis()->SetTitle("Arbitrary units");
        if (Histogram1D->Integral() == 0.) {
            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2,"NDC");
//            TPaveText *displayText = new TPaveText(x_1,y_1,x_2,y_2);
            displayText->SetTextSize(diplayTextSize);
            displayText->SetFillColor(0);
            displayText->SetTextAlign(12);
            displayText->AddText("Empty histogram");
            Histogram1D->Draw();
            displayText->Draw();
        } else if (Histogram1D->Integral() != 0.) {
            Histogram1D->Draw();
        }
    }

    Histogram1D->GetXaxis()->SetTitleSize(titleSize);
    Histogram1D->GetXaxis()->SetLabelSize(labelSizex);
    Histogram1D->GetXaxis()->CenterTitle(centerTitle);
    Histogram1D->GetYaxis()->SetTitleSize(titleSize);
    Histogram1D->GetYaxis()->SetLabelSize(labelSizey);
    Histogram1D->GetYaxis()->CenterTitle(centerTitle);
    Histogram_list->Add(Histogram1D);
    Histogram1D->SetLineWidth(lineWidth);

    if (showStats == false) {
        Histogram1D->SetStats(0);
    }

    if (logScalePlot) {
        Histogram1DCanvas->SetLogy(1);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_log_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (linearScalePlot) {
        Histogram1DCanvas->SetLogy(0);
        string Histogram1DSaveNameDir = Histogram1DSaveNamePath + Histogram1DSaveName + "_linear_scale_" + finalState + ".png";
        const char *SaveDir = Histogram1DSaveNameDir.c_str();
        Histogram1DCanvas->SaveAs(SaveDir);
    }

    if (addToStack == true) {
        Histogram1D->SetLineColor(kColor);
        Histogram1D->SetStats(0);
        Histogram1DStack->Add(Histogram1D);
    }

    Histogram1DCanvas->Clear();

}
//</editor-fold>

//TODO:
//<editor-fold desc="histPlotter2D function">
//void histPlotter2D(double x, double y, double z) {
//}
//</editor-fold>

//TODO:
//<editor-fold desc="calc2p function">
//void calc2p(bool qel, bool mec, bool res, bool dis, bool calculate_2p, int nfp, int nfn, int nf, int length, int pdgf[15], int ProtonCounter_2p, int Proton_1_ind_2p,
//             int Proton_2_ind_2p, Double_t pxl, Double_t pyl, Double_t pzl, Double_t pxf[15], Double_t pyf[15], Double_t pzf[15], Double_t Ef[15], TH1D *P_L_hist_2p,
//             TH1D *P_R_hist_2p, TH1D *phi_p1_2p, TH1D *phi_p2_2p, TH1D *dphi_2p, TH1D *theta_p1_2p, TH1D *theta_p2_2p, TH1D *dtheta_2p,
//             TH2D *E_cal_VS_theta_p1_QEL_2p,
//             TH2D *E_cal_VS_theta_p2_QEL_2p, TH2D *E_Trans_VS_q3_all_2p, Double_t Ev, Double_t El, double q3, double q, TH1D *P_lp_hist_2p, TH1D *theta_l_2p,
//             TH1D *phi_l_2p, TH2D *E_cal_VS_theta_l_all_int_2p, Double_t Q2, Double_t W, Double_t En, Double_t pxn, Double_t pyn, Double_t pzn,
//             TH2D *E_cal_VS_Q2_all_int_2p,
//             TH2D *E_cal_VS_dtheta_all_int_2p, TH1D *fsEl_2p, TH2D *fsEl_VS_theta_l_2p, TH1D *E_Trans_all_ang_all_int_2p, TH1D *gamma_Lab_all_hist_weighted,
//             TH1D *E_Trans15_all_2p, TH1D *E_Trans45_all_2p, TH1D *E_Trans90_all_2p, TH1D *gamma_Lab_all_hist, TH1D *gamma_Lab_QEL_hist,
//             TH1D *gamma_Lab_QEL_hist_weighted,
//             TH1D *E_Trans15_QEL_2p, TH1D *E_Trans45_QEL_2p, TH1D *E_Trans90_QEL_2p, TH1D *E_cal_QEL_2p, TH2D *E_cal_VS_theta_l_QEL_2p,
//             TH2D *E_cal_VS_Q2_QEL_2p,
//             TH2D *E_cal_VS_W_QEL_2p, TH2D *E_cal_VS_En_QEL_2p, TH2D *E_cal_VS_Pn_QEL_2p, TH2D *E_cal_VS_Pn1_QEL_2p, TH2D *E_cal_VS_Pn2_QEL_2p,
//             TH2D *E_cal_VS_dtheta_QEL_2p, TH2D *E_Trans_VS_q3_QEL_2p, TH1D *fsEl_QEL_2p, TH1D *gamma_Lab_MEC_hist, TH1D *gamma_Lab_MEC_hist_weighted,
//             TH1D *E_Trans15_MEC_2p, TH1D *E_Trans45_MEC_2p, TH1D *E_Trans90_MEC_2p, TH1D *E_cal_MEC_2p, TH2D *E_Trans_VS_q3_MEC_2p, TH1D *fsEl_MEC_2p,
//             TH1D *gamma_Lab_RES_hist,
//             TH1D *gamma_Lab_RES_hist_weighted, TH1D *E_Trans15_RES_2p, TH1D *E_Trans45_RES_2p, TH1D *E_Trans90_RES_2p, TH1D *E_cal_RES_2p, TH1D *fsEl_RES_2p,
//             TH1D *gamma_Lab_DIS_hist, TH1D *gamma_Lab_DIS_hist_weighted, TH1D *E_Trans15_DIS_2p, TH1D *E_Trans45_DIS_2p, TH1D *E_Trans90_DIS_2p, TH1D *E_cal_DIS_2p,
//             TH1D *fsEl_DIS_2p) {
////void calc2p(bool qel, bool mec, bool res, bool dis, bool calculate_2p, int nfp, int nfn, int nf, int length, int pdgf[15], int ProtonCounter_2p, int Proton_1_ind_2p,
////             int Proton_2_ind_2p, Double_t pxl, Double_t pyl, Double_t pzl, Double_t pxf[15], Double_t pyf[15], Double_t pzf[15], Double_t Ef[15], TH1D P_L_hist_2p,
////             TH1D P_R_hist_2p, TH1D phi_p1_2p, TH1D phi_p2_2p, TH1D dphi_2p, TH1D theta_p1_2p, TH1D theta_p2_2p, TH1D dtheta_2p, TH2D E_cal_VS_theta_p1_QEL_2p,
////             TH2D E_cal_VS_theta_p2_QEL_2p, TH2D E_Trans_VS_q3_all_2p, Double_t Ev, Double_t El, double q3, double q, TH1D P_lp_hist_2p, TH1D theta_l_2p,
////             TH1D phi_l_2p, TH2D E_cal_VS_theta_l_all_int_2p, Double_t Q2, Double_t W, Double_t En, Double_t pxn, Double_t pyn, Double_t pzn, TH2D E_cal_VS_Q2_all_int_2p,
////             TH2D E_cal_VS_dtheta_all_int_2p, TH1D fsEl_2p, TH2D fsEl_VS_theta_l_2p, TH1D E_Trans_all_ang_all_int_2p, TH1D gamma_Lab_all_hist_weighted,
////             TH1D E_Trans15_all_2p, TH1D E_Trans45_all_2p, TH1D E_Trans90_all_2p, TH1D gamma_Lab_all_hist, TH1D gamma_Lab_QEL_hist, TH1D gamma_Lab_QEL_hist_weighted,
////             TH1D E_Trans15_QEL_2p, TH1D E_Trans45_QEL_2p, TH1D E_Trans90_QEL_2p, TH1D E_cal_QEL_2p, TH1D E_cal_VS_theta_l_QEL_2p, TH2D E_cal_VS_Q2_QEL_2p,
////             TH2D E_cal_VS_W_QEL_2p, TH2D E_cal_VS_En_QEL_2p, TH2D E_cal_VS_Pn_QEL_2p, TH2D E_cal_VS_Pn1_QEL_2p, TH2D E_cal_VS_Pn2_QEL_2p,
////             TH2D E_cal_VS_dtheta_QEL_2p, TH2D E_Trans_VS_q3_QEL_2p, TH1D fsEl_QEL_2p, TH1D gamma_Lab_MEC_hist, TH1D gamma_Lab_MEC_hist_weighted,
////             TH1D E_Trans15_MEC_2p, TH1D E_Trans45_MEC_2p, TH1D E_Trans90_MEC_2p, TH1D E_cal_MEC_2p, TH2D E_Trans_VS_q3_MEC_2p, TH1D fsEl_MEC_2p, TH1D gamma_Lab_RES_hist,
////             TH1D gamma_Lab_RES_hist_weighted, TH1D E_Trans15_RES_2p, TH1D E_Trans45_RES_2p, TH1D E_Trans90_RES_2p, TH1D E_cal_RES_2p, TH1D fsEl_RES_2p,
////             TH1D gamma_Lab_DIS_hist, TH1D gamma_Lab_DIS_hist_weighted, TH1D E_Trans15_DIS_2p, TH1D E_Trans45_DIS_2p, TH1D E_Trans90_DIS_2p, TH1D E_cal_DIS_2p,
////             TH1D fsEl_DIS_2p) {
//
//    if (calculate_2p && nfp == 2 && nfn == 0 && nf == 2) { // See if there are 2FS protons and 2FS hadrons (2p)
//        for (int i = 0; i < length; i++) {
////                if ((before_FSI_events == true) && (TMath::Abs(resc[i]) == 14) && (pdgf[i] == 2212)) {
////                    ProtonCounter_2p = ProtonCounter_2p + 1;
////                    if (ProtonCounter_2p == 1) {
////                        Proton_1_ind_2p = i;
////                    } else if (ProtonCounter_2p == 2) {
////                        Proton_2_ind_2p = i;
////                    }
////                } else if ((before_FSI_events == false) && pdgf[i] == 2212) {
//            if (pdgf[i] == 2212) {
//                ProtonCounter_2p = ProtonCounter_2p + 1;
//                if (ProtonCounter_2p == 1) {
//                    Proton_1_ind_2p = i;
//                } else if (ProtonCounter_2p == 2) {
//                    Proton_2_ind_2p = i;
//                }
//            }
//        }
//
////          Momentum of first proton in Ef[]:
//        double P_p1_2p = sqrt(
//                pxf[Proton_1_ind_2p] * pxf[Proton_1_ind_2p] + pyf[Proton_1_ind_2p] * pyf[Proton_1_ind_2p] + pzf[Proton_1_ind_2p] * pzf[Proton_1_ind_2p]);
//
////          Momentum of second proton in Ef[]:
//        double P_p2_2p = sqrt(
//                pxf[Proton_2_ind_2p] * pxf[Proton_2_ind_2p] + pyf[Proton_2_ind_2p] * pyf[Proton_2_ind_2p] + pzf[Proton_2_ind_2p] * pzf[Proton_2_ind_2p]);
//
////          Momentum of second proton in Ef[]:
//        double P_l_2p = sqrt(pxf[Proton_2_ind_2p] * pxf[Proton_2_ind_2p] + pyf[Proton_2_ind_2p] * pyf[Proton_2_ind_2p] + pzf[Proton_2_ind_2p] * pzf[Proton_2_ind_2p]);
//
////          Leading proton:
//        double P_L_2p = -1;
//
////          Recoil proton:
//        double P_R_2p = -1;
//
////          Momentum cut to at least 300 [MeV/c] == 0.3 [GeV/c]:
//        if (P_p1_2p >= 0.3 && P_p2_2p >= 0.3) {
//
//            double E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272);
//            double Theta_l_2p = acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359; // Theta_l_2p is in degrees
//
////              NOT REALLY dtheta:
//            double d_theta_2p = acos(
//                    (pxf[Proton_1_ind_2p] * pxf[Proton_2_ind_2p] + pyf[Proton_1_ind_2p] * pyf[Proton_2_ind_2p] + pzf[Proton_1_ind_2p] * pzf[Proton_2_ind_2p]) /
//                    (P_p1_2p * P_p2_2p));
//            dtheta_2p->Fill(d_theta_2p * 180.0 / 3.14159265359);
//
//            if (Ef[Proton_1_ind_2p] >= Ef[Proton_2_ind_2p]) { // If Proton_1_ind_2p is the leading proton and Proton_2_ind_2p is the recoil
////                if (P_p1_2p >= P_p2_2p) { // If Proton_1_ind_2p is the leading proton and Proton_2_ind_2p is the recoil
//
////                  Leading proton:
//                double P_L_2p = P_p1_2p;
//
////                  Recoil proton:
//                double P_R_2p = P_p2_2p;
//
//                P_L_hist_2p->Fill(P_L_2p);
//                P_R_hist_2p->Fill(P_R_2p);
//
//                double phi_p1 = atan2(pyf[Proton_1_ind_2p], pxf[Proton_1_ind_2p]); // Leading proton azimuthal angle in radians
//                double phi_p2 = atan2(pyf[Proton_2_ind_2p], pxf[Proton_2_ind_2p]); // Leading proton azimuthal angle in radians
//                double d_phi_p2 = phi_p1 - phi_p2; // In radians
//
//                double theta_p1 = acos(pzf[Proton_1_ind_2p] / P_p1_2p); // Leading proton scattering angle in radians
//                double theta_p2 = acos(pzf[Proton_2_ind_2p] / P_p2_2p); // Recoil proton scattering angle in radians
//
//                phi_p1_2p->Fill(phi_p1 * 180.0 / 3.14159265359);
//                phi_p2_2p->Fill(phi_p2 * 180.0 / 3.14159265359);
//                dphi_2p->Fill(d_phi_p2 * 180.0 / 3.14159265359);
//
//                theta_p1_2p->Fill(theta_p1 * 180.0 / 3.14159265359);
//                theta_p2_2p->Fill(theta_p2 * 180.0 / 3.14159265359);
//
//                if (qel == true) {
//                    E_cal_VS_theta_p1_QEL_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                    E_cal_VS_theta_p2_QEL_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                }
//            } else { // If Proton_2_ind_2p is the leading proton and Proton_1_ind_2p is the recoil
//
////                  Leading proton:
//                double P_L_2p = P_p2_2p;
//
////                  Recoil proton:
//                double P_R_2p = P_p1_2p;
//
//                P_L_hist_2p->Fill(P_L_2p);
//                P_R_hist_2p->Fill(P_R_2p);
//
//                double phi_p2 = atan2(pyf[Proton_1_ind_2p], pxf[Proton_1_ind_2p]); // Leading proton azimuthal angle in radians
//                double phi_p1 = atan2(pyf[Proton_2_ind_2p], pxf[Proton_2_ind_2p]); // Leading proton azimuthal angle in radians
//                double d_phi_p2 = phi_p1 - phi_p2; // In radians
//
//                double theta_p2 = acos(pzf[Proton_1_ind_2p] / P_p1_2p); // Leading proton scattering angle in radians
//                double theta_p1 = acos(pzf[Proton_2_ind_2p] / P_p2_2p); // Recoil proton scattering angle in radians
//
//                phi_p2_2p->Fill(phi_p1 * 180.0 / 3.14159265359);
//                phi_p1_2p->Fill(phi_p2 * 180.0 / 3.14159265359);
//                dphi_2p->Fill(d_phi_p2 * 180.0 / 3.14159265359);
//
//                theta_p1_2p->Fill(theta_p2 * 180.0 / 3.14159265359);
//                theta_p2_2p->Fill(theta_p1 * 180.0 / 3.14159265359);
//
//                if (qel == true) {
//                    E_cal_VS_theta_p2_QEL_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                    E_cal_VS_theta_p1_QEL_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                }
//            }
//
//            E_Trans_VS_q3_all_2p->Fill(q3, Ev - El);
//
//            P_lp_hist_2p->Fill(P_l_2p);
//
//            fsEl_2p->Fill(El);
//            theta_l_2p->Fill(Theta_l_2p);
//            phi_l_2p->Fill(atan2(pyl, pxl) * 180.0 / 3.14159265359);
//            fsEl_VS_theta_l_2p->Fill(Theta_l_2p, El);
//
//            E_Trans_all_ang_all_int_2p->Fill(Ev - El);
//
//            E_cal_VS_theta_l_all_int_2p->Fill(Theta_l_2p, E_cal_2p);
//            E_cal_VS_Q2_all_int_2p->Fill(Q2, E_cal_2p);
//            E_cal_VS_dtheta_all_int_2p->Fill(
//                    fabs(acos(pzf[Proton_1_ind_2p] / sqrt(pxf[Proton_1_ind_2p] * pxf[Proton_1_ind_2p] + pyf[Proton_1_ind_2p] * pyf[Proton_1_ind_2p] +
//                                                          pzf[Proton_1_ind_2p] * pzf[Proton_1_ind_2p])) -
//                         acos(pzf[Proton_2_ind_2p] / sqrt(pxf[Proton_2_ind_2p] * pxf[Proton_2_ind_2p] + pyf[Proton_2_ind_2p] * pyf[Proton_2_ind_2p] +
//                                                          pzf[Proton_2_ind_2p] * pzf[Proton_2_ind_2p]))) * 180.0 / 3.14159265359, E_cal_2p);
//
//            gamma_Lab_all_hist->Fill(cos(d_theta_2p));
//            gamma_Lab_all_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//            if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                E_Trans15_all_2p->Fill(Ev - El);
//            } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                E_Trans45_all_2p->Fill(Ev - El);
//            } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                E_Trans90_all_2p->Fill(Ev - El);
//            }
//
//            if (qel == true) {
//                gamma_Lab_QEL_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_QEL_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                    E_Trans15_QEL_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                    E_Trans45_QEL_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                    E_Trans90_QEL_2p->Fill(Ev - El);
//                }
//
//                E_cal_QEL_2p->Fill(E_cal_2p);
//                E_cal_VS_theta_l_QEL_2p->Fill(Theta_l_2p, E_cal_2p);
//                E_cal_VS_Q2_QEL_2p->Fill(Q2, E_cal_2p);
//                E_cal_VS_W_QEL_2p->Fill(W, E_cal_2p);
//                E_cal_VS_En_QEL_2p->Fill(En, E_cal_2p);
//                E_cal_VS_Pn_QEL_2p->Fill(sqrt(pxn * pxn + pyn * pyn + pzn * pzn), E_cal_2p);
//                E_cal_VS_Pn1_QEL_2p->Fill(P_L_2p, E_cal_2p);
//                E_cal_VS_Pn2_QEL_2p->Fill(P_R_2p, E_cal_2p);
//                E_cal_VS_dtheta_QEL_2p->Fill(
//                        fabs(acos(pzf[Proton_1_ind_2p] / sqrt(pxf[Proton_1_ind_2p] * pxf[Proton_1_ind_2p] + pyf[Proton_1_ind_2p] * pyf[Proton_1_ind_2p] +
//                                                              pzf[Proton_1_ind_2p] * pzf[Proton_1_ind_2p])) -
//                             acos(pzf[Proton_2_ind_2p] / sqrt(pxf[Proton_2_ind_2p] * pxf[Proton_2_ind_2p] +
//                                                              pyf[Proton_2_ind_2p] * pyf[Proton_2_ind_2p] + pzf[Proton_2_ind_2p] * pzf[Proton_2_ind_2p]))) * 180.0 /
//                        3.14159265359, El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272));
//
//                E_Trans_VS_q3_QEL_2p->Fill(q3, Ev - El);
//
//                fsEl_QEL_2p->Fill(El);
//            } else if (mec == true) {
//                gamma_Lab_MEC_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_MEC_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                    E_Trans15_MEC_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                    E_Trans45_MEC_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                    E_Trans90_MEC_2p->Fill(Ev - El);
//                }
//
//                E_cal_MEC_2p->Fill(E_cal_2p);
//
//                E_Trans_VS_q3_MEC_2p->Fill(q3, Ev - El);
//
//                fsEl_MEC_2p->Fill(El);
//            } else if (res == true) {
//                gamma_Lab_RES_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_RES_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                    E_Trans15_RES_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                    E_Trans45_RES_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                    E_Trans90_RES_2p->Fill(Ev - El);
//                }
//
//                E_cal_RES_2p->Fill(E_cal_2p);
//
//                fsEl_RES_2p->Fill(El);
//            } else if (dis == true) {
//                gamma_Lab_DIS_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_DIS_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                    E_Trans15_DIS_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                    E_Trans45_DIS_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                    E_Trans90_DIS_2p->Fill(Ev - El);
//                }
//
//                E_cal_DIS_2p->Fill(E_cal_2p);
//
//                fsEl_DIS_2p->Fill(El);
//            }
//        }
//    }
//}
//</editor-fold>

//TODO:
//<editor-fold desc="calc1n1p function">
//void calc1n1p(bool qel, bool mec, bool res, bool dis, bool calculate_1n1p, int nfp, int nfn, int nf, int length, int pdgf[15], int ProtonCounter_1n1p, int Proton_ind_1n1p,
//          int Neutron_ind_1n1p, Double_t pxl, Double_t pyl, Double_t pzl, Double_t pxf[15], Double_t pyf[15], Double_t pzf[15], Double_t Ef[15], TH1D *P_p_hist_1n1p,
//          TH1D *P_n_hist_1n1p, TH1D *phi_p_1n1p, TH1D *phi_n_1n1p, TH1D *dphi_1n1p, TH1D *theta_p_1n1p, TH1D *theta_n_1n1p, TH1D *dtheta_1n1p,
//          TH2D *E_cal_VS_theta_p_QEL_1n1p, TH2D *E_cal_VS_theta_n_QEL_1n1p, TH2D *E_Trans_VS_q3_all_1n1p, Double_t Ev, Double_t El, double q3, double q,
//          TH1D *P_l_hist_1n1p, TH1D *theta_l_1n1p, TH1D *phi_l_1n1p, Double_t Q2, Double_t W, Double_t En, Double_t pxn, Double_t pyn, Double_t pzn,
//          TH1D *fsEl_1n1p, TH2D *fsEl_VS_theta_l_1n1p, TH1D *E_Trans_all_ang_all_int_1n1p, TH1D *gamma_Lab_all_hist_weighted, TH1D *E_Trans15_all_1n1p,
//          TH1D *E_Trans45_all_1n1p, TH1D *E_Trans90_all_1n1p, TH1D *gamma_Lab_all_hist, TH1D *gamma_Lab_QEL_hist, TH1D *gamma_Lab_QEL_hist_weighted,
//          TH1D *E_Trans15_QEL_1n1p, TH1D *E_Trans45_QEL_1n1p, TH1D *E_Trans90_QEL_1n1p, TH1D *E_cal_QEL_1n1p, TH2D *E_cal_VS_theta_l_QEL_1n1p, TH2D *E_cal_VS_Q2_QEL_1n1p,
//          TH2D *E_cal_VS_W_QEL_1n1p, TH2D *E_cal_VS_En_QEL_1n1p, TH2D *E_cal_VS_Pn_QEL_1n1p, TH2D *E_cal_VS_P_p_QEL_1n1p, TH2D *E_cal_VS_P_n_QEL_1n1p,
//          TH2D *E_cal_VS_dtheta_QEL_1n1p, TH2D *E_Trans_VS_q3_QEL_1n1p, TH1D *fsEl_QEL_1n1p, TH1D *gamma_Lab_MEC_hist, TH1D *gamma_Lab_MEC_hist_weighted,
//          TH1D *E_Trans15_MEC_1n1p, TH1D *E_Trans45_MEC_1n1p, TH1D *E_Trans90_MEC_1n1p, TH1D *E_cal_MEC_1n1p, TH2D *E_Trans_VS_q3_MEC_1n1p, TH1D *fsEl_MEC_1n1p,
//          TH1D *gamma_Lab_RES_hist, TH1D *gamma_Lab_RES_hist_weighted, TH1D *E_Trans15_RES_1n1p, TH1D *E_Trans45_RES_1n1p, TH1D *E_Trans90_RES_1n1p, TH1D *E_cal_RES_1n1p,
//          TH1D *fsEl_RES_1n1p, TH1D *gamma_Lab_DIS_hist, TH1D *gamma_Lab_DIS_hist_weighted, TH1D *E_Trans15_DIS_1n1p, TH1D *E_Trans45_DIS_1n1p, TH1D *E_Trans90_DIS_1n1p,
//          TH1D *E_cal_DIS_1n1p, TH1D *fsEl_DIS_1n1p) {
//
//    if (calculate_1n1p && nfp == 1 && nfn == 1 && nf == 2) { // See if there 1FS p and n; and 2FS hadrons (1n1p)
//        for (int i = 0; i < length; i++) {
////            if (before_FSI_events) {
////                if ((TMath::Abs(resc[i]) == 14) && (pdgf[i] == 2212)) {
////                    Proton_ind_1n1p = i;
////                } else if ((TMath::Abs(resc[i]) == 14) && (pdgf[i] == 2112)) {
////                    Neutron_ind_1n1p = i;
////                }
////            } else {
//            if (pdgf[i] == 2212) {
//                Proton_ind_1n1p = i;
//            } else if (pdgf[i] == 2112) {
//                Neutron_ind_1n1p = i;
//            }
////            }
//        }
//
////          Proton momentum:
//        double P_p_1n1p = sqrt(
//                pxf[Proton_ind_1n1p] * pxf[Proton_ind_1n1p] + pyf[Proton_ind_1n1p] * pyf[Proton_ind_1n1p] + pzf[Proton_ind_1n1p] * pzf[Proton_ind_1n1p]);
//
////          Neutron momentum:
//        double P_n_1n1p = sqrt(
//                pxf[Neutron_ind_1n1p] * pxf[Neutron_ind_1n1p] + pyf[Neutron_ind_1n1p] * pyf[Neutron_ind_1n1p] + pzf[Neutron_ind_1n1p] * pzf[Neutron_ind_1n1p]);
//
////          Momentum cut to at least 300 [MeV/c] == 0.3 [GeV/c]:
//        if (P_p_1n1p >= 0.3 && P_n_1n1p >= 0.3) {
//
//            double E_cal_1n1p = El + (Ef[Proton_ind_1n1p] - 0.938272) + (Ef[Neutron_ind_1n1p] - 0.939565);
//            double Theta_l_1n1p = acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359;
//
//            double phi_p = atan2(pyf[Proton_ind_1n1p], pxf[Proton_ind_1n1p]) * 180.0 / 3.14159265359;
//            double phi_n = atan2(pyf[Neutron_ind_1n1p], pxf[Neutron_ind_1n1p]) * 180.0 / 3.14159265359;
//            double d_phi_1n1p = phi_p - phi_n; // In radians
//
//            double theta_p = acos(pzf[Proton_ind_1n1p] / sqrt(pxf[Proton_ind_1n1p] * pxf[Proton_ind_1n1p] + pyf[Proton_ind_1n1p] * pyf[Proton_ind_1n1p] +
//                                                              pzf[Proton_ind_1n1p] * pzf[Proton_ind_1n1p])) * 180.0 / 3.14159265359;
//            double theta_n = acos(pzf[Neutron_ind_1n1p] / sqrt(pxf[Neutron_ind_1n1p] * pxf[Neutron_ind_1n1p] + pyf[Neutron_ind_1n1p] * pyf[Neutron_ind_1n1p] +
//                                                               pzf[Neutron_ind_1n1p] * pzf[Neutron_ind_1n1p])) * 180.0 / 3.14159265359;
//
////              NOT REALLY dtheta:
//            double d_theta_1n1p = acos(
//                    (pxf[Proton_ind_1n1p] * pxf[Neutron_ind_1n1p] + pyf[Proton_ind_1n1p] * pyf[Neutron_ind_1n1p] + pzf[Proton_ind_1n1p] * pzf[Neutron_ind_1n1p]) /
//                    (P_p_1n1p * P_n_1n1p));
//
////              Lepton momentum:
//            double P_l_1n1p = sqrt(pxl * pxl + pyl * pyl + pzl * pzl);
//
//            E_Trans_VS_q3_all_1n1p->Fill(q3, Ev - El);
//
//            P_p_hist_1n1p->Fill(P_p_1n1p);
//            P_n_hist_1n1p->Fill(P_n_1n1p);
//            P_l_hist_1n1p->Fill(P_l_1n1p);
//
//            phi_p_1n1p->Fill(phi_p);
//            theta_p_1n1p->Fill(theta_p);
//            phi_n_1n1p->Fill(phi_n);
//            theta_n_1n1p->Fill(theta_n);
//            dphi_1n1p->Fill(d_phi_1n1p * 180.0 / 3.14159265359);
//            dtheta_1n1p->Fill(d_theta_1n1p * 180.0 / 3.14159265359);
//
//            theta_l_1n1p->Fill(Theta_l_1n1p);
//            phi_l_1n1p->Fill(atan2(pyl, pxl) * 180.0 / 3.14159265359);
//            fsEl_VS_theta_l_1n1p->Fill(Theta_l_1n1p, El);
//            fsEl_1n1p->Fill(El);
//
//            E_Trans_all_ang_all_int_1n1p->Fill(Ev - El);
//
//            if (Theta_l_1n1p >= 14.0 && Theta_l_1n1p <= 16.0) {
//                E_Trans15_all_1n1p->Fill(Ev - El);
//            } else if (Theta_l_1n1p >= 44.0 && Theta_l_1n1p <= 46.0) {
//                E_Trans45_all_1n1p->Fill(Ev - El);
//            } else if (Theta_l_1n1p >= 89.0 && Theta_l_1n1p <= 91.0) {
//                E_Trans90_all_1n1p->Fill(Ev - El);
//            }
//
//            if (qel == true) {
//                if (Theta_l_1n1p >= 14.0 && Theta_l_1n1p <= 16.0) {
//                    E_Trans15_QEL_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 44.0 && Theta_l_1n1p <= 46.0) {
//                    E_Trans45_QEL_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 89.0 && Theta_l_1n1p <= 91.0) {
//                    E_Trans90_QEL_1n1p->Fill(Ev - El);
//                }
//
//                E_cal_QEL_1n1p->Fill(E_cal_1n1p);
//                E_cal_VS_theta_l_QEL_1n1p->Fill(Theta_l_1n1p, E_cal_1n1p);
//                E_cal_VS_Q2_QEL_1n1p->Fill(Q2, E_cal_1n1p);
//                E_cal_VS_dtheta_QEL_1n1p->Fill(d_theta_1n1p, E_cal_1n1p);
//                E_cal_VS_theta_p_QEL_1n1p->Fill(theta_p, E_cal_1n1p);
//                E_cal_VS_theta_n_QEL_1n1p->Fill(theta_n, E_cal_1n1p);
//                E_cal_VS_W_QEL_1n1p->Fill(W, E_cal_1n1p);
//                E_cal_VS_En_QEL_1n1p->Fill(En, E_cal_1n1p);
//                E_cal_VS_Pn_QEL_1n1p->Fill(sqrt(pxn * pxn + pyn * pyn + pzn * pzn), E_cal_1n1p);
//                E_cal_VS_P_n_QEL_1n1p->Fill(P_p_1n1p, E_cal_1n1p);
//                E_cal_VS_P_p_QEL_1n1p->Fill(P_n_1n1p, E_cal_1n1p);
//
//                E_Trans_VS_q3_QEL_1n1p->Fill(q3, Ev - El);
//
//                fsEl_QEL_1n1p->Fill(El);
//            } else if (mec == true) {
//                if (Theta_l_1n1p >= 14.0 && Theta_l_1n1p <= 16.0) {
//                    E_Trans15_MEC_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 44.0 && Theta_l_1n1p <= 46.0) {
//                    E_Trans45_MEC_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 89.0 && Theta_l_1n1p <= 91.0) {
//                    E_Trans90_MEC_1n1p->Fill(Ev - El);
//                }
//
//                E_cal_MEC_1n1p->Fill(E_cal_1n1p);
//
//                E_Trans_VS_q3_MEC_1n1p->Fill(q3, Ev - El);
//
//                fsEl_MEC_1n1p->Fill(El);
//            } else if (res == true) {
//                if (Theta_l_1n1p >= 14.0 && Theta_l_1n1p <= 16.0) {
//                    E_Trans15_RES_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 44.0 && Theta_l_1n1p <= 46.0) {
//                    E_Trans45_RES_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 89.0 && Theta_l_1n1p <= 91.0) {
//                    E_Trans90_RES_1n1p->Fill(Ev - El);
//                }
//
//                E_cal_RES_1n1p->Fill(E_cal_1n1p);
//
//                fsEl_RES_1n1p->Fill(El);
//            } else if (dis == true) {
//                if (Theta_l_1n1p >= 14.0 && Theta_l_1n1p <= 16.0) {
//                    E_Trans15_DIS_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 44.0 && Theta_l_1n1p <= 46.0) {
//                    E_Trans45_DIS_1n1p->Fill(Ev - El);
//                } else if (Theta_l_1n1p >= 89.0 && Theta_l_1n1p <= 91.0) {
//                    E_Trans90_DIS_1n1p->Fill(Ev - El);
//                }
//
//                E_cal_DIS_1n1p->Fill(E_cal_1n1p);
//
//                fsEl_DIS_1n1p->Fill(El);
//            }
//        }
//    }
//}


//</editor-fold>

//</editor-fold>


// file & path definitions
// ======================================================================================================================================================================

// path definitions -----------------------------------------------------------------------------------------------------------------------------------------------------

std::string plots_path = "./plots/";
std::string plots_file_type = "_plots.root";

// file_name definitions and selection ----------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="file_name definitions">

//<editor-fold desc="Local files">

// General file ---------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Local General files">
std::string file_name = "general_file";
std::string AnalyseFile = "/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo";
//std::string AnalyseFile = "/home/alon/Analyser/asportes_11_1000060120_2222_wfsi_SuSAv2_fixed_seed_555_test_19.gst.root";
//</editor-fold>

//</editor-fold>

// Files from GENIE gpvm ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Files from GENIE gpvm">
//TODO: Implement a mechanism that selects file_name automatically when running on gpvm
//std::string file_name = "GENIE_with_fsi";
//std::string file_name = "GENIE_no_fsi";
//</editor-fold>

//</editor-fold>


// BEnergyToNucleus definition ------------------------------------------------------------------------------------------------------------------------------------------
// ======================================================================================================================================================================

double BEnergyToNucleus;

// Simulation parameters extraction -------------------------------------------------------------------------------------------------------------------------------------
// ======================================================================================================================================================================

int Target_pdg, Probe_pdg;
std::string Target_nucleus, Probe;
double BeamEnergy;


// Histogram ranges
// ======================================================================================================================================================================

// Range variables definitions ------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Histogram range variables">

//<editor-fold desc="Theta histograms">

//<editor-fold desc="Theta histograms (2p)">
double theta_l_upper_lim_2p, theta_l_lower_lim_2p;
double theta_p1_upper_lim_2p, theta_p1_lower_lim_2p;
double theta_p2_upper_lim_2p, theta_p2_lower_lim_2p;
double dtheta_upper_lim_2p, dtheta_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Theta histograms (1n1p)">
double theta_l_upper_lim_1n1p, theta_l_lower_lim_1n1p;
double theta_p_upper_lim_1n1p, theta_p_lower_lim_1n1p;
double theta_n_upper_lim_1n1p, theta_n_lower_lim_1n1p;
double dtheta_upper_lim_1n1p, dtheta_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Phi histograms">

//<editor-fold desc="Phi histograms (2p)">
double phi_l_upper_lim_2p, phi_l_lower_lim_2p;
double phi_p1_upper_lim_2p, phi_p1_lower_lim_2p;
double phi_p2_upper_lim_2p, phi_p2_lower_lim_2p;
double dphi_upper_lim_2p, dphi_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Phi histograms (1n1p)">
double phi_l_upper_lim_1n1p, phi_l_lower_lim_1n1p;
double phi_p_upper_lim_1n1p, phi_p_lower_lim_1n1p;
double phi_n_upper_lim_1n1p, phi_n_lower_lim_1n1p;
double dphi_upper_lim_1n1p, dphi_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy histograms">

//<editor-fold desc="Energy histograms (2p)">
double fsEl_upper_lim_2p, fsEl_lower_lim_2p;
double fsEl_QEL_upper_lim_2p, fsEl_QEL_lower_lim_2p;
double fsEl_MEC_upper_lim_2p, fsEl_MEC_lower_lim_2p;
double fsEl_RES_upper_lim_2p, fsEl_RES_lower_lim_2p;
double fsEl_DIS_upper_lim_2p, fsEl_DIS_lower_lim_2p;
double fsEl_VS_theta_l_upper_lim_2p_x, fsEl_VS_theta_l_lower_lim_2p_x;
double fsEl_VS_theta_l_upper_lim_2p_y, fsEl_VS_theta_l_lower_lim_2p_y;
//</editor-fold>

//<editor-fold desc="Energy histograms (1n1p)">
double fsEl_upper_lim_1n1p, fsEl_lower_lim_1n1p;
double fsEl_QEL_upper_lim_1n1p, fsEl_QEL_lower_lim_1n1p;
double fsEl_MEC_upper_lim_1n1p, fsEl_MEC_lower_lim_1n1p;
double fsEl_RES_upper_lim_1n1p, fsEl_RES_lower_lim_1n1p;
double fsEl_DIS_upper_lim_1n1p, fsEl_DIS_lower_lim_1n1p;
double fsEl_VS_theta_l_upper_lim_1n1p_x, fsEl_VS_theta_l_lower_lim_1n1p_x;
double fsEl_VS_theta_l_upper_lim_1n1p_y, fsEl_VS_theta_l_lower_lim_1n1p_y;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms">

//<editor-fold desc="Energy Transfer histograms (all interactions)">

//<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
double E_Trans_all_ang_all_int_upper_lim_2p;
double E_Trans_all_ang_all_int_lower_lim_2p;
double E_Trans15_all_upper_lim_2p;
double E_Trans15_all_lower_lim_2p;
double E_Trans45_all_upper_lim_2p;
double E_Trans45_all_lower_lim_2p;
double E_Trans90_all_upper_lim_2p;
double E_Trans90_all_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
double E_Trans_all_ang_all_int_upper_lim_1n1p;
double E_Trans_all_ang_all_int_lower_lim_1n1p;
double E_Trans15_all_upper_lim_1n1p;
double E_Trans15_all_lower_lim_1n1p;
double E_Trans45_all_upper_lim_1n1p;
double E_Trans45_all_lower_lim_1n1p;
double E_Trans90_all_upper_lim_1n1p;
double E_Trans90_all_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only)">

//<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
double E_Trans15_QEL_upper_lim_2p;
double E_Trans15_QEL_lower_lim_2p;
double E_Trans45_QEL_upper_lim_2p;
double E_Trans45_QEL_lower_lim_2p;
double E_Trans90_QEL_upper_lim_2p;
double E_Trans90_QEL_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
double E_Trans15_QEL_upper_lim_1n1p;
double E_Trans15_QEL_lower_lim_1n1p;
double E_Trans45_QEL_upper_lim_1n1p;
double E_Trans45_QEL_lower_lim_1n1p;
double E_Trans90_QEL_upper_lim_1n1p;
double E_Trans90_QEL_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only)">

//<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
double E_Trans15_MEC_upper_lim_2p;
double E_Trans15_MEC_lower_lim_2p;
double E_Trans45_MEC_upper_lim_2p;
double E_Trans45_MEC_lower_lim_2p;
double E_Trans90_MEC_upper_lim_2p;
double E_Trans90_MEC_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
double E_Trans15_MEC_upper_lim_1n1p;
double E_Trans15_MEC_lower_lim_1n1p;
double E_Trans45_MEC_upper_lim_1n1p;
double E_Trans45_MEC_lower_lim_1n1p;
double E_Trans90_MEC_upper_lim_1n1p;
double E_Trans90_MEC_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only)">

//<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
double E_Trans15_RES_upper_lim_2p;
double E_Trans15_RES_lower_lim_2p;
double E_Trans45_RES_upper_lim_2p;
double E_Trans45_RES_lower_lim_2p;
double E_Trans90_RES_upper_lim_2p;
double E_Trans90_RES_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
double E_Trans15_RES_upper_lim_1n1p;
double E_Trans15_RES_lower_lim_1n1p;
double E_Trans45_RES_upper_lim_1n1p;
double E_Trans45_RES_lower_lim_1n1p;
double E_Trans90_RES_upper_lim_1n1p;
double E_Trans90_RES_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only)">

//<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
double E_Trans15_DIS_upper_lim_2p;
double E_Trans15_DIS_lower_lim_2p;
double E_Trans45_DIS_upper_lim_2p;
double E_Trans45_DIS_lower_lim_2p;
double E_Trans90_DIS_upper_lim_2p;
double E_Trans90_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
double E_Trans15_DIS_upper_lim_1n1p;
double E_Trans15_DIS_lower_lim_1n1p;
double E_Trans45_DIS_upper_lim_1n1p;
double E_Trans45_DIS_lower_lim_1n1p;
double E_Trans90_DIS_upper_lim_1n1p;
double E_Trans90_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//</editor-fold>

//<editor-fold desc="Inclusive Energy Transfer histograms">
double E_Trans15_all_inclusive_upper_lim;
double E_Trans15_all_inclusive_lower_lim;
double E_Trans15_QEL_inclusive_upper_lim;
double E_Trans15_QEL_inclusive_lower_lim;
double E_Trans15_MEC_inclusive_upper_lim;
double E_Trans15_MEC_inclusive_lower_lim;
double E_Trans15_RES_inclusive_upper_lim;
double E_Trans15_RES_inclusive_lower_lim;
double E_Trans15_DIS_inclusive_upper_lim;
double E_Trans15_DIS_inclusive_lower_lim;
double E_Trans_VS_q_all_inclusive_upper_lim_x;
double E_Trans_VS_q_all_inclusive_lower_lim_x;
double E_Trans_VS_q_all_inclusive_upper_lim_y;
double E_Trans_VS_q_all_inclusive_lower_lim_y;
double E_Trans_VS_q_QEL_inclusive_upper_lim_x;
double E_Trans_VS_q_QEL_inclusive_lower_lim_x;
double E_Trans_VS_q_QEL_inclusive_upper_lim_y;
double E_Trans_VS_q_QEL_inclusive_lower_lim_y;
double E_Trans_VS_q_MEC_inclusive_upper_lim_x;
double E_Trans_VS_q_MEC_inclusive_lower_lim_x;
double E_Trans_VS_q_MEC_inclusive_upper_lim_y;
double E_Trans_VS_q_MEC_inclusive_lower_lim_y;
double E_Trans_VS_q3_all_upper_lim_x_2p;
double E_Trans_VS_q3_all_lower_lim_x_2p;
double E_Trans_VS_q3_all_upper_lim_y_2p;
double E_Trans_VS_q3_all_lower_lim_y_2p;
double E_Trans_VS_q3_QEL_upper_lim_x_2p;
double E_Trans_VS_q3_QEL_lower_lim_x_2p;
double E_Trans_VS_q3_QEL_upper_lim_y_2p;
double E_Trans_VS_q3_QEL_lower_lim_y_2p;
double E_Trans_VS_q3_MEC_upper_lim_x_2p;
double E_Trans_VS_q3_MEC_lower_lim_x_2p;
double E_Trans_VS_q3_MEC_upper_lim_y_2p;
double E_Trans_VS_q3_MEC_lower_lim_y_2p;
double E_Trans_VS_q3_all_upper_lim_x_1n1p;
double E_Trans_VS_q3_all_lower_lim_x_1n1p;
double E_Trans_VS_q3_all_upper_lim_y_1n1p;
double E_Trans_VS_q3_all_lower_lim_y_1n1p;
double E_Trans_VS_q3_QEL_upper_lim_x_1n1p;
double E_Trans_VS_q3_QEL_lower_lim_x_1n1p;
double E_Trans_VS_q3_QEL_upper_lim_y_1n1p;
double E_Trans_VS_q3_QEL_lower_lim_y_1n1p;
double E_Trans_VS_q3_MEC_upper_lim_x_1n1p;
double E_Trans_VS_q3_MEC_lower_lim_x_1n1p;
double E_Trans_VS_q3_MEC_upper_lim_y_1n1p;
double E_Trans_VS_q3_MEC_lower_lim_y_1n1p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms">

//<editor-fold desc="E_cal range">
double E_cal_QEL_upper_lim_range;
double E_cal_QEL_lower_lim_range;
double E_cal_MEC_upper_lim_range;
double E_cal_MEC_lower_lim_range;
double E_cal_RES_upper_lim_range;
double E_cal_RES_lower_lim_range;
double E_cal_DIS_upper_lim_range;
double E_cal_DIS_lower_lim_range;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (2p)">
double E_cal_QEL_upper_lim_2p, E_cal_QEL_lower_lim_2p;
double E_cal_MEC_upper_lim_2p, E_cal_MEC_lower_lim_2p;
double E_cal_RES_upper_lim_2p, E_cal_RES_lower_lim_2p;
double E_cal_DIS_upper_lim_2p, E_cal_DIS_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="E_cal restoration histograms (1n1p)">
double E_cal_QEL_upper_lim_1n1p, E_cal_QEL_lower_lim_1n1p;
double E_cal_MEC_upper_lim_1n1p, E_cal_MEC_lower_lim_1n1p;
double E_cal_RES_upper_lim_1n1p, E_cal_RES_lower_lim_1n1p;
double E_cal_DIS_upper_lim_1n1p, E_cal_DIS_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="Momentum histograms">

//<editor-fold desc="Momentum histograms (2p)">
double P_L_hist_upper_lim_2p, P_L_hist_lower_lim_2p;
double P_R_hist_upper_lim_2p, P_R_hist_lower_lim_2p;
double P_lp_hist_upper_lim_2p, P_lp_hist_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="Momentum histograms (1n1p)">
double P_p_hist_upper_lim_1n1p, P_p_hist_lower_lim_1n1p;
double P_n_hist_upper_lim_1n1p, P_n_hist_lower_lim_1n1p;
double P_l_hist_upper_lim_1n1p, P_l_hist_lower_lim_1n1p;
//</editor-fold>

//</editor-fold>

//<editor-fold desc="MicroBooNE plots">

//<editor-fold desc="MicroBooNE gamma plots (unweighted)">
double gamma_Lab_hist_upper_lim_2p, gamma_Lab_hist_lower_lim_2p;
double gamma_mu_p_tot_upper_lim_2p, gamma_mu_p_tot_lower_lim_2p;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
double gamma_Lab_hist_weighted_upper_lim_1n1p, gamma_Lab_hist_weighted_lower_lim_1n1p;
double gamma_mu_p_tot_weighted_upper_lim_1n1p, gamma_mu_p_tot_weighted_lower_lim_1n1p;
//</editor-fold>

//<editor-fold desc="MicroBooNE gamma plots (no pions, for every interaction)">
double gamma_Lab_all_hist_upper_lim, gamma_Lab_all_hist_lower_lim;
double gamma_Lab_all_hist_weighted_upper_lim, gamma_Lab_all_hist_weighted_lower_lim;
double gamma_Lab_QEL_hist_upper_lim, gamma_Lab_QEL_hist_lower_lim;
double gamma_Lab_QEL_hist_weighted_upper_lim, gamma_Lab_QEL_hist_weighted_lower_lim;
double gamma_Lab_MEC_hist_upper_lim, gamma_Lab_MEC_hist_lower_lim;
double gamma_Lab_MEC_hist_weighted_upper_lim, gamma_Lab_MEC_hist_weighted_lower_lim;
double gamma_Lab_RES_hist_upper_lim, gamma_Lab_RES_hist_lower_lim;
double gamma_Lab_RES_hist_weighted_upper_lim, gamma_Lab_RES_hist_weighted_lower_lim;
double gamma_Lab_DIS_hist_upper_lim, gamma_Lab_DIS_hist_lower_lim;
double gamma_Lab_DIS_hist_weighted_upper_lim, gamma_Lab_DIS_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE dP_T plots">
double dP_T_hist_upper_lim, dP_T_hist_lower_lim;
double dP_T_hist_weighted_upper_lim, dP_T_hist_weighted_lower_lim;
//</editor-fold>

//<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
double P_L_hist_upper_lim, P_L_hist_lower_lim;
double P_R_hist_upper_lim, P_R_hist_lower_lim;
double P_l_hist_upper_lim, P_l_hist_lower_lim;
double P_pion_hist_upper_lim, P_pion_hist_lower_lim;
//</editor-fold>

//</editor-fold>

//</editor-fold>

#endif //ANALYSER_CODESETUP_H
