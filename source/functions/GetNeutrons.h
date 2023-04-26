//
// Created by alons on 24/04/2023.
//

#ifndef GETNEUTRONS_H
#define GETNEUTRONS_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../classes/DSCuts/DSCuts.h"
#include "NeutronECAL_Cut_Veto.h"

using namespace std;

vector<int> GetNeutrons(vector<region_part_ptr> &allParticles, DSCuts &Momentum_cuts) {
    vector<int> Neutrons;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticalPDG = allParticles[i]->par()->getPid();

//        if ((allParticles[i]->getRegion() == FD) && (allParticles[i]->par()->getCharge() == 0)) { // if particle hit the FD
        if ((allParticles[i]->getRegion() == FD) && ((ParticalPDG == 2112) || (ParticalPDG == 22))) { // if particle hit the FD
//            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getEnergy() != 0.); // Non-zero energy deposit in PCAL (PCAL hit)
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if (!inPCAL && (inECIN || inECOUT)) {
                double Momentum = allParticles[i]->getP();

                // Log neutrons above momentum cuts (given by Momentum_cuts):
                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons.push_back(i); }
            } // end of if neutron/photon hit the EC but not in PCAL
        } // end of if neutron/photon is in the FD

/*
        if ((allParticles[i]->getRegion() == FD) && (ParticalPDG == 2112)) { // if particle hit the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit
            bool inECIN = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7); // ECIN hit
            bool inECOUT = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            if (!inPCAL && (inECIN || inECOUT)) {
                double Momentum = allParticles[i]->getP();

                // Log neutrons above momentum cuts (given by Momentum_cuts):
                if ((Momentum >= Momentum_cuts.GetLowerCut()) && (Momentum <= Momentum_cuts.GetUpperCut())) { Neutrons.push_back(i); }
            } // end of neutron in the EC but not in PCAL if
        } // end of neutron in the FD if
*/

    } // end of loop over allparticle vector

    return Neutrons;
}


#endif //GETNEUTRONS_H
