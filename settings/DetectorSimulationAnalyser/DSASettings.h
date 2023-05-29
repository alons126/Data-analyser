//
// Created by alons on 23/05/2023.
//

#ifndef DSASETTINGS_H
#define DSASETTINGS_H

#include <iostream>

#include "../codeSetup.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code settings                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//<editor-fold desc="Code settings">

// ======================================================================================================================================================================
// Event selection settings -------------------------------------------------------------------------------------------------------------------------------------------------
// ======================================================================================================================================================================

//<editor-fold desc="Event selection settings">
/* Settings to enable/disable specific FS plot calculations (Rec only): */

bool calculate_truth_level;

bool calculate_1p, calculate_1n;
bool calculate_2p, calculate_pFDpCD, calculate_nFDpCD;

bool Rec_wTL_ES; // Enforce TL event selection on Rec. plots
//
bool Enable_FD_photons; // Enforce TL event selection on Rec. plots
//</editor-fold>

// ======================================================================================================================================================================
// Input processing
// ======================================================================================================================================================================

//<editor-fold desc="Input processing">
/* Initial input processing of loaded files (given by AnalyseFile) */

/* Determine file path and name */
string LoadedInput ; // AnalyseFile is taken from codeSetup.h
string filePath;
string fileInput;
string plotsInput;

/* Configure and get run parameters */
double beamE; // Configure beam energy from SampleName
string Target; // Configure target (element) from SampleName
int TargetPDG; // Configure target PDG from SampleName
//</editor-fold>

// ======================================================================================================================================================================
// Cut settings
// ======================================================================================================================================================================

// Cut settings ---------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Cuts settings">
/* Settings that allow to disable/enable every cut individually */

// clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

//TODO: add beta = 1.2 cut for electrons
bool apply_cuts; // master ON/OFF switch for applying cuts

/* HTCC cut */
bool apply_Nphe_cut ;

/* Chi2 cuts (= PID cuts) */
bool apply_chi2_cuts_1e_cut ;

/* Vertex cuts */
bool apply_Vz_cuts , apply_dVz_cuts ;

/* Sampling Fraction (SF) cut */
bool apply_ECAL_SF_cuts ;
bool apply_ECAL_P_cuts ;

/* ECAL fiducial (edge) cuts */
bool apply_ECAL_fiducial_cuts ;

/* DC fiducial (edge) cuts */
bool apply_DC_fiducial_cut ;

/* Momentum cuts */
bool apply_momentum_cuts_1p , apply_momentum_cuts_1n ;
bool apply_momentum_cuts_2p , apply_momentum_cuts_pFDpCD , apply_momentum_cuts_nFDpCD ;

bool apply_nucleon_cuts ;

//<editor-fold desc="Custom cuts naming & print out execution variables">

//<editor-fold desc="Custom cuts naming">
/* Save plots to custom-named folders, to allow multi-sample runs at once. */

bool custom_cuts_naming ;
string Beta_Fit_Status, FD_photons_Status, Efficiency_Status;
//</editor-fold>

//</editor-fold>

//</editor-fold>

// Cut declarations -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Cuts declarations">
/* Log cut values to be used later when applying them. */

// clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="clas12ana cuts">
/* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
DSCuts Nphe_cuts_FD;

/* Chi2 cuts. NOTES:
 * Values for mean and sigma are filled from fit variables (overating these values later).
 * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */
DSCuts Chi2_Electron_cuts_CD;
DSCuts Chi2_Electron_cuts_FD;

DSCuts Chi2_Proton_cuts_CD;
DSCuts Chi2_Proton_cuts_FD;

DSCuts Chi2_piplus_cuts_CD;
DSCuts Chi2_piplus_cuts_FD;

DSCuts Chi2_piminus_cuts_CD;
DSCuts Chi2_piminus_cuts_FD;

//<editor-fold desc="Kaon PID cuts">
/* Kaon pid cuts. Not really applied in our analysis. */
DSCuts Chi2_Kplus_cuts_CD;
DSCuts Chi2_Kplus_cuts_FD;
DSCuts Chi2_Kminus_cuts_CD;
DSCuts Chi2_Kminus_cuts_FD;
//</editor-fold>

/* Vertex cuts */
DSCuts Vz_cut;
DSCuts dVz_cuts;

/* Sampling Fraction (SF) cuts (electrons only, FD) */
DSCuts SF_cuts;
//    DSCuts P_cuts;

/* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
DSCuts PCAL_edge_cuts;

/* DC edge cuts (fiducial cuts ,electrons only, FD) */
DSCuts DC_edge_cuts;

/* Momentum thresholds */
//    DSCuts e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
//    DSCuts p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
//    DSCuts pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, -9999, 9999);
//    DSCuts pim_mom_th = DSCuts("Momentum_th", "", "Piminus", "", 0, -9999, 9999);
//    DSCuts ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, -9999, 9999);
//    DSCuts n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, -9999, 9999);
DSCuts e_mom_th;
DSCuts p_mom_th;
DSCuts pip_mom_th;
DSCuts pim_mom_th;
DSCuts ph_mom_th;
DSCuts n_mom_th;
//</editor-fold>

// Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Neutron momentum cuts (1n, FD only)">
/* All sectors */
DSCuts n_momentum_cuts_ABF_FD_n_from_ph; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta.

DSCuts n_momentum_cuts_ABF_FD_All_neutrals; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_All_neutrals_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_noPCAL; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_noPCAL_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_noPCAL_wECIN; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_noPCAL_wECIN_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.

/* Specific sectors */
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S1; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 1).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S1_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 1).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S2; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 2).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S2_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 2).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S3; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 3).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S3_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 3).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S4; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 4).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S4_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 4).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S5; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 5).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S5_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 5).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S6; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 6).
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S6_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 6).
//</editor-fold>

//<editor-fold desc="Truth-level momentum cuts">
/* Momentum cuts */
DSCuts TL_e_mom_cuts;

DSCuts TL_n_mom_cuts;
DSCuts TL_p_mom_cuts;
DSCuts TL_pip_mom_cuts;
DSCuts TL_pim_mom_cuts;
DSCuts TL_pi0_mom_cuts;
DSCuts TL_ph_mom_cuts;
//</editor-fold>

/* FD theta range (1n & 1p) */
DSCuts ThetaFD;

/* Beta cut (1n, FD) */
DSCuts Beta_cut ;

DSCuts Beta_cut_ABF_FD_n_from_ph, Beta_cut_ABF_FD_n_from_ph_apprax; // All sectors
DSCuts Beta_cut_ABF_FD_n_from_ph_S1, Beta_cut_ABF_FD_n_from_ph_S1_apprax; // Sector 1
DSCuts Beta_cut_ABF_FD_n_from_ph_S2, Beta_cut_ABF_FD_n_from_ph_S2_apprax; // Sector 2
DSCuts Beta_cut_ABF_FD_n_from_ph_S3, Beta_cut_ABF_FD_n_from_ph_S3_apprax; // Sector 3
DSCuts Beta_cut_ABF_FD_n_from_ph_S4, Beta_cut_ABF_FD_n_from_ph_S4_apprax; // Sector 4
DSCuts Beta_cut_ABF_FD_n_from_ph_S5, Beta_cut_ABF_FD_n_from_ph_S5_apprax; // Sector 5
DSCuts Beta_cut_ABF_FD_n_from_ph_S6, Beta_cut_ABF_FD_n_from_ph_S6_apprax; // Sector 6

DSCuts Beta_cut_ABF_All_FD_neutrals, Beta_cut_ABF_FD_neutrals_noPDG0;
DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL;
DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL_wECIN, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_wECIN;
DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL_noECIN_wECOUT, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT;

/* Nucleon theta cut (1p & 1n, FD) */
DSCuts Theta_nuc_cut;

/* Neutron veto cut (1n & 1n1p, FD) */
DSCuts Neutron_veto_cut;

/* Ghost tracks handling (2p & pFDpCD, CD & FD) */
DSCuts Theta_p1_cuts_2p;
DSCuts Theta_p2_cuts_2p;
DSCuts dphi_p1_p2_2p;
DSCuts Theta_p1_cuts_pFDpCD;
DSCuts Theta_p2_cuts_pFDpCD;
DSCuts dphi_pFD_pCD_pFDpCD;
//</editor-fold>

// ======================================================================================================================================================================
// Other settings
// ======================================================================================================================================================================

// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="TList definition">
/* Definition of plots TList used to save all plots to .root file. */

TList *plots;
string listName = plots_path + "/" + AnalyseFileSample + plots_file_type;
const char *TListName = listName.c_str();
//</editor-fold>

//  Checking directories ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Creating directories">
/* Code for creating directories.
 * Directory creation is done in the Directories class.
 * Added for the case that plots out folder does not exist and for organization.
 * All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */

cout << "Creating plot directories...\n\n";

string Plots_Folder = plots_path;
Directories directories = Directories(Plots_Folder);
//</editor-fold>

// Plot settings --------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Plot selector">
/* Here are boolean variables used to turn ON/OFF the different plots of the code.
   Plot_selector_master must remain true, set it OFF only for debugging. */

/* Master plots variable */
bool Plot_selector_master = true; // Master plot selector for analysis

/* Cut variable plots */
bool Cut_plots_master = true; // Master cut plots selector
bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true, Momentum_plots = true;

/* Beta plots */
//    bool Beta_plots = true;
bool Beta_plots = false;
cout << "\n\n\n\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;";
cout << "\nbool Beta_plots = false;\n\n\n\n";
//    bool Beta_vs_P_plots = true;
bool Beta_vs_P_plots = false;
cout << "\n\n\n\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;";
cout << "\nbool Beta_vs_P_plots = false;\n\n\n\n";

/* Angle plots */
bool Angle_plots_master = true; // Master angle plots selector
bool Theta_e_plots = true, Phi_e_plots = true;
//    bool Angle_plots_master = false; // Master angle plots selector
//    bool Theta_e_plots = false, Phi_e_plots = false;
//    cout << "\n\n\n\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;\n\n\n\n";

/* Q2 plots */
//    bool Q2_plots = true;
bool Q2_plots = false;
cout << "\n\n\n\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;";
cout << "\nbool Q2_plots = false;\n\n\n\n";

/* E_e plots */
//    bool E_e_plots = true;
bool E_e_plots = false;
cout << "\n\n\n\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;";
cout << "\nbool E_e_plots = false;\n\n\n\n";

/* ET plots */
//    bool ETrans_plots_master = true; // Master ET plots selector
bool ETrans_plots_master = false; // Master ET plots selector
cout << "\n\n\n\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;";
cout << "\nbool ETrans_plots_master = false;\n\n\n\n";
bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

/* Ecal plots */
//    bool Ecal_plots = true;
bool Ecal_plots = false;
cout << "\n\n\n\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;";
cout << "\nbool Ecal_plots = false;\n\n\n\n";

/* Transverse variables plots */
//    bool TVariables_plots = true;
bool TVariables_plots = false;
cout << "\n\n\n\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;";
cout << "\nbool TVariables_plots = false;\n\n\n\n";

/* ToF plots */
bool ToF_plots = false;

/* Efficiency plots */
//    bool Efficiency_plots = true;
bool Efficiency_plots = false;
cout << "\n\n\n\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;";
cout << "\nbool Efficiency_plots = false;\n\n\n\n";

//<editor-fold desc="Turn off plots by master selectors">
if (!Plot_selector_master) {
Cut_plots_master = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Q2_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
TVariables_plots = ToF_plots = Efficiency_plots = false;
}

if (!Cut_plots_master) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }
if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }
if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }
//</editor-fold>

/* Other settings variables */
bool wider_margin = true;
bool debug_plots = true; // Print out clas12ana debugging plots
//</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Normalization settings">
/* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
 * Enable of presentations only, since event count is important otherwise. */

bool normalize_master = false;

bool norm_Nphe_plots = false, norm_Chi2_plots = false, norm_Vertex_plots = false, norm_SF_plots = false, norm_Fiducial_plots = false, norm_Momentum_plots = false;

bool norm_Beta_plots = false, norm_Angle_plots_master = false, norm_Q2_plots = false, norm_E_e_plots = false, norm_ET_plots = false, norm_Ecal_plots = false;
bool norm_TVariables_plots = false;

if (!normalize_master) { // Disable all normalizations if normalize_master == false
norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = norm_Momentum_plots = false;
norm_Angle_plots_master = norm_Q2_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TVariables_plots = false;

cout << "\nAll normalizations are disabled.\n\n";  // and no change to custom_FSI_status
}
//</editor-fold>

// Delete settings ------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Delete settings">
/* Clear files from previous runs (to prevent mix fo plots from different codes). */

bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

/* Delete existing .txt files */
if (delete_txt_files) { system(("find " + plots_path + " -type f -iname '*.txt' -delete").c_str()); }

//<editor-fold desc="Deleting files by cases">
if (delete_png_files && !delete_root_files) {
cout << "\nClearing old plots...";
system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
cout << " done.\n\n";
} else if (!delete_png_files && delete_root_files) {
cout << "\nClearing old root files...";
system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
cout << " done.\n\n";
} else if (delete_png_files && delete_root_files) {
cout << "\nClearing old plots & root files...";
system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
cout << " done.\n\n";
} else {
cout << "\nNo files were cleared.\n\n";
}
//</editor-fold>

//</editor-fold>

// Histogram limits -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Histogram boundaries">
/* Histogram boundary variables. Used to unify histograms to the same boundaries. */

/* Nphe plots */
double Nphe_boundary = 40;

/* Chi2 plots */
double Chi2_boundary = 30;
if (apply_cuts) { Chi2_boundary = 9; }

/* Vertex plots */
double Vertex_boundary = 50, Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;

if (apply_cuts) {
double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
Vertex_boundary = Vertex_boundary / 5;
}

double dV_boundary = 50;
if (apply_cuts) { /* dV_boundary = 7.5; */ dV_boundary = dVz_cuts.GetUpperCut() * 1.4; }

/* SF */
double SF_uboundary = 0.31, SF_lboundary = 0.16;

/* Momentum boundries */
double Momentum_lboundary = 0., Momentum_uboundary = beamE * 1.1;
//    double Momentum_uboundary = 20., Momentum_lboundary = 0.;

/* Beta plots */
double dBeta_sigma_boundary = 0.1;
double Beta_dist_uboundary = 1 + dBeta_sigma_boundary, Beta_dist_lboundary = 1 - dBeta_sigma_boundary;

double dBeta_sigma_ZOOMOUT_boundary = 0.1;
double Beta_dist_ZOOMOUT_uboundary = 1 + dBeta_sigma_ZOOMOUT_boundary;
double Beta_dist_ZOOMOUT_lboundary = 0.9;

double Beta_boundary = 3., P_boundary = beamE * 1.425;
if (apply_cuts) { Beta_boundary = 1.25, P_boundary = beamE * 1.1; }

/* Theta plots */
double Theta_lboundary_FD = 0., Theta_uboundary_FD = 50.;

/* Phi plots */
double Phi_lboundary_FD = -180., Phi_uboundary_FD = 180.;

/* Transverse variables */
double dP_T_boundary = 3.;
//</editor-fold>

// Debugging settings ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Debugging settings">
/* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */

bool PrintEvents = false;
int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;

ofstream EventPrint;
string EventPrint_save_Directory;

if (PrintEvents) {
if (!apply_chi2_cuts_1e_cut) {
EventPrint_save_Directory = plots_path + "/" + "Event_Print_without_chi2.txt";
} else if (apply_chi2_cuts_1e_cut) {
EventPrint_save_Directory = plots_path + "/" + "Event_Print_ALL_CUTS.txt";
}

EventPrint.open(EventPrint_save_Directory.c_str());

if (!apply_chi2_cuts_1e_cut) {
EventPrint << "//////////////////////////////////////////////////////////////////////\n";
EventPrint << "// Log of number of particles in event with all cuts except chi2    //\n";
EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
} else if (apply_chi2_cuts_1e_cut) {
EventPrint << "//////////////////////////////////////////////////////////////////////\n";
EventPrint << "// Log of number of particles in event with all cuts including chi2 //\n";
EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
}

EventPrint << "EVENT SELECTION:\n";
EventPrint << "#electrons in event:\t\t" << Ne_in_event << "\n";
EventPrint << "minimal #hadrons in event:\t" << Nf_in_event << "\n";
EventPrint << "#event upper bound:\t\t" << nEvents2print << "\n\n\n";
}
//</editor-fold>

//</editor-fold>

void SetSettings () {

//<editor-fold desc="Code settings">

// ======================================================================================================================================================================
// Event selection settings -------------------------------------------------------------------------------------------------------------------------------------------------
// ======================================================================================================================================================================

//<editor-fold desc="Event selection settings">
/* Settings to enable/disable specific FS plot calculations (Rec only): */

    bool calculate_truth_level = false;

    bool calculate_1p = false, calculate_1n = false;
    bool calculate_2p = true, calculate_pFDpCD = true, calculate_nFDpCD = false;

    bool Rec_wTL_ES = false; // Enforce TL event selection on Rec. plots
//
    bool Enable_FD_photons = false; // Enforce TL event selection on Rec. plots
//</editor-fold>

// ======================================================================================================================================================================
// Input processing
// ======================================================================================================================================================================

//<editor-fold desc="Input processing">
/* Initial input processing of loaded files (given by AnalyseFile) */

/* Determine file path and name */
    string LoadedInput = AnalyseFile; // AnalyseFile is taken from codeSetup.h
    string filePath = LoadedInput.substr(0, LoadedInput.find_last_of('/') + 1);
    string fileInput = LoadedInput.substr(LoadedInput.find_last_of('/') + 1);
    string plotsInput = fileInput.substr(0, fileInput.find_last_of(".hipo") - 4);

/* Configure and get run parameters */
    ExperimentParameters Experiment(AnalyseFilePath, AnalyseFileSample);
    string SampleName = Experiment.ConfigureSampleName(AnalyseFilePath, AnalyseFileSample); // Configure SampleName from input
    double beamE = Experiment.GetBeanEnergy(); // Configure beam energy from SampleName
    string Target = Experiment.GetTargetElement(); // Configure target (element) from SampleName
    int TargetPDG = Experiment.GetTargetElementPDG(); // Configure target PDG from SampleName
//</editor-fold>

// ======================================================================================================================================================================
// Cut settings
// ======================================================================================================================================================================

// Cut settings ---------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Cuts settings">
/* Settings that allow to disable/enable every cut individually */

// clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

//TODO: add beta = 1.2 cut for electrons
    bool apply_cuts = true; // master ON/OFF switch for applying cuts

/* HTCC cut */
    bool apply_Nphe_cut = true;

/* Chi2 cuts (= PID cuts) */
    bool apply_chi2_cuts_1e_cut = true;

/* Vertex cuts */
    bool apply_Vz_cuts = true, apply_dVz_cuts = true;

/* Sampling Fraction (SF) cut */
    bool apply_ECAL_SF_cuts = true;
    bool apply_ECAL_P_cuts = false;

/* ECAL fiducial (edge) cuts */
    bool apply_ECAL_fiducial_cuts = true;

/* DC fiducial (edge) cuts */
    bool apply_DC_fiducial_cut = true;

/* Momentum cuts */
    bool apply_momentum_cuts_1p = true, apply_momentum_cuts_1n = true;
    bool apply_momentum_cuts_2p = true, apply_momentum_cuts_pFDpCD = true, apply_momentum_cuts_nFDpCD = true;

    bool apply_nucleon_cuts = false;

//<editor-fold desc="Custom cuts naming & print out execution variables">

//<editor-fold desc="Custom cuts naming">
/* Save plots to custom-named folders, to allow multi-sample runs at once. */

    bool custom_cuts_naming = true;
    string Beta_Fit_Status, FD_photons_Status, Efficiency_Status;

    if (custom_cuts_naming) {
        if (apply_nucleon_cuts) {
            Beta_Fit_Status = "wBC_";
        } else {
            Beta_Fit_Status = "noBC_";
        }

        if (Enable_FD_photons) {
            FD_photons_Status = "wFDph_";
        } else {
            FD_photons_Status = "noFDph_";
        }

        if (Rec_wTL_ES) {
            Efficiency_Status = "Eff2";
//            Efficiency_Status = "Eff2_with_mom_th";
        } else {
            Efficiency_Status = "Eff1";
//            Efficiency_Status = "Eff1_wal1FDph";
        }

        if (!apply_cuts) {
            plots_path = WorkingDirectory + "plots_" + SampleName + "_-00_NO_CUTS";
            plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-00_NO_CUTS.txt";
        } else {
            if (!apply_chi2_cuts_1e_cut) {
                plots_path = WorkingDirectory + "plots_" + SampleName + "_-01_ALL_CUTS_woChi2";
                plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-01_ALL_CUTS_woChi2.txt";
            } else if (apply_chi2_cuts_1e_cut) {
                plots_path = WorkingDirectory + "plots_" + SampleName + "_-03_ALL_CUTS_" + Beta_Fit_Status + FD_photons_Status + Efficiency_Status;
                plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-03_ALL_CUTS_WithBetaCut_" + Beta_Fit_Status + FD_photons_Status
                                           + Efficiency_Status + ".txt";
            }
        }
    } else {
        Beta_Fit_Status = FD_photons_Status = Efficiency_Status = "";
    }
//</editor-fold>

//<editor-fold desc="Print out execution variables">
/* Print out execution variables (for self observation) */
    cout << "-- Execution variables ----------------------------------------------------\n";
    cout << "WorkingDirectory:\t" << WorkingDirectory << "\n";
    cout << "plots_path:\t\t" << plots_path << "\n\n";

    cout << "AnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n";
    cout << "AnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n";
    cout << "AnalyseFile:\t\t" << AnalyseFile << "\n";
    cout << "Settings mode:\t\t'" << file_name << "'\n\n";

    cout << "SampleName:\t\t" << SampleName << "\n";
    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
    cout << "Beam Energy:\t\t" << beamE << " [GeV]\n\n\n\n";
//</editor-fold>

//<editor-fold desc="Cuts output">
/* Print out the cuts within the run (for self-observation) */
    if (!apply_chi2_cuts_1e_cut) { apply_nucleon_cuts = false; }

    if (!apply_cuts) {
        cout << "Cuts are disabled:\n";

        apply_Nphe_cut = apply_chi2_cuts_1e_cut = apply_Vz_cuts = apply_dVz_cuts = false;
        apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_DC_fiducial_cut = false;
        apply_momentum_cuts_1p = apply_momentum_cuts_1n = apply_momentum_cuts_2p = apply_momentum_cuts_pFDpCD = apply_momentum_cuts_nFDpCD = false;
    } else {
        cout << "Cuts are enabled:\n";
    }

    cout << "apply_Nphe_cut:\t\t\t" << BoolToString(apply_Nphe_cut) << "\n";
    cout << "apply_chi2_cuts_1e_cut:\t\t" << BoolToString(apply_chi2_cuts_1e_cut) << "\n";
    cout << "apply_Vz_cuts:\t\t\t" << BoolToString(apply_Vz_cuts) << "\n";
    cout << "apply_dVz_cuts:\t\t\t" << BoolToString(apply_dVz_cuts) << "\n";
    cout << "apply_ECAL_SF_cuts:\t\t" << BoolToString(apply_ECAL_SF_cuts) << "\n";
    cout << "apply_ECAL_P_cuts:\t\t" << BoolToString(apply_ECAL_P_cuts) << "\n";
    cout << "apply_ECAL_fiducial_cuts:\t" << BoolToString(apply_ECAL_fiducial_cuts) << "\n";
    cout << "apply_DC_fiducial_cut:\t\t" << BoolToString(apply_DC_fiducial_cut) << "\n";
    cout << "apply_momentum_cuts_1p:\t\t" << BoolToString(apply_momentum_cuts_1p) << "\n";
    cout << "apply_momentum_cuts_1n:\t\t" << BoolToString(apply_momentum_cuts_1n) << "\n";
    cout << "apply_momentum_cuts_2p:\t\t" << BoolToString(apply_momentum_cuts_2p) << "\n";
    cout << "apply_momentum_cuts_pFDpCD:\t" << BoolToString(apply_momentum_cuts_pFDpCD) << "\n";
    cout << "apply_momentum_cuts_nFDpCD:\t" << BoolToString(apply_momentum_cuts_nFDpCD) << "\n";
    cout << "apply_nucleon_cuts:\t\t" << BoolToString(apply_nucleon_cuts) << "\n\n\n";
//</editor-fold>

//</editor-fold>

//</editor-fold>

// Cut declarations -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Cuts declarations">
/* Log cut values to be used later when applying them. */

// clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="clas12ana cuts">
/* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
    DSCuts Nphe_cuts_FD;

/* Chi2 cuts. NOTES:
 * Values for mean and sigma are filled from fit variables (overating these values later).
 * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */
    DSCuts Chi2_Electron_cuts_CD = DSCuts("Chi2", "CD", "Electron", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Electron_cuts_FD = DSCuts("Chi2", "FD", "Electron", "1e cut", 0, -9999, 9999);

    DSCuts Chi2_Proton_cuts_CD = DSCuts("Chi2", "CD", "Proton", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Proton_cuts_FD = DSCuts("Chi2", "FD", "Proton", "1e cut", 0, -9999, 9999);

    DSCuts Chi2_piplus_cuts_CD = DSCuts("Chi2", "CD", "piplus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_piplus_cuts_FD = DSCuts("Chi2", "FD", "piplus", "1e cut", 0, -9999, 9999);

    DSCuts Chi2_piminus_cuts_CD = DSCuts("Chi2", "CD", "piminus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_piminus_cuts_FD = DSCuts("Chi2", "FD", "piminus", "1e cut", 0, -9999, 9999);

//<editor-fold desc="Kaon PID cuts">
/* Kaon pid cuts. Not really applied in our analysis. */
    DSCuts Chi2_Kplus_cuts_CD = DSCuts("Chi2", "CD", "Kplus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Kplus_cuts_FD = DSCuts("Chi2", "FD", "Kplus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Kminus_cuts_CD = DSCuts("Chi2", "CD", "Kminus", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_Kminus_cuts_FD = DSCuts("Chi2", "FD", "Kminus", "1e cut", 0, -9999, 9999);
//</editor-fold>

/* Vertex cuts */
    DSCuts Vz_cut;
    DSCuts dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);

    if (SampleName == "LH2_data_6GeV_run_015032") {
        Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
    } else {
//TODO: these are good for Ca48 simulation and data. Check cuts for other targets!
        Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 5);
    }

/* Sampling Fraction (SF) cuts (electrons only, FD) */
    DSCuts SF_cuts;
//    DSCuts P_cuts;

/* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts PCAL_edge_cuts;

/* DC edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts DC_edge_cuts;

/* Momentum thresholds */
//    DSCuts e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
//    DSCuts p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
//    DSCuts pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, -9999, 9999);
//    DSCuts pim_mom_th = DSCuts("Momentum_th", "", "Piminus", "", 0, -9999, 9999);
//    DSCuts ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, -9999, 9999);
//    DSCuts n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, -9999, 9999);
    DSCuts e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
    DSCuts p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.3, 9999);
    DSCuts pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
    DSCuts n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.3, 9999);
//</editor-fold>

// Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Neutron momentum cuts (1n, FD only)">
/* All sectors */
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta.

    DSCuts n_momentum_cuts_ABF_FD_All_neutrals; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_All_neutrals_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_noPCAL; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_noPCAL_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_noPCAL_wECIN; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_noPCAL_wECIN_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT_noPDG0; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.

/* Specific sectors */
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S1; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 1).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S1_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 1).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S2; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 2).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S2_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 2).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S3; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 3).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S3_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 3).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S4; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 4).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S4_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 4).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S5; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 5).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S5_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 5).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S6; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file (sector 6).
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_S6_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta (sector 6).
//</editor-fold>

//<editor-fold desc="Truth-level momentum cuts">
/* Momentum cuts */
    DSCuts TL_e_mom_cuts = DSCuts("Momentum", "", "Electron", "", 0, e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut());

    DSCuts TL_n_mom_cuts = DSCuts("Momentum", "", "Neutrons", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());;
    DSCuts TL_p_mom_cuts = DSCuts("Momentum", "", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    DSCuts TL_pip_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pim_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts TL_pi0_mom_cuts = DSCuts("Momentum", "", "Pizero", "", 0, GetPi0MomTh(ph_mom_th), 9999);
    DSCuts TL_ph_mom_cuts = DSCuts("Momentum", "", "Photons", "", 0, ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut());
//</editor-fold>

/* FD theta range (1n & 1p) */
    DSCuts ThetaFD = DSCuts("Theta FD", "FD", "", "1n & 1p", 1, 5., 40.);

/* Beta cut (1n, FD) */
    DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, 0, 9999);

    DSCuts Beta_cut_ABF_FD_n_from_ph, Beta_cut_ABF_FD_n_from_ph_apprax; // All sectors
    DSCuts Beta_cut_ABF_FD_n_from_ph_S1, Beta_cut_ABF_FD_n_from_ph_S1_apprax; // Sector 1
    DSCuts Beta_cut_ABF_FD_n_from_ph_S2, Beta_cut_ABF_FD_n_from_ph_S2_apprax; // Sector 2
    DSCuts Beta_cut_ABF_FD_n_from_ph_S3, Beta_cut_ABF_FD_n_from_ph_S3_apprax; // Sector 3
    DSCuts Beta_cut_ABF_FD_n_from_ph_S4, Beta_cut_ABF_FD_n_from_ph_S4_apprax; // Sector 4
    DSCuts Beta_cut_ABF_FD_n_from_ph_S5, Beta_cut_ABF_FD_n_from_ph_S5_apprax; // Sector 5
    DSCuts Beta_cut_ABF_FD_n_from_ph_S6, Beta_cut_ABF_FD_n_from_ph_S6_apprax; // Sector 6

    DSCuts Beta_cut_ABF_All_FD_neutrals, Beta_cut_ABF_FD_neutrals_noPDG0;
    DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL;
    DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL_wECIN, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_wECIN;
    DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL_noECIN_wECOUT, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT;

/* Nucleon theta cut (1p & 1n, FD) */
    DSCuts Theta_nuc_cut = DSCuts("Theta_nuc", "FD", "", "1p & 1n", 0, -9999, 32.);

/* Neutron veto cut (1n & 1n1p, FD) */
    DSCuts Neutron_veto_cut = DSCuts("Neutron veto", "FD", "", "1n", 0, 100, 9999);

/* Ghost tracks handling (2p & pFDpCD, CD & FD) */
    DSCuts Theta_p1_cuts_2p = DSCuts("Theta_p1", "", "Proton", "2p", 40., -9999, 7.5);
    DSCuts Theta_p2_cuts_2p = DSCuts("Theta_p2", "", "Proton", "2p", 40., -9999, 7.5);
    DSCuts dphi_p1_p2_2p = DSCuts("dPhi_p1_p2", "", "Proton", "2p", 0, -9999, 15.);
    DSCuts Theta_p1_cuts_pFDpCD = DSCuts("Theta_p1", "", "Proton", "pFDpCD", 40., -9999, 7.5);
    DSCuts Theta_p2_cuts_pFDpCD = DSCuts("Theta_p2", "", "Proton", "pFDpCD", 40., -9999, 7.5);
    DSCuts dphi_pFD_pCD_pFDpCD = DSCuts("dPhi_p1_p2", "", "Proton", "pFDpCD", 0, -9999, 15.);
//</editor-fold>

// ======================================================================================================================================================================
// Other settings
// ======================================================================================================================================================================

// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="TList definition">
/* Definition of plots TList used to save all plots to .root file. */

    TList *plots = new TList();
    string listName = plots_path + "/" + AnalyseFileSample + plots_file_type;
    const char *TListName = listName.c_str();
//</editor-fold>

//  Checking directories ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Creating directories">
/* Code for creating directories.
 * Directory creation is done in the Directories class.
 * Added for the case that plots out folder does not exist and for organization.
 * All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */

    cout << "Creating plot directories...\n\n";

    string Plots_Folder = plots_path;
    Directories directories = Directories(Plots_Folder);
//</editor-fold>

// Plot settings --------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Plot selector">
/* Here are boolean variables used to turn ON/OFF the different plots of the code.
   Plot_selector_master must remain true, set it OFF only for debugging. */

/* Master plots variable */
    bool Plot_selector_master = true; // Master plot selector for analysis

/* Cut variable plots */
    bool Cut_plots_master = true; // Master cut plots selector
    bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true, Momentum_plots = true;

/* Beta plots */
//    bool Beta_plots = true;
    bool Beta_plots = false;
    cout << "\n\n\n\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;";
    cout << "\nbool Beta_plots = false;\n\n\n\n";
//    bool Beta_vs_P_plots = true;
    bool Beta_vs_P_plots = false;
    cout << "\n\n\n\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;";
    cout << "\nbool Beta_vs_P_plots = false;\n\n\n\n";

/* Angle plots */
    bool Angle_plots_master = true; // Master angle plots selector
    bool Theta_e_plots = true, Phi_e_plots = true;
//    bool Angle_plots_master = false; // Master angle plots selector
//    bool Theta_e_plots = false, Phi_e_plots = false;
//    cout << "\n\n\n\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;";
//    cout << "\nbool Angle_plots_master = false;\n\n\n\n";

/* Q2 plots */
//    bool Q2_plots = true;
    bool Q2_plots = false;
    cout << "\n\n\n\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;";
    cout << "\nbool Q2_plots = false;\n\n\n\n";

/* E_e plots */
//    bool E_e_plots = true;
    bool E_e_plots = false;
    cout << "\n\n\n\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;";
    cout << "\nbool E_e_plots = false;\n\n\n\n";

/* ET plots */
//    bool ETrans_plots_master = true; // Master ET plots selector
    bool ETrans_plots_master = false; // Master ET plots selector
    cout << "\n\n\n\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;";
    cout << "\nbool ETrans_plots_master = false;\n\n\n\n";
    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

/* Ecal plots */
//    bool Ecal_plots = true;
    bool Ecal_plots = false;
    cout << "\n\n\n\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;";
    cout << "\nbool Ecal_plots = false;\n\n\n\n";

/* Transverse variables plots */
//    bool TVariables_plots = true;
    bool TVariables_plots = false;
    cout << "\n\n\n\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;";
    cout << "\nbool TVariables_plots = false;\n\n\n\n";

/* ToF plots */
    bool ToF_plots = false;

/* Efficiency plots */
//    bool Efficiency_plots = true;
    bool Efficiency_plots = false;
    cout << "\n\n\n\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;";
    cout << "\nbool Efficiency_plots = false;\n\n\n\n";

//<editor-fold desc="Turn off plots by master selectors">
    if (!Plot_selector_master) {
        Cut_plots_master = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Q2_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
        TVariables_plots = ToF_plots = Efficiency_plots = false;
    }

    if (!Cut_plots_master) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }
    if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }
    if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }
//</editor-fold>

/* Other settings variables */
    bool wider_margin = true;
    bool debug_plots = true; // Print out clas12ana debugging plots
//</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Normalization settings">
/* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
 * Enable of presentations only, since event count is important otherwise. */

    bool normalize_master = false;

    bool norm_Nphe_plots = false, norm_Chi2_plots = false, norm_Vertex_plots = false, norm_SF_plots = false, norm_Fiducial_plots = false, norm_Momentum_plots = false;

    bool norm_Beta_plots = false, norm_Angle_plots_master = false, norm_Q2_plots = false, norm_E_e_plots = false, norm_ET_plots = false, norm_Ecal_plots = false;
    bool norm_TVariables_plots = false;

    if (!normalize_master) { // Disable all normalizations if normalize_master == false
        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = norm_Momentum_plots = false;
        norm_Angle_plots_master = norm_Q2_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TVariables_plots = false;

        cout << "\nAll normalizations are disabled.\n\n";  // and no change to custom_FSI_status
    }
//</editor-fold>

// Delete settings ------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Delete settings">
/* Clear files from previous runs (to prevent mix fo plots from different codes). */

    bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

/* Delete existing .txt files */
    if (delete_txt_files) { system(("find " + plots_path + " -type f -iname '*.txt' -delete").c_str()); }

//<editor-fold desc="Deleting files by cases">
    if (delete_png_files && !delete_root_files) {
        cout << "\nClearing old plots...";
        system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
        cout << " done.\n\n";
    } else if (!delete_png_files && delete_root_files) {
        cout << "\nClearing old root files...";
        system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
        cout << " done.\n\n";
    } else if (delete_png_files && delete_root_files) {
        cout << "\nClearing old plots & root files...";
        system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
        system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
        cout << " done.\n\n";
    } else {
        cout << "\nNo files were cleared.\n\n";
    }
//</editor-fold>

//</editor-fold>

// Histogram limits -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Histogram boundaries">
/* Histogram boundary variables. Used to unify histograms to the same boundaries. */

/* Nphe plots */
    double Nphe_boundary = 40;

/* Chi2 plots */
    double Chi2_boundary = 30;
    if (apply_cuts) { Chi2_boundary = 9; }

/* Vertex plots */
    double Vertex_boundary = 50, Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;

    if (apply_cuts) {
        double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
        Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
        Vertex_boundary = Vertex_boundary / 5;
    }

    double dV_boundary = 50;
    if (apply_cuts) { /* dV_boundary = 7.5; */ dV_boundary = dVz_cuts.GetUpperCut() * 1.4; }

/* SF */
    double SF_uboundary = 0.31, SF_lboundary = 0.16;

/* Momentum boundries */
    double Momentum_lboundary = 0., Momentum_uboundary = beamE * 1.1;
//    double Momentum_uboundary = 20., Momentum_lboundary = 0.;

/* Beta plots */
    double dBeta_sigma_boundary = 0.1;
    double Beta_dist_uboundary = 1 + dBeta_sigma_boundary, Beta_dist_lboundary = 1 - dBeta_sigma_boundary;

    double dBeta_sigma_ZOOMOUT_boundary = 0.1;
    double Beta_dist_ZOOMOUT_uboundary = 1 + dBeta_sigma_ZOOMOUT_boundary;
    double Beta_dist_ZOOMOUT_lboundary = 0.9;

    double Beta_boundary = 3., P_boundary = beamE * 1.425;
    if (apply_cuts) { Beta_boundary = 1.25, P_boundary = beamE * 1.1; }

/* Theta plots */
    double Theta_lboundary_FD = 0., Theta_uboundary_FD = 50.;

/* Phi plots */
    double Phi_lboundary_FD = -180., Phi_uboundary_FD = 180.;

/* Transverse variables */
    double dP_T_boundary = 3.;
//</editor-fold>

// Debugging settings ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Debugging settings">
/* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */

    bool PrintEvents = false;
    int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;

    ofstream EventPrint;
    string EventPrint_save_Directory;

    if (PrintEvents) {
        if (!apply_chi2_cuts_1e_cut) {
            EventPrint_save_Directory = plots_path + "/" + "Event_Print_without_chi2.txt";
        } else if (apply_chi2_cuts_1e_cut) {
            EventPrint_save_Directory = plots_path + "/" + "Event_Print_ALL_CUTS.txt";
        }

        EventPrint.open(EventPrint_save_Directory.c_str());

        if (!apply_chi2_cuts_1e_cut) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n";
            EventPrint << "// Log of number of particles in event with all cuts except chi2    //\n";
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
        } else if (apply_chi2_cuts_1e_cut) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n";
            EventPrint << "// Log of number of particles in event with all cuts including chi2 //\n";
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
        }

        EventPrint << "EVENT SELECTION:\n";
        EventPrint << "#electrons in event:\t\t" << Ne_in_event << "\n";
        EventPrint << "minimal #hadrons in event:\t" << Nf_in_event << "\n";
        EventPrint << "#event upper bound:\t\t" << nEvents2print << "\n\n\n";
    }
//</editor-fold>

//</editor-fold>

}

#endif //DSASETTINGS_H
