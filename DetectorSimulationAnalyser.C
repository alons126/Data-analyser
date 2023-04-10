
/*

 git pull && clas12root -b -q main.c

 * Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/

 * Copy commend (JLab -> local):
scp -r asportes@ftp.jlab.org:/u/home/asportes/Analyser/plots -J /home/alon/project/temp/
scp -r asportes@ftp.jlab.org:/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo -J /home/alon/project/temp/

 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <chrono>
#include <vector>
#include <typeinfo>
#include <sstream>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

#include <TBenchmark.h>

#include "HipoChain.h"
#include "clas12reader.h"

#include "settings/codeSetup.h"

using namespace std;
using namespace clas12;

void EventAnalyser() {

    cout << "\n\n===========================================================================\n";
    cout << "\t\t\tDetector simulation analyser\n";
    cout << "===========================================================================\n\n";

    string AnalyserVersion = "Beta version";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code settings                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Code settings">

//  Input processing ----------------------------------------------------------------------------------------------------------------------------------------------------

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

// Cuts settings --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Cuts settings">
    /* Settings that allow to disable/enable every cut individually */

    //TODO: add beta = 1.2 cut for electrons
    bool apply_cuts = true; // master ON/OFF switch for applying cuts

    /* HTCC cut */
    bool apply_Nphe_cut = true;

    /* Chi2 cuts (= PID cuts) */
    bool apply_chi2_cuts_1e_cut = true;

    /* Vertex cuts */
    bool apply_Vz_cuts = true, apply_dVz_cuts = true;

    /* Sampling Fraction (SF) cut */
    bool apply_SF_cuts = true;

    /* ECAL fiducial (edge) cuts */
    bool apply_ECAL_fiducial_cuts = true;

    /* DC fiducial (edge) cuts */
    bool apply_DC_fiducial_cut = true;

    /* Momentum cuts */
    bool apply_momentum_cuts_2p = true, apply_momentum_cuts_1n1p = true;

    //<editor-fold desc="Cuts output">
    /* Print out the cuts within the run (for self-observation) */

    if (apply_cuts == false) {
        cout << "Cuts are disabled.\n\n\n";

        apply_momentum_cuts_2p = apply_momentum_cuts_1n1p = apply_SF_cuts = apply_ECAL_fiducial_cuts = apply_DC_fiducial_cut = false;
        apply_Nphe_cut = apply_chi2_cuts_1e_cut = false;
    } else {
        cout << "Cuts are enabled. Cut settings:\n";
        cout << "apply_Nphe_cut:\t\t\t" << BoolToString(apply_Nphe_cut) << "\n";
        cout << "apply_chi2_cuts_1e_cut:\t\t" << BoolToString(apply_chi2_cuts_1e_cut) << "\n";
        cout << "apply_Vz_cuts:\t\t\t" << BoolToString(apply_Vz_cuts) << "\n";
        cout << "apply_dVz_cuts:\t\t\t" << BoolToString(apply_dVz_cuts) << "\n";
        cout << "apply_SF_cuts:\t\t\t" << BoolToString(apply_SF_cuts) << "\n";
        cout << "apply_ECAL_fiducial_cuts:\t" << BoolToString(apply_ECAL_fiducial_cuts) << "\n";
        cout << "apply_DC_fiducial_cut:\t\t" << BoolToString(apply_DC_fiducial_cut) << "\n";
        cout << "apply_momentum_cuts_2p:\t\t" << BoolToString(apply_momentum_cuts_2p) << "\n";
        cout << "apply_momentum_cuts_1n1p:\t" << BoolToString(apply_momentum_cuts_1n1p) << "\n\n\n";
    }
    //</editor-fold>

    //<editor-fold desc="Custom cuts naming">
    /* Save plots to custom-named folders, to allow multi-sample runs at once. */

    bool custom_cuts_naming = true;

    if (custom_cuts_naming == true) {
        if (apply_cuts == false) {
            plots_path = WorkingDirectory + "plots_" + SampleName + "_-_NO_CUTS/";
            plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-_NO_CUTS.txt";
        } else {
            if (apply_chi2_cuts_1e_cut == false) {
                plots_path = WorkingDirectory + "plots_" + SampleName + "_-_ALL_CUTS_woChi2/";
                plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-_ALL_CUTS_woChi2.txt";
            } else if (apply_chi2_cuts_1e_cut == true) {
                plots_path = WorkingDirectory + "plots_" + SampleName + "_-_ALL_CUTS/";
                plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-_ALL_CUTS.txt";
            }
        }
    }
    //</editor-fold>

    //</editor-fold>

// Cuts declarations -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Cuts declarations">
    /* Log cut values to be used later when applying them. */

    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
    DSCuts Nphe_cuts_FD;

    /* Chi2 cuts. NOTES:
     * Values for mean and sigma are filled from fit variables (overating these values later).
     * Upper cut lim (Cuts.at(2)) is the same as the sigma that is used in clas12ana to apply PID cuts */
    DSCuts Chi2_Electron_cuts_CD = DSCuts("Chi2", "CD", "Electron", "1e cut", 0, -6, 6);
    DSCuts Chi2_Electron_cuts_FD = DSCuts("Chi2", "FD", "Electron", "1e cut", 0, -6, 6);

    DSCuts Chi2_Proton_cuts_CD = DSCuts("Chi2", "CD", "Proton", "1e cut", 0, -1, -1);
    DSCuts Chi2_Proton_cuts_FD = DSCuts("Chi2", "FD", "Proton", "1e cut", 0, -1, -1);

    DSCuts Chi2_piplus_cuts_CD = DSCuts("Chi2", "CD", "piplus", "1e cut", 0, -1, -1);
    DSCuts Chi2_piplus_cuts_FD = DSCuts("Chi2", "FD", "piplus", "1e cut", 0, -1, -1);

    DSCuts Chi2_piminus_cuts_CD = DSCuts("Chi2", "CD", "piminus", "1e cut", 0, -1, -1);
    DSCuts Chi2_piminus_cuts_FD = DSCuts("Chi2", "FD", "piminus", "1e cut", 0, -1, -1);

    //<editor-fold desc="Kaon PID cuts">
    /* Kaon pid cuts. Not really applied in our analysis. */
    DSCuts Chi2_Kplus_cuts_CD = DSCuts("Chi2", "CD", "Kplus", "1e cut", 0, -1, -1);
    DSCuts Chi2_Kplus_cuts_FD = DSCuts("Chi2", "FD", "Kplus", "1e cut", 0, -1, -1);
    DSCuts Chi2_Kminus_cuts_CD = DSCuts("Chi2", "CD", "Kminus", "1e cut", 0, -1, -1);
    DSCuts Chi2_Kminus_cuts_FD = DSCuts("Chi2", "FD", "Kminus", "1e cut", 0, -1, -1);
    //</editor-fold>

    /* Vertex cuts */
    DSCuts Vz_cut = DSCuts("Vertex z component", "", "", "1e cut", 0, -5, 5);
    DSCuts dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -8, 4);

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
    DSCuts SF_cuts;

    /* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts PCAL_edge_cuts;

    /* DC edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts DC_edge_cuts;

    /* Momentum cuts */
    // Momentum cuts (1p)
    DSCuts e_momentum_cuts_1p = DSCuts("Momentum", "", "Electron", "1p", 0, -1, -1);
    DSCuts p_momentum_cuts_1p = DSCuts("Momentum", "", "Proton", "1p", 0, 0.3, -1);

    // Momentum cuts (2p)
    DSCuts e_momentum_cuts_2p = DSCuts("Momentum", "", "Electron", "2p", 0, -1, -1);
    DSCuts p_momentum_cuts_2p = DSCuts("Momentum", "", "Proton", "2p", 0, 0.3, -1);

    // Momentum cuts (1n1p)
    DSCuts e_momentum_cuts_1n1p = DSCuts("Momentum", "", "Electron", "1n1p", 0, -1, -1);
    DSCuts p_momentum_cuts_1n1p = DSCuts("Momentum", "", "Proton", "1n1p", 0, 0.3, -1);
    DSCuts n_momentum_cuts_1n1p = DSCuts("Momentum", "", "Neutron", "1n1p", 0, 0.3, -1);
    //</editor-fold>

// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="TList definition">
    /* Definition of plots TList used to save all plots to .root file. */

    TList *plots = new TList();
    string listName = plots_path + AnalyseFileSample + plots_file_type;
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

// Calculation settings -------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Calculation settings">
    /* settings to enable/disable specific FS plot calculations. */

    bool calculate_1p = true, calculate_2p = true, calculate_1n1p = true;
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

    /* Beta vs. P plots */
    bool Beta_vs_P_plots = true;

    /* Angle plots */
    bool Angle_plots_master = true; // Master angle plots selector
    bool Theta_e_plots = true, Phi_e_plots = true;

    /* Q2 plots */
    bool Q2_plots = true;

    /* E_e plots */
    bool E_e_plots = true;

    /* ET plots */
    bool ETrans_plots_master = true; // Master ET plots selector
    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

    /* Ecal plots */
    bool Ecal_plots = true;

    /* Transverse variables plots */
    bool TVariables_plots = true;

    /* Other settings variables */
    bool wider_margin = true;
    bool debug_plots = true; // Print out clas12ana debugging plots

    //<editor-fold desc="Turn off plots by master selectors">
    if (Plot_selector_master == false) {
        Cut_plots_master = Beta_vs_P_plots = Angle_plots_master = Q2_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
    }

    if (Cut_plots_master == false) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }
    if (ETrans_plots_master == false) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }
    if (Angle_plots_master == false) { Theta_e_plots = Phi_e_plots = false; }
    //</editor-fold>

    //</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization settings">
    /* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
     * Enable of presentations only, since event count is important otherwise. */

    bool normalize_master = false;

    bool norm_Nphe_plots = false, norm_Chi2_plots = false, norm_Vertex_plots = false, norm_SF_plots = false, norm_Fiducial_plots = false;

    bool norm_Angle_plots_master = false, norm_Q2_plots = false, norm_E_e_plots = false, norm_ET_plots = false, norm_Ecal_plots = false, norm_TVariables_plots = false;

    if (normalize_master == false) { // Disable all normalizations if normalize_master == false
        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = false;
        norm_Angle_plots_master = norm_Q2_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TVariables_plots = false;

        cout << "\nAll normalizations are disabled.\n\n";  // and no change to custom_FSI_status
    }
    //</editor-fold>

// Delete settings ------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Delete settings">
    /* Clear files from previous runs (to prevent mix fo plots from different codes). */

    bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

    /* Delete existing .txt files */
    if (delete_txt_files == true) { system(("find " + plots_path + " -type f -iname '*.txt' -delete").c_str()); }

    //<editor-fold desc="Deleting files by cases">
    if (delete_png_files == true && delete_root_files == false) {
        cout << "\nClearing old plots...";
        system(("find " + plots_path + " -type f -iname '*.png' -delete").c_str()); // Delete existing .png files
        cout << " done.\n\n";
    } else if (delete_png_files == false && delete_root_files == true) {
        cout << "\nClearing old root files...";
        system(("find " + plots_path + " -type f -iname '*.root' -delete").c_str()); // Delete existing .root files
        cout << " done.\n\n";
    } else if (delete_png_files == true && delete_root_files == true) {
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

    /* Chi2 plots */
    double Chi2_boundary = 30;
    if (apply_cuts == true) { Chi2_boundary = 9; }

    /* Vertex plots */
    double Vertex_boundary = 50, Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;

    if (apply_cuts == true) {
        double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
        Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
        Vertex_boundary = Vertex_boundary / 5;
    }

    double dV_boundary = 50;
    if (apply_cuts == true) { /* dV_boundary = 7.5; */ dV_boundary = dVz_cuts.GetUpperCut() * 1.4; }

    /* SF */
    double SF_uboundary = 0.31, SF_lboundary = 0.16;

    /* Beta vs. P plots */
    double Beta_boundary = 3, P_boundary = beamE * 1.425;
    if (apply_cuts == true) { Beta_boundary = 1.1, P_boundary = beamE * 1.1; }

    /* Transverse variables */
    double dP_T_boundary = 2.5;
    //</editor-fold>

// Debugging settings ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Debugging settings">
    /* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */

    bool PrintEvents = false;
    int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;

    ofstream EventPrint;
    string EventPrint_save_Directory;

    if (PrintEvents == true) {
        if (apply_chi2_cuts_1e_cut == false) {
            EventPrint_save_Directory = plots_path + "/" + "Event_Print_without_chi2.txt";
        } else if (apply_chi2_cuts_1e_cut == true) {
            EventPrint_save_Directory = plots_path + "/" + "Event_Print_ALL_CUTS.txt";
        }

        EventPrint.open(EventPrint_save_Directory.c_str());

        if (apply_chi2_cuts_1e_cut == false) {
            EventPrint << "//////////////////////////////////////////////////////////////////////\n";
            EventPrint << "// Log of number of particles in event with all cuts except chi2    //\n";
            EventPrint << "//////////////////////////////////////////////////////////////////////\n\n";
        } else if (apply_chi2_cuts_1e_cut == true) {
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                      Histogram definitions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Histogram definitions">
    /* Histogram definitions and settings. */

    cout << "\nDefining histograms...";

    //TODO: add weights to all histograms

// ======================================================================================================================================================================
// Cut parameters plots
// ======================================================================================================================================================================

    //<editor-fold desc="Cut parameters plots">

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Number of Photo-electrons (Nphe) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms (FD only)">

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms (1e cut, FD only)">

    THStack *sNphe_1e_cut_FD = new THStack("N_{phe} (1e Cut)", "#Photo-electrons in HTCC - N_{phe} (1e Cut);N_{phe}");
//    TH1D *hNphe_1e_cut_BC_FD, *hNphe_1e_cut_AC_FD;
//    string hNphe_1e_cut_BC_FD_Dir, hNphe_1e_cut_AC_FD_Dir;

    hPlot1D hNphe_1e_cut_BC_FD, hNphe_1e_cut_AC_FD;
//    Histogram1D hNphe_1e_cut_BC_FD, hNphe_1e_cut_AC_FD;

    if (apply_cuts == false) {
//        hNphe_1e_cut_BC_FD.FinalState = "1e Cut";
//        hNphe_1e_cut_BC_FD.Histogram1d = new TH1D(("N_{phe} in HTCC BC (" + hNphe_1e_cut_BC_FD.FinalState + ")").c_str(), ";N_{phe}", hNphe_1e_cut_BC_FD.NumOfXBins, 0,
//                                                  40);
//        hNphe_1e_cut_BC_FD.HistogramTitle = "#Photo-electrons in HTCC N_{phe} - before cuts";
//        hNphe_1e_cut_BC_FD.SaveDirectory = directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"];
//        hNphe_1e_cut_BC_FD.SaveName = "01_Nphe_1e_cut_BC";
//
//        hNphe_1e_cut_AC_FD.FinalState = "1e Cut";
//        hNphe_1e_cut_AC_FD.Histogram1d = new TH1D(("N_{phe} in HTCC AC (" + hNphe_1e_cut_BC_FD.FinalState + ")").c_str(), ";N_{phe}", hNphe_1e_cut_AC_FD.NumOfXBins, 0,
//                                                  40);
//        hNphe_1e_cut_AC_FD.HistogramTitle = "#Photo-electrons in HTCC N_{phe} - after cuts";
//        hNphe_1e_cut_AC_FD.SaveDirectory = directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"];
//        hNphe_1e_cut_AC_FD.SaveName = "02_Nphe_1e_cut_AC";
    } else {
        hNphe_1e_cut_BC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                     directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"], "01_Nphe_1e_cut", 0, 40);
//        hNphe_1e_cut_BC_FD.FinalState = "1e Cut";
//        hNphe_1e_cut_BC_FD.Histogram1d = new TH1D(("N_{phe} in HTCC (" + hNphe_1e_cut_BC_FD.FinalState + ")").c_str(), ";YYYYN_{phe}", hNphe_1e_cut_BC_FD.NumOfXBins, 0, 40);
//        hNphe_1e_cut_BC_FD.HistogramTitle = "#Photo-electrons in HTCC N_{phe}";
//        hNphe_1e_cut_BC_FD.SaveDirectory = directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"];
//        hNphe_1e_cut_BC_FD.SaveName = "01_Nphe_1e_cutYYYY";
////        hNphe_1e_cut_BC_FD.FinalState = "1e Cut";
////        hNphe_1e_cut_BC_FD.Histogram1d = new TH1D(("N_{phe} in HTCC BC (" + hNphe_1e_cut_BC_FD.FinalState + ")").c_str(), ";YYYYN_{phe}", hNphe_1e_cut_BC_FD.NumOfXBins, 0, 40);
////        hNphe_1e_cut_BC_FD.HistogramTitle = "#Photo-electrons in HTCC N_{phe}";
////        hNphe_1e_cut_BC_FD.SaveDirectory = directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"];
////        hNphe_1e_cut_BC_FD.SaveName = "01_Nphe_1e_cutYYYY";
    }

//    THStack *sNphe_1e_cut_FD = new THStack("N_{phe} (1e Cut)", "#Photo-electrons in HTCC - N_{phe} (1e Cut);N_{phe}");
//    TH1D *hNphe_1e_cut_BC_FD, *hNphe_1e_cut_AC_FD;
//    string hNphe_1e_cut_BC_FD_Dir, hNphe_1e_cut_AC_FD_Dir;
//
//    if (apply_cuts == false) {
//        hNphe_1e_cut_BC_FD = new TH1D("N_{phe} in HTCC BC (1e Cut)", "#Photo-electrons in HTCC - N_{phe} - Before Cuts (1e Cut);N_{phe}", 100, 0, 40);
//        hNphe_1e_cut_AC_FD = new TH1D("N_{phe} in HTCC AC (1e Cut)", "#Photo-electrons in HTCC - N_{phe} - After Cuts (1e Cut);N_{phe}", 100, 0, 40);
//        hNphe_1e_cut_BC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"];
//        hNphe_1e_cut_AC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"];
////        hNphe_1e_cut_BC_FD_Dir = Nphe_1e_cut_BC_Directory, hNphe_1e_cut_AC_FD_Dir = Nphe_1e_cut_AC_Directory;
//    } else {
//        hNphe_1e_cut_BC_FD = new TH1D("N_{phe} in HTCC (1e Cut)", "#Photo-electrons in HTCC - N_{phe} (1e Cut);N_{phe}", 100, 0, 40);
//        hNphe_1e_cut_BC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"];
//        hNphe_1e_cut_AC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"];
////        hNphe_1e_cut_BC_FD_Dir = Nphe_1e_cut_BC_Directory, hNphe_1e_cut_AC_FD_Dir = Nphe_1e_cut_AC_Directory;
//    }
    //</editor-fold>

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms (2p, FD only)">
    THStack *sNphe_2p_FD = new THStack("N_{phe} in HTCC (2p)", "#Photo-electrons in HTCC - N_{phe} Histogram (2p);N_{phe}");
    TH1D *hNphe_2p_FD = new TH1D("N_{phe} in HTCC (2p)", "#Photo-electrons in HTCC - N_{phe} (2p);N_{phe}", 100, 0, 40);
    string hNphe_2p_FD_Dir = directories.Nphe_Directory_map["Nphe_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">

    //<editor-fold desc="Chi2 plots (no #(e) cut)">
    /* Plots of chi2 with no cut on number of electrons - NOT used later! */

    THStack *sChi2_Electron_All_e = new THStack("#chi^{2}_{e} (no #(e) cut, CD & FD)", "#chi^{2}_{e} Histogram (no #(e) cut, CD & FD);#chi^{2}_{e};");
    TH1D *hChi2_Electron_FD = new TH1D("#chi^{2}_{e} (no #(e) cut)", "#chi^{2}_{e} Histogram (no #(e) cut);#chi^{2}_{e};",
                                       100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_FD_Dir = directories.Chi2_Directory_map["Chi2_All_e_Directory"];

    THStack *sChi2_Proton_All_e = new THStack("#chi^{2}_{p} (no #(e) cut, CD & FD)", "#chi^{2}_{p} Histogram (no #(e) cut, CD & FD);#chi^{2}_{p};");
    TH1D *hChi2_Proton_CD = new TH1D("#chi^{2}_{p} (no #(e) cut, CD)", "#chi^{2}_{p} Histogram (no #(e) cut, CD);#chi^{2}_{p};",
                                     100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_FD = new TH1D("#chi^{2}_{p} (no #(e) cut, FD)", "#chi^{2}_{p} Histogram (no #(e) cut, FD);#chi^{2}_{p};",
                                     100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Proton_CD_Dir = directories.Chi2_Directory_map["Chi2_All_e_Directory"];
    string hChi2_Proton_FD_Dir = directories.Chi2_Directory_map["Chi2_All_e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e cut)">
    /* Plots of chi2 with no cut on number of electrons - used later to apply cuts with my mechanism (NOT Justin's ana) */

    THStack *sChi2_Electron_1e_cut = new THStack("#chi^{2}_{e} (1e cut, CD & FD)", "#chi^{2}_{e} Histogram (1e cut, CD & FD);#chi^{2}_{e};");
    TH1D *hChi2_Electron_1e_cut_FD = new TH1D("#chi^{2}_{e} (1e cut)", "#chi^{2}_{e} Histogram (1e cut);#chi^{2}_{e};", 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_1e_cut_FD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];

    THStack *sChi2_Proton_1e_cut = new THStack("#chi^{2}_{p} (1e cut, CD & FD)", "#chi^{2}_{p} Histogram (1e cut, CD & FD);#chi^{2}_{p};");
    TH1D *hChi2_Proton_1e_cut_CD = new TH1D("#chi^{2}_{p} (1e cut, CD)", "#chi^{2}_{p} Histogram (1e cut, CD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_1e_cut_FD = new TH1D("#chi^{2}_{p} (1e cut, FD)", "#chi^{2}_{p} Histogram (1e cut, FD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Proton_1e_cut_CD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];
    string hChi2_Proton_1e_cut_FD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];

    THStack *sChi2_Kplus_1e_cut = new THStack("#chi^{2}_{K^{+}} (1e cut, CD & FD)", "#chi^{2}_{K^{+}} Histogram (1e cut, CD & FD);#chi^{2}_{K^{+}};");
    TH1D *hChi2_Kplus_1e_cut_CD = new TH1D("#chi^{2}_{K^{+}} (1e cut, CD)", "#chi^{2}_{K^{+}} Histogram (1e cut, CD);#chi^{2}_{K^{+}};",
                                           100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Kplus_1e_cut_FD = new TH1D("#chi^{2}_{K^{+}} (1e cut, FD)", "#chi^{2}_{K^{+}} Histogram (1e cut, FD);#chi^{2}_{K^{+}};",
                                           100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Kplus_1e_cut_CD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];
    string hChi2_Kplus_1e_cut_FD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];

    THStack *sChi2_Kminus_1e_cut = new THStack("#chi^{2}_{K^{-}} (1e cut, CD & FD)", "#chi^{2}_{K^{-}} Histogram (1e cut, CD & FD);#chi^{2}_{K^{-}};");
    TH1D *hChi2_Kminus_1e_cut_CD = new TH1D("#chi^{2}_{K^{-}} (1e cut, CD)", "#chi^{2}_{K^{-}} Histogram (1e cut, CD);#chi^{2}_{K^{-}};",
                                            100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Kminus_1e_cut_FD = new TH1D("#chi^{2}_{K^{-}} (1e cut, FD)", "#chi^{2}_{K^{-}} Histogram (1e cut, FD);#chi^{2}_{K^{-}};",
                                            100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Kminus_1e_cut_CD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];
    string hChi2_Kminus_1e_cut_FD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];

    THStack *sChi2_piplus_1e_cut = new THStack("#chi^{2}_{#pi^{+}} (1e cut, CD & FD)", "#chi^{2}_{#pi^{+}} Histogram (1e cut, CD & FD);#chi^{2}_{#pi^{+}};");
    TH1D *hChi2_piplus_1e_cut_CD = new TH1D("#chi^{2}_{#pi^{+}} (1e cut, CD)", "#chi^{2}_{#pi^{+}} Histogram (1e cut, CD);#chi^{2}_{#pi^{+}};",
                                            100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_piplus_1e_cut_FD = new TH1D("#chi^{2}_{#pi^{+}} (1e cut, FD)", "#chi^{2}_{#pi^{+}} Histogram (1e cut, FD);#chi^{2}_{#pi^{+}};",
                                            100, -Chi2_boundary, Chi2_boundary);
    string hChi2_piplus_1e_cut_CD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];
    string hChi2_piplus_1e_cut_FD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];

    THStack *sChi2_piminus_1e_cut = new THStack("#chi^{2}_{#pi^{-}} (1e cut, CD & FD)", "#chi^{2}_{#pi^{-}} Histogram (1e cut, CD & FD);#chi^{2}_{#pi^{-}};");
    TH1D *hChi2_piminus_1e_cut_CD = new TH1D("#chi^{2}_{#pi^{-}} (1e cut, CD)", "#chi^{2}_{#pi^{-}} Histogram (1e cut, CD);#chi^{2}_{#pi^{-}};",
                                             100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_piminus_1e_cut_FD = new TH1D("#chi^{2}_{#pi^{-}} (1e cut, FD)", "#chi^{2}_{#pi^{-}} Histogram (1e cut, FD);#chi^{2}_{#pi^{-}};",
                                             100, -Chi2_boundary, Chi2_boundary);
    string hChi2_piminus_1e_cut_CD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];
    string hChi2_piminus_1e_cut_FD_Dir = directories.Chi2_Directory_map["Chi2_1e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e2p)">
    THStack *sChi2_Electron_1e2p = new THStack("#chi^{2}_{e} (1e2p, CD & FD)", "#chi^{2}_{e} Histogram (1e2p, CD & FD);#chi^{2}_{e};");
    THStack *sChi2_Proton_1e2p = new THStack("#chi^{2}_{p} (1e2p, CD & FD)", "#chi^{2}_{p} Histogram (1e2p, CD & FD);#chi^{2}_{p};");

    TH1D *hChi2_Electron_1e2p_BC_FD, *hChi2_Proton_1e2p_BC_CD, *hChi2_Proton_1e2p_BC_FD;
    string hChi2_Electron_1e2p_BC_FD_Dir, hChi2_Proton_1e2p_BC_CD_Dir, hChi2_Proton_1e2p_BC_FD_Dir;
    TH1D *hChi2_Electron_1e2p_AC_FD, *hChi2_Proton_1e2p_AC_CD, *hChi2_Proton_1e2p_AC_FD;
    string hChi2_Electron_1e2p_AC_FD_Dir, hChi2_Proton_1e2p_AC_CD_Dir, hChi2_Proton_1e2p_AC_FD_Dir;

    if (apply_cuts == false) {
        hChi2_Electron_1e2p_BC_FD = new TH1D("#chi^{2}_{e} BC (1e2p)", "#chi^{2}_{e} Histogram Before Cut (1e2p);#chi^{2}_{e};",
                                             100, -Chi2_boundary, Chi2_boundary);
        hChi2_Electron_1e2p_BC_FD_Dir = directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"];

        hChi2_Proton_1e2p_BC_CD = new TH1D("#chi^{2}_{p} BC (1e2p, CD)", "#chi^{2}_{p} Histogram Before Cut (1e2p, CD);#chi^{2}_{p};",
                                           100, -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_FD = new TH1D("#chi^{2}_{p} BC (1e2p, FD)", "#chi^{2}_{p} Histogram Before Cut (1e2p, FD);#chi^{2}_{p};",
                                           100, -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_CD_Dir = hChi2_Proton_1e2p_BC_FD_Dir = directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"];

        hChi2_Electron_1e2p_AC_FD = new TH1D("#chi^{2}_{e} AC (1e2p)", "#chi^{2}_{e} Histogram After Cut (1e2p);#chi^{2}_{e};", 100, -Chi2_boundary, Chi2_boundary);
        hChi2_Electron_1e2p_AC_FD_Dir = directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"];

        hChi2_Proton_1e2p_AC_CD = new TH1D("#chi^{2}_{p} AC (1e2p, CD)", "#chi^{2}_{p} Histogram After Cut (1e2p, CD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_AC_FD = new TH1D("#chi^{2}_{p} AC (1e2p, FD)", "#chi^{2}_{p} Histogram After Cut (1e2p, FD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_AC_CD_Dir = hChi2_Proton_1e2p_AC_FD_Dir = directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"];
    } else {
        hChi2_Electron_1e2p_BC_FD = new TH1D("#chi^{2}_{e} (1e2p)", "#chi^{2}_{e} Histogram (1e2p);#chi^{2}_{e};", 100, -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_CD = new TH1D("#chi^{2}_{p} (1e2p, CD)", "#chi^{2}_{p} Histogram (1e2p, CD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_FD = new TH1D("#chi^{2}_{p} (1e2p, FD)", "#chi^{2}_{p} Histogram (1e2p, FD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
        hChi2_Electron_1e2p_BC_FD_Dir = hChi2_Proton_1e2p_BC_CD_Dir = hChi2_Proton_1e2p_BC_FD_Dir = directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"];
    }
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (2p)">
    TH1D *hChi2_Electron_2p_FD = new TH1D("#chi^{2}_{e} (2p)", "#chi^{2}_{e} Histogram (2p);#chi^{2}_{e};", 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_2p_FD_Dir = directories.Chi2_Directory_map["Chi2_2p_Directory"];

    TH1D *hChi2_Proton_2p_CD = new TH1D("#chi^{2}_{p} (2p, CD)", "#chi^{2}_{p} Histogram (2p, CD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_2p_FD = new TH1D("#chi^{2}_{p} (2p, FD)", "#chi^{2}_{p} Histogram (2p, FD);#chi^{2}_{p};", 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Proton_2p_CD_Dir = directories.Chi2_Directory_map["Chi2_2p_Directory"];
    string hChi2_Proton_2p_FD_Dir = directories.Chi2_Directory_map["Chi2_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">

    //<editor-fold desc="Vertex plots (1e only)">
    THStack *sVx_Electron_1e_cut = new THStack("V_{x}^{e} (1e Only, CD & FD)", "V_{x}^{e} (1e Only, CD & FD);V_{x}^{e} [cm];");
    THStack *sVy_Electron_1e_cut = new THStack("V_{y}^{e} (1e Only, CD & FD)", "V_{y}^{e} (1e Only, CD & FD);V_{y}^{e} [cm];");
    THStack *sVz_Electron_1e_cut = new THStack("V_{z}^{e} (1e Only, CD & FD)", "V_{z}^{e} (1e Only, CD & FD);V_{z}^{e} [cm];");
    TH1D *hVx_Electron_1e_cut_FD = new TH1D("V_{x}^{e} (1e Cut)", "V_{x}^{e} (1e Cut);V_{x}^{e} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Electron_1e_cut_FD = new TH1D("V_{y}^{e} (1e Cut)", "V_{y}^{e} (1e Cut);V_{y}^{e} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Electron_1e_cut_FD = new TH1D("V_{z}^{e} (1e Cut)", "V_{z}^{e} (1e Cut);V_{z}^{e} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    string hVx_Electron_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Electron_1e_Vx_Directory"];
    string hVy_Electron_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Electron_1e_Vy_Directory"];
    string hVz_Electron_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Electron_1e_Vz_Directory"];

    THStack *sVx_Proton_1e_cut = new THStack("V_{x}^{p} (1e Only, CD & FD)", "V_{x}^{p} (1e Only, CD & FD);V_{x}^{p} [cm];");
    THStack *sVy_Proton_1e_cut = new THStack("V_{y}^{p} (1e Only, CD & FD)", "V_{y}^{p} (1e Only, CD & FD);V_{y}^{p} [cm];");
    THStack *sVz_Proton_1e_cut = new THStack("V_{z}^{p} (1e Only, CD & FD)", "V_{z}^{p} (1e Only, CD & FD);V_{z}^{p} [cm];");
    TH1D *hVx_Proton_1e_cut_CD = new TH1D("V_{x}^{p} (1e Cut, CD)", "V_{x}^{p} (1e Cut, CD);V_{x}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Proton_1e_cut_CD = new TH1D("V_{y}^{p} (1e Cut, CD)", "V_{y}^{p} (1e Cut, CD);V_{y}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Proton_1e_cut_CD = new TH1D("V_{z}^{p} (1e Cut, CD)", "V_{z}^{p} (1e Cut, CD);V_{z}^{p} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    TH1D *hVx_Proton_1e_cut_FD = new TH1D("V_{x}^{p} (1e Cut, FD)", "V_{x}^{p} (1e Cut, FD);V_{x}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Proton_1e_cut_FD = new TH1D("V_{y}^{p} (1e Cut, FD)", "V_{y}^{p} (1e Cut, FD);V_{y}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Proton_1e_cut_FD = new TH1D("V_{z}^{p} (1e Cut, FD)", "V_{z}^{p} (1e Cut, FD);V_{z}^{p} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    string hVx_Proton_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Proton_1e_Vx_Directory"];
    string hVx_Proton_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Proton_1e_Vx_Directory"];
    string hVy_Proton_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Proton_1e_Vy_Directory"];
    string hVy_Proton_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Proton_1e_Vy_Directory"];
    string hVz_Proton_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Proton_1e_Vz_Directory"];
    string hVz_Proton_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Proton_1e_Vz_Directory"];

    THStack *sVx_Kplus_1e_cut = new THStack("V_{x}^{K^{+}} (1e Only, CD & FD)", "V_{x}^{K^{+}} (1e Only, CD & FD);V_{x}^{K^{+}} [cm];");
    THStack *sVy_Kplus_1e_cut = new THStack("V_{y}^{K^{+}} (1e Only, CD & FD)", "V_{y}^{K^{+}} (1e Only, CD & FD);V_{y}^{K^{+}} [cm];");
    THStack *sVz_Kplus_1e_cut = new THStack("V_{z}^{K^{+}} (1e Only, CD & FD)", "V_{z}^{K^{+}} (1e Only, CD & FD);V_{z}^{K^{+}} [cm];");
    TH1D *hVx_Kplus_1e_cut_CD = new TH1D("V_{x}^{K^{+}} (1e Cut, CD)", "V_{x}^{K^{+}} (1e Cut, CD);V_{x}^{K^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Kplus_1e_cut_CD = new TH1D("V_{y}^{K^{+}} (1e Cut, CD)", "V_{y}^{K^{+}} (1e Cut, CD);V_{y}^{K^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Kplus_1e_cut_CD = new TH1D("V_{z}^{K^{+}} (1e Cut, CD)", "V_{z}^{K^{+}} (1e Cut, CD);V_{z}^{K^{+}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    TH1D *hVx_Kplus_1e_cut_FD = new TH1D("V_{x}^{K^{+}} (1e Cut, FD)", "V_{x}^{K^{+}} (1e Cut, FD);V_{x}^{K^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Kplus_1e_cut_FD = new TH1D("V_{y}^{K^{+}} (1e Cut, FD)", "V_{y}^{K^{+}} (1e Cut, FD);V_{y}^{K^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Kplus_1e_cut_FD = new TH1D("V_{z}^{K^{+}} (1e Cut, FD)", "V_{z}^{K^{+}} (1e Cut, FD);V_{z}^{K^{+}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    string hVx_Kplus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Kplus_1e_Vx_Directory"];
    string hVx_Kplus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Kplus_1e_Vx_Directory"];
    string hVy_Kplus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Kplus_1e_Vy_Directory"];
    string hVy_Kplus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Kplus_1e_Vy_Directory"];
    string hVz_Kplus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Kplus_1e_Vz_Directory"];
    string hVz_Kplus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Kplus_1e_Vz_Directory"];

    THStack *sVx_Kminus_1e_cut = new THStack("V_{x}^{K^{-}} (1e Only, CD & FD)", "V_{x}^{K^{-}} (1e Only, CD & FD);V_{x}^{K^{-}} [cm];");
    THStack *sVy_Kminus_1e_cut = new THStack("V_{y}^{K^{-}} (1e Only, CD & FD)", "V_{y}^{K^{-}} (1e Only, CD & FD);V_{y}^{K^{-}} [cm];");
    THStack *sVz_Kminus_1e_cut = new THStack("V_{z}^{K^{-}} (1e Only, CD & FD)", "V_{z}^{K^{-}} (1e Only, CD & FD);V_{z}^{K^{-}} [cm];");
    TH1D *hVx_Kminus_1e_cut_CD = new TH1D("V_{x}^{K^{-}} (1e Cut, CD)", "V_{x}^{K^{-}} (1e Cut, CD);V_{x}^{K^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Kminus_1e_cut_CD = new TH1D("V_{y}^{K^{-}} (1e Cut, CD)", "V_{y}^{K^{-}} (1e Cut, CD);V_{y}^{K^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Kminus_1e_cut_CD = new TH1D("V_{z}^{K^{-}} (1e Cut, CD)", "V_{z}^{K^{-}} (1e Cut, CD);V_{z}^{K^{-}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    TH1D *hVx_Kminus_1e_cut_FD = new TH1D("V_{x}^{K^{-}} (1e Cut, FD)", "V_{x}^{K^{-}} (1e Cut, FD);V_{x}^{K^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_Kminus_1e_cut_FD = new TH1D("V_{y}^{K^{-}} (1e Cut, FD)", "V_{y}^{K^{-}} (1e Cut, FD);V_{y}^{K^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_Kminus_1e_cut_FD = new TH1D("V_{z}^{K^{-}} (1e Cut, FD)", "V_{z}^{K^{-}} (1e Cut, FD);V_{z}^{K^{-}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    string hVx_Kminus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Kminus_1e_Vx_Directory"];
    string hVx_Kminus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Kminus_1e_Vx_Directory"];
    string hVy_Kminus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Kminus_1e_Vy_Directory"];
    string hVy_Kminus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Kminus_1e_Vy_Directory"];
    string hVz_Kminus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_Kminus_1e_Vz_Directory"];
    string hVz_Kminus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_Kminus_1e_Vz_Directory"];

    THStack *sVx_piplus_1e_cut = new THStack("V_{x}^{#pi^{+}} (1e Only, CD & FD)", "V_{x}^{#pi^{+}} (1e Only, CD & FD);V_{x}^{#pi^{+}} [cm];");
    THStack *sVy_piplus_1e_cut = new THStack("V_{y}^{#pi^{+}} (1e Only, CD & FD)", "V_{y}^{#pi^{+}} (1e Only, CD & FD);V_{y}^{#pi^{+}} [cm];");
    THStack *sVz_piplus_1e_cut = new THStack("V_{z}^{#pi^{+}} (1e Only, CD & FD)", "V_{z}^{#pi^{+}} (1e Only, CD & FD);V_{z}^{#pi^{+}} [cm];");
    TH1D *hVx_piplus_1e_cut_CD = new TH1D("V_{x}^{#pi^{+}} (1e Cut, CD)", "V_{x}^{#pi^{+}} (1e Cut, CD);V_{x}^{#pi^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_piplus_1e_cut_CD = new TH1D("V_{y}^{#pi^{+}} (1e Cut, CD)", "V_{y}^{#pi^{+}} (1e Cut, CD);V_{y}^{#pi^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_piplus_1e_cut_CD = new TH1D("V_{z}^{#pi^{+}} (1e Cut, CD)", "V_{z}^{#pi^{+}} (1e Cut, CD);V_{z}^{#pi^{+}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    TH1D *hVx_piplus_1e_cut_FD = new TH1D("V_{x}^{#pi^{+}} (1e Cut, FD)", "V_{x}^{#pi^{+}} (1e Cut, FD);V_{x}^{#pi^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_piplus_1e_cut_FD = new TH1D("V_{y}^{#pi^{+}} (1e Cut, FD)", "V_{y}^{#pi^{+}} (1e Cut, FD);V_{y}^{#pi^{+}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_piplus_1e_cut_FD = new TH1D("V_{z}^{#pi^{+}} (1e Cut, FD)", "V_{z}^{#pi^{+}} (1e Cut, FD);V_{z}^{#pi^{+}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    string hVx_piplus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_piplus_1e_Vx_Directory"];
    string hVx_piplus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_piplus_1e_Vx_Directory"];
    string hVy_piplus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_piplus_1e_Vy_Directory"];
    string hVy_piplus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_piplus_1e_Vy_Directory"];
    string hVz_piplus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_piplus_1e_Vz_Directory"];
    string hVz_piplus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_piplus_1e_Vz_Directory"];

    THStack *sVx_piminus_1e_cut = new THStack("V_{x}^{#pi^{-}} (1e Only, CD & FD)", "V_{x}^{#pi^{-}} (1e Only, CD & FD);V_{x}^{#pi^{-}} [cm];");
    THStack *sVy_piminus_1e_cut = new THStack("V_{y}^{#pi^{-}} (1e Only, CD & FD)", "V_{y}^{#pi^{-}} (1e Only, CD & FD);V_{y}^{#pi^{-}} [cm];");
    THStack *sVz_piminus_1e_cut = new THStack("V_{z}^{#pi^{-}} (1e Only, CD & FD)", "V_{z}^{#pi^{-}} (1e Only, CD & FD);V_{z}^{#pi^{-}} [cm];");
    TH1D *hVx_piminus_1e_cut_CD = new TH1D("V_{x}^{#pi^{-}} (1e Cut, CD)", "V_{x}^{#pi^{-}} (1e Cut, CD);V_{x}^{#pi^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_piminus_1e_cut_CD = new TH1D("V_{y}^{#pi^{-}} (1e Cut, CD)", "V_{y}^{#pi^{-}} (1e Cut, CD);V_{y}^{#pi^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_piminus_1e_cut_CD = new TH1D("V_{z}^{#pi^{-}} (1e Cut, CD)", "V_{z}^{#pi^{-}} (1e Cut, CD);V_{z}^{#pi^{-}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    TH1D *hVx_piminus_1e_cut_FD = new TH1D("V_{x}^{#pi^{-}} (1e Cut, FD)", "V_{x}^{#pi^{-}} (1e Cut, FD);V_{x}^{#pi^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVy_piminus_1e_cut_FD = new TH1D("V_{y}^{#pi^{-}} (1e Cut, FD)", "V_{y}^{#pi^{-}} (1e Cut, FD);V_{y}^{#pi^{-}} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    TH1D *hVz_piminus_1e_cut_FD = new TH1D("V_{z}^{#pi^{-}} (1e Cut, FD)", "V_{z}^{#pi^{-}} (1e Cut, FD);V_{z}^{#pi^{-}} [cm];", 100, Vertex_lboundary, Vertex_uboundary);
    string hVx_piminus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_piminus_1e_Vx_Directory"];
    string hVx_piminus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_piminus_1e_Vx_Directory"];
    string hVy_piminus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_piminus_1e_Vy_Directory"];
    string hVy_piminus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_piminus_1e_Vy_Directory"];
    string hVz_piminus_1e_cut_CD_Dir = directories.Vertex_Directory_map["Vertex_piminus_1e_Vz_Directory"];
    string hVz_piminus_1e_cut_FD_Dir = directories.Vertex_Directory_map["Vertex_piminus_1e_Vz_Directory"];
    //</editor-fold>

    //<editor-fold desc="dV plots (1e2p & 2p)">
    THStack *sdVx_1e2p_before = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} BC (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} Before Cuts (1e2p, CD & FD);dV_{x} [cm];");
    THStack *sdVy_1e2p_before = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} BC (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} Before Cuts (1e2p, CD & FD);dV_{y} [cm];");
    THStack *sdVz_1e2p_before = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} BC (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} Before Cuts (1e2p, CD & FD);dV_{z} [cm];");
    THStack *sdVx_1e2p_after = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} AC (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} After Cuts (1e2p, CD & FD);dV_{x} [cm];");
    THStack *sdVy_1e2p_after = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} AC (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} After Cuts (1e2p, CD & FD);dV_{y} [cm];");
    THStack *sdVz_1e2p_after = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} AC (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} After Cuts (1e2p, CD & FD);dV_{z} [cm];");
    THStack *sdVx_2p = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} (2p, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} (2p, CD & FD);dV_{x} [cm];");
    THStack *sdVy_2p = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} (2p, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} (2p, CD & FD);dV_{y} [cm];");
    THStack *sdVz_2p = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} (2p, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} (2p, CD & FD);dV_{z} [cm];");

    TH1D *hdVx_1e2p_BC, *hdVy_1e2p_BC, *hdVz_1e2p_BC;
    string hdVx_1e2p_BC_Dir, hdVy_1e2p_BC_Dir, hdVz_1e2p_BC_Dir;

    TH1D *hdVx_1e2p_AC, *hdVy_1e2p_AC, *hdVz_1e2p_AC;
    string hdVx_1e2p_AC_Dir, hdVy_1e2p_AC_Dir, hdVz_1e2p_AC_Dir;

    TH1D *hdVx_2p, *hdVy_2p, *hdVz_2p;
    string hdVx_2p_Dir, hdVy_2p_Dir, hdVz_2p_Dir;

    if (apply_cuts == false) {
        hdVx_1e2p_BC = new TH1D("dV_{x} BC (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e}_{x} Before Cuts (1e2p, CD & FD);dV_{x} [cm];", 100, -dV_boundary, dV_boundary);
        hdVy_1e2p_BC = new TH1D("dV_{y} BC (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e}_{y} Before Cuts (1e2p, CD & FD);dV_{y} [cm];", 100, -dV_boundary, dV_boundary);
        hdVz_1e2p_BC = new TH1D("dV_{z} BC (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e}_{z} Before Cuts (1e2p, CD & FD);dV_{z} [cm];", 100, -dV_boundary, dV_boundary);
        hdVx_1e2p_BC_Dir = hdVy_1e2p_BC_Dir = hdVz_1e2p_BC_Dir = directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"];

        hdVx_1e2p_AC = new TH1D("dV_{x} AC (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e}_{x} After Cuts (1e2p, CD & FD);dV_{x} [cm];", 100, -dV_boundary, dV_boundary);
        hdVy_1e2p_AC = new TH1D("dV_{y} AC (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e}_{y} After Cuts (1e2p, CD & FD);dV_{y} [cm];", 100, -dV_boundary, dV_boundary);
        hdVz_1e2p_AC = new TH1D("dV_{z} AC (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e}_{z} After Cuts (1e2p, CD & FD);dV_{z} [cm];", 100, -dV_boundary, dV_boundary);
        hdVx_1e2p_AC_Dir = hdVy_1e2p_AC_Dir = hdVz_1e2p_AC_Dir = directories.Vertex_Directory_map["Vertex_1e2p_dV_AC_Directory"];

        hdVx_2p = new TH1D("dV_{x} (2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e}_{x} (2p, CD & FD);dV_{x} [cm];", 100, -dV_boundary, dV_boundary);
        hdVy_2p = new TH1D("dV_{y} (2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e}_{y} (2p, CD & FD);dV_{y} [cm];", 100, -dV_boundary, dV_boundary);
        hdVz_2p = new TH1D("dV_{z} (2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e}_{z} (2p, CD & FD);dV_{z} [cm];", 100, -dV_boundary, dV_boundary);
        hdVx_2p_Dir = hdVy_2p_Dir = hdVz_2p_Dir = directories.Vertex_Directory_map["Vertex_dV_2p_Directory"];
    } else {
        hdVx_1e2p_BC = new TH1D("dV_{x} (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e}_{x} (1e2p, CD & FD);dV_{x} [cm];", 100, -dV_boundary, dV_boundary);
        hdVy_1e2p_BC = new TH1D("dV_{y} (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e}_{y} (1e2p, CD & FD);dV_{y} [cm];", 100, -dV_boundary, dV_boundary);
        hdVz_1e2p_BC = new TH1D("dV_{z} (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e}_{z} (1e2p, CD & FD);dV_{z} [cm];", 100, -dV_boundary, dV_boundary);
        hdVx_1e2p_BC_Dir = hdVy_1e2p_BC_Dir = hdVz_1e2p_BC_Dir = directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"];

        hdVx_2p = new TH1D("dV_{x} (2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e}_{x} (2p, CD & FD);dV_{x} [cm];", 100, -dV_boundary, dV_boundary);
        hdVy_2p = new TH1D("dV_{y} (2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e}_{y} (2p, CD & FD);dV_{y} [cm];", 100, -dV_boundary, dV_boundary);
        hdVz_2p = new TH1D("dV_{z} (2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e}_{z} (2p, CD & FD);dV_{z} [cm];", 100, -dV_boundary, dV_boundary);
        hdVx_2p_Dir = hdVy_2p_Dir = hdVz_2p_Dir = directories.Vertex_Directory_map["Vertex_dV_2p_Directory"];
    }
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Sampling Fraction (SF) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Sampling Fraction (SF) histograms (FD only)">

    //<editor-fold desc="SF histograms (1e cut, FD only)">
    THStack *sSF_1e_cut_FD = new THStack("SF (1e Cut, FD)", "Sampling Fraction (f = #frac{E_{PCAL}+E_{IN}+E_{OUT}}{P_{e}}) Histogram (1e Cut, FD);f");
    TH1D *hSF_1e_cut_BC_FD, *hSF_1e_cut_AC_FD;
    string hSF_1e_cut_BC_FD_Dir, hSF_1e_cut_AC_FD_Dir;

    if (apply_cuts == false) {
        hSF_1e_cut_BC_FD = new TH1D("SF BC (1e Cut, FD)", "Sampling Fraction f Before Cuts (1e Cut, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                    100, SF_lboundary, SF_uboundary);
        hSF_1e_cut_AC_FD = new TH1D("SF AC (1e Cut, FD)", "Sampling Fraction f After Cuts (1e Cut, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                    100, SF_lboundary, SF_uboundary);
        hSF_1e_cut_BC_FD_Dir = directories.SF_Directory_map["SF_plots_1e_cut_BC_Directory"];
        hSF_1e_cut_AC_FD_Dir = directories.SF_Directory_map["SF_plots_1e_cut_AC_Directory"];
    } else {
        hSF_1e_cut_BC_FD = new TH1D("SF (1e Cut, FD)", "Sampling Fraction f (1e Cut, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 100, SF_lboundary, SF_uboundary);
        hSF_1e_cut_BC_FD_Dir = directories.SF_Directory_map["SF_plots_1e_cut_BC_Directory"];
    }
    //</editor-fold>

    //<editor-fold desc="SF vs. P histograms (1e cut, FD only)">
    TH2D *hSF_VS_P_e_1e_cut_BC_FD, *hSF_VS_P_e_1e_cut_AC_FD;
    string hSF_VS_P_e_1e_cut_BC_FD_Dir, hSF_VS_P_e_1e_cut_AC_FD_Dir;

    if (apply_cuts == false) {
        hSF_VS_P_e_1e_cut_BC_FD = new TH2D("SF vs. P_{e} BC (1e Cut, FD)",
                                           "Sampling Fraction f vs. P_{e} Before Cuts (1e Cut, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           250, 0, beamE * 1.1, 250, SF_lboundary, SF_uboundary);
        hSF_VS_P_e_1e_cut_AC_FD = new TH2D("SF vs. P_{e} AC (1e Cut, FD)",
                                           "Sampling Fraction f vs. P_{e} After Cuts (1e Cut, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           250, 0, beamE * 1.1, 250, SF_lboundary, SF_uboundary);
        hSF_VS_P_e_1e_cut_BC_FD_Dir = directories.SF_Directory_map["SF_VS_P_e_plots_1e_cut_BC_Directory"];
        hSF_VS_P_e_1e_cut_AC_FD_Dir = directories.SF_Directory_map["SF_VS_P_e_plots_1e_cut_AC_Directory"];
    } else {
        hSF_VS_P_e_1e_cut_BC_FD = new TH2D("SF vs. P_{e} (1e Cut, FD)", "Sampling Fraction f vs. P_{e} (1e Cut, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           250, 0, beamE * 1.1, 250, SF_lboundary, SF_uboundary);
        hSF_VS_P_e_1e_cut_BC_FD_Dir = directories.SF_Directory_map["SF_VS_P_e_plots_1e_cut_BC_Directory"];
    }
    //</editor-fold>

    //<editor-fold desc="SF histograms (2p, FD only)">
    THStack *sSF_2p_FD = new THStack("SF (2p, FD)", "Sampling Fraction (f = #frac{E_{PCAL}+E_{IN}+E_{OUT}}{P}) Histogram (2p, FD);f");
    TH1D *hSF_2p_FD = new TH1D("SF (2p, FD)", "Sampling Fraction f (2p, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 100, SF_lboundary, SF_uboundary);
    TH2D *hSF_VS_P_e_2p_FD = new TH2D("SF vs. P_{e} (2p, FD)", "Sampling Fraction f vs. P_{e} (2p, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                      250, 0, beamE * 1.1, 250, SF_lboundary, SF_uboundary);
    string hSF_2p_FD_Dir = directories.SF_Directory_map["SF_plots_2p_cuts_Directory"];
    string hSF_VS_P_e_2p_FD_Dir = directories.SF_Directory_map["SF_VS_P_e_plots_2p_cuts_Directory"];
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ECAL edge histograms (electrons only, FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="ECAL edge histograms (electrons only, FD only)">
    TH2D *Vcal_VS_EoP_1e_BC_PCAL, *Vcal_VS_EoP_1e_AC_PCAL, *Wcal_VS_EoP_1e_BC_PCAL, *Wcal_VS_EoP_1e_AC_PCAL;
    string Vcal_VS_EoP_1e_BC_PCAL_Dir, Vcal_VS_EoP_1e_AC_PCAL_Dir, Wcal_VS_EoP_1e_BC_PCAL_Dir, Wcal_VS_EoP_1e_AC_PCAL_Dir;

    if (apply_cuts == false) {
        Vcal_VS_EoP_1e_BC_PCAL = new TH2D("Vcal vs. SF BC (1e Cut, PCAL)",
                                          "ECAL V coordinate vs. SF Before Cuts (1e Cut, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, SF_lboundary, SF_uboundary);
        Vcal_VS_EoP_1e_AC_PCAL = new TH2D("Vcal vs. SF AC (1e Cut, PCAL)",
                                          "ECAL V coordinate vs. SF After Cuts (1e Cut, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, SF_lboundary, SF_uboundary);
        Vcal_VS_EoP_1e_BC_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"];
        Vcal_VS_EoP_1e_AC_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_1e_AC_PCAL_Directory"];

        Wcal_VS_EoP_1e_BC_PCAL = new TH2D("Wcal vs. SF BC (1e Cut, PCAL)",
                                          "ECAL W coordinate vs. SF Before Cuts (1e Cut, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, SF_lboundary, SF_uboundary);
        Wcal_VS_EoP_1e_AC_PCAL = new TH2D("Wcal vs. SF AC (1e Cut, PCAL)",
                                          "ECAL W coordinate vs. SF After Cuts (1e Cut, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, SF_lboundary, SF_uboundary);
        Wcal_VS_EoP_1e_BC_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"];
        Wcal_VS_EoP_1e_AC_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_1e_AC_PCAL_Directory"];
    } else {
        Vcal_VS_EoP_1e_BC_PCAL = new TH2D("Vcal vs. SF (1e Cut, PCAL)", "ECAL V coordinate vs. SF (1e Cut, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, SF_lboundary, SF_uboundary);
        Vcal_VS_EoP_1e_BC_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"];

        Wcal_VS_EoP_1e_BC_PCAL = new TH2D("Wcal vs. SF (1e Cut, PCAL)", "ECAL W coordinate vs. SF (1e Cut, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, SF_lboundary, SF_uboundary);
        Wcal_VS_EoP_1e_BC_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"];
    }

    TH2D *Vcal_VS_EoP_2p_PCAL = new TH2D("Vcal vs. SF (2p, PCAL)", "ECAL V coordinate vs. SF (2p, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                         250, 0, 50, 250, SF_lboundary, SF_uboundary);
    TH2D *Wcal_VS_EoP_2p_PCAL = new TH2D("Wcal vs. SF (2p, PCAL)", "ECAL W coordinate vs. SF (2p, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                         250, 0, 50, 250, SF_lboundary, SF_uboundary);
    string Vcal_VS_EoP_2p_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_2p_cuts_histograms_PCAL_Directory"];
    string Wcal_VS_EoP_2p_PCAL_Dir = directories.Fiducial_Directory_map["fiducial_plots_2p_cuts_histograms_PCAL_Directory"];
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum histograms">
    /* Here are the definitions for the momentum plots. These are used to check that momentum cuts were applied in 1e_cut and 2p plots. */

    //<editor-fold desc="Momentum histograms (1e cut)">
    THStack *sP_CD = new THStack("Particle Momentum histograms (1e cut, CD)", "Particle Momentum histograms (1e cut, CD);P [GeV];");
    THStack *sP_FD = new THStack("Particle Momentum histograms (1e cut, FD)", "Particle Momentum histograms (1e cut, FD);P [GeV];");
    TH1D *hP_e_1e_cut_FD = new TH1D("P_{e} (1e cut, FD)", "P_{e} (1e cut, FD);P_{e} [GeV];", 100, 0, beamE * 1.1);
    string hP_e_1e_cut_FD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];

    TH1D *hP_p_1e_cut_CD = new TH1D("P_{p} (1e cut, CD)", "P_{p} (1e cut, CD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_p_1e_cut_FD = new TH1D("P_{p} (1e cut, FD)", "P_{p} (1e cut, FD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    string hP_p_1e_cut_CD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];
    string hP_p_1e_cut_FD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];

    TH1D *hP_piplus_1e_cut_CD = new TH1D("P_{#pi^{+}} (1e cut, CD)", "P_{#pi^{+}} (1e cut, CD);P_{#pi^{+}} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_piplus_1e_cut_FD = new TH1D("P_{#pi^{+}} (1e cut, FD)", "P_{#pi^{+}} (1e cut, FD);P_{#pi^{+}} [GeV];", 100, 0, beamE * 1.1);
    string hP_piplus_1e_cut_CD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];
    string hP_piplus_1e_cut_FD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];

    TH1D *hP_piminus_1e_cut_CD = new TH1D("P_{#pi^{-}} (1e cut, CD)", "P_{#pi^{-}} (1e cut, CD);P_{#pi^{-}} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_piminus_1e_cut_FD = new TH1D("P_{#pi^{-}} (1e cut, FD)", "P_{#pi^{-}} (1e cut, FD);P_{#pi^{-}} [GeV];", 100, 0, beamE * 1.1);
    string hP_piminus_1e_cut_CD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];
    string hP_piminus_1e_cut_FD_Dir = directories.Momentum_Directory_map["Momentum_1e_cut_tests_Directory"];
    //</editor-fold>

    //<editor-fold desc="Momentum histograms (1e2p & 2p)">
    THStack *sP_1e2p_BC_CD = new THStack("Particle Momentum BC (1e2p, CD)", "Particle Momentum Histogram BC (1e2p, CD);P [GeV];");
    THStack *sP_1e2p_AC_CD = new THStack("Particle Momentum AC (1e2p, CD)", "Particle Momentum Histogram AC (1e2p, CD);P [GeV];");
    THStack *sP_1e2p_BC_FD = new THStack("Particle Momentum BC (1e2p, FD)", "Particle Momentum Histogram BC (1e2p, FD);P [GeV];");
    THStack *sP_1e2p_AC_FD = new THStack("Particle Momentum AC (1e2p, FD)", "Particle Momentum Histogram AC (1e2p, FD);P [GeV];");
    THStack *sP_1e2p_BC = new THStack("Particle Momentum BC (1e2p, CD & FD)", "Particle Momentum Histogram BC (1e2p, CD & FD);P [GeV];");
    THStack *sP_1e2p_AC = new THStack("Particle Momentum AC (1e2p, CD & FD)", "Particle Momentum Histogram AC (1e2p, CD & FD);P [GeV];");
    THStack *sP_2p_CD = new THStack("Particle Momentum (2p, CD)", "Particle Momentum Histogram (2p, CD);P [GeV];");
    THStack *sP_2p_FD = new THStack("Particle Momentum (2p, FD)", "Particle Momentum Histogram (2p, FD);P [GeV];");

    //<editor-fold desc="Momentum histograms before 1e2p cuts">
    TH1D *hP_e_1e2p_BC_FD = new TH1D("P_{e} BC (1e2p)", "P_{e} Before Cut (1e2p);P_{e} [GeV];", 100, 0, beamE * 1.1);
    string hP_e_1e2p_BC_FD_Dir = directories.Momentum_Directory_map["Momentum_1e2p_Directory"];

    TH1D *hP_p_1e2p_BC_CD = new TH1D("Proton momentum P_{p} BC (1e2p, CD)", "Proton momentum P_{p} Before Cut (1e2p, CD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_p_1e2p_BC_FD = new TH1D("Proton momentum P_{p} BC (1e2p, FD)", "Proton momentum P_{p} Before Cut (1e2p, FD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    string hP_p_1e2p_BC_CD_Dir = directories.Momentum_Directory_map["Momentum_1e2p_Directory"];
    string hP_p_1e2p_BC_FD_Dir = directories.Momentum_Directory_map["Momentum_1e2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after 1e2p cuts">
    TH1D *hP_e_1e2p_AC_FD = new TH1D("P_{e} AC (1e2p)", "P_{e} After Cut (1e2p);P_{e} [GeV];", 100, 0, beamE * 1.1);
    string hP_e_1e2p_AC_FD_Dir = directories.Momentum_Directory_map["Momentum_1e2p_Directory"];

    TH1D *hP_p_1e2p_AC_CD = new TH1D("Proton momentum P_{p} AC (1e2p, CD)", "Proton momentum P_{p} After Cut (1e2p, CD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_p_1e2p_AC_FD = new TH1D("Proton momentum P_{p} AC (1e2p, FD)", "Proton momentum P_{p} After Cut (1e2p, FD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    string hP_p_1e2p_AC_CD_Dir = directories.Momentum_Directory_map["Momentum_1e2p_Directory"];
    string hP_p_1e2p_AC_FD_Dir = directories.Momentum_Directory_map["Momentum_1e2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after 2p cuts">
    TH1D *hP_e_2p_FD = new TH1D("Electron momentum P_{e} (2p)", "Electron momentum P_{e} (2p);P_{e} [GeV];", 100, 0, beamE * 1.1);
    string hP_e_2p_FD_Dir = directories.Momentum_Directory_map["Momentum_2p_Directory"];

    TH1D *hP_p_2p_CD = new TH1D("Proton momentum P_{p} (2p, CD)", "Proton momentum P_{p} (2p, CD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_p_2p_FD = new TH1D("Proton momentum P_{p} (2p, FD)", "Proton momentum P_{p} (2p, FD);P_{p} [GeV];", 100, 0, beamE * 1.1);
    string hP_p_2p_CD_Dir = directories.Momentum_Directory_map["Momentum_2p_Directory"];
    string hP_p_2p_FD_Dir = directories.Momentum_Directory_map["Momentum_2p_Directory"];

    TH1D *hP_p_1_2p = new TH1D("Leading proton P_{1} (2p)", "Leading proton momentum P_{1} (2p);P_{1} [GeV];", 100, 0, beamE * 1.1);
    TH1D *hP_p_2_2p = new TH1D("Recoil proton P_{2} (2p)", "Recoil proton momentum P_{2} (2p);P_{2} [GeV];", 100, 0, beamE * 1.1);
    string hP_p_1_2p_Dir = directories.Momentum_Directory_map["Momentum_2p_Directory"];
    string hP_p_2_2p_Dir = directories.Momentum_Directory_map["Momentum_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="P1 vs P2 (2p, CD & FD)">
    TH2D *hP_p_1_vs_P_p_2_2p = new TH2D("P_{1} vs. P_{2} (2p)", "P_{1} vs. P_{2} (2p);P_{1} [GeV];P_{2} [GeV]", 250, 0, beamE * 1.1, 250, 0, beamE * 1.1);
    string hP_p_1_vs_P_p_2_2p_Dir = directories.Momentum_Directory_map["Momentum_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Beta vs. P histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Beta vs. P histograms">

    //<editor-fold desc="Beta vs. P (all particles)">

    //<editor-fold desc="Beta vs. P (no cuts, CD & FD)">
    TH2D *Beta_vs_P_CD = new TH2D("#beta vs. P (All Particles, CD)", "#beta vs. P (All Particles, CD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_FD = new TH2D("#beta vs. P (All Particles, FD)", "#beta vs. P (All Particles, FD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];
    string Beta_vs_P_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];

    TH2D *Beta_vs_P_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, FD)", "#beta vs. P (Electrons Only, FD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0.5, 3);
    string Beta_vs_P_Electrons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];

    TH2D *Beta_vs_P_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, CD)", "#beta vs. P (Protons Only, CD);P [GeV];#beta",
                                               250, 0, beamE * 1.1, 250, 0, 1.1);
    TH2D *Beta_vs_P_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, FD)", "#beta vs. P (Protons Only, FD);P [GeV];#beta",
                                               250, 0, beamE * 1.1, 250, 0, 1.1);
    string Beta_vs_P_Protons_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];
    string Beta_vs_P_Protons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];

//    TH2D *Beta_vs_P_Neutrons_Only_CD = new TH2D("#beta vs. P (Neutrons Only, CD)", "#beta vs. P (Neutrons Only, CD);P [GeV];#beta",
//                                                250, 0, beamE * 1.425, 250, 0, 3);
//    TH2D *Beta_vs_P_Neutrons_Only_FD = new TH2D("#beta vs. P (Neutrons Only, FD)", "#beta vs. P (Neutrons Only, FD);P [GeV];#beta",
//                                                250, 0, beamE * 1.425, 250, 0, 3);
//    string Beta_vs_P_Neutrons_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];
//    string Beta_vs_P_Neutrons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_All_e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge">
    TH2D *Beta_vs_P_positive_particles_All_e_CD = new TH2D("#beta vs. P & q = +1 (All p., CD)", "#beta vs. P for all particles with q = +1 (All p., CD);P [GeV];#beta",
                                                           250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_positive_particles_All_e_FD = new TH2D("#beta vs. P & q = +1 (All p., FD)", "#beta vs. P for all particles with q = +1 (All p., FD);P [GeV];#beta",
                                                           250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_All_e_CD = new TH2D("#beta vs. P & q = 0 (All p., CD)", "#beta vs. P for all particles with q = 0 (All p., CD);P [GeV];#beta",
                                                          250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_All_e_FD = new TH2D("#beta vs. P & q = 0 (All p., FD)", "#beta vs. P for all particles with q = 0 (All p., FD);P [GeV];#beta",
                                                          250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_All_e_CD = new TH2D("#beta vs. P & q = -1 (All p., CD)", "#beta vs. P for all particles with q = -1 (All p., CD);P [GeV];#beta",
                                                           250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_All_e_FD = new TH2D("#beta vs. P & q = -1 (All p., FD)", "#beta vs. P for all particles with q = -1 (All p., FD);P [GeV];#beta",
                                                           250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_positive_particles_All_e_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"];
    string Beta_vs_P_positive_particles_All_e_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"];
    string Beta_vs_P_neutral_particles_All_e_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"];
    string Beta_vs_P_neutral_particles_All_e_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"];
    string Beta_vs_P_negative_particles_All_e_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"];
    string Beta_vs_P_negative_particles_All_e_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_All_e_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1e cut)">

    //<editor-fold desc="Beta vs. P for all particles (1e cut)">
    TH2D *Beta_vs_P_1e_cut_CD = new TH2D("#beta vs. P (All Particles, 1e only, CD)", "#beta vs. P (All Particles, 1e Cut, CD);P [GeV];#beta",
                                         250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_1e_cut_FD = new TH2D("#beta vs. P (All Particles, 1e only, FD)", "#beta vs. P (All Particles, 1e Cut, FD);P [GeV];#beta",
                                         250, 0, beamE * 1.1, 250, 0, 1.7);
    string Beta_vs_P_1e_cut_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_cut_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, 1e only, FD)", "#beta vs. P (Electrons Only, 1e Cut, FD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 1.7);
    string Beta_vs_P_1e_Electrons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, 1e only, CD)", "#beta vs. P (Protons Only, 1e Cut, CD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, 1e only, FD)", "#beta vs. P (Protons Only, 1e Cut, FD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_1e_Protons_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_Protons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_Neutrons_Only_CD = new TH2D("#beta vs. P (Neutrons Only, 1e only, CD)", "#beta vs. P (Neutrons Only, 1e Cut, CD);P [GeV];#beta",
                                                   250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Neutrons_Only_FD = new TH2D("#beta vs. P (Neutrons Only, 1e only, FD)", "#beta vs. P (Neutrons Only, 1e Cut, FD);P [GeV];#beta",
                                                   250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_1e_Neutrons_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_Neutrons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_piplus_Only_CD = new TH2D("#beta vs. P (#pi^{+} Only, 1e only, CD)", "#beta vs. P (#pi^{+} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, 1.7);
    TH2D *Beta_vs_P_1e_piplus_Only_FD = new TH2D("#beta vs. P (#pi^{+} Only, 1e only, FD)", "#beta vs. P (#pi^{+} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, 1.7);
    string Beta_vs_P_1e_piplus_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_piplus_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_pizero_Only_CD = new TH2D("#beta vs. P (#pi^{0} Only, 1e only, CD)", "#beta vs. P (#pi^{0} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_pizero_Only_FD = new TH2D("#beta vs. P (#pi^{0} Only, 1e only, FD)", "#beta vs. P (#pi^{0} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_1e_pizero_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_pizero_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_piminus_Only_CD = new TH2D("#beta vs. P (#pi^{-} Only, 1e only, CD)", "#beta vs. P (#pi^{-} Only, 1e Cut, CD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, 1.7);
    TH2D *Beta_vs_P_1e_piminus_Only_FD = new TH2D("#beta vs. P (#pi^{-} Only, 1e only, FD)", "#beta vs. P (#pi^{-} Only, 1e Cut, FD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, 1.7);
    string Beta_vs_P_1e_piminus_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_piminus_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_Kplus_Only_CD = new TH2D("#beta vs. P (K^{+} Only, 1e only, CD)", "#beta vs. P (K^{+} Only, 1e Cut, CD);P [GeV];#beta",
                                                250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Kplus_Only_FD = new TH2D("#beta vs. P (K^{+} Only, 1e only, FD)", "#beta vs. P (K^{+} Only, 1e Cut, FD);P [GeV];#beta",
                                                250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_1e_Kplus_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_Kplus_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];

    TH2D *Beta_vs_P_1e_Kminus_Only_CD = new TH2D("#beta vs. P (K^{-} Only, 1e only, CD)", "#beta vs. P (K^{-} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Kminus_Only_FD = new TH2D("#beta vs. P (K^{-} Only, 1e only, FD)", "#beta vs. P (K^{-} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_1e_Kminus_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    string Beta_vs_P_1e_Kminus_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_Only_1e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1e cut)">
    TH2D *Beta_vs_P_positive_particles_1e_cut_CD = new TH2D("#beta vs. P & q = +1 (1e cut, CD)", "#beta vs. P for all particles with q = +1 (1e cut, CD);P [GeV];#beta",
                                                            250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_positive_particles_1e_cut_FD = new TH2D("#beta vs. P & q = +1 (1e cut, FD)", "#beta vs. P for all particles with q = +1 (1e cut, FD);P [GeV];#beta",
                                                            250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_neutral_particles_1e_cut_CD = new TH2D("#beta vs. P & q = 0 (1e cut, CD)", "#beta vs. P for all particles with q = 0 (1e cut, CD);P [GeV];#beta",
                                                           250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_neutral_particles_1e_cut_FD = new TH2D("#beta vs. P & q = 0 (1e cut, FD)", "#beta vs. P for all particles with q = 0 (1e cut, FD);P [GeV];#beta",
                                                           250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_negative_particles_1e_cut_CD = new TH2D("#beta vs. P & q = -1 (1e cut, CD)", "#beta vs. P for all particles with q = -1 (1e cut, CD);P [GeV];#beta",
                                                            250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_negative_particles_1e_cut_FD = new TH2D("#beta vs. P & q = -1 (1e cut, FD)", "#beta vs. P for all particles with q = -1 (1e cut, FD);P [GeV];#beta",
                                                            250, 0, beamE * 1.1, 250, 0, 1.7);
    string Beta_vs_P_positive_particles_1e_cut_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_Only_1e_Directory"];
    string Beta_vs_P_positive_particles_1e_cut_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_Only_1e_Directory"];
    string Beta_vs_P_neutral_particles_1e_cut_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_Only_1e_Directory"];
    string Beta_vs_P_neutral_particles_1e_cut_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_Only_1e_Directory"];
    string Beta_vs_P_negative_particles_1e_cut_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_Only_1e_Directory"];
    string Beta_vs_P_negative_particles_1e_cut_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_Only_1e_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (2p)">

    //<editor-fold desc="Beta vs. P for all particles (2p)">
    TH2D *Beta_vs_P_2p_CD = new TH2D("#beta vs. P (All Particles, 2p, CD)", "#beta vs. P (All Particles, 2p, CD);P [GeV];#beta",
                                     250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_2p_FD = new TH2D("#beta vs. P (All Particles, 2p, FD)", "#beta vs. P (All Particles, 2p, FD);P [GeV];#beta",
                                     250, 0, beamE * 1.1, 250, 0, 1.5);
    string Beta_vs_P_2p_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_2p_Directory"];
    string Beta_vs_P_2p_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_2p_Directory"];

    TH2D *Beta_vs_P_2p_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, 2p, FD)", "#beta vs. P (Electrons Only, 2p, FD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);
    string Beta_vs_P_2p_Electrons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_2p_Directory"];

    TH2D *Beta_vs_P_2p_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, 2p, CD)", "#beta vs. P (Protons Only, 2p, CD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_2p_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, 2p, FD)", "#beta vs. P (Protons Only, 2p, FD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_2p_Protons_Only_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_2p_Directory"];
    string Beta_vs_P_2p_Protons_Only_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (2p)">
    TH2D *Beta_vs_P_positive_particles_2p_CD = new TH2D("#beta vs. P & q = +1 (2p, CD)", "#beta vs. P for all particles with q = +1 (2p, CD);P [GeV];#beta",
                                                        250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_positive_particles_2p_FD = new TH2D("#beta vs. P & q = +1 (2p, FD)", "#beta vs. P for all particles with q = +1 (2p, FD);P [GeV];#beta",
                                                        250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_neutral_particles_2p_CD = new TH2D("#beta vs. P & q = 0 (2p, CD)", "#beta vs. P for all particles with q = 0 (2p, CD);P [GeV];#beta",
                                                       250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_neutral_particles_2p_FD = new TH2D("#beta vs. P & q = 0 (2p, FD)", "#beta vs. P for all particles with q = 0 (2p, FD);P [GeV];#beta",
                                                       250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_negative_particles_2p_CD = new TH2D("#beta vs. P & q = -1 (2p, CD)", "#beta vs. P for all particles with q = -1 (2p, CD);P [GeV];#beta",
                                                        250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_negative_particles_2p_FD = new TH2D("#beta vs. P & q = -1 (2p, FD)", "#beta vs. P for all particles with q = -1 (2p, FD);P [GeV];#beta",
                                                        250, 0, beamE * 1.1, 250, 0, 1.5);
    string Beta_vs_P_positive_particles_2p_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"];
    string Beta_vs_P_positive_particles_2p_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"];
    string Beta_vs_P_neutral_particles_2p_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"];
    string Beta_vs_P_neutral_particles_2p_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"];
    string Beta_vs_P_negative_particles_2p_CD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"];
    string Beta_vs_P_negative_particles_2p_FD_Dir = directories.Beta_VS_P_Directory_map["Beta_VS_P_by_charge_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Angle histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Angle histograms">

// Theta_e --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e histograms">
    THStack *sTheta_e = new THStack("#theta_{e} (CD & FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, CD & FD);#theta_{e} [Deg];");
    THStack *sTheta_e_1e2p_FD = new THStack("#theta_{e} (1e2p, FD)", "#theta_{e} of Outgoing Electron (1e2p, FD);#theta_{e} [Deg];");
    THStack *sTheta_e_2p_FD = new THStack("#theta_{e} (2p, FD)", "#theta_{e} of Outgoing Electron (2p, FD);#theta_{e} [Deg];");

    /* Theta_e histograms (no #(e) cut) */
    TH1D *hTheta_e_All_e_FD = new TH1D("#theta_{e} (no #(e) cut, FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_e_FD_Dir = directories.Angle_Directory_map["Theta_e_All_e_Directory"];

    /* Theta_e histograms (1e cut) */
    TH1D *hTheta_e_1e_cut_FD = new TH1D("#theta_{e} (1e Cut, FD)", "#theta_{e} of Outgoing Electron (1e Cut, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Theta_e_1e_cut_Directory"];

    /* Theta_e histograms (1p) */
    TH1D *hTheta_e_All_Int_1p_FD = new TH1D("#theta_{e} (All Int., 1p, FD)", "#theta_{e} of Outgoing Electron (All Int., 1p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_QEL_1p_FD = new TH1D("#theta_{e} (QEL Only, 1p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_MEC_1p_FD = new TH1D("#theta_{e} (MEC Only, 1p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_RES_1p_FD = new TH1D("#theta_{e} (RES Only, 1p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_DIS_1p_FD = new TH1D("#theta_{e} (DIS Only, 1p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1p, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_Int_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_QEL_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_MEC_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_RES_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_DIS_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];

    /* Theta_e histograms (1e2p) */
    TH1D *hTheta_e_All_Int_1e2p_FD = new TH1D("#theta_{e} (All Int., 1e2p, FD)", "#theta_{e} of Outgoing Electron (All Int., 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_QEL_1e2p_FD = new TH1D("#theta_{e} (QEL Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_MEC_1e2p_FD = new TH1D("#theta_{e} (MEC Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_RES_1e2p_FD = new TH1D("#theta_{e} (RES Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_DIS_1e2p_FD = new TH1D("#theta_{e} (DIS Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_Int_1e2p_FD_Dir = directories.Angle_Directory_map["Theta_e_1e2p_Directory"];
    string hTheta_e_QEL_1e2p_FD_Dir = directories.Angle_Directory_map["Theta_e_1e2p_Directory"];
    string hTheta_e_MEC_1e2p_FD_Dir = directories.Angle_Directory_map["Theta_e_1e2p_Directory"];
    string hTheta_e_RES_1e2p_FD_Dir = directories.Angle_Directory_map["Theta_e_1e2p_Directory"];
    string hTheta_e_DIS_1e2p_FD_Dir = directories.Angle_Directory_map["Theta_e_1e2p_Directory"];

    /* Theta_e histograms (2p) */
    TH1D *hTheta_e_All_Int_2p_FD = new TH1D("#theta_{e} (All Int., 2p, FD)", "#theta_{e} of Outgoing Electron (All Int., 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_QEL_2p_FD = new TH1D("#theta_{e} (QEL Only, 2p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_MEC_2p_FD = new TH1D("#theta_{e} (MEC Only, 2p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_RES_2p_FD = new TH1D("#theta_{e} (RES Only, 2p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_DIS_2p_FD = new TH1D("#theta_{e} (DIS Only, 2p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_Int_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_QEL_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_MEC_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_RES_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_DIS_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    //</editor-fold>

// Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_e histograms">
    THStack *sPhi_e = new THStack("#phi_{e} stack (CD & FD)", "#phi_{e} of Outgoing Electron (no #(e) cut, CD & FD);#phi_{e} [Deg];");

    /* Phi_e histograms (no #(e) cut) */
    TH1D *hPhi_e_All_e_FD = new TH1D("#phi_{e} (no #(e) cut, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    string hPhi_e_All_e_FD_Dir = directories.Angle_Directory_map["Phi_e_All_e_Directory"];

    /* Phi_e histograms (1e cut) */
    TH1D *hPhi_e_1e_cut_FD = new TH1D("#phi_{e} (1e Cut, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    string hPhi_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Phi_e_1e_cut_Directory"];

    /* Phi_e histograms (1p) */
    TH1D *hPhi_e_All_Int_1p_FD = new TH1D("#phi_{e} 1p (All Int., FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_1p_QEL_FD = new TH1D("#phi_{e} for 1p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_1p_MEC_FD = new TH1D("#phi_{e} for 1p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_1p_RES_FD = new TH1D("#phi_{e} for 1p (RES Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_1p_DIS_FD = new TH1D("#phi_{e} for 1p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    string hPhi_e_All_Int_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_1p_QEL_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_1p_MEC_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_1p_RES_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_1p_DIS_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];

    /* Phi_e histograms (1e2p) */
    TH1D *hPhi_e_All_Int_1e2p_FD = new TH1D("#phi_{e} 1e2p (All Int., FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_QEL_1e2p_FD = new TH1D("#phi_{e} for 1e2p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_MEC_1e2p_FD = new TH1D("#phi_{e} for 1e2p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_RES_1e2p_FD = new TH1D("#phi_{e} for 1e2p (RES Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_DIS_1e2p_FD = new TH1D("#phi_{e} for 1e2p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    string hPhi_e_All_Int_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_QEL_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_MEC_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_RES_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_DIS_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];

    /* Phi_e histograms (2p) */
    TH1D *hPhi_e_All_Int_2p_FD = new TH1D("#phi_{e} 2p (All Int., FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_QEL_2p_FD = new TH1D("#phi_{e} for 2p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_MEC_2p_FD = new TH1D("#phi_{e} for 2p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_RES_2p_FD = new TH1D("#phi_{e} for 2p (RES Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    TH1D *hPhi_e_DIS_2p_FD = new TH1D("#phi_{e} for 2p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, -200, 200);
    string hPhi_e_All_Int_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_QEL_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_MEC_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_RES_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_DIS_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    //</editor-fold>

// Theta_e vs. Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e vs. Phi_e">
    /* Theta_e vs. Phi_e histograms (no #(e) cut) */
    TH2D *hTheta_e_VS_Phi_e_All_e_FD = new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, FD)", "#theta_{e} vs. #phi_{e}  (no #(e) cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                250, -200, 200, 250, 0, 50);
    string hTheta_e_VS_Phi_e_All_e_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_All_e_Directory"];

    /* Theta_e vs. Phi_e histograms (1e cut) */
    TH2D *hTheta_e_VS_Phi_e_1e_cut_FD = new TH2D("#theta_{e} vs. #phi_{e} (1e Cut, FD)", "#theta_{e} vs. #phi_{e} (1e Cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                 250, -200, 200, 250, 0, 50);
    string hTheta_e_VS_Phi_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1e_cut_Directory"];

    /* Theta_e vs. Phi_e histograms (1p) */
    TH2D *hTheta_e_VS_Phi_e_1p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 1p, FD)", "#theta_{e} vs. #phi_{e} (All Int., 1p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             250, -200, 200, 250, 0, 50);
    string hTheta_e_VS_Phi_e_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1p_Directory"];

    /* Theta_e vs. Phi_e histograms (2p) */
    TH2D *hTheta_e_VS_Phi_e_2p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 2p, FD)", "#theta_{e} vs. #phi_{e} (All Int., 2p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             250, -200, 200, 250, 0, 50);
    string hTheta_e_VS_Phi_e_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_2p_Directory"];
    //</editor-fold>

// Theta_p_e_p_tot (CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (CD & FD)">
    THStack *sTheta_p_e_p_tot_2p = new THStack("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., 2p)",
                                               "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_p_e_p_tot_2p = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., 2p)",
                                         "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);"
                                         "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];", 100, 0, 180);
    string hTheta_p_e_p_tot_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    //</editor-fold>

// Theta_q_p (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (2p, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_2p = new THStack("#theta_{#vec{q},#vec{P}_{tot}} (All Int., 2p)",
                                             "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);#theta_{#vec{q},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_q_p_tot_2p = new TH1D("#theta_{#vec{q},#vec{P}_{tot}} (All Int., 2p)",
                                       "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);"
                                       "#theta_{#vec{q},#vec{P}_{tot}} [Deg];", 100, 0, 180);
    string hTheta_q_p_tot_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (2p, CD & FD)">
    THStack *sTheta_q_p_2p = new THStack("#theta_{#vec{q},#vec{P}} (All Int., 2p)",
                                         "#theta_{#vec{q},#vec{P}} - Opening Angle Between #vec{q} and #vec{P} (All Int., 2p);#theta_{#vec{q},#vec{P}} [Deg];");
    TH1D *hTheta_q_p_L_2p = new TH1D("#theta_{#vec{q},#vec{P}_{1}} (All Int., 2p)",
                                     "#theta_{#vec{q},#vec{P}_{1}} - Opening Angle Between #vec{q} and leading proton #vec{P}_{1} (All Int., 2p);#theta_{#vec{q},#vec{P}_{1}}",
                                     100, 0, 180);
    TH1D *hTheta_q_p_R_2p = new TH1D("#theta_{#vec{q},#vec{P}_{2}} (All Int., 2p)",
                                     "#theta_{#vec{q},#vec{P}_{2}} - Opening Angle Between #vec{q} and recoil proton #vec{P}_{2} (All Int., 2p);#theta_{#vec{q},#vec{P}_{2}}",
                                     100, 0, 180);
    string hTheta_q_p_L_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    string hTheta_q_p_R_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (2p, CD & FD) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    TH2D *hTheta_q_p_L_vs_p_L_q_2p = new TH2D("#theta_{#vec{q},#vec{P}_{1}} vs. r_{1} (All Int., 2p)",
                                              "#theta_{#vec{q},#vec{P}_{1}} vs. r_{1}=|#vec{P_{1}}|/|#vec{q}| (All Int., 2p);r_{1};#theta_{#vec{q},#vec{P}_{1}}",
                                              250, 0, 1.05, 250, 0, 180);
    string hTheta_q_p_L_vs_p_L_q_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    //</editor-fold>

// Theta_p1_p2 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 (CD & FD)">
    THStack *sTheta_p1_p2_2p = new THStack("#theta_{p_{1},p_{2}} (All Int., 2p)",
                                           "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (2p);#theta_{p_{1},p_{2}} [Deg];");
    TH1D *hTheta_p1_p2_All_Int_2p = new TH1D("#theta_{p_{1},p_{2}} (All Int., 2p)",
                                             "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (All Int., 2p);#theta_{p_{1},p_{2}} [Deg];", 100, 0, 180);
    TH1D *hTheta_p1_p2_QEL_2p = new TH1D("#theta_{p_{1},p_{2}} (QEL only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (QEL only, 2p);#theta_{p_{1},p_{2}} [Deg];", 100, 0, 180);
    TH1D *hTheta_p1_p2_MEC_2p = new TH1D("#theta_{p_{1},p_{2}} (MEC only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (MEC only, 2p);#theta_{p_{1},p_{2}} [Deg];", 100, 0, 180);
    TH1D *hTheta_p1_p2_RES_2p = new TH1D("#theta_{p_{1},p_{2}} (RES only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (RES only, 2p);#theta_{p_{1},p_{2}} [Deg];", 100, 0, 180);
    TH1D *hTheta_p1_p2_DIS_2p = new TH1D("#theta_{p_{1},p_{2}} (DIS only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (DIS only, 2p);#theta_{p_{1},p_{2}} [Deg];", 100, 0, 180);
    string sTheta_p1_p2_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    string hTheta_p1_p2_All_Int_2p_Dir = directories.Angle_Directory_map["Theta_p1_p2_by_interaction_Directory"];
    string hTheta_p1_p2_QEL_2p_Dir = directories.Angle_Directory_map["Theta_p1_p2_by_interaction_Directory"];
    string hTheta_p1_p2_MEC_2p_Dir = directories.Angle_Directory_map["Theta_p1_p2_by_interaction_Directory"];
    string hTheta_p1_p2_RES_2p_Dir = directories.Angle_Directory_map["Theta_p1_p2_by_interaction_Directory"];
    string hTheta_p1_p2_DIS_2p_Dir = directories.Angle_Directory_map["Theta_p1_p2_by_interaction_Directory"];
    //</editor-fold>

// Theta_p1_p2 vs. W (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. W (CD & FD)">
    TH2D *hTheta_p1_p2_vs_W_2p = new TH2D("#theta_{p_{1},p_{2}} vs. W (All Int., 2p)",
                                          "#theta_{p_{1},p_{2}} vs. W (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{p_{1},p_{2}} [Deg];",
                                          250, 0, beamE * 1.1, 250, 0, 180);
    string hTheta_p1_p2_vs_W_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    //</editor-fold>

// Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 10 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 10 (CD & FD)">
    TH2D *hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p = new TH2D("#theta_{p_{1}} vs. #theta_{p_{1}} for #theta_{p_{1},p_{2}}<10#circ (All Int., 2p)",
                                                                 "#theta_{p_{1}} vs. #theta_{p_{2}} for #theta_{p_{1},p_{2}}<10#circ (All Int., 2p);#theta_{p_{2}} [Deg];#theta_{p_{1}} [Deg];",
                                                                 250, 0, 180, 250, 0, 180);
    string hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory"];
    //</editor-fold>

// Phi of leading (p1) and recoil (p2) protons --------------------------------------------------------------------------------------------------------------------------

    //TODO: reorganize proprly

    //<editor-fold desc="Phi of leading (p1) and recoil (p2) protons">
    THStack *sPhi_Proton_1e2pXy = new THStack("#phi_{p} stack (1e2pXy, CD)", "#phi_{p} of outgoing protons (1e2pXy, CD);#phi_{p} [Deg];");
    TH1D *hPhi_p1_1e2pXy_CD = new TH1D("#phi_{p_{1}} (1e2pXy, CD)", ";#phi_{p_{1}} [Deg];", 100, -200, 200);
    TH1D *hPhi_p2_1e2pXy_CD = new TH1D("#phi_{p_{2}} (1e2pXy, CD)", ";#phi_{p_{2}} [Deg];", 100, -200, 200);
    string hPhi_p1_1e2pXy_CD_Dir = directories.Angle_Directory_map["Phi_Proton_1e2pXy_Directory"];
    string hPhi_p2_1e2pXy_CD_Dir = directories.Angle_Directory_map["Phi_Proton_1e2pXy_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Q2 histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Q2 histograms">
    /* Q2 histograms (no #(e) cut) */
    THStack *sQ2_All_e = new THStack("Q^{2} (no #(e) cut, CD & FD)", "Q^{2} Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_All_e_FD = new TH1D("Q^{2} (no #(e) cut, FD)", "Q^{2} (no #(e) cut, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_All_e_FD_Dir = directories.Q2_Directory_map["Q2_All_e_Directory"];

    /* Q2 histograms (1e cut) */
    THStack *sQ2_1e_cut = new THStack("Q^{2} (1e Cut ,CD & FD)", "Q^{2} Histogram (1e Cut, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1e_cut_FD = new TH1D("Q^{2} (1e Cut, FD)", "Q^{2} (1e Cut, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1e_cut_FD_Dir = directories.Q2_Directory_map["Q2_1e_cut_Directory"];


    /* Q2 histograms (1p) */
    THStack *sQ2_1p = new THStack("Q^{2} (1p, CD & FD)", "Q^{2} (1p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1p_FD = new TH1D("Q^{2} (1p, FD)", "Q^{2} (1p, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1p_FD_Dir = directories.Q2_Directory_map["Q2_1p_Directory"];

    /* Q2 histograms (1e2p) */
    THStack *sQ2_1e2p = new THStack("Q^{2} (1e2p, CD & FD)", "Q^{2} (1e2p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1e2p_FD = new TH1D("Q^{2} (1e2p, FD)", "Q^{2} (1e2p, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1e2p_FD_Dir = directories.Q2_Directory_map["Q2_1e2p_Directory"];

    /* Q2 histograms (2p) */
    THStack *sQ2_2p = new THStack("Q^{2} (2p, CD & FD)", "Q^{2} (2p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_2p_FD = new TH1D("Q^{2} (2p, FD)", "Q^{2} (2p, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_2p_FD_Dir = directories.Q2_Directory_map["Q2_2p_Directory"];
    //</editor-fold>

// ======================================================================================================================================================================
// Energy (E_e) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy (E_e) histograms">
    /* Energy (E_e) histograms (1e cut, CD & FD) */
    THStack *sE_e_1e_cut = new THStack("E_{e} (CD & FD)", "E_{e} Histogram (1e Cut, CD & FD);E_{e} [GeV]");
    TH1D *hE_e_1e_cut_FD = new TH1D("E_{e} (1e Cut)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];

    TH2D *hE_e_VS_Theta_e_All_Int_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1e Cut)",
                                                       "E_{e} vs. #theta_{e} (All Int., 1e Cut);#theta_{e} [Deg];E_{e} [GeV]", 250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];

    /* Energy (E_e) histograms (2p, CD & FD) */
    THStack *sE_e_2p_FD = new THStack("E_{e} (2p)", "E_{e} Histogram (2p);E_{e} [GeV]");
    string sE_e_2p_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_stack_Directory"];
    string sE_e_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_stack_Directory"];

    TH1D *hE_e_All_Int_2p_FD = new TH1D("E_{e} (All Int., 2p)", "E_{e} Histogram (All Int., 2p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_QEL_2p_FD = new TH1D("E_{e} (QEL Only, 2p)", "E_{e} Histogram (QEL Only, 2p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_MEC_2p_FD = new TH1D("E_{e} (MEC Only, 2p)", "E_{e} Histogram (MEC Only, 2p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_RES_2p_FD = new TH1D("E_{e} (RES Only, 2p)", "E_{e} Histogram (RES Only, 2p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_DIS_2p_FD = new TH1D("E_{e} (DIS Only, 2p)", "E_{e} Histogram (DIS Only, 2p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_All_Int_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_QEL_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_MEC_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_RES_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_DIS_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];

    /* E_e vs. Theta_e (2p, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_2p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int.)", "E_{e} vs. #theta_{e} (All Int., 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                                   250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_2p_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p)", "E_{e} vs. #theta_{e} (QEL Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_2p_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 2p)", "E_{e} vs. #theta_{e} (MEC Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_2p_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 2p)", "E_{e} vs. #theta_{e} (RES Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_2p_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 2p)", "E_{e} vs. #theta_{e} (DIS Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_QEL_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_MEC_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_RES_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_DIS_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (2p, CD & FD) */
    THStack *sE_e_15_2p_FD = new THStack("E_{e} (2p)", "E_{e} Histogram (2p);E_{e} [GeV]");
    string sE_e_15_2p_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_stack_Directory"];
    string sE_e_15_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_stack_Directory"];

    TH1D *hE_e_15_All_Int_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 2p)", "E_{e} around #theta_{e} = 15#circ (All Int., 2p);E_{e} [GeV]",
                                           100, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 2p)", "E_{e} around #theta_{e} = 15#circ (QEL Only, 2p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, 2p)", "E_{e} around #theta_{e} = 15#circ (MEC Only, 2p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, 2p)", "E_{e} around #theta_{e} = 15#circ (RES Only, 2p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, 2p)", "E_{e} around #theta_{e} = 15#circ (DIS Only, 2p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    string hE_e_15_All_Int_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_QEL_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_MEC_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_RES_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_DIS_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    //</editor-fold>

// ======================================================================================================================================================================
// Energy Transfer (ET) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy Transfer (ET) histograms">

    string tET_All_Ang = "Energy Transfer #omega for every #theta_{e}", tET15 = "Energy Transfer #omega Around #theta_{e} = 15#circ";

    //<editor-fold desc="ET for every theta_e (1p)">
    THStack *sET_All_Ang_All_Int_1p_FD = new THStack("#omega for all #theta_{e} (1p)", "Energy Transfer #omega for all #theta_{e} (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1p_Directory"];

    TH1D *hET_All_Ang_All_Int_1p_FD = new TH1D("#omega for all #theta_{e} (All Int., 1p)",
                                               "Energy Transfer #omega for all #theta_{e} (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1p_Directory"];
    string hET_All_Ang_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1p_Directory"];
    string hET_All_Ang_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1p_Directory"];
    string hET_All_Ang_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1p_Directory"];
    string hET_All_Ang_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (1p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_1p_FD = new THStack("ET around #theta_{e} = 15#circ (1p)",
                                               "Energy Transfer #omega Around #theta_{e} = 15#circ (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_1p_Directory"];

    TH1D *hET15_All_Int_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (All Int., 1p)",
                                         "Energy Transfer #omega Around #theta_{e} = 15#circ (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_QEL_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_MEC_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_RES_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_DIS_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_1p_Directory"];
    string hET15_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_1p_Directory"];
    string hET15_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_1p_Directory"];
    string hET15_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_1p_Directory"];
    string hET15_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="#omega for every theta_e (2p)">
    THStack *sET_All_Ang_All_Int_2p_FD = new THStack("#omega for all #theta_{e} (2p)", "Energy Transfer #omega for all #theta_{e} (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_2p_Directory"];

    TH1D *hET_All_Ang_All_Int_2p_FD = new TH1D("#omega for all #theta_{e} (All Int., 2p)",
                                               "Energy Transfer #omega for all #theta_{e} (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_2p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 2p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_2p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 2p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_2p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 2p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_2p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 2p, FD)",
                                           "Energy Transfer #omega for all #theta_{e} (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_2p_Directory"];
    string hET_All_Ang_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_2p_Directory"];
    string hET_All_Ang_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_2p_Directory"];
    string hET_All_Ang_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_2p_Directory"];
    string hET_All_Ang_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (2p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_2p_FD = new THStack("#omega around #theta_{e} = 15#circ (2p)",
                                               "Energy Transfer #omega Around #theta_{e} = 15#circ (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_2p_Directory"];

    TH1D *hET15_All_Int_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., 2p)",
                                         "Energy Transfer #omega Around #theta_{e} = 15#circ (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_QEL_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, 2p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_MEC_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, 2p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_RES_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, 2p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_DIS_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, 2p, FD)",
                                     "Energy Transfer #omega Around #theta_{e} = 15#circ (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET15_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_2p_Directory"];
    string hET15_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_2p_Directory"];
    string hET15_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_2p_Directory"];
    string hET15_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_2p_Directory"];
    string hET15_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Ecal reconstruction histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Ecal reconstruction histograms">

    //<editor-fold desc="Ecal histograms">

    //<editor-fold desc="Ecal histograms (2p)">
    THStack *sEcal_2p = new THStack("E_{cal} Reconstruction (2p)", "E_{cal} Reconstruction (2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]");
    string sEcal_2p_Dir = directories.Ecal_Directory_map["Ecal_stack_2p_Directory"];

    TH1D *hEcal_All_Int_2p = new TH1D("E_{cal} rec. (All Int., 2p)", "E_{cal} Reconstruction (All Int., 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                      100, 0, beamE * 1.35);
    TH1D *hEcal_QEL_2p = new TH1D("E_{cal} rec. (QEL only, 2p)", "E_{cal} Reconstruction (QEL only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_MEC_2p = new TH1D("E_{cal} rec. (MEC only, 2p)", "E_{cal} Reconstruction (MEC only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_RES_2p = new TH1D("E_{cal} rec. (RES only, 2p)", "E_{cal} Reconstruction (RES only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_DIS_2p = new TH1D("E_{cal} rec. (DIS only, 2p)", "E_{cal} Reconstruction (DIS only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  100, 0, beamE * 1.35);
    string hEcal_All_Int_2p_Dir = directories.Ecal_Directory_map["Ecal_All_Int_2p_Directory"];
    string hEcal_QEL_2p_Dir = directories.Ecal_Directory_map["Ecal_QEL_2p_Directory"];
    string hEcal_MEC_2p_Dir = directories.Ecal_Directory_map["Ecal_MEC_2p_Directory"];
    string hEcal_RES_2p_Dir = directories.Ecal_Directory_map["Ecal_RES_2p_Directory"];
    string hEcal_DIS_2p_Dir = directories.Ecal_Directory_map["Ecal_DIS_2p_Directory"];

    //<editor-fold desc="Chi2 plots for Ecal>Ebeam (2p)">
    TH1D *hChi2_Electron_Ecal_test_2p = new TH1D("#chi^{2}_{e} for E_{cal}>E_{beam} (2p)", ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                            " [GeV] (2p);#chi^{2}_{e};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_Ecal_test_2p = new TH1D("#chi^{2}_{p} for E_{cal}>E_{beam} (2p)", ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                          " [GeV] (2p);#chi^{2}_{p};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_Ecal_test_2p_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_2p_Directory"];
    string hChi2_Proton_Ecal_test_2p_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T">
    TH2D *hEcal_vs_dAlpha_T_L_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                            "E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                            250, 0, 180, 250, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                              "E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                              250, 0, 180, 250, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    string hEcal_vs_dAlpha_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T">
    TH2D *hEcal_vs_dP_T_L_2p = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., 2p)",
                                        "E_{cal} vs. #deltaP_{T,L} (All Int., 2p);#delta#P_{T,L} [Deg];E_{cal} [GeV];", 250, 0, dP_T_boundary, 250, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_2p = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., 2p)",
                                          "E_{cal} vs. #deltaP_{T,tot} (All Int., 2p);#deltaP_{T,tot} [Deg];E_{cal} [GeV];", 250, 0, dP_T_boundary, 250, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    string hEcal_vs_dP_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Transverse variables histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Transverse variables histograms">
    THStack *sdP_T_2p = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (2p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (2p);#deltaP_{T} [GeV]");
    TH1D *hdP_T_L_2p = new TH1D("#deltaP_{T,L} (2p)", "#deltaP_{T,L} by Leading Proton (2p);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,1}| [GeV]", 100, 0,
                                dP_T_boundary);
    TH1D *hdP_T_tot_2p = new TH1D("#deltaP_{T,tot} (2p)",
                                  "#deltaP_{T,tot} by Momentum Sum (2p);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,1} + #vec{p}_{T,2}| [GeV]", 100, 0, dP_T_boundary);
    string hdP_T_L_2p_Dir = directories.TVariables_Directory_map["dP_T_2p_Directory"];
    string hdP_T_tot_2p_Dir = directories.TVariables_Directory_map["dP_T_2p_Directory"];

    THStack *sdAlpha_T_2p = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (2p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_2p = new TH1D("#delta#alpha_{T,L} (2p)", "#delta#alpha_{T,L} by Leading Proton (2p);#delta#alpha_{T,L} [Deg]", 100, -10, 200);
    TH1D *hdAlpha_T_tot_2p = new TH1D("#delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,tot} by Momentum Sum (2p);#delta#alpha_{T,tot} [Deg]", 100, -10, 200);
    string hdAlpha_T_L_2p_Dir = directories.TVariables_Directory_map["dAlpha_T_2p_Directory"];
    string hdAlpha_T_tot_2p_Dir = directories.TVariables_Directory_map["dAlpha_T_2p_Directory"];

    THStack *sdPhi_T_2p = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (2p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (2p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_2p = new TH1D("#delta#phi_{T,L} (2p)", "#delta#phi_{T,L} by Leading Proton (2p);#delta#phi_{T,L} [Deg]", 100, -10, 200);
    TH1D *hdPhi_T_tot_2p = new TH1D("#delta#phi_{T,tot} (2p)", "#delta#phi_{T,tot} by Momentum Sum (2p);#delta#phi_{T,tot} [Deg]", 100, -10, 200);
    string hdPhi_T_L_2p_Dir = directories.TVariables_Directory_map["dPhi_T_2p_Directory"];
    string hdPhi_T_tot_2p_Dir = directories.TVariables_Directory_map["dPhi_T_2p_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_2p = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                              "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV];",
                                              250, 0, 180, 250, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_2p = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                                  "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV];",
                                                  250, 0, 180, 250, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_2p_Dir = directories.TVariables_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_2p_Dir = directories.TVariables_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code execution                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Code execution">
    cout << "Reading target parameter files\n\n";

    clas12ana clasAna;

    if (apply_cuts == true) {
        /* Read in target parameter files */
        if (apply_chi2_cuts_1e_cut == false) {
            clasAna.readInputParam((CutsDirectory + "ana.par").c_str());
        } else if (apply_chi2_cuts_1e_cut == true) {
            cout << "Loading fitted pid cuts...\n\n";
            clasAna.readInputParam((CutsDirectory + "Fitted_PID_Cuts__" + SampleName + ".par").c_str()); // load sample-appropreate cuts file from CutsDirectory

            /* Overwriting cuts according to SampleName */
            Chi2_Proton_cuts_CD.SetCutPram(clasAna.GetPidCutMean(2212, "CD"), -clasAna.GetPidCutSigma(2212, "CD"), clasAna.GetPidCutSigma(2212, "CD"));
            Chi2_Proton_cuts_FD.SetCutPram(clasAna.GetPidCutMean(2212, "FD"), -clasAna.GetPidCutSigma(2212, "FD"), clasAna.GetPidCutSigma(2212, "FD"));
            Chi2_piplus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(211, "CD"), -clasAna.GetPidCutSigma(211, "CD"), clasAna.GetPidCutSigma(211, "CD"));
            Chi2_piplus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(211, "FD"), -clasAna.GetPidCutSigma(211, "FD"), clasAna.GetPidCutSigma(211, "FD"));
            Chi2_piminus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(-211, "CD"), -clasAna.GetPidCutSigma(-211, "CD"), clasAna.GetPidCutSigma(-211, "CD"));
            Chi2_piminus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(-211, "FD"), -clasAna.GetPidCutSigma(-211, "FD"), clasAna.GetPidCutSigma(-211, "FD"));
        }

        clasAna.readEcalPar((CutsDirectory + "ecal.par").c_str());

        // Cuts on electrons only:
        if (apply_SF_cuts == true) { // making f_ecalSFCuts = ture
            SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.248125, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());
            clasAna.setEcalSFCuts();
        }

        if (apply_ECAL_fiducial_cuts == true) { // making f_ecalEdgeCuts = ture (ECAL fiducial cuts)
            PCAL_edge_cuts = DSCuts("PCAL edge", "FD", "Electron", "1e cut", 0, clasAna.getEcalEdgeCuts());
            clasAna.setEcalEdgeCuts();
        }
        if (apply_Nphe_cut == true) { // making f_NpheCuts = ture (HTCC cuts)
            Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, clasAna.getNpheCuts());
            clasAna.setNpheCuts();
        }

        // Cuts on protons and charged pions:
        if (apply_chi2_cuts_1e_cut == true) { // making f_pidCuts = ture
            clasAna.setPidCuts();
        }

        // Cuts on all particles:
        if (apply_Vz_cuts == true) {
            clasAna.setVertexCuts(); // making f_vertexCuts = ture
            clasAna.setVzcuts(Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut()); // setting Vz cuts for all (charged?) particles
        }

        // Cuts on charged particles:
        if (apply_DC_fiducial_cut == true) { // making f_DCEdgeCuts = ture (DC fiducial cuts?)
            DC_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, clasAna.getDCEdgeCuts());
            clasAna.setDCEdgeCuts();
        }

        if (apply_dVz_cuts == true) {
            clasAna.setVertexCorrCuts(); // making f_corr_vertexCuts = ture
            clasAna.setVertexCorrCuts(dVz_cuts.GetLowerCut(), dVz_cuts.GetUpperCut()); // setting dVz cuts?
        }

        clasAna.printParams();
    }

    cout << "\n\nLooping over chain files...\n\n";

    clas12root::HipoChain chain;
    chain.Add(AnalyseFile.c_str());
    chain.SetReaderTags({0});
    auto config_c12 = chain.GetC12Reader();
    auto &c12 = chain.C12ref();

    auto db = TDatabasePDG::Instance();
    chain.db()->turnOffQADB();

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Setting beam particle's momentum
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Setting beam particle's momentum">
    double Pv = beamE, Pvx = 0., Pvy = 0., Pvz = Pv; // Assuming momentum of incoming lepton is in the z direction

    TLorentzVector beam(0, 0, beamE, beamE);
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Counting variable definitions
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Counting variable definitions">
    int num_of_events = 0, num_of_events_without_any_e = 0, num_of_events_with_any_e = 0;
    int num_of_QEL_events = 0, num_of_MEC_events = 0, num_of_RES_events = 0, num_of_DIS_events = 0;

    int num_of_events_with_at_least_1e = 0, num_of_events_with_exactly_1e = 0, num_of_events_with_exactly_1e_from_file = 0, num_of_events_more_then_1e = 0;

    int num_of_events_1e1p_all = 0, num_of_events_1e2p_all = 0, num_of_events_1e2p_all_wo_FDph = 0;

    int num_of_events_with_1e2X = 0, num_of_events_with_1enP = 0, num_of_events_with_1e1p = 0, num_of_events_with_1e2p = 0;

    //TODO: change to 1e2X #(events) with e in det by reaction
    int num_of_QEL_1e2X_CD_events = 0, num_of_MEC_1e2X_CD_events = 0, num_of_RES_1e2X_CD_events = 0, num_of_DIS_1e2X_CD_events = 0;
    int num_of_QEL_1e2X_FD_events = 0, num_of_MEC_1e2X_FD_events = 0, num_of_RES_1e2X_FD_events = 0, num_of_DIS_1e2X_FD_events = 0;
    int num_of_QEL_1e2p_CD_events = 0, num_of_MEC_1e2p_CD_events = 0, num_of_RES_1e2p_CD_events = 0, num_of_DIS_1e2p_CD_events = 0;
    int num_of_QEL_1e2p_FD_events = 0, num_of_MEC_1e2p_FD_events = 0, num_of_RES_1e2p_FD_events = 0, num_of_DIS_1e2p_FD_events = 0;

    int num_of_events_1e2p_w_eChi2_cut_only_CD = 0, num_of_events_1e2p_w_eChi2_cut_only_FD = 0;
    int num_of_events_1e2p_w_pChi2_cut_only_CD = 0, num_of_events_1e2p_w_pChi2_cut_only_FD = 0;
    int num_of_events_1e2p_w_allChi2_cuts_CD = 0, num_of_events_1e2p_w_allChi2_cuts_FD = 0;
    int num_of_events_1e2p_w_allChi2_cuts = 0;

    /* 1e2p = 2p 1e + electron cuts only; 2p = 1e2p with all other cuts */
    int num_of_events_2p = 0; // = number of 2p events
    int num_of_QEL_2p_events = 0, num_of_MEC_2p_events = 0, num_of_RES_2p_events = 0, num_of_DIS_2p_events = 0;
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Looping over each HipoChain file
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    while (chain.Next()) { // loop over events
        ++num_of_events; // logging Total #(events)

        auto electrons_det = c12->getByID(11);
        if (electrons_det.size() == 1) { ++num_of_events_with_exactly_1e_from_file; }

        clasAna.Run(c12);

        /* All of these particles are with clas12ana cuts
           Only cuts missing are Nphe and momentum cuts - to be applied later */
        auto protons = clasAna.getByPid(2212);   // Protons
        auto Kplus = clasAna.getByPid(321);      // K+
        auto Kminus = clasAna.getByPid(-321);    // K-
        auto piplus = clasAna.getByPid(211);     // pi+
        auto piminus = clasAna.getByPid(-211);   // pi-
        auto electrons = clasAna.getByPid(11);   // Electrons

        auto deuterons = clasAna.getByPid(45);   // Deuterons
        auto neutrals = clasAna.getByPid(0);     // Neutrons
        auto otherpart = clasAna.getByPid(311);  // Other particles

        /* Number of specific particles in event */
        int Np = protons.size(), Nkp = Kplus.size(), Nkm = Kminus.size(), Npip = piplus.size(), Npim = piminus.size(), Ne = electrons.size();
        int Nd = deuterons.size(), Nn = neutrals.size(), No = otherpart.size();

        /* Total number of particles in event (= Nf) */
        int Nf = Np + Nkp + Nkm + Npip + Npim + Ne + Nd + Nn + No;

        //<editor-fold desc="Nph_CD, Nph_FD, No_Prime and Nf_Prime declarations and definitions">
        int Nph_CD = 0, Nph_FD = 0;

        for (int i = 0; i < No; i++) {
            if ((otherpart[i]->getRegion() == CD) && (otherpart[i]->par()->getPid() == 22)) {
                ++Nph_CD;
            } else if ((otherpart[i]->getRegion() == FD) && (otherpart[i]->par()->getPid() == 22)) {
                ++Nph_FD;
            }
        } // end of loop over otherpart vector

        /* No_Prime does not include:
         * Neutrals
         * Photons in the CD */
        int No_Prime = No; // don't ignore photons in CD
//        int No_Prime = No - Nph_CD; // ignore photons in CD
        int Nf_Prime = Np + Nkp + Nkm + Npip + Npim + Ne + Nd + No_Prime;
        //</editor-fold>

        bool qel = false, mec = false, res = false, dis = false;
        double processID = c12->mcevent()->getWeight(); // code = 1.,2.,3.,4. = type = qel, mec, res, dis

        //<editor-fold desc="Log total #(events) with and without electrons">
        if (Ne == 0) {
            ++num_of_events_without_any_e; // logging Total #(events) w/o any e
        } else {
            ++num_of_events_with_any_e; // logging Total #(events) w/ any e
        }
        //</editor-fold>

        //<editor-fold desc="Identify event process ID">
        if (processID == 1.) {
            ++num_of_QEL_events;
            qel = true;
        } else if (processID == 2.) {
            ++num_of_MEC_events;
            mec = true;
        } else if (processID == 3.) {
            ++num_of_RES_events;
            res = true;
        } else if (processID == 4.) {
            ++num_of_DIS_events;
            dis = true;
        }
        //</editor-fold>

        //<editor-fold desc="Debugging (print events to file)">
        if (PrintEvents == true) {
            bool EventPrintSelection = (Ne == Ne_in_event && Nf >= Nf_in_event);
            int event = c12->runconfig()->getEvent();

            if (EventPrintSelection == true) {
                if (event < (nEvents2print + 1)) {
                    EventPrint << "--- EVENT NUMBER " << event << " ---\n";
                    EventPrint << "#particles in event:\t" << Nf << "\n";
                    EventPrint << "protons.size() = " << protons.size() << "\n";
                    EventPrint << "Kplus.size() = " << Kplus.size() << "\n";
                    EventPrint << "Kminus.size() = " << Kminus.size() << "\n";
                    EventPrint << "piplus.size() = " << piplus.size() << "\n";
                    EventPrint << "piminus.size() = " << piminus.size() << "\n";
                    EventPrint << "electrons.size() = " << electrons.size() << "\n";
                    EventPrint << "deuterons.size() = " << deuterons.size() << "\n";
                    EventPrint << "neutrals.size() = " << neutrals.size() << "\n";
                    EventPrint << "otherpart.size() = " << otherpart.size() << "\n\n\n";
                }
            }
        }
        //</editor-fold>

        //  Fill All particles (All e) plots ------------------------------------------------------------------------------------------------------------------------------------

        // TODO: remove or change - clas12ana comes with 1e cut already

        //<editor-fold desc="All particles plots">
        /* Declaration of electron variables for all particles analysis.
         * Variables with "tmp" will be used only in all particles plots.
         * Variables without "tmp" will be used later as well.
         * If electrons.size() == 1 (always the case when applyng clas12ana cuts), then e_out is the outgoing electron 4-momentum. */
        TLorentzVector e_out, Q;
        double Theta_e_tmp, Phi_e_tmp, P_e_tmp, Q2;

        //<editor-fold desc="Electron plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Ne; i++) {
            /* Definition of electron variables for all particles analysis.
             * To be filled by region (CD or FD) */
            Theta_e_tmp = electrons[i]->getTheta() * 180.0 / pi; // Theta_e_tmp in deg
            Phi_e_tmp = electrons[i]->getPhi() * 180.0 / pi;     // Phi_e_tmp in deg
            P_e_tmp = electrons[i]->par()->getP();               // temp electron momentum
            //TODO: fix the SetLorentzVector function
//            SetLorentzVector(e_out, electrons[i], m_e);   // definition of outgoing electron 4-momentum
            e_out.SetPxPyPzE(electrons[i]->par()->getPx(), electrons[i]->par()->getPy(), electrons[i]->par()->getPz(), sqrt(m_e * m_e + P_e_tmp * P_e_tmp));
            Q = beam - e_out;                                    // definition of 4-momentum transfer
            Q2 = fabs(Q.Mag2());

            if (electrons[i]->getRegion() == FD) {
                hChi2_Electron_FD->Fill(electrons[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_Electrons_Only_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());

                hTheta_e_All_e_FD->Fill(Theta_e_tmp);
                hPhi_e_All_e_FD->Fill(Phi_e_tmp);
                hTheta_e_VS_Phi_e_All_e_FD->Fill(Phi_e_tmp, Theta_e_tmp);

                hQ2_All_e_FD->Fill(Q2);

                if (Ne == 1) { hQ2_1e_cut_FD->Fill(Q2); }

                if (Ne == 1 && Nf_Prime == 3) {
//                if (Ne == 1 && Nf == 3) {
//                    hQ2_1e2X_FD->Fill(Q2);

                    if (Np == 2) { hQ2_1e2p_FD->Fill(Q2); }
                }
            }
        } // end of loop over electrons vector
        //</editor-fold>

        //<editor-fold desc="Proton plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Np; i++) {
            if (protons[i]->getRegion() == CD) {
                hChi2_Proton_CD->Fill(protons[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_Protons_Only_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
            } else if (protons[i]->getRegion() == FD) {
                hChi2_Proton_FD->Fill(protons[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_Protons_Only_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
            }
        } // end of loop over protons vector
        //</editor-fold>

        //<editor-fold desc="Other particles plots">

//        //<editor-fold desc="Neutrons Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                Beta_vs_P_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                Beta_vs_P_Neutrons_Only_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                Beta_vs_P_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                Beta_vs_P_Neutrons_Only_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());

//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

        //<editor-fold desc="Kplus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
//                hChi2_Kplus_CD->Fill(Kplus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
            } else if (Kplus[i]->getRegion() == FD) {
//                hChi2_Kplus_FD->Fill(Kplus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
//                hChi2_Kminus_CD->Fill(Kminus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            } else if (Kminus[i]->getRegion() == FD) {
//                hChi2_Kminus_FD->Fill(Kminus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
//                hChi2_piplus_CD->Fill(piplus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
            } else if (piplus[i]->getRegion() == FD) {
//                hChi2_piplus_FD->Fill(piplus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
//                hChi2_piminus_CD->Fill(piminus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
            } else if (piminus[i]->getRegion() == FD) {
//                hChi2_piminus_FD->Fill(piminus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
            }
        } // end of loop over piminus vector
        //</editor-fold>

//        //<editor-fold desc="pizero Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < pizero.size(); i++) {
//            if (pizero[i]->getRegion() == CD) {
//                Beta_vs_P_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            } else if (pizero[i]->getRegion() == FD) {
//                Beta_vs_P_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over pizero vector
//        //</editor-fold>

        //<editor-fold desc="neutrals Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < neutrals.size(); i++) {
            if (neutrals[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());

                if (neutrals[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                }
            } else if (neutrals[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());

                if (neutrals[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                }
            }
        } // end of loop over neutrals vector
        //</editor-fold>

        //<editor-fold desc="deuterons Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < deuterons.size(); i++) {
            if (deuterons[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());

                if (deuterons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                }
            } else if (deuterons[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());

                if (deuterons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                }
            }
        } // end of loop over deuterons vector
        //</editor-fold>

        //<editor-fold desc="otherpart Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < otherpart.size(); i++) {
            if (otherpart[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());

                if (otherpart[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                }
            } else if (otherpart[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());

                if (otherpart[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                }
            }
        } // end of loop over otherpart vector
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

//  At least 1e cut (log only) ------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="At least 1e cut (log only)">
        if (Ne >= 1) {
            ++num_of_events_with_at_least_1e; // logging #(events) w/ at least 1e

            if (Ne > 1) { ++num_of_events_more_then_1e; /* logging #(events) w/ more then 1e */ }
        }
        //</editor-fold>

//  1e cut --------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e cut">
        /* Here we apply electron cut on everything that follows (1e2X, 1e2p, etc) */

        /* Applying rough 1e cut */
        if (Ne != 1) { continue; } // the rough 1e cut
        ++num_of_events_with_exactly_1e; // logging #(events) w/ exactly 1e

        //<editor-fold desc="1e cut event counts">
        if (Nf - Ne == Np) {
            ++num_of_events_with_1enP; // logging #(events) w/ 1e & any #p 1enP

            if (Np == 1) { ++num_of_events_with_1e1p; /* // logging #(events) w/ 1e1p */ }
        }

        if (Np == 1) { ++num_of_events_1e1p_all; }
        if (Np == 2) { ++num_of_events_1e2p_all; }
        if (Np == 2 && Nph_FD == 0) { ++num_of_events_1e2p_all_wo_FDph; }
        //</editor-fold>


        //TODO: recheck if beta cut should be applied to ALL particles and add it to clas12ana
        if (electrons[0]->par()->getBeta() > 1.2) { continue; }


        /* Electron 1e cut variables' definitions */
        TVector3 P_e_1e;
        P_e_1e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
        double P_e = P_e_1e.Mag(), E_e = sqrt(m_e * m_e + P_e * P_e);
        double Theta_e = P_e_1e.Theta() * 180.0 / pi; // Theta_e in deg
        double Phi_e = P_e_1e.Phi() * 180.0 / pi; // Phi_e in deg

        //<editor-fold desc="Testing electron cuts">
        /* Here we plot cut histograms:
         * If cuts are turned off (apply_cuts == false) - we plot each cut parameter before and after the cut.
         * If cuts are applied (apply_cuts == true), we plot the parameters for the 1e cut only */

        //TODO: all these tests are NOT 2p or 1e2p (they were moved here from below and here we have no constraint on protons.size) - redefine propely to 1e or add constrant on protons.size

        //<editor-fold desc="Testing cuts">
        /* Testing SF cuts */
        double EoP_e = (electrons[0]->cal(PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / P_e;

        if (apply_cuts == false) {
            /* SF plots before cuts */
            hSF_1e_cut_BC_FD->Fill(EoP_e), hSF_VS_P_e_1e_cut_BC_FD->Fill(P_e, EoP_e);

            /* SF plots after cuts */
            if ((EoP_e >= clasAna.getEcalSFLowerCut()) && (EoP_e <= clasAna.getEcalSFUpperCut())) {
                hSF_1e_cut_AC_FD->Fill(EoP_e), hSF_VS_P_e_1e_cut_AC_FD->Fill(P_e, EoP_e);
            }
        } else { hSF_1e_cut_BC_FD->Fill(EoP_e), hSF_VS_P_e_1e_cut_BC_FD->Fill(P_e, EoP_e); }

        /* Testing fiducial cuts */
        if (apply_cuts == false) {
            /* Fiducial plots before cuts */
            Vcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);

            /* Fiducial plots after cuts */
            if (electrons[0]->cal(PCAL)->getLv() >= clasAna.getEcalEdgeCuts()) { Vcal_VS_EoP_1e_AC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e); }
            if (electrons[0]->cal(PCAL)->getLw() >= clasAna.getEcalEdgeCuts()) { Wcal_VS_EoP_1e_AC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e); }
        } else {
            Vcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);
        }

        /* Testing Nphe cuts */
        int Nphe = electrons[0]->che(HTCC)->getNphe();

        if (apply_cuts == false) {
            /* Nphe plots before cuts */
            hNphe_1e_cut_BC_FD.hFill(Nphe);
//            hNphe_1e_cut_BC_FD.Histogram1d->Fill(Nphe);
//            hNphe_1e_cut_BC_FD->Fill(Nphe);

            /* Nphe plots after cuts */
            if (Nphe >= clasAna.getNpheCuts()) {
//                hNphe_1e_cut_AC_FD.Histogram1d->Fill(Nphe);
////                hNphe_1e_cut_AC_FD->Fill(Nphe);
            }
        } else {
            hNphe_1e_cut_BC_FD.hFill(Nphe);
//            hNphe_1e_cut_BC_FD.Histogram1d->Fill(Nphe);
//            hNphe_1e_cut_BC_FD->Fill(Nphe);
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Filling 1e only plots">

        //<editor-fold desc="Fill Electron plots (1e only, CD & FD)">
        for (auto &e: electrons) {
            if (e->getRegion() == FD) {
                hChi2_Electron_1e_cut_FD->Fill(e->par()->getChi2Pid());

                hVx_Electron_1e_cut_FD->Fill(e->par()->getVx());
                hVy_Electron_1e_cut_FD->Fill(e->par()->getVy());
                hVz_Electron_1e_cut_FD->Fill(e->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(P_e, e->par()->getBeta());
                Beta_vs_P_1e_Electrons_Only_FD->Fill(P_e, e->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_FD->Fill(P_e, e->par()->getBeta());

                hP_e_1e_cut_FD->Fill(P_e);

                hTheta_e_1e_cut_FD->Fill(Theta_e);
                hPhi_e_1e_cut_FD->Fill(Phi_e);
                hTheta_e_VS_Phi_e_1e_cut_FD->Fill(Phi_e, Theta_e);

                if (Nf == 3) {
//                    hTheta_e_All_Int_1e2X_FD->Fill(Theta_e);
//                    hPhi_e_1e2X_FD->Fill(Phi_e);

/*
                    if (qel) {
                        ++num_of_QEL_1e2X_FD_events;
//                        hTheta_e_QEL_1e2X_FD->Fill(Theta_e);
//                        hPhi_e_1e2X_QEL_FD->Fill(Phi_e);
                    } else if (mec) {
                        ++num_of_MEC_1e2X_FD_events;
//                        hTheta_e_MEC_1e2X_FD->Fill(Theta_e);
//                        hPhi_e_1e2X_MEC_FD->Fill(Phi_e);
                    } else if (res) {
                        ++num_of_RES_1e2X_FD_events;
//                        hTheta_e_RES_1e2X_FD->Fill(Theta_e);
//                        hPhi_e_1e2X_RES_FD->Fill(Phi_e);
                    } else if (dis) {
                        ++num_of_DIS_1e2X_FD_events;
//                        hTheta_e_DIS_1e2X_FD->Fill(Theta_e);
//                        hPhi_e_1e2X_DIS_FD->Fill(Phi_e);
                    }
*/

                    if (Np == 2) {
                        hTheta_e_All_Int_1e2p_FD->Fill(Theta_e);
                        hPhi_e_All_Int_1e2p_FD->Fill(Phi_e);

                        if (qel) {
                            ++num_of_QEL_1e2p_FD_events;
                            hTheta_e_QEL_1e2p_FD->Fill(Theta_e);
                            hPhi_e_QEL_1e2p_FD->Fill(Phi_e);
                        } else if (mec) {
                            ++num_of_MEC_1e2p_FD_events;
                            hTheta_e_MEC_1e2p_FD->Fill(Theta_e);
                            hPhi_e_MEC_1e2p_FD->Fill(Phi_e);
                        } else if (res) {
                            ++num_of_RES_1e2p_FD_events;
                            hTheta_e_RES_1e2p_FD->Fill(Theta_e);
                            hPhi_e_RES_1e2p_FD->Fill(Phi_e);
                        } else if (dis) {
                            ++num_of_DIS_1e2p_FD_events;
                            hTheta_e_DIS_1e2p_FD->Fill(Theta_e);
                            hPhi_e_DIS_1e2p_FD->Fill(Phi_e);
                        }
                    }
                }

                hE_e_VS_Theta_e_All_Int_1e_cut_FD->Fill(Theta_e, E_e);
                hE_e_1e_cut_FD->Fill(E_e);
            }
        } // end of loop over electrons vector
        //</editor-fold>

        //<editor-fold desc="Fill Proton plots (1e only, CD & FD)">
        for (auto &p: protons) {
            if (p->getRegion() == CD) {
                hChi2_Proton_1e_cut_CD->Fill(p->par()->getChi2Pid());

                hVx_Proton_1e_cut_CD->Fill(p->par()->getVx());
                hVy_Proton_1e_cut_CD->Fill(p->par()->getVy());
                hVz_Proton_1e_cut_CD->Fill(p->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_1e_Protons_Only_CD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_CD->Fill(p->getP(), p->par()->getBeta());

                hP_p_1e_cut_CD->Fill(p->getP());

            } else if (p->getRegion() == FD) {
                hChi2_Proton_1e_cut_FD->Fill(p->par()->getChi2Pid());

                hVx_Proton_1e_cut_FD->Fill(p->par()->getVx());
                hVy_Proton_1e_cut_FD->Fill(p->par()->getVy());
                hVz_Proton_1e_cut_FD->Fill(p->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_1e_Protons_Only_FD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_FD->Fill(p->getP(), p->par()->getBeta());

                hP_p_1e_cut_FD->Fill(p->getP());

            }
        } // end of loop over protons vector
        //</editor-fold>

        //<editor-fold desc="Fill Beta vs. P for other particles (1e cut, CD & FD)">

//        //<editor-fold desc="neutrons Beta vs. P plots (1e cut, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                Beta_vs_P_1e_cut_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                Beta_vs_P_1e_Neutrons_Only_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_cut_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_cut_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_cut_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                Beta_vs_P_1e_cut_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                Beta_vs_P_1e_Neutrons_Only_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_cut_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_cut_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_cut_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

        //<editor-fold desc="Kplus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
                hChi2_Kplus_1e_cut_CD->Fill(Kplus[i]->par()->getChi2Pid());

                hVx_Kplus_1e_cut_CD->Fill(Kplus[i]->par()->getVx());
                hVy_Kplus_1e_cut_CD->Fill(Kplus[i]->par()->getVy());
                hVz_Kplus_1e_cut_CD->Fill(Kplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_1e_Kplus_Only_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

            } else if (Kplus[i]->getRegion() == FD) {
                hChi2_Kplus_1e_cut_FD->Fill(Kplus[i]->par()->getChi2Pid());

                hVx_Kplus_1e_cut_FD->Fill(Kplus[i]->par()->getVx());
                hVy_Kplus_1e_cut_FD->Fill(Kplus[i]->par()->getVy());
                hVz_Kplus_1e_cut_FD->Fill(Kplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_1e_Kplus_Only_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                hChi2_Kminus_1e_cut_CD->Fill(Kminus[i]->par()->getChi2Pid());

                hVx_Kminus_1e_cut_CD->Fill(Kminus[i]->par()->getVx());
                hVy_Kminus_1e_cut_CD->Fill(Kminus[i]->par()->getVy());
                hVz_Kminus_1e_cut_CD->Fill(Kminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_1e_Kminus_Only_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            } else if (Kminus[i]->getRegion() == FD) {
                hChi2_Kminus_1e_cut_FD->Fill(Kminus[i]->par()->getChi2Pid());

                hVx_Kminus_1e_cut_FD->Fill(Kminus[i]->par()->getVx());
                hVy_Kminus_1e_cut_FD->Fill(Kminus[i]->par()->getVy());
                hVz_Kminus_1e_cut_FD->Fill(Kminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_1e_Kminus_Only_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                hChi2_piplus_1e_cut_CD->Fill(piplus[i]->par()->getChi2Pid());

                hVx_piplus_1e_cut_CD->Fill(piplus[i]->par()->getVx());
                hVy_piplus_1e_cut_CD->Fill(piplus[i]->par()->getVy());
                hVz_piplus_1e_cut_CD->Fill(piplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_1e_piplus_Only_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                hP_piplus_1e_cut_CD->Fill(piplus[i]->getP());

            } else if (piplus[i]->getRegion() == FD) {
                hChi2_piplus_1e_cut_FD->Fill(piplus[i]->par()->getChi2Pid());

                hVx_piplus_1e_cut_FD->Fill(piplus[i]->par()->getVx());
                hVy_piplus_1e_cut_FD->Fill(piplus[i]->par()->getVy());
                hVz_piplus_1e_cut_FD->Fill(piplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_1e_piplus_Only_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                hP_piplus_1e_cut_FD->Fill(piplus[i]->getP());

            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                hChi2_piminus_1e_cut_CD->Fill(piminus[i]->par()->getChi2Pid());

                hVx_piminus_1e_cut_CD->Fill(piminus[i]->par()->getVx());
                hVy_piminus_1e_cut_CD->Fill(piminus[i]->par()->getVy());
                hVz_piminus_1e_cut_CD->Fill(piminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_1e_piminus_Only_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                hP_piminus_1e_cut_CD->Fill(piminus[i]->getP());

            } else if (piminus[i]->getRegion() == FD) {
                hChi2_piminus_1e_cut_FD->Fill(piminus[i]->par()->getChi2Pid());

                hVx_piminus_1e_cut_FD->Fill(piminus[i]->par()->getVx());
                hVy_piminus_1e_cut_FD->Fill(piminus[i]->par()->getVy());
                hVz_piminus_1e_cut_FD->Fill(piminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_1e_piminus_Only_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                hP_piminus_1e_cut_FD->Fill(piminus[i]->getP());

            }
        } // end of loop over piminus vector
        //</editor-fold>

//        //<editor-fold desc="pizero Beta vs. P plots (1e cut, CD & FD)">
//        for (int i = 0; i < pizero.size(); i++) {
//            if (pizero[i]->getRegion() == CD) {
//                Beta_vs_P_1e_cut_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                Beta_vs_P_1e_pizero_Only_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_cut_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_cut_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_cut_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            } else if (pizero[i]->getRegion() == FD) {
//                Beta_vs_P_1e_cut_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                Beta_vs_P_1e_pizero_Only_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_cut_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_cut_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_cut_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over pizero vector
//        //</editor-fold>

        //<editor-fold desc="neutrals Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < neutrals.size(); i++) {
            if (neutrals[i]->getRegion() == CD) {
                Beta_vs_P_1e_cut_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());

                if (neutrals[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_cut_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_cut_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_cut_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                }
            } else if (neutrals[i]->getRegion() == FD) {
                Beta_vs_P_1e_cut_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());

                if (neutrals[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_cut_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_cut_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_cut_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="deuterons Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < deuterons.size(); i++) {
            if (deuterons[i]->getRegion() == CD) {
                Beta_vs_P_1e_cut_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());

                if (deuterons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_cut_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_cut_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_cut_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                }
            } else if (deuterons[i]->getRegion() == FD) {
                Beta_vs_P_1e_cut_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());

                if (deuterons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_cut_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_cut_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_cut_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="otherpart Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < otherpart.size(); i++) {
            if (otherpart[i]->getRegion() == CD) {
                Beta_vs_P_1e_cut_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());

                if (otherpart[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_cut_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_cut_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_cut_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                }
            } else if (otherpart[i]->getRegion() == FD) {
                Beta_vs_P_1e_cut_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());

                if (otherpart[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_cut_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_cut_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_cut_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

//  1p ------------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e1p">
        if ((calculate_1p == true) && ((Nf_Prime == 2) && (Np == 1))) { // for 2p calculations (with any number of neutrals)
//        if ((calculate_2p == true) && ((Nf == 2) && (Np == 1))) { // for 2p calculations
            for (auto &e: electrons) {
                if (e->getRegion() == FD) {
                    hET_All_Ang_All_Int_1p_FD->Fill(beamE - E_e);
                    if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_All_Int_1p_FD->Fill(beamE - E_e); }

                    if (qel) {
                        hET_All_Ang_QEL_1p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_QEL_1p_FD->Fill(beamE - E_e); }
                    } else if (mec) {
                        hET_All_Ang_MEC_1p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_MEC_1p_FD->Fill(beamE - E_e); }
                    } else if (res) {
                        hET_All_Ang_RES_1p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_RES_1p_FD->Fill(beamE - E_e); }
                    } else if (dis) {
                        hET_All_Ang_DIS_1p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_DIS_1p_FD->Fill(beamE - E_e); }
                    }
                }
            } // end of loop over electrons vector
        } // end of 1p cuts if
        //</editor-fold>

//  1e2pXy --------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e2pXy">
        if (Np == 2) { // 2p and everything else is allowed
            //TODO: reorganize properly
            TVector3 P_p_first_1e2pXy, P_p_second_1e2pXy; // P_L = leading proton (p1); P_R = recoil proton (p2)
            P_p_first_1e2pXy.SetMagThetaPhi(protons[0]->getP(), protons[0]->getTheta(), protons[0]->getPhi());
            P_p_second_1e2pXy.SetMagThetaPhi(protons[1]->getP(), protons[1]->getTheta(), protons[1]->getPhi());

            if (P_p_first_1e2pXy.Mag() >= P_p_second_1e2pXy.Mag()) { //if p_first is leading
                if (protons[0]->getRegion() == CD) { hPhi_p1_1e2pXy_CD->Fill(protons[0]->getPhi() * 180.0 / pi); }
                if (protons[1]->getRegion() == CD) { hPhi_p2_1e2pXy_CD->Fill(protons[1]->getPhi() * 180.0 / pi); }
            } else { //else if p_second is leading
                if (protons[1]->getRegion() == CD) { hPhi_p1_1e2pXy_CD->Fill(protons[1]->getPhi() * 180.0 / pi); }
                if (protons[0]->getRegion() == CD) { hPhi_p2_1e2pXy_CD->Fill(protons[0]->getPhi() * 180.0 / pi); }
            }
        }
        //</editor-fold>

//  1e2X cut ------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e2X cut">
        if (Nf == 3) { ++num_of_events_with_1e2X; } // logging #(events) w/ 1e2X (only 3 scattered/detected particles)
        //</editor-fold>

//  1e2p & 2p cuts ------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e2p & 2p cuts">
        //TODO: make sure that pdg=0 particles are included/excluded.
        if ((calculate_2p == true) && ((Nf_Prime == 3) && (Np == 2))) { // for 2p calculations (with any number of neutrals)
//        if ((calculate_2p == true) && ((Nf == 3) && (Np == 2))) { // for 2p calculations
            ++num_of_events_with_1e2p; // logging #(events) w/ 1e2p

            /* NOTE: p_first corresponds to protons[0] & p_second corresponds to protons[1] */
            TVector3 P_e_2p_3v, q_3v, P_p_first_2p_3v, P_p_second_2p_3v, P_tot_2p_3v, P_1_2p_3v, P_2_2p_3v, P_T_e_2p_3v, P_T_L_2p_3v, P_T_tot_2p_3v, dP_T_L_2p_3v, dP_T_tot_2p_3v;

            P_e_2p_3v.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi()); // electron 3 momentum
            q_3v = TVector3(Pvx - P_e_2p_3v.Px(), Pvy - P_e_2p_3v.Py(), Pvz - P_e_2p_3v.Pz()); // 3 momentum transfer
            P_T_e_2p_3v = TVector3(P_e_2p_3v.Px(), P_e_2p_3v.Py(), 0); // electron transverse momentum
            P_p_first_2p_3v.SetMagThetaPhi(protons[0]->getP(), protons[0]->getTheta(), protons[0]->getPhi()); // first proton in protons vector
            P_p_second_2p_3v.SetMagThetaPhi(protons[1]->getP(), protons[1]->getTheta(), protons[1]->getPhi()); // second proton in protons vector

            double E_e_2p = sqrt(m_e * m_e + P_e_2p_3v.Mag2()), omega_2p = beamE - E_e_2p, W_2p = sqrt((omega_2p + m_p) * (omega_2p + m_p) - q_3v.Mag2());
            double E_1_2p, E_2_2p, Theta_p1_p2_2p, Theta_p_e_p_tot_2p, Theta_q_p_tot_2p, Theta_q_p_L_2p, Theta_q_p_R_2p;
            double dAlpha_T_L, dAlpha_T_tot, dPhi_T_L, dPhi_T_tot, Ecal_2p;

            //  Testing cuts ----------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing cuts">

            //<editor-fold desc="Testing momentum cuts (protons only)">
            /* momentum before and after cuts */
            if (electrons[0]->getRegion() == FD) {
                hP_e_1e2p_BC_FD->Fill(P_e); // momentum before cuts
                LogEventCuts(hP_e_1e2p_AC_FD, electrons[0], e_momentum_cuts_2p.GetLowerCut(), e_momentum_cuts_2p.GetUpperCut()); // momentum after cuts
            }

            if (protons[0]->getRegion() == CD) {
                hP_p_1e2p_BC_CD->Fill(P_p_first_2p_3v.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_CD, protons[0], p_momentum_cuts_2p.GetLowerCut(), p_momentum_cuts_2p.GetUpperCut()); // momentum after cuts
            } else if (protons[0]->getRegion() == FD) {
                hP_p_1e2p_BC_FD->Fill(P_p_first_2p_3v.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_FD, protons[0], p_momentum_cuts_2p.GetLowerCut(), p_momentum_cuts_2p.GetUpperCut()); // momentum after cuts
            }

            if (protons[1]->getRegion() == CD) {
                hP_p_1e2p_BC_CD->Fill(P_p_second_2p_3v.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_CD, protons[1], p_momentum_cuts_2p.GetLowerCut(), p_momentum_cuts_2p.GetUpperCut()); // momentum after cuts
            } else if (protons[1]->getRegion() == FD) {
                hP_p_1e2p_BC_FD->Fill(P_p_second_2p_3v.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_FD, protons[1], p_momentum_cuts_2p.GetLowerCut(), p_momentum_cuts_2p.GetUpperCut()); // momentum after cuts
            }
            //</editor-fold>

            //<editor-fold desc="Testing chi2 cuts (protons only)">
            if (apply_cuts == false) {
                /* Chi2 before cut */
                if (electrons[0]->getRegion() == FD) { hChi2_Electron_1e2p_BC_FD->Fill(electrons[0]->par()->getChi2Pid()); }

                if (protons[0]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD->Fill(protons[0]->par()->getChi2Pid());
                } else if (protons[0]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD->Fill(protons[0]->par()->getChi2Pid());
                }

                if (protons[1]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD->Fill(protons[1]->par()->getChi2Pid());
                } else if (protons[1]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD->Fill(protons[1]->par()->getChi2Pid());
                }

                /* Chi2 after cut */
                if ((electrons[0]->getRegion() == FD) &&
                    (fabs(Chi2_Electron_cuts_FD.Cuts.at(0) - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cuts_FD.Cuts.at(2))) {
                    ++num_of_events_1e2p_w_eChi2_cut_only_FD;
                    hChi2_Electron_1e2p_AC_FD->Fill(electrons[0]->par()->getChi2Pid());
                }

                if ((protons[0]->getRegion() == CD) && (fabs(Chi2_Proton_cuts_CD.Cuts.at(0) - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cuts_CD.Cuts.at(2))) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                    hChi2_Proton_1e2p_AC_CD->Fill(protons[0]->par()->getChi2Pid());
                } else if ((protons[0]->getRegion() == FD) &&
                           (fabs(Chi2_Proton_cuts_FD.Cuts.at(0) - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cuts_FD.Cuts.at(2))) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                    hChi2_Proton_1e2p_AC_FD->Fill(protons[0]->par()->getChi2Pid());
                }

                if ((protons[1]->getRegion() == CD) && (fabs(Chi2_Proton_cuts_CD.Cuts.at(0) - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cuts_CD.Cuts.at(2))) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                    hChi2_Proton_1e2p_AC_CD->Fill(protons[1]->par()->getChi2Pid());
                } else if ((protons[1]->getRegion() == FD) &&
                           (fabs(Chi2_Proton_cuts_FD.Cuts.at(0) - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cuts_FD.Cuts.at(2))) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                    hChi2_Proton_1e2p_AC_FD->Fill(protons[1]->par()->getChi2Pid());
                }
            } else {
                if (electrons[0]->getRegion() == FD) { hChi2_Electron_1e2p_BC_FD->Fill(electrons[0]->par()->getChi2Pid()); }

                if (protons[0]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD->Fill(protons[0]->par()->getChi2Pid());
                } else if (protons[0]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD->Fill(protons[0]->par()->getChi2Pid());
                }

                if (protons[1]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD->Fill(protons[1]->par()->getChi2Pid());
                } else if (protons[1]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD->Fill(protons[1]->par()->getChi2Pid());
                }
            }
            //</editor-fold>

            //<editor-fold desc="Testing dV cuts">
            double Vx_e = electrons[0]->par()->getVx(), Vy_e = electrons[0]->par()->getVy(), Vz_e = electrons[0]->par()->getVz();
            double Vz_p0 = protons[0]->par()->getVz(), Vz_p1 = protons[1]->par()->getVz(), dVz0 = Vz_p0 - Vz_e, dVz1 = Vz_p1 - Vz_e;
            double Vx_p, Vy_p, Vz_p, dVx, dVy, dVz;

            for (auto &p: protons) {
                Vx_p = p->par()->getVx(), Vy_p = p->par()->getVy(), Vz_p = p->par()->getVz();
                dVx = Vx_p - Vx_e, dVy = Vy_p - Vy_e, dVz = Vz_p - Vz_e;

                if (apply_cuts == false) {
                    /* Testing dVx,dVy,dVz before cuts */
                    hdVx_1e2p_BC->Fill(dVx), hdVy_1e2p_BC->Fill(dVy), hdVz_1e2p_BC->Fill(dVz);

                    /* Testing dVx,dVy,dVz after cuts */
                    if (fabs(dVx) <= -1) { hdVx_1e2p_AC->Fill(dVx); } // no dVx cuts
                    if (fabs(dVy) <= -1) { hdVy_1e2p_AC->Fill(dVy); } // no dVx cuts
                    if (fabs(dVz) <= dVz_cuts.GetUpperCut()) { hdVz_1e2p_AC->Fill(dVz); }
                } else {
                    hdVx_1e2p_BC->Fill(dVx), hdVy_1e2p_BC->Fill(dVy), hdVz_1e2p_BC->Fill(dVz);
                }
            } // end of loop over protons vector
            //</editor-fold>

            //</editor-fold>

            //  Applying cuts ---------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying cuts">

            //<editor-fold desc="Applying momentum cuts">
            /* Applying momentum cuts. Other cuts are applied via clas12ana. */

            // Electrons:
            if ((apply_momentum_cuts_2p == true) && ((e_momentum_cuts_2p.GetUpperCut() != -1) && (P_e > e_momentum_cuts_2p.GetUpperCut()))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((e_momentum_cuts_2p.GetLowerCut() != -1) && (P_e < e_momentum_cuts_2p.GetLowerCut()))) { continue; }

            // Proton 0:
            if ((apply_momentum_cuts_2p == true) &&
                ((p_momentum_cuts_2p.GetUpperCut() != -1) && (P_p_first_2p_3v.Mag() > p_momentum_cuts_2p.GetUpperCut()))) { continue; }
            if ((apply_momentum_cuts_2p == true) &&
                ((p_momentum_cuts_2p.GetLowerCut() != -1) && (P_p_first_2p_3v.Mag() < p_momentum_cuts_2p.GetLowerCut()))) { continue; }

            // Proton 1:
            if ((apply_momentum_cuts_2p == true) &&
                ((p_momentum_cuts_2p.GetUpperCut() != -1) && (P_p_second_2p_3v.Mag() > p_momentum_cuts_2p.GetUpperCut()))) { continue; }
            if ((apply_momentum_cuts_2p == true) &&
                ((p_momentum_cuts_2p.GetLowerCut() != -1) && (P_p_second_2p_3v.Mag() < p_momentum_cuts_2p.GetLowerCut()))) { continue; }
            //</editor-fold>

            //</editor-fold>

            //  Fillings 2p histograms ---------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Counting 2p events by reaction">
            ++num_of_events_2p;

            if (qel) {
                ++num_of_QEL_2p_events;
            } else if (mec) {
                ++num_of_MEC_2p_events;
            } else if (res) {
                ++num_of_RES_2p_events;
            } else if (dis) {
                ++num_of_DIS_2p_events;
            }
            //</editor-fold>

            //<editor-fold desc="Filling cut parameters histograms (2p)">
            /* Filling Chi2 histograms (2p) */

            // Electrton Chi2 (2p):
            if (electrons[0]->getRegion() == FD) { hChi2_Electron_2p_FD->Fill(electrons[0]->par()->getChi2Pid()); }

            // Proton0 Chi2 (2p):
            if (protons[0]->getRegion() == CD) {
                hChi2_Proton_2p_CD->Fill(protons[0]->par()->getChi2Pid());
            } else if (protons[0]->getRegion() == FD) {
                hChi2_Proton_2p_FD->Fill(protons[0]->par()->getChi2Pid());
            }

            // Proton1 Chi2 (2p):
            if (protons[1]->getRegion() == CD) {
                hChi2_Proton_2p_CD->Fill(protons[1]->par()->getChi2Pid());
            } else if (protons[1]->getRegion() == FD) {
                hChi2_Proton_2p_FD->Fill(protons[1]->par()->getChi2Pid());
            }

            /* Filling dVx, dVy, dVz (2p) */
            for (auto &p: protons) {
                Vx_p = p->par()->getVx(), Vy_p = p->par()->getVy(), Vz_p = p->par()->getVz();
                dVx = Vx_p - Vx_e, dVy = Vy_p - Vy_e, dVz = Vz_p - Vz_e;

                hdVx_2p->Fill(dVx), hdVy_2p->Fill(dVy), hdVz_2p->Fill(dVz);
            } // end of loop over protons vector

            /* Filling fiducial plots (2p) */
            Vcal_VS_EoP_2p_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_2p_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);

            /* Filling SF histograms (2p) */
            hSF_2p_FD->Fill(EoP_e), hSF_VS_P_e_2p_FD->Fill(P_e, EoP_e);

            /* Filling Nphe plots (2p) */
            hNphe_2p_FD->Fill(Nphe);

            /* Filling momentum histograms */
            // Electrton momentum (2p):
            if (electrons[0]->getRegion() == FD) { hP_e_2p_FD->Fill(P_e); }

            // Proton0 momentum (2p):
            if (protons[0]->getRegion() == CD) {
                hP_p_2p_CD->Fill(P_p_first_2p_3v.Mag());
            } else if (protons[0]->getRegion() == FD) {
                hP_p_2p_FD->Fill(P_p_first_2p_3v.Mag());
            }

            // Proton1 momentum (2p):
            if (protons[1]->getRegion() == CD) {
                hP_p_2p_CD->Fill(P_p_second_2p_3v.Mag());
            } else if (protons[1]->getRegion() == FD) {
                hP_p_2p_FD->Fill(P_p_second_2p_3v.Mag());
            }
            //</editor-fold>

            //<editor-fold desc="Filling Beta vs. P plots (2p)">

            //<editor-fold desc="Beta vs. P from electrons (2p, CD & FD)">
            if (electrons[0]->getRegion() == FD) {
                Beta_vs_P_2p_FD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                Beta_vs_P_2p_Electrons_Only_FD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());

                if (electrons[0]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_2p_FD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                } else if (electrons[0]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_2p_FD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                } else if (electrons[0]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_2p_FD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                }
            }
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from protons (2p, CD & FD)">
            for (int i = 0; i < Np; i++) {
                if (protons[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    Beta_vs_P_2p_Protons_Only_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    Beta_vs_P_positive_particles_2p_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                } else if (protons[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    Beta_vs_P_2p_Protons_Only_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    Beta_vs_P_positive_particles_2p_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                }
            } // end of loop over protons vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from other particles (2p, CD & FD)">
            /* This is for self-consistency. Contributions from other particles should be zero */

//        //<editor-fold desc="Beta vs. P from neutrons (2p, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                Beta_vs_P_2p_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_2p_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_2p_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_2p_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                Beta_vs_P_2p_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_2p_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_2p_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_2p_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

            //<editor-fold desc="Beta vs. P from Kplus (2p, CD & FD)">
            for (int i = 0; i < Kplus.size(); i++) {
                if (Kplus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    Beta_vs_P_positive_particles_2p_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    Beta_vs_P_positive_particles_2p_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                }
            } // end of loop over Kplus vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from Kminus (2p, CD & FD)">
            for (int i = 0; i < Kminus.size(); i++) {
                if (Kminus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    Beta_vs_P_negative_particles_2p_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    Beta_vs_P_negative_particles_2p_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                }
            } // end of loop over Kminus vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from piplus (2p, CD & FD)">
            for (int i = 0; i < piplus.size(); i++) {
                if (piplus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    Beta_vs_P_positive_particles_2p_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    Beta_vs_P_positive_particles_2p_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                }
            } // end of loop over piplus vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from piminus (2p, CD & FD)">
            for (int i = 0; i < piminus.size(); i++) {
                if (piminus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    Beta_vs_P_negative_particles_2p_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    Beta_vs_P_negative_particles_2p_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                }
            } // end of loop over piminus vector
            //</editor-fold>

//        //<editor-fold desc="Beta vs. P from pizero (2p, CD & FD)">
//        for (int i = 0; i < pizero.size(); i++) {
//            if (pizero[i]->getRegion() == CD) {
//                Beta_vs_P_2p_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_2p_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_2p_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_2p_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            } else if (pizero[i]->getRegion() == FD) {
//                Beta_vs_P_2p_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_2p_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_2p_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_2p_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over pizero vector
//        //</editor-fold>

//            //<editor-fold desc="Beta vs. P from neutrals (2p, CD & FD)">
//            for (int i = 0; i < neutrals.size(); i++) {
//                if (neutrals[i]->getRegion() == CD) {
//                    Beta_vs_P_2p_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//
//                    if (neutrals[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_2p_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                    } else if (neutrals[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_2p_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                    } else if (neutrals[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_2p_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                    }
//                } else if (neutrals[i]->getRegion() == FD) {
//                    Beta_vs_P_2p_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//
//                    if (neutrals[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_2p_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                    } else if (neutrals[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_2p_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                    } else if (neutrals[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_2p_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                    }
//                }
//            } // end of loop over neutrals vector
//            //</editor-fold>
//
//            //<editor-fold desc="Beta vs. P from deuterons (2p, CD & FD)">
//            for (int i = 0; i < deuterons.size(); i++) {
//                if (deuterons[i]->getRegion() == CD) {
//                    Beta_vs_P_2p_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//
//                    if (deuterons[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_2p_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                    } else if (deuterons[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_2p_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                    } else if (deuterons[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_2p_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                    }
//                } else if (deuterons[i]->getRegion() == FD) {
//                    Beta_vs_P_2p_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//
//                    if (deuterons[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_2p_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                    } else if (deuterons[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_2p_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                    } else if (deuterons[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_2p_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                    }
//                }
//            } // end of loop over deuterons vector
//            //</editor-fold>
//
//            //<editor-fold desc="Beta vs. P from otherpart (2p, CD & FD)">
//            for (int i = 0; i < otherpart.size(); i++) {
//                if (otherpart[i]->getRegion() == CD) {
//                    Beta_vs_P_2p_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//
//                    if (otherpart[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_2p_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                    } else if (otherpart[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_2p_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                    } else if (otherpart[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_2p_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                    }
//                } else if (otherpart[i]->getRegion() == FD) {
//                    Beta_vs_P_2p_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//
//                    if (otherpart[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_2p_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                    } else if (otherpart[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_2p_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                    } else if (otherpart[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_2p_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                    }
//                }
//            } // end of loop over otherpart vector
//            //</editor-fold>

            //</editor-fold>

            //</editor-fold>

            for (auto &e: electrons) {
                if (e->getRegion() == CD) {
//                    hTheta_e_All_Int_2p_CD->Fill(Theta_e);
//                    hPhi_e_All_Int_2p_CD->Fill(Phi_e);
//                    hTheta_e_VS_Phi_e_2p_CD->Fill(Phi_e, Theta_e);
//
//                    hE_e_All_Int_2p_CD->Fill(E_e);
//                    hE_e_VS_Theta_e_All_Int_2p_CD->Fill(Theta_e, E_e);
//
//                    hET_All_Ang_All_Int_2p_FD->Fill(beamE - E_e);
//                    if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_All_Int_2p_CD->Fill(beamE - E_e); }
//
//                    if (qel) {
//                        hTheta_e_QEL_2p_CD->Fill(Theta_e);
//                        hPhi_e_QEL_2p_CD->Fill(Phi_e);
//                        hE_e_QEL_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_QEL_2p_CD->Fill(Theta_e, E_e);
//
//                        hET_All_Ang_QEL_2p_FD->Fill(beamE - E_e);
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_QEL_2p_CD->Fill(beamE - E_e); }
//                    } else if (mec) {
//                        hTheta_e_MEC_2p_CD->Fill(Theta_e);
//                        hPhi_e_MEC_2p_CD->Fill(Phi_e);
//                        hE_e_MEC_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_MEC_2p_CD->Fill(Theta_e, E_e);
//
//                        hET_All_Ang_MEC_2p_FD->Fill(beamE - E_e);
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_MEC_2p_CD->Fill(beamE - E_e); }
//                    } else if (res) {
//                        hTheta_e_RES_2p_CD->Fill(Theta_e);
//                        hPhi_e_RES_2p_CD->Fill(Phi_e);
//                        hE_e_RES_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_RES_2p_CD->Fill(Theta_e, E_e);
//
//                        hET_All_Ang_RES_2p_FD->Fill(beamE - E_e);
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_RES_2p_CD->Fill(beamE - E_e); }
//                    } else if (dis) {
//                        hTheta_e_DIS_2p_CD->Fill(Theta_e);
//                        hPhi_e_DIS_2p_CD->Fill(Phi_e);
//                        hE_e_DIS_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_DIS_2p_CD->Fill(Theta_e, E_e);
//
//                        hET_All_Ang_DIS_2p_FD->Fill(beamE - E_e);
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_DIS_2p_CD->Fill(beamE - E_e); }
//                    }
                } else if (e->getRegion() == FD) {
                    hTheta_e_All_Int_2p_FD->Fill(Theta_e);
                    hPhi_e_All_Int_2p_FD->Fill(Phi_e);
                    hTheta_e_VS_Phi_e_2p_FD->Fill(Phi_e, Theta_e);

                    hE_e_All_Int_2p_FD->Fill(E_e);
                    hE_e_VS_Theta_e_All_Int_2p_FD->Fill(Theta_e, E_e);

                    hET_All_Ang_All_Int_2p_FD->Fill(beamE - E_e);
                    if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                        hET15_All_Int_2p_FD->Fill(beamE - E_e);
                        hE_e_15_All_Int_2p_FD->Fill(E_e);
                    }

                    if (qel) {
                        hTheta_e_QEL_2p_FD->Fill(Theta_e);
                        hPhi_e_QEL_2p_FD->Fill(Phi_e);
                        hE_e_QEL_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_QEL_2p_FD->Fill(Theta_e, E_e);

                        hET_All_Ang_QEL_2p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_QEL_2p_FD->Fill(beamE - E_e);
                            hE_e_15_QEL_2p_FD->Fill(E_e);
                        }
                    } else if (mec) {
                        hTheta_e_MEC_2p_FD->Fill(Theta_e);
                        hPhi_e_MEC_2p_FD->Fill(Phi_e);
                        hE_e_MEC_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_MEC_2p_FD->Fill(Theta_e, E_e);

                        hET_All_Ang_MEC_2p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_MEC_2p_FD->Fill(beamE - E_e);
                            hE_e_15_MEC_2p_FD->Fill(E_e);
                        }
                    } else if (res) {
                        hTheta_e_RES_2p_FD->Fill(Theta_e);
                        hPhi_e_RES_2p_FD->Fill(Phi_e);
                        hE_e_RES_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_RES_2p_FD->Fill(Theta_e, E_e);

                        hET_All_Ang_RES_2p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_RES_2p_FD->Fill(beamE - E_e);
                            hE_e_15_RES_2p_FD->Fill(E_e);
                        }
                    } else if (dis) {
                        hTheta_e_DIS_2p_FD->Fill(Theta_e);
                        hPhi_e_DIS_2p_FD->Fill(Phi_e);
                        hE_e_DIS_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_DIS_2p_FD->Fill(Theta_e, E_e);

                        hET_All_Ang_DIS_2p_FD->Fill(beamE - E_e);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_DIS_2p_FD->Fill(beamE - E_e);
                            hE_e_15_DIS_2p_FD->Fill(E_e);
                        }
                    }
                }
            } // end of loop over electrons vector

            /* Filling Q2 histograms (2p) */
            if (electrons[0]->getRegion() == FD) { hQ2_2p_FD->Fill(Q2); }

            /* Filling Ecal histograms (2p) */
            // Determining leading and recoil protons:
            if (P_p_first_2p_3v.Mag() >= P_p_second_2p_3v.Mag()) { // If p_first is leading proton
                P_1_2p_3v = TVector3(P_p_first_2p_3v.Px(), P_p_first_2p_3v.Py(), P_p_first_2p_3v.Pz());
                P_2_2p_3v = TVector3(P_p_second_2p_3v.Px(), P_p_second_2p_3v.Py(), P_p_second_2p_3v.Pz());
            } else { // else if p_second is leading proton
                P_2_2p_3v = TVector3(P_p_first_2p_3v.Px(), P_p_first_2p_3v.Py(), P_p_first_2p_3v.Pz());
                P_1_2p_3v = TVector3(P_p_second_2p_3v.Px(), P_p_second_2p_3v.Py(), P_p_second_2p_3v.Pz());
            }

            hP_p_1_2p->Fill(P_1_2p_3v.Mag()); // Leading proton (2p)
            hP_p_2_2p->Fill(P_2_2p_3v.Mag()); // Recoil proton (2p)
            hP_p_1_vs_P_p_2_2p->Fill(P_1_2p_3v.Mag(), P_2_2p_3v.Mag());

            P_tot_2p_3v = TVector3(P_p_first_2p_3v.Px() + P_p_second_2p_3v.Px(), P_p_first_2p_3v.Py() + P_p_second_2p_3v.Py(),
                                   P_p_first_2p_3v.Pz() + P_p_second_2p_3v.Pz()); // P_tot = P_1 + P_2
            P_T_L_2p_3v = TVector3(P_1_2p_3v.Px(), P_1_2p_3v.Py(), 0); // transverse part of P_1
            P_T_tot_2p_3v = TVector3(P_p_first_2p_3v.Px() + P_p_second_2p_3v.Px(), P_p_first_2p_3v.Py() + P_p_second_2p_3v.Py(), 0); // transverse part of P_tot
            dP_T_L_2p_3v = TVector3(P_e_2p_3v.Px() + P_T_L_2p_3v.Px(), P_e_2p_3v.Py() + P_T_L_2p_3v.Py(), 0);
            dP_T_tot_2p_3v = TVector3(P_e_2p_3v.Px() + P_1_2p_3v.Px() + P_2_2p_3v.Px(), P_e_2p_3v.Py() + P_1_2p_3v.Py() + P_2_2p_3v.Py(), 0);

            Theta_p_e_p_tot_2p = acos((P_e_2p_3v.Px() * P_tot_2p_3v.Px() + P_e_2p_3v.Py() * P_tot_2p_3v.Py() + P_e_2p_3v.Pz() * P_tot_2p_3v.Pz())
                                      / (P_e_2p_3v.Mag() * P_tot_2p_3v.Mag())) * 180.0 / pi; // Theta_p_e_p_tot_2p in deg
            hTheta_p_e_p_tot_2p->Fill(Theta_p_e_p_tot_2p);

            Theta_p1_p2_2p = acos((P_1_2p_3v.Px() * P_2_2p_3v.Px() + P_1_2p_3v.Py() * P_2_2p_3v.Py() + P_1_2p_3v.Pz() * P_2_2p_3v.Pz())
                                  / (P_1_2p_3v.Mag() * P_2_2p_3v.Mag())) * 180.0 / pi; // Theta_p1_p2_2p in deg
            hTheta_p1_p2_All_Int_2p->Fill(Theta_p1_p2_2p);

            if (qel) {
                hTheta_p1_p2_QEL_2p->Fill(Theta_p1_p2_2p);
            } else if (mec) {
                hTheta_p1_p2_MEC_2p->Fill(Theta_p1_p2_2p);
            } else if (res) {
                hTheta_p1_p2_RES_2p->Fill(Theta_p1_p2_2p);
            } else if (dis) {
                hTheta_p1_p2_DIS_2p->Fill(Theta_p1_p2_2p);
            }

            hTheta_p1_p2_vs_W_2p->Fill(W_2p, Theta_p1_p2_2p);

            if (Theta_p1_p2_2p < 10) {
                double Theta_p1 = P_1_2p_3v.Theta() * 180.0 / pi, Theta_p2 = P_2_2p_3v.Theta() * 180.0 / pi; // Theta_p1, Theta_p2 in deg
                hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p->Fill(Theta_p2, Theta_p1);
            }

            Theta_q_p_tot_2p = acos((q_3v.Px() * P_tot_2p_3v.Px() + q_3v.Py() * P_tot_2p_3v.Py() + q_3v.Pz() * P_tot_2p_3v.Pz())
                                    / (q_3v.Mag() * P_tot_2p_3v.Mag())) * 180.0 / pi; // Theta_q_p_tot_2p in deg
            hTheta_q_p_tot_2p->Fill(Theta_q_p_tot_2p);

            Theta_q_p_L_2p = acos((q_3v.Px() * P_1_2p_3v.Px() + q_3v.Py() * P_1_2p_3v.Py() + q_3v.Pz() * P_1_2p_3v.Pz())
                                  / (q_3v.Mag() * P_1_2p_3v.Mag())) * 180.0 / pi; // Theta_q_p_L_2p = Theta_q_p_1_2p in deg
            Theta_q_p_R_2p = acos((q_3v.Px() * P_2_2p_3v.Px() + q_3v.Py() * P_2_2p_3v.Py() + q_3v.Pz() * P_2_2p_3v.Pz())
                                  / (q_3v.Mag() * P_2_2p_3v.Mag())) * 180.0 / pi; // Theta_q_p_R_2p = Theta_q_p_2_2p in deg
            hTheta_q_p_L_2p->Fill(Theta_q_p_L_2p);
            hTheta_q_p_R_2p->Fill(Theta_q_p_R_2p);
            hTheta_q_p_L_vs_p_L_q_2p->Fill(P_1_2p_3v.Mag() / q_3v.Mag(), Theta_q_p_L_2p);

            E_1_2p = sqrt(m_p * m_p + P_1_2p_3v.Mag2());
            E_2_2p = sqrt(m_p * m_p + P_2_2p_3v.Mag2());
            Ecal_2p = E_e_2p + (E_1_2p - m_p) + (E_2_2p - m_p);

            //<editor-fold desc="Filling Ecal plots">
            if (Ecal_2p > beamE) {
                hChi2_Electron_Ecal_test_2p->Fill(electrons[0]->par()->getChi2Pid());
                hChi2_Proton_Ecal_test_2p->Fill(protons[0]->par()->getChi2Pid());
                hChi2_Proton_Ecal_test_2p->Fill(protons[1]->par()->getChi2Pid());
            }

            hEcal_All_Int_2p->Fill(Ecal_2p); // Fill Ecal for all interactions

            if (qel) {
                hEcal_QEL_2p->Fill(Ecal_2p); // Fill Ecal for QEL only
            } else if (mec) {
                hEcal_MEC_2p->Fill(Ecal_2p); // Fill Ecal for MEC only
            } else if (res) {
                hEcal_RES_2p->Fill(Ecal_2p); // Fill Ecal for RES only
            } else if (dis) {
                hEcal_DIS_2p->Fill(Ecal_2p); // Fill Ecal for DIS only
            }
            //</editor-fold>

            hdP_T_L_2p->Fill(dP_T_L_2p_3v.Mag());
            hdP_T_tot_2p->Fill(dP_T_tot_2p_3v.Mag());

            dAlpha_T_L = acos(-(P_e_2p_3v.Px() * dP_T_L_2p_3v.Px() + P_e_2p_3v.Py() * dP_T_L_2p_3v.Py() + P_e_2p_3v.Pz() * dP_T_L_2p_3v.Pz())
                              / (P_T_e_2p_3v.Mag() * dP_T_L_2p_3v.Mag())) * 180.0 / pi; // dP_T_L_2p_3v.Pz() = 0; dAlpha_T_L in deg
            dAlpha_T_tot = acos(-(P_e_2p_3v.Px() * dP_T_tot_2p_3v.Px() + P_e_2p_3v.Py() * dP_T_tot_2p_3v.Py() + P_e_2p_3v.Pz() * dP_T_tot_2p_3v.Pz())
                                / (P_T_e_2p_3v.Mag() * dP_T_tot_2p_3v.Mag())) * 180.0 / pi; // dP_T_tot_2p_3v.Pz() = 0; dAlpha_T_tot in deg
            hdAlpha_T_L_2p->Fill(dAlpha_T_L);
            hdAlpha_T_tot_2p->Fill(dAlpha_T_tot);

            hdP_T_L_vs_dAlpha_T_L_2p->Fill(dAlpha_T_L, dP_T_L_2p_3v.Mag());
            hdP_T_tot_vs_dAlpha_T_tot_2p->Fill(dAlpha_T_tot, dP_T_tot_2p_3v.Mag());

            dPhi_T_L = acos(-(P_T_e_2p_3v.Px() * P_T_L_2p_3v.Px() + P_T_e_2p_3v.Py() * P_T_L_2p_3v.Py() + P_T_e_2p_3v.Pz() * P_T_L_2p_3v.Pz())
                            / (P_T_e_2p_3v.Mag() * P_T_L_2p_3v.Mag())) * 180.0 / pi; // P_T_L_2p_3v.Pz() = 0; dPhi_T_L in deg
            dPhi_T_tot = acos(-(P_T_e_2p_3v.Px() * P_T_tot_2p_3v.Px() + P_T_e_2p_3v.Py() * P_T_tot_2p_3v.Py() + P_T_e_2p_3v.Pz() * P_T_tot_2p_3v.Pz())
                              / (P_T_e_2p_3v.Mag() * P_T_tot_2p_3v.Mag())) * 180.0 / pi; // P_T_tot_2p_3v.Pz() = 0; dPhi_T_tot in deg
            hdPhi_T_L_2p->Fill(dPhi_T_L);
            hdPhi_T_tot_2p->Fill(dPhi_T_tot);

            hEcal_vs_dAlpha_T_L_2p->Fill(dAlpha_T_L, Ecal_2p);
            hEcal_vs_dAlpha_T_tot_2p->Fill(dAlpha_T_tot, Ecal_2p);
            hEcal_vs_dP_T_L_2p->Fill(dP_T_L_2p_3v.Mag(), Ecal_2p);
            hEcal_vs_dP_T_tot_2p->Fill(dP_T_tot_2p_3v.Mag(), Ecal_2p);
        } // end of 1e2p & 2p cuts if
        //</editor-fold>

    } // end of while
    // </editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        Histograms plots                                                                             //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Histograms plots">

// ======================================================================================================================================================================
// Canvas definitions
// ======================================================================================================================================================================

    //<editor-fold desc="Canvas definitions">

    //<editor-fold desc="Canvas c1">
    TCanvas *c1 = new TCanvas("canvas", "canvas", 2000, 1500);
//    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
//    c1->cd();
    c1->SetGrid();
    c1->SetBottomMargin(0.14);

    if (wider_margin) {
        c1->SetLeftMargin(0.16);
        c1->SetRightMargin(0.12);
//        c1->SetLeftMargin(0.14);
//        c1->SetRightMargin(0.12);
    }

    float DefStatX = gStyle->GetStatX();
    float DefStatY = gStyle->GetStatY();
    //</editor-fold>

    //<editor-fold desc="Canvas c2">
    TCanvas *c2 = new TCanvas("canvas2", "canvas2", 1650, 1150);
    c2->SetGrid();
    c2->SetBottomMargin(0.14);
    c2->SetTopMargin(0.15);

    if (wider_margin) {
        c2->SetLeftMargin(0.14);
    }
    //</editor-fold>

    c1->cd();
    //</editor-fold>

// ======================================================================================================================================================================
// Cut parameters plots
// ======================================================================================================================================================================

    //<editor-fold desc="Cut parameters plots">

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Number of Photo-electrons (Nphe) histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms">
    if (Nphe_plots) {
        cout << "\n\nPlotting number of photo-electrons (Nphe) histograms...\n\n";

        //<editor-fold desc="Number of Photo-electrons (Nphe) histogram (1e cut, FD)">
        if (apply_cuts == false) {
//            histPlotter1D(c1, hNphe_1e_cut_BC_FD.Histogram1d, norm_Nphe_plots, true, 1., hNphe_1e_cut_BC_FD.HistogramTitle, hNphe_1e_cut_BC_FD.FinalState, plots, 2,
//                          false, true, hNphe_1e_cut_BC_FD.SaveName, hNphe_1e_cut_BC_FD.SaveDirectory, "", true, false, true, clasAna.getNpheCuts(), 0, false);
//            histPlotter1D(c1, hNphe_1e_cut_AC_FD.Histogram1d, norm_Nphe_plots, true, 1., hNphe_1e_cut_AC_FD.HistogramTitle, hNphe_1e_cut_AC_FD.FinalState, plots, 2,
//                          false, true, hNphe_1e_cut_AC_FD.SaveName, hNphe_1e_cut_AC_FD.SaveDirectory, "", true, false, true, clasAna.getNpheCuts(), 0, false);
////            histPlotter1D(c1, hNphe_1e_cut_BC_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC - N_{phe} Before Cuts", "1e Cut", plots, 2, false, true,
////                          sNphe_1e_cut_FD, "01_Nphe_1e_cut_BC", hNphe_1e_cut_BC_FD_Dir, "", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0, false);
////            histPlotter1D(c1, hNphe_1e_cut_AC_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC - N_{phe} After Cuts", "1e Cut", plots, 2, false, true,
////                          sNphe_1e_cut_FD, "02_Nphe_1e_cut_AC", hNphe_1e_cut_AC_FD_Dir, "", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0, false);
        } else {
            hNphe_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
//            hNphe_1e_cut_BC_FD.hDrawAndSave(c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 0, false);
//            histPlotter1D(c1, hNphe_1e_cut_BC_FD.Histogram1d, norm_Nphe_plots, true, 1., hNphe_1e_cut_BC_FD.HistogramTitle, hNphe_1e_cut_BC_FD.FinalState, plots, 2,
//                          false, true, hNphe_1e_cut_BC_FD.SaveName, hNphe_1e_cut_BC_FD.SaveDirectory, "", true, false, true, clasAna.getNpheCuts(), 0, false);
//            histPlotter1D(c1, hNphe_1e_cut_BC_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC - N_{phe}", "1e Cut", plots, 2, false, true, sNphe_1e_cut_FD,
//                          "01_Nphe_1e_cut", hNphe_1e_cut_BC_FD_Dir, "", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0, false);
        }

        //<editor-fold desc="Number of Photo-electrons (Nphe) histogram (2p, FD)">
        histPlotter1D(c1, hNphe_2p_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC - N_{phe}", "2p", plots, 2, false, true, sNphe_2p_FD, "01_Nphe_2p",
                      hNphe_2p_FD_Dir, "", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0, false);
        //</editor-fold>

    } else {
        cout << "\n\nnumber of photo-electrons (Nphe) plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">
    if (Chi2_plots) {
        cout << "\n\nPlotting Chi2 plots...\n\n";

        //<editor-fold desc="Finding Xmax">
        Chi2_Electron_cuts_FD.MeanFromHistogram = hChi2_Electron_1e_cut_FD->GetBinCenter(hChi2_Electron_1e_cut_FD->GetMaximumBin());
        Chi2_Proton_cuts_CD.MeanFromHistogram = hChi2_Proton_1e_cut_CD->GetBinCenter(hChi2_Proton_1e_cut_CD->GetMaximumBin());
        Chi2_Proton_cuts_FD.MeanFromHistogram = hChi2_Proton_1e_cut_FD->GetBinCenter(hChi2_Proton_1e_cut_FD->GetMaximumBin());
        Chi2_Kplus_cuts_CD.MeanFromHistogram = hChi2_Kplus_1e_cut_CD->GetBinCenter(hChi2_Kplus_1e_cut_CD->GetMaximumBin());
        Chi2_Kplus_cuts_FD.MeanFromHistogram = hChi2_Kplus_1e_cut_FD->GetBinCenter(hChi2_Kplus_1e_cut_FD->GetMaximumBin());
        Chi2_Kminus_cuts_CD.MeanFromHistogram = hChi2_Kminus_1e_cut_CD->GetBinCenter(hChi2_Kminus_1e_cut_CD->GetMaximumBin());
        Chi2_Kminus_cuts_FD.MeanFromHistogram = hChi2_Kminus_1e_cut_FD->GetBinCenter(hChi2_Kminus_1e_cut_FD->GetMaximumBin());
        Chi2_piplus_cuts_CD.MeanFromHistogram = hChi2_piplus_1e_cut_CD->GetBinCenter(hChi2_piplus_1e_cut_CD->GetMaximumBin());
        Chi2_piplus_cuts_FD.MeanFromHistogram = hChi2_piplus_1e_cut_FD->GetBinCenter(hChi2_piplus_1e_cut_FD->GetMaximumBin());
        Chi2_piminus_cuts_CD.MeanFromHistogram = hChi2_piminus_1e_cut_CD->GetBinCenter(hChi2_piminus_1e_cut_CD->GetMaximumBin());
        Chi2_piminus_cuts_FD.MeanFromHistogram = hChi2_piminus_1e_cut_FD->GetBinCenter(hChi2_piminus_1e_cut_FD->GetMaximumBin());
        //</editor-fold>

//  Chi2 plots (no #(e) cut) ----------------------------------------------------------------------------

        //<editor-fold desc="Chi2 plots (no #(e) cut)">
        histPlotter1D(c1, hChi2_Electron_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Electron_All_e, "01_Electron_chi2", hChi2_Electron_FD_Dir, "FD", kRed, true, true, true, false);

        histPlotter1D(c1, hChi2_Proton_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_All_e, "02_Proton_chi2", hChi2_Proton_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2),
                      Chi2_Proton_cuts_CD.Cuts.at(0));
        histPlotter1D(c1, hChi2_Proton_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_All_e, "02_Proton_chi2", hChi2_Proton_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Proton_cuts_FD.Cuts.at(2),
                      Chi2_Proton_cuts_FD.Cuts.at(0));

//        histPlotter1D(c1, hChi2_Kplus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Kplus_All_e, "03_Kplus_chi2", hChi2_Kplus_CD_Dir, "CD", kBlue, true, true, true, false);
//        histPlotter1D(c1, hChi2_Kplus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Kplus_All_e, "03_Kplus_chi2", hChi2_Kplus_FD_Dir, "FD", kRed, true, true, true, false);

//        histPlotter1D(c1, hChi2_Kminus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Kminus_All_e, "04_Kminus_chi2", hChi2_Kminus_CD_Dir, "CD", kBlue, true, true, true, false);
//        histPlotter1D(c1, hChi2_Kminus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Kminus_All_e, "04_Kminus_chi2", hChi2_Kminus_FD_Dir, "FD", kRed, true, true, true, false);

//        histPlotter1D(c1, hChi2_piplus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_piplus_All_e, "03_piplus_chi2", hChi2_piplus_CD_Dir, "CD", kBlue, true, true, true, false);
//        histPlotter1D(c1, hChi2_piplus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_piplus_All_e, "03_piplus_chi2", hChi2_piplus_FD_Dir, "FD", kRed, true, true, true, false);

//        histPlotter1D(c1, hChi2_piminus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_piminus_All_e, "04_piminus_chi2", hChi2_piminus_CD_Dir, "CD", kBlue, true, true, true, false);
//        histPlotter1D(c1, hChi2_piminus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_piminus_All_e, "04_piminus_chi2", hChi2_piminus_FD_Dir, "FD", kRed, true, true, true, false);
        //</editor-fold>

//  Chi2 plots (1e cut) ----------------------------------------------------------------------------

        //<editor-fold desc="Chi2 plots (1e cut)">
        histPlotter1D(c1, hChi2_Electron_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Electron_1e_cut, "01_Electron_chi2", hChi2_Electron_1e_cut_FD_Dir, "FD", kRed, true, true, true, false);

        if (apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            histPlotter1DwFit(c1, hChi2_Proton_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram With Fit", "1e Cut", plots, "02_Proton_chi2_wFit",
                              hChi2_Proton_1e_cut_CD_Dir, "CD", Chi2_Proton_cuts_CD.Cuts.at(0), Chi2_Proton_cuts_CD.Cuts.at(1), Chi2_Proton_cuts_CD.Cuts.at(2),
                              Chi2_Proton_cuts_CD.FitStdFactor, true, "Protons");
            histPlotter1DwFit(c1, hChi2_Proton_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram With Fit", "1e Cut", plots, "02_Proton_chi2_wFit",
                              hChi2_Proton_1e_cut_FD_Dir, "FD", Chi2_Proton_cuts_FD.Cuts.at(0), Chi2_Proton_cuts_FD.Cuts.at(1), Chi2_Proton_cuts_FD.Cuts.at(2),
                              Chi2_Proton_cuts_FD.FitStdFactor, true, "Protons");
        } else {
            histPlotter1D(c1, hChi2_Proton_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_Proton_1e_cut, "02_Proton_chi2", hChi2_Proton_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2),
                          Chi2_Proton_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, hChi2_Proton_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_Proton_1e_cut, "02_Proton_chi2", hChi2_Proton_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Proton_cuts_FD.Cuts.at(2),
                          Chi2_Proton_cuts_FD.Cuts.at(0));
        }

        if (apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            histPlotter1DwFit(c1, hChi2_Kplus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram With Fit", "1e Cut", plots, "03_Kplus_chi2_wFit",
                              hChi2_Kplus_1e_cut_CD_Dir, "CD", Chi2_Kplus_cuts_CD.Cuts.at(0), Chi2_Kplus_cuts_CD.Cuts.at(1), Chi2_Kplus_cuts_CD.Cuts.at(2),
                              Chi2_Kplus_cuts_CD.FitStdFactor, true, "K+");
            histPlotter1DwFit(c1, hChi2_Kplus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram With Fit", "1e Cut", plots, "03_Kplus_chi2_wFit",
                              hChi2_Kplus_1e_cut_FD_Dir, "FD", Chi2_Kplus_cuts_FD.Cuts.at(0), Chi2_Kplus_cuts_FD.Cuts.at(1), Chi2_Kplus_cuts_FD.Cuts.at(2),
                              Chi2_Kplus_cuts_FD.FitStdFactor, true, "K+");
        } else {
            histPlotter1D(c1, hChi2_Kplus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_Kplus_1e_cut, "03_Kplus_chi2", hChi2_Kplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Kplus_cuts_CD.Cuts.at(2),
                          Chi2_Kplus_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, hChi2_Kplus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_Kplus_1e_cut, "03_Kplus_chi2", hChi2_Kplus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Kplus_cuts_FD.Cuts.at(2),
                          Chi2_Kplus_cuts_FD.Cuts.at(0));
        }

        if (apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            histPlotter1DwFit(c1, hChi2_Kminus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram With Fit", "1e Cut", plots, "04_Kminus_chi2_wFit",
                              hChi2_Kminus_1e_cut_CD_Dir, "CD", Chi2_Kminus_cuts_CD.Cuts.at(0), Chi2_Kminus_cuts_CD.Cuts.at(1), Chi2_Kminus_cuts_CD.Cuts.at(2),
                              Chi2_Kminus_cuts_CD.FitStdFactor, true, "K-");
            histPlotter1DwFit(c1, hChi2_Kminus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram With Fit", "1e Cut", plots, "04_Kminus_chi2_wFit",
                              hChi2_Kminus_1e_cut_FD_Dir, "FD", Chi2_Kminus_cuts_FD.Cuts.at(0), Chi2_Kminus_cuts_FD.Cuts.at(1), Chi2_Kminus_cuts_FD.Cuts.at(2),
                              Chi2_Kminus_cuts_FD.FitStdFactor, true, "K-");
        } else {
            histPlotter1D(c1, hChi2_Kminus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_Kminus_1e_cut, "04_Kminus_chi2", hChi2_Kminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Kminus_cuts_CD.Cuts.at(2),
                          Chi2_Kminus_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, hChi2_Kminus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_Kminus_1e_cut, "04_Kminus_chi2", hChi2_Kminus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Kminus_cuts_FD.Cuts.at(2),
                          Chi2_Kminus_cuts_FD.Cuts.at(0));
        }

        if (apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            histPlotter1DwFit(c1, hChi2_piplus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram With Fit", "1e Cut", plots, "05_piplus_chi2_wFit",
                              hChi2_piplus_1e_cut_CD_Dir, "CD", Chi2_piplus_cuts_CD.Cuts.at(0), Chi2_piplus_cuts_CD.Cuts.at(1), Chi2_piplus_cuts_CD.Cuts.at(2),
                              Chi2_piplus_cuts_CD.FitStdFactor, true, "pi+");
            histPlotter1DwFit(c1, hChi2_piplus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram With Fit", "1e Cut", plots, "05_piplus_chi2_wFit",
                              hChi2_piplus_1e_cut_FD_Dir, "FD", Chi2_piplus_cuts_FD.Cuts.at(0), Chi2_piplus_cuts_FD.Cuts.at(1), Chi2_piplus_cuts_FD.Cuts.at(2),
                              Chi2_piplus_cuts_FD.FitStdFactor, true, "pi+");
        } else {
            histPlotter1D(c1, hChi2_piplus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_piplus_1e_cut, "05_piplus_chi2", hChi2_piplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_piplus_cuts_CD.Cuts.at(2),
                          Chi2_piplus_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, hChi2_piplus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_piplus_1e_cut, "05_piplus_chi2", hChi2_piplus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_piplus_cuts_FD.Cuts.at(2),
                          Chi2_piplus_cuts_FD.Cuts.at(0));
        }

        if (apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            histPlotter1DwFit(c1, hChi2_piminus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram With Fit", "1e Cut", plots, "06_piminus_chi2_wFit",
                              hChi2_piminus_1e_cut_CD_Dir, "CD", Chi2_piminus_cuts_CD.Cuts.at(0), Chi2_piminus_cuts_CD.Cuts.at(1), Chi2_piminus_cuts_CD.Cuts.at(2),
                              Chi2_piminus_cuts_CD.FitStdFactor, true, "pi-");
            histPlotter1DwFit(c1, hChi2_piminus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram With Fit", "1e Cut", plots, "06_piminus_chi2_wFit",
                              hChi2_piminus_1e_cut_FD_Dir, "FD", Chi2_piminus_cuts_FD.Cuts.at(0), Chi2_piminus_cuts_FD.Cuts.at(1), Chi2_piminus_cuts_FD.Cuts.at(2),
                              Chi2_piminus_cuts_FD.FitStdFactor, true, "pi-");
        } else {
            histPlotter1D(c1, hChi2_piminus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_piminus_1e_cut, "06_piminus_chi2", hChi2_piminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true,
                          Chi2_piminus_cuts_CD.Cuts.at(2), Chi2_piminus_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, hChi2_piminus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sChi2_piminus_1e_cut, "06_piminus_chi2", hChi2_piminus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true,
                          Chi2_piminus_cuts_FD.Cuts.at(2), Chi2_piminus_cuts_FD.Cuts.at(0));
        }
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (1e2p)">
        if (apply_cuts == false) {
            histPlotter1D(c1, c2, hChi2_Electron_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e} Histogram Before Cut", "1e2p", "FD", "e", 0.06, 0.04, 0.04,
                          plots, 2, false, true, sChi2_Electron_1e2p, "02_Electron_Chi2_1e2p_BC", hChi2_Electron_1e2p_BC_FD_Dir, kBlue, true, true, true, false);

            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram Before Cut", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots, 2,
                          false, true, sChi2_Proton_1e2p, "03_Proton_Chi2_1e2p_BC", hChi2_Proton_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram Before Cut", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots, 2,
                          false, true, sChi2_Proton_1e2p, "04_Proton_Chi2_1e2p_BC", hChi2_Proton_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(0));

            histPlotter1D(c1, c2, hChi2_Electron_1e2p_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e} Histogram After Cut", "1e2p", "FD", "e", 0.06, 0.04, 0.04,
                          plots, 2, false, true, sChi2_Electron_1e2p, "02_Electron_Chi2_1e2p_AC", hChi2_Electron_1e2p_AC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Electron_cuts_FD.Cuts.at(2), Chi2_Electron_cuts_FD.Cuts.at(0));

            histPlotter1D(c1, c2, hChi2_Proton_1e2p_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram After Cut", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots,
                          2, false, true, sChi2_Proton_1e2p, "03_Proton_Chi2_1e2p_AC", hChi2_Proton_1e2p_AC_CD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, c2, hChi2_Proton_1e2p_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram After Cut", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots,
                          2, false, true, sChi2_Proton_1e2p, "04_Proton_Chi2_1e2p_AC", hChi2_Proton_1e2p_AC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(0));
        } else {
            histPlotter1D(c1, c2, hChi2_Electron_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e} Histogram", "1e2p", "FD", "e", 0.06, 0.04, 0.04, plots, 2,
                          false, true, sChi2_Electron_1e2p, "02_Electron_Chi2_1e2p", hChi2_Electron_1e2p_BC_FD_Dir, kBlue, true, true, true, false);

            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots, 2, false,
                          true, sChi2_Proton_1e2p, "03_Proton_Chi2_1e2p", hChi2_Proton_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots, 2, false,
                          true, sChi2_Proton_1e2p, "04_Proton_Chi2_1e2p", hChi2_Proton_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(0));
        }
        //</editor-fold>

        //<editor-fold desc="Testing Chi2 (2p)">
        histPlotter1D(c1, hChi2_Electron_2p_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Electron_1e2p, "01_Electron_Chi2_2p", hChi2_Electron_2p_FD_Dir, "", kBlue, true, true, true, false);

        histPlotter1D(c1, hChi2_Proton_2p_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sChi2_Proton_1e2p,
                      "02_Proton_Chi2_2p", hChi2_Proton_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2),
                      Chi2_Proton_cuts_CD.Cuts.at(0));
        histPlotter1D(c1, hChi2_Proton_2p_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sChi2_Proton_1e2p,
                      "02_Proton_Chi2_2p", hChi2_Proton_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, Chi2_Proton_cuts_FD.Cuts.at(2),
                      Chi2_Proton_cuts_FD.Cuts.at(0));

        //<editor-fold desc="Chi2 plots for Ecal>Ebeam (2p)">
        histPlotter1D(c1, hChi2_Electron_Ecal_test_2p, norm_Chi2_plots, true, 1., ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "2p",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, sChi2_Electron_1e2p, "01_Electron_Chi2_Ecal_test_2p", hChi2_Electron_Ecal_test_2p_Dir, "", kBlue, true,
                      true, true, false);
        histPlotter1D(c1, hChi2_Proton_Ecal_test_2p, norm_Chi2_plots, true, 1., ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sChi2_Proton_1e2p, "02_Proton_Chi2_Ecal_test_2p", hChi2_Proton_Ecal_test_2p_Dir, "", kBlue, true, true,
                      true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nChi2 plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">
    if (Vertex_plots) {
        cout << "\n\nPlotting Vertex plots...\n\n";

//  dV plots (1e2p, CD & FD) ----------------------------------------------------------------------------

        //<editor-fold desc="Vertesx plots (1e cut, CD & FD)">
        histPlotter1D(c1, hVx_Electron_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{x}^{e}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sVx_Electron_1e_cut, "01_Electron_Vx", hVx_Electron_1e_cut_FD_Dir, "", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Electron_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{y}^{e}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sVy_Electron_1e_cut, "02_Electron_Vy", hVy_Electron_1e_cut_FD_Dir, "", kBlue, true, true, true, false);
        histPlotter1D(c1, hVz_Electron_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{z}^{e}", "1e Cut", plots, 2, false, true, sVz_Electron_1e_cut, "03_Electron_Vz",
                      hVz_Electron_1e_cut_FD_Dir, "", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);

        histPlotter1D(c1, hVx_Proton_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{x}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_Proton_1e_cut,
                      "01_Proton_Vx", hVx_Proton_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVx_Proton_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{x}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_Proton_1e_cut,
                      "01_Proton_Vx", hVx_Proton_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Proton_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{y}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_Proton_1e_cut,
                      "02_Proton_Vy", hVy_Proton_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Proton_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{y}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_Proton_1e_cut,
                      "02_Proton_Vy", hVy_Proton_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVz_Proton_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{z}^{p}", "1e Cut", plots, 2, false, true, sVz_Proton_1e_cut, "03_Proton_Vz",
                      hVz_Proton_1e_cut_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);
        histPlotter1D(c1, hVz_Proton_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{z}^{p}", "1e Cut", plots, 2, false, true, sVz_Proton_1e_cut, "03_Proton_Vz",
                      hVz_Proton_1e_cut_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);

        histPlotter1D(c1, hVx_Kplus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{x}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_Kplus_1e_cut,
                      "01_Kplus_Vx", hVx_Kplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVx_Kplus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{x}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_Kplus_1e_cut,
                      "01_Kplus_Vx", hVx_Kplus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Kplus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{y}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_Kplus_1e_cut,
                      "02_Kplus_Vy", hVy_Kplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Kplus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{y}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_Kplus_1e_cut,
                      "02_Kplus_Vy", hVy_Kplus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVz_Kplus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{z}^{K^{+}}", "1e Cut", plots, 2, false, true, sVz_Kplus_1e_cut, "03_Kplus_Vz",
                      hVz_Kplus_1e_cut_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);
        histPlotter1D(c1, hVz_Kplus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{z}^{K^{+}}", "1e Cut", plots, 2, false, true, sVz_Kplus_1e_cut, "03_Kplus_Vz",
                      hVz_Kplus_1e_cut_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);

        histPlotter1D(c1, hVx_Kminus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{x}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_Kminus_1e_cut,
                      "01_Kminus_Vx", hVx_Kminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVx_Kminus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{x}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_Kminus_1e_cut,
                      "01_Kminus_Vx", hVx_Kminus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Kminus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{y}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_Kminus_1e_cut,
                      "02_Kminus_Vy", hVy_Kminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_Kminus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{y}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_Kminus_1e_cut,
                      "02_Kminus_Vy", hVy_Kminus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVz_Kminus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{z}^{K^{-}}", "1e Cut", plots, 2, false, true, sVz_Kminus_1e_cut, "03_Kminus_Vz",
                      hVz_Kminus_1e_cut_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);
        histPlotter1D(c1, hVz_Kminus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{z}^{K^{-}}", "1e Cut", plots, 2, false, true, sVz_Kminus_1e_cut, "03_Kminus_Vz",
                      hVz_Kminus_1e_cut_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);

        histPlotter1D(c1, hVx_piplus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_piplus_1e_cut,
                      "01_piplus_Vx", hVx_piplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVx_piplus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVx_piplus_1e_cut,
                      "01_piplus_Vx", hVx_piplus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_piplus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_piplus_1e_cut,
                      "02_piplus_Vy", hVy_piplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_piplus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sVy_piplus_1e_cut,
                      "02_piplus_Vy", hVy_piplus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVz_piplus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{+}}", "1e Cut", plots, 2, false, true, sVz_piplus_1e_cut, "03_piplus_Vz",
                      hVz_piplus_1e_cut_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);
        histPlotter1D(c1, hVz_piplus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{+}}", "1e Cut", plots, 2, false, true, sVz_piplus_1e_cut, "03_piplus_Vz",
                      hVz_piplus_1e_cut_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);

        histPlotter1D(c1, hVx_piminus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sVx_piminus_1e_cut, "01_piminus_Vx", hVx_piminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVx_piminus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sVx_piminus_1e_cut, "01_piminus_Vx", hVx_piminus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_piminus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sVy_piminus_1e_cut, "02_piminus_Vy", hVy_piminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVy_piminus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sVy_piminus_1e_cut, "02_piminus_Vy", hVy_piminus_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, hVz_piminus_1e_cut_CD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{-}}", "1e Cut", plots, 2, false, true, sVz_piminus_1e_cut, "03_piminus_Vz",
                      hVz_piminus_1e_cut_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);
        histPlotter1D(c1, hVz_piminus_1e_cut_FD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{-}}", "1e Cut", plots, 2, false, true, sVz_piminus_1e_cut, "03_piminus_Vz",
                      hVz_piminus_1e_cut_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cut.GetUpperCut(), Vz_cut.GetLowerCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="dV plots (1e2p, CD & FD)">
        if (apply_cuts == false) {
            histPlotter1D(c1, hdVx_1e2p_BC, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e}_{x} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                          true, sdVx_1e2p_before, "01_dVx_BC", hdVx_1e2p_BC_Dir, "CD & FD", kBlue, true, true, true, false);
            histPlotter1D(c1, hdVy_1e2p_BC, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e}_{y} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                          true, sdVy_1e2p_before, "02_dVy_BC", hdVy_1e2p_BC_Dir, "CD & FD", kBlue, true, true, true, false);
            histPlotter1D(c1, hdVz_1e2p_BC, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e}_{z} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                          true, sdVz_1e2p_before, "03_dVz_BC", hdVz_1e2p_BC_Dir, "CD & FD", kBlue, true, true, true, false, true, dVz_cuts.GetUpperCut(), 0,
                          false);

            histPlotter1D(c1, hdVx_1e2p_AC, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e}_{x} After Cuts", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sdVx_1e2p_after, "01_dVx_AC", hdVx_1e2p_AC_Dir, "CD & FD", kBlue, true, true, true, false);
            histPlotter1D(c1, hdVy_1e2p_AC, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e}_{y} After Cuts", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sdVy_1e2p_after, "02_dVy_AC", hdVy_1e2p_AC_Dir, "CD & FD", kBlue, true, true, true, false);
            histPlotter1D(c1, hdVz_1e2p_AC, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e}_{z} After Cuts", "1e2p", plots, 2, false, true, sdVz_1e2p_after,
                          "03_dVz_AC", hdVz_1e2p_AC_Dir, "CD & FD", kBlue, false, true, false, true, dVz_cuts.GetUpperCut(), dVz_cuts.GetLowerCut(), 0, false);
        } else {
            histPlotter1D(c1, hdVx_1e2p_BC, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e}_{x}", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sdVx_1e2p_before, "01_dVx", hdVx_1e2p_BC_Dir, "", kBlue, true, true, true, false);
            histPlotter1D(c1, hdVy_1e2p_BC, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e}_{y}", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sdVy_1e2p_before, "02_dVy", hdVy_1e2p_BC_Dir, "", kBlue, true, true, true, false);
            histPlotter1D(c1, hdVz_1e2p_BC, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e}_{z}", "1e2p", plots, 2, false, true, sdVy_1e2p_before, "03_dVz",
                          hdVz_1e2p_BC_Dir, "", kBlue, false, true, false, true, dVz_cuts.GetUpperCut(), dVz_cuts.GetLowerCut(), 0, false);
        }
        //</editor-fold>

        //<editor-fold desc="dV plots after dV cuts (2p, CD & FD)">
        histPlotter1D(c1, hdVx_2p, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e}_{x}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sdVx_2p, "01_dVx",
                      hdVx_2p_Dir, "", kBlue, true, true, true, false);
        histPlotter1D(c1, hdVy_2p, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e}_{y}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sdVy_2p, "02_dVy",
                      hdVy_2p_Dir, "", kBlue, true, true, true, false);
        histPlotter1D(c1, hdVz_2p, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e}_{z}", "2p", plots, 2, false, true, sdVz_2p, "03_dVz", hdVz_2p_Dir, "", kBlue,
                      false, true, false, true, dVz_cuts.GetUpperCut(), dVz_cuts.GetLowerCut(), 0, false);
        //</editor-fold>

    } else {
        cout << "\n\nVertex plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Sampling Fraction (SF) histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Sampling Fraction (SF) histograms">
    if (SF_plots) {
        cout << "\n\nPlotting Sampling Fraction histograms...\n\n";

//  SF histograms --------------------------------------------------------------

        //<editor-fold desc="SF and SF vs. P histograms (1e cut, FD)">
        if (apply_cuts == false) {
            histPlotter1D(c1, hSF_1e_cut_BC_FD, norm_SF_plots, true, 1., "Sampling Fraction f Before Cuts", "1e Cut", plots, 2, false, true, sSF_1e_cut_FD,
                          "01_SF_1e_cut_before_SF_cuts", hSF_1e_cut_BC_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getEcalSFUpperCut(),
                          clasAna.getEcalSFLowerCut(), 0, false);

            histPlotter1D(c1, hSF_1e_cut_AC_FD, norm_SF_plots, true, 1., "Sampling Fraction f After cuts", "1e Cut", plots, 2, false, true, sSF_1e_cut_FD,
                          "02_SF_1e_cut_after_SF_cuts", hSF_1e_cut_AC_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getEcalSFUpperCut(),
                          clasAna.getEcalSFLowerCut(), 0, false);

            //<editor-fold desc="SF vs. P before cuts (1e cut, FD)">
            hSF_VS_P_e_1e_cut_BC_FD->SetTitleSize(0.06, "xyz");
            hSF_VS_P_e_1e_cut_BC_FD->GetXaxis()->SetLabelSize(0.0425);
            hSF_VS_P_e_1e_cut_BC_FD->GetXaxis()->CenterTitle(true);
            hSF_VS_P_e_1e_cut_BC_FD->GetYaxis()->SetLabelSize(0.0425);
            hSF_VS_P_e_1e_cut_BC_FD->GetYaxis()->CenterTitle(true);
            hSF_VS_P_e_1e_cut_BC_FD->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(hSF_VS_P_e_1e_cut_BC_FD);
            hSF_VS_P_e_1e_cut_BC_FD->Draw("colz");
            c1->SetLogz(1);
            hSF_VS_P_e_1e_cut_BC_FD->SetStats(0);
            c1->SaveAs((hSF_VS_P_e_1e_cut_BC_FD_Dir + "01_SF_VS_P_e_1e_cut_BC_FD.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="SF vs. P after cuts (1e cut, FD)">
            hSF_VS_P_e_1e_cut_AC_FD->SetTitleSize(0.06, "xyz");
            hSF_VS_P_e_1e_cut_AC_FD->GetXaxis()->SetLabelSize(0.0425);
            hSF_VS_P_e_1e_cut_AC_FD->GetXaxis()->CenterTitle(true);
            hSF_VS_P_e_1e_cut_AC_FD->GetYaxis()->SetLabelSize(0.0425);
            hSF_VS_P_e_1e_cut_AC_FD->GetYaxis()->CenterTitle(true);
            hSF_VS_P_e_1e_cut_AC_FD->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(hSF_VS_P_e_1e_cut_AC_FD);
            hSF_VS_P_e_1e_cut_AC_FD->Draw("colz");
            c1->SetLogz(1);
            hSF_VS_P_e_1e_cut_AC_FD->SetStats(0);
            c1->SaveAs((hSF_VS_P_e_1e_cut_AC_FD_Dir + "02_SF_VS_P_e_1e_cut_AC_FD.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

        } else {
            histPlotter1D(c1, hSF_1e_cut_BC_FD, norm_SF_plots, true, 1., "Sampling Fraction f", "1e Cut", plots, 2, false, true, sSF_1e_cut_FD, "01_SF_1e_cut",
                          hSF_1e_cut_BC_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getEcalSFUpperCut(), clasAna.getEcalSFLowerCut(), 0, false);

            //<editor-fold desc="SF vs. P 1e2p (FD)">
            hSF_VS_P_e_1e_cut_BC_FD->SetTitleSize(0.06, "xyz");
            hSF_VS_P_e_1e_cut_BC_FD->GetXaxis()->SetLabelSize(0.0425);
            hSF_VS_P_e_1e_cut_BC_FD->GetXaxis()->CenterTitle(true);
            hSF_VS_P_e_1e_cut_BC_FD->GetYaxis()->SetLabelSize(0.0425);
            hSF_VS_P_e_1e_cut_BC_FD->GetYaxis()->CenterTitle(true);
            hSF_VS_P_e_1e_cut_BC_FD->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(hSF_VS_P_e_1e_cut_BC_FD);
            hSF_VS_P_e_1e_cut_BC_FD->Draw("colz");
            c1->SetLogz(1);
            hSF_VS_P_e_1e_cut_BC_FD->SetStats(0);
            c1->SaveAs((hSF_VS_P_e_1e_cut_BC_FD_Dir + "01_SF_VS_P_e_1e_cut_FD.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

        }
        //</editor-fold>

        //<editor-fold desc="SF histogram (2p, FD)">
        histPlotter1D(c1, hSF_2p_FD, norm_SF_plots, true, 1., "Sampling Fraction f", "2p", plots, 2, false, true, sSF_2p_FD, "01_SF_2p", hSF_2p_FD_Dir, "FD", kBlue, true,
                      true, false, true, clasAna.getEcalSFUpperCut(), clasAna.getEcalSFLowerCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="SF vs. P (2p, FD)">
        hSF_VS_P_e_2p_FD->SetTitleSize(0.06, "xyz");
        hSF_VS_P_e_2p_FD->GetXaxis()->SetLabelSize(0.0425);
        hSF_VS_P_e_2p_FD->GetXaxis()->CenterTitle(true);
        hSF_VS_P_e_2p_FD->GetYaxis()->SetLabelSize(0.0425);
        hSF_VS_P_e_2p_FD->GetYaxis()->CenterTitle(true);
        hSF_VS_P_e_2p_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(hSF_VS_P_e_2p_FD);
        hSF_VS_P_e_2p_FD->Draw("colz");
        c1->SetLogz(1);
        hSF_VS_P_e_2p_FD->SetStats(0);
        c1->SaveAs((hSF_VS_P_e_2p_FD_Dir + "01_SF_VS_P_e_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

    } else {
        cout << "\n\nSampling Fraction plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ECAL edge histograms (electrons only, FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="ECAL edge histograms (electrons only, FD only)">
    if (fiducial_plots) {
        cout << "\n\nPlotting fiducial histograms...\n\n";

//  fiducial histograms --------------------------------------------------------------

        if (apply_cuts == false) {

            //<editor-fold desc="Vcal vs. EoP 1e2p BC PCAL">
            Vcal_VS_EoP_1e_BC_PCAL->SetTitleSize(0.06, "xyz");
            Vcal_VS_EoP_1e_BC_PCAL->GetXaxis()->SetLabelSize(0.0425);
            Vcal_VS_EoP_1e_BC_PCAL->GetXaxis()->CenterTitle(true);
            Vcal_VS_EoP_1e_BC_PCAL->GetYaxis()->SetLabelSize(0.0425);
            Vcal_VS_EoP_1e_BC_PCAL->GetYaxis()->CenterTitle(true);
            Vcal_VS_EoP_1e_BC_PCAL->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(Vcal_VS_EoP_1e_BC_PCAL);
            Vcal_VS_EoP_1e_BC_PCAL->Draw("colz");
            c1->SetLogz(1);
            Vcal_VS_EoP_1e_BC_PCAL->SetStats(0);
            c1->SaveAs((Vcal_VS_EoP_1e_BC_PCAL_Dir + "01_Vcal_VS_EoP_1e_BC_PCAL.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Wcal vs. EoP 1e2p BC PCAL">
            Wcal_VS_EoP_1e_BC_PCAL->SetTitleSize(0.06, "xyz");
            Wcal_VS_EoP_1e_BC_PCAL->GetXaxis()->SetLabelSize(0.0425);
            Wcal_VS_EoP_1e_BC_PCAL->GetXaxis()->CenterTitle(true);
            Wcal_VS_EoP_1e_BC_PCAL->GetYaxis()->SetLabelSize(0.0425);
            Wcal_VS_EoP_1e_BC_PCAL->GetYaxis()->CenterTitle(true);
            Wcal_VS_EoP_1e_BC_PCAL->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(Wcal_VS_EoP_1e_BC_PCAL);
            Wcal_VS_EoP_1e_BC_PCAL->Draw("colz");
            c1->SetLogz(1);
            Wcal_VS_EoP_1e_BC_PCAL->SetStats(0);
            c1->SaveAs((Wcal_VS_EoP_1e_BC_PCAL_Dir + "02_Wcal_VS_EoP_1e_BC_PCAL.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Vcal vs. EoP 1e2p AC PCAL">
            Vcal_VS_EoP_1e_AC_PCAL->SetTitleSize(0.06, "xyz");
            Vcal_VS_EoP_1e_AC_PCAL->GetXaxis()->SetLabelSize(0.0425);
            Vcal_VS_EoP_1e_AC_PCAL->GetXaxis()->CenterTitle(true);
            Vcal_VS_EoP_1e_AC_PCAL->GetYaxis()->SetLabelSize(0.0425);
            Vcal_VS_EoP_1e_AC_PCAL->GetYaxis()->CenterTitle(true);
            Vcal_VS_EoP_1e_AC_PCAL->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(Vcal_VS_EoP_1e_AC_PCAL);
            Vcal_VS_EoP_1e_AC_PCAL->Draw("colz");
            c1->SetLogz(1);
            Vcal_VS_EoP_1e_AC_PCAL->SetStats(0);
            c1->SaveAs((Vcal_VS_EoP_1e_AC_PCAL_Dir + "01_Vcal_VS_EoP_1e_AC_PCAL.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Wcal vs. EoP 1e2p AC PCAL">
            Wcal_VS_EoP_1e_AC_PCAL->SetTitleSize(0.06, "xyz");
            Wcal_VS_EoP_1e_AC_PCAL->GetXaxis()->SetLabelSize(0.0425);
            Wcal_VS_EoP_1e_AC_PCAL->GetXaxis()->CenterTitle(true);
            Wcal_VS_EoP_1e_AC_PCAL->GetYaxis()->SetLabelSize(0.0425);
            Wcal_VS_EoP_1e_AC_PCAL->GetYaxis()->CenterTitle(true);
            Wcal_VS_EoP_1e_AC_PCAL->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(Wcal_VS_EoP_1e_AC_PCAL);
            Wcal_VS_EoP_1e_AC_PCAL->Draw("colz");
            c1->SetLogz(1);
            Wcal_VS_EoP_1e_AC_PCAL->SetStats(0);
            c1->SaveAs((Wcal_VS_EoP_1e_AC_PCAL_Dir + "02_Wcal_VS_EoP_1e_AC_PCAL.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

        } else {

            //<editor-fold desc="Vcal vs. EoP 1e2p BC PCAL">
            Vcal_VS_EoP_1e_BC_PCAL->SetTitleSize(0.06, "xyz");
            Vcal_VS_EoP_1e_BC_PCAL->GetXaxis()->SetLabelSize(0.0425);
            Vcal_VS_EoP_1e_BC_PCAL->GetXaxis()->CenterTitle(true);
            Vcal_VS_EoP_1e_BC_PCAL->GetYaxis()->SetLabelSize(0.0425);
            Vcal_VS_EoP_1e_BC_PCAL->GetYaxis()->CenterTitle(true);
            Vcal_VS_EoP_1e_BC_PCAL->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(Vcal_VS_EoP_1e_BC_PCAL);
            Vcal_VS_EoP_1e_BC_PCAL->Draw("colz");
            c1->SetLogz(1);
            Vcal_VS_EoP_1e_BC_PCAL->SetStats(0);
            c1->SaveAs((Vcal_VS_EoP_1e_BC_PCAL_Dir + "01_Vcal_VS_EoP_1e_PCAL.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Wcal vs. EoP 1e2p BC PCAL">
            Wcal_VS_EoP_1e_BC_PCAL->SetTitleSize(0.06, "xyz");
            Wcal_VS_EoP_1e_BC_PCAL->GetXaxis()->SetLabelSize(0.0425);
            Wcal_VS_EoP_1e_BC_PCAL->GetXaxis()->CenterTitle(true);
            Wcal_VS_EoP_1e_BC_PCAL->GetYaxis()->SetLabelSize(0.0425);
            Wcal_VS_EoP_1e_BC_PCAL->GetYaxis()->CenterTitle(true);
            Wcal_VS_EoP_1e_BC_PCAL->GetZaxis()->SetLabelSize(0.0425);
            plots->Add(Wcal_VS_EoP_1e_BC_PCAL);
            Wcal_VS_EoP_1e_BC_PCAL->Draw("colz");
            c1->SetLogz(1);
            Wcal_VS_EoP_1e_BC_PCAL->SetStats(0);
            c1->SaveAs((Wcal_VS_EoP_1e_BC_PCAL_Dir + "02_Wcal_VS_EoP_1e_PCAL.png").c_str());
            gStyle->SetStatX(DefStatX);
            gStyle->SetStatY(DefStatY);
            c1->Clear();
            //</editor-fold>

        }

        //<editor-fold desc="Vcal vs. EoP 2p PCAL">
        Vcal_VS_EoP_2p_PCAL->SetTitleSize(0.06, "xyz");
        Vcal_VS_EoP_2p_PCAL->GetXaxis()->SetLabelSize(0.0425);
        Vcal_VS_EoP_2p_PCAL->GetXaxis()->CenterTitle(true);
        Vcal_VS_EoP_2p_PCAL->GetYaxis()->SetLabelSize(0.0425);
        Vcal_VS_EoP_2p_PCAL->GetYaxis()->CenterTitle(true);
        Vcal_VS_EoP_2p_PCAL->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Vcal_VS_EoP_2p_PCAL);
        Vcal_VS_EoP_2p_PCAL->Draw("colz");
        c1->SetLogz(1);
        Vcal_VS_EoP_2p_PCAL->SetStats(0);
        c1->SaveAs((Vcal_VS_EoP_2p_PCAL_Dir + "01_Vcal_VS_EoP_2p_PCAL.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="Wcal vs. EoP 2p PCAL">
        Wcal_VS_EoP_2p_PCAL->SetTitleSize(0.06, "xyz");
        Wcal_VS_EoP_2p_PCAL->GetXaxis()->SetLabelSize(0.0425);
        Wcal_VS_EoP_2p_PCAL->GetXaxis()->CenterTitle(true);
        Wcal_VS_EoP_2p_PCAL->GetYaxis()->SetLabelSize(0.0425);
        Wcal_VS_EoP_2p_PCAL->GetYaxis()->CenterTitle(true);
        Wcal_VS_EoP_2p_PCAL->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Wcal_VS_EoP_2p_PCAL);
        Wcal_VS_EoP_2p_PCAL->Draw("colz");
        c1->SetLogz(1);
        Wcal_VS_EoP_2p_PCAL->SetStats(0);
        c1->SaveAs((Wcal_VS_EoP_2p_PCAL_Dir + "02_Wcal_VS_EoP_2p_PCAL.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

    } else {
        cout << "\n\nFiducial plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum histograms">
    if (Momentum_plots) {
        cout << "\n\nPlotting Momentum histograms...\n\n";

//  Momentum histograms (1e cut, CD & FD)  --------------------------------------------------------------

        //<editor-fold desc="Momentum histograms (1e cut, CD & FD)">
        histPlotter1D(c1, hP_e_1e_cut_FD, false, true, 1., "P_{e}", "1e cut", plots, 2, false, true, sP_FD, "01_P_e_1e_cut_FD", hP_e_1e_cut_FD_Dir, "FD", kBlue, true,
                      true, false, true, -1, -1, 0, false);

        histPlotter1D(c1, hP_p_1e_cut_CD, false, true, 1., "P_{p}", "1e cut", plots, 2, false, true, sP_CD, "02_P_p_1e_cut_CD", hP_p_1e_cut_CD_Dir, "CD", kBlue, true,
                      true, false, true, -1, -1, 0, false);
        histPlotter1D(c1, hP_p_1e_cut_FD, false, true, 1., "P_{p}", "1e cut", plots, 2, false, true, sP_FD, "02_P_p_1e_cut_FD", hP_p_1e_cut_FD_Dir, "FD", kBlue, true,
                      true, false, true, -1, -1, 0, false);

        histPlotter1D(c1, hP_piplus_1e_cut_CD, false, true, 1., "P_{#pi^{+}}", "1e cut", plots, 2, false, true, sP_CD, "03_P_piplus_1e_cut_CD", hP_piplus_1e_cut_CD_Dir,
                      "CD", kBlue, true, true, false, true, -1, -1, 0, false);
        histPlotter1D(c1, hP_piplus_1e_cut_FD, false, true, 1., "P_{#pi^{+}}", "1e cut", plots, 2, false, true, sP_FD, "03_P_piplus_1e_cut_FD", hP_piplus_1e_cut_FD_Dir,
                      "FD", kBlue, true, true, false, true, -1, -1, 0, false);

        histPlotter1D(c1, hP_piminus_1e_cut_CD, false, true, 1., "P_{#pi^{-}}", "1e cut", plots, 2, false, true, sP_CD, "04_P_piminus_1e_cut_CD",
                      hP_piminus_1e_cut_CD_Dir, "CD", kBlue, true, true, false, true, -1, -1, 0, false);
        histPlotter1D(c1, hP_piminus_1e_cut_FD, false, true, 1., "P_{#pi^{-}}", "1e cut", plots, 2, false, true, sP_FD, "04_P_piminus_1e_cut_FD",
                      hP_piminus_1e_cut_FD_Dir, "FD", kBlue, true, true, false, true, -1, -1, 0, false);
        //</editor-fold>

//  Momentum histograms (1e2p, CD & FD)  --------------------------------------------------------------

        //<editor-fold desc="Momentum histograms before & after cuts (1e2p, CD & FD)">
        histPlotter1D(c1, hP_e_1e2p_BC_FD, false, true, 1., "P_{e} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_FD, "02_P_e_1e2p_BC_FD", hP_e_1e2p_BC_FD_Dir,
                      "", kBlue, true, true, false, true, e_momentum_cuts_2p.GetUpperCut(), e_momentum_cuts_2p.GetLowerCut(), 0, false);
        histPlotter1D(c1, hP_e_1e2p_AC_FD, false, true, 1., "P_{e} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_FD, "02_P_e_1e2p_AC_FD", hP_e_1e2p_AC_FD_Dir,
                      "", kBlue, true, true, false, true, e_momentum_cuts_2p.GetUpperCut(), e_momentum_cuts_2p.GetLowerCut(), 0, false);

        histPlotter1D(c1, hP_p_1e2p_BC_CD, false, true, 1., "P_{p} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_CD, "03_P_p_1e2p_BC_CD", hP_p_1e2p_BC_CD_Dir,
                      "CD", kBlue, true, true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);
        histPlotter1D(c1, hP_p_1e2p_AC_CD, false, true, 1., "P_{p} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_CD, "03_P_p_1e2p_AC_CD", hP_p_1e2p_AC_CD_Dir,
                      "CD", kBlue, true, true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);

        histPlotter1D(c1, hP_p_1e2p_BC_FD, false, true, 1., "P_{p} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_FD, "04_P_p_1e2p_BC_FD", hP_p_1e2p_BC_FD_Dir,
                      "FD", kBlue, true, true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);
        histPlotter1D(c1, hP_p_1e2p_AC_FD, false, true, 1., "P_{p} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_FD, "04_P_p_1e2p_AC_FD", hP_p_1e2p_AC_FD_Dir,
                      "FD", kBlue, true, true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="Momentum histograms before & after cuts (2p, CD & FD)">
        histPlotter1D(c1, hP_e_2p_FD, false, true, 1., "Electron momentum P_{e}", "2p", plots, 2, false, true, sP_2p_FD, "01_P_e_2p_FD", hP_e_2p_FD_Dir, "", kBlue, true,
                      true, false, true, e_momentum_cuts_2p.GetUpperCut(), e_momentum_cuts_2p.GetLowerCut(), 0, false);

        histPlotter1D(c1, hP_p_2p_CD, false, true, 1., "Proton momentum P_{p}", "2p", plots, 2, false, true, sP_2p_CD, "02_P_p_2p_CD", hP_p_2p_CD_Dir, "CD", kBlue, true,
                      true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);
        histPlotter1D(c1, hP_p_2p_FD, false, true, 1., "Proton momentum P_{p}", "2p", plots, 2, false, true, sP_2p_FD, "02_P_p_2p_FD", hP_p_2p_FD_Dir, "FD", kBlue, true,
                      true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);

        histPlotter1D(c1, hP_p_1_2p, false, true, 1., "Leading proton momentum P_{1}", "2p", plots, 2, false, true, sP_2p_CD, "03_P_p_1_2p", hP_p_1_2p_Dir, "", kBlue,
                      true, true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);
        histPlotter1D(c1, hP_p_2_2p, false, true, 1., "Recoil proton momentum P_{2}", "2p", plots, 2, false, true, sP_2p_FD, "03_P_p_2_2p", hP_p_2_2p_Dir, "", kBlue,
                      true, true, false, true, p_momentum_cuts_2p.GetUpperCut(), p_momentum_cuts_2p.GetLowerCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="P1 vs P2 (2p, CD & FD)">
        histPlotter2D(c1, hP_p_1_vs_P_p_2_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hP_p_1_vs_P_p_2_2p_Dir, "04_P_p_1_vs_P_p_2", true);
        //</editor-fold>

    } else {
        cout << "\n\nMomentum (P_e) plots are disabled by user.\n\n";
    }
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Beta vs. P histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Beta vs. P histograms">
    if (Beta_vs_P_plots) {
        cout << "\n\nPlotting Beta vs. P histograms...\n\n";

//  Beta vs. P TF1 plots --------------------------------------------------------

        //<editor-fold desc="Beta vs. P TF1 plots">
        auto *beta_neutron = new TF1("beta_neutron", ("x/sqrt(x*x + " + to_string(m_n * m_n) + ")").c_str(), 0, beamE);
        auto *beta_proton = new TF1("beta_proton", ("x/sqrt(x*x + " + to_string(m_p * m_p) + ")").c_str(), 0, beamE);
        auto *beta_Kplus = new TF1("beta_Kplus", ("x/sqrt(x*x + " + to_string(m_Kplus * m_Kplus) + ")").c_str(), 0, beamE);
        auto *beta_Kminus = new TF1("beta_Kminus", ("x/sqrt(x*x + " + to_string(m_Kminus * m_Kminus) + ")").c_str(), 0, beamE);
        auto *beta_Kzero = new TF1("beta_Kplus", ("x/sqrt(x*x + " + to_string(m_Kzero * m_Kzero) + ")").c_str(), 0, beamE);
        auto *beta_piplus = new TF1("beta_piplus", ("x/sqrt(x*x + " + to_string(m_piplus * m_piplus) + ")").c_str(), 0, beamE);
        auto *beta_piminus = new TF1("beta_piminus", ("x/sqrt(x*x + " + to_string(m_piminus * m_piminus) + ")").c_str(), 0, beamE);
        auto *beta_pizero = new TF1("beta_piplus", ("x/sqrt(x*x + " + to_string(m_pizero * m_pizero) + ")").c_str(), 0, beamE);
        auto *beta_electron = new TF1("beta_electron", ("x/sqrt(x*x + " + to_string(m_e * m_e) + ")").c_str(), 0, beamE);
        auto *beta_photon = new TF1("beta_electron", ("x/sqrt(x*x + " + to_string(0) + ")").c_str(), 0, beamE);
        //</editor-fold>

//  Beta vs. P histograms --------------------------------------------------------

        //<editor-fold desc="Beta vs. P plots (no cuts)">

        //<editor-fold desc="Beta vs. P for all particles (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_CD_Dir, "01_Beta_vs_P_All_Particles_CD.png", beta_electron,
                      beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_FD_Dir, "01_Beta_vs_P_All_Particles_FD.png", beta_electron,
                      beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for all particles (Electrons Only, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_Electrons_Only_FD_Dir, "02_Beta_vs_P_Electrons_Only_FD.png",
                      beta_electron, "Electrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for all particles (Protons Only, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_Protons_Only_CD_Dir, "03_Beta_vs_P_Protons_Only_CD.png",
                      beta_proton, "Protons", true);

        histPlotter2D(c1, Beta_vs_P_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_Protons_Only_FD_Dir, "03_Beta_vs_P_Protons_Only_FD.png",
                      beta_proton, "Protons", true);
        //</editor-fold>

//        //<editor-fold desc="Beta vs. P for all particles (Neutrons Only, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_Neutrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_Neutrons_Only_CD_Dir,
//                      "04_Beta_vs_P_Neutrons_Only_CD.png",
//                      beta_neutron, "Neutrons", true);
//
//        histPlotter2D(c1, Beta_vs_P_Neutrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_Neutrons_Only_FD_Dir,
//                      "04_Beta_vs_P_Neutrons_Only_FD.png",
//                      beta_neutron, "Neutrons", true);
//        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (all particles, by charge)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_positive_particles_All_e_CD_Dir,
                      "01_Beta_vs_P_q_p1_All_e_CD.png", beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_positive_particles_All_e_FD_Dir,
                      "01_Beta_vs_P_q_p1_All_e_FD.png", beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_neutral_particles_All_e_CD_Dir,
                      "02_Beta_vs_P_q_0_All_e_CD.png", beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_neutral_particles_All_e_FD_Dir,
                      "02_Beta_vs_P_q_0_All_e_FD.png", beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_negative_particles_All_e_CD_Dir,
                      "03_Beta_vs_P_q_m1_All_e_CD.png", beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_negative_particles_All_e_FD_Dir,
                      "03_Beta_vs_P_q_m1_All_e_FD.png", beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (1e cut)">

        //<editor-fold desc="Beta vs. P for all particles (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_cut_CD_Dir, "01_Beta_vs_P_1e_cut_All_Particles_CD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_cut_FD_Dir, "01_Beta_vs_P_1e_cut_All_Particles_FD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Electrons Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Electrons_Only_FD_Dir,
                      "02_Beta_vs_P_1e_cut_Electrons_Only_FD.png", beta_electron, "Electrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Protons Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Protons_Only_CD_Dir,
                      "03_Beta_vs_P_1e_cut_Protons_Only_CD.png", beta_proton, "Protons", true);

        histPlotter2D(c1, Beta_vs_P_1e_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Protons_Only_FD_Dir,
                      "03_Beta_vs_P_1e_cut_Protons_Only_FD.png", beta_proton, "Protons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Neutrons Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Neutrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Neutrons_Only_CD_Dir,
                      "04_Beta_vs_P_1e_cut_Neutrons_Only_CD.png", beta_neutron, "Neutrons", true);

        histPlotter2D(c1, Beta_vs_P_1e_Neutrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Neutrons_Only_FD_Dir,
                      "04_Beta_vs_P_1e_cut_Neutrons_Only_FD.png", beta_neutron, "Neutrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Kplus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Kplus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Kplus_Only_CD_Dir,
                      "05_Beta_vs_P_1e_cut_Kplus_Only_CD.png", beta_Kplus, "K^{+}", true);

        histPlotter2D(c1, Beta_vs_P_1e_Kplus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Kplus_Only_FD_Dir,
                      "05_Beta_vs_P_1e_cut_Kplus_Only_FD.png", beta_Kplus, "K^{+}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Kminus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Kminus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Kminus_Only_CD_Dir,
                      "06_Beta_vs_P_1e_cut_Kminus_Only_CD.png", beta_Kminus, "K^{-}", true);

        histPlotter2D(c1, Beta_vs_P_1e_Kminus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_Kminus_Only_FD_Dir,
                      "06_Beta_vs_P_1e_cut_Kminus_Only_FD.png", beta_Kminus, "K^{-}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for piplus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_piplus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_piplus_Only_CD_Dir,
                      "08_Beta_vs_P_1e_cut_piplus_Only_CD.png", beta_piplus, "#pi^{+}", true);

        histPlotter2D(c1, Beta_vs_P_1e_piplus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_piplus_Only_FD_Dir,
                      "08_Beta_vs_P_1e_cut_piplus_Only_FD.png", beta_piplus, "#pi^{+}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for piminus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_piminus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_piminus_Only_CD_Dir,
                      "09_Beta_vs_P_1e_cut_piminus_Only_CD.png", beta_piminus, "#pi^{-}", true);

        histPlotter2D(c1, Beta_vs_P_1e_piminus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_piminus_Only_FD_Dir,
                      "09_Beta_vs_P_1e_cut_piminus_Only_FD.png", beta_piminus, "#pi^{-}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for pizero Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_pizero_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_pizero_Only_CD_Dir,
                      "10_Beta_vs_P_1e_cut_pizero_Only_CD.png", beta_pizero, "#pi^{0}", true);

        histPlotter2D(c1, Beta_vs_P_1e_pizero_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_1e_pizero_Only_FD_Dir,
                      "10_Beta_vs_P_1e_cut_pizero_Only_FD.png", beta_pizero, "#pi^{0}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (by charge, 1e cut, CD & FD)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_positive_particles_1e_cut_CD_Dir,
                      "01_Beta_vs_P_q_p1_1e_cut_CD.png", beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_positive_particles_1e_cut_FD_Dir,
                      "01_Beta_vs_P_q_p1_1e_cut_FD.png", beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_neutral_particles_1e_cut_CD_Dir,
                      "02_Beta_vs_P_q_0_1e_cut_CD.png", beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_neutral_particles_1e_cut_FD_Dir,
                      "02_Beta_vs_P_q_0_1e_cut_FD.png", beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_negative_particles_1e_cut_CD_Dir,
                      "03_Beta_vs_P_q_m1_1e_cut_CD.png", beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_negative_particles_1e_cut_FD_Dir,
                      "03_Beta_vs_P_q_m1_1e_cut_FD.png", beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (2p)">

        //<editor-fold desc="Beta vs. P for all particles (2p, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_2p_CD_Dir, "01_Beta_vs_P_2p_All_Particles_CD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_2p_FD_Dir, "01_Beta_vs_P_2p_All_Particles_FD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Electrons Only (2p, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_2p_Electrons_Only_FD_Dir, "02_Beta_vs_P_2p_Electrons_Only_FD.png",
                      beta_electron, "Electrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Protons Only (2p, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_2p_Protons_Only_CD_Dir, "03_Beta_vs_P_2p_Protons_Only_CD.png",
                      beta_proton, "Protons", true);

        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_2p_Protons_Only_FD_Dir, "03_Beta_vs_P_2p_Protons_Only_FD.png",
                      beta_proton, "Protons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (by charge, 2p, CD & FD)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_positive_particles_2p_CD_Dir, "01_Beta_vs_P_q_p1_2p_CD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_positive_particles_2p_FD_Dir, "01_Beta_vs_P_q_p1_2p_FD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_neutral_particles_2p_CD_Dir, "02_Beta_vs_P_q_0_2p_CD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_neutral_particles_2p_FD_Dir, "02_Beta_vs_P_q_0_2p_FD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_negative_particles_2p_CD_Dir, "03_Beta_vs_P_q_m1_2p_CD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_negative_particles_2p_FD_Dir, "03_Beta_vs_P_q_m1_2p_FD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nBeta vs. P plots are disabled by user.\n\n";
    } // end of Beta vs. P plot if
    //</editor-fold>

// ======================================================================================================================================================================
// Angle histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Angle histograms">
    if (Angle_plots_master) {
        cout << "\n\nPlotting Angle histograms...\n\n";

//  Theta_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Theta_e (no #(e) cut)">
        double Theta_e_integral = hTheta_e_All_e_FD->Integral();

        histPlotter1D(c1, hTheta_e_All_e_FD, norm_Angle_plots_master, true, Theta_e_integral, "#theta_{e} of Outgoing Electron", "All Int., No #(e) Cut", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e, "Theta_e_All_Int_All_e", hTheta_e_All_e_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        //</editor-fold>

        //<editor-fold desc="Theta_e (1e cut)">
        double Theta_e_1e_integral = hTheta_e_1e_cut_FD->Integral();
        histPlotter1D(c1, hTheta_e_1e_cut_FD, norm_Angle_plots_master, true, Theta_e_1e_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e Cut", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e, "Theta_e_All_Int_1e_cut", hTheta_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        //</editor-fold>

//        //<editor-fold desc="Theta_e (1e2X)">
//        double Theta_e_1e_1e2X_integral = hTheta_e_All_Int_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_QEL_integral = hTheta_e_QEL_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_MEC_integral = hTheta_e_MEC_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_RES_integral = hTheta_e_RES_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_DIS_integral = hTheta_e_DIS_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_integral = hTheta_e_All_Int_1e2X_CD->Integral() + hTheta_e_All_Int_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_QEL_integral = hTheta_e_QEL_1e2X_CD->Integral() + hTheta_e_QEL_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_MEC_integral = hTheta_e_MEC_1e2X_CD->Integral() + hTheta_e_MEC_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_RES_integral = hTheta_e_RES_1e2X_CD->Integral() + hTheta_e_RES_1e2X_FD->Integral();
//        double Theta_e_1e_1e2X_DIS_integral = hTheta_e_DIS_1e2X_CD->Integral() + hTheta_e_DIS_1e2X_FD->Integral();
//
//        <editor-fold desc="Theta_e (1e2X, CD)">
//        histPlotter1D(c1, hTheta_e_All_Int_1e2X_CD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_integral, "#theta_{e} 1e2X", "All Int.", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "00_Theta_e_1e2X", hTheta_e_All_Int_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_QEL_1e2X_CD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_QEL_integral, "#theta_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "01_Theta_e_1e2X_QEL_Only", hTheta_e_QEL_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_MEC_1e2X_CD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_MEC_integral, "#theta_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "02_Theta_e_1e2X_MEC_Only", hTheta_e_MEC_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_RES_1e2X_CD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_RES_integral, "#theta_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "03_Theta_e_1e2X_RES_Only", hTheta_e_RES_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_DIS_1e2X_CD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_DIS_integral, "#theta_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "04_Theta_e_1e2X_DIS_Only", hTheta_e_DIS_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//        </editor-fold>
//
//        <editor-fold desc="Theta_e (1e2X, FD)">
//        histPlotter1D(c1, hTheta_e_All_Int_1e2X_FD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_integral, "#theta_{e} 1e2X", "All Int.", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "00_Theta_e_1e_cut_1e2X", hTheta_e_All_Int_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_QEL_1e2X_FD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_QEL_integral, "#theta_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "01_Theta_e_1e2X_QEL_Only", hTheta_e_QEL_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_MEC_1e2X_FD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_MEC_integral, "#theta_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "02_Theta_e_1e2X_MEC_Only", hTheta_e_MEC_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_RES_1e2X_FD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_RES_integral, "#theta_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "03_Theta_e_1e2X_RES_Only", hTheta_e_RES_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_DIS_1e2X_FD, norm_Angle_plots_master, true, Theta_e_1e_1e2X_DIS_integral, "#theta_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "04_Theta_e_1e2X_DIS_Only", hTheta_e_DIS_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Theta_e (1e2p)">
        double Theta_e_All_Int_1e2p_integral = hTheta_e_All_Int_1e2p_FD->Integral();
        double Theta_e_QEL_1e2p_integral = hTheta_e_QEL_1e2p_FD->Integral();
        double Theta_e_MEC_1e2p_integral = hTheta_e_MEC_1e2p_FD->Integral();
        double Theta_e_RES_1e2p_integral = hTheta_e_RES_1e2p_FD->Integral();
        double Theta_e_DIS_1e2p_integral = hTheta_e_DIS_1e2p_FD->Integral();

        //<editor-fold desc="Theta_e 1e2p (FD)">
        histPlotter1D(c1, hTheta_e_All_Int_1e2p_FD, norm_Angle_plots_master, true, Theta_e_All_Int_1e2p_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e2p",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "00_Theta_e_All_Int_1e2p", hTheta_e_All_Int_1e2p_FD_Dir, "FD", kBlue, true, true,
                      true, false, true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_QEL_1e2p_FD, norm_Angle_plots_master, true, Theta_e_QEL_1e2p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 1e2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "01_Theta_e_QEL_Only_1e2p", hTheta_e_QEL_1e2p_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_MEC_1e2p_FD, norm_Angle_plots_master, true, Theta_e_MEC_1e2p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 1e2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "02_Theta_e_MEC_Only_1e2p", hTheta_e_MEC_1e2p_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_RES_1e2p_FD, norm_Angle_plots_master, true, Theta_e_RES_1e2p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 1e2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "03_Theta_e_RES_Only_1e2p", hTheta_e_RES_1e2p_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_DIS_1e2p_FD, norm_Angle_plots_master, true, Theta_e_DIS_1e2p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 1e2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "04_Theta_e_DIS_Only_1e2p", hTheta_e_DIS_1e2p_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);

        stackPlotter1D(c1, sTheta_e_1e2p_FD, norm_Angle_plots_master, "#theta_{e} of Outgoing Electron", "1e2p", plots, hTheta_e_All_Int_1e2p_FD, hTheta_e_QEL_1e2p_FD,
                       hTheta_e_MEC_1e2p_FD, hTheta_e_RES_1e2p_FD, hTheta_e_DIS_1e2p_FD, "05_Theta_e_Stack_1e2p", hTheta_e_All_Int_1e2p_FD_Dir, "FD");
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Theta_e (2p)">
        double Theta_e_All_Int_2p_integral = hTheta_e_All_Int_2p_FD->Integral();
        double Theta_e_QEL_2p_integral = hTheta_e_QEL_2p_FD->Integral();
        double Theta_e_MEC_2p_integral = hTheta_e_MEC_2p_FD->Integral();
        double Theta_e_RES_2p_integral = hTheta_e_RES_2p_FD->Integral();
        double Theta_e_DIS_2p_integral = hTheta_e_DIS_2p_FD->Integral();

        //<editor-fold desc="Theta_e 2p (FD)">
        histPlotter1D(c1, hTheta_e_All_Int_2p_FD, norm_Angle_plots_master, true, Theta_e_All_Int_2p_integral, "#theta_{e} of Outgoing Electron", "All Int., 2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_2p_FD, "00_Theta_e_All_Int_2p", hTheta_e_All_Int_2p_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_QEL_2p_FD, norm_Angle_plots_master, true, Theta_e_QEL_2p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_2p_FD, "01_Theta_e_QEL_Only_2p", hTheta_e_QEL_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_MEC_2p_FD, norm_Angle_plots_master, true, Theta_e_MEC_2p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_2p_FD, "02_Theta_e_MEC_Only_2p", hTheta_e_MEC_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_RES_2p_FD, norm_Angle_plots_master, true, Theta_e_RES_2p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_2p_FD, "03_Theta_e_RES_Only_2p", hTheta_e_RES_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_DIS_2p_FD, norm_Angle_plots_master, true, Theta_e_DIS_2p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_2p_FD, "04_Theta_e_DIS_Only_2p", hTheta_e_DIS_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);

        stackPlotter1D(c1, sTheta_e_2p_FD, norm_Angle_plots_master, "#theta_{e} of Outgoing Electron", "2p", plots, hTheta_e_All_Int_2p_FD, hTheta_e_QEL_2p_FD,
                       hTheta_e_MEC_2p_FD, hTheta_e_RES_2p_FD, hTheta_e_DIS_2p_FD, "05_Theta_e_Stack_2p", hTheta_e_All_Int_2p_FD_Dir, "FD");
        //</editor-fold>

        //</editor-fold>

//  Phi_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Phi_e (no #(e) cut)">
        double Phi_e_integral = hPhi_e_All_e_FD->Integral();

        histPlotter1D(c1, hPhi_e_All_e_FD, norm_Angle_plots_master, true, Phi_e_integral, "#phi_{e} of Outgoing Electron", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2,
                      false,
                      true, sPhi_e, "Phi_e_All_Int_All_e", hPhi_e_All_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e (1e cut)">
        double Phi_e_1e_integral = hPhi_e_1e_cut_FD->Integral();

        histPlotter1D(c1, hPhi_e_1e_cut_FD, norm_Angle_plots_master, true, Phi_e_1e_integral, "#phi_{e} of Outgoing Electron", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2,
                      false,
                      true, sPhi_e, "Phi_e_All_Int_1e_cut", hPhi_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi of protons (1e2pXy)">
        double Phi_p1_1e2pXy_CD_integral = hPhi_p1_1e2pXy_CD->Integral(), Phi_p2_1e2pXy_CD_integral = hPhi_p2_1e2pXy_CD->Integral();

        histPlotter1D(c1, hPhi_p1_1e2pXy_CD, norm_Angle_plots_master, true, Phi_p1_1e2pXy_CD_integral, "#phi_{p_{1}} of Leading Proton", "1e2pXy", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_Proton_1e2pXy, "01_Phi_p1_All_Int_1e2pXy", hPhi_p1_1e2pXy_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_p2_1e2pXy_CD, norm_Angle_plots_master, true, Phi_p2_1e2pXy_CD_integral, "#phi_{p_{2}} of Recoil Proton", "1e2pXy", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_Proton_1e2pXy, "01_Phi_p2_All_Int_1e2pXy", hPhi_p2_1e2pXy_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

//        //<editor-fold desc="Phi_e (1e2X)">
//        double Phi_e_1e_1e2X_integral = hPhi_e_1e2X_CD->Integral() + hPhi_e_1e2X_FD->Integral();
//        double Phi_e_1e_1e2X_QEL_integral = hPhi_e_1e2X_QEL_CD->Integral() + hPhi_e_1e2X_QEL_FD->Integral();
//        double Phi_e_1e_1e2X_MEC_integral = hPhi_e_1e2X_MEC_CD->Integral() + hPhi_e_1e2X_MEC_FD->Integral();
//        double Phi_e_1e_1e2X_RES_integral = hPhi_e_1e2X_RES_CD->Integral() + hPhi_e_1e2X_RES_FD->Integral();
//        double Phi_e_1e_1e2X_DIS_integral = hPhi_e_1e2X_DIS_CD->Integral() + hPhi_e_1e2X_DIS_FD->Integral();
//
//        //<editor-fold desc="Phi_e (1e2X, CD)">
//        histPlotter1D(c1, hPhi_e_1e2X_CD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2X", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "00_phi_e_All_Int_1e2X", hPhi_e_1e2X_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_QEL_CD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_QEL_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2X", hPhi_e_1e2X_QEL_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_MEC_CD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_MEC_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2X", hPhi_e_1e2X_MEC_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_RES_CD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_RES_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2X", hPhi_e_1e2X_RES_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_DIS_CD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_DIS_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1e2X", hPhi_e_1e2X_DIS_CD_Dir, "CD", kBlue, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="Phi_e (1e2X, FD)">
//        histPlotter1D(c1, hPhi_e_1e2X_FD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2X", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "00_phi_e_All_Int_1e2X", hPhi_e_1e2X_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_QEL_FD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_QEL_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2X", hPhi_e_1e2X_QEL_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_MEC_FD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_MEC_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2X", hPhi_e_1e2X_MEC_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_RES_FD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_RES_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2X", hPhi_e_1e2X_RES_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_DIS_FD, norm_Angle_plots_master, true, Phi_e_1e_1e2X_DIS_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1e2X", hPhi_e_1e2X_DIS_FD_Dir, "FD", kBlue, true, true, true);
//        //</editor-fold>
//
//        //</editor-fold>

        //<editor-fold desc="Phi_e (1e2p)">
        double Phi_e_All_Int_1e2p_integral = hPhi_e_All_Int_1e2p_FD->Integral();
        double Phi_e_QEL_1e2p_integral = hPhi_e_QEL_1e2p_FD->Integral();
        double Phi_e_MEC_1e2p_integral = hPhi_e_MEC_1e2p_FD->Integral();
        double Phi_e_RES_1e2p_integral = hPhi_e_RES_1e2p_FD->Integral();
        double Phi_e_DIS_1e2p_integral = hPhi_e_DIS_1e2p_FD->Integral();

        //<editor-fold desc="Phi_e (1e2p, FD)">
        histPlotter1D(c1, hPhi_e_All_Int_1e2p_FD, norm_Angle_plots_master, true, Phi_e_All_Int_1e2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sPhi_e, "00_Phi_e_All_Int_1e2p", hPhi_e_All_Int_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_1e2p_FD, norm_Angle_plots_master, true, Phi_e_QEL_1e2p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2p", hPhi_e_QEL_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_1e2p_FD, norm_Angle_plots_master, true, Phi_e_MEC_1e2p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2p", hPhi_e_MEC_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_1e2p_FD, norm_Angle_plots_master, true, Phi_e_RES_1e2p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2p", hPhi_e_RES_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_1e2p_FD, norm_Angle_plots_master, true, Phi_e_DIS_1e2p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1e2p", hPhi_e_DIS_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Phi_e (2p)">
        double Phi_e_All_Int_2p_integral = hPhi_e_All_Int_2p_FD->Integral();
        double Phi_e_QEL_2p_integral = hPhi_e_QEL_2p_FD->Integral();
        double Phi_e_MEC_2p_integral = hPhi_e_MEC_2p_FD->Integral();
        double Phi_e_RES_2p_integral = hPhi_e_RES_2p_FD->Integral();
        double Phi_e_DIS_2p_integral = hPhi_e_DIS_2p_FD->Integral();

        //<editor-fold desc="Phi_e (2p, FD)">
        histPlotter1D(c1, hPhi_e_All_Int_2p_FD, norm_Angle_plots_master, true, Phi_e_All_Int_2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 2p", 0.06, 0.0425,
                      0.0425,
                      plots, 2, false, true, sPhi_e, "00_phi_e_All_Int_cut_2p", hPhi_e_All_Int_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_2p_FD, norm_Angle_plots_master, true, Phi_e_QEL_2p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425, 0.0425,
                      plots,
                      2, false, true, sPhi_e, "01_Phi_e_QEL_Only_2p", hPhi_e_QEL_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_2p_FD, norm_Angle_plots_master, true, Phi_e_MEC_2p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425, 0.0425,
                      plots,
                      2, false, true, sPhi_e, "02_Phi_e_MEC_Only_2p", hPhi_e_MEC_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_2p_FD, norm_Angle_plots_master, true, Phi_e_RES_2p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425, 0.0425,
                      plots,
                      2, false, true, sPhi_e, "03_Phi_e_RES_Only_2p", hPhi_e_RES_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_2p_FD, norm_Angle_plots_master, true, Phi_e_DIS_2p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425, 0.0425,
                      plots,
                      2, false, true, sPhi_e, "04_Phi_e_DIS_Only_2p", hPhi_e_DIS_2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

//  Theta_e vs. Phi_e (FD) --------------------------------------------------------------

        //<editor-fold desc="Theta_e vs. Phi_e (no #(e) cut, CD & FD)">
        histPlotter2D(c1, hTheta_e_VS_Phi_e_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_All_e_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_All_e_FD");

        histPlotter2D(c1, hTheta_e_VS_Phi_e_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_1e_cut_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_1e_cut_FD");

        histPlotter2D(c1, hTheta_e_VS_Phi_e_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_2p_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_2p_FD");
        //</editor-fold>

// Theta_p_e_p_tot (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p_e_p_tot (2p, CD & FD)">
        double Theta_p_e_p_tot_2p_integral = hTheta_p_e_p_tot_2p->Integral();

        histPlotter1D(c1, hTheta_p_e_p_tot_2p, norm_Angle_plots_master, true, Theta_p_e_p_tot_2p_integral,
                      "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2}", "All Int., 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_p_e_p_tot_2p, "01_Theta_p_e_p_tot_All_Int_2p", hTheta_p_e_p_tot_2p_Dir, "", kBlue, true, true, true, false);
        //</editor-fold>

// Theta_q_p (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_tot (2p, CD & FD)">
        double Theta_q_p_tot_2p_integral = hTheta_q_p_tot_2p->Integral();

        histPlotter1D(c1, hTheta_q_p_tot_2p, norm_Angle_plots_master, true, Theta_q_p_tot_2p_integral,
                      "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2}", "All Int., 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_q_p_2p, "02_Theta_q_p_tot_All_Int_2p", hTheta_q_p_tot_2p_Dir, "", kBlue, true, true, true, false);
        //</editor-fold>

        //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (2p, CD & FD)">
        double Theta_q_p_L_2p_integral = hTheta_q_p_L_2p->Integral(), Theta_q_p_R_2p_integral = hTheta_q_p_R_2p->Integral();

        histPlotter1D(c1, hTheta_q_p_L_2p, norm_Angle_plots_master, true, Theta_q_p_L_2p_integral,
                      "#theta_{#vec{q},#vec{P}_{1}} - Opening Angle Between #vec{q} and leading proton #vec{P}_{1}", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, sTheta_q_p_2p, "03_Theta_q_p_1_All_Int_2p", hTheta_q_p_L_2p_Dir, "", kBlue, true, true, true, false);
        histPlotter1D(c1, hTheta_q_p_R_2p, norm_Angle_plots_master, true, Theta_q_p_R_2p_integral,
                      "#theta_{#vec{q},#vec{P}_{2}} - Opening Angle Between #vec{q} and recoil proton #vec{P}_{2}", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, sTheta_q_p_2p, "03_Theta_q_p_2_All_Int_2p", hTheta_q_p_R_2p_Dir, "", kBlue, true, true, true, false);
        //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (2p, CD & FD) -------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="hTheta_p1_p2_vs_W_2p (2p, CD & FD)">
        histPlotter2D(c1, hTheta_q_p_L_vs_p_L_q_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_q_p_L_vs_p_L_q_2p_Dir, "04_Theta_q_p_L_vs_p_L_q_2p");
        //</editor-fold>

// Theta_p1_p2 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_p2 (2p, CD & FD)">
        double Theta_p1_p2_integral = hTheta_p1_p2_All_Int_2p->Integral();

        histPlotter1D(c1, hTheta_p1_p2_All_Int_2p, norm_Angle_plots_master, true, Theta_p1_p2_integral, "#theta_{p_{1},p_{2}} - Opening Angle Between Protons",
                      "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sTheta_p1_p2_2p, "00_Theta_p1_p2_All_Int_2p", hTheta_p1_p2_All_Int_2p_Dir, "", kBlue,
                      true, true, true, false);
        histPlotter1D(c1, hTheta_p1_p2_QEL_2p, norm_Angle_plots_master, true, Theta_p1_p2_integral, "#theta_{p_{1},p_{2}} - Opening Angle Between Protons",
                      "QEL only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sTheta_p1_p2_2p, "01_Theta_p1_p2_QEL_only_2p", hTheta_p1_p2_QEL_2p_Dir, "", kBlue,
                      true, true, true, false);
        histPlotter1D(c1, hTheta_p1_p2_MEC_2p, norm_Angle_plots_master, true, Theta_p1_p2_integral, "#theta_{p_{1},p_{2}} - Opening Angle Between Protons",
                      "MEC only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sTheta_p1_p2_2p, "02_Theta_p1_p2_MEC_only_2p", hTheta_p1_p2_MEC_2p_Dir, "", kBlue,
                      true, true, true, false);
        histPlotter1D(c1, hTheta_p1_p2_RES_2p, norm_Angle_plots_master, true, Theta_p1_p2_integral, "#theta_{p_{1},p_{2}} - Opening Angle Between Protons",
                      "RES only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sTheta_p1_p2_2p, "03_Theta_p1_p2_RES_only_2p", hTheta_p1_p2_RES_2p_Dir, "", kBlue,
                      true, true, true, false);
        histPlotter1D(c1, hTheta_p1_p2_DIS_2p, norm_Angle_plots_master, true, Theta_p1_p2_integral, "#theta_{p_{1},p_{2}} - Opening Angle Between Protons",
                      "DIS only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sTheta_p1_p2_2p, "04_Theta_p1_p2_DIS_only_2p", hTheta_p1_p2_DIS_2p_Dir, "", kBlue,
                      true, true, true, false);

        stackPlotter1D(c1, sTheta_p1_p2_2p, norm_E_e_plots, "#theta_{p_{1},p_{2}} - Opening Angle Between Protons", "2p", plots, hTheta_p1_p2_All_Int_2p,
                       hTheta_p1_p2_QEL_2p, hTheta_p1_p2_MEC_2p, hTheta_p1_p2_RES_2p, hTheta_p1_p2_DIS_2p, "05_Theta_p1_p2_Stack", sTheta_p1_p2_2p_Dir, "");
        //</editor-fold>

// hTheta_p1_p2_vs_W_2p (2p, CD & FD) -----------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="hTheta_p1_p2_vs_W_2p (2p, CD & FD)">
        histPlotter2D(c1, hTheta_p1_p2_vs_W_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_p1_p2_vs_W_2p_Dir, "06_Theta_p1_p2_vs_W_2p");
        //</editor-fold>

// Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 10 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 10 (2p, CD & FD)">
        histPlotter2D(c1, hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p_Dir,
                      "07_Theta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p");
        //</editor-fold>

    } else {
        cout << "\n\nAngle plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Q2 histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Q2 histograms">
    if (Q2_plots) {
        cout << "\n\nPlotting Q2 histograms...\n\n";

//  Q2 (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Q2 (no #(e) cut, CD & FD)">
        double Q2_All_e_integral = hQ2_All_e_FD->Integral();

        histPlotter1D(c1, hQ2_All_e_FD, norm_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sQ2_All_e, "Q2_All_e", hQ2_All_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 (1e Cut, CD & FD)">
        double Q2_1e_integral = hQ2_1e_cut_FD->Integral();

        histPlotter1D(c1, hQ2_1e_cut_FD, norm_Q2_plots, true, Q2_1e_integral, "Q^{2} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e_cut,
                      "Q2_1e_cut", hQ2_1e_cut_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

//        //<editor-fold desc="Q2 (1e2X, CD & FD)">
//        double Q2_1e2X_integral = hQ2_1e2X_CD->Integral() + hQ2_1e2X_FD->Integral();
//
//        histPlotter1D(c1, hQ2_1e2X_CD, norm_Q2_plots, true, Q2_1e2X_integral, "Q^{2} Histogram", "1e2X", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e2X,
//                      "Q2_1e2X", hQ2_1e2X_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hQ2_1e2X_FD, norm_Q2_plots, true, Q2_1e2X_integral, "Q^{2} Histogram", "1e2X", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e2X,
//                      "Q2_1e2X", hQ2_1e2X_FD_Dir, "FD", kBlue, true, true, true);
//        //</editor-fold>

        //<editor-fold desc="Q2 (1e2p, CD & FD)">
        double Q2_1e2p_integral = hQ2_1e2p_FD->Integral();

        histPlotter1D(c1, hQ2_1e2p_FD, norm_Q2_plots, true, Q2_1e2p_integral, "Q^{2} Histogram", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e2p,
                      "Q2_1e2p", hQ2_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 (2p, CD & FD)">
        double Q2_2p_integral = hQ2_2p_FD->Integral();

        histPlotter1D(c1, hQ2_2p_FD, norm_Q2_plots, true, Q2_2p_integral, "Q^{2} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_2p, "Q2_2p",
                      hQ2_2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

    } else {
        cout << "\n\nQ2 plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Energy (E_e) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy (E_e) histograms">
    if (E_e_plots) {
        cout << "\n\nPlotting Energy (E_e) histograms...\n\n";

//  Energy (E_e) histograms  --------------------------------------------------------------

        //<editor-fold desc="Energy (E_e) histograms (1e cut, CD & FD)">
        double E_e_integral = hE_e_1e_cut_FD->Integral();

        histPlotter1D(c1, hE_e_1e_cut_FD, norm_E_e_plots, true, E_e_integral, "E_{e} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sE_e_1e_cut,
                      "01_E_e_1e_cut", hE_e_1e_cut_FD_Dir, "", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Energy (E_e) histograms (2p, CD & FD)">
        double E_e_2p_All_Int_integral = hE_e_All_Int_2p_FD->Integral();
        double E_e_QEL_Int_integral = hE_e_QEL_2p_FD->Integral(), E_e_MEC_Int_integral = hE_e_MEC_2p_FD->Integral();
        double E_e_RES_Int_integral = hE_e_RES_2p_FD->Integral(), E_e_DIS_Int_integral = hE_e_DIS_2p_FD->Integral();

        histPlotter1D(c1, hE_e_All_Int_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "00_E_e_All_Int_2p", hE_e_All_Int_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_QEL_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "01_E_e_QEL_Only_2p", hE_e_QEL_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_MEC_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "02_E_e_MEC_Only_2p", hE_e_MEC_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_RES_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "03_E_e_RES_Only_2p", hE_e_RES_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_DIS_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "04_E_e_DIS_Only_2p", hE_e_DIS_2p_FD_Dir, "", kBlue, true, true, true);

        stackPlotter1D(c1, sE_e_2p_FD, norm_E_e_plots, "E_{e} Histogram", "2p", plots, hE_e_All_Int_2p_FD, hE_e_QEL_2p_FD, hE_e_MEC_2p_FD, hE_e_RES_2p_FD,
                       hE_e_DIS_2p_FD, "02_E_e_Stack", sE_e_2p_FD_Dir, "");

        //<editor-fold desc="Energy (E_e) histograms around theta_e = 15 (2p, CD & FD)">
        double E_e_15_2p_All_Int_integral = hE_e_15_All_Int_2p_FD->Integral();
        double E_e_15_QEL_Int_integral = hE_e_15_QEL_2p_FD->Integral(), E_e_15_MEC_Int_integral = hE_e_15_MEC_2p_FD->Integral();
        double E_e_15_RES_Int_integral = hE_e_15_RES_2p_FD->Integral(), E_e_15_DIS_Int_integral = hE_e_15_DIS_2p_FD->Integral();

        histPlotter1D(c1, hE_e_15_All_Int_2p_FD, norm_E_e_plots, true, E_e_15_2p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "All Int., 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sE_e_15_2p_FD, "00_E_e_15_All_Int_2p", hE_e_15_All_Int_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_QEL_2p_FD, norm_E_e_plots, true, E_e_15_2p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "QEL Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_2p_FD, "01_E_e_15_QEL_Only_2p", hE_e_15_QEL_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_MEC_2p_FD, norm_E_e_plots, true, E_e_15_2p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "MEC Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_2p_FD, "02_E_e_15_MEC_Only_2p", hE_e_15_MEC_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_RES_2p_FD, norm_E_e_plots, true, E_e_15_2p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "RES Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_2p_FD, "03_E_e_15_RES_Only_2p", hE_e_15_RES_2p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_DIS_2p_FD, norm_E_e_plots, true, E_e_15_2p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "DIS Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_2p_FD, "04_E_e_15_DIS_Only_2p", hE_e_15_DIS_2p_FD_Dir, "", kBlue, true, true, true);

        stackPlotter1D(c1, sE_e_15_2p_FD, norm_E_e_plots, "E_{e} around #theta_{e} = 15#circ", "2p", plots, hE_e_15_All_Int_2p_FD, hE_e_15_QEL_2p_FD, hE_e_15_MEC_2p_FD,
                       hE_e_15_RES_2p_FD, hE_e_15_DIS_2p_FD, "02_E_e_15_Stack", sE_e_15_2p_FD_Dir, "");
        //</editor-fold>

        //</editor-fold>

//  E_e vs. Theta_e histograms --------------------------------------------------------------

        //<editor-fold desc="E_e vs. Theta_e histograms (1e cut, CD & FD)">
        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_All_e_FD");
        //</editor-fold>

        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_2p_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_QEL_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_QEL_2p_FD_Dir, "01_E_e_VS_Theta_e_QEL_Only_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_MEC_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_MEC_2p_FD_Dir, "02_E_e_VS_Theta_e_MEC_Only_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_RES_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_RES_2p_FD_Dir, "03_E_e_VS_Theta_e_RES_Only_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_DIS_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_DIS_2p_FD_Dir, "04_E_e_VS_Theta_e_DIS_Only_2p_FD");
        //</editor-fold>

    } else {
        cout << "\n\nEnergy (E_e) plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Energy transfer histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy transfer histograms">
    if (ETrans_all_plots) {

        cout << "\n\nPlotting Energy Transfer (ET) histograms...\n\n";

        if (ETrans_All_Int_plots) {
            double ETrans_All_Ang_All_Int_integral_1p = hET_All_Ang_All_Int_1p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_All_Int_1p_FD, norm_ET_plots, true, ETrans_All_Ang_All_Int_integral_1p, tET_All_Ang, "All Int., 1p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, sET_All_Ang_All_Int_1p_FD, "ET_All_Ang_All_Int_1p", hET_All_Ang_All_Int_1p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_All_Int_integral_1p = hET15_All_Int_1p_FD->Integral();

            histPlotter1D(c1, hET15_All_Int_1p_FD, norm_ET_plots, true, ETrans15_All_Int_integral_1p, tET15, "All Int., 1p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1p_FD, "ETrans_15_All_Int_1p", hET15_All_Int_1p_FD_Dir, "", kBlack, true, true, true);

            double ETrans_All_Ang_All_Int_integral_2p = hET_All_Ang_All_Int_2p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_All_Int_2p_FD, norm_ET_plots, true, ETrans_All_Ang_All_Int_integral_2p, tET_All_Ang, "All Int., 2p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, sET_All_Ang_All_Int_2p_FD, "ET_All_Ang_All_Int_2p", hET_All_Ang_All_Int_2p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_All_Int_integral_2p = hET15_All_Int_2p_FD->Integral();

            histPlotter1D(c1, hET15_All_Int_2p_FD, norm_ET_plots, true, ETrans15_All_Int_integral_2p, tET15, "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_All_Int_2p", hET15_All_Int_2p_FD_Dir, "", kBlack, true, true, true);
        }

        if (ETrans_QEL_plots) {
            double ETrans_All_Ang_QEL_integral_1p = hET_All_Ang_QEL_1p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_QEL_1p_FD, norm_ET_plots, true, ETrans_All_Ang_QEL_integral_1p, tET_All_Ang, "QEL Only, 1p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1p_FD, "ET_All_Ang_QEL_1p", hET_All_Ang_QEL_1p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_QEL_integral_1p = hET15_QEL_1p_FD->Integral();

            histPlotter1D(c1, hET15_QEL_1p_FD, norm_ET_plots, true, ETrans15_QEL_integral_1p, tET15, "QEL Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1p_FD, "ETrans_15_QEL_Only_1p", hET15_QEL_1p_FD_Dir, "", kBlue, true, true, true);

            double ETrans_All_Ang_QEL_integral_2p = hET_All_Ang_QEL_2p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_QEL_2p_FD, norm_ET_plots, true, ETrans_All_Ang_QEL_integral_2p, tET_All_Ang, "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_2p_FD, "ET_All_Ang_QEL_2p", hET_All_Ang_QEL_2p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_QEL_integral_2p = hET15_QEL_2p_FD->Integral();

            histPlotter1D(c1, hET15_QEL_2p_FD, norm_ET_plots, true, ETrans15_QEL_integral_2p, tET15, "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_QEL_Only_2p", hET15_QEL_2p_FD_Dir, "", kBlue, true, true, true);
        }

        if (ETrans_MEC_plots) {
            double ETrans_All_Ang_MEC_integral_1p = hET_All_Ang_MEC_1p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_MEC_1p_FD, norm_ET_plots, true, ETrans_All_Ang_MEC_integral_1p, tET_All_Ang, "MEC Only, 1p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1p_FD, "ET_All_Ang_MEC_1p", hET_All_Ang_MEC_1p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_MEC_integral_1p = hET15_MEC_1p_FD->Integral();

            histPlotter1D(c1, hET15_MEC_1p_FD, norm_ET_plots, true, ETrans15_MEC_integral_1p, tET15, "MEC Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1p_FD, "ETrans_15_MEC_Only_1p", hET15_MEC_1p_FD_Dir, "", kRed, true, true, true);

            double ETrans_All_Ang_MEC_integral_2p = hET_All_Ang_MEC_2p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_MEC_2p_FD, norm_ET_plots, true, ETrans_All_Ang_MEC_integral_2p, tET_All_Ang, "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_2p_FD, "ET_All_Ang_MEC_2p", hET_All_Ang_MEC_2p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_MEC_integral_2p = hET15_MEC_2p_FD->Integral();

            histPlotter1D(c1, hET15_MEC_2p_FD, norm_ET_plots, true, ETrans15_MEC_integral_2p, tET15, "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_MEC_Only_2p", hET15_MEC_2p_FD_Dir, "", kRed, true, true, true);
        }

        if (ETrans_RES_plots) {
            double ETrans_All_Ang_RES_integral_1p = hET_All_Ang_RES_1p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_RES_1p_FD, norm_ET_plots, true, ETrans_All_Ang_RES_integral_1p, tET_All_Ang, "RES Only, 1p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1p_FD, "ET_All_Ang_RES_1p", hET_All_Ang_RES_1p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_RES_integral_1p = hET15_RES_1p_FD->Integral();

            histPlotter1D(c1, hET15_RES_1p_FD, norm_ET_plots, true, ETrans15_RES_integral_1p, tET15, "RES Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1p_FD, "ETrans_15_RES_1p", hET15_RES_1p_FD_Dir, "", kGreen, true, true, true);

            double ETrans_All_Ang_RES_integral_2p = hET_All_Ang_RES_2p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_RES_2p_FD, norm_ET_plots, true, ETrans_All_Ang_RES_integral_2p, tET_All_Ang, "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_2p_FD, "ET_All_Ang_RES_2p", hET_All_Ang_RES_2p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_RES_integral_2p = hET15_RES_2p_FD->Integral();

            histPlotter1D(c1, hET15_RES_2p_FD, norm_ET_plots, true, ETrans15_RES_integral_2p, tET15, "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_RES_2p", hET15_RES_2p_FD_Dir, "", kGreen, true, true, true);
        }

        if (ETrans_DIS_plots) {
            double ETrans_All_Ang_DIS_integral_1p = hET_All_Ang_DIS_1p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_DIS_1p_FD, norm_ET_plots, true, ETrans_All_Ang_DIS_integral_1p, tET_All_Ang, "DIS Only, 1p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1p_FD, "ET_All_Ang_DIS_1p", hET_All_Ang_DIS_1p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_DIS_integral_1p = hET15_DIS_1p_FD->Integral();

            histPlotter1D(c1, hET15_DIS_1p_FD, norm_ET_plots, true, ETrans15_DIS_integral_1p, tET15, "DIS Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1p_FD, "ETrans_15_DIS_1p", hET15_DIS_1p_FD_Dir, "", kMagenta, true, true, true);

            double ETrans_All_Ang_DIS_integral_2p = hET_All_Ang_DIS_2p_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_DIS_2p_FD, norm_ET_plots, true, ETrans_All_Ang_DIS_integral_2p, tET_All_Ang, "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_2p_FD, "ET_All_Ang_DIS_2p", hET_All_Ang_DIS_2p_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_DIS_integral_2p = hET15_DIS_2p_FD->Integral();

            histPlotter1D(c1, hET15_DIS_2p_FD, norm_ET_plots, true, ETrans15_DIS_integral_2p, tET15, "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_DIS_2p", hET15_DIS_2p_FD_Dir, "", kMagenta, true, true, true);
        }

//  Energy transfer around 15 deg stack (2p only) ------------------------------------------------------

        if (ETrans_All_Int_plots && ETrans_QEL_plots && ETrans_MEC_plots && ETrans_RES_plots && ETrans_DIS_plots) {
            stackPlotter1D(c1, sET_All_Ang_All_Int_1p_FD, norm_ET_plots, tET_All_Ang, "1p", plots, hET_All_Ang_All_Int_1p_FD, hET_All_Ang_QEL_1p_FD,
                           hET_All_Ang_MEC_1p_FD,
                           hET_All_Ang_RES_1p_FD, hET_All_Ang_DIS_1p_FD, "02_ET_All_Ang_stack", sET_All_Ang_All_Int_1p_FD_Dir, "");

            stackPlotter1D(c1, sET15_All_Int_1p_FD, norm_ET_plots, tET15, "1p", plots, hET15_All_Int_1p_FD, hET15_QEL_1p_FD, hET15_MEC_1p_FD,
                           hET15_RES_1p_FD, hET15_DIS_1p_FD, "02_ET15_stack", sET15_All_Int_1p_FD_Dir, "");

            stackPlotter1D(c1, sET_All_Ang_All_Int_2p_FD, norm_ET_plots, tET_All_Ang, "2p", plots, hET_All_Ang_All_Int_2p_FD, hET_All_Ang_QEL_2p_FD,
                           hET_All_Ang_MEC_2p_FD,
                           hET_All_Ang_RES_2p_FD, hET_All_Ang_DIS_2p_FD, "02_ET_All_Ang_stack", sET_All_Ang_All_Int_2p_FD_Dir, "");

            stackPlotter1D(c1, sET15_All_Int_2p_FD, norm_ET_plots, tET15, "2p", plots, hET15_All_Int_2p_FD, hET15_QEL_2p_FD, hET15_MEC_2p_FD,
                           hET15_RES_2p_FD, hET15_DIS_2p_FD, "02_ET15_stack", sET15_All_Int_2p_FD_Dir, "");
        }

    } else {
        cout << "\n\nEnergy transfer plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Ecal reconstruction histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Ecal reconstruction histograms">
    if (Ecal_plots) {

        cout << "\n\nPlotting Ecal reconstruction histograms...\n\n";

        //<editor-fold desc="Ecal histograms">
        histPlotter1D(c1, hEcal_All_Int_2p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_2p,
                      "Ecal_rec_All_Int", hEcal_All_Int_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_QEL_2p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_2p,
                      "Ecal_rec_QEL_only", hEcal_QEL_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_MEC_2p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_2p,
                      "Ecal_rec_MEC_only", hEcal_MEC_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_RES_2p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "RES Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_2p,
                      "Ecal_rec_RES_only", hEcal_RES_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_DIS_2p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_2p,
                      "Ecal_rec_DIS_only", hEcal_DIS_2p_Dir, "2p", kBlue, true, true, true);

        stackPlotter1D(c1, sEcal_2p, norm_Ecal_plots, "E_{cal} Reconstruction", "2p", plots, hEcal_All_Int_2p, hEcal_QEL_2p, hEcal_MEC_2p, hEcal_RES_2p,
                       hEcal_DIS_2p,
                       "01_Ecal_rec_stack", sEcal_2p_Dir, "");
        //</editor-fold>

        //<editor-fold desc="Ecal vs. dAlpha_T">
        histPlotter2D(c1, hEcal_vs_dAlpha_T_L_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hEcal_vs_dAlpha_T_L_2p_Dir, "02_Ecal_vs_dAlpha_T_L", false);
        histPlotter2D(c1, hEcal_vs_dAlpha_T_tot_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hEcal_vs_dAlpha_T_tot_2p_Dir, "03_Ecal_vs_dAlpha_T_tot", false);
        //</editor-fold>

        //<editor-fold desc="Ecal vs. dP_T">
        histPlotter2D(c1, hEcal_vs_dP_T_L_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hEcal_vs_dP_T_L_2p_Dir, "02_Ecal_vs_dP_T_L", false);
        histPlotter2D(c1, hEcal_vs_dP_T_tot_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hEcal_vs_dP_T_tot_2p_Dir, "03_Ecal_vs_dP_T_tot", false);
        //</editor-fold>

    } else {
        cout << "\n\nEcal plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Transverse variables histogram
// ======================================================================================================================================================================

    //<editor-fold desc="Transverse variables histogram">
    if (TVariables_plots) {
        cout << "\n\nTransverse variables histograms...\n\n";

        /* dP_T plots */
        histPlotter1D(c1, hdP_T_L_2p, norm_TVariables_plots, true, 1., "#deltaP_{T,L} by Leading Proton", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdP_T_2p, "dP_T_L", hdP_T_L_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hdP_T_tot_2p, norm_TVariables_plots, true, 1., "#deltaP_{T,tot} by Momentum Sum", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdP_T_2p, "dP_T_tot", hdP_T_tot_2p_Dir, "2p", kBlue, true, true, true);

        /* dAlpha_T plots */
        histPlotter1D(c1, hdAlpha_T_L_2p, norm_TVariables_plots, true, 1., "#delta#alpha_{T,L} by Leading Proton", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sdAlpha_T_2p, "dAlpha_T_L", hdAlpha_T_L_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hdAlpha_T_tot_2p, norm_TVariables_plots, true, 1., "#delta#alpha_{T,tot} by Momentum Sum", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sdAlpha_T_2p, "dAlpha_T_tot", hdAlpha_T_tot_2p_Dir, "2p", kBlue, true, true, true);

        /* dPhi_T plots */
        histPlotter1D(c1, hdPhi_T_L_2p, norm_TVariables_plots, true, 1., "#delta#phi_{T,L} by Leading Proton", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdPhi_T_2p, "dPhi_T_L", hdPhi_T_L_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hdPhi_T_tot_2p, norm_TVariables_plots, true, 1., "#delta#phi_{T,tot} by Momentum Sum", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdPhi_T_2p, "dPhi_T_tot", hdPhi_T_tot_2p_Dir, "2p", kBlue, true, true, true);

        histPlotter2D(c1, hdP_T_L_vs_dAlpha_T_L_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hdP_T_L_vs_dAlpha_T_L_2p_Dir, "01_dP_T_L_vs_dAlpha_T_L_2p", false);
        histPlotter2D(c1, hdP_T_tot_vs_dAlpha_T_tot_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hdP_T_tot_vs_dAlpha_T_tot_2p_Dir,
                      "02_dP_T_tot_vs_dAlpha_T_tot_2p", false);

    } else {
        cout << "\n\nTransverse variables plots are disabled by user.\n\n";
    }
    //</editor-fold>

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          Saving histogram list and finishing execution                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Saving and finishing execution">

// Saving proton pid cuts to .par file ----------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving proton pid cuts to .par file">
    if (apply_cuts == true &&
        apply_chi2_cuts_1e_cut == false) { // log pid cuts only if all other cuts are enabled //todo: review this. make code preform cuts only in this case
        DSCuts chi2cuts[] = {Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD, Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD, Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        ofstream FittedPIDCuts;
        FittedPIDCuts.open((CutsDirectory + "Fitted_PID_Cuts__" + SampleName + ".par").c_str());
        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "\n";
        FittedPIDCuts << "# Cuts are fitted for - " + SampleName << "\n";
        FittedPIDCuts << "\n";
        FittedPIDCuts << "# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << Chi2_Proton_cuts_FD.FitStdFactor
                      << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            FittedPIDCuts << "pid_cuts_" << chi2cuts[i].GetRegion() << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":"
                          << chi2cuts[i].GetUpperCut() << "\n";
        }

        FittedPIDCuts.close();

        system(("cp " + CutsDirectory + "Fitted_PID_Cuts__" + SampleName + ".par " + plots_path).c_str()); // Copy fitted chi2 cuts file for easy download from ifarm
    }
    //</editor-fold>

// Saving debug files ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving debug files">
    if (PrintEvents == true) { EventPrint.close(); }

    if (debug_plots == true) {
        cout << "\n\nSaving debugging plots...\n\n";

        TString debug_filePath = plots_path + "DebugOutputFile.root";
        clasAna.setdebug_fileName(debug_filePath);
        clasAna.WriteDebugPlots();
    } else {
        cout << "\n\nDebugging plots are disabled...\n\n"; //TODO: disable them in clas12ana?
    }
    //</editor-fold>

// Saving settings to log file ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving settings to log file">
    ofstream myLogFile;
    myLogFile.open(plots_log_save_Directory.c_str());

    myLogFile << "///////////////////////////////////////////////////////////////////////////\n";
    myLogFile << "// Run was with '" << file_name << "' settings mode\n";
    myLogFile << "// Input file was " << LoadedInput << "\n";
    myLogFile << "// Code version was " << Ver << "\n";
    myLogFile << "// Analysis mode was 'Detector simulation'\n";
    myLogFile << "///////////////////////////////////////////////////////////////////////////\n\n";
    myLogFile << "Code ran with the following settings:" << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Input\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "LoadedInput: " << LoadedInput << "\n";
    myLogFile << "filePath: " << filePath << "\n";
    myLogFile << "fileInput: " << fileInput << "\n";
    myLogFile << "plotsInput: " << plotsInput << "\n\n";
    myLogFile << "WorkingDirectory: " << WorkingDirectory << "\n";
    myLogFile << "plots_path: " << plots_path << "\n";
    myLogFile << "SampleName: " << SampleName << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "wider_margin = " << BoolToString(wider_margin) << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Calculation settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "calculate_2p = " << BoolToString(calculate_2p) << "\n";
    myLogFile << "calculate_1n1p = " << BoolToString(calculate_1n1p) << "\n\n";

    myLogFile << "Probe = " << Probe << " (PDG: " << Probe_pdg << ")" << "\n";
    myLogFile << "Target = " << Target_nucleus << " (PDG: " << Target_pdg << ")" << "\n\n";

    myLogFile << "m_e = " << m_e << "\n";
    myLogFile << "m_p = " << m_p << "\n";
    myLogFile << "m_n = " << m_n << "\n";
    myLogFile << "m_pizero = " << m_pizero << "\n";
    myLogFile << "m_piplus = " << m_piplus << "\n";
    myLogFile << "m_piminus = " << m_piminus << "\n";
    myLogFile << "m_Kzero = " << m_Kzero << "\n";
    myLogFile << "m_Kplus = " << m_Kplus << "\n";
    myLogFile << "m_Kminus = " << m_Kminus << "\n\n";

    myLogFile << "beamE = " << beamE << " [GeV]\n";
    myLogFile << "Pv = " << Pv << "\n";
    myLogFile << "Pvx = " << Pvx << "\n";
    myLogFile << "Pvy = " << Pvy << "\n";
    myLogFile << "Pvz = " << Pvz << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot selector\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "Plot_selector_master = " << BoolToString(Plot_selector_master) << "\n";

    myLogFile << "\n-- Cut variable plots -----------------------------------------------------" << "\n";
    myLogFile << "Cut_plots_master = " << BoolToString(Cut_plots_master) << "\n";
    myLogFile << "Nphe_plots = " << BoolToString(Nphe_plots) << "\n";
    myLogFile << "Chi2_plots = " << BoolToString(Chi2_plots) << "\n";
    myLogFile << "Vertex_plots = " << BoolToString(Vertex_plots) << "\n";
    myLogFile << "SF_plots = " << BoolToString(SF_plots) << "\n";
    myLogFile << "fiducial_plots = " << BoolToString(fiducial_plots) << "\n";
    myLogFile << "Momentum_plots = " << BoolToString(Momentum_plots) << "\n";

    myLogFile << "\n-- Beta vs. P plots -------------------------------------------------------" << "\n";
    myLogFile << "Beta_vs_P_plots = " << BoolToString(Beta_vs_P_plots) << "\n";

    myLogFile << "\n-- Angle plots ------------------------------------------------------------" << "\n";
    myLogFile << "Angle_plots_master = " << BoolToString(Angle_plots_master) << "\n";
    myLogFile << "Theta_e_plots = " << BoolToString(Theta_e_plots) << "\n";
    myLogFile << "Phi_e_plots = " << BoolToString(Phi_e_plots) << "\n";

    myLogFile << "\n-- Q2 plots ---------------------------------------------------------------" << "\n";
    myLogFile << "Q2_plots = " << BoolToString(Q2_plots) << "\n";

    myLogFile << "\n-- E_e plots --------------------------------------------------------------" << "\n";
    myLogFile << "E_e_plots = " << BoolToString(E_e_plots) << "\n";

    myLogFile << "\n-- ET plots ---------------------------------------------------------------" << "\n";
    myLogFile << "ETrans_plots_master = " << BoolToString(ETrans_plots_master) << "\n";
    myLogFile << "ETrans_all_plots = " << BoolToString(ETrans_all_plots) << "\n";
    myLogFile << "ETrans_QEL_plots = " << BoolToString(ETrans_QEL_plots) << "\n";
    myLogFile << "ETrans_MEC_plots = " << BoolToString(ETrans_MEC_plots) << "\n";
    myLogFile << "ETrans_RES_plots = " << BoolToString(ETrans_RES_plots) << "\n";
    myLogFile << "ETrans_DIS_plots = " << BoolToString(ETrans_DIS_plots) << "\n";

    myLogFile << "\n-- E_cal plots ------------------------------------------------------------" << "\n";
    myLogFile << "E_cal_plots = " << BoolToString(Ecal_plots) << "\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Normalization settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "normalize_master = " << BoolToString(normalize_master) << "\n\n";

    myLogFile << "norm_Nphe_plots = " << BoolToString(norm_Nphe_plots) << "\n";
    myLogFile << "norm_Chi2_plots = " << BoolToString(norm_Chi2_plots) << "\n";
    myLogFile << "norm_Vertex_plots = " << BoolToString(norm_Vertex_plots) << "\n";
    myLogFile << "norm_SF_plots = " << BoolToString(norm_SF_plots) << "\n";
    myLogFile << "norm_Fiducial_plots = " << BoolToString(norm_Fiducial_plots) << "\n\n";

    myLogFile << "norm_Angle_plots_master = " << BoolToString(norm_Angle_plots_master) << "\n";
    myLogFile << "norm_Q2_plots = " << BoolToString(norm_Q2_plots) << "\n";
    myLogFile << "norm_E_e_plots = " << BoolToString(norm_E_e_plots) << "\n";
    myLogFile << "norm_ET_plots = " << BoolToString(norm_ET_plots) << "\n";
    myLogFile << "norm_Ecal_plots = " << BoolToString(norm_Ecal_plots) << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Delete settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "delete_png_files = " << BoolToString(delete_png_files) << "\n";
    myLogFile << "delete_root_files = " << BoolToString(delete_root_files) << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Cut application settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "apply_cuts = " << BoolToString(apply_cuts) << "\n\n";

    myLogFile << "apply_Nphe_cut = " << BoolToString(apply_Nphe_cut) << "\n";
    myLogFile << "apply_chi2_cuts_1e_cut = " << BoolToString(apply_chi2_cuts_1e_cut) << "\n";
    myLogFile << "apply_Vz_cuts = " << BoolToString(apply_Vz_cuts) << "\n";
    myLogFile << "apply_dVz_cuts = " << BoolToString(apply_dVz_cuts) << "\n";
    myLogFile << "apply_SF_cuts = " << BoolToString(apply_SF_cuts) << "\n";
    myLogFile << "apply_ECAL_fiducial_cuts = " << BoolToString(apply_ECAL_fiducial_cuts) << "\n";
    myLogFile << "apply_DC_fiducial_cut = " << BoolToString(apply_DC_fiducial_cut) << "\n";
    myLogFile << "apply_momentum_cuts_2p = " << BoolToString(apply_momentum_cuts_2p) << "\n";
    myLogFile << "apply_momentum_cuts_1n1p = " << BoolToString(apply_momentum_cuts_1n1p) << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Number of Photo-electrons (Nphe) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "Nphe_lower_cut:" << std::setw(59) << Nphe_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Nphe_upper_cut:" << std::setw(59) << Nphe_cuts_FD.GetUpperCut() << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Chi2 cuts\n";
    myLogFile << "===========================================================================\n\n";

    //<editor-fold desc="Chi2_Electron">
    myLogFile << "\n-- Electrons --------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Electrons in FD (1e cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Electron_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Electron_cuts_FD.GetUpperCut() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Electron_cuts_FD.MeanFromHistogram << "\n\n";

    //<editor-fold desc="Chi2_Proton">
    myLogFile << "\n\n-- Protons ----------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Protons in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Proton_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Proton_cuts_CD.GetUpperCut() / Chi2_Proton_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Proton_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Proton_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Proton_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Proton_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Protons in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Proton_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Proton_cuts_FD.GetUpperCut() / Chi2_Proton_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Proton_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Proton_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Proton_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Proton_cuts_FD.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_Kplus">
    myLogFile << "\n-- K+ ---------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kplus in CD (1e cut) ---------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kplus_cuts_CD.GetUpperCut() / Chi2_Kplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kplus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kplus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kplus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kplus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Kplus in FD (1e cut) ---------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kplus_cuts_FD.GetUpperCut() / Chi2_Kplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kplus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kplus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kplus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kplus_cuts_FD.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_Kminus">
    myLogFile << "\n-- K- ---------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kminus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kminus_cuts_CD.GetUpperCut() / Chi2_Kminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kminus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kminus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kminus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kminus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- Kminus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_Kminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_Kminus_cuts_FD.GetUpperCut() / Chi2_Kminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_Kminus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_Kminus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_Kminus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_Kminus_cuts_FD.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_piplus">
    myLogFile << "\n-- pi+ --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piplus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piplus_cuts_CD.GetUpperCut() / Chi2_piplus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piplus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piplus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piplus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piplus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- piplus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piplus_cuts_FD.GetUpperCut() / Chi2_piplus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piplus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piplus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piplus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piplus_cuts_FD.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_piminus">
    myLogFile << "\n-- pi- --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piminus in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piminus_cuts_CD.GetUpperCut() / Chi2_piminus_cuts_CD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piminus_cuts_CD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piminus_cuts_CD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piminus_cuts_CD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piminus_cuts_CD.GetUpperCut() << "\n\n";

    myLogFile << "\n-- piminus in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2 std factor:" << std::setw(58) << Chi2_piminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit std:" << std::setw(61) << Chi2_piminus_cuts_FD.GetUpperCut() / Chi2_piminus_cuts_FD.GetStdFactor() << "\n";
    myLogFile << "Chi2 fit mean:" << std::setw(60) << Chi2_piminus_cuts_FD.GetMean() << "\n";
    myLogFile << "Chi2 hist mean:" << std::setw(59) << Chi2_piminus_cuts_FD.MeanFromHistogram << "\n";
    myLogFile << "Chi2 lower cut:" << std::setw(59) << Chi2_piminus_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Chi2 upper cut:" << std::setw(59) << Chi2_piminus_cuts_FD.GetUpperCut() << "\n\n";
    //</editor-fold>

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Vertex cuts\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "Vz_cut.GetLowerCut() = " << Vz_cut.GetLowerCut() << "\n";
    myLogFile << "Vz_cut.GetUpperCut() = " << Vz_cut.GetUpperCut() << "\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Sampling Fraction (SF) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "SF_1e_upper_cut = " << clasAna.getEcalSFUpperCut() << "\n";
    myLogFile << "SF_1e_lower_cut = " << clasAna.getEcalSFLowerCut() << "\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "ECAL fiducial cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "fiducial_cut_Lv = " << clasAna.getEcalEdgeCuts() << "\n";
    myLogFile << "fiducial_cut_Lw = " << clasAna.getEcalEdgeCuts() << "\n\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum thresholds (2p)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "P_e_lower_cut_2p (e_momentum_cuts_2p) = " << e_momentum_cuts_2p.GetLowerCut() << "\n";
    myLogFile << "P_e_upper_cut_2p (e_momentum_cuts_2p) = " << e_momentum_cuts_2p.GetUpperCut() << "\n";
    myLogFile << "P_p_lower_cut_2p (p_momentum_cuts_2p) = " << p_momentum_cuts_2p.GetLowerCut() << "\n";
    myLogFile << "P_p_upper_cut_2p (p_momentum_cuts_2p) = " << p_momentum_cuts_2p.GetUpperCut() << "\n";

    myLogFile << "\n===========================================================================\n";
    myLogFile << "dV cuts\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "\n-- dVz cuts (CD & FD) -------------------------------------------------------" << "\n";
    myLogFile << "dVz_cut = " << dVz_cuts.GetUpperCut() << "\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Event counts\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "-- Total counts -----------------------------------------------------------\n";
    myLogFile << "Total #(events):\t\t\t\t\t" << num_of_events << "\n";
    myLogFile << "Total #(events) w/o any e:\t\t\t" << num_of_events_without_any_e << "\n";
    myLogFile << "Total #(events) w/ any e:\t\t\t" << num_of_events_with_any_e << "\n\n";

    myLogFile << "Total #(QEL events):\t\t\t\t" << num_of_QEL_events << "\n";
    myLogFile << "Total #(MEC events):\t\t\t\t" << num_of_MEC_events << "\n";
    myLogFile << "Total #(RES events):\t\t\t\t" << num_of_RES_events << "\n";
    myLogFile << "Total #(DIS events):\t\t\t\t" << num_of_DIS_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS:\t\t\t\t" << num_of_QEL_events + num_of_MEC_events + num_of_RES_events + num_of_DIS_events << "\n\n";

    myLogFile << "-- Events with electrons counts -------------------------------------------\n";
    myLogFile << "#(events) w/ at least 1e:\t\t\t" << num_of_events_with_at_least_1e << "\n";
    myLogFile << "#(events) w/ more then 1e:\t\t\t" << num_of_events_more_then_1e << "\n";
    myLogFile << "#(events) w/ exactly 1e:\t\t\t" << num_of_events_with_exactly_1e << "\n\n";

    myLogFile << "-- 1e2X event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2X:\t\t\t\t" << num_of_events_with_1e2X << "\n\n";

    myLogFile << "#(events) w/ 1e2X QEL in CD:\t\t\t" << num_of_QEL_1e2X_CD_events << "\n";
    myLogFile << "#(events) w/ 1e2X MEC in CD:\t\t\t" << num_of_MEC_1e2X_CD_events << "\n";
    myLogFile << "#(events) w/ 1e2X RES in CD:\t\t\t" << num_of_RES_1e2X_CD_events << "\n";
    myLogFile << "#(events) w/ 1e2X DIS in CD:\t\t\t" << num_of_DIS_1e2X_CD_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (CD):\t\t\t" << num_of_QEL_1e2X_CD_events + num_of_MEC_1e2X_CD_events + num_of_RES_1e2X_CD_events + num_of_DIS_1e2X_CD_events
              << "\n\n";

    myLogFile << "#(events) w/ 1e2X QEL in FD:\t\t\t" << num_of_QEL_1e2X_FD_events << "\n";
    myLogFile << "#(events) w/ 1e2X MEC in FD:\t\t\t" << num_of_MEC_1e2X_FD_events << "\n";
    myLogFile << "#(events) w/ 1e2X RES in FD:\t\t\t" << num_of_RES_1e2X_FD_events << "\n";
    myLogFile << "#(events) w/ 1e2X DIS in FD:\t\t\t" << num_of_DIS_1e2X_FD_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (FD):\t\t\t" << num_of_QEL_1e2X_FD_events + num_of_MEC_1e2X_FD_events + num_of_RES_1e2X_FD_events + num_of_DIS_1e2X_FD_events
              << "\n\n";

    myLogFile << "-- 1enp event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e & any #p:\t\t\t" << num_of_events_with_1enP << "\n\n";

    myLogFile << "-- 1e1p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e1p:\t\t\t\t" << num_of_events_with_1e1p << "\n\n";

    myLogFile << "-- 1e2p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2p:\t\t\t\t" << num_of_events_with_1e2p << "\n\n";

    myLogFile << "#(events) w/ 1e2p QEL in CD:\t\t\t" << num_of_QEL_1e2p_CD_events << "\n";
    myLogFile << "#(events) w/ 1e2p MEC in CD:\t\t\t" << num_of_MEC_1e2p_CD_events << "\n";
    myLogFile << "#(events) w/ 1e2p RES in CD:\t\t\t" << num_of_RES_1e2p_CD_events << "\n";
    myLogFile << "#(events) w/ 1e2p DIS in CD:\t\t\t" << num_of_DIS_1e2p_CD_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (CD):\t\t\t" << num_of_QEL_1e2p_CD_events + num_of_MEC_1e2p_CD_events + num_of_RES_1e2p_CD_events + num_of_DIS_1e2p_CD_events
              << "\n";
    myLogFile << "#(events) 1e2p & e Chi2 cuts CD:\t\t" << num_of_events_1e2p_w_eChi2_cut_only_CD << "\n";
    myLogFile << "#(events) 1e2p & p Chi2 cuts CD:\t\t" << num_of_events_1e2p_w_pChi2_cut_only_CD << "\n";
    myLogFile << "#(events) 1e2p & all Chi2 cuts CD*:\t" << num_of_events_1e2p_w_allChi2_cuts_CD << "\n\n";

    myLogFile << "#(events) w/ 1e2p QEL in FD:\t\t\t" << num_of_QEL_1e2p_FD_events << "\n";
    myLogFile << "#(events) w/ 1e2p MEC in FD:\t\t\t" << num_of_MEC_1e2p_FD_events << "\n";
    myLogFile << "#(events) w/ 1e2p RES in FD:\t\t\t" << num_of_RES_1e2p_FD_events << "\n";
    myLogFile << "#(events) w/ 1e2p DIS in FD:\t\t\t" << num_of_DIS_1e2p_FD_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (FD):\t\t\t" << num_of_QEL_1e2p_FD_events + num_of_MEC_1e2p_FD_events + num_of_RES_1e2p_FD_events + num_of_DIS_1e2p_FD_events
              << "\n";
    myLogFile << "#(events) 1e2p & e Chi2 cuts FD:\t\t" << num_of_events_1e2p_w_eChi2_cut_only_FD << "\n";
    myLogFile << "#(events) 1e2p & p Chi2 cuts FD:\t\t" << num_of_events_1e2p_w_pChi2_cut_only_FD << "\n";
    myLogFile << "#(events) 1e2p & all Chi2 cuts FD*:\t" << num_of_events_1e2p_w_allChi2_cuts_FD << "\n\n";

    myLogFile << "#(events) 1e2p & all Chi2 cuts:\t\t" << num_of_events_1e2p_w_allChi2_cuts << "\n\n\n";

    cout << "-- 2p event counts --------------------------------------------------------\n";
    myLogFile << "#(events) 2p:\t\t\t\t\t" << num_of_events_2p << "\n";
    myLogFile << "#(events) 2p QEL:\t\t\t\t\t" << num_of_QEL_2p_events << "\n";
    myLogFile << "#(events) 2p MEC:\t\t\t\t\t" << num_of_MEC_2p_events << "\n";
    myLogFile << "#(events) 2p RES:\t\t\t\t\t" << num_of_RES_2p_events << "\n";
    myLogFile << "#(events) 2p DIS:\t\t\t\t\t" << num_of_DIS_2p_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (2p):\t\t\t" << num_of_QEL_2p_events + num_of_MEC_2p_events + num_of_RES_2p_events + num_of_DIS_2p_events << "\n\n\n";

    myLogFile << "-- Proton counts ----------------------------------------------------------\n";
    myLogFile << "num_of_events_1e1p_all:\t\t\t\t" << num_of_events_1e1p_all << "\n";
    myLogFile << "num_of_events_1e2p_all:\t\t\t\t" << num_of_events_1e2p_all << "\n";
    myLogFile << "num_of_events_1e2p_all_wo_FDph:\t\t" << num_of_events_1e2p_all_wo_FDph << "\n";
    myLogFile << "num_of_events_2p:\t\t\t\t\t" << num_of_events_2p << "\n\n\n";

    if (apply_chi2_cuts_1e_cut == false) {
        myLogFile << "===========================================================================\n";
        myLogFile << "content of FittedPIDCuts.par file\n";
        myLogFile << "===========================================================================\n\n";

        DSCuts chi2cuts[] = {Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD, Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD, Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        myLogFile << "######################################################################\n";
        myLogFile << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        myLogFile << "######################################################################\n";
        myLogFile << "\n";
        myLogFile << "# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << Chi2_Proton_cuts_FD.FitStdFactor
                  << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            myLogFile << "pid_cuts_" << chi2cuts[i].GetRegion() << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":"
                      << chi2cuts[i].GetUpperCut() << "\n";
        }
    }

    myLogFile.close();
    //</editor-fold>

// Saving histogram list and finishing execution ------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving histogram list and finishing execution">

    //<editor-fold desc="Saving histogram list">
    cout << "\n\nSaving histogram list...";

    TFile *fout = new TFile(TListName, "recreate");
    fout->cd();
    plots->Write();
    fout->Write();
    fout->Close();

    cout << " done.\n\n";
    //</editor-fold>

    cout << "\n\n===========================================================================\n";
    cout << "\t\t\tExecution summary\n";
    cout << "===========================================================================\n\n";

    cout << "---------------------------------------------------------------------------\n";
    cout << "\t\t\tEvent counts\n";
    cout << "---------------------------------------------------------------------------\n\n";

    cout << "-- Total counts -----------------------------------------------------------\n";
    cout << "Total #(events):\t\t\t" << num_of_events << "\n";
    cout << "Total #(events) w/o any e:\t\t" << num_of_events_without_any_e << "\n";
    cout << "Total #(events) w/ any e:\t\t" << num_of_events_with_any_e << "\n\n";

    cout << "Total #(QEL events):\t\t\t" << num_of_QEL_events << "\n";
    cout << "Total #(MEC events):\t\t\t" << num_of_MEC_events << "\n";
    cout << "Total #(RES events):\t\t\t" << num_of_RES_events << "\n";
    cout << "Total #(DIS events):\t\t\t" << num_of_DIS_events << "\n";
    cout << "QEL + MEC + RES + DIS:\t\t\t" << num_of_QEL_events + num_of_MEC_events + num_of_RES_events + num_of_DIS_events << "\n\n";

    cout << "-- Events with electrons counts -------------------------------------------\n";
    cout << "#(events) w/ at least 1e:\t\t" << num_of_events_with_at_least_1e << "\n";
    cout << "#(events) w/ more then 1e:\t\t" << num_of_events_more_then_1e << "\n";
    cout << "#(events) w/ exactly 1e:\t\t" << num_of_events_with_exactly_1e << "\n\n";
    cout << "#(events) w/ exactly 1e (from file):\t" << num_of_events_with_exactly_1e_from_file << "\n\n";

    cout << "-- 1e2X event counts ------------------------------------------------------\n";
    cout << "#(events) w/ 1e2X:\t\t\t" << num_of_events_with_1e2X << "\n\n";

    cout << "#(events) w/ 1e2X QEL in CD:\t\t" << num_of_QEL_1e2X_CD_events << "\n";
    cout << "#(events) w/ 1e2X MEC in CD:\t\t" << num_of_MEC_1e2X_CD_events << "\n";
    cout << "#(events) w/ 1e2X RES in CD:\t\t" << num_of_RES_1e2X_CD_events << "\n";
    cout << "#(events) w/ 1e2X DIS in CD:\t\t" << num_of_DIS_1e2X_CD_events << "\n";
    cout << "QEL + MEC + RES + DIS (CD):\t\t" << num_of_QEL_1e2X_CD_events + num_of_MEC_1e2X_CD_events + num_of_RES_1e2X_CD_events + num_of_DIS_1e2X_CD_events
         << "\n\n";

    cout << "#(events) w/ 1e2X QEL in FD:\t\t" << num_of_QEL_1e2X_FD_events << "\n";
    cout << "#(events) w/ 1e2X MEC in FD:\t\t" << num_of_MEC_1e2X_FD_events << "\n";
    cout << "#(events) w/ 1e2X RES in FD:\t\t" << num_of_RES_1e2X_FD_events << "\n";
    cout << "#(events) w/ 1e2X DIS in FD:\t\t" << num_of_DIS_1e2X_FD_events << "\n";
    cout << "QEL + MEC + RES + DIS (FD):\t\t" << num_of_QEL_1e2X_FD_events + num_of_MEC_1e2X_FD_events + num_of_RES_1e2X_FD_events + num_of_DIS_1e2X_FD_events
         << "\n\n";

    cout << "-- 1enp event counts ------------------------------------------------------\n";
    cout << "#(events) w/ 1e & any #p:\t\t" << num_of_events_with_1enP << "\n\n";

    cout << "-- 1e1p event counts ------------------------------------------------------\n";
    cout << "#(events) w/ 1e1p:\t\t\t" << num_of_events_with_1e1p << "\n\n";

    cout << "-- 1e2p event counts ------------------------------------------------------\n";
    cout << "#(events) w/ 1e2p:\t\t\t" << num_of_events_with_1e2p << "\n\n";

    cout << "#(events) w/ 1e2p QEL in CD:\t\t" << num_of_QEL_1e2p_CD_events << "\n";
    cout << "#(events) w/ 1e2p MEC in CD:\t\t" << num_of_MEC_1e2p_CD_events << "\n";
    cout << "#(events) w/ 1e2p RES in CD:\t\t" << num_of_RES_1e2p_CD_events << "\n";
    cout << "#(events) w/ 1e2p DIS in CD:\t\t" << num_of_DIS_1e2p_CD_events << "\n";
    cout << "QEL + MEC + RES + DIS (CD):\t\t" << num_of_QEL_1e2p_CD_events + num_of_MEC_1e2p_CD_events + num_of_RES_1e2p_CD_events + num_of_DIS_1e2p_CD_events
         << "\n";
    cout << "#(events) 1e2p & e Chi2 cuts CD:\t" << num_of_events_1e2p_w_eChi2_cut_only_CD << "\n";
    cout << "#(events) 1e2p & p Chi2 cuts CD:\t" << num_of_events_1e2p_w_pChi2_cut_only_CD << "\n";
    cout << "#(events) 1e2p & all Chi2 cuts CD*:\t" << num_of_events_1e2p_w_allChi2_cuts_CD << "\n\n";

    cout << "#(events) w/ 1e2p QEL in FD:\t\t" << num_of_QEL_1e2p_FD_events << "\n";
    cout << "#(events) w/ 1e2p MEC in FD:\t\t" << num_of_MEC_1e2p_FD_events << "\n";
    cout << "#(events) w/ 1e2p RES in FD:\t\t" << num_of_RES_1e2p_FD_events << "\n";
    cout << "#(events) w/ 1e2p DIS in FD:\t\t" << num_of_DIS_1e2p_FD_events << "\n";
    cout << "QEL + MEC + RES + DIS (FD):\t\t" << num_of_QEL_1e2p_FD_events + num_of_MEC_1e2p_FD_events + num_of_RES_1e2p_FD_events + num_of_DIS_1e2p_FD_events << "\n";
    cout << "#(events) 1e2p & e Chi2 cuts FD:\t" << num_of_events_1e2p_w_eChi2_cut_only_FD << "\n";
    cout << "#(events) 1e2p & p Chi2 cuts FD:\t" << num_of_events_1e2p_w_pChi2_cut_only_FD << "\n";
    cout << "#(events) 1e2p & all Chi2 cuts FD*:\t" << num_of_events_1e2p_w_allChi2_cuts_FD << "\n\n";

    cout << "#(events) 1e2p & all Chi2 cuts:\t\t" << num_of_events_1e2p_w_allChi2_cuts << "\n\n";

    cout << "-- 2p event counts --------------------------------------------------------\n";
    cout << "#(events) 2p:\t\t\t\t" << num_of_events_2p << "\n";
    cout << "#(events) 2p QEL:\t\t\t" << num_of_QEL_2p_events << "\n";
    cout << "#(events) 2p MEC:\t\t\t" << num_of_MEC_2p_events << "\n";
    cout << "#(events) 2p RES:\t\t\t" << num_of_RES_2p_events << "\n";
    cout << "#(events) 2p DIS:\t\t\t" << num_of_DIS_2p_events << "\n";
    cout << "QEL + MEC + RES + DIS (2p):\t\t" << num_of_QEL_2p_events + num_of_MEC_2p_events + num_of_RES_2p_events + num_of_DIS_2p_events << "\n\n";

    cout << "-- Proton counts ----------------------------------------------------------\n";
    cout << "num_of_events_1e1p_all:\t\t\t" << num_of_events_1e1p_all << "\n";
    cout << "num_of_events_1e2p_all:\t\t\t" << num_of_events_1e2p_all << "\n";
    cout << "num_of_events_1e2p_all_wo_FDph:\t\t" << num_of_events_1e2p_all_wo_FDph << "\n";
    cout << "num_of_events_2p:\t\t\t" << num_of_events_2p << "\n\n";

    cout << "---------------------------------------------------------------------------\n";
    cout << "\t\t\tExecution variables\n";
    cout << "---------------------------------------------------------------------------\n\n";

    cout << "WorkingDirectory:\t" << WorkingDirectory << "\n";
    cout << "plots_path:\t\t" << plots_path << "\n\n";

    cout << "AnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n";
    cout << "AnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n";
    cout << "AnalyseFile:\t\t" << AnalyseFile << "\n\n";

    cout << "SampleName:\t\t" << SampleName << "\n\n";

    cout << "apply_cuts:\t\t'" << BoolToString(apply_cuts) << "'\n";
    cout << "Settings mode:\t\t'" << file_name << "'\n\n";

    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
    cout << "Beam Energy:\t\t" << beamE << " [GeV]\n\n";

    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n\n";
    //</editor-fold>

    //</editor-fold>

}