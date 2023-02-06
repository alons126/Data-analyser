//
// Created by alons on 27/01/2023.
//

#ifndef DETECTORSIMULATIONANALYSER_C_DETECTORSIMULATIONCUTS_H
#define DETECTORSIMULATIONANALYSER_C_DETECTORSIMULATIONCUTS_H

//TODO: confirm that turning all the cuts switchs off restores the before plots

// ======================================================================================================================================================================
// Detector simulation cuts
// ======================================================================================================================================================================

// Momentum -------------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Momentum">
bool apply_momentum_cuts_2p = true, apply_momentum_cuts_1n1p = true, apply_momentum_cuts_MicroBooNE = true;

// 2p momentum cuts:
double e_momentum_upper_cut_2p = -1, e_momentum_lower_cut_2p = -1;
double p_momentum_upper_cut_2p = -1, p_momentum_lower_cut_2p = 0.3;

// MicroBooNE momentum cuts:
double e_momentum_upper_cut_MicroBooNE = 1.2, e_momentum_lower_cut_MicroBooNE = 0.1;
double p_momentum_upper_cut_MicroBooNE = 1, p_momentum_lower_cut_MicroBooNE = 0.3;
double cpion_momentum_upper_cut_MicroBooNE = 0.065, cpion_momentum_lower_cut_MicroBooNE = -1;
//</editor-fold>

// Sampling Fraction (SF) cuts (electrons only, FD) ---------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Sampling Fraction (SF) cuts (electrons only, FD)">
bool apply_SF_cuts = true;

double SF_1e2p_upper_cut = 0.28;
double SF_1e2p_lower_cut = 0.18;
double SF_1e2p_Xmax; // for all e plots, no cuts applied
double SF_1e2p_peak = 0.25; // to fill using Chi2_Electron_1e_Xmax_CD
//</editor-fold>

// Fiducial cuts --------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Fiducial cuts">
bool apply_Lv_cut = true, apply_Lw_cut = true;

double fiducial_cut_Lv = 14;
double fiducial_cut_Lw = 14;
//</editor-fold>

// Number of Photo-electrons (nphe) cuts (electrons only, FD) -----------------------------------------------------------------------------------------------------------

//<editor-fold desc="Number of Photo-electrons (nphe) cuts (electrons only, FD)">
bool apply_nphe_cut = true;

double nphe_lower_cut = 2;
//</editor-fold>

// Chi2 cuts ------------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Chi2 cuts">
bool apply_chi2_cuts = true;

//<editor-fold desc="Electron chi2 cuts">
double Chi2_Electron_cut_CD = 15.; // 100 since electron detection is great
double Chi2_Electron_Xmax_CD; // for all e plots, no cuts applied
double Chi2_Electron_1e_peak_CD = 0; // to fill using Chi2_Electron_1e_Xmax_CD
double Chi2_Electron_1e_Xmax_CD;

double Chi2_Electron_cut_FD = 15.; // 100 since electron detection is great
double Chi2_Electron_Xmax_FD; // for all e plots, no cuts applied
double Chi2_Electron_1e_peak_FD = -0.05; // to fill using Chi2_Electron_1e_Xmax_FD
double Chi2_Electron_1e_Xmax_FD;
//</editor-fold>

//<editor-fold desc="Proton chi2 cuts">
double Chi2_Proton_cut_CD = 10.; // Josh's proton FD cut
double Chi2_Proton_Xmax_CD; // for all e plots, no cuts applied
double Chi2_Proton_1e_peak_CD = 0.45; // to fill using Chi2_Proton_1e_Xmax_CD
double Chi2_Proton_1e_Xmax_CD;

double Chi2_Proton_cut_FD = 3.; // Josh's proton FD cut
double Chi2_Proton_Xmax_FD; // for all e plots, no cuts applied
double Chi2_Proton_1e_peak_FD = 0.05; // to fill using Chi2_Proton_1e_Xmax_FD
double Chi2_Proton_1e_Xmax_FD;
//</editor-fold>

//    //<editor-fold desc="Neutron chi2 cuts">
//    double Chi2_Neutron_cut_CD = 3.;
//    double Chi2_Neutron_cut_FD = 3.;
////    double Chi2_Neutron_cut_CD = 1.;
////    double Chi2_Neutron_cut_FD = 1.;
//    //</editor-fold>

//</editor-fold>

// Vertex cuts ----------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Vertex cuts">
bool apply_dVz_cuts = true;

double Vertex_Electron_cut_CD = 3.;
double Vertex_Electron_1e_mean_CD = 0; // 1e mean
double Vertex_Electron_cut_FD = 3.;
double Vertex_Electron_1e_mean_FD = -0.8841; // 1e mean

double Vertex_Proton_cut_CD = 3.;
double Vertex_Proton_1e_mean_CD = 0.698; // 1e mean
double Vertex_Proton_cut_FD = 3.;
double Vertex_Proton_1e_mean_FD = -0.0521; // 1e mean

double dVx_Xmax_CD;
double dVx_cut_CD = 4., dVx_peak_CD = -0.01; // dVx_peak_CD to be reset by dVx_Xmax_CD in second run
double dVy_Xmax_CD;
double dVy_cut_CD = 4., dVy_peak_CD = 0.01; // dVy_peak_CD to be reset by dVy_Xmax_CD in second run
double dVz_Xmax_CD;
double dVz_cut_CD = 4., dVz_peak_CD = 0.51; // dVz_peak_CD to be reset by dVz_Xmax_CD in second run

double dVx_Xmax_FD;
double dVx_cut_FD = 4., dVx_peak_FD = -0.03; // dVx_peak_FD to be reset by dVx_Xmax_FD in second run
double dVy_Xmax_FD;
double dVy_cut_FD = 4., dVy_peak_FD = -0.03; // dVy_peak_FD to be reset by dVy_Xmax_FD in second run
double dVz_Xmax_FD;
double dVz_cut_FD = 4., dVz_peak_FD = -0.27; // dVz_peak_FD to be reset by dVz_Xmax_FD in second run

double dVx_Xmax;
double dVx_cut = 4., dVx_peak = -0.01; // dVx_peak to be reset by dVx_Xmax in second run
double dVy_Xmax;
double dVy_cut = 4., dVy_peak = -0.01; // dVy_peak to be reset by dVy_Xmax in second run
double dVz_Xmax;
double dVz_cut = 4., dVz_peak = -0.09; // dVz_peak to be reset by dVz_Xmax in second run

//    //<editor-fold desc="Neutron Vertex cut">
//    double Vertex_Neutron_cut_CD = 3.;
//    double Vertex_Neutron_cut_FD = 3.;
////    double Vertex_Neutron_cut_CD = 1.;
////    double Vertex_Neutron_cut_FD = 1.;
//    //</editor-fold>

//</editor-fold>

#endif //DETECTORSIMULATIONANALYSER_C_DETECTORSIMULATIONCUTS_H
