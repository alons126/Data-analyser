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

#include "../classes/DSCuts/DSCuts.h"

using namespace std;

vector<int> GetGoodParticles(vector<region_part_ptr> &Particle, // particle
//vector<int> GetGoodParticles(region_part_ptr &Particle, // particle
                             DSCuts &Momentum_cuts // corresponding momentum cuts
) {
    vector<int> within_momentum_cuts;

    for (int i = 0; i < Particle.size(); i++) {
        double Momentum = Particle[i]->getP();

        if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { within_momentum_cuts.push_back(i); }
    }

    return within_momentum_cuts;
}


#endif //GETGOODPARTICLES_H
