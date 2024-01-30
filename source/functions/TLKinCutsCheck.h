//
// Created by alons on 20/06/2023.
//

#ifndef TLKINCUTSCHECK_H
#define TLKINCUTSCHECK_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include <cstdlib>
#include <iomanip>
#include <chrono>
#include <typeinfo>
#include <sstream>

#include <TFile.h>
#include <TTree.h>
#include <TLorentzVector.h>
#include <TH1.h>
#include <TH2.h>
#include <TLatex.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TDatabasePDG.h>
#include <TApplication.h>
#include <TROOT.h>

#include "GeneralFunctions.h"
#include "AngleCalc/GetBinFromAng.h"
#include "../classes/DSCuts/DSCuts.h"
#include "../constants.h"

#include "clas12reader.h"

using namespace std;

/* TLKinCutsCheck for a general vector of particles */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const vector<int> &FD_nucleon,
                    const DSCuts &FD_nucleon_theta_cut, const DSCuts &FD_nucleon_momentum_cut) {
    auto mcpbank = c12->mcparts();

    if (!apply_kinematical_cuts) {
        return true;
    } else {
        for (int i: FD_nucleon) {
            mcpbank->setEntry(i);

            int particlePDGtmp = mcpbank->getPid();

            double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool Pass_FD_nucleon_theta_cuts = ((Particle_TL_Theta >= FD_nucleon_theta_cut.GetLowerCutConst()) &&
                                               (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCutConst()));
            bool Pass_FD_nucleon_momentum_cuts = ((Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCutConst()) &&
                                                  (Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCutConst()));

            if (!(Pass_FD_nucleon_theta_cuts && Pass_FD_nucleon_momentum_cuts)) { return false; }
        }

        return true;
    }
}

/* TLKinCutsCheck for leading FD neutrons */
bool TLKinCutsCheck(const std::unique_ptr<clas12::clas12reader> &c12, bool apply_kinematical_cuts, const int TL_IDed_neutrons_FD_mom_max,
                    const DSCuts &FD_nucleon_theta_cut, const DSCuts &FD_nucleon_momentum_cut) {
    auto mcpbank = c12->mcparts();

    if (!apply_kinematical_cuts) {
        return true;
    } else {
        if (TL_IDed_neutrons_FD_mom_max == -1) {
            return false;
        } else {
            mcpbank->setEntry(TL_IDed_neutrons_FD_mom_max);

            double Particle_TL_Momentum = rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz());
            double Particle_TL_Theta = acos((mcpbank->getPz()) / rCalc(mcpbank->getPx(), mcpbank->getPy(), mcpbank->getPz())) * 180.0 / pi;
            double Particle_TL_Phi = atan2(mcpbank->getPy(), mcpbank->getPx()) * 180.0 / pi;

            bool Pass_FD_nucleon_momentum_cuts = ((Particle_TL_Momentum >= FD_nucleon_momentum_cut.GetLowerCutConst()) &&
                                                  (Particle_TL_Momentum <= FD_nucleon_momentum_cut.GetUpperCutConst()));
            bool Pass_FD_nucleon_theta_cuts = ((Particle_TL_Theta >= FD_nucleon_theta_cut.GetLowerCutConst()) &&
                                               (Particle_TL_Theta <= FD_nucleon_theta_cut.GetUpperCutConst()));

            if (Pass_FD_nucleon_theta_cuts && Pass_FD_nucleon_momentum_cuts) {
                return true;
            } else {
                return false;
            }
        }
    }
}

#endif //TLKINCUTSCHECK_H
