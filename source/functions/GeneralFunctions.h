//
// Created by alons on 27/01/2023.
//

#ifndef DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H
#define DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H

//TODO: recheck which of these #include-s are needed and which aren't

#include <stdio.h>
#include <unistd.h>

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>

#include <cstdlib>
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
#include <iomanip>

#include "clas12reader.h"

using namespace clas12;
using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                      General functions                                                                              //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// GetCurrentDirectory function -----------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetCurrentDirectory function">
/* Usage: get the directory of the main.c code */

string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    string WorkingDirectory = pwd;

    return WorkingDirectory;
}
//</editor-fold>

// findSubstring function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="findSubstring function">
/* Usage: in getBeanE */

bool findSubstring(string string1, string string2) {
    if (string1.find(string2) != string::npos) {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

// getSampleName function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="getSampleName function">
/* Usage: get SampleName from AnalyseFileSample. It will be used in creating a custom cuts file */

string getSampleName(string AnalyseFilePath, string AnalyseFileSample) {
    string SampleName;

    if (AnalyseFilePath == "mnt/d/e4nu/hipo_data_files") { // C12, simulation, 6GeV, Q205, T5, local
        if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_10") {
            SampleName = "C12_simulation_6GeV_T5_first_10";
//            SampleName = "C12_simulation_Q205_598636MeV_T5_first_10";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5_first_100") {
            SampleName = "C12_simulation_6GeV_T5_first_100";
//            SampleName = "C12_simulation_Q205_598636MeV_T5_first_100";
        } else if (AnalyseFileSample == "recon_qe_GENIE_C_598636MeV_Q2_0_5_test_5") {
            SampleName = "C12_simulation_6GeV_T5";
//            SampleName = "C12_simulation_Q205_598636MeV_T5";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/598636MeV_Q2_0_5_test_5") { // C12, simulation, 6GeV, Q205, T5, ifarm
        if (AnalyseFileSample == "reconhipo") {
            SampleName = "C12_simulation_6GeV_T5";
//            SampleName = "C12_simulation_Q205_598636MeV_T5";
        }
    } else if (AnalyseFilePath == "volatile/clas12/users/esteejus/Simulation_sigmaCM") { // C, simulation, 6GeV, ifarm
        if (AnalyseFileSample == "reconhipo") {
            SampleName = "C_simulation_Justin_6GeV";
//            SampleName = "C_simulation_Justin_598636MeV";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/Ca48_G18_10a_02_11b_Q205_598636MeV") { // Ca48, simulation, 6GeV, Q205, ifarm
        if (AnalyseFileSample == "reconhipo") {
            SampleName = "Ca48_simulation_G18_10a_02_11b_6GeV";
//            SampleName = "Ca48_simulation_G18_10a_02_11b_Q205_598636MeV";
        }
    } else if (AnalyseFilePath == "lustre19/expphy/volatile/clas12/asportes/simulationFiles/H1_G18_10a_02_11b_Q205_598636MeV") { // H1, simulation, 6GeV, Q205, ifarm
        if (AnalyseFileSample == "reconhipo") {
            SampleName = "H1_simulation_G18_10a_02_11b_6GeV";
        }
    } else if (AnalyseFilePath == "volatile/clas12/rg-m/48Ca/dst/recon") { // Ca48, data, 6GeV, ifarm
        if (AnalyseFileSample == "015832") {
            SampleName = "Ca48_data_6GeV_run_015832";
//            SampleName = "Ca48_data_598636MeV_run_015832";
        } else if (AnalyseFileSample == "015843") {
            SampleName = "Ca48_data_6GeV_run_015843";
//            SampleName = "Ca48_data_598636MeV_run_015843";
        } else if (AnalyseFileSample == "015852") {
            SampleName = "Ca48_data_6GeV_run_015843";
//            SampleName = "Ca48_data_598636MeV_run_015843";
        } else if (AnalyseFileSample == "015854") {
            SampleName = "Ca48_data_6GeV_run_015854";
//            SampleName = "Ca48_data_598636MeV_run_015854";
        }
    } else if (AnalyseFilePath == "cache/hallb/scratch/rg-m/LH2/8.6.0") { // LH2, data, 6GeV, ifarm
        if (AnalyseFileSample == "015032") {
            SampleName = "LH2_data_6GeV_run_015032";
//            SampleName = "LH2_data_598636MeV_run_015032";
        }
    } else {
        SampleName = "unknown_target_598636MeV"; // to set beamE = 5.98636 by default
    }

    return SampleName;
}
//</editor-fold>

// getBeanE function ----------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="getBeanE function">
/* Usage: get beamE from SampleName */

double getBeanE(string SampleName) {
    double beamE;

    if ((findSubstring(SampleName, "598636MeV") == true) || (findSubstring(SampleName, "598636mev") == true)
        || (findSubstring(SampleName, "598636") == true) || (findSubstring(SampleName, "6GeV") == true)) {
        beamE = 5.98636;
    } else if ((findSubstring(SampleName, "402962MeV") == true) || (findSubstring(SampleName, "402962mev") == true)
               || (findSubstring(SampleName, "402962") == true) || (findSubstring(SampleName, "4GeV") == true)) {
        beamE = 4.02962;
    } else if ((findSubstring(SampleName, "207052MeV") == true) || (findSubstring(SampleName, "207052mev") == true)
               || (findSubstring(SampleName, "207052") == true) || (findSubstring(SampleName, "2GeV") == true)) {
        beamE = 2.07052;
    }

    return beamE;
}
//</editor-fold>

// to_string_with_precision function ------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="to_string_with_precision function">
/* Usage: convert a number to string with n figures after the decimal point in the plotting functions */

template<typename T>
string to_string_with_precision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
//</editor-fold>

// TFolderAdder function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="TFolderAdder function (regular)">
/* Usage: recursively create TFolder and sub-folders in TList. */

void TFolderAdder(TFolder *Histogram_List_Folder, string Plots_Parent_Folder, string Plots_Daughter_Folder) {
    if (Plots_Daughter_Folder != "") {
        if (Plots_Daughter_Folder.find_first_of('/') != 0) {
            int number = Plots_Daughter_Folder.find_first_of('/');
            string Daughter_Folder_Name = Plots_Daughter_Folder.substr(0, number);

            Histogram_List_Folder->AddFolder(Daughter_Folder_Name.c_str(), Plots_Parent_Folder.c_str());

            if (Plots_Daughter_Folder.size() - Daughter_Folder_Name.size() > 0) {
                TFolder *pf = (TFolder *) Histogram_List_Folder->FindObject(Daughter_Folder_Name.c_str());

                TFolderAdder(pf, Daughter_Folder_Name, Plots_Daughter_Folder.substr(number + 1, Plots_Daughter_Folder.size() - 1));
//            TFolderAdder(Histogram_list, Plots_Parent_Folder, Plots_Daughter_Folder.substr(number + 1, Plots_Daughter_Folder.size() - 1));
            }
        }
    }

//    TFolder *pf = (TFolder *) Histogram_list->FindObject(Plots_Parent_Folder.c_str());

//    Histogram_list->Add(List_Parent_Folder);
}
//</editor-fold>

//<editor-fold desc="TFolderAdder function (for cuts)">
/* Usage: recursively create TFolder and sub-folders in TList. */

void TFolderAdder(TFolder *Histogram_List_Folder, string Plots_Parent_Folder, string Plots_Daughter_Folder, bool cuts_TFolder) {
    if (Plots_Daughter_Folder != "") {
        if ((Plots_Parent_Folder.find_first_of('/') <= Plots_Parent_Folder.size()) && cuts_TFolder) {
            int number0 = Plots_Parent_Folder.find_first_of('/');
            string Plots_Parent_Folder_Name = Plots_Parent_Folder.substr(number0 + 1, Plots_Parent_Folder.size());
            TFolderAdder(Histogram_List_Folder, Plots_Parent_Folder_Name, Plots_Daughter_Folder, true);
        } else {
            if (Plots_Daughter_Folder.find_first_of('/') != 0) {
                int number = Plots_Daughter_Folder.find_first_of('/');
                string Daughter_Folder_Name = Plots_Daughter_Folder.substr(0, number);
                Histogram_List_Folder->AddFolder(Daughter_Folder_Name.c_str(), Plots_Parent_Folder.c_str());

                if (Plots_Daughter_Folder.size() - Daughter_Folder_Name.size() > 0) {
                    TFolder *pf = (TFolder *) Histogram_List_Folder->FindObject(Daughter_Folder_Name.c_str());

                    TFolderAdder(pf, Daughter_Folder_Name, Plots_Daughter_Folder.substr(number + 1, Plots_Daughter_Folder.size() - 1));
                }
            }
        }
    }
}
//</editor-fold>

// GetTFolder function ---------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetTFolder function (regular)">
/* Usage: recursively create TFolder and sub-folders in TList. */

void GetTFolder(TList *HistogramTList, string Plots_Parent_Folder, string Plots_Daughter_Folder) {
    string TFolder_path = Plots_Parent_Folder + "/" + Plots_Daughter_Folder;
    TFolder *TFolder_ptr = (TFolder *) HistogramTList->FindObject(TFolder_path.c_str());
//    TFolder_ptr->AddFolder("Daughter_Folder_Name.c_str()", "Daughter_Folder_Name.c_str()");

//    if (TFolder_path.find_first_of('/') <= TFolder_path.size()) {
//        int number = TFolder_path.find_last_of('/');
//        string Folder = TFolder_path.substr(number+1, TFolder_path.size());
////        int number = TFolder_path.find_first_of('/');
////        string Folder = TFolder_path.substr(0, number);
//        TFolder *TFolder_ptr = (TFolder *) HistogramTList->FindObject(Folder.c_str());
//
//        TFolder_ptr->AddFolder("Daughter_Folder_Name.c_str()", "Daughter_Folder_Name.c_str()");
//    }

//    if (TFolder_path.find_first_of('/') <= TFolder_path.size()) {
//        int number = TFolder_path.find_last_of('/');
//        string Folder = TFolder_path.substr(number+1, TFolder_path.size());
////        int number = TFolder_path.find_first_of('/');
////        string Folder = TFolder_path.substr(0, number);
//        TFolder *TFolder_ptr = (TFolder *) HistogramTList->FindObject(Folder.c_str());
//
//        TFolder_ptr->AddFolder("Daughter_Folder_Name.c_str()", "Daughter_Folder_Name.c_str()");
//    }
//    if (Plots_Daughter_Folder != "") {
//        if (Plots_Daughter_Folder.find_first_of('/') != 0) {
//            int number = Plots_Daughter_Folder.find_first_of('/');
//            string Daughter_Folder_Name = Plots_Daughter_Folder.substr(0, number);
//
//            Histogram_List_Folder->AddFolder(Daughter_Folder_Name.c_str(), Plots_Parent_Folder.c_str());
//
//            if (Plots_Daughter_Folder.size() - Daughter_Folder_Name.size() > 0) {
//                TFolder *pf = (TFolder *) Histogram_List_Folder->FindObject(Daughter_Folder_Name.c_str());
//
//                TFolderAdder(pf, Daughter_Folder_Name, Plots_Daughter_Folder.substr(number + 1, Plots_Daughter_Folder.size() - 1));
////            TFolderAdder(Histogram_list, Plots_Parent_Folder, Plots_Daughter_Folder.substr(number + 1, Plots_Daughter_Folder.size() - 1));
//            }
//        }
//    }
}
//</editor-fold>

// MakeDirectory function -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="MakeDirectory function (regular)">
/* Usage: made directory for plots. */

void MakeDirectory(bool Create_Directory, string Plots_Parent_Folder, string Plots_Daughter_Folder, bool Clear_Parent_Folder_content = false,
                   string Parent_Folder = "./plots") {

    string MakeDirectory = "mkdir -p " + Parent_Folder;
    string RemoveDirectoryContent = "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*";
//    cout << "rm -r " + Parent_Folder + "/" + Plots_Parent_Folder + "/*" << "\n\n";

    if (Clear_Parent_Folder_content == true && Create_Directory == true) {
        system(RemoveDirectoryContent.c_str());
        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    } else if (Clear_Parent_Folder_content == false && Create_Directory == true) {
        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
    }

    /*
//    if (Clear_Parent_Folder_content == true) {
//        system(RemoveDirectoryContent.c_str());
//    }
//
//    if (Create_Directory == true) {
//        system((MakeDirectory + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder).c_str());
////        cout << MakeDir + "/" + Plots_Parent_Folder + "/" + Plots_Daughter_Folder << "\n\n";
//    }
     */

}
//</editor-fold>

// SetLorentzVector function --------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="SetLorentzVector function">
/* Usage:  */

void SetLorentzVector(TLorentzVector &p4, clas12::region_part_ptr rp) {
    p4.SetXYZM(rp->par()->getPx(), rp->par()->getPy(), rp->par()->getPz(), p4.M());
}

void SetLorentzVector(TLorentzVector &p4, clas12::region_part_ptr rp, double mass) {
    double P = rp->par()->getP();
    p4.SetXYZM(rp->par()->getPx(), rp->par()->getPy(), rp->par()->getPz(), sqrt(mass * mass + P * P));
}
//</editor-fold>

// rCalc function -------------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="rCalc function">
/* Usage: calculate vector magnitude, given x,y,z components. */

double rCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}
//</editor-fold>

// LogEventCuts functions -----------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="LogEventCuts function (original/MicroBooNE)">
/* Usage: in cut parameter testings, log to an AC histogram given the cuts. May be applied to other cuts in the future. */

void LogEventCuts(TH1D *Histogram1D, clas12::region_part_ptr Particle, string CutType, double Upper_cut, double Lower_cut, double CutCenter = 0) {
    if (CutType == "momentum" || CutType == "") {
        TVector3 P;
        P.SetMagThetaPhi(Particle->getP(), Particle->getTheta(), Particle->getPhi());

        if ((Upper_cut == -1) && (Lower_cut == -1)) {
            Histogram1D->Fill(P.Mag());
        } else if ((Upper_cut != -1) && (Lower_cut == -1)) {
            if (P.Mag() <= Upper_cut) { Histogram1D->Fill(P.Mag()); }
        } else if ((Upper_cut == -1) && (Lower_cut != -1)) {
            if (P.Mag() >= Lower_cut) { Histogram1D->Fill(P.Mag()); }
        } else if ((Upper_cut != -1) && (Lower_cut != -1)) {
            if ((P.Mag() >= Lower_cut) && (P.Mag() <= Upper_cut)) { Histogram1D->Fill(P.Mag()); }
        }
    }
}
//</editor-fold>

//<editor-fold desc="LogEventCuts function (1e2p and 2p)">
/* Usage: in cut parameter testings, log to an AC histogram given the cuts. */

void LogEventCuts(TH1D *Histogram1D, clas12::region_part_ptr Particle, double Lower_cut, double Upper_cut, double CutCenter = 0) {
    TVector3 P;
    P.SetMagThetaPhi(Particle->getP(), Particle->getTheta(), Particle->getPhi());

    if ((Upper_cut == -1) && (Lower_cut == -1)) {
        Histogram1D->Fill(P.Mag());
    } else if ((Upper_cut != -1) && (Lower_cut == -1)) {
        if (P.Mag() <= Upper_cut) { Histogram1D->Fill(P.Mag()); }
    } else if ((Upper_cut == -1) && (Lower_cut != -1)) {
        if (P.Mag() >= Lower_cut) { Histogram1D->Fill(P.Mag()); }
    } else if ((Upper_cut != -1) && (Lower_cut != -1)) {
        if ((P.Mag() >= Lower_cut) && (P.Mag() <= Upper_cut)) { Histogram1D->Fill(P.Mag()); }
    }
}
//</editor-fold>

// BoolToString function ------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="BoolToString function">
/* Usage: convert bool variables to string. Used to log settings to file. */

inline const char *const BoolToString(bool b) { return b ? "true" : "false"; }
//</editor-fold>

// testPrint functions --------------------------------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="testPrint functions">
/* Usage: general functions used to print variables. */

void testPrint() { cout << "\n"; }

void testPrint(string varString = "") {
    if (varString == "") {
        cout << "\n";
    } else {
        cout << varString << "\n";
    }
}

void testPrint(int var, string varString = "") {
    if (varString == "") {
        cout << "\n";
    } else {
        cout << varString << " = " << var << "\n";
    }
}

void testPrint(double var, string varString = "") {
    if (varString == "") {
        cout << "\n";
    } else {
        cout << varString << " = " << var << "\n";
    }
}
//</editor-fold>

// fit functions --------------------------------------------------------------------------------------------------------------------------------------------------

////<editor-fold desc="testPrint functions">
///* Usage: general functions used to print variables. */
//
//Double_t fitf(Double_t *v, Double_t *par) {
//    Double_t arg = 0;
//    if (par[2] != 0) arg = (v[0] - par[1]) / par[2];
//
//    Double_t fitval = par[0] * TMath::Exp(-0.5 * arg * arg);
//    return fitval;
//}
//
//void fitexample() {
//    TFile *f = new TFile("hsimple.root");
//
//    TH1F *hpx = (TH1F *) f->Get("hpx");
//
//    //create a function with 3 parameters in the range [-3,3]
//    TF1 *func = new TF1("fit", fitf, -3, 3, 3);
//    func->SetParameters(500, hpx->GetMean(), hpx->GetRMS());
//    func->SetParNames("Constant", "Mean_value", "Sigma");
//    hpx->Fit("fit");
//}
////</editor-fold>

#endif //DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H
