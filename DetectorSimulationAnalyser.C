// git pull && clas12root -b -q main.c
/* Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/
*/

//#include "HipoChain.h"

//#include <HipoHist.h>

#include <cstdlib>
#include <iostream>
#include <chrono>
#include <TFile.h>
#include <TTree.h>
#include <TApplication.h>
#include <TROOT.h>
#include <TDatabasePDG.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TBenchmark.h>
#include "clas12reader.h"

//#include "TruthLevelAnalyser.h"
//#include "gst.h"
#include "codeSetup.h"

using namespace clas12;
using namespace std;


#pragma clang diagnostic push
#pragma ide diagnostic ignored "Simplify"

void EventAnalyser() {

    string AnalyserVersion = "Beta version";

    bool selection_test_2p = false, selection_test_1n1p = false;

//  Code settings
//  =====================================================================================================================================================================

    //<editor-fold desc="Code settings">

//  FSI settings --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="FSI settings">
    bool FSI_status; // true == with FSI; false == no FSI
    bool custom_FSI_status = true; // set as true by default

    int ni_selection; // 3 for G18; 2 for SuSAv2
//    int ni_selection = 2; // 3 for G18; 2 for SuSAv2

    //<editor-fold desc="Input processing (to determine custom_FSI_status)">
    string LoadedInput = AnalyseFile;
//    string LoadedInput = "/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo";
    string filePath = LoadedInput.substr(0, LoadedInput.find_last_of('/') + 1);
    string fileInput = LoadedInput.substr(LoadedInput.find_last_of('/') + 1);
    string plotsInput = fileInput.substr(0, fileInput.find_last_of(".root") - 4);

    if (fileInput.find("nofsi") <= fileInput[fileInput.size() - 1]) {
////        cout << "\n";
//        cout << "\n";
//        cout << "nofsi is in fileInput! Setting custom_FSI_status == false." << "\n";
//        cout << "\n";
//
        custom_FSI_status = false; // and custom_FSI_status is set to be false
    }

//    if (fileInput.find("wfsi") <= fileInput[fileInput.size() - 1]) {
////        cout << "\n";
////        cout << "\n";
////        cout << "wfsi is in fileInput! Setting custom_FSI_status == true." << "\n";  // and no change to custom_FSI_status
////        cout << "\n";
//    } else if (fileInput.find("nofsi") <= fileInput[fileInput.size() - 1]) {
////        cout << "\n";
//        cout << "\n";
//        cout << "nofsi is in fileInput! Setting custom_FSI_status == false." << "\n";
//        cout << "\n";
//
//        custom_FSI_status = false; // and custom_FSI_status is set to be false
//    } else {
////        cout << "\n";
//        cout << "\n";
//        cout << "Could not figure FSI status. FSI is kept ON." << "\n";
//        cout << "\n";
//    }
    //</editor-fold>

    //<editor-fold desc="Input processing (to determine ni_selection)">
    string tune;

    if (fileInput.find("G18") <= fileInput[fileInput.size() - 1]) {
        cout << "\n";
        cout << "G18 is in fileInput! Setting ni_selection == 3\n";  // and no change to custom_FSI_status
        cout << "\n";

        tune = "G18";
        ni_selection = 3;
    } else if (fileInput.find("GEM21") <= fileInput[fileInput.size() - 1]) {
        cout << "\n";
        cout << "GEM21 is in fileInput! Setting ni_selection == 2\n";
        cout << "\n";

        tune = "GEM21 (SuSAv2)";
        ni_selection = 2;
    } else if (fileInput.find("SuSAv2") <= fileInput[fileInput.size() - 1]) {
        cout << "\n";
        cout << "SuSAv2 is in fileInput! Setting ni_selection == 2\n";
        cout << "\n";

        tune = "SuSAv2";
        ni_selection = 2;
    } else {
        cout << "\n";
        cout << "Could not figure tune, keeping ni = 2\n";
        cout << "\n";

        ni_selection = 2;
        tune = "unknown";
    }
    //</editor-fold>

    //<editor-fold desc="Set FSI according to setting mode">
    if (file_name == "12C_056GeV_G18_10a_02_11a" || file_name == "12C_0961GeV_G18_10a_02_11a" || file_name == "C12_1161GeV_EM+MEC_G18_02a_00_000_Q2_0_1" ||
        file_name == "12C_1299GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000" ||
        file_name == "adi_11_1000060120_2222_fsi.gst" || file_name == "GENIE_with_fsi") {
        FSI_status = true;
    } else if (file_name == "asportes_11_1000060120_2222_nofsi_10M.gst" || file_name == "GENIE_no_fsi") {
        FSI_status = false;
    } else if (file_name == "general_file") {
        FSI_status = custom_FSI_status;
    }
    //</editor-fold>

    //<editor-fold desc="FSI indicator">
    cout << "\n";
    cout << "\n";
    cout << "Execution variables\n";
    cout << "---------------------------------------------------------------------------\n";
    cout << "File input:\t" << AnalyseFile << "\n";
    cout << "Settings mode:\t'" << file_name << "'\n";
    cout << "FSI status:\t" << BoolToString(FSI_status) << "\n";
    cout << "ni:\t\t" << ni_selection << "\n";
    cout << "Tune:\t\t" << tune << "\n";
    cout << "\n";
    cout << "\n";

    cout << "File input\n";
    cout << "---------------------------------------------------------------------------\n";
    cout << "LoadedInput:\t" << LoadedInput << "\n";
    cout << "filePath:\t" << filePath << "\n";
    cout << "fileInput:\t" << fileInput << "\n";
    cout << "\n";
    cout << "\n";
    //</editor-fold>

    //</editor-fold>

// Plot settings --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Plot settings">
//    bool lowest_nentries = false;
//    int custom_nentries;
//
//    if (lowest_nentries == true) {
//        custom_nentries = 10000000; // 10M entries
//    }

    bool wider_margin = true;
    //</editor-fold>

// Calculation settings -------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Calculation settings">
    bool calculate_2p = true, calculate_1n1p = true, calculate_MicroBooNE = true;

    bool BEnergyToNucleusCon = false; // For QEL ONLY!!!

//    //<editor-fold desc="Simulation parameters extraction (assuming all entries have the same parameters)">
//    if (fChain == 0) return;
//
//    Long64_t nbytes0 = 0, nb0 = 0;
//    x = 0;
//
//    for (Long64_t jentry0 = 0; jentry0 < 1; jentry0++) {
//        Long64_t ientry = LoadTree(jentry0);
//
//        if (ientry < 0) break;
//
//        nb0 = fChain->GetEntry(jentry0);
//        nbytes0 += nb0;
//
////        //<editor-fold desc="Energy selector (to nucleus; relevant to QEL only)">
////        switch (tgt) {
////            case 1000020040: // He4
////                BEnergyToNucleus = 0.0150;
////                Target_pdg = 1000020040;
////                Target_nucleus = "He4";
////                break;
////            case 1000030060: // Li6
////                BEnergyToNucleus = 0.0150;
////                Target_pdg = 1000030060;
////                Target_nucleus = "Li6";
////                break;
////            case 1000060120: // C12
////                BEnergyToNucleus = 0.0200;
////                Target_pdg = 1000060120;
////                Target_nucleus = "C12";
////                break;
////            case 1000080160: // O16
////                BEnergyToNucleus = 0.0160;
////                Target_pdg = 1000080160;
////                Target_nucleus = "O16";
////                break;
////            case 1000120240: // Mg24
////                BEnergyToNucleus = 0.0250;
////                Target_pdg = 1000120240;
////                Target_nucleus = "Mg24";
////                break;
////            case 1000180400: // Ar40
////                BEnergyToNucleus = 0.0280;
////                Target_pdg = 1000120240;
////                Target_nucleus = "Ar40";
////                break;
////            case 1000200400: // Ca40
////                BEnergyToNucleus = 0.0280;
////                Target_pdg = 1000200400;
////                Target_nucleus = "Ca40";
////                break;
////            case 1000260560: // Fe56
////                BEnergyToNucleus = 0.0230;
////                Target_pdg = 1000260560;
////                Target_nucleus = "Fe56";
////                break;
////            case 1000280580: // Ni58
////                BEnergyToNucleus = 0.0300;
////                Target_pdg = 1000280580;
////                Target_nucleus = "Ni58";
////                break;
////            case 1000501190: // Sn119
////                BEnergyToNucleus = 0.0280;
////                Target_pdg = 1000501190;
////                Target_nucleus = "Sn119";
////                break;
////            case 1000791970: // Au197
////                BEnergyToNucleus = 0.0310;
////                Target_pdg = 1000791970;
////                Target_nucleus = "Au197";
////                break;
////            case 1000822080: // Pb208
////                BEnergyToNucleus = 0.0310;
////                Target_pdg = 1000822080;
////                Target_nucleus = "Pb208";
////                break;
////            default: // In case tgt does not correspond to any the above pdg codes - no BE considerations
////                BEnergyToNucleus = 0.;
////                Target_pdg = tgt;
////                Target_nucleus = "Unknown";
////        }
////
////        BeamEnergy = Ev;
////        //</editor-fold>
//
////        //<editor-fold desc="Probe selector">
////        Probe_pdg = neu;
////
////        switch (Probe_pdg) {
////            case 11: // electron
////                Probe = "e-";
////                break;
////            case 12: // nu_e
////                Probe = "nu_e";
////                break;
////            case 13: // muon
////                Probe = "mu-";
////                break;
////            case 14: // nu_mu
////                Probe = "nu_mu";
////                break;
////            case 15: // tauon
////                Probe = "tau-";
////                break;
////            case 16: // nu_tau
////                Probe = "nu_tau";
////                break;
////            default: // In case tgt does not correspond to any the above pdg codes - no BE considerations
////                Probe = "Unknown";
////        }
////        //</editor-fold>
//
//        cout << "Event parameters (*)\n";
//        cout << "---------------------------------------------------------------------------\n";
//        cout << "Probe:\t\t" << Probe << " (PDG: " << Probe_pdg << ")\n";
//        cout << "Target:\t\t" << Target_nucleus << " (PDG: " << Target_pdg << ")\n";
//        cout << "Beam energy:\t" << BeamEnergy << " GeV\n";
//        cout << "\n";
//        cout << "(*) assuming all entries have the same parameters\n";
//        cout << "\n";
//        cout << "\n";
//    }
//    //</editor-fold>

    //</editor-fold>

// Plot selector --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Plot selector">
    bool Theta_plots = true, Phi_plots = true;

    bool Energy_histogram_plots = true;

    bool ET_plots = true, ET_all_plots = true, ET_QEL_plots = true, ET_MEC_plots = true, ET_RES_plots = true, ET_DIS_plots = true;
    if (ET_plots == false) {
        bool ET_all_plots = false, ET_QEL_plots = false, ET_MEC_plots = false, ET_RES_plots = false, ET_DIS_plots = false;
    }

    bool inclusive_plots = true;

    bool E_cal_plots = true, other_E_cal_plots = false;

    bool momentum_plots = true;

    bool MicroBooNE_plots = true;
    //</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization settings">
    bool normalize_master = false;

    bool normalized_theta_l_plots = true;
    bool normalized_theta_p1_plots = false, normalized_theta_p2_plots = false, normalized_dtheta_2p_plots = false; // 2p
    bool normalized_theta_p_plots = false, normalized_theta_n_plots = false, normalized_dtheta_1n1p_plots = false; // 1n1p

    bool normalized_phi_l_plots = false;
    bool normalized_phi_p1_plots = false, normalized_phi_p2_plots = false, normalized_dphi_2p_plots = false; // 2p
    bool normalized_phi_p_plots = false, normalized_phi_n_plots = false, normalized_dphi_1n1p_plots = false; // 1n1p

    bool normalized_E_l_plots = true;
    bool normalized_E_l_all_int_plots = true, normalized_E_l_QEL_plots = true, normalized_E_l_MEC_plots = true, normalized_E_l_RES_plots = true, normalized_E_l_DIS_plots = true;

    bool normalized_E_Trans_all_ang_all_int_plots = true;
    bool normalized_E_Trans15_plots = true;
    bool normalized_E_cal_plots = true;
    bool normalized_inclusive_plots = true;

    bool normalized_P_lp_plots = false; // 2p & 1n1p
    bool normalized_P_L_plots = false; // 2p & 1n1p
    bool normalized_P_R_plots = false; // 2p & 1n1p

    if (normalize_master == false) {
        normalized_theta_l_plots = false;

        normalized_theta_p1_plots = false;
        normalized_theta_p2_plots = false;
        normalized_dtheta_2p_plots = false; // 2p

        normalized_theta_p_plots = false;
        normalized_theta_n_plots = false;
        normalized_dtheta_1n1p_plots = false; // 1n1p

        normalized_phi_l_plots = false;

        normalized_phi_p1_plots = false;
        normalized_phi_p2_plots = false;
        normalized_dphi_2p_plots = false; // 2p

        normalized_phi_p_plots = false;
        normalized_phi_n_plots = false;
        normalized_dphi_1n1p_plots = false; // 1n1p

        normalized_E_l_plots = false;
        normalized_E_l_all_int_plots = false;
        normalized_E_l_QEL_plots = false;
        normalized_E_l_MEC_plots = false;
        normalized_E_l_RES_plots = false;
        normalized_E_l_DIS_plots = false;

        normalized_E_Trans_all_ang_all_int_plots = false;
        normalized_E_Trans15_plots = false;
        normalized_E_cal_plots = false;
        normalized_inclusive_plots = false;

        normalized_P_lp_plots = false; // 2p & 1n1p
        normalized_P_L_plots = false; // 2p & 1n1p
        normalized_P_R_plots = false; // 2p & 1n1p

        cout << "\n";
        cout << "All normalizations are disabled.\n";  // and no change to custom_FSI_status
        cout << "\n";
    }

    if (normalized_E_l_plots == false) {
        normalized_E_l_all_int_plots = false;
        normalized_E_l_QEL_plots = false;
        normalized_E_l_MEC_plots = false;
        normalized_E_l_RES_plots = false;
        normalized_E_l_DIS_plots = false;
    }
    //</editor-fold>

// Delete settings ------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Delete settings">
    bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

    //<editor-fold desc="Deleting files by cases">
    if (delete_png_files == true && delete_root_files == false) {
        cout << "\n";
        cout << "Clearing old plots...\n";
        cout << "\n";

        system("find ./plots -type f -iname '*.png' -delete"); // Delete existing .png files
    } else if (delete_png_files == false && delete_root_files == true) {
        cout << "\n";
        cout << "Clearing old root files...\n";
        cout << "\n";

        system("find ./plots -type f -iname '*.root' -delete"); // Delete existing .root files
    } else if (delete_png_files == true && delete_root_files == true) {
        cout << "\n";
        cout << "Clearing old plots & root files...\n";
        cout << "\n";

        system("find ./plots -type f -iname '*.png' -delete"); // Delete existing .png files
        system("find ./plots -type f -iname '*.root' -delete"); // Delete existing .root files
    } else {
        cout << "\n";
        cout << "No files were cleared.\n";
        cout << "\n";
    }
    //</editor-fold>

    //</editor-fold>

// TList setup ----------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="TList name setup">
    string listName = plots_path + plotsInput + plots_file_type; //todo: add if-else to choose plotsInput or file_name
    const char *TListName = listName.c_str();
    //</editor-fold>

// Momentum thresholds --------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum thresholds (2p)">
    double P_l_upper_lim_2p = -1, P_l_lower_lim_2p = -1;
    double P_p1_upper_lim_2p = -1, P_p1_lower_lim_2p = 0.3;
    double P_p2_upper_lim_2p = -1, P_p2_lower_lim_2p = 0.3;
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds (1n1p)">
    double P_l_upper_lim_1n1p = -1, P_l_lower_lim_1n1p = -1;
    double P_p_upper_lim_1n1p = -1, P_p_lower_lim_1n1p = 0.3;
    double P_n_upper_lim_1n1p = -1, P_n_lower_lim_1n1p = 0.3;
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds (2p, MicroBooNE)">
    double P_l_upper_lim_MicroBooNE = 1.2, P_l_lower_lim_MicroBooNE = 0.1;
    double P_L_upper_lim_MicroBooNE = 1.0, P_L_lower_lim_MicroBooNE = 0.3;
    double P_R_upper_lim_MicroBooNE = 1.0, P_R_lower_lim_MicroBooNE = 0.3;
    double P_pion_upper_lim_MicroBooNE = 0.065;
    //</editor-fold>

// Histogram limits ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Histogram limits">

    //<editor-fold desc="Histogram limits for every case">

    //<editor-fold desc="Theta histograms">

    //<editor-fold desc="Theta histograms (2p)">
    theta_l_upper_lim_2p = 60;
    theta_l_lower_lim_2p = 0;
    theta_p1_upper_lim_2p = 190;
    theta_p1_lower_lim_2p = -10;
    theta_p2_upper_lim_2p = 190;
    theta_p2_lower_lim_2p = -10;
    dtheta_upper_lim_2p = 190;
    dtheta_lower_lim_2p = -10;
    //</editor-fold>

    //<editor-fold desc="Theta histograms (1n1p)">
    theta_l_upper_lim_1n1p = 60;
    theta_l_lower_lim_1n1p = 0;
    theta_p_upper_lim_1n1p = 190;
    theta_p_lower_lim_1n1p = -10;
    theta_n_upper_lim_1n1p = 190;
    theta_n_lower_lim_1n1p = -10;
    dtheta_upper_lim_1n1p = 190;
    dtheta_lower_lim_1n1p = -10;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Phi histograms">

    //<editor-fold desc="Phi histograms (2p)">
    phi_l_upper_lim_2p = 200;
    phi_l_lower_lim_2p = -200;
    phi_p1_upper_lim_2p = 200;
    phi_p1_lower_lim_2p = -200;
    phi_p2_upper_lim_2p = 200;
    phi_p2_lower_lim_2p = -200;
    dphi_upper_lim_2p = 360;
    dphi_lower_lim_2p = -360;
    //</editor-fold>

    //<editor-fold desc="Phi histograms (1n1p)">
    phi_l_upper_lim_1n1p = 200;
    phi_l_lower_lim_1n1p = -200;
    phi_p_upper_lim_1n1p = 200;
    phi_p_lower_lim_1n1p = -200;
    phi_n_upper_lim_1n1p = 200;
    phi_n_lower_lim_1n1p = -200;
    dphi_upper_lim_1n1p = 360;
    dphi_lower_lim_1n1p = -360;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="MicroBooNE plots">

    //<editor-fold desc="MicroBooNE gamma plots (unweighted)">
    gamma_Lab_hist_upper_lim_2p = 1;
    gamma_Lab_hist_lower_lim_2p = -1;
    gamma_mu_p_tot_upper_lim_2p = 1;
    gamma_mu_p_tot_lower_lim_2p = -1;
    //</editor-fold>

    //<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
    gamma_Lab_hist_weighted_upper_lim_1n1p = 1;
    gamma_Lab_hist_weighted_lower_lim_1n1p = -1;
    gamma_mu_p_tot_weighted_upper_lim_1n1p = 1;
    gamma_mu_p_tot_weighted_lower_lim_1n1p = -1;
    //</editor-fold>

    //<editor-fold desc="MicroBooNE gamma plots (no pions, for every interaction)">
    gamma_Lab_all_hist_upper_lim = 1;
    gamma_Lab_all_hist_lower_lim = -1;
    gamma_Lab_all_hist_weighted_upper_lim = 1;
    gamma_Lab_all_hist_weighted_lower_lim = -1;
    gamma_Lab_QEL_hist_upper_lim = 1;
    gamma_Lab_QEL_hist_lower_lim = -1;
    gamma_Lab_QEL_hist_weighted_upper_lim = 1;
    gamma_Lab_QEL_hist_weighted_lower_lim = -1;
    gamma_Lab_MEC_hist_upper_lim = 1;
    gamma_Lab_MEC_hist_lower_lim = -1;
    gamma_Lab_MEC_hist_weighted_upper_lim = 1;
    gamma_Lab_MEC_hist_weighted_lower_lim = -1;
    gamma_Lab_RES_hist_upper_lim = 1;
    gamma_Lab_RES_hist_lower_lim = -1;
    gamma_Lab_RES_hist_weighted_upper_lim = 1;
    gamma_Lab_RES_hist_weighted_lower_lim = -1;
    gamma_Lab_DIS_hist_upper_lim = 1;
    gamma_Lab_DIS_hist_lower_lim = -1;
    gamma_Lab_DIS_hist_weighted_upper_lim = 1;
    gamma_Lab_DIS_hist_weighted_lower_lim = -1;
    //</editor-fold>

    //<editor-fold desc="MicroBooNE dP_T plots">
    if (FSI_status == true) {
        dP_T_hist_upper_lim = 2;
        dP_T_hist_lower_lim = 0;
        dP_T_hist_weighted_upper_lim = 2;
        dP_T_hist_weighted_lower_lim = 0;
    } else if (FSI_status == false) {
        dP_T_hist_upper_lim = 2;
        dP_T_hist_lower_lim = 0;
        dP_T_hist_weighted_upper_lim = 2;
        dP_T_hist_weighted_lower_lim = 0;
    }
    //</editor-fold>

    //<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
    P_L_hist_upper_lim = 3.0;
    P_L_hist_lower_lim = 0;
    P_R_hist_upper_lim = 3.0;
    P_R_hist_lower_lim = 0;
    P_l_hist_upper_lim = 3.0;
    P_l_hist_lower_lim = 0;
    P_pion_hist_upper_lim = 0.1;
    P_pion_hist_lower_lim = 0;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Histogram limits by cases">

    if (file_name == "12C_056GeV_G18_10a_02_11a") {

        //<editor-fold desc="12C_056GeV_G18_10a_02_11a histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        fsEl_upper_lim_2p = 0.6;
        fsEl_lower_lim_2p = 0;
        fsEl_QEL_upper_lim_2p = 0.6;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 0.6;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 0.6;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 0.6;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_2p_x = 0;
        fsEl_VS_theta_l_upper_lim_2p_y = 0.6;
        fsEl_VS_theta_l_lower_lim_2p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 0.6;
        fsEl_lower_lim_1n1p = 0;
        fsEl_QEL_upper_lim_1n1p = 0.6;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 0.6;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 0.6;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 0.6;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_y = 0.6;
        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 0.6;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 0.6;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 0.6;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 0.6;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 0.6;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 0.6;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 0.6;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 0.6;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 0.6;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 0.6;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 0.6;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 0.6;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 0.6;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 0.6;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 0.6;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 0.6;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 0.6;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 0.6;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 0.6;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 0.6;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 0.6;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 0.6;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 0.6;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 0.6;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 0.6;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 0.6;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 0.6;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 0.6;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 0.6;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 0.6;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 0.6;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 0.6;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 0.11;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 0.11;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 0.11;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 0.11;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 0.11;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 0.9;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 0.6;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 0.9;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 0.6;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 0.9;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 0.6;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 0.9;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 0.6;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 0.9;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 0.6;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 0.9;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 0.6;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 0.9;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 0.6;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 0.9;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 0.6;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 0.9;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 0.6;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 0.65;
        E_cal_QEL_lower_lim_range = 0.45;
        E_cal_MEC_upper_lim_range = 0.65;
        E_cal_MEC_lower_lim_range = 0.45;
        E_cal_RES_upper_lim_range = 0.65;
        E_cal_RES_lower_lim_range = 0.45;
        E_cal_DIS_upper_lim_range = 0.65;
        E_cal_DIS_lower_lim_range = 0.45;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 1.5;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 1.5;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 1.5;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 1.5;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 1.5;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 1.5;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "12C_0961GeV_G18_10a_02_11a") {

        //<editor-fold desc="12C_0961GeV_G18_10a_02_11a histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        fsEl_upper_lim_2p = 1.0;
        fsEl_lower_lim_2p = 0;
        fsEl_QEL_upper_lim_2p = 1.0;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 1.0;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 1.0;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 1.0;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_2p_x = 0;
        fsEl_VS_theta_l_upper_lim_2p_y = 1.0;
        fsEl_VS_theta_l_lower_lim_2p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 1.0;
        fsEl_lower_lim_1n1p = 0;
        fsEl_QEL_upper_lim_1n1p = 1.0;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 1.0;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 1.0;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 1.0;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.0;
        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 0.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 0.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 0.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 0.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 0.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 0.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 0.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 0.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 0.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 0.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 0.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 0.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 0.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 0.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 0.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 0.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 0.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 0.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 0.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 0.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 0.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 0.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 0.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 0.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 0.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 0.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 0.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 0.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 0.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 0.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 0.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 0.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 0.75;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 0.75;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 0.75;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 0.75;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 0.75;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.5;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.0;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.5;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.0;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.5;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.0;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 1.5;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 1.0;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.5;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.0;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.5;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.0;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.5;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.0;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.5;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.0;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.5;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.0;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 1.1;
        E_cal_QEL_lower_lim_range = 0.8;
        E_cal_MEC_upper_lim_range = 1.1;
        E_cal_MEC_lower_lim_range = 0.8;
        E_cal_RES_upper_lim_range = 1.1;
        E_cal_RES_lower_lim_range = 0.8;
        E_cal_DIS_upper_lim_range = 1.1;
        E_cal_DIS_lower_lim_range = 0.8;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

//        todo: whay is BEnergyToNucleusCon used here? find out.
        //<editor-fold desc="Momentum histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            P_L_hist_upper_lim_2p = 1.5;
            P_L_hist_lower_lim_2p = 0;
            P_R_hist_upper_lim_2p = 1.5;
            P_R_hist_lower_lim_2p = 0;
            P_lp_hist_upper_lim_2p = 1.5;
            P_lp_hist_lower_lim_2p = 0;
        } else if (BEnergyToNucleusCon == false) {
            P_L_hist_upper_lim_2p = 1.5;
            P_L_hist_lower_lim_2p = 0;
            P_R_hist_upper_lim_2p = 1.5;
            P_R_hist_lower_lim_2p = 0;
            P_lp_hist_upper_lim_2p = 1.5;
            P_lp_hist_lower_lim_2p = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 1.5;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 1.5;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 1.5;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "C12_1161GeV_EM+MEC_G18_02a_00_000_Q2_0_1") {

        //<editor-fold desc="C12_1161GeV_EM+MEC_G18_02a_00_000_Q2_0_1 histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        fsEl_upper_lim_2p = 1.2;
        fsEl_lower_lim_2p = 0;
        fsEl_QEL_upper_lim_2p = 1.2;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 1.2;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 1.2;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 1.2;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_2p_x = 0;
        fsEl_VS_theta_l_upper_lim_2p_y = 1.2;
        fsEl_VS_theta_l_lower_lim_2p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 2.21;
        fsEl_lower_lim_1n1p = 1;
        fsEl_QEL_upper_lim_1n1p = 1.2;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 1.2;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 1.2;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 1.2;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.2;
        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 2.0;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 2.0;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 2.0;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 2.0;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 2.0;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 2.0;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 2.0;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 2.0;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 2;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 2;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 2;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 2;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 2;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 2;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 2;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 2;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 2;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 2;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 2;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 2;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 2;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 2;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 2;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 2;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 2;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 2;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only)">
        E_Trans15_DIS_upper_lim_2p = 2;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 2;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 2;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">
        E_Trans15_DIS_upper_lim_1n1p = 2;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 2;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 2;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 1.2;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 1.2;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 1.2;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 1.2;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 1.2;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.8;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.2;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.8;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.2;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.8;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.2;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 1.8;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 1.2;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.8;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.2;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.8;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.2;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.8;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.2;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.8;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.2;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.8;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.2;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 1.2;
        E_cal_QEL_lower_lim_range = 0;
        E_cal_MEC_upper_lim_range = 1.2;
        E_cal_MEC_lower_lim_range = 0;
        E_cal_RES_upper_lim_range = 1.2;
        E_cal_RES_lower_lim_range = 0;
        E_cal_DIS_upper_lim_range = 1.2;
        E_cal_DIS_lower_lim_range = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 1.2;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 1.2;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 1.2;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 1.2;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 1.2;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 1.2;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "12C_1299GeV_G18_10a_02_11a") {

        //<editor-fold desc="12C_1299GeV_G18_10a_02_11a histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        fsEl_upper_lim_2p = 1.3;
        fsEl_lower_lim_2p = 0;
        fsEl_QEL_upper_lim_2p = 1.3;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 1.3;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 1.3;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 1.3;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_2p_x = 0;
        fsEl_VS_theta_l_upper_lim_2p_y = 1.3;
        fsEl_VS_theta_l_lower_lim_2p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 1.3;
        fsEl_lower_lim_1n1p = 0;
        fsEl_QEL_upper_lim_1n1p = 1.3;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 1.3;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 1.3;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 1.3;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.3;
        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 0.8;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 0.8;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 0.8;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 0.8;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 0.8;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 0.8;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 0.8;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 0.8;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 0.8;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 0.8;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 0.8;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 0.8;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 0.8;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 0.8;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 0.8;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 0.8;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 0.8;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 0.8;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 0.8;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 0.8;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 0.8;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 0.8;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 0.8;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 0.8;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 0.8;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 0.8;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 0.8;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 0.8;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 0.8;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 0.8;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 0.8;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 0.8;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 1.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 1.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 1.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 1.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 1.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.9;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.4;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.9;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.4;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.9;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.4;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 1.9;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 1.4;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.9;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.4;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.9;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.4;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.9;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.4;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.9;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.4;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.9;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.4;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 1.4;
        E_cal_QEL_lower_lim_range = 1.15;
        E_cal_MEC_upper_lim_range = 1.4;
        E_cal_MEC_lower_lim_range = 1.15;
        E_cal_RES_upper_lim_range = 1.4;
        E_cal_RES_lower_lim_range = 1.15;
        E_cal_DIS_upper_lim_range = 1.4;
        E_cal_DIS_lower_lim_range = 1.15;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 1.5;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 1.5;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 1.5;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 1.5;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 1.5;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 1.5;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "12C_2222GeV_G18_10a_02_11a") {

        //<editor-fold desc="12C_2222GeV_G18_10a_02_11a histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "GENIE_with_fsi") {

        //<editor-fold desc="GENIE_with_fsi histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "GENIE_no_fsi") {

        //<editor-fold desc="GENIE_no_fsi histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
//                fsEl_upper_lim_1n1p = 2.3;
//                fsEl_lower_lim_1n1p = 1.6;
//                fsEl_QEL_upper_lim_1n1p = 2.3;
//                fsEl_QEL_lower_lim_1n1p = 1.6;
//                fsEl_MEC_upper_lim_1n1p = 2.3;
//                fsEl_MEC_lower_lim_1n1p = 1.6;
//                fsEl_RES_upper_lim_1n1p = 2.3;
//                fsEl_RES_lower_lim_1n1p = 1.6;
//                fsEl_DIS_upper_lim_1n1p = 2.3;
//                fsEl_DIS_lower_lim_1n1p = 1.6;
//                fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
//                fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
//                fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
//                fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "12C_2222GeV_GTEST19_10b_00_000") {

        //<editor-fold desc="12C_2222GeV_GTEST19_10b_00_000 histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "adi_11_1000060120_2222_fsi.gst") {

        //<editor-fold desc="adi_11_1000060120_2222_fsi.gst histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "asportes_11_1000060120_2222_nofsi_10M.gst") {

        //<editor-fold desc="asportes_11_1000060120_2222_nofsi_10M.gst histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "40Ar_BNBFluxGeV_EM+MEC_G18_02a_00_000_Q2_0_0") {

        //<editor-fold desc="40Ar_BNBFluxGeV_EM+MEC_G18_02a_00_000_Q2_0_0 histogram limits">

        //<editor-fold desc="Energy histograms (2p)">
        fsEl_upper_lim_2p = 3;
        fsEl_lower_lim_2p = 0;
        fsEl_QEL_upper_lim_2p = 3;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 3;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 3;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 3;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_2p_x = 0;
        fsEl_VS_theta_l_upper_lim_2p_y = 3;
        fsEl_VS_theta_l_lower_lim_2p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 2.21;
        fsEl_lower_lim_1n1p = 1;
        fsEl_QEL_upper_lim_1n1p = 3;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 3;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 3;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 3;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_y = 3;
        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 3.0;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 3.0;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 3.0;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 3.0;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 3.0;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 3.0;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 3.0;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 3.0;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 3.0;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 3.0;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 3.0;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 3.0;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 3.0;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 3.0;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 3.0;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 3.0;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 3.0;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 3.0;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 3.0;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 3.0;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 3.0;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 3.0;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 3.0;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 3.0;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 3.0;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 3.0;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 3.0;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 3.0;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 3.0;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 3.0;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 3.0;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 3.0;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 3;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 3;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 3;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 3;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 3;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 3;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 3;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        E_cal_QEL_upper_lim_2p = 3.1;
        E_cal_QEL_lower_lim_2p = 3.1;
        E_cal_MEC_upper_lim_2p = 3.1;
        E_cal_MEC_lower_lim_2p = 3.1;
        E_cal_RES_upper_lim_2p = 3.1;
        E_cal_RES_lower_lim_2p = 3.1;
        E_cal_DIS_upper_lim_2p = 3.1;
        E_cal_DIS_lower_lim_2p = 3.1;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        E_cal_QEL_upper_lim_1n1p = 3.1;
        E_cal_QEL_lower_lim_1n1p = 3.1;
        E_cal_MEC_upper_lim_1n1p = 3.1;
        E_cal_MEC_lower_lim_1n1p = 3.1;
        E_cal_RES_upper_lim_1n1p = 3.1;
        E_cal_RES_lower_lim_1n1p = 3.1;
        E_cal_DIS_upper_lim_1n1p = 3.1;
        E_cal_DIS_lower_lim_1n1p = 3.1;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 3.0;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 3.0;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 3.0;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 3.0;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 3.0;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 3.0;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "56Fe_1299GeV_G18_10a_02_11a") {

        //<editor-fold desc="56Fe_1299GeV_G18_10a_02_11a histogram limits">

        //<editor-fold desc="Energy histograms (2p)">
        fsEl_upper_lim_2p = 1.3;
        fsEl_lower_lim_2p = 0;
        fsEl_QEL_upper_lim_2p = 1.3;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 1.3;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 1.3;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 1.3;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_l_upper_lim_2p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_2p_x = 0;
        fsEl_VS_theta_l_upper_lim_2p_y = 1.3;
        fsEl_VS_theta_l_lower_lim_2p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 2.21;
        fsEl_lower_lim_1n1p = 1;
        fsEl_QEL_upper_lim_1n1p = 1.3;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 1.3;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 1.3;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 1.3;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_x = 180.0;
        fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_l_upper_lim_1n1p_y = 1.3;
        fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.3;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.3;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.3;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.3;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.3;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.3;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.3;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.3;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.3;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.3;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.3;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.3;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.3;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.3;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.3;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.3;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.3;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.3;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.3;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.3;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.3;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.3;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.3;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.3;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.3;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.3;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.3;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.3;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.3;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.3;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.3;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.3;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 1.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 1.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 1.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 1.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 1.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 1.3;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 1.4;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 1.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 1.4;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 1.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 1.4;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 1.3;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 1.4;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 1.3;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 1.4;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 1.3;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 1.4;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 1.3;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 1.4;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 1.3;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 1.4;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 1.3;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 1.4;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        E_cal_QEL_upper_lim_2p = 1.5;
        E_cal_QEL_lower_lim_2p = 1.5;
        E_cal_MEC_upper_lim_2p = 1.5;
        E_cal_MEC_lower_lim_2p = 1.5;
        E_cal_RES_upper_lim_2p = 1.5;
        E_cal_RES_lower_lim_2p = 1.5;
        E_cal_DIS_upper_lim_2p = 1.5;
        E_cal_DIS_lower_lim_2p = 1.5;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        E_cal_QEL_upper_lim_1n1p = 1.5;
        E_cal_QEL_lower_lim_1n1p = 1.5;
        E_cal_MEC_upper_lim_1n1p = 1.5;
        E_cal_MEC_lower_lim_1n1p = 1.5;
        E_cal_RES_upper_lim_1n1p = 1.5;
        E_cal_RES_lower_lim_1n1p = 1.5;
        E_cal_DIS_upper_lim_1n1p = 1.5;
        E_cal_DIS_lower_lim_1n1p = 1.5;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 1.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 1.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 1.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 1.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 1.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 1.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

    } else if (file_name == "general_file") {

        //<editor-fold desc="12C_2222GeV_GTEST19_10b_00_000 histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
        if (FSI_status == true) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 70.0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_2p = 2.21;
            fsEl_lower_lim_2p = 1;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_l_upper_lim_2p_x = 70.0;
//            fsEl_VS_theta_l_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_2p_x = 0;
            fsEl_VS_theta_l_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_2p_y = 0;
        }
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        if (FSI_status == true) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 70.0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        } else if (FSI_status == false) {
            fsEl_upper_lim_1n1p = 2.3;
            fsEl_lower_lim_1n1p = 1;
            fsEl_QEL_upper_lim_1n1p = 2.3;
            fsEl_QEL_lower_lim_1n1p = 0;
            fsEl_MEC_upper_lim_1n1p = 2.3;
            fsEl_MEC_lower_lim_1n1p = 0;
            fsEl_RES_upper_lim_1n1p = 2.3;
            fsEl_RES_lower_lim_1n1p = 0;
            fsEl_DIS_upper_lim_1n1p = 2.3;
            fsEl_DIS_lower_lim_1n1p = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_x = 70.0;
//            fsEl_VS_theta_l_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_l_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_l_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_l_lower_lim_1n1p_y = 0;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms">

        //<editor-fold desc="Energy Transfer histograms (all interactions)">

        //<editor-fold desc="Energy Transfer histograms (all interactions, 2p)">
        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_2p = 0;
        E_Trans15_all_upper_lim_2p = 1.7;
        E_Trans15_all_lower_lim_2p = 0;
        E_Trans45_all_upper_lim_2p = 1.7;
        E_Trans45_all_lower_lim_2p = 0;
        E_Trans90_all_upper_lim_2p = 1.7;
        E_Trans90_all_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (all interactions, 1n1p)">
        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
        E_Trans15_all_upper_lim_1n1p = 1.7;
        E_Trans15_all_lower_lim_1n1p = 0;
        E_Trans45_all_upper_lim_1n1p = 1.7;
        E_Trans45_all_lower_lim_1n1p = 0;
        E_Trans90_all_upper_lim_1n1p = 1.7;
        E_Trans90_all_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only)">

        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
        E_Trans15_QEL_upper_lim_2p = 1.7;
        E_Trans15_QEL_lower_lim_2p = 0;
        E_Trans45_QEL_upper_lim_2p = 1.7;
        E_Trans45_QEL_lower_lim_2p = 0;
        E_Trans90_QEL_upper_lim_2p = 1.7;
        E_Trans90_QEL_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
        E_Trans15_QEL_upper_lim_1n1p = 1.7;
        E_Trans15_QEL_lower_lim_1n1p = 0;
        E_Trans45_QEL_upper_lim_1n1p = 1.7;
        E_Trans45_QEL_lower_lim_1n1p = 0;
        E_Trans90_QEL_upper_lim_1n1p = 1.7;
        E_Trans90_QEL_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only)">

        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
        E_Trans15_MEC_upper_lim_2p = 1.7;
        E_Trans15_MEC_lower_lim_2p = 0;
        E_Trans45_MEC_upper_lim_2p = 1.7;
        E_Trans45_MEC_lower_lim_2p = 0;
        E_Trans90_MEC_upper_lim_2p = 1.7;
        E_Trans90_MEC_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
        E_Trans15_MEC_upper_lim_1n1p = 1.7;
        E_Trans15_MEC_lower_lim_1n1p = 0;
        E_Trans45_MEC_upper_lim_1n1p = 1.7;
        E_Trans45_MEC_lower_lim_1n1p = 0;
        E_Trans90_MEC_upper_lim_1n1p = 1.7;
        E_Trans90_MEC_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only)">

        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
        E_Trans15_RES_upper_lim_2p = 1.7;
        E_Trans15_RES_lower_lim_2p = 0;
        E_Trans45_RES_upper_lim_2p = 1.7;
        E_Trans45_RES_lower_lim_2p = 0;
        E_Trans90_RES_upper_lim_2p = 1.7;
        E_Trans90_RES_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
        E_Trans15_RES_upper_lim_1n1p = 1.7;
        E_Trans15_RES_lower_lim_1n1p = 0;
        E_Trans45_RES_upper_lim_1n1p = 1.7;
        E_Trans45_RES_lower_lim_1n1p = 0;
        E_Trans90_RES_upper_lim_1n1p = 1.7;
        E_Trans90_RES_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only)">

        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
        E_Trans15_DIS_upper_lim_2p = 1.7;
        E_Trans15_DIS_lower_lim_2p = 0;
        E_Trans45_DIS_upper_lim_2p = 1.7;
        E_Trans45_DIS_lower_lim_2p = 0;
        E_Trans90_DIS_upper_lim_2p = 1.7;
        E_Trans90_DIS_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
        E_Trans15_DIS_upper_lim_1n1p = 1.7;
        E_Trans15_DIS_lower_lim_1n1p = 0;
        E_Trans45_DIS_upper_lim_1n1p = 1.7;
        E_Trans45_DIS_lower_lim_1n1p = 0;
        E_Trans90_DIS_upper_lim_1n1p = 1.7;
        E_Trans90_DIS_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Inclusive Energy Transfer histograms">
        E_Trans15_all_inclusive_upper_lim = 2.3;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 2.3;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 2.3;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 2.3;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 2.3;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 2.75;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 2.3;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 2.75;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 2.3;
        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms">

        //<editor-fold desc="E_cal range">
        E_cal_QEL_upper_lim_range = 2.35;
        E_cal_QEL_lower_lim_range = 2.11;
        E_cal_MEC_upper_lim_range = 2.35;
        E_cal_MEC_lower_lim_range = 2.11;
        E_cal_RES_upper_lim_range = 2.35;
        E_cal_RES_lower_lim_range = 2.11;
        E_cal_DIS_upper_lim_range = 2.35;
        E_cal_DIS_lower_lim_range = 2.11;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (2p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        if (BEnergyToNucleusCon == true) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range + 2 * BEnergyToNucleus;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range + 2 * BEnergyToNucleus;
        } else if (BEnergyToNucleusCon == false) {
            E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
            E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
            E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
            E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
            E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
            E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
            E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
            E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms">

        //<editor-fold desc="Momentum histograms (2p)">
        P_L_hist_upper_lim_2p = 2.3;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 2.3;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 2.3;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 2.3;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 2.3;
        P_n_hist_lower_lim_1n1p = 0;
        P_l_hist_upper_lim_1n1p = 2.3;
        P_l_hist_lower_lim_1n1p = 0;
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    }

    //</editor-fold>

    //</editor-fold>

// Saving settings to log file -------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving settings to log file">
    if (delete_txt_files == true) {
        system("find ./plots -type f -iname '*.txt' -delete"); // Delete existing .txt files
    }

    ofstream myLogFile;

    myLogFile.open("./plots/Run_log.txt");

    myLogFile << "//////////////////////////////////////////////////////////////////////////////////////////\n";
    myLogFile << "// Run was with '" << file_name << "' settings mode.\n";
    myLogFile << "// Input file was " << LoadedInput << ".\n";
    myLogFile << "// Code version was " << Ver << "\n";
    myLogFile << "//////////////////////////////////////////////////////////////////////////////////////////\n";
    myLogFile << "\n";
    myLogFile << "Code ran with the following settings:" << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Input:\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "LoadedInput: " << LoadedInput << "\n";
    myLogFile << "filePath: " << filePath << "\n";
    myLogFile << "fileInput: " << fileInput << "\n";
    myLogFile << "plotsInput: " << plotsInput << "\n";
    myLogFile << "\n";

    myLogFile << "FSI settings\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "FSI_status = " << BoolToString(FSI_status) << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Plot settings\n";
    myLogFile << "==========================================================================================\n";
//    myLogFile << "lowest_nentries = " << BoolToString(lowest_nentries) << "\n";
    myLogFile << "wider_margin = " << BoolToString(wider_margin) << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Calculation settings\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "calculate_2p = " << BoolToString(calculate_2p) << "\n";
    myLogFile << "calculate_1n1p = " << BoolToString(calculate_1n1p) << "\n";
    myLogFile << "calculate_MicroBooNE = " << BoolToString(calculate_MicroBooNE) << "\n";
    myLogFile << "\n";
    myLogFile << "BEnergyToNucleusCon = " << BoolToString(BEnergyToNucleusCon) << "\n";
    myLogFile << "BEnergyToNucleus = " << BEnergyToNucleus << "\n";
    myLogFile << "Probe = " << Probe << " (PDG: " << Probe_pdg << ")" << "\n";
    myLogFile << "Target = " << Target_nucleus << " (PDG: " << Target_pdg << ")" << "\n";
    myLogFile << "BeamEnergy = " << BeamEnergy << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Plot selector\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "Theta_plots = " << BoolToString(Theta_plots) << "\n";
    myLogFile << "Phi_plots = " << BoolToString(Phi_plots) << "\n";
    myLogFile << "Energy_histogram_plots = " << BoolToString(Energy_histogram_plots) << "\n";
    myLogFile << "ET_all_plots = " << BoolToString(ET_all_plots) << "\n";
    myLogFile << "ET_QEL_plots = " << BoolToString(ET_QEL_plots) << "\n";
    myLogFile << "ET_MEC_plots = " << BoolToString(ET_MEC_plots) << "\n";
    myLogFile << "ET_RES_plots = " << BoolToString(ET_RES_plots) << "\n";
    myLogFile << "ET_DIS_plots = " << BoolToString(ET_DIS_plots) << "\n";
    myLogFile << "inclusive_plots = " << BoolToString(inclusive_plots) << "\n";
    myLogFile << "E_cal_plots = " << BoolToString(E_cal_plots) << "\n";
    myLogFile << "other_E_cal_plots = " << BoolToString(other_E_cal_plots) << "\n";
    myLogFile << "momentum_plots = " << BoolToString(momentum_plots) << "\n";
    myLogFile << "MicroBooNE_plots = " << BoolToString(MicroBooNE_plots) << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Normalization settings\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "normalized_theta_l_plots = " << BoolToString(normalized_theta_l_plots) << "\n";
    myLogFile << "normalized_E_l_plots = " << BoolToString(normalized_E_l_plots) << "\n";
    myLogFile << "normalized_E_Trans15_plots = " << BoolToString(normalized_E_Trans15_plots) << "\n";
    myLogFile << "normalized_E_cal_plots = " << BoolToString(normalized_E_cal_plots) << "\n";
    myLogFile << "normalized_inclusive_plots = " << BoolToString(normalized_inclusive_plots) << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Delete settings\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "delete_png_files = " << BoolToString(delete_png_files) << "\n";
    myLogFile << "delete_root_files = " << BoolToString(delete_root_files) << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Momentum thresholds (2p)\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "P_l_upper_lim_2p = " << P_l_upper_lim_2p << "\n";
    myLogFile << "P_l_lower_lim_2p = " << P_l_lower_lim_2p << "\n";
    myLogFile << "P_p1_upper_lim_2p = " << P_p1_upper_lim_2p << "\n";
    myLogFile << "P_p1_lower_lim_2p = " << P_p1_lower_lim_2p << "\n";
    myLogFile << "P_p2_upper_lim_2p = " << P_p2_upper_lim_2p << "\n";
    myLogFile << "P_p2_lower_lim_2p = " << P_p2_lower_lim_2p << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Momentum thresholds (1n1p)\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "P_l_upper_lim_1n1p = " << P_l_upper_lim_1n1p << "\n";
    myLogFile << "P_l_lower_lim_1n1p = " << P_l_lower_lim_1n1p << "\n";
    myLogFile << "P_p_upper_lim_1n1p = " << P_p_upper_lim_1n1p << "\n";
    myLogFile << "P_p_lower_lim_1n1p = " << P_p_lower_lim_1n1p << "\n";
    myLogFile << "P_n_upper_lim_1n1p = " << P_n_upper_lim_1n1p << "\n";
    myLogFile << "P_n_lower_lim_1n1p = " << P_n_lower_lim_1n1p << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Momentum thresholds (2p, MicroBooNE)\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "P_l_upper_lim_MicroBooNE = " << P_l_upper_lim_MicroBooNE << "\n";
    myLogFile << "P_l_lower_lim_MicroBooNE = " << P_l_lower_lim_MicroBooNE << "\n";
    myLogFile << "P_L_upper_lim_MicroBooNE = " << P_L_upper_lim_MicroBooNE << "\n";
    myLogFile << "P_L_lower_lim_MicroBooNE = " << P_L_lower_lim_MicroBooNE << "\n";
    myLogFile << "P_R_upper_lim_MicroBooNE = " << P_R_upper_lim_MicroBooNE << "\n";
    myLogFile << "P_R_lower_lim_MicroBooNE = " << P_R_lower_lim_MicroBooNE << "\n";
    myLogFile << "P_pion_upper_lim_MicroBooNE = " << P_pion_upper_lim_MicroBooNE << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile.close();
    //</editor-fold>

    //</editor-fold>


// Histogram definitions:
// =======================================================================================================================================================================

    //<editor-fold desc="Histogram definitions">

    cout << "\n";
    cout << "Defining histograms...\n";
    cout << "\n";

// Theta histograms -----------------------------------------------------------------------------------

    //<editor-fold desc="Theta histograms">
    THStack *ThetaStack = new THStack("#theta_{l} stack (2p & 1n1p)", "#theta_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p);#theta_{l} [Deg];");

    TH1D *Theta_l_histogram = new TH1D("#theta_{l} det sim (2p)", ";#theta_{l} [Deg];", 100, theta_l_lower_lim_2p, theta_l_upper_lim_2p);
    TH1D *Theta_p1_histogram = new TH1D("#theta_{p1} det sim (2p)", ";#theta_{p1} [Deg];", 100, theta_p1_lower_lim_2p, theta_p1_upper_lim_2p);
    TH1D *Theta_p2_histogram = new TH1D("#theta_{p2} det sim (2p)", ";#theta_{p2} [Deg];", 100, theta_p2_lower_lim_2p, theta_p2_upper_lim_2p);
    TH1D *dtheta_2p = new TH1D("#gamma det sim (2p)", ";#gamma_{Lab} = #theta_{p1} - #theta_{p2} [Deg];", 100, dtheta_lower_lim_2p, dtheta_upper_lim_2p);

    TH1D *theta_l_1n1p = new TH1D("#theta_{l} det sim (1n1p)", ";#theta_{l} [Deg];", 100, theta_l_lower_lim_1n1p, theta_l_upper_lim_1n1p);
    TH1D *theta_p_1n1p = new TH1D("#theta_{p} det sim (1n1p)", ";#theta_{p} [Deg];", 100, theta_p_lower_lim_1n1p, theta_p_upper_lim_1n1p);
    TH1D *theta_n_1n1p = new TH1D("#theta_{n} det sim (1n1p)", ";#theta_{n} [Deg];", 100, theta_n_lower_lim_1n1p, theta_n_upper_lim_1n1p);
    TH1D *dtheta_1n1p = new TH1D("#gamma det sim (1n1p)", ";#gamma_{Lab} = #theta_{p} - #theta_{n} [Deg];", 100, dtheta_lower_lim_1n1p, dtheta_upper_lim_1n1p);
    //</editor-fold>

// Phi histograms -------------------------------------------------------------------------------------

    //<editor-fold desc="Phi histograms">
    THStack *PhiStack = new THStack("#phi_{l} stack det sim (2p & 1n1p)", "#phi_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p);#phi_{l} [Deg];");

    TH1D *phi_l_2p = new TH1D("#phi_{l} det sim (2p)", ";#phi_{l} [Deg];", 100, phi_l_lower_lim_2p, phi_l_upper_lim_2p);
    TH1D *phi_p1_2p = new TH1D("#phi_{p1} det sim (2p)", ";#phi_{p1} [Deg];", 100, phi_p1_lower_lim_2p, phi_p1_upper_lim_2p);
    TH1D *phi_p2_2p = new TH1D("#phi_{p2} det sim (2p)", ";#phi_{p2} [Deg];", 100, phi_p2_lower_lim_2p, phi_p2_upper_lim_2p);
    TH1D *dphi_2p = new TH1D("#Delta#phi det sim (2p)", ";#Delta#phi} [Deg];", 100, dphi_lower_lim_2p, dphi_upper_lim_2p);

    TH1D *phi_l_1n1p = new TH1D("#phi_{l} det sim (1n1p)", ";#phi_{l} [Deg];", 100, phi_l_lower_lim_1n1p, phi_l_upper_lim_1n1p);
    TH1D *phi_p_1n1p = new TH1D("#phi_{p} det sim (1n1p)", ";#phi_{p1} [Deg];", 100, phi_p_lower_lim_1n1p, phi_p_upper_lim_1n1p);
    TH1D *phi_n_1n1p = new TH1D("#phi_{n} det sim (1n1p)", ";#phi_{p2} [Deg];", 100, phi_n_lower_lim_1n1p, phi_n_upper_lim_1n1p);
    TH1D *dphi_1n1p = new TH1D("#Delta#phi det sim (1n1p)", ";#Delta#phi} [Deg];", 100, dphi_lower_lim_1n1p, dphi_upper_lim_1n1p);
    //</editor-fold>

// Energy histograms ----------------------------------------------------------------------------------

    //<editor-fold desc="Energy histograms">
    THStack *EnergyStack = new THStack("E_{l} stack det sim (2p & 1n1p)", "Final State E_{l} (All Interactions, 2p and 1n1p);E_{l} [GeV]");

    TH1D *fsEl_2p = new TH1D("Final State E_{l} det sim (2p)", ";E_{l} [GeV]", 100, fsEl_lower_lim_2p, fsEl_upper_lim_2p);
    TH1D *fsEl_QEL_2p = new TH1D("Final State E_{l} det sim (QEL Only, 2p)", ";E_{l} [GeV]", 100, fsEl_QEL_lower_lim_2p, fsEl_QEL_upper_lim_2p);
    TH1D *fsEl_MEC_2p = new TH1D("Final State E_{l} det sim (MEC Only, 2p)", ";E_{l} [GeV]", 100, fsEl_MEC_lower_lim_2p, fsEl_MEC_upper_lim_2p);
    TH1D *fsEl_RES_2p = new TH1D("Final State E_{l} det sim (RES Only, 2p)", ";E_{l} [GeV]", 100, fsEl_RES_lower_lim_2p, fsEl_RES_upper_lim_2p);
    TH1D *fsEl_DIS_2p = new TH1D("Final State E_{l} det sim (DIS Only, 2p)", ";E_{l} [GeV]", 100, fsEl_DIS_lower_lim_2p, fsEl_DIS_upper_lim_2p);
    TH2D *fsEl_VS_theta_l_all_int_2p = new TH2D("Stat det sim (All Interactions, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                                                200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_QEL_only_2p = new TH2D("Stat det sim (QEL only, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                                                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_MEC_only_2p = new TH2D("Stat det sim (MEC only, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                                                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);

    TH1D *fsEl_1n1p = new TH1D("Final State E_{l} det sim (1n1p)", ";E_{l} [GeV];", 100, fsEl_lower_lim_1n1p, fsEl_upper_lim_1n1p);
    TH1D *fsEl_QEL_1n1p = new TH1D("Final State E_{l} det sim (QEL Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_QEL_lower_lim_1n1p, fsEl_QEL_upper_lim_1n1p);
    TH1D *fsEl_MEC_1n1p = new TH1D("Final State E_{l} det sim (MEC Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_MEC_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p);
    TH1D *fsEl_RES_1n1p = new TH1D("Final State E_{l} det sim (RES Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_RES_lower_lim_1n1p, fsEl_RES_upper_lim_1n1p);
    TH1D *fsEl_DIS_1n1p = new TH1D("Final State E_{l} det sim (DIS Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_DIS_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p);
    TH2D *fsEl_VS_theta_l_all_int_1n1p = new TH2D("Stat det sim (All Interactions, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                  200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                                                  200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_QEL_only_1n1p = new TH2D("Stat det sim (QEL only, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                   200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                                                   200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_MEC_only_1n1p = new TH2D("Stat det sim (MEC only, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                   200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                                                   200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);

    //</editor-fold>

// Energy Transfer histograms (all interactions) ------------------------------------------------------

    //<editor-fold desc="Energy Transfer histograms (all interactions)">
    THStack *Energy_Transfer_all_int_15_Stack_2p = new
            THStack("ET around 15 degrees Stack (all interactions, 2p)",
                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (2p);E_{#nu}-E_{l} [GeV]");
    THStack *Energy_Transfer_all_int_15_Stack_1n1p = new
            THStack("ET around 15 deg Stack (all interactions, 1n1p)",
                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (1n1p);E_{#nu}-E_{l} [GeV]");


    TH1D *E_Trans_all_ang_all_int_2p = new
            TH1D("ET in all angles (all interactions, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) for every angle (All Interactions, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans_all_ang_all_int_lower_lim_2p, E_Trans_all_ang_all_int_upper_lim_2p);
    TH1D *E_Trans15_all_2p = new
            TH1D("ET around 15 degrees (all interactions, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_all_lower_lim_2p, E_Trans15_all_upper_lim_2p);
    TH1D *E_Trans45_all_2p = new
            TH1D("ET around 45 degrees (all interactions, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (All Interactions, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_all_lower_lim_2p, E_Trans45_all_upper_lim_2p);
    TH1D *E_Trans90_all_2p = new
            TH1D("ET around 90 degrees (all interactions, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (All Interactions, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_all_lower_lim_2p, E_Trans90_all_upper_lim_2p);

    TH1D *E_Trans_all_ang_all_int_1n1p = new
            TH1D("ET in all angles (all interactions, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in every angle (All Interactions, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans_all_ang_all_int_lower_lim_1n1p, E_Trans_all_ang_all_int_upper_lim_1n1p);
    TH1D *E_Trans15_all_1n1p = new
            TH1D("ET around 15 degrees (all interactions, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_all_lower_lim_1n1p, E_Trans15_all_upper_lim_1n1p);
    TH1D *E_Trans45_all_1n1p = new
            TH1D("ET around 45 degrees (all interactions, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (All Interactions, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_all_lower_lim_1n1p, E_Trans45_all_upper_lim_1n1p);
    TH1D *E_Trans90_all_1n1p = new
            TH1D("ET around 90 degrees (all interactions, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (All Interactions, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_all_lower_lim_1n1p, E_Trans90_all_upper_lim_1n1p);
    //</editor-fold>

// Energy Transfer histograms (QEL only) --------------------------------------------------------------

    //<editor-fold desc="Energy Transfer histograms (QEL only)">
    THStack *Energy_Transfer_QEL_Int_Stack_2p = new
            THStack("Energy_Transfer_QEL_Int_Stack_2p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (QEL only, 2p);E_{#nu}-E_{l} [GeV]");
    THStack *Energy_Transfer_QEL_Int_Stack_1n1p = new
            THStack("Energy_Transfer_QEL_Int_Stack_1n1p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (QEL only, 1n1p);E_{#nu}-E_{l} [GeV]");

    TH1D *E_Trans15_QEL_2p = new
            TH1D("ET around 15 degrees (QEL Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_QEL_lower_lim_2p, E_Trans15_QEL_upper_lim_2p);
    TH1D *E_Trans45_QEL_2p = new
            TH1D("ET around 45 degrees (QEL Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (QEL Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_QEL_lower_lim_2p, E_Trans45_QEL_upper_lim_2p);
    TH1D *E_Trans90_QEL_2p = new
            TH1D("ET around 90 degrees (QEL Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (QEL Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_QEL_lower_lim_2p, E_Trans90_QEL_upper_lim_2p);

    TH1D *E_Trans15_QEL_1n1p = new
            TH1D("ET around 15 degrees (QEL Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_QEL_lower_lim_1n1p, E_Trans15_QEL_upper_lim_1n1p);
    TH1D *E_Trans45_QEL_1n1p = new
            TH1D("ET around 45 degrees (QEL Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (QEL Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_QEL_lower_lim_1n1p, E_Trans45_QEL_upper_lim_1n1p);
    TH1D *E_Trans90_QEL_1n1p = new
            TH1D("ET around 90 degrees (QEL Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (QEL Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_QEL_lower_lim_1n1p, E_Trans90_QEL_upper_lim_1n1p);
    //</editor-fold>

// Energy Transfer histograms (MEC only) --------------------------------------------------------------

    //<editor-fold desc="Energy Transfer histograms (MEC only)">
    THStack *Energy_Transfer_MEC_Int_Stack_2p = new
            THStack("Energy_Transfer_MEC_Int_Stack_2p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (MEC only, 2p);E_{#nu}-E_{l} [GeV]");
    THStack *Energy_Transfer_MEC_Int_Stack_1n1p = new
            THStack("Energy_Transfer_MEC_Int_Stack_1n1p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (MEC only, 1n1p);E_{#nu}-E_{l} [GeV]");

    TH1D *E_Trans15_MEC_2p = new
            TH1D("ET around 15 degrees (MEC Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_MEC_lower_lim_2p, E_Trans15_MEC_upper_lim_2p);
    TH1D *E_Trans45_MEC_2p = new
            TH1D("ET around 45 degrees (MEC Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (MEC Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_MEC_lower_lim_2p, E_Trans45_MEC_upper_lim_2p);
    TH1D *E_Trans90_MEC_2p = new
            TH1D("ET around 90 degrees (MEC Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (MEC Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_MEC_lower_lim_2p, E_Trans90_MEC_upper_lim_2p);

    TH1D *E_Trans15_MEC_1n1p = new
            TH1D("ET around 15 degrees (MEC Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 [Deg] #leq #theta_{l} #leq 16 [Deg] (MEC Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_MEC_lower_lim_1n1p, E_Trans15_MEC_upper_lim_1n1p);
    TH1D *E_Trans45_MEC_1n1p = new
            TH1D("ET around 45 degrees (MEC Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 [Deg] #leq #theta_{l} #leq 46 [Deg] (MEC Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_MEC_lower_lim_1n1p, E_Trans45_MEC_upper_lim_1n1p);
    TH1D *E_Trans90_MEC_1n1p = new
            TH1D("ET around 90 degrees (MEC Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 [Deg] #leq #theta_{l} #leq 91 [Deg] (MEC Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_MEC_lower_lim_1n1p, E_Trans90_MEC_upper_lim_1n1p);
    //</editor-fold>

// Energy Transfer histograms (RES only) --------------------------------------------------------------

    //<editor-fold desc="Energy Transfer histograms (RES only)">
    THStack *Energy_Transfer_RES_Int_Stack_2p = new
            THStack("Energy_Transfer_RES_Int_Stack_2p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (RES only, 2p);E_{#nu}-E_{l} [GeV]");
    THStack *Energy_Transfer_RES_Int_Stack_1n1p = new
            THStack("Energy_Transfer_RES_Int_Stack_1n1p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (RES only, 1n1p);E_{#nu}-E_{l} [GeV]");

    TH1D *E_Trans15_RES_2p = new
            TH1D("ET around 15 degrees (RES Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_RES_lower_lim_2p, E_Trans15_RES_upper_lim_2p);
    TH1D *E_Trans45_RES_2p = new
            TH1D("ET around 45 degrees (RES Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (RES Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_RES_lower_lim_2p, E_Trans45_RES_upper_lim_2p);
    TH1D *E_Trans90_RES_2p = new
            TH1D("ET around 90 degrees (RES Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (RES Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_RES_lower_lim_2p, E_Trans90_RES_upper_lim_2p);

    TH1D *E_Trans15_RES_1n1p = new
            TH1D("ET around 15 degrees (RES Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_RES_lower_lim_1n1p, E_Trans15_RES_upper_lim_1n1p);
    TH1D *E_Trans45_RES_1n1p = new
            TH1D("ET around 45 degrees (RES Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (RES Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_RES_lower_lim_1n1p, E_Trans45_RES_upper_lim_1n1p);
    TH1D *E_Trans90_RES_1n1p = new
            TH1D("ET around 90 degrees (RES Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (RES Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_RES_lower_lim_1n1p, E_Trans90_RES_upper_lim_1n1p);
    //</editor-fold>

// Energy Transfer histograms (DIS interactions) ------------------------------------------------------

    //<editor-fold desc="Energy Transfer histograms (DIS interactions)">
    THStack *Energy_Transfer_DIS_Int_Stack_2p = new
            THStack("Energy_Transfer_DIS_Int_Stack_2p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (DIS only, 2p);E_{#nu}-E_{l} [GeV]");
    THStack *Energy_Transfer_DIS_Int_Stack_1n1p = new
            THStack("Energy_Transfer_DIS_Int_Stack_1n1p",
                    "Energy Transfer (E_{#nu}-E_{l}) for different #theta_{l} (DIS only, 1n1p);E_{#nu}-E_{l} [GeV]");

    TH1D *E_Trans15_DIS_2p = new
            TH1D("ET around 15 degrees (DIS Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_DIS_lower_lim_2p, E_Trans15_DIS_upper_lim_2p);
    TH1D *E_Trans45_DIS_2p = new
            TH1D("ET around 45 degrees (DIS Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (DIS Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_DIS_lower_lim_2p, E_Trans45_DIS_upper_lim_2p);
    TH1D *E_Trans90_DIS_2p = new
            TH1D("ET around 90 degrees (DIS Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (DIS Only, 2p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_DIS_lower_lim_2p, E_Trans90_DIS_upper_lim_2p);

    TH1D *E_Trans15_DIS_1n1p = new
            TH1D("ET around 15 degrees (DIS Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_DIS_lower_lim_1n1p, E_Trans15_DIS_upper_lim_1n1p);
    TH1D *E_Trans45_DIS_1n1p = new
            TH1D("ET around 45 degrees (DIS Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 44 #leq #theta_{l} #leq 46 (DIS Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans45_DIS_lower_lim_1n1p, E_Trans45_DIS_upper_lim_1n1p);
    TH1D *E_Trans90_DIS_1n1p = new
            TH1D("ET around 90 degrees (DIS Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 89 #leq #theta_{l} #leq 91 (DIS Only, 1n1p);E_{#nu}-E_{l} [GeV]",
                 100, E_Trans90_DIS_lower_lim_1n1p, E_Trans90_DIS_upper_lim_1n1p);
    //</editor-fold>

// Inclusive Energy Transfer histograms ---------------------------------------------------------------

    //<editor-fold desc="Inclusive Energy Transfer histograms">
    THStack *Energy_Transfer_all_int_15_inclusive_Stack = new
            THStack("Energy_Transfer_all_int_15_inclusive_Stack", ";E_{#nu}-E_{l} [GeV]");

    TH1D *E_Trans15_all_inclusive = new
            TH1D("ET around 15 degrees (all interactions, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_all_inclusive_lower_lim, E_Trans15_all_inclusive_upper_lim);
    TH1D *E_Trans15_QEL_inclusive = new
            TH1D("ET around 15 degrees (QEL Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_QEL_inclusive_lower_lim, E_Trans15_QEL_inclusive_upper_lim);
    TH1D *E_Trans15_MEC_inclusive = new
            TH1D("ET around 15 degrees (MEC Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_MEC_inclusive_lower_lim, E_Trans15_MEC_inclusive_upper_lim);
    TH1D *E_Trans15_RES_inclusive = new
            TH1D("ET around 15 degrees (RES Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_RES_inclusive_lower_lim, E_Trans15_RES_inclusive_upper_lim);
    TH1D *E_Trans15_DIS_inclusive = new
            TH1D("ET around 15 degrees (DIS Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                 100, E_Trans15_DIS_inclusive_lower_lim, E_Trans15_DIS_inclusive_upper_lim);

    TH1D *Q2_hist_inclusive = new
            TH1D("Q^{2} (inclusive)", ";Q^{2} [GeV^{2}]", 100, -0.05, 0.8); // CHANGE TITLE & ADD CUSTOM LIMITS

    TH2D *E_Trans_VS_q_all_inclusive = new
            TH2D("ET around #omega VS q_{3} (All Interactions)",
                 ";|q| = |p_{v} - p_{l}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q_all_inclusive_lower_lim_x, E_Trans_VS_q_all_inclusive_upper_lim_x,
                 200, E_Trans_VS_q_all_inclusive_lower_lim_y, E_Trans_VS_q_all_inclusive_upper_lim_y);
    TH2D *E_Trans_VS_q_QEL_inclusive = new
            TH2D("ET around #omega VS q_{3} (QEL Only)",
                 ";|q| = |p_{v} - p_{l}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q_QEL_inclusive_lower_lim_x, E_Trans_VS_q_QEL_inclusive_upper_lim_x,
                 200, E_Trans_VS_q_QEL_inclusive_lower_lim_y, E_Trans_VS_q_QEL_inclusive_upper_lim_y);
    TH2D *E_Trans_VS_q_MEC_inclusive = new
            TH2D("ET around #omega VS q_{3} (MEC Only)",
                 ";|q| = |p_{v} - p_{l}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q_MEC_inclusive_lower_lim_x, E_Trans_VS_q_MEC_inclusive_upper_lim_x,
                 200, E_Trans_VS_q_MEC_inclusive_lower_lim_y, E_Trans_VS_q_MEC_inclusive_upper_lim_y);

    TH2D *E_Trans_VS_q3_all_2p = new
            TH2D("ET around #omega VS q_{3} (All Interactions, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (All Interactions, 2p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q3_all_lower_lim_x_2p, E_Trans_VS_q3_all_upper_lim_x_2p,
                 200, E_Trans_VS_q3_all_lower_lim_y_2p, E_Trans_VS_q3_all_upper_lim_y_2p);
    TH2D *E_Trans_VS_q3_QEL_2p = new
            TH2D("ET around #omega VS q_{3} (QEL Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (QEL Only, 2p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q3_QEL_lower_lim_x_2p, E_Trans_VS_q3_QEL_upper_lim_x_2p,
                 200, E_Trans_VS_q3_QEL_lower_lim_y_2p, E_Trans_VS_q3_QEL_upper_lim_y_2p);
    TH2D *E_Trans_VS_q3_MEC_2p = new
            TH2D("ET around #omega VS q_{3} (MEC Only, 2p)",
                 "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (MEC Only, 2p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q3_MEC_lower_lim_x_2p, E_Trans_VS_q3_MEC_upper_lim_x_2p,
                 200, E_Trans_VS_q3_MEC_lower_lim_y_2p, E_Trans_VS_q3_MEC_upper_lim_y_2p);

    TH2D *E_Trans_VS_q3_all_1n1p = new
            TH2D("ET around #omega VS q_{3} (All Interactions, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (All Interactions, 1n1p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q3_all_lower_lim_x_1n1p, E_Trans_VS_q3_all_upper_lim_x_1n1p,
                 200, E_Trans_VS_q3_all_lower_lim_y_1n1p, E_Trans_VS_q3_all_upper_lim_y_1n1p);
    TH2D *E_Trans_VS_q3_QEL_1n1p = new
            TH2D("ET around #omega VS q_{3} (QEL Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (QEL Only, 1n1p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q3_QEL_lower_lim_x_1n1p, E_Trans_VS_q3_QEL_upper_lim_x_1n1p,
                 200, E_Trans_VS_q3_QEL_lower_lim_y_1n1p, E_Trans_VS_q3_QEL_upper_lim_y_1n1p);
    TH2D *E_Trans_VS_q3_MEC_1n1p = new
            TH2D("ET around #omega VS q_{3} (MEC Only, 1n1p)",
                 "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (MEC Only, 1n1p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                 200, E_Trans_VS_q3_MEC_lower_lim_x_1n1p, E_Trans_VS_q3_MEC_upper_lim_x_1n1p,
                 200, E_Trans_VS_q3_MEC_lower_lim_y_1n1p, E_Trans_VS_q3_MEC_upper_lim_y_1n1p);

    //<editor-fold desc="Inclusive Energy Transfer histograms - limits & titles by cases">
    if (Target_nucleus == "12C") {
        E_Trans15_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, ^{12}C(e,e'))");
        E_Trans15_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, ^{12}C(e,e'))");
        E_Trans15_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, ^{12}C(e,e'))");
        E_Trans15_RES_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, ^{12}C(e,e'))");
        E_Trans15_DIS_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, ^{12}C(e,e'))");
        Q2_hist_inclusive->SetTitle("Q^{2} Histogram ^{12}C(e,e')");
        E_Trans_VS_q_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (All Interactions, ^{12}C(e,e'))");
        E_Trans_VS_q_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (QEL Only, ^{12}C(e,e'))");
        E_Trans_VS_q_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (MEC Only, ^{12}C(e,e'))");
    } else if (Target_nucleus == "40Ar") {
        E_Trans15_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, ^{40}Ar(e,e'))");
        E_Trans15_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, ^{40}Ar(e,e'))");
        E_Trans15_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, ^{40}Ar(e,e'))");
        E_Trans15_RES_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, ^{40}Ar(e,e'))");
        E_Trans15_DIS_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, ^{40}Ar(e,e'))");
        Q2_hist_inclusive->SetTitle("Q^{2} Histogram ^{40}Ar(e,e')");
        E_Trans_VS_q_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (All Interactions, ^{40}Ar(e,e'))");
        E_Trans_VS_q_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (QEL Only, ^{40}Ar(e,e'))");
        E_Trans_VS_q_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (MEC Only, ^{40}Ar(e,e'))");
    } else if (Target_nucleus == "56Fe") {
        E_Trans15_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, ^{56}Fe(e,e'))");
        E_Trans15_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, ^{56}Fe(e,e'))");
        E_Trans15_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, ^{56}Fe(e,e'))");
        E_Trans15_RES_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, ^{56}Fe(e,e'))");
        E_Trans15_DIS_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, ^{56}Fe(e,e'))");
        Q2_hist_inclusive->SetTitle("Q^{2} Histogram ^{56}Fe(e,e')");
        E_Trans_VS_q_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (All Interactions, ^{56}Fe(e,e'))");
        E_Trans_VS_q_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (QEL Only, ^{56}Fe(e,e'))");
        E_Trans_VS_q_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (MEC Only, ^{56}Fe(e,e'))");
    }
    //</editor-fold>

    //</editor-fold>

// E_cal restoration histograms -----------------------------------------------------------------------

// Old plots:

    //todo: remove title and fix xlabel according to BEnergy for QEL case (confirm w/ Adi)
    //<editor-fold desc="E_cal restoration histograms - old plots">
    TH1D *E_cal_QEL_2p, *E_cal_MEC_2p, *E_cal_RES_2p, *E_cal_DIS_2p, *E_cal_QEL_1n1p, *E_cal_MEC_1n1p, *E_cal_RES_1n1p, *E_cal_DIS_1n1p;
    THStack *E_cal_QEL_Stack, *E_cal_MEC_Stack, *E_cal_RES_Stack, *E_cal_DIS_Stack;

    if (BEnergyToNucleusCon == true) {
        E_cal_QEL_Stack = new
                THStack("E_cal stack (QEL only)", "E_{cal} Histogram (QEL only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} + 2*E_{BE} [GeV]");
        E_cal_MEC_Stack = new
                THStack("E_cal stack (MEC only)", "E_{cal} Histogram (MEC only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} + 2*E_{BE} [GeV]");
        E_cal_RES_Stack = new
                THStack("E_cal stack (RES only)", "E_{cal} Histogram (RES only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} + 2*E_{BE} [GeV]");
        E_cal_DIS_Stack = new
                THStack("E_cal stack (DIS only)", "E_{cal} Histogram (DIS only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} + 2*E_{BE} [GeV]");
    } else if (BEnergyToNucleusCon == false) {
        E_cal_QEL_Stack = new
                THStack("E_cal stack (QEL only)", "E_{cal} Histogram (QEL only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} [GeV]");
        E_cal_MEC_Stack = new
                THStack("E_cal stack (MEC only)", "E_{cal} Histogram (MEC only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} [GeV]");
        E_cal_RES_Stack = new
                THStack("E_cal stack (RES only)", "E_{cal} Histogram (RES only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} [GeV]");
        E_cal_DIS_Stack = new
                THStack("E_cal stack (DIS only)", "E_{cal} Histogram (DIS only, 2p and 1n1p);E_{cal} = E_{l} + T_{nuc 1} + T_{nuc 2} [GeV]");
    }

    if (BEnergyToNucleusCon == true) {
        E_cal_QEL_2p = new
                TH1D("E_{cal} (QEL only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} + 2*E_{BE} [GeV]", 100, E_cal_QEL_lower_lim_2p, E_cal_QEL_upper_lim_2p);
        E_cal_MEC_2p = new
                TH1D("E_{cal} (MEC only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} + 2*E_{BE} [GeV]", 100, E_cal_MEC_lower_lim_2p, E_cal_MEC_upper_lim_2p);
        E_cal_RES_2p = new
                TH1D("E_{cal} (RES only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} + 2*E_{BE} [GeV]", 100, E_cal_RES_lower_lim_2p, E_cal_RES_upper_lim_2p);
        E_cal_DIS_2p = new
                TH1D("E_{cal} (DIS only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} + 2*E_{BE} [GeV]", 100, E_cal_DIS_lower_lim_2p, E_cal_DIS_upper_lim_2p);
    } else if (BEnergyToNucleusCon == false) {
        E_cal_QEL_2p = new
                TH1D("E_{cal} (QEL only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]", 100, E_cal_QEL_lower_lim_2p, E_cal_QEL_upper_lim_2p);
        E_cal_MEC_2p = new
                TH1D("E_{cal} (MEC only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]", 100, E_cal_MEC_lower_lim_2p, E_cal_MEC_upper_lim_2p);
        E_cal_RES_2p = new
                TH1D("E_{cal} (RES only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]", 100, E_cal_RES_lower_lim_2p, E_cal_RES_upper_lim_2p);
        E_cal_DIS_2p = new
                TH1D("E_{cal} (DIS only, 2p)", ";E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]", 100, E_cal_DIS_lower_lim_2p, E_cal_DIS_upper_lim_2p);
    }

    if (BEnergyToNucleusCon == true) {
        E_cal_QEL_1n1p = new
                TH1D("E_{cal} (QEL only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} + 2*E_{BE} [GeV]",
                     100, E_cal_QEL_lower_lim_1n1p, E_cal_QEL_upper_lim_1n1p);
        E_cal_MEC_1n1p = new
                TH1D("E_{cal} (MEC only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} + 2*E_{BE} [GeV]",
                     100, E_cal_MEC_lower_lim_1n1p, E_cal_MEC_upper_lim_1n1p);
        E_cal_RES_1n1p = new
                TH1D("E_{cal} (RES only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} + 2*E_{BE} [GeV]",
                     100, E_cal_RES_lower_lim_1n1p, E_cal_RES_upper_lim_1n1p);
        E_cal_DIS_1n1p = new
                TH1D("E_{cal} (DIS only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} + 2*E_{BE} [GeV]",
                     100, E_cal_DIS_lower_lim_1n1p, E_cal_DIS_upper_lim_1n1p);
    } else if (BEnergyToNucleusCon == false) {
        E_cal_QEL_1n1p = new
                TH1D("E_{cal} (QEL only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} [GeV]", 100, E_cal_QEL_lower_lim_1n1p, E_cal_QEL_upper_lim_1n1p);
        E_cal_MEC_1n1p = new
                TH1D("E_{cal} (MEC only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} [GeV]", 100, E_cal_MEC_lower_lim_1n1p, E_cal_MEC_upper_lim_1n1p);
        E_cal_RES_1n1p = new
                TH1D("E_{cal} (RES only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} [GeV]", 100, E_cal_RES_lower_lim_1n1p, E_cal_RES_upper_lim_1n1p);
        E_cal_DIS_1n1p = new
                TH1D("E_{cal} (DIS only, 1n1p)", ";E_{cal} = E_{l} + T_{p} + T_{n} [GeV]", 100, E_cal_DIS_lower_lim_1n1p, E_cal_DIS_upper_lim_1n1p);
    }
    //</editor-fold>

// E_cal VS other variables (all interactions, 2p):

    //<editor-fold desc="E_cal restoration histograms - E_cal VS other variables (all interactions, 2p)">
    TH2D *E_cal_VS_theta_l_all_int_2p = new
            TH2D("E_{cal} vs #theta_{l} (all interactions, 2p)",
                 "E_{cal} vs #theta_{l} (All Interactions, 2p);#theta_{l} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0.0, 120, 200, 2.12, 2.3);
    TH2D *E_cal_VS_Q2_all_int_2p = new
            TH2D("E_{cal} vs Q^{2} (all interactions, 2p)",
                 "E_{cal} vs Q^{2} (All Interactions, 2p);Q^{2} [GeV^{2}];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0, 0.2, 200, 2.12, 2.3);
    TH2D *E_cal_VS_theta_p1_all_int_2p = new
            TH2D("E_{cal} vs #theta_{p1} (2p)",
                 "E_{cal} vs #theta_{p1} (All Interactions, 2p);#theta_{p1} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -10, 190, 200, 2.12, 2.3);
    TH2D *E_cal_VS_theta_p2_all_int_2p = new
            TH2D("E_{cal} vs #theta_{p2} (all interactions, 2p)",
                 "E_{cal} vs #theta_{p2} (All Interactions, 2p);#theta_{p2} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -10, 190, 200, 2.12, 2.3);
    TH2D *E_cal_VS_dtheta_all_int_2p = new
            TH2D("E_{cal} vs #gamma (all interactions, 2p)",
                 "E_{cal} vs #gamma (All Interactions, 2p);#gamma} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -10, 180, 200, 2.12, 2.3);
    //</editor-fold>

// E_cal VS other variables (QEL only, 2p):

    //<editor-fold desc="E_cal restoration histograms - E_cal VS other variables (QEL only, 2p)">
    TH2D *E_cal_VS_theta_l_QEL_only_2p = new
            TH2D("E_{cal} vs #theta_{l} (QEL Only, 2p)",
                 "E_{cal} vs #theta_{l} (QEL Only, 2p);#theta_{l} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0.0, 180, 200, 2.12, 2.23);
    TH2D *E_cal_VS_Q2_QEL_only_2p = new
            TH2D("E_{cal} vs Q^{2} (QEL Only, 2p)",
                 "E_{cal} vs Q^{2} (QEL Only, 2p);Q^{2} [GeV^{2}];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 500, -0.1, 0.2, 500, 2.12, 2.23);
    TH2D *E_cal_VS_W_QEL_only_2p = new
            TH2D("E_{cal} vs W (QEL Only, 2p)",
                 "E_{cal} vs W (QEL Only, 2p);W} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0.25, 1.75, 200, 2.12, 2.23);
    TH2D *E_cal_VS_En_QEL_only_2p = new
            TH2D("E_{cal} vs En (QEL Only, 2p)",
                 "E_{cal} vs En (QEL Only, 2p);En} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0.918, 0.923, 200, 2.12, 2.23);
    TH2D *E_cal_VS_Pn_QEL_only_2p = new
            TH2D("E_{cal} vs Pn (QEL Only, 2p)",
                 "E_{cal} vs Pn (QEL Only, 2p);Pn} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -0.1, 0.28, 200, 2.12, 2.23);
    TH2D *E_cal_VS_Pn1_QEL_only_2p = new
            TH2D("E_{cal} vs P_{1} (QEL Only, 2p)",
                 "E_{cal} vs P_{1} (QEL Only, 2p);Pn1} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -0.1, 2.35, 200, 2.12, 2.23);
    TH2D *E_cal_VS_Pn2_QEL_only_2p = new
            TH2D("E_{cal} vs P_{2} (QEL Only, 2p)",
                 "E_{cal} vs P_{2} (QEL Only, 2p);Pn2} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -0.05, 1.15, 200, 2.12, 2.23);
    TH2D *E_cal_VS_theta_p1_QEL_only_2p = new
            TH2D("E_{cal} vs #theta_{p1} (QEL Only, 2p)",
                 "E_{cal} vs #theta_{p1} (QEL Only, 2p);#theta_{p1} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -10, 190, 200, 2.12, 2.23);
    TH2D *E_cal_VS_theta_p2_QEL_only_2p = new
            TH2D("E_{cal} vs #theta_{p2} (QEL Only, 2p)",
                 "E_{cal} vs #theta_{p2} (QEL Only, 2p);#theta_{p2} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -10, 190, 200, 2.12, 2.23);
    TH2D *E_cal_VS_dtheta_QEL_only_2p = new
            TH2D("E_{cal} vs #gamma (QEL Only, 2p)",
                 "E_{cal} vs #gamma (QEL Only, 2p);#gamma} [Deg];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -10, 180, 200, 2.12, 2.23);
    //</editor-fold>

// E_cal VS other variables (QEL only, 1n1p):

    //<editor-fold desc="E_cal restoration histograms - E_cal VS other variables (QEL only, 1n1p)">
    TH2D *E_cal_VS_theta_l_QEL_1n1p = new
            TH2D("E_{cal} vs #theta_{l} (QEL Only, 1n1p)",
                 "E_{cal} vs #theta_{l} (QEL Only, 1n1p);#theta_{l} [Deg];E_{cal} = E_{l} + T_{p} + T_{n} [GeV]",
                 200, 0.0, 180, 200, 2.12, 2.23);
    TH2D *E_cal_VS_Q2_QEL_only_1n1p = new
            TH2D("E_{cal} vs Q^{2} (QEL Only, 1n1p)",
                 "E_{cal} vs Q^{2} (QEL Only, 1n1p);Q^{2} [GeV^{2}];E_{cal} = E_{l} + T_{p} + T_{n} [GeV]",
                 500, -0.1, 0.2, 500, 2.12, 2.23);
    TH2D *E_cal_VS_W_QEL_only_1n1p = new
            TH2D("E_{cal} vs W (QEL Only, 1n1p)",
                 "E_{cal} vs W (QEL Only, 1n1p);W} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0.25, 1.75, 200, 2.12, 2.23);
    TH2D *E_cal_VS_En_QEL_only_1n1p = new
            TH2D("E_{cal} vs En (QEL Only, 1n1p)",
                 "E_{cal} vs En (QEL Only, 1n1p);En} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, 0.917, 0.9225, 200, 2.12, 2.23);
    TH2D *E_cal_VS_Pn_QEL_only_1n1p = new
            TH2D("E_{cal} vs Pn (QEL Only, 1n1p)",
                 "E_{cal} vs Pn (QEL Only, 1n1p);Pn} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -0.1, 0.28, 200, 2.12, 2.23);
    TH2D *E_cal_VS_P_n_QEL_only_1n1p = new
            TH2D("E_{cal} vs P_{n} (QEL Only, 1n1p)",
                 "E_{cal} vs P_{n} (QEL Only, 1n1p);Pn1} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -0.05, 2.35, 200, 2.12, 2.23);
    TH2D *E_cal_VS_P_p_QEL_only_1n1p = new
            TH2D("E_{cal} vs P_{p} (QEL Only, 1n1p)",
                 "E_{cal} vs P_{p} (QEL Only, 1n1p);Pn2} [GeV];E_{cal} = E_{l} + T_{p1} + T_{p2} [GeV]",
                 200, -0.05, 1.15, 200, 2.12, 2.23);
    TH2D *E_cal_VS_theta_p_QEL_only_1n1p = new
            TH2D("E_{cal} vs #theta_{p} (QEL Only, 1n1p)",
                 "E_{cal} vs #theta_{p} (QEL Only, 1n1p);#theta_{p} [Deg];E_{cal} = E_{l} + T_{p} + T_{n} [GeV]",
                 200, -10, 190, 200, 2.12, 2.23);
    TH2D *E_cal_VS_theta_n_QEL_only_1n1p = new
            TH2D("E_{cal} vs #theta_{n} (QEL Only, 1n1p)",
                 "E_{cal} vs #theta_{n} (QEL Only, 1n1p);#theta_{n} [Deg];E_{cal} = E_{l} + T_{p} + T_{n} [GeV]",
                 200, -10, 190, 200, 2.12, 2.23);
    TH2D *E_cal_VS_dtheta_QEL_only_1n1p = new
            TH2D("E_{cal} vs #gamma (QEL Only, 1n1p)",
                 "E_{cal} vs #gamma (QEL Only, 1n1p);#gamma} [Deg];E_{cal} = E_{l} + T_{p} + T_{n} [GeV]",
                 200, -10, 180, 200, 2.12, 2.23);
    //</editor-fold>

// Momentum histograms -----------------------------------------------------------------------------

    //<editor-fold desc="Momentum histograms">
    THStack *MomentumStack_2p = new
            THStack("Momentum Stack (2p)", "Momentum Histogram Stack (All Interactions, 2p);Momentum} [GeV]");
    THStack *MomentumStack_1n1p = new
            THStack("Momentum Stack (1n1p)", "Momentum Histogram Stack (All Interactions, 1n1p);Momentum} [GeV]");

    TH1D *P_L_hist_2p = new
            TH1D("P_{p1} (all interactions, 2p)", ";P_{L} [GeV/c]", 100, P_L_hist_lower_lim_2p, P_L_hist_upper_lim_2p);
    TH1D *P_R_hist_2p = new
            TH1D("P_{p2} (all interactions, 2p)", ";P_{R} [GeV/c]", 100, P_R_hist_lower_lim_2p, P_R_hist_upper_lim_2p);
    TH1D *P_lp_hist_2p = new
            TH1D("P_{l} (all interactions, 2p)", ";P_{l} [GeV/c]", 100, P_lp_hist_lower_lim_2p, P_lp_hist_upper_lim_2p);

    TH1D *P_p_hist_1n1p = new
            TH1D("P_{p} (all interactions, 1n1p)", ";P_{p} [GeV/c]", 100, P_p_hist_lower_lim_1n1p, P_p_hist_upper_lim_1n1p);
    TH1D *P_n_hist_1n1p = new
            TH1D("P_{n} (all interactions, 1n1p)", ";P_{n} [GeV/c]", 100, P_n_hist_lower_lim_1n1p, P_n_hist_upper_lim_1n1p);
    TH1D *P_l_hist_1n1p = new
            TH1D("P_{l} (all interactions, 1n1p)", ";P_{l} [GeV/c]", 100, P_l_hist_lower_lim_1n1p, P_l_hist_upper_lim_1n1p);
    //</editor-fold>

// MicroBooNE histogram reconstruction -------------------------------------------------------------

//todo: get rid of the stack in histPlotter1D() (make a default without it)
    THStack *gamma_Lab_Stack, *gamma_mu_p_tot_Stack, dP_T_Stack;
    THStack *gamma_Lab_weighted_Stack, *gamma_mu_p_tot_weighted_Stack, dP_T_Stack_weighted;

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE gamma plots (unweighted)">
    TH1D *gamma_Lab_hist = new
            TH1D("cos(#gamma_{Lab})", "cos(#gamma_{Lab}) Histogram;cos(#gamma_{Lab})", 8, gamma_Lab_hist_lower_lim_2p, gamma_Lab_hist_upper_lim_2p);
    TH1D *gamma_mu_p_tot = new
            TH1D("cos(#gamma_{#mu,p_{L}+p_{R}})", "cos(#gamma_{#mu,p_{L}+p_{R}}) Histogram;cos(#gamma_{#mu,p_{L}+p_{R}})",
                 8, gamma_mu_p_tot_lower_lim_2p, gamma_mu_p_tot_upper_lim_2p);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE gamma plots (Q4 weighted)">
    TH1D *gamma_Lab_hist_weighted = new
            TH1D("cos(#gamma_{Lab}) (Q^{4} weighted)", "cos(#gamma_{Lab}) Histogram (Q^{4} weighted);cos(#gamma_{Lab})",
                 8, gamma_Lab_hist_weighted_lower_lim_1n1p, gamma_Lab_hist_weighted_upper_lim_1n1p);

    TH1D *gamma_mu_p_tot_weighted = new
            TH1D("cos(#gamma_{#mu,p_{L}+p_{R}}) (Q^{4} weighted)",
                 "cos(#gamma_{#mu,p_{L}+p_{R}}) Histogram (Q^{4} weighted);cos(#gamma_{#mu,p_{L}+p_{R}})",
                 8, gamma_mu_p_tot_weighted_lower_lim_1n1p, gamma_mu_p_tot_weighted_upper_lim_1n1p);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE gamma plots (no pions, for every interaction)">
    TH1D *gamma_Lab_all_hist = new
            TH1D("cos(#gamma_{Lab}) -all interactions",
                 "cos(#gamma_{Lab}) Histogram -all interactions;cos(#gamma_{Lab})",
                 8, gamma_Lab_all_hist_lower_lim, gamma_Lab_all_hist_upper_lim);
    TH1D *gamma_Lab_all_hist_weighted = new
            TH1D("cos(#gamma_{Lab}) -all interactions (Q^{4} weighted)",
                 "cos(#gamma_{Lab}) Histogram -all interactions (Q^{4} weighted);cos(#gamma_{Lab})",
                 8, gamma_Lab_all_hist_weighted_lower_lim, gamma_Lab_all_hist_weighted_upper_lim);

    TH1D *gamma_Lab_QEL_hist = new
            TH1D("cos(#gamma_{Lab}) - QEL Only", "cos(#gamma_{Lab}) Histogram - QEL Only;cos(#gamma_{Lab})",
                 8, gamma_Lab_QEL_hist_lower_lim, gamma_Lab_QEL_hist_upper_lim);
    TH1D *gamma_Lab_QEL_hist_weighted = new
            TH1D("cos(#gamma_{Lab}) - QEL Only (Q^{4} weighted)",
                 "cos(#gamma_{Lab}) Histogram - QEL Only (Q^{4} weighted);cos(#gamma_{Lab})",
                 8, gamma_Lab_QEL_hist_weighted_lower_lim, gamma_Lab_QEL_hist_weighted_upper_lim);

    TH1D *gamma_Lab_MEC_hist = new
            TH1D("cos(#gamma_{Lab}) - MEC Only", "cos(#gamma_{Lab}) Histogram - MEC Only;cos(#gamma_{Lab})",
                 8, gamma_Lab_MEC_hist_lower_lim, gamma_Lab_MEC_hist_upper_lim);
    TH1D *gamma_Lab_MEC_hist_weighted = new
            TH1D("cos(#gamma_{Lab}) - MEC Only (Q^{4} weighted)",
                 "cos(#gamma_{Lab}) Histogram - MEC Only (Q^{4} weighted);cos(#gamma_{Lab})",
                 8, gamma_Lab_MEC_hist_weighted_lower_lim, gamma_Lab_MEC_hist_weighted_upper_lim);

    TH1D *gamma_Lab_RES_hist = new
            TH1D("cos(#gamma_{Lab}) - RES Only", "cos(#gamma_{Lab}) Histogram - RES Only;cos(#gamma_{Lab})",
                 8, gamma_Lab_RES_hist_lower_lim, gamma_Lab_RES_hist_upper_lim);
    TH1D *gamma_Lab_RES_hist_weighted = new
            TH1D("cos(#gamma_{Lab}) - RES Only (Q^{4} weighted)",
                 "cos(#gamma_{Lab}) Histogram - RES Only (Q^{4} weighted);cos(#gamma_{Lab})",
                 8, gamma_Lab_RES_hist_weighted_lower_lim, gamma_Lab_RES_hist_weighted_upper_lim);

    TH1D *gamma_Lab_DIS_hist = new
            TH1D("cos(#gamma_{Lab}) - DIS Only", "cos(#gamma_{Lab}) Histogram - DIS Only;cos(#gamma_{Lab})",
                 8, gamma_Lab_DIS_hist_lower_lim, gamma_Lab_DIS_hist_upper_lim);
    TH1D *gamma_Lab_DIS_hist_weighted = new
            TH1D("cos(#gamma_{Lab}) - DIS Only (Q^{4} weighted)",
                 "cos(#gamma_{Lab}) Histogram - DIS Only (Q^{4} weighted);cos(#gamma_{Lab})",
                 8, gamma_Lab_DIS_hist_weighted_lower_lim, gamma_Lab_DIS_hist_weighted_upper_lim);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE dP_T plots (unweighted and Q4 weighted)">
    TH1D *dP_T_hist = new
            TH1D("#deltaP_{T}", ";#deltaP_{T} = |#bf{P}_{l,T} + #bf{P}_{L,T} + #bf{P}_{R,T}| [GeV/c]",
                 100, dP_T_hist_lower_lim, dP_T_hist_upper_lim);
    TH1D *dP_T_hist_weighted = new
            TH1D("#deltaP_{T} (Q^{4} weighted)", ";#deltaP_{T} = |#bf{P}_{l,T} + #bf{P}_{L,T} + #bf{P}_{R,T}| [GeV/c]",
                 100, dP_T_hist_weighted_lower_lim, dP_T_hist_weighted_upper_lim);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
    THStack *MomentumStack_MicroBooNE = new
            THStack("Momentum Stack (MicroBooNE)", ";Momentum [GeV]");

    TH1D *P_L_hist = new
            TH1D("P_{L}", "Momentum of Leading Proton (P_{L});P_{L} [GeV/c]", 100, P_L_hist_lower_lim, P_L_hist_upper_lim);
    TH1D *P_R_hist = new
            TH1D("P_{R}", "Momentum of Recoil Proton (P_{R});P_{R} [GeV/c]", 100, P_R_hist_lower_lim, P_R_hist_upper_lim);
    TH1D *P_l_hist = new
            TH1D("P_{l}", "Momentum of Lepton (P_{l});P_{l} [GeV/c]", 100, P_l_hist_lower_lim, P_l_hist_upper_lim);
    TH1D *P_pion_hist = new
            TH1D("P_{#pi^{#pm}}", "Momentum of Pions (P_{#pi^{#pm}});P_{#pi^{#pm}} [GeV/c]", 100, P_pion_hist_lower_lim, P_pion_hist_upper_lim);

    string dP_T_title = "#deltaP_{T} Histogram (" + file_name + ")";
    const char *dP_T_Title = dP_T_title.c_str();
    dP_T_hist->SetTitle(dP_T_Title);

    string dP_T_weighted_title = "#deltaP_{T} Histogram (" + file_name + ", Q^{4} weighted)";
    const char *dP_T_weighted_Title = dP_T_weighted_title.c_str();
    dP_T_hist_weighted->SetTitle(dP_T_weighted_Title);
    //</editor-fold>

// List definition ---------------------------------------------------------------------------------

    TList *plots = new
            TList();

    //</editor-fold>


// Code execution:
// =======================================================================================================================================================================

    //<editor-fold desc="Code execution">

    clas12reader c12(LoadedInput.c_str()); // open file

// 2p+1e - 85 out of 970000
// 2p - 0 out of 970000
// 1n1p+1e - 284 out of 970000
// 1n1p - 0 out of 970000

//        c12.addExactPid(2212, NumberOfProtons); //exactly 2 protons
//        c12.addExactPid(2212, 1); //exactly 1 electron
    c12.addExactPid(11, 1); // exactly 1 electron (outgoing lepton)
    c12.addAtLeastPid(2212, 1); // at least 1 proton (1 for 1n1p, 2 for 2p)
    c12.addAtLeastPid(2112, 0); // at least 1 neutron (1 for 1n1p, 0 for 2p)

    c12.addZeroOfRestPid(); // nothing else

    int num_of_2p_events = 0;
    int num_of_1n1p_events = 0;

    while (c12.next()) { //loop over events (2p)

        auto particles = c12.getDetParticles(); //particles are now a std::vector of particles for this event

        auto electrons = c12.getByID(11);
        auto protons = c12.getByID(2212);
        auto neutrons = c12.getByID(2112);

// 2p -------------------------------------------------------------------------------------------------------------------------------------------------------------------

        if (calculate_2p && protons.size() == 2 && neutrons.size() == 0) {
            ++num_of_2p_events;

            if (selection_test_2p) {
                cout << "==========================================================================\n";
                cout << "num_of_2p_events = " << num_of_2p_events << "\n";
                cout << "\n";
                cout << "\n";
                cout << "#electrons = " << electrons.size() << ", #protons = " << protons.size() << ", #neutrons = " << neutrons.size() << "\n";
                cout << "\n";
                cout << "\n";
            }

            int ProtonCounter_2p = 0, OtherParticleCounter_2p = 0;
            int Lepton_ind_2p = -1, Proton_1_ind_2p = -1, Proton_2_ind_2p = -1;

            for (int i = 0; i < particles.size(); i++) {

                float particlePDG_2p = particles[i]->par()->getPid();
//                float Beta = particles[i]->par()->getBeta();
//                float P = particles[i]->par()->getP();
//
//                Beta_VS_P_2p->Fill(P, Beta);
//                P_histogram_2p->Fill(P);

                if (selection_test_2p) {
                    cout << "particlePDG_2p[" << i << "] = " << particlePDG_2p << "\n";
                }

                //<editor-fold desc="Proton selector (2p)">
                if (particlePDG_2p == 2212) {
                    ++ProtonCounter_2p;
                    //                    cout << "particlePDG_2p[" << i << "] = " << particlePDG_2p << "\n";
                    //                    cout << "i = " << i << "\n";
                    if (ProtonCounter_2p == 1) {
                        Proton_1_ind_2p = i;
//                            cout << "Proton_1_ind_2p = " << Proton_1_ind_2p << "\n";
                    } else if (ProtonCounter_2p == 2) {
                        Proton_2_ind_2p = i;
//                            cout << "Proton_2_ind_2p = " << Proton_2_ind_2p << "\n";
                    } else if (ProtonCounter_2p > 2) {
                        cout << "\n";
                        cout << "Additional Protons detected (2p). PDG = " << particlePDG_2p << "\n";
                        cout << "\n";
                        cout << "\n";
                    }
                } else if (particlePDG_2p == 11) {
                    Lepton_ind_2p = i;
                    //                    cout << "Lepton_ind_2p = " << Lepton_ind_2p << "\n";
//                        cout << "particlePDG_2p[" << i << "] = " << particlePDG_2p << "\n";
                } else if (particlePDG_2p != 2212) {
                    ++OtherParticleCounter_2p;
                    if (OtherParticleCounter_2p > 0) {
                        cout << "\n";
                        cout << "Additional particles detected (2p). PDG = " << particlePDG_2p << "\n";
                        cout << "\n";
                        cout << "\n";
                    }
                }
                //</editor-fold>



//              Momentum of first proton in Ef[]:
                double P_p1_2p = particles[Proton_1_ind_2p]->getP();

//              Momentum of second proton in Ef[]:
                double P_p2_2p = particles[Proton_2_ind_2p]->getP();

//              Momentum of second proton in Ef[]:
                double P_lp_2p = particles[Lepton_ind_2p]->getP();

//              Leading proton:
                double P_L_2p = -1;

//              Recoil proton:
                double P_R_2p = -1;

//              Momentum cut to at least 300 [MeV/c] == 0.3 [GeV/c]:
                if (P_p1_2p >= 0 && P_p2_2p >= 0) {
//                if (P_p1_2p >= P_p1_lower_lim_2p && P_p2_2p >= P_p2_lower_lim_2p) {


//                double E_cal_2p;

//                if (BEnergyToNucleusCon == true) {
//                    E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272) + 2 * BEnergyToNucleus;
//                } else if (BEnergyToNucleusCon == false) {
//                    E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272);
//                }

                    double theta_l_2p = particles[Lepton_ind_2p]->getTheta();

                    Theta_l_histogram->Fill(theta_l_2p);

//                  NOT REALLY dtheta:
                    double d_theta_2p = acos(
                            (particles[Proton_1_ind_2p]->par()->getPx() * particles[Proton_2_ind_2p]->par()->getPx() +
                             particles[Proton_1_ind_2p]->par()->getPx() * particles[Proton_2_ind_2p]->par()->getPx() +
                             particles[Proton_1_ind_2p]->par()->getPx() * particles[Proton_2_ind_2p]->par()->getPx()) /
                            (particles[Proton_1_ind_2p]->getP() * particles[Proton_2_ind_2p]->getP()));
                    dtheta_2p->Fill(d_theta_2p * 180.0 / 3.14159265359);

                    //<editor-fold desc="P_L & P_R selector">
                    if (P_p1_2p >= P_p2_2p) { // If Proton_1_ind_2p is the leading proton and Proton_2_ind_2p is the recoil

//                      Leading proton:
                        double P_L_2p = P_p1_2p;

//                      Recoil proton:
                        double P_R_2p = P_p2_2p;

                        P_L_hist_2p->Fill(P_L_2p);
                        P_R_hist_2p->Fill(P_R_2p);

                        double phi_p1 = particles[Proton_1_ind_2p]->getPhi(); // Leading proton azimuthal angle in radians
                        double phi_p2 = particles[Proton_2_ind_2p]->getPhi(); // Leading proton azimuthal angle in radians
                        double d_phi_p2 = phi_p1 - phi_p2; // In radians

                        double theta_p1 = particles[Proton_1_ind_2p]->getTheta(); // Leading proton scattering angle in radians
                        double theta_p2 = particles[Proton_2_ind_2p]->getTheta(); // Recoil proton scattering angle in radians

                        phi_p1_2p->Fill(phi_p1 * 180.0 / 3.14159265359);
                        phi_p2_2p->Fill(phi_p2 * 180.0 / 3.14159265359);
                        dphi_2p->Fill(d_phi_p2 * 180.0 / 3.14159265359);

                        Theta_p1_histogram->Fill(theta_p1 * 180.0 / 3.14159265359);
                        Theta_p2_histogram->Fill(theta_p2 * 180.0 / 3.14159265359);
//
//                    if (qel == true) {
//                        E_cal_VS_theta_p1_QEL_only_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                        E_cal_VS_theta_p2_QEL_only_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                    }
                    } else { // If Proton_2_ind_2p is the leading proton and Proton_1_ind_2p is the recoil

//                      Leading proton:
                        double P_L_2p = P_p2_2p;

//                      Recoil proton:
                        double P_R_2p = P_p1_2p;

                        P_L_hist_2p->Fill(P_L_2p);
                        P_R_hist_2p->Fill(P_R_2p);

                        double phi_p2 = particles[Proton_1_ind_2p]->getPhi(); // Leading proton azimuthal angle in radians
                        double phi_p1 = particles[Proton_2_ind_2p]->getPhi(); // Leading proton azimuthal angle in radians
                        double d_phi_p2 = phi_p1 - phi_p2; // In radians

                        double theta_p2 = particles[Proton_1_ind_2p]->getTheta(); // Leading proton scattering angle in radians
                        double theta_p1 = particles[Proton_2_ind_2p]->getTheta(); // Recoil proton scattering angle in radians

                        phi_p2_2p->Fill(phi_p1 * 180.0 / 3.14159265359);
                        phi_p1_2p->Fill(phi_p2 * 180.0 / 3.14159265359);
                        dphi_2p->Fill(d_phi_p2 * 180.0 / 3.14159265359);

                        Theta_p2_histogram->Fill(theta_p1 * 180.0 / 3.14159265359);
                        Theta_p1_histogram->Fill(theta_p2 * 180.0 / 3.14159265359);
//
//                    if (qel == true) {
//                        E_cal_VS_theta_p2_QEL_only_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                        E_cal_VS_theta_p1_QEL_only_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                    }
                    }
                    //</editor-fold>

//                E_Trans_VS_q3_all_2p->Fill(q3, Ev - El);

//                P_lp_hist_2p->Fill(P_lp_2p);

//                fsEl_2p->Fill(El);
//                Theta_l_histogram->Fill(Theta_l_histogram);
//                phi_l_2p->Fill(atan2(pyl, pxl) * 180.0 / 3.14159265359);
//                fsEl_VS_theta_l_all_int_2p->Fill(Theta_l_histogram, El);

//                E_Trans_all_ang_all_int_2p->Fill(Ev - El);

//                E_cal_VS_theta_l_all_int_2p->Fill(Theta_l_histogram, E_cal_2p);
//                E_cal_VS_Q2_all_int_2p->Fill(Q2, E_cal_2p);
//                E_cal_VS_dtheta_all_int_2p->Fill(fabs(acos(pzf[Proton_1_ind_2p] / rCalc(pxf[Proton_1_ind_2p], pyf[Proton_1_ind_2p], pzf[Proton_1_ind_2p])) -
//                                                      acos(pzf[Proton_2_ind_2p] / rCalc(pxf[Proton_2_ind_2p], pyf[Proton_2_ind_2p], pzf[Proton_2_ind_2p]))) *
//                                                 180.0 / 3.14159265359, E_cal_2p);

//                gamma_Lab_all_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_all_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);

//                if (Theta_l_histogram >= 14.0 && Theta_l_histogram <= 16.0) {
//                    E_Trans15_all_2p->Fill(Ev - El);
//                } else if (Theta_l_histogram >= 44.0 && Theta_l_histogram <= 46.0) {
//                    E_Trans45_all_2p->Fill(Ev - El);
//                } else if (Theta_l_histogram >= 89.0 && Theta_l_histogram <= 91.0) {
//                    E_Trans90_all_2p->Fill(Ev - El);
//                }

//                //<editor-fold desc="Histogram fill by reaction (2p)">
//                if (qel == true) {
//                    gamma_Lab_QEL_hist->Fill(cos(d_theta_2p));
//                    gamma_Lab_QEL_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                    if (Theta_l_histogram >= 14.0 && Theta_l_histogram <= 16.0) {
//                        E_Trans15_QEL_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 44.0 && Theta_l_histogram <= 46.0) {
//                        E_Trans45_QEL_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 89.0 && Theta_l_histogram <= 91.0) {
//                        E_Trans90_QEL_2p->Fill(Ev - El);
//                    }
//
//                    fsEl_VS_theta_l_QEL_only_2p->Fill(Theta_l_histogram, El);
//
//                    E_cal_QEL_2p->Fill(E_cal_2p);
//                    E_cal_VS_theta_l_QEL_only_2p->Fill(Theta_l_histogram, E_cal_2p);
//                    E_cal_VS_Q2_QEL_only_2p->Fill(Q2, E_cal_2p);
//                    E_cal_VS_W_QEL_only_2p->Fill(W, E_cal_2p);
//                    E_cal_VS_En_QEL_only_2p->Fill(En, E_cal_2p);
//                    E_cal_VS_Pn_QEL_only_2p->Fill(sqrt(pxn * pxn + pyn * pyn + pzn * pzn), E_cal_2p);
//                    E_cal_VS_Pn1_QEL_only_2p->Fill(P_L_2p, E_cal_2p);
//                    E_cal_VS_Pn2_QEL_only_2p->Fill(P_R_2p, E_cal_2p);
//                    E_cal_VS_dtheta_QEL_only_2p->Fill(
//                            fabs(acos(pzf[Proton_1_ind_2p] / sqrt(pxf[Proton_1_ind_2p] * pxf[Proton_1_ind_2p] + pyf[Proton_1_ind_2p] * pyf[Proton_1_ind_2p] +
//                                                                  pzf[Proton_1_ind_2p] * pzf[Proton_1_ind_2p])) -
//                                 acos(pzf[Proton_2_ind_2p] / sqrt(pxf[Proton_2_ind_2p] * pxf[Proton_2_ind_2p] +
//                                                                  pyf[Proton_2_ind_2p] * pyf[Proton_2_ind_2p] + pzf[Proton_2_ind_2p] * pzf[Proton_2_ind_2p]))) *
//                            180.0 /
//                            3.14159265359, El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272));
//
//                    E_Trans_VS_q3_QEL_2p->Fill(q3, Ev - El);
//
//                    fsEl_QEL_2p->Fill(El);
//                } else if (mec == true) {
//                    gamma_Lab_MEC_hist->Fill(cos(d_theta_2p));
//                    gamma_Lab_MEC_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                    if (Theta_l_histogram >= 14.0 && Theta_l_histogram <= 16.0) {
//                        E_Trans15_MEC_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 44.0 && Theta_l_histogram <= 46.0) {
//                        E_Trans45_MEC_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 89.0 && Theta_l_histogram <= 91.0) {
//                        E_Trans90_MEC_2p->Fill(Ev - El);
//                    }
//
//                    fsEl_VS_theta_l_MEC_only_2p->Fill(Theta_l_histogram, El);
//
//                    E_cal_MEC_2p->Fill(E_cal_2p);
//
//                    E_Trans_VS_q3_MEC_2p->Fill(q3, Ev - El);
//
//                    fsEl_MEC_2p->Fill(El);
//                } else if (res == true) {
//                    gamma_Lab_RES_hist->Fill(cos(d_theta_2p));
//                    gamma_Lab_RES_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                    if (Theta_l_histogram >= 14.0 && Theta_l_histogram <= 16.0) {
//                        E_Trans15_RES_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 44.0 && Theta_l_histogram <= 46.0) {
//                        E_Trans45_RES_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 89.0 && Theta_l_histogram <= 91.0) {
//                        E_Trans90_RES_2p->Fill(Ev - El);
//                    }
//
//                    E_cal_RES_2p->Fill(E_cal_2p);
//
//                    fsEl_RES_2p->Fill(El);
//                } else if (dis == true) {
//                    gamma_Lab_DIS_hist->Fill(cos(d_theta_2p));
//                    gamma_Lab_DIS_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                    if (Theta_l_histogram >= 14.0 && Theta_l_histogram <= 16.0) {
//                        E_Trans15_DIS_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 44.0 && Theta_l_histogram <= 46.0) {
//                        E_Trans45_DIS_2p->Fill(Ev - El);
//                    } else if (Theta_l_histogram >= 89.0 && Theta_l_histogram <= 91.0) {
//                        E_Trans90_DIS_2p->Fill(Ev - El);
//                    }
//
//                    E_cal_DIS_2p->Fill(E_cal_2p);
//
//                    fsEl_DIS_2p->Fill(El);
//                }
//                //</editor-fold>

//            }
                } // end of momentum cut if

            } // end of loop over particles

        } // end of 2p if

// 1n1p -----------------------------------------------------------------------------------------------------------------------------------------------------------------

        if (calculate_1n1p && protons.size() == 1 && neutrons.size() == 1) {
            ++num_of_1n1p_events;

            if (selection_test_1n1p) {
                cout << "==========================================================================\n";
                cout << "num_of_1n1p_events = " << num_of_1n1p_events << "\n";
                cout << "\n";
                cout << "#electrons = " << electrons.size() << ", #protons = " << protons.size() << ", #neutrons = " << neutrons.size() << "\n";
                cout << "\n";
            }

            int ProtonCounter_1n1p = 0, NeutronCounter_1n1p = 0, OtherParticleCounter_1n1p = 0;
            int Lepton_ind_1n1p = -1, Proton_ind_1n1p = -1, Neutron_ind_1n1p = -1;

            for (int i = 0; i < particles.size(); i++) {

                float particlePDG_1n1p = particles[i]->par()->getPid();
//                float Beta = particles[i]->par()->getBeta();
//                float P = particles[i]->getP();
//
//                Beta_VS_P_2p->Fill(P, Beta);
//                P_histogram_2p->Fill(P);

                if (selection_test_1n1p) {
                    cout << "particlePDG_1n1p[" << i << "] = " << particlePDG_1n1p << "\n";
                }

                //<editor-fold desc="Proton & Neutron selector">
                if (particlePDG_1n1p == 2212) {
                    ++ProtonCounter_1n1p;
                    Proton_ind_1n1p = i;
                } else if (particlePDG_1n1p == 2112) {
                    ++NeutronCounter_1n1p;
                    Neutron_ind_1n1p = i;
                } else if (particlePDG_1n1p == 11) {
                    Lepton_ind_1n1p = i;
//                    cout << "Lepton_ind_1n1p = " << Lepton_ind_1n1p << "\n";
//                    cout << "particlePDG_1n1p[" << i << "] = " << particlePDG_1n1p << "\n";
                } else if ((particlePDG_1n1p != 2212) && (particlePDG_1n1p != 2112) && (particlePDG_1n1p != 11)) {
                    ++OtherParticleCounter_1n1p;
                    if (OtherParticleCounter_1n1p > 0) {
                        cout << "\n";
                        cout << "Additional particles detected (1n1p). PDG = " << particlePDG_1n1p << "\n";
                        cout << "\n";
                        cout << "\n";
                    }
                }
                //</editor-fold>

            }
        }


    }

    //      Normalization factor:
//        double theta_l_integral = Theta_l_histogram->Integral() + theta_l_1n1p->Integral();
//
//        //<editor-fold desc="Theta of outgoing lepton histogram (2p)">
//        histPlotter1D(c1, Theta_l_histogram, normalized_theta_l_plots, true, theta_l_integral, "#theta_{l} of Outgoing Lepton", "All Interactions",
//                      0.06, 0.0425, 0.0425, plots, 2, true, true, ThetaStack, "Theta_of_lepton", "plots/theta_histograms/", "2p", kBlue, true, true, true);
//        //</editor-fold>


//        Beta_VS_P_2p->SetStats(0);
//        Beta_VS_P_2p->Draw("colz");
//        cout << "\n";
//        c1->SaveAs("./plots/Beta_VS_P_2p.png");
//        c1->Clear();

//        P_histogram_2p->Draw("colz");
//        cout << "\n";
//        c1->SaveAs("./plots/P_histogram_2p.png");
//        c1->Clear();



// ======================================================================================================================================================================
// ======================================================================================================================================================================
// ======================================================================================================================================================================
// ======================================================================================================================================================================
// ======================================================================================================================================================================

/*
    clas12reader c12(LoadedInput.c_str()); //open file

    c12.addExactPid(2212, NumberOfProtons);    //exactly 1 proton
    c12.addExactPid(2112, NumberOfNeutrons);    //exactly 1 Neutron
    c12.addExactPid(11, 1);    //exactly 1 electron
    c12.addZeroOfRestPid();  //nothing else

//        auto *Beta_VS_P_1n1p = new TH2F("Beta VS P (1n1p)", "#beta VS P (1n1p); P [GeV]; #beta [Arbitrary units]", 250, 0, 6, 250, 0, 1.05);
//        auto *P_histogram_1n1p = new TH1F("P Histogram (1n1p)", "P Histogram (1n1p)", 100, 0, 6);

    while (c12.next()) { //loop over events (1np)

        auto particles_1n1p = c12.getDetParticles(); //particles is now a std::vector of particles for this event

//            cout << "1n1p =======================================================================\n";

        for (int i = 0; i < particles_1n1p.size(); i++) {

            float particlePDG = particles_1n1p[i]->par()->getPid();
            float Beta = particles_1n1p[i]->par()->getBeta();
            float P = particles_1n1p[i]->getP();

//                Beta_VS_P_1n1p->Fill(P, Beta);
//                P_histogram_1n1p->Fill(P);

//                cout << "particlePDG[" << i << "] = " << particlePDG << "\n";

        }

//            cout << "\n";

    }

//        Beta_VS_P_1n1p->SetStats(0);
//        Beta_VS_P_1n1p->Draw("colz");
//        cout << "\n";
//        c1->SaveAs("./plots/Beta_VS_P_1n1p.png");
//        c1->Clear();
//
//        P_histogram_1n1p->Draw("colz");
//        cout << "\n";
//        c1->SaveAs("./plots/P_histogram_1n1p.png");
//        c1->Clear();

*/
    //</editor-fold>


/*


//  Canvas definitions
// =======================================================================================================================================================================

        //<editor-fold desc="Canvas definitions">

//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1500, 1000); // original
//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1500, 1250);
//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1500, 1150);
        TCanvas *c1 = new
                TCanvas("canvas", "canvas", 1650, 1150);
        c1->cd();
        c1->SetGrid();
        c1->SetBottomMargin(0.1275);
//    c1->SetBottomMargin(0.125);
//    c1->SetBottomMargin(0.115); // original

        if (wider_margin) {
            c1->SetLeftMargin(0.1275);
            c1->SetRightMargin(0.1275);
        }
        //</editor-fold>


//  Histograms plots
// =======================================================================================================================================================================

        //<editor-fold desc="Histograms plots">

//  Theta histograms
//  ===================================================================================================

        if (Theta_plots) {

            cout << "\n";
            cout << "\n";
            cout << "Plotting Theta histograms...\n";
            cout << "\n";

//  Theta of outgoing lepton histograms --------------------------------------------------------------

//      Normalization factor:
            double theta_l_integral = Theta_l_histogram->Integral() + theta_l_1n1p->Integral();

            //<editor-fold desc="Theta of outgoing lepton histogram (2p)">
            histPlotter1D(c1, Theta_l_histogram, normalized_theta_l_plots, true, theta_l_integral, "#theta_{l} of Outgoing Lepton", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, true, true, ThetaStack, "Theta_of_lepton", "plots/theta_histograms/", "2p", kBlue, true, true, true);
            //</editor-fold>

            //<editor-fold desc="Theta of outgoing lepton histogram (1n1p)">
            histPlotter1D(c1, theta_l_1n1p, normalized_theta_l_plots, true, theta_l_integral, "#theta_{l} of Outgoing Lepton", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, true, true, ThetaStack, "Theta_of_lepton", "plots/theta_histograms/", "1n1p", kRed, true, true, true);
            //</editor-fold>

            //<editor-fold desc="Theta of outgoing lepton histogram (stack)">
            ThetaStack->Draw("nostack");
            ThetaStack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
            ThetaStack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
            ThetaStack->GetHistogram()->GetXaxis()->CenterTitle(true);
            ThetaStack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

            if (normalized_theta_l_plots) {
                ThetaStack->SetTitle("#theta_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p) - Normalized");
                ThetaStack->GetYaxis()->SetTitle("Probability (%)");
                ThetaStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            } else {
                ThetaStack->GetYaxis()->SetTitle("Arbitrary units");
                ThetaStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            }

            auto ThetaStack_legend = new
                    TLegend(0.75, 0.775, 0.875, 0.9);

            TLegendEntry *ThetaStack_entry_2p = ThetaStack_legend->AddEntry(Theta_l_histogram, "2p", "l");
            TLegendEntry *ThetaStack_entry_1n1p = ThetaStack_legend->AddEntry(theta_l_1n1p, "1n1p", "l");
            ThetaStack_legend->Draw();

            plots->Add(ThetaStack);
            c1->SetLogy(1);
            c1->SaveAs("plots/theta_histograms/Theta_of_lepton_stack_log_scale.png");
            c1->SetLogy(0);
            c1->SaveAs("plots/theta_histograms/Theta_of_lepton_stack_linear_scale.png");
            c1->Clear();
            //</editor-fold>

//  Theta of nucleon 1 histogram ----------------------------------------------------------------------

            //<editor-fold desc="Theta of Proton 1 histogram (2p)">
            histPlotter1D(c1, Theta_p1_histogram, normalized_theta_p1_plots, true, 1., "#theta_{p1} of Proton 1", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, ThetaStack, "Theta_of_proton_1", "plots/theta_histograms/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="Theta of Proton histogram (1n1p)">
            histPlotter1D(c1, theta_p_1n1p, normalized_theta_p_plots, true, 1., "#theta_{p} of Scattered Proton", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, ThetaStack, "Theta_of_proton", "plots/theta_histograms/", "1n1p", kRed, true, false, true);
            //</editor-fold>

//  Theta of nucleon 2 histogram ---------------------------------------------------------------------

            //<editor-fold desc="Theta of Proton 2 histogram (2p)">
            histPlotter1D(c1, Theta_p2_histogram, normalized_theta_p2_plots, true, 1., "#theta_{p2} of Proton 2", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, ThetaStack, "Theta_of_proton_2", "plots/theta_histograms/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="Theta of Neutron histogram (1n1p)">
            histPlotter1D(c1, theta_n_1n1p, normalized_theta_p_plots, true, 1., "#theta_{n} of Scattered Neutron", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, ThetaStack, "Theta_of_neutron", "plots/theta_histograms/", "1n1p", kRed, true, false, true);
            //</editor-fold>

//  dTheta and dPhi histograms ----------------------------------------------------------------------------------

            //<editor-fold desc="dTheta histogram (2p)">
            histPlotter1D(c1, dtheta_2p, normalized_dtheta_2p_plots, true, 1., "#gamma = #theta_{p1} - #theta_{p2} of Scattered Protons", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, ThetaStack, "gamma_of_protons", "plots/theta_histograms/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="dTheta histogram (1n1p)">
            histPlotter1D(c1, dtheta_1n1p, normalized_dtheta_1n1p_plots, true, 1., "#gamma = #theta_{p} - #theta_{n} of Scattered Nucleons", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, ThetaStack, "gamma_of_nucleons", "plots/theta_histograms/", "1n1p", kRed, true, false, true);
            //</editor-fold>

        }

// Phi histograms
// ====================================================================================================

        if (Phi_plots) {

            cout << "\n";
            cout << "\n";
            cout << "Plotting Phi histograms...\n";
            cout << "\n";

//  Phi of outgoing lepton histogram ---------------------------------------------------------------------------

//      Normalization factor:
            double phi_l_integral = phi_l_2p->Integral() + phi_l_1n1p->Integral();

            //<editor-fold desc="Phi of outgoing lepton histogram (2p)">
            histPlotter1D(c1, phi_l_2p, normalized_phi_l_plots, true, phi_l_integral, "#phi_{l} of Outgoing Lepton", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_lepton", "plots/phi_histograms/", "2p", kBlue, true, true, true);
            //</editor-fold>

            //<editor-fold desc="Phi of outgoing lepton histogram (1n1p)">
            histPlotter1D(c1, phi_l_1n1p, normalized_phi_l_plots, true, phi_l_integral, "#phi_{l} of Outgoing Lepton", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_lepton", "plots/phi_histograms/", "1n1p", kRed, true, true, true);
            //</editor-fold>

            //<editor-fold desc="Phi of outgoing lepton histogram (stack)">
            PhiStack->Draw("nostack");
            PhiStack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
            PhiStack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
            PhiStack->GetHistogram()->GetXaxis()->CenterTitle(true);
            PhiStack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

            auto PhiStack_legend = new
                    TLegend(0.775, 0.775, 0.9, 0.9);
//        auto PhiStack_legend = new TLegend(0.8, 0.6, 0.9, 0.7);

            TLegendEntry *PhiStack_entry_2p = PhiStack_legend->AddEntry(phi_l_2p, "2p", "l");
            TLegendEntry *PhiStack_entry_1n1p = PhiStack_legend->AddEntry(phi_l_1n1p, "1n1p", "l");
            PhiStack_legend->Draw();

            plots->Add(PhiStack);
            c1->SetLogy(0);
            c1->SaveAs("plots/phi_histograms/Phi_of_lepton_stack.png");
            c1->Clear();
            //</editor-fold>

//  Phi of nucleon 1 histogram ------------------------------------------------------------------------

            //<editor-fold desc="Phi of Proton 1 histogram (2p)">
            histPlotter1D(c1, phi_p1_2p, normalized_phi_p1_plots, true, 1., "#phi_{p1} of Scattered Proton 1", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_proton_1", "plots/phi_histograms/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="Phi of Proton histogram (1n1p)">
            histPlotter1D(c1, phi_p_1n1p, normalized_phi_p_plots, true, 1., "#phi_{p} of Scattered Proton", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_proton", "plots/phi_histograms/", "1n1p", kRed, true, false, true);
            //</editor-fold>

//  Phi of nucleon 2 histogram ------------------------------------------------------------------------

            //<editor-fold desc="Phi of Proton 2 histogram (2p)">
            histPlotter1D(c1, phi_p2_2p, normalized_phi_p2_plots, true, 1., "#phi_{p2} of Scattered Proton 2", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_proton_2", "plots/phi_histograms/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="Phi of Neutron histogram (1n1p)">
            histPlotter1D(c1, phi_n_1n1p, normalized_phi_p_plots, true, 1., "#phi_{n} of Scattered Neutron", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_neutron", "plots/phi_histograms/", "1n1p", kRed, true, false, true);
            //</editor-fold>

//  dPhi histograms ----------------------------------------------------------------------------------

            //<editor-fold desc="dPhi histogram (2p)">
            histPlotter1D(c1, dphi_2p, normalized_dphi_2p_plots, true, 1., "#Delta#phi = #phi_{p1} - #phi_{p2} of Scattered Protons", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "dPhi_of_protons", "plots/phi_histograms/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="dPhi histogram (1n1p)">
            histPlotter1D(c1, dphi_1n1p, normalized_dphi_1n1p_plots, true, 1., "#Delta#phi = #phi_{p} - #phi_{n} of Scattered Nucleons", "All Interactions",
                          0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "dPhi_of_protons", "plots/phi_histograms/", "1n1p", kRed, true, false, true);
            //</editor-fold>

        }

// Energy histograms
// ====================================================================================================

        if (Energy_histogram_plots) {

            cout << "\n";
            cout << "\n";
            cout << "Plotting energy histograms...\n";
            cout << "\n";

//  El histograms --------------------------------------------------------------------------------------

//      Normalization factor:
            double fsEl_integral = fsEl_2p->Integral() + fsEl_1n1p->Integral();

            //<editor-fold desc="El histograms (2p)">

            //<editor-fold desc="El histograms (all interactions, 2p)">
            histPlotter1D(c1, fsEl_2p, normalized_E_l_all_int_plots, true, fsEl_integral, "Final State E_{l}", "All Interactions", 0.06, 0.0425, 0.0425,
                          plots, 2, true, true, EnergyStack, "Final_State_El", "plots/Energy_histograms/El_histograms/all_interactions/", "2p", kBlue, true, true, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (QEL only, 2p)">
            histPlotter1D(c1, fsEl_QEL_2p, normalized_E_l_QEL_plots, true, fsEl_integral, "Final State E_{l}", "QEL Only", 0.06, 0.0425, 0.0425,
                          plots, 2, true, true, EnergyStack, "Final_State_El_QEL", "plots/Energy_histograms/El_histograms/QEL_only/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (MEC only, 2p)">
            histPlotter1D(c1, fsEl_MEC_2p, normalized_E_l_MEC_plots, true, fsEl_integral, "Final State E_{l}", "MEC Only", 0.06, 0.0425, 0.0425,
                          plots, 2, true, true, EnergyStack, "Final_State_El_MEC", "plots/Energy_histograms/El_histograms/MEC_only/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (RES only, 2p)">
            histPlotter1D(c1, fsEl_RES_2p, normalized_E_l_RES_plots, true, fsEl_integral, "Final State E_{l}", "RES Only", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, EnergyStack, "Final_State_El_RES", "plots/Energy_histograms/El_histograms/RES_only/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (DIS, 2p)">
            histPlotter1D(c1, fsEl_DIS_2p, normalized_E_l_DIS_plots, true, fsEl_integral, "Final State E_{l}", "DIS Only", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, EnergyStack, "Final_State_El_DIS", "plots/Energy_histograms/El_histograms/DIS_only/", "2p", kBlue, true, false, true);
            //</editor-fold>

            //</editor-fold>

            //<editor-fold desc="El histograms (1n1p)">

            //<editor-fold desc="El histograms (all interaction, 1n1p)">
            histPlotter1D(c1, fsEl_1n1p, normalized_E_l_all_int_plots, true, fsEl_integral, "Final State E_{l}", "All Interactions", 0.06, 0.0425, 0.0425,
                          plots, 2, true, true, EnergyStack, "Final_State_El", "plots/Energy_histograms/El_histograms/all_interactions/", "1n1p", kRed, true, true, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (QEL only, 1n1p)">
            histPlotter1D(c1, fsEl_QEL_1n1p, normalized_E_l_QEL_plots, true, fsEl_integral, "Final State E_{l}", "QEL Only", 0.06, 0.0425, 0.0425,
                          plots, 2, true, true, EnergyStack, "Final_State_El_QEL", "plots/Energy_histograms/El_histograms/QEL_only/", "1n1p", kRed, true, false, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (MEC only, 1n1p)">
            histPlotter1D(c1, fsEl_MEC_1n1p, normalized_E_l_MEC_plots, true, fsEl_integral, "Final State E_{l}", "MEC Only", 0.06, 0.0425, 0.0425,
                          plots, 2, true, true, EnergyStack, "Final_State_El_MEC", "plots/Energy_histograms/El_histograms/MEC_only/", "1n1p", kRed, true, false, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (RES only, 1n1p)">
            histPlotter1D(c1, fsEl_RES_1n1p, normalized_E_l_RES_plots, true, fsEl_integral, "Final State E_{l}", "RES Only", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, EnergyStack, "Final_State_El_RES", "plots/Energy_histograms/El_histograms/RES_only/", "1n1p", kRed, true, false, true);
            //</editor-fold>

            //<editor-fold desc="El histograms (DIS, 1n1p)">
            histPlotter1D(c1, fsEl_DIS_1n1p, normalized_E_l_DIS_plots, true, fsEl_integral, "Final State E_{l}", "DIS Only", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, EnergyStack, "Final_State_El_DIS", "plots/Energy_histograms/El_histograms/DIS_only/", "1n1p", kRed, true, false, true);
            //</editor-fold>

            //</editor-fold>

            //<editor-fold desc="El histograms (all interactions, stack)">
            EnergyStack->Draw("nostack");
            EnergyStack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
            EnergyStack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
            EnergyStack->GetHistogram()->GetXaxis()->CenterTitle(true);
            EnergyStack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

            if (normalized_E_l_plots) {
                EnergyStack->SetTitle("Final State E_{l} (All Interactions, 2p and 1n1p) - Normalized");
                EnergyStack->GetYaxis()->SetTitle("Probability (%)");
                EnergyStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            } else {
                EnergyStack->GetYaxis()->SetTitle("Arbitrary units");
                EnergyStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            }

            auto EnergyStack_legend = new
                    TLegend(0.75, 0.775, 0.875, 0.9);

            TLegendEntry *EnergyStack_entry_2p = EnergyStack_legend->AddEntry(fsEl_2p, "2p", "l");
            TLegendEntry *EnergyStack_entry_1n1p = EnergyStack_legend->AddEntry(fsEl_1n1p, "1n1p", "l");
            EnergyStack_legend->Draw();

            plots->Add(EnergyStack);
            c1->SetLogy(1);
            c1->SaveAs("plots/Energy_histograms/El_histograms/Final_State_El_log_scale.png");
            c1->SetLogy(0);
            c1->SaveAs("plots/Energy_histograms/El_histograms/Final_State_El_linear_scale.png");
            c1->Clear();
            //</editor-fold>

//  Final State E_{l} vs #theta_{l} histogram ---------------------------------------------------------

            double set_Max_z = 4000;;
//        double set_Max_z = 1.5;

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (2p)">

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (all interactions, 2p)">
            double factor_El_VS_theta_l_all_int_2p = 1.;

            if (normalized_E_l_plots) {
                fsEl_VS_theta_l_all_int_2p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 2p) - Normalized");
                fsEl_VS_theta_l_all_int_2p->Scale(factor_El_VS_theta_l_all_int_2p / fsEl_VS_theta_l_all_int_2p->Integral(), "width");
                fsEl_VS_theta_l_all_int_2p->Draw("colz");
                fsEl_VS_theta_l_all_int_2p->SetMaximum(set_Max_z);
            } else {
                fsEl_VS_theta_l_all_int_2p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 2p)");
                fsEl_VS_theta_l_all_int_2p->Draw("colz");
//
                fsEl_VS_theta_l_all_int_2p->SetMaximum(set_Max_z);
//
            }

            fsEl_VS_theta_l_all_int_2p->SetTitleSize(0.06, "xyz");
            fsEl_VS_theta_l_all_int_2p->GetXaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_all_int_2p->GetXaxis()->CenterTitle(true);
            fsEl_VS_theta_l_all_int_2p->GetYaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_all_int_2p->GetYaxis()->CenterTitle(true);
            fsEl_VS_theta_l_all_int_2p->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(fsEl_VS_theta_l_all_int_2p);
            fsEl_VS_theta_l_all_int_2p->SetStats(0);
            c1->SetLogz(1);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_l_histogram_all_int_log_scale_2p.png");
            c1->SetLogz(0);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_l_histogram_all_int_linear_scale_2p.png");
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (QEL only, 2p)">
            double factor_El_VS_theta_l_QEL_only_2p = 1.;

            if (normalized_E_l_plots) {
                fsEl_VS_theta_l_QEL_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 2p) - Normalized");
                fsEl_VS_theta_l_QEL_only_2p->Scale(factor_El_VS_theta_l_QEL_only_2p / fsEl_VS_theta_l_QEL_only_2p->Integral(), "width");
                fsEl_VS_theta_l_QEL_only_2p->Draw("colz");
                fsEl_VS_theta_l_QEL_only_2p->SetMaximum(set_Max_z);
            } else {
                fsEl_VS_theta_l_QEL_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 2p)");
                fsEl_VS_theta_l_QEL_only_2p->Draw("colz");
                //
                fsEl_VS_theta_l_QEL_only_2p->SetMaximum(set_Max_z);
                //
            }

            fsEl_VS_theta_l_QEL_only_2p->SetTitleSize(0.06, "xyz");
            fsEl_VS_theta_l_QEL_only_2p->GetXaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_QEL_only_2p->GetXaxis()->CenterTitle(true);
            fsEl_VS_theta_l_QEL_only_2p->GetYaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_QEL_only_2p->GetYaxis()->CenterTitle(true);
            fsEl_VS_theta_l_QEL_only_2p->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(fsEl_VS_theta_l_QEL_only_2p);
            fsEl_VS_theta_l_QEL_only_2p->SetStats(0);
            c1->SetLogz(1);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_l_histogram_QEL_only_log_scale_2p.png");
            c1->SetLogz(0);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_l_histogram_QEL_only_linear_scale_2p.png");
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (MEC only, 2p)">
            double factor_El_VS_theta_l_MEC_only_2p = 1.;

            if (normalized_E_l_plots) {
                fsEl_VS_theta_l_MEC_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 2p) - Normalized");
                fsEl_VS_theta_l_MEC_only_2p->Scale(factor_El_VS_theta_l_MEC_only_2p / fsEl_VS_theta_l_MEC_only_2p->Integral(), "width");
                fsEl_VS_theta_l_MEC_only_2p->Draw("colz");
                fsEl_VS_theta_l_MEC_only_2p->SetMaximum(set_Max_z);
            } else {
                fsEl_VS_theta_l_MEC_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 2p)");
                fsEl_VS_theta_l_MEC_only_2p->Draw("colz");
                //
                fsEl_VS_theta_l_MEC_only_2p->SetMaximum(set_Max_z);
                //
            }

            fsEl_VS_theta_l_MEC_only_2p->SetTitleSize(0.06, "xyz");
            fsEl_VS_theta_l_MEC_only_2p->GetXaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_MEC_only_2p->GetXaxis()->CenterTitle(true);
            fsEl_VS_theta_l_MEC_only_2p->GetYaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_MEC_only_2p->GetYaxis()->CenterTitle(true);
            fsEl_VS_theta_l_MEC_only_2p->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(fsEl_VS_theta_l_MEC_only_2p);
            fsEl_VS_theta_l_MEC_only_2p->SetStats(0);
            c1->SetLogz(1);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_l_histogram_MEC_only_log_scale_2p.png");
            c1->SetLogz(0);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_l_histogram_MEC_only_linear_scale_2p.png");
            c1->Clear();
            //</editor-fold>

            //</editor-fold>

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (1n1p)">

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (all interactions, 1n1p)">
            double factor_El_VS_theta_l_all_int_1n1p = 1.;

            if (normalized_E_l_plots) {
                fsEl_VS_theta_l_all_int_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 1n1p) - Normalized");
                fsEl_VS_theta_l_all_int_1n1p->Scale(factor_El_VS_theta_l_all_int_1n1p / fsEl_VS_theta_l_all_int_1n1p->Integral(), "width");
                fsEl_VS_theta_l_all_int_1n1p->Draw("colz");
                fsEl_VS_theta_l_all_int_1n1p->SetMaximum(set_Max_z);
            } else {
                fsEl_VS_theta_l_all_int_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 1n1p)");
                fsEl_VS_theta_l_all_int_1n1p->Draw("colz");
                //
                fsEl_VS_theta_l_all_int_1n1p->SetMaximum(set_Max_z);
                //
            }

            fsEl_VS_theta_l_all_int_1n1p->SetTitleSize(0.06, "xyz");
            fsEl_VS_theta_l_all_int_1n1p->GetXaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_all_int_1n1p->GetXaxis()->CenterTitle(true);
            fsEl_VS_theta_l_all_int_1n1p->GetYaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_all_int_1n1p->GetYaxis()->CenterTitle(true);
            fsEl_VS_theta_l_all_int_1n1p->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(fsEl_VS_theta_l_all_int_1n1p);
            fsEl_VS_theta_l_all_int_1n1p->SetStats(0);
            c1->SetLogz(1);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_l_histogram_all_int_log_scale_1n1p.png");
            c1->SetLogz(0);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_l_histogram_all_int_linear_scale_1n1p.png");
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (QEL only, 1n1p)">
            double factor_El_VS_theta_l_QEL_only_1n1p = 1.;

            if (normalized_E_l_plots) {
                fsEl_VS_theta_l_QEL_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 1n1p) - Normalized");
                fsEl_VS_theta_l_QEL_only_1n1p->Scale(factor_El_VS_theta_l_QEL_only_1n1p / fsEl_VS_theta_l_QEL_only_1n1p->Integral(), "width");
                fsEl_VS_theta_l_QEL_only_1n1p->Draw("colz");
                fsEl_VS_theta_l_QEL_only_1n1p->SetMaximum(set_Max_z);
            } else {
                fsEl_VS_theta_l_QEL_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 1n1p)");
                fsEl_VS_theta_l_QEL_only_1n1p->Draw("colz");
                //
                fsEl_VS_theta_l_QEL_only_1n1p->SetMaximum(set_Max_z);
                //
            }

            fsEl_VS_theta_l_QEL_only_1n1p->SetTitleSize(0.06, "xyz");
            fsEl_VS_theta_l_QEL_only_1n1p->GetXaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
            fsEl_VS_theta_l_QEL_only_1n1p->GetYaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
            fsEl_VS_theta_l_QEL_only_1n1p->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(fsEl_VS_theta_l_QEL_only_1n1p);
            fsEl_VS_theta_l_QEL_only_1n1p->SetStats(0);
            c1->SetLogz(1);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_l_histogram_QEL_only_log_scale_1n1p.png");
            c1->SetLogz(0);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_l_histogram_QEL_only_linear_scale_1n1p.png");
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (MEC only, 1n1p)">
            double factor_El_VS_theta_l_MEC_only_1n1p = 1.;

            if (normalized_E_l_plots) {
                fsEl_VS_theta_l_MEC_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 1n1p) - Normalized");
                fsEl_VS_theta_l_MEC_only_1n1p->Scale(factor_El_VS_theta_l_MEC_only_1n1p / fsEl_VS_theta_l_MEC_only_1n1p->Integral(), "width");
                fsEl_VS_theta_l_MEC_only_1n1p->Draw("colz");
                fsEl_VS_theta_l_MEC_only_1n1p->SetMaximum(set_Max_z);
            } else {
                fsEl_VS_theta_l_MEC_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 1n1p)");
                fsEl_VS_theta_l_MEC_only_1n1p->Draw("colz");
                //
                fsEl_VS_theta_l_MEC_only_1n1p->SetMaximum(set_Max_z);
                //
            }

            fsEl_VS_theta_l_MEC_only_1n1p->SetTitleSize(0.06, "xyz");
            fsEl_VS_theta_l_MEC_only_1n1p->GetXaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_MEC_only_1n1p->GetXaxis()->CenterTitle(true);
            fsEl_VS_theta_l_MEC_only_1n1p->GetYaxis()->SetLabelSize(0.0425);
            fsEl_VS_theta_l_MEC_only_1n1p->GetYaxis()->CenterTitle(true);
            fsEl_VS_theta_l_MEC_only_1n1p->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(fsEl_VS_theta_l_MEC_only_1n1p);
            fsEl_VS_theta_l_MEC_only_1n1p->SetStats(0);
            c1->SetLogz(1);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_l_histogram_MEC_only_log_scale_1n1p.png");
            c1->SetLogz(0);
            c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_l_histogram_MEC_only_linear_scale_1n1p.png");
            c1->Clear();
            //</editor-fold>

            //</editor-fold>

        }

// Energy transfer histograms (all interactions)
// ====================================================================================================

//    //<editor-fold desc="Energy transfer histograms (all interactions) - normalization factors">
//
////  Normalization factors:
//
//    double E_Trans15_all_integral_2p = E_Trans15_all_2p->Integral(); // MOVE TO OTHER PLACE
//    double E_Trans15_all_integral_1n1p = E_Trans15_all_1n1p->Integral(); // MOVE TO OTHER PLACE
//
//    double E_Trans15_QEL_integral_2p = E_Trans15_QEL_2p->Integral(); // MOVE TO OTHER PLACE
//    double E_Trans15_QEL_integral_1n1p = E_Trans15_QEL_1n1p->Integral(); // MOVE TO OTHER PLACE
//
//    double E_Trans15_MEC_integral_2p = E_Trans15_MEC_2p->Integral(); // MOVE TO OTHER PLACE
//    double E_Trans15_MEC_integral_1n1p = E_Trans15_MEC_1n1p->Integral(); // MOVE TO OTHER PLACE
//
//    double E_Trans15_RES_integral_2p = E_Trans15_RES_2p->Integral(); // MOVE TO OTHER PLACE
//    double E_Trans15_RES_integral_1n1p = E_Trans15_RES_1n1p->Integral(); // MOVE TO OTHER PLACE
//
//    double E_Trans15_DIS_integral_2p = E_Trans15_DIS_2p->Integral(); // MOVE TO OTHER PLACE
//    double E_Trans15_DIS_integral_1n1p = E_Trans15_DIS_1n1p->Integral(); // MOVE TO OTHER PLACE
//    //</editor-fold>

//    if (ET_all_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting energy transfer histograms (all interactions)...\n";
//        cout << "\n";
//
////  Energy transfer (Ev-El) for every theta_{l} --------------------------------------------------------
//
//        //<editor-fold desc="Energy transfer (Ev-El) for every theta_{l} (2p)">
//        histPlotter1D(c1, E_Trans_all_ang_all_int_2p, normalized_E_Trans_all_ang_all_int_plots, false, 1., "Energy Transfer (E_{#nu}-E_{l}) for every angle",
//                      "All Interactions", 0.06, 0.0425, 0.0425, plots, 3, true, true, Energy_Transfer_all_int_15_Stack_2p,
//                      "Energy_transfer_Ev-El_all_Deg_all_interactions", "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/", "2p", kBlack,
//                      true, false, true);
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer (Ev-El) for every theta_{l} (1n1p)">
//        histPlotter1D(c1, E_Trans_all_ang_all_int_1n1p, normalized_E_Trans_all_ang_all_int_plots, false, 1., "Energy Transfer (E_{#nu}-E_{l}) for every angle",
//                      "All Interactions", 0.06, 0.0425, 0.0425, plots, 3, true, true, Energy_Transfer_all_int_15_Stack_1n1p,
//                      "Energy_transfer_Ev-El_all_Deg_all_interactions", "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/", "1n1p", kBlack,
//                      true, false, true);
//        //</editor-fold>
//
//
////  Energy transfer (Ev-El) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (all interactions) ------
//
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_all_2p->SetTitle(
//                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, 2p) - Normalized");
//            E_Trans15_all_2p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            E_Trans15_all_2p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_all_2p->Scale(100. / E_Trans15_all_integral_2p, "nosw2");
//            E_Trans15_all_2p->Draw();
//        } else {
//            E_Trans15_all_2p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_all_2p->Draw();
//        }
//
////        E_Trans15_all_2p->Draw();
//        plots->Add(E_Trans15_all_2p);
//        E_Trans15_all_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_15_Deg_all_interactions_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_15_Deg_all_interactions_normal_scale_2p.png");
//        E_Trans15_all_2p->SetStats(0);
//        E_Trans15_all_2p->SetLineColor(kBlack);
//        E_Trans15_all_2p->SetLineWidth(4);
//        E_Trans15_all_2p->SetLineStyle(5);
//        Energy_Transfer_all_int_15_Stack_2p->Add(E_Trans15_all_2p);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer (Ev-El) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (all interactions, 1n1p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_all_1n1p->SetTitle(
//                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, 1n1p) - Normalized");
//            E_Trans15_all_1n1p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            E_Trans15_all_1n1p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_all_1n1p->Scale(100. / E_Trans15_all_integral_1n1p, "nosw2");
//            E_Trans15_all_1n1p->Draw();
//        } else {
//            E_Trans15_all_1n1p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_all_1n1p->Draw();
//        }
//
////        E_Trans15_all_1n1p->Draw();
//        plots->Add(E_Trans15_all_1n1p);
//        E_Trans15_all_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_15_Deg_all_interactions_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_15_Deg_all_interactions_normal_scale_1n1p.png");
//        E_Trans15_all_1n1p->SetStats(0);
//        E_Trans15_all_1n1p->SetLineColor(kBlack);
//        E_Trans15_all_1n1p->SetLineWidth(4);
//        E_Trans15_all_1n1p->SetLineStyle(5);
//        Energy_Transfer_all_int_15_Stack_1n1p->Add(E_Trans15_all_1n1p);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (all interactions) --------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (all interactions, 2p)">
//        E_Trans45_all_2p->Draw();
//        plots->Add(E_Trans45_all_2p);
//        E_Trans45_all_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_45_Deg_all_interactions_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_45_Deg_all_interactions_normal_scale_2p.png");
//        E_Trans45_all_2p->SetLineColor(kRed);
//        E_Trans45_all_2p->SetLineColor(kViolet - 3);
//        E_Trans45_all_2p->SetStats(0);
//        E_Trans45_all_2p->SetLineColor(kBlack);
//        E_Trans45_all_2p->SetLineWidth(3);
//        E_Trans45_all_2p->SetLineStyle(4);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (all interactions, 1n1p)">
//        E_Trans45_all_1n1p->Draw();
//        plots->Add(E_Trans45_all_1n1p);
//        E_Trans45_all_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_45_Deg_all_interactions_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_45_Deg_all_interactions_normal_scale_1n1p.png");
//        E_Trans45_all_1n1p->SetLineColor(kRed);
//        E_Trans45_all_1n1p->SetStats(0);
//        E_Trans45_all_1n1p->SetLineColor(kBlack);
//        E_Trans45_all_1n1p->SetLineWidth(3);
//        E_Trans45_all_1n1p->SetLineStyle(4);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (all interactions) --------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (all interactions, 2p)">
//        E_Trans90_all_2p->Draw();
//        plots->Add(E_Trans90_all_2p);
//        E_Trans90_all_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_90_Deg_all_interactions_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_90_Deg_all_interactions_normal_scale_2p.png");
//        E_Trans90_all_2p->SetLineColor(kGreen);
//        E_Trans90_all_2p->SetStats(0);
//        E_Trans90_all_2p->SetLineColor(kBlack);
//        E_Trans90_all_2p->SetLineWidth(3);
//        E_Trans90_all_2p->SetLineStyle(6);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (all interactions, 1n1p)">
//        E_Trans90_all_1n1p->Draw();
//        plots->Add(E_Trans90_all_1n1p);
//        E_Trans90_all_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_90_Deg_all_interactions_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs(
//                "plots/Energy_transfer_histograms/Energy_transfer_histograms_all_interactions/Energy_transfer_Ev-El_around_90_Deg_all_interactions_normal_scale_1n1p.png");
//        E_Trans90_all_1n1p->SetLineColor(kGreen);
//        E_Trans90_all_1n1p->SetStats(0);
//        E_Trans90_all_1n1p->SetLineColor(kBlack);
//        E_Trans90_all_1n1p->SetLineWidth(3);
//        E_Trans90_all_1n1p->SetLineStyle(6);
//        c1->Clear();
//        //</editor-fold>
//
//
//// Energy transfer Ev-El stack (all interactions) -----------------------------------------------------
//
////        E_Trans15_all_2p->SetLineStyle(1);
////        E_Trans45_all_2p->SetLineStyle(1);
////        E_Trans90_all_2p->SetLineStyle(1);
//
//////        TO COMMENT LATER:
////        E_Trans15_all_1n1p->SetLineStyle(1);
////        E_Trans45_all_1n1p->SetLineStyle(1);
////        E_Trans90_all_1n1p->SetLineStyle(1);
//
//    }

// Energy transfer histograms (QEL only)
// ====================================================================================================

//    if (ET_QEL_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting energy transfer histograms (QEL only)...\n";
//        cout << "\n";
//
////  Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (QEL only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (QEL only, 2p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_QEL_2p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, 2p) - Normalized");
//            E_Trans15_QEL_2p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            E_Trans15_QEL_2p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_QEL_2p->Scale(100. * (1. / E_Trans15_all_integral_2p), "nosw2");
//            E_Trans15_QEL_2p->Draw();
//        } else {
//            E_Trans15_QEL_2p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_QEL_2p->Draw();
//        }
//
////        E_Trans15_QEL_2p->Draw();
//        plots->Add(E_Trans15_QEL_2p);
//        E_Trans15_QEL_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_15_Deg_QEL_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_15_Deg_QEL_only_normal_scale_2p.png");
//        E_Trans15_QEL_2p->SetLineColor(kBlue);
//        Energy_Transfer_QEL_Int_Stack_2p->Add(E_Trans15_QEL_2p);
//        E_Trans15_QEL_2p->SetStats(0);
//        E_Trans15_QEL_2p->SetLineColor(kAzure + 10);
//        Energy_Transfer_all_int_15_Stack_2p->Add(E_Trans15_QEL_2p);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (QEL only, 1n1p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_QEL_1n1p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, 1n1p) - Normalized");
//            E_Trans15_QEL_1n1p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_QEL_1n1p->Scale(100. * (1. / E_Trans15_all_integral_1n1p), "nosw2");
//            E_Trans15_QEL_1n1p->Draw();
//        } else {
//            E_Trans15_QEL_1n1p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_QEL_1n1p->Draw();
//        }
//
////        E_Trans15_QEL_1n1p->Draw();
//        plots->Add(E_Trans15_QEL_1n1p);
//        E_Trans15_QEL_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_15_Deg_QEL_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_15_Deg_QEL_only_normal_scale_1n1p.png");
//        E_Trans15_QEL_1n1p->SetLineColor(kBlue);
//        Energy_Transfer_QEL_Int_Stack_1n1p->Add(E_Trans15_QEL_1n1p);
//        E_Trans15_QEL_1n1p->SetStats(0);
//        E_Trans15_QEL_1n1p->SetLineColor(kAzure + 10);
////        E_Trans15_QEL_1n1p->SetLineStyle(2);
//        Energy_Transfer_all_int_15_Stack_1n1p->Add(E_Trans15_QEL_1n1p);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (QEL only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (QEL only, 2p)">
//        E_Trans45_QEL_2p->Draw();
//        plots->Add(E_Trans45_QEL_2p);
//        E_Trans45_QEL_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_45_Deg_QEL_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_45_Deg_QEL_only_normal_scale_2p.png");
//        E_Trans45_QEL_2p->SetLineColor(kRed);
//        Energy_Transfer_QEL_Int_Stack_2p->Add(E_Trans45_QEL_2p);
//        E_Trans45_QEL_2p->SetStats(0);
//        E_Trans45_QEL_2p->SetLineColor(kAzure + 10);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (QEL only, 1n1p)">
//        E_Trans45_QEL_1n1p->Draw();
//        plots->Add(E_Trans45_QEL_1n1p);
//        E_Trans45_QEL_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_45_Deg_QEL_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_45_Deg_QEL_only_normal_scale_1n1p.png");
//        E_Trans45_QEL_1n1p->SetLineColor(kRed);
//        Energy_Transfer_QEL_Int_Stack_1n1p->Add(E_Trans45_QEL_1n1p);
//        E_Trans45_QEL_1n1p->SetStats(0);
//        E_Trans45_QEL_1n1p->SetLineColor(kAzure + 10);
//        E_Trans45_QEL_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (QEL only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (QEL only, 2p)">
//        E_Trans90_QEL_2p->Draw();
//        plots->Add(E_Trans90_QEL_2p);
//        E_Trans90_QEL_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_90_Deg_QEL_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_90_Deg_QEL_only_normal_scale_2p.png");
//        E_Trans90_QEL_2p->SetLineColor(kGreen);
//        Energy_Transfer_QEL_Int_Stack_2p->Add(E_Trans90_QEL_2p);
//        E_Trans90_QEL_2p->SetStats(0);
//        E_Trans90_QEL_2p->SetLineColor(kAzure + 10);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (QEL only, 1n1p)">
//        E_Trans90_QEL_1n1p->Draw();
//        plots->Add(E_Trans90_QEL_1n1p);
//        E_Trans90_QEL_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_90_Deg_QEL_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_around_90_Deg_QEL_only_normal_scale_1n1p.png");
//        E_Trans90_QEL_1n1p->SetLineColor(kGreen);
//        Energy_Transfer_QEL_Int_Stack_1n1p->Add(E_Trans90_QEL_1n1p);
//        E_Trans90_QEL_1n1p->SetStats(0);
//        E_Trans90_QEL_1n1p->SetLineColor(kAzure + 10);
//        E_Trans90_QEL_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El stack of angles (QEL only) -------------------------------------------------------------
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (QEL only, 2p)">
//        Energy_Transfer_QEL_Int_Stack_2p->Draw("nostack");
//        Energy_Transfer_QEL_Int_Stack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_QEL_Int_Stack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_QEL_Int_Stack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_QEL_int_2p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_QEL_int_2p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_QEL_int_entry_2p = E_Trans_legend_QEL_int_2p->AddEntry(E_Trans15_QEL_2p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_QEL_int_entry_2p = E_Trans_legend_QEL_int_2p->AddEntry(E_Trans45_QEL_2p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_QEL_int_entry_2p = E_Trans_legend_QEL_int_2p->AddEntry(E_Trans90_QEL_2p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//        E_Trans_legend_QEL_int_2p->Draw();
//
//        plots->Add(Energy_Transfer_QEL_Int_Stack_2p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_QEL_only_Stack_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_QEL_only_Stack_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (QEL only, 1n1p)">
//        Energy_Transfer_QEL_Int_Stack_1n1p->Draw("nostack");
//        Energy_Transfer_QEL_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_QEL_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_QEL_Int_Stack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_QEL_int_1n1p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_QEL_int_1n1p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_QEL_int_entry_1n1p = E_Trans_legend_QEL_int_1n1p->AddEntry(E_Trans15_QEL_1n1p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_QEL_int_entry_1n1p = E_Trans_legend_QEL_int_1n1p->AddEntry(E_Trans45_QEL_1n1p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_QEL_int_entry_1n1p = E_Trans_legend_QEL_int_1n1p->AddEntry(E_Trans90_QEL_1n1p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//        E_Trans_legend_QEL_int_1n1p->Draw();
//
//        plots->Add(Energy_Transfer_QEL_Int_Stack_1n1p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_QEL_only_Stack_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_QEL_only/Energy_transfer_Ev-El_QEL_only_Stack_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// Energy transfer histograms (MEC only)
// ====================================================================================================

//    if (ET_MEC_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting energy transfer histograms (MEC only)...\n";
//        cout << "\n";
//
////  Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (MEC only) ------------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (MEC only, 2p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_MEC_2p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, 2p) - Normalized");
//            E_Trans15_MEC_2p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_MEC_2p->Scale(100. * (1. / E_Trans15_all_integral_2p), "nosw2");
//            E_Trans15_MEC_2p->Draw();
//        } else {
//            E_Trans15_MEC_2p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_MEC_2p->Draw();
//        }
//
////        E_Trans15_MEC_2p->Draw();
//        plots->Add(E_Trans15_MEC_2p);
//        E_Trans15_MEC_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_15_Deg_MEC_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_15_Deg_MEC_only_normal_scale_2p.png");
//        E_Trans15_MEC_2p->SetLineColor(kBlue);
//        Energy_Transfer_MEC_Int_Stack_2p->Add(E_Trans15_MEC_2p);
//        E_Trans15_MEC_2p->SetStats(0);
//        E_Trans15_MEC_2p->SetLineColor(kOrange + 10);
//        Energy_Transfer_all_int_15_Stack_2p->Add(E_Trans15_MEC_2p);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (MEC only, 1n1p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_MEC_1n1p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, 1n1p) - Normalized");
//            E_Trans15_MEC_1n1p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            E_Trans15_MEC_1n1p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_MEC_1n1p->Scale(100. * (1. / E_Trans15_all_integral_1n1p), "nosw2");
//            E_Trans15_MEC_1n1p->Draw();
//        } else {
//            E_Trans15_MEC_1n1p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_MEC_1n1p->Draw();
//        }
//
////        E_Trans15_MEC_1n1p->Draw();
//        plots->Add(E_Trans15_MEC_1n1p);
//        E_Trans15_MEC_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_15_Deg_MEC_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_15_Deg_MEC_only_normal_scale_1n1p.png");
//        E_Trans15_MEC_1n1p->SetLineColor(kBlue);
//        Energy_Transfer_MEC_Int_Stack_1n1p->Add(E_Trans15_MEC_1n1p);
//        E_Trans15_MEC_1n1p->SetStats(0);
//        E_Trans15_MEC_1n1p->SetLineColor(kOrange + 10);
////        E_Trans15_MEC_1n1p->SetLineStyle(2);
//        Energy_Transfer_all_int_15_Stack_1n1p->Add(E_Trans15_MEC_1n1p);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (MEC only) ------------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (MEC only, 2p)">
//        E_Trans45_MEC_2p->Draw();
//        plots->Add(E_Trans45_MEC_2p);
//        E_Trans45_MEC_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_45_Deg_MEC_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_45_Deg_MEC_only_normal_scale_2p.png");
//        E_Trans45_MEC_2p->SetLineColor(kRed);
//        Energy_Transfer_MEC_Int_Stack_2p->Add(E_Trans45_MEC_2p);
//        E_Trans45_MEC_2p->SetStats(0);
//        E_Trans45_MEC_2p->SetLineColor(kOrange + 10);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (MEC only, 1n1p)">
//        E_Trans15_MEC_1n1p->Draw();
//        plots->Add(E_Trans15_MEC_1n1p);
//        E_Trans15_MEC_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_45_Deg_MEC_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_45_Deg_MEC_only_normal_scale_1n1p.png");
//        E_Trans45_MEC_1n1p->SetLineColor(kRed);
//        Energy_Transfer_MEC_Int_Stack_1n1p->Add(E_Trans45_MEC_1n1p);
//        E_Trans45_MEC_1n1p->SetStats(0);
//        E_Trans45_MEC_1n1p->SetLineColor(kOrange + 10);
//        E_Trans45_MEC_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (MEC only) ------------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (MEC only, 2p)">
//        E_Trans90_MEC_2p->Draw();
//        plots->Add(E_Trans90_MEC_2p);
//        E_Trans90_MEC_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_90_Deg_MEC_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_90_Deg_MEC_only_normal_scale_2p.png");
//        E_Trans90_MEC_2p->SetLineColor(kGreen);
//        Energy_Transfer_MEC_Int_Stack_2p->Add(E_Trans90_MEC_2p);
//        E_Trans90_MEC_2p->SetStats(0);
//        E_Trans90_MEC_2p->SetLineColor(kOrange + 10);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (MEC only, 1n1p)">
//        E_Trans90_MEC_1n1p->Draw();
//        plots->Add(E_Trans90_MEC_1n1p);
//        E_Trans90_MEC_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_90_Deg_MEC_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_around_90_Deg_MEC_only_normal_scale_1n1p.png");
//        E_Trans90_MEC_1n1p->SetLineColor(kGreen);
//        Energy_Transfer_MEC_Int_Stack_1n1p->Add(E_Trans90_MEC_1n1p);
//        E_Trans90_MEC_1n1p->SetStats(0);
//        E_Trans90_MEC_1n1p->SetLineColor(kOrange + 10);
//        E_Trans90_MEC_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El stack of angles (MEC only) -------------------------------------------------------------
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (MEC only, 2p)">
//        Energy_Transfer_MEC_Int_Stack_2p->Draw("nostack");
//        Energy_Transfer_MEC_Int_Stack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_MEC_Int_Stack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_MEC_Int_Stack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_MEC_int_2p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_MEC_int_2p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_MEC_int_entry_2p = E_Trans_legend_MEC_int_2p->AddEntry(E_Trans15_MEC_2p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_MEC_int_entry_2p = E_Trans_legend_MEC_int_2p->AddEntry(E_Trans45_MEC_2p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_MEC_int_entry_2p = E_Trans_legend_MEC_int_2p->AddEntry(E_Trans90_MEC_2p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//
//        E_Trans_legend_MEC_int_2p->Draw();
//
//        plots->Add(Energy_Transfer_MEC_Int_Stack_2p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_MEC_only_Stack_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_MEC_only_Stack_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (MEC only, 1n1p)">
//        Energy_Transfer_MEC_Int_Stack_1n1p->Draw("nostack");
//        Energy_Transfer_MEC_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_MEC_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_MEC_Int_Stack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_MEC_int_1n1p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_MEC_int_1n1p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_MEC_int_entry_1n1p = E_Trans_legend_MEC_int_1n1p->AddEntry(E_Trans15_MEC_1n1p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_MEC_int_entry_1n1p = E_Trans_legend_MEC_int_1n1p->AddEntry(E_Trans45_MEC_1n1p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_MEC_int_entry_1n1p = E_Trans_legend_MEC_int_1n1p->AddEntry(E_Trans90_MEC_1n1p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//
//        E_Trans_legend_MEC_int_1n1p->Draw();
//
//        plots->Add(Energy_Transfer_MEC_Int_Stack_1n1p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_MEC_only_Stack_log_scale_1n1p_1111.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_MEC_only/Energy_transfer_Ev-El_MEC_only_Stack_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// Energy transfer histograms (RES only)
// ====================================================================================================

//    if (ET_RES_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting energy transfer histograms (RES only)...\n";
//        cout << "\n";
//
////  Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (RES only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (RES only, 2p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_RES_2p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, 2p) - Normalized");
//            E_Trans15_RES_2p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_RES_2p->Scale(100. * (1. / E_Trans15_all_integral_2p), "nosw2");
//            E_Trans15_RES_2p->Draw();
//        } else {
//            E_Trans15_RES_2p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_RES_2p->Draw();
//        }
//
////        E_Trans15_RES_2p->Draw();
//        plots->Add(E_Trans15_RES_2p);
//        E_Trans15_RES_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_15_Deg_RES_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_15_Deg_RES_only_normal_scale_2p.png");
//        E_Trans15_RES_2p->SetLineColor(kBlue);
//        Energy_Transfer_RES_Int_Stack_2p->Add(E_Trans15_RES_2p);
//        E_Trans15_RES_2p->SetStats(0);
//        E_Trans15_RES_2p->SetLineColor(kGreen);
//        Energy_Transfer_all_int_15_Stack_2p->Add(E_Trans15_RES_2p);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (RES only, 1n1p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_RES_1n1p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, 1n1p) - Normalized");
//            E_Trans15_RES_1n1p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            E_Trans15_RES_1n1p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_RES_1n1p->Scale(100. * (1. / E_Trans15_all_integral_1n1p), "nosw2");
//            E_Trans15_RES_1n1p->Draw();
//        } else {
//            E_Trans15_RES_1n1p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_RES_1n1p->Draw();
//        }
//
////        E_Trans15_RES_1n1p->Draw();
//        plots->Add(E_Trans15_RES_1n1p);
//        E_Trans15_RES_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_15_Deg_RES_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_15_Deg_RES_only_normal_scale_1n1p.png");
//        E_Trans15_RES_1n1p->SetLineColor(kBlue);
//        Energy_Transfer_RES_Int_Stack_1n1p->Add(E_Trans15_RES_1n1p);
//        E_Trans15_RES_1n1p->SetStats(0);
//        E_Trans15_RES_1n1p->SetLineColor(kGreen);
////        E_Trans15_RES_1n1p->SetLineStyle(2);
//        Energy_Transfer_all_int_15_Stack_1n1p->Add(E_Trans15_RES_1n1p);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (RES only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (RES only, 2p)">
//        E_Trans45_RES_2p->Draw();
//        plots->Add(E_Trans45_RES_2p);
//        E_Trans45_RES_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_45_Deg_RES_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_45_Deg_RES_only_normal_scale_2p.png");
//        E_Trans45_RES_2p->SetLineColor(kRed);
//        Energy_Transfer_RES_Int_Stack_2p->Add(E_Trans45_RES_2p);
//        E_Trans45_RES_2p->SetStats(0);
//        E_Trans45_RES_2p->SetLineColor(kGreen);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (RES only, 1n1p)">
//        E_Trans45_RES_1n1p->Draw();
//        plots->Add(E_Trans45_RES_1n1p);
//        E_Trans45_RES_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_45_Deg_RES_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_45_Deg_RES_only_normal_scale_1n1p.png");
//        E_Trans45_RES_1n1p->SetLineColor(kRed);
//        Energy_Transfer_RES_Int_Stack_1n1p->Add(E_Trans45_RES_1n1p);
//        E_Trans45_RES_1n1p->SetStats(0);
//        E_Trans45_RES_1n1p->SetLineColor(kGreen);
//        E_Trans45_RES_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (RES only) -----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (RES only, 2p)">
//        E_Trans90_RES_2p->Draw();
//        plots->Add(E_Trans90_RES_2p);
//        E_Trans90_RES_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_90_Deg_RES_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_90_Deg_RES_only_normal_scale_2p.png");
//        E_Trans90_RES_2p->SetLineColor(kGreen);
//        Energy_Transfer_RES_Int_Stack_2p->Add(E_Trans90_RES_2p);
//        E_Trans90_RES_2p->SetStats(0);
//        E_Trans90_RES_2p->SetLineColor(kGreen);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (RES only, 1n1p)">
//        E_Trans90_RES_1n1p->Draw();
//        plots->Add(E_Trans90_RES_1n1p);
//        E_Trans90_RES_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_90_Deg_RES_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_around_90_Deg_RES_only_normal_scale_1n1p.png");
//        E_Trans90_RES_1n1p->SetLineColor(kGreen);
//        Energy_Transfer_RES_Int_Stack_1n1p->Add(E_Trans90_RES_1n1p);
//        E_Trans90_RES_1n1p->SetStats(0);
//        E_Trans90_RES_1n1p->SetLineColor(kGreen);
//        E_Trans90_RES_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El stack of angles (RES only) -------------------------------------------------------------
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (RES only, 2p)">
//        Energy_Transfer_RES_Int_Stack_2p->Draw("nostack");
//        Energy_Transfer_RES_Int_Stack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_RES_Int_Stack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_RES_Int_Stack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_RES_int_2p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_RES_int_2p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_RES_int_entry_2p = E_Trans_legend_RES_int_2p->AddEntry(E_Trans15_RES_2p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_RES_int_entry_2p = E_Trans_legend_RES_int_2p->AddEntry(E_Trans45_RES_2p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_RES_int_entry_2p = E_Trans_legend_RES_int_2p->AddEntry(E_Trans90_RES_2p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//
//        E_Trans_legend_RES_int_2p->Draw();
//
//        plots->Add(Energy_Transfer_RES_Int_Stack_2p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_RES_only_Stack_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_RES_only_Stack_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (RES only, 1n1p)">
//        Energy_Transfer_RES_Int_Stack_1n1p->Draw("nostack");
//        Energy_Transfer_RES_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_RES_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_RES_Int_Stack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_RES_int_1n1p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_RES_int_1n1p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_RES_int_entry_1n1p = E_Trans_legend_RES_int_1n1p->AddEntry(E_Trans15_RES_1n1p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_RES_int_entry_1n1p = E_Trans_legend_RES_int_1n1p->AddEntry(E_Trans45_RES_1n1p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_RES_int_entry_1n1p = E_Trans_legend_RES_int_1n1p->AddEntry(E_Trans90_RES_1n1p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//
//        E_Trans_legend_RES_int_1n1p->Draw();
//
//        plots->Add(Energy_Transfer_RES_Int_Stack_1n1p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_RES_only_Stack_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_RES_only/Energy_transfer_Ev-El_RES_only_Stack_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// Energy transfer histograms (DIS only)
// ====================================================================================================

//    if (ET_DIS_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting energy transfer histograms (DIS only)...\n";
//        cout << "\n";
//
////  Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (DIS only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (DIS only, 2p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_DIS_2p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, 2p) - Normalized");
//            E_Trans15_DIS_2p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_DIS_2p->Scale(100. * (1. / E_Trans15_all_integral_2p), "nosw2");
//            E_Trans15_DIS_2p->Draw();
//        } else {
//            E_Trans15_DIS_2p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_DIS_2p->Draw();
//        }
//
////        E_Trans15_DIS_2p->Draw();
//        plots->Add(E_Trans15_DIS_2p);
//        E_Trans15_DIS_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_15_Deg_DIS_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_15_Deg_DIS_only_normal_scale_2p.png");
//        E_Trans15_DIS_2p->SetLineColor(kBlue);
//        Energy_Transfer_DIS_Int_Stack_2p->Add(E_Trans15_DIS_2p);
//        E_Trans15_DIS_2p->SetStats(0);
//        E_Trans15_DIS_2p->SetLineColor(kMagenta);
//        Energy_Transfer_all_int_15_Stack_2p->Add(E_Trans15_DIS_2p);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (DIS only, 1n1p)">
//        if (normalized_E_Trans15_plots) {
//            E_Trans15_DIS_1n1p->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, 1n1p) - Normalized");
//            E_Trans15_DIS_1n1p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            E_Trans15_DIS_1n1p->GetYaxis()->SetTitle("Probability (%)");
//            E_Trans15_DIS_1n1p->Scale(100. * (1. / E_Trans15_all_integral_1n1p), "nosw2");
//            E_Trans15_DIS_1n1p->Draw();
//        } else {
//            E_Trans15_DIS_1n1p->GetYaxis()->SetTitle("Arbitrary units");
//            E_Trans15_DIS_1n1p->Draw();
//        }
//
////        E_Trans15_DIS_1n1p->Draw();
//        plots->Add(E_Trans15_DIS_1n1p);
//        E_Trans15_DIS_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_15_Deg_DIS_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_15_Deg_DIS_only_normal_scale_1n1p.png");
//        E_Trans15_DIS_1n1p->SetLineColor(kBlue);
//        Energy_Transfer_DIS_Int_Stack_1n1p->Add(E_Trans15_DIS_1n1p);
//        E_Trans15_DIS_1n1p->SetStats(0);
//        E_Trans15_DIS_1n1p->SetLineColor(kMagenta);
////        E_Trans15_DIS_1n1p->SetLineStyle(2);
//        Energy_Transfer_all_int_15_Stack_1n1p->Add(E_Trans15_DIS_1n1p);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (DIS only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (DIS only, 2p)">
//        E_Trans45_DIS_2p->Draw();
//        plots->Add(E_Trans45_DIS_2p);
//        E_Trans45_DIS_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_45_Deg_DIS_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_45_Deg_DIS_only_normal_scale_2p.png");
//        E_Trans45_DIS_2p->SetLineColor(kRed);
//        Energy_Transfer_DIS_Int_Stack_2p->Add(E_Trans45_DIS_2p);
//        E_Trans45_DIS_2p->SetStats(0);
//        E_Trans45_DIS_2p->SetLineColor(kMagenta);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 44[Deg] <= theta_{l} <= 46[Deg] (DIS only, 1n1p)">
//        E_Trans45_DIS_1n1p->Draw();
//        plots->Add(E_Trans45_DIS_1n1p);
//        E_Trans45_DIS_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_45_Deg_DIS_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_45_Deg_DIS_only_normal_scale_1n1p.png");
//        E_Trans45_DIS_1n1p->SetLineColor(kRed);
//        Energy_Transfer_DIS_Int_Stack_1n1p->Add(E_Trans45_DIS_1n1p);
//        E_Trans45_DIS_1n1p->SetStats(0);
//        E_Trans45_DIS_1n1p->SetLineColor(kMagenta);
//        E_Trans45_DIS_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (DIS only) ----------------
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (DIS only, 2p)">
//        E_Trans90_DIS_2p->Draw();
//        plots->Add(E_Trans90_DIS_2p);
//        E_Trans90_DIS_2p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_90_Deg_DIS_only_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_90_Deg_DIS_only_normal_scale_2p.png");
//        E_Trans90_DIS_2p->SetLineColor(kGreen);
//        Energy_Transfer_DIS_Int_Stack_2p->Add(E_Trans90_DIS_2p);
//        E_Trans90_DIS_2p->SetStats(0);
//        E_Trans90_DIS_2p->SetLineColor(kMagenta);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El in the angle range 89[Deg] <= theta_{l} <= 91[Deg] (DIS only, 1n1p)">
//        E_Trans90_DIS_1n1p->Draw();
//        plots->Add(E_Trans90_DIS_1n1p);
//        E_Trans90_DIS_1n1p->SetLineWidth(2);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_90_Deg_DIS_only_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_around_90_Deg_DIS_only_normal_scale_1n1p.png");
//        E_Trans90_DIS_1n1p->SetLineColor(kGreen);
//        Energy_Transfer_DIS_Int_Stack_1n1p->Add(E_Trans90_DIS_1n1p);
//        E_Trans90_DIS_1n1p->SetStats(0);
//        E_Trans90_DIS_1n1p->SetLineColor(kMagenta);
//        E_Trans90_DIS_1n1p->SetLineStyle(2);
//        c1->Clear();
//        //</editor-fold>
//
//
////  Energy transfer Ev-El stack of angles (DIS only) -------------------------------------------------------------
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (DIS only, 2p)">
//        Energy_Transfer_DIS_Int_Stack_2p->Draw("nostack");
//        Energy_Transfer_DIS_Int_Stack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_DIS_Int_Stack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_DIS_Int_Stack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_DIS_int_2p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_DIS_int_2p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_DIS_int_entry_2p = E_Trans_legend_DIS_int_2p->AddEntry(E_Trans15_DIS_2p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_DIS_int_entry_2p = E_Trans_legend_DIS_int_2p->AddEntry(E_Trans45_DIS_2p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_DIS_int_entry_2p = E_Trans_legend_DIS_int_2p->AddEntry(E_Trans90_DIS_2p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//
//        E_Trans_legend_DIS_int_2p->Draw();
//
//        plots->Add(Energy_Transfer_DIS_Int_Stack_2p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_DIS_only_Stack_log_scale_2p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_DIS_only_Stack_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Energy transfer Ev-El stack of angles (DIS only, 1n1p)">
//        Energy_Transfer_DIS_Int_Stack_1n1p->Draw("nostack");
//        Energy_Transfer_DIS_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_DIS_Int_Stack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_DIS_Int_Stack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_Trans_legend_DIS_int_1n1p = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_Trans_legend_DIS_int_1n1p = new TLegend(0.65, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_Trans15_DIS_int_entry_1n1p = E_Trans_legend_DIS_int_1n1p->AddEntry(E_Trans15_DIS_1n1p, "14 [Deg] <= #theta_{l} <= 16 [Deg]", "l");
//        TLegendEntry *E_Trans45_DIS_int_entry_1n1p = E_Trans_legend_DIS_int_1n1p->AddEntry(E_Trans45_DIS_1n1p, "44 [Deg] <= #theta_{l} <= 46 [Deg]", "l");
//        TLegendEntry *E_Trans90_DIS_int_entry_1n1p = E_Trans_legend_DIS_int_1n1p->AddEntry(E_Trans90_DIS_1n1p, "89 [Deg] <= #theta_{l} <= 91 [Deg]", "l");
//
//        E_Trans_legend_DIS_int_1n1p->Draw();
//
//        plots->Add(Energy_Transfer_DIS_Int_Stack_1n1p);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_DIS_only_Stack_log_scale_1n1p.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histograms_DIS_only/Energy_transfer_Ev-El_DIS_only_Stack_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// Energy transfer stacks (around 15, 45 and 90 + all interactions) - histogram for every angle
// ====================================================================================================

//    if (ET_all_plots && ET_QEL_plots && ET_MEC_plots && ET_RES_plots && ET_DIS_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting energy transfer stacks...\n";
//        cout << "\n";
//
//
////  Energy transfer around 15 deg stack (2p only) ------------------------------------------------------
//
//        //<editor-fold desc="Energy transfer around 15 deg stack (2p only)">
//        Energy_Transfer_all_int_15_Stack_2p->Draw("nostack");
//        Energy_Transfer_all_int_15_Stack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_all_int_15_Stack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_all_int_15_Stack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        if (normalized_E_Trans15_plots) {
//            Energy_Transfer_all_int_15_Stack_2p->SetTitle(
//                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (2p) - Normalized");
//            Energy_Transfer_all_int_15_Stack_2p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            Energy_Transfer_all_int_15_Stack_2p->GetYaxis()->SetTitle("Probability (%)");
//            Energy_Transfer_all_int_15_Stack_2p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        } else {
//            Energy_Transfer_all_int_15_Stack_2p->GetYaxis()->SetTitle("Arbitrary units");
//            Energy_Transfer_all_int_15_Stack_2p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        }
//
//
//        auto E_Trans_15_legend_2p = new
//                TLegend(0.625, 0.625, 0.9, 0.9);
////        auto E_Trans_15_legend_2p = new TLegend(0.65, 0.65, 0.9, 0.9);
//
//        TLegendEntry *E_Trans_15_all_entry_2p_stack = E_Trans_15_legend_2p->AddEntry(E_Trans15_all_2p, "All interactions", "l");
//        TLegendEntry *E_Trans_15_QEL_entry_2p_stack = E_Trans_15_legend_2p->AddEntry(E_Trans15_QEL_2p, "QEL", "l");
//        TLegendEntry *E_Trans_15_MEC_entry_2p_stack = E_Trans_15_legend_2p->AddEntry(E_Trans15_MEC_2p, "MEC", "l");
//        TLegendEntry *E_Trans_15_RES_entry_2p_stack = E_Trans_15_legend_2p->AddEntry(E_Trans15_RES_2p, "RES", "l");
//        TLegendEntry *E_Trans_15_DIS_entry_2p_stack = E_Trans_15_legend_2p->AddEntry(E_Trans15_DIS_2p, "DIS", "l");
//        E_Trans_15_legend_2p->Draw();
//
//        plots->Add(Energy_Transfer_all_int_15_Stack_2p);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histogram_15_Stack_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
////  Energy transfer around 15 deg stack (1n1p only) ----------------------------------------------------
//
//        //<editor-fold desc="Energy transfer around 15 deg stack (1n1p only)">
//        Energy_Transfer_all_int_15_Stack_1n1p->Draw("nostack");
//        Energy_Transfer_all_int_15_Stack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_all_int_15_Stack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_all_int_15_Stack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_all_int_15_Stack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        if (normalized_E_Trans15_plots) {
//            Energy_Transfer_all_int_15_Stack_1n1p->SetTitle(
//                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (1n1p) - Normalized");
//            Energy_Transfer_all_int_15_Stack_1n1p->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
//            Energy_Transfer_all_int_15_Stack_1n1p->GetYaxis()->SetTitle("Probability (%)");
//            Energy_Transfer_all_int_15_Stack_1n1p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        } else {
//            Energy_Transfer_all_int_15_Stack_1n1p->GetYaxis()->SetTitle("Arbitrary units");
//            Energy_Transfer_all_int_15_Stack_1n1p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        }
//
//
//        auto E_Trans_15_legend_1n1p = new
//                TLegend(0.625, 0.625, 0.9, 0.9);
////        auto E_Trans_15_legend_1n1p = new TLegend(0.65, 0.65, 0.9, 0.9);
//
//        TLegendEntry *E_Trans_15_all_entry_1n1p_stack = E_Trans_15_legend_1n1p->AddEntry(E_Trans15_all_1n1p, "All interactions", "l");
//        TLegendEntry *E_Trans_15_QEL_entry_1n1p_stack = E_Trans_15_legend_1n1p->AddEntry(E_Trans15_QEL_1n1p, "QEL", "l");
//        TLegendEntry *E_Trans_15_MEC_entry_1n1p_stack = E_Trans_15_legend_1n1p->AddEntry(E_Trans15_MEC_1n1p, "MEC", "l");
//        TLegendEntry *E_Trans_15_RES_entry_1n1p_stack = E_Trans_15_legend_1n1p->AddEntry(E_Trans15_RES_1n1p, "RES", "l");
//        TLegendEntry *E_Trans_15_DIS_entry_1n1p_stack = E_Trans_15_legend_1n1p->AddEntry(E_Trans15_DIS_1n1p, "DIS", "l");
//
//        E_Trans_15_legend_1n1p->Draw();
//
//        plots->Add(Energy_Transfer_all_int_15_Stack_1n1p);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_histogram_15_Stack_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// E_cal restorations
// ====================================================================================================

//    if (E_cal_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting E_cal restoration histograms...\n";
//        cout << "\n";
//
////  E_cal_QEL restoration ------------------------------------------------------------------------------
//
//        //<editor-fold desc="E_cal_QEL restoration (2p & 1n1p)">
//        double E_cal_QEL_integral = E_cal_QEL_2p->Integral() + E_cal_QEL_1n1p->Integral();
//
//        histPlotter1D(c1, E_cal_QEL_2p, normalized_E_cal_plots, true, E_cal_QEL_integral, "E_{cal} Histogram", "QEL Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_QEL_Stack, "E_cal_restoration_QEL_only", "plots/E_cal_restorations/", "2p", kBlue, true, true, true);
//
//        histPlotter1D(c1, E_cal_QEL_1n1p, normalized_E_cal_plots, true, E_cal_QEL_integral, "E_{cal} Histogram", "QEL Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_QEL_Stack, "E_cal_restoration_QEL_only", "plots/E_cal_restorations/", "1n1p", kRed, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal_QEL restoration (stack)">
//        E_cal_QEL_Stack->Draw("nostack");
//        E_cal_QEL_Stack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        E_cal_QEL_Stack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        E_cal_QEL_Stack->GetHistogram()->GetXaxis()->CenterTitle(true);
//        E_cal_QEL_Stack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        if (normalized_E_l_plots) {
//            E_cal_QEL_Stack->SetTitle("E_{cal} Histogram (QEL only, 2p and 1n1p) - Normalized");
//            E_cal_QEL_Stack->GetYaxis()->SetTitle("Probability (%)");
//            E_cal_QEL_Stack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        } else {
//            E_cal_QEL_Stack->GetYaxis()->SetTitle("Arbitrary units");
//            E_cal_QEL_Stack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        }
//
//        auto E_cal_QEL_Stack_legend = new
//                TLegend(0.75, 0.775, 0.875, 0.9);
////        auto E_cal_QEL_Stack_legend = new TLegend(0.775, 0.775, 0.9, 0.9); //original
////        auto E_cal_QEL_Stack_legend = new TLegend(0.8, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_cal_QEL_Stack_legend_entry_2p =
//                E_cal_QEL_Stack_legend->AddEntry(E_cal_QEL_2p, "2p", "l");
//        TLegendEntry *E_cal_QEL_Stack_legend_entry_1n1p =
//                E_cal_QEL_Stack_legend->AddEntry(E_cal_QEL_1n1p, "1n1p", "l");
//
//        E_cal_QEL_Stack_legend->Draw();
//
//        plots->Add(E_cal_QEL_Stack);
//        c1->SaveAs("plots/E_cal_restorations/E_cal_restoration_stack_QEL_only.png");
//        c1->Clear();
//        //</editor-fold>
//
////  E_cal_MEC restoration ------------------------------------------------------------------------------
//
//        //<editor-fold desc="E_cal_MEC restoration (2p & 1n1p)">
//        double E_cal_MEC_integral = E_cal_MEC_2p->Integral() + E_cal_MEC_1n1p->Integral();
//
//        histPlotter1D(c1, E_cal_MEC_2p, normalized_E_cal_plots, true, E_cal_MEC_integral, "E_{cal} Histogram", "MEC Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_MEC_Stack, "E_cal_restoration_MEC_only", "plots/E_cal_restorations/", "2p", kBlue, true, true, true);
//
//        histPlotter1D(c1, E_cal_MEC_1n1p, normalized_E_cal_plots, true, E_cal_MEC_integral, "E_{cal} Histogram", "MEC Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_MEC_Stack, "E_cal_restoration_MEC_only", "plots/E_cal_restorations/", "1n1p", kRed, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal_MEC restoration (stack)">
//        E_cal_MEC_Stack->Draw("nostack");
//        E_cal_MEC_Stack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        E_cal_MEC_Stack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        E_cal_MEC_Stack->GetHistogram()->GetXaxis()->CenterTitle(true);
//        E_cal_MEC_Stack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        if (normalized_E_l_plots) {
//            E_cal_MEC_Stack->SetTitle("E_{cal} Histogram (MEC only, 2p and 1n1p) - Normalized");
//            E_cal_MEC_Stack->GetYaxis()->SetTitle("Probability (%)");
//            E_cal_MEC_Stack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        } else {
//            E_cal_MEC_Stack->GetYaxis()->SetTitle("Arbitrary units");
//            E_cal_MEC_Stack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
//        }
//
//        auto E_cal_MEC_Stack_legend = new
//                TLegend(0.75, 0.775, 0.875, 0.9);
////        auto E_cal_MEC_Stack_legend = new TLegend(0.775, 0.775, 0.9, 0.9); //original
////        auto E_cal_MEC_Stack_legend = new TLegend(0.8, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_cal_MEC_Stack_legend_entry_2p = E_cal_MEC_Stack_legend->AddEntry(E_cal_MEC_2p, "2p", "l");
//        TLegendEntry *E_cal_MEC_Stack_legend_entry_1n1p = E_cal_MEC_Stack_legend->AddEntry(E_cal_MEC_1n1p, "1n1p", "l");
//
//        E_cal_MEC_Stack_legend->Draw();
//
//        plots->Add(E_cal_MEC_Stack);
//        c1->SaveAs("plots/E_cal_restorations/E_cal_restoration_stack_MEC_only.png");
//        c1->Clear();
//        //</editor-fold>
//
//
////  E_cal_RES restoration ------------------------------------------------------------------------------
//
//        //<editor-fold desc="E_cal_RES restoration (2p & 1n1p)">
//        double E_cal_RES_integral = E_cal_RES_2p->Integral() + E_cal_RES_1n1p->Integral();
//
//        histPlotter1D(c1, E_cal_RES_2p, normalized_E_cal_plots, true, E_cal_RES_integral, "E_{cal} Histogram", "RES Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_RES_Stack, "E_cal_restoration_RES_only", "plots/E_cal_restorations/", "2p", kBlue, true, true, true);
//
//        histPlotter1D(c1, E_cal_RES_1n1p, normalized_E_cal_plots, true, E_cal_RES_integral, "E_{cal} Histogram", "RES Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_RES_Stack, "E_cal_restoration_RES_only", "plots/E_cal_restorations/", "1n1p", kRed, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal_RES restoration (stack)">
//        E_cal_RES_Stack->Draw("nostack");
//        E_cal_RES_Stack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        E_cal_RES_Stack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        E_cal_RES_Stack->GetHistogram()->GetXaxis()->CenterTitle(true);
//        E_cal_RES_Stack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_cal_RES_Stack_legend = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_cal_RES_Stack_legend = new TLegend(0.8, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_cal_RES_Stack_legend_entry_2p = E_cal_RES_Stack_legend->AddEntry(E_cal_RES_2p, "2p", "l");
//        TLegendEntry *E_cal_RES_Stack_legend_entry_1n1p = E_cal_RES_Stack_legend->AddEntry(E_cal_RES_1n1p, "1n1p", "l");
//
//        E_cal_RES_Stack_legend->Draw();
//
//        plots->Add(E_cal_RES_Stack);
//        c1->SaveAs("plots/E_cal_restorations/E_cal_restoration_stack_RES_only.png");
//        c1->Clear();
//        //</editor-fold>
//
//
////  E_cal_DIS restoration ------------------------------------------------------------------------------
//
//        //<editor-fold desc="E_cal_DIS restoration (2p & 1n1p)">
//        double E_cal_DIS_integral = E_cal_DIS_2p->Integral() + E_cal_DIS_1n1p->Integral();
//
//        histPlotter1D(c1, E_cal_DIS_2p, normalized_E_cal_plots, true, E_cal_DIS_integral, "E_{cal} Histogram", "DIS Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_DIS_Stack, "E_cal_restoration_DIS_only", "plots/E_cal_restorations/", "2p", kBlue, true, true, true);
//
//        histPlotter1D(c1, E_cal_DIS_1n1p, normalized_E_cal_plots, true, E_cal_DIS_integral, "E_{cal} Histogram", "DIS Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, E_cal_DIS_Stack, "E_cal_restoration_DIS_only", "plots/E_cal_restorations/", "1n1p", kRed, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal_DIS restoration (stack)">
//        E_cal_DIS_Stack->Draw("nostack");
//        E_cal_DIS_Stack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        E_cal_DIS_Stack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        E_cal_DIS_Stack->GetHistogram()->GetXaxis()->CenterTitle(true);
//        E_cal_DIS_Stack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        auto E_cal_DIS_Stack_legend = new
//                TLegend(0.775, 0.775, 0.9, 0.9);
////        auto E_cal_DIS_Stack_legend = new TLegend(0.8, 0.6, 0.9, 0.7);
//
//        TLegendEntry *E_cal_DIS_Stack_legend_entry_2p = E_cal_DIS_Stack_legend->AddEntry(E_cal_RES_2p, "2p", "l");
//        TLegendEntry *E_cal_DIS_Stack_legend_entry_1n1p = E_cal_DIS_Stack_legend->AddEntry(E_cal_RES_1n1p, "1n1p", "l");
//
//        E_cal_DIS_Stack_legend->Draw();
//
//        plots->Add(E_cal_DIS_Stack);
//        c1->SaveAs("plots/E_cal_restorations/E_cal_restoration_stack_DIS_only.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// Other E_cal plots ----------------------------------------------------------------------------------

//    if (other_E_cal_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting other E_cal restoration histograms...\n";
//        cout << "\n";
//
//        //<editor-fold desc="Other E_cal plots (all interactions, 2p)">
//        E_cal_VS_theta_l_all_int_2p->Draw("colz");
//        plots->Add(E_cal_VS_theta_l_all_int_2p);
//        E_cal_VS_theta_l_all_int_2p->SetStats(0);
//        E_cal_VS_theta_l_all_int_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_l_all_int_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_theta_l_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_theta_l_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Q2_all_int_2p->Draw("colz");
//        plots->Add(E_cal_VS_Q2_all_int_2p);
//        E_cal_VS_Q2_all_int_2p->SetStats(0);
//        E_cal_VS_Q2_all_int_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Q2_all_int_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_Q2_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_Q2_QEL_only_linear_scale_2p.png");
//        c1->Clear();
////
//        E_cal_VS_W_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_W_QEL_only_2p);
//        E_cal_VS_W_QEL_only_2p->SetStats(0);
//        E_cal_VS_W_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_W_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_W_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_W_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_En_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_En_QEL_only_2p);
//        E_cal_VS_En_QEL_only_2p->SetStats(0);
//        E_cal_VS_En_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_En_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_En_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_En_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_Pn_QEL_only_2p);
//        E_cal_VS_Pn_QEL_only_2p->SetStats(0);
//        E_cal_VS_Pn_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn1_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_Pn1_QEL_only_2p);
//        E_cal_VS_Pn1_QEL_only_2p->SetStats(0);
//        E_cal_VS_Pn1_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn1_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn1_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn1_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn2_QEL_only_2p->Draw("colz");
//        E_cal_VS_Pn2_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn2_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        plots->Add(E_cal_VS_Pn2_QEL_only_2p);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn2_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn2_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_dtheta_all_int_2p->Draw("colz");
//        E_cal_VS_dtheta_all_int_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_dtheta_all_int_2p->GetYaxis()->CenterTitle(true);
//        plots->Add(E_cal_VS_dtheta_all_int_2p);
//        E_cal_VS_dtheta_all_int_2p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_dtheta_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_dtheta_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Other E_cal plots (QEL only, 2p)">
//        E_cal_VS_theta_l_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_theta_l_QEL_only_2p);
//        E_cal_VS_theta_l_QEL_only_2p->SetStats(0);
//        E_cal_VS_theta_l_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_l_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_l_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_l_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Q2_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_Q2_QEL_only_2p);
//        E_cal_VS_Q2_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Q2_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Q2_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Q2_QEL_only_linear_scale_2p.png");
//        c1->Clear();
////
//        E_cal_VS_W_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_W_QEL_only_2p);
//        E_cal_VS_W_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_W_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_W_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_W_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_En_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_En_QEL_only_2p);
//        E_cal_VS_En_QEL_only_2p->SetStats(0);
//        E_cal_VS_En_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_En_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_En_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_En_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_Pn_QEL_only_2p);
//        E_cal_VS_Pn_QEL_only_2p->SetStats(0);
//        E_cal_VS_Pn_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn1_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_Pn1_QEL_only_2p);
//        E_cal_VS_Pn1_QEL_only_2p->SetStats(0);
//        E_cal_VS_Pn1_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn1_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn1_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn1_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn2_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_Pn2_QEL_only_2p);
//        E_cal_VS_Pn2_QEL_only_2p->SetStats(0);
//        E_cal_VS_Pn2_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn2_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn2_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_Pn2_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
////
//        E_cal_VS_theta_p1_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_theta_p1_QEL_only_2p);
//        E_cal_VS_theta_p1_QEL_only_2p->SetStats(0);
//        E_cal_VS_theta_p1_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_p1_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_p1_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_p1_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_theta_p2_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_theta_p2_QEL_only_2p);
//        E_cal_VS_theta_p2_QEL_only_2p->SetStats(0);
//        E_cal_VS_theta_p2_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_p2_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_p2_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_p2_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//
//        E_cal_VS_dtheta_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_dtheta_QEL_only_2p);
//        E_cal_VS_dtheta_QEL_only_2p->SetStats(0);
//        E_cal_VS_dtheta_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_dtheta_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_dtheta_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_dtheta_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Other E_cal plots (QEL only, 1n1p)">
//        E_cal_VS_theta_l_QEL_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_theta_l_QEL_1n1p);
//        E_cal_VS_theta_l_QEL_1n1p->SetStats(0);
//        E_cal_VS_theta_l_QEL_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_l_QEL_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_1n1p_vs_theta_l_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_1n1p_vs_theta_l_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_Q2_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_Q2_QEL_only_1n1p);
//        E_cal_VS_Q2_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_Q2_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Q2_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_Q2_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_Q2_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
////
//        E_cal_VS_W_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_W_QEL_only_1n1p);
//        E_cal_VS_W_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_W_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_W_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_W_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_W_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_En_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_En_QEL_only_1n1p);
//        E_cal_VS_En_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_En_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_En_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_En_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_En_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_Pn_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_Pn_QEL_only_1n1p);
//        E_cal_VS_Pn_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_Pn_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_Pn_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_Pn_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_Pn_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_P_p_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_P_p_QEL_only_1n1p);
//        E_cal_VS_P_p_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_P_p_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_P_p_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_P_p_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_P_p_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_P_n_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_P_n_QEL_only_1n1p);
//        E_cal_VS_P_n_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_P_n_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_P_n_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_P_n_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_P_n_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
////
//        E_cal_VS_theta_p_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_theta_p_QEL_only_1n1p);
//        E_cal_VS_theta_p_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_theta_p_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_p_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_theta_p_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_theta_p_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_theta_n_QEL_only_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_theta_n_QEL_only_1n1p);
//        E_cal_VS_theta_n_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_theta_n_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_n_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_theta_n_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_theta_n_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_cal_VS_dtheta_QEL_only_1n1p->Draw("colz");
//        E_cal_VS_dtheta_QEL_only_1n1p->SetStats(0);
//        E_cal_VS_dtheta_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_dtheta_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_dtheta_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_vs_dtheta_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }

// One commend plots ----------------------------------------------------------------------------------

//    if (other_E_cal_plots) {
//
//        //<editor-fold desc="One commend plots - Q2">
//
////  Q2 -all interactions (2p):
//
////        fChain->Draw("El + Ef[0] + Ef[1] - 2*0.938272:Q2>>h1(100, 0.0, 3.6, 100, 2.12, 2.3)", "nf==2 && nfn==0 && nfp == 2", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/E_cal_vs_Q2_all_int_2p.png");
////        c1->Clear();
//
//
////  Q2 - QEL only (2p):
//
////        fChain->Draw("El + Ef[0] + Ef[1] - 2*0.938272:Q2>>h1(100, 0.0, 3.6, 100, 2.12, 2.23)", "nf==2 && nfn==0 && nfp == 2 && qel", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/E_cal_vs_Q2_QEL_only_2p.png");
////        c1->Clear();
//
//
////  Q2 -all interactions (1n1p):
//
////        fChain->Draw("El + Ef[0] + Ef[1] - 0.938272 - 0.939565:Q2>>h1(100, 0.0, 3.6, 100, 2.12, 2.3)", "nf==2 && nfn==1 && nfp == 1", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/E_cal_vs_Q2_all_int_1n1p.png");
////        c1->Clear();
//
//
////  Q2 - QEL only (1n1p):
//
////        fChain->Draw("El + Ef[0] + Ef[1] - 0.938272 - 0.939565:Q2>>h1(100, 0.0, 3.6, 100, 2.12, 2.23)", "nf==2 && nfn==1 && nfp == 1 && qel", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/E_cal_vs_Q2_QEL_only_1n1p.png");
////        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="One commend plots - Theta_l">
//
//// Theta_l -all interactions (2p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 2*0.938272:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.3)",
////                "nf==2 && nfn==0 && nfp == 2", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/E_cal_vs_theta_l_all_int_2p.png");
////        c1->Clear();
//
//
//// Theta_l - QEL only (2p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 2*0.938272:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.23)",
////                "nf==2 && nfn==0 && nfp == 2 && qel", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/E_cal_vs_theta_l_QEL_only_2p.png");
////        c1->Clear();
//
//
//// Theta_l -all interactions (1n1p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 0.938272 - 0.939565:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.3)",
////                "nf==2 && nfn==1 && nfp == 1", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/E_cal_vs_theta_l_all_int_1n1p.png");
////        c1->Clear();
//
//
//// Theta_l - QEL only (1n1p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 0.938272 - 0.939565:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.23)",
////                "nf==2 && nfn==1 && nfp == 1 && qel", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/E_cal_vs_theta_l_QEL_only_1n1p.png");
////        c1->Clear();
//        //</editor-fold>
//
//    }


// Momentum histograms
// ====================================================================================================

//    if (momentum_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting momentum histograms for 2p and 1n1p...\n";
//        cout << "\n";
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        histPlotter1D(c1, P_L_hist_2p, normalized_P_L_plots, false, 1., "Momentum Histogram of Leading Proton P_{L} = P_{p1}", "all interactions", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, MomentumStack_2p, "P_L_histogram", "plots/momentum_histograms/2p/", "2p", kBlue, true, true, true);
//
//        histPlotter1D(c1, P_R_hist_2p, normalized_P_R_plots, false, 1., "Momentum Histogram of Recoil Proton P_{R} = P_{p2}", "all interactions", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, MomentumStack_2p, "P_R_histogram", "plots/momentum_histograms/2p/", "2p", kRed, true, true, true);
//
//        histPlotter1D(c1, P_lp_hist_2p, normalized_P_R_plots, false, 1., "Momentum Histogram of Outgoing Lepton P_{l}", "all interactions", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, MomentumStack_2p, "P_lp_histogram", "plots/momentum_histograms/2p/", "2p", kGreen, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        histPlotter1D(c1, P_p_hist_1n1p, normalized_P_L_plots, false, 1., "Momentum Histogram of Scattered Proton P_{p}", "all interactions", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, MomentumStack_1n1p, "P_p_histogram", "plots/momentum_histograms/1n1p/", "1n1p", kBlue, true, true, true);
//
//        histPlotter1D(c1, P_n_hist_1n1p, normalized_P_R_plots, false, 1., "Momentum Histogram of Scattered Neutron P_{n}", "all interactions", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, MomentumStack_1n1p, "P_n_histogram", "plots/momentum_histograms/1n1p/", "1n1p", kRed, true, true, true);
//
//        histPlotter1D(c1, P_l_hist_1n1p, normalized_P_R_plots, false, 1., "Momentum Histogram of Outgoing Lepton P_{l}", "all interactions", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, MomentumStack_1n1p, "P_l_histogram", "plots/momentum_histograms/1n1p/", "1n1p", kGreen, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histogram stack (2p)">
//        MomentumStack_2p->Draw("nostack");
//        MomentumStack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        MomentumStack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        MomentumStack_2p->GetHistogram()->GetXaxis()->CenterTitle(true);
//        MomentumStack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//        MomentumStack_2p->GetHistogram()->GetYaxis()->SetTitle("Arbitrary units");
//
//        auto MomentumStack_2p_legend = new
//                TLegend(0.775, 0.7, 0.85, 0.875);
//
//        TLegendEntry *MomentumStack_2p_legend_P_L_entry = MomentumStack_2p_legend->AddEntry(P_L_hist_2p, "P_{L}", "l");
//        TLegendEntry *MomentumStack_2p_legend_P_R_entry = MomentumStack_2p_legend->AddEntry(P_R_hist_2p, "P_{R}", "l");
//        TLegendEntry *MomentumStack_2p_legend_P_lp_entry = MomentumStack_2p_legend->AddEntry(P_lp_hist_2p, "P_{l}", "l");
//
//        MomentumStack_2p_legend->Draw();
//
//        plots->Add(MomentumStack_2p);
//        c1->SaveAs("plots/momentum_histograms/Momentum_stack_2p.png");
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histogram stack (1n1p)">`
//        MomentumStack_1n1p->Draw("nostack");
//        MomentumStack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        MomentumStack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        MomentumStack_1n1p->GetHistogram()->GetXaxis()->CenterTitle(true);
//        MomentumStack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//        MomentumStack_1n1p->GetHistogram()->GetYaxis()->SetTitle("Arbitrary units");
//
//        auto MomentumStack_1n1p_legend = new
//                TLegend(0.775, 0.7, 0.85, 0.875);
//
//        TLegendEntry *MomentumStack_1n1p_legend_P_p_entry = MomentumStack_1n1p_legend->AddEntry(P_p_hist_1n1p, "P_{p}", "l");
//        TLegendEntry *MomentumStack_1n1p_legend_P_n_entry = MomentumStack_1n1p_legend->AddEntry(P_n_hist_1n1p, "P_{n}", "l");
//        TLegendEntry *MomentumStack_1n1p_legend_P_l_entry = MomentumStack_1n1p_legend->AddEntry(P_l_hist_1n1p, "P_{l}", "l");
//
//        MomentumStack_1n1p_legend->Draw();
//
//        plots->Add(MomentumStack_1n1p);
//        c1->SaveAs("plots/momentum_histograms/Momentum_stack_1n1p.png");
//        c1->Clear();
//        //</editor-fold>
//
//    }


// MicroBooNE article histogram reconstructions
// ====================================================================================================

//    if (MicroBooNE_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting article histograms...\n";
//        cout << "\n";
//
//// Momentum plots -------------------------------------------------------------------------------------
//
//        P_L_hist->Draw();
//        plots->Add(P_L_hist);
//        P_L_hist->SetLineWidth(2);
//        P_L_hist->GetXaxis()->CenterTitle(true);
//        P_L_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_L_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_L_histogram_linear_scale.png");
//        c1->Clear();
//
//        P_R_hist->Draw();
//        plots->Add(P_R_hist);
//        P_R_hist->SetLineWidth(2);
//        P_R_hist->GetXaxis()->CenterTitle(true);
//        P_R_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_R_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_R_histogram_linear_scale.png");
//        c1->Clear();
//
//        P_l_hist->Draw();
//        plots->Add(P_l_hist);
//        P_l_hist->SetLineWidth(2);
//        P_l_hist->GetXaxis()->CenterTitle(true);
//        P_l_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_lp_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_lp_histogram_linear_scale.png");
//        c1->Clear();
//
//        P_pion_hist->Draw();
//        plots->Add(P_pion_hist);
//        P_pion_hist->SetLineWidth(2);
//        P_pion_hist->GetXaxis()->CenterTitle(true);
//        P_pion_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_pion_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_pion_histogram_linear_scale.png");
//        c1->Clear();
//
//// Unweighted plots -----------------------------------------------------------------------------
//
//        gamma_Lab_hist->Draw();
//        gamma_Lab_hist->SetTitleSize(0.06);
//        gamma_Lab_hist->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_hist->GetXaxis()->CenterTitle(true);
//        gamma_Lab_hist->GetYaxis()->SetLabelSize(0.0425);
////        gamma_Lab_hist->GetYaxis()->SetLimits(0., 3.);
//        plots->Add(gamma_Lab_hist);
//        gamma_Lab_hist->SetLineWidth(2);
//        gamma_Lab_hist->SetLineColor(kBlue);
////        gamma_Lab_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_Lab.png");
//        c1->Clear();
//
//        gamma_mu_p_tot->Draw();
//        gamma_mu_p_tot->SetTitleSize(0.06);
//        gamma_mu_p_tot->GetXaxis()->SetLabelSize(0.0425);
//        gamma_mu_p_tot->GetXaxis()->CenterTitle(true);
//        gamma_mu_p_tot->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_mu_p_tot);
//        gamma_mu_p_tot->SetLineWidth(2);
//        gamma_mu_p_tot->SetLineColor(kBlue);
////        gamma_mu_p_tot->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_mu_p_tot.png");
//        c1->Clear();
//
//        dP_T_hist->Draw();
//        dP_T_hist->SetTitleSize(0.06);
//        dP_T_hist->GetXaxis()->SetLabelSize(0.0425);
//        dP_T_hist->GetXaxis()->CenterTitle(true);
//        dP_T_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(dP_T_hist);
//        dP_T_hist->SetLineWidth(2);
//        dP_T_hist->SetLineColor(kBlue);
////        dP_T_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/dP_T_histogram.png");
//        c1->Clear();
//
//        gamma_Lab_all_hist->Draw();
//        gamma_Lab_all_hist->SetTitleSize(0.06);
//        gamma_Lab_all_hist->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_all_hist->GetXaxis()->CenterTitle(true);
//        gamma_Lab_all_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_all_hist);
//        gamma_Lab_all_hist->SetLineWidth(2);
//        gamma_Lab_all_hist->SetLineColor(kBlue);
////        gamma_Lab_all_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/all_interactions/gamma_Lab_all.png");
//        c1->Clear();
//
//        gamma_Lab_QEL_hist->Draw();
//        gamma_Lab_QEL_hist->SetTitleSize(0.06);
//        gamma_Lab_QEL_hist->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_QEL_hist->GetXaxis()->CenterTitle(true);
//        gamma_Lab_QEL_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_QEL_hist);
//        gamma_Lab_QEL_hist->SetLineWidth(2);
//        gamma_Lab_QEL_hist->SetLineColor(kBlue);
////        gamma_Lab_QEL_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/QEL_only/gamma_Lab_QEL.png");
//        c1->Clear();
//
//        gamma_Lab_MEC_hist->Draw();
//        gamma_Lab_MEC_hist->SetTitleSize(0.06);
//        gamma_Lab_MEC_hist->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_MEC_hist->GetXaxis()->CenterTitle(true);
//        gamma_Lab_MEC_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_MEC_hist);
//        gamma_Lab_MEC_hist->SetLineWidth(2);
//        gamma_Lab_MEC_hist->SetLineColor(kBlue);
////        gamma_Lab_MEC_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/MEC_only/gamma_Lab_MEC.png");
//        c1->Clear();
//
//        gamma_Lab_RES_hist->Draw();
//        gamma_Lab_RES_hist->SetTitleSize(0.06);
//        gamma_Lab_RES_hist->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_RES_hist->GetXaxis()->CenterTitle(true);
//        gamma_Lab_RES_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_RES_hist);
//        gamma_Lab_RES_hist->SetLineWidth(2);
//        gamma_Lab_RES_hist->SetLineColor(kBlue);
////        gamma_Lab_RES_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/RES_only/gamma_Lab_RES.png");
//        c1->Clear();
//
//        gamma_Lab_DIS_hist->Draw();
//        gamma_Lab_DIS_hist->SetTitleSize(0.06);
//        gamma_Lab_DIS_hist->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_DIS_hist->GetXaxis()->CenterTitle(true);
//        gamma_Lab_DIS_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_DIS_hist);
//        gamma_Lab_DIS_hist->SetLineWidth(2);
//        gamma_Lab_DIS_hist->SetLineColor(kBlue);
////        gamma_Lab_DIS_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/DIS_only/gamma_Lab_DIS.png");
//        c1->Clear();
//
//// Weighted plots -------------------------------------------------------------------------------
//
//        gamma_Lab_hist_weighted->Draw();
//        gamma_Lab_hist_weighted->Sumw2();
//        gamma_Lab_hist_weighted->SetTitleSize(0.06);
//        gamma_Lab_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_hist_weighted->GetXaxis()->CenterTitle(true);
//        gamma_Lab_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_hist_weighted);
//        gamma_Lab_hist_weighted->SetLineWidth(2);
//        gamma_Lab_hist_weighted->SetLineColor(kBlue);
//        gamma_Lab_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_Lab_weighted.png");
//        c1->Clear();
//
//        gamma_mu_p_tot_weighted->Draw();
//        gamma_mu_p_tot_weighted->Sumw2();
//        gamma_mu_p_tot_weighted->SetTitleSize(0.06);
//        gamma_mu_p_tot_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_mu_p_tot_weighted->GetXaxis()->CenterTitle(true);
//        gamma_mu_p_tot_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_mu_p_tot_weighted);
//        gamma_mu_p_tot_weighted->SetLineWidth(2);
//        gamma_mu_p_tot_weighted->SetLineColor(kBlue);
//        gamma_mu_p_tot_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_mu_p_tot_weighted.png");
//        c1->Clear();
//
//        dP_T_hist_weighted->Draw();
//        dP_T_hist_weighted->Sumw2();
//        dP_T_hist_weighted->SetTitleSize(0.06);
//        dP_T_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        dP_T_hist_weighted->GetXaxis()->CenterTitle(true);
//        dP_T_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(dP_T_hist_weighted);
//        dP_T_hist_weighted->SetLineWidth(2);
//        dP_T_hist_weighted->SetLineColor(kBlue);
//        dP_T_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/dP_T_histogram_weighted.png");
//        c1->Clear();
//
//
//        gamma_Lab_all_hist_weighted->Draw();
//        gamma_Lab_all_hist_weighted->Sumw2();
//        gamma_Lab_all_hist_weighted->SetTitleSize(0.06);
//        gamma_Lab_all_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_all_hist_weighted->GetXaxis()->CenterTitle(true);
//        gamma_Lab_all_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_all_hist_weighted);
//        gamma_Lab_all_hist_weighted->SetLineWidth(2);
//        gamma_Lab_all_hist_weighted->SetLineColor(kBlue);
////        gamma_Lab_all_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/all_interactions/gamma_Lab_all_weighted.png");
//        c1->Clear();
//
//        gamma_Lab_QEL_hist_weighted->Draw();
//        gamma_Lab_QEL_hist_weighted->Sumw2();
//        gamma_Lab_QEL_hist_weighted->SetTitleSize(0.06);
//        gamma_Lab_QEL_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_QEL_hist_weighted->GetXaxis()->CenterTitle(true);
//        gamma_Lab_QEL_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_QEL_hist_weighted);
//        gamma_Lab_QEL_hist_weighted->SetLineWidth(2);
//        gamma_Lab_QEL_hist_weighted->SetLineColor(kBlue);
////        gamma_Lab_QEL_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/QEL_only/gamma_Lab_QEL_weighted.png");
//        c1->Clear();
//
//        gamma_Lab_MEC_hist_weighted->Draw();
//        gamma_Lab_MEC_hist_weighted->Sumw2();
//        gamma_Lab_MEC_hist_weighted->SetTitleSize(0.06);
//        gamma_Lab_MEC_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_MEC_hist_weighted->GetXaxis()->CenterTitle(true);
//        gamma_Lab_MEC_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_MEC_hist_weighted);
//        gamma_Lab_MEC_hist_weighted->SetLineWidth(2);
//        gamma_Lab_MEC_hist_weighted->SetLineColor(kBlue);
////        gamma_Lab_MEC_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/MEC_only/gamma_Lab_MEC_weighted.png");
//        c1->Clear();
//
//        gamma_Lab_RES_hist_weighted->Draw();
//        gamma_Lab_RES_hist_weighted->Sumw2();
//        gamma_Lab_RES_hist_weighted->SetTitleSize(0.06);
//        gamma_Lab_RES_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_RES_hist_weighted->GetXaxis()->CenterTitle(true);
//        gamma_Lab_RES_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_RES_hist_weighted);
//        gamma_Lab_RES_hist_weighted->SetLineWidth(2);
//        gamma_Lab_RES_hist_weighted->SetLineColor(kBlue);
////        gamma_Lab_RES_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/RES_only/gamma_Lab_RES_weighted.png");
//        c1->Clear();
//
//        gamma_Lab_DIS_hist_weighted->Draw();
//        gamma_Lab_DIS_hist_weighted->Sumw2();
//        gamma_Lab_DIS_hist_weighted->SetTitleSize(0.06);
//        gamma_Lab_DIS_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        gamma_Lab_DIS_hist_weighted->GetXaxis()->CenterTitle(true);
//        gamma_Lab_DIS_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(gamma_Lab_DIS_hist_weighted);
//        gamma_Lab_DIS_hist_weighted->SetLineWidth(2);
//        gamma_Lab_DIS_hist_weighted->SetLineColor(kBlue);
////        gamma_Lab_DIS_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/DIS_only/gamma_Lab_DIS_weighted.png");
//        c1->Clear();
//
//    }


// Inclusive Energy transfer histograms
// ====================================================================================================

//    if (inclusive_plots) {
//
//        cout << "\n";
//        cout << "\n";
//        cout << "Plotting inclusive histograms...\n";
//        cout << "\n";
//
////      Q2 histogram (inclusive) -----------------------------------------------------
//
//        Q2_hist_inclusive->Draw();
//        plots->Add(Q2_hist_inclusive);
//        Q2_hist_inclusive->SetLineWidth(2);
//        Q2_hist_inclusive->GetXaxis()->CenterTitle(true);
//        Q2_hist_inclusive->SetLineColor(kBlue);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Q2_inclusive.png");
//        Q2_hist_inclusive->SetLineWidth(4);
//        Q2_hist_inclusive->SetLineStyle(2);
////        Q2_hist_inclusive->SetLineStyle(5);
//        Q2_hist_inclusive->SetStats(0);
//        c1->Clear();
//
//
////     all interactions (ET around theta_l = 15) -----------------------------------------------------
//
////      Normalization factor (equals to max(E_Trans15_all_inclusive)):
//        auto factor_E_Trans_inclusive = E_Trans15_all_inclusive->GetMaximum();
//
//        if (normalized_inclusive_plots) {
//            E_Trans15_all_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
//            E_Trans15_all_inclusive->Draw("HIST");
//        } else {
//            E_Trans15_all_inclusive->Draw();
//        }
//
//        plots->Add(E_Trans15_all_inclusive);
//        E_Trans15_all_inclusive->SetLineWidth(2);
//        E_Trans15_all_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans15_all_inclusive->SetLineColor(kBlack);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_all_interactions_liner_scale_inclusive.png");
//        E_Trans15_all_inclusive->SetLineWidth(4);
//        E_Trans15_all_inclusive->SetLineStyle(2);
////        E_Trans15_all_inclusive->SetLineStyle(5);
//        E_Trans15_all_inclusive->SetStats(0);
//        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_all_inclusive);
//        c1->Clear();
//
//
////      QEL only (ET around theta_l = 15) -------------------------------------------------------------
//
//        if (normalized_inclusive_plots) {
////        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
//            E_Trans15_QEL_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
//            E_Trans15_QEL_inclusive->Draw("HIST");
//        } else {
//            E_Trans15_QEL_inclusive->Draw();
//        }
//
//        plots->Add(E_Trans15_QEL_inclusive);
//        E_Trans15_QEL_inclusive->SetLineWidth(2);
//        E_Trans15_QEL_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans15_QEL_inclusive->SetLineColor(kBlue);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_QEL_only_liner_scale_inclusive.png");
//        E_Trans15_QEL_inclusive->SetStats(0);
//        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_QEL_inclusive);
//        c1->Clear();
//
//
////      MEC only (ET around theta_l = 15) -------------------------------------------------------------
//
//        if (normalized_inclusive_plots) {
////        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
//            E_Trans15_MEC_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
//            E_Trans15_MEC_inclusive->Draw("HIST");
//        } else {
//            E_Trans15_MEC_inclusive->Draw();
//        }
//        plots->Add(E_Trans15_MEC_inclusive);
//        E_Trans15_MEC_inclusive->SetLineWidth(2);
//        E_Trans15_MEC_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans15_MEC_inclusive->SetLineColor(kRed);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_MEC_only_liner_scale_inclusive.png");
//        E_Trans15_MEC_inclusive->SetStats(0);
//        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_MEC_inclusive);
//        c1->Clear();
//
//
////      RES only (ET around theta_l = 15) -------------------------------------------------------------
//
//        if (normalized_inclusive_plots) {
////        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
//            E_Trans15_RES_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
//            E_Trans15_RES_inclusive->Draw("HIST");
//        } else {
//            E_Trans15_RES_inclusive->Draw();
//        }
//        plots->Add(E_Trans15_RES_inclusive);
//        E_Trans15_RES_inclusive->SetLineWidth(2);
//        E_Trans15_RES_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans15_RES_inclusive->SetLineColor(kGreen);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_RES_only_liner_scale_inclusive.png");
//        E_Trans15_RES_inclusive->SetStats(0);
//        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_RES_inclusive);
//        c1->Clear();
//
//
////      DIS only (ET around theta_l = 15) -------------------------------------------------------------
//
//        if (normalized_inclusive_plots) {
////        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
//            E_Trans15_DIS_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
//            E_Trans15_DIS_inclusive->Draw("HIST");
//        } else {
//            E_Trans15_DIS_inclusive->Draw();
//        }
//        plots->Add(E_Trans15_DIS_inclusive);
//        E_Trans15_DIS_inclusive->SetLineWidth(2);
//        E_Trans15_DIS_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans15_DIS_inclusive->SetLineColor(kCyan);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_DIS_only_liner_scale_inclusive.png");
//        E_Trans15_DIS_inclusive->SetStats(0);
//        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_DIS_inclusive);
//        c1->Clear();
//
//
////      Inclusive ET stack (around theta_l = 15) ------------------------------------------------------
//
//        Energy_Transfer_all_int_15_inclusive_Stack->Draw("nostack");
//        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
//        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
//        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetXaxis()->CenterTitle(true);
//        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
//
//        if (normalized_inclusive_plots) {
//            Energy_Transfer_all_int_15_inclusive_Stack->SetTitle(
//                    "Energy Transfer (^{12}C(e,e')) in range 14#circ #leq #theta_{l} #leq 16#circ - Normalized");
//        } else {
//            Energy_Transfer_all_int_15_inclusive_Stack->SetTitle(
//                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (2p and 1n1p, ^{12}C(e,e'))");
//        }
//
//        auto E_Trans_15_inclusive_legend = new
//                TLegend(0.625, 0.625, 0.9, 0.9);
//
//        TLegendEntry *E_Trans_15_all_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_all_inclusive, "All interactions", "l");
//        TLegendEntry *E_Trans_15_QEL_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_QEL_inclusive, "QEL only", "l");
//        TLegendEntry *E_Trans_15_MEC_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_MEC_inclusive, "MEC only", "l");
//        TLegendEntry *E_Trans_15_RES_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_RES_inclusive, "RES only", "l");
//        TLegendEntry *E_Trans_15_DIS_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_DIS_inclusive, "DIS only ", "l");
//
//        E_Trans_15_inclusive_legend->Draw();
//
//        plots->Add(Energy_Transfer_all_int_15_inclusive_Stack);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_histogram_15_Stack_liner_scale_inclusive.png");
//        c1->Clear();
//
//
//////      Normalization factor (equals to max(E_Trans_VS_q_all_inclusive)):
////        auto E_Trans_VS_q_all_inclusive = E_Trans_VS_q_all_inclusive->GetMaximum();
//
////      Normalization factor (equals to max(E_Trans_VS_q_all_inclusive)):
//        double factor_E_Trans_VS_q_all_inclusive = 1.;
//
//        if (normalized_E_l_plots) {
//            E_Trans_VS_q_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (All Interactions, ^{12}C(e,e')) - Normalized");
//            E_Trans_VS_q_all_inclusive->Scale(factor_E_Trans_VS_q_all_inclusive / E_Trans_VS_q_all_inclusive->Integral(), "width");
//            E_Trans_VS_q_all_inclusive->Draw("colz");
//            E_Trans_VS_q_all_inclusive->SetMaximum(1.5);
//        } else {
//            E_Trans_VS_q_all_inclusive->Draw("colz");
//        }
//
//        E_Trans_VS_q_all_inclusive->SetTitleSize(0.06, "xyz");
//        E_Trans_VS_q_all_inclusive->GetXaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_all_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q_all_inclusive->GetYaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_all_inclusive->GetYaxis()->CenterTitle(true);
//        E_Trans_VS_q_all_inclusive->GetZaxis()->SetLabelSize(0.0425);
////        E_Trans_VS_q_all_inclusive->Draw("colz");
//        plots->Add(E_Trans_VS_q_all_inclusive);
//        E_Trans_VS_q_all_inclusive->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_all_inclusive_log_scale.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_all_inclusive_liner_scale.png");
//        c1->Clear();
//
////      Normalization factor (equals to max(fsEl_VS_theta_l_all_int_2p)):
//        double factor_E_Trans_VS_q_QEL_inclusive = 1.;
//
//        if (normalized_E_l_plots) {
//            E_Trans_VS_q_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (QEL Only, ^{12}C(e,e')) - Normalized");
//            E_Trans_VS_q_QEL_inclusive->Scale(factor_E_Trans_VS_q_QEL_inclusive / E_Trans_VS_q_QEL_inclusive->Integral(), "width");
//            E_Trans_VS_q_QEL_inclusive->Draw("colz");
//            E_Trans_VS_q_QEL_inclusive->SetMaximum(1.5);
//        } else {
//            E_Trans_VS_q_QEL_inclusive->Draw("colz");
//        }
//
//        E_Trans_VS_q_QEL_inclusive->SetTitleSize(0.06, "xyz");
//        E_Trans_VS_q_QEL_inclusive->GetXaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_QEL_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q_QEL_inclusive->GetYaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_QEL_inclusive->GetYaxis()->CenterTitle(true);
//        E_Trans_VS_q_QEL_inclusive->GetZaxis()->SetLabelSize(0.0425);
//
////        E_Trans_VS_q_QEL_inclusive->Draw("colz");
//        plots->Add(E_Trans_VS_q_QEL_inclusive);
//        E_Trans_VS_q_QEL_inclusive->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_QEL_inclusive_log_scale.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_QEL_inclusive_liner_scale.png");
//        c1->Clear();
//
////      Normalization factor (equals to max(E_Trans_VS_q_MEC_inclusive)):
//        double factor_E_Trans_VS_q_MEC_inclusive = 1.;
//
//        if (normalized_E_l_plots) {
//            E_Trans_VS_q_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (MEC Only, ^{12}C(e,e')) - Normalized");
//            E_Trans_VS_q_MEC_inclusive->Scale(factor_E_Trans_VS_q_MEC_inclusive / E_Trans_VS_q_MEC_inclusive->Integral(), "width");
//            E_Trans_VS_q_MEC_inclusive->Draw("colz");
//            E_Trans_VS_q_MEC_inclusive->SetMaximum(1.5);
//        } else {
//            E_Trans_VS_q_MEC_inclusive->Draw("colz");
//        }
//
//        E_Trans_VS_q_MEC_inclusive->SetTitleSize(0.06, "xyz");
//        E_Trans_VS_q_MEC_inclusive->GetXaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_MEC_inclusive->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q_MEC_inclusive->GetYaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_MEC_inclusive->GetYaxis()->CenterTitle(true);
//        E_Trans_VS_q_MEC_inclusive->GetZaxis()->SetLabelSize(0.0425);
////        E_Trans_VS_q_MEC_inclusive->Draw("colz");
//        plots->Add(E_Trans_VS_q_MEC_inclusive);
//        E_Trans_VS_q_MEC_inclusive->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_MEC_inclusive_log_scale.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_MEC_inclusive_liner_scale.png");
//        c1->Clear();
//
//// 2p: MOVE TO ET
//
//        E_Trans_VS_q3_all_2p->Draw("colz");
//        plots->Add(E_Trans_VS_q3_all_2p);
//        E_Trans_VS_q3_all_2p->SetStats(0);
//        E_Trans_VS_q3_all_2p->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q3_all_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_linear_scale_2p.png");
//        c1->Clear();
//
//        E_Trans_VS_q3_QEL_2p->Draw("colz");
//        plots->Add(E_Trans_VS_q3_QEL_2p);
//        E_Trans_VS_q3_QEL_2p->SetStats(0);
//        E_Trans_VS_q3_QEL_2p->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q3_QEL_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_linear_scale_2p.png");
//        c1->Clear();
//
//        E_Trans_VS_q3_MEC_2p->Draw("colz");
//        plots->Add(E_Trans_VS_q3_MEC_2p);
//        E_Trans_VS_q3_MEC_2p->SetStats(0);
//        E_Trans_VS_q3_MEC_2p->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q3_MEC_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_linear_scale_2p.png");
//        c1->Clear();
//
//// 1n1p: MOVE TO ET
//
//        E_Trans_VS_q3_all_1n1p->Draw("colz");
//        plots->Add(E_Trans_VS_q3_all_1n1p);
//        E_Trans_VS_q3_all_1n1p->SetStats(0);
//        E_Trans_VS_q3_all_1n1p->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q3_all_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_Trans_VS_q3_QEL_1n1p->Draw("colz");
//        plots->Add(E_Trans_VS_q3_QEL_1n1p);
//        E_Trans_VS_q3_QEL_1n1p->SetStats(0);
//        E_Trans_VS_q3_QEL_1n1p->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q3_QEL_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_linear_scale_1n1p.png");
//        c1->Clear();
//
//        E_Trans_VS_q3_MEC_1n1p->Draw("colz");
//        plots->Add(E_Trans_VS_q3_MEC_1n1p);
//        E_Trans_VS_q3_MEC_1n1p->SetStats(0);
//        E_Trans_VS_q3_MEC_1n1p->GetXaxis()->CenterTitle(true);
//        E_Trans_VS_q3_MEC_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_linear_scale_1n1p.png");
//        c1->Clear();
//
//    }
        //</editor-fold>.q

 */

// Saving histogram list and finishing execution
// =======================================================================================================================================================================

    //<editor-fold desc="Saving histogram list and finishing execution">
    cout << "\n";
    cout << "\n";
    cout << "Saving histogram list...\n";
    cout << "\n";

    TFile *fout = new
            TFile(TListName, "recreate");
    fout->cd();
    plots->Write();
    fout->Write();
    fout->Close();

    cout << "\n";
    cout << "\n";
    cout << "===========================================================================\n";
    cout << "\t\t\t\tExecution summary\n";
    cout << "===========================================================================\n";
    cout << "\n";

    cout << "#(2p) events:\t" << num_of_2p_events << "\n";
    cout << "#(1n1p) events:\t" << num_of_1n1p_events << "\n";

    if (FSI_status == false) {
        cout << "FSI status:\tOFF (ni = " << ni_selection << ")\n";
    } else if (FSI_status == true) {
        cout << "FSI status:\tON\n";
    }

    cout << "File input:\t" << LoadedInput << "\n";
    cout << "Settings mode:\t'" << file_name << "'\n";
    cout << "\n";

    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n";
    cout << "\n";
    //</editor-fold>

}

#pragma clang diagnostic pop