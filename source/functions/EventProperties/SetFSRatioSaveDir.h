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

void SetFSRatioSaveDir(string &SaveDir, string &TestSaveDir, const string &RecTitle, const string &Histogram1DSaveNamePathOriginal,
                       const string &Particle, const string &ParticleLC, const string &ParticleShort, const string &Type) {
//    string Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal;
    string Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/";
//    string Histogram1DSaveNamePath = Histogram1DSaveNamePathOriginal.substr(0, Histogram1DSaveNamePathOriginal.find("05_pFDpCD") - 1) + "/00_FS_Ratio_plots";




//    cout << "\n\n\nHistogram1DSaveNamePath = " << Histogram1DSaveNamePath << "\n\n\n";
//    exit(EXIT_FAILURE);




//    system(("mkdir -p " + Histogram1DSaveNamePath).c_str());
    SetSaveDir(SaveDir, TestSaveDir, RecTitle, Histogram1DSaveNamePath, Particle, ParticleLC, ParticleShort, Type);
}


//    cout << "\n\n\nHistogram1DSaveNamePath = " << Histogram1DSaveNamePath << "\n\n\n";
//    exit(EXIT_FAILURE);


#endif //SETFSRATIOSAVEDIR_H
