//
// Created by alons on 26/04/2023.
//

#ifndef GETPHOTONS_H
#define GETPHOTONS_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../classes/DSCuts/DSCuts.h"

using namespace std;

/* The GetPhotons function gets photons from the FD. */

vector<int> GetPhotons(vector<region_part_ptr> &allParticles) {
    vector<int> Photons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticalPDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->getRegion() == FD) && (ParticalPDG == 22)) { // if neutron/photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit

            // if particle hit the PCAL - it is a photon:
            if (inPCAL) { Photons_indices_in_allParticles.push_back(i); }
        } // end of if neutron/photon is in the FD
    } // end of loop over allparticle vector

    /* Return a vector of the neutrons in allParticles, according to the definition. */
    return Photons_indices_in_allParticles;
}


#endif //GETPHOTONS_H
