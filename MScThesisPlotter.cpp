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
    const char *NO_CUTS_beta_VS_P_no_clas12ana = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS_no_clas12ana/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_NO_CUTS_with_clas12ana = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S00_NO_CUTS/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_no_chi2 = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S01ACwoChi2/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const char *plots_no_NC = "/mnt/e/C12x4_simulation_G18_Q204_6GeV_afterTrip/01_Initial_runs/C12x4_simulation_G18_Q204_6GeV_S02ACnoNC/C12x4_simulation_G18_Q204_6GeV_plots.root";

    const string SampleName = "C12x4_simulation_G18_Q204_6GeV";

    //<editor-fold desc="Make directories">
    const string EventSelectionfolder = "Event_Selection";
    system(("rm -r " + EventSelectionfolder).c_str());
    system(("mkdir -p " + EventSelectionfolder).c_str());

    const string ElectronIDfolder = "Event_Selection/01_Electron_ID";
    system(("mkdir -p " + ElectronIDfolder).c_str());

    const string ElectronIDfolderBC = "Event_Selection/01_Electron_ID/Before_Cuts";
    system(("mkdir -p " + ElectronIDfolderBC).c_str());

    const string ElectronIDfolderAC = "Event_Selection/01_Electron_ID/After_Cuts";
    system(("mkdir -p " + ElectronIDfolderAC).c_str());

    const string ProtonIDfolder = "Event_Selection/02_Proton_ID";
    system(("mkdir -p " + ProtonIDfolder).c_str());

    const string ProtonIDfolderBC = "Event_Selection/02_Proton_ID/Before_Cuts";
    system(("mkdir -p " + ProtonIDfolderBC).c_str());

    const string ProtonIDfolderAC = "Event_Selection/02_Proton_ID/After_Cuts";
    system(("mkdir -p " + ProtonIDfolderAC).c_str());

    const string NeutralsFDIDfolder = "Event_Selection/03_Neutrals_FD_ID";
    system(("mkdir -p " + NeutralsFDIDfolder).c_str());

    const string NeutralsFDIDfolderBC = "Event_Selection/03_Neutrals_FD_ID/Before_Cuts";
    system(("mkdir -p " + NeutralsFDIDfolderBC).c_str());

    const string NeutralsFDIDfolderAC = "Event_Selection/03_Neutrals_FD_ID/After_Cuts";
    system(("mkdir -p " + NeutralsFDIDfolderAC).c_str());

    const string PionsIDfolder = "Event_Selection/04_Pions_ID";
    system(("mkdir -p " + PionsIDfolder).c_str());

    const string PionsIDfolderBC = "Event_Selection/04_Pions_ID/Before_Cuts";
    system(("mkdir -p " + PionsIDfolderBC).c_str());

    const string PionsIDfolderAC = "Event_Selection/04_Pions_ID/After_Cuts";
    system(("mkdir -p " + PionsIDfolderAC).c_str());

    const string OtherPartIDfolder = "Event_Selection/05_Other_part_ID";
    system(("mkdir -p " + OtherPartIDfolder).c_str());

    const string OtherPartIDfolderBC = "Event_Selection/05_Other_part_ID/Before_Cuts";
    system(("mkdir -p " + OtherPartIDfolderBC).c_str());

    const string OtherPartIDfolderAC = "Event_Selection/05_Other_part_ID/After_Cuts";
    system(("mkdir -p " + OtherPartIDfolderAC).c_str());
    //</editor-fold>

    TCanvas *c = new TCanvas("c1", "c2", 1000, 750); // normal res
    c->cd()->SetGrid(), c->cd()->SetBottomMargin(0.14), c->cd()->SetLeftMargin(0.18), c->cd()->SetRightMargin(0.12), c->cd();

    TList *MScThesisPlots = new TList();
    string listName = EventSelectionfolder + "/" + SampleName + ".root";
    const char *TListName = listName.c_str();

    HistPlotter2D(c, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana,
                  "#beta vs. P (all particles, no #(e) cut, CD)", SampleName,
                  ElectronIDfolderBC, "01_Beta_vs_P_CD_all_par_no_cuts");
    HistPlotter2D(c, MScThesisPlots, NO_CUTS_beta_VS_P_no_clas12ana,
                  "#beta vs. P (all particles, no #(e) cut, FD)", SampleName,
                  ElectronIDfolderBC, "01_Beta_vs_P_FD_all_par_no_cuts");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDfolderBC, "02_Beta_vs_P_CD_all_par_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDfolderBC, "02_Beta_vs_P_FD_all_par_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "SF vs. P_{e} BC (1e cut, FD)", SampleName,
                  ElectronIDfolderBC, "03_SF_vs_P_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Vcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDfolderBC, "04_Vcal_vs_SF_FD_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "Wcal vs. SF BC (1e cut, PCAL)", SampleName,
                  ElectronIDfolderBC, "04_Wcal_vs_FS_FD_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_NO_CUTS_with_clas12ana,
                  "#beta vs. P (electrons only, 1e cut)", SampleName,
                  ElectronIDfolderBC, "05_Beta_e_vs_P_FD_electrons_only_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#beta vs. P (all particles, 1e cut, CD)", SampleName,
                  ElectronIDfolderBC, "06_Beta_vs_P_CD_all_par_1e_cut");
    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#beta vs. P (all particles, 1e cut, FD)", SampleName,
                  ElectronIDfolderBC, "06_Beta_vs_P_FD_all_par_1e_cut");

    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#theta_{p_{1},p_{2}} vs. ToF_{1}-ToF_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDfolderBC, "0x_theta_p1_p2_vd_dToF");
    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#theta_{p_{1},p_{2}} vs. Position_{1}-Position_{2} AC (2p, CD-CTOF)", SampleName,
                  ProtonIDfolderBC, "0x_theta_p1_p2_vd_dPos");

    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#theta_{pFD} vs. #theta_{pCD} #forall#theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDfolderBC, "0x2_theta_pFD_vs_pCD_for_all_theta_pFD_pCD");
    HistPlotter2D(c, MScThesisPlots, plots_no_chi2,
                  "#theta_{pFD} vs. #theta_{pCD} for #theta_{pFD,pCD}<20#circ (All Int., 2p)", SampleName,
                  ProtonIDfolderBC, "0x2_theta_pFD_vs_pCD_for_small_theta_pFD_pCD");

    TFile *MScThesisPlots_fout = new TFile(TListName, "recreate");
    MScThesisPlots_fout->cd();
    MScThesisPlots->Write();
    MScThesisPlots_fout->Write();
    MScThesisPlots_fout->Close();
}
