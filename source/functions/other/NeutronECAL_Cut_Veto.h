//
// Created by alons on 13/04/2023.
//

#ifndef NEUTRONECAL_CUT_VETO_H
#define NEUTRONECAL_CUT_VETO_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"

#include "../constants.h"

/* This my original veto code as copied from Andrew. */


/* NOTE: this code is valid for a single neutral only. */

/* In the input, we have:
 * const std::unique_ptr<clas12::clas12reader> &c12 - the event
 * Ebeam
 * index - of the particle in question (the neutral) */
bool NeutronECAL_Cut_Veto(const std::unique_ptr<clas12::clas12reader> &c12, double Ebeam, int index) {
    TVector3 p_b(0, 0, Ebeam); /* beam energy */

    auto electrons = c12->getByID(11);
    TVector3 p_e; /* our electron */
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    auto allParticles = c12->getDetParticles();

    if (allParticles[index]->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    //Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(), allParticles[index]->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(), allParticles[index]->cal(detlayer)->getZ());
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = p_n_Angles.Theta() * 180 / pi;
    double theta_q = p_q.Theta() * 180 / pi;
    double theta_nq = p_n_Angles.Angle(p_q) * 180 / pi;

    if (beta < 0) { return false; }
    if (beta > 1.1) { return false; }
/*
    // physics cuts, to be ignored according to Larry.
    if (theta_nq > 25) { return false; }
    if (theta_q > 40) { return false; }
*/
    if (theta_n < 1) { return false; } /* to avoid events with theta_n = 0 (the "1" is in deg) */
    if (!(IC || OC)) { return false; } /* hit only one of these layers */
    if (PC) { return false; } /* to veto out the gammas (photons) */

    //Now let's put a charge particle veto
    bool Veto = false;
    for (int j = 0; j < allParticles.size(); j++) {
        if (allParticles[j]->par()->getCharge() == 0) { continue; } /* looking on charged particles only */
        TVector3 v_chit; /* v_chit = location of charged particle hit */

        if ((detlayer == clas12::ECIN) && (allParticles[j]->cal(clas12::ECIN)->getZ() != 0)) {
            /* if both particles hit the inner calorimeter, use the inner calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(), allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < 100) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(), allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < 100) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < 100) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetos are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}

#endif //NEUTRONECAL_CUT_VETO_H
