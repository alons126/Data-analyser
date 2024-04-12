//
// Created by alons on 09/04/2023.
//

#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <map>

using namespace std;

class Directories {
private:
    string Plots_Folder;

    bool ClearOldDirectories = true; // Clear old directories by default

    //<editor-fold desc="Cut parameters plots directories">

    //<editor-fold desc="Number of Photo-electrons (Nphe) plots directories">
    bool create_Nphe_Dir = true;
    string Nphe_Parent_Directory = "01_Cuts_plots/01_Nphe_plots";
    vector <string> Nphe_Daughter_Folders = {"",
                                             "01_1e_cut",
                                             "02_1p",
                                             "03_1n",
                                             "04_2p",
                                             "05_pFDpCD",
                                             "06_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="Chi2 plots directories">
    bool create_chi2_Dir = true;
    string Chi2_Parent_Directory = "01_Cuts_plots/02_Chi2_plots";
    vector <string> Chi2_Daughter_Folders = {"",
                                             "00_All_e",
                                             "01_1e_cut",
                                             "02_1p",
                                             "03_1n",
//                                                 "04_1e2p_BC-AC_cut_tests",
                                             "04_2p",
                                             "05_pFDpCD",
                                             "06_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="Vertex plots directories">
    bool create_Vertex_Dir = true;
    string Vertex_Parent_Directory = "01_Cuts_plots/03_Vertex_plots";
    vector <string> Vertex_Daughter_Folders = {"",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/01_Electrons",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/02_Protons",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/03_Kplus",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/04_Kminus",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/05_Piplus",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/06_Piminus",
                                               "01_1e_cut/01_cParticles_Vtx_comp_plots/07_Deuteron",
                                               "01_1e_cut/02_cParticles_dVtx_plots/01_Protons",
                                               "01_1e_cut/02_cParticles_dVtx_plots/02_Kplus",
                                               "01_1e_cut/02_cParticles_dVtx_plots/03_Kminus",
                                               "01_1e_cut/02_cParticles_dVtx_plots/04_Piplus",
                                               "01_1e_cut/02_cParticles_dVtx_plots/05_Piminus",
                                               "01_1e_cut/02_cParticles_dVtx_plots/06_Deuteron",

                                               "02_1p/01_dV_plots",
                                               "03_1n/01_dV_plots",
                                               "04_2p/01_dV_plots",
                                               "05_pFDpCD/01_dV_plots",
                                               "06_nFDpCD/01_dV_plots"
    };
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) plots directories">
    bool create_SF_Dir = true;
    string SF_Parent_Directory = "01_Cuts_plots/04_SF_plots";
    vector <string> SF_Daughter_Folders = {"",
//                                               "01_1e_cut",
                                           "01_1e_cut/01_SF_plots", "01_1e_cut/02_SF_VS_P_e_plots",

//                                               "02_1p",
                                           "02_1p/01_SF_plots", "02_1p/02_SF_VS_P_e_plots",

//                                               "03_1n",
                                           "03_1n/01_SF_plots", "03_1n/02_SF_VS_P_e_plots",

//                                               "04_2p",
                                           "04_2p/01_SF_plots", "04_2p/02_SF_VS_P_e_plots",

//                                               "05_pFDpCD",
                                           "05_pFDpCD/01_SF_plots", "05_pFDpCD/02_SF_VS_P_e_plots",

//                                               "06_nFDpCD",
                                           "06_nFDpCD/01_SF_plots", "06_nFDpCD/02_SF_VS_P_e_plots",
    };
    //</editor-fold>

    //<editor-fold desc="Edge cuts histograms plots directories">
    bool create_fiducial_Dir = true;
    string fiducial_Parent_Directory = "01_Cuts_plots/05_Edge_cuts_plots";
    vector <string> fiducial_Daughter_Folders = {"",
//                                                     "01_1e_cut",
                                                 "01_1e_cut/01_PCAL",

//                                                     "02_1p",
                                                 "02_1p/01_PCAL",

//                                                     "03_1n",
                                                 "03_1n/01_PCAL",

//                                                     "04_2p",
                                                 "04_2p/01_PCAL",

//                                                     "05_pFDpCD",
                                                 "05_pFDpCD/01_PCAL",

//                                                     "06_nFDpCD",
                                                 "06_nFDpCD/01_PCAL"
    };
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots directories">
    bool create_Momentum_Dir = true;
    string Momentum_Parent_Directory = "01_Cuts_plots/06_Momentum_plots";
    vector <string> Momentum_Daughter_Folders = {"",
                                                 "01_1e_cut", "01_1e_cut/01_Momentum_th_plots_1e_cut",
                                                 "02_1p", "02_1p/Analysis_plots_1p",
                                                 "03_1n", "03_1n/Analysis_plots_1n",
                                                 "04_2p",
                                                 "05_pFDpCD", "05_pFDpCD/Analysis_plots_pFDpCD",
                                                 "06_nFDpCD", "06_nFDpCD/Analysis_plots_nFDpCD"
    };
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="W plots directories">
    bool create_W_Dir = true;
    string W_Parent_Directory = "02_W_plots";
    vector <string> W_Daughter_Folders = {"",
                                          "01_1e_cut",
                                          "02_1p",
                                          "03_1n",
                                          "04_2p",
                                          "05_pFDpCD",
                                          "06_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="Beta plots directories">
    bool create_Beta_Dir = true;
    string Beta_Parent_Directory = "03_Beta_plots";
    vector <string> Beta_Daughter_Folders = {"",
                                             "01_Beta_fit_plots",
                                             "01_Beta_fit_plots/03_1n",
                                             "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n",
                                             "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/01_fPhotons_All_plots_1n",
                                             "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/02_fPhotons_noPCAL_hit_plots_1n",
                                             "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/03_fPhotons_noPCAL_yesECIN_plots_1n",
                                             "01_Beta_fit_plots/03_1n/01_Neutron_beta_from_fPhotons_plots_1n/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_1n",
                                             "01_Beta_fit_plots/03_1n/04_Beta_vs_other_variables_plots_1n",

                                             "01_Beta_fit_plots/06_nFDpCD",

//                                                 "02_Beta_VS_P_plots",
                                             "02_Beta_VS_P_plots/00_All_e", "02_Beta_VS_P_plots/00_All_e/By_charge",

                                             "02_Beta_VS_P_plots/01_1e_cut", "02_Beta_VS_P_plots/01_1e_cut/By_charge",

                                             "02_Beta_VS_P_plots/02_1p", "02_Beta_VS_P_plots/02_1p/By_charge",

                                             "02_Beta_VS_P_plots/03_1n", "02_Beta_VS_P_plots/03_1n/By_charge",

                                             "02_Beta_VS_P_plots/04_2p", "02_Beta_VS_P_plots/04_2p/By_charge",

                                             "02_Beta_VS_P_plots/05_pFDpCD", "02_Beta_VS_P_plots/05_pFDpCD/By_charge",

                                             "02_Beta_VS_P_plots/06_nFDpCD", "02_Beta_VS_P_plots/06_nFDpCD/By_charge"
    };
    //</editor-fold>

    //<editor-fold desc="Angle plots directories">
    bool create_Angle_Dir = true;
    string Angle_Parent_Directory = "04_Angle_plots";
    vector <string> Angle_Daughter_Folders = {"",
//                                                  "00_All_e",
                                              "00_All_e/01_Theta_e_All_e_plots", "00_All_e/02_Phi_e_All_e_plots",
                                              "00_All_e/03_Theta_e_VS_Phi_e_All_e_plots",
//                                                  "00_All_e/03_Theta_neut_VS_Phi_neut_All_e_plots",

//                                                  "01_1e_cut",
                                              "01_1e_cut/01_Theta_e_1e_cut_plots", "01_1e_cut/02_Phi_e_1e_cut_plots",
                                              "01_1e_cut/03_Theta_e_VS_Phi_e_1e_cut_plots", "01_1e_cut/04_Neutron_veto_1e_cut_plots",

//                                                  "02_1p",
                                              "02_1p/01_Theta_e_1p_plots", "02_1p/02_Phi_e_1p_plots", "02_1p/03_Theta_e_VS_Phi_e_1p_plots",
                                              "02_1p/04_Theta_p_1p",
                                              "02_1p/05_Phi_p_1p", "02_1p/06_Theta_p_VS_Phi_p_1p_plots", "02_1p/07_Opening_angles_1p_plots",

//                                                  "03_1n",
                                              "03_1n/01_Theta_e_1n_plots", "03_1n/02_Phi_e_1n_plots", "03_1n/03_Theta_e_VS_Phi_e_1n_plots",
                                              "03_1n/04_Theta_n_1n", "03_1n/05_Phi_n_1n", "03_1n/06_Theta_n_VS_Phi_n_1n_plots",
                                              "03_1n/07_Opening_angles_1n_plots", "03_1n/08_Neutron_veto_1n_plots",

//                                                  "05_1e2p",
//                                                  "05_1e2p/01_Theta_e_1e2p_plots", "05_1e2p/02_Phi_e_1e2p_plots", "05_1e2p/03_Theta_e_VS_Phi_e_1e2p_plots",

//                                                  "04_2p",
                                              "04_2p/01_Theta_e_2p_plots", "04_2p/02_Phi_e_2p_plots", "04_2p/03_Theta_e_VS_Phi_e_2p_plots",
                                              "04_2p/04_Opening_angles_2p_plots",
                                              "04_2p/04_Opening_angles_2p_plots/00_Theta_p1_p2_by_interaction",
                                              "04_2p/05_Ghost_tracks_handling_2p_plots",
                                              "04_2p/05_Ghost_tracks_handling_2p_plots/01_CFoF_hits_2p_plots",
                                              "04_2p/05_Ghost_tracks_handling_2p_plots/01_CFoF_hits_2p_plots/00_CFoF_hits_monitoring_2p_plots",
                                              "04_2p/05_Ghost_tracks_handling_2p_plots/02_Double_detection_2p_plots",
                                              "04_2p/05_Ghost_tracks_handling_2p_plots/02_Double_detection_2p_plots/00_Double_detection_monitoring_2p_plots",

//                                                  "05_pFDpCD",
                                              "05_pFDpCD/01_Theta_e_pFDpCD_plots", "05_pFDpCD/02_Phi_e_pFDpCD_plots",
                                              "05_pFDpCD/03_Theta_e_VS_Phi_e_pFDpCD_plots",
                                              "05_pFDpCD/04_Theta_pFD_pFDpCD_plots", "05_pFDpCD/05_Phi_pFD_pFDpCD_plots",
                                              "05_pFDpCD/06_Theta_pFD_VS_Phi_pFD_pFDpCD_plots", "05_pFDpCD/07_Theta_pCD_pFDpCD_plots",
                                              "05_pFDpCD/08_Phi_pCD_pFDpCD_plots", "05_pFDpCD/09_Theta_pCD_VS_Phi_pCD_pFDpCD_plots",
                                              "05_pFDpCD/10_Theta_tot_pFDpCD_plots", "05_pFDpCD/11_Phi_tot_pFDpCD_plots",
                                              "05_pFDpCD/12_Theta_tot_VS_Phi_tot_pFDpCD_plots",
                                              "05_pFDpCD/13_Theta_rel_pFDpCD_plots", "05_pFDpCD/14_Phi_rel_pFDpCD_plots",
                                              "05_pFDpCD/15_Theta_rel_VS_Phi_rel_pFDpCD_plots",
                                              "05_pFDpCD/16_Opening_angles_pFDpCD_plots",
                                              "05_pFDpCD/16_Opening_angles_pFDpCD_plots/00_Theta_pFD_pCD_by_interaction",
                                              "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots",
                                              "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/01_CFoF_hits_pFDpCD_plots",
                                              "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/01_CFoF_hits_pFDpCD_plots/00_CFoF_hits_Monitoring_pFDpCD_plots",
                                              "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/02_Double_detection_pFDpCD_plots",
                                              "05_pFDpCD/17_Ghost_tracks_handling_pFDpCD_plots/02_Double_detection_pFDpCD_plots/00_Double_detection_monitoring_pFDpCD_plots",

//                                                  "06_nFDpCD",
                                              "06_nFDpCD/01_Theta_e_nFDpCD_plots", "06_nFDpCD/02_Phi_e_nFDpCD_plots",
                                              "06_nFDpCD/03_Theta_e_VS_Phi_e_nFDpCD_plots",
                                              "06_nFDpCD/04_Theta_nFD_nFDpCD_plots", "06_nFDpCD/05_Phi_nFD_nFDpCD_plots",
                                              "06_nFDpCD/06_Theta_nFD_VS_Phi_nFD_nFDpCD_plots", "06_nFDpCD/07_Theta_pCD_nFDpCD_plots",
                                              "06_nFDpCD/08_Phi_pCD_nFDpCD_plots", "06_nFDpCD/09_Theta_pCD_VS_Phi_pCD_nFDpCD_plots",
                                              "06_nFDpCD/10_Theta_tot_nFDpCD_plots", "06_nFDpCD/11_Phi_tot_nFDpCD_plots",
                                              "06_nFDpCD/12_Theta_tot_VS_Phi_tot_nFDpCD_plots",
                                              "06_nFDpCD/13_Theta_rel_nFDpCD_plots", "06_nFDpCD/14_Phi_rel_nFDpCD_plots",
                                              "06_nFDpCD/15_Theta_rel_VS_Phi_rel_nFDpCD_plots",
                                              "06_nFDpCD/16_Opening_angles_nFDpCD_plots",
                                              "06_nFDpCD/16_Opening_angles_nFDpCD_plots/00_Theta_nFD_pCD_by_interaction",
                                              "06_nFDpCD/17_Ghost_tracks_handling_nFDpCD_plots/02_Double_detection_nFDpCD_plots",
                                              "06_nFDpCD/17_Ghost_tracks_handling_nFDpCD_plots/02_Double_detection_nFDpCD_plots/00_Double_detection_monitoring_nFDpCD_plots",
                                              "06_nFDpCD/18_Neutron_veto_nFDpCD_plots",

//                                                  "07_1e2pXy",
                                              "07_1e2pXy/01_Theta_e_1e2pXy_plots", "07_1e2pXy/02_Phi_e_1e2pXy_plots",
                                              "07_1e2pXy/03_Theta_e_VS_Phi_e_1e2pXy_plots",
                                              "07_1e2pXy/04_Phi_Proton_1e2pXy_plots"
    };
    //</editor-fold>

    //<editor-fold desc="Momentum transfer plots directories">
    bool create_Momentum_transfer_Dir = true;
    string Momentum_transfer_Parent_Directory = "05_Momentum_transfer_plots";
    vector <string> Momentum_transfer_Daughter_Folders = {"",
                                           "00_All_e",
                                           "01_1e_cut",
                                           "02_1p",
                                           "03_1n",
                                           "04_2p",
                                           "05_pFDpCD",
                                           "06_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="E_e plots directories">
    bool create_E_e_Dir = true;
    string E_e_Parent_Directory = "06_E_e_plots";
    vector <string> E_e_Daughter_Folders = {"",
                                            "01_1e_cut",
                                            "01_1e_cut/E_e_plots_1e_cut", "01_1e_cut/E_e_VS_Theta_e_1e_cut",

                                            "02_1p",
                                            "02_1p/E_e_plots_1p", "02_1p/E_e_VS_Theta_e_1p", "02_1p/E_e_plots_1p/Around_15_deg_1p",

                                            "03_1n",
                                            "03_1n/E_e_plots_1n", "03_1n/E_e_VS_Theta_e_1n", "03_1n/E_e_plots_1n/Around_15_deg_1n",

                                            "04_2p",
                                            "04_2p/E_e_plots_2p", "04_2p/E_e_VS_Theta_e_2p", "04_2p/E_e_plots_2p/Around_15_deg_2p",

                                            "05_pFDpCD",
                                            "05_pFDpCD/E_e_plots_pFDpCD", "05_pFDpCD/E_e_VS_Theta_e_pFDpCD",
                                            "05_pFDpCD/E_e_plots_pFDpCD/Around_15_deg_pFDpCD",

                                            "06_nFDpCD",
                                            "06_nFDpCD/E_e_plots_nFDpCD", "06_nFDpCD/E_e_VS_Theta_e_nFDpCD",
                                            "06_nFDpCD/E_e_plots_nFDpCD/Around_15_deg_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="ETrans plots directories">
    bool create_ETrans_Dir = true;
    string ETrans_Parent_Directory = "07_ET_plots";
    vector <string> ETrans_Daughter_Folders = {"",
                                               "02_1p",
                                               "02_1p/02_Around_15_Deg_1p", "02_1p/02_Around_15_Deg_1p/00_All_interactions",
                                               "02_1p/02_Around_15_Deg_1p/01_QEL_only",
                                               "02_1p/02_Around_15_Deg_1p/02_MEC_only", "02_1p/02_Around_15_Deg_1p/03_RES_only",
                                               "02_1p/02_Around_15_Deg_1p/04_DIS_only", "02_1p/01_Every_theta_e_1p",
                                               "02_1p/01_Every_theta_e_1p/00_All_interactions",
                                               "02_1p/01_Every_theta_e_1p/01_QEL_only", "02_1p/01_Every_theta_e_1p/02_MEC_only",
                                               "02_1p/01_Every_theta_e_1p/03_RES_only", "02_1p/01_Every_theta_e_1p/04_DIS_only",

                                               "03_1n",
                                               "03_1n/02_Around_15_Deg_1n", "03_1n/02_Around_15_Deg_1n/00_All_interactions",
                                               "03_1n/02_Around_15_Deg_1n/01_QEL_only",
                                               "03_1n/02_Around_15_Deg_1n/02_MEC_only", "03_1n/02_Around_15_Deg_1n/03_RES_only",
                                               "03_1n/02_Around_15_Deg_1n/04_DIS_only", "03_1n/01_Every_theta_e_1n",
                                               "03_1n/01_Every_theta_e_1n/00_All_interactions",
                                               "03_1n/01_Every_theta_e_1n/01_QEL_only", "03_1n/01_Every_theta_e_1n/02_MEC_only",
                                               "03_1n/01_Every_theta_e_1n/03_RES_only", "03_1n/01_Every_theta_e_1n/04_DIS_only",

                                               "04_2p",
                                               "04_2p/02_Around_15_Deg_2p", "04_2p/02_Around_15_Deg_2p/00_All_interactions",
                                               "04_2p/02_Around_15_Deg_2p/01_QEL_only",
                                               "04_2p/02_Around_15_Deg_2p/02_MEC_only", "04_2p/02_Around_15_Deg_2p/03_RES_only",
                                               "04_2p/02_Around_15_Deg_2p/04_DIS_only", "04_2p/01_Every_theta_e_2p",
                                               "04_2p/01_Every_theta_e_2p/00_All_interactions",
                                               "04_2p/01_Every_theta_e_2p/01_QEL_only", "04_2p/01_Every_theta_e_2p/02_MEC_only",
                                               "04_2p/01_Every_theta_e_2p/03_RES_only", "04_2p/01_Every_theta_e_2p/04_DIS_only",

                                               "05_pFDpCD",
                                               "05_pFDpCD/02_Around_15_Deg_pFDpCD", "05_pFDpCD/02_Around_15_Deg_pFDpCD/00_All_interactions",
                                               "05_pFDpCD/02_Around_15_Deg_pFDpCD/01_QEL_only",
                                               "05_pFDpCD/02_Around_15_Deg_pFDpCD/02_MEC_only",
                                               "05_pFDpCD/02_Around_15_Deg_pFDpCD/03_RES_only",
                                               "05_pFDpCD/02_Around_15_Deg_pFDpCD/04_DIS_only",
                                               "05_pFDpCD/01_Every_theta_e_pFDpCD", "05_pFDpCD/01_Every_theta_e_pFDpCD/00_All_interactions",
                                               "05_pFDpCD/01_Every_theta_e_pFDpCD/01_QEL_only",
                                               "05_pFDpCD/01_Every_theta_e_pFDpCD/02_MEC_only",
                                               "05_pFDpCD/01_Every_theta_e_pFDpCD/03_RES_only",
                                               "05_pFDpCD/01_Every_theta_e_pFDpCD/04_DIS_only",

                                               "06_nFDpCD",
                                               "06_nFDpCD/02_Around_15_Deg_nFDpCD", "06_nFDpCD/02_Around_15_Deg_nFDpCD/00_All_interactions",
                                               "06_nFDpCD/02_Around_15_Deg_nFDpCD/01_QEL_only",
                                               "06_nFDpCD/02_Around_15_Deg_nFDpCD/02_MEC_only",
                                               "06_nFDpCD/02_Around_15_Deg_nFDpCD/03_RES_only",
                                               "06_nFDpCD/02_Around_15_Deg_nFDpCD/04_DIS_only",
                                               "06_nFDpCD/01_Every_theta_e_nFDpCD", "06_nFDpCD/01_Every_theta_e_nFDpCD/00_All_interactions",
                                               "06_nFDpCD/01_Every_theta_e_nFDpCD/01_QEL_only",
                                               "06_nFDpCD/01_Every_theta_e_nFDpCD/02_MEC_only",
                                               "06_nFDpCD/01_Every_theta_e_nFDpCD/03_RES_only",
                                               "06_nFDpCD/01_Every_theta_e_nFDpCD/04_DIS_only"
    };
    //</editor-fold>

    //<editor-fold desc="Ecal plots directories">
    bool create_Ecal_Dir = true;
    string Ecal_Parent_Directory = "08_Ecal_rec_plots";
    vector <string> Ecal_Daughter_Folders = {"",
                                             "02_1p",
                                             "02_1p/01_Ecal_rec_by_reaction_1p/00_All_interactions",
                                             "02_1p/01_Ecal_rec_by_reaction_1p/01_QEL_only", "02_1p/01_Ecal_rec_by_reaction_1p/02_MEC_only",
                                             "02_1p/01_Ecal_rec_by_reaction_1p/03_RES_only", "02_1p/01_Ecal_rec_by_reaction_1p/04_DIS_only",
                                             "02_1p/02_Ecal_rec_vs_Mom_1p", "02_1p/03_Ecal_rec_vs_Ang_1p",
                                             "02_1p/04_Ecal_rec_vs_TKI_1p", "02_1p/05_Chi2_for_Ecal_above_Ebeam_1p",

                                             "03_1n",
                                             "03_1n/01_Ecal_rec_by_reaction_1n/00_All_interactions",
                                             "03_1n/01_Ecal_rec_by_reaction_1n/01_QEL_only", "03_1n/01_Ecal_rec_by_reaction_1n/02_MEC_only",
                                             "03_1n/01_Ecal_rec_by_reaction_1n/03_RES_only", "03_1n/01_Ecal_rec_by_reaction_1n/04_DIS_only",
                                             "03_1n/02_Ecal_rec_vs_Mom_1n", "03_1n/03_Ecal_rec_vs_Ang_1n",
                                             "03_1n/04_Ecal_rec_vs_TKI_1n", "03_1n/05_Chi2_for_Ecal_above_Ebeam_1n",

                                             "04_2p",
                                             "04_2p/01_Ecal_rec_by_reaction_2p/00_All_interactions",
                                             "04_2p/01_Ecal_rec_by_reaction_2p/01_QEL_only",
                                             "04_2p/01_Ecal_rec_by_reaction_2p/02_MEC_only", "04_2p/01_Ecal_rec_by_reaction_2p/03_RES_only",
                                             "04_2p/01_Ecal_rec_by_reaction_2p/04_DIS_only", "04_2p/02_Ecal_rec_vs_TKI_2p",
                                             "04_2p/03_Chi2_for_Ecal_above_Ebeam_2p",

                                             "05_pFDpCD",
                                             "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/00_All_interactions",
                                             "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/01_QEL_only",
                                             "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/02_MEC_only",
                                             "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/03_RES_only",
                                             "05_pFDpCD/01_Ecal_rec_by_reaction_pFDpCD/04_DIS_only",
                                             "05_pFDpCD/02_Ecal_rec_vs_Mom_pFDpCD", "05_pFDpCD/03_Ecal_rec_vs_Ang_pFDpCD",
                                             "05_pFDpCD/04_Ecal_rec_vs_TKI_pFDpCD", "05_pFDpCD/05_Chi2_for_Ecal_above_Ebeam_pFDpCD",

                                             "06_nFDpCD",
                                             "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/00_All_interactions",
                                             "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/01_QEL_only",
                                             "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/02_MEC_only",
                                             "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/03_RES_only",
                                             "06_nFDpCD/01_Ecal_rec_by_reaction_nFDpCD/04_DIS_only",
                                             "06_nFDpCD/02_Ecal_rec_vs_Mom_nFDpCD", "06_nFDpCD/03_Ecal_rec_vs_Ang_nFDpCD",
                                             "06_nFDpCD/04_Ecal_rec_vs_TKI_nFDpCD", "06_nFDpCD/05_Chi2_for_Ecal_above_Ebeam_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="TKI plots directories">
    bool create_TKI_Dir = true;
    string TKI_Parent_Directory = "09_TKI_plots";
    vector <string> TKI_Daughter_Folders = {"",
                                            "03_1n",
                                            "03_1n/01_dP_T_1n", "03_1n/02_dAlpha_T_1n", "03_1n/03_dPhi_T_1n",

                                            "02_1p",
                                            "02_1p/01_dP_T_1p", "02_1p/02_dAlpha_T_1p", "02_1p/03_dPhi_T_1p",

                                            "04_2p",
                                            "04_2p/01_dP_T_2p", "04_2p/02_dAlpha_T_2p", "04_2p/03_dPhi_T_2p",

                                            "05_pFDpCD",
                                            "05_pFDpCD/01_dP_T_pFDpCD", "05_pFDpCD/02_dAlpha_T_pFDpCD", "05_pFDpCD/03_dPhi_T_pFDpCD",

                                            "06_nFDpCD",
                                            "06_nFDpCD/01_dP_T_nFDpCD", "06_nFDpCD/02_dAlpha_T_nFDpCD", "06_nFDpCD/03_dPhi_T_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="ToF plots directories">
    // todo: change this folder's name
    bool create_ToF_Dir = true;
    string ToF_Parent_Directory = "10_ToF_plots";
    vector <string> ToF_Daughter_Folders = {"",
                                            "01_1e_cut",
                                            "01_1e_cut/01_Neutron_vs_cParticles_hits_FTOF_1e_cut",
                                            "06_nFDpCD", "06_nFDpCD/01_Neutron_vs_cParticles_hits_FTOF_nFDpCD"
    };
    //</editor-fold>

    //<editor-fold desc="Efficiency & Acceptance Correction plots directories">
    bool create_Eff_and_ACorr_Dir = true;
    string Eff_and_ACorr_Parent_Directory = "11_Eff_and_ACorr_plots";
    vector <string> Eff_and_ACorr_Daughter_Folders = {"",
                                                      "01_1e_cut",
                                                      "01_1e_cut/01_Momentum_Eff_and_ACorr_plots_1e_cut",
                                                      "01_1e_cut/02_Theta_Eff_and_ACorr_plots_1e_cut",
                                                      "01_1e_cut/03_Phi_Eff_and_ACorr_plots_1e_cut",
                                                      "01_1e_cut/04_Hit_maps",
                                                      "01_1e_cut/05_Loaded_reco_ref_hit_maps",
                                                      "01_1e_cut/05_Eff_plots_for_Momentum_th",

                                                      "02_1p",
                                                      "02_1p/01_Momentum_Eff_and_ACorr_plots_1p", "02_1p/02_Theta_Eff_and_ACorr_plots_1p",
                                                      "02_1p/03_Phi_Eff_and_ACorr_plots_1p", "02_1p/04_Proton_FD_Hit_map_1p",

                                                      "03_1n",
                                                      "03_1n/01_Momentum_Eff_and_ACorr_plots_1n", "03_1n/02_Theta_Eff_and_ACorr_plots_1n",
                                                      "03_1n/03_Phi_Eff_and_ACorr_plots_1n", "03_1n/04_Neutron_FD_Hit_map_1n",

                                                      "04_2p",
                                                      "04_2p/01_Momentum_Eff_and_ACorr_plots_2p", "04_2p/02_Theta_Eff_and_ACorr_plots_2p",
                                                      "04_2p/03_Phi_Eff_and_ACorr_plots_2p", "04_2p/04_Proton_FD_Hit_map_2p",

                                                      "05_pFDpCD",
                                                      "05_pFDpCD/01_Momentum_Eff_and_ACorr_plots_pFDpCD",
                                                      "05_pFDpCD/02_Theta_Eff_and_ACorr_plots_pFDpCD",
                                                      "05_pFDpCD/03_Phi_Eff_and_ACorr_plots_pFDpCD", "05_pFDpCD/04_Proton_FD_Hit_map_pFDpCD",

                                                      "06_nFDpCD",
                                                      "06_nFDpCD/01_Momentum_Eff_and_ACorr_plots_nFDpCD",
                                                      "06_nFDpCD/02_Theta_Eff_and_ACorr_plots_nFDpCD",
                                                      "06_nFDpCD/03_Phi_Eff_and_ACorr_plots_nFDpCD", "06_nFDpCD/04_Neutron_FD_Hit_map_nFDpCD",
    };
    //</editor-fold>

    //<editor-fold desc="Acceptance maps plots directories">
    bool create_Acceptance_Maps_Dir = true;
    string Hit_Maps_Parent_Directory = "12_Acceptance_maps_plots";
    vector <string> Hit_Maps_Daughter_Folders = {"",
                                                 "01_AMaps_1e_cut",
                                                 "01_AMaps_1e_cut/00a_AMaps_BC_from_file",

                                                 "02_WMaps_1e_cut",
                                                 "02_WMaps_1e_cut/00a_WMaps_BC_from_file",
    };
    //</editor-fold>

    //<editor-fold desc="Resolution plots directories">
    bool create_Resolution_Dir = true;
    string Resolution_Parent_Directory = "13_Resolution_plots";
    vector <string> Resolution_Daughter_Folders = {"",
                                                   "02_1p",
                                                   "02_1p/01_Proton_resolution_plots_1p",
                                                   "02_1p/02_Match_monitoring_plots_1p",
                                                   "02_1p/03_Match_multiplicity_plots_1p",
                                                   "02_1p/04_Fit_justification_plots_1p",
                                                   "02_1p/04_Fit_justification_plots_1p/01_Basic_variables_1p",
                                                   "02_1p/04_Fit_justification_plots_1p/02_Correction_1p",
                                                   "02_1p/04_Fit_justification_plots_1p/03_Smearing_1p",

                                                   "03_1n",
                                                   "03_1n/01_Neutron_resolution_plots_1n",
                                                   "03_1n/02_Match_monitoring_plots_1n",
                                                   "03_1n/03_Match_multiplicity_plots_1n",
                                                   "03_1n/04_Fit_justification_plots_1n",
                                                   "03_1n/04_Fit_justification_plots_1n/01_Basic_variables_1n",
                                                   "03_1n/04_Fit_justification_plots_1n/02_Correction_1n",
                                                   "03_1n/04_Fit_justification_plots_1n/03_Smearing_1n",
    };
    //</editor-fold>

    //<editor-fold desc="Multiplicity plots directories">
    bool create_Multiplicity_Dir = true;
    string Multiplicity_Parent_Directory = "14_Multiplicity_plots";
    vector <string> Multiplicity_Daughter_Folders = {"",
                                                     "01_1e_cut",
                                                     "02_1p",
                                                     "03_1n",
                                                     "05_pFDpCD",
                                                     "06_nFDpCD",

    };
    //</editor-fold>

// Private methods ------------------------------------------------------------------------------------------------------------------------------------------------------

    bool FindSubstring(string string1, string string2);

    string Find(vector <string> vector, string string);

public:

    map <string, string> Nphe_Directory_map;
    map <string, string> Chi2_Directory_map;
    map <string, string> Vertex_Directory_map;
    map <string, string> SF_Directory_map;
    map <string, string> Fiducial_Directory_map;
    map <string, string> Momentum_Directory_map;

    map <string, string> W_Directory_map;
    map <string, string> Beta_Directory_map;
    map <string, string> Angle_Directory_map;
    map <string, string> Momentum_transfer_Directory_map;
    map <string, string> E_e_Directory_map;
    map <string, string> ETrans_Directory_map;
    map <string, string> Ecal_Directory_map;
    map <string, string> TKI_Directory_map;
    map <string, string> ToF_Directory_map;
    map <string, string> Eff_and_ACorr_Directory_map;
    map <string, string> AMaps_Directory_map;
    map <string, string> Resolution_Directory_map;
    map <string, string> Multiplicity_Directory_map;

// MakeDirectory function -----------------------------------------------------------------------------------------------------------------------------------------------

    void MakeDirectory(const bool &Create_Directory, const string &Plots_Parent_Folder, const string &Plots_Daughter_Folder,
                       const bool &Clear_Parent_Folder_content = false, const string &Parent_Folder = "./plots");

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    Directories(const string &plots_path = "./plots", const bool Clear_Old_Directories = true);

};

#endif //DIRECTORIES_H
