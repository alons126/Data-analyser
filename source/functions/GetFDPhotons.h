//
// Created by alons on 27/04/2023.
//

#ifndef GETFDPHOTONS_H
#define GETFDPHOTONS_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

using namespace std;

/* The GetFDPhotons function gets photons from the FD, according to the definition from Larry:
 * Photon = a photon in the FD with a PCAL hit. */

vector<int> GetFDPhotons(vector<region_part_ptr> &allParticles, DSCuts &Momentum_cuts) {
    vector<int> Photons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->getRegion() == FD) && (ParticlePDG == 22)) { // if photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit

            if (inPCAL) { // if the criteria for a particle to be a photon is upheld
                /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */
                double Momentum = allParticles[i]->getP();

                /* Log photons above momentum cuts (given by Momentum_cuts): */
                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Photons_indices_in_allParticles.push_back(i); }
            } // end of if photon hit the EC but not in PCAL
        } // end of if photon is in the FD
    } // end of loop over allparticle vector

    /* Return a vector of the photons in allParticles, according to the definition. */
    return Photons_indices_in_allParticles;
}

#endif //GETFDPHOTONS_H
