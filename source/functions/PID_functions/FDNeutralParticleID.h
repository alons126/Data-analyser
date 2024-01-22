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
#include "../NeutronECAL_Cut_Veto.h"

using namespace std;

//<editor-fold desc="Get neutrals by new definition (with neutron ECAL veto and ECAL edge cuts)">
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void FDNeutralParticleID(vector <region_part_ptr> allParticles, vector <region_part_ptr> electrons,
                         vector<int> &FD_Neutrons_within_PID_cuts, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                         vector<int> &FD_Photons_within_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th,
                         DSCuts &Neutron_veto_cut, const double &beamE, const double &ECAL_V_edge_cut, const double &ECAL_W_edge_cut,
                         const bool &apply_nucleon_cuts) {
    for (int &i: ID_Neutrons_FD) { // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

        //<editor-fold desc="Safety check">
        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            cout << "\n\nFDNeutralParticleID (Neutrons): neutron PDG is not 2112 or 22 (" << NeutralPDG << "). Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);
        bool Neutron_with_ECIN_hit = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
        bool Neutron_with_ECOUT_hit = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit
        auto Neutron_ECAL_detlayer = Neutron_with_ECIN_hit ? clas12::ECIN
                                                           : clas12::ECOUT;                                // find first layer of hit

        bool Neutron_pass_momentum_th = (Momentum >= Neutron_momentum_th.GetLowerCut() && Momentum <= Neutron_momentum_th.GetUpperCut());
        bool Neutron_pass_ECAL_veto = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCut());
        bool Neutron_pass_ECAL_edge_cuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > ECAL_V_edge_cut &&
                                            allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > ECAL_W_edge_cut);

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Neutron_pass_momentum_th && Neutron_pass_ECAL_veto && Neutron_pass_ECAL_edge_cuts) { FD_Neutrons_within_PID_cuts.push_back(i); }
    } // end of loop over ID_Neutrons_FD vector

    for (int &i: ID_Photons_FD) { // Identify photons above momentum threshold
        /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

        //<editor-fold desc="Safety check">
        if (NeutralPDG != 22) {
            cout << "\n\nFDNeutralParticleID (Photons): photon PDG is not 22 (" << NeutralPDG << "). Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        double Momentum = allParticles[i]->getP();

        /* Log photons above momentum cuts (given by Momentum_cuts): */
        if (Momentum >= Photon_momentum_th.GetLowerCut() && Momentum <= Photon_momentum_th.GetUpperCut()) { FD_Photons_within_th.push_back(i); }
    } // end of loop over ID_Photons_FD vector
}
//</editor-fold>

//<editor-fold desc="Get neutrals by new definition (ORIGINAL!)">
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void FDNeutralParticleID(vector <region_part_ptr> allParticles,
                         vector<int> &FD_Neutrons_within_th, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                         vector<int> &FD_Photons_within_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th,
                         bool apply_nucleon_cuts) {

    for (int &i: ID_Neutrons_FD) { // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

        //<editor-fold desc="Safety check">
        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            cout << "\n\nFDNeutralParticleID (Neutrons): neutron PDG is not 2112 or 22 (" << NeutralPDG << "). Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Momentum >= Neutron_momentum_th.GetLowerCut() && Momentum <= Neutron_momentum_th.GetUpperCut()) { FD_Neutrons_within_th.push_back(i); }
    } // end of loop over ID_Neutrons_FD vector

    for (int &i: ID_Photons_FD) { // Identify photons above momentum threshold
        /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();

        //<editor-fold desc="Safety check">
        if (NeutralPDG != 22) {
            cout << "\n\nFDNeutralParticleID (Photons): photon PDG is not 22 (" << NeutralPDG << "). Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        double Momentum = allParticles[i]->getP();

        /* Log photons above momentum cuts (given by Momentum_cuts): */
        if (Momentum >= Photon_momentum_th.GetLowerCut() && Momentum <= Photon_momentum_th.GetUpperCut()) { FD_Photons_within_th.push_back(i); }
    } // end of loop over ID_Photons_FD vector
}
//</editor-fold>

//<editor-fold desc="Get leading neutron (ORIGINAL!)">
int FDNeutralMaxP(vector <region_part_ptr> allParticles, vector<int> &FD_Neutrons_within_th, const bool &apply_nucleon_cuts) {
    double P_max = -1;
    int MaxPIndex = -1;
    bool PrintLog = false;

    bool PrintOut = (PrintLog && (FD_Neutrons_within_th.size() > 0));

    for (int &i: FD_Neutrons_within_th) { // Identified neutron above momentum threshold
        double P_temp = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

        if (P_temp >= P_max) {
            P_max = P_temp;
            MaxPIndex = i;
        }

        if (PrintOut) {
            int ParticlePDG_temp = allParticles[i]->par()->getPid();

            bool PCAL_hit_temp = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
            bool ECIN_hit_temp = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
            bool ECOUT_hit_temp = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit

            cout << "P_temp = " << P_temp << " (i = " << i << ", PDG = " << ParticlePDG_temp <<
                 ", PCAL_hit = " << PCAL_hit_temp << ", ECIN_hit = " << ECIN_hit_temp << ", ECOUT_hit = " << ECOUT_hit_temp << ")\n";
        }
    }

    if (PrintOut) {
        cout << "P_max = " << P_max << " (MaxPIndex = " << MaxPIndex << ")\n\n";
        cout << "==========================================================\n\n\n";
    }

    return MaxPIndex;
}
//</editor-fold>

#endif //FDNEUTRALPARTICLEID_H
