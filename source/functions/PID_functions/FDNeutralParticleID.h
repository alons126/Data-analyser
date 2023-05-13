//
// Created by alons on 13/05/2023.
//

#ifndef FDNEUTRALPARTICLEID_H
#define FDNEUTRALPARTICLEID_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "GetFDNeutronP.h"
#include "../../classes/DSCuts/DSCuts.h"

using namespace std;

/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void FDNeutralParticleID(vector<region_part_ptr> allParticles,
                         vector<int> &FD_Neutrons_ind, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                         vector<int> &FD_Photons_ind, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th,
                         bool apply_neutron_Beta_Fit) {

    for (int &i: ID_Neutrons_FD) { // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            cout << "\n\nFDNeutralParticleID (Neutrons): Neutron PDG is not 2112 or 22 (" << NeutralPDG << "). Exiting...\n\n", exit(EXIT_FAILURE);
        }

        double Momentum = GetFDNeutronP(allParticles[i], apply_neutron_Beta_Fit);

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Momentum >= Neutron_momentum_th.GetLowerCut()) { FD_Neutrons_ind.push_back(i); }
    } // end of loop over ID_Neutrons_FD vector

    for (int &i: ID_Photons_FD) { // Identify photons above momentum threshold
        /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

        if (NeutralPDG != 22) {
            cout << "\n\nFDNeutralParticleID (Photons): Photon PDG is not 22 (" << NeutralPDG << "). Exiting...\n\n", exit(EXIT_FAILURE);
        }

        double Momentum = allParticles[i]->getP();

        /* Log photons above momentum cuts (given by Momentum_cuts): */
        if (Momentum >= Photon_momentum_th.GetLowerCut()) { FD_Photons_ind.push_back(i); }
    } // end of loop over ID_Photons_FD vector
}


#endif //FDNEUTRALPARTICLEID_H
