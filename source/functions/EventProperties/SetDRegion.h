//
// Created by alons on 05/06/2023.
//

#ifndef SETDREGION_H
#define SETDREGION_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

string SetDRegion(const string &RecTitle, const string &Particle, const string &ParticleLC) {
    string DRegion;

    if (findSubstring(RecTitle, ", FD)") || findSubstring(RecTitle, "FD " + Particle) ||
        findSubstring(RecTitle, "FD " + ParticleLC)) {
        DRegion = "FD";
    } else if (findSubstring(RecTitle, ", CD)") || findSubstring(RecTitle, "CD " + Particle) ||
               findSubstring(RecTitle, "CD " + ParticleLC)) {
        DRegion = "CD";
    }

    return DRegion;
}

#endif //SETDREGION_H
