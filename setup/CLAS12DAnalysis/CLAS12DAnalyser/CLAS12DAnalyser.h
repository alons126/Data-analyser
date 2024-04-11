//
// Created by alons on 11/08/2023.
//

#ifndef CLAS12DANALYSER_H
#define CLAS12DANALYSER_H

#include "../../codeSetup.h"
#include "../../../source/classes/AMaps/AMaps.cpp"
#include "../../../source/classes/clas12ana/clas12ana.h"
#include "../../../source/classes/DEfficiency/DEfficiency.cpp"
#include "../../../source/classes/DSCuts/DSCuts.h"
#include "../../../source/classes/hPlots/hPlot1D.cpp"
#include "../../../source/classes/hPlots/hPlot2D.cpp"
#include "../../../source/classes/MomentumResolution/MomentumResolution.cpp"
#include "../../../source/classes/TLCuts/TLCuts.cpp"
#include "../../../source/functions/AngleCalc/GetBinFromAng.h"
#include "../../../source/functions/FitFunctions/BetaFit.h"
#include "../../../source/functions/FitFunctions/BetaFitApprax.h"
#include "../../../source/functions/DrawAndSaveFunctions/DrawAndSaveACorrPlots.h"
#include "../../../source/functions/DrawAndSaveFunctions/DrawAndSaveEfficiencyPlots.h"
#include "../../../source/functions/DrawAndSaveFunctions/DrawAndSaveFSRatio.h"
#include "../../../source/functions/FillByInt.h"
#include "../../../source/functions/GetPi0MomTh.h"
#include "../../../source/functions/PID_functions/ChargedParticleID.h"
#include "../../../source/functions/PID_functions/FDNeutralParticle.h"
#include "../../../source/functions/PID_functions/FDNeutralParticleID.h"
#include "../../../source/functions/PID_functions/GetFDNeutronP.h"
#include "../../../source/functions/AngleCalc/CalcdPhi.h"
#include "../../../source/functions/PID_functions/GetGoodParticles.h"
#include "../../../source/functions/PID_functions/GetGoodProtons.h"
#include "../../../source/functions/NeutronECAL_Cut_Veto.h"
#include "../../../source/functions/StackPlot3.h"
#include "../../../source/functions/StackPlot4.h"
#include "../../../source/functions/TLKinCutsCheck.h"

#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;
using namespace clas12;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code setup                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//<editor-fold desc="Code setup">

// ======================================================================================================================================================================
// Input processing
// ======================================================================================================================================================================

//<editor-fold desc="Input processing">
/* Initial input processing of loaded files (given by AnalyseFile) */

/* Determine file path and name */
const string LoadedInput = AnalyseFile; // AnalyseFile is taken from codeSetup.h
const string filePath = LoadedInput.substr(0, LoadedInput.find_last_of('/') + 1);
const string fileInput = LoadedInput.substr(LoadedInput.find_last_of('/') + 1);
const string plotsInput = fileInput.substr(0, fileInput.find_last_of(".hipo") - 4);

/* Configure and get run parameters */
ExperimentParameters Experiment(AnalyseFilePath, AnalyseFileSample);
const string SampleName = Experiment.ConfigureSampleName(AnalyseFilePath, AnalyseFileSample); // Configure SampleName from input
const double beamE = Experiment.GetBeanEnergy(); // Configure beam energy from SampleName
const string Target = Experiment.GetTargetElement(); // Configure target (element) from SampleName
const int TargetPDG = Experiment.GetTargetElementPDG(); // Configure target PDG from SampleName
const bool isLocal = Experiment.SLocal();
const bool isSimulation = Experiment.SSample();
const bool isData = Experiment.DSample();
const bool is2GeVSample = Experiment.IsBeamAt2GeV();
const bool is4GeVSample = Experiment.IsBeamAt4GeV();
const bool is6GeVSample = Experiment.IsBeamAt6GeV();
//</editor-fold>

// ======================================================================================================================================================================
// Event selection setup
// ======================================================================================================================================================================

//<editor-fold desc="Event selection setup">
/* Settings to enable/disable specific FS plot calculations (Rec only): */

/* Final states to analyse */
bool calculate_1p = true, calculate_1n = true, calculate_2p = true;
bool calculate_pFDpCD = true, calculate_nFDpCD = true;

/* Truth level calculation setup */
bool calculate_truth_level = true; // TL master ON/OFF switch
bool fill_TL_plots = true;
bool Rec_wTL_ES = false; // Enforce TL event selection on reco. plots

bool limless_mom_eff_plots = false;
bool Enable_FD_photons = false; // keep as false to decrease RES and DIS
bool Enable_FD_neutrons = true; // keep as false to increse eff. plots

//TODO: add this switch to event selection variables!
bool ES_by_leading_FDneutron = true;

/* Acceptance maps setup */
bool Generate_AMaps = false; // Generate acceptance maps
bool TL_with_one_reco_electron = true;
bool reformat_e_bins = false;
bool varying_P_e_bins = true;

/* Neutron resolution setup */
bool plot_and_fit_MomRes = false;
bool VaryingDelta = true;
double DeltaSlices = 0.05;
bool nRes_SaS_test = false; // to test neutron shift AFTER calculating neutron resolution
//</editor-fold>

// ======================================================================================================================================================================
// Cut setup
// ======================================================================================================================================================================

// Cut setup ---------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Cuts setup">
/* Settings that allow to disable/enable every cut individually */

// clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
bool apply_cuts = true; // master ON/OFF switch for applying cuts

/* HTCC cut */
bool apply_Nphe_cut = true;

/* Chi2 cuts (= PID cuts) */
bool apply_chi2_cuts_1e_cut = true;

/* Vertex cuts */
bool apply_Vz_cuts = true, apply_dVz_cuts = true;

/* Sampling Fraction (SF) cut */
bool apply_ECAL_SF_cuts = true;
bool apply_ECAL_P_cuts = true;

/* ECAL fiducial (edge) cuts */
bool apply_ECAL_fiducial_cuts = true;

/* DC fiducial (edge) cuts */
bool apply_DC_fiducial_cuts = true;

/* Nucleon cuts */
bool apply_nucleon_cuts = true; // set as true to get good protons and chaculate neutron momentum

/* Physical cuts */
bool apply_nucleon_physical_cuts = true; // nucleon physical cuts master
bool apply_nBeta_fit_cuts = true;
bool apply_fiducial_cuts = false;
bool apply_kinematical_cuts = true;
bool apply_kinematical_weights = false;
bool apply_nucleon_SmearAndCorr = true;

//<editor-fold desc="Custom cuts naming & print out execution variables">

//<editor-fold desc="New samples setup">
//    if (!apply_chi2_cuts_1e_cut) { // for first run on new samples
//        Generate_AMaps = plot_and_fit_MomRes = true;
//        VaryingDelta = false;
//    }
//
//    if (Generate_AMaps) { Rec_wTL_ES = false; }
//</editor-fold>

//<editor-fold desc="Custom cuts naming">
/* Save plots to custom-named folders, to allow multi-sample runs at once. */
bool custom_cuts_naming = true;
string Nucleon_Cuts_Status, FD_photons_Status, PSmearing_Status, FiducialCuts_Status, KinCuts_Status, KinWei_Status, Additional_Status, Efficiency_Status;
//</editor-fold>

//<editor-fold desc="Print out execution variables">
//</editor-fold>

//<editor-fold desc="Cuts output">
/* Print out the cuts within the run (for self-observation) */
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

/* Sampling Fraction (SF) cuts (electrons only, FD) */
DSCuts SF_cuts;
//    DSCuts P_cuts;

/* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
DSCuts PCAL_edge_cuts;

/* DC edge cuts (fiducial cuts ,electrons only, FD) */
DSCuts DC_edge_cuts;

/* Momentum thresholds */
DSCuts e_mom_th, p_mom_th, n_mom_th, pip_mom_th, pim_mom_th, ph_mom_th;

//<editor-fold desc="Momentum thresholds definition">
/* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
//</editor-fold>

//</editor-fold>

// Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Other cuts">
/* Neutron beta cuts (1n & nFDpCD, FD only) */
DSCuts Beta_max_cut_ABF_FD_n_from_ph, Beta_max_cut_ABF_FD_n_from_ph_apprax;

/* Neutron momentum cuts (1n & nFDpCD, FD only) */
DSCuts n_momentum_cuts_ABF_FD_n_from_ph; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
DSCuts n_momentum_cuts_ABF_FD_n_from_ph_apprax; // Appraximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta.

/* Truth-level momentum cuts */
DSCuts TL_e_mom_cuts = DSCuts("Momentum", "", "Electron", "", 0, e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut());
DSCuts TL_n_mom_cuts = DSCuts("Momentum", "", "Neutrons", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());;
DSCuts TL_p_mom_cuts = DSCuts("Momentum", "", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
DSCuts TL_pip_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
DSCuts TL_pim_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
DSCuts TL_pi0_mom_cuts = DSCuts("Momentum", "", "Pizero", "", 0, GetPi0MomTh(ph_mom_th), 9999);
DSCuts TL_ph_mom_cuts = DSCuts("Momentum", "", "Photons", "", 0, ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut());

/* FD & CD theta range */
DSCuts ThetaFD = DSCuts("Theta FD", "FD", "", "", 1, 5., 40.);
DSCuts ThetaCD = DSCuts("Theta CD", "CD", "", "", 1, 40., 135.);

/* Beta cut (1n, FD) */
DSCuts Beta_cut = DSCuts("Beta_nuc", "FD", "", "1n", 1, 0, 9999);

/* Neutron veto cut (1n & nFDpCD, FD) */
DSCuts Neutron_veto_cut = DSCuts("Neutron veto", "FD", "", "1n", 0, 100, 9999);

/* Ghost tracks handling (2p & pFDpCD, CD & FD) */
DSCuts Theta_p1_cuts_2p = DSCuts("Theta_p1", "", "Proton", "2p", 40., -9999, 5.);
DSCuts Theta_p2_cuts_2p = DSCuts("Theta_p2", "", "Proton", "2p", 40., -9999, 5.);
DSCuts dphi_p1_p2_2p = DSCuts("dPhi_p1_p2", "", "Proton", "2p", 0, -9999, 15.);

DSCuts Theta_pFD_cuts_2p = DSCuts("Theta_p1 leading", "", "Proton", "2p", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
DSCuts Theta_pCD_cuts_2p = DSCuts("Theta_p2 recoil", "", "Proton", "2p", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
DSCuts dphi_pFD_pCD_2p = DSCuts("dPhi_pFD_pCD", "", "Proton", "2p", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

DSCuts Theta_L_cuts_pFDpCD = DSCuts("Theta_L leading", "", "Proton", "pFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
DSCuts Theta_R_cuts_pFDpCD = DSCuts("Theta_R recoil", "", "Proton", "pFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
DSCuts dphi_pFD_pCD_pFDpCD = DSCuts("dPhi_pFD_pCD", "", "Proton", "pFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

DSCuts Theta_L_cuts_nFDpCD = DSCuts("Theta_L", "", "", "nFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
DSCuts Theta_R_cuts_nFDpCD = DSCuts("Theta_R", "", "", "nFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
DSCuts dphi_L_R_nFDpCD = DSCuts("dphi_L_R", "", "", "nFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

/* reco. kinematic cuts (based on nucleons' efficiency) */
DSCuts FD_nucleon_theta_cut = DSCuts("FD Nucleon theta cut", "FD", "", "", 0, 0, 32.);
DSCuts Nucleon_momentum_cut = DSCuts("Nucleon momentum cut", "FD", "", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
//    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1., 3.); // new upper cut, following Larry meeting (10/08/23)
DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1., n_mom_th.GetUpperCut());
//</editor-fold>

//</editor-fold>

// ======================================================================================================================================================================
// Other setup
// ======================================================================================================================================================================

// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="TList definition">
/* Definition of plots TLists used to save all plots to .root file. */

/* General plots TList */
TList *plots = new TList();
string listName = plots_path + "/" + AnalyseFileSample + plots_file_type;
const char *TListName = listName.c_str();
//</editor-fold>

//  Creating directories ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Creating directories">
/* Code for creating directories.
* Directory creation is done in the Directories class.
* Added for the case that plots out folder does not exist and for organization.
* All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */

string Plots_Folder = plots_path;
Directories directories;

//</editor-fold>

// Plot setup --------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Plot selector">
/* Here are boolean variables used to turn ON/OFF the different plots of the code.
   Plot_selector_master must remain true, set it OFF only for debugging. */

//<editor-fold desc="Plot selector - plot all">
/* Master plots variable */
bool Plot_selector_master = true; // Master plot selector for analysis

/* Cut variable plots */
bool Cut_plots_master = true; // Master cut plots selector
bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true, Momentum_plots = true;

/* Beta plots */
bool W_plots = true;

/* Beta plots */
bool Beta_plots = true;
bool Beta_vs_P_plots = true;

/* Angle plots */
bool Angle_plots_master = true; // Master angle plots selector
bool Theta_e_plots = true, Phi_e_plots = true;

/* Momentum transfer plots */
bool Momentum_transfer_plots = true;

/* E_e plots */
bool E_e_plots = true;

/* ET plots */
bool ETrans_plots_master = true; // Master ET plots selector
bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

/* Ecal plots */
bool Ecal_plots = true;

/* Transverse variables plots */
bool TKI_plots = true;

/* ToF plots */
bool ToF_plots = false;

/* Efficiency plots */
bool Efficiency_plots = true;
bool TL_after_Acceptance_Maps_plots = true;

/* Resolution plots */
bool Hit_maps_plots = true;

/* Resolution plots */
bool Resolution_plots = true;

/* Final state ratio plots */
bool FSR_plots = true;
//</editor-fold>

//    //<editor-fold desc="Plot selector - selected plots">
//    /* Master plots variable */
//    bool Plot_selector_master = true; // Master plot selector for analysis
//
//    /* Cut variable plots */
//    bool Cut_plots_master = true; // Master cut plots selector
////    bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true;
//    bool Nphe_plots = false, Chi2_plots = false, Vertex_plots = false, SF_plots = false, fiducial_plots = false;
////
////    bool Momentum_plots = false;
//    bool Momentum_plots = true;
////
//
//    /* Beta plots */
////    bool W_plots = true;
//    bool W_plots = false;
//
//    /* Beta plots */
////    bool Beta_plots = true;
//    bool Beta_plots = false;
////    bool Beta_vs_P_plots = true;
//    bool Beta_vs_P_plots = false;
//
//    /* Angle plots */
//    bool Angle_plots_master = true; // Master angle plots selector
//    bool Theta_e_plots = true, Phi_e_plots = true;
////    bool Angle_plots_master = false; // Master angle plots selector
////    bool Theta_e_plots = false, Phi_e_plots = false;
//
//    /* Momentum transfer plots */
////    bool Momentum_transfer_plots = true;
//    bool Momentum_transfer_plots = false;
//
//    /* E_e plots */
////    bool E_e_plots = true;
//    bool E_e_plots = false;
//
//    /* ET plots */
////    bool ETrans_plots_master = true; // Master ET plots selector
//    bool ETrans_plots_master = false; // Master ET plots selector
//    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;
//
//    /* Ecal plots */
////    bool Ecal_plots = true;
//    bool Ecal_plots = false;
//
//    /* Transverse variables plots */
////    bool TKI_plots = true;
//    bool TKI_plots = false;
//
//    /* ToF plots */
////    bool ToF_plots = true;
//    bool ToF_plots = false;
//
//    /* Efficiency plots */
////    bool Efficiency_plots = true;
//    bool Efficiency_plots = false;
////    bool TL_after_Acceptance_Maps_plots = true;
//    bool TL_after_Acceptance_Maps_plots = false;
//
//    /* Resolution plots */
////    bool Hit_maps_plots = true;
//    bool Hit_maps_plots = false;
//
//    /* Resolution plots */
////    bool Resolution_plots = true;
//    bool Resolution_plots = false;
//
//    /* Final state ratio plots */
////    bool FSR_plots = true;
//    bool FSR_plots = false;
//    //</editor-fold>/

//<editor-fold desc="Turn off plots by master selectors">
//</editor-fold>

/* Other setup variables */
bool wider_margin = true;
bool debug_plots = true; // Print out clas12ana debugging plots
//</editor-fold>

// Normalization setup -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Normalization setup">
/* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
 * Enable of presentations only, since event count is important otherwise. */

bool normalize_master = false;

bool norm_Nphe_plots = false, norm_Chi2_plots = false, norm_Vertex_plots = false, norm_SF_plots = false, norm_Fiducial_plots = false, norm_Momentum_plots = false;

bool norm_W_plots = false, norm_Beta_plots = false, norm_Angle_plots_master = false, norm_Momentum_transfer_plots = false, norm_E_e_plots = false, norm_ET_plots = false;
bool norm_Ecal_plots = false, norm_TKI_plots = false;

//</editor-fold>

// Delete setup ------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Delete setup">
/* Clear files from previous runs (to prevent mix fo plots from different codes). */

bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

/* Delete existing .txt files */
//</editor-fold>

//</editor-fold>

// Histogram setup ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Histogram setup">
/* Histogram boundary variables. Used to unify histograms to the same boundaries. */

//<editor-fold desc="Number of histogram bins">
/* Default */
int numTH1Dbins = 50;
int numTH2Dbins = 65;

/* Beta plots */
int numTH1Dbins_Beta_Plots = 65;
int numTH2Dbins_Beta_Plots = 65;

/* W plots */
int numTH1Dbins_W_Plots = 50;
int numTH2Dbins_W_Plots = 65;

/* Angle plots */
int numTH1Dbins_Ang_Plots = 50;
int numTH2Dbins_Ang_Plots = 65;
int numTH2Dbins_Electron_Ang_Plots = 100;
int numTH2Dbins_Nucleon_Ang_Plots = 75;

/* Momentum transfer plots */
int numTH1Dbins_Momentum_transfer_plots = 50;

/* E_e plots */
int numTH1Dbins_E_e_Plots = 50;
int numTH2Dbins_E_e_Plots = 65;

/* ET plots */
int numTH1Dbins_ET_Plots = 50;

/* E_cal plots */
int numTH1Dbins_E_cal_Plots = 50;
int numTH2Dbins_E_cal_Plots = 65;

/* TKI plots */
int numTH1Dbins_TKI_dP_T_Plots = 50;
int numTH1Dbins_TKI_dAlpha_T_Plots = 30;
int numTH2Dbins_TKI_Plots = 65;

/* Efficiency % acceptance correction plots */
int numTH1Dbins_Mom_eff_Plots = numTH1Dbins;
int numTH1Dbins_Ang_eff_Plots = numTH1Dbins_Ang_Plots;
int numTH2Dbins_Ang_eff_Plots = numTH2Dbins_Ang_Plots;
int numTH2Dbins_Electron_Ang_eff_Plots = numTH2Dbins_Electron_Ang_Plots;
int numTH2Dbins_Nucleon_Ang_eff_Plots = numTH2Dbins_Nucleon_Ang_Plots;

/* Acceptance maps plots */
int numTH1Dbins_AMaps_Plots = numTH1Dbins_Ang_Plots;
int numTH2Dbins_AMaps_Plots = numTH2Dbins_Ang_Plots;
int numTH2Dbins_Electron_AMaps_Plots = numTH2Dbins_Electron_Ang_Plots;
int numTH2Dbins_Nucleon_AMaps_Plots = numTH2Dbins_Nucleon_Ang_Plots;

/* Neutron resolution plots */
int numTH1Dbins_nRes_Plots = numTH1Dbins_Ang_Plots;
int numTH2Dbins_nRes_Plots = numTH2Dbins_Nucleon_Ang_Plots;
//</editor-fold>

//<editor-fold desc="Histogram limits">
/* Nphe boundries */
double Nphe_boundary = 40;

/* Chi2 boundries */
double Chi2_boundary = 30;

/* Vertex boundries */
double Vertex_boundary = 50, Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;

double dV_boundary = 50;

/* SF boundries */
double SF_uboundary = 0.31, SF_lboundary = 0.16;

/* Momentum boundries */
double Momentum_lboundary = 0., Momentum_uboundary = beamE * 1.1; // Default
double CDMomentum_lboundary = 0., CDMomentum_uboundary = beamE / 2; // CD nucleons (pFDpCD & nFDpCD)

/* W boundries */
double W_lboundary = 0.5, W_uboundary = (beamE * 1.1) / 2; // Default

/* Beta boundries */
double dBeta_sigma_boundary = 0.1;
double Beta_dist_uboundary = 1 + dBeta_sigma_boundary, Beta_dist_lboundary = 1 - dBeta_sigma_boundary;

double dBeta_sigma_ZOOMOUT_boundary = 0.1;
double Beta_dist_ZOOMOUT_uboundary = 1 + dBeta_sigma_ZOOMOUT_boundary;
double Beta_dist_ZOOMOUT_lboundary = 0.9;

double Beta_boundary = 3., P_boundary = beamE * 1.425;

/* Angle boundries */
double Theta_lboundary_FD = 0., Theta_uboundary_FD = 50.;
double Theta_lboundary_CD = 30., Theta_uboundary_CD = 155.;
double Opening_Ang_wide_lboundary = 0, Opening_Ang_wide_uboundary = 180.;
double Opening_Ang_narrow_lboundary = 0, Opening_Ang_narrow_uboundary = 100.;
double Phi_lboundary = -180., Phi_uboundary = 180.;

/* TKI boundries */
double dP_T_boundary = 3.;
//</editor-fold>

//</editor-fold>

// Acceptance maps generation -------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Acceptance maps generation">
/* Acceptance maps are handled completely by the AMaps class */

/* Set Bins by case */
int HistElectronSliceNumOfXBins = numTH2Dbins_Electron_Ang_Plots, HistNucSliceNumOfXBins = numTH2Dbins_Nucleon_Ang_Plots;
int NumberNucOfMomSlices = 4;

AMaps aMaps, wMaps;
//</editor-fold>

// Neutron resolution & proton smearing ---------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Neutron resolution">
/* Neutron resolution fits is handled completely by the MomentumResolution class */

//<editor-fold desc="Neutron resolution class declaration & definition">
MomentumResolution nRes, pRes;

//</editor-fold>

//</editor-fold>

// Acceptance correction data -------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Acceptance correction">
/* Neutron resolution fits is handled completely by the MomentumResolution class */

bool save_ACorr_data = true;


DEfficiency eff;


TList *ACorr_data = new TList();
string ACorr_data_Dir = ACorrDirectory + SampleName;
string ACorr_data_listName = ACorr_data_Dir + "/" + "ACorr_data_-_" + SampleName + ".root";
const char *ACorr_DataName = ACorr_data_listName.c_str();
//</editor-fold>

// Debugging setup ---------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="Debugging setup">
/* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */

bool GoodProtonsMonitorPlots = true;

bool PrintEvents = false;
int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;

ofstream EventPrint;
string EventPrint_save_Directory;
//</editor-fold>

//</editor-fold>

void InitSettings() {

    //<editor-fold desc="Code setup">

// ======================================================================================================================================================================
// Input processing
// ======================================================================================================================================================================

    //<editor-fold desc="Input processing">
    //</editor-fold>

// ======================================================================================================================================================================
// Event selection setup
// ======================================================================================================================================================================

    //<editor-fold desc="Event selection setup">
//    if (!calculate_2p) { calculate_pFDpCD = false; }
    if (isData) { calculate_truth_level = false; }
    if (!calculate_truth_level) { TL_with_one_reco_electron = fill_TL_plots = Rec_wTL_ES = false; }

//    if (!apply_chi2_cuts_1e_cut) { // for first run on new samples
//        Generate_AMaps = plot_and_fit_MomRes = true;
//        VaryingDelta = false;
//    }

//    if (Generate_AMaps) { Rec_wTL_ES = false; }
    //</editor-fold>

// ======================================================================================================================================================================
// Cut setup
// ======================================================================================================================================================================

// Cut setup ---------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Cuts setup">
    /* Settings that allow to disable/enable every cut individually */

    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Custom cuts naming & print out execution variables">

    //<editor-fold desc="New samples setup">
//    if (!apply_chi2_cuts_1e_cut) { // for first run on new samples
//        Generate_AMaps = plot_and_fit_MomRes = true;
//        VaryingDelta = false;
//    }
//
//    if (Generate_AMaps) { Rec_wTL_ES = false; }
    //</editor-fold>

    //<editor-fold desc="Custom cuts naming">
    /* Save plots to custom-named folders, to allow multi-sample runs at once. */

    if (custom_cuts_naming) {

        //<editor-fold desc="Status additions">
        if (apply_nucleon_cuts) {
            Nucleon_Cuts_Status = "wNC_";
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

        if (!apply_nucleon_SmearAndCorr) {
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
                    Additional_Status = "nRes_";
                }
            } else if (Generate_AMaps && plot_and_fit_MomRes) {
                if (!VaryingDelta) {
                    Additional_Status = "nResSS_AMaps_";
                } else {
                    Additional_Status = "nRes_AMaps_";
                }
            }
        } else {
            Additional_Status = "";
        }

        if (!apply_nucleon_cuts) {
            Efficiency_Status = "";
        } else {
            if (Rec_wTL_ES) {
                Efficiency_Status = "Eff2";
            } else {
                Efficiency_Status = "Eff1";
            }
        }
        //</editor-fold>

        if (!apply_cuts) { // Stage 0 - no cuts
            plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-00_NO_CUTS";
            plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-00_NO_CUTS.txt";
        } else {
            string added_names = Nucleon_Cuts_Status + FD_photons_Status + PSmearing_Status + FiducialCuts_Status + KinCuts_Status + KinWei_Status + Additional_Status
                                 + Efficiency_Status;

            if (!apply_chi2_cuts_1e_cut) { // Stage 1 - with cuts except PID (chi2) cuts
                plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-01_AC_woChi2";
                plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-01_AC_woChi2.txt";
            } else if (apply_chi2_cuts_1e_cut) {
                if (!apply_nucleon_cuts) { // Stage 2 - set nucleon cuts (neutron beta fit & proton double detection cuts)
                    plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-02_AC_" + added_names;
                    plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-02_AC_" + added_names + ".txt";
                } else {
                    plots_path = WorkingDirectory + "00_plots_" + SampleName + "_-03_AC_" + added_names;
                    plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-03_AC_" + added_names + ".txt";
                }
            }
        }
    } else {
        Nucleon_Cuts_Status = FD_photons_Status = Efficiency_Status = "";
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

    if (!apply_nucleon_cuts) { apply_nucleon_physical_cuts = false; }

    if (!apply_nucleon_physical_cuts) { apply_nBeta_fit_cuts = apply_fiducial_cuts = apply_kinematical_cuts = apply_kinematical_weights = apply_nucleon_SmearAndCorr = false; }

    if (Generate_AMaps) { apply_fiducial_cuts = false; }

    if (!apply_cuts) {
        cout << "Cuts are disabled:\n";

        apply_Nphe_cut = apply_chi2_cuts_1e_cut = apply_Vz_cuts = apply_dVz_cuts = false;
        apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_DC_fiducial_cuts = false;
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
    cout << "apply_DC_fiducial_cuts:\t\t" << BoolToString(apply_DC_fiducial_cuts) << "\n";
    cout << "apply_nucleon_cuts:\t\t" << BoolToString(apply_nucleon_cuts) << "\n";
    cout << "apply_nucleon_physical_cuts:\t" << BoolToString(apply_nucleon_physical_cuts) << "\n";
    cout << "apply_nBeta_fit_cuts:\t\t" << BoolToString(apply_nBeta_fit_cuts) << "\n";
    cout << "apply_fiducial_cuts:\t\t" << BoolToString(apply_fiducial_cuts) << "\n";
    cout << "apply_kinematical_cuts:\t\t" << BoolToString(apply_kinematical_cuts) << "\n";
    cout << "apply_kinematical_weights:\t" << BoolToString(apply_kinematical_weights) << "\n";
    cout << "apply_nucleon_SmearAndCorr:\t" << BoolToString(apply_nucleon_SmearAndCorr) << "\n\n";
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// Cut declarations -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Cuts declarations">
    /* Log cut values to be used later when applying them. */

    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="clas12ana cuts">
    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */

    /* Chi2 cuts. NOTES:
     * Values for mean and sigma are filled from fit variables (overating these values later).
     * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */

    //<editor-fold desc="Kaon PID cuts">
    /* Kaon pid cuts. Not really applied in our analysis. */
    //</editor-fold>

    /* Vertex cuts */

    if (SampleName == "LH2_data_6GeV_run_015032") {
        Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -15, 5);
    } else {
        //TODO: these are good for Ca48 simulation and data. Check cuts for other targets!
        Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 5);
    }

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
//    DSCuts P_cuts;

    /* PCAL edge cuts (fiducial cuts ,electrons only, FD) */

    /* DC edge cuts (fiducial cuts ,electrons only, FD) */

    /* Momentum thresholds */

    //<editor-fold desc="Momentum thresholds definition">
//    /* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
//    e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
//    p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.4, 9999);
//    n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.4, 9999);
//    pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
//    pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
//    ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
    if (limless_mom_eff_plots) {
        /* If we enforce TL cuts, don't use momentum thresholds on nucleons. */
        e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
        p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
        n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, -9999, 9999);
        pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
        pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
        ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
    } else {
        /* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
        e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
        p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.4, 9999);
        n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.4, 9999);
        pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
        pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
        ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
    }
    //</editor-fold>

    //</editor-fold>

    // Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Other cuts">
    /* Neutron beta cuts (1n & nFDpCD, FD only) */

    /* Neutron momentum cuts (1n & nFDpCD, FD only) */

    /* Truth-level momentum cuts */

    /* FD & CD theta range */

    /* Beta cut (1n, FD) */

    /* Neutron veto cut (1n & nFDpCD, FD) */

    /* Ghost tracks handling (2p & pFDpCD, CD & FD) */

    /* reco. kinematic cuts (based on nucleons' efficiency) */
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Other setup
// ======================================================================================================================================================================

// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="TList definition">
    /* Definition of plots TLists used to save all plots to .root file. */

    /* General plots TList */
    //</editor-fold>

//  Creating directories ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Creating directories">
    /* Code for creating directories.
     * Directory creation is done in the Directories class.
     * Added for the case that plots out folder does not exist and for organization.
     * All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */
    cout << "\nCreating plot directories...";

    directories = Directories(Plots_Folder);

    cout << " done.\n\n";
    //</editor-fold>

// Plot setup --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Plot selector">
    /* Here are boolean variables used to turn ON/OFF the different plots of the code.
       Plot_selector_master must remain true, set it OFF only for debugging. */

    //<editor-fold desc="Plot selector - plot all">
    /* Master plots variable */

    /* Cut variable plots */

    /* Beta plots */

    /* Beta plots */

    /* Angle plots */

    /* Momentum transfer plots */

    /* E_e plots */

    /* ET plots */

    /* Ecal plots */

    /* Transverse variables plots */

    /* ToF plots */

    /* Efficiency plots */

    /* Resolution plots */

    /* Resolution plots */

    /* Final state ratio plots */
    //</editor-fold>

//    //<editor-fold desc="Plot selector - selected plots">
//    /* Master plots variable */
//    bool Plot_selector_master = true; // Master plot selector for analysis
//
//    /* Cut variable plots */
//    bool Cut_plots_master = true; // Master cut plots selector
////    bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true;
//    bool Nphe_plots = false, Chi2_plots = false, Vertex_plots = false, SF_plots = false, fiducial_plots = false;
////
////    bool Momentum_plots = false;
//    bool Momentum_plots = true;
////
//
//    /* Beta plots */
////    bool W_plots = true;
//    bool W_plots = false;
//
//    /* Beta plots */
////    bool Beta_plots = true;
//    bool Beta_plots = false;
////    bool Beta_vs_P_plots = true;
//    bool Beta_vs_P_plots = false;
//
//    /* Angle plots */
//    bool Angle_plots_master = true; // Master angle plots selector
//    bool Theta_e_plots = true, Phi_e_plots = true;
////    bool Angle_plots_master = false; // Master angle plots selector
////    bool Theta_e_plots = false, Phi_e_plots = false;
//
//    /* Momentum transfer plots */
////    bool Momentum_transfer_plots = true;
//    bool Momentum_transfer_plots = false;
//
//    /* E_e plots */
////    bool E_e_plots = true;
//    bool E_e_plots = false;
//
//    /* ET plots */
////    bool ETrans_plots_master = true; // Master ET plots selector
//    bool ETrans_plots_master = false; // Master ET plots selector
//    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;
//
//    /* Ecal plots */
////    bool Ecal_plots = true;
//    bool Ecal_plots = false;
//
//    /* Transverse variables plots */
////    bool TKI_plots = true;
//    bool TKI_plots = false;
//
//    /* ToF plots */
////    bool ToF_plots = true;
//    bool ToF_plots = false;
//
//    /* Efficiency plots */
////    bool Efficiency_plots = true;
//    bool Efficiency_plots = false;
////    bool TL_after_Acceptance_Maps_plots = true;
//    bool TL_after_Acceptance_Maps_plots = false;
//
//    /* Resolution plots */
////    bool Hit_maps_plots = true;
//    bool Hit_maps_plots = false;
//
//    /* Resolution plots */
////    bool Resolution_plots = true;
//    bool Resolution_plots = false;
//
//    /* Final state ratio plots */
////    bool FSR_plots = true;
//    bool FSR_plots = false;
//    //</editor-fold>/

    //<editor-fold desc="Turn off plots by master selectors">
    if (!Plot_selector_master) {
        Cut_plots_master = W_plots = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Momentum_transfer_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
        TKI_plots = ToF_plots = Efficiency_plots = Hit_maps_plots = Resolution_plots = false;
    }

    if (!Cut_plots_master) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }
    if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }
    if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }
    if (!fill_TL_plots) { Efficiency_plots = TL_after_Acceptance_Maps_plots = false; }
    if (!Generate_AMaps) { Hit_maps_plots = false; }
    if (!plot_and_fit_MomRes) { Resolution_plots = false; }
    //</editor-fold>

    /* Other setup variables */
    //</editor-fold>

// Normalization setup -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization setup">
    /* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
     * Enable of presentations only, since event count is important otherwise. */
    if (!normalize_master) { // Disable all normalizations if normalize_master == false
        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = norm_Momentum_plots = false;
        norm_Angle_plots_master = norm_Momentum_transfer_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TKI_plots = false;
    }
    //</editor-fold>

// Delete setup ------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Delete setup">
    /* Clear files from previous runs (to prevent mix fo plots from different codes). */

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

// Histogram setup ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Histogram setup">
    /* Histogram boundary variables. Used to unify histograms to the same boundaries. */

    //<editor-fold desc="Number of histogram bins">
    /* Default */

    /* Beta plots */

    /* W plots */

    /* Angle plots */

    /* Momentum transfer plots */

    /* E_e plots */

    /* ET plots */

    /* E_cal plots */

    /* TKI plots */

    /* Efficiency % acceptance correction plots */

    /* Acceptance maps plots */

    /* Neutron resolution plots */
    //</editor-fold>

    //<editor-fold desc="Histogram limits">
    /* Nphe boundries */

    /* Chi2 boundries */
    if (apply_cuts) { Chi2_boundary = 9; }

    /* Vertex boundries */
    if (apply_cuts) {
        double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
        Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
        Vertex_boundary = Vertex_boundary / 5;
    }

    if (apply_cuts) { /* dV_boundary = 7.5; */ dV_boundary = dVz_cuts.GetUpperCut() * 1.4; }

    /* SF boundries */

    /* Momentum boundries */

    /* W boundries */

    /* Beta boundries */
    if (apply_cuts) { Beta_boundary = 1.25, P_boundary = beamE * 1.1; }

    /* Angle boundries */

    /* TKI boundries */
    //</editor-fold>

    //</editor-fold>

// Acceptance maps generation -------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Acceptance maps generation">
    /* Acceptance maps are handled completely by the AMaps class */
    cout << "\nSetting Acceptance maps...";

    if (!calculate_truth_level) { Generate_AMaps = false; }
    if (!Generate_AMaps) { Hit_maps_plots = false; }
    if (reformat_e_bins) { varying_P_e_bins = false; }

    /* Set Bins by case */
    if (Generate_AMaps) {
        aMaps = AMaps(reformat_e_bins, varying_P_e_bins, beamE, directories.AMaps_Directory_map["AMaps_1e_cut_Directory"],
                      NumberNucOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);
        wMaps = AMaps(reformat_e_bins, varying_P_e_bins, beamE, directories.AMaps_Directory_map["WMaps_1e_cut_Directory"],
                      NumberNucOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins, HistElectronSliceNumOfXBins);
    } else {
        aMaps = AMaps(AcceptanceMapsDirectory, SampleName);
        wMaps = AMaps(AcceptanceWeightsDirectory, SampleName);
    }

    cout << " done.\n\n";
    //</editor-fold>

// Neutron resolution & proton smearing ---------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron resolution">
    /* Neutron resolution fits is handled completely by the MomentumResolution class */
    cout << "\nSetting neutron resolution data...";

    if (!calculate_truth_level) { plot_and_fit_MomRes = false; } // Disable resolution-realted operations if not calculating TL plots
    if (apply_nucleon_SmearAndCorr) { plot_and_fit_MomRes = false; }  // Disable resolution-realted operations when applying proton smearing

    //<editor-fold desc="Neutron resolution class declaration & definition">
    MomentumResolution nRes, pRes;

    if (plot_and_fit_MomRes) {
        nRes = MomentumResolution(SampleName, NucleonCutsDirectory, "Neutron", beamE, n_mom_th.GetLowerCut(),
                                 directories.Resolution_Directory_map["nRes_plots_1n_Directory"], DeltaSlices, VaryingDelta);
        pRes = MomentumResolution(SampleName, NucleonCutsDirectory, "Proton", beamE, p_mom_th.GetLowerCut(),
                                 directories.Resolution_Directory_map["pRes_plots_1p_Directory"], DeltaSlices, VaryingDelta);
    } else {
        nRes.ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_res_fit_param_-_" + SampleName + ".par").c_str(),
                              SampleName, NucleonCutsDirectory);
        nRes.ReadResDataParam((MomentumResolutionDirectory + "Res_data_-_" + SampleName + "/Neutron_res_hist_param_-_" + SampleName + ".par").c_str(),
                              SampleName, NucleonCutsDirectory);
    }
    //</editor-fold>

    cout << " done.\n\n";
    //</editor-fold>

// Acceptance correction data -------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Acceptance correction">
    /* Neutron resolution fits is handled completely by the MomentumResolution class */
    cout << "\nSetting acceptance correction data...";

    if (!calculate_truth_level) { save_ACorr_data = false; }

    cout << " done.\n\n";
    //</editor-fold>

// Debugging setup ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Debugging setup">
    /* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */
    SetGPMonitoringPlots(GoodProtonsMonitorPlots, directories.Angle_Directory_map["CToF_hits_monitoring_2p_Directory"],
                         directories.Angle_Directory_map["Double_detection_monitoring_2p_Directory"]);
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

#endif //CLAS12DANALYSER_H
