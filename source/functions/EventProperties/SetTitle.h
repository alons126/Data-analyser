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

string SetTitle(const string &RecTitle, const string &PlotsT, const string &DRegion) {
    string Title;

    string Particle = GetParticleName(RecTitle);
    string ParticleShort = GetParticleNameShort(RecTitle);
    string Type = SetType(RecTitle);

    if (PlotsT == "FSRatio") {
        if (Particle == "Electron") {
            if (Type == "momentum") {
                Title = Particle + " " + Type + " ratio";
            } else {
                Title = Particle + " #" + Type + " ratio";
            }
        } else {
            if (DRegion == "FD") {
                if (Type == "momentum") {
                    Title = DRegion + " nucleon " + Type + " ratio";
                } else {
                    Title = DRegion + " nucleon #" + Type + " ratio";
                }
            } else if (DRegion == "CD") {
                if (Type == "momentum") {
                    Title = DRegion + " proton " + Type + " ratio";
                } else {
                    Title = DRegion + " proton #" + Type + " ratio";
                }
            }
        }
    } else {
        if (Type == "momentum") {
            Title = Particle + " " + Type;
        } else {
            Title = "#" + Type + "_{" + ParticleShort + "}";
        }
    }

    return Title;
}

#endif //SETTITLE_H
