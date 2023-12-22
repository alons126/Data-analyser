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

#include "HistPlotter1D.cpp"
#include "HistPlotter2D.cpp"

using namespace std;

void MScThesisPlotter() {
    const string SampleName = "C12x4_simulation_G18_Q204_6GeV";

    //<editor-fold desc="Runs">
    const char *NO_CUTS_beta_VS_P_no_clas12ana = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_no_clas12ana/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_NO_CUTS_with_clas12ana = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts =
            "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_DC_fiducial_cuts/DebugOutputFile.root";

    const char *plots_no_chi2 = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S01ACwoChi2/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_no_NC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S02ACnoNC/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_wNC_AMaps02 = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/02_AMaps_runs/C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps02_Eff1"
                                    "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    //</editor-fold>

    //<editor-fold desc="Make directories">

    //<editor-fold desc="Preselection_Sim">
    const string PreselectionSimFolder = "Preselection_Sim";
    system(("rm -r " + PreselectionSimFolder).c_str());
    system(("mkdir -p " + PreselectionSimFolder).c_str());

    //<editor-fold desc="Vertex cuts">
    const string VertexCutsFolderSim = "Preselection_Sim/01_Vertex_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSim).c_str());

    const string VertexCutsFolderSimBC = "Preselection_Sim/01_Vertex_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSimBC).c_str());

    const string VertexCutsFolderSimAC = "Preselection_Sim/01_Vertex_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="Vertex correlation cuts">
    const string VertexCorrCutsFolderSim = "Preselection_Sim/02_Vertex_Corr_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSim).c_str());

    const string VertexCorrCutsFolderSimBC = "Preselection_Sim/02_Vertex_Corr_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + VertexCorrCutsFolderSimBC).c_str());

    const string VertexCorrCutsFolderSimAC = "Preselection_Sim/02_Vertex_Corr_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + VertexCorrCutsFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="DC fiducial cuts">
    const string DCFiducialCutsFolderSim = "Preselection_Sim/03_DC_fiducial_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSim).c_str());

    const string DCFiducialCutsFolderSimBC = "Preselection_Sim/03_DC_fiducial_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSimBC).c_str());

    const string DCFiducialCutsFolderSimAC = "Preselection_Sim/03_DC_fiducial_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSimAC).c_str());
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Event_Selection_Sim">
    const string EventSelectionSimFolder = "Event_Selection_Sim";
    system(("rm -r " + EventSelectionSimFolder).c_str());
    system(("mkdir -p " + EventSelectionSimFolder).c_str());

    const string ElectronIDFolderSim = "Event_Selection_Sim/01_Electron_ID_Sim";
    system(("mkdir -p " + ElectronIDFolderSim).c_str());

    const string ElectronIDFolderSimBC = "Event_Selection_Sim/01_Electron_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + ElectronIDFolderSimBC).c_str());

    const string ElectronIDFolderSimAC = "Event_Selection_Sim/01_Electron_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + ElectronIDFolderSimAC).c_str());

    const string ProtonIDFolderSim = "Event_Selection_Sim/02_Proton_ID_Sim";
    system(("mkdir -p " + ProtonIDFolderSim).c_str());

    const string ProtonIDFolderSimBC = "Event_Selection_Sim/02_Proton_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + ProtonIDFolderSimBC).c_str());

    const string ProtonIDFolderSimAC = "Event_Selection_Sim/02_Proton_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + ProtonIDFolderSimAC).c_str());

    const string NeutralsFDIDFolderSim = "Event_Selection_Sim/03_Neutrals_FD_ID_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSim).c_str());

    const string NeutralsFDIDFolderSimBC = "Event_Selection_Sim/03_Neutrals_FD_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSimBC).c_str());

    const string NeutralsFDIDFolderSimAC = "Event_Selection_Sim/03_Neutrals_FD_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSimAC).c_str());

    const string PionsIDFolderSim = "Event_Selection_Sim/04_Pions_ID_Sim";
    system(("mkdir -p " + PionsIDFolderSim).c_str());

    const string PionsIDFolderSimBC = "Event_Selection_Sim/04_Pions_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + PionsIDFolderSimBC).c_str());

    const string PionsIDFolderSimAC = "Event_Selection_Sim/04_Pions_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + PionsIDFolderSimAC).c_str());

    const string OtherPartIDFolderSim = "Event_Selection_Sim/05_Other_part_ID_Sim";
    system(("mkdir -p " + OtherPartIDFolderSim).c_str());

    const string OtherPartIDFolderSimBC = "Event_Selection_Sim/05_Other_part_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + OtherPartIDFolderSimBC).c_str());

    const string OtherPartIDFolderSimAC = "Event_Selection_Sim/05_Other_part_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + OtherPartIDFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="Results">
    const string ResultsFolder = "Results";
    system(("rm -r " + ResultsFolder).c_str());
    system(("mkdir -p " + ResultsFolder).c_str());

    const string ElectronIDFolderData = "Results/Event_Selection_Data/01_Electron_ID_Data";
    system(("mkdir -p " + ElectronIDFolderData).c_str());

    const string ElectronIDFolderDataBC = "Results/Event_Selection_Data/01_Electron_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + ElectronIDFolderDataBC).c_str());

    const string ElectronIDFolderDataAC = "Results/Event_Selection_Data/01_Electron_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + ElectronIDFolderDataAC).c_str());

    const string ProtonIDFolderData = "Results/Event_Selection_Data/02_Proton_ID_Data";
    system(("mkdir -p " + ProtonIDFolderData).c_str());

    const string ProtonIDFolderDataBC = "Results/Event_Selection_Data/02_Proton_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + ProtonIDFolderDataBC).c_str());

    const string ProtonIDFolderDataAC = "Results/Event_Selection_Data/02_Proton_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + ProtonIDFolderDataAC).c_str());

    const string NeutralsFDIDFolderData = "Results/Event_Selection_Data/03_Neutrals_FD_ID_Data";
    system(("mkdir -p " + NeutralsFDIDFolderData).c_str());

    const string NeutralsFDIDFolderDataBC = "Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + NeutralsFDIDFolderDataBC).c_str());

    const string NeutralsFDIDFolderDataAC = "Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + NeutralsFDIDFolderDataAC).c_str());

    const string PionsIDFolderData = "Results/Event_Selection_Data/04_Pions_ID_Data";
    system(("mkdir -p " + PionsIDFolderData).c_str());

    const string PionsIDFolderDataBC = "Results/Event_Selection_Data/04_Pions_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + PionsIDFolderDataBC).c_str());

    const string PionsIDFolderDataAC = "Results/Event_Selection_Data/04_Pions_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + PionsIDFolderDataAC).c_str());

    const string OtherPartIDFolderData = "Results/Event_Selection_Data/05_Other_part_ID_Data";
    system(("mkdir -p " + OtherPartIDFolderData).c_str());

    const string OtherPartIDFolderDataBC = "Results/Event_Selection_Data/05_Other_part_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + OtherPartIDFolderDataBC).c_str());

    const string OtherPartIDFolderDataAC = "Results/Event_Selection_Data/05_Other_part_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + OtherPartIDFolderDataAC).c_str());
    //</editor-fold>

    //</editor-fold>

    TCanvas *c = new TCanvas("c1", "c2", 1000, 750); // normal res
    c->cd()->SetGrid(), c->cd()->SetBottomMargin(0.14), c->cd()->SetLeftMargin(0.18), c->cd()->SetRightMargin(0.12), c->cd();

    TList *MScThesisPlots = new TList();
    string listName = EventSelectionSimFolder + "/" + SampleName + ".root";
    const char *TListName = listName.c_str();

    //<editor-fold desc="Preselection">

    //<editor-fold desc="Vertex plots">
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{e} (1e cut)", SampleName,
                  VertexCutsFolderSimBC, "01_Vz_e_FD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{p} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "02_Vz_p_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{p} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "02_Vz_p_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{D} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "03_Vz_D_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{D} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "03_Vz_D_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{#pi^{+}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "04_Vz_pip_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{#pi^{+}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "04_Vz_pip_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{#pi^{-}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "05_Vz_pim_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{#pi^{-}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "05_Vz_pim_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{K^{+}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "06_Vz_Kp_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{K^{+}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "06_Vz_Kp_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{K^{-}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "07_Vz_Km_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "V_{z}^{K^{-}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "07_Vz_Km_CD");
    //</editor-fold>

    //<editor-fold desc="Vertex correlation plots">
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{p}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "01_dVz_p_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{p}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "01_dVz_p_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{D}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "02_dVz_D_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{D}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "02_dVz_D_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "03_dVz_pip_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "03_dVz_pip_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "04_dVz_pim_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "04_dVz_pim_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{K^{+}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "05_dVz_Kp_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{K^{+}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "05_dVz_Kp_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{K^{-}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "06_dVz_Km_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vertex corr. dV^{K^{-}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "06_dVz_Km_CD");
    //</editor-fold>

    //<editor-fold desc="DC fiducial cuts">
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_1", SampleName,
                  DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R1");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_2", SampleName,
                  DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R2");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_3", SampleName,
                  DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R3");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_proton_1", SampleName,
                  DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R1");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_proton_2", SampleName,
                  DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R2");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_proton_3", SampleName,
                  DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R3");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_pion_1", SampleName,
                  DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R1");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_pion_2", SampleName,
                  DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R2");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana_DC_fiducial_cuts,
                  "dc_hitmap_before_pion_3", SampleName,
                  DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R3");
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Electron ID">
    HistPlotter2D(c, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana,
                  "#beta vs. P (all particles, no #(e) cut, CD)", SampleName,
                  ElectronIDFolderSimBC, "01_Beta_vs_P_CD_all_par_no_cuts");
    HistPlotter2D(c, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana,
                  "#beta vs. P (all particles, no #(e) cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "01_Beta_vs_P_FD_all_par_no_cuts");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDFolderSimBC, "02_Beta_vs_P_CD_all_par_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "02_Beta_vs_P_FD_all_par_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "N_{phe} in HTCC BC (1e cut)", SampleName,
                  ElectronIDFolderSimBC, "03a_Nphe_in_HTCC_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "SF vs. P_{e} BC (1e cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "03_SF_vs_P_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDFolderSimBC, "04_Vcal_vs_SF_FD_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Wcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDFolderSimBC, "04_Wcal_vs_FS_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "#beta vs. P (electrons only, 1e cut)", SampleName,
                  ElectronIDFolderSimBC, "05_Beta_e_vs_P_FD_electrons_only_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDFolderSimBC, "06_Beta_vs_P_CD_all_par_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "06_Beta_vs_P_FD_all_par_1e_cut");
    //</editor-fold>

    //<editor-fold desc="Proton ID">
    /* Before cuts */
    //TODO: try to gen these chi2 histograms (WITH fit) from file!
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{p} (1e cut, FD)", SampleName,
                  ProtonIDFolderSimBC, "01_chi2_p_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{p} (1e cut, CD)", SampleName,
                  ProtonIDFolderSimBC, "01_chi2_p_CD_1e_cut");

//    //TODO: fix duplicated histogram names!
//    HistPlotter1D(c, MScThesisPlots, plots_no_NC,
//                  "#theta_{p_{1},p_{2}} (All Int., 2p)", SampleName,
//                  ProtonIDFolderSimBC, "02_Theta_p1_p2_2p");

    HistPlotter2D(c, MScThesisPlots, plots_no_NC,
                  "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimBC, "03_theta_p1_p2_vd_dToF_2p");
    HistPlotter2D(c, MScThesisPlots, plots_no_NC,
                  "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimBC, "03_theta_p1_p2_vd_dPos_2p");

    HistPlotter2D(c, MScThesisPlots, plots_no_NC,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "04_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_no_NC,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "04_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");

    HistPlotter2D(c, MScThesisPlots, plots_no_NC,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "05_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_no_NC,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "05_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");

    HistPlotter1D(c, MScThesisPlots, plots_no_NC,
                  "#delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN", SampleName,
                  ProtonIDFolderSimBC, "06_dPhi_pFD_pCD_for_small_dTheta_pFD_pCD_2p");

    /* After cuts */
    //TODO: fix duplicated histogram names!
//    HistPlotter1D(c, MScThesisPlots, plots_wNC_AMaps02,
//                  "#theta_{p_{1},p_{2}} - Opening Angle Between Protons (All Int., 2p)", SampleName,
//                  ProtonIDFolderSimAC, "02_Theta_p1_p2_2p");

    HistPlotter2D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimAC, "03_theta_p1_p2_vd_dToF_2p");
    HistPlotter2D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimAC, "03_theta_p1_p2_vd_dPos_2p");

    HistPlotter2D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "04_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "04_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");

    HistPlotter2D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "05_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "05_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");
    //</editor-fold>

    //<editor-fold desc="Neutrals ID">
    /* Before cuts */
//    //TODO: regenerate cut!
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} BV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimBC, "01_ECAL_veto_LnFD_e_1e_cut");
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{LnFD,p} vs. #Delta#phi_{LnFD,p} BV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimBC, "01_ECAL_veto_LnFD_p_1e_cut");
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimBC, "02_ECAL_veto_nFD_e_1e_cut");
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{nFD,p} vs. #Delta#phi_{nFD,p} BV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimBC, "02_ECAL_veto_nFD_p_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "Leading FD neutron momentum APID (1n, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "03_P_LnFD_1n");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "Leading FD neutron momentum APID - ZOOMOUT (1n, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "03_P_LnFD_ZOOMOUT_1n");

    /* After cuts */
//    //TODO: regenerate cut!
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} AV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimAC, "01_ECAL_veto_LnFD_e_1e_cut");
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{LnFD,p} vs. #Delta#phi_{LnFD,p} AV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimAC, "01_ECAL_veto_LnFD_p_1e_cut");
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimAC, "02_ECAL_veto_nFD_e_1e_cut");
//    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
//                  "#Delta#theta_{nFD,p} vs. #Delta#phi_{nFD,p} AV (1e cut, FD)", SampleName,
//                  NeutralsFDIDFolderSimAC, "02_ECAL_veto_nFD_p_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "Leading FD neutron momentum APID (1n, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "03_P_LnFD_1n");
    HistPlotter1D(c, MScThesisPlots, plots_wNC_AMaps02,
                  "Leading FD neutron momentum APID - ZOOMOUT (1n, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "03_P_LnFD_ZOOMOUT_1n");
    //</editor-fold>

    //<editor-fold desc="Pions ID">
    /* Before cuts */
    //TODO: try to gen these chi2 histograms (WITH fit) from file!
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{#pi^{+}} (1e cut, FD)", SampleName,
                  PionsIDFolderSimBC, "01_chi2_pip_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{#pi^{+}} (1e cut, CD)", SampleName,
                  PionsIDFolderSimBC, "01_chi2_pip_CD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{#pi^{-}} (1e cut, FD)", SampleName,
                  PionsIDFolderSimBC, "02_chi2_pim_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{#pi^{-}} (1e cut, CD)", SampleName,
                  PionsIDFolderSimBC, "02_chi2_pim_CD_1e_cut");
    //</editor-fold>

    //<editor-fold desc="Other particles ID">
    /* Before cuts */
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{D} (1e cut, FD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_D_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{D} (1e cut, CD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_D_CD_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{K^{+}} (1e cut, FD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_Kp_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{K^{+}} (1e cut, CD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_Kp_CD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{K^{-}} (1e cut, FD)", SampleName,
                  OtherPartIDFolderSimBC, "02_chi2_Km_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_no_chi2,
                  "#chi^{2}_{K^{-}} (1e cut, CD)", SampleName,
                  OtherPartIDFolderSimBC, "02_chi2_Km_CD_1e_cut");
    //</editor-fold>

//    //TODO: figure out where to save these!
//    TFile *MScThesisPlots_fout = new TFile(TListName, "recreate");
//    MScThesisPlots_fout->cd();
//    MScThesisPlots->Write();
//    MScThesisPlots_fout->Write();
//    MScThesisPlots_fout->Close();
}
