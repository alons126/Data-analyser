//
// Created by alons on 06/03/2023.
//

#ifndef PROJECT_CLAS12SIMCUTS_H
#define PROJECT_CLAS12SIMCUTS_H

#include "../source/classes/DSCuts/DSCuts.h"

class CLAS12SimCuts : public DSCuts {
public:
    /* Constructors */
    CLAS12SimCuts(); // Default constructor

    /* Set functions */
    void SetUUSCut(DSCuts &dscuts, double uuscut) { dscuts.SetUpperUSCut(uuscut); }

    void SetLUSCut(DSCuts &dscuts, double luscut) { dscuts.SetLowerUSCut(luscut); }

    /* Get functions */
    double GetUUSCut(DSCuts dscuts) { return dscuts.GetUpperUSCut(); }

    double GetLUSCut(DSCuts dscuts) { return dscuts.GetLowerUSCut(); }

private:
    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
   // DSCuts Nphe_cuts_FD;

    /* Chi2 cuts */
    DSCuts Chi2_Electron_cuts_CD, Chi2_Electron_cuts_FD, Chi2_Proton_cuts_CD, Chi2_Proton_cuts_FD;
    DSCuts Chi2_Kplus_cuts_CD, Chi2_Kplus_cuts_FD, Chi2_Kminus_cuts_CD, Chi2_Kminus_cuts_FD;
    DSCuts Chi2_piplus_cuts_CD, Chi2_piplus_cuts_FD, Chi2_piminus_cuts_CD, Chi2_piminus_cuts_FD;

    /* Vertex cuts */
    DSCuts Vz_cut, dVz_cuts;

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
    DSCuts SF_cuts;

    /* ECAL edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts ECAL_edge_cuts;

    /* DC edge cuts (fiducial cuts ,electrons only, FD) */
    DSCuts DC_edge_cuts;

    /* Momentum cuts */
    DSCuts e_momentum_cuts_2p, p_momentum_cuts_2p;
    DSCuts e_momentum_cuts_MicroBooNE, p_momentum_cuts_MicroBooNE, cpion_momentum_cuts_MicroBooNE;
};

CLAS12SimCuts::CLAS12SimCuts() {
    /* Number of Photo-electrons (Nphe) cuts (electrons only, FD) */
    Nphe_cuts_FD = DSCuts("Nphe", "FD", "Electron", "1e cut", 0, 2, -1);

    /* Chi2 cuts */
    Chi2_Electron_cuts_CD = DSCuts("Chi2", "CD", "Electron", "1e cut", 0, -1, -1);
    Chi2_Electron_cuts_FD = DSCuts("Chi2", "FD", "Electron", "1e cut", 0, -1, -1);

    Chi2_Proton_cuts_CD = DSCuts("Chi2", "CD", "Proton", "1e cut", 0, -1, -1);
    Chi2_Proton_cuts_FD = DSCuts("Chi2", "FD", "Proton", "1e cut", 0, -1, -1);

    Chi2_Kplus_cuts_CD = DSCuts("Chi2", "CD", "Kplus", "1e cut", 0, -1, -1);
    Chi2_Kplus_cuts_FD = DSCuts("Chi2", "CD", "Kplus", "1e cut", 0, -1, -1);

    Chi2_Kminus_cuts_CD = DSCuts("Chi2", "CD", "Kminus", "1e cut", 0, -1, -1);
    Chi2_Kminus_cuts_FD = DSCuts("Chi2", "FD", "Kminus", "1e cut", 0, -1, -1);

    Chi2_piplus_cuts_CD = DSCuts("Chi2", "CD", "piplus", "1e cut", 0, -1, -1);
    Chi2_piplus_cuts_FD = DSCuts("Chi2", "FD", "piplus", "1e cut", 0, -1, -1);

    Chi2_piminus_cuts_CD = DSCuts("Chi2", "CD", "piminus", "1e cut", 0, -1, -1);
    Chi2_piminus_cuts_FD = DSCuts("Chi2", "FD", "piminus", "1e cut", 0, -1, -1);

    /* Vertex cuts */
    Vz_cut = DSCuts("Vertex z componente", "", "", "1e cut", 0, -6, 1);
    dVz_cuts = DSCuts("dVz", "", "", "1e cut", 0, -3, 3);

    /* Sampling Fraction (SF) cuts (electrons only, FD) */
    SF_cuts = DSCuts("SF", "FD", "Electron", "1e cut", 0.248125, 0.20, 0.28);

    /* ECAL edge cuts (fiducial cuts ,electrons only, FD) */
    ECAL_edge_cuts = DSCuts("ECAL edge", "FD", "Electron", "1e cut", 0, 14, -1);

    /* DC edge cuts (fiducial cuts ,electrons only, FD) */
    DC_edge_cuts = DSCuts("DC edge", "FD", "Electron", "1e cut", 0, 10, -1);

    /* Momentum cuts */
    e_momentum_cuts_2p = DSCuts("e_mom_cuts", "FD", "Electron", "1e cut", 0, -1, -1);
    p_momentum_cuts_2p = DSCuts("e_mom_cuts", "FD", "Electron", "1e cut", 0, 0.3, -1);

    e_momentum_cuts_MicroBooNE = DSCuts("e_mom_cuts", "", "Electron", "1e cut", 0, 0.1, 5);
    p_momentum_cuts_MicroBooNE = DSCuts("p_mom_cuts", "", "Proton", "1e cut", 0, 0.3, -1);
    cpion_momentum_cuts_MicroBooNE = DSCuts("cpion_mom_cuts", "", "cpion", "1e cut", 0, 0.3, -1);
}

#endif //PROJECT_CLAS12SIMCUTS_H
