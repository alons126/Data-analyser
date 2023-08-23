//
// Created by alons on 05/06/2023.
//

#ifndef SETFSRATIOSAVEDIR_H
#define SETFSRATIOSAVEDIR_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "GetParticleName.h"
#include "GetParticleNameLC.h"
#include "GetParticleNameShort.h"
#include "SetSaveDir.h"

#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

void SetFSRatioSaveDir(string &SaveDir, string &TestSaveDir, const string &RecTitle, const string &FinalState, const string &Histogram1DSaveNamePathOriginal,
                       const string &PlotsT, const string &DRegion, const string &Particle, const string &ParticleLC, const string &ParticleShort, const string &Type) {
    string Histogram1DSaveNamePath, FStoFind, FSTopology; //TODO: reconfirm what is a topology

    if (FinalState == "1p" || FinalState == "1n") {
        FSTopology = "_1N", FStoFind = "02_1p";
    } else if (FinalState == "pFDpCD" || FinalState == "nFDpCD") {
        FSTopology = "_2N", FStoFind = "05_pFDpCD";
    }

    if (!findSubstring(RecTitle, "vs") && !findSubstring(RecTitle, "vs.") &&
        !findSubstring(RecTitle, "VS") && !findSubstring(RecTitle, "VS.")) {
        if (Type == "deltaP_T_tot" || Type == "deltaP_T_L" ||
            Type == "deltaAlpha_T_tot" || Type == "deltaAlpha_T_L" ||
            Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L") { // TKI ratios
            Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                      "/FS_TKI_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
        } else if (findSubstring(Type, "Opening_ang") ||
                   findSubstring(Type, "theta") || findSubstring(Type, "phi")) {
            if (findSubstring(Type, "Opening_ang")) { // Angle ratios
                Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                          "/FS_ang_ratio_plots/FS_Opening_ang_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
            } else if (findSubstring(Type, "theta")) {
                Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                          "/FS_ang_ratio_plots/FS_theta_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
            } else if (findSubstring(Type, "phi")) {
                Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                          "/FS_ang_ratio_plots/FS_phi_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
            }
        } else if (findSubstring(Type, "momentum") || findSubstring(Type, "P_tot_minus_q")) { // Momentum ratios
            Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                      "/FS_momentum_ratio_plots/FS_" + Type + "_ratio_plots" + FSTopology;
        } else {
            Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                      "/FS_" + Type + "_ratio_plots" + FSTopology;
        }
    } else {
        Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find(FStoFind) - 1) +
                                  "/FS_2D_hist_ratios/FS_" + Type + "_ratio_plots" + FSTopology;
    }

    //<editor-fold desc="original">
    //    string Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/FS_" + Type + "_ratio_plots";

    /*
    if (Type == "W" || Type == "Q2") {
        Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/FS_" + Type + "_ratio_plots";
    } else {
        Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/06a_FS_" + Type + "_ratio_plots";
    }
*/
    //</editor-fold>

    SetSaveDir(SaveDir, TestSaveDir, RecTitle, Histogram1DSaveNamePath, PlotsT, DRegion, Particle, ParticleLC, ParticleShort, Type);
}

#endif //SETFSRATIOSAVEDIR_H
