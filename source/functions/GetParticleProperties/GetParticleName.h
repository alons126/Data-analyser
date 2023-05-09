//
// Created by alons on 09/05/2023.
//

#ifndef GETPARTICLENAME_H
#define GETPARTICLENAME_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

string GetParticleName(string Source) {
    string ParticleName;

    if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleName = "Electron";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleName = "Proton";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleName = "Neutron";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleName = "Piplus";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleName = "Piminus";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleName = "Kplus";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleName = "Kminus";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleName = "Photon";
    } else {
        ParticleName = "Unknown";
    }

    return ParticleName;
}

#endif //GETPARTICLENAME_H
