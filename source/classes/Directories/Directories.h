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
// Private attributes ---------------------------------------------------------------------------------------------------------------------------------------------------

    std::string Plots_Folder;

    //<editor-fold desc="Cut parameters plots directories">

    //<editor-fold desc="Number of Photo-electrons (Nphe) plots directories">
    bool create_Nphe_Dir = true;
    std::string Nphe_Parent_Directory = "01_Cuts_plots/01_Nphe_plots";
    vector<std::string> Nphe_Daughter_Folders = {"", "01_1e_cut", "02_1p", "03_2p", "04_1n"};
    //</editor-fold>

    //<editor-fold desc="Chi2 plots directories">
    bool create_chi2_Dir = true;
    std::string Chi2_Parent_Directory = "01_Cuts_plots/02_Chi2_plots";
    vector<std::string> Chi2_Daughter_Folders = {"", "01_All_e", "02_1e_cut", "03_1p", "04_1e2p_BC-AC_cut_tests", "05_2p", "06_1n"};
    //</editor-fold>

    //<editor-fold desc="Vertex plots directories">
    bool create_Vertex_Dir = true;
    std::string Vertex_Parent_Directory = "01_Cuts_plots/03_Vertex_plots";
    vector<std::string> Vertex_Daughter_Folders = {"",
                                                   "02_dV_plots/01_1p", "02_dV_plots/02_1e2p", "02_dV_plots/03_2p",

                                                   "01_Vertex_components_plots/01_1e_cut/01_Electrons", "01_Vertex_components_plots/01_1e_cut/02_Protons",
                                                   "01_Vertex_components_plots/01_1e_cut/03_Kplus", "01_Vertex_components_plots/01_1e_cut/04_Kminus",
                                                   "01_Vertex_components_plots/01_1e_cut/05_Piplus", "01_Vertex_components_plots/01_1e_cut/06_Piminus",
                                                   "02_dV_plots/04_1n"};
    //</editor-fold>

    //<editor-fold desc="Sampling Fraction (SF) plots directories">
    bool create_SF_Dir = true;
    std::string SF_Parent_Directory = "01_Cuts_plots/04_SF_plots";
    vector<std::string> SF_Daughter_Folders = {"",
                                               "01_1e_cut", "01_1e_cut/SF_plots", "01_1e_cut/SF_VS_P_e_plots",
                                               "02_1p", "02_1p/SF_plots", "02_1p/SF_VS_P_e_plots",
                                               "03_2p", "03_2p/SF_plots", "03_2p/SF_VS_P_e_plots",
                                               "04_1n", "04_1n/SF_plots", "04_1n/SF_VS_P_e_plots"};
    //</editor-fold>

    //<editor-fold desc="Edge cuts histograms plots directories">
    bool create_fiducial_Dir = true;
    std::string fiducial_Parent_Directory = "01_Cuts_plots/05_Edge_cuts_plots";
    vector<std::string> fiducial_Daughter_Folders = {"",
                                                     "01_1e_cut", "01_1e_cut/PCAL",
                                                     "02_1p", "02_1p/PCAL",
                                                     "03_2p", "03_2p/PCAL",
                                                     "04_1n", "04_1n/PCAL"};
    //</editor-fold>

    //<editor-fold desc="Momentum threshold plots directories">
    bool create_Momentum_th_Dir = true;
    std::string Momentum_th_Parent_Directory = "01_Cuts_plots/06_Momentum_th_plots";
    vector<std::string> Momentum_th_Daughter_Folders = {"",
                                                        "01_1e_cut",
                                                        "02_1p",
                                                        "03_1e2p",
                                                        "04_2p",
                                                        "05_1n"};
    //</editor-fold>

    //</editor-fold>

    //<editor-fold desc="Momentum plots directories">
    bool create_Momentum_cut_Dir = true;
    std::string Momentum_cut_Parent_Directory = "02_Momentum_cut_plots";
    vector<std::string> Momentum_cut_Daughter_Folders = {"",
                                                         "01_1e_cut",
                                                         "01_1e_cut/01_Electrons_1e_cut", "01_1e_cut/02_Protons_1e_cut", "01_1e_cut/03_Kplus_1e_cut",
                                                         "01_1e_cut/04_Kminus_1e_cut", "01_1e_cut/05_Piplus_1e_cut", "01_1e_cut/06_Piminus_1e_cut",
                                                         "01_1e_cut/07_Neutrons_1e_cut", "01_1e_cut/08_Photons_1e_cut",

                                                         "02_1p",
                                                         "02_1p/01_Electrons_1p", "02_1p/02_Protons_1p", "02_1p/03_Kplus_1p", "02_1p/04_Kminus_1p", "02_1p/05_Piplus_1p",
                                                         "02_1p/06_Piminus_1p", "02_1p/07_Neutrons_1p", "02_1p/08_Photons_1p",

                                                         "03_1n",
                                                         "03_1n/01_Electrons_1n", "03_1n/02_Protons_1n", "03_1n/03_Kplus_1n", "03_1n/04_Kminus_1n", "03_1n/05_Piplus_1n",
                                                         "03_1n/06_Piminus_1n", "03_1n/07_Neutrons_1n", "03_1n/08_Photons_1n",

                                                         "04_1n1p",
                                                         "04_1n1p/01_Electrons_1n1p", "04_1n1p/02_Protons_1n1p", "04_1n1p/03_Kplus_1n1p", "04_1n1p/04_Kminus_1n1p",
                                                         "04_1n1p/05_Piplus_1n1p", "04_1n1p/06_Piminus_1n1p", "04_1n1p/07_Neutrons_1n1p", "04_1n1p/08_Photons_1n1p",

                                                         "05_1e2p",
                                                         "05_1e2p/01_Electrons_1e2p", "05_1e2p/02_Protons_1e2p", "05_1e2p/03_Kplus_1e2p", "05_1e2p/04_Kminus_1e2p",
                                                         "05_1e2p/05_Piplus_1e2p", "05_1e2p/06_Piminus_1e2p", "05_1e2p/07_Neutrons_1e2p", "05_1e2p/08_Photons_1e2p",

                                                         "06_2p",
                                                         "06_2p/01_Electrons_1e2p", "06_2p/02_Protons_1e2p", "06_2p/03_Kplus_1e2p", "06_2p/04_Kminus_1e2p",
                                                         "06_2p/05_Piplus_1e2p", "06_2p/06_Piminus_1e2p", "06_2p/07_Neutrons_1e2p", "06_2p/08_Photons_1e2p"};

    //</editor-fold>

    //<editor-fold desc="Beta plots directories">
    bool create_Beta_Dir = true;
    std::string Beta_Parent_Directory = "03_Beta_plots";
    vector<std::string> Beta_Daughter_Folders = {"",
                                                 "01_Beta_plots",
                                                 "01_Beta_plots/05_1n",
                                                 "01_Beta_plots/05_1n/01_Neutron_beta_from_fPhotons_plots_1n",
                                                 "01_Beta_plots/05_1n/01_Neutron_beta_from_fPhotons_plots_1n/01_fPhotons_All_plots_1n",
                                                 "01_Beta_plots/05_1n/01_Neutron_beta_from_fPhotons_plots_1n/02_fPhotons_noPCAL_hit_plots_1n",
                                                 "01_Beta_plots/05_1n/01_Neutron_beta_from_fPhotons_plots_1n/03_fPhotons_noPCAL_yesECIN_plots_1n",
                                                 "01_Beta_plots/05_1n/01_Neutron_beta_from_fPhotons_plots_1n/04_fPhotons_noPCAL_noECIN_yesECOUT_plots_1n",

                                                 "01_Beta_plots/05_1n/02_All_FD_neutrals_beta_plots_1n",
                                                 "01_Beta_plots/05_1n/02_All_FD_neutrals_beta_plots_1n/01_All_FD_neutrals_All_plots_1n",
                                                 "01_Beta_plots/05_1n/02_All_FD_neutrals_beta_plots_1n/02_All_FD_neutrals_noPCAL_hit_plots_1n",
                                                 "01_Beta_plots/05_1n/02_All_FD_neutrals_beta_plots_1n/03_All_FD_neutrals_noPCAL_yesECIN_plots_1n",
                                                 "01_Beta_plots/05_1n/02_All_FD_neutrals_beta_plots_1n/04_All_FD_neutrals_noPCAL_noECIN_yesECOUT_plots_1n",

                                                 "01_Beta_plots/05_1n/03_FD_neutrals_beta_noPDG0_plots_1n",
                                                 "01_Beta_plots/05_1n/03_FD_neutrals_beta_noPDG0_plots_1n/01_FD_neutrals_noPDG0_All_plots_1n",
                                                 "01_Beta_plots/05_1n/03_FD_neutrals_beta_noPDG0_plots_1n/02_FD_neutrals_noPDG0_noPCAL_hit_plots_1n",
                                                 "01_Beta_plots/05_1n/03_FD_neutrals_beta_noPDG0_plots_1n/03_FD_neutrals_noPDG0_noPCAL_yesECIN_plots_1n",
                                                 "01_Beta_plots/05_1n/03_FD_neutrals_beta_noPDG0_plots_1n/04_FD_neutrals_noPDG0_noPCAL_noECIN_yesECOUT_plots_1n",


                                                 "01_Beta_plots/06_1n1p",

                                                 "02_Beta_VS_P_plots", "02_Beta_VS_P_plots/01_All_e", "02_Beta_VS_P_plots/01_All_e/By_charge",
                                                 "02_Beta_VS_P_plots/02_1e_cut", "02_Beta_VS_P_plots/02_1e_cut/By_charge", "02_Beta_VS_P_plots/04_1p",
                                                 "02_Beta_VS_P_plots/04_1p/By_charge", "02_Beta_VS_P_plots/05_1n", "02_Beta_VS_P_plots/05_1n/By_charge",
                                                 "02_Beta_VS_P_plots/06_1n1p", "02_Beta_VS_P_plots/06_1n1p/By_charge", "02_Beta_VS_P_plots/07_2p",
                                                 "02_Beta_VS_P_plots/07_2p/By_charge"};
    //</editor-fold>

    //<editor-fold desc="Angle plots directories">
    bool create_Angle_Dir = true;
    std::string Angle_Parent_Directory = "04_Angle_plots";
    vector<std::string> Angle_Daughter_Folders = {"",
                                                  "01_All_e", "01_All_e/01_Theta_e_All_e_plots", "01_All_e/02_Phi_e_All_e_plots",
                                                  "01_All_e/03_Theta_e_VS_Phi_e_All_e_plots", "01_All_e/03_Theta_neut_VS_Phi_neut_All_e_plots",

                                                  "02_1e_cut", "02_1e_cut/01_Theta_e_1e_cut_plots", "02_1e_cut/02_Phi_e_1e_cut_plots",
                                                  "02_1e_cut/03_Theta_e_VS_Phi_e_1e_cut_plots", "02_1e_cut/04_Neutron_Veto_1e_cut_plots",

                                                  "03_1p", "03_1p/01_Theta_e_1p_plots", "03_1p/02_Phi_e_1p_plots", "03_1p/03_Theta_e_VS_Phi_e_1p_plots",
                                                  "03_1p/04_Opening_angles_1p_plots", "03_1p/05_Theta_p_1p", "03_1p/06_Phi_p_1p", "03_1p/07_Theta_p_VS_Phi_p_1p_plots",

                                                  "05_1e2p", "05_1e2p/01_Theta_e_1e2p_plots", "05_1e2p/02_Phi_e_1e2p_plots", "05_1e2p/03_Theta_e_VS_Phi_e_1e2p_plots",

                                                  "06_2p", "06_2p/01_Theta_e_2p_plots", "06_2p/02_Phi_e_2p_plots", "06_2p/03_Theta_e_VS_Phi_e_2p_plots",
                                                  "06_2p/04_Opening_angles_2p_plots", "06_2p/04_Opening_angles_2p_plots/01_Theta_p1_p2_by_interaction",
                                                  "06_2p/05_Ghost_tracks_handling_2p_plots",

                                                  "07_1e2pXy", "07_1e2pXy/01_Theta_e_1e2pXy_plots", "07_1e2pXy/02_Phi_e_1e2pXy_plots",
                                                  "07_1e2pXy/03_Theta_e_VS_Phi_e_1e2pXy_plots", "07_1e2pXy/04_Phi_Proton_1e2pXy_plots",

                                                  "08_1n", "08_1n/01_Theta_e_1n_plots", "08_1n/02_Phi_e_1n_plots", "08_1n/03_Theta_e_VS_Phi_e_1n_plots",
                                                  "08_1n/04_Opening_angles_1n_plots", "08_1n/05_Theta_n_1n", "08_1n/06_Phi_n_1n", "08_1n/07_Theta_n_VS_Phi_n_1n_plots",
                                                  "08_1n/07_Theta_n_VS_Phi_n_1n_plots", "08_1n/08_Neutron_Veto_1n_plots",

                                                  "09_1n1p", "09_1n1p/01_Theta_e_1n1p_plots", "09_1n1p/02_Phi_e_1n1p_plots", "09_1n1p/03_Theta_e_VS_Phi_e_1n1p_plots",
                                                  "09_1n1p/04_Opening_angles_1n1p_plots", "09_1n1p/05_Theta_n_1n1p", "09_1n1p/06_Phi_n_1n1p",
                                                  "09_1n1p/07_Theta_n_VS_Phi_n_1n1p_plots", "09_1n1p/07_Theta_n_VS_Phi_n_1n1p_plots",
                                                  "09_1n1p/08_Neutron_Veto_1n1p_plots"};
    //</editor-fold>

    //<editor-fold desc="Q2 plots directories">
    bool create_Q2_Dir = true;
    std::string Q2_Parent_Directory = "05_Q2_plots";
    vector<std::string> Q2_Daughter_Folders = {"",
                                               "01_All_e",
                                               "02_1e_cut",
                                               "03_1p",
                                               "04_1e2p",
                                               "05_2p",
                                               "06_1n"};
    //</editor-fold>

    //<editor-fold desc="E_e plots directories">
    bool create_E_e_Dir = true;
    std::string E_e_Parent_Directory = "06_E_e_plots";
    vector<std::string> E_e_Daughter_Folders = {"",
                                                "01_1e_cut", "01_1e_cut/E_e_plots_1e_cut", "01_1e_cut/E_e_VS_Theta_e_1e_cut",
                                                "02_1p", "02_1p/E_e_plots_1p", "02_1p/E_e_VS_Theta_e_1p", "02_1p/E_e_plots_1p/Around_15_deg_1p",
                                                "03_2p", "03_2p/E_e_plots_2p", "03_2p/E_e_VS_Theta_e_2p", "03_2p/E_e_plots_2p/Around_15_deg_2p",
                                                "04_1n", "04_1n/E_e_plots_1n", "04_1n/E_e_VS_Theta_e_1n", "04_1n/E_e_plots_1n/Around_15_deg_1n"};
    //</editor-fold>

    //<editor-fold desc="ETrans plots directories">
    bool create_ETrans_Dir = true;
    std::string ETrans_Parent_Directory = "07_ET_plots";
    vector<std::string> ETrans_Daughter_Folders = {"",
                                                   "02_2p", "02_2p/02_Around_15_Deg_2p", "02_2p/02_Around_15_Deg_2p/00_All_interactions",
                                                   "02_2p/02_Around_15_Deg_2p/01_QEL_only", "02_2p/02_Around_15_Deg_2p/02_MEC_only",
                                                   "02_2p/02_Around_15_Deg_2p/03_RES_only", "02_2p/02_Around_15_Deg_2p/04_DIS_only", "02_2p/01_Every_theta_e_2p",

                                                   "02_2p/01_Every_theta_e_2p/00_All_interactions", "02_2p/01_Every_theta_e_2p/01_QEL_only",
                                                   "02_2p/01_Every_theta_e_2p/02_MEC_only", "02_2p/01_Every_theta_e_2p/03_RES_only",
                                                   "02_2p/01_Every_theta_e_2p/04_DIS_only",

                                                   "01_1p", "01_1p/02_Around_15_Deg_1p", "01_1p/02_Around_15_Deg_1p/00_All_interactions",
                                                   "01_1p/02_Around_15_Deg_1p/01_QEL_only", "01_1p/02_Around_15_Deg_1p/02_MEC_only",
                                                   "01_1p/02_Around_15_Deg_1p/03_RES_only", "01_1p/02_Around_15_Deg_1p/04_DIS_only", "01_1p/01_Every_theta_e_1p",
                                                   "01_1p/01_Every_theta_e_1p/00_All_interactions", "01_1p/01_Every_theta_e_1p/01_QEL_only",
                                                   "01_1p/01_Every_theta_e_1p/02_MEC_only", "01_1p/01_Every_theta_e_1p/03_RES_only",
                                                   "01_1p/01_Every_theta_e_1p/04_DIS_only",

                                                   "03_1n", "03_1n/02_Around_15_Deg_1n", "03_1n/02_Around_15_Deg_1n/00_All_interactions",
                                                   "03_1n/02_Around_15_Deg_1n/01_QEL_only", "03_1n/02_Around_15_Deg_1n/02_MEC_only",
                                                   "03_1n/02_Around_15_Deg_1n/03_RES_only", "03_1n/02_Around_15_Deg_1n/04_DIS_only", "03_1n/01_Every_theta_e_1n",
                                                   "03_1n/01_Every_theta_e_1n/00_All_interactions", "03_1n/01_Every_theta_e_1n/01_QEL_only",
                                                   "03_1n/01_Every_theta_e_1n/02_MEC_only", "03_1n/01_Every_theta_e_1n/03_RES_only",
                                                   "03_1n/01_Every_theta_e_1n/04_DIS_only"};
    //</editor-fold>

    //<editor-fold desc="Ecal plots directories">
    bool create_Ecal_Dir = true;
    std::string Ecal_Parent_Directory = "08_Ecal_rec_plots";
    vector<std::string> Ecal_Daughter_Folders = {"",
                                                 "02_2p", "02_2p/01_Ecal_rec_by_reaction_2p/00_All_interactions", "02_2p/01_Ecal_rec_by_reaction_2p/01_QEL_only",
                                                 "02_2p/01_Ecal_rec_by_reaction_2p/02_MEC_only", "02_2p/01_Ecal_rec_by_reaction_2p/03_RES_only",
                                                 "02_2p/01_Ecal_rec_by_reaction_2p/04_DIS_only", "02_2p/02_Ecal_rec_vs_transverse_variables_2p",
                                                 "02_2p/03_Chi2_for_Ecal_above_Ebeam_2p",

                                                 "01_1p", "01_1p/01_Ecal_rec_by_reaction_1p/00_All_interactions",
                                                 "01_1p/01_Ecal_rec_by_reaction_1p/01_QEL_only", "01_1p/01_Ecal_rec_by_reaction_1p/02_MEC_only",
                                                 "01_1p/01_Ecal_rec_by_reaction_1p/03_RES_only", "01_1p/01_Ecal_rec_by_reaction_1p/04_DIS_only",
                                                 "01_1p/02_Ecal_rec_vs_transverse_variables_1p", "01_1p/03_Chi2_for_Ecal_above_Ebeam_1p",

                                                 "03_1n", "03_1n/01_Ecal_rec_by_reaction_1n/00_All_interactions",
                                                 "03_1n/01_Ecal_rec_by_reaction_1n/01_QEL_only", "03_1n/01_Ecal_rec_by_reaction_1n/02_MEC_only",
                                                 "03_1n/01_Ecal_rec_by_reaction_1n/03_RES_only", "03_1n/01_Ecal_rec_by_reaction_1n/04_DIS_only",
                                                 "03_1n/02_Ecal_rec_vs_transverse_variables_1n", "03_1n/03_Chi2_for_Ecal_above_Ebeam_1n"};
    //</editor-fold>

    //<editor-fold desc="Transverse variables plots directories">
    bool create_TVariables_Dir = true;
    std::string TVariables_Parent_Directory = "09_Transverse_variable_plots";
    vector<std::string> TVariables_Daughter_Folders = {"",
                                                       "02_2p", "02_2p/01_dP_T_2p", "02_2p/02_dAlpha_T_2p", "02_2p/03_dPhi_T_2p",
                                                       "01_1p", "01_1p/01_dP_T_1p", "01_1p/02_dAlpha_T_1p", "01_1p/03_dPhi_T_1p",
                                                       "03_1n", "03_1n/01_dP_T_1n", "03_1n/02_dAlpha_T_1n", "03_1n/03_dPhi_T_1n"};
    //</editor-fold>

    //<editor-fold desc="ToF plots directories">
    // todo: change this folder's name
    bool create_ToF_Dir = true;
    std::string ToF_Parent_Directory = "10_ToF_plots";
    vector<std::string> ToF_Daughter_Folders = {"",
                                                "01_1e_cut", "01_1e_cut/01_Neutron_vs_cParticles_hits_FTOF_1e_cut",
                                                "02_1n1p", "02_1n1p/01_Neutron_vs_cParticles_hits_FTOF_1n1p"};
    //</editor-fold>

    //<editor-fold desc="Efficiency plots directories">
    // todo: change this folder's name
    bool create_Efficiency_Dir = true;
    std::string Efficiency_Parent_Directory = "11_Efficiency_plots";
    vector<std::string> Efficiency_Daughter_Folders = {"", "01_1e_cut", "01_1e_cut/01_Momentum_efficiency_plots_1e_cut", "01_1e_cut/02_Theta_efficiency_plots_1e_cut",
                                                       "01_1e_cut/03_Phi_efficiency_plots_1e_cut",

                                                       "02_1p", "02_1p/01_Momentum_efficiency_plots_1p", "02_1p/02_Theta_efficiency_plots_1p",
                                                       "02_1p/03_Phi_efficiency_plots_1p",

                                                       "03_1n", "03_1n/01_Momentum_efficiency_plots_1n", "03_1n/02_Theta_efficiency_plots_1n",
                                                       "03_1n/03_Phi_efficiency_plots_1n",

                                                       "04_1n1p", "04_1n1p/01_Momentum_efficiency_plots_1n1p", "04_1n1p/02_Theta_efficiency_plots_1n1p",
                                                       "04_1n1p/03_Phi_efficiency_plots_1n1p",

                                                       "05_1e2p", "05_1e2p/01_Momentum_efficiency_plots_1e2p", "05_1e2p/02_Theta_efficiency_plots_1e2p",
                                                       "05_1e2p/03_Phi_efficiency_plots_1e2p",

                                                       "06_2p", "06_2p/01_Momentum_efficiency_plots_2p", "06_2p/02_Theta_efficiency_plots_2p",
                                                       "06_2p/03_Phi_efficiency_plots_2p",

                                                       "07_1p_1n_efficiency_comparison"};
    //</editor-fold>

// Private methods ------------------------------------------------------------------------------------------------------------------------------------------------------

    bool FindSubstring(std::string string1, std::string string2);

    std::string Find(vector<std::string> vector, std::string string);

public:

    map<std::string, std::string> Nphe_Directory_map;
    map<std::string, std::string> Chi2_Directory_map;
    map<std::string, std::string> Vertex_Directory_map;
    map<std::string, std::string> SF_Directory_map;
    map<std::string, std::string> Fiducial_Directory_map;
    map<std::string, std::string> Momentum_th_Directory_map;

    map<std::string, std::string> Momentum_cut_Directory_map;
    map<std::string, std::string> Beta_Directory_map;
    map<std::string, std::string> Angle_Directory_map;
    map<std::string, std::string> Q2_Directory_map;
    map<std::string, std::string> E_e_Directory_map;
    map<std::string, std::string> ETrans_Directory_map;
    map<std::string, std::string> Ecal_Directory_map;
    map<std::string, std::string> TVariables_Directory_map;
    map<std::string, std::string> ToF_Directory_map;
    map<std::string, std::string> Efficiency_Directory_map;

// MakeDirectory function -----------------------------------------------------------------------------------------------------------------------------------------------

    void MakeDirectory(bool Create_Directory, std::string Plots_Parent_Folder, std::string Plots_Daughter_Folder, bool Clear_Parent_Folder_content = false,
                       std::string Parent_Folder = "./plots");

// Constructor ----------------------------------------------------------------------------------------------------------------------------------------------------------

    Directories(std::string plots_path = "./plots");
};

#endif //DIRECTORIES_H
