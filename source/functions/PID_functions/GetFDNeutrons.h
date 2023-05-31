//
// Created by alons on 26/04/2023.
//

#ifndef GETFDNEUTRONS_H
#define GETFDNEUTRONS_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "GetFDNeutronP.h"

using namespace std;

/* The GetFDNeutrons function gets neutrons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit. */

//bool GetFDNeutrons(region_part_ptr &particle) {
vector<int> GetFDNeutrons(vector<region_part_ptr> &allParticles, DSCuts &Momentum_cuts, bool apply_nucleon_cuts) {
    vector<int> Neutrons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->getRegion() == FD) && ((ParticlePDG == 2112) || (ParticlePDG == 22))) { // if neutron/photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if (!inPCAL && (inECIN || inECOUT)) { // if the criteria for a particle to be a neutron is upheld
                /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                /* Log neutrons above momentum cuts (given by Momentum_cuts): */
                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons_indices_in_allParticles.push_back(i); }
            } // end of if neutron/photon hit the EC but not in PCAL
        } // end of if neutron/photon is in the FD
    } // end of loop over allparticle vector

    /* Return a vector of the neutrons in allParticles, according to the definition. */
    return Neutrons_indices_in_allParticles;
}

////<editor-fold desc="Old">
//vector<int> GetFDNeutrons(vector<region_part_ptr> &allParticles, DSCuts &Momentum_cuts) {
//    vector<int> Neutrons_indices_in_allParticles;
//
//    for (int i = 0; i < allParticles.size(); i++) {
//        int ParticlePDG = allParticles[i]->par()->getPid();
//
////        if ((allParticles[i]->getRegion() == FD) && (ParticlePDG == 2112)) { // if neutron/photon is in the FD
//        if ((allParticles[i]->getRegion() == FD) && ((ParticlePDG == 2112) || (ParticlePDG == 22))) { // if neutron/photon is in the FD
//            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
//            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
//            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit
//
//            if (!inPCAL && (inECIN || inECOUT)) { // if the criteria for a particle to be a neutron is upheld
//                /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
////                double Momentum = allParticles[i]->getP();
//                double Momentum = GetFDNeutronP(allParticles[i]);
//
//                /* Log neutrons above momentum cuts (given by Momentum_cuts): */
//                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons_indices_in_allParticles.push_back(i); }
//            } // end of if neutron/photon hit the EC but not in PCAL
//        } // end of if neutron/photon is in the FD
//    } // end of loop over allparticle vector
//
//    /* Return a vector of the neutrons in allParticles, according to the definition. */
//    return Neutrons_indices_in_allParticles;
//}
////</editor-fold>

#endif //GETFDNEUTRONS_H
