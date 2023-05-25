//
// Created by alons on 23/05/2023.
//

#ifndef GETGOODPROTONS_H
#define GETGOODPROTONS_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../../constants.h"
#include "../../classes/DSCuts/DSCuts.h"

using namespace std;

vector<int> GetGoodProtons(vector<region_part_ptr> &protons, const vector<int> &Protons_ind0,
                           DSCuts &p1_Theta_p_cuts_2p, DSCuts &p2_Theta_p_cuts_2p, DSCuts &phi_p1_p2_diff_cuts_2p) {
    vector<int> GoodProtons;

    for (int i = 0; i < Protons_ind0.size(); i++) {
        bool GoodProt = true;

        for (int j = i + 1; j < Protons_ind0.size(); j++) {
            if ((protons[Protons_ind0.at(i)]->getRegion() == CD) && (protons[Protons_ind0.at(j)]->getRegion() == CD)) {
                TVector3 p1_hit_pos, p2_hit_pos, pos_diff; // hit position in the CTOF, and position difference

                p1_hit_pos.SetXYZ(protons[Protons_ind0.at(i)]->sci(clas12::CTOF)->getX(), protons[Protons_ind0.at(i)]->sci(clas12::CTOF)->getY(),
                                  protons[Protons_ind0.at(i)]->sci(clas12::CTOF)->getZ());
                p2_hit_pos.SetXYZ(protons[Protons_ind0.at(j)]->sci(clas12::CTOF)->getX(), protons[Protons_ind0.at(j)]->sci(clas12::CTOF)->getY(),
                                  protons[Protons_ind0.at(j)]->sci(clas12::CTOF)->getZ());
                pos_diff.SetXYZ(p1_hit_pos.Px() - p2_hit_pos.Px(), p1_hit_pos.Py() - p2_hit_pos.Py(), p1_hit_pos.Pz() - p2_hit_pos.Pz());

                if (pos_diff.Mag() == 0) { GoodProt = false; }
            } else if (((protons[Protons_ind0.at(i)]->getRegion() == FD) && (protons[Protons_ind0.at(j)]->getRegion() == CD)) ||
                       ((protons[Protons_ind0.at(i)]->getRegion() == CD) && (protons[Protons_ind0.at(j)]->getRegion() == FD))) {
                double Theta_p_i = protons[Protons_ind0.at(i)]->getTheta() * 180.0 / pi, Phi_p_i = protons[Protons_ind0.at(i)]->getPhi() * 180.0 / pi;
                double Theta_p_j = protons[Protons_ind0.at(j)]->getTheta() * 180.0 / pi, Phi_p_j = protons[Protons_ind0.at(j)]->getPhi() * 180.0 / pi;
                double dPhi = Phi_p_i - Phi_p_j;

                bool p_i_around_40 = (fabs(Theta_p_i - p1_Theta_p_cuts_2p.GetMean()) < p1_Theta_p_cuts_2p.GetUpperCut());
                bool p_j_around_40 = (fabs(Theta_p_j - p2_Theta_p_cuts_2p.GetMean()) < p2_Theta_p_cuts_2p.GetUpperCut());
                bool small_dPhi = (fabs(dPhi - phi_p1_p2_diff_cuts_2p.GetMean()) < phi_p1_p2_diff_cuts_2p.GetUpperCut());

                if ((p_i_around_40 && p_j_around_40) && small_dPhi) { GoodProt = false; }
            }
        }

        if (GoodProt) { GoodProtons.push_back(Protons_ind0.at(i)); }
    }

    //<editor-fold desc="Safety check">
    if (GoodProtons.size() > Protons_ind0.size()) {
        cout << "\n\nGetGoodProtons(): GoodProtons.size() can't be greater than Protons_ind0.size()! exiting...\n\n", exit(EXIT_FAILURE);
    }
    //</editor-fold>

    return GoodProtons;
}

#endif //GETGOODPROTONS_H
