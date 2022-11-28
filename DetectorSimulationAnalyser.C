
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

/* Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/
*/

//void Analyser() {
void EventAnalyser(int NumberOfProtons, int NumberOfNeutrons) {

    string AnalyserVersion = "3.1b";

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
    string filePath = LoadedInput.substr(0, LoadedInput.find_last_of("/") + 1);
    string fileInput = LoadedInput.substr(LoadedInput.find_last_of("/") + 1);
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
    myLogFile << "lowest_nentries = " << BoolToString(lowest_nentries) << "\n";
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
    THStack *ThetaStack = new
            THStack("#theta_{l} stack (2p & 1n1p)", "#theta_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p);#theta_{l} [Deg];");

    TH1D *theta_l_2p = new
            TH1D("#theta_{l} (2p)", ";#theta_{l} [Deg];", 100, theta_l_lower_lim_2p, theta_l_upper_lim_2p);
    TH1D *theta_p1_2p = new
            TH1D("#theta_{p1} (2p)", ";#theta_{p1} [Deg];", 100, theta_p1_lower_lim_2p, theta_p1_upper_lim_2p);
    TH1D *theta_p2_2p = new
            TH1D("#theta_{p2} (2p)", ";#theta_{p2} [Deg];", 100, theta_p2_lower_lim_2p, theta_p2_upper_lim_2p);
    TH1D *dtheta_2p = new
            TH1D("#gamma (2p)", ";#gamma_{Lab} = #theta_{p1} - #theta_{p2} [Deg];", 100, dtheta_lower_lim_2p, dtheta_upper_lim_2p);

    TH1D *theta_l_1n1p = new
            TH1D("#theta_{l} (1n1p)", ";#theta_{l} [Deg];", 100, theta_l_lower_lim_1n1p, theta_l_upper_lim_1n1p);
    TH1D *theta_p_1n1p = new
            TH1D("#theta_{p} (1n1p)", ";#theta_{p} [Deg];", 100, theta_p_lower_lim_1n1p, theta_p_upper_lim_1n1p);
    TH1D *theta_n_1n1p = new
            TH1D("#theta_{n} (1n1p)", ";#theta_{n} [Deg];", 100, theta_n_lower_lim_1n1p, theta_n_upper_lim_1n1p);
    TH1D *dtheta_1n1p = new
            TH1D("#gamma (1n1p)", ";#gamma_{Lab} = #theta_{p} - #theta_{n} [Deg];", 100, dtheta_lower_lim_1n1p, dtheta_upper_lim_1n1p);
    //</editor-fold>

// Phi histograms -------------------------------------------------------------------------------------

    //<editor-fold desc="Phi histograms">
    THStack *PhiStack = new
            THStack("#phi_{l} stack (2p & 1n1p)", "#phi_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p);#phi_{l} [Deg];");

    TH1D *phi_l_2p = new
            TH1D("#phi_{l} (2p)", ";#phi_{l} [Deg];", 100, phi_l_lower_lim_2p, phi_l_upper_lim_2p);
    TH1D *phi_p1_2p = new
            TH1D("#phi_{p1} (2p)", ";#phi_{p1} [Deg];", 100, phi_p1_lower_lim_2p, phi_p1_upper_lim_2p);
    TH1D *phi_p2_2p = new
            TH1D("#phi_{p2} (2p)", ";#phi_{p2} [Deg];", 100, phi_p2_lower_lim_2p, phi_p2_upper_lim_2p);
    TH1D *dphi_2p = new
            TH1D("#Delta#phi (2p)", ";#Delta#phi} [Deg];", 100, dphi_lower_lim_2p, dphi_upper_lim_2p);

    TH1D *phi_l_1n1p = new
            TH1D("#phi_{l} (1n1p)", ";#phi_{l} [Deg];", 100, phi_l_lower_lim_1n1p, phi_l_upper_lim_1n1p);
    TH1D *phi_p_1n1p = new
            TH1D("#phi_{p} (1n1p)", ";#phi_{p1} [Deg];", 100, phi_p_lower_lim_1n1p, phi_p_upper_lim_1n1p);
    TH1D *phi_n_1n1p = new
            TH1D("#phi_{n} (1n1p)", ";#phi_{p2} [Deg];", 100, phi_n_lower_lim_1n1p, phi_n_upper_lim_1n1p);
    TH1D *dphi_1n1p = new
            TH1D("#Delta#phi (1n1p)", ";#Delta#phi} [Deg];", 100, dphi_lower_lim_1n1p, dphi_upper_lim_1n1p);
    //</editor-fold>

// Energy histograms ----------------------------------------------------------------------------------

    //<editor-fold desc="Energy histograms">
    THStack *EnergyStack = new
            THStack("E_{l} stack (2p & 1n1p)", "Final State E_{l} (All Interactions, 2p and 1n1p);E_{l} [GeV]");

    TH1D *fsEl_2p = new
            TH1D("Final State E_{l} (2p)", ";E_{l} [GeV]", 100, fsEl_lower_lim_2p, fsEl_upper_lim_2p);
    TH1D *fsEl_QEL_2p = new
            TH1D("Final State E_{l} (QEL Only, 2p)", ";E_{l} [GeV]", 100, fsEl_QEL_lower_lim_2p, fsEl_QEL_upper_lim_2p);
    TH1D *fsEl_MEC_2p = new
            TH1D("Final State E_{l} (MEC Only, 2p)", ";E_{l} [GeV]", 100, fsEl_MEC_lower_lim_2p, fsEl_MEC_upper_lim_2p);
    TH1D *fsEl_RES_2p = new
            TH1D("Final State E_{l} (RES Only, 2p)", ";E_{l} [GeV]", 100, fsEl_RES_lower_lim_2p, fsEl_RES_upper_lim_2p);
    TH1D *fsEl_DIS_2p = new
            TH1D("Final State E_{l} (DIS Only, 2p)", ";E_{l} [GeV]", 100, fsEl_DIS_lower_lim_2p, fsEl_DIS_upper_lim_2p);
    TH2D *fsEl_VS_theta_l_all_int_2p = new
            TH2D("Stat} (All Interactions, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_QEL_only_2p = new
            TH2D("Stat} (QEL only, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_MEC_only_2p = new
            TH2D("Stat} (MEC only, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);

    TH1D *fsEl_1n1p = new
            TH1D("Final State E_{l} (1n1p)", ";E_{l} [GeV];", 100, fsEl_lower_lim_1n1p, fsEl_upper_lim_1n1p);
    TH1D *fsEl_QEL_1n1p = new
            TH1D("Final State E_{l} (QEL Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_QEL_lower_lim_1n1p, fsEl_QEL_upper_lim_1n1p);
    TH1D *fsEl_MEC_1n1p = new
            TH1D("Final State E_{l} (MEC Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_MEC_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p);
    TH1D *fsEl_RES_1n1p = new
            TH1D("Final State E_{l} (RES Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_RES_lower_lim_1n1p, fsEl_RES_upper_lim_1n1p);
    TH1D *fsEl_DIS_1n1p = new
            TH1D("Final State E_{l} (DIS Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_DIS_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p);
    TH2D *fsEl_VS_theta_l_all_int_1n1p = new
            TH2D("Stat (All Interactions, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_QEL_only_1n1p = new
            TH2D("Stat (QEL only, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                 200, fsEl_VS_theta_l_lower_lim_1n1p_x, fsEl_VS_theta_l_upper_lim_1n1p_x,
                 200, fsEl_VS_theta_l_lower_lim_1n1p_y, fsEl_VS_theta_l_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_MEC_only_1n1p = new
            TH2D("Stat (MEC only, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
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

    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
    c1->SetGrid();

    if (NumberOfProtons == 2 && NumberOfNeutrons == 0) {
        clas12reader c12(LoadedInput.c_str()); //open file

        c12.addExactPid(2212, NumberOfProtons); //exactly 2 protons
        c12.addExactPid(11, 1); //exactly 1 electron
        c12.addZeroOfRestPid(); //nothing else

        auto *Beta_VS_P_2p = new TH2F("Beta VS P (2p)", "#beta VS P (2p); P [GeV]; #beta [Arbitrary units]", 250, 0, 6, 250, 0, 1.05);
        auto *P_histogram_2p = new TH1F("P Histogram (2p)", "P Histogram (2p)", 100, 0, 6);

        TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
        c1->SetGrid();

        while (c12.next()) { //loop over events (2p)

            auto particles_2p = c12.getDetParticles(); //particles is now a std::vector of particles for this event

            cout << "2p =======================================================================\n";

            int ProtonCounter_2p = 0, OtherParticleCounter_2p = 0;
            int Lepton_ind_2p = -1, Proton_1_ind_2p = -1, Proton_2_ind_2p = -1;

            for (int i = 0; i < particles_2p.size(); i++) {

                float particlePDG = particles_2p[i]->par()->getPid();
                float Beta = particles_2p[i]->par()->getBeta();
                float P = particles_2p[i]->par()->getP();

                Beta_VS_P_2p->Fill(P, Beta);
                P_histogram_2p->Fill(P);

//                cout << "particlePDG[" << i << "] = " << particlePDG << "\n";

                //<editor-fold desc="Proton selector (2p)">
                if (particlePDG == 2212) {
                    ++ProtonCounter_2p;
//                    cout << "particlePDG[" << i << "] = " << particlePDG << "\n";
//                    cout << "i = " << i << "\n";
                    if (ProtonCounter_2p == 1) {
                        Proton_1_ind_2p = i;
//                        cout << "Proton_1_ind_2p = " << Proton_1_ind_2p << "\n";
                    } else if (ProtonCounter_2p == 2) {
                        Proton_2_ind_2p = i;
//                        cout << "Proton_2_ind_2p = " << Proton_2_ind_2p << "\n";
                    } else if (ProtonCounter_2p > 2) {
                        cout << "\n";
                        cout << "Additional Protons detected (2p). PDG = " << particlePDG << "\n";
                        cout << "\n";
                        cout << "\n";
                    }
                } else if (particlePDG == 11) {
                    Lepton_ind_2p = i;
                    cout << "Lepton_ind_2p = " << Lepton_ind_2p << "\n";
                    cout << "particlePDG[" << i << "] = " << particlePDG << "\n";
                } else if (particlePDG != 2212) {
                    ++OtherParticleCounter_2p;
                    if (OtherParticleCounter_2p > 0) {
                        cout << "\n";
                        cout << "Additional particles detected (2p). PDG = " << particlePDG << "\n";
                        cout << "\n";
                        cout << "\n";
                    }
                }
                //</editor-fold>

            }

//          Momentum of first proton in Ef[]:
            double P_p1_2p = particles_2p[Proton_1_ind_2p]->par()->getP();

//          Momentum of second proton in Ef[]:
            double P_p2_2p = particles_2p[Proton_2_ind_2p]->par()->getP();

//          Momentum of second proton in Ef[]:
            double P_lp_2p = particles_2p[Lepton_ind_2p]->par()->getP();

//          Leading proton:
            double P_L_2p = -1;

//          Recoil proton:
            double P_R_2p = -1;

//          Momentum cut to at least 300 [MeV/c] == 0.3 [GeV/c]:
            if (P_p1_2p >= 0 && P_p2_2p >= 0) {

//              if (P_p1_2p >= P_p1_lower_lim_2p && P_p2_2p >= P_p2_lower_lim_2p) {
//                double E_cal_2p;
//
//                if (BEnergyToNucleusCon == true) {
//                    E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272) + 2 * BEnergyToNucleus;
//                } else if (BEnergyToNucleusCon == false) {
//                    E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272);
//                }

                double Theta_l_2p = ->getTheta();

                theta_l_2p->Fill(Theta_l_2p);

////                      NOT REALLY dtheta:
//                double d_theta_2p = acos(
//                        (pxf[Proton_1_ind_2p] * pxf[Proton_2_ind_2p] + pyf[Proton_1_ind_2p] * pyf[Proton_2_ind_2p] +
//                         pzf[Proton_1_ind_2p] * pzf[Proton_2_ind_2p]) /
//                        (P_p1_2p * P_p2_2p));
//                dtheta_2p->Fill(d_theta_2p * 180.0 / 3.14159265359);

//                //<editor-fold desc="P_L & P_R selector">
//                if (Ef[Proton_1_ind_2p] >= Ef[Proton_2_ind_2p]) { // If Proton_1_ind_2p is the leading proton and Proton_2_ind_2p is the recoil
//
////                          Leading proton:
//                    double P_L_2p = P_p1_2p;
//
////                          Recoil proton:
//                    double P_R_2p = P_p2_2p;
//
//                    P_L_hist_2p->Fill(P_L_2p);
//                    P_R_hist_2p->Fill(P_R_2p);
//
//                    double phi_p1 = atan2(pyf[Proton_1_ind_2p], pxf[Proton_1_ind_2p]); // Leading proton azimuthal angle in radians
//                    double phi_p2 = atan2(pyf[Proton_2_ind_2p], pxf[Proton_2_ind_2p]); // Leading proton azimuthal angle in radians
//                    double d_phi_p2 = phi_p1 - phi_p2; // In radians
//
//                    double theta_p1 = acos(pzf[Proton_1_ind_2p] / P_p1_2p); // Leading proton scattering angle in radians
//                    double theta_p2 = acos(pzf[Proton_2_ind_2p] / P_p2_2p); // Recoil proton scattering angle in radians
//
//                    phi_p1_2p->Fill(phi_p1 * 180.0 / 3.14159265359);
//                    phi_p2_2p->Fill(phi_p2 * 180.0 / 3.14159265359);
//                    dphi_2p->Fill(d_phi_p2 * 180.0 / 3.14159265359);
//
//                    theta_p1_2p->Fill(theta_p1 * 180.0 / 3.14159265359);
//                    theta_p2_2p->Fill(theta_p2 * 180.0 / 3.14159265359);
//
//                    if (qel == true) {
//                        E_cal_VS_theta_p1_QEL_only_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                        E_cal_VS_theta_p2_QEL_only_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                    }
//                } else { // If Proton_2_ind_2p is the leading proton and Proton_1_ind_2p is the recoil
//
////                          Leading proton:
//                    double P_L_2p = P_p2_2p;
//
////                          Recoil proton:
//                    double P_R_2p = P_p1_2p;
//
//                    P_L_hist_2p->Fill(P_L_2p);
//                    P_R_hist_2p->Fill(P_R_2p);
//
//                    double phi_p2 = atan2(pyf[Proton_1_ind_2p], pxf[Proton_1_ind_2p]); // Leading proton azimuthal angle in radians
//                    double phi_p1 = atan2(pyf[Proton_2_ind_2p], pxf[Proton_2_ind_2p]); // Leading proton azimuthal angle in radians
//                    double d_phi_p2 = phi_p1 - phi_p2; // In radians
//
//                    double theta_p2 = acos(pzf[Proton_1_ind_2p] / P_p1_2p); // Leading proton scattering angle in radians
//                    double theta_p1 = acos(pzf[Proton_2_ind_2p] / P_p2_2p); // Recoil proton scattering angle in radians
//
//                    phi_p2_2p->Fill(phi_p1 * 180.0 / 3.14159265359);
//                    phi_p1_2p->Fill(phi_p2 * 180.0 / 3.14159265359);
//                    dphi_2p->Fill(d_phi_p2 * 180.0 / 3.14159265359);
//
//                    theta_p1_2p->Fill(theta_p2 * 180.0 / 3.14159265359);
//                    theta_p2_2p->Fill(theta_p1 * 180.0 / 3.14159265359);
//
//                    if (qel == true) {
//                        E_cal_VS_theta_p2_QEL_only_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                        E_cal_VS_theta_p1_QEL_only_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                    }
//                }
//                //</editor-fold>

//                E_Trans_VS_q3_all_2p->Fill(q3, Ev - El);

//                P_lp_hist_2p->Fill(P_lp_2p);

//                fsEl_2p->Fill(El);
//                theta_l_2p->Fill(Theta_l_2p);
//                phi_l_2p->Fill(atan2(pyl, pxl) * 180.0 / 3.14159265359);
//                fsEl_VS_theta_l_all_int_2p->Fill(Theta_l_2p, El);

//                E_Trans_all_ang_all_int_2p->Fill(Ev - El);

//                E_cal_VS_theta_l_all_int_2p->Fill(Theta_l_2p, E_cal_2p);
//                E_cal_VS_Q2_all_int_2p->Fill(Q2, E_cal_2p);
//                E_cal_VS_dtheta_all_int_2p->Fill(fabs(acos(pzf[Proton_1_ind_2p] / rCalc(pxf[Proton_1_ind_2p], pyf[Proton_1_ind_2p], pzf[Proton_1_ind_2p])) -
//                                                      acos(pzf[Proton_2_ind_2p] / rCalc(pxf[Proton_2_ind_2p], pyf[Proton_2_ind_2p], pzf[Proton_2_ind_2p]))) *
//                                                 180.0 / 3.14159265359, E_cal_2p);

//                gamma_Lab_all_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_all_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);

//                if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                    E_Trans15_all_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                    E_Trans45_all_2p->Fill(Ev - El);
//                } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                    E_Trans90_all_2p->Fill(Ev - El);
//                }

//                //<editor-fold desc="Histogram fill by reaction (2p)">
//                if (qel == true) {
//                    gamma_Lab_QEL_hist->Fill(cos(d_theta_2p));
//                    gamma_Lab_QEL_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                    if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                        E_Trans15_QEL_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                        E_Trans45_QEL_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                        E_Trans90_QEL_2p->Fill(Ev - El);
//                    }
//
//                    fsEl_VS_theta_l_QEL_only_2p->Fill(Theta_l_2p, El);
//
//                    E_cal_QEL_2p->Fill(E_cal_2p);
//                    E_cal_VS_theta_l_QEL_only_2p->Fill(Theta_l_2p, E_cal_2p);
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
//                    if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                        E_Trans15_MEC_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                        E_Trans45_MEC_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                        E_Trans90_MEC_2p->Fill(Ev - El);
//                    }
//
//                    fsEl_VS_theta_l_MEC_only_2p->Fill(Theta_l_2p, El);
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
//                    if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                        E_Trans15_RES_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                        E_Trans45_RES_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
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
//                    if (Theta_l_2p >= 14.0 && Theta_l_2p <= 16.0) {
//                        E_Trans15_DIS_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 44.0 && Theta_l_2p <= 46.0) {
//                        E_Trans45_DIS_2p->Fill(Ev - El);
//                    } else if (Theta_l_2p >= 89.0 && Theta_l_2p <= 91.0) {
//                        E_Trans90_DIS_2p->Fill(Ev - El);
//                    }
//
//                    E_cal_DIS_2p->Fill(E_cal_2p);
//
//                    fsEl_DIS_2p->Fill(El);
//                }
//                //</editor-fold>
            }
        }


            cout << "\n";

        }

        //      Normalization factor:
        double theta_l_integral = theta_l_2p->Integral() + theta_l_1n1p->Integral();

        //<editor-fold desc="Theta of outgoing lepton histogram (2p)">
        histPlotter1D(c1, theta_l_2p, normalized_theta_l_plots, true, theta_l_integral, "#theta_{l} of Outgoing Lepton", "All Interactions",
                      0.06, 0.0425, 0.0425, plots, 2, true, true, ThetaStack, "Theta_of_lepton", "plots/theta_histograms/", "2p", kBlue, true, true, true);
        //</editor-fold>


        Beta_VS_P_2p->SetStats(0);
        Beta_VS_P_2p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/Beta_VS_P_2p.png");
        c1->Clear();

        P_histogram_2p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/P_histogram_2p.png");
        c1->Clear();

    } else if (NumberOfProtons == 1 && NumberOfNeutrons == 1) {
        clas12reader c12(LoadedInput.c_str()); //open file

        c12.addExactPid(2212, NumberOfProtons);    //exactly 1 proton
        c12.addExactPid(2112, NumberOfNeutrons);    //exactly 1 Neutron
        c12.addExactPid(11, 1);    //exactly 1 electron
        c12.addZeroOfRestPid();  //nothing else

        auto *Beta_VS_P_1n1p = new TH2F("Beta VS P (1n1p)", "#beta VS P (1n1p); P [GeV]; #beta [Arbitrary units]", 250, 0, 6, 250, 0, 1.05);
        auto *P_histogram_1n1p = new TH1F("P Histogram (1n1p)", "P Histogram (1n1p)", 100, 0, 6);

        while (c12.next()) { //loop over events (1np)

            auto particles_1n1p = c12.getDetParticles(); //particles is now a std::vector of particles for this event

            cout << "1n1p =======================================================================\n";

            for (int i = 0; i < particles_1n1p.size(); i++) {

                float particlePDG = particles_1n1p[i]->par()->getPid();
                float Beta = particles_1n1p[i]->par()->getBeta();
                float P = particles_1n1p[i]->par()->getP();

                Beta_VS_P_1n1p->Fill(P, Beta);
                P_histogram_1n1p->Fill(P);

                cout << "particlePDG[" << i << "] = " << particlePDG << "\n";

            }

            cout << "\n";

        }

        Beta_VS_P_1n1p->SetStats(0);
        Beta_VS_P_1n1p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/Beta_VS_P_1n1p.png");
        c1->Clear();

        P_histogram_1n1p->Draw("colz");
        cout << "\n";
        c1->SaveAs("./plots/P_histogram_1n1p.png");
        c1->Clear();

    }

    cout << "\n";
    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n";
    cout << "\n";

}