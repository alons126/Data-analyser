//
// Created by alons on 23/12/2023.
//

#ifndef PROJECT_HISTOGRAMSTEMP_H
#define PROJECT_HISTOGRAMSTEMP_H

#include "setup/codeSetup.h"
#include "source/classes/DSCuts/DSCuts.h"
#include "source/classes/hPlots/hPlot1D.cpp"
#include "source/classes/hPlots/hPlot2D.cpp"

using namespace std;

class HistogramsTemp {
public:

    //<editor-fold desc="Histogram definitions">
    /* Histogram definitions and setup. */

    cout << "\nDefining histograms...";

// ======================================================================================================================================================================
// Cut parameters plots
// ======================================================================================================================================================================

    //<editor-fold desc="Cut parameters plots">

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Number of Photo-electrons (Nphe) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Number of Photo-electrons (Nphe) histograms (FD only)">

    //<editor-fold desc="Nphe plots (1e cut, FD)">
    hPlot1D hNphe_1e_cut_BC_FD, hNphe_1e_cut_AC_FD;
    //</editor-fold>

    //<editor-fold desc="Nphe plots (1p, FD)">
    hPlot1D hNphe_1p_FD;
    //</editor-fold>

    //<editor-fold desc="Nphe plots (1n, FD)">
    hPlot1D hNphe_1n_FD;
    //</editor-fold>

    //<editor-fold desc="Nphe plots (2p, FD)">
    hPlot1D hNphe_2p_FD;
    //</editor-fold>

    //<editor-fold desc="Nphe plots (pFDpCD, FD)">
    hPlot1D hNphe_pFDpCD_FD;
    //</editor-fold>

    //<editor-fold desc="Nphe plots (nFDpCD, FD)">
    hPlot1D hNphe_nFDpCD_FD;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Chi2 plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Chi2 plots">

    //<editor-fold desc="Chi2 plots (no #(e) cut)">
    /* Plots of chi2 with no cut on number of electrons - NOT used later! */

    hPlot1D hChi2_Electron_FD;

    hPlot1D hChi2_Proton_CD;
    hPlot1D hChi2_Proton_FD;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1e cut)">
    /* Plots of chi2 with no cut on number of electrons - used later to apply cuts with my mechanism (NOT Justin's ana) */

    hPlot1D hChi2_Electron_1e_cut_FD;

    hPlot1D hChi2_Proton_1e_cut_CD;
    hPlot1D hChi2_Proton_1e_cut_FD;

    hPlot1D hChi2_Kplus_1e_cut_CD;
    hPlot1D hChi2_Kplus_1e_cut_FD;

    hPlot1D hChi2_Kminus_1e_cut_CD;
    hPlot1D hChi2_Kminus_1e_cut_FD;

    hPlot1D hChi2_piplus_1e_cut_CD;
    hPlot1D hChi2_piplus_1e_cut_FD;

    hPlot1D hChi2_piminus_1e_cut_CD;
    hPlot1D hChi2_piminus_1e_cut_FD;

    hPlot1D hChi2_deuteron_1e_cut_CD;
    hPlot1D hChi2_deuteron_1e_cut_FD;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1p)">
    hPlot1D hChi2_Electron_1p_FD;
    hPlot1D hChi2_Proton_1p_CD;
    hPlot1D hChi2_Proton_1p_FD;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (1n)">
    hPlot1D hChi2_Electron_1n_FD;
    hPlot1D hChi2_Proton_1n_CD;
    hPlot1D hChi2_Proton_1n_FD;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (2p)">
    hPlot1D hChi2_Electron_2p_FD;
    hPlot1D hChi2_Proton_2p_CD;
    hPlot1D hChi2_Proton_2p_FD;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (pFDpCD)">
    hPlot1D hChi2_Electron_pFDpCD_FD;

    hPlot1D hChi2_Proton_pFDpCD_CD;
    hPlot1D hChi2_Proton_pFDpCD_FD;
    //</editor-fold>

    //<editor-fold desc="Chi2 plots (nFDpCD)">
    hPlot1D hChi2_Electron_nFDpCD_FD;

    hPlot1D hChi2_Proton_nFDpCD_CD;
    hPlot1D hChi2_Proton_nFDpCD_FD;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Vertex plots
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Vertex plots">

    //<editor-fold desc="Vertex plots (1e cut)">
    hPlot1D hVx_Electron_1e_cut_FD;
    hPlot1D hVy_Electron_1e_cut_FD;
    hPlot1D hVz_Electron_1e_cut_FD;

    hPlot1D hVx_Proton_1e_cut_CD;
    hPlot1D hVx_Proton_1e_cut_FD;
    hPlot1D hVy_Proton_1e_cut_CD;
    hPlot1D hVy_Proton_1e_cut_FD;
    hPlot1D hVz_Proton_1e_cut_CD;
    hPlot1D hVz_Proton_1e_cut_FD;

    hPlot1D hVx_Kplus_1e_cut_CD;
    hPlot1D hVx_Kplus_1e_cut_FD;
    hPlot1D hVy_Kplus_1e_cut_CD;
    hPlot1D hVy_Kplus_1e_cut_FD;
    hPlot1D hVz_Kplus_1e_cut_CD;
    hPlot1D hVz_Kplus_1e_cut_FD;

    hPlot1D hVx_Kminus_1e_cut_CD;
    hPlot1D hVx_Kminus_1e_cut_FD;
    hPlot1D hVy_Kminus_1e_cut_CD;
    hPlot1D hVy_Kminus_1e_cut_FD;
    hPlot1D hVz_Kminus_1e_cut_CD;
    hPlot1D hVz_Kminus_1e_cut_FD;

    hPlot1D hVx_piplus_1e_cut_CD;
    hPlot1D hVx_piplus_1e_cut_FD;
    hPlot1D hVy_piplus_1e_cut_CD;
    hPlot1D hVy_piplus_1e_cut_FD;
    hPlot1D hVz_piplus_1e_cut_CD;
    hPlot1D hVz_piplus_1e_cut_FD;

    hPlot1D hVx_piminus_1e_cut_CD;
    hPlot1D hVx_piminus_1e_cut_FD;
    hPlot1D hVy_piminus_1e_cut_CD;
    hPlot1D hVy_piminus_1e_cut_FD;
    hPlot1D hVz_piminus_1e_cut_CD;
    hPlot1D hVz_piminus_1e_cut_FD;

    hPlot1D hVx_Deuteron_1e_cut_CD;
    hPlot1D hVx_Deuteron_1e_cut_FD;
    hPlot1D hVy_Deuteron_1e_cut_CD;
    hPlot1D hVy_Deuteron_1e_cut_FD;
    hPlot1D hVz_Deuteron_1e_cut_CD;
    hPlot1D hVz_Deuteron_1e_cut_FD;
    //</editor-fold>

    //<editor-fold desc="dV plots (1e cut)">
    hPlot1D hdVx_Proton_FD_1e_cut;
    hPlot1D hdVy_Proton_FD_1e_cut;
    hPlot1D hdVz_Proton_FD_1e_cut;
    hPlot1D hdVx_Proton_CD_1e_cut;
    hPlot1D hdVy_Proton_CD_1e_cut;
    hPlot1D hdVz_Proton_CD_1e_cut;

    hPlot1D hdVx_Kplus_FD_1e_cut;
    hPlot1D hdVy_Kplus_FD_1e_cut;
    hPlot1D hdVz_Kplus_FD_1e_cut;
    hPlot1D hdVx_Kplus_CD_1e_cut;
    hPlot1D hdVy_Kplus_CD_1e_cut;
    hPlot1D hdVz_Kplus_CD_1e_cut;

    hPlot1D hdVx_Kminus_FD_1e_cut;
    hPlot1D hdVy_Kminus_FD_1e_cut;
    hPlot1D hdVz_Kminus_FD_1e_cut;
    hPlot1D hdVx_Kminus_CD_1e_cut;
    hPlot1D hdVy_Kminus_CD_1e_cut;
    hPlot1D hdVz_Kminus_CD_1e_cut;

    hPlot1D hdVx_piplus_FD_1e_cut;
    hPlot1D hdVy_piplus_FD_1e_cut;
    hPlot1D hdVz_piplus_FD_1e_cut;
    hPlot1D hdVx_piplus_CD_1e_cut;
    hPlot1D hdVy_piplus_CD_1e_cut;
    hPlot1D hdVz_piplus_CD_1e_cut;

    hPlot1D hdVx_piminus_FD_1e_cut;
    hPlot1D hdVy_piminus_FD_1e_cut;
    hPlot1D hdVz_piminus_FD_1e_cut;
    hPlot1D hdVx_piminus_CD_1e_cut;
    hPlot1D hdVy_piminus_CD_1e_cut;
    hPlot1D hdVz_piminus_CD_1e_cut;

    hPlot1D hdVx_Deuteron_FD_1e_cut;
    hPlot1D hdVy_Deuteron_FD_1e_cut;
    hPlot1D hdVz_Deuteron_FD_1e_cut;
    hPlot1D hdVx_Deuteron_CD_1e_cut;
    hPlot1D hdVy_Deuteron_CD_1e_cut;
    hPlot1D hdVz_Deuteron_CD_1e_cut;
    //</editor-fold>

    //<editor-fold desc="dV plots (1p)">
    hPlot1D hdVx_1p;
    hPlot1D hdVy_1p;
    hPlot1D hdVz_1p;
    //</editor-fold>

    //<editor-fold desc="dV plots (1n)">
    hPlot1D hdVx_1n;
    hPlot1D hdVy_1n;
    hPlot1D hdVz_1n;
    //</editor-fold>

    //<editor-fold desc="dV plots (2p)">
    hPlot1D hdVx_2p;
    hPlot1D hdVy_2p;
    hPlot1D hdVz_2p;
    //</editor-fold>

    //<editor-fold desc="dV plots (pFDpCD)">
    hPlot1D hdVx_pFDpCD;
    hPlot1D hdVy_pFDpCD;
    hPlot1D hdVz_pFDpCD;

    hPlot1D hdVx_pFD_pFDpCD;
    hPlot1D hdVy_pFD_pFDpCD;
    hPlot1D hdVz_pFD_pFDpCD;

    hPlot1D hdVx_pCD_pFDpCD;
    hPlot1D hdVy_pCD_pFDpCD;
    hPlot1D hdVz_pCD_pFDpCD;
    //</editor-fold>

    //<editor-fold desc="dV plots (nFDpCD)">
    hPlot1D hdVx_nFDpCD;
    hPlot1D hdVy_nFDpCD;
    hPlot1D hdVz_nFDpCD;

    hPlot1D hdVx_pCD_nFDpCD;
    hPlot1D hdVy_pCD_nFDpCD;
    hPlot1D hdVz_pCD_nFDpCD;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Sampling Fraction (SF) histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Sampling Fraction (SF) histograms (FD)">

    //<editor-fold desc="Sampling Fraction (SF) histograms (1e cut, FD only)">

    //<editor-fold desc="SF plots (1e cut, FD)">
    hPlot1D hSF_1e_cut_BC_FD, hSF_1e_cut_AC_FD;
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1e cut, FD)">
    hPlot2D hSF_VS_P_e_1e_cut_BC_FD, hSF_VS_P_e_1e_cut_AC_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (1p, FD only)">

    //<editor-fold desc="SF plots (1p, FD)">
    hPlot1D hSF_1p_FD;
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1p, FD)">
    hPlot2D hSF_VS_P_e_1p_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (1n, FD only)">

    //<editor-fold desc="SF plots (1n, FD)">
    hPlot1D hSF_1n_FD;
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (1n, FD)">
    hPlot2D hSF_VS_P_e_1n_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (2p, FD only)">

    //<editor-fold desc="SF plots (2p, FD)">
    hPlot1D hSF_2p_FD;
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (2p, FD)">
    hPlot2D hSF_VS_P_e_2p_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (pFDpCD, FD only)">

    //<editor-fold desc="SF plots (pFDpCD, FD)">
    hPlot1D hSF_pFDpCD_FD;
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (pFDpCD, FD)">
    hPlot2D hSF_VS_P_e_pFDpCD_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) histograms (nFDpCD, FD only)">

    //<editor-fold desc="SF plots (nFDpCD, FD)">
    hPlot1D hSF_nFDpCD_FD;
    //</editor-fold>

    //<editor-fold desc="SF vs. P plots (nFDpCD, FD)">
    hPlot2D hSF_VS_P_e_nFDpCD_FD;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// ECAL edge histograms (FD only)
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="ECAL edge histograms (FD only)">

    //<editor-fold desc="ECAL coordinates vs. SF plots (1e cut, FD)">
    hPlot2D hVcal_VS_EoP_1e_cut_BC_PCAL, hVcal_VS_EoP_1e_cut_AC_PCAL, hWcal_VS_EoP_1e_cut_BC_PCAL, hWcal_VS_EoP_1e_cut_AC_PCAL;
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (1p, FD)">
    hPlot2D hVcal_VS_EoP_1p_PCAL;
    hPlot2D hWcal_VS_EoP_1p_PCAL;
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (1n, FD)">
    hPlot2D hVcal_VS_EoP_1n_PCAL;
    hPlot2D hWcal_VS_EoP_1n_PCAL;
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (2p, FD)">
    hPlot2D hVcal_VS_EoP_2p_PCAL;
    hPlot2D hWcal_VS_EoP_2p_PCAL;
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (pFDpCD, FD)">
    hPlot2D hVcal_VS_EoP_pFDpCD_PCAL;
    hPlot2D hWcal_VS_EoP_pFDpCD_PCAL;
    //</editor-fold>

    //<editor-fold desc="ECAL coordinates vs. SF plots vs. SF plots (nFDpCD, FD)">
    hPlot2D hVcal_VS_EoP_nFDpCD_PCAL;
    hPlot2D hWcal_VS_EoP_nFDpCD_PCAL;
    //</editor-fold>

    //</editor-fold>

// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Momentum threshold histograms
// ----------------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Momentum threshold histograms">
    /* Here are the definitions for the Momentum threshold plots. These are used to check that momentum cuts were applied in 1e_cut and 2p plots. */

    //<editor-fold desc="Momentum threshold plots (1e cut)">

    //<editor-fold desc="Momentum threshold plots (1e cut)">
    hPlot1D hP_e_1e_cut_FD;

    hPlot1D hP_p_1e_cut_CD;
    hPlot1D hP_p_1e_cut_FD;

    hPlot1D hP_piplus_1e_cut_CD;
    hPlot1D hP_piplus_1e_cut_FD;

    hPlot1D hP_piminus_1e_cut_CD;
    hPlot1D hP_piminus_1e_cut_FD;

    hPlot1D hP_Kplus_1e_cut_CD;
    hPlot1D hP_Kplus_1e_cut_FD;

    hPlot1D hP_Kminus_1e_cut_CD;
    hPlot1D hP_Kminus_1e_cut_FD;

    hPlot1D hP_deuteron_1e_cut_CD;
    hPlot1D hP_deuteron_1e_cut_FD;
    //</editor-fold>

    //<editor-fold desc="Reco momentum plots for momentum thresholds (1e cut)">
    hPlot1D hP_e_reco_1e_cut_FD;
    hPlot1D hP_e_reco_1e_cut_FD_ZOOMIN;

    hPlot1D hP_p_reco_1e_cut_FD;
    hPlot1D hP_p_reco_1e_cut_CD;
    hPlot1D hP_p_reco_1e_cut_FD_ZOOMIN;
    hPlot1D hP_p_reco_1e_cut_CD_ZOOMIN;

    hPlot1D hP_n_reco_1e_cut_FD;
    hPlot1D hP_n_reco_1e_cut_FD_ZOOMIN;

    hPlot1D hP_piplus_reco_1e_cut;
    hPlot1D hP_piplus_reco_1e_cut_ZOOMIN;
    hPlot1D hP_piplus_reco_1e_cut_FD;
    hPlot1D hP_piplus_reco_1e_cut_CD;
    hPlot1D hP_piplus_reco_1e_cut_FD_ZOOMIN;
    hPlot1D hP_piplus_reco_1e_cut_CD_ZOOMIN;

    hPlot1D hP_piminus_reco_1e_cut;
    hPlot1D hP_piminus_reco_1e_cut_ZOOMIN;
    hPlot1D hP_piminus_reco_1e_cut_FD;
    hPlot1D hP_piminus_reco_1e_cut_CD;
    hPlot1D hP_piminus_reco_1e_cut_FD_ZOOMIN;
    hPlot1D hP_piminus_reco_1e_cut_CD_ZOOMIN;

    hPlot1D hP_ph_reco_1e_cut_FD;
    hPlot1D hP_ph_reco_1e_cut_FD_ZOOMIN;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1p)">
    hPlot1D hP_e_APID_1p_FD;
    hPlot1D hP_e_BPID_1p_FD;

    hPlot1D hP_p_APID_1p_CD;
    hPlot1D hP_p_APIDandPS_1p_CD;
    hPlot1D hP_p_BPID_1p_CD;
    hPlot1D hP_p_APID_1p_FD;
    hPlot1D hP_p_APIDandPS_1p_FD;
    hPlot1D hP_p_BPID_1p_FD;

    hPlot1D hP_pFD_APID_1p;
    hPlot1D hP_pFD_APIDandPS_1p;

    hPlot1D hP_piplus_APID_1p_CD;
    hPlot1D hP_piplus_BPID_1p_CD;
    hPlot1D hP_piplus_APID_1p_FD;
    hPlot1D hP_piplus_BPID_1p_FD;

    hPlot1D hP_piminus_APID_1p_CD;
    hPlot1D hP_piminus_BPID_1p_CD;
    hPlot1D hP_piminus_APID_1p_FD;
    hPlot1D hP_piminus_BPID_1p_FD;

    hPlot1D hP_ph_APID_1p_FD;
    hPlot1D hP_ph_BPID_1p_FD;
    hPlot1D hP_n_APID_1p_FD;
    hPlot1D hP_n_BPID_1p_FD;
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (1n)">
    hPlot1D hP_e_APID_1n_FD;
    hPlot1D hP_e_BPID_1n_FD;

    hPlot1D hP_n_APID_1n_FD;
    hPlot1D hP_n_APID_1n_ZOOMOUT_FD;
    hPlot1D hP_n_BPID_1n_FD;
    hPlot1D hP_n_BPID_1n_ZOOMOUT_FD;
    hPlot1D hP_n_APIDandNS_1n_FD;
    hPlot1D hP_n_APIDandNS_1n_ZOOMOUT_FD;

    hPlot1D hP_nFD_APID_1n;
    hPlot1D hP_nFD_APIDandNS_1n;
    hPlot1D hP_nFD_APID_1n_ZOOMOUT;
    hPlot1D hP_nFD_APIDandNS_1n_ZOOMOUT;

    hPlot1D hP_p_APID_1n_CD;
    hPlot1D hP_p_BPID_1n_CD;
    hPlot1D hP_p_APID_1n_FD;
    hPlot1D hP_p_BPID_1n_FD;

    hPlot1D hP_piplus_APID_1n_CD;
    hPlot1D hP_piplus_BPID_1n_CD;
    hPlot1D hP_piplus_APID_1n_FD;
    hPlot1D hP_piplus_BPID_1n_FD;

    hPlot1D hP_piminus_APID_1n_CD;
    hPlot1D hP_piminus_BPID_1n_CD;
    hPlot1D hP_piminus_APID_1n_FD;
    hPlot1D hP_piminus_BPID_1n_FD;

    hPlot1D hP_n_VN_BPID_1n_FD;
    hPlot1D hP_n_VN_APID_1n_FD;
    hPlot1D hP_n_Ph_BPID_1n_FD;
    hPlot1D hP_n_Ph_APID_1n_FD;

    hPlot1D hP_ph_APID_1n_FD;
    hPlot1D hP_ph_BPID_1n_FD;
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (2p)">

    //<editor-fold desc="Momentum threshold plots (2p)">
    hPlot1D hP_e_APID_2p_FD;
    hPlot1D hP_e_BPID_2p_FD;

    hPlot1D hP_p_APID_2p_CD;
    hPlot1D hP_p_BPID_2p_CD;
    hPlot1D hP_p_APID_2p_FD;
    hPlot1D hP_p_BPID_2p_FD;

    hPlot1D hP_piplus_APID_2p_CD;
    hPlot1D hP_piplus_BPID_2p_CD;
    hPlot1D hP_piplus_APID_2p_FD;
    hPlot1D hP_piplus_BPID_2p_FD;

    hPlot1D hP_piminus_APID_2p_CD;
    hPlot1D hP_piminus_BPID_2p_CD;
    hPlot1D hP_piminus_APID_2p_FD;
    hPlot1D hP_piminus_BPID_2p_FD;

    hPlot1D hP_ph_APID_2p_FD;
    hPlot1D hP_ph_BPID_2p_FD;

    hPlot1D hP_n_APID_2p_FD;
    hPlot1D hP_n_BPID_2p_FD;
    //</editor-fold>

    //<editor-fold desc="Leading (P_1) and recoil (P_2) Momentum threshold plots (2p)">
    hPlot1D hP_p_1_2p;
    hPlot1D hP_p_2_2p;
    //</editor-fold>

    //<editor-fold desc="P1 vs P2 (2p, CD & FD)">
    hPlot2D hP_p_1_vs_P_p_2_2p;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (pFDpCD)">
    hPlot1D hP_e_APID_pFDpCD_FD;
    hPlot1D hP_e_BPID_pFDpCD_FD;

    hPlot1D hP_p_APID_pFDpCD_CD;
    hPlot1D hP_p_BPID_pFDpCD_CD;
    hPlot1D hP_p_APID_pFDpCD_FD;
    hPlot1D hP_p_APIDandPS_pFDpCD_FD;
    hPlot1D hP_p_BPID_pFDpCD_FD;

    hPlot1D hP_piplus_APID_pFDpCD_CD;
    hPlot1D hP_piplus_BPID_pFDpCD_CD;
    hPlot1D hP_piplus_APID_pFDpCD_FD;
    hPlot1D hP_piplus_BPID_pFDpCD_FD;

    hPlot1D hP_piminus_APID_pFDpCD_CD;
    hPlot1D hP_piminus_BPID_pFDpCD_CD;
    hPlot1D hP_piminus_APID_pFDpCD_FD;
    hPlot1D hP_piminus_BPID_pFDpCD_FD;

    hPlot1D hP_ph_APID_pFDpCD_FD;
    hPlot1D hP_ph_BPID_pFDpCD_FD;

    hPlot1D hP_n_APID_pFDpCD_FD;
    hPlot1D hP_n_BPID_pFDpCD_FD;
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots (nFDpCD)">
    hPlot1D hP_e_APID_nFDpCD_FD;
    hPlot1D hP_e_BPID_nFDpCD_FD;

    hPlot1D hP_p_APID_nFDpCD_CD;
    hPlot1D hP_p_BPID_nFDpCD_CD;
    hPlot1D hP_p_APID_nFDpCD_FD;
    hPlot1D hP_p_BPID_nFDpCD_FD;

    hPlot1D hP_piplus_APID_nFDpCD_CD;
    hPlot1D hP_piplus_BPID_nFDpCD_CD;
    hPlot1D hP_piplus_APID_nFDpCD_FD;
    hPlot1D hP_piplus_BPID_nFDpCD_FD;

    hPlot1D hP_piminus_APID_nFDpCD_CD;
    hPlot1D hP_piminus_BPID_nFDpCD_CD;
    hPlot1D hP_piminus_APID_nFDpCD_FD;
    hPlot1D hP_piminus_BPID_nFDpCD_FD;

    hPlot1D hP_ph_APID_nFDpCD_FD;
    hPlot1D hP_ph_BPID_nFDpCD_FD;

    hPlot1D hP_n_APID_nFDpCD_FD;
    hPlot1D hP_n_APIDandNS_nFDpCD_FD;
    hPlot1D hP_n_BPID_nFDpCD_FD;

    hPlot1D hP_nL_APID_nFDpCD_FD;
    hPlot1D hP_nL_APIDandNS_nFDpCD_FD;
    //</editor-fold>

    //<editor-fold desc="Other momentum plots">

    //<editor-fold desc="pFD and pCD momentum plots (pFDpCD)">
    hPlot1D hP_pFD_pFDpCD;
    hPlot1D hP_pCD_pFDpCD;
    hPlot2D hP_pFD_vs_P_pCD_pFDpCD;
    //</editor-fold>

    //<editor-fold desc="nFD and pCD momentum plots (nFDpCD)">
    hPlot1D hP_nFD_nFDpCD;
    hPlot1D hP_pCD_nFDpCD;
    hPlot2D hP_nFD_vs_P_pCD_nFDpCD;
    //</editor-fold>

    //<editor-fold desc="Total and relative nucleon momenta (pFDpCD)">
    hPlot1D hP_tot_pFDpCD;
    hPlot1D hP_rel_pFDpCD;
    hPlot2D hP_tot_vs_P_rel_pFDpCD;

    hPlot1D hP_tot_mu_pFDpCD;
    hPlot1D hP_rel_mu_pFDpCD;
    hPlot2D hP_tot_mu_vs_P_rel_mu_pFDpCD;

    hPlot1D hP_tot_minus_q_pFDpCD;

    hPlot2D hP_tot_minus_q_vs_P_tot_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_pFDpCD;

    hPlot2D hP_tot_minus_q_vs_q_S1_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S2_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S3_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S4_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S5_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S6_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S7_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S8_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S9_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S10_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S11_pFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S12_pFDpCD;
    //</editor-fold>

    //<editor-fold desc="Total and Relative nucleon momenta (nFDpCD)">
    hPlot1D hP_tot_nFDpCD;
    hPlot1D hP_rel_nFDpCD;
    hPlot2D hP_tot_vs_P_rel_nFDpCD;

    hPlot1D hP_tot_mu_nFDpCD;
    hPlot1D hP_rel_mu_nFDpCD;
    hPlot2D hP_tot_mu_vs_P_rel_mu_nFDpCD;

    hPlot1D hP_tot_minus_q_nFDpCD;

    hPlot2D hP_tot_minus_q_vs_P_tot_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_nFDpCD;

    hPlot2D hP_tot_minus_q_vs_q_S1_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S2_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S3_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S4_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S5_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S6_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S7_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S8_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S9_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S10_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S11_nFDpCD;
    hPlot2D hP_tot_minus_q_vs_q_S12_nFDpCD;
    //</editor-fold>

    //<editor-fold desc="Leading and recoil nucleon momentum plots (pFDpCD)">
    hPlot1D hP_pL_pFDpCD;
    hPlot1D hP_pR_pFDpCD;
    hPlot2D hP_pL_vs_P_pR_pFDpCD;
    //</editor-fold>

    //<editor-fold desc="Leading and recoil nucleon momentum plots (nFDpCD)">
    hPlot1D hP_nL_nFDpCD;
    hPlot1D hP_nR_nFDpCD;
    hPlot2D hP_nL_vs_P_nR_nFDpCD;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Beta histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Beta histograms">

    //<editor-fold desc="Beta plots (1n)">

    //<editor-fold desc="Beta of neutrons from 'photons' (1n, FD)">
    hPlot1D hBeta_n_from_ph_01_1n_FD;
    hPlot1D hBeta_n_from_ph_01_1n_ZOOMOUT_FD;

    hPlot1D hBeta_n_from_ph_02_1n_FD;
    hPlot1D hBeta_n_from_ph_02_1n_ZOOMOUT_FD;

    hPlot1D hBeta_n_from_ph_03_1n_FD;
    hPlot1D hBeta_n_from_ph_03_1n_ZOOMOUT_FD;

    hPlot1D hBeta_n_from_ph_04_1n_FD;
    hPlot1D hBeta_n_from_ph_04_1n_ZOOMOUT_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P plots">

    //<editor-fold desc="Beta vs. P (no #(e) cut)">

    //<editor-fold desc="Beta vs. P (no #(e) cut)">
    hPlot2D hBeta_vs_P_CD;
    hPlot2D hBeta_vs_P_FD;

    hPlot2D hBeta_vs_P_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_Protons_Only_CD;
    hPlot2D hBeta_vs_P_Protons_Only_FD;

    hPlot2D hBeta_vs_P_Neutrons_Only_CD;
    hPlot2D hBeta_vs_P_Neutrons_Only_FD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (no #(e) cut)">
    hPlot2D hBeta_vs_P_positive_part_All_e_CD;
    hPlot2D hBeta_vs_P_positive_part_All_e_FD;
    hPlot2D hBeta_vs_P_neutral_part_All_e_CD;
    hPlot2D hBeta_vs_P_neutral_part_All_e_FD;
    hPlot2D hBeta_vs_P_negative_part_All_e_CD;
    hPlot2D hBeta_vs_P_negative_part_All_e_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1e cut)">

    //<editor-fold desc="Beta vs. P for all particles (1e cut)">
    hPlot2D hBeta_vs_P_1e_cut_CD;
    hPlot2D hBeta_vs_P_1e_cut_FD;

    hPlot2D hBeta_vs_P_1e_cut_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_Protons_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_Protons_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_Neutrons_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_Kplus_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_Kplus_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_Kminus_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_Kminus_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_piplus_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_piplus_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_piminus_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_piminus_Only_FD;

    hPlot2D hBeta_vs_P_1e_cut_pizero_Only_CD;
    hPlot2D hBeta_vs_P_1e_cut_pizero_Only_FD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1e cut)">
    hPlot2D hBeta_vs_P_positive_part_1e_cut_CD;
    hPlot2D hBeta_vs_P_positive_part_1e_cut_FD;
    hPlot2D hBeta_vs_P_neutral_part_1e_cut_CD;
    hPlot2D hBeta_vs_P_neutral_part_1e_cut_FD;
    hPlot2D hBeta_vs_P_negative_part_1e_cut_CD;
    hPlot2D hBeta_vs_P_negative_part_1e_cut_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1p)">

    //<editor-fold desc="Beta vs. P for all particles (1p)">
    hPlot2D hBeta_vs_P_1p_CD;
    hPlot2D hBeta_vs_P_1p_FD;

    hPlot2D hBeta_vs_P_1p_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_1p_Protons_Only_CD;
    hPlot2D hBeta_vs_P_1p_Protons_Only_FD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1p)">
    hPlot2D hBeta_vs_P_positive_part_1p_CD;
    hPlot2D hBeta_vs_P_positive_part_1p_FD;
    hPlot2D hBeta_vs_P_neutral_part_1p_CD;
    hPlot2D hBeta_vs_P_neutral_part_1p_FD;
    hPlot2D hBeta_vs_P_negative_part_1p_CD;
    hPlot2D hBeta_vs_P_negative_part_1p_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (1n)">

    //<editor-fold desc="Beta vs. P for all particles (1n)">
    hPlot2D hBeta_vs_P_1n_CD;
    hPlot2D hBeta_vs_P_1n_FD;

    hPlot2D hBeta_vs_P_1n_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_1n_Neutrons_Only_CD;
    hPlot2D hBeta_vs_P_1n_Neutrons_Only_FD;
    hPlot2D hBeta_vs_P_1n_Neutrons_Only_ZOOMOUT_FD;

    hPlot2D hBeta_vs_P_1n_Neutrons_Only_from_photons_FD;

    hPlot2D hBeta_vs_P_1n_Photons_Only_CD;
    hPlot2D hBeta_vs_P_1n_Photons_Only_FD;

    hPlot2D hBeta_vs_P_1n_Protons_Only_CD;
    hPlot2D hBeta_vs_P_1n_Protons_Only_FD;

    hPlot2D hBeta_vs_P_1n_Piplus_Only_CD;
    hPlot2D hBeta_vs_P_1n_Piplus_Only_FD;

    hPlot2D hBeta_vs_P_1n_Piminus_Only_CD;
    hPlot2D hBeta_vs_P_1n_Piminus_Only_FD;

    hPlot2D hBeta_vs_P_1n_Kplus_Only_CD;
    hPlot2D hBeta_vs_P_1n_Kplus_Only_FD;

    hPlot2D hBeta_vs_P_1n_Kminus_Only_CD;
    hPlot2D hBeta_vs_P_1n_Kminus_Only_FD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (1n)">
    hPlot2D hBeta_vs_P_pos_part_1n_CD;
    hPlot2D hBeta_vs_P_pos_part_1n_FD;
    hPlot2D hBeta_vs_P_neut_part_1n_CD;
    hPlot2D hBeta_vs_P_neut_part_1n_FD;
    hPlot2D hBeta_vs_P_neg_part_1n_CD;
    hPlot2D hBeta_vs_P_neg_part_1n_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (2p)">

    //<editor-fold desc="Beta vs. P for all particles (2p)">
    hPlot2D hBeta_vs_P_2p_CD;
    hPlot2D hBeta_vs_P_2p_FD;

    hPlot2D hBeta_vs_P_2p_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_2p_Protons_Only_CD;
    hPlot2D hBeta_vs_P_2p_Protons_Only_FD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (2p)">
    hPlot2D hBeta_vs_P_positive_part_2p_CD;
    hPlot2D hBeta_vs_P_positive_part_2p_FD;
    hPlot2D hBeta_vs_P_neutral_part_2p_CD;
    hPlot2D hBeta_vs_P_neutral_part_2p_FD;
    hPlot2D hBeta_vs_P_negative_part_2p_CD;
    hPlot2D hBeta_vs_P_negative_part_2p_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (pFDpCD)">

    //<editor-fold desc="Beta vs. P for all particles (pFDpCD)">
    hPlot2D hBeta_vs_P_pFDpCD_CD;
    hPlot2D hBeta_vs_P_pFDpCD_FD;

    hPlot2D hBeta_vs_P_pFDpCD_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_pFDpCD_Protons_Only_CD;
    hPlot2D hBeta_vs_P_pFDpCD_Protons_Only_FD;

    hPlot2D hBeta_vs_P_pFD_pFDpCD_Protons_Only_FD;
    hPlot2D hBeta_vs_P_pCD_pFDpCD_Protons_Only_CD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (pFDpCD)">
    hPlot2D hBeta_vs_P_positive_part_pFDpCD_CD;
    hPlot2D hBeta_vs_P_positive_part_pFDpCD_FD;
    hPlot2D hBeta_vs_P_neutral_part_pFDpCD_CD;
    hPlot2D hBeta_vs_P_neutral_part_pFDpCD_FD;
    hPlot2D hBeta_vs_P_negative_part_pFDpCD_CD;
    hPlot2D hBeta_vs_P_negative_part_pFDpCD_FD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Beta vs. P (nFDpCD)">

    //<editor-fold desc="Beta vs. P for all particles (nFDpCD)">
    hPlot2D hBeta_vs_P_nFDpCD_CD;
    hPlot2D hBeta_vs_P_nFDpCD_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Electrons_Only_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_FD;
    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_ZOOMOUT_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Neutrons_Only_from_photons_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Photons_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Photons_Only_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Protons_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Protons_Only_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Piplus_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Piplus_Only_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Piminus_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Piminus_Only_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Kplus_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Kplus_Only_FD;

    hPlot2D hBeta_vs_P_nFDpCD_Kminus_Only_CD;
    hPlot2D hBeta_vs_P_nFDpCD_Kminus_Only_FD;
    //</editor-fold>

    //<editor-fold desc="Beta vs. P by charge (nFDpCD)">
    hPlot2D hBeta_vs_P_pos_part_nFDpCD_CD;
    hPlot2D hBeta_vs_P_pos_part_nFDpCD_FD;
    hPlot2D hBeta_vs_P_neut_part_nFDpCD_CD;
    hPlot2D hBeta_vs_P_neut_part_nFDpCD_FD;
    hPlot2D hBeta_vs_P_neg_part_nFDpCD_CD;
    hPlot2D hBeta_vs_P_neg_part_nFDpCD_FD;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// W histograms
// ======================================================================================================================================================================

    //<editor-fold desc="W histograms">

    //<editor-fold desc="W plots (pFDpCD, CD & FD)">
    THStack *sW_pFDpCD;
    TH1D *hW_All_Int_pFDpCD;
    TH1D *hW_QEL_pFDpCD;
    TH1D *hW_MEC_pFDpCD;
    TH1D *hW_RES_pFDpCD;
    TH1D *hW_DIS_pFDpCD;
    string sW_pFDpCD_Dir;
    string hW_All_Int_pFDpCD_Dir;
    string hW_QEL_pFDpCD_Dir;
    string hW_MEC_pFDpCD_Dir;
    string hW_RES_pFDpCD_Dir;
    string hW_DIS_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="W plots (nFDpCD, CD & FD)">
    THStack *sW_nFDpCD;
    TH1D *hW_All_Int_nFDpCD;
    TH1D *hW_QEL_nFDpCD;
    TH1D *hW_MEC_nFDpCD;
    TH1D *hW_RES_nFDpCD;
    TH1D *hW_DIS_nFDpCD;
    string sW_nFDpCD_Dir;
    string hW_All_Int_nFDpCD_Dir;
    string hW_QEL_nFDpCD_Dir;
    string hW_MEC_nFDpCD_Dir;
    string hW_RES_nFDpCD_Dir;
    string hW_DIS_nFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Angle histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Angle histograms">

// Theta_e --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e histograms">
    THStack *sTheta_e;
    THStack *sTheta_e_1p_FD;
    THStack *sTheta_e_1n_FD;
    THStack *sTheta_e_2p_FD;
    THStack *sTheta_e_pFDpCD_FD;
    THStack *sTheta_e_nFDpCD_FD;

    //<editor-fold desc="Theta_e histograms (no #(e) cut)">
    TH1D *hTheta_e_All_e_FD;
    string hTheta_e_All_e_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (1e cut)">
    TH1D *hTheta_e_1e_cut_FD;
    string hTheta_e_1e_cut_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (1p)">
    TH1D *hTheta_e_All_Int_1p_FD;
    TH1D *hTheta_e_QEL_1p_FD;
    TH1D *hTheta_e_MEC_1p_FD;
    TH1D *hTheta_e_RES_1p_FD;
    TH1D *hTheta_e_DIS_1p_FD;
    string hTheta_e_All_Int_1p_FD_Dir;
    string hTheta_e_QEL_1p_FD_Dir;
    string hTheta_e_MEC_1p_FD_Dir;
    string hTheta_e_RES_1p_FD_Dir;
    string hTheta_e_DIS_1p_FD_Dir;

    TH2D *hTheta_e_VS_P_e_1p_FD;
    TH2D *hTheta_e_VS_W_1p_FD;
    string hTheta_e_VS_P_e_1p_FD_Dir;
    string hTheta_e_VS_W_1p_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (1n)">
    TH1D *hTheta_e_All_Int_1n_FD;
    TH1D *hTheta_e_QEL_1n_FD;
    TH1D *hTheta_e_MEC_1n_FD;
    TH1D *hTheta_e_RES_1n_FD;
    TH1D *hTheta_e_DIS_1n_FD;
    string hTheta_e_All_Int_1n_FD_Dir;
    string hTheta_e_QEL_1n_FD_Dir;
    string hTheta_e_MEC_1n_FD_Dir;
    string hTheta_e_RES_1n_FD_Dir;
    string hTheta_e_DIS_1n_FD_Dir;

    TH2D *hTheta_e_VS_P_e_1n_FD;
    TH2D *hTheta_e_VS_W_1n_FD;
    string hTheta_e_VS_P_e_1n_FD_Dir;
    string hTheta_e_VS_W_1n_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (2p)">
    TH1D *hTheta_e_All_Int_2p_FD;
    TH1D *hTheta_e_QEL_2p_FD;
    TH1D *hTheta_e_MEC_2p_FD;
    TH1D *hTheta_e_RES_2p_FD;
    TH1D *hTheta_e_DIS_2p_FD;
    string hTheta_e_All_Int_2p_FD_Dir;
    string hTheta_e_QEL_2p_FD_Dir;
    string hTheta_e_MEC_2p_FD_Dir;
    string hTheta_e_RES_2p_FD_Dir;
    string hTheta_e_DIS_2p_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (pFDpCD)">
    TH1D *hTheta_e_All_Int_pFDpCD_FD;
    TH1D *hTheta_e_QEL_pFDpCD_FD;
    TH1D *hTheta_e_MEC_pFDpCD_FD;
    TH1D *hTheta_e_RES_pFDpCD_FD;
    TH1D *hTheta_e_DIS_pFDpCD_FD;
    string hTheta_e_All_Int_pFDpCD_FD_Dir;
    string hTheta_e_QEL_pFDpCD_FD_Dir;
    string hTheta_e_MEC_pFDpCD_FD_Dir;
    string hTheta_e_RES_pFDpCD_FD_Dir;
    string hTheta_e_DIS_pFDpCD_FD_Dir;

    TH2D *hTheta_e_VS_P_e_pFDpCD_FD;
    TH2D *hTheta_e_VS_W_pFDpCD_FD;
    string hTheta_e_VS_P_e_pFDpCD_FD_Dir;
    string hTheta_e_VS_W_pFDpCD_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_e histograms (nFDpCD)">
    TH1D *hTheta_e_All_Int_nFDpCD_FD;
    TH1D *hTheta_e_QEL_nFDpCD_FD;
    TH1D *hTheta_e_MEC_nFDpCD_FD;
    TH1D *hTheta_e_RES_nFDpCD_FD;
    TH1D *hTheta_e_DIS_nFDpCD_FD;
    string hTheta_e_All_Int_nFDpCD_FD_Dir;
    string hTheta_e_QEL_nFDpCD_FD_Dir;
    string hTheta_e_MEC_nFDpCD_FD_Dir;
    string hTheta_e_RES_nFDpCD_FD_Dir;
    string hTheta_e_DIS_nFDpCD_FD_Dir;

    TH2D *hTheta_e_VS_P_e_nFDpCD_FD;
    TH2D *hTheta_e_VS_W_nFDpCD_FD;
    string hTheta_e_VS_P_e_nFDpCD_FD_Dir;
    string hTheta_e_VS_W_nFDpCD_FD_Dir;
    //</editor-fold>

    //</editor-fold>

// Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_e histograms">
    THStack *sPhi_e;

    //<editor-fold desc="Phi_e histograms (no #(e) cut)">
    TH1D *hPhi_e_All_e_FD;
    string hPhi_e_All_e_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (1e cut)">
    TH1D *hPhi_e_1e_cut_FD;
    string hPhi_e_1e_cut_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (1p)">
    TH1D *hPhi_e_All_Int_1p_FD;
    TH1D *hPhi_e_QEL_1p_FD;
    TH1D *hPhi_e_MEC_1p_FD;
    TH1D *hPhi_e_RES_1p_FD;
    TH1D *hPhi_e_DIS_1p_FD;
    string hPhi_e_All_Int_1p_FD_Dir;
    string hPhi_e_QEL_1p_FD_Dir;
    string hPhi_e_MEC_1p_FD_Dir;
    string hPhi_e_RES_1p_FD_Dir;
    string hPhi_e_DIS_1p_FD_Dir;

    TH2D *hPhi_e_VS_P_e_1p_FD;
    TH2D *hPhi_e_VS_W_1p_FD;
    string hPhi_e_VS_P_e_1p_FD_Dir;
    string hPhi_e_VS_W_1p_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (1n)">
    TH1D *hPhi_e_All_Int_1n_FD;
    TH1D *hPhi_e_QEL_1n_FD;
    TH1D *hPhi_e_MEC_1n_FD;
    TH1D *hPhi_e_RES_1n_FD;
    TH1D *hPhi_e_DIS_1n_FD;
    string hPhi_e_All_Int_1n_FD_Dir;
    string hPhi_e_QEL_1n_FD_Dir;
    string hPhi_e_MEC_1n_FD_Dir;
    string hPhi_e_RES_1n_FD_Dir;
    string hPhi_e_DIS_1n_FD_Dir;

    TH2D *hPhi_e_VS_P_e_1n_FD;
    TH2D *hPhi_e_VS_W_1n_FD;
    string hPhi_e_VS_P_e_1n_FD_Dir;
    string hPhi_e_VS_W_1n_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (2p)">
    TH1D *hPhi_e_All_Int_2p_FD;
    TH1D *hPhi_e_QEL_2p_FD;
    TH1D *hPhi_e_MEC_2p_FD;
    TH1D *hPhi_e_RES_2p_FD;
    TH1D *hPhi_e_DIS_2p_FD;
    string hPhi_e_All_Int_2p_FD_Dir;
    string hPhi_e_QEL_2p_FD_Dir;
    string hPhi_e_MEC_2p_FD_Dir;
    string hPhi_e_RES_2p_FD_Dir;
    string hPhi_e_DIS_2p_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (pFDpCD)">
    TH1D *hPhi_e_All_Int_pFDpCD_FD;
    TH1D *hPhi_e_QEL_pFDpCD_FD;
    TH1D *hPhi_e_MEC_pFDpCD_FD;
    TH1D *hPhi_e_RES_pFDpCD_FD;
    TH1D *hPhi_e_DIS_pFDpCD_FD;
    string hPhi_e_All_Int_pFDpCD_FD_Dir;
    string hPhi_e_QEL_pFDpCD_FD_Dir;
    string hPhi_e_MEC_pFDpCD_FD_Dir;
    string hPhi_e_RES_pFDpCD_FD_Dir;
    string hPhi_e_DIS_pFDpCD_FD_Dir;

    TH2D *hPhi_e_VS_P_e_pFDpCD_FD;
    TH2D *hPhi_e_VS_W_pFDpCD_FD;
    string hPhi_e_VS_P_e_pFDpCD_FD_Dir;
    string hPhi_e_VS_W_pFDpCD_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="Phi_e histograms (nFDpCD)">
    TH1D *hPhi_e_All_Int_nFDpCD_FD;
    TH1D *hPhi_e_QEL_nFDpCD_FD;
    TH1D *hPhi_e_MEC_nFDpCD_FD;
    TH1D *hPhi_e_RES_nFDpCD_FD;
    TH1D *hPhi_e_DIS_nFDpCD_FD;
    string hPhi_e_All_Int_nFDpCD_FD_Dir;
    string hPhi_e_QEL_nFDpCD_FD_Dir;
    string hPhi_e_MEC_nFDpCD_FD_Dir;
    string hPhi_e_RES_nFDpCD_FD_Dir;
    string hPhi_e_DIS_nFDpCD_FD_Dir;

    TH2D *hPhi_e_VS_P_e_nFDpCD_FD;
    TH2D *hPhi_e_VS_W_nFDpCD_FD;
    string hPhi_e_VS_P_e_nFDpCD_FD_Dir;
    string hPhi_e_VS_W_nFDpCD_FD_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_e vs. Phi_e ----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_e vs. Phi_e">
    /* Theta_e vs. Phi_e histograms (no #(e) cut) */
    TH2D *hTheta_e_VS_Phi_e_All_e_FD;
    string hTheta_e_VS_Phi_e_All_e_FD_Dir;

    /* Theta_e vs. Phi_e histograms (1e cut) */
    TH2D *hTheta_e_VS_Phi_e_1e_cut_FD;
    string hTheta_e_VS_Phi_e_1e_cut_FD_Dir;

    /* Theta_e vs. Phi_e histograms (1p) */
    TH2D *hTheta_e_VS_Phi_e_1p_FD;
    string hTheta_e_VS_Phi_e_1p_FD_Dir;

    /* Theta_e vs. Phi_e histograms (1n) */
    TH2D *hTheta_e_VS_Phi_e_1n_FD;
    string hTheta_e_VS_Phi_e_1n_FD_Dir;

    /* Theta_e vs. Phi_e histograms (2p) */
    TH2D *hTheta_e_VS_Phi_e_2p_FD;
    string hTheta_e_VS_Phi_e_2p_FD_Dir;

    /* Theta_e vs. Phi_e histograms (pFDpCD) */
    TH2D *hTheta_e_VS_Phi_e_pFDpCD_FD;
    string hTheta_e_VS_Phi_e_pFDpCD_FD_Dir;

    /* Theta_e vs. Phi_e histograms (nFDpCD) */
    TH2D *hTheta_e_VS_Phi_e_nFDpCD_FD;
    string hTheta_e_VS_Phi_e_nFDpCD_FD_Dir;
    //</editor-fold>

// Other angle plots ----------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Other angle plots">

    //<editor-fold desc="Other angle plots (1e cut)">

    //<editor-fold desc="Neutron veto plots (1e cut)">

    //<editor-fold desc="CLAS12 neutrons and protons">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1e_cut;
    hPlot2D hdTheta_n_p_VS_dPhi_n_p_Protons_BV_1e_cut;
    //</editor-fold>

    //<editor-fold desc="Electrons and redefined neutrons">

    //<editor-fold desc="Electrons and leading FD neutron">
    hPlot2D hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_BV_1e_cut;
    hPlot2D hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_AV_1e_cut;
    hPlot2D hdTheta_LnFD_e_VS_dPhi_LnFD_e_Electrons_Vetoed_1e_cut;
    //</editor-fold>

    //<editor-fold desc="Electrons and all FD neutrons">
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_1e_cut;
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_1e_cut;
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_1e_cut;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Identified protons and redefined neutrons">

    //<editor-fold desc="Identified protons and leading FD neutron">
    hPlot2D hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_BV_1e_cut;
    hPlot2D hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_AV_1e_cut;
    hPlot2D hdTheta_LnFD_p_VS_dPhi_LnFD_p_Protons_Vetoed_1e_cut;
    //</editor-fold>

    //<editor-fold desc="Identified protons and all FD neutrons">
    hPlot2D hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_BV_1e_cut;
    hPlot2D hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_AV_1e_cut;
    hPlot2D hdTheta_nFD_p_VS_dPhi_nFD_p_Protons_Vetoed_1e_cut;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1p)">

// Theta_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p (1p, FD only)">
    THStack *sTheta_p_1p;
    TH1D *hTheta_p_All_Int_1p;
    string hTheta_p_All_Int_1p_Dir;

    TH2D *hTheta_p_VS_P_p_1p_FD;
    TH2D *hTheta_p_VS_W_1p_FD;
    string hTheta_p_VS_P_p_1p_FD_Dir;
    string hTheta_p_VS_W_1p_FD_Dir;
    //</editor-fold>

// Phi_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_p (1p, FD only)">
    THStack *sPhi_p_1p;
    TH1D *hPhi_p_All_Int_1p;
    string hPhi_p_All_Int_1p_Dir;
    //</editor-fold>

// Theta_p vs. Phi_p ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p vs. Phi_p histograms">
    TH2D *hTheta_p_VS_Phi_p_1p_FD;
    string hTheta_p_VS_Phi_p_1p_FD_Dir;
    //</editor-fold>

// Theta_p_e_p_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_p (1p, FD only)">
    THStack *sTheta_p_e_p_p_1p;
    TH1D *hTheta_p_e_p_p_1p;
    string hTheta_p_e_p_p_1p_Dir;
    //</editor-fold>

// Theta_q_p (1p, FD only) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (1p, FD only)">
    THStack *sTheta_q_p_p_1p;
    TH1D *hTheta_q_p_p_1p;
    string hTheta_q_p_p_1p_Dir;
    //</editor-fold>

// Theta_q_p_p vs. |P_p|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_p vs. |P_p|/|q| (1p, FD only)">
    TH2D *hTheta_q_p_p_vs_p_p_q_1p;
    string hTheta_q_p_p_vs_p_p_q_1p_Dir;
    //</editor-fold>

// Theta_q_p_p vs. |p_N|/|q| (1p, FD only) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_p vs. |p_N|/|q| (1p, FD only)">
    TH2D *hTheta_q_p_p_vs_p_N_q_1p;
    string hTheta_q_p_p_vs_p_N_q_1p_Dir;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1n)">

// Theta_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_n (1n, FD only)">
    THStack *sTheta_n_1n;
    TH1D *hTheta_n_All_Int_1n;
    string hTheta_n_All_Int_1n_Dir;

    TH2D *hTheta_n_VS_P_n_1n_FD;
    TH2D *hTheta_n_VS_W_1n_FD;
    string hTheta_n_VS_P_n_1n_FD_Dir;
    string hTheta_n_VS_W_1n_FD_Dir;
    //</editor-fold>

// Phi_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_n (1n, FD only)">
    THStack *sPhi_n_1n;
    TH1D *hPhi_n_All_Int_1n;
    string hPhi_n_All_Int_1n_Dir;
    //</editor-fold>

// Theta_n vs. Phi_n ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_n vs. Phi_n histograms">
    TH2D *hTheta_n_VS_Phi_n_1n_FD;
    string hTheta_n_VS_Phi_n_1n_FD_Dir;
    //</editor-fold>

// Theta_p_e_p_n (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_n (1n, FD only)">
    THStack *sTheta_p_e_p_n_1n;
    TH1D *hTheta_p_e_p_n_1n;
    string hTheta_p_e_p_n_1n_Dir;
    //</editor-fold>

// Theta_q_p_n (1n, FD only) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n (1n, FD only)">
    THStack *sTheta_q_p_n_1n;
    TH1D *hTheta_q_p_n_1n;
    string hTheta_q_p_n_1n_Dir;
    //</editor-fold>

// Theta_q_p_n vs. |p_n|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n vs. |p_n|/|q| (1n, FD only)">
    TH2D *hTheta_q_p_n_vs_p_n_q_1n;
    string hTheta_q_p_n_vs_p_n_q_1n_Dir;
    //</editor-fold>

// Theta_q_p_n vs. |p_N|/|q| (1n, FD only) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_n vs. |p_N|/|q| (1n, FD only)">
    TH2D *hTheta_q_p_n_vs_p_N_q_1n;
    string hTheta_q_p_n_vs_p_N_q_1n_Dir;
    //</editor-fold>

// Neutron veto plots (1n) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron veto plots (1n)">
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_BV_1n;
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_AV_1n;
    hPlot2D hdTheta_n_e_VS_dPhi_n_e_Electrons_Vetoed_Neutrons_1n;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (1e2pXy)">

// Phi of leading (p1) and recoil (p2) protons --------------------------------------------------------------------------------------------------------------------------

    //TODO: reorganize proprly

    //<editor-fold desc="Phi of leading (p1) and recoil (p2) protons">
    THStack *sPhi_Proton_1e2pXy;
    TH1D *hPhi_p1_1e2pXy_CD;
    TH1D *hPhi_p2_1e2pXy_CD;
    string hPhi_p1_1e2pXy_CD_Dir;
    string hPhi_p2_1e2pXy_CD_Dir;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (2p)">

// Theta_p_e_p_tot (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (2p, CD & FD)">
    THStack *sTheta_p_e_p_tot_2p;
    TH1D *hTheta_p_e_p_tot_2p;
    string hTheta_p_e_p_tot_2p_Dir;
    //</editor-fold>

// Theta_q_p (2p, CD & FD) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (2p, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_2p;
    TH1D *hTheta_q_p_tot_2p;
    string hTheta_q_p_tot_2p_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (2p, CD & FD)">
    THStack *sTheta_q_p_2p;
    TH1D *hTheta_q_p_L_2p;
    TH1D *hTheta_q_p_R_2p;
    string hTheta_q_p_L_2p_Dir;
    string hTheta_q_p_R_2p_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (2p, CD & FD) -------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    TH2D *hTheta_q_p_L_vs_p_L_q_2p;
    string hTheta_q_p_L_vs_p_L_q_2p_Dir;
    //</editor-fold>

// Theta_p1_p2 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 (CD & FD)">
    THStack *sTheta_p1_p2_2p;
    TH1D *hTheta_p1_p2_All_Int_2p;
    TH1D *hTheta_p1_p2_QEL_2p;
    TH1D *hTheta_p1_p2_MEC_2p;
    TH1D *hTheta_p1_p2_RES_2p;
    TH1D *hTheta_p1_p2_DIS_2p;
    string sTheta_p1_p2_2p_Dir;
    string hTheta_p1_p2_All_Int_2p_Dir;
    string hTheta_p1_p2_QEL_2p_Dir;
    string hTheta_p1_p2_MEC_2p_Dir;
    string hTheta_p1_p2_RES_2p_Dir;
    string hTheta_p1_p2_DIS_2p_Dir;
    //</editor-fold>

// Theta_p1_p2 vs. W (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. W (CD & FD)">
    TH2D *hTheta_p1_p2_vs_W_2p;
    string hTheta_p1_p2_vs_W_2p_Dir;
    //</editor-fold>

// Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (CD & FD)">
    TH2D *hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p;
    string hTheta_p1_vs_theta_p2_for_Theta_p1_p2_20_2p_Dir;
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (2p, CD & FD) ----------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for Theta_p1_p2 < 20 (CD & FD)">
    TH2D *hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p;
    string hTheta_pFD_vs_Theta_pCD_for_Theta_pFD_pCD_20_2p_Dir;
    //</editor-fold>

// dPhi_p1_p2 for Theta_p1_p2 < 20 (2p, CD & FD) ------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_p1_p2 for Theta_p1_p2 < 20 (CD & FD)">
    TH1D *hdPhi_p1_p2_for_Theta_p1_p2_20_2p;
    string hdPhi_p1_p2_for_Theta_p1_p2_20_2p_Dir;

    TH1D *hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p;
    string hdPhi_p1_p2_for_Theta_p1_p2_20_ZOOMIN_2p_Dir;
    //</editor-fold>

// dPhi_pFD_pCD for Theta_pFD_pCD < 20 (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_p1_p2 for Theta_pFD_pCD < 20 (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p;
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_2p_Dir;

    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p;
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_2p_Dir;
    //</editor-fold>

// Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (2p, CD & FD) -------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_vs_Theta_p2 for every Theta_p1_p2 (CD & FD)">
    TH2D *hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p;
    string hTheta_p1_vs_theta_p2_forall_Theta_p1_p2_2p_Dir;
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (2p, CD & FD) ---------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (CD & FD)">
    TH2D *hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p;
    string hTheta_pFD_vs_Theta_pCD_forall_Theta_pFD_pCD_2p_Dir;
    //</editor-fold>

// dPhi_p1_p2 for every Theta_p1_p2 (2p, CD & FD) -----------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_p1_p2 for every Theta_p1_p2 (CD & FD)">
    TH1D *hdPhi_p1_p2_for_all_Theta_p1_p2_2p;
    string hdPhi_p1_p2_for_all_Theta_p1_p2_2p_Dir;

    TH1D *hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p;
    string hdPhi_p1_p2_for_all_Theta_p1_p2_ZOOMIN_2p_Dir;

    TH1D *hdPhi_p1_p2_for_small_dTheta_2p;
    string hdPhi_p1_p2_for_small_dTheta_2p_Dir;

    TH1D *hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p;
    string hdPhi_p1_p2_for_small_dTheta_ZOOMIN_2p_Dir;
    //</editor-fold>

// dPhi_pFD_pCD for every Theta_pFD_pCD (2p, CD & FD) --------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_pFD_pCD for every Theta_pFD_pCD (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p;
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_2p_Dir;

    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p;
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_2p_Dir;

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_2p;
    string hdPhi_pFD_pCD_for_small_dTheta_2p_Dir;

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p;
    string hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_2p_Dir;
    //</editor-fold>

// Ghost tracks handling (2p, CD only) ----------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p1_p2 vs. TOF1-TOF2 plots (2p)">
    hPlot2D hTheta_p1_p2_VS_ToF1_ToF2_AC_2p;
    //</editor-fold>

    //<editor-fold desc="Theta_p1_p2 vs. position1-position2 plots (2p)">
    hPlot2D hTheta_p1_p2_VS_Pos1_Pos2_AC_2p;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (pFDpCD)">

    //<editor-fold desc="Nucleons' angles plots (pFDpCD)">

// Theta_pFD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD histograms">
    THStack *sTheta_pFD_pFDpCD_FD;

    TH1D *hTheta_pFD_All_Int_pFDpCD_FD;
    TH1D *hTheta_pFD_QEL_pFDpCD_FD;
    TH1D *hTheta_pFD_MEC_pFDpCD_FD;
    TH1D *hTheta_pFD_RES_pFDpCD_FD;
    TH1D *hTheta_pFD_DIS_pFDpCD_FD;
    string hTheta_pFD_All_Int_pFDpCD_FD_Dir;
    string hTheta_pFD_QEL_pFDpCD_FD_Dir;
    string hTheta_pFD_MEC_pFDpCD_FD_Dir;
    string hTheta_pFD_RES_pFDpCD_FD_Dir;
    string hTheta_pFD_DIS_pFDpCD_FD_Dir;

    TH2D *hTheta_pFD_VS_P_pFD_pFDpCD_FD;
    TH2D *hTheta_pFD_VS_W_pFDpCD_FD;
    string hTheta_pFD_VS_P_pFD_pFDpCD_FD_Dir;
    string hTheta_pFD_VS_W_pFDpCD_FD_Dir;
    //</editor-fold>

// Phi_pFD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_pFD histograms">
    THStack *sPhi_pFD_pFDpCD_FD;

    TH1D *hPhi_pFD_All_Int_pFDpCD_FD;
    TH1D *hPhi_pFD_QEL_pFDpCD_FD;
    TH1D *hPhi_pFD_MEC_pFDpCD_FD;
    TH1D *hPhi_pFD_RES_pFDpCD_FD;
    TH1D *hPhi_pFD_DIS_pFDpCD_FD;
    string hPhi_pFD_All_Int_pFDpCD_FD_Dir;
    string hPhi_pFD_QEL_pFDpCD_FD_Dir;
    string hPhi_pFD_MEC_pFDpCD_FD_Dir;
    string hPhi_pFD_RES_pFDpCD_FD_Dir;
    string hPhi_pFD_DIS_pFDpCD_FD_Dir;

    TH2D *hPhi_pFD_VS_P_pFD_pFDpCD_FD;
    TH2D *hPhi_pFD_VS_W_pFDpCD_FD;
    string hPhi_pFD_VS_P_pFD_pFDpCD_FD_Dir;
    string hPhi_pFD_VS_W_pFDpCD_FD_Dir;
    //</editor-fold>

// Theta_pFD vs. Phi_pFD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD vs. Phi_pFD histograms">
    TH2D *hTheta_pFD_VS_Phi_pFD_pFDpCD_FD;
    string hTheta_pFD_VS_Phi_pFD_pFDpCD_FD_Dir;
    //</editor-fold>

// Theta_pCD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD histograms">
    THStack *sTheta_pCD_pFDpCD_CD;

    TH1D *hTheta_pCD_All_Int_pFDpCD_CD;
    TH1D *hTheta_pCD_QEL_pFDpCD_CD;
    TH1D *hTheta_pCD_MEC_pFDpCD_CD;
    TH1D *hTheta_pCD_RES_pFDpCD_CD;
    TH1D *hTheta_pCD_DIS_pFDpCD_CD;
    string hTheta_pCD_All_Int_pFDpCD_CD_Dir;
    string hTheta_pCD_QEL_pFDpCD_CD_Dir;
    string hTheta_pCD_MEC_pFDpCD_CD_Dir;
    string hTheta_pCD_RES_pFDpCD_CD_Dir;
    string hTheta_pCD_DIS_pFDpCD_CD_Dir;

    TH2D *hTheta_pCD_VS_P_pCD_pFDpCD_CD;
    TH2D *hTheta_pCD_VS_W_pFDpCD_CD;
    string hTheta_pCD_VS_P_pCD_pFDpCD_CD_Dir;
    string hTheta_pCD_VS_W_pFDpCD_CD_Dir;
    //</editor-fold>

// Phi_pCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_pCD histograms">
    THStack *sPhi_pCD_pFDpCD_CD;

    TH1D *hPhi_pCD_All_Int_pFDpCD_CD;
    TH1D *hPhi_pCD_QEL_pFDpCD_CD;
    TH1D *hPhi_pCD_MEC_pFDpCD_CD;
    TH1D *hPhi_pCD_RES_pFDpCD_CD;
    TH1D *hPhi_pCD_DIS_pFDpCD_CD;
    string hPhi_pCD_All_Int_pFDpCD_CD_Dir;
    string hPhi_pCD_QEL_pFDpCD_CD_Dir;
    string hPhi_pCD_MEC_pFDpCD_CD_Dir;
    string hPhi_pCD_RES_pFDpCD_CD_Dir;
    string hPhi_pCD_DIS_pFDpCD_CD_Dir;

    TH2D *hPhi_pCD_VS_P_pCD_pFDpCD_CD;
    TH2D *hPhi_pCD_VS_W_pFDpCD_CD;
    string hPhi_pCD_VS_P_pCD_pFDpCD_CD_Dir;
    string hPhi_pCD_VS_W_pFDpCD_CD_Dir;
    //</editor-fold>

// Theta_pCD vs. Phi_pCD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD vs. Phi_pCD histograms">
    TH2D *hTheta_pCD_VS_Phi_pCD_pFDpCD_CD;
    string hTheta_pCD_VS_Phi_pCD_pFDpCD_CD_Dir;
    //</editor-fold>

// Theta_tot ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot histograms">
    THStack *sTheta_tot_pFDpCD;

    TH1D *hTheta_tot_All_Int_pFDpCD;
    TH1D *hTheta_tot_QEL_pFDpCD;
    TH1D *hTheta_tot_MEC_pFDpCD;
    TH1D *hTheta_tot_RES_pFDpCD;
    TH1D *hTheta_tot_DIS_pFDpCD;
    string hTheta_tot_All_Int_pFDpCD_Dir;
    string hTheta_tot_QEL_pFDpCD_Dir;
    string hTheta_tot_MEC_pFDpCD_Dir;
    string hTheta_tot_RES_pFDpCD_Dir;
    string hTheta_tot_DIS_pFDpCD_Dir;

    TH2D *hTheta_tot_VS_P_tot_pFDpCD;
    TH2D *hTheta_tot_VS_W_pFDpCD;
    string hTheta_tot_VS_P_tot_pFDpCD_Dir;
    string hTheta_tot_VS_W_pFDpCD_Dir;
    //</editor-fold>

// Phi_tot --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_tot histograms">
    THStack *sPhi_tot_pFDpCD;

    TH1D *hPhi_tot_All_Int_pFDpCD;
    TH1D *hPhi_tot_QEL_pFDpCD;
    TH1D *hPhi_tot_MEC_pFDpCD;
    TH1D *hPhi_tot_RES_pFDpCD;
    TH1D *hPhi_tot_DIS_pFDpCD;
    string hPhi_tot_All_Int_pFDpCD_Dir;
    string hPhi_tot_QEL_pFDpCD_Dir;
    string hPhi_tot_MEC_pFDpCD_Dir;
    string hPhi_tot_RES_pFDpCD_Dir;
    string hPhi_tot_DIS_pFDpCD_Dir;

    TH2D *hPhi_tot_VS_P_tot_pFDpCD;
    TH2D *hPhi_tot_VS_W_pFDpCD;
    string hPhi_tot_VS_P_tot_pFDpCD_Dir;
    string hPhi_tot_VS_W_pFDpCD_Dir;
    //</editor-fold>

// Theta_tot vs. Phi_tot ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot vs. Phi_tot histograms">
    TH2D *hTheta_tot_VS_Phi_tot_pFDpCD;
    string hTheta_tot_VS_Phi_tot_pFDpCD_Dir;
    //</editor-fold>

// Theta_rel ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel histograms">
    THStack *sTheta_rel_pFDpCD;

    TH1D *hTheta_rel_All_Int_pFDpCD;
    TH1D *hTheta_rel_QEL_pFDpCD;
    TH1D *hTheta_rel_MEC_pFDpCD;
    TH1D *hTheta_rel_RES_pFDpCD;
    TH1D *hTheta_rel_DIS_pFDpCD;
    string hTheta_rel_All_Int_pFDpCD_Dir;
    string hTheta_rel_QEL_pFDpCD_Dir;
    string hTheta_rel_MEC_pFDpCD_Dir;
    string hTheta_rel_RES_pFDpCD_Dir;
    string hTheta_rel_DIS_pFDpCD_Dir;

    TH2D *hTheta_rel_VS_P_rel_pFDpCD;
    TH2D *hTheta_rel_VS_W_pFDpCD;
    string hTheta_rel_VS_P_rel_pFDpCD_Dir;
    string hTheta_rel_VS_W_pFDpCD_Dir;
    //</editor-fold>

// Phi_rel --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_rel histograms">
    THStack *sPhi_rel_pFDpCD;

    TH1D *hPhi_rel_All_Int_pFDpCD;
    TH1D *hPhi_rel_QEL_pFDpCD;
    TH1D *hPhi_rel_MEC_pFDpCD;
    TH1D *hPhi_rel_RES_pFDpCD;
    TH1D *hPhi_rel_DIS_pFDpCD;
    string hPhi_rel_All_Int_pFDpCD_Dir;
    string hPhi_rel_QEL_pFDpCD_Dir;
    string hPhi_rel_MEC_pFDpCD_Dir;
    string hPhi_rel_RES_pFDpCD_Dir;
    string hPhi_rel_DIS_pFDpCD_Dir;

    TH2D *hPhi_rel_VS_P_rel_pFDpCD;
    TH2D *hPhi_rel_VS_W_pFDpCD;
    string hPhi_rel_VS_P_rel_pFDpCD_Dir;
    string hPhi_rel_VS_W_pFDpCD_Dir;
    //</editor-fold>

// Theta_rel vs. Phi_rel ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel vs. Phi_rel histograms">
    TH2D *hTheta_rel_VS_Phi_rel_pFDpCD;
    string hTheta_rel_VS_Phi_rel_pFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_p_e_p_tot (pFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (pFDpCD, CD & FD)">
    THStack *sTheta_p_e_p_tot_pFDpCD;
    TH1D *hTheta_p_e_p_tot_pFDpCD;
    string hTheta_p_e_p_tot_pFDpCD_Dir;
    //</editor-fold>

// Theta_p_e_p_tot vs. W (pFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot vs. W (pFDpCD)">
    TH2D *hTheta_p_e_p_tot_vs_W_pFDpCD;
    string hTheta_p_e_p_tot_vs_W_pFDpCD_Dir;
    //</editor-fold>

// Theta_q_p_tot (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (pFDpCD, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_pFDpCD;
    TH1D *hTheta_q_p_tot_pFDpCD;
    string hTheta_q_p_tot_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_max (CD & FD)">
    TH1D *hTheta_q_p_max_pFDpCD;
    string hTheta_q_p_max_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_P_pL_minus_q_pR and Theta_q_p_R (pFDpCD, CD & FD)">
    THStack *sTheta_P_pL_minus_q_pR_pFDpCD;
    TH1D *hTheta_P_pL_minus_q_pR_pFDpCD;
    string hTheta_P_pL_minus_q_pR_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (pFDpCD, CD & FD)">
    THStack *sTheta_q_p_L_R_pFDpCD;
    TH1D *hTheta_q_p_L_pFDpCD;
    TH1D *hTheta_q_p_R_pFDpCD;
    string hTheta_q_p_L_pFDpCD_Dir;
    string hTheta_q_p_R_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_pFD and Theta_q_pCD (pFDpCD, CD & FD)">
    THStack *sTheta_q_p_pFDpCD;
    TH1D *hTheta_q_pFD_pFDpCD;
    TH1D *hTheta_q_pCD_pFDpCD;
    string hTheta_q_pFD_pFDpCD_Dir;
    string hTheta_q_pCD_pFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_tot vs. W (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_tot vs. W (CD & FD)">
    TH2D *hTheta_q_p_tot_vs_W_pFDpCD;
    string hTheta_q_p_tot_vs_W_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_P_pL_minus_q_pR vs. W (CD & FD)">
    TH2D *hTheta_P_pL_minus_q_pR_vs_W_pFDpCD;
    string hTheta_P_pL_minus_q_pR_vs_W_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L vs. W (CD & FD)">
    TH2D *hTheta_q_p_L_vs_W_pFDpCD;
    string hTheta_q_p_L_vs_W_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_R vs. W (CD & FD)">
    TH2D *hTheta_q_p_R_vs_W_pFDpCD;
    string hTheta_q_p_R_vs_W_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_pFD vs. W (CD & FD)">
    TH2D *hTheta_q_pFD_vs_W_pFDpCD;
    string hTheta_q_pFD_vs_W_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_pCD vs. W (CD & FD)">
    TH2D *hTheta_q_pCD_vs_W_pFDpCD;
    string hTheta_q_pCD_vs_W_pFDpCD_Dir;
    //</editor-fold>

// Theta_q_p vs |P_p|/|q| (pFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p vs |P_p|/|q| (CD & FD)">

    //<editor-fold desc="Theta_q_p_L vs |P_p_L|/|q|">
    TH2D *hTheta_q_p_L_vs_p_L_q_pFDpCD;
    string hTheta_q_p_L_vs_p_L_q_pFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_q_p vs Theta_q_p (pFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_L vs Theta_q_p_R">
    TH2D *hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD;
    string hTheta_q_p_L_vs_Theta_q_p_R_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_pFD vs Theta_q_pCD">
    TH2D *hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD;
    string hTheta_q_pFD_vs_Theta_q_pCD_pFDpCD_Dir;
    //</editor-fold>

// Theta_pFD_pCD (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_pCD (CD & FD)">
    THStack *sTheta_pFD_pCD_pFDpCD;
    TH1D *hTheta_pFD_pCD_QEL_pFDpCD;
    TH1D *hTheta_pFD_pCD_MEC_pFDpCD;
    TH1D *hTheta_pFD_pCD_RES_pFDpCD;
    TH1D *hTheta_pFD_pCD_DIS_pFDpCD;
    string sTheta_pFD_pCD_pFDpCD_Dir;
    string hTheta_pFD_pCD_All_Int_pFDpCD_Dir;
    string hTheta_pFD_pCD_QEL_pFDpCD_Dir;
    string hTheta_pFD_pCD_MEC_pFDpCD_Dir;
    string hTheta_pFD_pCD_RES_pFDpCD_Dir;
    string hTheta_pFD_pCD_DIS_pFDpCD_Dir;
    //</editor-fold>

// Theta_pFD_pCD vs. W (pFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_pCD vs. W (CD & FD)">
    TH2D *hTheta_pFD_pCD_vs_W_pFDpCD;
    string hTheta_pFD_pCD_vs_W_pFDpCD_Dir;
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (CD & FD) ------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_vs_Theta_pCD for Theta_pFD_pCD < 20 (CD & FD)">
    TH2D *hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD;
    string hTheta_pFD_vs_theta_pCD_for_Theta_pFD_pCD_20_pFDpCD_Dir;
    //</editor-fold>

// dPhi_pFD_pCD for Theta_pFD_pCD < 20 (pFDpCD, CD & FD) ----------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_pFD_pCD for Theta_pFD_pCD < 20 (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD;
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_pFDpCD_Dir;

    TH1D *hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD;
    string hdPhi_pFD_pCD_for_Theta_pFD_pCD_20_ZOOMIN_pFDpCD_Dir;
    //</editor-fold>

// Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (pFDpCD, CD & FD) -----------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_vs_Theta_pCD for every Theta_pFD_pCD (CD & FD)">
    TH2D *hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD;
    string hTheta_pFD_vs_theta_pCD_forall_Theta_pFD_pCD_pFDpCD_Dir;
    //</editor-fold>

// dPhi_pFD_pCD for every Theta_pFD_pCD (pFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dPhi_pFD_pCD for every Theta_pFD_pCD (CD & FD)">
    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD;
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_pFDpCD_Dir;

    TH1D *hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD;
    string hdPhi_pFD_pCD_for_all_Theta_pFD_pCD_ZOOMIN_pFDpCD_Dir;

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_pFDpCD;
    string hdPhi_pFD_pCD_for_small_dTheta_pFDpCD_Dir;

    TH1D *hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD;
    string hdPhi_pFD_pCD_for_small_dTheta_ZOOMIN_pFDpCD_Dir;
    //</editor-fold>

// Ghost tracks handling (CD only) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pFD_pCD vs. TOFpFD-TOFpCD plots (pFDpCD)">
    hPlot2D hTheta_pFD_pCD_VS_ToFpFD_ToFpCD_AC_pFDpCD;
    //</editor-fold>

    //<editor-fold desc="Theta_pFD_pCD vs. positionpFD-positionpCD plots (pFDpCD)">
    hPlot2D hTheta_pFD_pCD_VS_PospFD_PospCD_AC_pFDpCD;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Other angle plots (nFDpCD)">

    //<editor-fold desc="Nucleons' angles plots (nFDpCD)">

// Theta_nFD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD histograms">
    THStack *sTheta_nFD_nFDpCD_FD;

    TH1D *hTheta_nFD_All_Int_nFDpCD_FD;
    TH1D *hTheta_nFD_QEL_nFDpCD_FD;
    TH1D *hTheta_nFD_MEC_nFDpCD_FD;
    TH1D *hTheta_nFD_RES_nFDpCD_FD;
    TH1D *hTheta_nFD_DIS_nFDpCD_FD;
    string hTheta_nFD_All_Int_nFDpCD_FD_Dir;
    string hTheta_nFD_QEL_nFDpCD_FD_Dir;
    string hTheta_nFD_MEC_nFDpCD_FD_Dir;
    string hTheta_nFD_RES_nFDpCD_FD_Dir;
    string hTheta_nFD_DIS_nFDpCD_FD_Dir;

    TH2D *hTheta_nFD_VS_P_nFD_nFDpCD_FD;
    TH2D *hTheta_nFD_VS_W_nFDpCD_FD;
    string hTheta_nFD_VS_P_nFD_nFDpCD_FD_Dir;
    string hTheta_nFD_VS_W_nFDpCD_FD_Dir;
    //</editor-fold>

// Phi_nFD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_nFD histograms">
    THStack *sPhi_nFD_nFDpCD_FD;

    TH1D *hPhi_nFD_All_Int_nFDpCD_FD;
    TH1D *hPhi_nFD_QEL_nFDpCD_FD;
    TH1D *hPhi_nFD_MEC_nFDpCD_FD;
    TH1D *hPhi_nFD_RES_nFDpCD_FD;
    TH1D *hPhi_nFD_DIS_nFDpCD_FD;
    string hPhi_nFD_All_Int_nFDpCD_FD_Dir;
    string hPhi_nFD_QEL_nFDpCD_FD_Dir;
    string hPhi_nFD_MEC_nFDpCD_FD_Dir;
    string hPhi_nFD_RES_nFDpCD_FD_Dir;
    string hPhi_nFD_DIS_nFDpCD_FD_Dir;

    TH2D *hPhi_nFD_VS_P_nFD_nFDpCD_FD;
    TH2D *hPhi_nFD_VS_W_nFDpCD_FD;
    string hPhi_nFD_VS_P_nFD_nFDpCD_FD_Dir;
    string hPhi_nFD_VS_W_nFDpCD_FD_Dir;
    //</editor-fold>

// Theta_nFD vs. Phi_nFD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD vs. Phi_nFD histograms">
    TH2D *hTheta_nFD_VS_Phi_nFD_nFDpCD_FD;
    string hTheta_nFD_VS_Phi_nFD_nFDpCD_FD_Dir;
    //</editor-fold>

// Theta_pCD ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD histograms">
    THStack *sTheta_pCD_nFDpCD_CD;

    TH1D *hTheta_pCD_All_Int_nFDpCD_CD;
    TH1D *hTheta_pCD_QEL_nFDpCD_CD;
    TH1D *hTheta_pCD_MEC_nFDpCD_CD;
    TH1D *hTheta_pCD_RES_nFDpCD_CD;
    TH1D *hTheta_pCD_DIS_nFDpCD_CD;
    string hTheta_pCD_All_Int_nFDpCD_CD_Dir;
    string hTheta_pCD_QEL_nFDpCD_CD_Dir;
    string hTheta_pCD_MEC_nFDpCD_CD_Dir;
    string hTheta_pCD_RES_nFDpCD_CD_Dir;
    string hTheta_pCD_DIS_nFDpCD_CD_Dir;

    TH2D *hTheta_pCD_VS_P_pCD_nFDpCD_CD;
    TH2D *hTheta_pCD_VS_W_nFDpCD_CD;
    string hTheta_pCD_VS_P_pCD_nFDpCD_CD_Dir;
    string hTheta_pCD_VS_W_nFDpCD_CD_Dir;
    //</editor-fold>

// Phi_pCD --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_pCD histograms">
    THStack *sPhi_pCD_nFDpCD_CD;

    TH1D *hPhi_pCD_All_Int_nFDpCD_CD;
    TH1D *hPhi_pCD_QEL_nFDpCD_CD;
    TH1D *hPhi_pCD_MEC_nFDpCD_CD;
    TH1D *hPhi_pCD_RES_nFDpCD_CD;
    TH1D *hPhi_pCD_DIS_nFDpCD_CD;
    string hPhi_pCD_All_Int_nFDpCD_CD_Dir;
    string hPhi_pCD_QEL_nFDpCD_CD_Dir;
    string hPhi_pCD_MEC_nFDpCD_CD_Dir;
    string hPhi_pCD_RES_nFDpCD_CD_Dir;
    string hPhi_pCD_DIS_nFDpCD_CD_Dir;

    TH2D *hPhi_pCD_VS_P_pCD_nFDpCD_CD;
    TH2D *hPhi_pCD_VS_W_nFDpCD_CD;
    string hPhi_pCD_VS_P_pCD_nFDpCD_CD_Dir;
    string hPhi_pCD_VS_W_nFDpCD_CD_Dir;
    //</editor-fold>

// Theta_pCD vs. Phi_pCD ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_pCD vs. Phi_pCD histograms">
    TH2D *hTheta_pCD_VS_Phi_pCD_nFDpCD_CD;
    string hTheta_pCD_VS_Phi_pCD_nFDpCD_CD_Dir;
    //</editor-fold>

// Theta_tot ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot histograms">
    THStack *sTheta_tot_nFDpCD;

    TH1D *hTheta_tot_All_Int_nFDpCD;
    TH1D *hTheta_tot_QEL_nFDpCD;
    TH1D *hTheta_tot_MEC_nFDpCD;
    TH1D *hTheta_tot_RES_nFDpCD;
    TH1D *hTheta_tot_DIS_nFDpCD;
    string hTheta_tot_All_Int_nFDpCD_Dir;
    string hTheta_tot_QEL_nFDpCD_Dir;
    string hTheta_tot_MEC_nFDpCD_Dir;
    string hTheta_tot_RES_nFDpCD_Dir;
    string hTheta_tot_DIS_nFDpCD_Dir;

    TH2D *hTheta_tot_VS_P_tot_nFDpCD;
    TH2D *hTheta_tot_VS_W_nFDpCD;
    string hTheta_tot_VS_P_tot_nFDpCD_Dir;
    string hTheta_tot_VS_W_nFDpCD_Dir;
    //</editor-fold>

// Phi_tot --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_tot histograms">
    THStack *sPhi_tot_nFDpCD;

    TH1D *hPhi_tot_All_Int_nFDpCD;
    TH1D *hPhi_tot_QEL_nFDpCD;
    TH1D *hPhi_tot_MEC_nFDpCD;
    TH1D *hPhi_tot_RES_nFDpCD;
    TH1D *hPhi_tot_DIS_nFDpCD;
    string hPhi_tot_All_Int_nFDpCD_Dir;
    string hPhi_tot_QEL_nFDpCD_Dir;
    string hPhi_tot_MEC_nFDpCD_Dir;
    string hPhi_tot_RES_nFDpCD_Dir;
    string hPhi_tot_DIS_nFDpCD_Dir;

    TH2D *hPhi_tot_VS_P_tot_nFDpCD;
    TH2D *hPhi_tot_VS_W_nFDpCD;
    string hPhi_tot_VS_P_tot_nFDpCD_Dir;
    string hPhi_tot_VS_W_nFDpCD_Dir;
    //</editor-fold>

// Theta_tot vs. Phi_tot ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_tot vs. Phi_tot histograms">
    TH2D *hTheta_tot_VS_Phi_tot_nFDpCD;
    string hTheta_tot_VS_Phi_tot_nFDpCD_Dir;
    //</editor-fold>

// Theta_rel ------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel histograms">
    THStack *sTheta_rel_nFDpCD;

    TH1D *hTheta_rel_All_Int_nFDpCD;
    TH1D *hTheta_rel_MEC_nFDpCD;
    TH1D *hTheta_rel_RES_nFDpCD;
    TH1D *hTheta_rel_DIS_nFDpCD;
    string hTheta_rel_All_Int_nFDpCD_Dir;
    string hTheta_rel_QEL_nFDpCD_Dir;
    string hTheta_rel_MEC_nFDpCD_Dir;
    string hTheta_rel_RES_nFDpCD_Dir;
    string hTheta_rel_DIS_nFDpCD_Dir;

    TH2D *hTheta_rel_VS_P_rel_nFDpCD;
    TH2D *hTheta_rel_VS_W_nFDpCD;
    string hTheta_rel_VS_P_rel_nFDpCD_Dir;
    string hTheta_rel_VS_W_nFDpCD_Dir;
    //</editor-fold>

// Phi_rel --------------------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Phi_rel histograms">
    THStack *sPhi_rel_nFDpCD;

    TH1D *hPhi_rel_All_Int_nFDpCD;
    TH1D *hPhi_rel_QEL_nFDpCD;
    TH1D *hPhi_rel_MEC_nFDpCD;
    TH1D *hPhi_rel_RES_nFDpCD;
    TH1D *hPhi_rel_DIS_nFDpCD;
    string hPhi_rel_All_Int_nFDpCD_Dir;
    string hPhi_rel_QEL_nFDpCD_Dir;
    string hPhi_rel_MEC_nFDpCD_Dir;
    string hPhi_rel_RES_nFDpCD_Dir;
    string hPhi_rel_DIS_nFDpCD_Dir;

    TH2D *hPhi_rel_VS_P_rel_nFDpCD;
    TH2D *hPhi_rel_VS_W_nFDpCD;
    string hPhi_rel_VS_P_rel_nFDpCD_Dir;
    string hPhi_rel_VS_W_nFDpCD_Dir;
    //</editor-fold>

// Theta_rel vs. Phi_rel ------------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_rel vs. Phi_rel histograms">
    TH2D *hTheta_rel_VS_Phi_rel_nFDpCD;
    string hTheta_rel_VS_Phi_rel_nFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_p_e_p_tot (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot (nFDpCD, CD & FD)">
    THStack *sTheta_p_e_p_tot_nFDpCD;
    TH1D *hTheta_p_e_p_tot_nFDpCD;
    string hTheta_p_e_p_tot_nFDpCD_Dir;
    //</editor-fold>

// Theta_p_e_p_tot vs. W (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_p_e_p_tot vs. W (nFDpCD)">
    TH2D *hTheta_p_e_p_tot_vs_W_nFDpCD;
    string hTheta_p_e_p_tot_vs_W_nFDpCD_Dir;
    //</editor-fold>

// Theta_q_p (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p (nFDpCD, CD & FD)">

    //<editor-fold desc="Theta_q_p_tot (CD & FD)">
    THStack *sTheta_q_p_tot_nFDpCD;
    TH1D *hTheta_q_p_tot_nFDpCD;
    string hTheta_q_p_tot_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_P_nL_minus_q_nR and Theta_q_p_R (nFDpCD, CD & FD)">
    THStack *sTheta_P_nL_minus_q_nR_nFDpCD;
    TH1D *hTheta_P_nL_minus_q_nR_nFDpCD;
    string hTheta_P_nL_minus_q_nR_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L and Theta_q_p_R (nFDpCD, CD & FD)">
    THStack *sTheta_q_p_L_R_nFDpCD;
    TH1D *hTheta_q_p_L_nFDpCD;
    TH1D *hTheta_q_p_R_nFDpCD;
    string hTheta_q_p_L_nFDpCD_Dir;
    string hTheta_q_p_R_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_nFD and Theta_q_pCD (nFDpCD, CD & FD)">
    THStack *sTheta_q_p_nFDpCD;
    TH1D *hTheta_q_nFD_nFDpCD;
    TH1D *hTheta_q_pCD_nFDpCD;
    string hTheta_q_nFD_nFDpCD_Dir;
    string hTheta_q_pCD_nFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// Theta_q_p_tot vs. W (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_pCD vs. W (CD & FD)">
    TH2D *hTheta_q_p_tot_vs_W_nFDpCD;
    string hTheta_q_p_tot_vs_W_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_P_nL_minus_q_nR vs. W (CD & FD)">
    TH2D *hTheta_P_nL_minus_q_nR_vs_W_nFDpCD;
    string hTheta_P_nL_minus_q_nR_vs_W_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_L vs. W (CD & FD)">
    TH2D *hTheta_q_p_L_vs_W_nFDpCD;
    string hTheta_q_p_L_vs_W_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_p_R vs. W (CD & FD)">
    TH2D *hTheta_q_p_R_vs_W_nFDpCD;
    string hTheta_q_p_R_vs_W_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_nFD vs. W (CD & FD)">
    TH2D *hTheta_q_nFD_vs_W_nFDpCD;
    string hTheta_q_nFD_vs_W_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_pCD vs. W (CD & FD)">
    TH2D *hTheta_q_pCD_vs_W_nFDpCD;
    string hTheta_q_pCD_vs_W_nFDpCD_Dir;
    //</editor-fold>

// Theta_q_p_L vs |P_L|/|q| (nFDpCD, CD & FD) ---------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p vs |P_p|/|q| (CD & FD)">
    TH2D *hTheta_q_p_L_vs_p_L_q_nFDpCD;
    string hTheta_q_p_L_vs_p_L_q_nFDpCD_Dir;
    //</editor-fold>

// Theta_q_p vs Theta_q_p (nFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_q_p_L vs Theta_q_p_R">
    TH2D *hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD;
    string hTheta_q_p_L_vs_Theta_q_p_R_nFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Theta_q_nFD vs Theta_q_pCD">
    TH2D *hTheta_q_nFD_vs_Theta_q_pCD_nFDpCD;
    string hTheta_q_nFD_vs_Theta_q_pCD_nFDpCD_Dir;
    //</editor-fold>

// Theta_nFD_pCD (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_pCD (CD & FD)">
    THStack *sTheta_nFD_pCD_nFDpCD;
    TH1D *hTheta_nFD_pCD_All_Int_nFDpCD;
    TH1D *hTheta_nFD_pCD_QEL_nFDpCD;
    TH1D *hTheta_nFD_pCD_MEC_nFDpCD;
    TH1D *hTheta_nFD_pCD_RES_nFDpCD;
    TH1D *hTheta_nFD_pCD_DIS_nFDpCD;
    string sTheta_nFD_pCD_nFDpCD_Dir;
    string hTheta_nFD_pCD_All_Int_nFDpCD_Dir;
    string hTheta_nFD_pCD_QEL_nFDpCD_Dir;
    string hTheta_nFD_pCD_MEC_nFDpCD_Dir;
    string hTheta_nFD_pCD_RES_nFDpCD_Dir;
    string hTheta_nFD_pCD_DIS_nFDpCD_Dir;
    //</editor-fold>

// Theta_nFD_pCD vs. W (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_pCD vs. W (CD & FD)">
    TH2D *hTheta_nFD_pCD_vs_W_nFDpCD;
    string hTheta_nFD_pCD_vs_W_nFDpCD_Dir;
    //</editor-fold>

// Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) ------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (CD & FD)">
    TH2D *hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD;
    string hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_20_nFDpCD_Dir;
    //</editor-fold>

// dphi_nFD_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="dphi_nFD_pCD for Theta_nFD_pCD < 20 (CD & FD)">
    TH1D *hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD;
    string hdphi_nFD_pCD_for_Theta_nFD_pCD_20_nFDpCD_Dir;

    TH1D *hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD;
    string hdphi_nFD_pCD_for_Theta_nFD_pCD_20_ZOOMIN_nFDpCD_Dir;
    //</editor-fold>

// Theta_nFD_vs_theta_pCD for every Theta_nFD_pCD (nFDpCD, CD & FD) -------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for Theta_nFD_pCD (CD & FD)">
    TH2D *hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD;
    string hTheta_nFD_vs_theta_pCD_forall_Theta_nFD_pCD_nFDpCD_Dir;
    //</editor-fold>

// dphi_nFD_pCD for every Theta_nFD_pCD (nFDpCD, CD & FD) -----------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for every Theta_nFD_pCD (CD & FD)">
    TH1D *hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD;
    string hdphi_nFD_pCD_for_all_Theta_nFD_pCD_nFDpCD_Dir;

    TH1D *hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD;
    string hdphi_nFD_pCD_for_all_Theta_nFD_pCD_ZOOMIN_nFDpCD_Dir;

    TH1D *hdphi_nFD_pCD_for_small_dTheta_nFDpCD;
    string hdphi_nFD_pCD_for_small_dTheta_nFDpCD_Dir;

    TH1D *hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD;
    string hdphi_nFD_pCD_for_small_dTheta_ZOOMIN_nFDpCD_Dir;
    //</editor-fold>

// Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (nFDpCD, CD & FD) --------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Theta_nFD_vs_theta_pCD for Theta_nFD_pCD < 20 (CD & FD)">
    TH2D *hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_10_nFDpCD;
    string hTheta_nFD_vs_theta_pCD_for_Theta_nFD_pCD_10_nFDpCD_Dir;
    //</editor-fold>

// Neutron veto plots (nFDpCD) ----------------------------------------------------------------------------------------------------------------------------------------------

    //<editor-fold desc="Neutron veto plots (nFDpCD)">
    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_BV_nFDpCD;

    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_AV_nFDpCD;

    hPlot2D hdTheta_nFD_e_VS_dPhi_nFD_e_Electrons_Vetoed_Neutrons_nFDpCD;
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Q2 histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Q2 histograms">

    //<editor-fold desc="Q2 histograms (no #(e) cut)">
    THStack *sQ2_All_e;
    TH1D *hQ2_All_e;
    string hQ2_All_e_Dir;
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1e cut)">
    THStack *sQ2_1e_cut;
    TH1D *hQ2_1e_cut;
    string hQ2_1e_cut_Dir;
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1p)">
    THStack *sQ2_1p;
    TH1D *hQ2_1p;
    string hQ2_1p_Dir;
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (1n)">
    THStack *sQ2_1n;
    TH1D *hQ2_1n;
    string hQ2_1n_Dir;
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (2p)">
    THStack *sQ2_2p;
    TH1D *hQ2_2p;
    string hQ2_2p_Dir;
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (pFDpCD)">
    THStack *sQ2_pFDpCD;
    TH1D *hQ2_pFDpCD;
    string hQ2_pFDpCD_Dir;

    TH2D *hQ2_VS_W_pFDpCD;
    string hQ2_VS_W_pFDpCD_Dir;
    //</editor-fold>

    //<editor-fold desc="Q2 histograms (nFDpCD)">
    THStack *sQ2_nFDpCD;
    TH1D *hQ2_nFDpCD;
    string hQ2_nFDpCD_Dir;

    TH2D *hQ2_VS_W_nFDpCD;
    string hQ2_VS_W_nFDpCD_Dir;
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Energy (E_e) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy (E_e) histograms">

    //<editor-fold desc="E_e plots (1e cut)">
    /* Energy (E_e) histograms (1e cut, CD & FD) */
    THStack *sE_e_1e_cut;
    string sE_e_1e_cut_Dir;

    TH1D *hE_e_All_Int_1e_cut_FD;
    TH1D *hE_e_QEL_1e_cut_FD;
    TH1D *hE_e_MEC_1e_cut_FD;
    TH1D *hE_e_RES_1e_cut_FD;
    TH1D *hE_e_DIS_1e_cut_FD;
    string hE_e_All_Int_1e_cut_FD_Dir;
    string hE_e_QEL_1e_cut_FD_Dir;
    string hE_e_MEC_1e_cut_FD_Dir;
    string hE_e_RES_1e_cut_FD_Dir;
    string hE_e_DIS_1e_cut_FD_Dir;

    TH2D *hE_e_VS_Theta_e_All_Int_1e_cut_FD;
    TH2D *hE_e_VS_Theta_e_QEL_1e_cut_FD;
    TH2D *hE_e_VS_Theta_e_MEC_1e_cut_FD;
    TH2D *hE_e_VS_Theta_e_RES_1e_cut_FD;
    TH2D *hE_e_VS_Theta_e_DIS_1e_cut_FD;
    string hE_e_VS_Theta_e_All_Int_1e_cut_FD_Dir;
    string hE_e_VS_Theta_e_QEL_1e_cut_FD_Dir;
    string hE_e_VS_Theta_e_MEC_1e_cut_FD_Dir;
    string hE_e_VS_Theta_e_RES_1e_cut_FD_Dir;
    string hE_e_VS_Theta_e_DIS_1e_cut_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="E_e plots (1p)">
    /* Energy (E_e) histograms (1p, CD & FD) */
    THStack *sE_e_1p_FD;
    string sE_e_1p_FD_Dir;

    TH1D *hE_e_All_Int_1p_FD;
    TH1D *hE_e_QEL_1p_FD;
    TH1D *hE_e_MEC_1p_FD;
    TH1D *hE_e_RES_1p_FD;
    TH1D *hE_e_DIS_1p_FD;
    string hE_e_All_Int_1p_FD_Dir;
    string hE_e_QEL_1p_FD_Dir;
    string hE_e_MEC_1p_FD_Dir;
    string hE_e_RES_1p_FD_Dir;
    string hE_e_DIS_1p_FD_Dir;

    /* E_e vs. Theta_e (1p, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_1p_FD;
    TH2D *hE_e_VS_Theta_e_QEL_1p_FD;
    TH2D *hE_e_VS_Theta_e_MEC_1p_FD;
    TH2D *hE_e_VS_Theta_e_RES_1p_FD;
    TH2D *hE_e_VS_Theta_e_DIS_1p_FD;
    string hE_e_VS_Theta_e_All_Int_1p_FD_Dir;
    string hE_e_VS_Theta_e_QEL_1p_FD_Dir;
    string hE_e_VS_Theta_e_MEC_1p_FD_Dir;
    string hE_e_VS_Theta_e_RES_1p_FD_Dir;
    string hE_e_VS_Theta_e_DIS_1p_FD_Dir;

    /* Energy (E_e) histograms around theta_e = 15 (1p, CD & FD) */
    THStack *sE_e_15_1p_FD;
    string sE_e_15_1p_CD_Dir;
    string sE_e_15_1p_FD_Dir;

    TH1D *hE_e_15_All_Int_1p_FD;
    TH1D *hE_e_15_QEL_1p_FD;
    TH1D *hE_e_15_MEC_1p_FD;
    TH1D *hE_e_15_RES_1p_FD;
    TH1D *hE_e_15_DIS_1p_FD;
    string hE_e_15_All_Int_1p_FD_Dir;
    string hE_e_15_QEL_1p_FD_Dir;
    string hE_e_15_MEC_1p_FD_Dir;
    string hE_e_15_RES_1p_FD_Dir;
    string hE_e_15_DIS_1p_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="E_e plots (1n)">
    /* Energy (E_e) histograms (1n, CD & FD) */
    THStack *sE_e_1n_FD;
    string sE_e_1n_FD_Dir;

    TH1D *hE_e_All_Int_1n_FD;
    TH1D *hE_e_QEL_1n_FD;
    TH1D *hE_e_MEC_1n_FD;
    TH1D *hE_e_RES_1n_FD;
    TH1D *hE_e_DIS_1n_FD;
    string hE_e_All_Int_1n_FD_Dir;
    string hE_e_QEL_1n_FD_Dir;
    string hE_e_MEC_1n_FD_Dir;
    string hE_e_RES_1n_FD_Dir;
    string hE_e_DIS_1n_FD_Dir;

    /* E_e vs. Theta_e (1n, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_1n_FD;
    TH2D *hE_e_VS_Theta_e_QEL_1n_FD;
    TH2D *hE_e_VS_Theta_e_MEC_1n_FD;
    TH2D *hE_e_VS_Theta_e_RES_1n_FD;
    TH2D *hE_e_VS_Theta_e_DIS_1n_FD;
    string hE_e_VS_Theta_e_All_Int_1n_FD_Dir;
    string hE_e_VS_Theta_e_QEL_1n_FD_Dir;
    string hE_e_VS_Theta_e_MEC_1n_FD_Dir;
    string hE_e_VS_Theta_e_RES_1n_FD_Dir;
    string hE_e_VS_Theta_e_DIS_1n_FD_Dir;

    /* Energy (E_e) histograms around theta_e = 15 (1n, CD & FD) */
    THStack *sE_e_15_1n_FD;
    string sE_e_15_1n_CD_Dir;
    string sE_e_15_1n_FD_Dir;

    TH1D *hE_e_15_All_Int_1n_FD;
    TH1D *hE_e_15_QEL_1n_FD;
    TH1D *hE_e_15_MEC_1n_FD;
    TH1D *hE_e_15_RES_1n_FD;
    TH1D *hE_e_15_DIS_1n_FD;
    string hE_e_15_All_Int_1n_FD_Dir;
    string hE_e_15_QEL_1n_FD_Dir;
    string hE_e_15_MEC_1n_FD_Dir;
    string hE_e_15_RES_1n_FD_Dir;
    string hE_e_15_DIS_1n_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="E_e plots (2p)">
    /* Energy (E_e) histograms (2p, CD & FD) */
    THStack *sE_e_2p_FD;
    string sE_e_2p_FD_Dir;

    TH1D *hE_e_All_Int_2p_FD;
    TH1D *hE_e_QEL_2p_FD;
    TH1D *hE_e_MEC_2p_FD;
    TH1D *hE_e_RES_2p_FD;
    TH1D *hE_e_DIS_2p_FD;
    string hE_e_All_Int_2p_FD_Dir;
    string hE_e_QEL_2p_FD_Dir;
    string hE_e_MEC_2p_FD_Dir;
    string hE_e_RES_2p_FD_Dir;
    string hE_e_DIS_2p_FD_Dir;

    /* E_e vs. Theta_e (2p, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_2p_FD;
    TH2D *hE_e_VS_Theta_e_QEL_2p_FD;
    TH2D *hE_e_VS_Theta_e_MEC_2p_FD;
    TH2D *hE_e_VS_Theta_e_RES_2p_FD;
    TH2D *hE_e_VS_Theta_e_DIS_2p_FD;
    string hE_e_VS_Theta_e_All_Int_2p_FD_Dir;
    string hE_e_VS_Theta_e_QEL_2p_FD_Dir;
    string hE_e_VS_Theta_e_MEC_2p_FD_Dir;
    string hE_e_VS_Theta_e_RES_2p_FD_Dir;
    string hE_e_VS_Theta_e_DIS_2p_FD_Dir;

    /* Energy (E_e) histograms around theta_e = 15 (2p, CD & FD) */
    THStack *sE_e_15_2p_FD;
    string sE_e_15_2p_CD_Dir;
    string sE_e_15_2p_FD_Dir;

    TH1D *hE_e_15_All_Int_2p_FD;
    TH1D *hE_e_15_QEL_2p_FD;
    TH1D *hE_e_15_MEC_2p_FD;
    TH1D *hE_e_15_RES_2p_FD;
    TH1D *hE_e_15_DIS_2p_FD;
    string hE_e_15_All_Int_2p_FD_Dir;
    string hE_e_15_QEL_2p_FD_Dir;
    string hE_e_15_MEC_2p_FD_Dir;
    string hE_e_15_RES_2p_FD_Dir;
    string hE_e_15_DIS_2p_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="E_e plots (pFDpCD)">
    /* Energy (E_e) histograms (pFDpCD, CD & FD) */
    THStack *sE_e_pFDpCD_FD;
    string sE_e_pFDpCD_CD_Dir;
    string sE_e_pFDpCD_FD_Dir;

    TH1D *hE_e_All_Int_pFDpCD_FD;
    TH1D *hE_e_QEL_pFDpCD_FD;
    TH1D *hE_e_MEC_pFDpCD_FD;
    TH1D *hE_e_RES_pFDpCD_FD;
    TH1D *hE_e_DIS_pFDpCD_FD;
    string hE_e_All_Int_pFDpCD_FD_Dir;
    string hE_e_QEL_pFDpCD_FD_Dir;
    string hE_e_MEC_pFDpCD_FD_Dir;
    string hE_e_RES_pFDpCD_FD_Dir;
    string hE_e_DIS_pFDpCD_FD_Dir;

    /* E_e vs. Theta_e (pFDpCD, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_pFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_QEL_pFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_MEC_pFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_RES_pFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_DIS_pFDpCD_FD;
    string hE_e_VS_Theta_e_All_Int_pFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_QEL_pFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_MEC_pFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_RES_pFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_DIS_pFDpCD_FD_Dir;

    /* Energy (E_e) histograms around theta_e = 15 (pFDpCD, CD & FD) */
    THStack *sE_e_15_pFDpCD_FD;
    string sE_e_15_pFDpCD_CD_Dir;
    string sE_e_15_pFDpCD_FD_Dir;

    TH1D *hE_e_15_All_Int_pFDpCD_FD;
    TH1D *hE_e_15_QEL_pFDpCD_FD;
    TH1D *hE_e_15_MEC_pFDpCD_FD;
    TH1D *hE_e_15_RES_pFDpCD_FD;
    TH1D *hE_e_15_DIS_pFDpCD_FD;
    string hE_e_15_All_Int_pFDpCD_FD_Dir;
    string hE_e_15_QEL_pFDpCD_FD_Dir;
    string hE_e_15_MEC_pFDpCD_FD_Dir;
    string hE_e_15_RES_pFDpCD_FD_Dir;
    string hE_e_15_DIS_pFDpCD_FD_Dir;
    //</editor-fold>

    //<editor-fold desc="E_e plots (nFDpCD)">
    /* Energy (E_e) histograms (nFDpCD, CD & FD) */
    THStack *sE_e_nFDpCD_FD;
    string sE_e_nFDpCD_FD_Dir;

    TH1D *hE_e_All_Int_nFDpCD_FD;
    TH1D *hE_e_QEL_nFDpCD_FD;
    TH1D *hE_e_MEC_nFDpCD_FD;
    TH1D *hE_e_RES_nFDpCD_FD;
    TH1D *hE_e_DIS_nFDpCD_FD;
    string hE_e_All_Int_nFDpCD_FD_Dir;
    string hE_e_QEL_nFDpCD_FD_Dir;
    string hE_e_MEC_nFDpCD_FD_Dir;
    string hE_e_RES_nFDpCD_FD_Dir;
    string hE_e_DIS_nFDpCD_FD_Dir;

    /* E_e vs. Theta_e (nFDpCD, CD & FD) */
    TH2D *hE_e_VS_Theta_e_All_Int_nFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_QEL_nFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_MEC_nFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_RES_nFDpCD_FD;
    TH2D *hE_e_VS_Theta_e_DIS_nFDpCD_FD;
    string hE_e_VS_Theta_e_All_Int_nFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_QEL_nFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_MEC_nFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_RES_nFDpCD_FD_Dir;
    string hE_e_VS_Theta_e_DIS_nFDpCD_FD_Dir;

    /* Energy (E_e) histograms around theta_e = 15 (nFDpCD, CD & FD) */
    THStack *sE_e_15_nFDpCD_FD;
    string sE_e_15_nFDpCD_CD_Dir;
    string sE_e_15_nFDpCD_FD_Dir;

    TH1D *hE_e_15_All_Int_nFDpCD_FD;
    TH1D *hE_e_15_QEL_nFDpCD_FD;
    TH1D *hE_e_15_MEC_nFDpCD_FD;
    TH1D *hE_e_15_RES_nFDpCD_FD;
    TH1D *hE_e_15_DIS_nFDpCD_FD;
    string hE_e_15_All_Int_nFDpCD_FD_Dir;
    string hE_e_15_QEL_nFDpCD_FD_Dir;
    string hE_e_15_MEC_nFDpCD_FD_Dir;
    string hE_e_15_RES_nFDpCD_FD_Dir;
    string hE_e_15_DIS_nFDpCD_FD_Dir;
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Energy Transfer (ET) histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Energy Transfer (ET) histograms">
    string tET_All_Ang = "Energy transfer #omega for every #theta_{e}", tET15 = "Energy transfer #omega Around #theta_{e} = 15#circ";

    //<editor-fold desc="ET for every theta_e (1p)">
    THStack *sET_All_Ang_All_Int_1p_FD = new THStack("#omega for all #theta_{e} (1p)",
                                                     "Energy transfer #omega for all #theta_{e} (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1p_Directory"];

    TH1D *hET_All_Ang_All_Int_1p_FD = new TH1D("#omega for all #theta_{e} (All Int., 1p)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1p_Directory"];
    string hET_All_Ang_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1p_Directory"];
    string hET_All_Ang_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1p_Directory"];
    string hET_All_Ang_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1p_Directory"];
    string hET_All_Ang_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (1n)">
    THStack *sET_All_Ang_All_Int_1n_FD = new THStack("#omega for all #theta_{e} (1n)",
                                                     "Energy transfer #omega for all #theta_{e} (1n);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_1n_Directory"];

    TH1D *hET_All_Ang_All_Int_1n_FD = new TH1D("#omega for all #theta_{e} (All Int., 1n)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_1n_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_1n_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_1n_FD = new TH1D("#omega for all #theta_{e} (RES Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_1n_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 1n, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_1n_Directory"];
    string hET_All_Ang_QEL_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_1n_Directory"];
    string hET_All_Ang_MEC_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_1n_Directory"];
    string hET_All_Ang_RES_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_1n_Directory"];
    string hET_All_Ang_DIS_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (2p)">
    THStack *sET_All_Ang_All_Int_2p_FD = new THStack("#omega for all #theta_{e} (2p)",
                                                     "Energy transfer #omega for all #theta_{e} (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_2p_Directory"];

    TH1D *hET_All_Ang_All_Int_2p_FD = new TH1D("#omega for all #theta_{e} (All Int., 2p)",
                                               "Energy transfer #omega for all #theta_{e} (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_2p_FD = new TH1D("#omega for all #theta_{e} (QEL Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_2p_FD = new TH1D("#omega for all #theta_{e} (MEC Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_2p_FD = new TH1D("#omega for all #theta_{e} (RES Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_2p_FD = new TH1D("#omega for all #theta_{e} (DIS Only, 2p, FD)",
                                           "Energy transfer #omega for all #theta_{e} (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                           numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_2p_Directory"];
    string hET_All_Ang_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_2p_Directory"];
    string hET_All_Ang_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_2p_Directory"];
    string hET_All_Ang_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_2p_Directory"];
    string hET_All_Ang_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (pFDpCD)">
    THStack *sET_All_Ang_All_Int_pFDpCD_FD = new THStack("#omega for all #theta_{e} (pFDpCD)",
                                                         "Energy transfer #omega for all #theta_{e} (pFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_pFDpCD_Directory"];

    TH1D *hET_All_Ang_All_Int_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (All Int., pFDpCD)",
                                                   "Energy transfer #omega for all #theta_{e} (All Int., pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                                   numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (QEL Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (QEL Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (MEC Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (MEC Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (RES Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (RES Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_pFDpCD_FD = new TH1D("#omega for all #theta_{e} (DIS Only, pFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (DIS Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_QEL_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_MEC_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_RES_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_pFDpCD_Directory"];
    string hET_All_Ang_DIS_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET for every theta_e (nFDpCD)">
    THStack *sET_All_Ang_All_Int_nFDpCD_FD = new THStack("#omega for all #theta_{e} (nFDpCD)",
                                                         "Energy transfer #omega for all #theta_{e} (nFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET_All_Ang_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Ang_stack_nFDpCD_Directory"];

    TH1D *hET_All_Ang_All_Int_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (All Int., nFDpCD)",
                                                   "Energy transfer #omega for all #theta_{e} (All Int., nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                                   numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_QEL_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (QEL Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (QEL Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_MEC_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (MEC Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (MEC Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_RES_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (RES Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (RES Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET_All_Ang_DIS_nFDpCD_FD = new TH1D("#omega for all #theta_{e} (DIS Only, nFDpCD, FD)",
                                               "Energy transfer #omega for all #theta_{e} (DIS Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                               numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET_All_Ang_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_QEL_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_MEC_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_RES_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_All_Ang_nFDpCD_Directory"];
    string hET_All_Ang_DIS_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_All_Ang_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (1p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_1p_FD = new THStack("ET around #theta_{e} = 15#circ (1p)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (1p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_1p_Directory"];

    TH1D *hET15_All_Int_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (All Int., 1p)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1p);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_1p_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_1p_Directory"];
    string hET15_QEL_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_1p_Directory"];
    string hET15_MEC_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_1p_Directory"];
    string hET15_RES_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_1p_Directory"];
    string hET15_DIS_1p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (1n)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_1n_FD = new THStack("ET around #theta_{e} = 15#circ (1n)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (1n);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_1n_Directory"];

    TH1D *hET15_All_Int_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (All Int., 1n)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 1n);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (QEL Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (MEC Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (RES Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_1n_FD = new TH1D("ET around #theta_{e} = 15#circ (DIS Only, 1n, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 1n);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_1n_Directory"];
    string hET15_QEL_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_1n_Directory"];
    string hET15_MEC_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_1n_Directory"];
    string hET15_RES_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_1n_Directory"];
    string hET15_DIS_1n_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (2p)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_2p_FD = new THStack("#omega around #theta_{e} = 15#circ (2p)",
                                               "Energy transfer #omega Around #theta_{e} = 15#circ (2p);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_2p_Directory"];

    TH1D *hET15_All_Int_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., 2p)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., 2p);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_2p_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, 2p, FD)",
                                     "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, 2p);#omega = E_{beam}-E_{e} [GeV]",
                                     numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_2p_Directory"];
    string hET15_QEL_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_2p_Directory"];
    string hET15_MEC_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_2p_Directory"];
    string hET15_RES_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_2p_Directory"];
    string hET15_DIS_2p_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (pFDpCD)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_pFDpCD_FD = new THStack("#omega around #theta_{e} = 15#circ (pFDpCD)",
                                                   "Energy transfer #omega Around #theta_{e} = 15#circ (pFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_pFDpCD_Directory"];

    TH1D *hET15_All_Int_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., pFDpCD)",
                                             "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                             numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_pFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, pFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, pFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_pFDpCD_Directory"];
    string hET15_QEL_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_pFDpCD_Directory"];
    string hET15_MEC_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_pFDpCD_Directory"];
    string hET15_RES_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_pFDpCD_Directory"];
    string hET15_DIS_pFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="ET around 15 Deg (nFDpCD)">
    /* ET around 15 Deg */
    THStack *sET15_All_Int_nFDpCD_FD = new THStack("#omega around #theta_{e} = 15#circ (nFDpCD)",
                                                   "Energy transfer #omega Around #theta_{e} = 15#circ (nFDpCD);#omega = E_{beam}-E_{e} [GeV]");
    string sET15_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_15_stack_nFDpCD_Directory"];

    TH1D *hET15_All_Int_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (All Int., nFDpCD)",
                                             "Energy transfer #omega Around #theta_{e} = 15#circ (All Int., nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                             numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_QEL_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (QEL Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (QEL Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_MEC_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (MEC Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (MEC Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_RES_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (RES Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (RES Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    TH1D *hET15_DIS_nFDpCD_FD = new TH1D("#omega around #theta_{e} = 15#circ (DIS Only, nFDpCD, FD)",
                                         "Energy transfer #omega Around #theta_{e} = 15#circ (DIS Only, nFDpCD);#omega = E_{beam}-E_{e} [GeV]",
                                         numTH1Dbins_ET_Plots, 0, beamE * 1.1);
    string hET15_All_Int_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_All_Int_15_nFDpCD_Directory"];
    string hET15_QEL_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_QEL_15_nFDpCD_Directory"];
    string hET15_MEC_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_MEC_15_nFDpCD_Directory"];
    string hET15_RES_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_RES_15_nFDpCD_Directory"];
    string hET15_DIS_nFDpCD_FD_Dir = directories.ETrans_Directory_map["ETrans_DIS_15_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Ecal reconstruction histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Ecal reconstruction histograms">

    //<editor-fold desc="Ecal reconstruction histograms (1p)">

    //<editor-fold desc="Ecal plots (1p)">
    THStack *sEcal_1p = new THStack("E_{cal} Reconstruction (1p)", "E_{cal} Reconstruction (1p);E_{cal} = E_{e} + T_{p} [GeV]");
    string sEcal_1p_Dir = directories.Ecal_Directory_map["Ecal_stack_1p_Directory"];

    TH1D *hEcal_All_Int_1p = new TH1D("E_{cal} reco. (All Int., 1p)", "E_{cal} Reconstruction (All Int., 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_1p = new TH1D("E_{cal} reco. (QEL only, 1p)", "E_{cal} Reconstruction (QEL only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_1p = new TH1D("E_{cal} reco. (MEC only, 1p)", "E_{cal} Reconstruction (MEC only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_1p = new TH1D("E_{cal} reco. (RES only, 1p)", "E_{cal} Reconstruction (RES only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_1p = new TH1D("E_{cal} reco. (DIS only, 1p)", "E_{cal} Reconstruction (DIS only, 1p);E_{cal} = E_{e} + T_{p} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_1p_Dir = directories.Ecal_Directory_map["Ecal_All_Int_1p_Directory"];
    string hEcal_QEL_1p_Dir = directories.Ecal_Directory_map["Ecal_QEL_1p_Directory"];
    string hEcal_MEC_1p_Dir = directories.Ecal_Directory_map["Ecal_MEC_1p_Directory"];
    string hEcal_RES_1p_Dir = directories.Ecal_Directory_map["Ecal_RES_1p_Directory"];
    string hEcal_DIS_1p_Dir = directories.Ecal_Directory_map["Ecal_DIS_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (1p)">
    TH2D *hEcal_vs_P_e_1p = new TH2D("E_{cal} vs. P_{e} (All Int., 1p)", "E_{cal} vs. P_{e} (All Int., 1p);P_{e} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_1p = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1p)",
                                          "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1p);P_{e} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_e_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1p_Directory"];

    TH2D *hEcal_vs_P_p_1p = new TH2D("E_{cal} vs. P_{p} (All Int., 1p)", "E_{cal} vs. P_{p} (All Int., 1p);P_{p} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_p_test_1p = new TH2D("E_{cal} vs. P_{p} for E_{cal}>E_{beam} (All Int., 1p)",
                                          "E_{cal} vs. P_{p} for E_{cal}>E_{beam} (All Int., 1p);P_{p} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_p_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (1p)">
    TH2D *hEcal_vs_Theta_e_1p = new TH2D("E_{cal} vs. #theta_{e} (All Int., 1p)",
                                         "E_{cal} vs. #theta_{e} (All Int., 1p);#theta_{e} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_1p = new TH2D("E_{cal} vs. #phi_{e} (All Int., 1p)",
                                       "E_{cal} vs. #phi_{e} (All Int., 1p);#phi_{e} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_1p = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1p)",
                                              "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1p);#theta_{e} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_1p = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1p)",
                                            "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1p);#phi_{e} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];
    string hEcal_vs_Phi_e_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];

    TH2D *hEcal_vs_Theta_p_1p = new TH2D("E_{cal} vs. #theta_{p} (All Int., 1p)",
                                         "E_{cal} vs. #theta_{p} (All Int., 1p);#theta_{p} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_p_1p = new TH2D("E_{cal} vs. #phi_{p} (All Int., 1p)",
                                       "E_{cal} vs. #phi_{p} (All Int., 1p);#phi_{p} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_p_test_1p = new TH2D("E_{cal} vs. #theta_{p} for E_{cal}>E_{beam} (All Int., 1p)",
                                              "E_{cal} vs. #theta_{p} for E_{cal}>E_{beam} (All Int., 1p);#theta_{p} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_p_test_1p = new TH2D("E_{cal} vs. #phi_{p} for E_{cal}>E_{beam} (All Int., 1p)",
                                            "E_{cal} vs. #phi_{p} for E_{cal}>E_{beam} (All Int., 1p);#phi_{p} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_p_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];
    string hEcal_vs_Phi_p_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (1p)">
    TH2D *hEcal_vs_dAlpha_T_1p = new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1p)",
                                          "E_{cal} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [Deg];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (1p)">
    TH2D *hEcal_vs_dP_T_1p = new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1p)",
                                      "E_{cal} vs. #deltaP_{T} (All Int., 1p);#deltaP_{T} [GeV/c];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_1p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (1n)">

    //<editor-fold desc="Ecal plots (1n)">
    THStack *sEcal_1n = new THStack("E_{cal} Reconstruction (1n)", "E_{cal} Reconstruction (1n);E_{cal} = E_{e} + T_{n} [GeV]");
    string sEcal_1n_Dir = directories.Ecal_Directory_map["Ecal_stack_1n_Directory"];

    TH1D *hEcal_All_Int_1n = new TH1D("E_{cal} reco. (All Int., 1n)", "E_{cal} Reconstruction (All Int., 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_1n = new TH1D("E_{cal} reco. (QEL only, 1n)", "E_{cal} Reconstruction (QEL only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_1n = new TH1D("E_{cal} reco. (MEC only, 1n)", "E_{cal} Reconstruction (MEC only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_1n = new TH1D("E_{cal} reco. (RES only, 1n)", "E_{cal} Reconstruction (RES only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_1n = new TH1D("E_{cal} reco. (DIS only, 1n)", "E_{cal} Reconstruction (DIS only, 1n);E_{cal} = E_{e} + T_{n} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_1n_Dir = directories.Ecal_Directory_map["Ecal_All_Int_1n_Directory"];
    string hEcal_QEL_1n_Dir = directories.Ecal_Directory_map["Ecal_QEL_1n_Directory"];
    string hEcal_MEC_1n_Dir = directories.Ecal_Directory_map["Ecal_MEC_1n_Directory"];
    string hEcal_RES_1n_Dir = directories.Ecal_Directory_map["Ecal_RES_1n_Directory"];
    string hEcal_DIS_1n_Dir = directories.Ecal_Directory_map["Ecal_DIS_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (1n)">
    TH2D *hEcal_vs_P_e_1n = new TH2D("E_{cal} vs. P_{e} (All Int., 1n)", "E_{cal} vs. P_{e} (All Int., 1n);P_{e} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_1n = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1n)",
                                          "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., 1n);P_{e} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_e_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1n_Directory"];

    TH2D *hEcal_vs_P_n_1n = new TH2D("E_{cal} vs. P_{n} (All Int., 1n)", "E_{cal} vs. P_{n} (All Int., 1n);P_{n} [GeV/c];E_{cal} [GeV];",
                                     numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_n_test_1n = new TH2D("E_{cal} vs. P_{n} for E_{cal}>E_{beam} (All Int., 1n)",
                                          "E_{cal} vs. P_{n} for E_{cal}>E_{beam} (All Int., 1n);P_{n} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_n_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_1n_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (1n)">
    TH2D *hEcal_vs_Theta_e_1n = new TH2D("E_{cal} vs. #theta_{e} (All Int., 1n)",
                                         "E_{cal} vs. #theta_{e} (All Int., 1n);#theta_{e} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_1n = new TH2D("E_{cal} vs. #phi_{e} (All Int., 1n)",
                                       "E_{cal} vs. #phi_{e} (All Int., 1n);#phi_{e} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_1n = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1n)",
                                              "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., 1n);#theta_{e} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_1n = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1n)",
                                            "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., 1n);#phi_{e} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];
    string hEcal_vs_Phi_e_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];

    TH2D *hEcal_vs_Theta_n_1n = new TH2D("E_{cal} vs. #theta_{n} (All Int., 1n)",
                                         "E_{cal} vs. #theta_{n} (All Int., 1n);#theta_{n} [Deg];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_n_1n = new TH2D("E_{cal} vs. #phi_{n} (All Int., 1n)",
                                       "E_{cal} vs. #phi_{n} (All Int., 1n);#phi_{n} [Deg];E_{cal} [GeV];",
                                       numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_n_test_1n = new TH2D("E_{cal} vs. #theta_{n} for E_{cal}>E_{beam} (All Int., 1n)",
                                              "E_{cal} vs. #theta_{n} for E_{cal}>E_{beam} (All Int., 1n);#theta_{n} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_n_test_1n = new TH2D("E_{cal} vs. #phi_{n} for E_{cal}>E_{beam} (All Int., 1n)",
                                            "E_{cal} vs. #phi_{n} for E_{cal}>E_{beam} (All Int., 1n);#phi_{n} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_n_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];
    string hEcal_vs_Phi_n_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (1n)">
    TH2D *hEcal_vs_dAlpha_T_1n = new TH2D("E_{cal} vs. #delta#alpha_{T} (All Int., 1n)",
                                          "E_{cal} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [Deg];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1n_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (1n)">
    TH2D *hEcal_vs_dP_T_1n = new TH2D("E_{cal} vs. #deltaP_{T} (All Int., 1n)",
                                      "E_{cal} vs. #deltaP_{T} (All Int., 1n);#deltaP_{T} [GeV/c];E_{cal} [GeV];",
                                      numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_1n_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_1n_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (2p)">

    //<editor-fold desc="Ecal plots (2p)">
    THStack *sEcal_2p = new THStack("E_{cal} Reconstruction (2p)", "E_{cal} Reconstruction (2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]");
    string sEcal_2p_Dir = directories.Ecal_Directory_map["Ecal_stack_2p_Directory"];

    TH1D *hEcal_All_Int_2p = new TH1D("E_{cal} reco. (All Int., 2p)", "E_{cal} Reconstruction (All Int., 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_2p = new TH1D("E_{cal} reco. (QEL only, 2p)", "E_{cal} Reconstruction (QEL only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_2p = new TH1D("E_{cal} reco. (MEC only, 2p)", "E_{cal} Reconstruction (MEC only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_2p = new TH1D("E_{cal} reco. (RES only, 2p)", "E_{cal} Reconstruction (RES only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_2p = new TH1D("E_{cal} reco. (DIS only, 2p)", "E_{cal} Reconstruction (DIS only, 2p);E_{cal} = E_{e} + T_{p_{1}} + T_{p_{2}} [GeV]",
                                  numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_2p_Dir = directories.Ecal_Directory_map["Ecal_All_Int_2p_Directory"];
    string hEcal_QEL_2p_Dir = directories.Ecal_Directory_map["Ecal_QEL_2p_Directory"];
    string hEcal_MEC_2p_Dir = directories.Ecal_Directory_map["Ecal_MEC_2p_Directory"];
    string hEcal_RES_2p_Dir = directories.Ecal_Directory_map["Ecal_RES_2p_Directory"];
    string hEcal_DIS_2p_Dir = directories.Ecal_Directory_map["Ecal_DIS_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (2p)">
    TH2D *hEcal_vs_dAlpha_T_L_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                            "E_{cal} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_2p = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                              "E_{cal} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    string hEcal_vs_dAlpha_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (2p)">
    TH2D *hEcal_vs_dP_T_L_2p = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., 2p)",
                                        "E_{cal} vs. #deltaP_{T,L} (All Int., 2p);#deltaP_{T,L} [GeV/c];E_{cal} [GeV];",
                                        numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_2p = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., 2p)",
                                          "E_{cal} vs. #deltaP_{T,tot} (All Int., 2p);#deltaP_{T,tot} [GeV/c];E_{cal} [GeV];",
                                          numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    string hEcal_vs_dP_T_tot_2p_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_2p_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (pFDpCD)">

    //<editor-fold desc="Ecal plots (pFDpCD)">
    THStack *sEcal_pFDpCD = new THStack("E_{cal} Reconstruction (pFDpCD)", "E_{cal} Reconstruction (pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]");
    string sEcal_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_pFDpCD_Directory"];

    TH1D *hEcal_All_Int_pFDpCD = new TH1D("E_{cal} reco. (All Int., pFDpCD)", "E_{cal} Reconstruction (All Int., pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                          numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_pFDpCD = new TH1D("E_{cal} reco. (QEL only, pFDpCD)", "E_{cal} Reconstruction (QEL only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_pFDpCD = new TH1D("E_{cal} reco. (MEC only, pFDpCD)", "E_{cal} Reconstruction (MEC only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_pFDpCD = new TH1D("E_{cal} reco. (RES only, pFDpCD)", "E_{cal} Reconstruction (RES only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_pFDpCD = new TH1D("E_{cal} reco. (DIS only, pFDpCD)", "E_{cal} Reconstruction (DIS only, pFDpCD);E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_All_Int_pFDpCD_Directory"];
    string hEcal_QEL_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_QEL_pFDpCD_Directory"];
    string hEcal_MEC_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_MEC_pFDpCD_Directory"];
    string hEcal_RES_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_RES_pFDpCD_Directory"];
    string hEcal_DIS_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_DIS_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (pFDpCD)">
    TH2D *hEcal_vs_P_e_pFDpCD = new TH2D("E_{cal} vs. P_{e} (All Int., pFDpCD)", "E_{cal} vs. P_{e} (All Int., pFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_pFDpCD = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                              "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_e_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"];

    TH2D *hEcal_vs_P_pFD_pFDpCD = new TH2D("E_{cal} vs. P_{pFD} (All Int., pFDpCD)", "E_{cal} vs. P_{pFD} (All Int., pFDpCD);P_{pFD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_pFDpCD = new TH2D("E_{cal} vs. P_{pCD} (All Int., pFDpCD)", "E_{cal} vs. P_{pCD} (All Int., pFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_pFD_test_pFDpCD = new TH2D("E_{cal} vs. P_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                "E_{cal} vs. P_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{pFD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_test_pFDpCD = new TH2D("E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                "E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_pFD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"];
    string hEcal_vs_P_pCD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_pFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (pFDpCD)">
    TH2D *hEcal_vs_Theta_e_pFDpCD = new TH2D("E_{cal} vs. #theta_{e} (All Int., pFDpCD)",
                                             "E_{cal} vs. #theta_{e} (All Int., pFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_pFDpCD = new TH2D("E_{cal} vs. #phi_{e} (All Int., pFDpCD)",
                                           "E_{cal} vs. #phi_{e} (All Int., pFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_pFDpCD = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_pFDpCD = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    string hEcal_vs_Phi_e_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_pFD_pFDpCD = new TH2D("E_{cal} vs. #theta_{pFD} (All Int., pFDpCD)",
                                               "E_{cal} vs. #theta_{pFD} (All Int., pFDpCD);#theta_{pFD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_pFD_pFDpCD = new TH2D("E_{cal} vs. #phi_{pFD} (All Int., pFDpCD)",
                                             "E_{cal} vs. #phi_{pFD} (All Int., pFDpCD);#phi_{pFD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_pFD_test_pFDpCD = new TH2D("E_{cal} vs. #theta_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                    "E_{cal} vs. #theta_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{pFD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_pFD_test_pFDpCD = new TH2D("E_{cal} vs. #phi_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #phi_{pFD} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{pFD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_pFD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    string hEcal_vs_Phi_pFD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_pCD_pFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} (All Int., pFDpCD)",
                                               "E_{cal} vs. #theta_{pCD} (All Int., pFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_pFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} (All Int., pFDpCD)",
                                             "E_{cal} vs. #phi_{pCD} (All Int., pFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_pCD_test_pFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                    "E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_test_pFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., pFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_pCD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    string hEcal_vs_Phi_pCD_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (pFDpCD)">
    TH2D *hEcal_vs_dAlpha_T_L_pFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., pFDpCD)",
                                                "E_{cal} vs. #delta#alpha_{T,L} (All Int., pFDpCD);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_pFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
                                                  "E_{cal} vs. #delta#alpha_{T,tot} (All Int., pFDpCD);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    string hEcal_vs_dAlpha_T_tot_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (pFDpCD)">
    TH2D *hEcal_vs_dP_T_L_pFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., pFDpCD)",
                                            "E_{cal} vs. #deltaP_{T,L} (All Int., pFDpCD);#deltaP_{T,L} [GeV];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_pFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD)",
                                              "E_{cal} vs. #deltaP_{T,tot} (All Int., pFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    string hEcal_vs_dP_T_tot_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. W (pFDpCD)">
    TH2D *hEcal_vs_W_pFDpCD = new TH2D("E_{cal} vs. W (All Int., pFDpCD)",
                                       "E_{cal} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];E_{cal} = E_{e} + T_{pFD} + T_{pCD} [GeV];",
                                       numTH2Dbins_E_cal_Plots, W_lboundary, W_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_W_pFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Ecal reconstruction histograms (nFDpCD)">

    //<editor-fold desc="Ecal plots (nFDpCD)">
    THStack *sEcal_nFDpCD = new THStack("E_{cal} Reconstruction (nFDpCD)", "E_{cal} Reconstruction (nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]");
    string sEcal_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_nFDpCD_Directory"];

    TH1D *hEcal_All_Int_nFDpCD = new TH1D("E_{cal} reco. (All Int., nFDpCD)", "E_{cal} Reconstruction (All Int., nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                          numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_QEL_nFDpCD = new TH1D("E_{cal} reco. (QEL only, nFDpCD)", "E_{cal} Reconstruction (QEL only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_MEC_nFDpCD = new TH1D("E_{cal} reco. (MEC only, nFDpCD)", "E_{cal} Reconstruction (MEC only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_RES_nFDpCD = new TH1D("E_{cal} reco. (RES only, nFDpCD)", "E_{cal} Reconstruction (RES only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH1D *hEcal_DIS_nFDpCD = new TH1D("E_{cal} reco. (DIS only, nFDpCD)", "E_{cal} Reconstruction (DIS only, nFDpCD);E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV]",
                                      numTH1Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_All_Int_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_All_Int_nFDpCD_Directory"];
    string hEcal_QEL_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_QEL_nFDpCD_Directory"];
    string hEcal_MEC_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_MEC_nFDpCD_Directory"];
    string hEcal_RES_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_RES_nFDpCD_Directory"];
    string hEcal_DIS_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_DIS_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. momentum (nFDpCD)">
    TH2D *hEcal_vs_P_e_nFDpCD = new TH2D("E_{cal} vs. P_{e} (All Int., nFDpCD)", "E_{cal} vs. P_{e} (All Int., nFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                         numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_e_test_nFDpCD = new TH2D("E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                              "E_{cal} vs. P_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{e} [GeV/c];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, Momentum_lboundary, Momentum_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_e_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"];

    TH2D *hEcal_vs_P_nFD_nFDpCD = new TH2D("E_{cal} vs. P_{nFD} (All Int., nFDpCD)", "E_{cal} vs. P_{nFD} (All Int., nFDpCD);P_{nFD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_nFDpCD = new TH2D("E_{cal} vs. P_{pCD} (All Int., nFDpCD)", "E_{cal} vs. P_{pCD} (All Int., nFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_nFD_test_nFDpCD = new TH2D("E_{cal} vs. P_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                "E_{cal} vs. P_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{nFD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucFD_lboundary, P_nucFD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_P_pCD_test_nFDpCD = new TH2D("E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                "E_{cal} vs. P_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);P_{pCD} [GeV/c];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, P_nucCD_lboundary, P_nucCD_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_P_nFD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"];
    string hEcal_vs_P_pCD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Mom_nFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. angles (nFDpCD)">
    TH2D *hEcal_vs_Theta_e_nFDpCD = new TH2D("E_{cal} vs. #theta_{e} (All Int., nFDpCD)",
                                             "E_{cal} vs. #theta_{e} (All Int., nFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_nFDpCD = new TH2D("E_{cal} vs. #phi_{e} (All Int., nFDpCD)",
                                           "E_{cal} vs. #phi_{e} (All Int., nFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                           numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_e_test_nFDpCD = new TH2D("E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #theta_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{e} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_e_test_nFDpCD = new TH2D("E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                "E_{cal} vs. #phi_{e} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{e} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_e_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    string hEcal_vs_Phi_e_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_nFD_nFDpCD = new TH2D("E_{cal} vs. #theta_{nFD} (All Int., nFDpCD)",
                                               "E_{cal} vs. #theta_{nFD} (All Int., nFDpCD);#theta_{nFD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_nFD_nFDpCD = new TH2D("E_{cal} vs. #phi_{nFD} (All Int., nFDpCD)",
                                             "E_{cal} vs. #phi_{nFD} (All Int., nFDpCD);#phi_{nFD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_nFD_test_nFDpCD = new TH2D("E_{cal} vs. #theta_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                    "E_{cal} vs. #theta_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{nFD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_nFD_test_nFDpCD = new TH2D("E_{cal} vs. #phi_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #phi_{nFD} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{nFD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_nFD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    string hEcal_vs_Phi_nFD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];

    TH2D *hEcal_vs_Theta_pCD_nFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} (All Int., nFDpCD)",
                                               "E_{cal} vs. #theta_{pCD} (All Int., nFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                               numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_nFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} (All Int., nFDpCD)",
                                             "E_{cal} vs. #phi_{pCD} (All Int., nFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                             numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Theta_pCD_test_nFDpCD = new TH2D("E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                    "E_{cal} vs. #theta_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);#theta_{pCD} [Deg];E_{cal} [GeV];",
                                                    numTH2Dbins_E_cal_Plots, Theta_lboundary_CD, Theta_uboundary_CD, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_Phi_pCD_test_nFDpCD = new TH2D("E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #phi_{pCD} for E_{cal}>E_{beam} (All Int., nFDpCD);#phi_{pCD} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_Theta_pCD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    string hEcal_vs_Phi_pCD_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_Ang_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. dAlpha_T (nFDpCD)">
    TH2D *hEcal_vs_dAlpha_T_L_nFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,L} (All Int., nFDpCD)",
                                                "E_{cal} vs. #delta#alpha_{T,L} (All Int., nFDpCD);#delta#alpha_{T,L} [Deg];E_{cal} [GeV];",
                                                numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dAlpha_T_tot_nFDpCD = new TH2D("E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
                                                  "E_{cal} vs. #delta#alpha_{T,tot} (All Int., nFDpCD);#delta#alpha_{T,tot} [Deg];E_{cal} [GeV];",
                                                  numTH2Dbins_E_cal_Plots, 0, 180, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dAlpha_T_L_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    string hEcal_vs_dAlpha_T_tot_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    //</editor-fold>`

    //<editor-fold desc="Ecal vs. dP_T (nFDpCD)">
    TH2D *hEcal_vs_dP_T_L_nFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,L} (All Int., nFDpCD)",
                                            "E_{cal} vs. #deltaP_{T,L} (All Int., nFDpCD);#deltaP_{T,L} [GeV];E_{cal} [GeV];",
                                            numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    TH2D *hEcal_vs_dP_T_tot_nFDpCD = new TH2D("E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD)",
                                              "E_{cal} vs. #deltaP_{T,tot} (All Int., nFDpCD);#deltaP_{T,tot} [GeV];E_{cal} [GeV];",
                                              numTH2Dbins_E_cal_Plots, 0, dP_T_boundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_dP_T_L_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    string hEcal_vs_dP_T_tot_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_rec_vs_TKI_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="Ecal vs. W (nFDpCD)">
    TH2D *hEcal_vs_W_nFDpCD = new TH2D("E_{cal} vs. W (All Int., nFDpCD)",
                                       "E_{cal} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];E_{cal} = E_{e} + T_{nFD} + T_{pCD} [GeV];",
                                       numTH2Dbins_E_cal_Plots, W_lboundary, W_uboundary, numTH2Dbins_E_cal_Plots, 0, beamE * 1.35);
    string hEcal_vs_W_nFDpCD_Dir = directories.Ecal_Directory_map["Ecal_stack_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// TKI histograms
// ======================================================================================================================================================================

    //<editor-fold desc="TKI histograms">

    //<editor-fold desc="TKI histograms (1p)">
    THStack *sdP_T_1p = new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1p);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_1p = new TH1D("#deltaP_{T} (1p)", "#deltaP_{T} histogram (1p);#deltaP_{T} = |#vec{p}_{T,e} + #vec{p}_{T,p}| [GeV/c]",
                              numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_1p_Dir = directories.TKI_Directory_map["dP_T_1p_Directory"];

    THStack *sdAlpha_T_1p = new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_1p = new TH1D("#delta#alpha_{T} (1p)", "#delta#alpha_{T} histogram (1p);#delta#alpha_{T} [Deg]",
                                  numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_1p_Dir = directories.TKI_Directory_map["dAlpha_T_1p_Directory"];

    THStack *sdPhi_T_1p = new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_1p = new TH1D("#delta#phi_{T} (1p)", "#delta#phi_{T} histogram (1p);#delta#phi_{T} [Deg]",
                                numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_1p_Dir = directories.TKI_Directory_map["dPhi_T_1p_Directory"];

    TH2D *hdP_T_vs_dAlpha_T_1p = new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p)",
                                          "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1p);#delta#alpha_{T} [Deg];#deltaP_{T} [GeV/c];",
                                          numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_vs_dAlpha_T_1p_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_1p_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI histograms (1n)">
    THStack *sdP_T_1n = new THStack("#deltaP_{T} vs. #deltaP_{T,tot} (1n)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (1n);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_1n = new TH1D("#deltaP_{T} (1n)", "#deltaP_{T} histogram (1n);#deltaP_{T} = |#vec{p}_{T,e} + #vec{p}_{T,n}| [GeV/c]",
                              numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_1n_Dir = directories.TKI_Directory_map["dP_T_1n_Directory"];

    THStack *sdAlpha_T_1n = new THStack("#delta#alpha_{T} & #delta#alpha_{T,tot} (1n)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (1n);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_1n = new TH1D("#delta#alpha_{T} (1n)", "#delta#alpha_{T} histogram (1n);#delta#alpha_{T} [Deg]",
                                  numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_1n_Dir = directories.TKI_Directory_map["dAlpha_T_1n_Directory"];

    THStack *sdPhi_T_1n = new THStack("#delta#phi_{T} & #delta#phi_{T,tot} (1n)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (1n);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_1n = new TH1D("#delta#phi_{T} (1n)", "#delta#phi_{T} histogram (1n);#delta#phi_{T} [Deg]",
                                numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_1n_Dir = directories.TKI_Directory_map["dPhi_T_1n_Directory"];

    TH2D *hdP_T_vs_dAlpha_T_1n = new TH2D("#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n)",
                                          "#deltaP_{T} vs. #delta#alpha_{T} (All Int., 1n);#delta#alpha_{T} [Deg];#deltaP_{T} [GeV/c];",
                                          numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_vs_dAlpha_T_1n_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_1n_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI histograms (2p)">
    THStack *sdP_T_2p = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (2p)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (2p);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_2p = new TH1D("#deltaP_{T,L} (2p)", "#deltaP_{T,L} by Leading Proton (2p);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,1}| [GeV/c]",
                                numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    TH1D *hdP_T_tot_2p = new TH1D("#deltaP_{T,tot} (2p)",
                                  "#deltaP_{T,tot} by Momentum Sum (2p);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,1} + #vec{p}_{T,2}| [GeV/c]",
                                  numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_L_2p_Dir = directories.TKI_Directory_map["dP_T_2p_Directory"];
    string hdP_T_tot_2p_Dir = directories.TKI_Directory_map["dP_T_2p_Directory"];

    THStack *sdAlpha_T_2p = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (2p);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_2p = new TH1D("#delta#alpha_{T,L} (2p)", "#delta#alpha_{T,L} by Leading Proton (2p);#delta#alpha_{T,L} [Deg]",
                                    numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    TH1D *hdAlpha_T_tot_2p = new TH1D("#delta#alpha_{T,tot} (2p)", "#delta#alpha_{T,tot} by Momentum Sum (2p);#delta#alpha_{T,tot} [Deg]",
                                      numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_L_2p_Dir = directories.TKI_Directory_map["dAlpha_T_2p_Directory"];
    string hdAlpha_T_tot_2p_Dir = directories.TKI_Directory_map["dAlpha_T_2p_Directory"];

    THStack *sdPhi_T_2p = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (2p)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (2p);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_2p = new TH1D("#delta#phi_{T,L} (2p)", "#delta#phi_{T,L} by Leading Proton (2p);#delta#phi_{T,L} [Deg]",
                                  numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    TH1D *hdPhi_T_tot_2p = new TH1D("#delta#phi_{T,tot} (2p)", "#delta#phi_{T,tot} by Momentum Sum (2p);#delta#phi_{T,tot} [Deg]",
                                    numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_L_2p_Dir = directories.TKI_Directory_map["dPhi_T_2p_Directory"];
    string hdPhi_T_tot_2p_Dir = directories.TKI_Directory_map["dPhi_T_2p_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_2p = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p)",
                                              "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., 2p);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                              numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_2p = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p)",
                                                  "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., 2p);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                  numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_2p_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_2p_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_2p_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI histograms (pFDpCD)">

    //<editor-fold desc="TKI plots (pFDpCD)">
    THStack *sdP_T_pFDpCD = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (pFDpCD)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (pFDpCD);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_pFDpCD = new TH1D("#deltaP_{T,L} (pFDpCD)", "#deltaP_{T,L} by leading proton (pFDpCD);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,pL}| [GeV/c]",
                                    numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    TH1D *hdP_T_tot_pFDpCD = new TH1D("#deltaP_{T,tot} (pFDpCD)",
                                      "#deltaP_{T,tot} by Momentum Sum (pFDpCD);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}| [GeV/c]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];
    string hdP_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];

    THStack *sdAlpha_T_pFDpCD = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (pFDpCD)",
                                            "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (pFDpCD);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_pFDpCD = new TH1D("#delta#alpha_{T,L} (pFDpCD)", "#delta#alpha_{T,L} by leading proton (pFDpCD);#delta#alpha_{T,L} [Deg]",
                                        numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    TH1D *hdAlpha_T_tot_pFDpCD = new TH1D("#delta#alpha_{T,tot} (pFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (pFDpCD);#delta#alpha_{T,tot} [Deg]",
                                          numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];
    string hdAlpha_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];

    THStack *sdPhi_T_pFDpCD = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (pFDpCD)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (pFDpCD);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_pFDpCD = new TH1D("#delta#phi_{T,L} (pFDpCD)", "#delta#phi_{T,L} by leading proton (pFDpCD);#delta#phi_{T,L} [Deg]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    TH1D *hdPhi_T_tot_pFDpCD = new TH1D("#delta#phi_{T,tot} (pFDpCD)", "#delta#phi_{T,tot} by Momentum Sum (pFDpCD);#delta#phi_{T,tot} [Deg]",
                                        numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_pFDpCD_Directory"];
    string hdPhi_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_pFDpCD_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_pFDpCD = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., pFDpCD)",
                                                  "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., pFDpCD);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                                  numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_pFDpCD = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD)",
                                                      "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., pFDpCD);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                      numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_pFDpCD_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_pFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI vs. W (pFDpCD)">
    TH2D *hdP_T_L_vs_W_pFDpCD = new TH2D("#deltaP_{T,L} vs. W (All Int., pFDpCD)",
                                         "#deltaP_{T,L} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                         "#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,pL}| [GeV/c];",
                                         numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_W_pFDpCD = new TH2D("#deltaP_{T,tot} vs. W (All Int., pFDpCD)",
                                           "#deltaP_{T,tot} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                           "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,pFD} + #vec{p}_{T,pCD}| [GeV/c];",
                                           numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];
    string hdP_T_tot_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dP_T_pFDpCD_Directory"];

    TH2D *hdAlpha_T_L_vs_W_pFDpCD = new TH2D("#delta#alpha_{T,L} vs. W (All Int., pFDpCD)",
                                             "#delta#alpha_{T,L} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,L} [Deg];",
                                             numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    TH2D *hdAlpha_T_tot_vs_W_pFDpCD = new TH2D("#delta#alpha_{T,tot} vs. W (All Int., pFDpCD)",
                                               "#delta#alpha_{T,tot} vs. W (All Int., pFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,tot} [Deg];",
                                               numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    string hdAlpha_T_L_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];
    string hdAlpha_T_tot_vs_W_pFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_pFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="TKI histograms (nFDpCD)">

    //<editor-fold desc="TKI plots (nFDpCD)">
    THStack *sdP_T_nFDpCD = new THStack("#deltaP_{T,L} & #deltaP_{T,tot} (nFDpCD)", "#deltaP_{T,L} vs. #deltaP_{T,tot} (nFDpCD);#deltaP_{T} [GeV/c]");
    TH1D *hdP_T_L_nFDpCD = new TH1D("#deltaP_{T,L} (nFDpCD)", "#deltaP_{T,L} by leading nucleon (nFDpCD);#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,nL}| [GeV/c]",
                                    numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    TH1D *hdP_T_tot_nFDpCD = new TH1D("#deltaP_{T,tot} (nFDpCD)",
                                      "#deltaP_{T,tot} by Momentum Sum (nFDpCD);#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}| [GeV/c]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, dP_T_boundary);
    string hdP_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];
    string hdP_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];

    THStack *sdAlpha_T_nFDpCD = new THStack("#delta#alpha_{T,L} & #delta#alpha_{T,tot} (nFDpCD)",
                                            "#delta#alpha_{T,L} vs. #delta#alpha_{T,tot} (nFDpCD);#delta#alpha_{T} [Deg]");
    TH1D *hdAlpha_T_L_nFDpCD = new TH1D("#delta#alpha_{T,L} (nFDpCD)", "#delta#alpha_{T,L} by leading nucleon (nFDpCD);#delta#alpha_{T,L} [Deg]",
                                        numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    TH1D *hdAlpha_T_tot_nFDpCD = new TH1D("#delta#alpha_{T,tot} (nFDpCD)", "#delta#alpha_{T,tot} by Momentum Sum (nFDpCD);#delta#alpha_{T,tot} [Deg]",
                                          numTH1Dbins_TKI_dAlpha_T_Plots, 0, 180);
    string hdAlpha_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];
    string hdAlpha_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];

    THStack *sdPhi_T_nFDpCD = new THStack("#delta#phi_{T,L} & #delta#phi_{T,tot} (nFDpCD)", "#delta#phi_{T,L} vs. #delta#phi_{T,tot} (nFDpCD);#delta#phi_{T} [Deg]");
    TH1D *hdPhi_T_L_nFDpCD = new TH1D("#delta#phi_{T,L} (nFDpCD)", "#delta#phi_{T,L} by leading nucleon (nFDpCD);#delta#phi_{T,L} [Deg]",
                                      numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    TH1D *hdPhi_T_tot_nFDpCD = new TH1D("#delta#phi_{T,tot} (nFDpCD)", "#delta#phi_{T,tot} by Momentum Sum (nFDpCD);#delta#phi_{T,tot} [Deg]",
                                        numTH1Dbins_TKI_dP_T_Plots, 0, 180);
    string hdPhi_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_nFDpCD_Directory"];
    string hdPhi_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dPhi_T_nFDpCD_Directory"];

    TH2D *hdP_T_L_vs_dAlpha_T_L_nFDpCD = new TH2D("#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., nFDpCD)",
                                                  "#deltaP_{T,L} vs. #delta#alpha_{T,L} (All Int., nFDpCD);#delta#alpha_{T,L} [Deg];#deltaP_{T,L} [GeV/c];",
                                                  numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_dAlpha_T_tot_nFDpCD = new TH2D("#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD)",
                                                      "#deltaP_{T,tot} vs. #delta#alpha_{T,tot} (All Int., nFDpCD);#delta#alpha_{T,tot} [Deg];#deltaP_{T,tot} [GeV/c];",
                                                      numTH2Dbins_TKI_Plots, 0, 180, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_dAlpha_T_L_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_nFDpCD_Directory"];
    string hdP_T_tot_vs_dAlpha_T_tot_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_vs_dAlpha_T_nFDpCD_Directory"];
    //</editor-fold>

    //<editor-fold desc="TKI vs. W (nFDpCD)">
    TH2D *hdP_T_L_vs_W_nFDpCD = new TH2D("#deltaP_{T,L} vs. W (All Int., nFDpCD)",
                                         "#deltaP_{T,L} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                         "#deltaP_{T,L} = |#vec{p}_{T,e} + #vec{p}_{T,nL}| [GeV/c];",
                                         numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    TH2D *hdP_T_tot_vs_W_nFDpCD = new TH2D("#deltaP_{T,tot} vs. W (All Int., nFDpCD)",
                                           "#deltaP_{T,tot} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];"
                                           "#deltaP_{T,tot} = |#vec{p}_{T,e} + #vec{p}_{T,nFD} + #vec{p}_{T,pCD}| [GeV/c];",
                                           numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, dP_T_boundary);
    string hdP_T_L_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];
    string hdP_T_tot_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dP_T_nFDpCD_Directory"];

    TH2D *hdAlpha_T_L_vs_W_nFDpCD = new TH2D("#delta#alpha_{T,L} vs. W (All Int., nFDpCD)",
                                             "#delta#alpha_{T,L} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,L} [Deg];",
                                             numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    TH2D *hdAlpha_T_tot_vs_W_nFDpCD = new TH2D("#delta#alpha_{T,tot} vs. W (All Int., nFDpCD)",
                                               "#delta#alpha_{T,tot} vs. W (All Int., nFDpCD);W = #sqrt{(#omega + m_{p})^{2} - #vec{q}^{2}}  [GeV];#delta#alpha_{T,tot} [Deg];",
                                               numTH2Dbins_TKI_Plots, W_lboundary, W_uboundary, numTH2Dbins_TKI_Plots, 0, 180);
    string hdAlpha_T_L_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];
    string hdAlpha_T_tot_vs_W_nFDpCD_Dir = directories.TKI_Directory_map["dAlpha_T_nFDpCD_Directory"];
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Efficiency histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Efficiency histograms">

    //<editor-fold desc="Efficiency plots (1e cut)">

    //<editor-fold desc="Truth level momentum plots (1e cut)">
    hPlot1D hP_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "01_P_e_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "01_P_e_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "02_P_n_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "02_P_n_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "03_P_p_AC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "03_P_p_BC_truth_1e_cut",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "04_P_piplus_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "04_P_piplus_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "05_P_piminus_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "05_P_piminus_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "06_P_pi0_AC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "06_P_pi0_BC_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "07_P_ph_AC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "07_P_ph_BC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_Kp_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL K^{+} momentum AC", "K^{+} momentum P^{truth}_{K^{+}} AC", "P^{truth}_{K^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "04_P_Kplus_AC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_Kp_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL K^{+} momentum BC", "K^{+} momentum P^{truth}_{K^{+}} BC", "P^{truth}_{K^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1e_cut_Directory"], "04_P_Kplus_BC_truth_1e_cut",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL momentum plots for momentum thresholds (1e cut)">
    hPlot1D hP_e_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD Electron momentum", "FD Electron momentum P^{truth}_{e}", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "01a_P_e_truth_1e_cut_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD Electron momentum - ZOOMIN", "FD Electron momentum P^{truth}_{e} - ZOOMIN",
                                                  "P^{truth}_{e} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                  "01a_P_e_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD Proton momentum", "FD Proton momentum P^{truth}_{p}", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "02a_P_p_truth_1e_cut_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_truth_1e_cut_CD = hPlot1D("1e cut", "CD", "TL CD Proton momentum", "CD Proton momentum P^{truth}_{p}", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "02b_P_p_truth_1e_cut_CD",
                                           CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD Proton momentum - ZOOMIN", "FD Proton momentum P^{truth}_{p} - ZOOMIN",
                                                  "P^{truth}_{p} [GeV/c]",
                                                  directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "02a_P_p_truth_1e_cut_FD_ZOOMIN",
                                                  0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_truth_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "TL CD Proton momentum - ZOOMIN", "CD Proton momentum P^{truth}_{p} - ZOOMIN",
                                                  "P^{truth}_{p} [GeV/c]",
                                                  directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "02b_P_p_truth_1e_cut_CD_ZOOMIN",
                                                  0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD Neutron momentum", "FD Neutron momentum P^{truth}_{n}", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "03a_P_n_truth_1e_cut_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots); // leading nFD!
    hPlot1D hP_n_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD Neutron momentum - ZOOMIN", "FD Neutron momentum P^{truth}_{n} - ZOOMIN",
                                                  "P^{truth}_{n} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                  "03a_P_n_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots); // leading nFD!

    hPlot1D hP_piplus_truth_1e_cut = hPlot1D("1e cut", "CD & FD", "TL #pi^{+} momentum", "#pi^{+} momentum P^{truth}_{#pi^{+}}", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "04_P_piplus_truth_1e_cut",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "TL #pi^{+} momentum - ZOOMIN", "#pi^{+} momentum P^{truth}_{#pi^{+}} - ZOOMIN",
                                                    "P^{truth}_{#pi^{+}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                    "04_P_piplus_truth_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD #pi^{+} momentum", "FD #pi^{+} momentum P^{truth}_{#pi^{+}}", "P^{truth}_{#pi^{+}} [GeV/c]",
                                                directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "04a_P_piplus_truth_1e_cut_FD",
                                                Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_CD = hPlot1D("1e cut", "CD", "TL CD #pi^{+} momentum", "CD #pi^{+} momentum P^{truth}_{#pi^{+}}", "P^{truth}_{#pi^{+}} [GeV/c]",
                                                directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "04b_P_piplus_truth_1e_cut_CD",
                                                CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD #pi^{+} momentum - ZOOMIN", "FD #pi^{+} momentum P^{truth}_{#pi^{+}} - ZOOMIN",
                                                       "P^{truth}_{#pi^{+}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                       "04a_P_piplus_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piplus_truth_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "TL CD #pi^{+} momentum - ZOOMIN", "CD #pi^{+} momentum P^{truth}_{#pi^{+}} - ZOOMIN",
                                                       "P^{truth}_{#pi^{+}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                       "04b_P_piplus_truth_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_piminus_truth_1e_cut = hPlot1D("1e cut", "CD & FD", "TL #pi^{-} momentum", "#pi^{-} momentum P^{truth}_{#pi^{-}}", "P^{truth}_{#pi^{-}} [GeV/c]",
                                              directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "05_P_piminus_truth_1e_cut",
                                              Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_ZOOMIN = hPlot1D("1e cut", "CD & FD", "TL #pi^{-} momentum - ZOOMIN", "#pi^{-} momentum P^{truth}_{#pi^{-}} - ZOOMIN",
                                                     "P^{truth}_{#pi^{-}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                     "05_P_piminus_truth_1e_cut_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD #pi^{-} momentum", "FD #pi^{-} momentum P^{truth}_{#pi^{-}}", "P^{truth}_{#pi^{-}} [GeV/c]",
                                                 directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "05a_P_piminus_truth_1e_cut_FD",
                                                 Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_CD = hPlot1D("1e cut", "CD", "TL CD #pi^{-} momentum", "CD #pi^{-} momentum P^{truth}_{#pi^{-}}", "P^{truth}_{#pi^{-}} [GeV/c]",
                                                 directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "05b_P_piminus_truth_1e_cut_CD",
                                                 CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD #pi^{-} momentum - ZOOMIN", "FD #pi^{-} momentum P^{truth}_{#pi^{-}} - ZOOMIN",
                                                        "P^{truth}_{#pi^{-}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                        "05a_P_piminus_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_piminus_truth_1e_cut_CD_ZOOMIN = hPlot1D("1e cut", "CD", "TL CD #pi^{-} momentum - ZOOMIN", "CD #pi^{-} momentum P^{truth}_{#pi^{-}} - ZOOMIN",
                                                        "P^{truth}_{#pi^{-}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                        "05b_P_piminus_truth_1e_cut_CD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_truth_1e_cut_FD = hPlot1D("1e cut", "FD", "TL FD #gamma momentum", "FD #gamma momentum P^{truth}_{#gamma}", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"], "09a_P_ph_truth_1e_cut_FD",
//                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
                                            CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_truth_1e_cut_FD_ZOOMIN = hPlot1D("1e cut", "FD", "TL FD #gamma momentum - ZOOMIN", "FD #gamma momentum P^{truth}_{#gamma} - ZOOMIN",
                                                   "P^{truth}_{#gamma} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Momentum_th_TL_1e_cut_Directory"],
                                                   "09a_P_ph_truth_1e_cut_FD_ZOOMIN", 0, 1, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1e cut)">
    hPlot1D hTheta_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "01_Theta_e_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "01_Theta_e_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "02_Theta_n_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "02_Theta_n_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "03_Theta_p_AC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "03_Theta_p_BC_truth_1e_cut",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "04_Theta_piplus_AC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "04_Theta_piplus_BC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "05_Theta_piminus_AC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                 "05_Theta_piminus_BC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "06_Theta_pi0_AC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]",
                                                 directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"], "06_Theta_pi0_BC_truth_1e_cut",
                                                 Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                "07_Theta_ph_AC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1e_cut_Directory"],
                                                "07_Theta_ph_BC_truth_1e_cut", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1e cut)">
    hPlot1D hPhi_e_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "01_Phi_e_AC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "01_Phi_e_BC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "02_Phi_n_AC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "02_Phi_n_BC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "03_Phi_p_AC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "03_Phi_p_BC_truth_1e_cut",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "04_Phi_piplus_AC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "04_Phi_piplus_BC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "05_Phi_piminus_AC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "05_Phi_piminus_BC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "06_Phi_pi0_AC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"],
                                               "06_Phi_pi0_BC_truth_1e_cut", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "07_Phi_ph_AC_truth_1e_cut",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1e_cut = hPlot1D("1e cut", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1e_cut_Directory"], "07_Phi_ph_BC_truth_1e_cut",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (1e cut)">
    hPlot2D hTheta_e_vs_Phi_e_truth_1e_cut = hPlot2D("1e cut", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"],
                                                     "01_Theta_e_vs_Phi_e_truth_1e_cut",
                                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_truth_1e_cut = hPlot2D("1e cut", "FD", "TL #theta_{nFD} vs. #phi_{nFD}", "TL #theta_{nFD} vs. #phi_{nFD}", "#phi_{nFD} [Deg]",
                                                         "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"],
                                                         "02_Theta_nFD_vs_Phi_nFD_truth_1e_cut", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_truth_1e_cut = hPlot2D("1e cut", "FD", "TL #theta_{pFD} vs. #phi_{pFD}", "TL #theta_{pFD} vs. #phi_{pFD}", "#phi_{pFD} [Deg]",
                                                         "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["TL_Acceptance_Maps_1e_cut_Directory"],
                                                         "03_Theta_pFD_vs_Phi_pFD_truth_1e_cut", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (1p)">

    //<editor-fold desc="Truth level momentum plots (1p)">
    hPlot1D hP_e_AC_truth_1p = hPlot1D("1p", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "01_P_e_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_1p = hPlot1D("1p", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "01_P_e_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_1p = hPlot1D("1p", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "02_P_n_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_1p = hPlot1D("1p", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "02_P_n_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_1p = hPlot1D("1p", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03a_P_p_AC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1p = hPlot1D("1p", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03a_P_p_BC_truth_1p",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_AC_truth_1p = hPlot1D("1p", "FD", "FD TL Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03b_P_pFD_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_BC_truth_1p = hPlot1D("1p", "FD", "FD TL Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "03b_P_pFD_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "04_P_piplus_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "04_P_piplus_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "05_P_piminus_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "05_P_piminus_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "06_P_pi0_AC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "06_P_pi0_BC_truth_1p",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_1p = hPlot1D("1p", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07a_P_ph_AC_truth_1p",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1p = hPlot1D("1p", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07a_P_ph_BC_truth_1p",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07b_P_ph_AC_truth_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1p_Directory"], "07b_P_ph_BC_truth_1p_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1p)">
    hPlot1D hTheta_e_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "01_Theta_e_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "01_Theta_e_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "02_Theta_n_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "02_Theta_n_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "03a_Theta_p_AC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "03a_Theta_p_BC_truth_1p",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_AC_truth_1p = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing FD Proton AC", "#theta^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "03b_Theta_pFD_AC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_BC_truth_1p = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing FD Proton BC", "#theta^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "03b_Theta_pFD_BC_truth_1p",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "04_Theta_piplus_AC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "04_Theta_piplus_BC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "05_Theta_piminus_AC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "05_Theta_piminus_BC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "06_Theta_pi0_AC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                             "06_Theta_pi0_BC_truth_1p", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "07a_Theta_ph_AC_truth_1p",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1p = hPlot1D("1p", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"], "07a_Theta_ph_BC_truth_1p",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                               "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                               "07b_Theta_ph_AC_truth_1p_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                               "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1p_Directory"],
                                               "07b_Theta_ph_BC_truth_1p_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1p)">
    hPlot1D hPhi_e_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "01_Phi_e_AC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "01_Phi_e_BC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "02_Phi_n_AC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "02_Phi_n_BC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03a_Phi_p_AC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03a_Phi_p_BC_truth_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_AC_truth_1p = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of FD Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03b_Phi_pFD_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_BC_truth_1p = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of FD Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "03b_Phi_pFD_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "04_Phi_piplus_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "04_Phi_piplus_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "05_Phi_piminus_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "05_Phi_piminus_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "06_Phi_pi0_AC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "06_Phi_pi0_BC_truth_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "07a_Phi_ph_AC_truth_1p",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1p = hPlot1D("1p", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"], "07a_Phi_ph_BC_truth_1p",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                             "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"],
                                             "07b_Phi_ph_AC_truth_1p_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1p_FD = hPlot1D("1p", "FD", "FD TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                             "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1p_Directory"],
                                             "07b_Phi_ph_BC_truth_1p_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (1p)">
    hPlot2D hpFD_Hit_map_1p_BEC = hPlot2D("1p", "FD", "FD proton hit map BEC", "FD proton hit map BEC", "x_{pFD}", "y_{pFD}",
                                          directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                          "01a_Proton_FD_Hit_map_BEC_1p", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hpFD_Hit_map_1p_AEC = hPlot2D("1p", "FD", "FD proton hit map AEC", "FD proton hit map AEC", "x_{pFD}", "y_{pFD}",
                                          directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                          "01b_Proton_FD_Hit_map_AEC_1p", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_1p_BEC = hPlot2D("1p", "FD", "#theta_{pFD} vs. #phi_{pFD} BEC", "TL #theta_{pFD} vs. #phi_{pFD} BEC", "#phi_{pFD} [Deg]",
                                                   "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                   "02a_Theta_pFD_vs_Phi_pFD_BEC_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_pFD_vs_Phi_pFD_1p_AEC = hPlot2D("1p", "FD", "#theta_{pFD} vs. #phi_{pFD} AEC", "TL #theta_{pFD} vs. #phi_{pFD} AEC", "#phi_{pFD} [Deg]",
                                                   "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                   "02b_Theta_pFD_vs_Phi_pFD_AEC_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (1p)">
    hPlot2D hTheta_e_vs_Phi_e_truth_1p = hPlot2D("1p", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                 "#theta_{e} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                 "01_Theta_e_vs_Phi_e_truth_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_truth_1p = hPlot2D("1p", "FD", "TL #theta_{pFD} vs. #phi_{pFD}", "TL #theta_{pFD} vs. #phi_{pFD}", "#phi_{pFD} [Deg]",
                                                     "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_1p_Directory"],
                                                     "02_Theta_pFD_vs_Phi_pFD_truth_1p", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (1n)">

    //<editor-fold desc="Truth level momentum plots (1n)">
    hPlot1D hP_e_AC_truth_1n = hPlot1D("1n", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "01_P_e_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_1n = hPlot1D("1n", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "01_P_e_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_1n = hPlot1D("1n", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02a_P_n_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_1n = hPlot1D("1n", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02a_P_n_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_AC_truth_1n = hPlot1D("1n", "FD", "FD TL Neutron momentum AC", "FD Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02b_P_nFD_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_BC_truth_1n = hPlot1D("1n", "FD", "CD TL Neutron momentum BC", "FD Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "02b_P_nFD_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_1n = hPlot1D("1n", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03_P_p_AC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n = hPlot1D("1n", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                       directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03_P_p_BC_truth_1n",
                                       Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03FD_P_p_AC_truth_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03FD_P_p_BC_truth_1n_FD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD Proton momentum AC", "CD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03CD_P_p_AC_truth_1n_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD Proton momentum BC", "CD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                          directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03CD_P_p_BC_truth_1n_CD",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet Proton momentum AC", "undet Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03undet_P_p_AC_truth_1n_undet",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet Proton momentum BC", "undet Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "03undet_P_p_BC_truth_1n_undet",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04_P_piplus_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04_P_piplus_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{+} momentum AC", "FD #pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04FD_P_pip_AC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{+} momentum BC", "FD #pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04FD_P_pip_BC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{+} momentum AC", "CD #pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04CD_P_pip_AC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{+} momentum BC", "CD #pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "04CD_P_pip_BC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{+} momentum AC", "undet #pi^{+} momentum P^{truth}_{#pi^{+}} AC",
                                               "P^{truth}_{#pi^{+}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "04undet_P_pip_AC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{+} momentum BC", "undet #pi^{+} momentum P^{truth}_{#pi^{+}} BC",
                                               "P^{truth}_{#pi^{+}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "04undet_P_pip_BC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05_P_piminus_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05_P_piminus_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{-} momentum AC", "FD #pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05FD_P_pim_AC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #pi^{-} momentum BC", "FD #pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05FD_P_pim_BC_truth_1n_FD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{-} momentum AC", "CD #pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05CD_P_pim_AC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #pi^{-} momentum BC", "CD #pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "05CD_P_pim_BC_truth_1n_CD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{-} momentum AC", "undet #pi^{-} momentum P^{truth}_{#pi^{-}} AC",
                                               "P^{truth}_{#pi^{-}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "05undet_P_pim_AC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #pi^{-} momentum BC", "undet #pi^{-} momentum P^{truth}_{#pi^{-}} BC",
                                               "P^{truth}_{#pi^{-}} [GeV/c]", directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"],
                                               "05undet_P_pim_BC_truth_1n_undet", Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "06_P_pi0_AC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                         directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "06_P_pi0_BC_truth_1n",
                                         Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_1n = hPlot1D("1n", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07a_P_ph_AC_truth_1n",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1n = hPlot1D("1n", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                        directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07a_P_ph_BC_truth_1n",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07b_P_ph_AC_truth_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_1n_Directory"], "07b_P_ph_BC_truth_1n_FD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (1n)">
    hPlot1D hTheta_e_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "01_Theta_e_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "01_Theta_e_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "02a_Theta_n_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "02a_Theta_n_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_AC_truth_1n = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing FD Neutron AC", "#theta^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "02b_Theta_nFD_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_BC_truth_1n = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing FD Neutron BC", "#theta^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "02b_Theta_nFD_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03_Theta_p_AC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03_Theta_p_BC_truth_1n",
                                           Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{p} AC", "FD #theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03FD_Theta_p_AC_truth_1n_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{p} BC", "FD #theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03FD_Theta_p_BC_truth_1n_FD",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{p} AC", "CD #theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03CD_Theta_p_AC_truth_1n_CD",
                                              Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{p} BC", "CD #theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "03CD_Theta_p_BC_truth_1n_CD",
                                              Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{p} AC", "undet #theta^{truth}_{p} of Outgoing Proton AC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                 "03undet_Theta_p_AC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{p} BC", "undet #theta^{truth}_{p} of Outgoing Proton BC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                 "03undet_Theta_p_BC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "04_Theta_piplus_AC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                             "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "04_Theta_piplus_BC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{+}} AC", "FD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04FD_Theta_piplus_AC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{+}} BC", "FD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04FD_Theta_piplus_BC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{+}} AC", "CD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04CD_Theta_piplus_AC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{+}} BC", "CD #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "04CD_Theta_piplus_BC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{+}} AC", "undet #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                   "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "04undet_Theta_piplus_AC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{+}} BC", "undet #theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                   "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "04undet_Theta_piplus_BC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "05_Theta_piminus_AC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                             "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                             "05_Theta_piminus_BC_truth_1n", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{-}} AC", "FD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05FD_Theta_piminus_AC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #theta^{truth}_{#pi^{-}} BC", "FD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05FD_Theta_piminus_BC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{-}} AC", "CD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05CD_Theta_piminus_AC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #theta^{truth}_{#pi^{-}} BC", "CD #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                "05CD_Theta_piminus_BC_truth_1n_CD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{-}} AC", "undet #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                   "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "05undet_Theta_piminus_AC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #theta^{truth}_{#pi^{-}} BC", "undet #theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                   "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                                   "05undet_Theta_piminus_BC_truth_1n_undet", 0., 180., numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "06_Theta_pi0_AC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                             "#theta^{truth}_{#pi^{0}} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "06_Theta_pi0_BC_truth_1n",
                                             Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "07a_Theta_ph_AC_truth_1n",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1n = hPlot1D("1n", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                            "#theta^{truth}_{#gamma} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"], "07a_Theta_ph_BC_truth_1n",
                                            Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                               "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                               "07b_Theta_ph_AC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                               "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_1n_Directory"],
                                               "07b_Theta_ph_BC_truth_1n_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (1n)">
    hPlot1D hPhi_e_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "01_Phi_e_AC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "01_Phi_e_BC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02a_Phi_n_AC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02a_Phi_n_BC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_AC_truth_1n = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing FD Neutron AC", "#phi^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02b_Phi_nFD_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_BC_truth_1n = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing FD Neutron BC", "#phi^{truth}_{n} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "02b_Phi_nFD_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03_Phi_p_AC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                         directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03_Phi_p_BC_truth_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{p} AC", "FD #phi^{truth}_{p} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03FD_Phi_p_AC_truth_1n_FD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{p} BC", "FD #phi^{truth}_{p} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03FD_Phi_p_BC_truth_1n_FD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{p} AC", "CD #phi^{truth}_{p} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03CD_Phi_p_AC_truth_1n_CD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{p} BC", "CD #phi^{truth}_{p} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                            directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "03CD_Phi_p_BC_truth_1n_CD",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{p} AC", "undet #phi^{truth}_{p} of Outgoing Proton AC",
                                               "#phi^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                               "03undet_Phi_p_AC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{p} BC", "undet #phi^{truth}_{p} of Outgoing Proton BC",
                                               "#phi^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                               "03undet_Phi_p_BC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "04_Phi_piplus_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC", "#phi^{truth}_{#pi^{+}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "04_Phi_piplus_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{+}} AC", "FD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04FD_Phi_piplus_AC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{+}} BC", "FD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04FD_Phi_piplus_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{+}} AC", "CD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04CD_Phi_piplus_AC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{+}} BC", "CD #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                              "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "04CD_Phi_piplus_BC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{+}} AC", "undet #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "04undet_Phi_piplus_AC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{+}} BC", "undet #phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "04undet_Phi_piplus_BC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "05_Phi_piminus_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC", "#phi^{truth}_{#pi^{-}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "05_Phi_piminus_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_AC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{-}} AC", "FD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05FD_Phi_piminus_AC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n_FD = hPlot1D("1n", "FD", "TL FD #phi^{truth}_{#pi^{-}} BC", "FD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05FD_Phi_piminus_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_AC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{-}} AC", "CD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05CD_Phi_piminus_AC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n_CD = hPlot1D("1n", "CD", "TL CD #phi^{truth}_{#pi^{-}} BC", "CD #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                              "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                              "05CD_Phi_piminus_BC_truth_1n_CD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_AC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{-}} AC", "undet #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "05undet_Phi_piminus_AC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_1n_undet = hPlot1D("1n", "undet", "TL undet #phi^{truth}_{#pi^{-}} BC", "undet #phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                                 "05undet_Phi_piminus_BC_truth_1n_undet", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "06_Phi_pi0_AC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC", "#phi^{truth}_{#pi^{0}} [Deg]",
                                           directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "06_Phi_pi0_BC_truth_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "07a_Phi_ph_AC_truth_1n",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1n = hPlot1D("1n", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC", "#phi^{truth}_{#gamma} [Deg]",
                                          directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"], "07a_Phi_hp_BC_truth_1n",
                                          Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                             "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                             "07b_Phi_ph_AC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_1n_FD = hPlot1D("1n", "FD", "FD TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                             "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_1n_Directory"],
                                             "07b_Phi_hp_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (1n)">
    hPlot2D hnFD_Hit_map_1n_BEC = hPlot2D("1n", "FD", "FD neutron hit map BEC", "FD neutron hit map BEC", "x_{nFD}", "y_{nFD}",
                                          directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                          "01a_Neutron_FD_Hit_map_BEC_1n", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hnFD_Hit_map_1n_AEC = hPlot2D("1n", "FD", "FD neutron hit map AEC", "FD neutron hit map AEC", "x_{nFD}", "y_{nFD}",
                                          directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                          "01b_Neutron_FD_Hit_map_AEC_1n", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                          numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_1n_BEC = hPlot2D("1n", "FD", "#theta_{nFD} vs. #phi_{nFD} BEC", "TL #theta_{nFD} vs. #phi_{nFD} BEC", "#phi_{nFD} [Deg]",
                                                   "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                   "02a_Theta_nFD_vs_Phi_nFD_BEC_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_nFD_vs_Phi_nFD_1n_AEC = hPlot2D("1n", "FD", "#theta_{nFD} vs. #phi_{nFD} AEC", "TL #theta_{nFD} vs. #phi_{nFD} AEC", "#phi_{nFD} [Deg]",
                                                   "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                   "02b_Theta_nFD_vs_Phi_nFD_AEC_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                   numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (1n)">
    hPlot2D hTheta_e_vs_Phi_e_truth_1n = hPlot2D("1n", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]",
                                                 "#theta_{e} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                 "01_Theta_e_vs_Phi_e_truth_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                 numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_truth_1n = hPlot2D("1n", "FD", "TL #theta_{nFD} vs. #phi_{nFD}", "TL #theta_{nFD} vs. #phi_{nFD}", "#phi_{nFD} [Deg]",
                                                     "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_1n_Directory"],
                                                     "02_Theta_nFD_vs_Phi_nFD_truth_1n", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (pFDpCD)">

    //<editor-fold desc="Truth level momentum plots (pFDpCD)">
    hPlot1D hP_e_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "01_P_e_AC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "01_P_e_BC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "02_P_n_AC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "02_P_n_BC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "03a_P_p_AC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "03a_P_p_BC_truth_pFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "03b_P_pFD_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "03b_P_pFD_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL Proton momentum AC", "CD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "03b_P_pCD_AC_truth_pFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
//                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL Proton momentum BC", "CD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "03b_P_pCD_BC_truth_pFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
//                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "04_P_piplus_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "04_P_piplus_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "05_P_piminus_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "05_P_piminus_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "06_P_pi0_AC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "06_P_pi0_BC_truth_pFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "07a_P_ph_AC_truth_pFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "07a_P_ph_BC_truth_pFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "07b_P_ph_AC_truth_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_pFDpCD_Directory"], "07b_P_ph_BC_truth_pFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (pFDpCD)">
    hPlot1D hTheta_e_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"], "01_Theta_e_AC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"], "01_Theta_e_BC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"], "02_Theta_n_AC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"], "02_Theta_n_BC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"], "03a_Theta_p_AC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"], "03a_Theta_p_BC_truth_pFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing FD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pFD_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing FD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pFD_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing CD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pCD_AC_truth_pFDpCD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing CD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "03b_Theta_pCD_BC_truth_pFDpCD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "04_Theta_piplus_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "04_Theta_piplus_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "05_Theta_piminus_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "05_Theta_piminus_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "06_Theta_pi0_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "06_Theta_pi0_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                "07a_Theta_ph_AC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                "07a_Theta_ph_BC_truth_pFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                   "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                   "07b_Theta_ph_AC_truth_pFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                   "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_pFDpCD_Directory"],
                                                   "07b_Theta_ph_BC_truth_pFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (pFDpCD)">
    hPlot1D hPhi_e_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "01_Phi_e_AC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "01_Phi_e_BC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "02_Phi_n_AC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "02_Phi_n_BC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "03a_Phi_p_AC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "03a_Phi_p_BC_truth_pFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of FD Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "03b_Phi_pFD_AC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of FD Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "03b_Phi_pFD_BC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_AC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of CD Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "03b_Phi_pCD_AC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_BC_truth_pFDpCD = hPlot1D("pFDpCD", "CD", "CD TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of CD Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "03b_Phi_pCD_BC_truth_pFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "04_Phi_piplus_AC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "04_Phi_piplus_BC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "05_Phi_piminus_AC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "05_Phi_piminus_BC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "06_Phi_pi0_AC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                               "06_Phi_pi0_BC_truth_pFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "07a_Phi_ph_AC_truth_pFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_pFDpCD = hPlot1D("pFDpCD", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"], "07a_Phi_ph_BC_truth_pFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                 "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "07b_Phi_ph_AC_truth_pFDpCD_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_pFDpCD_FD = hPlot1D("pFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                 "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_pFDpCD_Directory"],
                                                 "07b_Phi_ph_BC_truth_pFDpCD_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (pFDpCD)">
    hPlot2D hpFD_Hit_map_pFDpCD_BEC = hPlot2D("pFDpCD", "FD", "FD proton hit map BEC", "FD proton hit map BEC", "x_{pFD}", "y_{pFD}",
                                              directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                              "01a_Proton_FD_Hit_map_BEC_pFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hpFD_Hit_map_pFDpCD_AEC = hPlot2D("pFDpCD", "FD", "FD proton hit map AEC", "FD proton hit map AEC", "x_{pFD}", "y_{pFD}",
                                              directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                              "01b_Proton_FD_Hit_map_AEC_pFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_pFDpCD_BEC = hPlot2D("pFDpCD", "FD", "#theta_{pFD} vs. #phi_{pFD} BEC", "TL #theta_{pFD} vs. #phi_{pFD} BEC", "#phi_{pFD} [Deg]",
                                                       "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                       "02a_Theta_pFD_vs_Phi_pFD_BEC_pFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                       numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_pFD_vs_Phi_pFD_pFDpCD_AEC = hPlot2D("pFDpCD", "FD", "#theta_{pFD} vs. #phi_{pFD} AEC", "TL #theta_{pFD} vs. #phi_{pFD} AEC", "#phi_{pFD} [Deg]",
                                                       "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                       "02b_Theta_pFD_vs_Phi_pFD_AEC_pFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                       numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (pFDpCD)">
    hPlot2D hTheta_e_vs_Phi_e_truth_pFDpCD = hPlot2D("pFDpCD", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                     "01_Theta_e_vs_Phi_e_truth_pFDpCD",
                                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_pFD_vs_Phi_pFD_truth_pFDpCD = hPlot2D("pFDpCD", "FD", "TL #theta_{pFD} vs. #phi_{pFD}", "TL #theta_{pFD} vs. #phi_{pFD}", "#phi_{pFD} [Deg]",
                                                         "#theta_{pFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Proton_FD_Hit_map_pFDpCD_Directory"],
                                                         "02_Theta_pFD_vs_Phi_pFD_truth_pFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Efficiency plots (nFDpCD)">

    //<editor-fold desc="Truth level momentum plots (nFDpCD)">
    hPlot1D hP_e_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Electron momentum AC", "Electron momentum P^{truth}_{e} AC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "01_P_e_AC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_e_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Electron momentum BC", "Electron momentum P^{truth}_{e} BC", "P^{truth}_{e} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "01_P_e_BC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_n_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Neutron momentum AC", "Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02a_P_n_AC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_n_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Neutron momentum BC", "Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02a_P_n_BC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Neutron momentum AC", "FD Neutron momentum P^{truth}_{n} AC", "P^{truth}_{n} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02b_P_nFD_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_nFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Neutron momentum BC", "FD Neutron momentum P^{truth}_{n} BC", "P^{truth}_{n} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02b_P_nFD_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_p_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Proton momentum AC", "Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "03_P_p_AC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_p_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL Proton momentum BC", "Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                           directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "03_P_p_BC_truth_nFDpCD",
                                           Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Proton momentum AC", "FD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02b_P_pFD_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL Proton momentum BC", "FD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02b_P_pFD_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL Proton momentum AC", "CD Proton momentum P^{truth}_{p} AC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02b_P_pCD_AC_truth_nFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
//                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pCD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL Proton momentum BC", "CD Proton momentum P^{truth}_{p} BC", "P^{truth}_{p} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "02b_P_pCD_BC_truth_nFDpCD",
                                             CDMomentum_lboundary, CDMomentum_uboundary, numTH1Dbins_Mom_eff_Plots);
//                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pip_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{+} momentum AC", "#pi^{+} momentum P^{truth}_{#pi^{+}} AC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "04_P_piplus_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pip_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{+} momentum BC", "#pi^{+} momentum P^{truth}_{#pi^{+}} BC", "P^{truth}_{#pi^{+}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "04_P_piplus_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pim_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{-} momentum AC", "#pi^{-} momentum P^{truth}_{#pi^{-}} AC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "05_P_piminus_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pim_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{-} momentum BC", "#pi^{-} momentum P^{truth}_{#pi^{-}} BC", "P^{truth}_{#pi^{-}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "05_P_piminus_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_pi0_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{0} momentum AC", "#pi^{0} momentum P^{truth}_{#pi^{0}} AC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "06_P_pi0_AC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_pi0_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #pi^{0} momentum BC", "#pi^{0} momentum P^{truth}_{#pi^{0}} BC", "P^{truth}_{#pi^{0}} [GeV/c]",
                                             directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "06_P_pi0_BC_truth_nFDpCD",
                                             Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);

    hPlot1D hP_ph_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #gamma momentum AC", "#gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "07a_P_ph_AC_truth_nFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #gamma momentum BC", "#gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                            directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "07a_P_ph_BC_truth_nFDpCD",
                                            Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_AC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #gamma momentum AC", "FD #gamma momentum P^{truth}_{#gamma} AC", "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "07b_P_ph_AC_truth_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    hPlot1D hP_ph_BC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #gamma momentum BC", "FD #gamma momentum P^{truth}_{#gamma} BC", "P^{truth}_{#gamma} [GeV/c]",
                                               directories.Eff_and_ACorr_Directory_map["Mom_Eff_and_ACorr_nFDpCD_Directory"], "07b_P_ph_BC_truth_nFDpCD_FD",
                                               Momentum_lboundary, Momentum_uboundary, numTH1Dbins_Mom_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta plots (nFDpCD)">
    hPlot1D hTheta_e_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{e} AC", "#theta^{truth}_{e} of Outgoing Electron AC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"], "01_Theta_e_AC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_e_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{e} BC", "#theta^{truth}_{e} of Outgoing Electron BC", "#theta^{truth}_{e} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"], "01_Theta_e_BC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_n_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing Neutron AC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"], "02a_Theta_n_AC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_n_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing Neutron BC", "#theta^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"], "02a_Theta_n_BC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{n} AC", "#theta^{truth}_{n} of Outgoing FD Neutron AC",
                                                 "#theta^{truth}_{n} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_nFD_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_nFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{n} BC", "#theta^{truth}_{n} of Outgoing FD Neutron BC",
                                                 "#theta^{truth}_{n} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_nFD_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_p_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing Proton AC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"], "03_Theta_p_AC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_p_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing Proton BC", "#theta^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"], "03_Theta_p_BC_truth_nFDpCD",
                                               Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing FD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pFD_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing FD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pFD_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #theta^{truth}_{p} AC", "#theta^{truth}_{p} of Outgoing CD Proton AC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pCD_AC_truth_nFDpCD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pCD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #theta^{truth}_{p} BC", "#theta^{truth}_{p} of Outgoing CD Proton BC",
                                                 "#theta^{truth}_{p} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "02b_Theta_pCD_BC_truth_nFDpCD", Theta_lboundary_CD, Theta_uboundary_CD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pip_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{+}} AC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "04_Theta_piplus_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pip_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{+}} BC", "#theta^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                                 "#theta^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "04_Theta_piplus_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pim_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{-}} AC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "05_Theta_piminus_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pim_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{-}} BC", "#theta^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                                 "#theta^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "05_Theta_piminus_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_pi0_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{0}} AC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "06_Theta_pi0_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_pi0_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#pi^{0}} BC", "#theta^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                                 "#theta^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "06_Theta_pi0_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hTheta_ph_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of Outgoing #gamma AC",
                                                "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                "07a_Theta_ph_AC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of Outgoing #gamma BC",
                                                "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                "07a_Theta_ph_BC_truth_nFDpCD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_AC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} AC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                   "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                   "07b_Theta_ph_AC_truth_nFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hTheta_ph_BC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #theta^{truth}_{#gamma} BC", "#theta^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                   "#theta^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Theta_Eff_and_ACorr_nFDpCD_Directory"],
                                                   "07b_Theta_ph_BC_truth_nFDpCD_FD", Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level phi plots (nFDpCD)">
    hPlot1D hPhi_e_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{e} AC", "#phi^{truth}_{e} of Outgoing Electron AC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "01_Phi_e_AC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_e_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{e} BC", "#phi^{truth}_{e} of Outgoing Electron BC", "#phi^{truth}_{e} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "01_Phi_e_BC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_n_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing Neutron AC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02a_Phi_n_AC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_n_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing Neutron BC", "#phi^{truth}_{n} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02a_Phi_n_BC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{n} AC", "#phi^{truth}_{n} of Outgoing FD Neutron AC", "#phi^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02b_Phi_nFD_AC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_nFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{n} BC", "#phi^{truth}_{n} of Outgoing FD Neutron AC", "#phi^{truth}_{n} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02b_Phi_nFD_BC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_p_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{p} AC", "#phi^{truth}_{n} of Outgoing Proton AC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "03_Phi_p_AC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_p_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{p} BC", "#phi^{truth}_{n} of Outgoing Proton BC", "#phi^{truth}_{p} [Deg]",
                                             directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "03_Phi_p_BC_truth_nFDpCD",
                                             Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{p} AC", "#phi^{truth}_{p} of Outgoing FD Proton AC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02b_Phi_pFD_AC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pFD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{p} BC", "#phi^{truth}_{p} of Outgoing FD Proton BC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02b_Phi_pFD_BC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_AC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #phi^{truth}_{p} AC", "#phi^{truth}_{p} of Outgoing CD Proton AC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02b_Phi_pCD_AC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pCD_BC_truth_nFDpCD = hPlot1D("nFDpCD", "CD", "CD TL #phi^{truth}_{p} BC", "#phi^{truth}_{p} of Outgoing CD Proton BC", "#phi^{truth}_{p} [Deg]",
                                               directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "02b_Phi_pCD_BC_truth_nFDpCD",
                                               Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pip_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{+}} AC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} AC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "04_Phi_piplus_AC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pip_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{+}} BC", "#phi^{truth}_{#pi^{+}} of Outgoing #pi^{+} BC",
                                               "#phi^{truth}_{#pi^{+}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "04_Phi_piplus_BC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pim_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{-}} AC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} AC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "05_Phi_piminus_AC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pim_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{-}} BC", "#phi^{truth}_{#pi^{-}} of Outgoing #pi^{-} BC",
                                               "#phi^{truth}_{#pi^{-}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "05_Phi_piminus_BC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_pi0_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{0}} AC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} AC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "06_Phi_pi0_AC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_pi0_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#pi^{0}} BC", "#phi^{truth}_{#pi^{0}} of Outgoing #pi^{0} BC",
                                               "#phi^{truth}_{#pi^{0}} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                               "06_Phi_pi0_BC_truth_nFDpCD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);

    hPlot1D hPhi_ph_AC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of Outgoing #gamma AC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "07a_Phi_ph_AC_truth_nFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_nFDpCD = hPlot1D("nFDpCD", "", "TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of Outgoing #gamma BC",
                                              "#phi^{truth}_{#gamma} [Deg]",
                                              directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"], "07a_Phi_hp_BC_truth_nFDpCD",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_AC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} AC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma AC",
                                                 "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "07b_Phi_ph_AC_truth_nFDpCD_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    hPlot1D hPhi_ph_BC_truth_nFDpCD_FD = hPlot1D("nFDpCD", "FD", "FD TL #phi^{truth}_{#gamma} BC", "#phi^{truth}_{#gamma} of FD Outgoing #gamma BC",
                                                 "#phi^{truth}_{#gamma} [Deg]", directories.Eff_and_ACorr_Directory_map["Phi_Eff_and_ACorr_nFDpCD_Directory"],
                                                 "07b_Phi_hp_BC_truth_1n_FD", Phi_lboundary, Phi_uboundary, numTH1Dbins_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="TL fiducial cuts (nFDpCD)">
    hPlot2D hnFD_Hit_map_nFDpCD_BEC = hPlot2D("nFDpCD", "FD", "FD neutron hit map BEC", "FD neutron hit map BEC", "x_{nFD}", "y_{nFD}",
                                              directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                              "01a_Neutron_FD_Hit_map_BEC_nFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hnFD_Hit_map_nFDpCD_AEC = hPlot2D("nFDpCD", "FD", "FD neutron hit map AEC", "FD neutron hit map AEC", "x_{nFD}", "y_{nFD}",
                                              directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                              "01b_Neutron_FD_Hit_map_AEC_nFDpCD", -1.1, 1.1, -1.1, 1.1, numTH2Dbins_Nucleon_Ang_eff_Plots,
                                              numTH2Dbins_Nucleon_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_nFDpCD_BEC = hPlot2D("nFDpCD", "FD", "#theta_{nFD} vs. #phi_{nFD} BEC", "TL #theta_{nFD} vs. #phi_{nFD} BEC", "#phi_{nFD} [Deg]",
                                                       "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                       "02a_Theta_nFD_vs_Phi_nFD_BEC_nFDpCD", Phi_lboundary, Phi_uboundary,
                                                       Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    hPlot2D hTheta_nFD_vs_Phi_nFD_nFDpCD_AEC = hPlot2D("nFDpCD", "FD", "#theta_{nFD} vs. #phi_{nFD} AEC", "TL #theta_{nFD} vs. #phi_{nFD} AEC", "#phi_{nFD} [Deg]",
                                                       "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                       "02b_Theta_nFD_vs_Phi_nFD_AEC_nFDpCD", Phi_lboundary, Phi_uboundary,
                                                       Theta_lboundary_FD, Theta_uboundary_FD, numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //<editor-fold desc="Truth level theta vs. phi plots (nFDpCD)">
    hPlot2D hTheta_e_vs_Phi_e_truth_nFDpCD = hPlot2D("nFDpCD", "", "TL #theta_{e} vs. #phi_{e}", "TL #theta_{e} vs. #phi_{e}", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                                     directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                     "01_Theta_e_vs_Phi_e_truth_nFDpCD",
                                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                                     numTH2Dbins_Electron_Ang_eff_Plots, numTH2Dbins_Electron_Ang_eff_Plots);

    hPlot2D hTheta_nFD_vs_Phi_nFD_truth_nFDpCD = hPlot2D("nFDpCD", "FD", "TL #theta_{nFD} vs. #phi_{nFD}", "TL #theta_{nFD} vs. #phi_{nFD}", "#phi_{nFD} [Deg]",
                                                         "#theta_{nFD} [Deg]", directories.Eff_and_ACorr_Directory_map["Neutron_FD_Hit_map_nFDpCD_Directory"],
                                                         "02_Theta_nFD_vs_Phi_nFD_truth_nFDpCD", Phi_lboundary, Phi_uboundary, Theta_lboundary_FD,
                                                         Theta_uboundary_FD,
                                                         numTH2Dbins_Nucleon_Ang_eff_Plots, numTH2Dbins_Nucleon_Ang_eff_Plots);
    //</editor-fold>

    //</editor-fold>

    //</editor-fold>

// ======================================================================================================================================================================
// Acceptance maps histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Acceptance maps histograms">
    /* Acceptance maps are handled completely by the AMaps class */
    hPlot2D hElectronAMapBC = hPlot2D("", "", "Electron_AMap_BC", "Electron AMap BC", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                      directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "01_Electron_AMap_BC",
                                      Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                      numTH2Dbins_Electron_AMaps_Plots, numTH2Dbins_Electron_AMaps_Plots);
    hPlot2D hProtonAMapBC = hPlot2D("", "", "Proton_AMap_BC", "Proton AMap BC", "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                    directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "02_Proton_AMap_BC",
                                    Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                    numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNeutronAMapBC = hPlot2D("", "", "Neutron_AMap_BC", "Neutron AMap BC", "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                     directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "03_Neutron_AMap_BC",
                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                     numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNucleonAMapBC = hPlot2D("", "", "Nucleon_AMap_BC", "Nucleon AMap BC", "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]",
                                     directories.AMaps_Directory_map["AMaps_BC_1e_cut_Directory"], "04_Nucleon_AMap_BC",
                                     Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                     numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hPlot1D hReco_P_e_AMaps = hPlot1D("1e cut", "FD", "Reco P_{e} used in AMaps", "Reco P_{e} used in AMaps", "P_{e} [GeV/c]",
                                      directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "01a_Reco_P_e_used_in_AMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_e_AMaps = hPlot1D("1e cut", "FD", "TL P_{e} used in AMaps", "TL P_{e} used in AMaps", "P_{e} [GeV/c]",
                                    directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "01b_TL_P_e_used_in_AMaps",
                                    Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_pFD_AMaps = hPlot1D("1e cut", "FD", "Reco P_{pFD} used in AMaps", "Reco P_{pFD} used in AMaps", "P_{pFD} [GeV/c]",
                                        directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "02a_Reco_P_pFD_used_in_AMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_pFD_AMaps = hPlot1D("1e cut", "FD", "TL P_{pFD} used in AMaps", "TL P_{pFD} used in AMaps", "P_{pFD} [GeV/c]",
                                      directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "02b_TL_P_pFD_used_in_AMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_nFD_AMaps = hPlot1D("1e cut", "FD", "Reco P_{nFD} used in AMaps", "Reco P_{nFD} used in AMaps", "P_{nFD} [GeV/c]",
                                        directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "03a_Reco_P_nFD_used_in_AMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_nFD_AMaps = hPlot1D("1e cut", "FD", "TL P_{nFD} used in AMaps", "TL P_{nFD} used in AMaps", "P_{nFD} [GeV/c]",
                                      directories.AMaps_Directory_map["AMaps_1e_cut_Directory"], "03b_TL_P_nFD_used_in_AMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);

    hPlot2D hElectronAMapBCwKC = hPlot2D("", "", "Electron_AMap_BCwKC", "Electron AMap BC wKC", "#phi_{e} [Deg]", "#theta_{e} [Deg]",
                                         directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "01_Electron_AMap_BCwKC",
                                         Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                         numTH2Dbins_Electron_AMaps_Plots, numTH2Dbins_Electron_AMaps_Plots);
    hPlot2D hProtonAMapBCwKC = hPlot2D("", "", "Proton_AMap_BCwKC", "Proton AMap BC wKC", "#phi_{p} [Deg]", "#theta_{p} [Deg]",
                                       directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "02_Proton_AMap_BCwKC",
                                       Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                       numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNeutronAMapBCwKC = hPlot2D("", "", "Neutron_AMap_BCwKC", "Neutron AMap BC wKC", "#phi_{n} [Deg]", "#theta_{n} [Deg]",
                                        directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "03_Neutron_AMap_BCwKC",
                                        Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                        numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);
    hPlot2D hNucleonAMapBCwKC = hPlot2D("", "", "Nucleon_AMap_BCwKC", "Nucleon AMap BC wKC", "#phi_{nuc} [Deg]", "#theta_{nuc} [Deg]",
                                        directories.AMaps_Directory_map["WMaps_BC_1e_cut_Directory"], "04_Nucleon_AMap_BCwKC",
                                        Phi_lboundary, Phi_uboundary, Theta_lboundary_FD, Theta_uboundary_FD,
                                        numTH2Dbins_Nucleon_AMaps_Plots, numTH2Dbins_Nucleon_AMaps_Plots);

    hPlot1D hReco_P_e_WMaps = hPlot1D("1e cut", "FD", "Reco P_{e} used in WMaps", "Reco P_{e} used in WMaps", "P_{e} [GeV/c]",
                                      directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "01a_Reco_P_e_used_in_WMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_e_WMaps = hPlot1D("1e cut", "FD", "TL P_{e} used in WMaps", "TL P_{e} used in WMaps", "P_{e} [GeV/c]",
                                    directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "01b_TL_P_e_used_in_WMaps",
                                    Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_pFD_WMaps = hPlot1D("1e cut", "FD", "Reco P_{pFD} used in WMaps", "Reco P_{pFD} used in WMaps", "P_{pFD} [GeV/c]",
                                        directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "02a_Reco_P_pFD_used_in_WMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_pFD_WMaps = hPlot1D("1e cut", "FD", "TL P_{pFD} used in WMaps", "TL P_{pFD} used in WMaps", "P_{pFD} [GeV/c]",
                                      directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "02b_TL_P_pFD_used_in_WMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hReco_P_nFD_WMaps = hPlot1D("1e cut", "FD", "Reco P_{nFD} used in WMaps", "Reco P_{nFD} used in WMaps", "P_{nFD} [GeV/c]",
                                        directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "03a_Reco_P_nFD_used_in_WMaps",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_nFD_WMaps = hPlot1D("1e cut", "FD", "TL P_{nFD} used in WMaps", "TL P_{nFD} used in WMaps", "P_{nFD} [GeV/c]",
                                      directories.AMaps_Directory_map["WMaps_1e_cut_Directory"], "03b_TL_P_nFD_used_in_WMaps",
                                      Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

// ======================================================================================================================================================================
// Resolution histograms
// ======================================================================================================================================================================

    //<editor-fold desc="Resolution histograms">

    //<editor-fold desc="Resolution histograms (1p)">
    hPlot1D hdTheta_pFD_TL_BC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} BC", "#Delta#theta_{pFD} of FD proton BC",
                                           "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1p_Directory"], "00a_DeltaTheta_pFD_BC_TL_1p",
                                           -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_pFD_TL_ZOOMIN_BC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} - ZOOMIN BC", "#Delta#theta_{pFD} of FD proton - ZOOMIN BC",
                                                  "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1p_Directory"], "00b_DeltaTheta_pFD_BC_TL_ZOOMIN_1p",
                                                  -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_BC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} BC", "#Delta#phi_{pFD} of FD proton BC",
                                         "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                         directories.Resolution_Directory_map["Resolution_1p_Directory"], "00c_DeltaPhi_pFD_BC_TL_1p",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_ZOOMIN_BC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} - ZOOMIN BC", "#Delta#phi_{pFD} of FD proton - ZOOMIN BC",
                                                "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1p_Directory"], "00d_DeltaPhi_pFD_BC_TL_ZOOMIN_1p",
                                                -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hdTheta_pFD_TL_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} AdPC", "#Delta#theta_{pFD} of FD proton AdPC",
                                             "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                             directories.Resolution_Directory_map["Resolution_1p_Directory"], "00e_DeltaTheta_pFD_AdPC_TL_1p",
                                             -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_pFD_TL_ZOOMIN_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#theta_{pFD} - ZOOMIN AdPC", "#Delta#theta_{pFD} of FD proton - ZOOMIN AdPC",
                                                    "#Delta#theta_{pFD} = #theta^{rec}_{pFD} - #theta^{truth}_{pFD} [Deg]",
                                                    directories.Resolution_Directory_map["Resolution_1p_Directory"], "00f_DeltaTheta_pFD_AdPC_TL_ZOOMIN_1p",
                                                    -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} AdPC", "#Delta#phi_{pFD} of FD proton AdPC",
                                           "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1p_Directory"], "00g_DeltaPhi_pFD_AdPC_TL_1p",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_pFD_TL_ZOOMIN_AdPC_1p = hPlot1D("1p", "FD", "TL #Delta#phi_{pFD} - ZOOMIN AdPC", "#Delta#phi_{pFD} of FD proton - ZOOMIN AdPC",
                                                  "#Delta#phi_{pFD} = #phi^{rec}_{pFD} - #phi^{truth}_{pFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1p_Directory"], "00h_DeltaPhi_pFD_AdPC_TL_ZOOMIN_1p",
                                                  -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hTheta_pFD_TL_ApResC_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} ApResC", "#theta^{truth}_{pFD} of FD proton ApResC", "#theta^{truth}_{pFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1p_Directory"], "01a_Theta_pFD_ApResC_TL_1p",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_pFD_TL_ApResC_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} ApResC", "#phi^{truth}_{pFD} of FD proton ApResC", "#phi^{truth}_{pFD} [Deg]",
                                            directories.Resolution_Directory_map["Resolution_1p_Directory"], "02a_Phi_pFD_ApResC_TL_1p",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p = new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} ApResC (1p, FD)",
                                                           "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} ApResC (1p, FD);#phi^{truth}_{pFD} [Deg];#theta^{truth}_{pFD} [Deg]",
                                                           numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                           numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_pFD_TL_VS_Phi_pFD_TL_ApResC_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hTheta_pFD_TL_MatchedP_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} of matched TL p", "#theta^{truth}_{pFD} of matched TL FD proton",
                                                "#theta^{truth}_{pFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1p_Directory"], "01b_Theta_pFD_MatchedN_TL_1p",
                                                Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_pFD_TL_MatchedP_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} of matched TL p", "#phi^{truth}_{pFD} of matched TL FD proton",
                                              "#phi^{truth}_{pFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1p_Directory"], "02b_Phi_pFD_MatchedN_TL_1p",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p = new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} of matched TL p (1p, FD)",
                                                             "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} of matched TL FD proton (1p, FD);#phi^{truth}_{pFD} [Deg];#theta^{truth}_{pFD} [Deg]",
                                                             numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                             numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_pFD_TL_VS_Phi_pFD_TL_MatchedP_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hTheta_pFD_TL_1p = hPlot1D("1p", "", "TL #theta^{truth}_{pFD} AC", "#theta^{truth}_{pFD} of FD proton AC", "#theta^{truth}_{pFD} [Deg]",
                                       directories.Resolution_Directory_map["Resolution_1p_Directory"], "01_Theta_pFD_AC_TL_1p",
                                       Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_pFD_TL_1p = hPlot1D("1p", "", "TL #phi^{truth}_{pFD} AC", "#phi^{truth}_{pFD} of FD proton AC", "#phi^{truth}_{pFD} [Deg]",
                                     directories.Resolution_Directory_map["Resolution_1p_Directory"], "02_Phi_pFD_AC_TL_1p",
                                     Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_pFD_TL_VS_Phi_pFD_TL_1p = new TH2D("#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} (1p, FD)",
                                                    "#theta^{truth}_{pFD} vs. #phi^{truth}_{pFD} (1p, FD);#phi^{truth}_{pFD} [Deg];#theta^{truth}_{pFD} [Deg]",
                                                    numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary, numTH2Dbins_nRes_Plots, Theta_lboundary_FD,
                                                    Theta_uboundary_FD);
    string hTheta_pFD_TL_VS_Phi_pFD_TL_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hP_pFD_Res_1p = hPlot1D("1p", "", "P_{pFD} resolution AC", "FD proton P_{pFD} resolution AC",
                                    "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}",
                                    directories.Resolution_Directory_map["Resolution_1p_Directory"], "04_P_pFD_Res_1p",
                                    -2, 2, numTH1Dbins_nRes_Plots);
    TH2D *hP_pFD_Res_VS_P_pFD_1p = new TH2D("P_{pFD} resolution AC vs. P^{truth}_{pFD} (1p, FD)",
                                            "P_{pFD} resolution AC vs. P^{truth}_{pFD} (1p, FD);P^{truth}_{pFD} [GeV/c];"
                                            "Resolution = (P^{truth}_{pFD} - P^{reco.}_{pFD})/P^{truth}_{pFD}",
                                            numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -2, 2);
    string hP_pFD_Res_VS_P_pFD_1p_Dir = directories.Resolution_Directory_map["Resolution_1p_Directory"];

    hPlot1D hReco_P_pFD_pRes_1p = hPlot1D("1p", "FD", "Reco P_{pFD} used in pRes", "Reco P_{pFD} used in pRes", "P_{pFD} [GeV/c]",
                                          directories.Resolution_Directory_map["Resolution_1p_Directory"], "06a_Reco_P_pFD_used_in_pRes_1p",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_pFD_pRes_1p = hPlot1D("1p", "FD", "TL P_{pFD} used in pRes", "TL P_{pFD} used in pRes", "P_{pFD} [GeV/c]",
                                        directories.Resolution_Directory_map["Resolution_1p_Directory"], "06b_TL_P_pFD_used_in_pRes_1p",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //<editor-fold desc="Resolution histograms (1n)">
    hPlot1D hdTheta_nFD_TL_BC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} BC", "#Delta#theta_{nFD} of FD neutron BC",
                                           "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1n_Directory"], "00a_DeltaTheta_nFD_BC_TL_1n",
                                           -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_nFD_TL_ZOOMIN_BC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} - ZOOMIN BC", "#Delta#theta_{nFD} of FD neutron - ZOOMIN BC",
                                                  "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1n_Directory"], "00b_DeltaTheta_nFD_BC_TL_ZOOMIN_1n",
                                                  -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_BC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} BC", "#Delta#phi_{nFD} of FD neutron BC",
                                         "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                         directories.Resolution_Directory_map["Resolution_1n_Directory"], "00c_DeltaPhi_nFD_BC_TL_1n",
                                         Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_ZOOMIN_BC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} - ZOOMIN BC", "#Delta#phi_{nFD} of FD neutron - ZOOMIN BC",
                                                "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1n_Directory"], "00d_DeltaPhi_nFD_BC_TL_ZOOMIN_1n",
                                                -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hdTheta_nFD_TL_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} AdPC", "#Delta#theta_{nFD} of FD neutron AdPC",
                                             "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                             directories.Resolution_Directory_map["Resolution_1n_Directory"], "00e_DeltaTheta_nFD_AdPC_TL_1n",
                                             -Theta_uboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hdTheta_nFD_TL_ZOOMIN_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#theta_{nFD} - ZOOMIN AdPC", "#Delta#theta_{nFD} of FD neutron - ZOOMIN AdPC",
                                                    "#Delta#theta_{nFD} = #theta^{rec}_{nFD} - #theta^{truth}_{nFD} [Deg]",
                                                    directories.Resolution_Directory_map["Resolution_1n_Directory"], "00f_DeltaTheta_nFD_AdPC_TL_ZOOMIN_1n",
                                                    -10.0, 10.0, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} AdPC", "#Delta#phi_{nFD} of FD neutron AdPC",
                                           "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                           directories.Resolution_Directory_map["Resolution_1n_Directory"], "00g_DeltaPhi_nFD_AdPC_TL_1n",
                                           Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    hPlot1D hdPhi_nFD_TL_ZOOMIN_AdPC_1n = hPlot1D("1n", "FD", "TL #Delta#phi_{nFD} - ZOOMIN AdPC", "#Delta#phi_{nFD} of FD neutron - ZOOMIN AdPC",
                                                  "#Delta#phi_{nFD} = #phi^{rec}_{nFD} - #phi^{truth}_{nFD} [Deg]",
                                                  directories.Resolution_Directory_map["Resolution_1n_Directory"], "00h_DeltaPhi_nFD_AdPC_TL_ZOOMIN_1n",
                                                  -20, 20, numTH1Dbins_nRes_Plots);

    hPlot1D hTheta_nFD_TL_AnResC_1n = hPlot1D("1n", "", "TL #theta^{truth}_{nFD} AnResC", "#theta^{truth}_{nFD} of FD neutron AnResC", "#theta^{truth}_{nFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1n_Directory"], "01a_Theta_nFD_AnResC_TL_1n",
                                              Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_nFD_TL_AnResC_1n = hPlot1D("1n", "", "TL #phi^{truth}_{nFD} AnResC", "#phi^{truth}_{nFD} of FD neutron AnResC", "#phi^{truth}_{nFD} [Deg]",
                                            directories.Resolution_Directory_map["Resolution_1n_Directory"], "02a_Phi_nFD_AnResC_TL_1n",
                                            Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n = new TH2D("#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} AnResC (1n, FD)",
                                                           "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} AnResC (1n, FD);#phi^{truth}_{nFD} [Deg];#theta^{truth}_{nFD} [Deg]",
                                                           numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                           numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_nFD_TL_VS_Phi_nFD_TL_AnResC_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];

    hPlot1D hTheta_nFD_TL_MatchedN_1n = hPlot1D("1n", "", "TL #theta^{truth}_{nFD} of matched TL n", "#theta^{truth}_{nFD} of matched TL FD neutron",
                                                "#theta^{truth}_{nFD} [Deg]",
                                                directories.Resolution_Directory_map["Resolution_1n_Directory"], "01b_Theta_nFD_MatchedN_TL_1n",
                                                Theta_lboundary_FD, Theta_uboundary_FD, numTH1Dbins_nRes_Plots);
    hPlot1D hPhi_nFD_TL_MatchedN_1n = hPlot1D("1n", "", "TL #phi^{truth}_{nFD} of matched TL n", "#phi^{truth}_{nFD} of matched TL FD neutron",
                                              "#phi^{truth}_{nFD} [Deg]",
                                              directories.Resolution_Directory_map["Resolution_1n_Directory"], "02b_Phi_nFD_MatchedN_TL_1n",
                                              Phi_lboundary, Phi_uboundary, numTH1Dbins_nRes_Plots);
    TH2D *hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n = new TH2D("#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} of matched TL n (1n, FD)",
                                                             "#theta^{truth}_{nFD} vs. #phi^{truth}_{nFD} of matched TL FD neutron (1n, FD);#phi^{truth}_{nFD} [Deg];#theta^{truth}_{nFD} [Deg]",
                                                             numTH2Dbins_nRes_Plots, Phi_lboundary, Phi_uboundary,
                                                             numTH2Dbins_nRes_Plots, Theta_lboundary_FD, Theta_uboundary_FD);
    string hTheta_nFD_TL_VS_Phi_nFD_TL_MatchedN_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];

    hPlot1D hP_nFD_Res_1n = hPlot1D("1n", "", "P_{nFD} resolution AC", "FD neutron P_{nFD} resolution AC",
                                    "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                    directories.Resolution_Directory_map["Resolution_1n_Directory"], "04_P_nFD_Res_1n",
                                    -2, 2, numTH1Dbins_nRes_Plots);
    TH2D *hP_nFD_Res_VS_P_nFD_1n = new TH2D("P_{nFD} resolution AC vs. P^{truth}_{nFD} (1n, FD)",
                                            "P_{nFD} resolution AC vs. P^{truth}_{nFD} (1n, FD);P^{truth}_{nFD} [GeV/c];"
                                            "Resolution = (P^{truth}_{nFD} - P^{reco.}_{nFD})/P^{truth}_{nFD}",
                                            numTH2Dbins_nRes_Plots, 0, beamE * 1.1, numTH2Dbins_nRes_Plots, -2, 2);
    string hP_nFD_Res_VS_P_nFD_1n_Dir = directories.Resolution_Directory_map["Resolution_1n_Directory"];

    hPlot1D hReco_P_nFD_nRes_1n = hPlot1D("1n", "FD", "Reco P_{nFD} used in nRes", "Reco P_{nFD} used in nRes", "P_{nFD} [GeV/c]",
                                          directories.Resolution_Directory_map["Resolution_1n_Directory"], "06a_Reco_P_nFD_used_in_nRes_1n",
                                          Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    hPlot1D hTL_P_nFD_nRes_1n = hPlot1D("1n", "FD", "TL P_{nFD} used in nRes", "TL P_{nFD} used in nRes", "P_{nFD} [GeV/c]",
                                        directories.Resolution_Directory_map["Resolution_1n_Directory"], "06b_TL_P_nFD_used_in_nRes_1n",
                                        Momentum_lboundary, Momentum_uboundary, numTH1Dbins);
    //</editor-fold>

    //</editor-fold>

    cout << " done.\n\n";
    //</editor-fold>

    //<editor-fold desc="Histogram setup">
    /* Histogram boundary variables. Used to unify histograms to the same boundaries. */

    //<editor-fold desc="Number of histogram bins">
    /* Default */
    int numTH1Dbins = 50;
    int numTH2Dbins = 65;

    /* Momentum plots */
    int numTH2Dbins_Mom_Plots = 65; // To be changed if apply_kinematical_cuts = true

    /* Beta plots */
    int numTH1Dbins_Beta_Plots = 65;
    int numTH2Dbins_Beta_Plots = 65;

    /* W plots */
    int numTH1Dbins_W_Plots = 50;
    int numTH2Dbins_W_Plots = 65;

    /* Angle plots */
    int numTH1Dbins_Ang_Plots = 50;
    int numTH2Dbins_Ang_Plots = 65;
    int numTH2Dbins_Electron_Ang_Plots = 100;
    int numTH2Dbins_Nucleon_Ang_Plots = 75;

    /* Momentum transfer plots */
    int numTH1Dbins_Momentum_transfer_plots = 50;
    int numTH2Dbins_Momentum_transfer_plots = 65;

    /* E_e plots */
    int numTH1Dbins_E_e_Plots = 50;
    int numTH2Dbins_E_e_Plots = 65;

    /* ET plots */
    int numTH1Dbins_ET_Plots = 50;

    /* E_cal plots */
    int numTH1Dbins_E_cal_Plots = 50;
    int numTH2Dbins_E_cal_Plots = 65;

    /* TKI plots */
    int numTH1Dbins_TKI_dP_T_Plots = 50;
    int numTH1Dbins_TKI_dAlpha_T_Plots = 30;
    int numTH2Dbins_TKI_Plots = 65;

    /* Efficiency % acceptance correction plots */
    int numTH1Dbins_Mom_eff_Plots = numTH1Dbins;
    int numTH1Dbins_Ang_eff_Plots = numTH1Dbins_Ang_Plots;
    int numTH2Dbins_Ang_eff_Plots = numTH2Dbins_Ang_Plots;
    int numTH2Dbins_Electron_Ang_eff_Plots = numTH2Dbins_Electron_Ang_Plots;
    int numTH2Dbins_Nucleon_Ang_eff_Plots = numTH2Dbins_Nucleon_Ang_Plots;

    /* Acceptance maps plots */
    int numTH1Dbins_AMaps_Plots = numTH1Dbins_Ang_Plots;
    int numTH2Dbins_AMaps_Plots = numTH2Dbins_Ang_Plots;
    int numTH2Dbins_Electron_AMaps_Plots = numTH2Dbins_Electron_Ang_Plots;
    int numTH2Dbins_Nucleon_AMaps_Plots = numTH2Dbins_Nucleon_Ang_Plots;

    /* Neutron resolution plots */
    int numTH1Dbins_nRes_Plots = numTH1Dbins_Ang_Plots;
    int numTH2Dbins_nRes_Plots = numTH2Dbins_Nucleon_Ang_Plots;
    //</editor-fold>

    //<editor-fold desc="Histogram limits">
    /* Nphe boundries */
    const double Nphe_boundary = 40;

    /* Chi2 boundries */
    double Chi2_boundary = 20;
//    double Chi2_boundary = 30;

    if (apply_cuts) { Chi2_boundary = 9; }

    /* Vertex boundries */
    double Vertex_boundary = 20., Vertex_uboundary = Vertex_boundary, Vertex_lboundary = -Vertex_boundary;
    double Vertex_boundary_FD = Vertex_boundary, Vertex_uboundary_FD = Vertex_uboundary, Vertex_lboundary_FD = Vertex_lboundary;
    double Vertex_boundary_CD = Vertex_boundary, Vertex_uboundary_CD = Vertex_uboundary, Vertex_lboundary_CD = Vertex_lboundary;
    double dV_boundary = 8., dV_uboundary = dV_boundary, dV_lboundary = -dV_boundary;
    double dV_boundary_FD = dV_boundary, dV_uboundary_FD = dV_uboundary, dV_lboundary_FD = dV_lboundary;
    double dV_boundary_CD = dV_boundary, dV_uboundary_CD = dV_uboundary, dV_lboundary_CD = dV_lboundary;

    if (apply_cuts) {
        double dVertex_boundary = Vz_cut.GetUpperCut() - Vz_cut.GetLowerCut();
        double dVertex_boundary_FD = Vz_cut_FD.GetUpperCut() - Vz_cut_FD.GetLowerCut(), dVertex_boundary_CD = Vz_cut_CD.GetUpperCut() - Vz_cut_CD.GetLowerCut();
        double ddV_boundary = dVz_cuts.GetUpperCut() - dVz_cuts.GetLowerCut();
        double ddV_boundary_FD = dVz_cuts_FD.GetUpperCut() - dVz_cuts_FD.GetLowerCut(), ddV_boundary_CD = dVz_cuts_CD.GetUpperCut() - dVz_cuts_CD.GetLowerCut();

        Vertex_uboundary = Vz_cut.GetUpperCut() + 0.2 * dVertex_boundary, Vertex_lboundary = Vz_cut.GetLowerCut() - 0.1 * dVertex_boundary;
        Vertex_uboundary_FD = Vz_cut_FD.GetUpperCut() + 0.2 * dVertex_boundary_FD, Vertex_lboundary_FD = Vz_cut_FD.GetLowerCut() - 0.1 * dVertex_boundary_FD;
        Vertex_uboundary_CD = Vz_cut_CD.GetUpperCut() + 0.2 * dVertex_boundary_CD, Vertex_lboundary_CD = Vz_cut_CD.GetLowerCut() - 0.1 * dVertex_boundary_CD;
        dV_uboundary = dVz_cuts.GetUpperCut() + 0.2 * ddV_boundary, dV_lboundary = dVz_cuts.GetLowerCut() - 0.1 * ddV_boundary;
        dV_uboundary_FD = dVz_cuts_FD.GetUpperCut() + 0.2 * ddV_boundary_FD, dV_lboundary_FD = dVz_cuts_FD.GetLowerCut() - 0.1 * ddV_boundary_FD;
        dV_uboundary_CD = dVz_cuts_CD.GetUpperCut() + 0.2 * ddV_boundary_CD, dV_lboundary_CD = dVz_cuts_CD.GetLowerCut() - 0.1 * ddV_boundary_CD;

        Vertex_boundary = 8., Vertex_boundary_FD = 8., Vertex_boundary_CD = 8.;
        dV_boundary = 8., dV_boundary_FD = 8., dV_boundary_CD = 8.;
    }

    /* SF boundries */
    const double SF_uboundary = 0.31, SF_lboundary = 0.16;

    /* Momentum boundries */
    const double Momentum_lboundary = 0., Momentum_uboundary = beamE * 1.1;   // Default
    const double CDMomentum_lboundary = 0., CDMomentum_uboundary = beamE / 2; // CD nucleons (pFDpCD & nFDpCD)
    double P_nucFD_lboundary = 0., P_nucFD_uboundary = beamE * 1.1;           // Default
    double P_nucCD_lboundary = 0., P_nucCD_uboundary = beamE / 2;             // CD nucleons (pFDpCD & nFDpCD)

    if (apply_kinematical_cuts) {
        P_nucFD_lboundary = FD_nucleon_momentum_cut.GetLowerCut(), P_nucFD_uboundary = FD_nucleon_momentum_cut.GetUpperCut() * 1.1;
        P_nucCD_lboundary = 0.4, P_nucCD_uboundary = 2.5; // CD nucleons (pFDpCD & nFDpCD)
    }

    /* W boundries */
    const double W_lboundary = 0.5, W_uboundary = (beamE * 1.1) / 2; // Default

    /* Beta boundries */
    const double dBeta_sigma_boundary = 0.1;
    const double Beta_dist_uboundary = 1 + dBeta_sigma_boundary, Beta_dist_lboundary = 1 - dBeta_sigma_boundary;

    const double dBeta_sigma_ZOOMOUT_boundary = 0.1;
    const double Beta_dist_ZOOMOUT_uboundary = 1 + dBeta_sigma_ZOOMOUT_boundary;
    const double Beta_dist_ZOOMOUT_lboundary = 0.9;

    double Beta_boundary = 3., P_boundary = beamE * 1.425;

    if (apply_cuts) {
        Beta_boundary = 1.25;
        P_boundary = beamE * 1.1;
    }

    /* Angle boundries */
    const double Theta_lboundary_FD = 0., Theta_uboundary_FD = 50.;
    const double Theta_lboundary_CD = 30., Theta_uboundary_CD = 155.;
    const double Opening_Ang_wide_lboundary = 0, Opening_Ang_wide_uboundary = 180.;
    const double Opening_Ang_narrow_lboundary = 0, Opening_Ang_narrow_uboundary = 100.;
    const double Phi_lboundary = -180., Phi_uboundary = 180.;

    /* TKI boundries */
    const double dP_T_boundary = 3.;
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Histogram functions">

    // Constructor -------------------------------------------------------------------------------------------------------------------------------------------------
    HistogramsTemp(const Directories &directories, const bool apply_cuts, const bool apply_kinematical_cuts);
    //</editor-fold>

};


#endif //PROJECT_HISTOGRAMSTEMP_H
