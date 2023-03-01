
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
#include "source/classes/hPlots/hPlot1D.h"

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
    cout << "plots_path:\t\t" << plots_path << "\n\n";
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

// Cuts settings --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Cuts settings">
    bool apply_cuts = false;

    /* HTCC cut */
    bool apply_Nphe_cut = true;

    /* Chi2 cuts */
    bool apply_chi2_cuts_2p = true, apply_chi2_cuts_MicroBooNE = true;

    /* Vertex cuts */
    bool apply_Vz_cuts = true, apply_dVz_cuts = true;

    /* Sampling Fraction (SF) cut */
    bool apply_SF_cuts = true;

    /* ECAL fiducial (edge) cuts */
    bool apply_ECAL_fiducial_cut = true;

    /* DC fiducial (edge) cuts */
    bool apply_DC_fiducial_cut = true;

    /* Momentum cuts */
    bool apply_momentum_cuts_2p = true, apply_momentum_cuts_1n1p = false, apply_momentum_cuts_MicroBooNE = true;

    if (apply_cuts == false) {
        cout << "Cuts are disabled.\n\n\n";

        apply_momentum_cuts_2p = apply_momentum_cuts_1n1p = apply_momentum_cuts_MicroBooNE = apply_SF_cuts = apply_ECAL_fiducial_cut = apply_DC_fiducial_cut = false;
        apply_Nphe_cut = apply_chi2_cuts_2p = apply_chi2_cuts_MicroBooNE = false;
    } else {
        cout << "Cuts are enabled. Cut settings:\n";
        cout << "apply_Nphe_cut:\t\t\t" << BoolToString(apply_Nphe_cut) << "\n";
        cout << "apply_chi2_cuts_2p:\t\t" << BoolToString(apply_chi2_cuts_2p) << "\n";
        cout << "apply_chi2_cuts_MicroBooNE:\t" << BoolToString(apply_chi2_cuts_MicroBooNE) << "\n";
        cout << "apply_Vz_cuts:\t\t\t" << BoolToString(apply_Vz_cuts) << "\n";
        cout << "apply_dVz_cuts:\t\t\t" << BoolToString(apply_dVz_cuts) << "\n";
        cout << "apply_SF_cuts:\t\t\t" << BoolToString(apply_SF_cuts) << "\n";
        cout << "apply_ECAL_fiducial_cut:\t" << BoolToString(apply_ECAL_fiducial_cut) << "\n";
        cout << "apply_DC_fiducial_cut:\t\t" << BoolToString(apply_DC_fiducial_cut) << "\n";
        cout << "apply_momentum_cuts_2p:\t\t" << BoolToString(apply_momentum_cuts_2p) << "\n";
        cout << "apply_momentum_cuts_1n1p:\t" << BoolToString(apply_momentum_cuts_1n1p) << "\n";
        cout << "apply_momentum_cuts_MicroBooNE:\t" << BoolToString(apply_momentum_cuts_MicroBooNE) << "\n\n\n";
    }
    //</editor-fold>

//  Checking directories ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Creating directories">
    cout << "Creating plot directories...\n\n";

    string Plots_Folder = plots_path; // Plots_Folder = Parent_Folder
    system(("mkdir -p " + Plots_Folder).c_str()); // clear old stuff in Parent_Folder
    system(("rm -r " + Plots_Folder + "*").c_str()); // clear old stuff in Parent_Folder

    //<editor-fold desc="Cut parameters plots directories">

    //<editor-fold desc="Number of Photo-electrons (Nphe) plots directories">
    bool create_Nphe_Dir = true;
    string Nphe_Parent_Directory = "01_Cuts_plots/01_Nphe_plots";
//    string Nphe_Parent_Directory = "Nphe_plots";
    TFolder *Nphe_Folder = new TFolder(Nphe_Parent_Directory.c_str(), Nphe_Parent_Directory.c_str());
    string Nphe_Daughter_Folders[] = {"", "01_1e_cut", "02_2p",};

    for (string folders_name: Nphe_Daughter_Folders) {
        MakeDirectory(create_Nphe_Dir, Nphe_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Nphe_Folder, Nphe_Parent_Directory, folders_name);
    }

    plots->Add(Nphe_Folder);

    string Nphe_Directory = Plots_Folder + "/" + Nphe_Parent_Directory + "/";

    // 1e2p - i.e. plots before all other cuts
    // before cuts - i.e. plots before Nphe cuts
    string Nphe_1e_cut_BC_Directory = Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Nphe_Daughter_Folders[1] + "/";

    // after cuts - i.e. plots after Nphe cuts
    string Nphe_1e_cut_AC_Directory = Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Nphe_Daughter_Folders[1] + "/";

    // 2p - i.e. plots after Nphe and all other cuts
    string Nphe_2p_Directory = Plots_Folder + "/" + Nphe_Parent_Directory + "/" + Nphe_Daughter_Folders[2] + "/";
    //</editor-fold>

    //<editor-fold desc="Chi2 plots directories">
    bool create_chi2_Dir = true;
    string Chi2_Parent_Directory = "01_Cuts_plots/02_Chi2_plots";
//    string Chi2_Parent_Directory = "Chi2_plots";
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
    string Vertex_Parent_Directory = "01_Cuts_plots/03_Vertex_plots";
//    string Vertex_Parent_Directory = "Vertex_plots";
    TFolder *Vertex_Folder = new TFolder(Vertex_Parent_Directory.c_str(), Vertex_Parent_Directory.c_str());
    string Vertex_Daughter_Folders[] = {"", "02_dV_plots/02_1e2p", "02_dV_plots/03_2p", "02_dV_plots/01_MicroBooNE",
                                        "02_dV_plots/01_MicroBooNE/BC-AC_cut_tests", "01_Vertex_components_plots/01_1e_cut/01_Electrons",
                                        "01_Vertex_components_plots/01_1e_cut/02_Protons", "01_Vertex_components_plots/01_1e_cut/03_Kplus",
                                        "01_Vertex_components_plots/01_1e_cut/04_Kminus", "01_Vertex_components_plots/01_1e_cut/05_Piplus",
                                        "01_Vertex_components_plots/01_1e_cut/06_Piminus"};

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

    string Vertex_Electron_1e_Vx_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[5] + "/";
    string Vertex_Electron_1e_Vy_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[5] + "/";
    string Vertex_Electron_1e_Vz_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[5] + "/";
    string Vertex_Proton_1e_Vx_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[6] + "/";
    string Vertex_Proton_1e_Vy_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[6] + "/";
    string Vertex_Proton_1e_Vz_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[6] + "/";
    string Vertex_Kplus_1e_Vx_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[7] + "/";
    string Vertex_Kplus_1e_Vy_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[7] + "/";
    string Vertex_Kplus_1e_Vz_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[7] + "/";
    string Vertex_Kminus_1e_Vx_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[8] + "/";
    string Vertex_Kminus_1e_Vy_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[8] + "/";
    string Vertex_Kminus_1e_Vz_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[8] + "/";
    string Vertex_piplus_1e_Vx_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[9] + "/";
    string Vertex_piplus_1e_Vy_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[9] + "/";
    string Vertex_piplus_1e_Vz_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[9] + "/";
    string Vertex_piminus_1e_Vx_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[10] + "/";
    string Vertex_piminus_1e_Vy_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[10] + "/";
    string Vertex_piminus_1e_Vz_Directory = Plots_Folder + "/" + Vertex_Parent_Directory + "/" + Vertex_Daughter_Folders[10] + "/";
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) plots directories">
    bool create_SF_Dir = true;
    string SF_Parent_Directory = "01_Cuts_plots/04_SF_plots";
//    string SF_Parent_Directory = "SF_plots";
    TFolder *SF_Folder = new TFolder(SF_Parent_Directory.c_str(), SF_Parent_Directory.c_str());
    string SF_Daughter_Folders[] = {"", "01_1e_cut", "01_1e_cut/SF_plots", "01_1e_cut/SF_VS_P_e_plots", "02_2p", "02_2p/SF_plots",
                                    "02_2p/SF_VS_P_e_plots"};

    for (string folders_name: SF_Daughter_Folders) {
        MakeDirectory(create_SF_Dir, SF_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(SF_Folder, SF_Parent_Directory, folders_name);
    }

    plots->Add(SF_Folder);

    string SF_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/";

    string SF_plots_1e_cut_BC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[2] + "/";
    string SF_VS_P_e_plots_1e_cut_BC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[3] + "/";
    string SF_plots_1e_cut_AC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[2] + "/";
    string SF_VS_P_e_plots_1e_cut_AC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[3] + "/";

    // 1e2p - i.e. plots before all other cuts
    // before cuts - i.e. plots before SF cuts
    string SF_plots_1e2p_BC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[2] + "/";
    string SF_VS_P_e_plots_1e2p_BC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[3] + "/";

    // after cuts - i.e. plots after SF cuts
    string SF_plots_1e2p_AC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[2] + "/";
    string SF_VS_P_e_plots_1e2p_AC_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[3] + "/";

    // 2p - i.e. plots after SF and all other cuts
    string SF_plots_2p_cuts_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[5] + "/";
    string SF_VS_P_e_plots_2p_cuts_Directory = Plots_Folder + "/" + SF_Parent_Directory + "/" + SF_Daughter_Folders[6] + "/";
    //</editor-fold>

    //<editor-fold desc="Fiducial histograms plots directories">
    bool create_fiducial_Dir = true;
    string fiducial_Parent_Directory = "01_Cuts_plots/05_Fiducial_plots";
//    string fiducial_Parent_Directory = "Fiducial_plots";
    TFolder *fiducial_Folder = new TFolder(fiducial_Parent_Directory.c_str(), fiducial_Parent_Directory.c_str());
    string fiducial_Daughter_Folders[] = {"", "1e_cut", "1e_cut/PCAL", "2p", "2p/PCAL"};

    for (string folders_name: fiducial_Daughter_Folders) {
        MakeDirectory(create_fiducial_Dir, fiducial_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(fiducial_Folder, fiducial_Parent_Directory, folders_name);
    }

    plots->Add(fiducial_Folder);

    //TODO: add fiducial plots by reaction?
    string fiducial_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/";

    // 1e2p - i.e. plots before all other cuts
    // before/after cuts - i.e. plots before/after fiducial cuts
    string fiducial_plots_1e_BC_PCAL_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[2] + "/";
    string fiducial_plots_1e_AC_PCAL_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[2] + "/";

    // 2p - i.e. plots after fiducial and all other cuts
    string fiducial_plots_2p_cuts_histograms_PCAL_Directory = Plots_Folder + "/" + fiducial_Parent_Directory + "/" + fiducial_Daughter_Folders[4] + "/";
    //</editor-fold>

    //<editor-fold desc="Momentum plots directories">
    bool create_Momentum_Dir = true;
    string Momentum_Parent_Directory = "01_Cuts_plots/06_Momentum_plots";
//    string Momentum_Parent_Directory = "Momentum_plots";
    TFolder *Momentum_Folder = new TFolder(Momentum_Parent_Directory.c_str(), Momentum_Parent_Directory.c_str());
    string Momentum_Daughter_Folders[] = {"", "01_MicroBooNE", "01_MicroBooNE/BC-AC_cut_tests", "02_1e2p", "03_2p"};

    for (string folders_name: Momentum_Daughter_Folders) {
        MakeDirectory(create_Momentum_Dir, Momentum_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Momentum_Folder, Momentum_Parent_Directory, folders_name);
    }

    plots->Add(Momentum_Folder);

    string Momentum_MicroBooNE_cut_tests_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[1] + "/";
    string Momentum_MicroBooNE_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[2] + "/";

    string Momentum_1e2p_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[3] + "/";

    string Momentum_2p_Directory = Plots_Folder + "/" + Momentum_Parent_Directory + "/" + Momentum_Daughter_Folders[4] + "/";
    //</editor-fold>

    //</editor-fold>

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

    //<editor-fold desc="Angle plots directories">
    bool create_Angle_Dir = true;
    string Angle_Parent_Directory = "03_Angle_plots";
    TFolder *Angle_Folder = new TFolder(Angle_Parent_Directory.c_str(), Angle_Parent_Directory.c_str());
    string Angle_Daughter_Folders[] = {"", "01_All_e", "01_All_e/01_Theta_e_All_e_plots", "01_All_e/02_Phi_e_All_e_plots", "01_All_e/03_Theta_e_VS_Phi_e_All_e_plots",
                                       "02_1e_cut", "02_1e_cut/01_Theta_e_1e_cut_plots", "02_1e_cut/02_Phi_e_1e_cut_plots", "02_1e_cut/03_Theta_e_VS_Phi_e_1e_cut_plots",
                                       "03_MicroBooNE", "03_MicroBooNE/01_Theta_e_MicroBooNE_plots", "03_MicroBooNE/02_Phi_e_MicroBooNE_plots",
                                       "03_MicroBooNE/03_Theta_e_VS_Phi_e_MicroBooNE_plots",
                                       "04_1e2X", "04_1e2X/01_Theta_e_1e2X_plots", "04_1e2X/02_Phi_e_1e2X_plots", "04_1e2X/03_Theta_e_VS_Phi_e_1e2X_plots",
                                       "05_1e2p", "05_1e2p/01_Theta_e_1e2p_plots", "05_1e2p/02_Phi_e_1e2p_plots", "05_1e2p/03_Theta_e_VS_Phi_e_1e2p_plots",
                                       "06_2p", "06_2p/01_Theta_e_2p_plots", "06_2p/02_Phi_e_2p_plots", "06_2p/03_Theta_e_VS_Phi_e_2p_plots"};

    for (string folders_name: Angle_Daughter_Folders) {
        MakeDirectory(create_Angle_Dir, Angle_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Angle_Folder, Angle_Parent_Directory, folders_name);
    }

    plots->Add(Angle_Folder);

    string Theta_e_All_e_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[2] + "/";
    string Phi_e_All_e_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[3] + "/";
    string Theta_e_VS_Phi_e_All_e_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[4] + "/";

    string Theta_e_1e_cut_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[6] + "/";
    string Phi_e_1e_cut_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[7] + "/";
    string Theta_e_VS_Phi_e_1e_cut_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[8] + "/";

    string Theta_e_MicroBooNE_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[10] + "/";
    string Phi_e_MicroBooNE_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[11] + "/";
    string Theta_e_VS_Phi_e_MicroBooNE_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[12] + "/";

    string Theta_e_1e2X_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[14] + "/";
    string Phi_e_1e2X_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[15] + "/";
//    string Theta_e_VS_Phi_e_1e2X_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[16] + "/";

    string Theta_e_1e2p_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[18] + "/";
    string Phi_e_1e2p_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[19] + "/";
//    string Theta_e_VS_Phi_e_1e2p_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[20] + "/";

    string Theta_e_2p_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[22] + "/";
    string Phi_e_2p_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[23] + "/";
    string Theta_e_VS_Phi_e_2p_Directory = Plots_Folder + "/" + Angle_Parent_Directory + "/" + Angle_Daughter_Folders[24] + "/";
    //</editor-fold>

    //<editor-fold desc="Q2 plots directories">
    bool create_Q2_Dir = true;
    string Q2_Parent_Directory = "04_Q2_plots";
    TFolder *Q2_Folder = new TFolder(Q2_Parent_Directory.c_str(), Q2_Parent_Directory.c_str());
    string Q2_Daughter_Folders[] = {"", "01_All_e", "02_1e_cut", "03_MicroBooNE", "04_1e2X", "05_1e2p", "06_2p"};

    for (string folders_name: Q2_Daughter_Folders) {
        MakeDirectory(create_Q2_Dir, Q2_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Q2_Folder, Q2_Parent_Directory, folders_name);
    }

    plots->Add(Q2_Folder);

    string Q2_All_e_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[1] + "/";

    string Q2_1e_cut_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[2] + "/";

    string Q2_MicroBooNE_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[3] + "/";

    string Q2_1e2X_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[4] + "/";

    string Q2_1e2p_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[5] + "/";

    string Q2_2p_Directory = Plots_Folder + "/" + Q2_Parent_Directory + "/" + Q2_Daughter_Folders[6] + "/";
    //</editor-fold>

    //<editor-fold desc="E_e plots directories">
    bool create_E_e_Dir = true;
    string E_e_Parent_Directory = "05_E_e_plots";
    TFolder *E_e_Folder = new TFolder(E_e_Parent_Directory.c_str(), E_e_Parent_Directory.c_str());
    string E_e_Daughter_Folders[] = {"", "01_1e_cut", "01_1e_cut/E_e_plots", "01_1e_cut/E_e_VS_Theta_e", "02_MicroBooNE", "02_MicroBooNE/E_e_plots",
                                     "02_MicroBooNE/E_e_VS_Theta_e", "03_2p", "03_2p/E_e_plots", "03_2p/E_e_VS_Theta_e"};

//    string E_e_Daughter_Folders[] = {"", "E_e_plots", "E_e_VS_Theta_e", "E_e_plots/2p", "E_e_VS_Theta_e/2p"};

    for (string folders_name: E_e_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, E_e_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(E_e_Folder, E_e_Parent_Directory, folders_name);
    }

    plots->Add(E_e_Folder);

    string E_e_All_Int_1e_cut_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
//    string E_e_QEL_1e_cut_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
//    string E_e_MEC_1e_cut_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
//    string E_e_RES_1e_cut_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";
//    string E_e_DIS_1e_cut_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[2] + "/";

    string E_e_VS_Theta_e_All_Int_1e_cut_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
//    string E_e_VS_Theta_e_1e_cut_QEL_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
//    string E_e_VS_Theta_e_1e_cut_MEC_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
//    string E_e_VS_Theta_e_1e_cut_RES_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";
//    string E_e_VS_Theta_e_1e_cut_DIS_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[3] + "/";

    string E_e_All_Int_MicroBooNE_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[5] + "/";
//    string E_e_MicroBooNE_QEL_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[5] + "/";
//    string E_e_MicroBooNE_MEC_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[5] + "/";
//    string E_e_MicroBooNE_RES_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[5] + "/";
//    string E_e_MicroBooNE_DIS_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[5] + "/";

    string E_e_VS_Theta_e_All_Int_MicroBooNE_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[6] + "/";
//    string E_e_VS_Theta_e_MicroBooNE_QEL_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[6] + "/";
//    string E_e_VS_Theta_e_MicroBooNE_MEC_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[6] + "/";
//    string E_e_VS_Theta_e_MicroBooNE_RES_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[6] + "/";
//    string E_e_VS_Theta_e_MicroBooNE_DIS_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[6] + "/";

    string E_e_All_Int_2p_stack_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[7] + "/";
    string E_e_All_Int_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[8] + "/";
//    string E_e_QEL_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[8] + "/";
//    string E_e_MEC_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[8] + "/";
//    string E_e_RES_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[8] + "/";
//    string E_e_DIS_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[8] + "/";

    string E_e_VS_Theta_e_All_Int_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[9] + "/";
//    string E_e_VS_Theta_e_QEL_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[9] + "/";
//    string E_e_VS_Theta_e_MEC_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[9] + "/";
//    string E_e_VS_Theta_e_RES_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[9] + "/";
//    string E_e_VS_Theta_e_DIS_2p_Directory = Plots_Folder + "/" + E_e_Parent_Directory + "/" + E_e_Daughter_Folders[9] + "/";
    //</editor-fold>

    //<editor-fold desc="ETrans plots directories">
    bool create_ETrans_Dir = true;
    string ETrans_Parent_Directory = "ETrans_plots";
    TFolder *ETrans_Folder = new TFolder(ETrans_Parent_Directory.c_str(), ETrans_Parent_Directory.c_str());
    string ETrans_Daughter_Folders[] = {"", "01_2p", "01_2p/01_Around_15_Deg", "01_2p/01_Around_15_Deg/All_interactions", "01_2p/01_Around_15_Deg/QEL_only",
                                        "01_2p/01_Around_15_Deg/MEC_only", "01_2p/01_Around_15_Deg/RES_only", "01_2p/01_Around_15_Deg/DIS_only"};
//    string ETrans_Daughter_Folders[] = {"", "2p", "2p/Around_15_Deg", "2p/Around_15_Deg/All_interactions", "2p/Around_15_Deg/QEL_only",
//                                        "2p/Around_15_Deg/MEC_only", "2p/Around_15_Deg/RES_only", "2p/Around_15_Deg/DIS_only"};

    for (string folders_name: ETrans_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, ETrans_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(ETrans_Folder, ETrans_Parent_Directory, folders_name);
    }

    plots->Add(ETrans_Folder);

    string ETrans_15_stack_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[2] + "/";
    string ETrans_All_Int_15_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[3] + "/";
    string ETrans_QEL_15_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[4] + "/";
    string ETrans_MEC_15_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[5] + "/";
    string ETrans_RES_15_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[6] + "/";
    string ETrans_DIS_15_Directory = Plots_Folder + "/" + ETrans_Parent_Directory + "/" + ETrans_Daughter_Folders[7] + "/";

    //</editor-fold>

    //<editor-fold desc="Ecal plots directories">
    bool create_Ecal_Dir = true;
    string Ecal_Parent_Directory = "Ecal_rec_plots";
    TFolder *Ecal_Folder = new TFolder(Ecal_Parent_Directory.c_str(), Ecal_Parent_Directory.c_str());
    string Ecal_Daughter_Folders[] = {"", "2p", "2p/All_interactions", "2p/QEL_only", "2p/MEC_only", "2p/RES_only", "2p/DIS_only"};

    for (string folders_name: Ecal_Daughter_Folders) {
        MakeDirectory(create_E_e_Dir, Ecal_Parent_Directory, folders_name, false, Plots_Folder);
        TFolderAdder(Ecal_Folder, Ecal_Parent_Directory, folders_name);
    }

    plots->Add(Ecal_Folder);

    string Ecal_stack_2p_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[1] + "/";
    string Ecal_All_Int_2p_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[2] + "/";
    string Ecal_QEL_2p_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[3] + "/";
    string Ecal_MEC_2p_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[4] + "/";
    string Ecal_RES_2p_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[5] + "/";
    string Ecal_DIS_2p_Directory = Plots_Folder + "/" + Ecal_Parent_Directory + "/" + Ecal_Daughter_Folders[6] + "/";
    //</editor-fold>

    //</editor-fold>

// Calculation settings -------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Calculation settings">
    bool calculate_2p = true, calculate_MicroBooNE = true;
    //</editor-fold>

// Plot settings --------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Plot selector">
    //TODO: add these var to log file
    bool wider_margin = true;

    /* Cut parameter plots */
    bool Nphe_plots = true, Chi2_plots = true, Vertex_plots = true, SF_plots = true, fiducial_plots = true, Momentum_plots = true;

    /* Beta vs. P plots */
    bool Beta_vs_P_plots = true;

    /* Angle plots */
    bool Angle_plots = true, Theta_e_plots = true, Phi_e_plots = true;
    if (Angle_plots == false) { Theta_e_plots = Phi_e_plots = false; }

    /* Q2 plots */
    bool Q2_plots = true;

    /* E_e plots */
    bool E_e_plots = true;

    /* ET plots */
    bool ETrans_plots = true;
    bool ETrans_all_plots = true, ETrans_All_Int_plots = true, ETrans_QEL_plots = true, ETrans_MEC_plots = true, ETrans_RES_plots = true, ETrans_DIS_plots = true;
    if (ETrans_plots == false) { ETrans_all_plots = ETrans_QEL_plots = ETrans_MEC_plots = ETrans_RES_plots = ETrans_DIS_plots = false; }

    /* Ecal plots */
    bool Ecal_plots = true;

    /* MicroBooNE plots */
    bool MicroBooNE_plots = false;
    //</editor-fold>

// Normalization settings -----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Normalization settings">
    bool normalize_master = false;

    bool norm_Nphe_plots = false, norm_Chi2_plots = false, norm_Vertex_plots = false, norm_SF_plots = false, norm_Fiducial_plots = false;

    bool norm_Angle_plots = false, norm_Q2_plots = false, norm_E_e_plots = false, norm_ET_plots = false, norm_Ecal_plots = false;

    if (normalize_master == false) { // Disable all normalizations if normalize_master == false
        norm_Nphe_plots = norm_Chi2_plots = norm_Vertex_plots = norm_SF_plots = norm_Fiducial_plots = false;
        norm_Angle_plots = norm_Q2_plots = norm_E_e_plots = norm_ET_plots = norm_Ecal_plots = false;

        cout << "\nAll normalizations are disabled.\n\n";  // and no change to custom_FSI_status
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

// Histogram limits -----------------------------------------------------------------------------------------------------------------------------------------------------

    //TODO: see if I can remove some of these hist. limits.
    //<editor-fold desc="Histogram limits">

    //<editor-fold desc="Histogram limits for every case">
    /* Chi2 plots */
    double Chi2_boundary = 35;
    if (apply_cuts == true) { Chi2_boundary = 15; }

    /* Vertex plots */
    double Vertex_boundary = 25;
    double dV_boundary = 25;
    if (apply_cuts == true) { Vertex_boundary = 7.5; }
    if (apply_cuts == true) { dV_boundary = 7.5; }

    /* Beta vs. P plots */
    double Beta_boundary = 3;
    double P_boundary = beamE * 1.425;
    if (apply_cuts == true) { Beta_boundary = 1.1; }
    if (apply_cuts == true) { P_boundary = beamE * 1.1; }

//    //<editor-fold desc="Theta histograms">
//
//    //<editor-fold desc="Theta histograms (2p)">
//    theta_lp_upper_lim_2p = 60;
//    theta_lp_lower_lim_2p = 0;
//    theta_p1_upper_lim_2p = 190;
//    theta_p1_lower_lim_2p = -10;
//    theta_p2_upper_lim_2p = 190;
//    theta_p2_lower_lim_2p = -10;
//    dtheta_upper_lim_2p = 190;
//    dtheta_lower_lim_2p = -10;
//    //</editor-fold>
//
//    //<editor-fold desc="Theta histograms (1n1p)">
//    theta_lp_upper_lim_1n1p = 60;
//    theta_lp_lower_lim_1n1p = 0;
//    theta_p_upper_lim_1n1p = 190;
//    theta_p_lower_lim_1n1p = -10;
//    theta_n_upper_lim_1n1p = 190;
//    theta_n_lower_lim_1n1p = -10;
//    dtheta_upper_lim_1n1p = 190;
//    dtheta_lower_lim_1n1p = -10;
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //<editor-fold desc="Phi histograms">
//
//    //<editor-fold desc="Phi histograms (2p)">
//    phi_lp_upper_lim_2p = 200;
//    phi_lp_lower_lim_2p = -200;
//    phi_p1_upper_lim_2p = 200;
//    phi_p1_lower_lim_2p = -200;
//    phi_p2_upper_lim_2p = 200;
//    phi_p2_lower_lim_2p = -200;
//    dphi_upper_lim_2p = 360;
//    dphi_lower_lim_2p = -360;
//    //</editor-fold>
//
//    //<editor-fold desc="Phi histograms (1n1p)">
//    phi_lp_upper_lim_1n1p = 200;
//    phi_lp_lower_lim_1n1p = -200;
//    phi_p_upper_lim_1n1p = 200;
//    phi_p_lower_lim_1n1p = -200;
//    phi_n_upper_lim_1n1p = 200;
//    phi_n_lower_lim_1n1p = -200;
//    dphi_upper_lim_1n1p = 360;
//    dphi_lower_lim_1n1p = -360;
//    //</editor-fold>
//
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE plots">
//
//    //<editor-fold desc="MicroBooNE gamma plots (unweighted)">
//    Gamma_Lab_upper_lim = 1;
//    Gamma_Lab_lower_lim = -1;
//    Gamma_mu_p_tot_upper_lim = 1;
//    Gamma_mu_p_tot_lower_lim = -1;
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE gamma plots (Q4 weighted)">
//    Gamma_Lab_weighted_upper_lim = 1;
//    Gamma_Lab_weighted_lower_lim = -1;
//    Gamma_mu_p_tot_weighted_upper_lim = 1;
//    Gamma_mu_p_tot_weighted_lower_lim = -1;
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE gamma plots (no pions, for every interaction)">
//    Gamma_Lab_noPions_All_Int_upper_lim = 1;
//    Gamma_Lab_noPions_All_Int_lower_lim = -1;
//    Gamma_Lab_noPions_All_Int_weighted_upper_lim = 1;
//    Gamma_Lab_noPions_All_Int_weighted_lower_lim = -1;
//    Gamma_Lab_noPions_QEL_upper_lim = 1;
//    Gamma_Lab_noPions_QEL_lower_lim = -1;
//    Gamma_Lab_noPions_QEL_weighted_upper_lim = 1;
//    Gamma_Lab_noPions_QEL_weighted_lower_lim = -1;
//    Gamma_Lab_noPions_MEC_upper_lim = 1;
//    Gamma_Lab_noPions_MEC_lower_lim = -1;
//    Gamma_Lab_noPions_MEC_weighted_upper_lim = 1;
//    Gamma_Lab_noPions_MEC_weighted_lower_lim = -1;
//    Gamma_Lab_noPions_RES_upper_lim = 1;
//    Gamma_Lab_noPions_RES_lower_lim = -1;
//    Gamma_Lab_noPions_RES_weighted_upper_lim = 1;
//    Gamma_Lab_noPions_RES_weighted_lower_lim = -1;
//    Gamma_Lab_noPions_DIS_upper_lim = 1;
//    Gamma_Lab_noPions_DIS_lower_lim = -1;
//    Gamma_Lab_noPions_DIS_weighted_upper_lim = 1;
//    Gamma_Lab_noPions_DIS_weighted_lower_lim = -1;
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE dP_T plots">
//    dP_T_hist_upper_lim = 2;
//    dP_T_hist_lower_lim = 0;
//    dP_T_hist_weighted_upper_lim = 2;
//    dP_T_hist_weighted_lower_lim = 0;
//
//    /*
//    if (FSI_status == true) {
//        dP_T_hist_upper_lim = 2;
//        dP_T_hist_lower_lim = 0;
//        dP_T_hist_weighted_upper_lim = 2;
//        dP_T_hist_weighted_lower_lim = 0;
//    } else if (FSI_status == false) {
//        dP_T_hist_upper_lim = 2;
//        dP_T_hist_lower_lim = 0;
//        dP_T_hist_weighted_upper_lim = 2;
//        dP_T_hist_weighted_lower_lim = 0;
//    }
//     */
//    //</editor-fold>
//
//    //<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
//    P_L_hist_upper_lim = 3.0;
//    P_L_hist_lower_lim = 0;
//    P_R_hist_upper_lim = 3.0;
//    P_R_hist_lower_lim = 0;
//    P_lp_hist_upper_lim = 3.0;
//    P_lp_hist_lower_lim = 0;
//    P_pion_hist_upper_lim = 0.1;
//    P_pion_hist_lower_lim = 0;
//    //</editor-fold>
//
//    //</editor-fold>

    //</editor-fold>

//    //<editor-fold desc="Histogram limits by cases">
//
//    if (file_name == "general_file") {
//
//        //<editor-fold desc="12C_2222GeV_GTEST19_10b_00_000 histogram limits">
//
//        //<editor-fold desc="Energy histograms">
//
//        //<editor-fold desc="Energy histograms (2p)">
////            fsEl_upper_lim_2p = 110.;
////            fsEl_lower_lim_2p = 1;
//        fsEl_upper_lim_2p = 300;
//        fsEl_lower_lim_2p = -10;
////        fsEl_upper_lim_2p = 6.5;
////        fsEl_lower_lim_2p = 0.;
//        fsEl_QEL_upper_lim_2p = 2.3;
//        fsEl_QEL_lower_lim_2p = 0;
//        fsEl_MEC_upper_lim_2p = 2.3;
//        fsEl_MEC_lower_lim_2p = 0;
//        fsEl_RES_upper_lim_2p = 2.3;
//        fsEl_RES_lower_lim_2p = 0;
//        fsEl_DIS_upper_lim_2p = 2.3;
//        fsEl_DIS_lower_lim_2p = 0;
//        fsEl_VS_theta_lp_upper_lim_2p_x = 70.0;
////            fsEl_VS_theta_lp_upper_lim_2p_x = 90.0;
//        fsEl_VS_theta_lp_lower_lim_2p_x = 0;
//        fsEl_VS_theta_lp_upper_lim_2p_y = 2.3;
//        fsEl_VS_theta_lp_lower_lim_2p_y = 0;
//
//        /*
//        if (FSI_status == true) {
////            fsEl_upper_lim_2p = 110.;
////            fsEl_lower_lim_2p = 1;
//            fsEl_upper_lim_2p = 6.;
//            fsEl_lower_lim_2p = 0.;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_lp_upper_lim_2p_x = 70.0;
////            fsEl_VS_theta_lp_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_lp_lower_lim_2p_x = 0;
//            fsEl_VS_theta_lp_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_lp_lower_lim_2p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_2p = 2.21;
//            fsEl_lower_lim_2p = 1;
//            fsEl_QEL_upper_lim_2p = 2.3;
//            fsEl_QEL_lower_lim_2p = 0;
//            fsEl_MEC_upper_lim_2p = 2.3;
//            fsEl_MEC_lower_lim_2p = 0;
//            fsEl_RES_upper_lim_2p = 2.3;
//            fsEl_RES_lower_lim_2p = 0;
//            fsEl_DIS_upper_lim_2p = 2.3;
//            fsEl_DIS_lower_lim_2p = 0;
//            fsEl_VS_theta_lp_upper_lim_2p_x = 70.0;
////            fsEl_VS_theta_lp_upper_lim_2p_x = 90.0;
//            fsEl_VS_theta_lp_lower_lim_2p_x = 0;
//            fsEl_VS_theta_lp_upper_lim_2p_y = 2.3;
//            fsEl_VS_theta_lp_lower_lim_2p_y = 0;
//        }
//         */
//        //</editor-fold>
//
//        //<editor-fold desc="Energy histograms (1n1p)">
//        fsEl_upper_lim_1n1p = 300;
//        fsEl_lower_lim_1n1p = -10;
////        fsEl_upper_lim_1n1p = 2.3;
////        fsEl_lower_lim_1n1p = 1;
//        fsEl_QEL_upper_lim_1n1p = 2.3;
//        fsEl_QEL_lower_lim_1n1p = 0;
//        fsEl_MEC_upper_lim_1n1p = 2.3;
//        fsEl_MEC_lower_lim_1n1p = 0;
//        fsEl_RES_upper_lim_1n1p = 2.3;
//        fsEl_RES_lower_lim_1n1p = 0;
//        fsEl_DIS_upper_lim_1n1p = 2.3;
//        fsEl_DIS_lower_lim_1n1p = 0;
//        fsEl_VS_theta_lp_upper_lim_1n1p_x = 70.0;
////            fsEl_VS_theta_lp_upper_lim_1n1p_x = 90.0;
//        fsEl_VS_theta_lp_lower_lim_1n1p_x = 0;
//        fsEl_VS_theta_lp_upper_lim_1n1p_y = 2.3;
//        fsEl_VS_theta_lp_lower_lim_1n1p_y = 0;
//
//        /*        if (FSI_status == true) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_x = 70.0;
////            fsEl_VS_theta_lp_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_lp_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_lp_lower_lim_1n1p_y = 0;
//        } else if (FSI_status == false) {
//            fsEl_upper_lim_1n1p = 2.3;
//            fsEl_lower_lim_1n1p = 1;
//            fsEl_QEL_upper_lim_1n1p = 2.3;
//            fsEl_QEL_lower_lim_1n1p = 0;
//            fsEl_MEC_upper_lim_1n1p = 2.3;
//            fsEl_MEC_lower_lim_1n1p = 0;
//            fsEl_RES_upper_lim_1n1p = 2.3;
//            fsEl_RES_lower_lim_1n1p = 0;
//            fsEl_DIS_upper_lim_1n1p = 2.3;
//            fsEl_DIS_lower_lim_1n1p = 0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_x = 70.0;
////            fsEl_VS_theta_lp_upper_lim_1n1p_x = 90.0;
//            fsEl_VS_theta_lp_lower_lim_1n1p_x = 0;
//            fsEl_VS_theta_lp_upper_lim_1n1p_y = 2.3;
//            fsEl_VS_theta_lp_lower_lim_1n1p_y = 0;
//        }
//*/
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms">
//
//        //<editor-fold desc="Energy Transfer histograms (All Interactions)">
//
//        //<editor-fold desc="Energy Transfer histograms (All Interactions, 2p)">
//        E_Trans_all_ang_all_int_upper_lim_2p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_2p = 0;
//        E_Trans15_all_upper_lim_2p = 1.7;
//        E_Trans15_all_lower_lim_2p = 0;
//        E_Trans45_all_upper_lim_2p = 1.7;
//        E_Trans45_all_lower_lim_2p = 0;
//        E_Trans90_all_upper_lim_2p = 1.7;
//        E_Trans90_all_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (All Interactions, 1n1p)">
//        E_Trans_all_ang_all_int_upper_lim_1n1p = 1.7;
//        E_Trans_all_ang_all_int_lower_lim_1n1p = 0;
//        E_Trans15_all_upper_lim_1n1p = 1.7;
//        E_Trans15_all_lower_lim_1n1p = 0;
//        E_Trans45_all_upper_lim_1n1p = 1.7;
//        E_Trans45_all_lower_lim_1n1p = 0;
//        E_Trans90_all_upper_lim_1n1p = 1.7;
//        E_Trans90_all_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only)">
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 2p)">
//        E_Trans15_QEL_upper_lim_2p = 1.7;
//        E_Trans15_QEL_lower_lim_2p = 0;
//        E_Trans45_QEL_upper_lim_2p = 1.7;
//        E_Trans45_QEL_lower_lim_2p = 0;
//        E_Trans90_QEL_upper_lim_2p = 1.7;
//        E_Trans90_QEL_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (QEL only, 1n1p)">
//        E_Trans15_QEL_upper_lim_1n1p = 1.7;
//        E_Trans15_QEL_lower_lim_1n1p = 0;
//        E_Trans45_QEL_upper_lim_1n1p = 1.7;
//        E_Trans45_QEL_lower_lim_1n1p = 0;
//        E_Trans90_QEL_upper_lim_1n1p = 1.7;
//        E_Trans90_QEL_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only)">
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 2p)">
//        E_Trans15_MEC_upper_lim_2p = 1.7;
//        E_Trans15_MEC_lower_lim_2p = 0;
//        E_Trans45_MEC_upper_lim_2p = 1.7;
//        E_Trans45_MEC_lower_lim_2p = 0;
//        E_Trans90_MEC_upper_lim_2p = 1.7;
//        E_Trans90_MEC_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (MEC only, 1n1p)">
//        E_Trans15_MEC_upper_lim_1n1p = 1.7;
//        E_Trans15_MEC_lower_lim_1n1p = 0;
//        E_Trans45_MEC_upper_lim_1n1p = 1.7;
//        E_Trans45_MEC_lower_lim_1n1p = 0;
//        E_Trans90_MEC_upper_lim_1n1p = 1.7;
//        E_Trans90_MEC_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only)">
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 2p)">
//        E_Trans15_RES_upper_lim_2p = 1.7;
//        E_Trans15_RES_lower_lim_2p = 0;
//        E_Trans45_RES_upper_lim_2p = 1.7;
//        E_Trans45_RES_lower_lim_2p = 0;
//        E_Trans90_RES_upper_lim_2p = 1.7;
//        E_Trans90_RES_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (RES only, 1n1p)">
//        E_Trans15_RES_upper_lim_1n1p = 1.7;
//        E_Trans15_RES_lower_lim_1n1p = 0;
//        E_Trans45_RES_upper_lim_1n1p = 1.7;
//        E_Trans45_RES_lower_lim_1n1p = 0;
//        E_Trans90_RES_upper_lim_1n1p = 1.7;
//        E_Trans90_RES_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only)">
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 2p)">
//        E_Trans15_DIS_upper_lim_2p = 1.7;
//        E_Trans15_DIS_lower_lim_2p = 0;
//        E_Trans45_DIS_upper_lim_2p = 1.7;
//        E_Trans45_DIS_lower_lim_2p = 0;
//        E_Trans90_DIS_upper_lim_2p = 1.7;
//        E_Trans90_DIS_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Energy Transfer histograms (DIS only, 1n1p)">
//        E_Trans15_DIS_upper_lim_1n1p = 1.7;
//        E_Trans15_DIS_lower_lim_1n1p = 0;
//        E_Trans45_DIS_upper_lim_1n1p = 1.7;
//        E_Trans45_DIS_lower_lim_1n1p = 0;
//        E_Trans90_DIS_upper_lim_1n1p = 1.7;
//        E_Trans90_DIS_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Inclusive Energy Transfer histograms">
//        E_Trans15_all_inclusive_upper_lim = 6;
//        E_Trans15_all_inclusive_lower_lim = 0;
//        E_Trans15_QEL_inclusive_upper_lim = 6;
//        E_Trans15_QEL_inclusive_lower_lim = 0;
//        E_Trans15_MEC_inclusive_upper_lim = 6;
//        E_Trans15_MEC_inclusive_lower_lim = 0;
//        E_Trans15_RES_inclusive_upper_lim = 6;
//        E_Trans15_RES_inclusive_lower_lim = 0;
//        E_Trans15_DIS_inclusive_upper_lim = 6;
//        E_Trans15_DIS_inclusive_lower_lim = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_x = 6;
//        E_Trans_VS_q_all_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_all_inclusive_upper_lim_y = 6;
//        E_Trans_VS_q_all_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_x = 6;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_QEL_inclusive_upper_lim_y = 6;
//        E_Trans_VS_q_QEL_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_x = 6;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_x = 0;
//        E_Trans_VS_q_MEC_inclusive_upper_lim_y = 6;
//        E_Trans_VS_q_MEC_inclusive_lower_lim_y = 0;
//        E_Trans_VS_q3_all_upper_lim_x_2p = 6;
//        E_Trans_VS_q3_all_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_2p = 6;
//        E_Trans_VS_q3_all_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_2p = 6;
//        E_Trans_VS_q3_QEL_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_2p = 6;
//        E_Trans_VS_q3_QEL_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_2p = 6;
//        E_Trans_VS_q3_MEC_lower_lim_x_2p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_2p = 6;
//        E_Trans_VS_q3_MEC_lower_lim_y_2p = 0;
//        E_Trans_VS_q3_all_upper_lim_x_1n1p = 6;
//        E_Trans_VS_q3_all_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_all_upper_lim_y_1n1p = 6;
//        E_Trans_VS_q3_all_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_x_1n1p = 6;
//        E_Trans_VS_q3_QEL_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_QEL_upper_lim_y_1n1p = 6;
//        E_Trans_VS_q3_QEL_lower_lim_y_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_x_1n1p = 6;
//        E_Trans_VS_q3_MEC_lower_lim_x_1n1p = 0;
//        E_Trans_VS_q3_MEC_upper_lim_y_1n1p = 6;
//        E_Trans_VS_q3_MEC_lower_lim_y_1n1p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal reconstruction histograms">
//
//        //<editor-fold desc="E_cal range">
//        E_cal_QEL_upper_lim_range = 2.35;
//        E_cal_QEL_lower_lim_range = 2.11;
//        E_cal_MEC_upper_lim_range = 2.35;
//        E_cal_MEC_lower_lim_range = 2.11;
//        E_cal_RES_upper_lim_range = 2.35;
//        E_cal_RES_lower_lim_range = 2.11;
//        E_cal_DIS_upper_lim_range = 2.35;
//        E_cal_DIS_lower_lim_range = 2.11;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal reconstruction histograms (2p)">
//        E_cal_QEL_upper_lim_2p = E_cal_QEL_upper_lim_range;
//        E_cal_QEL_lower_lim_2p = E_cal_QEL_lower_lim_range;
//        E_cal_MEC_upper_lim_2p = E_cal_MEC_upper_lim_range;
//        E_cal_MEC_lower_lim_2p = E_cal_MEC_lower_lim_range;
//        E_cal_RES_upper_lim_2p = E_cal_RES_upper_lim_range;
//        E_cal_RES_lower_lim_2p = E_cal_RES_lower_lim_range;
//        E_cal_DIS_upper_lim_2p = E_cal_DIS_upper_lim_range;
//        E_cal_DIS_lower_lim_2p = E_cal_DIS_lower_lim_range;
//        //</editor-fold>
//
//        //<editor-fold desc="E_cal reconstruction histograms (1n1p)">
//        E_cal_QEL_upper_lim_1n1p = E_cal_QEL_upper_lim_range;
//        E_cal_QEL_lower_lim_1n1p = E_cal_QEL_lower_lim_range;
//        E_cal_MEC_upper_lim_1n1p = E_cal_MEC_upper_lim_range;
//        E_cal_MEC_lower_lim_1n1p = E_cal_MEC_lower_lim_range;
//        E_cal_RES_upper_lim_1n1p = E_cal_RES_upper_lim_range;
//        E_cal_RES_lower_lim_1n1p = E_cal_RES_lower_lim_range;
//        E_cal_DIS_upper_lim_1n1p = E_cal_DIS_upper_lim_range;
//        E_cal_DIS_lower_lim_1n1p = E_cal_DIS_lower_lim_range;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms">
//
//        //<editor-fold desc="Momentum histograms (2p)">
//        P_L_hist_upper_lim_2p = 6.0;
//        P_L_hist_lower_lim_2p = 0;
//        P_R_hist_upper_lim_2p = 6.0;
//        P_R_hist_lower_lim_2p = 0;
//        P_lp_hist_upper_lim_2p = 6.0;
//        P_lp_hist_lower_lim_2p = 0;
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms (1n1p)">
//        P_p_hist_upper_lim_1n1p = 6.0;
//        P_p_hist_lower_lim_1n1p = 0;
//        P_n_hist_upper_lim_1n1p = 6.0;
//        P_n_hist_lower_lim_1n1p = 0;
//        P_lp_hist_upper_lim_1n1p = 6.0;
//        P_lp_hist_lower_lim_1n1p = 0;
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//    }
//
//    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                      Histogram definitions                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    //<editor-fold desc="Histogram definitions">

    cout << "\nDefining histograms...";

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Number of Photo-electrons (Nphe) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Nphe histograms (FD only)">

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms (1e cut, FD only)">
    THStack *sNphe_1e_cut_FD = new THStack("N_{phe} (1e Cut, FD)", "Nphe Histogram (All Int., 1e Cut, FD);N_{phe}");

    TH1D *hNphe_1e_cut_BC_FD, *hNphe_1e_cut_AC_FD;
    string hNphe_1e_cut_BC_FD_Dir, hNphe_1e_cut_AC_FD_Dir;

    if (apply_cuts == false) {
        hNphe_1e_cut_BC_FD = new TH1D("N_{phe} BC (1e Cut, FD)", "#Photo-electrons in HTCC (N_{phe}) Before Cuts (1e Cut, FD);N_{phe}", 200, 0, 40);
        hNphe_1e_cut_AC_FD = new TH1D("N_{phe} AC (1e Cut, FD)", "#Photo-electrons in HTCC (N_{phe}) After Cuts (1e Cut, FD);N_{phe}", 200, 0, 40);
        hNphe_1e_cut_BC_FD_Dir = Nphe_1e_cut_BC_Directory, hNphe_1e_cut_AC_FD_Dir = Nphe_1e_cut_AC_Directory;
    } else {
        hNphe_1e_cut_BC_FD = new TH1D("N_{phe} (1e Cut, FD)", "#Photo-electrons in HTCC N_{phe} (1e Cut, FD);N_{phe}", 200, 0, 40);
        hNphe_1e_cut_BC_FD_Dir = Nphe_1e_cut_BC_Directory, hNphe_1e_cut_AC_FD_Dir = Nphe_1e_cut_AC_Directory;
    }
    //</editor-fold>

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms (2p, FD only)">
    THStack *sNphe_2p_FD = new THStack("N_{phe} (2p, FD)", "#Photo-electrons in HTCC (N_{phe}) Histogram (2p, FD);N_{phe}");

    TH1D *hNphe_2p_FD = new TH1D("N_{phe} (2p, FD)", "#Photo-electrons in HTCC (N_{phe}) (2p, FD);N_{phe}", 200, 0, 40);
    string hNphe_2p_FD_Dir = Nphe_2p_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">

    //<editor-fold desc="Chi2 plots (no #(e) cut)">
    /* Plots of chi2 with no cut on number of electrons - NOT used later! */

    THStack *sChi2_Electron_All_e = new THStack("#chi^{2}_{e^{-}} (no #(e) cut, CD & FD)", "#chi^{2}_{e^{-}} Histogram (no #(e) cut, CD & FD);#chi^{2}_{e^{-}};");
//    TH1D *hChi2_Electron_CD = new TH1D("#chi^{2}_{e^{-}} (no #(e) cut, CD)", "#chi^{2}_{e^{-}} Histogram (no #(e) cut, CD);#chi^{2}_{e^{-}};", 500, 0, beamE);
    TH1D *hChi2_Electron_FD = new TH1D("#chi^{2}_{e^{-}} (no #(e) cut, FD)", "#chi^{2}_{e^{-}} Histogram (no #(e) cut, FD);#chi^{2}_{e^{-}};",
                                       500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_CD_Dir = Chi2_All_e_Directory, hChi2_Electron_FD_Dir = Chi2_All_e_Directory;

    THStack *sChi2_Proton_All_e = new THStack("#chi^{2}_{p} (no #(e) cut, CD & FD)", "#chi^{2}_{p} Histogram (no #(e) cut, CD & FD);#chi^{2}_{p};");
    TH1D *hChi2_Proton_CD = new TH1D("#chi^{2}_{p} (no #(e) cut, CD)", "#chi^{2}_{p} Histogram (no #(e) cut, CD);#chi^{2}_{p};",
                                     500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_FD = new TH1D("#chi^{2}_{p} (no #(e) cut, FD)", "#chi^{2}_{p} Histogram (no #(e) cut, FD);#chi^{2}_{p};",
                                     500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Proton_CD_Dir = Chi2_All_e_Directory, hChi2_Proton_FD_Dir = Chi2_All_e_Directory;

    THStack *sChi2_Kplus_All_e = new THStack("#chi^{2}_{K^{+}} (no #(e) cut, CD & FD)", "#chi^{2}_{K^{+}} Histogram (no #(e) cut, CD & FD);#chi^{2}_{K^{+}};");
    TH1D *hChi2_Kplus_CD = new TH1D("#chi^{2}_{K^{+}} (no #(e) cut, CD)", "#chi^{2}_{K^{+}} Histogram (no #(e) cut, CD);#chi^{2}_{K^{+}};",
                                    500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Kplus_FD = new TH1D("#chi^{2}_{K^{+}} (no #(e) cut, FD)", "#chi^{2}_{K^{+}} Histogram (no #(e) cut, FD);#chi^{2}_{K^{+}};",
                                    500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Kplus_CD_Dir = Chi2_All_e_Directory, hChi2_Kplus_FD_Dir = Chi2_All_e_Directory;

    THStack *sChi2_Kminus_All_e = new THStack("#chi^{2}_{K^{-}} (no #(e) cut, CD & FD)", "#chi^{2}_{K^{-}} Histogram (no #(e) cut, CD & FD);#chi^{2}_{K^{-}};");
    TH1D *hChi2_Kminus_CD = new TH1D("#chi^{2}_{K^{-}} (no #(e) cut, CD)", "#chi^{2}_{K^{-}} Histogram (no #(e) cut, CD);#chi^{2}_{K^{-}};",
                                     500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Kminus_FD = new TH1D("#chi^{2}_{K^{-}} (no #(e) cut, FD)", "#chi^{2}_{K^{-}} Histogram (no #(e) cut, FD);#chi^{2}_{K^{-}};",
                                     500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Kminus_CD_Dir = Chi2_All_e_Directory, hChi2_Kminus_FD_Dir = Chi2_All_e_Directory;

    THStack *sChi2_piplus_All_e = new THStack("#chi^{2}_{#pi^{+}} (no #(e) cut, CD & FD)", "#chi^{2}_{#pi^{+}} Histogram (no #(e) cut, CD & FD);#chi^{2}_{#pi^{+}};");
    TH1D *hChi2_piplus_CD = new TH1D("#chi^{2}_{#pi^{+}} (no #(e) cut, CD)", "#chi^{2}_{#pi^{+}} Histogram (no #(e) cut, CD);#chi^{2}_{#pi^{+}};",
                                     500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_piplus_FD = new TH1D("#chi^{2}_{#pi^{+}} (no #(e) cut, FD)", "#chi^{2}_{#pi^{+}} Histogram (no #(e) cut, FD);#chi^{2}_{#pi^{+}};",
                                     500, -Chi2_boundary, Chi2_boundary);
    string hChi2_piplus_CD_Dir = Chi2_All_e_Directory, hChi2_piplus_FD_Dir = Chi2_All_e_Directory;

    THStack *sChi2_piminus_All_e = new THStack("#chi^{2}_{#pi^{-}} (no #(e) cut, CD & FD)", "#chi^{2}_{#pi^{-}} Histogram (no #(e) cut, CD & FD);#chi^{2}_{#pi^{-}};");
    TH1D *hChi2_piminus_CD = new TH1D("#chi^{2}_{#pi^{-}} (no #(e) cut, CD)", "#chi^{2}_{#pi^{-}} Histogram (no #(e) cut, CD);#chi^{2}_{#pi^{-}};",
                                      500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_piminus_FD = new TH1D("#chi^{2}_{#pi^{-}} (no #(e) cut, FD)", "#chi^{2}_{#pi^{-}} Histogram (no #(e) cut, FD);#chi^{2}_{#pi^{-}};",
                                      500, -Chi2_boundary, Chi2_boundary);
    string hChi2_piminus_CD_Dir = Chi2_All_e_Directory, hChi2_piminus_FD_Dir = Chi2_All_e_Directory;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e cut)">
    /* Plots of chi2 with no cut on number of electrons - used later to apply cuts with my mechanism (NOT Justin's ana) */

    THStack *sChi2_Electron_1e_cut = new THStack("#chi^{2}_{e^{-}} (1e cut, CD & FD)", "#chi^{2}_{e^{-}} Histogram (1e cut, CD & FD);#chi^{2}_{e^{-}};");
//    TH1D *hChi2_Electron_1e_cut_CD = new TH1D("#chi^{2}_{e^{-}} (1e cut, CD)", "#chi^{2}_{e^{-}} Histogram (1e cut, CD);#chi^{2}_{e^{-}};", 500, 0, beamE);
    TH1D *hChi2_Electron_1e_cut_FD = new TH1D("#chi^{2}_{e^{-}} (1e cut, FD)", "#chi^{2}_{e^{-}} Histogram (1e cut, FD);#chi^{2}_{e^{-}};",
                                              500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Electron_1e_cut_CD_Dir = Chi2_1e_Directory, hChi2_Electron_1e_cut_FD_Dir = Chi2_1e_Directory;

    THStack *sChi2_Proton_1e_cut = new THStack("#chi^{2}_{p} (1e cut, CD & FD)", "#chi^{2}_{p} Histogram (1e cut, CD & FD);#chi^{2}_{p};");
    TH1D *hChi2_Proton_1e_cut_CD = new TH1D("#chi^{2}_{p} (1e cut, CD)", "#chi^{2}_{p} Histogram (1e cut, CD);#chi^{2}_{p};",
                                            500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Proton_1e_cut_FD = new TH1D("#chi^{2}_{p} (1e cut, FD)", "#chi^{2}_{p} Histogram (1e cut, FD);#chi^{2}_{p};",
                                            500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Proton_1e_cut_CD_Dir = Chi2_1e_Directory, hChi2_Proton_1e_cut_FD_Dir = Chi2_1e_Directory;

    THStack *sChi2_Kplus_1e_cut = new THStack("#chi^{2}_{K^{+}} (1e cut, CD & FD)", "#chi^{2}_{K^{+}} Histogram (1e cut, CD & FD);#chi^{2}_{K^{+}};");
    TH1D *hChi2_Kplus_1e_cut_CD = new TH1D("#chi^{2}_{K^{+}} (1e cut, CD)", "#chi^{2}_{K^{+}} Histogram (1e cut, CD);#chi^{2}_{K^{+}};",
                                           500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Kplus_1e_cut_FD = new TH1D("#chi^{2}_{K^{+}} (1e cut, FD)", "#chi^{2}_{K^{+}} Histogram (1e cut, FD);#chi^{2}_{K^{+}};",
                                           500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Kplus_1e_cut_CD_Dir = Chi2_1e_Directory, hChi2_Kplus_1e_cut_FD_Dir = Chi2_1e_Directory;

    THStack *sChi2_Kminus_1e_cut = new THStack("#chi^{2}_{K^{-}} (1e cut, CD & FD)", "#chi^{2}_{K^{-}} Histogram (1e cut, CD & FD);#chi^{2}_{K^{-}};");
    TH1D *hChi2_Kminus_1e_cut_CD = new TH1D("#chi^{2}_{K^{-}} (1e cut, CD)", "#chi^{2}_{K^{-}} Histogram (1e cut, CD);#chi^{2}_{K^{-}};",
                                            500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_Kminus_1e_cut_FD = new TH1D("#chi^{2}_{K^{-}} (1e cut, FD)", "#chi^{2}_{K^{-}} Histogram (1e cut, FD);#chi^{2}_{K^{-}};",
                                            500, -Chi2_boundary, Chi2_boundary);
    string hChi2_Kminus_1e_cut_CD_Dir = Chi2_1e_Directory, hChi2_Kminus_1e_cut_FD_Dir = Chi2_1e_Directory;

    THStack *sChi2_piplus_1e_cut = new THStack("#chi^{2}_{#pi^{+}} (1e cut, CD & FD)", "#chi^{2}_{#pi^{+}} Histogram (1e cut, CD & FD);#chi^{2}_{#pi^{+}};");
    TH1D *hChi2_piplus_1e_cut_CD = new TH1D("#chi^{2}_{#pi^{+}} (1e cut, CD)", "#chi^{2}_{#pi^{+}} Histogram (1e cut, CD);#chi^{2}_{#pi^{+}};",
                                            500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_piplus_1e_cut_FD = new TH1D("#chi^{2}_{#pi^{+}} (1e cut, FD)", "#chi^{2}_{#pi^{+}} Histogram (1e cut, FD);#chi^{2}_{#pi^{+}};",
                                            500, -Chi2_boundary, Chi2_boundary);
    string hChi2_piplus_1e_cut_CD_Dir = Chi2_1e_Directory, hChi2_piplus_1e_cut_FD_Dir = Chi2_1e_Directory;

    THStack *sChi2_piminus_1e_cut = new THStack("#chi^{2}_{#pi^{-}} (1e cut, CD & FD)", "#chi^{2}_{#pi^{-}} Histogram (1e cut, CD & FD);#chi^{2}_{#pi^{-}};");
    TH1D *hChi2_piminus_1e_cut_CD = new TH1D("#chi^{2}_{#pi^{-}} (1e cut, CD)", "#chi^{2}_{#pi^{-}} Histogram (1e cut, CD);#chi^{2}_{#pi^{-}};",
                                             500, -Chi2_boundary, Chi2_boundary);
    TH1D *hChi2_piminus_1e_cut_FD = new TH1D("#chi^{2}_{#pi^{-}} (1e cut, FD)", "#chi^{2}_{#pi^{-}} Histogram (1e cut, FD);#chi^{2}_{#pi^{-}};",
                                             500, -Chi2_boundary, Chi2_boundary);
    string hChi2_piminus_1e_cut_CD_Dir = Chi2_1e_Directory, hChi2_piminus_1e_cut_FD_Dir = Chi2_1e_Directory;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (MicroBooNE)">
    /* Here we plot chi2 three times:
     * First - before applying individual chi2 cuts
     * Second - after applying individual chi2 cuts
     * Third - after applying ALL cuts, not just individual chi2 cuts (= c-cut) */

    //<editor-fold desc="Testing Chi2 before MicroBooNE cuts test">
    THStack *sChi2_Electron_MicroBooNE = new THStack("#chi^{2}_{e^{-}} (MicroBooNE, CD & FD)", "#chi^{2}_{e^{-}} (MicroBooNE, CD & FD);#chi^{2}_{e^{-}};");
//    TH1D *hChi2_Electron_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{e^{-}} BC (MicroBooNE, CD)", "#chi^{2}_{e^{-}} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{e^{-}};",
//                                                    500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
//    TH1D *hChi2_Electron_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{e^{-}} AC (MicroBooNE, CD)", "#chi^{2}_{e^{-}} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{e^{-}};",
//                                                    500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *hChi2_Electron_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{e^{-}} BC (MicroBooNE, FD)", "#chi^{2}_{e^{-}} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{e^{-}};",
                                                     500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    TH1D *hChi2_Electron_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{e^{-}} AC (MicroBooNE, FD)", "#chi^{2}_{e^{-}} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{e^{-}};",
                                                     500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    string hChi2_Electron_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_Electron_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string hChi2_Electron_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_Electron_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;

    THStack *sChi2_Proton_MicroBooNE = new THStack("#chi^{2}_{p} (MicroBooNE, CD & FD)", "#chi^{2}_{p} (MicroBooNE, CD & FD);#chi^{2}_{p};");
    TH1D *hChi2_Proton_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{p} BC (MicroBooNE, CD)", "#chi^{2}_{p} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{p};",
                                                   500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *hChi2_Proton_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{p} AC (MicroBooNE, CD)", "#chi^{2}_{p} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{p};",
                                                   500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *hChi2_Proton_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{p} BC (MicroBooNE, FD)", "#chi^{2}_{p} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{p};",
                                                   500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    TH1D *hChi2_Proton_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{p} AC (MicroBooNE, FD)", "#chi^{2}_{p} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{p};",
                                                   500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string hChi2_Proton_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_Proton_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string hChi2_Proton_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_Proton_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;

    THStack *sChi2_piplus_MicroBooNE = new THStack("#chi^{2}_{#pi^{+}} (MicroBooNE, CD & FD)", "#chi^{2}_{#pi^{+}} (MicroBooNE, CD & FD);#chi^{2}_{#pi^{+}};");
    TH1D *hChi2_piplus_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{#pi^{+}} BC (MicroBooNE, CD)",
                                                   "#chi^{2}_{#pi^{+}} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{+}};",
                                                   500, -1.5 * Chi2_piplus_cut_MicroBooNE_CD, 1.5 * Chi2_piplus_cut_MicroBooNE_CD);
    TH1D *hChi2_piplus_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{#pi^{+}} AC (MicroBooNE, CD)",
                                                   "#chi^{2}_{#pi^{+}} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{+}};",
                                                   500, -1.5 * Chi2_piplus_cut_MicroBooNE_CD, 1.5 * Chi2_piplus_cut_MicroBooNE_CD);
    TH1D *hChi2_piplus_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{#pi^{+}} BC (MicroBooNE, FD)",
                                                   "#chi^{2}_{#pi^{+}} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{+}};",
                                                   500, -1.5 * Chi2_piplus_cut_MicroBooNE_FD, 1.5 * Chi2_piplus_cut_MicroBooNE_FD);
    TH1D *hChi2_piplus_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{#pi^{+}} AC (MicroBooNE, FD)",
                                                   "#chi^{2}_{#pi^{+}} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{+}};",
                                                   500, -1.5 * Chi2_piplus_cut_MicroBooNE_FD, 1.5 * Chi2_piplus_cut_MicroBooNE_FD);
    string hChi2_piplus_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_piplus_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string hChi2_piplus_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_piplus_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;

    THStack *sChi2_piminus_MicroBooNE = new THStack("#chi^{2}_{#pi^{-}} (MicroBooNE, CD & FD)", "#chi^{2}_{#pi^{-}} (MicroBooNE, CD & FD);#chi^{2}_{#pi^{-}};");
    TH1D *hChi2_piminus_MicroBooNE_BC_CD = new TH1D("#chi^{2}_{#pi^{-}} BC (MicroBooNE, CD)",
                                                    "#chi^{2}_{#pi^{-}} Histogram Before C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{-}};",
                                                    500, -1.5 * Chi2_piminus_cut_MicroBooNE_CD, 1.5 * Chi2_piminus_cut_MicroBooNE_CD);
    TH1D *hChi2_piminus_MicroBooNE_AC_CD = new TH1D("#chi^{2}_{#pi^{-}} AC (MicroBooNE, CD)",
                                                    "#chi^{2}_{#pi^{-}} Histogram After C-Cut (MicroBooNE, CD);#chi^{2}_{#pi^{-}};",
                                                    500, -1.5 * Chi2_piminus_cut_MicroBooNE_CD, 1.5 * Chi2_piminus_cut_MicroBooNE_CD);
    TH1D *hChi2_piminus_MicroBooNE_BC_FD = new TH1D("#chi^{2}_{#pi^{-}} BC (MicroBooNE, FD)",
                                                    "#chi^{2}_{#pi^{-}} Histogram Before C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{-}};",
                                                    500, -1.5 * Chi2_piminus_cut_MicroBooNE_FD, 1.5 * Chi2_piminus_cut_MicroBooNE_FD);
    TH1D *hChi2_piminus_MicroBooNE_AC_FD = new TH1D("#chi^{2}_{#pi^{-}} AC (MicroBooNE, FD)",
                                                    "#chi^{2}_{#pi^{-}} Histogram After C-Cut (MicroBooNE, FD);#chi^{2}_{#pi^{-}};",
                                                    500, -1.5 * Chi2_piminus_cut_MicroBooNE_FD, 1.5 * Chi2_piminus_cut_MicroBooNE_FD);
    string hChi2_piminus_MicroBooNE_BC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_piminus_MicroBooNE_BC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    string hChi2_piminus_MicroBooNE_AC_CD_Dir = Chi2_MicroBooNE_cut_tests_Directory, hChi2_piminus_MicroBooNE_AC_FD_Dir = Chi2_MicroBooNE_cut_tests_Directory;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots after MicroBooNE cuts">
//    TH1D *hChi2_Electron_MicroBooNE_CD = new TH1D("#chi^{2}_{e^{-}} (MicroBooNE-CC, CD)", "#chi^{2}_{e^{-}} Histogram (MicroBooNE-CC, CD);#chi^{2}_{e^{-}};",
//                                                 500, -1.5 * Chi2_Electron_cut_MicroBooNE_CD, 1.5 * Chi2_Electron_cut_MicroBooNE_CD);
    TH1D *hChi2_Electron_MicroBooNE_FD = new TH1D("#chi^{2}_{e^{-}} (MicroBooNE-CC, FD)", "#chi^{2}_{e^{-}} Histogram (MicroBooNE-CC, FD);#chi^{2}_{e^{-}};",
                                                  500, -1.5 * Chi2_Electron_cut_MicroBooNE_FD, 1.5 * Chi2_Electron_cut_MicroBooNE_FD);
    string hChi2_Electron_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, hChi2_Electron_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;

    TH1D *hChi2_Proton_MicroBooNE_CD = new TH1D("#chi^{2}_{p} (MicroBooNE-CC, CD)", "#chi^{2}_{p} Histogram (MicroBooNE-CC, CD);#chi^{2}_{p};",
                                                500, -1.5 * Chi2_Proton_cut_MicroBooNE_CD, 1.5 * Chi2_Proton_cut_MicroBooNE_CD);
    TH1D *hChi2_Proton_MicroBooNE_FD = new TH1D("#chi^{2}_{p} (MicroBooNE-CC, FD)", "#chi^{2}_{p} Histogram (MicroBooNE-CC, FD);#chi^{2}_{p};",
                                                500, -1.5 * Chi2_Proton_cut_MicroBooNE_FD, 1.5 * Chi2_Proton_cut_MicroBooNE_FD);
    string hChi2_Proton_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, hChi2_Proton_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;

    TH1D *hChi2_piplus_MicroBooNE_CD = new TH1D("#chi^{2}_{#pi^{+}} (MicroBooNE-CC, CD)", "#chi^{2}_{#pi^{+}} Histogram (MicroBooNE-CC, CD);#chi^{2}_{#pi^{+}};",
                                                500, -1.5 * Chi2_piplus_cut_MicroBooNE_CD, 1.5 * Chi2_piplus_cut_MicroBooNE_CD);
    TH1D *hChi2_piplus_MicroBooNE_FD = new TH1D("#chi^{2}_{#pi^{+}} (MicroBooNE-CC, FD)", "#chi^{2}_{#pi^{+}} Histogram (MicroBooNE-CC, FD);#chi^{2}_{#pi^{+}};",
                                                500, -1.5 * Chi2_piplus_cut_MicroBooNE_FD, 1.5 * Chi2_piplus_cut_MicroBooNE_FD);
    string hChi2_piplus_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, hChi2_piplus_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;

    TH1D *hChi2_piminus_MicroBooNE_CD = new TH1D("#chi^{2}_{#pi^{-}} (MicroBooNE-CC, CD)", "#chi^{2}_{#pi^{-}} Histogram (MicroBooNE-CC, CD);#chi^{2}_{#pi^{-}};",
                                                 500, -1.5 * Chi2_piminus_cut_MicroBooNE_CD, 1.5 * Chi2_piminus_cut_MicroBooNE_CD);
    TH1D *hChi2_piminus_MicroBooNE_FD = new TH1D("#chi^{2}_{#pi^{-}} (MicroBooNE-CC, FD)", "#chi^{2}_{#pi^{-}} Histogram (MicroBooNE-CC, FD);#chi^{2}_{#pi^{-}};",
                                                 500, -1.5 * Chi2_piminus_cut_MicroBooNE_FD, 1.5 * Chi2_piminus_cut_MicroBooNE_FD);
    string hChi2_piminus_MicroBooNE_CD_Dir = Chi2_MicroBooNE_Directory, hChi2_piminus_MicroBooNE_FD_Dir = Chi2_MicroBooNE_Directory;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e2p)">
    THStack *sChi2_Electron_1e2p = new THStack("#chi^{2}_{e^{-}} (1e2p, CD & FD)", "#chi^{2}_{e^{-}} Histogram (1e2p, CD & FD);#chi^{2}_{e^{-}};");
    THStack *sChi2_Proton_1e2p = new THStack("#chi^{2}_{p} (1e2p, CD & FD)", "#chi^{2}_{p} Histogram (1e2p, CD & FD);#chi^{2}_{p};");

    TH1D *hChi2_Electron_1e2p_BC_CD, *hChi2_Electron_1e2p_BC_FD, *hChi2_Proton_1e2p_BC_CD, *hChi2_Proton_1e2p_BC_FD;
    string hChi2_Electron_1e2p_BC_CD_Dir, hChi2_Electron_1e2p_BC_FD_Dir, hChi2_Proton_1e2p_BC_CD_Dir, hChi2_Proton_1e2p_BC_FD_Dir;
    TH1D *hChi2_Electron_1e2p_AC_CD, *hChi2_Electron_1e2p_AC_FD, *hChi2_Proton_1e2p_AC_CD, *hChi2_Proton_1e2p_AC_FD;
    string hChi2_Electron_1e2p_AC_CD_Dir, hChi2_Electron_1e2p_AC_FD_Dir, hChi2_Proton_1e2p_AC_CD_Dir, hChi2_Proton_1e2p_AC_FD_Dir;

    if (apply_cuts == false) {
//        hChi2_Electron_1e2p_BC_CD = new TH1D("#chi^{2}_{e^{-}} BC (1e2p, CD)", "#chi^{2}_{e^{-}} Histogram Before Cut (1e2p, CD);#chi^{2}_{e^{-}};",
//                                            500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
        hChi2_Electron_1e2p_BC_FD = new TH1D("#chi^{2}_{e^{-}} BC (1e2p, FD)", "#chi^{2}_{e^{-}} Histogram Before Cut (1e2p, FD);#chi^{2}_{e^{-}};",
                                             500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
        hChi2_Electron_1e2p_BC_CD_Dir = Chi2_1e2p_cut_tests_Directory, hChi2_Electron_1e2p_BC_FD_Dir = Chi2_1e2p_cut_tests_Directory;

        hChi2_Proton_1e2p_BC_CD = new TH1D("#chi^{2}_{p} BC (1e2p, CD)", "#chi^{2}_{p} Histogram Before Cut (1e2p, CD);#chi^{2}_{p};",
                                           500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
        hChi2_Proton_1e2p_BC_FD = new TH1D("#chi^{2}_{p} BC (1e2p, FD)", "#chi^{2}_{p} Histogram Before Cut (1e2p, FD);#chi^{2}_{p};",
                                           500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
        hChi2_Proton_1e2p_BC_CD_Dir = Chi2_1e2p_cut_tests_Directory, hChi2_Proton_1e2p_BC_FD_Dir = Chi2_1e2p_cut_tests_Directory;

//        hChi2_Electron_1e2p_AC_CD = new TH1D("#chi^{2}_{e^{-}} AC (1e2p, CD)", "#chi^{2}_{e^{-}} Histogram After Cut (1e2p, CD);#chi^{2}_{e^{-}};",
//                                             500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
        hChi2_Electron_1e2p_AC_FD = new TH1D("#chi^{2}_{e^{-}} AC (1e2p, FD)", "#chi^{2}_{e^{-}} Histogram After Cut (1e2p, FD);#chi^{2}_{e^{-}};",
                                             500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
        hChi2_Electron_1e2p_AC_CD_Dir = Chi2_1e2p_cut_tests_Directory, hChi2_Electron_1e2p_AC_FD_Dir = Chi2_1e2p_cut_tests_Directory;

        hChi2_Proton_1e2p_AC_CD = new TH1D("#chi^{2}_{p} AC (1e2p, CD)", "#chi^{2}_{p} Histogram After Cut (1e2p, CD);#chi^{2}_{p};",
                                           500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
        hChi2_Proton_1e2p_AC_FD = new TH1D("#chi^{2}_{p} AC (1e2p, FD)", "#chi^{2}_{p} Histogram After Cut (1e2p, FD);#chi^{2}_{p};",
                                           500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
        hChi2_Proton_1e2p_AC_CD_Dir = Chi2_1e2p_cut_tests_Directory, hChi2_Proton_1e2p_AC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
    } else {
//        hChi2_Electron_1e2p_BC_CD = new TH1D("#chi^{2}_{e^{-}} (1e2p, CD)", "#chi^{2}_{e^{-}} Histogram (1e2p, CD);#chi^{2}_{e^{-}};",
//                                            500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
        hChi2_Electron_1e2p_BC_FD = new TH1D("#chi^{2}_{e^{-}} (1e2p, FD)", "#chi^{2}_{e^{-}} Histogram (1e2p, FD);#chi^{2}_{e^{-}};",
                                             500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
        hChi2_Proton_1e2p_BC_CD = new TH1D("#chi^{2}_{p} (1e2p, CD)", "#chi^{2}_{p} Histogram (1e2p, CD);#chi^{2}_{p};",
                                           500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
        hChi2_Proton_1e2p_BC_FD = new TH1D("#chi^{2}_{p} (1e2p, FD)", "#chi^{2}_{p} Histogram (1e2p, FD);#chi^{2}_{p};",
                                           500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
        hChi2_Electron_1e2p_BC_CD_Dir = Chi2_1e2p_cut_tests_Directory, hChi2_Electron_1e2p_BC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
        hChi2_Proton_1e2p_BC_CD_Dir = Chi2_1e2p_cut_tests_Directory, hChi2_Proton_1e2p_BC_FD_Dir = Chi2_1e2p_cut_tests_Directory;
    }
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (2p)">
//    TH1D *hChi2_Electron_2p_CD = new TH1D("#chi^{2}_{e^{-}} (2p, CD)", "#chi^{2}_{e^{-}} Histogram (2p, CD);#chi^{2}_{e^{-}};",
//                                         500, -1.5 * Chi2_Electron_cut_CD, 1.5 * Chi2_Electron_cut_CD);
    TH1D *hChi2_Electron_2p_FD = new TH1D("#chi^{2}_{e^{-}} (2p, FD)", "#chi^{2}_{e^{-}} Histogram (2p, FD);#chi^{2}_{e^{-}};",
                                          500, -1.5 * Chi2_Electron_cut_FD, 1.5 * Chi2_Electron_cut_FD);
    string hChi2_Electron_2p_CD_Dir = Chi2_2p_Directory, hChi2_Electron_2p_FD_Dir = Chi2_2p_Directory;

    TH1D *hChi2_Proton_2p_CD = new TH1D("#chi^{2}_{p} (2p, CD)", "#chi^{2}_{p} Histogram (2p, CD);#chi^{2}_{p};",
                                        500, -1.5 * Chi2_Proton_cut_CD, 1.5 * Chi2_Proton_cut_CD);
    TH1D *hChi2_Proton_2p_FD = new TH1D("#chi^{2}_{p} (2p, FD)", "#chi^{2}_{p} Histogram (2p, FD);#chi^{2}_{p};",
                                        500, -1.5 * Chi2_Proton_cut_FD, 1.5 * Chi2_Proton_cut_FD);
    string hChi2_Proton_2p_CD_Dir = Chi2_2p_Directory, hChi2_Proton_2p_FD_Dir = Chi2_2p_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">

    //<editor-fold desc="Vertex plots (no #(e) cut)">
    //TODO: figure out if the code shuold plot these or should I delete them:
//    THStack *Vertex_Electron_Vx_Stack = new THStack("V_{x}^{e^{-}} (no #(e) cut, CD & FD)", "V_{x}^{e^{-}} (no #(e) cut, CD & FD);V_{x}^{e^{-}} [cm];");
//    THStack *Vertex_Electron_Vy_Stack = new THStack("V_{y}^{e^{-}} (no #(e) cut, CD & FD)", "V_{y}^{e^{-}} (no #(e) cut, CD & FD);V_{y}^{e^{-}} [cm];");
//    THStack *Vertex_Electron_Vz_Stack = new THStack("V_{z}^{e^{-}} (no #(e) cut, CD & FD)", "V_{z}^{e^{-}} (no #(e) cut, CD & FD);V_{z}^{e^{-}} [cm];");
//    THStack *Vertex_Proton_Vx_Stack = new THStack("V_{x}^{p} (no #(e) cut, CD & FD)", "V_{x}^{p} (no #(e) cut, CD & FD);V_{x}^{p} [cm];");
//    THStack *Vertex_Proton_Vy_Stack = new THStack("V_{y}^{p} (no #(e) cut, CD & FD)", "V_{y}^{p} (no #(e) cut, CD & FD);V_{y}^{p} [cm];");
//    THStack *Vertex_Proton_Vz_Stack = new THStack("V_{z}^{p} (no #(e) cut, CD & FD)", "V_{z}^{p} (no #(e) cut, CD & FD);V_{z}^{p} [cm];");

//    TH1D *Vertex_Electron_Vx_CD = new TH1D("V_{x}^{e^{-}} (no #(e) cut, CD)", "V_{x}^{e^{-}} (no #(e) cut, CD);V_{x}^{e^{-}} [cm];",
//                                           100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Electron_Vy_CD = new TH1D("V_{y}^{e^{-}} (no #(e) cut, CD)", "V_{y}^{e^{-}} (no #(e) cut, CD);V_{y}^{e^{-}} [cm];",
//                                           100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Electron_Vz_CD = new TH1D("V_{z}^{e^{-}} (no #(e) cut, CD)", "V_{z}^{e^{-}} (no #(e) cut, CD);V_{z}^{e^{-}} [cm];",
//                                           100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Electron_Vx_FD = new TH1D("V_{x}^{e^{-}} (no #(e) cut, FD)", "V_{x}^{e^{-}} (no #(e) cut, CD);V_{x}^{e^{-}} [cm];",
//                                           100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Electron_Vy_FD = new TH1D("V_{y}^{e^{-}} (no #(e) cut, FD)", "V_{y}^{e^{-}} (no #(e) cut, CD);V_{y}^{e^{-}} [cm];",
//                                           100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Electron_Vz_FD = new TH1D("V_{z}^{e^{-}} (no #(e) cut, FD)", "V_{z}^{e^{-}} (no #(e) cut, CD);V_{z}^{e^{-}} [cm];",
//                                           100, -Vertex_boundary, Vertex_boundary);

//    TH1D *Vertex_Proton_Vx_CD = new TH1D("V_{x}^{p} (no #(e) cut, CD)", "V_{x}^{p} (no #(e) cut, CD);V_{x}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Proton_Vy_CD = new TH1D("V_{y}^{p} (no #(e) cut, CD)", "V_{y}^{p} (no #(e) cut, CD);V_{y}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Proton_Vz_CD = new TH1D("V_{z}^{p} (no #(e) cut, CD)", "V_{z}^{p} (no #(e) cut, CD);V_{z}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Proton_Vx_FD = new TH1D("V_{x}^{p} (no #(e) cut, FD)", "V_{x}^{p} (no #(e) cut, CD);V_{x}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Proton_Vy_FD = new TH1D("V_{y}^{p} (no #(e) cut, FD)", "V_{y}^{p} (no #(e) cut, CD);V_{y}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
//    TH1D *Vertex_Proton_Vz_FD = new TH1D("V_{z}^{p} (no #(e) cut, FD)", "V_{z}^{p} (no #(e) cut, CD);V_{z}^{p} [cm];", 100, -Vertex_boundary, Vertex_boundary);
    //</editor-fold>

    //<editor-fold desc="Vertex plots (1e only)">
    THStack *Vertex_Electron_1e_Vx_Stack = new THStack("V_{x}^{e^{-}} (1e Only, CD & FD)", "V_{x}^{e^{-}} (1e Only, CD & FD);V_{x}^{e^{-}} [cm];");
    THStack *Vertex_Electron_1e_Vy_Stack = new THStack("V_{y}^{e^{-}} (1e Only, CD & FD)", "V_{y}^{e^{-}} (1e Only, CD & FD);V_{y}^{e^{-}} [cm];");
    THStack *Vertex_Electron_1e_Vz_Stack = new THStack("V_{z}^{e^{-}} (1e Only, CD & FD)", "V_{z}^{e^{-}} (1e Only, CD & FD);V_{z}^{e^{-}} [cm];");
    THStack *Vertex_Proton_1e_Vx_Stack = new THStack("V_{x}^{p} (1e Only, CD & FD)", "V_{x}^{p} (1e Only, CD & FD);V_{x}^{p} [cm];");
    THStack *Vertex_Proton_1e_Vy_Stack = new THStack("V_{y}^{p} (1e Only, CD & FD)", "V_{y}^{p} (1e Only, CD & FD);V_{y}^{p} [cm];");
    THStack *Vertex_Proton_1e_Vz_Stack = new THStack("V_{z}^{p} (1e Only, CD & FD)", "V_{z}^{p} (1e Only, CD & FD);V_{z}^{p} [cm];");
    THStack *Vertex_Kplus_1e_Vx_Stack = new THStack("V_{x}^{K^{+}} (1e Only, CD & FD)", "V_{x}^{K^{+}} (1e Only, CD & FD);V_{x}^{K^{+}} [cm];");
    THStack *Vertex_Kplus_1e_Vy_Stack = new THStack("V_{y}^{K^{+}} (1e Only, CD & FD)", "V_{y}^{K^{+}} (1e Only, CD & FD);V_{y}^{K^{+}} [cm];");
    THStack *Vertex_Kplus_1e_Vz_Stack = new THStack("V_{z}^{K^{+}} (1e Only, CD & FD)", "V_{z}^{K^{+}} (1e Only, CD & FD);V_{z}^{K^{+}} [cm];");
    THStack *Vertex_Kminus_1e_Vx_Stack = new THStack("V_{x}^{K^{-}} (1e Only, CD & FD)", "V_{x}^{K^{-}} (1e Only, CD & FD);V_{x}^{K^{-}} [cm];");
    THStack *Vertex_Kminus_1e_Vy_Stack = new THStack("V_{y}^{K^{-}} (1e Only, CD & FD)", "V_{y}^{K^{-}} (1e Only, CD & FD);V_{y}^{K^{-}} [cm];");
    THStack *Vertex_Kminus_1e_Vz_Stack = new THStack("V_{z}^{K^{-}} (1e Only, CD & FD)", "V_{z}^{K^{-}} (1e Only, CD & FD);V_{z}^{K^{-}} [cm];");
    THStack *Vertex_piplus_1e_Vx_Stack = new THStack("V_{x}^{#pi^{+}} (1e Only, CD & FD)", "V_{x}^{#pi^{+}} (1e Only, CD & FD);V_{x}^{#pi^{+}} [cm];");
    THStack *Vertex_piplus_1e_Vy_Stack = new THStack("V_{y}^{#pi^{+}} (1e Only, CD & FD)", "V_{y}^{#pi^{+}} (1e Only, CD & FD);V_{y}^{#pi^{+}} [cm];");
    THStack *Vertex_piplus_1e_Vz_Stack = new THStack("V_{z}^{#pi^{+}} (1e Only, CD & FD)", "V_{z}^{#pi^{+}} (1e Only, CD & FD);V_{z}^{#pi^{+}} [cm];");
    THStack *Vertex_piminus_1e_Vx_Stack = new THStack("V_{x}^{#pi^{-}} (1e Only, CD & FD)", "V_{x}^{#pi^{-}} (1e Only, CD & FD);V_{x}^{#pi^{-}} [cm];");
    THStack *Vertex_piminus_1e_Vy_Stack = new THStack("V_{y}^{#pi^{-}} (1e Only, CD & FD)", "V_{y}^{#pi^{-}} (1e Only, CD & FD);V_{y}^{#pi^{-}} [cm];");
    THStack *Vertex_piminus_1e_Vz_Stack = new THStack("V_{z}^{#pi^{-}} (1e Only, CD & FD)", "V_{z}^{#pi^{-}} (1e Only, CD & FD);V_{z}^{#pi^{-}} [cm];");

    TH1D *Vertex_Electron_1e_Vx_CD = new TH1D("V_{x}^{e^{-}} (1e Cut, CD)", "V_{x}^{e^{-}} (1e Cut, CD);V_{x}^{e^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Electron_1e_Vy_CD = new TH1D("V_{y}^{e^{-}} (1e Cut, CD)", "V_{y}^{e^{-}} (1e Cut, CD);V_{y}^{e^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Electron_1e_Vz_CD = new TH1D("V_{z}^{e^{-}} (1e Cut, CD)", "V_{z}^{e^{-}} (1e Cut, CD);V_{z}^{e^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Electron_1e_Vx_FD = new TH1D("V_{x}^{e^{-}} (1e Cut, FD)", "V_{x}^{e^{-}} (1e Cut, FD);V_{x}^{e^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Electron_1e_Vy_FD = new TH1D("V_{y}^{e^{-}} (1e Cut, FD)", "V_{y}^{e^{-}} (1e Cut, FD);V_{y}^{e^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Electron_1e_Vz_FD = new TH1D("V_{z}^{e^{-}} (1e Cut, FD)", "V_{z}^{e^{-}} (1e Cut, FD);V_{z}^{e^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);

    TH1D *Vertex_Proton_1e_Vx_CD = new TH1D("V_{x}^{p} (1e Cut, CD)", "V_{x}^{p} (1e Cut, CD);V_{x}^{p} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Proton_1e_Vy_CD = new TH1D("V_{y}^{p} (1e Cut, CD)", "V_{y}^{p} (1e Cut, CD);V_{y}^{p} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Proton_1e_Vz_CD = new TH1D("V_{z}^{p} (1e Cut, CD)", "V_{z}^{p} (1e Cut, CD);V_{z}^{p} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Proton_1e_Vx_FD = new TH1D("V_{x}^{p} (1e Cut, FD)", "V_{x}^{p} (1e Cut, FD);V_{x}^{p} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Proton_1e_Vy_FD = new TH1D("V_{y}^{p} (1e Cut, FD)", "V_{y}^{p} (1e Cut, FD);V_{y}^{p} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Proton_1e_Vz_FD = new TH1D("V_{z}^{p} (1e Cut, FD)", "V_{z}^{p} (1e Cut, FD);V_{z}^{p} [cm];", 250, -Vertex_boundary, Vertex_boundary);

    TH1D *Vertex_Kplus_1e_Vx_CD = new TH1D("V_{x}^{K^{+}} (1e Cut, CD)", "V_{x}^{K^{+}} (1e Cut, CD);V_{x}^{K^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kplus_1e_Vy_CD = new TH1D("V_{y}^{K^{+}} (1e Cut, CD)", "V_{y}^{K^{+}} (1e Cut, CD);V_{y}^{K^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kplus_1e_Vz_CD = new TH1D("V_{z}^{K^{+}} (1e Cut, CD)", "V_{z}^{K^{+}} (1e Cut, CD);V_{z}^{K^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kplus_1e_Vx_FD = new TH1D("V_{x}^{K^{+}} (1e Cut, FD)", "V_{x}^{K^{+}} (1e Cut, FD);V_{x}^{K^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kplus_1e_Vy_FD = new TH1D("V_{y}^{K^{+}} (1e Cut, FD)", "V_{y}^{K^{+}} (1e Cut, FD);V_{y}^{K^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kplus_1e_Vz_FD = new TH1D("V_{z}^{K^{+}} (1e Cut, FD)", "V_{z}^{K^{+}} (1e Cut, FD);V_{z}^{K^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);

    TH1D *Vertex_Kminus_1e_Vx_CD = new TH1D("V_{x}^{K^{-}} (1e Cut, CD)", "V_{x}^{K^{-}} (1e Cut, CD);V_{x}^{K^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kminus_1e_Vy_CD = new TH1D("V_{y}^{K^{-}} (1e Cut, CD)", "V_{y}^{K^{-}} (1e Cut, CD);V_{y}^{K^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kminus_1e_Vz_CD = new TH1D("V_{z}^{K^{-}} (1e Cut, CD)", "V_{z}^{K^{-}} (1e Cut, CD);V_{z}^{K^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kminus_1e_Vx_FD = new TH1D("V_{x}^{K^{-}} (1e Cut, FD)", "V_{x}^{K^{-}} (1e Cut, FD);V_{x}^{K^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kminus_1e_Vy_FD = new TH1D("V_{y}^{K^{-}} (1e Cut, FD)", "V_{y}^{K^{-}} (1e Cut, FD);V_{y}^{K^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_Kminus_1e_Vz_FD = new TH1D("V_{z}^{K^{-}} (1e Cut, FD)", "V_{z}^{K^{-}} (1e Cut, FD);V_{z}^{K^{-}} [cm];", 250, -Vertex_boundary, Vertex_boundary);

    TH1D *Vertex_piplus_1e_Vx_CD = new TH1D("V_{x}^{#pi^{+}} (1e Cut, CD)", "V_{x}^{#pi^{+}} (1e Cut, CD);V_{x}^{#pi^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piplus_1e_Vy_CD = new TH1D("V_{y}^{#pi^{+}} (1e Cut, CD)", "V_{y}^{#pi^{+}} (1e Cut, CD);V_{y}^{#pi^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piplus_1e_Vz_CD = new TH1D("V_{z}^{#pi^{+}} (1e Cut, CD)", "V_{z}^{#pi^{+}} (1e Cut, CD);V_{z}^{#pi^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piplus_1e_Vx_FD = new TH1D("V_{x}^{#pi^{+}} (1e Cut, FD)", "V_{x}^{#pi^{+}} (1e Cut, FD);V_{x}^{#pi^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piplus_1e_Vy_FD = new TH1D("V_{y}^{#pi^{+}} (1e Cut, FD)", "V_{y}^{#pi^{+}} (1e Cut, FD);V_{y}^{#pi^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piplus_1e_Vz_FD = new TH1D("V_{z}^{#pi^{+}} (1e Cut, FD)", "V_{z}^{#pi^{+}} (1e Cut, FD);V_{z}^{#pi^{+}} [cm];", 250, -Vertex_boundary, Vertex_boundary);

    TH1D *Vertex_piminus_1e_Vx_CD = new TH1D("V_{x}^{#pi^{-}} (1e Cut, CD)", "V_{x}^{#pi^{-}} (1e Cut, CD);V_{x}^{#pi^{-}} [cm];",
                                             250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piminus_1e_Vy_CD = new TH1D("V_{y}^{#pi^{-}} (1e Cut, CD)", "V_{y}^{#pi^{-}} (1e Cut, CD);V_{y}^{#pi^{-}} [cm];",
                                             250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piminus_1e_Vz_CD = new TH1D("V_{z}^{#pi^{-}} (1e Cut, CD)", "V_{z}^{#pi^{-}} (1e Cut, CD);V_{z}^{#pi^{-}} [cm];",
                                             250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piminus_1e_Vx_FD = new TH1D("V_{x}^{#pi^{-}} (1e Cut, FD)", "V_{x}^{#pi^{-}} (1e Cut, FD);V_{x}^{#pi^{-}} [cm];",
                                             250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piminus_1e_Vy_FD = new TH1D("V_{y}^{#pi^{-}} (1e Cut, FD)", "V_{y}^{#pi^{-}} (1e Cut, FD);V_{y}^{#pi^{-}} [cm];",
                                             250, -Vertex_boundary, Vertex_boundary);
    TH1D *Vertex_piminus_1e_Vz_FD = new TH1D("V_{z}^{#pi^{-}} (1e Cut, FD)", "V_{z}^{#pi^{-}} (1e Cut, FD);V_{z}^{#pi^{-}} [cm];",
                                             250, -Vertex_boundary, Vertex_boundary);
    string Vertex_Electron_1e_Vx_CD_Dir = Vertex_Electron_1e_Vx_Directory, Vertex_Electron_1e_Vx_FD_Dir = Vertex_Electron_1e_Vx_Directory;
    string Vertex_Electron_1e_Vy_CD_Dir = Vertex_Electron_1e_Vy_Directory, Vertex_Electron_1e_Vy_FD_Dir = Vertex_Electron_1e_Vy_Directory;
    string Vertex_Electron_1e_Vz_CD_Dir = Vertex_Electron_1e_Vz_Directory, Vertex_Electron_1e_Vz_FD_Dir = Vertex_Electron_1e_Vz_Directory;
    string Vertex_Proton_1e_Vx_CD_Dir = Vertex_Proton_1e_Vx_Directory, Vertex_Proton_1e_Vx_FD_Dir = Vertex_Proton_1e_Vx_Directory;
    string Vertex_Proton_1e_Vy_CD_Dir = Vertex_Proton_1e_Vy_Directory, Vertex_Proton_1e_Vy_FD_Dir = Vertex_Proton_1e_Vy_Directory;
    string Vertex_Proton_1e_Vz_CD_Dir = Vertex_Proton_1e_Vz_Directory, Vertex_Proton_1e_Vz_FD_Dir = Vertex_Proton_1e_Vz_Directory;
    string Vertex_Kplus_1e_Vx_CD_Dir = Vertex_Kplus_1e_Vx_Directory, Vertex_Kplus_1e_Vx_FD_Dir = Vertex_Kplus_1e_Vx_Directory;
    string Vertex_Kplus_1e_Vy_CD_Dir = Vertex_Kplus_1e_Vy_Directory, Vertex_Kplus_1e_Vy_FD_Dir = Vertex_Kplus_1e_Vy_Directory;
    string Vertex_Kplus_1e_Vz_CD_Dir = Vertex_Kplus_1e_Vz_Directory, Vertex_Kplus_1e_Vz_FD_Dir = Vertex_Kplus_1e_Vz_Directory;
    string Vertex_Kminus_1e_Vx_CD_Dir = Vertex_Kminus_1e_Vx_Directory, Vertex_Kminus_1e_Vx_FD_Dir = Vertex_Kminus_1e_Vx_Directory;
    string Vertex_Kminus_1e_Vy_CD_Dir = Vertex_Kminus_1e_Vy_Directory, Vertex_Kminus_1e_Vy_FD_Dir = Vertex_Kminus_1e_Vy_Directory;
    string Vertex_Kminus_1e_Vz_CD_Dir = Vertex_Kminus_1e_Vz_Directory, Vertex_Kminus_1e_Vz_FD_Dir = Vertex_Kminus_1e_Vz_Directory;
    string Vertex_piplus_1e_Vx_CD_Dir = Vertex_piplus_1e_Vx_Directory, Vertex_piplus_1e_Vx_FD_Dir = Vertex_piplus_1e_Vx_Directory;
    string Vertex_piplus_1e_Vy_CD_Dir = Vertex_piplus_1e_Vy_Directory, Vertex_piplus_1e_Vy_FD_Dir = Vertex_piplus_1e_Vy_Directory;
    string Vertex_piplus_1e_Vz_CD_Dir = Vertex_piplus_1e_Vz_Directory, Vertex_piplus_1e_Vz_FD_Dir = Vertex_piplus_1e_Vz_Directory;
    string Vertex_piminus_1e_Vx_CD_Dir = Vertex_piminus_1e_Vx_Directory, Vertex_piminus_1e_Vx_FD_Dir = Vertex_piminus_1e_Vx_Directory;
    string Vertex_piminus_1e_Vy_CD_Dir = Vertex_piminus_1e_Vy_Directory, Vertex_piminus_1e_Vy_FD_Dir = Vertex_piminus_1e_Vy_Directory;
    string Vertex_piminus_1e_Vz_CD_Dir = Vertex_piminus_1e_Vz_Directory, Vertex_piminus_1e_Vz_FD_Dir = Vertex_piminus_1e_Vz_Directory;
    //</editor-fold>

    //<editor-fold desc="dV plots (1e2p & 2p)">
    THStack *dVx_1e2p_before_Stack = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} (1e2p, CD & FD) test;dV_{x} [cm];");
    THStack *dVy_1e2p_before_Stack = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} (1e2p, CD & FD) test;dV_{y} [cm];");
    THStack *dVz_1e2p_before_Stack = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} (1e2p, CD & FD) test;dV_{z} [cm];");

    THStack *dVx_1e2p_after_Stack = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} (1e2p, CD & FD) test;dV_{x} [cm];");
    THStack *dVy_1e2p_after_Stack = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} (1e2p, CD & FD) test;dV_{y} [cm];");
    THStack *dVz_1e2p_after_Stack = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} (1e2p, CD & FD) test;dV_{z} [cm];");

    THStack *dVx_2p_Stack = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} (2p, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} (2p, CD & FD);dV_{x} [cm];");
    THStack *dVy_2p_Stack = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} (2p, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} (2p, CD & FD);dV_{y} [cm];");
    THStack *dVz_2p_Stack = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} (2p, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} (2p, CD & FD);dV_{z} [cm];");

    TH1D *deltaVx_before_dV_cuts_1e2p, *deltaVy_before_dV_cuts_1e2p, *deltaVz_before_dV_cuts_1e2p;
    TH1D *deltaVx_after_dV_cuts_1e2p, *deltaVy_after_dV_cuts_1e2p, *deltaVz_after_dV_cuts_1e2p;
    TH1D *deltaVx_2p, *deltaVy_2p, *deltaVz_2p;

    if (apply_cuts == false) {
        deltaVx_before_dV_cuts_1e2p = new TH1D("dV_{x} BC (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} Before Cuts (1e2p, CD & FD);dV_{x} [cm];",
                                               250, -dV_boundary, dV_boundary);
        deltaVy_before_dV_cuts_1e2p = new TH1D("dV_{y} BC (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} Before Cuts (1e2p, CD & FD);dV_{y} [cm];",
                                               250, -dV_boundary, dV_boundary);
        deltaVz_before_dV_cuts_1e2p = new TH1D("dV_{z} BC (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} Before Cuts (1e2p, CD & FD);dV_{z} [cm];",
                                               250, -dV_boundary, dV_boundary);

        deltaVx_after_dV_cuts_1e2p = new TH1D("dV_{x} AC (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} After Cuts (1e2p, CD & FD);dV_{x} [cm];",
                                              250, -dV_boundary, dV_boundary);
        deltaVy_after_dV_cuts_1e2p = new TH1D("dV_{y} AC (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} After Cuts (1e2p, CD & FD);dV_{y} [cm];",
                                              250, -dV_boundary, dV_boundary);
        deltaVz_after_dV_cuts_1e2p = new TH1D("dV_{z} AC (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} After Cuts (1e2p, CD & FD);dV_{z} [cm];",
                                              250, -dV_boundary, dV_boundary);

        deltaVx_2p = new TH1D("dV_{x} AC (2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} (2p, CD & FD);dV_{x} [cm];", 250, -dV_boundary, dV_boundary);
        deltaVy_2p = new TH1D("dV_{y} AC (2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} (2p, CD & FD);dV_{y} [cm];", 250, -dV_boundary, dV_boundary);
        deltaVz_2p = new TH1D("dV_{z} AC (2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} (2p, CD & FD);dV_{z} [cm];", 250, -dV_boundary, dV_boundary);
    } else {
        deltaVx_before_dV_cuts_1e2p = new TH1D("dV_{x} (1e2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} (1e2p, CD & FD);dV_{x} [cm];", 250, -dV_boundary, dV_boundary);
        deltaVy_before_dV_cuts_1e2p = new TH1D("dV_{y} (1e2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} (1e2p, CD & FD);dV_{y} [cm];", 250, -dV_boundary, dV_boundary);
        deltaVz_before_dV_cuts_1e2p = new TH1D("dV_{z} (1e2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} (1e2p, CD & FD);dV_{z} [cm];", 250, -dV_boundary, dV_boundary);

        deltaVx_2p = new TH1D("dV_{x} (2p, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} (2p, CD & FD);dV_{x} [cm];", 250, -dV_boundary, dV_boundary);
        deltaVy_2p = new TH1D("dV_{y} (2p, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} (2p, CD & FD);dV_{y} [cm];", 250, -dV_boundary, dV_boundary);
        deltaVz_2p = new TH1D("dV_{z} (2p, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} (2p, CD & FD);dV_{z} [cm];", 250, -dV_boundary, dV_boundary);
    }
    //</editor-fold>

    //<editor-fold desc="dV plots (MicroBooNE)">
    THStack *dVx_MicroBooNE_Stack = new THStack("dV_{x}=V^{p}_{x}-dV^{e}_{x} (MicroBooNE, CD & FD)", "dV_{x}=V^{p}_{x}-dV^{e}_{x} (MicroBooNE, CD & FD);dV_{x} [cm];");
    THStack *dVy_MicroBooNE_Stack = new THStack("dV_{y}=V^{p}_{y}-dV^{e}_{y} (MicroBooNE, CD & FD)", "dV_{y}=V^{p}_{y}-dV^{e}_{y} (MicroBooNE, CD & FD);dV_{y} [cm];");
    THStack *dVz_MicroBooNE_Stack = new THStack("dV_{z}=V^{p}_{z}-dV^{e}_{z} (MicroBooNE, CD & FD)", "dV_{z}=V^{p}_{z}-dV^{e}_{z} (MicroBooNE, CD & FD);dV_{z} [cm];");

    TH1D *deltaVx_MicroBooNE_BC = new TH1D("dV_{x} BC (MicroBooNE, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} Before Cuts (MicroBooNE, CD & FD);dV_{x} [cm];",
                                           250, -dV_boundary, dV_boundary);
    TH1D *deltaVy_MicroBooNE_BC = new TH1D("dV_{y} BC (MicroBooNE, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} Before Cuts (MicroBooNE, CD & FD);dV_{y} [cm];",
                                           250, -dV_boundary, dV_boundary);
    TH1D *deltaVz_MicroBooNE_BC = new TH1D("dV_{z} BC (MicroBooNE, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} Before Cuts (MicroBooNE, CD & FD);dV_{z} [cm];",
                                           250, -dV_boundary, dV_boundary);

    TH1D *deltaVx_MicroBooNE_AC = new TH1D("dV_{x} AC (MicroBooNE, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} After Cuts (MicroBooNE, CD & FD);dV_{x} [cm];",
                                           250, -dV_boundary, dV_boundary);
    TH1D *deltaVy_MicroBooNE_AC = new TH1D("dV_{y} AC (MicroBooNE, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} After Cuts (MicroBooNE, CD & FD);dV_{y} [cm];",
                                           250, -dV_boundary, dV_boundary);
    TH1D *deltaVz_MicroBooNE_AC = new TH1D("dV_{z} AC (MicroBooNE, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} After Cuts (MicroBooNE, CD & FD);dV_{z} [cm];",
                                           250, -dV_boundary, dV_boundary);

    TH1D *deltaVx_MicroBooNE = new TH1D("dV_{x} (MicroBooNE, CD & FD)", "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} (MicroBooNE, CD & FD);dV_{x} [cm];",
                                        250, -dV_boundary, dV_boundary);
    TH1D *deltaVy_MicroBooNE = new TH1D("dV_{y} (MicroBooNE, CD & FD)", "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} (MicroBooNE, CD & FD);dV_{y} [cm];",
                                        250, -dV_boundary, dV_boundary);
    TH1D *deltaVz_MicroBooNE = new TH1D("dV_{z} (MicroBooNE, CD & FD)", "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} (MicroBooNE, CD & FD);dV_{z} [cm];",
                                        250, -dV_boundary, dV_boundary);
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
        hSF_1e_cut_BC_FD = new TH1D("SF BC (1e Cut, FD)", "Sampling Fraction f Before Cuts (1e Cut, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 200, 0.1, 0.35);
        hSF_1e_cut_AC_FD = new TH1D("SF AC (1e Cut, FD)", "Sampling Fraction f After Cuts (1e Cut, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 200, 0.1, 0.35);
        hSF_1e_cut_BC_FD_Dir = SF_plots_1e_cut_BC_Directory, hSF_1e_cut_AC_FD_Dir = SF_plots_1e_cut_AC_Directory;
    } else {
        hSF_1e_cut_BC_FD = new TH1D("SF (1e Cut, FD)", "Sampling Fraction f (1e Cut, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 200, 0.1, 0.35);
        hSF_1e_cut_BC_FD_Dir = SF_plots_1e_cut_BC_Directory;
    }
    //</editor-fold>

    //<editor-fold desc="SF vs. P histograms (1e cut, FD only)">
    TH2D *hSF_VS_P_e_1e_cut_BC_FD, *hSF_VS_P_e_1e_cut_AC_FD;
    string hSF_VS_P_e_1e_cut_BC_FD_Dir, hSF_VS_P_e_1e_cut_AC_FD_Dir;

    if (apply_cuts == false) {
        hSF_VS_P_e_1e_cut_BC_FD = new TH2D("SF vs. P_{e} BC (1e Cut, FD)",
                                           "Sampling Fraction f vs. P_{e} Before Cuts (1e Cut, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           250, 0, beamE * 1.1, 250, 0.1, 0.35);
        hSF_VS_P_e_1e_cut_AC_FD = new TH2D("SF vs. P_{e} AC (1e Cut, FD)",
                                           "Sampling Fraction f vs. P_{e} After Cuts (1e Cut, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           250, 0, beamE * 1.1, 250, 0.1, 0.35);
        hSF_VS_P_e_1e_cut_BC_FD_Dir = SF_VS_P_e_plots_1e_cut_BC_Directory, hSF_VS_P_e_1e_cut_AC_FD_Dir = SF_VS_P_e_plots_1e_cut_AC_Directory;
    } else {
        hSF_VS_P_e_1e_cut_BC_FD = new TH2D("SF vs. P_{e} (1e Cut, FD)", "Sampling Fraction f vs. P_{e} (1e Cut, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                           250, 0, beamE * 1.1, 250, 0.1, 0.35);
        hSF_VS_P_e_1e_cut_BC_FD_Dir = SF_VS_P_e_plots_1e_cut_BC_Directory;
    }
    //</editor-fold>

    //<editor-fold desc="SF histograms (2p, FD only)">
    THStack *sSF_2p_FD = new THStack("SF (2p, FD)", "Sampling Fraction (f = #frac{E_{PCAL}+E_{IN}+E_{OUT}}{P}) Histogram (2p, FD);f");

    TH1D *hSF_2p_FD = new TH1D("SF (2p, FD)", "Sampling Fraction f (2p, FD);f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}", 200, 0.1, 0.35);
    TH2D *hSF_VS_P_e_2p_FD = new TH2D("SF vs. P_{e} (2p, FD)", "Sampling Fraction f vs. P_{e} (2p, FD);P_{e} [GeV];f = (E_{PCAL} + E_{IN} + E_{OUT})/P_{e}",
                                      250, 0, beamE * 1.1, 250, 0.1, 0.35);
    string hSF_2p_FD_Dir = SF_plots_2p_cuts_Directory, hSF_VS_P_e_2p_FD_Dir = SF_VS_P_e_plots_2p_cuts_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Electron fiducial histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Electron fiducial histograms (FD only)">
    //TODO: make sure clas12ana applies edge cuts on PCAL
    TH2D *Vcal_VS_EoP_1e_BC_PCAL, *Vcal_VS_EoP_1e_AC_PCAL, *Wcal_VS_EoP_1e_BC_PCAL, *Wcal_VS_EoP_1e_AC_PCAL;
    string Vcal_VS_EoP_1e_BC_PCAL_Dir, Vcal_VS_EoP_1e_AC_PCAL_Dir, Wcal_VS_EoP_1e_BC_PCAL_Dir, Wcal_VS_EoP_1e_AC_PCAL_Dir;

    if (apply_cuts == false) {
        Vcal_VS_EoP_1e_BC_PCAL = new TH2D("Vcal vs. SF BC (1e Cut, PCAL)",
                                          "ECAL V coordinate vs. SF Before Cuts (1e Cut, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, 0.05, 0.35);
        Vcal_VS_EoP_1e_AC_PCAL = new TH2D("Vcal vs. SF AC (1e Cut, PCAL)",
                                          "ECAL V coordinate vs. SF After Cuts (1e Cut, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, 0.05, 0.35);
        Vcal_VS_EoP_1e_BC_PCAL_Dir = fiducial_plots_1e_BC_PCAL_Directory, Vcal_VS_EoP_1e_AC_PCAL_Dir = fiducial_plots_1e_AC_PCAL_Directory;

        Wcal_VS_EoP_1e_BC_PCAL = new TH2D("Wcal vs. SF BC (1e Cut, PCAL)",
                                          "ECAL W coordinate vs. SF Before Cuts (1e Cut, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, 0.05, 0.35);
        Wcal_VS_EoP_1e_AC_PCAL = new TH2D("Wcal vs. SF AC (1e Cut, PCAL)",
                                          "ECAL W coordinate vs. SF After Cuts (1e Cut, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, 0.05, 0.35);
        Wcal_VS_EoP_1e_BC_PCAL_Dir = fiducial_plots_1e_BC_PCAL_Directory, Wcal_VS_EoP_1e_AC_PCAL_Dir = fiducial_plots_1e_AC_PCAL_Directory;
    } else {
        Vcal_VS_EoP_1e_BC_PCAL = new TH2D("Vcal vs. SF BC (1e Cut, PCAL)", "ECAL V coordinate vs. SF (1e Cut, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, 0.05, 0.35);
        Vcal_VS_EoP_1e_BC_PCAL_Dir = fiducial_plots_1e_BC_PCAL_Directory;

        Wcal_VS_EoP_1e_BC_PCAL = new TH2D("Wcal vs. SF BC (1e Cut, PCAL)", "ECAL W coordinate vs. SF (1e Cut, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                          250, 0, 50, 250, 0.05, 0.35);
        Wcal_VS_EoP_1e_BC_PCAL_Dir = fiducial_plots_1e_BC_PCAL_Directory;
    }

    TH2D *Vcal_VS_EoP_2p_PCAL = new TH2D("Vcal vs. SF (2p, PCAL)", "ECAL V coordinate vs. SF (2p, PCAL);ECAL V coordinate [cm];Sampling Fraction (SF)",
                                         250, 0, 50, 250, 0.05, 0.35);
    TH2D *Wcal_VS_EoP_2p_PCAL = new TH2D("Wcal vs. SF (2p, PCAL)", "ECAL W coordinate vs. SF (2p, PCAL);ECAL W coordinate [cm];Sampling Fraction (SF)",
                                         250, 0, 50, 250, 0.05, 0.35);
    string Vcal_VS_EoP_2p_PCAL_Dir = fiducial_plots_2p_cuts_histograms_PCAL_Directory, Wcal_VS_EoP_2p_PCAL_Dir = fiducial_plots_2p_cuts_histograms_PCAL_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum histograms">
    /* Here are the definitions for the momentum plots. These are used to check that momentum cuts were applied in MicroBooNE, 1e2p and 2p plots. */

//    //<editor-fold desc="Momentum MicroBooNE plots">
//    THStack *P_Stack_MicroBooNE_BC_CD = new THStack("Particle Momentum BC (MicroBooNE, CD)", "Particle Momentum Histogram BC (MicroBooNE, CD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_AC_CD = new THStack("Particle Momentum AC (MicroBooNE, CD)", "Particle Momentum Histogram AC (MicroBooNE, CD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_BC_FD = new THStack("Particle Momentum BC (MicroBooNE, FD)", "Particle Momentum Histogram BC (MicroBooNE, FD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_AC_FD = new THStack("Particle Momentum AC (MicroBooNE, FD)", "Particle Momentum Histogram AC (MicroBooNE, FD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_BC = new THStack("Particle Momentum BC (MicroBooNE, CD & FD)", "Particle Momentum Histogram BC (MicroBooNE, CD & FD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_AC = new THStack("Particle Momentum AC (MicroBooNE, CD & FD)", "Particle Momentum Histogram AC (MicroBooNE, CD & FD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_CD = new THStack("Particle Momentum (MicroBooNE, CD)", "Particle Momentum Histogram (MicroBooNE, CD);P [GeV];");
//    THStack *P_Stack_MicroBooNE_FD = new THStack("Particle Momentum (MicroBooNE, FD)", "Particle Momentum Histogram (MicroBooNE, FD);P [GeV];");
//
//    //<editor-fold desc="Momentum histograms before MicroBooNE cuts test">
//    TH1D *P_e_MicroBooNE_BC_CD = new TH1D("P_{e} BC (MicroBooNE, CD)", "P_{e} Before Cut (MicroBooNE, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_e_MicroBooNE_BC_FD = new TH1D("P_{e} BC (MicroBooNE, FD)", "P_{e} Before Cut (MicroBooNE, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
//    string P_e_MicroBooNE_BC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_e_MicroBooNE_BC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
//
//    TH1D *P_p_MicroBooNE_BC_CD = new TH1D("P_{p} BC (MicroBooNE, CD)", "P_{p} Before Cut (MicroBooNE, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_p_MicroBooNE_BC_FD = new TH1D("P_{p} BC (MicroBooNE, FD)", "P_{p} Before Cut (MicroBooNE, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
//    string P_p_MicroBooNE_BC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_p_MicroBooNE_BC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
//
//    TH1D *P_cpion_MicroBooNE_BC_CD = new TH1D("P_{#pi^{#pm}} BC (MicroBooNE, CD)", "P_{#pi^{#pm}} Before Cut (MicroBooNE, CD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_cpion_MicroBooNE_BC_FD = new TH1D("P_{#pi^{#pm}} BC (MicroBooNE, FD)", "P_{#pi^{#pm}} Before Cut (MicroBooNE, FD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
//    string P_cpion_MicroBooNE_BC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_cpion_MicroBooNE_BC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
//    //</editor-fold>
//
//    //<editor-fold desc="Momentum histograms after MicroBooNE cuts test">
//    TH1D *P_e_MicroBooNE_AC_CD = new TH1D("P_{e} AC (MicroBooNE, CD)", "P_{e} After Cut (MicroBooNE, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_e_MicroBooNE_AC_FD = new TH1D("P_{e} AC (MicroBooNE, FD)", "P_{e} After Cut (MicroBooNE, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
//    string P_e_MicroBooNE_AC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_e_MicroBooNE_AC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
//
//    TH1D *P_p_MicroBooNE_AC_CD = new TH1D("P_{p} AC (MicroBooNE, CD)", "P_{p} After Cut (MicroBooNE, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_p_MicroBooNE_AC_FD = new TH1D("P_{p} AC (MicroBooNE, FD)", "P_{p} After Cut (MicroBooNE, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
//    string P_p_MicroBooNE_AC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_p_MicroBooNE_AC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
//
//    TH1D *P_cpion_MicroBooNE_AC_CD = new TH1D("P_{#pi^{#pm}} AC (MicroBooNE, CD)", "P_{#pi^{#pm}} After Cut (MicroBooNE, CD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_cpion_MicroBooNE_AC_FD = new TH1D("P_{#pi^{#pm}} AC (MicroBooNE, FD)", "P_{#pi^{#pm}} After Cut (MicroBooNE, FD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
//    string P_cpion_MicroBooNE_AC_CD_Dir = Momentum_MicroBooNE_cut_tests_Directory, P_cpion_MicroBooNE_AC_FD_Dir = Momentum_MicroBooNE_cut_tests_Directory;
//    //</editor-fold>
//
//    //<editor-fold desc="Momentum histograms after MicroBooNE cuts">
//    TH1D *P_e_MicroBooNE_CD = new TH1D("P_{e} (MicroBooNE, CD)", "P_{e} (MicroBooNE, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_e_MicroBooNE_FD = new TH1D("P_{e} (MicroBooNE, FD)", "P_{e} (MicroBooNE, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
//    string P_e_MicroBooNE_CD_Dir = Momentum_MicroBooNE_Directory, P_e_MicroBooNE_FD_Dir = Momentum_MicroBooNE_Directory;
//
//    TH1D *P_p_MicroBooNE_CD = new TH1D("P_{p} (MicroBooNE, CD)", "P_{p} (MicroBooNE, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_p_MicroBooNE_FD = new TH1D("P_{p} (MicroBooNE, FD)", "P_{p} (MicroBooNE, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
//    string P_p_MicroBooNE_CD_Dir = Momentum_MicroBooNE_Directory, P_p_MicroBooNE_FD_Dir = Momentum_MicroBooNE_Directory;
//
//    TH1D *P_cpion_MicroBooNE_CD = new TH1D("P_{#pi^{#pm}} (MicroBooNE, CD)", "P_{#pi^{#pm}} (MicroBooNE, CD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
//    TH1D *P_cpion_MicroBooNE_FD = new TH1D("P_{#pi^{#pm}} (MicroBooNE, FD)", "P_{#pi^{#pm}} (MicroBooNE, FD);P_{#pi^{#pm}} [GeV];", 250, 0, 1.1 * beamE);
//    string P_cpion_MicroBooNE_CD_Dir = Momentum_MicroBooNE_Directory, P_cpion_MicroBooNE_FD_Dir = Momentum_MicroBooNE_Directory;
//    //</editor-fold>
//
//    //</editor-fold>

    //<editor-fold desc="Momentum 1e2p & 2p plots">
    THStack *sP_1e2p_BC_CD = new THStack("Particle Momentum BC (1e2p, CD)", "Particle Momentum Histogram BC (1e2p, CD);P [GeV];");
    THStack *sP_1e2p_AC_CD = new THStack("Particle Momentum AC (1e2p, CD)", "Particle Momentum Histogram AC (1e2p, CD);P [GeV];");
    THStack *sP_1e2p_BC_FD = new THStack("Particle Momentum BC (1e2p, FD)", "Particle Momentum Histogram BC (1e2p, FD);P [GeV];");
    THStack *sP_1e2p_AC_FD = new THStack("Particle Momentum AC (1e2p, FD)", "Particle Momentum Histogram AC (1e2p, FD);P [GeV];");
    THStack *sP_1e2p_BC = new THStack("Particle Momentum BC (1e2p, CD & FD)", "Particle Momentum Histogram BC (1e2p, CD & FD);P [GeV];");
    THStack *sP_1e2p_AC = new THStack("Particle Momentum AC (1e2p, CD & FD)", "Particle Momentum Histogram AC (1e2p, CD & FD);P [GeV];");
    THStack *sP_2p_CD = new THStack("Particle Momentum (2p, CD)", "Particle Momentum Histogram (2p, CD);P [GeV];");
    THStack *sP_2p_FD = new THStack("Particle Momentum (2p, FD)", "Particle Momentum Histogram (2p, FD);P [GeV];");

    //<editor-fold desc="Momentum histograms before 1e2p cuts">
//    TH1D *hP_e_1e2p_BC_CD = new TH1D("P_{e} BC (1e2p, CD)", "P_{e} Before Cut (1e2p, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *hP_e_1e2p_BC_FD = new TH1D("P_{e} BC (1e2p, FD)", "P_{e} Before Cut (1e2p, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string hP_e_1e2p_BC_CD_Dir = Momentum_1e2p_Directory, hP_e_1e2p_BC_FD_Dir = Momentum_1e2p_Directory;

    TH1D *hP_p_1e2p_BC_CD = new TH1D("P_{p} BC (1e2p, CD)", "P_{p} Before Cut (1e2p, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *hP_p_1e2p_BC_FD = new TH1D("P_{p} BC (1e2p, FD)", "P_{p} Before Cut (1e2p, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string hP_p_1e2p_BC_CD_Dir = Momentum_1e2p_Directory, hP_p_1e2p_BC_FD_Dir = Momentum_1e2p_Directory;
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after 1e2p cuts">
//    TH1D *hP_e_1e2p_AC_CD = new TH1D("P_{e} AC (1e2p, CD)", "P_{e} After Cut (1e2p, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *hP_e_1e2p_AC_FD = new TH1D("P_{e} AC (1e2p, FD)", "P_{e} After Cut (1e2p, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string hP_e_1e2p_AC_CD_Dir = Momentum_1e2p_Directory, hP_e_1e2p_AC_FD_Dir = Momentum_1e2p_Directory;

    TH1D *hP_p_1e2p_AC_CD = new TH1D("P_{p} AC (1e2p, CD)", "P_{p} After Cut (1e2p, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *hP_p_1e2p_AC_FD = new TH1D("P_{p} AC (1e2p, FD)", "P_{p} After Cut (1e2p, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string hP_p_1e2p_AC_CD_Dir = Momentum_1e2p_Directory, hP_p_1e2p_AC_FD_Dir = Momentum_1e2p_Directory;
    //</editor-fold>

    //<editor-fold desc="Momentum histograms after 2p cuts">
//    TH1D *hP_e_2p_CD = new TH1D("P_{e} (2p, CD)", "P_{e} (2p, CD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *hP_e_2p_FD = new TH1D("P_{e} (2p, FD)", "P_{e} (2p, FD);P_{e} [GeV];", 250, 0, 1.1 * beamE);
    string hP_e_2p_CD_Dir = Momentum_2p_Directory, hP_e_2p_FD_Dir = Momentum_2p_Directory;

    TH1D *hP_p_2p_CD = new TH1D("P_{p} (2p, CD)", "P_{p} (2p, CD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    TH1D *hP_p_2p_FD = new TH1D("P_{p} (2p, FD)", "P_{p} (2p, FD);P_{p} [GeV];", 250, 0, 1.1 * beamE);
    string hP_p_2p_CD_Dir = Momentum_2p_Directory, hP_p_2p_FD_Dir = Momentum_2p_Directory;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

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
    TH2D *Beta_vs_P_1e_cut_CD = new TH2D("#beta vs. P (All Particles, 1e only, CD)", "#beta vs. P (All Particles, 1e Cut, CD);P [GeV];#beta",
                                         250, 0, beamE * 1.1, 250, 0, 1.7);
    TH2D *Beta_vs_P_1e_cut_FD = new TH2D("#beta vs. P (All Particles, 1e only, FD)", "#beta vs. P (All Particles, 1e Cut, FD);P [GeV];#beta",
                                         250, 0, beamE * 1.1, 250, 0, 1.7);

    TH2D *Beta_vs_P_1e_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, 1e only, CD)", "#beta vs. P (Electrons Only, 1e Cut, CD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 1.7);
    TH2D *Beta_vs_P_1e_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, 1e only, FD)", "#beta vs. P (Electrons Only, 1e Cut, FD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 1.7);

    TH2D *Beta_vs_P_1e_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, 1e only, CD)", "#beta vs. P (Protons Only, 1e Cut, CD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, 1e only, FD)", "#beta vs. P (Protons Only, 1e Cut, FD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);

    TH2D *Beta_vs_P_1e_Neutrons_Only_CD = new TH2D("#beta vs. P (Neutrons Only, 1e only, CD)", "#beta vs. P (Neutrons Only, 1e Cut, CD);P [GeV];#beta",
                                                   250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Neutrons_Only_FD = new TH2D("#beta vs. P (Neutrons Only, 1e only, FD)", "#beta vs. P (Neutrons Only, 1e Cut, FD);P [GeV];#beta",
                                                   250, 0, P_boundary, 250, 0, Beta_boundary);

    TH2D *Beta_vs_P_1e_piplus_Only_CD = new TH2D("#beta vs. P (#pi^{+} Only, 1e only, CD)", "#beta vs. P (#pi^{+} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, 1.7);
    TH2D *Beta_vs_P_1e_piplus_Only_FD = new TH2D("#beta vs. P (#pi^{+} Only, 1e only, FD)", "#beta vs. P (#pi^{+} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, 1.7);

    TH2D *Beta_vs_P_1e_pizero_Only_CD = new TH2D("#beta vs. P (#pi^{0} Only, 1e only, CD)", "#beta vs. P (#pi^{0} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_pizero_Only_FD = new TH2D("#beta vs. P (#pi^{0} Only, 1e only, FD)", "#beta vs. P (#pi^{0} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);

    TH2D *Beta_vs_P_1e_piminus_Only_CD = new TH2D("#beta vs. P (#pi^{-} Only, 1e only, CD)", "#beta vs. P (#pi^{-} Only, 1e Cut, CD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, 1.7);
    TH2D *Beta_vs_P_1e_piminus_Only_FD = new TH2D("#beta vs. P (#pi^{-} Only, 1e only, FD)", "#beta vs. P (#pi^{-} Only, 1e Cut, FD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, 1.7);

    TH2D *Beta_vs_P_1e_Kplus_Only_CD = new TH2D("#beta vs. P (K^{+} Only, 1e only, CD)", "#beta vs. P (K^{+} Only, 1e Cut, CD);P [GeV];#beta",
                                                250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Kplus_Only_FD = new TH2D("#beta vs. P (K^{+} Only, 1e only, FD)", "#beta vs. P (K^{+} Only, 1e Cut, FD);P [GeV];#beta",
                                                250, 0, P_boundary, 250, 0, Beta_boundary);

    TH2D *Beta_vs_P_1e_Kminus_Only_CD = new TH2D("#beta vs. P (K^{-} Only, 1e only, CD)", "#beta vs. P (K^{-} Only, 1e Cut, CD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_1e_Kminus_Only_FD = new TH2D("#beta vs. P (K^{-} Only, 1e only, FD)", "#beta vs. P (K^{-} Only, 1e Cut, FD);P [GeV];#beta",
                                                 250, 0, P_boundary, 250, 0, Beta_boundary);
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
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (2p)">

    //<editor-fold desc="Beta vs. P for all particles (2p)">
    TH2D *Beta_vs_P_2p_CD = new TH2D("#beta vs. P (All Particles, 2p, CD)", "#beta vs. P (All Particles, 2p, CD);P [GeV];#beta",
                                     250, 0, beamE * 1.1, 250, 0, 1.5);
    TH2D *Beta_vs_P_2p_FD = new TH2D("#beta vs. P (All Particles, 2p, FD)", "#beta vs. P (All Particles, 2p, FD);P [GeV];#beta",
                                     250, 0, beamE * 1.1, 250, 0, 1.5);
    string Beta_vs_P_2p_CD_Dir = Beta_VS_P_2p_Directory, Beta_vs_P_2p_FD_Dir = Beta_VS_P_2p_Directory;

    TH2D *Beta_vs_P_2p_Electrons_Only_CD = new TH2D("#beta vs. P (Electrons Only, 2p, CD)", "#beta vs. P (Electrons Only, 2p, CD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);
    TH2D *Beta_vs_P_2p_Electrons_Only_FD = new TH2D("#beta vs. P (Electrons Only, 2p, FD)", "#beta vs. P (Electrons Only, 2p, FD);P [GeV];#beta",
                                                    250, 0, beamE * 1.425, 250, 0.5, 3);
    string Beta_vs_P_2p_Electrons_Only_CD_Dir = Beta_VS_P_2p_Directory, Beta_vs_P_2p_Electrons_Only_FD_Dir = Beta_VS_P_2p_Directory;

    TH2D *Beta_vs_P_2p_Protons_Only_CD = new TH2D("#beta vs. P (Protons Only, 2p, CD)", "#beta vs. P (Protons Only, 2p, CD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    TH2D *Beta_vs_P_2p_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, 2p, FD)", "#beta vs. P (Protons Only, 2p, FD);P [GeV];#beta",
                                                  250, 0, P_boundary, 250, 0, Beta_boundary);
    string Beta_vs_P_2p_Protons_Only_CD_Dir = Beta_VS_P_2p_Directory, Beta_vs_P_2p_Protons_Only_FD_Dir = Beta_VS_P_2p_Directory;
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
//                                                             "#beta vs. P (Protons Only, MicroBooNE-BC, CD);P [GeV];#beta", 250, 0, P_boundary, 250, 0, Beta_boundary);
//    TH2D *Beta_vs_P_MicroBooNE_BC_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, MicroBooNE-BC, FD)",
//                                                             "#beta vs. P (Protons Only, MicroBooNE-BC, FD);P [GeV];#beta", 250, 0, P_boundary, 250, 0, Beta_boundary);
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
//                                                             "#beta vs. P (Protons Only, MicroBooNE-AC, CD);P [GeV];#beta", 250, 0, P_boundary, 250, 0, Beta_boundary);
//    TH2D *Beta_vs_P_MicroBooNE_AC_Protons_Only_FD = new TH2D("#beta vs. P (Protons Only, MicroBooNE-AC, FD)",
//                                                             "#beta vs. P (Protons Only, MicroBooNE-AC, FD);P [GeV];#beta", 250, 0, P_boundary, 250, 0, Beta_boundary);
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
// Angle histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Angle histograms">

    // Theta_e --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e histograms">
    THStack *sTheta_e = new THStack("#theta_{e} (CD & FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, CD & FD);#theta_{e} [Deg];");
//    THStack *sTheta_e_1e2p_CD = new THStack("#theta_{e} (1e2p, CD)", "#theta_{e} of Outgoing Electron (1e2p, CD);#theta_{e} [Deg];");
    THStack *sTheta_e_1e2p_FD = new THStack("#theta_{e} (1e2p, FD)", "#theta_{e} of Outgoing Electron (1e2p, FD);#theta_{e} [Deg];");
//    THStack *sTheta_e_2p_CD = new THStack("#theta_{e} (2p, CD)", "#theta_{e} of Outgoing Electron (2p, CD);#theta_{e} [Deg];");
    THStack *sTheta_e_2p_FD = new THStack("#theta_{e} (2p, FD)", "#theta_{e} of Outgoing Electron (2p, FD);#theta_{e} [Deg];");

    /* Theta_e histograms (no #(e) cut) */
//    TH1D *hTheta_e_All_e_CD = new TH1D("#theta_{e} (no #(e) cut, CD)", "#theta_{e} of Outgoing Electron (no #(e) cut, CD);#theta_{e} [Deg];", 100, 35, 140);
    TH1D *hTheta_e_All_e_FD = new TH1D("#theta_{e} (no #(e) cut, FD)", "#theta_{e} of Outgoing Electron (no #(e) cut, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_e_CD_Dir = Theta_e_All_e_Directory, hTheta_e_All_e_FD_Dir = Theta_e_All_e_Directory;

    /* Theta_e histograms (1e cut) */
//    TH1D *hTheta_e_1e_cut_CD = new TH1D("#theta_{e} (1e Cut, CD)", "#theta_{e} of Outgoing Electron (1e Cut, CD);#theta_{e} [Deg];", 100, 35, 140);
    TH1D *hTheta_e_1e_cut_FD = new TH1D("#theta_{e} (1e Cut, FD)", "#theta_{e} of Outgoing Electron (1e Cut, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_1e_cut_CD_Dir = Theta_e_1e_cut_Directory, hTheta_e_1e_cut_FD_Dir = Theta_e_1e_cut_Directory;

    /* Theta_e histograms (1e2X) */
//    TH1D *hTheta_e_All_Int_1e2X_CD = new TH1D("#theta_{e} (All Int., 1e2X, CD)", "#theta_{e} of Outgoing Electron (All Int., 1e2X, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_QEL_1e2X_CD = new TH1D("#theta_{e} (QEL Only, 1e2X, CD)", "#theta_{e} of Outgoing Electron (QEL Only, 1e2X, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_MEC_1e2X_CD = new TH1D("#theta_{e} (MEC Only, 1e2X, CD)", "#theta_{e} of Outgoing Electron (MEC Only, 1e2X, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_RES_1e2X_CD = new TH1D("#theta_{e} (RES Only, 1e2X, CD)", "#theta_{e} of Outgoing Electron (RES Only, 1e2X, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_DIS_1e2X_CD = new TH1D("#theta_{e} (DIS Only, 1e2X, CD)", "#theta_{e} of Outgoing Electron (DIS Only, 1e2X, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_All_Int_1e2X_FD = new TH1D("#theta_{e} (All Int., 1e2X, FD)", "#theta_{e} of Outgoing Electron (All Int., 1e2X, FD);#theta_{e} [Deg];", 100, 0, 50);
//    TH1D *hTheta_e_QEL_1e2X_FD = new TH1D("#theta_{e} (QEL Only, 1e2X, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1e2X, FD);#theta_{e} [Deg];", 100, 0, 50);
//    TH1D *hTheta_e_MEC_1e2X_FD = new TH1D("#theta_{e} (MEC Only, 1e2X, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1e2X, FD);#theta_{e} [Deg];", 100, 0, 50);
//    TH1D *hTheta_e_RES_1e2X_FD = new TH1D("#theta_{e} (RES Only, 1e2X, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1e2X, FD);#theta_{e} [Deg];", 100, 0, 50);
//    TH1D *hTheta_e_DIS_1e2X_FD = new TH1D("#theta_{e} (DIS Only, 1e2X, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1e2X, FD);#theta_{e} [Deg];", 100, 0, 50);
//    string hTheta_e_All_Int_1e2X_CD_Dir = Theta_e_1e2X_Directory, hTheta_e_All_Int_1e2X_FD_Dir = Theta_e_1e2X_Directory;
//    string hTheta_e_QEL_1e2X_CD_Dir = Theta_e_1e2X_Directory, hTheta_e_QEL_1e2X_FD_Dir = Theta_e_1e2X_Directory;
//    string hTheta_e_MEC_1e2X_CD_Dir = Theta_e_1e2X_Directory, hTheta_e_MEC_1e2X_FD_Dir = Theta_e_1e2X_Directory;
//    string hTheta_e_RES_1e2X_CD_Dir = Theta_e_1e2X_Directory, hTheta_e_RES_1e2X_FD_Dir = Theta_e_1e2X_Directory;
//    string hTheta_e_DIS_1e2X_CD_Dir = Theta_e_1e2X_Directory, hTheta_e_DIS_1e2X_FD_Dir = Theta_e_1e2X_Directory;

    /* Theta_e histograms (1e2p) */
//    TH1D *hTheta_e_All_Int_1e2p_CD = new TH1D("#theta_{e} of Outgoing Electron (All Int., 1e2p, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_QEL_1e2p_CD = new TH1D("#theta_{e} for 1e2p (QEL Only, 1e2p, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_MEC_1e2p_CD = new TH1D("#theta_{e} for 1e2p (MEC Only, 1e2p, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_RES_1e2p_CD = new TH1D("#theta_{e} for 1e2p (RES Only, 1e2p, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_DIS_1e2p_CD = new TH1D("#theta_{e} for 1e2p (DIS Only, 1e2p, CD)", ";#theta_{e} [Deg];", 100, 35, 140);
    TH1D *hTheta_e_All_Int_1e2p_FD = new TH1D("#theta_{e} (All Int., 1e2p, FD)", "#theta_{e} of Outgoing Electron (All Int., 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_QEL_1e2p_FD = new TH1D("#theta_{e} (QEL Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_MEC_1e2p_FD = new TH1D("#theta_{e} (MEC Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_RES_1e2p_FD = new TH1D("#theta_{e} (RES Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_DIS_1e2p_FD = new TH1D("#theta_{e} (DIS Only, 1e2p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 1e2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_Int_1e2p_CD_Dir = Theta_e_1e2p_Directory, hTheta_e_All_Int_1e2p_FD_Dir = Theta_e_1e2p_Directory;
    string hTheta_e_QEL_1e2p_CD_Dir = Theta_e_1e2p_Directory, hTheta_e_QEL_1e2p_FD_Dir = Theta_e_1e2p_Directory;
    string hTheta_e_MEC_1e2p_CD_Dir = Theta_e_1e2p_Directory, hTheta_e_MEC_1e2p_FD_Dir = Theta_e_1e2p_Directory;
    string hTheta_e_RES_1e2p_CD_Dir = Theta_e_1e2p_Directory, hTheta_e_RES_1e2p_FD_Dir = Theta_e_1e2p_Directory;
    string hTheta_e_DIS_1e2p_CD_Dir = Theta_e_1e2p_Directory, hTheta_e_DIS_1e2p_FD_Dir = Theta_e_1e2p_Directory;

    /* Theta_e histograms (2p) */
//    TH1D *hTheta_e_All_Int_2p_CD = new TH1D("#theta_{e} (All Int., 2p, CD)", "#theta_{e} of Outgoing Electron (All Int., 2p, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_QEL_2p_CD = new TH1D("#theta_{e} (QEL Only, 2p, CD)", "#theta_{e} of Outgoing Electron (QEL Only, 2p, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_MEC_2p_CD = new TH1D("#theta_{e} (MEC Only, 2p, CD)", "#theta_{e} of Outgoing Electron (MEC Only, 2p, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_RES_2p_CD = new TH1D("#theta_{e} (RES Only, 2p, CD)", "#theta_{e} of Outgoing Electron (RES Only, 2p, CD);#theta_{e} [Deg];", 100, 35, 140);
//    TH1D *hTheta_e_DIS_2p_CD = new TH1D("#theta_{e} (DIS Only, 2p, CD)", "#theta_{e} of Outgoing Electron (DIS Only, 2p, CD);#theta_{e} [Deg];", 100, 35, 140);
    TH1D *hTheta_e_All_Int_2p_FD = new TH1D("#theta_{e} (All Int., 2p, FD)", "#theta_{e} of Outgoing Electron (All Int., 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_QEL_2p_FD = new TH1D("#theta_{e} (QEL Only, 2p, FD)", "#theta_{e} of Outgoing Electron (QEL Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_MEC_2p_FD = new TH1D("#theta_{e} (MEC Only, 2p, FD)", "#theta_{e} of Outgoing Electron (MEC Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_RES_2p_FD = new TH1D("#theta_{e} (RES Only, 2p, FD)", "#theta_{e} of Outgoing Electron (RES Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    TH1D *hTheta_e_DIS_2p_FD = new TH1D("#theta_{e} (DIS Only, 2p, FD)", "#theta_{e} of Outgoing Electron (DIS Only, 2p, FD);#theta_{e} [Deg];", 100, 0, 50);
    string hTheta_e_All_Int_2p_CD_Dir = Theta_e_2p_Directory, hTheta_e_All_Int_2p_FD_Dir = Theta_e_2p_Directory;
    string hTheta_e_QEL_2p_CD_Dir = Theta_e_2p_Directory, hTheta_e_QEL_2p_FD_Dir = Theta_e_2p_Directory;
    string hTheta_e_MEC_2p_CD_Dir = Theta_e_2p_Directory, hTheta_e_MEC_2p_FD_Dir = Theta_e_2p_Directory;
    string hTheta_e_RES_2p_CD_Dir = Theta_e_2p_Directory, hTheta_e_RES_2p_FD_Dir = Theta_e_2p_Directory;
    string hTheta_e_DIS_2p_CD_Dir = Theta_e_2p_Directory, hTheta_e_DIS_2p_FD_Dir = Theta_e_2p_Directory;
    //</editor-fold>

// Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_e histograms">
    THStack *sPhi_e = new THStack("#phi_{e} stack (CD & FD)", "#phi_{e} of Outgoing Electron (no #(e) cut, CD & FD);#phi_{e} [Deg];");

    /* Phi_e histograms (no #(e) cut) */
//    TH1D *hPhi_e_All_e_CD = new TH1D("#phi_{e} (no #(e) cut, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_All_e_FD = new TH1D("#phi_{e} (no #(e) cut, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string hPhi_e_All_e_CD_Dir = Phi_e_All_e_Directory, hPhi_e_All_e_FD_Dir = Phi_e_All_e_Directory;

    /* Phi_e histograms (1e cut) */
//    TH1D *hPhi_e_1e_cut_CD = new TH1D("#phi_{e} (1e Cut, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_1e_cut_FD = new TH1D("#phi_{e} (1e Cut, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string hPhi_e_1e_cut_CD_Dir = Phi_e_1e_cut_Directory, hPhi_e_1e_cut_FD_Dir = Phi_e_1e_cut_Directory;

    /* Phi_e histograms (1e2X) */
//    TH1D *hPhi_e_1e2X_CD = new TH1D("#phi_{e} 1e2X (All Int., CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_QEL_CD = new TH1D("#phi_{e} for 1e2X (QEL Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_MEC_CD = new TH1D("#phi_{e} for 1e2X (MEC Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_RES_CD = new TH1D("#phi_{e} for 1e2X (RES Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_DIS_CD = new TH1D("#phi_{e} for 1e2X (DIS Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_FD = new TH1D("#phi_{e} 1e2X (All Int., FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_QEL_FD = new TH1D("#phi_{e} for 1e2X (QEL Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_MEC_FD = new TH1D("#phi_{e} for 1e2X (MEC Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_RES_FD = new TH1D("#phi_{e} for 1e2X (RES Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1e2X_DIS_FD = new TH1D("#phi_{e} for 1e2X (DIS Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    string hPhi_e_1e2X_CD_Dir = Phi_e_1e2X_Directory, hPhi_e_1e2X_FD_Dir = Phi_e_1e2X_Directory;
//    string hPhi_e_1e2X_QEL_CD_Dir = Phi_e_1e2X_Directory, hPhi_e_1e2X_QEL_FD_Dir = Phi_e_1e2X_Directory;
//    string hPhi_e_1e2X_MEC_CD_Dir = Phi_e_1e2X_Directory, hPhi_e_1e2X_MEC_FD_Dir = Phi_e_1e2X_Directory;
//    string hPhi_e_1e2X_RES_CD_Dir = Phi_e_1e2X_Directory, hPhi_e_1e2X_RES_FD_Dir = Phi_e_1e2X_Directory;
//    string hPhi_e_1e2X_DIS_CD_Dir = Phi_e_1e2X_Directory, hPhi_e_1e2X_DIS_FD_Dir = Phi_e_1e2X_Directory;

    /* Phi_e histograms (1e2p) */
//    TH1D *hPhi_e_All_Int_1e2p_CD = new TH1D("#phi_{e} 1e2p (All Int., CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1eQEL_2p_CD = new TH1D("#phi_{e} for 1e2p (QEL Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1eMEC_2p_CD = new TH1D("#phi_{e} for 1e2p (MEC Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1eRES_2p_CD = new TH1D("#phi_{e} for 1e2p (RES Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_1eDIS_2p_CD = new TH1D("#phi_{e} for 1e2p (DIS Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_All_Int_1e2p_FD = new TH1D("#phi_{e} 1e2p (All Int., FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_QEL_1e2p_FD = new TH1D("#phi_{e} for 1e2p (QEL Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_MEC_1e2p_FD = new TH1D("#phi_{e} for 1e2p (MEC Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_RES_1e2p_FD = new TH1D("#phi_{e} for 1e2p (RES Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_DIS_1e2p_FD = new TH1D("#phi_{e} for 1e2p (DIS Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string hPhi_e_All_Int_1e2p_FD_Dir = Phi_e_1e2p_Directory, hPhi_e_All_Int_1e2p_CD_Dir = Phi_e_1e2p_Directory;
    string hPhi_e_1eQEL_2p_CD_Dir = Phi_e_1e2p_Directory, hPhi_e_QEL_1e2p_FD_Dir = Phi_e_1e2p_Directory;
    string hPhi_e_1eMEC_2p_CD_Dir = Phi_e_1e2p_Directory, hPhi_e_MEC_1e2p_FD_Dir = Phi_e_1e2p_Directory;
    string hPhi_e_1eRES_2p_CD_Dir = Phi_e_1e2p_Directory, hPhi_e_RES_1e2p_FD_Dir = Phi_e_1e2p_Directory;
    string hPhi_e_1eDIS_2p_CD_Dir = Phi_e_1e2p_Directory, hPhi_e_DIS_1e2p_FD_Dir = Phi_e_1e2p_Directory;

    /* Phi_e histograms (2p) */
//    TH1D *hPhi_e_All_Int_2p_CD = new TH1D("#phi_{e} 2p (All Int., CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_QEL_2p_CD = new TH1D("#phi_{e} for 2p (QEL Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_MEC_2p_CD = new TH1D("#phi_{e} for 2p (MEC Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_RES_2p_CD = new TH1D("#phi_{e} for 2p (RES Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
//    TH1D *hPhi_e_DIS_2p_CD = new TH1D("#phi_{e} for 2p (DIS Only, CD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_All_Int_2p_FD = new TH1D("#phi_{e} 2p (All Int., FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_QEL_2p_FD = new TH1D("#phi_{e} for 2p (QEL Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_MEC_2p_FD = new TH1D("#phi_{e} for 2p (MEC Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_RES_2p_FD = new TH1D("#phi_{e} for 2p (RES Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    TH1D *hPhi_e_DIS_2p_FD = new TH1D("#phi_{e} for 2p (DIS Only, FD)", ";#phi_{e} [Deg];", 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p);
    string hPhi_e_All_Int_2p_CD_Dir = Phi_e_2p_Directory, hPhi_e_All_Int_2p_FD_Dir = Phi_e_2p_Directory;
    string hPhi_e_QEL_2p_CD_Dir = Phi_e_2p_Directory, hPhi_e_QEL_2p_FD_Dir = Phi_e_2p_Directory;
    string hPhi_e_MEC_2p_CD_Dir = Phi_e_2p_Directory, hPhi_e_MEC_2p_FD_Dir = Phi_e_2p_Directory;
    string hPhi_e_RES_2p_CD_Dir = Phi_e_2p_Directory, hPhi_e_RES_2p_FD_Dir = Phi_e_2p_Directory;
    string hPhi_e_DIS_2p_CD_Dir = Phi_e_2p_Directory, hPhi_e_DIS_2p_FD_Dir = Phi_e_2p_Directory;
    //</editor-fold>

// Theta_e vs. Phi_e -----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e vs. Phi_e">
    /* Theta_e vs. Phi_e histograms (no #(e) cut) */
//    TH2D *hTheta_e_VS_Phi_e_All_e_CD = new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, CD)", "#theta_{e} vs. #phi_{e}  (no #(e) cut, CD);#phi_{e} [Deg];#theta_{e} [Deg]",
//                                                250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 35, 140);
    TH2D *hTheta_e_VS_Phi_e_All_e_FD = new TH2D("#theta_{e} vs. #phi_{e} (no #(e) cut, FD)", "#theta_{e} vs. #phi_{e}  (no #(e) cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 0, 50);
    string hTheta_e_VS_Phi_e_All_e_CD_Dir = Theta_e_VS_Phi_e_All_e_Directory, hTheta_e_VS_Phi_e_All_e_FD_Dir = Theta_e_VS_Phi_e_All_e_Directory;

    /* Theta_e vs. Phi_e histograms (1e cut) */
//    TH2D *hTheta_e_VS_Phi_e_1e_cut_CD = new TH2D("#theta_{e} vs. #phi_{e} (1e Cut, CD)", "#theta_{e} vs. #phi_{e}  (1e Cut, CD);#phi_{e} [Deg];#theta_{e} [Deg]",
//                                                 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 35, 140);
    TH2D *hTheta_e_VS_Phi_e_1e_cut_FD = new TH2D("#theta_{e} vs. #phi_{e} (1e Cut, FD)", "#theta_{e} vs. #phi_{e} (1e Cut, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                                 250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 0, 50);
    string hTheta_e_VS_Phi_e_1e_cut_CD_Dir = Theta_e_VS_Phi_e_1e_cut_Directory, hTheta_e_VS_Phi_e_1e_cut_FD_Dir = Theta_e_VS_Phi_e_1e_cut_Directory;

    /* Theta_e vs. Phi_e histograms (2p) */
//    TH2D *hTheta_e_VS_Phi_e_2p_CD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 2p, CD)", "#theta_{e} vs. #phi_{e} (All Int., 2p, CD);#phi_{e} [Deg];#theta_{e} [Deg]",
//                                             250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 35, 140);
    TH2D *hTheta_e_VS_Phi_e_2p_FD = new TH2D("#theta_{e} vs. #phi_{e} (All Int., 2p, FD)", "#theta_{e} vs. #phi_{e} (All Int., 2p, FD);#phi_{e} [Deg];#theta_{e} [Deg]",
                                             250, phi_lp_lower_lim_2p, phi_lp_upper_lim_2p, 250, 0, 50);
    string hTheta_e_VS_Phi_e_2p_CD_Dir = Theta_e_VS_Phi_e_2p_Directory, hTheta_e_VS_Phi_e_2p_FD_Dir = Theta_e_VS_Phi_e_2p_Directory;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Q2 histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Q2 histograms">
    /* Q2 histograms (no #(e) cut) */
    THStack *sQ2_All_e = new THStack("Q^{2} (no #(e) cut, CD & FD)", "Q^{2} Histogram (no #(e) cut, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_All_e_CD = new TH1D("Q^{2} (no #(e) cut, CD)", "Q^{2} (no #(e) cut, CD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    TH1D *hQ2_All_e_FD = new TH1D("Q^{2} (no #(e) cut, FD)", "Q^{2} (no #(e) cut, FD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    string hQ2_All_e_CD_Dir = Q2_All_e_Directory, hQ2_All_e_FD_Dir = Q2_All_e_Directory;

    /* Q2 histograms (1e cut) */
    THStack *sQ2_1e_cut = new THStack("Q^{2} (1e Cut ,CD & FD)", "Q^{2} Histogram (1e Cut, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1e_cut_CD = new TH1D("Q^{2} (1e Cut, CD)", "Q^{2} (1e Cut, CD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    TH1D *hQ2_1e_cut_FD = new TH1D("Q^{2} (1e Cut, FD)", "Q^{2} (1e Cut, FD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    string hQ2_1e_cut_CD_Dir = Q2_1e_cut_Directory, hQ2_1e_cut_FD_Dir = Q2_1e_cut_Directory;

    /* Q2 histograms (MicroBooNE) */
//    THStack *sQ2_MicroBooNE = new THStack("Q^{2} (MicroBooNE ,CD & FD)", "Q^{2} Histogram (MicroBooNE, CD & FD);Q^{2} [GeV^{2}];");
//    TH1D *hQ2_MicroBooNE_CD = new TH1D("Q^{2} (MicroBooNE, CD)", "Q^{2} (MicroBooNE, CD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
//    TH1D *hQ2_MicroBooNE_FD = new TH1D("Q^{2} (MicroBooNE, FD)", "Q^{2} (MicroBooNE, FD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
//    string hQ2_MicroBooNE_CD_Dir = Q2_MicroBooNE_Directory, hQ2_MicroBooNE_FD_Dir = Q2_MicroBooNE_Directory;

    /* Q2 histograms (1e2X) */
//    THStack *sQ2_1e2X = new THStack("Q^{2} (1e2X, CD & FD)", "Q^{2} (1e2X, CD & FD);Q^{2} [GeV^{2}];");
//    TH1D *hQ2_1e2X_CD = new TH1D("Q^{2} (1e2X, CD)", "Q^{2} (1e2X, CD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
//    TH1D *hQ2_1e2X_FD = new TH1D("Q^{2} (1e2X, FD)", "Q^{2} (1e2X, FD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
//    string hQ2_1e2X_CD_Dir = Q2_1e2X_Directory, hQ2_1e2X_FD_Dir = Q2_1e2X_Directory;

    /* Q2 histograms (1e2p) */
    THStack *sQ2_1e2p = new THStack("Q^{2} (1e2p, CD & FD)", "Q^{2} (1e2p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_1e2p_CD = new TH1D("Q^{2} (1e2p, CD)", "Q^{2} (1e2p, CD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    TH1D *hQ2_1e2p_FD = new TH1D("Q^{2} (1e2p, FD)", "Q^{2} (1e2p, FD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    string hQ2_1e2p_CD_Dir = Q2_1e2p_Directory, hQ2_1e2p_FD_Dir = Q2_1e2p_Directory;

    /* Q2 histograms (2p) */
    THStack *sQ2_2p = new THStack("Q^{2} (2p, CD & FD)", "Q^{2} (2p, CD & FD);Q^{2} [GeV^{2}];");
    TH1D *hQ2_2p_CD = new TH1D("Q^{2} (2p, CD)", "Q^{2} (2p, CD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    TH1D *hQ2_2p_FD = new TH1D("Q^{2} (2p, FD)", "Q^{2} (2p, FD);Q^{2} [GeV^{2}];", 250, 0, 1.1 * beamE);
    string hQ2_2p_CD_Dir = Q2_2p_Directory, hQ2_2p_FD_Dir = Q2_2p_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Energy (E_e) histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Energy (E_e) histograms">
    /* Energy (E_e) histograms (1e cut, CD & FD) */
    THStack *sE_e_1e_cut = new THStack("E_{e} (CD & FD)", "E_{e} Histogram (1e Cut, CD & FD);E_{e} [GeV]");
//    TH1D *hE_e_1e_cut_CD = new TH1D("E_{e} (1e Cut, CD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_1e_cut_FD = new TH1D("E_{e} (1e Cut, FD)", ";E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_1e_cut_CD_Dir = E_e_All_Int_1e_cut_Directory, hE_e_1e_cut_FD_Dir = E_e_All_Int_1e_cut_Directory;

//    TH2D *hE_e_VS_Theta_e_All_Int_1e_cut_CD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1e Cut, CD)",
//                                                       "E_{e} vs. #theta_{e} (All Int., 1e Cut, CD);#theta_{e} [Deg];E_{e} [GeV]", 250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_All_Int_1e_cut_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., 1e Cut, FD)",
                                                       "E_{e} vs. #theta_{e} (All Int., 1e Cut, FD);#theta_{e} [Deg];E_{e} [GeV]", 250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_1e_cut_CD_Dir = E_e_VS_Theta_e_All_Int_1e_cut_Directory, hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir = E_e_VS_Theta_e_All_Int_1e_cut_Directory;

    /* Energy (E_e) histograms (2p, CD & FD) */
//    THStack *sE_e_2p_CD = new THStack("E_{e} (2p, CD)", "E_{e} Histogram (2p, CD);E_{e} [GeV]");
    THStack *sE_e_2p_FD = new THStack("E_{e} (2p, FD)", "E_{e} Histogram (2p, FD);E_{e} [GeV]");
    string sE_e_2p_CD_Dir = E_e_All_Int_2p_stack_Directory, sE_e_2p_FD_Dir = E_e_All_Int_2p_stack_Directory;

//    TH1D *hE_e_All_Int_2p_CD = new TH1D("E_{e} (All Int., 2p, CD)", "E_{e} Histogram (All Int., 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
//    TH1D *hE_e_QEL_2p_CD = new TH1D("E_{e} (QEL Only, 2p, CD)", "E_{e} Histogram (QEL Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
//    TH1D *hE_e_MEC_2p_CD = new TH1D("E_{e} (MEC Only, 2p, CD)", "E_{e} Histogram (MEC Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
//    TH1D *hE_e_RES_2p_CD = new TH1D("E_{e} (RES Only, 2p, CD)", "E_{e} Histogram (RES Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
//    TH1D *hE_e_DIS_2p_CD = new TH1D("E_{e} (DIS Only, 2p, CD)", "E_{e} Histogram (DIS Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_All_Int_2p_FD = new TH1D("E_{e} (All Int., 2p, FD)", "E_{e} Histogram (All Int., 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_QEL_2p_FD = new TH1D("E_{e} (QEL Only, 2p, FD)", "E_{e} Histogram (QEL Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_MEC_2p_FD = new TH1D("E_{e} (MEC Only, 2p, FD)", "E_{e} Histogram (MEC Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_RES_2p_FD = new TH1D("E_{e} (RES Only, 2p, FD)", "E_{e} Histogram (RES Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
    TH1D *hE_e_DIS_2p_FD = new TH1D("E_{e} (DIS Only, 2p, FD)", "E_{e} Histogram (DIS Only, 2p, CD);E_{e} [GeV]", 100, 0, beamE * 1.1);
    string hE_e_All_Int_2p_CD_Dir = E_e_All_Int_2p_Directory, hE_e_All_Int_2p_FD_Dir = E_e_All_Int_2p_Directory;
    string hE_e_QEL_2p_CD_Dir = E_e_All_Int_2p_Directory, hE_e_QEL_2p_FD_Dir = E_e_All_Int_2p_Directory;
    string hE_e_MEC_2p_CD_Dir = E_e_All_Int_2p_Directory, hE_e_MEC_2p_FD_Dir = E_e_All_Int_2p_Directory;
    string hE_e_RES_2p_CD_Dir = E_e_All_Int_2p_Directory, hE_e_RES_2p_FD_Dir = E_e_All_Int_2p_Directory;
    string hE_e_DIS_2p_CD_Dir = E_e_All_Int_2p_Directory, hE_e_DIS_2p_FD_Dir = E_e_All_Int_2p_Directory;

    /* E_e vs. Theta_e (2p, CD & FD) */
//    TH2D *hE_e_VS_Theta_e_All_Int_2p_CD = new TH2D("E_{e} vs. #theta_{e} (All Int., CD)", "E_{e} vs. #theta_{e} (All Int., 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
//                                                   250, 35, 140, 250, 0, beamE * 1.1);
//    TH2D *hE_e_VS_Theta_e_QEL_2p_CD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p, CD)", "E_{e} vs. #theta_{e} (QEL Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
//                                               250, 35, 140, 250, 0, beamE * 1.1);
//    TH2D *hE_e_VS_Theta_e_MEC_2p_CD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 2p, CD)", "E_{e} vs. #theta_{e} (MEC Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
//                                               250, 35, 140, 250, 0, beamE * 1.1);
//    TH2D *hE_e_VS_Theta_e_RES_2p_CD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 2p, CD)", "E_{e} vs. #theta_{e} (RES Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
//                                               250, 35, 140, 250, 0, beamE * 1.1);
//    TH2D *hE_e_VS_Theta_e_DIS_2p_CD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 2p, CD)", "E_{e} vs. #theta_{e} (DIS Only, 2p, CD);#theta_{e} [Deg];E_{e} [GeV]",
//                                               250, 35, 140, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_All_Int_2p_FD = new TH2D("E_{e} vs. #theta_{e} (All Int., FD)", "E_{e} vs. #theta_{e} (All Int., 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                                   250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_QEL_2p_FD = new TH2D("E_{e} vs. #theta_{e} (QEL Only, 2p, FD)", "E_{e} vs. #theta_{e} (QEL Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_MEC_2p_FD = new TH2D("E_{e} vs. #theta_{e} (MEC Only, 2p, FD)", "E_{e} vs. #theta_{e} (MEC Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_RES_2p_FD = new TH2D("E_{e} vs. #theta_{e} (RES Only, 2p, FD)", "E_{e} vs. #theta_{e} (RES Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    TH2D *hE_e_VS_Theta_e_DIS_2p_FD = new TH2D("E_{e} vs. #theta_{e} (DIS Only, 2p, FD)", "E_{e} vs. #theta_{e} (DIS Only, 2p, FD);#theta_{e} [Deg];E_{e} [GeV]",
                                               250, 0, 50, 250, 0, beamE * 1.1);
    string hE_e_VS_Theta_e_All_Int_2p_CD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory, hE_e_VS_Theta_e_All_Int_2p_FD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory;
    string hE_e_VS_Theta_e_QEL_2p_CD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory, hE_e_VS_Theta_e_QEL_2p_FD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory;
    string hE_e_VS_Theta_e_MEC_2p_CD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory, hE_e_VS_Theta_e_MEC_2p_FD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory;
    string hE_e_VS_Theta_e_RES_2p_CD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory, hE_e_VS_Theta_e_RES_2p_FD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory;
    string hE_e_VS_Theta_e_DIS_2p_CD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory, hE_e_VS_Theta_e_DIS_2p_FD_Dir = E_e_VS_Theta_e_All_Int_2p_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Energy Transfer (ET) histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Energy Transfer (ET) histograms">
    /* ET around 15 Deg */
//    THStack *sET15_All_Int_2p_CD = new THStack("ET around 15#circ (All Int., 2p, CD)",
//                                               "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (All Int., 2p, CD);E_{beam}-E_{e} [GeV]");
    THStack *sET15_All_Int_2p_FD = new THStack("ET around 15#circ (All Int., 2p, FD)",
                                               "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (All Int., 2p, FD);E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_2p_CD_Dir = ETrans_15_stack_Directory, sET15_All_Int_2p_FD_Dir = ETrans_15_stack_Directory;

//    TH1D *hET15_All_Int_2p_CD = new TH1D("ET around 15#circ (All Int., 2p, CD)",
//                                         "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (All Int., 2p, CD);E_{beam}-E_{e} [GeV]",
//                                         100, 0, beamE * 1.1);
//    TH1D *hET15_QEL_2p_CD = new TH1D("ET around 15#circ (QEL Only, 2p, CD)",
//                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (QEL Only, 2p, CD);E_{beam}-E_{e} [GeV]",
//                                     100, 0, beamE * 1.1);
//    TH1D *hET15_MEC_2p_CD = new TH1D("ET around 15#circ (MEC Only, 2p, CD)",
//                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (MEC Only, 2p, CD);E_{beam}-E_{e} [GeV]",
//                                     100, 0, beamE * 1.1);
//    TH1D *hET15_RES_2p_CD = new TH1D("ET around 15#circ (RES Only, 2p, CD)",
//                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (RES Only, 2p, CD);E_{beam}-E_{e} [GeV]",
//                                     100, 0, beamE * 1.1);
//    TH1D *hET15_DIS_2p_CD = new TH1D("ET around 15#circ (DIS Only, 2p, CD)",
//                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (DIS Only, 2p, CD);E_{beam}-E_{e} [GeV]",
//                                     100, 0, beamE * 1.1);
    TH1D *hET15_All_Int_2p_FD = new TH1D("ET around 15#circ (All Int., 2p, FD)",
                                         "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (All Int., 2p, FD);E_{beam}-E_{e} [GeV]",
                                         100, 0, beamE * 1.1);
    TH1D *hET15_QEL_2p_FD = new TH1D("ET around 15#circ (QEL Only, 2p, FD)",
                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (QEL Only, 2p, FD);E_{beam}-E_{e} [GeV]",
                                     100, 0, beamE * 1.1);
    TH1D *hET15_MEC_2p_FD = new TH1D("ET around 15#circ (MEC Only, 2p, FD)",
                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (MEC Only, 2p, FD);E_{beam}-E_{e} [GeV]",
                                     100, 0, beamE * 1.1);
    TH1D *hET15_RES_2p_FD = new TH1D("ET around 15#circ (RES Only, 2p, FD)",
                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (RES Only, 2p, FD);E_{beam}-E_{e} [GeV]",
                                     100, 0, beamE * 1.1);
    TH1D *hET15_DIS_2p_FD = new TH1D("ET around 15#circ (DIS Only, 2p, FD)",
                                     "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ (DIS Only, 2p, FD);E_{beam}-E_{e} [GeV]",
                                     100, 0, beamE * 1.1);
    string hET15_All_Int_2p_CD_Dir = ETrans_All_Int_15_Directory, hET15_All_Int_2p_FD_Dir = ETrans_All_Int_15_Directory;
    string hET15_QEL_2p_CD_Dir = ETrans_QEL_15_Directory, hET15_QEL_2p_FD_Dir = ETrans_QEL_15_Directory;
    string hET15_MEC_2p_CD_Dir = ETrans_MEC_15_Directory, hET15_MEC_2p_FD_Dir = ETrans_MEC_15_Directory;
    string hET15_RES_2p_CD_Dir = ETrans_RES_15_Directory, hET15_RES_2p_FD_Dir = ETrans_RES_15_Directory;
    string hET15_DIS_2p_CD_Dir = ETrans_DIS_15_Directory, hET15_DIS_2p_FD_Dir = ETrans_DIS_15_Directory;
    string tET15 = "ET (E_{beam}-E_{e}) in the Angle Range 14#circ #leq #theta_{e} #leq 16#circ";
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Ecal reconstruction histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Ecal reconstruction histograms">
    //TODO: confirm with Adi if Ecal should be separated to CD and FD or not
    THStack *sEcal_2p = new THStack("E_{cal} Reconstruction (2p)", "E_{cal} Reconstruction (2p);E_{cal} = E_{e} + T_{p_{L}} + T_{p_{R}} [GeV]");
    string sEcal_2p_Dir = Ecal_stack_2p_Directory;

    TH1D *hEcal_All_Int_2p = new TH1D("E_{cal} Reconstruction (All Int., 2p)", "E_{cal} Reconstruction (All Int., 2p);E_{cal} = E_{e} + T_{p_{L}} + T_{p_{R}} [GeV]",
                                      100, 0, beamE * 1.35);
    TH1D *hEcal_QEL_2p = new TH1D("E_{cal} Reconstruction (QEL only, 2p)", "E_{cal} Reconstruction (QEL only, 2p);E_{cal} = E_{e} + T_{p_{L}} + T_{p_{R}} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_MEC_2p = new TH1D("E_{cal} Reconstruction (MEC only, 2p)", "E_{cal} Reconstruction (MEC only, 2p);E_{cal} = E_{e} + T_{p_{L}} + T_{p_{R}} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_RES_2p = new TH1D("E_{cal} Reconstruction (RES only, 2p)", "E_{cal} Reconstruction (RES only, 2p);E_{cal} = E_{e} + T_{p_{L}} + T_{p_{R}} [GeV]",
                                  100, 0, beamE * 1.35);
    TH1D *hEcal_DIS_2p = new TH1D("E_{cal} Reconstruction (DIS only, 2p)", "E_{cal} Reconstruction (DIS only, 2p);E_{cal} = E_{e} + T_{p_{L}} + T_{p_{R}} [GeV]",
                                  100, 0, beamE * 1.35);
    string hEcal_All_Int_2p_Dir = Ecal_All_Int_2p_Directory, hEcal_QEL_2p_Dir = Ecal_QEL_2p_Directory, hEcal_MEC_2p_Dir = Ecal_MEC_2p_Directory;
    string hEcal_RES_2p_Dir = Ecal_RES_2p_Directory, hEcal_DIS_2p_Dir = Ecal_DIS_2p_Directory;
    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// MicroBooNE histogram reconstruction
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="MicroBooNE histogram reconstruction">

    //TODO: finish setting up these histograms
    THStack *sGamma_Lab, *sGamma_mu_p_tot, *sdP_T, *sGamma_Lab_weighted, *sGamma_mu_p_tot_weighted_Stack, *sdP_T_weighted;

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE gamma plots (unweighted)">
    TH1D *hGamma_Lab_All_Int = new TH1D("cos(#gamma_{Lab}) (All Int., MicroBooNE)",
                                        "cos(#gamma_{Lab}) Histogram (All Int., MicroBooNE);cos(#gamma_{Lab})",
                                        8, Gamma_Lab_lower_lim, Gamma_Lab_upper_lim);
    TH1D *hGamma_mu_p_tot = new TH1D("cos(#gamma_{#mu,p_{L}+p_{R}}) (All Int., MicroBooNE)",
                                     "cos(#gamma_{#mu,p_{L}+p_{R}}) Histogram (All Int., MicroBooNE);cos(#gamma_{#mu,p_{L}+p_{R}})",
                                     8, Gamma_mu_p_tot_lower_lim, Gamma_mu_p_tot_upper_lim);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE gamma plots (Q4 weighted)">
    TH1D *hGamma_Lab_weighted = new TH1D("cos(#gamma_{Lab}) (Q^{4} weighted, All Int., MicroBooNE)",
                                         "cos(#gamma_{Lab}) Histogram (Q^{4} weighted, All Int., MicroBooNE);cos(#gamma_{Lab})",
                                         8, Gamma_Lab_weighted_lower_lim, Gamma_Lab_weighted_upper_lim);

    TH1D *hGamma_mu_p_tot_weighted = new TH1D("cos(#gamma_{#mu,p_{L}+p_{R}}) (Q^{4} weighted, All Int., MicroBooNE)",
                                              "cos(#gamma_{#mu,p_{L}+p_{R}}) Histogram (Q^{4} weighted, All Int., MicroBooNE);cos(#gamma_{#mu,p_{L}+p_{R}})",
                                              8, Gamma_mu_p_tot_weighted_lower_lim, Gamma_mu_p_tot_weighted_upper_lim);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE gamma plots (All Int., no pions)">
    TH1D *hGamma_Lab_All_Int_noPions = new TH1D("cos(#gamma_{Lab}) w/o pions (All Int., MicroBooNE)",
                                                "cos(#gamma_{Lab}) w/o Pions Histogram (All Int., MicroBooNE);cos(#gamma_{Lab})",
                                                8, Gamma_Lab_noPions_All_Int_lower_lim, Gamma_Lab_noPions_All_Int_upper_lim);
    TH1D *hGamma_Lab_All_Int_noPions_weighted = new TH1D("cos(#gamma_{Lab}) w/o pions (Q^{4} weighted, All Int., MicroBooNE)",
                                                         "cos(#gamma_{Lab}) Histogram  w/o pions (Q^{4} weighted, All Int., MicroBooNE);cos(#gamma_{Lab})",
                                                         8, Gamma_Lab_noPions_All_Int_weighted_lower_lim, Gamma_Lab_noPions_All_Int_weighted_upper_lim);

    TH1D *hGamma_Lab_QEL_noPions = new TH1D("cos(#gamma_{Lab}) w/o Pions (QEL Only, MicroBooNE)",
                                            "cos(#gamma_{Lab}) Histogram w/o Pions (QEL Only, MicroBooNE);cos(#gamma_{Lab})",
                                            8, Gamma_Lab_noPions_QEL_lower_lim, Gamma_Lab_noPions_QEL_upper_lim);
    TH1D *hGamma_Lab_QEL_noPions_weighted = new TH1D("cos(#gamma_{Lab}) w/o Pions (Q^{4} weighted, QEL Only, MicroBooNE)",
                                                     "cos(#gamma_{Lab}) Histogram w/o Pions (Q^{4} weighted, QEL Only, MicroBooNE);cos(#gamma_{Lab})",
                                                     8, Gamma_Lab_noPions_QEL_weighted_lower_lim, Gamma_Lab_noPions_QEL_weighted_upper_lim);

    TH1D *hGamma_Lab_MEC_noPions = new TH1D("cos(#gamma_{Lab}) w/o Pions (MEC Only, MicroBooNE)",
                                            "cos(#gamma_{Lab}) Histogram w/o Pions (MEC Only, MicroBooNE);cos(#gamma_{Lab})",
                                            8, Gamma_Lab_noPions_MEC_lower_lim, Gamma_Lab_noPions_MEC_upper_lim);
    TH1D *hGamma_Lab_MEC_noPions_weighted = new TH1D("cos(#gamma_{Lab}) w/o Pions (Q^{4} weighted, MEC Only, MicroBooNE)",
                                                     "cos(#gamma_{Lab}) Histogram w/o Pions (Q^{4} weighted, MEC Only, MicroBooNE);cos(#gamma_{Lab})",
                                                     8, Gamma_Lab_noPions_MEC_weighted_lower_lim, Gamma_Lab_noPions_MEC_weighted_upper_lim);

    TH1D *hGamma_Lab_RES_noPions = new TH1D("cos(#gamma_{Lab}) w/o Pions (RES Only, MicroBooNE)",
                                            "cos(#gamma_{Lab}) Histogram w/o Pions (RES Only, MicroBooNE);cos(#gamma_{Lab})",
                                            8, Gamma_Lab_noPions_RES_lower_lim, Gamma_Lab_noPions_RES_upper_lim);
    TH1D *hGamma_Lab_RES_noPions_weighted = new TH1D("cos(#gamma_{Lab}) w/o Pions (Q^{4} weighted, RES Only, MicroBooNE)",
                                                     "cos(#gamma_{Lab}) Histogram w/o Pions (Q^{4} weighted, RES Only, MicroBooNE);cos(#gamma_{Lab})",
                                                     8, Gamma_Lab_noPions_RES_weighted_lower_lim, Gamma_Lab_noPions_RES_weighted_upper_lim);

    TH1D *hGamma_Lab_DIS_noPions = new TH1D("cos(#gamma_{Lab}) w/o Pions (DIS Only, MicroBooNE)",
                                            "cos(#gamma_{Lab}) Histogram w/o Pions (DIS Only, MicroBooNE);cos(#gamma_{Lab})",
                                            8, Gamma_Lab_noPions_DIS_lower_lim, Gamma_Lab_noPions_DIS_upper_lim);
    TH1D *hGamma_Lab_DIS_noPions_weighted = new TH1D("cos(#gamma_{Lab}) w/o Pions (Q^{4} weighted, DIS Only, MicroBooNE)",
                                                     "cos(#gamma_{Lab}) Histogram w/o Pions (Q^{4} weighted, DIS Only, MicroBooNE);cos(#gamma_{Lab})",
                                                     8, Gamma_Lab_noPions_DIS_weighted_lower_lim, Gamma_Lab_noPions_DIS_weighted_upper_lim);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE histogram reconstruction - MicroBooNE dP_T plots (unweighted and Q4 weighted)">
    TH1D *dP_T_hist = new TH1D("#deltaP_{T}", ";#deltaP_{T} = |#bf{P}_{l,T} + #bf{P}_{L,T} + #bf{P}_{R,T}| [GeV/c]",
                               100, dP_T_hist_lower_lim, dP_T_hist_upper_lim);
    TH1D *dP_T_hist_weighted = new TH1D("#deltaP_{T} (Q^{4} weighted)", ";#deltaP_{T} = |#bf{P}_{l,T} + #bf{P}_{L,T} + #bf{P}_{R,T}| [GeV/c]",
                                        100, dP_T_hist_weighted_lower_lim, dP_T_hist_weighted_upper_lim);
    //</editor-fold>

    //<editor-fold desc="MicroBooNE momentum plots (for self-examination)">
    THStack *MomentumStack_MicroBooNE = new THStack("Momentum Stack (MicroBooNE)", ";Momentum [GeV]");

    TH1D *P_L_hist = new TH1D("P_{L}", "Momentum of Leading Proton (P_{L});P_{L} [GeV/c]", 100, P_L_hist_lower_lim, P_L_hist_upper_lim);
    TH1D *P_R_hist = new TH1D("P_{R}", "Momentum of Recoil Proton (P_{R});P_{R} [GeV/c]", 100, P_R_hist_lower_lim, P_R_hist_upper_lim);
    TH1D *P_lp_hist = new TH1D("P_{l}", "Momentum of Lepton (P_{l});P_{l} [GeV/c]", 100, P_lp_hist_lower_lim, P_lp_hist_upper_lim);
    TH1D *P_pion_hist = new TH1D("P_{#pi^{#pm}}", "Momentum of Pions (P_{#pi^{#pm}});P_{#pi^{#pm}} [GeV/c]", 100, P_pion_hist_lower_lim, P_pion_hist_upper_lim);

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

    if (apply_cuts == true) {
        /* Read in target parameter files */
        clasAna.readInputParam((CutsDirectory + "ana.par").c_str());
        clasAna.readEcalPar((CutsDirectory + "ecal.par").c_str());

        // Cuts on electrons only:
        if (apply_SF_cuts == true) { clasAna.setEcalSFCuts(); }                        // making f_ecalSFCuts = ture
        if (apply_ECAL_fiducial_cut == true) { clasAna.setEcalEdgeCuts(); }            // making f_ecalEdgeCuts = ture (ECAL fiducial cuts)
        if (apply_Nphe_cut == true) { clasAna.setNpheCuts(); }                         // making f_NpheCuts = ture (HTCC cuts)

        // Cuts on protons and charged pions:
        if (apply_chi2_cuts_2p == true) { clasAna.setPidCuts(); }                      // making f_pidCuts = ture

        // Cuts on all particles:
        if (apply_Vz_cuts == true) {
            clasAna.setVertexCuts();                                                   // making f_vertexCuts = ture
            clasAna.setVzcuts(Vz_cuts.at(0), Vz_cuts.at(1));                           // setting Vz cuts for all (charged?) particles
        }

        // Cuts on charged particles:
        if (apply_DC_fiducial_cut == true) { clasAna.setDCEdgeCuts(); }                // making f_DCEdgeCuts = ture (DC fiducial cuts?)
        if (apply_dVz_cuts == true) {
            clasAna.setVertexCorrCuts();                                               // making f_corr_vertexCuts = ture
            clasAna.setVertexCorrCuts(dVz_cuts.at(0), dVz_cuts.at(1));                 // setting dVz cuts?
        }

        clasAna.printParams();
    }

    cout << "\n\nLooping over chain files...\n\n";

    clas12root::HipoChain chain;
    chain.Add(AnalyseFile.c_str());
    chain.SetReaderTags({0});                   //TODO: check with Justin what are these tags
    auto config_c12 = chain.GetC12Reader();     //TODO: check with Justin what is this used for
    auto &c12 = chain.C12ref();                 //TODO: check with Justin what is this used for

    auto db = TDatabasePDG::Instance();
    chain.db()->turnOffQADB();                  //TODO: check with Justin what is this used for

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
    int num_of_QEL_2p_events = 0, num_of_MEC_2p_events = 0, num_of_RES_2p_events = 0, num_of_DIS_2p_events = 0;

    int num_of_MicroBooNE_events_BC = 0;
    int num_of_MicroBooNE_events_BC_wNeutrons = 0, num_of_MicroBooNE_events_BC_wpi0 = 0, num_of_MicroBooNE_events_BC_wpip = 0, num_of_MicroBooNE_events_BC_wpim = 0;
    int num_of_MicroBooNE_events_AC = 0;
    int num_of_MicroBooNE_events_AC_wNeutrons = 0, num_of_MicroBooNE_events_AC_wpi0 = 0, num_of_MicroBooNE_events_AC_wpip = 0, num_of_MicroBooNE_events_AC_wpim = 0;
    //</editor-fold>

//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//  Looping over each HipoChain file
//  ---------------------------------------------------------------------------------------------------------------------------------------------------------------------

    int Np, Nkp, Nkm, Npip, Npim, Ne, Nd, Nn, No, Nf;

    while (chain.Next()) { // loop over events
        ++num_of_events; // logging Total #(events)

        clasAna.Run(c12);

        /* All of these particles are with clas12ana cuts
           Only cuts missing are Nphe and momentum cuts - to be applied later */
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
        Np = protons.size(), Nkp = Kplus.size(), Nkm = Kminus.size(), Npip = piplus.size(), Npim = piminus.size(), Ne = electrons.size();
        Nd = deuterons.size(), Nn = neutrals.size(), No = otherpart.size();

        /* Total number of particles in event (= Nf) */
        Nf = Np + Nkp + Nkm + Npip + Npim + Ne + Nd + Nn + No;

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

        // TODO: remove or change - clas12ana comes with 1e cut already

        //<editor-fold desc="All particles plots">
        TLorentzVector e_out, Q;
        double Theta_e_tmp, Phi_e_tmp, P_e_tmp, Q2;

        //<editor-fold desc="Electron plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < electrons.size(); i++) {
            Theta_e_tmp = electrons[i]->getTheta() * 180.0 / pi; // Theta_e_tmp in deg
            Phi_e_tmp = electrons[i]->getPhi() * 180.0 / pi; // Phi_e_tmp in deg

            P_e_tmp = electrons[i]->par()->getP();
            e_out.SetPxPyPzE(electrons[i]->par()->getPx(), electrons[i]->par()->getPy(), electrons[i]->par()->getPz(), sqrt(m_e * m_e + P_e_tmp * P_e_tmp));
            Q = beam - e_out;
            Q2 = fabs(Q.Mag2());

            if (electrons[i]->getRegion() == CD) {
//                hChi2_Electron_CD->Fill(electrons[i]->par()->getChi2Pid());

//                Vertex_Electron_Vx_CD->Fill(electrons[i]->par()->getVx());
//                Vertex_Electron_Vy_CD->Fill(electrons[i]->par()->getVy());
//                Vertex_Electron_Vz_CD->Fill(electrons[i]->par()->getVz());

                Beta_vs_P_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_Electrons_Only_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_CD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());

//                hTheta_e_All_e_CD->Fill(Theta_e_tmp);
//                hPhi_e_All_e_CD->Fill(Phi_e_tmp);
//                hTheta_e_VS_Phi_e_All_e_CD->Fill(Phi_e_tmp, Theta_e_tmp);

                hQ2_All_e_CD->Fill(Q2);

                if (electrons.size() == 1) { hQ2_1e_cut_CD->Fill(Q2); }

                if (electrons.size() == 1 && Nf == 3) {
//                    hQ2_1e2X_CD->Fill(Q2);

                    if (protons.size() == 2) { hQ2_1e2p_CD->Fill(Q2); }
                }
            } else if (electrons[i]->getRegion() == FD) {
                hChi2_Electron_FD->Fill(electrons[i]->par()->getChi2Pid());

//                Vertex_Electron_Vx_FD->Fill(electrons[i]->par()->getVx());
//                Vertex_Electron_Vy_FD->Fill(electrons[i]->par()->getVy());
//                Vertex_Electron_Vz_FD->Fill(electrons[i]->par()->getVz());

                Beta_vs_P_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_Electrons_Only_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_FD->Fill(electrons[i]->getP(), electrons[i]->par()->getBeta());

                hTheta_e_All_e_FD->Fill(Theta_e_tmp);
                hPhi_e_All_e_FD->Fill(Phi_e_tmp);
                hTheta_e_VS_Phi_e_All_e_FD->Fill(Phi_e_tmp, Theta_e_tmp);

                hQ2_All_e_FD->Fill(Q2);

                if (electrons.size() == 1) { hQ2_1e_cut_FD->Fill(Q2); }

                if (electrons.size() == 1 && Nf == 3) {
//                    hQ2_1e2X_FD->Fill(Q2);

                    if (protons.size() == 2) { hQ2_1e2p_FD->Fill(Q2); }
                }
            }
        } // end of loop over electrons vector
        //</editor-fold>

        //<editor-fold desc="Proton plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < protons.size(); i++) {
            if (protons[i]->getRegion() == CD) {
                hChi2_Proton_CD->Fill(protons[i]->par()->getChi2Pid());

                //TODO: figure out if the code shuold plot these or should I delete them:
//                Vertex_Proton_Vx_CD->Fill(protons[i]->par()->getVx());
//                Vertex_Proton_Vy_CD->Fill(protons[i]->par()->getVy());
//                Vertex_Proton_Vz_CD->Fill(protons[i]->par()->getVz());

                Beta_vs_P_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_Protons_Only_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_CD->Fill(protons[i]->getP(), protons[i]->par()->getBeta());
            } else if (protons[i]->getRegion() == FD) {
                hChi2_Proton_FD->Fill(protons[i]->par()->getChi2Pid());

                //TODO: figure out if the code shuold plot these or should I delete them:
//                Vertex_Proton_Vx_FD->Fill(protons[i]->par()->getVx());
//                Vertex_Proton_Vy_FD->Fill(protons[i]->par()->getVy());
//                Vertex_Proton_Vz_FD->Fill(protons[i]->par()->getVz());

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
                hChi2_Kplus_CD->Fill(Kplus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
            } else if (Kplus[i]->getRegion() == FD) {
                hChi2_Kplus_FD->Fill(Kplus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_FD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
            }
        } // end of loop over Kplus vector
        //</editor-fold>

        //<editor-fold desc="Kminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < Kminus.size(); i++) {
            if (Kminus[i]->getRegion() == CD) {
                hChi2_Kminus_CD->Fill(Kminus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            } else if (Kminus[i]->getRegion() == FD) {
                hChi2_Kminus_FD->Fill(Kminus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_FD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            }
        } // end of loop over Kminus vector
        //</editor-fold>

        //<editor-fold desc="piplus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piplus.size(); i++) {
            if (piplus[i]->getRegion() == CD) {
                hChi2_piplus_CD->Fill(piplus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
            } else if (piplus[i]->getRegion() == FD) {
                hChi2_piplus_FD->Fill(piplus[i]->par()->getChi2Pid());

                Beta_vs_P_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_All_e_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (no #(electron) cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                hChi2_piminus_CD->Fill(piminus[i]->par()->getChi2Pid());

                Beta_vs_P_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_All_e_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
            } else if (piminus[i]->getRegion() == FD) {
                hChi2_piminus_FD->Fill(piminus[i]->par()->getChi2Pid());

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

        /* Applying rough 1e cut */
        if (Ne != 1) { continue; } // the rough 1e cut
        ++num_of_events_with_exactly_1e; // logging #(events) w/ exactly 1e

        if (Nf - Ne == Np) {
            ++num_of_events_with_1enP; // logging #(events) w/ 1e & any #p 1enP

            if (protons.size() == 1) { ++num_of_events_with_1e1p; /* // logging #(events) w/ 1e1p */ }
        }

        /* Electron variables */
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
            if ((EoP_e >= SF_1e_lower_cut) && (EoP_e <= SF_1e_upper_cut)) { hSF_1e_cut_AC_FD->Fill(EoP_e), hSF_VS_P_e_1e_cut_AC_FD->Fill(P_e, EoP_e); }
        } else { hSF_1e_cut_BC_FD->Fill(EoP_e), hSF_VS_P_e_1e_cut_BC_FD->Fill(P_e, EoP_e); }

        /* Testing fiducial cuts */
        if (apply_cuts == false) {
            /* Fiducial plots before cuts */
            Vcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);

            /* Fiducial plots after cuts */
            if (electrons[0]->cal(PCAL)->getLv() >= fiducial_cut_Lv) { Vcal_VS_EoP_1e_AC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e); }
            if (electrons[0]->cal(PCAL)->getLw() >= fiducial_cut_Lw) { Wcal_VS_EoP_1e_AC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e); }
        } else {
            Vcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_1e_BC_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);
        }

        /* Testing Nphe cuts */
        int Nphe = electrons[0]->che(HTCC)->getNphe();

        if (apply_cuts == false) {
            /* Nphe plots before cuts */
            hNphe_1e_cut_BC_FD->Fill(Nphe);

            /* Nphe plots after cuts */
            if (Nphe >= clasAna.getNpheCuts()) { hNphe_1e_cut_AC_FD->Fill(Nphe); }
        } else {
            hNphe_1e_cut_BC_FD->Fill(Nphe);
        }
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Filling 1e only plots">

        //<editor-fold desc="Fill Electron plots (1e only, CD & FD)">
        for (auto &e: electrons) {
            if (e->getRegion() == CD) {
//                hChi2_Electron_1e_cut_CD->Fill(e->par()->getChi2Pid());

                Vertex_Electron_1e_Vx_CD->Fill(e->par()->getVx());
                Vertex_Electron_1e_Vy_CD->Fill(e->par()->getVy());
                Vertex_Electron_1e_Vz_CD->Fill(e->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(P_e, e->par()->getBeta());
                Beta_vs_P_1e_Electrons_Only_CD->Fill(P_e, e->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_CD->Fill(P_e, e->par()->getBeta());

//                hTheta_e_1e_cut_CD->Fill(Theta_e);
//                hPhi_e_1e_cut_CD->Fill(Phi_e);
//                hTheta_e_VS_Phi_e_1e_cut_CD->Fill(Phi_e, Theta_e);

                if (Nf == 3) {
//                    hTheta_e_All_Int_1e2X_CD->Fill(Theta_e);
//                    hPhi_e_1e2X_CD->Fill(Phi_e);

                    if (qel) {
                        ++num_of_QEL_1e2X_CD_events;
//                        hTheta_e_QEL_1e2X_CD->Fill(Theta_e);
//                        hPhi_e_1e2X_QEL_CD->Fill(Phi_e);
                    } else if (mec) {
                        ++num_of_MEC_1e2X_CD_events;
//                        hTheta_e_MEC_1e2X_CD->Fill(Theta_e);
//                        hPhi_e_1e2X_MEC_CD->Fill(Phi_e);
                    } else if (res) {
                        ++num_of_RES_1e2X_CD_events;
//                        hTheta_e_RES_1e2X_CD->Fill(Theta_e);
//                        hPhi_e_1e2X_RES_CD->Fill(Phi_e);
                    } else if (dis) {
                        ++num_of_DIS_1e2X_CD_events;
//                        hTheta_e_DIS_1e2X_CD->Fill(Theta_e);
//                        hPhi_e_1e2X_DIS_CD->Fill(Phi_e);
                    }

                    if (Np == 2) {
//                        hTheta_e_All_Int_1e2p_CD->Fill(Theta_e);
//                        hPhi_e_All_Int_1e2p_CD->Fill(Phi_e);

                        if (qel) {
                            ++num_of_QEL_1e2p_CD_events;
//                            hTheta_e_QEL_1e2p_CD->Fill(Theta_e);
//                            hPhi_e_1eQEL_2p_CD->Fill(Phi_e);
                        } else if (mec) {
                            ++num_of_MEC_1e2p_CD_events;
//                            hTheta_e_MEC_1e2p_CD->Fill(Theta_e);
//                            hPhi_e_1eMEC_2p_CD->Fill(Phi_e);
                        } else if (res) {
                            ++num_of_RES_1e2p_CD_events;
//                            hTheta_e_RES_1e2p_CD->Fill(Theta_e);
//                            hPhi_e_1eRES_2p_CD->Fill(Phi_e);
                        } else if (dis) {
                            ++num_of_DIS_1e2p_CD_events;
//                            hTheta_e_DIS_1e2p_CD->Fill(Theta_e);
//                            hPhi_e_1eDIS_2p_CD->Fill(Phi_e);
                        }
                    }
                }

//                hE_e_1e_cut_CD->Fill(E_e);
//                hE_e_VS_Theta_e_All_Int_1e_cut_CD->Fill(Theta_e, E_e);
            } else if (e->getRegion() == FD) {
                hChi2_Electron_1e_cut_FD->Fill(e->par()->getChi2Pid());

                Vertex_Electron_1e_Vx_FD->Fill(e->par()->getVx());
                Vertex_Electron_1e_Vy_FD->Fill(e->par()->getVy());
                Vertex_Electron_1e_Vz_FD->Fill(e->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(P_e, e->par()->getBeta());
                Beta_vs_P_1e_Electrons_Only_FD->Fill(P_e, e->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_FD->Fill(P_e, e->par()->getBeta());

                hTheta_e_1e_cut_FD->Fill(Theta_e);
                hPhi_e_1e_cut_FD->Fill(Phi_e);
                hTheta_e_VS_Phi_e_1e_cut_FD->Fill(Phi_e, Theta_e);

                if (Nf == 3) {
//                    hTheta_e_All_Int_1e2X_FD->Fill(Theta_e);
//                    hPhi_e_1e2X_FD->Fill(Phi_e);

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

                Vertex_Proton_1e_Vx_CD->Fill(p->par()->getVx());
                Vertex_Proton_1e_Vy_CD->Fill(p->par()->getVy());
                Vertex_Proton_1e_Vz_CD->Fill(p->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_1e_Protons_Only_CD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_CD->Fill(p->getP(), p->par()->getBeta());
            } else if (p->getRegion() == FD) {
                hChi2_Proton_1e_cut_FD->Fill(p->par()->getChi2Pid());

                Vertex_Proton_1e_Vx_FD->Fill(p->par()->getVx());
                Vertex_Proton_1e_Vy_FD->Fill(p->par()->getVy());
                Vertex_Proton_1e_Vz_FD->Fill(p->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_1e_Protons_Only_FD->Fill(p->getP(), p->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_FD->Fill(p->getP(), p->par()->getBeta());
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

                Vertex_Kplus_1e_Vx_CD->Fill(Kplus[i]->par()->getVx());
                Vertex_Kplus_1e_Vy_CD->Fill(Kplus[i]->par()->getVy());
                Vertex_Kplus_1e_Vz_CD->Fill(Kplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_1e_Kplus_Only_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_CD->Fill(Kplus[i]->getP(), Kplus[i]->par()->getBeta());

            } else if (Kplus[i]->getRegion() == FD) {
                hChi2_Kplus_1e_cut_FD->Fill(Kplus[i]->par()->getChi2Pid());

                Vertex_Kplus_1e_Vx_FD->Fill(Kplus[i]->par()->getVx());
                Vertex_Kplus_1e_Vy_FD->Fill(Kplus[i]->par()->getVy());
                Vertex_Kplus_1e_Vz_FD->Fill(Kplus[i]->par()->getVz());

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

                Vertex_Kminus_1e_Vx_CD->Fill(Kminus[i]->par()->getVx());
                Vertex_Kminus_1e_Vy_CD->Fill(Kminus[i]->par()->getVy());
                Vertex_Kminus_1e_Vz_CD->Fill(Kminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_1e_Kminus_Only_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_CD->Fill(Kminus[i]->getP(), Kminus[i]->par()->getBeta());
            } else if (Kminus[i]->getRegion() == FD) {
                hChi2_Kminus_1e_cut_FD->Fill(Kminus[i]->par()->getChi2Pid());

                Vertex_Kminus_1e_Vx_FD->Fill(Kminus[i]->par()->getVx());
                Vertex_Kminus_1e_Vy_FD->Fill(Kminus[i]->par()->getVy());
                Vertex_Kminus_1e_Vz_FD->Fill(Kminus[i]->par()->getVz());

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

                Vertex_piplus_1e_Vx_CD->Fill(piplus[i]->par()->getVx());
                Vertex_piplus_1e_Vy_CD->Fill(piplus[i]->par()->getVy());
                Vertex_piplus_1e_Vz_CD->Fill(piplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_1e_piplus_Only_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_CD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
            } else if (piplus[i]->getRegion() == FD) {
                hChi2_piplus_1e_cut_FD->Fill(piplus[i]->par()->getChi2Pid());

                Vertex_piplus_1e_Vx_FD->Fill(piplus[i]->par()->getVx());
                Vertex_piplus_1e_Vy_FD->Fill(piplus[i]->par()->getVy());
                Vertex_piplus_1e_Vz_FD->Fill(piplus[i]->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_1e_piplus_Only_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
                Beta_vs_P_positive_particles_1e_cut_FD->Fill(piplus[i]->getP(), piplus[i]->par()->getBeta());
            }
        } // end of loop over piplus vector
        //</editor-fold>

        //<editor-fold desc="piminus Beta vs. P plots (1e cut, CD & FD)">
        for (int i = 0; i < piminus.size(); i++) {
            if (piminus[i]->getRegion() == CD) {
                hChi2_piminus_1e_cut_CD->Fill(piminus[i]->par()->getChi2Pid());

                Vertex_piminus_1e_Vx_CD->Fill(piminus[i]->par()->getVx());
                Vertex_piminus_1e_Vy_CD->Fill(piminus[i]->par()->getVy());
                Vertex_piminus_1e_Vz_CD->Fill(piminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_1e_piminus_Only_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_CD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
            } else if (piminus[i]->getRegion() == FD) {
                hChi2_piminus_1e_cut_FD->Fill(piminus[i]->par()->getChi2Pid());

                Vertex_piminus_1e_Vx_FD->Fill(piminus[i]->par()->getVx());
                Vertex_piminus_1e_Vy_FD->Fill(piminus[i]->par()->getVy());
                Vertex_piminus_1e_Vz_FD->Fill(piminus[i]->par()->getVz());

                Beta_vs_P_1e_cut_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_1e_piminus_Only_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
                Beta_vs_P_negative_particles_1e_cut_FD->Fill(piminus[i]->getP(), piminus[i]->par()->getBeta());
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
//                LogEventCuts(P_e_MicroBooNE_AC_CD, electrons[0], "", e_mom_cuts_MicroBooNE.at(1), e_mom_cuts_MicroBooNE.at(0));
//            } else if (electrons[0]->getRegion() == FD) {
//                P_e_MicroBooNE_BC_FD->Fill(P_e_MicroBooNE.Mag());
//                LogEventCuts(P_e_MicroBooNE_AC_FD, electrons[0], "", e_mom_cuts_MicroBooNE.at(1), e_mom_cuts_MicroBooNE.at(0));
//            }
//
//            if (protons[0]->getRegion() == CD) {
//                P_p_MicroBooNE_BC_CD->Fill(P_p0_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_CD, protons[0], "", p_mom_cuts_MicroBooNE.at(1), p_mom_cuts_MicroBooNE.at(0));
//            } else if (protons[0]->getRegion() == FD) {
//                P_p_MicroBooNE_BC_FD->Fill(P_p0_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_FD, protons[0], "", p_mom_cuts_MicroBooNE.at(1), p_mom_cuts_MicroBooNE.at(0));
//            }
//
//            if (protons[1]->getRegion() == CD) {
//                P_p_MicroBooNE_BC_CD->Fill(P_p1_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_CD, protons[1], "", p_mom_cuts_MicroBooNE.at(1), p_mom_cuts_MicroBooNE.at(0));
//            } else if (protons[1]->getRegion() == FD) {
//                P_p_MicroBooNE_BC_FD->Fill(P_p1_MicroBooNE.Mag());
//                LogEventCuts(P_p_MicroBooNE_AC_FD, protons[1], "", p_mom_cuts_MicroBooNE.at(1), p_mom_cuts_MicroBooNE.at(0));
//            }
//
//            /* Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)") */
//            for (int i = 0; i < piplus.size(); i++) {
//                P_pcpion_MicroBooNE.SetMagThetaPhi(piplus[i]->getP(), piplus[i]->getTheta(), piplus[i]->getPhi());
//
//                if (piplus[i]->getRegion() == CD) {
//                    P_cpion_MicroBooNE_BC_CD->Fill(P_pcpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_CD, piplus[i], "", cpion_mom_cuts_MicroBooNE(1), cpion_momentum_lower_cut_MicroBooNE);
//                } else if (piplus[i]->getRegion() == FD) {
//                    P_cpion_MicroBooNE_BC_FD->Fill(P_pcpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_FD, piplus[i], "", cpion_mom_cuts_MicroBooNE(1), cpion_momentum_lower_cut_MicroBooNE);
//                }
//            } // end of loop over piplus vector
//
//            /* Pion momentum modulus (according to "no charged pions with momentum above 65 MeV/c (= 0.065 GeV)") */
//            for (int i = 0; i < piminus.size(); i++) {
//                P_ncpion_MicroBooNE.SetMagThetaPhi(piminus[i]->getP(), piminus[i]->getTheta(), piminus[i]->getPhi());
//
//                if (piminus[i]->getRegion() == CD) {
//                    P_cpion_MicroBooNE_BC_CD->Fill(P_ncpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_CD, piminus[i], "", cpion_mom_cuts_MicroBooNE(1), cpion_momentum_lower_cut_MicroBooNE);
//                } else if (piminus[i]->getRegion() == FD) {
//                    P_cpion_MicroBooNE_BC_FD->Fill(P_ncpion_MicroBooNE.Mag());
//                    LogEventCuts(P_cpion_MicroBooNE_AC_FD, piminus[i], "", cpion_mom_cuts_MicroBooNE(1), cpion_momentum_lower_cut_MicroBooNE);
//                }
//            } // end of loop over piminus vector
//            //</editor-fold>
//
//            //  Testing chi2 cuts (MicroBooNE) ----------------------------------------------------------------------------------------------------------------------
//
//            //<editor-fold desc="Testing chi2 before and after cuts">
//            /* Chi2 plots before and after cuts */
//            if (electrons[0]->getRegion() == CD) {
//                hChi2_Electron_MicroBooNE_BC_CD->Fill(electrons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Electron_1e_peak_MicroBooNE_CD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_CD) {
//                    hChi2_Electron_MicroBooNE_AC_CD->Fill(electrons[0]->par()->getChi2Pid());
//                }
//            } else if (electrons[0]->getRegion() == FD) {
//                hChi2_Electron_MicroBooNE_BC_FD->Fill(electrons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Electron_1e_peak_MicroBooNE_FD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_FD) {
//                    hChi2_Electron_MicroBooNE_AC_FD->Fill(electrons[0]->par()->getChi2Pid());
//                }
//            }
//
//            if (protons[0]->getRegion() == CD) {
//                hChi2_Proton_MicroBooNE_BC_CD->Fill(protons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_CD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD) {
//                    hChi2_Proton_MicroBooNE_AC_CD->Fill(protons[0]->par()->getChi2Pid());
//                }
//            } else if (protons[0]->getRegion() == FD) {
//                hChi2_Proton_MicroBooNE_BC_FD->Fill(protons[0]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_FD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD) {
//                    hChi2_Proton_MicroBooNE_AC_FD->Fill(protons[0]->par()->getChi2Pid());
//                }
//            }
//
//            if (protons[1]->getRegion() == CD) {
//                hChi2_Proton_MicroBooNE_BC_CD->Fill(protons[1]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_CD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD) {
//                    hChi2_Proton_MicroBooNE_AC_CD->Fill(protons[1]->par()->getChi2Pid());
//                }
//            } else if (protons[1]->getRegion() == FD) {
//                hChi2_Proton_MicroBooNE_BC_FD->Fill(protons[1]->par()->getChi2Pid()); // Before chi2 cuts
//
//                if (fabs(Chi2_Proton_1e_peak_MicroBooNE_FD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD) {
//                    hChi2_Proton_MicroBooNE_AC_FD->Fill(protons[1]->par()->getChi2Pid());
//                }
//            }
//
//            for (int i = 0; i < piplus.size(); i++) {
//                if (piplus[i]->getRegion() == CD) {
//                    hChi2_piplus_MicroBooNE_BC_CD->Fill(piplus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piplus_1e_peak_MicroBooNE_CD - piplus[i]->par()->getChi2Pid()) <= Chi2_piplus_cut_MicroBooNE_CD) {
//                        hChi2_piplus_MicroBooNE_AC_CD->Fill(piplus[i]->par()->getChi2Pid());
//                    }
//                } else if (piplus[i]->getRegion() == FD) {
//                    hChi2_piplus_MicroBooNE_BC_FD->Fill(piplus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piplus_1e_peak_MicroBooNE_FD - piplus[i]->par()->getChi2Pid()) <= Chi2_piplus_cut_MicroBooNE_FD) {
//                        hChi2_piplus_MicroBooNE_AC_FD->Fill(piplus[i]->par()->getChi2Pid());
//                    }
//                }
//            } // end of loop over piplus vector
//
//            for (int i = 0; i < piminus.size(); i++) {
//                if (piminus[i]->getRegion() == CD) {
//                    hChi2_piminus_MicroBooNE_BC_CD->Fill(piminus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piminus_1e_peak_MicroBooNE_CD - piminus[i]->par()->getChi2Pid()) <= Chi2_piminus_cut_MicroBooNE_CD) {
//                        hChi2_piminus_MicroBooNE_AC_CD->Fill(piminus[i]->par()->getChi2Pid());
//                    }
//                } else if (piminus[i]->getRegion() == FD) {
//                    hChi2_piminus_MicroBooNE_BC_FD->Fill(piminus[i]->par()->getChi2Pid());
//
//                    if (fabs(Chi2_piminus_1e_peak_MicroBooNE_FD - piminus[i]->par()->getChi2Pid()) <= Chi2_piminus_cut_MicroBooNE_FD) {
//                        hChi2_piminus_MicroBooNE_AC_FD->Fill(piminus[i]->par()->getChi2Pid());
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
//            double dVz0_MicroBooNE = Vz_p0_MicroBooNE - Vz_e_MicroBooNE, dVz1_MicroBooNE = Vz_p1_MicroBooNE - Vz_e_MicroBooNE;
//            double Vx_p_MicroBooNE, Vy_p_MicroBooNE, Vz_p_MicroBooNE, dVx_MicroBooNE, dVy_MicroBooNE, dVz_MicroBooNE;
//
//            for (auto &p: protons) {
//                Vx_p_MicroBooNE = p->par()->getVx(), Vy_p_MicroBooNE = p->par()->getVy(), Vz_p_MicroBooNE = p->par()->getVz();
//                dVx_MicroBooNE = Vx_p_MicroBooNE - Vx_e_MicroBooNE;
//                dVy_MicroBooNE = Vy_p_MicroBooNE - Vy_e_MicroBooNE;
//                dVz_MicroBooNE = Vz_p_MicroBooNE - Vz_e_MicroBooNE;
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
//            if ((e_mom_cuts_MicroBooNE.at(1) == -1) && (e_mom_cuts_MicroBooNE.at(0) == -1)) {
//                e_mom_ucut_MicroBooNE = e_mom_lcut_MicroBooNE = true;
//            } else if ((e_mom_cuts_MicroBooNE.at(1) != -1) && (e_mom_cuts_MicroBooNE.at(0) == -1)) {
//                e_mom_ucut_MicroBooNE = (P_e_MicroBooNE.Mag() <= e_mom_cuts_MicroBooNE.at(1));
//                e_mom_lcut_MicroBooNE = true;
//            } else if ((e_mom_cuts_MicroBooNE.at(1) == -1) && (e_mom_cuts_MicroBooNE.at(0) != -1)) {
//                e_mom_ucut_MicroBooNE = true;
//                e_mom_lcut_MicroBooNE = (P_e_MicroBooNE.Mag() >= e_mom_cuts_MicroBooNE.at(0));
//            } else if ((e_mom_cuts_MicroBooNE.at(1) != -1) && (e_mom_cuts_MicroBooNE.at(0) != -1)) {
//                e_mom_ucut_MicroBooNE = (P_e_MicroBooNE.Mag() <= e_mom_cuts_MicroBooNE.at(1));
//                e_mom_lcut_MicroBooNE = (P_e_MicroBooNE.Mag() >= e_mom_cuts_MicroBooNE.at(0));
//            }
//
//            bool e_mom_cut_MicroBooNE = (e_mom_ucut_MicroBooNE && e_mom_lcut_MicroBooNE);
//
//            // Proton 0:
//            bool p0_mom_ucut_MicroBooNE, p0_mom_lcut_MicroBooNE;
//
//            if ((p_mom_cuts_MicroBooNE.at(1) == -1) && (p_mom_cuts_MicroBooNE.at(0) == -1)) {
//                p0_mom_ucut_MicroBooNE = p0_mom_lcut_MicroBooNE = true;
//            } else if ((p_mom_cuts_MicroBooNE.at(1) != -1) && (p_mom_cuts_MicroBooNE.at(0) == -1)) {
//                p0_mom_ucut_MicroBooNE = (P_p0_MicroBooNE.Mag() <= p_mom_cuts_MicroBooNE.at(1));
//                p0_mom_lcut_MicroBooNE = true;
//            } else if ((p_mom_cuts_MicroBooNE.at(1) == -1) && (p_mom_cuts_MicroBooNE.at(0) != -1)) {
//                p0_mom_ucut_MicroBooNE = true;
//                p0_mom_lcut_MicroBooNE = (P_p0_MicroBooNE.Mag() >= p_mom_cuts_MicroBooNE.at(0));
//            } else if ((p_mom_cuts_MicroBooNE.at(1) != -1) && (p_mom_cuts_MicroBooNE.at(0) != -1)) {
//                p0_mom_ucut_MicroBooNE = (P_p0_MicroBooNE.Mag() <= p_mom_cuts_MicroBooNE.at(1));
//                p0_mom_lcut_MicroBooNE = (P_p0_MicroBooNE.Mag() >= p_mom_cuts_MicroBooNE.at(0));
//            }
//
//            bool p0_mom_cut_MicroBooNE = (p0_mom_ucut_MicroBooNE && p0_mom_lcut_MicroBooNE);
//
//            // Proton 1:
//            bool p1_mom_ucut_MicroBooNE, p1_mom_lcut_MicroBooNE;
//
//            if ((p_mom_cuts_MicroBooNE.at(1) == -1) && (p_mom_cuts_MicroBooNE.at(0) == -1)) {
//                p1_mom_ucut_MicroBooNE = p1_mom_lcut_MicroBooNE = true;
//            } else if ((p_mom_cuts_MicroBooNE.at(1) != -1) && (p_mom_cuts_MicroBooNE.at(0) == -1)) {
//                p1_mom_ucut_MicroBooNE = (P_p1_MicroBooNE.Mag() <= p_mom_cuts_MicroBooNE.at(1));
//                p1_mom_lcut_MicroBooNE = true;
//            } else if ((p_mom_cuts_MicroBooNE.at(1) == -1) && (p_mom_cuts_MicroBooNE.at(0) != -1)) {
//                p1_mom_ucut_MicroBooNE = true;
//                p1_mom_lcut_MicroBooNE = (P_p1_MicroBooNE.Mag() >= p_mom_cuts_MicroBooNE.at(0));
//            } else if ((p_mom_cuts_MicroBooNE.at(1) != -1) && (p_mom_cuts_MicroBooNE.at(0) != -1)) {
//                p1_mom_ucut_MicroBooNE = (P_p1_MicroBooNE.Mag() <= p_mom_cuts_MicroBooNE.at(1));
//                p1_mom_lcut_MicroBooNE = (P_p1_MicroBooNE.Mag() >= p_mom_cuts_MicroBooNE.at(0));
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
//                    if ((cpion_mom_cuts_MicroBooNE(1) == -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        pcpion_mom_ucut_MicroBooNE = pcpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_mom_cuts_MicroBooNE(1) != -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        pcpion_mom_ucut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() <= cpion_mom_cuts_MicroBooNE(1));
//                        pcpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_mom_cuts_MicroBooNE(1) == -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        pcpion_mom_ucut_MicroBooNE = true;
//                        pcpion_mom_lcut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() >= cpion_momentum_lower_cut_MicroBooNE);
//                    } else if ((cpion_mom_cuts_MicroBooNE(1) != -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        pcpion_mom_ucut_MicroBooNE = (P_pcpion_MicroBooNE.Mag() <= cpion_mom_cuts_MicroBooNE(1));
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
//                    if ((cpion_mom_cuts_MicroBooNE(1) == -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        ncpion_mom_ucut_MicroBooNE = ncpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_mom_cuts_MicroBooNE(1) != -1) && (cpion_momentum_lower_cut_MicroBooNE == -1)) {
//                        ncpion_mom_ucut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() <= cpion_mom_cuts_MicroBooNE(1));
//                        ncpion_mom_lcut_MicroBooNE = true;
//                    } else if ((cpion_mom_cuts_MicroBooNE(1) == -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        ncpion_mom_ucut_MicroBooNE = true;
//                        ncpion_mom_lcut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() >= cpion_momentum_lower_cut_MicroBooNE);
//                    } else if ((cpion_mom_cuts_MicroBooNE(1) != -1) && (cpion_momentum_lower_cut_MicroBooNE != -1)) {
//                        ncpion_mom_ucut_MicroBooNE = (P_ncpion_MicroBooNE.Mag() <= cpion_mom_cuts_MicroBooNE(1));
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
//                    hChi2_Electron_MicroBooNE_CD->Fill(electrons[0]->par()->getChi2Pid());
//                } else if (electrons[0]->getRegion() == FD) {
//                    hChi2_Electron_MicroBooNE_FD->Fill(electrons[0]->par()->getChi2Pid());
//                }
//
//                if (protons[0]->getRegion() == CD) {
//                    hChi2_Proton_MicroBooNE_CD->Fill(protons[0]->par()->getChi2Pid());
//                } else if (protons[0]->getRegion() == FD) {
//                    hChi2_Proton_MicroBooNE_FD->Fill(protons[0]->par()->getChi2Pid());
//                }
//
//                if (protons[1]->getRegion() == CD) {
//                    hChi2_Proton_MicroBooNE_CD->Fill(protons[1]->par()->getChi2Pid());
//                } else if (protons[1]->getRegion() == FD) {
//                    hChi2_Proton_MicroBooNE_FD->Fill(protons[1]->par()->getChi2Pid());
//                }
//
//                for (int i = 0; i < piplus.size(); i++) {
//                    if (piplus[i]->getRegion() == CD) {
//                        hChi2_piplus_MicroBooNE_CD->Fill(piplus[i]->par()->getChi2Pid());
//                    } else if (piplus[i]->getRegion() == FD) {
//                        hChi2_piplus_MicroBooNE_FD->Fill(piplus[i]->par()->getChi2Pid());
//                    }
//                } // end of loop over piplus vector
//
//                for (int i = 0; i < piminus.size(); i++) {
//                    if (piminus[i]->getRegion() == CD) {
//                        hChi2_piminus_MicroBooNE_CD->Fill(piminus[i]->par()->getChi2Pid());
//                    } else if (piminus[i]->getRegion() == FD) {
//                        hChi2_piminus_MicroBooNE_FD->Fill(piminus[i]->par()->getChi2Pid());
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
        if ((calculate_2p == true) && ((Nf == 3) && (Np == 2))) { // for 2p calculations
            ++num_of_events_with_1e2p; // logging #(events) w/ 1e2p

            /* NOTE: p0 corresponds to protons[0] & p1 corresponds to protons[1] */
            TVector3 P_p0, P_p1;
            P_p0.SetMagThetaPhi(protons[0]->getP(), protons[0]->getTheta(), protons[0]->getPhi());
            P_p1.SetMagThetaPhi(protons[1]->getP(), protons[1]->getTheta(), protons[1]->getPhi());

            //  Testing cuts ----------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing cuts">

            //  Testing momentum cuts (protons only) --------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing momentum cuts (protons only)">
            /* momentum before and after cuts */
            if (electrons[0]->getRegion() == CD) {
//                hP_e_1e2p_BC_CD->Fill(P_e); // momentum before cuts
//                LogEventCuts(hP_e_1e2p_AC_CD, electrons[0], e_mom_cuts_2p.at(0), e_mom_cuts_2p.at(1)); // momentum after cuts
            } else if (electrons[0]->getRegion() == FD) {
                hP_e_1e2p_BC_FD->Fill(P_e); // momentum before cuts
                LogEventCuts(hP_e_1e2p_AC_FD, electrons[0], e_mom_cuts_2p.at(0), e_mom_cuts_2p.at(1)); // momentum after cuts
            }

            if (protons[0]->getRegion() == CD) {
                hP_p_1e2p_BC_CD->Fill(P_p0.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_CD, protons[0], p_mom_cuts_2p.at(0), p_mom_cuts_2p.at(1)); // momentum after cuts
            } else if (protons[0]->getRegion() == FD) {
                hP_p_1e2p_BC_FD->Fill(P_p0.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_FD, protons[0], p_mom_cuts_2p.at(0), p_mom_cuts_2p.at(1)); // momentum after cuts
            }

            if (protons[1]->getRegion() == CD) {
                hP_p_1e2p_BC_CD->Fill(P_p1.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_CD, protons[1], p_mom_cuts_2p.at(0), p_mom_cuts_2p.at(1)); // momentum after cuts
            } else if (protons[1]->getRegion() == FD) {
                hP_p_1e2p_BC_FD->Fill(P_p1.Mag()); // momentum before cuts
                LogEventCuts(hP_p_1e2p_AC_FD, protons[1], p_mom_cuts_2p.at(0), p_mom_cuts_2p.at(1)); // momentum after cuts
            }
            //</editor-fold>

            //  Testing chi2 cuts (protons only) ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing chi2 cuts (protons only)">
            if (apply_cuts == false) {
                /* Chi2 before cut */
                if (electrons[0]->getRegion() == CD) {
//                    hChi2_Electron_1e2p_BC_CD->Fill(electrons[0]->par()->getChi2Pid());
                } else if (electrons[0]->getRegion() == FD) {
                    hChi2_Electron_1e2p_BC_FD->Fill(electrons[0]->par()->getChi2Pid());
                }

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
                if ((electrons[0]->getRegion() == CD) && (fabs(Chi2_Electron_1e_peak_CD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_CD)) {
                    ++num_of_events_1e2p_w_eChi2_cut_only_CD;
//                    hChi2_Electron_1e2p_AC_CD->Fill(electrons[0]->par()->getChi2Pid());
                } else if ((electrons[0]->getRegion() == FD) && (fabs(Chi2_Electron_1e_peak_FD - electrons[0]->par()->getChi2Pid()) <= Chi2_Electron_cut_FD)) {
                    ++num_of_events_1e2p_w_eChi2_cut_only_FD;
                    hChi2_Electron_1e2p_AC_FD->Fill(electrons[0]->par()->getChi2Pid());
                }

                if ((protons[0]->getRegion() == CD) && (fabs(Chi2_Proton_1e_peak_CD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD)) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                    hChi2_Proton_1e2p_AC_CD->Fill(protons[0]->par()->getChi2Pid());
                } else if ((protons[0]->getRegion() == FD) && (fabs(Chi2_Proton_1e_peak_FD - protons[0]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD)) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                    hChi2_Proton_1e2p_AC_FD->Fill(protons[0]->par()->getChi2Pid());
                }

                if ((protons[1]->getRegion() == CD) && (fabs(Chi2_Proton_1e_peak_CD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_CD)) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_CD;
                    hChi2_Proton_1e2p_AC_CD->Fill(protons[1]->par()->getChi2Pid());
                } else if ((protons[1]->getRegion() == FD) && (fabs(Chi2_Proton_1e_peak_FD - protons[1]->par()->getChi2Pid()) <= Chi2_Proton_cut_FD)) {
                    ++num_of_events_1e2p_w_pChi2_cut_only_FD;
                    hChi2_Proton_1e2p_AC_FD->Fill(protons[1]->par()->getChi2Pid());
                }
            } else {
                if (electrons[0]->getRegion() == CD) {
//                    hChi2_Electron_1e2p_BC_CD->Fill(electrons[0]->par()->getChi2Pid());
                } else if (electrons[0]->getRegion() == FD) {
                    hChi2_Electron_1e2p_BC_FD->Fill(electrons[0]->par()->getChi2Pid());
                }

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

            //  Testing dVz cuts ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Testing dV cuts">
            double Vx_e = electrons[0]->par()->getVx(), Vy_e = electrons[0]->par()->getVy(), Vz_e = electrons[0]->par()->getVz();
            double Vz_p0 = protons[0]->par()->getVz(), Vz_p1 = protons[1]->par()->getVz(), dVz0 = Vz_p0 - Vz_e, dVz1 = Vz_p1 - Vz_e;
            double Vx_p, Vy_p, Vz_p, dVx, dVy, dVz;

            for (auto &p: protons) {
                Vx_p = p->par()->getVx(), Vy_p = p->par()->getVy(), Vz_p = p->par()->getVz();
                dVx = Vx_p - Vx_e, dVy = Vy_p - Vy_e, dVz = Vz_p - Vz_e;

                if (apply_cuts == false) {
                    /* Testing dVx,dVy,dVz before cuts */
                    deltaVx_before_dV_cuts_1e2p->Fill(dVx), deltaVy_before_dV_cuts_1e2p->Fill(dVy), deltaVz_before_dV_cuts_1e2p->Fill(dVz);

                    /* Testing dVx,dVy,dVz after cuts */
                    if (fabs(dVx_peak - dVx) <= dVx_cut) { deltaVx_after_dV_cuts_1e2p->Fill(dVx); }
                    if (fabs(dVy_peak - dVy) <= dVy_cut) { deltaVy_after_dV_cuts_1e2p->Fill(dVy); }
                    if (fabs(dVz_peak - dVz) <= dVz_cut) { deltaVz_after_dV_cuts_1e2p->Fill(dVz); }
                } else {
                    deltaVx_before_dV_cuts_1e2p->Fill(dVx), deltaVy_before_dV_cuts_1e2p->Fill(dVy), deltaVz_before_dV_cuts_1e2p->Fill(dVz);
                }
            } // end of loop over protons vector
            //</editor-fold>

            //</editor-fold>

            //  Applying cuts ---------------------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying cuts">

            //  Applying momentum cuts ------------------------------------------------------------------------------------------------------------------------

            //<editor-fold desc="Applying momentum cuts">
            // Electrons:
            if ((apply_momentum_cuts_2p == true) && ((e_mom_cuts_2p.at(1) != -1) && (P_e > e_mom_cuts_2p.at(1)))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((e_mom_cuts_2p.at(0) != -1) && (P_e < e_mom_cuts_2p.at(0)))) { continue; }

            // Proton 0:
            if ((apply_momentum_cuts_2p == true) && ((p_mom_cuts_2p.at(1) != -1) && (P_p0.Mag() > p_mom_cuts_2p.at(1)))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((p_mom_cuts_2p.at(0) != -1) && (P_p0.Mag() < p_mom_cuts_2p.at(0)))) { continue; }

            // Proton 1:
            if ((apply_momentum_cuts_2p == true) && ((p_mom_cuts_2p.at(1) != -1) && (P_p1.Mag() > p_mom_cuts_2p.at(1)))) { continue; }
            if ((apply_momentum_cuts_2p == true) && ((p_mom_cuts_2p.at(0) != -1) && (P_p1.Mag() < p_mom_cuts_2p.at(0)))) { continue; }
            //</editor-fold>

            //  Applying chi2 cuts ------------------------------------------------------------------------------------------------------------------------

//            //<editor-fold desc="Applying Chi2 cuts (protons only)">
//            // TODO: move proton blocks here to for loop to save some space
//            if (protons[0]->getRegion() == CD) {
//                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_CD - protons[0]->par()->getChi2Pid()) > Chi2_Proton_cut_CD)) { continue; }
//            } else if (protons[0]->getRegion() == FD) {
//                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_FD - protons[0]->par()->getChi2Pid()) > Chi2_Proton_cut_FD)) { continue; }
//            }
//
//            if (protons[1]->getRegion() == CD) {
//                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_CD - protons[1]->par()->getChi2Pid()) > Chi2_Proton_cut_CD)) { continue; }
//            } else if (protons[1]->getRegion() == FD) {
//                if ((apply_chi2_cuts_2p == true) && (fabs(Chi2_Proton_1e_peak_FD - protons[1]->par()->getChi2Pid()) > Chi2_Proton_cut_FD)) { continue; }
//            }
//
//            ++num_of_events_1e2p_w_allChi2_cuts;
//            //</editor-fold>

            //  Applying dVz cuts ------------------------------------------------------------------------------------------------------------------------

//            //<editor-fold desc="Applying dVz cuts">
//            if ((apply_dVz_cuts == true) && ((fabs(dVz_peak - dVz0) > dVz_cut) || (fabs(dVz_peak - dVz1) > dVz_cut))) { continue; }
//            //</editor-fold>

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

            //<editor-fold desc="Filling Chi2 histograms (2p)">

            //<editor-fold desc="Filling electron Chi2 cut (2p)">
            if (electrons[0]->getRegion() == CD) {
//                hChi2_Electron_2p_CD->Fill(electrons[0]->par()->getChi2Pid());
            } else if (electrons[0]->getRegion() == FD) {
                hChi2_Electron_2p_FD->Fill(electrons[0]->par()->getChi2Pid());
            }
            //</editor-fold>

            //<editor-fold desc="Filling proton0 Chi2 cut (2p)">
            if (protons[0]->getRegion() == CD) {
                hChi2_Proton_2p_CD->Fill(protons[0]->par()->getChi2Pid());
            } else if (protons[0]->getRegion() == FD) {
                hChi2_Proton_2p_FD->Fill(protons[0]->par()->getChi2Pid());
            }
            //</editor-fold>

            //<editor-fold desc="Filling proton1 Chi2 cut (2p)">
            if (protons[1]->getRegion() == CD) {
                hChi2_Proton_2p_CD->Fill(protons[1]->par()->getChi2Pid());
            } else if (protons[1]->getRegion() == FD) {
                hChi2_Proton_2p_FD->Fill(protons[1]->par()->getChi2Pid());
            }
            //</editor-fold>

            //</editor-fold>

            //<editor-fold desc="Filling dVx, dVy, dVz (2p)">
            for (auto &p: protons) {
                Vx_p = p->par()->getVx(), Vy_p = p->par()->getVy(), Vz_p = p->par()->getVz();
                dVx = Vx_p - Vx_e, dVy = Vy_p - Vy_e, dVz = Vz_p - Vz_e;

                deltaVx_2p->Fill(dVx), deltaVy_2p->Fill(dVy), deltaVz_2p->Fill(dVz);
            } // end of loop over protons vector
            //</editor-fold>

            //<editor-fold desc="Filling fiducial plots (2p)">
            Vcal_VS_EoP_2p_PCAL->Fill(electrons[0]->cal(PCAL)->getLv(), EoP_e);
            Wcal_VS_EoP_2p_PCAL->Fill(electrons[0]->cal(PCAL)->getLw(), EoP_e);
            //</editor-fold>

            //<editor-fold desc="Filling SF histograms (2p)">
            hSF_2p_FD->Fill(EoP_e);
            hSF_VS_P_e_2p_FD->Fill(P_e, EoP_e);
            //</editor-fold>

            //<editor-fold desc="Filling Nphe plots (2p)">
            hNphe_2p_FD->Fill(Nphe);
            //</editor-fold>

            //<editor-fold desc="Filling momentum histograms">
            if (electrons[0]->getRegion() == CD) {
//                hP_e_2p_CD->Fill(P_e);
            } else if (electrons[0]->getRegion() == FD) {
                hP_e_2p_FD->Fill(P_e);
            }

            if (protons[0]->getRegion() == CD) {
                hP_p_2p_CD->Fill(P_p0.Mag());
            } else if (protons[0]->getRegion() == FD) {
                hP_p_2p_FD->Fill(P_p0.Mag());
            }

            if (protons[1]->getRegion() == CD) {
                hP_p_2p_CD->Fill(P_p1.Mag());
            } else if (protons[1]->getRegion() == FD) {
                hP_p_2p_FD->Fill(P_p1.Mag());
            }
            //</editor-fold>

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
//                    if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_All_Int_2p_CD->Fill(beamE - E_e); }
//
//                    if (qel) {
//                        hTheta_e_QEL_2p_CD->Fill(Theta_e);
//                        hPhi_e_QEL_2p_CD->Fill(Phi_e);
//                        hE_e_QEL_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_QEL_2p_CD->Fill(Theta_e, E_e);
//
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_QEL_2p_CD->Fill(beamE - E_e); }
//                    } else if (mec) {
//                        hTheta_e_MEC_2p_CD->Fill(Theta_e);
//                        hPhi_e_MEC_2p_CD->Fill(Phi_e);
//                        hE_e_MEC_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_MEC_2p_CD->Fill(Theta_e, E_e);
//
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_MEC_2p_CD->Fill(beamE - E_e); }
//                    } else if (res) {
//                        hTheta_e_RES_2p_CD->Fill(Theta_e);
//                        hPhi_e_RES_2p_CD->Fill(Phi_e);
//                        hE_e_RES_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_RES_2p_CD->Fill(Theta_e, E_e);
//
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_RES_2p_CD->Fill(beamE - E_e); }
//                    } else if (dis) {
//                        hTheta_e_DIS_2p_CD->Fill(Theta_e);
//                        hPhi_e_DIS_2p_CD->Fill(Phi_e);
//                        hE_e_DIS_2p_CD->Fill(E_e);
//                        hE_e_VS_Theta_e_DIS_2p_CD->Fill(Theta_e, E_e);
//
//                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_DIS_2p_CD->Fill(beamE - E_e); }
//                    }
                } else if (e->getRegion() == FD) {
                    hTheta_e_All_Int_2p_FD->Fill(Theta_e);
                    hPhi_e_All_Int_2p_FD->Fill(Phi_e);
                    hTheta_e_VS_Phi_e_2p_FD->Fill(Phi_e, Theta_e);

                    hE_e_All_Int_2p_FD->Fill(E_e);
                    hE_e_VS_Theta_e_All_Int_2p_FD->Fill(Theta_e, E_e);

                    if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_All_Int_2p_FD->Fill(beamE - E_e); }

                    if (qel) {
                        hTheta_e_QEL_2p_FD->Fill(Theta_e);
                        hPhi_e_QEL_2p_FD->Fill(Phi_e);
                        hE_e_QEL_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_QEL_2p_FD->Fill(Theta_e, E_e);

                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_QEL_2p_FD->Fill(beamE - E_e); }
                    } else if (mec) {
                        hTheta_e_MEC_2p_FD->Fill(Theta_e);
                        hPhi_e_MEC_2p_FD->Fill(Phi_e);
                        hE_e_MEC_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_MEC_2p_FD->Fill(Theta_e, E_e);

                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_MEC_2p_FD->Fill(beamE - E_e); }
                    } else if (res) {
                        hTheta_e_RES_2p_FD->Fill(Theta_e);
                        hPhi_e_RES_2p_FD->Fill(Phi_e);
                        hE_e_RES_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_RES_2p_FD->Fill(Theta_e, E_e);

                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_RES_2p_FD->Fill(beamE - E_e); }
                    } else if (dis) {
                        hTheta_e_DIS_2p_FD->Fill(Theta_e);
                        hPhi_e_DIS_2p_FD->Fill(Phi_e);
                        hE_e_DIS_2p_FD->Fill(E_e);
                        hE_e_VS_Theta_e_DIS_2p_FD->Fill(Theta_e, E_e);

                        if ((Theta_e >= 14.0) && (Theta_e <= 16.0)) { hET15_DIS_2p_FD->Fill(beamE - E_e); }
                    }
                }
            } // end of loop over electrons vector

            //<editor-fold desc="Filling Q2 histograms (2p)">
            if (electrons[0]->getRegion() == CD) {
                hQ2_2p_CD->Fill(Q2);
            } else if (electrons[0]->getRegion() == FD) {
                hQ2_2p_FD->Fill(Q2);
            }
            //</editor-fold>

            //<editor-fold desc="Filling Ecal histograms (2p)">
            double Ecal_2p, P_Leading, E_Leading, P_Recoil, E_Recoil;

            if (P_p0.Mag() >= P_p1.Mag()) {
                P_Leading = P_p0.Mag();
                E_Leading = sqrt(m_p * m_p + P_Leading * P_Leading);
                P_Recoil = P_p1.Mag();
                E_Recoil = sqrt(m_p * m_p + P_Recoil * P_Recoil);
            } else {
                P_Leading = P_p1.Mag();
                E_Leading = sqrt(m_p * m_p + P_Leading * P_Leading);
                P_Recoil = P_p0.Mag();
                E_Recoil = sqrt(m_p * m_p + P_Recoil * P_Recoil);
            }

            Ecal_2p = E_e + (E_Leading - m_p) + (E_Recoil - m_p);

            hEcal_All_Int_2p->Fill(Ecal_2p);

            if (qel) {
                hEcal_QEL_2p->Fill(Ecal_2p);
            } else if (mec) {
                hEcal_MEC_2p->Fill(Ecal_2p);
            } else if (res) {
                hEcal_RES_2p->Fill(Ecal_2p);
            } else if (dis) {
                hEcal_DIS_2p->Fill(Ecal_2p);
            }
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
// Number of Photo-electrons (Nphe) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms">
    if (Nphe_plots) {
        cout << "\n\nPlotting number of photo-electrons (Nphe) histograms...\n\n";

        //<editor-fold desc="Number of Photo-electrons (Nphe) histogram (1e cut, FD)">
        if (apply_cuts == false) {
            histPlotter1D(c1, hNphe_1e_cut_BC_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC (N_{phe}) Before Cuts", "1e Cut", plots, 2, false, true,
                          sNphe_1e_cut_FD, "01_Nphe_1e_cut_before_Nphe_cuts", hNphe_1e_cut_BC_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0,
                          false);

            histPlotter1D(c1, hNphe_1e_cut_AC_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC (N_{phe}) After Cuts", "1e Cut", plots, 2, false, true,
                          sNphe_1e_cut_FD, "02_Nphe_1e_cut_after_Nphe_cuts", hNphe_1e_cut_AC_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0,
                          false);
        } else {
            histPlotter1D(c1, hNphe_1e_cut_BC_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC (N_{phe})", "1e Cut", plots, 2, false, true, sNphe_1e_cut_FD,
                          "01_Nphe_1e_cut", hNphe_1e_cut_BC_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0, false);
        }
        //</editor-fold>

        //<editor-fold desc="Number of Photo-electrons (Nphe) histogram (2p, FD)">
        histPlotter1D(c1, hNphe_2p_FD, norm_Nphe_plots, true, 1., "#Photo-electrons in HTCC (N_{phe})", "2p", plots, 2, false, true, sNphe_2p_FD,
                      "01_Nphe_2p", hNphe_2p_FD_Dir, "FD", kBlue, true, true, false, true, clasAna.getNpheCuts(), 0, false);
        //</editor-fold>

    } else {
        cout << "\n\nnumber of photo-electrons (Nphe) plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Chi2 plots
// ======================================================================================================================================================================

    //<editor-fold desc="Chi2 plots">
    double Chi2_Kplus_Xmax_CD, Chi2_Kplus_Xmax_FD, Chi2_Kminus_Xmax_CD, Chi2_Kminus_Xmax_FD;
    double Chi2_piplus_Xmax_CD, Chi2_piplus_Xmax_FD, Chi2_piminus_Xmax_CD, Chi2_piminus_Xmax_FD;
    double Chi2_Kplus_1e_Xmax_CD, Chi2_Kplus_1e_Xmax_FD, Chi2_Kminus_1e_Xmax_CD, Chi2_Kminus_1e_Xmax_FD;
    double Chi2_piplus_1e_Xmax_CD, Chi2_piplus_1e_Xmax_FD, Chi2_piminus_1e_Xmax_CD, Chi2_piminus_1e_Xmax_FD;

    if (Chi2_plots) {
        cout << "\n\nPlotting Chi2 plots...\n\n";

        //<editor-fold desc="Finding Xmax">
        //todo: calculate Xmax according to BC histograms?
//        Chi2_Electron_Xmax_CD = hChi2_Electron_CD->GetBinCenter(hChi2_Electron_CD->GetMaximumBin());
        Chi2_Electron_Xmax_FD = hChi2_Electron_FD->GetBinCenter(hChi2_Electron_FD->GetMaximumBin());
        Chi2_Proton_Xmax_CD = hChi2_Proton_CD->GetBinCenter(hChi2_Proton_CD->GetMaximumBin());
        Chi2_Proton_Xmax_FD = hChi2_Proton_FD->GetBinCenter(hChi2_Proton_FD->GetMaximumBin());
        Chi2_Kplus_Xmax_CD = hChi2_Kplus_CD->GetBinCenter(hChi2_Kplus_CD->GetMaximumBin());
        Chi2_Kplus_Xmax_FD = hChi2_Kplus_FD->GetBinCenter(hChi2_Kplus_FD->GetMaximumBin());
        Chi2_Kminus_Xmax_CD = hChi2_Kminus_CD->GetBinCenter(hChi2_Kminus_CD->GetMaximumBin());
        Chi2_Kminus_Xmax_FD = hChi2_Kminus_FD->GetBinCenter(hChi2_Kminus_FD->GetMaximumBin());
        Chi2_piplus_Xmax_CD = hChi2_piplus_CD->GetBinCenter(hChi2_piplus_CD->GetMaximumBin());
        Chi2_piplus_Xmax_FD = hChi2_piplus_FD->GetBinCenter(hChi2_piplus_FD->GetMaximumBin());
        Chi2_piminus_Xmax_CD = hChi2_piminus_CD->GetBinCenter(hChi2_piminus_CD->GetMaximumBin());
        Chi2_piminus_Xmax_FD = hChi2_piminus_FD->GetBinCenter(hChi2_piminus_FD->GetMaximumBin());
//        Chi2_Electron_1e_Xmax_CD = hChi2_Electron_1e_cut_CD->GetBinCenter(hChi2_Electron_1e_cut_CD->GetMaximumBin());
        Chi2_Electron_1e_Xmax_FD = hChi2_Electron_1e_cut_FD->GetBinCenter(hChi2_Electron_1e_cut_FD->GetMaximumBin());
        Chi2_Proton_1e_Xmax_CD = hChi2_Proton_1e_cut_CD->GetBinCenter(hChi2_Proton_1e_cut_CD->GetMaximumBin());
        Chi2_Proton_1e_Xmax_FD = hChi2_Proton_1e_cut_FD->GetBinCenter(hChi2_Proton_1e_cut_FD->GetMaximumBin());
        Chi2_Kplus_1e_Xmax_CD = hChi2_Kplus_1e_cut_CD->GetBinCenter(hChi2_Kplus_1e_cut_CD->GetMaximumBin());
        Chi2_Kplus_1e_Xmax_FD = hChi2_Kplus_1e_cut_FD->GetBinCenter(hChi2_Kplus_1e_cut_FD->GetMaximumBin());
        Chi2_Kminus_1e_Xmax_CD = hChi2_Kminus_1e_cut_CD->GetBinCenter(hChi2_Kminus_1e_cut_CD->GetMaximumBin());
        Chi2_Kminus_1e_Xmax_FD = hChi2_Kminus_1e_cut_FD->GetBinCenter(hChi2_Kminus_1e_cut_FD->GetMaximumBin());
        Chi2_piplus_1e_Xmax_CD = hChi2_piplus_1e_cut_CD->GetBinCenter(hChi2_piplus_1e_cut_CD->GetMaximumBin());
        Chi2_piplus_1e_Xmax_FD = hChi2_piplus_1e_cut_FD->GetBinCenter(hChi2_piplus_1e_cut_FD->GetMaximumBin());
        Chi2_piminus_1e_Xmax_CD = hChi2_piminus_1e_cut_CD->GetBinCenter(hChi2_piminus_1e_cut_CD->GetMaximumBin());
        Chi2_piminus_1e_Xmax_FD = hChi2_piminus_1e_cut_FD->GetBinCenter(hChi2_piminus_1e_cut_FD->GetMaximumBin());

//        Chi2_Electron_MicroBooNE_Xmax_CD = hChi2_Electron_MicroBooNE_CD->GetBinCenter(hChi2_Electron_MicroBooNE_CD->GetMaximumBin());
        Chi2_Electron_MicroBooNE_Xmax_FD = hChi2_Electron_MicroBooNE_FD->GetBinCenter(hChi2_Electron_MicroBooNE_FD->GetMaximumBin());
        Chi2_Proton_MicroBooNE_Xmax_CD = hChi2_Proton_MicroBooNE_CD->GetBinCenter(hChi2_Proton_MicroBooNE_CD->GetMaximumBin());
        Chi2_Proton_MicroBooNE_Xmax_FD = hChi2_Proton_MicroBooNE_FD->GetBinCenter(hChi2_Proton_MicroBooNE_FD->GetMaximumBin());
        Chi2_piplus_MicroBooNE_Xmax_CD = hChi2_piplus_MicroBooNE_CD->GetBinCenter(hChi2_piplus_MicroBooNE_CD->GetMaximumBin());
        Chi2_piplus_MicroBooNE_Xmax_FD = hChi2_piplus_MicroBooNE_FD->GetBinCenter(hChi2_piplus_MicroBooNE_FD->GetMaximumBin());
        Chi2_piminus_MicroBooNE_Xmax_CD = hChi2_piminus_MicroBooNE_CD->GetBinCenter(hChi2_piminus_MicroBooNE_CD->GetMaximumBin());
        Chi2_piminus_MicroBooNE_Xmax_FD = hChi2_piminus_MicroBooNE_FD->GetBinCenter(hChi2_piminus_MicroBooNE_FD->GetMaximumBin());
        //</editor-fold>

//  Chi2 plots (no #(e) cut) ----------------------------------------------------------------------------

        //<editor-fold desc="Chi2 plots (no #(e) cut)">
//        histPlotter1D(c1, hChi2_Electron_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Electron_All_e, "01_Electron_chi2", hChi2_Electron_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Electron_cut_CD,
//                      Chi2_Electron_Xmax_CD);
        histPlotter1D(c1, hChi2_Electron_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Electron_All_e, "01_Electron_chi2", hChi2_Electron_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Electron_cut_FD,
                      Chi2_Electron_Xmax_FD);

        histPlotter1D(c1, hChi2_Proton_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_All_e, "02_Proton_chi2", hChi2_Proton_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cut_CD, Chi2_Proton_Xmax_CD);
        histPlotter1D(c1, hChi2_Proton_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_All_e, "02_Proton_chi2", hChi2_Proton_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Proton_cut_FD, Chi2_Proton_Xmax_FD);

        histPlotter1D(c1, hChi2_Kplus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kplus_All_e, "03_Kplus_chi2", hChi2_Kplus_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hChi2_Kplus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kplus_All_e, "03_Kplus_chi2", hChi2_Kplus_FD_Dir, "FD", kRed, true, true, true, false);

        histPlotter1D(c1, hChi2_Kminus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kminus_All_e, "04_Kminus_chi2", hChi2_Kminus_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hChi2_Kminus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kminus_All_e, "04_Kminus_chi2", hChi2_Kminus_FD_Dir, "FD", kRed, true, true, true, false);

        histPlotter1D(c1, hChi2_piplus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piplus_All_e, "03_piplus_chi2", hChi2_piplus_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hChi2_piplus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piplus_All_e, "03_piplus_chi2", hChi2_piplus_FD_Dir, "FD", kRed, true, true, true, false);

        histPlotter1D(c1, hChi2_piminus_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piminus_All_e, "04_piminus_chi2", hChi2_piminus_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, hChi2_piminus_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piminus_All_e, "04_piminus_chi2", hChi2_piminus_FD_Dir, "FD", kRed, true, true, true, false);
        //</editor-fold>

//  Chi2 plots (1e cut) ----------------------------------------------------------------------------

        //<editor-fold desc="Electron chi2 (1e cut)">
//        histPlotter1D(c1, hChi2_Electron_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Electron_1e_cut, "01_Electron_chi2", hChi2_Electron_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Electron_cut_CD,
//                      Chi2_Electron_1e_Xmax_CD);
        histPlotter1D(c1, hChi2_Electron_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Electron_1e_cut, "01_Electron_chi2", hChi2_Electron_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Electron_cut_FD,
                      Chi2_Electron_1e_Xmax_FD);

        histPlotter1D(c1, hChi2_Proton_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_1e_cut, "02_Proton_chi2", hChi2_Proton_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_1e_Xmax_CD);
        histPlotter1D(c1, hChi2_Proton_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_1e_cut, "02_Proton_chi2", hChi2_Proton_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, Chi2_Proton_cut_FD,
                      Chi2_Proton_1e_Xmax_FD);

        histPlotter1D(c1, hChi2_Kplus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kplus_1e_cut, "03_Kplus_chi2", hChi2_Kplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, 6, Chi2_Kplus_1e_Xmax_CD);
        histPlotter1D(c1, hChi2_Kplus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kplus_1e_cut, "03_Kplus_chi2", hChi2_Kplus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, 6, Chi2_Kplus_1e_Xmax_FD);

        histPlotter1D(c1, hChi2_Kminus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kminus_1e_cut, "04_Kminus_chi2", hChi2_Kminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, 6, Chi2_Kminus_1e_Xmax_CD);
        histPlotter1D(c1, hChi2_Kminus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{K^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Kminus_1e_cut, "04_Kminus_chi2", hChi2_Kminus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, 6, Chi2_Kminus_1e_Xmax_FD);

        histPlotter1D(c1, hChi2_piplus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piplus_1e_cut, "05_piplus_chi2", hChi2_piplus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, 6, Chi2_piplus_1e_Xmax_CD);
        histPlotter1D(c1, hChi2_piplus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piplus_1e_cut, "05_piplus_chi2", hChi2_piplus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, 6, Chi2_piplus_1e_Xmax_FD);

        histPlotter1D(c1, hChi2_piminus_1e_cut_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piminus_1e_cut, "06_piminus_chi2", hChi2_piminus_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, 6, Chi2_piminus_1e_Xmax_CD);
        histPlotter1D(c1, hChi2_piminus_1e_cut_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_piminus_1e_cut, "06_piminus_chi2", hChi2_piminus_1e_cut_FD_Dir, "FD", kRed, true, true, true, false, true, 6, Chi2_piminus_1e_Xmax_FD);
        //</editor-fold>

//        //<editor-fold desc="Chi2 plots (MicroBooNE)">
//
//        //<editor-fold desc="Testing Chi2 cuts before applying (MicroBooNE)">
//        histPlotter1D(c1, c2, hChi2_Electron_MicroBooNE_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram Before C-Cut", "MicroBooNE", "CD", "e", 0.06,
//                      0.04, 0.04, plots, 2, false, true, sChi2_Electron_MicroBooNE, "01_Electron_Chi2_MicroBooNE_BC", hChi2_Electron_MicroBooNE_BC_CD_Dir, kBlue,
//                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_CD, Chi2_Electron_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_Electron_MicroBooNE_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram Before C-Cut", "MicroBooNE", "FD", "e", 0.06,
//                      0.04, 0.04, plots, 2, false, true, sChi2_Electron_MicroBooNE, "02_Electron_Chi2_MicroBooNE_BC", hChi2_Electron_MicroBooNE_BC_FD_Dir, kBlue,
//                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_FD, Chi2_Electron_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, c2, hChi2_Proton_MicroBooNE_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram Before C-Cut", "MicroBooNE", "CD", "p", 0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_Proton_MicroBooNE, "03_Proton_Chi2_MicroBooNE_BC", hChi2_Proton_MicroBooNE_BC_CD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_Proton_cut_MicroBooNE_CD, Chi2_Proton_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_Proton_MicroBooNE_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram Before C-Cut", "MicroBooNE", "FD", "p", 0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_Proton_MicroBooNE, "04_Proton_Chi2_MicroBooNE_BC", hChi2_Proton_MicroBooNE_BC_FD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_Proton_cut_MicroBooNE_FD, Chi2_Proton_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, c2, hChi2_piplus_MicroBooNE_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram Before C-Cut", "MicroBooNE", "CD", "#pi^{+}",
//                      0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_piplus_MicroBooNE, "05_piplus_Chi2_MicroBooNE_BC", hChi2_piplus_MicroBooNE_BC_CD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_piplus_cut_MicroBooNE_CD, Chi2_piplus_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_piplus_MicroBooNE_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram Before C-Cut", "MicroBooNE", "FD", "#pi^{+}",
//                      0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_piplus_MicroBooNE, "06_piplus_Chi2_MicroBooNE_BC", hChi2_piplus_MicroBooNE_BC_FD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_piplus_cut_MicroBooNE_FD, Chi2_piplus_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, c2, hChi2_piminus_MicroBooNE_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram Before C-Cut", "MicroBooNE", "CD", "#pi^{-}",
//                      0.06,
//                      0.04, 0.04, plots, 2, false, true, sChi2_piminus_MicroBooNE, "07_piminus_Chi2_MicroBooNE_BC", hChi2_piminus_MicroBooNE_BC_CD_Dir, kBlue, true,
//                      true, true, false, true, Chi2_piminus_cut_MicroBooNE_CD, Chi2_piminus_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_piminus_MicroBooNE_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram Before C-Cut", "MicroBooNE", "FD", "#pi^{-}",
//                      0.06,
//                      0.04, 0.04, plots, 2, false, true, sChi2_piminus_MicroBooNE, "08_piminus_Chi2_MicroBooNE_BC", hChi2_piminus_MicroBooNE_BC_FD_Dir, kBlue, true,
//                      true, true, false, true, Chi2_piminus_cut_MicroBooNE_FD, Chi2_piminus_1e_peak_MicroBooNE_FD);
//        //</editor-fold>
//
//        //<editor-fold desc="Testing Chi2 cuts after applying (MicroBooNE)">
//        histPlotter1D(c1, c2, hChi2_Electron_MicroBooNE_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram After C-Cut", "MicroBooNE", "CD", "e", 0.06,
//                      0.04, 0.04, plots, 2, false, true, sChi2_Electron_MicroBooNE, "01_Electron_Chi2_MicroBooNE_AC", hChi2_Electron_MicroBooNE_AC_CD_Dir, kBlue,
//                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_CD, Chi2_Electron_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_Electron_MicroBooNE_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram After C-Cut", "MicroBooNE", "FD", "e", 0.06,
//                      0.04, 0.04, plots, 2, false, true, sChi2_Electron_MicroBooNE, "02_Electron_Chi2_MicroBooNE_AC", hChi2_Electron_MicroBooNE_AC_FD_Dir, kBlue,
//                      true, true, true, false, true, Chi2_Electron_cut_MicroBooNE_FD, Chi2_Electron_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, c2, hChi2_Proton_MicroBooNE_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram After C-Cut", "MicroBooNE", "CD", "p", 0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_Proton_MicroBooNE, "03_Proton_Chi2_MicroBooNE_AC", hChi2_Proton_MicroBooNE_AC_CD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_Proton_cut_MicroBooNE_CD, Chi2_Proton_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_Proton_MicroBooNE_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram After C-Cut", "MicroBooNE", "FD", "p", 0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_Proton_MicroBooNE, "04_Proton_Chi2_MicroBooNE_AC", hChi2_Proton_MicroBooNE_AC_FD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_Proton_cut_MicroBooNE_FD, Chi2_Proton_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, c2, hChi2_piplus_MicroBooNE_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram After C-Cut", "MicroBooNE", "CD", "#pi^{+}",
//                      0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_piplus_MicroBooNE, "05_piplus_Chi2_MicroBooNE_AC", hChi2_piplus_MicroBooNE_AC_CD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_piplus_cut_MicroBooNE_CD, Chi2_piplus_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_piplus_MicroBooNE_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram After C-Cut", "MicroBooNE", "FD", "#pi^{+}",
//                      0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_piplus_MicroBooNE, "06_piplus_Chi2_MicroBooNE_AC", hChi2_piplus_MicroBooNE_AC_FD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_piplus_cut_MicroBooNE_FD, Chi2_piplus_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, c2, hChi2_piminus_MicroBooNE_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram After C-Cut", "MicroBooNE", "CD", "#pi^{-}",
//                      0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_piminus_MicroBooNE, "07_piminus_Chi2_MicroBooNE_AC", hChi2_piminus_MicroBooNE_AC_CD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_piminus_cut_MicroBooNE_CD, Chi2_piminus_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, c2, hChi2_piminus_MicroBooNE_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram After C-Cut", "MicroBooNE", "FD", "#pi^{-}",
//                      0.06, 0.04,
//                      0.04, plots, 2, false, true, sChi2_piminus_MicroBooNE, "08_piminus_Chi2_MicroBooNE_AC", hChi2_piminus_MicroBooNE_AC_FD_Dir, kBlue, true, true,
//                      true, false, true, Chi2_piminus_cut_MicroBooNE_FD, Chi2_piminus_1e_peak_MicroBooNE_FD);
//        //</editor-fold>
//
//        //<editor-fold desc="Chi2 plots (MicroBooNE)">
//        histPlotter1D(c1, hChi2_Electron_MicroBooNE_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sChi2_Electron_MicroBooNE, "01_Electron_Chi2_MicroBooNE_CD", hChi2_Electron_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      Chi2_Electron_cut_MicroBooNE_CD, Chi2_Electron_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, hChi2_Electron_MicroBooNE_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sChi2_Electron_MicroBooNE, "01_Electron_Chi2_MicroBooNE_FD", hChi2_Electron_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
//                      Chi2_Electron_cut_MicroBooNE_FD, Chi2_Electron_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, hChi2_Proton_MicroBooNE_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Proton_MicroBooNE, "02_Proton_Chi2_MicroBooNE_CD", hChi2_Proton_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      Chi2_Proton_cut_MicroBooNE_CD, Chi2_Proton_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, hChi2_Proton_MicroBooNE_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Proton_MicroBooNE, "02_Proton_Chi2_MicroBooNE_FD", hChi2_Proton_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
//                      Chi2_Proton_cut_MicroBooNE_FD, Chi2_Proton_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, hChi2_piplus_MicroBooNE_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true,
//                      sChi2_piplus_MicroBooNE, "03_piplus_Chi2_MicroBooNE_CD", hChi2_piplus_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      Chi2_piplus_cut_MicroBooNE_CD, Chi2_piplus_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, hChi2_piplus_MicroBooNE_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{+}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true,
//                      sChi2_piplus_MicroBooNE, "03_piplus_Chi2_MicroBooNE_FD", hChi2_piplus_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
//                      Chi2_piplus_cut_MicroBooNE_FD, Chi2_piplus_1e_peak_MicroBooNE_FD);
//
//        histPlotter1D(c1, hChi2_piminus_MicroBooNE_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true,
//                      sChi2_piminus_MicroBooNE, "03_piminus_Chi2_MicroBooNE_CD", hChi2_piminus_MicroBooNE_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      Chi2_piminus_cut_MicroBooNE_CD, Chi2_piminus_1e_peak_MicroBooNE_CD);
//        histPlotter1D(c1, hChi2_piminus_MicroBooNE_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{#pi^{-}} Histogram", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true,
//                      sChi2_piminus_MicroBooNE, "03_piminus_Chi2_MicroBooNE_FD", hChi2_piminus_MicroBooNE_FD_Dir, "FD", kBlue, true, true, true, false, true,
//                      Chi2_piminus_cut_MicroBooNE_FD, Chi2_piminus_1e_peak_MicroBooNE_FD);
//        //</editor-fold>
//
//        //</editor-fold>

        //<editor-fold desc="Chi2 plots (1e2p)">
        if (apply_cuts == false) {
//            histPlotter1D(c1, c2, hChi2_Electron_1e2p_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram Before Cut", "1e2p", "CD", "e", 0.06, 0.04, 0.04,
//                          plots, 2, false, true, sChi2_Electron_1e2p, "01_Electron_Chi2_1e2p_BC", hChi2_Electron_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true,
//                          Chi2_Electron_cut_CD, Chi2_Electron_1e_peak_CD);
            histPlotter1D(c1, c2, hChi2_Electron_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram Before Cut", "1e2p", "FD", "e", 0.06, 0.04, 0.04,
                          plots, 2, false, true, sChi2_Electron_1e2p, "02_Electron_Chi2_1e2p_BC", hChi2_Electron_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Electron_cut_FD, Chi2_Electron_1e_peak_FD);

            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram Before Cut", "1e2p", "CD", "p", 0.06, 0.04, 0.04,
                          plots, 2, false, true, sChi2_Proton_1e2p, "03_Proton_Chi2_1e2p_BC", hChi2_Proton_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cut_CD, Chi2_Proton_1e_peak_CD);
            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram Before Cut", "1e2p", "FD", "p", 0.06, 0.04, 0.04,
                          plots, 2, false, true, sChi2_Proton_1e2p, "04_Proton_Chi2_1e2p_BC", hChi2_Proton_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cut_FD, Chi2_Proton_1e_peak_FD);

//            histPlotter1D(c1, c2, hChi2_Electron_1e2p_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram After Cut", "1e2p", "CD", "e", 0.06, 0.04, 0.04,
//                          plots, 2, false, true, sChi2_Electron_1e2p, "01_Electron_Chi2_1e2p_AC", hChi2_Electron_1e2p_AC_CD_Dir, kBlue, true, true, true, false, true,
//                          Chi2_Electron_cut_CD, Chi2_Electron_1e_peak_CD);
            histPlotter1D(c1, c2, hChi2_Electron_1e2p_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram After Cut", "1e2p", "FD", "e", 0.06, 0.04, 0.04,
                          plots, 2, false, true, sChi2_Electron_1e2p, "02_Electron_Chi2_1e2p_AC", hChi2_Electron_1e2p_AC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Electron_cut_FD, Chi2_Electron_1e_peak_FD);

            histPlotter1D(c1, c2, hChi2_Proton_1e2p_AC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram After Cut", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots,
                          2, false, true, sChi2_Proton_1e2p, "03_Proton_Chi2_1e2p_AC", hChi2_Proton_1e2p_AC_CD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cut_CD, Chi2_Proton_1e_peak_CD);
            histPlotter1D(c1, c2, hChi2_Proton_1e2p_AC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram After Cut", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots,
                          2, false, true, sChi2_Proton_1e2p, "04_Proton_Chi2_1e2p_AC", hChi2_Proton_1e2p_AC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Proton_cut_FD, Chi2_Proton_1e_peak_FD);
        } else {
//            histPlotter1D(c1, c2, hChi2_Electron_1e2p_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "1e2p", "CD", "e", 0.06, 0.04, 0.04, plots, 2,
//                          false, true, sChi2_Electron_1e2p, "01_Electron_Chi2_1e2p", hChi2_Electron_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true,
//                          Chi2_Electron_cut_CD, Chi2_Electron_1e_peak_CD);
            histPlotter1D(c1, c2, hChi2_Electron_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "1e2p", "FD", "e", 0.06, 0.04, 0.04, plots, 2,
                          false, true, sChi2_Electron_1e2p, "02_Electron_Chi2_1e2p", hChi2_Electron_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true,
                          Chi2_Electron_cut_FD, Chi2_Electron_1e_peak_FD);

            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e2p", "CD", "p", 0.06, 0.04, 0.04, plots, 2, false,
                          true, sChi2_Proton_1e2p, "03_Proton_Chi2_1e2p", hChi2_Proton_1e2p_BC_CD_Dir, kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                          Chi2_Proton_1e_peak_CD);
            histPlotter1D(c1, c2, hChi2_Proton_1e2p_BC_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "1e2p", "FD", "p", 0.06, 0.04, 0.04, plots, 2, false,
                          true, sChi2_Proton_1e2p, "04_Proton_Chi2_1e2p", hChi2_Proton_1e2p_BC_FD_Dir, kBlue, true, true, true, false, true, Chi2_Proton_cut_FD,
                          Chi2_Proton_1e_peak_FD);
        }
        //</editor-fold>

        //<editor-fold desc="Testing Chi2 (2p)">
//        histPlotter1D(c1, hChi2_Electron_2p_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      sChi2_Electron_1e2p, "01_Electron_Chi2_2p_CD", hChi2_Electron_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Electron_cut_CD,
//                      Chi2_Electron_1e_peak_CD);
        histPlotter1D(c1, hChi2_Electron_2p_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{e^{-}} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Electron_1e2p, "01_Electron_Chi2_2p_FD", hChi2_Electron_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, Chi2_Electron_cut_FD,
                      Chi2_Electron_1e_peak_FD);

        histPlotter1D(c1, hChi2_Proton_2p_CD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_1e2p, "02_Proton_Chi2_2p_CD", hChi2_Proton_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, Chi2_Proton_cut_CD,
                      Chi2_Proton_1e_peak_CD);
        histPlotter1D(c1, hChi2_Proton_2p_FD, norm_Chi2_plots, true, 1., "#chi^{2}_{p} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sChi2_Proton_1e2p, "02_Proton_Chi2_2p_FD", hChi2_Proton_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, Chi2_Proton_cut_FD,
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
        double Vertex_Electron_1e_Vx_Xmax_CD = Vertex_Electron_1e_Vx_CD->GetBinCenter(Vertex_Electron_1e_Vx_CD->GetMaximumBin());
        double Vertex_Electron_1e_Vy_Xmax_CD = Vertex_Electron_1e_Vy_CD->GetBinCenter(Vertex_Electron_1e_Vy_CD->GetMaximumBin());
        double Vertex_Electron_1e_Vz_Xmax_CD = Vertex_Electron_1e_Vz_CD->GetBinCenter(Vertex_Electron_1e_Vz_CD->GetMaximumBin());
        double Vertex_Electron_1e_Vx_Xmax_FD = Vertex_Electron_1e_Vx_FD->GetBinCenter(Vertex_Electron_1e_Vx_FD->GetMaximumBin());
        double Vertex_Electron_1e_Vy_Xmax_FD = Vertex_Electron_1e_Vy_FD->GetBinCenter(Vertex_Electron_1e_Vy_FD->GetMaximumBin());
        double Vertex_Electron_1e_Vz_Xmax_FD = Vertex_Electron_1e_Vz_FD->GetBinCenter(Vertex_Electron_1e_Vz_FD->GetMaximumBin());
        double Vertex_Proton_1e_Vx_Xmax_CD = Vertex_Proton_1e_Vx_CD->GetBinCenter(Vertex_Proton_1e_Vx_CD->GetMaximumBin());
        double Vertex_Proton_1e_Vy_Xmax_CD = Vertex_Proton_1e_Vy_CD->GetBinCenter(Vertex_Proton_1e_Vy_CD->GetMaximumBin());
        double Vertex_Proton_1e_Vz_Xmax_CD = Vertex_Proton_1e_Vz_CD->GetBinCenter(Vertex_Proton_1e_Vz_CD->GetMaximumBin());
        double Vertex_Proton_1e_Vx_Xmax_FD = Vertex_Proton_1e_Vx_FD->GetBinCenter(Vertex_Proton_1e_Vx_FD->GetMaximumBin());
        double Vertex_Proton_1e_Vy_Xmax_FD = Vertex_Proton_1e_Vy_FD->GetBinCenter(Vertex_Proton_1e_Vy_FD->GetMaximumBin());
        double Vertex_Proton_1e_Vz_Xmax_FD = Vertex_Proton_1e_Vz_FD->GetBinCenter(Vertex_Proton_1e_Vz_FD->GetMaximumBin());
        double Vertex_Kplus_1e_Vx_Xmax_CD = Vertex_Kplus_1e_Vx_CD->GetBinCenter(Vertex_Kplus_1e_Vx_CD->GetMaximumBin());
        double Vertex_Kplus_1e_Vy_Xmax_CD = Vertex_Kplus_1e_Vy_CD->GetBinCenter(Vertex_Kplus_1e_Vy_CD->GetMaximumBin());
//        Vz_cuts.at(2) = Vertex_Kplus_1e_Vz_CD->GetBinCenter(Vertex_Kplus_1e_Vz_CD->GetMaximumBin());
        double Vertex_Kplus_1e_Vz_Xmax_CD = Vertex_Kplus_1e_Vz_FD->GetBinCenter(Vertex_Kplus_1e_Vz_CD->GetMaximumBin());
        double Vertex_Kplus_1e_Vx_Xmax_FD = Vertex_Kplus_1e_Vx_FD->GetBinCenter(Vertex_Kplus_1e_Vx_FD->GetMaximumBin());
        double Vertex_Kplus_1e_Vy_Xmax_FD = Vertex_Kplus_1e_Vy_FD->GetBinCenter(Vertex_Kplus_1e_Vy_FD->GetMaximumBin());
        double Vertex_Kplus_1e_Vz_Xmax_FD = Vertex_Kplus_1e_Vz_FD->GetBinCenter(Vertex_Kplus_1e_Vz_FD->GetMaximumBin());
//        Vz_cuts.at(2) = Vertex_Kplus_1e_Vz_CD->GetBinCenter(Vertex_Kplus_1e_Vz_CD->GetMaximumBin());
        double Vertex_Kminus_1e_Vx_Xmax_CD = Vertex_Kminus_1e_Vx_CD->GetBinCenter(Vertex_Kminus_1e_Vx_CD->GetMaximumBin());
        double Vertex_Kminus_1e_Vy_Xmax_CD = Vertex_Kminus_1e_Vy_CD->GetBinCenter(Vertex_Kminus_1e_Vy_CD->GetMaximumBin());
        double Vertex_Kminus_1e_Vz_Xmax_CD = Vertex_Kminus_1e_Vz_CD->GetBinCenter(Vertex_Kminus_1e_Vz_CD->GetMaximumBin());
        double Vertex_Kminus_1e_Vx_Xmax_FD = Vertex_Kminus_1e_Vx_FD->GetBinCenter(Vertex_Kminus_1e_Vx_FD->GetMaximumBin());
        double Vertex_Kminus_1e_Vy_Xmax_FD = Vertex_Kminus_1e_Vy_FD->GetBinCenter(Vertex_Kminus_1e_Vy_FD->GetMaximumBin());
        double Vertex_Kminus_1e_Vz_Xmax_FD = Vertex_Kminus_1e_Vz_FD->GetBinCenter(Vertex_Kminus_1e_Vz_FD->GetMaximumBin());
        double Vertex_piplus_1e_Vx_Xmax_CD = Vertex_piplus_1e_Vx_CD->GetBinCenter(Vertex_piplus_1e_Vx_CD->GetMaximumBin());
        double Vertex_piplus_1e_Vy_Xmax_CD = Vertex_piplus_1e_Vy_CD->GetBinCenter(Vertex_piplus_1e_Vy_CD->GetMaximumBin());
        double Vertex_piplus_1e_Vz_Xmax_CD = Vertex_piplus_1e_Vz_CD->GetBinCenter(Vertex_piplus_1e_Vz_CD->GetMaximumBin());
        double Vertex_piplus_1e_Vx_Xmax_FD = Vertex_piplus_1e_Vx_FD->GetBinCenter(Vertex_piplus_1e_Vx_FD->GetMaximumBin());
        double Vertex_piplus_1e_Vy_Xmax_FD = Vertex_piplus_1e_Vy_FD->GetBinCenter(Vertex_piplus_1e_Vy_FD->GetMaximumBin());
        double Vertex_piplus_1e_Vz_Xmax_FD = Vertex_piplus_1e_Vz_FD->GetBinCenter(Vertex_piplus_1e_Vz_FD->GetMaximumBin());
        double Vertex_piminus_1e_Vx_Xmax_CD = Vertex_piminus_1e_Vx_CD->GetBinCenter(Vertex_piminus_1e_Vx_CD->GetMaximumBin());
        double Vertex_piminus_1e_Vy_Xmax_CD = Vertex_piminus_1e_Vy_CD->GetBinCenter(Vertex_piminus_1e_Vy_CD->GetMaximumBin());
        double Vertex_piminus_1e_Vz_Xmax_CD = Vertex_piminus_1e_Vz_CD->GetBinCenter(Vertex_piminus_1e_Vz_CD->GetMaximumBin());
        double Vertex_piminus_1e_Vx_Xmax_FD = Vertex_piminus_1e_Vx_FD->GetBinCenter(Vertex_piminus_1e_Vx_FD->GetMaximumBin());
        double Vertex_piminus_1e_Vy_Xmax_FD = Vertex_piminus_1e_Vy_FD->GetBinCenter(Vertex_piminus_1e_Vy_FD->GetMaximumBin());
        double Vertex_piminus_1e_Vz_Xmax_FD = Vertex_piminus_1e_Vz_FD->GetBinCenter(Vertex_piminus_1e_Vz_FD->GetMaximumBin());
        dVx_Xmax = deltaVx_before_dV_cuts_1e2p->GetBinCenter(deltaVx_before_dV_cuts_1e2p->GetMaximumBin());
        dVy_Xmax = deltaVy_before_dV_cuts_1e2p->GetBinCenter(deltaVy_before_dV_cuts_1e2p->GetMaximumBin());
        dVz_Xmax = deltaVz_before_dV_cuts_1e2p->GetBinCenter(deltaVz_before_dV_cuts_1e2p->GetMaximumBin());
        //</editor-fold>

//  dV plots (1e2p, CD & FD) ----------------------------------------------------------------------------

        //<editor-fold desc="Vertesx plots (1e cut, CD & FD)">
        histPlotter1D(c1, Vertex_Electron_1e_Vx_CD, norm_Vertex_plots, true, 1., "V_{x}^{e^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Electron_1e_Vx_Stack, "01_Electron_Vx", Vertex_Electron_1e_Vx_CD_Dir, "CD", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);
        histPlotter1D(c1, Vertex_Electron_1e_Vx_FD, norm_Vertex_plots, true, 1., "V_{x}^{e^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Electron_1e_Vx_Stack, "01_Electron_Vx", Vertex_Electron_1e_Vx_FD_Dir, "FD", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);
        histPlotter1D(c1, Vertex_Electron_1e_Vy_CD, norm_Vertex_plots, true, 1., "V_{y}^{e^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Electron_1e_Vy_Stack, "02_Electron_Vy", Vertex_Electron_1e_Vy_CD_Dir, "CD", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);
        histPlotter1D(c1, Vertex_Electron_1e_Vy_FD, norm_Vertex_plots, true, 1., "V_{y}^{e^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Electron_1e_Vy_Stack, "02_Electron_Vy", Vertex_Electron_1e_Vy_FD_Dir, "FD", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);
        histPlotter1D(c1, Vertex_Electron_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{e^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Electron_1e_Vz_Stack, "03_Electron_Vz", Vertex_Electron_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        histPlotter1D(c1, Vertex_Electron_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{e^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Electron_1e_Vz_Stack, "03_Electron_Vz", Vertex_Electron_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);

        histPlotter1D(c1, Vertex_Proton_1e_Vx_CD, norm_Vertex_plots, true, 1., "V_{x}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Proton_1e_Vx_Stack, "01_Proton_Vx", Vertex_Proton_1e_Vx_CD_Dir, "CD", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);
        histPlotter1D(c1, Vertex_Proton_1e_Vx_FD, norm_Vertex_plots, true, 1., "V_{x}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Proton_1e_Vx_Stack, "01_Proton_Vx", Vertex_Proton_1e_Vx_FD_Dir, "FD", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);
        histPlotter1D(c1, Vertex_Proton_1e_Vy_CD, norm_Vertex_plots, true, 1., "V_{y}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Proton_1e_Vy_Stack, "02_Proton_Vy", Vertex_Proton_1e_Vy_CD_Dir, "CD", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);
        histPlotter1D(c1, Vertex_Proton_1e_Vy_FD, norm_Vertex_plots, true, 1., "V_{y}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Proton_1e_Vy_Stack, "02_Proton_Vy", Vertex_Proton_1e_Vy_FD_Dir, "FD", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);
//        histPlotter1D(c1, Vertex_Proton_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_Proton_1e_Vz_Stack, "03_Proton_Vz", Vertex_Proton_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
//        histPlotter1D(c1, Vertex_Proton_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{p}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_Proton_1e_Vz_Stack, "03_Proton_Vz", Vertex_Proton_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        histPlotter1D(c1, Vertex_Proton_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{p}", "1e Cut", plots, 2, false, true, Vertex_Proton_1e_Vz_Stack,
                      "03_Proton_Vz", Vertex_Proton_1e_Vz_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_Proton_1e_Vz_Xmax_CD);
        histPlotter1D(c1, Vertex_Proton_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{p}", "1e Cut", plots, 2, false, true, Vertex_Proton_1e_Vz_Stack,
                      "03_Proton_Vz", Vertex_Proton_1e_Vz_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_Proton_1e_Vz_Xmax_FD);

        histPlotter1D(c1, Vertex_Kplus_1e_Vx_CD, norm_Vertex_plots, true, 1., "V_{x}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kplus_1e_Vx_Stack, "01_Kplus_Vx", Vertex_Kplus_1e_Vx_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kplus_1e_Vx_FD, norm_Vertex_plots, true, 1., "V_{x}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kplus_1e_Vx_Stack, "01_Kplus_Vx", Vertex_Kplus_1e_Vx_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kplus_1e_Vy_CD, norm_Vertex_plots, true, 1., "V_{y}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kplus_1e_Vy_Stack, "02_Kplus_Vy", Vertex_Kplus_1e_Vy_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kplus_1e_Vy_FD, norm_Vertex_plots, true, 1., "V_{y}^{K^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kplus_1e_Vy_Stack, "02_Kplus_Vy", Vertex_Kplus_1e_Vy_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kplus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{K^{+}}", "1e Cut", plots, 2, false, true, Vertex_Kplus_1e_Vz_Stack,
                      "03_Kplus_Vz", Vertex_Kplus_1e_Vz_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_Kplus_1e_Vz_Xmax_CD);
        histPlotter1D(c1, Vertex_Kplus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{K^{+}}", "1e Cut", plots, 2, false, true, Vertex_Kplus_1e_Vz_Stack,
                      "03_Kplus_Vz", Vertex_Kplus_1e_Vz_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_Kplus_1e_Vz_Xmax_FD);

        histPlotter1D(c1, Vertex_Kminus_1e_Vx_CD, norm_Vertex_plots, true, 1., "V_{x}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kminus_1e_Vx_Stack, "01_Kminus_Vx", Vertex_Kminus_1e_Vx_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kminus_1e_Vx_FD, norm_Vertex_plots, true, 1., "V_{x}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kminus_1e_Vx_Stack, "01_Kminus_Vx", Vertex_Kminus_1e_Vx_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kminus_1e_Vy_CD, norm_Vertex_plots, true, 1., "V_{y}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kminus_1e_Vy_Stack, "02_Kminus_Vy", Vertex_Kminus_1e_Vy_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_Kminus_1e_Vy_FD, norm_Vertex_plots, true, 1., "V_{y}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_Kminus_1e_Vy_Stack, "02_Kminus_Vy", Vertex_Kminus_1e_Vy_FD_Dir, "FD", kBlue, true, true, true, false);
//        histPlotter1D(c1, Vertex_Kminus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_Kminus_1e_Vz_Stack, "03_Kminus_Vz", Vertex_Kminus_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false);
////                      Vertex_Kminus_1e_Vz_Stack, "03_Kminus_Vz", Vertex_Kminus_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
//        histPlotter1D(c1, Vertex_Kminus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{K^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_Kminus_1e_Vz_Stack, "03_Kminus_Vz", Vertex_Kminus_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false);
////                      Vertex_Kminus_1e_Vz_Stack, "03_Kminus_Vz", Vertex_Kminus_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        histPlotter1D(c1, Vertex_Kminus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{K^{-}}", "1e Cut", plots, 2, false, true, Vertex_Kminus_1e_Vz_Stack,
                      "03_Kminus_Vz", Vertex_Kminus_1e_Vz_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_Kminus_1e_Vz_Xmax_CD);
        histPlotter1D(c1, Vertex_Kminus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{K^{-}}", "1e Cut", plots, 2, false, true, Vertex_Kminus_1e_Vz_Stack,
                      "03_Kminus_Vz", Vertex_Kminus_1e_Vz_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_Kminus_1e_Vz_Xmax_FD);

        histPlotter1D(c1, Vertex_piplus_1e_Vx_CD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piplus_1e_Vx_Stack, "01_piplus_Vx", Vertex_piplus_1e_Vx_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_piplus_1e_Vx_FD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piplus_1e_Vx_Stack, "01_piplus_Vx", Vertex_piplus_1e_Vx_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_piplus_1e_Vy_CD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piplus_1e_Vy_Stack, "02_piplus_Vy", Vertex_piplus_1e_Vy_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_piplus_1e_Vy_FD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piplus_1e_Vy_Stack, "02_piplus_Vy", Vertex_piplus_1e_Vy_FD_Dir, "FD", kBlue, true, true, true, false);
//        histPlotter1D(c1, Vertex_piplus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_piplus_1e_Vz_Stack, "03_piplus_Vz", Vertex_piplus_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false);
////                      Vertex_piplus_1e_Vz_Stack, "03_piplus_Vz", Vertex_piplus_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
//        histPlotter1D(c1, Vertex_piplus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{+}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_piplus_1e_Vz_Stack, "03_piplus_Vz", Vertex_piplus_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false);
////                      Vertex_piplus_1e_Vz_Stack, "03_piplus_Vz", Vertex_piplus_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        histPlotter1D(c1, Vertex_piplus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{+}}", "1e Cut", plots, 2, false, true, Vertex_piplus_1e_Vz_Stack,
                      "03_piplus_Vz", Vertex_piplus_1e_Vz_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_piplus_1e_Vz_Xmax_CD);
        histPlotter1D(c1, Vertex_piplus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{+}}", "1e Cut", plots, 2, false, true, Vertex_piplus_1e_Vz_Stack,
                      "03_piplus_Vz", Vertex_piplus_1e_Vz_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_piplus_1e_Vz_Xmax_FD);

        histPlotter1D(c1, Vertex_piminus_1e_Vx_CD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piminus_1e_Vx_Stack, "01_piminus_Vx", Vertex_piminus_1e_Vx_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_piminus_1e_Vx_FD, norm_Vertex_plots, true, 1., "V_{x}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piminus_1e_Vx_Stack, "01_piminus_Vx", Vertex_piminus_1e_Vx_FD_Dir, "FD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_piminus_1e_Vy_CD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piminus_1e_Vy_Stack, "02_piminus_Vy", Vertex_piminus_1e_Vy_CD_Dir, "CD", kBlue, true, true, true, false);
        histPlotter1D(c1, Vertex_piminus_1e_Vy_FD, norm_Vertex_plots, true, 1., "V_{y}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      Vertex_piminus_1e_Vy_Stack, "02_piminus_Vy", Vertex_piminus_1e_Vy_FD_Dir, "FD", kBlue, true, true, true, false);
//        histPlotter1D(c1, Vertex_piminus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_piminus_1e_Vz_Stack, "03_piminus_Vz", Vertex_piminus_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false);
////                      Vertex_piminus_1e_Vz_Stack, "03_piminus_Vz", Vertex_piminus_1e_Vz_CD_Dir, "CD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
//        histPlotter1D(c1, Vertex_piminus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{-}}", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      Vertex_piminus_1e_Vz_Stack, "03_piminus_Vz", Vertex_piminus_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false);
////                      Vertex_piminus_1e_Vz_Stack, "03_piminus_Vz", Vertex_piminus_1e_Vz_FD_Dir, "FD", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        histPlotter1D(c1, Vertex_piminus_1e_Vz_CD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{-}}", "1e Cut", plots, 2, false, true, Vertex_piminus_1e_Vz_Stack,
                      "03_piminus_Vz", Vertex_piminus_1e_Vz_CD_Dir, "CD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_piminus_1e_Vz_Xmax_CD);
        histPlotter1D(c1, Vertex_piminus_1e_Vz_FD, norm_Vertex_plots, true, 1., "V_{z}^{#pi^{-}}", "1e Cut", plots, 2, false, true, Vertex_piminus_1e_Vz_Stack,
                      "03_piminus_Vz", Vertex_piminus_1e_Vz_FD_Dir, "FD", kBlue, false, true, false, true, Vz_cuts.at(1), Vz_cuts.at(0), Vertex_piminus_1e_Vz_Xmax_CD);
        //</editor-fold>

        //<editor-fold desc="dV plots (1e2p, CD & FD)">
        if (apply_cuts == false) {
            histPlotter1D(c1, deltaVx_before_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, dVx_1e2p_before_Stack, "01_dVx_BC", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                          dVx_cut, dVx_peak);
            histPlotter1D(c1, deltaVy_before_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, dVy_1e2p_before_Stack, "02_dVy_BC", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                          dVy_cut, dVy_peak);
            histPlotter1D(c1, deltaVz_before_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} Before Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, dVz_1e2p_before_Stack, "03_dVz_BC", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                          dVz_cut, dVz_peak);

            histPlotter1D(c1, deltaVx_after_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} After Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, dVx_1e2p_after_Stack, "01_dVx_AC", Vertex_1e2p_dV_AC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                          dVx_cut, dVx_peak);
            histPlotter1D(c1, deltaVy_after_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} After Cuts", "1e2p", 0.06, 0.0425, 0.0425,
                          plots, 2, false, true, dVy_1e2p_after_Stack, "02_dVy_AC", Vertex_1e2p_dV_AC_Directory, "CD & FD", kBlue, true, true, true, false, true,
                          dVy_cut, dVy_peak);
            histPlotter1D(c1, deltaVz_after_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} After Cuts", "1e2p", plots, 2, false, true,
                          dVz_1e2p_after_Stack, "03_dVz_AC", Vertex_1e2p_dV_AC_Directory, "CD & FD", kBlue, false, true, false, true, dVz_cuts.at(1), dVz_cuts.at(0),
                          dVz_peak);
        } else {
            histPlotter1D(c1, deltaVx_before_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x}", "1e2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, dVx_1e2p_before_Stack, "01_dVx", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);
            histPlotter1D(c1, deltaVy_before_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y}", "1e2p", 0.06, 0.0425, 0.0425, plots, 2,
                          false, true, dVy_1e2p_before_Stack, "02_dVy", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);
            histPlotter1D(c1, deltaVz_before_dV_cuts_1e2p, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z}", "1e2p", plots, 2, false, true,
                          dVy_1e2p_before_Stack, "03_dVz", Vertex_1e2p_dV_BC_Directory, "CD & FD", kBlue, false, true, false, true, dVz_cuts.at(1), dVz_cuts.at(0),
                          dVz_peak);
        }
        //</editor-fold>

        //<editor-fold desc="dV plots after dV cuts (2p, CD & FD)">
        histPlotter1D(c1, deltaVx_2p, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      dVx_2p_Stack, "01_dVx", Vertex_dV_2p_Directory, "CD & FD", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);

        histPlotter1D(c1, deltaVy_2p, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      dVy_2p_Stack, "02_dVy", Vertex_dV_2p_Directory, "CD & FD", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);

//        histPlotter1D(c1, deltaVz_2p, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z}", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      dVz_2p_Stack, "03_dVz", Vertex_dV_2p_Directory, "2p", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
        histPlotter1D(c1, deltaVz_2p, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z}", "2p", plots, 2, false, true, dVz_2p_Stack, "03_dVz",
                      Vertex_dV_2p_Directory, "CD & FD", kBlue, false, true, false, true, dVz_cuts.at(1), dVz_cuts.at(0), dVz_peak);
        //</editor-fold>

//        //<editor-fold desc="dV plots before dV cuts (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, deltaVx_MicroBooNE_BC, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} Before Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, dVx_MicroBooNE_Stack, "01_dVx_MicroBooNE_BC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
//                      false, true, dVx_cut, dVx_peak);
//
//        histPlotter1D(c1, deltaVy_MicroBooNE_BC, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} Before Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, dVy_MicroBooNE_Stack, "02_dVy_MicroBooNE_BC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
//                      false, true, dVy_cut, dVy_peak);
//
//        histPlotter1D(c1, deltaVz_MicroBooNE_BC, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} Before Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, dVz_MicroBooNE_Stack, "03_dVz_MicroBooNE_BC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
//                      false, true, dVz_cut, dVz_peak);
//
//        histPlotter1D(c1, deltaVx_MicroBooNE_AC, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x} After Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, dVx_MicroBooNE_Stack, "01_dVx_MicroBooNE_AC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
//                      false, true, dVx_cut, dVx_peak);
//
//        histPlotter1D(c1, deltaVy_MicroBooNE_AC, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y} After Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, dVy_MicroBooNE_Stack, "02_dVy_MicroBooNE_AC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
//                      false, true, dVy_cut, dVy_peak);
//
//        histPlotter1D(c1, deltaVz_MicroBooNE_AC, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z} After Cuts", "MicroBooNE", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, dVz_MicroBooNE_Stack, "03_dVz_MicroBooNE_AC", Vertex_dV_MicroBooNE_cut_tests_Directory, "CD & FD", kBlue, true, true, true,
//                      false, true, dVz_cut, dVz_peak);
//        //</editor-fold>
//
//        //<editor-fold desc="dV plots after dV cuts (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, deltaVx_MicroBooNE, norm_Vertex_plots, true, 1., "dV_{x}=V^{p}_{x}-V^{e^{-}}_{x}", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      dVx_MicroBooNE_Stack, "01_dVx_MicroBooNE", Vertex_dV_MicroBooNE_Directory, "MicroBooNE", kBlue, true, true, true, false, true, dVx_cut, dVx_peak);
//
//        histPlotter1D(c1, deltaVy_MicroBooNE, norm_Vertex_plots, true, 1., "dV_{y}=V^{p}_{y}-V^{e^{-}}_{y}", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      dVy_MicroBooNE_Stack, "02_dVy_MicroBooNE", Vertex_dV_MicroBooNE_Directory, "MicroBooNE", kBlue, true, true, true, false, true, dVy_cut, dVy_peak);
//
//        histPlotter1D(c1, deltaVz_MicroBooNE, norm_Vertex_plots, true, 1., "dV_{z}=V^{p}_{z}-V^{e^{-}}_{z}", "MicroBooNE", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                      dVz_MicroBooNE_Stack, "03_dVz_MicroBooNE", Vertex_dV_MicroBooNE_Directory, "MicroBooNE", kBlue, true, true, true, false, true, dVz_cut, dVz_peak);
//        //</editor-fold>

    } else {
        cout << "\n\nVertex plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Sampling Fraction (SF) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Sampling Fraction (SF) histograms">
    if (SF_plots) {
        cout << "\n\nPlotting Sampling Fraction histograms...\n\n";

//  SF histograms --------------------------------------------------------------

        SF_1e2p_Xmax = hSF_1e_cut_BC_FD->GetBinCenter(hSF_1e_cut_BC_FD->GetMaximumBin());

        //<editor-fold desc="SF and SF vs. P histograms (1e cut, FD)">
        if (apply_cuts == false) {
            histPlotter1D(c1, hSF_1e_cut_BC_FD, norm_SF_plots, true, 1., "Sampling Fraction f Before Cuts", "1e Cut", plots, 2, false, true, sSF_1e_cut_FD,
                          "01_SF_1e_cut_before_SF_cuts", hSF_1e_cut_BC_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e_upper_cut, SF_1e_lower_cut, SF_1e2p_Xmax);

            histPlotter1D(c1, hSF_1e_cut_AC_FD, norm_SF_plots, true, 1., "Sampling Fraction f After cuts", "1e Cut", plots, 2, false, true, sSF_1e_cut_FD,
                          "02_SF_1e_cut_after_SF_cuts", hSF_1e_cut_AC_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e_upper_cut, SF_1e_lower_cut, SF_1e2p_Xmax);

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
            histPlotter1D(c1, hSF_1e_cut_BC_FD, norm_SF_plots, true, 1., "Sampling Fraction f", "1e Cut", plots, 2, false, true, sSF_1e_cut_FD, "01_SF_1e2p",
                          hSF_1e_cut_BC_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e_upper_cut, SF_1e_lower_cut, SF_1e2p_Xmax);

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
        histPlotter1D(c1, hSF_2p_FD, norm_SF_plots, true, 1., "Sampling Fraction f", "All Int., 2p", plots, 2, false, true, sSF_2p_FD, "SF_2p_wAll_cuts",
                      hSF_2p_FD_Dir, "FD", kBlue, true, true, false, true, SF_1e_upper_cut, SF_1e_lower_cut, SF_1e2p_Xmax);
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
//        gStyle->SetStatX(0.87);
//        gStyle->SetStatY(0.4);
        hSF_VS_P_e_2p_FD->SetStats(0);
        c1->SaveAs((hSF_VS_P_e_2p_FD_Dir + "hSF_VS_P_e_2p_FD.png").c_str());
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

    } else {
        cout << "\n\nFiducial plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// Momentum histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Momentum histograms">
    if (Momentum_plots) {
        cout << "\n\nPlotting Momentum histograms...\n\n";

//  Momentum histograms (1e2p, CD & FD)  --------------------------------------------------------------

        //<editor-fold desc="Momentum histograms before & after cuts (1e2p, CD & FD)">
//        histPlotter1D(c1, hP_e_1e2p_BC_CD, false, true, 1., "P_{e} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_CD, "01_P_e_1e2p_BC_CD", hP_e_1e2p_BC_CD_Dir,
//                      "CD", kBlue, true, true, false, true, e_mom_cuts_2p.at(1), e_mom_cuts_2p.at(0), 0, false);
//        histPlotter1D(c1, hP_e_1e2p_AC_CD, false, true, 1., "P_{e} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_CD, "01_P_e_1e2p_AC_CD", hP_e_1e2p_AC_CD_Dir,
//                      "CD", kBlue, true, true, false, true, e_mom_cuts_2p.at(1), e_mom_cuts_2p.at(0), 0, false);

        histPlotter1D(c1, hP_e_1e2p_BC_FD, false, true, 1., "P_{e} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_FD, "02_P_e_1e2p_BC_FD", hP_e_1e2p_BC_FD_Dir,
                      "FD", kBlue, true, true, false, true, e_mom_cuts_2p.at(1), e_mom_cuts_2p.at(0), 0, false);
        histPlotter1D(c1, hP_e_1e2p_AC_FD, false, true, 1., "P_{e} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_FD, "02_P_e_1e2p_AC_FD", hP_e_1e2p_AC_FD_Dir,
                      "FD", kBlue, true, true, false, true, e_mom_cuts_2p.at(1), e_mom_cuts_2p.at(0), 0, false);

        histPlotter1D(c1, hP_p_1e2p_BC_CD, false, true, 1., "P_{p} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_CD, "03_P_p_1e2p_BC_CD", hP_p_1e2p_BC_CD_Dir,
                      "CD", kBlue, true, true, false, true, p_mom_cuts_2p.at(1), p_mom_cuts_2p.at(0), 0, false);
        histPlotter1D(c1, hP_p_1e2p_AC_CD, false, true, 1., "P_{p} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_CD, "03_P_p_1e2p_AC_CD", hP_p_1e2p_AC_CD_Dir,
                      "CD", kBlue, true, true, false, true, p_mom_cuts_2p.at(1), p_mom_cuts_2p.at(0), 0, false);

        histPlotter1D(c1, hP_p_1e2p_BC_FD, false, true, 1., "P_{p} Before Cut", "1e2p", plots, 2, false, true, sP_1e2p_BC_FD, "04_P_p_1e2p_BC_FD", hP_p_1e2p_BC_FD_Dir,
                      "FD", kBlue, true, true, false, true, p_mom_cuts_2p.at(1), p_mom_cuts_2p.at(0), 0, false);
        histPlotter1D(c1, hP_p_1e2p_AC_FD, false, true, 1., "P_{p} After Cut", "1e2p", plots, 2, false, true, sP_1e2p_AC_FD, "04_P_p_1e2p_AC_FD", hP_p_1e2p_AC_FD_Dir,
                      "FD", kBlue, true, true, false, true, p_mom_cuts_2p.at(1), p_mom_cuts_2p.at(0), 0, false);
        //</editor-fold>

        //<editor-fold desc="Momentum histograms before & after cuts (2p, CD & FD)">
//        histPlotter1D(c1, hP_e_2p_CD, false, true, 1., "P_{e}", "2p", plots, 2, false, true, sP_2p_CD, "01_P_e_2p_CD", hP_e_2p_CD_Dir, "CD", kBlue, true, true, false,
//                      true, e_mom_cuts_2p.at(1), e_mom_cuts_2p.at(0), 0, false);
        histPlotter1D(c1, hP_e_2p_FD, false, true, 1., "P_{e}", "2p", plots, 2, false, true, sP_2p_FD, "01_P_e_2p_FD", hP_e_2p_FD_Dir, "FD", kBlue, true, true, false,
                      true, e_mom_cuts_2p.at(1), e_mom_cuts_2p.at(0), 0, false);

        histPlotter1D(c1, hP_p_2p_CD, false, true, 1., "P_{p}", "2p", plots, 2, false, true, sP_2p_CD, "02_P_p_2p_CD", hP_p_2p_CD_Dir, "CD", kBlue, true, true, false,
                      true, p_mom_cuts_2p.at(1), p_mom_cuts_2p.at(0), 0, false);
        histPlotter1D(c1, hP_p_2p_FD, false, true, 1., "P_{p}", "2p", plots, 2, false, true, sP_2p_FD, "02_P_p_2p_FD", hP_p_2p_FD_Dir, "FD", kBlue, true, true, false,
                      true, p_mom_cuts_2p.at(1), p_mom_cuts_2p.at(0), 0, false);
        //</editor-fold>

////  Momentum histograms (MicroBooNE, CD & FD)  --------------------------------------------------------------
//
////        double Momentum_integral = P_e_histogram_CD->Integral() + P_e_histogram_FD->Integral();
//
//        //<editor-fold desc="Momentum histograms before cuts (MicroBooNE, CD & FD)">
//
//        //<editor-fold desc="Electrons momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_e_MicroBooNE_BC_CD, false, true, 1., "P_{e} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_CD,
//                      "01_P_e_MicroBooNE_BC_CD", P_e_MicroBooNE_BC_CD_Dir, "CD", kBlue, true, true, false, true, e_mom_cuts_MicroBooNE.at(1),
//                      e_mom_cuts_MicroBooNE.at(0), 0, false);
//
//        histPlotter1D(c1, P_e_MicroBooNE_BC_FD, false, true, 1., "P_{e} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_FD,
//                      "02_P_e_MicroBooNE_BC_FD", P_e_MicroBooNE_BC_FD_Dir, "FD", kBlue, true, true, false, true, e_mom_cuts_MicroBooNE.at(1),
//                      e_mom_cuts_MicroBooNE.at(0), 0, false);
//        //</editor-fold>
//
//        //<editor-fold desc="Protons momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_p_MicroBooNE_BC_CD, false, true, 1., "P_{p} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_CD,
//                      "03_P_p_MicroBooNE_BC_CD", P_p_MicroBooNE_BC_CD_Dir, "CD", kBlue, true, true, false, true, p_mom_cuts_MicroBooNE.at(1),
//                      p_mom_cuts_MicroBooNE.at(0), 0, false);
//
//        histPlotter1D(c1, P_p_MicroBooNE_BC_FD, false, true, 1., "P_{p} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_FD,
//                      "04_P_p_MicroBooNE_BC_FD", P_p_MicroBooNE_BC_FD_Dir, "FD", kBlue, true, true, false, true, p_mom_cuts_MicroBooNE.at(1),
//                      p_mom_cuts_MicroBooNE.at(0), 0, false);
//        //</editor-fold>
//
//        //<editor-fold desc="cPions momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_cpion_MicroBooNE_BC_CD, false, true, 1., "P_{#pi^{#pm}} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_CD,
//                      "05_P_cpion_MicroBooNE_BC_CD", P_cpion_MicroBooNE_BC_CD_Dir, "CD", kBlue, true, true, false, true, cpion_mom_cuts_MicroBooNE(1),
//                      cpion_momentum_lower_cut_MicroBooNE, 0, false);
//
//        histPlotter1D(c1, P_cpion_MicroBooNE_BC_FD, false, true, 1., "P_{#pi^{#pm}} Before Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_BC_FD,
//                      "06_P_cpion_MicroBooNE_BC_FD", P_cpion_MicroBooNE_BC_FD_Dir, "FD", kBlue, true, true, false, true, cpion_mom_cuts_MicroBooNE(1),
//                      cpion_momentum_lower_cut_MicroBooNE, 0, false);
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms after cuts (MicroBooNE, CD & FD)">
//
//        //<editor-fold desc="Electrons momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_e_MicroBooNE_AC_CD, false, true, 1., "P_{e} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_CD,
//                      "01_P_e_MicroBooNE_AC_CD", P_e_MicroBooNE_AC_CD_Dir, "CD", kBlue, true, true, false, true, e_mom_cuts_MicroBooNE.at(1),
//                      e_mom_cuts_MicroBooNE.at(0), 0, false);
//
//        histPlotter1D(c1, P_e_MicroBooNE_AC_FD, false, true, 1., "P_{e} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_FD,
//                      "02_P_e_MicroBooNE_AC_FD", P_e_MicroBooNE_AC_FD_Dir, "FD", kBlue, true, true, false, true, e_mom_cuts_MicroBooNE.at(1),
//                      e_mom_cuts_MicroBooNE.at(0), 0, false);
//        //</editor-fold>
//
//        //<editor-fold desc="Protons momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_p_MicroBooNE_AC_CD, false, true, 1., "P_{p} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_CD,
//                      "03_P_p_MicroBooNE_AC_CD", P_p_MicroBooNE_AC_CD_Dir, "CD", kBlue, true, true, false, true, p_mom_cuts_MicroBooNE.at(1),
//                      p_mom_cuts_MicroBooNE.at(0), 0, false);
//
//        histPlotter1D(c1, P_p_MicroBooNE_AC_FD, false, true, 1., "P_{p} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_FD,
//                      "04_P_p_MicroBooNE_AC_FD", P_p_MicroBooNE_AC_FD_Dir, "FD", kBlue, true, true, false, true, p_mom_cuts_MicroBooNE.at(1),
//                      p_mom_cuts_MicroBooNE.at(0), 0, false);
//        //</editor-fold>
//
//        //<editor-fold desc="cPions momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_cpion_MicroBooNE_AC_CD, false, true, 1., "P_{#pi^{#pm}} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_CD,
//                      "05_P_cpion_MicroBooNE_AC_CD", P_cpion_MicroBooNE_AC_CD_Dir, "CD", kBlue, true, true, false, true, cpion_mom_cuts_MicroBooNE(1),
//                      cpion_momentum_lower_cut_MicroBooNE, 0, false);
//
//        histPlotter1D(c1, P_cpion_MicroBooNE_AC_FD, false, true, 1., "P_{#pi^{#pm}} After Cut", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_AC_FD,
//                      "06_P_cpion_MicroBooNE_AC_FD", P_cpion_MicroBooNE_AC_FD_Dir, "FD", kBlue, true, true, false, true, cpion_mom_cuts_MicroBooNE(1),
//                      cpion_momentum_lower_cut_MicroBooNE, 0, false);
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Momentum histograms after MicroBooNE cuts (CD & FD)">
//
//        //<editor-fold desc="Electrons momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_e_MicroBooNE_CD, false, true, 1., "P_{e}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_CD, "01_P_e_MicroBooNE_CD",
//                      P_e_MicroBooNE_CD_Dir, "CD", kBlue, true, true, false, true, e_mom_cuts_MicroBooNE.at(1), e_mom_cuts_MicroBooNE.at(0), 0, false);
//
//        histPlotter1D(c1, P_e_MicroBooNE_FD, false, true, 1., "P_{e}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_FD, "02_P_e_MicroBooNE_FD",
//                      P_e_MicroBooNE_FD_Dir, "FD", kBlue, true, true, false, true, e_mom_cuts_MicroBooNE.at(1), e_mom_cuts_MicroBooNE.at(0), 0, false);
//        //</editor-fold>
//
//        //<editor-fold desc="Protons momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_p_MicroBooNE_CD, false, true, 1., "P_{p}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_CD, "03_P_p_MicroBooNE_CD",
//                      P_p_MicroBooNE_CD_Dir, "CD", kBlue, true, true, false, true, p_mom_cuts_MicroBooNE.at(1), p_mom_cuts_MicroBooNE.at(0), 0, false);
//
//        histPlotter1D(c1, P_p_MicroBooNE_FD, false, true, 1., "P_{p}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_FD, "04_P_p_MicroBooNE_FD",
//                      P_p_MicroBooNE_FD_Dir, "FD", kBlue, true, true, false, true, p_mom_cuts_MicroBooNE.at(1), p_mom_cuts_MicroBooNE.at(0), 0, false);
//        //</editor-fold>
//
//        //<editor-fold desc="cPions momentum histograms (MicroBooNE, CD & FD)">
//        histPlotter1D(c1, P_cpion_MicroBooNE_CD, false, true, 1., "P_{#pi^{#pm}}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_CD, "05_P_cpion_MicroBooNE_CD",
//                      P_cpion_MicroBooNE_CD_Dir, "CD", kBlue, true, true, false, true, cpion_mom_cuts_MicroBooNE(1), cpion_momentum_lower_cut_MicroBooNE, 0,
//                      false);
//
//        histPlotter1D(c1, P_cpion_MicroBooNE_FD, false, true, 1., "P_{#pi^{#pm}}", "MicroBooNE", plots, 2, false, true, P_Stack_MicroBooNE_FD, "06_P_cpion_MicroBooNE_FD",
//                      P_cpion_MicroBooNE_FD_Dir, "FD", kBlue, true, true, false, true, cpion_mom_cuts_MicroBooNE(1), cpion_momentum_lower_cut_MicroBooNE, 0,
//                      false);
//        //</editor-fold>
//
//        //</editor-fold>

    } else {
        cout << "\n\nMomentum (P_e) plots are disabled by user.\n\n";
    }
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
        histPlotter2D(c1, Beta_vs_P_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_VS_P_Only_1e_Directory, "01_Beta_vs_P_1e_cut_All_Particles_CD.png",
                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);

        histPlotter2D(c1, Beta_vs_P_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_VS_P_Only_1e_Directory, "01_Beta_vs_P_1e_cut_All_Particles_FD.png",
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
        histPlotter2D(c1, Beta_vs_P_positive_particles_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "01_Beta_vs_P_q_p1_1e_cut_CD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);

        histPlotter2D(c1, Beta_vs_P_positive_particles_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "01_Beta_vs_P_q_p1_1e_cut_FD.png",
                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_neutral_particles_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "02_Beta_vs_P_q_0_1e_cut_CD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);

        histPlotter2D(c1, Beta_vs_P_neutral_particles_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "02_Beta_vs_P_q_0_1e_cut_FD.png",
                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
        //</editor-fold>

        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
        histPlotter2D(c1, Beta_vs_P_negative_particles_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "03_Beta_vs_P_q_m1_1e_cut_CD.png",
                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);

        histPlotter2D(c1, Beta_vs_P_negative_particles_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
                      plots, true, Beta_VS_P_by_charge_Only_1e_Directory, "03_Beta_vs_P_q_m1_1e_cut_FD.png",
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

//        //<editor-fold desc="Beta vs. P plots (MicroBooNE-BC)">
//
//        //<editor-fold desc="Beta vs. P for all particles (MicroBooNE-BC, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_BC_CD_Dir,
//                      "01_Beta_vs_P_MicroBooNE_BC_CD.png",
//                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
//
//        histPlotter2D(c1, Beta_vs_P_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_BC_FD_Dir,
//                      "01_Beta_vs_P_MicroBooNE_BC_FD.png",
//                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
//        //</editor-fold>
//
////        //<editor-fold desc="Beta vs. P for Electrons Only (MicroBooNE-BC, CD & FD)">
////        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Electrons_Only_CD_Dir, "02_Beta_vs_P_2p_Electrons_Only_CD.png",
////                      beta_electron, "Electrons", true);
////
////        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Electrons_Only_FD_Dir, "02_Beta_vs_P_2p_Electrons_Only_FD.png",
////                      beta_electron, "Electrons", true);
////        //</editor-fold>
////
////        //<editor-fold desc="Beta vs. P for Protons Only (MicroBooNE-BC, CD & FD)">
////        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Protons_Only_CD_Dir, "03_Beta_vs_P_2p_Protons_Only_CD.png",
////                      beta_proton, "Protons", true);
////
////        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Protons_Only_FD_Dir, "03_Beta_vs_P_2p_Protons_Only_FD.png",
////                      beta_proton, "Protons", true);
////        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P plots (by charge, MicroBooNE-BC, CD & FD)">
//
//        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true,
//                      Beta_vs_P_positive_particles_MicroBooNE_BC_CD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_BC_CD.png",
//                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
//
//        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_positive_particles_MicroBooNE_BC_FD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_BC_FD.png",
//                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_BC_CD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_BC_CD.png",
//                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
//
//        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_BC_FD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_BC_FD.png",
//                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_BC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_BC_CD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_BC_CD.png",
//                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
//
//        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_BC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_BC_FD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_BC_FD.png",
//                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P plots (MicroBooNE-AC)">
//
//        //<editor-fold desc="Beta vs. P for all particles (MicroBooNE-AC, CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_AC_CD_Dir,
//                      "01_Beta_vs_P_MicroBooNE_AC_CD.png",
//                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
//
//        histPlotter2D(c1, Beta_vs_P_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, Beta_vs_P_MicroBooNE_AC_FD_Dir,
//                      "01_Beta_vs_P_MicroBooNE_AC_FD.png",
//                      beta_electron, beta_proton, beta_neutron, beta_pizero, beta_piplus, beta_piminus, beta_Kzero, beta_Kplus, beta_Kminus);
//        //</editor-fold>
//
////        //<editor-fold desc="Beta vs. P for Electrons Only (MicroBooNE-AC, CD & FD)">
////        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Electrons_Only_CD_Dir, "02_Beta_vs_P_2p_Electrons_Only_CD.png",
////                      beta_electron, "Electrons", true);
////
////        histPlotter2D(c1, Beta_vs_P_2p_Electrons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Electrons_Only_FD_Dir, "02_Beta_vs_P_2p_Electrons_Only_FD.png",
////                      beta_electron, "Electrons", true);
////        //</editor-fold>
////
////        //<editor-fold desc="Beta vs. P for Protons Only (MicroBooNE-AC, CD & FD)">
////        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Protons_Only_CD_Dir, "03_Beta_vs_P_2p_Protons_Only_CD.png",
////                      beta_proton, "Protons", true);
////
////        histPlotter2D(c1, Beta_vs_P_2p_Protons_Only_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
////                      plots, true, Beta_vs_P_2p_Protons_Only_FD_Dir, "03_Beta_vs_P_2p_Protons_Only_FD.png",
////                      beta_proton, "Protons", true);
////        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P plots (by charge, MicroBooNE-AC, CD & FD)">
//
//        //<editor-fold desc="Beta vs. P for q = +1 (CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true,
//                      Beta_vs_P_positive_particles_MicroBooNE_AC_CD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_AC_CD.png",
//                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
//
//        histPlotter2D(c1, Beta_vs_P_positive_particles_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_positive_particles_MicroBooNE_AC_FD_Dir, "01_Beta_vs_P_q_p1_MicroBooNE_AC_FD.png",
//                      beta_proton, "Protons", beta_Kplus, "Positive kaons", beta_piplus, "Positive pions", true);
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P for q = 0 (CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_AC_CD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_AC_CD.png",
//                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
//
//        histPlotter2D(c1, Beta_vs_P_neutral_particles_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_neutral_particles_MicroBooNE_AC_FD_Dir, "02_Beta_vs_P_q_0_MicroBooNE_AC_FD.png",
//                      beta_neutron, "Neutrons", beta_Kzero, "Neutral kaons", beta_pizero, "Neutral pions", true);
//        //</editor-fold>
//
//        //<editor-fold desc="Beta vs. P for q = -1 (CD & FD)">
//        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_AC_CD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_AC_CD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_AC_CD.png",
//                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
//
//        histPlotter2D(c1, Beta_vs_P_negative_particles_MicroBooNE_AC_FD, 0.06, true, 0.0425, 0.0425, 0.0425,
//                      plots, true, Beta_vs_P_negative_particles_MicroBooNE_AC_FD_Dir, "03_Beta_vs_P_q_m1_MicroBooNE_AC_FD.png",
//                      beta_Kminus, "Negative kaons", beta_piminus, "Negative pions", beta_electron, "Electrons", true);
//        //</editor-fold>
//
//        //</editor-fold>
//
//        //</editor-fold>

    } else {
        cout << "\n\nBeta vs. P plots are disabled by user.\n\n";
    } // end of Beta vs. P plot if
    //</editor-fold>

// ======================================================================================================================================================================
// Angle histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Angle histograms">
    if (Angle_plots) {
        cout << "\n\nPlotting Angle histograms...\n\n";

//  Theta_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Theta_e (no #(e) cut)">
        double Theta_e_integral = hTheta_e_All_e_FD->Integral();
//        double Theta_e_integral = hTheta_e_All_e_CD->Integral() + hTheta_e_All_e_FD->Integral();

//        histPlotter1D(c1, hTheta_e_All_e_CD, norm_Angle_plots, true, Theta_e_integral, "#theta_{e} of Outgoing Electron", "All Int., No #(e) Cut", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e, "Theta_e_All_Int_All_e", hTheta_e_All_e_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        histPlotter1D(c1, hTheta_e_All_e_FD, norm_Angle_plots, true, Theta_e_integral, "#theta_{e} of Outgoing Electron", "All Int., No #(e) Cut", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_e, "Theta_e_All_Int_All_e", hTheta_e_All_e_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
        //</editor-fold>

        //<editor-fold desc="Theta_e (1e cut)">
        double Theta_e_1e_integral = hTheta_e_1e_cut_FD->Integral();
//        double Theta_e_1e_integral = hTheta_e_1e_cut_CD->Integral() + hTheta_e_1e_cut_FD->Integral();

//        histPlotter1D(c1, hTheta_e_1e_cut_CD, norm_Angle_plots, true, Theta_e_1e_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e Cut", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e, "Theta_e_All_Int_1e_cut", hTheta_e_1e_cut_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
        histPlotter1D(c1, hTheta_e_1e_cut_FD, norm_Angle_plots, true, Theta_e_1e_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e Cut", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_e, "Theta_e_All_Int_1e_cut", hTheta_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
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
//        histPlotter1D(c1, hTheta_e_All_Int_1e2X_CD, norm_Angle_plots, true, Theta_e_1e_1e2X_integral, "#theta_{e} 1e2X", "All Int.", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "00_Theta_e_1e2X", hTheta_e_All_Int_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_QEL_1e2X_CD, norm_Angle_plots, true, Theta_e_1e_1e2X_QEL_integral, "#theta_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "01_Theta_e_1e2X_QEL_Only", hTheta_e_QEL_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_MEC_1e2X_CD, norm_Angle_plots, true, Theta_e_1e_1e2X_MEC_integral, "#theta_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "02_Theta_e_1e2X_MEC_Only", hTheta_e_MEC_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_RES_1e2X_CD, norm_Angle_plots, true, Theta_e_1e_1e2X_RES_integral, "#theta_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "03_Theta_e_1e2X_RES_Only", hTheta_e_RES_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//
//        histPlotter1D(c1, hTheta_e_DIS_1e2X_CD, norm_Angle_plots, true, Theta_e_1e_1e2X_DIS_integral, "#theta_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "04_Theta_e_1e2X_DIS_Only", hTheta_e_DIS_1e2X_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5, false);
//        </editor-fold>
//
//        <editor-fold desc="Theta_e (1e2X, FD)">
//        histPlotter1D(c1, hTheta_e_All_Int_1e2X_FD, norm_Angle_plots, true, Theta_e_1e_1e2X_integral, "#theta_{e} 1e2X", "All Int.", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "00_Theta_e_1e_cut_1e2X", hTheta_e_All_Int_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_QEL_1e2X_FD, norm_Angle_plots, true, Theta_e_1e_1e2X_QEL_integral, "#theta_{e} 1e2X", "QEL Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "01_Theta_e_1e2X_QEL_Only", hTheta_e_QEL_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_MEC_1e2X_FD, norm_Angle_plots, true, Theta_e_1e_1e2X_MEC_integral, "#theta_{e} 1e2X", "MEC Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "02_Theta_e_1e2X_MEC_Only", hTheta_e_MEC_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_RES_1e2X_FD, norm_Angle_plots, true, Theta_e_1e_1e2X_RES_integral, "#theta_{e} 1e2X", "RES Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "03_Theta_e_1e2X_RES_Only", hTheta_e_RES_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//
//        histPlotter1D(c1, hTheta_e_DIS_1e2X_FD, norm_Angle_plots, true, Theta_e_1e_1e2X_DIS_integral, "#theta_{e} 1e2X", "DIS Only", 0.06, 0.0425, 0.0425, plots, 2,
//                      false, true, sTheta_e, "04_Theta_e_1e2X_DIS_Only", hTheta_e_DIS_1e2X_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5, false);
//        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Theta_e (1e2p)">
        double Theta_e_All_Int_1e2p_integral = hTheta_e_All_Int_1e2p_FD->Integral();
        double Theta_e_QEL_1e2p_integral = hTheta_e_QEL_1e2p_FD->Integral();
        double Theta_e_MEC_1e2p_integral = hTheta_e_MEC_1e2p_FD->Integral();
        double Theta_e_RES_1e2p_integral = hTheta_e_RES_1e2p_FD->Integral();
        double Theta_e_DIS_1e2p_integral = hTheta_e_DIS_1e2p_FD->Integral();
        double Theta_e_All_Int_2p_integral = hTheta_e_All_Int_2p_FD->Integral();
        double Theta_e_QEL_2p_integral = hTheta_e_QEL_2p_FD->Integral();
        double Theta_e_MEC_2p_integral = hTheta_e_MEC_2p_FD->Integral();
        double Theta_e_RES_2p_integral = hTheta_e_RES_2p_FD->Integral();
        double Theta_e_DIS_2p_integral = hTheta_e_DIS_2p_FD->Integral();
//        double Theta_e_All_Int_1e2p_integral = hTheta_e_All_Int_1e2p_CD->Integral() + hTheta_e_All_Int_1e2p_FD->Integral();
//        double Theta_e_QEL_1e2p_integral = hTheta_e_QEL_1e2p_CD->Integral() + hTheta_e_QEL_1e2p_FD->Integral();
//        double Theta_e_MEC_1e2p_integral = hTheta_e_MEC_1e2p_CD->Integral() + hTheta_e_MEC_1e2p_FD->Integral();
//        double Theta_e_RES_1e2p_integral = hTheta_e_RES_1e2p_CD->Integral() + hTheta_e_RES_1e2p_FD->Integral();
//        double Theta_e_DIS_1e2p_integral = hTheta_e_DIS_1e2p_CD->Integral() + hTheta_e_DIS_1e2p_FD->Integral();
//        double Theta_e_All_Int_2p_integral = hTheta_e_All_Int_2p_CD->Integral() + hTheta_e_All_Int_2p_FD->Integral();
//        double Theta_e_QEL_2p_integral = hTheta_e_QEL_2p_CD->Integral() + hTheta_e_QEL_2p_FD->Integral();
//        double Theta_e_MEC_2p_integral = hTheta_e_MEC_2p_CD->Integral() + hTheta_e_MEC_2p_FD->Integral();
//        double Theta_e_RES_2p_integral = hTheta_e_RES_2p_CD->Integral() + hTheta_e_RES_2p_FD->Integral();
//        double Theta_e_DIS_2p_integral = hTheta_e_DIS_2p_CD->Integral() + hTheta_e_DIS_2p_FD->Integral();

        //<editor-fold desc="Theta_e 1e2p (CD)">
//        histPlotter1D(c1, hTheta_e_All_Int_1e2p_CD, norm_Angle_plots, true, Theta_e_All_Int_1e2p_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e2p", 0.06,
//                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_CD, "00_Theta_e_All_Int_1e2p", hTheta_e_All_Int_1e2p_CD_Dir, "CD", kBlue, true, true, true,
//                      false, true, 47.5, 87.5, false);
//        histPlotter1D(c1, hTheta_e_QEL_1e2p_CD, norm_Angle_plots, true, Theta_e_QEL_1e2p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sTheta_e_1e2p_CD, "01_Theta_e_QEL_Only_1e2p", hTheta_e_QEL_1e2p_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      47.5, 87.5, false);
//        histPlotter1D(c1, hTheta_e_MEC_1e2p_CD, norm_Angle_plots, true, Theta_e_MEC_1e2p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sTheta_e_1e2p_CD, "02_Theta_e_MEC_Only_1e2p", hTheta_e_MEC_1e2p_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      47.5, 87.5, false);
//        histPlotter1D(c1, hTheta_e_RES_1e2p_CD, norm_Angle_plots, true, Theta_e_RES_1e2p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sTheta_e_1e2p_CD, "03_Theta_e_RES_Only_1e2p", hTheta_e_RES_1e2p_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      47.5, 87.5, false);
//        histPlotter1D(c1, hTheta_e_DIS_1e2p_CD, norm_Angle_plots, true, Theta_e_DIS_1e2p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sTheta_e_1e2p_CD, "04_Theta_e_DIS_Only_1e2p", hTheta_e_DIS_1e2p_CD_Dir, "CD", kBlue, true, true, true, false, true,
//                      47.5, 87.5, false);
//
//        stackPlotter1D(c1, sTheta_e_1e2p_CD, norm_Angle_plots, "#theta_{e} of Outgoing Electron", "1e2p", plots, hTheta_e_All_Int_1e2p_CD, hTheta_e_QEL_1e2p_CD,
//                       hTheta_e_MEC_1e2p_CD, hTheta_e_RES_1e2p_CD, hTheta_e_DIS_1e2p_CD, "05_Theta_e_Stack_1e2p", hTheta_e_All_Int_1e2p_CD_Dir, "CD");
        //</editor-fold>

        //<editor-fold desc="Theta_e 1e2p (FD)">
        histPlotter1D(c1, hTheta_e_All_Int_1e2p_FD, norm_Angle_plots, true, Theta_e_All_Int_1e2p_integral, "#theta_{e} of Outgoing Electron", "All Int., 1e2p", 0.06,
                      0.0425, 0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "00_Theta_e_All_Int_1e2p", hTheta_e_All_Int_1e2p_FD_Dir, "FD", kBlue, true, true, true,
                      false, true, 17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_QEL_1e2p_FD, norm_Angle_plots, true, Theta_e_QEL_1e2p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "01_Theta_e_QEL_Only_1e2p", hTheta_e_QEL_1e2p_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_MEC_1e2p_FD, norm_Angle_plots, true, Theta_e_MEC_1e2p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "02_Theta_e_MEC_Only_1e2p", hTheta_e_MEC_1e2p_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_RES_1e2p_FD, norm_Angle_plots, true, Theta_e_RES_1e2p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "03_Theta_e_RES_Only_1e2p", hTheta_e_RES_1e2p_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      17.5, 22.5, false);
        histPlotter1D(c1, hTheta_e_DIS_1e2p_FD, norm_Angle_plots, true, Theta_e_DIS_1e2p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sTheta_e_1e2p_FD, "04_Theta_e_DIS_Only_1e2p", hTheta_e_DIS_1e2p_FD_Dir, "FD", kBlue, true, true, true, false, true,
                      17.5, 22.5, false);

        stackPlotter1D(c1, sTheta_e_1e2p_FD, norm_Angle_plots, "#theta_{e} of Outgoing Electron", "1e2p", plots, hTheta_e_All_Int_1e2p_FD, hTheta_e_QEL_1e2p_FD,
                       hTheta_e_MEC_1e2p_FD, hTheta_e_RES_1e2p_FD, hTheta_e_DIS_1e2p_FD, "05_Theta_e_Stack_1e2p", hTheta_e_All_Int_1e2p_FD_Dir, "FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e 2p (CD)">
//        histPlotter1D(c1, hTheta_e_All_Int_2p_CD, norm_Angle_plots, true, Theta_e_All_Int_2p_integral, "#theta_{e} of Outgoing Electron", "All Int., 2p", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e_2p_CD, "00_Theta_e_All_Int_2p", hTheta_e_All_Int_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5,
//                      false);
//        histPlotter1D(c1, hTheta_e_QEL_2p_CD, norm_Angle_plots, true, Theta_e_QEL_2p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e_2p_CD, "01_Theta_e_QEL_Only_2p", hTheta_e_QEL_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5,
//                      false);
//        histPlotter1D(c1, hTheta_e_MEC_2p_CD, norm_Angle_plots, true, Theta_e_MEC_2p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e_2p_CD, "02_Theta_e_MEC_Only_2p", hTheta_e_MEC_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5,
//                      false);
//        histPlotter1D(c1, hTheta_e_RES_2p_CD, norm_Angle_plots, true, Theta_e_RES_2p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e_2p_CD, "03_Theta_e_RES_Only_2p", hTheta_e_RES_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5,
//                      false);
//        histPlotter1D(c1, hTheta_e_DIS_2p_CD, norm_Angle_plots, true, Theta_e_DIS_2p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sTheta_e_2p_CD, "04_Theta_e_DIS_Only_2p", hTheta_e_DIS_2p_CD_Dir, "CD", kBlue, true, true, true, false, true, 47.5, 87.5,
//                      false);
//
//        stackPlotter1D(c1, sTheta_e_2p_CD, norm_Angle_plots, "#theta_{e} of Outgoing Electron", "2p", plots, hTheta_e_All_Int_2p_CD, hTheta_e_QEL_2p_CD,
//                       hTheta_e_MEC_2p_CD, hTheta_e_RES_2p_CD, hTheta_e_DIS_Only_2p_CD, "05_Theta_e_Stack_2p", hTheta_e_All_Int_2p_CD_Dir, "CD");
        //</editor-fold>

        //<editor-fold desc="Theta_e 2p (FD)">
        histPlotter1D(c1, hTheta_e_All_Int_2p_FD, norm_Angle_plots, true, Theta_e_All_Int_2p_integral, "#theta_{e} of Outgoing Electron", "All Int., 2p", 0.06, 0.0425,
                      0.0425,
                      plots, 2, false, true, sTheta_e_2p_FD, "00_Theta_e_All_Int_2p", hTheta_e_All_Int_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        histPlotter1D(c1, hTheta_e_QEL_2p_FD, norm_Angle_plots, true, Theta_e_QEL_2p_integral, "#theta_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_e_2p_FD, "01_Theta_e_QEL_Only_2p", hTheta_e_QEL_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        histPlotter1D(c1, hTheta_e_MEC_2p_FD, norm_Angle_plots, true, Theta_e_MEC_2p_integral, "#theta_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_e_2p_FD, "02_Theta_e_MEC_Only_2p", hTheta_e_MEC_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        histPlotter1D(c1, hTheta_e_RES_2p_FD, norm_Angle_plots, true, Theta_e_RES_2p_integral, "#theta_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_e_2p_FD, "03_Theta_e_RES_Only_2p", hTheta_e_RES_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);
        histPlotter1D(c1, hTheta_e_DIS_2p_FD, norm_Angle_plots, true, Theta_e_DIS_2p_integral, "#theta_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sTheta_e_2p_FD, "04_Theta_e_DIS_Only_2p", hTheta_e_DIS_2p_FD_Dir, "FD", kBlue, true, true, true, false, true, 17.5, 22.5,
                      false);

        stackPlotter1D(c1, sTheta_e_2p_FD, norm_Angle_plots, "#theta_{e} of Outgoing Electron", "2p", plots, hTheta_e_All_Int_2p_FD, hTheta_e_QEL_2p_FD,
                       hTheta_e_MEC_2p_FD, hTheta_e_RES_2p_FD, hTheta_e_DIS_2p_FD, "05_Theta_e_Stack_2p", hTheta_e_All_Int_2p_FD_Dir, "FD");
        //</editor-fold>

        //</editor-fold>

//  Phi_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Phi_e (no #(e) cut)">
        double Phi_e_integral = hPhi_e_All_e_FD->Integral();
//        double Phi_e_integral = hPhi_e_All_e_CD->Integral() + hPhi_e_All_e_FD->Integral();

//        histPlotter1D(c1, hPhi_e_All_e_CD, norm_Angle_plots, true, Phi_e_integral, "#phi_{e} of Outgoing Electron", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sPhi_e, "Phi_e_All_Int_All_e", hPhi_e_All_e_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_All_e_FD, norm_Angle_plots, true, Phi_e_integral, "#phi_{e} of Outgoing Electron", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sPhi_e, "Phi_e_All_Int_All_e", hPhi_e_All_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Phi_e (1e cut)">
        double Phi_e_1e_integral = hPhi_e_1e_cut_FD->Integral();
//        double Phi_e_1e_integral = hPhi_e_1e_cut_CD->Integral() + hPhi_e_1e_cut_FD->Integral();

//        histPlotter1D(c1, hPhi_e_1e_cut_CD, norm_Angle_plots, true, Phi_e_1e_integral, "#phi_{e} of Outgoing Electron", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sPhi_e, "Phi_e_All_Int_1e_cut", hPhi_e_1e_cut_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_1e_cut_FD, norm_Angle_plots, true, Phi_e_1e_integral, "#phi_{e} of Outgoing Electron", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sPhi_e, "Phi_e_All_Int_1e_cut", hPhi_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

//        //<editor-fold desc="Phi_e (1e2X)">
//        double Phi_e_1e_1e2X_integral = hPhi_e_1e2X_CD->Integral() + hPhi_e_1e2X_FD->Integral();
//        double Phi_e_1e_1e2X_QEL_integral = hPhi_e_1e2X_QEL_CD->Integral() + hPhi_e_1e2X_QEL_FD->Integral();
//        double Phi_e_1e_1e2X_MEC_integral = hPhi_e_1e2X_MEC_CD->Integral() + hPhi_e_1e2X_MEC_FD->Integral();
//        double Phi_e_1e_1e2X_RES_integral = hPhi_e_1e2X_RES_CD->Integral() + hPhi_e_1e2X_RES_FD->Integral();
//        double Phi_e_1e_1e2X_DIS_integral = hPhi_e_1e2X_DIS_CD->Integral() + hPhi_e_1e2X_DIS_FD->Integral();
//
//        //<editor-fold desc="Phi_e (1e2X, CD)">
//        histPlotter1D(c1, hPhi_e_1e2X_CD, norm_Angle_plots, true, Phi_e_1e_1e2X_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2X", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "00_phi_e_All_Int_1e2X", hPhi_e_1e2X_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_QEL_CD, norm_Angle_plots, true, Phi_e_1e_1e2X_QEL_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2X", hPhi_e_1e2X_QEL_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_MEC_CD, norm_Angle_plots, true, Phi_e_1e_1e2X_MEC_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2X", hPhi_e_1e2X_MEC_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_RES_CD, norm_Angle_plots, true, Phi_e_1e_1e2X_RES_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2X", hPhi_e_1e2X_RES_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_DIS_CD, norm_Angle_plots, true, Phi_e_1e_1e2X_DIS_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1e2X", hPhi_e_1e2X_DIS_CD_Dir, "CD", kBlue, true, true, true);
//        //</editor-fold>
//
//        //<editor-fold desc="Phi_e (1e2X, FD)">
//        histPlotter1D(c1, hPhi_e_1e2X_FD, norm_Angle_plots, true, Phi_e_1e_1e2X_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2X", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "00_phi_e_All_Int_1e2X", hPhi_e_1e2X_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_QEL_FD, norm_Angle_plots, true, Phi_e_1e_1e2X_QEL_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2X", hPhi_e_1e2X_QEL_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_MEC_FD, norm_Angle_plots, true, Phi_e_1e_1e2X_MEC_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2X", hPhi_e_1e2X_MEC_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_RES_FD, norm_Angle_plots, true, Phi_e_1e_1e2X_RES_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1e2X", 0.06, 0.0425, 0.0425,
//                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2X", hPhi_e_1e2X_RES_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1e2X_DIS_FD, norm_Angle_plots, true, Phi_e_1e_1e2X_DIS_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1e2X", 0.06, 0.0425, 0.0425,
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
//        double Phi_e_All_Int_1e2p_integral = hPhi_e_All_Int_1e2p_CD->Integral() + hPhi_e_All_Int_1e2p_FD->Integral();
//        double Phi_e_QEL_1e2p_integral = hPhi_e_1eQEL_2p_CD->Integral() + hPhi_e_QEL_1e2p_FD->Integral();
//        double Phi_e_MEC_1e2p_integral = hPhi_e_1eMEC_2p_CD->Integral() + hPhi_e_MEC_1e2p_FD->Integral();
//        double Phi_e_RES_1e2p_integral = hPhi_e_1eRES_2p_CD->Integral() + hPhi_e_RES_1e2p_FD->Integral();
//        double Phi_e_DIS_1e2p_integral = hPhi_e_1eDIS_2p_CD->Integral() + hPhi_e_DIS_1e2p_FD->Integral();

//        //<editor-fold desc="Phi_e (1e2p, CD)">
//        histPlotter1D(c1, hPhi_e_All_Int_1e2p_CD, norm_Angle_plots, true, Phi_e_All_Int_1e2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sPhi_e, "00_Phi_e_All_Int_1e2p", hPhi_e_All_Int_1e2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1eQEL_2p_CD, norm_Angle_plots, true, Phi_e_QEL_1e2p_integral, "#phi_{e} of Outgoing Electron 1e2p", "QEL Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2p", hPhi_e_1eQEL_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1eMEC_2p_CD, norm_Angle_plots, true, Phi_e_MEC_1e2p_integral, "#phi_{e} of Outgoing Electron 1e2p", "MEC Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2p", hPhi_e_1eMEC_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1eRES_2p_CD, norm_Angle_plots, true, Phi_e_RES_1e2p_integral, "#phi_{e} of Outgoing Electron 1e2p", "RES Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2p", hPhi_e_1eRES_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_1eDIS_2p_CD, norm_Angle_plots, true, Phi_e_DIS_1e2p_integral, "#phi_{e} of Outgoing Electron 1e2p", "DIS Only, 1e2p", 0.06, 0.0425,
//                      0.0425, plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1e2p", hPhi_e_1eDIS_2p_CD_Dir, "CD", kBlue, true, true, true);
//        //</editor-fold>

        //<editor-fold desc="Phi_e (1e2p, FD)">
        histPlotter1D(c1, hPhi_e_All_Int_1e2p_FD, norm_Angle_plots, true, Phi_e_All_Int_1e2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 1e2p", 0.06, 0.0425,
                      0.0425, plots, 2, false, true, sPhi_e, "00_Phi_e_All_Int_1e2p", hPhi_e_All_Int_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_1e2p_FD, norm_Angle_plots, true, Phi_e_QEL_1e2p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "01_Phi_e_QEL_Only_1e2p", hPhi_e_QEL_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_1e2p_FD, norm_Angle_plots, true, Phi_e_MEC_1e2p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "02_Phi_e_MEC_Only_1e2p", hPhi_e_MEC_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_1e2p_FD, norm_Angle_plots, true, Phi_e_RES_1e2p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "03_Phi_e_RES_Only_1e2p", hPhi_e_RES_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_1e2p_FD, norm_Angle_plots, true, Phi_e_DIS_1e2p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 1e2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "04_Phi_e_DIS_Only_1e2p", hPhi_e_DIS_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

        //<editor-fold desc="Phi_e (2p)">
        double Phi_e_All_Int_2p_integral = hPhi_e_All_Int_2p_FD->Integral();
        double Phi_e_QEL_2p_integral = hPhi_e_QEL_2p_FD->Integral();
        double Phi_e_MEC_2p_integral = hPhi_e_MEC_2p_FD->Integral();
        double Phi_e_RES_2p_integral = hPhi_e_RES_2p_FD->Integral();
        double Phi_e_DIS_2p_integral = hPhi_e_DIS_2p_FD->Integral();
//        double Phi_e_All_Int_2p_integral = hPhi_e_All_Int_2p_CD->Integral() + hPhi_e_All_Int_2p_FD->Integral();
//        double Phi_e_QEL_2p_integral = hPhi_e_QEL_2p_CD->Integral() + hPhi_e_QEL_2p_FD->Integral();
//        double Phi_e_MEC_2p_integral = hPhi_e_MEC_2p_CD->Integral() + hPhi_e_MEC_2p_FD->Integral();
//        double Phi_e_RES_2p_integral = hPhi_e_RES_2p_CD->Integral() + hPhi_e_RES_2p_FD->Integral();
//        double Phi_e_DIS_2p_integral = hPhi_e_DIS_2p_CD->Integral() + hPhi_e_DIS_2p_FD->Integral();

//        //<editor-fold desc="Phi_e (2p, CD)">
//        histPlotter1D(c1, hPhi_e_All_Int_2p_CD, norm_Angle_plots, true, Phi_e_All_Int_2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 2p", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "00_phi_e_All_Int_2p", hPhi_e_All_Int_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_QEL_2p_CD, norm_Angle_plots, true, Phi_e_QEL_2p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "01_Phi_e_QEL_Only_2p", hPhi_e_QEL_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_MEC_2p_CD, norm_Angle_plots, true, Phi_e_MEC_2p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "02_Phi_e_MEC_Only_2p", hPhi_e_MEC_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_RES_2p_CD, norm_Angle_plots, true, Phi_e_RES_2p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "03_Phi_e_RES_Only_2p", hPhi_e_RES_2p_CD_Dir, "CD", kBlue, true, true, true);
//        histPlotter1D(c1, hPhi_e_DIS_2p_CD, norm_Angle_plots, true, Phi_e_DIS_2p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots,
//                      2, false, true, sPhi_e, "04_Phi_e_DIS_Only_2p", hPhi_e_DIS_2p_CD_Dir, "CD", kBlue, true, true, true);
//        //</editor-fold>

        //<editor-fold desc="Phi_e (2p, FD)">
        histPlotter1D(c1, hPhi_e_All_Int_2p_FD, norm_Angle_plots, true, Phi_e_All_Int_2p_integral, "#phi_{e} of Outgoing Electron", "All Int., 2p", 0.06, 0.0425, 0.0425,
                      plots, 2, false, true, sPhi_e, "00_phi_e_All_Int_cut_2p", hPhi_e_All_Int_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_QEL_2p_FD, norm_Angle_plots, true, Phi_e_QEL_2p_integral, "#phi_{e} of Outgoing Electron", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots,
                      2, false, true, sPhi_e, "01_Phi_e_QEL_Only_2p", hPhi_e_QEL_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_MEC_2p_FD, norm_Angle_plots, true, Phi_e_MEC_2p_integral, "#phi_{e} of Outgoing Electron", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots,
                      2, false, true, sPhi_e, "02_Phi_e_MEC_Only_2p", hPhi_e_MEC_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_RES_2p_FD, norm_Angle_plots, true, Phi_e_RES_2p_integral, "#phi_{e} of Outgoing Electron", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots,
                      2, false, true, sPhi_e, "03_Phi_e_RES_Only_2p", hPhi_e_RES_2p_FD_Dir, "FD", kBlue, true, true, true);
        histPlotter1D(c1, hPhi_e_DIS_2p_FD, norm_Angle_plots, true, Phi_e_DIS_2p_integral, "#phi_{e} of Outgoing Electron", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots,
                      2, false, true, sPhi_e, "04_Phi_e_DIS_Only_2p", hPhi_e_DIS_2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //</editor-fold>

//  Theta_e vs. Phi_e (CD & FD) --------------------------------------------------------------

        //<editor-fold desc="Theta_e vs. Phi_e (no #(e) cut, CD & FD)">
//        histPlotter2D(c1, hTheta_e_VS_Phi_e_All_e_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_All_e_CD_Dir, "Theta_e_VS_Phi_e_All_Int_All_e_CD");
        histPlotter2D(c1, hTheta_e_VS_Phi_e_All_e_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_All_e_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_All_e_FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e (1e cut, CD & FD)">
//        histPlotter2D(c1, hTheta_e_VS_Phi_e_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_1e_cut_CD_Dir,
//                      "Theta_e_VS_Phi_e_All_Int_1e_cut_CD");
        histPlotter2D(c1, hTheta_e_VS_Phi_e_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_1e_cut_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_1e_cut_FD");
        //</editor-fold>

        //<editor-fold desc="Theta_e vs. Phi_e (2p, CD & FD)">
//        histPlotter2D(c1, hTheta_e_VS_Phi_e_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_2p_CD_Dir, "Theta_e_VS_Phi_e_All_Int_2p_CD");
        histPlotter2D(c1, hTheta_e_VS_Phi_e_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hTheta_e_VS_Phi_e_2p_FD_Dir,
                      "Theta_e_VS_Phi_e_All_Int_2p_FD");
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
        double Q2_All_e_integral = hQ2_All_e_CD->Integral() + hQ2_All_e_FD->Integral();

        histPlotter1D(c1, hQ2_All_e_CD, norm_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sQ2_All_e, "Q2_All_e", hQ2_All_e_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hQ2_All_e_FD, norm_Q2_plots, true, Q2_All_e_integral, "Q^{2} Histogram", "no #(e) cut", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                      sQ2_All_e, "Q2_All_e", hQ2_All_e_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 (1e Cut, CD & FD)">
        double Q2_1e_integral = hQ2_1e_cut_CD->Integral() + hQ2_1e_cut_FD->Integral();

        histPlotter1D(c1, hQ2_1e_cut_CD, norm_Q2_plots, true, Q2_1e_integral, "Q^{2} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e_cut,
                      "Q2_1e_cut", hQ2_1e_cut_CD_Dir, "CD", kBlue, true, true, true);
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
        double Q2_1e2p_integral = hQ2_1e2p_CD->Integral() + hQ2_1e2p_FD->Integral();

        histPlotter1D(c1, hQ2_1e2p_CD, norm_Q2_plots, true, Q2_1e2p_integral, "Q^{2} Histogram", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e2p,
                      "Q2_1e2p", hQ2_1e2p_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hQ2_1e2p_FD, norm_Q2_plots, true, Q2_1e2p_integral, "Q^{2} Histogram", "1e2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_1e2p,
                      "Q2_1e2p", hQ2_1e2p_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Q2 (2p, CD & FD)">
        double Q2_2p_integral = hQ2_2p_CD->Integral() + hQ2_2p_FD->Integral();

        histPlotter1D(c1, hQ2_2p_CD, norm_Q2_plots, true, Q2_2p_integral, "Q^{2} Histogram", "2p", 0.06, 0.0425, 0.0425, plots, 2, false, true, sQ2_2p, "Q2_2p",
                      hQ2_2p_CD_Dir, "CD", kBlue, true, true, true);
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
//        double E_e_integral = hE_e_1e_cut_CD->Integral() + hE_e_1e_cut_FD->Integral();

//        histPlotter1D(c1, hE_e_1e_cut_CD, norm_E_e_plots, true, E_e_integral, "E_{e} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sE_e_1e_cut,
//                      "01_E_e_1e_cut", hE_e_1e_cut_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_1e_cut_FD, norm_E_e_plots, true, E_e_integral, "E_{e} Histogram", "1e Cut", 0.06, 0.0425, 0.0425, plots, 2, false, true, sE_e_1e_cut,
                      "01_E_e_1e_cut", hE_e_1e_cut_FD_Dir, "FD", kBlue, true, true, true);
        //</editor-fold>

        //<editor-fold desc="Energy (E_e) histograms (2p, CD & FD)">
        double E_e_2p_All_Int_integral = hE_e_All_Int_2p_FD->Integral();
        double E_e_QEL_Int_integral = hE_e_QEL_2p_FD->Integral();
        double E_e_MEC_Int_integral = hE_e_MEC_2p_FD->Integral();
        double E_e_RES_Int_integral = hE_e_RES_2p_FD->Integral();
        double E_e_DIS_Int_integral = hE_e_DIS_2p_FD->Integral();
//        double E_e_2p_All_Int_integral = hE_e_All_Int_2p_CD->Integral() + hE_e_All_Int_2p_FD->Integral();
//        double E_e_QEL_Int_integral = hE_e_QEL_2p_CD->Integral() + hE_e_QEL_2p_FD->Integral();
//        double E_e_MEC_Int_integral = hE_e_MEC_2p_CD->Integral() + hE_e_MEC_2p_FD->Integral();
//        double E_e_RES_Int_integral = hE_e_RES_2p_CD->Integral() + hE_e_RES_2p_FD->Integral();
//        double E_e_DIS_Int_integral = hE_e_DIS_2p_CD->Integral() + hE_e_DIS_2p_FD->Integral();

//        histPlotter1D(c1, hE_e_All_Int_2p_CD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sE_e_2p_CD, "00_E_e_All_Int_2p_CD", hE_e_All_Int_2p_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_All_Int_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "00_E_e_All_Int_2p_FD", hE_e_All_Int_2p_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hE_e_QEL_2p_CD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sE_e_2p_CD, "01_E_e_QEL_Only_2p_CD", hE_e_QEL_2p_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_QEL_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "01_E_e_QEL_Only_2p_FD", hE_e_QEL_2p_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hE_e_MEC_2p_CD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sE_e_2p_CD, "02_E_e_MEC_Only_2p_CD", hE_e_MEC_2p_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_MEC_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "02_E_e_MEC_Only_2p_FD", hE_e_MEC_2p_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hE_e_RES_2p_CD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sE_e_2p_CD, "03_E_e_RES_Only_2p_CD", hE_e_RES_2p_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_RES_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "03_E_e_RES_Only_2p_FD", hE_e_RES_2p_FD_Dir, "FD", kBlue, true, true, true);
//        histPlotter1D(c1, hE_e_DIS_2p_CD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
//                      true, sE_e_2p_CD, "04_E_e_DIS_Only_2p_CD", hE_e_DIS_2p_CD_Dir, "CD", kBlue, true, true, true);
        histPlotter1D(c1, hE_e_DIS_2p_FD, norm_E_e_plots, true, E_e_2p_All_Int_integral, "E_{e} Histogram", "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false,
                      true, sE_e_2p_FD, "04_E_e_DIS_Only_2p_FD", hE_e_DIS_2p_FD_Dir, "FD", kBlue, true, true, true);

//        stackPlotter1D(c1, sE_e_2p_CD, norm_E_e_plots, "E_{e} Histogram", "2p", plots, hE_e_All_Int_2p_CD, hE_e_QEL_2p_CD, hE_e_MEC_2p_CD, hE_e_RES_2p_CD,
//                       hE_e_DIS_2p_CD, "01_E_e_Stack", sE_e_2p_CD_Dir, "CD");
        stackPlotter1D(c1, sE_e_2p_FD, norm_E_e_plots, "E_{e} Histogram", "2p", plots, hE_e_All_Int_2p_FD, hE_e_QEL_2p_FD, hE_e_MEC_2p_FD, hE_e_RES_2p_FD,
                       hE_e_DIS_2p_FD, "02_E_e_Stack", sE_e_2p_FD_Dir, "FD");
        //</editor-fold>

//  E_e vs. Theta_e histograms --------------------------------------------------------------

        //<editor-fold desc="E_e vs. Theta_e histograms (1e cut, CD & FD)">
//        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_1e_cut_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_1e_cut_CD_Dir,
//                      "00_Theta_e_VS_Phi_e_All_Int_All_e_CD");
        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_1e_cut_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_All_e_FD");
        //</editor-fold>

        //<editor-fold desc="E_e vs. Theta_e histograms (2p, CD & FD)">
//        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_2p_CD_Dir,
//                      "00_E_e_VS_Theta_e_All_Int_2p_CD");
//        histPlotter2D(c1, hE_e_VS_Theta_e_QEL_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_QEL_2p_CD_Dir, "01_E_e_VS_Theta_e_QEL_Only_2p_CD");
//        histPlotter2D(c1, hE_e_VS_Theta_e_MEC_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_MEC_2p_CD_Dir, "02_E_e_VS_Theta_e_MEC_Only_2p_CD");
//        histPlotter2D(c1, hE_e_VS_Theta_e_RES_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_RES_2p_CD_Dir, "03_E_e_VS_Theta_e_RES_Only_2p_CD");
//        histPlotter2D(c1, hE_e_VS_Theta_e_DIS_2p_CD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_DIS_2p_CD_Dir, "04_E_e_VS_Theta_e_DIS_Only_2p_CD");

        histPlotter2D(c1, hE_e_VS_Theta_e_All_Int_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_All_Int_2p_FD_Dir,
                      "00_E_e_VS_Theta_e_All_Int_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_QEL_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_QEL_2p_FD_Dir, "01_E_e_VS_Theta_e_QEL_Only_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_MEC_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_MEC_2p_FD_Dir, "02_E_e_VS_Theta_e_MEC_Only_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_RES_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_RES_2p_FD_Dir, "03_E_e_VS_Theta_e_RES_Only_2p_FD");
        histPlotter2D(c1, hE_e_VS_Theta_e_DIS_2p_FD, 0.06, true, 0.0425, 0.0425, 0.0425, plots, true, hE_e_VS_Theta_e_DIS_2p_FD_Dir, "04_E_e_VS_Theta_e_DIS_Only_2p_FD");

//        //<editor-fold desc="E_e histograms (All Int., 2p, CD)">
//        hE_e_VS_Theta_e_All_Int_2p_CD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_All_Int_2p_CD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_All_Int_2p_CD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_All_Int_2p_CD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_All_Int_2p_CD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_All_Int_2p_CD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_All_Int_2p_CD);
//        hE_e_VS_Theta_e_All_Int_2p_CD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_All_Int_2p_CD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_All_Int_2p_CD_Dir + "00 E_e_VS_Theta_e_All_Int_2p_CD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (All Int., 2p, FD)">
//        hE_e_VS_Theta_e_All_Int_2p_FD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_All_Int_2p_FD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_All_Int_2p_FD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_All_Int_2p_FD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_All_Int_2p_FD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_All_Int_2p_FD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_All_Int_2p_FD);
//        hE_e_VS_Theta_e_All_Int_2p_FD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_All_Int_2p_FD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_All_Int_2p_FD_Dir + "00 E_e_VS_Theta_e_All_Int_2p_FD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (QEL only, 2p, CD)">
//        hE_e_VS_Theta_e_QEL_2p_CD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_QEL_2p_CD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_QEL_2p_CD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_QEL_2p_CD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_QEL_2p_CD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_QEL_2p_CD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_QEL_2p_CD);
//        hE_e_VS_Theta_e_QEL_2p_CD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_QEL_2p_CD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_QEL_2p_CD_Dir + "01_E_e_VS_Theta_e_QEL_only_2p_CD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (QEL only, 2p, FD)">
//        hE_e_VS_Theta_e_QEL_2p_FD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_QEL_2p_FD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_QEL_2p_FD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_QEL_2p_FD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_QEL_2p_FD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_QEL_2p_FD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_QEL_2p_FD);
//        hE_e_VS_Theta_e_QEL_2p_FD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_QEL_2p_FD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_QEL_2p_FD_Dir + "01_E_e_VS_Theta_e_QEL_only_2p_FD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (MEC only, 2p, CD)">
//        hE_e_VS_Theta_e_MEC_2p_CD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_MEC_2p_CD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_MEC_2p_CD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_MEC_2p_CD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_MEC_2p_CD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_MEC_2p_CD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_MEC_2p_CD);
//        hE_e_VS_Theta_e_MEC_2p_CD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_MEC_2p_CD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_MEC_2p_CD_Dir + "02_E_e_VS_Theta_e_MEC_only_2p_CD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (MEC only, 2p, FD)">
//        hE_e_VS_Theta_e_MEC_2p_FD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_MEC_2p_FD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_MEC_2p_FD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_MEC_2p_FD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_MEC_2p_FD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_MEC_2p_FD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_MEC_2p_FD);
//        hE_e_VS_Theta_e_MEC_2p_FD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_MEC_2p_FD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_MEC_2p_FD_Dir + "02_E_e_VS_Theta_e_MEC_only_2p_FD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (RES only, 2p, CD)">
//        hE_e_VS_Theta_e_RES_2p_CD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_RES_2p_CD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_RES_2p_CD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_RES_2p_CD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_RES_2p_CD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_RES_2p_CD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_RES_2p_CD);
//        hE_e_VS_Theta_e_RES_2p_CD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_RES_2p_CD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_RES_2p_CD_Dir + "03_E_e_VS_Theta_e_RES_only_2p_CD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (RES only, 2p, FD)">
//        hE_e_VS_Theta_e_RES_2p_FD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_RES_2p_FD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_RES_2p_FD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_RES_2p_FD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_RES_2p_FD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_RES_2p_FD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_RES_2p_FD);
//        hE_e_VS_Theta_e_RES_2p_FD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_RES_2p_FD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_RES_2p_FD_Dir + "03_E_e_VS_Theta_e_RES_only_2p_FD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (DIS only, 2p, CD)">
//        hE_e_VS_Theta_e_DIS_2p_CD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_DIS_2p_CD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_DIS_2p_CD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_DIS_2p_CD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_DIS_2p_CD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_DIS_2p_CD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_DIS_2p_CD);
//        hE_e_VS_Theta_e_DIS_2p_CD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_DIS_2p_CD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_DIS_2p_CD_Dir + "04_E_e_VS_Theta_e_DIS_only_2p_CD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

//        //<editor-fold desc="E_e histograms (DIS only, 2p, FD)">
//        hE_e_VS_Theta_e_DIS_2p_FD->SetTitleSize(0.06, "xyz");
//        hE_e_VS_Theta_e_DIS_2p_FD->GetXaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_DIS_2p_FD->GetXaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_DIS_2p_FD->GetYaxis()->SetLabelSize(0.0425);
//        hE_e_VS_Theta_e_DIS_2p_FD->GetYaxis()->CenterTitle(true);
//        hE_e_VS_Theta_e_DIS_2p_FD->GetZaxis()->SetLabelSize(0.0425);
//        plots->Add(hE_e_VS_Theta_e_DIS_2p_FD);
//        hE_e_VS_Theta_e_DIS_2p_FD->Draw("colz");
//        c1->SetLogz(1);
////        gStyle->SetStatX(0.87);
////        gStyle->SetStatY(0.4);
//        hE_e_VS_Theta_e_DIS_2p_FD->SetStats(0);
//        c1->SaveAs((hE_e_VS_Theta_e_DIS_2p_FD_Dir + "04_E_e_VS_Theta_e_DIS_only_2p_FD.png").c_str());
//        gStyle->SetStatX(DefStatX);
//        gStyle->SetStatY(DefStatY);
//        c1->Clear();
//        //</editor-fold>

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
            double ETrans15_All_Int_integral_2p = hET15_All_Int_2p_FD->Integral();
//            double ETrans15_All_Int_integral_2p = hET15_All_Int_2p_CD->Integral() + hET15_All_Int_2p_FD->Integral();

//            histPlotter1D(c1, hET15_All_Int_2p_CD, norm_ET_plots, true, ETrans15_All_Int_integral_2p, tET15, "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                          sET15_All_Int_2p_CD, "ETrans_15_All_Int_2p", hET15_All_Int_2p_CD_Dir, "CD", kBlack, true, true, true);
            histPlotter1D(c1, hET15_All_Int_2p_FD, norm_ET_plots, true, ETrans15_All_Int_integral_2p, tET15, "All Int., 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_All_Int_2p", hET15_All_Int_2p_FD_Dir, "FD", kBlack, true, true, true);
        }

        if (ETrans_QEL_plots) {
            double ETrans15_QEL_integral_2p = hET15_QEL_2p_FD->Integral();
//            double ETrans15_QEL_integral_2p = hET15_QEL_2p_CD->Integral() + hET15_QEL_2p_FD->Integral();

//            histPlotter1D(c1, hET15_QEL_2p_CD, norm_ET_plots, true, ETrans15_QEL_integral_2p, tET15, "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                          sET15_All_Int_2p_CD, "ETrans_15_QEL_Only_2p", hET15_QEL_2p_CD_Dir, "CD", kBlue, true, true, true);
            histPlotter1D(c1, hET15_QEL_2p_FD, norm_ET_plots, true, ETrans15_QEL_integral_2p, tET15, "QEL Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_QEL_Only_2p", hET15_QEL_2p_FD_Dir, "FD", kBlue, true, true, true);
        }

        if (ETrans_MEC_plots) {
            double ETrans15_MEC_integral_2p = hET15_MEC_2p_FD->Integral();
//            double ETrans15_MEC_integral_2p = hET15_MEC_2p_CD->Integral() + hET15_MEC_2p_FD->Integral();

//            histPlotter1D(c1, hET15_MEC_2p_CD, norm_ET_plots, true, ETrans15_MEC_integral_2p, tET15, "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                          sET15_All_Int_2p_CD, "ETrans_15_MEC_Only_2p", hET15_MEC_2p_CD_Dir, "CD", kRed, true, true, true);
            histPlotter1D(c1, hET15_MEC_2p_FD, norm_ET_plots, true, ETrans15_MEC_integral_2p, tET15, "MEC Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_MEC_Only_2p", hET15_MEC_2p_FD_Dir, "FD", kRed, true, true, true);
        }

        if (ETrans_RES_plots) {
            double ETrans15_RES_integral_2p = hET15_RES_2p_FD->Integral();
//            double ETrans15_RES_integral_2p = hET15_RES_2p_CD->Integral() + hET15_RES_2p_FD->Integral();

//            histPlotter1D(c1, hET15_RES_2p_CD, norm_ET_plots, true, ETrans15_RES_integral_2p, tET15, "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                          sET15_All_Int_2p_CD, "ETrans_15_RES_2p", hET15_RES_2p_CD_Dir, "CD", kGreen, true, true, true);
            histPlotter1D(c1, hET15_RES_2p_FD, norm_ET_plots, true, ETrans15_RES_integral_2p, tET15, "RES Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_RES_2p", hET15_RES_2p_FD_Dir, "FD", kGreen, true, true, true);
        }

        if (ETrans_DIS_plots) {
            double ETrans15_DIS_integral_2p = hET15_DIS_2p_FD->Integral();
//            double ETrans15_DIS_integral_2p = hET15_DIS_2p_CD->Integral() + hET15_DIS_2p_FD->Integral();

//            histPlotter1D(c1, hET15_DIS_2p_CD, norm_ET_plots, true, ETrans15_DIS_integral_2p, tET15, "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
//                          sET15_All_Int_2p_CD, "ETrans_15_DIS_2p", hET15_DIS_2p_CD_Dir, "CD", kMagenta, true, true, true);
            histPlotter1D(c1, hET15_DIS_2p_FD, norm_ET_plots, true, ETrans15_DIS_integral_2p, tET15, "DIS Only, 2p", 0.06, 0.0425, 0.0425, plots, 2, false, true,
                          sET15_All_Int_2p_FD, "ETrans_15_DIS_2p", hET15_DIS_2p_FD_Dir, "FD", kMagenta, true, true, true);
        }

//  Energy transfer around 15 deg stack (2p only) ------------------------------------------------------

        if (ETrans_All_Int_plots && ETrans_QEL_plots && ETrans_MEC_plots && ETrans_RES_plots && ETrans_DIS_plots) {
//            stackPlotter1D(c1, sET15_All_Int_2p_CD, norm_ET_plots, tET15, "2p", plots, hET15_All_Int_2p_CD, hET15_QEL_2p_CD, hET15_MEC_2p_CD,
//                           hET15_RES_2p_CD, hET15_DIS_2p_CD, "01_ET15_stack", sET15_All_Int_2p_CD_Dir, "CD");
            stackPlotter1D(c1, sET15_All_Int_2p_FD, norm_ET_plots, tET15, "2p", plots, hET15_All_Int_2p_FD, hET15_QEL_2p_FD, hET15_MEC_2p_FD,
                           hET15_RES_2p_FD, hET15_DIS_2p_FD, "02_ET15_stack", sET15_All_Int_2p_FD_Dir, "FD");
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
    } else {
        cout << "\n\nEcal plots are disabled by user.\n\n";
    }
    //</editor-fold>

// ======================================================================================================================================================================
// MicroBooNE article histogram reconstructions
// ======================================================================================================================================================================

    //<editor-fold desc="MicroBooNE article histogram reconstructions">
    if (MicroBooNE_plots) {

        cout << "\n\nPlotting MicroBooNE histograms...\n\n";

// Momentum plots -------------------------------------------------------------------------------------

//        P_L_hist->Draw();
//        plots->Add(P_L_hist);
//        P_L_hist->SetLineWidth(2);
//        P_L_hist->GetXaxis()->CenterTitle(true);
//        P_L_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_L_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_L_histogram_linear_scale.png");
//        c1->Clear();
//
//        P_R_hist->Draw();
//        plots->Add(P_R_hist);
//        P_R_hist->SetLineWidth(2);
//        P_R_hist->GetXaxis()->CenterTitle(true);
//        P_R_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_R_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_R_histogram_linear_scale.png");
//        c1->Clear();
//
//        P_lp_hist->Draw();
//        plots->Add(P_lp_hist);
//        P_lp_hist->SetLineWidth(2);
//        P_lp_hist->GetXaxis()->CenterTitle(true);
//        P_lp_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_lp_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_lp_histogram_linear_scale.png");
//        c1->Clear();
//
//        P_pion_hist->Draw();
//        plots->Add(P_pion_hist);
//        P_pion_hist->SetLineWidth(2);
//        P_pion_hist->GetXaxis()->CenterTitle(true);
//        P_pion_hist->SetLineColor(kBlue);
//        c1->SetLogy(1);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_pion_histogram_log_scale.png");
//        c1->SetLogy(0);
//        c1->SaveAs("plots/MicroBooNE_plots/momentum_distributions/P_pion_histogram_linear_scale.png");
//        c1->Clear();

// Unweighted plots -----------------------------------------------------------------------------

//        hGamma_Lab_All_Int->Draw();
//        hGamma_Lab_All_Int->SetTitleSize(0.06);
//        hGamma_Lab_All_Int->GetXaxis()->SetLabelSize(0.0425);
//        hGamma_Lab_All_Int->GetXaxis()->CenterTitle(true);
//        hGamma_Lab_All_Int->GetYaxis()->SetLabelSize(0.0425);
////        hGamma_Lab_All_Int->GetYaxis()->SetLimits(0., 3.);
//        plots->Add(hGamma_Lab_All_Int);
//        hGamma_Lab_All_Int->SetLineWidth(2);
//        hGamma_Lab_All_Int->SetLineColor(kBlue);
////        hGamma_Lab_All_Int->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_Lab.png");
//        c1->Clear();
//
//        hGamma_mu_p_tot->Draw();
//        hGamma_mu_p_tot->SetTitleSize(0.06);
//        hGamma_mu_p_tot->GetXaxis()->SetLabelSize(0.0425);
//        hGamma_mu_p_tot->GetXaxis()->CenterTitle(true);
//        hGamma_mu_p_tot->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(hGamma_mu_p_tot);
//        hGamma_mu_p_tot->SetLineWidth(2);
//        hGamma_mu_p_tot->SetLineColor(kBlue);
////        hGamma_mu_p_tot->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_mu_p_tot.png");
//        c1->Clear();
//
//        dP_T_hist->Draw();
//        dP_T_hist->SetTitleSize(0.06);
//        dP_T_hist->GetXaxis()->SetLabelSize(0.0425);
//        dP_T_hist->GetXaxis()->CenterTitle(true);
//        dP_T_hist->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(dP_T_hist);
//        dP_T_hist->SetLineWidth(2);
//        dP_T_hist->SetLineColor(kBlue);
////        dP_T_hist->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/dP_T_histogram.png");
//        c1->Clear();
//
//        hGamma_Lab_All_Int_noPions->Draw();
//        hGamma_Lab_All_Int_noPions->SetTitleSize(0.06);
//        hGamma_Lab_All_Int_noPions->GetXaxis()->SetLabelSize(0.0425);
//        hGamma_Lab_All_Int_noPions->GetXaxis()->CenterTitle(true);
//        hGamma_Lab_All_Int_noPions->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(hGamma_Lab_All_Int_noPions);
//        hGamma_Lab_All_Int_noPions->SetLineWidth(2);
//        hGamma_Lab_All_Int_noPions->SetLineColor(kBlue);
////        hGamma_Lab_All_Int_noPions->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/all_interactions/gamma_Lab_all.png");
//        c1->Clear();

        /*
        hGamma_Lab_QEL_noPions->Draw();
        hGamma_Lab_QEL_noPions->SetTitleSize(0.06);
        hGamma_Lab_QEL_noPions->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_QEL_noPions->GetXaxis()->CenterTitle(true);
        hGamma_Lab_QEL_noPions->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_QEL_noPions);
        hGamma_Lab_QEL_noPions->SetLineWidth(2);
        hGamma_Lab_QEL_noPions->SetLineColor(kBlue);
//        hGamma_Lab_QEL_noPions->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/QEL_only/gamma_Lab_QEL.png");
        c1->Clear();

        hGamma_Lab_MEC_noPions->Draw();
        hGamma_Lab_MEC_noPions->SetTitleSize(0.06);
        hGamma_Lab_MEC_noPions->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_MEC_noPions->GetXaxis()->CenterTitle(true);
        hGamma_Lab_MEC_noPions->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_MEC_noPions);
        hGamma_Lab_MEC_noPions->SetLineWidth(2);
        hGamma_Lab_MEC_noPions->SetLineColor(kBlue);
//        hGamma_Lab_MEC_noPions->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/MEC_only/gamma_Lab_MEC.png");
        c1->Clear();

        hGamma_Lab_RES_noPions->Draw();
        hGamma_Lab_RES_noPions->SetTitleSize(0.06);
        hGamma_Lab_RES_noPions->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_RES_noPions->GetXaxis()->CenterTitle(true);
        hGamma_Lab_RES_noPions->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_RES_noPions);
        hGamma_Lab_RES_noPions->SetLineWidth(2);
        hGamma_Lab_RES_noPions->SetLineColor(kBlue);
//        hGamma_Lab_RES_noPions->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/RES_only/gamma_Lab_RES.png");
        c1->Clear();

        hGamma_Lab_DIS_noPions->Draw();
        hGamma_Lab_DIS_noPions->SetTitleSize(0.06);
        hGamma_Lab_DIS_noPions->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_DIS_noPions->GetXaxis()->CenterTitle(true);
        hGamma_Lab_DIS_noPions->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_DIS_noPions);
        hGamma_Lab_DIS_noPions->SetLineWidth(2);
        hGamma_Lab_DIS_noPions->SetLineColor(kBlue);
//        hGamma_Lab_DIS_noPions->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/DIS_only/gamma_Lab_DIS.png");
        c1->Clear();
        */

// Weighted plots -------------------------------------------------------------------------------

//        hGamma_Lab_weighted->Draw();
//        hGamma_Lab_weighted->Sumw2();
//        hGamma_Lab_weighted->SetTitleSize(0.06);
//        hGamma_Lab_weighted->GetXaxis()->SetLabelSize(0.0425);
//        hGamma_Lab_weighted->GetXaxis()->CenterTitle(true);
//        hGamma_Lab_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(hGamma_Lab_weighted);
//        hGamma_Lab_weighted->SetLineWidth(2);
//        hGamma_Lab_weighted->SetLineColor(kBlue);
//        hGamma_Lab_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_Lab_weighted.png");
//        c1->Clear();
//
//        hGamma_mu_p_tot_weighted->Draw();
//        hGamma_mu_p_tot_weighted->Sumw2();
//        hGamma_mu_p_tot_weighted->SetTitleSize(0.06);
//        hGamma_mu_p_tot_weighted->GetXaxis()->SetLabelSize(0.0425);
//        hGamma_mu_p_tot_weighted->GetXaxis()->CenterTitle(true);
//        hGamma_mu_p_tot_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(hGamma_mu_p_tot_weighted);
//        hGamma_mu_p_tot_weighted->SetLineWidth(2);
//        hGamma_mu_p_tot_weighted->SetLineColor(kBlue);
//        hGamma_mu_p_tot_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_mu_p_tot_weighted.png");
//        c1->Clear();
//
//        dP_T_hist_weighted->Draw();
//        dP_T_hist_weighted->Sumw2();
//        dP_T_hist_weighted->SetTitleSize(0.06);
//        dP_T_hist_weighted->GetXaxis()->SetLabelSize(0.0425);
//        dP_T_hist_weighted->GetXaxis()->CenterTitle(true);
//        dP_T_hist_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(dP_T_hist_weighted);
//        dP_T_hist_weighted->SetLineWidth(2);
//        dP_T_hist_weighted->SetLineColor(kBlue);
//        dP_T_hist_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/dP_T_histogram_weighted.png");
//        c1->Clear();
//
//
//        hGamma_Lab_All_Int_noPions_weighted->Draw();
//        hGamma_Lab_All_Int_noPions_weighted->Sumw2();
//        hGamma_Lab_All_Int_noPions_weighted->SetTitleSize(0.06);
//        hGamma_Lab_All_Int_noPions_weighted->GetXaxis()->SetLabelSize(0.0425);
//        hGamma_Lab_All_Int_noPions_weighted->GetXaxis()->CenterTitle(true);
//        hGamma_Lab_All_Int_noPions_weighted->GetYaxis()->SetLabelSize(0.0425);
//        plots->Add(hGamma_Lab_All_Int_noPions_weighted);
//        hGamma_Lab_All_Int_noPions_weighted->SetLineWidth(2);
//        hGamma_Lab_All_Int_noPions_weighted->SetLineColor(kBlue);
////        hGamma_Lab_All_Int_noPions_weighted->SetStats(0);
//        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/all_interactions/gamma_Lab_all_weighted.png");
//        c1->Clear();

        /*
        hGamma_Lab_QEL_noPions_weighted->Draw();
        hGamma_Lab_QEL_noPions_weighted->Sumw2();
        hGamma_Lab_QEL_noPions_weighted->SetTitleSize(0.06);
        hGamma_Lab_QEL_noPions_weighted->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_QEL_noPions_weighted->GetXaxis()->CenterTitle(true);
        hGamma_Lab_QEL_noPions_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_QEL_noPions_weighted);
        hGamma_Lab_QEL_noPions_weighted->SetLineWidth(2);
        hGamma_Lab_QEL_noPions_weighted->SetLineColor(kBlue);
//        hGamma_Lab_QEL_noPions_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/QEL_only/gamma_Lab_QEL_weighted.png");
        c1->Clear();

        hGamma_Lab_MEC_noPions_weighted->Draw();
        hGamma_Lab_MEC_noPions_weighted->Sumw2();
        hGamma_Lab_MEC_noPions_weighted->SetTitleSize(0.06);
        hGamma_Lab_MEC_noPions_weighted->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_MEC_noPions_weighted->GetXaxis()->CenterTitle(true);
        hGamma_Lab_MEC_noPions_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_MEC_noPions_weighted);
        hGamma_Lab_MEC_noPions_weighted->SetLineWidth(2);
        hGamma_Lab_MEC_noPions_weighted->SetLineColor(kBlue);
//        hGamma_Lab_MEC_noPions_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/MEC_only/gamma_Lab_MEC_weighted.png");
        c1->Clear();

        hGamma_Lab_RES_noPions_weighted->Draw();
        hGamma_Lab_RES_noPions_weighted->Sumw2();
        hGamma_Lab_RES_noPions_weighted->SetTitleSize(0.06);
        hGamma_Lab_RES_noPions_weighted->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_RES_noPions_weighted->GetXaxis()->CenterTitle(true);
        hGamma_Lab_RES_noPions_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_RES_noPions_weighted);
        hGamma_Lab_RES_noPions_weighted->SetLineWidth(2);
        hGamma_Lab_RES_noPions_weighted->SetLineColor(kBlue);
//        hGamma_Lab_RES_noPions_weighted->SetStats(0);
        c1->SaveAs("plots/MicroBooNE_plots/gamma_lab/RES_only/gamma_Lab_RES_weighted.png");
        c1->Clear();

        hGamma_Lab_DIS_noPions_weighted->Draw();
        hGamma_Lab_DIS_noPions_weighted->Sumw2();
        hGamma_Lab_DIS_noPions_weighted->SetTitleSize(0.06);
        hGamma_Lab_DIS_noPions_weighted->GetXaxis()->SetLabelSize(0.0425);
        hGamma_Lab_DIS_noPions_weighted->GetXaxis()->CenterTitle(true);
        hGamma_Lab_DIS_noPions_weighted->GetYaxis()->SetLabelSize(0.0425);
        plots->Add(hGamma_Lab_DIS_noPions_weighted);
        hGamma_Lab_DIS_noPions_weighted->SetLineWidth(2);
        hGamma_Lab_DIS_noPions_weighted->SetLineColor(kBlue);
//        hGamma_Lab_DIS_noPions_weighted->SetStats(0);
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
    myLogFile << "Beta_vs_P_plots = " << BoolToString(Beta_vs_P_plots) << "\n";
    myLogFile << "Theta_e_plots = " << BoolToString(Theta_e_plots) << "\n";
    myLogFile << "Phi_e_plots = " << BoolToString(Phi_e_plots) << "\n";
    myLogFile << "Q2_plots = " << BoolToString(Q2_plots) << "\n";
    myLogFile << "E_e_plots = " << BoolToString(E_e_plots) << "\n";
    myLogFile << "ETrans_all_plots = " << BoolToString(ETrans_all_plots) << "\n";
    myLogFile << "ETrans_QEL_plots = " << BoolToString(ETrans_QEL_plots) << "\n";
    myLogFile << "ETrans_MEC_plots = " << BoolToString(ETrans_MEC_plots) << "\n";
    myLogFile << "ETrans_RES_plots = " << BoolToString(ETrans_RES_plots) << "\n";
    myLogFile << "ETrans_DIS_plots = " << BoolToString(ETrans_DIS_plots) << "\n";
    myLogFile << "E_cal_plots = " << BoolToString(Ecal_plots) << "\n";
    myLogFile << "MicroBooNE_plots = " << BoolToString(MicroBooNE_plots) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Normalization settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "normalize_master = " << BoolToString(normalize_master) << "\n\n";

    myLogFile << "norm_Nphe_plots = " << BoolToString(norm_Nphe_plots) << "\n";
    myLogFile << "norm_Chi2_plots = " << BoolToString(norm_Chi2_plots) << "\n";
    myLogFile << "norm_Vertex_plots = " << BoolToString(norm_Vertex_plots) << "\n";
    myLogFile << "norm_SF_plots = " << BoolToString(norm_SF_plots) << "\n";
    myLogFile << "norm_Fiducial_plots = " << BoolToString(norm_Fiducial_plots) << "\n\n";

    myLogFile << "norm_Angle_plots = " << BoolToString(norm_Angle_plots) << "\n";
    myLogFile << "norm_Q2_plots = " << BoolToString(norm_Q2_plots) << "\n";
    myLogFile << "norm_E_e_plots = " << BoolToString(norm_E_e_plots) << "\n";
    myLogFile << "norm_ET_plots = " << BoolToString(norm_ET_plots) << "\n";
    myLogFile << "norm_Ecal_plots = " << BoolToString(norm_Ecal_plots) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Delete settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "delete_png_files = " << BoolToString(delete_png_files) << "\n";
    myLogFile << "delete_root_files = " << BoolToString(delete_root_files) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Cut application settings\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "apply_cuts = " << BoolToString(apply_cuts) << "\n\n";

    myLogFile << "apply_Nphe_cut = " << BoolToString(apply_Nphe_cut) << "\n";
    myLogFile << "apply_chi2_cuts_2p = " << BoolToString(apply_chi2_cuts_2p) << "\n";
    myLogFile << "apply_chi2_cuts_MicroBooNE = " << BoolToString(apply_chi2_cuts_MicroBooNE) << "\n";
    myLogFile << "apply_Vz_cuts = " << BoolToString(apply_Vz_cuts) << "\n";
    myLogFile << "apply_dVz_cuts = " << BoolToString(apply_dVz_cuts) << "\n";
    myLogFile << "apply_SF_cuts = " << BoolToString(apply_SF_cuts) << "\n";
    myLogFile << "apply_ECAL_fiducial_cut = " << BoolToString(apply_ECAL_fiducial_cut) << "\n";
    myLogFile << "apply_DC_fiducial_cut = " << BoolToString(apply_DC_fiducial_cut) << "\n";
    myLogFile << "apply_momentum_cuts_2p = " << BoolToString(apply_momentum_cuts_2p) << "\n";
    myLogFile << "apply_momentum_cuts_1n1p = " << BoolToString(apply_momentum_cuts_1n1p) << "\n";
    myLogFile << "apply_momentum_cuts_MicroBooNE = " << BoolToString(apply_momentum_cuts_MicroBooNE) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Momentum thresholds (2p)\n";
    myLogFile << "===========================================================================\n\n";
    myLogFile << "e_mom_cuts_2p.at(0) = " << e_mom_cuts_2p.at(0) << "\n";
    myLogFile << "e_mom_cuts_2p.at(1) = " << e_mom_cuts_2p.at(1) << "\n";
    myLogFile << "P_L_lower_lim_2p* = " << p_mom_cuts_2p.at(0) << "\n";
    myLogFile << "P_L_upper_lim_2p* = " << p_mom_cuts_2p.at(1) << "\n";
    myLogFile << "P_R_lower_lim_2p* = " << p_mom_cuts_2p.at(0) << "\n\n\n";
    myLogFile << "P_R_upper_lim_2p* = " << p_mom_cuts_2p.at(1) << "\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Momentum thresholds (MicroBooNE)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "e_momentum_lower_cut_MicroBooNE = " << e_mom_cuts_MicroBooNE.at(0) << "\n";
    myLogFile << "e_momentum_upper_cut_MicroBooNE = " << e_mom_cuts_MicroBooNE.at(1) << "\n";
    myLogFile << "p_momentum_lower_cut_MicroBooNE = " << p_mom_cuts_MicroBooNE.at(0) << "\n";
    myLogFile << "p_momentum_upper_cut_MicroBooNE = " << p_mom_cuts_MicroBooNE.at(1) << "\n";
    myLogFile << "cpion_momentum_lower_cut_MicroBooNE = " << cpion_mom_cuts_MicroBooNE.at(0) << "\n\n\n";
    myLogFile << "cpion_momentum_upper_cut_MicroBooNE = " << cpion_mom_cuts_MicroBooNE.at(1) << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Sampling Fraction (SF) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_SF_cuts = " << BoolToString(apply_SF_cuts) << "\n";
    myLogFile << "SF_1e_upper_cut = " << SF_1e_upper_cut << "\n";
    myLogFile << "SF_1e_lower_cut = " << SF_1e_lower_cut << "\n";
    myLogFile << "SF_1e2p_Xmax (from histogram) = " << SF_1e2p_Xmax << "\n";
    myLogFile << "SF_1e2p_peak (used in cuts) = " << SF_1e2p_peak << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "ECAL fiducial cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_ECAL_fiducial_cut = " << BoolToString(apply_ECAL_fiducial_cut) << "\n";
    myLogFile << "fiducial_cut_Lv = " << fiducial_cut_Lv << "\n";
    myLogFile << "fiducial_cut_Lw = " << fiducial_cut_Lw << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Number of Photo-electrons (Nphe) cuts (electrons only, FD)\n";
    myLogFile << "===========================================================================\n\n";

    myLogFile << "apply_Nphe_cut = " << BoolToString(apply_Nphe_cut) << "\n";
    myLogFile << "Nphe_lower_cut = " << clasAna.getNpheCuts() << "\n\n\n";

    myLogFile << "===========================================================================\n";
    myLogFile << "Chi2 cuts\n";
    myLogFile << "===========================================================================\n\n";

    //<editor-fold desc="Chi2_Electron">
    myLogFile << "\n-- Electrons --------------------------------------------------------------" << "\n\n";

//    myLogFile << "\n-- Electrons in CD (no #e cuts) -------------------------------------------" << "\n";
//    myLogFile << "Chi2_Electron_cut_CD = " << Chi2_Electron_cut_CD << "\n";
//
//    if (hChi2_Electron_CD->Integral() == 0.) {
//        myLogFile << "Chi2_Electron_Xmax_CD (from histogram) = NO EVENTS" << "\n";
//    } else {
//        myLogFile << "Chi2_Electron_Xmax_CD (from histogram) = " << Chi2_Electron_Xmax_CD << "\n";
//    }

    myLogFile << "\n-- Electrons in FD (no #e cuts) -------------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_cut_FD = " << Chi2_Electron_cut_FD << "\n";

    if (hChi2_Electron_FD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_Xmax_FD (from histogram) = " << Chi2_Electron_Xmax_FD << "\n";
    }

//    myLogFile << "\n-- Electrons in CD (1e cut) -----------------------------------------------" << "\n";
//    myLogFile << "Chi2_Electron_1e_cut_CD = " << Chi2_Electron_cut_CD << "\n";
//
//    if (hChi2_Electron_1e_cut_CD->Integral() == 0.) {
//        myLogFile << "Chi2_Electron_1e_Xmax_CD (from histogram) = NO EVENTS" << "\n";
//    } else {
//        myLogFile << "Chi2_Electron_1e_Xmax_CD (from histogram) = " << Chi2_Electron_1e_Xmax_CD << "\n";
//    }

    myLogFile << "Chi2_Electron_1e_peak_CD (used in cuts) = " << Chi2_Electron_1e_peak_CD << "\n";

    myLogFile << "\n-- Electrons in FD (1e cut) -----------------------------------------------" << "\n";
    myLogFile << "Chi2_Electron_1e_cut_FD = " << Chi2_Electron_cut_FD << "\n";

    if (hChi2_Electron_1e_cut_FD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_1e_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_1e_Xmax_FD (from histogram) = " << Chi2_Electron_1e_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Electron_1e_peak_FD (used in cuts) = " << Chi2_Electron_1e_peak_FD << "\n";

//    myLogFile << "\n-- Electrons in CD (MicroBooNE cut) ---------------------------------------" << "\n";
//    myLogFile << "Chi2_Electron_cut_MicroBooNE_CD = " << Chi2_Electron_cut_MicroBooNE_CD << "\n";
//
//    if (hChi2_Electron_MicroBooNE_CD->Integral() == 0.) {
//        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_CD (from histogram) = NO EVENTS" << "\n";
//    } else {
//        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_CD (from histogram) = " << Chi2_Electron_MicroBooNE_Xmax_CD << "\n";
//    }

    myLogFile << "Chi2_Electron_1e_peak_MicroBooNE_CD (used in cuts) = " << Chi2_Electron_1e_peak_MicroBooNE_CD << "\n";

    myLogFile << "\n-- Electrons in FD (MicroBooNE cut) ---------------------------------------" << "\n";

    myLogFile << "Chi2_Electron_cut_MicroBooNE_FD = " << Chi2_Electron_cut_MicroBooNE_FD << "\n";

    if (hChi2_Electron_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Electron_MicroBooNE_Xmax_FD (from histogram) = " << Chi2_Electron_MicroBooNE_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Electron_1e_peak_MicroBooNE_FD (used in cuts) = " << Chi2_Electron_1e_peak_MicroBooNE_FD << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_Proton">
    myLogFile << "\n\n-- Protons ----------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Protons in CD (no #e cuts) ---------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_CD = " << Chi2_Proton_cut_CD << "\n";

    if (hChi2_Proton_CD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_Xmax_CD (from histogram) = " << Chi2_Proton_Xmax_CD << "\n";
    }

    myLogFile << "Chi2 Proton mean CD (from histogram) = " << to_string_with_precision(hChi2_Proton_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Protons in FD (no #e cuts) ---------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_FD = " << Chi2_Proton_cut_FD << "\n";

    if (hChi2_Proton_FD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_Xmax_FD (from histogram) = " << Chi2_Proton_Xmax_FD << "\n";
    }

    myLogFile << "Chi2 Proton mean FD (from histogram) = " << to_string_with_precision(hChi2_Proton_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Protons in CD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_1e_cut_CD = " << Chi2_Proton_cut_CD << "\n";

    if (hChi2_Proton_1e_cut_CD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_1e_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_1e_Xmax_CD (from histogram) = " << Chi2_Proton_1e_Xmax_CD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_CD (used in cuts) = " << Chi2_Proton_1e_peak_CD << "\n";
    myLogFile << "Chi2 Proton 1e mean CD (from histogram) = " << to_string_with_precision(hChi2_Proton_1e_cut_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Protons in FD (1e cut) -------------------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_1e_cut_FD = " << Chi2_Proton_cut_FD << "\n";

    if (hChi2_Proton_1e_cut_FD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_1e_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_1e_Xmax_FD (from histogram) = " << Chi2_Proton_1e_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_FD (used in cuts) = " << Chi2_Proton_1e_peak_FD << "\n";
    myLogFile << "Chi2 Proton 1e mean FD (from histogram) = " << to_string_with_precision(hChi2_Proton_1e_cut_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Protons in CD (MicroBooNE cut) -----------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_MicroBooNE_CD = " << Chi2_Proton_cut_MicroBooNE_CD << "\n";

    if (hChi2_Proton_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_CD (from histogram) = " << Chi2_Proton_MicroBooNE_Xmax_CD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_MicroBooNE_CD (used in cuts) = " << Chi2_Proton_1e_peak_MicroBooNE_CD << "\n";
    myLogFile << "Chi2 Proton 1e MicroBooNE mean CD (from histogram) = " << to_string_with_precision(hChi2_Proton_MicroBooNE_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Protons in FD (MicroBooNE cut) -----------------------------------------" << "\n";
    myLogFile << "Chi2_Proton_cut_MicroBooNE_FD = " << Chi2_Proton_cut_MicroBooNE_FD << "\n";

    if (hChi2_Proton_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_Proton_MicroBooNE_Xmax_FD (from histogram) = " << Chi2_Proton_MicroBooNE_Xmax_FD << "\n";
    }

    myLogFile << "Chi2_Proton_1e_peak_MicroBooNE_FD (used in cuts) = " << Chi2_Proton_1e_peak_MicroBooNE_FD << "\n";
    myLogFile << "Chi2 Proton 1e MicroBooNE mean FD (from histogram) = " << to_string_with_precision(hChi2_Proton_MicroBooNE_FD->GetMean(), 20) << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_Kplus">
    myLogFile << "\n\n-- K+ --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kplus in CD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kplus_Xmax_CD (used in cuts) = " << Chi2_Kplus_Xmax_CD << "\n";
    myLogFile << "Chi2 K+ mean CD (from histogram) = " << to_string_with_precision(hChi2_Kplus_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Kplus in FD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kplus_Xmax_FD (used in cuts) = " << Chi2_Kplus_Xmax_FD << "\n";
    myLogFile << "Chi2 K+ mean FD (from histogram) = " << to_string_with_precision(hChi2_Kplus_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Kplus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kplus_1e_Xmax_CD (used in cuts) = " << Chi2_Kplus_1e_Xmax_CD << "\n";
    myLogFile << "Chi2 K+ mean CD (from histogram) = " << to_string_with_precision(hChi2_Kplus_1e_cut_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Kplus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kplus_1e_Xmax_FD (used in cuts) = " << Chi2_Kplus_1e_Xmax_FD << "\n";
    myLogFile << "Chi2 K+ mean FD (from histogram) = " << to_string_with_precision(hChi2_Kplus_1e_cut_FD->GetMean(), 20) << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_Kminus">
    myLogFile << "\n\n-- K- --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- Kminus in CD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kminus_Xmax_CD (used in cuts) = " << Chi2_Kminus_Xmax_CD << "\n";
    myLogFile << "Chi2 K- mean CD (from histogram) = " << to_string_with_precision(hChi2_Kminus_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Kminus in FD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kminus_Xmax_FD (used in cuts) = " << Chi2_Kminus_Xmax_FD << "\n";
    myLogFile << "Chi2 K- mean FD (from histogram) = " << to_string_with_precision(hChi2_Kminus_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Kminus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kminus_1e_Xmax_CD (used in cuts) = " << Chi2_Kminus_1e_Xmax_CD << "\n";
    myLogFile << "Chi2 K- mean CD (from histogram) = " << to_string_with_precision(hChi2_Kminus_1e_cut_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- Kminus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_Kminus_1e_Xmax_FD (used in cuts) = " << Chi2_Kminus_1e_Xmax_FD << "\n";
    myLogFile << "Chi2 K- mean FD (from histogram) = " << to_string_with_precision(hChi2_Kminus_1e_cut_FD->GetMean(), 20) << "\n";
//</editor-fold>

    //<editor-fold desc="Chi2_piplus">
    myLogFile << "\n\n-- pi+ --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piplus in CD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_piplus_Xmax_CD (used in cuts) = " << Chi2_piplus_Xmax_CD << "\n";
    myLogFile << "Chi2 pi+ mean CD (from histogram) = " << to_string_with_precision(hChi2_piplus_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piplus in FD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_piplus_Xmax_FD (used in cuts) = " << Chi2_piplus_Xmax_FD << "\n";
    myLogFile << "Chi2 pi+ mean FD (from histogram) = " << to_string_with_precision(hChi2_piplus_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piplus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_piplus_1e_Xmax_CD (used in cuts) = " << Chi2_piplus_1e_Xmax_CD << "\n";
    myLogFile << "Chi2 pi+ mean CD (from histogram) = " << to_string_with_precision(hChi2_piplus_1e_cut_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piplus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_piplus_1e_Xmax_FD (used in cuts) = " << Chi2_piplus_1e_Xmax_FD << "\n";
    myLogFile << "Chi2 pi+ mean FD (from histogram) = " << to_string_with_precision(hChi2_piplus_1e_cut_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piplus in CD (MicroBooNE cut) ------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_CD = " << Chi2_piminus_cut_MicroBooNE_CD << "\n";

    if (hChi2_piplus_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_CD (from histogram) = " << Chi2_piplus_1e_peak_MicroBooNE_CD << "\n";
    }

    myLogFile << "Chi2_piplus_MicroBooNE_Xmax_CD (used in cuts) = " << Chi2_piplus_MicroBooNE_Xmax_CD << "\n";
    myLogFile << "Chi2 pi+ MicroBooNE mean CD (from histogram) = " << to_string_with_precision(hChi2_piplus_MicroBooNE_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piplus in FD (MicroBooNE cut) ------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_FD = " << Chi2_piminus_cut_MicroBooNE_FD << "\n";

    if (hChi2_piplus_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piplus_1e_peak_MicroBooNE_FD (from histogram) = " << Chi2_piplus_1e_peak_MicroBooNE_FD << "\n";
    }

    myLogFile << "Chi2_piplus_MicroBooNE_Xmax_FD (used in cuts) = " << Chi2_piplus_MicroBooNE_Xmax_FD << "\n";
    myLogFile << "Chi2 pi+ MicroBooNE mean FD (from histogram) = " << to_string_with_precision(hChi2_piplus_MicroBooNE_FD->GetMean(), 20) << "\n";
    //</editor-fold>

    //<editor-fold desc="Chi2_piminus">
    myLogFile << "\n\n-- pi- --------------------------------------------------------------------" << "\n\n";

    myLogFile << "\n-- piminus in CD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_Xmax_CD (used in cuts) = " << Chi2_piminus_Xmax_CD << "\n";
    myLogFile << "Chi2 pi+ mean CD (from histogram) = " << to_string_with_precision(hChi2_piminus_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piminus in FD -----------------------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_Xmax_FD (used in cuts) = " << Chi2_piminus_Xmax_FD << "\n";
    myLogFile << "Chi2 pi+ mean FD (from histogram) = " << to_string_with_precision(hChi2_piminus_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piminus in CD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_1e_Xmax_CD (used in cuts) = " << Chi2_piminus_1e_Xmax_CD << "\n";
    myLogFile << "Chi2 pi- mean CD (from histogram) = " << to_string_with_precision(hChi2_piminus_1e_cut_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piminus in FD (1e cut) --------------------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_1e_Xmax_FD (used in cuts) = " << Chi2_piminus_1e_Xmax_FD << "\n";
    myLogFile << "Chi2 pi- mean FD (from histogram) = " << to_string_with_precision(hChi2_piminus_1e_cut_FD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piminus in CD (MicroBooNE cut) -----------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_CD = " << Chi2_piminus_cut_MicroBooNE_CD << "\n";

    if (hChi2_piminus_MicroBooNE_CD->Integral() == 0.) {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_CD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_CD (from histogram) = " << Chi2_piminus_1e_peak_MicroBooNE_CD << "\n";
    }

    myLogFile << "Chi2_piminus_MicroBooNE_Xmax_CD (used in cuts) = " << Chi2_piminus_MicroBooNE_Xmax_CD << "\n";
    myLogFile << "Chi2 pi- MicroBooNE mean CD (from histogram) = " << to_string_with_precision(hChi2_piminus_MicroBooNE_CD->GetMean(), 20) << "\n";

    myLogFile << "\n-- piminus in FD (MicroBooNE cut) -----------------------------------------" << "\n";
    myLogFile << "Chi2_piminus_cut_MicroBooNE_FD = " << Chi2_piminus_cut_MicroBooNE_FD << "\n";

    if (hChi2_piminus_MicroBooNE_FD->Integral() == 0.) {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_FD (from histogram) = NO EVENTS" << "\n";
    } else {
        myLogFile << "Chi2_piminus_1e_peak_MicroBooNE_FD (from histogram) = " << Chi2_piminus_1e_peak_MicroBooNE_FD << "\n";
    }

    myLogFile << "Chi2_piminus_MicroBooNE_Xmax_FD (used in cuts) = " << Chi2_piminus_MicroBooNE_Xmax_FD << "\n\n\n";
    myLogFile << "Chi2 pi- MicroBooNE mean FD (from histogram) = " << to_string_with_precision(hChi2_piminus_MicroBooNE_FD->GetMean(), 20) << "\n";
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
    myLogFile << "#(events) 2p QEL:\t\t\t" << num_of_QEL_2p_events << "\n";
    myLogFile << "#(events) 2p MEC:\t\t\t" << num_of_MEC_2p_events << "\n";
    myLogFile << "#(events) 2p RES:\t\t\t" << num_of_RES_2p_events << "\n";
    myLogFile << "#(events) 2p DIS:\t\t\t" << num_of_DIS_2p_events << "\n";
    myLogFile << "QEL + MEC + RES + DIS (2p):\t\t\t" << num_of_QEL_2p_events + num_of_MEC_2p_events + num_of_RES_2p_events + num_of_DIS_2p_events << "\n\n\n";

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
    cout << "#(events) 2p QEL:\t\t\t" << num_of_QEL_2p_events << "\n";
    cout << "#(events) 2p MEC:\t\t\t" << num_of_MEC_2p_events << "\n";
    cout << "#(events) 2p RES:\t\t\t" << num_of_RES_2p_events << "\n";
    cout << "#(events) 2p DIS:\t\t\t" << num_of_DIS_2p_events << "\n";
    cout << "QEL + MEC + RES + DIS (2p):\t\t" << num_of_QEL_2p_events + num_of_MEC_2p_events + num_of_RES_2p_events + num_of_DIS_2p_events << "\n\n";

//    cout << "-- MicroBooNE event counts ------------------------------------------------\n";
//    cout << "#(events) MicroBooNE BEFORE cuts:\t" << num_of_MicroBooNE_events_BC << "\n";
//    cout << "#(events) MicroBooNE BC with Neutrons:\t" << num_of_MicroBooNE_events_BC_wNeutrons << "\n";
//    cout << "#(events) MicroBooNE BC with pi0:\t" << num_of_MicroBooNE_events_BC_wpi0 << "\n";
//    cout << "#(events) MicroBooNE BC with pi+:\t" << num_of_MicroBooNE_events_BC_wpip << "\n";
//    cout << "#(events) MicroBooNE BC with pi-:\t" << num_of_MicroBooNE_events_BC_wpim << "\n\n";

//    cout << "#(events) MicroBooNE AFTER cuts:\t" << num_of_MicroBooNE_events_AC << "\n";
//    cout << "#(events) MicroBooNE AC with Neutrons:\t" << num_of_MicroBooNE_events_AC_wNeutrons << "\n";
//    cout << "#(events) MicroBooNE AC with pi0:\t" << num_of_MicroBooNE_events_AC_wpi0 << "\n";
//    cout << "#(events) MicroBooNE AC with pi+:\t" << num_of_MicroBooNE_events_AC_wpip << "\n";
//    cout << "#(events) MicroBooNE AC with pi-:\t" << num_of_MicroBooNE_events_AC_wpim << "\n\n";

    cout << "---------------------------------------------------------------------------\n";
    cout << "\t\t\tExecution variables\n";
    cout << "---------------------------------------------------------------------------\n\n";

    cout << "WorkingDirectory:\t" << WorkingDirectory << "\n";
    cout << "plots_path:\t\t" << plots_path << "\n\n";

    cout << "AnalyseFilePath:\t" << "/" << AnalyseFilePath << "/" << "\n";
    cout << "AnalyseFileSample:\t" << "/" << AnalyseFileSample << "/" << "\n";
    cout << "AnalyseFile:\t\t" << AnalyseFile << "\n\n";

    cout << "apply_cuts:\t\t'" << BoolToString(apply_cuts) << "'\n";
    cout << "Settings mode:\t\t'" << file_name << "'\n\n";

    cout << "Target:\t\t\t" << Target << " (PDG: " << TargetPDG << ")\n";
    cout << "Beam Energy:\t\t" << beamE << "\n\n";

    cout << "Operation finished (AnalyserVersion = " << AnalyserVersion << ")." << "\n\n";
    //</editor-fold>

    //</editor-fold>

}