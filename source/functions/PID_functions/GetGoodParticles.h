//
// Created by alons on 24/04/2023.
//

#ifndef GETGOODPARTICLES_H
#define GETGOODPARTICLES_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../../classes/DSCuts/DSCuts.h"

using namespace std;

vector<int> GetGoodParticles(vector<region_part_ptr> &Particle, // particle
                             DSCuts &Momentum_cuts // corresponding momentum cuts
) {
    vector<int> GoodParticles;

    for (int i = 0; i < Particle.size(); i++) {
        double Momentum = Particle[i]->getP();

        if (Momentum >= Momentum_cuts.GetLowerCut()) { GoodParticles.push_back(i); }
//        if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { GoodParticles.push_back(i); }
    }

    return GoodParticles;
}

#endif //GETGOODPARTICLES_H
