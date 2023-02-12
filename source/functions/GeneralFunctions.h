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

//<editor-fold desc="GetCurrentDirectory function">
/* This function is used to get the directory of the main.c code */
string GetCurrentDirectory() {
    char pwd[PATH_MAX];
    getcwd(pwd, sizeof(pwd));

    string WorkingDirectory = pwd;

    return WorkingDirectory;
}
//</editor-fold>

//<editor-fold desc="findSubstring function">
bool findSubstring(string string1, string string2) {
    if (string1.find(string2) != std::string::npos) {
        return true;
    } else {
        return false;
    }
}
//</editor-fold>

//<editor-fold desc="getBeanE function">
double getBeanE(string AnalyseFileSample) {
    double beamE;

    if ((findSubstring(AnalyseFileSample, "598636MeV") == true) || (findSubstring(AnalyseFileSample, "598636mev") == true)
        || (findSubstring(AnalyseFileSample, "598636") == true)) {
        beamE = 5.98636;
    } else if ((findSubstring(AnalyseFileSample, "402962MeV") == true) || (findSubstring(AnalyseFileSample, "402962mev") == true)
               || (findSubstring(AnalyseFileSample, "402962") == true)) {
        beamE = 4.02962;
    } else if ((findSubstring(AnalyseFileSample, "207052MeV") == true) || (findSubstring(AnalyseFileSample, "207052mev") == true)
               || (findSubstring(AnalyseFileSample, "207052") == true)) {
        beamE = 2.07052;
    }

    return beamE;
}
//</editor-fold>

//<editor-fold desc="to_string_with_precision function">
template<typename T>
std::string to_string_with_precision(const T a_value, const int n = 2) {
    std::ostringstream out;
    out.precision(n);
    out << std::fixed << a_value;
    return out.str();
}
//</editor-fold>

//<editor-fold desc="BoolToString function">
inline const char *const BoolToString(bool b) {
    return b ? "true" : "false";
}
//</editor-fold>

//<editor-fold desc="testPrint functions">
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

//<editor-fold desc="rCalc function">
double rCalc(double x, double y, double z) {
    double r = sqrt(x * x + y * y + z * z);
    return r;
}
//</editor-fold>

//<editor-fold desc="TFolderAdder function">
void TFolderAdder(TFolder *Histogram_List_Folder, std::string Plots_Parent_Folder, std::string Plots_Daughter_Folder) {
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

//<editor-fold desc="MakeDirectory">
void MakeDirectory(bool Create_Directory, std::string Plots_Parent_Folder, std::string Plots_Daughter_Folder, bool Clear_Parent_Folder_content = false,
                   std::string Parent_Folder = "./plots") {

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

//<editor-fold desc="LogEventCuts function">
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

#endif //DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H
