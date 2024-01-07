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
#include "../../classes/hPlots/hPlot1D.h"
#include "../../classes/hPlots/hPlot2D.h"
#include "../AngleCalc/CalcdPhi.h"

using namespace std;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                       GetGoodProtons function                                                                       //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//<editor-fold desc="GetGoodProtons function">
vector<int> GetGoodProtons(bool apply_nucleon_cuts, vector<region_part_ptr> &protons, const vector<int> &IDProtons_ind,
                           DSCuts &Theta_p1_cuts_2p, DSCuts &Theta_p2_cuts_2p, DSCuts &dphi_p1_p2_2p) {
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
                    double Theta_p_i = protons[IDProtons_ind.at(i)]->getTheta() * 180.0 / pi, Theta_p_j = protons[IDProtons_ind.at(j)]->getTheta() * 180.0 / pi;
                    double dPhi = CalcdPhi(protons[IDProtons_ind.at(i)], protons[IDProtons_ind.at(j)]);

                    bool p_i_around_40 = (fabs(Theta_p_i - Theta_p1_cuts_2p.GetMean()) < Theta_p1_cuts_2p.GetUpperCut());
                    bool p_j_around_40 = (fabs(Theta_p_j - Theta_p2_cuts_2p.GetMean()) < Theta_p2_cuts_2p.GetUpperCut());
                    bool small_dPhi = (fabs(dPhi - dphi_p1_p2_2p.GetMean()) < dphi_p1_p2_2p.GetUpperCut());

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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                        Monitor good protons                                                                         //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// ======================================================================================================================================================================
// Monitoring histograms declarations
// ======================================================================================================================================================================

//<editor-fold desc="Monitoring histograms definitions">
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

//</editor-fold>

// ======================================================================================================================================================================
// SetGPMonitoringPlots function
// ======================================================================================================================================================================

//<editor-fold desc="SetGPMonitoringPlots function">
void SetGPMonitoringPlots(bool GoodProtonsMonitorPlots, string CToF_hits_monitoring_2p_Directory, string Double_detection_monitoring_2p_Directory) {
    if (GoodProtonsMonitorPlots) {
// Monitoring histograms definitions ------------------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Monitoring histograms definitions">

// Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only) ------------------------------------------------------------------------------------------------------------------------

        //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p, CD only)">
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_2idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 2 id. p. BC",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 2 id. protons BC",
                                                        "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "01a_Theta_pi_pj_VS_ToFi-ToFj_2idp_BC_2p", 0, 180, -3,
                                                        3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_RE_2idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 2 id. p. RE",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 2 id. protons RE",
                                                        "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "01b_Theta_pi_pj_VS_ToFi-ToFj_2idp_RE_2p", 0, 180, -3,
                                                        3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_3idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 3 id. p. BC",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 3 id. protons BC",
                                                        "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "02a_Theta_pi_pj_VS_ToFi-ToFj_3idp_BC_2p", 0, 180, -3,
                                                        3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 3 id. p. AE",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 3 id. protons AE",
                                                        "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "02b_Theta_pi_pj_VS_ToFi-ToFj_3idp_RE_2p", 0, 180, -3,
                                                        3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 4 id. p. BC",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 4 id. protons BC",
                                                        "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "03a_Theta_pi_pj_VS_ToFi-ToFj_4idp_BC_2p", 0, 180, -3,
                                                        3,
                                                        150, 150);
        hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p = hPlot2D("2p", "CD-CTOF", "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} for 4 id. p. AE",
                                                        "#theta_{p_{i},p_{j}} vs. ToF_{i}-ToF_{j} between proton pairs for 4 id. protons AE",
                                                        "#theta_{p_{i},p_{j}} [Deg]",
                                                        "ToF_{i}-ToF_{j} [ns]", CToF_hits_monitoring_2p_Directory, "03b_Theta_pi_pj_VS_ToFi-ToFj_4idp_RE_2p", 0, 180, -3,
                                                        3,
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
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p = new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 2 id. p. BC (All Int., 2p)",
                                                                       "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                                                                       150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p = new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 2 id. p. RE (All Int., 2p)",
                                                                       "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 2 id. protons RE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                                                                       150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_RE_2idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p = new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 3 id. p. BC (All Int., 2p)",
                                                                       "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                                                                       150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p = new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 3 id. p. AE (All Int., 2p)",
                                                                       "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 3 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                                                                       150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p = new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 4 id. p. BC (All Int., 2p)",
                                                                       "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons BC (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                                                                       150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;

        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p = new TH2D("#theta_{p_{i}} vs. #theta_{p_{j}} #forall#theta_{p_{i},p_{j}} for 4 id. p. AE (All Int., 2p)",
                                                                       "#theta_{p_{i}} vs. #theta_{p_{j}} for every #theta_{p_{i},p_{j}}<20#circ between proton pairs for 4 id. protons AE (All Int., 2p);#theta_{p_{j}} [Deg];#theta_{p_{i}} [Deg];",
                                                                       150, 30, 50, 150, 30, 50);
        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p_Dir = Double_detection_monitoring_2p_Directory;
        //</editor-fold>

        //</editor-fold>

    }
}
//</editor-fold>

// ======================================================================================================================================================================
// GPMonitoring function
// ======================================================================================================================================================================

//<editor-fold desc="GPMonitoring function">
/* Counting variables for good protons monitoring */
int num_of_RM_2p_events_sCTOFhp = 0, num_of_AD_2p_events_from_3p_sCTOFhp = 0, num_of_AD_2p_events_from_4p_sCTOFhp = 0;
int num_of_RM_2p_events_dCDaFDd = 0, num_of_AD_2p_events_from_3p_dCDaFDd = 0, num_of_AD_2p_events_from_4p_dCDaFDd = 0;

void GPMonitoring(bool GoodProtonsMonitorPlots, vector<region_part_ptr> &protons, const vector<int> &IDProtons_ind, const vector<int> &Protons_ind,
                  DSCuts &Theta_p1_cuts_2p, DSCuts &Theta_p2_cuts_2p, DSCuts &dphi_p1_p2_2p, double Weight) {
    if (GoodProtonsMonitorPlots) {
        for (int i = 0; i < IDProtons_ind.size(); i++) {
            auto proton_i_2p = protons[IDProtons_ind.at(i)];
            TVector3 proton_i_2p_2p_3v;
            proton_i_2p_2p_3v.SetMagThetaPhi(proton_i_2p->getP(), proton_i_2p->getTheta(), proton_i_2p->getPhi());                          // proton i in protons vector
            double Theta_pi = proton_i_2p->getTheta() * 180.0 / pi, Phi_pi = proton_i_2p->getPhi() * 180.0 / pi;                            // Theta_pi; Phi_pi in deg

            for (int j = i + 1; j < IDProtons_ind.size(); j++) {
                auto proton_j_2p = protons[IDProtons_ind.at(j)];
                TVector3 proton_j_2p_2p_3v;
                proton_j_2p_2p_3v.SetMagThetaPhi(proton_j_2p->getP(), proton_j_2p->getTheta(), proton_j_2p->getPhi());                      // proton j in protons vector
                double Theta_pj = proton_j_2p->getTheta() * 180.0 / pi, Phi_pj = proton_j_2p->getPhi() * 180.0 / pi;                        // Theta_pi; Phi_pi in deg

                double Theta_pi_pj_2p = acos((proton_i_2p_2p_3v.Px() * proton_j_2p_2p_3v.Px() + proton_i_2p_2p_3v.Py() * proton_j_2p_2p_3v.Py() +
                                              proton_i_2p_2p_3v.Pz() * proton_j_2p_2p_3v.Pz()) /
                                             (proton_i_2p_2p_3v.Mag() * proton_j_2p_2p_3v.Mag())) * 180.0 / pi;                                  // Theta_pi_pj_2p in deg
                double dPhi_ij_2p = Phi_pi - Phi_pj;                                                                                             // dPhi_ij_2p in deg

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
                            ++num_of_AD_2p_events_from_3p_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_AE_3idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_AE_3idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }

                    if (IDProtons_ind.size() == 4 && Protons_ind.size() == 2) {
                        hdTheta_pi_pj_VS_ToFi_ToFj_BC_4idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                        hTheta_pi_pj_VS_Posi_Posj_BC_4idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);

                        if (pos_diff_ij.Mag() == 0) {
                            ++num_of_AD_2p_events_from_4p_sCTOFhp;
                            hdTheta_pi_pj_VS_ToFi_ToFj_AE_4idp_2p.hFill(Theta_pi_pj_2p, time_diff_ij, Weight);
                            hTheta_pi_pj_VS_Posi_Posj_AE_4idp_2p.hFill(Theta_pi_pj_2p, pos_diff_ij.Mag(), Weight);
                        }
                    }
                } else if (((proton_i_2p->getRegion() == FD) && (proton_j_2p->getRegion() == CD)) ||
                           ((proton_i_2p->getRegion() == CD) && (proton_j_2p->getRegion() == FD))) {

                    bool p_i_around_40 = (fabs(Theta_pi - Theta_p1_cuts_2p.GetMean()) < Theta_p1_cuts_2p.GetUpperCut());
                    bool p_j_around_40 = (fabs(Theta_pj - Theta_p2_cuts_2p.GetMean()) < Theta_p2_cuts_2p.GetUpperCut());
                    bool small_dPhi = (fabs(dPhi_ij_2p - dphi_p1_p2_2p.GetMean()) < dphi_p1_p2_2p.GetUpperCut());

                    if (IDProtons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_2idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) { hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_RE_2idp_2p->Fill(Theta_pj, Theta_pi); }
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

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) { hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_3idp_2p->Fill(Theta_pj, Theta_pi); }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_3idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            ++num_of_AD_2p_events_from_3p_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_3idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }

                    if (IDProtons_ind.size() == 4 && Protons_ind.size() == 2) {
                        if (Theta_pi_pj_2p < 20) {
                            hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_BC_4idp_2p->Fill(Theta_pj, Theta_pi);

                            if ((p_i_around_40 && p_j_around_40) && small_dPhi) { hTheta_pi_vs_theta_pj_for_Theta_pi_pj_20_AE_4idp_2p->Fill(Theta_pj, Theta_pi); }
                        }

                        hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_BC_4idp_2p->Fill(Theta_pj, Theta_pi);

                        if ((p_i_around_40 && p_j_around_40) && small_dPhi) {
                            ++num_of_AD_2p_events_from_4p_dCDaFDd;
                            hTheta_pi_vs_theta_pj_forall_Theta_pi_pj_AE_4idp_2p->Fill(Theta_pj, Theta_pi);
                        }
                    }
                }
            } // end of second for loop over IDProtons_ind (with j)
        } // end of first for loop over IDProtons_ind (with i)
    }
}
//</editor-fold>

#endif //GETGOODPROTONS_H
