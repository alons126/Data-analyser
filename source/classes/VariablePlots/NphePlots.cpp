//
// Created by alons on 10/04/2023.
//

#include "NphePlots.h"

NphePlots::NphePlots(bool aCuts, std::string fState, std::string SavePath) {
    if (aCuts == false) {
        hNphe_BC_FD = hPlot1D("", "1e cut", "", "N_{phe} in HTCC BC", "#Photo-electrons in HTCC N_{phe} - before cuts", "N_{phe}", 0, 40);
//        hNphe_1e_cut_BC_FD = new TH1D("N_{phe} in HTCC BC (1e Cut)", "#Photo-electrons in HTCC - N_{phe} - Before Cuts (1e Cut);N_{phe}", 100, 0, 40);
//        hNphe_1e_cut_AC_FD = new TH1D("N_{phe} in HTCC AC (1e Cut)", "#Photo-electrons in HTCC - N_{phe} - After Cuts (1e Cut);N_{phe}", 100, 0, 40);
        hNphe_AC_FD = hPlot1D("", "1e cut", "", "N_{phe} in HTCC AC", "#Photo-electrons in HTCC N_{phe} - after cuts", "N_{phe}", 0, 40);
        hNphe_BC_FD.SetHistogram1DSaveNamePath(SavePath);
        hNphe_AC_FD.SetHistogram1DSaveNamePath(SavePath);
        hNphe_1e_cut_AC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"];
//        hNphe_1e_cut_BC_FD_Dir = Nphe_1e_cut_BC_Directory, hNphe_1e_cut_AC_FD_Dir = Nphe_1e_cut_AC_Directory;
    } else {
        hNphe_1e_cut_BC_FD = new TH1D("N_{phe} in HTCC (1e Cut)", "#Photo-electrons in HTCC - N_{phe} (1e Cut);N_{phe}", 100, 0, 40);
        hNphe_1e_cut_BC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_BC_Directory"];
        hNphe_1e_cut_AC_FD_Dir = directories.Nphe_Directory_map["Nphe_1e_cut_AC_Directory"];
//        hNphe_1e_cut_BC_FD_Dir = Nphe_1e_cut_BC_Directory, hNphe_1e_cut_AC_FD_Dir = Nphe_1e_cut_AC_Directory;
    }

}