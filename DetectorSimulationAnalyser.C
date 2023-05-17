
/*

 git pull && clas12root -b -q main.c

 * Copy commend (local -> JLab):
scp -r /home/alon/project/temp/BankHist/Analyser.C -J asportes@ftp.jlab.org:/u/home/asportes/clas12project/

 * Copy commend (JLab -> local):
scp -r asportes@ftp.jlab.org:/u/home/asportes/Analyser/plots -J /home/alon/project/temp/
scp -r asportes@ftp.jlab.org:/w/hallb-scshelf2102/clas12/asportes/recon_c12_6gev.hipo -J /home/alon/project/temp/

 */

/*
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
*/

#include "HipoChain.h"
#include "clas12reader.h"

#include "settings/codeSetup.h"
#include "source/classes/clas12ana/clas12ana.h"
#include "source/classes/DSCuts/DSCuts.h"
#include "source/classes/hPlots/hPlot1D.cpp"
#include "source/classes/hPlots/hPlot2D.cpp"
#include "source/functions/FitFunctions/BetaFit.h"
#include "source/functions/FitFunctions/BetaFitApprax.h"
#include "source/functions/DrawAndSaveAcceptanceCorrectionPlots.h"
#include "source/functions/DrawAndSaveEfficiencyPlots.h"
#include "source/functions/GetPi0MomTh.h"
#include "source/functions/PID_functions/ChargedParticleID.h"
#include "source/functions/PID_functions/GetFDNeutrons.h"
#include "source/functions/PID_functions/GetFDNeutronP.h"
#include "source/functions/PID_functions/GetFDPhotons.h"
#include "source/functions/PID_functions/GetGoodParticles.h"
//#include "source/functions/PID_functions/NeutralParticleID.h"
#include "source/functions/PID_functions/FDNeutralParticle.h"
#include "source/functions/PID_functions/FDNeutralParticleID.h"
#include "source/functions/PID_functions/nParticleID.h"
#include "source/functions/NeutronECAL_Cut_Veto.h"
#include "source/functions/StackPlot3.h"
#include "source/functions/StackPlot4.h"

using namespace std;
using namespace clas12;



// TODO: add sumw2 to all histograms after their definition



void EventAnalyser() {
    cout << "\n\n===========================================================================\n";
    cout << "\t\t\tDetector simulation analyser\n";
    cout << "===========================================================================\n\n";

    string AnalyserVersion = "Beta version";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code settings                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Code settings">

// ======================================================================================================================================================================
// Event selection settings -------------------------------------------------------------------------------------------------------------------------------------------------
// ======================================================================================================================================================================

    //<editor-fold desc="Event selection settings">
    /* Settings to enable/disable specific FS plot calculations (Rec only): */

    bool calculate_truth_level = true;

    bool calculate_1p = true, calculate_1n = true, calculate_1n1p = false, calculate_2p = false;

    bool Rec_wTL_ES = false; // Enforce TL event selection on Rec. plots

    bool Enable_FD_photons = true; // Enforce TL event selection on Rec. plots
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
    bool apply_momentum_cuts_1p = true, apply_momentum_cuts_1n = true, apply_momentum_cuts_2p = true, apply_momentum_cuts_1n1p = true;

    bool apply_neutron_Beta_Fit = true;
    //<editor-fold desc="Custom cuts naming & print out execution variables">

    //<editor-fold desc="Custom cuts naming">
    /* Save plots to custom-named folders, to allow multi-sample runs at once. */

    bool custom_cuts_naming = true;
    string Beta_Fit_Status, FD_photons_Status, Efficiency_Status;

    if (custom_cuts_naming) {
        if (apply_neutron_Beta_Fit) {
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
        } else {
            Efficiency_Status = "Eff1";
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
/*
                if (!apply_neutron_Beta_Fit) {
                    if (Enable_FD_photons) {
                        plots_path =
                                WorkingDirectory + "plots_" + SampleName + "_-02_ALL_CUTS_NoBetaCut_wFDphotons"; // NOTE: not acctually a beta cut - but a neutron mom cut
                        plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-02_ALL_CUTS_NoBetaCut_wFDphotons.txt";
                    } else if (!Enable_FD_photons) {
                        plots_path = WorkingDirectory + "plots_" + SampleName +
                                     "_-02_ALL_CUTS_NoBetaCut_NoFDphotons"; // NOTE: not acctually a beta cut - but a neutron mom cut
                        plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-02_ALL_CUTS_NoBetaCut_NoFDphotons.txt";
                    }
//                    plots_path = WorkingDirectory + "plots_" + SampleName + "_-02_ALL_CUTS_NoBetaCut"; // NOTE: not acctually a beta cut - but a neutron mom cut
//                    plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-02_ALL_CUTS_NoBetaCut.txt";
                } else if (apply_neutron_Beta_Fit) {
                    if (Enable_FD_photons) {
                        plots_path = WorkingDirectory + "plots_" + SampleName + "_-03_ALL_CUTS_WithBetaCut_wFDphotons"; //original
                        plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-03_ALL_CUTS_WithBetaCut_wFDphotons.txt";
                    } else if (!Enable_FD_photons) {
                        plots_path = WorkingDirectory + "plots_" + SampleName +
                                     "_-02_ALL_CUTS_WithBetaCut_NoFDphotons"; // NOTE: not acctually a beta cut - but a neutron mom cut
                        plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-02_ALL_CUTS_WithBetaCut_NoFDphotons.txt";
                    }
//                    plots_path = WorkingDirectory + "plots_" + SampleName + "_-03_ALL_CUTS_WithBetaCut"; //original
//                    plots_log_save_Directory = plots_path + "/" + "Run_log_" + SampleName + "_-03_ALL_CUTS_WithBetaCut.txt";
                }
*/
            }
        }
    } else {
        Beta_Fit_Status, FD_photons_Status, Efficiency_Status = "";
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
    if (!apply_chi2_cuts_1e_cut) { apply_neutron_Beta_Fit = false; }

    if (!apply_cuts) {
        cout << "Cuts are disabled:\n";

        apply_Nphe_cut = apply_chi2_cuts_1e_cut = apply_Vz_cuts = apply_dVz_cuts = false;
        apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_DC_fiducial_cut = false;
        apply_momentum_cuts_1p = apply_momentum_cuts_1n = apply_momentum_cuts_2p = apply_momentum_cuts_1n1p = false;
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
    cout << "apply_momentum_cuts_1n1p:\t" << BoolToString(apply_momentum_cuts_1n1p) << "\n";
    cout << "apply_neutron_Beta_Fit:\t\t" << BoolToString(apply_neutron_Beta_Fit) << "\n\n\n";
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
    DSCuts e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
    DSCuts p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.3, 9999);
    DSCuts pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
    DSCuts ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
    DSCuts n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, 0.3, 9999);
    //</editor-fold>

    // Other cuts -------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron momentum cuts (1n, FD only)">
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

    DSCuts Beta_cut_ABF_FD_n_from_ph, Beta_cut_ABF_FD_n_from_ph_apprax;

    DSCuts Beta_cut_ABF_All_FD_neutrals, Beta_cut_ABF_FD_neutrals_noPDG0;
    DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL;
    DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL_wECIN, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_wECIN;
    DSCuts Beta_cut_ABF_All_FD_neutrals_noPCAL_noECIN_wECOUT, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT;

    /* Nucleon theta cut (1p & 1n, FD) */
    DSCuts Theta_nuc_cut = DSCuts("Theta_nuc", "FD", "", "1p & 1n", 0, -9999, 32.);

    /* Neutron veto cut (1n & 1n1p, FD) */
    DSCuts Neutron_veto_cut = DSCuts("Neutron veto", "FD", "", "1n", 0, 100, 9999);

    /* Ghost tracks handling (2p, CD & FD) */
    DSCuts p1_Theta_p_cuts_2p = DSCuts("Theta_p1", "", "Proton", "2p", 40., -9999, 7.5);
    DSCuts p2_Theta_p_cuts_2p = DSCuts("Theta_p2", "", "Proton", "2p", 40., -9999, 7.5);
    DSCuts phi_p1_p2_diff_cuts_2p = DSCuts("dPhi_p1_p2", "", "Proton", "2p", 0, -9999, 15.);
//    DSCuts p1_Theta_p_cuts_2p = DSCuts("Theta_p1", "", "Proton", "", 40., -9999, 5.);
//    DSCuts p2_Theta_p_cuts_2p = DSCuts("Theta_p2", "", "Proton", "", 40., -9999, 5.);
//    DSCuts phi_p1_p2_diff_cuts_2p = DSCuts("dPhi_p1_p2", "", "Proton", "", 0, -9999, 15.);
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
    bool Beta_plots = true;
//    bool Beta_plots = false;
//    cout << "\n\n\n\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;";
//    cout << "\nbool Beta_plots = false;\n\n\n\n";
    bool Beta_vs_P_plots = true;
//    bool Beta_vs_P_plots = false;
//    cout << "\n\n\n\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;";
//    cout << "\nbool Beta_vs_P_plots = false;\n\n\n\n";

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
    bool Q2_plots = true;
//    bool Q2_plots = false;
//    cout << "\n\n\n\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;";
//    cout << "\nbool Q2_plots = false;\n\n\n\n";

    /* E_e plots */
    bool E_e_plots = true;
//    bool E_e_plots = false;
//    cout << "\n\n\n\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;";
//    cout << "\nbool E_e_plots = false;\n\n\n\n";

    /* ET plots */
    bool ETrans_plots_master = true; // Master ET plots selector
//    bool ETrans_plots_master = false; // Master ET plots selector
//    cout << "\n\n\n\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;";
//    cout << "\nbool ETrans_plots_master = false;\n\n\n\n";
    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

    /* Ecal plots */
    bool Ecal_plots = true;
//    bool Ecal_plots = false;
//    cout << "\n\n\n\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;";
//    cout << "\nbool Ecal_plots = false;\n\n\n\n";

    /* Transverse variables plots */
    bool TVariables_plots = true;
//    bool TVariables_plots = false;
//    cout << "\n\n\n\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;";
//    cout << "\nbool TVariables_plots = false;\n\n\n\n";

    /* ToF plots */
    bool ToF_plots = false;

    /* Efficiency plots */
    bool Efficiency_plots = true;

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

    //<editor-fold desc="Nphe plots (1e cut, FD)">
    hPlot1D hNphe_1e_cut_BC_FD, hNphe_1e_cut_AC_FD;

    if (!apply_cuts) {
        hNphe_1e_cut_BC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC BC", "#Photo-electrons in HTCC - N_{phe} - BC", "N_{phe}",
                                     directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"], "01_Nphe_1e_cut_BC", 0, Nphe_boundary);
        hNphe_1e_cut_AC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC AC", "#Photo-electrons in HTCC - N_{phe} - AC", "N_{phe}",
                                     directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"], "02_Nphe_1e_cut_AC", 0, Nphe_boundary);
    } else {
        hNphe_1e_cut_BC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                     directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"], "01_Nphe_1e_cut", 0, Nphe_boundary);
    }
    //</editor-fold>

    //<editor-fold desc="Nphe plots (1p, FD)">
    hPlot1D hNphe_1p_FD = hPlot1D("1p", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_Directory_map["Nphe_1p_Directory"],
                                  "01_Nphe_1p", 0, Nphe_boundary);
    //</editor-fold>

    //<editor-fold desc="Nphe plots (1n, FD)">
    hPlot1D hNphe_1n_FD = hPlot1D("1n", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_Directory_map["Nphe_1n_Directory"],
                                  "01_Nphe_1n", 0, Nphe_boundary);
    //</editor-fold>

    //<editor-fold desc="Nphe plots (2p, FD)">
    hPlot1D hNphe_2p_FD = hPlot1D("2p", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}", directories.Nphe_Directory_map["Nphe_2p_Directory"],
                                  "01_Nphe_2p", 0, Nphe_boundary);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">

    //<editor-fold desc="Chi2 plots (no #(e) cut)">
    /* Plots of chi2 with no cut on number of electrons - NOT used later! */

    hPlot1D hChi2_Electron_FD = hPlot1D("no #(e) cut", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                        directories.Chi2_Directory_map["Chi2_All_e_Directory"], "01_Electron_chi2", -Chi2_boundary, Chi2_boundary);

    hPlot1D hChi2_Proton_CD = hPlot1D("no #(e) cut", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                      directories.Chi2_Directory_map["Chi2_All_e_Directory"], "02_Proton_chi2", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_FD = hPlot1D("no #(e) cut", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                      directories.Chi2_Directory_map["Chi2_All_e_Directory"], "02_Proton_chi2", -Chi2_boundary, Chi2_boundary);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e cut)">
    /* Plots of chi2 with no cut on number of electrons - used later to apply cuts with my mechanism (NOT Justin's ana) */

    hPlot1D hChi2_Electron_1e_cut_FD = hPlot1D("1e cut", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                               directories.Chi2_Directory_map["Chi2_1e_Directory"], "01_Electron_chi2", -Chi2_boundary, Chi2_boundary);

    hPlot1D hChi2_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                             directories.Chi2_Directory_map["Chi2_1e_Directory"], "02_Proton_chi2", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                             directories.Chi2_Directory_map["Chi2_1e_Directory"], "02_Proton_chi2", -Chi2_boundary, Chi2_boundary);

    hPlot1D hChi2_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}",
                                            directories.Chi2_Directory_map["Chi2_1e_Directory"], "03_Kplus_chi2", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}",
                                            directories.Chi2_Directory_map["Chi2_1e_Directory"], "03_Kplus_chi2", -Chi2_boundary, Chi2_boundary);

    hPlot1D hChi2_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}",
                                             directories.Chi2_Directory_map["Chi2_1e_Directory"], "04_Kminus_chi2", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}",
                                             directories.Chi2_Directory_map["Chi2_1e_Directory"], "04_Kminus_chi2", -Chi2_boundary, Chi2_boundary);

    hPlot1D hChi2_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}",
                                             directories.Chi2_Directory_map["Chi2_1e_Directory"], "05_piplus_chi2", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}",
                                             directories.Chi2_Directory_map["Chi2_1e_Directory"], "05_piplus_chi2", -Chi2_boundary, Chi2_boundary);

    hPlot1D hChi2_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}",
                                              directories.Chi2_Directory_map["Chi2_1e_Directory"], "06_piminus_chi2", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}",
                                              directories.Chi2_Directory_map["Chi2_1e_Directory"], "06_piminus_chi2", -Chi2_boundary, Chi2_boundary);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1p)">
    hPlot1D hChi2_Electron_1p_FD = hPlot1D("1p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_Directory_map["Chi2_1p_Directory"],
                                           "01_Electron_Chi2_1p", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_1p_CD = hPlot1D("1p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_Directory_map["Chi2_1p_Directory"],
                                         "02_Proton_Chi2_1p", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_1p_FD = hPlot1D("1p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_Directory_map["Chi2_1p_Directory"],
                                         "02_Proton_Chi2_1p", -Chi2_boundary, Chi2_boundary);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1n)">
    hPlot1D hChi2_Electron_1n_FD = hPlot1D("1n", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_Directory_map["Chi2_1n_Directory"],
                                           "01_Electron_Chi2_1n", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_1n_CD = hPlot1D("1n", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_Directory_map["Chi2_1n_Directory"],
                                         "02_Proton_Chi2_1n", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_1n_FD = hPlot1D("1n", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_Directory_map["Chi2_1n_Directory"],
                                         "02_Proton_Chi2_1n", -Chi2_boundary, Chi2_boundary);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e2p)">
    hPlot1D hChi2_Electron_1e2p_BC_FD, hChi2_Proton_1e2p_BC_CD, hChi2_Proton_1e2p_BC_FD;
    hPlot1D hChi2_Electron_1e2p_AC_FD, hChi2_Proton_1e2p_AC_CD, hChi2_Proton_1e2p_AC_FD;

    if (!apply_cuts) {
        hChi2_Electron_1e2p_BC_FD = hPlot1D("1e2p", "", "#chi^{2}_{e} BC", "#chi^{2}_{e} histogram - before cuts", "#chi^{2}_{e}",
                                            directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "01_Electron_Chi2_1e2p_BC",
                                            -Chi2_boundary, Chi2_boundary);

        hChi2_Proton_1e2p_BC_CD = hPlot1D("1e2p", "CD", "#chi^{2}_{p} BC", "#chi^{2}_{p} histogram - before cuts", "#chi^{2}_{p}",
                                          directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "02_Proton_Chi2_1e2p_BC",
                                          -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_FD = hPlot1D("1e2p", "FD", "#chi^{2}_{p} BC", "#chi^{2}_{p} histogram - before cuts", "#chi^{2}_{p}",
                                          directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "03_Proton_Chi2_1e2p_BC",
                                          -Chi2_boundary, Chi2_boundary);

        hChi2_Electron_1e2p_AC_FD = hPlot1D("1e2p", "", "#chi^{2}_{e} AC", "#chi^{2}_{e} histogram - after cuts", "#chi^{2}_{e}",
                                            directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "01_Electron_Chi2_1e2p_AC",
                                            -Chi2_boundary, Chi2_boundary);

        hChi2_Proton_1e2p_AC_CD = hPlot1D("1e2p", "CD", "#chi^{2}_{p} AC", "#chi^{2}_{p} histogram - after cuts", "#chi^{2}_{p}",
                                          directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "02_Proton_Chi2_1e2p_AC",
                                          -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_AC_FD = hPlot1D("1e2p", "FD", "#chi^{2}_{p} AC", "#chi^{2}_{p} histogram - after cuts", "#chi^{2}_{p}",
                                          directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "03_Proton_Chi2_1e2p_AC",
                                          -Chi2_boundary, Chi2_boundary);
    } else {
        hChi2_Electron_1e2p_BC_FD = hPlot1D("1e2p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                            directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "01_Electron_Chi2_2p", -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_CD = hPlot1D("1e2p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                          directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "02_Proton_Chi2_2p", -Chi2_boundary, Chi2_boundary);
        hChi2_Proton_1e2p_BC_FD = hPlot1D("1e2p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                          directories.Chi2_Directory_map["Chi2_1e2p_cut_tests_Directory"], "03_Proton_Chi2_2p", -Chi2_boundary, Chi2_boundary);
    }
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (2p)">
    hPlot1D hChi2_Electron_2p_FD = hPlot1D("2p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}", directories.Chi2_Directory_map["Chi2_2p_Directory"],
                                           "01_Electron_Chi2_2p", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_2p_CD = hPlot1D("2p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_Directory_map["Chi2_2p_Directory"],
                                         "02_Proton_Chi2_2p", -Chi2_boundary, Chi2_boundary);
    hPlot1D hChi2_Proton_2p_FD = hPlot1D("2p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}", directories.Chi2_Directory_map["Chi2_2p_Directory"],
                                         "02_Proton_Chi2_2p", -Chi2_boundary, Chi2_boundary);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">

    //<editor-fold desc="Vertex plots (1e cut)">
    hPlot1D hVx_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{x}^{e}", "V_{x}^{e} histogram", "V_{x}^{e} [cm]",
                                             directories.Vertex_Directory_map["Vertex_Electron_1e_Vx_Directory"], "01_Electron_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{y}^{e}", "V_{y}^{e} histogram", "V_{y}^{e} [cm]",
                                             directories.Vertex_Directory_map["Vertex_Electron_1e_Vy_Directory"], "02_Electron_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{z}^{e}", "V_{z}^{e} histogram", "V_{z}^{e} [cm]",
                                             directories.Vertex_Directory_map["Vertex_Electron_1e_Vz_Directory"], "03_Electron_Vz", -Vertex_boundary, Vertex_boundary);

    hPlot1D hVx_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{p}", "V_{x}^{p} histogram", "V_{x}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_Vx_Directory"], "01_Proton_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVx_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{p}", "V_{x}^{p} histogram", "V_{x}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_Vx_Directory"], "01_Proton_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{p}", "V_{y}^{p} histogram", "V_{y}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_Vy_Directory"], "02_Proton_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{p}", "V_{y}^{p} histogram", "V_{y}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_Vy_Directory"], "02_Proton_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{p}", "V_{z}^{p} histogram", "V_{z}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_Vz_Directory"], "03_Proton_Vz", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{p}", "V_{z}^{p} histogram", "V_{z}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_Vz_Directory"], "03_Proton_Vz", -Vertex_boundary, Vertex_boundary);

    hPlot1D hVx_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{K^{+}}", "V_{x}^{K^{+}} histogram", "V_{x}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_Vx_Directory"], "01_Kplus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVx_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{K^{+}}", "V_{x}^{K^{+}} histogram", "V_{x}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_Vx_Directory"], "01_Kplus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{K^{+}}", "V_{y}^{K^{+}} histogram", "V_{y}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_Vy_Directory"], "02_Kplus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{K^{+}}", "V_{y}^{K^{+}} histogram", "V_{y}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_Vy_Directory"], "02_Kplus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{K^{+}}", "V_{z}^{K^{+}} histogram", "V_{z}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_Vz_Directory"], "03_Kplus_Vz", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{K^{+}}", "V_{z}^{K^{+}} histogram", "V_{z}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_Vz_Directory"], "03_Kplus_Vz", -Vertex_boundary, Vertex_boundary);

    hPlot1D hVx_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{K^{-}}", "V_{x}^{K^{-}} histogram", "V_{x}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_Vx_Directory"], "01_Kminus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVx_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{K^{-}}", "V_{x}^{K^{-}} histogram", "V_{x}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_Vx_Directory"], "01_Kminus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{K^{-}}", "V_{y}^{K^{-}} histogram", "V_{y}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_Vy_Directory"], "02_Kminus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{K^{-}}", "V_{y}^{K^{-}} histogram", "V_{y}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_Vy_Directory"], "02_Kminus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{K^{-}}", "V_{z}^{K^{-}} histogram", "V_{z}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_Vz_Directory"], "03_Kminus_Vz", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{K^{-}}", "V_{z}^{K^{-}} histogram", "V_{z}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_Vz_Directory"], "03_Kminus_Vz", -Vertex_boundary, Vertex_boundary);

    hPlot1D hVx_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{#pi^{+}}", "V_{x}^{#pi^{+}} histogram", "V_{x}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_Vx_Directory"], "01_piplus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVx_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{#pi^{+}}", "V_{x}^{#pi^{+}} histogram", "V_{x}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_Vx_Directory"], "01_piplus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{#pi^{+}}", "V_{y}^{#pi^{+}} histogram", "V_{y}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_Vy_Directory"], "02_piplus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{#pi^{+}}", "V_{y}^{#pi^{+}} histogram", "V_{y}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_Vy_Directory"], "02_piplus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{#pi^{+}}", "V_{z}^{#pi^{+}} histogram", "V_{z}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_Vz_Directory"], "03_piplus_Vz", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{#pi^{+}}", "V_{z}^{#pi^{+}} histogram", "V_{z}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_Vz_Directory"], "03_piplus_Vz", -Vertex_boundary, Vertex_boundary);

    hPlot1D hVx_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{#pi^{-}}", "V_{x}^{#pi^{-}} histogram", "V_{x}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_Vx_Directory"], "01_piminus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVx_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{#pi^{-}}", "V_{x}^{#pi^{-}} histogram", "V_{x}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_Vx_Directory"], "01_piminus_Vx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{#pi^{-}}", "V_{y}^{#pi^{-}} histogram", "V_{y}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_Vy_Directory"], "02_piminus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVy_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{#pi^{-}}", "V_{y}^{#pi^{-}} histogram", "V_{y}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_Vy_Directory"], "02_piminus_Vy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{#pi^{-}}", "V_{z}^{#pi^{-}} histogram", "V_{z}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_Vz_Directory"], "03_piminus_Vz", -Vertex_boundary, Vertex_boundary);
    hPlot1D hVz_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{#pi^{-}}", "V_{z}^{#pi^{-}} histogram", "V_{z}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_Vz_Directory"], "03_piminus_Vz", -Vertex_boundary, Vertex_boundary);
    //</editor-fold>

    //<editor-fold desc="dV plots (1p)">
    hPlot1D hdVx_1p = hPlot1D("1p", "", "Vertex corr. dV_{x}", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x}", "dV_{x} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1p_Directory"], "01_dVx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hdVy_1p = hPlot1D("1p", "", "Vertex corr. dV_{y}", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y}", "dV_{y} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1p_Directory"], "02_dVy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hdVz_1p = hPlot1D("1p", "", "Vertex corr. dV_{z}", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z}", "dV_{z} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1p_Directory"], "03_dVz", -Vertex_boundary, Vertex_boundary);
    //</editor-fold>

    //<editor-fold desc="dV plots (1n)">
    hPlot1D hdVx_1n = hPlot1D("1n", "", "Vertex corr. dV_{x}", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x}", "dV_{x} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1n_Directory"], "01_dVx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hdVy_1n = hPlot1D("1n", "", "Vertex corr. dV_{y}", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y}", "dV_{y} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1n_Directory"], "02_dVy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hdVz_1n = hPlot1D("1n", "", "Vertex corr. dV_{z}", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z}", "dV_{z} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1n_Directory"], "03_dVz", -Vertex_boundary, Vertex_boundary);
    //</editor-fold>

    //<editor-fold desc="dV plots (1e2p)">
    hPlot1D hdVx_1e2p_BC, hdVy_1e2p_BC, hdVz_1e2p_BC;
    hPlot1D hdVx_1e2p_AC, hdVy_1e2p_AC, hdVz_1e2p_AC;

    if (!apply_cuts) {
        hdVx_1e2p_BC = hPlot1D("1e2p", "", "Vertex corr. dV_{x} BC", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x} - before cuts", "dV_{x} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"], "01_dVx_BC", -Vertex_boundary, Vertex_boundary);
        hdVy_1e2p_BC = hPlot1D("1e2p", "", "Vertex corr. dV_{y} BC", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y} - before cuts", "dV_{y} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"], "02_dVy_BC", -Vertex_boundary, Vertex_boundary);
        hdVz_1e2p_BC = hPlot1D("1e2p", "", "Vertex corr. dV_{z} BC", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z} - before cuts", "dV_{z} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"], "03_dVz_BC", -Vertex_boundary, Vertex_boundary);

        hdVx_1e2p_AC = hPlot1D("1e2p", "", "Vertex corr. dV_{x} AC", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x} - after cuts", "dV_{x} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_AC_Directory"], "01_dVx_AC", -Vertex_boundary, Vertex_boundary);
        hdVy_1e2p_AC = hPlot1D("1e2p", "", "Vertex corr. dV_{y} AC", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y} - after cuts", "dV_{y} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_AC_Directory"], "02_dVy_AC", -Vertex_boundary, Vertex_boundary);
        hdVz_1e2p_AC = hPlot1D("1e2p", "", "Vertex corr. dV_{z} AC", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z} - after cuts", "dV_{z} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_AC_Directory"], "03_dVz_AC", -Vertex_boundary, Vertex_boundary);
    } else {
        hdVx_1e2p_BC = hPlot1D("1e2p", "", "Vertex corr. dV_{x}", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x}", "dV_{x} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"],
                               "01_dVx", -Vertex_boundary, Vertex_boundary);
        hdVy_1e2p_BC = hPlot1D("1e2p", "", "Vertex corr. dV_{y}", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y}", "dV_{y} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"],
                               "02_dVy", -Vertex_boundary, Vertex_boundary);
        hdVz_1e2p_BC = hPlot1D("1e2p", "", "Vertex corr. dV_{z}", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z}", "dV_{z} [cm]",
                               directories.Vertex_Directory_map["Vertex_1e2p_dV_BC_Directory"],
                               "03_dVz", -Vertex_boundary, Vertex_boundary);
    }
    //</editor-fold>

    //<editor-fold desc="dV plots (2p)">
    hPlot1D hdVx_2p = hPlot1D("2p", "", "Vertex corr. dV_{x}", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x}", "dV_{x} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_2p_Directory"], "01_dVx", -Vertex_boundary, Vertex_boundary);
    hPlot1D hdVy_2p = hPlot1D("2p", "", "Vertex corr. dV_{y}", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y}", "dV_{y} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_2p_Directory"], "02_dVy", -Vertex_boundary, Vertex_boundary);
    hPlot1D hdVz_2p = hPlot1D("2p", "", "Vertex corr. dV_{z}", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z}", "dV_{z} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_2p_Directory"], "03_dVz", -Vertex_boundary, Vertex_boundary);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Sampling Fraction (SF) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Sampling Fraction (SF) histograms (FD)">

    //<editor-fold desc="SF plots (1e cut, FD)">
    hPlot1D hSF_1e_cut_BC_FD, hSF_1e_cut_AC_FD;

    if (!apply_cuts) {
        hSF_1e_cut_BC_FD = hPlot1D("1e cut", "FD", "SF BC", "Sampling fraction f - before cuts", "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_Directory_map["SF_plots_1e_cut_BC_Directory"], "01_SF_1e_cut_before_SF_cuts", SF_lboundary, SF_uboundary);
        hSF_1e_cut_AC_FD = hPlot1D("1e cut", "FD", "SF AC", "Sampling fraction f - after cuts", "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_Directory_map["SF_plots_1e_cut_AC_Directory"], "02_SF_1e_cut_after_SF_cuts", SF_lboundary, SF_uboundary);
    } else {
        hSF_1e_cut_BC_FD = hPlot1D("1e cut", "FD", "SF", "Sampling fraction f", "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_Directory_map["SF_plots_1e_cut_BC_Directory"], "01_SF_1e_cut", SF_lboundary, SF_uboundary);
    }
    //</editor-fold>

    //<editor-fold desc="SF plots (1p, FD)">
    hPlot1D hSF_1p_FD = hPlot1D("1p", "FD", "SF", "Sampling Fraction f", "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                directories.SF_Directory_map["SF_plots_1p_cuts_Directory"], "01_SF_1p", SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="SF plots (1n, FD)">
    hPlot1D hSF_1n_FD = hPlot1D("1n", "FD", "SF", "Sampling Fraction f", "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                directories.SF_Directory_map["SF_plots_1n_cuts_Directory"], "01_SF_1n", SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="SF plots (2p, FD)">
    hPlot1D hSF_2p_FD = hPlot1D("2p", "FD", "SF", "Sampling Fraction f", "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                directories.SF_Directory_map["SF_plots_2p_cuts_Directory"], "01_SF_2p", SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1e cut, FD)">
    hPlot2D hSF_VS_P_e_1e_cut_BC_FD, hSF_VS_P_e_1e_cut_AC_FD;

    if (!apply_cuts) {
        hSF_VS_P_e_1e_cut_BC_FD = hPlot2D("1e cut", "FD", "SF vs. P_{e} BC", "Sampling fraction f vs. P_{e} - before cuts", "P_{e} [GeV/c]",
                                          "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_plots_1e_cut_BC_Directory"],
                                          "01_SF_VS_P_e_1e_cut_BC_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary);
        hSF_VS_P_e_1e_cut_AC_FD = hPlot2D("1e cut", "FD", "SF vs. P_{e} AC", "Sampling fraction f vs. P_{e} - after cuts", "P_{e} [GeV/c]",
                                          "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_plots_1e_cut_AC_Directory"],
                                          "01_SF_VS_P_e_1e_cut_AC_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary);
    } else {
        hSF_VS_P_e_1e_cut_BC_FD = hPlot2D("1e cut", "FD", "SF vs. P_{e}", "Sampling fraction f vs. P_{e}", "P_{e} [GeV/c]",
                                          "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_plots_1e_cut_BC_Directory"],
                                          "01_SF_VS_P_e_1e_cut_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary);
    }
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1p, FD)">
    hPlot2D hSF_VS_P_e_1p_FD = hPlot2D("1p", "FD", "SF vs. P_{e}", "Sampling fraction f vs. P_{e}", "P_{e} [GeV/c]",
                                       "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_plots_1p_cuts_Directory"],
                                       "01_SF_VS_P_e_1p_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1n, FD)">
    hPlot2D hSF_VS_P_e_1n_FD = hPlot2D("1n", "FD", "SF vs. P_{e}", "Sampling fraction f vs. P_{e}", "P_{e} [GeV/c]",
                                       "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_plots_1n_cuts_Directory"],
                                       "01_SF_VS_P_e_1n_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (2p, FD)">
    hPlot2D hSF_VS_P_e_2p_FD = hPlot2D("2p", "FD", "SF vs. P_{e}", "Sampling fraction f vs. P_{e}", "P_{e} [GeV/c]",
                                       "f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_plots_2p_cuts_Directory"],
                                       "01_SF_VS_P_e_2p_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ECAL edge histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="ECAL edge histograms (FD only)">

    //<editor-fold desc="ECAL coordinates vs. SF plots (1e cut, FD)">
    hPlot2D hVcal_VS_EoP_1e_cut_BC_PCAL, hVcal_VS_EoP_1e_cut_AC_PCAL, hWcal_VS_EoP_1e_cut_BC_PCAL, hWcal_VS_EoP_1e_cut_AC_PCAL;

    if (!apply_cuts) {
        hVcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Vcal vs. SF BC", "ECAL V coordinate vs. SF - before cuts", "ECAL V coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"],
                                              "01_Vcal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_lboundary, SF_uboundary);
        hVcal_VS_EoP_1e_cut_AC_PCAL = hPlot2D("1e cut", "PCAL", "Vcal vs. SF AC", "ECAL V coordinate vs. SF - after cuts", "ECAL V coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1e_AC_PCAL_Directory"],
                                              "01_Vcal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_lboundary, SF_uboundary);

        hWcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Wcal vs. SF BC", "ECAL W coordinate vs. SF - before cuts", "ECAL W coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"],
                                              "02_Wcal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_lboundary, SF_uboundary);
        hWcal_VS_EoP_1e_cut_AC_PCAL = hPlot2D("1e cut", "PCAL", "Wcal vs. SF AC", "ECAL W coordinate vs. SF - after cuts", "ECAL W coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1e_AC_PCAL_Directory"],
                                              "02_Wcal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_lboundary, SF_uboundary);
    } else {
        hVcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"],
                                              "01_Vcal_VS_EoP_PCAL_1e_cut", 0, 50, SF_lboundary, SF_uboundary);
        hWcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1e_BC_PCAL_Directory"],
                                              "02_Wcal_VS_EoP_PCAL_1e_cut", 0, 50, SF_lboundary, SF_uboundary);
    }
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (1p, FD)">
    hPlot2D hVcal_VS_EoP_1p_PCAL = hPlot2D("1p", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1p_cuts_histograms_PCAL_Directory"],
                                           "01_Vcal_VS_EoP_PCAL_1p", 0, 50, SF_lboundary, SF_uboundary);
    hPlot2D hWcal_VS_EoP_1p_PCAL = hPlot2D("1p", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1p_cuts_histograms_PCAL_Directory"],
                                           "02_Wcal_VS_EoP_PCAL_1p", 0, 50, SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (1n, FD)">
    hPlot2D hVcal_VS_EoP_1n_PCAL = hPlot2D("1n", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1n_cuts_histograms_PCAL_Directory"],
                                           "01_Vcal_VS_EoP_PCAL_1n", 0, 50, SF_lboundary, SF_uboundary);
    hPlot2D hWcal_VS_EoP_1n_PCAL = hPlot2D("1n", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_1n_cuts_histograms_PCAL_Directory"],
                                           "02_Wcal_VS_EoP_PCAL_1n", 0, 50, SF_lboundary, SF_uboundary);
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (2p, FD)">
    hPlot2D hVcal_VS_EoP_2p_PCAL = hPlot2D("2p", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_2p_cuts_histograms_PCAL_Directory"],
                                           "01_Vcal_VS_EoP_PCAL_2p", 0, 50, SF_lboundary, SF_uboundary);
    hPlot2D hWcal_VS_EoP_2p_PCAL = hPlot2D("2p", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["fiducial_plots_2p_cuts_histograms_PCAL_Directory"],
                                           "02_Wcal_VS_EoP_PCAL_2p", 0, 50, SF_lboundary, SF_uboundary);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum threshold histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum threshold histograms">
    /* Here are the definitions for the Momentum threshold plots. These are used to check that momentum cuts were applied in 1e_cut and 2p plots. */

    //<editor-fold desc="Momentum threshold plots (1e cut)">
    hPlot1D hP_e_1e_cut_FD = hPlot1D("1e cut", "", "Electron momentum", "Electron momentum P_{e}", "P_{e} [GeV/c]",
                                     directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "01_P_e_1e_cut_FD", 0, beamE * 1.1);
    hPlot1D hP_p_1e_cut_CD = hPlot1D("1e cut", "CD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]",
                                     directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "02_P_p_1e_cut_CD", 0, beamE * 1.1);
    hPlot1D hP_p_1e_cut_FD = hPlot1D("1e cut", "FD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]",
                                     directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "02_P_p_1e_cut_FD", 0, beamE * 1.1);
    hPlot1D hP_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "03_P_piplus_1e_cut_CD", 0, beamE * 1.1);
    hPlot1D hP_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "03_P_piplus_1e_cut_FD", 0, beamE * 1.1);
    hPlot1D hP_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "04_P_piminus_1e_cut_CD", 0, beamE * 1.1);
    hPlot1D hP_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1e_cut_Directory"], "04_P_piminus_1e_cut_FD", 0, beamE * 1.1);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1p)">
    hPlot1D hP_e_APID_1p_FD = hPlot1D("1p", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "01a_P_e_APID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_e_BPID_1p_FD = hPlot1D("1p", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "01b_P_e_BPID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_p_APID_1p_CD = hPlot1D("1p", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "02a_P_p_APID_1p_CD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BPID_1p_CD = hPlot1D("1p", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "02b_P_p_BPID_1p_CD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_APID_1p_FD = hPlot1D("1p", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "03c_P_p_APID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BPID_1p_FD = hPlot1D("1p", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "03d_P_p_BPID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_piplus_APID_1p_CD = hPlot1D("1p", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "04a_P_piplus_APID_1p_CD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piplus_BPID_1p_CD = hPlot1D("1p", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "04b_P_piplus_BPID_1p_CD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piplus_APID_1p_FD = hPlot1D("1p", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "04c_P_piplus_APID_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piplus_BPID_1p_FD = hPlot1D("1p", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "04d_P_piplus_BPID_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_piminus_APID_1p_CD = hPlot1D("1p", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "05a_P_piminus_APID_1p_CD",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piminus_BPID_1p_CD = hPlot1D("1p", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "05b_P_piminus_BPID_1p_CD",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piminus_APID_1p_FD = hPlot1D("1p", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "05c_P_piminus_APID_1p_FD",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piminus_BPID_1p_FD = hPlot1D("1p", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "05d_P_piminus_BPID_1p_FD",
                                            Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_ph_APID_1p_FD = hPlot1D("1p", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "06c_P_ph_APID_1p_FD",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BPID_1p_FD = hPlot1D("1p", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "06d_P_ph_BPID_1p_FD",
                                       Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_n_APID_1p_FD = hPlot1D("1p", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "07c_P_n_APID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_BPID_1p_FD = hPlot1D("1p", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1p_Directory"], "07d_P_n_BPID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1n)">
    hPlot1D hP_e_APID_1n_FD = hPlot1D("1n", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "01a_P_e_APID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_e_BPID_1n_FD = hPlot1D("1n", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "01b_P_e_BPID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_n_APID_1n_FD = hPlot1D("1n", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "02a_P_n_APID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_BPID_1n_FD = hPlot1D("1n", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "02b_P_n_BPID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_APID_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum APID - ZOOMOUT", "Neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]",
                                              directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "02c_P_n_APID_1n_ZOOMOUT_FD",
                                              Momentum_lboundary, 20.);
    hPlot1D hP_n_BPID_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum BPID - ZOOMOUT", "Neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]",
                                              directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "02d_P_n_BPID_1n_ZOOMOUT_FD",
                                              Momentum_lboundary, 20.);

    hPlot1D hP_p_APID_1n_CD = hPlot1D("1n", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "03a_P_p_APID_1n_CD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BPID_1n_CD = hPlot1D("1n", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "03b_P_p_BPID_1n_CD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_APID_1n_FD = hPlot1D("1n", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "03c_P_p_APID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BPID_1n_FD = hPlot1D("1n", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "03d_P_p_BPID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_piplus_APID_1n_CD = hPlot1D("1n", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "04a_P_piplus_APID_1n_CD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piplus_BPID_1n_CD = hPlot1D("1n", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "04b_P_piplus_BPID_1n_CD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piplus_APID_1n_FD = hPlot1D("1n", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "04c_P_piplus_APID_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piplus_BPID_1n_FD = hPlot1D("1n", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "04d_P_piplus_BPID_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_piminus_APID_1n_CD = hPlot1D("1n", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "05a_P_piminus_APID_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piminus_BPID_1n_CD = hPlot1D("1n", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "05b_P_piminus_BPID_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piminus_APID_1n_FD = hPlot1D("1n", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "05c_P_piminus_APID_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_piminus_BPID_1n_FD = hPlot1D("1n", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "05d_P_piminus_BPID_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_n_VN_BPID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from v. neutrons) BPID", "Neutron Momentum (from v. neutrons) BPID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "06a_P_n_VNeutrons_BPID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_VN_APID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from v. neutrons) APID", "Neutron Momentum (from v. neutrons) APID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "06b_P_n_VNeutrons_APID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_Ph_BPID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from 'Photons') BPID", "Neutron Momentum (from 'Photons') BPID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "07a_P_n_From_Photons_BPID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_Ph_APID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from 'Photons') APID", "Neutron Momentum (from 'Photons') APID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "07b_P_n_From_Photons_APID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_ph_APID_1n_FD = hPlot1D("1n", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "08a_P_ph_APID_1n_FD",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BPID_1n_FD = hPlot1D("1n", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_th_1n_Directory"], "08b_P_ph_BPID_1n_FD",
                                       Momentum_lboundary, Momentum_uboundary);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1e2p)">
    hPlot1D hP_e_1e2p_BC_FD = hPlot1D("1e2p", "", "Electron momentum BC", "Electron momentum P_{e} - before cuts", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1e2p_Directory"], "01_P_e_1e2p_BC_FD", 0, beamE * 1.1);
    hPlot1D hP_e_1e2p_AC_FD = hPlot1D("1e2p", "", "Electron momentum AC", "Electron momentum P_{e} - after cuts", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1e2p_Directory"], "01_P_e_1e2p_AC_FD", 0, beamE * 1.1);
    hPlot1D hP_p_1e2p_BC_CD = hPlot1D("1e2p", "CD", "Proton momentum BC", "Proton momentum P_{p} - before cuts", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1e2p_Directory"], "02_P_p_1e2p_BC_CD", 0, beamE * 1.1);
    hPlot1D hP_p_1e2p_BC_FD = hPlot1D("1e2p", "FD", "Proton momentum BC", "Proton momentum P_{p} - before cuts", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1e2p_Directory"], "03_P_p_1e2p_BC_FD", 0, beamE * 1.1);
    hPlot1D hP_p_1e2p_AC_CD = hPlot1D("1e2p", "CD", "Proton momentum AC", "Proton momentum P_{p} - after cuts", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1e2p_Directory"], "02_P_p_1e2p_AC_CD", 0, beamE * 1.1);
    hPlot1D hP_p_1e2p_AC_FD = hPlot1D("1e2p", "FD", "Proton momentum AC", "Proton momentum P_{p} - after cuts", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_1e2p_Directory"], "03_P_p_1e2p_AC_FD", 0, beamE * 1.1);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (2p)">

    //<editor-fold desc="Momentum threshold plots (2p)">
    hPlot1D hP_e_2p_FD = hPlot1D("2p", "", "Electron momentum", "Electron momentum P_{e}", "P_{e} [GeV/c]",
                                 directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "01_P_e_2p_FD", 0, beamE * 1.1);
    hPlot1D hP_e_BC_2p_FD = hPlot1D("2p", "", "Electron momentum BC", "Electron momentum P_{e} - before cuts", "P_{e} [GeV/c]",
                                    directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "01_P_e_BC_2p_FD", 0, beamE * 1.1);

    hPlot1D hP_p_2p_CD = hPlot1D("2p", "CD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]",
                                 directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "02_P_p_2p_CD", 0, beamE * 1.1);
    hPlot1D hP_p_BC_2p_CD = hPlot1D("2p", "CD", "Proton momentum BC", "Proton momentum P_{p} - before cuts", "P_{p} [GeV/c]",
                                    directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "02_P_p_BC_2p_CD", 0, beamE * 1.1);

    hPlot1D hP_p_2p_FD = hPlot1D("2p", "FD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]",
                                 directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "03_P_p_2p_FD", 0, beamE * 1.1);
    hPlot1D hP_p_BC_2p_FD = hPlot1D("2p", "FD", "Proton momentum BC", "Proton momentum P_{p} - before cuts", "P_{p} [GeV/c]",
                                    directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "03_P_p_BC_2p_FD", 0, beamE * 1.1);

    hPlot1D hP_pip_BC_2p_CD = hPlot1D("2p", "CD", "#pi^{+} momentum BC", "#pi^{+} momentum P_{#pi^{+}} - before cuts", "P_{#pi^{+}} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "04_P_piplus_BC_2p_CD", 0, beamE * 1.1);
    hPlot1D hP_pip_BC_2p_FD = hPlot1D("2p", "FD", "#pi^{+} momentum BC", "#pi^{+} momentum P_{#pi^{+}} - before cuts", "P_{#pi^{+}} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "04_P_piplus_BC_2p_FD", 0, beamE * 1.1);

    hPlot1D hP_pim_BC_2p_CD = hPlot1D("2p", "CD", "#pi^{-} momentum BC", "#pi^{-} momentum P_{#pi^{-}} - before cuts", "P_{#pi^{-}} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "05_P_piminus_BC_2p_CD", 0, beamE * 1.1);
    hPlot1D hP_pim_BC_2p_FD = hPlot1D("2p", "FD", "#pi^{-} momentum BC", "#pi^{-} momentum P_{#pi^{-}} - before cuts", "P_{#pi^{-}} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "05_P_piminus_BC_2p_FD", 0, beamE * 1.1);

    hPlot1D hP_n_BC_2p_CD = hPlot1D("2p", "CD", "Neutron momentum BC", "Neutron momentum P_{n} - before cuts", "P_{n} [GeV/c]",
                                    directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "06_P_n_BC_2p_CD", 0, beamE * 1.1);
    hPlot1D hP_n_BC_2p_FD = hPlot1D("2p", "FD", "Neutron momentum BC", "Neutron momentum P_{n} - before cuts", "P_{n} [GeV/c]",
                                    directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "06_P_n_BC_2p_FD", 0, beamE * 1.1);
    //</editor-fold>

    //<editor-fold desc="Leading (P_1) and recoil (P_2) Momentum threshold plots (2p)">
    hPlot1D hP_p_1_2p = hPlot1D("2p", "", "Leading proton P_{1}", "Leading proton momentum P_{1}", "P_{1} [GeV/c]",
                                directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "07_P_p_1_2p", 0, beamE * 1.1);
    hPlot1D hP_p_2_2p = hPlot1D("2p", "", "Recoil proton P_{2}", "Recoil proton momentum P_{2}", "P_{2} [GeV/c]",
                                directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "07_P_p_2_2p", 0, beamE * 1.1);
    //</editor-fold>

    //<editor-fold desc="P1 vs P2 (2p, CD & FD)">
    hPlot2D hP_p_1_vs_P_p_2_2p = hPlot2D("2p", "", "P_{1} vs. P_{2}", "P_{1} vs. P_{2}", "P_{1} [GeV/c]", "P_{2} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_th_2p_Directory"], "08_P_p_1_vs_P_p_2",
                                         0, beamE * 1.1, 0, beamE * 1.1);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Beta histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Beta histograms">

    //<editor-fold desc="Beta plots (1n)">

    //<editor-fold desc="Beta of neutrons from 'photons' (1n, FD)">

    //<editor-fold desc="Beta of neutrons from 'photons' (1n, FD)">
    hPlot1D hBeta_n_from_ph_01_1n_FD = hPlot1D("1n", "FD", "#beta of n from '#gamma'", "Neutron #beta from 'photons'", "#beta",
                                               directories.Beta_Directory_map["Neutron_beta_from_ph_01_1n_Directory"], "01_Beta_Neutron_from_photons_1n",
                                               0.98, Beta_dist_uboundary, 65);
    hPlot1D hBeta_n_from_ph_01_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of n from '#gamma' - ZOOMOUT", "Neutron #beta from 'photons' - ZOOMOUT", "#beta",
                                                       directories.Beta_Directory_map["Neutron_beta_from_ph_01_1n_Directory"], "01_Beta_Neutron_from_photons_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_n_from_ph_02_1n_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL", "Neutron #beta from 'photons' w/o PCAL hit", "#beta",
                                               directories.Beta_Directory_map["Neutron_beta_from_ph_02_1n_Directory"], "01_Beta_Neutron_from_photons_noPCAL_1n",
                                               Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_n_from_ph_02_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL - ZOOMOUT",
                                                       "Neutron #beta from 'photons' w/o PCAL hit - ZOOMOUT",
                                                       "#beta", directories.Beta_Directory_map["Neutron_beta_from_ph_02_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_noPCAL_1n_ZOOMOUT", Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_n_from_ph_03_1n_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & ECIN", "Neutron #beta from 'photons' w/o PCAL & w/ECIN", "#beta",
                                               directories.Beta_Directory_map["Neutron_beta_from_ph_03_1n_Directory"], "01_Beta_Neutron_from_photons_noPCAL_yesECIN_1n",
                                               Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_n_from_ph_03_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & ECIN - ZOOMOUT",
                                                       "Neutron #beta from 'photons' w/o PCAL & w/ECIN - ZOOMOUT", "#beta",
                                                       directories.Beta_Directory_map["Neutron_beta_from_ph_03_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_noPCAL_yesECIN_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_n_from_ph_04_1n_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & !ECIN & ECOUT", "Neutron #beta from 'photons' w/o PCAL/ECIN & w/ECOUT",
                                               "#beta", directories.Beta_Directory_map["Neutron_beta_from_ph_04_1n_Directory"],
                                               "01_Beta_Neutron_from_photons_noPCAL_noECIN_yesECOUT_1n", Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_n_from_ph_04_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & !ECIN & ECOUT - ZOOMOUT",
                                                       "Neutron #beta from 'photons' w/o PCAL/ECIN & w/ECOUT - ZOOMOUT", "#beta",
                                                       directories.Beta_Directory_map["Neutron_beta_from_ph_04_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_noPCAL_noECIN_yesECOUT_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);
    //</editor-fold>

    //<editor-fold desc="Beta of neutrons from 'photons' for different variables (1n, FD)">
    hPlot2D hBeta_nfph_vs_numOfFDph_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. #(FD photons)", "#beta_{n} (from 'photons') vs. #(FD photons)",
                                                    "#(FD photons)", "#beta_{n}",
                                                    directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "01_Beta_nfph_vs_numOfFDph_1n_FD",
                                                    0, 10, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_Theta_n_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. #theta_{n}", "#beta_{n} (from 'photons') vs. #theta_{n}",
                                                  "#theta_{n} [Deg]", "#beta_{n}",
                                                  directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "02_Beta_nfph_vs_Theta_n_1n_FD",
                                                  0, 50, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_Phi_n_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. #phi_{n}", "#beta_{n} (from 'photons') vs. #phi_{n}", "#phi_{n} [Deg]",
                                                "#beta_{n}",
                                                directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "03_Beta_nfph_vs_Phi_n_1n_FD",
                                                -180, 180, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_SF_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. SF", "#beta_{n} (from 'photons') vs. Sampling fraction f",
                                             "f = (E_{n,PCAL} + E_{n,IN} + E_{n,OUT})/P_{n}", "#beta_{n}",
                                             directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "04_Beta_nfph_vs_SF_1n_FD",
                                             SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_SF_PCAL_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. SF_PCAL", "#beta_{n} (from 'photons') vs. Sampling fraction f_PCAL",
                                                  "f_PCAL = E_{n,PCAL}/P_{n}", "#beta_{n}",
                                                  directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "05_Beta_nfph_vs_SF_PCAL_1n_FD",
                                                  SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_E_PCAL_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. E_{PCAL}", "#beta_{n} (from 'photons') vs. E_{PCAL}",
                                                 "E_{n,PCAL} [GeV]", "#beta_{n}",
                                                 directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "06_Beta_nfph_vs_E_PCAL_1n_FD",
                                                 SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_SF_ECIN_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. SF_ECIN", "#beta_{n} (from 'photons') vs. Sampling fraction f_ECIN",
                                                  "f_ECIN = E_{n,ECIN}/P_{n}", "#beta_{n}",
                                                  directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "07_Beta_nfph_vs_SF_ECIN_1n_FD",
                                                  SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_E_ECIN_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. E_{ECIN}", "#beta_{n} (from 'photons') vs. E_{ECIN}",
                                                 "E_{n,ECIN} [GeV]", "#beta_{n}",
                                                 directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "08_Beta_nfph_vs_E_ECIN_1n_FD",
                                                 SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_SF_ECOUT_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. SF_ECOUT", "#beta_{n} (from 'photons') vs. Sampling fraction f_ECOUT",
                                                   "f_ECOUT = E_{n,ECOUT}/P_{n}", "#beta_{n}",
                                                   directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "09_Beta_nfph_vs_SF_ECOUT_1n_FD",
                                                   SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_E_ECOUT_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. E_{ECOUT}", "#beta_{n} (from 'photons') vs. E_{ECOUT}",
                                                  "E_{n,ECOUT} [GeV]", "#beta_{n}",
                                                  directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "10_Beta_nfph_vs_E_ECOUT_1n_FD",
                                                  SF_lboundary, SF_uboundary, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_VcalECIN_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. ECIN V coordinate", "#beta_{n} (from 'photons') vs. ECIN V coordinate",
                                                   "ECIN V coordinate [cm]", "#beta_{n}",
                                                   directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "11_Beta_nfph_vs_VcalECIN_1n_FD",
                                                   0, 50, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_WcalECIN_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. ECIN W coordinate", "#beta_{n} (from 'photons') vs. ECIN W coordinate",
                                                   "ECIN W coordinate [cm]", "#beta_{n}",
                                                   directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "12_Beta_nfph_vs_WcalECIN_1n_FD",
                                                   0, 50, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_VcalECOUT_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. ECOUT V coordinate", "#beta_{n} (from 'photons') vs. ECOUT V coordinate",
                                                    "ECOUT V coordinate [cm]", "#beta_{n}",
                                                    directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "13_Beta_nfph_vs_VcalECOUT_1n_FD",
                                                    0, 50, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_WcalECOUT_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. ECOUT W coordinate", "#beta_{n} (from 'photons') vs. ECOUT W coordinate",
                                                    "ECOUT W coordinate [cm]", "#beta_{n}",
                                                    directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "14_Beta_nfph_vs_WcalECOUT_1n_FD",
                                                    0, 50, 0.9, 1.1);

    hPlot2D hBeta_nfph_vs_P_e_1n_FD = hPlot2D("1n", "FD", "#beta_{n} (from 'photons') vs. P_{e}", "#beta_{n} (from 'photons') vs. P_{e}",
                                              " P_{e} [GeV/c]", "#beta_{n}",
                                              directories.Beta_Directory_map["Beta_vs_other_variables_1n_Directory"], "15_Beta_nfph_vs_P_e_1n_FD",
                                              0, 50, 0.9, 1.1);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta of all FD neutrals (1n, FD)">
    hPlot1D hBeta_All_FD_Neut_01_1n_FD = hPlot1D("1n", "FD", "#beta of all FD neut.", "#beta of all FD neut.", "#beta",
                                                 directories.Beta_Directory_map["All_FD_neutrals_01_1n_Directory"], "01_Beta_All_FD_Neutrals_1n",
                                                 Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_All_FD_Neut_01_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - ZOOMOUT", "#beta of all FD neut. - ZOOMOUT", "#beta",
                                                         directories.Beta_Directory_map["All_FD_neutrals_01_1n_Directory"], "01_Beta_All_FD_Neutrals_1n_ZOOMOUT",
                                                         Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_All_FD_Neut_02_1n_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - !PCAL", "#beta of all FD neut. w/o PCAL hit", "#beta",
                                                 directories.Beta_Directory_map["All_FD_neutrals_02_1n_Directory"], "01_Beta_All_FD_Neutrals_noPCAL_1n",
                                                 Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_All_FD_Neut_02_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - !PCAL - ZOOMOUT", "#beta of all FD neut. w/o PCAL hit - ZOOMOUT",
                                                         "#beta", directories.Beta_Directory_map["All_FD_neutrals_02_1n_Directory"],
                                                         "01_Beta_All_FD_Neutrals_noPCAL_1n_ZOOMOUT", Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_All_FD_Neut_03_1n_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - !PCAL & ECIN", "#beta of all FD neut. w/o PCAL & w/ECIN", "#beta",
                                                 directories.Beta_Directory_map["All_FD_neutrals_03_1n_Directory"], "01_Beta_All_FD_Neutrals_noPCAL_yesECIN_1n",
                                                 Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_All_FD_Neut_03_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - !PCAL & ECIN - ZOOMOUT",
                                                         "#beta of all FD neut. w/o PCAL & w/ECIN - ZOOMOUT", "#beta",
                                                         directories.Beta_Directory_map["All_FD_neutrals_03_1n_Directory"],
                                                         "01_Beta_All_FD_Neutrals_noPCAL_yesECIN_1n_ZOOMOUT",
                                                         Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_All_FD_Neut_04_1n_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - !PCAL & !ECIN & ECOUT", "#beta of all FD neut. w/o PCAL/ECIN & w/ECOUT",
                                                 "#beta", directories.Beta_Directory_map["All_FD_neutrals_04_1n_Directory"],
                                                 "01_Beta_All_FD_Neutrals_noPCAL_noECIN_yesECOUT_1n", Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_All_FD_Neut_04_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of all FD neut. - !PCAL & !ECIN & ECOUT - ZOOMOUT",
                                                         "#beta of all FD neut. w/o PCAL/ECIN & w/ECOUT - ZOOMOUT", "#beta",
                                                         directories.Beta_Directory_map["All_FD_neutrals_04_1n_Directory"],
                                                         "01_Beta_All_FD_Neutrals_noPCAL_noECIN_yesECOUT_1n_ZOOMOUT",
                                                         Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);
    //</editor-fold>

    //<editor-fold desc="Beta of all FD neutrals - no PDG=0 (1n, FD)">
    hPlot1D hBeta_FD_Neut_noPDG0_01_1n_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0)", "#beta of FD neut. (noPDG0)", "#beta",
                                                    directories.Beta_Directory_map["FD_neutrals_noPDG0_01_1n_Directory"], "01_Beta_FD_Neutrals_1n",
                                                    Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_FD_Neut_noPDG0_01_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - ZOOMOUT", "#beta of FD neut. (noPDG0) - ZOOMOUT", "#beta",
                                                            directories.Beta_Directory_map["FD_neutrals_noPDG0_01_1n_Directory"], "01_Beta_FD_Neutrals_1n_ZOOMOUT",
                                                            Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_FD_Neut_noPDG0_02_1n_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - !PCAL", "#beta of FD neut. (no PDG=0) w/o PCAL hit", "#beta",
                                                    directories.Beta_Directory_map["FD_neutrals_noPDG0_02_1n_Directory"], "01_Beta_FD_Neutrals_noPCAL_1n",
                                                    Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_FD_Neut_noPDG0_02_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - !PCAL - ZOOMOUT",
                                                            "#beta of FD neut. (no PDG=0) w/o PCAL hit - ZOOMOUT",
                                                            "#beta", directories.Beta_Directory_map["FD_neutrals_noPDG0_02_1n_Directory"],
                                                            "01_Beta_FD_Neutrals_noPCAL_1n_ZOOMOUT", Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_FD_Neut_noPDG0_03_1n_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - !PCAL & ECIN", "#beta of FD neut. (noPDG0) w/o PCAL & w/ECIN", "#beta",
                                                    directories.Beta_Directory_map["FD_neutrals_noPDG0_03_1n_Directory"], "01_Beta_FD_Neutrals_noPCAL_yesECIN_1n",
                                                    Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_FD_Neut_noPDG0_03_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - !PCAL & ECIN - ZOOMOUT",
                                                            "#beta of FD neut. (noPDG0) w/o PCAL & w/ECIN - ZOOMOUT", "#beta",
                                                            directories.Beta_Directory_map["FD_neutrals_noPDG0_03_1n_Directory"],
                                                            "01_Beta_FD_Neutrals_noPCAL_yesECIN_1n_ZOOMOUT", Beta_dist_ZOOMOUT_lboundary,
                                                            Beta_dist_ZOOMOUT_uboundary, 65);

    hPlot1D hBeta_FD_Neut_noPDG0_04_1n_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - !PCAL & !ECIN & ECOUT",
                                                    "#beta of FD neut. (noPDG0) w/o PCAL/ECIN & w/ECOUT",
                                                    "#beta", directories.Beta_Directory_map["FD_neutrals_noPDG0_04_1n_Directory"],
                                                    "01_Beta_FD_Neutrals_noPCAL_noECIN_yesECOUT_1n", Beta_dist_lboundary, Beta_dist_uboundary, 65);
    hPlot1D hBeta_FD_Neut_noPDG0_04_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of FD neut. (noPDG0) - !PCAL & !ECIN & ECOUT - ZOOMOUT",
                                                            "#beta of FD neut. (noPDG0) w/o PCAL/ECIN & w/ECOUT - ZOOMOUT", "#beta",
                                                            directories.Beta_Directory_map["FD_neutrals_noPDG0_04_1n_Directory"],
                                                            "01_Beta_FD_Neutrals_noPCAL_noECIN_yesECOUT_1n_ZOOMOUT",
                                                            Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, 65);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P plots">

    //<editor-fold desc="Beta vs. P (no #(e) cut)">

    //<editor-fold desc="Beta vs. P (no #(e) cut)">
    hPlot2D hBeta_vs_P_CD = hPlot2D("all particles", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                    directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "01_Beta_vs_P_All_Particles_CD", 0, beamE * 1.425, 0, 3);
    hPlot2D hBeta_vs_P_FD = hPlot2D("all particles", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                    directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "01_Beta_vs_P_All_Particles_FD", 0, beamE * 1.425, 0, 3);

    hPlot2D hBeta_vs_P_Electrons_Only_FD = hPlot2D("electrons only", "no #(e) cut", "", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "02_Beta_vs_P_Electrons_Only_FD", 0, beamE * 1.425,
                                                   0.5, 3);

    hPlot2D hBeta_vs_P_Protons_Only_CD = hPlot2D("protons only", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                 directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "03_Beta_vs_P_Protons_Only_CD",
                                                 0, beamE * 1.1, 0, 1.1);
    hPlot2D hBeta_vs_P_Protons_Only_FD = hPlot2D("protons only", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                 directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "03_Beta_vs_P_Protons_Only_FD",
                                                 0, beamE * 1.1, 0, 1.1);

    hPlot2D hBeta_vs_P_Neutrons_Only_CD = hPlot2D("neutrons only", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "04_Beta_vs_P_Neutrons_Only_CD",
                                                  0, beamE * 1.1, 0, 3);
    hPlot2D hBeta_vs_P_Neutrons_Only_FD = hPlot2D("neutrons only", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "04_Beta_vs_P_Neutrons_Only_FD",
                                                  0, beamE * 1.1, 0, 3);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (no #(e) cut)">
    hPlot2D hBeta_vs_P_positive_part_All_e_CD = hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                        "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "01_Beta_vs_P_q_p1_All_e_CD", 0, beamE * 1.425, 0, 3);
    hPlot2D hBeta_vs_P_positive_part_All_e_FD = hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                        "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "01_Beta_vs_P_q_p1_All_e_FD", 0, beamE * 1.425, 0, 3);
    hPlot2D hBeta_vs_P_neutral_part_All_e_CD = hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                       "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                       "02_Beta_vs_P_q_0_All_e_CD", 0, beamE * 1.425, 0, 3);
    hPlot2D hBeta_vs_P_neutral_part_All_e_FD = hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                       "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                       "02_Beta_vs_P_q_0_All_e_FD", 0, beamE * 1.425, 0, 3);
    hPlot2D hBeta_vs_P_negative_part_All_e_CD = hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                        "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "03_Beta_vs_P_q_m1_All_e_CD", 0, beamE * 1.425, 0, 3);
    hPlot2D hBeta_vs_P_negative_part_All_e_FD = hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                        "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "03_Beta_vs_P_q_m1_All_e_FD", 0, beamE * 1.425, 0, 3);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1e cut)">

    //<editor-fold desc="Beta vs. P for all particles (1e cut)">
    hPlot2D hBeta_vs_P_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "01_Beta_vs_P_1e_cut_All_Particles_CD",
                                           0, beamE * 1.1, 0, 1.7);
    hPlot2D hBeta_vs_P_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "01_Beta_vs_P_1e_cut_All_Particles_FD",
                                           0, beamE * 1.1, 0, 1.7);

    hPlot2D hBeta_vs_P_1e_cut_Electrons_Only_FD = hPlot2D("electrons only", "1e cut", "", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                          directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "02_Beta_vs_P_1e_cut_Electrons_Only_FD",
                                                          0, beamE * 1.425, 0.5, 1.7);

    hPlot2D hBeta_vs_P_1e_cut_Protons_Only_CD = hPlot2D("protons only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "03_Beta_vs_P_1e_cut_Protons_Only_CD",
                                                        0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1e_cut_Protons_Only_FD = hPlot2D("protons only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "03_Beta_vs_P_1e_cut_Protons_Only_FD",
                                                        0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_CD = hPlot2D("neutrons only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "04_Beta_vs_P_1e_cut_Neutrons_Only_CD",
                                                         0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_FD = hPlot2D("neutrons only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "04_Beta_vs_P_1e_cut_Neutrons_Only_FD",
                                                         0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1e_cut_Kplus_Only_CD = hPlot2D("K^{+} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "05_Beta_vs_P_1e_cut_Kplus_Only_CD",
                                                      0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1e_cut_Kplus_Only_FD = hPlot2D("K^{+} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "05_Beta_vs_P_1e_cut_Kplus_Only_FD",
                                                      0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1e_cut_Kminus_Only_CD = hPlot2D("K^{-} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "06_Beta_vs_P_1e_cut_Kminus_Only_CD",
                                                       0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1e_cut_Kminus_Only_FD = hPlot2D("K^{-} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "06_Beta_vs_P_1e_cut_Kminus_Only_FD",
                                                       0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1e_cut_piplus_Only_CD = hPlot2D("#pi^{+} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "07_Beta_vs_P_1e_cut_piplus_Only_CD",
                                                       0, P_boundary, 0, 1.7);
    hPlot2D hBeta_vs_P_1e_cut_piplus_Only_FD = hPlot2D("#pi^{+} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "07_Beta_vs_P_1e_cut_piplus_Only_FD",
                                                       0, P_boundary, 0, 1.7);

    hPlot2D hBeta_vs_P_1e_cut_piminus_Only_CD = hPlot2D("#pi^{-} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "08_Beta_vs_P_1e_cut_piminus_Only_CD",
                                                        0, P_boundary, 0, 1.7);
    hPlot2D hBeta_vs_P_1e_cut_piminus_Only_FD = hPlot2D("#pi^{-} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "08_Beta_vs_P_1e_cut_piminus_Only_FD",
                                                        0, P_boundary, 0, 1.7);

    hPlot2D hBeta_vs_P_1e_cut_pizero_Only_CD = hPlot2D("#pi^{0} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "09_Beta_vs_P_1e_cut_pizero_Only_CD",
                                                       0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1e_cut_pizero_Only_FD = hPlot2D("#pi^{0} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "09_Beta_vs_P_1e_cut_pizero_Only_FD",
                                                       0, P_boundary, 0, Beta_boundary);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1e cut)">
    hPlot2D hBeta_vs_P_positive_part_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                         "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "01_Beta_vs_P_q_p1_1e_cut_CD", 0, beamE * 1.1, 0, 1.7);
    hPlot2D hBeta_vs_P_positive_part_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                         "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "01_Beta_vs_P_q_p1_1e_cut_FD", 0, beamE * 1.1, 0, 1.7);
    hPlot2D hBeta_vs_P_neutral_part_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                        "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                        "02_Beta_vs_P_q_0_1e_cut_CD", 0, beamE * 1.1, 0, 1.7);
    hPlot2D hBeta_vs_P_neutral_part_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                        "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                        "02_Beta_vs_P_q_0_1e_cut_FD", 0, beamE * 1.1, 0, 1.7);
    hPlot2D hBeta_vs_P_negative_part_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                         "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "03_Beta_vs_P_q_m1_1e_cut_CD", 0, beamE * 1.1, 0, 1.7);
    hPlot2D hBeta_vs_P_negative_part_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                         "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "03_Beta_vs_P_q_m1_1e_cut_FD", 0, beamE * 1.1, 0, 1.7);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1p)">

    //<editor-fold desc="Beta vs. P for all particles (1p)">
    hPlot2D hBeta_vs_P_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "01_Beta_vs_P_1p_All_Particles_CD",
                                       0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "01_Beta_vs_P_1p_All_Particles_FD",
                                       0, beamE * 1.1, 0, 1.5);

    hPlot2D hBeta_vs_P_1p_Electrons_Only_FD = hPlot2D("electrons only", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "02_Beta_vs_P_1p_Electrons_Only_FD",
                                                      0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1p_Protons_Only_CD = hPlot2D("protons only", "1p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "03_Beta_vs_P_1p_Protons_Only_CD",
                                                    0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1p_Protons_Only_FD = hPlot2D("protons only", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "03_Beta_vs_P_1p_Protons_Only_FD",
                                                    0, P_boundary, 0, Beta_boundary);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1p)">
    hPlot2D hBeta_vs_P_positive_part_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "01_Beta_vs_P_q_p1_1p_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_positive_part_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "01_Beta_vs_P_q_p1_1p_FD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neutral_part_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                    "02_Beta_vs_P_q_0_1p_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neutral_part_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                    "02_Beta_vs_P_q_0_1p_FD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_negative_part_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "03_Beta_vs_P_q_m1_1p_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_negative_part_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "03_Beta_vs_P_q_m1_1p_FD", 0, beamE * 1.1, 0, 1.5);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1n)">

    //<editor-fold desc="Beta vs. P for all particles (1n)">
    hPlot2D hBeta_vs_P_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "00_Beta_vs_P_All_Particles_CD_1n",
                                       0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "00_Beta_vs_P_All_Particles_FD_1n",
                                       0, beamE * 1.1, 0, 1.5);

    hPlot2D hBeta_vs_P_1n_Electrons_Only_FD = hPlot2D("electrons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "01_Beta_vs_P_Electrons_Only_FD_1n",
                                                      0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1n_Neutrons_Only_CD = hPlot2D("id. neutrons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                     directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "02_Beta_vs_P_id_Neutrons_Only_CD_1n",
                                                     0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Neutrons_Only_FD = hPlot2D("id. neutrons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                     directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "02_Beta_vs_P_id_Neutrons_Only_FD_1n",
                                                     0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD = hPlot2D("id. neutrons only", "1n", "FD", "#beta vs. P - ZOOMOUT", "#beta vs. P", "P [GeV/c]", "#beta",
                                                             directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "02_Beta_vs_P_id_Neutrons_Only_ZOOMOUT_FD_1n",
                                                             0, P_boundary, 0, 2);

    hPlot2D hBeta_vs_P_1n_Neutrons_Only_from_photons_FD = hPlot2D("id. neutrons", "1n", "FD", "#beta vs. P",
                                                                  " Cond. to id. neutron's #beta vs. P from 'photons'", "P [GeV/c]", "#beta",
                                                                  directories.Beta_Directory_map["Beta_VS_P_1n_Directory"],
                                                                  "02a_Beta_vs_P_id_Neutrons_Only_from_potons_FD_1n",
                                                                  0, P_boundary, 0.9, 1.15);

    hPlot2D hBeta_vs_P_1n_Photons_Only_CD = hPlot2D("id. photons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "03_Beta_vs_P_id_Photons_Only_CD_1n",
                                                    0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Photons_Only_FD = hPlot2D("id. photons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "03_Beta_vs_P_id_Photons_Only_FD_1n",
                                                    0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1n_Protons_Only_CD = hPlot2D("id. protons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "04_Beta_vs_P_id_Protons_Only_CD_1n",
                                                    0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Protons_Only_FD = hPlot2D("id. protons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "04_Beta_vs_P_id_Protons_Only_FD_1n",
                                                    0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1n_Piplus_Only_CD = hPlot2D("id. #pi^{+} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_piplus_Only_CD_1n",
                                                   0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Piplus_Only_FD = hPlot2D("id. #pi^{+} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_piplus_Only_FD_1n",
                                                   0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1n_Piminus_Only_CD = hPlot2D("id. #pi^{-} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_piminus_Only_CD_1n",
                                                    0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Piminus_Only_FD = hPlot2D("id. #pi^{-} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_piminus_Only_FD_1n",
                                                    0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1n_Kplus_Only_CD = hPlot2D("id. K^{+} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_Kplus_Only_CD_1n",
                                                  0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Kplus_Only_FD = hPlot2D("id. K^{+} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_Kplus_Only_FD_1n",
                                                  0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_1n_Kminus_Only_CD = hPlot2D("id. K^{-} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_Kminus_Only_CD_1n",
                                                   0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_1n_Kminus_Only_FD = hPlot2D("id. K^{-} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_Kminus_Only_FD_1n",
                                                   0, P_boundary, 0, Beta_boundary);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1n)">
    hPlot2D hBeta_vs_P_pos_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "01_Beta_vs_P_q_p1_1n_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_pos_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "01_Beta_vs_P_q_p1_1n_FD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neut_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                 "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                 "02_Beta_vs_P_q_0_1n_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neut_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                 "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                 "02_Beta_vs_P_q_0_1n_FD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neg_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "03_Beta_vs_P_q_m1_1n_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neg_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "03_Beta_vs_P_q_m1_1n_FD", 0, beamE * 1.1, 0, 1.5);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (2p)">

    //<editor-fold desc="Beta vs. P for all particles (2p)">
    hPlot2D hBeta_vs_P_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "01_Beta_vs_P_2p_All_Particles_CD",
                                       0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "01_Beta_vs_P_2p_All_Particles_FD",
                                       0, beamE * 1.1, 0, 1.5);

    hPlot2D hBeta_vs_P_2p_Electrons_Only_FD = hPlot2D("electrons only", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "02_Beta_vs_P_2p_Electrons_Only_FD",
                                                      0, P_boundary, 0, Beta_boundary);

    hPlot2D hBeta_vs_P_2p_Protons_Only_CD = hPlot2D("protons only", "2p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "03_Beta_vs_P_2p_Protons_Only_CD",
                                                    0, P_boundary, 0, Beta_boundary);
    hPlot2D hBeta_vs_P_2p_Protons_Only_FD = hPlot2D("protons only", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "03_Beta_vs_P_2p_Protons_Only_FD",
                                                    0, P_boundary, 0, Beta_boundary);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (2p)">
    hPlot2D hBeta_vs_P_positive_part_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "01_Beta_vs_P_q_p1_2p_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_positive_part_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "01_Beta_vs_P_q_p1_2p_FD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neutral_part_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                    "02_Beta_vs_P_q_0_2p_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_neutral_part_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                    "02_Beta_vs_P_q_0_2p_FD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_negative_part_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "03_Beta_vs_P_q_m1_2p_CD", 0, beamE * 1.1, 0, 1.5);
    hPlot2D hBeta_vs_P_negative_part_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "03_Beta_vs_P_q_m1_2p_FD", 0, beamE * 1.1, 0, 1.5);
    //</editor-fold>

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
    THStack *sTheta_e_1p_FD = new THStack("#theta_{e} (1p, FD)", "#theta_{e} of Outgoing Electron (1p, FD);#theta_{e} [Deg];");
    THStack *sTheta_e_1n_FD = new THStack("#theta_{e} (1n, FD)", "#theta_{e} of Outgoing Electron (1n, FD);#theta_{e} [Deg];");
    THStack *sTheta_e_1e2p_FD = new THStack("#theta_{e} (1e2p, FD)", "#theta_{e} of Outgoing Electron (1e2p, FD);#theta_{e} [Deg];");
    THStack *sTheta_e_2p_FD = new THStack("#theta_{e} (2p, FD)", "#theta_{e} of Outgoing Electron (2p, FD);#theta_{e} [Deg];");

    /* Theta_e histograms (no #(e) cut) */
    TH1D *hTheta_e_All_e_FD = new TH1D("#theta_{e} (no #(e) cut, FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_e_FD_Dir = directories.Angle_Directory_map["Theta_e_All_e_Directory"];

    /* Theta_e histograms (1e cut) */
    TH1D *hTheta_e_1e_cut_FD = new TH1D("#theta_{e} (1e Cut, FD)", "#theta_{e} of Outgoing Electron (1e Cut, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Theta_e_1e_cut_Directory"];

    /* Theta_e histograms (1p) */
    TH1D *hTheta_e_All_Int_1p_FD = new TH1D("#theta_{e} (All Int., 1p, FD)", "#theta_{e} of Outgoing Electron (All Int., 1p, FD);#theta_{e} [Deg];", 100,
                                            Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_1p_FD = new TH1D("#theta_{e} (QEL Only, 1p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1p, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_1p_FD = new TH1D("#theta_{e} (MEC Only, 1p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1p, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    TH1D *hTheta_e_RES_1p_FD = new TH1D("#theta_{e} (RES Only, 1p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1p, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_1p_FD = new TH1D("#theta_{e} (DIS Only, 1p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1p, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    string hTheta_e_All_Int_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_QEL_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_MEC_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_RES_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_DIS_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];

    /* Theta_e histograms (1n) */
    TH1D *hTheta_e_All_Int_1n_FD = new TH1D("#theta_{e} (All Int., 1n, FD)", "#theta_{e} of Outgoing Electron (All Int., 1n, FD);#theta_{e} [Deg];", 100,
                                            Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_1n_FD = new TH1D("#theta_{e} (QEL Only, 1n, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1n, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_1n_FD = new TH1D("#theta_{e} (MEC Only, 1n, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1n, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    TH1D *hTheta_e_RES_1n_FD = new TH1D("#theta_{e} (RES Only, 1n, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1n, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_1n_FD = new TH1D("#theta_{e} (DIS Only, 1n, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1n, FD);#theta_{e} [Deg];", 100, Theta_lboundary_FD,
                                        Theta_uboundary_FD);
    string hTheta_e_All_Int_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_QEL_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_MEC_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_RES_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_DIS_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];

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
    TH1D *hPhi_e_All_e_FD = new TH1D("#phi_{e} (no #(e) cut, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    string hPhi_e_All_e_FD_Dir = directories.Angle_Directory_map["Phi_e_All_e_Directory"];

    /* Phi_e histograms (1e cut) */
    TH1D *hPhi_e_1e_cut_FD = new TH1D("#phi_{e} (1e Cut, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    string hPhi_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Phi_e_1e_cut_Directory"];

    /* Phi_e histograms (1p) */
    TH1D *hPhi_e_All_Int_1p_FD = new TH1D("#phi_{e} 1p (All Int., FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_QEL_1p_FD = new TH1D("#phi_{e} for 1p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_MEC_1p_FD = new TH1D("#phi_{e} for 1p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_RES_1p_FD = new TH1D("#phi_{e} for 1p (RES Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_DIS_1p_FD = new TH1D("#phi_{e} for 1p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    string hPhi_e_All_Int_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_QEL_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_MEC_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_RES_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_DIS_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];

    /* Phi_e histograms (1n) */
    TH1D *hPhi_e_All_Int_1n_FD = new TH1D("#phi_{e} 1n (All Int., FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_QEL_1n_FD = new TH1D("#phi_{e} for 1n (QEL Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_MEC_1n_FD = new TH1D("#phi_{e} for 1n (MEC Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_RES_1n_FD = new TH1D("#phi_{e} for 1n (RES Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    TH1D *hPhi_e_DIS_1n_FD = new TH1D("#phi_{e} for 1n (DIS Only, FD)", ";#phi_{e} [Deg];", 100, Phi_lboundary_FD, Phi_uboundary_FD);
    string hPhi_e_All_Int_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_QEL_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_MEC_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_RES_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_DIS_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];

    /* Phi_e histograms (1e2p) */
    TH1D *hPhi_e_All_Int_1e2p_FD = new TH1D("#phi_{e} 1e2p (All Int., FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_QEL_1e2p_FD = new TH1D("#phi_{e} for 1e2p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_MEC_1e2p_FD = new TH1D("#phi_{e} for 1e2p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_RES_1e2p_FD = new TH1D("#phi_{e} for 1e2p (RES Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_DIS_1e2p_FD = new TH1D("#phi_{e} for 1e2p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    string hPhi_e_All_Int_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_QEL_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_MEC_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_RES_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];
    string hPhi_e_DIS_1e2p_FD_Dir = directories.Angle_Directory_map["Phi_e_1e2p_Directory"];

    /* Phi_e histograms (2p) */
    TH1D *hPhi_e_All_Int_2p_FD = new TH1D("#phi_{e} 2p (All Int., FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_QEL_2p_FD = new TH1D("#phi_{e} for 2p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_MEC_2p_FD = new TH1D("#phi_{e} for 2p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_RES_2p_FD = new TH1D("#phi_{e} for 2p (RES Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
    TH1D *hPhi_e_DIS_2p_FD = new TH1D("#phi_{e} for 2p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, -180, 180);
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
                                             250, -200, 200, 250, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1p_Directory"];

    /* Theta_e vs. Phi_e histograms (1n) */
    TH2D *hTheta_e_VS_Phi_e_1n_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 1n, FD)", "#theta_{e} vs. #phi_{e} (All Int., 1n, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             250, Phi_lboundary_FD, Phi_uboundary_FD, 250, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1n_Directory"];

    /* Theta_e vs. Phi_e histograms (2p) */
    TH2D *hTheta_e_VS_Phi_e_2p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 2p, FD)", "#theta_{e} vs. #phi_{e} (All Int., 2p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             250, -200, 200, 250, 0, 50);
    string hTheta_e_VS_Phi_e_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_2p_Directory"];
    //</editor-fold>

// Other angle plots ----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Other angle plots">

    //<editor-fold desc="Other angle plots (no #(e) cut)">

    //<editor-fold desc="Theta_n vs. Phi_n for neutrals hitting the PCAL (no #(e) cut, FD only)">
    hPlot2D hTheta_neut_VS_Phi_neut_All_e_FD = hPlot2D("no #(e) cut", "FD", "#theta_{neut} vs. #phi_{neut} w/ PCAL hit",
                                                       "#theta_{neut} vs. #phi_{neut} for neutrals with PCAL hit",
                                                       "#phi_{neut} [Deg]", "#theta_{neut} [Deg]", directories.Angle_Directory_map["Theta_n_VS_Phi_n_1n_Directory"],
                                                       "01_Theta_neut_VS_Phi_neut_All_Int_1n_FD", -180, 180, 0, 50);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1e cut)">

    //<editor-fold desc="Neutron veto plots (1e cut)">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e}",
                                                                  "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                                  "#Delta#phi_{n,e} = #phi_{n}^{ECIN/ECOUT} - #phi_{e}^{PCAL} [Deg]",
                                                                  "#Delta#theta_{n,e} = #theta_{n}^{ECIN/ECOUT} - #theta_{e}^{PCAL} [Deg]",
                                                                  directories.Angle_Directory_map["Neutron_Veto_Directory_1e_cut"],
                                                                  "01_Neutron_hits_vs_electron_hits_1e_cut", -180, 180, -50, 50);
    hPlot2D hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{n,p} vs. #Delta#phi_{n,p}",
                                                                "'Neutron Hits' vs. Proton Hits - Before Veto",
                                                                "#Delta#phi_{n,p} = #phi_{n}^{ECIN/ECOUT} - #phi_{p}^{PCAL} [Deg]",
                                                                "#Delta#theta_{n,p} = #theta_{n}^{ECIN/ECOUT} - #theta_{p}^{PCAL} [Deg]",
                                                                directories.Angle_Directory_map["Neutron_Veto_Directory_1e_cut"],
                                                                "02_Neutron_hits_vs_proton_hits_1e_cut", -180, 180, -50, 50);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1p)">

// Theta_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p (1p, FD only)">
    THStack *sTheta_p_1p = new THStack("#theta_{p} (All Int., 1p, FD)", "#theta_{p} of Outgoing Proton (All Int., 1p, FD);#theta_{p} [Deg];");
    TH1D *hTheta_p_All_Int_1p = new TH1D("#theta_{p} (All Int., 1p, FD)", "#theta_{p} of Outgoing Proton (All Int., 1p, FD);#theta_{p} [Deg];", 100, Theta_lboundary_FD,
                                         Theta_uboundary_FD);
    string hTheta_p_All_Int_1p_Dir = directories.Angle_Directory_map["Theta_p_Directory_1p"];
    //</editor-fold>

// Phi_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_p (1p, FD only)">
    THStack *sPhi_p_1p = new THStack("#phi_{p} (All Int., 1p, FD)", "#phi_{p} of Outgoing Proton (All Int., 1p, FD);#phi_{p} [Deg];");
    TH1D *hPhi_p_All_Int_1p = new TH1D("#phi_{p} (All Int., 1p, FD)", "#phi_{p} of Outgoing Proton (All Int., 1p, FD);#phi_{p} [Deg];", 100, Phi_lboundary_FD,
                                       Phi_uboundary_FD);
    string hPhi_p_All_Int_1p_Dir = directories.Angle_Directory_map["Phi_p_Directory_1p"];
    //</editor-fold>

// Theta_p_e_p_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_p (1p, FD only)">
    THStack *sTheta_p_e_p_p_1p = new THStack("#theta_{#vec{P}_{e},#vec{P}_{p}} (All Int., 1p, FD)",
                                             "#theta_{#vec{P}_{e},#vec{P}_{p}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{p} (All Int., 1p, FD);#theta_{#vec{P}_{e},#vec{P}_{p}} [Deg];");
    TH1D *hTheta_p_e_p_p_1p = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{p}} (All Int., 1p, FD)",
                                       "#theta_{#vec{P}_{e},#vec{P}_{p}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{p} (All Int., 1p, FD);"
                                       "#theta_{#vec{P}_{e},#vec{P}_{p}} [Deg];", 100, 0, 180);
    string hTheta_p_e_p_p_1p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1p"];
    //</editor-fold>

// Theta_q_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (1p, FD only)">
    THStack *sTheta_q_p_p_1p = new THStack("#theta_{#vec{q},#vec{P}_{p}} (All Int., 1p, FD)",
                                           "#theta_{#vec{q},#vec{P}_{p}} - Opening Angle Between #vec{q} and #vec{P}_{p} (All Int., 1p, FD);#theta_{#vec{q},#vec{P}_{p}} [Deg];");
    TH1D *hTheta_q_p_p_1p = new TH1D("#theta_{#vec{q},#vec{P}_{p}} (All Int., 1p, FD)",
                                     "#theta_{#vec{q},#vec{P}_{p}} - Opening Angle Between #vec{q} and #vec{P}_{p} (All Int., 1p, FD);"
                                     "#theta_{#vec{q},#vec{P}_{p}} [Deg];", 100, 0, 180);
    string hTheta_q_p_p_1p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1p"];
    //</editor-fold>

// Theta_q_p_p vs. |P_p|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_p vs. |P_p|/|q| (1p, FD only)">
    TH2D *hTheta_q_p_p_vs_p_p_q_1p = new TH2D("#theta_{#vec{q},#vec{P}_{p}} vs. r=|P_{p}|/|q| (All Int., 1p, FD)",
                                              "#theta_{#vec{q},#vec{P}_{p}} vs. r=|#vec{P_{p}}|/|#vec{q}| (All Int., 1p, FD);r;#theta_{#vec{q},#vec{P}_{p}}",
                                              250, 0, 1.05, 250, 0, 180);
    string hTheta_q_p_p_vs_p_p_q_1p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1p"];
    //</editor-fold>

// Theta_q_p_p vs. |p_N|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_p vs. |p_N|/|q| (1p, FD only)">
    TH2D *hTheta_q_p_p_vs_p_N_q_1p = new TH2D("#theta_{#vec{q},#vec{P}_{p}} vs. r=|P_{N}|/|q| (All Int., 1p, FD)",
                                              "#theta_{#vec{q},#vec{P}_{p}} vs. r=|#vec{P_{N}}|/|#vec{q}| (All Int., 1p, FD);r;#theta_{#vec{q},#vec{P}_{p}}",
                                              250, 0, 1.05, 250, 0, 180);
    string hTheta_q_p_p_vs_p_N_q_1p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1p"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1n)">

// Theta_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_n (1n, FD only)">
    THStack *sTheta_n_1n = new THStack("#theta_{n} (All Int., 1n, FD)", "#theta_{n} of Outgoing Neutron (All Int., 1n, FD);#theta_{n} [Deg];");
    TH1D *hTheta_n_All_Int_1n = new TH1D("#theta_{n} (All Int., 1n, FD)", "#theta_{n} of Outgoing Neutron (All Int., 1n, FD);#theta_{n} [Deg];", 100, Theta_lboundary_FD,
                                         Theta_uboundary_FD);
    string hTheta_n_All_Int_1n_Dir = directories.Angle_Directory_map["Theta_n_Directory_1n"];
    //</editor-fold>

// Phi_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_n (1n, FD only)">
    THStack *sPhi_n_1n = new THStack("#phi_{n} (All Int., 1n, FD)", "#phi_{n} of Outgoing Neutron (All Int., 1n, FD);#phi_{n} [Deg];");
    TH1D *hPhi_n_All_Int_1n = new TH1D("#phi_{n} (All Int., 1n, FD)", "#phi_{n} of Outgoing Neutron (All Int., 1n, FD);#phi_{n} [Deg];", 100, Phi_lboundary_FD,
                                       Phi_uboundary_FD);
    string hPhi_n_All_Int_1n_Dir = directories.Angle_Directory_map["Phi_n_Directory_1n"];
    //</editor-fold>

// Theta_n vs. Phi_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_n vs. Phi_n (1n, FD only)">
    /* All 1n neutrons (by definition) */
    hPlot2D hTheta_n_VS_Phi_n_around_beta1_1n_FD = hPlot2D("1n", "FD", "#theta_{n} vs. #phi_{n} for '#gamma' with #beta~1",
                                                           "#theta_{n} vs. #phi_{n}  for 'photons' with #beta~1", "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                                           directories.Angle_Directory_map["Theta_n_VS_Phi_n_1n_Directory"],
                                                           "02_Theta_n_VS_Phi_n_around_beta1_All_Int_1n_FD", -180, 180, 0, 50);
    //</editor-fold>

// Theta_p_e_p_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_n (1n, FD only)">
    THStack *sTheta_p_e_p_n_1n = new THStack("#theta_{#vec{P}_{e},#vec{P}_{n}} (All Int., 1n, FD)",
                                             "#theta_{#vec{P}_{e},#vec{P}_{n}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{n} (All Int., 1n, FD);#theta_{#vec{P}_{e},#vec{P}_{n}} [Deg];");
    TH1D *hTheta_p_e_p_n_1n = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{n}} (All Int., 1n, FD)",
                                       "#theta_{#vec{P}_{e},#vec{P}_{n}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{n} (All Int., 1n, FD);"
                                       "#theta_{#vec{P}_{e},#vec{P}_{n}} [Deg];", 100, 0, 180);
    string hTheta_p_e_p_n_1n_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1n"];
    //</editor-fold>

// Theta_q_p_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n (1n, FD only)">
    THStack *sTheta_q_p_n_1n = new THStack("#theta_{#vec{q},#vec{P}_{n}} (All Int., 1n, FD)",
                                           "#theta_{#vec{q},#vec{P}_{n}} - Opening Angle Between #vec{q} and #vec{P}_{n} (All Int., 1n, FD);#theta_{#vec{q},#vec{P}_{n}} [Deg];");
    TH1D *hTheta_q_p_n_1n = new TH1D("#theta_{#vec{q},#vec{P}_{n}} (All Int., 1n, FD)",
                                     "#theta_{#vec{q},#vec{P}_{n}} - Opening Angle Between #vec{q} and #vec{P}_{n} (All Int., 1n, FD);"
                                     "#theta_{#vec{q},#vec{P}_{n}} [Deg];", 100, 0, 180);
    string hTheta_q_p_n_1n_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1n"];
    //</editor-fold>

// Theta_q_p_n vs. |p_n|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n vs. |p_n|/|q| (1n, FD only)">
    TH2D *hTheta_q_p_n_vs_p_n_q_1n = new TH2D("#theta_{#vec{q},#vec{P}_{n}} vs. r=|P_{n}|/|q| (All Int., 1n, FD)",
                                              "#theta_{#vec{q},#vec{P}_{n}} vs. r=|#vec{P}_{n}|/|#vec{q}| (All Int., 1n, FD);r;#theta_{#vec{q},#vec{P}_{n}}",
                                              250, 0, 1.05, 250, 0, 180);
    string hTheta_q_p_n_vs_p_n_q_1n_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1n"];
    //</editor-fold>

// Theta_q_p_n vs. |p_N|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n vs. |p_N|/|q| (1n, FD only)">
    TH2D *hTheta_q_p_n_vs_p_N_q_1n = new TH2D("#theta_{#vec{q},#vec{P}_{n}} vs. r=|P_{N}|/|q| (All Int., 1n, FD)",
                                              "#theta_{#vec{q},#vec{P}_{n}} vs. r=|#vec{P_{N}}|/|#vec{q}| (All Int., 1n, FD);r;#theta_{#vec{q},#vec{P}_{n}}",
                                              250, 0, 1.05, 250, 0, 180);
    string hTheta_q_p_n_vs_p_N_q_1n_Dir = directories.Angle_Directory_map["Opening_angle_Directory_1n"];
    //</editor-fold>

// Neutron veto plots (1n) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron veto plots (1n)">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} BV",
                                                              "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                              "#Delta#phi_{n,e} = #phi_{n}^{ECIN/ECOUT} - #phi_{e}^{PCAL} [Deg]",
                                                              "#Delta#theta_{n,e} = #theta_{n}^{ECIN/ECOUT} - #theta_{e}^{PCAL} [Deg]",
                                                              directories.Angle_Directory_map["Neutron_Veto_Directory_1n"],
//                                                              "01_Neutron_hits_vs_electron_hits_BV_1n", -180 * 3, 180 * 3, -50, 50);
//                                                              "01_Neutron_hits_vs_electron_hits_BV_1n", -360, 360, -50, 50);
                                                              "01_Neutron_hits_vs_electron_hits_BV_1n", -180, 180, -50, 50);
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} AV",
                                                              "'Neutron Hits' vs. Electron Hits - After Veto",
                                                              "#Delta#phi_{n,e} = #phi_{n}^{ECIN/ECOUT} - #phi_{e}^{PCAL} [Deg]",
                                                              "#Delta#theta_{n,e} = #theta_{n}^{ECIN/ECOUT} - #theta_{e}^{PCAL} [Deg]",
                                                              directories.Angle_Directory_map["Neutron_Veto_Directory_1n"],
//                                                              "01_Neutron_hits_vs_electron_hits_AV_1n", -360, 360, -50, 50);
                                                              "01_Neutron_hits_vs_electron_hits_AV_1n", -180, 180, -50, 50);
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e}",
                                                                           "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                                                                           "#Delta#phi_{n,e} = #phi_{n}^{ECIN/ECOUT} - #phi_{e}^{PCAL} [Deg]",
                                                                           "#Delta#theta_{n,e} = #theta_{n}^{ECIN/ECOUT} - #theta_{e}^{PCAL} [Deg]",
                                                                           directories.Angle_Directory_map["Neutron_Veto_Directory_1n"],
//                                                                           "01_Neutron_hits_vs_electron_hits_Vetoed_1n", -360, 360, -50, 50);
                                                                           "01_Neutron_hits_vs_electron_hits_Vetoed_1n", -180, 180, -50, 50);
//    hPlot2D hdTheta_n_pos_VS_dPhi_n_pos_Protons_BV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,p} vs. #Delta#phi_{n,p}",
//                                                                "'Neutron Hits' vs. Proton Hits - Before Veto",
//                                                                "#Delta#phi_{n,p} = #phi_{n}^{ECIN/ECOUT} - #phi_{p}^{PCAL} [Deg]",
//                                                                "#Delta#theta_{n,p} = #theta_{n}^{ECIN/ECOUT} - #theta_{p}^{PCAL} [Deg]",
//                                                                directories.Angle_Directory_map["Neutron_Veto_Directory_1n"], "02_Neutron_hits_vs_proton_hits_1n",
//                                                                -180, 180, -50, 50);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1n1p)">

    //<editor-fold desc="Neutron veto plots (1n1p)">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n1p = hPlot2D("1n1p", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e}",
                                                                "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                                "#Delta#phi_{n,e} = #phi_{n}^{ECIN/ECOUT} - #phi_{e}^{PCAL} [Deg]",
                                                                "#Delta#theta_{n,e} = #theta_{n}^{ECIN/ECOUT} - #theta_{e}^{PCAL} [Deg]",
                                                                directories.Angle_Directory_map["Neutron_Veto_Directory_1n1p"],
                                                                "01_Neutron_hits_vs_electron_hits_1n1p", -180, 180, -50, 50);
    hPlot2D hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1n1p = hPlot2D("1n1p", "FD", "#Delta#theta_{n,p} vs. #Delta#phi_{n,p}",
                                                              "'Neutron Hits' vs. Proton Hits - Before Veto",
                                                              "#Delta#phi_{n,p} = #phi_{n}^{ECIN/ECOUT} - #phi_{p}^{PCAL} [Deg]",
                                                              "#Delta#theta_{n,p} = #theta_{n}^{ECIN/ECOUT} - #theta_{p}^{PCAL} [Deg]",
                                                              directories.Angle_Directory_map["Neutron_Veto_Directory_1n1p"],
                                                              "02_Neutron_hits_vs_proton_hits_1n1p", -180, 180, -50, 50);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1e2pXy)">

// Phi of leading (p1) and recoil (p2) protons --------------------------------------------------------------------------------------------------------------------------

    //TODO: reorganize proprly

    //<editor-fold desc="Phi of leading (p1) and recoil (p2) protons">
    THStack *sPhi_Proton_1e2pXy = new THStack("#phi_{p} stack (1e2pXy, CD)", "#phi_{p} of Outgoing protons (1e2pXy, CD);#phi_{p} [Deg];");
    TH1D *hPhi_p1_1e2pXy_CD = new TH1D("#phi_{p_{1}} (1e2pXy, CD)", ";#phi_{p_{1}} [Deg];", 100, -180, 180);
    TH1D *hPhi_p2_1e2pXy_CD = new TH1D("#phi_{p_{2}} (1e2pXy, CD)", ";#phi_{p_{2}} [Deg];", 100, -180, 180);
    string hPhi_p1_1e2pXy_CD_Dir = directories.Angle_Directory_map["Phi_Proton_1e2pXy_Directory"];
    string hPhi_p2_1e2pXy_CD_Dir = directories.Angle_Directory_map["Phi_Proton_1e2pXy_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (2p)">

// Theta_p_e_p_tot (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (2p, CD & FD)">
    THStack *sTheta_p_e_p_tot_2p = new THStack("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., 2p)",
                                               "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_p_e_p_tot_2p = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., 2p)",
                                         "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);"
                                         "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];", 100, 0, 180);
    string hTheta_p_e_p_tot_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
    //</editor-fold>

// Theta_q_p (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (2p, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_2p = new THStack("#theta_{#vec{q},#vec{P}_{tot}} (All Int., 2p)",
                                             "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);#theta_{#vec{q},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_q_p_tot_2p = new TH1D("#theta_{#vec{q},#vec{P}_{tot}} (All Int., 2p)",
                                       "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);"
                                       "#theta_{#vec{q},#vec{P}_{tot}} [Deg];", 100, 0, 180);
    string hTheta_q_p_tot_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
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
    string hTheta_q_p_L_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
    string hTheta_q_p_R_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (2p, CD & FD) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    TH2D *hTheta_q_p_L_vs_p_L_q_2p = new TH2D("#theta_{#vec{q},#vec{P}_{1}} vs. r_{1} (All Int., 2p)",
                                              "#theta_{#vec{q},#vec{P}_{1}} vs. r_{1}=|#vec{P_{1}}|/|#vec{q}| (All Int., 2p);r_{1};#theta_{#vec{q},#vec{P}_{1}}",
                                              250, 0, 1.05, 250, 0, 180);
    string hTheta_q_p_L_vs_p_L_q_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
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
    string sTheta_p1_p2_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
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
    string hTheta_p1_p2_vs_W_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
    //</editor-fold>

// Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 10 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 10 (CD & FD)">
    TH2D *hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p = new TH2D("#theta_{p_{1}} vs. #theta_{p_{1}} for #theta_{p_{1},p_{2}}<10#circ (All Int., 2p)",
                                                                 "#theta_{p_{1}} vs. #theta_{p_{2}} for #theta_{p_{1},p_{2}}<10#circ (All Int., 2p);#theta_{p_{2}} [Deg];#theta_{p_{1}} [Deg];",
                                                                 250, 10, 80, 250, 10, 80);
//                                                                 250, 0, 120, 250, 0, 120);
//                                                                 250, 0, 180, 250, 0, 180);
    string hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p_Dir = directories.Angle_Directory_map["Opening_angle_Directory_2p"];
    //</editor-fold>

// Ghost tracks handling (CD only) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p)">
    hPlot2D hTheta_p1_p2_VS_ToF1_ToF2_BC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} BC",
                                                      "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} - Before Cut", "#theta_{p_{1},p_{2}} [Deg]", "ToF_{1}-ToF_{2} [ns]",
                                                      directories.Angle_Directory_map["Ghost_tracks_handling_2p"],
                                                      "01_Theta_p1_p2_VS_ToF1-ToF2_BC_2p", 0, 180, -3, 3, 150, 150);
    hPlot2D hTheta_p1_p2_VS_ToF1_ToF2_AC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC",
                                                      "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} - After Cut", "#theta_{p_{1},p_{2}} [Deg]", "ToF_{1}-ToF_{2} [ns]",
                                                      directories.Angle_Directory_map["Ghost_tracks_handling_2p"],
                                                      "01_Theta_p1_p2_VS_ToF1-ToF2_AC_2p", 0, 180, -3, 3, 150, 150);
    //</editor-fold>

    //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p)">
    hPlot2D hTheta_p1_p2_VS_Pos1_Pos2_BC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} BC",
                                                      "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} - Before Cut", "#theta_{p_{1},p_{2}} [Deg]",
                                                      "Position_{1}-Position_{2} [cm]", directories.Angle_Directory_map["Ghost_tracks_handling_2p"],
                                                      "02_Theta_p1_p2_VS_Pos1-Pos2_BC_2p", 0, 180, 0, 100, 150, 150);
    hPlot2D hTheta_p1_p2_VS_Pos1_Pos2_AC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC",
                                                      "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} - After Cut", "#theta_{p_{1},p_{2}} [Deg]",
                                                      "Position_{1}-Position_{2} [cm]", directories.Angle_Directory_map["Ghost_tracks_handling_2p"],
                                                      "02_Theta_p1_p2_VS_Pos1-Pos2_AC_2p", 0, 180, 0, 100, 150, 150);
    //</editor-fold>

//    //<editor-fold desc="Theta_p1_p2 vs position1-position2 and TOF1-TOF2 plots">
//    hPlot2D hTheta_p1_p2_VS_ToF1_ToF2_BC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{R1,R2} vs. ToF_{1}-ToF_{2}", "#theta_{R1,R2} vs. ToF_{1}-ToF_{2}", "#theta_{R1,R2} [Deg]",
//                                                   "ToF_{1}-ToF_{2} [ns]", directories.Angle_Directory_map["Ghost_tracks_handling_2p"], "01_Theta_R1_R2_VS_ToF1_ToF2_2p",
//                                                   0, 180, -3, 3);
//    hPlot2D hTheta_p1_p2_VS_Pos1_Pos2_BC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{R1,R2} vs. Position_{1}-Position_{2}", "#theta_{R1,R2} vs. Position_{1}-Position_{2}",
//                                                   "#theta_{R1,R2} [Deg]", "Position_{1}-Position_{2} [cm]", directories.Angle_Directory_map["Ghost_tracks_handling_2p"],
//                                                   "02_Theta_R1_R2_VS_Pos1_Pos2_2p", 0, 180, 0, 100);
//    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Q2 histograms
// ======================================================================================================================================================================

    //TODO: confirm units with Adi

    //<editor-fold desc="Q2 histograms">

    //<editor-fold desc="Q2 histograms (no #(e) cut)">
    THStack *sQ2_All_e = new THStack("Q^{2} (no #(e) cut, CD & FD)", "Q^{2} Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_All_e_FD = new TH1D("Q^{2} (no #(e) cut, FD)", "Q^{2} (no #(e) cut, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_All_e_FD_Dir = directories.Q2_Directory_map["Q2_All_e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1e cut)">
    THStack *sQ2_1e_cut = new THStack("Q^{2} (1e Cut ,CD & FD)", "Q^{2} Histogram (1e Cut, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1e_cut_FD = new TH1D("Q^{2} (1e Cut, FD)", "Q^{2} (1e Cut, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1e_cut_FD_Dir = directories.Q2_Directory_map["Q2_1e_cut_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1p)">
    THStack *sQ2_1p = new THStack("Q^{2} (1p, CD & FD)", "Q^{2} (1p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1p_FD = new TH1D("Q^{2} (1p, FD)", "Q^{2} (1p, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1p_FD_Dir = directories.Q2_Directory_map["Q2_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1n)">
    THStack *sQ2_1n = new THStack("Q^{2} (1n, CD & FD)", "Q^{2} (1n, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1n_FD = new TH1D("Q^{2} (1n, FD)", "Q^{2} (1n, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1n_FD_Dir = directories.Q2_Directory_map["Q2_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1e2p)">
    THStack *sQ2_1e2p = new THStack("Q^{2} (1e2p, CD & FD)", "Q^{2} (1e2p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1e2p_FD = new TH1D("Q^{2} (1e2p, FD)", "Q^{2} (1e2p, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_1e2p_FD_Dir = directories.Q2_Directory_map["Q2_1e2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (2p)">
    THStack *sQ2_2p = new THStack("Q^{2} (2p, CD & FD)", "Q^{2} (2p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_2p_FD = new TH1D("Q^{2} (2p, FD)", "Q^{2} (2p, FD);Q^{2} [GeV^{2}];", 100, 0, beamE * 1.1);
    string hQ2_2p_FD_Dir = directories.Q2_Directory_map["Q2_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Energy (E_e) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy (E_e) histograms">

    //<editor-fold desc="E_e plots (1e cut)">
    /* Energy (E_e) histograms (1e cut, CD & FD) */
    THStack *sE_e_1e_cut = new THStack("E_{e} (CD & FD)", "E_{e} Histogram (1e Cut, CD & FD);E_{e} [GeV]");
    TH1D *hE_e_1e_cut_FD = new TH1D("E_{e} (1e Cut)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];

    TH2D *hE_e_VS_Theta_e_All_Int_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1e Cut)",
                                                       "E_{e} vs. #theta_{e} (All Int., 1e Cut);#theta_{e} [Deg];E_{e} [GeV]", 250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];
    //</editor-fold>

    //<editor-fold desc="E_e plots (1p)">
    /* Energy (E_e) histograms (1p, CD & FD) */
    THStack *sE_e_1p_FD = new THStack("E_{e} (1p)", "E_{e} Histogram (1p);E_{e} [GeV]");
    string sE_e_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_stack_Directory"];

    TH1D *hE_e_All_Int_1p_FD = new TH1D("E_{e} (All Int., 1p)", "E_{e} Histogram (All Int., 1p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_QEL_1p_FD = new TH1D("E_{e} (QEL Only, 1p)", "E_{e} Histogram (QEL Only, 1p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_MEC_1p_FD = new TH1D("E_{e} (MEC Only, 1p)", "E_{e} Histogram (MEC Only, 1p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_RES_1p_FD = new TH1D("E_{e} (RES Only, 1p)", "E_{e} Histogram (RES Only, 1p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_DIS_1p_FD = new TH1D("E_{e} (DIS Only, 1p)", "E_{e} Histogram (DIS Only, 1p);E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_All_Int_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_QEL_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_MEC_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_RES_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_DIS_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];

    /* E_e vs. Theta_e (1p, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_1p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1p)", "E_{e} vs. #theta_{e} (All Int., 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                                   250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_1p_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1p)", "E_{e} vs. #theta_{e} (QEL Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_1p_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 1p)", "E_{e} vs. #theta_{e} (MEC Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_1p_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 1p)", "E_{e} vs. #theta_{e} (RES Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_1p_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 1p)", "E_{e} vs. #theta_{e} (DIS Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_QEL_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_MEC_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_RES_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_DIS_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (1p, CD & FD) */
    THStack *sE_e_15_1p_FD = new THStack("E_{e} (1p)", "E_{e} Histogram (1p);E_{e} [GeV]");
    string sE_e_15_1p_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_stack_Directory"];
    string sE_e_15_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_stack_Directory"];

    TH1D *hE_e_15_All_Int_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 1p)", "E_{e} around #theta_{e} = 15#circ (All Int., 1p);E_{e} [GeV]",
                                           100, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 1p)", "E_{e} around #theta_{e} = 15#circ (QEL Only, 1p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, 1p)", "E_{e} around #theta_{e} = 15#circ (MEC Only, 1p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, 1p)", "E_{e} around #theta_{e} = 15#circ (RES Only, 1p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, 1p)", "E_{e} around #theta_{e} = 15#circ (DIS Only, 1p);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    string hE_e_15_All_Int_1p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1p_Directory"];
    string hE_e_15_QEL_1p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1p_Directory"];
    string hE_e_15_MEC_1p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1p_Directory"];
    string hE_e_15_RES_1p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1p_Directory"];
    string hE_e_15_DIS_1p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="E_e plots (1n)">
    /* Energy (E_e) histograms (1n, CD & FD) */
    THStack *sE_e_1n_FD = new THStack("E_{e} (1n)", "E_{e} Histogram (1n);E_{e} [GeV]");
    string sE_e_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_stack_Directory"];

    TH1D *hE_e_All_Int_1n_FD = new TH1D("E_{e} (All Int., 1n)", "E_{e} Histogram (All Int., 1n);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_QEL_1n_FD = new TH1D("E_{e} (QEL Only, 1n)", "E_{e} Histogram (QEL Only, 1n);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_MEC_1n_FD = new TH1D("E_{e} (MEC Only, 1n)", "E_{e} Histogram (MEC Only, 1n);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_RES_1n_FD = new TH1D("E_{e} (RES Only, 1n)", "E_{e} Histogram (RES Only, 1n);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_DIS_1n_FD = new TH1D("E_{e} (DIS Only, 1n)", "E_{e} Histogram (DIS Only, 1n);E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_All_Int_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_QEL_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_MEC_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_RES_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_DIS_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];

    /* E_e vs. Theta_e (1n, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_1n_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1n)", "E_{e} vs. #theta_{e} (All Int., 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                                   250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_1n_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1n)", "E_{e} vs. #theta_{e} (QEL Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_1n_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 1n)", "E_{e} vs. #theta_{e} (MEC Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_1n_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 1n)", "E_{e} vs. #theta_{e} (RES Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_1n_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 1n)", "E_{e} vs. #theta_{e} (DIS Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_QEL_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_MEC_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_RES_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_DIS_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (1n, CD & FD) */
    THStack *sE_e_15_1n_FD = new THStack("E_{e} (1n)", "E_{e} Histogram (1n);E_{e} [GeV]");
    string sE_e_15_1n_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_stack_Directory"];
    string sE_e_15_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_stack_Directory"];

    TH1D *hE_e_15_All_Int_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 1n)", "E_{e} around #theta_{e} = 15#circ (All Int., 1n);E_{e} [GeV]",
                                           100, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 1n)", "E_{e} around #theta_{e} = 15#circ (QEL Only, 1n);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, 1n)", "E_{e} around #theta_{e} = 15#circ (MEC Only, 1n);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, 1n)", "E_{e} around #theta_{e} = 15#circ (RES Only, 1n);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, 1n)", "E_{e} around #theta_{e} = 15#circ (DIS Only, 1n);E_{e} [GeV]",
                                       100, 0, beamE * 1.1);
    string hE_e_15_All_Int_1n_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1n_Directory"];
    string hE_e_15_QEL_1n_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1n_Directory"];
    string hE_e_15_MEC_1n_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1n_Directory"];
    string hE_e_15_RES_1n_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1n_Directory"];
    string hE_e_15_DIS_1n_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="E_e plots (2p)">
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
    TH2D *hE_e_VS_Theta_e_All_Int_2p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 2p)", "E_{e} vs. #theta_{e} (All Int., 2p);#theta_{e} [Deg];E_{e} [GeV]",
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

    //</editor-fold>

// ======================================================================================================================================================================
// Energy Transfer (ET) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy Transfer (ET) histograms">

    string tET_All_Ang = "Energy transfer #omega for every #theta_{e}", tET15 = "Energy transfer #omega Around #theta_{e} = 15#circ";

    //<editor-fold desc="ET for every theta_e (1p)">
    THStack *sET_All_Ang_All_Int_1p_FD = new THStack("#omega for all #theta_{e} (1p)", "Energy transfer #omega for all #theta_{e} (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1p_Directory"];

    TH1D *hET_All_Ang_All_Int_1p_FD = new TH1D("#omega for all #theta_{e} (All Int., 1p)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1p_Directory"];
    string hET_All_Ang_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1p_Directory"];
    string hET_All_Ang_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1p_Directory"];
    string hET_All_Ang_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1p_Directory"];
    string hET_All_Ang_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (1n)">
    THStack *sET_All_Ang_All_Int_1n_FD = new THStack("#omega for all #theta_{e} (1n)", "Energy transfer #omega for all #theta_{e} (1n);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1n_Directory"];

    TH1D *hET_All_Ang_All_Int_1n_FD = new TH1D("#omega for all #theta_{e} (All Int., 1n)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1n_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1n_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1n_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1n_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1n_Directory"];
    string hET_All_Ang_QEL_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1n_Directory"];
    string hET_All_Ang_MEC_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1n_Directory"];
    string hET_All_Ang_RES_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1n_Directory"];
    string hET_All_Ang_DIS_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (2p)">
    THStack *sET_All_Ang_All_Int_2p_FD = new THStack("#omega for all #theta_{e} (2p)", "Energy transfer #omega for all #theta_{e} (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_2p_Directory"];

    TH1D *hET_All_Ang_All_Int_2p_FD = new TH1D("#omega for all #theta_{e} (All Int., 2p)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_2p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_2p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_2p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_2p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_2p_Directory"];
    string hET_All_Ang_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_2p_Directory"];
    string hET_All_Ang_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_2p_Directory"];
    string hET_All_Ang_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_2p_Directory"];
    string hET_All_Ang_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (1p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_1p_FD = new THStack("ET around #theta_{e} = 15#circ (1p)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_1p_Directory"];

    TH1D *hET15_All_Int_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (All Int., 1p)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_QEL_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_MEC_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_RES_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_DIS_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_1p_Directory"];
    string hET15_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_1p_Directory"];
    string hET15_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_1p_Directory"];
    string hET15_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_1p_Directory"];
    string hET15_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (1n)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_1n_FD = new THStack("ET around #theta_{e} = 15#circ (1n)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (1n);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_1n_Directory"];

    TH1D *hET15_All_Int_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (All Int., 1n)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_QEL_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_MEC_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_RES_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_DIS_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1n);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hET15_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_1n_Directory"];
    string hET15_QEL_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_1n_Directory"];
    string hET15_MEC_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_1n_Directory"];
    string hET15_RES_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_1n_Directory"];
    string hET15_DIS_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (2p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_2p_FD = new THStack("#omega around #theta_{e} = 15#circ (2p)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_2p_Directory"];

    TH1D *hET15_All_Int_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., 2p)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_QEL_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_MEC_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_RES_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hET15_DIS_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]", 100, 0, beamE * 1.1);
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

    //<editor-fold desc="Ecal plots (1p)">
    THStack *sEcal_1p = new THStack("E_{cal} Reconstruction (1p)", "E_{cal} Reconstruction (1p);E_{cal} = E_{e} + T_{p} [GeV]");
    string sEcal_1p_Dir = directories.Ecal_Directory_map["Ecal_stack_1p_Directory"];

    TH1D *hEcal_All_Int_1p = new TH1D("E_{cal} rec. (All Int., 1p)", "E_{cal} Reconstruction (All Int., 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                      100, 0, beamE * 1.35);
    TH1D *hEcal_QEL_1p = new TH1D("E_{cal} rec. (QEL only, 1p)", "E_{cal} Reconstruction (QEL only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_MEC_1p = new TH1D("E_{cal} rec. (MEC only, 1p)", "E_{cal} Reconstruction (MEC only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_RES_1p = new TH1D("E_{cal} rec. (RES only, 1p)", "E_{cal} Reconstruction (RES only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_DIS_1p = new TH1D("E_{cal} rec. (DIS only, 1p)", "E_{cal} Reconstruction (DIS only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  100, 0, beamE * 1.35);
    string hEcal_All_Int_1p_Dir = directories.Ecal_Directory_map["Ecal_All_Int_1p_Directory"];
    string hEcal_QEL_1p_Dir = directories.Ecal_Directory_map["Ecal_QEL_1p_Directory"];
    string hEcal_MEC_1p_Dir = directories.Ecal_Directory_map["Ecal_MEC_1p_Directory"];
    string hEcal_RES_1p_Dir = directories.Ecal_Directory_map["Ecal_RES_1p_Directory"];
    string hEcal_DIS_1p_Dir = directories.Ecal_Directory_map["Ecal_DIS_1p_Directory"];

    //<editor-fold desc="Chi2 plots for Ecal>Ebeam (1p)">
    THStack *sChi2_Electron_1e1p = new THStack("#chi^{2}_{e} (1e1p, CD & FD)", "#chi^{2}_{e} Histogram (1e1p, CD & FD);#chi^{2}_{e};");
    THStack *sChi2_Proton_1e1p = new THStack("#chi^{2}_{p} (1e1p, CD & FD)", "#chi^{2}_{p} Histogram (1e1p, CD & FD);#chi^{2}_{p};");

    TH1D *hChi2_Electron_Ecal_test_1p = new TH1D("#chi^{2}_{e} for E_{cal}>E_{beam} (1p)", ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                            " [GeV] (1p);#chi^{2}_{e};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_Ecal_test_1p = new TH1D("#chi^{2}_{p} for E_{cal}>E_{beam} (1p)", ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                          " [GeV] (1p);#chi^{2}_{p};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_Ecal_test_1p_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_1p_Directory"];
    string hChi2_Proton_Ecal_test_1p_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_1p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal plots (1n)">
    THStack *sEcal_1n = new THStack("E_{cal} Reconstruction (1n)", "E_{cal} Reconstruction (1n);E_{cal} = E_{e} + T_{n} [GeV]");
    string sEcal_1n_Dir = directories.Ecal_Directory_map["Ecal_stack_1n_Directory"];

    TH1D *hEcal_All_Int_1n = new TH1D("E_{cal} rec. (All Int., 1n)", "E_{cal} Reconstruction (All Int., 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                      100, 0, beamE * 1.35);
    TH1D *hEcal_QEL_1n = new TH1D("E_{cal} rec. (QEL only, 1n)", "E_{cal} Reconstruction (QEL only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_MEC_1n = new TH1D("E_{cal} rec. (MEC only, 1n)", "E_{cal} Reconstruction (MEC only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_RES_1n = new TH1D("E_{cal} rec. (RES only, 1n)", "E_{cal} Reconstruction (RES only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_DIS_1n = new TH1D("E_{cal} rec. (DIS only, 1n)", "E_{cal} Reconstruction (DIS only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  100, 0, beamE * 1.35);
    string hEcal_All_Int_1n_Dir = directories.Ecal_Directory_map["Ecal_All_Int_1n_Directory"];
    string hEcal_QEL_1n_Dir = directories.Ecal_Directory_map["Ecal_QEL_1n_Directory"];
    string hEcal_MEC_1n_Dir = directories.Ecal_Directory_map["Ecal_MEC_1n_Directory"];
    string hEcal_RES_1n_Dir = directories.Ecal_Directory_map["Ecal_RES_1n_Directory"];
    string hEcal_DIS_1n_Dir = directories.Ecal_Directory_map["Ecal_DIS_1n_Directory"];

    //<editor-fold desc="Chi2 plots for Ecal>Ebeam (1n)">
    THStack *sChi2_Electron_1e1n = new THStack("#chi^{2}_{e} (1e1n, CD & FD)", "#chi^{2}_{e} Histogram (1e1n, CD & FD);#chi^{2}_{e};");
    THStack *sChi2_Proton_1e1n = new THStack("#chi^{2}_{p} (1e1n, CD & FD)", "#chi^{2}_{p} Histogram (1e1n, CD & FD);#chi^{2}_{p};");

    TH1D *hChi2_Electron_Ecal_test_1n = new TH1D("#chi^{2}_{e} for E_{cal}>E_{beam} (1n)", ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                            " [GeV] (1n);#chi^{2}_{e};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_Ecal_test_1n = new TH1D("#chi^{2}_{p} for E_{cal}>E_{beam} (1n)", ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                          " [GeV] (1n);#chi^{2}_{p};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_Ecal_test_1n_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_1n_Directory"];
    string hChi2_Proton_Ecal_test_1n_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_1n_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal plots (2p)">
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
    THStack *sChi2_Electron_1e2p = new THStack("#chi^{2}_{e} (1e2p, CD & FD)", "#chi^{2}_{e} Histogram (1e2p, CD & FD);#chi^{2}_{e};");
    THStack *sChi2_Proton_1e2p = new THStack("#chi^{2}_{p} (1e2p, CD & FD)", "#chi^{2}_{p} Histogram (1e2p, CD & FD);#chi^{2}_{p};");

    TH1D *hChi2_Electron_Ecal_test_2p = new TH1D("#chi^{2}_{e} for E_{cal}>E_{beam} (2p)", ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                            " [GeV] (2p);#chi^{2}_{e};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_Ecal_test_2p = new TH1D("#chi^{2}_{p} for E_{cal}>E_{beam} (2p)", ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) +
                                                                                          " [GeV] (2p);#chi^{2}_{p};").c_str(), 100, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_Ecal_test_2p_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_2p_Directory"];
    string hChi2_Proton_Ecal_test_2p_Dir = directories.Ecal_Directory_map["Chi2_Ecal_test_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (1p)">
    TH2D *hEcal_vs_dAlpha_T_1p = new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1p)",
                                          "E_{cal} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [Deg];E_{cal} [GeV];",
                                          250, 0, 180, 250, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_1p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dAlpha_T (1n)">
    TH2D *hEcal_vs_dAlpha_T_1n = new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1n)",
                                          "E_{cal} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [Deg];E_{cal} [GeV];",
                                          250, 0, 180, 250, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_1n_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dAlpha_T (2p)">
    TH2D *hEcal_vs_dAlpha_T_L_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                            "E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                            250, 0, 180, 250, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                              "E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                              250, 0, 180, 250, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    string hEcal_vs_dAlpha_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (1p)">
    TH2D *hEcal_vs_dP_T_1p = new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1p)",
                                      "E_{cal} vs. #deltaP_{T} (All Int., 1p);#deltaP_{T} [GeV/c];E_{cal} [GeV];", 250, 0, dP_T_boundary, 250, 0, beamE * 1.35);
    string hEcal_vs_dP_T_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dP_T (1n)">
    TH2D *hEcal_vs_dP_T_1n = new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1n)",
                                      "E_{cal} vs. #deltaP_{T} (All Int., 1n);#deltaP_{T} [GeV/c];E_{cal} [GeV];", 250, 0, dP_T_boundary, 250, 0, beamE * 1.35);
    string hEcal_vs_dP_T_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dP_T (2p)">
    TH2D *hEcal_vs_dP_T_L_2p = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., 2p)",
                                        "E_{cal} vs. #deltaP_{T,L} (All Int., 2p);#deltaP_{T,L} [GeV];E_{cal} [GeV];", 250, 0, dP_T_boundary, 250, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_2p = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., 2p)",
                                          "E_{cal} vs. #deltaP_{T,tot} (All Int., 2p);#deltaP_{T,tot} [GeV];E_{cal} [GeV];", 250, 0, dP_T_boundary, 250, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    string hEcal_vs_dP_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_transverse_variables_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Transverse variables histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Transverse variables histograms">

    //<editor-fold desc="TV plots (1p)">
    THStack *sdP_T_1p = new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1p);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_1p = new TH1D("#deltaP_{T} (1p)", "#deltaP_{T} histogram (1p);#deltaP_{T} = |#vec{p}_{T,e} + #vec{p}_{T,p}| [GeV/c]", 100, 0, dP_T_boundary);
    string hdP_T_1p_Dir = directories.TVariables_Directory_map["dP_T_1p_Directory"];

    THStack *sdAlpha_T_1p = new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_1p = new TH1D("#delta#alpha_{T} (1p)", "#delta#alpha_{T} histogram (1p);#delta#alpha_{T} [Deg]", 100, 0, 180);
    string hdAlpha_T_1p_Dir = directories.TVariables_Directory_map["dAlpha_T_1p_Directory"];

    THStack *sdPhi_T_1p = new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_1p = new TH1D("#delta#phi_{T} (1p)", "#delta#phi_{T} histogram (1p);#delta#phi_{T} [Deg]", 100, 0, 180);
    string hdPhi_T_1p_Dir = directories.TVariables_Directory_map["dPhi_T_1p_Directory"];

    TH2D *hdP_T_vs_dAlpha_T_1p = new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p)",
                                          "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [Deg];#deltaP_{T} [GeV/c];",
                                          250, 0, 180, 250, 0, dP_T_boundary);
    string hdP_T_vs_dAlpha_T_1p_Dir = directories.TVariables_Directory_map["dP_T_vs_dAlpha_T_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="TV plots (1n)">
    THStack *sdP_T_1n = new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1n)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1n);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_1n = new TH1D("#deltaP_{T} (1n)", "#deltaP_{T} histogram (1n);#deltaP_{T} = |#vec{p}_{T,e} + #vec{p}_{T,n}| [GeV/c]", 100, 0, dP_T_boundary);
    string hdP_T_1n_Dir = directories.TVariables_Directory_map["dP_T_1n_Directory"];

    THStack *sdAlpha_T_1n = new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1n)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1n);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_1n = new TH1D("#delta#alpha_{T} (1n)", "#delta#alpha_{T} histogram (1n);#delta#alpha_{T} [Deg]", 100, 0, 180);
    string hdAlpha_T_1n_Dir = directories.TVariables_Directory_map["dAlpha_T_1n_Directory"];

    THStack *sdPhi_T_1n = new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1n)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1n);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_1n = new TH1D("#delta#phi_{T} (1n)", "#delta#phi_{T} histogram (1n);#delta#phi_{T} [Deg]", 100, 0, 180);
    string hdPhi_T_1n_Dir = directories.TVariables_Directory_map["dPhi_T_1n_Directory"];

    TH2D *hdP_T_vs_dAlpha_T_1n = new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n)",
                                          "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [Deg];#deltaP_{T} [GeV/c];",
                                          250, 0, 180, 250, 0, dP_T_boundary);
    string hdP_T_vs_dAlpha_T_1n_Dir = directories.TVariables_Directory_map["dP_T_vs_dAlpha_T_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="TV plots (2p)">
    THStack *sdP_T_2p = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (2p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (2p);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_2p = new TH1D("#deltaP_{T,L} (2p)", "#deltaP_{T,L} by Leading Proton (2p);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,1}| [GeV/c]", 100, 0,
                                dP_T_boundary);
    TH1D *hdP_T_tot_2p = new TH1D("#deltaP_{T,tot} (2p)",
                                  "#deltaP_{T,tot} by Momentum Sum (2p);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,1} + #vec{p}_{T,2}| [GeV/c]", 100, 0,
                                  dP_T_boundary);
    string hdP_T_L_2p_Dir = directories.TVariables_Directory_map["dP_T_2p_Directory"];
    string hdP_T_tot_2p_Dir = directories.TVariables_Directory_map["dP_T_2p_Directory"];

    THStack *sdAlpha_T_2p = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (2p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_2p = new TH1D("#delta#alpha_{T,L} (2p)", "#delta#alpha_{T,L} by Leading Proton (2p);#delta#alpha_{T,L} [Deg]", 100, 0, 180);
    TH1D *hdAlpha_T_tot_2p = new TH1D("#delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,tot} by Momentum Sum (2p);#delta#alpha_{T,tot} [Deg]", 100, 0, 180);
    string hdAlpha_T_L_2p_Dir = directories.TVariables_Directory_map["dAlpha_T_2p_Directory"];
    string hdAlpha_T_tot_2p_Dir = directories.TVariables_Directory_map["dAlpha_T_2p_Directory"];

    THStack *sdPhi_T_2p = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (2p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (2p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_2p = new TH1D("#delta#phi_{T,L} (2p)", "#delta#phi_{T,L} by Leading Proton (2p);#delta#phi_{T,L} [Deg]", 100, 0, 180);
    TH1D *hdPhi_T_tot_2p = new TH1D("#delta#phi_{T,tot} (2p)", "#delta#phi_{T,tot} by Momentum Sum (2p);#delta#phi_{T,tot} [Deg]", 100, 0, 180);
    string hdPhi_T_L_2p_Dir = directories.TVariables_Directory_map["dPhi_T_2p_Directory"];
    string hdPhi_T_tot_2p_Dir = directories.TVariables_Directory_map["dPhi_T_2p_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_2p = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                              "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                              250, 0, 180, 250, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_2p = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                                  "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                  250, 0, 180, 250, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_2p_Dir = directories.TVariables_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_2p_Dir = directories.TVariables_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Efficiency histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Efficiency histograms">

    //<editor-fold desc="Efficiency plots (1e cut)">

    //<editor-fold desc="Truth level momentum plots (1e cut)">
    hPlot1D hP_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "01_P_e_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "01_P_e_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "02_P_n_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "02_P_n_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "03_P_p_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "03_P_p_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "04_P_piplus_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "04_P_piplus_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "05_P_piminus_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "05_P_piminus_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "06_P_pi0_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "06_P_pi0_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "07_P_ph_AC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1e_cut"], "07_P_ph_BC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1e cut)">
    hPlot1D hTheta_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "01_Theta_e_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "01_Theta_e_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "02_Theta_n_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "02_Theta_n_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "03_Theta_p_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "03_Theta_p_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "04_Theta_piplus_AC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "04_Theta_piplus_BC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "05_Theta_piminus_AC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "05_Theta_piminus_BC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "06_Theta_pi0_AC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "06_Theta_pi0_BC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "07_Theta_ph_AC_truth_1e_cut",
                                                Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1e_cut"], "07_Theta_ph_BC_truth_1e_cut",
                                                Theta_lboundary_FD, Theta_uboundary_FD);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1e cut)">
    hPlot1D hPhi_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "01_Phi_e_AC_truth_1e_cut",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "01_Phi_e_BC_truth_1e_cut",
                                             Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "02_Phi_n_AC_truth_1e_cut",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "02_Phi_n_BC_truth_1e_cut",
                                             Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "03_Phi_p_AC_truth_1e_cut",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "03_Phi_p_BC_truth_1e_cut",
                                             Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "04_Phi_piplus_AC_truth_1e_cut",
                                               Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "04_Phi_piplus_BC_truth_1e_cut",
                                               Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "05_Phi_piminus_AC_truth_1e_cut",
                                               Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "05_Phi_piminus_BC_truth_1e_cut",
                                               Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "06_Phi_pi0_AC_truth_1e_cut",
                                               Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "06_Phi_pi0_BC_truth_1e_cut",
                                               Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "07_Phi_ph_AC_truth_1e_cut",
                                              Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1e_cut"], "07_Phi_ph_BC_truth_1e_cut",
                                              Phi_lboundary_FD, Phi_uboundary_FD);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (1p)">

    //<editor-fold desc="Truth level momentum plots (1p)">
    hPlot1D hP_e_AC_truth_1p = hPlot1D("1p", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "01_P_e_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_e_BC_truth_1p = hPlot1D("1p", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "01_P_e_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_n_AC_truth_1p = hPlot1D("1p", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "02_P_n_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_BC_truth_1p = hPlot1D("1p", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "02_P_n_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_p_AC_truth_1p = hPlot1D("1p", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "03a_P_p_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BC_truth_1p = hPlot1D("1p", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "03a_P_p_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_AC_truth_1p_FD = hPlot1D("1p", "FD", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "03b_P_p_AC_truth_1p_FD",
                                          Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BC_truth_1p_FD = hPlot1D("1p", "FD", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "03b_P_p_BC_truth_1p_FD",
                                          Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pip_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "04_P_piplus_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pip_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "04_P_piplus_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pim_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "05_P_piminus_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pim_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "05_P_piminus_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "06_P_pi0_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "06_P_pi0_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_ph_AC_truth_1p = hPlot1D("1p", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "07a_P_ph_AC_truth_1p",
                                        Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BC_truth_1p = hPlot1D("1p", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "07a_P_ph_BC_truth_1p",
                                        Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "07b_P_ph_AC_truth_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1p"], "07b_P_ph_BC_truth_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1p)">
    hPlot1D hTheta_e_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "01_Theta_e_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_e_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "01_Theta_e_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_n_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "02_Theta_n_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_n_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "02_Theta_n_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_p_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "03a_Theta_p_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_p_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "03a_Theta_p_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_p_AC_truth_1p_FD = hPlot1D("1p", "FD", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "03b_Theta_p_AC_truth_1p_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_p_BC_truth_1p_FD = hPlot1D("1p", "FD", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "03b_Theta_p_BC_truth_1p_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pip_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "04_Theta_piplus_AC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pip_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "04_Theta_piplus_BC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pim_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "05_Theta_piminus_AC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pim_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "05_Theta_piminus_BC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "06_Theta_pi0_AC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "06_Theta_pi0_BC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_ph_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC", "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "07a_Theta_ph_AC_truth_1p",
                                            Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC", "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "07a_Theta_ph_BC_truth_1p",
                                            Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "07b_Theta_ph_AC_truth_1p_FD",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1p"], "07b_Theta_ph_BC_truth_1p_FD",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1p)">
    hPlot1D hPhi_e_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "01_Phi_e_AC_truth_1p",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_e_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "01_Phi_e_BC_truth_1p",
                                         Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_n_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "02_Phi_n_AC_truth_1p",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_n_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "02_Phi_n_BC_truth_1p",
                                         Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_p_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "03a_Phi_p_AC_truth_1p",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_p_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "03a_Phi_p_BC_truth_1p",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_p_AC_truth_1p_FD = hPlot1D("1p", "FD", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "03b_Phi_p_AC_truth_1p_FD",
                                            Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_p_BC_truth_1p_FD = hPlot1D("1p", "FD", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "03b_Phi_p_BC_truth_1p_FD",
                                            Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pip_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "04_Phi_piplus_AC_truth_1p",
                                           Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pip_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "04_Phi_piplus_BC_truth_1p",
                                           Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pim_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "05_Phi_piminus_AC_truth_1p",
                                           Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pim_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "05_Phi_piminus_BC_truth_1p",
                                           Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "06_Phi_pi0_AC_truth_1p",
                                           Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "06_Phi_pi0_BC_truth_1p",
                                           Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_ph_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "07a_Phi_ph_AC_truth_1p",
                                          Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "07a_Phi_ph_BC_truth_1p",
                                          Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "07b_Phi_ph_AC_truth_1p_FD",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1p"], "07b_Phi_ph_BC_truth_1p_FD",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (1n)">

    //<editor-fold desc="Truth level momentum plots (1n)">
    hPlot1D hP_e_AC_truth_1n = hPlot1D("1n", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "01_P_e_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_e_BC_truth_1n = hPlot1D("1n", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "01_P_e_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_n_AC_truth_1n = hPlot1D("1n", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "02a_P_n_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_BC_truth_1n = hPlot1D("1n", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "02a_P_n_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "02b_P_n_AC_truth_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_n_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "02b_P_n_BC_truth_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_p_AC_truth_1n = hPlot1D("1n", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "03_P_p_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_p_BC_truth_1n = hPlot1D("1n", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "03_P_p_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pip_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "04_P_piplus_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pip_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "04_P_piplus_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pim_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "05_P_piminus_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pim_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "05_P_piminus_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "06_P_pi0_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "06_P_pi0_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary);

    hPlot1D hP_ph_AC_truth_1n = hPlot1D("1n", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "07a_P_ph_AC_truth_1n",
                                        Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BC_truth_1n = hPlot1D("1n", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "07a_P_ph_BC_truth_1n",
                                        Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "07b_P_ph_AC_truth_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary);
    hPlot1D hP_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_Eff_and_ACorr_Directory_1n"], "07b_P_ph_BC_truth_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1n)">
    hPlot1D hTheta_e_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "01_Theta_e_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_e_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "01_Theta_e_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_n_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "02a_Theta_n_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_n_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "02a_Theta_n_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_n_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "02b_Theta_n_AC_truth_1n_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_n_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "02b_Theta_n_BC_truth_1n_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_p_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "03_Theta_p_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_p_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "03_Theta_p_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pip_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "04_Theta_piplus_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pip_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "04_Theta_piplus_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pim_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "05_Theta_piminus_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pim_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "05_Theta_piminus_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "06_Theta_pi0_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "06_Theta_pi0_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD);

    hPlot1D hTheta_ph_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC", "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "07a_Theta_ph_AC_truth_1n",
                                            Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC", "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "07a_Theta_ph_BC_truth_1n",
                                            Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "07b_Theta_ph_AC_truth_1n_FD",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    hPlot1D hTheta_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_Directory_1n"], "07b_Theta_ph_BC_truth_1n_FD",
                                               Theta_lboundary_FD, Theta_uboundary_FD);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1n)">
    hPlot1D hPhi_e_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "01_Phi_e_AC_truth_1n",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_e_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "01_Phi_e_BC_truth_1n",
                                         Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_n_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "02a_Phi_n_AC_truth_1n",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_n_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "02a_Phi_n_BC_truth_1n",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_n_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "02b_Phi_n_AC_truth_1n_FD",
                                            Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_n_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "02b_Phi_n_BC_truth_1n_FD",
                                            Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_p_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "03_Phi_p_AC_truth_1n",
                                         Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_p_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "03_Phi_p_BC_truth_1n",
                                         Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pip_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "04_Phi_piplus_AC_truth_1n",
                                           Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pip_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "04_Phi_piplus_BC_truth_1n",
                                           Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pim_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "05_Phi_piminus_AC_truth_1n",
                                           Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pim_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "05_Phi_piminus_BC_truth_1n",
                                           Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "06_Phi_pi0_AC_truth_1n",
                                           Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "06_Phi_pi0_BC_truth_1n",
                                           Phi_lboundary_FD, Phi_uboundary_FD);

    hPlot1D hPhi_ph_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "07a_Phi_ph_AC_truth_1n",
                                          Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "07a_Phi_hp_BC_truth_1n",
                                          Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "07b_Phi_ph_AC_truth_1n_FD",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    hPlot1D hPhi_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_Directory_1n"], "07b_Phi_hp_BC_truth_1n_FD",
                                             Phi_lboundary_FD, Phi_uboundary_FD);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code execution                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Code execution">
    cout << "Reading target parameter files\n\n";

    clas12ana clasAna;

    if (apply_cuts) {
        /* Read in target parameter files */
        if (!apply_chi2_cuts_1e_cut) {
            clasAna.readInputParam((CutsDirectory + "ana.par").c_str());
        } else if (apply_chi2_cuts_1e_cut) {
            cout << "Loading fitted pid cuts...\n\n";
            clasAna.readInputParam((CutsDirectory + "Fitted_PID_Cuts_-_" + SampleName + ".par").c_str()); // load sample-appropreate cuts file from CutsDirectory

            /* Overwriting PID cuts according to SampleName */
            Chi2_Proton_cuts_CD.SetCutPram(clasAna.GetPidCutMean(2212, "CD"), -clasAna.GetPidCutSigma(2212, "CD"), clasAna.GetPidCutSigma(2212, "CD"));
            Chi2_Proton_cuts_FD.SetCutPram(clasAna.GetPidCutMean(2212, "FD"), -clasAna.GetPidCutSigma(2212, "FD"), clasAna.GetPidCutSigma(2212, "FD"));
            Chi2_piplus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(211, "CD"), -clasAna.GetPidCutSigma(211, "CD"), clasAna.GetPidCutSigma(211, "CD"));
            Chi2_piplus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(211, "FD"), -clasAna.GetPidCutSigma(211, "FD"), clasAna.GetPidCutSigma(211, "FD"));
            Chi2_piminus_cuts_CD.SetCutPram(clasAna.GetPidCutMean(-211, "CD"), -clasAna.GetPidCutSigma(-211, "CD"), clasAna.GetPidCutSigma(-211, "CD"));
            Chi2_piminus_cuts_FD.SetCutPram(clasAna.GetPidCutMean(-211, "FD"), -clasAna.GetPidCutSigma(-211, "FD"), clasAna.GetPidCutSigma(-211, "FD"));

            clasAna.setPidCuts(); // making f_pidCuts = ture
        }

//        clasAna.readEcalPar((CutsDirectory + "ecal.par").c_str()); // OLD!!!

        // Cuts on electrons only:
        if (apply_ECAL_SF_cuts) { // making f_ecalSFCuts = ture
            //todo: ask justin what are these cuts:
            //todo: ask justin for these cuts for LH2 and C12 (and other elements)
//        clasAna.readEcalPar((CutsDirectory + "ecal.par").c_str()); // OLD!!!
//        clasAna.readEcalSFPar((CutsDirectory + "ecal.par").c_str());
            clasAna.readEcalSFPar((CutsDirectory + "paramsSF_40Ca_x2.dat").c_str());
            //TODO: RECHECK WHAT ARE THE CUTS HERE:
            SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.248125, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());

            clasAna.setEcalSFCuts();
        }

        if (apply_ECAL_P_cuts) { // making f_ecalSFCuts = ture

            //todo: ask justin what are these cuts:
            //todo: ask justin for these cuts for LH2 and C12 (and other elements)
//        clasAna.readEcalPar((CutsDirectory + "ecal.par").c_str()); // OLD!!!
//        clasAna.readEcalSFPar((CutsDirectory + "ecal.par").c_str());
            clasAna.readEcalPPar((CutsDirectory + "paramsPI_40Ca_x2.dat").c_str());

//            SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.248125, clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut());
            clasAna.setEcalPCuts();
        }

        if (apply_ECAL_fiducial_cuts) { // making f_ecalEdgeCuts = ture (ECAL fiducial cuts)
            PCAL_edge_cuts = DSCuts("PCAL edge", "FD", "Electron", "1e cut", 0, clasAna.getEcalEdgeCuts());
            clasAna.setEcalEdgeCuts();
        }

        if (apply_Nphe_cut) { // making f_NpheCuts = ture (HTCC cuts)
            Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, clasAna.getNpheCuts());
            clasAna.setNpheCuts();
        }

        // Cuts on all particles:
        if (apply_Vz_cuts) {
            clasAna.setVertexCuts(); // making f_vertexCuts = ture
            clasAna.setVzcuts(Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut()); // setting Vz cuts for all (charged?) particles
        }

        // Cuts on charged particles:
        if (apply_DC_fiducial_cut) { // making f_DCEdgeCuts = ture (DC fiducial cuts?)
            DC_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, clasAna.getDCEdgeCuts());
            clasAna.setDCEdgeCuts();
        }

        if (apply_dVz_cuts) {
            clasAna.setVertexCorrCuts(); // making f_corr_vertexCuts = ture
            clasAna.setVertexCorrCuts(dVz_cuts.GetLowerCut(), dVz_cuts.GetUpperCut()); // setting dVz cuts?
        }

        if (!apply_neutron_Beta_Fit) {
            /* Setting neutron momentum cut before beta fit (i.e., no cut!) */
            n_momentum_cuts_ABF_FD_n_from_ph = DSCuts("Momentum_cuts_ECAL", "FD-ECAL", "Neutron", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_n_from_ph_apprax = DSCuts("Momentum_cuts_ECAL_apprax", "FD-ECAL_apprax", "Neutron", "", 0, n_mom_th.GetLowerCut(), 9999);

            n_momentum_cuts_ABF_FD_All_neutrals = DSCuts("Momentum_cuts_All_neutrals", "FD-ECAL", "", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_All_neutrals_noPDG0 = DSCuts("Momentum_cuts_All_neutrals_noPDG0", "FD-ECAL", "", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_noPCAL = DSCuts("Momentum_cuts_noPCAL", "FD-noPCAL", "", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_noPCAL_noPDG0 = DSCuts("Momentum_cuts_noPCAL_noPDG0", "FD-noPCAL", "", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_noPCAL_wECIN = DSCuts("Momentum_cuts_ECIN", "FD-ECIN", "Neutron", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_noPCAL_wECIN_noPDG0 = DSCuts("Momentum_cuts_ECIN_noPDG0", "FD-ECIN", "", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT = DSCuts("Momentum_cuts_ECOUT", "FD-ECOUT", "", "", 0, n_mom_th.GetLowerCut(), 9999);
            n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT_noPDG0 = DSCuts("Momentum_cuts_ECOUT_noPDG0", "FD-ECOUT", "", "", 0, n_mom_th.GetLowerCut(), 9999);

            /* Setting neutron beta cut */
            Beta_cut_ABF_FD_n_from_ph = DSCuts("Beta_cut_ECAL", "FD-ECAL", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_FD_n_from_ph_apprax = DSCuts("Beta_cut_ECAL_apprax", "FD-ECAL_apprax", "", "1n", 1, -9999, 9999);

            Beta_cut_ABF_All_FD_neutrals = DSCuts("Beta_cut_All_FD_neutrals", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_FD_neutrals_noPDG0 = DSCuts("Beta_cut_FD_neutrals_noPDG0", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_All_FD_neutrals_noPCAL = DSCuts("Beta_cut_All_FD_neutrals_noPCAL", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL = DSCuts("Beta_cut_FD_neutrals_noPDG0_noPCAL", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_All_FD_neutrals_noPCAL_wECIN = DSCuts("Beta_cut_All_FD_neutrals_noPCAL_wECIN", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_wECIN = DSCuts("Beta_cut_FD_neutrals_noPDG0_noPCAL_wECIN", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_All_FD_neutrals_noPCAL_noECIN_wECOUT = DSCuts("Beta_cut_All_FD_neutrals_noPCAL_noECIN_wECOUT", "FD", "", "1n", 1, -9999, 9999);
            Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT = DSCuts("Beta_cut_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT", "FD", "", "1n", 1, -9999, 9999);
        } else if (apply_neutron_Beta_Fit) {
            cout << "Loading fitted Beta cuts...\n\n";
            clasAna.readInputParam((CutsDirectory + "Fitted_n_Mom_Cuts_-_" + SampleName + ".par").c_str()); // load sample-appropreate cuts file from CutsDirectory

            /* Setting cuts after beta fit */
            n_mom_th.SetUpperCut(clasAna.getNeutronMomentumCut());
            TL_n_mom_cuts.SetUpperCut(clasAna.getNeutronMomentumCut());
            Beta_cut.SetUpperCut(clasAna.getNeutralBetaCut());
            Beta_cut.SetMean(clasAna.getNeutralBetaCutMean());
//            Beta_cut.SetMean(1.);
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
    TVector3 Pv_3v(0, 0, beamE);
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Counting variable definitions
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Counting variable definitions">
    int num_of_events = 0, num_of_events_without_any_e = 0, num_of_events_with_any_e = 0;
    int num_of_QEL_events = 0, num_of_MEC_events = 0, num_of_RES_events = 0, num_of_DIS_events = 0;

    int num_of_events_with_at_least_1e = 0, num_of_events_with_exactly_1e = 0, num_of_events_with_exactly_1e_from_file = 0, num_of_events_more_then_1e = 0;

    int num_of_events_1e1p_all = 0, num_of_events_1e2p_all = 0, num_of_events_1e2p_all_woFDphotons = 0;
    int num_of_events_with_1e1p = 0, num_of_events_with_1e2p = 0;

    int num_of_events_1p_inFD = 0;

    int num_of_events_1n_inFD = 0, num_of_events_1n_inFD_AV = 0;

    int num_of_events_1e1n1p_wFakeNeut = 0;

    int num_of_events_2p = 0;
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Looping over each HipoChain file
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    while (chain.Next()) { // loop over events
        ++num_of_events; // logging Total #(events) in sample

        auto electrons_det = c12->getByID(11);
        if (electrons_det.size() == 1) { ++num_of_events_with_exactly_1e_from_file; }

        clasAna.Run(c12);

        /* allParticles vector from clas12ana (my addition). Used mostly for 1n1p.  */
        auto allParticles = clasAna.getParticles();

        /* All of these particles are with clas12ana cuts. Only cuts missing are momentum and beta(?) cuts - to be applied later */
        auto neutrons = clasAna.getByPid(2112);  // Neutrons
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
        int Nn = neutrons.size(), Np = protons.size(), Nkp = Kplus.size(), Nkm = Kminus.size(), Npip = piplus.size(), Npim = piminus.size(), Ne = electrons.size();
        int Nd = deuterons.size(), Nneut = neutrals.size(), No = otherpart.size();

        /* Total number of particles in event (= Nf) */
        int Nf = Nn + Np + Nkp + Nkm + Npip + Npim + Ne + Nd + Nneut + No;

        //<editor-fold desc="Configure good particles & basic event selection">
        /* Configure particles within general momentum cuts (i.e. "identified particles") */

        //<editor-fold desc="Charged particles' identification">
        vector<int> Electron_ind = ChargedParticleID(electrons, e_mom_th);
        vector<int> Protons_ind = ChargedParticleID(protons, p_mom_th);
        vector<int> Piplus_ind = ChargedParticleID(piplus, pip_mom_th);
        vector<int> Piminus_ind = ChargedParticleID(piminus, pim_mom_th);
        //</editor-fold>

        //<editor-fold desc="Neutral particles' identification (FD only)">
        /* Get FD neutrons and photons, according to the definitions: */
        vector<int> FD_Neutrons, FD_Photons;                                                              // FD neutrons and photons to be set by definition
        FDNeutralParticle(allParticles, FD_Neutrons, FD_Photons);                          // Get FD neutrons and photons, according to the definitions

        /* Get FD neutrons and photons above momentum threshold: */
        vector<int> NeutronsFD_ind, PhotonsFD_ind;                                                         // FD neutrons and photons by definition - within momentum th.
        FDNeutralParticleID(allParticles, NeutronsFD_ind, FD_Neutrons, n_mom_th, PhotonsFD_ind, FD_Photons, ph_mom_th, apply_neutron_Beta_Fit);
        //</editor-fold>

        //<editor-fold desc="Setting up event selection">
        /* set up basic event selection: */
        bool single_electron = (Electron_ind.size() == 1);                                                 // one electron above momentum threshold
        bool no_carged_Kaons = ((Nkp == 0) && (Nkm == 0));                                                 // no charged Kaons whatsoever
        bool no_carged_pions = ((Piplus_ind.size() == 0) && (Piminus_ind.size() == 0));                    // no charged Pions above momentum threshold
        bool no_deuterons = (Nd == 0);                                                                     // no Deuterons whatsoever

        bool basic_event_selection = (single_electron && no_carged_Kaons && no_carged_pions && no_deuterons);
        //</editor-fold>

        //</editor-fold>

        /* Safety check that allParticles.size(), Nf are the same */
        if (allParticles.size() != Nf) { cout << "\n\nallParticles.size() is different than Nf! exiting...\n\n", exit(EXIT_FAILURE); }

        //<editor-fold desc="Nph_CD, Nph_FD, No_Prime and Nf_Prime declarations and definitions">
        int Nph_CD = 0, Nph_FD = 0;

        for (int i = 0; i < No; i++) {
            if ((otherpart[i]->getRegion() == CD) && (otherpart[i]->par()->getPid() == 22)) {
                ++Nph_CD;
            } else if ((otherpart[i]->getRegion() == FD) && (otherpart[i]->par()->getPid() == 22)) {
                ++Nph_FD;
            }
        } // end of loop over otherpart vector

        /* No_Prime does not include (ignored particles): Neutrals, Neutrons, Photons in the CD (?) */
        int No_Prime = No; // don't ignore photons in CD
//        int No_Prime = No - Nph_CD; // ignore photons in CD
        int N_charged_p = Np + Nkp + Nkm + Npip + Npim + Ne + Nd; // Particles that are not ignored

        int Nf_Prime = N_charged_p + No_Prime;
        //</editor-fold>

        double Weight = 1;

        //<editor-fold desc="Some event counts">
        if (Ne == 0) { // Log total #(events) with and without electrons
            ++num_of_events_without_any_e; // logging Total #(events) w/o any e
        } else {
            ++num_of_events_with_any_e; // logging Total #(events) w/ any e
        }

        if (Ne >= 1) { // At least 1e cut (log only)
            ++num_of_events_with_at_least_1e; // logging #(events) w/ at least 1e

            if (Ne > 1) { ++num_of_events_more_then_1e; /* logging #(events) w/ more then 1e */ }
        }
        //</editor-fold>

        //<editor-fold desc="Process ID">
        bool qel = false, mec = false, res = false, dis = false;
        double processID = c12->mcevent()->getWeight(); // code = 1.,2.,3.,4. = type = qel, mec, res, dis

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
        if (PrintEvents) {
            bool EventPrintSelection = (Ne == Ne_in_event && Nf >= Nf_in_event);
            int event = c12->runconfig()->getEvent();

            if (EventPrintSelection) {
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

//  Filling truth level histograms (lundfile loop) ----------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Filling truth level histograms (lundfile loop)">
        if (calculate_truth_level && findSubstring(SampleName, "simulation") && apply_neutron_Beta_Fit) { // run only for CLAS12 simulation & AFTER beta fit
            auto mcpbank = c12->mcparts();
            const Int_t Ngen = mcpbank->getRows();

            //<editor-fold desc="Particle counting">
            /* Particle index vectors */
            vector<int> TL_Electron_ind, TL_Neutrons_ind, TL_Protons_ind, TL_piplus_ind, TL_piminus_ind, TL_pizero_ind, TL_Photons_ind, TL_OtherPart_ind;

            /* Particle index vectors (for FD particles) */
            vector<int> TL_NeutronsFD_ind, TL_ProtonsFD_ind, TL_pi0FD_ind, TL_PhotonsFD_ind;

            /* Particle index vectors (for particles above momentum threshold) */
            vector<int> TL_Electron_mom_ind, TL_Neutrons_mom_ind, TL_Protons_mom_ind, TL_piplus_mom_ind, TL_piminus_mom_ind, TL_pizero_mom_ind, TL_Photons_mom_ind;

            /* Particle index vectors (for FD particles above momentum threshold) */
            vector<int> TL_NeutronsFD_mom_ind, TL_ProtonsFD_mom_ind, TL_pi0FD_mom_ind, TL_PhotonsFD_mom_ind;

            for (Int_t i = 0; i < Ngen; i++) {
                mcpbank->setEntry(i);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));

                if (particlePDGtmp == 11) {
                    if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) { TL_Electron_mom_ind.push_back(i); }

                    TL_Electron_ind.push_back(i);
                } else if (particlePDGtmp == 2112) {
                    if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_Neutrons_mom_ind.push_back(i); }

                    TL_Neutrons_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_NeutronsFD_mom_ind.push_back(i); }

                        TL_NeutronsFD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 2212) {
                    if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_Protons_mom_ind.push_back(i); }

                    TL_Protons_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_ProtonsFD_mom_ind.push_back(i); }

                        TL_ProtonsFD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 211) {
                    if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) { TL_piplus_mom_ind.push_back(i); }

                    TL_piplus_ind.push_back(i);
                } else if (particlePDGtmp == -211) {
                    if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) { TL_piminus_mom_ind.push_back(i); }

                    TL_piminus_ind.push_back(i);
                } else if (particlePDGtmp == 111) {
                    if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pizero_mom_ind.push_back(i); }

                    TL_pizero_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pi0FD_mom_ind.push_back(i); }

                        TL_pi0FD_ind.push_back(i);
                    }
                } else if (particlePDGtmp == 22) {
                    if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) &&
                        (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_Photons_mom_ind.push_back(i); }

                    TL_Photons_ind.push_back(i);

                    if (inFD) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_PhotonsFD_mom_ind.push_back(i); }

                        TL_PhotonsFD_ind.push_back(i);
                    }
                } else {
                    TL_OtherPart_ind.push_back(i);
                }
            }
            //</editor-fold>

            /* Setting up basic TL event selection */
            bool no_TL_cPions = (TL_piplus_mom_ind.size() == 0 && TL_piminus_mom_ind.size() == 0);        // No id. cPions above momentum threshold
            bool no_TL_OtherPart = (TL_OtherPart_ind.size() == 0);                                        // No other part. above momentum threshold
            bool no_TL_FDpi0 = (Enable_FD_photons || (TL_pi0FD_mom_ind.size() == 0));                     // No id. pi0 in the FD above momentum threshold
            bool no_TL_FDPhotons = (Enable_FD_photons || (TL_PhotonsFD_mom_ind.size() == 0));             // No id. photons in the FD above momentum threshold
            bool TL_Event_Selection_1e_cut = (TL_Electron_mom_ind.size() == 1);                           // One id. electron above momentum threshold
            bool TL_Basic_ES = (TL_Event_Selection_1e_cut && no_TL_cPions && no_TL_OtherPart && no_TL_FDpi0 && no_TL_FDPhotons);

            /* Setting up 1p TL event selection */
            bool one_FDproton_1p = (TL_Protons_mom_ind.size() == 1 && TL_ProtonsFD_mom_ind.size() == 1);

            /* Setting up 1n TL event selection */
            bool one_FDNeutron_1n = (TL_NeutronsFD_mom_ind.size() == 1);
            bool no_protons_1n = (TL_Protons_mom_ind.size() == 0);
//            bool no_protons_1n = (TL_ProtonsFD_mom_ind.size() == 0);

            bool TL_Event_Selection_1p = (TL_Basic_ES && one_FDproton_1p);                                // One id. FD proton above momentum threshold
            bool TL_Event_Selection_1n = (TL_Basic_ES && one_FDNeutron_1n && no_protons_1n);              // One id. FD neutron above momentum threshold & no id. protons

            //<editor-fold desc="Fill TL histograms">
            for (Int_t i = 0; i < Ngen; i++) {
                mcpbank->setEntry(i);

                int particlePDGtmp = mcpbank->getPid();

                double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
                double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
                double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

                bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));

                if (particlePDGtmp == 11) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                            hP_e_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_e_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_e_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_e_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                            hP_e_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_e_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_e_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_e_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) {
                            hP_e_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_e_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_e_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_e_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_e_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_e_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                    }
                } else if (particlePDGtmp == 2112) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                            hP_n_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_n_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_n_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_n_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                            hP_n_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_n_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_n_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_n_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) {
                            hP_n_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_n_AC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_n_AC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_n_AC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        hP_n_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_n_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_n_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                        if (inFD) {
                            hP_n_BC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_n_BC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                            hPhi_n_BC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                        }
                    }
                } else if (particlePDGtmp == 2212) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                            hP_p_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_p_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_p_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_p_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                            hP_p_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_AC_truth_1p.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_p_AC_truth_1p_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_p_AC_truth_1p_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_p_AC_truth_1p_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        hP_p_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_p_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_p_BC_truth_1p.hFill(Particle_TL_Phi, Weight);

                        if (inFD) {
                            hP_p_BC_truth_1p_FD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_BC_truth_1p_FD.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_BC_truth_1p_FD.hFill(Particle_TL_Phi, Weight);
                        }
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) {
                            hP_p_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_p_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_p_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_p_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_p_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_p_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                    }
                } else if (particlePDGtmp == 211) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                            hP_pip_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pip_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pip_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_pip_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                            hP_pip_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pip_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pip_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_pip_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) {
                            hP_pip_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pip_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_pip_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pip_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pip_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_pip_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                    }
                } else if (particlePDGtmp == -211) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                            hP_pim_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pim_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pim_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_pim_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                            hP_pim_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pim_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pim_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_pim_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) {
                            hP_pim_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pim_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_pim_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pim_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pim_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_pim_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                    }
                } else if (particlePDGtmp == 111) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                            hP_pi0_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pi0_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pi0_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_pi0_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                            hP_pi0_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_AC_truth_1p.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pi0_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pi0_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_pi0_BC_truth_1p.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) {
                            hP_pi0_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_pi0_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_pi0_AC_truth_1n.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_pi0_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_pi0_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_pi0_BC_truth_1n.hFill(Particle_TL_Phi, Weight);
                    }
                } else if (particlePDGtmp == 22) {
                    if (TL_Event_Selection_1e_cut) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                            hP_ph_AC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_AC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_AC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                        }

                        hP_ph_BC_truth_1e_cut.hFill(Particle_TL_Momentum, Weight);
                        hTheta_ph_BC_truth_1e_cut.hFill(Particle_TL_Theta, Weight);
                        hPhi_ph_BC_truth_1e_cut.hFill(Particle_TL_Phi, Weight);
                    }

                    if (TL_Event_Selection_1p) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                            hP_ph_AC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_AC_truth_1p.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_AC_truth_1p.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_ph_AC_truth_1p_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_1p_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_1p_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        hP_ph_BC_truth_1p.hFill(Particle_TL_Momentum, Weight);
                        hTheta_ph_BC_truth_1p.hFill(Particle_TL_Theta, Weight);
                        hPhi_ph_BC_truth_1p.hFill(Particle_TL_Phi, Weight);

                        if (inFD) {
                            hP_ph_BC_truth_1p_FD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_1p_FD.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_1p_FD.hFill(Particle_TL_Phi, Weight);
                        }
                    }

                    if (TL_Event_Selection_1n) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) && (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) {
                            hP_ph_AC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_AC_truth_1n.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_AC_truth_1n.hFill(Particle_TL_Phi, Weight);

                            if (inFD) {
                                hP_ph_AC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                                hTheta_ph_AC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                                hPhi_ph_AC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                            }
                        }

                        hP_ph_BC_truth_1n.hFill(Particle_TL_Momentum, Weight);
                        hTheta_ph_BC_truth_1n.hFill(Particle_TL_Theta, Weight);
                        hPhi_ph_BC_truth_1n.hFill(Particle_TL_Phi, Weight);

                        if (inFD) {
                            hP_ph_BC_truth_1n_FD.hFill(Particle_TL_Momentum, Weight);
                            hTheta_ph_BC_truth_1n_FD.hFill(Particle_TL_Theta, Weight);
                            hPhi_ph_BC_truth_1n_FD.hFill(Particle_TL_Phi, Weight);
                        }
                    }
                }
            } // end of for loop over TL particles
            //</editor-fold>

        }
        //</editor-fold>

//  Fill All particles (All e) plots ------------------------------------------------------------------------------------------------------------------------------------

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
            e_out.SetPxPyPzE(electrons[i]->par()->getPx(), electrons[i]->par()->getPy(), electrons[i]->par()->getPz(), sqrt(m_e * m_e + P_e_tmp * P_e_tmp));
            Q = beam - e_out;                                    // definition of 4-momentum transfer
            Q2 = fabs(Q.Mag2());

            if (electrons[i]->getRegion() == FD) {
                hChi2_Electron_FD.hFill(electrons[i]->par()->getChi2Pid(), Weight);

                hBeta_vs_P_FD.hFill(electrons[i]->getP(), electrons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_Electrons_Only_FD.hFill(electrons[i]->getP(), electrons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_All_e_FD.hFill(electrons[i]->getP(), electrons[i]->par()->getBeta(), Weight);

                hTheta_e_All_e_FD->Fill(Theta_e_tmp);
                hPhi_e_All_e_FD->Fill(Phi_e_tmp);
                hTheta_e_VS_Phi_e_All_e_FD->Fill(Phi_e_tmp, Theta_e_tmp);

                hQ2_All_e_FD->Fill(Q2);

                if (Ne == 1) {
                    hQ2_1e_cut_FD->Fill(Q2);

                    if (Nf_Prime == 3 && Np == 2) {
                        hQ2_1e2p_FD->Fill(Q2);
                    }
                }
            }
        } // end of loop over electrons vector
        //</editor-fold>

        //<editor-fold desc="Proton plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Np; i++) {
            if (protons[i]->getRegion() == CD) {
                hChi2_Proton_CD.hFill(protons[i]->par()->getChi2Pid(), Weight);

                hBeta_vs_P_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
            } else if (protons[i]->getRegion() == FD) {
                hChi2_Proton_FD.hFill(protons[i]->par()->getChi2Pid(), Weight);

                hBeta_vs_P_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
            }
        } // end of loop over protons vector
        //</editor-fold>

        //<editor-fold desc="Other particles plots">

//        //<editor-fold desc="Neutrons Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                hBeta_vs_P_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                hBeta_vs_P_Neutrons_Only_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_All_e_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_All_e_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_All_e_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                hBeta_vs_P_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                hBeta_vs_P_Neutrons_Only_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);

//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_All_e_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_All_e_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_All_e_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

        //<editor-fold desc="Kplus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
//                hChi2_Kplus_CD.hFill(Kplus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
            } else if (Kplus[i]->getRegion() == FD) {
//                hChi2_Kplus_FD.hFill(Kplus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
//                hChi2_Kminus_CD.hFill(Kminus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_All_e_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
            } else if (Kminus[i]->getRegion() == FD) {
//                hChi2_Kminus_FD.hFill(Kminus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_All_e_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
//                hChi2_piplus_CD.hFill(piplus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
            } else if (piplus[i]->getRegion() == FD) {
//                hChi2_piplus_FD.hFill(piplus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_All_e_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
//                hChi2_piminus_CD.hFill(piminus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_All_e_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
            } else if (piminus[i]->getRegion() == FD) {
//                hChi2_piminus_FD.hFill(piminus[i]->par()->getChi2Pid(),Weight);

                hBeta_vs_P_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_All_e_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
            }
        } // end of loop over piminus vector
        //</editor-fold>

//        //<editor-fold desc="pizero Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < pizero.size(); i++) {
//            if (pizero[i]->getRegion() == CD) {
//                hBeta_vs_P_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_All_e_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_All_e_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_All_e_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                }
//            } else if (pizero[i]->getRegion() == FD) {
//                hBeta_vs_P_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_All_e_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_All_e_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_All_e_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                }
//            }
//        } // end of loop over pizero vector
//        //</editor-fold>

        //<editor-fold desc="neutrals Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < neutrals.size(); i++) {
            if (neutrals[i]->getRegion() == CD) {
                hBeta_vs_P_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);

                if (neutrals[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                }
            } else if (neutrals[i]->getRegion() == FD) {
                hBeta_vs_P_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);

                if (neutrals[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                }
            }
        } // end of loop over neutrals vector
        //</editor-fold>

        //<editor-fold desc="deuterons Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < deuterons.size(); i++) {
            if (deuterons[i]->getRegion() == CD) {
                hBeta_vs_P_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);

                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }
            } else if (deuterons[i]->getRegion() == FD) {
                hBeta_vs_P_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);

                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }
            }
        } // end of loop over deuterons vector
        //</editor-fold>

        //<editor-fold desc="otherpart Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < otherpart.size(); i++) {
            if (otherpart[i]->getRegion() == CD) {
                hBeta_vs_P_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);

                if (otherpart[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                }
            } else if (otherpart[i]->getRegion() == FD) {
                hBeta_vs_P_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);

                if (otherpart[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_All_e_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_All_e_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_All_e_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                }
            }
        } // end of loop over otherpart vector
        //</editor-fold>

        //</editor-fold>




        for (int i = 0; i < allParticles.size(); i++) {
            if ((allParticles[i]->getRegion() == FD) && (allParticles[i]->par()->getCharge() == 0)) {
                bool inPCAL_1n_temp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit

                if (inPCAL_1n_temp) { hTheta_neut_VS_Phi_neut_All_e_FD.hFill(allParticles[i]->getPhi() * 180.0 / pi, allParticles[i]->getTheta() * 180.0 / pi, Weight); }
            }
        }




        //</editor-fold>

//  1e cut --------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e cut">
        /* Here we apply electron cut on everything that follows (1p, 1n1p, 1e2p and 2p) */

        /* Applying rough 1e cut */
        if (Ne != 1) { continue; } // the rough 1e cut
        if (electrons[0]->par()->getBeta() > 1.2) { continue; } // electron beta cut
        ++num_of_events_with_exactly_1e; // logging #(events) w/ exactly 1e (after beta cut)

        //<editor-fold desc="Safety check (1e cut)">
        /* Safety check that we are looking at 1e cut. */

        // Check that we do have only one electron:
        if (electrons.size() != 1) { cout << "\n\n1e cut: electrons.size() is different than 1. Exiting...\n\n", exit(EXIT_FAILURE); }

        // Check that our one electron is within momentum cuts:
        //TODO: might be problemetic if electron momentum cuts are changed. consider removing this if that does happen.
        if (Electron_ind.size() != 1) { cout << "\n\n1e cut: Electron_ind.size() is different than 1. Exiting...\n\n", exit(EXIT_FAILURE); }
        if (Electron_ind.at(0) != 0) { cout << "\n\n1e cut: Electron_ind.at(0) is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
        //</editor-fold>

        //<editor-fold desc="events counts with protons (1e cut)">
        if (Nf_Prime == 2 && Np == 1) { ++num_of_events_with_1e1p; /* // logging #(events) w/ 1e1p */ }
        if (Protons_ind.size() == 1) { ++num_of_events_1e1p_all; } // #(1p) events with 1 identified protons (i.e. above momentum cuts)
//        if (Np == 1) { ++num_of_events_1e1p_all; } // #(1p) events with 1 proton
        if (Protons_ind.size() == 2) { ++num_of_events_1e2p_all; } // #(2p) events with 2 identified protons (i.e. above momentum cuts)
//        if (Np == 2) { ++num_of_events_1e2p_all; } // #(2p) events with 2 protons
        if (Protons_ind.size() == 2 && PhotonsFD_ind.size() == 0) { ++num_of_events_1e2p_all_woFDphotons; }
//        if (Np == 2 && Nph_FD == 0) { ++num_of_events_1e2p_all_woFDphotons; }
        //</editor-fold>

        /* Electron 1e cut variables definitions */
        TVector3 P_e_1e;
        P_e_1e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

        double P_e = P_e_1e.Mag(), E_e = sqrt(m_e * m_e + P_e * P_e);
        double Theta_e = P_e_1e.Theta() * 180.0 / pi, Phi_e = P_e_1e.Phi() * 180.0 / pi; // Theta_e, Phi_e in deg
        double Vx_e = electrons[0]->par()->getVx(), Vy_e = electrons[0]->par()->getVy(), Vz_e = electrons[0]->par()->getVz();

        //<editor-fold desc="Testing electron cuts">
        /* Here we plot cut histograms:
         * If cuts are turned off (apply_cuts == false) - we plot each cut parameter before and after the cut.
         * If cuts are applied (apply_cuts == true), we plot the parameters for the 1e cut only */

        //TODO: all these tests are NOT 2p or 1e2p (they were moved here from below and here we have no constraint on protons.size) - redefine propely to 1e or add constrant on protons.size

        //<editor-fold desc="Testing cuts">
        /* Testing SF cuts */
        double EoP_e = (electrons[0]->cal(PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / P_e;

        if (!apply_cuts) {
            /* SF plots before cuts */
            hSF_1e_cut_BC_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1e_cut_BC_FD.hFill(P_e, EoP_e, Weight);

            /* SF plots after cuts */
            if ((EoP_e >= clasAna.getEcalSFLowerCut()) && (EoP_e <= clasAna.getEcalSFUpperCut())) {
                hSF_1e_cut_AC_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1e_cut_AC_FD.hFill(P_e, EoP_e, Weight);
            }
        } else { hSF_1e_cut_BC_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1e_cut_BC_FD.hFill(P_e, EoP_e, Weight); }

        /* Testing fiducial cuts */
        if (!apply_cuts) {
            /* Fiducial plots before cuts */
            hVcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(PCAL)->getLv(), EoP_e, Weight);
            hWcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(PCAL)->getLw(), EoP_e, Weight);

            /* Fiducial plots after cuts */
            if (electrons[0]->cal(PCAL)->getLv() >= clasAna.getEcalEdgeCuts()) { hVcal_VS_EoP_1e_cut_AC_PCAL.hFill(electrons[0]->cal(PCAL)->getLv(), EoP_e, Weight); }
            if (electrons[0]->cal(PCAL)->getLw() >= clasAna.getEcalEdgeCuts()) { hWcal_VS_EoP_1e_cut_AC_PCAL.hFill(electrons[0]->cal(PCAL)->getLw(), EoP_e, Weight); }
        } else {
            hVcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(PCAL)->getLv(), EoP_e, Weight);
            hWcal_VS_EoP_1e_cut_BC_PCAL.hFill(electrons[0]->cal(PCAL)->getLw(), EoP_e, Weight);
        }

        /* Testing Nphe cuts */
        int Nphe = electrons[0]->che(HTCC)->getNphe();

        if (!apply_cuts) {
            /* Nphe plots before cuts */
            hNphe_1e_cut_BC_FD.hFill(Nphe, Weight);

            /* Nphe plots after cuts */
            if (Nphe >= clasAna.getNpheCuts()) { hNphe_1e_cut_AC_FD.hFill(Nphe, Weight); }
        } else {
            hNphe_1e_cut_BC_FD.hFill(Nphe, Weight);
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Filling 1e cut plots">

        //<editor-fold desc="Fill Electron plots (1e cut, FD only)">
        if (electrons[0]->getRegion() == FD) {
            hChi2_Electron_1e_cut_FD.hFill(electrons[0]->par()->getChi2Pid(), Weight);

            hVx_Electron_1e_cut_FD.hFill(electrons[0]->par()->getVx(), Weight);
            hVy_Electron_1e_cut_FD.hFill(electrons[0]->par()->getVy(), Weight);
            hVz_Electron_1e_cut_FD.hFill(electrons[0]->par()->getVz(), Weight);

            hBeta_vs_P_1e_cut_FD.hFill(P_e, electrons[0]->par()->getBeta(), Weight);
            hBeta_vs_P_1e_cut_Electrons_Only_FD.hFill(P_e, electrons[0]->par()->getBeta(), Weight);
            hBeta_vs_P_negative_part_1e_cut_FD.hFill(P_e, electrons[0]->par()->getBeta(), Weight);

            hP_e_1e_cut_FD.hFill(P_e, Weight);

            hTheta_e_1e_cut_FD->Fill(Theta_e);
            hPhi_e_1e_cut_FD->Fill(Phi_e);
            hTheta_e_VS_Phi_e_1e_cut_FD->Fill(Phi_e, Theta_e);

            if (Nf == 3 && Np == 2) {
                hTheta_e_All_Int_1e2p_FD->Fill(Theta_e);
                hPhi_e_All_Int_1e2p_FD->Fill(Phi_e);

                if (qel) {
                    hTheta_e_QEL_1e2p_FD->Fill(Theta_e);
                    hPhi_e_QEL_1e2p_FD->Fill(Phi_e);
                } else if (mec) {
                    hTheta_e_MEC_1e2p_FD->Fill(Theta_e);
                    hPhi_e_MEC_1e2p_FD->Fill(Phi_e);
                } else if (res) {
                    hTheta_e_RES_1e2p_FD->Fill(Theta_e);
                    hPhi_e_RES_1e2p_FD->Fill(Phi_e);
                } else if (dis) {
                    hTheta_e_DIS_1e2p_FD->Fill(Theta_e);
                    hPhi_e_DIS_1e2p_FD->Fill(Phi_e);
                }
            }

            hE_e_VS_Theta_e_All_Int_1e_cut_FD->Fill(Theta_e, E_e);
            hE_e_1e_cut_FD->Fill(E_e);
        }
        //</editor-fold>

        //<editor-fold desc="Fill Proton plots (1e cut, CD & FD)">
        for (auto &p: protons) {
            if (p->getRegion() == CD) {
                hChi2_Proton_1e_cut_CD.hFill(p->par()->getChi2Pid(), Weight);

                hVx_Proton_1e_cut_CD.hFill(p->par()->getVx(), Weight);
                hVy_Proton_1e_cut_CD.hFill(p->par()->getVy(), Weight);
                hVz_Proton_1e_cut_CD.hFill(p->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_CD.hFill(p->getP(), p->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_Protons_Only_CD.hFill(p->getP(), p->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_CD.hFill(p->getP(), p->par()->getBeta(), Weight);

                hP_p_1e_cut_CD.hFill(p->getP(), Weight);
            } else if (p->getRegion() == FD) {
                hChi2_Proton_1e_cut_FD.hFill(p->par()->getChi2Pid(), Weight);

                hVx_Proton_1e_cut_FD.hFill(p->par()->getVx(), Weight);
                hVy_Proton_1e_cut_FD.hFill(p->par()->getVy(), Weight);
                hVz_Proton_1e_cut_FD.hFill(p->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_FD.hFill(p->getP(), p->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_Protons_Only_FD.hFill(p->getP(), p->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_FD.hFill(p->getP(), p->par()->getBeta(), Weight);

                hP_p_1e_cut_FD.hFill(p->getP(), Weight);
            }
        } // end of loop over protons vector
        //</editor-fold>

        //<editor-fold desc="Fill Beta vs. P for other particles (1e cut, CD & FD)">

//        //<editor-fold desc="neutrons Beta vs. P plots (1e cut, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                hBeta_vs_P_1e_cut_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                hBeta_vs_P_1e_cut_Neutrons_Only_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                hBeta_vs_P_1e_cut_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                hBeta_vs_P_1e_cut_Neutrons_Only_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

        //<editor-fold desc="Kplus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
                hChi2_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getChi2Pid(), Weight);

                hVx_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getVx(), Weight);
                hVy_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getVy(), Weight);
                hVz_Kplus_1e_cut_CD.hFill(Kplus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_Kplus_Only_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
            } else if (Kplus[i]->getRegion() == FD) {
                hChi2_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getChi2Pid(), Weight);

                hVx_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getVx(), Weight);
                hVy_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getVy(), Weight);
                hVz_Kplus_1e_cut_FD.hFill(Kplus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_Kplus_Only_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                hChi2_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getChi2Pid(), Weight);

                hVx_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getVx(), Weight);
                hVy_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getVy(), Weight);
                hVz_Kminus_1e_cut_CD.hFill(Kminus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_Kminus_Only_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
            } else if (Kminus[i]->getRegion() == FD) {
                hChi2_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getChi2Pid(), Weight);

                hVx_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getVx(), Weight);
                hVy_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getVy(), Weight);
                hVz_Kminus_1e_cut_FD.hFill(Kminus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_Kminus_Only_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                hChi2_piplus_1e_cut_CD.hFill(piplus[i]->par()->getChi2Pid(), Weight);

                hVx_piplus_1e_cut_CD.hFill(piplus[i]->par()->getVx(), Weight);
                hVy_piplus_1e_cut_CD.hFill(piplus[i]->par()->getVy(), Weight);
                hVz_piplus_1e_cut_CD.hFill(piplus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_piplus_Only_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);

                hP_piplus_1e_cut_CD.hFill(piplus[i]->getP(), Weight);
            } else if (piplus[i]->getRegion() == FD) {
                hChi2_piplus_1e_cut_FD.hFill(piplus[i]->par()->getChi2Pid(), Weight);

                hVx_piplus_1e_cut_FD.hFill(piplus[i]->par()->getVx(), Weight);
                hVy_piplus_1e_cut_FD.hFill(piplus[i]->par()->getVy(), Weight);
                hVz_piplus_1e_cut_FD.hFill(piplus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_piplus_Only_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_positive_part_1e_cut_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);

                hP_piplus_1e_cut_FD.hFill(piplus[i]->getP(), Weight);
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                hChi2_piminus_1e_cut_CD.hFill(piminus[i]->par()->getChi2Pid(), Weight);

                hVx_piminus_1e_cut_CD.hFill(piminus[i]->par()->getVx(), Weight);
                hVy_piminus_1e_cut_CD.hFill(piminus[i]->par()->getVy(), Weight);
                hVz_piminus_1e_cut_CD.hFill(piminus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_piminus_Only_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);

                hP_piminus_1e_cut_CD.hFill(piminus[i]->getP(), Weight);
            } else if (piminus[i]->getRegion() == FD) {
                hChi2_piminus_1e_cut_FD.hFill(piminus[i]->par()->getChi2Pid(), Weight);

                hVx_piminus_1e_cut_FD.hFill(piminus[i]->par()->getVx(), Weight);
                hVy_piminus_1e_cut_FD.hFill(piminus[i]->par()->getVy(), Weight);
                hVz_piminus_1e_cut_FD.hFill(piminus[i]->par()->getVz(), Weight);

                hBeta_vs_P_1e_cut_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_1e_cut_piminus_Only_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                hBeta_vs_P_negative_part_1e_cut_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);

                hP_piminus_1e_cut_FD.hFill(piminus[i]->getP(), Weight);
            }
        } // end of loop over piminus vector
        //</editor-fold>

//        //<editor-fold desc="pizero Beta vs. P plots (1e cut, CD & FD)">
//        for (int i = 0; i < pizero.size(); i++) {
//            if (pizero[i]->getRegion() == CD) {
//                hBeta_vs_P_1e_cut_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                hBeta_vs_P_1e_cut_pizero_Only_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                }
//            } else if (pizero[i]->getRegion() == FD) {
//                hBeta_vs_P_1e_cut_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                hBeta_vs_P_1e_cut_pizero_Only_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(pizero[i]->getP(), pizero[i]->par()->getBeta(),Weight);
//                }
//            }
//        } // end of loop over pizero vector
//        //</editor-fold>

        //<editor-fold desc="neutrals Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < neutrals.size(); i++) {
            if (neutrals[i]->getRegion() == CD) {
                hBeta_vs_P_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);

                if (neutrals[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                }
            } else if (neutrals[i]->getRegion() == FD) {
                hBeta_vs_P_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);

                if (neutrals[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                } else if (neutrals[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(neutrals[i]->getP(), neutrals[i]->par()->getBeta(), Weight);
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="deuterons Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < deuterons.size(); i++) {
            if (deuterons[i]->getRegion() == CD) {
                hBeta_vs_P_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);

                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }
            } else if (deuterons[i]->getRegion() == FD) {
                hBeta_vs_P_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);

                if (deuterons[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                } else if (deuterons[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(deuterons[i]->getP(), deuterons[i]->par()->getBeta(), Weight);
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="otherpart Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < otherpart.size(); i++) {
            if (otherpart[i]->getRegion() == CD) {
                hBeta_vs_P_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);

                if (otherpart[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_CD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                }
            } else if (otherpart[i]->getRegion() == FD) {
                hBeta_vs_P_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);

                if (otherpart[i]->par()->getCharge() == 1) {
                    hBeta_vs_P_positive_part_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == 0) {
                    hBeta_vs_P_neutral_part_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                } else if (otherpart[i]->par()->getCharge() == -1) {
                    hBeta_vs_P_negative_part_1e_cut_FD.hFill(otherpart[i]->getP(), otherpart[i]->par()->getBeta(), Weight);
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Fill ToF plots for electron (1e cut, FD)">
        //TODO: rename and move from ToF to angles folder

        bool e_hit_PCAL_1e_cut = (electrons[0]->cal(clas12::PCAL)->getDetector() == 7); // check if electron hit the PCAL

        //TODO: ask Justin if electrons and/or protons can be detected in the FD WITHOUT hitting the PCAL
        if (e_hit_PCAL_1e_cut) { // if there's an electron hit in the PCAL
            // electron PCAL hit vector and angles:
            TVector3 e_hit_1e_cut_3v(electrons[0]->cal(clas12::PCAL)->getX(), electrons[0]->cal(clas12::PCAL)->getY(), electrons[0]->cal(clas12::PCAL)->getZ());
            double e_hit_Theta_1e_cut = e_hit_1e_cut_3v.Theta() * 180 / pi, e_hit_Phi_1e_cut = e_hit_1e_cut_3v.Phi() * 180 / pi;

            for (auto &n: neutrons) { // loop on every detected neutron
                // check neutron hits in the PCAL, ECIN and ECOUT:
                bool n_hit_PCAL_1e_cut = (n->cal(clas12::PCAL)->getDetector() == 7);
                bool n_hit_ECIN_1e_cut = (n->cal(clas12::ECIN)->getDetector() == 7);
                bool n_hit_ECOUT_1e_cut = (n->cal(clas12::ECOUT)->getDetector() == 7);

                if ((n_hit_PCAL_1e_cut == false) &&
                    ((n_hit_ECIN_1e_cut == true) || (n_hit_ECOUT_1e_cut == true))) { // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT
                    auto n_detlayer_1e_cut = n_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT; // find first layer of hit

                    // neutron ECIN/ECAL hit vector and angles:
                    TVector3 n_hit_1e_cut_3v(n->cal(n_detlayer_1e_cut)->getX(), n->cal(n_detlayer_1e_cut)->getY(), n->cal(n_detlayer_1e_cut)->getZ());
                    double n_hit_Theta_1e_cut = n_hit_1e_cut_3v.Theta() * 180 / pi, n_hit_Phi_1e_cut = n_hit_1e_cut_3v.Phi() * 180 / pi;

                    // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                    hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut.hFill(n_hit_Phi_1e_cut - e_hit_Phi_1e_cut, n_hit_Theta_1e_cut - e_hit_Theta_1e_cut, Weight);
                }
            } // end of ToF loop over neutrons vector
        }
        //</editor-fold>

        //<editor-fold desc="Fill ToF plots for protons (1e cut, FD)">
        for (auto &n: neutrons) { // loop on every detected neutron
            // check neutron (n) hits in the PCAL, ECIN and ECOUT:
            bool n_hit_PCAL_1e_cut = (n->cal(clas12::PCAL)->getDetector() == 7);
            bool n_hit_ECIN_1e_cut = (n->cal(clas12::ECIN)->getDetector() == 7);
            bool n_hit_ECOUT_1e_cut = (n->cal(clas12::ECOUT)->getDetector() == 7);

            // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT // if neutron (n) did not hit PCAL & hit either ECIN or ECOUT:
            if ((n_hit_PCAL_1e_cut == false) && ((n_hit_ECIN_1e_cut == true) || (n_hit_ECOUT_1e_cut == true))) {
                auto n_detlayer_1e_cut = n_hit_ECIN_1e_cut ? clas12::ECIN : clas12::ECOUT; // find first layer of hit

                // neutron (n)'s ECIN/ECAL hit vector and angles:
                TVector3 n_hit_1e_cut_3v(n->cal(n_detlayer_1e_cut)->getX(), n->cal(n_detlayer_1e_cut)->getY(), n->cal(n_detlayer_1e_cut)->getZ());
                double n_hit_Theta_1e_cut = n_hit_1e_cut_3v.Theta() * 180 / pi, n_hit_Phi_1e_cut = n_hit_1e_cut_3v.Phi() * 180 / pi;

                for (auto &p: protons) { // loop over protons vector
                    bool p_hit_PCAL_1e_cut = (p->cal(clas12::PCAL)->getDetector() == 7); // check if proton hit the PCAL
                    //TODO: rename and move from ToF to angles folder

                    //TODO: ask Justin if electrons and/or protons can be detected in the FD WITHOUT hitting the PCAL
                    if (p_hit_PCAL_1e_cut) { // if there's a proton hit in the PCAL
                        // proton PCAL hit vector and angles:
                        TVector3 p_hit_1e_cut_3v(p->cal(clas12::PCAL)->getX(), p->cal(clas12::PCAL)->getY(), p->cal(clas12::PCAL)->getZ());
                        double p_hit_Theta_1e_cut = p_hit_1e_cut_3v.Theta() * 180 / pi, p_hit_Phi_1e_cut = p_hit_1e_cut_3v.Phi() * 180 / pi;

                        // subtracting the angles between the neutron (n) hit and proton hit to see if we have fake neutrons:
                        hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut.hFill(n_hit_Phi_1e_cut - p_hit_Phi_1e_cut, n_hit_Theta_1e_cut - p_hit_Theta_1e_cut, Weight);
                    }
                } // end of ToF loop over protons vector
            }
        } // end of ToF loop over neutrons vector
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

//  1p (FD only) --------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1p (FD only)">
        /* 1p event selection: 1p = Protons_ind.size() = 1 and any number of neutrons, other neutrals and particles with pdg=0.*/
        bool event_selection_1p = (basic_event_selection && (Protons_ind.size() == 1) && (Enable_FD_photons || (PhotonsFD_ind.size() == 0)));

        if (calculate_1p && event_selection_1p) { // for 1p calculations (with any number of neutrals)

            //<editor-fold desc="Safety check (1p)">
            /* Safety check that we are looking at 1p */
            if (!(Enable_FD_photons || (PhotonsFD_ind.size() == 0))) {
                cout << "\n\n1p: PhotonsFD_ind.size() is non-zero (" << PhotonsFD_ind.size() << "). Exiting...\n\n", exit(EXIT_FAILURE);
            }

            if (Protons_ind.size() != 1) { cout << "\n\n1p: Protons_ind.size() is different than 1. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kplus.size() != 0) { cout << "\n\n1p: Kplus.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kminus.size() != 0) { cout << "\n\n1p: Kminus.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Piplus_ind.size() != 0) { cout << "\n\n1p: Piplus_ind.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Piminus_ind.size() != 0) { cout << "\n\n1p: Piminus_ind.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Electron_ind.size() != 1) { cout << "\n\n1p: Electron_ind.size() is different than 1. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (deuterons.size() != 0) { cout << "\n\n1p: deuterons.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            //</editor-fold>

            // looking at events with 1p in the FD only:
            if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                ++num_of_events_1p_inFD; // 1p event count after momentum and theta_p cuts

                TVector3 P_e_1p_3v, q_1p_3v, P_p_1p_3v, P_T_e_1p_3v, P_T_p_1p_3v, dP_T_1p_3v, P_N_1p_3v;
                P_e_1p_3v.SetMagThetaPhi(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->getTheta(),
                                         electrons[Electron_ind.at(0)]->getPhi());                                                              // electron 3 momentum
                q_1p_3v = TVector3(Pvx - P_e_1p_3v.Px(), Pvy - P_e_1p_3v.Py(), Pvz - P_e_1p_3v.Pz());                                              // 3 momentum transfer
                P_p_1p_3v.SetMagThetaPhi(protons[Protons_ind.at(0)]->getP(), protons[Protons_ind.at(0)]->getTheta(),
                                         protons[Protons_ind.at(0)]->getPhi());                                                                      // proton 3 momentum
                P_T_e_1p_3v = TVector3(P_e_1p_3v.Px(), P_e_1p_3v.Py(), 0);                                                                // electron transverse momentum
                P_T_p_1p_3v = TVector3(P_p_1p_3v.Px(), P_p_1p_3v.Py(), 0);                                                                  // proton transverse momentum

                double E_e_1p = sqrt(m_e * m_e + P_e_1p_3v.Mag2()), E_p_1p = sqrt(m_p * m_p + P_p_1p_3v.Mag2()), Ecal_1p, dAlpha_T_1p, dPhi_T_1p;
                double Theta_p_e_p_p_1p, Theta_q_p_p_1p;

                //  Fillings 1p histograms ------------------------------------------------------------------------------------------------------------------------------

                //<editor-fold desc="Truth-level 1p enforcement configuration">
                /* Configure rather to apply 1p truth level ES on rec. plots or not. */
                bool TL_Event_Selection_1p_1p, TL_Event_Selection_1n_1p;

                //<editor-fold desc="Setting TL ES conditions">
                auto mcpbank_1p = c12->mcparts();
                const Int_t Ngen_1p = mcpbank_1p->getRows();

                //<editor-fold desc="Particle counting">
                /* Particle index vectors */
                vector<int> TL_Electron_ind_1p, TL_Neutrons_ind_1p, TL_Protons_ind_1p;
                vector<int> TL_piplus_ind_1p, TL_piminus_ind_1p, TL_pizero_ind_1p, TL_Photons_ind_1p, TL_OtherPart_ind_1p;

                /* Particle index vectors (for FD particles) */
                vector<int> TL_NeutronsFD_ind_1p, TL_ProtonsFD_ind_1p, TL_pi0FD_ind_1p, TL_PhotonsFD_ind_1p;

                /* Particle index vectors (for particles above momentum threshold) */
                vector<int> TL_Electron_mom_ind_1p, TL_Neutrons_mom_ind_1p, TL_Protons_mom_ind_1p, TL_piplus_mom_ind_1p, TL_piminus_mom_ind_1p, TL_pizero_mom_ind_1p, TL_Photons_mom_ind_1p;

                /* Particle index vectors (for FD particles above momentum threshold) */
                vector<int> TL_NeutronsFD_mom_ind_1p, TL_ProtonsFD_mom_ind_1p, TL_pi0FD_mom_ind_1p, TL_PhotonsFD_mom_ind_1p;

                for (Int_t i = 0; i < Ngen_1p; i++) {
                    mcpbank_1p->setEntry(i);

                    int particlePDGtmp = mcpbank_1p->getPid();

                    double Particle_TL_Momentum = rCalc(mcpbank_1p->getPx(), mcpbank_1p->getPy(), mcpbank_1p->getPz());
                    double Particle_TL_Theta = acos((mcpbank_1p->getPz()) / rCalc(mcpbank_1p->getPx(), mcpbank_1p->getPy(), mcpbank_1p->getPz())) * 180.0 / pi;
                    double Particle_TL_Phi = atan2(mcpbank_1p->getPy(), mcpbank_1p->getPx()) * 180.0 / pi;

                    bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));

                    if (particlePDGtmp == 11) {
                        if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) { TL_Electron_mom_ind_1p.push_back(i); }

                        TL_Electron_ind_1p.push_back(i);
                    } else if (particlePDGtmp == 2112) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_Neutrons_mom_ind_1p.push_back(i); }

                        TL_Neutrons_ind_1p.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_NeutronsFD_mom_ind_1p.push_back(i); }

                            TL_NeutronsFD_ind_1p.push_back(i);
                        }
                    } else if (particlePDGtmp == 2212) {
                        if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_Protons_mom_ind_1p.push_back(i); }

                        TL_Protons_ind_1p.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_ProtonsFD_mom_ind_1p.push_back(i); }

                            TL_ProtonsFD_ind_1p.push_back(i);
                        }
                    } else if (particlePDGtmp == 211) {
                        if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) { TL_piplus_mom_ind_1p.push_back(i); }

                        TL_piplus_ind_1p.push_back(i);
                    } else if (particlePDGtmp == -211) {
                        if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) { TL_piminus_mom_ind_1p.push_back(i); }

                        TL_piminus_ind_1p.push_back(i);
                    } else if (particlePDGtmp == 111) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pizero_mom_ind_1p.push_back(i); }

                        TL_pizero_ind_1p.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pi0FD_mom_ind_1p.push_back(i); }

                            TL_pi0FD_ind_1p.push_back(i);
                        }
                    } else if (particlePDGtmp == 22) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_Photons_mom_ind_1p.push_back(i); }

                        TL_Photons_ind_1p.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_PhotonsFD_mom_ind_1p.push_back(i); }

                            TL_PhotonsFD_ind_1p.push_back(i);
                        }
                    } else {
                        TL_OtherPart_ind_1p.push_back(i);
                    }
                }
                //</editor-fold>

                /* Setting up basic TL event selection */
                bool no_TL_cPions_1p = (TL_piplus_mom_ind_1p.size() == 0 && TL_piminus_mom_ind_1p.size() == 0);     // No id. cPions above momentum threshold
                bool no_TL_OtherPart_1p = (TL_OtherPart_ind_1p.size() == 0);                                        // No other part. above momentum threshold
                bool no_TL_FDpi0_1p = (Enable_FD_photons || (TL_pi0FD_mom_ind_1p.size() == 0));                     // No id. pi0 in the FD above momentum threshold
                bool no_TL_FDPhotons_1p = (Enable_FD_photons || (TL_PhotonsFD_mom_ind_1p.size() == 0));             // No id. photons in the FD above momentum threshold
                bool TL_Event_Selection_1e_cut_1p = (TL_Electron_mom_ind_1p.size() == 1);                           // One id. electron above momentum threshold
                bool TL_Basic_ES = (TL_Event_Selection_1e_cut_1p && no_TL_cPions_1p && no_TL_OtherPart_1p && no_TL_FDpi0_1p && no_TL_FDPhotons_1p);

                /* Setting up 1p TL event selection */
                bool one_FDproton_1p_1p = (TL_Protons_mom_ind_1p.size() == 1 && TL_ProtonsFD_mom_ind_1p.size() == 1);

                /* Setting up 1n TL event selection */
                bool one_FDNeutron_1n_1p = (TL_NeutronsFD_mom_ind_1p.size() == 1);
                bool no_protons_1n_1p = (TL_Protons_mom_ind_1p.size() == 0);
//                bool no_protons_1n_1p = (TL_ProtonsFD_mom_ind_1p.size() == 0);

                /* One id. FD proton above momentum threshold: */
                TL_Event_Selection_1p_1p = (TL_Basic_ES && one_FDproton_1p_1p);

                /* One id. FD neutron above momentum threshold & no id. protons: */
                TL_Event_Selection_1n_1p = (TL_Basic_ES && one_FDNeutron_1n_1p && no_protons_1n_1p);
                //</editor-fold>

                bool apply_TL_1p_ES;

                if (Rec_wTL_ES) { // Enforce TL event selection
                    apply_TL_1p_ES = TL_Event_Selection_1p_1p; // Events will be logged only is TL_Event_Selection_1p is true
                } else if (!Rec_wTL_ES) { // Don't enforce TL event selection
                    apply_TL_1p_ES = true; // Events will be logged regardless of TL_Event_Selection_1p is true
                }
                //</editor-fold>

                //<editor-fold desc="Fillings 1p histograms">
                if (apply_TL_1p_ES) {

                    //<editor-fold desc="Filling cut variable plots (1p)">
                    /* Filling Nphe plots (1p) */
                    hNphe_1p_FD.hFill(Nphe, Weight);

                    /* Filling Chi2 histograms (1p) */
                    // Electrton Chi2 (1p):
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) { hChi2_Electron_1p_FD.hFill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight); }

                    // Proton Chi2 (1p):
                    if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                        hChi2_Proton_1p_CD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                    } else if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                        hChi2_Proton_1p_FD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                    }

                    /* Filling dVx, dVy, dVz (1p) */
                    for (auto &p: protons) {
                        double Vx_p_1p = p->par()->getVx(), Vy_p_1p = p->par()->getVy(), Vz_p_1p = p->par()->getVz();
                        double dVx = Vx_p_1p - Vx_e, dVy = Vy_p_1p - Vy_e, dVz = Vz_p_1p - Vz_e;

                        hdVx_1p.hFill(dVx, Weight), hdVy_1p.hFill(dVy, Weight), hdVz_1p.hFill(dVz, Weight);
                    } // end of loop over protons vector

                    /* Filling SF histograms (1p) */
                    hSF_1p_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1p_FD.hFill(P_e, EoP_e, Weight);

                    /* Filling fiducial plots (1p) */
                    hVcal_VS_EoP_1p_PCAL.hFill(electrons[Electron_ind.at(0)]->cal(PCAL)->getLv(), EoP_e, Weight);
                    hWcal_VS_EoP_1p_PCAL.hFill(electrons[Electron_ind.at(0)]->cal(PCAL)->getLw(), EoP_e, Weight);

                    //<editor-fold desc="Filling momentum threshold histograms (1p)">
                    /* Filling momentum threshold histograms (1p) */

                    //<editor-fold desc="Electron momentum (1p)">
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) { hP_e_APID_1p_FD.hFill(P_e, Weight); }

                    for (int i = 0; i < Ne; i++) {
                        if (electrons[i]->getRegion() == FD) { hP_e_BPID_1p_FD.hFill(P_e, Weight); }
                    } // before mom. th.
                    //</editor-fold>

                    //<editor-fold desc="Proton momentum (1p)">
                    //TODO: remove 1p plots for protons in CD (we're looking at 1p in the FD only!)
                    for (int &i: Protons_ind) {
                        if (protons[i]->getRegion() == CD) {
                            hP_p_APID_1p_CD.hFill(protons[i]->getP(), Weight); // after mom. th.
                        } else if (protons[i]->getRegion() == FD) {
                            hP_p_APID_1p_FD.hFill(protons[i]->getP(), Weight); // after mom. th.
                        }
                    }

                    for (int i = 0; i < Np; i++) {
                        if (protons[i]->getRegion() == CD) {
                            hP_p_BPID_1p_CD.hFill(protons[i]->getP(), Weight); // before mom. th.
                        } else if (protons[i]->getRegion() == FD) {
                            hP_p_BPID_1p_FD.hFill(protons[i]->getP(), Weight); // before mom. th.
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Piplus momentum (1p)">
                    for (int &i: Piplus_ind) {
                        if (piplus[i]->getRegion() == CD) {
                            hP_piplus_APID_1p_CD.hFill(piplus[i]->getP(), Weight); // after mom. th.
                        } else if (piplus[i]->getRegion() == FD) {
                            hP_piplus_APID_1p_FD.hFill(piplus[i]->getP(), Weight); // after mom. th.
                        }
                    }

                    for (int i = 0; i < Npip; i++) {
                        if (piplus[i]->getRegion() == CD) {
                            hP_piplus_BPID_1p_CD.hFill(piplus[i]->getP(), Weight); // before mom. th.
                        } else if (piplus[i]->getRegion() == FD) {
                            hP_piplus_BPID_1p_FD.hFill(piplus[i]->getP(), Weight); // before mom. th.
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Piminus momentum (1p)">
                    for (int &i: Piminus_ind) {
                        if (piminus[i]->getRegion() == CD) {
                            hP_piminus_APID_1p_CD.hFill(piminus[i]->getP(), Weight); // after mom. th.
                        } else if (piminus[i]->getRegion() == FD) {
                            hP_piminus_APID_1p_FD.hFill(piminus[i]->getP(), Weight); // after mom. th.
                        }
                    }

                    for (int i = 0; i < piminus.size(); i++) {
                        if (piminus[i]->getRegion() == CD) {
                            hP_piminus_BPID_1p_CD.hFill(piminus[i]->getP(), Weight); // before mom. th.
                        } else if (piminus[i]->getRegion() == FD) {
                            hP_piminus_BPID_1p_FD.hFill(piminus[i]->getP(), Weight); // before mom. th.
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Photon momentum (1p)">
                    for (int &i: PhotonsFD_ind) { hP_ph_APID_1p_FD.hFill(allParticles[i]->getP(), Weight); } // after mom. th.

                    for (int &i: FD_Photons) { hP_ph_BPID_1p_FD.hFill(allParticles[i]->getP(), Weight); } // before mom. th.
                    //</editor-fold>

                    //<editor-fold desc="Neutron momentum (1p)">
                    for (int &i: NeutronsFD_ind) { hP_n_APID_1p_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); } // after mom. th.

                    for (int &i: FD_Neutrons) { hP_n_BPID_1p_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); } // before mom. th.
                    //</editor-fold>

                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Filling Beta vs. P plots (1p)">

                    //<editor-fold desc="Beta vs. P from electrons (1p, CD & FD)">
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                        hBeta_vs_P_1p_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                        hBeta_vs_P_1p_Electrons_Only_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                        hBeta_vs_P_negative_part_1p_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                    }
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from protons (1p, CD & FD)">
                    //TODO: get rid of the loop. there is only one proton here.
                    for (int i = 0; i < Np; i++) {
                        if (protons[i]->getRegion() == CD) {
                            hBeta_vs_P_1p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1p_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_positive_part_1p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                        } else if (protons[i]->getRegion() == FD) {
                            hBeta_vs_P_1p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1p_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_positive_part_1p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over protons vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from other particles (1p, CD & FD)">
                    /* This is for self-consistency. Contributions from other particles should be zero */
                    //TODO: move to one loop over allParticles vector
//        //<editor-fold desc="Beta vs. P from neutrons (1p, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                hBeta_vs_P_1p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_1p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_1p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_1p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                hBeta_vs_P_1p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_1p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_1p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_1p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

                    //<editor-fold desc="Beta vs. P from Kplus (1p, CD & FD)">
                    for (int i = 0; i < Kplus.size(); i++) {
                        if (Kplus[i]->getRegion() == CD) {
                            hBeta_vs_P_1p_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_positive_part_1p_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                        } else if (Kplus[i]->getRegion() == FD) {
                            hBeta_vs_P_1p_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_positive_part_1p_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over Kplus vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from Kminus (1p, CD & FD)">
                    for (int i = 0; i < Kminus.size(); i++) {
                        if (Kminus[i]->getRegion() == CD) {
                            hBeta_vs_P_1p_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_negative_part_1p_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                        } else if (Kminus[i]->getRegion() == FD) {
                            hBeta_vs_P_1p_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_negative_part_1p_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over Kminus vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from piplus (1p, CD & FD)">
                    for (int i = 0; i < piplus.size(); i++) {
                        if (piplus[i]->getRegion() == CD) {
                            hBeta_vs_P_1p_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_positive_part_1p_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                        } else if (piplus[i]->getRegion() == FD) {
                            hBeta_vs_P_1p_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_positive_part_1p_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over piplus vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from piminus (1p, CD & FD)">
                    for (int i = 0; i < piminus.size(); i++) {
                        if (piminus[i]->getRegion() == CD) {
                            hBeta_vs_P_1p_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_negative_part_1p_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                        } else if (piminus[i]->getRegion() == FD) {
                            hBeta_vs_P_1p_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_negative_part_1p_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over piminus vector
                    //</editor-fold>

                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Filling electron plots (1p)">
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                        hTheta_e_All_Int_1p_FD->Fill(Theta_e, Weight);
                        hPhi_e_All_Int_1p_FD->Fill(Phi_e, Weight);
                        hTheta_e_VS_Phi_e_1p_FD->Fill(Phi_e, Theta_e, Weight);

                        hE_e_All_Int_1p_FD->Fill(E_e, Weight);
                        hE_e_VS_Theta_e_All_Int_1p_FD->Fill(Theta_e, E_e, Weight);

                        hET_All_Ang_All_Int_1p_FD->Fill(beamE - E_e, Weight);

                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_All_Int_1p_FD->Fill(beamE - E_e, Weight);
                            hE_e_15_All_Int_1p_FD->Fill(E_e, Weight);
                        }

                        if (qel) {
                            hTheta_e_QEL_1p_FD->Fill(Theta_e, Weight);
                            hPhi_e_QEL_1p_FD->Fill(Phi_e, Weight);
                            hE_e_QEL_1p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_QEL_1p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_QEL_1p_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_QEL_1p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_QEL_1p_FD->Fill(E_e, Weight);
                            }
                        } else if (mec) {
                            hTheta_e_MEC_1p_FD->Fill(Theta_e, Weight);
                            hPhi_e_MEC_1p_FD->Fill(Phi_e, Weight);
                            hE_e_MEC_1p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_MEC_1p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_MEC_1p_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_MEC_1p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_MEC_1p_FD->Fill(E_e, Weight);
                            }
                        } else if (res) {
                            hTheta_e_RES_1p_FD->Fill(Theta_e, Weight);
                            hPhi_e_RES_1p_FD->Fill(Phi_e, Weight);
                            hE_e_RES_1p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_RES_1p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_RES_1p_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_RES_1p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_RES_1p_FD->Fill(E_e, Weight);
                            }
                        } else if (dis) {
                            hTheta_e_DIS_1p_FD->Fill(Theta_e, Weight);
                            hPhi_e_DIS_1p_FD->Fill(Phi_e, Weight);
                            hE_e_DIS_1p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_DIS_1p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_DIS_1p_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_DIS_1p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_DIS_1p_FD->Fill(E_e, Weight);
                            }
                        }

                        /* Filling Q2 histograms (1p) */
                        hQ2_1p_FD->Fill(Q2, Weight);
                    }
                    //</editor-fold>

                    //<editor-fold desc="Filling Ecal plots (1p)">
                    Ecal_1p = E_e_1p + (E_p_1p - m_p);

                    if (Ecal_1p > beamE) {
                        hChi2_Electron_Ecal_test_1p->Fill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight);
                        hChi2_Proton_Ecal_test_1p->Fill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                    }

                    hEcal_All_Int_1p->Fill(Ecal_1p, Weight); // Fill Ecal for all interactions

                    if (qel) {
                        hEcal_QEL_1p->Fill(Ecal_1p, Weight); // Fill Ecal for QEL only
                    } else if (mec) {
                        hEcal_MEC_1p->Fill(Ecal_1p, Weight); // Fill Ecal for MEC only
                    } else if (res) {
                        hEcal_RES_1p->Fill(Ecal_1p, Weight); // Fill Ecal for RES only
                    } else if (dis) {
                        hEcal_DIS_1p->Fill(Ecal_1p, Weight); // Fill Ecal for DIS only
                    }
                    //</editor-fold>

                    dP_T_1p_3v = TVector3(P_T_e_1p_3v.Px() + P_T_p_1p_3v.Px(), P_T_e_1p_3v.Py() + P_T_p_1p_3v.Py(), 0);
                    dAlpha_T_1p = acos(-(P_e_1p_3v.Px() * dP_T_1p_3v.Px() + P_e_1p_3v.Py() * dP_T_1p_3v.Py() + P_e_1p_3v.Pz() * dP_T_1p_3v.Pz())
                                       / (P_T_e_1p_3v.Mag() * dP_T_1p_3v.Mag())) * 180.0 / pi; // dP_T_1p_3v.Pz() = 0; dAlpha_T_1p in deg
                    hdP_T_1p->Fill(dP_T_1p_3v.Mag(), Weight);
                    hdAlpha_T_1p->Fill(dAlpha_T_1p, Weight);
                    hdP_T_vs_dAlpha_T_1p->Fill(dAlpha_T_1p, dP_T_1p_3v.Mag(), Weight);

                    dPhi_T_1p = acos(-(P_T_e_1p_3v.Px() * P_T_p_1p_3v.Px() + P_T_e_1p_3v.Py() * P_T_p_1p_3v.Py() + P_T_e_1p_3v.Pz() * P_T_p_1p_3v.Pz())
                                     / (P_T_e_1p_3v.Mag() * P_T_p_1p_3v.Mag())) * 180.0 / pi; // P_T_p_1p_3v.Pz() = 0; dPhi_T_1p in deg
                    hdPhi_T_1p->Fill(dPhi_T_1p, Weight);

                    hEcal_vs_dAlpha_T_1p->Fill(dAlpha_T_1p, Ecal_1p, Weight);
                    hEcal_vs_dP_T_1p->Fill(dP_T_1p_3v.Mag(), Ecal_1p, Weight);


                    hTheta_p_All_Int_1p->Fill(protons[Protons_ind.at(0)]->getTheta() * 180.0 / pi, Weight);
                    hPhi_p_All_Int_1p->Fill(protons[Protons_ind.at(0)]->getPhi() * 180.0 / pi, Weight);

                    Theta_p_e_p_p_1p = acos((P_e_1p_3v.Px() * P_p_1p_3v.Px() + P_e_1p_3v.Py() * P_p_1p_3v.Py() + P_e_1p_3v.Pz() * P_p_1p_3v.Pz())
                                            / (P_e_1p_3v.Mag() * P_p_1p_3v.Mag())) * 180.0 / pi; // Theta_p_e_p_p_1p in deg
                    hTheta_p_e_p_p_1p->Fill(Theta_p_e_p_p_1p, Weight);

                    Theta_q_p_p_1p = acos((q_1p_3v.Px() * P_p_1p_3v.Px() + q_1p_3v.Py() * P_p_1p_3v.Py() + q_1p_3v.Pz() * P_p_1p_3v.Pz())
                                          / (q_1p_3v.Mag() * P_p_1p_3v.Mag())) * 180.0 / pi; // Theta_q_p_p_1p in deg
                    hTheta_q_p_p_1p->Fill(Theta_q_p_p_1p, Weight);

                    hTheta_q_p_p_vs_p_p_q_1p->Fill(P_p_1p_3v.Mag() / q_1p_3v.Mag(), Theta_q_p_p_1p, Weight);

                    P_N_1p_3v = TVector3(P_e_1p_3v.Px() + P_p_1p_3v.Px() - Pvx, P_e_1p_3v.Py() + P_p_1p_3v.Py() - Pvy, P_e_1p_3v.Pz() + P_p_1p_3v.Pz() - Pvz);
                    hTheta_q_p_p_vs_p_N_q_1p->Fill(P_N_1p_3v.Mag() / q_1p_3v.Mag(), Theta_q_p_p_1p, Weight);

                }
                //</editor-fold>

            } // end of proton in FD if
        } // end of 1p cuts if
        //</editor-fold>

//  1n (FD only) --------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1n (FD only)">
        /* 1n event selection: 1n = NeutronsFD_ind.size() = 1 with no charged particles (except electrons) and any number of other neutrals and particles with pdg=0. */
        bool event_selection_1n = (basic_event_selection && (Protons_ind.size() == 0) && (NeutronsFD_ind.size() == 1) &&
                                   (Enable_FD_photons || (PhotonsFD_ind.size() == 0)));

        if (calculate_1n && event_selection_1n) { // for 1n calculations (with any number of neutrals)

            //<editor-fold desc="Safety check (1n)">
            /* Safety check that we are looking at 1n */
            if (!(Enable_FD_photons || (PhotonsFD_ind.size() == 0))) {
                cout << "\n\n1n: PhotonsFD_ind.size() is non-zero (" << PhotonsFD_ind.size() << "). Exiting...\n\n", exit(EXIT_FAILURE);
            }

            if (NeutronsFD_ind.size() != 1) { cout << "\n\n1n: NeutronsFD_ind.size() is different than 1. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Protons_ind.size() != 0) { cout << "\n\n1n: Protons_ind.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kplus.size() != 0) { cout << "\n\n1n: Kplus.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kminus.size() != 0) { cout << "\n\n1n: Kminus.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Piplus_ind.size() != 0) { cout << "\n\n1n: Piplus_ind.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Piminus_ind.size() != 0) { cout << "\n\n1n: Piminus_ind.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (Electron_ind.size() != 1) { cout << "\n\n1n: Electron_ind.size() is different than 1. Exiting...\n\n", exit(EXIT_FAILURE); }
            if (deuterons.size() != 0) { cout << "\n\n1n: deuterons.size() is different than 0. Exiting...\n\n", exit(EXIT_FAILURE); }

            for (int &i: NeutronsFD_ind) {
                bool NeutronInPCAL_1n = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                if (NeutronInPCAL_1n) { cout << "\n\n1n: a neutron have been found with a PCAL hit. Exiting...\n\n", exit(EXIT_FAILURE); }
                if (!((allParticles[i]->par()->getPid() == 2112) || (allParticles[i]->par()->getPid() == 22))) {
                    cout << "\n\n1n: A neutron PDG is not 2112 or 22 (" << allParticles[i]->par()->getPid() << "). Exiting...\n\n", exit(EXIT_FAILURE);
                }
            }

            for (int &i: PhotonsFD_ind) {
                bool PhotonInPCAL_1n = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                if (!PhotonInPCAL_1n) { cout << "\n\n1n: a photon have been found without a PCAL hit. Exiting...\n\n", exit(EXIT_FAILURE); }
                if (allParticles[i]->par()->getPid() != 22) {
                    cout << "\n\n1n: A photon PDG is not 2112 or 22 (" << allParticles[i]->par()->getPid() << "). Exiting...\n\n", exit(EXIT_FAILURE);
                }
            }
            //</editor-fold>

            // looking at 1n events in the FD only & below theta_n cut:
            if (allParticles[NeutronsFD_ind.at(0)]->getRegion() == FD) {
                ++num_of_events_1n_inFD; // 1n event count after momentum and theta_n cuts

                int NeutronPDG = allParticles[NeutronsFD_ind.at(0)]->par()->getPid();

                bool NeutronInPCAL_1n = (allParticles[NeutronsFD_ind.at(0)]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
                bool NeutronInECIN_1n = (allParticles[NeutronsFD_ind.at(0)]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
                bool NeutronInECOUT_1n = (allParticles[NeutronsFD_ind.at(0)]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                //<editor-fold desc="Safety check (1n)">
                /* Safety check that we are looking at good neutron (BEFORE VETO!!!) */
                if (allParticles[NeutronsFD_ind.at(0)]->getRegion() != FD) { cout << "\n\n1n: neutron is not in FD. Exiting...\n\n", exit(EXIT_FAILURE); }
                if (!((NeutronPDG == 22) || (NeutronPDG == 2112))) {
                    cout << "\n\n1n: neutral PDG is not 2112 or 22 (" << NeutronPDG << "). Exiting...\n\n", exit(EXIT_FAILURE);
                }
                if (NeutronInPCAL_1n) { cout << "\n\n1n: neutron hit in PCAL. Exiting...\n\n", exit(EXIT_FAILURE); }
                if (!(NeutronInECIN_1n || NeutronInECOUT_1n)) { cout << "\n\n1n: no neutron hit in ECIN or ECOUT. Exiting...\n\n", exit(EXIT_FAILURE); }
                //</editor-fold>

                TVector3 P_e_1n_3v, q_1n_3v, P_n_1n_3v, P_T_e_1n_3v, P_T_n_1n_3v, dP_T_1n_3v, P_N_1n_3v;
                P_e_1n_3v.SetMagThetaPhi(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->getTheta(),
                                         electrons[Electron_ind.at(0)]->getPhi());                                                          // electron 3 momentum
                q_1n_3v = TVector3(Pvx - P_e_1n_3v.Px(), Pvy - P_e_1n_3v.Py(), Pvz - P_e_1n_3v.Pz());                                          // 3 momentum transfer
                P_n_1n_3v.SetMagThetaPhi(GetFDNeutronP(allParticles[NeutronsFD_ind.at(0)], apply_neutron_Beta_Fit), allParticles[NeutronsFD_ind.at(0)]->getTheta(),
                                         allParticles[NeutronsFD_ind.at(0)]->getPhi());                                                         // neutron 3 momentum
                P_T_e_1n_3v = TVector3(P_e_1n_3v.Px(), P_e_1n_3v.Py(), 0);                                                                    // electron t. momentum
                P_T_n_1n_3v = TVector3(P_n_1n_3v.Px(), P_n_1n_3v.Py(), 0);                                                                     // neutron t. momentum

                double E_e_1n = sqrt(m_e * m_e + P_e_1n_3v.Mag2()), E_n_1n = sqrt(m_n * m_n + P_n_1n_3v.Mag2()), Ecal_1n, dAlpha_T_1n, dPhi_T_1n;
                double Theta_p_e_p_n_1n, Theta_q_p_n_1n;

                // Fake neutrons handling (neutron veto) ------------------------------------------------------------------------------------------------------------

                //<editor-fold desc="Fake neutrons handling (neutron veto)">
                TVector3 n_hit_1n_3v, e_hit_1n_3v;

                double n_hit_Phi_1n, n_hit_Theta_1n, e_hit_Phi_1n, e_hit_Theta_1n, dPhi_hit_1n, dTheta_hit_1n;

                if (!NeutronInPCAL_1n && (NeutronInECIN_1n || NeutronInECOUT_1n)) { // if neutron did not hit PCAL, and hit either ECIN or ECOUT
                    auto n_detlayer_1n = NeutronInECIN_1n ? clas12::ECIN : clas12::ECOUT; // find first layer of hit

                    // neutron ECIN/ECAL hit vector and angles:
                    n_hit_1n_3v.SetXYZ(allParticles[NeutronsFD_ind.at(0)]->cal(n_detlayer_1n)->getX(),
                                       allParticles[NeutronsFD_ind.at(0)]->cal(n_detlayer_1n)->getY(),
                                       allParticles[NeutronsFD_ind.at(0)]->cal(n_detlayer_1n)->getZ());
                    n_hit_Theta_1n = n_hit_1n_3v.Theta() * 180 / pi, n_hit_Phi_1n = n_hit_1n_3v.Phi() * 180 / pi;

                    if ((n_detlayer_1n == clas12::ECIN) && (electrons[Electron_ind.at(0)]->cal(clas12::ECIN)->getZ() != 0)) {
                        e_hit_1n_3v.SetXYZ(electrons[Electron_ind.at(0)]->cal(clas12::ECIN)->getX(), electrons[Electron_ind.at(0)]->cal(clas12::ECIN)->getY(),
                                           electrons[Electron_ind.at(0)]->cal(clas12::ECIN)->getZ());
                        e_hit_Theta_1n = e_hit_1n_3v.Theta() * 180 / pi, e_hit_Phi_1n = e_hit_1n_3v.Phi() * 180 / pi;
                    } else if ((n_detlayer_1n == clas12::ECOUT) && (electrons[Electron_ind.at(0)]->cal(clas12::ECOUT)->getZ() != 0)) {
                        e_hit_1n_3v.SetXYZ(electrons[Electron_ind.at(0)]->cal(clas12::ECOUT)->getX(), electrons[Electron_ind.at(0)]->cal(clas12::ECOUT)->getY(),
                                           electrons[Electron_ind.at(0)]->cal(clas12::ECOUT)->getZ());
                        e_hit_Theta_1n = e_hit_1n_3v.Theta() * 180 / pi, e_hit_Phi_1n = e_hit_1n_3v.Phi() * 180 / pi;
                    } else {
                        int trajlayer = (n_detlayer_1n == clas12::ECIN) ? 4 : 7;
                        e_hit_1n_3v.SetXYZ(electrons[Electron_ind.at(0)]->traj(clas12::ECAL, trajlayer)->getX(),
                                           electrons[Electron_ind.at(0)]->traj(clas12::ECAL, trajlayer)->getY(),
                                           electrons[Electron_ind.at(0)]->traj(clas12::ECAL, trajlayer)->getZ());
                        e_hit_Theta_1n = e_hit_1n_3v.Theta() * 180 / pi, e_hit_Phi_1n = e_hit_1n_3v.Phi() * 180 / pi;
                    }

                    dPhi_hit_1n = n_hit_Phi_1n - e_hit_Phi_1n, dTheta_hit_1n = n_hit_Theta_1n - e_hit_Theta_1n;

                    //<editor-fold desc="Shift dPhi (from symmetry)">
                    if (dPhi_hit_1n > 180) {
                        dPhi_hit_1n = n_hit_Phi_1n - e_hit_Phi_1n - 360;
                    } else if (dPhi_hit_1n < -180) {
                        dPhi_hit_1n = n_hit_Phi_1n - e_hit_Phi_1n + 360;
                    }
                    //</editor-fold>

                    // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                    hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n.hFill(dPhi_hit_1n, dTheta_hit_1n, Weight);
                } // end of if neutron did not hit PCAL & hit either ECIN or ECOUT

                bool NeutronPassVeto = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, NeutronsFD_ind.at(0), Neutron_veto_cut.GetLowerCut());

                /* Log vetoed neutron values (for self-consistency) */
                if (!NeutronPassVeto) { hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n.hFill(dPhi_hit_1n, dTheta_hit_1n, Weight); }
                //</editor-fold>

                // Fillings 1n histograms ---------------------------------------------------------------------------------------------------------------------------

                //<editor-fold desc="Fillings 1n histograms">

                //<editor-fold desc="Truth-level 1n enforcement configuration">
                /* Configure rather to apply 1n truth level ES on rec. plots or not. */

                //<editor-fold desc="Setting TL ES conditions">
                auto mcpbank_1n = c12->mcparts();
                const Int_t Ngen_1n = mcpbank_1n->getRows();

                //<editor-fold desc="Particle counting">
                /* Particle index vectors */
                vector<int> TL_Electron_ind_1n, TL_Neutrons_ind_1n, TL_Protons_ind_1n;
                vector<int> TL_piplus_ind_1n, TL_piminus_ind_1n, TL_pizero_ind_1n, TL_Photons_ind_1n, TL_OtherPart_ind_1n;

                /* Particle index vectors (for FD particles) */
                vector<int> TL_NeutronsFD_ind_1n, TL_ProtonsFD_ind_1n, TL_pi0FD_ind_1n, TL_PhotonsFD_ind_1n;

                /* Particle index vectors (for particles above momentum threshold) */
                vector<int> TL_Electron_mom_ind_1n, TL_Neutrons_mom_ind_1n, TL_Protons_mom_ind_1n, TL_piplus_mom_ind_1n, TL_piminus_mom_ind_1n, TL_pizero_mom_ind_1n, TL_Photons_mom_ind_1n;

                /* Particle index vectors (for FD particles above momentum threshold) */
                vector<int> TL_NeutronsFD_mom_ind_1n, TL_ProtonsFD_mom_ind_1n, TL_pi0FD_mom_ind_1n, TL_PhotonsFD_mom_ind_1n;

                for (Int_t i = 0; i < Ngen_1n; i++) {
                    mcpbank_1n->setEntry(i);

                    int particlePDGtmp = mcpbank_1n->getPid();

                    double Particle_TL_Momentum = rCalc(mcpbank_1n->getPx(), mcpbank_1n->getPy(), mcpbank_1n->getPz());
                    double Particle_TL_Theta = acos((mcpbank_1n->getPz()) / rCalc(mcpbank_1n->getPx(), mcpbank_1n->getPy(), mcpbank_1n->getPz())) * 180.0 / pi;
                    double Particle_TL_Phi = atan2(mcpbank_1n->getPy(), mcpbank_1n->getPx()) * 180.0 / pi;

                    bool inFD = ((Particle_TL_Theta >= ThetaFD.GetLowerCut()) && (Particle_TL_Theta <= ThetaFD.GetUpperCut()));

                    if (particlePDGtmp == 11) {
                        if ((Particle_TL_Momentum >= TL_e_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_e_mom_cuts.GetUpperCut())) { TL_Electron_mom_ind_1n.push_back(i); }

                        TL_Electron_ind_1n.push_back(i);
                    } else if (particlePDGtmp == 2112) {
                        if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_Neutrons_mom_ind_1n.push_back(i); }

                        TL_Neutrons_ind_1n.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_n_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_n_mom_cuts.GetUpperCut())) { TL_NeutronsFD_mom_ind_1n.push_back(i); }

                            TL_NeutronsFD_ind_1n.push_back(i);
                        }
                    } else if (particlePDGtmp == 2212) {
                        if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_Protons_mom_ind_1n.push_back(i); }

                        TL_Protons_ind_1n.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_p_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_p_mom_cuts.GetUpperCut())) { TL_ProtonsFD_mom_ind_1n.push_back(i); }

                            TL_ProtonsFD_ind_1n.push_back(i);
                        }
                    } else if (particlePDGtmp == 211) {
                        if ((Particle_TL_Momentum >= TL_pip_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pip_mom_cuts.GetUpperCut())) { TL_piplus_mom_ind_1n.push_back(i); }

                        TL_piplus_ind_1n.push_back(i);
                    } else if (particlePDGtmp == -211) {
                        if ((Particle_TL_Momentum >= TL_pim_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pim_mom_cuts.GetUpperCut())) { TL_piminus_mom_ind_1n.push_back(i); }

                        TL_piminus_ind_1n.push_back(i);
                    } else if (particlePDGtmp == 111) {
                        if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pizero_mom_ind_1n.push_back(i); }

                        TL_pizero_ind_1n.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_pi0_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_pi0_mom_cuts.GetUpperCut())) { TL_pi0FD_mom_ind_1n.push_back(i); }

                            TL_pi0FD_ind_1n.push_back(i);
                        }
                    } else if (particlePDGtmp == 22) {
                        if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) &&
                            (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_Photons_mom_ind_1n.push_back(i); }

                        TL_Photons_ind_1n.push_back(i);

                        if (inFD) {
                            if ((Particle_TL_Momentum >= TL_ph_mom_cuts.GetLowerCut()) &&
                                (Particle_TL_Momentum <= TL_ph_mom_cuts.GetUpperCut())) { TL_PhotonsFD_mom_ind_1n.push_back(i); }

                            TL_PhotonsFD_ind_1n.push_back(i);
                        }
                    } else {
                        TL_OtherPart_ind_1n.push_back(i);
                    }
                }
                //</editor-fold>

                /* Setting up basic TL event selection */
                bool no_TL_cPions_1n = (TL_piplus_mom_ind_1n.size() == 0 && TL_piminus_mom_ind_1n.size() == 0);     // No id. cPions above momentum threshold
                bool no_TL_OtherPart_1n = (TL_OtherPart_ind_1n.size() == 0);                                        // No other part. above momentum threshold
                bool no_TL_FDpi0_1n = (Enable_FD_photons || (TL_pi0FD_mom_ind_1n.size() == 0));                     // No id. pi0 in the FD above momentum threshold
                bool no_TL_FDPhotons_1n = (Enable_FD_photons || (TL_PhotonsFD_mom_ind_1n.size() == 0));             // No id. photons in the FD above momentum threshold
                bool TL_Event_Selection_1e_cut_1n = (TL_Electron_mom_ind_1n.size() == 1);                           // One id. electron above momentum threshold
                bool TL_Basic_ES = (TL_Event_Selection_1e_cut_1n && no_TL_cPions_1n && no_TL_OtherPart_1n && no_TL_FDpi0_1n && no_TL_FDPhotons_1n);

                /* Setting up 1p TL event selection */
                bool one_FDproton_1p_1n = (TL_Protons_mom_ind_1n.size() == 1 && TL_ProtonsFD_mom_ind_1n.size() == 1);

                /* Setting up 1n TL event selection */
                bool one_FDNeutron_1n_1n = (TL_NeutronsFD_mom_ind_1n.size() == 1);
                bool no_protons_1n_1n = (TL_Protons_mom_ind_1n.size() == 0);
//                bool no_protons_1n_1n = (TL_ProtonsFD_mom_ind_1n.size() == 0);

                /* One id. FD proton above momentum threshold: */
                bool TL_Event_Selection_1p_1n = (TL_Basic_ES && one_FDproton_1p_1n);

                /* One id. FD neutron above momentum threshold & no id. protons: */
                bool TL_Event_Selection_1n_1n = (TL_Basic_ES && one_FDNeutron_1n_1n && no_protons_1n_1n);
                //</editor-fold>

                bool apply_TL_1n_ES;

                if (Rec_wTL_ES) { // Enforce TL event selection
                    apply_TL_1n_ES = TL_Event_Selection_1n_1n; // Events will be logged only is TL_Event_Selection_1n is true
                } else if (!Rec_wTL_ES) { // Don't enforce TL event selection
                    apply_TL_1n_ES = true; // Events will be logged regardless of TL_Event_Selection_1n is true
                }
                //</editor-fold>

                //<editor-fold desc="Applying neutron veto and Fillings 1n histograms">
                if (NeutronPassVeto && apply_TL_1n_ES) {
                    ++num_of_events_1n_inFD_AV;

                    //<editor-fold desc="Filling cut variable plots (1n)">
                    /* Filling Nphe plots (1n) */
                    hNphe_1n_FD.hFill(Nphe, Weight);

                    /* Filling electrton Chi2 histograms (1n) */
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                        hChi2_Electron_1n_FD.hFill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight);
                    }

                    /* Filling dVx, dVy, dVz (1n) */
                    //TODO: recheck with Adi rather or not to remove these plots
                    for (auto &p: neutrons) {
                        double Vx_n_1n = p->par()->getVx(), Vy_n_1n = p->par()->getVy(), Vz_n_1n = p->par()->getVz();
                        double dVx = Vx_n_1n - Vx_e, dVy = Vy_n_1n - Vy_e, dVz = Vz_n_1n - Vz_e;

                        hdVx_1n.hFill(dVx, Weight), hdVy_1n.hFill(dVy, Weight), hdVz_1n.hFill(dVz, Weight);
                    } // end of loop over neutrons vector

                    /* Filling SF histograms (1n) */
                    hSF_1n_FD.hFill(EoP_e, Weight), hSF_VS_P_e_1n_FD.hFill(P_e, EoP_e, Weight);

                    /* Filling fiducial plots (1n) */
                    hVcal_VS_EoP_1n_PCAL.hFill(electrons[Electron_ind.at(0)]->cal(clas12::PCAL)->getLv(), EoP_e, Weight);
                    hWcal_VS_EoP_1n_PCAL.hFill(electrons[Electron_ind.at(0)]->cal(clas12::PCAL)->getLw(), EoP_e, Weight);

                    //<editor-fold desc="Filling momentum threshold histograms (1n)">
                    /* Filling momentum histograms (1n) */

                    //<editor-fold desc="Electrton momentum (1n)">
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) { hP_e_APID_1n_FD.hFill(P_e, Weight); } // after mom. th.

                    for (int i = 0; i < Ne; i++) {
                        if (electrons[i]->getRegion() == FD) { hP_e_BPID_1n_FD.hFill(P_e, Weight); } // before mom. th.
                    }
                    //</editor-fold>

                    //<editor-fold desc="Neutron momentum (1n)">

                    //<editor-fold desc="Neutron momentum - all contributions (1n)">
//                    hP_n_APID_1n_FD.hFill(P_n_1n_3v.Mag(), Weight); // after mom. th.
//                    hP_n_APID_1n_ZOOMOUT_FD.hFill(P_n_1n_3v.Mag(), Weight); // after mom. th.
                    for (int &i: NeutronsFD_ind) {
                        hP_n_APID_1n_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); // after mom. th.
                        hP_n_APID_1n_ZOOMOUT_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); // after mom. th.
                    }

                    /* Neutron mom. after th. (verified neutrons) */
                    for (int &i: FD_Neutrons) {
                        hP_n_BPID_1n_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); // before mom. th.
                        hP_n_BPID_1n_ZOOMOUT_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); // before mom. th.
                    }
                    //</editor-fold>

                    //<editor-fold desc="Neutron momentum - verified neutrons (1n)">
                    /* Neutron mom. before th. (verified neutrons) */
                    for (int i = 0; i < neutrons.size(); i++) {
                        bool inPCALtmp = (neutrons[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                        bool inECINtmp = (neutrons[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
                        bool inECOUTtmp = (neutrons[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                        if ((neutrons[i]->getRegion() == FD) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                            hP_n_VN_BPID_1n_FD.hFill(neutrons[i]->getP(), Weight); // before mom. th.
                        }
                    }

                    /* Neutron mom. after th. (verified neutrons) */
                    for (int &i: NeutronsFD_ind) {
                        int ParticlePDGtmp = allParticles[i]->par()->getPid();

                        bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                        bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
                        bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                        if ((allParticles[i]->getRegion() == FD) && (ParticlePDGtmp == 2112) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                            hP_n_VN_APID_1n_FD.hFill(allParticles[i]->getP(), Weight); // after mom. th.
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Neutron momentum - from 'photons (1n)">
                    /* Neutron mom before cuts (from 'photons') */
                    for (int i = 0; i < allParticles.size(); i++) {
                        int ParticlePDGtmp = allParticles[i]->par()->getPid();

                        bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                        bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
                        bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                        // 'photon' mom before cuts:
                        if ((allParticles[i]->getRegion() == FD) && (ParticlePDGtmp == 22) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                            hP_n_Ph_BPID_1n_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); // before mom. th.
                        }
                    }

                    /* Neutron mom after cuts (from 'photons') */
                    for (int &i: NeutronsFD_ind) {
                        int ParticlePDGtmp = allParticles[i]->par()->getPid();

                        bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                        bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
                        bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                        if ((allParticles[i]->getRegion() == FD) && (ParticlePDGtmp == 22) && (!inPCALtmp && (inECINtmp || inECOUTtmp))) {
                            hP_n_Ph_APID_1n_FD.hFill(GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit), Weight); // after mom. th.
                        }
                    }
                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Proton momentum (1n)">
                    for (int &i: Protons_ind) {
                        if (protons[i]->getRegion() == CD) {
                            hP_p_APID_1n_CD.hFill(protons[i]->getP(), Weight); // after mom. th.
                        } else if (protons[i]->getRegion() == FD) {
                            hP_p_APID_1n_FD.hFill(protons[i]->getP(), Weight); // after mom. th.
                        }
                    }

                    for (int i = 0; i < Np; i++) {
                        if (protons[i]->getRegion() == CD) {
                            hP_p_BPID_1n_CD.hFill(protons[i]->getP(), Weight); // before mom cuts
                        } else if (protons[i]->getRegion() == FD) {
                            hP_p_BPID_1n_FD.hFill(protons[i]->getP(), Weight); // before mom cuts
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Piplus momentum (1n)">
                    for (int &i: Piplus_ind) {
                        if (piplus[i]->getRegion() == CD) {
                            hP_piplus_APID_1n_CD.hFill(piplus[i]->getP(), Weight); // after mom. th.
                        } else if (piplus[i]->getRegion() == FD) {
                            hP_piplus_APID_1n_FD.hFill(piplus[i]->getP(), Weight); // after mom. th.
                        }
                    }

                    for (int i = 0; i < Npip; i++) {
                        if (piplus[i]->getRegion() == CD) {
                            hP_piplus_BPID_1n_CD.hFill(piplus[i]->getP(), Weight); // before mom cuts
                        } else if (piplus[i]->getRegion() == FD) {
                            hP_piplus_BPID_1n_FD.hFill(piplus[i]->getP(), Weight); // before mom cuts
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Piminus momentum (1n)">
                    for (int &i: Piminus_ind) {
                        if (piminus[i]->getRegion() == CD) {
                            hP_piminus_APID_1n_CD.hFill(piminus[i]->getP(), Weight); // after mom. th.
                        } else if (piminus[i]->getRegion() == FD) {
                            hP_piminus_APID_1n_FD.hFill(piminus[i]->getP(), Weight); // after mom. th.
                        }
                    }

                    for (int i = 0; i < Npim; i++) {
                        if (piminus[i]->getRegion() == CD) {
                            hP_piminus_BPID_1n_CD.hFill(piminus[i]->getP(), Weight); // before mom cuts
                        } else if (piminus[i]->getRegion() == FD) {
                            hP_piminus_BPID_1n_FD.hFill(piminus[i]->getP(), Weight); // before mom cuts
                        }
                    }
                    //</editor-fold>

                    //<editor-fold desc="Photon momentum (1n)">
                    for (int &i: PhotonsFD_ind) { hP_ph_APID_1n_FD.hFill(allParticles[i]->getP(), Weight); } // after mom. th.

                    for (int &i: FD_Photons) { hP_ph_BPID_1n_FD.hFill(allParticles[i]->getP(), Weight); } // before mom. th.
                    //</editor-fold>

                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Filling Beta plots (1n)">

                    //<editor-fold desc="Fill Beta plots (1n, FD only)">

                    //<editor-fold desc="Beta plots for neutrons from 'photons' (1n, FD)">
                    for (int &i: NeutronsFD_ind) {
                        int PDGtmp = allParticles[i]->par()->getPid();
                        double P_n_temp = GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit);

                        bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                        bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
                        bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                        if (PDGtmp == 22) {
                            if (!(allParticles[i]->getRegion() == FD)) { cout << "\n\nBeta_n_1n: neutron is not in FD. Exiting...\n\n", exit(EXIT_FAILURE); }
                            if (!(!inPCALtmp && (inECINtmp || inECOUTtmp))) { cout << "\n\nBeta_n_1n: photon is not a neutron. Exiting...\n\n", exit(EXIT_FAILURE); }

                            hBeta_n_from_ph_01_1n_FD.hFill(allParticles[i]->par()->getBeta());
                            hBeta_n_from_ph_01_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            hBeta_vs_P_1n_Neutrons_Only_from_photons_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);

                            //<editor-fold desc="Beta_n_from_ph - !PCAL">
                            if (!inPCALtmp) {
                                hBeta_n_from_ph_02_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_n_from_ph_02_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }
                            //</editor-fold>

                            //<editor-fold desc="Beta_n_from_ph - !PCAL && ECIN">
                            if (!inPCALtmp && inECINtmp) {
                                hBeta_n_from_ph_03_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_n_from_ph_03_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }
                            //</editor-fold>

                            //<editor-fold desc="Beta_n_from_ph - !PCAL && !ECIN && ECOUT">
                            if (!inPCALtmp && !inECINtmp && inECOUTtmp) {
                                hBeta_n_from_ph_04_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_n_from_ph_04_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }
                            //</editor-fold>





                            //<editor-fold desc="Beta of neutrons from 'photons' for different variables">
                            hBeta_nfph_vs_numOfFDph_1n_FD.hFill(PhotonsFD_ind.size(), allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_Theta_n_1n_FD.hFill(allParticles[i]->getTheta() * 180 / pi, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_nfph_vs_Phi_n_1n_FD.hFill(allParticles[i]->getPhi() * 180 / pi, allParticles[i]->par()->getBeta(), Weight);

                            double neutron_SF = (allParticles[i]->cal(clas12::PCAL)->getEnergy() + allParticles[i]->cal(clas12::ECIN)->getEnergy() +
                                                 allParticles[i]->cal(clas12::ECOUT)->getEnergy()) / P_n_temp;
                            hBeta_nfph_vs_SF_1n_FD.hFill(neutron_SF, allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_SF_PCAL_1n_FD.hFill((allParticles[i]->cal(clas12::PCAL)->getEnergy()) / P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_nfph_vs_E_PCAL_1n_FD.hFill(allParticles[i]->cal(clas12::PCAL)->getEnergy(), allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_SF_ECIN_1n_FD.hFill((allParticles[i]->cal(clas12::ECIN)->getEnergy()) / P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_nfph_vs_E_ECIN_1n_FD.hFill(allParticles[i]->cal(clas12::ECIN)->getEnergy(), allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_SF_ECOUT_1n_FD.hFill((allParticles[i]->cal(clas12::ECOUT)->getEnergy()) / P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_nfph_vs_E_ECOUT_1n_FD.hFill(allParticles[i]->cal(clas12::ECOUT)->getEnergy(), allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_VcalECIN_1n_FD.hFill(allParticles[i]->cal(clas12::ECIN)->getLv(), allParticles[i]->par()->getBeta(), Weight);
                            hBeta_nfph_vs_WcalECIN_1n_FD.hFill(allParticles[i]->cal(clas12::ECIN)->getLw(), allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_VcalECOUT_1n_FD.hFill(allParticles[i]->cal(clas12::ECOUT)->getLv(), allParticles[i]->par()->getBeta(), Weight);
                            hBeta_nfph_vs_WcalECOUT_1n_FD.hFill(allParticles[i]->cal(clas12::ECOUT)->getLw(), allParticles[i]->par()->getBeta(), Weight);

                            hBeta_nfph_vs_P_e_1n_FD.hFill(electrons[Electron_ind.at(0)]->getP(), allParticles[i]->par()->getBeta(), Weight);
                            //</editor-fold>

                        }
                    }

                    //<editor-fold desc="Testing Theta_n_VS_Phi_n_around_beta1">
                    for (int &i: FD_Neutrons) {
                        int PDGtmp = allParticles[i]->par()->getPid();
                        double P_n_temp = allParticles[i]->getP();;

                        bool inPCALtmp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
                        bool inECINtmp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
                        bool inECOUTtmp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                        bool ided_neutron = (P_n_temp >= n_mom_th.GetLowerCut());

                        if (PDGtmp == 22 && ided_neutron) {
                            if (!(allParticles[i]->getRegion() == FD)) { cout << "\n\nBeta_n_1n: neutron is not in FD. Exiting...\n\n", exit(EXIT_FAILURE); }
                            if (!(!inPCALtmp && (inECINtmp || inECOUTtmp))) { cout << "\n\nBeta_n_1n: photon is not a neutron. Exiting...\n\n", exit(EXIT_FAILURE); }

                            if (fabs(allParticles[i]->par()->getBeta() - Beta_cut.GetMean()) <= Beta_cut.GetUpperCut()) {
                                hTheta_n_VS_Phi_n_around_beta1_1n_FD.hFill(allParticles[i]->getPhi() * 180.0 / pi, allParticles[i]->getTheta() * 180.0 / pi, Weight);
                            }
                        }
                    }
                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Filling Beta plots from FD neutrrals">
                    for (int i = 0; i < allParticles.size(); i++) {
                        if ((allParticles[i]->getRegion() == FD) && (allParticles[i]->par()->getCharge() == 0)) {
                            bool inPCAL_1n_temp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
                            bool inECIN_1n_temp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
                            bool inECOUT_1n_temp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

                            //<editor-fold desc="All_FD_Neut">
                            hBeta_All_FD_Neut_01_1n_FD.hFill(allParticles[i]->par()->getBeta());
                            hBeta_All_FD_Neut_01_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());

                            if (allParticles[i]->par()->getPid() != 0) {
                                hBeta_FD_Neut_noPDG0_01_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_FD_Neut_noPDG0_01_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                            }
                            //</editor-fold>

                            //<editor-fold desc="All_FD_Neut - !PCAL">
                            if (!inPCAL_1n_temp) {
                                hBeta_All_FD_Neut_02_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_All_FD_Neut_02_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());

                                if (allParticles[i]->par()->getPid() != 0) {
                                    hBeta_FD_Neut_noPDG0_02_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                    hBeta_FD_Neut_noPDG0_02_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                                }
                            }
                            //</editor-fold>

                            //<editor-fold desc="All_FD_Neut - !PCAL && ECIN">
                            if (!inPCAL_1n_temp && inECIN_1n_temp) {
                                hBeta_All_FD_Neut_03_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_All_FD_Neut_03_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());

                                if (allParticles[i]->par()->getPid() != 0) {
                                    hBeta_FD_Neut_noPDG0_03_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                    hBeta_FD_Neut_noPDG0_03_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                                }
                            }
                            //</editor-fold>

                            //<editor-fold desc="All_FD_Neut - !PCAL && !ECIN && ECOUT">
                            if (!inPCAL_1n_temp && !inECIN_1n_temp && inECOUT_1n_temp) {
                                hBeta_All_FD_Neut_04_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                hBeta_All_FD_Neut_04_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());

                                if (allParticles[i]->par()->getPid() != 0) {
                                    hBeta_FD_Neut_noPDG0_04_1n_FD.hFill(allParticles[i]->par()->getBeta());
                                    hBeta_FD_Neut_noPDG0_04_1n_ZOOMOUT_FD.hFill(allParticles[i]->par()->getBeta());
                                }
                            }
                            //</editor-fold>
                        }
                    }
                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from identified particles">

                    //<editor-fold desc="Beta vs. P from identified electrons (1n, FD)">
                    /* loop over Electron_ind, so that, is Electron_ind.size() != 1, you'll see what thw other electrons have */
                    for (int &i: Electron_ind) {
                        double P_e_temp = electrons[i]->getP();

                        if (electrons[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Electrons_Only_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neg_part_1n_FD.hFill(P_e_temp, electrons[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over identified electrons vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from identified neutrons (1n, FD)">
                    for (int &i: NeutronsFD_ind) {
                        double P_n_temp = GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit);

                        if (allParticles[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Neutrons_Only_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neut_part_1n_CD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                        } else if (allParticles[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Neutrons_Only_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neut_part_1n_FD.hFill(P_n_temp, allParticles[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over identified neutrons vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from identified photons (1n, FD)">
                    for (int &i: PhotonsFD_ind) {
                        double P_ph_temp = allParticles[i]->getP();

                        if (allParticles[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Photons_Only_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neut_part_1n_CD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight);
                        } else if (allParticles[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Photons_Only_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neut_part_1n_FD.hFill(P_ph_temp, allParticles[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over identified photons vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from identified protons (1n, FD)">
                    for (int &i: Protons_ind) {
                        double P_p_temp = protons[i]->getP();

                        if (protons[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Protons_Only_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_pos_part_1n_CD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight);
                        } else if (protons[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Protons_Only_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_pos_part_1n_FD.hFill(P_p_temp, protons[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over identified protons vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from identified piplus (1n, FD)">
                    for (int &i: Piplus_ind) {
                        double P_pip_temp = piplus[i]->getP();

                        if (piplus[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Piplus_Only_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_pos_part_1n_CD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight);
                        } else if (piplus[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Piplus_Only_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_pos_part_1n_FD.hFill(P_pip_temp, piplus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over identified piplus vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from identified piminus (1n, FD)">
                    for (int &i: Piminus_ind) {
                        double P_pim_temp = piminus[i]->getP();

                        if (piminus[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Piminus_Only_CD.hFill(P_pim_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neg_part_1n_CD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight);
                        } else if (piminus[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Piminus_Only_FD.hFill(P_pim_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neg_part_1n_FD.hFill(P_pim_temp, piminus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over identified piminus vector
                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from other particles (1n, FD)">
                    /* This is for self-consistency. Contributions from other particles should be zero */

                    //<editor-fold desc="Beta vs. P from Kplus (1n, FD)">
                    for (int i = 0; i < Kplus.size(); i++) {
                        double P_Kp_temp = Kplus[i]->getP();

                        if (Kplus[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Kplus_Only_CD.hFill(P_Kp_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_pos_part_1n_CD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight);
                        } else if (Kplus[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Kplus_Only_FD.hFill(P_Kp_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_pos_part_1n_FD.hFill(P_Kp_temp, Kplus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over Kplus vector
                    //</editor-fold>

                    //<editor-fold desc="Beta vs. P from Kminus (1n, FD)">
                    for (int i = 0; i < Kminus.size(); i++) {
                        double P_Km_temp = Kminus[i]->getP();

                        if (Kminus[i]->getRegion() == CD) {
                            hBeta_vs_P_1n_CD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Kminus_Only_CD.hFill(P_Km_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neg_part_1n_CD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight);
                        } else if (Kminus[i]->getRegion() == FD) {
                            hBeta_vs_P_1n_FD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_1n_Kminus_Only_FD.hFill(P_Km_temp, piplus[i]->par()->getBeta(), Weight);
                            hBeta_vs_P_neg_part_1n_FD.hFill(P_Km_temp, Kminus[i]->par()->getBeta(), Weight);
                        }
                    } // end of loop over Kminus vector
                    //</editor-fold>

                    //</editor-fold>

                    //</editor-fold>

                    //<editor-fold desc="Filling electron plots (1n)">
                    if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                        hTheta_e_All_Int_1n_FD->Fill(Theta_e, Weight);
                        hPhi_e_All_Int_1n_FD->Fill(Phi_e, Weight);
                        hTheta_e_VS_Phi_e_1n_FD->Fill(Phi_e, Theta_e, Weight);

                        hE_e_All_Int_1n_FD->Fill(E_e, Weight);
                        hE_e_VS_Theta_e_All_Int_1n_FD->Fill(Theta_e, E_e, Weight);

                        hET_All_Ang_All_Int_1n_FD->Fill(beamE - E_e, Weight);

                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_All_Int_1n_FD->Fill(beamE - E_e, Weight);
                            hE_e_15_All_Int_1n_FD->Fill(E_e, Weight);
                        }

                        if (qel) {
                            hTheta_e_QEL_1n_FD->Fill(Theta_e, Weight);
                            hPhi_e_QEL_1n_FD->Fill(Phi_e, Weight);
                            hE_e_QEL_1n_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_QEL_1n_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_QEL_1n_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_QEL_1n_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_QEL_1n_FD->Fill(E_e, Weight);
                            }
                        } else if (mec) {
                            hTheta_e_MEC_1n_FD->Fill(Theta_e, Weight);
                            hPhi_e_MEC_1n_FD->Fill(Phi_e, Weight);
                            hE_e_MEC_1n_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_MEC_1n_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_MEC_1n_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_MEC_1n_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_MEC_1n_FD->Fill(E_e, Weight);
                            }
                        } else if (res) {
                            hTheta_e_RES_1n_FD->Fill(Theta_e, Weight);
                            hPhi_e_RES_1n_FD->Fill(Phi_e, Weight);
                            hE_e_RES_1n_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_RES_1n_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_RES_1n_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_RES_1n_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_RES_1n_FD->Fill(E_e, Weight);
                            }
                        } else if (dis) {
                            hTheta_e_DIS_1n_FD->Fill(Theta_e, Weight);
                            hPhi_e_DIS_1n_FD->Fill(Phi_e, Weight);
                            hE_e_DIS_1n_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_DIS_1n_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_DIS_1n_FD->Fill(beamE - E_e, Weight);

                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_DIS_1n_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_DIS_1n_FD->Fill(E_e, Weight);
                            }
                        }

                        /* Filling Q2 histograms (1n) */
                        hQ2_1n_FD->Fill(Q2, Weight);
                    }
                    //</editor-fold>

                    //<editor-fold desc="Filling Ecal plots (1n)">
                    Ecal_1n = E_e_1n + (E_n_1n - m_n);

                    if (Ecal_1n > beamE) { hChi2_Electron_Ecal_test_1n->Fill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight); }

                    hEcal_All_Int_1n->Fill(Ecal_1n, Weight); // Fill Ecal for all interactions

                    if (qel) {
                        hEcal_QEL_1n->Fill(Ecal_1n, Weight); // Fill Ecal for QEL only
                    } else if (mec) {
                        hEcal_MEC_1n->Fill(Ecal_1n, Weight); // Fill Ecal for MEC only
                    } else if (res) {
                        hEcal_RES_1n->Fill(Ecal_1n, Weight); // Fill Ecal for RES only
                    } else if (dis) {
                        hEcal_DIS_1n->Fill(Ecal_1n, Weight); // Fill Ecal for DIS only
                    }
                    //</editor-fold>

                    dP_T_1n_3v = TVector3(P_T_e_1n_3v.Px() + P_T_n_1n_3v.Px(), P_T_e_1n_3v.Py() + P_T_n_1n_3v.Py(), 0);
                    dAlpha_T_1n = acos(-(P_e_1n_3v.Px() * dP_T_1n_3v.Px() + P_e_1n_3v.Py() * dP_T_1n_3v.Py() + P_e_1n_3v.Pz() * dP_T_1n_3v.Pz())
                                       / (P_T_e_1n_3v.Mag() * dP_T_1n_3v.Mag())) * 180.0 / pi;                             // dP_T_1n_3v.Pz() = 0; dAlpha_T_1n in deg
                    hdP_T_1n->Fill(dP_T_1n_3v.Mag(), Weight);
                    hdAlpha_T_1n->Fill(dAlpha_T_1n, Weight);
                    hdP_T_vs_dAlpha_T_1n->Fill(dAlpha_T_1n, dP_T_1n_3v.Mag(), Weight);

                    dPhi_T_1n = acos(-(P_T_e_1n_3v.Px() * P_T_n_1n_3v.Px() + P_T_e_1n_3v.Py() * P_T_n_1n_3v.Py() + P_T_e_1n_3v.Pz() * P_T_n_1n_3v.Pz())
                                     / (P_T_e_1n_3v.Mag() * P_T_n_1n_3v.Mag())) * 180.0 / pi;                               // P_T_n_1n_3v.Pz() = 0; dPhi_T_1n in deg
                    hdPhi_T_1n->Fill(dPhi_T_1n, Weight);

                    hEcal_vs_dAlpha_T_1n->Fill(dAlpha_T_1n, Ecal_1n, Weight);
                    hEcal_vs_dP_T_1n->Fill(dP_T_1n_3v.Mag(), Ecal_1n, Weight);


                    hTheta_n_All_Int_1n->Fill(allParticles[NeutronsFD_ind.at(0)]->getTheta() * 180.0 / pi, Weight);
                    hPhi_n_All_Int_1n->Fill(allParticles[NeutronsFD_ind.at(0)]->getPhi() * 180.0 / pi, Weight);

                    Theta_p_e_p_n_1n = acos((P_e_1n_3v.Px() * P_n_1n_3v.Px() + P_e_1n_3v.Py() * P_n_1n_3v.Py() + P_e_1n_3v.Pz() * P_n_1n_3v.Pz())
                                            / (P_e_1n_3v.Mag() * P_n_1n_3v.Mag())) * 180.0 / pi;                                           // Theta_p_e_p_n_1n in deg
                    hTheta_p_e_p_n_1n->Fill(Theta_p_e_p_n_1n, Weight);

                    Theta_q_p_n_1n = acos((q_1n_3v.Px() * P_n_1n_3v.Px() + q_1n_3v.Py() * P_n_1n_3v.Py() + q_1n_3v.Pz() * P_n_1n_3v.Pz())
                                          / (q_1n_3v.Mag() * P_n_1n_3v.Mag())) * 180.0 / pi;                                                 // Theta_q_p_n_1n in deg
                    hTheta_q_p_n_1n->Fill(Theta_q_p_n_1n, Weight);

                    hTheta_q_p_n_vs_p_n_q_1n->Fill(P_n_1n_3v.Mag() / q_1n_3v.Mag(), Theta_q_p_n_1n, Weight);

                    P_N_1n_3v = TVector3(P_e_1n_3v.Px() + P_n_1n_3v.Px() - Pvx, P_e_1n_3v.Py() + P_n_1n_3v.Py() - Pvy, P_e_1n_3v.Pz() + P_n_1n_3v.Pz() - Pvz);
                    hTheta_q_p_n_vs_p_N_q_1n->Fill(P_N_1n_3v.Mag() / q_1n_3v.Mag(), Theta_q_p_n_1n, Weight);


                    hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n.hFill(dPhi_hit_1n, dTheta_hit_1n, Weight);

                } // end of if NeutronPassVeto is true (i.e. if neutron did not hit PCAL & hit either ECIN or ECOUT)
                //</editor-fold>

                //</editor-fold>

            } // end of proton in FD if
        } // end of 1n cuts if
        //</editor-fold>

//  1n1p ----------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1n1p">
        if (calculate_1n1p && ((Nf_Prime == 2) && (Np == 1) && (Nn == 1))) { // for 1n1p calculations (with any number of other neutrals)
            ++num_of_events_1e1n1p_wFakeNeut;

            //<editor-fold desc="Safty check (1n1p)">
            /* Safety check that we are looking at 1n1p */
            if (neutrons.size() != 1) { cout << "\n\n1n1p: neutrons.size() is different than 1 exiting...\n\n", exit(EXIT_FAILURE); }
            if (protons.size() != 1) { cout << "\n\n1n1p: protons.size() is different than 1 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kplus.size() != 0) { cout << "\n\n1n1p: Kplus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kminus.size() != 0) { cout << "\n\n1n1p: Kminus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (piplus.size() != 0) { cout << "\n\n1n1p: piplus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (piminus.size() != 0) { cout << "\n\n1n1p: piminus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (piminus.size() != 0) { cout << "\n\n1n1p: piminus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (electrons.size() != 1) { cout << "\n\n1n1p: electrons.size() is different than 1 exiting...\n\n", exit(EXIT_FAILURE); }
            if (deuterons.size() != 0) { cout << "\n\n1n1p: deuterons.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            //</editor-fold>

            //todo: add mom cuts for 1n1p

            //TODO: rename and move from ToF to angles folder
            bool e_hit_PCAL_1n1p = (electrons[0]->cal(clas12::PCAL)->getDetector() == 7); // check if electron hit the PCAL
            bool p_hit_PCAL_1n1p = (protons[0]->cal(clas12::PCAL)->getDetector() == 7); // check if proton hit the PCAL

            bool n_hit_PCAL_1n1p = (neutrons[0]->cal(clas12::PCAL)->getDetector() == 7); // check if neutron hit the PCAL
            bool n_hit_ECIN_1n1p = (neutrons[0]->cal(clas12::ECIN)->getDetector() == 7); // check if neutron hit the ECIN
            bool n_hit_ECOUT_1n1p = (neutrons[0]->cal(clas12::ECOUT)->getDetector() == 7); // check if neutron hit the ECOUT

            if (!n_hit_PCAL_1n1p && (n_hit_ECIN_1n1p || n_hit_ECOUT_1n1p)) { // if neutron did not hit PCAL & hit either ECIN or ECOUT
                auto n_detlayer_1n1p = n_hit_ECIN_1n1p ? clas12::ECIN : clas12::ECOUT; // find first layer of hit

                // neutron ECIN/ECAL hit vector and angles:
                TVector3 n_hit_1n1p_3v(neutrons[0]->cal(n_detlayer_1n1p)->getX(), neutrons[0]->cal(n_detlayer_1n1p)->getY(), neutrons[0]->cal(n_detlayer_1n1p)->getZ());
                double n_hit_Theta_1n1p = n_hit_1n1p_3v.Theta() * 180 / pi, n_hit_Phi_1n1p = n_hit_1n1p_3v.Phi() * 180 / pi;

                if (e_hit_PCAL_1n1p) { // if there's an electron hit in the PCAL
                    // electron PCAL hit vector and angles:
                    TVector3 e_hit_1n1p_3v(electrons[0]->cal(clas12::PCAL)->getX(), electrons[0]->cal(clas12::PCAL)->getY(), electrons[0]->cal(clas12::PCAL)->getZ());
                    double e_hit_Theta_1n1p = e_hit_1n1p_3v.Theta() * 180 / pi, e_hit_Phi_1n1p = e_hit_1n1p_3v.Phi() * 180 / pi;

                    // subtracting the angles between the neutron hit and electron hit to see if we have fake neutrons:
                    hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n1p.hFill(n_hit_Phi_1n1p - e_hit_Phi_1n1p, n_hit_Theta_1n1p - e_hit_Theta_1n1p, Weight);
                }

                if (p_hit_PCAL_1n1p) { // if there's an proton hit in the PCAL
                    // proton PCAL hit vector and angles:
                    TVector3 p_hit_1n1p_3v(protons[0]->cal(clas12::PCAL)->getX(), protons[0]->cal(clas12::PCAL)->getY(), protons[0]->cal(clas12::PCAL)->getZ());
                    double p_hit_Theta_1n1p = p_hit_1n1p_3v.Theta() * 180 / pi, p_hit_Phi_1n1p = p_hit_1n1p_3v.Phi() * 180 / pi;

                    // subtracting the angles between the neutron hit and proton hit to see if we have fake neutrons:
                    hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1n1p.hFill(n_hit_Phi_1n1p - p_hit_Phi_1n1p, n_hit_Theta_1n1p - p_hit_Theta_1n1p, Weight);
                }
            }
        } // end of 1n1p cuts if
        //</editor-fold>

//  1e2pXy (or (e,e'pp)X) -----------------------------------------------------------------------------------------------------------------------------------------------

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

//  1e2p & 2p cuts ------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e2p & 2p cuts">
        /* 2p event selection: 2p = Protons_ind.size() = 2 and any number of neutrons, other neutrals and particles with pdg=0. */
        bool event_selection_2p = (basic_event_selection && (Protons_ind.size() == 2));

        if (calculate_2p && event_selection_2p) { // for 2p calculations (with any number of neutrals, neutrons and pdg=0)
//        if ((calculate_2p == true) && ((Nf == 3) && (Np == 2))) { // for 2p calculations
            ++num_of_events_with_1e2p; // logging #(events) w/ 1e2p

            //<editor-fold desc="Safty check (2p)">
            /* Safety check that we are looking at 2p */
            if (Protons_ind.size() != 2) { cout << "\n\n2p: Protons_ind.size() is different than 2 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kplus.size() != 0) { cout << "\n\n2p: Kplus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Kminus.size() != 0) { cout << "\n\n2p: Kminus.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Piplus_ind.size() != 0) { cout << "\n\n2p: Piplus_ind.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Piminus_ind.size() != 0) { cout << "\n\n2p: Piminus_ind.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            if (Electron_ind.size() != 1) { cout << "\n\n2p: Electron_ind.size() is different than 1 exiting...\n\n", exit(EXIT_FAILURE); }
            if (deuterons.size() != 0) { cout << "\n\n2p: deuterons.size() is different than 0 exiting...\n\n", exit(EXIT_FAILURE); }
            //</editor-fold>

            /* NOTE: p_first corresponds to protons[Protons_ind.at(0)] & p_second corresponds to protons[Protons_ind.at(1)] */
            TVector3 P_e_2p_3v, q_2p_3v, P_p_first_2p_3v, P_p_second_2p_3v, P_tot_2p_3v, P_1_2p_3v, P_2_2p_3v, P_T_e_2p_3v, P_T_L_2p_3v, P_T_tot_2p_3v, dP_T_L_2p_3v, dP_T_tot_2p_3v;

            P_e_2p_3v.SetMagThetaPhi(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->getTheta(),
                                     electrons[Electron_ind.at(0)]->getPhi());                                              // electron 3 momentum
            q_2p_3v = TVector3(Pvx - P_e_2p_3v.Px(), Pvy - P_e_2p_3v.Py(), Pvz - P_e_2p_3v.Pz());                               // 3 momentum transfer
            P_T_e_2p_3v = TVector3(P_e_2p_3v.Px(), P_e_2p_3v.Py(), 0);                                                          // electron transverse momentum
            P_p_first_2p_3v.SetMagThetaPhi(protons[Protons_ind.at(0)]->getP(), protons[Protons_ind.at(0)]->getTheta(),
                                           protons[Protons_ind.at(0)]->getPhi());                                              // first proton in protons vector
            P_p_second_2p_3v.SetMagThetaPhi(protons[Protons_ind.at(1)]->getP(), protons[Protons_ind.at(1)]->getTheta(),
                                            protons[Protons_ind.at(1)]->getPhi());                                             // second proton in protons vector

            double E_e_2p = sqrt(m_e * m_e + P_e_2p_3v.Mag2()), omega_2p = beamE - E_e_2p, W_2p = sqrt((omega_2p + m_p) * (omega_2p + m_p) - q_2p_3v.Mag2());
            double E_1_2p, E_2_2p, Theta_p_e_p_tot_2p, Theta_q_p_tot_2p, Theta_q_p_L_2p, Theta_q_p_R_2p;
            double dAlpha_T_L_2p, dAlpha_T_tot_2p, dPhi_T_L_2p, dPhi_T_tot_2p, Ecal_2p;

            //<editor-fold desc="Determining leading and recoil protons">
            int lead_p_ind, recoil_p_ind;

            if (P_p_first_2p_3v.Mag() >= P_p_second_2p_3v.Mag()) { // If p_first = protons[0] is leading proton
                P_1_2p_3v = TVector3(P_p_first_2p_3v.Px(), P_p_first_2p_3v.Py(), P_p_first_2p_3v.Pz());
                P_2_2p_3v = TVector3(P_p_second_2p_3v.Px(), P_p_second_2p_3v.Py(), P_p_second_2p_3v.Pz());
                lead_p_ind = Protons_ind.at(0), recoil_p_ind = Protons_ind.at(1);
//                lead_p_ind = 0, recoil_p_ind = 1;
            } else { // else if p_second = protons[Protons_ind.at(1)] is leading proton
                P_2_2p_3v = TVector3(P_p_first_2p_3v.Px(), P_p_first_2p_3v.Py(), P_p_first_2p_3v.Pz());
                P_1_2p_3v = TVector3(P_p_second_2p_3v.Px(), P_p_second_2p_3v.Py(), P_p_second_2p_3v.Pz());
                lead_p_ind = Protons_ind.at(1), recoil_p_ind = Protons_ind.at(0);
//                lead_p_ind = 1, recoil_p_ind = 0;
            }
            //</editor-fold>

            /* Angles of leading and recoil protons */
            double Theta_p1 = P_1_2p_3v.Theta() * 180.0 / pi, Theta_p2 = P_2_2p_3v.Theta() * 180.0 / pi;                                    // Theta_p1, Theta_p2 in deg
            double Phi_p1 = P_1_2p_3v.Phi() * 180.0 / pi, Phi_p2 = P_2_2p_3v.Phi() * 180.0 / pi;                                                // Phi_p1, Phi_p2 in deg
            double Theta_p1_p2_2p = acos((P_1_2p_3v.Px() * P_2_2p_3v.Px() + P_1_2p_3v.Py() * P_2_2p_3v.Py() + P_1_2p_3v.Pz() * P_2_2p_3v.Pz())
                                         / (P_1_2p_3v.Mag() * P_2_2p_3v.Mag())) * 180.0 / pi;                                                   // Theta_p1_p2_2p in deg

            //  Testing cuts --------------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing cuts">

            //<editor-fold desc="Testing momentum cuts (protons only)">
            /* momentum before and after cuts */
            if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                hP_e_1e2p_BC_FD.hFill(P_e, Weight); // momentum before cuts
                hP_e_1e2p_AC_FD.hLogEventCuts(electrons[Electron_ind.at(0)], e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, Weight);
            }

            if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                hP_p_1e2p_BC_CD.hFill(P_p_first_2p_3v.Mag(), Weight); // momentum before cuts
                hP_p_1e2p_AC_CD.hLogEventCuts(protons[Protons_ind.at(0)], p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, Weight);
            } else if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                hP_p_1e2p_BC_FD.hFill(P_p_first_2p_3v.Mag(), Weight); // momentum before cuts
                hP_p_1e2p_AC_FD.hLogEventCuts(protons[Protons_ind.at(0)], p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, Weight);
            }

            if (protons[Protons_ind.at(1)]->getRegion() == CD) {
                hP_p_1e2p_BC_CD.hFill(P_p_second_2p_3v.Mag(), Weight); // momentum before cuts
                hP_p_1e2p_AC_CD.hLogEventCuts(protons[Protons_ind.at(1)], p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, Weight);
            } else if (protons[Protons_ind.at(1)]->getRegion() == FD) {
                hP_p_1e2p_BC_FD.hFill(P_p_second_2p_3v.Mag(), Weight); // momentum before cuts
                hP_p_1e2p_AC_FD.hLogEventCuts(protons[Protons_ind.at(1)], p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, Weight);
            }
            //</editor-fold>

            //<editor-fold desc="Testing chi2 cuts (protons only)">
            if (apply_cuts == false) {
                /* Chi2 before cut */
                if (electrons[Electron_ind.at(0)]->getRegion() == FD) { hChi2_Electron_1e2p_BC_FD.hFill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight); }

                if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                } else if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                }

                if (protons[Protons_ind.at(1)]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                } else if (protons[Protons_ind.at(1)]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                }

                /* Chi2 after cut */
                if ((electrons[Electron_ind.at(0)]->getRegion() == FD) &&
                    (fabs(Chi2_Electron_cuts_FD.Cuts.at(0) - electrons[Electron_ind.at(0)]->par()->getChi2Pid()) <= Chi2_Electron_cuts_FD.Cuts.at(2))) {
                    hChi2_Electron_1e2p_AC_FD.hFill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight);
                }

                if ((protons[Protons_ind.at(0)]->getRegion() == CD) &&
                    (fabs(Chi2_Proton_cuts_CD.Cuts.at(0) - protons[Protons_ind.at(0)]->par()->getChi2Pid()) <= Chi2_Proton_cuts_CD.Cuts.at(2))) {
                    hChi2_Proton_1e2p_AC_CD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                } else if ((protons[Protons_ind.at(0)]->getRegion() == FD) &&
                           (fabs(Chi2_Proton_cuts_FD.Cuts.at(0) - protons[Protons_ind.at(0)]->par()->getChi2Pid()) <= Chi2_Proton_cuts_FD.Cuts.at(2))) {
                    hChi2_Proton_1e2p_AC_FD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                }

                if ((protons[Protons_ind.at(1)]->getRegion() == CD) &&
                    (fabs(Chi2_Proton_cuts_CD.Cuts.at(0) - protons[Protons_ind.at(1)]->par()->getChi2Pid()) <= Chi2_Proton_cuts_CD.Cuts.at(2))) {
                    hChi2_Proton_1e2p_AC_CD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                } else if ((protons[Protons_ind.at(1)]->getRegion() == FD) &&
                           (fabs(Chi2_Proton_cuts_FD.Cuts.at(0) - protons[Protons_ind.at(1)]->par()->getChi2Pid()) <= Chi2_Proton_cuts_FD.Cuts.at(2))) {
                    hChi2_Proton_1e2p_AC_FD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                }
            } else {
                if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                    hChi2_Electron_1e2p_BC_FD.hFill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight);
                }

                if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                } else if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                }

                if (protons[Protons_ind.at(1)]->getRegion() == CD) {
                    hChi2_Proton_1e2p_BC_CD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                } else if (protons[Protons_ind.at(1)]->getRegion() == FD) {
                    hChi2_Proton_1e2p_BC_FD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                }
            }
            //</editor-fold>

            //<editor-fold desc="Testing dV cuts">
//            double Vx_e = electrons[Electron_ind.at(0)]->par()->getVx(), Vy_e = electrons[Electron_ind.at(0)]->par()->getVy(), Vz_e = electrons[Electron_ind.at(0)]->par()->getVz();
            double Vz_p0 = protons[Protons_ind.at(0)]->par()->getVz(), Vz_p1 = protons[Protons_ind.at(1)]->par()->getVz(), dVz0 = Vz_p0 - Vz_e, dVz1 = Vz_p1 - Vz_e;
            double Vx_p_1e2p, Vy_p_1e2p, Vz_p_1e2p, dVx, dVy, dVz;

            for (auto &p: protons) {
                Vx_p_1e2p = p->par()->getVx(), Vy_p_1e2p = p->par()->getVy(), Vz_p_1e2p = p->par()->getVz();
                dVx = Vx_p_1e2p - Vx_e, dVy = Vy_p_1e2p - Vy_e, dVz = Vz_p_1e2p - Vz_e;

                if (apply_cuts == false) {
                    /* Testing dVx,dVy,dVz before cuts */
                    hdVx_1e2p_BC.hFill(dVx, Weight), hdVy_1e2p_BC.hFill(dVy, Weight), hdVz_1e2p_BC.hFill(dVz, Weight);

                    /* Testing dVx,dVy,dVz after cuts */
                    if (fabs(dVx) <= -1) {
                        hdVx_1e2p_AC.hFill(dVx, Weight);
                    } // no dVx cuts
                    if (fabs(dVy) <= -1) {
                        hdVy_1e2p_AC.hFill(dVy, Weight);
                    } // no dVx cuts
                    if (fabs(dVz) <= dVz_cuts.GetUpperCut()) {
                        hdVz_1e2p_AC.hFill(dVz, Weight);
                    }
                } else {
                    hdVx_1e2p_BC.hFill(dVx, Weight), hdVy_1e2p_BC.hFill(dVy, Weight), hdVz_1e2p_BC.hFill(dVz, Weight);
                }
            } // end of loop over protons vector
            //</editor-fold>

            //</editor-fold>

            //  Ghost tracks handling (2p) -----------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Ghost tracks handling (2p)">

            //<editor-fold desc="Single edge detection cut (on CD-FD border)">
//            cout << "\np1_Theta_p_cuts_2p.GetMean() = " << p1_Theta_p_cuts_2p.GetMean() << "\n";
//            cout << "p2_Theta_p_cuts_2p.GetMean() = " << p2_Theta_p_cuts_2p.GetMean() << "\n";

            bool p1_Theta_p_cut = (fabs(Theta_p1 - p1_Theta_p_cuts_2p.GetMean()) < p1_Theta_p_cuts_2p.GetUpperCut()); // Lead proton theta_p cut
            bool p2_Theta_p_cut = (fabs(Theta_p2 - p2_Theta_p_cuts_2p.GetMean()) < p2_Theta_p_cuts_2p.GetUpperCut()); // Recoil proton theta_p cut
            bool Lead_and_Recoil_with_close_phi = (fabs(Phi_p1 - Phi_p2) < phi_p1_p2_diff_cuts_2p.GetUpperCut());
//            bool p1_Theta_p_cut = (fabs(Theta_p1 - 40.) < 5.); // Lead proton theta_p cut
//            bool p2_Theta_p_cut = (fabs(Theta_p2 - 40.) < 5.); // Recoil proton theta_p cut
//            bool Lead_and_Recoil_with_close_phi = (fabs(Phi_p1 - Phi_p2) < 15.);

            //best one (too wide?)
//            bool p1_Theta_p_cut = (fabs(Theta_p1 - 40.) < 7.5); // Lead proton theta_p cut
//            bool p2_Theta_p_cut = (fabs(Theta_p2 - 40.) < 7.5); // Recoil proton theta_p cut
//            bool Lead_and_Recoil_with_close_phi = (fabs(Phi_p1 - Phi_p2) < 20.);

//            bool p1_Theta_p_cut = (fabs(Theta_p1 - 40.) < 10.);
//            bool p2_Theta_p_cut = (fabs(Theta_p2 - 40.) < 10.);
//            bool Lead_and_Recoil_with_close_phi = (fabs(Phi_p1 - Phi_p2) < 10.);

//            bool single_edge_detection = !(p1_Theta_p_cut && p2_Theta_p_cut);
            bool single_edge_detection = !(p1_Theta_p_cut && p2_Theta_p_cut && Lead_and_Recoil_with_close_phi);
            //</editor-fold>

            //<editor-fold desc="Identical CTOF position cut">
            TVector3 p1_hit_pos, p2_hit_pos, pos_diff; // hit position in the CTOF, and position difference

            double time_diff;

            bool true_2p_event = true;

            if (single_edge_detection) {
                if ((protons[Protons_ind.at(0)]->getRegion() == CD) && (protons[Protons_ind.at(1)]->getRegion() == CD)) { // if both 2p protons are in the CD
                    p1_hit_pos.SetXYZ(protons[Protons_ind.at(0)]->sci(clas12::CTOF)->getX(), protons[Protons_ind.at(0)]->sci(clas12::CTOF)->getY(),
                                      protons[Protons_ind.at(0)]->sci(clas12::CTOF)->getZ());
                    p2_hit_pos.SetXYZ(protons[Protons_ind.at(1)]->sci(clas12::CTOF)->getX(), protons[Protons_ind.at(1)]->sci(clas12::CTOF)->getY(),
                                      protons[Protons_ind.at(1)]->sci(clas12::CTOF)->getZ());
                    pos_diff.SetXYZ(p1_hit_pos.Px() - p2_hit_pos.Px(), p1_hit_pos.Py() - p2_hit_pos.Py(), p1_hit_pos.Pz() - p2_hit_pos.Pz());

                    time_diff = protons[lead_p_ind]->getTime() - protons[recoil_p_ind]->getTime();

                    hTheta_p1_p2_VS_ToF1_ToF2_BC_2p.hFill(Theta_p1_p2_2p, time_diff, Weight);
                    hTheta_p1_p2_VS_Pos1_Pos2_BC_2p.hFill(Theta_p1_p2_2p, pos_diff.Mag(), Weight);

                    true_2p_event = (pos_diff.Mag() != 0);
                }
            }
            //</editor-fold>

            //</editor-fold>

            //  Fillings 2p histograms ---------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Fillings 2p histograms">
            if (true_2p_event && single_edge_detection) { // with identical CTOF position cut & single edge detection cut
                ++num_of_events_2p;

                //<editor-fold desc="Filling cut parameters histograms (2p)">
                /* Filling Chi2 histograms (2p) */

                // Electrton Chi2 (2p):
                if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                    hChi2_Electron_2p_FD.hFill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight);
//                hChi2_Electron_2p_FD->Fill(electrons[Electron_ind.at(0)]->par()->getChi2Pid());
                }

                // Proton0 Chi2 (2p):
                if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                    hChi2_Proton_2p_CD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
//                hChi2_Proton_2p_CD->Fill(protons[Protons_ind.at(0)]->par()->getChi2Pid());
                } else if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                    hChi2_Proton_2p_FD.hFill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
//                hChi2_Proton_2p_FD->Fill(protons[Protons_ind.at(0)]->par()->getChi2Pid());
                }

                // Proton1 Chi2 (2p):
                if (protons[Protons_ind.at(1)]->getRegion() == CD) {
                    hChi2_Proton_2p_CD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
//                hChi2_Proton_2p_CD->Fill(protons[Protons_ind.at(1)]->par()->getChi2Pid());
                } else if (protons[Protons_ind.at(1)]->getRegion() == FD) {
                    hChi2_Proton_2p_FD.hFill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
//                hChi2_Proton_2p_FD->Fill(protons[Protons_ind.at(1)]->par()->getChi2Pid());
                }

                /* Filling dVx, dVy, dVz (2p) */
                for (auto &p: protons) {
                    Vx_p_1e2p = p->par()->getVx(), Vy_p_1e2p = p->par()->getVy(), Vz_p_1e2p = p->par()->getVz();
                    dVx = Vx_p_1e2p - Vx_e, dVy = Vy_p_1e2p - Vy_e, dVz = Vz_p_1e2p - Vz_e;

                    hdVx_2p.hFill(dVx, Weight), hdVy_2p.hFill(dVy, Weight), hdVz_2p.hFill(dVz, Weight);
//                hdVx_2p->Fill(dVx), hdVy_2p->Fill(dVy), hdVz_2p->Fill(dVz);
                } // end of loop over protons vector

                /* Filling SF histograms (2p) */
                hSF_2p_FD.hFill(EoP_e, Weight), hSF_VS_P_e_2p_FD.hFill(P_e, EoP_e, Weight);
//            hSF_2p_FD->Fill(EoP_e), hSF_VS_P_e_2p_FD->Fill(P_e, EoP_e);

                /* Filling fiducial plots (2p) */
                hVcal_VS_EoP_2p_PCAL.hFill(electrons[Electron_ind.at(0)]->cal(PCAL)->getLv(), EoP_e, Weight);
//            hVcal_VS_EoP_2p_PCAL->Fill(electrons[Electron_ind.at(0)]->cal(PCAL)->getLv(), EoP_e);
                hWcal_VS_EoP_2p_PCAL.hFill(electrons[Electron_ind.at(0)]->cal(PCAL)->getLw(), EoP_e, Weight);
//            hWcal_VS_EoP_2p_PCAL->Fill(electrons[Electron_ind.at(0)]->cal(PCAL)->getLw(), EoP_e);

                /* Filling Nphe plots (2p) */
                hNphe_2p_FD.hFill(Nphe, Weight);
//            hNphe_2p_FD->Fill(Nphe);

                /* Filling momentum histograms */
                // Electrton momentum (2p):
                if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                    hP_e_2p_FD.hFill(P_e, Weight);
//                hP_e_2p_FD->Fill(P_e);
                }

                for (int i = 0; i < Ne; i++) {
                    if (electrons[i]->getRegion() == FD) { hP_e_BC_2p_FD.hFill(P_e, Weight); } // before mom cuts
                }

                // Proton0 momentum (2p):
                if (protons[Protons_ind.at(0)]->getRegion() == CD) {
                    hP_p_2p_CD.hFill(P_p_first_2p_3v.Mag(), Weight);
                } else if (protons[Protons_ind.at(0)]->getRegion() == FD) {
                    hP_p_2p_FD.hFill(P_p_first_2p_3v.Mag(), Weight);
                }

                // Proton1 momentum (2p):
                if (protons[Protons_ind.at(1)]->getRegion() == CD) {
                    hP_p_2p_CD.hFill(P_p_second_2p_3v.Mag(), Weight);
                } else if (protons[Protons_ind.at(1)]->getRegion() == FD) {
                    hP_p_2p_FD.hFill(P_p_second_2p_3v.Mag(), Weight);
                }

                for (int i = 0; i < Nn; i++) {
                    if (neutrons[i]->getRegion() == CD) {
                        hP_n_BC_2p_CD.hFill(neutrons[i]->getP(), Weight); // before mom cuts
                    } else if (neutrons[i]->getRegion() == FD) {
                        hP_n_BC_2p_FD.hFill(neutrons[i]->getP(), Weight); // before mom cuts
                    }
                }

                for (int i = 0; i < Np; i++) {
                    if (protons[i]->getRegion() == CD) {
                        hP_p_BC_2p_CD.hFill(protons[i]->getP(), Weight); // before mom cuts
                    } else if (protons[i]->getRegion() == FD) {
                        hP_p_BC_2p_FD.hFill(protons[i]->getP(), Weight); // before mom cuts
                    }
                }

                for (int i = 0; i < Npip; i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hP_pip_BC_2p_CD.hFill(piplus[i]->getP(), Weight); // before mom cuts
                    } else if (piplus[i]->getRegion() == FD) {
                        hP_pip_BC_2p_FD.hFill(piplus[i]->getP(), Weight); // before mom cuts
                    }
                }

                for (int i = 0; i < Npim; i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hP_pim_BC_2p_CD.hFill(piminus[i]->getP(), Weight); // before mom cuts
                    } else if (piminus[i]->getRegion() == FD) {
                        hP_piminus_BPID_1n_FD.hFill(piminus[i]->getP(), Weight); // before mom cuts
                    }
                }
                //</editor-fold>

                //<editor-fold desc="Filling Beta vs. P plots (2p)">

                //<editor-fold desc="Beta vs. P from electrons (2p, CD & FD)">
                if (electrons[Electron_ind.at(0)]->getRegion() == FD) {
                    hBeta_vs_P_2p_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                    hBeta_vs_P_2p_Electrons_Only_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);

                    if (electrons[Electron_ind.at(0)]->par()->getCharge() == 1) {
                        hBeta_vs_P_positive_part_2p_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                    } else if (electrons[Electron_ind.at(0)]->par()->getCharge() == 0) {
                        hBeta_vs_P_neutral_part_2p_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                    } else if (electrons[Electron_ind.at(0)]->par()->getCharge() == -1) {
                        hBeta_vs_P_negative_part_2p_FD.hFill(electrons[Electron_ind.at(0)]->getP(), electrons[Electron_ind.at(0)]->par()->getBeta(), Weight);
                    }
                }
                //</editor-fold>

                //<editor-fold desc="Beta vs. P from protons (2p, CD & FD)">
                for (int i = 0; i < Np; i++) {
                    if (protons[i]->getRegion() == CD) {
                        hBeta_vs_P_2p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_2p_Protons_Only_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_positive_part_2p_CD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                    } else if (protons[i]->getRegion() == FD) {
                        hBeta_vs_P_2p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_2p_Protons_Only_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_positive_part_2p_FD.hFill(protons[i]->getP(), protons[i]->par()->getBeta(), Weight);
                    }
                } // end of loop over protons vector
                //</editor-fold>

                //<editor-fold desc="Beta vs. P from other particles (2p, CD & FD)">
                /* This is for self-consistency. Contributions from other particles should be zero */

//        //<editor-fold desc="Beta vs. P from neutrons (2p, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                hBeta_vs_P_2p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_2p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_2p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_2p_CD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                hBeta_vs_P_2p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    hBeta_vs_P_positive_part_2p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    hBeta_vs_P_neutral_part_2p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    hBeta_vs_P_negative_part_2p_FD.hFill(neutrons[i]->getP(), neutrons[i]->par()->getBeta(),Weight);
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

                //<editor-fold desc="Beta vs. P from Kplus (2p, CD & FD)">
                for (int i = 0; i < Kplus.size(); i++) {
                    if (Kplus[i]->getRegion() == CD) {
                        hBeta_vs_P_2p_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_positive_part_2p_CD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                    } else if (Kplus[i]->getRegion() == FD) {
                        hBeta_vs_P_2p_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_positive_part_2p_FD.hFill(Kplus[i]->getP(), Kplus[i]->par()->getBeta(), Weight);
                    }
                } // end of loop over Kplus vector
                //</editor-fold>

                //<editor-fold desc="Beta vs. P from Kminus (2p, CD & FD)">
                for (int i = 0; i < Kminus.size(); i++) {
                    if (Kminus[i]->getRegion() == CD) {
                        hBeta_vs_P_2p_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_negative_part_2p_CD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                    } else if (Kminus[i]->getRegion() == FD) {
                        hBeta_vs_P_2p_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_negative_part_2p_FD.hFill(Kminus[i]->getP(), Kminus[i]->par()->getBeta(), Weight);
                    }
                } // end of loop over Kminus vector
                //</editor-fold>

                //<editor-fold desc="Beta vs. P from piplus (2p, CD & FD)">
                for (int i = 0; i < piplus.size(); i++) {
                    if (piplus[i]->getRegion() == CD) {
                        hBeta_vs_P_2p_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_positive_part_2p_CD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                    } else if (piplus[i]->getRegion() == FD) {
                        hBeta_vs_P_2p_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_positive_part_2p_FD.hFill(piplus[i]->getP(), piplus[i]->par()->getBeta(), Weight);
                    }
                } // end of loop over piplus vector
                //</editor-fold>

                //<editor-fold desc="Beta vs. P from piminus (2p, CD & FD)">
                for (int i = 0; i < piminus.size(); i++) {
                    if (piminus[i]->getRegion() == CD) {
                        hBeta_vs_P_2p_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_negative_part_2p_CD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                    } else if (piminus[i]->getRegion() == FD) {
                        hBeta_vs_P_2p_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                        hBeta_vs_P_negative_part_2p_FD.hFill(piminus[i]->getP(), piminus[i]->par()->getBeta(), Weight);
                    }
                } // end of loop over piminus vector
                //</editor-fold>

                //</editor-fold>

                //</editor-fold>

                for (auto &e: electrons) {
                    if (e->getRegion() == FD) {
                        hTheta_e_All_Int_2p_FD->Fill(Theta_e, Weight);
                        hPhi_e_All_Int_2p_FD->Fill(Phi_e, Weight);
                        hTheta_e_VS_Phi_e_2p_FD->Fill(Phi_e, Theta_e, Weight);

                        hE_e_All_Int_2p_FD->Fill(E_e, Weight);
                        hE_e_VS_Theta_e_All_Int_2p_FD->Fill(Theta_e, E_e, Weight);

                        hET_All_Ang_All_Int_2p_FD->Fill(beamE - E_e, Weight);
                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                            hET15_All_Int_2p_FD->Fill(beamE - E_e, Weight);
                            hE_e_15_All_Int_2p_FD->Fill(E_e, Weight);
                        }

                        if (qel) {
                            hTheta_e_QEL_2p_FD->Fill(Theta_e, Weight);
                            hPhi_e_QEL_2p_FD->Fill(Phi_e, Weight);
                            hE_e_QEL_2p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_QEL_2p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_QEL_2p_FD->Fill(beamE - E_e, Weight);
                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_QEL_2p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_QEL_2p_FD->Fill(E_e, Weight);
                            }
                        } else if (mec) {
                            hTheta_e_MEC_2p_FD->Fill(Theta_e, Weight);
                            hPhi_e_MEC_2p_FD->Fill(Phi_e, Weight);
                            hE_e_MEC_2p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_MEC_2p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_MEC_2p_FD->Fill(beamE - E_e, Weight);
                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_MEC_2p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_MEC_2p_FD->Fill(E_e, Weight);
                            }
                        } else if (res) {
                            hTheta_e_RES_2p_FD->Fill(Theta_e, Weight);
                            hPhi_e_RES_2p_FD->Fill(Phi_e, Weight);
                            hE_e_RES_2p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_RES_2p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_RES_2p_FD->Fill(beamE - E_e, Weight);
                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_RES_2p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_RES_2p_FD->Fill(E_e, Weight);
                            }
                        } else if (dis) {
                            hTheta_e_DIS_2p_FD->Fill(Theta_e, Weight);
                            hPhi_e_DIS_2p_FD->Fill(Phi_e, Weight);
                            hE_e_DIS_2p_FD->Fill(E_e, Weight);
                            hE_e_VS_Theta_e_DIS_2p_FD->Fill(Theta_e, E_e, Weight);

                            hET_All_Ang_DIS_2p_FD->Fill(beamE - E_e, Weight);
                            if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) {
                                hET15_DIS_2p_FD->Fill(beamE - E_e, Weight);
                                hE_e_15_DIS_2p_FD->Fill(E_e, Weight);
                            }
                        }
                    }
                } // end of loop over electrons vector

                /* Filling Q2 histograms (2p) */
                if (electrons[Electron_ind.at(0)]->getRegion() == FD) { hQ2_2p_FD->Fill(Q2, Weight); }

                hP_p_1_2p.hFill(P_1_2p_3v.Mag(), Weight); // Leading proton (2p)
                hP_p_2_2p.hFill(P_2_2p_3v.Mag(), Weight); // Recoil proton (2p)
                hP_p_1_vs_P_p_2_2p.hFill(P_1_2p_3v.Mag(), P_2_2p_3v.Mag(), Weight);

                P_tot_2p_3v = TVector3(P_p_first_2p_3v.Px() + P_p_second_2p_3v.Px(), P_p_first_2p_3v.Py() + P_p_second_2p_3v.Py(),
                                       P_p_first_2p_3v.Pz() + P_p_second_2p_3v.Pz());                                                    // P_tot = P_1 + P_2
                P_T_L_2p_3v = TVector3(P_1_2p_3v.Px(), P_1_2p_3v.Py(), 0);                                                               // transverse part of P_1
                P_T_tot_2p_3v = TVector3(P_p_first_2p_3v.Px() + P_p_second_2p_3v.Px(), P_p_first_2p_3v.Py() + P_p_second_2p_3v.Py(), 0); // transverse part of P_tot
                dP_T_L_2p_3v = TVector3(P_e_2p_3v.Px() + P_T_L_2p_3v.Px(), P_e_2p_3v.Py() + P_T_L_2p_3v.Py(), 0);
                dP_T_tot_2p_3v = TVector3(P_e_2p_3v.Px() + P_1_2p_3v.Px() + P_2_2p_3v.Px(), P_e_2p_3v.Py() + P_1_2p_3v.Py() + P_2_2p_3v.Py(), 0);

                Theta_p_e_p_tot_2p = acos((P_e_2p_3v.Px() * P_tot_2p_3v.Px() + P_e_2p_3v.Py() * P_tot_2p_3v.Py() + P_e_2p_3v.Pz() * P_tot_2p_3v.Pz())
                                          / (P_e_2p_3v.Mag() * P_tot_2p_3v.Mag())) * 180.0 / pi; // Theta_p_e_p_tot_2p in deg
                hTheta_p_e_p_tot_2p->Fill(Theta_p_e_p_tot_2p, Weight);

                hTheta_p1_p2_All_Int_2p->Fill(Theta_p1_p2_2p, Weight);

                if (qel) {
                    hTheta_p1_p2_QEL_2p->Fill(Theta_p1_p2_2p, Weight);
                } else if (mec) {
                    hTheta_p1_p2_MEC_2p->Fill(Theta_p1_p2_2p, Weight);
                } else if (res) {
                    hTheta_p1_p2_RES_2p->Fill(Theta_p1_p2_2p, Weight);
                } else if (dis) {
                    hTheta_p1_p2_DIS_2p->Fill(Theta_p1_p2_2p, Weight);
                }

                hTheta_p1_p2_vs_W_2p->Fill(W_2p, Theta_p1_p2_2p, Weight);

                if (Theta_p1_p2_2p < 10) { hTheta_p1_vs_theta_p2_for_Theta_p1_p2_10_2p->Fill(Theta_p2, Theta_p1, Weight); }

                Theta_q_p_tot_2p = acos((q_2p_3v.Px() * P_tot_2p_3v.Px() + q_2p_3v.Py() * P_tot_2p_3v.Py() + q_2p_3v.Pz() * P_tot_2p_3v.Pz())
                                        / (q_2p_3v.Mag() * P_tot_2p_3v.Mag())) * 180.0 / pi; // Theta_q_p_tot_2p in deg
                hTheta_q_p_tot_2p->Fill(Theta_q_p_tot_2p, Weight);

                Theta_q_p_L_2p = acos((q_2p_3v.Px() * P_1_2p_3v.Px() + q_2p_3v.Py() * P_1_2p_3v.Py() + q_2p_3v.Pz() * P_1_2p_3v.Pz())
                                      / (q_2p_3v.Mag() * P_1_2p_3v.Mag())) * 180.0 / pi; // Theta_q_p_L_2p = Theta_q_p_1_2p in deg
                Theta_q_p_R_2p = acos((q_2p_3v.Px() * P_2_2p_3v.Px() + q_2p_3v.Py() * P_2_2p_3v.Py() + q_2p_3v.Pz() * P_2_2p_3v.Pz())
                                      / (q_2p_3v.Mag() * P_2_2p_3v.Mag())) * 180.0 / pi; // Theta_q_p_R_2p = Theta_q_p_2_2p in deg
                hTheta_q_p_L_2p->Fill(Theta_q_p_L_2p, Weight);
                hTheta_q_p_R_2p->Fill(Theta_q_p_R_2p, Weight);
                hTheta_q_p_L_vs_p_L_q_2p->Fill(P_1_2p_3v.Mag() / q_2p_3v.Mag(), Theta_q_p_L_2p, Weight);

                E_1_2p = sqrt(m_p * m_p + P_1_2p_3v.Mag2());
                E_2_2p = sqrt(m_p * m_p + P_2_2p_3v.Mag2());
                Ecal_2p = E_e_2p + (E_1_2p - m_p) + (E_2_2p - m_p);

                //<editor-fold desc="Filling Ecal plots">
                if (Ecal_2p > beamE) {
                    hChi2_Electron_Ecal_test_2p->Fill(electrons[Electron_ind.at(0)]->par()->getChi2Pid(), Weight);
                    hChi2_Proton_Ecal_test_2p->Fill(protons[Protons_ind.at(0)]->par()->getChi2Pid(), Weight);
                    hChi2_Proton_Ecal_test_2p->Fill(protons[Protons_ind.at(1)]->par()->getChi2Pid(), Weight);
                }

                hEcal_All_Int_2p->Fill(Ecal_2p, Weight); // Fill Ecal for all interactions

                if (qel) {
                    hEcal_QEL_2p->Fill(Ecal_2p, Weight); // Fill Ecal for QEL only
                } else if (mec) {
                    hEcal_MEC_2p->Fill(Ecal_2p, Weight); // Fill Ecal for MEC only
                } else if (res) {
                    hEcal_RES_2p->Fill(Ecal_2p, Weight); // Fill Ecal for RES only
                } else if (dis) {
                    hEcal_DIS_2p->Fill(Ecal_2p, Weight); // Fill Ecal for DIS only
                }
                //</editor-fold>

                hdP_T_L_2p->Fill(dP_T_L_2p_3v.Mag(), Weight);
                hdP_T_tot_2p->Fill(dP_T_tot_2p_3v.Mag(), Weight);

                dAlpha_T_L_2p = acos(-(P_e_2p_3v.Px() * dP_T_L_2p_3v.Px() + P_e_2p_3v.Py() * dP_T_L_2p_3v.Py() + P_e_2p_3v.Pz() * dP_T_L_2p_3v.Pz())
                                     / (P_T_e_2p_3v.Mag() * dP_T_L_2p_3v.Mag())) * 180.0 / pi; // dP_T_L_2p_3v.Pz() = 0; dAlpha_T_L_2p in deg
                dAlpha_T_tot_2p = acos(-(P_e_2p_3v.Px() * dP_T_tot_2p_3v.Px() + P_e_2p_3v.Py() * dP_T_tot_2p_3v.Py() + P_e_2p_3v.Pz() * dP_T_tot_2p_3v.Pz())
                                       / (P_T_e_2p_3v.Mag() * dP_T_tot_2p_3v.Mag())) * 180.0 / pi; // dP_T_tot_2p_3v.Pz() = 0; dAlpha_T_tot_2p in deg
                hdAlpha_T_L_2p->Fill(dAlpha_T_L_2p, Weight);
                hdAlpha_T_tot_2p->Fill(dAlpha_T_tot_2p, Weight);

                hdP_T_L_vs_dAlpha_T_L_2p->Fill(dAlpha_T_L_2p, dP_T_L_2p_3v.Mag(), Weight);
                hdP_T_tot_vs_dAlpha_T_tot_2p->Fill(dAlpha_T_tot_2p, dP_T_tot_2p_3v.Mag(), Weight);

                dPhi_T_L_2p = acos(-(P_T_e_2p_3v.Px() * P_T_L_2p_3v.Px() + P_T_e_2p_3v.Py() * P_T_L_2p_3v.Py() + P_T_e_2p_3v.Pz() * P_T_L_2p_3v.Pz())
                                   / (P_T_e_2p_3v.Mag() * P_T_L_2p_3v.Mag())) * 180.0 / pi; // P_T_L_2p_3v.Pz() = 0; dPhi_T_L_2p in deg
                dPhi_T_tot_2p = acos(-(P_T_e_2p_3v.Px() * P_T_tot_2p_3v.Px() + P_T_e_2p_3v.Py() * P_T_tot_2p_3v.Py() + P_T_e_2p_3v.Pz() * P_T_tot_2p_3v.Pz())
                                     / (P_T_e_2p_3v.Mag() * P_T_tot_2p_3v.Mag())) * 180.0 / pi; // P_T_tot_2p_3v.Pz() = 0; dPhi_T_tot_2p in deg
                hdPhi_T_L_2p->Fill(dPhi_T_L_2p, Weight);
                hdPhi_T_tot_2p->Fill(dPhi_T_tot_2p, Weight);

                hEcal_vs_dAlpha_T_L_2p->Fill(dAlpha_T_L_2p, Ecal_2p, Weight);
                hEcal_vs_dAlpha_T_tot_2p->Fill(dAlpha_T_tot_2p, Ecal_2p, Weight);
                hEcal_vs_dP_T_L_2p->Fill(dP_T_L_2p_3v.Mag(), Ecal_2p, Weight);
                hEcal_vs_dP_T_tot_2p->Fill(dP_T_tot_2p_3v.Mag(), Ecal_2p, Weight);

                if ((protons[Protons_ind.at(0)]->getRegion() == CD) && (protons[Protons_ind.at(1)]->getRegion() == CD)) { // if both 2p protons are in the CD
                    hTheta_p1_p2_VS_ToF1_ToF2_AC_2p.hFill(Theta_p1_p2_2p, time_diff, Weight);
                    hTheta_p1_p2_VS_Pos1_Pos2_AC_2p.hFill(Theta_p1_p2_2p, pos_diff.Mag(), Weight);
                }
            } // end of if true 2p event
            //</editor-fold>

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
    TCanvas *c1 = new TCanvas("canvas", "canvas", 1000, 750); // normal res
//    TCanvas *c1 = new TCanvas("canvas", "canvas", 2000, 1500); // high res
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

    float DefStatX = gStyle->GetStatX(), DefStatY = gStyle->GetStatY();

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

//  Nphe plots ----------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Nphe plots (1e cut, FD)">
        if (apply_cuts == false) {
            hNphe_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
            hNphe_1e_cut_AC_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
        } else {
            hNphe_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
        }
        //</editor-fold>

        //<editor-fold desc="Nphe plots (1p, FD)">
        hNphe_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
        //</editor-fold>

        //<editor-fold desc="Nphe plots (1n, FD)">
        hNphe_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
        //</editor-fold>

        //<editor-fold desc="Nphe plots (2p, FD)">
        hNphe_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Nphe_plots, true, 1., clasAna.getNpheCuts(), 9999, 0, false);
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
        Chi2_Electron_cuts_FD.MeanFromHistogram = hChi2_Electron_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Electron_1e_cut_FD.GetHistogram1D().GetMaximumBin());
        Chi2_Proton_cuts_CD.MeanFromHistogram = hChi2_Proton_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_Proton_1e_cut_CD.GetHistogram1D().GetMaximumBin());
        Chi2_Proton_cuts_FD.MeanFromHistogram = hChi2_Proton_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Proton_1e_cut_FD.GetHistogram1D().GetMaximumBin());
        Chi2_Kplus_cuts_CD.MeanFromHistogram = hChi2_Kplus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_Kplus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
        Chi2_Kplus_cuts_FD.MeanFromHistogram = hChi2_Kplus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Kplus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
        Chi2_Kminus_cuts_CD.MeanFromHistogram = hChi2_Kminus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_Kminus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
        Chi2_Kminus_cuts_FD.MeanFromHistogram = hChi2_Kminus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_Kminus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
        Chi2_piplus_cuts_CD.MeanFromHistogram = hChi2_piplus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_piplus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
        Chi2_piplus_cuts_FD.MeanFromHistogram = hChi2_piplus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_piplus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
        Chi2_piminus_cuts_CD.MeanFromHistogram = hChi2_piminus_1e_cut_CD.GetHistogram1D().GetBinCenter(hChi2_piminus_1e_cut_CD.GetHistogram1D().GetMaximumBin());
        Chi2_piminus_cuts_FD.MeanFromHistogram = hChi2_piminus_1e_cut_FD.GetHistogram1D().GetBinCenter(hChi2_piminus_1e_cut_FD.GetHistogram1D().GetMaximumBin());
        //</editor-fold>

//  Chi2 plots ----------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Chi2 plots (no #(e) cut)">
        hChi2_Electron_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        hChi2_Proton_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                     Chi2_Proton_cuts_CD.Cuts.at(0), false);
        hChi2_Proton_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                     Chi2_Proton_cuts_FD.Cuts.at(0), false);
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (1e cut)">
        hChi2_Electron_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);

        if (apply_cuts == false && apply_chi2_cuts_1e_cut == false) {
            /* No cuts are applied. Plot without cut limits or fit */
            hChi2_Proton_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_Proton_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            hChi2_Proton_1e_cut_CD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_Proton_cuts_CD.FitStdFactor, Chi2_Proton_cuts_CD.Cuts.at(1),
                                                    Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0), true);
            hChi2_Proton_1e_cut_FD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_Proton_cuts_FD.FitStdFactor, Chi2_Proton_cuts_FD.Cuts.at(1),
                                                    Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(0), true);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == true) {
            hChi2_Proton_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                                Chi2_Proton_cuts_CD.Cuts.at(0), true);
            hChi2_Proton_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                                Chi2_Proton_cuts_FD.Cuts.at(0), true);
        }

        if (apply_cuts == false && apply_chi2_cuts_1e_cut == false) {
            /* No cuts are applied. Plot without cut limits or fit */
            hChi2_Kplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_Kplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            hChi2_Kplus_1e_cut_CD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_Kplus_cuts_CD.FitStdFactor, Chi2_Kplus_cuts_CD.Cuts.at(1),
                                                   Chi2_Kplus_cuts_CD.Cuts.at(2), Chi2_Kplus_cuts_CD.Cuts.at(0), true);
            hChi2_Kplus_1e_cut_FD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_Kplus_cuts_FD.FitStdFactor, Chi2_Kplus_cuts_FD.Cuts.at(1),
                                                   Chi2_Kplus_cuts_FD.Cuts.at(2), Chi2_Kplus_cuts_FD.Cuts.at(0), true);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == true) {
            hChi2_Kplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Kplus_cuts_CD.Cuts.at(2), Chi2_Kplus_cuts_CD.Cuts.at(2),
                                               Chi2_Kplus_cuts_CD.Cuts.at(0), true);
            hChi2_Kplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Kplus_cuts_FD.Cuts.at(2), Chi2_Kplus_cuts_FD.Cuts.at(2),
                                               Chi2_Kplus_cuts_FD.Cuts.at(0), true);
        }

        if (apply_cuts == false && apply_chi2_cuts_1e_cut == false) {
            /* No cuts are applied. Plot without cut limits or fit */
            hChi2_Kminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_Kminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            hChi2_Kminus_1e_cut_CD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_Kminus_cuts_CD.FitStdFactor, Chi2_Kminus_cuts_CD.Cuts.at(1),
                                                    Chi2_Kminus_cuts_CD.Cuts.at(2), Chi2_Kminus_cuts_CD.Cuts.at(0), true);
            hChi2_Kminus_1e_cut_FD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_Kminus_cuts_FD.FitStdFactor, Chi2_Kminus_cuts_FD.Cuts.at(1),
                                                    Chi2_Kminus_cuts_FD.Cuts.at(2), Chi2_Kminus_cuts_FD.Cuts.at(0), true);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == true) {
            hChi2_Kminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Kminus_cuts_CD.Cuts.at(2), Chi2_Kminus_cuts_CD.Cuts.at(2),
                                                Chi2_Kminus_cuts_CD.Cuts.at(0), true);
            hChi2_Kminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Kminus_cuts_FD.Cuts.at(2), Chi2_Kminus_cuts_FD.Cuts.at(2),
                                                Chi2_Kminus_cuts_FD.Cuts.at(0), true);
        }

        if (apply_cuts == false && apply_chi2_cuts_1e_cut == false) {
            /* No cuts are applied. Plot without cut limits or fit */
            hChi2_piplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_piplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            hChi2_piplus_1e_cut_CD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_piplus_cuts_CD.FitStdFactor, Chi2_piplus_cuts_CD.Cuts.at(1),
                                                    Chi2_piplus_cuts_CD.Cuts.at(2), Chi2_piplus_cuts_CD.Cuts.at(0), true);
            hChi2_piplus_1e_cut_FD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_piplus_cuts_FD.FitStdFactor, Chi2_piplus_cuts_FD.Cuts.at(1),
                                                    Chi2_piplus_cuts_FD.Cuts.at(2), Chi2_piplus_cuts_FD.Cuts.at(0), true);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == true) {
            hChi2_piplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_piplus_cuts_CD.Cuts.at(2), Chi2_piplus_cuts_CD.Cuts.at(2),
                                                Chi2_piplus_cuts_CD.Cuts.at(0), true);
            hChi2_piplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_piplus_cuts_FD.Cuts.at(2), Chi2_piplus_cuts_FD.Cuts.at(2),
                                                Chi2_piplus_cuts_FD.Cuts.at(0), true);
        }

        if (apply_cuts == false && apply_chi2_cuts_1e_cut == false) {
            /* No cuts are applied. Plot without cut limits or fit */
            hChi2_piminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_piminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == false) {
            /* Do Gaussian fit if not applying chi2 cuts */
            hChi2_piminus_1e_cut_CD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_piminus_cuts_CD.FitStdFactor, Chi2_piminus_cuts_CD.Cuts.at(1),
                                                     Chi2_piminus_cuts_CD.Cuts.at(2), Chi2_piminus_cuts_CD.Cuts.at(0), true);
            hChi2_piminus_1e_cut_FD.hDrawAndSaveWFit(SampleName, c1, plots, norm_Chi2_plots, true, 1., Chi2_piminus_cuts_FD.FitStdFactor, Chi2_piminus_cuts_FD.Cuts.at(1),
                                                     Chi2_piminus_cuts_FD.Cuts.at(2), Chi2_piminus_cuts_FD.Cuts.at(0), true);
        } else if (apply_cuts == true && apply_chi2_cuts_1e_cut == true) {
            hChi2_piminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_piminus_cuts_CD.Cuts.at(2), Chi2_piminus_cuts_CD.Cuts.at(2),
                                                 Chi2_piminus_cuts_CD.Cuts.at(0), true);
            hChi2_piminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_piminus_cuts_FD.Cuts.at(2), Chi2_piminus_cuts_FD.Cuts.at(2),
                                                 Chi2_piminus_cuts_FD.Cuts.at(0), true);
        }
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (1p)">
        hChi2_Electron_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        hChi2_Proton_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                        Chi2_Proton_cuts_CD.Cuts.at(0), false);
        hChi2_Proton_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                        Chi2_Proton_cuts_FD.Cuts.at(0), false);
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (1n)">
        hChi2_Electron_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
        hChi2_Proton_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                        Chi2_Proton_cuts_CD.Cuts.at(0), false);
        hChi2_Proton_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                        Chi2_Proton_cuts_FD.Cuts.at(0), false);
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (1e2p)">
        if (apply_cuts == false) {
            hChi2_Electron_1e2p_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_Proton_1e2p_BC_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                                 Chi2_Proton_cuts_CD.Cuts.at(0), true);
            hChi2_Proton_1e2p_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                                 Chi2_Proton_cuts_FD.Cuts.at(0), true);

            hChi2_Electron_1e2p_AC_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Electron_cuts_FD.Cuts.at(2), Chi2_Electron_cuts_FD.Cuts.at(2),
                                                   Chi2_Electron_cuts_FD.Cuts.at(0), true);
            hChi2_Proton_1e2p_AC_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                                 Chi2_Proton_cuts_CD.Cuts.at(0), true);
            hChi2_Proton_1e2p_AC_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                                 Chi2_Proton_cuts_FD.Cuts.at(0), true);
        } else {
            hChi2_Electron_1e2p_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);
            hChi2_Proton_1e2p_BC_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                                 Chi2_Proton_cuts_CD.Cuts.at(0), true);
            hChi2_Proton_1e2p_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                                 Chi2_Proton_cuts_FD.Cuts.at(0), true);
        }
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (2p)">
        hChi2_Electron_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., 9999, 9999, 0, false);

        hChi2_Proton_2p_CD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(2),
                                        Chi2_Proton_cuts_CD.Cuts.at(0), true);
        hChi2_Proton_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Chi2_plots, true, 1., -Chi2_Proton_cuts_FD.Cuts.at(2), Chi2_Proton_cuts_FD.Cuts.at(2),
                                        Chi2_Proton_cuts_FD.Cuts.at(0), true);
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

//  Vertex plots --------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Vertex plots (1e cut, CD & FD)">
        hVx_Electron_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Electron_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVz_Electron_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);

        hVx_Proton_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVx_Proton_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Proton_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Proton_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVz_Proton_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);
        hVz_Proton_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);

        hVx_Kplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVx_Kplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Kplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Kplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVz_Kplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);
        hVz_Kplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);

        hVx_Kminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVx_Kminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Kminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_Kminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVz_Kminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);
        hVz_Kminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);

        hVx_piplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVx_piplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_piplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_piplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVz_piplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);
        hVz_piplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);

        hVx_piminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVx_piminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_piminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVy_piminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hVz_piminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);
        hVz_piminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., Vz_cut.GetLowerCut(), Vz_cut.GetUpperCut(), 0, false);
        //</editor-fold>

//  dV plots ------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="dV plots (1p, CD & FD)">
        hdVx_1p.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVy_1p.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVz_1p.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., -dVz_cuts.GetUpperCut(), dVz_cuts.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="dV plots (1n, CD & FD)">
        hdVx_1n.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVy_1n.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVz_1n.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., -dVz_cuts.GetUpperCut(), dVz_cuts.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="dV plots (1e2p, CD & FD)">
        hdVx_1e2p_BC.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVy_1e2p_BC.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVz_1e2p_BC.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., -dVz_cuts.GetUpperCut(), dVz_cuts.GetUpperCut(), 0, false);

        if (apply_cuts == false) {
            hdVx_1e2p_AC.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
            hdVy_1e2p_AC.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
            hdVz_1e2p_AC.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., -dVz_cuts.GetUpperCut(), dVz_cuts.GetUpperCut(), 0, false);
        }
        //</editor-fold>

        //<editor-fold desc="dV plots (2p, CD & FD)">
        hdVx_2p.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVy_2p.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., 9999, 9999, 0, false);
        hdVz_2p.hDrawAndSave(SampleName, c1, plots, norm_Vertex_plots, true, 1., -dVz_cuts.GetUpperCut(), dVz_cuts.GetUpperCut(), 0, false);
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

//  SF plots ------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="SF plots (1e cut, FD)">
        if (apply_cuts == false) {
            hSF_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_SF_plots, true, 1., clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut(), 0, false);
            hSF_1e_cut_AC_FD.hDrawAndSave(SampleName, c1, plots, norm_SF_plots, true, 1., clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut(), 0, false);
        } else {
            hSF_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_SF_plots, true, 1., clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut(), 0, false);
        }
        //</editor-fold>

        //<editor-fold desc="SF plots (1p, FD)">
        hSF_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_SF_plots, true, 1., clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="SF plots (1n, FD)">
        hSF_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_SF_plots, true, 1., clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="SF plots (2p, FD)">
        hSF_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_SF_plots, true, 1., clasAna.getEcalSFLowerCut(), clasAna.getEcalSFUpperCut(), 0, false);
        //</editor-fold>

//  SF vs. P plots ------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="SF vs. P plots (1e cut, FD)">
        if (apply_cuts == false) {
            hSF_VS_P_e_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, false);
            hSF_VS_P_e_1e_cut_AC_FD.hDrawAndSave(SampleName, c1, plots, false);
        } else {
            hSF_VS_P_e_1e_cut_BC_FD.hDrawAndSave(SampleName, c1, plots, false);
        }
        //</editor-fold>

        //<editor-fold desc="SF vs. P plots (1p, FD)">
        hSF_VS_P_e_1p_FD.hDrawAndSave(SampleName, c1, plots, false);
        //</editor-fold>

        //<editor-fold desc="SF vs. P plots (1n, FD)">
        hSF_VS_P_e_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        //</editor-fold>

        //<editor-fold desc="SF vs. P plots (2p, FD)">
        hSF_VS_P_e_2p_FD.hDrawAndSave(SampleName, c1, plots, false);
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

//  ECAL coordinates vs. SF plots ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="ECAL coordinates vs. SF plots (1e cut, FD only)">
        if (apply_cuts == false) {
            hVcal_VS_EoP_1e_cut_BC_PCAL.hDrawAndSave(SampleName, c1, plots, false);
            hWcal_VS_EoP_1e_cut_BC_PCAL.hDrawAndSave(SampleName, c1, plots, false);
            hVcal_VS_EoP_1e_cut_AC_PCAL.hDrawAndSave(SampleName, c1, plots, false);
            hWcal_VS_EoP_1e_cut_AC_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        } else {
            hVcal_VS_EoP_1e_cut_BC_PCAL.hDrawAndSave(SampleName, c1, plots, false);
            hWcal_VS_EoP_1e_cut_BC_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        }
        //</editor-fold>

        //<editor-fold desc="ECAL coordinates vs. SF plots (1p, FD only)">
        hVcal_VS_EoP_1p_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        hWcal_VS_EoP_1p_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        //</editor-fold>

        //<editor-fold desc="ECAL coordinates vs. SF plots (1n, FD only)">
        hVcal_VS_EoP_1n_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        hWcal_VS_EoP_1n_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        //</editor-fold>

        //<editor-fold desc="ECAL coordinates vs. SF plots (2p, FD only)">
        hVcal_VS_EoP_2p_PCAL.hDrawAndSave(SampleName, c1, plots, false);
        hWcal_VS_EoP_2p_PCAL.hDrawAndSave(SampleName, c1, plots, false);
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

//  Momentum plots ------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Momentum plots (1e cut, CD & FD)">
        hP_e_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        hP_p_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        hP_p_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        hP_piplus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        hP_piplus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        hP_piminus_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        hP_piminus_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., 9999, 9999, 0, false);
        //</editor-fold>

        //<editor-fold desc="Momentum plots (1p, CD & FD)">
        hP_e_APID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);
        hP_e_BPID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);

        hP_p_APID_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_BPID_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_APID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_BPID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);

        hP_piplus_APID_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_piplus_BPID_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_piplus_APID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_piplus_BPID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);

        hP_piminus_APID_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_piminus_BPID_1p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_piminus_APID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_piminus_BPID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);

        hP_ph_APID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut(), 0, false);
        hP_ph_BPID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut(), 0, false);

        hP_n_APID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_BPID_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="Momentum plots (1n, CD & FD)">
        hP_e_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);
        hP_e_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);

        hP_n_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_APID_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_BPID_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);

        hP_p_APID_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_BPID_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);

        hP_piplus_APID_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_piplus_BPID_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_piplus_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_piplus_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);

        hP_piminus_APID_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_piminus_BPID_1n_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_piminus_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_piminus_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);

        hP_n_VN_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_VN_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_Ph_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_Ph_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);

        hP_ph_APID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut(), 0, false);
        hP_ph_BPID_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., ph_mom_th.GetLowerCut(), ph_mom_th.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="Momentum plots (1e2p, CD & FD)">
        hP_e_1e2p_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);
        hP_e_1e2p_AC_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);
        hP_p_1e2p_BC_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_1e2p_AC_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_1e2p_BC_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_1e2p_AC_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="Momentum plots (2p, CD & FD)">
        hP_e_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);
        hP_p_2p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);

        hP_e_BC_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut(), 0, false);
        hP_p_BC_2p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_BC_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_pip_BC_2p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_pip_BC_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut(), 0, false);
        hP_pim_BC_2p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_pim_BC_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut(), 0, false);
        hP_n_BC_2p_CD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);
        hP_n_BC_2p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut(), 0, false);

        hP_p_1_2p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        hP_p_2_2p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="P1 vs P2 plots (2p, CD & FD)">
        hP_p_1_vs_P_p_2_2p.hDrawAndSave(SampleName, c1, plots, true);
        //</editor-fold>

    } else {
        cout << "\n\nMomentum (P_e) plots are disabled by user.\n\n";
    }
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Beta histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Beta histograms">

    //<editor-fold desc="Beta plots">
    if (Beta_plots) {
        cout << "\n\nPlotting Beta histograms...\n\n";

//  Beta plots ----------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Beta plots (1n)">

        //<editor-fold desc="Beta plots (1n)">
        hBeta_n_from_ph_01_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_01_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_02_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_02_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_03_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_03_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_04_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_n_from_ph_04_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);

        StackPlot3(SampleName, plots,
                   hBeta_n_from_ph_02_1n_FD, "!PCAL", hBeta_n_from_ph_03_1n_FD, "!PCAL && ECIN", hBeta_n_from_ph_04_1n_FD, "!PCAL && !ECIN && ECOUT");

        if (!apply_neutron_Beta_Fit) {
            BetaFit(SampleName, Beta_cut_ABF_FD_n_from_ph, n_momentum_cuts_ABF_FD_n_from_ph, hBeta_n_from_ph_01_1n_FD, plots);
            BetaFitApprax(SampleName, Beta_cut_ABF_FD_n_from_ph_apprax, n_momentum_cuts_ABF_FD_n_from_ph_apprax, hBeta_n_from_ph_01_1n_FD, plots);
        }

        hBeta_All_FD_Neut_01_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_01_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_02_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_02_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_03_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_03_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_04_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_All_FD_Neut_04_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);

        StackPlot3(SampleName, plots,
                   hBeta_All_FD_Neut_02_1n_FD, "!PCAL", hBeta_All_FD_Neut_03_1n_FD, "!PCAL && ECIN", hBeta_All_FD_Neut_04_1n_FD, "!PCAL && !ECIN && ECOUT");
        StackPlot4(SampleName, plots,
                   hBeta_All_FD_Neut_01_1n_FD, "All FD neutrals", hBeta_All_FD_Neut_02_1n_FD, "!PCAL",
                   hBeta_All_FD_Neut_03_1n_FD, "!PCAL && ECIN", hBeta_All_FD_Neut_04_1n_FD, "!PCAL && !ECIN && ECOUT");

        if (!apply_neutron_Beta_Fit) {
            BetaFit(SampleName, Beta_cut_ABF_All_FD_neutrals, n_momentum_cuts_ABF_FD_All_neutrals, hBeta_All_FD_Neut_01_1n_FD, plots);
            BetaFit(SampleName, Beta_cut_ABF_All_FD_neutrals_noPCAL, n_momentum_cuts_ABF_FD_noPCAL, hBeta_All_FD_Neut_02_1n_FD, plots);
            BetaFit(SampleName, Beta_cut_ABF_All_FD_neutrals_noPCAL_wECIN, n_momentum_cuts_ABF_FD_noPCAL_wECIN, hBeta_All_FD_Neut_03_1n_FD, plots);
            BetaFit(SampleName, Beta_cut_ABF_All_FD_neutrals_noPCAL_noECIN_wECOUT, n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT, hBeta_All_FD_Neut_04_1n_FD, plots);
        }

        hBeta_FD_Neut_noPDG0_01_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_01_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_02_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_02_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_03_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_03_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_04_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);
        hBeta_FD_Neut_noPDG0_04_1n_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, norm_Beta_plots, true, 1., 9999, 9999, 0, false);

        StackPlot3(SampleName, plots,
                   hBeta_FD_Neut_noPDG0_02_1n_FD, "!PCAL", hBeta_FD_Neut_noPDG0_03_1n_FD, "!PCAL && ECIN", hBeta_FD_Neut_noPDG0_04_1n_FD, "!PCAL && !ECIN && ECOUT");
        StackPlot4(SampleName, plots,
                   hBeta_FD_Neut_noPDG0_01_1n_FD, "FD neutrals (no PDG=0)", hBeta_FD_Neut_noPDG0_02_1n_FD, "!PCAL",
                   hBeta_FD_Neut_noPDG0_03_1n_FD, "!PCAL && ECIN", hBeta_FD_Neut_noPDG0_04_1n_FD, "!PCAL && !ECIN && ECOUT");

        if (!apply_neutron_Beta_Fit) {
            BetaFit(SampleName, Beta_cut_ABF_FD_neutrals_noPDG0, n_momentum_cuts_ABF_FD_All_neutrals_noPDG0, hBeta_FD_Neut_noPDG0_01_1n_FD, plots);
            BetaFit(SampleName, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL, n_momentum_cuts_ABF_FD_noPCAL_noPDG0, hBeta_FD_Neut_noPDG0_02_1n_FD, plots);
            BetaFit(SampleName, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_wECIN, n_momentum_cuts_ABF_FD_noPCAL_wECIN_noPDG0, hBeta_FD_Neut_noPDG0_03_1n_FD, plots);
            BetaFit(SampleName, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT, n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT_noPDG0,
                    hBeta_FD_Neut_noPDG0_04_1n_FD, plots);
        }
        //</editor-fold>

        //<editor-fold desc="Beta plots (1n)">
        hBeta_nfph_vs_numOfFDph_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_Theta_n_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_Phi_n_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_SF_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_SF_PCAL_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_E_PCAL_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_SF_ECIN_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_E_ECIN_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_SF_ECOUT_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_E_ECOUT_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_VcalECIN_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_WcalECIN_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_VcalECOUT_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_WcalECOUT_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_nfph_vs_P_e_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nBeta plots are disabled by user.\n\n";
    } // end of Beta plot if
    //</editor-fold>

    //<editor-fold desc="Beta vs. P plots">
    if (Beta_vs_P_plots) {
        cout << "\n\nPlotting Beta vs. P histograms...\n\n";

//  Beta vs. P TF1 plots ------------------------------------------------------------------------------------------------------------------------------------------------

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

//  Beta vs. P plots ----------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Beta vs. P plots (no #(e) cut)">
        hBeta_vs_P_CD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                   beta_Kminus, true);
        hBeta_vs_P_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                   beta_Kminus, true);
        hBeta_vs_P_Electrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, "Electrons", true);
        hBeta_vs_P_Protons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_Protons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_Neutrons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);
        hBeta_vs_P_Neutrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);
        hBeta_vs_P_positive_part_All_e_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_positive_part_All_e_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_neutral_part_All_e_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_neutral_part_All_e_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_negative_part_All_e_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                       true);
        hBeta_vs_P_negative_part_All_e_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                       true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (1e cut)">
        hBeta_vs_P_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                          beta_Kminus, true);
        hBeta_vs_P_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                          beta_Kminus, true);
        hBeta_vs_P_1e_cut_Electrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, "Electrons", true);
        hBeta_vs_P_1e_cut_Protons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_1e_cut_Protons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_1e_cut_Neutrons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);
        hBeta_vs_P_1e_cut_Neutrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);
        hBeta_vs_P_1e_cut_Kplus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_Kplus, "K^{+}", true);
        hBeta_vs_P_1e_cut_Kplus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_Kplus, "K^{+}", true);
        hBeta_vs_P_1e_cut_Kminus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "K^{-}", true);
        hBeta_vs_P_1e_cut_Kminus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "K^{-}", true);
        hBeta_vs_P_1e_cut_piplus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_piplus, "#pi^{+}", true);
        hBeta_vs_P_1e_cut_piplus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_piplus, "#pi^{+}", true);
        hBeta_vs_P_1e_cut_piminus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_piminus, "#pi^{-}", true);
        hBeta_vs_P_1e_cut_piminus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_piminus, "#pi^{-}", true);
        hBeta_vs_P_1e_cut_pizero_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_pizero, "#pi^{0}", true);
        hBeta_vs_P_1e_cut_pizero_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_pizero, "#pi^{0}", true);
        hBeta_vs_P_positive_part_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_positive_part_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_neutral_part_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_neutral_part_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_negative_part_1e_cut_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                        true);
        hBeta_vs_P_negative_part_1e_cut_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                        true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (1p)">
        hBeta_vs_P_1p_CD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                      beta_Kminus, true);
        hBeta_vs_P_1p_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                      beta_Kminus, true);
        hBeta_vs_P_1p_Electrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, "Electrons", true);
        hBeta_vs_P_1p_Protons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_1p_Protons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_positive_part_1p_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_positive_part_1p_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_neutral_part_1p_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_neutral_part_1p_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_negative_part_1p_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                    true);
        hBeta_vs_P_negative_part_1p_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                    true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (1n)">
        hBeta_vs_P_1n_CD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_CD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
//                                      beta_Kminus, true);
        hBeta_vs_P_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
//                                      beta_Kminus, true);

        hBeta_vs_P_1n_Electrons_Only_FD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_Electrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, "Electrons", true);

        hBeta_vs_P_1n_Neutrons_Only_CD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_Neutrons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);
        hBeta_vs_P_1n_Neutrons_Only_FD.hDrawAndSave(SampleName, c1, plots, false);
        hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_Neutrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);

        hBeta_vs_P_1n_Neutrons_Only_from_photons_FD.hDrawAndSave(SampleName, c1, plots, false);

        hBeta_vs_P_1n_Photons_Only_CD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_Photons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);
        hBeta_vs_P_1n_Photons_Only_FD.hDrawAndSave(SampleName, c1, plots, false);
//        hBeta_vs_P_1n_Photons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", true);

        hBeta_vs_P_1n_Protons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_1n_Protons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_1n_Piplus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_piplus, "#pi^{+}", true);
        hBeta_vs_P_1n_Piplus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_piplus, "#pi^{+}", true);
        hBeta_vs_P_1n_Piminus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_piminus, "#pi^{-}", true);
        hBeta_vs_P_1n_Piminus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_piminus, "#pi^{-}", true);
        hBeta_vs_P_1n_Kplus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_Kplus, "K^{+}", true);
        hBeta_vs_P_1n_Kplus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_Kplus, "K^{+}", true);
        hBeta_vs_P_1n_Kminus_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "K^{-}", true);
        hBeta_vs_P_1n_Kminus_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "K^{-}", true);

        hBeta_vs_P_pos_part_1n_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_pos_part_1n_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_neut_part_1n_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_neut_part_1n_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_neg_part_1n_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        hBeta_vs_P_neg_part_1n_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (2p)">
        hBeta_vs_P_2p_CD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                      beta_Kminus, true);
        hBeta_vs_P_2p_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus,
                                      beta_Kminus, true);
        hBeta_vs_P_2p_Electrons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_electron, "Electrons", true);
        hBeta_vs_P_2p_Protons_Only_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_2p_Protons_Only_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", true);
        hBeta_vs_P_positive_part_2p_CD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_positive_part_2p_FD.hDrawAndSave(SampleName, c1, plots, beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        hBeta_vs_P_neutral_part_2p_CD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_neutral_part_2p_FD.hDrawAndSave(SampleName, c1, plots, beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        hBeta_vs_P_negative_part_2p_CD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                    true);
        hBeta_vs_P_negative_part_2p_FD.hDrawAndSave(SampleName, c1, plots, beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons",
                                                    true);
        //</editor-fold>

    } else {
        cout << "\n\nBeta vs. P plots are disabled by user.\n\n";
    } // end of Beta vs. P plot if
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Angle histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Angle histograms">
    if (Angle_plots_master) {
        cout << "\n\nPlotting Angle histograms...\n\n";

//  Theta_e plots (FD only) ---------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_e plots (no #(e) cut, FD)">
        double Theta_e_integral = hTheta_e_All_e_FD->Integral();

        histPlotter1D(c1, hTheta_e_All_e_FD, norm_Angle_plots_master, true, Theta_e_integral, "#theta_{e} of Outgoing Electron", "All Int., No #(e) Cut", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e, "Theta_e_All_Int_All_e", hTheta_e_All_e_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        //</editor-fold>

        //<editor-fold desc="Theta_e plots (1e cut, FD)">
        double Theta_e_1e_integral = hTheta_e_1e_cut_FD->Integral();

        histPlotter1D(c1, hTheta_e_1e_cut_FD, norm_Angle_plots_master, true, Theta_e_1e_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e Cut", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e, "Theta_e_All_Int_1e_cut", hTheta_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        //</editor-fold>

        //<editor-fold desc="Theta_e plots (1p, FD)">
        double Theta_e_All_Int_1p_integral = hTheta_e_All_Int_1p_FD->Integral();
        double Theta_e_QEL_1p_integral = hTheta_e_QEL_1p_FD->Integral();
        double Theta_e_MEC_1p_integral = hTheta_e_MEC_1p_FD->Integral();
        double Theta_e_RES_1p_integral = hTheta_e_RES_1p_FD->Integral();
        double Theta_e_DIS_1p_integral = hTheta_e_DIS_1p_FD->Integral();

        histPlotter1D(c1, hTheta_e_All_Int_1p_FD, norm_Angle_plots_master, true, Theta_e_All_Int_1p_integral, "#theta_{e} of Outgoing Electron", "All Int., 1p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1p_FD, "00_Theta_e_All_Int_1p", hTheta_e_All_Int_1p_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_QEL_1p_FD, norm_Angle_plots_master, true, Theta_e_QEL_1p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1p_FD, "01_Theta_e_QEL_Only_1p", hTheta_e_QEL_1p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_MEC_1p_FD, norm_Angle_plots_master, true, Theta_e_MEC_1p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1p_FD, "02_Theta_e_MEC_Only_1p", hTheta_e_MEC_1p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_RES_1p_FD, norm_Angle_plots_master, true, Theta_e_RES_1p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1p_FD, "03_Theta_e_RES_Only_1p", hTheta_e_RES_1p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_DIS_1p_FD, norm_Angle_plots_master, true, Theta_e_DIS_1p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1p_FD, "04_Theta_e_DIS_Only_1p", hTheta_e_DIS_1p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);

        stackPlotter1D(c1, sTheta_e_1p_FD, norm_Angle_plots_master, "#theta_{e} of Outgoing Electron", "1p", plots, hTheta_e_All_Int_1p_FD, hTheta_e_QEL_1p_FD,
                       hTheta_e_MEC_1p_FD, hTheta_e_RES_1p_FD, hTheta_e_DIS_1p_FD, "05_Theta_e_Stack_1p", hTheta_e_All_Int_1p_FD_Dir, "FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e plots (1n, FD)">
        double Theta_e_All_Int_1n_integral = hTheta_e_All_Int_1n_FD->Integral();
        double Theta_e_QEL_1n_integral = hTheta_e_QEL_1n_FD->Integral();
        double Theta_e_MEC_1n_integral = hTheta_e_MEC_1n_FD->Integral();
        double Theta_e_RES_1n_integral = hTheta_e_RES_1n_FD->Integral();
        double Theta_e_DIS_1n_integral = hTheta_e_DIS_1n_FD->Integral();

        histPlotter1D(c1, hTheta_e_All_Int_1n_FD, norm_Angle_plots_master, true, Theta_e_All_Int_1n_integral, "#theta_{e} of Outgoing Electron", "All Int., 1n", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1n_FD, "00_Theta_e_All_Int_1n", hTheta_e_All_Int_1n_FD_Dir, "FD", kBlue, true, true, true, false,
                      true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_QEL_1n_FD, norm_Angle_plots_master, true, Theta_e_QEL_1n_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1n_FD, "01_Theta_e_QEL_Only_1n", hTheta_e_QEL_1n_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_MEC_1n_FD, norm_Angle_plots_master, true, Theta_e_MEC_1n_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1n_FD, "02_Theta_e_MEC_Only_1n", hTheta_e_MEC_1n_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_RES_1n_FD, norm_Angle_plots_master, true, Theta_e_RES_1n_integral, "#theta_{e} of Outgoing Electron", "RES Only, 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1n_FD, "03_Theta_e_RES_Only_1n", hTheta_e_RES_1n_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);
        histPlotter1D(c1, hTheta_e_DIS_1n_FD, norm_Angle_plots_master, true, Theta_e_DIS_1n_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1n_FD, "04_Theta_e_DIS_Only_1n", hTheta_e_DIS_1n_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5,
                      22.5, false);

        stackPlotter1D(c1, sTheta_e_1n_FD, norm_Angle_plots_master, "#theta_{e} of Outgoing Electron", "1n", plots, hTheta_e_All_Int_1n_FD, hTheta_e_QEL_1n_FD,
                       hTheta_e_MEC_1n_FD, hTheta_e_RES_1n_FD, hTheta_e_DIS_1n_FD, "05_Theta_e_Stack_1n", hTheta_e_All_Int_1n_FD_Dir, "FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e plots (1e2p, FD)">
        double Theta_e_All_Int_1e2p_integral = hTheta_e_All_Int_1e2p_FD->Integral();
        double Theta_e_QEL_1e2p_integral = hTheta_e_QEL_1e2p_FD->Integral();
        double Theta_e_MEC_1e2p_integral = hTheta_e_MEC_1e2p_FD->Integral();
        double Theta_e_RES_1e2p_integral = hTheta_e_RES_1e2p_FD->Integral();
        double Theta_e_DIS_1e2p_integral = hTheta_e_DIS_1e2p_FD->Integral();

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

        //<editor-fold desc="Theta_e plots (2p, FD)">
        double Theta_e_All_Int_2p_integral = hTheta_e_All_Int_2p_FD->Integral();
        double Theta_e_QEL_2p_integral = hTheta_e_QEL_2p_FD->Integral();
        double Theta_e_MEC_2p_integral = hTheta_e_MEC_2p_FD->Integral();
        double Theta_e_RES_2p_integral = hTheta_e_RES_2p_FD->Integral();
        double Theta_e_DIS_2p_integral = hTheta_e_DIS_2p_FD->Integral();

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

//  Phi_e plots (FD only) -----------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Phi_e plots (no #(e) cut, FD)">
        double Phi_e_integral = hPhi_e_All_e_FD->Integral();

        histPlotter1D(c1, hPhi_e_All_e_FD, norm_Angle_plots_master, true, Phi_e_integral, "#phi_{e} of Outgoing Electron", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, sPhi_e, "Phi_e_All_Int_All_e", hPhi_e_All_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e plots (1e cut, FD)">
        double Phi_e_1e_integral = hPhi_e_1e_cut_FD->Integral();

        histPlotter1D(c1, hPhi_e_1e_cut_FD, norm_Angle_plots_master, true, Phi_e_1e_integral, "#phi_{e} of Outgoing Electron", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, sPhi_e, "Phi_e_All_Int_1e_cut", hPhi_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e plots (1p, FD)">
        double Phi_e_All_Int_1p_integral = hPhi_e_All_Int_1p_FD->Integral();
        double Phi_e_QEL_1p_integral = hPhi_e_QEL_1p_FD->Integral();
        double Phi_e_MEC_1p_integral = hPhi_e_MEC_1p_FD->Integral();
        double Phi_e_RES_1p_integral = hPhi_e_RES_1p_FD->Integral();
        double Phi_e_DIS_1p_integral = hPhi_e_DIS_1p_FD->Integral();

        histPlotter1D(c1, hPhi_e_All_Int_1p_FD, norm_Angle_plots_master, true, Phi_e_All_Int_1p_integral, "#phi_{e} of Outgoing Electron", "All Int., 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "00_phi_e_All_Int_cut_1p", hPhi_e_All_Int_1p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_1p_FD, norm_Angle_plots_master, true, Phi_e_QEL_1p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1p", hPhi_e_QEL_1p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_1p_FD, norm_Angle_plots_master, true, Phi_e_MEC_1p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1p", hPhi_e_MEC_1p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_1p_FD, norm_Angle_plots_master, true, Phi_e_RES_1p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1p", hPhi_e_RES_1p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_1p_FD, norm_Angle_plots_master, true, Phi_e_DIS_1p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1p", hPhi_e_DIS_1p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e plots (1n, FD)">
        double Phi_e_All_Int_1n_integral = hPhi_e_All_Int_1n_FD->Integral();
        double Phi_e_QEL_1n_integral = hPhi_e_QEL_1n_FD->Integral();
        double Phi_e_MEC_1n_integral = hPhi_e_MEC_1n_FD->Integral();
        double Phi_e_RES_1n_integral = hPhi_e_RES_1n_FD->Integral();
        double Phi_e_DIS_1n_integral = hPhi_e_DIS_1n_FD->Integral();

        histPlotter1D(c1, hPhi_e_All_Int_1n_FD, norm_Angle_plots_master, true, Phi_e_All_Int_1n_integral, "#phi_{e} of Outgoing Electron", "All Int., 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "00_phi_e_All_Int_cut_1n", hPhi_e_All_Int_1n_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_1n_FD, norm_Angle_plots_master, true, Phi_e_QEL_1n_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1n", hPhi_e_QEL_1n_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_1n_FD, norm_Angle_plots_master, true, Phi_e_MEC_1n_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1n", hPhi_e_MEC_1n_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_1n_FD, norm_Angle_plots_master, true, Phi_e_RES_1n_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1n", hPhi_e_RES_1n_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_1n_FD, norm_Angle_plots_master, true, Phi_e_DIS_1n_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1n", hPhi_e_DIS_1n_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e plots (1e2p, FD)">
        double Phi_e_All_Int_1e2p_integral = hPhi_e_All_Int_1e2p_FD->Integral();
        double Phi_e_QEL_1e2p_integral = hPhi_e_QEL_1e2p_FD->Integral();
        double Phi_e_MEC_1e2p_integral = hPhi_e_MEC_1e2p_FD->Integral();
        double Phi_e_RES_1e2p_integral = hPhi_e_RES_1e2p_FD->Integral();
        double Phi_e_DIS_1e2p_integral = hPhi_e_DIS_1e2p_FD->Integral();

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

        //<editor-fold desc="Phi_e plots (2p, FD)">
        double Phi_e_All_Int_2p_integral = hPhi_e_All_Int_2p_FD->Integral();
        double Phi_e_QEL_2p_integral = hPhi_e_QEL_2p_FD->Integral();
        double Phi_e_MEC_2p_integral = hPhi_e_MEC_2p_FD->Integral();
        double Phi_e_RES_2p_integral = hPhi_e_RES_2p_FD->Integral();
        double Phi_e_DIS_2p_integral = hPhi_e_DIS_2p_FD->Integral();

        histPlotter1D(c1, hPhi_e_All_Int_2p_FD, norm_Angle_plots_master, true, Phi_e_All_Int_2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "00_phi_e_All_Int_cut_2p", hPhi_e_All_Int_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_2p_FD, norm_Angle_plots_master, true, Phi_e_QEL_2p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_2p", hPhi_e_QEL_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_2p_FD, norm_Angle_plots_master, true, Phi_e_MEC_2p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_2p", hPhi_e_MEC_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_2p_FD, norm_Angle_plots_master, true, Phi_e_RES_2p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_2p", hPhi_e_RES_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_2p_FD, norm_Angle_plots_master, true, Phi_e_DIS_2p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_2p", hPhi_e_DIS_2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

//  Phi of protons (1e2pXy) ---------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Phi of protons (1e2pXy)">
        double Phi_p1_1e2pXy_CD_integral = hPhi_p1_1e2pXy_CD->Integral(), Phi_p2_1e2pXy_CD_integral = hPhi_p2_1e2pXy_CD->Integral();

        histPlotter1D(c1, hPhi_p1_1e2pXy_CD, norm_Angle_plots_master, true, Phi_p1_1e2pXy_CD_integral, "#phi_{p_{1}} of Leading Proton", "1e2pXy", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_Proton_1e2pXy, "01_Phi_p1_All_Int_1e2pXy", hPhi_p1_1e2pXy_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_p2_1e2pXy_CD, norm_Angle_plots_master, true, Phi_p2_1e2pXy_CD_integral, "#phi_{p_{2}} of Recoil Proton", "1e2pXy", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_Proton_1e2pXy, "01_Phi_p2_All_Int_1e2pXy", hPhi_p2_1e2pXy_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

//  Theta_e vs. Phi_e plots (FD only) -----------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_e vs. Phi_e plots (no #(e) cut, FD)">
        histPlotter2D(c1, hTheta_e_VS_Phi_e_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_All_e_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_All_e_FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e plots (1e cut, FD)">
        histPlotter2D(c1, hTheta_e_VS_Phi_e_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_1e_cut_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_1e_cut_FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e plots (1p, FD)">
        histPlotter2D(c1, hTheta_e_VS_Phi_e_1p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_1p_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_1p_FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e plots (1n, FD)">
        histPlotter2D(c1, hTheta_e_VS_Phi_e_1n_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_1n_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_1n_FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e plots (2p, FD)">
        histPlotter2D(c1, hTheta_e_VS_Phi_e_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_2p_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_2p_FD");
        //</editor-fold>

//  Other angle plots ---------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Other angle plots">

        //<editor-fold desc="1p plots">

//  Theta_p (1p, CD & FD) ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p (1p, CD & FD)">
        double Theta_p_1p_integral = hTheta_p_All_Int_1p->Integral();

        histPlotter1D(c1, hTheta_p_All_Int_1p, norm_Angle_plots_master, true, Theta_p_1p_integral, "#theta_{p} of Outgoing Proton", "All Int., 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_p_1p, "01_Theta_p_All_Int_1p", hTheta_p_All_Int_1p_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Phi_p (1p, CD & FD) ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Phi_p (1p, CD & FD)">
        double Phi_p_1p_integral = hPhi_p_All_Int_1p->Integral();

        histPlotter1D(c1, hPhi_p_All_Int_1p, norm_Angle_plots_master, true, Phi_p_1p_integral, "#phi_{p} of Outgoing Proton", "All Int., 1p", 0.06, 0.0425, 0.0425, plots,
                      2, false, true, sPhi_p_1p, "01_Phi_p_All_Int_1p", hPhi_p_All_Int_1p_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Theta_p_e_p_tot (1p, CD & FD) ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p_e_p_tot (1p, CD & FD)">
        double Theta_p_e_p_p_1p_integral = hTheta_p_e_p_p_1p->Integral();

        histPlotter1D(c1, hTheta_p_e_p_p_1p, norm_Angle_plots_master, true, Theta_p_e_p_p_1p_integral,
                      "#theta_{#vec{P}_{e},#vec{P}_{p}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{p}", "All Int., 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_p_e_p_p_1p, "01_Theta_p_e_p_p_All_Int_1p", hTheta_p_e_p_p_1p_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Theta_q_p_p (1p, FD only) ---------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_p (1p, FD only)">
        double Theta_q_p_p_1p_1p_integral = hTheta_q_p_p_1p->Integral();

        histPlotter1D(c1, hTheta_q_p_p_1p, norm_Angle_plots_master, true, Theta_q_p_p_1p_1p_integral,
                      "#theta_{#vec{q},#vec{P}_{p}} - Opening Angle Between #vec{q} and #vec{P}_{p}", "All Int., 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_q_p_p_1p, "02_Theta_q_p_p_All_Int_1p", hTheta_q_p_p_1p_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Theta_q_p_p vs. |P_p|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_p vs. |P_p|/|q| (1p, FD only)">
        histPlotter2D(c1, hTheta_q_p_p_vs_p_p_q_1p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hTheta_q_p_p_vs_p_p_q_1p_Dir, "03_Theta_q_p_p_vs_p_p_q_1p");
        //</editor-fold>

//  Theta_q_p_p vs. |p_N|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_p vs. |P_p|/|q| (1p, FD only)">
        histPlotter2D(c1, hTheta_q_p_p_vs_p_N_q_1p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hTheta_q_p_p_vs_p_N_q_1p_Dir, "04_Theta_q_p_N_vs_p_p_q_1p");
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="1n plots">

//  Theta_n (1n, CD & FD) ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_n (1n, CD & FD)">
        double Theta_n_1n_integral = hTheta_n_All_Int_1n->Integral();

        histPlotter1D(c1, hTheta_n_All_Int_1n, norm_Angle_plots_master, true, Theta_n_1n_integral, "#theta_{n} of Outgoing Neutron", "All Int., 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_n_1n, "01_Theta_n_All_Int_1n", hTheta_n_All_Int_1n_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Phi_n (1n, CD & FD) -------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Phi_n (1n, CD & FD)">
        double Phi_n_1n_integral = hPhi_n_All_Int_1n->Integral();

        histPlotter1D(c1, hPhi_n_All_Int_1n, norm_Angle_plots_master, true, Phi_n_1n_integral, "#phi_{n} of Outgoing Neutron", "All Int., 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_n_1n, "01_Phi_n_All_Int_1n", hPhi_n_All_Int_1n_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

// Theta_n vs. Phi_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_n vs. Phi_n plots (1e cut, FD)">
        /* All 1n neutrons (by definition) */
        hTheta_neut_VS_Phi_neut_All_e_FD.hDrawAndSave(SampleName, c1, plots, false);

        hTheta_n_VS_Phi_n_around_beta1_1n_FD.hDrawAndSave(SampleName, c1, plots, false);
        //</editor-fold>

//  Theta_p_e_p_tot (1n, CD & FD) ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p_e_p_tot (1n, CD & FD)">
        double Theta_p_e_p_n_1n_integral = hTheta_p_e_p_n_1n->Integral();

        histPlotter1D(c1, hTheta_p_e_p_n_1n, norm_Angle_plots_master, true, Theta_p_e_p_n_1n_integral,
                      "#theta_{#vec{P}_{e},#vec{P}_{n}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{n}", "All Int., 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_p_e_p_n_1n, "01_Theta_p_e_p_n_All_Int_1n", hTheta_p_e_p_n_1n_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Theta_q_p_n (1n, FD only) ---------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_n (1n, FD only)">
        double Theta_q_p_n_1n_1n_integral = hTheta_q_p_n_1n->Integral();

        histPlotter1D(c1, hTheta_q_p_n_1n, norm_Angle_plots_master, true, Theta_q_p_n_1n_1n_integral,
                      "#theta_{#vec{q},#vec{P}_{n}} - Opening Angle Between #vec{q} and #vec{P}_{n}", "All Int., 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_q_p_n_1n, "02_Theta_q_p_n_All_Int_1n", hTheta_q_p_n_1n_Dir, "FD", kBlue, true, true, true, false);
        //</editor-fold>

//  Theta_q_p_n vs. |p_n|/|q| (1n, FD only) -------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_n vs. |p_n|/|q| (1n, FD only)">
        histPlotter2D(c1, hTheta_q_p_n_vs_p_n_q_1n, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hTheta_q_p_n_vs_p_n_q_1n_Dir, "03_Theta_q_p_n_vs_p_n_q_1n");
        //</editor-fold>

//  Theta_q_p_n vs. |p_N|/|q| (1n, FD only) -------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_q_p_n vs. |p_N|/|q| (1n, FD only)">
        histPlotter2D(c1, hTheta_q_p_n_vs_p_N_q_1n, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hTheta_q_p_n_vs_p_N_q_1n_Dir, "04_Theta_q_p_n_vs_p_N_q_1n");
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="2p plots">

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

//  Ghost tracks handling (2p, CD only) ---------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD)">
        hTheta_p1_p2_VS_ToF1_ToF2_BC_2p.hDrawAndSave(SampleName, c1, plots, true);
        hTheta_p1_p2_VS_ToF1_ToF2_AC_2p.hDrawAndSave(SampleName, c1, plots, true);
        //</editor-fold>

        //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p, CD)">
        hTheta_p1_p2_VS_Pos1_Pos2_BC_2p.hDrawAndSave(SampleName, c1, plots, true);
        hTheta_p1_p2_VS_Pos1_Pos2_AC_2p.hDrawAndSave(SampleName, c1, plots, true);
        //</editor-fold>

        //</editor-fold>

//  Neutron veto plots -------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Neutron veto plots (1e cut)">
        hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut.hDrawAndSave(SampleName, c1, plots, true);
        hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut.hDrawAndSave(SampleName, c1, plots, true);
        //</editor-fold>

        //<editor-fold desc="Neutron veto plots (1n)">
        hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n.hDrawAndSave(SampleName, c1, plots, true);
        hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n.hDrawAndSave(SampleName, c1, plots, true);
        hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n.hDrawAndSave(SampleName, c1, plots, true);
//        hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1n.hDrawAndSave(SampleName, c1, plots, true);
        //</editor-fold>

        //<editor-fold desc="Neutron veto plots (1n1p)">
        hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n1p.hDrawAndSave(SampleName, c1, plots, true);
        hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1n1p.hDrawAndSave(SampleName, c1, plots, true);
        //</editor-fold>

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

//  Q2 plots (FD only) --------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Q2 plots (no #(e) cut, FD)">
        double Q2_All_e_integral = hQ2_All_e_FD->Integral();

        histPlotter1D(c1, hQ2_All_e_FD, norm_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_All_e,
                      "Q2_All_e", hQ2_All_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 plots (1e Cut, FD)">
        double Q2_1e_integral = hQ2_1e_cut_FD->Integral();

        histPlotter1D(c1, hQ2_1e_cut_FD, norm_Q2_plots, true, Q2_1e_integral, "Q^{2} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e_cut,
                      "Q2_1e_cut", hQ2_1e_cut_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 plots (1p, FD)">
        double Q2_1p_integral = hQ2_1p_FD->Integral();

        histPlotter1D(c1, hQ2_1p_FD, norm_Q2_plots, true, Q2_1p_integral, "Q^{2} Histogram", "1p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1p, "Q2_1p",
                      hQ2_1p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 plots (1n, FD)">
        double Q2_1n_integral = hQ2_1n_FD->Integral();

        histPlotter1D(c1, hQ2_1n_FD, norm_Q2_plots, true, Q2_1n_integral, "Q^{2} Histogram", "1n", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1n, "Q2_1n",
                      hQ2_1n_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 plots (1e2p, FD)">
        double Q2_1e2p_integral = hQ2_1e2p_FD->Integral();

        histPlotter1D(c1, hQ2_1e2p_FD, norm_Q2_plots, true, Q2_1e2p_integral, "Q^{2} Histogram", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e2p, "Q2_1e2p",
                      hQ2_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 plots (2p, FD)">
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

//  E_e plots (FD only) -------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="E_e plots (1e cut, FD)">
        double E_e_integral = hE_e_1e_cut_FD->Integral();

        histPlotter1D(c1, hE_e_1e_cut_FD, norm_E_e_plots, true, E_e_integral, "E_{e} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sE_e_1e_cut,
                      "01_E_e_1e_cut", hE_e_1e_cut_FD_Dir, "", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e plots (1p, FD)">
        double E_e_1p_All_Int_integral = hE_e_All_Int_1p_FD->Integral();
//        double E_e_QEL_Int_integral = hE_e_QEL_1p_FD->Integral(), E_e_MEC_Int_integral = hE_e_MEC_1p_FD->Integral();
//        double E_e_RES_Int_integral = hE_e_RES_1p_FD->Integral(), E_e_DIS_Int_integral = hE_e_DIS_1p_FD->Integral();

        histPlotter1D(c1, hE_e_All_Int_1p_FD, norm_E_e_plots, true, E_e_1p_All_Int_integral, "E_{e} Histogram", "All Int., 1p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1p_FD, "00_E_e_All_Int_1p", hE_e_All_Int_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_QEL_1p_FD, norm_E_e_plots, true, E_e_1p_All_Int_integral, "E_{e} Histogram", "QEL Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1p_FD, "01_E_e_QEL_Only_1p", hE_e_QEL_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_MEC_1p_FD, norm_E_e_plots, true, E_e_1p_All_Int_integral, "E_{e} Histogram", "MEC Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1p_FD, "02_E_e_MEC_Only_1p", hE_e_MEC_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_RES_1p_FD, norm_E_e_plots, true, E_e_1p_All_Int_integral, "E_{e} Histogram", "RES Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1p_FD, "03_E_e_RES_Only_1p", hE_e_RES_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_DIS_1p_FD, norm_E_e_plots, true, E_e_1p_All_Int_integral, "E_{e} Histogram", "DIS Only, 1p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1p_FD, "04_E_e_DIS_Only_1p", hE_e_DIS_1p_FD_Dir, "", kBlue, true, true, true);

        stackPlotter1D(c1, sE_e_1p_FD, norm_E_e_plots, "E_{e} Histogram", "1p", plots, hE_e_All_Int_1p_FD, hE_e_QEL_1p_FD, hE_e_MEC_1p_FD, hE_e_RES_1p_FD,
                       hE_e_DIS_1p_FD, "02_E_e_Stack", sE_e_1p_FD_Dir, "");

        //<editor-fold desc="Energy (E_e) histograms around theta_e = 15 (1p, CD & FD)">
        double E_e_15_1p_All_Int_integral = hE_e_15_All_Int_1p_FD->Integral();
//        double E_e_15_1p_QEL_Int_integral = hE_e_15_QEL_1p_FD->Integral(), E_e_15_1p_MEC_Int_integral = hE_e_15_MEC_1p_FD->Integral();
//        double E_e_15_1p_RES_Int_integral = hE_e_15_RES_1p_FD->Integral(), E_e_15_1p_DIS_Int_integral = hE_e_15_DIS_1p_FD->Integral();

        histPlotter1D(c1, hE_e_15_All_Int_1p_FD, norm_E_e_plots, true, E_e_15_1p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "All Int., 1p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sE_e_15_1p_FD, "00_E_e_15_All_Int_1p", hE_e_15_All_Int_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_QEL_1p_FD, norm_E_e_plots, true, E_e_15_1p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "QEL Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1p_FD, "01_E_e_15_QEL_Only_1p", hE_e_15_QEL_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_MEC_1p_FD, norm_E_e_plots, true, E_e_15_1p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "MEC Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1p_FD, "02_E_e_15_MEC_Only_1p", hE_e_15_MEC_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_RES_1p_FD, norm_E_e_plots, true, E_e_15_1p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "RES Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1p_FD, "03_E_e_15_RES_Only_1p", hE_e_15_RES_1p_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_DIS_1p_FD, norm_E_e_plots, true, E_e_15_1p_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "DIS Only, 1p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1p_FD, "04_E_e_15_DIS_Only_1p", hE_e_15_DIS_1p_FD_Dir, "", kBlue, true, true, true);

        stackPlotter1D(c1, sE_e_15_1p_FD, norm_E_e_plots, "E_{e} around #theta_{e} = 15#circ", "1p", plots, hE_e_15_All_Int_1p_FD, hE_e_15_QEL_1p_FD, hE_e_15_MEC_1p_FD,
                       hE_e_15_RES_1p_FD, hE_e_15_DIS_1p_FD, "02_E_e_15_Stack", sE_e_15_1p_FD_Dir, "");
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="E_e plots (1n, FD)">
        double E_e_1n_All_Int_integral = hE_e_All_Int_1n_FD->Integral();
//        double E_e_QEL_Int_integral = hE_e_QEL_1n_FD->Integral(), E_e_MEC_Int_integral = hE_e_MEC_1n_FD->Integral();
//        double E_e_RES_Int_integral = hE_e_RES_1n_FD->Integral(), E_e_DIS_Int_integral = hE_e_DIS_1n_FD->Integral();

        histPlotter1D(c1, hE_e_All_Int_1n_FD, norm_E_e_plots, true, E_e_1n_All_Int_integral, "E_{e} Histogram", "All Int., 1n", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1n_FD, "00_E_e_All_Int_1n", hE_e_All_Int_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_QEL_1n_FD, norm_E_e_plots, true, E_e_1n_All_Int_integral, "E_{e} Histogram", "QEL Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1n_FD, "01_E_e_QEL_Only_1n", hE_e_QEL_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_MEC_1n_FD, norm_E_e_plots, true, E_e_1n_All_Int_integral, "E_{e} Histogram", "MEC Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1n_FD, "02_E_e_MEC_Only_1n", hE_e_MEC_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_RES_1n_FD, norm_E_e_plots, true, E_e_1n_All_Int_integral, "E_{e} Histogram", "RES Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1n_FD, "03_E_e_RES_Only_1n", hE_e_RES_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_DIS_1n_FD, norm_E_e_plots, true, E_e_1n_All_Int_integral, "E_{e} Histogram", "DIS Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_1n_FD, "04_E_e_DIS_Only_1n", hE_e_DIS_1n_FD_Dir, "", kBlue, true, true, true);

        stackPlotter1D(c1, sE_e_1n_FD, norm_E_e_plots, "E_{e} Histogram", "1n", plots, hE_e_All_Int_1n_FD, hE_e_QEL_1n_FD, hE_e_MEC_1n_FD, hE_e_RES_1n_FD,
                       hE_e_DIS_1n_FD, "02_E_e_Stack", sE_e_1n_FD_Dir, "");

        //<editor-fold desc="Energy (E_e) histograms around theta_e = 15 (1n, CD & FD)">
        double E_e_15_1n_All_Int_integral = hE_e_15_All_Int_1n_FD->Integral();
//        double E_e_15_1n_QEL_Int_integral = hE_e_15_QEL_1n_FD->Integral(), E_e_15_1n_MEC_Int_integral = hE_e_15_MEC_1n_FD->Integral();
//        double E_e_15_1n_RES_Int_integral = hE_e_15_RES_1n_FD->Integral(), E_e_15_1n_DIS_Int_integral = hE_e_15_DIS_1n_FD->Integral();

        histPlotter1D(c1, hE_e_15_All_Int_1n_FD, norm_E_e_plots, true, E_e_15_1n_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "All Int., 1n", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sE_e_15_1n_FD, "00_E_e_15_All_Int_1n", hE_e_15_All_Int_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_QEL_1n_FD, norm_E_e_plots, true, E_e_15_1n_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "QEL Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1n_FD, "01_E_e_15_QEL_Only_1n", hE_e_15_QEL_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_MEC_1n_FD, norm_E_e_plots, true, E_e_15_1n_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "MEC Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1n_FD, "02_E_e_15_MEC_Only_1n", hE_e_15_MEC_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_RES_1n_FD, norm_E_e_plots, true, E_e_15_1n_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "RES Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1n_FD, "03_E_e_15_RES_Only_1n", hE_e_15_RES_1n_FD_Dir, "", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_15_DIS_1n_FD, norm_E_e_plots, true, E_e_15_1n_All_Int_integral, "E_{e} around #theta_{e} = 15#circ", "DIS Only, 1n", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sE_e_15_1n_FD, "04_E_e_15_DIS_Only_1n", hE_e_15_DIS_1n_FD_Dir, "", kBlue, true, true, true);

        stackPlotter1D(c1, sE_e_15_1n_FD, norm_E_e_plots, "E_{e} around #theta_{e} = 15#circ", "1n", plots, hE_e_15_All_Int_1n_FD, hE_e_15_QEL_1n_FD, hE_e_15_MEC_1n_FD,
                       hE_e_15_RES_1n_FD, hE_e_15_DIS_1n_FD, "02_E_e_15_Stack", sE_e_15_1n_FD_Dir, "");
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="E_e plots (2p, FD)">
        double E_e_2p_All_Int_integral = hE_e_All_Int_2p_FD->Integral();
        double E_e_QEL_integral = hE_e_QEL_2p_FD->Integral(), E_e_MEC_integral = hE_e_MEC_2p_FD->Integral();
        double E_e_RES_integral = hE_e_RES_2p_FD->Integral(), E_e_DIS_integral = hE_e_DIS_2p_FD->Integral();

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
        double E_e_15_2p_QEL_integral = hE_e_15_QEL_2p_FD->Integral(), E_e_15_2p_MEC_integral = hE_e_15_MEC_2p_FD->Integral();
        double E_e_15_2p_RES_integral = hE_e_15_RES_2p_FD->Integral(), E_e_15_2p_DIS_integral = hE_e_15_DIS_2p_FD->Integral();

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

//  E_e vs. Theta_e plots (FD only) -------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="E_e vs. Theta_e histograms (1e cut, CD & FD)">
        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_All_e_FD");
        //</editor-fold>

        //<editor-fold desc="E_e vs. Theta_e histograms (1p, CD & FD)">
        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_1p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_1p_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_1p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_QEL_1p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_QEL_1p_FD_Dir, "01_E_e_VS_Theta_e_QEL_Only_1p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_MEC_1p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_MEC_1p_FD_Dir, "02_E_e_VS_Theta_e_MEC_Only_1p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_RES_1p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_RES_1p_FD_Dir, "03_E_e_VS_Theta_e_RES_Only_1p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_DIS_1p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_DIS_1p_FD_Dir, "04_E_e_VS_Theta_e_DIS_Only_1p_FD");
        //</editor-fold>

        //<editor-fold desc="E_e vs. Theta_e histograms (1n, CD & FD)">
        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_1n_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_1n_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_1n_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_QEL_1n_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_QEL_1n_FD_Dir, "01_E_e_VS_Theta_e_QEL_Only_1n_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_MEC_1n_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_MEC_1n_FD_Dir, "02_E_e_VS_Theta_e_MEC_Only_1n_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_RES_1n_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_RES_1n_FD_Dir, "03_E_e_VS_Theta_e_RES_Only_1n_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_DIS_1n_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_DIS_1n_FD_Dir, "04_E_e_VS_Theta_e_DIS_Only_1n_FD");
        //</editor-fold>

        //<editor-fold desc="E_e vs. Theta_e histograms (2p, CD & FD)">
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

            double ETrans_All_Ang_All_Int_integral_1n = hET_All_Ang_All_Int_1n_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_All_Int_1n_FD, norm_ET_plots, true, ETrans_All_Ang_All_Int_integral_1n, tET_All_Ang, "All Int., 1n", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, sET_All_Ang_All_Int_1n_FD, "ET_All_Ang_All_Int_1n", hET_All_Ang_All_Int_1n_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_All_Int_integral_1n = hET15_All_Int_1n_FD->Integral();

            histPlotter1D(c1, hET15_All_Int_1n_FD, norm_ET_plots, true, ETrans15_All_Int_integral_1n, tET15, "All Int., 1n", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1n_FD, "ETrans_15_All_Int_1n", hET15_All_Int_1n_FD_Dir, "", kBlack, true, true, true);

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

            double ETrans_All_Ang_QEL_integral_1n = hET_All_Ang_QEL_1n_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_QEL_1n_FD, norm_ET_plots, true, ETrans_All_Ang_QEL_integral_1n, tET_All_Ang, "QEL Only, 1n", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1n_FD, "ET_All_Ang_QEL_1n", hET_All_Ang_QEL_1n_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_QEL_integral_1n = hET15_QEL_1n_FD->Integral();

            histPlotter1D(c1, hET15_QEL_1n_FD, norm_ET_plots, true, ETrans15_QEL_integral_1n, tET15, "QEL Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1n_FD, "ETrans_15_QEL_Only_1n", hET15_QEL_1n_FD_Dir, "", kBlue, true, true, true);

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

            double ETrans_All_Ang_MEC_integral_1n = hET_All_Ang_MEC_1n_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_MEC_1n_FD, norm_ET_plots, true, ETrans_All_Ang_MEC_integral_1n, tET_All_Ang, "MEC Only, 1n", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1n_FD, "ET_All_Ang_MEC_1n", hET_All_Ang_MEC_1n_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_MEC_integral_1n = hET15_MEC_1n_FD->Integral();

            histPlotter1D(c1, hET15_MEC_1n_FD, norm_ET_plots, true, ETrans15_MEC_integral_1n, tET15, "MEC Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1n_FD, "ETrans_15_MEC_Only_1n", hET15_MEC_1n_FD_Dir, "", kRed, true, true, true);

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

            double ETrans_All_Ang_RES_integral_1n = hET_All_Ang_RES_1n_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_RES_1n_FD, norm_ET_plots, true, ETrans_All_Ang_RES_integral_1n, tET_All_Ang, "RES Only, 1n", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1n_FD, "ET_All_Ang_RES_1n", hET_All_Ang_RES_1n_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_RES_integral_1n = hET15_RES_1n_FD->Integral();

            histPlotter1D(c1, hET15_RES_1n_FD, norm_ET_plots, true, ETrans15_RES_integral_1n, tET15, "RES Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1n_FD, "ETrans_15_RES_1n", hET15_RES_1n_FD_Dir, "", kGreen, true, true, true);

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

            double ETrans_All_Ang_DIS_integral_1n = hET_All_Ang_DIS_1n_FD->Integral();

            histPlotter1D(c1, hET_All_Ang_DIS_1n_FD, norm_ET_plots, true, ETrans_All_Ang_DIS_integral_1n, tET_All_Ang, "DIS Only, 1n", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, sET_All_Ang_All_Int_1n_FD, "ET_All_Ang_DIS_1n", hET_All_Ang_DIS_1n_FD_Dir, "FD", kBlack, true, true, true);

            double ETrans15_DIS_integral_1n = hET15_DIS_1n_FD->Integral();

            histPlotter1D(c1, hET15_DIS_1n_FD, norm_ET_plots, true, ETrans15_DIS_integral_1n, tET15, "DIS Only, 1n", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_1n_FD, "ETrans_15_DIS_1n", hET15_DIS_1n_FD_Dir, "", kMagenta, true, true, true);

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

            stackPlotter1D(c1, sET_All_Ang_All_Int_1n_FD, norm_ET_plots, tET_All_Ang, "1n", plots, hET_All_Ang_All_Int_1n_FD, hET_All_Ang_QEL_1n_FD,
                           hET_All_Ang_MEC_1n_FD,
                           hET_All_Ang_RES_1n_FD, hET_All_Ang_DIS_1n_FD, "02_ET_All_Ang_stack", sET_All_Ang_All_Int_1n_FD_Dir, "");

            stackPlotter1D(c1, sET15_All_Int_1n_FD, norm_ET_plots, tET15, "1n", plots, hET15_All_Int_1n_FD, hET15_QEL_1n_FD, hET15_MEC_1n_FD,
                           hET15_RES_1n_FD, hET15_DIS_1n_FD, "02_ET15_stack", sET15_All_Int_1n_FD_Dir, "");

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

//  Ecal plots (CD & FD) ------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Ecal plots (1p)">
        histPlotter1D(c1, hEcal_All_Int_1p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1p,
                      "Ecal_rec_All_Int", hEcal_All_Int_1p_Dir, "1p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_QEL_1p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1p,
                      "Ecal_rec_QEL_only", hEcal_QEL_1p_Dir, "1p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_MEC_1p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1p,
                      "Ecal_rec_MEC_only", hEcal_MEC_1p_Dir, "1p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_RES_1p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "RES Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1p,
                      "Ecal_rec_RES_only", hEcal_RES_1p_Dir, "1p", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_DIS_1p, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1p,
                      "Ecal_rec_DIS_only", hEcal_DIS_1p_Dir, "1p", kBlue, true, true, true);

        stackPlotter1D(c1, sEcal_1p, norm_Ecal_plots, "E_{cal} Reconstruction", "1p", plots, hEcal_All_Int_1p, hEcal_QEL_1p, hEcal_MEC_1p, hEcal_RES_1p,
                       hEcal_DIS_1p, "01_Ecal_rec_stack", sEcal_1p_Dir, "");
        //</editor-fold>

        //<editor-fold desc="Ecal plots (1n)">
        histPlotter1D(c1, hEcal_All_Int_1n, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1n,
                      "Ecal_rec_All_Int", hEcal_All_Int_1n_Dir, "1n", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_QEL_1n, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1n,
                      "Ecal_rec_QEL_only", hEcal_QEL_1n_Dir, "1n", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_MEC_1n, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1n,
                      "Ecal_rec_MEC_only", hEcal_MEC_1n_Dir, "1n", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_RES_1n, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "RES Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1n,
                      "Ecal_rec_RES_only", hEcal_RES_1n_Dir, "1n", kBlue, true, true, true);
        histPlotter1D(c1, hEcal_DIS_1n, norm_Ecal_plots, true, 1., "E_{cal} Reconstruction", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2, false, true, sEcal_1n,
                      "Ecal_rec_DIS_only", hEcal_DIS_1n_Dir, "1n", kBlue, true, true, true);

        stackPlotter1D(c1, sEcal_1n, norm_Ecal_plots, "E_{cal} Reconstruction", "1n", plots, hEcal_All_Int_1n, hEcal_QEL_1n, hEcal_MEC_1n, hEcal_RES_1n,
                       hEcal_DIS_1n, "01_Ecal_rec_stack", sEcal_1n_Dir, "");
        //</editor-fold>

        //<editor-fold desc="Ecal plots (2p)">
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
                       hEcal_DIS_2p, "01_Ecal_rec_stack", sEcal_2p_Dir, "");
        //</editor-fold>

//  Chi2 plots for Ecal>Ebeam (CD & FD) ---------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Chi2 plots for Ecal>Ebeam (1p)">
        histPlotter1D(c1, hChi2_Electron_Ecal_test_1p, norm_Chi2_plots, true, 1., ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "1p",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, sChi2_Electron_1e1p, "01_Electron_Chi2_Ecal_test_1p", hChi2_Electron_Ecal_test_1p_Dir, "", kBlue, true,
                      true, true, false);
        histPlotter1D(c1, hChi2_Proton_Ecal_test_1p, norm_Chi2_plots, true, 1., ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "1p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sChi2_Proton_1e1p, "02_Proton_Chi2_Ecal_test_1p", hChi2_Proton_Ecal_test_1p_Dir, "", kBlue, true, true,
                      true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
        //</editor-fold>

        //<editor-fold desc="Chi2 plots for Ecal>Ebeam (1n)">
        histPlotter1D(c1, hChi2_Electron_Ecal_test_1n, norm_Chi2_plots, true, 1., ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "1n",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, sChi2_Electron_1e1n, "01_Electron_Chi2_Ecal_test_1n", hChi2_Electron_Ecal_test_1n_Dir, "", kBlue, true,
                      true, true, false);
        histPlotter1D(c1, hChi2_Proton_Ecal_test_1n, norm_Chi2_plots, true, 1., ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "1n", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sChi2_Proton_1e1n, "02_Proton_Chi2_Ecal_test_1n", hChi2_Proton_Ecal_test_1n_Dir, "", kBlue, true, true,
                      true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
        //</editor-fold>

        //<editor-fold desc="Chi2 plots for Ecal>Ebeam (2p)">
        histPlotter1D(c1, hChi2_Electron_Ecal_test_2p, norm_Chi2_plots, true, 1., ("#chi^{2}_{e} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "2p",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, sChi2_Electron_1e2p, "01_Electron_Chi2_Ecal_test_2p", hChi2_Electron_Ecal_test_2p_Dir, "", kBlue, true,
                      true, true, false);
        histPlotter1D(c1, hChi2_Proton_Ecal_test_2p, norm_Chi2_plots, true, 1., ("#chi^{2}_{p} for E_{cal}>E_{beam}=" + to_string(beamE) + " [GeV]").c_str(), "2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sChi2_Proton_1e2p, "02_Proton_Chi2_Ecal_test_2p", hChi2_Proton_Ecal_test_2p_Dir, "", kBlue, true, true,
                      true, false, true, Chi2_Proton_cuts_CD.Cuts.at(2), Chi2_Proton_cuts_CD.Cuts.at(0));
        //</editor-fold>

//  Ecal vs. dP_T plots (CD & FD) ---------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Ecal vs. dP_T plots (1p)">
        histPlotter2D(c1, hEcal_vs_dP_T_1p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dP_T_1p_Dir, "02_Ecal_vs_dP_T_1p", false);
        //</editor-fold>

        //<editor-fold desc="Ecal vs. dP_T plots (1n)">
        histPlotter2D(c1, hEcal_vs_dP_T_1n, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dP_T_1n_Dir, "02_Ecal_vs_dP_T_1n", false);
        //</editor-fold>

        //<editor-fold desc="Ecal vs. dP_T plots (2p)">
        histPlotter2D(c1, hEcal_vs_dP_T_L_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dP_T_L_2p_Dir, "02_Ecal_vs_dP_T_L_2p", false);
        histPlotter2D(c1, hEcal_vs_dP_T_tot_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dP_T_tot_2p_Dir, "03_Ecal_vs_dP_T_tot_2p", false);
        //</editor-fold>

//  Ecal vs. dAlpha_T plots (CD & FD) -----------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Ecal vs. dAlpha_T plots (1p)">
        histPlotter2D(c1, hEcal_vs_dAlpha_T_1p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dAlpha_T_1p_Dir, "02_Ecal_vs_dAlpha_T_1p", false);
        //</editor-fold>

        //<editor-fold desc="Ecal vs. dAlpha_T plots (1n)">
        histPlotter2D(c1, hEcal_vs_dAlpha_T_1n, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dAlpha_T_1n_Dir, "02_Ecal_vs_dAlpha_T_1n", false);
        //</editor-fold>

        //<editor-fold desc="Ecal vs. dAlpha_T plots (2p)">
        histPlotter2D(c1, hEcal_vs_dAlpha_T_L_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dAlpha_T_L_2p_Dir, "02_Ecal_vs_dAlpha_T_L_2p", false);
        histPlotter2D(c1, hEcal_vs_dAlpha_T_tot_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hEcal_vs_dAlpha_T_tot_2p_Dir, "03_Ecal_vs_dAlpha_T_tot_2p", false);
        //</editor-fold>

    } else {
        cout << "\n\nEcal plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Transverse variables histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Transverse variables histograms">
    if (TVariables_plots) {
        cout << "\n\nTransverse variables histograms...\n\n";

//  dP_T plots (CD & FD) ------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="dP_T plots (1p)">
        histPlotter1D(c1, hdP_T_1p, norm_TVariables_plots, true, 1., "#deltaP_{T} histogram", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdP_T_1p, "dP_T_1p", hdP_T_1p_Dir, "1p", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="dP_T plots (1n)">
        histPlotter1D(c1, hdP_T_1n, norm_TVariables_plots, true, 1., "#deltaP_{T} histogram", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdP_T_1n, "dP_T_1n", hdP_T_1n_Dir, "1n", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="dP_T plots (2p)">
        histPlotter1D(c1, hdP_T_L_2p, norm_TVariables_plots, true, 1., "#deltaP_{T,L} by Leading Proton", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdP_T_2p, "dP_T_L_2p", hdP_T_L_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hdP_T_tot_2p, norm_TVariables_plots, true, 1., "#deltaP_{T,tot} by Momentum Sum", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdP_T_2p, "dP_T_tot_2p", hdP_T_tot_2p_Dir, "2p", kBlue, true, true, true);
        //</editor-fold>

//  dAlpha_T plots (CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="dAlpha_T plots (1p)">
        histPlotter1D(c1, hdAlpha_T_1p, norm_TVariables_plots, true, 1., "#delta#alpha_{T} histogram", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sdAlpha_T_1p, "dAlpha_T_1p", hdAlpha_T_1p_Dir, "1p", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="dAlpha_T plots (1n)">
        histPlotter1D(c1, hdAlpha_T_1n, norm_TVariables_plots, true, 1., "#delta#alpha_{T} histogram", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sdAlpha_T_1n, "dAlpha_T_1n", hdAlpha_T_1n_Dir, "1n", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="dAlpha_T plots (2p)">
        histPlotter1D(c1, hdAlpha_T_L_2p, norm_TVariables_plots, true, 1., "#delta#alpha_{T,L} by Leading Proton", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sdAlpha_T_2p, "dAlpha_T_L_2p", hdAlpha_T_L_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hdAlpha_T_tot_2p, norm_TVariables_plots, true, 1., "#delta#alpha_{T,tot} by Momentum Sum", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sdAlpha_T_2p, "dAlpha_T_tot_2p", hdAlpha_T_tot_2p_Dir, "2p", kBlue, true, true, true);
        //</editor-fold>

//  dPhi_T plots (CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="dPhi_T plots (1p)">
        histPlotter1D(c1, hdPhi_T_1p, norm_TVariables_plots, true, 1., "#delta#phi_{T} histogram", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdPhi_T_1p, "dPhi_T_1p", hdPhi_T_1p_Dir, "1p", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="dPhi_T plots (1n)">
        histPlotter1D(c1, hdPhi_T_1n, norm_TVariables_plots, true, 1., "#delta#phi_{T} histogram", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdPhi_T_1n, "dPhi_T_1n", hdPhi_T_1n_Dir, "1n", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="dPhi_T plots (2p)">
        histPlotter1D(c1, hdPhi_T_L_2p, norm_TVariables_plots, true, 1., "#delta#phi_{T,L} by Leading Proton", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdPhi_T_2p, "dPhi_T_L_2p", hdPhi_T_L_2p_Dir, "2p", kBlue, true, true, true);
        histPlotter1D(c1, hdPhi_T_tot_2p, norm_TVariables_plots, true, 1., "#delta#phi_{T,tot} by Momentum Sum", "All Int.", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sdPhi_T_2p, "dPhi_T_tot_2p", hdPhi_T_tot_2p_Dir, "2p", kBlue, true, true, true);
        //</editor-fold>

//  dP_T vs. dAlpha_T plots (CD & FD) -----------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="dP_T vs. dAlpha_T plots (1p)">
        histPlotter2D(c1, hdP_T_vs_dAlpha_T_1p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hdP_T_vs_dAlpha_T_1p_Dir, "01_dP_T_vs_dAlpha_T_1p", false);
        //</editor-fold>

        //<editor-fold desc="dP_T vs. dAlpha_T plots (1n)">
        histPlotter2D(c1, hdP_T_vs_dAlpha_T_1n, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hdP_T_vs_dAlpha_T_1n_Dir, "01_dP_T_vs_dAlpha_T_1n", false);
        //</editor-fold>

        //<editor-fold desc="dP_T vs. dAlpha_T plots (2p)">
        histPlotter2D(c1, hdP_T_L_vs_dAlpha_T_L_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hdP_T_L_vs_dAlpha_T_L_2p_Dir, "01_dP_T_L_vs_dAlpha_T_L_2p", false);
        histPlotter2D(c1, hdP_T_tot_vs_dAlpha_T_tot_2p, 0.06, true, 0.0425, 0.0425, 0.0425, plots, false, hdP_T_tot_vs_dAlpha_T_tot_2p_Dir,
                      "02_dP_T_tot_vs_dAlpha_T_tot_2p", false);
        //</editor-fold>

    } else {
        cout << "\n\nTransverse variables plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// ToF histograms
// ======================================================================================================================================================================

    //<editor-fold desc="ToF histograms">
    if (ToF_plots) {
        cout << "\n\nToF histograms...\n\n";

    } else {
        cout << "\n\nToF plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Efficiency histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Efficiency histograms">
    if (Efficiency_plots) {
        cout << "\n\nPlotting Efficiency histograms...\n\n";

//  Efficiency plots -----------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Efficiency plots (1e cut, CD & FD)">

        //<editor-fold desc="Momentum efficiency plots (1e cut)">
        hP_e_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_e_mom_cuts.GetLowerCut(), TL_e_mom_cuts.GetUpperCut(), 0, false);
        hP_e_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_e_mom_cuts.GetLowerCut(), TL_e_mom_cuts.GetUpperCut(), 0, false);

        hP_n_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);
        hP_n_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);

        hP_p_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);
        hP_p_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);

        hP_pip_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pip_mom_cuts.GetLowerCut(), TL_pip_mom_cuts.GetUpperCut(), 0, false);
        hP_pip_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pip_mom_cuts.GetLowerCut(), TL_pip_mom_cuts.GetUpperCut(), 0, false);

        hP_pim_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pim_mom_cuts.GetLowerCut(), TL_pim_mom_cuts.GetUpperCut(), 0, false);
        hP_pim_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pim_mom_cuts.GetLowerCut(), TL_pim_mom_cuts.GetUpperCut(), 0, false);

        hP_pi0_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pi0_mom_cuts.GetLowerCut(), TL_pi0_mom_cuts.GetUpperCut(), 0, false);
        hP_pi0_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pi0_mom_cuts.GetLowerCut(), TL_pi0_mom_cuts.GetUpperCut(), 0, false);

        hP_ph_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        //</editor-fold>

        //<editor-fold desc="Theta efficiency plots (1e cut)">
        hTheta_e_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_e_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_n_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_n_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_p_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_p_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pip_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pip_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pim_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pim_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pi0_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pi0_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_ph_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        //</editor-fold>

        //<editor-fold desc="Phi efficiency plots (1e cut)">
        hPhi_e_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_e_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_n_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_n_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_p_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_p_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pip_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pip_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pim_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pim_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pi0_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pi0_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_ph_AC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_BC_truth_1e_cut.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Efficiency plots (1p, CD & FD)">

        //<editor-fold desc="Momentum efficiency plots (1p)">
        hP_e_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_e_mom_cuts.GetLowerCut(), TL_e_mom_cuts.GetUpperCut(), 0, false);
        hP_e_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_e_mom_cuts.GetLowerCut(), TL_e_mom_cuts.GetUpperCut(), 0, false);

        hP_n_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);
        hP_n_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);

        hP_p_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);
        hP_p_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);
        hP_p_AC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);
        hP_p_BC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);

        hP_pip_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pip_mom_cuts.GetLowerCut(), TL_pip_mom_cuts.GetUpperCut(), 0, false);
        hP_pip_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pip_mom_cuts.GetLowerCut(), TL_pip_mom_cuts.GetUpperCut(), 0, false);

        hP_pim_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pim_mom_cuts.GetLowerCut(), TL_pim_mom_cuts.GetUpperCut(), 0, false);
        hP_pim_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pim_mom_cuts.GetLowerCut(), TL_pim_mom_cuts.GetUpperCut(), 0, false);

        hP_pi0_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pi0_mom_cuts.GetLowerCut(), TL_pi0_mom_cuts.GetUpperCut(), 0, false);
        hP_pi0_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pi0_mom_cuts.GetLowerCut(), TL_pi0_mom_cuts.GetUpperCut(), 0, false);

        hP_ph_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_AC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_BC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);

        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hP_e_AC_truth_1p, hP_e_APID_1p_FD, plots);
        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hP_p_AC_truth_1p_FD, hP_p_APID_1p_FD, plots);

        DrawAndSaveEfficiencyPlots(SampleName, hP_e_AC_truth_1p, hP_e_APID_1p_FD, plots);
        DrawAndSaveEfficiencyPlots(SampleName, hP_p_AC_truth_1p_FD, hP_p_APID_1p_FD, plots);
        //</editor-fold>

        //<editor-fold desc="Theta efficiency plots (1p)">
        hTheta_e_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_e_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_n_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_n_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_p_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_p_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_p_AC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_p_BC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pip_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pip_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pim_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pim_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pi0_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pi0_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_ph_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_AC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_BC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hTheta_e_AC_truth_1p, hTheta_e_All_Int_1p_FD, plots);
        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hTheta_p_AC_truth_1p_FD, hTheta_p_All_Int_1p, plots);

        DrawAndSaveEfficiencyPlots(SampleName, hTheta_e_AC_truth_1p, hTheta_e_All_Int_1p_FD, plots);
        DrawAndSaveEfficiencyPlots(SampleName, hTheta_p_AC_truth_1p_FD, hTheta_p_All_Int_1p, plots);
        //</editor-fold>

        //<editor-fold desc="Phi efficiency plots (1p)">
        hPhi_e_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_e_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_n_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_n_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_p_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_p_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_p_AC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_p_BC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pip_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pip_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pim_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pim_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pi0_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pi0_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_ph_AC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_BC_truth_1p.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_AC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_BC_truth_1p_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hPhi_e_AC_truth_1p, hPhi_e_All_Int_1p_FD, plots);
        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hPhi_p_AC_truth_1p_FD, hPhi_p_All_Int_1p, plots);

        DrawAndSaveEfficiencyPlots(SampleName, hPhi_e_AC_truth_1p, hPhi_e_All_Int_1p_FD, plots);
        DrawAndSaveEfficiencyPlots(SampleName, hPhi_p_AC_truth_1p_FD, hPhi_p_All_Int_1p, plots);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Efficiency plots (1n, CD & FD)">

        //<editor-fold desc="Momentum efficiency plots (1n)">
        hP_e_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_e_mom_cuts.GetLowerCut(), TL_e_mom_cuts.GetUpperCut(), 0, false);
        hP_e_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_e_mom_cuts.GetLowerCut(), TL_e_mom_cuts.GetUpperCut(), 0, false);

        hP_n_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);
        hP_n_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);
        hP_n_AC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);
        hP_n_BC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_n_mom_cuts.GetLowerCut(), TL_n_mom_cuts.GetUpperCut(), 0, false);

        hP_p_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);
        hP_p_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_p_mom_cuts.GetLowerCut(), TL_p_mom_cuts.GetUpperCut(), 0, false);

        hP_pip_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pip_mom_cuts.GetLowerCut(), TL_pip_mom_cuts.GetUpperCut(), 0, false);
        hP_pip_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pip_mom_cuts.GetLowerCut(), TL_pip_mom_cuts.GetUpperCut(), 0, false);

        hP_pim_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pim_mom_cuts.GetLowerCut(), TL_pim_mom_cuts.GetUpperCut(), 0, false);
        hP_pim_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pim_mom_cuts.GetLowerCut(), TL_pim_mom_cuts.GetUpperCut(), 0, false);

        hP_pi0_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pi0_mom_cuts.GetLowerCut(), TL_pi0_mom_cuts.GetUpperCut(), 0, false);
        hP_pi0_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_pi0_mom_cuts.GetLowerCut(), TL_pi0_mom_cuts.GetUpperCut(), 0, false);

        hP_ph_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_AC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);
        hP_ph_BC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Momentum_plots, true, 1., TL_ph_mom_cuts.GetLowerCut(), TL_ph_mom_cuts.GetUpperCut(), 0, false);

        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hP_e_AC_truth_1n, hP_e_APID_1n_FD, plots);
        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hP_n_AC_truth_1n_FD, hP_n_APID_1n_FD, plots);

        DrawAndSaveEfficiencyPlots(SampleName, hP_e_AC_truth_1n, hP_e_APID_1n_FD, plots);
        DrawAndSaveEfficiencyPlots(SampleName, hP_n_AC_truth_1n_FD, hP_n_APID_1n_FD, plots);
        //</editor-fold>

        //<editor-fold desc="Theta efficiency plots (1n)">
        hTheta_e_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_e_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_n_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_n_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_n_AC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_n_BC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_p_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_p_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pip_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pip_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pim_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pim_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_pi0_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_pi0_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hTheta_ph_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_AC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hTheta_ph_BC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hTheta_e_AC_truth_1n, hTheta_e_All_Int_1n_FD, plots);
        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hTheta_n_AC_truth_1n_FD, hTheta_n_All_Int_1n, plots);

        DrawAndSaveEfficiencyPlots(SampleName, hTheta_e_AC_truth_1n, hTheta_e_All_Int_1n_FD, plots);
        DrawAndSaveEfficiencyPlots(SampleName, hTheta_n_AC_truth_1n_FD, hTheta_n_All_Int_1n, plots);
        //</editor-fold>

        //<editor-fold desc="Phi efficiency plots (1n)">
        hPhi_e_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_e_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_n_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_n_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_n_AC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_n_BC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_p_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_p_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pip_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pip_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pim_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pim_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_pi0_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_pi0_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        hPhi_ph_AC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_BC_truth_1n.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_AC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);
        hPhi_ph_BC_truth_1n_FD.hDrawAndSave(SampleName, c1, plots, norm_Angle_plots_master, true, 1., 9999, 9999, 0, false);

        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hPhi_e_AC_truth_1n, hPhi_e_All_Int_1n_FD, plots);
        DrawAndSaveAcceptanceCorrectionPlots(SampleName, hPhi_n_AC_truth_1n_FD, hPhi_n_All_Int_1n, plots);

        DrawAndSaveEfficiencyPlots(SampleName, hPhi_e_AC_truth_1n, hPhi_e_All_Int_1n_FD, plots);
        DrawAndSaveEfficiencyPlots(SampleName, hPhi_n_AC_truth_1n_FD, hPhi_n_All_Int_1n, plots);
        //</editor-fold>

        //</editor-fold>

//  Efficiency comperison plots -----------------------------------------------------------------------------------------------------------------------------------------


    } else {
        cout << "\n\nEfficiency plots are disabled by user.\n\n";
    }
    //</editor-fold>

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          Saving histogram list and finishing execution                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Saving and finishing execution">

// Saving proton pid cuts to .par file ----------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving pid cuts to .par file">
    if (apply_cuts == true &&
        apply_chi2_cuts_1e_cut == false) { // log pid cuts only if all other cuts are enabled //todo: review this. make code preform cuts only in this case
        DSCuts chi2cuts[] = {Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD, Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD, Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD};
        int chi2cuts_length = 6;

        ofstream FittedPIDCuts;
        std::string FittedPIDCutsFilePath = CutsDirectory + "Fitted_PID_Cuts_-_" + SampleName + ".par";

        FittedPIDCuts.open(FittedPIDCutsFilePath);
        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "# CLAS12 analysis cuts and parameters file (after chi2 Gaussian fit) #\n";
        FittedPIDCuts << "######################################################################\n";
        FittedPIDCuts << "\n";
        FittedPIDCuts << "# Cuts are fitted for - " + SampleName << "\n";
        FittedPIDCuts << "\n";
        FittedPIDCuts << "# pid cuts by detector (pid:mean:sigma) - sigma_CD=" << Chi2_Proton_cuts_CD.FitStdFactor << ";sigma_FD=" << Chi2_Proton_cuts_FD.FitStdFactor
                      << ":\n";

        for (int i = 0; i < chi2cuts_length; i++) {
            FittedPIDCuts << "pid_cuts" << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":" << chi2cuts[i].GetUpperCut() << ":"
                          << chi2cuts[i].GetRegion() << "\n";
        }
        FittedPIDCuts.close();

        system(("cp " + FittedPIDCutsFilePath + " " + plots_path).c_str()); // Copy fitted chi2 cuts file for easy download from ifarm
    }
    //</editor-fold>

    //<editor-fold desc="Saving neutron momentum and beta cuts to .par file">
    if (!apply_neutron_Beta_Fit) { // log neutron beta cut from fit
        /* Log cuts from 'photons' fit first */
        DSCuts Neutron_Momentum_Cuts[] = {n_momentum_cuts_ABF_FD_n_from_ph, n_momentum_cuts_ABF_FD_n_from_ph_apprax};
        int Neutron_Momentum_Cuts_length = 2;

        ofstream Fitted_n_Mom_Cuts;
        std::string Fitted_n_Mom_CutsFilePath = CutsDirectory + "Fitted_n_Mom_Cuts_-_" + SampleName + ".par";
        Fitted_n_Mom_Cuts.open(Fitted_n_Mom_CutsFilePath);
        Fitted_n_Mom_Cuts << "######################################################################\n";
        Fitted_n_Mom_Cuts << "# CLAS12 analysis cuts and parameters file (after Beta Gaussian fit) #\n";
        Fitted_n_Mom_Cuts << "######################################################################\n";
        Fitted_n_Mom_Cuts << "\n";
        Fitted_n_Mom_Cuts << "# Cuts are fitted for - " + SampleName + ":\t" + Beta_Fit_Status + FD_photons_Status + Efficiency_Status << "\n";
        Fitted_n_Mom_Cuts << "\n";

        //<editor-fold desc="Neutron momentum cuts by detector">
        Fitted_n_Mom_Cuts << "# Neutron momentum cuts by detector (pid:mean:sigma) - sigma_FD=" << n_mom_th.FitStdFactor << ":\n";

        Fitted_n_Mom_Cuts << n_mom_th.GetCutVariable() << "\t\t\t" << n_mom_th.GetPartPDG() << ":" << n_mom_th.Cuts.at(0) << ":" << n_mom_th.GetLowerCut() << ":"
                          << n_mom_th.GetRegion() << "\n";

        Fitted_n_Mom_Cuts << "\n";

        for (int i = 0; i < Neutron_Momentum_Cuts_length; i++) {
            Fitted_n_Mom_Cuts << Neutron_Momentum_Cuts[i].GetCutVariable() << "\t\t\t" << Neutron_Momentum_Cuts[i].GetPartPDG() << ":"
                              << Neutron_Momentum_Cuts[i].Cuts.at(0) << ":" << Neutron_Momentum_Cuts[i].GetUpperCut() << ":" << Neutron_Momentum_Cuts[i].GetRegion()
                              << "\n";
        }

        Fitted_n_Mom_Cuts << "\n";

        /* Log cuts from other tests */
        DSCuts Neutron_Momentum_Cuts_test[] = {n_momentum_cuts_ABF_FD_All_neutrals, n_momentum_cuts_ABF_FD_All_neutrals_noPDG0,
                                               n_momentum_cuts_ABF_FD_noPCAL, n_momentum_cuts_ABF_FD_noPCAL_noPDG0,
                                               n_momentum_cuts_ABF_FD_noPCAL_wECIN, n_momentum_cuts_ABF_FD_noPCAL_wECIN_noPDG0,
                                               n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT, n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT_noPDG0};
        int Neutron_Momentum_Cuts_test_length = 8;

        for (int i = 0; i < Neutron_Momentum_Cuts_test_length; i++) {
            Fitted_n_Mom_Cuts << Neutron_Momentum_Cuts_test[i].GetCutVariable() << "\t\t\t" << Neutron_Momentum_Cuts_test[i].GetPartPDG() << ":"
                              << Neutron_Momentum_Cuts_test[i].Cuts.at(0) << ":" << Neutron_Momentum_Cuts_test[i].GetUpperCut() << ":"
                              << Neutron_Momentum_Cuts_test[i].GetRegion() << "\n";
        }
        //</editor-fold>

        Fitted_n_Mom_Cuts << "\n\n";

        //<editor-fold desc="Beta std by detector">
        Fitted_n_Mom_Cuts << "# Beta std by detector (pid:mean:sigma) - sigma_FD=" << n_mom_th.FitStdFactor << ":\n";

        DSCuts Beta_cut_Fits[] = {Beta_cut_ABF_FD_n_from_ph, Beta_cut_ABF_FD_n_from_ph_apprax,
                                  Beta_cut_ABF_All_FD_neutrals, Beta_cut_ABF_FD_neutrals_noPDG0,
                                  Beta_cut_ABF_All_FD_neutrals_noPCAL, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL,
                                  Beta_cut_ABF_All_FD_neutrals_noPCAL_wECIN, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_wECIN,
                                  Beta_cut_ABF_All_FD_neutrals_noPCAL_noECIN_wECOUT, Beta_cut_ABF_FD_neutrals_noPDG0_noPCAL_noECIN_wECOUT};
        int Beta_cut_Fits_length = 10;

        for (int i = 0; i < Beta_cut_Fits_length; i++) {
            Fitted_n_Mom_Cuts << Beta_cut_Fits[i].GetCutVariable() << "\t\t\t" << Beta_cut_Fits[i].GetPartPDG() << ":" << Beta_cut_Fits[i].Cuts.at(0) << ":"
                              << Beta_cut_Fits[i].GetUpperCut() << ":" << Beta_cut_Fits[i].GetRegion() << "\n";
        }
        //</editor-fold>

        Fitted_n_Mom_Cuts.close();

        system(("cp " + Fitted_n_Mom_CutsFilePath + " " + plots_path).c_str()); // Copy fitted chi2 cuts file for easy download from ifarm
    }
    //</editor-fold>

// Saving debug files ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving debug files">
    if (PrintEvents == true) { EventPrint.close(); }

    if (debug_plots == true) {
        cout << "\n\nSaving debugging plots...\n\n";

        TString debug_filePath = plots_path + "/" + "DebugOutputFile.root";
        clasAna.setdebug_fileName(debug_filePath);
        clasAna.WriteDebugPlots();
    } else {
        cout << "\n\nDebugging plots are disabled...\n\n";
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

    //<editor-fold desc="Input">
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
    //</editor-fold>

    //<editor-fold desc="settings">

    //<editor-fold desc="Plot settings">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Plot settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "wider_margin = " << BoolToString(wider_margin) << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Calculation settings">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Calculation settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "calculate_1p = " << BoolToString(calculate_1p) << "\n";
    myLogFile << "calculate_1n = " << BoolToString(calculate_1n) << "\n";
    myLogFile << "calculate_1n1p = " << BoolToString(calculate_1n1p) << "\n";
    myLogFile << "calculate_2p = " << BoolToString(calculate_2p) << "\n\n";

    myLogFile << "calculate_truth_level = " << BoolToString(calculate_truth_level) << "\n";
    myLogFile << "Rec_wTL_ES = " << BoolToString(Rec_wTL_ES) << "\n";
    myLogFile << "Enable_FD_photons = " << BoolToString(Enable_FD_photons) << "\n\n";

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
    //</editor-fold>

    //<editor-fold desc="Plot selector">
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
    //</editor-fold>

    //<editor-fold desc="Normalization settings">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Normalization settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "normalize_master = " << BoolToString(normalize_master) << "\n\n";

    myLogFile << "norm_Nphe_plots = " << BoolToString(norm_Nphe_plots) << "\n";
    myLogFile << "norm_Chi2_plots = " << BoolToString(norm_Chi2_plots) << "\n";
    myLogFile << "norm_Vertex_plots = " << BoolToString(norm_Vertex_plots) << "\n";
    myLogFile << "norm_SF_plots = " << BoolToString(norm_SF_plots) << "\n";
    myLogFile << "norm_Momentum_plots = " << BoolToString(norm_Momentum_plots) << "\n";
    myLogFile << "norm_Fiducial_plots = " << BoolToString(norm_Fiducial_plots) << "\n\n";

    myLogFile << "norm_Angle_plots_master = " << BoolToString(norm_Angle_plots_master) << "\n";
    myLogFile << "norm_Q2_plots = " << BoolToString(norm_Q2_plots) << "\n";
    myLogFile << "norm_E_e_plots = " << BoolToString(norm_E_e_plots) << "\n";
    myLogFile << "norm_ET_plots = " << BoolToString(norm_ET_plots) << "\n";
    myLogFile << "norm_Ecal_plots = " << BoolToString(norm_Ecal_plots) << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Delete settings">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Delete settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "delete_png_files = " << BoolToString(delete_png_files) << "\n";
    myLogFile << "delete_root_files = " << BoolToString(delete_root_files) << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Cut application settings">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Cut application settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_cuts = " << BoolToString(apply_cuts) << "\n\n";

    myLogFile << "apply_Nphe_cut = " << BoolToString(apply_Nphe_cut) << "\n";
    myLogFile << "apply_chi2_cuts_1e_cut = " << BoolToString(apply_chi2_cuts_1e_cut) << "\n";
    myLogFile << "apply_Vz_cuts = " << BoolToString(apply_Vz_cuts) << "\n";
    myLogFile << "apply_dVz_cuts = " << BoolToString(apply_dVz_cuts) << "\n";
    myLogFile << "apply_ECAL_SF_cuts = " << BoolToString(apply_ECAL_SF_cuts) << "\n";
    myLogFile << "apply_ECAL_fiducial_cuts = " << BoolToString(apply_ECAL_fiducial_cuts) << "\n";
    myLogFile << "apply_DC_fiducial_cut = " << BoolToString(apply_DC_fiducial_cut) << "\n";
    myLogFile << "apply_momentum_cuts_1p = " << BoolToString(apply_momentum_cuts_1p) << "\n";
    myLogFile << "apply_momentum_cuts_1n = " << BoolToString(apply_momentum_cuts_1n) << "\n";
    myLogFile << "apply_momentum_cuts_2p = " << BoolToString(apply_momentum_cuts_2p) << "\n";
    myLogFile << "apply_momentum_cuts_1n1p = " << BoolToString(apply_momentum_cuts_1n1p) << "\n\n";

    myLogFile << "apply_neutron_Beta_Fit = " << BoolToString(apply_neutron_Beta_Fit) << "\n\n";
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="cuts">

    //<editor-fold desc="#Photo-electrons (Nphe) cuts (electrons only, FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "#Photo-electrons (Nphe) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Nphe_lower_cut:" << std::setw(59) << Nphe_cuts_FD.GetLowerCut() << "\n";
    myLogFile << "Nphe_upper_cut:" << std::setw(59) << Nphe_cuts_FD.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Chi2 cuts">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Chi2 cuts\n";
    myLogFile << "===========================================================================\n\n";

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

    //</editor-fold>

    //<editor-fold desc="Vertex cuts">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Vertex cuts\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Vz_cut.GetLowerCut() = " << Vz_cut.GetLowerCut() << "\n";
    myLogFile << "Vz_cut.GetUpperCut() = " << Vz_cut.GetUpperCut() << "\n";
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) cuts (electrons only, FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Sampling Fraction (SF) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "SF_1e_upper_cut = " << clasAna.getEcalSFUpperCut() << "\n";
    myLogFile << "SF_1e_lower_cut = " << clasAna.getEcalSFLowerCut() << "\n";
    //</editor-fold>

    //<editor-fold desc="ECAL fiducial cuts (electrons only, FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "ECAL fiducial cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "fiducial_cut_Lv = " << clasAna.getEcalEdgeCuts() << "\n";
    myLogFile << "fiducial_cut_Lw = " << clasAna.getEcalEdgeCuts() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum thresholds {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "e_mom_th = " << e_mom_th.GetLowerCut() << ", " << e_mom_th.GetUpperCut() << "}\n";
    myLogFile << "n_mom_th = " << n_mom_th.GetLowerCut() << ", " << n_mom_th.GetUpperCut() << "}\n";
    myLogFile << "ph_mom_th = " << ph_mom_th.GetLowerCut() << ", " << ph_mom_th.GetUpperCut() << "}\n";
    myLogFile << "p_mom_th = " << p_mom_th.GetLowerCut() << ", " << p_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pip_mom_th = " << pip_mom_th.GetLowerCut() << ", " << pip_mom_th.GetUpperCut() << "}\n";
    myLogFile << "pim_mom_th = " << pim_mom_th.GetLowerCut() << ", " << pim_mom_th.GetUpperCut() << "}\n";
    //</editor-fold>

    //<editor-fold desc="Momentum cuts">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Momentum cuts {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "\n-- Momentum cuts (1n, FD only) --------------------------------------------" << "\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECAL):\t\t{" << n_momentum_cuts_ABF_FD_n_from_ph.GetLowerCut() << ", " << n_momentum_cuts_ABF_FD_n_from_ph.GetUpperCut()
              << "}\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECAL apprax):\t{" << n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetLowerCut() << ", "
              << n_momentum_cuts_ABF_FD_n_from_ph_apprax.GetUpperCut() << "}\n";
    myLogFile << "Neutrons (n_momentum_cuts - noPCAL):\t{" << n_momentum_cuts_ABF_FD_noPCAL.GetLowerCut() << ", " << n_momentum_cuts_ABF_FD_noPCAL.GetUpperCut() << "}\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECIN):\t\t{" << n_momentum_cuts_ABF_FD_noPCAL_wECIN.GetLowerCut() << ", "
              << n_momentum_cuts_ABF_FD_noPCAL_wECIN.GetUpperCut() << "}\n";
    myLogFile << "Neutrons (n_momentum_cuts - ECOUT):\t\t{" << n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT.GetLowerCut() << ", "
              << n_momentum_cuts_ABF_FD_noPCAL_noECIN_wECOUT.GetUpperCut() << "}\n\n";
    //</editor-fold>

    //<editor-fold desc="TL Momentum thresholds">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "TL Momentum thresholds {min, max}\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "TL_e_mom_cuts:\t{" << TL_e_mom_cuts.GetLowerCut() << ", " << TL_e_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_n_mom_cuts:\t{" << TL_n_mom_cuts.GetLowerCut() << ", " << TL_n_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_p_mom_cuts:\t{" << TL_p_mom_cuts.GetLowerCut() << ", " << TL_p_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pip_mom_cuts:\t{" << TL_pip_mom_cuts.GetLowerCut() << ", " << TL_pip_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pim_mom_cuts:\t{" << TL_pim_mom_cuts.GetLowerCut() << ", " << TL_pim_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_pi0_mom_cuts:\t{" << TL_pi0_mom_cuts.GetLowerCut() << ", " << TL_pi0_mom_cuts.GetUpperCut() << "}\n";
    myLogFile << "TL_ph_mom_cuts:\t{" << TL_ph_mom_cuts.GetLowerCut() << ", " << TL_ph_mom_cuts.GetUpperCut() << "}\n\n";
    //</editor-fold>

    //<editor-fold desc="dV cuts (CD & FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "dV cuts (CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "dVz_cuts = " << dVz_cuts.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Beta cut (1n, FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Beta cut (1n, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Beta_cut.GetUpperCut() (1 fit std) = " << Beta_cut.GetUpperCut() << "\n";
    myLogFile << "Beta_cut.GetMean() = " << Beta_cut.GetMean() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Nucleon theta cut (1p & 1n, FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Nucleon theta cut (1p & 1n, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Theta_nuc_cut.GetUpperCut() = " << Theta_nuc_cut.GetUpperCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Neutron veto cut (1n & 1n1p, FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Neutron veto cut (1n & 1n1p, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "Neutron_veto_cut.GetLowerCut() = " << Neutron_veto_cut.GetLowerCut() << "\n\n";
    //</editor-fold>

    //<editor-fold desc="Ghost tracks handling (2p, CD & FD)">
    myLogFile << "\n===========================================================================\n";
    myLogFile << "Ghost tracks handling (2p, CD & FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "p1_Theta_p_cuts_2p = " << p1_Theta_p_cuts_2p.GetUpperCut() << "\n";
    myLogFile << "p1_Theta_p_cuts_2p mean = " << p1_Theta_p_cuts_2p.GetMean() << "\n";
    myLogFile << "p2_Theta_p_cuts_2p = " << p2_Theta_p_cuts_2p.GetUpperCut() << "\n";
    myLogFile << "p2_Theta_p_cuts_2p mean = " << p2_Theta_p_cuts_2p.GetMean() << "\n";
    myLogFile << "phi_p1_p2_diff_cuts_2p = " << phi_p1_p2_diff_cuts_2p.GetUpperCut() << "\n\n";
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Event counts">
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

    myLogFile << "-- 1e1p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e1p:\t\t\t\t" << num_of_events_with_1e1p << "\n\n";

    myLogFile << "-- 1e2p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2p:\t\t\t\t" << num_of_events_with_1e2p << "\n\n";

    myLogFile << "-- 2p event counts --------------------------------------------------------\n";
    myLogFile << "#(events) 2p:\t\t\t\t\t" << num_of_events_2p << "\n\n";

    myLogFile << "-- Event counts -----------------------------------------------------------\n";
    myLogFile << "num_of_events_1e1p_all:\t\t\t\t" << num_of_events_1e1p_all << "\n";
    myLogFile << "num_of_events_1p_inFD:\t\t\t\t" << num_of_events_1p_inFD << "\n";
    myLogFile << "num_of_events_1n_inFD:\t\t\t\t" << num_of_events_1n_inFD << "\n";
    myLogFile << "num_of_events_1n_inFD_AV:\t\t" << num_of_events_1n_inFD_AV << "\n";
//    myLogFile << "num_of_events_1e1n1p_wFakeNeut:\t\t" << num_of_events_1e1n1p_wFakeNeut << "\n";
    myLogFile << "num_of_events_1e2p_all:\t\t\t\t" << num_of_events_1e2p_all << "\n";
    myLogFile << "num_of_events_1e2p_all_woFDphotons:\t" << num_of_events_1e2p_all_woFDphotons << "\n";
    myLogFile << "num_of_events_2p:\t\t\t\t\t" << num_of_events_2p << "\n\n\n";
    //</editor-fold>

    //<editor-fold desc="content of FittedPIDCuts.par file">
    if (apply_cuts == true && apply_chi2_cuts_1e_cut == false) {
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
            myLogFile << "pid_cuts" << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":" << chi2cuts[i].GetUpperCut() << ":"
                      << chi2cuts[i].GetRegion() << "\n";
//            myLogFile << "pid_cuts_" << chi2cuts[i].GetRegion() << "\t\t" << chi2cuts[i].GetPartPDG() << ":" << chi2cuts[i].Cuts.at(0) << ":"
//                      << chi2cuts[i].GetUpperCut() << "\n";
        }
    }
    //</editor-fold>

    myLogFile.close();

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

    cout << "-- 1e1p event counts ------------------------------------------------------\n";
    cout << "#(events) w/ 1e1p:\t\t\t" << num_of_events_with_1e1p << "\n\n";

    cout << "-- 1e2p event counts ------------------------------------------------------\n";
    cout << "#(events) w/ 1e2p:\t\t\t" << num_of_events_with_1e2p << "\n\n";

    cout << "-- 2p event counts --------------------------------------------------------\n";
    cout << "#(events) 2p:\t\t\t\t" << num_of_events_2p << "\n\n";

    cout << "-- Event counts -----------------------------------------------------------\n";
    cout << "num_of_events_1e1p_all:\t\t\t" << num_of_events_1e1p_all << "\n";
    cout << "num_of_events_1p_inFD:\t\t\t" << num_of_events_1p_inFD << "\n";
    cout << "num_of_events_1n_inFD:\t\t\t" << num_of_events_1n_inFD << "\n";
    cout << "num_of_events_1n_inFD_AV:\t\t" << num_of_events_1n_inFD_AV << "\n";
    cout << "num_of_events_1e1n1p_wFakeNeut:\t\t" << num_of_events_1e1n1p_wFakeNeut << "\n";
    cout << "num_of_events_1e2p_all:\t\t\t" << num_of_events_1e2p_all << "\n";
    cout << "num_of_events_1e2p_all_woFDphotons:\t" << num_of_events_1e2p_all_woFDphotons << "\n";
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

    //</editor-fold>

}