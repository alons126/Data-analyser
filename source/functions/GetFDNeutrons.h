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

using namespace std;

/* The GetFDNeutrons function gets neutrons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit. */

//bool GetFDNeutrons(region_part_ptr &particle) {
vector<int> GetFDNeutrons(vector<region_part_ptr> &allParticles, DSCuts &Momentum_cuts) {
    vector<int> Neutrons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticalPDG = allParticles[i]->par()->getPid();

//        if ((allParticles[i]->getRegion() == FD) && (ParticalPDG == 2112)) { // if neutron/photon is in the FD
        if ((allParticles[i]->getRegion() == FD) && ((ParticalPDG == 2112) || (ParticalPDG == 22))) { // if neutron/photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if (!inPCAL && (inECIN || inECOUT)) { // if the criteria for a particle to be a neutron is upheld
                /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                double Momentum = allParticles[i]->getP();

                /* Log neutrons above momentum cuts (given by Momentum_cuts): */
                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons_indices_in_allParticles.push_back(i); }
            } // end of if neutron/photon hit the EC but not in PCAL
        } // end of if neutron/photon is in the FD
    } // end of loop over allparticle vector

    /* Return a vector of the neutrons in allParticles, according to the definition. */
    return Neutrons_indices_in_allParticles;
}

////<editor-fold desc="old 2">
//bool GetFDNeutrons(region_part_ptr &particle) {
//    int ParticalPDG = particle->par()->getPid();
//
//    if ((particle->getRegion() == FD) && ((ParticalPDG == 2112) || (ParticalPDG == 22))) { // if neutron/photon is in the FD
//        bool inPCAL = (particle->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
//        bool inECIN = (particle->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
//        bool inECOUT = (particle->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit
//
//        if (!inPCAL) { // if neutral did not hit the PCAL
//            if (inECIN || inECOUT) { // if the criteria for a particle to be a neutron is upheld
//                return true;
//            } // end of if neutron/photon hit the EC but not in PCAL
//        } else { // if neutral hit the PCAL - it is a photon
//            return false;
//        }
//    } else {
//        return false;
//    }
//
//    return false; // if we survied to here, return false (to have non-void function)
//}
////</editor-fold>

////<editor-fold desc="Old">
//vector<int> GetFDNeutrons(vector<region_part_ptr> &allParticles, vector<region_part_ptr> &electron, double Ebeam, DSCuts &Momentum_cuts, DSCuts &Veto_cut) {
//    vector<int> Neutrons_indices_in_allParticles;
//
//    for (int i = 0; i < allParticles.size(); i++) {
//        int ParticalPDG = allParticles[i]->par()->getPid();
//
//        bool isFDNeutron = NeutronECAL_Cut_Veto(allParticles, electron, Ebeam, i, Veto_cut.GetLowerCut());
//
//        if (isFDNeutron) { // if the criteria for a particle to be a neutron is upheld
//            /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
//            double Momentum = allParticles[i]->getP();
//
//            /* Log neutrons above momentum cuts (given by Momentum_cuts): */
//            if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons_indices_in_allParticles.push_back(i); }
//        } // end of if isFDNeutron
//    } // end of loop over allparticle vector
//
//
////    for (int i = 0; i < allParticles.size(); i++) {
////        int ParticalPDG = allParticles[i]->par()->getPid();
////
//////        if ((allParticles[i]->getRegion() == FD) && (ParticalPDG == 2112)) { // if neutron/photon is in the FD
////        if ((allParticles[i]->getRegion() == FD) && ((ParticalPDG == 2112) || (ParticalPDG == 22))) { // if neutron/photon is in the FD
////            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
////            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
////            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit
////
////            if (!inPCAL && (inECIN || inECOUT)) { // if the criteria for a particle to be a neutron is upheld
////                /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
////                double Momentum = allParticles[i]->getP();
////
////                /* Log neutrons above momentum cuts (given by Momentum_cuts): */
////                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons_indices_in_allParticles.push_back(i); }
////            } // end of if neutron/photon hit the EC but not in PCAL
////        } // end of if neutron/photon is in the FD
////    } // end of loop over allparticle vector
//
//    /* Return a vector of the neutrons in allParticles, according to the definition. */
//    return Neutrons_indices_in_allParticles;
//}
////</editor-fold>


#endif //GETFDNEUTRONS_H
