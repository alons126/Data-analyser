/* root BetaFitAndSave.cpp -q -b */

#include <iostream>
#include <fstream>
#include <string>

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

#define noFSRatio false
#define FullAnalyser false
#define Independent1DHistDraw true
#define Independent1DMomResDraw false
#define Independent1DGraphDraw false
#define IndependentStackdraw false
#define IndependentStack1draw false
#define Independent2Ddraw false

#define PresentationMode true
#define PosterMode true
#define ColorblindMode false

#include "HistPlotter1D.cpp"
#include "HistPlotterStack.cpp"
#include "HistPlotterStack1.cpp"
#include "HistPlotter2D.cpp"
#include "AMapsPlotter.cpp"
#include "MomResSlicePlotter.cpp"
#include "GraphPlotter1D.cpp"
#include "FSRPlotter.cpp"

using namespace std;

/* sudo mount -t drvfs E: /mnt/e */
/* clas12root macros/MScThesisPlotter/MScThesisPlotter.cpp -b -q */

void MScThesisPlotter() {
    hData utilities;

    const string SampleName = "C12x4_simulation_G18_Q204_6GeV";

    //<editor-fold desc="Runs">

    //<editor-fold desc="Initial runs">
    const char *NO_CUTS_beta_VS_P_no_clas12ana = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/"
                                                 "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_no_clas12ana_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_NO_CUTS = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_v2"
                                "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_NO_CUTS_DC = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_v2"
                                   "/DebugOutputFile.root";
    const char *plots_Preselection_only = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/"
                                          "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_Preselection_Cuts_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_Preselection_only_DC = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/"
                                             "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_Preselection_Cuts_v2/DebugOutputFile.root";
    const char *plots_eQC_only = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_eQC_v2"
                                 "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_PreselectionAndeQC_only = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/"
                                                "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_Preselection_Cuts_eQC_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_woChi2 = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S01ACwoChi2_v2"
                               "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_noNC = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S02ACnoNC_v2"
                             "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_NC = "/mnt/e/C12x4_sim_G18_Q204_6GeV/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_v2"
                           "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    //</editor-fold>

    //<editor-fold desc="AMaps runs">
    const char *plots_AMaps = "/mnt/e/C12x4_sim_G18_Q204_6GeV/02_AMaps_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_AMaps_v3"
                              "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_AMaps_plotsPath = "/mnt/e/C12x4_sim_G18_Q204_6GeV/02_AMaps_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_AMaps_v3/";
    const char *plots_AMaps_GenMaps = "/mnt/e/C12x4_sim_G18_Q204_6GeV/02_AMaps_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_AMaps_v3/"
                                      "12_Acceptance_maps_plots/01_AMaps_1e_cut/05_Generated_maps/";
    //</editor-fold>

    //<editor-fold desc="Efficiency runs">
    const char *plots_Efficiency_reg = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_EffwZoomIn_v3"
                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_Efficiency_reg_wMomTh = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_EffwZoomIn_v3"
                                              "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_Efficiency_wFC = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_EffwZoomIn_v3"
                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_Efficiency_wFC_wMomTh = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_EffwZoomIn_wFC_v3"
                                              "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_Efficiency_reg = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_Eff_v2"
//                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_Efficiency_reg_wMomTh = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_Eff_v2"
//                                              "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_Efficiency_wFC = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_Eff_v2"
//                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_Efficiency_wFC_wMomTh = "/mnt/e/C12x4_sim_G18_Q204_6GeV/04_Efficiency_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_Eff_wFC_v2"
//                                              "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    //</editor-fold>

    //<editor-fold desc="momRes runs">
    const char *plots_momResS1_Gen = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1T_v3_ReRun3"
                                     "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_momResS1_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1T_v3_ReRun3"
                                             "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_pResS1_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1T_v3_ReRun3"
                                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_momResS2_Gen = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2T_v3_ReRun3"
                                     "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_momResS2_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2T_v3_ReRun3"
                                             "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_pResS2_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2T_v3_ReRun3"
                                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_momResS2RT_Gen = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3"
                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_momResS2RT_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3_ReRun3"
                                               "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_pResS2RT_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3_ReRun3"
                                             "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_pResS2RT_FittedPlots2 = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3_ReRun3"
                                              "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *plots_momResS1_Gen = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1_v3"
//                                     "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_momResS1_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1_v3"
//                                             "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *plots_pResS1_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_momResS1_v3"
//                                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *plots_momResS2_Gen = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2_v3"
//                                     "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_momResS2_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2_v3"
//                                             "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *plots_pResS2_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2_v3"
//                                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *plots_momResS2RT_Gen = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3"
//                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *plots_momResS2RT_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3"
//                                               "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
//    const char *plots_pResS2RT_FittedPlots = "/mnt/e/C12x4_sim_G18_Q204_6GeV/03_momRes_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_momResS2RT_v3"
//                                             "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    //</editor-fold>

    //<editor-fold desc="Final runs">
    const char *Final_noSaC_Sim = "/mnt/e/C12x4_sim_G18_Q204_6GeV/05_Final_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wFCwKC_wKW_v3/"
                                  "C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *Final_wSaC_Sim = "/mnt/e/C12x4_sim_G18_Q204_6GeV/05_Final_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_wFCwKC_wKW_v3_ReRun2/"
                                 "C12x4_simulation_G18_Q204_6GeV_plots.root";
//    const char *Final_wSaC_Sim = "/mnt/e/C12x4_sim_G18_Q204_6GeV/05_Final_runs/v3/C12x4_simulation_G18_Q204_6GeV_S03AC_NC_wNSaC_wFCwKC_wKW_v3_ReRun/"
//                                 "C12x4_simulation_G18_Q204_6GeV_plots.root";

    /* C12x4 data - run 015188 */
    const char *Final_noSaC_Data = "/mnt/e/C12x4_data_6GeV_run_015188_for_Thesis/05_Final_runs/v3/C12x4_data_6GeV_run_015188_S03AC_NC_wFCwKC_wKW_v3/"
                                   "C12x4_data_6GeV_run_015188_plots.root";
    const char *Final_wSaC_Data = "/mnt/e/C12x4_data_6GeV_run_015188_for_Thesis/05_Final_runs/v3/C12x4_data_6GeV_run_015188_S03AC_NC_wNSaC_wFCwKC_wKW_v3_ReRun2/"
                                  "C12x4_data_6GeV_run_015188_plots.root";
//    const char *Final_wSaC_Data = "/mnt/e/C12x4_data_6GeV_run_015188_for_Thesis/05_Final_runs/v3/C12x4_data_6GeV_run_015188_S03AC_NC_wNSaC_wFCwKC_wKW_v3/"
//                                  "C12x4_data_6GeV_run_015188_plots.root";
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Make directories">

    //<editor-fold desc="Preselection_Sim">
    const string PreselectionSimFolder = "00_MScThesisPlotter/Preselection_Sim";
    system(("rm -r " + PreselectionSimFolder).c_str());
    system(("mkdir -p " + PreselectionSimFolder).c_str());

    //<editor-fold desc="Vertex cuts">
    const string VertexCutsFolderSim = "00_MScThesisPlotter/Preselection_Sim/01_Vertex_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSim).c_str());

    const string VertexCutsFolderSimBC = "00_MScThesisPlotter/Preselection_Sim/01_Vertex_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSimBC).c_str());

    const string VertexCutsFolderSimAC = "00_MScThesisPlotter/Preselection_Sim/01_Vertex_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="Vertex correlation cuts">
    const string VertexCorrCutsFolderSim = "00_MScThesisPlotter/Preselection_Sim/02_Vertex_Corr_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSim).c_str());

    const string VertexCorrCutsFolderSimBC = "00_MScThesisPlotter/Preselection_Sim/02_Vertex_Corr_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + VertexCorrCutsFolderSimBC).c_str());

    const string VertexCorrCutsFolderSimAC = "00_MScThesisPlotter/Preselection_Sim/02_Vertex_Corr_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + VertexCorrCutsFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="DC fiducial cuts">
    const string DCFiducialCutsFolderSim = "00_MScThesisPlotter/Preselection_Sim/03_DC_fiducial_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSim).c_str());

    const string DCFiducialCutsFolderSimBC = "00_MScThesisPlotter/Preselection_Sim/03_DC_fiducial_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSimBC).c_str());

    const string DCFiducialCutsFolderSimAC = "00_MScThesisPlotter/Preselection_Sim/03_DC_fiducial_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSimAC).c_str());
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Event_Selection_Sim">
    const string EventSelectionSimFolder = "00_MScThesisPlotter/Event_Selection_Sim";
    system(("rm -r " + EventSelectionSimFolder).c_str());
    system(("mkdir -p " + EventSelectionSimFolder).c_str());

    const string ElectronIDFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/01_Electron_ID_Sim";
    system(("mkdir -p " + ElectronIDFolderSim).c_str());

    const string ElectronIDFolderSimBC = "00_MScThesisPlotter/Event_Selection_Sim/01_Electron_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + ElectronIDFolderSimBC).c_str());

    const string ElectronIDFolderSimAC = "00_MScThesisPlotter/Event_Selection_Sim/01_Electron_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + ElectronIDFolderSimAC).c_str());

    const string ProtonIDFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/02_Proton_ID_Sim";
    system(("mkdir -p " + ProtonIDFolderSim).c_str());

    const string ProtonIDFolderSimBC = "00_MScThesisPlotter/Event_Selection_Sim/02_Proton_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + ProtonIDFolderSimBC).c_str());

    const string ProtonIDFolderSimAC = "00_MScThesisPlotter/Event_Selection_Sim/02_Proton_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + ProtonIDFolderSimAC).c_str());

    const string NeutralsFDIDFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/03_Neutrals_FD_ID_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSim).c_str());

    const string NeutralsFDIDFolderSimBC = "00_MScThesisPlotter/Event_Selection_Sim/03_Neutrals_FD_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSimBC).c_str());

    const string NeutralsFDIDFolderSimAC = "00_MScThesisPlotter/Event_Selection_Sim/03_Neutrals_FD_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSimAC).c_str());

    const string PionsIDFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/04_Pions_ID_Sim";
    system(("mkdir -p " + PionsIDFolderSim).c_str());

    const string PionsIDFolderSimBC = "00_MScThesisPlotter/Event_Selection_Sim/04_Pions_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + PionsIDFolderSimBC).c_str());

    const string PionsIDFolderSimAC = "00_MScThesisPlotter/Event_Selection_Sim/04_Pions_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + PionsIDFolderSimAC).c_str());

    const string OtherPartIDFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/05_Other_part_ID_Sim";
    system(("mkdir -p " + OtherPartIDFolderSim).c_str());

    const string OtherPartIDFolderSimBC = "00_MScThesisPlotter/Event_Selection_Sim/05_Other_part_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + OtherPartIDFolderSimBC).c_str());

    const string OtherPartIDFolderSimAC = "00_MScThesisPlotter/Event_Selection_Sim/05_Other_part_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + OtherPartIDFolderSimAC).c_str());

    const string AMapsFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/06_AMaps";
    system(("mkdir -p " + AMapsFolderSim).c_str());

    const string AMapsFolderMapsSim = "00_MScThesisPlotter/Event_Selection_Sim/06_AMaps/01_Maps";
    system(("mkdir -p " + AMapsFolderMapsSim).c_str());

    const string AMapsFolderMomentumPlotsSim = "00_MScThesisPlotter/Event_Selection_Sim/06_AMaps/02_Momentum_Plots";
    system(("mkdir -p " + AMapsFolderMomentumPlotsSim).c_str());

    const string EfficiencyFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/07_Efficiency";
    system(("mkdir -p " + EfficiencyFolderSim).c_str());

    const string EfficiencyRegFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/07_Efficiency/01_No_FC";
    system(("mkdir -p " + EfficiencyRegFolderSim).c_str());

    const string EfficiencyWithFCFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/07_Efficiency/02_wFC";
    system(("mkdir -p " + EfficiencyWithFCFolderSim).c_str());

    const string MomResFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/08_MomRes";
    system(("mkdir -p " + MomResFolderSim).c_str());

    const string MomResS1FolderSim = "00_MScThesisPlotter/Event_Selection_Sim/08_MomRes/01_momResS1_Calc";
    system(("mkdir -p " + MomResS1FolderSim).c_str());

    const string MomResS2FolderSim = "00_MScThesisPlotter/Event_Selection_Sim/08_MomRes/02_momResS2_Calc";
    system(("mkdir -p " + MomResS2FolderSim).c_str());

    const string MomResS2RTFolderSim = "00_MScThesisPlotter/Event_Selection_Sim/08_MomRes/03_momResS2RT_Calc";
    system(("mkdir -p " + MomResS2RTFolderSim).c_str());
    //</editor-fold>

    //<editor-fold desc="Results">
    const string ResultsFolder = "00_MScThesisPlotter/Results";
    system(("rm -r " + ResultsFolder).c_str());
    system(("mkdir -p " + ResultsFolder).c_str());

    //<editor-fold desc="Event_Selection_Data">
    const string ElectronIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data";
    system(("mkdir -p " + ElectronIDFolderData).c_str());

    const string ElectronIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + ElectronIDFolderDataBC).c_str());

    const string ElectronIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + ElectronIDFolderDataAC).c_str());

    const string ProtonIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data";
    system(("mkdir -p " + ProtonIDFolderData).c_str());

    const string ProtonIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + ProtonIDFolderDataBC).c_str());

    const string ProtonIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + ProtonIDFolderDataAC).c_str());

    const string NeutralsFDIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data";
    system(("mkdir -p " + NeutralsFDIDFolderData).c_str());

    const string NeutralsFDIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + NeutralsFDIDFolderDataBC).c_str());

    const string NeutralsFDIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + NeutralsFDIDFolderDataAC).c_str());

    const string PionsIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data";
    system(("mkdir -p " + PionsIDFolderData).c_str());

    const string PionsIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + PionsIDFolderDataBC).c_str());

    const string PionsIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + PionsIDFolderDataAC).c_str());

    const string OtherPartIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data";
    system(("mkdir -p " + OtherPartIDFolderData).c_str());

    const string OtherPartIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + OtherPartIDFolderDataBC).c_str());

    const string OtherPartIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + OtherPartIDFolderDataAC).c_str());
    //</editor-fold>

    //<editor-fold desc="Final_State_Comparison">
    const string FinalStateCompFolderData = "00_MScThesisPlotter/Results/Final_State_Comp";
    system(("mkdir -p " + FinalStateCompFolderData).c_str());

    //<editor-fold desc="01_Momentum_comp">
    const string MomCompFolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp";
    system(("mkdir -p " + MomCompFolderData).c_str());

    const string MomComp_BSaC_1N_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/00_SaC_Results_Sim/01_Before_SaC_Sim";
    system(("mkdir -p " + MomComp_BSaC_1N_Sim_FolderData).c_str());

    const string MomComp_BSaC_1N_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/00_SaC_Results_Data/01_Before_SaC_Data";
    system(("mkdir -p " + MomComp_BSaC_1N_Data_FolderData).c_str());

    const string MomComp_ASaC_1N_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/00_SaC_Results_Sim/02_After_SaC_Sim";
    system(("mkdir -p " + MomComp_ASaC_1N_Sim_FolderData).c_str());

    const string MomComp_ASaC_1N_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/00_SaC_Results_Data/02_After_SaC_Data";
    system(("mkdir -p " + MomComp_ASaC_1N_Data_FolderData).c_str());

    const string MomComp_ElecMom_2N_Reco_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/01_Elec_Mom_Sim";
    system(("mkdir -p " + MomComp_ElecMom_2N_Reco_Sim_FolderData).c_str());

    const string MomComp_ElecMom_2N_Reco_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/01_Elec_Mom_Data";
    system(("mkdir -p " + MomComp_ElecMom_2N_Reco_Data_FolderData).c_str());

    const string MomComp_NucMom_2N_Reco_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/02_Nuc_Mom/01_Nuc_Mom_2N_Reco_Sim";
    system(("mkdir -p " + MomComp_NucMom_2N_Reco_Sim_FolderData).c_str());

    const string MomComp_NucMom_2N_Reco_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/02_Nuc_Mom/01_Nuc_Mom_2N_Reco_Data";
    system(("mkdir -p " + MomComp_NucMom_2N_Reco_Data_FolderData).c_str());

    const string MomComp_NucMom_2N_TL_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/02_Nuc_Mom/02_Nuc_Mom_2N_TL_Sim";
    system(("mkdir -p " + MomComp_NucMom_2N_TL_Sim_FolderData).c_str());

    const string MomComp_NucMom_1N_TL_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/02_Nuc_Mom/03_Nuc_Mom_1N_TL_Sim";
    system(("mkdir -p " + MomComp_NucMom_1N_TL_Sim_FolderData).c_str());

    const string MomComp_NucMom_1N_Reco_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/02_Nuc_Mom/03_Nuc_Mom_1N_Reco_Sim";
    system(("mkdir -p " + MomComp_NucMom_1N_Reco_Sim_FolderData).c_str());

    const string MomComp_NucMom_1N_Reco_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/02_Nuc_Mom/03_Nuc_Mom_1N_Reco_Data";
    system(("mkdir -p " + MomComp_NucMom_1N_Reco_Data_FolderData).c_str());

    const string MomComp_TotAndRel_Mom_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/03_Total_And_Rel_Mom_Sim";
    system(("mkdir -p " + MomComp_TotAndRel_Mom_Sim_FolderData).c_str());

    const string MomComp_TotAndRel_Mom_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/03_Total_And_Rel_Mom_Data";
    system(("mkdir -p " + MomComp_TotAndRel_Mom_Data_FolderData).c_str());

    const string MomComp_P_miss_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/04_P_miss_Sim";
    system(("mkdir -p " + MomComp_P_miss_Sim_FolderData).c_str());

    const string MomComp_P_miss_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/01_Momentum_comp/04_P_miss_Data";
    system(("mkdir -p " + MomComp_P_miss_Data_FolderData).c_str());
    //</editor-fold>

    //<editor-fold desc="02_Inv_mass_comp">
    const string InvMassCompFolderData = "00_MScThesisPlotter/Results/Final_State_Comp/02_Inv_mass_comp";
    system(("mkdir -p " + InvMassCompFolderData).c_str());

    const string InvMassComp_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/02_Inv_mass_comp/02_Inv_mass_comp_Sim";
    system(("mkdir -p " + InvMassComp_Sim_FolderData).c_str());

    const string InvMassComp_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/02_Inv_mass_comp/02_Inv_mass_comp_Data";
    system(("mkdir -p " + InvMassComp_Data_FolderData).c_str());
    //</editor-fold>

    //<editor-fold desc="03_Angular_dist_comp">
    const string AngDistCompFolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp";
    system(("mkdir -p " + AngDistCompFolderData).c_str());

    const string AngDistComp_Scattring_Ang_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/01_Scattring_Ang_Sim";
    system(("mkdir -p " + AngDistComp_Scattring_Ang_Sim_FolderData).c_str());

    const string AngDistComp_Scattring_Ang_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/01_Scattring_Ang_Data";
    system(("mkdir -p " + AngDistComp_Scattring_Ang_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_P_FD_and_P_CD_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/02_Opening_Ang__P_FD_and_P_CD__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_P_FD_and_P_CD_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_P_FD_and_P_CD_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/02_Opening_Ang__P_FD_and_P_CD__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_P_FD_and_P_CD_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_P_e_and_P_tot_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/03_Opening_Ang__P_e_and_P_tot__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_P_e_and_P_tot_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_P_e_and_P_tot_Data_FolderData =
            "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/03_Opening_Ang__P_e_and_P_tot__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_P_e_and_P_tot_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_tot_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/04_Opening_Ang__q_and_P_tot__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_tot_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_tot_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/04_Opening_Ang__q_and_P_tot__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_tot_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_L_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/05_Opening_Ang__q_and_P_L__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_L_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_L_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/05_Opening_Ang__q_and_P_L__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_L_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_R_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/06_Opening_Ang__q_and_P_R__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_R_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_R_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/06_Opening_Ang__q_and_P_R__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_R_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_P_L_minus_q_and_P_R_Sim_FolderData =
            "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/07_Opening_Ang__P_L_minus_q_and_P_R__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_P_L_minus_q_and_P_R_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_P_L_minus_q_and_P_R_Data_FolderData =
            "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/07_Opening_Ang__P_L_minus_q_and_P_R__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_P_L_minus_q_and_P_R_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_nucFD_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/08_Opening_Ang__q_and_P_nucFD__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_nucFD_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_nucFD_Data_FolderData =
            "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/08_Opening_Ang__q_and_P_nucFD__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_nucFD_Data_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_nucCD_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/09_Opening_Ang__q_and_P_nucCD__Sim";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_nucCD_Sim_FolderData).c_str());

    const string AngDistComp_Opening_Ang_q_and_P_nucCD_Data_FolderData =
            "00_MScThesisPlotter/Results/Final_State_Comp/03_Angular_dist_comp/09_Opening_Ang__q_and_P_nucCD__Data";
    system(("mkdir -p " + AngDistComp_Opening_Ang_q_and_P_nucCD_Data_FolderData).c_str());
    //</editor-fold>

    //<editor-fold desc="04_Energy_dist_comp">
    const string EnergyCompFolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp";
    system(("mkdir -p " + EnergyCompFolderData).c_str());

    const string EnergyComp_E_e_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp/01_E_e_Sim";
    system(("mkdir -p " + EnergyComp_E_e_Sim_FolderData).c_str());

    const string EnergyComp_E_e_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp/01_E_e_Data";
    system(("mkdir -p " + EnergyComp_E_e_Data_FolderData).c_str());

    const string EnergyComp_Omega_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp/02_Energy_Trans_Sim";
    system(("mkdir -p " + EnergyComp_Omega_Sim_FolderData).c_str());

    const string EnergyComp_Omega_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp/02_Energy_Trans_Data";
    system(("mkdir -p " + EnergyComp_Omega_Data_FolderData).c_str());

    const string EnergyComp_E_cal_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp/03_E_cal_Sim";
    system(("mkdir -p " + EnergyComp_E_cal_Sim_FolderData).c_str());

    const string EnergyComp_E_cal_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/04_Energy_dist_comp/03_E_cal_Data";
    system(("mkdir -p " + EnergyComp_E_cal_Data_FolderData).c_str());
    //</editor-fold>

    //<editor-fold desc="05_TKI_comp">
    const string TKICompFolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp";
    system(("mkdir -p " + TKICompFolderData).c_str());

    const string TKIComp_dP_T_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp/01_dP_T_Sim";
    system(("mkdir -p " + TKIComp_dP_T_Sim_FolderData).c_str());

    const string TKIComp_dP_T_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp/01_dP_T_Data";
    system(("mkdir -p " + TKIComp_dP_T_Data_FolderData).c_str());

    const string TKIComp_dAlpha_T_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp/02_dAlpha_T_Sim";
    system(("mkdir -p " + TKIComp_dAlpha_T_Sim_FolderData).c_str());

    const string TKIComp_dAlpha_T_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp/02_dAlpha_T_Data";
    system(("mkdir -p " + TKIComp_dAlpha_T_Data_FolderData).c_str());

    const string TKIComp_dP_T_vs_dAlpha_T_Sim_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp/03_dP_T_vs_dAlpha_T_Sim";
    system(("mkdir -p " + TKIComp_dP_T_vs_dAlpha_T_Sim_FolderData).c_str());

    const string TKIComp_dP_T_vs_dAlpha_T_Data_FolderData = "00_MScThesisPlotter/Results/Final_State_Comp/05_TKI_comp/03_dP_T_vs_dAlpha_T_Data";
    system(("mkdir -p " + TKIComp_dP_T_vs_dAlpha_T_Data_FolderData).c_str());
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    TCanvas *Canv;

#if PosterMode
    Canv = new TCanvas("c1", "c2", utilities.GetStandardCanvasWidth() * 4, 850 * 4); // Double resolution for poster
#else
    Canv = new TCanvas("c1", "c2", utilities.GetStandardCanvasWidth() * 2, utilities.GetStandardCanvasHeight() * 2); // Normal resolution for poster
#endif


#if PosterMode
    Canv->cd()->SetGrid(), Canv->cd()->SetBottomMargin(0.14), Canv->cd()->SetLeftMargin(0.20), Canv->cd()->SetRightMargin(0.05), Canv->cd()->SetTopMargin(0.12), Canv->cd();
//    Canv->cd()->SetGrid(), Canv->cd()->SetBottomMargin(0.14), Canv->cd()->SetLeftMargin(0.16), Canv->cd()->SetRightMargin(0.01), Canv->cd()->SetTopMargin(0.12), Canv->cd();
#else
    Canv->cd()->SetGrid(), Canv->cd()->SetBottomMargin(0.14), Canv->cd()->SetLeftMargin(0.16), Canv->cd()->SetRightMargin(0.16), Canv->cd()->SetTopMargin(0.12), Canv->cd();
#endif

    TList *MScThesisPlots = new TList();
    string listName = EventSelectionSimFolder + "/" + SampleName + ".root";
    const char *TListName = listName.c_str();

//    //<editor-fold desc="Preselection">
//    cout << "\n\n\nPlotting preselection cut plots\n";
//
//    //<editor-fold desc="Vertex plots">
//    cout << "\n\n\nPlotting vertex cuts plots\n";
//
//    /* Before cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{e} (1e cut)", SampleName, VertexCutsFolderSimBC, "01_Vz_e_FD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{p} (1e cut, FD)", SampleName, VertexCutsFolderSimBC, "02_Vz_p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{p} (1e cut, CD)", SampleName, VertexCutsFolderSimBC, "02_Vz_p_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{D} (1e cut, FD)", SampleName, VertexCutsFolderSimBC, "03_Vz_D_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{D} (1e cut, CD)", SampleName, VertexCutsFolderSimBC, "03_Vz_D_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{#pi^{+}} (1e cut, FD)", SampleName, VertexCutsFolderSimBC, "04_Vz_pip_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{#pi^{+}} (1e cut, CD)", SampleName, VertexCutsFolderSimBC, "04_Vz_pip_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{#pi^{-}} (1e cut, FD)", SampleName, VertexCutsFolderSimBC, "05_Vz_pim_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{#pi^{-}} (1e cut, CD)", SampleName, VertexCutsFolderSimBC, "05_Vz_pim_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{K^{+}} (1e cut, FD)", SampleName, VertexCutsFolderSimBC, "06_Vz_Kp_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{K^{+}} (1e cut, CD)", SampleName, VertexCutsFolderSimBC, "06_Vz_Kp_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{K^{-}} (1e cut, FD)", SampleName, VertexCutsFolderSimBC, "07_Vz_Km_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "V_{z}^{K^{-}} (1e cut, CD)", SampleName, VertexCutsFolderSimBC, "07_Vz_Km_CD");
//
//    /* After cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{e} (1e cut)", SampleName, VertexCutsFolderSimAC, "01_Vz_e_FD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{p} (1e cut, FD)", SampleName, VertexCutsFolderSimAC, "02_Vz_p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{p} (1e cut, CD)", SampleName, VertexCutsFolderSimAC, "02_Vz_p_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{D} (1e cut, FD)", SampleName, VertexCutsFolderSimAC, "03_Vz_D_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{D} (1e cut, CD)", SampleName, VertexCutsFolderSimAC, "03_Vz_D_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{#pi^{+}} (1e cut, FD)", SampleName, VertexCutsFolderSimAC, "04_Vz_pip_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{#pi^{+}} (1e cut, CD)", SampleName, VertexCutsFolderSimAC, "04_Vz_pip_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{#pi^{-}} (1e cut, FD)", SampleName, VertexCutsFolderSimAC, "05_Vz_pim_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{#pi^{-}} (1e cut, CD)", SampleName, VertexCutsFolderSimAC, "05_Vz_pim_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{K^{+}} (1e cut, FD)", SampleName, VertexCutsFolderSimAC, "06_Vz_Kp_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{K^{+}} (1e cut, CD)", SampleName, VertexCutsFolderSimAC, "06_Vz_Kp_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{K^{-}} (1e cut, FD)", SampleName, VertexCutsFolderSimAC, "07_Vz_Km_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "V_{z}^{K^{-}} (1e cut, CD)", SampleName, VertexCutsFolderSimAC, "07_Vz_Km_CD");
//    //</editor-fold>
//
//    //<editor-fold desc="Vertex correlation plots">
//    cout << "\n\n\nPlotting vertex correlation cuts plots\n";
//
//    /* Before cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{p}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimBC, "01_dVz_p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{p}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimBC, "01_dVz_p_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{D}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimBC, "02_dVz_D_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{D}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimBC, "02_dVz_D_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimBC, "03_dVz_pip_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimBC, "03_dVz_pip_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimBC, "04_dVz_pim_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimBC, "04_dVz_pim_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{K^{+}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimBC, "05_dVz_Kp_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{K^{+}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimBC, "05_dVz_Kp_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{K^{-}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimBC, "06_dVz_Km_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NO_CUTS, "Vertex corr. dV^{K^{-}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimBC, "06_dVz_Km_CD");
//
//    /* After cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{p}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimAC, "01_dVz_p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{p}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimAC, "01_dVz_p_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{D}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimAC, "02_dVz_D_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{D}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimAC, "02_dVz_D_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimAC, "03_dVz_pip_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimAC, "03_dVz_pip_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimAC, "04_dVz_pim_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimAC, "04_dVz_pim_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{K^{+}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimAC, "05_dVz_Kp_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{K^{+}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimAC, "05_dVz_Kp_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{K^{-}}_{z} (1e cut, FD)", SampleName, VertexCorrCutsFolderSimAC, "06_dVz_Km_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "Vertex corr. dV^{K^{-}}_{z} (1e cut, CD)", SampleName, VertexCorrCutsFolderSimAC, "06_dVz_Km_CD");
//    //</editor-fold>
//
//    //<editor-fold desc="DC fiducial cuts">
//    cout << "\n\n\nPlotting vertex DC fiducial cuts plots\n";
//
//    /* Before cuts */
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_1", SampleName, DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R1");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_2", SampleName, DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R2");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_3", SampleName, DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R3");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_proton_1", SampleName, DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R1");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_proton_2", SampleName, DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R2");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_proton_3", SampleName, DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R3");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_pion_1", SampleName, DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R1");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_pion_2", SampleName, DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R2");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NO_CUTS_DC, "dc_hitmap_before_pion_3", SampleName, DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R3");
//
//    /* After cuts */
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_1", SampleName, DCFiducialCutsFolderSimAC, "01_dc_hitmap_e_R1");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_2", SampleName, DCFiducialCutsFolderSimAC, "01_dc_hitmap_e_R2");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_3", SampleName, DCFiducialCutsFolderSimAC, "01_dc_hitmap_e_R3");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_proton_1", SampleName, DCFiducialCutsFolderSimAC, "02_dc_hitmap_p_R1");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_proton_2", SampleName, DCFiducialCutsFolderSimAC, "02_dc_hitmap_p_R2");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_proton_3", SampleName, DCFiducialCutsFolderSimAC, "02_dc_hitmap_p_R3");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_pion_1", SampleName, DCFiducialCutsFolderSimAC, "03_dc_hitmap_pi_R1");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_pion_2", SampleName, DCFiducialCutsFolderSimAC, "03_dc_hitmap_pi_R2");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only_DC, "dc_hitmap_after_pion_3", SampleName, DCFiducialCutsFolderSimAC, "03_dc_hitmap_pi_R3");
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //<editor-fold desc="Electron ID">
//    cout << "\n\n\nPlotting Electron ID plots\n";
//
//    /* Before cuts */
//    HistPlotter2D(Canv, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana, "#beta vs. P (all particles, no #(e) cut, CD)", SampleName, ElectronIDFolderSimBC,
//                  "01_Beta_vs_P_CD_all_par_no_cuts");
//    HistPlotter2D(Canv, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana, "#beta vs. P (all particles, no #(e) cut, FD)", SampleName, ElectronIDFolderSimBC,
//                  "01_Beta_vs_P_FD_all_par_no_cuts");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only, "#beta vs. P (all particles, 1e cut, CD)", SampleName, ElectronIDFolderSimBC,
//                  "02_Beta_vs_P_CD_all_par_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only, "#beta vs. P (all particles, 1e cut, FD)", SampleName, ElectronIDFolderSimBC,
//                  "02_Beta_vs_P_FD_all_par_1e_cut");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Preselection_only, "N_{phe} in HTCC BC (1e cut)", SampleName, ElectronIDFolderSimBC, "03a_Nphe_in_HTCC_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only, "SF vs. P_{e} BC (1e cut, FD)", SampleName, ElectronIDFolderSimBC, "03_SF_vs_P_FD_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only, "Vcal vs. SF BC (1e cut, PCAL)", SampleName, ElectronIDFolderSimBC, "04_Vcal_vs_SF_FD_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only, "Wcal vs. SF BC (1e cut, PCAL)", SampleName, ElectronIDFolderSimBC, "04_Wcal_vs_FS_FD_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_Preselection_only, "#beta vs. P (electrons only, 1e cut)", SampleName, ElectronIDFolderSimBC,
//                  "05_Beta_e_vs_P_FD_electrons_only_1e_cut");
//
//    /* After cuts */
//    HistPlotter2D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "#beta vs. P (all particles, 1e cut, CD)", SampleName, ElectronIDFolderSimAC,
//                  "02_Beta_vs_P_CD_all_par_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "#beta vs. P (all particles, 1e cut, FD)", SampleName, ElectronIDFolderSimAC,
//                  "02_Beta_vs_P_FD_all_par_1e_cut");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "N_{phe} in HTCC BC (1e cut)", SampleName, ElectronIDFolderSimAC, "03a_Nphe_in_HTCC_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "SF vs. P_{e} BC (1e cut, FD)", SampleName, ElectronIDFolderSimAC, "03_SF_vs_P_FD_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "Vcal vs. SF BC (1e cut, PCAL)", SampleName, ElectronIDFolderSimAC, "04_Vcal_vs_SF_FD_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "Wcal vs. SF BC (1e cut, PCAL)", SampleName, ElectronIDFolderSimAC, "04_Wcal_vs_FS_FD_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_PreselectionAndeQC_only, "#beta vs. P (electrons only, 1e cut)", SampleName, ElectronIDFolderSimAC,
//                  "05_Beta_e_vs_P_FD_electrons_only_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#beta vs. P (all particles, 1e cut, CD)", SampleName, ElectronIDFolderSimAC, "06_Beta_vs_P_CD_all_par_all_cuts_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#beta vs. P (all particles, 1e cut, FD)", SampleName, ElectronIDFolderSimAC, "06_Beta_vs_P_FD_all_par_all_cuts_1e_cut");
//    //</editor-fold>
//
//    //<editor-fold desc="Proton ID">
//    cout << "\n\n\nPlotting Proton ID plots\n";
//
//    /* Before cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{p} (1e cut, FD)", SampleName, ProtonIDFolderSimBC, "01_chi2_p_FD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{p} (1e cut, CD)", SampleName, ProtonIDFolderSimBC, "01_chi2_p_CD_1e_cut");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "FD Proton momentum #epsilon_{eff} (1e cut)", SampleName, ProtonIDFolderSimBC, "02_P_pFD_eff_1e_cut_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "CD Proton momentum #epsilon_{eff} (1e cut)", SampleName, ProtonIDFolderSimBC, "02_P_pCD_eff_1e_cut_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_noNC, "#theta_{p_{1},p_{2}} (All Int., 2p)", SampleName, ProtonIDFolderSimBC, "02_Theta_p1_p2_2p");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName, ProtonIDFolderSimBC, "03_theta_p1_p2_vd_dToF_2p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName, ProtonIDFolderSimBC,
//                  "03_theta_p1_p2_vd_dPos_2p");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)", SampleName, ProtonIDFolderSimBC,
//                  "04_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
////    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName, ProtonIDFolderSimBC,
////                  "04_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_noNC, "#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN", SampleName, ProtonIDFolderSimBC,
//                  "05_dPhi_pFD_pCD_for_small_dTheta_pFD_pCD_2p");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_woChi2, "P_{p} vs. #theta_{p} (1e cut, FD)", SampleName, ProtonIDFolderSimBC, "06_P_p_vs_Theta_p_1e_cut_FD");
//    HistPlotter2D(Canv, MScThesisPlots, plots_woChi2, "P_{p} vs. #theta_{p} (1e cut, CD)", SampleName, ProtonIDFolderSimBC, "06_P_p_vs_Theta_p_1e_cut_CD");
//
//    /* After cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "#theta_{p_{1},p_{2}} (All Int., 2p)", SampleName, ProtonIDFolderSimAC, "02_Theta_p1_p2_2p");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName, ProtonIDFolderSimAC, "03_theta_p1_p2_vd_dToF_2p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName, ProtonIDFolderSimAC,
//                  "03_theta_p1_p2_vd_dPos_2p");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)", SampleName, ProtonIDFolderSimAC,
//                  "04_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
////    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName, ProtonIDFolderSimAC,
////                  "04_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "P_{p} vs. #theta_{p} (1e cut, FD)", SampleName, ProtonIDFolderSimBC, "06_P_p_vs_Theta_p_1e_cut_FD");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "P_{p} vs. #theta_{p} (1e cut, CD)", SampleName, ProtonIDFolderSimBC, "06_P_p_vs_Theta_p_1e_cut_CD");
//    //</editor-fold>
//
//    //<editor-fold desc="Neutrals ID">
//    cout << "\n\n\nPlotting Neutrals ID plots\n";
//
//    /* ECAL veto before cuts */
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} BV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimBC,
//                  "01_ECAL_veto_LnFD_e_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} BV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimBC,
//                  "01_ECAL_veto_LnFD_pDF_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimBC,
//                  "02_ECAL_veto_nFD_e_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_noNC, "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} BV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimBC,
//                  "02_ECAL_veto_nFD_pFD_1e_cut");
//
//    /* ECAL veto after cuts */
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} AV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "01_ECAL_veto_LnFD_e_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} AV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "01_ECAL_veto_LnFD_pDF_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "02_ECAL_veto_nFD_e_1e_cut");
//    HistPlotter2D(Canv, MScThesisPlots, plots_NC, "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} AV (1e cut, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "02_ECAL_veto_nFD_pFD_1e_cut");
//
//    /* Before nFD momentum threshold */
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "FD neutron momentum BPID (1e_cut, FD)", SampleName, NeutralsFDIDFolderSimBC, "03a_P_nFD_BPID_1e_cut_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "FD neutron momentum BPID - ZOOMOUT (1e_cut, FD)", SampleName, NeutralsFDIDFolderSimBC,
//                  "03b_P_nFD_ZOOMOUT_BPID_1e_cut_FD");
//
//    /* After nFD momentum threshold */
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "FD neutron momentum APID (1e_cut, FD)", SampleName, NeutralsFDIDFolderSimAC, "03a_P_nFD_APID_1e_cut_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "FD neutron momentum APID - ZOOMOUT (1e_cut, FD)", SampleName, NeutralsFDIDFolderSimAC, "03b_P_nFD_ZOOMOUT_APID_1e_cut_FD");
//
////    /* Redef. FD neutron multiplicity BPID */
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&BV (1p, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "04a_FD_neut_Multi_BPID_BV_1p_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&AV (1p, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "04b_FD_neut_Multi_BPID_AV_1p_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&BV (1n, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "04c_FD_neut_Multi_BPID_BV_1n_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&AV (1n, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "04d_FD_neut_Multi_BPID_AV_1n_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&BV (pFDpCD, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "05a_FD_neut_Multi_BPID_BV_pFDpCD_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&AV (pFDpCD, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "05b_FD_neut_Multi_BPID_AV_pFDpCD_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&BV (nFDpCD, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "05c_FD_neut_Multi_BPID_BV_nFDpCD_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "Multiplicity of FD neutrons by redef. BPID&AV (nFDpCD, FD)", SampleName, NeutralsFDIDFolderSimBC,
////                  "05d_FD_neut_Multi_BPID_AV_nFDpCD_FD");
//
//    /* Redef. FD neutron multiplicity APID */
////    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&BV (1p, FD)", SampleName, NeutralsFDIDFolderSimAC,
////                  "04a_FD_neut_Multi_APID_BV_1p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&AV (1p, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "04b_FD_neut_Multi_APID_AV_1p_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&BV (1n, FD)", SampleName, NeutralsFDIDFolderSimAC,
////                  "04c_FD_neut_Multi_APID_BV_1n_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&AV (1n, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "04d_FD_neut_Multi_APID_AV_1n_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&BV (pFDpCD, FD)", SampleName, NeutralsFDIDFolderSimAC,
////                  "05a_FD_neut_Multi_APID_BV_pFDpCD_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&AV (pFDpCD, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "05b_FD_neut_Multi_APID_AV_pFDpCD_FD");
////    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&BV (nFDpCD, FD)", SampleName, NeutralsFDIDFolderSimAC,
////                  "05c_FD_neut_Multi_APID_BV_nFDpCD_FD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "Multiplicity of FD neutrons by redef. APID&AV (nFDpCD, FD)", SampleName, NeutralsFDIDFolderSimAC,
//                  "05d_FD_neut_Multi_APID_AV_nFDpCD_FD");
//    //</editor-fold>
//
//    //<editor-fold desc="Pions ID">
//    cout << "\n\n\nPlotting Pions ID plots\n";
//
//    /* Before cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{#pi^{+}} (1e cut, FD)", SampleName, PionsIDFolderSimBC, "01_chi2_pip_FD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{#pi^{+}} (1e cut, CD)", SampleName, PionsIDFolderSimBC, "01_chi2_pip_CD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{#pi^{-}} (1e cut, FD)", SampleName, PionsIDFolderSimBC, "02_chi2_pim_FD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{#pi^{-}} (1e cut, CD)", SampleName, PionsIDFolderSimBC, "02_chi2_pim_CD_1e_cut");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_woChi2, "P_{#pi^{+}} vs. #theta_{#pi^{+}} (1e cut, FD)", SampleName, PionsIDFolderSimBC, "05_P_pip_vs_Theta_pip_1e_cut_FD");
//    HistPlotter2D(Canv, MScThesisPlots, plots_woChi2, "P_{#pi^{+}} vs. #theta_{#pi^{+}} (1e cut, CD)", SampleName, PionsIDFolderSimBC, "05_P_pip_vs_Theta_pip_1e_cut_CD");
//    HistPlotter2D(Canv, MScThesisPlots, plots_woChi2, "P_{#pi^{-}} vs. #theta_{#pi^{-}} (1e cut, FD)", SampleName, PionsIDFolderSimBC, "05_P_pim_vs_Theta_pim_1e_cut_FD");
//    HistPlotter2D(Canv, MScThesisPlots, plots_woChi2, "P_{#pi^{-}} vs. #theta_{#pi^{-}} (1e cut, CD)", SampleName, PionsIDFolderSimBC, "05_P_pim_vs_Theta_pim_1e_cut_CD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "CD & FD Piplus momentum #epsilon_{eff} (1e cut)", SampleName, PionsIDFolderSimBC, "03_P_pip_eff_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_NC, "CD & FD Piminus momentum #epsilon_{eff} (1e cut)", SampleName, PionsIDFolderSimBC, "04_P_pim_eff_1e_cut");
//    //</editor-fold>
//
//    //<editor-fold desc="Other particles ID">
//    cout << "\n\n\nPlotting Other particles ID plots\n";
//
//    /* Before cuts */
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{D} (1e cut, FD)", SampleName, OtherPartIDFolderSimBC, "01_chi2_D_FD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{D} (1e cut, CD)", SampleName, OtherPartIDFolderSimBC, "01_chi2_D_CD_1e_cut");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{K^{+}} (1e cut, FD)", SampleName, OtherPartIDFolderSimBC, "01_chi2_Kp_FD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{K^{+}} (1e cut, CD)", SampleName, OtherPartIDFolderSimBC, "01_chi2_Kp_CD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{K^{-}} (1e cut, FD)", SampleName, OtherPartIDFolderSimBC, "02_chi2_Km_FD_1e_cut");
//    HistPlotter1D(Canv, MScThesisPlots, plots_woChi2, "#chi^{2}_{K^{-}} (1e cut, CD)", SampleName, OtherPartIDFolderSimBC, "02_chi2_Km_CD_1e_cut");
//    //</editor-fold>
//
//    //<editor-fold desc="AMaps">
//    cout << "\n\n\nPlotting AMaps plots\n";
//
//    string TL_root_file_prefix = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
//    string Reco_root_file_prefix = "02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root";
//    string Ratio_root_file_prefix = "03_AMap_Ratio_-_C12x4_simulation_G18_Q204_6GeV.root";
//    string cPart_Sep_AMaps_root_file_prefix = "04_cPart_Sep_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";
//    string Finalized_AMaps_root_file_prefix = "05_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";
//
//    AMapsPlotter(plots_AMaps_plotsPath, true, (AMapsFolderMapsSim + "/"));
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_AMaps, "Reco P_{e} used in AMaps (1e cut, FD)", SampleName, AMapsFolderMomentumPlotsSim, "01_reco_P_e");
//    HistPlotter1D(Canv, MScThesisPlots, plots_AMaps, "TL P_{e} used in AMaps (1e cut, FD)", SampleName, AMapsFolderMomentumPlotsSim, "01_TL_P_e");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_AMaps, "Reco P_{pFD} used in AMaps (1e cut, FD)", SampleName, AMapsFolderMomentumPlotsSim, "02_reco_P_pFD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_AMaps, "TL P_{pFD} used in AMaps (1e cut, FD)", SampleName, AMapsFolderMomentumPlotsSim, "02_TL_P_pFD");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_AMaps, "Reco P_{nFD} used in AMaps (1e cut, FD)", SampleName, AMapsFolderMomentumPlotsSim, "03_reco_P_nFD");
//    HistPlotter1D(Canv, MScThesisPlots, plots_AMaps, "TL P_{nFD} used in AMaps (1e cut, FD)", SampleName, AMapsFolderMomentumPlotsSim, "03_TL_P_nFD");
//    //</editor-fold>
//
//    //<editor-fold desc="Efficiency">
//    cout << "\n\n\nPlotting Efficiency plots\n";
//
//    /* No fiducial cuts & with momentum th. */
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "Electron momentum #epsilon_{eff} (1p)", SampleName, EfficiencyRegFolderSim, "01_electron_mom_eff_1p_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "Electron theta #epsilon_{eff} (1p)", SampleName, EfficiencyRegFolderSim, "01_electron_theta_eff_1p_wMomTh");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "FD Proton momentum #epsilon_{eff} (1p)", SampleName, EfficiencyRegFolderSim, "02_proton_mom_eff_1p_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "Proton theta #epsilon_{eff} (1p)", SampleName, EfficiencyRegFolderSim, "02_proton_theta_eff_1p_wMomTh");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "Electron momentum #epsilon_{eff} (1n)", SampleName, EfficiencyRegFolderSim, "03_electron_mom_eff_1n_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "Electron theta #epsilon_{eff} (1n)", SampleName, EfficiencyRegFolderSim, "03_electron_theta_eff_1n_wMomTh");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "FD Leading neutron momentum #epsilon_{eff} (1n)", SampleName, EfficiencyRegFolderSim,
//                  "04_neutron_mom_eff_1n_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_reg, "Neutron theta #epsilon_{eff} (1n)", SampleName, EfficiencyRegFolderSim, "04_neutron_theta_eff_1n_wMomTh");
//
//    /* With fiducial cuts & with momentum th. */
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "Electron momentum #epsilon_{eff} (1p)", SampleName, EfficiencyWithFCFolderSim,
//                  "01_electron_mom_eff_1p_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "Electron theta #epsilon_{eff} (1p)", SampleName, EfficiencyWithFCFolderSim,
//                  "01_electron_theta_eff_1p_wMomTh");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "FD Proton momentum #epsilon_{eff} (1p)", SampleName, EfficiencyWithFCFolderSim,
//                  "02_proton_mom_eff_1p_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "Proton theta #epsilon_{eff} (1p)", SampleName, EfficiencyWithFCFolderSim,
//                  "02_proton_theta_eff_1p_wMomTh");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "Electron momentum #epsilon_{eff} (1n)", SampleName, EfficiencyWithFCFolderSim,
//                  "03_electron_mom_eff_1n_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "Electron theta #epsilon_{eff} (1n)", SampleName, EfficiencyWithFCFolderSim,
//                  "03_electron_theta_eff_1n_wMomTh");
//
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "FD Leading neutron momentum #epsilon_{eff} (1n)", SampleName, EfficiencyWithFCFolderSim,
//                  "04_neutron_mom_eff_1n_wMomTh");
//    HistPlotter1D(Canv, MScThesisPlots, plots_Efficiency_wFC_wMomTh, "Neutron theta #epsilon_{eff} (1n)", SampleName, EfficiencyWithFCFolderSim,
//                  "04_neutron_theta_eff_1n_wMomTh");
//    //</editor-fold>
//
//    //<editor-fold desc="momRes">
//    cout << "\n\n\nPlotting Momentum Resolution plots\n";
//
//    //<editor-fold desc="MomResS1">
//    /* Mean fit before correction */
//    GraphPlotter1D(MScThesisPlots, plots_momResS1_FittedPlots, "Fitted reco neutron resolution slice width", "reco_f_Smear_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "01a_Neutron_reco_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS1_FittedPlots, "Fitted TL neutron resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "01b_Neutron_truth_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS1_FittedPlots, "Fitted reco neutron resolution slice mean", "reco_f_Corr_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "02a_Neutron_reco_f_Corr_pol1_wKC_Important"); // Important! (correction!)
//    GraphPlotter1D(MScThesisPlots, plots_momResS1_FittedPlots, "Fitted TL neutron resolution slice mean", "truth_f_Corr_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "02b_Neutron_truth_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS1_FittedPlots, "Fitted reco neutron resolution slice mean", "reco_f_Corr_pol1", SampleName, MomResS1FolderSim,
//                   "02c_Neutron_reco_f_Corr_pol1_noKC_Important"); // For KC explanation!
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{nFD} vs. P^{reco}_{nFD} (1n, FD)", SampleName, MomResS1FolderSim,
//                  "03a_P_nFD_Res_VS_Reco_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{nFD} vs. P^{truth}_{nFD} (1n, FD)", SampleName, MomResS1FolderSim,
//                  "03a_P_nFD_Res_VS_TL_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{nFD} vs. P^{reco}_{nFD} - ZOOMIN (1n, FD)", SampleName, MomResS1FolderSim,
//                  "03b_P_nFD_Res_VS_Reco_P_nFD_ZOOMIN_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{nFD} vs. P^{truth}_{nFD} - ZOOMIN (1n, FD)", SampleName, MomResS1FolderSim,
//                  "03b_P_nFD_Res_VS_TL_P_nFD_ZOOMIN_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{pFD} vs. P^{reco}_{pFD} (1p, FD)", SampleName, MomResS1FolderSim,
//                  "04a_P_pFD_Res_VS_Reco_P_pFD_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{pFD} vs. P^{truth}_{pFD} (1p, FD)", SampleName, MomResS1FolderSim,
//                  "04a_P_pFD_Res_VS_TL_P_pFD_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{pFD} vs. P^{reco}_{pFD} - ZOOMIN (1p, FD)", SampleName, MomResS1FolderSim,
//                  "04b_P_pFD_Res_VS_Reco_P_pFD_ZOOMIN_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "R_{pFD} vs. P^{truth}_{pFD} - ZOOMIN (1p, FD)", SampleName, MomResS1FolderSim,
//                  "04b_P_pFD_Res_VS_TL_P_pFD_ZOOMIN_1p");
//
//    /* Resolution match multiplicity (neutron) */
//    HistPlotter1D(Canv, MScThesisPlots, plots_momResS1_Gen, "Neutron resolution match multiplicity (1n, FD)", SampleName, MomResS1FolderSim,
//                  "05a_Neut_nRes_match_multi_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "Match multiplicity vs. P^{reco}_{nFD} (1n, FD)", SampleName, MomResS1FolderSim,
//                  "05b_Neut_nRes_match_multi_vs_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS1_Gen, "Match multiplicity vs. #phi^{reco}_{nFD} (1n, FD)", SampleName, MomResS1FolderSim,
//                  "05c_Neut_nRes_match_multi_vs_phi_nFD_1n");
//    HistPlotter1D(Canv, MScThesisPlots, plots_momResS1_Gen, "TL P_{nFD} used in nRes (1n, FD)", SampleName, MomResS1FolderSim, "05d_matched_truth_P_nFD_1n");
//    HistPlotter1D(Canv, MScThesisPlots, plots_momResS1_Gen, "Reco P_{nFD} used in nRes (1n, FD)", SampleName, MomResS1FolderSim, "05e_matched_reco_P_nFD_1n");
//
//    GraphPlotter1D(MScThesisPlots, plots_pResS1_FittedPlots, "Fitted reco proton resolution slice width", "reco_f_Smear_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "06a_Proton_reco_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS1_FittedPlots, "Fitted TL proton resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "06b_Proton_truth_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS1_FittedPlots, "Fitted reco proton resolution slice mean", "reco_f_Corr_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "07a_Proton_reco_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS1_FittedPlots, "Fitted TL proton resolution slice mean", "truth_f_Corr_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "07b_Proton_truth_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS1_FittedPlots, "Fitted TL proton resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS1FolderSim,
//                   "07c_Proton_truth_f_Smear_pol1_wKC_sigma_pFD_test"); // For sigma_pFD before smearing!
//
//    /* Resplution slice fits (momResS1) */
//    MomResSlicePlotter(plots_momResS1_FittedPlots, "n", 7, 24, "reco", MomResS1FolderSim);
//    //</editor-fold>
//
//    //<editor-fold desc="MomResS2">
//    /* Width fit after correction (neutron) */
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted reco neutron resolution slice width", "reco_f_Smear_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "01a_Neutron_reco_f_Smear_pol1_wKC_Important"); // Important! (smearing!)
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted TL neutron resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "01b_Neutron_truth_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted reco neutron resolution slice mean", "reco_f_Corr_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "02a_Neutron_reco_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted TL neutron resolution slice mean", "truth_f_Corr_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "02b_Neutron_truth_f_Corr_pol1_wKC");
//
//    /* Width fit after correction (neutron) */
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted reco neutron resolution slice width", "reco_f_Smear_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "01_Neutron_reco_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted TL neutron resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "01_Neutron_truth_f_Smear_pol1_wKC_Important"); // Important! (smearing!)
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted reco neutron resolution slice mean", "reco_f_Corr_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "02_Neutron_reco_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_momResS2_FittedPlots, "Fitted TL neutron resolution slice mean", "truth_f_Corr_pol1_wKC", SampleName, MomResS2FolderSim,
//                   "02_Neutron_truth_f_Corr_pol1_wKC");
//
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{nFD} vs. P^{reco}_{nFD} (1n, FD)", SampleName, MomResS2FolderSim,
//                  "03a_P_nFD_Res_VS_Reco_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{nFD} vs. P^{truth}_{nFD} (1n, FD)", SampleName, MomResS2FolderSim,
//                  "03a_P_nFD_Res_VS_TL_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{nFD} vs. P^{reco}_{nFD} - ZOOMIN (1n, FD)", SampleName, MomResS2FolderSim,
//                  "03b_P_nFD_Res_VS_Reco_P_nFD_ZOOMIN_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{nFD} vs. P^{truth}_{nFD} - ZOOMIN (1n, FD)", SampleName, MomResS2FolderSim,
//                  "03b_P_nFD_Res_VS_TL_P_nFD_ZOOMIN_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{pFD} vs. P^{reco}_{pFD} (1p, FD)", SampleName, MomResS2FolderSim,
//                  "04a_P_pFD_Res_VS_Reco_P_pFD_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{pFD} vs. P^{truth}_{pFD} (1p, FD)", SampleName, MomResS2FolderSim,
//                  "04a_P_pFD_Res_VS_TL_P_pFD_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{pFD} vs. P^{reco}_{pFD} - ZOOMIN (1p, FD)", SampleName, MomResS2FolderSim,
//                  "04b_P_pFD_Res_VS_Reco_P_pFD_ZOOMIN_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2_Gen, "R_{pFD} vs. P^{truth}_{pFD} - ZOOMIN (1p, FD)", SampleName, MomResS2FolderSim,
//                  "04b_P_pFD_Res_VS_TL_P_pFD_ZOOMIN_1p");
//
//    /* Resplution slice fits (momResS2) */
//    MomResSlicePlotter(plots_momResS2_FittedPlots, "n", 7, 24, "reco", MomResS2FolderSim);
//    //</editor-fold>
//
//    //<editor-fold desc="MomResS2RT">
//    /* momRes test */
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{nFD} vs. P^{reco}_{nFD} (1n, FD)", SampleName, MomResS2RTFolderSim,
//                  "03a_P_nFD_Res_VS_Reco_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{nFD} vs. P^{truth}_{nFD} (1n, FD)", SampleName, MomResS2RTFolderSim,
//                  "03a_P_nFD_Res_VS_TL_P_nFD_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{nFD} vs. P^{reco}_{nFD} - ZOOMIN (1n, FD)", SampleName, MomResS2RTFolderSim,
//                  "03b_P_nFD_Res_VS_Reco_P_nFD_ZOOMIN_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{nFD} vs. P^{truth}_{nFD} - ZOOMIN (1n, FD)", SampleName, MomResS2RTFolderSim,
//                  "03b_P_nFD_Res_VS_TL_P_nFD_ZOOMIN_1n");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{pFD} vs. P^{reco}_{pFD} (1p, FD)", SampleName, MomResS2RTFolderSim,
//                  "04a_P_pFD_Res_VS_Reco_P_pFD_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{pFD} vs. P^{truth}_{pFD} (1p, FD)", SampleName, MomResS2RTFolderSim,
//                  "04a_P_pFD_Res_VS_TL_P_pFD_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{pFD} vs. P^{reco}_{pFD} - ZOOMIN (1p, FD)", SampleName, MomResS2RTFolderSim,
//                  "04b_P_pFD_Res_VS_Reco_P_pFD_ZOOMIN_1p");
//    HistPlotter2D(Canv, MScThesisPlots, plots_momResS2RT_Gen, "R_{pFD} vs. P^{truth}_{pFD} - ZOOMIN (1p, FD)", SampleName, MomResS2RTFolderSim,
//                  "04b_P_pFD_Res_VS_TL_P_pFD_ZOOMIN_1p");
//
//    GraphPlotter1D(MScThesisPlots, plots_pResS2RT_FittedPlots, "Fitted reco proton resolution slice width", "reco_f_Smear_pol1_wKC", SampleName, MomResS2RTFolderSim,
//                   "06a_Proton_reco_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS2RT_FittedPlots, "Fitted TL proton resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS2RTFolderSim,
//                   "06b_Proton_truth_f_Smear_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS2RT_FittedPlots, "Fitted reco proton resolution slice mean", "reco_f_Corr_pol1_wKC", SampleName, MomResS2RTFolderSim,
//                   "07a_Proton_reco_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS2RT_FittedPlots, "Fitted TL proton resolution slice mean", "truth_f_Corr_pol1_wKC", SampleName, MomResS2RTFolderSim,
//                   "07b_Proton_truth_f_Corr_pol1_wKC");
//    GraphPlotter1D(MScThesisPlots, plots_pResS2RT_FittedPlots2, "Fitted TL proton resolution slice width", "truth_f_Smear_pol1_wKC", SampleName, MomResS2RTFolderSim,
//                   "07c_Proton_truth_f_Smear_pol1_sigma_pFD_test_wKC"); // For sigma_pFD after smearing!
//    //</editor-fold>
//
//    //</editor-fold>

    //<editor-fold desc="Results">
    cout << "\n\n\nPlotting results plots\n";

//    //<editor-fold desc="Event_Selection_Data">
//    const string ElectronIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data";
//    system(("mkdir -p " + ElectronIDFolderData).c_str());
//
//    const string ElectronIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data/Before_Cuts_Data";
//    system(("mkdir -p " + ElectronIDFolderDataBC).c_str());
//
//    const string ElectronIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data/After_Cuts_Data";
//    system(("mkdir -p " + ElectronIDFolderDataAC).c_str());
//
//    const string ProtonIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data";
//    system(("mkdir -p " + ProtonIDFolderData).c_str());
//
//    const string ProtonIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data/Before_Cuts_Data";
//    system(("mkdir -p " + ProtonIDFolderDataBC).c_str());
//
//    const string ProtonIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data/After_Cuts_Data";
//    system(("mkdir -p " + ProtonIDFolderDataAC).c_str());
//
//    const string NeutralsFDIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data";
//    system(("mkdir -p " + NeutralsFDIDFolderData).c_str());
//
//    const string NeutralsFDIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/Before_Cuts_Data";
//    system(("mkdir -p " + NeutralsFDIDFolderDataBC).c_str());
//
//    const string NeutralsFDIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/After_Cuts_Data";
//    system(("mkdir -p " + NeutralsFDIDFolderDataAC).c_str());
//
//    const string PionsIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data";
//    system(("mkdir -p " + PionsIDFolderData).c_str());
//
//    const string PionsIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data/Before_Cuts_Data";
//    system(("mkdir -p " + PionsIDFolderDataBC).c_str());
//
//    const string PionsIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data/After_Cuts_Data";
//    system(("mkdir -p " + PionsIDFolderDataAC).c_str());
//
//    const string OtherPartIDFolderData = "00_MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data";
//    system(("mkdir -p " + OtherPartIDFolderData).c_str());
//
//    const string OtherPartIDFolderDataBC = "00_MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data/Before_Cuts_Data";
//    system(("mkdir -p " + OtherPartIDFolderDataBC).c_str());
//
//    const string OtherPartIDFolderDataAC = "00_MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data/After_Cuts_Data";
//    system(("mkdir -p " + OtherPartIDFolderDataAC).c_str());
//    //</editor-fold>

    //<editor-fold desc="Final_State_Comparison">

//    //<editor-fold desc="01_Momentum_comp_Sim">
//    HistPlotter1D(Canv, MScThesisPlots, Final_noSaC_Sim, "FD proton momentum APID (1p, FD)", SampleName, MomComp_BSaC_1N_Sim_FolderData, "01_P_pFD_APID_1p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, Final_noSaC_Sim, "Leading FD neutron momentum APID (1n, FD)", SampleName, MomComp_BSaC_1N_Sim_FolderData, "01_P_nFD_APID_1n_FD");
//
//    HistPlotter1D(Canv, MScThesisPlots, Final_wSaC_Sim, "FD proton momentum APID&PS (1p, FD)", SampleName, MomComp_ASaC_1N_Sim_FolderData, "01_P_pFD_APIDandPS_1p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, Final_wSaC_Sim, "Leading FD neutron momentum APID&NC (1n, FD)", SampleName, MomComp_ASaC_1N_Sim_FolderData,
//                  "01_P_nFD_APIDandNC_1n_FD");
//
////    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "1N", "TH1D", "FD", "Electron momentum APID", "Electron momentum APID", "Electron momentum",
////               MomComp_ElecMom_2N_Reco_Sim_FolderData, "P_e_1p_FD_1N", "P_e_1n_FD_1N", "P_e_FSR_1N", 1);
////    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH1D", "FD", "Electron momentum APID", "Electron momentum APID", "Electron momentum",
////               MomComp_ElecMom_2N_Reco_Sim_FolderData, "P_e_pFDpCD_FD_2N", "P_e_nFDpCD_FD_2N", "P_e_FSR_2N", 2);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "1N", "FD", "FD proton momentum APID&PS", "Leading FD neutron momentum APID&NC",
//               "Leading FD neutron momentum", MomComp_NucMom_1N_Reco_Sim_FolderData, "P_pFD_APIDandPS_1p_FD", "P_nFD_APIDandNS_1n_FD", "P_nucFD_FSR", 1);
//
    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "FD proton momentum", "FD neutron momentum", "FD neutron momentum",
               MomComp_NucMom_2N_Reco_Sim_FolderData, "P_pFD_pFDpCD_FD", "P_nFD_nFDpCD_FD", "P_nucFD_FSR_2N", 1);
    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "CD proton momentum", "CD proton momentum", "CD proton momentum",
               MomComp_NucMom_2N_Reco_Sim_FolderData, "P_pCD_pFDpCD_CD", "P_pCD_nFDpCD_CD", "P_pCD_FSR_2N", 2);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "P_{pFD} vs. P_{pCD}", "P_{nFD} vs. P_{pCD}", "P_{nFD} vs. P_{pCD} (nFDpCD)",
//               MomComp_NucMom_2N_Reco_Sim_FolderData, "P_pFD_vs_P_pCD_pFDpCD", "P_nFD_vs_P_pCD_nFDpCD", "P_nucFD_vs_P_nucCD_FSR_2N", 3, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "Leading proton momentum", "Leading nucleon momentum",
//               "Leading nucleon momentum", MomComp_NucMom_2N_Reco_Sim_FolderData, "P_pL_pFDpCD_FD", "P_nL_nFDpCD_FD", "P_nucL_FSR_2N", 4);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "Recoil proton momentum", "Recoil nucleon momentum",
//               "Recoil nucleon momentum", MomComp_NucMom_2N_Reco_Sim_FolderData, "P_pR_pFDpCD_FD", "P_nR_nFDpCD_FD", "P_nucR_FSR_2N", 5);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "P_{pL} vs. P_{pR}", "P_{nL} vs. P_{nR}", "P_{nL} vs. P_{nR} (nFDpCD)",
//               MomComp_NucMom_2N_Reco_Sim_FolderData, "P_pL_vs_P_pR_pFDpCD", "P_nL_vs_P_nR_nFDpCD", "P_nucL_vs_P_nucR_FSR_2N", 6, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#vec{P}_{tot}-#vec{q} distribution",
//               "#vec{P}_{tot}-#vec{q} distribution", "#vec{P}_{tot}-#vec{q} distribution", MomComp_P_miss_Sim_FolderData, "P_miss_pFDpCD", "P_miss_nFDpCD", "P_miss_FSR_2N",
//               1);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "Total nucleon momentum (3-vector)",
//               "Total nucleon momentum (3-vector)", "Total nucleon 3-momentum", MomComp_TotAndRel_Mom_Sim_FolderData, "P_3tot_pFDpCD", "P_3tot_nFDpCD", "P_tot_FSR_2N", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "Relative nucleon momentum (3-vector)",
//               "Relative nucleon momentum (3-vector)", "Relative nucleon 3-momentum", MomComp_TotAndRel_Mom_Sim_FolderData, "P_3rel_pFDpCD", "P_3rel_nFDpCD", "P_rel_FSR_2N",
//               2);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|",
//               "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", MomComp_TotAndRel_Mom_Sim_FolderData, "P_tot_vs_P_rel_pFDpCD", "P_tot_vs_P_rel_nFDpCD", "P_tot_vs_P_rel_FSR_2N", 3,
//               true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "1N", "TH1D", "FD", "FD TL Proton momentum AC", "FD TL Neutron momentum AC",
//               "FD TL Neutron momentum AC", MomComp_NucMom_1N_TL_Sim_FolderData, "P_pFD_TL_1p", "P_nFD_TL_1n", "P_nucFD_TL_FSR_1N", 1);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH1D", "FD", "FD TL Proton momentum AC", "FD TL Neutron momentum AC",
//               "FD TL Neutron momentum AC", MomComp_NucMom_2N_TL_Sim_FolderData, "P_pFD_TL_pFDpCD", "P_nFD_TL_pFDpCD", "P_nucFD_TL_FSR_2N", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH1D", "CD", "CD TL Proton momentum AC", "CD TL Proton momentum AC",
//               "CD TL Proton momentum AC", MomComp_NucMom_2N_TL_Sim_FolderData, "P_pCD_TL_pFDpCD", "P_pCD_TL_pFDpCD", "P_nucCD_TL_FSR_2N", 2);
//    //</editor-fold>
//
//    //<editor-fold desc="01_Momentum_comp_Data">
//    HistPlotter1D(Canv, MScThesisPlots, Final_noSaC_Data, "FD proton momentum APID (1p, FD)", SampleName, MomComp_BSaC_1N_Data_FolderData, "01_P_pFD_APID_1p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, Final_noSaC_Data, "Leading FD neutron momentum APID (1n, FD)", SampleName, MomComp_BSaC_1N_Data_FolderData, "01_P_nFD_APID_1n_FD");
//
//    HistPlotter1D(Canv, MScThesisPlots, Final_wSaC_Data, "FD proton momentum APID&PS (1p, FD)", SampleName, MomComp_ASaC_1N_Data_FolderData, "01_P_pFD_APIDandPS_1p_FD");
//    HistPlotter1D(Canv, MScThesisPlots, Final_wSaC_Data, "Leading FD neutron momentum APID&NC (1n, FD)", SampleName, MomComp_ASaC_1N_Data_FolderData,
//                  "01_P_nFD_APIDandNC_1n_FD");
//
////    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "1N", "TH1D", "FD", "Electron momentum APID", "Electron momentum APID", "Electron momentum",
////               MomComp_ElecMom_2N_Reco_Data_FolderData, "P_e_1p_FD_1N", "P_e_1n_FD_1N", "P_e_FSR_1N", 1);
////    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "Electron momentum APID", "Electron momentum APID", "Electron momentum",
////               MomComp_ElecMom_2N_Reco_Data_FolderData, "P_e_pFDpCD_FD_2N", "P_e_nFDpCD_FD_2N", "P_e_FSR_2N", 2);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "1N", "TH1D", "FD", "FD proton momentum APID&PS", "Leading FD neutron momentum APID&NC",
//               "Leading FD neutron momentum", MomComp_NucMom_1N_Reco_Data_FolderData, "P_pFD_APIDandPS_1p_FD", "P_nFD_APIDandNS_1n_FD", "P_nucFD_FSR", 1);
//
    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "FD proton momentum", "FD neutron momentum", "FD neutron momentum",
               MomComp_NucMom_2N_Reco_Data_FolderData, "P_pFD_pFDpCD_FD", "P_nFD_nFDpCD_FD", "P_nucFD_FSR_2N", 1);
    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "CD proton momentum", "CD proton momentum", "CD proton momentum",
               MomComp_NucMom_2N_Reco_Data_FolderData, "P_pCD_pFDpCD_CD", "P_pCD_nFDpCD_CD", "P_pCD_FSR_2N", 2);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "P_{pFD} vs. P_{pCD}", "P_{nFD} vs. P_{pCD}", "P_{nFD} vs. P_{pCD} (nFDpCD)",
//               MomComp_NucMom_2N_Reco_Data_FolderData, "P_pFD_vs_P_pCD_pFDpCD", "P_nFD_vs_P_pCD_nFDpCD", "P_nucFD_vs_P_nucCD_FSR_2N", 3, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "Leading proton momentum", "Leading nucleon momentum",
//               "Leading nucleon momentum", MomComp_NucMom_2N_Reco_Data_FolderData, "P_pL_pFDpCD_FD", "P_nL_nFDpCD_FD", "P_nucL_FSR_2N", 4);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "Recoil proton momentum", "Recoil nucleon momentum",
//               "Recoil nucleon momentum", MomComp_NucMom_2N_Reco_Data_FolderData, "P_pR_pFDpCD_FD", "P_nR_nFDpCD_FD", "P_nucR_FSR_2N", 5);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "P_{pL} vs. P_{pR}", "P_{nL} vs. P_{nR}", "P_{nL} vs. P_{nR} (nFDpCD)",
//               MomComp_NucMom_2N_Reco_Data_FolderData, "P_pL_vs_P_pR_pFDpCD", "P_nL_vs_P_nR_nFDpCD", "P_nucL_vs_P_nucR_FSR_2N", 6, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#vec{P}_{tot}-#vec{q} distribution", "#vec{P}_{tot}-#vec{q} distribution",
//               "#vec{P}_{tot}-#vec{q} distribution", MomComp_P_miss_Data_FolderData, "P_miss_pFDpCD", "P_miss_nFDpCD", "P_miss_FSR_2N", 1);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "Total nucleon momentum (3-vector)", "Total nucleon momentum (3-vector)",
//               "Total nucleon 3-momentum", MomComp_TotAndRel_Mom_Data_FolderData, "P_3rel_pFDpCD", "P_3rel_nFDpCD", "P_rel_FSR_2N", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "Relative nucleon momentum (3-vector)",
//               "Relative nucleon momentum (3-vector)", "Relative nucleon 3-momentum", MomComp_TotAndRel_Mom_Data_FolderData, "P_3rel_pFDpCD", "P_3rel_nFDpCD", "P_rel_FSR_2N",
//               2);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|",
//               "|#vec{P}_{tot}| vs. |#vec{P}_{rel}|", MomComp_TotAndRel_Mom_Data_FolderData, "P_tot_vs_P_rel_pFDpCD", "P_tot_vs_P_rel_nFDpCD", "P_tot_vs_P_rel_FSR_2N", 3,
//               true);
//    //</editor-fold>
//
//    //<editor-fold desc="02_Inv_mass_comp_Sim">
    FSRPlotterStack(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "W distribution (All Int., pFDpCD)",
                    "W distribution (All Int., nFDpCD)", "W distribution (All Int., nFDpCD)", InvMassComp_Sim_FolderData, "W_All_Int_pFDpCD", "W_All_Int_nFDpCD", "W_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "Q^{2} (pFDpCD, FD)", "Q^{2} (nFDpCD, FD)",
//               "Q^{2} (nFDpCD, FD)", InvMassComp_Sim_FolderData, "Q2_pFDpCD", "Q2_nFDpCD", "Q2_FSR", 2);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "Q^{2} vs. W (All Int., pFDpCD)", "Q^{2} vs. W (All Int., nFDpCD)",
//               "Q^{2} vs. W (All Int., nFDpCD)", InvMassComp_Sim_FolderData, "Q2_VS_W_pFDpCD", "Q2_VS_W_nFDpCD", "Q2_VS_W_FSR_2N", 3, true);
//    //</editor-fold>
//
//    //<editor-fold desc="02_Inv_mass_comp_Data">
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "W distribution (All Int., pFDpCD)", "W distribution (All Int., nFDpCD)",
//               "W distribution (All Int., nFDpCD)", InvMassComp_Data_FolderData, "W_All_Int_pFDpCD", "W_All_Int_nFDpCD", "W_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "Q^{2} (pFDpCD, FD)", "Q^{2} (nFDpCD, FD)", "Q^{2} (nFDpCD, FD)",
//               InvMassComp_Data_FolderData, "Q2_pFDpCD", "Q2_nFDpCD", "Q2_FSR", 2);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "Q^{2} vs. W (All Int., pFDpCD)", "Q^{2} vs. W (All Int., nFDpCD)",
//               "Q^{2} vs. W (All Int., nFDpCD)", InvMassComp_Data_FolderData, "Q2_VS_W_pFDpCD", "Q2_VS_W_nFDpCD", "Q2_VS_W_FSR_2N", 3, true);
//    //</editor-fold>
//
//    //<editor-fold desc="03_Angular_dist_comp_Sim">
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#theta_{e} (All Int., pFDpCD, FD)",
//               "#theta_{e} (All Int., nFDpCD, FD)",
//               "#theta_{e} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_e_All_Int_pFDpCD_FD", "Theta_e_All_Int_nFDpCD_FD",
//               "Theta_e_FSR_pFDpCD_FD", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{e} vs. P_{e} (All Int., pFDpCD)",
//               "#theta_{e} vs. P_{e} (All Int., nFDpCD)", "#theta_{e} vs. P_{e} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_e_VS_P_e_pFDpCD",
//               "Theta_e_VS_P_e_nFDpCD", "Theta_e_VS_P_e_FSR_2N", 2, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{e} vs. W (All Int., pFDpCD)", "#theta_{e} vs. W (All Int., nFDpCD)",
//               "#theta_{e} vs. W (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_e_VS_W_pFDpCD", "Theta_e_VS_W_nFDpCD", "Theta_e_VS_W_FSR_2N", 3, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{e} vs. P_{e} (All Int., pFDpCD)",
//               "#phi_{e} vs. P_{e} (All Int., nFDpCD)", "#phi_{e} vs. P_{e} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Phi_e_VS_P_e_pFDpCD",
//               "Phi_e_VS_P_e_nFDpCD", "Phi_e_VS_P_e_FSR_2N", 4, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{e} vs. W (All Int., pFDpCD)", "#phi_{e} vs. W (All Int., nFDpCD)",
//               "#phi_{e} vs. W (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Phi_e_VS_W_pFDpCD", "Phi_e_VS_W_nFDpCD", "Phi_e_VS_W_FSR_2N", 5, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#theta_{pFD} (All Int., pFDpCD)", "#theta_{nFD} (All Int., nFDpCD)",
//               "#theta_{nFD} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_pFD_All_Int_pFDpCD_FD", "Theta_nFD_All_Int_nFDpCD_FD",
//               "Theta_nucFD_FSR_pFDpCD_FD", 6);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)",
//               "#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", "#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Sim_FolderData,
//               "Theta_pFD_VS_P_pFD_pFDpCD", "Theta_nFD_VS_P_nFD_nFDpCD", "Theta_nucFD_VS_P_pFD_FSR_2N", 7, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{pFD} vs. W (All Int., pFDpCD, FD)",
//               "#theta_{nFD} vs. W (All Int., nFDpCD, FD)", "#theta_{nFD} vs. W (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_pFD_VS_W_pFDpCD",
//               "Theta_nFD_VS_W_nFDpCD", "Theta_nucFD_VS_W_FSR_2N", 8, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)",
//               "#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", "#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Sim_FolderData,
//               "phi_pFD_VS_P_pFD_pFDpCD", "phi_nFD_VS_P_nFD_nFDpCD", "phi_nucFD_VS_P_pFD_FSR_2N", 9, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{pFD} vs. W (All Int., pFDpCD, FD)",
//               "#phi_{nFD} vs. W (All Int., nFDpCD, FD)", "#phi_{nFD} vs. W (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Sim_FolderData, "phi_pFD_VS_W_pFDpCD",
//               "phi_nFD_VS_W_nFDpCD", "phi_nucFD_VS_W_FSR_2N", 10, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{pCD} (All Int., pFDpCD)", "#theta_{pCD} (All Int., nFDpCD)",
//               "#theta_{pCD} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_pCD_All_Int_pFDpCD_CD", "Theta_pCD_All_Int_nFDpCD_CD",
//               "Theta_nucCD_FSR_pFDpCD_CD", 11);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
//               "#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Sim_FolderData,
//               "Theta_pCD_VS_P_pCD_pFDpCD", "Theta_pCD_VS_P_pCD_nFDpCD", "Theta_pCD_VS_P_pCD_FSR_2N", 12, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{pCD} vs. W (All Int., pFDpCD, CD)",
//               "#theta_{pCD} vs. W (All Int., nFDpCD, CD)", "#theta_{pCD} vs. W (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_pCD_VS_W_pFDpCD",
//               "Theta_pCD_VS_W_nFDpCD", "Theta_pCD_VS_W_FSR_2N", 13, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Sim_FolderData,
//               "phi_pCD_VS_P_pCD_pFDpCD", "phi_pCD_VS_P_pCD_nFDpCD", "phi_pCD_VS_P_pCD_FSR_2N", 14, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. W (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Sim_FolderData, "phi_pCD_VS_W_pFDpCD",
//               "phi_pCD_VS_W_nFDpCD", "phi_pCD_VS_W_FSR_2N", 15, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{e} vs. #phi_{e} (All Int., pFDpCD, FD)",
//               "#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD)", "#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Sim_FolderData,
//               "Theta_e_VS_Phi_e_pFDpCD", "Theta_e_VS_Phi_e_nFDpCD", "Theta_e_VS_Phi_e_FSR_2N", 16, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Sim_FolderData,
//               "phi_pCD_VS_P_pCD_pFDpCD", "phi_pCD_VS_P_pCD_nFDpCD", "phi_pCD_VS_P_pCD_FSR_2N", 17, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. W (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Sim_FolderData, "phi_pCD_VS_W_pFDpCD",
//               "phi_pCD_VS_W_nFDpCD", "phi_pCD_VS_W_FSR_2N", 18, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{tot} (All Int., pFDpCD)", "#theta_{tot} (All Int., nFDpCD)",
//               "theta_tot FSRatio ()", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_tot_All_Int_pFDpCD", "Theta_tot_All_Int_nFDpCD", "Theta_tot_FSR_pFDpCD", 19);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{rel} (All Int., pFDpCD)", "#theta_{rel} (All Int., nFDpCD)",
//               "theta_rel FSRatio ()", AngDistComp_Scattring_Ang_Sim_FolderData, "Theta_rel_All_Int_pFDpCD", "Theta_rel_All_Int_nFDpCD", "Theta_rel_FSR_pFDpCD", 20);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., pFDpCD)",
//               "#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., nFDpCD)", "#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_P_e_and_P_tot_Sim_FolderData, "Theta__P_e_and_P_tot__All_Int_pFDpCD", "Theta__P_e_and_P_tot__All_Int_nFDpCD",
//               "Theta__P_e_and_P_tot__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., nFDpCD)", "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_P_e_and_P_tot_Sim_FolderData, "Theta_p_e_p_tot_vs_W_pFDpCD", "Theta_p_e_p_tot_vs_W_nFDpCD", "Theta_p_e_p_tot_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{#vec{q},#vec{P}_{tot}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{tot}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{tot}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_tot_Sim_FolderData,
//               "Theta__q_and_P_tot__All_Int_pFDpCD", "Theta__q_and_P_tot__All_Int_nFDpCD", "Theta__q_and_P_tot__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_tot_Sim_FolderData, "Theta_q_p_tot_vs_W_pFDpCD", "Theta_q_p_tot_vs_W_nFDpCD", "Theta_q_p_tot_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
//               "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)", "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_P_L_minus_q_and_P_R_Sim_FolderData, "Theta__P_L_minus_q_and_P_R__All_Int_pFDpCD", "Theta__P_L_minus_q_and_P_R__All_Int_nFDpCD",
//               "Theta__P_L_minus_q_and_P_R__FSR", 1);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{#vec{q},#vec{P}_{pL}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nL}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nL}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_L_Sim_FolderData,
//               "Theta__q_and_P_L__All_Int_pFDpCD", "Theta__q_and_P_L__All_Int_nFDpCD", "Theta__q_and_P_L__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pL}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nL}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nL}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_L_Sim_FolderData, "Theta_q_pL_vs_W_pFDpCD", "Theta_q_nL_vs_W_nFDpCD", "Theta_q_nucL_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_R_Sim_FolderData,
//               "Theta__q_and_P_R__All_Int_pFDpCD", "Theta__q_and_P_R__All_Int_nFDpCD", "Theta__q_and_P_R__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pR}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_R_Sim_FolderData, "Theta_q_pR_vs_W_pFDpCD", "Theta_q_nR_vs_W_nFDpCD", "Theta_q_nucR_vs_W_FSR_2N", 2, true);
//
    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#theta_{#vec{q},#vec{P}_{pFD}} (All Int., pFDpCD)",
               "#theta_{#vec{q},#vec{P}_{nFD}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{pFD}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_nucFD_Sim_FolderData,
               "Theta__q_and_P_pFD__All_Int_pFDpCD", "Theta__q_and_P_nFD__All_Int_nFDpCD", "Theta__q_and_P_nucFD__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pFD}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nFD}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nFD}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_nucFD_Sim_FolderData, "Theta_q_pFD_vs_W_pFDpCD", "Theta_q_nFD_vs_W_nFDpCD", "Theta_q_nucFD_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{#vec{q},#vec{P}_{pCD}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_nucCD_Sim_FolderData,
//               "Theta__q_and_P_pCD__All_Int_pFDpCD", "Theta__q_and_P_pCD__All_Int_nFDpCD", "Theta__q_and_P_nucCD__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_nucCD_Sim_FolderData, "Theta_q_pCD_vs_W_pFDpCD", "Theta_q_pCD_vs_W_nFDpCD", "Theta_q_nucCD_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "CD", "#theta_{pFD,pCD} (All Int., pFDpCD)",
//               "#theta_{nFD,pCD} (All Int., nFDpCD)",
//               "#theta_{nFD,pCD} (All Int., nFDpCD)", AngDistComp_Opening_Ang_P_FD_and_P_CD_Sim_FolderData, "Theta__Theta_pFD_pCD__All_Int_pFDpCD",
//               "Theta__Theta_nFD_pCD__All_Int_nFDpCD", "Theta__Theta_nucFD_nucCD__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#theta_{pFD,pCD} vs. W (All Int., pFDpCD)",
//               "#theta_{nFD,pCD} vs. W (All Int., nFDpCD)", "#theta_{nFD,pCD} vs. W (All Int., nFDpCD)", AngDistComp_Opening_Ang_P_FD_and_P_CD_Sim_FolderData,
//               "Theta_pFD_pCD_vs_W_pFDpCD", "Theta_nFD_pCD_vs_W_nFDpCD", "Theta_nucFD_nucCD_vs_W_FSR_2N", 2, true);
//    //</editor-fold>
//
//    //<editor-fold desc="03_Angular_dist_comp_Data">
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#theta_{e} (All Int., pFDpCD, FD)", "#theta_{e} (All Int., nFDpCD, FD)",
//               "#theta_{e} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_e_All_Int_pFDpCD_FD", "Theta_e_All_Int_nFDpCD_FD",
//               "Theta_e_FSR_pFDpCD_FD", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{e} vs. P_{e} (All Int., pFDpCD)",
//               "#theta_{e} vs. P_{e} (All Int., nFDpCD)", "#theta_{e} vs. P_{e} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_e_VS_P_e_pFDpCD",
//               "Theta_e_VS_P_e_nFDpCD", "Theta_e_VS_P_e_FSR_2N", 2, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{e} vs. W (All Int., pFDpCD)", "#theta_{e} vs. W (All Int., nFDpCD)",
//               "#theta_{e} vs. W (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_e_VS_W_pFDpCD", "Theta_e_VS_W_nFDpCD", "Theta_e_VS_W_FSR_2N", 3, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{e} vs. P_{e} (All Int., pFDpCD)",
//               "#phi_{e} vs. P_{e} (All Int., nFDpCD)", "#phi_{e} vs. P_{e} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Data_FolderData, "Phi_e_VS_P_e_pFDpCD",
//               "Phi_e_VS_P_e_nFDpCD", "Phi_e_VS_P_e_FSR_2N", 4, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{e} vs. W (All Int., pFDpCD)", "#phi_{e} vs. W (All Int., nFDpCD)",
//               "#phi_{e} vs. W (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Data_FolderData, "Phi_e_VS_W_pFDpCD", "Phi_e_VS_W_nFDpCD", "Phi_e_VS_W_FSR_2N", 5, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#theta_{pFD} (All Int., pFDpCD)", "#theta_{nFD} (All Int., nFDpCD)",
//               "#theta_{nFD} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_pFD_All_Int_pFDpCD_FD", "Theta_nFD_All_Int_nFDpCD_FD",
//               "Theta_nucFD_FSR_pFDpCD_FD", 6);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)",
//               "#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", "#theta_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Data_FolderData,
//               "Theta_pFD_VS_P_pFD_pFDpCD", "Theta_nFD_VS_P_nFD_nFDpCD", "Theta_nucFD_VS_P_pFD_FSR_2N", 7, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{pFD} vs. W (All Int., pFDpCD, FD)",
//               "#theta_{nFD} vs. W (All Int., nFDpCD, FD)", "#theta_{nFD} vs. W (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_pFD_VS_W_pFDpCD",
//               "Theta_nFD_VS_W_nFDpCD", "Theta_nucFD_VS_W_FSR_2N", 8, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{pFD} vs. P_{pFD} (All Int., pFDpCD, FD)",
//               "#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", "#phi_{nFD} vs. P_{nFD} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Data_FolderData,
//               "phi_pFD_VS_P_pFD_pFDpCD", "phi_nFD_VS_P_nFD_nFDpCD", "phi_nucFD_VS_P_pFD_FSR_2N", 9, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{pFD} vs. W (All Int., pFDpCD, FD)",
//               "#phi_{nFD} vs. W (All Int., nFDpCD, FD)", "#phi_{nFD} vs. W (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Data_FolderData, "phi_pFD_VS_W_pFDpCD",
//               "phi_nFD_VS_W_nFDpCD", "phi_nucFD_VS_W_FSR_2N", 10, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{pCD} (All Int., pFDpCD)", "#theta_{pCD} (All Int., nFDpCD)",
//               "#theta_{pCD} (All Int., nFDpCD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_pCD_All_Int_pFDpCD_CD", "Theta_pCD_All_Int_nFDpCD_CD",
//               "Theta_nucCD_FSR_pFDpCD_CD", 11);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
//               "#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#theta_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Data_FolderData,
//               "Theta_pCD_VS_P_pCD_pFDpCD", "Theta_pCD_VS_P_pCD_nFDpCD", "Theta_pCD_VS_P_pCD_FSR_2N", 12, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{pCD} vs. W (All Int., pFDpCD, CD)",
//               "#theta_{pCD} vs. W (All Int., nFDpCD, CD)", "#theta_{pCD} vs. W (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_pCD_VS_W_pFDpCD",
//               "Theta_pCD_VS_W_nFDpCD", "Theta_pCD_VS_W_FSR_2N", 13, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Data_FolderData,
//               "phi_pCD_VS_P_pCD_pFDpCD", "phi_pCD_VS_P_pCD_nFDpCD", "phi_pCD_VS_P_pCD_FSR_2N", 14, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. W (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Data_FolderData, "phi_pCD_VS_W_pFDpCD",
//               "phi_pCD_VS_W_nFDpCD", "phi_pCD_VS_W_FSR_2N", 15, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{e} vs. #phi_{e} (All Int., pFDpCD, FD)",
//               "#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD)", "#theta_{e} vs. #phi_{e} (All Int., nFDpCD, FD)", AngDistComp_Scattring_Ang_Data_FolderData,
//               "Theta_e_VS_Phi_e_pFDpCD", "Theta_e_VS_Phi_e_nFDpCD", "Theta_e_VS_Phi_e_FSR_2N", 16, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. P_{pCD} (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", "#phi_{pCD} vs. P_{pCD} (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Data_FolderData,
//               "phi_pCD_VS_P_pCD_pFDpCD", "phi_pCD_VS_P_pCD_nFDpCD", "phi_pCD_VS_P_pCD_FSR_2N", 17, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#phi_{pCD} vs. W (All Int., pFDpCD, CD)",
//               "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", "#phi_{pCD} vs. W (All Int., nFDpCD, CD)", AngDistComp_Scattring_Ang_Data_FolderData, "phi_pCD_VS_W_pFDpCD",
//               "phi_pCD_VS_W_nFDpCD", "phi_pCD_VS_W_FSR_2N", 18, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{tot} (All Int., pFDpCD)", "#theta_{tot} (All Int., nFDpCD)",
//               "theta_tot FSRatio ()", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_tot_All_Int_pFDpCD", "Theta_tot_All_Int_nFDpCD", "Theta_tot_FSR_pFDpCD", 19);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{rel} (All Int., pFDpCD)", "#theta_{rel} (All Int., nFDpCD)",
//               "theta_rel FSRatio ()", AngDistComp_Scattring_Ang_Data_FolderData, "Theta_rel_All_Int_pFDpCD", "Theta_rel_All_Int_nFDpCD", "Theta_rel_FSR_pFDpCD", 20);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., pFDpCD)",
//               "#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., nFDpCD)", "#theta_{#vec{P}_{e},#vec{P}_{tot}} (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_P_e_and_P_tot_Data_FolderData, "Theta__P_e_and_P_tot__All_Int_pFDpCD", "Theta__P_e_and_P_tot__All_Int_nFDpCD",
//               "Theta__P_e_and_P_tot__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., nFDpCD)", "#theta_{#vec{P}_{e},#vec{P}_{tot}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_P_e_and_P_tot_Data_FolderData, "Theta_p_e_p_tot_vs_W_pFDpCD", "Theta_p_e_p_tot_vs_W_nFDpCD", "Theta_p_e_p_tot_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{#vec{q},#vec{P}_{tot}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{tot}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{tot}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_tot_Data_FolderData,
//               "Theta__q_and_P_tot__All_Int_pFDpCD", "Theta__q_and_P_tot__All_Int_nFDpCD", "Theta__q_and_P_tot__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{tot}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_tot_Data_FolderData, "Theta_q_p_tot_vs_W_pFDpCD", "Theta_q_p_tot_vs_W_nFDpCD", "Theta_q_p_tot_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{#vec{P}_{pL}-#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
//               "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)", "#theta_{#vec{P}_{nL}-#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_P_L_minus_q_and_P_R_Data_FolderData, "Theta__P_L_minus_q_and_P_R__All_Int_pFDpCD", "Theta__P_L_minus_q_and_P_R__All_Int_nFDpCD",
//               "Theta__P_L_minus_q_and_P_R__FSR", 1);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{#vec{q},#vec{P}_{pL}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nL}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nL}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_L_Data_FolderData,
//               "Theta__q_and_P_L__All_Int_pFDpCD", "Theta__q_and_P_L__All_Int_nFDpCD", "Theta__q_and_P_L__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pL}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nL}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nL}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_L_Data_FolderData, "Theta_q_pL_vs_W_pFDpCD", "Theta_q_nL_vs_W_nFDpCD", "Theta_q_nucL_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{#vec{q},#vec{P}_{pR}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nR}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_R_Data_FolderData,
//               "Theta__q_and_P_R__All_Int_pFDpCD", "Theta__q_and_P_R__All_Int_nFDpCD", "Theta__q_and_P_R__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pR}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nR}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_R_Data_FolderData, "Theta_q_pR_vs_W_pFDpCD", "Theta_q_nR_vs_W_nFDpCD", "Theta_q_nucR_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#theta_{#vec{q},#vec{P}_{pFD}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nFD}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{pFD}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_nucFD_Data_FolderData,
//               "Theta__q_and_P_pFD__All_Int_pFDpCD", "Theta__q_and_P_nFD__All_Int_nFDpCD", "Theta__q_and_P_nucFD__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pFD}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{nFD}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{nFD}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_nucFD_Data_FolderData, "Theta_q_pFD_vs_W_pFDpCD", "Theta_q_nFD_vs_W_nFDpCD", "Theta_q_nucFD_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{#vec{q},#vec{P}_{pCD}} (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{pCD}} (All Int., nFDpCD)", AngDistComp_Opening_Ang_q_and_P_nucCD_Data_FolderData,
//               "Theta__q_and_P_pCD__All_Int_pFDpCD", "Theta__q_and_P_pCD__All_Int_nFDpCD", "Theta__q_and_P_nucCD__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., pFDpCD)",
//               "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., nFDpCD)", "#theta_{#vec{q},#vec{P}_{pCD}} vs. W (All Int., nFDpCD)",
//               AngDistComp_Opening_Ang_q_and_P_nucCD_Data_FolderData, "Theta_q_pCD_vs_W_pFDpCD", "Theta_q_pCD_vs_W_nFDpCD", "Theta_q_nucCD_vs_W_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "CD", "#theta_{pFD,pCD} (All Int., pFDpCD)", "#theta_{nFD,pCD} (All Int., nFDpCD)",
//               "#theta_{nFD,pCD} (All Int., nFDpCD)", AngDistComp_Opening_Ang_P_FD_and_P_CD_Data_FolderData, "Theta__Theta_pFD_pCD__All_Int_pFDpCD",
//               "Theta__Theta_nFD_pCD__All_Int_nFDpCD", "Theta__Theta_nucFD_nucCD__FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#theta_{pFD,pCD} vs. W (All Int., pFDpCD)",
//               "#theta_{nFD,pCD} vs. W (All Int., nFDpCD)", "#theta_{nFD,pCD} vs. W (All Int., nFDpCD)", AngDistComp_Opening_Ang_P_FD_and_P_CD_Data_FolderData,
//               "Theta_pFD_pCD_vs_W_pFDpCD", "Theta_nFD_pCD_vs_W_nFDpCD", "Theta_nucFD_nucCD_vs_W_FSR_2N", 2, true);
//    //</editor-fold>
//
//    //<editor-fold desc="04_Energy_dist_comp_Sim">
//    FSRPlotterStack(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "E_{e} (All Int., pFDpCD)", "E_{e} (All Int., nFDpCD)",
//                    "E_{e} (All Int., nFDpCD)", EnergyComp_E_e_Sim_FolderData, "E_e_All_Int_pFDpCD", "E_e_All_Int_nFDpCD", "E_e_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "E_{e} vs. #theta_{e} (All Int., pFDpCD)",
//               "E_{e} vs. #theta_{e} (All Int., nFDpCD)", "E_{e} vs. #theta_{e} (All Int., nFDpCD)", EnergyComp_E_e_Sim_FolderData, "E_e_VS_Theta_e_All_Int_pFDpCD",
//               "E_e_VS_Theta_e_All_Int_nFDpCD", "E_e_VS_Theta_e_All_Int_FSR_2N", 2, true);
//
//    FSRPlotterStack(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#omega around #theta_{e} = 15#circ (All Int., pFDpCD)",
//                    "#omega around #theta_{e} = 15#circ (All Int., nFDpCD)", "#omega around #theta_{e} = 15#circ (All Int., nFDpCD)", EnergyComp_Omega_Sim_FolderData,
//                    "Omega_15_All_Int_pFDpCD", "Omega_15_All_Int_nFDpCD", "Omega_15_FSR", 1);
//
//    FSRPlotterStack(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "E_{cal} reco. (All Int., pFDpCD)",
//                    "E_{cal} reco. (All Int., nFDpCD)",
//                    "E_{cal} reco. (All Int., nFDpCD)", EnergyComp_E_cal_Sim_FolderData, "E_cal_All_Int_pFDpCD", "E_cal_All_Int_nFDpCD", "E_cal_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD)",
//               "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)", EnergyComp_E_cal_Sim_FolderData, "Ecal_vs_dP_T_tot_pFDpCD",
//               "Ecal_vs_dP_T_tot_nFDpCD", "Ecal_vs_dP_T_tot_FSR_2N", 2, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
//               "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", EnergyComp_E_cal_Sim_FolderData,
//               "Ecal_vs_dAlpha_T_tot_pFDpCD", "Ecal_vs_dAlpha_T_tot_nFDpCD", "Ecal_vs_dAlpha_T_tot_FSR_2N", 3, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "E_{cal} vs. W (All Int., pFDpCD)", "E_{cal} vs. W (All Int., nFDpCD)",
//               "E_{cal} vs. W (All Int., nFDpCD)", EnergyComp_E_cal_Sim_FolderData, "Ecal_vs_W_pFDpCD", "Ecal_vs_W_nFDpCD", "Ecal_vs_W_FSR_2N", 4, true);
//    //</editor-fold>
//
//    //<editor-fold desc="04_Energy_dist_comp_Data">
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "E_{e} (All Int., pFDpCD)", "E_{e} (All Int., nFDpCD)",
//               "E_{e} (All Int., nFDpCD)", EnergyComp_E_e_Data_FolderData, "E_e_All_Int_pFDpCD", "E_e_All_Int_nFDpCD", "E_e_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "E_{e} vs. #theta_{e} (All Int., pFDpCD)",
//               "E_{e} vs. #theta_{e} (All Int., nFDpCD)", "E_{e} vs. #theta_{e} (All Int., nFDpCD)", EnergyComp_E_e_Data_FolderData, "E_e_VS_Theta_e_All_Int_pFDpCD",
//               "E_e_VS_Theta_e_All_Int_nFDpCD", "E_e_VS_Theta_e_All_Int_FSR_2N", 2, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#omega around #theta_{e} = 15#circ (All Int., pFDpCD)",
//               "#omega around #theta_{e} = 15#circ (All Int., nFDpCD)", "#omega around #theta_{e} = 15#circ (All Int., nFDpCD)", EnergyComp_Omega_Data_FolderData,
//               "Omega_15_All_Int_pFDpCD", "Omega_15_All_Int_nFDpCD", "Omega_15_FSR", 1);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "E_{cal} reco. (All Int., pFDpCD)", "E_{cal} reco. (All Int., nFDpCD)",
//               "E_{cal} reco. (All Int., nFDpCD)", EnergyComp_E_cal_Data_FolderData, "E_cal_All_Int_pFDpCD", "E_cal_All_Int_nFDpCD", "E_cal_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD)",
//               "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)", "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)", EnergyComp_E_cal_Data_FolderData, "Ecal_vs_dP_T_tot_pFDpCD",
//               "Ecal_vs_dP_T_tot_nFDpCD", "Ecal_vs_dP_T_tot_FSR_2N", 2, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
//               "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", EnergyComp_E_cal_Data_FolderData,
//               "Ecal_vs_dAlpha_T_tot_pFDpCD", "Ecal_vs_dAlpha_T_tot_nFDpCD", "Ecal_vs_dAlpha_T_tot_FSR_2N", 3, true);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "E_{cal} vs. W (All Int., pFDpCD)", "E_{cal} vs. W (All Int., nFDpCD)",
//               "E_{cal} vs. W (All Int., nFDpCD)", EnergyComp_E_cal_Data_FolderData, "Ecal_vs_W_pFDpCD", "Ecal_vs_W_nFDpCD", "Ecal_vs_W_FSR_2N", 4, true);
//
//    HistPlotter2D(Canv, MScThesisPlots, Final_wSaC_Data, "E_{cal} vs. #theta_{pCD} (All Int., nFDpCD)", SampleName,
//                  EnergyComp_E_cal_Data_FolderData, "05_Ecal_vs_pCD_nFDpCD");
//    //</editor-fold>
//
//    //<editor-fold desc="05_TKI_comp_Sim">
//    FSRPlotterStack(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#deltaP_{T,tot} (pFDpCD)",
//                    "#deltaP_{T,tot} (nFDpCD)", "#deltaP_{T,tot} (nFDpCD)", TKIComp_dP_T_Sim_FolderData, "dP_T_pFDpCD", "dP_T_nFDpCD", "dP_T_FSR", 1);
////    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#deltaP_{T,tot} (pFDpCD)", "#deltaP_{T,tot} (nFDpCD)",
////               "#deltaP_{T,tot} (nFDpCD)", TKIComp_dP_T_Sim_FolderData, "dP_T_pFDpCD", "dP_T_nFDpCD", "dP_T_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#deltaP_{T,tot} vs. W (All Int., pFDpCD)",
//               "#deltaP_{T,tot} vs. W (All Int., nFDpCD)", "#deltaP_{T,tot} vs. W (All Int., nFDpCD)", TKIComp_dP_T_Sim_FolderData, "dP_T_tot_vs_W_pFDpCD",
//               "dP_T_tot_vs_W_nFDpCD", "dP_T_tot_vs_W_FSR_2N", 3, true);
//
////    FSRPlotterStack(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#delta#alpha_{T,tot} (pFDpCD)",
////                    "#delta#alpha_{T,tot} (nFDpCD)", "#delta#alpha_{T,tot} (nFDpCD)", TKIComp_dAlpha_T_Sim_FolderData, "dAlpha_T_pFDpCD", "dAlpha_T_nFDpCD", "dAlpha_T_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, Final_wSaC_Data, SampleName, "2N", "FD", "#delta#alpha_{T,tot} (pFDpCD)", "#delta#alpha_{T,tot} (nFDpCD)",
//               "#delta#alpha_{T,tot} (nFDpCD)", TKIComp_dAlpha_T_Sim_FolderData, "dAlpha_T_pFDpCD", "dAlpha_T_nFDpCD", "dAlpha_T_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#delta#alpha_{T,tot} vs. W (All Int., pFDpCD)",
//               "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)", "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)", TKIComp_dAlpha_T_Sim_FolderData, "dAlpha_T_tot_vs_W_pFDpCD",
//               "dAlpha_T_tot_vs_W_nFDpCD", "dAlpha_T_tot_vs_W_FSR_2N", 3, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Sim, SampleName, "2N", "TH2D", "FD", "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
//               "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
//               TKIComp_dP_T_vs_dAlpha_T_Sim_FolderData, "dP_T_tot_vs_dAlpha_T_tot_pFDpCD", "dP_T_tot_vs_dAlpha_T_tot_nFDpCD", "dP_T_tot_vs_dAlpha_T_tot_FSR_2N", 1, true);
//    //</editor-fold>
//
//    //<editor-fold desc="05_TKI_comp_Data">
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#deltaP_{T,tot} (pFDpCD)", "#deltaP_{T,tot} (nFDpCD)",
//               "#deltaP_{T,tot} (nFDpCD)", TKIComp_dP_T_Data_FolderData, "dP_T_pFDpCD", "dP_T_nFDpCD", "dP_T_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#deltaP_{T,tot} vs. W (All Int., pFDpCD)",
//               "#deltaP_{T,tot} vs. W (All Int., nFDpCD)", "#deltaP_{T,tot} vs. W (All Int., nFDpCD)", TKIComp_dP_T_Data_FolderData, "dP_T_tot_vs_W_pFDpCD",
//               "dP_T_tot_vs_W_nFDpCD", "dP_T_tot_vs_W_FSR_2N", 3, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH1D", "FD", "#delta#alpha_{T,tot} (pFDpCD)", "#delta#alpha_{T,tot} (nFDpCD)",
//               "#delta#alpha_{T,tot} (nFDpCD)", TKIComp_dAlpha_T_Data_FolderData, "dAlpha_T_pFDpCD", "dAlpha_T_nFDpCD", "dAlpha_T_FSR", 1);
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#delta#alpha_{T,tot} vs. W (All Int., pFDpCD)",
//               "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)", "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)", TKIComp_dAlpha_T_Data_FolderData, "dAlpha_T_tot_vs_W_pFDpCD",
//               "dAlpha_T_tot_vs_W_nFDpCD", "dAlpha_T_tot_vs_W_FSR_2N", 3, true);
//
//    FSRPlotter(utilities, Canv, MScThesisPlots, Final_wSaC_Data, SampleName, "2N", "TH2D", "FD", "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
//               "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)", "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
//               TKIComp_dP_T_vs_dAlpha_T_Data_FolderData, "dP_T_tot_vs_dAlpha_T_tot_pFDpCD", "dP_T_tot_vs_dAlpha_T_tot_nFDpCD", "dP_T_tot_vs_dAlpha_T_tot_FSR_2N", 1, true);
//    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    cout << "\n\nExcecution finished!\n", exit(0);

    Canv->Clear();
}
