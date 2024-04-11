//
// Created by alons on 23/01/2024.
//

#ifndef RECOANALYSER_H
#define RECOANALYSER_H

#include "../../setup/codeSetup.h"
//#include "TempInclude/DetectorSimulationAnalyser_Histograms_Temp.C" //TODO: move to a class!
#include "../AMaps/AMaps.cpp"
#include "../clas12ana/clas12ana.h"
#include "../DEfficiency/DEfficiency.cpp"
#include "../DSCuts/DSCuts.h"
#include "../hPlots/hPlot1D.cpp"
#include "../hPlots/hPlot2D.cpp"
#include "../MomentumResolution/MomentumResolution.cpp"
#include "../ParticleID/ParticleID.cpp"
#include "../Settings/Settings.cpp"
#include "../TLCuts/TLCuts.cpp"
#include "../../functions/AngleCalc/GetBinFromAng.h"
#include "../../functions/FitFunctions/BetaFit.h"
#include "../../functions/FitFunctions/BetaFitApprax.h"
#include "../../functions/DrawAndSaveFunctions/DrawAndSaveACorrPlots.h"
#include "../../functions/DrawAndSaveFunctions/DrawAndSaveEfficiencyPlots.h"
#include "../../functions/DrawAndSaveFunctions/DrawAndSaveFSRatio.h"
#include "../../functions/FillByInt.h"
#include "../../functions/GetPi0MomTh.h"
#include "../../functions/AngleCalc/CalcdPhi.h"
#include "../../functions/StackPlot3.h"
#include "../../functions/StackPlot4.h"
#include "../../functions/TLKinCutsCheck.h"

#include "HipoChain.h"
#include "clas12reader.h"

using namespace std;
using namespace clas12;

class RecoAnalyser {
private:
    cout << "\n\n===========================================================================\n";
    cout << "\t\t\tDetector simulation analyser\n";
    cout << "===========================================================================\n\n";

    string AnalyserVersion = "Version 1.9";

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code setup                                                                               //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Code setup">
    Settings settings;
    ParticleID pid;

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
    //TODO: change VaryingSampleName to simulation of a 4-foil!
    const string VaryingSampleName = Experiment.GetVaryingSampleName();                             // Get VaryingSampleName (configured from SampleName) - for data runs!
    const double beamE = Experiment.GetBeanEnergy();                                              // Configure beam energy from SampleName
    const string Target = Experiment.GetTargetElement();                                          // Configure target (element) from SampleName
    const int TargetPDG = Experiment.GetTargetElementPDG();                                       // Configure target PDG from SampleName
    const bool isLocal = Experiment.SLocal();
    const bool isMC = Experiment.SSample();
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

    /* Final states to analyse (1N & 2N) */
    const bool calculate_1p = true, calculate_1n = true;
    const bool calculate_2p = true, calculate_pFDpCD = true, calculate_nFDpCD = true;

    /* Truth level calculation setup */
    bool calculate_truth_level = true; // TL master ON/OFF switch
    bool TL_plots_only_for_NC = false; // TL plots only AFTER beta fit
    bool fill_TL_plots = true;
    bool ZoomIn_On_mom_th_plots = false; // momentum th. efficiencies with zoomin
    bool Eff_calc_with_one_reco_electron = true;
    bool Calc_inc_p_eff_with_extended_theta = false;
    bool Rec_wTL_ES = false; // Force TL event selection on reco. plots

    const bool limless_mom_eff_plots = false;

    /* FD neutrals settings */
    const bool Enable_FD_photons = false; // keep as false to decrease RES & DIS
    const bool Enable_FD_neutrons = true; // keep as false to increse eff. plots
    const bool Count_FD_neurton_and_photon_hits = true;

    //TODO: add this switch to event selection variables!
    const bool ES_by_leading_FDneutron = true;

    /* Acceptance maps setup */
    bool Generate_AMaps = false;             // Generate acceptance maps
    //TODO: UPDATE AMaps loading constructor electron histogram's number of bins
    bool AMaps_calc_with_one_reco_electron = true;
    bool reformat_e_bins = false;
    bool varying_P_e_bins = true;
    bool Electron_single_slice_test = false; // keep as false for normal runs!
    bool Nucleon_single_slice_test = false;  // keep as false for normal runs!
    vector<int> TestSlices = {1, 1, 1};      // {ElectronTestSlice, ProtonTestSlice, NeutronTestSlice}

    /* Neutron resolution setup */
    //TODO: align neutron and proton momRes calculations!
    bool plot_and_fit_MomRes = true; // Generate nRes plots
    bool Calculate_momResS2 = true; // Calculate momResS2 variables
    const double DeltaSlices = 0.05;
    const bool VaryingDelta = true; // 1st momResS1 w/ VaryingDelta = false
    const bool ForceSmallpResLimits = false; // 1st momResS1 w/ VaryingDelta = false
    const string SmearMode = "pol1_wKC";
    const string CorrMode = "pol3_wKC";
    bool Run_with_momResS2 = false; // Smear w/ momResS2 & correct w/ momResS1
    bool momRes_test = false; // false by default
    /*
    MomRes run order guide:
    1. momResS1 calculation 1:
                           1a:  VaryingDelta = false  , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
                           1b:  VaryingDelta = true , plot_and_fit_MomRes = true  , Calculate_momResS2 = false , Run_with_momResS2 = false
    3. momResS2 calculation:    VaryingDelta = true , plot_and_fit_MomRes = true  , Calculate_momResS2 = true  , Run_with_momResS2 = false
    4. momResS2 run:            VaryingDelta = true , plot_and_fit_MomRes = false , Calculate_momResS2 = false , Run_with_momResS2 = true
    */
    //</editor-fold>

// ======================================================================================================================================================================
// Cut setup
// ======================================================================================================================================================================

// Cut setup ---------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Cuts setup">
    /* Settings that allow to disable/enable every cut individually */

    // clas12ana cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
    bool apply_cuts = true; // master ON/OFF switch for applying cuts
    bool clas12ana_particles = true; //TODO: move form here!
    bool only_preselection_cuts = false; // keep as false for regular runs!
    bool only_electron_quality_cuts = false; // keep as false for regular runs!

    /* Preselection cuts (event cuts) */
    bool apply_preselection_cuts = true; // master ON/OFF switch for preselection cuts
    bool apply_Vz_cuts = true, apply_dVz_cuts = true; // Vertex cuts
    bool apply_DC_fiducial_cuts = true; // DC fiducial (edge) cuts

    /* Electron quality cuts */
    bool apply_electron_quality_cuts = true; // master ON/OFF switch for eQC
    bool apply_Nphe_cut = true; // Number of photo-electrons in HTCC cut
    bool apply_ECAL_SF_cuts = true; // SF cut on both E_deb AND P_e
    bool apply_ECAL_P_cuts = false; // SF cut on P_e (keep as false for now!)
    bool apply_ECAL_fiducial_cuts = true; // ECAL edge cuts for other charged particles
    bool apply_Electron_beta_cut = true; // Electron beta cut

    /* Chi2 cuts (= PID cuts) */
    bool apply_chi2_cuts_1e_cut = true;

    // My analysis cuts ---------------------------------------------------------------------------------------------------------------------------------------------------
    /* Nucleon cuts */
    bool apply_nucleon_cuts = true; // set as true to get good protons and calculate upper neutron momentum th.

    /* Physical cuts */
    bool apply_nucleon_physical_cuts = true; // nucleon physical cuts master
    //TODO: automate adding upper mom. th. to nucleon cuts (for nRes calc)
    bool apply_nBeta_fit_cuts = true; // apply neutron upper mom. th.
    bool apply_fiducial_cuts = false;
    bool apply_kinematical_cuts = false;
    bool apply_kinematical_weights = false;
    bool apply_nucleon_SmearAndCorr = false;

    //<editor-fold desc="Custom cuts naming & print out execution variables">

    //<editor-fold desc="Auto-disable variables">
    if (only_preselection_cuts || only_electron_quality_cuts) { apply_cuts = false; }

    if (!apply_cuts) {
        if (!only_preselection_cuts) { apply_preselection_cuts = false; }
        if (!only_electron_quality_cuts) { apply_electron_quality_cuts = false; }

        apply_chi2_cuts_1e_cut = apply_nucleon_cuts = false;
    }

    if (!apply_preselection_cuts) { apply_Vz_cuts = apply_dVz_cuts = apply_DC_fiducial_cuts = false; }

    if (!apply_electron_quality_cuts) { apply_Nphe_cut = apply_ECAL_SF_cuts = apply_ECAL_P_cuts = apply_ECAL_fiducial_cuts = apply_Electron_beta_cut = false; }

    if (!apply_chi2_cuts_1e_cut) { apply_nucleon_cuts = false; }

    if (!apply_nucleon_cuts) { apply_nucleon_physical_cuts = false; }

    if (!apply_nucleon_physical_cuts) {
        apply_nBeta_fit_cuts = apply_fiducial_cuts = apply_kinematical_cuts = apply_kinematical_weights = apply_nucleon_SmearAndCorr = false;
    } else {
        if (Calculate_momResS2) { apply_nucleon_SmearAndCorr = true; }
    }

    if (Generate_AMaps) { apply_fiducial_cuts = false; }

    if (!VaryingDelta) { apply_nucleon_SmearAndCorr = false; }

    if (isData) { // no TL calculation, AMap,WMap generation nor nRes calculation when running on data
        calculate_truth_level = Generate_AMaps = plot_and_fit_MomRes = momRes_test = false;
    }

    if (!calculate_truth_level) { AMaps_calc_with_one_reco_electron = fill_TL_plots = Rec_wTL_ES = false; }

    if (Rec_wTL_ES) {
        /* if Rec_wTL_ES = true, there are no momentum thresholds, and we get an infinite loop in the nRes slice calculations!
           Additionally, there is no need to calculate the resolution and efficiency in the same time! */
        plot_and_fit_MomRes = false;
    }

    if (!plot_and_fit_MomRes) { Calculate_momResS2 = false; }

    if ((Calculate_momResS2 && Run_with_momResS2) // Don't run calculate momResS2 and run on it at the same time
    || (Calculate_momResS2 && !VaryingDelta) // Don't run calculate momResS2 and small momentum slices at the same time
    ) {
        cout << "\n\nmomRes order error! Exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //<editor-fold desc="Custom cuts naming">

    /* Save plots to custom-named folders, to allow multi-sample runs at once. */
    const bool custom_cuts_naming = true;
    settings.SetCustomCutsNaming(custom_cuts_naming);
    settings.ConfigureStatuses(apply_cuts, clas12ana_particles, only_preselection_cuts, apply_chi2_cuts_1e_cut, only_electron_quality_cuts,
            apply_nucleon_cuts, Enable_FD_photons, apply_nucleon_SmearAndCorr, apply_kinematical_cuts,
            apply_kinematical_weights, apply_fiducial_cuts, Generate_AMaps, plot_and_fit_MomRes, VaryingDelta,
            Calculate_momResS2, Run_with_momResS2, momRes_test, Rec_wTL_ES, ZoomIn_On_mom_th_plots);
    settings.SetPaths(WorkingDirectory, SampleName, plots_path, apply_cuts, apply_chi2_cuts_1e_cut, apply_nucleon_cuts);
    settings.GetPlotsPath(plots_path);
    settings.GetPlotsLogSaveDirectory(plots_log_save_Directory);
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
    cout << "VaryingSampleName:\t" << VaryingSampleName << "\n";
    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
    cout << "Beam Energy:\t\t" << beamE << " [GeV]\n\n\n\n";
    //</editor-fold>

    //<editor-fold desc="Cuts output">
    /* Print out the cuts within the run (for self-observation) */
    if (!apply_cuts) {
        cout << "Cuts are disabled:\n";
    } else {
        cout << "Cuts are enabled:\n";
    }

    cout << "apply_cuts:\t\t\t" << BoolToString(apply_cuts) << "\n";
    cout << "clas12ana_particles:\t\t" << BoolToString(clas12ana_particles) << "\n"; //TODO: move form here!
    cout << "only_preselection_cuts:\t\t" << BoolToString(only_preselection_cuts) << "\n";
    cout << "only_electron_quality_cuts:\t" << BoolToString(only_electron_quality_cuts) << "\n\n";

    cout << "apply_preselection_cuts:\t" << BoolToString(apply_preselection_cuts) << "\n";
    cout << "apply_Vz_cuts:\t\t\t" << BoolToString(apply_Vz_cuts) << "\n";
    cout << "apply_dVz_cuts:\t\t\t" << BoolToString(apply_dVz_cuts) << "\n";
    cout << "apply_DC_fiducial_cuts:\t\t" << BoolToString(apply_DC_fiducial_cuts) << "\n\n";

    cout << "apply_electron_quality_cuts:\t" << BoolToString(apply_electron_quality_cuts) << "\n";
    cout << "apply_Nphe_cut:\t\t\t" << BoolToString(apply_Nphe_cut) << "\n";
    cout << "apply_ECAL_SF_cuts:\t\t" << BoolToString(apply_ECAL_SF_cuts) << "\n";
    cout << "apply_ECAL_P_cuts:\t\t" << BoolToString(apply_ECAL_P_cuts) << "\n";
    cout << "apply_ECAL_fiducial_cuts:\t" << BoolToString(apply_ECAL_fiducial_cuts) << "\n";
    cout << "apply_Electron_beta_cut:\t" << BoolToString(apply_Electron_beta_cut) << "\n\n";

    cout << "apply_chi2_cuts_1e_cut:\t\t" << BoolToString(apply_chi2_cuts_1e_cut) << "\n";

    cout << "apply_nucleon_cuts:\t\t" << BoolToString(apply_nucleon_cuts) << "\n\n";

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

    //<editor-fold desc="Deuteron PID cuts">
    /* Deuteron pid cuts. Not really applied in our analysis. */
    DSCuts Chi2_deuteron_cuts_CD = DSCuts("Chi2", "CD", "deuteron", "1e cut", 0, -9999, 9999);
    DSCuts Chi2_deuteron_cuts_FD = DSCuts("Chi2", "FD", "deuteron", "1e cut", 0, -9999, 9999);
    //</editor-fold>

    /* Vertex cuts */
    DSCuts Vz_cut = Experiment.GetVz_cuts(), Vz_cut_FD = Experiment.GetVz_cuts_FD(), Vz_cut_CD = Experiment.GetVz_cuts_CD();
    DSCuts dVz_cuts = Experiment.GetdVz_cuts(), dVz_cuts_FD = Experiment.GetdVz_cuts_FD(), dVz_cuts_CD = Experiment.GetdVz_cuts_CD();

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
    DSCuts SF_cuts;
//    DSCuts P_cuts;

    /* PCAL edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts PCAL_edge_cuts;

    /* DC edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts DC_edge_cuts;

    //<editor-fold desc="Reco particles momentum thresholds">
    /* Momentum thresholds (declarations) */
    DSCuts e_mom_th, p_mom_th, n_mom_th, pip_mom_th, pim_mom_th, ph_mom_th; // Momentum thresholds for PID
    DSCuts no_p_mom_th; // (no) momentum thresholds for (e,e'Xp)Y efficiency

    if (Rec_wTL_ES || limless_mom_eff_plots) {
        /* If we enforce TL cuts, don't use momentum thresholds on nucleons. */
        e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
        p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
        no_p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
        n_mom_th = DSCuts("Momentum_th", "", "Neutrons", "", 0, -9999, 9999);
        pip_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
        pim_mom_th = DSCuts("Momentum_th", "", "Piplus", "", 0, 0.2, 9999);
        ph_mom_th = DSCuts("Momentum_th", "", "Photons", "", 0, 0.3, 9999);
    } else {
        /* If we don't enforce TL cuts, use momentum thresholds on nucleons. */
        e_mom_th = DSCuts("Momentum_th", "", "Electron", "", 0, -9999, 9999);
        p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, 0.4, 9999);
        no_p_mom_th = DSCuts("Momentum_th", "", "Proton", "", 0, -9999, 9999);
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
    DSCuts Beta_max_cut_ABF_FD_n_from_ph, Beta_max_cut_ABF_FD_n_from_ph_apprax;

    /* Neutron momentum cuts (1n & nFDpCD, FD only) */
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph; // ABF = After Beta Fit. These are momentum cuts to logged to the fitted cuts file.
    DSCuts n_momentum_cuts_ABF_FD_n_from_ph_apprax; // Approximated max. momentum, obtained by taking Beta=1, such that deltaBeta/Beta=deltaBeta.

    /* Truth-level momentum cuts */
    //TODO: remove pion mom. th. separation by CD and FD. It's useless (according to Adi)
    DSCuts TL_e_mom_cuts = DSCuts("Momentum", "", "Electron", "", 0, e_mom_th.GetLowerCut(), e_mom_th.GetUpperCut());
    DSCuts TL_n_mom_cuts = DSCuts("Momentum", "", "Neutrons", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    DSCuts TL_p_mom_cuts = DSCuts("Momentum", "", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    DSCuts TL_pFD_mom_cuts = DSCuts("Momentum", "FD", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    DSCuts TL_pCD_mom_cuts = DSCuts("Momentum", "CD", "Proton", "", 0, p_mom_th.GetLowerCut(), p_mom_th.GetUpperCut());
    DSCuts TL_pip_mom_cuts = DSCuts("Momentum", "", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pipFD_mom_cuts = DSCuts("Momentum", "FD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pipCD_mom_cuts = DSCuts("Momentum", "CD", "Piplus", "", 0, pip_mom_th.GetLowerCut(), pip_mom_th.GetUpperCut());
    DSCuts TL_pim_mom_cuts = DSCuts("Momentum", "", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts TL_pimFD_mom_cuts = DSCuts("Momentum", "FD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
    DSCuts TL_pimCD_mom_cuts = DSCuts("Momentum", "CD", "Piminus", "", 0, pim_mom_th.GetLowerCut(), pim_mom_th.GetUpperCut());
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

    DSCuts Theta_L_cuts_pFDpCD = DSCuts("Theta_L leading", "", "Proton", "pFDpCD", Theta_p1_cuts_2p.GetMean(), -9999,
                                        Theta_p1_cuts_2p.GetUpperCut());
    DSCuts Theta_R_cuts_pFDpCD = DSCuts("Theta_R recoil", "", "Proton", "pFDpCD", Theta_p2_cuts_2p.GetMean(), -9999,
                                        Theta_p2_cuts_2p.GetUpperCut());
    DSCuts dphi_pFD_pCD_pFDpCD = DSCuts("dPhi_pFD_pCD", "", "Proton", "pFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    DSCuts Theta_L_cuts_nFDpCD = DSCuts("Theta_L", "", "", "nFDpCD", Theta_p1_cuts_2p.GetMean(), -9999, Theta_p1_cuts_2p.GetUpperCut());
    DSCuts Theta_R_cuts_nFDpCD = DSCuts("Theta_R", "", "", "nFDpCD", Theta_p2_cuts_2p.GetMean(), -9999, Theta_p2_cuts_2p.GetUpperCut());
    DSCuts dphi_L_R_nFDpCD = DSCuts("dphi_L_R", "", "", "nFDpCD", dphi_p1_p2_2p.GetMean(), -9999, dphi_p1_p2_2p.GetUpperCut());

    /* reco. kinematic cuts (based on nucleons' efficiency) */
    DSCuts FD_nucleon_theta_cut = DSCuts("FD Nucleon theta cut", "FD", "", "", 0, 0, 32.);
    DSCuts Nucleon_momentum_cut = DSCuts("Nucleon momentum cut", "FD", "", "", 0, n_mom_th.GetLowerCut(), n_mom_th.GetUpperCut());
    DSCuts FD_nucleon_momentum_cut = DSCuts("FD nucleon momentum cut", "FD", "", "pFDpCD & nFDpCD", 0, 1.,
                                            3.); // new upper cut, following Larry meeting (10/08/23)
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
    string listName = plots_path + "/" + SampleName + plots_file_type;
    const char *TListName = listName.c_str();
    //</editor-fold>

//  Creating directories ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Creating directories">
    /* Code for creating directories.
     * Directory creation is done in the Directories class.
     * Added for the case that plots out folder does not exist and for organization.
     * All cut plots are separate from the analysis plots, and withing the 01_Cuts_plots folder. */
    cout << "\nCreating plot directories...";

    string Plots_Folder = plots_path;
    Directories directories = Directories(Plots_Folder);

    cout << " done.\n\n";
    //</editor-fold>

// Plot selector --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Plot selector">
    /* Here are boolean variables used to turn ON/OFF the different plots of the code.
       Plot_selector_master must remain true, set it OFF only for debugging. */

    //<editor-fold desc="Plot selector variable declarations">
    /* Master plots variable */
    bool Plot_selector_master; // Master plot selector for analysis

    /* Cut variable plots */
    bool Cut_plots_master; // Master cut plots selector
    bool Nphe_plots, Chi2_plots, Vertex_plots, SF_plots, fiducial_plots, Momentum_plots;

    /* Beta plots */
    bool W_plots;

    /* Beta plots */
    bool Beta_plots;
    bool Beta_vs_P_plots;

    /* Angle plots */
    bool Angle_plots_master; // Master angle plots selector
    bool Theta_e_plots, Phi_e_plots;

    /* Momentum transfer plots */
    bool Momentum_transfer_plots;

    /* E_e plots */
    bool E_e_plots;

    /* ET plots */
    bool ETrans_plots_master; // Master ET plots selector
    bool ETrans_all_plots, ETrans_All_Int_plots, ETrans_QEL_plots, ETrans_MEC_plots, ETrans_RES_plots, ETrans_DIS_plots;

    /* Ecal plots */
    bool Ecal_plots;

    /* Transverse variables plots */
    bool TKI_plots;

    /* ToF plots */
    bool ToF_plots;

    /* Efficiency plots */
    bool Efficiency_plots;
    bool TL_after_Acceptance_Maps_plots;

    /* Resolution plots */
    bool AMaps_plots;

    /* Resolution plots */
    bool Resolution_plots;

    /* Multiplicity plots */
    bool Multiplicity_plots;

    /* Final state ratio plots */
    bool FSR_1D_plots, FSR_2D_plots; // FSR_2D_plots is disabled below if HipoChainLength is 2 or lower
    //</editor-fold>

    bool TestRun = false; // set as false for a full run

    if (!TestRun) {

        //<editor-fold desc="Plot everything (full run)">
        /* Master plots variable */
        Plot_selector_master = true; // Master plot selector for analysis

        /* Cut variable plots */
        Cut_plots_master = true; // Master cut plots selector
        Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true, Momentum_plots = true;

        /* W plots */
        W_plots = true;

        /* Beta plots */
        Beta_plots = true;
        Beta_vs_P_plots = true;

        /* Angle plots */
        Angle_plots_master = true; // Master angle plots selector
        Theta_e_plots = true, Phi_e_plots = true;

        /* Momentum transfer plots */
        Momentum_transfer_plots = true;

        /* E_e plots */
        E_e_plots = true;

        /* ET plots */
        ETrans_plots_master = true; // Master ET plots selector
        ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

        /* Ecal plots */
        Ecal_plots = true;

        /* Transverse variables plots */
        TKI_plots = true;

        /* ToF plots */
        ToF_plots = false;

        /* Efficiency plots */
        Efficiency_plots = true;
        TL_after_Acceptance_Maps_plots = true;

        /* Resolution plots */
        AMaps_plots = true;

        /* Resolution plots */
        Resolution_plots = true;

        /* Multiplicity plots */
        Multiplicity_plots = true;

        /* Final state ratio plots */
        FSR_1D_plots = true;
        FSR_2D_plots = true; // disabled below if HipoChainLength is less than 100
        //</editor-fold>

    } else {

        //<editor-fold desc="Selected plots (test run)">
        /* Master plots variable */
        Plot_selector_master = true; // Master plot selector for analysis

        /* Cut variable plots */
        Cut_plots_master = true; // Master cut plots selector
//     Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true;
        Nphe_plots = false, Chi2_plots = false, Vertex_plots = false, SF_plots = false, fiducial_plots = false;

//        Momentum_plots = true;
        Momentum_plots = false;

        /* W plots */
//     W_plots = true;
        W_plots = false;

        /* Beta plots */
//     Beta_plots = true;
        Beta_plots = false;
//     Beta_vs_P_plots = true;
        Beta_vs_P_plots = false;

        /* Angle plots */
//        Angle_plots_master = true; // Master angle plots selector
//        Theta_e_plots = true, Phi_e_plots = true;
        Angle_plots_master = false; // Master angle plots selector
        Theta_e_plots = false, Phi_e_plots = false;

        /* Momentum transfer plots */
//     Momentum_transfer_plots = true;
        Momentum_transfer_plots = false;

        /* E_e plots */
//     E_e_plots = true;
        E_e_plots = false;

        /* ET plots */
//     ETrans_plots_master = true; // Master ET plots selector
        ETrans_plots_master = false; // Master ET plots selector
        ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;

        /* Ecal plots */
//     Ecal_plots = true;
        Ecal_plots = false;

        /* Transverse variables plots */
//     TKI_plots = true;
        TKI_plots = false;

        /* ToF plots */
//     ToF_plots = true;
        ToF_plots = false;

        /* Efficiency plots */
//        Efficiency_plots = true;
        Efficiency_plots = false;
//        TL_after_Acceptance_Maps_plots = true;
        TL_after_Acceptance_Maps_plots = false;

        /* Resolution plots */
//        AMaps_plots = true;
        AMaps_plots = false;

        /* Resolution plots */
        Resolution_plots = true;
//        Resolution_plots = false;

        /* Multiplicity plots */
//        Multiplicity_plots = true;
        Multiplicity_plots = false;

        /* Final state ratio plots */
//        FSR_1D_plots = true;
//        FSR_2D_plots = true; // disabled below if HipoChainLength is 2 or lower
        FSR_1D_plots = false;
        FSR_2D_plots = false; // disabled below if HipoChainLength is 2 or lower
        //</editor-fold>/

    }

    /* Other setup variables */
    bool wider_margin = true;
    bool debug_plots = true; // Print out clas12ana debugging plots

    bool Log_scale_Vx_plots = false, Log_scale_Vy_plots = false, Log_scale_Vz_plots = true;
    bool Log_scale_dVx_plots = false, Log_scale_dVy_plots = false, Log_scale_dVz_plots = true;

    //<editor-fold desc="Auto-disable plot selector variables">
    if (!Plot_selector_master) {
        Cut_plots_master = W_plots = Beta_plots = Beta_vs_P_plots = Angle_plots_master = Momentum_transfer_plots = E_e_plots = ETrans_plots_master = Ecal_plots = false;
        TKI_plots = ToF_plots = Efficiency_plots = AMaps_plots = Resolution_plots = Multiplicity_plots = false;
        FSR_1D_plots = FSR_2D_plots = false;
    }

    if (!Cut_plots_master) { Nphe_plots = Chi2_plots = Vertex_plots = SF_plots = fiducial_plots = Momentum_plots = false; }

    if (!Angle_plots_master) { Theta_e_plots = Phi_e_plots = false; }

    if (!ETrans_plots_master) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }

    if (!fill_TL_plots) { Efficiency_plots = TL_after_Acceptance_Maps_plots = false; }

    if (!Generate_AMaps) { AMaps_plots = false; }

    if (!apply_nucleon_cuts || (Electron_single_slice_test || Nucleon_single_slice_test)) { FSR_1D_plots = FSR_2D_plots = false; }

//    if (!plot_and_fit_MomRes) { Resolution_plots = false; }
    //</editor-fold>

    //</editor-fold>

// Normalization setup -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization setup">
    /* Here are boolean variables used to turn ON/OFF the different plot normalizations of the code.
     * Enable of presentations only, since event count is important otherwise. */

    bool normalize_master = false;

    bool norm_Nphe_plots = false, norm_Chi2_plots = false, norm_Vertex_plots = false, norm_SF_plots = false, norm_Fiducial_plots = false, norm_Momentum_plots = false;

    bool norm_W_plots = false, norm_Beta_plots = false, norm_Angle_plots_master = false, norm_Momentum_transfer_plots = false, norm_E_e_plots = false, norm_ET_plots = false;
    bool norm_Ecal_plots = false, norm_TKI_plots = false;

    //<editor-fold desc="Auto-disable plot normalization variables">
    if (!normalize_master) { // Disable all normalizations if normalize_master == false
        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = norm_Momentum_plots = false;
        norm_Angle_plots_master = norm_Momentum_transfer_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = norm_TKI_plots = false;
    }
    //</editor-fold>

    //</editor-fold>

// Delete setup ------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Delete setup">
    /* Clear files from previous runs (to prevent mix fo plots from different codes). */

    const bool delete_png_files = true, delete_root_files = true, delete_txt_files = true;

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
    int numTH1Dbins = 50;
    int numTH2Dbins = 65;

    /* Momentum plots */
    int numTH2Dbins_Mom_Plots = 65; // To be changed if apply_kinematical_cuts = true

    /* Beta plots */
    int numTH1Dbins_Beta_Plots = 65;
    int numTH2Dbins_Beta_Plots = 65 * 2;
//    int numTH1Dbins_Beta_Plots = 65;
//    int numTH2Dbins_Beta_Plots = 65;

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
    int numTH2Dbins_Momentum_transfer_plots = 65;

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
    int numTH2Dbins_nRes_Plots = numTH2Dbins_Nucleon_Ang_Plots * 4 / 3;
    //</editor-fold>

    //<editor-fold desc="Histogram limits">
    /* Nphe boundaries */
    const double Nphe_boundary = 40;

    /* Chi2 boundaries */
    double Chi2_boundary = 20;
//    double Chi2_boundary = 30;

    if (apply_cuts) { Chi2_boundary = 9; }

    /* Vertex boundaries */
    double Vertex_boundary = 20., Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;
    double Vertex_boundary_FD = Vertex_boundary, Vertex_uboundary_FD = Vertex_uboundary, Vertex_lboundary_FD = Vertex_lboundary;
    double Vertex_boundary_CD = Vertex_boundary, Vertex_uboundary_CD = Vertex_uboundary, Vertex_lboundary_CD = Vertex_lboundary;
    double dV_boundary = 8., dV_uboundary = dV_boundary, dV_lboundary = -dV_boundary;
    double dV_boundary_FD = dV_boundary, dV_uboundary_FD = dV_uboundary, dV_lboundary_FD = dV_lboundary;
    double dV_boundary_CD = dV_boundary, dV_uboundary_CD = dV_uboundary, dV_lboundary_CD = dV_lboundary;

    if (apply_cuts) {
        double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
        double dVertex_boundary_FD = Vz_cut_FD.GetUpperCut() - Vz_cut_FD.GetLowerCut(), dVertex_boundary_CD =
                Vz_cut_CD.GetUpperCut() - Vz_cut_CD.GetLowerCut();
        double ddV_boundary = dVz_cuts.GetUpperCut() - dVz_cuts.GetLowerCut();
        double ddV_boundary_FD = dVz_cuts_FD.GetUpperCut() - dVz_cuts_FD.GetLowerCut(), ddV_boundary_CD =
                dVz_cuts_CD.GetUpperCut() - dVz_cuts_CD.GetLowerCut();

        Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
        Vertex_uboundary_FD = Vz_cut_FD.GetUpperCut() + 0.2 * dVertex_boundary_FD, Vertex_lboundary_FD =
                                                                                           Vz_cut_FD.GetLowerCut() - 0.1 * dVertex_boundary_FD;
        Vertex_uboundary_CD = Vz_cut_CD.GetUpperCut() + 0.2 * dVertex_boundary_CD, Vertex_lboundary_CD =
                                                                                           Vz_cut_CD.GetLowerCut() - 0.1 * dVertex_boundary_CD;
        dV_uboundary = dVz_cuts.GetUpperCut() + 0.2 * ddV_boundary, dV_lboundary = dVz_cuts.GetLowerCut() - 0.1 * ddV_boundary;
        dV_uboundary_FD = dVz_cuts_FD.GetUpperCut() + 0.2 * ddV_boundary_FD, dV_lboundary_FD = dVz_cuts_FD.GetLowerCut() - 0.1 * ddV_boundary_FD;
        dV_uboundary_CD = dVz_cuts_CD.GetUpperCut() + 0.2 * ddV_boundary_CD, dV_lboundary_CD = dVz_cuts_CD.GetLowerCut() - 0.1 * ddV_boundary_CD;

        Vertex_boundary = 8., Vertex_boundary_FD = 8., Vertex_boundary_CD = 8.;
        dV_boundary = 8., dV_boundary_FD = 8., dV_boundary_CD = 8.;
    }

    /* SF boundries */
    const double SF_uboundary = 0.31, SF_lboundary = 0.16;

    /* Momentum boundries */
    const double Momentum_lboundary = 0., Momentum_uboundary = beamE * 1.1;   // Default
    const double CDMomentum_lboundary = 0., CDMomentum_uboundary = beamE / 2; // CD nucleons (pFDpCD & nFDpCD)
    double P_nucFD_lboundary = 0., P_nucFD_uboundary = beamE * 1.1;           // Default
    double P_nucCD_lboundary = 0., P_nucCD_uboundary = beamE / 2;             // CD nucleons (pFDpCD & nFDpCD)

    if (apply_kinematical_cuts) {
        P_nucFD_lboundary = FD_nucleon_momentum_cut.GetLowerCut(), P_nucFD_uboundary = FD_nucleon_momentum_cut.GetUpperCut() * 1.1;
        P_nucCD_lboundary = 0.4, P_nucCD_uboundary = 2.5; // CD nucleons (pFDpCD & nFDpCD)
    }

    /* W boundries */
    const double W_lboundary = 0.5, W_uboundary = (beamE * 1.1) / 2; // Default

    /* Beta boundries */
    const double dBeta_sigma_boundary = 0.1;
    const double Beta_dist_uboundary = 1 + dBeta_sigma_boundary, Beta_dist_lboundary = 1 - dBeta_sigma_boundary;

    const double dBeta_sigma_ZOOMOUT_boundary = 0.1;
    const double Beta_dist_ZOOMOUT_uboundary = 1 + dBeta_sigma_ZOOMOUT_boundary;
    const double Beta_dist_ZOOMOUT_lboundary = 0.9;

    double Beta_boundary_const = 3., Beta_boundary = 3., P_boundary = beamE * 1.425;

    if (apply_cuts) {
        Beta_boundary = 1.25;
        P_boundary = beamE * 1.1;
    }

    /* Angle boundries */
    const double Theta_lboundary_FD = 0., Theta_uboundary_FD = 50.;
    const double Theta_lboundary_CD = 30., Theta_uboundary_CD = 155.;
    const double Opening_Ang_wide_lboundary = 0, Opening_Ang_wide_uboundary = 180.;
    const double Opening_Ang_narrow_lboundary = 0, Opening_Ang_narrow_uboundary = 100.;
    const double Phi_lboundary = -180., Phi_uboundary = 180.;

    /* TKI boundries */
    const double dP_T_boundary = 3.;
    //</editor-fold>

    //</editor-fold>

// Acceptance maps generation -------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Acceptance maps generation">
    /* Acceptance maps are handled completely by the AMaps class */
    cout << "\nSetting Acceptance maps...";

    if (!calculate_truth_level) { Generate_AMaps = false; }

    if (!Generate_AMaps) { AMaps_plots = false; }

    if (reformat_e_bins) { varying_P_e_bins = false; }

    /* Set Bins by case */
    int NumberNucOfMomSlices, NumberElecOfMomSlices, HistElectronSliceNumOfXBins = numTH2Dbins_Electron_Ang_Plots, HistNucSliceNumOfXBins = numTH2Dbins_Nucleon_Ang_Plots;

    //<editor-fold desc="Determine NumberNucOfMomSlices by sample">
    if (VaryingSampleName == "C12_simulation_G18_Q204_6GeV" || VaryingSampleName == "C12x4_simulation_G18_Q204_6GeV") {
        NumberNucOfMomSlices = 9, NumberElecOfMomSlices = 9;
//        NumberNucOfMomSlices = 9, NumberElecOfMomSlices = 35;
    } else {
        NumberNucOfMomSlices = 4, NumberElecOfMomSlices = 4;
    }
    //</editor-fold>

    AMaps aMaps, wMaps;
    //TODO: UPDATE AMaps loading constructor electron histogram's number of bins

    if (Generate_AMaps) {
        aMaps = AMaps(SampleName, reformat_e_bins, varying_P_e_bins, beamE, "AMaps", directories.AMaps_Directory_map["AMaps_1e_cut_Directory"],
                      NumberNucOfMomSlices,
                      NumberElecOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins,
                      HistElectronSliceNumOfXBins);
        wMaps = AMaps(SampleName, reformat_e_bins, varying_P_e_bins, beamE, "WMaps", directories.AMaps_Directory_map["WMaps_1e_cut_Directory"],
                      NumberNucOfMomSlices,
                      NumberElecOfMomSlices, HistNucSliceNumOfXBins, HistNucSliceNumOfXBins, HistElectronSliceNumOfXBins,
                      HistElectronSliceNumOfXBins);
    } else {
        aMaps = AMaps(AcceptanceMapsDirectory, VaryingSampleName, Electron_single_slice_test, Nucleon_single_slice_test, TestSlices);
        wMaps = AMaps(AcceptanceWeightsDirectory, VaryingSampleName, Electron_single_slice_test, Nucleon_single_slice_test, TestSlices);
    }

    cout << " done.\n\n";
    //</editor-fold>

// Acceptance correction data -------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Acceptance correction">
    /* Neutron resolution fits is handled completely by the MomentumResolution class */
    cout << "\nSetting acceptance correction data...";

    bool save_ACorr_data = false;

    DEfficiency eff;

    TList *ACorr_data = new TList();
    string ACorr_data_Dir = ACorrDirectory + SampleName;
    string ACorr_data_listName = ACorr_data_Dir + "/" + "ACorr_data_-_" + SampleName + ".root";
    const char *ACorr_DataName = ACorr_data_listName.c_str();

    if (!calculate_truth_level) { save_ACorr_data = false; }

    cout << " done.\n\n";
    //</editor-fold>

// Neutron resolution & proton smearing ---------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron resolution">
    /* Neutron resolution fits is handled completely by the MomentumResolution class */
    cout << "\nSetting neutron resolution data...";

    if (!calculate_truth_level) { plot_and_fit_MomRes = false; } // Disable resolution-related operations if not calculating TL plots

    if (!apply_nucleon_cuts) { plot_and_fit_MomRes = false; } // Disable resolution-related operations in initial runs

    if (!plot_and_fit_MomRes) { Calculate_momResS2 = false; }

    /* Comment to test smearing and shift */
//    if (apply_nucleon_SmearAndCorr) { plot_and_fit_MomRes = false; }  // Disable resolution-related operations when applying proton smearing

    //<editor-fold desc="Neutron resolution class declaration & definition">
    MomentumResolution nRes("Neutron"), pRes("Proton");

    nRes.MomResInit(plot_and_fit_MomRes, Calculate_momResS2, Run_with_momResS2, VaryingSampleName, NucleonCutsDirectory, beamE,
            FD_nucleon_momentum_cut,
            n_mom_th.GetLowerCut(), MomentumResolutionDirectory, directories.Resolution_Directory_map["nRes_plots_1n_Directory"],
    DeltaSlices, VaryingDelta, SmearMode, CorrMode, momRes_test);
    pRes.MomResInit(plot_and_fit_MomRes, Calculate_momResS2, Run_with_momResS2, VaryingSampleName, NucleonCutsDirectory, beamE,
            FD_nucleon_momentum_cut,
            p_mom_th.GetLowerCut(), MomentumResolutionDirectory, directories.Resolution_Directory_map["pRes_plots_1p_Directory"],
    DeltaSlices, VaryingDelta, SmearMode, CorrMode, momRes_test, ForceSmallpResLimits);

//    //<editor-fold desc="MomRes Original">
//    if (plot_and_fit_MomRes) {
//        nRes.SetMomResCalculations(SampleName, NucleonCutsDirectory, beamE, FD_nucleon_momentum_cut, n_mom_th.GetLowerCut(), Calculate_momResS2,
//                                   Run_with_momResS2,
//                                   MomentumResolutionDirectory, directories.Resolution_Directory_map["nRes_plots_1n_Directory"], DeltaSlices,
//                                   VaryingDelta, SmearMode,
//                                   CorrMode, momRes_test);
//
//        pRes.SetMomResCalculations(SampleName, NucleonCutsDirectory, beamE, FD_nucleon_momentum_cut, p_mom_th.GetLowerCut(), Calculate_momResS2,
//                                   Run_with_momResS2,
//                                   MomentumResolutionDirectory, directories.Resolution_Directory_map["pRes_plots_1p_Directory"], DeltaSlices,
//                                   VaryingDelta, SmearMode, CorrMode,
//                                   momRes_test, ForceSmallpResLimits);
//
//        if (momRes_test) {
//            if (Calculate_momResS2) { // if Calculate_momResS2 = true => load everything from momResS1 files
//                /* Load neutron correction fit parameters */
//                nRes.ReadResDataParam(
//                        (MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" + VaryingSampleName +
//                         ".par").c_str(),
//                        Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, true, false);
//
//                /* Load proton smearing fit parameters */
//                nRes.ReadResDataParam(
//                        (MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" + VaryingSampleName +
//                         ".par").c_str(),
//                        Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, false, true);
//            } else { // if Calculate_momResS2 = false => load everything from either momResS1 or momResS2
//                if (Run_with_momResS2) { // if Calculate_momResS2 = false and Run_with_momResS2 = true => load everything correction from momResS1 and smearing from momResS2
//                    /* Load neutron correction fit parameters */
//                    nRes.ReadResDataParam(
//                            (MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" +
//                             VaryingSampleName + ".par").c_str(),
//                            Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, true, false);
//
//                    /* Load proton smearing fit parameters */
//                    nRes.ReadResDataParam(
//                            (MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS2_fit_param_-_" +
//                             VaryingSampleName + ".par").c_str(),
//                            Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, false, true);
//                } else { // if Calculate_momResS2 = false and Run_with_momResS2 = false => load both correction and smearing from momResS1
//                    /* Load neutron correction fit parameters */
//                    nRes.ReadResDataParam(
//                            (MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" +
//                             VaryingSampleName + ".par").c_str(),
//                            Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, true, false);
//
//                    /* Load proton smearing fit parameters */
//                    nRes.ReadResDataParam(
//                            (MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" +
//                             VaryingSampleName + ".par").c_str(),
//                            Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, false, true);
//                }
//            }
//        }
//    } else { // if plot_and_fit_MomRes = false => Calculate_momResS2 = false !!!
//        nRes.SetSmearAndCorrModes(SmearMode, CorrMode);
//
//        if (Run_with_momResS2) { // if Run_with_momResS2 = true => load everything correction from momResS1 and smearing from momResS2
//
//            string NeutronCorrectionDataFile =
//                    MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" + VaryingSampleName + ".par";
//            string ProtonSmearingDataFile =
//                    MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS2_fit_param_-_" + VaryingSampleName + ".par";
//
//            /* Load neutron correction fit parameters */
//            nRes.ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, true, false);
//
//            /* Load proton smearing fit parameters */
//            nRes.ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, false, true);
//
//            //<editor-fold desc="Safety checks for data files">
//            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
//                cout
//                        << "\n\nMomentumResolution::MomentumResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(
//                        0);
//            }
//
//            if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
//                cout
//                        << "\n\nMomentumResolution::MomentumResolution: proton smearing variables are not being loaded from neutron data! Exiting...\n\n", exit(
//                        0);
//            }
//            //</editor-fold>
//
//        } else { // if Calculate_momResS2 = false and Run_with_momResS2 = false => load both correction and smearing from momResS1
//
//            string NeutronCorrectionDataFile =
//                    MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" + VaryingSampleName + ".par";
//            string ProtonSmearingDataFile =
//                    MomentumResolutionDirectory + "Res_data_-_" + VaryingSampleName + "/Neutron_momResS1_fit_param_-_" + VaryingSampleName + ".par";
//
//            /* Load neutron correction fit parameters */
//            nRes.ReadResDataParam(NeutronCorrectionDataFile.c_str(), Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, true, false);
//
//            /* Load proton smearing fit parameters */
//            nRes.ReadResDataParam(ProtonSmearingDataFile.c_str(), Calculate_momResS2, VaryingSampleName, NucleonCutsDirectory, false, true);
//
//            //<editor-fold desc="Safety checks for data files">
//            if (!findSubstring(NeutronCorrectionDataFile, "Neutron") || findSubstring(NeutronCorrectionDataFile, "Proton")) {
//                cout
//                        << "\n\nMomentumResolution::MomentumResolution: neutron correction variables are not being loaded from neutron data! Exiting...\n\n", exit(
//                        0);
//            }
//
//            if (!findSubstring(ProtonSmearingDataFile, "Neutron") || findSubstring(ProtonSmearingDataFile, "Proton")) {
//                cout
//                        << "\n\nMomentumResolution::MomentumResolution: proton smearing variables are not being loaded from neutron data! Exiting...\n\n", exit(
//                        0);
//            }
//            //</editor-fold>
//
//        }
//    }
//    //</editor-fold>

    //</editor-fold>

    cout << "\ndone.\n\n";
    //</editor-fold>

// Debugging setup ---------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Debugging setup">
    /* Saving a printout of the number of particles in nEvents2print events. Used for clas12ana debugging. */

    const bool GoodProtonsMonitorPlots = true;

    pid.SetGPMonitoringPlots(GoodProtonsMonitorPlots, directories.Angle_Directory_map["CToF_hits_monitoring_2p_Directory"],
    directories.Angle_Directory_map["Double_detection_monitoring_2p_Directory"]);

    const bool PrintEvents = false;
    const int Ne_in_event = 1, Nf_in_event = 2, nEvents2print = 10000;

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
    /* Histogram definitions and setup. */

    cout << "\nDefining histograms...";

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
                                     directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"], "01_Nphe_1e_cut_BC", 0, Nphe_boundary,
                                     numTH1Dbins);
        hNphe_1e_cut_AC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC AC", "#Photo-electrons in HTCC - N_{phe} - AC", "N_{phe}",
                                     directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"], "02_Nphe_1e_cut_AC", 0, Nphe_boundary,
                                     numTH1Dbins);
    } else {
        hNphe_1e_cut_BC_FD = hPlot1D("1e cut", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                     directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"], "01_Nphe_1e_cut", 0, Nphe_boundary, numTH1Dbins);
    }
    //</editor-fold>

    //<editor-fold desc="Nphe plots (1p, FD)">
    hPlot1D hNphe_1p_FD = hPlot1D("1p", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                  directories.Nphe_Directory_map["Nphe_1p_Directory"],
                                  "01_Nphe_1p", 0, Nphe_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Nphe plots (1n, FD)">
    hPlot1D hNphe_1n_FD = hPlot1D("1n", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                  directories.Nphe_Directory_map["Nphe_1n_Directory"],
                                  "01_Nphe_1n", 0, Nphe_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Nphe plots (2p, FD)">
    hPlot1D hNphe_2p_FD = hPlot1D("2p", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                  directories.Nphe_Directory_map["Nphe_2p_Directory"],
                                  "01_Nphe_2p", 0, Nphe_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Nphe plots (pFDpCD, FD)">
    hPlot1D hNphe_pFDpCD_FD = hPlot1D("pFDpCD", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                      directories.Nphe_Directory_map["Nphe_pFDpCD_Directory"], "01_Nphe_pFDpCD", 0, Nphe_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Nphe plots (nFDpCD, FD)">
    hPlot1D hNphe_nFDpCD_FD = hPlot1D("nFDpCD", "", "N_{phe} in HTCC", "#Photo-electrons in HTCC - N_{phe}", "N_{phe}",
                                      directories.Nphe_Directory_map["Nphe_nFDpCD_Directory"], "01_Nphe_nFDpCD", 0, Nphe_boundary, numTH1Dbins);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">

    //<editor-fold desc="Chi2 plots (no #(e) cut)">
    /* Plots of chi2 with no cut on number of electrons - NOT used later! */

    hPlot1D hChi2_Electron_FD = hPlot1D("no #(e) cut", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                        directories.Chi2_Directory_map["Chi2_All_e_Directory"], "01_Electron_chi2", -Chi2_boundary, Chi2_boundary,
                                        numTH1Dbins);

    hPlot1D hChi2_Proton_CD = hPlot1D("no #(e) cut", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                      directories.Chi2_Directory_map["Chi2_All_e_Directory"], "02_Proton_chi2", -Chi2_boundary, Chi2_boundary,
                                      numTH1Dbins);
    hPlot1D hChi2_Proton_FD = hPlot1D("no #(e) cut", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                      directories.Chi2_Directory_map["Chi2_All_e_Directory"], "02_Proton_chi2", -Chi2_boundary, Chi2_boundary,
                                      numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e cut)">
    /* Plots of chi2 with no cut on number of electrons - used later to apply cuts with my mechanism (NOT Justin's ana) */

    hPlot1D hChi2_Electron_1e_cut_FD = hPlot1D("1e cut", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                               directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "01_Electron_chi2", -Chi2_boundary,
                                               Chi2_boundary,
                                               numTH1Dbins);

    hPlot1D hChi2_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                             directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "02_Proton_chi2", -Chi2_boundary,
                                             Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                             directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "02_Proton_chi2", -Chi2_boundary,
                                             Chi2_boundary, numTH1Dbins);

    hPlot1D hChi2_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}",
                                            directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "03_Kplus_chi2", -Chi2_boundary,
                                            Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{K^{+}}", "#chi^{2}_{K^{+}} histogram", "#chi^{2}_{K^{+}}",
                                            directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "03_Kplus_chi2", -Chi2_boundary,
                                            Chi2_boundary, numTH1Dbins);

    hPlot1D hChi2_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}",
                                             directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "04_Kminus_chi2", -Chi2_boundary,
                                             Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{K^{-}}", "#chi^{2}_{K^{-}} histogram", "#chi^{2}_{K^{-}}",
                                             directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "04_Kminus_chi2", -Chi2_boundary,
                                             Chi2_boundary, numTH1Dbins);

    hPlot1D hChi2_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}",
                                             directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "05_piplus_chi2", -Chi2_boundary,
                                             Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{#pi^{+}}", "#chi^{2}_{#pi^{+}} histogram", "#chi^{2}_{#pi^{+}}",
                                             directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "05_piplus_chi2", -Chi2_boundary,
                                             Chi2_boundary, numTH1Dbins);

    hPlot1D hChi2_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}",
                                              directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "06_piminus_chi2", -Chi2_boundary,
                                              Chi2_boundary,
                                              numTH1Dbins);
    hPlot1D hChi2_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{#pi^{-}}", "#chi^{2}_{#pi^{-}} histogram", "#chi^{2}_{#pi^{-}}",
                                              directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "06_piminus_chi2", -Chi2_boundary,
                                              Chi2_boundary,
                                              numTH1Dbins);

    hPlot1D hChi2_deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "#chi^{2}_{D}", "#chi^{2}_{D} histogram", "#chi^{2}_{D}",
                                               directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "07_deuteron_chi2", -Chi2_boundary,
                                               Chi2_boundary,
                                               numTH1Dbins);
    hPlot1D hChi2_deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "#chi^{2}_{D}", "#chi^{2}_{D} histogram", "#chi^{2}_{D}",
                                               directories.Chi2_Directory_map["Chi2_1e_cut_Directory"], "07_deuteron_chi2", -Chi2_boundary,
                                               Chi2_boundary,
                                               numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1p)">
    hPlot1D hChi2_Electron_1p_FD = hPlot1D("1p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                           directories.Chi2_Directory_map["Chi2_1p_Directory"],
                                           "01_Electron_Chi2_1p", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_1p_CD = hPlot1D("1p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                         directories.Chi2_Directory_map["Chi2_1p_Directory"],
                                         "02_Proton_Chi2_1p", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_1p_FD = hPlot1D("1p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                         directories.Chi2_Directory_map["Chi2_1p_Directory"],
                                         "02_Proton_Chi2_1p", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1n)">
    hPlot1D hChi2_Electron_1n_FD = hPlot1D("1n", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                           directories.Chi2_Directory_map["Chi2_1n_Directory"],
                                           "01_Electron_Chi2_1n", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_1n_CD = hPlot1D("1n", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                         directories.Chi2_Directory_map["Chi2_1n_Directory"],
                                         "02_Proton_Chi2_1n", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_1n_FD = hPlot1D("1n", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                         directories.Chi2_Directory_map["Chi2_1n_Directory"],
                                         "02_Proton_Chi2_1n", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (2p)">
    hPlot1D hChi2_Electron_2p_FD = hPlot1D("2p", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                           directories.Chi2_Directory_map["Chi2_2p_Directory"],
                                           "01_Electron_Chi2_2p", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_2p_CD = hPlot1D("2p", "CD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                         directories.Chi2_Directory_map["Chi2_2p_Directory"],
                                         "02_Proton_Chi2_2p", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    hPlot1D hChi2_Proton_2p_FD = hPlot1D("2p", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                         directories.Chi2_Directory_map["Chi2_2p_Directory"],
                                         "02_Proton_Chi2_2p", -Chi2_boundary, Chi2_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (pFDpCD)">
    hPlot1D hChi2_Electron_pFDpCD_FD = hPlot1D("pFDpCD", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                               directories.Chi2_Directory_map["Chi2_pFDpCD_Directory"], "01_Electron_Chi2_pFDpCD", -Chi2_boundary,
                                               Chi2_boundary,
                                               numTH1Dbins);

    hPlot1D hChi2_Proton_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#chi^{2}_{pCD}", "#chi^{2}_{pCD} histogram", "#chi^{2}_{pCD}",
                                             directories.Chi2_Directory_map["Chi2_pFDpCD_Directory"], "02_Proton_Chi2_pFDpCD", -Chi2_boundary,
                                             Chi2_boundary,
                                             numTH1Dbins);
    hPlot1D hChi2_Proton_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#chi^{2}_{pFD}", "#chi^{2}_{pFD} histogram", "#chi^{2}_{pFD}",
                                             directories.Chi2_Directory_map["Chi2_pFDpCD_Directory"], "02_Proton_Chi2_pFDpCD", -Chi2_boundary,
                                             Chi2_boundary,
                                             numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (nFDpCD)">
    hPlot1D hChi2_Electron_nFDpCD_FD = hPlot1D("nFDpCD", "", "#chi^{2}_{e}", "#chi^{2}_{e} histogram", "#chi^{2}_{e}",
                                               directories.Chi2_Directory_map["Chi2_nFDpCD_Directory"], "01_Electron_Chi2_nFDpCD", -Chi2_boundary,
                                               Chi2_boundary,
                                               numTH1Dbins);

    hPlot1D hChi2_Proton_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#chi^{2}_{pCD}", "#chi^{2}_{pCD} histogram", "#chi^{2}_{pCD}",
                                             directories.Chi2_Directory_map["Chi2_nFDpCD_Directory"], "02_Proton_Chi2_nFDpCD", -Chi2_boundary,
                                             Chi2_boundary,
                                             numTH1Dbins);
    hPlot1D hChi2_Proton_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#chi^{2}_{p}", "#chi^{2}_{p} histogram", "#chi^{2}_{p}",
                                             directories.Chi2_Directory_map["Chi2_nFDpCD_Directory"], "02_Proton_Chi2_nFDpCD", -Chi2_boundary,
                                             Chi2_boundary,
                                             numTH1Dbins);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">

    //<editor-fold desc="Vertex plots (1e cut)">
    hPlot1D hVx_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{x}^{e}", "V_{x}^{e} histogram", "V_{x}^{e} [cm]",
                                             directories.Vertex_Directory_map["Vertex_Electron_1e_cut_Vtx_Directory"],
                                             "01_Electron_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_Electron_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{y}^{e}", "V_{y}^{e} histogram", "V_{y}^{e} [cm]",
                                             directories.Vertex_Directory_map["Vertex_Electron_1e_cut_Vtx_Directory"],
                                             "02_Electron_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_Electron_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_Electron_1e_cut_FD = hPlot1D("1e cut", "", "V_{z}^{e}", "V_{z}^{e} histogram", "V_{z}^{e} [cm]",
                                             directories.Vertex_Directory_map["Vertex_Electron_1e_cut_Vtx_Directory"],
                                             "03_Electron_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_Electron_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    hPlot1D hVx_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{p}", "V_{x}^{p} histogram", "V_{x}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"],
                                           "01_Proton_Vx", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVx_Proton_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVx_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{p}", "V_{x}^{p} histogram", "V_{x}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"],
                                           "01_Proton_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_Proton_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{p}", "V_{y}^{p} histogram", "V_{y}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"],
                                           "02_Proton_Vy", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVy_Proton_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVy_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{p}", "V_{y}^{p} histogram", "V_{y}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"],
                                           "02_Proton_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_Proton_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_Proton_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{p}", "V_{z}^{p} histogram", "V_{z}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"],
                                           "03_Proton_Vz", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVz_Proton_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hPlot1D hVz_Proton_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{p}", "V_{z}^{p} histogram", "V_{z}^{p} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Proton_1e_cut_Vtx_Directory"],
                                           "03_Proton_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_Proton_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    hPlot1D hVx_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{K^{+}}", "V_{x}^{K^{+}} histogram", "V_{x}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"],
                                          "01_Kplus_Vx", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVx_Kplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVx_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{K^{+}}", "V_{x}^{K^{+}} histogram", "V_{x}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"],
                                          "01_Kplus_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_Kplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{K^{+}}", "V_{y}^{K^{+}} histogram", "V_{y}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"],
                                          "02_Kplus_Vy", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVy_Kplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVy_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{K^{+}}", "V_{y}^{K^{+}} histogram", "V_{y}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"],
                                          "02_Kplus_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_Kplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{K^{+}}", "V_{z}^{K^{+}} histogram", "V_{z}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"],
                                          "03_Kplus_Vz", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVz_Kplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hPlot1D hVz_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{K^{+}}", "V_{z}^{K^{+}} histogram", "V_{z}^{K^{+}} [cm]",
                                          directories.Vertex_Directory_map["Vertex_Kplus_1e_cut_Vtx_Directory"],
                                          "03_Kplus_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_Kplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    hPlot1D hVx_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{K^{-}}", "V_{x}^{K^{-}} histogram", "V_{x}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"],
                                           "01_Kminus_Vx", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVx_Kminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVx_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{K^{-}}", "V_{x}^{K^{-}} histogram", "V_{x}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"],
                                           "01_Kminus_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_Kminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{K^{-}}", "V_{y}^{K^{-}} histogram", "V_{y}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"],
                                           "02_Kminus_Vy", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVy_Kminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVy_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{K^{-}}", "V_{y}^{K^{-}} histogram", "V_{y}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"],
                                           "02_Kminus_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_Kminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{K^{-}}", "V_{z}^{K^{-}} histogram", "V_{z}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"],
                                           "03_Kminus_Vz", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVz_Kminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hPlot1D hVz_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{K^{-}}", "V_{z}^{K^{-}} histogram", "V_{z}^{K^{-}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_Kminus_1e_cut_Vtx_Directory"],
                                           "03_Kminus_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_Kminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    hPlot1D hVx_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{#pi^{+}}", "V_{x}^{#pi^{+}} histogram", "V_{x}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"],
                                           "01_piplus_Vx", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVx_piplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVx_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{#pi^{+}}", "V_{x}^{#pi^{+}} histogram", "V_{x}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"],
                                           "01_piplus_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_piplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{#pi^{+}}", "V_{y}^{#pi^{+}} histogram", "V_{y}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"],
                                           "02_piplus_Vy", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVy_piplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVy_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{#pi^{+}}", "V_{y}^{#pi^{+}} histogram", "V_{y}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"],
                                           "02_piplus_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_piplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{#pi^{+}}", "V_{z}^{#pi^{+}} histogram", "V_{z}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"],
                                           "03_piplus_Vz", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVz_piplus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hPlot1D hVz_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{#pi^{+}}", "V_{z}^{#pi^{+}} histogram", "V_{z}^{#pi^{+}} [cm]",
                                           directories.Vertex_Directory_map["Vertex_piplus_1e_cut_Vtx_Directory"],
                                           "03_piplus_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_piplus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    hPlot1D hVx_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{#pi^{-}}", "V_{x}^{#pi^{-}} histogram", "V_{x}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"],
                                            "01_piminus_Vx", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVx_piminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVx_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{#pi^{-}}", "V_{x}^{#pi^{-}} histogram", "V_{x}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"],
                                            "01_piminus_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_piminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{#pi^{-}}", "V_{y}^{#pi^{-}} histogram", "V_{y}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"],
                                            "02_piminus_Vy", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVy_piminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVy_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{#pi^{-}}", "V_{y}^{#pi^{-}} histogram", "V_{y}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"],
                                            "02_piminus_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_piminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{#pi^{-}}", "V_{z}^{#pi^{-}} histogram", "V_{z}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"],
                                            "03_piminus_Vz", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVz_piminus_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hPlot1D hVz_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{#pi^{-}}", "V_{z}^{#pi^{-}} histogram", "V_{z}^{#pi^{-}} [cm]",
                                            directories.Vertex_Directory_map["Vertex_piminus_1e_cut_Vtx_Directory"],
                                            "03_piminus_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_piminus_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);

    hPlot1D hVx_Deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{x}^{D}", "V_{x}^{D} histogram", "V_{x}^{D} [cm]",
                                             directories.Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"],
                                             "01_Deuteron_Vx", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVx_Deuteron_1e_cut_CD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVx_Deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{x}^{D}", "V_{x}^{D} histogram", "V_{x}^{D} [cm]",
                                             directories.Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"],
                                             "01_Deuteron_Vx", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVx_Deuteron_1e_cut_FD.SetLogScalePlot(Log_scale_Vx_plots);
    hPlot1D hVy_Deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{y}^{D}", "V_{y}^{D} histogram", "V_{y}^{D} [cm]",
                                             directories.Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"],
                                             "02_Deuteron_Vy", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVy_Deuteron_1e_cut_CD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVy_Deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{y}^{D}", "V_{y}^{D} histogram", "V_{y}^{D} [cm]",
                                             directories.Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"],
                                             "02_Deuteron_Vy", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVy_Deuteron_1e_cut_FD.SetLogScalePlot(Log_scale_Vy_plots);
    hPlot1D hVz_Deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "V_{z}^{D}", "V_{z}^{D} histogram", "V_{z}^{D} [cm]",
                                             directories.Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"],
                                             "03_Deuteron_Vz", Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hVz_Deuteron_1e_cut_CD.SetLogScalePlot(Log_scale_Vz_plots);
    hPlot1D hVz_Deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "V_{z}^{D}", "V_{z}^{D} histogram", "V_{z}^{D} [cm]",
                                             directories.Vertex_Directory_map["Vertex_deuteron_1e_cut_Vtx_Directory"],
                                             "03_Deuteron_Vz", Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hVz_Deuteron_1e_cut_FD.SetLogScalePlot(Log_scale_Vz_plots);
    //</editor-fold>

    //<editor-fold desc="dV plots (1e cut)">
    hPlot1D hdVx_Proton_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}",
                                            "dV^{p}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"],
                                            "01_dVx_d", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_Proton_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}",
                                            "dV^{p}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"],
                                            "02_dVy_d", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_Proton_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}",
                                            "dV^{p}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"],
                                            "03_dVz_d", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hdVz_Proton_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hPlot1D hdVx_Proton_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}",
                                            "dV^{p}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"],
                                            "01_dVx_d", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_Proton_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}",
                                            "dV^{p}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"],
                                            "02_dVy_d", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_Proton_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}",
                                            "dV^{p}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_Proton_1e_cut_Vtx_Directory"],
                                            "03_dVz_d", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hdVz_Proton_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);

    hPlot1D hdVx_Kplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{+}}_{x}",
                                           "Vertex correlation dV^{K^{+}}_{x}=V^{K^{+}}_{x}-V^{e}_{x}",
                                           "dV^{K^{+}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"],
                                           "01_dVx_Kplus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_Kplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{+}}_{y}",
                                           "Vertex correlation dV^{K^{+}}_{y}=V^{K^{+}}_{y}-V^{e}_{y}",
                                           "dV^{K^{+}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"],
                                           "02_dVy_Kplus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_Kplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{+}}_{z}",
                                           "Vertex correlation dV^{K^{+}}_{z}=V^{K^{+}}_{z}-V^{e}_{z}",
                                           "dV^{K^{+}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"],
                                           "03_dVz_Kplus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hdVz_Kplus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hPlot1D hdVx_Kplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{+}}_{x}",
                                           "Vertex correlation dV^{K^{+}}_{x}=V^{K^{+}}_{x}-V^{e}_{x}",
                                           "dV^{K^{+}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"],
                                           "01_dVx_Kplus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_Kplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{+}}_{y}",
                                           "Vertex correlation dV^{K^{+}}_{y}=V^{K^{+}}_{y}-V^{e}_{y}",
                                           "dV^{K^{+}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"],
                                           "02_dVy_Kplus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_Kplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{+}}_{z}",
                                           "Vertex correlation dV^{K^{+}}_{z}=V^{K^{+}}_{z}-V^{e}_{z}",
                                           "dV^{K^{+}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kplus_1e_cut_Vtx_Directory"],
                                           "03_dVz_Kplus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hdVz_Kplus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);

    hPlot1D hdVx_Kminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{-}}_{x}",
                                            "Vertex correlation dV^{K^{-}}_{x}=V^{K^{-}}_{x}-V^{e}_{x}",
                                            "dV^{K^{-}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"],
                                            "01_dVx_Kminus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_Kminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{-}}_{y}",
                                            "Vertex correlation dV^{K^{-}}_{y}=V^{K^{-}}_{y}-V^{e}_{y}",
                                            "dV^{K^{-}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"],
                                            "02_dVy_Kminus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_Kminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{K^{-}}_{z}",
                                            "Vertex correlation dV^{K^{-}}_{z}=V^{K^{-}}_{z}-V^{e}_{z}",
                                            "dV^{K^{-}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"],
                                            "03_dVz_Kminus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hdVz_Kminus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hPlot1D hdVx_Kminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{-}}_{x}",
                                            "Vertex correlation dV^{K^{-}}_{x}=V^{K^{-}}_{x}-V^{e}_{x}",
                                            "dV^{K^{-}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"],
                                            "01_dVx_Kminus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_Kminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{-}}_{y}",
                                            "Vertex correlation dV^{K^{-}}_{y}=V^{K^{-}}_{y}-V^{e}_{y}",
                                            "dV^{K^{-}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"],
                                            "02_dVy_Kminus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_Kminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{K^{-}}_{z}",
                                            "Vertex correlation dV^{K^{-}}_{z}=V^{K^{-}}_{z}-V^{e}_{z}",
                                            "dV^{K^{-}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_Kminus_1e_cut_Vtx_Directory"],
                                            "03_dVz_Kminus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hdVz_Kminus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);

    hPlot1D hdVx_piplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{+}}_{x}",
                                            "Vertex correlation dV^{#pi^{+}}_{x}=V^{#pi^{+}}_{x}-V^{e}_{x}",
                                            "dV^{#pi^{+}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"],
                                            "01_dVx_Piplus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_piplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{+}}_{y}",
                                            "Vertex correlation dV^{#pi^{+}}_{y}=V^{#pi^{+}}_{y}-V^{e}_{y}",
                                            "dV^{#pi^{+}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"],
                                            "02_dVy_Piplus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_piplus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{+}}_{z}",
                                            "Vertex correlation dV^{#pi^{+}}_{z}=V^{#pi^{+}}_{z}-V^{e}_{z}",
                                            "dV^{#pi^{+}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"],
                                            "03_dVz_Piplus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hdVz_piplus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hPlot1D hdVx_piplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{+}}_{x}",
                                            "Vertex correlation dV^{#pi^{+}}_{x}=V^{#pi^{+}}_{x}-V^{e}_{x}",
                                            "dV^{#pi^{+}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"],
                                            "01_dVx_Piplus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_piplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{+}}_{y}",
                                            "Vertex correlation dV^{#pi^{+}}_{y}=V^{#pi^{+}}_{y}-V^{e}_{y}",
                                            "dV^{#pi^{+}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"],
                                            "02_dVy_Piplus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_piplus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{+}}_{z}",
                                            "Vertex correlation dV^{#pi^{+}}_{z}=V^{#pi^{+}}_{z}-V^{e}_{z}",
                                            "dV^{#pi^{+}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_piplus_1e_cut_Vtx_Directory"],
                                            "03_dVz_Piplus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hdVz_piplus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);

    hPlot1D hdVx_piminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{-}}_{x}",
                                             "Vertex correlation dV^{#pi^{-}}_{x}=V^{#pi^{-}}_{x}-V^{e}_{x}",
                                             "dV^{#pi^{-}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"],
                                             "01_dVx_Piminus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_piminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{-}}_{y}",
                                             "Vertex correlation dV^{#pi^{-}}_{y}=V^{#pi^{-}}_{y}-V^{e}_{y}",
                                             "dV^{#pi^{-}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"],
                                             "02_dVy_Piminus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_piminus_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{#pi^{-}}_{z}",
                                             "Vertex correlation dV^{#pi^{-}}_{z}=V^{#pi^{-}}_{z}-V^{e}_{z}",
                                             "dV^{#pi^{-}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"],
                                             "03_dVz_Piminus", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hdVz_piminus_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hPlot1D hdVx_piminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{-}}_{x}",
                                             "Vertex correlation dV^{#pi^{-}}_{x}=V^{#pi^{-}}_{x}-V^{e}_{x}",
                                             "dV^{#pi^{-}}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"],
                                             "01_dVx_Piminus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_piminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{-}}_{y}",
                                             "Vertex correlation dV^{#pi^{-}}_{y}=V^{#pi^{-}}_{y}-V^{e}_{y}",
                                             "dV^{#pi^{-}}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"],
                                             "02_dVy_Piminus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_piminus_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{#pi^{-}}_{z}",
                                             "Vertex correlation dV^{#pi^{-}}_{z}=V^{#pi^{-}}_{z}-V^{e}_{z}",
                                             "dV^{#pi^{-}}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_piminus_1e_cut_Vtx_Directory"],
                                             "03_dVz_Piminus", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hdVz_piminus_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);

    hPlot1D hdVx_Deuteron_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{D}_{x}", "Vertex correlation dV^{D}_{x}=V^{D}_{x}-V^{e}_{x}",
                                              "dV^{D}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"],
                                              "01_dVx_d", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_Deuteron_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{D}_{y}", "Vertex correlation dV^{D}_{y}=V^{D}_{y}-V^{e}_{y}",
                                              "dV^{D}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"],
                                              "02_dVy_d", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_Deuteron_FD_1e_cut = hPlot1D("1e cut", "FD", "Vertex corr. dV^{D}_{z}", "Vertex correlation dV^{D}_{z}=V^{D}_{z}-V^{e}_{z}",
                                              "dV^{D}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"],
                                              "03_dVz_d", dV_lboundary_FD, dV_uboundary_FD, numTH1Dbins);
    hdVz_Deuteron_FD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    hPlot1D hdVx_Deuteron_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{D}_{x}", "Vertex correlation dV^{D}_{x}=V^{D}_{x}-V^{e}_{x}",
                                              "dV^{D}_{x} [cm]", directories.Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"],
                                              "01_dVx_d", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_Deuteron_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{D}_{y}", "Vertex correlation dV^{D}_{y}=V^{D}_{y}-V^{e}_{y}",
                                              "dV^{D}_{y} [cm]", directories.Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"],
                                              "02_dVy_d", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_Deuteron_CD_1e_cut = hPlot1D("1e cut", "CD", "Vertex corr. dV^{D}_{z}", "Vertex correlation dV^{D}_{z}=V^{D}_{z}-V^{e}_{z}",
                                              "dV^{D}_{z} [cm]", directories.Vertex_Directory_map["Vertex_corr_deuteron_1e_cut_Vtx_Directory"],
                                              "03_dVz_d", dV_lboundary_CD, dV_uboundary_CD, numTH1Dbins);
    hdVz_Deuteron_CD_1e_cut.SetLogScalePlot(Log_scale_dVz_plots);
    //</editor-fold>

    //<editor-fold desc="dV plots (1p)">
    hPlot1D hdVx_1p = hPlot1D("1p", "", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1p_Directory"], "01_dVx", -dV_boundary, dV_boundary, numTH1Dbins);
    hPlot1D hdVy_1p = hPlot1D("1p", "", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1p_Directory"], "02_dVy", -dV_boundary, dV_boundary, numTH1Dbins);
    hPlot1D hdVz_1p = hPlot1D("1p", "", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1p_Directory"], "03_dVz", -dV_boundary, dV_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="dV plots (1n)">
    hPlot1D hdVx_1n = hPlot1D("1n", "", "Vertex corr. dV^{n}_{x}", "Vertex correlation dV^{n}_{x}=V^{n}_{x}-V^{e}_{x}", "dV^{n}_{x} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1n_Directory"], "01_dVx", -dV_boundary, dV_boundary, numTH1Dbins);
    hPlot1D hdVy_1n = hPlot1D("1n", "", "Vertex corr. dV^{n}_{y}", "Vertex correlation dV^{n}_{y}=V^{n}_{y}-V^{e}_{y}", "dV^{n}_{y} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1n_Directory"], "02_dVy", -dV_boundary, dV_boundary, numTH1Dbins);
    hPlot1D hdVz_1n = hPlot1D("1n", "", "Vertex corr. dV^{n}_{z}", "Vertex correlation dV^{n}_{z}=V^{n}_{z}-V^{e}_{z}", "dV^{n}_{z} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_1n_Directory"], "03_dVz", -dV_boundary, dV_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="dV plots (2p)">
    hPlot1D hdVx_2p = hPlot1D("2p", "", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_2p_Directory"], "01_dVx", -dV_boundary, dV_boundary, numTH1Dbins);
    hPlot1D hdVy_2p = hPlot1D("2p", "", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_2p_Directory"], "02_dVy", -dV_boundary, dV_boundary, numTH1Dbins);
    hPlot1D hdVz_2p = hPlot1D("2p", "", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]",
                              directories.Vertex_Directory_map["Vertex_dV_2p_Directory"], "03_dVz", -dV_boundary, dV_boundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="dV plots (pFDpCD)">
    hPlot1D hdVx_pFDpCD = hPlot1D("pFDpCD", "", "Vertex corr. dV^{p}_{x}", "Vertex correlation dV^{p}_{x}=V^{p}_{x}-V^{e}_{x}", "dV^{p}_{x} [cm]",
                                  directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "01_dVx", -dV_boundary, dV_boundary,
                                  numTH1Dbins);
    hPlot1D hdVy_pFDpCD = hPlot1D("pFDpCD", "", "Vertex corr. dV^{p}_{y}", "Vertex correlation dV^{p}_{y}=V^{p}_{y}-V^{e}_{y}", "dV^{p}_{y} [cm]",
                                  directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "02_dVy", -dV_boundary, dV_boundary,
                                  numTH1Dbins);
    hPlot1D hdVz_pFDpCD = hPlot1D("pFDpCD", "", "Vertex corr. dV^{p}_{z}", "Vertex correlation dV^{p}_{z}=V^{p}_{z}-V^{e}_{z}", "dV^{p}_{z} [cm]",
                                  directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "03_dVz", -dV_boundary, dV_boundary,
                                  numTH1Dbins);

    hPlot1D hdVx_pFD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pFD}_{x} of FD proton", "Vertex corr. dV^{pFD}_{x}=V^{pFD}_{x}-V^{e}_{x} of FD proton",
                                      "dV^{pFD}_{x} [cm]", directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "04_dVx_pFD",
                                      Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hPlot1D hdVy_pFD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pFD}_{y} of FD proton", "Vertex corr. dV^{pFD}_{y}=V^{pFD}_{y}-V^{e}_{y} of FD proton",
                                      "dV^{pFD}_{y} [cm]", directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "05_dVy_pFD",
                                      Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);
    hPlot1D hdVz_pFD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pFD}_{z} of FD proton", "Vertex corr. dV^{pFD}_{z}=V^{pFD}_{z}-V^{e}_{z} of FD proton",
                                      "dV^{pFD}_{z} [cm]", directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "06_dVz_pFD",
                                      Vertex_lboundary_FD, Vertex_uboundary_FD, numTH1Dbins);

    hPlot1D hdVx_pCD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pCD}_{x} of CD proton", "Vertex corr. dV^{pCD}_{x}=V^{pCD}_{x}-V^{e}_{x} of CD proton",
                                      "dV^{pCD}_{x} [cm]", directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "07_dVx_pCD",
                                      Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_pCD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pCD}_{y} of CD proton", "Vertex corr. dV^{pCD}_{y}=V^{pCD}_{y}-V^{e}_{y} of CD proton",
                                      "dV^{pCD}_{y} [cm]", directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "08_dVy_pCD",
                                      Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_pCD_pFDpCD = hPlot1D("pFDpCD", "", "dV^{pCD}_{z} of CD proton", "Vertex corr. dV^{pCD}_{z}=V^{pCD}_{z}-V^{e}_{z} of CD proton",
                                      "dV^{pCD}_{z} [cm]", directories.Vertex_Directory_map["Vertex_dV_pFDpCD_Directory"], "09_dVz_pCD",
                                      Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="dV plots (nFDpCD)">
    hPlot1D hdVx_nFDpCD = hPlot1D("nFDpCD", "", "Vertex corr. dV_{x}", "Vertex correlation dV_{x}=V^{p}_{x}-V^{e}_{x}", "dV_{x} [cm]",
                                  directories.Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"], "01_dVx", -dV_boundary, dV_boundary,
                                  numTH1Dbins);
    hPlot1D hdVy_nFDpCD = hPlot1D("nFDpCD", "", "Vertex corr. dV_{y}", "Vertex correlation dV_{y}=V^{p}_{y}-V^{e}_{y}", "dV_{y} [cm]",
                                  directories.Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"], "02_dVy", -dV_boundary, dV_boundary,
                                  numTH1Dbins);
    hPlot1D hdVz_nFDpCD = hPlot1D("nFDpCD", "", "Vertex corr. dV_{z}", "Vertex correlation dV_{z}=V^{p}_{z}-V^{e}_{z}", "dV_{z} [cm]",
                                  directories.Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"], "03_dVz", -dV_boundary, dV_boundary,
                                  numTH1Dbins);

    hPlot1D hdVx_pCD_nFDpCD = hPlot1D("nFDpCD", "", "dV^{pCD}_{x} of CD proton", "Vertex corr. dV^{pCD}_{x}=V^{pCD}_{x}-V^{e}_{x} of CD proton",
                                      "dV^{pCD}_{x} [cm]", directories.Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"], "07_dVx_pCD",
                                      Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hPlot1D hdVy_pCD_nFDpCD = hPlot1D("nFDpCD", "", "dV^{pCD}_{y} of CD proton", "Vertex corr. dV^{pCD}_{y}=V^{pCD}_{y}-V^{e}_{y} of CD proton",
                                      "dV^{pCD}_{y} [cm]", directories.Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"], "08_dVy_pCD",
                                      Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    hPlot1D hdVz_pCD_nFDpCD = hPlot1D("nFDpCD", "", "dV^{pCD}_{z} of CD proton", "Vertex corr. dV^{pCD}_{z}=V^{pCD}_{z}-V^{e}_{z} of CD proton",
                                      "dV^{pCD}_{z} [cm]", directories.Vertex_Directory_map["Vertex_dV_nFDpCD_Directory"], "09_dVz_pCD",
                                      Vertex_lboundary_CD, Vertex_uboundary_CD, numTH1Dbins);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Sampling Fraction (SF) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Sampling Fraction (SF) histograms (FD)">

    //<editor-fold desc="Sampling Fraction (SF) histograms (1e cut, FD only)">

    //<editor-fold desc="SF plots (1e cut, FD)">
    hPlot1D hSF_1e_cut_BC_FD, hSF_1e_cut_AC_FD;

    if (!apply_cuts) {
        hSF_1e_cut_BC_FD = hPlot1D("1e cut", "FD", "SF BC", "Sampling fraction f_{e} - before cuts", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_Directory_map["SF_1e_cut_BC_Directory"], "01_SF_1e_cut_before_SF_cuts", SF_lboundary,
                                   SF_uboundary, numTH1Dbins);
        hSF_1e_cut_AC_FD = hPlot1D("1e cut", "FD", "SF AC", "Sampling fraction f_{e} - after cuts", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_Directory_map["SF_1e_cut_AC_Directory"], "02_SF_1e_cut_after_SF_cuts", SF_lboundary,
                                   SF_uboundary, numTH1Dbins);
    } else {
        hSF_1e_cut_BC_FD = hPlot1D("1e cut", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                   directories.SF_Directory_map["SF_1e_cut_BC_Directory"], "01_SF_1e_cut", SF_lboundary, SF_uboundary,
                                   numTH1Dbins);
    }
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1e cut, FD)">
    hPlot2D hSF_VS_P_e_1e_cut_BC_FD, hSF_VS_P_e_1e_cut_AC_FD;

    if (!apply_cuts) {
        hSF_VS_P_e_1e_cut_BC_FD = hPlot2D("1e cut", "FD", "SF vs. P_{e} BC", "Sampling fraction f_{e} vs. P_{e} - before cuts", "P_{e} [GeV/c]",
                                          "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                          directories.SF_Directory_map["SF_VS_P_e_1e_cut_BC_Directory"],
                                          "01_SF_VS_P_e_1e_cut_BC_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
        hSF_VS_P_e_1e_cut_AC_FD = hPlot2D("1e cut", "FD", "SF vs. P_{e} AC", "Sampling fraction f_{e} vs. P_{e} - after cuts", "P_{e} [GeV/c]",
                                          "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                          directories.SF_Directory_map["SF_VS_P_e_1e_cut_AC_Directory"],
                                          "01_SF_VS_P_e_1e_cut_AC_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    } else {
        hSF_VS_P_e_1e_cut_BC_FD = hPlot2D("1e cut", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]",
                                          "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                          directories.SF_Directory_map["SF_VS_P_e_1e_cut_BC_Directory"],
                                          "01_SF_VS_P_e_1e_cut_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    }
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (1p, FD only)">

    //<editor-fold desc="SF plots (1p, FD)">
    hPlot1D hSF_1p_FD = hPlot1D("1p", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                directories.SF_Directory_map["SF_1p_Directory"], "01_SF_1p", SF_lboundary, SF_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1p, FD)">
    hPlot2D hSF_VS_P_e_1p_FD = hPlot2D("1p", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]",
                                       "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_1p_Directory"],
                                       "01_SF_VS_P_e_1p_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (1n, FD only)">

    //<editor-fold desc="SF plots (1n, FD)">
    hPlot1D hSF_1n_FD = hPlot1D("1n", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                directories.SF_Directory_map["SF_1n_Directory"], "01_SF_1n", SF_lboundary, SF_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1n, FD)">
    hPlot2D hSF_VS_P_e_1n_FD = hPlot2D("1n", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]",
                                       "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_1n_Directory"],
                                       "01_SF_VS_P_e_1n_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (2p, FD only)">

    //<editor-fold desc="SF plots (2p, FD)">
    hPlot1D hSF_2p_FD = hPlot1D("2p", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                directories.SF_Directory_map["SF_2p_Directory"], "01_SF_2p", SF_lboundary, SF_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (2p, FD)">
    hPlot2D hSF_VS_P_e_2p_FD = hPlot2D("2p", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]",
                                       "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", directories.SF_Directory_map["SF_VS_P_e_2p_Directory"],
                                       "01_SF_VS_P_e_2p_FD", 0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (pFDpCD, FD only)">

    //<editor-fold desc="SF plots (pFDpCD, FD)">
    hPlot1D hSF_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                    directories.SF_Directory_map["SF_pFDpCD_Directory"], "01_SF_pFDpCD", SF_lboundary, SF_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (pFDpCD, FD)">
    hPlot2D hSF_VS_P_e_pFDpCD_FD = hPlot2D("pFDpCD", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]",
                                           "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           directories.SF_Directory_map["SF_VS_P_e_pFDpCD_Directory"], "01_SF_VS_P_e_pFDpCD_FD",
                                           0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (nFDpCD, FD only)">

    //<editor-fold desc="SF plots (nFDpCD, FD)">
    hPlot1D hSF_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "SF", "Sampling fraction f_{e}", "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                    directories.SF_Directory_map["SF_nFDpCD_Directory"], "01_SF_nFDpCD", SF_lboundary, SF_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (nFDpCD, FD)">
    hPlot2D hSF_VS_P_e_nFDpCD_FD = hPlot2D("nFDpCD", "FD", "SF vs. P_{e}", "Sampling fraction f_{e} vs. P_{e}", "P_{e} [GeV/c]",
                                           "f_{e} = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           directories.SF_Directory_map["SF_VS_P_e_nFDpCD_Directory"], "01_SF_VS_P_e_nFDpCD_FD",
                                           0, beamE * 1.1, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ECAL edge histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="ECAL edge histograms (FD only)">

    //<editor-fold desc="ECAL coordinates vs. SF plots (1e cut, FD)">
    hPlot2D hVcal_VS_EoP_1e_cut_BC_PCAL, hVcal_VS_EoP_1e_cut_AC_PCAL, hWcal_VS_EoP_1e_cut_BC_PCAL, hWcal_VS_EoP_1e_cut_AC_PCAL;

    if (!apply_cuts) {
        hVcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Vcal vs. SF BC", "ECAL V coordinate vs. SF - before cuts",
                                              "ECAL V coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"],
                                              "01_Vcal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
        hVcal_VS_EoP_1e_cut_AC_PCAL = hPlot2D("1e cut", "PCAL", "Vcal vs. SF AC", "ECAL V coordinate vs. SF - after cuts",
                                              "ECAL V coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"],
                                              "01_Vcal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);

        hWcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Wcal vs. SF BC", "ECAL W coordinate vs. SF - before cuts",
                                              "ECAL W coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"],
                                              "02_Wcal_VS_EoP_PCAL_1e_cut_BC", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
        hWcal_VS_EoP_1e_cut_AC_PCAL = hPlot2D("1e cut", "PCAL", "Wcal vs. SF AC", "ECAL W coordinate vs. SF - after cuts",
                                              "ECAL W coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"],
                                              "02_Wcal_VS_EoP_PCAL_1e_cut_AC", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    } else {
        hVcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"],
                                              "01_Vcal_VS_EoP_PCAL_1e_cut", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
        hWcal_VS_EoP_1e_cut_BC_PCAL = hPlot2D("1e cut", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                              "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1e_BC_PCAL_Directory"],
                                              "02_Wcal_VS_EoP_PCAL_1e_cut", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    }
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (1p, FD)">
    hPlot2D hVcal_VS_EoP_1p_PCAL = hPlot2D("1p", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1p_PCAL_Directory"],
                                           "01_Vcal_VS_EoP_PCAL_1p", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    hPlot2D hWcal_VS_EoP_1p_PCAL = hPlot2D("1p", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1p_PCAL_Directory"],
                                           "02_Wcal_VS_EoP_PCAL_1p", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (1n, FD)">
    hPlot2D hVcal_VS_EoP_1n_PCAL = hPlot2D("1n", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1n_PCAL_Directory"],
                                           "01_Vcal_VS_EoP_PCAL_1n", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    hPlot2D hWcal_VS_EoP_1n_PCAL = hPlot2D("1n", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_1n_PCAL_Directory"],
                                           "02_Wcal_VS_EoP_PCAL_1n", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (2p, FD)">
    hPlot2D hVcal_VS_EoP_2p_PCAL = hPlot2D("2p", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_2p_PCAL_Directory"],
                                           "01_Vcal_VS_EoP_PCAL_2p", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    hPlot2D hWcal_VS_EoP_2p_PCAL = hPlot2D("2p", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                           "Sampling Fraction (SF)", directories.Fiducial_Directory_map["Edge_2p_PCAL_Directory"],
                                           "02_Wcal_VS_EoP_PCAL_2p", 0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (pFDpCD, FD)">
    hPlot2D hVcal_VS_EoP_pFDpCD_PCAL = hPlot2D("pFDpCD", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                               "Sampling Fraction (SF)",
                                               directories.Fiducial_Directory_map["Edge_pFDpCD_PCAL_Directory"], "01_Vcal_VS_EoP_PCAL_pFDpCD",
                                               0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    hPlot2D hWcal_VS_EoP_pFDpCD_PCAL = hPlot2D("pFDpCD", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                               "Sampling Fraction (SF)",
                                               directories.Fiducial_Directory_map["Edge_pFDpCD_PCAL_Directory"], "02_Wcal_VS_EoP_PCAL_pFDpCD",
                                               0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (nFDpCD, FD)">
    hPlot2D hVcal_VS_EoP_nFDpCD_PCAL = hPlot2D("nFDpCD", "PCAL", "Vcal vs. SF", "ECAL V coordinate vs. SF", "ECAL V coordinate [cm]",
                                               "Sampling Fraction (SF)",
                                               directories.Fiducial_Directory_map["Edge_nFDpCD_PCAL_Directory"], "01_Vcal_VS_EoP_PCAL_nFDpCD",
                                               0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    hPlot2D hWcal_VS_EoP_nFDpCD_PCAL = hPlot2D("nFDpCD", "PCAL", "Wcal vs. SF", "ECAL W coordinate vs. SF", "ECAL W coordinate [cm]",
                                               "Sampling Fraction (SF)",
                                               directories.Fiducial_Directory_map["Edge_nFDpCD_PCAL_Directory"], "02_Wcal_VS_EoP_PCAL_nFDpCD",
                                               0, 50, SF_lboundary, SF_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum threshold histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum threshold histograms">
    /* Here are the definitions for the Momentum threshold plots. These are used to check that momentum cuts were applied in 1e_cut and 2p plots. */

    //<editor-fold desc="Momentum threshold plots (1e cut)">

    //<editor-fold desc="Momentum threshold plots (1e cut)">
    hPlot1D hP_e_1e_cut_FD = hPlot1D("1e cut", "", "Electron momentum", "Electron momentum P_{e}", "P_{e} [GeV/c]",
                                     directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "01_P_e_1e_cut_FD", 0, beamE * 1.1,
                                     numTH1Dbins);

    hPlot1D hP_p_1e_cut_CD = hPlot1D("1e cut", "CD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]",
                                     directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "02_P_p_1e_cut_CD", 0, beamE * 1.1,
                                     numTH1Dbins);
    hPlot1D hP_p_1e_cut_FD = hPlot1D("1e cut", "FD", "Proton momentum", "Proton momentum P_{p}", "P_{p} [GeV/c]",
                                     directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "02_P_p_1e_cut_FD", 0, beamE * 1.1,
                                     numTH1Dbins);

    hPlot1D hP_piplus_1e_cut_CD = hPlot1D("1e cut", "CD", "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "03_P_piplus_1e_cut_CD", 0, beamE * 1.1,
                                          numTH1Dbins);
    hPlot1D hP_piplus_1e_cut_FD = hPlot1D("1e cut", "FD", "#pi^{+} momentum", "#pi^{+} momentum P_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "03_P_piplus_1e_cut_FD", 0, beamE * 1.1,
                                          numTH1Dbins);

    hPlot1D hP_piminus_1e_cut_CD = hPlot1D("1e cut", "CD", "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "04_P_piminus_1e_cut_CD", 0,
                                           beamE * 1.1, numTH1Dbins);
    hPlot1D hP_piminus_1e_cut_FD = hPlot1D("1e cut", "FD", "#pi^{-} momentum", "#pi^{-} momentum P_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "04_P_piminus_1e_cut_FD", 0,
                                           beamE * 1.1, numTH1Dbins);

    hPlot1D hP_Kplus_1e_cut_CD = hPlot1D("1e cut", "CD", "K^{+} momentum", "K^{+} momentum P_{K^{+}}", "P_{K^{+}} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "05_P_Kplus_1e_cut_CD", 0, beamE * 1.1,
                                         numTH1Dbins);
    hPlot1D hP_Kplus_1e_cut_FD = hPlot1D("1e cut", "FD", "K^{+} momentum", "K^{+} momentum P_{K^{+}}", "P_{K^{+}} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "05_P_Kplus_1e_cut_FD", 0, beamE * 1.1,
                                         numTH1Dbins);

    hPlot1D hP_Kminus_1e_cut_CD = hPlot1D("1e cut", "CD", "K^{-} momentum", "K^{-} momentum P_{K^{-}}", "P_{K^{-}} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "06_P_Kminus_1e_cut_CD", 0, beamE * 1.1,
                                          numTH1Dbins);
    hPlot1D hP_Kminus_1e_cut_FD = hPlot1D("1e cut", "FD", "K^{-} momentum", "K^{-} momentum P_{K^{-}}", "P_{K^{-}} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "06_P_Kminus_1e_cut_FD", 0, beamE * 1.1,
                                          numTH1Dbins);

    hPlot1D hP_deuteron_1e_cut_CD = hPlot1D("1e cut", "CD", "Deuterons momentum", "Deuterons momentum P_{D}", "P_{D} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "07_P_deuteron_1e_cut_CD", 0,
                                            beamE * 1.1, numTH1Dbins);
    hPlot1D hP_deuteron_1e_cut_FD = hPlot1D("1e cut", "FD", "Deuterons momentum", "Deuterons momentum P_{D}", "P_{D} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "07_P_deuteron_1e_cut_FD", 0,
                                            beamE * 1.1, numTH1Dbins);

    hPlot1D hP_LnFD_APID_1e_cut_FD = hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID", "Leading FD neutron momentum P_{n} APID",
                                             "P_{n} [GeV/c]",
                                             directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "08a_P_LnFD_APID_1e_cut_FD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins); // Leading nFD
    hPlot1D hP_LnFD_APID_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID - ZOOMOUT",
                                                     "Leading FD neutron momentum P_{n} APID - ZOOMOUT",
                                                     "P_{n} [GeV/c]",
                                                     directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                     "08b_P_LnFD_APID_1e_cut_ZOOMOUT_FD",
                                                     Momentum_lboundary, 15., numTH1Dbins); // Leading nFD
    hPlot1D hP_LnFD_BPID_1e_cut_FD = hPlot1D("1e_cut", "FD", "Leading FD neutron momentum BPID", "Leading FD neutron momentum P_{n} BPID",
                                             "P_{n} [GeV/c]",
                                             directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "08c_P_LnFD_BPID_1e_cut_FD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins); // Leading nFD
    hPlot1D hP_LnFD_BPID_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "Leading FD neutron momentum BPID - ZOOMOUT",
                                                     "Leading FD neutron momentum P_{n} BPID - ZOOMOUT",
                                                     "P_{n} [GeV/c]",
                                                     directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                     "08d_P_LnFD_BPID_1e_cut_ZOOMOUT_FD",
                                                     Momentum_lboundary, 15., numTH1Dbins); // Leading nFD
    hPlot1D hP_LnFD_APIDandNS_1e_cut_FD = hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID&NC",
                                                  "Leading FD neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]",
                                                  directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                  "08e_P_LnFD_APIDandNS_1e_cut_FD",
                                                  Momentum_lboundary, Momentum_uboundary, numTH1Dbins); // Leading nFD
    hPlot1D hP_LnFD_APIDandNS_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "Leading FD neutron momentum APID&NC - ZOOMOUT",
                                                          "Leading FD neutron momentum P_{n} APID&NC - ZOOMOUT",
                                                          "P_{n} [GeV/c]", directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                          "08f_P_LnFD_APIDandNS_1e_cut_ZOOMOUT_FD", Momentum_lboundary, 15.,
                                                          numTH1Dbins); // Leading nFD

    hPlot1D hP_nFD_APID_1e_cut_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum APID", "FD neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "09a_P_nFD_APID_1e_cut_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins); // All nFD
    hPlot1D hP_nFD_APID_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum APID - ZOOMOUT",
                                                    "FD neutron momentum P_{n} APID - ZOOMOUT", "P_{n} [GeV/c]",
                                                    directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                    "09b_P_nFD_APID_1e_cut_ZOOMOUT_FD",
                                                    Momentum_lboundary, 15., numTH1Dbins); // All nFD
    hPlot1D hP_nFD_BPID_1e_cut_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum BPID", "FD neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "09c_P_nFD_BPID_1e_cut_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins); // All nFD
    hPlot1D hP_nFD_BPID_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum BPID - ZOOMOUT",
                                                    "FD neutron momentum P_{n} BPID - ZOOMOUT", "P_{n} [GeV/c]",
                                                    directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                    "09d_P_nFD_BPID_1e_cut_ZOOMOUT_FD",
                                                    Momentum_lboundary, 15., numTH1Dbins); // All nFD
    hPlot1D hP_nFD_APIDandNS_1e_cut_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum APID&NC", "FD neutron momentum P_{n} APID&NC",
                                                 "P_{n} [GeV/c]",
                                                 directories.Momentum_Directory_map["Momentum_1e_cut_Directory"], "09e_P_nFD_APIDandNS_1e_cut_FD",
                                                 Momentum_lboundary, Momentum_uboundary, numTH1Dbins); // All nFD
    hPlot1D hP_nFD_APIDandNS_1e_cut_ZOOMOUT_FD = hPlot1D("1e_cut", "FD", "FD neutron momentum APID&NC - ZOOMOUT",
                                                         "FD neutron momentum P_{n} APID&NC - ZOOMOUT",
                                                         "P_{n} [GeV/c]", directories.Momentum_Directory_map["Momentum_1e_cut_Directory"],
                                                         "09f_P_nFD_APIDandNS_1e_cut_ZOOMOUT_FD", Momentum_lboundary, 15., numTH1Dbins); // All nFD
    //</editor-fold>

    //<editor-fold desc="Reco momentum plots for momentum thresholds (1e cut)">
    hPlot1D hP_e_reco_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD Electron momentum", "FD Electron momentum P^{reco}_{e}", "P^{reco}_{e} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"], "01a_P_e_reco_1e_cut_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD Electron momentum - ZOOMIN",
                                                 "FD Electron momentum P^{reco}_{e} - ZOOMIN",
                                                 "P^{reco}_{e} [GeV/c]", directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                 "01b_P_e_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_e_vs_Theta_e_reco_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{e} vs. #theta_{e}", "P_{e} vs. #theta_{e}", "P_{e} [GeV/c]",
                                                     "#theta_{e} [Deg]",
                                                     directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                     "01c_P_e_vs_Theta_e_reco_1e_cut_FD",
                                                     Momentum_lboundary, Momentum_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins * 2, numTH2Dbins * 2);

    hPlot1D hP_p_reco_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD Proton momentum", "FD Proton momentum P^{reco}_{p}", "P^{reco}_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"], "02a_P_p_reco_1e_cut_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_reco_1e_cut_CD = hPlot1D("1e cut", "CD", "Reco CD Proton momentum", "CD Proton momentum P^{reco}_{p}", "P^{reco}_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"], "02b_P_p_reco_1e_cut_CD",
                                          CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD Proton momentum - ZOOMIN", "FD Proton momentum P^{reco}_{p} - ZOOMIN",
                                                 "P^{reco}_{p} [GeV/c]",
                                                 directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                 "02aa_P_p_reco_1e_cut_FD_ZOOMIN",
                                                 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_reco_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "Reco CD Proton momentum - ZOOMIN", "CD Proton momentum P^{reco}_{p} - ZOOMIN",
                                                 "P^{reco}_{p} [GeV/c]",
                                                 directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                 "02bb_P_p_reco_1e_cut_CD_ZOOMIN",
                                                 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_p_vs_Theta_p_reco_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{p} vs. #theta_{p}", "P_{p} vs. #theta_{p}", "P_{p} [GeV/c]",
                                                     "#theta_{p} [Deg]",
                                                     directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                     "02c_P_p_vs_Theta_p_reco_1e_cut_FD",
                                                     Momentum_lboundary, Momentum_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins * 2, numTH2Dbins * 2);
    hPlot2D hP_p_vs_Theta_p_reco_1e_cut_CD = hPlot2D("1e cut", "CD", "P_{p} vs. #theta_{p}", "P_{p} vs. #theta_{p}", "P_{p} [GeV/c]",
                                                     "#theta_{p} [Deg]",
                                                     directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                     "02d_P_p_vs_Theta_p_reco_1e_cut_CD",
                                                     CDMomentum_lboundary, CDMomentum_uboundary, Theta_lboundary_CD, Theta_uboundary_CD,
                                                     numTH2Dbins * 2, numTH2Dbins * 2);

    // leading nFD (APID):
    hPlot1D hP_LnFD_reco_APID_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum APID",
                                                  "Leading FD neutron momentum P^{reco}_{n} APID",
                                                  "P^{reco}_{n} [GeV/c]", directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                  "03a_P_LnFD_reco_APID_1e_cut_FD", Momentum_lboundary, Momentum_uboundary,
                                                  numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_LnFD_reco_APID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum APID - ZOOMIN",
                                                         "Leading FD neutron momentum P^{reco}_{n} APID - ZOOMIN",
                                                         "P^{reco}_{n} [GeV/c]",
                                                         directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                         "03b_P_LnFD_reco_APID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_LnFD_reco_APID_1e_cut_FD_ZOOMOUT = hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum APID - ZOOMOUT",
                                                          "Leading FD neutron momentum P^{reco}_{n} APID - ZOOMOUT", "P^{reco}_{n} [GeV/c]",
                                                          directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                          "03c_P_LnFD_reco_APID_1e_cut_FD_ZOOMOUT", 0,
                                                          15., numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_LnFD_vs_Theta_LnFD_reco_APID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{LnFD} vs. #theta_{LnFD} APID",
                                                                "P_{LnFD} vs. #theta_{LnFD} APID",
                                                                "P_{LnFD} [GeV/c]", "#theta_{LnFD} [Deg]",
                                                                directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                                "03d_P_LnFD_vs_Theta_LnFD_reco_APID_1e_cut_FD", Momentum_lboundary,
                                                                Momentum_uboundary,
                                                                Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    // leading nFD (BPID):
    hPlot1D hP_LnFD_reco_BPID_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum BPID",
                                                  "Leading FD neutron momentum P^{reco}_{n} BPID",
                                                  "P^{reco}_{n} [GeV/c]", directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                  "03a_P_LnFD_reco_BPID_1e_cut_FD", Momentum_lboundary, Momentum_uboundary,
                                                  numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_LnFD_reco_BPID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum BPID - ZOOMIN",
                                                         "Leading FD neutron momentum P^{reco}_{n} BPID - ZOOMIN",
                                                         "P^{reco}_{n} [GeV/c]",
                                                         directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                         "03b_P_LnFD_reco_BPID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_LnFD_reco_BPID_1e_cut_FD_ZOOMOUT = hPlot1D("1e cut", "FD", "Reco leading FD neutron momentum BPID - ZOOMOUT",
                                                          "Leading FD neutron momentum P^{reco}_{n} BPID - ZOOMOUT", "P^{reco}_{n} [GeV/c]",
                                                          directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                          "03c_P_LnFD_reco_BPID_1e_cut_FD_ZOOMOUT", 0,
                                                          15., numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_LnFD_vs_Theta_LnFD_reco_BPID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{LnFD} vs. #theta_{LnFD} BPID",
                                                                "P_{LnFD} vs. #theta_{LnFD} BPID",
                                                                "P_{LnFD} [GeV/c]", "#theta_{LnFD} [Deg]",
                                                                directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                                "03d_P_LnFD_vs_Theta_LnFD_reco_BPID_1e_cut_FD", Momentum_lboundary,
                                                                Momentum_uboundary,
                                                                Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    // all nFD (APID):
    hPlot1D hP_nFD_reco_APID_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD neutrons momentum APID", "FD neutrons momentum P^{reco}_{n} APID",
                                                 "P^{reco}_{n} [GeV/c]",
                                                 directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                 "03a_P_nFD_reco_APID_1e_cut_FD",
                                                 Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_reco_APID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD neutrons momentum APID - ZOOMIN",
                                                        "FD neutrons momentum P^{reco}_{n} APID - ZOOMIN",
                                                        "P^{reco}_{n} [GeV/c]",
                                                        directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                        "03b_P_nFD_reco_APID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_reco_APID_1e_cut_FD_ZOOMOUT = hPlot1D("1e cut", "FD", "Reco FD neutron momentum APID - ZOOMOUT",
                                                         "FD neutrons momentum P^{reco}_{n} APID - ZOOMOUT",
                                                         "P^{reco}_{n} [GeV/c]",
                                                         directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                         "03c_P_nFD_reco_APID_1e_cut_FD_ZOOMOUT", 0, 15., numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_nFD_vs_Theta_nFD_reco_APID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{nFD} vs. #theta_{nFD} APID", "P_{nFD} vs. #theta_{nFD} APID",
                                                              "P_{nFD} [GeV/c]",
                                                              "#theta_{nFD} [Deg]",
                                                              directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                              "03d_P_nFD_vs_Theta_nFD_reco_APID_1e_cut_FD", Momentum_lboundary, Momentum_uboundary,
                                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    // all nFD (BPID):
    hPlot1D hP_nFD_reco_BPID_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD neutrons momentum BPID", "FD neutrons momentum P^{reco}_{n} BPID",
                                                 "P^{reco}_{n} [GeV/c]",
                                                 directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                 "03a_P_nFD_reco_BPID_1e_cut_FD",
                                                 Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_reco_BPID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD neutrons momentum BPID - ZOOMIN",
                                                        "FD neutrons momentum P^{reco}_{n} BPID - ZOOMIN",
                                                        "P^{reco}_{n} [GeV/c]",
                                                        directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                        "03b_P_nFD_reco_BPID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_reco_BPID_1e_cut_FD_ZOOMOUT = hPlot1D("1e cut", "FD", "Reco FD neutron momentum BPID - ZOOMOUT",
                                                         "FD neutrons momentum P^{reco}_{n} BPID - ZOOMOUT",
                                                         "P^{reco}_{n} [GeV/c]",
                                                         directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                         "03c_P_nFD_reco_BPID_1e_cut_FD_ZOOMOUT", 0, 15., numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_nFD_vs_Theta_nFD_reco_BPID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{nFD} vs. #theta_{nFD} BPID", "P_{nFD} vs. #theta_{nFD} BPID",
                                                              "P_{nFD} [GeV/c]",
                                                              "#theta_{nFD} [Deg]",
                                                              directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                              "03d_P_nFD_vs_Theta_nFD_reco_BPID_1e_cut_FD", Momentum_lboundary, Momentum_uboundary,
                                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins * 2, numTH2Dbins * 2);

    hPlot1D hP_piplus_reco_1e_cut = hPlot1D("1e cut", "CD & FD", "Reco #pi^{+} momentum", "#pi^{+} momentum P^{reco}_{#pi^{+}}",
                                            "P^{reco}_{#pi^{+}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"], "04a_P_piplus_reco_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_reco_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "Reco #pi^{+} momentum - ZOOMIN",
                                                   "#pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN",
                                                   "P^{reco}_{#pi^{+}} [GeV/c]",
                                                   directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                   "04b_P_piplus_reco_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_reco_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD #pi^{+} momentum", "FD #pi^{+} momentum P^{reco}_{#pi^{+}}",
                                               "P^{reco}_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                               "04c_P_piplus_reco_1e_cut_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_reco_1e_cut_CD = hPlot1D("1e cut", "CD", "Reco CD #pi^{+} momentum", "CD #pi^{+} momentum P^{reco}_{#pi^{+}}",
                                               "P^{reco}_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                               "04d_P_piplus_reco_1e_cut_CD",
                                               CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #pi^{+} momentum - ZOOMIN",
                                                      "FD #pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN",
                                                      "P^{reco}_{#pi^{+}} [GeV/c]",
                                                      directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                      "04e_P_piplus_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_reco_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "Reco CD #pi^{+} momentum - ZOOMIN",
                                                      "CD #pi^{+} momentum P^{reco}_{#pi^{+}} - ZOOMIN",
                                                      "P^{reco}_{#pi^{+}} [GeV/c]",
                                                      directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                      "04f_P_piplus_reco_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_piplus_vs_Theta_piplus_reco_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{#pi^{+}} vs. #theta_{#pi^{+}}",
                                                               "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]",
                                                               "#theta_{#pi^{+}} [Deg]",
                                                               directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                               "04g_P_piplus_vs_Theta_piplus_reco_1e_cut_FD",
                                                               Momentum_lboundary, Momentum_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                               numTH2Dbins * 2,
                                                               numTH2Dbins * 2);
    hPlot2D hP_piplus_vs_Theta_piplus_reco_1e_cut_CD = hPlot2D("1e cut", "CD", "P_{#pi^{+}} vs. #theta_{#pi^{+}}",
                                                               "P_{#pi^{+}} vs. #theta_{#pi^{+}}", "P_{#pi^{+}} [GeV/c]",
                                                               "#theta_{#pi^{+}} [Deg]",
                                                               directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                               "04h_P_piplus_vs_Theta_piplus_reco_1e_cut_CD",
                                                               CDMomentum_lboundary, CDMomentum_uboundary, Theta_lboundary_CD, Theta_uboundary_CD,
                                                               numTH2Dbins * 2,
                                                               numTH2Dbins * 2);

    hPlot1D hP_piminus_reco_1e_cut = hPlot1D("1e cut", "CD & FD", "Reco #pi^{-} momentum", "#pi^{-} momentum P^{reco}_{#pi^{-}}",
                                             "P^{reco}_{#pi^{-}} [GeV/c]",
                                             directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"], "05a_P_piminus_reco_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_reco_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "Reco #pi^{-} momentum - ZOOMIN",
                                                    "#pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN",
                                                    "P^{reco}_{#pi^{-}} [GeV/c]",
                                                    directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                    "05b_P_piminus_reco_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_reco_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD #pi^{-} momentum", "FD #pi^{-} momentum P^{reco}_{#pi^{-}}",
                                                "P^{reco}_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                "05c_P_piminus_reco_1e_cut_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_reco_1e_cut_CD = hPlot1D("1e cut", "CD", "Reco CD #pi^{-} momentum", "CD #pi^{-} momentum P^{reco}_{#pi^{-}}",
                                                "P^{reco}_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                "05d_P_piminus_reco_1e_cut_CD",
                                                CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_reco_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #pi^{-} momentum - ZOOMIN",
                                                       "FD #pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN",
                                                       "P^{reco}_{#pi^{-}} [GeV/c]",
                                                       directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                       "05e_P_piminus_reco_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_reco_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "Reco CD #pi^{-} momentum - ZOOMIN",
                                                       "CD #pi^{-} momentum P^{reco}_{#pi^{-}} - ZOOMIN",
                                                       "P^{reco}_{#pi^{-}} [GeV/c]",
                                                       directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                       "05f_P_piminus_reco_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_piminus_vs_Theta_piminus_reco_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{#pi^{-}} vs. #theta_{#pi^{-}}",
                                                                 "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]",
                                                                 "#theta_{#pi^{-}} [Deg]",
                                                                 directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                                 "05g_P_piminus_vs_Theta_piminus_reco_1e_cut_FD",
                                                                 Momentum_lboundary, Momentum_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                                 numTH2Dbins * 2,
                                                                 numTH2Dbins * 2);
    hPlot2D hP_piminus_vs_Theta_piminus_reco_1e_cut_CD = hPlot2D("1e cut", "CD", "P_{#pi^{-}} vs. #theta_{#pi^{-}}",
                                                                 "P_{#pi^{-}} vs. #theta_{#pi^{-}}", "P_{#pi^{-}} [GeV/c]",
                                                                 "#theta_{#pi^{-}} [Deg]",
                                                                 directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                                 "05h_P_piminus_vs_Theta_piminus_reco_1e_cut_CD",
                                                                 CDMomentum_lboundary, CDMomentum_uboundary, Theta_lboundary_CD,
                                                                 Theta_uboundary_CD, numTH2Dbins * 2,
                                                                 numTH2Dbins * 2);

    hPlot1D hP_ph_reco_APID_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD #gamma momentum APID", "FD #gamma momentum P^{reco}_{#gamma} APID",
                                                "P^{reco}_{#gamma} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                "06a_P_ph_reco_APID_1e_cut_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_reco_APID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #gamma momentum APID - ZOOMIN",
                                                       "FD #gamma momentum P^{reco}_{#gamma} APID - ZOOMIN",
                                                       "P^{reco}_{#gamma} [GeV/c]",
                                                       directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                       "06b_P_ph_reco_APID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_reco_BPID_1e_cut_FD = hPlot1D("1e cut", "FD", "Reco FD #gamma momentum BPID", "FD #gamma momentum P^{reco}_{#gamma} BPID",
                                                "P^{reco}_{#gamma} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                "06a_P_ph_reco_BPID_1e_cut_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_reco_BPID_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "Reco FD #gamma momentum BPID - ZOOMIN",
                                                       "FD #gamma momentum P^{reco}_{#gamma} BPID - ZOOMIN",
                                                       "P^{reco}_{#gamma} [GeV/c]",
                                                       directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                       "06b_P_ph_reco_BPID_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot2D hP_ph_vs_Theta_ph_reco_APID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{#gamma} vs. #theta_{#gamma} APID",
                                                            "P_{#gamma} vs. #theta_{#gamma} APID",
                                                            "P_{#gamma} [GeV/c]", "#theta_{#gamma} [Deg]",
                                                            directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                            "06e_P_ph_vs_Theta_ph_reco_APID_1e_cut_FD", Momentum_lboundary,
                                                            Momentum_uboundary,
                                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    hPlot2D hP_ph_vs_Theta_ph_reco_BPID_1e_cut_FD = hPlot2D("1e cut", "FD", "P_{#gamma} vs. #theta_{#gamma} BPID",
                                                            "P_{#gamma} vs. #theta_{#gamma} BPID",
                                                            "P_{#gamma} [GeV/c]", "#theta_{#gamma} [Deg]",
                                                            directories.Momentum_Directory_map["Momentum_th_reco_1e_cut_Directory"],
                                                            "06e_P_ph_vs_Theta_ph_reco_BPID_1e_cut_FD", Momentum_lboundary,
                                                            Momentum_uboundary,
                                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins * 2, numTH2Dbins * 2);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1p)">
    hPlot1D hP_e_APID_1p_FD = hPlot1D("1p", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "01a_P_e_APID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_e_BPID_1p_FD = hPlot1D("1p", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "01b_P_e_BPID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_p_APID_1p_CD = hPlot1D("1p", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "02a_P_p_APID_1p_CD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APIDandPS_1p_CD = hPlot1D("1p", "CD", "Proton momentum APID&PS", "Proton momentum P_{p} APID&PS", "P_{p} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1p_Directory"], "02b_P_p_APIDandPS_1p_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_1p_CD = hPlot1D("1p", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "02c_P_p_BPID_1p_CD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APID_1p_FD = hPlot1D("1p", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "02d_P_p_APID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APIDandPS_1p_FD = hPlot1D("1p", "FD", "Proton momentum APID&PS", "Proton momentum P_{p} APID&PS", "P_{p} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1p_Directory"], "02e_P_p_APIDandPS_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_1p_FD = hPlot1D("1p", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "02f_P_p_BPID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_pFD_APID_1p = hPlot1D("1p", "FD", "FD proton momentum APID", "FD proton momentum APID", "P_{p} [GeV/c]",
                                     directories.Momentum_Directory_map["Analysis_plots_momentum_1p_Directory"], "01a_P_pFD_APID_1p_FD",
                                     Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_pFD_APIDandPS_1p = hPlot1D("1p", "FD", "FD proton momentum APID&PS", "FD proton momentum APID&PS", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Analysis_plots_momentum_1p_Directory"], "01b_P_pFD_APIDandPS_1p_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piplus_APID_1p_CD = hPlot1D("1p", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1p_Directory"], "03a_P_piplus_APID_1p_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_1p_CD = hPlot1D("1p", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1p_Directory"], "03b_P_piplus_BPID_1p_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_APID_1p_FD = hPlot1D("1p", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1p_Directory"], "03c_P_piplus_APID_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_1p_FD = hPlot1D("1p", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1p_Directory"], "03d_P_piplus_BPID_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piminus_APID_1p_CD = hPlot1D("1p", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1p_Directory"], "04a_P_piminus_APID_1p_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_1p_CD = hPlot1D("1p", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1p_Directory"], "04b_P_piminus_BPID_1p_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_APID_1p_FD = hPlot1D("1p", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1p_Directory"], "04c_P_piminus_APID_1p_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_1p_FD = hPlot1D("1p", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1p_Directory"], "04d_P_piminus_BPID_1p_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_ph_APID_1p_FD = hPlot1D("1p", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_1p_Directory"], "05c_P_ph_APID_1p_FD",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_ph_BPID_1p_FD = hPlot1D("1p", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_1p_Directory"], "05d_P_ph_BPID_1p_FD",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_n_APID_1p_FD = hPlot1D("1p", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "06c_P_n_APID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_BPID_1p_FD = hPlot1D("1p", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1p_Directory"], "06d_P_n_BPID_1p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1n)">
    hPlot1D hP_e_APID_1n_FD = hPlot1D("1n", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "01a_P_e_APID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_e_BPID_1n_FD = hPlot1D("1n", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "01b_P_e_BPID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_n_APID_1n_FD = hPlot1D("1n", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "02a_P_n_APID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_APID_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum APID - ZOOMOUT", "Neutron momentum P_{n} APID - ZOOMOUT",
                                              "P_{n} [GeV/c]",
                                              directories.Momentum_Directory_map["Momentum_1n_Directory"], "02b_P_n_APID_1n_ZOOMOUT_FD",
                                              Momentum_lboundary, 15., numTH1Dbins);
    hPlot1D hP_n_BPID_1n_FD = hPlot1D("1n", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "02c_P_n_BPID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_BPID_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum BPID - ZOOMOUT", "Neutron momentum P_{n} BPID - ZOOMOUT",
                                              "P_{n} [GeV/c]",
                                              directories.Momentum_Directory_map["Momentum_1n_Directory"], "02d_P_n_BPID_1n_ZOOMOUT_FD",
                                              Momentum_lboundary, 15., numTH1Dbins);
    hPlot1D hP_n_APIDandNS_1n_FD = hPlot1D("1n", "FD", "Neutron momentum APID&NC", "Neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1n_Directory"], "02e_P_n_APIDandNS_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_APIDandNS_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron momentum APID&NC - ZOOMOUT", "Neutron momentum P_{n} APID&NC - ZOOMOUT",
                                                   "P_{n} [GeV/c]",
                                                   directories.Momentum_Directory_map["Momentum_1n_Directory"], "02f_P_n_APIDandNS_1n_ZOOMOUT_FD",
                                                   Momentum_lboundary, 15., numTH1Dbins);

    hPlot1D hP_nFD_APID_1n = hPlot1D("1n", "FD", "Leading FD neutron momentum APID", "Leading FD neutron momentum APID", "P_{n} [GeV/c]",
                                     directories.Momentum_Directory_map["Analysis_plots_momentum_1n_Directory"], "01a_P_nFD_APID_1n_FD",
                                     Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_nFD_APIDandNS_1n = hPlot1D("1n", "FD", "Leading FD neutron momentum APID&NC", "Leading FD neutron momentum APID&NC",
                                          "P_{n} [GeV/c]",
                                          directories.Momentum_Directory_map["Analysis_plots_momentum_1n_Directory"], "01b_P_nFD_APIDandNS_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_nFD_APID_1n_ZOOMOUT = hPlot1D("1n", "FD", "Leading FD neutron momentum APID - ZOOMOUT",
                                             "Leading FD neutron momentum APID - ZOOMOUT", "P_{n} [GeV/c]",
                                             directories.Momentum_Directory_map["Analysis_plots_momentum_1n_Directory"],
                                             "01c_P_nFD_APID_1n_ZOOMOUT_FD",
                                             Momentum_lboundary, 15., numTH1Dbins);
    hPlot1D hP_nFD_APIDandNS_1n_ZOOMOUT = hPlot1D("1n", "FD", "Leading FD neutron momentum APID&NC - ZOOMOUT",
                                                  "Leading FD neutron momentum APID&NC - ZOOMOUT", "P_{n} [GeV/c]",
                                                  directories.Momentum_Directory_map["Analysis_plots_momentum_1n_Directory"],
                                                  "01d_P_nFD_APIDandNS_1n_ZOOMOUT_FD",
                                                  Momentum_lboundary, 15., numTH1Dbins);

    hPlot1D hP_p_APID_1n_CD = hPlot1D("1n", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "03a_P_p_APID_1n_CD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_1n_CD = hPlot1D("1n", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "03b_P_p_BPID_1n_CD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APID_1n_FD = hPlot1D("1n", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "03c_P_p_APID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_1n_FD = hPlot1D("1n", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_1n_Directory"], "03d_P_p_BPID_1n_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piplus_APID_1n_CD = hPlot1D("1n", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1n_Directory"], "04a_P_piplus_APID_1n_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_1n_CD = hPlot1D("1n", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1n_Directory"], "04b_P_piplus_BPID_1n_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_APID_1n_FD = hPlot1D("1n", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1n_Directory"], "04c_P_piplus_APID_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_1n_FD = hPlot1D("1n", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_1n_Directory"], "04d_P_piplus_BPID_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piminus_APID_1n_CD = hPlot1D("1n", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1n_Directory"], "05a_P_piminus_APID_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_1n_CD = hPlot1D("1n", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1n_Directory"], "05b_P_piminus_BPID_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_APID_1n_FD = hPlot1D("1n", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1n_Directory"], "05c_P_piminus_APID_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_1n_FD = hPlot1D("1n", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_1n_Directory"], "05d_P_piminus_BPID_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_n_VN_BPID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from v. neutrons) BPID", "Neutron Momentum (from v. neutrons) BPID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_1n_Directory"], "06a_P_n_VNeutrons_BPID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_VN_APID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from v. neutrons) APID", "Neutron Momentum (from v. neutrons) APID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_1n_Directory"], "06b_P_n_VNeutrons_APID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_Ph_BPID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from 'Photons') BPID", "Neutron Momentum (from 'Photons') BPID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_1n_Directory"], "07a_P_n_From_Photons_BPID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_Ph_APID_1n_FD = hPlot1D("1n", "FD", "P_{n} (from 'Photons') APID", "Neutron Momentum (from 'Photons') APID", "P_{n} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_1n_Directory"], "07b_P_n_From_Photons_APID_1n_FD",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_ph_APID_1n_FD = hPlot1D("1n", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_1n_Directory"], "08a_P_ph_APID_1n_FD",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_ph_BPID_1n_FD = hPlot1D("1n", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_1n_Directory"], "08b_P_ph_BPID_1n_FD",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (2p)">

    //<editor-fold desc="Momentum threshold plots (2p)">
    hPlot1D hP_e_APID_2p_FD = hPlot1D("2p", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "01a_P_e_APID_2p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_e_BPID_2p_FD = hPlot1D("2p", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "01b_P_e_BPID_2p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_p_APID_2p_CD = hPlot1D("2p", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "02a_P_p_APID_2p_CD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_2p_CD = hPlot1D("2p", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "02b_P_p_BPID_2p_CD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APID_2p_FD = hPlot1D("2p", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "03c_P_p_APID_2p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_2p_FD = hPlot1D("2p", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "03d_P_p_BPID_2p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piplus_APID_2p_CD = hPlot1D("2p", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_2p_Directory"], "04a_P_piplus_APID_2p_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_2p_CD = hPlot1D("2p", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_2p_Directory"], "04b_P_piplus_BPID_2p_CD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_APID_2p_FD = hPlot1D("2p", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_2p_Directory"], "04c_P_piplus_APID_2p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_2p_FD = hPlot1D("2p", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_2p_Directory"], "04d_P_piplus_BPID_2p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piminus_APID_2p_CD = hPlot1D("2p", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_2p_Directory"], "05a_P_piminus_APID_2p_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_2p_CD = hPlot1D("2p", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_2p_Directory"], "05b_P_piminus_BPID_2p_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_APID_2p_FD = hPlot1D("2p", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_2p_Directory"], "05c_P_piminus_APID_2p_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_2p_FD = hPlot1D("2p", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID", "P_{#pi^{-}} [GeV/c]",
                                            directories.Momentum_Directory_map["Momentum_2p_Directory"], "05d_P_piminus_BPID_2p_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_ph_APID_2p_FD = hPlot1D("2p", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_2p_Directory"], "06c_P_ph_APID_2p_FD",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_ph_BPID_2p_FD = hPlot1D("2p", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                       directories.Momentum_Directory_map["Momentum_2p_Directory"], "06d_P_ph_BPID_2p_FD",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_n_APID_2p_FD = hPlot1D("2p", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "07c_P_n_APID_2p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_BPID_2p_FD = hPlot1D("2p", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                      directories.Momentum_Directory_map["Momentum_2p_Directory"], "07d_P_n_BPID_2p_FD",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Leading (P_1) and recoil (P_2) Momentum threshold plots (2p)">
    hPlot1D hP_p_1_2p = hPlot1D("2p", "", "Leading proton P_{1}", "Leading proton momentum P_{1}", "P_{1} [GeV/c]",
                                directories.Momentum_Directory_map["Momentum_2p_Directory"], "07_P_p_1_2p", Momentum_lboundary, Momentum_uboundary,
                                numTH1Dbins);
    hPlot1D hP_p_2_2p = hPlot1D("2p", "", "Recoil proton P_{2}", "Recoil proton momentum P_{2}", "P_{2} [GeV/c]",
                                directories.Momentum_Directory_map["Momentum_2p_Directory"], "07_P_p_2_2p", Momentum_lboundary, Momentum_uboundary,
                                numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="P1 vs P2 (2p, CD & FD)">
    hPlot2D hP_p_1_vs_P_p_2_2p = hPlot2D("2p", "", "P_{1} vs. P_{2}", "P_{1} vs. P_{2}", "P_{1} [GeV/c]", "P_{2} [GeV/c]",
                                         directories.Momentum_Directory_map["Momentum_2p_Directory"], "08_P_p_1_vs_P_p_2",
                                         Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (pFDpCD)">
    hPlot1D hP_e_APID_pFDpCD_FD = hPlot1D("pFDpCD", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "01a_P_e_APID_pFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_e_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "01b_P_e_BPID_pFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_p_APID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "02a_P_p_APID_pFDpCD_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "02b_P_p_BPID_pFDpCD_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "03c_P_p_APID_pFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APIDandPS_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Proton momentum APID&PS", "Proton momentum P_{p} APID&PS", "P_{p} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "03d_P_p_APIDandPS_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "03e_P_p_BPID_pFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piplus_APID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "04a_P_piplus_APID_pFDpCD_CD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "04b_P_piplus_BPID_pFDpCD_CD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "04c_P_piplus_APID_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "04d_P_piplus_BPID_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piminus_APID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "05a_P_piminus_APID_pFDpCD_CD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_pFDpCD_CD = hPlot1D("pFDpCD", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "05b_P_piminus_BPID_pFDpCD_CD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "05c_P_piminus_APID_pFDpCD_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "05d_P_piminus_BPID_pFDpCD_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_ph_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "06c_P_ph_APID_pFDpCD_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_ph_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "06d_P_ph_BPID_pFDpCD_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_n_APID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "07c_P_n_APID_pFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_BPID_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_pFDpCD_Directory"], "07d_P_n_BPID_pFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (nFDpCD)">
    hPlot1D hP_e_APID_nFDpCD_FD = hPlot1D("nFDpCD", "", "Electron momentum APID", "Electron momentum P_{e} APID", "P_{e} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "01a_P_e_APID_nFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_e_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "", "Electron momentum BPID", "Electron momentum P_{e} BPID", "P_{e} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "01b_P_e_BPID_nFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_p_APID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "02a_P_p_APID_nFDpCD_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "02b_P_p_BPID_nFDpCD_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Proton momentum APID", "Proton momentum P_{p} APID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "03c_P_p_APID_nFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_p_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Proton momentum BPID", "Proton momentum P_{p} BPID", "P_{p} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "03d_P_p_BPID_nFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piplus_APID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "04a_P_piplus_APID_nFDpCD_CD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "04b_P_piplus_BPID_nFDpCD_CD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{+} momentum APID", "#pi^{+} momentum P_{#pi^{+}} APID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "04c_P_piplus_APID_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piplus_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{+} momentum BPID", "#pi^{+} momentum P_{#pi^{+}} BPID", "P_{#pi^{+}} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "04d_P_piplus_BPID_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_piminus_APID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "05a_P_piminus_APID_nFDpCD_CD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_nFDpCD_CD = hPlot1D("nFDpCD", "CD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "05b_P_piminus_BPID_nFDpCD_CD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{-} momentum APID", "#pi^{-} momentum P_{#pi^{-}} APID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "05c_P_piminus_APID_nFDpCD_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_piminus_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "#pi^{-} momentum BPID", "#pi^{-} momentum P_{#pi^{-}} BPID",
                                                "P_{#pi^{-}} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "05d_P_piminus_BPID_nFDpCD_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_ph_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Photon momentum APID", "Photon momentum P_{#gamma} APID", "P_{#gamma} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "06c_P_ph_APID_nFDpCD_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_ph_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Photon momentum BPID", "Photon momentum P_{#gamma} BPID", "P_{#gamma} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "06d_P_ph_BPID_nFDpCD_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_n_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Neutron momentum APID", "Neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "07a_P_n_APID_nFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_APIDandNS_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Neutron momentum APID&NC", "Neutron momentum P_{n} APID&NC", "P_{n} [GeV/c]",
                                               directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "07b_P_n_APIDandNS_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_n_BPID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Neutron momentum BPID", "Neutron momentum P_{n} BPID", "P_{n} [GeV/c]",
                                          directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "07c_P_n_BPID_nFDpCD_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hP_nL_APID_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Leading neutron momentum APID", "Leading neutron momentum P_{n} APID", "P_{n} [GeV/c]",
                                           directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "07a_P_n_APID_nFDpCD_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_nL_APIDandNS_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "Leading neutron momentum APID&NC", "Leading neutron momentum P_{n} APID&NC",
                                                "P_{n} [GeV/c]",
                                                directories.Momentum_Directory_map["Momentum_nFDpCD_Directory"], "07b_P_n_APIDandNS_nFDpCD_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Other momentum plots">

    //<editor-fold desc="pFD and pCD momentum plots (pFDpCD)">
    hPlot1D hP_pFD_pFDpCD = hPlot1D("pFDpCD", "", "FD proton momentum", "FD proton momentum P_{pFD}", "P_{pFD} [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "01a_P_pFD_pFDpCD",
                                    Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_pCD_pFDpCD = hPlot1D("pFDpCD", "", "CD proton momentum", "CD proton momentum P_{pCD}", "P_{pCD} [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "01b_P_pCD_pFDpCD",
                                    CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_pFD_vs_P_pCD_pFDpCD = hPlot2D("pFDpCD", "", "P_{pFD} vs. P_{pCD}", "P_{pFD} vs. P_{pCD}", "P_{pFD} [GeV/c]", "P_{pCD} [GeV/c]",
                                             directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "05_P_pFD_vs_P_pCD",
                                             P_nucFD_lboundary, P_nucFD_uboundary, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_Mom_Plots,
                                             numTH2Dbins_Mom_Plots);
//                                             Momentum_lboundary, Momentum_uboundary, CDMomentum_lboundary, CDMomentum_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="nFD and pCD momentum plots (nFDpCD)">
    hPlot1D hP_nFD_nFDpCD = hPlot1D("nFDpCD", "", "FD neutron momentum", "FD neutron momentum P_{nFD}", "P_{nFD} [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "01a_P_nFD_nFDpCD",
                                    Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_pCD_nFDpCD = hPlot1D("nFDpCD", "", "CD proton momentum", "CD proton momentum P_{pCD}", "P_{pCD} [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "01b_P_pCD_nFDpCD",
                                    CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_nFD_vs_P_pCD_nFDpCD = hPlot2D("nFDpCD", "", "P_{nFD} vs. P_{pCD}", "P_{nFD} vs. P_{pCD}", "P_{nFD} [GeV/c]", "P_{pCD} [GeV/c]",
                                             directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "05_P_nFD_vs_P_pCD",
                                             P_nucFD_lboundary, P_nucFD_uboundary, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_Mom_Plots,
                                             numTH2Dbins_Mom_Plots);
//                                             Momentum_lboundary, Momentum_uboundary, CDMomentum_lboundary, CDMomentum_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="Total and relative nucleon momenta (pFDpCD)">
    hPlot1D hP_tot_pFDpCD = hPlot1D("pFDpCD", "", "Total nucleon momentum (3-vector)", "Total nucleon 3-momentum",
                                    "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}| [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                    "02a_P_tot_pFDpCD", Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_rel_pFDpCD = hPlot1D("pFDpCD", "", "Relative nucleon momentum (3-vector)", "Relative nucleon 3-momentum",
                                    "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2 [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                    "02b_P_rel_pFDpCD", CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_tot_vs_P_rel_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|",
                                             "|#vec{P}_{tot}| = |#vec{P}_{pL} + #vec{P}_{pR}| [GeV/c]",
                                             "|#vec{P}_{rel}| = |#vec{P}_{pL} - #vec{P}_{pR}|/2 [GeV/c]",
                                             directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "02c_P_tot_vs_P_rel",
                                             Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins,
                                             numTH2Dbins);

    hPlot1D hP_tot_mu_pFDpCD = hPlot1D("pFDpCD", "", "Total nucleon momentum (4-vector)", "Total nucleon 4-momentum",
                                       "P_{tot}^{#mu} = (P_{pL}^{#mu} + P_{pR}^{#mu}) [GeV/c]",
                                       directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                       "02d_P_tot_pFDpCD", Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_rel_mu_pFDpCD = hPlot1D("pFDpCD", "", "Relative nucleon momentum (4-vector)", "Relative nucleon 4-momentum",
                                       "P_{rel}^{#mu} = (P_{pL}^{#mu} - P_{pR}^{#mu})/2 [GeV/c]",
                                       directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                       "02e_P_rel_pFDpCD", -CDMomentum_uboundary, CDMomentum_lboundary, numTH1Dbins);
//                                       "02e_P_rel_pFDpCD", CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_tot_mu_vs_P_rel_mu_pFDpCD = hPlot2D("pFDpCD", "", "P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} vs. P_{rel}^{#mu}",
                                                   "P_{tot}^{#mu} = P_{pL}^{#mu} + P_{pR}^{#mu} [GeV/c]",
                                                   "P_{rel}^{#mu} = (P_{pL}^{#mu} - P_{pR}^{#mu})/2 [GeV/c]",
                                                   directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                                   "02f_P_tot_vs_P_rel",
                                                   Momentum_lboundary, Momentum_uboundary, -CDMomentum_uboundary, CDMomentum_lboundary,
                                                   numTH2Dbins, numTH2Dbins);
//                                                   Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);

    hPlot1D hP_tot_minus_q_pFDpCD = hPlot1D("pFDpCD", "", "#vec{P}_{tot}-#vec{q} distribution", "#vec{P}_{tot}-#vec{q} distribution",
                                            "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{pL} + #vec{P}_{pR}- #vec{q}| [GeV/c]",
                                            directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                            "06_P_tot_minus_q_pFDpCD", Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot2D hP_tot_minus_q_vs_P_tot_pFDpCD = hPlot2D("pFDpCD", "", "#vec{P}_{tot}-#vec{q} vs. #vec{P}_{tot}",
                                                     "#vec{P}_{tot}-#vec{q} vs. #vec{P}_{tot}",
                                                     "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}| [GeV/c]",
                                                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                                     "07a_P_tot_minus_q_vs_P_tot_pFDpCD",
                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins,
                                                     numTH2Dbins);
    hPlot2D hP_tot_minus_q_vs_q_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}|",
                                                 "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}|",
                                                 "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
                                                 directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
                                                 "07b_P_tot_minus_q_vs_q_pFDpCD",
                                                 Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins,
                                                 numTH2Dbins);

//    hPlot2D hP_tot_minus_q_vs_q_S1_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S1",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 0#leqP_{tot}<0.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S1",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S2_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S2",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 0.5#leqP_{tot}<1.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S2",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S3_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S3",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 1.0#leqP_{tot}<1.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S3",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S4_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S4",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 1.5#leqP_{tot}<2.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S4",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S5_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S5",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 2.0#leqP_{tot}<2.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S5",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S6_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S6",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 2.5#leqP_{tot}<3.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S6",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S7_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S7",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 3.0#leqP_{tot}<3.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S7",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S8_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S8",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 3.5#leqP_{tot}<4.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S8",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S9_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S9",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 4.0#leqP_{tot}<4.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_pFDpCD_S9",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S10_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S10",
//                                                     "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 4.5#leqP_{tot}<5.0 [GeV/C]",
//                                                     "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                     "08_P_tot_minus_q_vs_q_pFDpCD_S10",
//                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S11_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S11",
//                                                     "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 5.0#leqP_{tot}<5.5 [GeV/C]",
//                                                     "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                     "08_P_tot_minus_q_vs_q_pFDpCD_S11",
//                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S12_pFDpCD = hPlot2D("pFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S12",
//                                                     "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 5.5#leqP_{tot}<6.0 [GeV/C]",
//                                                     "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"],
//                                                     "08_P_tot_minus_q_vs_q_pFDpCD_S12",
//                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="Total and Relative nucleon momenta (nFDpCD)">
    hPlot1D hP_tot_nFDpCD = hPlot1D("nFDpCD", "", "Total nucleon momentum (3-vector)", "Total nucleon 3-momentum",
                                    "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}| [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                    "02a_P_tot_nFDpCD", Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_rel_nFDpCD = hPlot1D("nFDpCD", "", "Relative nucleon momentum (3-vector)", "Relative nucleon 3-momentum",
                                    "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2 [GeV/c]",
                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                    "02b_P_rel_nFDpCD", CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_tot_vs_P_rel_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|",
                                             "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}| [GeV/c]",
                                             "|#vec{P}_{rel}| = |#vec{P}_{nL} - #vec{P}_{nR}|/2 [GeV/c]",
                                             directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "02c_P_tot_vs_P_rel",
                                             Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins,
                                             numTH2Dbins);

    hPlot1D hP_tot_mu_nFDpCD = hPlot1D("nFDpCD", "", "Total nucleon momentum (4-vector)", "Total nucleon 4-momentum",
                                       "P_{tot}^{#mu} = (P_{nL}^{#mu} + P_{nR}^{#mu}) [GeV/c]",
                                       directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                       "02d_P_tot_nFDpCD", Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_rel_mu_nFDpCD = hPlot1D("nFDpCD", "", "Relative nucleon momentum (4-vector)", "Relative nucleon 4-momentum",
                                       "P_{rel}^{#mu} = (P_{nL}^{#mu} - P_{nR}^{#mu})/2 [GeV/c]",
                                       directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                       "02e_P_rel_nFDpCD", -CDMomentum_uboundary, CDMomentum_lboundary, numTH1Dbins);
//                                       "02e_P_rel_nFDpCD", CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_tot_mu_vs_P_rel_mu_nFDpCD = hPlot2D("nFDpCD", "", "P_{tot}^{#mu} vs. P_{rel}^{#mu}", "P_{tot}^{#mu} vs. P_{rel}^{#mu}",
                                                   "P_{tot}^{#mu} = P_{nL}^{#mu} + P_{nR}^{#mu} [GeV/c]",
                                                   "P_{rel}^{#mu} = (P_{nL}^{#mu} - P_{nR}^{#mu})/2 [GeV/c]",
                                                   directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                                   "02f_P_tot_vs_P_rel",
                                                   Momentum_lboundary, Momentum_uboundary, -CDMomentum_uboundary, CDMomentum_lboundary,
                                                   numTH2Dbins, numTH2Dbins);

    hPlot1D hP_tot_minus_q_nFDpCD = hPlot1D("nFDpCD", "", "#vec{P}_{tot}-#vec{q} distribution", "#vec{P}_{tot}-#vec{q} distribution",
                                            "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
                                            directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                            "06_P_tot_minus_q_nFDpCD", Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot2D hP_tot_minus_q_vs_P_tot_nFDpCD = hPlot2D("nFDpCD", "", "#vec{P}_{tot}-#vec{q} vs. #vec{P}_{tot}",
                                                     "#vec{P}_{tot}-#vec{q} vs. #vec{P}_{tot}",
                                                     "|#vec{P}_{tot}| = |#vec{P}_{nL} + #vec{P}_{nR}| [GeV/c]",
                                                     "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                                     "07a_P_tot_minus_q_vs_P_tot_nFDpCD",
                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins,
                                                     numTH2Dbins);
    hPlot2D hP_tot_minus_q_vs_q_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}|",
                                                 "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}|",
                                                 "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
                                                 directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
                                                 "07b_P_tot_minus_q_vs_q_nFDpCD",
                                                 Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins,
                                                 numTH2Dbins);

//    hPlot2D hP_tot_minus_q_vs_q_S1_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S1",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 0#leqP_{tot}<0.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S1",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S2_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S2",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 0.5#leqP_{tot}<1.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S2",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S3_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S3",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 1.0#leqP_{tot}<1.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S3",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S4_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S4",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 1.5#leqP_{tot}<2.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S4",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S5_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S5",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 2.0#leqP_{tot}<2.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S5",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S6_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S6",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 2.5#leqP_{tot}<3.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S6",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S7_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S7",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 3.0#leqP_{tot}<3.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S7",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S8_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S8",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 3.5#leqP_{tot}<4.0 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S8",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S9_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S9",
//                                                    "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 4.0#leqP_{tot}<4.5 [GeV/C]",
//                                                    "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                    directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                    "08_P_tot_minus_q_vs_q_nFDpCD_S9",
//                                                    Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S10_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S10",
//                                                     "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 4.5#leqP_{tot}<5.0 [GeV/C]",
//                                                     "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                     "08_P_tot_minus_q_vs_q_nFDpCD_S10",
//                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S11_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S11",
//                                                     "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 5.0#leqP_{tot}<5.5 [GeV/C]",
//                                                     "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                     "08_P_tot_minus_q_vs_q_nFDpCD_S11",
//                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
//    hPlot2D hP_tot_minus_q_vs_q_S12_nFDpCD = hPlot2D("nFDpCD", "", "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| S12",
//                                                     "|#vec{P}_{tot} - #vec{q}| vs. |#vec{q}| for 5.5#leqP_{tot}<6.0 [GeV/C]",
//                                                     "|#vec{q}| [GeV/c]", "|#vec{P}_{tot} - #vec{q}| = |#vec{P}_{nL} + #vec{P}_{nR}- #vec{q}| [GeV/c]",
//                                                     directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"],
//                                                     "08_P_tot_minus_q_vs_q_nFDpCD_S12",
//                                                     Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="Leading and recoil nucleon momentum plots (pFDpCD)">
    hPlot1D hP_pL_pFDpCD = hPlot1D("pFDpCD", "", "Leading proton momentum", "Leading proton momentum", "P_{pL} [GeV/c]",
                                   directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "03a_P_pL_pFDpCD",
                                   Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_pR_pFDpCD = hPlot1D("pFDpCD", "", "Recoil proton momentum", "Recoil proton momentum", "P_{pR} [GeV/c]",
                                   directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "03b_P_pR_pFDpCD",
                                   CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_pL_vs_P_pR_pFDpCD = hPlot2D("pFDpCD", "", "P_{pL} vs. P_{pR}", "P_{pL} vs. P_{pR}", "P_{pL} [GeV/c]", "P_{pR} [GeV/c]",
                                           directories.Momentum_Directory_map["Analysis_plots_momentum_pFDpCD_Directory"], "04_P_pL_vs_P_pR",
                                           P_nucFD_lboundary, P_nucFD_uboundary, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_Mom_Plots,
                                           numTH2Dbins_Mom_Plots);
//                                           Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //<editor-fold desc="Leading and recoil nucleon momentum plots (nFDpCD)">
    hPlot1D hP_nL_nFDpCD = hPlot1D("nFDpCD", "", "Leading nucleon momentum", "Leading nucleon momentum", "P_{nL} [GeV/c]",
                                   directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "03a_P_nL_nFDpCD",
                                   Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hP_nR_nFDpCD = hPlot1D("nFDpCD", "", "Recoil nucleon momentum", "Recoil nucleon momentum", "P_{nR} [GeV/c]",
                                   directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "03b_P_nR_nFDpCD",
                                   CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins);
    hPlot2D hP_nL_vs_P_nR_nFDpCD = hPlot2D("nFDpCD", "", "P_{nL} vs. P_{nR}", "P_{nL} vs. P_{nR}", "P_{nL} [GeV/c]", "P_{nR} [GeV/c]",
                                           directories.Momentum_Directory_map["Analysis_plots_momentum_nFDpCD_Directory"], "04_P_nL_vs_P_nR",
                                           P_nucFD_lboundary, P_nucFD_uboundary, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_Mom_Plots,
                                           numTH2Dbins_Mom_Plots);
//                                           Momentum_lboundary, Momentum_uboundary, Momentum_lboundary, Momentum_uboundary, numTH2Dbins, numTH2Dbins);
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
    hPlot1D hBeta_n_from_ph_01_1n_FD = hPlot1D("1n", "FD", "#beta of n from '#gamma'", "Neutron #beta from 'photons'", "#beta",
                                               directories.Beta_Directory_map["Neutron_beta_from_ph_01_1n_Directory"],
                                               "01_Beta_Neutron_from_photons_1n",
                                               0.98, Beta_dist_uboundary, numTH1Dbins_Beta_Plots);
    hPlot1D hBeta_n_from_ph_01_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "#beta of n from '#gamma' - ZOOMOUT", "Neutron #beta from 'photons' - ZOOMOUT",
                                                       "#beta",
                                                       directories.Beta_Directory_map["Neutron_beta_from_ph_01_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, numTH1Dbins_Beta_Plots);

    hPlot1D hBeta_n_from_ph_02_1n_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL", "Neutron #beta from 'photons' w/o PCAL hit",
                                               "#beta",
                                               directories.Beta_Directory_map["Neutron_beta_from_ph_02_1n_Directory"],
                                               "01_Beta_Neutron_from_photons_noPCAL_1n",
                                               Beta_dist_lboundary, Beta_dist_uboundary, numTH1Dbins_Beta_Plots);
    hPlot1D hBeta_n_from_ph_02_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL - ZOOMOUT",
                                                       "Neutron #beta from 'photons' w/o PCAL hit - ZOOMOUT",
                                                       "#beta", directories.Beta_Directory_map["Neutron_beta_from_ph_02_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_noPCAL_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, numTH1Dbins_Beta_Plots);

    hPlot1D hBeta_n_from_ph_03_1n_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & ECIN",
                                               "Neutron #beta from 'photons' w/o PCAL & w/ECIN", "#beta",
                                               directories.Beta_Directory_map["Neutron_beta_from_ph_03_1n_Directory"],
                                               "01_Beta_Neutron_from_photons_noPCAL_yesECIN_1n",
                                               Beta_dist_lboundary, Beta_dist_uboundary, numTH1Dbins_Beta_Plots);
    hPlot1D hBeta_n_from_ph_03_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & ECIN - ZOOMOUT",
                                                       "Neutron #beta from 'photons' w/o PCAL & w/ECIN - ZOOMOUT", "#beta",
                                                       directories.Beta_Directory_map["Neutron_beta_from_ph_03_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_noPCAL_yesECIN_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, numTH1Dbins_Beta_Plots);

    hPlot1D hBeta_n_from_ph_04_1n_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & !ECIN & ECOUT",
                                               "Neutron #beta from 'photons' w/o PCAL/ECIN & w/ECOUT",
                                               "#beta", directories.Beta_Directory_map["Neutron_beta_from_ph_04_1n_Directory"],
                                               "01_Beta_Neutron_from_photons_noPCAL_noECIN_yesECOUT_1n",
                                               Beta_dist_lboundary, Beta_dist_uboundary, numTH1Dbins_Beta_Plots);
    hPlot1D hBeta_n_from_ph_04_1n_ZOOMOUT_FD = hPlot1D("1n", "FD", "Neutron #beta from 'photons' - !PCAL & !ECIN & ECOUT - ZOOMOUT",
                                                       "Neutron #beta from 'photons' w/o PCAL/ECIN & w/ECOUT - ZOOMOUT", "#beta",
                                                       directories.Beta_Directory_map["Neutron_beta_from_ph_04_1n_Directory"],
                                                       "01_Beta_Neutron_from_photons_noPCAL_noECIN_yesECOUT_1n_ZOOMOUT",
                                                       Beta_dist_ZOOMOUT_lboundary, Beta_dist_ZOOMOUT_uboundary, numTH1Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P plots">

    //<editor-fold desc="Beta vs. P (no #(e) cut)">

    //<editor-fold desc="Beta vs. P (no #(e) cut)">
    hPlot2D hBeta_vs_P_CD = hPlot2D("all particles", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                    directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "01_Beta_vs_P_All_Particles_CD",
                                    0, beamE * 1.425, 0, Beta_boundary_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_FD = hPlot2D("all particles", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                    directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "01_Beta_vs_P_All_Particles_FD",
                                    0, beamE * 1.425, 0, Beta_boundary_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_Electrons_Only_FD = hPlot2D("electrons only", "no #(e) cut", "", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "02_Beta_vs_P_Electrons_Only_FD",
                                                   0, beamE * 1.425, 0.5, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_Protons_Only_CD = hPlot2D("protons only", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                 directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "03_Beta_vs_P_Protons_Only_CD",
                                                 0, beamE * 1.1, 0, 1.1, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_Protons_Only_FD = hPlot2D("protons only", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                 directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "03_Beta_vs_P_Protons_Only_FD",
                                                 0, beamE * 1.1, 0, 1.1, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_Neutrons_Only_CD = hPlot2D("neutrons only", "no #(e) cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "04_Beta_vs_P_Neutrons_Only_CD",
                                                  0, beamE * 1.1, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_Neutrons_Only_FD = hPlot2D("neutrons only", "no #(e) cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_All_e_Directory"], "04_Beta_vs_P_Neutrons_Only_FD",
                                                  0, beamE * 1.1, 0, 3, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (no #(e) cut)">
    hPlot2D hBeta_vs_P_positive_part_All_e_CD = hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = +1",
                                                        "#beta vs. P for all particles with q = +1",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "01_Beta_vs_P_q_p1_All_e_CD", 0, beamE * 1.425, 0, 3, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_positive_part_All_e_FD = hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = +1",
                                                        "#beta vs. P for all particles with q = +1",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "01_Beta_vs_P_q_p1_All_e_FD", 0, beamE * 1.425, 0, 3, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_All_e_CD = hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                       "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                       "02_Beta_vs_P_q_0_All_e_CD", 0, beamE * 1.425, 0, 3, numTH2Dbins_Beta_Plots,
                                                       numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_All_e_FD = hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                       "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                       "02_Beta_vs_P_q_0_All_e_FD", 0, beamE * 1.425, 0, 3, numTH2Dbins_Beta_Plots,
                                                       numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_All_e_CD = hPlot2D("", "no #(e) cut", "CD", "#beta vs. P & q = -1",
                                                        "#beta vs. P for all particles with q = -1",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "03_Beta_vs_P_q_m1_All_e_CD", 0, beamE * 1.425, 0, 3, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_All_e_FD = hPlot2D("", "no #(e) cut", "FD", "#beta vs. P & q = -1",
                                                        "#beta vs. P for all particles with q = -1",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_All_e_Directory"],
                                                        "03_Beta_vs_P_q_m1_All_e_FD", 0, beamE * 1.425, 0, 3, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1e cut)">

    //<editor-fold desc="Beta vs. P for all particles (1e cut)">
    hPlot2D hBeta_vs_P_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "01_Beta_vs_P_1e_cut_All_Particles_CD",
                                           0, beamE * 1.1, 0, Beta_boundary_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"], "01_Beta_vs_P_1e_cut_All_Particles_FD",
                                           0, beamE * 1.1, 0, Beta_boundary_const, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_Electrons_Only_FD = hPlot2D("electrons only", "1e cut", "", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                          directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                          "02_Beta_vs_P_1e_cut_Electrons_Only_FD",
                                                          0, beamE * 1.425, 0.5, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_Protons_Only_CD = hPlot2D("protons only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                        "03_Beta_vs_P_1e_cut_Protons_Only_CD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_Protons_Only_FD = hPlot2D("protons only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                        "03_Beta_vs_P_1e_cut_Protons_Only_FD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_CD_CLAS12 = hPlot2D("neutrons only clas12", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]",
                                                                "#beta",
                                                                directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                                "04a_Beta_vs_P_1e_cut_Neutrons_Only_CD",
                                                                0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_FD_CLAS12 = hPlot2D("neutrons only clas12", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]",
                                                                "#beta",
                                                                directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                                "04a_Beta_vs_P_1e_cut_Neutrons_Only_FD",
                                                                0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_FD_ByDef = hPlot2D("neutrons only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                               directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                               "04b_Beta_vs_P_1e_cut_Neutrons_Only_FD",
                                                               0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_Kplus_Only_CD = hPlot2D("K^{+} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                      "05_Beta_vs_P_1e_cut_Kplus_Only_CD",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_Kplus_Only_FD = hPlot2D("K^{+} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                      "05_Beta_vs_P_1e_cut_Kplus_Only_FD",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_Kminus_Only_CD = hPlot2D("K^{-} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                       "06_Beta_vs_P_1e_cut_Kminus_Only_CD",
                                                       0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_Kminus_Only_FD = hPlot2D("K^{-} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                       "06_Beta_vs_P_1e_cut_Kminus_Only_FD",
                                                       0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_piplus_Only_CD = hPlot2D("#pi^{+} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                       "07_Beta_vs_P_1e_cut_piplus_Only_CD",
                                                       0, P_boundary, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_piplus_Only_FD = hPlot2D("#pi^{+} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                       "07_Beta_vs_P_1e_cut_piplus_Only_FD",
                                                       0, P_boundary, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1e_cut_piminus_Only_CD = hPlot2D("#pi^{-} only", "1e cut", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                        "08_Beta_vs_P_1e_cut_piminus_Only_CD",
                                                        0, P_boundary, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1e_cut_piminus_Only_FD = hPlot2D("#pi^{-} only", "1e cut", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_1e_cut_Directory"],
                                                        "08_Beta_vs_P_1e_cut_piminus_Only_FD",
                                                        0, P_boundary, 0, 1.7, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1e cut)">
    hPlot2D hBeta_vs_P_positive_part_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = +1",
                                                         "#beta vs. P for all particles with q = +1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "01_Beta_vs_P_q_p1_1e_cut_CD", 0, beamE * 1.1, 0, 1.7, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_positive_part_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = +1",
                                                         "#beta vs. P for all particles with q = +1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "01_Beta_vs_P_q_p1_1e_cut_FD", 0, beamE * 1.1, 0, 1.7, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = 0",
                                                        "#beta vs. P for all particles with q = 0",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                        "02_Beta_vs_P_q_0_1e_cut_CD", 0, beamE * 1.1, 0, 1.7, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = 0",
                                                        "#beta vs. P for all particles with q = 0",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                        "02_Beta_vs_P_q_0_1e_cut_FD", 0, beamE * 1.1, 0, 1.7, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_1e_cut_CD = hPlot2D("all particles", "1e cut", "CD", "#beta vs. P & q = -1",
                                                         "#beta vs. P for all particles with q = -1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "03_Beta_vs_P_q_m1_1e_cut_CD", 0, beamE * 1.1, 0, 1.7, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_1e_cut_FD = hPlot2D("all particles", "1e cut", "FD", "#beta vs. P & q = -1",
                                                         "#beta vs. P for all particles with q = -1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_1e_cut_Directory"],
                                                         "03_Beta_vs_P_q_m1_1e_cut_FD", 0, beamE * 1.1, 0, 1.7, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1p)">

    //<editor-fold desc="Beta vs. P for all particles (1p)">
    hPlot2D hBeta_vs_P_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "01_Beta_vs_P_1p_All_Particles_CD",
                                       0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "01_Beta_vs_P_1p_All_Particles_FD",
                                       0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1p_Electrons_Only_FD = hPlot2D("electrons only", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1p_Directory"],
                                                      "02_Beta_vs_P_1p_Electrons_Only_FD",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1p_Protons_Only_CD = hPlot2D("protons only", "1p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "03_Beta_vs_P_1p_Protons_Only_CD",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1p_Protons_Only_FD = hPlot2D("protons only", "1p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1p_Directory"], "03_Beta_vs_P_1p_Protons_Only_FD",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1p)">
    hPlot2D hBeta_vs_P_positive_part_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = +1",
                                                     "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "01_Beta_vs_P_q_p1_1p_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_positive_part_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = +1",
                                                     "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "01_Beta_vs_P_q_p1_1p_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                    "02_Beta_vs_P_q_0_1p_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                    "02_Beta_vs_P_q_0_1p_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_1p_CD = hPlot2D("all particles", "1p", "CD", "#beta vs. P & q = -1",
                                                     "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "03_Beta_vs_P_q_m1_1p_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_1p_FD = hPlot2D("all particles", "1p", "FD", "#beta vs. P & q = -1",
                                                     "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1p_Directory"],
                                                     "03_Beta_vs_P_q_m1_1p_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1n)">

    //<editor-fold desc="Beta vs. P for all particles (1n)">
    hPlot2D hBeta_vs_P_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "00_Beta_vs_P_All_Particles_CD_1n",
                                       0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "00_Beta_vs_P_All_Particles_FD_1n",
                                       0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Electrons_Only_FD = hPlot2D("electrons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_1n_Directory"],
                                                      "01_Beta_vs_P_Electrons_Only_FD_1n",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Neutrons_Only_CD = hPlot2D("id. neutrons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                     directories.Beta_Directory_map["Beta_VS_P_1n_Directory"],
                                                     "02_Beta_vs_P_id_Neutrons_Only_CD_1n",
                                                     0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Neutrons_Only_FD = hPlot2D("id. neutrons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                     directories.Beta_Directory_map["Beta_VS_P_1n_Directory"],
                                                     "02_Beta_vs_P_id_Neutrons_Only_FD_1n",
                                                     0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD = hPlot2D("id. neutrons only", "1n", "FD", "#beta vs. P - ZOOMOUT", "#beta vs. P", "P [GeV/c]",
                                                             "#beta",
                                                             directories.Beta_Directory_map["Beta_VS_P_1n_Directory"],
                                                             "02_Beta_vs_P_id_Neutrons_Only_ZOOMOUT_FD_1n",
                                                             0, P_boundary, 0, 2, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Neutrons_Only_from_photons_FD = hPlot2D("id. neutrons", "1n", "FD", "#beta vs. P",
                                                                  " Cond. to id. neutron's #beta vs. P from 'photons'", "P [GeV/c]", "#beta",
                                                                  directories.Beta_Directory_map["Beta_VS_P_1n_Directory"],
                                                                  "02a_Beta_vs_P_id_Neutrons_Only_from_potons_FD_1n",
                                                                  0, P_boundary, 0.9, 1.15, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Photons_Only_CD = hPlot2D("id. photons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "03_Beta_vs_P_id_Photons_Only_CD_1n",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Photons_Only_FD = hPlot2D("id. photons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "03_Beta_vs_P_id_Photons_Only_FD_1n",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Protons_Only_CD = hPlot2D("id. protons only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "04_Beta_vs_P_id_Protons_Only_CD_1n",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Protons_Only_FD = hPlot2D("id. protons only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "04_Beta_vs_P_id_Protons_Only_FD_1n",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Piplus_Only_CD = hPlot2D("id. #pi^{+} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_piplus_Only_CD_1n",
                                                   0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Piplus_Only_FD = hPlot2D("id. #pi^{+} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_piplus_Only_FD_1n",
                                                   0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Piminus_Only_CD = hPlot2D("id. #pi^{-} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_piminus_Only_CD_1n",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Piminus_Only_FD = hPlot2D("id. #pi^{-} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_piminus_Only_FD_1n",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Kplus_Only_CD = hPlot2D("id. K^{+} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_Kplus_Only_CD_1n",
                                                  0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Kplus_Only_FD = hPlot2D("id. K^{+} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                  directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "05_Beta_vs_P_id_Kplus_Only_FD_1n",
                                                  0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_1n_Kminus_Only_CD = hPlot2D("id. K^{-} only", "1n", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_Kminus_Only_CD_1n",
                                                   0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_1n_Kminus_Only_FD = hPlot2D("id. K^{-} only", "1n", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                   directories.Beta_Directory_map["Beta_VS_P_1n_Directory"], "06_Beta_vs_P_id_Kminus_Only_FD_1n",
                                                   0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1n)">
    hPlot2D hBeta_vs_P_pos_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "01_Beta_vs_P_q_p1_1n_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_pos_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = +1", "#beta vs. P for all particles with q = +1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "01_Beta_vs_P_q_p1_1n_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neut_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                 "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                 "02_Beta_vs_P_q_0_1n_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neut_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                 "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                 "02_Beta_vs_P_q_0_1n_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neg_part_1n_CD = hPlot2D("all particles", "1n", "CD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "03_Beta_vs_P_q_m1_1n_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neg_part_1n_FD = hPlot2D("all particles", "1n", "FD", "#beta vs. P & q = -1", "#beta vs. P for all particles with q = -1",
                                                "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_1n_Directory"],
                                                "03_Beta_vs_P_q_m1_1n_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (2p)">

    //<editor-fold desc="Beta vs. P for all particles (2p)">
    hPlot2D hBeta_vs_P_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "01_Beta_vs_P_2p_All_Particles_CD",
                                       0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                       directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "01_Beta_vs_P_2p_All_Particles_FD",
                                       0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_2p_Electrons_Only_FD = hPlot2D("electrons only", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_2p_Directory"],
                                                      "02_Beta_vs_P_2p_Electrons_Only_FD",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_2p_Protons_Only_CD = hPlot2D("protons only", "2p", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "03_Beta_vs_P_2p_Protons_Only_CD",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_2p_Protons_Only_FD = hPlot2D("protons only", "2p", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                    directories.Beta_Directory_map["Beta_VS_P_2p_Directory"], "03_Beta_vs_P_2p_Protons_Only_FD",
                                                    0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (2p)">
    hPlot2D hBeta_vs_P_positive_part_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = +1",
                                                     "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "01_Beta_vs_P_q_p1_2p_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_positive_part_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = +1",
                                                     "#beta vs. P for all particles with q = +1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "01_Beta_vs_P_q_p1_2p_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                    "02_Beta_vs_P_q_0_2p_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = 0", "#beta vs. P for all particles with q = 0",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                    "02_Beta_vs_P_q_0_2p_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_2p_CD = hPlot2D("all particles", "2p", "CD", "#beta vs. P & q = -1",
                                                     "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "03_Beta_vs_P_q_m1_2p_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_2p_FD = hPlot2D("all particles", "2p", "FD", "#beta vs. P & q = -1",
                                                     "#beta vs. P for all particles with q = -1",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_2p_Directory"],
                                                     "03_Beta_vs_P_q_m1_2p_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (pFDpCD)">

    //<editor-fold desc="Beta vs. P for all particles (pFDpCD)">
    hPlot2D hBeta_vs_P_pFDpCD_CD = hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"], "01_Beta_vs_P_pFDpCD_All_Particles_CD",
                                           0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_pFDpCD_FD = hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"], "01_Beta_vs_P_pFDpCD_All_Particles_FD",
                                           0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_pFDpCD_Electrons_Only_FD = hPlot2D("electrons only", "pFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                          directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"],
                                                          "02_Beta_vs_P_pFDpCD_Electrons_Only_FD",
                                                          0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_pFDpCD_Protons_Only_CD = hPlot2D("protons only", "pFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"],
                                                        "03_Beta_vs_P_pFDpCD_Protons_Only_CD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_pFDpCD_Protons_Only_FD = hPlot2D("protons only", "pFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"],
                                                        "03_Beta_vs_P_pFDpCD_Protons_Only_FD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_pFD_pFDpCD_Protons_Only_FD = hPlot2D("", "pFDpCD", "FD", "#beta vs. FD proton momentum", "#beta vs. FD proton momentum",
                                                            "P_{pFD} [GeV/c]",
                                                            "#beta", directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"],
                                                            "04a_Beta_vs_P_pFD_pFDpCD_Protons_Only_CD",
                                                            0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_pCD_pFDpCD_Protons_Only_CD = hPlot2D("", "pFDpCD", "CD", "#beta vs. CD proton momentum", "#beta vs. CD proton momentum",
                                                            "P_{pCD} [GeV/c]",
                                                            "#beta", directories.Beta_Directory_map["Beta_VS_P_pFDpCD_Directory"],
                                                            "04b_Beta_vs_P_pCD_pFDpCD_Protons_Only_CD",
                                                            0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (pFDpCD)">
    hPlot2D hBeta_vs_P_positive_part_pFDpCD_CD = hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P & q = +1",
                                                         "#beta vs. P for all particles with q = +1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"],
                                                         "01_Beta_vs_P_q_p1_pFDpCD_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_positive_part_pFDpCD_FD = hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P & q = +1",
                                                         "#beta vs. P for all particles with q = +1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"],
                                                         "01_Beta_vs_P_q_p1_pFDpCD_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_pFDpCD_CD = hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P & q = 0",
                                                        "#beta vs. P for all particles with q = 0",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"],
                                                        "02_Beta_vs_P_q_0_pFDpCD_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neutral_part_pFDpCD_FD = hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P & q = 0",
                                                        "#beta vs. P for all particles with q = 0",
                                                        "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"],
                                                        "02_Beta_vs_P_q_0_pFDpCD_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                        numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_pFDpCD_CD = hPlot2D("all particles", "pFDpCD", "CD", "#beta vs. P & q = -1",
                                                         "#beta vs. P for all particles with q = -1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"],
                                                         "03_Beta_vs_P_q_m1_pFDpCD_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_negative_part_pFDpCD_FD = hPlot2D("all particles", "pFDpCD", "FD", "#beta vs. P & q = -1",
                                                         "#beta vs. P for all particles with q = -1",
                                                         "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_by_charge_pFDpCD_Directory"],
                                                         "03_Beta_vs_P_q_m1_pFDpCD_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                         numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (nFDpCD)">

    //<editor-fold desc="Beta vs. P for all particles (nFDpCD)">
    hPlot2D hBeta_vs_P_nFDpCD_CD = hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"], "00_Beta_vs_P_All_Particles_CD_nFDpCD",
                                           0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_FD = hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                           directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"], "00_Beta_vs_P_All_Particles_FD_nFDpCD",
                                           0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Electrons_Only_FD = hPlot2D("electrons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                          directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                          "01_Beta_vs_P_Electrons_Only_FD_nFDpCD",
                                                          0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_CD = hPlot2D("id. neutrons only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                         "02_Beta_vs_P_id_Neutrons_Only_CD_nFDpCD",
                                                         0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_FD = hPlot2D("id. neutrons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                         directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                         "02_Beta_vs_P_id_Neutrons_Only_FD_nFDpCD",
                                                         0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_ZOOMOUT_FD = hPlot2D("id. neutrons only", "nFDpCD", "FD", "#beta vs. P - ZOOMOUT", "#beta vs. P",
                                                                 "P [GeV/c]", "#beta",
                                                                 directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                                 "02_Beta_vs_P_id_Neutrons_Only_ZOOMOUT_FD_nFDpCD",
                                                                 0, P_boundary, 0, 2, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_from_photons_FD = hPlot2D("id. neutrons", "nFDpCD", "FD", "#beta vs. P",
                                                                      " Cond. to id. neutron's #beta vs. P from 'photons'",
                                                                      "P [GeV/c]", "#beta",
                                                                      directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                                      "02a_Beta_vs_P_id_Neutrons_Only_from_potons_FD_nFDpCD",
                                                                      0, P_boundary, 0.9, 1.15, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Photons_Only_CD = hPlot2D("id. photons only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                        "03_Beta_vs_P_id_Photons_Only_CD_nFDpCD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Photons_Only_FD = hPlot2D("id. photons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                        "03_Beta_vs_P_id_Photons_Only_FD_nFDpCD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Protons_Only_CD = hPlot2D("id. protons only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                        "04_Beta_vs_P_id_Protons_Only_CD_nFDpCD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Protons_Only_FD = hPlot2D("id. protons only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                        "04_Beta_vs_P_id_Protons_Only_FD_nFDpCD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Piplus_Only_CD = hPlot2D("id. #pi^{+} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                       "05_Beta_vs_P_id_piplus_Only_CD_nFDpCD",
                                                       0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Piplus_Only_FD = hPlot2D("id. #pi^{+} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                       "05_Beta_vs_P_id_piplus_Only_FD_nFDpCD",
                                                       0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Piminus_Only_CD = hPlot2D("id. #pi^{-} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                        "06_Beta_vs_P_id_piminus_Only_CD_nFDpCD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Piminus_Only_FD = hPlot2D("id. #pi^{-} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                        directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                        "06_Beta_vs_P_id_piminus_Only_FD_nFDpCD",
                                                        0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Kplus_Only_CD = hPlot2D("id. K^{+} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                      "05_Beta_vs_P_id_Kplus_Only_CD_nFDpCD",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Kplus_Only_FD = hPlot2D("id. K^{+} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                      directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                      "05_Beta_vs_P_id_Kplus_Only_FD_nFDpCD",
                                                      0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);

    hPlot2D hBeta_vs_P_nFDpCD_Kminus_Only_CD = hPlot2D("id. K^{-} only", "nFDpCD", "CD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                       "06_Beta_vs_P_id_Kminus_Only_CD_nFDpCD",
                                                       0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_nFDpCD_Kminus_Only_FD = hPlot2D("id. K^{-} only", "nFDpCD", "FD", "#beta vs. P", "#beta vs. P", "P [GeV/c]", "#beta",
                                                       directories.Beta_Directory_map["Beta_VS_P_nFDpCD_Directory"],
                                                       "06_Beta_vs_P_id_Kminus_Only_FD_nFDpCD",
                                                       0, P_boundary, 0, Beta_boundary, numTH2Dbins_Beta_Plots, numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (nFDpCD)">
    hPlot2D hBeta_vs_P_pos_part_nFDpCD_CD = hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P & q = +1",
                                                    "#beta vs. P for all particles with q = +1",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"],
                                                    "01_Beta_vs_P_q_p1_nFDpCD_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_pos_part_nFDpCD_FD = hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P & q = +1",
                                                    "#beta vs. P for all particles with q = +1",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"],
                                                    "01_Beta_vs_P_q_p1_nFDpCD_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neut_part_nFDpCD_CD = hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P & q = 0",
                                                     "#beta vs. P for all particles with q = 0",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"],
                                                     "02_Beta_vs_P_q_0_nFDpCD_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neut_part_nFDpCD_FD = hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P & q = 0",
                                                     "#beta vs. P for all particles with q = 0",
                                                     "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"],
                                                     "02_Beta_vs_P_q_0_nFDpCD_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                     numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neg_part_nFDpCD_CD = hPlot2D("all particles", "nFDpCD", "CD", "#beta vs. P & q = -1",
                                                    "#beta vs. P for all particles with q = -1",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"],
                                                    "03_Beta_vs_P_q_m1_nFDpCD_CD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    hPlot2D hBeta_vs_P_neg_part_nFDpCD_FD = hPlot2D("all particles", "nFDpCD", "FD", "#beta vs. P & q = -1",
                                                    "#beta vs. P for all particles with q = -1",
                                                    "P [GeV/c]", "#beta", directories.Beta_Directory_map["Beta_VS_P_by_charge_nFDpCD_Directory"],
                                                    "03_Beta_vs_P_q_m1_nFDpCD_FD", 0, beamE * 1.1, 0, 1.5, numTH2Dbins_Beta_Plots,
                                                    numTH2Dbins_Beta_Plots);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// W histograms
// ======================================================================================================================================================================

    //<editor-fold desc="W histograms">

    //<editor-fold desc="W plots (pFDpCD, CD & FD)">
    THStack *sW_pFDpCD = new THStack("W distribution (All Int., pFDpCD)",
                                     "W distribution (pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];");
    TH1D *hW_All_Int_pFDpCD = new TH1D("W distribution (All Int., pFDpCD)",
                                       "W distribution (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                       numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_QEL_pFDpCD = new TH1D("W distribution (QEL only, pFDpCD)",
                                   "W distribution (QEL only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_MEC_pFDpCD = new TH1D("W distribution (MEC only, pFDpCD)",
                                   "W distribution (MEC only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_RES_pFDpCD = new TH1D("W distribution (RES only, pFDpCD)",
                                   "W distribution (RES only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_DIS_pFDpCD = new TH1D("W distribution (DIS only, pFDpCD)",
                                   "W distribution (DIS only, pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    string sW_pFDpCD_Dir = directories.W_Directory_map["W_pFDpCD_Directory"];
    string hW_All_Int_pFDpCD_Dir = directories.W_Directory_map["W_pFDpCD_Directory"];
    string hW_QEL_pFDpCD_Dir = directories.W_Directory_map["W_pFDpCD_Directory"];
    string hW_MEC_pFDpCD_Dir = directories.W_Directory_map["W_pFDpCD_Directory"];
    string hW_RES_pFDpCD_Dir = directories.W_Directory_map["W_pFDpCD_Directory"];
    string hW_DIS_pFDpCD_Dir = directories.W_Directory_map["W_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="W plots (nFDpCD, CD & FD)">
    THStack *sW_nFDpCD = new THStack("W distribution (All Int., nFDpCD)",
                                     "W distribution (nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];");
    TH1D *hW_All_Int_nFDpCD = new TH1D("W distribution (All Int., nFDpCD)",
                                       "W distribution (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                       numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_QEL_nFDpCD = new TH1D("W distribution (QEL only, nFDpCD)",
                                   "W distribution (QEL only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_MEC_nFDpCD = new TH1D("W distribution (MEC only, nFDpCD)",
                                   "W distribution (MEC only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_RES_nFDpCD = new TH1D("W distribution (RES only, nFDpCD)",
                                   "W distribution (RES only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    TH1D *hW_DIS_nFDpCD = new TH1D("W distribution (DIS only, nFDpCD)",
                                   "W distribution (DIS only, nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];",
                                   numTH1Dbins_W_Plots, W_lboundary, W_uboundary);
    string sW_nFDpCD_Dir = directories.W_Directory_map["W_nFDpCD_Directory"];
    string hW_All_Int_nFDpCD_Dir = directories.W_Directory_map["W_nFDpCD_Directory"];
    string hW_QEL_nFDpCD_Dir = directories.W_Directory_map["W_nFDpCD_Directory"];
    string hW_MEC_nFDpCD_Dir = directories.W_Directory_map["W_nFDpCD_Directory"];
    string hW_RES_nFDpCD_Dir = directories.W_Directory_map["W_nFDpCD_Directory"];
    string hW_DIS_nFDpCD_Dir = directories.W_Directory_map["W_nFDpCD_Directory"];
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
    THStack *sTheta_e_2p_FD = new THStack("#theta_{e} (2p, FD)", "#theta_{e} of Outgoing Electron (2p, FD);#theta_{e} [Deg];");
    THStack *sTheta_e_pFDpCD_FD = new THStack("#theta_{e} (pFDpCD, FD)", "#theta_{e} of Outgoing Electron (pFDpCD, FD);#theta_{e} [Deg];");
    THStack *sTheta_e_nFDpCD_FD = new THStack("#theta_{e} (nFDpCD, FD)", "#theta_{e} of Outgoing Electron (nFDpCD, FD);#theta_{e} [Deg];");

    //<editor-fold desc="Theta_e histograms (no #(e) cut)">
    TH1D *hTheta_e_All_e_FD = new TH1D("#theta_{e} (no #(e) cut, FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, FD);#theta_{e} [Deg];",
                                       numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_All_e_FD_Dir = directories.Angle_Directory_map["Theta_e_All_e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (1e cut)">
    TH1D *hTheta_e_1e_cut_FD = new TH1D("#theta_{e} (1e Cut, FD)", "#theta_{e} of Outgoing Electron (1e Cut, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Theta_e_1e_cut_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (1p)">
    TH1D *hTheta_e_All_Int_1p_FD = new TH1D("#theta_{e} (All Int., 1p, FD)",
                                            "#theta_{e} of Outgoing Electron (All Int., 1p, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_1p_FD = new TH1D("#theta_{e} (QEL Only, 1p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_1p_FD = new TH1D("#theta_{e} (MEC Only, 1p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_RES_1p_FD = new TH1D("#theta_{e} (RES Only, 1p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_1p_FD = new TH1D("#theta_{e} (DIS Only, 1p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_All_Int_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_QEL_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_MEC_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_RES_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_DIS_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];

    TH2D *hTheta_e_VS_P_e_1p_FD = new TH2D("#theta_{e} vs. P_{e} (All Int., 1p)",
                                           "#theta_{e} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];#theta_{e} [Deg]",
                                           numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH2D *hTheta_e_VS_W_1p_FD = new TH2D("#theta_{e} vs. W (All Int., 1p)",
                                         "#theta_{e} vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{e} [Deg]",
                                         numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                         Theta_uboundary_FD);
    string hTheta_e_VS_P_e_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    string hTheta_e_VS_W_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (1n)">
    TH1D *hTheta_e_All_Int_1n_FD = new TH1D("#theta_{e} (All Int., 1n, FD)",
                                            "#theta_{e} of Outgoing Electron (All Int., 1n, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_1n_FD = new TH1D("#theta_{e} (QEL Only, 1n, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1n, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_1n_FD = new TH1D("#theta_{e} (MEC Only, 1n, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1n, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_RES_1n_FD = new TH1D("#theta_{e} (RES Only, 1n, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1n, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_1n_FD = new TH1D("#theta_{e} (DIS Only, 1n, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1n, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_All_Int_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_QEL_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_MEC_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_RES_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_DIS_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];

    TH2D *hTheta_e_VS_P_e_1n_FD = new TH2D("#theta_{e} vs. P_{e} (All Int., 1n)",
                                           "#theta_{e} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];#theta_{e} [Deg]",
                                           numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH2D *hTheta_e_VS_W_1n_FD = new TH2D("#theta_{e} vs. W (All Int., 1n)",
                                         "#theta_{e} vs. W (All Int., 1n);W = #sqrt{(#omega + m_{n})^{2} - #vec{q}^{2}}  [GeV];#theta_{e} [Deg]",
                                         numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                         Theta_uboundary_FD);
    string hTheta_e_VS_P_e_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    string hTheta_e_VS_W_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (2p)">
    TH1D *hTheta_e_All_Int_2p_FD = new TH1D("#theta_{e} (All Int., 2p, FD)",
                                            "#theta_{e} of Outgoing Electron (All Int., 2p, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_2p_FD = new TH1D("#theta_{e} (QEL Only, 2p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 2p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_2p_FD = new TH1D("#theta_{e} (MEC Only, 2p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 2p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_RES_2p_FD = new TH1D("#theta_{e} (RES Only, 2p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 2p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_2p_FD = new TH1D("#theta_{e} (DIS Only, 2p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 2p, FD);#theta_{e} [Deg];",
                                        numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_All_Int_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_QEL_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_MEC_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_RES_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    string hTheta_e_DIS_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (pFDpCD)">
    TH1D *hTheta_e_All_Int_pFDpCD_FD = new TH1D("#theta_{e} (All Int., pFDpCD, FD)",
                                                "#theta_{e} of Outgoing Electron (All Int., pFDpCD, FD);#theta_{e} [Deg];",
                                                numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_pFDpCD_FD = new TH1D("#theta_{e} (QEL Only, pFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (QEL Only, pFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_pFDpCD_FD = new TH1D("#theta_{e} (MEC Only, pFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (MEC Only, pFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_RES_pFDpCD_FD = new TH1D("#theta_{e} (RES Only, pFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (RES Only, pFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_pFDpCD_FD = new TH1D("#theta_{e} (DIS Only, pFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (DIS Only, pFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_All_Int_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];
    string hTheta_e_QEL_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];
    string hTheta_e_MEC_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];
    string hTheta_e_RES_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];
    string hTheta_e_DIS_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];

    TH2D *hTheta_e_VS_P_e_pFDpCD_FD = new TH2D("#theta_{e} vs. P_{e} (All Int., pFDpCD)",
                                               "#theta_{e} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];#theta_{e} [Deg]",
                                               numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                               Theta_uboundary_FD);
    TH2D *hTheta_e_VS_W_pFDpCD_FD = new TH2D("#theta_{e} vs. W (All Int., pFDpCD)",
                                             "#theta_{e} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{e} [Deg]",
                                             numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                             Theta_uboundary_FD);
    string hTheta_e_VS_P_e_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];
    string hTheta_e_VS_W_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (nFDpCD)">
    TH1D *hTheta_e_All_Int_nFDpCD_FD = new TH1D("#theta_{e} (All Int., nFDpCD, FD)",
                                                "#theta_{e} of Outgoing Electron (All Int., nFDpCD, FD);#theta_{e} [Deg];",
                                                numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_QEL_nFDpCD_FD = new TH1D("#theta_{e} (QEL Only, nFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (QEL Only, nFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_MEC_nFDpCD_FD = new TH1D("#theta_{e} (MEC Only, nFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (MEC Only, nFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_RES_nFDpCD_FD = new TH1D("#theta_{e} (RES Only, nFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (RES Only, nFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_e_DIS_nFDpCD_FD = new TH1D("#theta_{e} (DIS Only, nFDpCD, FD)",
                                            "#theta_{e} of Outgoing Electron (DIS Only, nFDpCD, FD);#theta_{e} [Deg];",
                                            numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_All_Int_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];
    string hTheta_e_QEL_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];
    string hTheta_e_MEC_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];
    string hTheta_e_RES_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];
    string hTheta_e_DIS_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];

    TH2D *hTheta_e_VS_P_e_nFDpCD_FD = new TH2D("#theta_{e} vs. P_{e} (All Int., nFDpCD)",
                                               "#theta_{e} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];#theta_{e} [Deg]",
                                               numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                               Theta_uboundary_FD);
    TH2D *hTheta_e_VS_W_nFDpCD_FD = new TH2D("#theta_{e} vs. W (All Int., nFDpCD)",
                                             "#theta_{e} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{e} [Deg]",
                                             numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                             Theta_uboundary_FD);
    string hTheta_e_VS_P_e_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];
    string hTheta_e_VS_W_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_e histograms">
    THStack *sPhi_e = new THStack("#phi_{e} stack (CD & FD)", "#phi_{e} of Outgoing Electron (no #(e) cut, CD & FD);#phi_{e} [Deg];");

    //<editor-fold desc="Phi_e histograms (no #(e) cut)">
    TH1D *hPhi_e_All_e_FD = new TH1D("#phi_{e} (no #(e) cut, FD)", "#phi_{e} of Outgoing Electron (All Int., no #(e) cut, FD);#phi_{e} [Deg];",
                                     numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_All_e_FD_Dir = directories.Angle_Directory_map["Phi_e_All_e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (1e cut)">
    TH1D *hPhi_e_1e_cut_FD = new TH1D("#phi_{e} (1e Cut, FD)", "#phi_{e} of Outgoing Electron (All Int., 1e Cut, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots,
                                      Phi_lboundary, Phi_uboundary);
    string hPhi_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Phi_e_1e_cut_Directory"];
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (1p)">
    TH1D *hPhi_e_All_Int_1p_FD = new TH1D("#phi_{e} 1p (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., 1p, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_QEL_1p_FD = new TH1D("#phi_{e} for 1p (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, 1p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_MEC_1p_FD = new TH1D("#phi_{e} for 1p (MEC Only, FD)", "#phi_{e} of Outgoing Electron (MEC Only, 1p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_RES_1p_FD = new TH1D("#phi_{e} for 1p (RES Only, FD)", "#phi_{e} of Outgoing Electron (RES Only, 1p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_DIS_1p_FD = new TH1D("#phi_{e} for 1p (DIS Only, FD)", "#phi_{e} of Outgoing Electron (DIS Only, 1p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_All_Int_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_QEL_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_MEC_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_RES_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_DIS_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];

    TH2D *hPhi_e_VS_P_e_1p_FD = new TH2D("#phi_{e} vs. P_{e} (All Int., 1p)",
                                         "#phi_{e} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];#phi_{e} [Deg]",
                                         numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_e_VS_W_1p_FD = new TH2D("#phi_{e} vs. W (All Int., 1p)",
                                       "#phi_{e} vs. W (All Int., 1p);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{e} [Deg]",
                                       numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_VS_P_e_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    string hPhi_e_VS_W_1p_FD_Dir = directories.Angle_Directory_map["Phi_e_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (1n)">
    TH1D *hPhi_e_All_Int_1n_FD = new TH1D("#phi_{e} 1n (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., 1n, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_QEL_1n_FD = new TH1D("#phi_{e} for 1n (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, 1n, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_MEC_1n_FD = new TH1D("#phi_{e} for 1n (MEC Only, FD)", "#phi_{e} of Outgoing Electron (MEC Only, 1n, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_RES_1n_FD = new TH1D("#phi_{e} for 1n (RES Only, FD)", "#phi_{e} of Outgoing Electron (RES Only, 1n, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_DIS_1n_FD = new TH1D("#phi_{e} for 1n (DIS Only, FD)", "#phi_{e} of Outgoing Electron (DIS Only, 1n, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_All_Int_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_QEL_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_MEC_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_RES_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_DIS_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];

    TH2D *hPhi_e_VS_P_e_1n_FD = new TH2D("#phi_{e} vs. P_{e} (All Int., 1n)",
                                         "#phi_{e} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];#phi_{e} [Deg]",
                                         numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_e_VS_W_1n_FD = new TH2D("#phi_{e} vs. W (All Int., 1n)",
                                       "#phi_{e} vs. W (All Int., 1n);W = #sqrt{(#omega + m_{n})^{2} - #vec{q}^{2}}  [GeV];#phi_{e} [Deg]",
                                       numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_VS_P_e_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    string hPhi_e_VS_W_1n_FD_Dir = directories.Angle_Directory_map["Phi_e_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (2p)">
    TH1D *hPhi_e_All_Int_2p_FD = new TH1D("#phi_{e} 2p (All Int., FD)", "#phi_{e} of Outgoing Electron (All Int., 2p, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_QEL_2p_FD = new TH1D("#phi_{e} for 2p (QEL Only, FD)", "#phi_{e} of Outgoing Electron (QEL Only, 2p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_MEC_2p_FD = new TH1D("#phi_{e} for 2p (MEC Only, FD)", "#phi_{e} of Outgoing Electron (MEC Only, 2p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_RES_2p_FD = new TH1D("#phi_{e} for 2p (RES Only, FD)", "#phi_{e} of Outgoing Electron (RES Only, 2p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_DIS_2p_FD = new TH1D("#phi_{e} for 2p (DIS Only, FD)", "#phi_{e} of Outgoing Electron (DIS Only, 2p, FD);#phi_{e} [Deg];",
                                      numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_All_Int_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_QEL_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_MEC_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_RES_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    string hPhi_e_DIS_2p_FD_Dir = directories.Angle_Directory_map["Phi_e_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (pFDpCD)">
    TH1D *hPhi_e_All_Int_pFDpCD_FD = new TH1D("#phi_{e} pFDpCD (All Int., FD)",
                                              "#phi_{e} of Outgoing Electron (All Int., pFDpCD, FD);#phi_{e} [Deg];",
                                              numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_QEL_pFDpCD_FD = new TH1D("#phi_{e} for pFDpCD (QEL Only, FD)",
                                          "#phi_{e} of Outgoing Electron (QEL Only, pFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_MEC_pFDpCD_FD = new TH1D("#phi_{e} for pFDpCD (MEC Only, FD)",
                                          "#phi_{e} of Outgoing Electron (MEC Only, pFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_RES_pFDpCD_FD = new TH1D("#phi_{e} for pFDpCD (RES Only, FD)",
                                          "#phi_{e} of Outgoing Electron (RES Only, pFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_DIS_pFDpCD_FD = new TH1D("#phi_{e} for pFDpCD (DIS Only, FD)",
                                          "#phi_{e} of Outgoing Electron (DIS Only, pFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_All_Int_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];
    string hPhi_e_QEL_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];
    string hPhi_e_MEC_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];
    string hPhi_e_RES_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];
    string hPhi_e_DIS_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];

    TH2D *hPhi_e_VS_P_e_pFDpCD_FD = new TH2D("#phi_{e} vs. P_{e} (All Int., pFDpCD)",
                                             "#phi_{e} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];#phi_{e} [Deg]",
                                             numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_e_VS_W_pFDpCD_FD = new TH2D("#phi_{e} vs. W (All Int., pFDpCD)",
                                           "#phi_{e} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{e} [Deg]",
                                           numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_VS_P_e_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];
    string hPhi_e_VS_W_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (nFDpCD)">
    TH1D *hPhi_e_All_Int_nFDpCD_FD = new TH1D("#phi_{e} nFDpCD (All Int., FD)",
                                              "#phi_{e} of Outgoing Electron (All Int., nFDpCD, FD);#phi_{e} [Deg];",
                                              numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_QEL_nFDpCD_FD = new TH1D("#phi_{e} for nFDpCD (QEL Only, FD)",
                                          "#phi_{e} of Outgoing Electron (QEL Only, nFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_MEC_nFDpCD_FD = new TH1D("#phi_{e} for nFDpCD (MEC Only, FD)",
                                          "#phi_{e} of Outgoing Electron (MEC Only, nFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_RES_nFDpCD_FD = new TH1D("#phi_{e} for nFDpCD (RES Only, FD)",
                                          "#phi_{e} of Outgoing Electron (RES Only, nFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_e_DIS_nFDpCD_FD = new TH1D("#phi_{e} for nFDpCD (DIS Only, FD)",
                                          "#phi_{e} of Outgoing Electron (DIS Only, nFDpCD, FD);#phi_{e} [Deg];",
                                          numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_All_Int_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];
    string hPhi_e_QEL_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];
    string hPhi_e_MEC_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];
    string hPhi_e_RES_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];
    string hPhi_e_DIS_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];

    TH2D *hPhi_e_VS_P_e_nFDpCD_FD = new TH2D("#phi_{e} vs. P_{e} (All Int., nFDpCD)",
                                             "#phi_{e} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];#phi_{e} [Deg]",
                                             numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_e_VS_W_nFDpCD_FD = new TH2D("#phi_{e} vs. W (All Int., nFDpCD)",
                                           "#phi_{e} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{e} [Deg]",
                                           numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_e_VS_P_e_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];
    string hPhi_e_VS_W_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_e_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_e vs. Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e vs. Phi_e">
    /* Theta_e vs. Phi_e histograms (no #(e) cut) */
    TH2D *hTheta_e_VS_Phi_e_All_e_FD = new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, FD)",
                                                "#theta_{e} vs. #phi_{e}  (no #(e) cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_All_e_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_All_e_Directory"];

    /* Theta_e vs. Phi_e histograms (1e cut) */
    TH2D *hTheta_e_VS_Phi_e_1e_cut_FD = new TH2D("#theta_{e} vs. #phi_{e} (1e Cut, FD)",
                                                 "#theta_{e} vs. #phi_{e} (1e Cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                 numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                 numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_1e_cut_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1e_cut_Directory"];

    /* Theta_e vs. Phi_e histograms (1p) */
    TH2D *hTheta_e_VS_Phi_e_1p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 1p, FD)",
                                             "#theta_{e} vs. #phi_{e} (All Int., 1p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                             numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_1p_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1p_Directory"];

    /* Theta_e vs. Phi_e histograms (1n) */
    TH2D *hTheta_e_VS_Phi_e_1n_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 1n, FD)",
                                             "#theta_{e} vs. #phi_{e} (All Int., 1n, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                             numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_1n_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_1n_Directory"];

    /* Theta_e vs. Phi_e histograms (2p) */
    TH2D *hTheta_e_VS_Phi_e_2p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 2p, FD)",
                                             "#theta_{e} vs. #phi_{e} (All Int., 2p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                             numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_2p_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_2p_Directory"];

    /* Theta_e vs. Phi_e histograms (pFDpCD) */
    TH2D *hTheta_e_VS_Phi_e_pFDpCD_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., pFDpCD, FD)",
                                                 "#theta_{e} vs. #phi_{e} (All Int., pFDpCD, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                 numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                 numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_pFDpCD_Directory"];

    /* Theta_e vs. Phi_e histograms (nFDpCD) */
    TH2D *hTheta_e_VS_Phi_e_nFDpCD_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD)",
                                                 "#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                 numTH2Dbins_Electron_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                 numTH2Dbins_Electron_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_e_VS_Phi_e_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_e_VS_Phi_e_nFDpCD_Directory"];
    //</editor-fold>

// Other angle plots ----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Other angle plots">

    //<editor-fold desc="Other angle plots (1e cut)">

    //<editor-fold desc="Neutron veto plots (1e cut)">

    //<editor-fold desc="CLAS12 neutrons and protons">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} BV",
                                                                  "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                                  "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                  "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                  directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                  "01_Neutron_hits_vs_electron_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hPlot2D hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{n,p} vs. #Delta#phi_{n,p} BV",
                                                                "'Neutron Hits' vs. Proton Hits - Before Veto",
                                                                "#Delta#phi_{n,p} = #phi_{n}^{ECAL} - #phi_{p}^{ECAL} [Deg]",
                                                                "#Delta#theta_{n,p} = #theta_{n}^{ECAL} - #theta_{p}^{ECAL} [Deg]",
                                                                directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                "02_Neutron_hits_vs_proton_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    //</editor-fold>

    //<editor-fold desc="Electrons and redefined neutrons">

    //<editor-fold desc="Electrons and leading FD neutron">
    hPlot2D hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} BV",
                                                                        "'Leading Neutron Hits' vs. Electron Hits - Before Veto",
                                                                        "#Delta#phi_{LnFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                        "#Delta#theta_{LnFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                        directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                        "03_LnFD_Neutron_hits_vs_electron_hits_BV_1e_cut", -180, 180, -50, 50, 100,
                                                                        100);
    hPlot2D hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_AV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} AV",
                                                                        "'Leading Neutron Hits' vs. Electron Hits - After Veto",
                                                                        "#Delta#phi_{LnFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                        "#Delta#theta_{LnFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                        directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                        "04_LnFD_Neutron_hits_vs_electron_hits_AV_1e_cut", -180, 180, -50, 50, 100,
                                                                        100);
    hPlot2D hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_Vetoed_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} Vetoed",
                                                                            "'Leading Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                                                                            "#Delta#phi_{LnFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                            "#Delta#theta_{LnFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                            directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                            "05_LnFD_Neutron_hits_vs_electron_hits_Vetoed_1e_cut", -180, 180, -50,
                                                                            50, 100, 100);
    //</editor-fold>

    //<editor-fold desc="Electrons and all FD neutrons">
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV",
                                                                      "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                                      "#Delta#phi_{nFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                      "#Delta#theta_{nFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                      directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                      "06_nFD_Neutron_hits_vs_electron_hits_BV_1e_cut", -180, 180, -50, 50, 100,
                                                                      100);
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV",
                                                                      "'Neutron Hits' vs. Electron Hits - After Veto",
                                                                      "#Delta#phi_{nFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                      "#Delta#theta_{nFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                      directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                      "07_nFD_Neutron_hits_vs_electron_hits_AV_1e_cut", -180, 180, -50, 50, 100,
                                                                      100);
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} Vetoed",
                                                                          "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                                                                          "#Delta#phi_{nFD,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                          "#Delta#theta_{nFD,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                          directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                          "08_nFD_Neutron_hits_vs_electron_hits_Vetoed_1e_cut", -180, 180, -50, 50,
                                                                          100, 100);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Identified protons and redefined neutrons">

    //<editor-fold desc="Identified protons and leading FD neutron">
    hPlot2D hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} BV",
                                                                      "'Leading Neutron Hits' vs. Proton Hits - Before Veto",
                                                                      "#Delta#phi_{LnFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [Deg]",
                                                                      "#Delta#theta_{LnFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [Deg]",
                                                                      directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                      "09_LnFD_Neutron_hits_vs_proton_hits_BV_1e_cut", -180, 180, -50, 50, 100,
                                                                      100);
    hPlot2D hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_AV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} AV",
                                                                      "'Leading Neutron Hits' vs. Proton Hits - After Veto",
                                                                      "#Delta#phi_{LnFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [Deg]",
                                                                      "#Delta#theta_{LnFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [Deg]",
                                                                      directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                      "10_LnFD_Neutron_hits_vs_proton_hits_AV_1e_cut", -180, 180, -50, 50, 100,
                                                                      100);
    hPlot2D hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_Vetoed_1e_cut = hPlot2D("1e cut", "FD",
                                                                          "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} Vetoed",
                                                                          "'Leading Neutron Hits' vs. Proton Hits - Vetoed Neutrons",
                                                                          "#Delta#phi_{LnFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [Deg]",
                                                                          "#Delta#theta_{LnFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [Deg]",
                                                                          directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                          "11_LnFD_Neutron_hits_vs_proton_hits_Vetoed_1e_cut", -180, 180, -50, 50,
                                                                          100, 100);
    //</editor-fold>

    //<editor-fold desc="Identified protons and all FD neutrons">
    hPlot2D hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_BV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} BV",
                                                                    "'Neutron Hits' vs. Proton Hits - Before Veto",
                                                                    "#Delta#phi_{nFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [Deg]",
                                                                    "#Delta#theta_{nFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [Deg]",
                                                                    directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                    "12_nFD_Neutron_hits_vs_proton_hits_BV_1e_cut", -180, 180, -50, 50, 100, 100);
    hPlot2D hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_AV_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} AV",
                                                                    "'Neutron Hits' vs. Proton Hits - After Veto",
                                                                    "#Delta#phi_{nFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [Deg]",
                                                                    "#Delta#theta_{nFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [Deg]",
                                                                    directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                    "13_nFD_Neutron_hits_vs_proton_hits_AV_1e_cut", -180, 180, -50, 50, 100, 100);
    hPlot2D hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_Vetoed_1e_cut = hPlot2D("1e cut", "FD", "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} Vetoed",
                                                                        "'Neutron Hits' vs. Proton Hits - Vetoed Neutrons",
                                                                        "#Delta#phi_{nFD,pFD} = #phi_{n}^{ECAL} - #phi_{pFD}^{ECAL} [Deg]",
                                                                        "#Delta#theta_{nFD,pFD} = #theta_{n}^{ECAL} - #theta_{pFD}^{ECAL} [Deg]",
                                                                        directories.Angle_Directory_map["Neutron_veto_1e_cut_Directory"],
                                                                        "14_nFD_Neutron_hits_vs_proton_hits_Vetoed_1e_cut", -180, 180, -50, 50,
                                                                        100, 100);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1p)">

// Theta_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p (1p, FD only)">
    THStack *sTheta_p_1p = new THStack("#theta_{p} (All Int., 1p, FD)", "#theta_{p} of Outgoing FD Proton (All Int., 1p, FD);#theta_{p} [Deg];");
    TH1D *hTheta_p_All_Int_1p = new TH1D("#theta_{p} (All Int., 1p, FD)", "#theta_{p} of Outgoing FD Proton (All Int., 1p, FD);#theta_{p} [Deg];",
                                         numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_p_All_Int_1p_Dir = directories.Angle_Directory_map["Theta_p_1p_Directory"];

    TH2D *hTheta_p_VS_P_p_1p_FD = new TH2D("#theta_{p} vs. P_{p} (All Int., 1p, FD)",
                                           "#theta_{p} vs. P_{p} (All Int., 1p, FD);P_{p} [GeV/c];#theta_{p} [Deg]",
                                           numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH2D *hTheta_p_VS_W_1p_FD = new TH2D("#theta_{p} vs. W (All Int., 1p, FD)",
                                         "#theta_{p} vs. W (All Int., 1p, FD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{p} [Deg]",
                                         numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                         Theta_uboundary_FD);
    string hTheta_p_VS_P_p_1p_FD_Dir = directories.Angle_Directory_map["Theta_p_1p_Directory"];
    string hTheta_p_VS_W_1p_FD_Dir = directories.Angle_Directory_map["Theta_p_1p_Directory"];
    //</editor-fold>

// Phi_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_p (1p, FD only)">
    THStack *sPhi_p_1p = new THStack("#phi_{p} (All Int., 1p, FD)", "#phi_{p} of Outgoing Proton (All Int., 1p, FD);#phi_{p} [Deg];");
    TH1D *hPhi_p_All_Int_1p = new TH1D("#phi_{p} (All Int., 1p, FD)", "#phi_{p} of Outgoing Proton (All Int., 1p, FD);#phi_{p} [Deg];",
                                       numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_p_All_Int_1p_Dir = directories.Angle_Directory_map["Phi_p_1p_Directory"];
    //</editor-fold>

// Theta_p vs. Phi_p ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p vs. Phi_p histograms">
    TH2D *hTheta_p_VS_Phi_p_1p_FD = new TH2D("#theta_{p} vs. #phi_{p} of FD proton (All Int., 1p)",
                                             "#theta_{p} vs. #phi_{p} of FD proton (All Int., 1p);#phi_{p} [Deg];#theta_{p} [Deg]",
                                             numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                             numTH2Dbins_Nucleon_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_p_VS_Phi_p_1p_FD_Dir = directories.Angle_Directory_map["Theta_p_VS_Phi_p_1p_Directory"];
    //</editor-fold>

// Theta_p_e_p_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_p (1p, FD only)">
    THStack *sTheta_p_e_p_p_1p = new THStack("#theta_{#vec{P}_{e},#vec{P}_{p}} (All Int., 1p, FD)",
                                             "#theta_{#vec{P}_{e},#vec{P}_{p}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{p} (All Int., 1p, FD);#theta_{#vec{P}_{e},#vec{P}_{p}} [Deg];");
    TH1D *hTheta_p_e_p_p_1p = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{p}} (All Int., 1p, FD)",
                                       "#theta_{#vec{P}_{e},#vec{P}_{p}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{p} (All Int., 1p, FD);"
                                       "#theta_{#vec{P}_{e},#vec{P}_{p}} [Deg];", numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_p_e_p_p_1p_Dir = directories.Angle_Directory_map["Opening_angle_1p_Directory"];
    //</editor-fold>

// Theta_q_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (1p, FD only)">
    THStack *sTheta_q_p_p_1p = new THStack("#theta_{#vec{q},#vec{P}_{p}} (All Int., 1p, FD)",
                                           "#theta_{#vec{q},#vec{P}_{p}} - Opening Angle Between #vec{q} and #vec{P}_{p} (All Int., 1p, FD);#theta_{#vec{q},#vec{P}_{p}} [Deg];");
    TH1D *hTheta_q_p_p_1p = new TH1D("#theta_{#vec{q},#vec{P}_{p}} (All Int., 1p, FD)",
                                     "#theta_{#vec{q},#vec{P}_{p}} - Opening Angle Between #vec{q} and #vec{P}_{p} (All Int., 1p, FD);"
                                     "#theta_{#vec{q},#vec{P}_{p}} [Deg];", numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_p_1p_Dir = directories.Angle_Directory_map["Opening_angle_1p_Directory"];
    //</editor-fold>

// Theta_q_p_p vs. |P_p|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_p vs. |P_p|/|q| (1p, FD only)">
    TH2D *hTheta_q_p_p_vs_p_p_q_1p = new TH2D("#theta_{#vec{q},#vec{P}_{p}} vs. r=|P_{p}|/|q| (All Int., 1p, FD)",
                                              "#theta_{#vec{q},#vec{P}_{p}} vs. r=|#vec{P_{p}}|/|#vec{q}| (All Int., 1p, FD);r;#theta_{#vec{q},#vec{P}_{p}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_p_vs_p_p_q_1p_Dir = directories.Angle_Directory_map["Opening_angle_1p_Directory"];
    //</editor-fold>

// Theta_q_p_p vs. |p_N|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_p vs. |p_N|/|q| (1p, FD only)">
    TH2D *hTheta_q_p_p_vs_p_N_q_1p = new TH2D("#theta_{#vec{q},#vec{P}_{p}} vs. r=|P_{N}|/|q| (All Int., 1p, FD)",
                                              "#theta_{#vec{q},#vec{P}_{p}} vs. r=|#vec{P_{N}}|/|#vec{q}| (All Int., 1p, FD);r;#theta_{#vec{q},#vec{P}_{p}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_p_vs_p_N_q_1p_Dir = directories.Angle_Directory_map["Opening_angle_1p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1n)">

// Theta_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_n (1n, FD only)">
    THStack *sTheta_n_1n = new THStack("#theta_{n} (All Int., 1n, FD)", "#theta_{n} of Outgoing FD Neutron (All Int., 1n, FD);#theta_{n} [Deg];");
    TH1D *hTheta_n_All_Int_1n = new TH1D("#theta_{n} (All Int., 1n, FD)", "#theta_{n} of Outgoing FD Neutron (All Int., 1n, FD);#theta_{n} [Deg];",
                                         numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_n_All_Int_1n_Dir = directories.Angle_Directory_map["Theta_n_1n_Directory"];

    TH2D *hTheta_n_VS_P_n_1n_FD = new TH2D("#theta_{n} vs. P_{n} (All Int., 1n, FD)",
                                           "#theta_{n} vs. P_{n} (All Int., 1n, FD);P_{n} [GeV/c];#theta_{n} [Deg]",
                                           numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH2D *hTheta_n_VS_W_1n_FD = new TH2D("#theta_{n} vs. W (All Int., 1n, FD)",
                                         "#theta_{n} vs. W (All Int., 1n, FD);W = #sqrt{(#omega + m_{n})^{2} - #vec{q}^{2}}  [GeV];#theta_{n} [Deg]",
                                         numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                         Theta_uboundary_FD);
    string hTheta_n_VS_P_n_1n_FD_Dir = directories.Angle_Directory_map["Theta_n_1n_Directory"];
    string hTheta_n_VS_W_1n_FD_Dir = directories.Angle_Directory_map["Theta_n_1n_Directory"];
    //</editor-fold>

// Phi_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_n (1n, FD only)">
    THStack *sPhi_n_1n = new THStack("#phi_{n} (All Int., 1n, FD)", "#phi_{n} of Outgoing Neutron (All Int., 1n, FD);#phi_{n} [Deg];");
    TH1D *hPhi_n_All_Int_1n = new TH1D("#phi_{n} (All Int., 1n, FD)", "#phi_{n} of Outgoing Neutron (All Int., 1n, FD);#phi_{n} [Deg];",
                                       numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_n_All_Int_1n_Dir = directories.Angle_Directory_map["Phi_n_1n_Directory"];
    //</editor-fold>

// Theta_n vs. Phi_n ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_n vs. Phi_n histograms">
    TH2D *hTheta_n_VS_Phi_n_1n_FD = new TH2D("#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., 1n)",
                                             "#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., 1n);#phi_{nFD} [Deg];#theta_{nFD} [Deg]",
                                             numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                             numTH2Dbins_Nucleon_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_n_VS_Phi_n_1n_FD_Dir = directories.Angle_Directory_map["Theta_n_VS_Phi_n_1n_Directory"];
    //</editor-fold>

// Theta_p_e_p_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_n (1n, FD only)">
    THStack *sTheta_p_e_p_n_1n = new THStack("#theta_{#vec{P}_{e},#vec{P}_{n}} (All Int., 1n, FD)",
                                             "#theta_{#vec{P}_{e},#vec{P}_{n}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{n} (All Int., 1n, FD);#theta_{#vec{P}_{e},#vec{P}_{n}} [Deg];");
    TH1D *hTheta_p_e_p_n_1n = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{n}} (All Int., 1n, FD)",
                                       "#theta_{#vec{P}_{e},#vec{P}_{n}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{n} (All Int., 1n, FD);"
                                       "#theta_{#vec{P}_{e},#vec{P}_{n}} [Deg];", numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_p_e_p_n_1n_Dir = directories.Angle_Directory_map["Opening_angle_1n_Directory"];
    //</editor-fold>

// Theta_q_p_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n (1n, FD only)">
    THStack *sTheta_q_p_n_1n = new THStack("#theta_{#vec{q},#vec{P}_{n}} (All Int., 1n, FD)",
                                           "#theta_{#vec{q},#vec{P}_{n}} - Opening Angle Between #vec{q} and #vec{P}_{n} (All Int., 1n, FD);#theta_{#vec{q},#vec{P}_{n}} [Deg];");
    TH1D *hTheta_q_p_n_1n = new TH1D("#theta_{#vec{q},#vec{P}_{n}} (All Int., 1n, FD)",
                                     "#theta_{#vec{q},#vec{P}_{n}} - Opening Angle Between #vec{q} and #vec{P}_{n} (All Int., 1n, FD);"
                                     "#theta_{#vec{q},#vec{P}_{n}} [Deg];", numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_n_1n_Dir = directories.Angle_Directory_map["Opening_angle_1n_Directory"];
    //</editor-fold>

// Theta_q_p_n vs. |p_n|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n vs. |p_n|/|q| (1n, FD only)">
    TH2D *hTheta_q_p_n_vs_p_n_q_1n = new TH2D("#theta_{#vec{q},#vec{P}_{n}} vs. r=|P_{n}|/|q| (All Int., 1n, FD)",
                                              "#theta_{#vec{q},#vec{P}_{n}} vs. r=|#vec{P}_{n}|/|#vec{q}| (All Int., 1n, FD);r;#theta_{#vec{q},#vec{P}_{n}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_n_vs_p_n_q_1n_Dir = directories.Angle_Directory_map["Opening_angle_1n_Directory"];
    //</editor-fold>

// Theta_q_p_n vs. |p_N|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n vs. |p_N|/|q| (1n, FD only)">
    TH2D *hTheta_q_p_n_vs_p_N_q_1n = new TH2D("#theta_{#vec{q},#vec{P}_{n}} vs. r=|P_{N}|/|q| (All Int., 1n, FD)",
                                              "#theta_{#vec{q},#vec{P}_{n}} vs. r=|#vec{P_{N}}|/|#vec{q}| (All Int., 1n, FD);r;#theta_{#vec{q},#vec{P}_{n}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_n_vs_p_N_q_1n_Dir = directories.Angle_Directory_map["Opening_angle_1n_Directory"];
    //</editor-fold>

// Neutron veto plots (1n) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron veto plots (1n)">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} BV",
                                                              "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                              "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                              "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                              directories.Angle_Directory_map["Neutron_veto_1n_Directory"],
                                                              "01_Neutron_hits_vs_electron_hits_BV_1n", -180, 180, -50, 50, 65, 65);
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e} AV",
                                                              "'Neutron Hits' vs. Electron Hits - After Veto",
                                                              "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                              "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                              directories.Angle_Directory_map["Neutron_veto_1n_Directory"],
                                                              "02_Neutron_hits_vs_electron_hits_AV_1n", -180, 180, -50, 50, 65, 65);
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n = hPlot2D("1n", "FD", "#Delta#theta_{n,e} vs. #Delta#phi_{n,e}",
                                                                           "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                                                                           "#Delta#phi_{n,e} = #phi_{n}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                           "#Delta#theta_{n,e} = #theta_{n}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                           directories.Angle_Directory_map["Neutron_veto_1n_Directory"],
                                                                           "03_Neutron_hits_vs_electron_hits_Vetoed_1n", -180, 180, -50, 50, 65,
                                                                           65);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1e2pXy)">

// Phi of leading (p1) and recoil (p2) protons --------------------------------------------------------------------------------------------------------------------------

    //TODO: reorganize proprly

    //<editor-fold desc="Phi of leading (p1) and recoil (p2) protons">
    THStack *sPhi_Proton_1e2pXy = new THStack("#phi_{p} stack (1e2pXy, CD)", "#phi_{p} of Outgoing protons (1e2pXy, CD);#phi_{p} [Deg];");
    TH1D *hPhi_p1_1e2pXy_CD = new TH1D("#phi_{p_{1}} (1e2pXy, CD)", ";#phi_{p_{1}} [Deg];", numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_p2_1e2pXy_CD = new TH1D("#phi_{p_{2}} (1e2pXy, CD)", ";#phi_{p_{2}} [Deg];", numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
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
                                         "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];", numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_p_e_p_tot_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    //</editor-fold>

// Theta_q_p (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (2p, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_2p = new THStack("#theta_{#vec{q},#vec{P}_{tot}} (All Int., 2p)",
                                             "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);#theta_{#vec{q},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_q_p_tot_2p = new TH1D("#theta_{#vec{q},#vec{P}_{tot}} (All Int., 2p)",
                                       "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{1}+#vec{P}_{2} (All Int., 2p);"
                                       "#theta_{#vec{q},#vec{P}_{tot}} [Deg];", numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_tot_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (2p, CD & FD)">
    THStack *sTheta_q_p_2p = new THStack("#theta_{#vec{q},#vec{P}} (All Int., 2p)",
                                         "#theta_{#vec{q},#vec{P}} - Opening Angle Between #vec{q} and #vec{P} (All Int., 2p);#theta_{#vec{q},#vec{P}} [Deg];");
    TH1D *hTheta_q_p_L_2p = new TH1D("#theta_{#vec{q},#vec{P}_{1}} (All Int., 2p)",
                                     "#theta_{#vec{q},#vec{P}_{1}} - Opening Angle Between #vec{q} and leading proton #vec{P}_{1} (All Int., 2p);#theta_{#vec{q},#vec{P}_{1}}",
                                     numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_q_p_R_2p = new TH1D("#theta_{#vec{q},#vec{P}_{2}} (All Int., 2p)",
                                     "#theta_{#vec{q},#vec{P}_{2}} - Opening Angle Between #vec{q} and recoil proton #vec{P}_{2} (All Int., 2p);#theta_{#vec{q},#vec{P}_{2}}",
                                     numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_L_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    string hTheta_q_p_R_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (2p, CD & FD) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    TH2D *hTheta_q_p_L_vs_p_L_q_2p = new TH2D("#theta_{#vec{q},#vec{P}_{1}} vs. r_{1} (All Int., 2p)",
                                              "#theta_{#vec{q},#vec{P}_{1}} vs. r_{1}=|#vec{P_{1}}|/|#vec{q}| (All Int., 2p);r_{1};#theta_{#vec{q},#vec{P}_{1}}",
                                              numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_L_vs_p_L_q_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    //</editor-fold>

// Theta_p1_p2 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 (CD & FD)">
    THStack *sTheta_p1_p2_2p = new THStack("#theta_{p_{1},p_{2}} (All Int., stack, 2p)",
                                           "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (2p);#theta_{p_{1},p_{2}} [Deg];");
    TH1D *hTheta_p1_p2_All_Int_2p = new TH1D("#theta_{p_{1},p_{2}} (All Int., 2p)",
                                             "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (All Int., 2p);#theta_{p_{1},p_{2}} [Deg];",
                                             numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_p1_p2_QEL_2p = new TH1D("#theta_{p_{1},p_{2}} (QEL only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (QEL only, 2p);#theta_{p_{1},p_{2}} [Deg];",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_p1_p2_MEC_2p = new TH1D("#theta_{p_{1},p_{2}} (MEC only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (MEC only, 2p);#theta_{p_{1},p_{2}} [Deg];",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_p1_p2_RES_2p = new TH1D("#theta_{p_{1},p_{2}} (RES only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (RES only, 2p);#theta_{p_{1},p_{2}} [Deg];",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_p1_p2_DIS_2p = new TH1D("#theta_{p_{1},p_{2}} (DIS only, 2p)",
                                         "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (DIS only, 2p);#theta_{p_{1},p_{2}} [Deg];",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    string sTheta_p1_p2_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    string hTheta_p1_p2_All_Int_2p_Dir = directories.Angle_Directory_map["Opening_angle_by_interaction_2p_Directory"];
    string hTheta_p1_p2_QEL_2p_Dir = directories.Angle_Directory_map["Opening_angle_by_interaction_2p_Directory"];
    string hTheta_p1_p2_MEC_2p_Dir = directories.Angle_Directory_map["Opening_angle_by_interaction_2p_Directory"];
    string hTheta_p1_p2_RES_2p_Dir = directories.Angle_Directory_map["Opening_angle_by_interaction_2p_Directory"];
    string hTheta_p1_p2_DIS_2p_Dir = directories.Angle_Directory_map["Opening_angle_by_interaction_2p_Directory"];
    //</editor-fold>

// Theta_p1_p2 vs. W (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. W (CD & FD)">
    TH2D *hTheta_p1_p2_vs_W_2p = new TH2D("#theta_{p_{1},p_{2}} vs. W (All Int., 2p)",
                                          "#theta_{p_{1},p_{2}} vs. W (All Int., 2p);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{p_{1},p_{2}} [Deg];",
                                          numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_p1_p2_vs_W_2p_Dir = directories.Angle_Directory_map["Opening_angle_2p_Directory"];
    //</editor-fold>

// Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (CD & FD)">
    TH2D *hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p = new TH2D(
            "#theta_{p_{1}} vs. #theta_{p_{1}} for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p)",
            "#theta_{p_{1}} vs. #theta_{p_{2}} for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p);#theta_{p_{2}} [Deg];"
            "#theta_{p_{1}} [Deg];", numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50);
    string hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (2p, CD & FD) ----------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (CD & FD)">
    TH2D *hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p = new TH2D("#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)",
                                                                     "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p);#theta_{pCD} [Deg];"
                                                                     "#theta_{pFD} [Deg];", numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots,
                                                                     30, 50);
    string hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// dPhi_p1_p2 for Theta_p1_p2 < 20 (2p, CD & FD) ------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_p1_p2 for Theta_p1_p2 < 20 (CD & FD)">
    TH1D *hdPhi_p1_p2_for_Theta_p1_p2_20_2p = new TH1D("#Delta#phi for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p)",
                                                       "#Delta#phi for #theta_{p_{1},p_{2}}<20#circ (All Int., 2p);"
                                                       "#Delta#phi = #phi_{p,1} - #phi_{p,2} [Deg];", numTH1Dbins_Ang_Plots, Phi_lboundary,
                                                       Phi_uboundary);
    string hdPhi_p1_p2_for_Theta_p1_p2_20_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p = new TH1D("#Delta#phi for #theta_{p_{1},p_{2}}<20#circ - ZOOMIN (All Int., 2p)",
                                                              "#Delta#phi for #theta_{p_{1},p_{2}}<20#circ - ZOOMIN (All Int., 2p);"
                                                              "#Delta#phi = #phi_{p,1} - #phi_{p,2} [Deg];", numTH1Dbins_Ang_Plots, -25, 25);
    string hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// dPhi_pFD_pCD for Theta_pFD_pCD < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_p1_p2 for Theta_pFD_pCD < 20 (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p = new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., 2p)",
                                                           "#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., 2p);"
                                                           "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p = new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., 2p)",
                                                                  "#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., 2p);"
                                                                  "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (CD & FD)">
    TH2D *hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p = new TH2D("#theta_{p_{1}} vs. #theta_{p_{1}} #forall#theta_{p_{1},p_{2}} (All Int., 2p)",
                                                                 "#theta_{p_{1}} vs. #theta_{p_{2}} for every #theta_{p_{1},p_{2}} (All Int., 2p);#theta_{p_{2}} [Deg];"
                                                                 "#theta_{p_{1}} [Deg];", numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30,
                                                                 50);
    string hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (2p, CD & FD) ---------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (CD & FD)">
    TH2D *hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p = new TH2D("#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)",
                                                                     "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p);#theta_{pCD} [Deg];"
                                                                     "#theta_{pFD} [Deg];", numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots,
                                                                     30, 50);
    string hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// dPhi_p1_p2 for every Theta_p1_p2 (2p, CD & FD) -----------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_p1_p2 for every Theta_p1_p2 (CD & FD)">
    TH1D *hdPhi_p1_p2_for_all_Theta_p1_p2_2p = new TH1D("#Delta#phi #forall#theta_{p_{1},p_{2}} (All Int., 2p)",
                                                        "#Delta#phi for every #theta_{p_{1},p_{2}} (All Int., 2p);"
                                                        "#Delta#phi = #phi_{p,1} - #phi_{p,2} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_p1_p2_for_all_Theta_p1_p2_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p = new TH1D("#Delta#phi #forall#theta_{p_{1},p_{2}} - ZOOMIN (All Int., 2p)",
                                                               "#Delta#phi for every #theta_{p_{1},p_{2}} - ZOOMIN(All Int., 2p);"
                                                               "#Delta#phi = #phi_{p,1} - #phi_{p,2} [Deg];", 50, -40, 40);
    string hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_p1_p2_for_small_dTheta_2p = new TH1D("#Delta#phi for small #Delta#theta_{1/2} (All Int., 2p)",
                                                     "#Delta#phi for small #Delta#theta_{1/2} = |#theta_{1/2}-40#circ|;"
                                                     "#Delta#phi = #phi_{p,1} - #phi_{p,2} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_p1_p2_for_small_dTheta_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p = new TH1D("#Delta#phi for small #Delta#theta_{1/2} - ZOOMIN (All Int., 2p)",
                                                            "#Delta#phi for small #Delta#theta_{1/2} = |#theta_{1/2}-40#circ| - ZOOMIN;"
                                                            "#Delta#phi = #phi_{p,1} - #phi_{p,2} [Deg];", 50, -40, 40);
    string hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// dPhi_pFD_pCD for every Theta_pFD_pCD (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_pFD_pCD for every Theta_pFD_pCD (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p = new TH1D("#Delta#phi for #theta_{pFD,pCD} (All Int., 2p)",
                                                            "#Delta#phi for #theta_{pFD,pCD} (All Int., 2p);"
                                                            "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p = new TH1D("#Delta#phi for #theta_{pFD,pCD} - ZOOMIN (All Int., 2p)",
                                                                   "#Delta#phi for #theta_{pFD,pCD} - ZOOMIN (All Int., 2p);"
                                                                   "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_2p = new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} (All Int., 2p)",
                                                       "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ|;"
                                                       "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_pFD_pCD_for_small_dTheta_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p = new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN (All Int., 2p)",
                                                              "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ| - ZOOMIN;"
                                                              "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p_Dir = directories.Angle_Directory_map["Double_detection_2p_Directory"];
    //</editor-fold>

// Ghost tracks handling (2p, CD only) ----------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p)">
    hPlot2D hTheta_p1_p2_VS_ToF1_ToF2_AC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC",
                                                      "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC",
                                                      "#theta_{p_{1},p_{2}} [Deg]", "ToF_{1}-ToF_{2} [ns]",
                                                      directories.Angle_Directory_map["CToF_hits_2p_Directory"],
                                                      "01_Theta_p1_p2_VS_ToF1-ToF2_AC_2p", 0, 180, -3, 3, numTH2Dbins_Ang_Plots,
                                                      numTH2Dbins_Ang_Plots);
    //</editor-fold>

    //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p)">
    hPlot2D hTheta_p1_p2_VS_Pos1_Pos2_AC_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC",
                                                      "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC", "#theta_{p_{1},p_{2}} [Deg]",
                                                      "Position_{1}-Position_{2} [cm]", directories.Angle_Directory_map["CToF_hits_2p_Directory"],
                                                      "02_Theta_p1_p2_VS_Pos1-Pos2_AC_2p", 0, 180, 0, 100, numTH2Dbins_Ang_Plots,
                                                      numTH2Dbins_Ang_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (pFDpCD)">

    //<editor-fold desc="Nucleons' angles plots (pFDpCD)">

// Theta_pFD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD histograms">
    THStack *sTheta_pFD_pFDpCD_FD = new THStack("#theta_{pFD} (pFDpCD)", "#theta_{pFD} of FD proton (pFDpCD);#theta_{pFD} [Deg];");

    TH1D *hTheta_pFD_All_Int_pFDpCD_FD = new TH1D("#theta_{pFD} (All Int., pFDpCD)",
                                                  "#theta_{pFD} of FD proton (All Int., pFDpCD);#theta_{pFD} [Deg];",
                                                  numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_pFD_QEL_pFDpCD_FD = new TH1D("#theta_{pFD} (QEL Only, pFDpCD)",
                                              "#theta_{pFD} of FD proton (QEL Only, pFDpCD);#theta_{pFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_pFD_MEC_pFDpCD_FD = new TH1D("#theta_{pFD} (MEC Only, pFDpCD)",
                                              "#theta_{pFD} of FD proton (MEC Only, pFDpCD);#theta_{pFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_pFD_RES_pFDpCD_FD = new TH1D("#theta_{pFD} (RES Only, pFDpCD)",
                                              "#theta_{pFD} of FD proton (RES Only, pFDpCD);#theta_{pFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_pFD_DIS_pFDpCD_FD = new TH1D("#theta_{pFD} (DIS Only, pFDpCD)",
                                              "#theta_{pFD} of FD proton (DIS Only, pFDpCD);#theta_{pFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_pFD_All_Int_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];
    string hTheta_pFD_QEL_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];
    string hTheta_pFD_MEC_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];
    string hTheta_pFD_RES_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];
    string hTheta_pFD_DIS_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];

    TH2D *hTheta_pFD_VS_P_pFD_pFDpCD_FD = new TH2D("#theta_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)",
                                                   "#theta_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD);P_{pFD} [GeV/c];#theta_{pFD} [Deg]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                                   Theta_uboundary_FD);
    TH2D *hTheta_pFD_VS_W_pFDpCD_FD = new TH2D("#theta_{pFD} vs. W (All Int., pFDpCD, FD)",
                                               "#theta_{pFD} vs. W (All Int., pFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{pFD} [Deg]",
                                               numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                               Theta_uboundary_FD);
    string hTheta_pFD_VS_P_pFD_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];
    string hTheta_pFD_VS_W_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_pFDpCD_Directory"];
    //</editor-fold>

// Phi_pFD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_pFD histograms">
    THStack *sPhi_pFD_pFDpCD_FD = new THStack("#phi_{pFD} (pFDpCD)", "#phi_{pFD} of FD proton (pFDpCD);#phi_{pFD} [Deg];");

    TH1D *hPhi_pFD_All_Int_pFDpCD_FD = new TH1D("#phi_{pFD} (All Int., pFDpCD)", "#phi_{pFD} of FD proton (All Int., pFDpCD);#phi_{pFD} [Deg];",
                                                numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pFD_QEL_pFDpCD_FD = new TH1D("#phi_{pFD} (QEL Only, pFDpCD)", "#phi_{pFD} of FD proton (QEL Only, pFDpCD);#phi_{pFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pFD_MEC_pFDpCD_FD = new TH1D("#phi_{pFD} (MEC Only, pFDpCD)", "#phi_{pFD} of FD proton (MEC Only, pFDpCD);#phi_{pFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pFD_RES_pFDpCD_FD = new TH1D("#phi_{pFD} (RES Only, pFDpCD)", "#phi_{pFD} of FD proton (RES Only, pFDpCD);#phi_{pFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pFD_DIS_pFDpCD_FD = new TH1D("#phi_{pFD} (DIS Only, pFDpCD)", "#phi_{pFD} of FD proton (DIS Only, pFDpCD);#phi_{pFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_pFD_All_Int_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];
    string hPhi_pFD_QEL_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];
    string hPhi_pFD_MEC_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];
    string hPhi_pFD_RES_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];
    string hPhi_pFD_DIS_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];

    TH2D *hPhi_pFD_VS_P_pFD_pFDpCD_FD = new TH2D("#phi_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)",
                                                 "#phi_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD);P_{pFD} [GeV/c];#phi_{pFD} [Deg]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_pFD_VS_W_pFDpCD_FD = new TH2D("#phi_{pFD} vs. W (All Int., pFDpCD, FD)",
                                             "#phi_{pFD} vs. W (All Int., pFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                             "#phi_{pFD} [Deg]", numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                             Phi_lboundary,
                                             Phi_uboundary);
    string hPhi_pFD_VS_P_pFD_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];
    string hPhi_pFD_VS_W_pFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_pFD_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pFD vs. Phi_pFD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD vs. Phi_pFD histograms">
    TH2D *hTheta_pFD_VS_Phi_pFD_pFDpCD_FD = new TH2D("#theta_{pFD} vs. #phi_{pFD} of FD proton (All Int., pFDpCD)",
                                                     "#theta_{pFD} vs. #phi_{pFD} of FD proton (All Int., pFDpCD);#phi_{pFD} [Deg];#theta_{pFD} [Deg]",
                                                     numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                     numTH2Dbins_Nucleon_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_pFD_VS_Phi_pFD_pFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_pFD_VS_Phi_pFD_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pCD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD histograms">
    THStack *sTheta_pCD_pFDpCD_CD = new THStack("#theta_{pCD} (pFDpCD)", "#theta_{pCD} of CD proton (pFDpCD);#theta_{pCD} [Deg];");

    TH1D *hTheta_pCD_All_Int_pFDpCD_CD = new TH1D("#theta_{pCD} (All Int., pFDpCD)",
                                                  "#theta_{pCD} of CD proton (All Int., pFDpCD);#theta_{pCD} [Deg];",
                                                  numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_QEL_pFDpCD_CD = new TH1D("#theta_{pCD} (QEL Only, pFDpCD)",
                                              "#theta_{pCD} of CD proton (QEL Only, pFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_MEC_pFDpCD_CD = new TH1D("#theta_{pCD} (MEC Only, pFDpCD)",
                                              "#theta_{pCD} of CD proton (MEC Only, pFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_RES_pFDpCD_CD = new TH1D("#theta_{pCD} (RES Only, pFDpCD)",
                                              "#theta_{pCD} of CD proton (RES Only, pFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_DIS_pFDpCD_CD = new TH1D("#theta_{pCD} (DIS Only, pFDpCD)",
                                              "#theta_{pCD} of CD proton (DIS Only, pFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    string hTheta_pCD_All_Int_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];
    string hTheta_pCD_QEL_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];
    string hTheta_pCD_MEC_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];
    string hTheta_pCD_RES_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];
    string hTheta_pCD_DIS_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];

    TH2D *hTheta_pCD_VS_P_pCD_pFDpCD_CD = new TH2D("#theta_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
                                                   "#theta_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD);P_{pCD} [GeV/c];#theta_{pCD} [Deg]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, 30, 155);
    TH2D *hTheta_pCD_VS_W_pFDpCD_CD = new TH2D("#theta_{pCD} vs. W (All Int., pFDpCD, CD)",
                                               "#theta_{pCD} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{pCD} [Deg]",
                                               numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 30, 155);
    string hTheta_pCD_VS_P_pCD_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];
    string hTheta_pCD_VS_W_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_pFDpCD_Directory"];
    //</editor-fold>

// Phi_pCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_pCD histograms">
    THStack *sPhi_pCD_pFDpCD_CD = new THStack("#phi_{pCD} (pFDpCD)", "#phi_{pCD} of CD proton (pFDpCD);#phi_{pCD} [Deg];");

    TH1D *hPhi_pCD_All_Int_pFDpCD_CD = new TH1D("#phi_{pCD} (All Int., pFDpCD)", "#phi_{pCD} of CD proton (All Int., pFDpCD);#phi_{pCD} [Deg];",
                                                numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_QEL_pFDpCD_CD = new TH1D("#phi_{pCD} (QEL Only, pFDpCD)", "#phi_{pCD} of CD proton (QEL Only, pFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_MEC_pFDpCD_CD = new TH1D("#phi_{pCD} (MEC Only, pFDpCD)", "#phi_{pCD} of CD proton (MEC Only, pFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_RES_pFDpCD_CD = new TH1D("#phi_{pCD} (RES Only, pFDpCD)", "#phi_{pCD} of CD proton (RES Only, pFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_DIS_pFDpCD_CD = new TH1D("#phi_{pCD} (DIS Only, pFDpCD)", "#phi_{pCD} of CD proton (DIS Only, pFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_pCD_All_Int_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];
    string hPhi_pCD_QEL_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];
    string hPhi_pCD_MEC_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];
    string hPhi_pCD_RES_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];
    string hPhi_pCD_DIS_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];

    TH2D *hPhi_pCD_VS_P_pCD_pFDpCD_CD = new TH2D("#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
                                                 "#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD);P_{pCD} [GeV/c];#phi_{pCD} [Deg]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_pCD_VS_W_pFDpCD_CD = new TH2D("#phi_{pCD} vs. W (All Int., pFDpCD, CD)",
                                             "#phi_{pCD} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{pCD} [Deg]",
                                             numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_pCD_VS_P_pCD_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];
    string hPhi_pCD_VS_W_pFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pCD vs. Phi_pCD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD vs. Phi_pCD histograms">
    TH2D *hTheta_pCD_VS_Phi_pCD_pFDpCD_CD = new TH2D("#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., pFDpCD)",
                                                     "#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., pFDpCD);#phi_{pCD} [Deg];#theta_{pCD} [Deg]",
                                                     numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_Nucleon_Ang_Plots,
                                                     30, 155);
    string hTheta_pCD_VS_Phi_pCD_pFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_VS_Phi_pCD_pFDpCD_Directory"];
    //</editor-fold>

// Theta_tot ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot histograms">
    THStack *sTheta_tot_pFDpCD = new THStack("#theta_{tot} (pFDpCD)", "#theta_{tot} of total 3-momentum (pFDpCD);#theta_{tot} [Deg];");

    TH1D *hTheta_tot_All_Int_pFDpCD = new TH1D("#theta_{tot} (All Int., pFDpCD)",
                                               "#theta_{tot} of total 3-momentum (All Int., pFDpCD);#theta_{tot} [Deg];",
                                               numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_QEL_pFDpCD = new TH1D("#theta_{tot} (QEL Only, pFDpCD)",
                                           "#theta_{tot} of total 3-momentum (QEL Only, pFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_MEC_pFDpCD = new TH1D("#theta_{tot} (MEC Only, pFDpCD)",
                                           "#theta_{tot} of total 3-momentum (MEC Only, pFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_RES_pFDpCD = new TH1D("#theta_{tot} (RES Only, pFDpCD)",
                                           "#theta_{tot} of total 3-momentum (RES Only, pFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_DIS_pFDpCD = new TH1D("#theta_{tot} (DIS Only, pFDpCD)",
                                           "#theta_{tot} of total 3-momentum (DIS Only, pFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_tot_All_Int_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];
    string hTheta_tot_QEL_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];
    string hTheta_tot_MEC_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];
    string hTheta_tot_RES_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];
    string hTheta_tot_DIS_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];

    TH2D *hTheta_tot_VS_P_tot_pFDpCD = new TH2D("#theta_{tot} vs. P_{tot} (All Int., pFDpCD, CD)",
                                                "#theta_{tot} vs. P_{tot} (All Int., pFDpCD, CD);P_{tot} [GeV/c];#theta_{tot} [Deg]",
                                                numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                                Opening_Ang_narrow_uboundary);
    TH2D *hTheta_tot_VS_W_pFDpCD = new TH2D("#theta_{tot} vs. W (All Int., pFDpCD, CD)",
                                            "#theta_{tot} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{tot} [Deg]",
                                            numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                            Opening_Ang_narrow_uboundary);
    string hTheta_tot_VS_P_tot_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];
    string hTheta_tot_VS_W_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_pFDpCD_Directory"];
    //</editor-fold>

// Phi_tot --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_tot histograms">
    THStack *sPhi_tot_pFDpCD = new THStack("#phi_{tot} (pFDpCD)", "#phi_{tot} of total 3-momentum (pFDpCD);#phi_{tot} [Deg];");

    TH1D *hPhi_tot_All_Int_pFDpCD = new TH1D("#phi_{tot} (All Int., pFDpCD)",
                                             "#phi_{tot} of total 3-momentum (All Int., pFDpCD);#phi_{tot} [Deg];",
                                             numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_QEL_pFDpCD = new TH1D("#phi_{tot} (QEL Only, pFDpCD)", "#phi_{tot} of total 3-momentum (QEL Only, pFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_MEC_pFDpCD = new TH1D("#phi_{tot} (MEC Only, pFDpCD)", "#phi_{tot} of total 3-momentum (MEC Only, pFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_RES_pFDpCD = new TH1D("#phi_{tot} (RES Only, pFDpCD)", "#phi_{tot} of total 3-momentum (RES Only, pFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_DIS_pFDpCD = new TH1D("#phi_{tot} (DIS Only, pFDpCD)", "#phi_{tot} of total 3-momentum (DIS Only, pFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_tot_All_Int_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];
    string hPhi_tot_QEL_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];
    string hPhi_tot_MEC_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];
    string hPhi_tot_RES_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];
    string hPhi_tot_DIS_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];

    TH2D *hPhi_tot_VS_P_tot_pFDpCD = new TH2D("#phi_{tot} vs. P_{tot} (All Int., pFDpCD, CD)",
                                              "#phi_{tot} vs. P_{tot} (All Int., pFDpCD, CD);P_{tot} [GeV/c];#phi_{tot} [Deg]",
                                              numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_tot_VS_W_pFDpCD = new TH2D("#phi_{tot} vs. W (All Int., pFDpCD, CD)",
                                          "#phi_{tot} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{tot} [Deg]",
                                          numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_tot_VS_P_tot_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];
    string hPhi_tot_VS_W_pFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_pFDpCD_Directory"];
    //</editor-fold>

// Theta_tot vs. Phi_tot ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot vs. Phi_tot histograms">
    TH2D *hTheta_tot_VS_Phi_tot_pFDpCD = new TH2D("#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., pFDpCD)",
                                                  "#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., pFDpCD);#phi_{tot} [Deg];#theta_{tot} [Deg]",
                                                  numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                  numTH2Dbins_Nucleon_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_tot_VS_Phi_tot_pFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_VS_Phi_tot_pFDpCD_Directory"];
    //</editor-fold>

// Theta_rel ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel histograms">
    THStack *sTheta_rel_pFDpCD = new THStack("#theta_{rel} (pFDpCD)", "#theta_{rel} of relative 3-momentum (pFDpCD);#theta_{rel} [Deg];");

    TH1D *hTheta_rel_All_Int_pFDpCD = new TH1D("#theta_{rel} (All Int., pFDpCD)",
                                               "#theta_{rel} of relative 3-momentum (All Int., pFDpCD);#theta_{rel} [Deg];",
                                               numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_QEL_pFDpCD = new TH1D("#theta_{rel} (QEL Only, pFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (QEL Only, pFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_MEC_pFDpCD = new TH1D("#theta_{rel} (MEC Only, pFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (MEC Only, pFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_RES_pFDpCD = new TH1D("#theta_{rel} (RES Only, pFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (RES Only, pFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_DIS_pFDpCD = new TH1D("#theta_{rel} (DIS Only, pFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (DIS Only, pFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    string hTheta_rel_All_Int_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];
    string hTheta_rel_QEL_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];
    string hTheta_rel_MEC_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];
    string hTheta_rel_RES_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];
    string hTheta_rel_DIS_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];

    TH2D *hTheta_rel_VS_P_rel_pFDpCD = new TH2D("#theta_{rel} vs. P_{rel} (All Int., pFDpCD, CD)",
                                                "#theta_{rel} vs. P_{rel} (All Int., pFDpCD, CD);P_{rel} [GeV/c];#theta_{rel} [Deg]",
                                                numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, 30, 155);
    TH2D *hTheta_rel_VS_W_pFDpCD = new TH2D("#theta_{rel} vs. W (All Int., pFDpCD, CD)",
                                            "#theta_{rel} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{rel} [Deg]",
                                            numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 30, 155);
    string hTheta_rel_VS_P_rel_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];
    string hTheta_rel_VS_W_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_pFDpCD_Directory"];
    //</editor-fold>

// Phi_rel --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_rel histograms">
    THStack *sPhi_rel_pFDpCD = new THStack("#phi_{rel} (pFDpCD)", "#phi_{rel} of relative 3-momentum (pFDpCD);#phi_{rel} [Deg];");

    TH1D *hPhi_rel_All_Int_pFDpCD = new TH1D("#phi_{rel} (All Int., pFDpCD)",
                                             "#phi_{rel} of relative 3-momentum (All Int., pFDpCD);#phi_{rel} [Deg];",
                                             numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_QEL_pFDpCD = new TH1D("#phi_{rel} (QEL Only, pFDpCD)", "#phi_{rel} of relative 3-momentum (QEL Only, pFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_MEC_pFDpCD = new TH1D("#phi_{rel} (MEC Only, pFDpCD)", "#phi_{rel} of relative 3-momentum (MEC Only, pFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_RES_pFDpCD = new TH1D("#phi_{rel} (RES Only, pFDpCD)", "#phi_{rel} of relative 3-momentum (RES Only, pFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_DIS_pFDpCD = new TH1D("#phi_{rel} (DIS Only, pFDpCD)", "#phi_{rel} of relative 3-momentum (DIS Only, pFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_rel_All_Int_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];
    string hPhi_rel_QEL_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];
    string hPhi_rel_MEC_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];
    string hPhi_rel_RES_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];
    string hPhi_rel_DIS_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];

    TH2D *hPhi_rel_VS_P_rel_pFDpCD = new TH2D("#phi_{rel} vs. P_{rel} (All Int., pFDpCD, CD)",
                                              "#phi_{rel} vs. P_{rel} (All Int., pFDpCD, CD);P_{rel} [GeV/c];#phi_{rel} [Deg]",
                                              numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_rel_VS_W_pFDpCD = new TH2D("#phi_{rel} vs. W (All Int., pFDpCD, CD)",
                                          "#phi_{rel} vs. W (All Int., pFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{rel} [Deg]",
                                          numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_rel_VS_P_rel_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];
    string hPhi_rel_VS_W_pFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_pFDpCD_Directory"];
    //</editor-fold>

// Theta_rel vs. Phi_rel ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel vs. Phi_rel histograms">
    TH2D *hTheta_rel_VS_Phi_rel_pFDpCD = new TH2D("#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., pFDpCD)",
                                                  "#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., pFDpCD);#phi_{rel} [Deg];#theta_{rel} [Deg]",
                                                  numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_Nucleon_Ang_Plots, 30,
                                                  155);
    string hTheta_rel_VS_Phi_rel_pFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_VS_Phi_rel_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_p_e_p_tot (pFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (pFDpCD, CD & FD)">
    THStack *sTheta_p_e_p_tot_pFDpCD = new THStack("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., pFDpCD)",
                                                   "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{pFD}+#vec{P}_{pCD} "
                                                   "(All Int., pFDpCD);#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_p_e_p_tot_pFDpCD = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., pFDpCD)",
                                             "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{pFD}+#vec{P}_{pCD} "
                                             "(All Int., pFDpCD);#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];",
                                             numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_p_e_p_tot_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

// Theta_p_e_p_tot vs. W (pFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot vs. W (pFDpCD)">
    TH2D *hTheta_p_e_p_tot_vs_W_pFDpCD = new TH2D("#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., pFDpCD)",
                                                  "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                                  "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];",
                                                  numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                                  Opening_Ang_narrow_lboundary,
                                                  Opening_Ang_narrow_uboundary);
    string hTheta_p_e_p_tot_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

// Theta_q_p_tot (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (pFDpCD, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_pFDpCD = new THStack("#theta_{#vec{q},#vec{P}_{tot}} (All Int., pFDpCD)",
                                                 "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{pFD}+#vec{P}_{pCD} "
                                                 "(All Int., pFDpCD);#theta_{#vec{q},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_q_p_tot_pFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{tot}} (All Int., pFDpCD)",
                                           "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{pFD}+#vec{P}_{pCD} "
                                           "(All Int., pFDpCD);#theta_{#vec{q},#vec{P}_{tot}} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_q_p_tot_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_max (CD & FD)">
    TH1D *hTheta_q_p_max_pFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{max}} (All Int., pFDpCD)",
                                           "#theta_{#vec{q},#vec{P}_{max}} - Opening Angle Between #vec{q} and #vec{P}_{max} "
                                           "(All Int., pFDpCD);#theta_{#vec{q},#vec{P}_{max}} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_q_p_max_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_P_pL_minus_q_pR and Theta_q_p_R (pFDpCD, CD & FD)">
    THStack *sTheta_P_pL_minus_q_pR_pFDpCD = new THStack("#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
                                                         "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} - Opening Angle Between #vec{P}_{pL}-#vec{q} and #vec{P}_{pR} (All Int., pFDpCD);"
                                                         "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} [Deg];");
    TH1D *hTheta_P_pL_minus_q_pR_pFDpCD = new TH1D("#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
                                                   "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} - Opening Angle Between #vec{P}_{pL}-#vec{q} and #vec{P}_{pR} (All Int., pFDpCD)"
                                                   ";#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} [Deg]",
                                                   numTH1Dbins_Ang_Plots, Opening_Ang_wide_lboundary, Opening_Ang_wide_uboundary);
    string hTheta_P_pL_minus_q_pR_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (pFDpCD, CD & FD)">
    THStack *sTheta_q_p_L_R_pFDpCD = new THStack("#theta_{#vec{q},#vec{P}} (All Int., pFDpCD)",
                                                 "#theta_{#vec{q},#vec{P}} - Opening Angle Between #vec{q} and #vec{P} (All Int., pFDpCD);#theta_{#vec{q},#vec{P}} [Deg];");
    TH1D *hTheta_q_p_L_pFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{pL}} (All Int., pFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{pL}} - Opening Angle Between #vec{q} and Leading Proton #vec{P}_{pL} (All Int., pFDpCD)"
                                         ";#theta_{#vec{q},#vec{P}_{pL}} [Deg]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_q_p_R_pFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{pR}} - Opening Angle Between #vec{q} and Recoil Proton #vec{P}_{pR} (All Int., pFDpCD)"
                                         ";#theta_{#vec{q},#vec{P}_{pR}} [Deg]",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_L_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    string hTheta_q_p_R_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_pFD and Theta_q_pCD (pFDpCD, CD & FD)">
    THStack *sTheta_q_p_pFDpCD = new THStack("#theta_{#vec{q},#vec{P}} (All Int., pFDpCD)",
                                             "#theta_{#vec{q},#vec{P}} - Opening Angle Between #vec{q} and #vec{P} (All Int., pFDpCD);#theta_{#vec{q},#vec{P}} [Deg];");
    TH1D *hTheta_q_pFD_pFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{pFD}} (All Int., pFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{pFD}} - Opening Angle Between #vec{q} and FD Proton #vec{P}_{pFD} (All Int., pFDpCD)"
                                         ";#theta_{#vec{q},#vec{P}_{pFD}} [Deg]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_q_pCD_pFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{pCD}} (All Int., pFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{pCD}} - Opening Angle Between #vec{q} and CD Proton #vec{P}_{pCD} (All Int., pFDpCD)"
                                         ";#theta_{#vec{q},#vec{P}_{pCD}} [Deg]",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_pFD_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    string hTheta_q_pCD_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_tot vs. W (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_tot vs. W (CD & FD)">
    TH2D *hTheta_q_p_tot_vs_W_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., pFDpCD)",
                                                "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                                "#theta_{#vec{q},#vec{P}_{tot}} [Deg];",
                                                numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                                Opening_Ang_narrow_lboundary,
                                                Opening_Ang_narrow_uboundary);
    string hTheta_q_p_tot_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_P_pL_minus_q_pR vs. W (CD & FD)">
    TH2D *hTheta_P_pL_minus_q_pR_vs_W_pFDpCD = new TH2D("#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} vs. W (All Int., pFDpCD)",
                                                        "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                                        "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} [Deg];",
                                                        numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                                        Opening_Ang_wide_lboundary,
                                                        Opening_Ang_wide_uboundary);
    string hTheta_P_pL_minus_q_pR_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L vs. W (CD & FD)">
    TH2D *hTheta_q_p_L_vs_W_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pL}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{pL}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{pL}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                              Opening_Ang_narrow_uboundary);
    string hTheta_q_p_L_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_R vs. W (CD & FD)">
    TH2D *hTheta_q_p_R_vs_W_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pR}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{pR}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{pR}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_R_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_pFD vs. W (CD & FD)">
    TH2D *hTheta_q_pFD_vs_W_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pFD}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{pFD}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{pFD}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                              Opening_Ang_narrow_uboundary);
    string hTheta_q_pFD_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_pCD vs. W (CD & FD)">
    TH2D *hTheta_q_pCD_vs_W_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., pFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{pCD}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_pCD_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

// Theta_q_p vs |P_p|/|q| (pFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p vs |P_p|/|q| (CD & FD)">

    //<editor-fold desc="Theta_q_p_L vs |P_p_L|/|q|">
    TH2D *hTheta_q_p_L_vs_p_L_q_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL} (All Int., pFDpCD)",
                                                  "#theta_{#vec{q},#vec{P}_{pL}} vs. r_{pL}=|#vec{P}_{pL}|/|#vec{q}| (All Int., pFDpCD);"
                                                  "r_{pL};#theta_{#vec{q},#vec{P}_{pL}} [Deg]",
                                                  numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                                  Opening_Ang_narrow_uboundary);
    string hTheta_q_p_L_vs_p_L_q_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_q_p vs Theta_q_p (pFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_L vs Theta_q_p_R">
    TH2D *hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
                                                        "#theta_{#vec{q},#vec{P}_{pL}} vs. #theta_{#vec{q},#vec{P}_{pR}} (All Int., pFDpCD);"
                                                        "#theta_{#vec{q},#vec{P}_{pL}};#theta_{#vec{q},#vec{P}_{pR}} [Deg]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary,
                                                        numTH2Dbins_Ang_Plots, 0,
                                                        180);
    string hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_pFD vs Theta_q_pCD">
    TH2D *hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}} (All Int., pFDpCD)",
                                                        "#theta_{#vec{q},#vec{P}_{pFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}} (All Int., pFDpCD);"
                                                        "#theta_{#vec{q},#vec{P}_{pFD}} [Deg];#theta_{#vec{q},#vec{P}_{pCD}} [Deg]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary,
                                                        numTH2Dbins_Ang_Plots, 0,
                                                        180);
    string hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pFD_pCD (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_pCD (CD & FD)">
    THStack *sTheta_pFD_pCD_pFDpCD = new THStack("#theta_{pFD,pCD} (All Int., pFDpCD)",
                                                 "#theta_{pFD,pCD} - Opening Angle Between Protons (pFDpCD);#theta_{pFD,pCD} [Deg];");
    TH1D *hTheta_pFD_pCD_All_Int_pFDpCD = new TH1D("#theta_{pFD,pCD} (All Int., pFDpCD)",
                                                   "#theta_{pFD,pCD} - Opening Angle Between Protons (All Int., pFDpCD);#theta_{pFD,pCD} [Deg];",
                                                   numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_pFD_pCD_QEL_pFDpCD = new TH1D("#theta_{pFD,pCD} (QEL only, pFDpCD)",
                                               "#theta_{pFD,pCD} - Opening Angle Between Protons (QEL only, pFDpCD);#theta_{pFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_pFD_pCD_MEC_pFDpCD = new TH1D("#theta_{pFD,pCD} (MEC only, pFDpCD)",
                                               "#theta_{pFD,pCD} - Opening Angle Between Protons (MEC only, pFDpCD);#theta_{pFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_pFD_pCD_RES_pFDpCD = new TH1D("#theta_{pFD,pCD} (RES only, pFDpCD)",
                                               "#theta_{pFD,pCD} - Opening Angle Between Protons (RES only, pFDpCD);#theta_{pFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_pFD_pCD_DIS_pFDpCD = new TH1D("#theta_{pFD,pCD} (DIS only, pFDpCD)",
                                               "#theta_{pFD,pCD} - Opening Angle Between Protons (DIS only, pFDpCD);#theta_{pFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    string sTheta_pFD_pCD_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    string hTheta_pFD_pCD_All_Int_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_pFDpCD_Directory"];
    string hTheta_pFD_pCD_QEL_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_pFDpCD_Directory"];
    string hTheta_pFD_pCD_MEC_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_pFDpCD_Directory"];
    string hTheta_pFD_pCD_RES_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_pFDpCD_Directory"];
    string hTheta_pFD_pCD_DIS_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pFD_pCD vs. W (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_pCD vs. W (CD & FD)">
    TH2D *hTheta_pFD_pCD_vs_W_pFDpCD = new TH2D("#theta_{pFD,pCD} vs. W (All Int., pFDpCD)",
                                                "#theta_{pFD,pCD} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{pFD,pCD} [Deg];",
                                                numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_pFD_pCD_vs_W_pFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (CD & FD) ------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (CD & FD)">
    TH2D *hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD = new TH2D(
            "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD)",
            "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD);"
            "#theta_{pCD} [Deg];#theta_{pFD} [Deg];",
            numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50);
    string hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];
    //</editor-fold>

// dPhi_pFD_pCD for Theta_pFD_pCD < 20 (pFDpCD, CD & FD) ----------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_pFD_pCD for Theta_pFD_pCD < 20 (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD = new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD)",
                                                               "#Delta#phi for #theta_{pFD,pCD}<20#circ (All Int., pFDpCD);"
                                                               "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];

    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD = new TH1D("#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., pFDpCD)",
                                                                      "#Delta#phi for #theta_{pFD,pCD}<20#circ - ZOOMIN (All Int., pFDpCD);"
                                                                      "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (pFDpCD, CD & FD) -----------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (CD & FD)">
    TH2D *hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD = new TH2D(
            "#theta_{pFD} vs. #theta_{pFD} #forall#theta_{pFD,pCD} (All Int., pFDpCD)",
            "#theta_{pFD} vs. #theta_{pCD} for every #theta_{pFD,pCD} (All Int., pFDpCD);"
            "#theta_{pCD} [Deg];#theta_{pFD} [Deg];",
            numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50);
    string hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];
    //</editor-fold>

// dPhi_pFD_pCD for every Theta_pFD_pCD (pFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_pFD_pCD for every Theta_pFD_pCD (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD = new TH1D("#Delta#phi #forall#theta_{pFD,pCD} (All Int., pFDpCD)",
                                                                "#Delta#phi for every #theta_{pFD,pCD} (All Int., pFDpCD);"
                                                                "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];

    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD = new TH1D("#Delta#phi #forall#theta_{pFD,pCD} - ZOOMIN (All Int., pFDpCD)",
                                                                       "#Delta#phi for every #theta_{pFD,pCD} - ZOOMIN(All Int., pFDpCD);"
                                                                       "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_pFDpCD = new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} (All Int., pFDpCD)",
                                                           "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ|;"
                                                           "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdPhi_pFD_pCD_for_small_dTheta_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD = new TH1D("#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN (All Int., pFDpCD)",
                                                                  "#Delta#phi for small #Delta#theta_{pFD/pCD} = |#theta_{pFD/pCD}-40#circ| - ZOOMIN;"
                                                                  "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD_Dir = directories.Angle_Directory_map["Double_detection_pFDpCD_Directory"];
    //</editor-fold>

// Ghost tracks handling (CD only) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_pCD vs. TOFpFD-TOFpCD plots (pFDpCD)">
    hPlot2D hTheta_pFD_pCD_VS_ToFpFD_ToFpCD_AC_pFDpCD = hPlot2D("pFDpCD", "CD-CTOF", "#theta_{pFD,pCD} vs. ToF_{pFD}-ToF_{pCD} AC",
                                                                "#theta_{pFD,pCD} vs. ToF_{pFD}-ToF_{pCD} AC", "#theta_{pFD,pCD} [Deg]",
                                                                "ToF_{pFD}-ToF_{pCD} [ns]",
                                                                directories.Angle_Directory_map["CToF_hits_pFDpCD_Directory"],
                                                                "01_Theta_pFD_pCD_VS_ToFpFD_ToFpCD_AC_pFDpCD",
                                                                0, 180, -3, 3, numTH2Dbins_Ang_Plots, numTH2Dbins_Ang_Plots);
    //</editor-fold>

    //<editor-fold desc="Theta_pFD_pCD vs. positionpFD-positionpCD plots (pFDpCD)">
    hPlot2D hTheta_pFD_pCD_VS_PospFD_PospCD_AC_pFDpCD = hPlot2D("pFDpCD", "CD-CTOF", "#theta_{pFD,pCD} vs. Position_{pFD}-Position_{pCD} AC",
                                                                "#theta_{pFD,pCD} vs. Position_{pFD}-Position_{pCD} AC", "#theta_{pFD,pCD} [Deg]",
                                                                "Position_{pFD}-Position_{pCD} [cm]",
                                                                directories.Angle_Directory_map["CToF_hits_pFDpCD_Directory"],
                                                                "02_Theta_p1_p2_VS_Pos1-Pos2_AC_pFDpCD",
                                                                0, 180, 0, 100, numTH2Dbins_Ang_Plots, numTH2Dbins_Ang_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (nFDpCD)">

    //<editor-fold desc="Nucleons' angles plots (nFDpCD)">

// Theta_nFD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD histograms">
    THStack *sTheta_nFD_nFDpCD_FD = new THStack("#theta_{nFD} (nFDpCD)", "#theta_{nFD} of FD neutron (nFDpCD);#theta_{nFD} [Deg];");

    TH1D *hTheta_nFD_All_Int_nFDpCD_FD = new TH1D("#theta_{nFD} (All Int., nFDpCD)",
                                                  "#theta_{nFD} of FD neutron (All Int., nFDpCD);#theta_{nFD} [Deg];",
                                                  numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_nFD_QEL_nFDpCD_FD = new TH1D("#theta_{nFD} (QEL Only, nFDpCD)",
                                              "#theta_{nFD} of FD neutron (QEL Only, nFDpCD);#theta_{nFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_nFD_MEC_nFDpCD_FD = new TH1D("#theta_{nFD} (MEC Only, nFDpCD)",
                                              "#theta_{nFD} of FD neutron (MEC Only, nFDpCD);#theta_{nFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_nFD_RES_nFDpCD_FD = new TH1D("#theta_{nFD} (RES Only, nFDpCD)",
                                              "#theta_{nFD} of FD neutron (RES Only, nFDpCD);#theta_{nFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    TH1D *hTheta_nFD_DIS_nFDpCD_FD = new TH1D("#theta_{nFD} (DIS Only, nFDpCD)",
                                              "#theta_{nFD} of FD neutron (DIS Only, nFDpCD);#theta_{nFD} [Deg];",
                                              numTH1Dbins_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_nFD_All_Int_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];
    string hTheta_nFD_QEL_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];
    string hTheta_nFD_MEC_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];
    string hTheta_nFD_RES_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];
    string hTheta_nFD_DIS_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];

    TH2D *hTheta_nFD_VS_P_nFD_nFDpCD_FD = new TH2D("#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)",
                                                   "#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD);P_{nFD} [GeV/c];#theta_{nFD} [Deg]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                                   Theta_uboundary_FD);
    TH2D *hTheta_nFD_VS_W_nFDpCD_FD = new TH2D("#theta_{nFD} vs. W (All Int., nFDpCD, FD)",
                                               "#theta_{nFD} vs. W (All Int., nFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{nFD} [Deg]",
                                               numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Theta_lboundary_FD,
                                               Theta_uboundary_FD);
    string hTheta_nFD_VS_P_nFD_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];
    string hTheta_nFD_VS_W_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_nFDpCD_Directory"];
    //</editor-fold>

// Phi_nFD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_nFD histograms">
    THStack *sPhi_nFD_nFDpCD_FD = new THStack("#phi_{nFD} (nFDpCD)", "#phi_{nFD} of FD neutron (nFDpCD);#phi_{nFD} [Deg];");

    TH1D *hPhi_nFD_All_Int_nFDpCD_FD = new TH1D("#phi_{nFD} (All Int., nFDpCD)", "#phi_{nFD} of FD neutron (All Int., nFDpCD);#phi_{nFD} [Deg];",
                                                numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_nFD_QEL_nFDpCD_FD = new TH1D("#phi_{nFD} (QEL Only, nFDpCD)", "#phi_{nFD} of FD neutron (QEL Only, nFDpCD);#phi_{nFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_nFD_MEC_nFDpCD_FD = new TH1D("#phi_{nFD} (MEC Only, nFDpCD)", "#phi_{nFD} of FD neutron (MEC Only, nFDpCD);#phi_{nFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_nFD_RES_nFDpCD_FD = new TH1D("#phi_{nFD} (RES Only, nFDpCD)", "#phi_{nFD} of FD neutron (RES Only, nFDpCD);#phi_{nFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_nFD_DIS_nFDpCD_FD = new TH1D("#phi_{nFD} (DIS Only, nFDpCD)", "#phi_{nFD} of FD neutron (DIS Only, nFDpCD);#phi_{nFD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_nFD_All_Int_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];
    string hPhi_nFD_QEL_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];
    string hPhi_nFD_MEC_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];
    string hPhi_nFD_RES_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];
    string hPhi_nFD_DIS_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];

    TH2D *hPhi_nFD_VS_P_nFD_nFDpCD_FD = new TH2D("#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)",
                                                 "#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD);P_{nFD} [GeV/c];#phi_{nFD} [Deg]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_nFD_VS_W_nFDpCD_FD = new TH2D("#phi_{nFD} vs. W (All Int., nFDpCD, FD)",
                                             "#phi_{nFD} vs. W (All Int., nFDpCD, FD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{nFD} [Deg]",
                                             numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_nFD_VS_P_nFD_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];
    string hPhi_nFD_VS_W_nFDpCD_FD_Dir = directories.Angle_Directory_map["Phi_nFD_nFDpCD_Directory"];
    //</editor-fold>

// Theta_nFD vs. Phi_nFD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD vs. Phi_nFD histograms">
    TH2D *hTheta_nFD_VS_Phi_nFD_nFDpCD_FD = new TH2D("#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., nFDpCD)",
                                                     "#theta_{nFD} vs. #phi_{nFD} of FD neutron (All Int., nFDpCD);#phi_{nFD} [Deg];#theta_{nFD} [Deg]",
                                                     numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                     numTH2Dbins_Nucleon_Ang_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_nFD_VS_Phi_nFD_nFDpCD_FD_Dir = directories.Angle_Directory_map["Theta_nFD_VS_Phi_nFD_nFDpCD_Directory"];
    //</editor-fold>

// Theta_pCD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD histograms">
    THStack *sTheta_pCD_nFDpCD_CD = new THStack("#theta_{pCD} (nFDpCD)", "#theta_{pCD} of CD proton (nFDpCD);#theta_{pCD} [Deg];");

    TH1D *hTheta_pCD_All_Int_nFDpCD_CD = new TH1D("#theta_{pCD} (All Int., nFDpCD)",
                                                  "#theta_{pCD} of CD proton (All Int., nFDpCD);#theta_{pCD} [Deg];",
                                                  numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_QEL_nFDpCD_CD = new TH1D("#theta_{pCD} (QEL Only, nFDpCD)",
                                              "#theta_{pCD} of CD proton (QEL Only, nFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_MEC_nFDpCD_CD = new TH1D("#theta_{pCD} (MEC Only, nFDpCD)",
                                              "#theta_{pCD} of CD proton (MEC Only, nFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_RES_nFDpCD_CD = new TH1D("#theta_{pCD} (RES Only, nFDpCD)",
                                              "#theta_{pCD} of CD proton (RES Only, nFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_pCD_DIS_nFDpCD_CD = new TH1D("#theta_{pCD} (DIS Only, nFDpCD)",
                                              "#theta_{pCD} of CD proton (DIS Only, nFDpCD);#theta_{pCD} [Deg];",
                                              numTH1Dbins_Ang_Plots, 30, 155);
    string hTheta_pCD_All_Int_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];
    string hTheta_pCD_QEL_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];
    string hTheta_pCD_MEC_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];
    string hTheta_pCD_RES_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];
    string hTheta_pCD_DIS_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];

    TH2D *hTheta_pCD_VS_P_pCD_nFDpCD_CD = new TH2D("#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)",
                                                   "#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD);P_{pCD} [GeV/c];#theta_{pCD} [Deg]",
                                                   numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, 30, 155);
    TH2D *hTheta_pCD_VS_W_nFDpCD_CD = new TH2D("#theta_{pCD} vs. W (All Int., nFDpCD, CD)",
                                               "#theta_{pCD} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{pCD} [Deg]",
                                               numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 30, 155);
    string hTheta_pCD_VS_P_pCD_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];
    string hTheta_pCD_VS_W_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_nFDpCD_Directory"];
//</editor-fold>

// Phi_pCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_pCD histograms">
    THStack *sPhi_pCD_nFDpCD_CD = new THStack("#phi_{pCD} (nFDpCD)", "#phi_{pCD} of CD proton (nFDpCD);#phi_{pCD} [Deg];");

    TH1D *hPhi_pCD_All_Int_nFDpCD_CD = new TH1D("#phi_{pCD} (All Int., nFDpCD)", "#phi_{pCD} of CD proton (All Int., nFDpCD);#phi_{pCD} [Deg];",
                                                numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_QEL_nFDpCD_CD = new TH1D("#phi_{pCD} (QEL Only, nFDpCD)", "#phi_{pCD} of CD proton (QEL Only, nFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_MEC_nFDpCD_CD = new TH1D("#phi_{pCD} (MEC Only, nFDpCD)", "#phi_{pCD} of CD proton (MEC Only, nFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_RES_nFDpCD_CD = new TH1D("#phi_{pCD} (RES Only, nFDpCD)", "#phi_{pCD} of CD proton (RES Only, nFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_pCD_DIS_nFDpCD_CD = new TH1D("#phi_{pCD} (DIS Only, nFDpCD)", "#phi_{pCD} of CD proton (DIS Only, nFDpCD);#phi_{pCD} [Deg];",
                                            numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_pCD_All_Int_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];
    string hPhi_pCD_QEL_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];
    string hPhi_pCD_MEC_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];
    string hPhi_pCD_RES_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];
    string hPhi_pCD_DIS_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];

    TH2D *hPhi_pCD_VS_P_pCD_nFDpCD_CD = new TH2D("#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)",
                                                 "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD);P_{pCD} [GeV/c];#phi_{pCD} [Deg]",
                                                 numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_pCD_VS_W_nFDpCD_CD = new TH2D("#phi_{pCD} vs. W (All Int., nFDpCD, CD)",
                                             "#phi_{pCD} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{pCD} [Deg]",
                                             numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_pCD_VS_P_pCD_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];
    string hPhi_pCD_VS_W_nFDpCD_CD_Dir = directories.Angle_Directory_map["Phi_pCD_nFDpCD_Directory"];
    //</editor-fold>

// Theta_pCD vs. Phi_pCD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD vs. Phi_pCD histograms">
    TH2D *hTheta_pCD_VS_Phi_pCD_nFDpCD_CD = new TH2D("#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., nFDpCD)",
                                                     "#theta_{pCD} vs. #phi_{pCD} of CD proton (All Int., nFDpCD);#phi_{pCD} [Deg];#theta_{pCD} [Deg]",
                                                     numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_Nucleon_Ang_Plots,
                                                     30, 155);
    string hTheta_pCD_VS_Phi_pCD_nFDpCD_CD_Dir = directories.Angle_Directory_map["Theta_pCD_VS_Phi_pCD_nFDpCD_Directory"];
    //</editor-fold>

// Theta_tot ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot histograms">
    THStack *sTheta_tot_nFDpCD = new THStack("#theta_{tot} (nFDpCD)", "#theta_{tot} of total 3-momentum (nFDpCD);#theta_{tot} [Deg];");

    TH1D *hTheta_tot_All_Int_nFDpCD = new TH1D("#theta_{tot} (All Int., nFDpCD)",
                                               "#theta_{tot} of total 3-momentum (All Int., nFDpCD);#theta_{tot} [Deg];",
                                               numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_QEL_nFDpCD = new TH1D("#theta_{tot} (QEL Only, nFDpCD)",
                                           "#theta_{tot} of total 3-momentum (QEL Only, nFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_MEC_nFDpCD = new TH1D("#theta_{tot} (MEC Only, nFDpCD)",
                                           "#theta_{tot} of total 3-momentum (MEC Only, nFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_RES_nFDpCD = new TH1D("#theta_{tot} (RES Only, nFDpCD)",
                                           "#theta_{tot} of total 3-momentum (RES Only, nFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_tot_DIS_nFDpCD = new TH1D("#theta_{tot} (DIS Only, nFDpCD)",
                                           "#theta_{tot} of total 3-momentum (DIS Only, nFDpCD);#theta_{tot} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_tot_All_Int_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];
    string hTheta_tot_QEL_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];
    string hTheta_tot_MEC_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];
    string hTheta_tot_RES_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];
    string hTheta_tot_DIS_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];

    TH2D *hTheta_tot_VS_P_tot_nFDpCD = new TH2D("#theta_{tot} vs. P_{tot} (All Int., nFDpCD, CD)",
                                                "#theta_{tot} vs. P_{tot} (All Int., nFDpCD, CD);P_{tot} [GeV/c];#theta_{tot} [Deg]",
                                                numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                                Opening_Ang_narrow_uboundary);
    TH2D *hTheta_tot_VS_W_nFDpCD = new TH2D("#theta_{tot} vs. W (All Int., nFDpCD, CD)",
                                            "#theta_{tot} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{tot} [Deg]",
                                            numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                            Opening_Ang_narrow_uboundary);
    string hTheta_tot_VS_P_tot_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];
    string hTheta_tot_VS_W_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_nFDpCD_Directory"];
    //</editor-fold>

// Phi_tot --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_tot histograms">
    THStack *sPhi_tot_nFDpCD = new THStack("#phi_{tot} (nFDpCD)", "#phi_{tot} of total 3-momentum (nFDpCD);#phi_{tot} [Deg];");

    TH1D *hPhi_tot_All_Int_nFDpCD = new TH1D("#phi_{tot} (All Int., nFDpCD)",
                                             "#phi_{tot} of total 3-momentum (All Int., nFDpCD);#phi_{tot} [Deg];",
                                             numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_QEL_nFDpCD = new TH1D("#phi_{tot} (QEL Only, nFDpCD)", "#phi_{tot} of total 3-momentum (QEL Only, nFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_MEC_nFDpCD = new TH1D("#phi_{tot} (MEC Only, nFDpCD)", "#phi_{tot} of total 3-momentum (MEC Only, nFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_RES_nFDpCD = new TH1D("#phi_{tot} (RES Only, nFDpCD)", "#phi_{tot} of total 3-momentum (RES Only, nFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_tot_DIS_nFDpCD = new TH1D("#phi_{tot} (DIS Only, nFDpCD)", "#phi_{tot} of total 3-momentum (DIS Only, nFDpCD);#phi_{tot} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_tot_All_Int_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];
    string hPhi_tot_QEL_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];
    string hPhi_tot_MEC_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];
    string hPhi_tot_RES_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];
    string hPhi_tot_DIS_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];

    TH2D *hPhi_tot_VS_P_tot_nFDpCD = new TH2D("#phi_{tot} vs. P_{tot} (All Int., nFDpCD, CD)",
                                              "#phi_{tot} vs. P_{tot} (All Int., nFDpCD, CD);P_{tot} [GeV/c];#phi_{tot} [Deg]",
                                              numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_tot_VS_W_nFDpCD = new TH2D("#phi_{tot} vs. W (All Int., nFDpCD, CD)",
                                          "#phi_{tot} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{tot} [Deg]",
                                          numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_tot_VS_P_tot_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];
    string hPhi_tot_VS_W_nFDpCD_Dir = directories.Angle_Directory_map["Phi_tot_nFDpCD_Directory"];
    //</editor-fold>

// Theta_tot vs. Phi_tot ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot vs. Phi_tot histograms">
    TH2D *hTheta_tot_VS_Phi_tot_nFDpCD = new TH2D("#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., nFDpCD)",
                                                  "#theta_{tot} vs. #phi_{tot} of total 3-momentum (All Int., nFDpCD);#phi_{tot} [Deg];#theta_{tot} [Deg]",
                                                  numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary,
                                                  numTH2Dbins_Nucleon_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_tot_VS_Phi_tot_nFDpCD_Dir = directories.Angle_Directory_map["Theta_tot_VS_Phi_tot_nFDpCD_Directory"];
    //</editor-fold>

// Theta_rel ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel histograms">
    THStack *sTheta_rel_nFDpCD = new THStack("#theta_{rel} (nFDpCD)", "#theta_{rel} of relative 3-momentum (nFDpCD);#theta_{rel} [Deg];");

    TH1D *hTheta_rel_All_Int_nFDpCD = new TH1D("#theta_{rel} (All Int., nFDpCD)",
                                               "#theta_{rel} of relative 3-momentum (All Int., nFDpCD);#theta_{rel} [Deg];",
                                               numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_QEL_nFDpCD = new TH1D("#theta_{rel} (QEL Only, nFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (QEL Only, nFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_MEC_nFDpCD = new TH1D("#theta_{rel} (MEC Only, nFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (MEC Only, nFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_RES_nFDpCD = new TH1D("#theta_{rel} (RES Only, nFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (RES Only, nFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    TH1D *hTheta_rel_DIS_nFDpCD = new TH1D("#theta_{rel} (DIS Only, nFDpCD)",
                                           "#theta_{rel} of relative 3-momentum (DIS Only, nFDpCD);#theta_{rel} [Deg];",
                                           numTH1Dbins_Ang_Plots, 30, 155);
    string hTheta_rel_All_Int_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];
    string hTheta_rel_QEL_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];
    string hTheta_rel_MEC_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];
    string hTheta_rel_RES_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];
    string hTheta_rel_DIS_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];

    TH2D *hTheta_rel_VS_P_rel_nFDpCD = new TH2D("#theta_{rel} vs. P_{rel} (All Int., nFDpCD, CD)",
                                                "#theta_{rel} vs. P_{rel} (All Int., nFDpCD, CD);P_{rel} [GeV/c];#theta_{rel} [Deg]",
                                                numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, 30, 155);
    TH2D *hTheta_rel_VS_W_nFDpCD = new TH2D("#theta_{rel} vs. W (All Int., nFDpCD, CD)",
                                            "#theta_{rel} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{rel} [Deg]",
                                            numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 30, 155);
    string hTheta_rel_VS_P_rel_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];
    string hTheta_rel_VS_W_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_nFDpCD_Directory"];
    //</editor-fold>

// Phi_rel --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_rel histograms">
    THStack *sPhi_rel_nFDpCD = new THStack("#phi_{rel} (nFDpCD)", "#phi_{rel} of relative 3-momentum (nFDpCD);#phi_{rel} [Deg];");

    TH1D *hPhi_rel_All_Int_nFDpCD = new TH1D("#phi_{rel} (All Int., nFDpCD)",
                                             "#phi_{rel} of relative 3-momentum (All Int., nFDpCD);#phi_{rel} [Deg];",
                                             numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_QEL_nFDpCD = new TH1D("#phi_{rel} (QEL Only, nFDpCD)", "#phi_{rel} of relative 3-momentum (QEL Only, nFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_MEC_nFDpCD = new TH1D("#phi_{rel} (MEC Only, nFDpCD)", "#phi_{rel} of relative 3-momentum (MEC Only, nFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_RES_nFDpCD = new TH1D("#phi_{rel} (RES Only, nFDpCD)", "#phi_{rel} of relative 3-momentum (RES Only, nFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH1D *hPhi_rel_DIS_nFDpCD = new TH1D("#phi_{rel} (DIS Only, nFDpCD)", "#phi_{rel} of relative 3-momentum (DIS Only, nFDpCD);#phi_{rel} [Deg];",
                                         numTH1Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_rel_All_Int_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];
    string hPhi_rel_QEL_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];
    string hPhi_rel_MEC_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];
    string hPhi_rel_RES_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];
    string hPhi_rel_DIS_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];

    TH2D *hPhi_rel_VS_P_rel_nFDpCD = new TH2D("#phi_{rel} vs. P_{rel} (All Int., nFDpCD, CD)",
                                              "#phi_{rel} vs. P_{rel} (All Int., nFDpCD, CD);P_{rel} [GeV/c];#phi_{rel} [Deg]",
                                              numTH2Dbins_Ang_Plots, 0, beamE * 1.1, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    TH2D *hPhi_rel_VS_W_nFDpCD = new TH2D("#phi_{rel} vs. W (All Int., nFDpCD, CD)",
                                          "#phi_{rel} vs. W (All Int., nFDpCD, CD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#phi_{rel} [Deg]",
                                          numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Phi_lboundary, Phi_uboundary);
    string hPhi_rel_VS_P_rel_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];
    string hPhi_rel_VS_W_nFDpCD_Dir = directories.Angle_Directory_map["Phi_rel_nFDpCD_Directory"];
    //</editor-fold>

// Theta_rel vs. Phi_rel ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel vs. Phi_rel histograms">
    TH2D *hTheta_rel_VS_Phi_rel_nFDpCD = new TH2D("#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., nFDpCD)",
                                                  "#theta_{rel} vs. #phi_{rel} of relative 3-momentum (All Int., nFDpCD);#phi_{rel} [Deg];#theta_{rel} [Deg]",
                                                  numTH2Dbins_Nucleon_Ang_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_Nucleon_Ang_Plots, 30,
                                                  155);
    string hTheta_rel_VS_Phi_rel_nFDpCD_Dir = directories.Angle_Directory_map["Theta_rel_VS_Phi_rel_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_p_e_p_tot (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (nFDpCD, CD & FD)">
    THStack *sTheta_p_e_p_tot_nFDpCD = new THStack("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., nFDpCD)",
                                                   "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{nFD}+#vec{P}_{pCD} "
                                                   "(All Int., nFDpCD);#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_p_e_p_tot_nFDpCD = new TH1D("#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., nFDpCD)",
                                             "#theta_{#vec{P}_{e},#vec{P}_{tot}} - Opening Angle Between #vec{P}_{e} and #vec{P}_{tot}=#vec{P}_{nFD}+#vec{P}_{pCD} "
                                             "(All Int., nFDpCD);#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];",
                                             numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_p_e_p_tot_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

// Theta_p_e_p_tot vs. W (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot vs. W (nFDpCD)">
    TH2D *hTheta_p_e_p_tot_vs_W_nFDpCD = new TH2D("#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., nFDpCD)",
                                                  "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                                  "#theta_{#vec{P}_{e},#vec{P}_{tot}} [Deg];",
                                                  numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                                  Opening_Ang_narrow_lboundary,
                                                  Opening_Ang_narrow_uboundary);
    string hTheta_p_e_p_tot_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

// Theta_q_p (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (nFDpCD, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_nFDpCD = new THStack("#theta_{#vec{q},#vec{P}_{tot}} (All Int., nFDpCD)",
                                                 "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{nFD}+#vec{P}_{pCD} "
                                                 "(All Int., nFDpCD);#theta_{#vec{q},#vec{P}_{tot}} [Deg];");
    TH1D *hTheta_q_p_tot_nFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{tot}} (All Int., nFDpCD)",
                                           "#theta_{#vec{q},#vec{P}_{tot}} - Opening Angle Between #vec{q} and #vec{P}_{tot}=#vec{P}_{nFD}+#vec{P}_{pCD} "
                                           "(All Int., nFDpCD);#theta_{#vec{q},#vec{P}_{tot}} [Deg];",
                                           numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    string hTheta_q_p_tot_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_P_nL_minus_q_nR and Theta_q_p_R (nFDpCD, CD & FD)">
    THStack *sTheta_P_nL_minus_q_nR_nFDpCD = new THStack("#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)",
                                                         "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} - Opening Angle Between #vec{P}_L-#vec{q} and #vec{P}_R (All Int., nFDpCD);"
                                                         "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} [Deg];");
    TH1D *hTheta_P_nL_minus_q_nR_nFDpCD = new TH1D("#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)",
                                                   "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} - Opening Angle Between #vec{q} and #vec{P}_{nL} (All Int., nFDpCD)"
                                                   ";#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} [Deg]",
                                                   numTH1Dbins_Ang_Plots, Opening_Ang_wide_lboundary, Opening_Ang_wide_uboundary);
    string hTheta_P_nL_minus_q_nR_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (nFDpCD, CD & FD)">
    THStack *sTheta_q_p_L_R_nFDpCD = new THStack("#theta_{#vec{q},#vec{P}} (All Int., nFDpCD)",
                                                 "#theta_{#vec{q},#vec{P}} - Opening Angle Between #vec{q} and #vec{P} (All Int., nFDpCD);#theta_{#vec{q},#vec{P}} [Deg];");
    TH1D *hTheta_q_p_L_nFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{nL}} (All Int., nFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{nL}} - Opening Angle Between #vec{q} and Leading Nucleon #vec{P}_{nL} (All Int., nFDpCD);"
                                         "#theta_{#vec{q},#vec{P}_{nL}} [Deg]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_q_p_R_nFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{nR}} - Opening Angle Between #vec{q} and Recoil Nucleon #vec{P}_{nR} (All Int., nFDpCD);"
                                         "#theta_{#vec{q},#vec{P}_{nR}} [Deg]",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_L_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    string hTheta_q_p_R_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_nFD and Theta_q_pCD (nFDpCD, CD & FD)">
    THStack *sTheta_q_p_nFDpCD = new THStack("#theta_{#vec{q},#vec{P}} (All Int., nFDpCD)",
                                             "#theta_{#vec{q},#vec{P}} - Opening Angle Between #vec{q} and #vec{P} (All Int., nFDpCD);#theta_{#vec{q},#vec{P}} [Deg];");
    TH1D *hTheta_q_nFD_nFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{nFD}} (All Int., nFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{nFD}} - Opening Angle Between #vec{q} and FD Neutron #vec{P}_{nFD} (All Int., nFDpCD);"
                                         "#theta_{#vec{q},#vec{P}_{nFD}} [Deg]",
                                         numTH1Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary);
    TH1D *hTheta_q_pCD_nFDpCD = new TH1D("#theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD)",
                                         "#theta_{#vec{q},#vec{P}_{pCD}} - Opening Angle Between #vec{q} and CD Proton #vec{P}_{pCD} (All Int., nFDpCD);"
                                         "#theta_{#vec{q},#vec{P}_{pCD}} [Deg]",
                                         numTH1Dbins_Ang_Plots, 0, 180);
    string hTheta_q_nFD_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    string hTheta_q_pCD_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_tot vs. W (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_pCD vs. W (CD & FD)">
    TH2D *hTheta_q_p_tot_vs_W_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., nFDpCD)",
                                                "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                                "#theta_{#vec{q},#vec{P}_{tot}} [Deg];",
                                                numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                                Opening_Ang_narrow_lboundary,
                                                Opening_Ang_narrow_uboundary);
    string hTheta_q_p_tot_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_P_nL_minus_q_nR vs. W (CD & FD)">
    TH2D *hTheta_P_nL_minus_q_nR_vs_W_nFDpCD = new TH2D("#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD)",
                                                        "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                                        "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} [Deg];",
                                                        numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots,
                                                        Opening_Ang_wide_lboundary,
                                                        Opening_Ang_wide_uboundary);
    string hTheta_P_nL_minus_q_nR_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L vs. W (CD & FD)">
    TH2D *hTheta_q_p_L_vs_W_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{nL}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{nL}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{nL}} [Deg];", numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary,
                                              numTH2Dbins_Ang_Plots,
                                              Opening_Ang_narrow_lboundary,
                                              Opening_Ang_narrow_uboundary);
    string hTheta_q_p_L_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_R vs. W (CD & FD)">
    TH2D *hTheta_q_p_R_vs_W_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{nR}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_p_R_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_nFD vs. W (CD & FD)">
    TH2D *hTheta_q_nFD_vs_W_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{nFD}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{nFD}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{nFD}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                              Opening_Ang_narrow_uboundary);
    string hTheta_q_nFD_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_pCD vs. W (CD & FD)">
    TH2D *hTheta_q_pCD_vs_W_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., nFDpCD)",
                                              "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                              "#theta_{#vec{q},#vec{P}_{pCD}} [Deg];",
                                              numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_q_pCD_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (nFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p vs |P_p|/|q| (CD & FD)">
    TH2D *hTheta_q_p_L_vs_p_L_q_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL} (All Int., nFDpCD)",
                                                  "#theta_{#vec{q},#vec{P}_{nL}} vs. r_{nL}=|#vec{P}_{nL}|/|#vec{q}| (All Int., nFDpCD);"
                                                  "r_{nFD};#theta_{#vec{q},#vec{P}_{nFD}} [Deg]",
                                                  numTH2Dbins_Ang_Plots, 0, 1.05, numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary,
                                                  Opening_Ang_narrow_uboundary);
    string hTheta_q_p_L_vs_p_L_q_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

// Theta_q_p vs Theta_q_p (nFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_L vs Theta_q_p_R">
    TH2D *hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)",
                                                        "#theta_{#vec{q},#vec{P}_{nL}} vs. #theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD);"
                                                        "#theta_{#vec{q},#vec{P}_{nL}};#theta_{#vec{q},#vec{P}_{nR}} [Deg]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary,
                                                        numTH2Dbins_Ang_Plots, 0,
                                                        180);
    string hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Theta_q_nFD vs Theta_q_pCD">
    TH2D *hTheta_q_nFD_vs_Theta_q_pCD_nFDpCD = new TH2D("#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD)",
                                                        "#theta_{#vec{q},#vec{P}_{nFD}} vs. #theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD);"
                                                        "#theta_{#vec{q},#vec{P}_{nFD}} [Deg];#theta_{#vec{q},#vec{P}_{pCD}} [Deg]",
                                                        numTH2Dbins_Ang_Plots, Opening_Ang_narrow_lboundary, Opening_Ang_narrow_uboundary,
                                                        numTH2Dbins_Ang_Plots, 0,
                                                        180);
    string hTheta_q_nFD_vs_Theta_q_pCD_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

// Theta_nFD_pCD (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_pCD (CD & FD)">
    THStack *sTheta_nFD_pCD_nFDpCD = new THStack("#theta_{nFD,pCD} (All Int., nFDpCD)",
                                                 "#theta_{nFD,pCD} - Opening Angle Between Protons (nFDpCD);#theta_{nFD,pCD} [Deg];");
    TH1D *hTheta_nFD_pCD_All_Int_nFDpCD = new TH1D("#theta_{nFD,pCD} (All Int., nFDpCD)",
                                                   "#theta_{nFD,pCD} - Opening Angle Between Protons (All Int., nFDpCD);#theta_{nFD,pCD} [Deg];",
                                                   numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_nFD_pCD_QEL_nFDpCD = new TH1D("#theta_{nFD,pCD} (QEL only, nFDpCD)",
                                               "#theta_{nFD,pCD} - Opening Angle Between Protons (QEL only, nFDpCD);#theta_{nFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_nFD_pCD_MEC_nFDpCD = new TH1D("#theta_{nFD,pCD} (MEC only, nFDpCD)",
                                               "#theta_{nFD,pCD} - Opening Angle Between Protons (MEC only, nFDpCD);#theta_{nFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_nFD_pCD_RES_nFDpCD = new TH1D("#theta_{nFD,pCD} (RES only, nFDpCD)",
                                               "#theta_{nFD,pCD} - Opening Angle Between Protons (RES only, nFDpCD);#theta_{nFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    TH1D *hTheta_nFD_pCD_DIS_nFDpCD = new TH1D("#theta_{nFD,pCD} (DIS only, nFDpCD)",
                                               "#theta_{nFD,pCD} - Opening Angle Between Protons (DIS only, nFDpCD);#theta_{nFD,pCD} [Deg];",
                                               numTH1Dbins_Ang_Plots, 0, 180);
    string sTheta_nFD_pCD_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"];
    string hTheta_nFD_pCD_All_Int_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"];
    string hTheta_nFD_pCD_QEL_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"];
    string hTheta_nFD_pCD_MEC_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"];
    string hTheta_nFD_pCD_RES_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"];
    string hTheta_nFD_pCD_DIS_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_by_interaction_nFDpCD_Directory"];
    //</editor-fold>

// Theta_nFD_pCD vs. W (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_pCD vs. W (CD & FD)">
    TH2D *hTheta_nFD_pCD_vs_W_nFDpCD = new TH2D("#theta_{nFD,pCD} vs. W (All Int., nFDpCD)",
                                                "#theta_{nFD,pCD} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#theta_{nFD,pCD} [Deg];",
                                                numTH2Dbins_Ang_Plots, W_lboundary, W_uboundary, numTH2Dbins_Ang_Plots, 0, 180);
    string hTheta_nFD_pCD_vs_W_nFDpCD_Dir = directories.Angle_Directory_map["Opening_angles_nFDpCD_Directory"];
    //</editor-fold>

// Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (CD & FD)">
    TH2D *hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD = new TH2D(
            "#theta_{nFD} vs. #theta_{pCD} for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD)",
            "#theta_{nFD} vs. #theta_{pCD} for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD);"
            "#theta_{pCD} [Deg];#theta_{nFD} [Deg];",
            numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50);
    string hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];
    //</editor-fold>

// dphi_nFD_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dphi_nFD_pCD for Theta_nFD_pCD < 20 (CD & FD)">
    TH1D *hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD = new TH1D("#Delta#phi for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD)",
                                                               "#Delta#phi for #theta_{nFD,pCD}<20#circ (All Int., nFDpCD);"
                                                               "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];

    TH1D *hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD = new TH1D("#Delta#phi for #theta_{nFD,pCD}<20#circ - ZOOMIN (All Int., nFDpCD)",
                                                                      "#Delta#phi for #theta_{nFD,pCD}<20#circ - ZOOMIN (All Int., nFDpCD);"
                                                                      "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];
    //</editor-fold>

// Theta_nFD_vs_theta_pCD for every Theta_nFD_pCD (nFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for Theta_nFD_pCD (CD & FD)">
    TH2D *hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD = new TH2D(
            "#theta_{nFD} vs. #theta_{nFD} #forall#theta_{nFD,pCD} (All Int., nFDpCD)",
            "#theta_{nFD} vs. #theta_{pCD} for every #theta_{nFD,pCD} (All Int., nFDpCD);"
            "#theta_{pCD} [Deg];#theta_{nFD} [Deg];",
            numTH2Dbins_Ang_Plots, 30, 50, numTH2Dbins_Ang_Plots, 30, 50);
    string hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];
    //</editor-fold>

// dphi_nFD_pCD for every Theta_nFD_pCD (nFDpCD, CD & FD) -----------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for every Theta_nFD_pCD (CD & FD)">
    TH1D *hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD = new TH1D("#Delta#phi #forall#theta_{nFD,pCD} (All Int., nFDpCD)",
                                                                "#Delta#phi for every #theta_{nFD,pCD} (All Int., nFDpCD);"
                                                                "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];

    TH1D *hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD = new TH1D("#Delta#phi #forall#theta_{nFD,pCD} - ZOOMIN (All Int., nFDpCD)",
                                                                       "#Delta#phi for every #theta_{nFD,pCD} - ZOOMIN(All Int., nFDpCD);"
                                                                       "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -40, 40);
    string hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];

    TH1D *hdphi_nFD_pCD_for_small_dTheta_nFDpCD = new TH1D("#Delta#phi for small #Delta#theta_{nFD/pCD} (All Int., nFDpCD)",
                                                           "#Delta#phi for small #Delta#theta_{nFD/pCD} = #theta_{nFD/pCD}-40#circ;"
                                                           "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, Phi_lboundary, Phi_uboundary);
    string hdphi_nFD_pCD_for_small_dTheta_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];

    TH1D *hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD = new TH1D("#Delta#phi for small #Delta#theta_{nFD/pCD} - ZOOMIN (All Int., nFDpCD)",
                                                                  "#Delta#phi for small #Delta#theta_{nFD/pCD} = #theta_{nFD/pCD}-40#circ - ZOOMIN;"
                                                                  "#Delta#phi = #phi_{pFD} - #phi_{pCD} [Deg];", 50, -50, 05);
    string hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD_Dir = directories.Angle_Directory_map["Double_detection_nFDpCD_Directory"];
    //</editor-fold>

// Neutron veto plots (nFDpCD) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron veto plots (nFDpCD)">
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_nFDpCD = hPlot2D("nFDpCD", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV",
                                                                      "'Neutron Hits' vs. Electron Hits - Before Veto",
                                                                      "#Delta#phi_{nFD,e} = #phi_{nFD}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                      "#Delta#theta_{nFD,e} = #theta_{nFD}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                      directories.Angle_Directory_map["Neutron_veto_nFDpCD_Directory"],
                                                                      "01_Neutron_hits_vs_electron_hits_BV_nFDpCD", -180, 180, -50, 50, 65, 65);

    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_nFDpCD = hPlot2D("nFDpCD", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV",
                                                                      "'Neutron Hits' vs. Electron Hits - After Veto",
                                                                      "#Delta#phi_{nFD,e} = #phi_{nFD}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                      "#Delta#theta_{nFD,e} = #theta_{nFD}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                      directories.Angle_Directory_map["Neutron_veto_nFDpCD_Directory"],
                                                                      "02_Neutron_hits_vs_electron_hits_AV_nFDpCD", -180, 180, -50, 50, 65, 65);

    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_Neutrons_nFDpCD = hPlot2D("nFDpCD", "FD", "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e}",
                                                                                   "'Neutron Hits' vs. Electron Hits - Vetoed Neutrons",
                                                                                   "#Delta#phi_{nFD,e} = #phi_{nFD}^{ECAL} - #phi_{e}^{ECAL} [Deg]",
                                                                                   "#Delta#theta_{nFD,e} = #theta_{nFD}^{ECAL} - #theta_{e}^{ECAL} [Deg]",
                                                                                   directories.Angle_Directory_map["Neutron_veto_nFDpCD_Directory"],
                                                                                   "03_Neutron_hits_vs_electron_hits_Vetoed_nFDpCD", -180, 180,
                                                                                   -50, 50, 65, 65);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Q2 histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Q2 histograms">

    //<editor-fold desc="Q2 histograms (no #(e) cut)">
    THStack *sQ2_All_e = new THStack("Q^{2} (no #(e) cut, CD & FD)", "Q^{2} Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_All_e = new TH1D("Q^{2} (no #(e) cut, FD)", "Q^{2} (no #(e) cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_All_e_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_All_e_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1e cut)">
    THStack *sQ2_1e_cut = new THStack("Q^{2} (1e Cut ,CD & FD)", "Q^{2} Histogram (1e Cut, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_1e_cut = new TH1D("Q^{2} (1e Cut, FD)", "Q^{2} (1e Cut, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_1e_cut_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_1e_cut_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1p)">
    THStack *sQ2_1p = new THStack("Q^{2} (1p, CD & FD)", "Q^{2} (1p, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_1p = new TH1D("Q^{2} (1p, FD)", "Q^{2} (1p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_1p_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1n)">
    THStack *sQ2_1n = new THStack("Q^{2} (1n, CD & FD)", "Q^{2} (1n, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_1n = new TH1D("Q^{2} (1n, FD)", "Q^{2} (1n, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_1n_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (2p)">
    THStack *sQ2_2p = new THStack("Q^{2} (2p, CD & FD)", "Q^{2} (2p, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_2p = new TH1D("Q^{2} (2p, FD)", "Q^{2} (2p, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_2p_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (pFDpCD)">
    THStack *sQ2_pFDpCD = new THStack("Q^{2} (pFDpCD, CD & FD)", "Q^{2} (pFDpCD, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_pFDpCD = new TH1D("Q^{2} (pFDpCD, FD)", "Q^{2} (pFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_pFDpCD_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_pFDpCD_Directory"];

    TH2D *hQ2_VS_W_pFDpCD = new TH2D("Q^{2} vs. W (All Int., pFDpCD)",
                                     "Q^{2} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];Q^{2} [GeV^{2}/c^{2}]",
                                     numTH2Dbins_Momentum_transfer_plots, W_lboundary, W_uboundary, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_VS_W_pFDpCD_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (nFDpCD)">
    THStack *sQ2_nFDpCD = new THStack("Q^{2} (nFDpCD, CD & FD)", "Q^{2} (nFDpCD, CD & FD);Q^{2} [GeV^{2}/c^{2}];");
    TH1D *hQ2_nFDpCD = new TH1D("Q^{2} (nFDpCD, FD)", "Q^{2} (nFDpCD, FD);Q^{2} [GeV^{2}/c^{2}];", numTH1Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_nFDpCD_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_nFDpCD_Directory"];

    TH2D *hQ2_VS_W_nFDpCD = new TH2D("Q^{2} vs. W (All Int., nFDpCD)",
                                     "Q^{2} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];Q^{2} [GeV^{2}/c^{2}]",
                                     numTH2Dbins_Momentum_transfer_plots, W_lboundary, W_uboundary, numTH2Dbins_Momentum_transfer_plots, 0, beamE * 1.1);
    string hQ2_VS_W_nFDpCD_Dir = directories.Momentum_transfer_Directory_map["Momentum_transfer_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Energy (E_e) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy (E_e) histograms">

    //<editor-fold desc="E_e plots (1e cut)">
    /* Energy (E_e) histograms (1e cut, CD & FD) */
    THStack *sE_e_1e_cut = new THStack("E_{e} (1e Cut)", "E_{e} Histogram (1e Cut);E_{e} [GeV]");
    string sE_e_1e_cut_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_stack_Directory"];

    TH1D *hE_e_All_Int_1e_cut_FD = new TH1D("E_{e} (1e Cut)", "E_{e} Histogram (All Int., 1e Cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                            beamE * 1.1);
    TH1D *hE_e_QEL_1e_cut_FD = new TH1D("E_{e} (QEL Only, 1e cut)", "E_{e} Histogram (QEL Only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_MEC_1e_cut_FD = new TH1D("E_{e} (MEC Only, 1e cut)", "E_{e} Histogram (MEC Only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_RES_1e_cut_FD = new TH1D("E_{e} (RES Only, 1e cut)", "E_{e} Histogram (RES Only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_DIS_1e_cut_FD = new TH1D("E_{e} (DIS Only, 1e cut)", "E_{e} Histogram (DIS Only, 1e cut);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    string hE_e_All_Int_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];
    string hE_e_QEL_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];
    string hE_e_MEC_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];
    string hE_e_RES_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];
    string hE_e_DIS_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1e_cut_Directory"];

    TH2D *hE_e_VS_Theta_e_All_Int_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1e Cut)",
                                                       "E_{e} vs. #theta_{e} (All Int., 1e Cut);#theta_{e} [Deg];E_{e} [GeV]",
                                                       numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1e cut)",
                                                   "E_{e} vs. #theta_{e} (QEL Only, 1e cut);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 1e cut)",
                                                   "E_{e} vs. #theta_{e} (MEC Only, 1e cut);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 1e cut)",
                                                   "E_{e} vs. #theta_{e} (RES Only, 1e cut);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 1e cut)",
                                                   "E_{e} vs. #theta_{e} (DIS Only, 1e cut);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];
    string hE_e_VS_Theta_e_QEL_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];
    string hE_e_VS_Theta_e_MEC_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];
    string hE_e_VS_Theta_e_RES_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];
    string hE_e_VS_Theta_e_DIS_1e_cut_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1e_cut_Directory"];
    //</editor-fold>

    //<editor-fold desc="E_e plots (1p)">
    /* Energy (E_e) histograms (1p, CD & FD) */
    THStack *sE_e_1p_FD = new THStack("E_{e} (1p)", "E_{e} Histogram (1p);E_{e} [GeV]");
    string sE_e_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_stack_Directory"];

    TH1D *hE_e_All_Int_1p_FD = new TH1D("E_{e} (All Int., 1p)", "E_{e} Histogram (All Int., 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_QEL_1p_FD = new TH1D("E_{e} (QEL Only, 1p)", "E_{e} Histogram (QEL Only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_MEC_1p_FD = new TH1D("E_{e} (MEC Only, 1p)", "E_{e} Histogram (MEC Only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_RES_1p_FD = new TH1D("E_{e} (RES Only, 1p)", "E_{e} Histogram (RES Only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_DIS_1p_FD = new TH1D("E_{e} (DIS Only, 1p)", "E_{e} Histogram (DIS Only, 1p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_All_Int_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_QEL_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_MEC_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_RES_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];
    string hE_e_DIS_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_Directory"];

    /* E_e vs. Theta_e (1p, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_1p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1p)",
                                                   "E_{e} vs. #theta_{e} (All Int., 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_1p_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1p)",
                                               "E_{e} vs. #theta_{e} (QEL Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_1p_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 1p)",
                                               "E_{e} vs. #theta_{e} (MEC Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_1p_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 1p)",
                                               "E_{e} vs. #theta_{e} (RES Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_1p_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 1p)",
                                               "E_{e} vs. #theta_{e} (DIS Only, 1p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_QEL_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_MEC_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_RES_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];
    string hE_e_VS_Theta_e_DIS_1p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1p_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (1p, CD & FD) */
    THStack *sE_e_15_1p_FD = new THStack("E_{e} (1p)", "E_{e} Histogram (1p);E_{e} [GeV]");
    string sE_e_15_1p_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_stack_Directory"];
    string sE_e_15_1p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1p_stack_Directory"];

    TH1D *hE_e_15_All_Int_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 1p)",
                                           "E_{e} around #theta_{e} = 15#circ (All Int., 1p);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 1p)",
                                       "E_{e} around #theta_{e} = 15#circ (QEL Only, 1p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, 1p)",
                                       "E_{e} around #theta_{e} = 15#circ (MEC Only, 1p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, 1p)",
                                       "E_{e} around #theta_{e} = 15#circ (RES Only, 1p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_1p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, 1p)",
                                       "E_{e} around #theta_{e} = 15#circ (DIS Only, 1p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
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

    TH1D *hE_e_All_Int_1n_FD = new TH1D("E_{e} (All Int., 1n)", "E_{e} Histogram (All Int., 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_QEL_1n_FD = new TH1D("E_{e} (QEL Only, 1n)", "E_{e} Histogram (QEL Only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_MEC_1n_FD = new TH1D("E_{e} (MEC Only, 1n)", "E_{e} Histogram (MEC Only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_RES_1n_FD = new TH1D("E_{e} (RES Only, 1n)", "E_{e} Histogram (RES Only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_DIS_1n_FD = new TH1D("E_{e} (DIS Only, 1n)", "E_{e} Histogram (DIS Only, 1n);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_All_Int_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_QEL_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_MEC_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_RES_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];
    string hE_e_DIS_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_Directory"];

    /* E_e vs. Theta_e (1n, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_1n_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1n)",
                                                   "E_{e} vs. #theta_{e} (All Int., 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_1n_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 1n)",
                                               "E_{e} vs. #theta_{e} (QEL Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_1n_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 1n)",
                                               "E_{e} vs. #theta_{e} (MEC Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_1n_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 1n)",
                                               "E_{e} vs. #theta_{e} (RES Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_1n_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 1n)",
                                               "E_{e} vs. #theta_{e} (DIS Only, 1n);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_QEL_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_MEC_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_RES_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];
    string hE_e_VS_Theta_e_DIS_1n_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_1n_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (1n, CD & FD) */
    THStack *sE_e_15_1n_FD = new THStack("E_{e} (1n)", "E_{e} Histogram (1n);E_{e} [GeV]");
    string sE_e_15_1n_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_stack_Directory"];
    string sE_e_15_1n_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_1n_stack_Directory"];

    TH1D *hE_e_15_All_Int_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 1n)",
                                           "E_{e} around #theta_{e} = 15#circ (All Int., 1n);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 1n)",
                                       "E_{e} around #theta_{e} = 15#circ (QEL Only, 1n);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, 1n)",
                                       "E_{e} around #theta_{e} = 15#circ (MEC Only, 1n);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, 1n)",
                                       "E_{e} around #theta_{e} = 15#circ (RES Only, 1n);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_1n_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, 1n)",
                                       "E_{e} around #theta_{e} = 15#circ (DIS Only, 1n);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
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

    TH1D *hE_e_All_Int_2p_FD = new TH1D("E_{e} (All Int., 2p)", "E_{e} Histogram (All Int., 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_QEL_2p_FD = new TH1D("E_{e} (QEL Only, 2p)", "E_{e} Histogram (QEL Only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_MEC_2p_FD = new TH1D("E_{e} (MEC Only, 2p)", "E_{e} Histogram (MEC Only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_RES_2p_FD = new TH1D("E_{e} (RES Only, 2p)", "E_{e} Histogram (RES Only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_DIS_2p_FD = new TH1D("E_{e} (DIS Only, 2p)", "E_{e} Histogram (DIS Only, 2p);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_All_Int_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_QEL_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_MEC_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_RES_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];
    string hE_e_DIS_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_Directory"];

    /* E_e vs. Theta_e (2p, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_2p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 2p)",
                                                   "E_{e} vs. #theta_{e} (All Int., 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_2p_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p)",
                                               "E_{e} vs. #theta_{e} (QEL Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_2p_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 2p)",
                                               "E_{e} vs. #theta_{e} (MEC Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_2p_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 2p)",
                                               "E_{e} vs. #theta_{e} (RES Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_2p_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 2p)",
                                               "E_{e} vs. #theta_{e} (DIS Only, 2p);#theta_{e} [Deg];E_{e} [GeV]",
                                               numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_QEL_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_MEC_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_RES_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];
    string hE_e_VS_Theta_e_DIS_2p_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_2p_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (2p, CD & FD) */
    THStack *sE_e_15_2p_FD = new THStack("E_{e} (2p)", "E_{e} Histogram (2p);E_{e} [GeV]");
    string sE_e_15_2p_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_stack_Directory"];
    string sE_e_15_2p_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_2p_stack_Directory"];

    TH1D *hE_e_15_All_Int_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., 2p)",
                                           "E_{e} around #theta_{e} = 15#circ (All Int., 2p);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, 2p)",
                                       "E_{e} around #theta_{e} = 15#circ (QEL Only, 2p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, 2p)",
                                       "E_{e} around #theta_{e} = 15#circ (MEC Only, 2p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, 2p)",
                                       "E_{e} around #theta_{e} = 15#circ (RES Only, 2p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_2p_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, 2p)",
                                       "E_{e} around #theta_{e} = 15#circ (DIS Only, 2p);E_{e} [GeV]",
                                       numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_15_All_Int_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_QEL_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_MEC_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_RES_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    string hE_e_15_DIS_2p_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="E_e plots (pFDpCD)">
    /* Energy (E_e) histograms (pFDpCD, CD & FD) */
    THStack *sE_e_pFDpCD_FD = new THStack("E_{e} (pFDpCD)", "E_{e} Histogram (pFDpCD);E_{e} [GeV]");
    string sE_e_pFDpCD_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_stack_Directory"];
    string sE_e_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_stack_Directory"];

    TH1D *hE_e_All_Int_pFDpCD_FD = new TH1D("E_{e} (All Int., pFDpCD)", "E_{e} Histogram (All Int., pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                            beamE * 1.1);
    TH1D *hE_e_QEL_pFDpCD_FD = new TH1D("E_{e} (QEL Only, pFDpCD)", "E_{e} Histogram (QEL Only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_MEC_pFDpCD_FD = new TH1D("E_{e} (MEC Only, pFDpCD)", "E_{e} Histogram (MEC Only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_RES_pFDpCD_FD = new TH1D("E_{e} (RES Only, pFDpCD)", "E_{e} Histogram (RES Only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_DIS_pFDpCD_FD = new TH1D("E_{e} (DIS Only, pFDpCD)", "E_{e} Histogram (DIS Only, pFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    string hE_e_All_Int_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_Directory"];
    string hE_e_QEL_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_Directory"];
    string hE_e_MEC_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_Directory"];
    string hE_e_RES_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_Directory"];
    string hE_e_DIS_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_Directory"];

    /* E_e vs. Theta_e (pFDpCD, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_pFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., pFDpCD)",
                                                       "E_{e} vs. #theta_{e} (All Int., pFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                       numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_pFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, pFDpCD)",
                                                   "E_{e} vs. #theta_{e} (QEL Only, pFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_pFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, pFDpCD)",
                                                   "E_{e} vs. #theta_{e} (MEC Only, pFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_pFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, pFDpCD)",
                                                   "E_{e} vs. #theta_{e} (RES Only, pFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_pFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, pFDpCD)",
                                                   "E_{e} vs. #theta_{e} (DIS Only, pFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_pFDpCD_Directory"];
    string hE_e_VS_Theta_e_QEL_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_pFDpCD_Directory"];
    string hE_e_VS_Theta_e_MEC_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_pFDpCD_Directory"];
    string hE_e_VS_Theta_e_RES_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_pFDpCD_Directory"];
    string hE_e_VS_Theta_e_DIS_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_pFDpCD_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (pFDpCD, CD & FD) */
    THStack *sE_e_15_pFDpCD_FD = new THStack("E_{e} (pFDpCD)", "E_{e} Histogram (pFDpCD);E_{e} [GeV]");
    string sE_e_15_pFDpCD_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_stack_Directory"];
    string sE_e_15_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_pFDpCD_stack_Directory"];

    TH1D *hE_e_15_All_Int_pFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., pFDpCD)",
                                               "E_{e} around #theta_{e} = 15#circ (All Int., pFDpCD);E_{e} [GeV]",
                                               numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_pFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, pFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (QEL Only, pFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_pFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, pFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (MEC Only, pFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_pFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, pFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (RES Only, pFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_pFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, pFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (DIS Only, pFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_15_All_Int_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_pFDpCD_Directory"];
    string hE_e_15_QEL_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_pFDpCD_Directory"];
    string hE_e_15_MEC_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_pFDpCD_Directory"];
    string hE_e_15_RES_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_pFDpCD_Directory"];
    string hE_e_15_DIS_pFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="E_e plots (nFDpCD)">
    /* Energy (E_e) histograms (nFDpCD, CD & FD) */
    THStack *sE_e_nFDpCD_FD = new THStack("E_{e} (nFDpCD)", "E_{e} Histogram (nFDpCD);E_{e} [GeV]");
    string sE_e_nFDpCD_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_stack_Directory"];
    string sE_e_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_stack_Directory"];

    TH1D *hE_e_All_Int_nFDpCD_FD = new TH1D("E_{e} (All Int., nFDpCD)", "E_{e} Histogram (All Int., nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                            beamE * 1.1);
    TH1D *hE_e_QEL_nFDpCD_FD = new TH1D("E_{e} (QEL Only, nFDpCD)", "E_{e} Histogram (QEL Only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_MEC_nFDpCD_FD = new TH1D("E_{e} (MEC Only, nFDpCD)", "E_{e} Histogram (MEC Only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_RES_nFDpCD_FD = new TH1D("E_{e} (RES Only, nFDpCD)", "E_{e} Histogram (RES Only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    TH1D *hE_e_DIS_nFDpCD_FD = new TH1D("E_{e} (DIS Only, nFDpCD)", "E_{e} Histogram (DIS Only, nFDpCD);E_{e} [GeV]", numTH1Dbins_E_e_Plots, 0,
                                        beamE * 1.1);
    string hE_e_All_Int_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_Directory"];
    string hE_e_QEL_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_Directory"];
    string hE_e_MEC_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_Directory"];
    string hE_e_RES_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_Directory"];
    string hE_e_DIS_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_Directory"];

    /* E_e vs. Theta_e (nFDpCD, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_nFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., nFDpCD)",
                                                       "E_{e} vs. #theta_{e} (All Int., nFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                       numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_nFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, nFDpCD)",
                                                   "E_{e} vs. #theta_{e} (QEL Only, nFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_nFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, nFDpCD)",
                                                   "E_{e} vs. #theta_{e} (MEC Only, nFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_nFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, nFDpCD)",
                                                   "E_{e} vs. #theta_{e} (RES Only, nFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_nFDpCD_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, nFDpCD)",
                                                   "E_{e} vs. #theta_{e} (DIS Only, nFDpCD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   numTH2Dbins_E_e_Plots, 0, 50, numTH2Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_nFDpCD_Directory"];
    string hE_e_VS_Theta_e_QEL_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_nFDpCD_Directory"];
    string hE_e_VS_Theta_e_MEC_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_nFDpCD_Directory"];
    string hE_e_VS_Theta_e_RES_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_nFDpCD_Directory"];
    string hE_e_VS_Theta_e_DIS_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_VS_Theta_e_All_Int_nFDpCD_Directory"];

    /* Energy (E_e) histograms around theta_e = 15 (nFDpCD, CD & FD) */
    THStack *sE_e_15_nFDpCD_FD = new THStack("E_{e} (nFDpCD)", "E_{e} Histogram (nFDpCD);E_{e} [GeV]");
    string sE_e_15_nFDpCD_CD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_stack_Directory"];
    string sE_e_15_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_All_Int_nFDpCD_stack_Directory"];

    TH1D *hE_e_15_All_Int_nFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (All Int., nFDpCD)",
                                               "E_{e} around #theta_{e} = 15#circ (All Int., nFDpCD);E_{e} [GeV]",
                                               numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_QEL_nFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (QEL Only, nFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (QEL Only, nFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_MEC_nFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (MEC Only, nFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (MEC Only, nFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_RES_nFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (RES Only, nFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (RES Only, nFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    TH1D *hE_e_15_DIS_nFDpCD_FD = new TH1D("E_{e} around #theta_{e} = 15#circ (DIS Only, nFDpCD)",
                                           "E_{e} around #theta_{e} = 15#circ (DIS Only, nFDpCD);E_{e} [GeV]",
                                           numTH1Dbins_E_e_Plots, 0, beamE * 1.1);
    string hE_e_15_All_Int_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_nFDpCD_Directory"];
    string hE_e_15_QEL_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_nFDpCD_Directory"];
    string hE_e_15_MEC_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_nFDpCD_Directory"];
    string hE_e_15_RES_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_nFDpCD_Directory"];
    string hE_e_15_DIS_nFDpCD_FD_Dir = directories.E_e_Directory_map["E_e_15_All_Int_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Energy Transfer (ET) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy Transfer (ET) histograms">
    string tET_All_Ang = "Energy transfer #omega for every #theta_{e}", tET15 = "Energy transfer #omega Around #theta_{e} = 15#circ";

    //<editor-fold desc="ET for every theta_e (1p)">
    THStack *sET_All_Ang_All_Int_1p_FD = new THStack("#omega for all #theta_{e} (1p)",
                                                     "Energy transfer #omega for all #theta_{e} (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1p_Directory"];

    TH1D *hET_All_Ang_All_Int_1p_FD = new TH1D("#omega for all #theta_{e} (All Int., 1p)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1p_Directory"];
    string hET_All_Ang_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1p_Directory"];
    string hET_All_Ang_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1p_Directory"];
    string hET_All_Ang_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1p_Directory"];
    string hET_All_Ang_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (1n)">
    THStack *sET_All_Ang_All_Int_1n_FD = new THStack("#omega for all #theta_{e} (1n)",
                                                     "Energy transfer #omega for all #theta_{e} (1n);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1n_Directory"];

    TH1D *hET_All_Ang_All_Int_1n_FD = new TH1D("#omega for all #theta_{e} (All Int., 1n)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1n_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1n_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1n_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1n_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1n_Directory"];
    string hET_All_Ang_QEL_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1n_Directory"];
    string hET_All_Ang_MEC_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1n_Directory"];
    string hET_All_Ang_RES_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1n_Directory"];
    string hET_All_Ang_DIS_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (2p)">
    THStack *sET_All_Ang_All_Int_2p_FD = new THStack("#omega for all #theta_{e} (2p)",
                                                     "Energy transfer #omega for all #theta_{e} (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_2p_Directory"];

    TH1D *hET_All_Ang_All_Int_2p_FD = new TH1D("#omega for all #theta_{e} (All Int., 2p)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_2p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_2p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_2p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_2p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_2p_Directory"];
    string hET_All_Ang_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_2p_Directory"];
    string hET_All_Ang_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_2p_Directory"];
    string hET_All_Ang_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_2p_Directory"];
    string hET_All_Ang_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (pFDpCD)">
    THStack *sET_All_Ang_All_Int_pFDpCD_FD = new THStack("#omega for all #theta_{e} (pFDpCD)",
                                                         "Energy transfer #omega for all #theta_{e} (pFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_pFDpCD_Directory"];

    TH1D *hET_All_Ang_All_Int_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (All Int., pFDpCD)",
                                                   "Energy transfer #omega for all #theta_{e} (All Int., pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                                   numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (QEL Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (QEL Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (MEC Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (MEC Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (RES Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (RES Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (DIS Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (DIS Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_QEL_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_MEC_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_RES_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_DIS_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (nFDpCD)">
    THStack *sET_All_Ang_All_Int_nFDpCD_FD = new THStack("#omega for all #theta_{e} (nFDpCD)",
                                                         "Energy transfer #omega for all #theta_{e} (nFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_nFDpCD_Directory"];

    TH1D *hET_All_Ang_All_Int_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (All Int., nFDpCD)",
                                                   "Energy transfer #omega for all #theta_{e} (All Int., nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                                   numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (QEL Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (QEL Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (MEC Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (MEC Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (RES Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (RES Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (DIS Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (DIS Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_QEL_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_MEC_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_RES_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_DIS_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (1p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_1p_FD = new THStack("ET around #theta_{e} = 15#circ (1p)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_1p_Directory"];

    TH1D *hET15_All_Int_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (All Int., 1p)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
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
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
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
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_2p_Directory"];
    string hET15_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_2p_Directory"];
    string hET15_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_2p_Directory"];
    string hET15_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_2p_Directory"];
    string hET15_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (pFDpCD)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_pFDpCD_FD = new THStack("#omega around #theta_{e} = 15#circ (pFDpCD)",
                                                   "Energy transfer #omega Around #theta_{e} = 15#circ (pFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_pFDpCD_Directory"];

    TH1D *hET15_All_Int_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., pFDpCD)",
                                             "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                             numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_pFDpCD_Directory"];
    string hET15_QEL_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_pFDpCD_Directory"];
    string hET15_MEC_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_pFDpCD_Directory"];
    string hET15_RES_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_pFDpCD_Directory"];
    string hET15_DIS_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (nFDpCD)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_nFDpCD_FD = new THStack("#omega around #theta_{e} = 15#circ (nFDpCD)",
                                                   "Energy transfer #omega Around #theta_{e} = 15#circ (nFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_nFDpCD_Directory"];

    TH1D *hET15_All_Int_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., nFDpCD)",
                                             "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                             numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_nFDpCD_Directory"];
    string hET15_QEL_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_nFDpCD_Directory"];
    string hET15_MEC_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_nFDpCD_Directory"];
    string hET15_RES_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_nFDpCD_Directory"];
    string hET15_DIS_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Ecal reconstruction histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Ecal reconstruction histograms">

    //<editor-fold desc="Ecal reconstruction histograms (1p)">

    //<editor-fold desc="Ecal plots (1p)">
    THStack *sEcal_1p = new THStack("E_{cal} Reconstruction (1p)", "E_{cal} Reconstruction (1p);E_{cal} = E_{e} + T_{p} [GeV]");
    string sEcal_1p_Dir = directories.Ecal_Directory_map["Ecal_stack_1p_Directory"];

    TH1D *hEcal_All_Int_1p = new TH1D("E_{cal} reco. (All Int., 1p)", "E_{cal} Reconstruction (All Int., 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_1p = new TH1D("E_{cal} reco. (QEL only, 1p)", "E_{cal} Reconstruction (QEL only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_1p = new TH1D("E_{cal} reco. (MEC only, 1p)", "E_{cal} Reconstruction (MEC only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_1p = new TH1D("E_{cal} reco. (RES only, 1p)", "E_{cal} Reconstruction (RES only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_1p = new TH1D("E_{cal} reco. (DIS only, 1p)", "E_{cal} Reconstruction (DIS only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_1p_Dir = directories.Ecal_Directory_map["Ecal_All_Int_1p_Directory"];
    string hEcal_QEL_1p_Dir = directories.Ecal_Directory_map["Ecal_QEL_1p_Directory"];
    string hEcal_MEC_1p_Dir = directories.Ecal_Directory_map["Ecal_MEC_1p_Directory"];
    string hEcal_RES_1p_Dir = directories.Ecal_Directory_map["Ecal_RES_1p_Directory"];
    string hEcal_DIS_1p_Dir = directories.Ecal_Directory_map["Ecal_DIS_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (1p)">
    TH2D *hEcal_vs_P_e_1p = new TH2D("E_{cal} vs. P_{e} (All Int., 1p)", "E_{cal} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_1p = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1p)",
                                          "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1p);P_{e} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                          beamE * 1.35);
    string hEcal_vs_P_e_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1p_Directory"];

    TH2D *hEcal_vs_P_p_1p = new TH2D("E_{cal} vs. P_{p} (All Int., 1p)", "E_{cal} vs. P_{p} (All Int., 1p);P_{p} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_p_test_1p = new TH2D("E_{cal} vs. P_{p} for E_{cal}>E_{beam} (All Int., 1p)",
                                          "E_{cal} vs. P_{p} for E_{cal}>E_{beam} (All Int., 1p);P_{p} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_p_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (1p)">
    TH2D *hEcal_vs_Theta_e_1p = new TH2D("E_{cal} vs. #theta_{e} (All Int., 1p)",
                                         "E_{cal} vs. #theta_{e} (All Int., 1p);#theta_{e} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                         beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_1p = new TH2D("E_{cal} vs. #phi_{e} (All Int., 1p)",
                                       "E_{cal} vs. #phi_{e} (All Int., 1p);#phi_{e} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_1p = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1p)",
                                              "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1p);#theta_{e} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                              beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_1p = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1p)",
                                            "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1p);#phi_{e} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];
    string hEcal_vs_Phi_e_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];

    TH2D *hEcal_vs_Theta_p_1p = new TH2D("E_{cal} vs. #theta_{p} (All Int., 1p)",
                                         "E_{cal} vs. #theta_{p} (All Int., 1p);#theta_{p} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                         beamE * 1.35);
    TH2D *hEcal_vs_Phi_p_1p = new TH2D("E_{cal} vs. #phi_{p} (All Int., 1p)",
                                       "E_{cal} vs. #phi_{p} (All Int., 1p);#phi_{p} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_p_test_1p = new TH2D("E_{cal} vs. #theta_{p} for E_{cal}>E_{beam} (All Int., 1p)",
                                              "E_{cal} vs. #theta_{p} for E_{cal}>E_{beam} (All Int., 1p);#theta_{p} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                              beamE * 1.35);
    TH2D *hEcal_vs_Phi_p_test_1p = new TH2D("E_{cal} vs. #phi_{p} for E_{cal}>E_{beam} (All Int., 1p)",
                                            "E_{cal} vs. #phi_{p} for E_{cal}>E_{beam} (All Int., 1p);#phi_{p} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_p_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];
    string hEcal_vs_Phi_p_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (1p)">
    TH2D *hEcal_vs_dAlpha_T_1p = new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1p)",
                                          "E_{cal} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [Deg];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (1p)">
    TH2D *hEcal_vs_dP_T_1p = new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1p)",
                                      "E_{cal} vs. #deltaP_{T} (All Int., 1p);#deltaP_{T} [GeV/c];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (1n)">

    //<editor-fold desc="Ecal plots (1n)">
    THStack *sEcal_1n = new THStack("E_{cal} Reconstruction (1n)", "E_{cal} Reconstruction (1n);E_{cal} = E_{e} + T_{n} [GeV]");
    string sEcal_1n_Dir = directories.Ecal_Directory_map["Ecal_stack_1n_Directory"];

    TH1D *hEcal_All_Int_1n = new TH1D("E_{cal} reco. (All Int., 1n)", "E_{cal} Reconstruction (All Int., 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_1n = new TH1D("E_{cal} reco. (QEL only, 1n)", "E_{cal} Reconstruction (QEL only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_1n = new TH1D("E_{cal} reco. (MEC only, 1n)", "E_{cal} Reconstruction (MEC only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_1n = new TH1D("E_{cal} reco. (RES only, 1n)", "E_{cal} Reconstruction (RES only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_1n = new TH1D("E_{cal} reco. (DIS only, 1n)", "E_{cal} Reconstruction (DIS only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_1n_Dir = directories.Ecal_Directory_map["Ecal_All_Int_1n_Directory"];
    string hEcal_QEL_1n_Dir = directories.Ecal_Directory_map["Ecal_QEL_1n_Directory"];
    string hEcal_MEC_1n_Dir = directories.Ecal_Directory_map["Ecal_MEC_1n_Directory"];
    string hEcal_RES_1n_Dir = directories.Ecal_Directory_map["Ecal_RES_1n_Directory"];
    string hEcal_DIS_1n_Dir = directories.Ecal_Directory_map["Ecal_DIS_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (1n)">
    TH2D *hEcal_vs_P_e_1n = new TH2D("E_{cal} vs. P_{e} (All Int., 1n)", "E_{cal} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_1n = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1n)",
                                          "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1n);P_{e} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                          beamE * 1.35);
    string hEcal_vs_P_e_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1n_Directory"];

    TH2D *hEcal_vs_P_n_1n = new TH2D("E_{cal} vs. P_{n} (All Int., 1n)", "E_{cal} vs. P_{n} (All Int., 1n);P_{n} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_n_test_1n = new TH2D("E_{cal} vs. P_{n} for E_{cal}>E_{beam} (All Int., 1n)",
                                          "E_{cal} vs. P_{n} for E_{cal}>E_{beam} (All Int., 1n);P_{n} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_n_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1n_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (1n)">
    TH2D *hEcal_vs_Theta_e_1n = new TH2D("E_{cal} vs. #theta_{e} (All Int., 1n)",
                                         "E_{cal} vs. #theta_{e} (All Int., 1n);#theta_{e} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                         beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_1n = new TH2D("E_{cal} vs. #phi_{e} (All Int., 1n)",
                                       "E_{cal} vs. #phi_{e} (All Int., 1n);#phi_{e} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_1n = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1n)",
                                              "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1n);#theta_{e} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                              beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_1n = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1n)",
                                            "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1n);#phi_{e} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];
    string hEcal_vs_Phi_e_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];

    TH2D *hEcal_vs_Theta_n_1n = new TH2D("E_{cal} vs. #theta_{n} (All Int., 1n)",
                                         "E_{cal} vs. #theta_{n} (All Int., 1n);#theta_{n} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                         beamE * 1.35);
    TH2D *hEcal_vs_Phi_n_1n = new TH2D("E_{cal} vs. #phi_{n} (All Int., 1n)",
                                       "E_{cal} vs. #phi_{n} (All Int., 1n);#phi_{n} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_n_test_1n = new TH2D("E_{cal} vs. #theta_{n} for E_{cal}>E_{beam} (All Int., 1n)",
                                              "E_{cal} vs. #theta_{n} for E_{cal}>E_{beam} (All Int., 1n);#theta_{n} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                              beamE * 1.35);
    TH2D *hEcal_vs_Phi_n_test_1n = new TH2D("E_{cal} vs. #phi_{n} for E_{cal}>E_{beam} (All Int., 1n)",
                                            "E_{cal} vs. #phi_{n} for E_{cal}>E_{beam} (All Int., 1n);#phi_{n} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_n_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];
    string hEcal_vs_Phi_n_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (1n)">
    TH2D *hEcal_vs_dAlpha_T_1n = new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1n)",
                                          "E_{cal} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [Deg];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1n_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (1n)">
    TH2D *hEcal_vs_dP_T_1n = new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1n)",
                                      "E_{cal} vs. #deltaP_{T} (All Int., 1n);#deltaP_{T} [GeV/c];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1n_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (2p)">

    //<editor-fold desc="Ecal plots (2p)">
    THStack *sEcal_2p = new THStack("E_{cal} Reconstruction (2p)", "E_{cal} Reconstruction (2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]");
    string sEcal_2p_Dir = directories.Ecal_Directory_map["Ecal_stack_2p_Directory"];

    TH1D *hEcal_All_Int_2p = new TH1D("E_{cal} reco. (All Int., 2p)",
                                      "E_{cal} Reconstruction (All Int., 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_2p = new TH1D("E_{cal} reco. (QEL only, 2p)",
                                  "E_{cal} Reconstruction (QEL only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_2p = new TH1D("E_{cal} reco. (MEC only, 2p)",
                                  "E_{cal} Reconstruction (MEC only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_2p = new TH1D("E_{cal} reco. (RES only, 2p)",
                                  "E_{cal} Reconstruction (RES only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_2p = new TH1D("E_{cal} reco. (DIS only, 2p)",
                                  "E_{cal} Reconstruction (DIS only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_2p_Dir = directories.Ecal_Directory_map["Ecal_All_Int_2p_Directory"];
    string hEcal_QEL_2p_Dir = directories.Ecal_Directory_map["Ecal_QEL_2p_Directory"];
    string hEcal_MEC_2p_Dir = directories.Ecal_Directory_map["Ecal_MEC_2p_Directory"];
    string hEcal_RES_2p_Dir = directories.Ecal_Directory_map["Ecal_RES_2p_Directory"];
    string hEcal_DIS_2p_Dir = directories.Ecal_Directory_map["Ecal_DIS_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (2p)">
    TH2D *hEcal_vs_dAlpha_T_L_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                            "E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                              "E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    string hEcal_vs_dAlpha_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (2p)">
    TH2D *hEcal_vs_dP_T_L_2p = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., 2p)",
                                        "E_{cal} vs. #deltaP_{T,L} (All Int., 2p);#deltaP_{T,L} [GeV/c];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_2p = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., 2p)",
                                          "E_{cal} vs. #deltaP_{T,tot} (All Int., 2p);#deltaP_{T,tot} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    string hEcal_vs_dP_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (pFDpCD)">

    //<editor-fold desc="Ecal plots (pFDpCD)">
    THStack *sEcal_pFDpCD = new THStack("E_{cal} Reconstruction (pFDpCD)",
                                        "E_{cal} Reconstruction (pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]");
    string sEcal_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_pFDpCD_Directory"];

    TH1D *hEcal_All_Int_pFDpCD = new TH1D("E_{cal} reco. (All Int., pFDpCD)",
                                          "E_{cal} Reconstruction (All Int., pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                          numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_pFDpCD = new TH1D("E_{cal} reco. (QEL only, pFDpCD)",
                                      "E_{cal} Reconstruction (QEL only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_pFDpCD = new TH1D("E_{cal} reco. (MEC only, pFDpCD)",
                                      "E_{cal} Reconstruction (MEC only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_pFDpCD = new TH1D("E_{cal} reco. (RES only, pFDpCD)",
                                      "E_{cal} Reconstruction (RES only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_pFDpCD = new TH1D("E_{cal} reco. (DIS only, pFDpCD)",
                                      "E_{cal} Reconstruction (DIS only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_All_Int_pFDpCD_Directory"];
    string hEcal_QEL_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_QEL_pFDpCD_Directory"];
    string hEcal_MEC_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_MEC_pFDpCD_Directory"];
    string hEcal_RES_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_RES_pFDpCD_Directory"];
    string hEcal_DIS_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_DIS_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (pFDpCD)">
    TH2D *hEcal_vs_P_e_pFDpCD = new TH2D("E_{cal} vs. P_{e} (All Int., pFDpCD)",
                                         "E_{cal} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                         beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_pFDpCD = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                              "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                              beamE * 1.35);
    string hEcal_vs_P_e_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"];

    TH2D *hEcal_vs_P_pFD_pFDpCD = new TH2D("E_{cal} vs. P_{pFD} (All Int., pFDpCD)",
                                           "E_{cal} vs. P_{pFD} (All Int., pFDpCD);P_{pFD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                           beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_pFDpCD = new TH2D("E_{cal} vs. P_{pCD} (All Int., pFDpCD)",
                                           "E_{cal} vs. P_{pCD} (All Int., pFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                           beamE * 1.35);
    TH2D *hEcal_vs_P_pFD_test_pFDpCD = new TH2D("E_{cal} vs. P_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                "E_{cal} vs. P_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{pFD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                                beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_test_pFDpCD = new TH2D("E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                "E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                                beamE * 1.35);
    string hEcal_vs_P_pFD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"];
    string hEcal_vs_P_pCD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (pFDpCD)">
    TH2D *hEcal_vs_Theta_e_pFDpCD = new TH2D("E_{cal} vs. #theta_{e} (All Int., pFDpCD)",
                                             "E_{cal} vs. #theta_{e} (All Int., pFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                             beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_pFDpCD = new TH2D("E_{cal} vs. #phi_{e} (All Int., pFDpCD)",
                                           "E_{cal} vs. #phi_{e} (All Int., pFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_pFDpCD = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                                  beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_pFDpCD = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    string hEcal_vs_Phi_e_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_pFD_pFDpCD = new TH2D("E_{cal} vs. #theta_{pFD} (All Int., pFDpCD)",
                                               "E_{cal} vs. #theta_{pFD} (All Int., pFDpCD);#theta_{pFD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                               beamE * 1.35);
    TH2D *hEcal_vs_Phi_pFD_pFDpCD = new TH2D("E_{cal} vs. #phi_{pFD} (All Int., pFDpCD)",
                                             "E_{cal} vs. #phi_{pFD} (All Int., pFDpCD);#phi_{pFD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_pFD_test_pFDpCD = new TH2D("E_{cal} vs. #theta_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                    "E_{cal} vs. #theta_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{pFD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                                    beamE * 1.35);
    TH2D *hEcal_vs_Phi_pFD_test_pFDpCD = new TH2D("E_{cal} vs. #phi_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #phi_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{pFD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_pFD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    string hEcal_vs_Phi_pFD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_pCD_pFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} (All Int., pFDpCD)",
                                               "E_{cal} vs. #theta_{pCD} (All Int., pFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0,
                                               beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_pFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} (All Int., pFDpCD)",
                                             "E_{cal} vs. #phi_{pCD} (All Int., pFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_pCD_test_pFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                    "E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0,
                                                    beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_test_pFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_pCD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    string hEcal_vs_Phi_pCD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (pFDpCD)">
    TH2D *hEcal_vs_dAlpha_T_L_pFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., pFDpCD)",
                                                "E_{cal} vs. #delta#alpha_{T,L} (All Int., pFDpCD);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_pFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    string hEcal_vs_dAlpha_T_tot_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (pFDpCD)">
    TH2D *hEcal_vs_dP_T_L_pFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., pFDpCD)",
                                            "E_{cal} vs. #deltaP_{T,L} (All Int., pFDpCD);#deltaP_{T,L} [GeV];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_pFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD)",
                                              "E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    string hEcal_vs_dP_T_tot_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. W (pFDpCD)">
    TH2D *hEcal_vs_W_pFDpCD = new TH2D("E_{cal} vs. W (All Int., pFDpCD)",
                                       "E_{cal} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV];",
                                       numTH2Dbins_E_cal_Plots, W_lboundary, W_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_W_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (nFDpCD)">

    //<editor-fold desc="Ecal plots (nFDpCD)">
    THStack *sEcal_nFDpCD = new THStack("E_{cal} Reconstruction (nFDpCD)",
                                        "E_{cal} Reconstruction (nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]");
    string sEcal_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_nFDpCD_Directory"];

    TH1D *hEcal_All_Int_nFDpCD = new TH1D("E_{cal} reco. (All Int., nFDpCD)",
                                          "E_{cal} Reconstruction (All Int., nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                          numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_nFDpCD = new TH1D("E_{cal} reco. (QEL only, nFDpCD)",
                                      "E_{cal} Reconstruction (QEL only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_nFDpCD = new TH1D("E_{cal} reco. (MEC only, nFDpCD)",
                                      "E_{cal} Reconstruction (MEC only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_nFDpCD = new TH1D("E_{cal} reco. (RES only, nFDpCD)",
                                      "E_{cal} Reconstruction (RES only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_nFDpCD = new TH1D("E_{cal} reco. (DIS only, nFDpCD)",
                                      "E_{cal} Reconstruction (DIS only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_All_Int_nFDpCD_Directory"];
    string hEcal_QEL_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_QEL_nFDpCD_Directory"];
    string hEcal_MEC_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_MEC_nFDpCD_Directory"];
    string hEcal_RES_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_RES_nFDpCD_Directory"];
    string hEcal_DIS_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_DIS_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (nFDpCD)">
    TH2D *hEcal_vs_P_e_nFDpCD = new TH2D("E_{cal} vs. P_{e} (All Int., nFDpCD)",
                                         "E_{cal} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                         beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_nFDpCD = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                              "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                              beamE * 1.35);
    string hEcal_vs_P_e_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"];

    TH2D *hEcal_vs_P_nFD_nFDpCD = new TH2D("E_{cal} vs. P_{nFD} (All Int., nFDpCD)",
                                           "E_{cal} vs. P_{nFD} (All Int., nFDpCD);P_{nFD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                           beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_nFDpCD = new TH2D("E_{cal} vs. P_{pCD} (All Int., nFDpCD)",
                                           "E_{cal} vs. P_{pCD} (All Int., nFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                           beamE * 1.35);
    TH2D *hEcal_vs_P_nFD_test_nFDpCD = new TH2D("E_{cal} vs. P_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                "E_{cal} vs. P_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{nFD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                                beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_test_nFDpCD = new TH2D("E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                "E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0,
                                                beamE * 1.35);
    string hEcal_vs_P_nFD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"];
    string hEcal_vs_P_pCD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (nFDpCD)">
    TH2D *hEcal_vs_Theta_e_nFDpCD = new TH2D("E_{cal} vs. #theta_{e} (All Int., nFDpCD)",
                                             "E_{cal} vs. #theta_{e} (All Int., nFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                             beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_nFDpCD = new TH2D("E_{cal} vs. #phi_{e} (All Int., nFDpCD)",
                                           "E_{cal} vs. #phi_{e} (All Int., nFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_nFDpCD = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                                  beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_nFDpCD = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    string hEcal_vs_Phi_e_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_nFD_nFDpCD = new TH2D("E_{cal} vs. #theta_{nFD} (All Int., nFDpCD)",
                                               "E_{cal} vs. #theta_{nFD} (All Int., nFDpCD);#theta_{nFD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                               beamE * 1.35);
    TH2D *hEcal_vs_Phi_nFD_nFDpCD = new TH2D("E_{cal} vs. #phi_{nFD} (All Int., nFDpCD)",
                                             "E_{cal} vs. #phi_{nFD} (All Int., nFDpCD);#phi_{nFD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_nFD_test_nFDpCD = new TH2D("E_{cal} vs. #theta_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                    "E_{cal} vs. #theta_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{nFD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0,
                                                    beamE * 1.35);
    TH2D *hEcal_vs_Phi_nFD_test_nFDpCD = new TH2D("E_{cal} vs. #phi_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #phi_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{nFD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_nFD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    string hEcal_vs_Phi_nFD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_pCD_nFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} (All Int., nFDpCD)",
                                               "E_{cal} vs. #theta_{pCD} (All Int., nFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0,
                                               beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_nFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} (All Int., nFDpCD)",
                                             "E_{cal} vs. #phi_{pCD} (All Int., nFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_pCD_test_nFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                    "E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0,
                                                    beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_test_nFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_pCD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    string hEcal_vs_Phi_pCD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (nFDpCD)">
    TH2D *hEcal_vs_dAlpha_T_L_nFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., nFDpCD)",
                                                "E_{cal} vs. #delta#alpha_{T,L} (All Int., nFDpCD);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_nFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    string hEcal_vs_dAlpha_T_tot_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (nFDpCD)">
    TH2D *hEcal_vs_dP_T_L_nFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., nFDpCD)",
                                            "E_{cal} vs. #deltaP_{T,L} (All Int., nFDpCD);#deltaP_{T,L} [GeV];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_nFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)",
                                              "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    string hEcal_vs_dP_T_tot_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. W (nFDpCD)">
    TH2D *hEcal_vs_W_nFDpCD = new TH2D("E_{cal} vs. W (All Int., nFDpCD)",
                                       "E_{cal} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV];",
                                       numTH2Dbins_E_cal_Plots, W_lboundary, W_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_W_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// TKI histograms
// ======================================================================================================================================================================

    //<editor-fold desc="TKI histograms">

    //<editor-fold desc="TKI histograms (1p)">
    THStack *sdP_T_1p = new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1p);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_1p = new TH1D("#deltaP_{T} (1p)", "#deltaP_{T} histogram (1p);#deltaP_{T} = |#vec{p}_{T,e} + #vec{p}_{T,p}| [GeV/c]",
                              numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_1p_Dir = directories.TKI_Directory_map["dP_T_1p_Directory"];

    THStack *sdAlpha_T_1p = new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1p)",
                                        "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_1p = new TH1D("#delta#alpha_{T} (1p)", "#delta#alpha_{T} histogram (1p);#delta#alpha_{T} [Deg]",
                                  numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_1p_Dir = directories.TKI_Directory_map["dAlpha_T_1p_Directory"];

    THStack *sdPhi_T_1p = new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1p)",
                                      "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_1p = new TH1D("#delta#phi_{T} (1p)", "#delta#phi_{T} histogram (1p);#delta#phi_{T} [Deg]",
                                numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_1p_Dir = directories.TKI_Directory_map["dPhi_T_1p_Directory"];

    TH2D *hdP_T_vs_dAlpha_T_1p = new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p)",
                                          "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [Deg];#deltaP_{T} [GeV/c];",
                                          numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_vs_dAlpha_T_1p_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI histograms (1n)">
    THStack *sdP_T_1n = new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1n)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1n);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_1n = new TH1D("#deltaP_{T} (1n)", "#deltaP_{T} histogram (1n);#deltaP_{T} = |#vec{p}_{T,e} + #vec{p}_{T,n}| [GeV/c]",
                              numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_1n_Dir = directories.TKI_Directory_map["dP_T_1n_Directory"];

    THStack *sdAlpha_T_1n = new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1n)",
                                        "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1n);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_1n = new TH1D("#delta#alpha_{T} (1n)", "#delta#alpha_{T} histogram (1n);#delta#alpha_{T} [Deg]",
                                  numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_1n_Dir = directories.TKI_Directory_map["dAlpha_T_1n_Directory"];

    THStack *sdPhi_T_1n = new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1n)",
                                      "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1n);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_1n = new TH1D("#delta#phi_{T} (1n)", "#delta#phi_{T} histogram (1n);#delta#phi_{T} [Deg]",
                                numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_1n_Dir = directories.TKI_Directory_map["dPhi_T_1n_Directory"];

    TH2D *hdP_T_vs_dAlpha_T_1n = new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n)",
                                          "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [Deg];#deltaP_{T} [GeV/c];",
                                          numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_vs_dAlpha_T_1n_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI histograms (2p)">
    THStack *sdP_T_2p = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (2p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (2p);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_2p = new TH1D("#deltaP_{T,L} (2p)",
                                "#deltaP_{T,L} by Leading Proton (2p);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,1}| [GeV/c]",
                                numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    TH1D *hdP_T_tot_2p = new TH1D("#deltaP_{T,tot} (2p)",
                                  "#deltaP_{T,tot} by Momentum Sum (2p);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,1} + #vec{p}_{T,2}| [GeV/c]",
                                  numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_L_2p_Dir = directories.TKI_Directory_map["dP_T_2p_Directory"];
    string hdP_T_tot_2p_Dir = directories.TKI_Directory_map["dP_T_2p_Directory"];

    THStack *sdAlpha_T_2p = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (2p)",
                                        "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (2p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_2p = new TH1D("#delta#alpha_{T,L} (2p)", "#delta#alpha_{T,L} by Leading Proton (2p);#delta#alpha_{T,L} [Deg]",
                                    numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    TH1D *hdAlpha_T_tot_2p = new TH1D("#delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,tot} by Momentum Sum (2p);#delta#alpha_{T,tot} [Deg]",
                                      numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_L_2p_Dir = directories.TKI_Directory_map["dAlpha_T_2p_Directory"];
    string hdAlpha_T_tot_2p_Dir = directories.TKI_Directory_map["dAlpha_T_2p_Directory"];

    THStack *sdPhi_T_2p = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (2p)",
                                      "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (2p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_2p = new TH1D("#delta#phi_{T,L} (2p)", "#delta#phi_{T,L} by Leading Proton (2p);#delta#phi_{T,L} [Deg]",
                                  numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    TH1D *hdPhi_T_tot_2p = new TH1D("#delta#phi_{T,tot} (2p)", "#delta#phi_{T,tot} by Momentum Sum (2p);#delta#phi_{T,tot} [Deg]",
                                    numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_L_2p_Dir = directories.TKI_Directory_map["dPhi_T_2p_Directory"];
    string hdPhi_T_tot_2p_Dir = directories.TKI_Directory_map["dPhi_T_2p_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_2p = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                              "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                              numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_2p = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                                  "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                  numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_2p_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_2p_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI histograms (pFDpCD)">

    //<editor-fold desc="TKI plots (pFDpCD)">
    THStack *sdP_T_pFDpCD = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (pFDpCD)",
                                        "#deltaP_{T,L} vs. #deltaP_{T,tot} (pFDpCD);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_pFDpCD = new TH1D("#deltaP_{T,L} (pFDpCD)",
                                    "#deltaP_{T,L} by leading proton (pFDpCD);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,pL}| [GeV/c]",
                                    numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    TH1D *hdP_T_tot_pFDpCD = new TH1D("#deltaP_{T,tot} (pFDpCD)",
                                      "#deltaP_{T,tot} by Momentum Sum (pFDpCD);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}| [GeV/c]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];
    string hdP_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];

    THStack *sdAlpha_T_pFDpCD = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (pFDpCD)",
                                            "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (pFDpCD);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_pFDpCD = new TH1D("#delta#alpha_{T,L} (pFDpCD)", "#delta#alpha_{T,L} by leading proton (pFDpCD);#delta#alpha_{T,L} [Deg]",
                                        numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    TH1D *hdAlpha_T_tot_pFDpCD = new TH1D("#delta#alpha_{T,tot} (pFDpCD)",
                                          "#delta#alpha_{T,tot} by Momentum Sum (pFDpCD);#delta#alpha_{T,tot} [Deg]",
                                          numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];
    string hdAlpha_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];

    THStack *sdPhi_T_pFDpCD = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (pFDpCD)",
                                          "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (pFDpCD);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_pFDpCD = new TH1D("#delta#phi_{T,L} (pFDpCD)", "#delta#phi_{T,L} by leading proton (pFDpCD);#delta#phi_{T,L} [Deg]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    TH1D *hdPhi_T_tot_pFDpCD = new TH1D("#delta#phi_{T,tot} (pFDpCD)", "#delta#phi_{T,tot} by Momentum Sum (pFDpCD);#delta#phi_{T,tot} [Deg]",
                                        numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_pFDpCD_Directory"];
    string hdPhi_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_pFDpCD_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_pFDpCD = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., pFDpCD)",
                                                  "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., pFDpCD);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                                  numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_pFDpCD = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
                                                      "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                      numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_pFDpCD_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI vs. W (pFDpCD)">
    TH2D *hdP_T_L_vs_W_pFDpCD = new TH2D("#deltaP_{T,L} vs. W (All Int., pFDpCD)",
                                         "#deltaP_{T,L} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                         "#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,pL}| [GeV/c];",
                                         numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_W_pFDpCD = new TH2D("#deltaP_{T,tot} vs. W (All Int., pFDpCD)",
                                           "#deltaP_{T,tot} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                           "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}| [GeV/c];",
                                           numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];
    string hdP_T_tot_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];

    TH2D *hdAlpha_T_L_vs_W_pFDpCD = new TH2D("#delta#alpha_{T,L} vs. W (All Int., pFDpCD)",
                                             "#delta#alpha_{T,L} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,L} [Deg];",
                                             numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    TH2D *hdAlpha_T_tot_vs_W_pFDpCD = new TH2D("#delta#alpha_{T,tot} vs. W (All Int., pFDpCD)",
                                               "#delta#alpha_{T,tot} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,tot} [Deg];",
                                               numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    string hdAlpha_T_L_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];
    string hdAlpha_T_tot_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="TKI histograms (nFDpCD)">

    //<editor-fold desc="TKI plots (nFDpCD)">
    THStack *sdP_T_nFDpCD = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (nFDpCD)",
                                        "#deltaP_{T,L} vs. #deltaP_{T,tot} (nFDpCD);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_nFDpCD = new TH1D("#deltaP_{T,L} (nFDpCD)",
                                    "#deltaP_{T,L} by leading nucleon (nFDpCD);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,nL}| [GeV/c]",
                                    numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    TH1D *hdP_T_tot_nFDpCD = new TH1D("#deltaP_{T,tot} (nFDpCD)",
                                      "#deltaP_{T,tot} by Momentum Sum (nFDpCD);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}| [GeV/c]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];
    string hdP_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];

    THStack *sdAlpha_T_nFDpCD = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (nFDpCD)",
                                            "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (nFDpCD);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_nFDpCD = new TH1D("#delta#alpha_{T,L} (nFDpCD)", "#delta#alpha_{T,L} by leading nucleon (nFDpCD);#delta#alpha_{T,L} [Deg]",
                                        numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    TH1D *hdAlpha_T_tot_nFDpCD = new TH1D("#delta#alpha_{T,tot} (nFDpCD)",
                                          "#delta#alpha_{T,tot} by Momentum Sum (nFDpCD);#delta#alpha_{T,tot} [Deg]",
                                          numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];
    string hdAlpha_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];

    THStack *sdPhi_T_nFDpCD = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (nFDpCD)",
                                          "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (nFDpCD);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_nFDpCD = new TH1D("#delta#phi_{T,L} (nFDpCD)", "#delta#phi_{T,L} by leading nucleon (nFDpCD);#delta#phi_{T,L} [Deg]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    TH1D *hdPhi_T_tot_nFDpCD = new TH1D("#delta#phi_{T,tot} (nFDpCD)", "#delta#phi_{T,tot} by Momentum Sum (nFDpCD);#delta#phi_{T,tot} [Deg]",
                                        numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_nFDpCD_Directory"];
    string hdPhi_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_nFDpCD_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_nFDpCD = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., nFDpCD)",
                                                  "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., nFDpCD);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                                  numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_nFDpCD = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
                                                      "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                      numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_nFDpCD_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI vs. W (nFDpCD)">
    TH2D *hdP_T_L_vs_W_nFDpCD = new TH2D("#deltaP_{T,L} vs. W (All Int., nFDpCD)",
                                         "#deltaP_{T,L} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                         "#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,nL}| [GeV/c];",
                                         numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_W_nFDpCD = new TH2D("#deltaP_{T,tot} vs. W (All Int., nFDpCD)",
                                           "#deltaP_{T,tot} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                           "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}| [GeV/c];",
                                           numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];
    string hdP_T_tot_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];

    TH2D *hdAlpha_T_L_vs_W_nFDpCD = new TH2D("#delta#alpha_{T,L} vs. W (All Int., nFDpCD)",
                                             "#delta#alpha_{T,L} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,L} [Deg];",
                                             numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    TH2D *hdAlpha_T_tot_vs_W_nFDpCD = new TH2D("#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)",
                                               "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,tot} [Deg];",
                                               numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    string hdAlpha_T_L_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];
    string hdAlpha_T_tot_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Efficiency histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Efficiency histograms">

    //<editor-fold desc="Efficiency plots (1e cut)">

    //<editor-fold desc="Truth level momentum plots (1e cut)">
    hPlot1D hP_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "01_P_e_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "01_P_e_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "02_P_n_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "02_P_n_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "03_P_p_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "03_P_p_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                             "04_P_piplus_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                             "04_P_piplus_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                             "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                             "05_P_piminus_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                             "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                             "05_P_piminus_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC",
                                             "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                             "06_P_pi0_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC",
                                             "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                             "06_P_pi0_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                            "07_P_ph_AC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                            "07_P_ph_BC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_Kp_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL K^{+} momentum AC", "K^{+} momentum P^{truth}_{K^{+}} AC",
                                            "P^{truth}_{K^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                            "04_P_Kplus_AC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_Kp_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL K^{+} momentum BC", "K^{+} momentum P^{truth}_{K^{+}} BC",
                                            "P^{truth}_{K^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"],
                                            "04_P_Kplus_BC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL momentum plots for momentum thresholds (1e cut)">
    hPlot1D hP_e_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD Electron momentum", "FD Electron momentum P^{truth}_{e}",
                                           "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "01a_P_e_truth_1e_cut_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD Electron momentum - ZOOMIN",
                                                  "FD Electron momentum P^{truth}_{e} - ZOOMIN",
                                                  "P^{truth}_{e} [GeV/c]",
                                                  directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                  "01a_P_e_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD Proton momentum", "FD Proton momentum P^{truth}_{p}", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "02a_P_p_truth_1e_cut_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_truth_1e_cut_CD = hPlot1D("1e cut", "CD", "TL CD Proton momentum", "CD Proton momentum P^{truth}_{p}", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "02b_P_p_truth_1e_cut_CD",
                                           CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD Proton momentum - ZOOMIN", "FD Proton momentum P^{truth}_{p} - ZOOMIN",
                                                  "P^{truth}_{p} [GeV/c]",
                                                  directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                  "02a_P_p_truth_1e_cut_FD_ZOOMIN",
                                                  0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_truth_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "TL CD Proton momentum - ZOOMIN", "CD Proton momentum P^{truth}_{p} - ZOOMIN",
                                                  "P^{truth}_{p} [GeV/c]",
                                                  directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                  "02b_P_p_truth_1e_cut_CD_ZOOMIN",
                                                  0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_LnFD_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL leading FD neutron momentum", "Leading FD neutron momentum P^{truth}_{n}",
                                              "P^{truth}_{n} [GeV/c]",
                                              directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                              "03a_P_LnFD_truth_1e_cut_FD",
                                              Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots); // leading nFD!
    hPlot1D hP_LnFD_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL leading FD neutron momentum - ZOOMIN",
                                                     "Leading FD neutron momentum P^{truth}_{n} - ZOOMIN",
                                                     "P^{truth}_{n} [GeV/c]",
                                                     directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                     "03a_P_LnFD_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots); // leading nFD!
    hPlot1D hP_nFD_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD neutrons momentum", "FD neutrons momentum P^{truth}_{n}",
                                             "P^{truth}_{n} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                             "03b_P_nFD_truth_1e_cut_FD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots); // all nFD!
    hPlot1D hP_nFD_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD neutrons momentum - ZOOMIN",
                                                    "FD neutrons momentum P^{truth}_{n} - ZOOMIN",
                                                    "P^{truth}_{n} [GeV/c]",
                                                    directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                    "03b_P_nFD_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots); // all nFD!

    hPlot1D hP_piplus_truth_1e_cut = hPlot1D("1e cut", "CD & FD", "TL #pi^{+} momentum", "#pi^{+} momentum P^{truth}_{#pi^{+}}",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                             "04_P_piplus_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "TL #pi^{+} momentum - ZOOMIN",
                                                    "#pi^{+} momentum P^{truth}_{#pi^{+}} - ZOOMIN",
                                                    "P^{truth}_{#pi^{+}} [GeV/c]",
                                                    directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                    "04_P_piplus_truth_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD #pi^{+} momentum", "FD #pi^{+} momentum P^{truth}_{#pi^{+}}",
                                                "P^{truth}_{#pi^{+}} [GeV/c]",
                                                directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                "04a_P_piplus_truth_1e_cut_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_CD = hPlot1D("1e cut", "CD", "TL CD #pi^{+} momentum", "CD #pi^{+} momentum P^{truth}_{#pi^{+}}",
                                                "P^{truth}_{#pi^{+}} [GeV/c]",
                                                directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                "04b_P_piplus_truth_1e_cut_CD",
                                                CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD #pi^{+} momentum - ZOOMIN",
                                                       "FD #pi^{+} momentum P^{truth}_{#pi^{+}} - ZOOMIN",
                                                       "P^{truth}_{#pi^{+}} [GeV/c]",
                                                       directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                       "04a_P_piplus_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "TL CD #pi^{+} momentum - ZOOMIN",
                                                       "CD #pi^{+} momentum P^{truth}_{#pi^{+}} - ZOOMIN",
                                                       "P^{truth}_{#pi^{+}} [GeV/c]",
                                                       directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                       "04b_P_piplus_truth_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_piminus_truth_1e_cut = hPlot1D("1e cut", "CD & FD", "TL #pi^{-} momentum", "#pi^{-} momentum P^{truth}_{#pi^{-}}",
                                              "P^{truth}_{#pi^{-}} [GeV/c]",
                                              directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                              "05_P_piminus_truth_1e_cut",
                                              Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "TL #pi^{-} momentum - ZOOMIN",
                                                     "#pi^{-} momentum P^{truth}_{#pi^{-}} - ZOOMIN",
                                                     "P^{truth}_{#pi^{-}} [GeV/c]",
                                                     directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                     "05_P_piminus_truth_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD #pi^{-} momentum", "FD #pi^{-} momentum P^{truth}_{#pi^{-}}",
                                                 "P^{truth}_{#pi^{-}} [GeV/c]",
                                                 directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                 "05a_P_piminus_truth_1e_cut_FD",
                                                 Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_CD = hPlot1D("1e cut", "CD", "TL CD #pi^{-} momentum", "CD #pi^{-} momentum P^{truth}_{#pi^{-}}",
                                                 "P^{truth}_{#pi^{-}} [GeV/c]",
                                                 directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                 "05b_P_piminus_truth_1e_cut_CD",
                                                 CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD #pi^{-} momentum - ZOOMIN",
                                                        "FD #pi^{-} momentum P^{truth}_{#pi^{-}} - ZOOMIN",
                                                        "P^{truth}_{#pi^{-}} [GeV/c]",
                                                        directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                        "05a_P_piminus_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "TL CD #pi^{-} momentum - ZOOMIN",
                                                        "CD #pi^{-} momentum P^{truth}_{#pi^{-}} - ZOOMIN",
                                                        "P^{truth}_{#pi^{-}} [GeV/c]",
                                                        directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                        "05b_P_piminus_truth_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD #gamma momentum", "FD #gamma momentum P^{truth}_{#gamma}",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "09a_P_ph_truth_1e_cut_FD",
                                            CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD #gamma momentum - ZOOMIN",
                                                   "FD #gamma momentum P^{truth}_{#gamma} - ZOOMIN",
                                                   "P^{truth}_{#gamma} [GeV/c]",
                                                   directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                   "09a_P_ph_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1e cut)">
    hPlot1D hTheta_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC",
                                               "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                               "01_Theta_e_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC",
                                               "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                               "01_Theta_e_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC",
                                               "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                               "02_Theta_n_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC",
                                               "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                               "02_Theta_n_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC",
                                               "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                               "03_Theta_p_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC",
                                               "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                               "03_Theta_p_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "04_Theta_piplus_AC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "04_Theta_piplus_BC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "05_Theta_piminus_AC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "05_Theta_piminus_BC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "06_Theta_pi0_AC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "06_Theta_pi0_BC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                "07_Theta_ph_AC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                "07_Theta_ph_BC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1e cut)">
    hPlot1D hPhi_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC",
                                             "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                             "01_Phi_e_AC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC",
                                             "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                             "01_Phi_e_BC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC",
                                             "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                             "02_Phi_n_AC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC",
                                             "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                             "02_Phi_n_BC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC",
                                             "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                             "03_Phi_p_AC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC",
                                             "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                             "03_Phi_p_BC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "04_Phi_piplus_AC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "04_Phi_piplus_BC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "05_Phi_piminus_AC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "05_Phi_piminus_BC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "06_Phi_pi0_AC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "06_Phi_pi0_BC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                              "07_Phi_ph_AC_truth_1e_cut",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                              "07_Phi_ph_BC_truth_1e_cut",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (1e cut)">
    hPlot2D hTheta_e_vs_Phi_e_truth_1e_cut = hPlot2D("1e cut", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                     "#theta_{e} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"],
                                                     "01_Theta_e_vs_Phi_e_truth_1e_cut",
                                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_truth_1e_cut = hPlot2D("1e cut", "FD", "TL #theta_{nFD} vs. #phi_{nFD}", "TL #theta_{nFD} vs. #phi_{nFD}",
                                                         "#phi_{nFD} [Deg]",
                                                         "#theta_{nFD} [Deg]",
                                                         directories.Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"],
                                                         "02_Theta_nFD_vs_Phi_nFD_truth_1e_cut", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_truth_1e_cut = hPlot2D("1e cut", "FD", "TL #theta_{pFD} vs. #phi_{pFD}", "TL #theta_{pFD} vs. #phi_{pFD}",
                                                         "#phi_{pFD} [Deg]",
                                                         "#theta_{pFD} [Deg]",
                                                         directories.Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"],
                                                         "03_Theta_pFD_vs_Phi_pFD_truth_1e_cut", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (1p)">

    //<editor-fold desc="Truth level momentum plots (1p)">
    hPlot1D hP_e_AC_truth_1p = hPlot1D("1p", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "01_P_e_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_1p = hPlot1D("1p", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "01_P_e_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_1p = hPlot1D("1p", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "02_P_n_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_1p = hPlot1D("1p", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "02_P_n_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_1p = hPlot1D("1p", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03a_P_p_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1p = hPlot1D("1p", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03a_P_p_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_AC_truth_1p = hPlot1D("1p", "FD", "FD TL Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03b_P_pFD_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_BC_truth_1p = hPlot1D("1p", "FD", "FD TL Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03b_P_pFD_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                         "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "04_P_piplus_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                         "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "04_P_piplus_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                         "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "05_P_piminus_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                         "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "05_P_piminus_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC",
                                         "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "06_P_pi0_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC",
                                         "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "06_P_pi0_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_1p = hPlot1D("1p", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07a_P_ph_AC_truth_1p",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1p = hPlot1D("1p", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07a_P_ph_BC_truth_1p",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC",
                                           "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07b_P_ph_AC_truth_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC",
                                           "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07b_P_ph_BC_truth_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1p)">
    hPlot1D hTheta_e_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC",
                                           "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "01_Theta_e_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC",
                                           "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "01_Theta_e_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC",
                                           "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "02_Theta_n_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC",
                                           "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "02_Theta_n_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC",
                                           "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "03a_Theta_p_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC",
                                           "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "03a_Theta_p_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_AC_truth_1p = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing FD Proton AC",
                                             "#theta^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "03b_Theta_pFD_AC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_BC_truth_1p = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing FD Proton BC",
                                             "#theta^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "03b_Theta_pFD_BC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "04_Theta_piplus_AC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "04_Theta_piplus_BC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "05_Theta_piminus_AC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "05_Theta_piminus_BC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "06_Theta_pi0_AC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "06_Theta_pi0_BC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                            "07a_Theta_ph_AC_truth_1p",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                            "07a_Theta_ph_BC_truth_1p",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                               "07b_Theta_ph_AC_truth_1p_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                               "07b_Theta_ph_BC_truth_1p_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1p)">
    hPlot1D hPhi_e_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "01_Phi_e_AC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "01_Phi_e_BC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "02_Phi_n_AC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "02_Phi_n_BC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03a_Phi_p_AC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03a_Phi_p_BC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_AC_truth_1p = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of FD Outgoing Proton AC",
                                           "#phi^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03b_Phi_pFD_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_BC_truth_1p = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of FD Outgoing Proton BC",
                                           "#phi^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03b_Phi_pFD_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                           "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "04_Phi_piplus_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                           "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "04_Phi_piplus_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                           "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "05_Phi_piminus_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                           "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "05_Phi_piminus_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                           "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "06_Phi_pi0_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                           "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "06_Phi_pi0_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                          "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "07a_Phi_ph_AC_truth_1p",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                          "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "07a_Phi_ph_BC_truth_1p",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                             "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"],
                                             "07b_Phi_ph_AC_truth_1p_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                             "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"],
                                             "07b_Phi_ph_BC_truth_1p_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (1p)">
    hPlot2D hpFD_Hit_map_1p_BEC = hPlot2D("1p", "FD", "FD proton hit map BEC", "FD proton hit map BEC", "x_{pFD}", "y_{pFD}",
                                          directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                          "01a_Proton_FD_Hit_map_BEC_1p", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hpFD_Hit_map_1p_AEC = hPlot2D("1p", "FD", "FD proton hit map AEC", "FD proton hit map AEC", "x_{pFD}", "y_{pFD}",
                                          directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                          "01b_Proton_FD_Hit_map_AEC_1p", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_1p_BEC = hPlot2D("1p", "FD", "#theta_{pFD} vs. #phi_{pFD} BEC", "TL #theta_{pFD} vs. #phi_{pFD} BEC",
                                                   "#phi_{pFD} [Deg]",
                                                   "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                   "02a_Theta_pFD_vs_Phi_pFD_BEC_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                   Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_pFD_vs_Phi_pFD_1p_AEC = hPlot2D("1p", "FD", "#theta_{pFD} vs. #phi_{pFD} AEC", "TL #theta_{pFD} vs. #phi_{pFD} AEC",
                                                   "#phi_{pFD} [Deg]",
                                                   "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                   "02b_Theta_pFD_vs_Phi_pFD_AEC_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                   Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (1p)">
    hPlot2D hTheta_e_vs_Phi_e_truth_1p = hPlot2D("1p", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                 "#theta_{e} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                 "01_Theta_e_vs_Phi_e_truth_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                 Theta_uboundary_FD,
                                                 numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_truth_1p = hPlot2D("1p", "FD", "TL #theta_{pFD} vs. #phi_{pFD}", "TL #theta_{pFD} vs. #phi_{pFD}",
                                                     "#phi_{pFD} [Deg]",
                                                     "#theta_{pFD} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                     "02_Theta_pFD_vs_Phi_pFD_truth_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                     Theta_uboundary_FD,
                                                     numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (1n)">

    //<editor-fold desc="Truth level momentum plots (1n)">
    hPlot1D hP_e_AC_truth_1n = hPlot1D("1n", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "01_P_e_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_1n = hPlot1D("1n", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "01_P_e_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_1n = hPlot1D("1n", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02a_P_n_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_1n = hPlot1D("1n", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02a_P_n_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_AC_truth_1n = hPlot1D("1n", "FD", "FD TL Neutron momentum AC", "FD Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02b_P_nFD_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_BC_truth_1n = hPlot1D("1n", "FD", "CD TL Neutron momentum BC", "FD Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02b_P_nFD_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_1n = hPlot1D("1n", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03_P_p_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n = hPlot1D("1n", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03_P_p_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03FD_P_p_AC_truth_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03FD_P_p_BC_truth_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD Proton momentum AC", "CD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03CD_P_p_AC_truth_1n_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD Proton momentum BC", "CD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03CD_P_p_BC_truth_1n_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet Proton momentum AC", "undet Proton momentum P^{truth}_{p} AC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                             "03undet_P_p_AC_truth_1n_undet",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet Proton momentum BC", "undet Proton momentum P^{truth}_{p} BC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                             "03undet_P_p_BC_truth_1n_undet",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                         "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04_P_piplus_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                         "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04_P_piplus_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{+} momentum AC", "FD #pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                            "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04FD_P_pip_AC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{+} momentum BC", "FD #pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                            "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04FD_P_pip_BC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{+} momentum AC", "CD #pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                            "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04CD_P_pip_AC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{+} momentum BC", "CD #pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                            "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04CD_P_pip_BC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{+} momentum AC", "undet #pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                               "P^{truth}_{#pi^{+}} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "04undet_P_pip_AC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary,
                                               numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{+} momentum BC", "undet #pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                               "P^{truth}_{#pi^{+}} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "04undet_P_pip_BC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary,
                                               numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                         "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05_P_piminus_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                         "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05_P_piminus_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{-} momentum AC", "FD #pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                            "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05FD_P_pim_AC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{-} momentum BC", "FD #pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                            "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05FD_P_pim_BC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{-} momentum AC", "CD #pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                            "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05CD_P_pim_AC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{-} momentum BC", "CD #pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                            "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05CD_P_pim_BC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{-} momentum AC", "undet #pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                               "P^{truth}_{#pi^{-}} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "05undet_P_pim_AC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary,
                                               numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{-} momentum BC", "undet #pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                               "P^{truth}_{#pi^{-}} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "05undet_P_pim_BC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary,
                                               numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC",
                                         "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "06_P_pi0_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC",
                                         "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "06_P_pi0_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_1n = hPlot1D("1n", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07a_P_ph_AC_truth_1n",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1n = hPlot1D("1n", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07a_P_ph_BC_truth_1n",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC",
                                           "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07b_P_ph_AC_truth_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC",
                                           "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07b_P_ph_BC_truth_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1n)">
    hPlot1D hTheta_e_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC",
                                           "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "01_Theta_e_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC",
                                           "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "01_Theta_e_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC",
                                           "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "02a_Theta_n_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC",
                                           "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "02a_Theta_n_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_AC_truth_1n = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing FD Neutron AC",
                                             "#theta^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "02b_Theta_nFD_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_BC_truth_1n = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing FD Neutron BC",
                                             "#theta^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "02b_Theta_nFD_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC",
                                           "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03_Theta_p_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC",
                                           "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03_Theta_p_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{p} AC", "FD #theta^{truth}_{p} of Outgoing Proton AC",
                                              "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                              "03FD_Theta_p_AC_truth_1n_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{p} BC", "FD #theta^{truth}_{p} of Outgoing Proton BC",
                                              "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                              "03FD_Theta_p_BC_truth_1n_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{p} AC", "CD #theta^{truth}_{p} of Outgoing Proton AC",
                                              "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                              "03CD_Theta_p_AC_truth_1n_CD",
                                              Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{p} BC", "CD #theta^{truth}_{p} of Outgoing Proton BC",
                                              "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                              "03CD_Theta_p_BC_truth_1n_CD",
                                              Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{p} AC", "undet #theta^{truth}_{p} of Outgoing Proton AC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                 "03undet_Theta_p_AC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{p} BC", "undet #theta^{truth}_{p} of Outgoing Proton BC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                 "03undet_Theta_p_BC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "04_Theta_piplus_AC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "04_Theta_piplus_BC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{+}} AC",
                                                "FD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04FD_Theta_piplus_AC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{+}} BC",
                                                "FD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04FD_Theta_piplus_BC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{+}} AC",
                                                "CD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04CD_Theta_piplus_AC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{+}} BC",
                                                "CD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04CD_Theta_piplus_BC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{+}} AC",
                                                   "undet #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                   "#theta^{truth}_{#pi^{+}} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "04undet_Theta_piplus_AC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{+}} BC",
                                                   "undet #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                   "#theta^{truth}_{#pi^{+}} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "04undet_Theta_piplus_BC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "05_Theta_piminus_AC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "05_Theta_piminus_BC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{-}} AC",
                                                "FD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05FD_Theta_piminus_AC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{-}} BC",
                                                "FD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05FD_Theta_piminus_BC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{-}} AC",
                                                "CD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05CD_Theta_piminus_AC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{-}} BC",
                                                "CD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05CD_Theta_piminus_BC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{-}} AC",
                                                   "undet #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                   "#theta^{truth}_{#pi^{-}} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "05undet_Theta_piminus_AC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{-}} BC",
                                                   "undet #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                   "#theta^{truth}_{#pi^{-}} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "05undet_Theta_piminus_BC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "06_Theta_pi0_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "06_Theta_pi0_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                            "07a_Theta_ph_AC_truth_1n",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                            "07a_Theta_ph_BC_truth_1n",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                               "07b_Theta_ph_AC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                               "#theta^{truth}_{#gamma} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                               "07b_Theta_ph_BC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1n)">
    hPlot1D hPhi_e_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "01_Phi_e_AC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "01_Phi_e_BC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02a_Phi_n_AC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02a_Phi_n_BC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_AC_truth_1n = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing FD Neutron AC",
                                           "#phi^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02b_Phi_nFD_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_BC_truth_1n = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing FD Neutron BC",
                                           "#phi^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02b_Phi_nFD_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03_Phi_p_AC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03_Phi_p_BC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{p} AC", "FD #phi^{truth}_{p} of Outgoing Proton AC",
                                            "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03FD_Phi_p_AC_truth_1n_FD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{p} BC", "FD #phi^{truth}_{p} of Outgoing Proton BC",
                                            "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03FD_Phi_p_BC_truth_1n_FD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{p} AC", "CD #phi^{truth}_{p} of Outgoing Proton AC",
                                            "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03CD_Phi_p_AC_truth_1n_CD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{p} BC", "CD #phi^{truth}_{p} of Outgoing Proton BC",
                                            "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03CD_Phi_p_BC_truth_1n_CD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{p} AC", "undet #phi^{truth}_{p} of Outgoing Proton AC",
                                               "#phi^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                               "03undet_Phi_p_AC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{p} BC", "undet #phi^{truth}_{p} of Outgoing Proton BC",
                                               "#phi^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                               "03undet_Phi_p_BC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                           "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "04_Phi_piplus_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                           "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "04_Phi_piplus_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{+}} AC", "FD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04FD_Phi_piplus_AC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{+}} BC", "FD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04FD_Phi_piplus_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{+}} AC", "CD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04CD_Phi_piplus_AC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{+}} BC", "CD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04CD_Phi_piplus_BC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{+}} AC",
                                                 "undet #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#phi^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "04undet_Phi_piplus_AC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{+}} BC",
                                                 "undet #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#phi^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "04undet_Phi_piplus_BC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                           "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "05_Phi_piminus_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                           "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "05_Phi_piminus_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{-}} AC", "FD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05FD_Phi_piminus_AC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{-}} BC", "FD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05FD_Phi_piminus_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{-}} AC", "CD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05CD_Phi_piminus_AC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{-}} BC", "CD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05CD_Phi_piminus_BC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{-}} AC",
                                                 "undet #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#phi^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "05undet_Phi_piminus_AC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{-}} BC",
                                                 "undet #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#phi^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "05undet_Phi_piminus_BC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                           "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "06_Phi_pi0_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                           "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "06_Phi_pi0_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                          "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "07a_Phi_ph_AC_truth_1n",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                          "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "07a_Phi_hp_BC_truth_1n",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                             "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                             "07b_Phi_ph_AC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                             "#phi^{truth}_{#gamma} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                             "07b_Phi_hp_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (1n)">
    hPlot2D hnFD_Hit_map_1n_BEC = hPlot2D("1n", "FD", "FD neutron hit map BEC", "FD neutron hit map BEC", "x_{nFD}", "y_{nFD}",
                                          directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                          "01a_Neutron_FD_Hit_map_BEC_1n", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hnFD_Hit_map_1n_AEC = hPlot2D("1n", "FD", "FD neutron hit map AEC", "FD neutron hit map AEC", "x_{nFD}", "y_{nFD}",
                                          directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                          "01b_Neutron_FD_Hit_map_AEC_1n", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_1n_BEC = hPlot2D("1n", "FD", "#theta_{nFD} vs. #phi_{nFD} BEC", "TL #theta_{nFD} vs. #phi_{nFD} BEC",
                                                   "#phi_{nFD} [Deg]",
                                                   "#theta_{nFD} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                   "02a_Theta_nFD_vs_Phi_nFD_BEC_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                   Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_nFD_vs_Phi_nFD_1n_AEC = hPlot2D("1n", "FD", "#theta_{nFD} vs. #phi_{nFD} AEC", "TL #theta_{nFD} vs. #phi_{nFD} AEC",
                                                   "#phi_{nFD} [Deg]",
                                                   "#theta_{nFD} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                   "02b_Theta_nFD_vs_Phi_nFD_AEC_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                   Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (1n)">
    hPlot2D hTheta_e_vs_Phi_e_truth_1n = hPlot2D("1n", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                 "#theta_{e} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                 "01_Theta_e_vs_Phi_e_truth_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                 Theta_uboundary_FD,
                                                 numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_truth_1n = hPlot2D("1n", "FD", "TL #theta_{nFD} vs. #phi_{nFD}", "TL #theta_{nFD} vs. #phi_{nFD}",
                                                     "#phi_{nFD} [Deg]",
                                                     "#theta_{nFD} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                     "02_Theta_nFD_vs_Phi_nFD_truth_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                     Theta_uboundary_FD,
                                                     numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (pFDpCD)">

    //<editor-fold desc="Truth level momentum plots (pFDpCD)">
    hPlot1D hP_e_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "01_P_e_AC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "01_P_e_BC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "02_P_n_AC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "02_P_n_BC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                           "03a_P_p_AC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                           "03a_P_p_BC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "03b_P_pFD_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "03b_P_pFD_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL Proton momentum AC", "CD Proton momentum P^{truth}_{p} AC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "03b_P_pCD_AC_truth_pFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL Proton momentum BC", "CD Proton momentum P^{truth}_{p} BC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "03b_P_pCD_BC_truth_pFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "04_P_piplus_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "04_P_piplus_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                             "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "05_P_piminus_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                             "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "05_P_piminus_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC",
                                             "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "06_P_pi0_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC",
                                             "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                             "06_P_pi0_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                            "07a_P_ph_AC_truth_pFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                            "07a_P_ph_BC_truth_pFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC",
                                               "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                               "07b_P_ph_AC_truth_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC",
                                               "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"],
                                               "07b_P_ph_BC_truth_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (pFDpCD)">
    hPlot1D hTheta_e_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC",
                                               "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                               "01_Theta_e_AC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC",
                                               "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                               "01_Theta_e_BC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC",
                                               "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                               "02_Theta_n_AC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC",
                                               "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                               "02_Theta_n_BC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC",
                                               "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                               "03a_Theta_p_AC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC",
                                               "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                               "03a_Theta_p_BC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing FD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pFD_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing FD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pFD_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing CD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pCD_AC_truth_pFDpCD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing CD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pCD_BC_truth_pFDpCD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "04_Theta_piplus_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "04_Theta_piplus_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "05_Theta_piminus_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "05_Theta_piminus_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "06_Theta_pi0_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "06_Theta_pi0_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                "07a_Theta_ph_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                "07a_Theta_ph_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} AC",
                                                   "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                   "#theta^{truth}_{#gamma} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                   "07b_Theta_ph_AC_truth_pFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} BC",
                                                   "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                   "#theta^{truth}_{#gamma} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                   "07b_Theta_ph_BC_truth_pFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (pFDpCD)">
    hPlot1D hPhi_e_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC",
                                             "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                             "01_Phi_e_AC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC",
                                             "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                             "01_Phi_e_BC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC",
                                             "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                             "02_Phi_n_AC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC",
                                             "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                             "02_Phi_n_BC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC",
                                             "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                             "03a_Phi_p_AC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC",
                                             "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                             "03a_Phi_p_BC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of FD Outgoing Proton AC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "03b_Phi_pFD_AC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of FD Outgoing Proton BC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "03b_Phi_pFD_BC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of CD Outgoing Proton AC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "03b_Phi_pCD_AC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of CD Outgoing Proton BC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "03b_Phi_pCD_BC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "04_Phi_piplus_AC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "04_Phi_piplus_BC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "05_Phi_piminus_AC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "05_Phi_piminus_BC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "06_Phi_pi0_AC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "06_Phi_pi0_BC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                              "07a_Phi_ph_AC_truth_pFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                              "07a_Phi_ph_BC_truth_pFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} AC",
                                                 "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                 "#phi^{truth}_{#gamma} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "07b_Phi_ph_AC_truth_pFDpCD_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} BC",
                                                 "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                 "#phi^{truth}_{#gamma} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "07b_Phi_ph_BC_truth_pFDpCD_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (pFDpCD)">
    hPlot2D hpFD_Hit_map_pFDpCD_BEC = hPlot2D("pFDpCD", "FD", "FD proton hit map BEC", "FD proton hit map BEC", "x_{pFD}", "y_{pFD}",
                                              directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                              "01a_Proton_FD_Hit_map_BEC_pFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hpFD_Hit_map_pFDpCD_AEC = hPlot2D("pFDpCD", "FD", "FD proton hit map AEC", "FD proton hit map AEC", "x_{pFD}", "y_{pFD}",
                                              directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                              "01b_Proton_FD_Hit_map_AEC_pFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_pFDpCD_BEC = hPlot2D("pFDpCD", "FD", "#theta_{pFD} vs. #phi_{pFD} BEC", "TL #theta_{pFD} vs. #phi_{pFD} BEC",
                                                       "#phi_{pFD} [Deg]",
                                                       "#theta_{pFD} [Deg]",
                                                       directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                       "02a_Theta_pFD_vs_Phi_pFD_BEC_pFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                       Theta_uboundary_FD,
                                                       numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_pFD_vs_Phi_pFD_pFDpCD_AEC = hPlot2D("pFDpCD", "FD", "#theta_{pFD} vs. #phi_{pFD} AEC", "TL #theta_{pFD} vs. #phi_{pFD} AEC",
                                                       "#phi_{pFD} [Deg]",
                                                       "#theta_{pFD} [Deg]",
                                                       directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                       "02b_Theta_pFD_vs_Phi_pFD_AEC_pFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                       Theta_uboundary_FD,
                                                       numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (pFDpCD)">
    hPlot2D hTheta_e_vs_Phi_e_truth_pFDpCD = hPlot2D("pFDpCD", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                     "#theta_{e} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                     "01_Theta_e_vs_Phi_e_truth_pFDpCD",
                                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_truth_pFDpCD = hPlot2D("pFDpCD", "FD", "TL #theta_{pFD} vs. #phi_{pFD}", "TL #theta_{pFD} vs. #phi_{pFD}",
                                                         "#phi_{pFD} [Deg]",
                                                         "#theta_{pFD} [Deg]",
                                                         directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                         "02_Theta_pFD_vs_Phi_pFD_truth_pFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (nFDpCD)">

    //<editor-fold desc="Truth level momentum plots (nFDpCD)">
    hPlot1D hP_e_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "01_P_e_AC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "01_P_e_BC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                           "02a_P_n_AC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                           "02a_P_n_BC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Neutron momentum AC", "FD Neutron momentum P^{truth}_{n} AC",
                                             "P^{truth}_{n} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02b_P_nFD_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Neutron momentum BC", "FD Neutron momentum P^{truth}_{n} BC",
                                             "P^{truth}_{n} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02b_P_nFD_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "03_P_p_AC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "03_P_p_BC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02b_P_pFD_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02b_P_pFD_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL Proton momentum AC", "CD Proton momentum P^{truth}_{p} AC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02b_P_pCD_AC_truth_nFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL Proton momentum BC", "CD Proton momentum P^{truth}_{p} BC",
                                             "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02b_P_pCD_BC_truth_nFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "04_P_piplus_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                             "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "04_P_piplus_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                             "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "05_P_piminus_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                             "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "05_P_piminus_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC",
                                             "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "06_P_pi0_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC",
                                             "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                             "06_P_pi0_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                            "07a_P_ph_AC_truth_nFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC",
                                            "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                            "07a_P_ph_BC_truth_nFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC",
                                               "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                               "07b_P_ph_AC_truth_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC",
                                               "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"],
                                               "07b_P_ph_BC_truth_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (nFDpCD)">
    hPlot1D hTheta_e_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC",
                                               "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                               "01_Theta_e_AC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC",
                                               "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                               "01_Theta_e_BC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC",
                                               "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02a_Theta_n_AC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC",
                                               "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02a_Theta_n_BC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing FD Neutron AC",
                                                 "#theta^{truth}_{n} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_nFD_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing FD Neutron BC",
                                                 "#theta^{truth}_{n} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_nFD_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC",
                                               "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                               "03_Theta_p_AC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC",
                                               "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                               "03_Theta_p_BC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing FD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pFD_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing FD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pFD_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing CD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pCD_AC_truth_nFDpCD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing CD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pCD_BC_truth_nFDpCD", Theta_lboundary_CD, Theta_uboundary_CD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "04_Theta_piplus_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "04_Theta_piplus_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "05_Theta_piminus_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "05_Theta_piminus_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "06_Theta_pi0_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "06_Theta_pi0_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                "07a_Theta_ph_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]",
                                                directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                "07a_Theta_ph_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} AC",
                                                   "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                   "#theta^{truth}_{#gamma} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                   "07b_Theta_ph_AC_truth_nFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} BC",
                                                   "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                   "#theta^{truth}_{#gamma} [Deg]",
                                                   directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                   "07b_Theta_ph_BC_truth_nFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (nFDpCD)">
    hPlot1D hPhi_e_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC",
                                             "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                             "01_Phi_e_AC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC",
                                             "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                             "01_Phi_e_BC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC",
                                             "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02a_Phi_n_AC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC",
                                             "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                             "02a_Phi_n_BC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing FD Neutron AC",
                                               "#phi^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02b_Phi_nFD_AC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing FD Neutron AC",
                                               "#phi^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02b_Phi_nFD_BC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC",
                                             "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                             "03_Phi_p_AC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC",
                                             "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                             "03_Phi_p_BC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{p} AC", "#phi^{truth}_{p} of Outgoing FD Proton AC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02b_Phi_pFD_AC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{p} BC", "#phi^{truth}_{p} of Outgoing FD Proton BC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02b_Phi_pFD_BC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #phi^{truth}_{p} AC", "#phi^{truth}_{p} of Outgoing CD Proton AC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02b_Phi_pCD_AC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #phi^{truth}_{p} BC", "#phi^{truth}_{p} of Outgoing CD Proton BC",
                                               "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "02b_Phi_pCD_BC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "04_Phi_piplus_AC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "04_Phi_piplus_BC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "05_Phi_piminus_AC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "05_Phi_piminus_BC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "06_Phi_pi0_AC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "06_Phi_pi0_BC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                              "07a_Phi_ph_AC_truth_nFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                              "07a_Phi_hp_BC_truth_nFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} AC",
                                                 "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                 "#phi^{truth}_{#gamma} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "07b_Phi_ph_AC_truth_nFDpCD_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} BC",
                                                 "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                 "#phi^{truth}_{#gamma} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "07b_Phi_hp_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (nFDpCD)">
    hPlot2D hnFD_Hit_map_nFDpCD_BEC = hPlot2D("nFDpCD", "FD", "FD neutron hit map BEC", "FD neutron hit map BEC", "x_{nFD}", "y_{nFD}",
                                              directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                              "01a_Neutron_FD_Hit_map_BEC_nFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hnFD_Hit_map_nFDpCD_AEC = hPlot2D("nFDpCD", "FD", "FD neutron hit map AEC", "FD neutron hit map AEC", "x_{nFD}", "y_{nFD}",
                                              directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                              "01b_Neutron_FD_Hit_map_AEC_nFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_nFDpCD_BEC = hPlot2D("nFDpCD", "FD", "#theta_{nFD} vs. #phi_{nFD} BEC", "TL #theta_{nFD} vs. #phi_{nFD} BEC",
                                                       "#phi_{nFD} [Deg]",
                                                       "#theta_{nFD} [Deg]",
                                                       directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                       "02a_Theta_nFD_vs_Phi_nFD_BEC_nFDpCD", Phi_lboundary, Phi_uboundary,
                                                       Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                                       numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_nFD_vs_Phi_nFD_nFDpCD_AEC = hPlot2D("nFDpCD", "FD", "#theta_{nFD} vs. #phi_{nFD} AEC", "TL #theta_{nFD} vs. #phi_{nFD} AEC",
                                                       "#phi_{nFD} [Deg]",
                                                       "#theta_{nFD} [Deg]",
                                                       directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                       "02b_Theta_nFD_vs_Phi_nFD_AEC_nFDpCD", Phi_lboundary, Phi_uboundary,
                                                       Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                                       numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (nFDpCD)">
    hPlot2D hTheta_e_vs_Phi_e_truth_nFDpCD = hPlot2D("nFDpCD", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                     "#theta_{e} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                     "01_Theta_e_vs_Phi_e_truth_nFDpCD",
                                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_truth_nFDpCD = hPlot2D("nFDpCD", "FD", "TL #theta_{nFD} vs. #phi_{nFD}", "TL #theta_{nFD} vs. #phi_{nFD}",
                                                         "#phi_{nFD} [Deg]",
                                                         "#theta_{nFD} [Deg]",
                                                         directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                         "02_Theta_nFD_vs_Phi_nFD_truth_nFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Acceptance maps histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Acceptance maps histograms">
    /* Acceptance maps are handled completely by the AMaps class */
    hPlot2D hElectronAMapBC = hPlot2D("", "", "Electron_AMap_BC", "Electron AMap BC", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                      directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "01_Electron_AMap_BC",
                                      Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                      numTH2Dbins_Electron_AMaps_Plots, numTH2Dbins_Electron_AMaps_Plots);
    hPlot2D hProtonAMapBC = hPlot2D("", "", "Proton_AMap_BC", "Proton AMap BC", "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                    directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "02_Proton_AMap_BC",
                                    Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                    numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNeutronAMapBC = hPlot2D("", "", "Neutron_AMap_BC", "Neutron AMap BC", "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                     directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "03_Neutron_AMap_BC",
                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                     numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNucleonAMapBC = hPlot2D("", "", "Nucleon_AMap_BC", "Nucleon AMap BC", "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]",
                                     directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "04_Nucleon_AMap_BC",
                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                     numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hPlot1D hReco_P_e_AMaps = hPlot1D("1e cut", "FD", "Reco P_{e} used in AMaps", "Reco P_{e} used in AMaps", "P_{e} [GeV/c]",
                                      directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "01a_Reco_P_e_used_in_AMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_e_AMaps = hPlot1D("1e cut", "FD", "TL P_{e} used in AMaps", "TL P_{e} used in AMaps", "P_{e} [GeV/c]",
                                    directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "01b_TL_P_e_used_in_AMaps",
                                    Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_pFD_AMaps = hPlot1D("1e cut", "FD", "Reco P_{pFD} used in AMaps", "Reco P_{pFD} used in AMaps", "P_{pFD} [GeV/c]",
                                        directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "02a_Reco_P_pFD_used_in_AMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_pFD_AMaps = hPlot1D("1e cut", "FD", "TL P_{pFD} used in AMaps", "TL P_{pFD} used in AMaps", "P_{pFD} [GeV/c]",
                                      directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "02b_TL_P_pFD_used_in_AMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_nFD_AMaps = hPlot1D("1e cut", "FD", "Reco P_{nFD} used in AMaps", "Reco P_{nFD} used in AMaps", "P_{nFD} [GeV/c]",
                                        directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "03a_Reco_P_nFD_used_in_AMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_nFD_AMaps = hPlot1D("1e cut", "FD", "TL P_{nFD} used in AMaps", "TL P_{nFD} used in AMaps", "P_{nFD} [GeV/c]",
                                      directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "03b_TL_P_nFD_used_in_AMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot2D hElectronAMapBCwKC = hPlot2D("", "", "Electron_AMap_BCwKC", "Electron AMap BC wKC", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                         directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "01_Electron_AMap_BCwKC",
                                         Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                         numTH2Dbins_Electron_AMaps_Plots, numTH2Dbins_Electron_AMaps_Plots);
    hPlot2D hProtonAMapBCwKC = hPlot2D("", "", "Proton_AMap_BCwKC", "Proton AMap BC wKC", "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                       directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "02_Proton_AMap_BCwKC",
                                       Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                       numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNeutronAMapBCwKC = hPlot2D("", "", "Neutron_AMap_BCwKC", "Neutron AMap BC wKC", "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                        directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "03_Neutron_AMap_BCwKC",
                                        Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                        numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNucleonAMapBCwKC = hPlot2D("", "", "Nucleon_AMap_BCwKC", "Nucleon AMap BC wKC", "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]",
                                        directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "04_Nucleon_AMap_BCwKC",
                                        Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                        numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hPlot1D hReco_P_e_WMaps = hPlot1D("1e cut", "FD", "Reco P_{e} used in WMaps", "Reco P_{e} used in WMaps", "P_{e} [GeV/c]",
                                      directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "01a_Reco_P_e_used_in_WMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_e_WMaps = hPlot1D("1e cut", "FD", "TL P_{e} used in WMaps", "TL P_{e} used in WMaps", "P_{e} [GeV/c]",
                                    directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "01b_TL_P_e_used_in_WMaps",
                                    Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_pFD_WMaps = hPlot1D("1e cut", "FD", "Reco P_{pFD} used in WMaps", "Reco P_{pFD} used in WMaps", "P_{pFD} [GeV/c]",
                                        directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "02a_Reco_P_pFD_used_in_WMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_pFD_WMaps = hPlot1D("1e cut", "FD", "TL P_{pFD} used in WMaps", "TL P_{pFD} used in WMaps", "P_{pFD} [GeV/c]",
                                      directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "02b_TL_P_pFD_used_in_WMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_nFD_WMaps = hPlot1D("1e cut", "FD", "Reco P_{nFD} used in WMaps", "Reco P_{nFD} used in WMaps", "P_{nFD} [GeV/c]",
                                        directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "03a_Reco_P_nFD_used_in_WMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_nFD_WMaps = hPlot1D("1e cut", "FD", "TL P_{nFD} used in WMaps", "TL P_{nFD} used in WMaps", "P_{nFD} [GeV/c]",
                                      directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "03b_TL_P_nFD_used_in_WMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

// ======================================================================================================================================================================
// Resolution histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Resolution histograms">

    //<editor-fold desc="Resolution histograms (1p)">
    hPlot1D hdTheta_pFD_TL_BC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} BC", "#Delta#theta_{pFD} of FD proton BC",
                                           "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1p_Directory"], "00a_DeltaTheta_pFD_BC_TL_1p",
                                           -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_pFD_TL_ZOOMIN_BC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} - ZOOMIN BC", "#Delta#theta_{pFD} of FD proton - ZOOMIN BC",
                                                  "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1p_Directory"],
                                                  "00b_DeltaTheta_pFD_BC_TL_ZOOMIN_1p",
                                                  -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_BC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} BC", "#Delta#phi_{pFD} of FD proton BC",
                                         "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                         directories.Resolution_Directory_map["Resolution_1p_Directory"], "00c_DeltaPhi_pFD_BC_TL_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_ZOOMIN_BC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} - ZOOMIN BC", "#Delta#phi_{pFD} of FD proton - ZOOMIN BC",
                                                "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1p_Directory"],
                                                "00d_DeltaPhi_pFD_BC_TL_ZOOMIN_1p",
                                                -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hdTheta_pFD_TL_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} AdPC", "#Delta#theta_{pFD} of FD proton AdPC",
                                             "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                             directories.Resolution_Directory_map["Resolution_1p_Directory"], "00e_DeltaTheta_pFD_AdPC_TL_1p",
                                             -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_pFD_TL_ZOOMIN_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} - ZOOMIN AdPC",
                                                    "#Delta#theta_{pFD} of FD proton - ZOOMIN AdPC",
                                                    "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                                    directories.Resolution_Directory_map["Resolution_1p_Directory"],
                                                    "00f_DeltaTheta_pFD_AdPC_TL_ZOOMIN_1p",
                                                    -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} AdPC", "#Delta#phi_{pFD} of FD proton AdPC",
                                           "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1p_Directory"], "00g_DeltaPhi_pFD_AdPC_TL_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_ZOOMIN_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} - ZOOMIN AdPC", "#Delta#phi_{pFD} of FD proton - ZOOMIN AdPC",
                                                  "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1p_Directory"],
                                                  "00h_DeltaPhi_pFD_AdPC_TL_ZOOMIN_1p",
                                                  -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hTheta_pFD_TL_ApResC_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} ApResC", "#theta^{truth}_{pFD} of FD proton ApResC",
                                              "#theta^{truth}_{pFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1p_Directory"], "01a_Theta_pFD_ApResC_TL_1p",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_pFD_TL_ApResC_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} ApResC", "#phi^{truth}_{pFD} of FD proton ApResC",
                                            "#phi^{truth}_{pFD} [Deg]",
                                            directories.Resolution_Directory_map["Resolution_1p_Directory"], "02a_Phi_pFD_ApResC_TL_1p",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p = new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} ApResC (1p, FD)",
                                                           "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} ApResC (1p, FD);#phi^{truth}_{pFD} [Deg];#theta^{truth}_{pFD} [Deg]",
                                                           numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                           numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hTheta_pFD_TL_MatchedP_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} of matched TL p",
                                                "#theta^{truth}_{pFD} of matched TL FD proton",
                                                "#theta^{truth}_{pFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1p_Directory"], "01b_Theta_pFD_MatchedN_TL_1p",
                                                Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_pFD_TL_MatchedP_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} of matched TL p", "#phi^{truth}_{pFD} of matched TL FD proton",
                                              "#phi^{truth}_{pFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1p_Directory"], "02b_Phi_pFD_MatchedN_TL_1p",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p = new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} of matched TL p (1p, FD)",
                                                             "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} of matched TL FD proton (1p, FD);#phi^{truth}_{pFD} [Deg];#theta^{truth}_{pFD} [Deg]",
                                                             numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                             numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hTheta_pFD_TL_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} AC", "#theta^{truth}_{pFD} of FD proton AC",
                                       "#theta^{truth}_{pFD} [Deg]",
                                       directories.Resolution_Directory_map["Resolution_1p_Directory"], "01_Theta_pFD_AC_TL_1p",
                                       Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_pFD_TL_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} AC", "#phi^{truth}_{pFD} of FD proton AC", "#phi^{truth}_{pFD} [Deg]",
                                     directories.Resolution_Directory_map["Resolution_1p_Directory"], "02_Phi_pFD_AC_TL_1p",
                                     Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_pFD_TL_VS_Phi_pFD_TL_1p = new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} (1p, FD)",
                                                    "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} (1p, FD);#phi^{truth}_{pFD} [Deg];#theta^{truth}_{pFD} [Deg]",
                                                    numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_nRes_Plots,
                                                    Theta_lboundary_FD,
                                                    Theta_uboundary_FD);
    string hTheta_pFD_TL_VS_Phi_pFD_TL_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hP_pFD_Res_1p = hPlot1D("1p", "", "P_{pFD} resolution AC", "FD proton P_{pFD} resolution AC",
                                    "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}",
                                    directories.Resolution_Directory_map["Resolution_1p_Directory"], "04_P_pFD_Res_1p",
                                    -2, 2, numTH1Dbins_nRes_Plots);
    TH2D *hP_pFD_Res_VS_TL_P_pFD_1p = new TH2D("P_{pFD} resolution AC vs. P^{truth}_{pFD} (1p, FD)",
                                               "P_{pFD} resolution AC vs. P^{truth}_{pFD} (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                               "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}",
                                               numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -1.1, 1.1);
    TH2D *hP_pFD_Res_VS_TL_P_pFD_ZOOMIN_1p = new TH2D("P_{pFD} resolution AC vs. P^{truth}_{pFD} - ZOOMIN (1p, FD)",
                                                      "P_{pFD} resolution AC vs. P^{truth}_{pFD} - ZOOMIN (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                                      "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}",
                                                      numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut.GetLowerCut(),
                                                      FD_nucleon_momentum_cut.GetUpperCut(),
                                                      numTH2Dbins_nRes_Plots, -0.75, 0.75);
    TH2D *hP_pFD_Res_VS_TL_P_pFD_noKC_1p = new TH2D("P_{pFD} resolution AC vs. P^{truth}_{pFD} no mom. KC (1p, FD)",
                                                    "P_{pFD} resolution AC vs. P^{truth}_{pFD} no mom. KC (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                                    "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}",
                                                    numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -1.1, 1.1);
    string hP_pFD_Res_VS_TL_P_pFD_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hReco_P_pFD_pRes_1p = hPlot1D("1p", "FD", "Reco P_{pFD} used in pRes", "Reco P_{pFD} used in pRes", "P_{pFD} [GeV/c]",
                                          directories.Resolution_Directory_map["Resolution_1p_Directory"], "06a_Reco_P_pFD_used_in_pRes_1p",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_pFD_pRes_1p = hPlot1D("1p", "FD", "TL P_{pFD} used in pRes", "TL P_{pFD} used in pRes", "P_{pFD} [GeV/c]",
                                        directories.Resolution_Directory_map["Resolution_1p_Directory"], "06b_TL_P_pFD_used_in_pRes_1p",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Resolution histograms (1n)">
    hPlot1D hdTheta_nFD_TL_BC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} BC", "#Delta#theta_{nFD} of FD neutron BC",
                                           "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1n_Directory"], "00a_DeltaTheta_nFD_BC_TL_1n",
                                           -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_nFD_TL_ZOOMIN_BC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} - ZOOMIN BC", "#Delta#theta_{nFD} of FD neutron - ZOOMIN BC",
                                                  "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                  "00b_DeltaTheta_nFD_BC_TL_ZOOMIN_1n",
                                                  -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_BC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} BC", "#Delta#phi_{nFD} of FD neutron BC",
                                         "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                         directories.Resolution_Directory_map["Resolution_1n_Directory"], "00c_DeltaPhi_nFD_BC_TL_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_ZOOMIN_BC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} - ZOOMIN BC", "#Delta#phi_{nFD} of FD neutron - ZOOMIN BC",
                                                "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                "00d_DeltaPhi_nFD_BC_TL_ZOOMIN_1n",
                                                -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hdTheta_nFD_TL_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} AdPC", "#Delta#theta_{nFD} of FD neutron AdPC",
                                             "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                             directories.Resolution_Directory_map["Resolution_1n_Directory"], "00e_DeltaTheta_nFD_AdPC_TL_1n",
                                             -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_nFD_TL_ZOOMIN_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} - ZOOMIN AdPC",
                                                    "#Delta#theta_{nFD} of FD neutron - ZOOMIN AdPC",
                                                    "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                                    directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                    "00f_DeltaTheta_nFD_AdPC_TL_ZOOMIN_1n",
                                                    -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} AdPC", "#Delta#phi_{nFD} of FD neutron AdPC",
                                           "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1n_Directory"], "00g_DeltaPhi_nFD_AdPC_TL_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_ZOOMIN_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} - ZOOMIN AdPC", "#Delta#phi_{nFD} of FD neutron - ZOOMIN AdPC",
                                                  "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                  "00h_DeltaPhi_nFD_AdPC_TL_ZOOMIN_1n",
                                                  -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hTheta_nFD_TL_AnResC_1n = hPlot1D("1n", "", "TL #theta^{truth}_{nFD} AnResC", "#theta^{truth}_{nFD} of FD neutron AnResC",
                                              "#theta^{truth}_{nFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1n_Directory"], "01a_Theta_nFD_AnResC_TL_1n",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_nFD_TL_AnResC_1n = hPlot1D("1n", "", "TL #phi^{truth}_{nFD} AnResC", "#phi^{truth}_{nFD} of FD neutron AnResC",
                                            "#phi^{truth}_{nFD} [Deg]",
                                            directories.Resolution_Directory_map["Resolution_1n_Directory"], "02a_Phi_nFD_AnResC_TL_1n",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n = new TH2D("#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} AnResC (1n, FD)",
                                                           "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} AnResC (1n, FD);#phi^{truth}_{nFD} [Deg];#theta^{truth}_{nFD} [Deg]",
                                                           numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                           numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];

    hPlot1D hTheta_nFD_TL_MatchedN_1n = hPlot1D("1n", "", "TL #theta^{truth}_{nFD} of matched TL n",
                                                "#theta^{truth}_{nFD} of matched TL FD neutron",
                                                "#theta^{truth}_{nFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1n_Directory"], "01b_Theta_nFD_MatchedN_TL_1n",
                                                Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_nFD_TL_MatchedN_1n = hPlot1D("1n", "", "TL #phi^{truth}_{nFD} of matched TL n", "#phi^{truth}_{nFD} of matched TL FD neutron",
                                              "#phi^{truth}_{nFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1n_Directory"], "02b_Phi_nFD_MatchedN_TL_1n",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n = new TH2D("#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} of matched TL n (1n, FD)",
                                                             "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} of matched TL FD neutron (1n, FD);#phi^{truth}_{nFD} [Deg];#theta^{truth}_{nFD} [Deg]",
                                                             numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                             numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];

    hPlot1D hP_nFD_Res_1n = hPlot1D("1n", "", "P_{nFD} resolution AC", "FD neutron P_{nFD} resolution AC",
                                    "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                    directories.Resolution_Directory_map["Resolution_1n_Directory"], "04_P_nFD_Res_1n",
                                    -2, 2, numTH1Dbins_nRes_Plots);
    TH2D *hP_nFD_Res_VS_TL_P_nFD_1n = new TH2D("P_{nFD} resolution AC vs. P^{truth}_{nFD} (1n, FD)",
                                               "P_{nFD} resolution AC vs. P^{truth}_{nFD} (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                               "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                               numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -1.1, 1.1);
    TH2D *hP_nFD_Res_VS_TL_P_nFD_ZOOMIN_1n = new TH2D("P_{nFD} resolution AC vs. P^{truth}_{nFD} - ZOOMIN (1n, FD)",
                                                      "P_{nFD} resolution AC vs. P^{truth}_{nFD} - ZOOMIN (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                                      "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                                      numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut.GetLowerCut(),
                                                      FD_nucleon_momentum_cut.GetUpperCut(),
                                                      numTH2Dbins_nRes_Plots, -0.75, 0.75);
    TH2D *hP_nFD_Res_VS_TL_P_nFD_noKC_1n = new TH2D("P_{nFD} resolution AC vs. P^{truth}_{nFD} no mom. KC (1n, FD)",
                                                    "P_{nFD} resolution AC vs. P^{truth}_{nFD} no mom. KC (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                                    "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                                    numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -1.1, 1.1);
    TH2D *hP_nFD_Res_VS_Reco_P_nFD_1n = new TH2D("P_{nFD} resolution AC vs. P^{reco}_{nFD} (1n, FD)",
                                                 "P_{nFD} resolution AC vs. P^{reco}_{nFD} (1n, FD);P^{reco}_{nFD} [GeV/c];"
                                                 "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                                 numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -1.1, 1.1);
    TH2D *hP_nFD_Res_VS_Reco_P_nFD_ZOOMIN_1n = new TH2D("P_{nFD} resolution AC vs. P^{reco}_{nFD} - ZOOMIN (1n, FD)",
                                                        "P_{nFD} resolution AC vs. P^{reco}_{nFD} - ZOOMIN (1n, FD);P^{reco}_{nFD} [GeV/c];"
                                                        "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                                        numTH2Dbins_nRes_Plots, FD_nucleon_momentum_cut.GetLowerCut(),
                                                        FD_nucleon_momentum_cut.GetUpperCut(),
                                                        numTH2Dbins_nRes_Plots, -0.75, 0.75);
    TH2D *hP_nFD_Res_VS_Reco_P_nFD_noKC_1n = new TH2D("P_{nFD} resolution AC vs. P^{reco}_{nFD} no mom. KC (1n, FD)",
                                                      "P_{nFD} resolution AC vs. P^{reco}_{nFD} no mom. KC (1n, FD);P^{reco}_{nFD} [GeV/c];"
                                                      "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                                      numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -1.1, 1.1);
    string hP_nFD_Res_VS_TL_P_nFD_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];
    string hP_nFD_Res_VS_Reco_P_nFD_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];

    hPlot1D hReco_P_nFD_nRes_1n = hPlot1D("1n", "FD", "Reco P_{nFD} used in nRes", "Reco P_{nFD} used in nRes", "P_{nFD} [GeV/c]",
                                          directories.Resolution_Directory_map["Resolution_1n_Directory"], "06a_Reco_P_nFD_used_in_nRes_1n",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_nFD_nRes_1n = hPlot1D("1n", "FD", "TL P_{nFD} used in nRes", "TL P_{nFD} used in nRes", "P_{nFD} [GeV/c]",
                                        directories.Resolution_Directory_map["Resolution_1n_Directory"], "06b_TL_P_nFD_used_in_nRes_1n",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot1D hEff_dist_TL_1n = hPlot1D("1n", "FD", "Effective distance #font[12]{l}_{eff}^{TL} with #beta_{LT}",
                                      "Effective distance #font[12]{l}_{eff}^{TL} with #beta_{LT}", "#font[12]{l}_{eff}^{TL} [cm]",
                                      directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_Eff_dist_TL_1n", -6000, 6000,
                                      numTH1Dbins);
    hPlot2D hEff_dist_TL_VS_TL_P_nFD_1n = hPlot2D("1n", "FD", "Effective distance #font[12]{l}_{eff}^{TL} with #beta_{LT} vs. P^{truth}_{nFD}",
                                                  "Effective distance #font[12]{l}_{eff}^{TL} with #beta_{LT} vs. P^{truth}_{nFD}",
                                                  "#font[12]{l}_{eff}^{TL} [cm]",
                                                  "P^{truth}_{nFD} [GeV/c]", directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                  "00XX_Eff_dist_TL_vs_TL_P_nFD_1n", -6000, 6000, n_mom_th.GetLowerCut() * 0.95, 4 * 1.05,
                                                  numTH2Dbins, numTH2Dbins);
    hPlot1D hddist_TL_1n = hPlot1D("1n", "FD", "#Delta#font[12]{l}_{eff}^{TL}", "#Delta#font[12]{l}_{eff}^{TL}",
                                   "#Delta#font[12]{l}_{eff}^{TL} = #font[12]{l}_{path} - #font[12]{l}_{eff}^{TL} [cm]",
                                   directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_ddist_TL_1n", -6000, 6000, numTH1Dbins);
    hPlot2D hddist_TL_VS_TL_P_nFD_1n = hPlot2D("1n", "FD", "#Delta#font[12]{l}_{eff}^{TL} vs. P^{truth}_{nFD}",
                                               "#Delta#font[12]{l}_{eff}^{TL} vs. P^{truth}_{nFD}",
                                               "#Delta#font[12]{l}_{eff}^{TL} = #font[12]{l}_{path} - #font[12]{l}_{eff}^{TL} [cm]",
                                               "P^{truth}_{nFD} [GeV/c]",
                                               directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_ddist_TL_vs_TL_P_nFD_1n",
                                               -6000, 6000, n_mom_th.GetLowerCut() * 0.95, 4 * 1.05, numTH2Dbins, numTH2Dbins);

    hPlot1D hEff_dist_Reco_1n = hPlot1D("1n", "FD", "Effective distance #font[12]{l}_{eff}^{reco} with #beta_{reco}",
                                        "Effective distance #font[12]{l}_{eff}^{reco} with #beta_{LT}", "#font[12]{l}_{eff}^{reco} [cm]",
                                        directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_Eff_dist_Reco_1n", -6000, 6000,
                                        numTH1Dbins);
    hPlot2D hEff_dist_Reco_VS_TL_P_nFD_1n = hPlot2D("1n", "FD",
                                                    "Effective distance #font[12]{l}_{eff}^{reco} with #beta_{reco} vs. P^{truth}_{nFD}",
                                                    "Effective distance #font[12]{l}_{eff}^{reco} with #beta_{reco} vs. P^{truth}_{nFD}",
                                                    "#font[12]{l}_{eff}^{reco} [cm]",
                                                    "P^{truth}_{nFD} [GeV/c]", directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                    "00XX_Eff_dist_Reco_vs_TL_P_nFD_1n", -6000, 6000, n_mom_th.GetLowerCut() * 0.95, 4 * 1.05,
                                                    numTH2Dbins, numTH2Dbins);
    hPlot1D hddist_Reco_1n = hPlot1D("1n", "FD", "#Delta#font[12]{l}_{eff}^{reco}", "#Delta#font[12]{l}_{eff}^{reco}",
                                     "#Delta#font[12]{l}_{eff}^{reco} = #font[12]{l}_{path} - #font[12]{l}_{eff}^{reco} [cm]",
                                     directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_ddist_reco_1n", -6000, 6000,
                                     numTH1Dbins);
    hPlot2D hddist_Reco_VS_TL_P_nFD_1n = hPlot2D("1n", "FD", "#Delta#font[12]{l}_{eff}^{reco} vs. P^{truth}_{nFD}",
                                                 "#Delta#font[12]{l}_{eff}^{reco} vs. P^{truth}_{nFD}",
                                                 "#Delta#font[12]{l}_{eff}^{reco} = #font[12]{l}_{path} - #font[12]{l}_{eff}^{reco} [cm]",
                                                 "P^{truth}_{nFD} [GeV/c]", directories.Resolution_Directory_map["Resolution_1n_Directory"],
                                                 "00XX_ddist_reco_vs_reco_P_nFD_1n", -6000, 6000, n_mom_th.GetLowerCut() * 0.95, 4 * 1.05,
                                                 numTH2Dbins, numTH2Dbins);

    hPlot1D hTOF_error_1n = hPlot1D("1n", "FD", "TOF Error #Deltat_{TOF}", "TOF Error #Deltat_{TOF}", "#Deltat_{TOF} [ns]",
                                    directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_TOF_error_1n", -10., 10., numTH1Dbins);
    hPlot2D hTOF_error_VS_TL_P_nFD_1n = hPlot2D("1n", "FD", "TOF Error #Deltat_{TOF} vs. P^{truth}_{nFD}",
                                                "TOF Error #Deltat_{TOF} vs. P^{truth}_{nFD}",
                                                "#Deltat_{TOF} [ns]", "P^{truth}_{nFD} [GeV/c]",
                                                directories.Resolution_Directory_map["Resolution_1n_Directory"], "00XX_TOF_error_VS_TL_P_nFD_1n",
                                                -10., 10., n_mom_th.GetLowerCut() * 0.95, 4 * 1.05, numTH2Dbins, numTH2Dbins);
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Multiplicity histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Multiplicity histograms">

    //<editor-fold desc="Resolution histograms (1e cut)">
    hPlot1D hMultiplicity_Neutrons_By_Redef_APID_BV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by redef.  APID BV",
                                                                        "Multiplicity of neutrons by redef.  APID BV", "Multiplicity",
                                                                        directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                        "01_Multiplicity_Neutrons_By_Redef_APID_BV_1e_cut_FD", 0, 10,
                                                                        numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_APID_AV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by redef.  APID AV",
                                                                        "Multiplicity of neutrons by redef.  APID AV", "Multiplicity",
                                                                        directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                        "02_Multiplicity_Neutrons_By_Redef_APID_AV_1e_cut_FD", 0, 10,
                                                                        numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_BPID_BV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by redef.  BPID BV",
                                                                        "Multiplicity of neutrons by redef.  BPID BV", "Multiplicity",
                                                                        directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                        "03_Multiplicity_Neutrons_By_Redef_BPID_BV_1e_cut_FD", 0, 10,
                                                                        numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_BPID_AV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by redef.  BPID AV",
                                                                        "Multiplicity of neutrons by redef.  BPID AV", "Multiplicity",
                                                                        directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                        "04_Multiplicity_Neutrons_By_Redef_BPID_AV_1e_cut_FD", 0, 10,
                                                                        numTH1Dbins_Mom_eff_Plots);

    hPlot1D hMultiplicity_Neutrons_By_clas12pid_APID_BV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by clas12pid  APID BV",
                                                                            "Multiplicity of neutrons by clas12pid  APID BV", "Multiplicity",
                                                                            directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                            "01_Multiplicity_Neutrons_By_clas12pid_APID_BV_1e_cut_FD", 0, 10,
                                                                            numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_clas12pid_APID_AV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by clas12pid  APID AV",
                                                                            "Multiplicity of neutrons by clas12pid  APID AV", "Multiplicity",
                                                                            directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                            "02_Multiplicity_Neutrons_By_clas12pid_APID_AV_1e_cut_FD", 0, 10,
                                                                            numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_clas12pid_BPID_BV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by clas12pid  BPID BV",
                                                                            "Multiplicity of neutrons by clas12pid  BPID BV", "Multiplicity",
                                                                            directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                            "03_Multiplicity_Neutrons_By_clas12pid_BPID_BV_1e_cut_FD", 0, 10,
                                                                            numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_clas12pid_BPID_AV_1e_cut_FD = hPlot1D("1e cut", "FD", "Multiplicity of neutrons by clas12pid  BPID AV",
                                                                            "Multiplicity of neutrons by clas12pid  BPID AV", "Multiplicity",
                                                                            directories.Multiplicity_Directory_map["Multiplicity_1e_cut_Directory"],
                                                                            "04_Multiplicity_Neutrons_By_clas12pid_BPID_AV_1e_cut_FD", 0, 10,
                                                                            numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Resolution histograms (1p)">
    //TODO: need AV plots?
    hPlot1D hMultiplicity_Neutrons_By_Redef_APID_BV_1p_FD = hPlot1D("1p", "FD", "Multiplicity of neutrons by redef.  APID BV",
                                                                    "Multiplicity of neutrons by redef.  APID BV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1p_Directory"],
                                                                    "01_Multiplicity_Neutrons_By_Redef_APID_BV_1p_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_APID_AV_1p_FD = hPlot1D("1p", "FD", "Multiplicity of neutrons by redef.  APID AV",
                                                                    "Multiplicity of neutrons by redef.  APID AV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1p_Directory"],
                                                                    "02_Multiplicity_Neutrons_By_Redef_APID_AV_1p_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_BPID_BV_1p_FD = hPlot1D("1p", "FD", "Multiplicity of neutrons by redef.  BPID BV",
                                                                    "Multiplicity of neutrons by redef.  BPID BV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1p_Directory"],
                                                                    "03_Multiplicity_Neutrons_By_Redef_BPID_BV_1p_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_BPID_AV_1p_FD = hPlot1D("1p", "FD", "Multiplicity of neutrons by redef.  BPID AV",
                                                                    "Multiplicity of neutrons by redef.  BPID AV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1p_Directory"],
                                                                    "04_Multiplicity_Neutrons_By_Redef_BPID_AV_1p_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Resolution histograms (1n)">
    //TODO: need AV plots?
    hPlot1D hMultiplicity_Neutrons_By_Redef_APID_BV_1n_FD = hPlot1D("1n", "FD", "Multiplicity of neutrons by redef.  APID BV",
                                                                    "Multiplicity of neutrons by redef.  APID BV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1n_Directory"],
                                                                    "01_Multiplicity_Neutrons_By_Redef_APID_BV_1n_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_APID_AV_1n_FD = hPlot1D("1n", "FD", "Multiplicity of neutrons by redef.  APID AV",
                                                                    "Multiplicity of neutrons by redef.  APID AV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1n_Directory"],
                                                                    "02_Multiplicity_Neutrons_By_Redef_APID_AV_1n_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_BPID_BV_1n_FD = hPlot1D("1n", "FD", "Multiplicity of neutrons by redef.  BPID BV",
                                                                    "Multiplicity of neutrons by redef.  BPID BV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1n_Directory"],
                                                                    "03_Multiplicity_Neutrons_By_Redef_BPID_BV_1n_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    hPlot1D hMultiplicity_Neutrons_By_Redef_BPID_AV_1n_FD = hPlot1D("1n", "FD", "Multiplicity of neutrons by redef.  BPID AV",
                                                                    "Multiplicity of neutrons by redef.  BPID AV", "Multiplicity",
                                                                    directories.Multiplicity_Directory_map["Multiplicity_1n_Directory"],
                                                                    "04_Multiplicity_Neutrons_By_Redef_BPID_AV_1n_FD", 0, 10,
                                                                    numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    cout << " done.\n\n";
    //</editor-fold>

public:

// Default constructor --------------------------------------------------------------------------------------------------------------------------------------------------

    RecoAnalyser() = default;

// InitSettings function ------------------------------------------------------------------------------------------------------------------------------------------------

void InitSettings();

};

#endif //RECOANALYSER_H
