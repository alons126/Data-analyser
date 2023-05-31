//
// Created by alons on 13/05/2023.
//

#ifndef CHARGEDPARTICLEID_H
#define CHARGEDPARTICLEID_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../../classes/DSCuts/DSCuts.h"

using namespace std;

vector<int> ChargedParticleID(vector<region_part_ptr> &Particle, DSCuts &Momentum_th) {
    vector<int> ChargedParticle;

    for (int i = 0; i < Particle.size(); i++) {
        if (Particle[i]->par()->getCharge() == 0) { // Check that the particle's charge is zero
            cout << "\n\nChargedParticleID: Particle is neutral! Exiting...\n\n", exit(EXIT_FAILURE);
        }

        double Momentum = Particle[i]->getP();

        if ((Momentum >= Momentum_th.GetLowerCut()) && (Momentum <= Momentum_th.GetUpperCut())) { ChargedParticle.push_back(i); }
//        if (fabs(Momentum_th.GetLowerCut()) == 9999 && fabs(Momentum_th.GetUpperCut()) == 9999) { // no momentum thresholds
//            ChargedParticle.push_back(i);
//        } else if (fabs(Momentum_th.GetLowerCut()) != 9999) {
//            double Momentum = Particle[i]->getP();
//
//            if ((Momentum >= Momentum_th.GetLowerCut()) && (Momentum <= Momentum_th.GetUpperCut())) { ChargedParticle.push_back(i); }
//        }
    }

    return ChargedParticle;
}

////<editor-fold desc="OLD">
//vector<int> ChargedParticleID(vector<region_part_ptr> &Particle, DSCuts &Momentum_th) {
//    vector<int> ChargedParticle;
//
//    for (int i = 0; i < Particle.size(); i++) {
//        if (Particle[i]->par()->getCharge() == 0) { // Check that the particle's charge is zero
//            cout << "\n\nChargedParticleID: Particle is neutral! Exiting...\n\n", exit(EXIT_FAILURE);
//        }
//
//        double Momentum = Particle[i]->getP();
//
//        if ((Momentum >= Momentum_th.GetLowerCut()) && (Momentum <= Momentum_th.GetUpperCut())) { ChargedParticle.push_back(i); }
//    }
//
//    return ChargedParticle;
//}
////</editor-fold>

#endif //CHARGEDPARTICLEID_H
