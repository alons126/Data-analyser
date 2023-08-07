//
// Created by alons on 09/05/2023.
//

#ifndef GETPARTICLENAMESHORT_H
#define GETPARTICLENAMESHORT_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

string GetParticleNameShort(string Source) {
    string ParticleNameShort;

    if (findSubstring(Source, "neutrals") || findSubstring(Source, "Neutrals")
        || findSubstring(Source, "neut.") || findSubstring(Source, "Neut.")) {
        ParticleNameShort = "neut";
    } else if (findSubstring(Source, "Electron") || findSubstring(Source, "electron")) {
        ParticleNameShort = "e";
    } else if (findSubstring(Source, "Proton") || findSubstring(Source, "proton")) {
        ParticleNameShort = "p";
    } else if (findSubstring(Source, "Neutron") || findSubstring(Source, "neutron")) {
        ParticleNameShort = "n";
    } else if (findSubstring(Source, "#pi^{+}")) {
        ParticleNameShort = "#pi^{+}";
    } else if (findSubstring(Source, "#pi^{-}")) {
        ParticleNameShort = "#pi^{-}";
    } else if (findSubstring(Source, "#K^{+}")) {
        ParticleNameShort = "K^{+}";
    } else if (findSubstring(Source, "#K^{-}")) {
        ParticleNameShort = "K^{-}";
    } else if (findSubstring(Source, "#gamma") || findSubstring(Source, "photon")
               || findSubstring(Source, "Photon")) {
        ParticleNameShort = "Photon";
    } else {
        ParticleNameShort = "unknown";
    }

    return ParticleNameShort;
}

#endif //GETPARTICLENAMESHORT_H
