
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

// TODO: finish Histogram class

/* The final states selected here are: //TODO: finish code explanation
 * All e / no #(e) cut: Two scenarios:
    - My cuts: plots are with no cuts
    - With clas12ana cuts: plots are with class12ana cuts yet not constraint on #electrons
 *  1e cut:
 *
 * */

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

    //<editor-fold desc="Determine file path and name">
    string LoadedInput = AnalyseFile; /* AnalyseFile is taken from codeSetup.h */
    string filePath = LoadedInput.substr(0, LoadedInput.find_last_of('/') + 1);
    string fileInput = LoadedInput.substr(LoadedInput.find_last_of('/') + 1);
    string plotsInput = fileInput.substr(0, fileInput.find_last_of(".root") - 4);
    //</editor-fold>

    //<editor-fold desc="Configure beam energy (beamE)">
    double beamE = 5.98636; // electron energy declaration
//    double beamE = getBeanE(AnalyseFile); // electron energy declaration
    //</editor-fold>

    //<editor-fold desc="Configure target">
    TargetParameters ScattringTarget;

    if ((AnalyseFileSample.find("c12") <= AnalyseFileSample[AnalyseFileSample.size() - 1]) ||
        (AnalyseFileSample.find("C12") <= AnalyseFileSample[AnalyseFileSample.size() - 1]) ||
        (AnalyseFileSample.find("12c") <= AnalyseFileSample[AnalyseFileSample.size() - 1]) ||
        (AnalyseFileSample.find("12C") <= AnalyseFileSample[AnalyseFileSample.size() - 1]) ||
        (AnalyseFileSample.find("_c_") <= AnalyseFileSample[AnalyseFileSample.size() - 1]) ||
        (AnalyseFileSample.find("_C_") <= AnalyseFileSample[AnalyseFileSample.size() - 1])) {
        ScattringTarget.SetTargetElement("C12");
        ScattringTarget.SetTargetElementPDG(1000060120);
    } else {
        ScattringTarget.SetTargetElement("UNKOWN");
        ScattringTarget.SetTargetElementPDG(0);
    }

    string Target = ScattringTarget.GetTargetElement();
    int TargetPDG = ScattringTarget.GetTargetElementPDG();
    //</editor-fold>

    //<editor-fold desc="Execution variables">
    cout << "-- Execution variables ----------------------------------------------------\n";
    cout << "WorkingDirectory:\t" << WorkingDirectory << "\n";
    cout << "plots_path:\t\t" << plots_path << "\n";
    cout << "AnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n";
    cout << "AnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n";
    cout << "AnalyseFile:\t\t" << AnalyseFile << "\n";
    cout << "Settings mode:\t\t'" << file_name << "'\n\n";
    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
    cout << "Beam Energy:\t\t" << beamE << "\n\n\n\n";
    //</editor-fold>

    //</editor-fold>

// TList definition -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="TList definition">
    TList *plots = new TList();

    string listName = plots_path + AnalyseFileSample + plots_file_type;
    const char *TListName = listName.c_str();

    //    TFolder *folder_test = new TFolder("folder_test_name1/folder_test_name2","folder_test_title");
    //    plots->Add(folder_test);
    //</editor-fold>

//  Checking directories ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Creating directories">
    cout << "Creating plot directories...\n\n";

    string Plots_Folder = plots_path; // Plots_Folder = Parent_Folder
    system(("mkdir -p " + Plots_Folder).c_str()); // clear old stuff in Parent_Folder
    system(("rm -r " + Plots_Folder + "*").c_str()); // clear old stuff in Parent_Folder

    //<editor-fold desc="Beta vs. p plots directories">
    bool create_Beta_vs_P_Dir = true;
    string Beta_VS_P_Parent_Directory = "Beta_VS_P_plots";
    TFolder *Beta_vs_P_Folder = new TFolder(Beta_VS_P_Parent_Directory.c_str(), Beta_VS_P_Parent_Directory.c_str());
    string Beta_VS_P_Daughter_Folders[] = {"", "01_All_e", "01_All_e/By_charge", "02_1e_cut", "02_1e_cut/By_charge", "03_MicroBooNE",
                                           "03_MicroBooNE/Before_cuts", "03_MicroBooNE/Before_cuts/By_charge", "03_MicroBooNE/After_cuts",
                                           "03_MicroBooNE/After_cuts/By_charge", "04_2p", "04_2p/By_charge"};

    for (string folders_name: Beta_VS_P_Daughter_Folders) {
        MakeDirectory(create_Beta_vs_P_Dir, Beta_VS_P_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Beta_vs_P_Folder, Beta_VS_P_Parent_Directory, folders_name);
    }

    plots->Add(Beta_vs_P_Folder);

    string Beta_VS_P_All_e_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[1] + "/";
    string Beta_VS_P_by_charge_All_e_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[2] + "/";

    string Beta_VS_P_Only_1e_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[3] + "/";
    string Beta_VS_P_by_charge_Only_1e_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[4] + "/";

    string Beta_VS_P_MicroBooNE_BC_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[6] + "/";
    string Beta_VS_P_by_charge_MicroBooNE_BC_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[7] + "/";
    string Beta_VS_P_MicroBooNE_AC_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[8] + "/";
    string Beta_VS_P_by_charge_MicroBooNE_AC_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[9] + "/";

    string Beta_VS_P_2p_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[10] + "/";
    string Beta_VS_P_by_charge_2p_Directory = Plots_Folder + "/" + Beta_VS_P_Parent_Directory + "/" + Beta_VS_P_Daughter_Folders[11] + "/";
    //</editor-fold>

    //<editor-fold desc="Chi2 plots directories">
    bool create_chi2_Dir = true;
    string Chi2_Parent_Directory = "Chi2_plots";
    TFolder *Chi2_Folder = new TFolder(Chi2_Parent_Directory.c_str(), Chi2_Parent_Directory.c_str());
    string Chi2_Daughter_Folders[] = {"", "01_All_e", "02_1e_cut", "03_MicroBooNE", "03_MicroBooNE/BC-AC_cut_tests", "04_1e2p_BC-AC_cut_tests",
                                      "05_2p"};

    for (string folders_name: Chi2_Daughter_Folders) {
        MakeDirectory(create_chi2_Dir, Chi2_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Chi2_Folder, Chi2_Parent_Directory, folders_name);
    }

    plots->Add(Chi2_Folder);

    string Chi2_All_e_Directory = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Chi2_Daughter_Folders[1] + "/"; // 01_All_e

    string Chi2_1e_Directory = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Chi2_Daughter_Folders[2] + "/"; // 02_1e_cut

    string Chi2_MicroBooNE_Directory = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Chi2_Daughter_Folders[3] + "/"; // 03_MicroBooNE
    string Chi2_MicroBooNE_cut_tests_Directory = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Chi2_Daughter_Folders[4] + "/"; // 03_MicroBooNE/BC-AC_cut_tests

    string Chi2_1e2p_cut_tests_Directory = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Chi2_Daughter_Folders[5] + "/"; // 04_1e2p_BC-AC_cut_tests

    string Chi2_2p_Directory = Plots_Folder + "/" + Chi2_Parent_Directory + "/" + Chi2_Daughter_Folders[6] + "/"; // 05_2p
    //</editor-fold>

    //<editor-fold desc="Vertex plots directories">
    bool create_Vertex_Dir = true;
    string Vertex_Parent_Directory = "Vertex_plots";
    TFolder *Vertex_Folder = new TFolder(Vertex_Parent_Directory.c_str(), Vertex_Parent_Directory.c_str());
    string Vertex_Daughter_Folders[] = {"", "1e2p", "2p", "MicroBooNE", "MicroBooNE/BC-AC_cut_tests"};

    for (string folders_name: Vertex_Daughter_Folders) {
        MakeDirectory(create_Vertex_Dir, Vertex_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Vertex_Folder, Vertex_Parent_Directory, folders_name);
    }

    plots->Add(Vertex_Folder);

    string Vertex_1e2p_dV_BC_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[1] + "/";
    string Vertex_1e2p_dV_AC_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[1] + "/";

    string Vertex_dV_2p_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[2] + "/";

    string Vertex_dV_MicroBooNE_cut_tests_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[4] + "/";
    string Vertex_dV_MicroBooNE_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[3] + "/";
    //</editor-fold>

    //<editor-fold desc="Theta_e plots directories">
    bool create_Theta_e_Dir = true;
    string Theta_e_Parent_Directory = "Ang_plots";
    TFolder *Theta_e_Folder = new TFolder(Theta_e_Parent_Directory.c_str(), Theta_e_Parent_Directory.c_str());
    string Theta_e_Daughter_Folders[] = {"", "Theta_e_plots", "Theta_e_plots/All_e", "Theta_e_plots/Only_1e_cut", "Theta_e_plots/Only_1e_cut/1e2X",
                                         "Theta_e_plots/Only_1e_cut/1e2p", "2p", "2p/Theta_e_plots"};

    for (string folders_name: Theta_e_Daughter_Folders) {
        MakeDirectory(create_Theta_e_Dir, Theta_e_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Theta_e_Folder, Theta_e_Parent_Directory, folders_name);
    }

    plots->Add(Theta_e_Folder);

    string Theta_e_All_e_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[2] + "/";

    string Theta_e_Only_1e_cut_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[3] + "/";

    string Theta_e_Only_1e2X_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[4] + "/";
    string Theta_e_Only_1e2X_QEL_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[4] + "/";
    string Theta_e_Only_1e2X_MEC_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[4] + "/";
    string Theta_e_Only_1e2X_RES_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[4] + "/";
    string Theta_e_Only_1e2X_DIS_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[4] + "/";

    string Theta_e_Only_1e2p_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[5] + "/";
    string Theta_e_Only_1e2p_QEL_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[5] + "/";
    string Theta_e_Only_1e2p_MEC_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[5] + "/";
    string Theta_e_Only_1e2p_RES_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[5] + "/";
    string Theta_e_Only_1e2p_DIS_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[5] + "/";

    string Theta_e_VS_Phi_e_2p_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[6] + "/";

    string Theta_e_Only_2p_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[7] + "/";
    string Theta_e_Only_2p_QEL_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[7] + "/";
    string Theta_e_Only_2p_MEC_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[7] + "/";
    string Theta_e_Only_2p_RES_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[7] + "/";
    string Theta_e_Only_2p_DIS_Directory = Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + Theta_e_Daughter_Folders[7] + "/";
    //</editor-fold>

    //<editor-fold desc="Phi_e plots directories">
    bool create_Phi_e_Dir = true;
    string Phi_e_Parent_Directory = "Ang_plots";
    TFolder *Phi_e_Folder = new TFolder(Phi_e_Parent_Directory.c_str(), Phi_e_Parent_Directory.c_str());
    string Phi_e_Daughter_Folders[] = {"", "Phi_e_plots", "Phi_e_plots/All_e", "Phi_e_plots/Only_1e_cut", "Phi_e_plots/Only_1e_cut/1e2X",
                                       "Phi_e_plots/Only_1e_cut/1e2p", "2p/Phi_e_plots"};

    for (string folders_name: Phi_e_Daughter_Folders) {
        MakeDirectory(create_Phi_e_Dir, Phi_e_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Phi_e_Folder, Phi_e_Parent_Directory, folders_name);
    }

    plots->Add(Phi_e_Folder);

    string Phi_e_All_e_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[2] + "/";

    string Phi_e_Only_1e_cut_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[3] + "/";

    string Phi_e_Only_1e2X_cut_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[4] + "/";
    string Phi_e_Only_1e2X_QEL_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[4] + "/";
    string Phi_e_Only_1e2X_MEC_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[4] + "/";
    string Phi_e_Only_1e2X_RES_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[4] + "/";
    string Phi_e_Only_1e2X_DIS_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[4] + "/";

    string Phi_e_Only_1e2p_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[5] + "/";
    string Phi_e_Only_1e2p_QEL_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[5] + "/";
    string Phi_e_Only_1e2p_MEC_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[5] + "/";
    string Phi_e_Only_1e2p_RES_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[5] + "/";
    string Phi_e_Only_1e2p_DIS_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[5] + "/";

    string Phi_e_Only_2p_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[6] + "/";
    string Phi_e_Only_2p_QEL_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[6] + "/";
    string Phi_e_Only_2p_MEC_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[6] + "/";
    string Phi_e_Only_2p_RES_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[6] + "/";
    string Phi_e_Only_2p_DIS_Directory = Plots_Folder + "/" + Phi_e_Parent_Directory + "/" + Phi_e_Daughter_Folders[6] + "/";
    //</editor-fold>

    //<editor-fold desc="Q2 plots directories">
    bool create_Q2_Dir = true;
    string Q2_Parent_Directory = "Q2_plots";
    TFolder *Q2_Folder = new TFolder(Q2_Parent_Directory.c_str(), Q2_Parent_Directory.c_str());
    string Q2_Daughter_Folders[] = {"", "All_e", "Only_1e_cut", "1e2p_weChi2_cut"};

    for (string folders_name: Q2_Daughter_Folders) {
        MakeDirectory(create_Q2_Dir, Q2_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Q2_Folder, Q2_Parent_Directory, folders_name);
    }

    plots->Add(Q2_Folder);

    string Q2_All_e_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[1] + "/";
    string Q2_Only_1e_cut_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[2] + "/";
    //</editor-fold>

    //<editor-fold desc="Momentum plots directories">
    bool create_Momentum_Dir = true;
    string Momentum_Parent_Directory = "Momentum_plots";
    TFolder *Momentum_Folder = new TFolder(Momentum_Parent_Directory.c_str(), Momentum_Parent_Directory.c_str());
    string Momentum_Daughter_Folders[] = {"", "1e2p", "2p", "MicroBooNE", "MicroBooNE/BC-AC_cut_tests"};

    for (string folders_name: Momentum_Daughter_Folders) {
        MakeDirectory(create_Momentum_Dir, Momentum_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Momentum_Folder, Momentum_Parent_Directory, folders_name);
    }

    plots->Add(Momentum_Folder);

    string Momentum_1e2p_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[1] + "/";
    string Momentum_2p_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[2] + "/";
    string Momentum_MicroBooNE_cut_tests_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[4] + "/";
    string Momentum_MicroBooNE_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[3] + "/";
    //</editor-fold>

    //<editor-fold desc="E_e plots directories">
    bool create_E_e_Dir = true;
    string E_e_Parent_Directory = "Energy_plots";
    TFolder *E_e_Folder = new TFolder(E_e_Parent_Directory.c_str(), E_e_Parent_Directory.c_str());
    string E_e_Daughter_Folders[] = {"", "E_e_plots", "E_e_VS_theta_e", "E_e_plots/2p", "E_e_VS_theta_e/2p"};

    for (string folders_name: E_e_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, E_e_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(E_e_Folder, E_e_Parent_Directory, folders_name);
    }

    plots->Add(E_e_Folder);

    string E_e_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[1] + "/";
    string E_e_QEL_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[1] + "/";
    string E_e_MEC_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[1] + "/";
    string E_e_RES_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[1] + "/";
    string E_e_DIS_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[1] + "/";

    string E_e_VS_Theta_e_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
    string E_e_VS_Theta_e_QEL_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
    string E_e_VS_Theta_e_MEC_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
    string E_e_VS_Theta_e_RES_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
    string E_e_VS_Theta_e_DIS_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";

    string E_e_2p_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
    string E_e_2p_QEL_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
    string E_e_2p_MEC_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
    string E_e_2p_RES_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
    string E_e_2p_DIS_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";

    string E_e_VS_Theta_e_2p_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[4] + "/";
    string E_e_VS_Theta_e_2p_QEL_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[4] + "/";
    string E_e_VS_Theta_e_2p_MEC_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[4] + "/";
    string E_e_VS_Theta_e_2p_RES_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[4] + "/";
    string E_e_VS_Theta_e_2p_DIS_histograms_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[4] + "/";
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) plots directories">
    bool create_SF_Dir = true;
    string SF_Parent_Directory = "SF_plots";
    TFolder *SF_Folder = new TFolder(SF_Parent_Directory.c_str(), SF_Parent_Directory.c_str());
    string SF_Daughter_Folders[] = {"", "1e2p", "1e2p/SF_plots", "1e2p/SF_VS_P_e_plots", "2p", "2p/SF_plots", "2p/SF_VS_P_e_plots"};

    for (string folders_name: SF_Daughter_Folders) {
        MakeDirectory(create_SF_Dir, SF_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(SF_Folder, SF_Parent_Directory, folders_name);
    }

    plots->Add(SF_Folder);

    string SF_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/";

    // 1e2p - i.e. plots before all other cuts
    // before cuts - i.e. plots before SF cuts
    string SF_plots_1e2p_BC_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[2] + "/";
    string SF_VS_P_e_plots_1e2p_BC_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[3] + "/";

    // after cuts - i.e. plots after SF cuts
    string SF_plots_1e2p_AC_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[2] + "/";
    string SF_VS_P_e_plots_1e2p_AC_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[3] + "/";

    // 2p - i.e. plots after SF and all other cuts
    string SF_plots_2p_cuts_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[5] + "/";
    string SF_VS_P_e_plots_2p_cuts_histograms_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[6] + "/";
    //</editor-fold>

    //<editor-fold desc="Number of Photo-electrons (nphe) plots directories">
    bool create_nphe_Dir = true;
    string nphe_Parent_Directory = "nphe_plots";
    TFolder *nphe_Folder = new TFolder(nphe_Parent_Directory.c_str(), nphe_Parent_Directory.c_str());
    string nphe_Daughter_Folders[] = {"", "1e2p", "2p",};

    for (string folders_name: nphe_Daughter_Folders) {
        MakeDirectory(create_nphe_Dir, nphe_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(nphe_Folder, nphe_Parent_Directory, folders_name);
    }

    plots->Add(nphe_Folder);

    //TODO: add nphe plots by reaction
    string nphe_histograms_Directory = Plots_Folder + "/" + nphe_Parent_Directory + "/";

    // 1e2p - i.e. plots before all other cuts
    // before cuts - i.e. plots before nphe cuts
    string nphe_plots_1e2p_BC_histograms_Directory = Plots_Folder + "/" + nphe_Parent_Directory + "/" + nphe_Daughter_Folders[1] + "/";

    // after cuts - i.e. plots after nphe cuts
    string nphe_plots_1e2p_AC_histograms_Directory = Plots_Folder + "/" + nphe_Parent_Directory + "/" + nphe_Daughter_Folders[1] + "/";

    // 2p - i.e. plots after nphe and all other cuts
    string nphe_plots_2p_cuts_histograms_Directory = Plots_Folder + "/" + nphe_Parent_Directory + "/" + nphe_Daughter_Folders[2] + "/";
    //</editor-fold>

    //<editor-fold desc="Fiducial histograms plots directories">
    bool create_fiducial_Dir = true;
    string fiducial_Parent_Directory = "Fiducial_plots";
    TFolder *fiducial_Folder = new TFolder(fiducial_Parent_Directory.c_str(), fiducial_Parent_Directory.c_str());
    string fiducial_Daughter_Folders[] = {"", "1e_cut", "1e_cut/ECIN", "1e_cut/PCAL", "2p", "2p/ECIN", "2p/PCAL"};

    for (string folders_name: fiducial_Daughter_Folders) {
        MakeDirectory(create_fiducial_Dir, fiducial_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(fiducial_Folder, fiducial_Parent_Directory, folders_name);
    }

    plots->Add(fiducial_Folder);

    //TODO: add fiducial plots by reaction?
    string fiducial_histograms_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/";

    // 1e2p - i.e. plots before all other cuts
    // before cuts - i.e. plots before fiducial cuts
    string fiducial_plots_1e_BC_ECIN_histograms_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[2] + "/";
    string fiducial_plots_1e_BC_PCAL_histograms_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[3] + "/";

    // after cuts - i.e. plots after fiducial cuts
    string fiducial_plots_1e_AC_ECIN_histograms_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[2] + "/";
    string fiducial_plots_1e_AC_PCAL_histograms_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[3] + "/";

    // 2p - i.e. plots after fiducial and all other cuts
    string fiducial_plots_2p_cuts_histograms_ECIN_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[5] + "/";
    string fiducial_plots_2p_cuts_histograms_PCAL_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[6] + "/";
    //</editor-fold>

    //<editor-fold desc="ETrans plots directories">
    bool create_ETrans_Dir = true;
    string ETrans_Parent_Directory = "ETrans_plots";
    TFolder *ETrans_Folder = new TFolder(ETrans_Parent_Directory.c_str(), ETrans_Parent_Directory.c_str());
    string ETrans_Daughter_Folders[] = {"", "2p", "2p/Around_15_Deg", "2p/Around_15_Deg/All_interactions", "2p/Around_15_Deg/QEL_only",
                                        "2p/Around_15_Deg/MEC_only", "2p/Around_15_Deg/RES_only", "2p/Around_15_Deg/DIS_only"};

    for (string folders_name: ETrans_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, ETrans_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(ETrans_Folder, ETrans_Parent_Directory, folders_name);
    }

    plots->Add(ETrans_Folder);

    string ETrans_All_Int_15_histograms_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[3] + "/";
    string ETrans_QEL_15_histograms_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[4] + "/";
    string ETrans_MEC_15_histograms_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[5] + "/";
    string ETrans_RES_15_histograms_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[6] + "/";
    string ETrans_DIS_15_histograms_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[7] + "/";

    string ETrans_15_stack_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[2] + "/";
    //</editor-fold>

    //<editor-fold desc="Ecal plots directories">
    bool create_Ecal_Dir = true;
    string Ecal_Parent_Directory = "Ecal_plots";
    TFolder *Ecal_Folder = new TFolder(Ecal_Parent_Directory.c_str(), Ecal_Parent_Directory.c_str());
    string Ecal_Daughter_Folders[] = {"", "2p", "2p/All_interactions", "2p/QEL_only", "2p/MEC_only", "2p/RES_only", "2p/DIS_only"};

    for (string folders_name: Ecal_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, Ecal_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Ecal_Folder, Ecal_Parent_Directory, folders_name);
    }

    plots->Add(Ecal_Folder);

    string Ecal_All_Int_histograms_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[2] + "/";
    string Ecal_QEL_histograms_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[3] + "/";
    string Ecal_MEC_histograms_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[4] + "/";
    string Ecal_RES_histograms_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[5] + "/";
    string Ecal_DIS_histograms_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[6] + "/";

    string Ecal_stack_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[1] + "/";
    //</editor-fold>

    //</editor-fold>

// Calculation settings -------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Calculation settings">
    bool calculate_inclusive = false, calculate_2p = true, calculate_1n1p = false, calculate_MicroBooNE = true;
    //</editor-fold>

// Plot settings --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Plot selector">
    bool wider_margin = true;

    bool Beta_vs_P_plots = true;

    bool Chi2_plots = true;

    bool Vertex_plots = true;

    bool Angle_plots = false, Theta_e_plots = true, Phi_e_plots = true;
    if (Angle_plots == false) { Theta_e_plots = Phi_e_plots = false; }

    bool Q2_plots = true;

    bool Momentum_plots = true;

    bool E_e_plots = false;

    bool SF_plots = true;

    bool nphe_plots = true;

    bool fiducial_plots = true;

    bool ETrans_plots = true, ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;
    if (ETrans_plots == false) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }

    bool Ecal_plots = false, other_E_cal_plots = false;

    //<editor-fold desc="other plots">
    bool Theta_plots = false, Phi_plots = false;

    bool Energy_histogram_plots = false;

    bool inclusive_plots = false;

    bool momentum_plots = false;

    bool MicroBooNE_plots = false;
    //</editor-fold>

    //</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization settings">
    bool normalize_master = false;

    bool normalized_chi2_plots = false;

    bool normalized_vertex_plots = false;

    bool normalized_Angle_plots = false;

    bool normalized_Q2_plots = false;

    bool normalized_E_e_plots = false;

    bool normalized_SF_plots = false;

    bool normalized_nphe_plots = false;

    bool normalized_fiducial_plots = false;

    bool normalized_theta_lp_plots = true;
    bool normalized_theta_p1_plots = false, normalized_theta_p2_plots = false, normalized_dtheta_2p_plots = false; // 2p
    bool normalized_theta_p_plots = false, normalized_theta_n_plots = false, normalized_dtheta_1n1p_plots = false; // 1n1p

    bool normalized_phi_lp_plots = false;
    bool normalized_phi_p1_plots = false, normalized_phi_p2_plots = false, normalized_dphi_2p_plots = false; // 2p
    bool normalized_phi_p_plots = false, normalized_phi_n_plots = false, normalized_dphi_1n1p_plots = false; // 1n1p

    bool normalized_E_lp_plots = true;
    bool normalized_E_lp_all_int_plots = true, normalized_E_lp_QEL_plots = true, normalized_E_lp_MEC_plots = true, normalized_E_lp_RES_plots = true, normalized_E_lp_DIS_plots = true;

    bool normalized_E_Trans_all_ang_all_int_plots = true, normalized_E_Trans15_plots = true, normalized_inclusive_plots = true;

    bool normalized_E_cal_plots = true;

    bool normalized_P_lp_plots = false, normalized_P_L_plots = false, normalized_P_R_plots = false; // 2p & 1n1p

    if (normalize_master == false) {
        normalized_chi2_plots = normalized_vertex_plots = normalized_Angle_plots = normalized_Q2_plots = normalized_E_e_plots = normalized_SF_plots = normalized_nphe_plots = normalized_theta_lp_plots = normalized_fiducial_plots = normalized_theta_lp_plots = false;

        normalized_theta_p1_plots = normalized_theta_p2_plots = normalized_dtheta_2p_plots = false; // 2p

        normalized_theta_p_plots = normalized_theta_n_plots = normalized_dtheta_1n1p_plots = false; // 1n1p

        normalized_phi_lp_plots = false;

        normalized_phi_p1_plots = normalized_phi_p2_plots = normalized_dphi_2p_plots = false; // 2p

        normalized_phi_p_plots = normalized_phi_n_plots = normalized_dphi_1n1p_plots = false; // 1n1p

        normalized_E_lp_plots = normalized_E_lp_all_int_plots = normalized_E_lp_QEL_plots = normalized_E_lp_MEC_plots = normalized_E_lp_RES_plots = normalized_E_lp_DIS_plots = false;

        normalized_E_Trans_all_ang_all_int_plots = normalized_E_Trans15_plots = normalized_E_cal_plots = normalized_inclusive_plots = false;

        normalized_P_lp_plots = normalized_P_L_plots = normalized_P_R_plots = false; // 2p & 1n1p

        cout << "\nAll normalizations are disabled.\n\n";  // and no change to custom_FSI_status
    }

    if (normalized_E_lp_plots == false) {
        normalized_E_lp_all_int_plots = normalized_E_lp_QEL_plots = normalized_E_lp_MEC_plots = normalized_E_lp_RES_plots = normalized_E_lp_DIS_plots = false;
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

// Momentum thresholds --------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum thresholds">

    //<editor-fold desc="Momentum thresholds (2p)">
    double P_lp_upper_lim_2p = -1, P_lp_lower_lim_2p = -1, P_p1_upper_lim_2p = -1, P_p1_lower_lim_2p = 0.3, P_p2_upper_lim_2p = -1, P_p2_lower_lim_2p = 0.3;
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds (1n1p)">
    double P_lp_upper_lim_1n1p = -1, P_lp_lower_lim_1n1p = -1, P_p_upper_lim_1n1p = -1, P_p_lower_lim_1n1p = 0.3, P_n_upper_lim_1n1p = -1, P_n_lower_lim_1n1p = 0.3;
    //</editor-fold>

    //<editor-fold desc="Momentum thresholds (2p, MicroBooNE)">
    double P_lp_upper_lim_MicroBooNE = 1.2, P_lp_lower_lim_MicroBooNE = 0.1, P_L_upper_lim_MicroBooNE = 1.0, P_L_lower_lim_MicroBooNE = 0.3, P_R_upper_lim_MicroBooNE = 1.0, P_R_lower_lim_MicroBooNE = 0.3;
    double P_pion_upper_lim_MicroBooNE = 0.065;
    //</editor-fold>

    //</editor-fold>

// Histogram limits -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Histogram limits">

    //<editor-fold desc="Histogram limits for every case">

    //<editor-fold desc="Chi2 plots">
    Chi2_upper_lim = 25;
    Chi2_lower_lim = -Chi2_upper_lim;
    //</editor-fold>

    //<editor-fold desc="Vertex plots">
    Vertex_upper_lim = 50;
    Vertex_lower_lim = -Vertex_upper_lim;

    dV_upper_lim = 10;
    dV_lower_lim = -dV_upper_lim;
    //</editor-fold>

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
        E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
        E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
        E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
        E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
        E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
        E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
        E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
        E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
        //</editor-fold>

        //<editor-fold desc="E_cal restoration histograms (1n1p)">
        E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
        E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
        E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
        E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
        E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
        E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
        E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
        E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
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

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                      Histogram definitions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Histogram definitions">

    cout << "\nDefining histograms...";

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Beta vs. P histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Beta vs. P histograms">

    //<editor-fold desc="Beta vs. P (all particles)">

    //<editor-fold desc="Beta vs. P (no cuts, CD & FD)">
    TH2D *Beta_vs_P_CD = new TH2D("#beta vs. P (All Particles, CD)", "#beta vs. P (All Particles, CD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_FD = new TH2D("#beta vs. P (All Particles, FD)", "#beta vs. P (All Particles, FD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_CD_Dir = Beta_VS_P_All_e_Directory, Beta_vs_P_FD_Dir = Beta_VS_P_All_e_Directory;

    TH2D *Beta_vs_P_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, CD)", "#beta vs. P (Electrons Only, CD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0.5, 3);
    TH2D *Beta_vs_P_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, FD)", "#beta vs. P (Electrons Only, FD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0.5, 3);
    string Beta_vs_P_Electrons_Only_CD_Dir = Beta_VS_P_All_e_Directory, Beta_vs_P_Electrons_Only_FD_Dir = Beta_VS_P_All_e_Directory;

    TH2D *Beta_vs_P_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, CD)", "#beta vs. P (Protons Only, CD);P [GeV];#beta",
                                               250, 0, beamE * 1.1, 250, 0, 1.1);
    TH2D *Beta_vs_P_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, FD)", "#beta vs. P (Protons Only, FD);P [GeV];#beta",
                                               250, 0, beamE * 1.1, 250, 0, 1.1);
    string Beta_vs_P_Protons_Only_CD_Dir = Beta_VS_P_All_e_Directory, Beta_vs_P_Protons_Only_FD_Dir = Beta_VS_P_All_e_Directory;

    TH2D *Beta_vs_P_Neutrons_Only_CD = new TH2D("#beta vs. P (Neutrons Only, CD)", "#beta vs. P (Neutrons Only, CD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_Neutrons_Only_FD = new TH2D("#beta vs. P (Neutrons Only, FD)", "#beta vs. P (Neutrons Only, FD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_Neutrons_Only_CD_Dir = Beta_VS_P_All_e_Directory, Beta_vs_P_Neutrons_Only_FD_Dir = Beta_VS_P_All_e_Directory;
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
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1e cut)">

    //<editor-fold desc="Beta vs. P for all particles (1e cut)">
    TH2D *Beta_vs_P_1e_CD = new TH2D("#beta vs. P (All Particles, 1e only, CD)", "#beta vs. P (All Particles, 1e Cut, CD);P [GeV];#beta",
                                     250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_FD = new TH2D("#beta vs. P (All Particles, 1e only, FD)", "#beta vs. P (All Particles, 1e Cut, FD);P [GeV];#beta",
                                     250, 0, beamE * 1.425, 250, 0, 3);

    TH2D *Beta_vs_P_1e_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, 1e only, CD)", "#beta vs. P (Electrons Only, 1e Cut, CD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);
    TH2D *Beta_vs_P_1e_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, 1e only, FD)", "#beta vs. P (Electrons Only, 1e Cut, FD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);

    TH2D *Beta_vs_P_1e_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, 1e only, CD)", "#beta vs. P (Protons Only, 1e Cut, CD);P [GeV];#beta",
                                                  250, 0, beamE * 1.1, 250, 0, 1.1);
    TH2D *Beta_vs_P_1e_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, 1e only, FD)", "#beta vs. P (Protons Only, 1e Cut, FD);P [GeV];#beta",
                                                  250, 0, beamE * 1.1, 250, 0, 1.1);

    TH2D *Beta_vs_P_1e_Neutrons_Only_CD = new TH2D("#beta vs. P (Neutrons Only, 1e only, CD)", "#beta vs. P (Neutrons Only, 1e Cut, CD);P [GeV];#beta",
                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_Neutrons_Only_FD = new TH2D("#beta vs. P (Neutrons Only, 1e only, FD)", "#beta vs. P (Neutrons Only, 1e Cut, FD);P [GeV];#beta",
                                                   250, 0, beamE * 1.425, 250, 0, 3);

    TH2D *Beta_vs_P_1e_piplus_Only_CD = new TH2D("#beta vs. P (#pi^{+} Only, 1e only, CD)", "#beta vs. P (#pi^{+} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_piplus_Only_FD = new TH2D("#beta vs. P (#pi^{+} Only, 1e only, FD)", "#beta vs. P (#pi^{+} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0, 3);

    TH2D *Beta_vs_P_1e_pizero_Only_CD = new TH2D("#beta vs. P (#pi^{0} Only, 1e only, CD)", "#beta vs. P (#pi^{0} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_pizero_Only_FD = new TH2D("#beta vs. P (#pi^{0} Only, 1e only, FD)", "#beta vs. P (#pi^{0} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0, 3);

    TH2D *Beta_vs_P_1e_piminus_Only_CD = new TH2D("#beta vs. P (#pi^{-} Only, 1e only, CD)", "#beta vs. P (#pi^{-} Only, 1e Cut, CD);P [GeV];#beta",
                                                  250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_piminus_Only_FD = new TH2D("#beta vs. P (#pi^{-} Only, 1e only, FD)", "#beta vs. P (#pi^{-} Only, 1e Cut, FD);P [GeV];#beta",
                                                  250, 0, beamE * 1.425, 250, 0, 3);

    TH2D *Beta_vs_P_1e_Kplus_Only_CD = new TH2D("#beta vs. P (K^{+} Only, 1e only, CD)", "#beta vs. P (K^{+} Only, 1e Cut, CD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_Kplus_Only_FD = new TH2D("#beta vs. P (K^{+} Only, 1e only, FD)", "#beta vs. P (K^{+} Only, 1e Cut, FD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);

    TH2D *Beta_vs_P_1e_Kminus_Only_CD = new TH2D("#beta vs. P (K^{-} Only, 1e only, CD)", "#beta vs. P (K^{-} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_1e_Kminus_Only_FD = new TH2D("#beta vs. P (K^{-} Only, 1e only, FD)", "#beta vs. P (K^{-} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, beamE * 1.425, 250, 0, 3);
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1e cut)">
    TH2D *Beta_vs_P_positive_particles_1e_CD = new TH2D("#beta vs. P & q = +1 (1e cut, CD)", "#beta vs. P for all particles with q = +1 (1e cut, CD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_positive_particles_1e_FD = new TH2D("#beta vs. P & q = +1 (1e cut, FD)", "#beta vs. P for all particles with q = +1 (1e cut, FD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_1e_CD = new TH2D("#beta vs. P & q = 0 (1e cut, CD)", "#beta vs. P for all particles with q = 0 (1e cut, CD);P [GeV];#beta",
                                                       250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_1e_FD = new TH2D("#beta vs. P & q = 0 (1e cut, FD)", "#beta vs. P for all particles with q = 0 (1e cut, FD);P [GeV];#beta",
                                                       250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_1e_CD = new TH2D("#beta vs. P & q = -1 (1e cut, CD)", "#beta vs. P for all particles with q = -1 (1e cut, CD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_1e_FD = new TH2D("#beta vs. P & q = -1 (1e cut, FD)", "#beta vs. P for all particles with q = -1 (1e cut, FD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (2p)">

    //<editor-fold desc="Beta vs. P for all particles (2p)">
    TH2D *Beta_vs_P_2p_CD = new TH2D("#beta vs. P (All Particles, 2p, CD)", "#beta vs. P (All Particles, 2p, CD);P [GeV];#beta",
                                     250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_2p_FD = new TH2D("#beta vs. P (All Particles, 2p, FD)", "#beta vs. P (All Particles, 2p, FD);P [GeV];#beta",
                                     250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_2p_CD_Dir = Beta_VS_P_2p_Directory, Beta_vs_P_2p_FD_Dir = Beta_VS_P_2p_Directory;

    TH2D *Beta_vs_P_2p_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, 2p, CD)", "#beta vs. P (Electrons Only, 2p, CD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);
    TH2D *Beta_vs_P_2p_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, 2p, FD)", "#beta vs. P (Electrons Only, 2p, FD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);
    string Beta_vs_P_2p_Electrons_Only_CD_Dir = Beta_VS_P_2p_Directory, Beta_vs_P_2p_Electrons_Only_FD_Dir = Beta_VS_P_2p_Directory;

    TH2D *Beta_vs_P_2p_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, 2p, CD)", "#beta vs. P (Protons Only, 2p, CD);P [GeV];#beta",
                                                  250, 0, beamE * 1.1, 250, 0, 1.1);
    TH2D *Beta_vs_P_2p_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, 2p, FD)", "#beta vs. P (Protons Only, 2p, FD);P [GeV];#beta",
                                                  250, 0, beamE * 1.1, 250, 0, 1.1);
    string Beta_vs_P_2p_Protons_Only_CD_Dir = Beta_VS_P_2p_Directory, Beta_vs_P_2p_Protons_Only_FD_Dir = Beta_VS_P_2p_Directory;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (2p)">
    TH2D *Beta_vs_P_positive_particles_2p_CD = new TH2D("#beta vs. P & q = +1 (2p, CD)", "#beta vs. P for all particles with q = +1 (2p, CD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_positive_particles_2p_FD = new TH2D("#beta vs. P & q = +1 (2p, FD)", "#beta vs. P for all particles with q = +1 (2p, FD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_2p_CD = new TH2D("#beta vs. P & q = 0 (2p, CD)", "#beta vs. P for all particles with q = 0 (2p, CD);P [GeV];#beta",
                                                       250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_2p_FD = new TH2D("#beta vs. P & q = 0 (2p, FD)", "#beta vs. P for all particles with q = 0 (2p, FD);P [GeV];#beta",
                                                       250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_2p_CD = new TH2D("#beta vs. P & q = -1 (2p, CD)", "#beta vs. P for all particles with q = -1 (2p, CD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_2p_FD = new TH2D("#beta vs. P & q = -1 (2p, FD)", "#beta vs. P for all particles with q = -1 (2p, FD);P [GeV];#beta",
                                                        250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_positive_particles_2p_CD_Dir = Beta_VS_P_by_charge_2p_Directory, Beta_vs_P_positive_particles_2p_FD_Dir = Beta_VS_P_by_charge_2p_Directory;
    string Beta_vs_P_neutral_particles_2p_CD_Dir = Beta_VS_P_by_charge_2p_Directory, Beta_vs_P_neutral_particles_2p_FD_Dir = Beta_VS_P_by_charge_2p_Directory;
    string Beta_vs_P_negative_particles_2p_CD_Dir = Beta_VS_P_by_charge_2p_Directory, Beta_vs_P_negative_particles_2p_FD_Dir = Beta_VS_P_by_charge_2p_Directory;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (MicroBooNE-BC)">

    //<editor-fold desc="Beta vs. P for all particles (MicroBooNE-BC)">

    // Note: here "MicroBooNE-BC" means before ALL MicroBooNE cuts!
    TH2D *Beta_vs_P_MicroBooNE_BC_CD = new TH2D("#beta vs. P (All Particles, MicroBooNE-BC, CD)", "#beta vs. P (All Particles, MicroBooNE-BC, CD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_MicroBooNE_BC_FD = new TH2D("#beta vs. P (All Particles, MicroBooNE-BC, FD)", "#beta vs. P (All Particles, MicroBooNE-BC, FD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_MicroBooNE_BC_CD_Dir = Beta_VS_P_MicroBooNE_BC_Directory, Beta_vs_P_MicroBooNE_BC_FD_Dir = Beta_VS_P_MicroBooNE_BC_Directory;

//    TH2D *Beta_vs_P_MicroBooNE_BC_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, MicroBooNE-BC, CD)",
//                                                               "#beta vs. P (Electrons Only, MicroBooNE-BC, CD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0.5, 3);
//    TH2D *Beta_vs_P_MicroBooNE_BC_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, MicroBooNE-BC, FD)",
//                                                               "#beta vs. P (Electrons Only, MicroBooNE-BC, FD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0.5, 3);
//    string Beta_vs_P_MicroBooNE_BC_Electrons_Only_CD_Dir = Beta_VS_P_MicroBooNE_BC_Directory, Beta_vs_P_MicroBooNE_BC_Electrons_Only_FD_Dir = Beta_VS_P_MicroBooNE_BC_Directory;
//
//    TH2D *Beta_vs_P_MicroBooNE_BC_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, MicroBooNE-BC, CD)",
//                                                             "#beta vs. P (Protons Only, MicroBooNE-BC, CD);P [GeV];#beta", 250, 0, beamE * 1.1, 250, 0, 1.1);
//    TH2D *Beta_vs_P_MicroBooNE_BC_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, MicroBooNE-BC, FD)",
//                                                             "#beta vs. P (Protons Only, MicroBooNE-BC, FD);P [GeV];#beta", 250, 0, beamE * 1.1, 250, 0, 1.1);
//    string Beta_vs_P_MicroBooNE_BC_Protons_Only_CD_Dir = Beta_VS_P_MicroBooNE_BC_Directory, Beta_vs_P_MicroBooNE_BC_Protons_Only_FD_Dir = Beta_VS_P_MicroBooNE_BC_Directory;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (MicroBooNE-BC)">

    // Note: here "MicroBooNE-BC" means before ALL MicroBooNE cuts!
    TH2D *Beta_vs_P_positive_particles_MicroBooNE_BC_CD = new TH2D("#beta vs. P & q = +1 (MicroBooNE-BC, CD)",
                                                                   "#beta vs. P for all particles with q = +1 (MicroBooNE-BC, CD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_positive_particles_MicroBooNE_BC_FD = new TH2D("#beta vs. P & q = +1 (MicroBooNE-BC, FD)",
                                                                   "#beta vs. P for all particles with q = +1 (MicroBooNE-BC, FD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_MicroBooNE_BC_CD = new TH2D("#beta vs. P & q = 0 (MicroBooNE-BC, CD)",
                                                                  "#beta vs. P for all particles with q = 0 (MicroBooNE-BC, CD);P [GeV];#beta",
                                                                  250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_MicroBooNE_BC_FD = new TH2D("#beta vs. P & q = 0 (MicroBooNE-BC, FD)",
                                                                  "#beta vs. P for all particles with q = 0 (MicroBooNE-BC, FD);P [GeV];#beta",
                                                                  250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_MicroBooNE_BC_CD = new TH2D("#beta vs. P & q = -1 (MicroBooNE-BC, CD)",
                                                                   "#beta vs. P for all particles with q = -1 (MicroBooNE-BC, CD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_MicroBooNE_BC_FD = new TH2D("#beta vs. P & q = -1 (MicroBooNE-BC, FD)",
                                                                   "#beta vs. P for all particles with q = -1 (MicroBooNE-BC, FD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_positive_particles_MicroBooNE_BC_CD_Dir = Beta_VS_P_by_charge_MicroBooNE_BC_Directory, Beta_vs_P_positive_particles_MicroBooNE_BC_FD_Dir = Beta_VS_P_by_charge_MicroBooNE_BC_Directory;
    string Beta_vs_P_neutral_particles_MicroBooNE_BC_CD_Dir = Beta_VS_P_by_charge_MicroBooNE_BC_Directory, Beta_vs_P_neutral_particles_MicroBooNE_BC_FD_Dir = Beta_VS_P_by_charge_MicroBooNE_BC_Directory;
    string Beta_vs_P_negative_particles_MicroBooNE_BC_CD_Dir = Beta_VS_P_by_charge_MicroBooNE_BC_Directory, Beta_vs_P_negative_particles_MicroBooNE_BC_FD_Dir = Beta_VS_P_by_charge_MicroBooNE_BC_Directory;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (MicroBooNE-AC)">

    //<editor-fold desc="Beta vs. P for all particles (MicroBooNE-AC)">

    // Note: here "MicroBooNE-BC" means after ALL MicroBooNE cuts!
    TH2D *Beta_vs_P_MicroBooNE_AC_CD = new TH2D("#beta vs. P (All Particles, MicroBooNE-AC, CD)", "#beta vs. P (All Particles, MicroBooNE-AC, CD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_MicroBooNE_AC_FD = new TH2D("#beta vs. P (All Particles, MicroBooNE-AC, FD)", "#beta vs. P (All Particles, MicroBooNE-AC, FD);P [GeV];#beta",
                                                250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_MicroBooNE_AC_CD_Dir = Beta_VS_P_MicroBooNE_AC_Directory, Beta_vs_P_MicroBooNE_AC_FD_Dir = Beta_VS_P_MicroBooNE_AC_Directory;

//    TH2D *Beta_vs_P_MicroBooNE_AC_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, MicroBooNE-AC, CD)",
//                                                               "#beta vs. P (Electrons Only, MicroBooNE-AC, CD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0.5, 3);
//    TH2D *Beta_vs_P_MicroBooNE_AC_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, MicroBooNE-AC, FD)",
//                                                               "#beta vs. P (Electrons Only, MicroBooNE-AC, FD);P [GeV];#beta", 250, 0, beamE * 1.425, 250, 0.5, 3);
//    string Beta_vs_P_MicroBooNE_AC_Electrons_Only_CD_Dir = Beta_VS_P_MicroBooNE_AC_Directory, Beta_vs_P_MicroBooNE_AC_Electrons_Only_FD_Dir = Beta_VS_P_MicroBooNE_AC_Directory;
//
//    TH2D *Beta_vs_P_MicroBooNE_AC_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, MicroBooNE-AC, CD)",
//                                                             "#beta vs. P (Protons Only, MicroBooNE-AC, CD);P [GeV];#beta", 250, 0, beamE * 1.1, 250, 0, 1.1);
//    TH2D *Beta_vs_P_MicroBooNE_AC_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, MicroBooNE-AC, FD)",
//                                                             "#beta vs. P (Protons Only, MicroBooNE-AC, FD);P [GeV];#beta", 250, 0, beamE * 1.1, 250, 0, 1.1);
//    string Beta_vs_P_MicroBooNE_AC_Protons_Only_CD_Dir = Beta_VS_P_MicroBooNE_AC_Directory, Beta_vs_P_MicroBooNE_AC_Protons_Only_FD_Dir = Beta_VS_P_MicroBooNE_AC_Directory;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (MicroBooNE-AC)">

    // Note: here "MicroBooNE-BC" means after ALL MicroBooNE cuts!
    TH2D *Beta_vs_P_positive_particles_MicroBooNE_AC_CD = new TH2D("#beta vs. P & q = +1 (MicroBooNE-AC, CD)",
                                                                   "#beta vs. P for all particles with q = +1 (MicroBooNE-AC, CD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_positive_particles_MicroBooNE_AC_FD = new TH2D("#beta vs. P & q = +1 (MicroBooNE-AC, FD)",
                                                                   "#beta vs. P for all particles with q = +1 (MicroBooNE-AC, FD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_MicroBooNE_AC_CD = new TH2D("#beta vs. P & q = 0 (MicroBooNE-AC, CD)",
                                                                  "#beta vs. P for all particles with q = 0 (MicroBooNE-AC, CD);P [GeV];#beta",
                                                                  250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_neutral_particles_MicroBooNE_AC_FD = new TH2D("#beta vs. P & q = 0 (MicroBooNE-AC, FD)",
                                                                  "#beta vs. P for all particles with q = 0 (MicroBooNE-AC, FD);P [GeV];#beta",
                                                                  250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_MicroBooNE_AC_CD = new TH2D("#beta vs. P & q = -1 (MicroBooNE-AC, CD)",
                                                                   "#beta vs. P for all particles with q = -1 (MicroBooNE-AC, CD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    TH2D *Beta_vs_P_negative_particles_MicroBooNE_AC_FD = new TH2D("#beta vs. P & q = -1 (MicroBooNE-AC, FD)",
                                                                   "#beta vs. P for all particles with q = -1 (MicroBooNE-AC, FD);P [GeV];#beta",
                                                                   250, 0, beamE * 1.425, 250, 0, 3);
    string Beta_vs_P_positive_particles_MicroBooNE_AC_CD_Dir = Beta_VS_P_by_charge_MicroBooNE_AC_Directory, Beta_vs_P_positive_particles_MicroBooNE_AC_FD_Dir = Beta_VS_P_by_charge_MicroBooNE_AC_Directory;
    string Beta_vs_P_neutral_particles_MicroBooNE_AC_CD_Dir = Beta_VS_P_by_charge_MicroBooNE_AC_Directory, Beta_vs_P_neutral_particles_MicroBooNE_AC_FD_Dir = Beta_VS_P_by_charge_MicroBooNE_AC_Directory;
    string Beta_vs_P_negative_particles_MicroBooNE_AC_CD_Dir = Beta_VS_P_by_charge_MicroBooNE_AC_Directory, Beta_vs_P_negative_particles_MicroBooNE_AC_FD_Dir = Beta_VS_P_by_charge_MicroBooNE_AC_Directory;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">

    //<editor-fold desc="Chi2 plots (no #(e) cut)">
    /* Plots of chi2 with no cut on number of electrons - NOT used later */
    THStack *Chi2_Electron_Stack = new THStack("#chi^{2}_{e^{-}} (CD & FD)", "#chi^{2}_{e^{-}} Histogram (CD & FD);#chi^{2}_{e^{-}};");
    THStack *Chi2_Proton_Stack = new THStack("#chi^{2}_{p} (CD & FD)", "#chi^{2}_{p} Histogram (CD & FD);#chi^{2}_{p};");

    TH1D *Chi2_Electron_CD = new TH1D("#chi^{2}_{e^{-}} (no #(e) cut, CD) test", "#chi^{2}_{e^{-}} Histogram (no #(e) cut, CD);#chi^{2}_{e^{-}};",
                                      500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_FD = new TH1D("#chi^{2}_{e^{-}} (no #(e) cut, FD) test", "#chi^{2}_{e^{-}} Histogram (no #(e) cut, FD);#chi^{2}_{e^{-}};",
                                      500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_CD = new TH1D("#chi^{2}_{p} (no #(e) cut, CD) test", "#chi^{2}_{p} Histogram (no #(e) cut, CD);#chi^{2}_{p};",
                                    500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_FD = new TH1D("#chi^{2}_{p} (no #(e) cut, FD) test", "#chi^{2}_{p} Histogram (no #(e) cut, FD);#chi^{2}_{p};",
                                    500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string Chi2_Electron_CD_Dir = Chi2_All_e_Directory, Chi2_Electron_FD_Dir = Chi2_All_e_Directory;
    string Chi2_Proton_CD_Dir = Chi2_All_e_Directory, Chi2_Proton_FD_Dir = Chi2_All_e_Directory;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e cut)">
    /* Plots of chi2 with no cut on number of electrons - used later to apply cuts with my mechanisem (NOT Justin's ana) */
    THStack *Chi2_Electron_1e_Stack = new THStack("#chi^{2}_{e^{-}} (1e cut, CD & FD)", "#chi^{2}_{e^{-}} Histogram (1e cut, CD & FD);#chi^{2}_{e^{-}};");
    THStack *Chi2_Proton_1e_Stack = new THStack("#chi^{2}_{p} (1e cut, CD & FD)", "#chi^{2}_{p} Histogram (1e cut, CD & FD);#chi^{2}_{p};");

    TH1D *Chi2_Electron_1e_CD = new TH1D("#chi^{2}_{e^{-}} (1e^{-} cut, CD) test", "#chi^{2}_{e^{-}} Histogram (1e^{-} cut, CD);#chi^{2}_{e^{-}};",
                                         500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_1e_FD = new TH1D("#chi^{2}_{e^{-}} (1e^{-} cut, FD) test", "#chi^{2}_{e^{-}} Histogram (1e^{-} cut, FD);#chi^{2}_{e^{-}};",
                                         500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_1e_CD = new TH1D("#chi^{2}_{p} (1e^{-} cut, CD) test", "#chi^{2}_{p} Histogram (1e^{-} cut, CD);#chi^{2}_{p};",
                                       500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_1e_FD = new TH1D("#chi^{2}_{p} (1e^{-} cut, FD) test", "#chi^{2}_{p} Histogram (1e^{-} cut, FD);#chi^{2}_{p};",
                                       500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string Chi2_Electron_1e_CD_Dir = Chi2_1e_Directory, Chi2_Electron_1e_FD_Dir = Chi2_1e_Directory;
    string Chi2_Proton_1e_CD_Dir = Chi2_1e_Directory, Chi2_Proton_1e_FD_Dir = Chi2_1e_Directory;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (MicroBooNE)">
    /* Here we plot chi2 three times:
     * First - before applying individual chi2 cuts
     * Second - after applying individual chi2 cuts
     * Third - after applying ALL cuts, not just individual chi2 cuts (= c-cut) */
    THStack *Chi2_Electron_MicroBooNE_Stack = new THStack("#chi^{2}_{e^{-}} (MicroBooNE, CD & FD)", "#chi^{2}_{e^{-}} (MicroBooNE, CD & FD);#chi^{2}_{e^{-}};");
    THStack *Chi2_Proton_MicroBooNE_Stack = new THStack("#chi^{2}_{p} (MicroBooNE, CD & FD)", "#chi^{2}_{p} (MicroBooNE, CD & FD);#chi^{2}_{p};");
    THStack *Chi2_piplus_MicroBooNE_Stack = new THStack("#chi^{2}_{#pi^{+}} (MicroBooNE, CD & FD)", "#chi^{2}_{#pi^{+}} (MicroBooNE, CD & FD);#chi^{2}_{#pi^{+}};");
    THStack *Chi2_piminus_MicroBooNE_Stack = new THStack("#chi^{2}_{#pi^{-}} (MicroBooNE, CD & FD)", "#chi^{2}_{#pi^{-}} (MicroBooNE, CD & FD);#chi^{2}_{#pi^{-}};");

    //<editor-fold desc="Testing Chi2 before MicroBooNE cuts test">
    TH1D *Chi2_Electron_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{e^{-}} BC (MicroBooNE, CD)", "#chi^{2}_{e^{-}} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{e^{-}};",
                                                    500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{e^{-}} BC (MicroBooNE, FD)", "#chi^{2}_{e^{-}} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{e^{-}};",
                                                    500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{p} BC (MicroBooNE, CD)", "#chi^{2}_{p} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{p};",
                                                  500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{p} BC (MicroBooNE, FD)", "#chi^{2}_{p} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{p};",
                                                  500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    TH1D *Chi2_piplus_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{#pi^{+}} BC (MicroBooNE, CD)",
                                                  "#chi^{2}_{#pi^{+}} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{+}};",
                                                  500, -1.5 * Chi2_piplus_cut_MicroBooNE_CD, 1.5 * Chi2_piplus_cut_MicroBooNE_CD);
    TH1D *Chi2_piplus_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{#pi^{+}} BC (MicroBooNE, FD)",
                                                  "#chi^{2}_{#pi^{+}} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{+}};",
                                                  500, -1.5 * Chi2_piplus_cut_MicroBooNE_FD, 1.5 * Chi2_piplus_cut_MicroBooNE_FD);
    TH1D *Chi2_piminus_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{#pi^{-}} BC (MicroBooNE, CD)",
                                                   "#chi^{2}_{#pi^{-}} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{-}};",
                                                   500, -1.5 * Chi2_piminus_cut_MicroBooNE_CD, 1.5 * Chi2_piminus_cut_MicroBooNE_CD);
    TH1D *Chi2_piminus_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{#pi^{-}} BC (MicroBooNE, FD)",
                                                   "#chi^{2}_{#pi^{-}} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{-}};",
                                                   500, -1.5 * Chi2_piminus_cut_MicroBooNE_FD, 1.5 * Chi2_piminus_cut_MicroBooNE_FD);
    string Chi2_Electron_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_Electron_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string Chi2_Proton_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_Proton_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string Chi2_piplus_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_piplus_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string Chi2_piminus_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_piminus_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    //</editor-fold>

    //<editor-fold desc="Testing Chi2 after MicroBooNE cuts test">
    TH1D *Chi2_Electron_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{e^{-}} AC (MicroBooNE, CD)", "#chi^{2}_{e^{-}} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{e^{-}};",
                                                    500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{e^{-}} AC (MicroBooNE, FD)", "#chi^{2}_{e^{-}} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{e^{-}};",
                                                    500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{p} AC (MicroBooNE, CD)", "#chi^{2}_{p} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{p};",
                                                  500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{p} AC (MicroBooNE, FD)", "#chi^{2}_{p} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{p};",
                                                  500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    TH1D *Chi2_piplus_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{#pi^{+}} AC (MicroBooNE, CD)",
                                                  "#chi^{2}_{#pi^{+}} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{+}};",
                                                  500, -1.5 * Chi2_piplus_cut_MicroBooNE_CD, 1.5 * Chi2_piplus_cut_MicroBooNE_CD);
    TH1D *Chi2_piplus_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{#pi^{+}} AC (MicroBooNE, FD)",
                                                  "#chi^{2}_{#pi^{+}} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{+}};",
                                                  500, -1.5 * Chi2_piplus_cut_MicroBooNE_FD, 1.5 * Chi2_piplus_cut_MicroBooNE_FD);
    TH1D *Chi2_piminus_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{#pi^{-}} AC (MicroBooNE, CD)",
                                                   "#chi^{2}_{#pi^{-}} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{-}};",
                                                   500, -1.5 * Chi2_piminus_cut_MicroBooNE_CD, 1.5 * Chi2_piminus_cut_MicroBooNE_CD);
    TH1D *Chi2_piminus_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{#pi^{-}} AC (MicroBooNE, FD)",
                                                   "#chi^{2}_{#pi^{-}} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{-}};",
                                                   500, -1.5 * Chi2_piminus_cut_MicroBooNE_FD, 1.5 * Chi2_piminus_cut_MicroBooNE_FD);
    string Chi2_Electron_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_Electron_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string Chi2_Proton_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_Proton_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string Chi2_piplus_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_piplus_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string Chi2_piminus_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, Chi2_piminus_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots after MicroBooNE cuts">
    TH1D *Chi2_Electron_MicroBooNE_CD = new TH1D("#chi^{2}_{e^{-}} (MicroBooNE-CC, CD)", "#chi^{2}_{e^{-}} Histogram (MicroBooNE-CC, CD);#chi^{2}_{e^{-}};",
                                                 500, -1.5 * Chi2_Electron_cut_MicroBooNE_CD, 1.5 * Chi2_Electron_cut_MicroBooNE_CD);
    TH1D *Chi2_Electron_MicroBooNE_FD = new TH1D("#chi^{2}_{e^{-}} (MicroBooNE-CC, FD)", "#chi^{2}_{e^{-}} Histogram (MicroBooNE-CC, FD);#chi^{2}_{e^{-}};",
                                                 500, -1.5 * Chi2_Electron_cut_MicroBooNE_FD, 1.5 * Chi2_Electron_cut_MicroBooNE_FD);
    TH1D *Chi2_Proton_MicroBooNE_CD = new TH1D("#chi^{2}_{p} (MicroBooNE-CC, CD)", "#chi^{2}_{p} Histogram (MicroBooNE-CC, CD);#chi^{2}_{p};",
                                               500, -1.5 * Chi2_Proton_cut_MicroBooNE_CD, 1.5 * Chi2_Proton_cut_MicroBooNE_CD);
    TH1D *Chi2_Proton_MicroBooNE_FD = new TH1D("#chi^{2}_{p} (MicroBooNE-CC, FD)", "#chi^{2}_{p} Histogram (MicroBooNE-CC, FD);#chi^{2}_{p};",
                                               500, -1.5 * Chi2_Proton_cut_MicroBooNE_FD, 1.5 * Chi2_Proton_cut_MicroBooNE_FD);
    TH1D *Chi2_piplus_MicroBooNE_CD = new TH1D("#chi^{2}_{#pi^{+}} (MicroBooNE-CC, CD)", "#chi^{2}_{#pi^{+}} Histogram (MicroBooNE-CC, CD);#chi^{2}_{#pi^{+}};",
                                               500, -1.5 * Chi2_piplus_cut_MicroBooNE_CD, 1.5 * Chi2_piplus_cut_MicroBooNE_CD);
    TH1D *Chi2_piplus_MicroBooNE_FD = new TH1D("#chi^{2}_{#pi^{+}} (MicroBooNE-CC, FD)", "#chi^{2}_{#pi^{+}} Histogram (MicroBooNE-CC, FD);#chi^{2}_{#pi^{+}};",
                                               500, -1.5 * Chi2_piplus_cut_MicroBooNE_FD, 1.5 * Chi2_piplus_cut_MicroBooNE_FD);
    TH1D *Chi2_piminus_MicroBooNE_CD = new TH1D("#chi^{2}_{#pi^{-}} (MicroBooNE-CC, CD)",
                                                "#chi^{2}_{#pi^{-}} Histogram (MicroBooNE-CC, CD);#chi^{2}_{#pi^{-}};",
                                                500, -1.5 * Chi2_piminus_cut_MicroBooNE_CD, 1.5 * Chi2_piminus_cut_MicroBooNE_CD);
    TH1D *Chi2_piminus_MicroBooNE_FD = new TH1D("#chi^{2}_{#pi^{-}} (MicroBooNE-CC, FD)",
                                                "#chi^{2}_{#pi^{-}} Histogram (MicroBooNE-CC, FD);#chi^{2}_{#pi^{-}};",
                                                500, -1.5 * Chi2_piminus_cut_MicroBooNE_FD, 1.5 * Chi2_piminus_cut_MicroBooNE_FD);
    string Chi2_Electron_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, Chi2_Electron_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;
    string Chi2_Proton_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, Chi2_Proton_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;
    string Chi2_piplus_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, Chi2_piplus_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;
    string Chi2_piminus_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, Chi2_piminus_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e2p)">
    THStack *Chi2_Electron_1e2p_Stack = new THStack("#chi^{2}_{e^{-}} (1e2p, CD & FD)", "#chi^{2}_{e^{-}} Histogram (1e2p, CD & FD);#chi^{2}_{e^{-}};");
    THStack *Chi2_Proton_1e2p_Stack = new THStack("#chi^{2}_{p} (1e2p, CD & FD)", "#chi^{2}_{p} Histogram (1e2p, CD & FD);#chi^{2}_{p};");

    //<editor-fold desc="Testing Chi2 before cuts (1e2p)">
    TH1D *Chi2_Electron_1e2p_BC_CD = new TH1D("#chi^{2}_{e^{-}} BC (1e2p, CD)", "#chi^{2}_{e^{-}} Histogram Before Cut (1e2p, CD);#chi^{2}_{e^{-}};",
                                              500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_1e2p_BC_FD = new TH1D("#chi^{2}_{e^{-}} BC (1e2p, FD)", "#chi^{2}_{e^{-}} Histogram Before Cut (1e2p, FD);#chi^{2}_{e^{-}};",
                                              500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_1e2p_BC_CD = new TH1D("#chi^{2}_{p} BC (1e2p, CD)", "#chi^{2}_{p} Histogram Before Cut (1e2p, CD);#chi^{2}_{p};",
                                            500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_1e2p_BC_FD = new TH1D("#chi^{2}_{p} BC (1e2p, FD)", "#chi^{2}_{p} Histogram Before Cut (1e2p, FD);#chi^{2}_{p};",
                                            500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string Chi2_Electron_1e2p_BC_CD_Dir = Chi2_1e2p_cut_tests_Directory, Chi2_Electron_1e2p_BC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
    string Chi2_Proton_1e2p_BC_CD_Dir = Chi2_1e2p_cut_tests_Directory, Chi2_Proton_1e2p_BC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
    //</editor-fold>

    //<editor-fold desc="Testing Chi2 after cuts (1e2p)">
    TH1D *Chi2_Electron_1e2p_AC_CD = new TH1D("#chi^{2}_{e^{-}} AC (1e2p, CD)", "#chi^{2}_{e^{-}} Histogram After Cut (1e2p, CD);#chi^{2}_{e^{-}};",
                                              500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_1e2p_AC_FD = new TH1D("#chi^{2}_{e^{-}} AC (1e2p, FD)", "#chi^{2}_{e^{-}} Histogram After Cut (1e2p, FD);#chi^{2}_{e^{-}};",
                                              500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_1e2p_AC_CD = new TH1D("#chi^{2}_{p} AC (1e2p, CD)", "#chi^{2}_{p} Histogram After Cut (1e2p, CD);#chi^{2}_{p};",
                                            500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_1e2p_AC_FD = new TH1D("#chi^{2}_{p} AC (1e2p, FD)", "#chi^{2}_{p} Histogram After Cut (1e2p, FD);#chi^{2}_{p};",
                                            500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string Chi2_Electron_1e2p_AC_CD_Dir = Chi2_1e2p_cut_tests_Directory, Chi2_Electron_1e2p_AC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
    string Chi2_Proton_1e2p_AC_CD_Dir = Chi2_1e2p_cut_tests_Directory, Chi2_Proton_1e2p_AC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Chi2 plots (2p)">
    TH1D *Chi2_Electron_2p_CD = new TH1D("#chi^{2}_{e^{-}} (2p, CD)", "#chi^{2}_{e^{-}} Histogram (2p, CD);#chi^{2}_{e^{-}};",
                                         500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *Chi2_Electron_2p_FD = new TH1D("#chi^{2}_{e^{-}} (2p, FD)", "#chi^{2}_{e^{-}} Histogram (2p, FD);#chi^{2}_{e^{-}};",
                                         500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *Chi2_Proton_2p_CD = new TH1D("#chi^{2}_{p} (2p, CD)", "#chi^{2}_{p} Histogram (2p, CD);#chi^{2}_{p};",
                                       500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *Chi2_Proton_2p_FD = new TH1D("#chi^{2}_{p} (2p, FD)", "#chi^{2}_{p} Histogram (2p, FD);#chi^{2}_{p};",
                                       500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string Chi2_Electron_2p_CD_Dir = Chi2_2p_Directory, Chi2_Electron_2p_FD_Dir = Chi2_2p_Directory;
    string Chi2_Proton_2p_CD_Dir = Chi2_2p_Directory, Chi2_Proton_2p_FD_Dir = Chi2_2p_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">

    //<editor-fold desc="Vertex plots (no #(e) cut)">
    // TODO: remove the "TEST" and return to pointer vertex plots (using git backup)
    THStack Vertex_Electron_Vx_Stack_test("Electron V_{x} (CD & FD) test", "Electron V_{x} (CD & FD);Electron V_{x} [cm];");
    THStack Vertex_Electron_Vy_Stack_test("Electron V_{y} (CD & FD) test", "Electron V_{y} (CD & FD);Electron V_{y} [cm];");
    THStack Vertex_Electron_Vz_Stack_test("Electron V_{z} (CD & FD) test", "Electron V_{z} (CD & FD);Electron V_{z} [cm];");
    THStack Vertex_Proton_Vx_Stack_test("Proton V_{x} (CD & FD) test", "Proton V_{x} (CD & FD);Proton V_{x} [cm];");
    THStack Vertex_Proton_Vy_Stack_test("Proton V_{y} (CD & FD) test", "Proton V_{y} (CD & FD);Proton V_{y} [cm];");
    THStack Vertex_Proton_Vz_Stack_test("Proton V_{z} (CD & FD) test", "Proton V_{z} (CD & FD);Proton V_{z} [cm];");

    TH1D Vertex_Electron_Vx_CD_test("Electron V_{x} (no #(e) cut, CD) test", "Electron V_{x} (no #(e) cut, CD);Electron V_{x} [cm];",
                                    100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_Vy_CD_test("Electron V_{y} (no #(e) cut, CD) test", "Electron V_{y} (no #(e) cut, CD);Electron V_{y} [cm];",
                                    100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_Vz_CD_test("Electron V_{z} (no #(e) cut, CD) test", "Electron V_{z} (no #(e) cut, CD);Electron V_{z} [cm];",
                                    100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_Vx_FD_test("Electron V_{x} (no #(e) cut, FD) test", "Electron V_{x} (no #(e) cut, CD);Electron V_{x} [cm];",
                                    100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_Vy_FD_test("Electron V_{y} (no #(e) cut, FD) test", "Electron V_{y} (no #(e) cut, CD);Electron V_{y} [cm];",
                                    100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_Vz_FD_test("Electron V_{z} (no #(e) cut, FD) test", "Electron V_{z} (no #(e) cut, CD);Electron V_{z} [cm];",
                                    100, Vertex_lower_lim, Vertex_upper_lim);

    TH1D Vertex_Proton_Vx_CD_test("Proton V_{x} (no #(e) cut, CD) test", "Proton V_{x} (no #(e) cut, CD);Proton V_{x} [cm];",
                                  100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_Vy_CD_test("Proton V_{y} (no #(e) cut, CD) test", "Proton V_{y} (no #(e) cut, CD);Proton V_{y} [cm];",
                                  100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_Vz_CD_test("Proton V_{z} (no #(e) cut, CD) test", "Proton V_{z} (no #(e) cut, CD);Proton V_{z} [cm];",
                                  100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_Vx_FD_test("Proton V_{x} (no #(e) cut, FD) test", "Proton V_{x} (no #(e) cut, CD);Proton V_{x} [cm];",
                                  100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_Vy_FD_test("Proton V_{y} (no #(e) cut, FD) test", "Proton V_{y} (no #(e) cut, CD);Proton V_{y} [cm];",
                                  100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_Vz_FD_test("Proton V_{z} (no #(e) cut, FD) test", "Proton V_{z} (no #(e) cut, CD);Proton V_{z} [cm];",
                                  100, Vertex_lower_lim, Vertex_upper_lim);
    //</editor-fold>

    //<editor-fold desc="Vertex plots (1e only)">
    THStack Vertex_Electron_1e_Vx_Stack_test("Electron V_{x} (1e only, CD & FD) test", "Electron V_{x} (1e only, CD & FD);Electron V_{x} [cm];");
    THStack Vertex_Electron_1e_Vy_Stack_test("Electron V_{y} (1e only, CD & FD) test", "Electron V_{y} (1e only, CD & FD);Electron V_{y} [cm];");
    THStack Vertex_Electron_1e_Vz_Stack_test("Electron V_{z} (1e only, CD & FD) test", "Electron V_{z} (1e only, CD & FD);Electron V_{z} [cm];");
    THStack Vertex_Proton_1e_Vx_Stack_test("Proton V_{x} (1e only, CD & FD) test", "Proton V_{x} (1e only, CD & FD);Proton V_{x} [cm];");
    THStack Vertex_Proton_1e_Vy_Stack_test("Proton V_{y} (1e only, CD & FD) test", "Proton V_{y} (1e only, CD & FD);Proton V_{y} [cm];");
    THStack Vertex_Proton_1e_Vz_Stack_test("Proton V_{z} (1e only, CD & FD) test", "Proton V_{z} (1e only, CD & FD);Proton V_{z} [cm];");

    TH1D Vertex_Electron_1e_Vx_CD_test("Electron V_{x} (1e only, CD) test", "Electron V_{x} (1e only, CD);Electron V_{x} [cm];",
                                       100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_1e_Vy_CD_test("Electron V_{y} (1e only, CD) test", "Electron V_{y} (1e only, CD);Electron V_{y} [cm];",
                                       100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_1e_Vz_CD_test("Electron V_{z} (1e only, CD) test", "Electron V_{z} (1e only, CD);Electron V_{z} [cm];",
                                       100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_1e_Vx_FD_test("Electron V_{x} (1e only, FD) test", "Electron V_{x} (1e only, FD);Electron V_{x} [cm];",
                                       100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_1e_Vy_FD_test("Electron V_{y} (1e only, FD) test", "Electron V_{y} (1e only, FD);Electron V_{y} [cm];",
                                       100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Electron_1e_Vz_FD_test("Electron V_{z} (1e only, FD) test", "Electron V_{z} (1e only, FD);Electron V_{z} [cm];",
                                       100, Vertex_lower_lim, Vertex_upper_lim);

    TH1D Vertex_Proton_1e_Vx_CD_test("Proton V_{x} (1e only, CD) test", "Proton V_{x} (1e only, CD);Proton V_{x} [cm];",
                                     100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_1e_Vy_CD_test("Proton V_{y} (1e only, CD) test", "Proton V_{y} (1e only, CD);Proton V_{y} [cm];",
                                     100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_1e_Vz_CD_test("Proton V_{z} (1e only, CD) test", "Proton V_{z} (1e only, CD);Proton V_{z} [cm];",
                                     100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_1e_Vx_FD_test("Proton V_{x} (1e only, FD) test", "Proton V_{x} (1e only, FD);Proton V_{x} [cm];",
                                     100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_1e_Vy_FD_test("Proton V_{y} (1e only, FD) test", "Proton V_{y} (1e only, FD);Proton V_{y} [cm];",
                                     100, Vertex_lower_lim, Vertex_upper_lim);
    TH1D Vertex_Proton_1e_Vz_FD_test("Proton V_{z} (1e only, FD) test", "Proton V_{z} (1e only, FD);Proton V_{z} [cm];",
                                     100, Vertex_lower_lim, Vertex_upper_lim);
    //</editor-fold>

    //<editor-fold desc="dV plots (1e2p & chi2 cuts)">
    THStack *dVx_1e2p_before_Stack = new THStack("dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (1e2p & All #chi^{2} cuts, CD & FD)",
                                                 "dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (1e2p & All #chi^{2} cuts, CD & FD) test;dV_{x} [cm];");
    THStack *dVy_1e2p_before_Stack = new THStack("dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (1e2p & All #chi^{2} cuts, CD & FD)",
                                                 "dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (1e2p & All #chi^{2} cuts, CD & FD) test;dV_{y} [cm];");
    THStack *dVz_1e2p_before_Stack = new THStack("dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (1e2p & All #chi^{2} cuts, CD & FD)",
                                                 "dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (1e2p & All #chi^{2} cuts, CD & FD) test;dV_{z} [cm];");

    THStack *dVx_1e2p_after_Stack = new THStack("dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (1e2p & All #chi^{2} cuts, CD & FD)",
                                                "dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (1e2p & All #chi^{2} cuts, CD & FD) test;dV_{x} [cm];");
    THStack *dVy_1e2p_after_Stack = new THStack("dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (1e2p & All #chi^{2} cuts, CD & FD)",
                                                "dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (1e2p & All #chi^{2} cuts, CD & FD) test;dV_{y} [cm];");
    THStack *dVz_1e2p_after_Stack = new THStack("dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (1e2p & All #chi^{2} cuts, CD & FD)",
                                                "dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (1e2p & All #chi^{2} cuts, CD & FD) test;dV_{z} [cm];");

    THStack *dVx_2p_Stack = new THStack("dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (2p, CD & FD)", "dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (2p, CD & FD);dV_{x} [cm];");
    THStack *dVy_2p_Stack = new THStack("dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (2p, CD & FD)", "dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (2p, CD & FD);dV_{y} [cm];");
    THStack *dVz_2p_Stack = new THStack("dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (2p, CD & FD)", "dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (2p, CD & FD);dV_{z} [cm];");

    TH1D *deltaVx_before_dV_cuts_1e2p = new TH1D("dV_{x} BC (1e2p, CD & FD)",
                                                 "dV_{x}=V^{e}_{x}-V^{p}_{x} Before Cuts (1e2p, CD & FD);dV_{x} [cm];", 500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVy_before_dV_cuts_1e2p = new TH1D("dV_{y} BC (1e2p, CD & FD)",
                                                 "dV_{y}=V^{e}_{y}-V^{p}_{y} Before Cuts (1e2p, CD & FD);dV_{y} [cm];", 500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVz_before_dV_cuts_1e2p = new TH1D("dV_{z} BC (1e2p, CD & FD)",
                                                 "dV_{z}=V^{e}_{z}-V^{p}_{z} Before Cuts (1e2p, CD & FD);dV_{z} [cm];", 500, dV_lower_lim, dV_upper_lim);

    TH1D *deltaVx_after_dV_cuts_1e2p = new TH1D("dV_{x} AC (1e2p, CD & FD)",
                                                "dV_{x}=V^{e}_{x}-V^{p}_{x} After Cuts (1e2p, CD & FD);dV_{x} [cm];", 500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVy_after_dV_cuts_1e2p = new TH1D("dV_{y} AC (1e2p, CD & FD)",
                                                "dV_{y}=V^{e}_{y}-V^{p}_{y} After Cuts (1e2p, CD & FD);dV_{y} [cm];", 500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVz_after_dV_cuts_1e2p = new TH1D("dV_{z} AC (1e2p, CD & FD)",
                                                "dV_{z}=V^{e}_{z}-V^{p}_{z} After Cuts (1e2p, CD & FD);dV_{z} [cm];", 500, dV_lower_lim, dV_upper_lim);

    TH1D *deltaVx_2p = new TH1D("dV_{x} AC (2p, CD & FD)", "dV_{x}=V^{e}_{x}-V^{p}_{x} (2p, CD & FD);dV_{x} [cm];", 500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVy_2p = new TH1D("dV_{y} AC (2p, CD & FD)", "dV_{y}=V^{e}_{y}-V^{p}_{y} (2p, CD & FD);dV_{y} [cm];", 500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVz_2p = new TH1D("dV_{z} AC (2p, CD & FD)", "dV_{z}=V^{e}_{z}-V^{p}_{z} (2p, CD & FD);dV_{z} [cm];", 500, dV_lower_lim, dV_upper_lim);
    //</editor-fold>

    //<editor-fold desc="dV plots (MicroBooNE)">
    THStack *dVx_MicroBooNE_Stack = new THStack("dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (MicroBooNE, CD & FD)",
                                                "dV_{x}=|V^{e}_{x}-dV^{p}_{x}| (MicroBooNE, CD & FD);dV_{x} [cm];");
    THStack *dVy_MicroBooNE_Stack = new THStack("dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (MicroBooNE, CD & FD)",
                                                "dV_{y}=|V^{e}_{y}-dV^{p}_{y}| (MicroBooNE, CD & FD);dV_{y} [cm];");
    THStack *dVz_MicroBooNE_Stack = new THStack("dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (MicroBooNE, CD & FD)",
                                                "dV_{z}=|V^{e}_{z}-dV^{p}_{z}| (MicroBooNE, CD & FD);dV_{z} [cm];");

    TH1D *deltaVx_MicroBooNE_BC = new TH1D("dV_{x} BC (MicroBooNE, CD & FD)", "dV_{x}=V^{e}_{x}-V^{p}_{x} Before Cuts (MicroBooNE, CD & FD);dV_{x} [cm];",
                                           500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVy_MicroBooNE_BC = new TH1D("dV_{y} BC (MicroBooNE, CD & FD)", "dV_{y}=V^{e}_{y}-V^{p}_{y} Before Cuts (MicroBooNE, CD & FD);dV_{y} [cm];",
                                           500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVz_MicroBooNE_BC = new TH1D("dV_{z} BC (MicroBooNE, CD & FD)", "dV_{z}=V^{e}_{z}-V^{p}_{z} Before Cuts (MicroBooNE, CD & FD);dV_{z} [cm];",
                                           500, dV_lower_lim, dV_upper_lim);

    TH1D *deltaVx_MicroBooNE_AC = new TH1D("dV_{x} AC (MicroBooNE, CD & FD)", "dV_{x}=V^{e}_{x}-V^{p}_{x} After Cuts (MicroBooNE, CD & FD);dV_{x} [cm];",
                                           500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVy_MicroBooNE_AC = new TH1D("dV_{y} AC (MicroBooNE, CD & FD)", "dV_{y}=V^{e}_{y}-V^{p}_{y} After Cuts (MicroBooNE, CD & FD);dV_{y} [cm];",
                                           500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVz_MicroBooNE_AC = new TH1D("dV_{z} AC (MicroBooNE, CD & FD)", "dV_{z}=V^{e}_{z}-V^{p}_{z} After Cuts (MicroBooNE, CD & FD);dV_{z} [cm];",
                                           500, dV_lower_lim, dV_upper_lim);

    TH1D *deltaVx_MicroBooNE = new TH1D("dV_{x} (MicroBooNE, CD & FD)", "dV_{x}=V^{e}_{x}-V^{p}_{x} (MicroBooNE, CD & FD);dV_{x} [cm];",
                                        500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVy_MicroBooNE = new TH1D("dV_{y} (MicroBooNE, CD & FD)", "dV_{y}=V^{e}_{y}-V^{p}_{y} (MicroBooNE, CD & FD);dV_{y} [cm];",
                                        500, dV_lower_lim, dV_upper_lim);
    TH1D *deltaVz_MicroBooNE = new TH1D("dV_{z} (MicroBooNE, CD & FD)", "dV_{z}=V^{e}_{z}-V^{p}_{z} (MicroBooNE, CD & FD);dV_{z} [cm];",
                                        500, dV_lower_lim, dV_upper_lim);
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Angle histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Angle histograms">

    // Theta_e --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e histograms">
    THStack *Theta_e_Stack = new THStack("#theta_{e} stack (CD & FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, CD & FD);#theta_{e} [Deg];");

    TH1D *Theta_e_CD = new TH1D("#theta_{e} (no #(e) cut, CD)", ";#theta_{e} [Deg];", 250, 35, 140);
    TH1D *Theta_e_FD = new TH1D("#theta_{e} (no #(e) cut, FD)", ";#theta_{e} [Deg];", 250, 0, 50);
    string Theta_e_CD_Dir = Theta_e_All_e_Directory, Theta_e_FD_Dir = Theta_e_All_e_Directory;

    TH1D *Theta_e_1e_CD = new TH1D("#theta_{e} (1e Only Cut, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e_FD = new TH1D("#theta_{e} (1e Only Cut, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    string Theta_e_1e_CD_Dir = Theta_e_Only_1e_cut_Directory, Theta_e_1e_FD_Dir = Theta_e_Only_1e_cut_Directory;

    TH1D *Theta_e_1e2X_CD = new TH1D("#theta_{e} for 1e2X (All int., CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2X_QEL_CD = new TH1D("#theta_{e} for 1e2X (QEL Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2X_MEC_CD = new TH1D("#theta_{e} for 1e2X (MEC Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2X_RES_CD = new TH1D("#theta_{e} for 1e2X (RES Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2X_DIS_CD = new TH1D("#theta_{e} for 1e2X (DIS Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2X_FD = new TH1D("#theta_{e} for 1e2X (All int., FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2X_QEL_FD = new TH1D("#theta_{e} for 1e2X (QEL Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2X_MEC_FD = new TH1D("#theta_{e} for 1e2X (MEC Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2X_RES_FD = new TH1D("#theta_{e} for 1e2X (RES Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2X_DIS_FD = new TH1D("#theta_{e} for 1e2X (DIS Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    string Theta_e_1e2X_CD_Dir = Theta_e_Only_1e2X_Directory, Theta_e_1e2X_FD_Dir = Theta_e_Only_1e2X_Directory;
    string Theta_e_1e2X_QEL_CD_Dir = Theta_e_Only_1e2X_QEL_Directory, Theta_e_1e2X_QEL_FD_Dir = Theta_e_Only_1e2X_QEL_Directory;
    string Theta_e_1e2X_MEC_CD_Dir = Theta_e_Only_1e2X_MEC_Directory, Theta_e_1e2X_MEC_FD_Dir = Theta_e_Only_1e2X_MEC_Directory;
    string Theta_e_1e2X_RES_CD_Dir = Theta_e_Only_1e2X_RES_Directory, Theta_e_1e2X_RES_FD_Dir = Theta_e_Only_1e2X_RES_Directory;
    string Theta_e_1e2X_DIS_CD_Dir = Theta_e_Only_1e2X_DIS_Directory, Theta_e_1e2X_DIS_FD_Dir = Theta_e_Only_1e2X_DIS_Directory;

    TH1D *Theta_e_1e2p_CD = new TH1D("#theta_{e} 1e2p (All int.,CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2p_QEL_CD = new TH1D("#theta_{e} for 1e2p (QEL Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2p_MEC_CD = new TH1D("#theta_{e} for 1e2p (MEC Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2p_RES_CD = new TH1D("#theta_{e} for 1e2p (RES Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2p_DIS_CD = new TH1D("#theta_{e} for 1e2p (DIS Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_1e2p_FD = new TH1D("#theta_{e} 1e2p (All int.,FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2p_QEL_FD = new TH1D("#theta_{e} for 1e2p (QEL Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2p_MEC_FD = new TH1D("#theta_{e} for 1e2p (MEC Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2p_RES_FD = new TH1D("#theta_{e} for 1e2p (RES Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_1e2p_DIS_FD = new TH1D("#theta_{e} for 1e2p (DIS Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    string Theta_e_1e2p_CD_Dir = Theta_e_Only_1e2p_Directory, Theta_e_1e2p_FD_Dir = Theta_e_Only_1e2p_Directory;
    string Theta_e_1e2p_QEL_CD_Dir = Theta_e_Only_1e2p_QEL_Directory, Theta_e_1e2p_QEL_FD_Dir = Theta_e_Only_1e2p_QEL_Directory;
    string Theta_e_1e2p_MEC_CD_Dir = Theta_e_Only_1e2p_MEC_Directory, Theta_e_1e2p_MEC_FD_Dir = Theta_e_Only_1e2p_MEC_Directory;
    string Theta_e_1e2p_RES_CD_Dir = Theta_e_Only_1e2p_RES_Directory, Theta_e_1e2p_RES_FD_Dir = Theta_e_Only_1e2p_RES_Directory;
    string Theta_e_1e2p_DIS_CD_Dir = Theta_e_Only_1e2p_DIS_Directory, Theta_e_1e2p_DIS_FD_Dir = Theta_e_Only_1e2p_DIS_Directory;

    TH1D *Theta_e_2p_CD = new TH1D("#theta_{e} 2p (All int.,CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_2p_QEL_CD = new TH1D("#theta_{e} for 2p (QEL Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_2p_MEC_CD = new TH1D("#theta_{e} for 2p (MEC Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_2p_RES_CD = new TH1D("#theta_{e} for 2p (RES Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_2p_DIS_CD = new TH1D("#theta_{e} for 2p (DIS Only, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *Theta_e_2p_FD = new TH1D("#theta_{e} 2p (All int.,FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_2p_QEL_FD = new TH1D("#theta_{e} for 2p (QEL Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_2p_MEC_FD = new TH1D("#theta_{e} for 2p (MEC Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_2p_RES_FD = new TH1D("#theta_{e} for 2p (RES Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    TH1D *Theta_e_2p_DIS_FD = new TH1D("#theta_{e} for 2p (DIS Only, FD)", ";#theta_{e} [Deg];", 100, 0, 50);
    string Theta_e_2p_CD_Dir = Theta_e_Only_2p_Directory, Theta_e_2p_FD_Dir = Theta_e_Only_2p_Directory;
    string Theta_e_2p_QEL_CD_Dir = Theta_e_Only_2p_QEL_Directory, Theta_e_2p_QEL_FD_Dir = Theta_e_Only_2p_QEL_Directory;
    string Theta_e_2p_MEC_CD_Dir = Theta_e_Only_2p_MEC_Directory, Theta_e_2p_MEC_FD_Dir = Theta_e_Only_2p_MEC_Directory;
    string Theta_e_2p_RES_CD_Dir = Theta_e_Only_2p_RES_Directory, Theta_e_2p_RES_FD_Dir = Theta_e_Only_2p_RES_Directory;
    string Theta_e_2p_DIS_CD_Dir = Theta_e_Only_2p_DIS_Directory, Theta_e_2p_DIS_FD_Dir = Theta_e_Only_2p_DIS_Directory;
    //</editor-fold>

// Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_e histograms">
    THStack *Phi_e_Stack = new THStack("#phi_{e} stack (CD & FD)", "#phi_{e} of Outgoing Electron (no #(e) cut, CD & FD);#phi_{e} [Deg];");

    TH1D *Phi_e_CD = new TH1D("#phi_{e} (no #(e) cut, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_FD = new TH1D("#phi_{e} (no #(e) cut, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string Phi_e_CD_Dir = Phi_e_All_e_Directory, Phi_e_FD_Dir = Phi_e_All_e_Directory;

    TH1D *Phi_e_1e_CD = new TH1D("#phi_{e} (1e Only Cut, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e_FD = new TH1D("#phi_{e} (1e Only Cut, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string Phi_e_1e_CD_Dir = Phi_e_Only_1e_cut_Directory, Phi_e_1e_FD_Dir = Phi_e_Only_1e_cut_Directory;

    TH1D *Phi_e_1e2X_CD = new TH1D("#phi_{e} 1e2X (All int.,CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_QEL_CD = new TH1D("#phi_{e} for 1e2X (QEL Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_MEC_CD = new TH1D("#phi_{e} for 1e2X (MEC Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_RES_CD = new TH1D("#phi_{e} for 1e2X (RES Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_DIS_CD = new TH1D("#phi_{e} for 1e2X (DIS Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_FD = new TH1D("#phi_{e} 1e2X (All int.,FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_QEL_FD = new TH1D("#phi_{e} for 1e2X (QEL Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_MEC_FD = new TH1D("#phi_{e} for 1e2X (MEC Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_RES_FD = new TH1D("#phi_{e} for 1e2X (RES Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2X_DIS_FD = new TH1D("#phi_{e} for 1e2X (DIS Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string Phi_e_1e2X_CD_Dir = Phi_e_Only_1e2X_cut_Directory, Phi_e_1e2X_FD_Dir = Phi_e_Only_1e2X_cut_Directory;
    string Phi_e_1e2X_QEL_CD_Dir = Phi_e_Only_1e2X_QEL_Directory, Phi_e_1e2X_QEL_FD_Dir = Phi_e_Only_1e2X_QEL_Directory;
    string Phi_e_1e2X_MEC_CD_Dir = Phi_e_Only_1e2X_MEC_Directory, Phi_e_1e2X_MEC_FD_Dir = Phi_e_Only_1e2X_MEC_Directory;
    string Phi_e_1e2X_RES_CD_Dir = Phi_e_Only_1e2X_RES_Directory, Phi_e_1e2X_RES_FD_Dir = Phi_e_Only_1e2X_RES_Directory;
    string Phi_e_1e2X_DIS_CD_Dir = Phi_e_Only_1e2X_DIS_Directory, Phi_e_1e2X_DIS_FD_Dir = Phi_e_Only_1e2X_DIS_Directory;

    TH1D *Phi_e_1e2p_CD = new TH1D("#phi_{e} 1e2p (All int.,CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_QEL_CD = new TH1D("#phi_{e} for 1e2p (QEL Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_MEC_CD = new TH1D("#phi_{e} for 1e2p (MEC Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_RES_CD = new TH1D("#phi_{e} for 1e2p (RES Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_DIS_CD = new TH1D("#phi_{e} for 1e2p (DIS Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_FD = new TH1D("#phi_{e} 1e2p (All int.,FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_QEL_FD = new TH1D("#phi_{e} for 1e2p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_MEC_FD = new TH1D("#phi_{e} for 1e2p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_RES_FD = new TH1D("#phi_{e} for 1e2p (RES Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_1e2p_DIS_FD = new TH1D("#phi_{e} for 1e2p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string Phi_e_1e2p_CD_Dir = Phi_e_Only_1e2p_Directory, Phi_e_1e2p_FD_Dir = Phi_e_Only_1e2p_Directory;
    string Phi_e_1e2p_QEL_CD_Dir = Phi_e_Only_1e2p_QEL_Directory, Phi_e_1e2p_QEL_FD_Dir = Phi_e_Only_1e2p_QEL_Directory;
    string Phi_e_1e2p_MEC_CD_Dir = Phi_e_Only_1e2p_MEC_Directory, Phi_e_1e2p_MEC_FD_Dir = Phi_e_Only_1e2p_MEC_Directory;
    string Phi_e_1e2p_RES_CD_Dir = Phi_e_Only_1e2p_RES_Directory, Phi_e_1e2p_RES_FD_Dir = Phi_e_Only_1e2p_RES_Directory;
    string Phi_e_1e2p_DIS_CD_Dir = Phi_e_Only_1e2p_DIS_Directory, Phi_e_1e2p_DIS_FD_Dir = Phi_e_Only_1e2p_DIS_Directory;

    TH1D *Phi_e_2p_CD = new TH1D("#phi_{e} 2p (All int.,CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_QEL_CD = new TH1D("#phi_{e} for 2p (QEL Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_MEC_CD = new TH1D("#phi_{e} for 2p (MEC Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_RES_CD = new TH1D("#phi_{e} for 2p (RES Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_DIS_CD = new TH1D("#phi_{e} for 2p (DIS Only, CD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_FD = new TH1D("#phi_{e} 2p (All int.,FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_QEL_FD = new TH1D("#phi_{e} for 2p (QEL Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_MEC_FD = new TH1D("#phi_{e} for 2p (MEC Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_RES_FD = new TH1D("#phi_{e} for 2p (RES Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *Phi_e_2p_DIS_FD = new TH1D("#phi_{e} for 2p (DIS Only, FD)", ";#phi_{e} [Deg];", 100, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string Phi_e_2p_CD_Dir = Phi_e_Only_2p_Directory, Phi_e_2p_FD_Dir = Phi_e_Only_2p_Directory;
    string Phi_e_2p_QEL_CD_Dir = Phi_e_Only_2p_QEL_Directory, Phi_e_2p_QEL_FD_Dir = Phi_e_Only_2p_QEL_Directory;
    string Phi_e_2p_MEC_CD_Dir = Phi_e_Only_2p_MEC_Directory, Phi_e_2p_MEC_FD_Dir = Phi_e_Only_2p_MEC_Directory;
    string Phi_e_2p_RES_CD_Dir = Phi_e_Only_2p_RES_Directory, Phi_e_2p_RES_FD_Dir = Phi_e_Only_2p_RES_Directory;
    string Phi_e_2p_DIS_CD_Dir = Phi_e_Only_2p_DIS_Directory, Phi_e_2p_DIS_FD_Dir = Phi_e_Only_2p_DIS_Directory;
    //</editor-fold>

// Theta_e vs. Phi_e -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e vs. Phi_e">

    //<editor-fold desc="Theta_e vs. Phi_e histograms (no #(e) cut)">
    TH2D *Theta_e_VS_Phi_e_CD = new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, CD)",
                                         "#theta_{e} vs. #phi_{e}  (no #(e) cut, CD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                         250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 35, 140);
    TH2D *Theta_e_VS_Phi_e_FD = new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, FD)",
                                         "#theta_{e} vs. #phi_{e}  (no #(e) cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                         250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 0, 50);
    //</editor-fold>

    //<editor-fold desc="Theta_e vs. Phi_e histograms (1e Only Cut)">
    TH2D *Theta_e_VS_Phi_e_1e_CD = new TH2D("#theta_{e} vs. #phi_{e} (1e Only Cut, CD)",
                                            "#theta_{e} vs. #phi_{e}  (1e Only Cut, CD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                            250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 35, 140);
    TH2D *Theta_e_VS_Phi_e_1e_FD = new TH2D("#theta_{e} vs. #phi_{e} (1e Only Cut, FD)",
                                            "#theta_{e} vs. #phi_{e} (1e Only Cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                            250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 0, 50);
    //</editor-fold>

    //<editor-fold desc="Theta_e vs. Phi_e histograms (2p)">
    TH2D *Theta_e_VS_Phi_e_2p_CD = new TH2D("#theta_{e} vs. #phi_{e} (All int., 2p, CD)", "#theta_{e} vs. #phi_{e} (All int., 2p, CD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                            250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 35, 140);
    TH2D *Theta_e_VS_Phi_e_2p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All int., 2p, FD)", "#theta_{e} vs. #phi_{e} (All int., 2p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                            250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 0, 50);
    string Theta_e_VS_Phi_e_2p_CD_Dir = Theta_e_VS_Phi_e_2p_Directory, Theta_e_VS_Phi_e_2p_FD_Dir = Theta_e_VS_Phi_e_2p_Directory;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum histograms">

    //<editor-fold desc="Momentum MicroBooNE plots">
    THStack *P_Stack_MicroBooNE_BC_CD = new THStack("Particle Momentum BC (MicroBooNE, CD)", "Particle Momentum Histogram BC (MicroBooNE, CD);P [GeV];");
    THStack *P_Stack_MicroBooNE_AC_CD = new THStack("Particle Momentum AC (MicroBooNE, CD)", "Particle Momentum Histogram AC (MicroBooNE, CD);P [GeV];");
    THStack *P_Stack_MicroBooNE_BC_FD = new THStack("Particle Momentum BC (MicroBooNE, FD)", "Particle Momentum Histogram BC (MicroBooNE, FD);P [GeV];");
    THStack *P_Stack_MicroBooNE_AC_FD = new THStack("Particle Momentum AC (MicroBooNE, FD)", "Particle Momentum Histogram AC (MicroBooNE, FD);P [GeV];");
    THStack *P_Stack_MicroBooNE_BC = new THStack("Particle Momentum BC (MicroBooNE, CD & FD)", "Particle Momentum Histogram BC (MicroBooNE, CD & FD);P [GeV];");
    THStack *P_Stack_MicroBooNE_AC = new THStack("Particle Momentum AC (MicroBooNE, CD & FD)", "Particle Momentum Histogram AC (MicroBooNE, CD & FD);P [GeV];");
    THStack *P_Stack_MicroBooNE_CD = new THStack("Particle Momentum (MicroBooNE, CD)", "Particle Momentum Histogram (MicroBooNE, CD);P [GeV];");
    THStack *P_Stack_MicroBooNE_FD = new THStack("Particle Momentum (MicroBooNE, FD)", "Particle Momentum Histogram (MicroBooNE, FD);P [GeV];");

    //<editor-fold desc="Momentum histograms before MicroBooNE cuts test">
    TH1D *P_e_MicroBooNE_BC_CD = new TH1D("P_{e} BC (MicroBooNE, CD)", "P_{e} Before Cut (MicroBooNE, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_e_MicroBooNE_BC_FD = new TH1D("P_{e} BC (MicroBooNE, FD)", "P_{e} Before Cut (MicroBooNE, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string P_e_MicroBooNE_BC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_e_MicroBooNE_BC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;

    TH1D *P_p_MicroBooNE_BC_CD = new TH1D("P_{p} BC (MicroBooNE, CD)", "P_{p} Before Cut (MicroBooNE, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_p_MicroBooNE_BC_FD = new TH1D("P_{p} BC (MicroBooNE, FD)", "P_{p} Before Cut (MicroBooNE, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string P_p_MicroBooNE_BC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_p_MicroBooNE_BC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;

    TH1D *P_cpion_MicroBooNE_BC_CD = new TH1D("P_{#pi^{#pm}} BC (MicroBooNE, CD)", "P_{#pi^{#pm}} Before Cut (MicroBooNE, CD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_cpion_MicroBooNE_BC_FD = new TH1D("P_{#pi^{#pm}} BC (MicroBooNE, FD)", "P_{#pi^{#pm}} Before Cut (MicroBooNE, FD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
    string P_cpion_MicroBooNE_BC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_cpion_MicroBooNE_BC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after MicroBooNE cuts test">
    TH1D *P_e_MicroBooNE_AC_CD = new TH1D("P_{e} AC (MicroBooNE, CD)", "P_{e} After Cut (MicroBooNE, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_e_MicroBooNE_AC_FD = new TH1D("P_{e} AC (MicroBooNE, FD)", "P_{e} After Cut (MicroBooNE, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string P_e_MicroBooNE_AC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_e_MicroBooNE_AC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;

    TH1D *P_p_MicroBooNE_AC_CD = new TH1D("P_{p} AC (MicroBooNE, CD)", "P_{p} After Cut (MicroBooNE, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_p_MicroBooNE_AC_FD = new TH1D("P_{p} AC (MicroBooNE, FD)", "P_{p} After Cut (MicroBooNE, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string P_p_MicroBooNE_AC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_p_MicroBooNE_AC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;

    TH1D *P_cpion_MicroBooNE_AC_CD = new TH1D("P_{#pi^{#pm}} AC (MicroBooNE, CD)", "P_{#pi^{#pm}} After Cut (MicroBooNE, CD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_cpion_MicroBooNE_AC_FD = new TH1D("P_{#pi^{#pm}} AC (MicroBooNE, FD)", "P_{#pi^{#pm}} After Cut (MicroBooNE, FD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
    string P_cpion_MicroBooNE_AC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_cpion_MicroBooNE_AC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after MicroBooNE cuts">
    TH1D *P_e_MicroBooNE_CD = new TH1D("P_{e} (MicroBooNE, CD)", "P_{e} (MicroBooNE, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_e_MicroBooNE_FD = new TH1D("P_{e} (MicroBooNE, FD)", "P_{e} (MicroBooNE, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string P_e_MicroBooNE_CD_Dir = Momentum_MicroBooNE_Directory, P_e_MicroBooNE_FD_Dir = Momentum_MicroBooNE_Directory;

    TH1D *P_p_MicroBooNE_CD = new TH1D("P_{p} (MicroBooNE, CD)", "P_{p} (MicroBooNE, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_p_MicroBooNE_FD = new TH1D("P_{p} (MicroBooNE, FD)", "P_{p} (MicroBooNE, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string P_p_MicroBooNE_CD_Dir = Momentum_MicroBooNE_Directory, P_p_MicroBooNE_FD_Dir = Momentum_MicroBooNE_Directory;

    TH1D *P_cpion_MicroBooNE_CD = new TH1D("P_{#pi^{#pm}} (MicroBooNE, CD)", "P_{#pi^{#pm}} (MicroBooNE, CD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_cpion_MicroBooNE_FD = new TH1D("P_{#pi^{#pm}} (MicroBooNE, FD)", "P_{#pi^{#pm}} (MicroBooNE, FD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
    string P_cpion_MicroBooNE_CD_Dir = Momentum_MicroBooNE_Directory, P_cpion_MicroBooNE_FD_Dir = Momentum_MicroBooNE_Directory;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Momentum 1e2p & 2p plots">
    THStack *P_Stack_1e2p_BC_CD = new THStack("Particle Momentum BC (1e2p, CD)", "Particle Momentum Histogram BC (1e2p, CD);P [GeV];");
    THStack *P_Stack_1e2p_AC_CD = new THStack("Particle Momentum AC (1e2p, CD)", "Particle Momentum Histogram AC (1e2p, CD);P [GeV];");
    THStack *P_Stack_1e2p_BC_FD = new THStack("Particle Momentum BC (1e2p, FD)", "Particle Momentum Histogram BC (1e2p, FD);P [GeV];");
    THStack *P_Stack_1e2p_AC_FD = new THStack("Particle Momentum AC (1e2p, FD)", "Particle Momentum Histogram AC (1e2p, FD);P [GeV];");
    THStack *P_Stack_1e2p_BC = new THStack("Particle Momentum BC (1e2p, CD & FD)", "Particle Momentum Histogram BC (1e2p, CD & FD);P [GeV];");
    THStack *P_Stack_1e2p_AC = new THStack("Particle Momentum AC (1e2p, CD & FD)", "Particle Momentum Histogram AC (1e2p, CD & FD);P [GeV];");
    THStack *P_Stack_2p_CD = new THStack("Particle Momentum (2p, CD)", "Particle Momentum Histogram (2p, CD);P [GeV];");
    THStack *P_Stack_2p_FD = new THStack("Particle Momentum (2p, FD)", "Particle Momentum Histogram (2p, FD);P [GeV];");

    //<editor-fold desc="Momentum histograms before 1e2p cuts">
    TH1D *P_e_1e2p_BC_CD = new TH1D("P_{e} BC (1e2p, CD)", "P_{e} Before Cut (1e2p, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_e_1e2p_BC_FD = new TH1D("P_{e} BC (1e2p, FD)", "P_{e} Before Cut (1e2p, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string P_e_1e2p_BC_CD_Dir = Momentum_1e2p_Directory, P_e_1e2p_BC_FD_Dir = Momentum_1e2p_Directory;

    TH1D *P_p_1e2p_BC_CD = new TH1D("P_{p} BC (1e2p, CD)", "P_{p} Before Cut (1e2p, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_p_1e2p_BC_FD = new TH1D("P_{p} BC (1e2p, FD)", "P_{p} Before Cut (1e2p, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string P_p_1e2p_BC_CD_Dir = Momentum_1e2p_Directory, P_p_1e2p_BC_FD_Dir = Momentum_1e2p_Directory;
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after 1e2p cuts">
    TH1D *P_e_1e2p_AC_CD = new TH1D("P_{e} AC (1e2p, CD)", "P_{e} After Cut (1e2p, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_e_1e2p_AC_FD = new TH1D("P_{e} AC (1e2p, FD)", "P_{e} After Cut (1e2p, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string P_e_1e2p_AC_CD_Dir = Momentum_1e2p_Directory, P_e_1e2p_AC_FD_Dir = Momentum_1e2p_Directory;

    TH1D *P_p_1e2p_AC_CD = new TH1D("P_{p} AC (1e2p, CD)", "P_{p} After Cut (1e2p, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_p_1e2p_AC_FD = new TH1D("P_{p} AC (1e2p, FD)", "P_{p} After Cut (1e2p, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string P_p_1e2p_AC_CD_Dir = Momentum_1e2p_Directory, P_p_1e2p_AC_FD_Dir = Momentum_1e2p_Directory;
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after 2p cuts">
    TH1D *P_e_2p_CD = new TH1D("P_{e} (2p, CD)", "P_{e} (2p, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_e_2p_FD = new TH1D("P_{e} (2p, FD)", "P_{e} (2p, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string P_e_2p_CD_Dir = Momentum_2p_Directory, P_e_2p_FD_Dir = Momentum_2p_Directory;

    TH1D *P_p_2p_CD = new TH1D("P_{p} (2p, CD)", "P_{p} (2p, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *P_p_2p_FD = new TH1D("P_{p} (2p, FD)", "P_{p} (2p, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string P_p_2p_CD_Dir = Momentum_2p_Directory, P_p_2p_FD_Dir = Momentum_2p_Directory;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Q2 histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Q2 histograms">
    THStack *Q2_Stack = new THStack("Q^{2} stack (CD & FD)", "Q^{2} Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}];");
    THStack *Q2_1e2X_Stack = new THStack("Q^{2} 1e2X stack (CD & FD)", "Q^{2} 1e2X Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}];");
    THStack *Q2_1e2p_Stack = new THStack("Q^{2} 1e2p stack (CD & FD)", "Q^{2} 1e2p Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}];");

    TH1D *Q2_histogram_CD = new TH1D("Q^{2} (no #(e) cut, CD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    TH1D *Q2_histogram_FD = new TH1D("Q^{2} (no #(e) cut, FD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    string Q2_histogram_CD_Dir = Q2_All_e_Directory, Q2_histogram_FD_Dir = Q2_All_e_Directory;

    TH1D *Q2_histogram_1e_CD = new TH1D("Q^{2} (1e Only Cut, CD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    TH1D *Q2_histogram_1e_FD = new TH1D("Q^{2} (1e Only Cut, FD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    string Q2_histogram_1e_CD_Dir = Q2_Only_1e_cut_Directory, Q2_histogram_1e_FD_Dir = Q2_Only_1e_cut_Directory;

    TH1D *Q2_histogram_1e2X_CD = new TH1D("Q^{2} 1e2X (CD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    TH1D *Q2_histogram_1e2X_FD = new TH1D("Q^{2} 1e2X (FD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    string Q2_histogram_1e2X_CD_Dir = Q2_Only_1e_cut_Directory, Q2_histogram_1e2X_FD_Dir = Q2_Only_1e_cut_Directory;

    TH1D *Q2_histogram_1e2p_CD = new TH1D("Q^{2} 1e2p (CD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    TH1D *Q2_histogram_1e2p_FD = new TH1D("Q^{2} 1e2p (FD)", ";Q^{2} [GeV^{2}];", 500, 0, 1.1 * beamE);
    string Q2_histogram_1e2p_CD_Dir = Q2_Only_1e_cut_Directory, Q2_histogram_1e2p_FD_Dir = Q2_Only_1e_cut_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Energy (E_e) histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Energy (E_e) histograms">

    //<editor-fold desc="Energy (E_e) histograms (1e Only Cut, CD & FD)">
    THStack *E_e_Stack = new THStack("E_{e} stack (CD & FD)", "E_{e} Histogram (1e Only Cut, CD & FD);E_{e} [GeV]");

    TH1D *E_e_hist_CD = new TH1D("E_{e} (1e Only Cut, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_hist_FD = new TH1D("E_{e} (1e Only Cut, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    string E_e_hist_CD_Dir = E_e_histograms_Directory, E_e_hist_FD_Dir = E_e_histograms_Directory;

    TH2D *E_e_VS_Theta_e_hist_CD = new TH2D("E_{e} vs. #theta_{e} (1e Only Cut, CD)", "E_{e} vs. #theta_{e} (1e Only Cut, CD);#theta_{e} [Deg];E_{e} [GeV]",
                                            250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_hist_FD = new TH2D("E_{e} vs. #theta_{e} (1e Only Cut, FD)", "E_{e} vs. #theta_{e} (1e Only Cut, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                            250, 0, 50, 250, 0, beamE * 1.1);
    string E_e_VS_Theta_e_hist_CD_Dir = E_e_VS_Theta_e_histograms_Directory, E_e_VS_Theta_e_hist_FD_Dir = E_e_VS_Theta_e_histograms_Directory;
    //</editor-fold>

    //<editor-fold desc="Energy (E_e) histograms (2p, CD & FD)">
    THStack *E_e_All_int_2p_Stack = new THStack("E_{e} stack (All Int., 2p, CD & FD)", "E_{e} Histogram (All Int., 2p, CD & FD);E_{e} [GeV]");
    THStack *E_e_QEL_2p_Stack = new THStack("E_{e} stack (QEL Only, 2p, CD & FD)", "E_{e} Histogram (QEL Only, 2p, CD & FD);E_{e} [GeV]");
    THStack *E_e_MEC_2p_Stack = new THStack("E_{e} stack (MEC Only, 2p, CD & FD)", "E_{e} Histogram (MEC Only, 2p, CD & FD);E_{e} [GeV]");
    THStack *E_e_RES_2p_Stack = new THStack("E_{e} stack (RES Only, 2p, CD & FD)", "E_{e} Histogram (RES Only, 2p, CD & FD);E_{e} [GeV]");
    THStack *E_e_DIS_2p_Stack = new THStack("E_{e} stack (DIS Only, 2p, CD & FD)", "E_{e} Histogram (DIS Only, 2p, CD & FD);E_{e} [GeV]");

    TH1D *E_e_2p_CD = new TH1D("E_{e} (All int., 2p, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_QEL_CD = new TH1D("E_{e} (QEL Only, 2p, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_MEC_CD = new TH1D("E_{e} (MEC Only, 2p, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_RES_CD = new TH1D("E_{e} (RES Only, 2p, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_DIS_CD = new TH1D("E_{e} (DIS Only, 2p, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_FD = new TH1D("E_{e} (1e Only Cut, 2p, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_QEL_FD = new TH1D("E_{e} (QEL Only, 2p, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_MEC_FD = new TH1D("E_{e} (MEC Only, 2p, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_RES_FD = new TH1D("E_{e} (RES Only, 2p, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *E_e_2p_DIS_FD = new TH1D("E_{e} (DIS Only, 2p, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    string E_e_2p_CD_Dir = E_e_2p_histograms_Directory, E_e_2p_FD_Dir = E_e_2p_histograms_Directory;
    string E_e_2p_QEL_CD_Dir = E_e_2p_QEL_histograms_Directory, E_e_2p_QEL_FD_Dir = E_e_2p_QEL_histograms_Directory;
    string E_e_2p_MEC_CD_Dir = E_e_2p_MEC_histograms_Directory, E_e_2p_MEC_FD_Dir = E_e_2p_MEC_histograms_Directory;
    string E_e_2p_RES_CD_Dir = E_e_2p_RES_histograms_Directory, E_e_2p_RES_FD_Dir = E_e_2p_RES_histograms_Directory;
    string E_e_2p_DIS_CD_Dir = E_e_2p_DIS_histograms_Directory, E_e_2p_DIS_FD_Dir = E_e_2p_DIS_histograms_Directory;
    //</editor-fold>

    //<editor-fold desc="E_e vs. Theta_e (2p, CD & FD)">
    TH2D *E_e_VS_Theta_e_2p_CD = new TH2D("E_{e} vs. #theta_{e} (All Int., CD)", "E_{e} vs. #theta_{e} (All Int., 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
                                          250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_QEL_CD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p, CD)", "E_{e} vs. #theta_{e} (QEL Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_MEC_CD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 2p, CD)", "E_{e} vs. #theta_{e} (MEC Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_RES_CD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 2p, CD)", "E_{e} vs. #theta_{e} (RES Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_DIS_CD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 2p, CD)", "E_{e} vs. #theta_{e} (DIS Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., FD)", "E_{e} vs. #theta_{e} (All Int., 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                          250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_QEL_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p, FD)", "E_{e} vs. #theta_{e} (QEL Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_MEC_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 2p, FD)", "E_{e} vs. #theta_{e} (MEC Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_RES_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 2p, FD)", "E_{e} vs. #theta_{e} (RES Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *E_e_VS_Theta_e_2p_DIS_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 2p, FD)", "E_{e} vs. #theta_{e} (DIS Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                              250, 0, 50, 250, 0, beamE * 1.1);
    string E_e_VS_Theta_e_2p_CD_Dir = E_e_VS_Theta_e_2p_histograms_Directory, E_e_VS_Theta_e_2p_FD_Dir = E_e_VS_Theta_e_2p_histograms_Directory;
    string E_e_VS_Theta_e_2p_QEL_CD_Dir = E_e_VS_Theta_e_2p_QEL_histograms_Directory, E_e_VS_Theta_e_2p_QEL_FD_Dir = E_e_VS_Theta_e_2p_QEL_histograms_Directory;
    string E_e_VS_Theta_e_2p_MEC_CD_Dir = E_e_VS_Theta_e_2p_MEC_histograms_Directory, E_e_VS_Theta_e_2p_MEC_FD_Dir = E_e_VS_Theta_e_2p_MEC_histograms_Directory;
    string E_e_VS_Theta_e_2p_RES_CD_Dir = E_e_VS_Theta_e_2p_RES_histograms_Directory, E_e_VS_Theta_e_2p_RES_FD_Dir = E_e_VS_Theta_e_2p_RES_histograms_Directory;
    string E_e_VS_Theta_e_2p_DIS_CD_Dir = E_e_VS_Theta_e_2p_DIS_histograms_Directory, E_e_VS_Theta_e_2p_DIS_FD_Dir = E_e_VS_Theta_e_2p_DIS_histograms_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Sampling Fraction (SF) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Sampling Fraction (SF) histograms (FD only)">

    //<editor-fold desc="SF histograms (1e2p, FD only)">
    THStack *SF_1e2p_Stack = new THStack("SF stack (All Int., 1e2p, FD)",
                                         "Sampling Fraction (f = #frac{E_{PCAL}+E_{IN}+E_{OUT}}{P_{e}}) Histogram (All Int., 1e2p, FD);f");

    TH1D *SF_All_Int_1e2p_BC_FD = new TH1D("SF BC (All Int., 1e2p, FD)", "Sampling Fraction f Before Cuts (All Int., 1e2p, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           200, 0.1, 0.35);
    TH1D *SF_All_Int_1e2p_AC_FD = new TH1D("SF AC (All Int., 1e2p, FD)", "Sampling Fraction f After Cuts (All Int., 1e2p, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           200, 0.1, 0.35);
    string SF_All_Int_1e2p_BC_FD_Dir = SF_plots_1e2p_BC_histograms_Directory, SF_All_Int_1e2p_AC_FD_Dir = SF_plots_1e2p_AC_histograms_Directory;
    //</editor-fold>

    //<editor-fold desc="SF vs. P histograms (1e2p, FD only)">
    TH2D *SF_VS_P_e_1e2p_BC_FD = new TH2D("SF vs. P_{e} BC (All Int., 1e2p, FD)",
                                          "Sampling Fraction f vs. P_{e} Before Cuts (All Int., 1e2p, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                          250, 0, beamE * 1.1, 250, 0.1, 0.35);
    TH2D *SF_VS_P_e_1e2p_AC_FD = new TH2D("SF vs. P_{e} AC (All Int., 1e2p, FD)",
                                          "Sampling Fraction f vs. P_{e} After Cuts (All Int., 1e2p, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                          250, 0, beamE * 1.1, 250, 0.1, 0.35);
    string SF_VS_P_e_1e2p_BC_FD_Dir = SF_VS_P_e_plots_1e2p_BC_histograms_Directory, SF_VS_P_e_1e2p_AC_FD_Dir = SF_VS_P_e_plots_1e2p_AC_histograms_Directory;
    //</editor-fold>

    //<editor-fold desc="SF histograms (2p, FD only)">
    THStack *SF_2p_Stack = new THStack("SF stack (All Int., 2p, FD)", "Sampling Fraction (f = #frac{E_{PCAL}+E_{IN}+E_{OUT}}{P}) Histogram (All Int., 2p, FD);f");

    TH1D *SF_All_Int_2p_FD = new TH1D("SF (All Int., 2p, FD)",
                                      "Sampling Fraction f (All Int., 2p, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 200, 0.1, 0.35);
    TH2D *SF_VS_P_e_2p_FD = new TH2D("SF vs. P_{e} (All Int., 2p, FD)",
                                     "Sampling Fraction f vs. P_{e} (All Int., 2p, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                     250, 0, beamE * 1.1, 250, 0.1, 0.35);
    string SF_All_Int_2p_FD_Dir = SF_plots_2p_cuts_histograms_Directory, SF_VS_P_e_2p_FD_Dir = SF_VS_P_e_plots_2p_cuts_histograms_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Electron fiducial histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Electron fiducial histograms (FD only)">

    //<editor-fold desc="Electron fiducial histograms (ECIN)">
    TH2D *Vcal_VS_EoP_1e2p_BC_ECIN = new TH2D("Vcal vs. EoP BC (All Int., 1e2p, ECIN)",
                                              "ECAL V coordinate vs. Sampling Fraction Before Cuts (All Int., 1e2p, ECIN);ECAL V coordinate [cm];Sampling Fraction",
                                              250, 0, 20, 250, 0.05, 0.35);
    TH2D *Vcal_VS_EoP_1e2p_AC_ECIN = new TH2D("Vcal vs. EoP AC (All Int., 1e2p, ECIN)",
                                              "ECAL V coordinate vs. Sampling Fraction After Cuts (All Int., 1e2p, ECIN);ECAL V coordinate [cm];Sampling Fraction",
                                              250, 0, 20, 250, 0.05, 0.35);
    string Vcal_VS_EoP_1e2p_BC_ECIN_Dir = fiducial_plots_1e_BC_ECIN_histograms_Directory, Vcal_VS_EoP_1e2p_AC_ECIN_Dir = fiducial_plots_1e_BC_ECIN_histograms_Directory;

    TH2D *Wcal_VS_EoP_1e2p_BC_ECIN = new TH2D("Wcal vs. EoP BC (All Int., 1e2p, ECIN)",
                                              "ECAL W coordinate vs. Sampling Fraction Before Cuts (All Int., 1e2p, ECIN);ECAL W coordinate [cm];Sampling Fraction",
                                              250, 0, 50, 250, 0.05, 0.35);
    TH2D *Wcal_VS_EoP_1e2p_AC_ECIN = new TH2D("Wcal vs. EoP AC (All Int., 1e2p, ECIN)",
                                              "ECAL W coordinate vs. Sampling Fraction After Cuts (All Int., 1e2p, ECIN);ECAL W coordinate [cm];Sampling Fraction",
                                              250, 0, 50, 250, 0.05, 0.35);
    string Wcal_VS_EoP_1e2p_BC_ECIN_Dir = fiducial_plots_1e_AC_ECIN_histograms_Directory, Wcal_VS_EoP_1e2p_AC_ECIN_Dir = fiducial_plots_1e_AC_ECIN_histograms_Directory;

    TH2D *Vcal_VS_EoP_2p_ECIN = new TH2D("Vcal vs. EoP (All Int., 2p, ECIN)",
                                         "ECAL V coordinate vs. Sampling Fraction (All Int., 2p, ECIN);ECAL V coordinate [cm];Sampling Fraction",
                                         250, 0, 50, 250, 0.05, 0.35);
    TH2D *Wcal_VS_EoP_2p_ECIN = new TH2D("Wcal vs. EoP (All Int., 2p, ECIN)",
                                         "ECAL W coordinate vs. Sampling Fraction (All Int., 2p, ECIN);ECAL W coordinate [cm];Sampling Fraction",
                                         250, 0, 50, 250, 0.05, 0.35);
    string Vcal_VS_EoP_2p_ECIN_Dir = fiducial_plots_2p_cuts_histograms_ECIN_Directory, Wcal_VS_EoP_2p_ECIN_Dir = fiducial_plots_2p_cuts_histograms_ECIN_Directory;
    //</editor-fold>

    //<editor-fold desc="Electron fiducial histograms (PCAL)">
    TH2D *Vcal_VS_EoP_1e_BC_PCAL = new TH2D("Vcal vs. EoP BC (All Int., 1e, PCAL)",
                                            "ECAL V coordinate vs. Sampling Fraction Before Cuts (All Int., 1e, PCAL);ECAL V coordinate [cm];Sampling Fraction",
                                            250, 0, 50, 250, 0.05, 0.35);
    TH2D *Vcal_VS_EoP_1e_AC_PCAL = new TH2D("Vcal vs. EoP AC (All Int., 1e, PCAL)",
                                            "ECAL V coordinate vs. Sampling Fraction After Cuts (All Int., 1e, PCAL);ECAL V coordinate [cm];Sampling Fraction",
                                            250, 0, 50, 250, 0.05, 0.35);
    string Vcal_VS_EoP_1e_BC_PCAL_Dir = fiducial_plots_1e_BC_PCAL_histograms_Directory, Vcal_VS_EoP_1e_AC_PCAL_Dir = fiducial_plots_1e_AC_PCAL_histograms_Directory;

    TH2D *Wcal_VS_EoP_1e_BC_PCAL = new TH2D("Wcal vs. EoP BC (All Int., 1e, PCAL)",
                                            "ECAL W coordinate vs. Sampling Fraction Before Cuts (All Int., 1e, PCAL);ECAL W coordinate [cm];Sampling Fraction",
                                            250, 0, 50, 250, 0.05, 0.35);
    TH2D *Wcal_VS_EoP_1e_AC_PCAL = new TH2D("Wcal vs. EoP AC (All Int., 1e, PCAL)",
                                            "ECAL W coordinate vs. Sampling Fraction After Cuts (All Int., 1e, PCAL);ECAL W coordinate [cm];Sampling Fraction",
                                            250, 0, 50, 250, 0.05, 0.35);
    string Wcal_VS_EoP_1e_BC_PCAL_Dir = fiducial_plots_1e_BC_PCAL_histograms_Directory, Wcal_VS_EoP_1e_AC_PCAL_Dir = fiducial_plots_1e_AC_PCAL_histograms_Directory;

    TH2D *Vcal_VS_EoP_2p_PCAL = new TH2D("Vcal vs. EoP (All Int., 2p, PCAL)",
                                         "ECAL V coordinate vs. Sampling Fraction (All Int., 2p, PCAL);ECAL V coordinate [cm];Sampling Fraction",
                                         250, 0, 50, 250, 0.05, 0.35);
    TH2D *Wcal_VS_EoP_2p_PCAL = new TH2D("Wcal vs. EoP (All Int., 2p, PCAL)",
                                         "ECAL W coordinate vs. Sampling Fraction (All Int., 2p, PCAL);ECAL W coordinate [cm];Sampling Fraction",
                                         250, 0, 50, 250, 0.05, 0.35);
    string Vcal_VS_EoP_2p_PCAL_Dir = fiducial_plots_2p_cuts_histograms_PCAL_Directory, Wcal_VS_EoP_2p_PCAL_Dir = fiducial_plots_2p_cuts_histograms_PCAL_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Number of Photo-electrons (nphe) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="nphe histograms (FD only)">

    //<editor-fold desc="Number of Photo-electrons (nphe) histograms (1e2p, FD only)">
    THStack *nphe_1e2p_Stack = new THStack("n_{phe} stack (All Int., 1e2p, FD)", "nphe Histogram (All Int., 1e2p, FD);n_{phe}");

    TH1D *nphe_All_Int_1e2p_BC_FD = new TH1D("n_{phe} BC (All Int., 1e2p, FD)", "#Photo-electrons in HTCC n_{phe} Before Cuts (All Int., 1e2p, FD);n_{phe}", 200, 0, 40);
    TH1D *nphe_All_Int_1e2p_AC_FD = new TH1D("n_{phe} AC (All Int., 1e2p, FD)", "#Photo-electrons in HTCC (n_{phe}) After Cuts (All Int., 1e2p, FD);n_{phe}", 200, 0, 40);
    string nphe_All_Int_1e2p_BC_FD_Dir = nphe_plots_1e2p_BC_histograms_Directory, nphe_All_Int_1e2p_AC_FD_Dir = nphe_plots_1e2p_AC_histograms_Directory;
    //</editor-fold>

    //<editor-fold desc="Number of Photo-electrons (nphe) histograms (2p, FD only)">
    THStack *nphe_2p_Stack = new THStack("n_{phe} stack (All Int., 2p, FD)", "#Photo-electrons in HTCC (n_{phe} ) Histogram (All Int., 2p, FD);n_{phe}");

    TH1D *nphe_All_Int_2p_FD = new TH1D("n_{phe} (All Int., 2p, FD)", "#Photo-electrons in HTCC (n_{phe} ) (All Int., 2p, FD);n_{phe}", 200, 0, 40);
    string nphe_All_Int_2p_FD_Dir = nphe_plots_2p_cuts_histograms_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Energy Transfer (ET) histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Energy Transfer (ET) histograms">
    THStack *ETrans_all_int_15_Stack_2p_CD = new THStack("ET around 15#circ Stack (All Int., 2p, CD)",
                                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (All Int., 2p, CD);E_{l}-E_{e} [GeV]");
    THStack *ETrans_all_int_15_Stack_2p_FD = new THStack("ET around 15#circ Stack (All Int., 2p, FD)",
                                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (All Int., 2p, FD);E_{l}-E_{e} [GeV]");
    string ETrans_all_int_15_Stack_2p_CD_Dir = ETrans_15_stack_Directory, ETrans_all_int_15_Stack_2p_FD_Dir = ETrans_15_stack_Directory;

    TH1D *ETrans_15_All_Int_2p_CD = new TH1D("ET around 15#circ (All Int., 2p, CD)",
                                             "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (All Int., 2p, CD);E_{l}-E_{e} [GeV]",
                                             100, 0, beamE * 1.1);
    TH1D *ETrans_15_QEL_2p_CD = new TH1D("ET around 15#circ (QEL Only, 2p, CD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (QEL Only, 2p, CD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_MEC_2p_CD = new TH1D("ET around 15#circ (MEC Only, 2p, CD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (MEC Only, 2p, CD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_RES_2p_CD = new TH1D("ET around 15#circ (RES Only, 2p, CD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (RES Only, 2p, CD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_DIS_2p_CD = new TH1D("ET around 15#circ (DIS Only, 2p, CD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (DIS Only, 2p, CD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_All_Int_2p_FD = new TH1D("ET around 15#circ (All Int., 2p, FD)",
                                             "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (All Int., 2p, FD);E_{l}-E_{e} [GeV]",
                                             100, 0, beamE * 1.1);
    TH1D *ETrans_15_QEL_2p_FD = new TH1D("ET around 15#circ (QEL Only, 2p, FD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (QEL Only, 2p, FD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_MEC_2p_FD = new TH1D("ET around 15#circ (MEC Only, 2p, FD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (MEC Only, 2p, FD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_RES_2p_FD = new TH1D("ET around 15#circ (RES Only, 2p, FD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (RES Only, 2p, FD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *ETrans_15_DIS_2p_FD = new TH1D("ET around 15#circ (DIS Only, 2p, FD)",
                                         "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{l} #leq 16#circ (DIS Only, 2p, FD);E_{l}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    string ETrans_15_All_Int_2p_CD_Dir = ETrans_All_Int_15_histograms_Directory, ETrans_15_All_Int_2p_FD_Dir = ETrans_All_Int_15_histograms_Directory;
    string ETrans_15_QEL_2p_CD_Dir = ETrans_QEL_15_histograms_Directory, ETrans_15_QEL_2p_FD_Dir = ETrans_QEL_15_histograms_Directory;
    string ETrans_15_MEC_2p_CD_Dir = ETrans_MEC_15_histograms_Directory, ETrans_15_MEC_2p_FD_Dir = ETrans_MEC_15_histograms_Directory;
    string ETrans_15_RES_2p_CD_Dir = ETrans_RES_15_histograms_Directory, ETrans_15_RES_2p_FD_Dir = ETrans_RES_15_histograms_Directory;
    string ETrans_15_DIS_2p_CD_Dir = ETrans_DIS_15_histograms_Directory, ETrans_15_DIS_2p_FD_Dir = ETrans_DIS_15_histograms_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Ecal restoration histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Ecal restoration histograms">
    //TODO: confirm with Adi if Ecal should be separated to CD and FD or not
    THStack *Ecal_Stack_2p = new THStack("E_{cal} Reconstruction (2p)", "E_{cal} Reconstruction (2p);E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]");
    string Ecal_Stack_2p_Dir = Ecal_stack_Directory, Ecal_Stack_2p_FD_Dir = Ecal_stack_Directory;

    TH1D *Ecal_All_Int_2p = new TH1D("E_{cal} (All Int., 2p, CD)", ";E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]", 100, 0, beamE * 1.35);
    TH1D *Ecal_QEL_2p = new TH1D("E_{cal} (QEL only, 2p, CD)", ";E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]", 100, 0, beamE * 1.35);
    TH1D *Ecal_MEC_2p = new TH1D("E_{cal} (MEC only, 2p, CD)", ";E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]", 100, 0, beamE * 1.35);
    TH1D *Ecal_RES_2p = new TH1D("E_{cal} (RES only, 2p, CD)", ";E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]", 100, 0, beamE * 1.35);
    TH1D *Ecal_DIS_2p = new TH1D("E_{cal} (DIS only, 2p, CD)", ";E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]", 100, 0, beamE * 1.35);
    string Ecal_All_Int_2p_Dir = Ecal_All_Int_histograms_Directory;
    string Ecal_QEL_2p_Dir = Ecal_QEL_histograms_Directory, Ecal_MEC_2p_Dir = Ecal_MEC_histograms_Directory;
    string Ecal_RES_2p_Dir = Ecal_RES_histograms_Directory, Ecal_DIS_2p_Dir = Ecal_DIS_histograms_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MicroBooNE histogram reconstruction
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="MicroBooNE histogram reconstruction">

    //TODO: finish setting up these histograms
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

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                         Code execution                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Code execution">
    cout << "Reading target parameter files\n\n";

    clas12ana clasAna;

    /* Read in target parameter files */
    clasAna.readInputParam((CutsDirectory + "ana.par").c_str());
    clasAna.readEcalPar((CutsDirectory + "ecal.par").c_str());
    clasAna.printParams();

    cout << "\n\nLooping over chain files...\n\n";

    clas12root::HipoChain chain;
    chain.Add(AnalyseFile.c_str());
    chain.SetReaderTags({0});                   //TODO: check with Justin what are these tags
    auto config_c12 = chain.GetC12Reader();     //TODO: check with Justin what is this used for
    auto &c12 = chain.C12ref();                 //TODO: check with Justin what is this used for

    auto db=TDatabasePDG::Instance();
    chain.db()->turnOffQADB();                  //TODO: check with Justin what is this used for

    /* Setting cuts */
//    clasAna.setEcalSFCuts();                    // making f_ecalSFCuts = ture
//    clasAna.setEcalEdgeCuts();                  // making f_ecalEdgeCuts = ture
//    clasAna.setPidCuts();                       // making f_pidCuts = ture
//    clasAna.setVertexCuts();                    // making f_vertexCuts = ture
//    clasAna.setVertexCorrCuts();                // making f_corr_vertexCuts = ture
//    clasAna.setDCEdgeCuts();                    // making f_DCEdgeCuts = ture (edge cuts = fiducial cuts)
//
////    clasAna.setVzcuts(-3, 3);         // setting Vz cuts?
//    clasAna.setVzcuts(-6, 1);         // setting Vz cuts?
////    clasAna.setVertexCorrCuts(-3, 3); // setting dVz cuts?
//    clasAna.setVertexCorrCuts(-3, 1); // setting dVz cuts?

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Setting beam particle's momentum
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Setting beam particle's momentum">
    double Pv = sqrt(beamE * beamE - m_e * m_e), Pvx = 0., Pvy = 0., Pvz = Pv; // Assuming momentum of incoming lepton is in the z direction

    TLorentzVector beam(0, 0, sqrt(beamE * beamE - m_e * m_e), beamE);
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Counting variable definitions
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Counting variable definitions">
    int num_of_events = 0, num_of_events_without_any_e = 0, num_of_events_with_any_e = 0;
    int num_of_QEL_events = 0, num_of_MEC_events = 0, num_of_RES_events = 0, num_of_DIS_events = 0;

//    int num_of_events_with_e_in_CD = 0, num_of_events_with_e_in_FD = 0;

    int num_of_events_with_at_least_1e = 0, num_of_events_with_exactly_1e = 0, num_of_events_more_then_1e = 0;

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
    int num_of_2p_QEL_events = 0, num_of_2p_MEC_events = 0, num_of_2p_RES_events = 0, num_of_2p_DIS_events = 0;

    int num_of_MicroBooNE_events_BC = 0;
    int num_of_MicroBooNE_events_BC_wNeutrons = 0, num_of_MicroBooNE_events_BC_wpi0 = 0, num_of_MicroBooNE_events_BC_wpip = 0, num_of_MicroBooNE_events_BC_wpim = 0;
    int num_of_MicroBooNE_events_AC = 0;
    int num_of_MicroBooNE_events_AC_wNeutrons = 0, num_of_MicroBooNE_events_AC_wpi0 = 0, num_of_MicroBooNE_events_AC_wpip = 0, num_of_MicroBooNE_events_AC_wpim = 0;
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Looping over each HipoChain file
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    while (chain.Next()) { // loop over events
        ++num_of_events; // logging Total #(events)

//        //TODO: see what this command gives:
//        if (c12->getDetParticles().empty()) { continue; }

        clasAna.Run(c12);

        /* All of these particles are with clas12ana cuts
           Only cuts missing are nphe and momentum cuts - to be applied later */


//        auto neutrons = clasAna.getByPid(2112);  // Neutrons
        auto protons = clasAna.getByPid(2212);   // Protons
        auto Kplus = clasAna.getByPid(321);      // K+
        auto Kminus = clasAna.getByPid(-321);    // K-
        auto piplus = clasAna.getByPid(211);     // pi+
        auto piminus = clasAna.getByPid(-211);   // pi-
//        auto pizero = clasAna.getByPid(111);     // pi0
        auto electrons = clasAna.getByPid(11);   // Electrons

        auto deuterons = clasAna.getByPid(45);   // Deuterons
        auto neutrals = clasAna.getByPid(0);     // Neutrons
        auto otherpart = clasAna.getByPid(311);  // Other particles

        /* Number of specific particles in event */
        int Np = protons.size(), Nkp = Kplus.size(), Nkm = Kminus.size(), Npip = piplus.size(), Npim = piminus.size(), Ne = electrons.size();
        int Nd = deuterons.size(), Nn = neutrals.size(), No = otherpart.size();

        /* Total number of particles in event (= Nf) */
        int Nf = Np + Nkp + Nkm + Npip + Npim + Ne + Nd + Nn + No;
//        int Nf = neutrons.size() + protons.size() + Kplus.size() + Kminus.size() + piplus.size() + piminus.size() + pizero.size() + electrons.size() +
//                   deuterons.size() + neutrals.size() + otherpart.size();


//        auto AllParticles = c12->getDetParticles();
//        auto protons = c12->getByID(2212);   // Protons
//        auto Kplus = c12->getByID(321);      // K+
//        auto Kminus = c12->getByID(-321);    // K-
//        auto piplus = c12->getByID(211);     // pi+
//        auto piminus = c12->getByID(-211);   // pi-
//        auto electrons = c12->getByID(11);   // Electrons
////        auto deuterons = clasAna.getByPid(45);   // Deuterons
////        auto neutrals = clasAna.getByPid(0);     // Neutrons
////        auto otherpart = clasAna.getByPid(311);  // Other particles
//
//        /* Number of specific particles in event */
//        int Np = protons.size(), Nkp = Kplus.size(), Nkm = Kminus.size(), Npip = piplus.size(), Npim = piminus.size(), Ne = electrons.size();
//
//        /* Total number of particles in event (= Nf) */
//        int Nf = AllParticles.size();


        bool qel = false, mec = false, res = false, dis = false;
        double processID = c12->mcevent()->getWeight(); // code = 1,2,3,4 = type = qel, mec, res, dis

        //<editor-fold desc="Log total #(events) with and without electrons">
        if (electrons.size() == 0) {
            ++num_of_events_without_any_e; // logging Total #(events) w/o any e
        } else {
            ++num_of_events_with_any_e; // logging Total #(events) w/ any e
        }
        //</editor-fold>

        //<editor-fold desc="Identify event process ID">
        if (processID == 1) {
            ++num_of_QEL_events;
            qel = true;
        } else if (processID == 2) {
            ++num_of_MEC_events;
            mec = true;
        } else if (processID == 3) {
            ++num_of_RES_events;
            res = true;
        } else if (processID == 4) {
            ++num_of_DIS_events;
            dis = true;
        }
        //</editor-fold>

//  All particles plots -------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="All particles plots">

        //<editor-fold desc="Electron plots (no #(electron) cut, CD & FD)">
        TLorentzVector e_out_CD, Q_CD, e_out_FD, Q_FD;
        double theta_e_CD, phi_e_CD, Ee_CD, Pe_CD, Pex_CD, Pey_CD, Pez_CD, omega_CD, q_CD, qx_CD, qy_CD, qz_CD, Q2_CD;
        double theta_e_FD, phi_e_FD, Ee_FD, Pe_FD, Pex_FD, Pey_FD, Pez_FD, omega_FD, q_FD, qx_FD, qy_FD, qz_FD, Q2_FD;

        for (int i = 0; i < electrons.size(); i++) {
            if (electrons[i]->getRegion() == CD) {
//                ++num_of_events_with_e_in_CD; // logging #e in CD

                Beta_vs_P_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_Electrons_Only_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());

                if (electrons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                } else if (electrons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                } else if (electrons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                }

                Chi2_Electron_CD->Fill(electrons[i]->par()->getChi2Pid());

                Vertex_Electron_Vx_CD_test.Fill(electrons[i]->par()->getVx());
                Vertex_Electron_Vy_CD_test.Fill(electrons[i]->par()->getVy());
                Vertex_Electron_Vz_CD_test.Fill(electrons[i]->par()->getVz());

                theta_e_CD = electrons[i]->getTheta() * 180.0 / pi; // theta_e_CD in deg
                Theta_e_CD->Fill(theta_e_CD);
                phi_e_CD = electrons[i]->getPhi() * 180.0 / pi; // phi_e_CD in deg
                Phi_e_CD->Fill(phi_e_CD);
                Theta_e_VS_Phi_e_CD->Fill(phi_e_CD, theta_e_CD);

                Pe_CD = electrons[i]->par()->getP();
                e_out_CD.SetPxPyPzE(electrons[i]->par()->getPx(), electrons[i]->par()->getPy(), electrons[i]->par()->getPz(), sqrt(m_e * m_e + Pe_CD * Pe_CD));

                Q_CD = beam - e_out_CD;
                Q2_CD = fabs(Q_CD.Mag2());
                Q2_histogram_CD->Fill(Q2_CD);

                if (electrons.size() == 1) {
                    Q2_histogram_1e_CD->Fill(Q2_CD);
                }

                if (electrons.size() == 1 && Nf == 3) {
                    Q2_histogram_1e2X_CD->Fill(Q2_CD);

                    if (protons.size() == 2) {
                        Q2_histogram_1e2p_CD->Fill(Q2_CD);
                    }
                }
            } else if (electrons[i]->getRegion() == FD) {
//                ++num_of_events_with_e_in_FD; // logging #e in FD

                Beta_vs_P_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_Electrons_Only_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());

                if (electrons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                } else if (electrons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                } else if (electrons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                }

                Chi2_Electron_FD->Fill(electrons[i]->par()->getChi2Pid());

                Vertex_Electron_Vx_FD_test.Fill(electrons[i]->par()->getVx());
                Vertex_Electron_Vy_FD_test.Fill(electrons[i]->par()->getVy());
                Vertex_Electron_Vz_FD_test.Fill(electrons[i]->par()->getVz());

                theta_e_FD = electrons[i]->getTheta() * 180.0 / pi; // theta_e_FD in deg
                Theta_e_FD->Fill(theta_e_FD);
                phi_e_FD = electrons[i]->getPhi() * 180.0 / pi; // phi_e_FD in deg
                Phi_e_FD->Fill(phi_e_FD);
                Theta_e_VS_Phi_e_FD->Fill(phi_e_FD, theta_e_FD);

                Pe_FD = electrons[i]->par()->getP();
                e_out_FD.SetPxPyPzE(electrons[i]->par()->getPx(), electrons[i]->par()->getPy(), electrons[i]->par()->getPz(), sqrt(m_e * m_e + Pe_FD * Pe_FD));

                Q_FD = beam - e_out_FD;
                Q2_FD = fabs(Q_FD.Mag2());
                Q2_histogram_FD->Fill(Q2_FD);

                if (electrons.size() == 1) {
                    Q2_histogram_1e_FD->Fill(Q2_FD);
                }

                if (electrons.size() == 1 && Nf == 3) {
                    Q2_histogram_1e2X_FD->Fill(Q2_FD);

                    if (protons.size() == 2) {
                        Q2_histogram_1e2p_FD->Fill(Q2_FD);
                    }
                }
            }
        } // end of loop over electrons vector
        //</editor-fold>

        //<editor-fold desc="Proton plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < protons.size(); i++) {
            if (protons[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_Protons_Only_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());

                if (protons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                } else if (protons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                } else if (protons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                }

                Chi2_Proton_CD->Fill(protons[i]->par()->getChi2Pid());

                Vertex_Proton_Vx_CD_test.Fill(protons[i]->par()->getVx());
                Vertex_Proton_Vy_CD_test.Fill(protons[i]->par()->getVy());
                Vertex_Proton_Vz_CD_test.Fill(protons[i]->par()->getVz());
            } else if (protons[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_Protons_Only_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());

                if (protons[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                } else if (protons[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                } else if (protons[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                }

                Chi2_Proton_FD->Fill(protons[i]->par()->getChi2Pid());

                Vertex_Proton_Vx_FD_test.Fill(protons[i]->par()->getVx());
                Vertex_Proton_Vy_FD_test.Fill(protons[i]->par()->getVy());
                Vertex_Proton_Vz_FD_test.Fill(protons[i]->par()->getVz());
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
                Beta_vs_P_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

                if (Kplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                }
            } else if (Kplus[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

                if (Kplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());

                if (Kminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                }
            } else if (Kminus[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());

                if (Kminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                }
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                if (piplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                }
            } else if (piplus[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                if (piplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                }
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                Beta_vs_P_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                if (piminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                }
            } else if (piminus[i]->getRegion() == FD) {
                Beta_vs_P_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                if (piminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_All_e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_All_e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_All_e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                }
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

//        //<editor-fold desc="neutrals Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < neutrals.size(); i++) {
//            if (neutrals[i]->getRegion() == CD) {
//                Beta_vs_P_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//
//                if (neutrals[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                }
//            } else if (neutrals[i]->getRegion() == FD) {
//                Beta_vs_P_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//
//                if (neutrals[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over neutrals vector
//        //</editor-fold>
//
//        //<editor-fold desc="deuterons Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < deuterons.size(); i++) {
//            if (deuterons[i]->getRegion() == CD) {
//                Beta_vs_P_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//
//                if (deuterons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                }
//            } else if (deuterons[i]->getRegion() == FD) {
//                Beta_vs_P_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//
//                if (deuterons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over deuterons vector
//        //</editor-fold>
//
//        //<editor-fold desc="otherpart Beta vs. P plots (no #(electron) cut, CD & FD)">
//        for (int i = 0; i < otherpart.size(); i++) {
//            if (otherpart[i]->getRegion() == CD) {
//                Beta_vs_P_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//
//                if (otherpart[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                }
//            } else if (otherpart[i]->getRegion() == FD) {
//                Beta_vs_P_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//
//                if (otherpart[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_All_e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_All_e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_All_e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over otherpart vector
//        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

//  At least 1e cut (log only) ------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="At least 1e cut (log only)">
        if (electrons.size() >= 1) {
            ++num_of_events_with_at_least_1e; // logging #(events) w/ at least 1e

            if (electrons.size() > 1) {
                ++num_of_events_more_then_1e; // logging #(events) w/ more then 1e
            }
        }
        //</editor-fold>

//  1e cut --------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e cut">
        /* Here we apply electron cut on everything that follows (MicroBooNE, 1e2X, 1e2p, etc) */

        //<editor-fold desc="rough 1e cut">
        /* Applying rough 1e cut */
        if (Ne != 1) { continue; } /* the rough 1e cut */
        ++num_of_events_with_exactly_1e; /* logging #(events) w/ exactly 1e */

        if (Nf - Ne == Np) {
            ++num_of_events_with_1enP; /* logging #(events) w/ 1e & any #p 1enP */

            if (protons.size() == 1) { ++num_of_events_with_1e1p; /* logging #(events) w/ 1e1p */ }
        }
        //</editor-fold>

        //<editor-fold desc="Testing additional electron cuts">

        //TODO: all these tests are NOT 2p or 1e2p (the're moved from below and here we have no constrant on protons.size) - redefine propely to 1e or add constrant on protons.size

        TVector3 P_e_1e;
        P_e_1e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());

        //<editor-fold desc="Testing momentum cuts (2p)">
        /* Momentum plots before cuts */
        if (electrons[0]->getRegion() == CD) {
            P_e_1e2p_BC_CD->Fill(P_e_1e.Mag());
        } else if (electrons[0]->getRegion() == FD) {
            P_e_1e2p_BC_FD->Fill(P_e_1e.Mag());
        }

        /* Momentum plots after cuts */
        if ((e_momentum_upper_cut_2p == -1) && (e_momentum_lower_cut_2p == -1)) {
            if (electrons[0]->getRegion() == CD) {
                P_e_1e2p_AC_CD->Fill(P_e_1e.Mag());
            } else if (electrons[0]->getRegion() == FD) {
                P_e_1e2p_AC_FD->Fill(P_e_1e.Mag());
            }
        } else if ((e_momentum_upper_cut_2p != -1) && (e_momentum_lower_cut_2p == -1)) {
            if (P_e_1e.Mag() <= e_momentum_upper_cut_2p) {
                if (electrons[0]->getRegion() == CD) {
                    P_e_1e2p_AC_CD->Fill(P_e_1e.Mag());
                } else if (electrons[0]->getRegion() == FD) {
                    P_e_1e2p_AC_FD->Fill(P_e_1e.Mag());
                }
            }
        } else if ((e_momentum_upper_cut_2p == -1) && (e_momentum_lower_cut_2p != -1)) {
            if (P_e_1e.Mag() >= e_momentum_lower_cut_2p) {
                if (electrons[0]->getRegion() == CD) {
                    P_e_1e2p_AC_CD->Fill(P_e_1e.Mag());
                } else if (electrons[0]->getRegion() == FD) {
                    P_e_1e2p_AC_FD->Fill(P_e_1e.Mag());
                }
            }
        } else if ((e_momentum_upper_cut_2p != -1) && (e_momentum_lower_cut_2p != -1)) {
            if ((P_e_1e.Mag() >= e_momentum_lower_cut_2p) && (P_e_1e.Mag() <= e_momentum_upper_cut_2p)) {
                if (electrons[0]->getRegion() == CD) {
                    P_e_1e2p_AC_CD->Fill(P_e_1e.Mag());
                } else if (electrons[0]->getRegion() == FD) {
                    P_e_1e2p_AC_FD->Fill(P_e_1e.Mag());
                }
            }
        }
        //</editor-fold>

        //<editor-fold desc="Testing SF cuts">
        double EoP_e = (electrons[0]->cal(PCAL)->getEnergy() + electrons[0]->cal(ECIN)->getEnergy() + electrons[0]->cal(ECOUT)->getEnergy()) / P_e_1e.Mag();

        /* SF plots before cuts */
        SF_All_Int_1e2p_BC_FD->Fill(EoP_e);
        SF_VS_P_e_1e2p_BC_FD->Fill(P_e_1e.Mag(), EoP_e);

        /* SF plots after cuts */
        if ((EoP_e >= SF_1e2p_lower_cut) && (EoP_e <= SF_1e2p_upper_cut)) {
            SF_All_Int_1e2p_AC_FD->Fill(EoP_e);
            SF_VS_P_e_1e2p_AC_FD->Fill(P_e_1e.Mag(), EoP_e);
        }
        //</editor-fold>

        //<editor-fold desc="Testing fiducial cuts">
        /* Fiducial plots before cuts */
//            Vcal_VS_EoP_1e2p_BC_ECIN->Fill(electrons[0]->cal(ECIN)->getLv(), EoP_e);
//            Wcal_VS_EoP_1e2p_BC_ECIN->Fill(electrons[0]->cal(ECIN)->getLw(), EoP_e);
        Vcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
        Wcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);

        /* Fiducial plots after cuts */
        //TODO: remove fiducial ECIN plots
//                if (electrons[0]->cal(ECIN)->getLv() >= fiducial_cut_Lv) { Vcal_VS_EoP_1e2p_AC_ECIN->Fill(electrons[0]->cal(ECIN)->getLv(), EoP_e); }
//                if (electrons[0]->cal(ECIN)->getLw() >= fiducial_cut_Lw) { Wcal_VS_EoP_1e2p_AC_ECIN->Fill(electrons[0]->cal(ECIN)->getLw(), EoP_e); }
        if (electrons[0]->cal(PCAL)->getLv() >= fiducial_cut_Lv) { Vcal_VS_EoP_1e_AC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e); }
        if (electrons[0]->cal(PCAL)->getLw() >= fiducial_cut_Lw) { Wcal_VS_EoP_1e_AC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e); }
        //</editor-fold>

        //<editor-fold desc="Testing nphe cuts">
        int nphe = electrons[0]->che(HTCC)->getNphe();

        /* nphe plots before cuts */
        nphe_All_Int_1e2p_BC_FD->Fill(nphe);

        /* nphe plots after cuts */
        if (nphe >= nphe_lower_cut) { nphe_All_Int_1e2p_AC_FD->Fill(nphe); }
        //</editor-fold>

        //<editor-fold desc="Testing chi2 cuts">
        /* Chi2 plots before cut */
        if (electrons[0]->getRegion() == CD) {
            Chi2_Electron_1e2p_BC_CD->Fill(electrons[0]->par()->getChi2Pid());
        } else if (electrons[0]->getRegion() == FD) {
            Chi2_Electron_1e2p_BC_FD->Fill(electrons[0]->par()->getChi2Pid());
        }

        /* Chi2 plots after cut */
        if ((electrons[0]->getRegion() == CD) && (fabs(Chi2_Electron_1e_peak_CD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_CD)) {
            ++num_of_events_1e2p_w_eChi2_cut_only_CD;
            Chi2_Electron_1e2p_AC_CD->Fill(electrons[0]->par()->getChi2Pid());
        } else if ((electrons[0]->getRegion() == FD) && (fabs(Chi2_Electron_1e_peak_FD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_FD)) {
            ++num_of_events_1e2p_w_eChi2_cut_only_FD;
            Chi2_Electron_1e2p_AC_FD->Fill(electrons[0]->par()->getChi2Pid());
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Applying additional electron cuts">
//        /* SF cuts */
//        if ((apply_SF_cuts == true) && ((EoP_e < SF_1e2p_lower_cut) || (EoP_e > SF_1e2p_upper_cut))) { continue; }

//        /* fiducial cuts */
//        if ((apply_Lv_cut == true) && (electrons[0]->cal(PCAL)->getLv() < fiducial_cut_Lv)) { continue; }
//        if ((apply_Lw_cut == true) && (electrons[0]->cal(PCAL)->getLw() < fiducial_cut_Lw)) { continue; }

//        /* nphe cuts */
//        if ((apply_nphe_cut == true) && (nphe < nphe_lower_cut)) { continue; }

//        /* Chi2 cuts */
//        if (electrons[0]->getRegion() == CD) {
//            if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Electron_1e_peak_CD - electrons[0]->par()->getChi2Pid()) > Chi2_Electron_cut_CD)) { continue; }
//        } else if (electrons[0]->getRegion() == FD) {
//            if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Electron_1e_peak_FD - electrons[0]->par()->getChi2Pid()) > Chi2_Electron_cut_FD)) { continue; }
//        }

////            //TODO: see if other cuts can be applied to improve beta vs. P plots
////            if (electrons[0]->cal(PCAL)->getEnergy() < 0.07) { continue; }
//////            if(electrons[0]->par()->getVz() < -1){continue;}
//////            if(electrons[0]->par()->getVz() > 1){continue;}
////
////            //TODO: see if other cuts can be applied to improve beta vs. P plots
////            double vtz_e = electrons[0]->par()->getVz();
////            bool vt_cut_skip = false;
////
////            for (int i = 0; i < AllParticles.size(); i++) {
////                double vtz_part = AllParticles[i]->par()->getVz(), dvtz = vtz_e - vtz_part;
////
////                if (fabs(0 - dvtz) > 3.) { vt_cut_skip = true; }
//////                if (fabs(dVz_peak - dvtz) > dVz_cut) { vt_cut_skip = true; }
//////                if ((fabs(dVz_peak - dvtz) > dVz_cut) || (fabs(dVz_peak - dvtz) > dVz_cut)) { continue; }
////            }
////
////            if (vt_cut_skip == true) { continue; }
//
//        //</editor-fold>

        //<editor-fold desc="Filling 1e only plots">

        //<editor-fold desc="Fill Electron plots (1e only, CD & FD)">
        double theta_e_1e_CD, phi_e_1e_CD, P_e_CD, E_e_CD, e_Chi2_CD, e_Vx_CD, e_Vy_CD, e_Vz_CD;
        double theta_e_1e_FD, phi_e_1e_FD, P_e_FD, E_e_FD, e_Chi2_FD, e_Vx_FD, e_Vy_FD, e_Vz_FD;

        for (auto &e: electrons) {
            if (e->getRegion() == CD) {
                P_e_CD = e->getP();
                E_e_CD = sqrt(m_e * m_e + P_e_CD * P_e_CD);
                E_e_hist_CD->Fill(E_e_CD);

                Beta_vs_P_1e_CD->Fill(P_e_CD, e->par()->getBeta());
                Beta_vs_P_1e_Electrons_Only_CD->Fill(P_e_CD, e->par()->getBeta());

                if (e->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_CD->Fill(P_e_CD, e->par()->getBeta());
                } else if (e->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_CD->Fill(P_e_CD, e->par()->getBeta());
                } else if (e->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_CD->Fill(P_e_CD, e->par()->getBeta());
                }

                theta_e_1e_CD = e->getTheta() * 180.0 / pi; // theta_e_1e_CD in deg
                Theta_e_1e_CD->Fill(theta_e_1e_CD);
                phi_e_1e_CD = e->getPhi() * 180.0 / pi; // phi_e_1e_CD in deg
                Phi_e_1e_CD->Fill(phi_e_1e_CD);
                Theta_e_VS_Phi_e_1e_CD->Fill(phi_e_1e_CD, theta_e_1e_CD);
                E_e_VS_Theta_e_hist_CD->Fill(theta_e_1e_CD, E_e_CD);

                if (Nf == 3) {
                    Theta_e_1e2X_CD->Fill(theta_e_1e_CD);
                    Phi_e_1e2X_CD->Fill(phi_e_1e_CD);

                    if (qel) {
                        ++num_of_QEL_1e2X_CD_events;
                        Theta_e_1e2X_QEL_CD->Fill(theta_e_1e_CD);
                        Phi_e_1e2X_QEL_CD->Fill(phi_e_1e_CD);
                    } else if (mec) {
                        ++num_of_MEC_1e2X_CD_events;
                        Theta_e_1e2X_MEC_CD->Fill(theta_e_1e_CD);
                        Phi_e_1e2X_MEC_CD->Fill(phi_e_1e_CD);
                    } else if (res) {
                        ++num_of_RES_1e2X_CD_events;
                        Theta_e_1e2X_RES_CD->Fill(theta_e_1e_CD);
                        Phi_e_1e2X_RES_CD->Fill(phi_e_1e_CD);
                    } else if (dis) {
                        ++num_of_DIS_1e2X_CD_events;
                        Theta_e_1e2X_DIS_CD->Fill(theta_e_1e_CD);
                        Phi_e_1e2X_DIS_CD->Fill(phi_e_1e_CD);
                    }

                    if (protons.size() == 2) {
                        Theta_e_1e2p_CD->Fill(theta_e_1e_CD);
                        Phi_e_1e2p_CD->Fill(phi_e_1e_CD);

                        if (qel) {
                            ++num_of_QEL_1e2p_CD_events;
                            Theta_e_1e2p_QEL_CD->Fill(theta_e_1e_CD);
                            Phi_e_1e2p_QEL_CD->Fill(phi_e_1e_CD);
                        } else if (mec) {
                            ++num_of_MEC_1e2p_CD_events;
                            Theta_e_1e2p_MEC_CD->Fill(theta_e_1e_CD);
                            Phi_e_1e2p_MEC_CD->Fill(phi_e_1e_CD);
                        } else if (res) {
                            ++num_of_RES_1e2p_CD_events;
                            Theta_e_1e2p_RES_CD->Fill(theta_e_1e_CD);
                            Phi_e_1e2p_RES_CD->Fill(phi_e_1e_CD);
                        } else if (dis) {
                            ++num_of_DIS_1e2p_CD_events;
                            Theta_e_1e2p_DIS_CD->Fill(theta_e_1e_CD);
                            Phi_e_1e2p_DIS_CD->Fill(phi_e_1e_CD);
                        }
                    }
                }

                e_Chi2_CD = e->par()->getChi2Pid();
                Chi2_Electron_1e_CD->Fill(e_Chi2_CD);

                e_Vx_CD = e->par()->getVx();
                Vertex_Electron_1e_Vx_CD_test.Fill(e_Vx_CD);
                e_Vy_CD = e->par()->getVy();
                Vertex_Electron_1e_Vy_CD_test.Fill(e_Vy_CD);
                e_Vz_CD = e->par()->getVz();
                Vertex_Electron_1e_Vz_CD_test.Fill(e_Vz_CD);
            } else if (e->getRegion() == FD) {
                P_e_FD = e->getP();
                E_e_FD = sqrt(m_e * m_e + P_e_FD * P_e_FD);
                E_e_hist_FD->Fill(E_e_FD);

                Beta_vs_P_1e_FD->Fill(P_e_FD, e->par()->getBeta());
                Beta_vs_P_1e_Electrons_Only_FD->Fill(P_e_FD, e->par()->getBeta());

                if (e->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_FD->Fill(P_e_FD, e->par()->getBeta());
                } else if (e->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_FD->Fill(P_e_FD, e->par()->getBeta());
                } else if (e->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_FD->Fill(P_e_FD, e->par()->getBeta());
                }

                theta_e_1e_FD = e->getTheta() * 180.0 / pi; // theta_e_1e_FD in deg
                Theta_e_1e_FD->Fill(theta_e_1e_FD);
                phi_e_1e_FD = e->getPhi() * 180.0 / pi; // phi_e_1e_FD in deg
                Phi_e_1e_FD->Fill(phi_e_1e_FD);
                Theta_e_VS_Phi_e_1e_FD->Fill(phi_e_1e_FD, theta_e_1e_FD);
                E_e_VS_Theta_e_hist_FD->Fill(theta_e_1e_FD, E_e_FD);

                if (Nf == 3) {
                    Theta_e_1e2X_FD->Fill(theta_e_1e_FD);
                    Phi_e_1e2X_FD->Fill(phi_e_1e_FD);

                    if (qel) {
                        ++num_of_QEL_1e2X_FD_events;
                        Theta_e_1e2X_QEL_FD->Fill(theta_e_1e_FD);
                        Phi_e_1e2X_QEL_FD->Fill(phi_e_1e_FD);
                    } else if (mec) {
                        ++num_of_MEC_1e2X_FD_events;
                        Theta_e_1e2X_MEC_FD->Fill(theta_e_1e_FD);
                        Phi_e_1e2X_MEC_FD->Fill(phi_e_1e_FD);
                    } else if (res) {
                        ++num_of_RES_1e2X_FD_events;
                        Theta_e_1e2X_RES_FD->Fill(theta_e_1e_FD);
                        Phi_e_1e2X_RES_FD->Fill(phi_e_1e_FD);
                    } else if (dis) {
                        ++num_of_DIS_1e2X_FD_events;
                        Theta_e_1e2X_DIS_FD->Fill(theta_e_1e_FD);
                        Phi_e_1e2X_DIS_FD->Fill(phi_e_1e_FD);
                    }

                    if (protons.size() == 2) {
                        Theta_e_1e2p_FD->Fill(theta_e_1e_FD);
                        Phi_e_1e2p_FD->Fill(phi_e_1e_FD);

                        if (qel) {
                            ++num_of_QEL_1e2p_FD_events;
                            Theta_e_1e2p_QEL_FD->Fill(theta_e_1e_FD);
                            Phi_e_1e2p_QEL_FD->Fill(phi_e_1e_FD);
                        } else if (mec) {
                            ++num_of_MEC_1e2p_FD_events;
                            Theta_e_1e2p_MEC_FD->Fill(theta_e_1e_FD);
                            Phi_e_1e2p_MEC_FD->Fill(phi_e_1e_FD);
                        } else if (res) {
                            ++num_of_RES_1e2p_FD_events;
                            Theta_e_1e2p_RES_FD->Fill(theta_e_1e_FD);
                            Phi_e_1e2p_RES_FD->Fill(phi_e_1e_FD);
                        } else if (dis) {
                            ++num_of_DIS_1e2p_FD_events;
                            Theta_e_1e2p_DIS_FD->Fill(theta_e_1e_FD);
                            Phi_e_1e2p_DIS_FD->Fill(phi_e_1e_FD);
                        }
                    }
                }

                e_Chi2_FD = e->par()->getChi2Pid();
                Chi2_Electron_1e_FD->Fill(e_Chi2_FD);

                e_Vx_FD = e->par()->getVx();
                Vertex_Electron_1e_Vx_FD_test.Fill(e_Vx_FD);
                e_Vy_FD = e->par()->getVy();
                Vertex_Electron_1e_Vy_FD_test.Fill(e_Vy_FD);
                e_Vz_FD = e->par()->getVz();
                Vertex_Electron_1e_Vz_FD_test.Fill(e_Vz_FD);
            }
        } // end of loop over electrons vector
        //</editor-fold>

        //<editor-fold desc="Fill Proton plots (1e only, CD & FD)">
        double p_Chi2_tmp_CD, p_Vx_tmp_CD, p_Vy_tmp_CD, p_Vz_tmp_CD;
        double p_Chi2_tmp_FD, p_Vx_tmp_FD, p_Vy_tmp_FD, p_Vz_tmp_FD;

        for (auto &p: protons) {
            if (p->getRegion() == CD) {
                Beta_vs_P_1e_CD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_1e_Protons_Only_CD->Fill(p->getP(), p->par()->getBeta());

                if (p->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_CD->Fill(p->getP(), p->par()->getBeta());
                } else if (p->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_CD->Fill(p->getP(), p->par()->getBeta());
                } else if (p->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_CD->Fill(p->getP(), p->par()->getBeta());
                }

                p_Chi2_tmp_CD = p->par()->getChi2Pid();
                Chi2_Proton_1e_CD->Fill(p_Chi2_tmp_CD);

                p_Vx_tmp_CD = p->par()->getVx();
                Vertex_Proton_1e_Vx_CD_test.Fill(p_Vx_tmp_CD);
                p_Vy_tmp_CD = p->par()->getVy();
                Vertex_Proton_1e_Vy_CD_test.Fill(p_Vy_tmp_CD);
                p_Vz_tmp_CD = p->par()->getVz();
                Vertex_Proton_1e_Vz_CD_test.Fill(p_Vz_tmp_CD);
            } else if (p->getRegion() == FD) {
                Beta_vs_P_1e_FD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_1e_Protons_Only_FD->Fill(p->getP(), p->par()->getBeta());

                if (p->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_FD->Fill(p->getP(), p->par()->getBeta());
                } else if (p->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_FD->Fill(p->getP(), p->par()->getBeta());
                } else if (p->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_FD->Fill(p->getP(), p->par()->getBeta());
                }

                p_Chi2_tmp_FD = p->par()->getChi2Pid();
                Chi2_Proton_1e_FD->Fill(p_Chi2_tmp_FD);

                p_Vx_tmp_FD = p->par()->getVx();
                Vertex_Proton_1e_Vx_FD_test.Fill(p_Vx_tmp_FD);
                p_Vy_tmp_FD = p->par()->getVy();
                Vertex_Proton_1e_Vy_FD_test.Fill(p_Vy_tmp_FD);
                p_Vz_tmp_FD = p->par()->getVz();
                Vertex_Proton_1e_Vz_FD_test.Fill(p_Vz_tmp_FD);
            }
        } // end of loop over protons vector
        //</editor-fold>

        //<editor-fold desc="Fill Beta vs. P (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST)">

//        //<editor-fold desc="neutrons Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
//        for (int i = 0; i < neutrons.size(); i++) {
//            if (neutrons[i]->getRegion() == CD) {
//                Beta_vs_P_1e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                Beta_vs_P_1e_Neutrons_Only_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_CD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            } else if (neutrons[i]->getRegion() == FD) {
//                Beta_vs_P_1e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                Beta_vs_P_1e_Neutrons_Only_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//
//                if (neutrons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                } else if (neutrons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_FD->Fill(neutrons[i]->getP(), neutrons[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over neutrons vector
//        //</editor-fold>

        //<editor-fold desc="Kplus Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
        for (int i = 0; i < Kplus.size(); i++) {
            if (Kplus[i]->getRegion() == CD) {
                Beta_vs_P_1e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_1e_Kplus_Only_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

                if (Kplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                }
            } else if (Kplus[i]->getRegion() == FD) {
                Beta_vs_P_1e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_1e_Kplus_Only_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

                if (Kplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                } else if (Kplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                }
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                Beta_vs_P_1e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_1e_Kminus_Only_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());

                if (Kminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                }
            } else if (Kminus[i]->getRegion() == FD) {
                Beta_vs_P_1e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_1e_Kminus_Only_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());

                if (Kminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                } else if (Kminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                }
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                Beta_vs_P_1e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_1e_piplus_Only_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                if (piplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                }
            } else if (piplus[i]->getRegion() == FD) {
                Beta_vs_P_1e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_1e_piplus_Only_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                if (piplus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                } else if (piplus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                }
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                Beta_vs_P_1e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_1e_piminus_Only_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                if (piminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                }
            } else if (piminus[i]->getRegion() == FD) {
                Beta_vs_P_1e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_1e_piminus_Only_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                if (piminus[i]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_1e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_1e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                } else if (piminus[i]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_1e_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                }
            }
        } // end of loop over piminus vector
        //</editor-fold>

//        //<editor-fold desc="pizero Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
//        for (int i = 0; i < pizero.size(); i++) {
//            if (pizero[i]->getRegion() == CD) {
//                Beta_vs_P_1e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                Beta_vs_P_1e_pizero_Only_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_CD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            } else if (pizero[i]->getRegion() == FD) {
//                Beta_vs_P_1e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                Beta_vs_P_1e_pizero_Only_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//
//                if (pizero[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                } else if (pizero[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_FD->Fill(pizero[i]->getP(), pizero[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over pizero vector
//        //</editor-fold>

//        //<editor-fold desc="neutrals Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
//        for (int i = 0; i < neutrals.size(); i++) {
//            if (neutrals[i]->getRegion() == CD) {
//                Beta_vs_P_1e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//
//                if (neutrals[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_CD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                }
//            } else if (neutrals[i]->getRegion() == FD) {
//                Beta_vs_P_1e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//
//                if (neutrals[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                } else if (neutrals[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_FD->Fill(neutrals[i]->getP(), neutrals[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over Kplus vector
//        //</editor-fold>
//
//        //<editor-fold desc="deuterons Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
//        for (int i = 0; i < deuterons.size(); i++) {
//            if (deuterons[i]->getRegion() == CD) {
//                Beta_vs_P_1e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//
//                if (deuterons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_CD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                }
//            } else if (deuterons[i]->getRegion() == FD) {
//                Beta_vs_P_1e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//
//                if (deuterons[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                } else if (deuterons[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_FD->Fill(deuterons[i]->getP(), deuterons[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over Kplus vector
//        //</editor-fold>
//
//        //<editor-fold desc="otherpart Beta vs. P plots (1e cut TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST TEST, CD & FD)">
//        for (int i = 0; i < otherpart.size(); i++) {
//            if (otherpart[i]->getRegion() == CD) {
//                Beta_vs_P_1e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//
//                if (otherpart[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_CD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                }
//            } else if (otherpart[i]->getRegion() == FD) {
//                Beta_vs_P_1e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//
//                if (otherpart[i]->par()->getCharge() == 1) {
//                    Beta_vs_P_positive_particles_1e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == 0) {
//                    Beta_vs_P_neutral_particles_1e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                } else if (otherpart[i]->par()->getCharge() == -1) {
//                    Beta_vs_P_negative_particles_1e_FD->Fill(otherpart[i]->getP(), otherpart[i]->par()->getBeta());
//                }
//            }
//        } // end of loop over Kplus vector
//        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

//  MicroBooNE cuts -----------------------------------------------------------------------------------------------------------------------------------------------------

//        //<editor-fold desc="MicroBooNE cuts">
//        /* 2p with no pi0 (according to "no neutral pions of any momentum" and "any number of neutrons") */
//        int ne = electrons.size(), np = protons.size(), npi0 = pizero.size(), nn = neutrons.size(), npip = piplus.size(), npim = piminus.size();
//        bool MicroBooNE_particle_selection = ((ne == 1) && (np == 2) && (npi0 == 0) && (Nf == ne + np + nn + npi0 + npip + npim));
//
////            if ((calculate_MicroBooNE == true) && ((protons.size() == 2) && (pizero.size() == 0) && (pizero.size() == 0))) {
//        if ((calculate_MicroBooNE == true) && MicroBooNE_particle_selection) {
//            ++num_of_MicroBooNE_events_BC;
//
//            if (nn != 0) { ++num_of_MicroBooNE_events_BC_wNeutrons; }
//            if (npi0 != 0) { ++num_of_MicroBooNE_events_BC_wpi0; }
//            if (npip != 0) { ++num_of_MicroBooNE_events_BC_wpip; }
//            if (npim != 0) { ++num_of_MicroBooNE_events_BC_wpim; }
//
//            //<editor-fold desc="Filling Beta vs. P (MicroBooNE-BC)">
//            /* Testing Beta vs. P before applying the MicroBooNE cuts */
//            for (int i = 0; i < AllParticles.size(); i++) {
//                if (AllParticles[i]->getRegion() == CD) {
//                    Beta_vs_P_MicroBooNE_BC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//
//                    if (AllParticles[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_MicroBooNE_BC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                    } else if (AllParticles[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_MicroBooNE_BC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                    } else if (AllParticles[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_MicroBooNE_BC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                    }
//                } else if (AllParticles[i]->getRegion() == FD) {
//                    Beta_vs_P_MicroBooNE_BC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//
//                    if (AllParticles[i]->par()->getCharge() == 1) {
//                        Beta_vs_P_positive_particles_MicroBooNE_BC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                    } else if (AllParticles[i]->par()->getCharge() == 0) {
//                        Beta_vs_P_neutral_particles_MicroBooNE_BC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                    } else if (AllParticles[i]->par()->getCharge() == -1) {
//                        Beta_vs_P_negative_particles_MicroBooNE_BC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                    }
//                }
//            } // end of loop over AllParticles vector
//            //</editor-fold>
//
//            TVector3 P_e_MicroBooNE, P_p0_MicroBooNE, P_p1_MicroBooNE, P_pcpion_MicroBooNE, P_ncpion_MicroBooNE;
//            /* NOTE: p0 corresponds to protons[0] & p1 corresponds to protons[1] */
//            P_e_MicroBooNE.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
//            P_p0_MicroBooNE.SetMagThetaPhi(protons[0]->getP(), protons[0]->getTheta(), protons[0]->getPhi());
//            P_p1_MicroBooNE.SetMagThetaPhi(protons[1]->getP(), protons[1]->getTheta(), protons[1]->getPhi());
//
//            //  Testing cuts (MicroBooNE) ---------------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Testing cuts (MicroBooNE)">
//
//            //  Testing momentum cuts (MicroBooNE) ------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Testing momentum cuts (MicroBooNE)">
//            /* Momentum plots before and after cuts */
//            if (electrons[0]->getRegion() == CD) {
//                P_e_MicroBooNE_BC_CD->Fill(P_e_MicroBooNE.Mag());
//                LogEventCuts(P_e_MicroBooNE_AC_CD, electrons[0], "", e_momentum_upper_cut_MicroBooNE, e_momentum_lower_cut_MicroBooNE);
//            } else if (electrons[0]->getRegion() == FD) {
//                P_e_MicroBooNE_BC_FD->Fill(P_e_MicroBooNE.Mag());
//                LogEventCuts(P_e_MicroBooNE_AC_FD, electrons[0], "", e_momentum_upper_cut_MicroBooNE, e_momentum_lower_cut_MicroBooNE);
//            }
//
//            if (protons[0]->getRegion() == CD) {
//                P_p_MicroBooNE_BC_CD->Fill(P_p0_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_CD, protons[0], "", p_momentum_upper_cut_MicroBooNE, p_momentum_lower_cut_MicroBooNE);
//            } else if (protons[0]->getRegion() == FD) {
//                P_p_MicroBooNE_BC_FD->Fill(P_p0_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_FD, protons[0], "", p_momentum_upper_cut_MicroBooNE, p_momentum_lower_cut_MicroBooNE);
//            }
//
//            if (protons[1]->getRegion() == CD) {
//                P_p_MicroBooNE_BC_CD->Fill(P_p1_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_CD, protons[1], "", p_momentum_upper_cut_MicroBooNE, p_momentum_lower_cut_MicroBooNE);
//            } else if (protons[1]->getRegion() == FD) {
//                P_p_MicroBooNE_BC_FD->Fill(P_p1_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_FD, protons[1], "", p_momentum_upper_cut_MicroBooNE, p_momentum_lower_cut_MicroBooNE);
//            }
//
//            /* Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)") */
//            for (int i = 0; i < piplus.size(); i++) {
//                P_pcpion_MicroBooNE.SetMagThetaPhi(piplus[i]->getP(), piplus[i]->getTheta(), piplus[i]->getPhi());
//
//                if (piplus[i]->getRegion() == CD) {
//                    P_cpion_MicroBooNE_BC_CD->Fill(P_pcpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_CD, piplus[i], "", cpion_momentum_upper_cut_MicroBooNE, cpion_momentum_lower_cut_MicroBooNE);
//                } else if (piplus[i]->getRegion() == FD) {
//                    P_cpion_MicroBooNE_BC_FD->Fill(P_pcpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_FD, piplus[i], "", cpion_momentum_upper_cut_MicroBooNE, cpion_momentum_lower_cut_MicroBooNE);
//                }
//            } // end of loop over piplus vector
//
//            /* Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)") */
//            for (int i = 0; i < piminus.size(); i++) {
//                P_ncpion_MicroBooNE.SetMagThetaPhi(piminus[i]->getP(), piminus[i]->getTheta(), piminus[i]->getPhi());
//
//                if (piminus[i]->getRegion() == CD) {
//                    P_cpion_MicroBooNE_BC_CD->Fill(P_ncpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_CD, piminus[i], "", cpion_momentum_upper_cut_MicroBooNE, cpion_momentum_lower_cut_MicroBooNE);
//                } else if (piminus[i]->getRegion() == FD) {
//                    P_cpion_MicroBooNE_BC_FD->Fill(P_ncpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_FD, piminus[i], "", cpion_momentum_upper_cut_MicroBooNE, cpion_momentum_lower_cut_MicroBooNE);
//                }
//            } // end of loop over piminus vector
//            //</editor-fold>
//
//            //  Testing chi2 cuts (MicroBooNE) ----------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Testing chi2 before and after cuts">
//            /* Chi2 plots before and after cuts */
//            if (electrons[0]->getRegion() == CD) {
//                Chi2_Electron_MicroBooNE_BC_CD->Fill(electrons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Electron_1e_peak_MicroBooNE_CD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_CD) {
//                    Chi2_Electron_MicroBooNE_AC_CD->Fill(electrons[0]->par()->getChi2Pid());
//                }
//            } else if (electrons[0]->getRegion() == FD) {
//                Chi2_Electron_MicroBooNE_BC_FD->Fill(electrons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Electron_1e_peak_MicroBooNE_FD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_FD) {
//                    Chi2_Electron_MicroBooNE_AC_FD->Fill(electrons[0]->par()->getChi2Pid());
//                }
//            }
//
//            if (protons[0]->getRegion() == CD) {
//                Chi2_Proton_MicroBooNE_BC_CD->Fill(protons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_CD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD) {
//                    Chi2_Proton_MicroBooNE_AC_CD->Fill(protons[0]->par()->getChi2Pid());
//                }
//            } else if (protons[0]->getRegion() == FD) {
//                Chi2_Proton_MicroBooNE_BC_FD->Fill(protons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_FD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD) {
//                    Chi2_Proton_MicroBooNE_AC_FD->Fill(protons[0]->par()->getChi2Pid());
//                }
//            }
//
//            if (protons[1]->getRegion() == CD) {
//                Chi2_Proton_MicroBooNE_BC_CD->Fill(protons[1]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_CD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD) {
//                    Chi2_Proton_MicroBooNE_AC_CD->Fill(protons[1]->par()->getChi2Pid());
//                }
//            } else if (protons[1]->getRegion() == FD) {
//                Chi2_Proton_MicroBooNE_BC_FD->Fill(protons[1]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_FD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD) {
//                    Chi2_Proton_MicroBooNE_AC_FD->Fill(protons[1]->par()->getChi2Pid());
//                }
//            }
//
//            for (int i = 0; i < piplus.size(); i++) {
//                if (piplus[i]->getRegion() == CD) {
//                    Chi2_piplus_MicroBooNE_BC_CD->Fill(piplus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piplus_1e_peak_MicroBooNE_CD - piplus[i]->par()->getChi2Pid()) <= Chi2_piplus_cut_MicroBooNE_CD) {
//                        Chi2_piplus_MicroBooNE_AC_CD->Fill(piplus[i]->par()->getChi2Pid());
//                    }
//                } else if (piplus[i]->getRegion() == FD) {
//                    Chi2_piplus_MicroBooNE_BC_FD->Fill(piplus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piplus_1e_peak_MicroBooNE_FD - piplus[i]->par()->getChi2Pid()) <= Chi2_piplus_cut_MicroBooNE_FD) {
//                        Chi2_piplus_MicroBooNE_AC_FD->Fill(piplus[i]->par()->getChi2Pid());
//                    }
//                }
//            } // end of loop over piplus vector
//
//            for (int i = 0; i < piminus.size(); i++) {
//                if (piminus[i]->getRegion() == CD) {
//                    Chi2_piminus_MicroBooNE_BC_CD->Fill(piminus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piminus_1e_peak_MicroBooNE_CD - piminus[i]->par()->getChi2Pid()) <= Chi2_piminus_cut_MicroBooNE_CD) {
//                        Chi2_piminus_MicroBooNE_AC_CD->Fill(piminus[i]->par()->getChi2Pid());
//                    }
//                } else if (piminus[i]->getRegion() == FD) {
//                    Chi2_piminus_MicroBooNE_BC_FD->Fill(piminus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piminus_1e_peak_MicroBooNE_FD - piminus[i]->par()->getChi2Pid()) <= Chi2_piminus_cut_MicroBooNE_FD) {
//                        Chi2_piminus_MicroBooNE_AC_FD->Fill(piminus[i]->par()->getChi2Pid());
//                    }
//                }
//            } // end of loop over piminus vector
//            //</editor-fold>
//
//            //  Testing dVz cuts (MicroBooNE) -----------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Testing dV cuts before and after">
//            //TODO: check with Adi/Justin if vertex cuts should be applied on cPions as well
//            double Vx_e_MicroBooNE = electrons[0]->par()->getVx(), Vy_e_MicroBooNE = electrons[0]->par()->getVy(), Vz_e_MicroBooNE = electrons[0]->par()->getVz();
//            double Vz_p0_MicroBooNE = protons[0]->par()->getVz(), Vz_p1_MicroBooNE = protons[1]->par()->getVz();
//            double dVz0_MicroBooNE = Vz_e_MicroBooNE - Vz_p0_MicroBooNE, dVz1_MicroBooNE = Vz_e_MicroBooNE - Vz_p1_MicroBooNE;
//            double Vx_p_MicroBooNE, Vy_p_MicroBooNE, Vz_p_MicroBooNE, dVx_MicroBooNE, dVy_MicroBooNE, dVz_MicroBooNE;
//
//            for (auto &p: protons) {
//                Vx_p_MicroBooNE = p->par()->getVx(), Vy_p_MicroBooNE = p->par()->getVy(), Vz_p_MicroBooNE = p->par()->getVz();
//                dVx_MicroBooNE = Vx_e_MicroBooNE - Vx_p_MicroBooNE;
//                dVy_MicroBooNE = Vy_e_MicroBooNE - Vy_p_MicroBooNE;
//                dVz_MicroBooNE = Vz_e_MicroBooNE - Vz_p_MicroBooNE;
//
//                // Testing dVx,dVy,dVz before cuts
//                deltaVx_MicroBooNE_BC->Fill(dVx_MicroBooNE), deltaVy_MicroBooNE_BC->Fill(dVy_MicroBooNE), deltaVz_MicroBooNE_BC->Fill(dVz_MicroBooNE);
//
//                // Testing dVx,dVy,dVz after cuts
//                if (fabs(dVx_peak_MicroBooNE - dVx_MicroBooNE) <= dVx_cut_MicroBooNE) { deltaVx_MicroBooNE_AC->Fill(dVx_MicroBooNE); }
//                if (fabs(dVy_peak_MicroBooNE - dVy_MicroBooNE) <= dVy_cut_MicroBooNE) { deltaVy_MicroBooNE_AC->Fill(dVy_MicroBooNE); }
//                if (fabs(dVz_peak_MicroBooNE - dVz_MicroBooNE) <= dVz_cut_MicroBooNE) { deltaVz_MicroBooNE_AC->Fill(dVz_MicroBooNE); }
//            } // end of loop over protons vector
//            //</editor-fold>
//
//            //</editor-fold>
//
//            //  Applying cuts (MicroBooNE) --------------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Applying cuts (MicroBooNE)">
//
//            //  Applying momentum cuts (MicroBooNE) -------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Applying momentum cuts (MicroBooNE)">
//            bool MicroBooNE_mom_cuts;
//
//            // Electron:
//            bool e_mom_ucut_MicroBooNE, e_mom_lcut_MicroBooNE;
//
//            if ((e_momentum_upper_cut_MicroBooNE == -1) && (e_momentum_lower_cut_MicroBooNE == -1)) {
//                e_mom_ucut_MicroBooNE = e_mom_lcut_MicroBooNE = true;
//            } else if ((e_momentum_upper_cut_MicroBooNE != -1) && (e_momentum_lower_cut_MicroBooNE == -1)) {
//                e_mom_ucut_MicroBooNE = (P_e_MicroBooNE.Mag() <= e_momentum_upper_cut_MicroBooNE);
//                e_mom_lcut_MicroBooNE = true;
//            } else if ((e_momentum_upper_cut_MicroBooNE == -1) && (e_momentum_lower_cut_MicroBooNE != -1)) {
//                e_mom_ucut_MicroBooNE = true;
//                e_mom_lcut_MicroBooNE = (P_e_MicroBooNE.Mag() >= e_momentum_lower_cut_MicroBooNE);
//            } else if ((e_momentum_upper_cut_MicroBooNE != -1) && (e_momentum_lower_cut_MicroBooNE != -1)) {
//                e_mom_ucut_MicroBooNE = (P_e_MicroBooNE.Mag() <= e_momentum_upper_cut_MicroBooNE);
//                e_mom_lcut_MicroBooNE = (P_e_MicroBooNE.Mag() >= e_momentum_lower_cut_MicroBooNE);
//            }
//
//            bool e_mom_cut_MicroBooNE = (e_mom_ucut_MicroBooNE && e_mom_lcut_MicroBooNE);
//
//            // Proton 0:
//            bool p0_mom_ucut_MicroBooNE, p0_mom_lcut_MicroBooNE;
//
//            if ((p_momentum_upper_cut_MicroBooNE == -1) && (p_momentum_lower_cut_MicroBooNE == -1)) {
//                p0_mom_ucut_MicroBooNE = p0_mom_lcut_MicroBooNE = true;
//            } else if ((p_momentum_upper_cut_MicroBooNE != -1) && (p_momentum_lower_cut_MicroBooNE == -1)) {
//                p0_mom_ucut_MicroBooNE = (P_p0_MicroBooNE.Mag() <= p_momentum_upper_cut_MicroBooNE);
//                p0_mom_lcut_MicroBooNE = true;
//            } else if ((p_momentum_upper_cut_MicroBooNE == -1) && (p_momentum_lower_cut_MicroBooNE != -1)) {
//                p0_mom_ucut_MicroBooNE = true;
//                p0_mom_lcut_MicroBooNE = (P_p0_MicroBooNE.Mag() >= p_momentum_lower_cut_MicroBooNE);
//            } else if ((p_momentum_upper_cut_MicroBooNE != -1) && (p_momentum_lower_cut_MicroBooNE != -1)) {
//                p0_mom_ucut_MicroBooNE = (P_p0_MicroBooNE.Mag() <= p_momentum_upper_cut_MicroBooNE);
//                p0_mom_lcut_MicroBooNE = (P_p0_MicroBooNE.Mag() >= p_momentum_lower_cut_MicroBooNE);
//            }
//
//            bool p0_mom_cut_MicroBooNE = (p0_mom_ucut_MicroBooNE && p0_mom_lcut_MicroBooNE);
//
//            // Proton 1:
//            bool p1_mom_ucut_MicroBooNE, p1_mom_lcut_MicroBooNE;
//
//            if ((p_momentum_upper_cut_MicroBooNE == -1) && (p_momentum_lower_cut_MicroBooNE == -1)) {
//                p1_mom_ucut_MicroBooNE = p1_mom_lcut_MicroBooNE = true;
//            } else if ((p_momentum_upper_cut_MicroBooNE != -1) && (p_momentum_lower_cut_MicroBooNE == -1)) {
//                p1_mom_ucut_MicroBooNE = (P_p1_MicroBooNE.Mag() <= p_momentum_upper_cut_MicroBooNE);
//                p1_mom_lcut_MicroBooNE = true;
//            } else if ((p_momentum_upper_cut_MicroBooNE == -1) && (p_momentum_lower_cut_MicroBooNE != -1)) {
//                p1_mom_ucut_MicroBooNE = true;
//                p1_mom_lcut_MicroBooNE = (P_p1_MicroBooNE.Mag() >= p_momentum_lower_cut_MicroBooNE);
//            } else if ((p_momentum_upper_cut_MicroBooNE != -1) && (p_momentum_lower_cut_MicroBooNE != -1)) {
//                p1_mom_ucut_MicroBooNE = (P_p1_MicroBooNE.Mag() <= p_momentum_upper_cut_MicroBooNE);
//                p1_mom_lcut_MicroBooNE = (P_p1_MicroBooNE.Mag() >= p_momentum_lower_cut_MicroBooNE);
//            }
//
//            bool p1_mom_cut_MicroBooNE = (p1_mom_ucut_MicroBooNE && p1_mom_lcut_MicroBooNE);
//
//            bool p_mom_cut_MicroBooNE = (p0_mom_cut_MicroBooNE && p1_mom_cut_MicroBooNE);
//
//            // Positive pions:
//            bool pcpion_mom_ucut_MicroBooNE, pcpion_mom_lcut_MicroBooNE;
//            bool pip_mom_cut_MicroBooNE = true;
//
//            if (piplus.size() > 0) {
//                /* Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)") */
//                for (int i = 0; i < piplus.size(); i++) {
//                    bool temp_pip_mom_cut_MicroBooNE;
//                    P_pcpion_MicroBooNE.SetMagThetaPhi(piplus[i]->getP(), piplus[i]->getTheta(), piplus[i]->getPhi());
//
//                    if ((cpion_momentum_upper_cut_MicroBooNE == -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        pcpion_mom_ucut_MicroBooNE = pcpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_momentum_upper_cut_MicroBooNE != -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        pcpion_mom_ucut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() <= cpion_momentum_upper_cut_MicroBooNE);
//                        pcpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_momentum_upper_cut_MicroBooNE == -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        pcpion_mom_ucut_MicroBooNE = true;
//                        pcpion_mom_lcut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() >= cpion_momentum_lower_cut_MicroBooNE);
//                    } else if ((cpion_momentum_upper_cut_MicroBooNE != -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        pcpion_mom_ucut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() <= cpion_momentum_upper_cut_MicroBooNE);
//                        pcpion_mom_lcut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() >= cpion_momentum_lower_cut_MicroBooNE);
//                    }
//
//                    temp_pip_mom_cut_MicroBooNE = (pcpion_mom_ucut_MicroBooNE && pcpion_mom_lcut_MicroBooNE);
//
//                    if (temp_pip_mom_cut_MicroBooNE == false) { pip_mom_cut_MicroBooNE = temp_pip_mom_cut_MicroBooNE; }
//                } // end of loop over piplus vector
//            }
//
//            // Negative pions:
//            bool ncpion_mom_ucut_MicroBooNE, ncpion_mom_lcut_MicroBooNE;
//            bool pim_mom_cut_MicroBooNE = true;
//
//            if (piminus.size() > 0) {
//                /* Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)") */
//                for (int i = 0; i < piminus.size(); i++) {
//                    bool temp_pim_mom_cut_MicroBooNE;
//                    P_ncpion_MicroBooNE.SetMagThetaPhi(piminus[i]->getP(), piminus[i]->getTheta(), piminus[i]->getPhi());
//
//                    if ((cpion_momentum_upper_cut_MicroBooNE == -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        ncpion_mom_ucut_MicroBooNE = ncpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_momentum_upper_cut_MicroBooNE != -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        ncpion_mom_ucut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() <= cpion_momentum_upper_cut_MicroBooNE);
//                        ncpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_momentum_upper_cut_MicroBooNE == -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        ncpion_mom_ucut_MicroBooNE = true;
//                        ncpion_mom_lcut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() >= cpion_momentum_lower_cut_MicroBooNE);
//                    } else if ((cpion_momentum_upper_cut_MicroBooNE != -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        ncpion_mom_ucut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() <= cpion_momentum_upper_cut_MicroBooNE);
//                        ncpion_mom_lcut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() >= cpion_momentum_lower_cut_MicroBooNE);
//                    }
//
//                    temp_pim_mom_cut_MicroBooNE = (ncpion_mom_ucut_MicroBooNE && ncpion_mom_lcut_MicroBooNE);
//
//                    if (temp_pim_mom_cut_MicroBooNE == false) { pim_mom_cut_MicroBooNE = temp_pim_mom_cut_MicroBooNE; }
//                } // end of loop over piminus vector
//            }
//
//            // Total momentum cut variable:
//            MicroBooNE_mom_cuts = (e_mom_cut_MicroBooNE && p_mom_cut_MicroBooNE && pip_mom_cut_MicroBooNE && pim_mom_cut_MicroBooNE);
//            //</editor-fold>
//
//            //  Applying chi2 cuts (MicroBooNE) -----------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Applying chi2 cuts (MicroBooNE)">
//            bool MicroBooNE_chi2_cuts;
//
//            // Electron:
//            bool e_chi2_MicroBooNE_cut_CD = ((electrons[0]->getRegion() == CD)
//                                             && fabs(Chi2_Electron_1e_peak_MicroBooNE_CD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_CD);
//            bool e_chi2_MicroBooNE_cut_FD = ((electrons[0]->getRegion() == FD)
//                                             && fabs(Chi2_Electron_1e_peak_MicroBooNE_FD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_FD);
//            bool e_chi2_MicroBooNE_cut = (e_chi2_MicroBooNE_cut_CD || e_chi2_MicroBooNE_cut_FD);
//
//            // Proton 0:
//            bool p0_chi2_MicroBooNE_cut_CD = ((protons[0]->getRegion() == CD)
//                                              && fabs(Chi2_Proton_1e_peak_MicroBooNE_CD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD);
//            bool p0_chi2_MicroBooNE_cut_FD = ((protons[0]->getRegion() == FD)
//                                              && fabs(Chi2_Proton_1e_peak_MicroBooNE_FD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD);
//            bool p0_chi2_MicroBooNE_cut = (p0_chi2_MicroBooNE_cut_CD || p0_chi2_MicroBooNE_cut_FD);
//
//            // Proton 1:
//            bool p1_chi2_MicroBooNE_cut_CD = ((protons[1]->getRegion() == CD)
//                                              && fabs(Chi2_Proton_1e_peak_MicroBooNE_CD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD);
//            bool p1_chi2_MicroBooNE_cut_FD = ((protons[1]->getRegion() == FD)
//                                              && fabs(Chi2_Proton_1e_peak_MicroBooNE_FD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD);
//            bool p1_chi2_MicroBooNE_cut = (p1_chi2_MicroBooNE_cut_CD || p1_chi2_MicroBooNE_cut_FD);
//
//            bool p_chi2_MicroBooNE_cut = (p0_chi2_MicroBooNE_cut && p1_chi2_MicroBooNE_cut);
//
//            // Positive pions:
//            bool pcpion_chi2_MicroBooNE_CD, pcpion_chi2_MicroBooNE_FD;
//            bool pip_chi2_cut_MicroBooNE = true;
//
//            if (piplus.size() > 0) {
//                for (int i = 0; i < piplus.size(); i++) {
//                    bool temp_pip_chi2_cut_MicroBooNE;
//
//                    pcpion_chi2_MicroBooNE_CD = ((piplus[i]->getRegion() == CD)
//                                                 && (fabs(Chi2_piplus_1e_peak_MicroBooNE_CD - piplus[i]->par()->getChi2Pid()) <= Chi2_piplus_cut_MicroBooNE_CD));
//                    pcpion_chi2_MicroBooNE_FD = ((piplus[i]->getRegion() == FD)
//                                                 && (fabs(Chi2_piplus_1e_peak_MicroBooNE_FD - piplus[i]->par()->getChi2Pid()) <= Chi2_piplus_cut_MicroBooNE_FD));
//                    temp_pip_chi2_cut_MicroBooNE = (pcpion_chi2_MicroBooNE_CD || pcpion_chi2_MicroBooNE_FD);
//
//                    if (temp_pip_chi2_cut_MicroBooNE == false) { pip_chi2_cut_MicroBooNE = temp_pip_chi2_cut_MicroBooNE; }
//                } // end of loop over piplus vector
//            }
//
//            // Negative pions:
//            bool ncpion_chi2_MicroBooNE_CD, ncpion_chi2_MicroBooNE_FD;
//            bool pim_chi2_cut_MicroBooNE = true;
//
//            if (piminus.size() > 0) {
//                for (int i = 0; i < piminus.size(); i++) {
//                    bool temp_pim_chi2_cut_MicroBooNE;
//
//                    ncpion_chi2_MicroBooNE_CD = ((piminus[i]->getRegion() == CD)
//                                                 && (fabs(Chi2_piminus_1e_peak_MicroBooNE_CD - piminus[i]->par()->getChi2Pid()) <= Chi2_piminus_cut_MicroBooNE_CD));
//                    ncpion_chi2_MicroBooNE_FD = ((piminus[i]->getRegion() == FD)
//                                                 && (fabs(Chi2_piminus_1e_peak_MicroBooNE_FD - piminus[i]->par()->getChi2Pid()) <= Chi2_piminus_cut_MicroBooNE_FD));
//                    temp_pim_chi2_cut_MicroBooNE = (ncpion_chi2_MicroBooNE_CD || ncpion_chi2_MicroBooNE_FD);
//
//                    if (temp_pim_chi2_cut_MicroBooNE == false) { pim_chi2_cut_MicroBooNE = temp_pim_chi2_cut_MicroBooNE; }
//                } // end of loop over piminus vector
//            }
//
//            // Total chi2 cut variable:
//            MicroBooNE_chi2_cuts = (e_chi2_MicroBooNE_cut && p_chi2_MicroBooNE_cut && pip_chi2_cut_MicroBooNE && pim_chi2_cut_MicroBooNE);
//            //</editor-fold>
//
//            //  Applying dVz cuts (MicroBooNE) ------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Applying dVz cuts (MicroBooNE)">
//            bool MicroBooNE_dVz_cuts;
//
//            bool MicroBooNE_dVz0_cuts = (fabs(dVz_peak_MicroBooNE - dVz0_MicroBooNE) <= dVz_cut_MicroBooNE);
//            bool MicroBooNE_dVz1_cuts = (fabs(dVz_peak_MicroBooNE - dVz1_MicroBooNE) <= dVz_cut_MicroBooNE);
//
//            MicroBooNE_dVz_cuts = (MicroBooNE_dVz0_cuts && MicroBooNE_dVz1_cuts);
//            //</editor-fold>
//
//            //</editor-fold>
//
//            //  Fillings histograms (MicroBooNE) --------------------------------------------------------------------------------------------------------------------
//
//            //TODO: add on/off switch to each individuale microboone cut
//            if (MicroBooNE_mom_cuts && MicroBooNE_chi2_cuts && MicroBooNE_dVz_cuts) {
//                ++num_of_MicroBooNE_events_AC;
//
//                if (nn != 0) { ++num_of_MicroBooNE_events_AC_wNeutrons; }
//                if (npi0 != 0) { ++num_of_MicroBooNE_events_AC_wpi0; }
//                if (npip != 0) { ++num_of_MicroBooNE_events_AC_wpip; }
//                if (npim != 0) { ++num_of_MicroBooNE_events_AC_wpim; }
//
//                //<editor-fold desc="Filling Beta vs. P (MicroBooNE-AC)">
//                for (int i = 0; i < AllParticles.size(); i++) {
//                    if (AllParticles[i]->getRegion() == CD) {
//                        Beta_vs_P_MicroBooNE_AC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//
//                        if (AllParticles[i]->par()->getCharge() == 1) {
//                            Beta_vs_P_positive_particles_MicroBooNE_AC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                        } else if (AllParticles[i]->par()->getCharge() == 0) {
//                            Beta_vs_P_neutral_particles_MicroBooNE_AC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                        } else if (AllParticles[i]->par()->getCharge() == -1) {
//                            Beta_vs_P_negative_particles_MicroBooNE_AC_CD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                        }
//                    } else if (AllParticles[i]->getRegion() == FD) {
//                        Beta_vs_P_MicroBooNE_AC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//
//                        if (AllParticles[i]->par()->getCharge() == 1) {
//                            Beta_vs_P_positive_particles_MicroBooNE_AC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                        } else if (AllParticles[i]->par()->getCharge() == 0) {
//                            Beta_vs_P_neutral_particles_MicroBooNE_AC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                        } else if (AllParticles[i]->par()->getCharge() == -1) {
//                            Beta_vs_P_negative_particles_MicroBooNE_AC_FD->Fill(AllParticles[i]->getP(), AllParticles[i]->par()->getBeta());
//                        }
//                    }
//                } // end of loop over AllParticles vector
//                //</editor-fold>
//
//                //<editor-fold desc="Filling MicroBooNE momentum histograms">
//                if (electrons[0]->getRegion() == CD) {
//                    P_e_MicroBooNE_CD->Fill(P_e_MicroBooNE.Mag());
//                } else if (electrons[0]->getRegion() == FD) {
//                    P_e_MicroBooNE_FD->Fill(P_e_MicroBooNE.Mag());
//                }
//
//                if (protons[0]->getRegion() == CD) {
//                    P_p_MicroBooNE_CD->Fill(P_p0_MicroBooNE.Mag());
//                } else if (protons[0]->getRegion() == FD) {
//                    P_p_MicroBooNE_FD->Fill(P_p0_MicroBooNE.Mag());
//                }
//
//                if (protons[1]->getRegion() == CD) {
//                    P_p_MicroBooNE_CD->Fill(P_p1_MicroBooNE.Mag());
//                } else if (protons[1]->getRegion() == FD) {
//                    P_p_MicroBooNE_FD->Fill(P_p1_MicroBooNE.Mag());
//                }
//
//                // Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)"):
//                for (int i = 0; i < piplus.size(); i++) {
//                    P_pcpion_MicroBooNE.SetMagThetaPhi(piplus[i]->getP(), piplus[i]->getTheta(), piplus[i]->getPhi());
//
//                    if (piplus[i]->getRegion() == CD) {
//                        P_cpion_MicroBooNE_CD->Fill(P_pcpion_MicroBooNE.Mag());
//                    } else if (piplus[i]->getRegion() == FD) {
//                        P_cpion_MicroBooNE_FD->Fill(P_pcpion_MicroBooNE.Mag());
//                    }
//                } // end of loop over piplus vector
//
//                // Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)"):
//                for (int i = 0; i < piminus.size(); i++) {
//                    P_ncpion_MicroBooNE.SetMagThetaPhi(piminus[i]->getP(), piminus[i]->getTheta(), piminus[i]->getPhi());
//
//                    if (piminus[i]->getRegion() == CD) {
//                        P_cpion_MicroBooNE_CD->Fill(P_ncpion_MicroBooNE.Mag());
//                    } else if (piminus[i]->getRegion() == FD) {
//                        P_cpion_MicroBooNE_FD->Fill(P_ncpion_MicroBooNE.Mag());
//                    }
//                } // end of loop over piminus vector
//                //</editor-fold>
//
//                //<editor-fold desc="Filling MicroBooNE chi2 histograms">
//                if (electrons[0]->getRegion() == CD) {
//                    Chi2_Electron_MicroBooNE_CD->Fill(electrons[0]->par()->getChi2Pid());
//                } else if (electrons[0]->getRegion() == FD) {
//                    Chi2_Electron_MicroBooNE_FD->Fill(electrons[0]->par()->getChi2Pid());
//                }
//
//                if (protons[0]->getRegion() == CD) {
//                    Chi2_Proton_MicroBooNE_CD->Fill(protons[0]->par()->getChi2Pid());
//                } else if (protons[0]->getRegion() == FD) {
//                    Chi2_Proton_MicroBooNE_FD->Fill(protons[0]->par()->getChi2Pid());
//                }
//
//                if (protons[1]->getRegion() == CD) {
//                    Chi2_Proton_MicroBooNE_CD->Fill(protons[1]->par()->getChi2Pid());
//                } else if (protons[1]->getRegion() == FD) {
//                    Chi2_Proton_MicroBooNE_FD->Fill(protons[1]->par()->getChi2Pid());
//                }
//
//                for (int i = 0; i < piplus.size(); i++) {
//                    if (piplus[i]->getRegion() == CD) {
//                        Chi2_piplus_MicroBooNE_CD->Fill(piplus[i]->par()->getChi2Pid());
//                    } else if (piplus[i]->getRegion() == FD) {
//                        Chi2_piplus_MicroBooNE_FD->Fill(piplus[i]->par()->getChi2Pid());
//                    }
//                } // end of loop over piplus vector
//
//                for (int i = 0; i < piminus.size(); i++) {
//                    if (piminus[i]->getRegion() == CD) {
//                        Chi2_piminus_MicroBooNE_CD->Fill(piminus[i]->par()->getChi2Pid());
//                    } else if (piminus[i]->getRegion() == FD) {
//                        Chi2_piminus_MicroBooNE_FD->Fill(piminus[i]->par()->getChi2Pid());
//                    }
//                } // end of loop over piminus vector
//                //</editor-fold>
//
//                //<editor-fold desc="Filling MicroBooNE dVz histograms">
//                for (auto &p: protons) {
//                    deltaVx_MicroBooNE->Fill(Vx_e_MicroBooNE - p->par()->getVx());
//                    deltaVy_MicroBooNE->Fill(Vy_e_MicroBooNE - p->par()->getVy());
//                    deltaVz_MicroBooNE->Fill(Vz_e_MicroBooNE - p->par()->getVz());
//                } // end of loop over protons vector
//                //</editor-fold>
//
//            }
//        } // End of calculate_MicroBooNE if
//        //</editor-fold>

//  1e2X cut ------------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e2X cut">
        if (Nf == 3) { ++num_of_events_with_1e2X; } // logging #(events) w/ 1e2X (only 3 scattered/detected particles)
        //</editor-fold>

//  1e2p & 2p cuts ------------------------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="1e2p & 2p cuts">
//        if ((Nf == 3) && (Np == 2)) {
//            cout << "Nf == " << Nf << "\n";
//            cout << "Np == " << Np << "\n\n";
//        }

        if ((calculate_2p == true) && ((Nf == 3) && (Np == 2))) { // for 2p calculations
            ++num_of_events_with_1e2p; // logging #(events) w/ 1e2p

            //TODO: rename P_e

            TVector3 P_p0, P_p1;
            /* NOTE: p0 corresponds to protons[0] & p1 corresponds to protons[1] */
            P_p0.SetMagThetaPhi(protons[0]->getP(), protons[0]->getTheta(), protons[0]->getPhi());
            P_p1.SetMagThetaPhi(protons[1]->getP(), protons[1]->getTheta(), protons[1]->getPhi());

            //  Testing cuts ----------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing cuts">

            //  Testing momentum cuts (protons only) --------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing momentum cuts (protons only)">

            /* momentum before cuts */
            if (protons[0]->getRegion() == CD) {
                P_p_1e2p_BC_CD->Fill(P_p0.Mag());
            } else if (protons[0]->getRegion() == FD) {
                P_p_1e2p_BC_FD->Fill(P_p0.Mag());
            }

            if (protons[1]->getRegion() == CD) {
                P_p_1e2p_BC_CD->Fill(P_p1.Mag());
            } else if (protons[1]->getRegion() == FD) {
                P_p_1e2p_BC_FD->Fill(P_p1.Mag());
            }

            /* momentum after cuts */
            if ((p_momentum_upper_cut_2p == -1) && (p_momentum_lower_cut_2p == -1)) {
                if (protons[0]->getRegion() == CD) {
                    P_p_1e2p_AC_CD->Fill(P_p0.Mag());
                } else if (protons[0]->getRegion() == FD) {
                    P_p_1e2p_AC_FD->Fill(P_p0.Mag());
                }
            } else if ((p_momentum_upper_cut_2p != -1) && (p_momentum_lower_cut_2p == -1)) {
                if (P_p0.Mag() <= p_momentum_upper_cut_2p) {
                    if (protons[0]->getRegion() == CD) {
                        P_p_1e2p_AC_CD->Fill(P_p0.Mag());
                    } else if (protons[0]->getRegion() == FD) {
                        P_p_1e2p_AC_FD->Fill(P_p0.Mag());
                    }
                }
            } else if ((p_momentum_upper_cut_2p == -1) && (p_momentum_lower_cut_2p != -1)) {
                if (P_p0.Mag() >= p_momentum_lower_cut_2p) {
                    if (protons[0]->getRegion() == CD) {
                        P_p_1e2p_AC_CD->Fill(P_p0.Mag());
                    } else if (protons[0]->getRegion() == FD) {
                        P_p_1e2p_AC_FD->Fill(P_p0.Mag());
                    }
                }
            } else if ((p_momentum_upper_cut_2p != -1) && (p_momentum_lower_cut_2p != -1)) {
                if ((P_p0.Mag() >= p_momentum_lower_cut_2p) && (P_p0.Mag() <= p_momentum_upper_cut_2p)) {
                    if (protons[0]->getRegion() == CD) {
                        P_p_1e2p_AC_CD->Fill(P_p0.Mag());
                    } else if (protons[0]->getRegion() == FD) {
                        P_p_1e2p_AC_FD->Fill(P_p0.Mag());
                    }
                }
            }

            if ((p_momentum_upper_cut_2p == -1) && (p_momentum_lower_cut_2p == -1)) {
                if (protons[1]->getRegion() == CD) {
                    P_p_1e2p_AC_CD->Fill(P_p1.Mag());
                } else if (protons[1]->getRegion() == FD) {
                    P_p_1e2p_AC_FD->Fill(P_p1.Mag());
                }
            } else if ((p_momentum_upper_cut_2p != -1) && (p_momentum_lower_cut_2p == -1)) {
                if (P_p1.Mag() <= p_momentum_upper_cut_2p) {
                    if (protons[1]->getRegion() == CD) {
                        P_p_1e2p_AC_CD->Fill(P_p1.Mag());
                    } else if (protons[1]->getRegion() == FD) {
                        P_p_1e2p_AC_FD->Fill(P_p1.Mag());
                    }
                }
            } else if ((p_momentum_upper_cut_2p == -1) && (p_momentum_lower_cut_2p != -1)) {
                if (P_p1.Mag() >= p_momentum_lower_cut_2p) {
                    if (protons[1]->getRegion() == CD) {
                        P_p_1e2p_AC_CD->Fill(P_p1.Mag());
                    } else if (protons[1]->getRegion() == FD) {
                        P_p_1e2p_AC_FD->Fill(P_p1.Mag());
                    }
                }
            } else if ((p_momentum_upper_cut_2p != -1) && (p_momentum_lower_cut_2p != -1)) {
                if ((P_p1.Mag() >= p_momentum_lower_cut_2p) && (P_p1.Mag() <= p_momentum_upper_cut_2p)) {
                    if (protons[1]->getRegion() == CD) {
                        P_p_1e2p_AC_CD->Fill(P_p1.Mag());
                    } else if (protons[1]->getRegion() == FD) {
                        P_p_1e2p_AC_FD->Fill(P_p1.Mag());
                    }
                }
            }
            //</editor-fold>

            //  Testing chi2 cuts (protons only) ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing chi2 cuts (protons only)">

            /* Chi2 before cut */
            if (protons[0]->getRegion() == CD) {
//                    ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                Chi2_Proton_1e2p_BC_CD->Fill(protons[0]->par()->getChi2Pid());
            } else if (protons[0]->getRegion() == FD) {
//                    ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                Chi2_Proton_1e2p_BC_FD->Fill(protons[0]->par()->getChi2Pid());
            }

            if (protons[1]->getRegion() == CD) {
//                    ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                Chi2_Proton_1e2p_BC_CD->Fill(protons[1]->par()->getChi2Pid());
            } else if (protons[1]->getRegion() == FD) {
//                    ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                Chi2_Proton_1e2p_BC_FD->Fill(protons[1]->par()->getChi2Pid());
            }

            /* Chi2 after cut */
            if ((protons[0]->getRegion() == CD) && (fabs(Chi2_Proton_1e_peak_CD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD)) {
                ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                Chi2_Proton_1e2p_AC_CD->Fill(protons[0]->par()->getChi2Pid());
            } else if ((protons[0]->getRegion() == FD) && (fabs(Chi2_Proton_1e_peak_FD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD)) {
                ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                Chi2_Proton_1e2p_AC_FD->Fill(protons[0]->par()->getChi2Pid());
            }

            if ((protons[1]->getRegion() == CD) && (fabs(Chi2_Proton_1e_peak_CD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD)) {
                ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                Chi2_Proton_1e2p_AC_CD->Fill(protons[1]->par()->getChi2Pid());
            } else if ((protons[1]->getRegion() == FD) && (fabs(Chi2_Proton_1e_peak_FD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD)) {
                ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                Chi2_Proton_1e2p_AC_FD->Fill(protons[1]->par()->getChi2Pid());
            }
            //</editor-fold>

            //  Testing dVz cuts ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing dV cuts">
            double Vx_e = electrons[0]->par()->getVx(), Vy_e = electrons[0]->par()->getVy(), Vz_e = electrons[0]->par()->getVz();
            double Vz_p0 = protons[0]->par()->getVz(), Vz_p1 = protons[1]->par()->getVz(), dVz0 = Vz_e - Vz_p0, dVz1 = Vz_e - Vz_p1;
            double Vx_p, Vy_p, Vz_p, dVx, dVy, dVz;

            for (auto &p: protons) {
                Vx_p = p->par()->getVx(), Vy_p = p->par()->getVy(), Vz_p = p->par()->getVz();
                dVx = Vx_e - Vx_p, dVy = Vy_e - Vy_p, dVz = Vz_e - Vz_p;

                // Testing dVx,dVy,dVz before cuts
                deltaVx_before_dV_cuts_1e2p->Fill(dVx), deltaVy_before_dV_cuts_1e2p->Fill(dVy), deltaVz_before_dV_cuts_1e2p->Fill(dVz);

                // Testing dVx,dVy,dVz after cuts
                if (fabs(dVx_peak - dVx) <= dVx_cut) { deltaVx_after_dV_cuts_1e2p->Fill(dVx); }
                if (fabs(dVy_peak - dVy) <= dVy_cut) { deltaVy_after_dV_cuts_1e2p->Fill(dVy); }
                if (fabs(dVz_peak - dVz) <= dVz_cut) { deltaVz_after_dV_cuts_1e2p->Fill(dVz); }
            } // end of loop over protons vector
            //</editor-fold>

            //</editor-fold>

            //  Applying cuts ---------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying cuts">

            //  Applying momentum cuts ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying momentum cuts">
            // Electrons:
            if ((apply_momentum_cuts_2p == true) && ((e_momentum_upper_cut_2p != -1) && (P_e_1e.Mag() > e_momentum_upper_cut_2p))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((e_momentum_lower_cut_2p != -1) && (P_e_1e.Mag() < e_momentum_lower_cut_2p))) { continue; }

            // Proton 0:
            if ((apply_momentum_cuts_2p == true) && ((p_momentum_upper_cut_2p != -1) && (P_p0.Mag() > p_momentum_upper_cut_2p))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((p_momentum_lower_cut_2p != -1) && (P_p0.Mag() < p_momentum_lower_cut_2p))) { continue; }

            // Proton 1:
            if ((apply_momentum_cuts_2p == true) && ((p_momentum_upper_cut_2p != -1) && (P_p1.Mag() > p_momentum_upper_cut_2p))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((p_momentum_lower_cut_2p != -1) && (P_p1.Mag() < p_momentum_lower_cut_2p))) { continue; }
            //</editor-fold>

            //  Applying chi2 cuts ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying Chi2 cuts (protons only)">
            // TODO: move proton blocks here to for loop to save some space
            if (protons[0]->getRegion() == CD) {
                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_CD - protons[0]->par()->getChi2Pid()) > Chi2_Proton_cut_CD)) { continue; }
            } else if (protons[0]->getRegion() == FD) {
                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_FD - protons[0]->par()->getChi2Pid()) > Chi2_Proton_cut_FD)) { continue; }
            }

            if (protons[1]->getRegion() == CD) {
                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_CD - protons[1]->par()->getChi2Pid()) > Chi2_Proton_cut_CD)) { continue; }
            } else if (protons[1]->getRegion() == FD) {
                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_FD - protons[1]->par()->getChi2Pid()) > Chi2_Proton_cut_FD)) { continue; }
            }

            ++num_of_events_1e2p_w_allChi2_cuts;
            //</editor-fold>

            //  Applying dVz cuts ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying dVz cuts">
            if ((apply_dVz_cuts == true) && ((fabs(dVz_peak - dVz0) > dVz_cut) || (fabs(dVz_peak - dVz1) > dVz_cut))) { continue; }
            //</editor-fold>

            //</editor-fold>

            //  Fillings histograms ---------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Counting 2p events by reaction">
            ++num_of_events_2p;

            if (qel) {
                ++num_of_2p_QEL_events;
            } else if (mec) {
                ++num_of_2p_MEC_events;
            } else if (res) {
                ++num_of_2p_RES_events;
            } else if (dis) {
                ++num_of_2p_DIS_events;
            }
            //</editor-fold>

            //<editor-fold desc="Filling momentum histograms">
            if (electrons[0]->getRegion() == CD) {
                P_e_2p_CD->Fill(P_e_1e.Mag());
            } else if (electrons[0]->getRegion() == FD) {
                P_e_2p_FD->Fill(P_e_1e.Mag());
            }

            if (protons[0]->getRegion() == CD) {
                P_p_2p_CD->Fill(P_p0.Mag());
            } else if (protons[0]->getRegion() == FD) {
                P_p_2p_FD->Fill(P_p0.Mag());
            }

            if (protons[1]->getRegion() == CD) {
                P_p_2p_CD->Fill(P_p1.Mag());
            } else if (protons[1]->getRegion() == FD) {
                P_p_2p_FD->Fill(P_p1.Mag());
            }
            //</editor-fold>

            //<editor-fold desc="Filling Beta vs. P plots (2p)">

            //<editor-fold desc="Beta vs. P from electrons (2p, CD & FD)">
            if (electrons[0]->getRegion() == CD) {
                Beta_vs_P_2p_CD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                Beta_vs_P_2p_Electrons_Only_CD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());

                if (electrons[0]->par()->getCharge() == 1) {
                    Beta_vs_P_positive_particles_2p_CD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                } else if (electrons[0]->par()->getCharge() == 0) {
                    Beta_vs_P_neutral_particles_2p_CD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                } else if (electrons[0]->par()->getCharge() == -1) {
                    Beta_vs_P_negative_particles_2p_CD->Fill(electrons[0]->getP(), electrons[0]->par()->getBeta());
                }
            } else if (electrons[0]->getRegion() == FD) {
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
            for (int i = 0; i < protons.size(); i++) {
                if (protons[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    Beta_vs_P_2p_Protons_Only_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());

                    if (protons[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    } else if (protons[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    } else if (protons[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    }
                } else if (protons[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    Beta_vs_P_2p_Protons_Only_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());

                    if (protons[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    } else if (protons[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    } else if (protons[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_FD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                    }
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

                    if (Kplus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    } else if (Kplus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    } else if (Kplus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    }
                } else if (Kplus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

                    if (Kplus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    } else if (Kplus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    } else if (Kplus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                    }
                }
            } // end of loop over Kplus vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from Kminus (2p, CD & FD)">
            for (int i = 0; i < Kminus.size(); i++) {
                if (Kminus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());

                    if (Kminus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    } else if (Kminus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    } else if (Kminus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    }
                } else if (Kminus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());

                    if (Kminus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    } else if (Kminus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    } else if (Kminus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                    }
                }
            } // end of loop over Kminus vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from piplus (2p, CD & FD)">
            for (int i = 0; i < piplus.size(); i++) {
                if (piplus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                    if (piplus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    } else if (piplus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    } else if (piplus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    }
                } else if (piplus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());

                    if (piplus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    } else if (piplus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    } else if (piplus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                    }
                }
            } // end of loop over piplus vector
            //</editor-fold>

            //<editor-fold desc="Beta vs. P from piminus (2p, CD & FD)">
            for (int i = 0; i < piminus.size(); i++) {
                if (piminus[i]->getRegion() == CD) {
                    Beta_vs_P_2p_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                    if (piminus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    } else if (piminus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    } else if (piminus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    }
                } else if (piminus[i]->getRegion() == FD) {
                    Beta_vs_P_2p_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());

                    if (piminus[i]->par()->getCharge() == 1) {
                        Beta_vs_P_positive_particles_2p_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    } else if (piminus[i]->par()->getCharge() == 0) {
                        Beta_vs_P_neutral_particles_2p_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    } else if (piminus[i]->par()->getCharge() == -1) {
                        Beta_vs_P_negative_particles_2p_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                    }
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

            for (auto &e: electrons) {
                if (e->getRegion() == CD) {
                    E_e_2p_CD->Fill(E_e_CD);
                    Theta_e_2p_CD->Fill(theta_e_1e_CD);
                    Phi_e_2p_CD->Fill(phi_e_1e_CD);

                    Theta_e_VS_Phi_e_2p_CD->Fill(phi_e_1e_CD, theta_e_1e_CD);
                    E_e_VS_Theta_e_2p_CD->Fill(theta_e_1e_CD, E_e_CD);

                    if (theta_e_1e_CD >= 14.0 && theta_e_1e_CD <= 16.0) { ETrans_15_All_Int_2p_CD->Fill(beamE - E_e_CD); }

                    if (qel) {
                        Theta_e_2p_QEL_CD->Fill(theta_e_1e_CD);
                        Phi_e_2p_QEL_CD->Fill(phi_e_1e_CD);
                        E_e_2p_QEL_CD->Fill(E_e_CD);
                        E_e_VS_Theta_e_2p_QEL_CD->Fill(theta_e_1e_CD, E_e_CD);

                        if (theta_e_1e_CD >= 14.0 && theta_e_1e_CD <= 16.0) { ETrans_15_QEL_2p_CD->Fill(beamE - E_e_CD); }
                    } else if (mec) {
                        Theta_e_2p_MEC_CD->Fill(theta_e_1e_CD);
                        Phi_e_2p_MEC_CD->Fill(phi_e_1e_CD);
                        E_e_2p_MEC_CD->Fill(E_e_CD);
                        E_e_VS_Theta_e_2p_MEC_CD->Fill(theta_e_1e_CD, E_e_CD);

                        if (theta_e_1e_CD >= 14.0 && theta_e_1e_CD <= 16.0) { ETrans_15_MEC_2p_CD->Fill(beamE - E_e_CD); }
                    } else if (res) {
                        Theta_e_2p_RES_CD->Fill(theta_e_1e_CD);
                        Phi_e_2p_RES_CD->Fill(phi_e_1e_CD);
                        E_e_2p_RES_CD->Fill(E_e_CD);
                        E_e_VS_Theta_e_2p_RES_CD->Fill(theta_e_1e_CD, E_e_CD);

                        if (theta_e_1e_CD >= 14.0 && theta_e_1e_CD <= 16.0) { ETrans_15_RES_2p_CD->Fill(beamE - E_e_CD); }
                    } else if (dis) {
                        Theta_e_2p_DIS_CD->Fill(theta_e_1e_CD);
                        Phi_e_2p_DIS_CD->Fill(phi_e_1e_CD);
                        E_e_2p_DIS_CD->Fill(E_e_CD);
                        E_e_VS_Theta_e_2p_DIS_CD->Fill(theta_e_1e_CD, E_e_CD);

                        if (theta_e_1e_CD >= 14.0 && theta_e_1e_CD <= 16.0) { ETrans_15_DIS_2p_CD->Fill(beamE - E_e_CD); }
                    }
                } else if (e->getRegion() == FD) {
                    E_e_2p_FD->Fill(E_e_FD);
                    Theta_e_2p_FD->Fill(theta_e_1e_FD);
                    Phi_e_2p_FD->Fill(phi_e_1e_FD);

                    Theta_e_VS_Phi_e_2p_FD->Fill(phi_e_1e_FD, theta_e_1e_FD);
                    E_e_VS_Theta_e_2p_FD->Fill(theta_e_1e_FD, E_e_FD);

                    if (theta_e_1e_FD >= 14.0 && theta_e_1e_FD <= 16.0) { ETrans_15_All_Int_2p_FD->Fill(beamE - E_e_FD); }

                    if (qel) {
                        Theta_e_2p_QEL_FD->Fill(theta_e_1e_FD);
                        Phi_e_2p_QEL_FD->Fill(phi_e_1e_FD);
                        E_e_2p_QEL_FD->Fill(E_e_FD);
                        E_e_VS_Theta_e_2p_QEL_FD->Fill(theta_e_1e_FD, E_e_FD);

                        if (theta_e_1e_FD >= 14.0 && theta_e_1e_FD <= 16.0) { ETrans_15_QEL_2p_FD->Fill(beamE - E_e_FD); }
                    } else if (mec) {
                        Theta_e_2p_MEC_FD->Fill(theta_e_1e_FD);
                        Phi_e_2p_MEC_FD->Fill(phi_e_1e_FD);
                        E_e_2p_MEC_FD->Fill(E_e_FD);
                        E_e_VS_Theta_e_2p_MEC_FD->Fill(theta_e_1e_FD, E_e_FD);

                        if (theta_e_1e_FD >= 14.0 && theta_e_1e_FD <= 16.0) { ETrans_15_MEC_2p_FD->Fill(beamE - E_e_FD); }
                    } else if (res) {
                        Theta_e_2p_RES_FD->Fill(theta_e_1e_FD);
                        Phi_e_2p_RES_FD->Fill(phi_e_1e_FD);
                        E_e_2p_RES_FD->Fill(E_e_FD);
                        E_e_VS_Theta_e_2p_RES_FD->Fill(theta_e_1e_FD, E_e_FD);

                        if (theta_e_1e_FD >= 14.0 && theta_e_1e_FD <= 16.0) { ETrans_15_RES_2p_FD->Fill(beamE - E_e_FD); }
                    } else if (dis) {
                        Theta_e_2p_DIS_FD->Fill(theta_e_1e_FD);
                        Phi_e_2p_DIS_FD->Fill(phi_e_1e_FD);
                        E_e_2p_DIS_FD->Fill(E_e_FD);
                        E_e_VS_Theta_e_2p_DIS_FD->Fill(theta_e_1e_FD, E_e_FD);

                        if (theta_e_1e_FD >= 14.0 && theta_e_1e_FD <= 16.0) { ETrans_15_DIS_2p_FD->Fill(beamE - E_e_FD); }
                    }
                }
            } // end of loop over electrons vector

            //<editor-fold desc="Filling SF histograms (2p)">
            SF_All_Int_2p_FD->Fill(EoP_e);
            SF_VS_P_e_2p_FD->Fill(P_e_1e.Mag(), EoP_e);
            //</editor-fold>

            //<editor-fold desc="Filling fiducial plots (2p)">
//                Vcal_VS_EoP_2p_ECIN->Fill(electrons[0]->cal(ECIN)->getLv(), EoP_e);
//                Wcal_VS_EoP_2p_ECIN->Fill(electrons[0]->cal(ECIN)->getLw(), EoP_e);
            Vcal_VS_EoP_2p_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_2p_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);
            //</editor-fold>

            //<editor-fold desc="Filling nphe plots (2p)">
            nphe_All_Int_2p_FD->Fill(nphe);
            //</editor-fold>

            //<editor-fold desc="Filling Ecal histograms (2p)">
            double p_e, E_e, Pp0, Ep0, Pp1, Ep1, Ecal_2p;

            p_e = electrons[0]->getP();
            E_e = sqrt(m_e * m_e + p_e * p_e);
            Pp0 = protons[0]->getP();
            Ep0 = sqrt(m_p * m_p + Pp0 * Pp0);
            Pp1 = protons[1]->getP();
            Ep1 = sqrt(m_p * m_p + Pp1 * Pp1);
            Ecal_2p = E_e + (Ep0 - m_p) + (Ep1 - m_p);

            Ecal_All_Int_2p->Fill(Ecal_2p);

            if (qel) {
                Ecal_QEL_2p->Fill(Ecal_2p);
            } else if (mec) {
                Ecal_MEC_2p->Fill(Ecal_2p);
            } else if (res) {
                Ecal_RES_2p->Fill(Ecal_2p);
            } else if (dis) {
                Ecal_DIS_2p->Fill(Ecal_2p);
            }
            //</editor-fold>

            //<editor-fold desc="Filling Chi2 histograms (2p)">

            //<editor-fold desc="Filling electron Chi2 cut 2p">
            if (electrons[0]->getRegion() == CD) {
                Chi2_Electron_2p_CD->Fill(electrons[0]->par()->getChi2Pid());
            } else if (electrons[0]->getRegion() == FD) {
                Chi2_Electron_2p_FD->Fill(electrons[0]->par()->getChi2Pid());
            }
            //</editor-fold>

            //<editor-fold desc="Filling proton0 Chi2 cut 2p">
            if (protons[0]->getRegion() == CD) {
                Chi2_Proton_2p_CD->Fill(protons[0]->par()->getChi2Pid());
            } else if (protons[0]->getRegion() == FD) {
                Chi2_Proton_2p_FD->Fill(protons[0]->par()->getChi2Pid());
            }
            //</editor-fold>

            //<editor-fold desc="Filling proton1 Chi2 cut 2p">
            if (protons[1]->getRegion() == CD) {
                Chi2_Proton_2p_CD->Fill(protons[1]->par()->getChi2Pid());
            } else if (protons[1]->getRegion() == FD) {
                Chi2_Proton_2p_FD->Fill(protons[1]->par()->getChi2Pid());
            }
            //</editor-fold>

            //</editor-fold>

            //<editor-fold desc="Filling dVx, dVy, dVz (2p)">
            for (auto &p: protons) {
                Vx_p = p->par()->getVx();
                Vy_p = p->par()->getVy();
                Vz_p = p->par()->getVz();
                dVx = Vx_e - Vx_p;
                dVy = Vy_e - Vy_p;
                dVz = Vz_e - Vz_p;

                deltaVx_2p->Fill(dVx);
                deltaVy_2p->Fill(dVy);
                deltaVz_2p->Fill(dVz);
            } // end of loop over protons vector
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

    //<editor-fold desc="Canvas c1">
    TCanvas *c1 = new TCanvas("canvas", "canvas", 1650, 1150);
//    c1->cd();
    c1->SetGrid();
    c1->SetBottomMargin(0.14);

    if (wider_margin) {
        c1->SetLeftMargin(0.14);
        c1->SetRightMargin(0.12);
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
        histPlotter2D(c1, Beta_vs_P_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_Electrons_Only_CD_Dir, "02_Beta_vs_P_Electrons_Only_CD.png",
                      beta_electron, "Electrons", true);

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

        //<editor-fold desc="Beta vs. P for all particles (Neutrons Only, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_Neutrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_Neutrons_Only_CD_Dir,
                      "04_Beta_vs_P_Neutrons_Only_CD.png",
                      beta_neutron, "Neutrons", true);

        histPlotter2D(c1, Beta_vs_P_Neutrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_Neutrons_Only_FD_Dir,
                      "04_Beta_vs_P_Neutrons_Only_FD.png",
                      beta_neutron, "Neutrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (all particles, by charge)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_All_e_Directory, "01_Beta_vs_P_q_p1_All_e_CD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_All_e_Directory, "01_Beta_vs_P_q_p1_All_e_FD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_All_e_Directory, "02_Beta_vs_P_q_0_All_e_CD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_All_e_Directory, "02_Beta_vs_P_q_0_All_e_FD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_All_e_Directory, "03_Beta_vs_P_q_m1_All_e_CD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_All_e_Directory, "03_Beta_vs_P_q_m1_All_e_FD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (1e cut)">

        //<editor-fold desc="Beta vs. P for all particles (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_VS_P_Only_1e_Directory, "01_Beta_vs_P_1e_cut_All_Particles_CD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_1e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_VS_P_Only_1e_Directory, "01_Beta_vs_P_1e_cut_All_Particles_FD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Electrons Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "02_Beta_vs_P_1e_cut_Electrons_Only_CD.png",
                      beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_1e_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "02_Beta_vs_P_1e_cut_Electrons_Only_FD.png",
                      beta_electron, "Electrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Protons Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "03_Beta_vs_P_1e_cut_Protons_Only_CD.png",
                      beta_proton, "Protons", true);

        histPlotter2D(c1, Beta_vs_P_1e_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "03_Beta_vs_P_1e_cut_Protons_Only_FD.png",
                      beta_proton, "Protons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Neutrons Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Neutrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "04_Beta_vs_P_1e_cut_Neutrons_Only_CD.png",
                      beta_neutron, "Neutrons", true);

        histPlotter2D(c1, Beta_vs_P_1e_Neutrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "04_Beta_vs_P_1e_cut_Neutrons_Only_FD.png",
                      beta_neutron, "Neutrons", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Kplus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Kplus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "05_Beta_vs_P_1e_cut_Kplus_Only_CD.png",
                      beta_Kplus, "K^{+}", true);

        histPlotter2D(c1, Beta_vs_P_1e_Kplus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "05_Beta_vs_P_1e_cut_Kplus_Only_FD.png",
                      beta_Kplus, "K^{+}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for Kminus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_Kminus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "06_Beta_vs_P_1e_cut_Kminus_Only_CD.png",
                      beta_Kminus, "K^{-}", true);

        histPlotter2D(c1, Beta_vs_P_1e_Kminus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "06_Beta_vs_P_1e_cut_Kminus_Only_FD.png",
                      beta_Kminus, "K^{-}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for piplus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_piplus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "08_Beta_vs_P_1e_cut_piplus_Only_CD.png",
                      beta_piplus, "#pi^{+}", true);

        histPlotter2D(c1, Beta_vs_P_1e_piplus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "08_Beta_vs_P_1e_cut_piplus_Only_FD.png",
                      beta_piplus, "#pi^{+}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for piminus Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_piminus_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "09_Beta_vs_P_1e_cut_piminus_Only_CD.png",
                      beta_piminus, "#pi^{-}", true);

        histPlotter2D(c1, Beta_vs_P_1e_piminus_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "09_Beta_vs_P_1e_cut_piminus_Only_FD.png",
                      beta_piminus, "#pi^{-}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for pizero Only (1e cut, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_1e_pizero_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "10_Beta_vs_P_1e_cut_pizero_Only_CD.png",
                      beta_pizero, "#pi^{0}", true);

        histPlotter2D(c1, Beta_vs_P_1e_pizero_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_Only_1e_Directory, "10_Beta_vs_P_1e_cut_pizero_Only_FD.png",
                      beta_pizero, "#pi^{0}", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (by charge, 1e cut, CD & FD)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_1e_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "01_Beta_vs_P_q_p1_1e_CD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_1e_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "01_Beta_vs_P_q_p1_1e_FD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_1e_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "02_Beta_vs_P_q_0_1e_CD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_1e_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "02_Beta_vs_P_q_0_1e_FD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_1e_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "03_Beta_vs_P_q_m1_1e_CD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_1e_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "03_Beta_vs_P_q_m1_1e_FD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
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
        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_2p_Electrons_Only_CD_Dir, "02_Beta_vs_P_2p_Electrons_Only_CD.png",
                      beta_electron, "Electrons", true);

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

        //<editor-fold desc="Beta vs. P plots (MicroBooNE-BC)">

        //<editor-fold desc="Beta vs. P for all particles (MicroBooNE-BC, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_BC_CD_Dir,
                      "01_Beta_vs_P_MicroBooNE_BC_CD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_BC_FD_Dir,
                      "01_Beta_vs_P_MicroBooNE_BC_FD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
        //</editor-fold>

//        //<editor-fold desc="Beta vs. P for Electrons Only (MicroBooNE-BC, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Electrons_Only_CD_Dir, "02_Beta_vs_P_2p_Electrons_Only_CD.png",
//                      beta_electron, "Electrons", true);
//
//        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Electrons_Only_FD_Dir, "02_Beta_vs_P_2p_Electrons_Only_FD.png",
//                      beta_electron, "Electrons", true);
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P for Protons Only (MicroBooNE-BC, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Protons_Only_CD_Dir, "03_Beta_vs_P_2p_Protons_Only_CD.png",
//                      beta_proton, "Protons", true);
//
//        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Protons_Only_FD_Dir, "03_Beta_vs_P_2p_Protons_Only_FD.png",
//                      beta_proton, "Protons", true);
//        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (by charge, MicroBooNE-BC, CD & FD)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true,
                      Beta_vs_P_positive_particles_MicroBooNE_BC_CD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_BC_CD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_positive_particles_MicroBooNE_BC_FD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_BC_FD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_BC_CD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_BC_CD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_BC_FD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_BC_FD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_BC_CD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_BC_CD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_BC_FD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_BC_FD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (MicroBooNE-AC)">

        //<editor-fold desc="Beta vs. P for all particles (MicroBooNE-AC, CD & FD)">
        histPlotter2D(c1, Beta_vs_P_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_AC_CD_Dir,
                      "01_Beta_vs_P_MicroBooNE_AC_CD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_AC_FD_Dir,
                      "01_Beta_vs_P_MicroBooNE_AC_FD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
        //</editor-fold>

//        //<editor-fold desc="Beta vs. P for Electrons Only (MicroBooNE-AC, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Electrons_Only_CD_Dir, "02_Beta_vs_P_2p_Electrons_Only_CD.png",
//                      beta_electron, "Electrons", true);
//
//        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Electrons_Only_FD_Dir, "02_Beta_vs_P_2p_Electrons_Only_FD.png",
//                      beta_electron, "Electrons", true);
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P for Protons Only (MicroBooNE-AC, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Protons_Only_CD_Dir, "03_Beta_vs_P_2p_Protons_Only_CD.png",
//                      beta_proton, "Protons", true);
//
//        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_2p_Protons_Only_FD_Dir, "03_Beta_vs_P_2p_Protons_Only_FD.png",
//                      beta_proton, "Protons", true);
//        //</editor-fold>

        //<editor-fold desc="Beta vs. P plots (by charge, MicroBooNE-AC, CD & FD)">

        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true,
                      Beta_vs_P_positive_particles_MicroBooNE_AC_CD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_AC_CD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_positive_particles_MicroBooNE_AC_FD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_AC_FD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_AC_CD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_AC_CD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_AC_FD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_AC_FD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_AC_CD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_AC_CD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_AC_FD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_AC_FD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
        //</editor-fold>

        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nBeta vs. P plots are disabled by user.\n\n";
    } // end of Beta vs. P plot if
    //</editor-fold>

// ======================================================================================================================================================================
// Chi2 plots
// ======================================================================================================================================================================

    //<editor-fold desc="Chi2 plots">
    if (Chi2_plots) {

        cout << "\n\nPlotting Chi2 plots...\n\n";

        //<editor-fold desc="Finding Xmax">
        //todo: calculate Xmax according to BC histograms?
        Chi2_Electron_Xmax_CD = Chi2_Electron_CD->GetBinCenter(Chi2_Electron_CD->GetMaximumBin());
        Chi2_Electron_Xmax_FD = Chi2_Electron_FD->GetBinCenter(Chi2_Electron_FD->GetMaximumBin());
        Chi2_Proton_Xmax_CD = Chi2_Proton_CD->GetBinCenter(Chi2_Proton_CD->GetMaximumBin());
        Chi2_Proton_Xmax_FD = Chi2_Proton_FD->GetBinCenter(Chi2_Proton_FD->GetMaximumBin());
        Chi2_Electron_1e_Xmax_CD = Chi2_Electron_1e_CD->GetBinCenter(Chi2_Electron_1e_CD->GetMaximumBin());
        Chi2_Electron_1e_Xmax_FD = Chi2_Electron_1e_FD->GetBinCenter(Chi2_Electron_1e_FD->GetMaximumBin());
        Chi2_Proton_1e_Xmax_CD = Chi2_Proton_1e_CD->GetBinCenter(Chi2_Proton_1e_CD->GetMaximumBin());
        Chi2_Proton_1e_Xmax_FD = Chi2_Proton_1e_FD->GetBinCenter(Chi2_Proton_1e_FD->GetMaximumBin());

        Chi2_Electron_MicroBooNE_Xmax_CD = Chi2_Electron_MicroBooNE_CD->GetBinCenter(Chi2_Electron_MicroBooNE_CD->GetMaximumBin());
        Chi2_Electron_MicroBooNE_Xmax_FD = Chi2_Electron_MicroBooNE_FD->GetBinCenter(Chi2_Electron_MicroBooNE_FD->GetMaximumBin());
        Chi2_Proton_MicroBooNE_Xmax_CD = Chi2_Proton_MicroBooNE_CD->GetBinCenter(Chi2_Proton_MicroBooNE_CD->GetMaximumBin());
        Chi2_Proton_MicroBooNE_Xmax_FD = Chi2_Proton_MicroBooNE_FD->GetBinCenter(Chi2_Proton_MicroBooNE_FD->GetMaximumBin());
        Chi2_piplus_MicroBooNE_Xmax_CD = Chi2_piplus_MicroBooNE_CD->GetBinCenter(Chi2_piplus_MicroBooNE_CD->GetMaximumBin());
        Chi2_piplus_MicroBooNE_Xmax_FD = Chi2_piplus_MicroBooNE_FD->GetBinCenter(Chi2_piplus_MicroBooNE_FD->GetMaximumBin());
        Chi2_piminus_MicroBooNE_Xmax_CD = Chi2_piminus_MicroBooNE_CD->GetBinCenter(Chi2_piminus_MicroBooNE_CD->GetMaximumBin());
        Chi2_piminus_MicroBooNE_Xmax_FD = Chi2_piminus_MicroBooNE_FD->GetBinCenter(Chi2_piminus_MicroBooNE_FD->GetMaximumBin());
        //</editor-fold>

//  Chi2 plots (no #(e) cut) ----------------------------------------------------------------------------

        //<editor-fold desc="Chi2 plots (no #(e) cut)">
        histPlotter1D(c1, Chi2_Electron_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Electron_Stack, "01_Electron_chi2", Chi2_Electron_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Electron_cut_CD,
                      Chi2_Electron_Xmax_CD);
        histPlotter1D(c1, Chi2_Electron_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Electron_Stack, "01_Electron_chi2", Chi2_Electron_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Electron_cut_FD,
                      Chi2_Electron_Xmax_FD);

        histPlotter1D(c1, Chi2_Proton_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_Stack, "02_Proton_chi2", Chi2_Proton_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_Xmax_CD);
        histPlotter1D(c1, Chi2_Proton_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_Stack, "02_Proton_chi2", Chi2_Proton_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Proton_cut_FD, Chi2_Proton_Xmax_FD);
        //</editor-fold>

//  Chi2 plots (1e cut) ----------------------------------------------------------------------------

        //<editor-fold desc="Electron chi2 (1e cut)">
        histPlotter1D(c1, Chi2_Electron_1e_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Electron_1e_Stack, "01_Electron_chi2", Chi2_Electron_1e_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Electron_cut_CD,
                      Chi2_Electron_1e_Xmax_CD);
        histPlotter1D(c1, Chi2_Electron_1e_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Electron_1e_Stack, "01_Electron_chi2", Chi2_Electron_1e_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Electron_cut_FD,
                      Chi2_Electron_1e_Xmax_FD);

        histPlotter1D(c1, Chi2_Proton_1e_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_1e_Stack, "02_Proton_chi2", Chi2_Proton_1e_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_1e_Xmax_CD);
        histPlotter1D(c1, Chi2_Proton_1e_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_1e_Stack, "02_Proton_chi2", Chi2_Proton_1e_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Proton_cut_FD,
                      Chi2_Proton_1e_Xmax_FD);
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (MicroBooNE)">

        //<editor-fold desc="Testing Chi2 cuts before applying (MicroBooNE)">
        histPlotter1D(c1, c2, Chi2_Electron_MicroBooNE_BC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram Before C-Cut", "MicroBooNE", "CD", "e", 0.06,
                      0.04, 0.04, plots, 2, false, true, Chi2_Electron_MicroBooNE_Stack, "01_Electron_Chi2_MicroBooNE_BC", Chi2_Electron_MicroBooNE_BC_CD_Dir, kBlue,
                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_CD, Chi2_Electron_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_Electron_MicroBooNE_BC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram Before C-Cut", "MicroBooNE", "FD", "e", 0.06,
                      0.04, 0.04, plots, 2, false, true, Chi2_Electron_MicroBooNE_Stack, "02_Electron_Chi2_MicroBooNE_BC", Chi2_Electron_MicroBooNE_BC_FD_Dir, kBlue,
                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_FD, Chi2_Electron_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, c2, Chi2_Proton_MicroBooNE_BC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram Before C-Cut", "MicroBooNE", "CD", "p", 0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_Proton_MicroBooNE_Stack, "03_Proton_Chi2_MicroBooNE_BC", Chi2_Proton_MicroBooNE_BC_CD_Dir, kBlue, true, true,
                      true, false, true, Chi2_Proton_cut_MicroBooNE_CD, Chi2_Proton_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_Proton_MicroBooNE_BC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram Before C-Cut", "MicroBooNE", "FD", "p", 0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_Proton_MicroBooNE_Stack, "04_Proton_Chi2_MicroBooNE_BC", Chi2_Proton_MicroBooNE_BC_FD_Dir, kBlue, true, true,
                      true, false, true, Chi2_Proton_cut_MicroBooNE_FD, Chi2_Proton_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, c2, Chi2_piplus_MicroBooNE_BC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{+}} Histogram Before C-Cut", "MicroBooNE", "CD", "#pi^{+}",
                      0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_piplus_MicroBooNE_Stack, "05_piplus_Chi2_MicroBooNE_BC", Chi2_piplus_MicroBooNE_BC_CD_Dir, kBlue, true, true,
                      true, false, true, Chi2_piplus_cut_MicroBooNE_CD, Chi2_piplus_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_piplus_MicroBooNE_BC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{+}} Histogram Before C-Cut", "MicroBooNE", "FD", "#pi^{+}",
                      0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_piplus_MicroBooNE_Stack, "06_piplus_Chi2_MicroBooNE_BC", Chi2_piplus_MicroBooNE_BC_FD_Dir, kBlue, true, true,
                      true, false, true, Chi2_piplus_cut_MicroBooNE_FD, Chi2_piplus_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, c2, Chi2_piminus_MicroBooNE_BC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{-}} Histogram Before C-Cut", "MicroBooNE", "CD", "#pi^{-}",
                      0.06,
                      0.04, 0.04, plots, 2, false, true, Chi2_piminus_MicroBooNE_Stack, "07_piminus_Chi2_MicroBooNE_BC", Chi2_piminus_MicroBooNE_BC_CD_Dir, kBlue, true,
                      true, true, false, true, Chi2_piminus_cut_MicroBooNE_CD, Chi2_piminus_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_piminus_MicroBooNE_BC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{-}} Histogram Before C-Cut", "MicroBooNE", "FD", "#pi^{-}",
                      0.06,
                      0.04, 0.04, plots, 2, false, true, Chi2_piminus_MicroBooNE_Stack, "08_piminus_Chi2_MicroBooNE_BC", Chi2_piminus_MicroBooNE_BC_FD_Dir, kBlue, true,
                      true, true, false, true, Chi2_piminus_cut_MicroBooNE_FD, Chi2_piminus_1e_peak_MicroBooNE_FD);
        //</editor-fold>

        //<editor-fold desc="Testing Chi2 cuts after applying (MicroBooNE)">
        histPlotter1D(c1, c2, Chi2_Electron_MicroBooNE_AC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram After C-Cut", "MicroBooNE", "CD", "e", 0.06,
                      0.04, 0.04, plots, 2, false, true, Chi2_Electron_MicroBooNE_Stack, "01_Electron_Chi2_MicroBooNE_AC", Chi2_Electron_MicroBooNE_AC_CD_Dir, kBlue,
                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_CD, Chi2_Electron_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_Electron_MicroBooNE_AC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram After C-Cut", "MicroBooNE", "FD", "e", 0.06,
                      0.04, 0.04, plots, 2, false, true, Chi2_Electron_MicroBooNE_Stack, "02_Electron_Chi2_MicroBooNE_AC", Chi2_Electron_MicroBooNE_AC_FD_Dir, kBlue,
                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_FD, Chi2_Electron_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, c2, Chi2_Proton_MicroBooNE_AC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram After C-Cut", "MicroBooNE", "CD", "p", 0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_Proton_MicroBooNE_Stack, "03_Proton_Chi2_MicroBooNE_AC", Chi2_Proton_MicroBooNE_AC_CD_Dir, kBlue, true, true,
                      true, false, true, Chi2_Proton_cut_MicroBooNE_CD, Chi2_Proton_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_Proton_MicroBooNE_AC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram After C-Cut", "MicroBooNE", "FD", "p", 0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_Proton_MicroBooNE_Stack, "04_Proton_Chi2_MicroBooNE_AC", Chi2_Proton_MicroBooNE_AC_FD_Dir, kBlue, true, true,
                      true, false, true, Chi2_Proton_cut_MicroBooNE_FD, Chi2_Proton_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, c2, Chi2_piplus_MicroBooNE_AC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{+}} Histogram After C-Cut", "MicroBooNE", "CD", "#pi^{+}",
                      0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_piplus_MicroBooNE_Stack, "05_piplus_Chi2_MicroBooNE_AC", Chi2_piplus_MicroBooNE_AC_CD_Dir, kBlue, true, true,
                      true, false, true, Chi2_piplus_cut_MicroBooNE_CD, Chi2_piplus_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_piplus_MicroBooNE_AC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{+}} Histogram After C-Cut", "MicroBooNE", "FD", "#pi^{+}",
                      0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_piplus_MicroBooNE_Stack, "06_piplus_Chi2_MicroBooNE_AC", Chi2_piplus_MicroBooNE_AC_FD_Dir, kBlue, true, true,
                      true, false, true, Chi2_piplus_cut_MicroBooNE_FD, Chi2_piplus_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, c2, Chi2_piminus_MicroBooNE_AC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{-}} Histogram After C-Cut", "MicroBooNE", "CD", "#pi^{-}",
                      0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_piminus_MicroBooNE_Stack, "07_piminus_Chi2_MicroBooNE_AC", Chi2_piminus_MicroBooNE_AC_CD_Dir, kBlue, true, true,
                      true, false, true, Chi2_piminus_cut_MicroBooNE_CD, Chi2_piminus_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, c2, Chi2_piminus_MicroBooNE_AC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{-}} Histogram After C-Cut", "MicroBooNE", "FD", "#pi^{-}",
                      0.06, 0.04,
                      0.04, plots, 2, false, true, Chi2_piminus_MicroBooNE_Stack, "08_piminus_Chi2_MicroBooNE_AC", Chi2_piminus_MicroBooNE_AC_FD_Dir, kBlue, true, true,
                      true, false, true, Chi2_piminus_cut_MicroBooNE_FD, Chi2_piminus_1e_peak_MicroBooNE_FD);
        //</editor-fold>

        //<editor-fold desc="Chi2 plots (MicroBooNE)">
        histPlotter1D(c1, Chi2_Electron_MicroBooNE_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, Chi2_Electron_MicroBooNE_Stack, "01_Electron_Chi2_MicroBooNE_CD", Chi2_Electron_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
                      Chi2_Electron_cut_MicroBooNE_CD, Chi2_Electron_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, Chi2_Electron_MicroBooNE_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, Chi2_Electron_MicroBooNE_Stack, "01_Electron_Chi2_MicroBooNE_FD", Chi2_Electron_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      Chi2_Electron_cut_MicroBooNE_FD, Chi2_Electron_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, Chi2_Proton_MicroBooNE_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_MicroBooNE_Stack, "02_Proton_Chi2_MicroBooNE_CD", Chi2_Proton_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
                      Chi2_Proton_cut_MicroBooNE_CD, Chi2_Proton_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, Chi2_Proton_MicroBooNE_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_MicroBooNE_Stack, "02_Proton_Chi2_MicroBooNE_FD", Chi2_Proton_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      Chi2_Proton_cut_MicroBooNE_FD, Chi2_Proton_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, Chi2_piplus_MicroBooNE_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{+}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true,
                      Chi2_piplus_MicroBooNE_Stack, "03_piplus_Chi2_MicroBooNE_CD", Chi2_piplus_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
                      Chi2_piplus_cut_MicroBooNE_CD, Chi2_piplus_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, Chi2_piplus_MicroBooNE_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{+}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true,
                      Chi2_piplus_MicroBooNE_Stack, "03_piplus_Chi2_MicroBooNE_FD", Chi2_piplus_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      Chi2_piplus_cut_MicroBooNE_FD, Chi2_piplus_1e_peak_MicroBooNE_FD);

        histPlotter1D(c1, Chi2_piminus_MicroBooNE_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true,
                      Chi2_piminus_MicroBooNE_Stack, "03_piminus_Chi2_MicroBooNE_CD", Chi2_piminus_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
                      Chi2_piminus_cut_MicroBooNE_CD, Chi2_piminus_1e_peak_MicroBooNE_CD);
        histPlotter1D(c1, Chi2_piminus_MicroBooNE_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{#pi^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true,
                      Chi2_piminus_MicroBooNE_Stack, "03_piminus_Chi2_MicroBooNE_FD", Chi2_piminus_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      Chi2_piminus_cut_MicroBooNE_FD, Chi2_piminus_1e_peak_MicroBooNE_FD);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Testing Chi2 cuts before applying (1e2p)">
        histPlotter1D(c1, c2, Chi2_Electron_1e2p_BC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram Before Cut", "1e2p", "CD", "e", 0.06, 0.04, 0.04,
                      plots, 2, false, true, Chi2_Electron_1e2p_Stack, "01_Electron_Chi2_1e2p_BC", Chi2_Electron_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true,
                      Chi2_Electron_cut_CD, Chi2_Electron_1e_peak_CD);
        histPlotter1D(c1, c2, Chi2_Electron_1e2p_BC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram Before Cut", "1e2p", "FD", "e", 0.06, 0.04, 0.04,
                      plots, 2, false, true, Chi2_Electron_1e2p_Stack, "02_Electron_Chi2_1e2p_BC", Chi2_Electron_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true,
                      Chi2_Electron_cut_FD, Chi2_Electron_1e_peak_FD);

        histPlotter1D(c1, c2, Chi2_Proton_1e2p_BC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram Before Cut", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots, 2,
                      false, true, Chi2_Proton_1e2p_Stack, "03_Proton_Chi2_1e2p_BC", Chi2_Proton_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_1e_peak_CD);
        histPlotter1D(c1, c2, Chi2_Proton_1e2p_BC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram Before Cut", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots, 2,
                      false, true, Chi2_Proton_1e2p_Stack, "04_Proton_Chi2_1e2p_BC", Chi2_Proton_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true, Chi2_Proton_cut_FD,
                      Chi2_Proton_1e_peak_FD);
        //</editor-fold>

        //<editor-fold desc="Testing Chi2 cuts after applying (1e2p)">
        histPlotter1D(c1, c2, Chi2_Electron_1e2p_AC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram After Cut", "1e2p", "CD", "e", 0.06, 0.04, 0.04,
                      plots, 2, false, true, Chi2_Electron_1e2p_Stack, "01_Electron_Chi2_1e2p_AC", Chi2_Electron_1e2p_AC_CD_Dir, kBlue, true, true, true, false, true,
                      Chi2_Electron_cut_CD, Chi2_Electron_1e_peak_CD);
        histPlotter1D(c1, c2, Chi2_Electron_1e2p_AC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram After Cut", "1e2p", "FD", "e", 0.06, 0.04, 0.04,
                      plots, 2, false, true, Chi2_Electron_1e2p_Stack, "02_Electron_Chi2_1e2p_AC", Chi2_Electron_1e2p_AC_FD_Dir, kBlue, true, true, true, false, true,
                      Chi2_Electron_cut_FD, Chi2_Electron_1e_peak_FD);

        histPlotter1D(c1, c2, Chi2_Proton_1e2p_AC_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram After Cut", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots, 2,
                      false, true, Chi2_Proton_1e2p_Stack, "03_Proton_Chi2_1e2p_AC", Chi2_Proton_1e2p_AC_CD_Dir, kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_1e_peak_CD);
        histPlotter1D(c1, c2, Chi2_Proton_1e2p_AC_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram After Cut", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots, 2,
                      false, true, Chi2_Proton_1e2p_Stack, "04_Proton_Chi2_1e2p_AC", Chi2_Proton_1e2p_AC_FD_Dir, kBlue, true, true, true, false, true, Chi2_Proton_cut_FD,
                      Chi2_Proton_1e_peak_FD);
        //</editor-fold>

        //<editor-fold desc="Testing Chi2 (2p)">
        histPlotter1D(c1, Chi2_Electron_2p_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Electron_1e2p_Stack, "01_Electron_Chi2_2p_CD", Chi2_Electron_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Electron_cut_CD,
                      Chi2_Electron_1e_peak_CD);
        histPlotter1D(c1, Chi2_Electron_2p_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{e^{-}} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Electron_1e2p_Stack, "01_Electron_Chi2_2p_FD", Chi2_Electron_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, Chi2_Electron_cut_FD,
                      Chi2_Electron_1e_peak_FD);

        histPlotter1D(c1, Chi2_Proton_2p_CD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_1e2p_Stack, "02_Proton_Chi2_2p_CD", Chi2_Proton_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_1e_peak_CD);
        histPlotter1D(c1, Chi2_Proton_2p_FD, normalized_chi2_plots, true, .1, "#chi^{2}_{p} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Chi2_Proton_1e2p_Stack, "02_Proton_Chi2_2p_FD", Chi2_Proton_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, Chi2_Proton_cut_FD,
                      Chi2_Proton_1e_peak_FD);
        //</editor-fold>

    } else {
        cout << "\n\nChi2 plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Vertex plots
// ======================================================================================================================================================================

    //<editor-fold desc="Vertex plots">
    if (Vertex_plots) {

        cout << "\n\nPlotting Vertex plots...\n\n";

        //<editor-fold desc="Finding Xmax">
        dVx_Xmax = deltaVx_before_dV_cuts_1e2p->GetBinCenter(deltaVx_before_dV_cuts_1e2p->GetMaximumBin());
        dVy_Xmax = deltaVy_before_dV_cuts_1e2p->GetBinCenter(deltaVy_before_dV_cuts_1e2p->GetMaximumBin());
        dVz_Xmax = deltaVz_before_dV_cuts_1e2p->GetBinCenter(deltaVz_before_dV_cuts_1e2p->GetMaximumBin());
        //</editor-fold>

//  dV plots (1e2p, CD & FD) ----------------------------------------------------------------------------

        //<editor-fold desc="dV plots (1e2p, CD & FD)">

        //<editor-fold desc="dV plots before dV cuts (1e2p, CD & FD)">
        histPlotter1D(c1, deltaVx_before_dV_cuts_1e2p, normalized_vertex_plots, true, .1, "dV_{x}=V^{e}_{x}-V^{p}_{x} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVx_1e2p_before_Stack, "01_dVx_BC", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                      dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_before_dV_cuts_1e2p, normalized_vertex_plots, true, .1, "dV_{y}=V^{e}_{y}-V^{p}_{y} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVy_1e2p_before_Stack, "02_dVy_BC", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                      dVy_cut, dVy_peak);

        histPlotter1D(c1, deltaVz_before_dV_cuts_1e2p, normalized_vertex_plots, true, .1, "dV_{z}=V^{e}_{z}-V^{p}_{z} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVz_1e2p_before_Stack, "03_dVz_BC", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                      dVz_cut, dVz_peak);

        histPlotter1D(c1, deltaVx_after_dV_cuts_1e2p, normalized_vertex_plots, true, .1, "dV_{x}=V^{e}_{x}-V^{p}_{x} After Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVx_1e2p_after_Stack, "01_dVx_AC", Vertex_1e2p_dV_AC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                      dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_after_dV_cuts_1e2p, normalized_vertex_plots, true, .1, "dV_{y}=V^{e}_{y}-V^{p}_{y} After Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVy_1e2p_after_Stack, "02_dVy_AC", Vertex_1e2p_dV_AC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                      dVy_cut, dVy_peak);

        histPlotter1D(c1, deltaVz_after_dV_cuts_1e2p, normalized_vertex_plots, true, .1, "dV_{z}=V^{e}_{z}-V^{p}_{z} After Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVz_1e2p_after_Stack, "03_dVz_AC", Vertex_1e2p_dV_AC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                      dVz_cut, dVz_peak);
        //</editor-fold>

        //<editor-fold desc="dV plots after dV cuts (2p, CD & FD)">
        histPlotter1D(c1, deltaVx_2p, normalized_vertex_plots, true, .1, "dV_{x}=V^{e}_{x}-V^{p}_{x}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, dVx_2p_Stack,
                      "01_dVx", Vertex_dV_2p_Directory, "2p", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_2p, normalized_vertex_plots, true, .1, "dV_{y}=V^{e}_{y}-V^{p}_{y}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, dVy_2p_Stack,
                      "02_dVy", Vertex_dV_2p_Directory, "2p", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);

        histPlotter1D(c1, deltaVz_2p, normalized_vertex_plots, true, .1, "dV_{z}=V^{e}_{z}-V^{p}_{z}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, dVz_2p_Stack,
                      "03_dVz", Vertex_dV_2p_Directory, "2p", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="dV plots before dV cuts (MicroBooNE, CD & FD)">
        histPlotter1D(c1, deltaVx_MicroBooNE_BC, normalized_vertex_plots, true, .1, "dV_{x}=V^{e}_{x}-V^{p}_{x} Before Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVx_MicroBooNE_Stack, "01_dVx_MicroBooNE_BC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
                      false, true, dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_MicroBooNE_BC, normalized_vertex_plots, true, .1, "dV_{y}=V^{e}_{y}-V^{p}_{y} Before Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVy_MicroBooNE_Stack, "02_dVy_MicroBooNE_BC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
                      false, true, dVy_cut, dVy_peak);

        histPlotter1D(c1, deltaVz_MicroBooNE_BC, normalized_vertex_plots, true, .1, "dV_{z}=V^{e}_{z}-V^{p}_{z} Before Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVz_MicroBooNE_Stack, "03_dVz_MicroBooNE_BC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
                      false, true, dVz_cut, dVz_peak);

        histPlotter1D(c1, deltaVx_MicroBooNE_AC, normalized_vertex_plots, true, .1, "dV_{x}=V^{e}_{x}-V^{p}_{x} After Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVx_MicroBooNE_Stack, "01_dVx_MicroBooNE_AC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
                      false, true, dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_MicroBooNE_AC, normalized_vertex_plots, true, .1, "dV_{y}=V^{e}_{y}-V^{p}_{y} After Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVy_MicroBooNE_Stack, "02_dVy_MicroBooNE_AC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
                      false, true, dVy_cut, dVy_peak);

        histPlotter1D(c1, deltaVz_MicroBooNE_AC, normalized_vertex_plots, true, .1, "dV_{z}=V^{e}_{z}-V^{p}_{z} After Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, dVz_MicroBooNE_Stack, "03_dVz_MicroBooNE_AC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
                      false, true, dVz_cut, dVz_peak);
        //</editor-fold>

        //<editor-fold desc="dV plots after dV cuts (MicroBooNE, CD & FD)">
        histPlotter1D(c1, deltaVx_MicroBooNE, normalized_vertex_plots, true, .1, "dV_{x}=V^{e}_{x}-V^{p}_{x}", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      dVx_MicroBooNE_Stack, "01_dVx_MicroBooNE", Vertex_dV_MicroBooNE_Directory, "MicroBooNE", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_MicroBooNE, normalized_vertex_plots, true, .1, "dV_{y}=V^{e}_{y}-V^{p}_{y}", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      dVy_MicroBooNE_Stack, "02_dVy_MicroBooNE", Vertex_dV_MicroBooNE_Directory, "MicroBooNE", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);

        histPlotter1D(c1, deltaVz_MicroBooNE, normalized_vertex_plots, true, .1, "dV_{z}=V^{e}_{z}-V^{p}_{z}", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      dVz_MicroBooNE_Stack, "03_dVz_MicroBooNE", Vertex_dV_MicroBooNE_Directory, "MicroBooNE", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        //</editor-fold>

    } else {
        cout << "\n\nVertex plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Angle histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Angle histograms">
    if (Theta_e_plots) {

        cout << "\n\nPlotting Angle histograms...\n\n";

//  Theta_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Theta_e (no #(e) cut)">
        double Theta_e_integral = Theta_e_CD->Integral() + Theta_e_FD->Integral();

        //<editor-fold desc="Theta of electron (CD)">
        histPlotter1D(c1, Theta_e_CD, normalized_Angle_plots, true, Theta_e_integral, "#theta_{e}", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Theta_e_Stack, "Theta_e_All_e", Theta_e_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta of electron (FD)">
        histPlotter1D(c1, Theta_e_FD, normalized_Angle_plots, true, Theta_e_integral, "#theta_{e}", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Theta_e_Stack, "Theta_e_All_e", Theta_e_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Theta_e (1e Only Cut)">
        double Theta_e_1e_integral = Theta_e_1e_CD->Integral() + Theta_e_1e_FD->Integral();

        //<editor-fold desc="Theta of electron (CD)">
        histPlotter1D(c1, Theta_e_1e_CD, normalized_Angle_plots, true, Theta_e_1e_integral, "#theta_{e}", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Theta_e_Stack, "Theta_e_Only_1e_cut", Theta_e_1e_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta of electron (FD)">
        histPlotter1D(c1, Theta_e_1e_FD, normalized_Angle_plots, true, Theta_e_1e_integral, "#theta_{e}", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Theta_e_Stack, "Theta_e_Only_1e_cut", Theta_e_1e_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Theta_e 1e2X (1e Only Cut)">
        double Theta_e_1e_1e2X_integral = Theta_e_1e2X_CD->Integral() + Theta_e_1e2X_FD->Integral();
        double Theta_e_1e_1e2X_QEL_integral = Theta_e_1e2X_QEL_CD->Integral() + Theta_e_1e2X_QEL_FD->Integral();
        double Theta_e_1e_1e2X_MEC_integral = Theta_e_1e2X_MEC_CD->Integral() + Theta_e_1e2X_MEC_FD->Integral();
        double Theta_e_1e_1e2X_RES_integral = Theta_e_1e2X_RES_CD->Integral() + Theta_e_1e2X_RES_FD->Integral();
        double Theta_e_1e_1e2X_DIS_integral = Theta_e_1e2X_DIS_CD->Integral() + Theta_e_1e2X_DIS_FD->Integral();

        //<editor-fold desc="Theta_e 1e2X (CD)">
        histPlotter1D(c1, Theta_e_1e2X_CD, normalized_Angle_plots, true, Theta_e_1e_1e2X_integral, "#theta_{e} 1e2X", "All int.", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "00_Theta_e_1e2X", Theta_e_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2X_QEL_CD, normalized_Angle_plots, true, Theta_e_1e_1e2X_QEL_integral, "#theta_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "01_Theta_e_1e2X_QEL_Only", Theta_e_1e2X_QEL_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2X_MEC_CD, normalized_Angle_plots, true, Theta_e_1e_1e2X_MEC_integral, "#theta_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "02_Theta_e_1e2X_MEC_Only", Theta_e_1e2X_MEC_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2X_RES_CD, normalized_Angle_plots, true, Theta_e_1e_1e2X_RES_integral, "#theta_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "03_Theta_e_1e2X_RES_Only", Theta_e_1e2X_RES_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2X_DIS_CD, normalized_Angle_plots, true, Theta_e_1e_1e2X_DIS_integral, "#theta_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "04_Theta_e_1e2X_DIS_Only", Theta_e_1e2X_DIS_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta_e 1e2X (FD)">
        histPlotter1D(c1, Theta_e_1e2X_FD, normalized_Angle_plots, true, Theta_e_1e_1e2X_integral, "#theta_{e} 1e2X", "All int.", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "00_Theta_e_Only_1e_cut_1e2X", Theta_e_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2X_QEL_FD, normalized_Angle_plots, true, Theta_e_1e_1e2X_QEL_integral, "#theta_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "01_Theta_e_1e2X_QEL_Only", Theta_e_1e2X_QEL_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2X_MEC_FD, normalized_Angle_plots, true, Theta_e_1e_1e2X_MEC_integral, "#theta_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "02_Theta_e_1e2X_MEC_Only", Theta_e_1e2X_MEC_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2X_RES_FD, normalized_Angle_plots, true, Theta_e_1e_1e2X_RES_integral, "#theta_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "03_Theta_e_1e2X_RES_Only", Theta_e_1e2X_RES_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2X_DIS_FD, normalized_Angle_plots, true, Theta_e_1e_1e2X_DIS_integral, "#theta_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "04_Theta_e_1e2X_DIS_Only", Theta_e_1e2X_DIS_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Theta_e 1e2p (1e Only Cut)">
        double Theta_e_1e_1e2p_integral = Theta_e_1e2p_CD->Integral() + Theta_e_1e2p_FD->Integral();
        double Theta_e_1e_1e2p_QEL_integral = Theta_e_1e2p_QEL_CD->Integral() + Theta_e_1e2p_QEL_FD->Integral();
        double Theta_e_1e_1e2p_MEC_integral = Theta_e_1e2p_MEC_CD->Integral() + Theta_e_1e2p_MEC_FD->Integral();
        double Theta_e_1e_1e2p_RES_integral = Theta_e_1e2p_RES_CD->Integral() + Theta_e_1e2p_RES_FD->Integral();
        double Theta_e_1e_1e2p_DIS_integral = Theta_e_1e2p_DIS_CD->Integral() + Theta_e_1e2p_DIS_FD->Integral();
        double Theta_e_1e_2p_integral = Theta_e_2p_CD->Integral() + Theta_e_2p_FD->Integral();
        double Theta_e_1e_2p_QEL_integral = Theta_e_2p_QEL_CD->Integral() + Theta_e_2p_QEL_FD->Integral();
        double Theta_e_1e_2p_MEC_integral = Theta_e_2p_MEC_CD->Integral() + Theta_e_2p_MEC_FD->Integral();
        double Theta_e_1e_2p_RES_integral = Theta_e_2p_RES_CD->Integral() + Theta_e_2p_RES_FD->Integral();
        double Theta_e_1e_2p_DIS_integral = Theta_e_2p_DIS_CD->Integral() + Theta_e_2p_DIS_FD->Integral();

        //<editor-fold desc="Theta_e 1e2p (CD)">
        histPlotter1D(c1, Theta_e_1e2p_CD, normalized_Angle_plots, true, Theta_e_1e_1e2p_integral, "#theta_{e} 1e2p", "All int.", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "00_Theta_e_1e2p", Theta_e_1e2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2p_QEL_CD, normalized_Angle_plots, true, Theta_e_1e_1e2p_QEL_integral, "#theta_{e} 1e2p", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "01_Theta_e_1e2p_QEL_Only", Theta_e_1e2p_QEL_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2p_MEC_CD, normalized_Angle_plots, true, Theta_e_1e_1e2p_MEC_integral, "#theta_{e} 1e2p", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "02_Theta_e_1e2p_MEC_Only", Theta_e_1e2p_MEC_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2p_RES_CD, normalized_Angle_plots, true, Theta_e_1e_1e2p_RES_integral, "#theta_{e} 1e2p", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "03_Theta_e_1e2p_RES_Only", Theta_e_1e2p_RES_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_1e2p_DIS_CD, normalized_Angle_plots, true, Theta_e_1e_1e2p_DIS_integral, "#theta_{e} 1e2p", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "04_Theta_e_1e2p_DIS_Only", Theta_e_1e2p_DIS_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta_e 1e2p (FD)">
        histPlotter1D(c1, Theta_e_1e2p_FD, normalized_Angle_plots, true, Theta_e_1e_1e2p_integral, "#theta_{e} 1e2p", "All int.", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "00_Theta_e_1e2p", Theta_e_1e2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2p_QEL_FD, normalized_Angle_plots, true, Theta_e_1e_1e2p_QEL_integral, "#theta_{e} 1e2p", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "01_Theta_e_1e2p_QEL_Only", Theta_e_1e2p_QEL_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2p_MEC_FD, normalized_Angle_plots, true, Theta_e_1e_1e2p_MEC_integral, "#theta_{e} 1e2p", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "02_Theta_e_1e2p_MEC_Only", Theta_e_1e2p_MEC_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2p_RES_FD, normalized_Angle_plots, true, Theta_e_1e_1e2p_RES_integral, "#theta_{e} 1e2p", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "03_Theta_e_1e2p_RES_Only", Theta_e_1e2p_RES_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_1e2p_DIS_FD, normalized_Angle_plots, true, Theta_e_1e_1e2p_DIS_integral, "#theta_{e} 1e2p", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "04_Theta_e_1e2p_DIS_Only", Theta_e_1e2p_DIS_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta_e 2p (CD)">
        histPlotter1D(c1, Theta_e_2p_CD, normalized_Angle_plots, true, Theta_e_1e_2p_integral, "#theta_{e}", "All int., 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "00_Theta_e_2p", Theta_e_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_2p_QEL_CD, normalized_Angle_plots, true, Theta_e_1e_2p_QEL_integral, "#theta_{e}", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "01_Theta_e_2p_QEL_Only", Theta_e_2p_QEL_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_2p_MEC_CD, normalized_Angle_plots, true, Theta_e_1e_2p_MEC_integral, "#theta_{e}", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "02_Theta_e_2p_MEC_Only", Theta_e_2p_MEC_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_2p_RES_CD, normalized_Angle_plots, true, Theta_e_1e_2p_RES_integral, "#theta_{e}", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "03_Theta_e_2p_RES_Only", Theta_e_2p_RES_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);

        histPlotter1D(c1, Theta_e_2p_DIS_CD, normalized_Angle_plots, true, Theta_e_1e_2p_DIS_integral, "#theta_{e}", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "04_Theta_e_2p_DIS_Only", Theta_e_2p_DIS_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta_e 2p (FD)">
        histPlotter1D(c1, Theta_e_2p_FD, normalized_Angle_plots, true, Theta_e_1e_2p_integral, "#theta_{e}", "All int., 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "00_Theta_e_2p", Theta_e_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_2p_QEL_FD, normalized_Angle_plots, true, Theta_e_1e_2p_QEL_integral, "#theta_{e}", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "01_Theta_e_2p_QEL_Only", Theta_e_2p_QEL_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_2p_MEC_FD, normalized_Angle_plots, true, Theta_e_1e_2p_MEC_integral, "#theta_{e}", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "02_Theta_e_2p_MEC_Only", Theta_e_2p_MEC_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_2p_RES_FD, normalized_Angle_plots, true, Theta_e_1e_2p_RES_integral, "#theta_{e}", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "03_Theta_e_2p_RES_Only", Theta_e_2p_RES_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);

        histPlotter1D(c1, Theta_e_2p_DIS_FD, normalized_Angle_plots, true, Theta_e_1e_2p_DIS_integral, "#theta_{e}", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Theta_e_Stack, "04_Theta_e_2p_DIS_Only", Theta_e_2p_DIS_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
        //</editor-fold>

        //</editor-fold>

//  Phi_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Phi_e (no #(e) cut)">
        double Phi_e_integral = Phi_e_CD->Integral() + Phi_e_FD->Integral();

        //<editor-fold desc="Phi of electron (CD)">
        histPlotter1D(c1, Phi_e_CD, normalized_Angle_plots, true, Phi_e_integral, "#phi_{e}", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "Phi_e_All_e", Phi_e_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi of electron (FD)">
        histPlotter1D(c1, Phi_e_FD, normalized_Angle_plots, true, Phi_e_integral, "#phi_{e}", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "Phi_e_All_e", Phi_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Phi_e (1e Only Cut)">
        double Phi_e_1e_integral = Phi_e_1e_CD->Integral() + Phi_e_1e_FD->Integral();

        //<editor-fold desc="Phi of electron (CD)">
        histPlotter1D(c1, Phi_e_1e_CD, normalized_Angle_plots, true, Phi_e_1e_integral, "#phi_{e}", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "Phi_e_Only_1e_cut", Phi_e_1e_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi of electron (FD)">
        histPlotter1D(c1, Phi_e_1e_FD, normalized_Angle_plots, true, Phi_e_1e_integral, "#phi_{e}", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "Phi_e_Only_1e_cut", Phi_e_1e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Phi_e 1e2X">
        double Phi_e_1e_1e2X_integral = Phi_e_1e2X_CD->Integral() + Phi_e_1e2X_FD->Integral();
        double Phi_e_1e_1e2X_QEL_integral = Phi_e_1e2X_QEL_CD->Integral() + Phi_e_1e2X_QEL_FD->Integral();
        double Phi_e_1e_1e2X_MEC_integral = Phi_e_1e2X_MEC_CD->Integral() + Phi_e_1e2X_MEC_FD->Integral();
        double Phi_e_1e_1e2X_RES_integral = Phi_e_1e2X_RES_CD->Integral() + Phi_e_1e2X_RES_FD->Integral();
        double Phi_e_1e_1e2X_DIS_integral = Phi_e_1e2X_DIS_CD->Integral() + Phi_e_1e2X_DIS_FD->Integral();

        //<editor-fold desc="Phi_e 1e2X (CD)">
        histPlotter1D(c1, Phi_e_1e2X_CD, normalized_Angle_plots, true, Phi_e_1e_1e2X_integral, "#phi_{e} 1e2X", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, Phi_e_Stack, "00_phi_e_Only_1e_cut_1e2X", Phi_e_1e2X_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_QEL_CD, normalized_Angle_plots, true, Phi_e_1e_1e2X_QEL_integral, "#phi_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "01_Phi_e_1e2X_QEL_Only", Phi_e_1e2X_QEL_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_MEC_CD, normalized_Angle_plots, true, Phi_e_1e_1e2X_MEC_integral, "#phi_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "02_Phi_e_1e2X_MEC_Only", Phi_e_1e2X_MEC_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_RES_CD, normalized_Angle_plots, true, Phi_e_1e_1e2X_RES_integral, "#phi_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "03_Phi_e_1e2X_RES_Only", Phi_e_1e2X_RES_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_DIS_CD, normalized_Angle_plots, true, Phi_e_1e_1e2X_DIS_integral, "#phi_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "04_Phi_e_1e2X_DIS_Only", Phi_e_1e2X_DIS_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e 1e2X (FD)">
        histPlotter1D(c1, Phi_e_1e2X_FD, normalized_Angle_plots, true, Phi_e_1e_1e2X_integral, "#phi_{e} 1e2X", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, Phi_e_Stack, "00_phi_e_Only_1e_cut_1e2X", Phi_e_1e2X_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_QEL_FD, normalized_Angle_plots, true, Phi_e_1e_1e2X_QEL_integral, "#phi_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "01_Phi_e_1e2X_QEL_Only", Phi_e_1e2X_QEL_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_MEC_FD, normalized_Angle_plots, true, Phi_e_1e_1e2X_MEC_integral, "#phi_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "02_Phi_e_1e2X_MEC_Only", Phi_e_1e2X_MEC_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_RES_FD, normalized_Angle_plots, true, Phi_e_1e_1e2X_RES_integral, "#phi_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "03_Phi_e_1e2X_RES_Only", Phi_e_1e2X_RES_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2X_DIS_FD, normalized_Angle_plots, true, Phi_e_1e_1e2X_DIS_integral, "#phi_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "04_Phi_e_1e2X_DIS_Only", Phi_e_1e2X_DIS_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Phi_e 1e2p">
        double Phi_e_1e_1e2p_integral = Phi_e_1e2p_CD->Integral() + Phi_e_1e2p_FD->Integral();
        double Phi_e_1e_1e2p_QEL_integral = Phi_e_1e2p_QEL_CD->Integral() + Phi_e_1e2p_QEL_FD->Integral();
        double Phi_e_1e_1e2p_MEC_integral = Phi_e_1e2p_MEC_CD->Integral() + Phi_e_1e2p_MEC_FD->Integral();
        double Phi_e_1e_1e2p_RES_integral = Phi_e_1e2p_RES_CD->Integral() + Phi_e_1e2p_RES_FD->Integral();
        double Phi_e_1e_1e2p_DIS_integral = Phi_e_1e2p_DIS_CD->Integral() + Phi_e_1e2p_DIS_FD->Integral();

        //<editor-fold desc="Phi_e 1e2p (CD)">
        histPlotter1D(c1, Phi_e_1e2p_CD, normalized_Angle_plots, true, Phi_e_1e_1e2p_integral, "#phi_{e} 1e2p", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, Phi_e_Stack, "00_phi_e_Only_1e_cut_1e2p", Phi_e_1e2p_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_QEL_CD, normalized_Angle_plots, true, Phi_e_1e_1e2p_QEL_integral, "#phi_{e} 1e2p", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "01_Phi_e_1e2p_QEL_Only", Phi_e_1e2p_QEL_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_MEC_CD, normalized_Angle_plots, true, Phi_e_1e_1e2p_MEC_integral, "#phi_{e} 1e2p", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "02_Phi_e_1e2p_MEC_Only", Phi_e_1e2p_MEC_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_RES_CD, normalized_Angle_plots, true, Phi_e_1e_1e2p_RES_integral, "#phi_{e} 1e2p", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "03_Phi_e_1e2p_RES_Only", Phi_e_1e2p_RES_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_DIS_CD, normalized_Angle_plots, true, Phi_e_1e_1e2p_DIS_integral, "#phi_{e} 1e2p", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "04_Phi_e_1e2p_DIS_Only", Phi_e_1e2p_DIS_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e 1e2p (FD)">
        histPlotter1D(c1, Phi_e_1e2p_FD, normalized_Angle_plots, true, Phi_e_1e_1e2p_integral, "#phi_{e} 1e2p", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, Phi_e_Stack, "00_phi_e_Only_1e_cut_1e2p", Phi_e_1e2p_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_QEL_FD, normalized_Angle_plots, true, Phi_e_1e_1e2p_QEL_integral, "#phi_{e} 1e2p", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "01_Phi_e_1e2p_QEL_Only", Phi_e_1e2p_QEL_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_MEC_FD, normalized_Angle_plots, true, Phi_e_1e_1e2p_MEC_integral, "#phi_{e} 1e2p", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "02_Phi_e_1e2p_MEC_Only", Phi_e_1e2p_MEC_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_RES_FD, normalized_Angle_plots, true, Phi_e_1e_1e2p_RES_integral, "#phi_{e} 1e2p", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "03_Phi_e_1e2p_RES_Only", Phi_e_1e2p_RES_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_1e2p_DIS_FD, normalized_Angle_plots, true, Phi_e_1e_1e2p_DIS_integral, "#phi_{e} 1e2p", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
                      false, true, Phi_e_Stack, "04_Phi_e_1e2p_DIS_Only", Phi_e_1e2p_DIS_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Phi_e 2p">
        double Phi_e_2p_integral = Phi_e_2p_CD->Integral() + Phi_e_2p_FD->Integral();
        double Phi_e_2p_QEL_integral = Phi_e_2p_QEL_CD->Integral() + Phi_e_2p_QEL_FD->Integral();
        double Phi_e_2p_MEC_integral = Phi_e_2p_MEC_CD->Integral() + Phi_e_2p_MEC_FD->Integral();
        double Phi_e_2p_RES_integral = Phi_e_2p_RES_CD->Integral() + Phi_e_2p_RES_FD->Integral();
        double Phi_e_2p_DIS_integral = Phi_e_2p_DIS_CD->Integral() + Phi_e_2p_DIS_FD->Integral();

        //<editor-fold desc="Phi_e 2p (CD)">
        histPlotter1D(c1, Phi_e_2p_CD, normalized_Angle_plots, true, Phi_e_2p_integral, "#phi_{e}", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "00_phi_e_All_Int_2p", Phi_e_2p_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_QEL_CD, normalized_Angle_plots, true, Phi_e_2p_QEL_integral, "#phi_{e}", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "01_Phi_e_2p_QEL_Only", Phi_e_2p_QEL_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_MEC_CD, normalized_Angle_plots, true, Phi_e_2p_MEC_integral, "#phi_{e}", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "02_Phi_e_2p_MEC_Only", Phi_e_2p_MEC_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_RES_CD, normalized_Angle_plots, true, Phi_e_2p_RES_integral, "#phi_{e}", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "03_Phi_e_2p_RES_Only", Phi_e_2p_RES_CD_Dir, "CD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_DIS_CD, normalized_Angle_plots, true, Phi_e_2p_DIS_integral, "#phi_{e}", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "04_Phi_e_2p_DIS_Only", Phi_e_2p_DIS_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e 2p (FD)">
        histPlotter1D(c1, Phi_e_2p_FD, normalized_Angle_plots, true, Phi_e_2p_integral, "#phi_{e}", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "00_phi_e_All_Int_cut_2p", Phi_e_2p_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_QEL_FD, normalized_Angle_plots, true, Phi_e_2p_QEL_integral, "#phi_{e}", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "01_Phi_e_2p_QEL_Only", Phi_e_2p_QEL_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_MEC_FD, normalized_Angle_plots, true, Phi_e_2p_MEC_integral, "#phi_{e}", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "02_Phi_e_2p_MEC_Only", Phi_e_2p_MEC_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_RES_FD, normalized_Angle_plots, true, Phi_e_2p_RES_integral, "#phi_{e}", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "03_Phi_e_2p_RES_Only", Phi_e_2p_RES_FD_Dir, "FD", kBlue, true, true, true);

        histPlotter1D(c1, Phi_e_2p_DIS_FD, normalized_Angle_plots, true, Phi_e_2p_DIS_integral, "#phi_{e}", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Phi_e_Stack, "04_Phi_e_2p_DIS_Only", Phi_e_2p_DIS_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

//  Theta_e vs. Phi_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Theta_e vs. Phi_e (CD & FD)">
        Theta_e_VS_Phi_e_CD->SetTitleSize(0.06, "xyz");
        Theta_e_VS_Phi_e_CD->GetXaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_CD->GetXaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_CD->GetYaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_CD->GetYaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Theta_e_VS_Phi_e_CD);
        Theta_e_VS_Phi_e_CD->Draw("colz");
        c1->SetLogz(0);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Theta_e_VS_Phi_e_CD->SetStats(0);
        c1->SaveAs((Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + "Theta_e_VS_Phi_e_All_e_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();

        Theta_e_VS_Phi_e_FD->SetTitleSize(0.06, "xyz");
        Theta_e_VS_Phi_e_FD->GetXaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_FD->GetXaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_FD->GetYaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_FD->GetYaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Theta_e_VS_Phi_e_FD);
        Theta_e_VS_Phi_e_FD->Draw("colz");
        c1->SetLogz(0);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Theta_e_VS_Phi_e_FD->SetStats(0);
        c1->SaveAs((Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + "Theta_e_VS_Phi_e_All_e_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();

        Theta_e_VS_Phi_e_1e_CD->SetTitleSize(0.06, "xyz");
        Theta_e_VS_Phi_e_1e_CD->GetXaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_1e_CD->GetXaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_1e_CD->GetYaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_1e_CD->GetYaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_1e_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Theta_e_VS_Phi_e_1e_CD);
        Theta_e_VS_Phi_e_1e_CD->Draw("colz");
        c1->SetLogz(0);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Theta_e_VS_Phi_e_1e_CD->SetStats(0);
        c1->SaveAs((Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + "Theta_e_VS_Phi_e_Only_1e_cut_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();

        Theta_e_VS_Phi_e_1e_FD->SetTitleSize(0.06, "xyz");
        Theta_e_VS_Phi_e_1e_FD->GetXaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_1e_FD->GetXaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_1e_FD->GetYaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_1e_FD->GetYaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_1e_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Theta_e_VS_Phi_e_1e_FD);
        Theta_e_VS_Phi_e_1e_FD->Draw("colz");
        c1->SetLogz(0);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Theta_e_VS_Phi_e_1e_FD->SetStats(0);
        c1->SaveAs((Plots_Folder + "/" + Theta_e_Parent_Directory + "/" + "Theta_e_VS_Phi_e_Only_1e_cut_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e (2p, CD & FD)">
        Theta_e_VS_Phi_e_2p_CD->SetTitleSize(0.06, "xyz");
        Theta_e_VS_Phi_e_2p_CD->GetXaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_2p_CD->GetXaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_2p_CD->GetYaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_2p_CD->GetYaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_2p_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Theta_e_VS_Phi_e_2p_CD);
        Theta_e_VS_Phi_e_2p_CD->Draw("colz");
        c1->SetLogz(0);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Theta_e_VS_Phi_e_2p_CD->SetStats(0);
        c1->SaveAs((Theta_e_VS_Phi_e_2p_CD_Dir + "Theta_e_VS_Phi_e_All_Int_2p_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();

        Theta_e_VS_Phi_e_2p_FD->SetTitleSize(0.06, "xyz");
        Theta_e_VS_Phi_e_2p_FD->GetXaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_2p_FD->GetXaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_2p_FD->GetYaxis()->SetLabelSize(0.0425);
        Theta_e_VS_Phi_e_2p_FD->GetYaxis()->CenterTitle(true);
        Theta_e_VS_Phi_e_2p_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(Theta_e_VS_Phi_e_2p_FD);
        Theta_e_VS_Phi_e_2p_FD->Draw("colz");
        c1->SetLogz(0);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Theta_e_VS_Phi_e_2p_FD->SetStats(0);
        c1->SaveAs((Theta_e_VS_Phi_e_2p_FD_Dir + "Theta_e_VS_Phi_e_All_Int_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
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

        //<editor-fold desc="Q2 (no #(e) cut)">
        double Q2_All_e_integral = Q2_histogram_CD->Integral() + Q2_histogram_FD->Integral();

        //<editor-fold desc="Q2 (no #(e) cut, CD)">
        histPlotter1D(c1, Q2_histogram_CD, normalized_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_Stack, "Q2_All_e", Q2_histogram_CD_Dir.c_str(), "CD", kBlue, true, true, true);
//        histPlotter1D(c1, Q2_histogram_CD, normalized_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut",
//                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_Stack, "Q2_All_e", "plots/Q2_histograms/", "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 (no #(e) cut, FD)">
        histPlotter1D(c1, Q2_histogram_FD, normalized_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_Stack, "Q2_All_e", Q2_histogram_FD_Dir.c_str(), "FD", kBlue, true, true, true);
//        histPlotter1D(c1, Q2_histogram_FD, normalized_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut",
//                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_Stack, "Q2_All_e", "plots/Q2_histograms/", "FD", kBlue, true, true, true);
        //</editor-fold>
        //</editor-fold>

        //<editor-fold desc="Q2 (1e Only Cut)">
        double Q2_1e_integral = Q2_histogram_1e_CD->Integral() + Q2_histogram_1e_FD->Integral();

        //<editor-fold desc="Q2 (1e Only Cut, CD)">
        histPlotter1D(c1, Q2_histogram_1e_CD, normalized_Q2_plots, true, Q2_1e_integral, "Q^{2} Histogram", "1e Only Cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_1e2X_Stack, "Only_1e_cut", Q2_histogram_1e_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 (1e Only Cut, FD)">
        histPlotter1D(c1, Q2_histogram_1e_FD, normalized_Q2_plots, true, Q2_1e_integral, "Q^{2} Histogram", "1e Only Cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_1e2X_Stack, "Only_1e_cut", Q2_histogram_1e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>
        //</editor-fold>

        //<editor-fold desc="Q2 1e2X (1e Only Cut)">
        double Q2_1e2X_integral = Q2_histogram_1e2X_CD->Integral() + Q2_histogram_1e2X_FD->Integral();

        //<editor-fold desc="Q2 1e2X (CD)">
        histPlotter1D(c1, Q2_histogram_1e2X_CD, normalized_Q2_plots, true, Q2_1e2X_integral, "Q^{2} 1e2X Histogram", "1e Only Cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_1e2X_Stack, "Q2_1e2X", Q2_histogram_1e2X_CD_Dir.c_str(), "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 1e2X (FD)">
        histPlotter1D(c1, Q2_histogram_1e2X_FD, normalized_Q2_plots, true, Q2_1e2X_integral, "Q^{2} 1e2X Histogram", "1e Only Cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_1e2X_Stack, "Q2_1e2X", Q2_histogram_1e2X_FD_Dir.c_str(), "FD", kBlue, true, true, true);
        //</editor-fold>
        //</editor-fold>

        //<editor-fold desc="Q2 1e2p (1e Only Cut)">
        double Q2_1e2p_integral = Q2_histogram_1e2p_CD->Integral() + Q2_histogram_1e2p_FD->Integral();

        //<editor-fold desc="Q2 1e2p (CD)">
        histPlotter1D(c1, Q2_histogram_1e2p_CD, normalized_Q2_plots, true, Q2_1e2p_integral, "Q^{2} 1e2p Histogram", "1e Only Cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_1e2p_Stack, "Q2_1e2p", Q2_histogram_1e2p_CD_Dir.c_str(), "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 1e2p (FD)">
        histPlotter1D(c1, Q2_histogram_1e2p_FD, normalized_Q2_plots, true, Q2_1e2p_integral, "Q^{2} 1e2p Histogram", "1e Only Cut",
                      0.06, 0.0425, 0.0425, plots, 2, false, true, Q2_1e2p_Stack, "Q2_1e2p", Q2_histogram_1e2p_FD_Dir.c_str(), "FD", kBlue, true, true, true);
        //</editor-fold>
        //</editor-fold>

    } else {
        cout << "\n\nQ2 plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Momentum histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Momentum histograms">
    if (Momentum_plots) {

        cout << "\n\nPlotting Momentum histograms...\n\n";

//  Momentum histograms (1e2p, CD & FD)  --------------------------------------------------------------

//        double Momentum_integral = P_e_histogram_CD->Integral() + P_e_histogram_FD->Integral();

        //<editor-fold desc="Momentum histograms before cuts (1e2p, CD & FD)">

        //<editor-fold desc="Electrons momentum histograms (1e2p, CD & FD)">
        histPlotter1D(c1, P_e_1e2p_BC_CD, false, true, 1., "P_{e} Before Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_BC_CD, "01_P_e_1e2p_BC_CD", P_e_1e2p_BC_CD_Dir,
                      "CD", kBlue, true, true, false, true, e_momentum_upper_cut_2p, e_momentum_lower_cut_2p, 0, false);

        histPlotter1D(c1, P_e_1e2p_BC_FD, false, true, 1., "P_{e} Before Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_BC_FD, "01_P_e_1e2p_BC_FD", P_e_1e2p_BC_FD_Dir,
                      "FD", kBlue, true, true, false, true, e_momentum_upper_cut_2p, e_momentum_lower_cut_2p, 0, false);
        //</editor-fold>

        //<editor-fold desc="Protons momentum histograms (1e2p, CD & FD)">
        histPlotter1D(c1, P_p_1e2p_BC_CD, false, true, 1., "P_{p} Before Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_BC_CD, "02_P_p_1e2p_BC_CD", P_p_1e2p_BC_CD_Dir,
                      "CD", kBlue, true, true, false, true, p_momentum_upper_cut_2p, p_momentum_lower_cut_2p, 0, false);

        histPlotter1D(c1, P_p_1e2p_BC_FD, false, true, 1., "P_{p} Before Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_BC_FD, "02_P_p_1e2p_BC_FD", P_p_1e2p_BC_FD_Dir,
                      "FD", kBlue, true, true, false, true, p_momentum_upper_cut_2p, p_momentum_lower_cut_2p, 0, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms after cuts (1e2p, CD & FD)">

        //<editor-fold desc="Electrons momentum histograms (1e2p, CD & FD)">
        histPlotter1D(c1, P_e_1e2p_AC_CD, false, true, 1., "P_{e} After Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_AC_CD, "03_P_e_1e2p_AC_CD", P_e_1e2p_AC_CD_Dir,
                      "CD", kBlue, true, true, false, true, e_momentum_upper_cut_2p, e_momentum_lower_cut_2p, 0, false);

        histPlotter1D(c1, P_e_1e2p_AC_FD, false, true, 1., "P_{e} After Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_AC_FD, "03_P_e_1e2p_AC_FD", P_e_1e2p_AC_FD_Dir,
                      "FD", kBlue, true, true, false, true, e_momentum_upper_cut_2p, e_momentum_lower_cut_2p, 0, false);
        //</editor-fold>

        //<editor-fold desc="Protons momentum histograms (1e2p, CD & FD)">
        histPlotter1D(c1, P_p_1e2p_AC_CD, false, true, 1., "P_{p} After Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_AC_CD, "04_P_p_1e2p_AC_CD", P_p_1e2p_AC_CD_Dir,
                      "CD", kBlue, true, true, false, true, p_momentum_upper_cut_2p, p_momentum_lower_cut_2p, 0, false);

        histPlotter1D(c1, P_p_1e2p_AC_FD, false, true, 1., "P_{p} After Cut", "1e2p", plots, 2, false, true, P_Stack_1e2p_AC_FD, "04_P_p_1e2p_AC_FD", P_p_1e2p_AC_FD_Dir,
                      "FD", kBlue, true, true, false, true, p_momentum_upper_cut_2p, p_momentum_lower_cut_2p, 0, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms after 2p cuts (CD & FD)">

        //<editor-fold desc="Electrons momentum histograms (2p, CD & FD)">
        histPlotter1D(c1, P_e_2p_CD, false, true, 1., "P_{e}", "2p", plots, 2, false, true, P_Stack_2p_CD, "03_P_e_2p_CD", P_e_2p_CD_Dir, "CD", kBlue, true, true, false,
                      true, e_momentum_upper_cut_2p, e_momentum_lower_cut_2p, 0, false);

        histPlotter1D(c1, P_e_2p_FD, false, true, 1., "P_{e}", "2p", plots, 2, false, true, P_Stack_2p_FD, "03_P_e_2p_FD", P_e_2p_FD_Dir, "FD", kBlue, true, true, false,
                      true, e_momentum_upper_cut_2p, e_momentum_lower_cut_2p, 0, false);
        //</editor-fold>

        //<editor-fold desc="Protons momentum histograms (2p, CD & FD)">
        histPlotter1D(c1, P_p_2p_CD, false, true, 1., "P_{p}", "2p", plots, 2, false, true, P_Stack_2p_CD, "04_P_p_2p_CD", P_p_2p_CD_Dir, "CD", kBlue, true, true, false,
                      true, p_momentum_upper_cut_2p, p_momentum_lower_cut_2p, 0, false);

        histPlotter1D(c1, P_p_2p_FD, false, true, 1., "P_{p}", "2p", plots, 2, false, true, P_Stack_2p_FD, "04_P_p_2p_FD", P_p_2p_FD_Dir, "FD", kBlue, true, true, false,
                      true, p_momentum_upper_cut_2p, p_momentum_lower_cut_2p, 0, false);
        //</editor-fold>

        //</editor-fold>

//  Momentum histograms (MicroBooNE, CD & FD)  --------------------------------------------------------------

//        double Momentum_integral = P_e_histogram_CD->Integral() + P_e_histogram_FD->Integral();

        //<editor-fold desc="Momentum histograms before cuts (MicroBooNE, CD & FD)">

        //<editor-fold desc="Electrons momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_e_MicroBooNE_BC_CD, false, true, 1., "P_{e} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_CD,
                      "01_P_e_MicroBooNE_BC_CD", P_e_MicroBooNE_BC_CD_Dir, "CD", kBlue, true, true, false, true, e_momentum_upper_cut_MicroBooNE,
                      e_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_e_MicroBooNE_BC_FD, false, true, 1., "P_{e} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_FD,
                      "02_P_e_MicroBooNE_BC_FD", P_e_MicroBooNE_BC_FD_Dir, "FD", kBlue, true, true, false, true, e_momentum_upper_cut_MicroBooNE,
                      e_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //<editor-fold desc="Protons momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_p_MicroBooNE_BC_CD, false, true, 1., "P_{p} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_CD,
                      "03_P_p_MicroBooNE_BC_CD", P_p_MicroBooNE_BC_CD_Dir, "CD", kBlue, true, true, false, true, p_momentum_upper_cut_MicroBooNE,
                      p_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_p_MicroBooNE_BC_FD, false, true, 1., "P_{p} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_FD,
                      "04_P_p_MicroBooNE_BC_FD", P_p_MicroBooNE_BC_FD_Dir, "FD", kBlue, true, true, false, true, p_momentum_upper_cut_MicroBooNE,
                      p_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //<editor-fold desc="cPions momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_cpion_MicroBooNE_BC_CD, false, true, 1., "P_{#pi^{#pm}} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_CD,
                      "05_P_cpion_MicroBooNE_BC_CD", P_cpion_MicroBooNE_BC_CD_Dir, "CD", kBlue, true, true, false, true, cpion_momentum_upper_cut_MicroBooNE,
                      cpion_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_cpion_MicroBooNE_BC_FD, false, true, 1., "P_{#pi^{#pm}} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_FD,
                      "06_P_cpion_MicroBooNE_BC_FD", P_cpion_MicroBooNE_BC_FD_Dir, "FD", kBlue, true, true, false, true, cpion_momentum_upper_cut_MicroBooNE,
                      cpion_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms after cuts (MicroBooNE, CD & FD)">

        //<editor-fold desc="Electrons momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_e_MicroBooNE_AC_CD, false, true, 1., "P_{e} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_CD,
                      "01_P_e_MicroBooNE_AC_CD", P_e_MicroBooNE_AC_CD_Dir, "CD", kBlue, true, true, false, true, e_momentum_upper_cut_MicroBooNE,
                      e_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_e_MicroBooNE_AC_FD, false, true, 1., "P_{e} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_FD,
                      "02_P_e_MicroBooNE_AC_FD", P_e_MicroBooNE_AC_FD_Dir, "FD", kBlue, true, true, false, true, e_momentum_upper_cut_MicroBooNE,
                      e_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //<editor-fold desc="Protons momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_p_MicroBooNE_AC_CD, false, true, 1., "P_{p} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_CD,
                      "03_P_p_MicroBooNE_AC_CD", P_p_MicroBooNE_AC_CD_Dir, "CD", kBlue, true, true, false, true, p_momentum_upper_cut_MicroBooNE,
                      p_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_p_MicroBooNE_AC_FD, false, true, 1., "P_{p} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_FD,
                      "04_P_p_MicroBooNE_AC_FD", P_p_MicroBooNE_AC_FD_Dir, "FD", kBlue, true, true, false, true, p_momentum_upper_cut_MicroBooNE,
                      p_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //<editor-fold desc="cPions momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_cpion_MicroBooNE_AC_CD, false, true, 1., "P_{#pi^{#pm}} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_CD,
                      "05_P_cpion_MicroBooNE_AC_CD", P_cpion_MicroBooNE_AC_CD_Dir, "CD", kBlue, true, true, false, true, cpion_momentum_upper_cut_MicroBooNE,
                      cpion_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_cpion_MicroBooNE_AC_FD, false, true, 1., "P_{#pi^{#pm}} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_FD,
                      "06_P_cpion_MicroBooNE_AC_FD", P_cpion_MicroBooNE_AC_FD_Dir, "FD", kBlue, true, true, false, true, cpion_momentum_upper_cut_MicroBooNE,
                      cpion_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Momentum histograms after MicroBooNE cuts (CD & FD)">

        //<editor-fold desc="Electrons momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_e_MicroBooNE_CD, false, true, 1., "P_{e}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_CD, "01_P_e_MicroBooNE_CD",
                      P_e_MicroBooNE_CD_Dir, "CD", kBlue, true, true, false, true, e_momentum_upper_cut_MicroBooNE, e_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_e_MicroBooNE_FD, false, true, 1., "P_{e}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_FD, "02_P_e_MicroBooNE_FD",
                      P_e_MicroBooNE_FD_Dir, "FD", kBlue, true, true, false, true, e_momentum_upper_cut_MicroBooNE, e_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //<editor-fold desc="Protons momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_p_MicroBooNE_CD, false, true, 1., "P_{p}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_CD, "03_P_p_MicroBooNE_CD",
                      P_p_MicroBooNE_CD_Dir, "CD", kBlue, true, true, false, true, p_momentum_upper_cut_MicroBooNE, p_momentum_lower_cut_MicroBooNE, 0, false);

        histPlotter1D(c1, P_p_MicroBooNE_FD, false, true, 1., "P_{p}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_FD, "04_P_p_MicroBooNE_FD",
                      P_p_MicroBooNE_FD_Dir, "FD", kBlue, true, true, false, true, p_momentum_upper_cut_MicroBooNE, p_momentum_lower_cut_MicroBooNE, 0, false);
        //</editor-fold>

        //<editor-fold desc="cPions momentum histograms (MicroBooNE, CD & FD)">
        histPlotter1D(c1, P_cpion_MicroBooNE_CD, false, true, 1., "P_{#pi^{#pm}}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_CD, "05_P_cpion_MicroBooNE_CD",
                      P_cpion_MicroBooNE_CD_Dir, "CD", kBlue, true, true, false, true, cpion_momentum_upper_cut_MicroBooNE, cpion_momentum_lower_cut_MicroBooNE, 0,
                      false);

        histPlotter1D(c1, P_cpion_MicroBooNE_FD, false, true, 1., "P_{#pi^{#pm}}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_FD, "06_P_cpion_MicroBooNE_FD",
                      P_cpion_MicroBooNE_FD_Dir, "FD", kBlue, true, true, false, true, cpion_momentum_upper_cut_MicroBooNE, cpion_momentum_lower_cut_MicroBooNE, 0,
                      false);
        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nMomentum (P_e) plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Energy (E_e) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy (E_e) histograms">
    if (E_e_plots) {

        cout << "\n\nPlotting Energy (E_e) histograms...\n\n";

//  E_e  --------------------------------------------------------------

        //<editor-fold desc="1e Only Cut (CD & FD)">
        double E_e_integral = E_e_hist_CD->Integral() + E_e_hist_FD->Integral();

        //<editor-fold desc="E_e histograms (CD)">
        histPlotter1D(c1, E_e_hist_CD, false, true, E_e_integral, "E_{e} Histogram", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, E_e_Stack,
                      "E_e", E_e_hist_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (FD)">
        histPlotter1D(c1, E_e_hist_FD, false, true, E_e_integral, "E_{e} Histogram", "1e Only Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, E_e_Stack,
                      "E_e", E_e_hist_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="2p (CD & FD)">
        double E_e_2p_All_Int_integral = E_e_2p_CD->Integral() + E_e_2p_FD->Integral();
        double E_e_QEL_Int_integral = E_e_2p_QEL_CD->Integral() + E_e_2p_QEL_FD->Integral();
        double E_e_MEC_Int_integral = E_e_2p_MEC_CD->Integral() + E_e_2p_MEC_FD->Integral();
        double E_e_RES_Int_integral = E_e_2p_RES_CD->Integral() + E_e_2p_RES_FD->Integral();
        double E_e_DIS_Int_integral = E_e_2p_DIS_CD->Integral() + E_e_2p_DIS_FD->Integral();

        //<editor-fold desc="E_e histograms (all int, 2p, CD)">
        histPlotter1D(c1, E_e_2p_CD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_All_int_2p_Stack, "00 E_e_2p_All_Int_CD", E_e_2p_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (all int, 2p, FD)">
        histPlotter1D(c1, E_e_2p_FD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_All_int_2p_Stack, "00 E_e_2p_All_Int_FD", E_e_2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (QEL only, 2p, CD)">
        histPlotter1D(c1, E_e_2p_QEL_CD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_QEL_2p_Stack, "01_E_e_2p_QEL_Only_CD", E_e_2p_QEL_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (QEL only, 2p, FD)">
        histPlotter1D(c1, E_e_2p_QEL_FD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_QEL_2p_Stack, "01_E_e_2p_QEL_Only_FD", E_e_2p_QEL_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (MEC only, 2p, CD)">
        histPlotter1D(c1, E_e_2p_MEC_CD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_MEC_2p_Stack, "02_E_e_2p_MEC_Only_CD", E_e_2p_MEC_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (MEC only, 2p, FD)">
        histPlotter1D(c1, E_e_2p_MEC_FD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_MEC_2p_Stack, "02_E_e_2p_MEC_Only_FD", E_e_2p_MEC_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (RES only, 2p, CD)">
        histPlotter1D(c1, E_e_2p_RES_CD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_RES_2p_Stack, "03_E_e_2p_RES_Only_CD", E_e_2p_RES_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (RES only, 2p, FD)">
        histPlotter1D(c1, E_e_2p_RES_FD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_RES_2p_Stack, "03_E_e_2p_RES_Only_FD", E_e_2p_RES_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (DIS only, 2p, CD)">
        histPlotter1D(c1, E_e_2p_DIS_CD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_DIS_2p_Stack, "04_E_e_2p_DIS_Only_CD", E_e_2p_DIS_CD_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="E_e histograms (DIS only, 2p, FD)">
        histPlotter1D(c1, E_e_2p_DIS_FD, false, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      E_e_DIS_2p_Stack, "04_E_e_2p_DIS_Only_FD", E_e_2p_DIS_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>


//  E_e vs. Theta_e  --------------------------------------------------------------

        //<editor-fold desc="1e Only Cut (CD & FD)">

        //<editor-fold desc="E_e histograms (CD)">
        E_e_VS_Theta_e_hist_CD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_hist_CD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_hist_CD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_hist_CD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_hist_CD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_hist_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_hist_CD);
        E_e_VS_Theta_e_hist_CD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_hist_CD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_hist_CD_Dir + "Theta_e_VS_Phi_e_All_e_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (FD)">
        E_e_VS_Theta_e_hist_FD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_hist_FD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_hist_FD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_hist_FD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_hist_FD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_hist_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_hist_FD);
        E_e_VS_Theta_e_hist_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_hist_FD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_hist_FD_Dir + "Theta_e_VS_Phi_e_All_e_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="2p (CD & FD)">

        //<editor-fold desc="E_e histograms (All Int., 2p, CD)">
        E_e_VS_Theta_e_2p_CD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_CD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_CD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_CD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_CD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_CD);
        E_e_VS_Theta_e_2p_CD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_CD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_CD_Dir + "00 E_e_VS_Theta_e_All_Int_2p_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (All Int., 2p, FD)">
        E_e_VS_Theta_e_2p_FD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_FD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_FD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_FD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_FD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_FD);
        E_e_VS_Theta_e_2p_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_FD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_FD_Dir + "00 E_e_VS_Theta_e_All_Int_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (QEL only, 2p, CD)">
        E_e_VS_Theta_e_2p_QEL_CD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_QEL_CD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_QEL_CD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_QEL_CD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_QEL_CD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_QEL_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_QEL_CD);
        E_e_VS_Theta_e_2p_QEL_CD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_QEL_CD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_QEL_CD_Dir + "01_E_e_VS_Theta_e_QEL_only_2p_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (QEL only, 2p, FD)">
        E_e_VS_Theta_e_2p_QEL_FD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_QEL_FD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_QEL_FD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_QEL_FD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_QEL_FD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_QEL_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_QEL_FD);
        E_e_VS_Theta_e_2p_QEL_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_QEL_FD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_QEL_FD_Dir + "01_E_e_VS_Theta_e_QEL_only_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (MEC only, 2p, CD)">
        E_e_VS_Theta_e_2p_MEC_CD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_MEC_CD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_MEC_CD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_MEC_CD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_MEC_CD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_MEC_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_MEC_CD);
        E_e_VS_Theta_e_2p_MEC_CD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_MEC_CD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_MEC_CD_Dir + "02_E_e_VS_Theta_e_MEC_only_2p_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (MEC only, 2p, FD)">
        E_e_VS_Theta_e_2p_MEC_FD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_MEC_FD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_MEC_FD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_MEC_FD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_MEC_FD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_MEC_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_MEC_FD);
        E_e_VS_Theta_e_2p_MEC_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_MEC_FD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_MEC_FD_Dir + "02_E_e_VS_Theta_e_MEC_only_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (RES only, 2p, CD)">
        E_e_VS_Theta_e_2p_RES_CD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_RES_CD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_RES_CD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_RES_CD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_RES_CD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_RES_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_RES_CD);
        E_e_VS_Theta_e_2p_RES_CD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_RES_CD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_RES_CD_Dir + "03_E_e_VS_Theta_e_RES_only_2p_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (RES only, 2p, FD)">
        E_e_VS_Theta_e_2p_RES_FD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_RES_FD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_RES_FD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_RES_FD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_RES_FD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_RES_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_RES_FD);
        E_e_VS_Theta_e_2p_RES_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_RES_FD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_RES_FD_Dir + "03_E_e_VS_Theta_e_RES_only_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (DIS only, 2p, CD)">
        E_e_VS_Theta_e_2p_DIS_CD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_DIS_CD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_DIS_CD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_DIS_CD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_DIS_CD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_DIS_CD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_DIS_CD);
        E_e_VS_Theta_e_2p_DIS_CD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_DIS_CD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_DIS_CD_Dir + "04_E_e_VS_Theta_e_DIS_only_2p_CD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="E_e histograms (DIS only, 2p, FD)">
        E_e_VS_Theta_e_2p_DIS_FD->SetTitleSize(0.06, "xyz");
        E_e_VS_Theta_e_2p_DIS_FD->GetXaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_DIS_FD->GetXaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_DIS_FD->GetYaxis()->SetLabelSize(0.0425);
        E_e_VS_Theta_e_2p_DIS_FD->GetYaxis()->CenterTitle(true);
        E_e_VS_Theta_e_2p_DIS_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(E_e_VS_Theta_e_2p_DIS_FD);
        E_e_VS_Theta_e_2p_DIS_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        E_e_VS_Theta_e_2p_DIS_FD->SetStats(0);
        c1->SaveAs((E_e_VS_Theta_e_2p_DIS_FD_Dir + "04_E_e_VS_Theta_e_DIS_only_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nEnergy (E_e) plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Sampling Fraction (SF) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Sampling Fraction (SF) histograms">
    if (SF_plots) {

        cout << "\n\nPlotting Sampling Fraction histograms...\n\n";

//  SF histograms --------------------------------------------------------------

        SF_1e2p_Xmax = SF_All_Int_1e2p_BC_FD->GetBinCenter(SF_All_Int_1e2p_BC_FD->GetMaximumBin());

        //<editor-fold desc="SF histogram (1e2p, FD)">
//        double E_e_integral = E_e_hist_CD->Integral() + E_e_hist_FD->Integral();

        histPlotter1D(c1, SF_All_Int_1e2p_BC_FD, normalized_SF_plots, true, .1, "Sampling Fraction f Before Cuts", "All Int., 1e2p", plots, 2, false, true, SF_1e2p_Stack,
                      "01_SF_1e2p_before_SF_cuts", SF_All_Int_1e2p_BC_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e2p_upper_cut, SF_1e2p_lower_cut, SF_1e2p_Xmax);

        histPlotter1D(c1, SF_All_Int_1e2p_AC_FD, normalized_SF_plots, true, .1, "Sampling Fraction f After cuts", "All Int., 1e2p", plots, 2, false, true, SF_1e2p_Stack,
                      "02_SF_1e2p_after_SF_cuts", SF_All_Int_1e2p_AC_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e2p_upper_cut, SF_1e2p_lower_cut, SF_1e2p_Xmax);
        //</editor-fold>

        //<editor-fold desc="SF histogram (2p, FD)">
//        double E_e_integral = E_e_hist_CD->Integral() + E_e_hist_FD->Integral();

        histPlotter1D(c1, SF_All_Int_2p_FD, normalized_SF_plots, true, .1, "Sampling Fraction f", "All Int., 2p", plots, 2, false, true, SF_2p_Stack, "SF_2p_wAll_cuts",
                      SF_All_Int_2p_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e2p_upper_cut, SF_1e2p_lower_cut, SF_1e2p_Xmax);
        //</editor-fold>

//  SF vs. P  --------------------------------------------------------------

        //<editor-fold desc="SF vs. P 1e2p before cuts (FD)">
        SF_VS_P_e_1e2p_BC_FD->SetTitleSize(0.06, "xyz");
        SF_VS_P_e_1e2p_BC_FD->GetXaxis()->SetLabelSize(0.0425);
        SF_VS_P_e_1e2p_BC_FD->GetXaxis()->CenterTitle(true);
        SF_VS_P_e_1e2p_BC_FD->GetYaxis()->SetLabelSize(0.0425);
        SF_VS_P_e_1e2p_BC_FD->GetYaxis()->CenterTitle(true);
        SF_VS_P_e_1e2p_BC_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(SF_VS_P_e_1e2p_BC_FD);
        SF_VS_P_e_1e2p_BC_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        SF_VS_P_e_1e2p_BC_FD->SetStats(0);
        c1->SaveAs((SF_VS_P_e_1e2p_BC_FD_Dir + "01_SF_VS_P_e_1e2p_BC_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="SF vs. P 1e2p after cuts (FD)">
        SF_VS_P_e_1e2p_AC_FD->SetTitleSize(0.06, "xyz");
        SF_VS_P_e_1e2p_AC_FD->GetXaxis()->SetLabelSize(0.0425);
        SF_VS_P_e_1e2p_AC_FD->GetXaxis()->CenterTitle(true);
        SF_VS_P_e_1e2p_AC_FD->GetYaxis()->SetLabelSize(0.0425);
        SF_VS_P_e_1e2p_AC_FD->GetYaxis()->CenterTitle(true);
        SF_VS_P_e_1e2p_AC_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(SF_VS_P_e_1e2p_AC_FD);
        SF_VS_P_e_1e2p_AC_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        SF_VS_P_e_1e2p_AC_FD->SetStats(0);
        c1->SaveAs((SF_VS_P_e_1e2p_AC_FD_Dir + "02_SF_VS_P_e_1e2p_AC_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //<editor-fold desc="SF vs. P 2p after all cuts (FD)">
        SF_VS_P_e_2p_FD->SetTitleSize(0.06, "xyz");
        SF_VS_P_e_2p_FD->GetXaxis()->SetLabelSize(0.0425);
        SF_VS_P_e_2p_FD->GetXaxis()->CenterTitle(true);
        SF_VS_P_e_2p_FD->GetYaxis()->SetLabelSize(0.0425);
        SF_VS_P_e_2p_FD->GetYaxis()->CenterTitle(true);
        SF_VS_P_e_2p_FD->GetZaxis()->SetLabelSize(0.0425);
        plots->Add(SF_VS_P_e_2p_FD);
        SF_VS_P_e_2p_FD->Draw("colz");
        c1->SetLogz(1);
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        SF_VS_P_e_2p_FD->SetStats(0);
        c1->SaveAs((SF_VS_P_e_2p_FD_Dir + "SF_VS_P_e_2p_FD.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

    } else {
        cout << "\n\nSampling Fraction plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Electron fiducial histograms (FD only)
// ======================================================================================================================================================================

    //<editor-fold desc="Electron fiducial histograms (FD only)">
    if (fiducial_plots) {

        cout << "\n\nPlotting fiducial histograms...\n\n";

//  fiducial histograms --------------------------------------------------------------

//        //<editor-fold desc="fiducial histograms (ECIN)">
//
//        //<editor-fold desc="Vcal vs. EoP 1e2p BC ECIN">
//        Vcal_VS_EoP_1e2p_BC_ECIN->SetTitleSize(0.06, "xyz");
//        Vcal_VS_EoP_1e2p_BC_ECIN->GetXaxis()->SetLabelSize(0.0425);
//        Vcal_VS_EoP_1e2p_BC_ECIN->GetXaxis()->CenterTitle(true);
//        Vcal_VS_EoP_1e2p_BC_ECIN->GetYaxis()->SetLabelSize(0.0425);
//        Vcal_VS_EoP_1e2p_BC_ECIN->GetYaxis()->CenterTitle(true);
//        Vcal_VS_EoP_1e2p_BC_ECIN->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(Vcal_VS_EoP_1e2p_BC_ECIN);
//        Vcal_VS_EoP_1e2p_BC_ECIN->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        Vcal_VS_EoP_1e2p_BC_ECIN->SetStats(0);
//        c1->SaveAs((Vcal_VS_EoP_1e2p_BC_ECIN_Dir + "01_Vcal_VS_EoP_1e2p_BC_ECIN.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Vcal vs. EoP 1e2p AC ECIN">
//        Vcal_VS_EoP_1e2p_AC_ECIN->SetTitleSize(0.06, "xyz");
//        Vcal_VS_EoP_1e2p_AC_ECIN->GetXaxis()->SetLabelSize(0.0425);
//        Vcal_VS_EoP_1e2p_AC_ECIN->GetXaxis()->CenterTitle(true);
//        Vcal_VS_EoP_1e2p_AC_ECIN->GetYaxis()->SetLabelSize(0.0425);
//        Vcal_VS_EoP_1e2p_AC_ECIN->GetYaxis()->CenterTitle(true);
//        Vcal_VS_EoP_1e2p_AC_ECIN->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(Vcal_VS_EoP_1e2p_AC_ECIN);
//        Vcal_VS_EoP_1e2p_AC_ECIN->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        Vcal_VS_EoP_1e2p_AC_ECIN->SetStats(0);
//        c1->SaveAs((Vcal_VS_EoP_1e2p_AC_ECIN_Dir + "02_Vcal_VS_EoP_1e2p_AC_ECIN.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Wcal vs. EoP 1e2p BC ECIN">
//        Wcal_VS_EoP_1e2p_BC_ECIN->SetTitleSize(0.06, "xyz");
//        Wcal_VS_EoP_1e2p_BC_ECIN->GetXaxis()->SetLabelSize(0.0425);
//        Wcal_VS_EoP_1e2p_BC_ECIN->GetXaxis()->CenterTitle(true);
//        Wcal_VS_EoP_1e2p_BC_ECIN->GetYaxis()->SetLabelSize(0.0425);
//        Wcal_VS_EoP_1e2p_BC_ECIN->GetYaxis()->CenterTitle(true);
//        Wcal_VS_EoP_1e2p_BC_ECIN->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(Wcal_VS_EoP_1e2p_BC_ECIN);
//        Wcal_VS_EoP_1e2p_BC_ECIN->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        Wcal_VS_EoP_1e2p_BC_ECIN->SetStats(0);
//        c1->SaveAs((Wcal_VS_EoP_1e2p_BC_ECIN_Dir + "03_Wcal_VS_EoP_1e2p_BC_ECIN.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Wcal vs. EoP 1e2p AC ECIN">
//        Wcal_VS_EoP_1e2p_AC_ECIN->SetTitleSize(0.06, "xyz");
//        Wcal_VS_EoP_1e2p_AC_ECIN->GetXaxis()->SetLabelSize(0.0425);
//        Wcal_VS_EoP_1e2p_AC_ECIN->GetXaxis()->CenterTitle(true);
//        Wcal_VS_EoP_1e2p_AC_ECIN->GetYaxis()->SetLabelSize(0.0425);
//        Wcal_VS_EoP_1e2p_AC_ECIN->GetYaxis()->CenterTitle(true);
//        Wcal_VS_EoP_1e2p_AC_ECIN->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(Wcal_VS_EoP_1e2p_AC_ECIN);
//        Wcal_VS_EoP_1e2p_AC_ECIN->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        Wcal_VS_EoP_1e2p_AC_ECIN->SetStats(0);
//        c1->SaveAs((Wcal_VS_EoP_1e2p_AC_ECIN_Dir + "04_Wcal_VS_EoP_1e2p_AC_ECIN.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Vcal vs. EoP 2p ECIN">
//        Vcal_VS_EoP_2p_ECIN->SetTitleSize(0.06, "xyz");
//        Vcal_VS_EoP_2p_ECIN->GetXaxis()->SetLabelSize(0.0425);
//        Vcal_VS_EoP_2p_ECIN->GetXaxis()->CenterTitle(true);
//        Vcal_VS_EoP_2p_ECIN->GetYaxis()->SetLabelSize(0.0425);
//        Vcal_VS_EoP_2p_ECIN->GetYaxis()->CenterTitle(true);
//        Vcal_VS_EoP_2p_ECIN->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(Vcal_VS_EoP_2p_ECIN);
//        Vcal_VS_EoP_2p_ECIN->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        Vcal_VS_EoP_2p_ECIN->SetStats(0);
//        c1->SaveAs((Vcal_VS_EoP_2p_ECIN_Dir + "01_Vcal_VS_EoP_2p_ECIN.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>
//
//        //<editor-fold desc="Wcal vs. EoP 2p ECIN">
//        Wcal_VS_EoP_2p_ECIN->SetTitleSize(0.06, "xyz");
//        Wcal_VS_EoP_2p_ECIN->GetXaxis()->SetLabelSize(0.0425);
//        Wcal_VS_EoP_2p_ECIN->GetXaxis()->CenterTitle(true);
//        Wcal_VS_EoP_2p_ECIN->GetYaxis()->SetLabelSize(0.0425);
//        Wcal_VS_EoP_2p_ECIN->GetYaxis()->CenterTitle(true);
//        Wcal_VS_EoP_2p_ECIN->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(Wcal_VS_EoP_2p_ECIN);
//        Wcal_VS_EoP_2p_ECIN->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        Wcal_VS_EoP_2p_ECIN->SetStats(0);
//        c1->SaveAs((Wcal_VS_EoP_2p_ECIN_Dir + "02_Wcal_VS_EoP_2p_ECIN.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>
//
//        //</editor-fold>

        //<editor-fold desc="fiducial histograms (PCAL)">

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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Wcal_VS_EoP_1e_AC_PCAL->SetStats(0);
        c1->SaveAs((Wcal_VS_EoP_1e_AC_PCAL_Dir + "02_Wcal_VS_EoP_1e_AC_PCAL.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        Wcal_VS_EoP_2p_PCAL->SetStats(0);
        c1->SaveAs((Wcal_VS_EoP_2p_PCAL_Dir + "02_Wcal_VS_EoP_2p_PCAL.png").c_str());
        gStyle->SetStatX(DefStatX);
        gStyle->SetStatY(DefStatY);
        c1->Clear();
        //</editor-fold>

        //</editor-fold>

    } else {
        cout << "\n\nFiducial plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Number of Photo-electrons (nphe) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Number of Photo-electrons (nphe) histograms">
    if (nphe_plots) {

        cout << "\n\nPlotting number of photo-electrons (nphe) histograms...\n\n";

//  Number of Photo-electrons (nphe) histograms --------------------------------------------------------------

//        nphe_1e2p_Xmax = nphe_All_Int_1e2p_BC_FD->GetBinCenter(nphe_All_Int_1e2p_BC_FD->GetMaximumBin());

        //<editor-fold desc="Number of Photo-electrons (nphe) histogram (1e2p, FD)">
//        double E_e_integral = E_e_hist_CD->Integral() + E_e_hist_FD->Integral();

        histPlotter1D(c1, nphe_All_Int_1e2p_BC_FD, normalized_nphe_plots, true, .1, "#Photo-electrons in HTCC (n_{phe} ) Before Cuts", "All Int., 1e2p",
                      plots, 2, false, true, nphe_1e2p_Stack, "01_nphe_1e2p_before_nphe_cuts", nphe_All_Int_1e2p_BC_FD_Dir, "FD", kBlue, true, true, false, true,
                      nphe_lower_cut, 0, false);

        histPlotter1D(c1, nphe_All_Int_1e2p_AC_FD, normalized_nphe_plots, true, .1, "#Photo-electrons in HTCC (n_{phe} ) After Cuts", "All Int., 1e2p",
                      plots, 2, false, true, nphe_1e2p_Stack, "02_nphe_1e2p_after_nphe_cuts", nphe_All_Int_1e2p_AC_FD_Dir, "FD", kBlue, true, true, false, true,
                      nphe_lower_cut, 0, false);
        //</editor-fold>

        //<editor-fold desc="Number of Photo-electrons (nphe) histogram (2p, FD)">
//        double E_e_integral = E_e_hist_CD->Integral() + E_e_hist_FD->Integral();

        histPlotter1D(c1, nphe_All_Int_2p_FD, normalized_nphe_plots, true, .1, "#Photo-electrons in HTCC (n_{phe} )", "All Int., 2p", plots, 2, false,
                      true, nphe_2p_Stack, "nphe_2p", nphe_All_Int_2p_FD_Dir, "FD", kBlue, true, true, false, true, nphe_lower_cut, 0, false);
        //</editor-fold>

    } else {
        cout << "\n\nnumber of photo-electrons (nphe) plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Energy transfer histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy transfer histograms">
    if (ETrans_all_plots) {

        cout << "\n\nPlotting Energy Transfer (ET) histograms...\n\n";

        if (ETrans_All_Int_plots) {

            cout << "\n\nPlotting ET for all interactions...\n\n";

            //<editor-fold desc="ET (beamE-E_e) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (all interactions)">
            double ETrans15_All_Int_integral_2p = ETrans_15_All_Int_2p_CD->Integral() + ETrans_15_All_Int_2p_FD->Integral();

            histPlotter1D(c1, ETrans_15_All_Int_2p_CD, normalized_E_Trans15_plots, true, ETrans15_All_Int_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_CD, "ETrans_15_All_Int_2p", ETrans_15_All_Int_2p_CD_Dir, "CD", kBlack, true, true, true);

            histPlotter1D(c1, ETrans_15_All_Int_2p_FD, normalized_E_Trans15_plots, true, ETrans15_All_Int_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_FD, "ETrans_15_All_Int_2p", ETrans_15_All_Int_2p_FD_Dir, "FD", kBlack, true, true, true);
            //</editor-fold>

        }

        if (ETrans_QEL_plots) {

            cout << "\n\nPlotting ET for QEL only...\n\n";

            //<editor-fold desc="ET (beamE-E_e) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (QEL only)">
            double ETrans15_QEL_integral_2p = ETrans_15_QEL_2p_CD->Integral() + ETrans_15_QEL_2p_FD->Integral();

            histPlotter1D(c1, ETrans_15_QEL_2p_CD, normalized_E_Trans15_plots, true, ETrans15_QEL_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_CD, "ETrans_15_QEL_2p", ETrans_15_QEL_2p_CD_Dir, "CD", kBlue, true, true, true);

            histPlotter1D(c1, ETrans_15_QEL_2p_FD, normalized_E_Trans15_plots, true, ETrans15_QEL_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_FD, "ETrans_15_QEL_2p", ETrans_15_QEL_2p_FD_Dir, "FD", kBlue, true, true, true);
            //</editor-fold>

        }

        if (ETrans_MEC_plots) {

            cout << "\n\nPlotting ET for MEC only...\n\n";

            //<editor-fold desc="ET (beamE-E_e) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (MEC only)">
            double ETrans15_MEC_integral_2p = ETrans_15_MEC_2p_CD->Integral() + ETrans_15_MEC_2p_FD->Integral();

            histPlotter1D(c1, ETrans_15_MEC_2p_CD, normalized_E_Trans15_plots, true, ETrans15_MEC_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_CD, "ETrans_15_MEC_2p", ETrans_15_MEC_2p_CD_Dir, "CD", kRed, true, true, true);

            histPlotter1D(c1, ETrans_15_MEC_2p_FD, normalized_E_Trans15_plots, true, ETrans15_MEC_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_FD, "ETrans_15_MEC_2p", ETrans_15_MEC_2p_FD_Dir, "FD", kRed, true, true, true);
            //</editor-fold>

        }

        if (ETrans_RES_plots) {

            cout << "\n\nPlotting ET for RES only...\n\n";

            //<editor-fold desc="ET (beamE-E_e) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (RES only)">
            double ETrans15_RES_integral_2p = ETrans_15_RES_2p_CD->Integral() + ETrans_15_RES_2p_FD->Integral();

            histPlotter1D(c1, ETrans_15_RES_2p_CD, normalized_E_Trans15_plots, true, ETrans15_RES_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_CD, "ETrans_15_RES_2p", ETrans_15_RES_2p_CD_Dir, "CD", kGreen, true, true, true);

            histPlotter1D(c1, ETrans_15_RES_2p_FD, normalized_E_Trans15_plots, true, ETrans15_RES_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_FD, "ETrans_15_RES_2p", ETrans_15_RES_2p_FD_Dir, "FD", kGreen, true, true, true);
            //</editor-fold>

        }

        if (ETrans_DIS_plots) {

            cout << "\n\nPlotting ET for DIS only...\n\n";

            //<editor-fold desc="ET (beamE-E_e) in the angle range 14[Deg] <= theta_{l} <= 16[Deg] (DIS only)">
            double ETrans15_DIS_integral_2p = ETrans_15_DIS_2p_CD->Integral() + ETrans_15_DIS_2p_FD->Integral();

            histPlotter1D(c1, ETrans_15_DIS_2p_CD, normalized_E_Trans15_plots, true, ETrans15_DIS_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_CD, "ETrans_15_DIS_2p", ETrans_15_DIS_2p_CD_Dir, "CD", kMagenta, true, true, true);

            histPlotter1D(c1, ETrans_15_DIS_2p_FD, normalized_E_Trans15_plots, true, ETrans15_DIS_integral_2p,
                          "ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, ETrans_all_int_15_Stack_2p_FD, "ETrans_15_DIS_2p", ETrans_15_DIS_2p_FD_Dir, "FD", kMagenta, true, true, true);
            //</editor-fold>

        }

//  Energy transfer around 15 deg stack (2p only) ------------------------------------------------------

        if (ETrans_All_Int_plots && ETrans_QEL_plots && ETrans_MEC_plots && ETrans_RES_plots && ETrans_DIS_plots) {

            //<editor-fold desc="Energy transfer around 15 deg stack (2p, CD)">
            ETrans_all_int_15_Stack_2p_CD->Draw("nostack");
            ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
            ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
            ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetXaxis()->CenterTitle(true);
            ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
            ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetYaxis()->CenterTitle(true);

            if (normalized_E_Trans15_plots) {
                ETrans_all_int_15_Stack_2p_CD->SetTitle("ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (2p, CD) - Normalized");
                ETrans_all_int_15_Stack_2p_CD->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
                ETrans_all_int_15_Stack_2p_CD->GetYaxis()->SetTitle("Probability (%)");
                ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            } else {
                ETrans_all_int_15_Stack_2p_CD->GetYaxis()->SetTitle("Arbitrary units");
                ETrans_all_int_15_Stack_2p_CD->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            }

            auto E_Trans_15_legend_2p_CD = new TLegend(0.775, 0.625, 0.9, 0.9);

            TLegendEntry *E_Trans_15_all_entry_2p_stack_CD = E_Trans_15_legend_2p_CD->AddEntry(ETrans_15_All_Int_2p_CD, "All int.", "l");
            TLegendEntry *E_Trans_15_QEL_entry_2p_stack_CD = E_Trans_15_legend_2p_CD->AddEntry(ETrans_15_QEL_2p_CD, "QEL", "l");
            TLegendEntry *E_Trans_15_MEC_entry_2p_stack_CD = E_Trans_15_legend_2p_CD->AddEntry(ETrans_15_MEC_2p_CD, "MEC", "l");
            TLegendEntry *E_Trans_15_RES_entry_2p_stack_CD = E_Trans_15_legend_2p_CD->AddEntry(ETrans_15_RES_2p_CD, "RES", "l");
            TLegendEntry *E_Trans_15_DIS_entry_2p_stack_CD = E_Trans_15_legend_2p_CD->AddEntry(ETrans_15_DIS_2p_CD, "DIS", "l");
            E_Trans_15_legend_2p_CD->Draw();

            plots->Add(ETrans_all_int_15_Stack_2p_CD);
            c1->SaveAs((ETrans_all_int_15_Stack_2p_CD_Dir + "E_Trans_15_2p_stack_CD.png").c_str());
            c1->Clear();
            //</editor-fold>

            //<editor-fold desc="Energy transfer around 15 deg stack (2p, FD)">
            ETrans_all_int_15_Stack_2p_FD->Draw("nostack");
            ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
            ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
            ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetXaxis()->CenterTitle(true);
            ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
            ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetYaxis()->CenterTitle(true);

            if (normalized_E_Trans15_plots) {
                ETrans_all_int_15_Stack_2p_FD->SetTitle("ET (E_{l}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (2p, FD) - Normalized");
                ETrans_all_int_15_Stack_2p_FD->GetXaxis()->SetTitle("E_{#nu}-E_{l} [GeV]");
                ETrans_all_int_15_Stack_2p_FD->GetYaxis()->SetTitle("Probability (%)");
                ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            } else {
                ETrans_all_int_15_Stack_2p_FD->GetYaxis()->SetTitle("Arbitrary units");
                ETrans_all_int_15_Stack_2p_FD->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
            }

            auto E_Trans_15_legend_2p_FD = new TLegend(0.775, 0.625, 0.9, 0.9);

            TLegendEntry *E_Trans_15_all_entry_2p_stack_FD = E_Trans_15_legend_2p_FD->AddEntry(ETrans_15_All_Int_2p_FD, "All int.", "l");
            TLegendEntry *E_Trans_15_QEL_entry_2p_stack_FD = E_Trans_15_legend_2p_FD->AddEntry(ETrans_15_QEL_2p_FD, "QEL", "l");
            TLegendEntry *E_Trans_15_MEC_entry_2p_stack_FD = E_Trans_15_legend_2p_FD->AddEntry(ETrans_15_MEC_2p_FD, "MEC", "l");
            TLegendEntry *E_Trans_15_RES_entry_2p_stack_FD = E_Trans_15_legend_2p_FD->AddEntry(ETrans_15_RES_2p_FD, "RES", "l");
            TLegendEntry *E_Trans_15_DIS_entry_2p_stack_FD = E_Trans_15_legend_2p_FD->AddEntry(ETrans_15_DIS_2p_FD, "DIS", "l");
            E_Trans_15_legend_2p_FD->Draw();

            plots->Add(ETrans_all_int_15_Stack_2p_FD);
            c1->SaveAs((ETrans_all_int_15_Stack_2p_FD_Dir + "E_Trans_15_2p_stack_FD.png").c_str());
            c1->Clear();
            //</editor-fold>

        }
        //</editor-fold>

    } else {
        cout << "\n\nEnergy transfer plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Ecal restorations
// ======================================================================================================================================================================

    //<editor-fold desc="Ecal restorations">
    if (Ecal_plots) {

        cout << "\n\nPlotting Ecal restoration histograms...\n\n";

//  E_cal_All_Int restoration ------------------------------------------------------------------------------

        //<editor-fold desc="E_cal_QEL restoration (2p & 1n1p)">
//        double E_cal_QEL_integral = E_cal_QEL_2p->Integral() + E_cal_QEL_1n1p->Integral();

        histPlotter1D(c1, Ecal_All_Int_2p, normalized_E_cal_plots, true, 1., "E_{cal} Histogram", "All Int., 2p", 0.06, 0.0425, 0.0425,
                      plots, 4, false, true, Ecal_Stack_2p, "Ecal_restoration_All_Int", Ecal_All_Int_2p_Dir, "CD", kBlack, true, true, true);
        //</editor-fold>

//  E_cal_QEL restoration ------------------------------------------------------------------------------

        //<editor-fold desc="E_cal_QEL restoration (2p & 1n1p)">
//        double E_cal_QEL_integral = E_cal_QEL_2p->Integral() + E_cal_QEL_1n1p->Integral();

        histPlotter1D(c1, Ecal_QEL_2p, normalized_E_cal_plots, true, 1., "E_{cal} Histogram", "QEL Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, Ecal_Stack_2p, "Ecal_restoration_QEL_only", Ecal_QEL_2p_Dir, "CD", kBlue, true, true, true);
        //</editor-fold>

//  E_cal_MEC restoration ------------------------------------------------------------------------------

        //<editor-fold desc="E_cal_MEC restoration (2p & 1n1p)">
//        double E_cal_MEC_integral = E_cal_MEC_2p->Integral() + E_cal_MEC_1n1p->Integral();

        histPlotter1D(c1, Ecal_MEC_2p, normalized_E_cal_plots, true, 1., "E_{cal} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, Ecal_Stack_2p, "Ecal_restoration_MEC_only", Ecal_MEC_2p_Dir, "CD", kRed, true, true, true);
        //</editor-fold>


//  E_cal_RES restoration ------------------------------------------------------------------------------

        //<editor-fold desc="E_cal_RES restoration (2p & 1n1p)">
//        double E_cal_RES_integral = E_cal_RES_2p->Integral() + E_cal_RES_1n1p->Integral();

        histPlotter1D(c1, Ecal_RES_2p, normalized_E_cal_plots, true, 1., "E_{cal} Histogram", "RES Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, Ecal_Stack_2p, "Ecal_restoration_RES_only", Ecal_RES_2p_Dir, "CD", kGreen, true, true, true);
        //</editor-fold>


//  E_cal_DIS restoration ------------------------------------------------------------------------------

        //<editor-fold desc="E_cal_DIS restoration (2p & 1n1p)">
//        double E_cal_DIS_integral = E_cal_DIS_2p->Integral() + E_cal_DIS_1n1p->Integral();

        histPlotter1D(c1, Ecal_DIS_2p, normalized_E_cal_plots, true, 1., "E_{cal} Histogram", "DIS Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, Ecal_Stack_2p, "Ecal_restoration_DIS_only", Ecal_DIS_2p_Dir, "CD", kMagenta, true, true, true);
        //</editor-fold>

//  Ecal stack (2p only) ------------------------------------------------------

        //<editor-fold desc="Energy transfer around 15 deg stack (2p, CD)">
        Ecal_Stack_2p->Draw("nostack");
        Ecal_Stack_2p->GetHistogram()->GetXaxis()->SetTitleSize(0.06);
        Ecal_Stack_2p->GetHistogram()->GetXaxis()->SetLabelSize(0.0425);
        Ecal_Stack_2p->GetHistogram()->GetXaxis()->CenterTitle(true);
        Ecal_Stack_2p->GetHistogram()->GetYaxis()->SetLabelSize(0.0425);
        Ecal_Stack_2p->GetHistogram()->GetYaxis()->CenterTitle(true);

        if (normalized_E_Trans15_plots) {
            Ecal_Stack_2p->SetTitle("E_{cal} Reconstruction (2p) - Normalized");
            Ecal_Stack_2p->GetXaxis()->SetTitle("E_{cal} = E_{e} + T_{p1} + T_{p2} [GeV]");
            Ecal_Stack_2p->GetYaxis()->SetTitle("Probability (%)");
            Ecal_Stack_2p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        } else {
            Ecal_Stack_2p->GetYaxis()->SetTitle("Arbitrary units");
            Ecal_Stack_2p->GetHistogram()->GetYaxis()->SetTitleSize(0.06);
        }

        auto Ecal_legend_2p_CD = new TLegend(0.775, 0.625, 0.9, 0.9);

        TLegendEntry *Ecal_all_entry_2p_stack_CD = Ecal_legend_2p_CD->AddEntry(Ecal_All_Int_2p, "All int.", "l");
        TLegendEntry *Ecal_QEL_entry_2p_stack_CD = Ecal_legend_2p_CD->AddEntry(Ecal_QEL_2p, "QEL", "l");
        TLegendEntry *Ecal_MEC_entry_2p_stack_CD = Ecal_legend_2p_CD->AddEntry(Ecal_MEC_2p, "MEC", "l");
        TLegendEntry *Ecal_RES_entry_2p_stack_CD = Ecal_legend_2p_CD->AddEntry(Ecal_RES_2p, "RES", "l");
        TLegendEntry *Ecal_DIS_entry_2p_stack_CD = Ecal_legend_2p_CD->AddEntry(Ecal_DIS_2p, "DIS", "l");
        Ecal_legend_2p_CD->Draw();

        plots->Add(Ecal_Stack_2p);
        c1->SaveAs((Ecal_Stack_2p_Dir + "Ecal_Stack_2p.png").c_str());
        c1->Clear();
        //</editor-fold>

    } else {
        cout << "\n\nEcal plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// MicroBooNE article histogram reconstructions
// ====================================================================================================

    //<editor-fold desc="MicroBooNE article histogram reconstructions">
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

    } else {
        cout << "\n\nMicroBooNE plots are disabled by user.\n\n";
    }
    //</editor-fold>

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                          Saving histogram list and finishing execution                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Saving histogram list and finishing execution">

// Saving settings to log file ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Saving settings to log file">
    if (delete_txt_files == true) {
        system(("find " + plots_path + " -type f -iname '*.txt' -delete").c_str()); // Delete existing .txt files
    }

    ofstream myLogFile;
    myLogFile.open(plots_log_save_Directory.c_str());

    myLogFile << "///////////////////////////////////////////////////////////////////////////\n";
    myLogFile << "// Run was with '" << file_name << "' settings mode\n";
    myLogFile << "// Input file was " << LoadedInput << "\n";
    myLogFile << "// Code version was " << Ver << "\n";
    myLogFile << "// Analysis mode was 'Detector simulation'\n";
    myLogFile << "///////////////////////////////////////////////////////////////////////////\n\n";
    myLogFile << "Code ran with the following settings:" << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Input\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "LoadedInput: " << LoadedInput << "\n";
    myLogFile << "filePath: " << filePath << "\n";
    myLogFile << "fileInput: " << fileInput << "\n";
    myLogFile << "plotsInput: " << plotsInput << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Plot settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "wider_margin = " << BoolToString(wider_margin) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Calculation settings\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "calculate_2p = " << BoolToString(calculate_2p) << "\n";
    myLogFile << "calculate_1n1p = " << BoolToString(calculate_1n1p) << "\n";
    myLogFile << "calculate_MicroBooNE = " << BoolToString(calculate_MicroBooNE) << "\n\n";

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

    myLogFile << "beamE = " << beamE << "\n";
    myLogFile << "Pv = " << Pv << "\n";
    myLogFile << "Pvx = " << Pvx << "\n";
    myLogFile << "Pvy = " << Pvy << "\n";
    myLogFile << "Pvz = " << Pvz << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Plot selector\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "Theta_plots = " << BoolToString(Theta_plots) << "\n";
    myLogFile << "Phi_plots = " << BoolToString(Phi_plots) << "\n";
    myLogFile << "Energy_histogram_plots = " << BoolToString(Energy_histogram_plots) << "\n";
    myLogFile << "ETrans_all_plots = " << BoolToString(ETrans_all_plots) << "\n";
    myLogFile << "ETrans_QEL_plots = " << BoolToString(ETrans_QEL_plots) << "\n";
    myLogFile << "ETrans_MEC_plots = " << BoolToString(ETrans_MEC_plots) << "\n";
    myLogFile << "ETrans_RES_plots = " << BoolToString(ETrans_RES_plots) << "\n";
    myLogFile << "ETrans_DIS_plots = " << BoolToString(ETrans_DIS_plots) << "\n";
    myLogFile << "inclusive_plots = " << BoolToString(inclusive_plots) << "\n";
    myLogFile << "E_cal_plots = " << BoolToString(Ecal_plots) << "\n";
    myLogFile << "other_E_cal_plots = " << BoolToString(other_E_cal_plots) << "\n";
    myLogFile << "momentum_plots = " << BoolToString(momentum_plots) << "\n";
    myLogFile << "MicroBooNE_plots = " << BoolToString(MicroBooNE_plots) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Normalization settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "normalized_theta_lp_plots = " << BoolToString(normalized_theta_lp_plots) << "\n";
    myLogFile << "normalized_E_lp_plots = " << BoolToString(normalized_E_lp_plots) << "\n";
    myLogFile << "normalized_E_Trans15_plots = " << BoolToString(normalized_E_Trans15_plots) << "\n";
    myLogFile << "normalized_E_cal_plots = " << BoolToString(normalized_E_cal_plots) << "\n";
    myLogFile << "normalized_inclusive_plots = " << BoolToString(normalized_inclusive_plots) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Delete settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "delete_png_files = " << BoolToString(delete_png_files) << "\n";
    myLogFile << "delete_root_files = " << BoolToString(delete_root_files) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Momentum thresholds (2p)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "P_lp_upper_lim_2p = " << P_lp_upper_lim_2p << "\n";
    myLogFile << "P_lp_lower_lim_2p = " << P_lp_lower_lim_2p << "\n";
    myLogFile << "P_p1_upper_lim_2p = " << P_p1_upper_lim_2p << "\n";
    myLogFile << "P_p1_lower_lim_2p = " << P_p1_lower_lim_2p << "\n";
    myLogFile << "P_p2_upper_lim_2p = " << P_p2_upper_lim_2p << "\n";
    myLogFile << "P_p2_lower_lim_2p = " << P_p2_lower_lim_2p << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Momentum thresholds (1n1p)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "P_lp_upper_lim_1n1p = " << P_lp_upper_lim_1n1p << "\n";
    myLogFile << "P_lp_lower_lim_1n1p = " << P_lp_lower_lim_1n1p << "\n";
    myLogFile << "P_p_upper_lim_1n1p = " << P_p_upper_lim_1n1p << "\n";
    myLogFile << "P_p_lower_lim_1n1p = " << P_p_lower_lim_1n1p << "\n";
    myLogFile << "P_n_upper_lim_1n1p = " << P_n_upper_lim_1n1p << "\n";
    myLogFile << "P_n_lower_lim_1n1p = " << P_n_lower_lim_1n1p << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Momentum thresholds (2p)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "e_momentum_upper_cut_2p = " << e_momentum_upper_cut_2p << "\n";
    myLogFile << "e_momentum_lower_cut_2p = " << e_momentum_lower_cut_2p << "\n";
    myLogFile << "P_L_upper_lim_2p* = " << p_momentum_upper_cut_2p << "\n";
    myLogFile << "P_L_lower_lim_2p* = " << p_momentum_lower_cut_2p << "\n";
    myLogFile << "P_R_upper_lim_2p* = " << p_momentum_upper_cut_2p << "\n";
    myLogFile << "P_R_lower_lim_2p* = " << p_momentum_lower_cut_2p << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Momentum thresholds (MicroBooNE)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "e_momentum_upper_cut_MicroBooNE = " << e_momentum_upper_cut_MicroBooNE << "\n";
    myLogFile << "e_momentum_lower_cut_MicroBooNE = " << e_momentum_lower_cut_MicroBooNE << "\n";
    myLogFile << "p_momentum_upper_cut_MicroBooNE = " << p_momentum_upper_cut_MicroBooNE << "\n";
    myLogFile << "p_momentum_lower_cut_MicroBooNE = " << p_momentum_lower_cut_MicroBooNE << "\n";
    myLogFile << "cpion_momentum_upper_cut_MicroBooNE = " << cpion_momentum_upper_cut_MicroBooNE << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Sampling Fraction (SF) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_SF_cuts = " << BoolToString(apply_SF_cuts) << "\n";
    myLogFile << "SF_1e2p_upper_cut = " << SF_1e2p_upper_cut << "\n";
    myLogFile << "SF_1e2p_lower_cut = " << SF_1e2p_lower_cut << "\n";
    myLogFile << "SF_1e2p_Xmax (from histogram) = " << SF_1e2p_Xmax << "\n";
    myLogFile << "SF_1e2p_peak (used in cuts) = " << SF_1e2p_peak << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Fiducial cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_Lv_cut = " << BoolToString(apply_Lv_cut) << "\n";
    myLogFile << "fiducial_cut_Lv = " << fiducial_cut_Lv << "\n";
    myLogFile << "apply_Lw_cut = " << BoolToString(apply_Lw_cut) << "\n";
    myLogFile << "fiducial_cut_Lw = " << fiducial_cut_Lw << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Number of Photo-electrons (nphe) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_nphe_cut = " << BoolToString(apply_nphe_cut) << "\n";
    myLogFile << "nphe_lower_cut = " << nphe_lower_cut << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Chi2 cuts\n";
    myLogFile << "===========================================================================\n\n";

    //<editor-fold desc="Chi2_Electron">
    myLogFile << "\n-- Electrons in CD (no #e cuts) -------------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_cut_CD = " << Chi2_Electron_cut_CD << "\n";

    if (Chi2_Electron_CD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_Xmax_CD (from histogram) = " << Chi2_Electron_Xmax_CD << "\n";
    }

    myLogFile << "\n-- Electrons in FD (no #e cuts) -------------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_cut_FD = " << Chi2_Electron_cut_FD << "\n";

    if (Chi2_Electron_FD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_Xmax_FD (from histogram) = " << Chi2_Electron_Xmax_FD << "\n";
    }

    myLogFile << "\n-- Electrons in CD (1e cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_1e_cut_CD = " << Chi2_Electron_cut_CD << "\n";

    if (Chi2_Electron_1e_CD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_1e_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_1e_Xmax_CD (from histogram) = " << Chi2_Electron_1e_Xmax_CD << "\n";
    }

    myLogFile << "Chi2_Electron_1e_peak_CD (used in cuts) = " << Chi2_Electron_1e_peak_CD << "\n";

    myLogFile << "\n-- Electrons in FD (1e cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_1e_cut_FD = " << Chi2_Electron_cut_FD << "\n";

    if (Chi2_Electron_1e_FD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_1e_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_1e_Xmax_FD (from histogram) = " << Chi2_Electron_1e_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Electron_1e_peak_FD (used in cuts) = " << Chi2_Electron_1e_peak_FD << "\n";

    myLogFile << "\n-- Electrons in CD (MicroBooNE cut) ---------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_cut_MicroBooNE_CD = " << Chi2_Electron_cut_MicroBooNE_CD << "\n";

    if (Chi2_Electron_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_CD (from histogram) = " << Chi2_Electron_MicroBooNE_Xmax_CD << "\n";
    }

    myLogFile << "Chi2_Electron_1e_peak_MicroBooNE_CD (used in cuts) = " << Chi2_Electron_1e_peak_MicroBooNE_CD << "\n";

    myLogFile << "\n-- Electrons in FD (MicroBooNE cut) ---------------------------------------" << "\n";

    myLogFile << "Chi2_Electron_cut_MicroBooNE_FD = " << Chi2_Electron_cut_MicroBooNE_FD << "\n";

    if (Chi2_Electron_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_FD (from histogram) = " << Chi2_Electron_MicroBooNE_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Electron_1e_peak_MicroBooNE_FD (used in cuts) = " << Chi2_Electron_1e_peak_MicroBooNE_FD << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_Proton">
    myLogFile << "\n-- Protons in CD (no #e cuts) ---------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_CD = " << Chi2_Proton_cut_CD << "\n";

    if (Chi2_Proton_CD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_Xmax_CD (from histogram) = " << Chi2_Proton_Xmax_CD << "\n";
    }

    myLogFile << "\n-- Protons in FD (no #e cuts) ---------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_FD = " << Chi2_Proton_cut_FD << "\n";

    if (Chi2_Proton_FD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_Xmax_FD (from histogram) = " << Chi2_Proton_Xmax_FD << "\n";
    }

    myLogFile << "\n-- Protons in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_1e_cut_CD = " << Chi2_Proton_cut_CD << "\n";

    if (Chi2_Proton_1e_CD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_1e_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_1e_Xmax_CD (from histogram) = " << Chi2_Proton_1e_Xmax_CD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_CD (used in cuts) = " << Chi2_Proton_1e_peak_CD << "\n";

    myLogFile << "\n-- Protons in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_1e_cut_FD = " << Chi2_Proton_cut_FD << "\n";

    if (Chi2_Proton_1e_FD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_1e_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_1e_Xmax_FD (from histogram) = " << Chi2_Proton_1e_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_FD (used in cuts) = " << Chi2_Proton_1e_peak_FD << "\n";

    myLogFile << "\n-- Protons in CD (MicroBooNE cut) -----------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_MicroBooNE_CD = " << Chi2_Proton_cut_MicroBooNE_CD << "\n";

    if (Chi2_Proton_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_CD (from histogram) = " << Chi2_Proton_MicroBooNE_Xmax_CD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_MicroBooNE_CD (used in cuts) = " << Chi2_Proton_1e_peak_MicroBooNE_CD << "\n";

    myLogFile << "\n-- Protons in FD (MicroBooNE cut) -----------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_MicroBooNE_FD = " << Chi2_Proton_cut_MicroBooNE_FD << "\n";

    if (Chi2_Proton_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_FD (from histogram) = " << Chi2_Proton_MicroBooNE_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_MicroBooNE_FD (used in cuts) = " << Chi2_Proton_1e_peak_MicroBooNE_FD << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_piplus (MicroBooNE only)">
    myLogFile << "\n-- pipluss in CD (MicroBooNE cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_CD = " << Chi2_piminus_cut_MicroBooNE_CD << "\n";

    if (Chi2_piplus_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_CD (from histogram) = " << Chi2_piplus_1e_peak_MicroBooNE_CD << "\n";
    }

    myLogFile << "Chi2_piplus_MicroBooNE_Xmax_CD (used in cuts) = " << Chi2_piplus_MicroBooNE_Xmax_CD << "\n";

    myLogFile << "\n-- pipluss in FD (MicroBooNE cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_FD = " << Chi2_piminus_cut_MicroBooNE_FD << "\n";

    if (Chi2_piplus_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_FD (from histogram) = " << Chi2_piplus_1e_peak_MicroBooNE_FD << "\n";
    }

    myLogFile << "Chi2_piplus_MicroBooNE_Xmax_FD (used in cuts) = " << Chi2_piplus_MicroBooNE_Xmax_FD << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_piminus (MicroBooNE only)">
    myLogFile << "\n-- piminus in CD (MicroBooNE cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_CD = " << Chi2_piminus_cut_MicroBooNE_CD << "\n";

    if (Chi2_piminus_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_CD (from histogram) = " << Chi2_piminus_1e_peak_MicroBooNE_CD << "\n";
    }

    myLogFile << "Chi2_piminus_MicroBooNE_Xmax_CD (used in cuts) = " << Chi2_piminus_MicroBooNE_Xmax_CD << "\n";

    myLogFile << "\n-- piminus in FD (MicroBooNE cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_FD = " << Chi2_piminus_cut_MicroBooNE_FD << "\n";

    if (Chi2_piminus_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_FD (from histogram) = " << Chi2_piminus_1e_peak_MicroBooNE_FD << "\n";
    }

    myLogFile << "Chi2_piminus_MicroBooNE_Xmax_FD (used in cuts) = " << Chi2_piminus_MicroBooNE_Xmax_FD << "\n\n\n";
    //</editor-fold>

    myLogFile << "===========================================================================\n";
    myLogFile << "Vertex cuts\n";
    myLogFile << "===========================================================================\n";
    myLogFile << "Vertex_Electron_cut_CD = " << Vertex_Electron_cut_CD << "\n";
    myLogFile << "Vertex_Electron_1e_mean_CD = " << Vertex_Electron_1e_mean_CD << "\n";
    myLogFile << "Vertex_Electron_cut_FD = " << Vertex_Electron_cut_FD << "\n";
    myLogFile << "Vertex_Electron_1e_mean_FD = " << Vertex_Electron_1e_mean_FD << "\n\n";

    myLogFile << "Vertex_Proton_cut_CD = " << Vertex_Proton_cut_CD << "\n";
    myLogFile << "Vertex_Proton_1e_mean_CD = " << Vertex_Proton_1e_mean_CD << "\n";
    myLogFile << "Vertex_Proton_cut_FD = " << Vertex_Proton_cut_FD << "\n";
    myLogFile << "Vertex_Proton_1e_mean_FD = " << Vertex_Proton_1e_mean_FD << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "dV cuts\n";
    myLogFile << "===========================================================================\n";

    myLogFile << "\n-- dVx (CD) ---------------------------------------------------------------" << "\n";
    myLogFile << "dVx_cut_CD = " << dVx_cut_CD << "\n";
    myLogFile << "dVx_Xmax_CD (from histogram) = " << dVx_Xmax_CD << "\n";
    myLogFile << "dVx_peak_CD (used in cuts) = " << dVx_peak_CD << "\n";

    myLogFile << "\n-- dVx (FD) ---------------------------------------------------------------" << "\n";
    myLogFile << "dVx_cut_FD = " << dVz_cut_FD << "\n";
    myLogFile << "dVx_Xmax_FD (from histogram) = " << dVx_Xmax_FD << "\n";
    myLogFile << "dVx_peak_FD (used in cuts) = " << dVx_peak_FD << "\n";

    myLogFile << "\n-- dVx (All) --------------------------------------------------------------" << "\n";
    myLogFile << "dVx_cut = " << dVz_cut << "\n";
    myLogFile << "dVx_Xmax (from histogram) = " << dVx_Xmax << "\n";
    myLogFile << "dVx_peak (used in cuts) = " << dVx_peak << "\n";
    myLogFile << "---------------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- dVy (CD) ---------------------------------------------------------------" << "\n";
    myLogFile << "dVy_cut_CD = " << dVy_cut_CD << "\n";
    myLogFile << "dVy_Xmax_CD (from histogram) = " << dVy_Xmax_CD << "\n";
    myLogFile << "dVy_peak_CD (used in cuts) = " << dVy_peak_CD << "\n";

    myLogFile << "\n-- dVy (FD) ---------------------------------------------------------------" << "\n";
    myLogFile << "dVy_cut_FD = " << dVz_cut_FD << "\n";
    myLogFile << "dVy_Xmax_FD (from histogram) = " << dVy_Xmax_FD << "\n";
    myLogFile << "dVy_peak_FD (used in cuts) = " << dVy_peak_FD << "\n";

    myLogFile << "\n-- dVy (All) --------------------------------------------------------------" << "\n";
    myLogFile << "dVy_cut = " << dVz_cut << "\n";
    myLogFile << "dVy_Xmax (from histogram) = " << dVy_Xmax << "\n";
    myLogFile << "dVy_peak (used in cuts) = " << dVy_peak << "\n";
    myLogFile << "---------------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- dVz (CD) ---------------------------------------------------------------" << "\n";
    myLogFile << "dVz_cut_CD = " << dVz_cut_CD << "\n";
    myLogFile << "dVz_Xmax_CD (from histogram) = " << dVz_Xmax_CD << "\n";
    myLogFile << "dVz_peak_CD (used in cuts) = " << dVz_peak_CD << "\n";

    myLogFile << "\n-- dVz (FD) ---------------------------------------------------------------" << "\n";
    myLogFile << "dVz_cut_FD = " << dVz_cut_FD << "\n";
    myLogFile << "dVz_Xmax_FD (from histogram) = " << dVz_Xmax_FD << "\n";
    myLogFile << "dVz_peak_FD (used in cuts) = " << dVz_peak_FD << "\n";

    myLogFile << "\n-- dVz (All) --------------------------------------------------------------" << "\n";
    myLogFile << "dVz_cut = " << dVz_cut << "\n";
    myLogFile << "dVz_Xmax (from histogram) = " << dVz_Xmax << "\n";
    myLogFile << "dVz_peak (used in cuts) = " << dVz_peak << "\n";
    myLogFile << "---------------------------------------------------------------------------" << "\n\n";

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

//    myLogFile << "-- Counts to be added -----------------------------------------------------\n";
//    myLogFile << "#(events) in CD*:\t\t\t\t\t" << "to be added" << "\n";
//    myLogFile << "#(events) in FD*:\t\t\t\t\t" << "to be added" << "\n";
//    myLogFile << "#(e) in CD*:\t\t\t\t\t\t" << num_of_events_with_e_in_CD << "\n";
//    myLogFile << "#(e) in FD*:\t\t\t\t\t\t" << num_of_events_with_e_in_FD << "\n";

    myLogFile << "-- Events with electrons counts -------------------------------------------\n";
    myLogFile << "#(events) w/ at least 1e:\t\t\t" << num_of_events_with_at_least_1e << "\n";
    myLogFile << "#(events) w/ more then 1e:\t\t\t" << num_of_events_more_then_1e << "\n\n";
    myLogFile << "#(events) w/ exactly 1e:\t\t\t\t" << num_of_events_with_exactly_1e << "\n";

    // TODO: add percentage relative to 'num_of_events_with_at_least_1e'
    myLogFile << "-- 1e2X event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2X:\t\t\t\t\t" << num_of_events_with_1e2X << "\n\n";

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
    myLogFile << "#(events) w/ 1e1p:\t\t\t\t\t" << num_of_events_with_1e1p << "\n\n";

    myLogFile << "-- 1e2p event counts ------------------------------------------------------\n";
    myLogFile << "#(events) w/ 1e2p:\t\t\t\t\t" << num_of_events_with_1e2p << "\n\n";

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
    myLogFile << "#(events) 1e2p & Chi2 & dVz cuts (2p):\t" << num_of_events_2p << "\n";
    myLogFile << "#(events) 2p QEL:\t\t\t" << num_of_2p_QEL_events << "\n";
    myLogFile << "#(events) 2p MEC:\t\t\t" << num_of_2p_MEC_events << "\n";
    myLogFile << "#(events) 2p RES:\t\t\t" << num_of_2p_RES_events << "\n";
    myLogFile << "#(events) 2p DIS:\t\t\t" << num_of_2p_DIS_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (2p):\t\t\t" << num_of_2p_QEL_events + num_of_2p_MEC_events + num_of_2p_RES_events + num_of_2p_DIS_events << "\n\n\n";

    myLogFile << "-- MicroBooNE event counts ------------------------------------------------\n";
    myLogFile << "#(events) MicroBooNE BEFORE cuts:\t\t" << num_of_MicroBooNE_events_BC << "\n";
    myLogFile << "#(events) MicroBooNE BC with Neutrons:\t" << num_of_MicroBooNE_events_BC_wNeutrons << "\n";
    myLogFile << "#(events) MicroBooNE BC with pi0:\t\t" << num_of_MicroBooNE_events_BC_wpi0 << "\n";
    myLogFile << "#(events) MicroBooNE BC with pi+:\t\t" << num_of_MicroBooNE_events_BC_wpip << "\n";
    myLogFile << "#(events) MicroBooNE BC with pi-:\t\t" << num_of_MicroBooNE_events_BC_wpim << "\n\n";

    myLogFile << "#(events) MicroBooNE AFTER cuts:\t\t" << num_of_MicroBooNE_events_AC << "\n";
    myLogFile << "#(events) MicroBooNE AC with Neutrons:\t" << num_of_MicroBooNE_events_AC_wNeutrons << "\n";
    myLogFile << "#(events) MicroBooNE AC with pi0:\t\t" << num_of_MicroBooNE_events_AC_wpi0 << "\n";
    myLogFile << "#(events) MicroBooNE AC with pi+:\t\t" << num_of_MicroBooNE_events_AC_wpip << "\n";
    myLogFile << "#(events) MicroBooNE AC with pi-:\t\t" << num_of_MicroBooNE_events_AC_wpim << "\n\n\n";

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

//    cout << "-- Counts to be added -----------------------------------------------------\n";
//    cout << "#(events) in CD*:\t\t\t" << "to be added" << "\n";
//    cout << "#(events) in FD*:\t\t\t" << "to be added" << "\n";
//    cout << "#(e) in CD*:\t\t\t\t" << num_of_events_with_e_in_CD << "\n";
//    cout << "#(e) in FD*:\t\t\t\t" << num_of_events_with_e_in_FD << "\n\n";

    cout << "-- Events with electrons counts -------------------------------------------\n";
    cout << "#(events) w/ at least 1e:\t\t" << num_of_events_with_at_least_1e << "\n";
    cout << "#(events) w/ more then 1e:\t\t" << num_of_events_more_then_1e << "\n";
    cout << "#(events) w/ exactly 1e:\t\t" << num_of_events_with_exactly_1e << "\n\n";

    // TODO: add percentage relative to 'num_of_events_with_at_least_1e'
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
    cout << "QEL + MEC + RES + DIS (FD):\t\t" << num_of_QEL_1e2p_FD_events + num_of_MEC_1e2p_FD_events + num_of_RES_1e2p_FD_events + num_of_DIS_1e2p_FD_events
         << "\n";
    cout << "#(events) 1e2p & e Chi2 cuts FD:\t" << num_of_events_1e2p_w_eChi2_cut_only_FD << "\n";
    cout << "#(events) 1e2p & p Chi2 cuts FD:\t" << num_of_events_1e2p_w_pChi2_cut_only_FD << "\n";
    cout << "#(events) 1e2p & all Chi2 cuts FD*:\t" << num_of_events_1e2p_w_allChi2_cuts_FD << "\n\n";

    cout << "#(events) 1e2p & all Chi2 cuts:\t\t" << num_of_events_1e2p_w_allChi2_cuts << "\n\n";

    cout << "-- 2p event counts --------------------------------------------------------\n";
    cout << "#(events) 1e2p & Chi2 & dVz cuts (2p):\t" << num_of_events_2p << "\n";
    cout << "#(events) 2p QEL:\t\t\t" << num_of_2p_QEL_events << "\n";
    cout << "#(events) 2p MEC:\t\t\t" << num_of_2p_MEC_events << "\n";
    cout << "#(events) 2p RES:\t\t\t" << num_of_2p_RES_events << "\n";
    cout << "#(events) 2p DIS:\t\t\t" << num_of_2p_DIS_events << "\n";
    cout << "QEL + MEC + RES + DIS (2p):\t\t" << num_of_2p_QEL_events + num_of_2p_MEC_events + num_of_2p_RES_events + num_of_2p_DIS_events << "\n\n";

    cout << "-- MicroBooNE event counts ------------------------------------------------\n";
    cout << "#(events) MicroBooNE BEFORE cuts:\t" << num_of_MicroBooNE_events_BC << "\n";
    cout << "#(events) MicroBooNE BC with Neutrons:\t" << num_of_MicroBooNE_events_BC_wNeutrons << "\n";
    cout << "#(events) MicroBooNE BC with pi0:\t" << num_of_MicroBooNE_events_BC_wpi0 << "\n";
    cout << "#(events) MicroBooNE BC with pi+:\t" << num_of_MicroBooNE_events_BC_wpip << "\n";
    cout << "#(events) MicroBooNE BC with pi-:\t" << num_of_MicroBooNE_events_BC_wpim << "\n\n";

    cout << "#(events) MicroBooNE AFTER cuts:\t" << num_of_MicroBooNE_events_AC << "\n";
    cout << "#(events) MicroBooNE AC with Neutrons:\t" << num_of_MicroBooNE_events_AC_wNeutrons << "\n";
    cout << "#(events) MicroBooNE AC with pi0:\t" << num_of_MicroBooNE_events_AC_wpi0 << "\n";
    cout << "#(events) MicroBooNE AC with pi+:\t" << num_of_MicroBooNE_events_AC_wpip << "\n";
    cout << "#(events) MicroBooNE AC with pi-:\t" << num_of_MicroBooNE_events_AC_wpim << "\n\n";

    cout << "---------------------------------------------------------------------------\n";
    cout << "\t\t\tExecution variables\n";
    cout << "---------------------------------------------------------------------------\n\n";

    cout << "WorkingDirectory:\t" << WorkingDirectory << "\n";
    cout << "plots_path:\t\t" << plots_path << "\n";
    cout << "AnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n";
    cout << "AnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n";
    cout << "AnalyseFile:\t\t" << AnalyseFile << "\n";
//    cout << "AnalyseFileDirContent:\t" << AnalyseFileDirContent << "\n";
    cout << "Settings mode:\t\t'" << file_name << "'\n\n";
//    cout << "filePath:\t" << filePath << "\n";
    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
    cout << "Beam Energy:\t\t" << beamE << "\n\n";

    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n\n";
    //</editor-fold>

    //</editor-fold>

}