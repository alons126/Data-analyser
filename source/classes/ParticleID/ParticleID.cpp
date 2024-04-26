//
// Created by alons on 22/01/2024.
//

#include "ParticleID.h"

// NeutronECAL_Cut_Veto functions ----------------------------------------------------------------------------------------------------------------

//<editor-fold desc="NeutronECAL_Cut_Veto function">
/* NOTE: this code is valid for a single neutral only. */

/* In the input, we have:
 * const std::unique_ptr<clas12::clas12reader> &c12 - the event
 * beamE
 * index - of the particle in question (the neutral) */
bool ParticleID::NeutronECAL_Cut_Veto(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons,
                                      const double &beamE, const int &index, const double &veto_cut) {
    TVector3 p_b(0, 0, beamE); /* beam energy */

    TVector3 p_e; /* our electron */
    p_e.SetMagThetaPhi(electrons[0]->getP(), electrons[0]->getTheta(), electrons[0]->getPhi());
    TVector3 p_q = p_b - p_e; /* 3-momentum transfer */

    if (allParticles[index]->par()->getCharge() != 0) { return false; } /* determine if the particle is neutral or not */

    //Check which layers of the ECAL have been hit
    TVector3 p_n_Angles;
    p_n_Angles.SetMagThetaPhi(1, allParticles[index]->getTheta(),
                              allParticles[index]->getPhi()); /* calculate the angles of the neutral particle */

    /* check where did the particle hit.
     * no hit - we'll get 0
     * we have a hit - we'll get a 7 (7 is the ID of the calorimeter).
       Can also be done by checking deposited energy (double comparison), yet this method is better (int comparison) */
    bool PC = (allParticles[index]->cal(clas12::PCAL)->getDetector() == 7);
    bool IC = (allParticles[index]->cal(clas12::ECIN)->getDetector() == 7);
    bool OC = (allParticles[index]->cal(clas12::ECOUT)->getDetector() == 7);
    auto detlayer = PC ? clas12::PCAL : IC ? clas12::ECIN : clas12::ECOUT; /* determine the earliest layer that the neutral hit in */

    /* v_nhit = location of neutral particle hit */
    TVector3 v_nhit(allParticles[index]->cal(detlayer)->getX(), allParticles[index]->cal(detlayer)->getY(),
                    allParticles[index]->cal(detlayer)->getZ());
    double beta = allParticles[index]->par()->getBeta();
    double gamma = 1 / sqrt(1 - (beta * beta));
    double theta_n = p_n_Angles.Theta() * 180 / pi;
    double theta_q = p_q.Theta() * 180 / pi;
    double theta_nq = p_n_Angles.Angle(p_q) * 180 / pi;

    if (beta < 0) { return false; }
//    if (beta > 1.1) { return false; }
//    // physics cuts, to be ignored according to Larry.
//    if (theta_nq > 25) { return false; }
//    if (theta_q > 40) { return false; }
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
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECIN)->getX(), allParticles[j]->cal(clas12::ECIN)->getY(),
                          allParticles[j]->cal(clas12::ECIN)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else if ((detlayer == clas12::ECOUT) && (allParticles[j]->cal(clas12::ECOUT)->getZ() != 0)) {
            /* if both particles hit the outer calorimeter, use the outer calorimeter to determine v_chit */
            v_chit.SetXYZ(allParticles[j]->cal(clas12::ECOUT)->getX(), allParticles[j]->cal(clas12::ECOUT)->getY(),
                          allParticles[j]->cal(clas12::ECOUT)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        } else {
            /* the neutral has to hit either the ECIN or ECOUT.
               If the charged particle hit the other calorimeter, then look at where the charged particle was expected to be according to the trajectory. */
            int trajlayer = (detlayer == clas12::ECIN) ? 4 : 7;
            v_chit.SetXYZ(allParticles[j]->traj(clas12::ECAL, trajlayer)->getX(), allParticles[j]->traj(clas12::ECAL, trajlayer)->getY(),
                          allParticles[j]->traj(clas12::ECAL, trajlayer)->getZ());
            TVector3 v_dist = v_nhit - v_chit;

            if (v_dist.Mag() < veto_cut) { Veto = true; }
        }
    }

    if (Veto) { return false; } /* if any of the vetoes are true, return false */

    return true; /* we survived up to this point, we do have a neutral particle */
}
//</editor-fold>

// ChargedParticleID function --------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="ChargedParticleID function">
vector<int> ParticleID::ChargedParticleID(vector <region_part_ptr> &Particle, const DSCuts &Momentum_th) {
    vector<int> ChargedParticle;

    for (int i = 0; i < Particle.size(); i++) {
        if (Particle[i]->par()->getCharge() == 0) { // Check that the particle's charge is zero
            cout << "\n\nChargedParticleID: Particle is neutral! Exiting...\n\n", exit(0);
        }

        double Momentum = Particle[i]->getP();

        if (Momentum >= Momentum_th.GetLowerCutConst()) { ChargedParticle.push_back(i); }
//        if ((Momentum >= Momentum_th.GetLowerCutConst()) && (Momentum <= Momentum_th.GetUpperCutConst())) { ChargedParticle.push_back(i); }
    }

    return ChargedParticle;
}
//</editor-fold>

// FDNeutralParticleID function ------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="FDNeutralParticleID function">

//<editor-fold desc="Get neutrals by new definition (with neutron ECAL veto and ECAL edge cuts)">
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void ParticleID::FDNeutralParticleID(vector <region_part_ptr> allParticles, vector <region_part_ptr> electrons,
                                     vector<int> &FD_Neutrons_within_PID_cuts, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                                     vector<int> &FD_Photons_within_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th,
                                     DSCuts &Neutron_veto_cut, const double &beamE, const double &ECAL_V_edge_cut, const double &ECAL_W_edge_cut,
                                     const bool &apply_nucleon_cuts) {
    for (int &i: ID_Neutrons_FD) { // Identify neutron above momentum threshold
        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */

        int NeutralPDG = allParticles[i]->par()->getPid();
        double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);
        bool Neutron_with_PCAL_hit = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7);   // PCAL hit
        bool Neutron_with_ECIN_hit = (allParticles[i]->cal(clas12::ECIN)->getDetector() == 7);   // ECIN hit
        bool Neutron_with_ECOUT_hit = (allParticles[i]->cal(clas12::ECOUT)->getDetector() == 7); // ECOUT hit
        auto Neutron_ECAL_detlayer = Neutron_with_ECIN_hit ? clas12::ECIN : clas12::ECOUT; // find first layer of hit

        //<editor-fold desc="Safety checks">
        if (!((NeutralPDG == 22) || (NeutralPDG == 2112))) {
            cout << "\n\nFDNeutralParticleID (Neutrons): neutron PDG is not 2112 or 22 (" << NeutralPDG << "). Exiting...\n\n", exit(0);
        }

        if (Neutron_with_PCAL_hit) {
            cout << "\n\nFDNeutralParticleID (Neutrons): redefined neutron is in the PCAL!! Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        bool Neutron_pass_momentum_th = (Momentum >= Neutron_momentum_th.GetLowerCutConst() && Momentum <= Neutron_momentum_th.GetUpperCutConst());
        bool Neutron_pass_ECAL_veto, Neutron_pass_ECAL_edge_cuts;

        if (apply_nucleon_cuts) {
            Neutron_pass_ECAL_veto = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, Neutron_veto_cut.GetLowerCutConst());
            Neutron_pass_ECAL_edge_cuts = (allParticles[i]->cal(Neutron_ECAL_detlayer)->getLv() > ECAL_V_edge_cut &&
                                           allParticles[i]->cal(Neutron_ECAL_detlayer)->getLw() > ECAL_W_edge_cut);
        } else {
            Neutron_pass_ECAL_veto = Neutron_pass_ECAL_edge_cuts = true;
        }

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
        if (Momentum >= Photon_momentum_th.GetLowerCutConst() &&
            Momentum <= Photon_momentum_th.GetUpperCutConst()) { FD_Photons_within_th.push_back(i); }
    } // end of loop over ID_Photons_FD vector
}
//</editor-fold>

//<editor-fold desc="Get neutrals by new definition (ORIGINAL!)">
/* The FDNeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void ParticleID::FDNeutralParticleID(vector <region_part_ptr> allParticles,
                                     vector<int> &FD_Neutrons_within_th, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                                     vector<int> &FD_Photons_within_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th,
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

        /* Log neutrons above momentum cuts (given by Momentum_th): */
        if (Momentum >= Neutron_momentum_th.GetLowerCutConst() &&
            Momentum <= Neutron_momentum_th.GetUpperCutConst()) { FD_Neutrons_within_th.push_back(i); }
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
        if (Momentum >= Photon_momentum_th.GetLowerCutConst() &&
            Momentum <= Photon_momentum_th.GetUpperCutConst()) { FD_Photons_within_th.push_back(i); }
    } // end of loop over ID_Photons_FD vector
}
//</editor-fold>

//<editor-fold desc="Get leading neutron (ORIGINAL!)">
int ParticleID::GetLnFDIndex(vector <region_part_ptr> allParticles, vector<int> &FD_Neutrons_within_th, const bool &apply_nucleon_cuts) {
    bool PrintOut = false;

    double P_max = -1;
    int MaxPIndex = -1;

    bool PrintLog = (PrintOut && (FD_Neutrons_within_th.size() > 0));

    for (int &i: FD_Neutrons_within_th) { // Identify the leading neutron
        double P_temp = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);
        int NeutralPDG_temp = allParticles[i]->par()->getPid();

        //<editor-fold desc="Safety check">
        if (!((NeutralPDG_temp == 22) || (NeutralPDG_temp == 2112))) {
            cout << "\n\nFDNeutralParticleID (Neutrons): neutron PDG is not 2112 or 22 (" << NeutralPDG_temp << "). Exiting...\n\n", exit(0);
        }
        //</editor-fold>

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

    if (PrintLog) {
        cout << "P_max = " << P_max << " (MaxPIndex = " << MaxPIndex << ")\n\n";
        cout << "==========================================================\n\n\n";
    }

    return MaxPIndex;
}
//</editor-fold>

//</editor-fold>

// GetCorrLnFDIndex function ---------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetCorrLnFDIndex function">
/* A function similar to GetLnFDIndex that selects the leading (LnFD) after correction.
   This function might be usefully if the neutron correction factor (1/(1 - mu)) is not increasing/desreasing monotonically! */
int ParticleID::GetCorrLnFDIndex(MomentumResolution &nRes, vector <region_part_ptr> allParticles, vector<int> &FD_Neutrons_within_th,
                                 const bool &apply_nucleon_cuts, const bool &apply_nucleon_SmearAndCorr) {
    bool PrintOut = false;
    bool PrintOutCorr = false;
    bool PrintLog = (PrintOut && (FD_Neutrons_within_th.size() > 0));

    double P_max_ACorr = -1;
    int MaxPIndex_ACorr = -1;
    vector<double> FD_Neutrons_within_th_ACorr;

    // Create a vector of corrected neutron momenta, corresponding to the i-th neutron in FD_Neutrons_within_th:
    for (int i = 0; i < FD_Neutrons_within_th.size(); i++) {
        double P_temp = GetFDNeutronP(allParticles[FD_Neutrons_within_th.at(i)], apply_nucleon_cuts);
        int NeutralPDG_temp = allParticles[FD_Neutrons_within_th.at(i)]->par()->getPid();

        //<editor-fold desc="Safety check">
        if (!((NeutralPDG_temp == 22) || (NeutralPDG_temp == 2112))) {
            cout << "\n\nFDNeutralParticleID (Neutrons): neutron PDG is not 2112 or 22 (" << NeutralPDG_temp << "). Exiting...\n\n", exit(0);
        }
        //</editor-fold>

        double P_temp_ACorr = nRes.NCorr(apply_nucleon_SmearAndCorr, P_temp);

        FD_Neutrons_within_th_ACorr.push_back(P_temp_ACorr);
    }

    //<editor-fold desc="Safety check">
    if (FD_Neutrons_within_th.size() != FD_Neutrons_within_th_ACorr.size()) {
        cout << "\n\nParticleID::GetCorrLnFDIndex: FD_Neutrons_within_th_ACorr have been filled incorrectly! Exiting...\n", exit(0);
    }
    //</editor-fold>

    for (int j = 0; j < FD_Neutrons_within_th_ACorr.size(); j++) { // Identify the leading neutron after correction
        double P_temp_ACorr = FD_Neutrons_within_th_ACorr.at(j);

        if (P_temp_ACorr >= P_max_ACorr) {
            P_max_ACorr = P_temp_ACorr;
            MaxPIndex_ACorr = FD_Neutrons_within_th.at(j);
        }
    }

    if (PrintLog) {
        for (int i = 0; i < FD_Neutrons_within_th.size(); i++) {
            double P_temp0 = GetFDNeutronP(allParticles[FD_Neutrons_within_th.at(i)], apply_nucleon_cuts);

            cout << "P_temp = " << P_temp0 << " (ind = " << FD_Neutrons_within_th.at(i) << ")\n";
        }

        cout << "\n";

        for (int i = 0; i < FD_Neutrons_within_th_ACorr.size(); i++) {
            double P_temp_ACorr0 = FD_Neutrons_within_th_ACorr.at(i);

            cout << "P_temp_ACorr = " << P_temp_ACorr0 << " (ind = " << FD_Neutrons_within_th.at(i) << ")\n";
        }

        cout << "\n";

        cout << "P_max_ACorr = " << P_max_ACorr << " (MaxPIndex_ACorr = " << MaxPIndex_ACorr << ")\n\n";
        cout << "==========================================================\n\n\n";
    }

    return MaxPIndex_ACorr;
}
//</editor-fold>

// FDNeutralParticle functions -------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="FDNeutralParticle functions">
/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

/* FDNeutralParticle without ECAL veto */
void ParticleID::FDNeutralParticle(vector <region_part_ptr> allParticles, vector<int> &ID_Neutrons_FD, vector<int> &ID_Photons_FD) {
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
//</editor-fold>

// GetFDNeutronP functions -----------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFDNeutronP functions">
double ParticleID::GetFDNeutronP(region_part_ptr &Neutron, const bool &apply_nucleon_cuts) {
    double Momentum;

    if (apply_nucleon_cuts) {

        //<editor-fold desc="Get for ver. neutrons & calculate for 'photons'">
        int ParticlePDG = Neutron->par()->getPid();

        if (ParticlePDG == 2112) {
            /* Momentum for neutrons - get from file. */
            Momentum = Neutron->par()->getP();

            //        //<editor-fold desc="how neutron momentum is calculated in clas12">
//        TVector3 P_n;
//        P_n.SetMagThetaPhi(Neutron->getP(), Neutron->getTheta(), Neutron->getPhi());
//        double E_n = sqrt(m_n * m_n + P_n.Mag2());
//        double Path_n = Neutron->getPath();
//
//        /* NOTE: Beta = Beta_from_Momentum */
//        double Beta_n = Neutron->par()->getBeta();
//        double Beta_n_from_Momentum = P_n.Mag() / E_n;
//
//        /* NOTE: Time_from_Momentum = Time_from_Beta_n */
//        double Time_from_Momentum = Path_n / (c * Beta_n_from_Momentum);
//        double Time_from_Beta_n = Path_n / (c * Beta_n);
//
//        double Velocity_n = Path_n / Time_from_Beta_n;
//        double Gamma_n = 1 / sqrt(1 - (Velocity_n * Velocity_n) / (c * c));
//        double Momentum_n = m_n * Beta_n * Gamma_n; // how neutron momentum is calculated in clas12
//        //</editor-fold>

        } else if (ParticlePDG == 22) {
            /* Momentum for "photons" - calculate. */

            /* NOTE: all of these give the same path:
             * Taking dv_nhit = (Neutron->cal(detlayer)->getX() - Neutron->par()->getVx(), X <-> Y,  X <-> Z) and dv_nhit.Mag().
             * Neutron->cal(detlayer)->getPath().
             * Neutron->getPath(). */

            /* NOTE: Neutron->cal(detlayer)->getTime() = Neutron->getTime() */

            double Path_ph = Neutron->getPath();
            double Time_ph = Neutron->getTime(); // bad
//        double Velocity = Path_ph / Time_ph;
            double Beta_ph = Neutron->par()->getBeta();
            double Time_ph_from_Beta_ph = Path_ph / (c * Beta_ph);
            double Velocity_ph = Path_ph / Time_ph_from_Beta_ph;
//        double Gamma_ph = 1 / sqrt(1 - (Velocity_ph * Velocity_ph) / (c * c));
            double Gamma_ph = 1 / sqrt(1 - (Beta_ph * Beta_ph));

            Momentum = m_n * Beta_ph * Gamma_ph;
        }
        //</editor-fold>

    } else {

        //<editor-fold desc="Get momentum for both neutrons and photons from file">
        Momentum = Neutron->par()->getP();
        //</editor-fold>

    }

    return Momentum;
}
//</editor-fold>

// GetFDNeutrons functions -----------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFDNeutrons functions">
/* The GetFDNeutrons function gets neutrons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit. */

vector<int> ParticleID::GetFDNeutrons(vector <region_part_ptr> &allParticles, const DSCuts &Momentum_cuts, const bool &apply_nucleon_cuts) {
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
                if ((Momentum >= Momentum_cuts.GetLowerCutConst()) &&
                    (Momentum <= Momentum_cuts.GetUpperCutConst())) { Neutrons_indices_in_allParticles.push_back(i); }
            } // end of if neutron/photon hit the EC but not in PCAL
        } // end of if neutron/photon is in the FD
    } // end of loop over allparticle vector

    /* Return a vector of the neutrons in allParticles, according to the definition. */
    return Neutrons_indices_in_allParticles;
}
//</editor-fold>

// GetFDPhotons functions ------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetFDPhotons functions">
/* The GetFDPhotons function gets photons from the FD, according to the definition from Larry:
 * Photon = a photon in the FD with a PCAL hit. */

vector<int> ParticleID::GetFDPhotons(vector <region_part_ptr> &allParticles, const DSCuts &Momentum_cuts) {
    vector<int> Photons_indices_in_allParticles;

    for (int i = 0; i < allParticles.size(); i++) {
        int ParticlePDG = allParticles[i]->par()->getPid();

        if ((allParticles[i]->getRegion() == FD) && (ParticlePDG == 22)) { // if photon is in the FD
            bool inPCAL = (allParticles[i]->cal(clas12::PCAL)->getDetector() == 7); // PCAL hit

            if (inPCAL) { // if the criteria for a particle to be a photon is upheld
                /* Particles that get in here are photons. Now we take photons who pass momentum cuts. */
                double Momentum = allParticles[i]->getP();

                /* Log photons above momentum cuts (given by Momentum_cuts): */
                if ((Momentum >= Momentum_cuts.GetLowerCutConst()) &&
                    (Momentum <= Momentum_cuts.GetUpperCutConst())) { Photons_indices_in_allParticles.push_back(i); }
            } // end of if photon hit the EC but not in PCAL
        } // end of if photon is in the FD
    } // end of loop over allparticle vector

    /* Return a vector of the photons in allParticles, according to the definition. */
    return Photons_indices_in_allParticles;
}
//</editor-fold>

// GetGoodParticles functions --------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetGoodParticles functions">
vector<int> ParticleID::GetGoodParticles(vector <region_part_ptr> &Particle, // particle
                                         const DSCuts &Momentum_cuts // corresponding momentum cuts
) {
    vector<int> GoodParticles;

    for (int i = 0; i < Particle.size(); i++) {
        double Momentum = Particle[i]->getP();

        if (Momentum >= Momentum_cuts.GetLowerCutConst()) { GoodParticles.push_back(i); }
//        if ((Momentum >= Momentum_cuts.GetLowerCutConst()) && (Momentum <= Momentum_cuts.GetUpperCutConst())) { GoodParticles.push_back(i); }
    }

    return GoodParticles;
}
//</editor-fold>

// GetGoodProtons functions ----------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="GetGoodProtons functions">

//<editor-fold desc="GetGoodProtons function">
vector<int> ParticleID::GetGoodProtons(const bool &apply_nucleon_cuts, vector <region_part_ptr> &protons, const vector<int> &IDProtons_ind,
                                       const DSCuts &Theta_p1_cuts_2p, const DSCuts &Theta_p2_cuts_2p, const DSCuts &dphi_p1_p2_2p) {
    vector<int> GoodProtons; // good protons vector after the cuts

    /* Monitoring variables */
    bool TwoCutsPrintout = false;                                                                            // set as true to print a massage when both cuts are applied
    bool Cut_sCTOFhp = false, Cut_dCDaFDd = false;                                                                          // variables to indicate which cut is applied

    for (int i = 0; i < IDProtons_ind.size(); i++) {
        bool GoodProt = true; // when GoodProt == true at the end of the loop, the proton's index will be logged

        if (apply_nucleon_cuts) {
            for (int j = i + 1; j < IDProtons_ind.size(); j++) {
                if ((protons[IDProtons_ind.at(i)]->getRegion() == CD) && (protons[IDProtons_ind.at(j)]->getRegion() == CD)) { // if proton pair is in the CD only
                    /* Set hit positions in the CTOF, and position difference: */
                    TVector3 p1_hit_pos, p2_hit_pos, pos_diff;
                    p1_hit_pos.SetXYZ(protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getX(), protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getY(),
                                      protons[IDProtons_ind.at(i)]->sci(clas12::CTOF)->getZ());
                    p2_hit_pos.SetXYZ(protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getX(), protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getY(),
                                      protons[IDProtons_ind.at(j)]->sci(clas12::CTOF)->getZ());
                    pos_diff.SetXYZ(p1_hit_pos.Px() - p2_hit_pos.Px(), p1_hit_pos.Py() - p2_hit_pos.Py(), p1_hit_pos.Pz() - p2_hit_pos.Pz());

                    if (pos_diff.Mag() == 0) { // if protons have the same hit position
                        /* Same hit position for protons i and j - don't log proton i (j will be logged later as a single proton) */
                        GoodProt = false; // don't log proton i
                        Cut_sCTOFhp = true; // monitor sCTOFhp
                    }
                } else if (((protons[IDProtons_ind.at(i)]->getRegion() == FD) && (protons[IDProtons_ind.at(j)]->getRegion() == CD)) ||
                           ((protons[IDProtons_ind.at(i)]->getRegion() == CD) && (protons[IDProtons_ind.at(j)]->getRegion() == FD))) { // if proton pair CD and FD
                    double Theta_p_i = protons[IDProtons_ind.at(i)]->getTheta() * 180.0 / pi, Theta_p_j =
                            protons[IDProtons_ind.at(j)]->getTheta() * 180.0 / pi;
                    double dPhi = CalcdPhi(protons[IDProtons_ind.at(i)], protons[IDProtons_ind.at(j)]);

                    bool p_i_around_40 = (fabs(Theta_p_i - Theta_p1_cuts_2p.GetMeanConst()) < Theta_p1_cuts_2p.GetUpperCutConst());
                    bool p_j_around_40 = (fabs(Theta_p_j - Theta_p2_cuts_2p.GetMeanConst()) < Theta_p2_cuts_2p.GetUpperCutConst());
                    bool small_dPhi = (fabs(dPhi - dphi_p1_p2_2p.GetMeanConst()) < dphi_p1_p2_2p.GetUpperCutConst());

                    if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                        /* Same angles for protons i and j on the border of the CD-FD - don't log proton i (j will be logged later as a single proton) */
                        GoodProt = false; // don't log proton i
                        Cut_dCDaFDd = true; // monitor dCDaFDd
                    }
                }
            }
        }

        if (GoodProt) { GoodProtons.push_back(IDProtons_ind.at(i)); }
    }

    //<editor-fold desc="Safety checks">
    if (!apply_nucleon_cuts && (GoodProtons.size() != IDProtons_ind.size())) {
        cout << "\n\nGetGoodProtons(): GoodProtons and IDProtons_ind are not the same withot neucleon cut! exiting...\n\n", exit(0);
    }

    if (GoodProtons.size() > IDProtons_ind.size()) {
        cout << "\n\nGetGoodProtons(): GoodProtons.size() can't be greater than IDProtons_ind.size()! exiting...\n\n", exit(0);
    }
    //</editor-fold>

    //<editor-fold desc="Monitoring printout">
    if (TwoCutsPrintout && Cut_sCTOFhp && Cut_dCDaFDd) {
        cout << "\n\nGetGoodProtons(): We have a duble cut!\n";
        cout << "IDProtons_ind.size() = " << IDProtons_ind.size() << "\n";
        cout << "GoodProtons.size() = " << GoodProtons.size() << "\n\n\n";
    }
//    if (Cut_sCTOFhp && Cut_dCDaFDd) { cout << "\n\nGetGoodProtons(): We have a duble cut! exiting...\n\n", exit(0); }
    //</editor-fold>

    return GoodProtons;
}
//</editor-fold>

//<editor-fold desc="SetGPMonitoringPlots function">
void ParticleID::SetGPMonitoringPlots(const bool &GoodProtonsMonitorPlots, string CToF_hits_monitoring_2p_Directory, string Double_detection_monitoring_2p_Directory) {
    if (GoodProtonsMonitorPlots) {
// Monitoring histograms definitions ------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Monitoring histograms definitions">

// Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only) ------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only)">
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 2 id. p. BC",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 2 id. protons BC", "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "01a_Theta_pi_pj_VS_ToFi-ToFj_2idp_BC_2p", 0, 180, -3, 3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 2 id. p. RE",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 2 id. protons RE", "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "01b_Theta_pi_pj_VS_ToFi-ToFj_2idp_RE_2p", 0, 180, -3, 3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 3 id. p. BC",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 3 id. protons BC", "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "02a_Theta_pi_pj_VS_ToFi-ToFj_3idp_BC_2p", 0, 180, -3, 3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 3 id. p. AE",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 3 id. protons AE", "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "02b_Theta_pi_pj_VS_ToFi-ToFj_3idp_RE_2p", 0, 180, -3, 3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 4 id. p. BC",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 4 id. protons BC", "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "03a_Theta_pi_pj_VS_ToFi-ToFj_4idp_BC_2p", 0, 180, -3, 3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 4 id. p. AE",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 4 id. protons AE", "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "03b_Theta_pi_pj_VS_ToFi-ToFj_4idp_RE_2p", 0, 180, -3, 3,
                                                        150, 150);
        //</editor-fold>

// Theta_p1_p2 vs. position1-position2 plots (2p, CD only) --------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p, CD only)">
        hTheta_pi_pj_VS_Posi_Posj_BC_2idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 2 id. p. BC",
                                                       "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 2 id. protons BC",
                                                       "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]", CToF_hits_monitoring_2p_Directory,
                                                       "04a_Theta_pi_pj_VS_Posi-Posj_2idp_BC_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_RE_2idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 2 id. p. RE",
                                                       "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 2 id. protons RE",
                                                       "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]", CToF_hits_monitoring_2p_Directory,
                                                       "04b_Theta_pi_pj_VS_Posi-Posj_2idp_RE_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_BC_3idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 3 id. p. BC",
                                                       "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 3 id. protons BC",
                                                       "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]", CToF_hits_monitoring_2p_Directory,
                                                       "05a_Theta_pi_pj_VS_Posi-Posj_3idp_BC_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 3 id. p. AE",
                                                       "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 3 id. protons AE",
                                                       "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]", CToF_hits_monitoring_2p_Directory,
                                                       "05b_Theta_pi_pj_VS_Posi-Posj_3idp_RE_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 4 id. p. BC",
                                                       "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 4 id. protons BC",
                                                       "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]", CToF_hits_monitoring_2p_Directory,
                                                       "06a_Theta_pi_pj_VS_Posi-Posj_4idp_BC_2p", 0, 180, 0, 100, 150, 150);
        hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} for 4 id. p. AE",
                                                       "#theta_{p_{i},p_{j}} vs. Position_{i}-Position_{j} between proton pairs for 4 id. protons AE",
                                                       "#theta_{p_{i},p_{j}} [Deg]", "Position_{i}-Position_{j} [cm]", CToF_hits_monitoring_2p_Directory,
                                                       "06b_Theta_pi_pj_VS_Posi-Posj_4idp_RE_2p", 0, 180, 0, 100, 150, 150);
        //</editor-fold>

// Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD)">
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 2 id. p. BC (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 2 id. p. RE (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons RE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 3 id. p. BC (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 3 id. p. AE (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 4 id. p. BC (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ  for 4 id. p. AE (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;
        //</editor-fold>

// Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD)">
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 2 id. p. BC (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 2 id. p. RE (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons RE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 3 id. p. BC (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 3 id. p. AE (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 4 id. p. BC (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p = new TH2D(
                "#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 4 id. p. AE (All Int., 2p)",
                "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;
        //</editor-fold>

        //</editor-fold>

    }
}
//</editor-fold>

//<editor-fold desc="GPMonitoring function">
void ParticleID::GPMonitoring(const bool &GoodProtonsMonitorPlots, vector <region_part_ptr> &protons, const vector<int> &IDProtons_ind, const vector<int> &Protons_ind,
                              const DSCuts &Theta_p1_cuts_2p, const DSCuts &Theta_p2_cuts_2p, const DSCuts &dphi_p1_p2_2p, const double &Weight) {
    if (GoodProtonsMonitorPlots) {
        for (int i = 0; i < IDProtons_ind.size(); i++) {
            bool Is_sCTOFhp = false, Is_dCDaFDd = false; // variables to indicate which cut is applied

            auto proton_i_2p = protons[IDProtons_ind.at(i)];
            TVector3 proton_i_2p_2p_3v;
            proton_i_2p_2p_3v.SetMagThetaPhi(proton_i_2p->getP(), proton_i_2p->getTheta(), proton_i_2p->getPhi()); // proton i in protons vector
            double Theta_pi = proton_i_2p->getTheta() * 180.0 / pi, Phi_pi = proton_i_2p->getPhi() * 180.0 / pi; // Theta_pi; Phi_pi in deg

            for (int j = i + 1; j < IDProtons_ind.size(); j++) {
                auto proton_j_2p = protons[IDProtons_ind.at(j)];
                TVector3 proton_j_2p_2p_3v;
                proton_j_2p_2p_3v.SetMagThetaPhi(proton_j_2p->getP(), proton_j_2p->getTheta(),
                                                 proton_j_2p->getPhi()); // proton j in protons vector
                double Theta_pj = proton_j_2p->getTheta() * 180.0 / pi, Phi_pj = proton_j_2p->getPhi() * 180.0 / pi; // Theta_pi; Phi_pi in deg

                double Theta_pi_pj_2p = acos((proton_i_2p_2p_3v.Px() * proton_j_2p_2p_3v.Px() + proton_i_2p_2p_3v.Py() * proton_j_2p_2p_3v.Py() +
                                              proton_i_2p_2p_3v.Pz() * proton_j_2p_2p_3v.Pz()) /
                                             (proton_i_2p_2p_3v.Mag() * proton_j_2p_2p_3v.Mag())) * 180.0 / pi; // Theta_pi_pj_2p in deg

                if ((proton_i_2p->getRegion() == CD) && (proton_j_2p->getRegion() == CD)) { // if both 2p protons are in the CD
                    TVector3 pi_hit_pos, pj_hit_pos, pos_diff_ij;
                    pi_hit_pos.SetXYZ(proton_i_2p->sci(clas12::CTOF)->getX(), proton_i_2p->sci(clas12::CTOF)->getY(), proton_i_2p->sci(clas12::CTOF)->getZ());
                    pj_hit_pos.SetXYZ(proton_j_2p->sci(clas12::CTOF)->getX(), proton_j_2p->sci(clas12::CTOF)->getY(), proton_j_2p->sci(clas12::CTOF)->getZ());

                    pos_diff_ij.SetXYZ(pi_hit_pos.Px() - pj_hit_pos.Px(), pi_hit_pos.Py() - pj_hit_pos.Py(), pi_hit_pos.Pz() - pj_hit_pos.Pz());
                    double time_diff_ij = proton_i_2p->getTime() - proton_j_2p->getTime();

                    if (IDProtons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_2idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            ++num_of_RM_2p_events_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_RE_2idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() == 3 && Protons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_3idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            Is_sCTOFhp = true;
                            ++num_of_AD_2p_events_from_3p_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() == 4 && Protons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            Is_sCTOFhp = true;
                            ++num_of_AD_2p_events_from_4p_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() >= 5 && Protons_ind.size() == 2) {
                        if (pos_diff_ij.Mag() == 0) {
                            Is_sCTOFhp = true;
                            ++num_of_AD_2p_events_from_Xp_sCTOFhp;
                        }
                    }
                } else if (((proton_i_2p->getRegion() == FD) && (proton_j_2p->getRegion() == CD)) ||
                           ((proton_i_2p->getRegion() == CD) && (proton_j_2p->getRegion() == FD))) {
                    double dPhi_ij_2p = CalcdPhi(protons[IDProtons_ind.at(i)], protons[IDProtons_ind.at(j)]); // dPhi_ij_2p in deg

                    bool p_i_around_40 = (fabs(Theta_pi - Theta_p1_cuts_2p.GetMeanConst()) < Theta_p1_cuts_2p.GetUpperCutConst());
                    bool p_j_around_40 = (fabs(Theta_pj - Theta_p2_cuts_2p.GetMeanConst()) < Theta_p2_cuts_2p.GetUpperCutConst());
                    bool small_dPhi = (fabs(dPhi_ij_2p - dphi_p1_p2_2p.GetMeanConst()) < dphi_p1_p2_2p.GetUpperCutConst());

                    if (IDProtons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                                hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p->Fill(Theta_pj, Theta_pi);
                            }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            ++num_of_RM_2p_events_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() == 3 && Protons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                                hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p->Fill(Theta_pj, Theta_pi);
                            }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            Is_dCDaFDd = true;
                            ++num_of_AD_2p_events_from_3p_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() == 4 && Protons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                                hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p->Fill(Theta_pj, Theta_pi);
                            }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            Is_dCDaFDd = true;
                            ++num_of_AD_2p_events_from_4p_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() >= 5 && Protons_ind.size() == 2) {
                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            Is_dCDaFDd = true;
                            ++num_of_AD_2p_events_from_Xp_dCDaFDd;
                        }
                    }
                }
            } // end of second for loop over IDProtons_ind (with j)

            if (Is_sCTOFhp && Is_dCDaFDd) { ++num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd; }
        } // end of first for loop over IDProtons_ind (with i)
    }
}
//</editor-fold>

//</editor-fold>

// nParticleID functions -------------------------------------------------------------------------------------------------------------------------

//<editor-fold desc="nParticleID functions">
/* The NeutralParticleID function gets neutrons or photons from the FD, according to the definition from Larry:
 * Neutron = a neutral particle (i.e., neutron or photon) in the FD with no PCal hit and with an ECal hit.
 * Photon = a neutral particle (i.e., neutron or photon) in the FD with a PCal hit. */

void ParticleID::nParticleID(vector <region_part_ptr> &allParticles,
                             vector<int> &ID_Neutrons_FD, const DSCuts &Neutron_momentum_th,
                             vector<int> &ID_Photons_FD, const DSCuts &Photon_momentum_th,
                             const bool &apply_nucleon_cuts) {

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
                    if (Momentum >= Photon_momentum_th.GetLowerCutConst()) { ID_Photons_FD.push_back(i); }
                } else if (!ParticleInPCAL) { // if there is a neutron or a 'photon' without a PCAL hit
                    if (ParticleInECIN || ParticleInECOUT) { // if there is either a ECIN or ECOUT hit -> neutron
//                        ID_Neutrons_FD.push_back(i);
//
                        /* Particles that get in here are neutrons. Now we take neutrons who pass momentum cuts. */
                        double Momentum = GetFDNeutronP(allParticles[i], apply_nucleon_cuts);

                        /* Log neutrons above momentum cuts (given by Momentum_th): */
                        if (Momentum >= Neutron_momentum_th.GetLowerCutConst()) { ID_Neutrons_FD.push_back(i); }
                    }
                }
            } // end of clas12root neutron or 'photon' if
        } // end of neutral and in the FD if
    } // end of loop over allparticle vector
}
//</editor-fold>

// Fill neutron multiplicity plots functions -----------------------------------------------------------------------------------------------------

//TODO: move from this class

//<editor-fold desc="Neutrons by CLAS12PID">
void ParticleID::FillNeutMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight,
                                    const double &beamE, const double &ECAL_veto_cut, hPlot1D &hNeutronMulti_BPID_BV,
                                    hPlot1D &hNeutronMulti_BPID_AV, vector <region_part_ptr> &Neutrons_BPID, hPlot1D &hNeutronMulti_APID_BV,
                                    hPlot1D &hNeutronMulti_APID_AV, vector <region_part_ptr> &Neutrons_APID) {
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_BPID_BV, hNeutronMulti_BPID_AV, Neutrons_BPID);
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_APID_BV, hNeutronMulti_APID_AV, Neutrons_APID);
}

void ParticleID::FillMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight,
                                const double &beamE, const double &ECAL_veto_cut, hPlot1D &hNeutronMulti_BV, hPlot1D &hNeutronMulti_AV,
                                vector <region_part_ptr> &Neutrons) {
    int Multiplicity_BV = Neutrons.size();
    int Multiplicity_AV = 0;

    for (int i = 0; i < allParticles.size(); i++) {
        if ((allParticles[i]->par()->getCharge() == 0) && (allParticles[i]->par()->getPid() == 2112) && (allParticles[i]->getRegion() == FD)) {
            bool NeutronPassVeto_Test = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, ECAL_veto_cut);

            if (NeutronPassVeto_Test) { ++Multiplicity_AV; }
        }
    }

    hNeutronMulti_BV.hFill(Multiplicity_BV, Weight);
    hNeutronMulti_AV.hFill(Multiplicity_AV, Weight);
}
//</editor-fold>

//<editor-fold desc="Neutrons by redefinition">
void ParticleID::FillNeutMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight,
                                    const double &beamE, const double &ECAL_veto_cut, hPlot1D &hNeutronMulti_BPID_BV,
                                    hPlot1D &hNeutronMulti_BPID_AV, vector<int> &Neutrons_BPID, hPlot1D &hNeutronMulti_APID_BV,
                                    hPlot1D &hNeutronMulti_APID_AV, vector<int> &Neutrons_APID) {
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_BPID_BV, hNeutronMulti_BPID_AV, Neutrons_BPID);
    FillMultiPlots(allParticles, electrons, Weight, beamE, ECAL_veto_cut, hNeutronMulti_APID_BV, hNeutronMulti_APID_AV, Neutrons_APID);
}

void ParticleID::FillMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons,
                                const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                                hPlot1D &hNeutronMulti_BV, hPlot1D &hNeutronMulti_AV, vector<int> &Neutrons_indices) {
    int Multiplicity_BV = Neutrons_indices.size();
    int Multiplicity_AV = 0;

    for (int &i: Neutrons_indices) {
        bool NeutronPassVeto_Test = NeutronECAL_Cut_Veto(allParticles, electrons, beamE, i, ECAL_veto_cut);

        if (NeutronPassVeto_Test) { ++Multiplicity_AV; }
    }

    hNeutronMulti_BV.hFill(Multiplicity_BV, Weight);
    hNeutronMulti_AV.hFill(Multiplicity_AV, Weight);
}
//</editor-fold>
