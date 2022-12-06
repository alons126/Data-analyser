// git pull && clas12root -b -q main.c

/*

 git pull && clas12root -b -q main.c

 * Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/

 * Copy commend (JLab -> local):
scp -r asportes@ftp.jlab.org:/u/home/asportes/Analyser/plots -J /home/alon/project/temp/
scp -r asportes@ftp.jlab.org:/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo -J /home/alon/project/temp/

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
//#include "/home/alon/clas12root/Clas12Banks/clas12reader.h"
#include "clas12reader.h"

//#include "TruthLevelAnalyser.h"
//#include "gst.h"
#include "codeSetup.h"

using namespace clas12;
using namespace std;


void EventAnalyser() {

    cout << "\n\n===========================================================================\n";
    cout << "\t\t\tDetector simulation analyser\n";
    cout << "===========================================================================\n\n";

    string AnalyserVersion = "Beta version";

//  Code settings
//  =====================================================================================================================================================================

    //<editor-fold desc="Code settings">

//  Checking directories ------------------------------------------------------------------------------------------------------------------------------------------------

//  todo: finish auto-generating directories
    //<editor-fold desc="Checking directories">
//    DirectoryChecker("/home/alon/project/plots/Beta_VS_p");
//    DirectoryChecker("/home/alon/project/plots/Chi2_plots");

//    string BetaVSPDir = "/home/alon/project/plots/Beta_VS_p";
//
//    if (IsPathExist(BetaVSPDir.c_str())) {
//        cout << BetaVSPDir << " exists!\n\n";
//    } else {
//        string commend = "mkdir " + BetaVSPDir;
//        system(commend.c_str());
//        cout << BetaVSPDir << " does not exists! directory created.\n\n";
////        fs::create_directory(BetaVSPDir.c_str());
//    }
//
//
//    string Chi2Dir = "/home/alon/project/plots/Chi2_plots";
//
//    if (IsPathExist(Chi2Dir.c_str())) {
//        cout << Chi2Dir << " exists!\n\n";
//    } else {
//        string commend = "mkdir " + Chi2Dir;
//        system(commend.c_str());
//        cout << Chi2Dir << " does not exists! directory created.\n\n";
////        fs::create_directory(Chi2Dir.c_str());
//    }

    //</editor-fold>

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
        tune = "G18";
        ni_selection = 3;
    } else if (fileInput.find("GEM21") <= fileInput[fileInput.size() - 1]) {
        tune = "GEM21 (SuSAv2)";
        ni_selection = 2;
    } else if (fileInput.find("SuSAv2") <= fileInput[fileInput.size() - 1]) {
        tune = "SuSAv2";
        ni_selection = 2;
    } else {
        ni_selection = 2;
        tune = "unknown";
    }
    //</editor-fold>

    //<editor-fold desc="Set Ev and Pv of incoming lepton">
    double Ev; // electron energy decloration
    double m_e = 0.00051099895; // electron mass in GeV

    if (fileInput == "recon_c12_6gev.hipo") {
        Ev = 5.98636;
    }

    double Pv = sqrt(Ev * Ev - m_e * m_e);
    double Pvx = 0.; // assuming momentum of incoming lepton is on the z direction
    double Pvy = 0.; // assuming momentum of incoming lepton is on the z direction
    double Pvz = Pv; // assuming momentum of incoming lepton is on the z direction
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
//    cout << "\n";
//    cout << "\n";
//    cout << "\nExecution variables\n";
    cout << "\nExecution variables\n";
    cout << "---------------------------------------------------------------------------\n";
    cout << "File input:\t" << AnalyseFile << "\n";
    cout << "Settings mode:\t'" << file_name << "'\n";
    cout << "FSI status:\t" << BoolToString(FSI_status) << "\n";
    cout << "ni:\t\t" << ni_selection << "\n";
    cout << "Tune:\t\t" << tune << "\n";
    cout << "\n";

    cout << "\nFile input\n";
    cout << "---------------------------------------------------------------------------\n";
    cout << "LoadedInput:\t" << LoadedInput << "\n";
    cout << "filePath:\t" << filePath << "\n";
    cout << "fileInput:\t" << fileInput << "\n\n\n";
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
    bool calculate_inclusive = false, calculate_2p = false, calculate_1n1p = false, calculate_MicroBooNE = false;

    bool selection_test_inclusive = false, selection_test_2p = false, selection_test_1n1p = false, selection_test_MicroBooNE = false;

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
    bool Beta_vs_P_plots = true;

    bool Chi2_plots = true;

    bool Theta_plots = true, Phi_plots = true;

    bool Energy_histogram_plots = true;

    bool ET_plots = true;
    bool ET_all_plots = true, ET_QEL_plots = true, ET_MEC_plots = true, ET_RES_plots = true, ET_DIS_plots = true;

    if (ET_plots == false) {
        bool ET_all_plots = false, ET_QEL_plots = false, ET_MEC_plots = false, ET_RES_plots = false, ET_DIS_plots = false;
    }

    bool inclusive_plots = false;

    bool E_cal_plots = true, other_E_cal_plots = false;

    bool momentum_plots = true;

    bool MicroBooNE_plots = false;
    //</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization settings">
    bool normalize_master = false;

    bool normalized_chi2_plots = false;

    bool normalized_theta_lp_plots = true;
    bool normalized_theta_p1_plots = false, normalized_theta_p2_plots = false, normalized_dtheta_2p_plots = false; // 2p
    bool normalized_theta_p_plots = false, normalized_theta_n_plots = false, normalized_dtheta_1n1p_plots = false; // 1n1p

    bool normalized_phi_lp_plots = false;
    bool normalized_phi_p1_plots = false, normalized_phi_p2_plots = false, normalized_dphi_2p_plots = false; // 2p
    bool normalized_phi_p_plots = false, normalized_phi_n_plots = false, normalized_dphi_1n1p_plots = false; // 1n1p

    bool normalized_E_lp_plots = true;
    bool normalized_E_lp_all_int_plots = true, normalized_E_lp_QEL_plots = true, normalized_E_lp_MEC_plots = true, normalized_E_lp_RES_plots = true, normalized_E_lp_DIS_plots = true;

    bool normalized_E_Trans_all_ang_all_int_plots = true;
    bool normalized_E_Trans15_plots = true;
    bool normalized_E_cal_plots = true;
    bool normalized_inclusive_plots = true;

    bool normalized_P_lp_plots = false; // 2p & 1n1p
    bool normalized_P_L_plots = false; // 2p & 1n1p
    bool normalized_P_R_plots = false; // 2p & 1n1p

    if (normalize_master == false) {
        normalized_chi2_plots = false;

        normalized_theta_lp_plots = false;

        normalized_theta_p1_plots = false;
        normalized_theta_p2_plots = false;
        normalized_dtheta_2p_plots = false; // 2p

        normalized_theta_p_plots = false;
        normalized_theta_n_plots = false;
        normalized_dtheta_1n1p_plots = false; // 1n1p

        normalized_phi_lp_plots = false;

        normalized_phi_p1_plots = false;
        normalized_phi_p2_plots = false;
        normalized_dphi_2p_plots = false; // 2p

        normalized_phi_p_plots = false;
        normalized_phi_n_plots = false;
        normalized_dphi_1n1p_plots = false; // 1n1p

        normalized_E_lp_plots = false;
        normalized_E_lp_all_int_plots = false;
        normalized_E_lp_QEL_plots = false;
        normalized_E_lp_MEC_plots = false;
        normalized_E_lp_RES_plots = false;
        normalized_E_lp_DIS_plots = false;

        normalized_E_Trans_all_ang_all_int_plots = false;
        normalized_E_Trans15_plots = false;
        normalized_E_cal_plots = false;
        normalized_inclusive_plots = false;

        normalized_P_lp_plots = false; // 2p & 1n1p
        normalized_P_L_plots = false; // 2p & 1n1p
        normalized_P_R_plots = false; // 2p & 1n1p

        cout << "\nAll normalizations are disabled.\n\n";  // and no change to custom_FSI_status
    }

    if (normalized_E_lp_plots == false) {
        normalized_E_lp_all_int_plots = false;
        normalized_E_lp_QEL_plots = false;
        normalized_E_lp_MEC_plots = false;
        normalized_E_lp_RES_plots = false;
        normalized_E_lp_DIS_plots = false;
    }
    //</editor-fold>

// Delete settings ------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Delete settings">
    bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

    //<editor-fold desc="Deleting files by cases">
    if (delete_png_files == true && delete_root_files == false) {
        cout << "\nClearing old plots...";

        system("find ./plots -type f -iname '*.png' -delete"); // Delete existing .png files

        cout << " done.\n\n";
    } else if (delete_png_files == false && delete_root_files == true) {
        cout << "\nClearing old root files...";

        system("find ./plots -type f -iname '*.root' -delete"); // Delete existing .root files

        cout << " done.\n\n";
    } else if (delete_png_files == true && delete_root_files == true) {
        cout << "\nClearing old plots & root files...";

        system("find ./plots -type f -iname '*.png' -delete"); // Delete existing .png files
        system("find ./plots -type f -iname '*.root' -delete"); // Delete existing .root files

        cout << " done.\n\n";
    } else {
        cout << "\nNo files were cleared.\n\n";
    }
    //</editor-fold>

    //</editor-fold>

// TList setup ----------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="TList name setup">
    string listName = plots_path + plotsInput + plots_file_type; //todo: add if-else to choose plotsInput or file_name
    const char *TListName = listName.c_str();
    //</editor-fold>

// Momentum thresholds --------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum thresholds">

    //<editor-fold desc="Momentum thresholds (2p)">
    double P_lp_upper_lim_2p = -1, P_lp_lower_lim_2p = -1;
    double P_p1_upper_lim_2p = -1, P_p1_lower_lim_2p = 0.3;
    double P_p2_upper_lim_2p = -1, P_p2_lower_lim_2p = 0.3;
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds (1n1p)">
    double P_lp_upper_lim_1n1p = -1, P_lp_lower_lim_1n1p = -1;
    double P_p_upper_lim_1n1p = -1, P_p_lower_lim_1n1p = 0.3;
    double P_n_upper_lim_1n1p = -1, P_n_lower_lim_1n1p = 0.3;
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds (2p, MicroBooNE)">
    double P_lp_upper_lim_MicroBooNE = 1.2, P_lp_lower_lim_MicroBooNE = 0.1;
    double P_L_upper_lim_MicroBooNE = 1.0, P_L_lower_lim_MicroBooNE = 0.3;
    double P_R_upper_lim_MicroBooNE = 1.0, P_R_lower_lim_MicroBooNE = 0.3;
    double P_pion_upper_lim_MicroBooNE = 0.065;
    //</editor-fold>

    //</editor-fold>

// Chi2 cuts ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 cuts">

    //<editor-fold desc="Electron chi2 cut">
    double Chi2_Electron_cut_CD = 1.;
    double Chi2_Electron_cut_FD = 1.;
    //</editor-fold>

    //<editor-fold desc="Proton chi2 cut">
    double Chi2_Proton_cut_CD = 1.;
    double Chi2_Proton_cut_FD = 1.;
    //</editor-fold>

    //<editor-fold desc="Neutron chi2 cut">
    double Chi2_Neutron_cut_CD = 1.;
    double Chi2_Neutron_cut_FD = 1.;
    //</editor-fold>

    //</editor-fold>

// Histogram limits ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Histogram limits">

    //<editor-fold desc="Histogram limits for every case">

    //<editor-fold desc="Theta histograms">

    //<editor-fold desc="Theta histograms (2p)">
    theta_lp_upper_lim_2p = 60;
    theta_lp_lower_lim_2p = 0;
    theta_p1_upper_lim_2p = 190;
    theta_p1_lower_lim_2p = -10;
    theta_p2_upper_lim_2p = 190;
    theta_p2_lower_lim_2p = -10;
    dtheta_upper_lim_2p = 190;
    dtheta_lower_lim_2p = -10;
    //</editor-fold>

    //<editor-fold desc="Theta histograms (1n1p)">
    theta_lp_upper_lim_1n1p = 60;
    theta_lp_lower_lim_1n1p = 0;
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
    phi_lp_upper_lim_2p = 200;
    phi_lp_lower_lim_2p = -200;
    phi_p1_upper_lim_2p = 200;
    phi_p1_lower_lim_2p = -200;
    phi_p2_upper_lim_2p = 200;
    phi_p2_lower_lim_2p = -200;
    dphi_upper_lim_2p = 360;
    dphi_lower_lim_2p = -360;
    //</editor-fold>

    //<editor-fold desc="Phi histograms (1n1p)">
    phi_lp_upper_lim_1n1p = 200;
    phi_lp_lower_lim_1n1p = -200;
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
    dP_T_hist_upper_lim = 2;
    dP_T_hist_lower_lim = 0;
    dP_T_hist_weighted_upper_lim = 2;
    dP_T_hist_weighted_lower_lim = 0;

    /*
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
     */
    //</editor-fold>

    //<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
    P_L_hist_upper_lim = 3.0;
    P_L_hist_lower_lim = 0;
    P_R_hist_upper_lim = 3.0;
    P_R_hist_lower_lim = 0;
    P_lp_hist_upper_lim = 3.0;
    P_lp_hist_lower_lim = 0;
    P_pion_hist_upper_lim = 0.1;
    P_pion_hist_lower_lim = 0;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Histogram limits by cases">

    if (file_name == "general_file") {

        //<editor-fold desc="12C_2222GeV_GTEST19_10b_00_000 histogram limits">

        //<editor-fold desc="Energy histograms">

        //<editor-fold desc="Energy histograms (2p)">
//            fsEl_upper_lim_2p = 110.;
//            fsEl_lower_lim_2p = 1;
        fsEl_upper_lim_2p = 300;
        fsEl_lower_lim_2p = -10;
//        fsEl_upper_lim_2p = 6.5;
//        fsEl_lower_lim_2p = 0.;
        fsEl_QEL_upper_lim_2p = 2.3;
        fsEl_QEL_lower_lim_2p = 0;
        fsEl_MEC_upper_lim_2p = 2.3;
        fsEl_MEC_lower_lim_2p = 0;
        fsEl_RES_upper_lim_2p = 2.3;
        fsEl_RES_lower_lim_2p = 0;
        fsEl_DIS_upper_lim_2p = 2.3;
        fsEl_DIS_lower_lim_2p = 0;
        fsEl_VS_theta_lp_upper_lim_2p_x = 70.0;
//            fsEl_VS_theta_lp_upper_lim_2p_x = 90.0;
        fsEl_VS_theta_lp_lower_lim_2p_x = 0;
        fsEl_VS_theta_lp_upper_lim_2p_y = 2.3;
        fsEl_VS_theta_lp_lower_lim_2p_y = 0;

        /*
        if (FSI_status == true) {
//            fsEl_upper_lim_2p = 110.;
//            fsEl_lower_lim_2p = 1;
            fsEl_upper_lim_2p = 6.;
            fsEl_lower_lim_2p = 0.;
            fsEl_QEL_upper_lim_2p = 2.3;
            fsEl_QEL_lower_lim_2p = 0;
            fsEl_MEC_upper_lim_2p = 2.3;
            fsEl_MEC_lower_lim_2p = 0;
            fsEl_RES_upper_lim_2p = 2.3;
            fsEl_RES_lower_lim_2p = 0;
            fsEl_DIS_upper_lim_2p = 2.3;
            fsEl_DIS_lower_lim_2p = 0;
            fsEl_VS_theta_lp_upper_lim_2p_x = 70.0;
//            fsEl_VS_theta_lp_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_lp_lower_lim_2p_x = 0;
            fsEl_VS_theta_lp_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_lp_lower_lim_2p_y = 0;
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
            fsEl_VS_theta_lp_upper_lim_2p_x = 70.0;
//            fsEl_VS_theta_lp_upper_lim_2p_x = 90.0;
            fsEl_VS_theta_lp_lower_lim_2p_x = 0;
            fsEl_VS_theta_lp_upper_lim_2p_y = 2.3;
            fsEl_VS_theta_lp_lower_lim_2p_y = 0;
        }
         */
        //</editor-fold>

        //<editor-fold desc="Energy histograms (1n1p)">
        fsEl_upper_lim_1n1p = 300;
        fsEl_lower_lim_1n1p = -10;
//        fsEl_upper_lim_1n1p = 2.3;
//        fsEl_lower_lim_1n1p = 1;
        fsEl_QEL_upper_lim_1n1p = 2.3;
        fsEl_QEL_lower_lim_1n1p = 0;
        fsEl_MEC_upper_lim_1n1p = 2.3;
        fsEl_MEC_lower_lim_1n1p = 0;
        fsEl_RES_upper_lim_1n1p = 2.3;
        fsEl_RES_lower_lim_1n1p = 0;
        fsEl_DIS_upper_lim_1n1p = 2.3;
        fsEl_DIS_lower_lim_1n1p = 0;
        fsEl_VS_theta_lp_upper_lim_1n1p_x = 70.0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_x = 90.0;
        fsEl_VS_theta_lp_lower_lim_1n1p_x = 0;
        fsEl_VS_theta_lp_upper_lim_1n1p_y = 2.3;
        fsEl_VS_theta_lp_lower_lim_1n1p_y = 0;

        /*        if (FSI_status == true) {
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
            fsEl_VS_theta_lp_upper_lim_1n1p_x = 70.0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_lp_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_lp_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_lp_lower_lim_1n1p_y = 0;
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
            fsEl_VS_theta_lp_upper_lim_1n1p_x = 70.0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_x = 90.0;
            fsEl_VS_theta_lp_lower_lim_1n1p_x = 0;
            fsEl_VS_theta_lp_upper_lim_1n1p_y = 2.3;
            fsEl_VS_theta_lp_lower_lim_1n1p_y = 0;
        }
*/
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
        E_Trans15_all_inclusive_upper_lim = 6;
        E_Trans15_all_inclusive_lower_lim = 0;
        E_Trans15_QEL_inclusive_upper_lim = 6;
        E_Trans15_QEL_inclusive_lower_lim = 0;
        E_Trans15_MEC_inclusive_upper_lim = 6;
        E_Trans15_MEC_inclusive_lower_lim = 0;
        E_Trans15_RES_inclusive_upper_lim = 6;
        E_Trans15_RES_inclusive_lower_lim = 0;
        E_Trans15_DIS_inclusive_upper_lim = 6;
        E_Trans15_DIS_inclusive_lower_lim = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_x = 6;
        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_all_inclusive_upper_lim_y = 6;
        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 6;
        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 6;
        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 6;
        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 6;
        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
        E_Trans_VS_q3_all_upper_lim_x_2p = 6;
        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
        E_Trans_VS_q3_all_upper_lim_y_2p = 6;
        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_2p = 6;
        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_2p = 6;
        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_2p = 6;
        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_2p = 6;
        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
        E_Trans_VS_q3_all_upper_lim_x_1n1p = 6;
        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_all_upper_lim_y_1n1p = 6;
        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 6;
        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 6;
        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 6;
        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 6;
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
        P_L_hist_upper_lim_2p = 6.0;
        P_L_hist_lower_lim_2p = 0;
        P_R_hist_upper_lim_2p = 6.0;
        P_R_hist_lower_lim_2p = 0;
        P_lp_hist_upper_lim_2p = 6.0;
        P_lp_hist_lower_lim_2p = 0;
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        P_p_hist_upper_lim_1n1p = 6.0;
        P_p_hist_lower_lim_1n1p = 0;
        P_n_hist_upper_lim_1n1p = 6.0;
        P_n_hist_lower_lim_1n1p = 0;
        P_lp_hist_upper_lim_1n1p = 6.0;
        P_lp_hist_lower_lim_1n1p = 0;
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

//    system("[ -d '/home/alon/project/plots/theta_histograms' ] && echo 'Directory /home/alon/project/plots/theta_histograms exists.'");

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
    myLogFile << "normalized_theta_lp_plots = " << BoolToString(normalized_theta_lp_plots) << "\n";
    myLogFile << "normalized_E_lp_plots = " << BoolToString(normalized_E_lp_plots) << "\n";
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
    myLogFile << "P_lp_upper_lim_2p = " << P_lp_upper_lim_2p << "\n";
    myLogFile << "P_lp_lower_lim_2p = " << P_lp_lower_lim_2p << "\n";
    myLogFile << "P_p1_upper_lim_2p = " << P_p1_upper_lim_2p << "\n";
    myLogFile << "P_p1_lower_lim_2p = " << P_p1_lower_lim_2p << "\n";
    myLogFile << "P_p2_upper_lim_2p = " << P_p2_upper_lim_2p << "\n";
    myLogFile << "P_p2_lower_lim_2p = " << P_p2_lower_lim_2p << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Momentum thresholds (1n1p)\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "P_lp_upper_lim_1n1p = " << P_lp_upper_lim_1n1p << "\n";
    myLogFile << "P_lp_lower_lim_1n1p = " << P_lp_lower_lim_1n1p << "\n";
    myLogFile << "P_p_upper_lim_1n1p = " << P_p_upper_lim_1n1p << "\n";
    myLogFile << "P_p_lower_lim_1n1p = " << P_p_lower_lim_1n1p << "\n";
    myLogFile << "P_n_upper_lim_1n1p = " << P_n_upper_lim_1n1p << "\n";
    myLogFile << "P_n_lower_lim_1n1p = " << P_n_lower_lim_1n1p << "\n";
    myLogFile << "\n";
    myLogFile << "\n";

    myLogFile << "Momentum thresholds (2p, MicroBooNE)\n";
    myLogFile << "==========================================================================================\n";
    myLogFile << "P_lp_upper_lim_MicroBooNE = " << P_lp_upper_lim_MicroBooNE << "\n";
    myLogFile << "P_lp_lower_lim_MicroBooNE = " << P_lp_lower_lim_MicroBooNE << "\n";
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

    cout << "\nDefining histograms...";

// Beta VS P histograms -------------------------------------------------------------------------------

    //<editor-fold desc="Beta vs P histograms">
    TH2D *Beta_vs_P_CD = new TH2D("#beta vs P (CD)", "#beta vs P (Central Detector);P [GeV];#beta", 250, 0, Ev * 1.1, 250, 0, 10);
    TH2D *Beta_vs_P_FD = new TH2D("#beta vs P (FD)", "#beta vs P (Forward Detector);P [GeV];#beta", 250, 0, Ev * 1.1, 250, 0, 10);
//    TH2D *Beta_vs_P_CD = new TH2D("#beta vs P (CD)", "#beta vs P (Central Detector);P [GeV];#beta", 250, 0, Ev * 1.1, 250, 0, 1.5);
//    TH2D *Beta_vs_P_FD = new TH2D("#beta vs P (FD)", "#beta vs P (Forward Detector);P [GeV];#beta", 250, 0, Ev * 1.1, 250, 0, 1.5);
    //</editor-fold>

// Chi2 plots -------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">
    THStack *Chi2_Electron_Stack = new THStack("Electron #chi^{2} (CD & FD)", "Electron #chi^{2} (CD & FD);Electron #chi^{2};");
    THStack *Chi2_Proton_Stack = new THStack("Proton #chi^{2} (CD & FD)", "Proton #chi^{2} (CD & FD);Proton #chi^{2};");

    TH1D *Chi2_Electron_CD = new TH1D("Electron #chi^{2} (CD)", "Electron #chi^{2} (Central Detector);Electron #chi^{2};", 100, -10, 10);
    TH1D *Chi2_Electron_FD = new TH1D("Electron #chi^{2} (FD)", "Electron #chi^{2} (Forward Detector);Electron #chi^{2};", 100, -10, 10);

    TH1D *Chi2_Proton_CD = new TH1D("Proton #chi^{2} (CD)", "Proton #chi^{2} (Central Detector);Proton #chi^{2};", 100, -10, 10);
    TH1D *Chi2_Proton_FD = new TH1D("Proton #chi^{2} (FD)", "Proton #chi^{2} (Forward Detector);Proton #chi^{2};", 100, -10, 10);
    //</editor-fold>

// Theta histograms -----------------------------------------------------------------------------------

    //<editor-fold desc="Theta histograms">
    THStack *ThetaStack = new THStack("#theta_{l} stack (2p & 1n1p)", "#theta_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p);#theta_{l} [Deg];");

//    TH1D *Theta_lp_histogram_test = new TH1D("Theta_lp_histogram_test", "Theta_lp_histogram_test;#theta_{l} [Deg];", 100, theta_lp_lower_lim_2p, theta_lp_upper_lim_2p);

    TH1D *Theta_lp_histogram = new TH1D("#theta_{l} det sim (2p)", ";#theta_{l} [Deg];", 100, theta_lp_lower_lim_2p, theta_lp_upper_lim_2p);
    TH1D *Theta_p1_histogram = new TH1D("#theta_{p1} det sim (2p)", ";#theta_{p1} [Deg];", 100, theta_p1_lower_lim_2p, theta_p1_upper_lim_2p);
    TH1D *Theta_p2_histogram = new TH1D("#theta_{p2} det sim (2p)", ";#theta_{p2} [Deg];", 100, theta_p2_lower_lim_2p, theta_p2_upper_lim_2p);
    TH1D *dtheta_2p = new TH1D("#gamma det sim (2p)", ";#gamma_{Lab} = #theta_{p1} - #theta_{p2} [Deg];", 100, dtheta_lower_lim_2p, dtheta_upper_lim_2p);

    TH1D *theta_lp_1n1p = new TH1D("#theta_{l} det sim (1n1p)", ";#theta_{l} [Deg];", 100, theta_lp_lower_lim_1n1p, theta_lp_upper_lim_1n1p);
    TH1D *theta_p_1n1p = new TH1D("#theta_{p} det sim (1n1p)", ";#theta_{p} [Deg];", 100, theta_p_lower_lim_1n1p, theta_p_upper_lim_1n1p);
    TH1D *theta_n_1n1p = new TH1D("#theta_{n} det sim (1n1p)", ";#theta_{n} [Deg];", 100, theta_n_lower_lim_1n1p, theta_n_upper_lim_1n1p);
    TH1D *dtheta_1n1p = new TH1D("#gamma det sim (1n1p)", ";#gamma_{Lab} = #theta_{p} - #theta_{n} [Deg];", 100, dtheta_lower_lim_1n1p, dtheta_upper_lim_1n1p);
    //</editor-fold>

// Phi histograms -------------------------------------------------------------------------------------

    //<editor-fold desc="Phi histograms">
    THStack *PhiStack = new THStack("#phi_{l} stack det sim (2p & 1n1p)", "#phi_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p);#phi_{l} [Deg];");

    TH1D *phi_lp_2p = new TH1D("#phi_{l} det sim (2p)", ";#phi_{l} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *phi_p1_2p = new TH1D("#phi_{p1} det sim (2p)", ";#phi_{p1} [Deg];", 100, phi_p1_lower_lim_2p, phi_p1_upper_lim_2p);
    TH1D *phi_p2_2p = new TH1D("#phi_{p2} det sim (2p)", ";#phi_{p2} [Deg];", 100, phi_p2_lower_lim_2p, phi_p2_upper_lim_2p);
    TH1D *dphi_2p = new TH1D("#Delta#phi det sim (2p)", ";#Delta#phi [Deg];", 100, dphi_lower_lim_2p, dphi_upper_lim_2p);

    TH1D *phi_lp_1n1p = new TH1D("#phi_{l} det sim (1n1p)", ";#phi_{l} [Deg];", 100, phi_lp_lower_lim_1n1p, phi_lp_upper_lim_1n1p);
    TH1D *phi_p_1n1p = new TH1D("#phi_{p} det sim (1n1p)", ";#phi_{p1} [Deg];", 100, phi_p_lower_lim_1n1p, phi_p_upper_lim_1n1p);
    TH1D *phi_n_1n1p = new TH1D("#phi_{n} det sim (1n1p)", ";#phi_{p2} [Deg];", 100, phi_n_lower_lim_1n1p, phi_n_upper_lim_1n1p);
    TH1D *dphi_1n1p = new TH1D("#Delta#phi det sim (1n1p)", ";#Delta#phi [Deg];", 100, dphi_lower_lim_1n1p, dphi_upper_lim_1n1p);
    //</editor-fold>

// Energy histograms ----------------------------------------------------------------------------------

    //<editor-fold desc="Energy histograms">
    THStack *EnergyStack = new THStack("E_{l} stack det sim (2p & 1n1p)", "Final State E_{l} (All Interactions, 2p and 1n1p);E_{l} [GeV]");

//    TH1D *fsEl_histogram_test = new TH1D("fsEl/100_histogram_test", "fsEl/100_histogram_test;E_{l}/100 []", 100, fsEl_lower_lim_2p, fsEl_upper_lim_2p);

    TH1D *fsEl_2p = new TH1D("Final State E_{l} det sim (2p)", ";E_{l} [???]", 100, fsEl_lower_lim_2p, fsEl_upper_lim_2p);
//    TH1D *fsEl_2p = new TH1D("Final State E_{l} det sim (2p)", ";E_{l} [GeV]", 100, fsEl_lower_lim_2p, fsEl_upper_lim_2p);
    TH1D *fsEl_QEL_2p = new TH1D("Final State E_{l} det sim (QEL Only, 2p)", ";E_{l} [GeV]", 100, fsEl_QEL_lower_lim_2p, fsEl_QEL_upper_lim_2p);
    TH1D *fsEl_MEC_2p = new TH1D("Final State E_{l} det sim (MEC Only, 2p)", ";E_{l} [GeV]", 100, fsEl_MEC_lower_lim_2p, fsEl_MEC_upper_lim_2p);
    TH1D *fsEl_RES_2p = new TH1D("Final State E_{l} det sim (RES Only, 2p)", ";E_{l} [GeV]", 100, fsEl_RES_lower_lim_2p, fsEl_RES_upper_lim_2p);
    TH1D *fsEl_DIS_2p = new TH1D("Final State E_{l} det sim (DIS Only, 2p)", ";E_{l} [GeV]", 100, fsEl_DIS_lower_lim_2p, fsEl_DIS_upper_lim_2p);
    TH2D *fsEl_VS_theta_lp_all_int_2p = new TH2D("Stat det sim (All Interactions, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                 200, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_x,
                                                 200, fsEl_VS_theta_lp_lower_lim_1n1p_y, fsEl_VS_theta_lp_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_l_QEL_only_2p = new TH2D("Stat det sim (QEL only, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                 200, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_x,
                                                 200, fsEl_VS_theta_lp_lower_lim_1n1p_y, fsEl_VS_theta_lp_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_lp_MEC_only_2p = new TH2D("Stat det sim (MEC only, 2p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                  200, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_x,
                                                  200, fsEl_VS_theta_lp_lower_lim_1n1p_y, fsEl_VS_theta_lp_upper_lim_1n1p_y);

    TH1D *fsEl_1n1p = new TH1D("Final State E_{l} det sim (1n1p)", ";E_{l} [???];", 100, fsEl_lower_lim_1n1p, fsEl_upper_lim_1n1p);
//    TH1D *fsEl_1n1p = new TH1D("Final State E_{l} det sim (1n1p)", ";E_{l} [GeV];", 100, fsEl_lower_lim_1n1p, fsEl_upper_lim_1n1p);
    TH1D *fsEl_QEL_1n1p = new TH1D("Final State E_{l} det sim (QEL Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_QEL_lower_lim_1n1p, fsEl_QEL_upper_lim_1n1p);
    TH1D *fsEl_MEC_1n1p = new TH1D("Final State E_{l} det sim (MEC Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_MEC_lower_lim_1n1p, fsEl_MEC_upper_lim_1n1p);
    TH1D *fsEl_RES_1n1p = new TH1D("Final State E_{l} det sim (RES Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_RES_lower_lim_1n1p, fsEl_RES_upper_lim_1n1p);
    TH1D *fsEl_DIS_1n1p = new TH1D("Final State E_{l} det sim (DIS Only, 1n1p)", ";E_{l} [GeV]", 100, fsEl_DIS_lower_lim_1n1p, fsEl_DIS_upper_lim_1n1p);
    TH2D *fsEl_VS_theta_lp_all_int_1n1p = new TH2D("Stat det sim (All Interactions, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                   200, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_x,
                                                   200, fsEl_VS_theta_lp_lower_lim_1n1p_y, fsEl_VS_theta_lp_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_lp_QEL_only_1n1p = new TH2D("Stat det sim (QEL only, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                    200, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_x,
                                                    200, fsEl_VS_theta_lp_lower_lim_1n1p_y, fsEl_VS_theta_lp_upper_lim_1n1p_y);
    TH2D *fsEl_VS_theta_lp_MEC_only_1n1p = new TH2D("Stat det sim (MEC only, 1n1p)", ";#theta_{l} [Deg];E_{l} [GeV]",
                                                    200, fsEl_VS_theta_lp_lower_lim_1n1p_x, fsEl_VS_theta_lp_upper_lim_1n1p_x,
                                                    200, fsEl_VS_theta_lp_lower_lim_1n1p_y, fsEl_VS_theta_lp_upper_lim_1n1p_y);

    //</editor-fold>

// Energy Transfer histograms (all interactions) ------------------------------------------------------

    //<editor-fold desc="Energy Transfer histograms">

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

    //</editor-fold>

// Inclusive Energy Transfer histograms ---------------------------------------------------------------

    //<editor-fold desc="Inclusive Energy Transfer histograms">
    THStack *Energy_Transfer_all_int_15_inclusive_Stack = new THStack("Energy_Transfer_all_int_15_inclusive_Stack", ";E_{#nu}-E_{l} [GeV]");

    TH1D *E_Trans15_all_inclusive = new TH1D("ET around 15 degrees (all interactions, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                                             100, E_Trans15_all_inclusive_lower_lim, E_Trans15_all_inclusive_upper_lim);
    TH1D *E_Trans15_QEL_inclusive = new TH1D("ET around 15 degrees (QEL Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                                             100, E_Trans15_QEL_inclusive_lower_lim, E_Trans15_QEL_inclusive_upper_lim);
    TH1D *E_Trans15_MEC_inclusive = new TH1D("ET around 15 degrees (MEC Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                                             100, E_Trans15_MEC_inclusive_lower_lim, E_Trans15_MEC_inclusive_upper_lim);
    TH1D *E_Trans15_RES_inclusive = new TH1D("ET around 15 degrees (RES Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                                             100, E_Trans15_RES_inclusive_lower_lim, E_Trans15_RES_inclusive_upper_lim);
    TH1D *E_Trans15_DIS_inclusive = new TH1D("ET around 15 degrees (DIS Only, inclusive)", ";E_{#nu}-E_{l} [GeV]",
                                             100, E_Trans15_DIS_inclusive_lower_lim, E_Trans15_DIS_inclusive_upper_lim);

    TH1D *Q2_hist_inclusive = new TH1D("Q^{2} (inclusive)", ";Q^{2} [GeV^{2}]", 100, -0.05, 0.8); // CHANGE TITLE & ADD CUSTOM LIMITS

    TH2D *E_Trans_VS_q_all_inclusive = new TH2D("ET around #omega VS q_{3} (All Interactions)", ";|q| = |p_{v} - p_{l}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                                200, E_Trans_VS_q_all_inclusive_lower_lim_x, E_Trans_VS_q_all_inclusive_upper_lim_x,
                                                200, E_Trans_VS_q_all_inclusive_lower_lim_y, E_Trans_VS_q_all_inclusive_upper_lim_y);
    TH2D *E_Trans_VS_q_QEL_inclusive = new TH2D("ET around #omega VS q_{3} (QEL Only)", ";|q| = |p_{v} - p_{l}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                                200, E_Trans_VS_q_QEL_inclusive_lower_lim_x, E_Trans_VS_q_QEL_inclusive_upper_lim_x,
                                                200, E_Trans_VS_q_QEL_inclusive_lower_lim_y, E_Trans_VS_q_QEL_inclusive_upper_lim_y);
    TH2D *E_Trans_VS_q_MEC_inclusive = new TH2D("ET around #omega VS q_{3} (MEC Only)", ";|q| = |p_{v} - p_{l}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                                200, E_Trans_VS_q_MEC_inclusive_lower_lim_x, E_Trans_VS_q_MEC_inclusive_upper_lim_x,
                                                200, E_Trans_VS_q_MEC_inclusive_lower_lim_y, E_Trans_VS_q_MEC_inclusive_upper_lim_y);

    TH2D *E_Trans_VS_q3_all_2p = new TH2D("ET around #omega VS q_{3} (All Interactions, 2p)",
                                          "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (All Interactions, 2p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                          200, E_Trans_VS_q3_all_lower_lim_x_2p, E_Trans_VS_q3_all_upper_lim_x_2p,
                                          200, E_Trans_VS_q3_all_lower_lim_y_2p, E_Trans_VS_q3_all_upper_lim_y_2p);
    TH2D *E_Trans_VS_q3_QEL_2p = new TH2D("ET around #omega VS q_{3} (QEL Only, 2p)",
                                          "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (QEL Only, 2p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                          200, E_Trans_VS_q3_QEL_lower_lim_x_2p, E_Trans_VS_q3_QEL_upper_lim_x_2p,
                                          200, E_Trans_VS_q3_QEL_lower_lim_y_2p, E_Trans_VS_q3_QEL_upper_lim_y_2p);
    TH2D *E_Trans_VS_q3_MEC_2p = new TH2D("ET around #omega VS q_{3} (MEC Only, 2p)",
                                          "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (MEC Only, 2p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                          200, E_Trans_VS_q3_MEC_lower_lim_x_2p, E_Trans_VS_q3_MEC_upper_lim_x_2p,
                                          200, E_Trans_VS_q3_MEC_lower_lim_y_2p, E_Trans_VS_q3_MEC_upper_lim_y_2p);

    TH2D *E_Trans_VS_q3_all_1n1p = new TH2D("ET around #omega VS q_{3} (All Interactions, 1n1p)",
                                            "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (All Interactions, 1n1p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                            200, E_Trans_VS_q3_all_lower_lim_x_1n1p, E_Trans_VS_q3_all_upper_lim_x_1n1p,
                                            200, E_Trans_VS_q3_all_lower_lim_y_1n1p, E_Trans_VS_q3_all_upper_lim_y_1n1p);
    TH2D *E_Trans_VS_q3_QEL_1n1p = new TH2D("ET around #omega VS q_{3} (QEL Only, 1n1p)",
                                            "Energy Transfer (E_{#nu}-E_{l}) vs q_{3} (QEL Only, 1n1p);q_{3} = |p_{v,3} - p_{l,3}| [GeV/c];#omega = E_{v} - E_{l} [GeV]",
                                            200, E_Trans_VS_q3_QEL_lower_lim_x_1n1p, E_Trans_VS_q3_QEL_upper_lim_x_1n1p,
                                            200, E_Trans_VS_q3_QEL_lower_lim_y_1n1p, E_Trans_VS_q3_QEL_upper_lim_y_1n1p);
    TH2D *E_Trans_VS_q3_MEC_1n1p = new TH2D("ET around #omega VS q_{3} (MEC Only, 1n1p)",
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
    } else {
        E_Trans15_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (All Interactions, ^{12}C(e,e'))");
        E_Trans15_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (QEL Only, ^{12}C(e,e'))");
        E_Trans15_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (MEC Only, ^{12}C(e,e'))");
        E_Trans15_RES_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (RES Only, ^{12}C(e,e'))");
        E_Trans15_DIS_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (DIS Only, ^{12}C(e,e'))");
        Q2_hist_inclusive->SetTitle("Q^{2} Histogram ^{12}C(e,e')");
        E_Trans_VS_q_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (All Interactions, ^{12}C(e,e'))");
        E_Trans_VS_q_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (QEL Only, ^{12}C(e,e'))");
        E_Trans_VS_q_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (MEC Only, ^{12}C(e,e'))");
    }
    //</editor-fold>

    //</editor-fold>

// E_cal restoration histograms -----------------------------------------------------------------------

    //<editor-fold desc="E_cal restoration histograms">

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
    TH2D *E_cal_VS_theta_lp_all_int_2p = new
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
    TH2D *E_cal_VS_theta_lp_QEL_only_2p = new
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
    TH2D *E_cal_VS_theta_lp_QEL_1n1p = new
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

    //</editor-fold>

// Momentum histograms -----------------------------------------------------------------------------

    //<editor-fold desc="Momentum histograms">
    THStack *MomentumStack_2p = new THStack("Momentum Stack (2p)", "Momentum Histogram Stack (All Interactions, 2p);Momentum [GeV]");
    THStack *MomentumStack_1n1p = new THStack("Momentum Stack (1n1p)", "Momentum Histogram Stack (All Interactions, 1n1p);Momentum [GeV]");

    TH1D *P_L_hist_2p = new TH1D("P_{p1} (all interactions, 2p)", ";P_{L} [GeV/c]", 100, P_L_hist_lower_lim_2p, P_L_hist_upper_lim_2p);
    TH1D *P_R_hist_2p = new TH1D("P_{p2} (all interactions, 2p)", ";P_{R} [GeV/c]", 100, P_R_hist_lower_lim_2p, P_R_hist_upper_lim_2p);
    TH1D *P_lp_hist_2p = new TH1D("P_{l} (all interactions, 2p)", ";P_{l} [GeV/c]", 100, P_lp_hist_lower_lim_2p, P_lp_hist_upper_lim_2p);

    TH1D *P_p_hist_1n1p = new TH1D("P_{p} (all interactions, 1n1p)", ";P_{p} [GeV/c]", 100, P_p_hist_lower_lim_1n1p, P_p_hist_upper_lim_1n1p);
    TH1D *P_n_hist_1n1p = new TH1D("P_{n} (all interactions, 1n1p)", ";P_{n} [GeV/c]", 100, P_n_hist_lower_lim_1n1p, P_n_hist_upper_lim_1n1p);
    TH1D *P_lp_hist_1n1p = new TH1D("P_{l} (all interactions, 1n1p)", ";P_{l} [GeV/c]", 100, P_lp_hist_lower_lim_1n1p, P_lp_hist_upper_lim_1n1p);
    //</editor-fold>

// MicroBooNE histogram reconstruction -------------------------------------------------------------

    //<editor-fold desc="MicroBooNE histogram reconstruction">

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
    TH1D *P_lp_hist = new
            TH1D("P_{l}", "Momentum of Lepton (P_{l});P_{l} [GeV/c]", 100, P_lp_hist_lower_lim, P_lp_hist_upper_lim);
    TH1D *P_pion_hist = new
            TH1D("P_{#pi^{#pm}}", "Momentum of Pions (P_{#pi^{#pm}});P_{#pi^{#pm}} [GeV/c]", 100, P_pion_hist_lower_lim, P_pion_hist_upper_lim);

    string dP_T_title = "#deltaP_{T} Histogram (" + file_name + ")";
    const char *dP_T_Title = dP_T_title.c_str();
    dP_T_hist->SetTitle(dP_T_Title);

    string dP_T_weighted_title = "#deltaP_{T} Histogram (" + file_name + ", Q^{4} weighted)";
    const char *dP_T_weighted_Title = dP_T_weighted_title.c_str();
    dP_T_hist_weighted->SetTitle(dP_T_weighted_Title);
    //</editor-fold>

    //</editor-fold>

// List definition ---------------------------------------------------------------------------------

    TList *plots = new TList();

    cout << " done.\n\n";
    //</editor-fold>


// Code execution:
// =======================================================================================================================================================================

    //<editor-fold desc="Code execution">

    //<editor-fold desc="HipoChain tests">
    //    gROOT->ProcessLine( "gErrorIgnoreLevel = 1000;");
//    gSystem->RedirectOutput("mylogfile.txt", "a");
//    clas12root::HipoChain chain;
//    string AnalyseFileDirContent = AnalyseFileDir + "/*.hipo";
//    chain.Add(AnalyseFileDirContent.c_str());
//    auto config_c12=chain.GetC12Reader();
//    chain.SetReaderTags({0});
//    auto& c12=chain.C12ref();
//    gSystem->RedirectOutput(0,0);
    //</editor-fold>

    clas12reader c12(LoadedInput.c_str()); // open file

//    c12.addExactPid(11, 1); // exactly 1 electron (outgoing lepton)
//    c12.addAtLeastPid(2212, 1); // at least 1 proton (1 for 1n1p, 2 for 2p)
//    c12.addAtLeastPid(2112, 0); // at least 1 neutron (1 for 1n1p, 0 for 2p)
//    c12.addAtLeastPid(211, 0); // at least 0 pi+ (MicroBooNE)
//    c12.addAtLeastPid(-211, 0); // at least 0 pi- (MicroBooNE)

//    c12.addZeroOfRestPid(); // nothing else

    int num_of_2p_events = 0;
    int num_of_1n1p_events = 0;
    int num_of_MicroBooNE_events = 0;

    while (c12.next()) { // loop over events

        auto particles = c12.getDetParticles(); //particles are now a std::vector of particles for this event

        auto electrons = c12.getByID(11);
        auto protons = c12.getByID(2212);
        auto neutrons = c12.getByID(2112);
        auto pizero = c12.getByID(111);
        auto piplus = c12.getByID(211);
        auto piminus = c12.getByID(-211);

        //<editor-fold desc="Fill Beta vs P">
        for (int i = 0; i < particles.size(); i++) {
            if (particles[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(particles[i]->getP(), particles[i]->par()->getBeta());
            } else if (particles[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(particles[i]->getP(), particles[i]->par()->getBeta());
            }
        } // end of loop over particles vector
        //</editor-fold>

        //<editor-fold desc="Fill Chi2_Proton_CD & Chi2_Proton_FD">
        for (auto &p: protons) {
            if (p->getRegion() == CD) {
                Chi2_Proton_CD->Fill(p->par()->getChi2Pid());
            } else if (p->getRegion() == FD) {
                Chi2_Proton_FD->Fill(p->par()->getChi2Pid());
            }
        } // end of loop over particles vector
        //</editor-fold>

        //<editor-fold desc="Fill Chi2_Electron_CD & Chi2_Electron_FD">
        for (auto &e: electrons) {
            if (e->getRegion() == CD) {
                Chi2_Electron_CD->Fill(e->par()->getChi2Pid());
            } else if (e->getRegion() == FD) {
                Chi2_Electron_FD->Fill(e->par()->getChi2Pid());
            }
        } // end of loop over particles vector
        //</editor-fold>


//  Inclusive calculations
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Inclusive calculations">
        if (calculate_inclusive == true) {
            int lepton_ind_inclusive = -1;

            for (int i = 0; i < particles.size(); i++) {

                float particlePDG_inclusive = particles[i]->par()->getPid();

                if (selection_test_inclusive) {
                    cout << "particlePDG_inclusive[" << i << "] = " << particlePDG_inclusive << "\n";
                } // end of selection test if (inclusive)

                //<editor-fold desc="lepton selector (inclusive)">
                if (particlePDG_inclusive == 11) {
                    lepton_ind_inclusive = i;
                } // end of lepton selector (inclusive)
                //</editor-fold>
            } // end of loop over particles vector

//          Energy transfer VS q3,q calculations:
            double Plx = particles[lepton_ind_inclusive]->par()->getPx();
            double Ply = particles[lepton_ind_inclusive]->par()->getPy();
            double Plz = particles[lepton_ind_inclusive]->par()->getPz();

            double El = particles[lepton_ind_inclusive]->getDeltaEnergy();

            double q3 = abs(Pvz - Plz);
            double q = rCalc(Pvx - Plx, Pvy - Ply, Pvz - Plz);

            E_Trans_VS_q_all_inclusive->Fill(q3, Ev - El);
//        Q2_hist_inclusive->Fill(Q2);

//        if (qel == true) {
//            E_Trans_VS_q_QEL_inclusive->Fill(q, Ev - El);
//        } else if (mec == true) {
//            E_Trans_VS_q_MEC_inclusive->Fill(q, Ev - El);
//        }

            double Theta_lp_inclusive = particles[lepton_ind_inclusive]->getTheta() * 180.0 / 3.14159265359; // In degrees
//        double Theta_lp_inclusive = acos(Plz / rCalc(Plx, Ply, Plz)) * 180.0 / 3.14159265359; // In degrees

//      Theta_l inclusive calculations:
            if (Theta_lp_inclusive >= 14.0 && Theta_lp_inclusive <= 16.0) {
                E_Trans15_all_inclusive->Fill(Ev - El);
//            if (qel == true) {
//                if (Theta_lp_inclusive >= 14.0 && Theta_lp_inclusive <= 16.0) {
//                    E_Trans15_QEL_inclusive->Fill(Ev - El);
//                }
//            } else if (mec == true) {
//                if (Theta_lp_inclusive >= 14.0 && Theta_lp_inclusive <= 16.0) {
//                    E_Trans15_MEC_inclusive->Fill(Ev - El);
//                }
//            } else if (res == true) {
//                if (Theta_lp_inclusive >= 14.0 && Theta_lp_inclusive <= 16.0) {
//                    E_Trans15_RES_inclusive->Fill(Ev - El);
//                }
//            } else if (dis == true) {
//                if (Theta_lp_inclusive >= 14.0 && Theta_lp_inclusive <= 16.0) {
//                    E_Trans15_DIS_inclusive->Fill(Ev - El);
//                }
//            }
            }
        }
        //</editor-fold>


//  2p calculations
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="2p calculations">
        if (calculate_2p && protons.size() == 2 && neutrons.size() == 0 && piplus.size() == 0 && piminus.size() == 0) {
            ++num_of_2p_events;

            if (selection_test_2p) {
                cout << "==========================================================================\n";
                cout << "num_of_2p_events = " << num_of_2p_events << "\n\n\n";
                cout << "#electrons = " << electrons.size() << ", #protons = " << protons.size() << ", #neutrons = " << neutrons.size() << "\n\n\n";
            } // end of selection test if (2p)

            int ProtonCounter_2p = 0, OtherParticleCounter_2p = 0;
            int Lepton_ind_2p = -1, Proton_1_ind_2p = -1, Proton_2_ind_2p = -1;

            for (int i = 0; i < particles.size(); i++) {

                float particlePDG_2p = particles[i]->par()->getPid();

                if (selection_test_2p) {
                    cout << "particlePDG_2p[" << i << "] = " << particlePDG_2p << "\n";
                } // end of selection test if (2p)

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
                        cout << "\nAdditional Protons detected (2p). PDG = " << particlePDG_2p << "\n\n\n";
                    }
                } else if (particlePDG_2p == 11) {
                    Lepton_ind_2p = i;
//                    cout << "Lepton_ind_2p = " << Lepton_ind_2p << "\n";
//                    cout << "particlePDG_2p[" << i << "] = " << particlePDG_2p << "\n";
                } else if (particlePDG_2p != 2212) {
                    ++OtherParticleCounter_2p;
                    if (OtherParticleCounter_2p > 0) {
                        cout << "\nAdditional particles detected (2p). PDG = " << particlePDG_2p << "\n\n\n";
                    }
                } // end of selector (2p)
                //</editor-fold>

            } // end of loop over particles vector

            double P_lp_2p = particles[Lepton_ind_2p]->getP(); // Momentum of lepton in particles vector
            double P_p1_2p = particles[Proton_1_ind_2p]->getP(); // Momentum of first proton in particles vector
            double P_p2_2p = particles[Proton_2_ind_2p]->getP(); // Momentum of first proton in particles vector

            double P_L_2p = -1; // Leading proton
            double P_R_2p = -1; // Recoil proton

//          Momentum cut to at least 300 [MeV/c] == 0.3 [GeV/c]:
            if (P_p1_2p >= 0 && P_p2_2p >= 0) {
//            if (P_p1_2p >= P_p1_lower_lim_2p && P_p2_2p >= P_p2_lower_lim_2p) {

//                double E_cal_2p;
//
//                if (BEnergyToNucleusCon == true) {
//                    E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272) + 2 * BEnergyToNucleus;
//                } else if (BEnergyToNucleusCon == false) {
//                    E_cal_2p = El + (Ef[Proton_1_ind_2p] - 0.938272) + (Ef[Proton_2_ind_2p] - 0.938272);
//                }

                double Phi_lp_2p = particles[Lepton_ind_2p]->getPhi() * 180.0 / 3.14159265359; // Phi of lepton in particles (in deg)
//                double Phi_lp_2p = atan2(particles[Lepton_ind_2p]->par()->getPy(), particles[Lepton_ind_2p]->par()->getPx())
//                                  * 180.0 / 3.14159265359; // Theta of lepton in particles (in deg)
                phi_lp_2p->Fill(Phi_lp_2p);

//                double theta_lp_2p = particles[Lepton_ind_2p]->che(HTCC)->getDtheta(); // Theta of lepton in particles (in radians)
                double theta_lp_2p = particles[Lepton_ind_2p]->getTheta() * 180.0 / 3.14159265359; // Theta of lepton in particles  (in deg)
                Theta_lp_histogram->Fill(theta_lp_2p);
//                cout << "theta_lp_2p = " << theta_lp_2p * 180.0 / 3.14159265359 << "\n\n";


//              ***NOT REALLY dtheta:
                double d_theta_2p = acos(
                        (particles[Proton_1_ind_2p]->par()->getPx() * particles[Proton_2_ind_2p]->par()->getPx() +
                         particles[Proton_1_ind_2p]->par()->getPx() * particles[Proton_2_ind_2p]->par()->getPx() +
                         particles[Proton_1_ind_2p]->par()->getPx() * particles[Proton_2_ind_2p]->par()->getPx()) /
                        (particles[Proton_1_ind_2p]->getP() * particles[Proton_2_ind_2p]->getP())) * 180.0 / 3.14159265359;
                dtheta_2p->Fill(d_theta_2p);

                //<editor-fold desc="P_L & P_R selector">
                if (P_p1_2p >= P_p2_2p) { // If Proton_1_ind_2p is the leading proton and Proton_2_ind_2p is the recoil

                    P_L_2p = P_p1_2p; // Leading proton
                    P_L_hist_2p->Fill(P_L_2p);

                    P_R_2p = P_p2_2p; // Recoil proton
                    P_R_hist_2p->Fill(P_R_2p);

                    double phi_p1 = particles[Proton_1_ind_2p]->getPhi() * 180.0 / 3.14159265359; // Leading proton phi (in deg)
                    phi_p1_2p->Fill(phi_p1);

                    double phi_p2 = particles[Proton_2_ind_2p]->getPhi() * 180.0 / 3.14159265359; // Recoil proton phi (in deg)
                    phi_p2_2p->Fill(phi_p2);

                    double d_phi_p2 = phi_p1 - phi_p2; // In deg
                    dphi_2p->Fill(d_phi_p2);

                    double theta_p1 = particles[Proton_1_ind_2p]->getTheta() * 180.0 / 3.14159265359; // Leading proton scattering angle theta (in deg)
                    Theta_p1_histogram->Fill(theta_p1);

                    double theta_p2 = particles[Proton_2_ind_2p]->getTheta() * 180.0 / 3.14159265359; // Recoil proton scattering angle theta (in deg)
                    Theta_p2_histogram->Fill(theta_p2);

//                    if (qel == true) {
//                        E_cal_VS_theta_p1_QEL_only_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                        E_cal_VS_theta_p2_QEL_only_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                    }

                } else { // If Proton_2_ind_2p is the leading proton and Proton_1_ind_2p is the recoil

                    P_L_2p = P_p2_2p; // Leading proton
                    P_L_hist_2p->Fill(P_L_2p);

                    P_R_2p = P_p1_2p; // Recoil proton
                    P_R_hist_2p->Fill(P_R_2p);

                    double phi_p2 = particles[Proton_1_ind_2p]->getPhi() * 180.0 / 3.14159265359; // Leading proton phi (in deg)
                    phi_p1_2p->Fill(phi_p2);

                    double phi_p1 = particles[Proton_2_ind_2p]->getPhi() * 180.0 / 3.14159265359; // Recoil proton phi (in deg)
                    phi_p2_2p->Fill(phi_p1);

                    double d_phi_p2 = phi_p1 - phi_p2; // In deg
                    dphi_2p->Fill(d_phi_p2);

                    double theta_p2 = particles[Proton_1_ind_2p]->getTheta() * 180.0 / 3.14159265359; // Leading proton scattering angle theta (in deg)
                    Theta_p1_histogram->Fill(theta_p2);

                    double theta_p1 = particles[Proton_2_ind_2p]->getTheta() * 180.0 / 3.14159265359; // Recoil proton scattering angle theta (in deg)
                    Theta_p2_histogram->Fill(theta_p1);

                    //
//                    if (qel == true) {
//                        E_cal_VS_theta_p2_QEL_only_2p->Fill(theta_p1 * 180.0 / 3.14159265359, E_cal_2p);
//                        E_cal_VS_theta_p1_QEL_only_2p->Fill(theta_p2 * 180.0 / 3.14159265359, E_cal_2p);
//                    }

                } // end of P_L & P_R selector
                //</editor-fold>

                P_lp_hist_2p->Fill(P_lp_2p); // Lepton momentum

//                E_Trans_VS_q3_all_2p->Fill(q3, Ev - El);

//                double El_2p, El1_2p, El2_2p;
//                if (theta_lp_2p <= 40 && theta_lp_2p >= 5) {
//                    El1_2p = particles[Proton_1_ind_2p]->sci(FTOF1A)->getEnergy() +
//                             particles[Proton_1_ind_2p]->sci(FTOF1B)->getEnergy() +
//                             particles[Proton_1_ind_2p]->sci(FTOF1B)->getEnergy() +
//                             particles[Proton_1_ind_2p]->sci(PCAL)->getEnergy() +
//                             particles[Proton_1_ind_2p]->sci(ECIN)->getEnergy() +
//                             particles[Proton_1_ind_2p]->sci(ECOUT)->getEnergy();
//
//                    El2_2p = particles[Proton_2_ind_2p]->sci(FTOF1A)->getEnergy() +
//                             particles[Proton_2_ind_2p]->sci(FTOF1B)->getEnergy() +
//                             particles[Proton_2_ind_2p]->sci(FTOF1B)->getEnergy() +
//                             particles[Proton_2_ind_2p]->sci(PCAL)->getEnergy() +
//                             particles[Proton_2_ind_2p]->sci(ECIN)->getEnergy() +
//                             particles[Proton_2_ind_2p]->sci(ECOUT)->getEnergy();
//
//                }
//
//                El_2p = El1_2p + El2_2p;
//                fsEl_2p->Fill(El_2p);
//                cout << "El_2p (E_cal) = El1_2p + El2_2p = " << El1_2p << " + " << El2_2p << " = " << El_2p << "\n\n";

//                double El_2p = particles[Lepton_ind_2p]->cal(FTOF1A)->getEnergy() +
//                               particles[Lepton_ind_2p]->cal(FTOF1B)->getEnergy() +
//                               particles[Lepton_ind_2p]->cal(FTOF2)->getEnergy() +
//                               particles[Lepton_ind_2p]->cal(PCAL)->getEnergy() +
//                               particles[Lepton_ind_2p]->cal(ECIN)->getEnergy() +
//                               particles[Lepton_ind_2p]->cal(ECOUT)->getEnergy();
//                fsEl_2p->Fill(El_2p);
//                cout << "El_2p = " << El_2p << "\n\n";

                fsEl_2p->Fill(particles[Lepton_ind_2p]->getDeltaEnergy());
//                cout << "particles[" << Lepton_ind_2p << "]->getDeltaEnergy() = " << particles[Lepton_ind_2p]->getDeltaEnergy() << "\n\n";
//                cout << "particles[" << Lepton_ind_2p << "]->getEnergy() = " << particles[Lepton_ind_2p]->getEnergy() << "\n\n";



//                fsEl_VS_theta_lp_all_int_2p->Fill(Theta_lp_histogram, El);

//                E_Trans_all_ang_all_int_2p->Fill(Ev - El);

//                E_cal_VS_theta_lp_all_int_2p->Fill(Theta_lp_histogram, E_cal_2p);
//                E_cal_VS_Q2_all_int_2p->Fill(Q2, E_cal_2p);
//                E_cal_VS_dtheta_all_int_2p->Fill(fabs(acos(pzf[Proton_1_ind_2p] / rCalc(pxf[Proton_1_ind_2p], pyf[Proton_1_ind_2p], pzf[Proton_1_ind_2p])) -
//                                                      acos(pzf[Proton_2_ind_2p] / rCalc(pxf[Proton_2_ind_2p], pyf[Proton_2_ind_2p], pzf[Proton_2_ind_2p]))) *
//                                                 180.0 / 3.14159265359, E_cal_2p);

//                gamma_Lab_all_hist->Fill(cos(d_theta_2p));
//                gamma_Lab_all_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);

//                if (Theta_lp_histogram >= 14.0 && Theta_lp_histogram <= 16.0) {
//                    E_Trans15_all_2p->Fill(Ev - El);
//                } else if (Theta_lp_histogram >= 44.0 && Theta_lp_histogram <= 46.0) {
//                    E_Trans45_all_2p->Fill(Ev - El);
//                } else if (Theta_lp_histogram >= 89.0 && Theta_lp_histogram <= 91.0) {
//                    E_Trans90_all_2p->Fill(Ev - El);
//                }

//                //<editor-fold desc="Histogram fill by reaction (2p)">
//                if (qel == true) {
//                    gamma_Lab_QEL_hist->Fill(cos(d_theta_2p));
//                    gamma_Lab_QEL_hist_weighted->Fill(cos(d_theta_2p), Q2 * Q2);
//
//                    if (Theta_lp_histogram >= 14.0 && Theta_lp_histogram <= 16.0) {
//                        E_Trans15_QEL_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 44.0 && Theta_lp_histogram <= 46.0) {
//                        E_Trans45_QEL_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 89.0 && Theta_lp_histogram <= 91.0) {
//                        E_Trans90_QEL_2p->Fill(Ev - El);
//                    }
//
//                    fsEl_VS_theta_lp_QEL_only_2p->Fill(Theta_lp_histogram, El);
//
//                    E_cal_QEL_2p->Fill(E_cal_2p);
//                    E_cal_VS_theta_lp_QEL_only_2p->Fill(Theta_lp_histogram, E_cal_2p);
//                    E_cal_VS_Q2_QEL_only_2p->Fill(Q2, E_cal_2p);
//                    E_cal_VS_W_QEL_only_2p->Fill(W, E_cal_2p);
//                    E_cal_VS_En_QEL_only_2p->Fill(En, E_cal_2p);
//                    E_cal_VS_Pn_QEL_only_2p->Fill(sqrt(pxn * pxn + pyn * pyn + pzn * pzn), E_cal_2p);
//                    E_cal_VS_Pn1_QEL_only_2p->Fill(P_lp_2p, E_cal_2p);
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
//                    if (Theta_lp_histogram >= 14.0 && Theta_lp_histogram <= 16.0) {
//                        E_Trans15_MEC_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 44.0 && Theta_lp_histogram <= 46.0) {
//                        E_Trans45_MEC_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 89.0 && Theta_lp_histogram <= 91.0) {
//                        E_Trans90_MEC_2p->Fill(Ev - El);
//                    }
//
//                    fsEl_VS_theta_lp_MEC_only_2p->Fill(Theta_lp_histogram, El);
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
//                    if (Theta_lp_histogram >= 14.0 && Theta_lp_histogram <= 16.0) {
//                        E_Trans15_RES_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 44.0 && Theta_lp_histogram <= 46.0) {
//                        E_Trans45_RES_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 89.0 && Theta_lp_histogram <= 91.0) {
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
//                    if (Theta_lp_histogram >= 14.0 && Theta_lp_histogram <= 16.0) {
//                        E_Trans15_DIS_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 44.0 && Theta_lp_histogram <= 46.0) {
//                        E_Trans45_DIS_2p->Fill(Ev - El);
//                    } else if (Theta_lp_histogram >= 89.0 && Theta_lp_histogram <= 91.0) {
//                        E_Trans90_DIS_2p->Fill(Ev - El);
//                    }
//
//                    E_cal_DIS_2p->Fill(E_cal_2p);
//
//                    fsEl_DIS_2p->Fill(El);
//                }
//                //</editor-fold>

//            }
            } // end of momentum cut if (2p)

        } // end of 2p if
        //</editor-fold>


//  1n1p calculations
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1n1p calculations">
        if (calculate_1n1p && protons.size() == 1 && neutrons.size() == 1 && piplus.size() == 0 && piminus.size() == 0) {
            ++num_of_1n1p_events;

            if (selection_test_1n1p) {
                cout << "==========================================================================\n";
                cout << "num_of_1n1p_events = " << num_of_1n1p_events << "\n";
                cout << "\n#electrons = " << electrons.size() << ", #protons = " << protons.size() << ", #neutrons = " << neutrons.size() << "\n\n\n";
            } // end of selection test if (1n1p)

            int ProtonCounter_1n1p = 0, NeutronCounter_1n1p = 0, OtherParticleCounter_1n1p = 0;
            int Lepton_ind_1n1p = -1, Proton_ind_1n1p = -1, Neutron_ind_1n1p = -1;

            for (int i = 0; i < particles.size(); i++) {

                float particlePDG_1n1p = particles[i]->par()->getPid();

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
                        cout << "\nAdditional particles detected (1n1p). PDG = " << particlePDG_1n1p << "\n\n\n";
                    }
                }
                //</editor-fold>

            }

            double P_lp_1n1p = particles[Lepton_ind_1n1p]->getP(); // Lepton momentum
            double P_p_1n1p = particles[Proton_ind_1n1p]->getP(); // Proton momentum
            double P_n_1n1p = particles[Neutron_ind_1n1p]->getP(); // Neutron momentum

//          Momentum cut to at least 300 [MeV/c] == 0.3 [GeV/c]:
            if (P_p_1n1p >= 0 && P_n_1n1p >= 0) {
//                if (P_p_1n1p >= P_p_lower_lim_1n1p && P_n_1n1p >= P_n_lower_lim_1n1p) {

//                    double E_cal_1n1p;

//                    if (BEnergyToNucleusCon == true) {
//                        E_cal_1n1p = El + (Ef[Proton_ind_1n1p] - 0.938272) + (Ef[Neutron_ind_1n1p] - 0.939565) + 2 * BEnergyToNucleus;
//                    } else if (BEnergyToNucleusCon == false) {
//                        E_cal_1n1p = El + (Ef[Proton_ind_1n1p] - 0.938272) + (Ef[Neutron_ind_1n1p] - 0.939565);
//                    }

                //<editor-fold desc="Lepton theta & phi">
                double Phi_lp_1n1p = particles[Lepton_ind_1n1p]->getPhi() * 180.0 / 3.14159265359; // Phi of lepton in particles (in deg)
//                double Phi_lp_1n1p = atan2(particles[Lepton_ind_1n1p]->par()->getPy(), particles[Lepton_ind_1n1p]->par()->getPx())
//                                    * 180.0 / 3.14159265359; // Phi of lepton in particles (in deg)
                phi_lp_1n1p->Fill(Phi_lp_1n1p);

                double Theta_lp_1n1p = particles[Lepton_ind_1n1p]->getTheta() * 180.0 / 3.14159265359; // Theta of lepton in particles (in deg)
                theta_lp_1n1p->Fill(Theta_lp_1n1p);
                //</editor-fold>

                //<editor-fold desc="Nucleon theta & phi">
                double phi_p = atan2(particles[Proton_ind_1n1p]->par()->getPy(), particles[Proton_ind_1n1p]->par()->getPx())
                               * 180.0 / 3.14159265359; // Phi of proton in particles (in deg)
                phi_p_1n1p->Fill(phi_p);

                double phi_n = atan2(particles[Neutron_ind_1n1p]->par()->getPy(), particles[Neutron_ind_1n1p]->par()->getPx())
                               * 180.0 / 3.14159265359; // Phi of neutron in particles (in deg)
                phi_n_1n1p->Fill(phi_n);

                double d_phi_1n1p = phi_p - phi_n; // In deg
                dphi_1n1p->Fill(d_phi_1n1p);

                double theta_p = particles[Proton_ind_1n1p]->getTheta() * 180.0 / 3.14159265359; // Theta of proton in particles (in deg)
                theta_p_1n1p->Fill(theta_p);

                double theta_n = particles[Neutron_ind_1n1p]->getTheta() * 180.0 / 3.14159265359; // Theta of neutron in particles (in deg)
                theta_n_1n1p->Fill(theta_n);

//              ***NOT REALLY dtheta:
                double d_theta_1n1p = acos(
                        (particles[Proton_ind_1n1p]->par()->getPx() * particles[Neutron_ind_1n1p]->par()->getPx() +
                         particles[Proton_ind_1n1p]->par()->getPx() * particles[Neutron_ind_1n1p]->par()->getPx() +
                         particles[Proton_ind_1n1p]->par()->getPx() * particles[Neutron_ind_1n1p]->par()->getPx()) /
                        (particles[Proton_ind_1n1p]->getP() * particles[Neutron_ind_1n1p]->getP())) * 180.0 / 3.14159265359;
                dtheta_1n1p->Fill(d_theta_1n1p);
                //</editor-fold>

//                    E_Trans_VS_q3_all_1n1p->Fill(q3, Ev - El);

                //<editor-fold desc="Momentum histograms fill (1n1p)">
                P_p_hist_1n1p->Fill(P_p_1n1p);
                P_n_hist_1n1p->Fill(P_n_1n1p);
                P_lp_hist_1n1p->Fill(P_lp_1n1p);
                //</editor-fold>

//                double El_1n1p, Elp_1n1p, Eln_1n1p;
//                if (Theta_lp_1n1p <= 40 && Theta_lp_1n1p >= 5) {
//                    Elp_1n1p = particles[Proton_ind_1n1p]->sci(FTOF1A)->getEnergy() +
//                               particles[Proton_ind_1n1p]->sci(FTOF1B)->getEnergy() +
//                               particles[Proton_ind_1n1p]->sci(FTOF1B)->getEnergy() +
//                               particles[Proton_ind_1n1p]->sci(PCAL)->getEnergy() +
//                               particles[Proton_ind_1n1p]->sci(ECIN)->getEnergy() +
//                               particles[Proton_ind_1n1p]->sci(ECOUT)->getEnergy();
//
//                    Eln_1n1p = particles[Neutron_ind_1n1p]->sci(FTOF1A)->getEnergy() +
//                               particles[Neutron_ind_1n1p]->sci(FTOF1B)->getEnergy() +
//                               particles[Neutron_ind_1n1p]->sci(FTOF1B)->getEnergy() +
//                               particles[Neutron_ind_1n1p]->sci(PCAL)->getEnergy() +
//                               particles[Neutron_ind_1n1p]->sci(ECIN)->getEnergy() +
//                               particles[Neutron_ind_1n1p]->sci(ECOUT)->getEnergy();
//
//                }
//
//                El_1n1p = Elp_1n1p + Eln_1n1p;
//                fsEl_1n1p->Fill(El_1n1p);
//                cout << "El_1n1p (E_cal) = Elp_1n1p + Eln_1n1p = " << Elp_1n1p << " + " << Eln_1n1p << " = " << El_1n1p << "\n\n";


//                fsEl_1n1p->Fill(particles[Lepton_ind_1n1p]->getDeltaEnergy());

                /*
                //<editor-fold desc="Energy histograms fill (1n1p)">
                fsEl_VS_theta_lp_all_int_1n1p->Fill(Theta_lp_1n1p, El);
                fsEl_1n1p->Fill(El);
                //</editor-fold>

                E_Trans_all_ang_all_int_1n1p->Fill(Ev - El);

                if (Theta_lp_1n1p >= 14.0 && Theta_lp_1n1p <= 16.0) {
                    E_Trans15_all_1n1p->Fill(Ev - El);
                } else if (Theta_lp_1n1p >= 44.0 && Theta_lp_1n1p <= 46.0) {
                    E_Trans45_all_1n1p->Fill(Ev - El);
                } else if (Theta_lp_1n1p >= 89.0 && Theta_lp_1n1p <= 91.0) {
                    E_Trans90_all_1n1p->Fill(Ev - El);
                }

                //<editor-fold desc="Histogram fill by reaction (1n1p)">
                if (qel == true) {
                    if (Theta_lp_1n1p >= 14.0 && Theta_lp_1n1p <= 16.0) {
                        E_Trans15_QEL_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 44.0 && Theta_lp_1n1p <= 46.0) {
                        E_Trans45_QEL_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 89.0 && Theta_lp_1n1p <= 91.0) {
                        E_Trans90_QEL_1n1p->Fill(Ev - El);
                    }

                    fsEl_VS_theta_lp_QEL_only_1n1p->Fill(Theta_lp_1n1p, El);

                    E_cal_QEL_1n1p->Fill(E_cal_1n1p);
                    E_cal_VS_theta_lp_QEL_1n1p->Fill(Theta_lp_1n1p, E_cal_1n1p);
                    E_cal_VS_Q2_QEL_only_1n1p->Fill(Q2, E_cal_1n1p);
                    E_cal_VS_dtheta_QEL_only_1n1p->Fill(d_theta_1n1p, E_cal_1n1p);
                    E_cal_VS_theta_p_QEL_only_1n1p->Fill(theta_p, E_cal_1n1p);
                    E_cal_VS_theta_n_QEL_only_1n1p->Fill(theta_n, E_cal_1n1p);
                    E_cal_VS_W_QEL_only_1n1p->Fill(W, E_cal_1n1p);
                    E_cal_VS_En_QEL_only_1n1p->Fill(En, E_cal_1n1p);
                    E_cal_VS_Pn_QEL_only_1n1p->Fill(sqrt(pxn * pxn + pyn * pyn + pzn * pzn), E_cal_1n1p);
                    E_cal_VS_P_n_QEL_only_1n1p->Fill(P_p_1n1p, E_cal_1n1p);
                    E_cal_VS_P_p_QEL_only_1n1p->Fill(P_n_1n1p, E_cal_1n1p);

                    E_Trans_VS_q3_QEL_1n1p->Fill(q3, Ev - El);

                    fsEl_QEL_1n1p->Fill(El);
                } else if (mec == true) {
                    if (Theta_lp_1n1p >= 14.0 && Theta_lp_1n1p <= 16.0) {
                        E_Trans15_MEC_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 44.0 && Theta_lp_1n1p <= 46.0) {
                        E_Trans45_MEC_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 89.0 && Theta_lp_1n1p <= 91.0) {
                        E_Trans90_MEC_1n1p->Fill(Ev - El);
                    }

                    fsEl_VS_theta_lp_MEC_only_1n1p->Fill(Theta_lp_1n1p, El);

                    E_cal_MEC_1n1p->Fill(E_cal_1n1p);

                    E_Trans_VS_q3_MEC_1n1p->Fill(q3, Ev - El);

                    fsEl_MEC_1n1p->Fill(El);
                } else if (res == true) {
                    if (Theta_lp_1n1p >= 14.0 && Theta_lp_1n1p <= 16.0) {
                        E_Trans15_RES_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 44.0 && Theta_lp_1n1p <= 46.0) {
                        E_Trans45_RES_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 89.0 && Theta_lp_1n1p <= 91.0) {
                        E_Trans90_RES_1n1p->Fill(Ev - El);
                    }

                    E_cal_RES_1n1p->Fill(E_cal_1n1p);

                    fsEl_RES_1n1p->Fill(El);
                } else if (dis == true) {
                    if (Theta_lp_1n1p >= 14.0 && Theta_lp_1n1p <= 16.0) {
                        E_Trans15_DIS_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 44.0 && Theta_lp_1n1p <= 46.0) {
                        E_Trans45_DIS_1n1p->Fill(Ev - El);
                    } else if (Theta_lp_1n1p >= 89.0 && Theta_lp_1n1p <= 91.0) {
                        E_Trans90_DIS_1n1p->Fill(Ev - El);
                    }

                    E_cal_DIS_1n1p->Fill(E_cal_1n1p);

                    fsEl_DIS_1n1p->Fill(El);
                }
                //</editor-fold>
                */

            } // end of momentum cut if (1n1p)
        } // end of 1n1p if
        //</editor-fold>


//  MicroBooNE calculations
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="MicroBooNE calculations">
        if (calculate_MicroBooNE == true && protons.size() == 2) { // 2p with no pi0 (according to "no neutral pions of any momentum" and "any number of neutrons")
            ++num_of_MicroBooNE_events;

            int ProtonCounter_MicroBooNE = 0, OtherParticleCounter_MicroBooNE = 0;
            int Lepton_ind_MicroBooNE = -1, Proton_1_ind_MicroBooNE = -1, Proton_2_ind_MicroBooNE = -1;

            for (int i = 0; i < particles.size(); i++) {

                float particlePDG_MicroBooNE = particles[i]->par()->getPid();

                if (selection_test_MicroBooNE) {
                    cout << "particlePDG_MicroBooNE[" << i << "] = " << particlePDG_MicroBooNE << "\n";
                } // end of selection test if (MicroBooNE)

                //<editor-fold desc="Selector (MicroBooNE)">
                if (particlePDG_MicroBooNE == 2212) {
                    ++ProtonCounter_MicroBooNE;
//                    cout << "particlePDG_MicroBooNE[" << i << "] = " << particlePDG_MicroBooNE << "\n";
//                    cout << "i = " << i << "\n";
                    if (ProtonCounter_MicroBooNE == 1) {
                        Proton_1_ind_MicroBooNE = i;
//                            cout << "Proton_1_ind_MicroBooNE = " << Proton_1_ind_MicroBooNE << "\n";
                    } else if (ProtonCounter_MicroBooNE == 2) {
                        Proton_2_ind_MicroBooNE = i;
//                            cout << "Proton_2_ind_MicroBooNE = " << Proton_2_ind_MicroBooNE << "\n";
                    }
                } else if (particlePDG_MicroBooNE == 11) {
                    Lepton_ind_MicroBooNE = i;
                } // end of selector (MicroBooNE)
                //</editor-fold>

            } // end of loop over particles vector

            double Plx = particles[Lepton_ind_MicroBooNE]->par()->getPx();
            double Ply = particles[Lepton_ind_MicroBooNE]->par()->getPy();
            double Plz = particles[Lepton_ind_MicroBooNE]->par()->getPz();

//          Lepton (muon) momentum modulus:
            double P_lp_f = sqrt(Plx * Plx + Ply * Ply + Plz * Plz);

//          Leading proton (according to "the proton with the most momentum is labeled as the leading proton") momentum modulus:
            double P_L = fmax(rCalc(particles[Proton_1_ind_MicroBooNE]->par()->getPx(), particles[Proton_1_ind_MicroBooNE]->par()->getPy(),
                                    particles[Proton_1_ind_MicroBooNE]->par()->getPz()),
                              rCalc(particles[Proton_2_ind_MicroBooNE]->par()->getPx(), particles[Proton_2_ind_MicroBooNE]->par()->getPy(),
                                    particles[Proton_2_ind_MicroBooNE]->par()->getPz()));
//            double P_L = fmax(rCalc(pxf[Proton_1_ind_article], pyf[Proton_1_ind_article], pzf[Proton_1_ind_article]),
//                              rCalc(pxf[Proton_2_ind_article], pyf[Proton_2_ind_article], pzf[Proton_2_ind_article]));

//          Recoil proton (according to "the secondary proton is labeled as the recoil proton") momentum modulus:
            double P_R = fmin(rCalc(particles[Proton_1_ind_MicroBooNE]->par()->getPx(), particles[Proton_1_ind_MicroBooNE]->par()->getPy(),
                                    particles[Proton_1_ind_MicroBooNE]->par()->getPz()),
                              rCalc(particles[Proton_2_ind_MicroBooNE]->par()->getPx(), particles[Proton_2_ind_MicroBooNE]->par()->getPy(),
                                    particles[Proton_2_ind_MicroBooNE]->par()->getPz()));
//            double P_R = fmin(rCalc(pxf[Proton_1_ind_article], pyf[Proton_1_ind_article], pzf[Proton_1_ind_article]),
//                              rCalc(pxf[Proton_2_ind_article], pyf[Proton_2_ind_article], pzf[Proton_2_ind_article]));

            if ((P_lp_f >= P_lp_lower_lim_MicroBooNE && P_lp_f <= P_lp_upper_lim_MicroBooNE)
                && (P_L >= P_lp_lower_lim_MicroBooNE && P_L <= P_lp_upper_lim_MicroBooNE)
                && (P_R >= P_R_lower_lim_MicroBooNE && P_R <= P_R_upper_lim_MicroBooNE)) {

//              Calculating P_T:
                double P_T_x = Plx + particles[Proton_1_ind_MicroBooNE]->par()->getPx() + particles[Proton_2_ind_MicroBooNE]->par()->getPx(); // x component
                double P_T_y = Ply + particles[Proton_1_ind_MicroBooNE]->par()->getPy() + particles[Proton_2_ind_MicroBooNE]->par()->getPy(); // y component
                double P_T = sqrt(P_T_x * P_T_x + P_T_y * P_T_y);

                if (piplus.size() == 0 && piminus.size() == 0) { // In events without pions

//                  Calculating the total proton momentum vector:
                    double P_tot_x = particles[Proton_1_ind_MicroBooNE]->par()->getPx() + particles[Proton_2_ind_MicroBooNE]->par()->getPx(); // x component
                    double P_tot_y = particles[Proton_1_ind_MicroBooNE]->par()->getPy() + particles[Proton_2_ind_MicroBooNE]->par()->getPy(); // y component
                    double P_tot_z = particles[Proton_1_ind_MicroBooNE]->par()->getPz() + particles[Proton_2_ind_MicroBooNE]->par()->getPz(); // z component

//                  Total proton momentum modulus:
                    double P_tot = sqrt(P_tot_x * P_tot_x + P_tot_y * P_tot_y + P_tot_z * P_tot_z);

                    gamma_mu_p_tot->Fill((P_tot_x * Plx + P_tot_y * Ply + P_tot_z * Plz) / (P_tot * P_lp_f));
//                    gamma_mu_p_tot_weighted->Fill((P_tot_x * Plx + P_tot_y * Ply + P_tot_z * Plz) / (P_tot * P_lp_f), Q2 * Q2);


//                  Gamma_Lab calculations -------------------------------------------------

                    double P_p1 = rCalc(particles[Proton_1_ind_MicroBooNE]->par()->getPx(), particles[Proton_1_ind_MicroBooNE]->par()->getPy(),
                                        particles[Proton_1_ind_MicroBooNE]->par()->getPz());
                    double P_p2 = rCalc(particles[Proton_2_ind_MicroBooNE]->par()->getPx(), particles[Proton_2_ind_MicroBooNE]->par()->getPy(),
                                        particles[Proton_2_ind_MicroBooNE]->par()->getPz());

                    gamma_Lab_hist->Fill((particles[Proton_1_ind_MicroBooNE]->par()->getPx() * particles[Proton_2_ind_MicroBooNE]->par()->getPx() +
                                          particles[Proton_1_ind_MicroBooNE]->par()->getPy() * particles[Proton_2_ind_MicroBooNE]->par()->getPy() +
                                          particles[Proton_1_ind_MicroBooNE]->par()->getPz() * particles[Proton_2_ind_MicroBooNE]->par()->getPz()) / (P_p1 * P_p2));
//                    gamma_Lab_hist_weighted->Fill((particles[Proton_1_ind_MicroBooNE]->par()->getPx() * particles[Proton_2_ind_MicroBooNE]->par()->getPx() +
//                                                   particles[Proton_1_ind_MicroBooNE]->par()->getPy() * particles[Proton_2_ind_MicroBooNE]->par()->getPy() +
//                                                   particles[Proton_1_ind_MicroBooNE]->par()->getPz() * particles[Proton_2_ind_MicroBooNE]->par()->getPz()) /
//                                                  (P_p1 * P_p2), Q2 * Q2);
                    dP_T_hist->Fill(P_T);
//                    dP_T_hist_weighted->Fill(P_T, Q2 * Q2);

                    //<editor-fold desc="MicroBooNE momentum plots fill (no charged pions case)">
                    P_R_hist->Fill(P_R);
                    P_lp_hist->Fill(P_L);
                    P_lp_hist->Fill(P_lp_f);
                    //</editor-fold>

                } else { // In events with pions
                    for (int i = 0; i < particles.size(); i++) {

                        float particlePDG_MicroBooNE1 = particles[i]->par()->getPid();

                        if (abs(particlePDG_MicroBooNE1) == 211) { // The abs() for either pi+ or pi-
                            double P_pion = rCalc(particles[i]->par()->getPx(), particles[i]->par()->getPy(), particles[i]->par()->getPz());

//                          Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)"):
                            if (P_pion <= P_pion_upper_lim_MicroBooNE) {

//                              Gamma_mu,P_L+P_R calculations ------------------------------------------

//                              Calculating the total proton momentum vector:
                                double P_tot_x = particles[Proton_1_ind_MicroBooNE]->par()->getPx() + particles[Proton_2_ind_MicroBooNE]->par()->getPx(); // x component
                                double P_tot_y = particles[Proton_1_ind_MicroBooNE]->par()->getPy() + particles[Proton_2_ind_MicroBooNE]->par()->getPy(); // y component
                                double P_tot_z = particles[Proton_1_ind_MicroBooNE]->par()->getPz() + particles[Proton_2_ind_MicroBooNE]->par()->getPz(); // z component

//                              Total proton momentum modulus:
                                double P_tot = sqrt(P_tot_x * P_tot_x + P_tot_y * P_tot_y + P_tot_z * P_tot_z);

                                gamma_mu_p_tot->Fill((P_tot_x * Plx + P_tot_y * Ply + P_tot_z * Plz) / (P_tot * P_lp_f));
//                                gamma_mu_p_tot_weighted->Fill((P_tot_x * Plx + P_tot_y * Ply + P_tot_z * Plz) / (P_tot * P_lp_f), Q2 * Q2);


//                              Gamma_Lab calculations -------------------------------------------------

                                double P_p1 = rCalc(particles[Proton_1_ind_MicroBooNE]->par()->getPx(), particles[Proton_1_ind_MicroBooNE]->par()->getPy(),
                                                    particles[Proton_1_ind_MicroBooNE]->par()->getPz());
                                double P_p2 = rCalc(particles[Proton_2_ind_MicroBooNE]->par()->getPx(), particles[Proton_2_ind_MicroBooNE]->par()->getPy(),
                                                    particles[Proton_2_ind_MicroBooNE]->par()->getPz());

                                gamma_Lab_hist->Fill(
                                        (particles[Proton_1_ind_MicroBooNE]->par()->getPx() * particles[Proton_2_ind_MicroBooNE]->par()->getPx() +
                                         particles[Proton_1_ind_MicroBooNE]->par()->getPy() * particles[Proton_2_ind_MicroBooNE]->par()->getPy() +
                                         particles[Proton_1_ind_MicroBooNE]->par()->getPz() * particles[Proton_2_ind_MicroBooNE]->par()->getPz()) / (P_p1 * P_p2));
//                                gamma_Lab_hist_weighted->Fill(
//                                        (particles[Proton_1_ind_MicroBooNE]->par()->getPx() * particles[Proton_2_ind_MicroBooNE]->par()->getPx() +
//                                         particles[Proton_1_ind_MicroBooNE]->par()->getPy() * particles[Proton_2_ind_MicroBooNE]->par()->getPy() +
//                                         particles[Proton_1_ind_MicroBooNE]->par()->getPz() * particles[Proton_2_ind_MicroBooNE]->par()->getPz()) / (P_p1 * P_p2),
//                                        Q2 * Q2);
                                dP_T_hist->Fill(P_T);
//                                dP_T_hist_weighted->Fill(P_T, Q2 * Q2);

                                //<editor-fold desc="MicroBooNE momentum plots (with charged pions case)">
                                P_R_hist->Fill(P_R);
                                P_lp_hist->Fill(P_L);
                                P_lp_hist->Fill(P_lp_f);
                                P_pion_hist->Fill(P_pion);
                                //</editor-fold>

                            } // end of pion threshold if
                        } // end of abs if
                    } // end of for loop
                } // end of "with pions" if
            } // end of momentum threshold if
        } // end of MicroBooNE if
        //</editor-fold>

    } // end of while
    //</editor-fold>


//  Canvas definitions
// =======================================================================================================================================================================

    //<editor-fold desc="Canvas definitions">

//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1500, 1000); // original
//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1500, 1250);
//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1500, 1150);
    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
    c1->cd();
    c1->SetGrid();
    c1->SetBottomMargin(0.14);
//        c1->SetBottomMargin(0.1275); // original now
//    c1->SetBottomMargin(0.125);
//    c1->SetBottomMargin(0.115); // original

    if (wider_margin) {
        c1->SetLeftMargin(0.14);
//
//            c1->SetLeftMargin(0.1275);
//            c1->SetRightMargin(0.1275);
    }

    float DefStatX = gStyle->GetStatX();
    float DefStatY = gStyle->GetStatY();
//    cout << "\n\nStatX = "<< StatX << "\n\n";
//    cout << "\n\nStatY = "<< StatY << "\n\n";
    //</editor-fold>

//  Histograms plots
// =======================================================================================================================================================================

    //<editor-fold desc="Histograms plots">

//  Beta vs P histograms
//  ===================================================================================================

    if (Beta_vs_P_plots) {

        cout << "\n\nPlotting Beta vs P histograms...\n\n";

//  Beta vs P histograms ------------------------------------------------------------------------------

        Beta_vs_P_CD->SetTitleSize(0.06, "xyz");
        Beta_vs_P_CD->GetXaxis()->SetLabelSize(0.0425);
        Beta_vs_P_CD->GetXaxis()->CenterTitle(true);
        Beta_vs_P_CD->GetYaxis()->SetLabelSize(0.0425);
        Beta_vs_P_CD->GetYaxis()->CenterTitle(true);
        Beta_vs_P_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Beta_vs_P_CD);
//        Beta_vs_P_CD->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_lp_histogram_all_int_log_scale_2p.png");
        Beta_vs_P_CD->Draw("colz");
        c1->SetLogz(1);
        gStyle->SetStatX(0.88);
        gStyle->SetStatY(0.4);
        c1->SaveAs("plots/Beta_VS_p/Beta_vs_P_CD.png");
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();

        Beta_vs_P_FD->SetTitleSize(0.06, "xyz");
        Beta_vs_P_FD->GetXaxis()->SetLabelSize(0.0425);
        Beta_vs_P_FD->GetXaxis()->CenterTitle(true);
        Beta_vs_P_FD->GetYaxis()->SetLabelSize(0.0425);
        Beta_vs_P_FD->GetYaxis()->CenterTitle(true);
        Beta_vs_P_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Beta_vs_P_FD);
//        Beta_vs_P_FD->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_lp_histogram_all_int_log_scale_2p.png");
        Beta_vs_P_FD->Draw("colz");
        c1->SetLogz(1);
        gStyle->SetStatX(0.88);
        gStyle->SetStatY(0.4);
        c1->SaveAs("plots/Beta_VS_p/Beta_vs_P_FD.png");
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();

    }


//  Chi2 plots
//  ===================================================================================================

    if (Chi2_plots) {

        cout << "\n\nPlotting Chi2 plots...\n\n";

        //<editor-fold desc="Electron chi2">
        histPlotter1D(c1, Chi2_Electron_CD, normalized_chi2_plots, true, .1, "Electron #chi^{2}", "",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Chi2_Electron_Stack, "Electron_chi2", "plots/Chi2_plots/", "CD", kBlue, true, true, true, true);

        histPlotter1D(c1, Chi2_Electron_FD, normalized_chi2_plots, true, .1, "Electron #chi^{2}", "",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Chi2_Electron_Stack, "Electron_chi2", "plots/Chi2_plots/", "FD", kBlue, true, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Proton chi2">
        histPlotter1D(c1, Chi2_Proton_CD, normalized_chi2_plots, true, .1, "Proton #chi^{2}", "",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Chi2_Proton_Stack, "Proton_chi2", "plots/Chi2_plots/", "CD", kBlue, true, true, true, true);

        histPlotter1D(c1, Chi2_Proton_FD, normalized_chi2_plots, true, .1, "Proton #chi^{2}", "",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Chi2_Proton_Stack, "Proton_chi2", "plots/Chi2_plots/", "FD", kBlue, true, true, true, true);
        //</editor-fold>

    }


//  Theta histograms
//  ===================================================================================================

    if (Theta_plots) {

        cout << "\n\nPlotting Theta histograms...\n\n";

//  Theta of outgoing lepton histograms --------------------------------------------------------------

//      Normalization factor:
        double theta_lp_integral = Theta_lp_histogram->Integral() + theta_lp_1n1p->Integral();

        //<editor-fold desc="Theta of outgoing lepton histogram (2p)">
        histPlotter1D(c1, Theta_lp_histogram, normalized_theta_lp_plots, true, theta_lp_integral, "#theta_{l} of Outgoing Lepton", "All Interactions",
                      0.06, 0.0425, 0.0425, plots, 2, true, true, ThetaStack, "Theta_of_lepton", "plots/theta_histograms/", "2p", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Theta of outgoing lepton histogram (1n1p)">
        histPlotter1D(c1, theta_lp_1n1p, normalized_theta_lp_plots, true, theta_lp_integral, "#theta_{l} of Outgoing Lepton", "All Interactions",
                      0.06, 0.0425, 0.0425, plots, 2, true, true, ThetaStack, "Theta_of_lepton", "plots/theta_histograms/", "1n1p", kRed, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Theta of outgoing lepton histogram (stack)">
        ThetaStack->Draw("nostack");
        ThetaStack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        ThetaStack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        ThetaStack->GetHistogram()->GetXaxis()->CenterTitle(true);
        ThetaStack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

        if (normalized_theta_lp_plots) {
            ThetaStack->SetTitle("#theta_{l} of Outgoing Lepton (All Interactions, 2p and 1n1p) - Normalized");
            ThetaStack->GetYaxis()->SetTitle("Probability (%)");
            ThetaStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        } else {
            ThetaStack->GetYaxis()->SetTitle("Arbitrary units");
            ThetaStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        }

        auto ThetaStack_legend = new TLegend(0.75, 0.775, 0.875, 0.9);

        TLegendEntry *ThetaStack_entry_2p = ThetaStack_legend->AddEntry(Theta_lp_histogram, "2p", "l");
        TLegendEntry *ThetaStack_entry_1n1p = ThetaStack_legend->AddEntry(theta_lp_1n1p, "1n1p", "l");
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

        cout << "\n\nPlotting Phi histograms...\n\n";

//  Phi of outgoing lepton histogram ---------------------------------------------------------------------------

//      Normalization factor:
        double phi_lp_integral = phi_lp_2p->Integral() + phi_lp_1n1p->Integral();

        //<editor-fold desc="Phi of outgoing lepton histogram (2p)">
        histPlotter1D(c1, phi_lp_2p, normalized_phi_lp_plots, true, phi_lp_integral, "#phi_{l} of Outgoing Lepton", "All Interactions",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_lepton", "plots/phi_histograms/", "2p", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi of outgoing lepton histogram (1n1p)">
        histPlotter1D(c1, phi_lp_1n1p, normalized_phi_lp_plots, true, phi_lp_integral, "#phi_{l} of Outgoing Lepton", "All Interactions",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, PhiStack, "Phi_of_lepton", "plots/phi_histograms/", "1n1p", kRed, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi of outgoing lepton histogram (stack)">
        PhiStack->Draw("nostack");
        PhiStack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        PhiStack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        PhiStack->GetHistogram()->GetXaxis()->CenterTitle(true);
        PhiStack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

        auto PhiStack_legend = new TLegend(0.775, 0.775, 0.9, 0.9);
//        auto PhiStack_legend = new TLegend(0.8, 0.6, 0.9, 0.7);

        TLegendEntry *PhiStack_entry_2p = PhiStack_legend->AddEntry(phi_lp_2p, "2p", "l");
        TLegendEntry *PhiStack_entry_1n1p = PhiStack_legend->AddEntry(phi_lp_1n1p, "1n1p", "l");
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

        cout << "\n\nPlotting energy histograms...\n\n";

//  El histograms --------------------------------------------------------------------------------------

//      Normalization factor:
        double fsEl_integral = fsEl_2p->Integral() + fsEl_1n1p->Integral();

        //<editor-fold desc="El histograms (2p)">

        //<editor-fold desc="El histograms (all interactions, 2p)">
        histPlotter1D(c1, fsEl_2p, normalized_E_lp_all_int_plots, true, fsEl_integral, "Final State E_{l}", "All Interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, true, true, EnergyStack, "Final_State_El", "plots/Energy_histograms/El_histograms/all_interactions/", "2p", kBlue, true, true, true);
        //</editor-fold>

//        //<editor-fold desc="El histograms (QEL only, 2p)">
//        histPlotter1D(c1, fsEl_QEL_2p, normalized_E_lp_QEL_plots, true, fsEl_integral, "Final State E_{l}", "QEL Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, true, true, EnergyStack, "Final_State_El_QEL", "plots/Energy_histograms/El_histograms/QEL_only/", "2p", kBlue, true, false, true);
//        //</editor-fold>

//        //<editor-fold desc="El histograms (MEC only, 2p)">
//        histPlotter1D(c1, fsEl_MEC_2p, normalized_E_lp_MEC_plots, true, fsEl_integral, "Final State E_{l}", "MEC Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, true, true, EnergyStack, "Final_State_El_MEC", "plots/Energy_histograms/El_histograms/MEC_only/", "2p", kBlue, true, false, true);
//        //</editor-fold>

//        //<editor-fold desc="El histograms (RES only, 2p)">
//        histPlotter1D(c1, fsEl_RES_2p, normalized_E_lp_RES_plots, true, fsEl_integral, "Final State E_{l}", "RES Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, EnergyStack, "Final_State_El_RES", "plots/Energy_histograms/El_histograms/RES_only/", "2p", kBlue, true, false, true);
//        //</editor-fold>

//        //<editor-fold desc="El histograms (DIS, 2p)">
//        histPlotter1D(c1, fsEl_DIS_2p, normalized_E_lp_DIS_plots, true, fsEl_integral, "Final State E_{l}", "DIS Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, EnergyStack, "Final_State_El_DIS", "plots/Energy_histograms/El_histograms/DIS_only/", "2p", kBlue, true, false, true);
//        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="El histograms (1n1p)">

        //<editor-fold desc="El histograms (all interaction, 1n1p)">
        histPlotter1D(c1, fsEl_1n1p, normalized_E_lp_all_int_plots, true, fsEl_integral, "Final State E_{l}", "All Interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, true, true, EnergyStack, "Final_State_El", "plots/Energy_histograms/El_histograms/all_interactions/", "1n1p", kRed, true, true, true);
        //</editor-fold>

//        //<editor-fold desc="El histograms (QEL only, 1n1p)">
//        histPlotter1D(c1, fsEl_QEL_1n1p, normalized_E_lp_QEL_plots, true, fsEl_integral, "Final State E_{l}", "QEL Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, true, true, EnergyStack, "Final_State_El_QEL", "plots/Energy_histograms/El_histograms/QEL_only/", "1n1p", kRed, true, false, true);
//        //</editor-fold>

//        //<editor-fold desc="El histograms (MEC only, 1n1p)">
//        histPlotter1D(c1, fsEl_MEC_1n1p, normalized_E_lp_MEC_plots, true, fsEl_integral, "Final State E_{l}", "MEC Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, true, true, EnergyStack, "Final_State_El_MEC", "plots/Energy_histograms/El_histograms/MEC_only/", "1n1p", kRed, true, false, true);
//        //</editor-fold>

//        //<editor-fold desc="El histograms (RES only, 1n1p)">
//        histPlotter1D(c1, fsEl_RES_1n1p, normalized_E_lp_RES_plots, true, fsEl_integral, "Final State E_{l}", "RES Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, EnergyStack, "Final_State_El_RES", "plots/Energy_histograms/El_histograms/RES_only/", "1n1p", kRed, true, false, true);
//        //</editor-fold>

//        //<editor-fold desc="El histograms (DIS, 1n1p)">
//        histPlotter1D(c1, fsEl_DIS_1n1p, normalized_E_lp_DIS_plots, true, fsEl_integral, "Final State E_{l}", "DIS Only", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, EnergyStack, "Final_State_El_DIS", "plots/Energy_histograms/El_histograms/DIS_only/", "1n1p", kRed, true, false, true);
//        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="El histograms (all interactions, stack)">
        EnergyStack->Draw("nostack");
        EnergyStack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        EnergyStack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        EnergyStack->GetHistogram()->GetXaxis()->CenterTitle(true);
        EnergyStack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

        if (normalized_E_lp_plots) {
            EnergyStack->SetTitle("Final State E_{l} (All Interactions, 2p and 1n1p) - Normalized");
            EnergyStack->GetYaxis()->SetTitle("Probability (%)");
            EnergyStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        } else {
            EnergyStack->GetYaxis()->SetTitle("Arbitrary units");
            EnergyStack->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        }

        auto EnergyStack_legend = new TLegend(0.75, 0.775, 0.875, 0.9);

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

//        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (all interactions, 2p)">
//        double factor_El_VS_theta_lp_all_int_2p = 1.;
//
//        if (normalized_E_lp_plots) {
//            fsEl_VS_theta_lp_all_int_2p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 2p) - Normalized");
//            fsEl_VS_theta_lp_all_int_2p->Scale(factor_El_VS_theta_lp_all_int_2p / fsEl_VS_theta_lp_all_int_2p->Integral(), "width");
//            fsEl_VS_theta_lp_all_int_2p->Draw("colz");
//            fsEl_VS_theta_lp_all_int_2p->SetMaximum(set_Max_z);
//        } else {
//            fsEl_VS_theta_lp_all_int_2p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 2p)");
//            fsEl_VS_theta_lp_all_int_2p->Draw("colz");
////
//            fsEl_VS_theta_lp_all_int_2p->SetMaximum(set_Max_z);
////
//        }
//
//        fsEl_VS_theta_lp_all_int_2p->SetTitleSize(0.06, "xyz");
//        fsEl_VS_theta_lp_all_int_2p->GetXaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_all_int_2p->GetXaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_all_int_2p->GetYaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_all_int_2p->GetYaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_all_int_2p->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(fsEl_VS_theta_lp_all_int_2p);
//        fsEl_VS_theta_lp_all_int_2p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_lp_histogram_all_int_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_lp_histogram_all_int_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (QEL only, 2p)">
//        double factor_El_VS_theta_lp_QEL_only_2p = 1.;
//
//        if (normalized_E_lp_plots) {
//            fsEl_VS_theta_lp_QEL_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 2p) - Normalized");
//            fsEl_VS_theta_lp_QEL_only_2p->Scale(factor_El_VS_theta_lp_QEL_only_2p / fsEl_VS_theta_lp_QEL_only_2p->Integral(), "width");
//            fsEl_VS_theta_lp_QEL_only_2p->Draw("colz");
//            fsEl_VS_theta_lp_QEL_only_2p->SetMaximum(set_Max_z);
//        } else {
//            fsEl_VS_theta_lp_QEL_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 2p)");
//            fsEl_VS_theta_lp_QEL_only_2p->Draw("colz");
//            //
//            fsEl_VS_theta_lp_QEL_only_2p->SetMaximum(set_Max_z);
//            //
//        }
//
//        fsEl_VS_theta_lp_QEL_only_2p->SetTitleSize(0.06, "xyz");
//        fsEl_VS_theta_lp_QEL_only_2p->GetXaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_QEL_only_2p->GetYaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_QEL_only_2p->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(fsEl_VS_theta_lp_QEL_only_2p);
//        fsEl_VS_theta_lp_QEL_only_2p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_lp_histogram_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_lp_histogram_QEL_only_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (MEC only, 2p)">
//        double factor_El_VS_theta_lp_MEC_only_2p = 1.;
//
//        if (normalized_E_lp_plots) {
//            fsEl_VS_theta_lp_MEC_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 2p) - Normalized");
//            fsEl_VS_theta_lp_MEC_only_2p->Scale(factor_El_VS_theta_lp_MEC_only_2p / fsEl_VS_theta_lp_MEC_only_2p->Integral(), "width");
//            fsEl_VS_theta_lp_MEC_only_2p->Draw("colz");
//            fsEl_VS_theta_lp_MEC_only_2p->SetMaximum(set_Max_z);
//        } else {
//            fsEl_VS_theta_lp_MEC_only_2p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 2p)");
//            fsEl_VS_theta_lp_MEC_only_2p->Draw("colz");
//            //
//            fsEl_VS_theta_lp_MEC_only_2p->SetMaximum(set_Max_z);
//            //
//        }
//
//        fsEl_VS_theta_lp_MEC_only_2p->SetTitleSize(0.06, "xyz");
//        fsEl_VS_theta_lp_MEC_only_2p->GetXaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_MEC_only_2p->GetXaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_MEC_only_2p->GetYaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_MEC_only_2p->GetYaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_MEC_only_2p->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(fsEl_VS_theta_lp_MEC_only_2p);
//        fsEl_VS_theta_lp_MEC_only_2p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_lp_histogram_MEC_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_lp_histogram_MEC_only_linear_scale_2p.png");
//        c1->Clear();
//        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (1n1p)">

//        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (all interactions, 1n1p)">
//        double factor_El_VS_theta_lp_all_int_1n1p = 1.;
//
//        if (normalized_E_lp_plots) {
//            fsEl_VS_theta_lp_all_int_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 1n1p) - Normalized");
//            fsEl_VS_theta_lp_all_int_1n1p->Scale(factor_El_VS_theta_lp_all_int_1n1p / fsEl_VS_theta_lp_all_int_1n1p->Integral(), "width");
//            fsEl_VS_theta_lp_all_int_1n1p->Draw("colz");
//            fsEl_VS_theta_lp_all_int_1n1p->SetMaximum(set_Max_z);
//        } else {
//            fsEl_VS_theta_lp_all_int_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (all interactions, 1n1p)");
//            fsEl_VS_theta_lp_all_int_1n1p->Draw("colz");
//            //
//            fsEl_VS_theta_lp_all_int_1n1p->SetMaximum(set_Max_z);
//            //
//        }
//
//        fsEl_VS_theta_lp_all_int_1n1p->SetTitleSize(0.06, "xyz");
//        fsEl_VS_theta_lp_all_int_1n1p->GetXaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_all_int_1n1p->GetXaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_all_int_1n1p->GetYaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_all_int_1n1p->GetYaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_all_int_1n1p->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(fsEl_VS_theta_lp_all_int_1n1p);
//        fsEl_VS_theta_lp_all_int_1n1p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_lp_histogram_all_int_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/all_interactions/El_VS_theta_lp_histogram_all_int_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (QEL only, 1n1p)">
//        double factor_El_VS_theta_lp_QEL_only_1n1p = 1.;
//
//        if (normalized_E_lp_plots) {
//            fsEl_VS_theta_lp_QEL_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 1n1p) - Normalized");
//            fsEl_VS_theta_lp_QEL_only_1n1p->Scale(factor_El_VS_theta_lp_QEL_only_1n1p / fsEl_VS_theta_lp_QEL_only_1n1p->Integral(), "width");
//            fsEl_VS_theta_lp_QEL_only_1n1p->Draw("colz");
//            fsEl_VS_theta_lp_QEL_only_1n1p->SetMaximum(set_Max_z);
//        } else {
//            fsEl_VS_theta_lp_QEL_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (QEL only, 1n1p)");
//            fsEl_VS_theta_lp_QEL_only_1n1p->Draw("colz");
//            //
//            fsEl_VS_theta_lp_QEL_only_1n1p->SetMaximum(set_Max_z);
//            //
//        }
//
//        fsEl_VS_theta_lp_QEL_only_1n1p->SetTitleSize(0.06, "xyz");
//        fsEl_VS_theta_lp_QEL_only_1n1p->GetXaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_QEL_only_1n1p->GetXaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_QEL_only_1n1p->GetYaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_QEL_only_1n1p->GetYaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_QEL_only_1n1p->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(fsEl_VS_theta_lp_QEL_only_1n1p);
//        fsEl_VS_theta_lp_QEL_only_1n1p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_lp_histogram_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/QEL_only/El_VS_theta_lp_histogram_QEL_only_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="Final State E_{l} vs #theta_{l} histogram (MEC only, 1n1p)">
//        double factor_El_VS_theta_lp_MEC_only_1n1p = 1.;
//
//        if (normalized_E_lp_plots) {
//            fsEl_VS_theta_lp_MEC_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 1n1p) - Normalized");
//            fsEl_VS_theta_lp_MEC_only_1n1p->Scale(factor_El_VS_theta_lp_MEC_only_1n1p / fsEl_VS_theta_lp_MEC_only_1n1p->Integral(), "width");
//            fsEl_VS_theta_lp_MEC_only_1n1p->Draw("colz");
//            fsEl_VS_theta_lp_MEC_only_1n1p->SetMaximum(set_Max_z);
//        } else {
//            fsEl_VS_theta_lp_MEC_only_1n1p->SetTitle("Final State E_{l} vs #theta_{l} (MEC only, 1n1p)");
//            fsEl_VS_theta_lp_MEC_only_1n1p->Draw("colz");
//            //
//            fsEl_VS_theta_lp_MEC_only_1n1p->SetMaximum(set_Max_z);
//            //
//        }
//
//        fsEl_VS_theta_lp_MEC_only_1n1p->SetTitleSize(0.06, "xyz");
//        fsEl_VS_theta_lp_MEC_only_1n1p->GetXaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_MEC_only_1n1p->GetXaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_MEC_only_1n1p->GetYaxis()->SetLabelSize(0.0425);
//        fsEl_VS_theta_lp_MEC_only_1n1p->GetYaxis()->CenterTitle(true);
//        fsEl_VS_theta_lp_MEC_only_1n1p->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(fsEl_VS_theta_lp_MEC_only_1n1p);
//        fsEl_VS_theta_lp_MEC_only_1n1p->SetStats(0);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_lp_histogram_MEC_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/Energy_histograms/El_VS_theta_l/MEC_only/El_VS_theta_lp_histogram_MEC_only_linear_scale_1n1p.png");
//        c1->Clear();
//        //</editor-fold>

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
//        cout << "\n\nPlotting energy transfer histograms (all interactions)...\n\n";
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
//        cout << "\n\nPlotting energy transfer histograms (QEL only)...\n\n";
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
//        cout << "\n\nPlotting energy transfer histograms (MEC only)...\n\n";
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
//        cout << "\n\nPlotting energy transfer histograms (RES only)...\n\n";
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
//        cout << "\n\nPlotting energy transfer histograms (DIS only)...\n\n";
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
//        cout << "\n\nPlotting energy transfer stacks...\n\n";
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
//        cout << "\n\nPlotting E_cal restoration histograms...\n\n";
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
//        if (normalized_E_lp_plots) {
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
//        if (normalized_E_lp_plots) {
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
//        cout << "\n\nPlotting other E_cal restoration histograms...\n\n";
//
//        //<editor-fold desc="Other E_cal plots (all interactions, 2p)">
//        E_cal_VS_theta_lp_all_int_2p->Draw("colz");
//        plots->Add(E_cal_VS_theta_lp_all_int_2p);
//        E_cal_VS_theta_lp_all_int_2p->SetStats(0);
//        E_cal_VS_theta_lp_all_int_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_lp_all_int_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_theta_lp_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/2p/E_cal_vs_theta_lp_QEL_only_linear_scale_2p.png");
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
//        E_cal_VS_theta_lp_QEL_only_2p->Draw("colz");
//        plots->Add(E_cal_VS_theta_lp_QEL_only_2p);
//        E_cal_VS_theta_lp_QEL_only_2p->SetStats(0);
//        E_cal_VS_theta_lp_QEL_only_2p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_lp_QEL_only_2p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_lp_QEL_only_log_scale_2p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/2p/E_cal_vs_theta_lp_QEL_only_linear_scale_2p.png");
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
//        E_cal_VS_theta_lp_QEL_1n1p->Draw("colz");
//        plots->Add(E_cal_VS_theta_lp_QEL_1n1p);
//        E_cal_VS_theta_lp_QEL_1n1p->SetStats(0);
//        E_cal_VS_theta_lp_QEL_1n1p->GetXaxis()->CenterTitle(true);
//        E_cal_VS_theta_lp_QEL_1n1p->GetYaxis()->CenterTitle(true);
//        c1->SetLogz(1);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_1n1p_vs_theta_lp_QEL_only_log_scale_1n1p.png");
//        c1->SetLogz(0);
//        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/1n1p/E_cal_1n1p_vs_theta_lp_QEL_only_linear_scale_1n1p.png");
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
////        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/E_cal_vs_theta_lp_all_int_2p.png");
////        c1->Clear();
//
//
//// Theta_l - QEL only (2p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 2*0.938272:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.23)",
////                "nf==2 && nfn==0 && nfp == 2 && qel", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/E_cal_vs_theta_lp_QEL_only_2p.png");
////        c1->Clear();
//
//
//// Theta_l -all interactions (1n1p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 0.938272 - 0.939565:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.3)",
////                "nf==2 && nfn==1 && nfp == 1", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/all_interactions/E_cal_vs_theta_lp_all_int_1n1p.png");
////        c1->Clear();
//
//
//// Theta_l - QEL only (1n1p):
//
////        fChain->Draw(
////                "El + Ef[0] + Ef[1] - 0.938272 - 0.939565:acos(pzl / sqrt(pxl * pxl + pyl * pyl + pzl * pzl)) * 180.0 / 3.14159265359>>h2(100, 0.0, 120, 100, 2.12, 2.23)",
////                "nf==2 && nfn==1 && nfp == 1 && qel", "colz");
////        c1->SaveAs("plots/E_cal_restorations/Other/QEL_only/E_cal_vs_theta_lp_QEL_only_1n1p.png");
////        c1->Clear();
//        //</editor-fold>
//
//    }


// Momentum histograms
// ====================================================================================================

    if (momentum_plots) {

        cout << "\n\nPlotting momentum histograms for 2p and 1n1p...\n\n";

        //<editor-fold desc="Momentum histograms (2p)">
        histPlotter1D(c1, P_L_hist_2p, normalized_P_L_plots, false, 1., "Momentum Histogram of Leading Proton P_{L} = P_{p1}", "all interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, MomentumStack_2p, "P_L_histogram", "plots/momentum_histograms/2p/", "2p", kBlue, true, true, true);

        histPlotter1D(c1, P_R_hist_2p, normalized_P_R_plots, false, 1., "Momentum Histogram of Recoil Proton P_{R} = P_{p2}", "all interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, MomentumStack_2p, "P_R_histogram", "plots/momentum_histograms/2p/", "2p", kRed, true, true, true);

        histPlotter1D(c1, P_lp_hist_2p, normalized_P_R_plots, false, 1., "Momentum Histogram of Outgoing Lepton P_{l}", "all interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, MomentumStack_2p, "P_lp_histogram", "plots/momentum_histograms/2p/", "2p", kGreen, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Momentum histograms (1n1p)">
        histPlotter1D(c1, P_p_hist_1n1p, normalized_P_L_plots, false, 1., "Momentum Histogram of Scattered Proton P_{p}", "all interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, MomentumStack_1n1p, "P_p_histogram", "plots/momentum_histograms/1n1p/", "1n1p", kBlue, true, true, true);

        histPlotter1D(c1, P_n_hist_1n1p, normalized_P_R_plots, false, 1., "Momentum Histogram of Scattered Neutron P_{n}", "all interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, MomentumStack_1n1p, "P_n_histogram", "plots/momentum_histograms/1n1p/", "1n1p", kRed, true, true, true);

        histPlotter1D(c1, P_lp_hist_1n1p, normalized_P_R_plots, false, 1., "Momentum Histogram of Outgoing Lepton P_{l}", "all interactions", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, MomentumStack_1n1p, "P_lp_histogram", "plots/momentum_histograms/1n1p/", "1n1p", kGreen, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Momentum histogram stack (2p)">
        MomentumStack_2p->Draw("nostack");
        MomentumStack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        MomentumStack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        MomentumStack_2p->GetHistogram()->GetXaxis()->CenterTitle(true);
        MomentumStack_2p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        MomentumStack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
        MomentumStack_2p->GetHistogram()->GetYaxis()->CenterTitle(true);
        MomentumStack_2p->GetHistogram()->GetYaxis()->SetTitle("Arbitrary units");

        auto MomentumStack_2p_legend = new TLegend(0.775, 0.7, 0.85, 0.875);

        TLegendEntry *MomentumStack_2p_legend_P_L_entry = MomentumStack_2p_legend->AddEntry(P_L_hist_2p, "P_{L}", "l");
        TLegendEntry *MomentumStack_2p_legend_P_R_entry = MomentumStack_2p_legend->AddEntry(P_R_hist_2p, "P_{R}", "l");
        TLegendEntry *MomentumStack_2p_legend_P_lp_entry = MomentumStack_2p_legend->AddEntry(P_lp_hist_2p, "P_{l}", "l");

        MomentumStack_2p_legend->Draw();

        plots->Add(MomentumStack_2p);
        c1->SaveAs("plots/momentum_histograms/Momentum_stack_2p.png");
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="Momentum histogram stack (1n1p)">`
        MomentumStack_1n1p->Draw("nostack");
        MomentumStack_1n1p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        MomentumStack_1n1p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        MomentumStack_1n1p->GetHistogram()->GetXaxis()->CenterTitle(true);
        MomentumStack_1n1p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        MomentumStack_1n1p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
        MomentumStack_1n1p->GetHistogram()->GetYaxis()->CenterTitle(true);
        MomentumStack_1n1p->GetHistogram()->GetYaxis()->SetTitle("Arbitrary units");

        auto MomentumStack_1n1p_legend = new TLegend(0.775, 0.7, 0.85, 0.875);

        TLegendEntry *MomentumStack_1n1p_legend_P_p_entry = MomentumStack_1n1p_legend->AddEntry(P_p_hist_1n1p, "P_{p}", "l");
        TLegendEntry *MomentumStack_1n1p_legend_P_n_entry = MomentumStack_1n1p_legend->AddEntry(P_n_hist_1n1p, "P_{n}", "l");
        TLegendEntry *MomentumStack_1n1p_legend_P_lp_entry = MomentumStack_1n1p_legend->AddEntry(P_lp_hist_1n1p, "P_{l}", "l");

        MomentumStack_1n1p_legend->Draw();

        plots->Add(MomentumStack_1n1p);
        c1->SaveAs("plots/momentum_histograms/Momentum_stack_1n1p.png");
        c1->Clear();
        //</editor-fold>

    }


// MicroBooNE article histogram reconstructions
// ====================================================================================================

    if (MicroBooNE_plots) {

        cout << "\n\nPlotting MicroBooNE histograms...\n\n";

// Momentum plots -------------------------------------------------------------------------------------

        P_L_hist->Draw();
        plots->Add(P_L_hist);
        P_L_hist->SetLineWidth(2);
        P_L_hist->GetXaxis()->CenterTitle(true);
        P_L_hist->SetLineColor(kBlue);
        c1->SetLogy(1);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_L_histogram_log_scale.png");
        c1->SetLogy(0);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_L_histogram_linear_scale.png");
        c1->Clear();

        P_R_hist->Draw();
        plots->Add(P_R_hist);
        P_R_hist->SetLineWidth(2);
        P_R_hist->GetXaxis()->CenterTitle(true);
        P_R_hist->SetLineColor(kBlue);
        c1->SetLogy(1);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_R_histogram_log_scale.png");
        c1->SetLogy(0);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_R_histogram_linear_scale.png");
        c1->Clear();

        P_lp_hist->Draw();
        plots->Add(P_lp_hist);
        P_lp_hist->SetLineWidth(2);
        P_lp_hist->GetXaxis()->CenterTitle(true);
        P_lp_hist->SetLineColor(kBlue);
        c1->SetLogy(1);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_lp_histogram_log_scale.png");
        c1->SetLogy(0);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_lp_histogram_linear_scale.png");
        c1->Clear();

        P_pion_hist->Draw();
        plots->Add(P_pion_hist);
        P_pion_hist->SetLineWidth(2);
        P_pion_hist->GetXaxis()->CenterTitle(true);
        P_pion_hist->SetLineColor(kBlue);
        c1->SetLogy(1);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_pion_histogram_log_scale.png");
        c1->SetLogy(0);
        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_pion_histogram_linear_scale.png");
        c1->Clear();

// Unweighted plots -----------------------------------------------------------------------------

        gamma_Lab_hist->Draw();
        gamma_Lab_hist->SetTitleSize(0.06);
        gamma_Lab_hist->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_hist->GetXaxis()->CenterTitle(true);
        gamma_Lab_hist->GetYaxis()->SetLabelSize(0.0425);
//        gamma_Lab_hist->GetYaxis()->SetLimits(0., 3.);
        plots->Add(gamma_Lab_hist);
        gamma_Lab_hist->SetLineWidth(2);
        gamma_Lab_hist->SetLineColor(kBlue);
//        gamma_Lab_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_Lab.png");
        c1->Clear();

        gamma_mu_p_tot->Draw();
        gamma_mu_p_tot->SetTitleSize(0.06);
        gamma_mu_p_tot->GetXaxis()->SetLabelSize(0.0425);
        gamma_mu_p_tot->GetXaxis()->CenterTitle(true);
        gamma_mu_p_tot->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_mu_p_tot);
        gamma_mu_p_tot->SetLineWidth(2);
        gamma_mu_p_tot->SetLineColor(kBlue);
//        gamma_mu_p_tot->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_mu_p_tot.png");
        c1->Clear();

        dP_T_hist->Draw();
        dP_T_hist->SetTitleSize(0.06);
        dP_T_hist->GetXaxis()->SetLabelSize(0.0425);
        dP_T_hist->GetXaxis()->CenterTitle(true);
        dP_T_hist->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(dP_T_hist);
        dP_T_hist->SetLineWidth(2);
        dP_T_hist->SetLineColor(kBlue);
//        dP_T_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/dP_T_histogram.png");
        c1->Clear();

        gamma_Lab_all_hist->Draw();
        gamma_Lab_all_hist->SetTitleSize(0.06);
        gamma_Lab_all_hist->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_all_hist->GetXaxis()->CenterTitle(true);
        gamma_Lab_all_hist->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_all_hist);
        gamma_Lab_all_hist->SetLineWidth(2);
        gamma_Lab_all_hist->SetLineColor(kBlue);
//        gamma_Lab_all_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/all_interactions/gamma_Lab_all.png");
        c1->Clear();

        /*
        gamma_Lab_QEL_hist->Draw();
        gamma_Lab_QEL_hist->SetTitleSize(0.06);
        gamma_Lab_QEL_hist->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_QEL_hist->GetXaxis()->CenterTitle(true);
        gamma_Lab_QEL_hist->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_QEL_hist);
        gamma_Lab_QEL_hist->SetLineWidth(2);
        gamma_Lab_QEL_hist->SetLineColor(kBlue);
//        gamma_Lab_QEL_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/QEL_only/gamma_Lab_QEL.png");
        c1->Clear();

        gamma_Lab_MEC_hist->Draw();
        gamma_Lab_MEC_hist->SetTitleSize(0.06);
        gamma_Lab_MEC_hist->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_MEC_hist->GetXaxis()->CenterTitle(true);
        gamma_Lab_MEC_hist->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_MEC_hist);
        gamma_Lab_MEC_hist->SetLineWidth(2);
        gamma_Lab_MEC_hist->SetLineColor(kBlue);
//        gamma_Lab_MEC_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/MEC_only/gamma_Lab_MEC.png");
        c1->Clear();

        gamma_Lab_RES_hist->Draw();
        gamma_Lab_RES_hist->SetTitleSize(0.06);
        gamma_Lab_RES_hist->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_RES_hist->GetXaxis()->CenterTitle(true);
        gamma_Lab_RES_hist->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_RES_hist);
        gamma_Lab_RES_hist->SetLineWidth(2);
        gamma_Lab_RES_hist->SetLineColor(kBlue);
//        gamma_Lab_RES_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/RES_only/gamma_Lab_RES.png");
        c1->Clear();

        gamma_Lab_DIS_hist->Draw();
        gamma_Lab_DIS_hist->SetTitleSize(0.06);
        gamma_Lab_DIS_hist->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_DIS_hist->GetXaxis()->CenterTitle(true);
        gamma_Lab_DIS_hist->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_DIS_hist);
        gamma_Lab_DIS_hist->SetLineWidth(2);
        gamma_Lab_DIS_hist->SetLineColor(kBlue);
//        gamma_Lab_DIS_hist->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/DIS_only/gamma_Lab_DIS.png");
        c1->Clear();
        */

// Weighted plots -------------------------------------------------------------------------------

        gamma_Lab_hist_weighted->Draw();
        gamma_Lab_hist_weighted->Sumw2();
        gamma_Lab_hist_weighted->SetTitleSize(0.06);
        gamma_Lab_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_hist_weighted->GetXaxis()->CenterTitle(true);
        gamma_Lab_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_hist_weighted);
        gamma_Lab_hist_weighted->SetLineWidth(2);
        gamma_Lab_hist_weighted->SetLineColor(kBlue);
        gamma_Lab_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_Lab_weighted.png");
        c1->Clear();

        gamma_mu_p_tot_weighted->Draw();
        gamma_mu_p_tot_weighted->Sumw2();
        gamma_mu_p_tot_weighted->SetTitleSize(0.06);
        gamma_mu_p_tot_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_mu_p_tot_weighted->GetXaxis()->CenterTitle(true);
        gamma_mu_p_tot_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_mu_p_tot_weighted);
        gamma_mu_p_tot_weighted->SetLineWidth(2);
        gamma_mu_p_tot_weighted->SetLineColor(kBlue);
        gamma_mu_p_tot_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_mu_p_tot_weighted.png");
        c1->Clear();

        dP_T_hist_weighted->Draw();
        dP_T_hist_weighted->Sumw2();
        dP_T_hist_weighted->SetTitleSize(0.06);
        dP_T_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        dP_T_hist_weighted->GetXaxis()->CenterTitle(true);
        dP_T_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(dP_T_hist_weighted);
        dP_T_hist_weighted->SetLineWidth(2);
        dP_T_hist_weighted->SetLineColor(kBlue);
        dP_T_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/dP_T_histogram_weighted.png");
        c1->Clear();


        gamma_Lab_all_hist_weighted->Draw();
        gamma_Lab_all_hist_weighted->Sumw2();
        gamma_Lab_all_hist_weighted->SetTitleSize(0.06);
        gamma_Lab_all_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_all_hist_weighted->GetXaxis()->CenterTitle(true);
        gamma_Lab_all_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_all_hist_weighted);
        gamma_Lab_all_hist_weighted->SetLineWidth(2);
        gamma_Lab_all_hist_weighted->SetLineColor(kBlue);
//        gamma_Lab_all_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/all_interactions/gamma_Lab_all_weighted.png");
        c1->Clear();

        /*
        gamma_Lab_QEL_hist_weighted->Draw();
        gamma_Lab_QEL_hist_weighted->Sumw2();
        gamma_Lab_QEL_hist_weighted->SetTitleSize(0.06);
        gamma_Lab_QEL_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_QEL_hist_weighted->GetXaxis()->CenterTitle(true);
        gamma_Lab_QEL_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_QEL_hist_weighted);
        gamma_Lab_QEL_hist_weighted->SetLineWidth(2);
        gamma_Lab_QEL_hist_weighted->SetLineColor(kBlue);
//        gamma_Lab_QEL_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/QEL_only/gamma_Lab_QEL_weighted.png");
        c1->Clear();

        gamma_Lab_MEC_hist_weighted->Draw();
        gamma_Lab_MEC_hist_weighted->Sumw2();
        gamma_Lab_MEC_hist_weighted->SetTitleSize(0.06);
        gamma_Lab_MEC_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_MEC_hist_weighted->GetXaxis()->CenterTitle(true);
        gamma_Lab_MEC_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_MEC_hist_weighted);
        gamma_Lab_MEC_hist_weighted->SetLineWidth(2);
        gamma_Lab_MEC_hist_weighted->SetLineColor(kBlue);
//        gamma_Lab_MEC_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/MEC_only/gamma_Lab_MEC_weighted.png");
        c1->Clear();

        gamma_Lab_RES_hist_weighted->Draw();
        gamma_Lab_RES_hist_weighted->Sumw2();
        gamma_Lab_RES_hist_weighted->SetTitleSize(0.06);
        gamma_Lab_RES_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_RES_hist_weighted->GetXaxis()->CenterTitle(true);
        gamma_Lab_RES_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_RES_hist_weighted);
        gamma_Lab_RES_hist_weighted->SetLineWidth(2);
        gamma_Lab_RES_hist_weighted->SetLineColor(kBlue);
//        gamma_Lab_RES_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/RES_only/gamma_Lab_RES_weighted.png");
        c1->Clear();

        gamma_Lab_DIS_hist_weighted->Draw();
        gamma_Lab_DIS_hist_weighted->Sumw2();
        gamma_Lab_DIS_hist_weighted->SetTitleSize(0.06);
        gamma_Lab_DIS_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
        gamma_Lab_DIS_hist_weighted->GetXaxis()->CenterTitle(true);
        gamma_Lab_DIS_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(gamma_Lab_DIS_hist_weighted);
        gamma_Lab_DIS_hist_weighted->SetLineWidth(2);
        gamma_Lab_DIS_hist_weighted->SetLineColor(kBlue);
//        gamma_Lab_DIS_hist_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/DIS_only/gamma_Lab_DIS_weighted.png");
        c1->Clear();
        */

    }


// Inclusive Energy transfer histograms
// ====================================================================================================

    if (inclusive_plots) {

        cout << "\n\nPlotting inclusive histograms...\n\n";

//      Q2 histogram (inclusive) -----------------------------------------------------

        Q2_hist_inclusive->Draw();
        plots->Add(Q2_hist_inclusive);
        Q2_hist_inclusive->SetLineWidth(2);
        Q2_hist_inclusive->GetXaxis()->CenterTitle(true);
        Q2_hist_inclusive->SetLineColor(kBlue);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Q2_inclusive.png");
        Q2_hist_inclusive->SetLineWidth(4);
        Q2_hist_inclusive->SetLineStyle(2);
//        Q2_hist_inclusive->SetLineStyle(5);
        Q2_hist_inclusive->SetStats(0);
        c1->Clear();


//     all interactions (ET around theta_l = 15) -----------------------------------------------------

//      Normalization factor (equals to max(E_Trans15_all_inclusive)):
        auto factor_E_Trans_inclusive = E_Trans15_all_inclusive->GetMaximum();

        if (normalized_inclusive_plots) {
            E_Trans15_all_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
            E_Trans15_all_inclusive->Draw("HIST");
        } else {
            E_Trans15_all_inclusive->Draw();
        }

        plots->Add(E_Trans15_all_inclusive);
        E_Trans15_all_inclusive->SetLineWidth(2);
        E_Trans15_all_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans15_all_inclusive->SetLineColor(kBlack);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_all_interactions_liner_scale_inclusive.png");
        E_Trans15_all_inclusive->SetLineWidth(4);
        E_Trans15_all_inclusive->SetLineStyle(2);
//        E_Trans15_all_inclusive->SetLineStyle(5);
        E_Trans15_all_inclusive->SetStats(0);
        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_all_inclusive);
        c1->Clear();


//      QEL only (ET around theta_l = 15) -------------------------------------------------------------

        if (normalized_inclusive_plots) {
//        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
            E_Trans15_QEL_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
            E_Trans15_QEL_inclusive->Draw("HIST");
        } else {
            E_Trans15_QEL_inclusive->Draw();
        }

        plots->Add(E_Trans15_QEL_inclusive);
        E_Trans15_QEL_inclusive->SetLineWidth(2);
        E_Trans15_QEL_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans15_QEL_inclusive->SetLineColor(kBlue);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_QEL_only_liner_scale_inclusive.png");
        E_Trans15_QEL_inclusive->SetStats(0);
        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_QEL_inclusive);
        c1->Clear();


//      MEC only (ET around theta_l = 15) -------------------------------------------------------------

        if (normalized_inclusive_plots) {
//        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
            E_Trans15_MEC_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
            E_Trans15_MEC_inclusive->Draw("HIST");
        } else {
            E_Trans15_MEC_inclusive->Draw();
        }
        plots->Add(E_Trans15_MEC_inclusive);
        E_Trans15_MEC_inclusive->SetLineWidth(2);
        E_Trans15_MEC_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans15_MEC_inclusive->SetLineColor(kRed);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_MEC_only_liner_scale_inclusive.png");
        E_Trans15_MEC_inclusive->SetStats(0);
        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_MEC_inclusive);
        c1->Clear();


//      RES only (ET around theta_l = 15) -------------------------------------------------------------

        if (normalized_inclusive_plots) {
//        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
            E_Trans15_RES_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
            E_Trans15_RES_inclusive->Draw("HIST");
        } else {
            E_Trans15_RES_inclusive->Draw();
        }
        plots->Add(E_Trans15_RES_inclusive);
        E_Trans15_RES_inclusive->SetLineWidth(2);
        E_Trans15_RES_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans15_RES_inclusive->SetLineColor(kGreen);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_RES_only_liner_scale_inclusive.png");
        E_Trans15_RES_inclusive->SetStats(0);
        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_RES_inclusive);
        c1->Clear();


//      DIS only (ET around theta_l = 15) -------------------------------------------------------------

        if (normalized_inclusive_plots) {
//        if (normalized_inclusive_plots && (file_name == "12C_2222GeV_G18_10a_02_11a" || file_name == "12C_2222GeV_GTEST19_10b_00_000")) {
            E_Trans15_DIS_inclusive->Scale(1 / factor_E_Trans_inclusive, "nosw2");
            E_Trans15_DIS_inclusive->Draw("HIST");
        } else {
            E_Trans15_DIS_inclusive->Draw();
        }
        plots->Add(E_Trans15_DIS_inclusive);
        E_Trans15_DIS_inclusive->SetLineWidth(2);
        E_Trans15_DIS_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans15_DIS_inclusive->SetLineColor(kCyan);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_around_15_Deg_DIS_only_liner_scale_inclusive.png");
        E_Trans15_DIS_inclusive->SetStats(0);
        Energy_Transfer_all_int_15_inclusive_Stack->Add(E_Trans15_DIS_inclusive);
        c1->Clear();


//      Inclusive ET stack (around theta_l = 15) ------------------------------------------------------

        Energy_Transfer_all_int_15_inclusive_Stack->Draw("nostack");
        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetXaxis()->CenterTitle(true);
        Energy_Transfer_all_int_15_inclusive_Stack->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);

        if (normalized_inclusive_plots) {
            Energy_Transfer_all_int_15_inclusive_Stack->SetTitle(
                    "Energy Transfer (^{12}C(e,e')) in range 14#circ #leq #theta_{l} #leq 16#circ - Normalized");
        } else {
            Energy_Transfer_all_int_15_inclusive_Stack->SetTitle(
                    "Energy Transfer (E_{#nu}-E_{l}) in the Angle Range 14 #leq #theta_{l} #leq 16 (2p and 1n1p, ^{12}C(e,e'))");
        }

        auto E_Trans_15_inclusive_legend = new
                TLegend(0.625, 0.625, 0.9, 0.9);

        TLegendEntry *E_Trans_15_all_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_all_inclusive, "All interactions", "l");
        TLegendEntry *E_Trans_15_QEL_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_QEL_inclusive, "QEL only", "l");
        TLegendEntry *E_Trans_15_MEC_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_MEC_inclusive, "MEC only", "l");
        TLegendEntry *E_Trans_15_RES_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_RES_inclusive, "RES only", "l");
        TLegendEntry *E_Trans_15_DIS_entry_inclusive = E_Trans_15_inclusive_legend->AddEntry(E_Trans15_DIS_inclusive, "DIS only ", "l");

        E_Trans_15_inclusive_legend->Draw();

        plots->Add(Energy_Transfer_all_int_15_inclusive_Stack);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_histogram_15_Stack_liner_scale_inclusive.png");
        c1->Clear();


////      Normalization factor (equals to max(E_Trans_VS_q_all_inclusive)):
//        auto E_Trans_VS_q_all_inclusive = E_Trans_VS_q_all_inclusive->GetMaximum();

//      Normalization factor (equals to max(E_Trans_VS_q_all_inclusive)):
        double factor_E_Trans_VS_q_all_inclusive = 1.;

        if (normalized_E_lp_plots) {
            E_Trans_VS_q_all_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (All Interactions, ^{12}C(e,e')) - Normalized");
            E_Trans_VS_q_all_inclusive->Scale(factor_E_Trans_VS_q_all_inclusive / E_Trans_VS_q_all_inclusive->Integral(), "width");
            E_Trans_VS_q_all_inclusive->Draw("colz");
            E_Trans_VS_q_all_inclusive->SetMaximum(1.5);
        } else {
            E_Trans_VS_q_all_inclusive->Draw("colz");
        }

        E_Trans_VS_q_all_inclusive->SetTitleSize(0.06, "xyz");
        E_Trans_VS_q_all_inclusive->GetXaxis()->SetLabelSize(0.0425);
        E_Trans_VS_q_all_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q_all_inclusive->GetYaxis()->SetLabelSize(0.0425);
        E_Trans_VS_q_all_inclusive->GetYaxis()->CenterTitle(true);
        E_Trans_VS_q_all_inclusive->GetZaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_all_inclusive->Draw("colz");
        plots->Add(E_Trans_VS_q_all_inclusive);
        E_Trans_VS_q_all_inclusive->SetStats(0);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_all_inclusive_log_scale.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_all_inclusive_liner_scale.png");
        c1->Clear();

//      Normalization factor (equals to max(fsEl_VS_theta_lp_all_int_2p)):
        double factor_E_Trans_VS_q_QEL_inclusive = 1.;

        if (normalized_E_lp_plots) {
            E_Trans_VS_q_QEL_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (QEL Only, ^{12}C(e,e')) - Normalized");
            E_Trans_VS_q_QEL_inclusive->Scale(factor_E_Trans_VS_q_QEL_inclusive / E_Trans_VS_q_QEL_inclusive->Integral(), "width");
            E_Trans_VS_q_QEL_inclusive->Draw("colz");
            E_Trans_VS_q_QEL_inclusive->SetMaximum(1.5);
        } else {
            E_Trans_VS_q_QEL_inclusive->Draw("colz");
        }

        E_Trans_VS_q_QEL_inclusive->SetTitleSize(0.06, "xyz");
        E_Trans_VS_q_QEL_inclusive->GetXaxis()->SetLabelSize(0.0425);
        E_Trans_VS_q_QEL_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q_QEL_inclusive->GetYaxis()->SetLabelSize(0.0425);
        E_Trans_VS_q_QEL_inclusive->GetYaxis()->CenterTitle(true);
        E_Trans_VS_q_QEL_inclusive->GetZaxis()->SetLabelSize(0.0425);

//        E_Trans_VS_q_QEL_inclusive->Draw("colz");
        plots->Add(E_Trans_VS_q_QEL_inclusive);
        E_Trans_VS_q_QEL_inclusive->SetStats(0);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_QEL_inclusive_log_scale.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_QEL_inclusive_liner_scale.png");
        c1->Clear();

//      Normalization factor (equals to max(E_Trans_VS_q_MEC_inclusive)):
        double factor_E_Trans_VS_q_MEC_inclusive = 1.;

        if (normalized_E_lp_plots) {
            E_Trans_VS_q_MEC_inclusive->SetTitle("Energy Transfer (E_{#nu}-E_{l}) vs |q| (MEC Only, ^{12}C(e,e')) - Normalized");
            E_Trans_VS_q_MEC_inclusive->Scale(factor_E_Trans_VS_q_MEC_inclusive / E_Trans_VS_q_MEC_inclusive->Integral(), "width");
            E_Trans_VS_q_MEC_inclusive->Draw("colz");
            E_Trans_VS_q_MEC_inclusive->SetMaximum(1.5);
        } else {
            E_Trans_VS_q_MEC_inclusive->Draw("colz");
        }

        E_Trans_VS_q_MEC_inclusive->SetTitleSize(0.06, "xyz");
        E_Trans_VS_q_MEC_inclusive->GetXaxis()->SetLabelSize(0.0425);
        E_Trans_VS_q_MEC_inclusive->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q_MEC_inclusive->GetYaxis()->SetLabelSize(0.0425);
        E_Trans_VS_q_MEC_inclusive->GetYaxis()->CenterTitle(true);
        E_Trans_VS_q_MEC_inclusive->GetZaxis()->SetLabelSize(0.0425);
//        E_Trans_VS_q_MEC_inclusive->Draw("colz");
        plots->Add(E_Trans_VS_q_MEC_inclusive);
        E_Trans_VS_q_MEC_inclusive->SetStats(0);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_MEC_inclusive_log_scale.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Inclusive_plots/Energy_transfer_Ev-El_VS_q_MEC_inclusive_liner_scale.png");
        c1->Clear();

// 2p: MOVE TO ET

        E_Trans_VS_q3_all_2p->Draw("colz");
        plots->Add(E_Trans_VS_q3_all_2p);
        E_Trans_VS_q3_all_2p->SetStats(0);
        E_Trans_VS_q3_all_2p->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q3_all_2p->GetYaxis()->CenterTitle(true);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_log_scale_2p.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_linear_scale_2p.png");
        c1->Clear();

        E_Trans_VS_q3_QEL_2p->Draw("colz");
        plots->Add(E_Trans_VS_q3_QEL_2p);
        E_Trans_VS_q3_QEL_2p->SetStats(0);
        E_Trans_VS_q3_QEL_2p->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q3_QEL_2p->GetYaxis()->CenterTitle(true);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_log_scale_2p.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_linear_scale_2p.png");
        c1->Clear();

        E_Trans_VS_q3_MEC_2p->Draw("colz");
        plots->Add(E_Trans_VS_q3_MEC_2p);
        E_Trans_VS_q3_MEC_2p->SetStats(0);
        E_Trans_VS_q3_MEC_2p->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q3_MEC_2p->GetYaxis()->CenterTitle(true);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_log_scale_2p.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_linear_scale_2p.png");
        c1->Clear();

// 1n1p: MOVE TO ET

        E_Trans_VS_q3_all_1n1p->Draw("colz");
        plots->Add(E_Trans_VS_q3_all_1n1p);
        E_Trans_VS_q3_all_1n1p->SetStats(0);
        E_Trans_VS_q3_all_1n1p->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q3_all_1n1p->GetYaxis()->CenterTitle(true);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_log_scale_1n1p.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_all_linear_scale_1n1p.png");
        c1->Clear();

        E_Trans_VS_q3_QEL_1n1p->Draw("colz");
        plots->Add(E_Trans_VS_q3_QEL_1n1p);
        E_Trans_VS_q3_QEL_1n1p->SetStats(0);
        E_Trans_VS_q3_QEL_1n1p->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q3_QEL_1n1p->GetYaxis()->CenterTitle(true);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_log_scale_1n1p.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_QEL_linear_scale_1n1p.png");
        c1->Clear();

        E_Trans_VS_q3_MEC_1n1p->Draw("colz");
        plots->Add(E_Trans_VS_q3_MEC_1n1p);
        E_Trans_VS_q3_MEC_1n1p->SetStats(0);
        E_Trans_VS_q3_MEC_1n1p->GetXaxis()->CenterTitle(true);
        E_Trans_VS_q3_MEC_1n1p->GetYaxis()->CenterTitle(true);
        c1->SetLogz(1);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_log_scale_1n1p.png");
        c1->SetLogz(0);
        c1->SaveAs("plots/Energy_transfer_histograms/Energy_transfer_VS_q3/Energy_transfer_Ev-El_VS_q3_MEC_linear_scale_1n1p.png");
        c1->Clear();

    }
    //</editor-fold>


// Saving histogram list and finishing execution
// =======================================================================================================================================================================

    //<editor-fold desc="Saving histogram list and finishing execution">
    cout << "\n\nSaving histogram list...";

    TFile *fout = new TFile(TListName, "recreate");
    fout->cd();
    plots->Write();
    fout->Write();
    fout->Close();

    cout << " done.\n\n";

    cout << "\n\n===========================================================================\n";
    cout << "\t\t\tExecution summary\n";
    cout << "===========================================================================\n\n";

    if (calculate_2p == true) {
        cout << "#(2p) events:\t\t" << num_of_2p_events << "\n";
    } else {
        cout << "#(2p) events:\t\tcalculation not performed\n";
    }

    if (calculate_1n1p == true) {
        cout << "#(1n1p) events:\t\t" << num_of_1n1p_events << "\n";
    } else {
        cout << "#(1n1p) events:\t\tcalculation not performed\n";
    }

    if (calculate_MicroBooNE == true) {
        cout << "#(MicroBooNE) events:\t" << num_of_MicroBooNE_events << "\n";
    } else {
        cout << "#(MicroBooNE) events:\tcalculation not performed\n";
    }

    if (FSI_status == false) {
        cout << "FSI status:\t\tOFF (ni = " << ni_selection << ")\n";
    } else if (FSI_status == true) {
        cout << "FSI status:\t\tON\n";
    }

    cout << "File input:\t\t" << LoadedInput << "\n";
    cout << "Settings mode:\t\t'" << file_name << "'\n\n";

    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n\n";
    //</editor-fold>

}