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
#include "AMapsPlotter.cpp"
#include "GraphPlotter1D.cpp"

using namespace std;

/* sudo mount -t drvfs E: /mnt/e */

void MScThesisPlotter() {
    hData utilities;

    const string SampleName = "C12x4_simulation_G18_Q204_6GeV";

    //<editor-fold desc="Runs">

    //<editor-fold desc="Initial runs">
    const char *NO_CUTS_beta_VS_P_no_clas12ana = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/"
                                                 "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_no_clas12ana_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_NO_CUTS = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_v2"
                                "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_NO_CUTS_DC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_v2"
                                   "/DebugOutputFile.root";
    const char *plots_Preselection_only = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/"
                                          "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_Preselection_Cuts_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_Preselection_only_DC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/"
                                             "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_Preselection_Cuts_v2/DebugOutputFile.root";
    const char *plots_eQC_only = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_eQC_v2"
                                 "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_PreselectionAndeQC_only = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/"
                                                "C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_Preselection_Cuts_eQC_v2/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_woChi2 = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S01ACwoChi2_v2"
                               "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_noNC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S02ACnoNC_v2"
                             "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_NC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_Eff1_v2"
                           "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    //</editor-fold>

    //<editor-fold desc="AMaps runs">
    const char *plots_AMaps = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/02_AMaps_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps_Eff1_v2"
                              "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_AMaps_plotsPath = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/02_AMaps_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps_Eff1_v2/";
    const char *plots_AMaps_GenMaps = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/02_AMaps_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps_Eff1_v2/"
                                      "12_Acceptance_maps_plots/01_AMaps_1e_cut/05_Generated_maps/";

//    const char *plots_wNC_AMaps02 = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/02_AMaps_runs/C12x4_simulation_G18_Q204_6GeV_S03ACNC_AMaps02_Eff1"
//                                    "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    //</editor-fold>

    //<editor-fold desc="Efficiency runs">
    const char *plots_Efficiency_reg = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/04_Efficiency_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_Eff2_v2"
                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_Efficiency_wFC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/04_Efficiency_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_wFC_Eff2_v2"
                                       "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    //</editor-fold>

    //<editor-fold desc="momRes runs">
    const char *plots_momResS1_Gen = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_nResS1_Eff1_v2"
                                     "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_nResS1_FittedPlots = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_nResS1_Eff1_v2"
                                           "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_pResS1_FittedPlots = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_nResS1_Eff1_v2"
                                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_momResS2_Gen = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_wNSaS_nResS2_Eff1_v2"
                                     "/C12x4_simulation_G18_Q204_6GeV_plots.root";
    const char *plots_nResS2_FittedPlots = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_wNSaS_nResS2_Eff1_v2"
                                           "/Neutron_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    const char *plots_pResS2_FittedPlots = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/03_momRes_runs/v2/C12x4_simulation_G18_Q204_6GeV_S03ACNC_wNSaS_nResS2_Eff1_v2"
                                           "/Proton_resolution_plots_-_C12x4_simulation_G18_Q204_6GeV.root";
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Make directories">

    //<editor-fold desc="Preselection_Sim">
    const string PreselectionSimFolder = "MScThesisPlotter/Preselection_Sim";
    system(("rm -r " + PreselectionSimFolder).c_str());
    system(("mkdir -p " + PreselectionSimFolder).c_str());

    //<editor-fold desc="Vertex cuts">
    const string VertexCutsFolderSim = "MScThesisPlotter/Preselection_Sim/01_Vertex_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSim).c_str());

    const string VertexCutsFolderSimBC = "MScThesisPlotter/Preselection_Sim/01_Vertex_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSimBC).c_str());

    const string VertexCutsFolderSimAC = "MScThesisPlotter/Preselection_Sim/01_Vertex_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="Vertex correlation cuts">
    const string VertexCorrCutsFolderSim = "MScThesisPlotter/Preselection_Sim/02_Vertex_Corr_Cuts_Sim";
    system(("mkdir -p " + VertexCutsFolderSim).c_str());

    const string VertexCorrCutsFolderSimBC = "MScThesisPlotter/Preselection_Sim/02_Vertex_Corr_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + VertexCorrCutsFolderSimBC).c_str());

    const string VertexCorrCutsFolderSimAC = "MScThesisPlotter/Preselection_Sim/02_Vertex_Corr_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + VertexCorrCutsFolderSimAC).c_str());
    //</editor-fold>

    //<editor-fold desc="DC fiducial cuts">
    const string DCFiducialCutsFolderSim = "MScThesisPlotter/Preselection_Sim/03_DC_fiducial_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSim).c_str());

    const string DCFiducialCutsFolderSimBC = "MScThesisPlotter/Preselection_Sim/03_DC_fiducial_Cuts_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSimBC).c_str());

    const string DCFiducialCutsFolderSimAC = "MScThesisPlotter/Preselection_Sim/03_DC_fiducial_Cuts_Sim/After_Cuts_Sim";
    system(("mkdir -p " + DCFiducialCutsFolderSimAC).c_str());
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Event_Selection_Sim">
    const string EventSelectionSimFolder = "MScThesisPlotter/Event_Selection_Sim";
    system(("rm -r " + EventSelectionSimFolder).c_str());
    system(("mkdir -p " + EventSelectionSimFolder).c_str());

    const string ElectronIDFolderSim = "MScThesisPlotter/Event_Selection_Sim/01_Electron_ID_Sim";
    system(("mkdir -p " + ElectronIDFolderSim).c_str());

    const string ElectronIDFolderSimBC = "MScThesisPlotter/Event_Selection_Sim/01_Electron_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + ElectronIDFolderSimBC).c_str());

    const string ElectronIDFolderSimAC = "MScThesisPlotter/Event_Selection_Sim/01_Electron_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + ElectronIDFolderSimAC).c_str());

    const string ProtonIDFolderSim = "MScThesisPlotter/Event_Selection_Sim/02_Proton_ID_Sim";
    system(("mkdir -p " + ProtonIDFolderSim).c_str());

    const string ProtonIDFolderSimBC = "MScThesisPlotter/Event_Selection_Sim/02_Proton_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + ProtonIDFolderSimBC).c_str());

    const string ProtonIDFolderSimAC = "MScThesisPlotter/Event_Selection_Sim/02_Proton_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + ProtonIDFolderSimAC).c_str());

    const string NeutralsFDIDFolderSim = "MScThesisPlotter/Event_Selection_Sim/03_Neutrals_FD_ID_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSim).c_str());

    const string NeutralsFDIDFolderSimBC = "MScThesisPlotter/Event_Selection_Sim/03_Neutrals_FD_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSimBC).c_str());

    const string NeutralsFDIDFolderSimAC = "MScThesisPlotter/Event_Selection_Sim/03_Neutrals_FD_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + NeutralsFDIDFolderSimAC).c_str());

    const string PionsIDFolderSim = "MScThesisPlotter/Event_Selection_Sim/04_Pions_ID_Sim";
    system(("mkdir -p " + PionsIDFolderSim).c_str());

    const string PionsIDFolderSimBC = "MScThesisPlotter/Event_Selection_Sim/04_Pions_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + PionsIDFolderSimBC).c_str());

    const string PionsIDFolderSimAC = "MScThesisPlotter/Event_Selection_Sim/04_Pions_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + PionsIDFolderSimAC).c_str());

    const string OtherPartIDFolderSim = "MScThesisPlotter/Event_Selection_Sim/05_Other_part_ID_Sim";
    system(("mkdir -p " + OtherPartIDFolderSim).c_str());

    const string OtherPartIDFolderSimBC = "MScThesisPlotter/Event_Selection_Sim/05_Other_part_ID_Sim/Before_Cuts_Sim";
    system(("mkdir -p " + OtherPartIDFolderSimBC).c_str());

    const string OtherPartIDFolderSimAC = "MScThesisPlotter/Event_Selection_Sim/05_Other_part_ID_Sim/After_Cuts_Sim";
    system(("mkdir -p " + OtherPartIDFolderSimAC).c_str());

    const string AMapsFolderSim = "MScThesisPlotter/Event_Selection_Sim/06_AMaps";
    system(("mkdir -p " + AMapsFolderSim).c_str());

    const string AMapsFolderMapsSim = "MScThesisPlotter/Event_Selection_Sim/06_AMaps/01_Maps";
    system(("mkdir -p " + AMapsFolderMapsSim).c_str());

    const string AMapsFolderMomentumPlotsSim = "MScThesisPlotter/Event_Selection_Sim/06_AMaps/02_Momentum_Plots";
    system(("mkdir -p " + AMapsFolderMomentumPlotsSim).c_str());

    const string EfficiencyFolderSim = "MScThesisPlotter/Event_Selection_Sim/07_Efficiency";
    system(("mkdir -p " + EfficiencyFolderSim).c_str());

    const string EfficiencyRegFolderSim = "MScThesisPlotter/Event_Selection_Sim/07_Efficiency/01_No_FC";
    system(("mkdir -p " + EfficiencyRegFolderSim).c_str());

    const string EfficiencyWithFCFolderSim = "MScThesisPlotter/Event_Selection_Sim/07_Efficiency/02_wFC";
    system(("mkdir -p " + EfficiencyWithFCFolderSim).c_str());

    const string MomResFolderSim = "MScThesisPlotter/Event_Selection_Sim/08_MomRes";
    system(("mkdir -p " + MomResFolderSim).c_str());

    const string MomResS1FolderSim = "MScThesisPlotter/Event_Selection_Sim/08_MomRes/01_momResS1_Calc";
    system(("mkdir -p " + MomResS1FolderSim).c_str());

    const string MomResS2FolderSim = "MScThesisPlotter/Event_Selection_Sim/08_MomRes/02_momResS2_Calc";
    system(("mkdir -p " + MomResS2FolderSim).c_str());
    //</editor-fold>

    //<editor-fold desc="Results">
    const string ResultsFolder = "MScThesisPlotter/Results";
    system(("rm -r " + ResultsFolder).c_str());
    system(("mkdir -p " + ResultsFolder).c_str());

    const string ElectronIDFolderData = "MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data";
    system(("mkdir -p " + ElectronIDFolderData).c_str());

    const string ElectronIDFolderDataBC = "MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + ElectronIDFolderDataBC).c_str());

    const string ElectronIDFolderDataAC = "MScThesisPlotter/Results/Event_Selection_Data/01_Electron_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + ElectronIDFolderDataAC).c_str());

    const string ProtonIDFolderData = "MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data";
    system(("mkdir -p " + ProtonIDFolderData).c_str());

    const string ProtonIDFolderDataBC = "MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + ProtonIDFolderDataBC).c_str());

    const string ProtonIDFolderDataAC = "MScThesisPlotter/Results/Event_Selection_Data/02_Proton_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + ProtonIDFolderDataAC).c_str());

    const string NeutralsFDIDFolderData = "MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data";
    system(("mkdir -p " + NeutralsFDIDFolderData).c_str());

    const string NeutralsFDIDFolderDataBC = "MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + NeutralsFDIDFolderDataBC).c_str());

    const string NeutralsFDIDFolderDataAC = "MScThesisPlotter/Results/Event_Selection_Data/03_Neutrals_FD_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + NeutralsFDIDFolderDataAC).c_str());

    const string PionsIDFolderData = "MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data";
    system(("mkdir -p " + PionsIDFolderData).c_str());

    const string PionsIDFolderDataBC = "MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + PionsIDFolderDataBC).c_str());

    const string PionsIDFolderDataAC = "MScThesisPlotter/Results/Event_Selection_Data/04_Pions_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + PionsIDFolderDataAC).c_str());

    const string OtherPartIDFolderData = "MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data";
    system(("mkdir -p " + OtherPartIDFolderData).c_str());

    const string OtherPartIDFolderDataBC = "MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data/Before_Cuts_Data";
    system(("mkdir -p " + OtherPartIDFolderDataBC).c_str());

    const string OtherPartIDFolderDataAC = "MScThesisPlotter/Results/Event_Selection_Data/05_Other_part_ID_Data/After_Cuts_Data";
    system(("mkdir -p " + OtherPartIDFolderDataAC).c_str());
    //</editor-fold>

    //</editor-fold>

    TCanvas *c = new TCanvas("c1", "c2", utilities.GetStandardCanvasWidth() * 1.25, utilities.GetStandardCanvasHeight() * 1.25); // normal res
    c->cd()->SetGrid(), c->cd()->SetBottomMargin(0.14), c->cd()->SetLeftMargin(0.16), c->cd()->SetRightMargin(0.16), c->cd()->SetTopMargin(0.12), c->cd();

    TList *MScThesisPlots = new TList();
    string listName = EventSelectionSimFolder + "/" + SampleName + ".root";
    const char *TListName = listName.c_str();

    //<editor-fold desc="Preselection">

    //<editor-fold desc="Vertex plots">
    /* Before cuts */
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{e} (1e cut)", SampleName,
                  VertexCutsFolderSimBC, "01_Vz_e_FD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{p} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "02_Vz_p_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{p} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "02_Vz_p_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{D} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "03_Vz_D_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{D} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "03_Vz_D_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{#pi^{+}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "04_Vz_pip_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{#pi^{+}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "04_Vz_pip_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{#pi^{-}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "05_Vz_pim_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{#pi^{-}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "05_Vz_pim_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{K^{+}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "06_Vz_Kp_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{K^{+}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "06_Vz_Kp_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{K^{-}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimBC, "07_Vz_Km_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "V_{z}^{K^{-}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimBC, "07_Vz_Km_CD");

    /* After cuts */
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{e} (1e cut)", SampleName,
                  VertexCutsFolderSimAC, "01_Vz_e_FD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{p} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimAC, "02_Vz_p_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{p} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimAC, "02_Vz_p_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{D} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimAC, "03_Vz_D_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{D} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimAC, "03_Vz_D_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{#pi^{+}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimAC, "04_Vz_pip_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{#pi^{+}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimAC, "04_Vz_pip_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{#pi^{-}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimAC, "05_Vz_pim_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{#pi^{-}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimAC, "05_Vz_pim_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{K^{+}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimAC, "06_Vz_Kp_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{K^{+}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimAC, "06_Vz_Kp_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{K^{-}} (1e cut, FD)", SampleName,
                  VertexCutsFolderSimAC, "07_Vz_Km_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "V_{z}^{K^{-}} (1e cut, CD)", SampleName,
                  VertexCutsFolderSimAC, "07_Vz_Km_CD");
    //</editor-fold>

    //<editor-fold desc="Vertex correlation plots">
    /* Before cuts */
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{p}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "01_dVz_p_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{p}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "01_dVz_p_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{D}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "02_dVz_D_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{D}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "02_dVz_D_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "03_dVz_pip_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "03_dVz_pip_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "04_dVz_pim_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "04_dVz_pim_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{K^{+}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "05_dVz_Kp_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{K^{+}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "05_dVz_Kp_CD");

    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{K^{-}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimBC, "06_dVz_Km_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NO_CUTS,
                  "Vertex corr. dV^{K^{-}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimBC, "06_dVz_Km_CD");

    /* After cuts */
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{p}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimAC, "01_dVz_p_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{p}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimAC, "01_dVz_p_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{D}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimAC, "02_dVz_D_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{D}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimAC, "02_dVz_D_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimAC, "03_dVz_pip_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{#pi^{+}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimAC, "03_dVz_pip_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimAC, "04_dVz_pim_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{#pi^{-}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimAC, "04_dVz_pim_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{K^{+}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimAC, "05_dVz_Kp_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{K^{+}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimAC, "05_dVz_Kp_CD");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{K^{-}}_{z} (1e cut, FD)", SampleName,
                  VertexCorrCutsFolderSimAC, "06_dVz_Km_FD");
    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "Vertex corr. dV^{K^{-}}_{z} (1e cut, CD)", SampleName,
                  VertexCorrCutsFolderSimAC, "06_dVz_Km_CD");
    //</editor-fold>

    //<editor-fold desc="DC fiducial cuts">
    /* Before cuts */
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_1", SampleName,
                  DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R1");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_2", SampleName,
                  DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R2");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_3", SampleName,
                  DCFiducialCutsFolderSimBC, "01_dc_hitmap_e_R3");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_proton_1", SampleName,
                  DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R1");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_proton_2", SampleName,
                  DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R2");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_proton_3", SampleName,
                  DCFiducialCutsFolderSimBC, "02_dc_hitmap_p_R3");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_pion_1", SampleName,
                  DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R1");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_pion_2", SampleName,
                  DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R2");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_DC,
                  "dc_hitmap_before_pion_3", SampleName,
                  DCFiducialCutsFolderSimBC, "03_dc_hitmap_pi_R3");

    /* After cuts */
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_1", SampleName,
                  DCFiducialCutsFolderSimAC, "01_dc_hitmap_e_R1");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_2", SampleName,
                  DCFiducialCutsFolderSimAC, "01_dc_hitmap_e_R2");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_3", SampleName,
                  DCFiducialCutsFolderSimAC, "01_dc_hitmap_e_R3");

    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_proton_1", SampleName,
                  DCFiducialCutsFolderSimAC, "02_dc_hitmap_p_R1");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_proton_2", SampleName,
                  DCFiducialCutsFolderSimAC, "02_dc_hitmap_p_R2");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_proton_3", SampleName,
                  DCFiducialCutsFolderSimAC, "02_dc_hitmap_p_R3");

    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_pion_1", SampleName,
                  DCFiducialCutsFolderSimAC, "03_dc_hitmap_pi_R1");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_pion_2", SampleName,
                  DCFiducialCutsFolderSimAC, "03_dc_hitmap_pi_R2");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only_DC,
                  "dc_hitmap_after_pion_3", SampleName,
                  DCFiducialCutsFolderSimAC, "03_dc_hitmap_pi_R3");
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Electron ID">
    /* Before cuts */
    HistPlotter2D(c, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana,
                  "#beta vs. P (all particles, no #(e) cut, CD)", SampleName,
                  ElectronIDFolderSimBC, "01_Beta_vs_P_CD_all_par_no_cuts");
    HistPlotter2D(c, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana,
                  "#beta vs. P (all particles, no #(e) cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "01_Beta_vs_P_FD_all_par_no_cuts");

    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDFolderSimBC, "02_Beta_vs_P_CD_all_par_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "02_Beta_vs_P_FD_all_par_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_Preselection_only,
                  "N_{phe} in HTCC BC (1e cut)", SampleName,
                  ElectronIDFolderSimBC, "03a_Nphe_in_HTCC_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only,
                  "SF vs. P_{e} BC (1e cut, FD)", SampleName,
                  ElectronIDFolderSimBC, "03_SF_vs_P_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only,
                  "Vcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDFolderSimBC, "04_Vcal_vs_SF_FD_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only,
                  "Wcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDFolderSimBC, "04_Wcal_vs_FS_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_Preselection_only,
                  "#beta vs. P (electrons only, 1e cut)", SampleName,
                  ElectronIDFolderSimBC, "05_Beta_e_vs_P_FD_electrons_only_1e_cut");

    /* After cuts */
    HistPlotter2D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDFolderSimAC, "02_Beta_vs_P_CD_all_par_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDFolderSimAC, "02_Beta_vs_P_FD_all_par_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "N_{phe} in HTCC BC (1e cut)", SampleName,
                  ElectronIDFolderSimAC, "03a_Nphe_in_HTCC_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "SF vs. P_{e} BC (1e cut, FD)", SampleName,
                  ElectronIDFolderSimAC, "03_SF_vs_P_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "Vcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDFolderSimAC, "04_Vcal_vs_SF_FD_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "Wcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDFolderSimAC, "04_Wcal_vs_FS_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_PreselectionAndeQC_only,
                  "#beta vs. P (electrons only, 1e cut)", SampleName,
                  ElectronIDFolderSimAC, "05_Beta_e_vs_P_FD_electrons_only_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDFolderSimAC, "06_Beta_vs_P_CD_all_par_all_cuts_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDFolderSimAC, "06_Beta_vs_P_FD_all_par_all_cuts_1e_cut");
    //</editor-fold>

    //<editor-fold desc="Proton ID">
    /* Before cuts */
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{p} (1e cut, FD)", SampleName,
                  ProtonIDFolderSimBC, "01_chi2_p_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{p} (1e cut, CD)", SampleName,
                  ProtonIDFolderSimBC, "01_chi2_p_CD_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "FD Proton momentum #epsilon_{eff} (1e cut)", SampleName,
                  ProtonIDFolderSimBC, "02_P_pFD_eff_1e_cut_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "CD Proton momentum #epsilon_{eff} (1e cut)", SampleName,
                  ProtonIDFolderSimBC, "02_P_pCD_eff_1e_cut_CD");

    HistPlotter1D(c, MScThesisPlots, plots_noNC,
                  "#theta_{p_{1},p_{2}} (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "02_Theta_p1_p2_2p");

    HistPlotter2D(c, MScThesisPlots, plots_noNC,
                  "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimBC, "03_theta_p1_p2_vd_dToF_2p");
    HistPlotter2D(c, MScThesisPlots, plots_noNC,
                  "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimBC, "03_theta_p1_p2_vd_dPos_2p");

    HistPlotter2D(c, MScThesisPlots, plots_noNC,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "04_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_noNC,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimBC, "04_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");

    HistPlotter1D(c, MScThesisPlots, plots_noNC,
                  "#Delta#phi for small #Delta#theta_{pFD/pCD} - ZOOMIN", SampleName,
                  ProtonIDFolderSimBC, "05_dPhi_pFD_pCD_for_small_dTheta_pFD_pCD_2p");

    /* After cuts */
    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "#theta_{p_{1},p_{2}} (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "02_Theta_p1_p2_2p");

    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimAC, "03_theta_p1_p2_vd_dToF_2p");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDFolderSimAC, "03_theta_p1_p2_vd_dPos_2p");

    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD} (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "04_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDFolderSimAC, "04_theta_pFD_vs_pCD_for_small_theta_pFD_pCD_2p");
    //</editor-fold>

    //<editor-fold desc="Neutrals ID">
    /* ECAL veto before cuts */
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} BV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "01_ECAL_veto_LnFD_e_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} BV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "01_ECAL_veto_LnFD_pDF_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} BV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "02_ECAL_veto_nFD_e_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} BV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "02_ECAL_veto_nFD_pFD_1e_cut");

    /* ECAL veto after cuts */
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{LnFD,e} vs. #Delta#phi_{LnFD,e} AV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "01_ECAL_veto_LnFD_e_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{LnFD,pFD} vs. #Delta#phi_{LnFD,pFD} AV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "01_ECAL_veto_LnFD_pDF_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{nFD,e} vs. #Delta#phi_{nFD,e} AV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "02_ECAL_veto_nFD_e_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NC,
                  "#Delta#theta_{nFD,pFD} vs. #Delta#phi_{nFD,pFD} AV (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "02_ECAL_veto_nFD_pFD_1e_cut");

    /* Before nFD momentum threshold */
    HistPlotter1D(c, MScThesisPlots, plots_noNC,
                  "Reco FD neutrons momentum (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "03a_P_nFD_1e_cut_FD");
    HistPlotter1D(c, MScThesisPlots, plots_noNC,
                  "Reco FD neutron momentum - ZOOMOUT (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimBC, "03b_P_nFD_ZOOMOUT_1e_cut_FD");

    /* After nFD momentum threshold */
    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "Reco FD neutrons momentum (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "03a_P_nFD_1e_cut_FD");
    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "Reco FD neutron momentum - ZOOMOUT (1e cut, FD)", SampleName,
                  NeutralsFDIDFolderSimAC, "03b_P_nFD_ZOOMOUT_1e_cut_FD");
    //</editor-fold>

    //<editor-fold desc="Pions ID">
    /* Before cuts */
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{#pi^{+}} (1e cut, FD)", SampleName,
                  PionsIDFolderSimBC, "01_chi2_pip_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{#pi^{+}} (1e cut, CD)", SampleName,
                  PionsIDFolderSimBC, "01_chi2_pip_CD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{#pi^{-}} (1e cut, FD)", SampleName,
                  PionsIDFolderSimBC, "02_chi2_pim_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{#pi^{-}} (1e cut, CD)", SampleName,
                  PionsIDFolderSimBC, "02_chi2_pim_CD_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "CD & FD Piplus momentum #epsilon_{eff} (1e cut)", SampleName,
                  PionsIDFolderSimBC, "03_P_pip_eff_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_NC,
                  "CD & FD Piminus momentum #epsilon_{eff} (1e cut)", SampleName,
                  PionsIDFolderSimBC, "04_P_pim_eff_1e_cut");
    //</editor-fold>

    //<editor-fold desc="Other particles ID">
    /* Before cuts */
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{D} (1e cut, FD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_D_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{D} (1e cut, CD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_D_CD_1e_cut");

    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{K^{+}} (1e cut, FD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_Kp_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{K^{+}} (1e cut, CD)", SampleName,
                  OtherPartIDFolderSimBC, "01_chi2_Kp_CD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{K^{-}} (1e cut, FD)", SampleName,
                  OtherPartIDFolderSimBC, "02_chi2_Km_FD_1e_cut");
    HistPlotter1D(c, MScThesisPlots, plots_woChi2,
                  "#chi^{2}_{K^{-}} (1e cut, CD)", SampleName,
                  OtherPartIDFolderSimBC, "02_chi2_Km_CD_1e_cut");
    //</editor-fold>

    //<editor-fold desc="AMaps">
    string TL_root_file_prefix = "01_AMap_TL_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Reco_root_file_prefix = "02_AMap_Reco_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Ratio_root_file_prefix = "03_AMap_Ratio_-_C12x4_simulation_G18_Q204_6GeV.root";
    string cPart_Sep_AMaps_root_file_prefix = "04_cPart_Sep_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";
    string Finalized_AMaps_root_file_prefix = "05_AMaps_-_C12x4_simulation_G18_Q204_6GeV.root";

    AMapsPlotter(plots_AMaps_plotsPath, (AMapsFolderMapsSim + "/"));

    HistPlotter1D(c, MScThesisPlots, plots_AMaps,
                  "Reco P_{e} used in AMaps (1e cut, FD)", SampleName,
                  AMapsFolderMomentumPlotsSim, "01_reco_P_e");
    HistPlotter1D(c, MScThesisPlots, plots_AMaps,
                  "TL P_{e} used in AMaps (1e cut, FD)", SampleName,
                  AMapsFolderMomentumPlotsSim, "01_TL_P_e");

    HistPlotter1D(c, MScThesisPlots, plots_AMaps,
                  "Reco P_{pFD} used in AMaps (1e cut, FD)", SampleName,
                  AMapsFolderMomentumPlotsSim, "02_reco_P_pFD");
    HistPlotter1D(c, MScThesisPlots, plots_AMaps,
                  "TL P_{pFD} used in AMaps (1e cut, FD)", SampleName,
                  AMapsFolderMomentumPlotsSim, "02_TL_P_pFD");

    HistPlotter1D(c, MScThesisPlots, plots_AMaps,
                  "Reco P_{nFD} used in AMaps (1e cut, FD)", SampleName,
                  AMapsFolderMomentumPlotsSim, "03_reco_P_nFD");
    HistPlotter1D(c, MScThesisPlots, plots_AMaps,
                  "TL P_{nFD} used in AMaps (1e cut, FD)", SampleName,
                  AMapsFolderMomentumPlotsSim, "03_TL_P_nFD");
    //</editor-fold>

    //<editor-fold desc="Efficiency">
    /* No fiducial cuts */
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "Electron momentum #epsilon_{eff} (1p)", SampleName,
                  EfficiencyRegFolderSim, "01_electron_mom_eff_1p");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "Electron theta #epsilon_{eff} (1p)", SampleName,
                  EfficiencyRegFolderSim, "01_electron_theta_eff_1p");

    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "FD Proton momentum #epsilon_{eff} (1p)", SampleName,
                  EfficiencyRegFolderSim, "02_proton_mom_eff_1p");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "Proton theta #epsilon_{eff} (1p)", SampleName,
                  EfficiencyRegFolderSim, "02_proton_theta_eff_1p");

    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "Electron momentum #epsilon_{eff} (1n)", SampleName,
                  EfficiencyRegFolderSim, "03_electron_mom_eff_1n");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "Electron theta #epsilon_{eff} (1n)", SampleName,
                  EfficiencyRegFolderSim, "03_electron_theta_eff_1n");

    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "FD Leading neutron momentum #epsilon_{eff} (1n)", SampleName,
                  EfficiencyRegFolderSim, "04_neutron_mom_eff_1n");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_reg,
                  "Neutron theta #epsilon_{eff} (1n)", SampleName,
                  EfficiencyRegFolderSim, "04_neutron_theta_eff_1n");

    /* With fiducial cuts */
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "Electron momentum #epsilon_{eff} (1p)", SampleName,
                  EfficiencyWithFCFolderSim, "01_electron_mom_eff_1p");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "Electron theta #epsilon_{eff} (1p)", SampleName,
                  EfficiencyWithFCFolderSim, "01_electron_theta_eff_1p");

    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "FD Proton momentum #epsilon_{eff} (1p)", SampleName,
                  EfficiencyWithFCFolderSim, "02_proton_mom_eff_1p");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "Proton theta #epsilon_{eff} (1p)", SampleName,
                  EfficiencyWithFCFolderSim, "02_proton_theta_eff_1p");

    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "Electron momentum #epsilon_{eff} (1n)", SampleName,
                  EfficiencyWithFCFolderSim, "03_electron_mom_eff_1n");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "Electron theta #epsilon_{eff} (1n)", SampleName,
                  EfficiencyWithFCFolderSim, "03_electron_theta_eff_1n");

    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "FD Leading neutron momentum #epsilon_{eff} (1n)", SampleName,
                  EfficiencyWithFCFolderSim, "04_neutron_mom_eff_1n");
    HistPlotter1D(c, MScThesisPlots, plots_Efficiency_wFC,
                  "Neutron theta #epsilon_{eff} (1n)", SampleName,
                  EfficiencyWithFCFolderSim, "04_neutron_theta_eff_1n");
    //</editor-fold>

    //<editor-fold desc="momRes">
    /* Mean fit before correction */
    GraphPlotter1D(MScThesisPlots, plots_nResS1_FittedPlots, "Fitted neutron resolution slice width", "g_Std_pol1_wPC",
                   SampleName, MomResS1FolderSim, "01_g_Std_pol1_wPC");
    GraphPlotter1D(MScThesisPlots, plots_nResS1_FittedPlots, "Fitted neutron resolution slice mean", "g_Corr_pol1_wPC",
                   SampleName, MomResS1FolderSim, "02_g_Corr_pol1_wPC_Important"); // Important! (correction!)

    /* Width fit after correction */
    GraphPlotter1D(MScThesisPlots, plots_nResS2_FittedPlots, "Fitted neutron resolution slice width", "g_Std_pol1_wPC",
                   SampleName, MomResS2FolderSim, "01_g_Std_pol1_wPC_Important"); // Important! (smearing!)
    GraphPlotter1D(MScThesisPlots, plots_nResS2_FittedPlots, "Fitted neutron resolution slice mean", "g_Corr_pol1_wPC",
                   SampleName, MomResS2FolderSim, "02_g_Corr_pol1_wPC");

//    /* momRes test */
//    GraphPlotter1D(c, MScThesisPlots, plots_nResS2_FittedPlots, "Fitted neutron resolution slice width", "g_Std_pol1_wPC",
//                   SampleName, MomResS2FolderSim, "01_g_Std_pol1_wPC"); // Important!
//    GraphPlotter1D(c, MScThesisPlots, plots_nResS2_FittedPlots, "Fitted neutron resolution slice mean", "g_Corr_pol1_wPC",
//                   SampleName, MomResS2FolderSim, "02_g_Corr_pol1_wPC");
    //</editor-fold>

//    //TODO: figure out where to save these!
//    TFile *MScThesisPlots_fout = new TFile(TListName, "recreate");
//    MScThesisPlots_fout->cd();
//    MScThesisPlots->Write();
//    MScThesisPlots_fout->Write();
//    MScThesisPlots_fout->Close();
}
