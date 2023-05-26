//
// Created by alons on 13/05/2023.
//

#ifndef NPARTICLEID_H
#define NPARTICLEID_H

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

void nParticleID(vector<region_part_ptr> &allParticles, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th,
                 bool apply_nucleon_cuts) {

    for (int i = 0; i < allParticles.size(); i++) {
        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD)) { // If particle is neutral and in the FD
            int ParticlePDG = allParticles[i]->par()->getPid();

            bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
            bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
            bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if ((ParticlePDG == 2112) || (ParticlePDG == 22)) {
                if (ParticleInPCAL && (ParticlePDG == 22)) { // if there's a 'photon' with a PCAL hit -> photon
//                    ID_Photons_FD.push_back(i);

                    /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */
                    double Momentum = allParticles[i]->getP();

                    /* Log photons above momentum cuts (given by Momentum_cuts): */
                    if (Momentum >= Photon_momentum_th.GetLowerCut()) { ID_Photons_FD.push_back(i); }
                } else if (!ParticleInPCAL) { // if there is a neutron or a 'photon' without a PCAL hit
                    if (ParticleInECIN || ParticleInECOUT) { // if there is either a ECIN or ECOUT hit -> neutron
//                        ID_Neutrons_FD.push_back(i);
//
                    /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                    double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                    /* Log neutrons above momentum cuts (given by Momentum_th): */
                    if (Momentum >= Neutron_momentum_th.GetLowerCut()) { ID_Neutrons_FD.push_back(i); }
                    }
                }
            } // end of clas12root neutron or 'photon' if
        } // end of neutral and in the FD if
    } // end of loop over allparticle vector
}


#endif //NPARTICLEID_H
