//
// Created by alons on 27/01/2023.
//

#ifndef DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H
#define DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H

#include <string>
#include <cmath>
#include <tuple>
#include <iostream>
#include <sys/stat.h>
#include <sstream>

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

#endif //DETECTORSIMULATIONANALYSER_C_GENERALFUNCTIONS_H