//
// Created by alons on 13/05/2023.
//

#ifndef FDNEUTRALPARTICLE_H
#define FDNEUTRALPARTICLE_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "GetFDNeutronP.h"
#include "../../classes/DSCuts/DSCuts.h"
#include "../NeutronECAL_Cut_Veto.h"

using namespace std;

/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

/* FDNeutralParticle without ECAL veto */
void FDNeutralParticle(vector <region_part_ptr> allParticles, vector<int> &ID_Neutrons_FD, vector<int> &ID_Photons_FD) {
    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (ParticlePDG != 0)) { // If particle is neutral and in the FD
            bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
            bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
            bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if ((ParticlePDG == 2112) || (ParticlePDG == 22)) {
                if (ParticleInPCAL) {
                    // if there's a 'photon' with a PCAL hit -> photon:
                    if (ParticlePDG == 22) { ID_Photons_FD.push_back(i); }
                } else if (!ParticleInPCAL) { // if there is a neutron or a 'photon' without a PCAL hit
                    // if there is either a ECIN or ECOUT hit -> neutron:
                    if (ParticleInECIN || ParticleInECOUT) { ID_Neutrons_FD.push_back(i); }
                }
            } // end of clas12root neutron or 'photon' if
        } // end of neutral and in the FD if
    } // end of loop over allParticles vector
}

/* FDNeutralParticle with ECAL veto */
void FDNeutralParticle(vector <region_part_ptr> allParticles, vector <region_part_ptr> electrons,
                       vector<int> &ID_Neutrons_FD, vector<int> &ID_Photons_FD,
                       DSCuts &Neutron_veto_cut, const double &beamE) {
    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->getRegion() == FD) && (ParticlePDG != 0)) { // If particle is neutral and in the FD
            bool ParticleInPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
            bool ParticleInECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
            bool ParticleInECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if ((ParticlePDG == 2112) || (ParticlePDG == 22)) {
                if (ParticleInPCAL) {
                    // if there's a 'photon' with a PCAL hit -> photon:
                    if (ParticlePDG == 22) { ID_Photons_FD.push_back(i); }
                } else if (!ParticleInPCAL) { // if there is a neutron or a 'photon' without a PCAL hit
                    // if there is either a ECIN or ECOUT hit -> neutron:
                    if (ParticleInECIN || ParticleInECOUT) {
                        bool NeutronPassVetoTest = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCut());

                        if (NeutronPassVetoTest) { // if there is a neutron or a 'photon' without a PCAL hit
                            // if there is either a ECIN or ECOUT hit -> neutron:
                            ID_Neutrons_FD.push_back(i);
                        }
                    }
                }
            } // end of clas12root neutron or 'photon' if
        } // end of neutral and in the FD if
    } // end of loop over allParticles vector
}

#endif //FDNEUTRALPARTICLE_H
