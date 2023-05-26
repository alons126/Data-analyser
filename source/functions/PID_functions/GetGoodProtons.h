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

vector<int> GetGoodProtons(vector<region_part_ptr> &protons, const vector<int> &IDProtons_ind,
                           DSCuts &p1_Theta_p_cuts_2p, DSCuts &p2_Theta_p_cuts_2p, DSCuts &phi_p1_p2_diff_cuts_2p) {
    vector<int> GoodProtons;

    bool dCut1 = false;
    bool dCut2 = false;

    for (int i = 0; i < IDProtons_ind.size(); i++) {
        bool GoodProt = true;

        for (int j = i + 1; j < IDProtons_ind.size(); j++) {
            if ((protons[IDProtons_ind.at(i)]->getRegion() == CD) && (protons[IDProtons_ind.at(j)]->getRegion() == CD)) {
                TVector3 p1_hit_pos, p2_hit_pos, pos_diff; // hit position in the CTOF, and position difference

                p1_hit_pos.SetXYZ(protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getX(), protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getY(),
                                  protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getZ());
                p2_hit_pos.SetXYZ(protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getX(), protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getY(),
                                  protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getZ());
                pos_diff.SetXYZ(p1_hit_pos.Px() - p2_hit_pos.Px(), p1_hit_pos.Py() - p2_hit_pos.Py(), p1_hit_pos.Pz() - p2_hit_pos.Pz());

                if (pos_diff.Mag() == 0) {
                    GoodProt = false;
                    dCut1 = true;
                }
            } else if (((protons[IDProtons_ind.at(i)]->getRegion() == FD) && (protons[IDProtons_ind.at(j)]->getRegion() == CD)) ||
                       ((protons[IDProtons_ind.at(i)]->getRegion() == CD) && (protons[IDProtons_ind.at(j)]->getRegion() == FD))) {
                double Theta_p_i = protons[IDProtons_ind.at(i)]->getTheta() * 180.0 / pi, Phi_p_i = protons[IDProtons_ind.at(i)]->getPhi() * 180.0 / pi;
                double Theta_p_j = protons[IDProtons_ind.at(j)]->getTheta() * 180.0 / pi, Phi_p_j = protons[IDProtons_ind.at(j)]->getPhi() * 180.0 / pi;
                double dPhi = Phi_p_i - Phi_p_j;

                bool p_i_around_40 = (fabs(Theta_p_i - p1_Theta_p_cuts_2p.GetMean()) < p1_Theta_p_cuts_2p.GetUpperCut());
                bool p_j_around_40 = (fabs(Theta_p_j - p2_Theta_p_cuts_2p.GetMean()) < p2_Theta_p_cuts_2p.GetUpperCut());
                bool small_dPhi = (fabs(dPhi - phi_p1_p2_diff_cuts_2p.GetMean()) < phi_p1_p2_diff_cuts_2p.GetUpperCut());

                if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                    GoodProt = false;
                    dCut2 = true;
                }
            }
        }

        if (GoodProt) { GoodProtons.push_back(IDProtons_ind.at(i)); }
    }

    //<editor-fold desc="Safety check">
    if (GoodProtons.size() > IDProtons_ind.size()) {
        cout << "\n\nGetGoodProtons(): GoodProtons.size() can't be greater than IDProtons_ind.size()! exiting...\n\n", exit(EXIT_FAILURE);
    }
    //</editor-fold>



    if (dCut1 && dCut2) {
        cout << "\n\nGetGoodProtons(): We have a duble cut!\n";
        cout << "IDProtons_ind.size() = " << IDProtons_ind.size() << "\n";
        cout << "GoodProtons.size() = " << GoodProtons.size() << "\n\n\n";
    }
//    if (dCut1 && dCut2) { cout << "\n\nGetGoodProtons(): We have a duble cut! exiting...\n\n", exit(EXIT_FAILURE); }


    return GoodProtons;
}

#endif //GETGOODPROTONS_H
