//
// Created by alons on 05/06/2023.
//

#ifndef SETTITLE_H
#define SETTITLE_H

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include "GetParticleName.h"
#include "GetParticleNameShort.h"
#include "SetType.h"

using namespace std;

string SetTitle(const string &RecTitle) {
    string Title;

    string Particle = GetParticleName(RecTitle);
    string ParticleShort = GetParticleNameShort(RecTitle);
    string Type = SetType(RecTitle);

    if (Type == "momentum") {
        Title = Particle + " " + Type;
    } else {
        Title = "#" + Type + "_{" + ParticleShort + "}";
    }

    return Title;
}

#endif //SETTITLE_H
