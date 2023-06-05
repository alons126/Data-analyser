//
// Created by alons on 05/06/2023.
//

#ifndef SETSTATSTITLE_H
#define SETSTATSTITLE_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "GetParticleNameShort.h"

using namespace std;

string SetStatsTitle(const string &RecTitle) {
    string StatsTitle;

    string ParticleShort = GetParticleNameShort(RecTitle);

    if (findSubstring(RecTitle, "momentum")) {
        StatsTitle = "P_{" + ParticleShort + "}";
    }else if (findSubstring(RecTitle, "#theta")) {
        StatsTitle = "#theta_{" + ParticleShort + "}";
    } else if (findSubstring(RecTitle, "#phi")) {
        StatsTitle = "#phi_{" + ParticleShort + "}";
    }

    return StatsTitle;
}

#endif //SETSTATSTITLE_H
