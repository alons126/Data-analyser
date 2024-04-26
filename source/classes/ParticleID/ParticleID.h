//
// Created by alons on 22/01/2024.
//

#ifndef PARTICLEID_H
#define PARTICLEID_H

#include <iostream>
#include <vector>
#include <TF1.h>
#include <math.h>
#include <map>

#include "clas12reader.h"
#include "region_particle.h"

#include "../../constants.h"
#include "../DSCuts/DSCuts.h"
#include "../../functions/PID_functions/GetFDNeutronP.h"
#include "../../functions/NeutronECAL_Cut_Veto.h"
#include "../hPlots/hPlot1D.h"
#include "../hPlots/hPlot2D.h"
#include "../../functions/AngleCalc/CalcdPhi.h"
//#include "../MomentumResolution/MomentumResolution.cpp"

using namespace std;
using namespace clas12;

class ParticleID {
public:

    //TODO: move from here!
    //<editor-fold desc="Good protons monitoring histograms">
    // Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only) ------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only)">
    hPlot2D hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p, hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p;
    hPlot2D hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p, hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p;
    hPlot2D hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p, hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p;
    //</editor-fold>

    // Theta_p1_p2 vs. position1-position2 plots (2p, CD only) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p, CD only)">
    hPlot2D hTheta_pi_pj_VS_Posi_Posj_BC_2idp_2p, hTheta_pi_pj_VS_Posi_Posj_RE_2idp_2p;
    hPlot2D hTheta_pi_pj_VS_Posi_Posj_BC_3idp_2p, hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p;
    hPlot2D hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p, hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p;
    //</editor-fold>

    // Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD)">
    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p;
    string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p;
    string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p;
    string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p;
    string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p;
    string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p;
    string hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p_Dir;
    //</editor-fold>

    // Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD)">
    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p;
    string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p;
    string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p;
    string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p;
    string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p;
    string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p_Dir;

    TH2D *hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p;
    string hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p_Dir;
    //</editor-fold>

    /* Counting variables for good protons monitoring */
    int num_of_RM_2p_events_sCTOFhp = 0, num_of_AD_2p_events_from_3p_sCTOFhp = 0, num_of_AD_2p_events_from_4p_sCTOFhp = 0;
    int num_of_AD_2p_events_from_Xp_sCTOFhp = 0; // events with 5p or more
    int num_of_RM_2p_events_dCDaFDd = 0, num_of_AD_2p_events_from_3p_dCDaFDd = 0, num_of_AD_2p_events_from_4p_dCDaFDd = 0;
    int num_of_AD_2p_events_from_Xp_dCDaFDd = 0; // events with 5p or more

    int num_of_AD_2p_events_from_mixed_sCTOFhp_dCDaFDd = 0; // events with mixed sCTOFhp dCDaFDd
    //</editor-fold>

// Default constructor ---------------------------------------------------------------------------------------------------------------------------

    ParticleID() = default;

// NeutronECAL_Cut_Veto function -----------------------------------------------------------------------------------------------------------------

    bool NeutronECAL_Cut_Veto(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &beamE,
                              const int &index, const double &veto_cut);

// ChargedParticleID function --------------------------------------------------------------------------------------------------------------------

    vector<int> ChargedParticleID(vector <region_part_ptr> &Particle, const DSCuts &Momentum_th);

// FDNeutralParticle function --------------------------------------------------------------------------------------------------------------------

    void FDNeutralParticle(vector <region_part_ptr> allParticles, vector<int> &ID_Neutrons_FD, vector<int> &ID_Photons_FD);

// FDNeutralParticleID function ------------------------------------------------------------------------------------------------------------------

    void FDNeutralParticleID(vector <region_part_ptr> allParticles, vector <region_part_ptr> electrons, vector<int> &FD_Neutrons_within_PID_cuts, vector<int> &ID_Neutrons_FD,
                             DSCuts &Neutron_momentum_th, vector<int> &FD_Photons_within_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th, DSCuts &Neutron_veto_cut,
                             const double &beamE, const double &ECAL_V_edge_cut, const double &ECAL_W_edge_cut, const bool &apply_nucleon_cuts);

    void FDNeutralParticleID(vector <region_part_ptr> allParticles, vector<int> &FD_Neutrons_within_th, vector<int> &ID_Neutrons_FD, DSCuts &Neutron_momentum_th,
                             vector<int> &FD_Photons_within_th, vector<int> &ID_Photons_FD, DSCuts &Photon_momentum_th, const bool &apply_nucleon_cuts);

// GetLnFDIndex function ------------------------------------------------------------------------------------------------------------------------

    int GetLnFDIndex(vector <region_part_ptr> allParticles, vector<int> &FD_Neutrons_within_th, const bool &apply_nucleon_cuts);

    int GetCorrLnFDIndex(MomentumResolution &nRes, vector <region_part_ptr> allParticles, vector<int> &FD_Neutrons_within_th, const bool &apply_nucleon_cuts,
                         const bool &apply_nucleon_SmearAndCorr);

// GetFDNeutronP function ------------------------------------------------------------------------------------------------------------------------

    double GetFDNeutronP(region_part_ptr &Neutron, const bool &apply_nucleon_cuts);

// GetFDNeutrons function ------------------------------------------------------------------------------------------------------------------------

    vector<int> GetFDNeutrons(vector <region_part_ptr> &allParticles, const DSCuts &Momentum_cuts, const bool &apply_nucleon_cuts);

// GetFDPhotons function -------------------------------------------------------------------------------------------------------------------------

    vector<int> GetFDPhotons(vector <region_part_ptr> &allParticles, const DSCuts &Momentum_cuts);

// GetGoodParticles function ---------------------------------------------------------------------------------------------------------------------

    vector<int> GetGoodParticles(vector <region_part_ptr> &Particle, const DSCuts &Momentum_cuts);

// GetGoodProtons function -----------------------------------------------------------------------------------------------------------------------

    vector<int> GetGoodProtons(const bool &apply_nucleon_cuts, vector <region_part_ptr> &protons, const vector<int> &IDProtons_ind, const DSCuts &Theta_p1_cuts_2p,
                               const DSCuts &Theta_p2_cuts_2p, const DSCuts &dphi_pFD_pCD_2p);

    void SetGPMonitoringPlots(const bool &GoodProtonsMonitorPlots, string CToF_hits_monitoring_2p_Directory, string Double_detection_monitoring_2p_Directory);

    void GPMonitoring(const bool &GoodProtonsMonitorPlots, vector <region_part_ptr> &protons, const vector<int> &IDProtons_ind, const vector<int> &Protons_ind,
                      const DSCuts &Theta_p1_cuts_2p, const DSCuts &Theta_p2_cuts_2p, const DSCuts &dphi_p1_p2_2p, const double &Weight);

// nParticleID function --------------------------------------------------------------------------------------------------------------------------

    void nParticleID(vector <region_part_ptr> &allParticles, vector<int> &ID_Neutrons_FD, const DSCuts &Neutron_momentum_th, vector<int> &ID_Photons_FD,
                     const DSCuts &Photon_momentum_th, const bool &apply_nucleon_cuts);

// Fill neutron multiplicity plots functions -----------------------------------------------------------------------------------------------------

    //TODO: move out of this class

    // Neutrons by CLAS12 PID
    void FillNeutMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight, const double &beamE,
                            const double &ECAL_veto_cut, hPlot1D &hNeutronMulti_BPID_BV, hPlot1D &hNeutronMulti_BPID_AV, vector <region_part_ptr> &Neutrons_BPID,
                            hPlot1D &hNeutronMulti_APID_BV, hPlot1D &hNeutronMulti_APID_AV, vector <region_part_ptr> &Neutrons_APID);

    void FillMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                        hPlot1D &hNeutronMulti_BV, hPlot1D &hNeutronMulti_AV, vector <region_part_ptr> &Neutrons);

    // Neutrons by redefinition
    void FillNeutMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight, const double &beamE,
                            const double &ECAL_veto_cut, hPlot1D &hNeutronMulti_BPID_BV, hPlot1D &hNeutronMulti_BPID_AV, vector<int> &Neutrons_BPID,
                            hPlot1D &hNeutronMulti_APID_BV, hPlot1D &hNeutronMulti_APID_AV, vector<int> &Neutrons_APID);

    void FillMultiPlots(vector <region_part_ptr> &allParticles, vector <region_part_ptr> &electrons, const double &Weight, const double &beamE, const double &ECAL_veto_cut,
                        hPlot1D &hNeutronMulti_BV, hPlot1D &hNeutronMulti_AV, vector<int> &Neutrons_indices);

};

#endif //PARTICLEID_H
