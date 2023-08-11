//
// Created by alons on 05/06/2023.
//

#ifndef SETSAVEDIR_H
#define SETSAVEDIR_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "GetParticleName.h"
#include "GetParticleNameLC.h"
#include "GetParticleNameShort.h"

#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

void SetSaveDir(string &SaveDir, string &TestSaveDir, const string &RecTitle, const string &Histogram1DSaveNamePath, const string &PlotsT, const string &DRegion,
                const string &Particle, const string &ParticleLC, const string &ParticleShort, const string &Type) {
    if (PlotsT == "FSRatio") {
        if (!findSubstring(Type, "vs") && !findSubstring(Type, "vs.") && !findSubstring(Type, "VS") && !findSubstring(Type, "VS.")) {
            if (Type == "W" || Type == "Q2" || Type == "E_e" || Type == "omega" || Type == "Ecal" || Type == "deltaP_T_tot" || Type == "deltaP_T_L" ||
                Type == "deltaAlpha_T_tot" || Type == "deltaAlpha_T_L" || Type == "deltaPhi_T_tot" || Type == "deltaPhi_T_L" ||
                Type == "total_3momentum" || Type == "relative_3momentum" || Type == "total_4momentum" || Type == "relative_4momentum" ||
                Type == "leading_nuc_momentum" || Type == "recoil_nuc_momentum" ||
                Type == "theta_tot" || Type == "phi_tot" || Type == "theta_rel" || Type == "phi_rel" ||
                Type == "Opening_ang_P_nucL_minus_q_nucR" || Type == "P_tot_minus_q"
                    ) {
                SaveDir = Histogram1DSaveNamePath + "/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                if (findSubstring(RecTitle, "Electron") || findSubstring(RecTitle, "electron")) {
                    SaveDir = Histogram1DSaveNamePath + "/00_" + Particle + "_" + Type + "_plots/";
                    TestSaveDir = SaveDir + "Cloned_hist_test/";
                } else {
                    if (DRegion == "FD") {
                        SaveDir = Histogram1DSaveNamePath + "/01_FD_Nucleon_" + Type + "_plots/";
                        TestSaveDir = SaveDir + "Cloned_hist_test/";
                    } else if (DRegion == "CD") {
                        SaveDir = Histogram1DSaveNamePath + "/02_CD_Nucleon_" + Type + "_plots/";
                        TestSaveDir = SaveDir + "Cloned_hist_test/";
                    } else {
                        SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                        TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
                    }
                }
            }
        } else {
            SaveDir = Histogram1DSaveNamePath + "/" + Type + "_plots/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        }
    } else {
        if (findSubstring(RecTitle, "Electron") || findSubstring(RecTitle, "electron")) {
            SaveDir = Histogram1DSaveNamePath + "/00_" + Particle + "_" + Type + "_plots/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else {
            if (findSubstring(RecTitle, ", FD)") || findSubstring(RecTitle, "FD " + Particle) ||
                findSubstring(RecTitle, "FD " + ParticleLC)) {
                SaveDir = Histogram1DSaveNamePath + "/01_FD_" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else if (findSubstring(RecTitle, ", CD)") || findSubstring(RecTitle, "CD " + Particle) ||
                       findSubstring(RecTitle, "CD " + ParticleLC)) {
                SaveDir = Histogram1DSaveNamePath + "/02_CD_" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
            }
        }
    }

    system(("mkdir -p " + SaveDir).c_str());
    system(("mkdir -p " + TestSaveDir).c_str());
}

#endif //SETSAVEDIR_H
