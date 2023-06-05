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
/*
    if (PlotsT == "FSRatio") {
        if (findSubstring(RecTitle, "Electron") || findSubstring(RecTitle, "electron")) {
            SaveDir = Histogram1DSaveNamePath + "/00_" + Particle + "_" + Type + "_plots/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else {
            if (DRegion == "FD" || DRegion == "CD") {
                SaveDir = Histogram1DSaveNamePath + "/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
            }
*/
    if (PlotsT == "FSRatio") {
        if (findSubstring(RecTitle, "Electron") || findSubstring(RecTitle, "electron")) {
            SaveDir = Histogram1DSaveNamePath + "/00_" + Particle + "_" + Type + "_plots/";
            TestSaveDir = SaveDir + "Cloned_hist_test/";
        } else {
/*
            if (DRegion == "FD" || DRegion == "CD") {
                SaveDir = Histogram1DSaveNamePath + "/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
            }
*/
            if (DRegion == "FD") {
//                SaveDir = Histogram1DSaveNamePath + "/";
                SaveDir = Histogram1DSaveNamePath + "/01_FD_Nucleon_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else if (DRegion == "CD") {
//                SaveDir = Histogram1DSaveNamePath + "/";
                SaveDir = Histogram1DSaveNamePath + "/02_CD_Nucleon_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test/";
            } else {
                SaveDir = Histogram1DSaveNamePath + "/" + Particle + "_" + Type + "_plots/";
                TestSaveDir = SaveDir + "Cloned_hist_test" + "/";
            }
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
