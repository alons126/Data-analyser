//
// Created by alons on 05/06/2023.
//

#ifndef SETXAXISTITLE_H
#define SETXAXISTITLE_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "GetParticleName.h"
#include "GetParticleNameShort.h"
#include "../GeneralFunctions.h"
//#include "../findSubstring.h"

using namespace std;

string SetXAxisTitle(const string &RecTitle) {
    string XAxisTitle;

    string Particle = GetParticleName(RecTitle);
    string ParticleShort = GetParticleNameShort(RecTitle);

    if (findSubstring(RecTitle, "momentum")) {
        XAxisTitle = "P_{" + ParticleShort + "} [GeV/c]";
    } else if (findSubstring(RecTitle, "#theta")) {
        XAxisTitle = RecTitle + " [Deg]";
    } else if (findSubstring(RecTitle, "#phi")) {
        XAxisTitle = RecTitle + " [Deg]";
    }

    return XAxisTitle;
}

#endif //SETXAXISTITLE_H
